
#include "Algorithm/Inpaint/InpaintManager.h"
#include "Algorithm/Inpaint/defineall.h"
#include "Algorithm/Common/glog.h"
#include "Algorithm/Inpaint/inpaint/patch_match.h"
#include "Algorithm/Inpaint/inpaint/criminisi_inpainter.h"
#include "MedicalAnalysisSDK.h"

#include "opencv/cv.h"
#include "opencv2/opencv.hpp"

namespace SYY {
namespace Inpainting {

	InpaintManager::InpaintManager()
		:m_nMode(0)
	{
	}

	SYY::ErrorCode InpaintManager::Init(unsigned long nMode)
	{
		m_nMode = nMode;
		return SYY_NO_ERROR;
	}

	SYY::ErrorCode InpaintManager::ExecuteInpaint(
		const cv::Mat& srcImg, const cv::Mat& maskImg, cv::Mat& inpaintImg)
	{
		if (srcImg.empty() || maskImg.empty())
		{
			GLOG("Error: srcImg or maskImg is empty! srcImg: %d, maskImg: %d\n", srcImg.empty(), maskImg.empty());
			return SYY_SYS_ERROR;
		}
		if (maskImg.channels() != 1)
		{
			GLOG("Error: maskImg channels is not equal to 1! maskImg channel is: %d\n", maskImg.channels());
			return SYY_SYS_ERROR;
		}

		bool res = false;
		if (m_nMode & InpaintMode::PatchMatch)
		{
			res = PatchMatch_v1(srcImg, maskImg, inpaintImg);
		}
		else if (m_nMode & InpaintMode::Criminisi)
		{
			res = Criminisi(srcImg, maskImg, inpaintImg);
		}

		if (false == res)
		{
			GLOG("Error: Inpaint func fail! mode is %d\n", m_nMode);
			return SYY_SYS_ERROR;
		}

		return SYY_NO_ERROR;
	}

	void InpaintManager::Release()
	{

	}

	bool InpaintManager::PatchMatch_v1(const cv::Mat& srcImg, const cv::Mat& maskImg, cv::Mat& inpaintImg)
	{
		IplImage *maskimage = NULL, *output_img = NULL;

		uchar* data;
		CvSize size;
		double height, width;

		IplImage src_img = (IplImage)srcImg;
		IplImage mask_img = (IplImage)maskImg;

		IplImage* input_img = &src_img;
		IplImage* distorted = &mask_img;

		size = cvGetSize(input_img);
		height = size.height;
		width = size.width;

		//Mask computation
		maskimage = cvCreateImage(size, 8, 1);
		cvZero(maskimage);

		if (maskimage->nChannels == 3)
			cvCvtColor(distorted, maskimage, CV_BGR2GRAY);
		else if (maskimage->nChannels == 1)
			maskimage = cvCloneImage(distorted);
		else {
			return false;
		}

		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				if (cvGet2D(maskimage, i, j).val[0] != 255)
				{
					cvSet2D(maskimage, i, j, cvScalar(0, 0, 0, 0));
				}
			}
		}

		// generate mask array from mask image
		int channels = maskimage->nChannels;
		int step = maskimage->widthStep / sizeof(uchar);
		int ** mask;

		mask = (int **)malloc(int(height) * sizeof(int*));

		for (int i = 0; i < height; i++)
		{
			mask[i] = (int *)malloc(int(width) * sizeof(int));
			for (int j = 0; j < width; j++)
			{
				mask[i][j] = 0;
			}
		}

		data = (uchar *)maskimage->imageData;

		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				if (data[i*step + j*channels] == 255)
				{
					mask[i][j] = 1;
				}
			}
		}

		Inpaint_P inp = initInpaint();
		output_img = inpaint(inp, input_img, (int**)mask, 1, false);
		freeInpaintingPyramid(inp);

		inpaintImg = cv::cvarrToMat(output_img).clone();

		cvReleaseImage(&output_img);
		cvReleaseImage(&maskimage);

		for (int i = 0; i < height; ++i)
		{
			free(mask[i]);
		}

		free(mask);

		return true;
	}

	bool InpaintManager::PatchMatch_v2(const cv::Mat& srcImg, const cv::Mat& maskImg, cv::Mat& inpaintImg)
	{
		cv::Mat corrs, distances, progress;
		progress.create(srcImg.size(), srcImg.type());

		cv::Mat target = srcImg.clone();

		for (int y = 0; y < target.rows; ++y) {
			cv::Vec3b *cRow = target.ptr<cv::Vec3b>(y);
			const uchar *iRow = maskImg.ptr<uchar>(y);
			for (int x = 0; x < target.cols; ++x) {
				if (iRow[x] != 255)
					continue;

				cRow[x] = cv::Vec3b(0, 0, 0);
			}
		}

		//cv::imshow("target", target);
		//cv::waitKey();

		Inpaint::patchMatch(srcImg, target, cv::noArray(), corrs, distances, 3, 0);
		const int iter_count = 1;

		for (size_t i = 0; i < iter_count; ++i) {
			Inpaint::patchMatch(srcImg, target, cv::noArray(), corrs, distances, 3, 2);
		}
		for (int y = 0; y < srcImg.rows; ++y) {
			cv::Vec2i *cRow = corrs.ptr<cv::Vec2i>(y);
			cv::Vec3b *iRow = progress.ptr<cv::Vec3b>(y);
			for (int x = 0; x < srcImg.cols; ++x) {
				iRow[x] = target.at<cv::Vec3b>(cRow[x][1], cRow[x][0]);
			}
		}

		//cv::imshow("progress", progress);
		//cv::waitKey(1);

		inpaintImg = progress.clone();
		return true;
	}

	bool InpaintManager::Criminisi(const cv::Mat& srcImg, const cv::Mat& maskImg, cv::Mat& inpaintImg)
	{
		Inpaint::CriminisiInpainter inpainter;

		auto targetMask = maskImg.clone();
		cv::Mat sourceMask = cv::Mat::zeros(targetMask.size(), CV_8UC1);

		for (int r = 0; r < targetMask.rows; r++)
		{
			auto t_ptr = targetMask.ptr(r);
			auto s_ptr = sourceMask.ptr(r);
			for (int c = 0; c < targetMask.cols; c++)
			{
				if (t_ptr[c] != 255)
				{
					t_ptr[c] = 0;
					s_ptr[c] = 255;
				}

			}
		}

		const int patchSize = 5;

		inpainter.setSourceImage(srcImg);
		inpainter.setTargetMask(targetMask);
		//inpainter.setSourceMask(sourceMask);
		inpainter.setPatchSize(patchSize);
		inpainter.initialize();

		while (inpainter.hasMoreSteps()) {
			inpainter.step();
		}

		inpaintImg = inpainter.image().clone();

		return true;
	}

}
}
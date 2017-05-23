
#include "Algorithm/Inpaint/InpaintManager.h"
#include "Algorithm/Inpaint/defineall.h"

#include "opencv/cv.h"

namespace SYY {
namespace Inpaint {

	InpaintManager::InpaintManager()
	{
	}

	SYY::ErrorCode InpaintManager::Init()
	{
		return SYY_NO_ERROR;
	}

	SYY::ErrorCode InpaintManager::ExecuteInpaint(
		const cv::Mat& srcImg, const cv::Mat& maskImg, cv::Mat& inpaintImg)
	{
		IplImage* input_img = NULL, *maskimage = NULL, *distorted = NULL,
			*output_img = NULL, *input_gray = NULL, *distorted_gray = NULL;

		uchar* data;
		CvSize size;
		double height, width;

		IplImage src_img = (IplImage)srcImg;
		IplImage mask_img = (IplImage)maskImg;

		input_img = &src_img;
		distorted = &mask_img;

		size = cvGetSize(input_img);
		height = size.height;
		width = size.width;

		//Mask computation
		maskimage = cvCreateImage(size, 8, 1);
		cvZero(maskimage);

		input_gray = cvCreateImage(size, 8, 1);
		cvZero(input_gray);

		distorted_gray = cvCreateImage(size, 8, 1);
		cvZero(distorted_gray);

		cvCvtColor(distorted, maskimage, CV_BGR2GRAY);

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
		cvReleaseImage(&input_gray);
		cvReleaseImage(&distorted_gray);
		cvReleaseImage(&maskimage);

		for (int i = 0; i < height; ++i)
		{
			free(mask[i]);
		}

		free(mask);

		return SYY_NO_ERROR;
	}

	void InpaintManager::Release()
	{

	}

}
}
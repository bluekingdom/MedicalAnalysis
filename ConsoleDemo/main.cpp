#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <fstream>

#include "MedicalAnalysisSDK.h"


using namespace SYY;
using namespace SYY::MedicalAnalysis;
using namespace SYY::Inpainting;
using namespace SYY::VideoUtils;

void test_BUAnalysis()
{
	ErrorCode res;

	HANDLE handle;
	res = InitBUAnalysisWithMode(handle, BUAnalysisMode::Crop_V1 | BUAnalysisMode::DetectFast);
	if (res != SYY_NO_ERROR)
		return;

	std::string fileName = "E:\\data\\»ÈœŸ∞©Õº∆¨\\5¿‡\\≈Àπ‚√Ù\\1.2.826.0.1.3680043.2.461.8921672.1968559331.jpg";

	cv::Mat img = cv::imread(fileName);

	Image image;
	image.nWidth = img.cols;
	image.nHeight = img.rows;
	image.nChannels = img.channels();
	image.pData = new char[image.nWidth * image.nHeight * image.nChannels];
	memcpy(image.pData, img.data, img.step[0] * img.rows);

	std::ifstream imgFile(fileName, std::ios::binary | std::ios::ate);
	std::streamsize size = imgFile.tellg();
	imgFile.seekg(0, std::ios::beg);

	char* pFileData = new char[size];
	imgFile.read(pFileData, size);

	BUAnalysisResult result;
	//res = ExecuteBUAnalysis(handle, (char* )img.data, img.cols, img.rows, &result);
	for (int i = 0; i < 1000; i++)
	{
		double t = (double)cv::getTickCount();
		res = ExecuteBUAnalysisFromImage(handle, &image, &result);
		std::cout << (cv::getTickCount() - t) / (double)cv::getTickFrequency() << std::endl;

		if (res != SYY_NO_ERROR)
			return;

		res = DrawResult2Image(&image, &result);
		if (res != SYY_NO_ERROR)
			return;

		cv::Mat srcImg = cv::Mat(image.nHeight, image.nWidth,
			image.nChannels == 3 ? CV_8UC3 : CV_8UC1, image.pData);

		cv::imshow("srcImg", srcImg);
		cv::waitKey(1);
	}

	cv::Rect cropRect( result.rCropRect.x, result.rCropRect.y, result.rCropRect.w, result.rCropRect.h );
	cv::rectangle(img, cropRect, cv::Scalar(255, 255, 255), 2);
	auto text = result.nGrading == 0 ? "1a" : "other";
	cv::putText(img, text, cropRect.tl(), 1, 1, cv::Scalar(255, 255, 255));

	std::stringstream ss;

	for (int i = 0; i < result.nLessionsCount; i++)
	{
		//if (result.pLessionConfidence[i] < 0.3f) continue;

		cv::Rect r(result.pLessionRects[i].x, result.pLessionRects[i].y, 
			result.pLessionRects[i].w, result.pLessionRects[i].h);

		cv::rectangle(img, r, cv::Scalar(255, 255, 255), 1);

		cv::putText(img,
			result.pLessionTypes[i] == SYY::MedicalAnalysis::NO_LESSION ? "non-lession" : "lession",
			r.tl(), 1, 1, cv::Scalar(255, 255, 255) );

		ss << i << ".jpg";

		cv::imwrite(ss.str(), img(r));

	}

	cv::imshow("img", img);
	cv::waitKey();

	ReleaseBUAnalysis(handle);
}

void test_inpaint() {
	ErrorCode res;
	HANDLE handle;

	res = InitInpaint(handle, InpaintMode::Criminisi_P3);
	if (res != SYY_NO_ERROR)
		return;

	auto srcImg = cv::imread("D:\\blue\\data\\»ÈœŸ∞©Õº∆¨\\4a¿‡\\±ˆ”Ó∂\\1.2.826.0.1.3680043.2.461.8663564.48427869.jpg");
	auto maskImg = cv::imread("D:\\blue\\data\\»ÈœŸ∞©Õº∆¨\\4a¿‡\\±ˆ”Ó∂\\1.2.826.0.1.3680043.2.461.8663564.48427869.jpg");
	//auto maskImg = cv::imread("C:\\blue\\code\\MedicalAnalysis\\ConsoleDemo\\1.2.826.0.1.3680043.2.461.8630787.3708226823.jpg");
	cv::cvtColor(maskImg, maskImg, CV_RGB2GRAY);
	Image inpaint, 
		src((char*)srcImg.data, srcImg.cols, srcImg.rows, srcImg.channels()), 
		mask((char*)maskImg.data, maskImg.cols, maskImg.rows, maskImg.channels());

	res = ExecuteInpaint(handle, src, mask, inpaint);
	if (res != SYY_NO_ERROR)
		return;

	ReleaseInpaint(handle);

	auto inpaintImg = cv::Mat(inpaint.nHeight, inpaint.nWidth, CV_8UC3, inpaint.pData);
	cv::imshow("srcImg", srcImg);
	cv::imshow("maskImg", maskImg);
	cv::imshow("inpaintImg", inpaintImg);
	cv::waitKey();
}

void test_video() {
	ErrorCode res;

	HANDLE handle, analysisHandle;
	const std::string video_file = "E:\\Download\\video_fast.avi";
	res = LoadVideo(handle, video_file.c_str(), video_file.size());
	if (res != SYY_NO_ERROR)
	{
		return;
	}

	Image image;
	res = InitBUAnalysisWithMode(analysisHandle, BUAnalysisMode::Crop_V1 | BUAnalysisMode::DetectFast);
	if (res != SYY_NO_ERROR)
		return;

	BUAnalysisResult result;
	while (true)
	{
		res = GetVideoFrame(handle, &image);
		if (res == SYY_SYS_ERROR)
		{
			return;
		}
		if (res == SYY_VIDEO_END_FRAME)
		{
			std::cout << "video end!" << std::endl;
			return;
		}

		//cv::Mat frame = cv::Mat(image.nHeight, image.nWidth, (image.nChannels == 3 ? CV_8UC3 : CV_8UC1), image.pData);

		res = ExecuteBUAnalysisFromImage(analysisHandle, &image, &result);

		if (res != SYY_NO_ERROR)
			return;

		res = DrawResult2Image(&image, &result);
		if (res != SYY_NO_ERROR)
			return;

		cv::Mat frame = cv::Mat(image.nHeight, image.nWidth,
			image.nChannels == 3 ? CV_8UC3 : CV_8UC1, image.pData);

		cv::imshow("frame", frame);
		cv::waitKey(33);
	}
}

void main() 
{
	ErrorCode res;
	
	res = InitSDK();
	if (res != SYY_NO_ERROR)
		return;

	//test_BUAnalysis();
	//test_inpaint();
	test_video();

	ReleaseSDK();
}

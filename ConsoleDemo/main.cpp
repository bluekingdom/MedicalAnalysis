#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>

#include "MedicalAnalysisSDK.h"

using namespace SYY;
using namespace SYY::MedicalAnalysis;

void test_BUAnalysis() {
	ErrorCode res;

	HANDLE handle;
	res = InitBUAnalysis(handle, BUAnalysisMode::Crop_V2);
	if (res != SYY_NO_ERROR)
		return;

	cv::Mat img = cv::imread("C:\\blue\\data\\»ÈœŸ∞©Õº∆¨\\4a¿‡\\±ˆ”Ó∂\\1.2.826.0.1.3680043.2.461.8663564.1117253524.jpg");

	BUAnalysisResult result;
	res = ExecuteBUAnalysis(handle, (char* )img.data, img.cols, img.rows, &result);
	if (res != SYY_NO_ERROR)
		return;

	cv::Rect cropRect( result.rCropRect.x, result.rCropRect.y, result.rCropRect.w, result.rCropRect.h );
	cv::rectangle(img, cropRect, cv::Scalar(255, 255, 255), 2);

	for (int i = 0; i < result.nLessionsCount; i++)
	{
		cv::Rect r(result.pLessionRects[i].x, result.pLessionRects[i].y, 
			result.pLessionRects[i].w, result.pLessionRects[i].h);

		cv::rectangle(img, r, cv::Scalar(255, 255, 255), 1);
	}

	cv::imshow("img", img);
	cv::waitKey();

	ReleaseBUAnalysis(handle);
}

void test_inpaint() {
	ErrorCode res;
	HANDLE handle;

	res = InitInpaint(handle);
	if (res != SYY_NO_ERROR)
		return;

	auto srcImg = cv::imread("C:\\blue\\data\\»ÈœŸ∞©Õº∆¨\\4a¿‡01\\≥¬Ω¡¨\\1.2.826.0.1.3680043.2.461.8630787.3708226823.jpg");
	auto maskImg = cv::imread("C:\\blue\\code\\MedicalAnalysis\\ConsoleDemo\\1.2.826.0.1.3680043.2.461.8630787.3708226823.jpg");
	Image inpaint, 
		src((char*)srcImg.data, srcImg.cols, srcImg.rows), 
		mask((char*)maskImg.data, maskImg.cols, maskImg.rows);

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

void main() {
	ErrorCode res;
	
	res = InitSDK();
	if (res != SYY_NO_ERROR)
		return;

	//test_BUAnalysis();
	test_inpaint();

	ReleaseSDK();
}

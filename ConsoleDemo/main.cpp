#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>

#include "MedicalAnalysisSDK.h"

using namespace SYY;
using namespace SYY::MedicalAnalysis;

void main() {
	ErrorCode res;
	
	res = InitSDK();
	if (res != SYY_NO_ERROR)
	{
		return;
	}

	MA_HANDLE handle;
	res = InitBUAnalysis(handle);
	if (res != SYY_NO_ERROR)
		return;

	cv::Mat img = cv::imread("C:\\blue\\data\\»ÈœŸ∞©Õº∆¨\\4a¿‡\\±ˆ”Ó∂\\1.2.826.0.1.3680043.2.461.8663564.1117253524.jpg");

	BUAnalysisResult result;
	res = ExecuteBUAnalysis(handle, (char* )img.data, img.cols, img.rows, true, &result);
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
	ReleaseSDK();
}

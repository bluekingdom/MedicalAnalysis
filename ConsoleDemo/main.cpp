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
	if (res != SYY_SYS_ERROR)
		return;

	ReleaseBUAnalysis(handle);
	ReleaseSDK();
}

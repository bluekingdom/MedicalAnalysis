#pragma once

#include <opencv2/opencv.hpp>
#include "MedicalAnalysisSDK.h"
#include "deeplearning.h"

namespace SYY {
namespace MedicalAnalysis {

	class BUAnalysis {
	public:
		BUAnalysis();
		~BUAnalysis() {}

		ErrorCode Init(unsigned long nMode);
		ErrorCode Release();

		ErrorCode Analysis(const cv::Mat& srcImg, BUAnalysisResult& result);

	protected:
		bool CropValidRegion(const cv::Mat& srcImg, cv::Rect& validRegion);
		bool CropValidRegion_V1(const cv::Mat& srcImg, cv::Rect& validRegion);
		bool CropValidRegion_V2(const cv::Mat& srcImg, cv::Rect& validRegion);
		void GetContoursBBox(const cv::Mat& srcImg, std::vector<cv::Rect>& bbox);
		void ClusterRegions(const cv::Mat& srcImg, std::vector<cv::Rect>& regions);

	private:
		DeepLearning* m_pBUDetector;
		DeepLearning* m_pValidRegionDetector;
		int m_nCropVersion;
	};

}
}
#pragma once

#include <opencv2/opencv.hpp>
#include "MedicalAnalysisSDK.h"
#include "deeplearning.h"

namespace SYY {
namespace MedicalAnalysis {

	class BUAnalysis {
	public:
		enum AnalysisMethod
		{
			None = 0,
			FRCNN = 1,
			SSD = 2,
		};
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

		bool InitFRCNN();
		bool AnalysisFRCNN(const cv::Mat& srcImg, std::vector<DeepLearning::Detect_Result>& dets);
		void ReleaseFRCNN();

		bool InitSSD();
		bool AnalysisSSD(const cv::Mat& srcImg, std::vector<DeepLearning::Detect_Result>& dets);
		void ReleaseSSD();

	private:
		DeepLearning* m_pBUDetector;
		DeepLearning* m_pValidRegionDetector;
		int m_nCropVersion;
		AnalysisMethod m_nAnalysisMethod;
	};

}
}
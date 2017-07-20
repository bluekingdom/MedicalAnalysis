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
			SSD_MULTI = 3,
		};
	public:
		BUAnalysis();
		~BUAnalysis() {}

		ErrorCode Init(unsigned long nMode);
		ErrorCode Release();

		ErrorCode Analysis(const cv::Mat& srcImg, BUAnalysisResult& result);
		ErrorCode ProcessResults(BUAnalysisResult& result);

	protected:
		bool CropValidRegion(const cv::Mat& srcImg, cv::Rect& validRegion);

		// 使用传统方法
		bool CropValidRegion_V1(const cv::Mat& srcImg, cv::Rect& validRegion);

		// 使用frcnn
		bool CropInit_V2();
		bool CropValidRegion_V2(const cv::Mat& srcImg, cv::Rect& validRegion);

		// 使用ssd
		bool CropInit_V3();
		bool CropValidRegion_V3(const cv::Mat& srcImg, cv::Rect& validRegion);

		void GetContoursBBox(const cv::Mat& srcImg, std::vector<cv::Rect>& bbox);
		void ClusterRegions(const cv::Mat& srcImg, std::vector<cv::Rect>& regions);

		bool InitFRCNN();
		bool AnalysisFRCNN(const cv::Mat& srcImg, std::vector<DeepLearning::Detect_Result>& dets);
		void ReleaseFRCNN();

		bool InitSSDMore();
		bool InitSSDAccurate();
		bool AnalysisSSD(const cv::Mat& srcImg, const cv::Rect& validRect, std::vector<DeepLearning::Detect_Result>& dets);
		void ReleaseSSD();

		static bool ClassifierInitializer(DeepLearning* &pClassifier,
			const std::string& prototxt, const std::string& caffemodel, 
			bool is_gpu);

		bool InitGradeClassifier();

		bool InitLessionClassifier();


	private:
		DeepLearning* m_pBUDetector;
		DeepLearning* m_pValidRegionDetector;
		DeepLearning* m_pGradingClassifier;
		DeepLearning* m_pLessionClassifier;
		int m_nCropVersion;
		AnalysisMethod m_nAnalysisMethod;

		static const int s_nNumBlasThread = 8;
		static const bool s_bIsUseGPU = true;
	};

}
}
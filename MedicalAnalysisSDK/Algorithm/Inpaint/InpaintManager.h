#pragma once

#include "MedicalAnalysisSDK.h"
#include <opencv2/core.hpp>

namespace SYY {
namespace Inpainting {
	class InpaintManager {
	public:
		InpaintManager();

		ErrorCode Init(unsigned long nMode);
		ErrorCode ExecuteInpaint(const cv::Mat& srcImg, const cv::Mat& maskImg, cv::Mat& inpaintImg);
		void Release();

	protected:
		bool PatchMatch_v1(const cv::Mat& srcImg, const cv::Mat& maskImg, cv::Mat& inpaintImg);
		bool PatchMatch_v2(const cv::Mat& srcImg, const cv::Mat& maskImg, cv::Mat& inpaintImg);
		bool Criminisi(const cv::Mat& srcImg, const cv::Mat& maskImg, cv::Mat& inpaintImg);

	private:
		unsigned long m_nMode;
	};

}
}

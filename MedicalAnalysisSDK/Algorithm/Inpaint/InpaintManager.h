#pragma once

#include "MedicalAnalysisSDK.h"
#include <opencv2/core.hpp>

namespace SYY {
namespace Inpaint {
	class InpaintManager {
	public:
		InpaintManager();

		ErrorCode Init();
		ErrorCode ExecuteInpaint(const cv::Mat& srcImg, const cv::Mat& maskImg, cv::Mat& inpaintImg);
		void Release();
	};

}
}

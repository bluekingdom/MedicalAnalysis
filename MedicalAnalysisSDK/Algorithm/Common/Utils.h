#pragma once

#include <string>
#include "MedicalAnalysisSDK.h"
#include <opencv2/core.hpp>

namespace SYY {
	namespace FileSystem
	{
		std::string GetCurExePath();
		bool CreateDir(std::string dir);
		bool IsExists(const std::string& file);
	}

	namespace Common {
		const cv::Rect Rect2CVRect(const Rect& rect);
		const Rect CVRect2Rect(const cv::Rect& rect);
		float IOU(const cv::Rect& a, const cv::Rect& b);
	}

	namespace Config {
		std::string GetConfValue(const std::string& key);
	}

}
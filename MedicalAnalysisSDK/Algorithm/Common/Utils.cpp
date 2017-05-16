#include "Utils.h"

#include <boost/filesystem.hpp>

namespace SYY {

namespace FileSystem
{

	std::string GetCurExePath()
	{
		return boost::filesystem::initial_path<boost::filesystem::path>().string();
	}

	bool CreateDir(std::string dir)
	{
		if (false == boost::filesystem::exists(dir))
			return boost::filesystem::create_directories(dir);
		return true;
	}

	bool IsExists(const std::string& file)
	{
		return boost::filesystem::exists(file);
	}

}

namespace Common {
	const cv::Rect& Rect2CVRect(const Rect& rect)
	{
		return cv::Rect(rect.x, rect.y, rect.w, rect.h);
	}

	const Rect& CVRect2Rect(const cv::Rect& rect)
	{
		Rect r;
		r.x = rect.x;
		r.y = rect.y;
		r.w = rect.width;
		r.h = rect.height;
		return r;
	}

}



}
#include "Utils.h"

#include <boost/filesystem.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>

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
	const cv::Rect Rect2CVRect(const Rect& rect)
	{
		return cv::Rect(rect.x, rect.y, rect.w, rect.h);
	}

	const Rect CVRect2Rect(const cv::Rect& rect)
	{
		Rect r;
		r.x = rect.x;
		r.y = rect.y;
		r.w = rect.width;
		r.h = rect.height;
		return r;
	}

	float IOU(const cv::Rect& a, const cv::Rect& b)
	{
		float i = (a & b).area();
		float u = std::abs(a.area() + b.area() - i);

		return i / u;
	}

}

namespace Config {

	std::string GetConfValue(const std::string& key)
	{
		std::string config_file = FileSystem::GetCurExePath() + "\\config\\sdk.cfg";
		if (false == FileSystem::IsExists(config_file))
		{
			return "!";
		}

		using boost::property_tree::ptree;
		ptree pt;
		read_xml(config_file, pt);

		return pt.get<std::string>("config." + key);
	}

}

}
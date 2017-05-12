#include "Utils.h"

#include <boost/filesystem.hpp>

namespace SYY {

namespace Utils
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

}
}
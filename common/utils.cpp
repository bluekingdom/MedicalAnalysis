
#include "stdafx.h"
#include "utils.h"
#include <boost/filesystem.hpp>
#include <opencv2/opencv.hpp>

const std::vector<std::string>& scanFilesUseRecursive(
	const std::string& rootPath, std::vector<std::string>& container){
	namespace fs = boost::filesystem;
	fs::path fullpath(rootPath, fs::native);
	std::vector<std::string> &ret = container;

	if (!fs::exists(fullpath)){ return ret; }
	fs::recursive_directory_iterator end_iter;
	for (fs::recursive_directory_iterator iter(fullpath); iter != end_iter; iter++){
		try{
			if (fs::is_directory(*iter)){
				//ret.push_back(iter->path().string());
			}
			else{
				ret.push_back(iter->path().string());
			}
		}
		catch (const std::exception & ex){
			//std::cerr << ex.what() << std::endl;
			continue;
		}
	}
	return ret;
}


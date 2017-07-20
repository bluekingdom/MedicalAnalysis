#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <fstream>

#include "MedicalAnalysisSDK.h"


using namespace SYY;
using namespace SYY::MedicalAnalysis;
using namespace SYY::Inpainting;

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

void test_BUAnalysis() 
{
	ErrorCode res;

	HANDLE handle;
	res = InitBUAnalysisWithMode(handle, BUAnalysisMode::Crop_V1 | BUAnalysisMode::DetectMore);
	if (res != SYY_NO_ERROR)
		return;

	//std::string fileName = "E:\\data\\ÈéÏÙ°©Í¼Æ¬\\5Àà\\ÅË¹âÃô\\1.2.826.0.1.3680043.2.461.8921672.1968559331.jpg";
	std::string fileName = "D:\\blue\\data\\ÈéÏÙ°©Í¼Æ¬\\5Àà\\ÅË¹âÃô\\1.2.826.0.1.3680043.2.461.8921672.1968559331.jpg";

	const std::string folder = "D:\\blue\\data\\²âÊÔÍ¼Æ¬\\";

	std::vector<std::string> files;

	scanFilesUseRecursive(folder, files);

	for (auto file : files)
	{
		cv::Mat img = cv::imread(file);

		Image image;
		image.nWidth = img.cols;
		image.nHeight = img.rows;
		image.nChannels = img.channels();
		image.pData = new char[image.nWidth * image.nHeight * image.nChannels];
		memcpy(image.pData, img.data, img.step[0] * img.rows);

		std::ifstream imgFile(fileName, std::ios::binary | std::ios::ate);
		std::streamsize size = imgFile.tellg();
		imgFile.seekg(0, std::ios::beg);

		char* pFileData = new char[size];
		imgFile.read(pFileData, size);

		BUAnalysisResult result;
		//res = ExecuteBUAnalysis(handle, (char* )img.data, img.cols, img.rows, &result);
		res = ExecuteBUAnalysisFromFile(handle, &image, &result);
		if (res != SYY_NO_ERROR)
			return;

		res = DrawResult2Image(&image, &result);
		if (res != SYY_NO_ERROR)
			return;

		cv::Mat srcImg = cv::Mat(image.nHeight, image.nWidth,
			image.nChannels == 3 ? CV_8UC3 : CV_8UC1, image.pData);

		//cv::imshow("srcImg", srcImg);
		//cv::waitKey();

		cv::Rect cropRect(result.rCropRect.x, result.rCropRect.y, result.rCropRect.w, result.rCropRect.h);
		cv::rectangle(img, cropRect, cv::Scalar(255, 255, 255), 2);
		auto text = result.nGrading == 0 ? "1a" : "other";
		cv::putText(img, text, cropRect.tl(), 1, 1, cv::Scalar(255, 255, 255));

		std::stringstream ss;

		for (int i = 0; i < result.nLessionsCount; i++)
		{
			//if (result.pLessionConfidence[i] < 0.3f) continue;

			const float radio = 0.0f;

			cv::Rect r(result.pLessionRects[i].x, result.pLessionRects[i].y, result.pLessionRects[i].w, result.pLessionRects[i].h);

			r.x = std::max(cropRect.x, int(r.x - radio * r.width));
			r.y = std::max(cropRect.y, int(r.y - radio * r.height));
			r.width = std::min(int(r.width + 2 * radio * r.width), cropRect.br().x - r.x);
			r.height = std::min(int(r.height + 2 * radio * r.height), cropRect.br().y - r.y);

			//auto part = img(r).clone();
			//cv::cvtColor(part, part, CV_RGB2GRAY);
			//cv::Mat edge;

			//cv::threshold(part, edge, 0, 255, cv::THRESH_OTSU);
			//cv::Canny(edge, edge, 0, 255);

			//edge.convertTo(edge, CV_32F);
			//edge = 255 - edge;
			//edge.convertTo(edge, CV_8U);

			auto c = (r.tl() + r.br()) / 2;
			cv::ellipse(img, c, r.size() / 2, 0, 0, 360, cv::Scalar(255, 255, 255));

			//cv::imshow("edge", edge);
			//cv::waitKey();

			cv::rectangle(img, r, cv::Scalar(255, 255, 255), 1);

			cv::putText(img,
				result.pLessionTypes[i] == SYY::MedicalAnalysis::NO_LESSION ? "non-lession" : "lession",
				r.tl(), 1, 1, cv::Scalar(255, 255, 255));

			ss.str("");
			ss.clear();

			ss << i << ".jpg";

			cv::imwrite(ss.str(), img(r));
		}

		cv::imshow("img", img);
		cv::waitKey();
	}

	ReleaseBUAnalysis(handle);
}

void test_inpaint() {
	ErrorCode res;
	HANDLE handle;

	res = InitInpaint(handle, InpaintMode::Criminisi_P3);
	if (res != SYY_NO_ERROR)
		return;

	auto srcImg = cv::imread("D:\\blue\\data\\ÈéÏÙ°©Í¼Æ¬\\4aÀà\\±öÓî¶ð\\1.2.826.0.1.3680043.2.461.8663564.48427869.jpg");
	auto maskImg = cv::imread("D:\\blue\\data\\ÈéÏÙ°©Í¼Æ¬\\4aÀà\\±öÓî¶ð\\1.2.826.0.1.3680043.2.461.8663564.48427869.jpg");
	//auto maskImg = cv::imread("C:\\blue\\code\\MedicalAnalysis\\ConsoleDemo\\1.2.826.0.1.3680043.2.461.8630787.3708226823.jpg");
	cv::cvtColor(maskImg, maskImg, CV_RGB2GRAY);
	Image inpaint, 
		src((char*)srcImg.data, srcImg.cols, srcImg.rows, srcImg.channels()), 
		mask((char*)maskImg.data, maskImg.cols, maskImg.rows, maskImg.channels());

	res = ExecuteInpaint(handle, src, mask, inpaint);
	if (res != SYY_NO_ERROR)
		return;

	ReleaseInpaint(handle);

	auto inpaintImg = cv::Mat(inpaint.nHeight, inpaint.nWidth, CV_8UC3, inpaint.pData);
	cv::imshow("srcImg", srcImg);
	cv::imshow("maskImg", maskImg);
	cv::imshow("inpaintImg", inpaintImg);
	cv::waitKey();
}

void main() 
{
	ErrorCode res;
	
	res = InitSDK();
	if (res != SYY_NO_ERROR)
		return;

	test_BUAnalysis();
	//test_inpaint();

	ReleaseSDK();
}

/*!
 * \file AlgorithmManager.cpp
 * \date 2017/05/09 17:40
 *
 * \author blue
 * Contact: yang.wang@shangyiyun.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#include "AlgorithmManager.h"

#include "Algorithm/Common/glog.h"
#include "Algorithm/B-scanUltrasonography/BUAnalysis.h"
#include "Algorithm/Inpaint/InpaintManager.h"
#include "Algorithm/Common/Utils.h"

namespace SYY {
	using namespace MedicalAnalysis;
	using namespace Inpainting;

	ErrorCode AlgorithmManager::Init()
	{

		return SYY_NO_ERROR;
	}

	ErrorCode AlgorithmManager::Release()
	{
		return SYY_NO_ERROR;
	}

	ErrorCode AlgorithmManager::InitBUAnalysis(HANDLE& hHandle, unsigned long nMode)
	{
		BUAnalysis* pBUAnalysis = new BUAnalysis();
		if (!pBUAnalysis || SYY_NO_ERROR != pBUAnalysis->Init(nMode))
		{
			GLOG("InitBUAnalysis Error: init fail!\n");
			return SYY_SYS_ERROR;
		}

		hHandle = reinterpret_cast<HANDLE>(pBUAnalysis);

		return SYY_NO_ERROR;
	}

	ErrorCode AlgorithmManager::ReleaseBUAnalysis(HANDLE& hHandle)
	{
		BUAnalysis* pBUAnalysis = reinterpret_cast<BUAnalysis*>(hHandle);
		if (pBUAnalysis == nullptr)
		{
			GLOG("ReleaseBUAnalysis Error: hHandle is invalid!\n");
			return SYY_SYS_ERROR;
		}

		pBUAnalysis->Release();

		return SYY_NO_ERROR;
	}

	ErrorCode AlgorithmManager::ExecuteBUAnalysis(HANDLE hHandle, 
		char* pImg, int nImgWidth, int nImgHeight, 
		BUAnalysisResult* pResult)
	{
		BUAnalysis* pBUAnalysis = reinterpret_cast<BUAnalysis*>(hHandle);
		if (pBUAnalysis == nullptr)
		{
			GLOG("ReleaseBUAnalysis Error: hHandle is invalid!\n");
			return SYY_SYS_ERROR;
		}

		return pBUAnalysis->Analysis(cv::Mat(nImgHeight, nImgWidth, CV_8UC3, pImg), *pResult);
	}

	SYY::ErrorCode AlgorithmManager::ExecuteBUAnalysisFromFile(HANDLE hHandle, 
		Image* pImage, MedicalAnalysis::BUAnalysisResult* pResult)
	{
		BUAnalysis* pBUAnalysis = reinterpret_cast<BUAnalysis*>(hHandle);
		if (pBUAnalysis == nullptr)
		{
			GLOG("ReleaseBUAnalysis Error: hHandle is invalid!\n");
			return SYY_SYS_ERROR;
		}

		cv::Mat srcImg = cv::Mat(pImage->nHeight, pImage->nWidth, 
			pImage->nChannels == 3 ? CV_8UC3 : CV_8UC1, pImage->pData);

		ErrorCode res = pBUAnalysis->Analysis(srcImg, *pResult);

		if (res != SYY_NO_ERROR)
			return res;

		return pBUAnalysis->ProcessResults(*pResult);
	}

	SYY::ErrorCode AlgorithmManager::ExecuteInpaint(HANDLE hHandle, 
		Image srcImg, Image maskImg, Image& inpaintImg)
	{
		InpaintManager* pInpaint = reinterpret_cast<InpaintManager*>(hHandle);
		if (pInpaint == nullptr)
		{
			GLOG("Error: hHandle is invalid!\n");
			return SYY_SYS_ERROR;
		}

		if (inpaintImg.pData)
			delete[] inpaintImg.pData;
		inpaintImg.pData = nullptr;

		auto src = cv::Mat(srcImg.nHeight, srcImg.nWidth, srcImg.nChannels == 3 ? CV_8UC3 : CV_8UC1, srcImg.pData);
		auto mask = cv::Mat(maskImg.nHeight, maskImg.nWidth, maskImg.nChannels == 1 ? CV_8UC1 : CV_8UC3, maskImg.pData);
		cv::Mat inpaint;
		//= cv::Mat(inpaintImg.nHeight, inpaintImg.nWidth, CV_8UC3, inpaintImg.pData);

		auto code = pInpaint->ExecuteInpaint(src, mask, inpaint);

		if (code != SYY_NO_ERROR)
			return code;

		inpaintImg.nHeight = inpaint.rows;
		inpaintImg.nWidth = inpaint.cols;
		inpaintImg.nChannels = inpaint.channels();
		int imgDataLen = inpaint.rows * inpaint.step[0];

		inpaintImg.pData = new char[imgDataLen];
		memcpy(inpaintImg.pData, inpaint.data, imgDataLen);

		return SYY_NO_ERROR;
	}

	SYY::ErrorCode AlgorithmManager::InitInpaint(HANDLE& hHandle, unsigned long nMode)
	{
		InpaintManager* pInpaint = new InpaintManager();
		if (!pInpaint || SYY_NO_ERROR != pInpaint->Init(nMode))
		{
			GLOG("Error: inpaint init fail!\n");
			return SYY_SYS_ERROR;
		}

		hHandle = reinterpret_cast<HANDLE>(pInpaint);
		return SYY_NO_ERROR;
	}

	SYY::ErrorCode AlgorithmManager::ReleaseInpaint(HANDLE hHandle)
	{
		InpaintManager* pInpaint = reinterpret_cast<InpaintManager*>(hHandle);
		if (pInpaint == nullptr)
		{
			GLOG("Error: hHandle is invalid!\n");
			return SYY_SYS_ERROR;
		}

		pInpaint->Release();

		return SYY_NO_ERROR;
	}

	SYY::ErrorCode AlgorithmManager::DrawResult2Image(
		Image* pImage,
		MedicalAnalysis::BUAnalysisResult* pResult)
	{
		if (pImage == nullptr || pImage->pData == 0)
		{
			GLOG("Error: pImage or pImage->pData is nullptr!\n");
			return SYY_SYS_ERROR;
		}

		cv::Mat srcImg = cv::Mat(pImage->nHeight, pImage->nWidth, 
			pImage->nChannels == 3 ? CV_8UC3 : CV_8UC1, pImage->pData);		

		//cv::imshow("srcImg", srcImg);
		//cv::waitKey();

		for (int i = 0; i < pResult->nLessionsCount; i++)
		{
			auto rect = Common::Rect2CVRect(pResult->pLessionRects[i]);
			cv::rectangle(srcImg, rect, cv::Scalar(255, 255, 255));
		}

		return SYY_NO_ERROR;
	}

}
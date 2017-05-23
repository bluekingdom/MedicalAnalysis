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

namespace SYY {
	using namespace MedicalAnalysis;

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

	SYY::ErrorCode AlgorithmManager::ExecuteInpaint(HANDLE hHandle, 
		Image srcImg, Image maskImg, Image inpaintImg)
	{

		return SYY_NO_ERROR;
	}

	SYY::ErrorCode AlgorithmManager::InitInpaint(HANDLE& hHandle)
	{

		return SYY_NO_ERROR;
	}

	SYY::ErrorCode AlgorithmManager::ReleaseInpaint(HANDLE hHandle)
	{

		return SYY_NO_ERROR;
	}

}
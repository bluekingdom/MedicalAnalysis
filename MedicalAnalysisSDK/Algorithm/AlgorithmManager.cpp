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

namespace SYY {
namespace MedicalAnalysis {

	ErrorCode AlgorithmManager::Init()
	{

		return SYY_NO_ERROR;
	}

	ErrorCode AlgorithmManager::Release()
	{

		return SYY_NO_ERROR;
	}

	ErrorCode AlgorithmManager::InitBUAnalysis(MA_HANDLE& hHandle)
	{
		BUAnalysis* pBUAnalysis = new BUAnalysis();
		if (!pBUAnalysis || SYY_NO_ERROR != pBUAnalysis->Init())
		{
			GLOG("InitBUAnalysis Error: init fail!\n");
			return SYY_SYS_ERROR;
		}

		hHandle = reinterpret_cast<MA_HANDLE>(pBUAnalysis);

		return SYY_NO_ERROR;
	}

	ErrorCode AlgorithmManager::ReleaseBUAnalysis(MA_HANDLE& hHandle)
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

	ErrorCode AlgorithmManager::ExecuteBUAnalysis(MA_HANDLE hHandle, 
		char* pImg, int nImgWidth, int nImgHeight, bool bIsRunCrop,
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

}
}
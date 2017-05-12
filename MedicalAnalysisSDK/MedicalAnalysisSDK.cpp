/*!
 * \file MedicalAnalysisSDK.cpp
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

#include "MedicalAnalysisSDK.h"
#include "Algorithm/AlgorithmManager.h"
#include  "Algorithm/Common/glog.h"


namespace SYY {
	namespace MedicalAnalysis {

		static AlgorithmManager* sg_pAlgorithmManager = nullptr;

		MEDICAL_ANALYSIS_SDK_API ErrorCode InitSDK()
		{
			if (!GLog::GetInstance())
			{
				return SYY_LOG_INIT_NO_PERMISSION;
			}

			GLOG("InitSDK Info: enter InitSDK!\n");

			if (sg_pAlgorithmManager != nullptr)
			{
				GLOG("InitSDK Error: repeat call InitSDK!\n");
				return SYY_SDK_REPEAT_INIT;
			}

			sg_pAlgorithmManager = new AlgorithmManager;

			ErrorCode code;
			if (!sg_pAlgorithmManager || SYY_NO_ERROR != (code = sg_pAlgorithmManager->Init()))
			{
				GLOG("InitSDK Error: algorithm manager init fail!\n");
				return code;
			}

			GLOG("InitSDK Info: InitSDK success!\n");
			return SYY_NO_ERROR;
		}

		MEDICAL_ANALYSIS_SDK_API ErrorCode ReleaseSDK()
		{
			GLOG("ReleaseSDK Info: enter!\n");

			if (sg_pAlgorithmManager)
			{
				sg_pAlgorithmManager->Release();
			}

			GLOG("ReleaseSDK Info: success!\n");
			return SYY_NO_ERROR;
		}

		MEDICAL_ANALYSIS_SDK_API ErrorCode InitBUAnalysis(OUT MA_HANDLE& hHandle)
		{
			if (sg_pAlgorithmManager == nullptr)
			{
				return SYY_SDK_NO_INIT;
			}
			return sg_pAlgorithmManager->InitBUAnalysis(hHandle);
		}

		MEDICAL_ANALYSIS_SDK_API ErrorCode ReleaseBUAnalysis(INOUT MA_HANDLE& hHandle)
		{
			if (sg_pAlgorithmManager == nullptr)
			{
				return SYY_SDK_NO_INIT;
			}
			return sg_pAlgorithmManager->ReleaseBUAnalysis(hHandle);
		}

		MEDICAL_ANALYSIS_SDK_API ErrorCode ExecuteBUAnalysis(IN MA_HANDLE hHandle, 
			IN char* pImg, IN int nImgWidth, IN int nImgHeight, IN bool bIsRunCrop,
			OUT BUAnalysisResult* pResult)
		{
			if (sg_pAlgorithmManager == nullptr)
			{
				return SYY_SDK_NO_INIT;
			}
			sg_pAlgorithmManager->ExecuteBUAnalysis(hHandle, pImg, nImgWidth, nImgHeight, bIsRunCrop, pResult);
			return SYY_NO_ERROR;
		}

	}
}
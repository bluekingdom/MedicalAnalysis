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

#define CHECK_ALGO_INIT() \
	if (sg_pAlgorithmManager == nullptr) return SYY_SDK_NO_INIT;

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

		MEDICAL_ANALYSIS_SDK_API ErrorCode InitBUAnalysis(OUT HANDLE& hHandle, IN unsigned long nMode)
		{
			if (sg_pAlgorithmManager == nullptr)
			{
				return SYY_SDK_NO_INIT;
			}
			return sg_pAlgorithmManager->InitBUAnalysis(hHandle, nMode);
		}

		MEDICAL_ANALYSIS_SDK_API ErrorCode ReleaseBUAnalysis(INOUT HANDLE& hHandle)
		{
			if (sg_pAlgorithmManager == nullptr)
			{
				return SYY_SDK_NO_INIT;
			}
			return sg_pAlgorithmManager->ReleaseBUAnalysis(hHandle);
		}

		MEDICAL_ANALYSIS_SDK_API ErrorCode ExecuteBUAnalysis(IN HANDLE hHandle, 
			IN char* pImg, IN int nImgWidth, IN int nImgHeight,
			OUT BUAnalysisResult* pResult)
		{
			if (sg_pAlgorithmManager == nullptr)
			{
				return SYY_SDK_NO_INIT;
			}
			return sg_pAlgorithmManager->ExecuteBUAnalysis(hHandle, pImg, nImgWidth, nImgHeight, pResult);
		}

		MEDICAL_ANALYSIS_SDK_API ErrorCode InitInpaint(OUT HANDLE& hHandle)
		{
			CHECK_ALGO_INIT();
			return sg_pAlgorithmManager->InitInpaint(hHandle);
		}

		MEDICAL_ANALYSIS_SDK_API ErrorCode ReleaseInpaint(INOUT HANDLE& hHandle)
		{
			CHECK_ALGO_INIT();
			return sg_pAlgorithmManager->ReleaseInpaint(hHandle);
		}

		MEDICAL_ANALYSIS_SDK_API ErrorCode ExecuteInpaint(IN HANDLE hHandle, IN Image srcImg, IN Image maskImg, OUT Image& inpaintImg)
		{
			CHECK_ALGO_INIT();
			return sg_pAlgorithmManager->ExecuteInpaint(hHandle, srcImg, maskImg, inpaintImg);
		}

	}
}
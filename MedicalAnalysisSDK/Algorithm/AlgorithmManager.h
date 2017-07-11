/*!
 * \file AlgorithmManager.h
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

#pragma once

#include "ErrorCode.h"
#include "MedicalAnalysisSDK.h"

namespace SYY {

	class AlgorithmManager {
	public:
		ErrorCode Init();
		ErrorCode Release();

		ErrorCode InitBUAnalysis( HANDLE& hHandle, unsigned long nMode);

		ErrorCode ReleaseBUAnalysis( HANDLE& hHandle );

		ErrorCode ExecuteBUAnalysis(HANDLE hHandle,
			char* pImg, int nImgWidth, int nImgHeight,
			MedicalAnalysis::BUAnalysisResult* pResult);

		ErrorCode ExecuteBUAnalysisFromFile(HANDLE hHandle,
			Image* pImage,
			MedicalAnalysis::BUAnalysisResult* pResult);

		ErrorCode InitInpaint(HANDLE& hHandle, unsigned long nMode);
		ErrorCode ReleaseInpaint(HANDLE hHandle);

		ErrorCode ExecuteInpaint(HANDLE hHandle, 
			Image srcImg, Image maskImg, 
			Image& inpaintImg);

		ErrorCode DrawResult2Image(
			INOUT Image* pImage,
			IN MedicalAnalysis::BUAnalysisResult* pResult);	

	private:

	};

}

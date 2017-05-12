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
#include "Algorithm/B-scanUltrasonography/BUAnalysis.h"

namespace SYY {
	namespace MedicalAnalysis {

	class AlgorithmManager {
	public:
		ErrorCode Init();
		ErrorCode Release();

		ErrorCode InitBUAnalysis( MA_HANDLE& hHandle );

		ErrorCode ReleaseBUAnalysis( MA_HANDLE& hHandle );

		ErrorCode ExecuteBUAnalysis(MA_HANDLE hHandle,
			char* pImg, int nImgWidth, int nImgHeight, bool bIsRunCrop,
			BUAnalysisResult* pResult);

	private:

	};

	}
}

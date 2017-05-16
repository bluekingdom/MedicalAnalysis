/*!
 * \file MedicalAnalysisSDK.h
 * \date 2017/05/09 16:20
 *
 * \author blue
 * Contact: yang.wang@shangyiyun.com
 *
 * \brief 
 *
 * description: sdk header
 *
 * \note
*/

#pragma once

#include "ErrorCode.h"

#ifdef MEDICAL_ANALYSIS_SDK_EXPORTS
#define MEDICAL_ANALYSIS_SDK_API __declspec(dllexport) 
#else
#define MEDICAL_ANALYSIS_SDK_API __declspec(dllimport) 
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef INOUT
#define INOUT
#endif

namespace SYY { 

	struct Rect { 
		int x, y, w, h; 
	};

namespace MedicalAnalysis {

	typedef unsigned long long MA_HANDLE;

	struct BUAnalysisResult {
		BUAnalysisResult() : pLessionRects(nullptr), nLessionsCount(0) {}

		Rect rCropRect;				// 有效图片区域

		int nLessionsCount;			// 病灶数量
		Rect* pLessionRects;		// 病灶区域
	};

	MEDICAL_ANALYSIS_SDK_API ErrorCode InitSDK();

	MEDICAL_ANALYSIS_SDK_API ErrorCode ReleaseSDK();

	MEDICAL_ANALYSIS_SDK_API ErrorCode InitBUAnalysis(
		OUT MA_HANDLE& hHandle
		);

	MEDICAL_ANALYSIS_SDK_API ErrorCode ReleaseBUAnalysis(
		INOUT MA_HANDLE& hHandle
		);

	MEDICAL_ANALYSIS_SDK_API ErrorCode ExecuteBUAnalysis(
		IN MA_HANDLE hHandle, 
		IN char* pImg, 
		IN int nImgWidth, 
		IN int nImgHeight,
		IN bool bIsRunCrop,
		OUT BUAnalysisResult* pResult
		);

}
}

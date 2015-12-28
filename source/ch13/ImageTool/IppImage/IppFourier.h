/******************************************************************************
*
* IppFourier.h
*
* Copyright (c) 2015~<current> by Sun-Kyoo Hwang <sunkyoo.ippbook@gmail.com>
*
* This source code is included in the book titled "Image Processing
* Programming By Visual C++ (2nd Edition)"
*
******************************************************************************/

#pragma once

#include "IppImage.h"

class IppFourier
{
public:
	int width;
	int height;
	IppDoubleImage real; // 실수부
	IppDoubleImage imag; // 허수부

public:
	IppFourier();

	void SetImage(IppByteImage& img);
	void GetImage(IppByteImage& img);
	void GetSpectrumImage(IppByteImage& img);
	void GetPhaseImage(IppByteImage& img);

	// 영상의 푸리에 변환 함수
	void DFT(int dir);
	void DFTRC(int dir);
	void FFT(int dir);

	// 주파수 공간에서의 필터링 함수
	void LowPassIdeal(int cutoff);
	void HighPassIdeal(int cutoff);
	void LowPassGaussian(int cutoff);
	void HighPassGaussian(int cutoff);
};

// 전역 함수 선언
void DFT1d(double* re, double* im, int N, int dir);
void FFT1d(double* re, double* im, int N, int dir);
bool IsPowerOf2(int n);

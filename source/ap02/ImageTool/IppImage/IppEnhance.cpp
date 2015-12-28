/******************************************************************************
 *
 * IppEnhance.cpp
 *
 * Copyright (c) 2015~<current> by Sun-Kyoo Hwang <sunkyoo.ippbook@gmail.com>
 *
 * This source code is included in the book titled "Image Processing 
 * Programming By Visual C++ (2nd Edition)"
 *
 *****************************************************************************/

#include "stdafx.h"
#include <math.h>
#include "IppEnhance.h"

void IppInverse(IppByteImage& img)
{
	int size = img.GetSize();
	BYTE* p = img.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p[i] = 255 - p[i];
	}
}

void IppBrightness(IppByteImage& img, int n)
{
	int size = img.GetSize();
	BYTE* p = img.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p[i] = limit(p[i] + n);
	}
}

void IppContrast(IppByteImage& img, int n)
{
	int size = img.GetSize();
	BYTE* p = img.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p[i] = static_cast<BYTE>(limit(p[i] + (p[i] - 128) * n / 100));
	}
}

void IppGammaCorrection(IppByteImage& img, float gamma)
{
	float inv_gamma = 1.f / gamma;

	float gamma_table[256];
	for (int i = 0; i < 256; i++)
		gamma_table[i] = pow((i / 255.f), inv_gamma);

	int size = img.GetSize();
	BYTE* p = img.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p[i] = static_cast<BYTE>(limit(gamma_table[p[i]] * 255 + 0.5f));
	}
}

void IppHistogram(IppByteImage& img, float histo[256])
{
	int size = img.GetSize();
	BYTE* p = img.GetPixels();

	// 히스토그램 계산
	int cnt[256];
	memset(cnt, 0, sizeof(int) * 256);
	for (int i = 0; i < size; i++)
		cnt[p[i]]++;

	// 히스토그램 정규화(histogram normalization)
	for (int i = 0; i < 256; i++)
	{
		histo[i] = static_cast<float>(cnt[i]) / size;
	}
}

void IppHistogramStretching(IppByteImage& img)
{
	int size = img.GetSize();
	BYTE* p = img.GetPixels();

	// 최대, 최소 그레이스케일 값 계산
	BYTE gray_max, gray_min;
	gray_max = gray_min = p[0];
	for (int i = 1; i < size; i++)
	{
		if (gray_max < p[i]) gray_max = p[i];
		if (gray_min > p[i]) gray_min = p[i];
	}

	if (gray_max == gray_min)
		return;

	// 히스토그램 스트레칭
	for (int i = 0; i < size; i++)
	{
		p[i] = (p[i] - gray_min) * 255 / (gray_max - gray_min);
	}
}

void IppHistogramEqualization(IppByteImage& img)
{
	int size = img.GetSize();
	BYTE* p = img.GetPixels();

	// 히스토그램 계산
	float hist[256];
	IppHistogram(img, hist);

	// 히스토그램 누적 함수 계산
	float cdf[256] = { 0.0, };
	cdf[0] = hist[0];
	for (int i = 1; i < 256; i++)
		cdf[i] = cdf[i - 1] + hist[i];

	// 히스토그램 균등화
	for (int i = 0; i < size; i++)
	{
		p[i] = static_cast<BYTE>(limit(cdf[p[i]] * 255));
	}
}

bool IppAdd(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3)
{
	int w = img1.GetWidth();
	int h = img1.GetHeight();

	if (w != img2.GetWidth() || h != img2.GetHeight())
		return false;

	img3.CreateImage(w, h);

	int size = img3.GetSize();
	BYTE* p1 = img1.GetPixels();
	BYTE* p2 = img2.GetPixels();
	BYTE* p3 = img3.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p3[i] = limit(p1[i] + p2[i]);
	}

	return true;
}

bool IppSub(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3)
{
	int w = img1.GetWidth();
	int h = img1.GetHeight();

	if (w != img2.GetWidth() || h != img2.GetHeight())
		return false;

	img3.CreateImage(w, h);

	int size = img3.GetSize();
	BYTE* p1 = img1.GetPixels();
	BYTE* p2 = img2.GetPixels();
	BYTE* p3 = img3.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p3[i] = limit(p1[i] - p2[i]);
	}

	return true;
}

bool IppAve(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3)
{
	int w = img1.GetWidth();
	int h = img1.GetHeight();

	if (w != img2.GetWidth() || h != img2.GetHeight())
		return false;

	img3.CreateImage(w, h);

	int size = img3.GetSize();
	BYTE* p1 = img1.GetPixels();
	BYTE* p2 = img2.GetPixels();
	BYTE* p3 = img3.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p3[i] = (p1[i] + p2[i]) / 2;
	}

	return true;
}

bool IppDiff(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3)
{
	int w = img1.GetWidth();
	int h = img1.GetHeight();

	if (w != img2.GetWidth() || h != img2.GetHeight())
		return false;

	img3.CreateImage(w, h);

	int size = img3.GetSize();
	BYTE* p1 = img1.GetPixels();
	BYTE* p2 = img2.GetPixels();
	BYTE* p3 = img3.GetPixels();

	int diff;
	for (int i = 0; i < size; i++)
	{
		diff = p1[i] - p2[i];
		p3[i] = static_cast<BYTE>((diff >= 0) ? diff : -diff);
	}

	return true;
}

bool IppAND(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3)
{
	int w = img1.GetWidth();
	int h = img1.GetHeight();

	if (w != img2.GetWidth() || h != img2.GetHeight())
		return false;

	img3.CreateImage(w, h);

	int size = img3.GetSize();
	BYTE* p1 = img1.GetPixels();
	BYTE* p2 = img2.GetPixels();
	BYTE* p3 = img3.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p3[i] = static_cast<BYTE>(p1[i] & p2[i]);
	}

	return true;
}

bool IppOR(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3)
{
	int w = img1.GetWidth();
	int h = img1.GetHeight();

	if (w != img2.GetWidth() || h != img2.GetHeight())
		return false;

	img3.CreateImage(w, h);

	int size = img3.GetSize();
	BYTE* p1 = img1.GetPixels();
	BYTE* p2 = img2.GetPixels();
	BYTE* p3 = img3.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p3[i] = static_cast<BYTE>(p1[i] | p2[i]);
	}

	return true;
}

void IppBitPlane(IppByteImage& img1, IppByteImage& img2, int bit)
{
	img2.CreateImage(img1.GetWidth(), img1.GetHeight());

	int size = img1.GetSize();
	BYTE* p1 = img1.GetPixels();
	BYTE* p2 = img2.GetPixels();

	for (int i = 0; i < size; i++)
	{
		p2[i] = (p1[i] & (1 << bit)) ? 255 : 0;
	}
}

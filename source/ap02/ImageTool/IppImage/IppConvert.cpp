/******************************************************************************
 *
 * IppConvert.cpp
 *
 * Copyright (c) 2015~<current> by Sun-Kyoo Hwang <sunkyoo.ippbook@gmail.com>
 *
 * This source code is included in the book titled "Image Processing 
 * Programming By Visual C++ (2nd Edition)"
 *
 *****************************************************************************/

#include "stdafx.h"
#include <assert.h>
#include "IppConvert.h"

void IppDibToImage(IppDib& dib, IppByteImage& img)
{
	assert(dib.IsValid());
	assert(dib.GetBitCount() == 8);

	int w = dib.GetWidth();
	int h = dib.GetHeight();
	int ws = (w + 3) & ~3;
	BYTE* pDIBits = dib.GetDIBitsAddr();

	img.CreateImage(w, h);
	BYTE** pixels = img.GetPixels2D();

	for (int i = 0; i < h; i++)
	{
		memcpy(pixels[i], &pDIBits[(h - 1 - i) * ws], w);
	}
}

void IppDibToImage(IppDib& dib, IppRgbImage& img)
{
	assert(dib.IsValid());
	assert(dib.GetBitCount() == 24);

	int w = dib.GetWidth();
	int h = dib.GetHeight();
	int ws = (w * 3 + 3) & ~3;
	BYTE* pDIBits = dib.GetDIBitsAddr();

	img.CreateImage(w, h);
	RGBBYTE** pixels = img.GetPixels2D();

	for (int i = 0; i < h; i++)
	{
		memcpy(pixels[i], &pDIBits[(h - 1 - i) * ws], w * 3);
	}
}

void IppImageToDib(IppByteImage& img, IppDib& dib)
{
	assert(img.IsValid());

	int w = img.GetWidth();
	int h = img.GetHeight();
	int ws = (w + 3) & ~3;
	BYTE** pixels = img.GetPixels2D();

	dib.CreateGrayBitmap(w, h);
	BYTE* pDIBits = dib.GetDIBitsAddr();

	for (int i = 0; i < h; i++)
	{
		memcpy(&pDIBits[(h - 1 - i) * ws], pixels[i], w);
	}
}

void IppImageToDib(IppFloatImage& img, IppDib& dib)
{
	assert(img.IsValid());

	int w = img.GetWidth();
	int h = img.GetHeight();
	int ws = (w + 3) & ~3;
	float** pixels = img.GetPixels2D();

	dib.CreateGrayBitmap(w, h);
	BYTE* pDIBits = dib.GetDIBitsAddr();

	int i, j;
	for (j = 0; j < h; j++)
	for (i = 0; i < w; i++)
	{
		pDIBits[(h - 1 - j) * ws + i] = static_cast<BYTE>(limit(pixels[j][i] + 0.5f));
	}
}

void IppImageToDib(IppRgbImage& img, IppDib& dib)
{
	assert(img.IsValid());

	int w = img.GetWidth();
	int h = img.GetHeight();
	int ws = (w * 3 + 3) & ~3;
	RGBBYTE** pixels = img.GetPixels2D();

	dib.CreateRgbBitmap(w, h);
	BYTE* pDIBits = dib.GetDIBitsAddr();

	for (int i = 0; i < h; i++)
	{
		memcpy(&pDIBits[(h - 1 - i) * ws], pixels[i], w * 3);
	}
}

void IppImageToMat(IppByteImage& img, cv::Mat& mat)
{
	int w = img.GetWidth();
	int h = img.GetHeight();

	mat.create(h, w, CV_8U);

	BYTE** pSrc = img.GetPixels2D();

	BYTE* pDst;
	for (int j = 0; j < h; j++)
	{
		pDst = mat.ptr<BYTE>(j);
		memcpy(pDst, pSrc[j], sizeof(BYTE) * w);
	}
}

void IppImageToMat(IppRgbImage& img, cv::Mat& mat)
{
	int w = img.GetWidth();
	int h = img.GetHeight();

	mat.create(h, w, CV_8UC3);

	RGBBYTE** pSrc = img.GetPixels2D();

	RGBBYTE* pDst;
	for (int j = 0; j < h; j++)
	{
		pDst = mat.ptr<RGBBYTE>(j);
		memcpy(pDst, pSrc[j], sizeof(RGBBYTE) * w);
	}
}

void IppMatToImage(cv::Mat& mat, IppByteImage& img)
{
	CV_Assert(mat.depth() == CV_8U);

	int w = mat.cols;
	int h = mat.rows;

	img.CreateImage(w, h);

	BYTE** pDst = img.GetPixels2D();

	BYTE* pSrc;
	for (int j = 0; j < h; j++)
	{
		pSrc = mat.ptr<BYTE>(j);
		memcpy(pDst[j], pSrc, sizeof(BYTE) * w);
	}
}

void IppMatToImage(cv::Mat& mat, IppRgbImage& img)
{
	CV_Assert(mat.depth() == CV_8UC3);

	int w = mat.cols;
	int h = mat.rows;

	img.CreateImage(w, h);

	RGBBYTE** pDst = img.GetPixels2D();

	RGBBYTE* pSrc;
	for (int j = 0; j < h; j++)
	{
		pSrc = mat.ptr<RGBBYTE>(j);
		memcpy(pDst[j], pSrc, sizeof(RGBBYTE) * w);
	}
}

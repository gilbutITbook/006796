/******************************************************************************
*
* IppGeometry.cpp
*
* Copyright (c) 2015~<current> by Sun-Kyoo Hwang <sunkyoo.ippbook@gmail.com>
*
* This source code is included in the book titled "Image Processing
* Programming By Visual C++ (2nd Edition)"
*
*****************************************************************************/

#include "stdafx.h"
#include "IppGeometry.h"

#include <math.h>

const double PI = 3.14159265358979323846;

void IppTranslate(IppByteImage& imgSrc, IppByteImage& imgDst, int sx, int sy)
{
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst.CreateImage(w, h);

	BYTE** pSrc = imgSrc.GetPixels2D();
	BYTE** pDst = imgDst.GetPixels2D();

	int i, j, x, y;
	for (j = 0; j < h; j++)
	for (i = 0; i < w; i++)
	{
		x = i - sx;
		y = j - sy;
		if (x >= 0 && x < w && y >= 0 && y < h)
			pDst[j][i] = pSrc[y][x];
	}
}

void IppResizeNearest(IppByteImage& imgSrc, IppByteImage& imgDst, int nw, int nh)
{
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst.CreateImage(nw, nh);

	BYTE** pSrc = imgSrc.GetPixels2D();
	BYTE** pDst = imgDst.GetPixels2D();

	int i, j, x, y;
	double rx, ry;
	for (j = 0; j < nh; j++)
	for (i = 0; i < nw; i++)
	{
		rx = static_cast<double>(w - 1) * i / (nw - 1);
		ry = static_cast<double>(h - 1) * j / (nh - 1);
		x = static_cast<int>(rx + 0.5);
		y = static_cast<int>(ry + 0.5);

		if (x >= w) x = w - 1;
		if (y >= h) y = h - 1;

		pDst[j][i] = pSrc[y][x];
	}
}

void IppResizeBilinear(IppByteImage& imgSrc, IppByteImage& imgDst, int nw, int nh)
{
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst.CreateImage(nw, nh);

	BYTE** pSrc = imgSrc.GetPixels2D();
	BYTE** pDst = imgDst.GetPixels2D();

	int i, j, x1, y1, x2, y2;
	double rx, ry, p, q, value;

	for (j = 0; j < nh; j++)
	for (i = 0; i < nw; i++)
	{
		rx = static_cast<double>(w - 1) * i / (nw - 1);
		ry = static_cast<double>(h - 1) * j / (nh - 1);

		x1 = static_cast<int>(rx);
		y1 = static_cast<int>(ry);

		x2 = x1 + 1; if (x2 == w) x2 = w - 1;
		y2 = y1 + 1; if (y2 == h) y2 = h - 1;

		p = rx - x1;
		q = ry - y1;

		value = (1. - p) * (1. - q) * pSrc[y1][x1] 
			+ p * (1. - q) * pSrc[y1][x2]
			+ (1. - p) * q * pSrc[y2][x1] 
			+ p * q * pSrc[y2][x2];

		pDst[j][i] = static_cast<BYTE>(limit(value + .5));
	}
}

void IppResizeCubic(IppByteImage& imgSrc, IppByteImage& imgDst, int nw, int nh)
{
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst.CreateImage(nw, nh);

	BYTE** pSrc = imgSrc.GetPixels2D();
	BYTE** pDst = imgDst.GetPixels2D();

	int i, j, x1, x2, x3, x4, y1, y2, y3, y4;
	double v1, v2, v3, v4, v;
	double rx, ry, p, q;

	for (j = 0; j < nh; j++)
	for (i = 0; i < nw; i++)
	{
		rx = static_cast<double>(w - 1) * i / (nw - 1);
		ry = static_cast<double>(h - 1) * j / (nh - 1);

		x2 = static_cast<int>(rx);
		x1 = x2 - 1; if (x1 <  0) x1 = 0;
		x3 = x2 + 1; if (x3 >= w) x3 = w - 1;
		x4 = x2 + 2; if (x4 >= w) x4 = w - 1;
		p = rx - x2;

		y2 = static_cast<int>(ry);
		y1 = y2 - 1; if (y1 <  0) y1 = 0;
		y3 = y2 + 1; if (y3 >= h) y3 = h - 1;
		y4 = y2 + 2; if (y4 >= h) y4 = h - 1;
		q = ry - y2;

		v1 = cubic_interpolation(pSrc[y1][x1], pSrc[y1][x2], pSrc[y1][x3], pSrc[y1][x4], p);
		v2 = cubic_interpolation(pSrc[y2][x1], pSrc[y2][x2], pSrc[y2][x3], pSrc[y2][x4], p);
		v3 = cubic_interpolation(pSrc[y3][x1], pSrc[y3][x2], pSrc[y3][x3], pSrc[y3][x4], p);
		v4 = cubic_interpolation(pSrc[y4][x1], pSrc[y4][x2], pSrc[y4][x3], pSrc[y4][x4], p);

		v = cubic_interpolation(v1, v2, v3, v4, q);

		pDst[j][i] = static_cast<BYTE>(limit(v + 0.5));
	}
}

double cubic_interpolation(double v1, double v2, double v3, double v4, double d)
{
	double v, p1, p2, p3, p4;

	p1 = 2 * v2;
	p2 = -v1 + v3;
	p3 = 2 * v1 - 5 * v2 + 4 * v3 - v4;
	p4 = -v1 + 3 * v2 - 3 * v3 + v4;

	v = (p1 + d * (p2 + d * (p3 + d * p4))) / 2.;

	return v;
}

void IppRotate(IppByteImage& imgSrc, IppByteImage& imgDst, double angle)
{
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	double rad = (angle * PI) / 180.;
	double cos_value = cos(rad);
	double sin_value = sin(rad);

	// 회전 후 생성되는 영상의 크기를 계산.
	// 4개의 코너 포인트의 이동 좌표를 계산하여 최대, 최소점의 차이를 구한다.

	int nx, ny, minx, miny, maxx, maxy, nw, nh;

	minx = maxx = 0;
	miny = maxy = 0;

	nx = static_cast<int>(floor((w - 1)* cos_value + 0.5));
	ny = static_cast<int>(floor((w - 1) * sin_value + 0.5));
	minx = (minx < nx) ? minx : nx;	maxx = (maxx > nx) ? maxx : nx;
	miny = (miny < ny) ? miny : ny;	maxy = (maxy > ny) ? maxy : ny;

	nx = static_cast<int>(floor(-(h - 1) * sin_value + 0.5));
	ny = static_cast<int>(floor( (h - 1) * cos_value + 0.5));
	minx = (minx < nx) ? minx : nx;	maxx = (maxx > nx) ? maxx : nx;
	miny = (miny < ny) ? miny : ny;	maxy = (maxy > ny) ? maxy : ny;

	nx = static_cast<int>(floor((w - 1) * cos_value - (h - 1) * sin_value + 0.5));
	ny = static_cast<int>(floor((w - 1) * sin_value + (h - 1) * cos_value + 0.5));
	minx = (minx < nx) ? minx : nx;	maxx = (maxx > nx) ? maxx : nx;
	miny = (miny < ny) ? miny : ny;	maxy = (maxy > ny) ? maxy : ny;

	nw = maxx - minx + 1;
	nh = maxy - miny + 1;

	// 실제 회전 변환

	imgDst.CreateImage(nw, nh);

	BYTE** pSrc = imgSrc.GetPixels2D();
	BYTE** pDst = imgDst.GetPixels2D();

	int i, j, x1, x2, y1, y2;
	double rx, ry, p, q, temp;

	for (j = miny; j <= maxy; j++)
	for (i = minx; i <= maxx; i++)
	{
		rx =  i * cos_value + j * sin_value;
		ry = -i * sin_value + j * cos_value;

		x1 = static_cast<int>(rx);
		y1 = static_cast<int>(ry);

		// 원본 영상 내에 포함된 좌표가 아니라면 무시.
		if (x1 < 0 || x1 > w - 1 || y1 < 0 || y1 > h - 1)
			continue;

		x2 = x1 + 1; if (x2 == w) x2 = w - 1;
		y2 = y1 + 1; if (y2 == h) y2 = h - 1;

		p = rx - x1;
		q = ry - y1;

		temp = (1.0 - p) * (1.0 - q) * pSrc[y1][x1] 
			+ p * (1.0 - q) * pSrc[y1][x2]
			+ (1.0 - p) * q * pSrc[y2][x1] 
			+ p * q * pSrc[y2][x2];

		pDst[j - miny][i - minx] = static_cast<BYTE>(limit(temp));
	}
}

void IppRotate90(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst.CreateImage(h, w);

	BYTE** pSrc = imgSrc.GetPixels2D();
	BYTE** pDst = imgDst.GetPixels2D();

	int i, j;
	for (j = 0; j < w; j++)
	for (i = 0; i < h; i++)
	{
		pDst[j][i] = pSrc[h - 1 - i][j];
	}
}

void IppRotate180(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst.CreateImage(w, h);

	BYTE** pSrc = imgSrc.GetPixels2D();
	BYTE** pDst = imgDst.GetPixels2D();

	int i, j;
	for (j = 0; j < h; j++)
	for (i = 0; i < w; i++)
	{
		pDst[j][i] = pSrc[h - 1 - j][w - 1 - i];
	}
}

void IppRotate270(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	IppByteImage cpy = imgSrc;

	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst.CreateImage(h, w);

	BYTE** pSrc = imgSrc.GetPixels2D();
	BYTE** pDst = imgDst.GetPixels2D();

	int i, j;
	for (j = 0; j < w; j++)
	for (i = 0; i < h; i++)
	{
		pDst[j][i] = pSrc[i][w - 1 - j];
	}
}

void IppMirror(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst.CreateImage(w, h);

	BYTE** pSrc = imgSrc.GetPixels2D();
	BYTE** pDst = imgDst.GetPixels2D();

	int i, j;
	for (j = 0; j < h; j++)
	for (i = 0; i < w; i++)
	{
		pDst[j][i] = pSrc[j][w - 1 - i];
	}
}

void IppFlip(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst.CreateImage(w, h);

	BYTE** pSrc = imgSrc.GetPixels2D();
	BYTE** pDst = imgDst.GetPixels2D();

	int i, j;
	for (j = 0; j < h; j++)
	for (i = 0; i < w; i++)
	{
		pDst[j][i] = pSrc[h - 1 - j][i];
	}
}


/******************************************************************************
*
* IppSegment.cpp
*
* Copyright (c) 2015~<current> by Sun-Kyoo Hwang <sunkyoo.ippbook@gmail.com>
*
* This source code is included in the book titled "Image Processing
* Programming By Visual C++ (2nd Edition)"
*
*****************************************************************************/

#include "StdAfx.h"
#include "IppSegment.h"
#include "IppEnhance.h"

#include <map>

void IppBinarization(IppByteImage& imgSrc, IppByteImage& imgDst, int threshold)
{
	imgDst.CreateImage(imgSrc.GetWidth(), imgSrc.GetHeight());

	int size = imgSrc.GetSize();
	BYTE* pSrc = imgSrc.GetPixels();
	BYTE* pDst = imgDst.GetPixels();

	for (int i = 0; i < size; i++)
	{
		pDst[i] = (pSrc[i] <= threshold) ? 0 : 255;
	}
}

int IppBinarizationIterative(IppByteImage& imgSrc)
{
	float hist[256] = { 0, };
	IppHistogram(imgSrc, hist); // 정규화된 히스토그램. hist 배열의 범위는 [0, 1].

	// 초기 임계값 결정 - 그레이스케일 값의 전체 평균

	int i, T, Told;

	float sum = 0.f;
	for (i = 0; i < 256; i++)
		sum += (i * hist[i]);

	T = static_cast<int>(sum + .5f);

	// 반복에 의한 임계값 결정

	float a1, b1, u1, a2, b2, u2;
	do {
		Told = T;

		a1 = b1 = u1 = 0.f;
		for (i = 0; i <= Told; i++)
		{
			a1 += (i*hist[i]);
			b1 += hist[i];
		}

		if (b1 != 0.f)
			u1 = a1 / b1;

		a2 = b2 = u2 = 0.f;
		for (i = Told + 1; i < 256; i++)
		{
			a2 += (i*hist[i]);
			b2 += hist[i];
		}

		if (b2 != 0.f)
			u2 = a2 / b2;

		T = static_cast<int>((u1 + u2) / 2 + 0.5f);
	} while (T != Told);

	return T;
}

int IppLabeling(IppByteImage& imgSrc, IppIntImage& imgDst, std::vector<IppLabelInfo>& labels)
{
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	BYTE** pSrc = imgSrc.GetPixels2D();

	//-------------------------------------------------------------------------
	// 임시로 레이블을 저장할 메모리 공간과 등가 테이블 생성
	//-------------------------------------------------------------------------

	IppIntImage imgMap(w, h);
	int** pMap = imgMap.GetPixels2D();

	const int MAX_LABEL = 100000;
	int eq_tbl[MAX_LABEL][2] = { { 0, }, };

	//-------------------------------------------------------------------------
	// 첫 번째 스캔 - 초기 레이블 지정 및 등가 테이블 생성
	//-------------------------------------------------------------------------

	register int i, j;
	int label = 0, maxl, minl, min_eq, max_eq;

	for (j = 1; j < h; j++)
	for (i = 1; i < w; i++)
	{
		if (pSrc[j][i] == 255)
		{
			// 바로 위 픽셀과 왼쪽 픽셀 모두에 레이블이 존재하는 경우
			if ((pMap[j - 1][i] != 0) && (pMap[j][i - 1] != 0))
			{
				if (pMap[j - 1][i] == pMap[j][i - 1])
				{
					// 두 레이블이 서로 같은 경우
					pMap[j][i] = pMap[j - 1][i];
				}
				else
				{
					// 두 레이블이 서로 다른 경우, 작은 레이블을 부여
					maxl = max(pMap[j - 1][i], pMap[j][i - 1]);
					minl = min(pMap[j - 1][i], pMap[j][i - 1]);

					pMap[j][i] = minl;

					// 등가 테이블 조정
					min_eq = min(eq_tbl[maxl][1], eq_tbl[minl][1]);
					max_eq = max(eq_tbl[maxl][1], eq_tbl[minl][1]);

					eq_tbl[eq_tbl[max_eq][1]][1] = min_eq;
				}
			}
			else if (pMap[j - 1][i] != 0)
			{
				// 바로 위 픽셀에만 레이블이 존재할 경우
				pMap[j][i] = pMap[j - 1][i];
			}
			else if (pMap[j][i - 1] != 0)
			{
				// 바로 왼쪽 픽셀에만 레이블이 존재할 경우
				pMap[j][i] = pMap[j][i - 1];
			}
			else
			{
				// 이웃에 레이블이 존재하지 않으면 새로운 레이블을 부여
				label++;
				pMap[j][i] = label;
				eq_tbl[label][0] = label;
				eq_tbl[label][1] = label;
			}
		}
	}

	//-------------------------------------------------------------------------
	// 등가 테이블 정리
	//-------------------------------------------------------------------------

	int temp;
	for (i = 1; i <= label; i++)
	{
		temp = eq_tbl[i][1];
		if (temp != eq_tbl[i][0])
			eq_tbl[i][1] = eq_tbl[temp][1];
	}

	// 등가 테이블의 레이블을 1부터 차례대로 증가시키기

	int* hash = new int[label + 1];
	memset(hash, 0, sizeof(int)*(label + 1));

	for (i = 1; i <= label; i++)
		hash[eq_tbl[i][1]] = eq_tbl[i][1];

	int label_cnt = 1;
	for (i = 1; i <= label; i++)
		if (hash[i] != 0)
			hash[i] = label_cnt++;

	for (i = 1; i <= label; i++)
		eq_tbl[i][1] = hash[eq_tbl[i][1]];

	delete[] hash;

	//-------------------------------------------------------------------------
	// 두 번째 스캔 - 등가 테이블을 이용하여 모든 픽셀에 고유의 레이블 부여
	//-------------------------------------------------------------------------

	imgDst.CreateImage(w, h);
	int** pDst = imgDst.GetPixels2D();

	int idx;
	for (j = 1; j < h; j++)
	for (i = 1; i < w; i++)
	{
		if (pMap[j][i] != 0)
		{
			idx = pMap[j][i];
			pDst[j][i] = eq_tbl[idx][1];
		}
	}

	//-------------------------------------------------------------------------
	// IppLabelInfo 정보 작성
	//-------------------------------------------------------------------------

	labels.resize(label_cnt - 1);

	IppLabelInfo* pLabel;
	for (j = 1; j < h; j++)
	for (i = 1; i < w; i++)
	{
		if (pDst[j][i] != 0)
		{
			pLabel = &labels.at(pDst[j][i] - 1);
			pLabel->pixels.push_back(IppPoint(i, j));
			pLabel->cx += i;
			pLabel->cy += j;

			if (i < pLabel->minx) pLabel->minx = i;
			if (i > pLabel->maxx) pLabel->maxx = i;
			if (j < pLabel->miny) pLabel->miny = j;
			if (j > pLabel->maxy) pLabel->maxy = j;
		}
	}

	for (IppLabelInfo& label : labels)
	{
		label.cx /= label.pixels.size();
		label.cy /= label.pixels.size();
	}

	return (label_cnt - 1);
}


void IppContourTracing(IppByteImage& imgSrc, int sx, int sy, std::vector<IppPoint>& cp)
{
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	BYTE** pSrc = imgSrc.GetPixels2D();

	// 외곽선 좌표를 저장할 구조체 초기화
	cp.clear();

	// 외곽선 추적 시작 픽셀이 객체가 아니면 종료
	if (pSrc[sy][sx] != 255)
		return;

	int x, y, nx, ny;
	int d, cnt;
	int  dir[8][2] = { // 진행 방향을 나타내는 배열
		{  1,  0 },
		{  1,  1 },
		{  0,  1 },
		{ -1,  1 },
		{ -1,  0 },
		{ -1, -1 },
		{  0, -1 },
		{  1, -1 }
	};

	x = sx;
	y = sy;
	d = cnt = 0;

	while (1)
	{
		nx = x + dir[d][0];
		ny = y + dir[d][1];

		if (nx < 0 || nx >= w || ny < 0 || ny >= h || pSrc[ny][nx] == 0)
		{
			// 진행 방향에 있는 픽셀이 객체가 아닌 경우,
			// 시계 방향으로 진행 방향을 바꾸고 다시 시도한다.

			if (++d > 7) d = 0;
			cnt++;

			// 8방향 모두 배경인 경우 
			if (cnt >= 8)
			{
				cp.push_back(IppPoint(x, y));
				break;  // 외곽선 추적을 끝냄.
			}
		}
		else
		{
			// 진행 방향의 픽셀이 객체일 경우, 현재 점을 외곽선 정보에 저장
			cp.push_back(IppPoint(x, y));

			// 진행 방향으로 이동
			x = nx;
			y = ny;

			// 방향 정보 초기화
			cnt = 0;
			d = (d + 6) % 8;	// d = d - 2 와 같은 형태
		}

		// 시작점으로 돌아왔고, 진행 방향이 초기화된 경우
		// 외곽선 추적을 끝낸다.
		if (x == sx && y == sy && d == 0)
			break;
	}
}



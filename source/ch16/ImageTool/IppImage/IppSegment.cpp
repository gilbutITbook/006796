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
#include "IppFourier.h"
#include "IppGeometry.h"

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

void IppMorphologyErosion(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	int i, j;
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst = imgSrc;

	BYTE** pDst = imgDst.GetPixels2D();
	BYTE** pSrc = imgSrc.GetPixels2D();

	for (j = 1; j < h - 1; j++)
	for (i = 1; i < w - 1; i++)
	{
		if (pSrc[j][i] != 0)
		{
			if (pSrc[j - 1][i    ] == 0 || pSrc[j - 1][i + 1] == 0 ||
				pSrc[j    ][i - 1] == 0 || pSrc[j    ][i + 1] == 0 ||
				pSrc[j + 1][i - 1] == 0 || pSrc[j + 1][i    ] == 0 ||
				pSrc[j + 1][i + 1] == 0 || pSrc[j - 1][i - 1] == 0)
			{
				pDst[j][i] = 0;
			}
		}
	}
}

void IppMorphologyDilation(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	int i, j;
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst = imgSrc;

	BYTE** pDst = imgDst.GetPixels2D();
	BYTE** pSrc = imgSrc.GetPixels2D();

	for (j = 1; j < h - 1; j++)
	for (i = 1; i < w - 1; i++)
	{
		if (pSrc[j][i] == 0)
		{
			if (pSrc[j - 1][i    ] != 0 || pSrc[j - 1][i + 1] != 0 ||
				pSrc[j    ][i - 1] != 0 || pSrc[j    ][i + 1] != 0 ||
				pSrc[j + 1][i - 1] != 0 || pSrc[j + 1][i    ] != 0 ||
				pSrc[j + 1][i + 1] != 0 || pSrc[j - 1][i - 1] != 0)
			{
				pDst[j][i] = 255;
			}
		}
	}
}

void IppMorphologyOpening(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	IppByteImage imgTmp;
	IppMorphologyErosion(imgSrc, imgTmp);
	IppMorphologyDilation(imgTmp, imgDst);
}

void IppMorphologyClosing(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	IppByteImage imgTmp;
	IppMorphologyDilation(imgSrc, imgTmp);
	IppMorphologyErosion(imgTmp, imgDst);
}

void IppMorphologyGrayErosion(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	int i, j, m, n, x, y, pmin;
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst = imgSrc;

	BYTE** pDst = imgDst.GetPixels2D();
	BYTE** pSrc = imgSrc.GetPixels2D();

	for (j = 0; j < h; j++)
	for (i = 0; i < w; i++)
	{
		pmin = 255;

		for (n = -1; n <= 1; n++)
		for (m = -1; m <= 1; m++)
		{
			x = i + m;
			y = j + n;

			if (x >= 0 && x < w && y >= 0 && y < h)
			{
				if (pSrc[y][x] < pmin)
					pmin = pSrc[y][x];
			}
		}

		pDst[j][i] = pmin;
	}
}

void IppMorphologyGrayDilation(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	int i, j, m, n, x, y, pmax;
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst = imgSrc;

	BYTE** pDst = imgDst.GetPixels2D();
	BYTE** pSrc = imgSrc.GetPixels2D();

	for (j = 0; j < h; j++)
	for (i = 0; i < w; i++)
	{
		pmax = 0;

		for (n = -1; n <= 1; n++)
		for (m = -1; m <= 1; m++)
		{
			x = i + m;
			y = j + n;

			if (x >= 0 && x < w && y >= 0 && y < h)
			{
				if (pSrc[y][x] > pmax)
					pmax = pSrc[y][x];
			}
		}

		pDst[j][i] = pmax;
	}
}

void IppMorphologyGrayOpening(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	IppByteImage imgTmp;
	IppMorphologyGrayErosion(imgSrc, imgTmp);
	IppMorphologyGrayDilation(imgTmp, imgDst);
}

void IppMorphologyGrayClosing(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	IppByteImage imgTmp;
	IppMorphologyGrayDilation(imgSrc, imgTmp);
	IppMorphologyGrayErosion(imgTmp, imgDst);
}

void IppFourierDescriptor(IppByteImage& img, int sx, int sy, int percent, std::vector<IppPoint>& cp)
{
	// 외곽선 좌표 구하기
	IppContourTracing(img, sx, sy, cp);

	// 푸리에 기술자 구하기
	int num = cp.size();

	double* x = new double[num];
	double* y = new double[num];

	for (int i = 0; i < num; i++)
	{
		x[i] = static_cast<double>(cp[i].x);
		y[i] = static_cast<double>(cp[i].y);
	}

	DFT1d(x, y, num, 1);	// 이산 푸리에 변환

	// 푸리에 기술자의 일부를 이용하여 외곽선 좌표 복원하기
	int p = num * percent / 100;
	for (int i = p; i < num; i++)
	{
		x[i] = 0.;
		y[i] = 0.;
	}

	DFT1d(x, y, num, -1);	// 이산 푸리에 역변환

	int w = img.GetWidth();
	int h = img.GetHeight();

	cp.clear();
	IppPoint pt;
	for (int i = 0; i < num; i++)
	{
		pt.x = limit(static_cast<int>(x[i] + 0.5), 0, w - 1);
		pt.y = limit(static_cast<int>(y[i] + 0.5), 0, h - 1);
		cp.push_back(pt);
	}

	// 동적 할당한 메모리 해제
	delete [] x;
	delete [] y;
}

void IppInvariantMoments(IppByteImage& img, double m[7])
{
	double g00, g10, g01, g20, g02, g11, g30, g03, g21, g12;
	double c00, c20, c02, c11, c30, c03, c21, c12;
	double n20, n02, n11, n30, n03, n21, n12;

	// 기하학적 모멘트 구하기

	g00 = IppGeometricMoment(img, 0, 0);
	g10 = IppGeometricMoment(img, 1, 0);
	g01 = IppGeometricMoment(img, 0, 1);
	g20 = IppGeometricMoment(img, 2, 0);
	g02 = IppGeometricMoment(img, 0, 2);
	g11 = IppGeometricMoment(img, 1, 1);
	g30 = IppGeometricMoment(img, 3, 0);
	g03 = IppGeometricMoment(img, 0, 3);
	g21 = IppGeometricMoment(img, 2, 1);
	g12 = IppGeometricMoment(img, 1, 2);

	// 중심 모멘트 구하기

	double cx = g10 / g00;
	double cy = g01 / g00;

	c00 = g00;
	c20 = g20 - cx * g10;
	c02 = g02 - cy * g01;
	c11 = g11 - cx * g01;
	c30 = g30 - 3 * cx * g20 + 2 * cx * cx * g10;
	c03 = g03 - 3 * cy * g02 + 2 * cy * cy * g01;
	c21 = g21 - 2 * cx * g11 - cy * g20 + 2 * cx * cx * g01;
	c12 = g12 - 2 * cy * g11 - cx * g02 + 2 * cy * cy * g10;

	// 정규화된 중심 모멘트

	n20 = c20 / pow(c00, 2.);
	n02 = c02 / pow(c00, 2.);
	n11 = c11 / pow(c00, 2.);
	n30 = c30 / pow(c00, 2.5);
	n03 = c03 / pow(c00, 2.5);
	n21 = c21 / pow(c00, 2.5);
	n12 = c12 / pow(c00, 2.5);

	// 불변 모멘트 구하기

	m[0] = n20 + n02;
	m[1] = (n20 - n02)*(n20 - n02) + 4 * n11*n11;
	m[2] = (n30 - 3*n12)*(n30 - 3*n12) + (3*n21 - n03)*(3*n21 - n03);
	m[3] = (n30 + n12)*(n30 + n12) + (n21 + n03)*(n21 + n03);
	m[4] = (n30 - 3 * n12)*(n30 + n12)*((n30 + n12)*(n30 + n12) - 3*(n21 + n03)*(n21 + n03))
		+ (3*n21 - n03)*(n21 + n03)*(3*(n30 + n12)*(n30 + n12) - (n21 + n03)*(n21 + n03));
	m[5] = (n20 - n02)*((n30 + n12)*(n30 + n12) - (n21 + n03)*(n21 + n03))
		+ 4*n11*(n30 + n12)*(n21 + n03);
	m[6] = (3*n21 - n03)*(n30 + n12)*((n30 + n12)*(n30 + n12) - 3*(n21 + n03)*(n21 + n03))
		+ (3*n12 - n30)*(n21 + n03)*(3*(n30 + n12)*(n30 + n12) - (n21 + n03)*(n21 + n03));
}

double IppGeometricMoment(IppByteImage& img, int p, int q)
{
	int w = img.GetWidth();
	int h = img.GetHeight();
	BYTE** ptr = img.GetPixels2D();

	register int i, j, k;
	double moment = 0, xp, yq;
	for (j = 0; j < h; j++)
	for (i = 0; i < w; i++)
	{
		xp = yq = 1;
		for (k = 0; k < p; k++) xp *= i;
		for (k = 0; k < q; k++) yq *= j;

		moment += (xp * yq * ptr[j][i]);
	}

	return moment;
}

bool IppZernikeMoments(IppByteImage& img, int n, int m, double& zr, double& zi)
{
	if (n < 0 || ((n - abs(m)) % 2 != 0) || abs(m) > n)
		return false;

	if (n > 8)
		return false;

	const int ZM_RADIUS = 100;
	const int ZM_SIZE = ZM_RADIUS * 2 + 1;
	const int FACT[] = { 1, 1, 2, 6, 24, 120, 720, 5040, 40320};

	// 저니키 방사 다항식 계산

	double zm_poly[ZM_RADIUS + 1] = { 0., };

	int sign;
	double rho;
	for (int r = 0; r <= ZM_RADIUS; r++)
	{
		for (int s = 0; s <= ((n - m) / 2); s++)
		{
			sign = (s % 2 == 0) ? 1 : -1;
			rho = static_cast<double>(r) / ZM_RADIUS;

			zm_poly[r] += (sign * FACT[n - s] * pow(rho, (n - 2 * s)))
				/ (FACT[s] * FACT[(n + m) / 2 - s] * FACT[(n - m) / 2 - s]);
		}
	}

	// 저니키 기저 함수 계산

	double zm_basis_real[ZM_SIZE][ZM_SIZE];
	double zm_basis_imag[ZM_SIZE][ZM_SIZE];
	int x, y, dist;
	double theta;

	for (y = 0; y < ZM_SIZE; y++)
	for (x = 0; x < ZM_SIZE; x++)
	{
		dist = static_cast<int>(hypot(y - ZM_RADIUS, x - ZM_RADIUS));

		if (dist <= ZM_RADIUS)
		{
			theta = atan2(y - ZM_RADIUS, x - ZM_RADIUS);
			zm_basis_real[y][x] = zm_poly[dist] * cos(m*theta);
			zm_basis_imag[y][x] = zm_poly[dist] * sin(m*theta);
		}
	}

	zm_basis_real[ZM_RADIUS][ZM_RADIUS] = zm_poly[0];
	zm_basis_imag[ZM_RADIUS][ZM_RADIUS] = 0.0;

	// 저니키 모멘트 계산

	IppByteImage imgDst;
	IppResizeBilinear(img, imgDst, ZM_SIZE, ZM_SIZE);
	BYTE** pDst = imgDst.GetPixels2D();

	zr = zi = 0;
	for (y = 0; y < ZM_SIZE; y++)
	for (x = 0; x < ZM_SIZE; x++)
	{
		dist = static_cast<int>(hypot(y - ZM_RADIUS, x - ZM_RADIUS));

		if (dist <= ZM_RADIUS)
		{
			zr += ((pDst[y][x] / 255.) * zm_basis_real[y][x]);
			zi -= ((pDst[y][x] / 255.) * zm_basis_imag[y][x]); // Complex conjugate
		}
	}

	return true;
}

IppPoint IppTemplateMatching(IppByteImage& imgSrc, IppByteImage& imgTmpl, IppIntImage& imgMap)
{
	int sw = imgSrc.GetWidth();   // 입력 영상의 가로 크기
	int sh = imgSrc.GetHeight();  // 입력 영상의 세로 크기
	int tw = imgTmpl.GetWidth();  // 템플릿 영상의 가로 크기
	int th = imgTmpl.GetHeight(); // 템플릿 영상의 세로 크기
	int tw2 = tw / 2;
	int th2 = th / 2;

	imgMap.CreateImage(sw, sh);

	BYTE** pSrc = imgSrc.GetPixels2D();
	BYTE** pTmpl = imgTmpl.GetPixels2D();
	int**  pMap = imgMap.GetPixels2D();

	IppPoint dp;

	int i, j, x, y;
	int min_value = 99999;
	int diff, sum_of_diff;
	for (j = th2; j <= sh - th2; j++)
	for (i = tw2; i <= sw - tw2; i++)
	{
		sum_of_diff = 0;
		for (y = 0; y < th; y++)
		for (x = 0; x < tw; x++)
		{
			diff = pSrc[j - th2 + y][i - tw2 + x] - pTmpl[y][x];
			sum_of_diff += (diff * diff);
		}

		pMap[j][i] = sum_of_diff / (tw * th);

		if (pMap[j][i] < min_value)
		{
			min_value = pMap[j][i];
			dp.x = i;
			dp.y = j;
		}
	}

	return dp;
}

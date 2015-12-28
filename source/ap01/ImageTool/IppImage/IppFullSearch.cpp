/******************************************************************************
 *
 * IppFullSearch.cpp
 *
 * Copyright (c) 2015~<current> by Sun-Kyoo Hwang <sunkyoo.ippbook@gmail.com>
 *
 * This source code is included in the book titled "Image Processing
 * Programming By Visual C++ (2nd Edition)"
 *
 *****************************************************************************/

#include "stdafx.h"
#include "IppFullSearch.h"
#include "IppFilter.h"
#include "IppSegment.h"

#define BLOCK_SIZE 16
#define WINDOW_SIZE 3

IppFullSearch::IppFullSearch(void)
{
	m_pImage1 = NULL;
	m_pImage2 = NULL;
	m_pMotion = NULL;
}

IppFullSearch::~IppFullSearch(void)
{
	DeleteMotion();
}

void IppFullSearch::SetImages(IppByteImage* pImage1, IppByteImage* pImage2)
{
	int w = pImage1->GetWidth();
	int h = pImage1->GetHeight();

	ASSERT(w == pImage2->GetWidth() && h == pImage2->GetHeight());

	m_pImage1 = pImage1;
	m_pImage2 = pImage2;

	m_nBlockCol = w / BLOCK_SIZE;
	m_nBlockRow = h / BLOCK_SIZE;

	// m_nBlockCol, m_nBlockRow 값을 설정하였으니
	// Motion vector를 저장할 공간(m_pMotion)을 할당한다.
	InitMotion();
}

void IppFullSearch::InitMotion()
{
	if (m_pMotion) 		
		DeleteMotion();

	m_pMotion = new MotionVector*[m_nBlockRow];
	for (int i = 0; i < m_nBlockRow; i++)
	{
		m_pMotion[i] = new MotionVector[m_nBlockCol];
		memset(m_pMotion[i], 0, sizeof(MotionVector)*m_nBlockCol);
	}
}

void IppFullSearch::DeleteMotion()
{
	if (m_pMotion != NULL)
	{
		for (register int i = 0; i < m_nBlockRow; i++)
			delete[] m_pMotion[i];
		delete[] m_pMotion;
		m_pMotion = NULL;
	}
}

MotionVector** IppFullSearch::GetMotionVector()
{
	return m_pMotion;
}

void IppFullSearch::FullSearch()
{
	for (int j = 0; j < m_nBlockRow; j++)
	for (int i = 0; i < m_nBlockCol; i++)
	{
		// 각 블럭에서 motion vector를 구하여 2차원 배열 m_pMotion에 저장
		m_pMotion[j][i] = BlockMotion(i, j);
	}
}

MotionVector IppFullSearch::BlockMotion(int bx, int by)
{
	int x1, y1, x2, y2;
	double err, min_err;
	MotionVector motion;

	x1 = bx * BLOCK_SIZE;
	y1 = by * BLOCK_SIZE;

	min_err = GetMAD(x1, y1, x1, y1);
	motion.x = 0;
	motion.y = 0;

	for (int j = -WINDOW_SIZE; j <= WINDOW_SIZE; j++)
	for (int i = -WINDOW_SIZE; i <= WINDOW_SIZE; i++)
	{
		if (j == 0 && i == 0) continue;

		x2 = x1 + i;
		y2 = y1 + j;

		err = GetMAD(x1, y1, x2, y2);

		if (err < min_err)
		{
			min_err = err;
			motion.x = i;
			motion.y = j;
		}
	}

	if (min_err < 2.0)
		motion.x = motion.y = 0;

	return motion;
}

double IppFullSearch::GetMAD(int x1, int y1, int x2, int y2)
{
	int w = m_pImage1->GetWidth();
	int h = m_pImage1->GetHeight();

	BYTE** ptr1 = m_pImage1->GetPixels2D();
	BYTE** ptr2 = m_pImage2->GetPixels2D();

	int cnt = 0;
	int temp, sum = 0;

	for (int j = 0; j < BLOCK_SIZE; j++)
	for (int i = 0; i < BLOCK_SIZE; i++)
	{
		if (x1 + i < 0 || x1 + i >= w || y1 + j < 0 || y1 + j >= h ||
			x2 + i < 0 || x2 + i >= w || y2 + j < 0 || y2 + j >= h)
			continue;

		temp = ptr1[y1 + j][x1 + i] - ptr2[y2 + j][x2 + i];
		if (temp < 0) temp = -temp;
		sum += temp;
		cnt++;
	}

	return ((double)sum / cnt);
}

void IppFullSearch::GetMotionImage(IppByteImage& imgMotion)
{
	int w = m_pImage1->GetWidth();
	int h = m_pImage1->GetHeight();

	imgMotion.CreateImage(w, h);

	int cx, cy;
	for (int j = 0; j < m_nBlockRow; j++)
	for (int i = 0; i < m_nBlockCol; i++)
	{
		if (m_pMotion[j][i].x == 0 && m_pMotion[j][i].y == 0)
			continue;

		cx = i * BLOCK_SIZE + BLOCK_SIZE / 2;
		cy = j * BLOCK_SIZE + BLOCK_SIZE / 2;

		IppDrawLine(imgMotion, cx, cy, cx + m_pMotion[j][i].x, cy + m_pMotion[j][i].y, 255);
	}
}


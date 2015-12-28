/******************************************************************************
 *
 * IppFullSearch.h
 *
 * Copyright (c) 2015~<current> by Sun-Kyoo Hwang <sunkyoo.ippbook@gmail.com>
 *
 * This source code is included in the book titled "Image Processing
 * Programming By Visual C++ (2nd Edition)"
 *
 *****************************************************************************/

#pragma once

#include "IppImage.h"

typedef struct _MotionVector
{
	int x;
	int y;
} MotionVector;

class IppFullSearch
{
public:
	IppFullSearch(void);
	~IppFullSearch(void);

public:
	IppByteImage* m_pImage1;
	IppByteImage* m_pImage2;

	MotionVector** m_pMotion;

	int m_nBlockRow;
	int m_nBlockCol;

public:
	void SetImages(IppByteImage* pImage1, IppByteImage* pImage2);
	void FullSearch();
	MotionVector** GetMotionVector();
	void GetMotionImage(IppByteImage& imgMotion);

protected:
	void InitMotion();
	void DeleteMotion();

	MotionVector BlockMotion(int bx, int by);
	double GetMAD(int x1, int y1, int x2, int y2);
};



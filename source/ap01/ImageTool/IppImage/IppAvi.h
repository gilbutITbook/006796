/******************************************************************************
 *
 * IppAvi.h
 *
 * Copyright (c) 2015~<current> by Sun-Kyoo Hwang <sunkyoo.ippbook@gmail.com>
 *
 * This source code is included in the book titled "Image Processing 
 * Programming By Visual C++ (2nd Edition)"
 *
 *****************************************************************************/

#pragma once

#include <vfw.h>
#include "IppDib.h"

class IppAvi
{
public:
	// 생성자
	IppAvi();
	~IppAvi();

public:
	// 멤버 함수
	BOOL    Open(LPCTSTR lpszPathName);
	void    Close();

	// 그리기 함수
	void    DrawFrame(HDC hDC, int nFrame);
	void    DrawFrame(HDC hDC, int nFrame, int dx, int dy);
	void    DrawFrame(HDC hDC, int nFrame, int dx, int dy, int dw, int dh, DWORD dwRop = SRCCOPY);

	// 현재 프레임 캡쳐 함수
	BOOL    GetFrame(int nFrame, IppDib& dib);

	// AVI 파일 정보 반환 함수
	int     GetFrameRate()  { return m_nFrameRate; }
	int     GetTotalFrame() { return m_nTotalFrame; }
	int     GetHeight()     { return m_nHeight; }
	int     GetWidth()      { return m_nWidth; }
	BOOL    IsValid()       { return (m_pAviFile != NULL); };

protected:
	// 멤버 변수
	PAVIFILE    m_pAviFile;         // AVI 파일 인터페이스 포인터
	PAVISTREAM  m_pVideoStream;     // 비디오 스트림 포인터
	PGETFRAME   m_pVideoFrame;      // 비디오 프레임 포인터

	int         m_nWidth;           // 비디오 프레임(영상) 가로 크기
	int         m_nHeight;          // 비디오 프레임(영상) 가로 크기

	int         m_nTotalFrame;      // 전체 프레임 개수
	int         m_nFrameRate;       // 초당 프레임 비율
};

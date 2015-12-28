/******************************************************************************
 *
 * IppAvi.cpp
 *
 * Copyright (c) 2015~<current> by Sun-Kyoo Hwang <sunkyoo.ippbook@gmail.com>
 *
 * This source code is included in the book titled "Image Processing 
 * Programming By Visual C++ (2nd Edition)"
 *
 *****************************************************************************/

#include "stdafx.h"
#include "IppAvi.h"
#include "RGBBYTE.h"

/*************************************************************************
 * 
 * 이름 : IppAvi()
 * 인자 : 없음.
 * 반환 : 없음.
 * 설명 : 기본 생성자. 멤버 변수 초기화.
 * 
 ************************************************************************/
IppAvi::IppAvi()
{
	// AVIFile 라이브러리를 초기화한다.
	AVIFileInit();

	// 멤버 초기화
	m_pAviFile = NULL;
	m_pVideoStream = NULL;
	m_pVideoFrame = NULL;

	m_nWidth = 0;
	m_nHeight = 0;

	m_nTotalFrame = 0;
	m_nFrameRate = 0;
}

/*************************************************************************
 * 
 * 이름 : ~IppAvi()
 * 인자 : 없음.
 * 반환 : 없음.
 * 설명 : 소멸자. AVI 사용을 중지한다.
 * 
 ************************************************************************/

IppAvi::~IppAvi()
{
	Close();

	// AVIFile 라이브러리 사용을 종료한다.
	AVIFileExit();
}

/*************************************************************************
 * 
 * 이름 : Open(LPCTSTR lpszFileName)
 * 인자 : lpszFileName - 불러올 파일의 전체 경로 이름.
 * 반환 : 성공하면 TRUE, 실패하면 FALSE를 반환.
 * 설명 : AVI 파일을 IppAvi 객체로 불러온다.
 * 
 ************************************************************************/

BOOL IppAvi::Open(LPCTSTR lpszPathName)
{
	int i;
	HRESULT hr;

	AVIFILEINFO     aviFileInfo;
	AVISTREAMINFO   aviStreamInfo;
	PAVISTREAM      pAviTmpStream = NULL;

	// 혹시 기존에 파일을 open 한 적이 있으면 닫는다.
	if (m_pAviFile)
	{
		Close();
		m_pAviFile = NULL;
	}

	// AVI 파일을 열고, 파일 인터페이스에 대한 포인터를 m_pAviFile에 저장한다.
	hr = AVIFileOpen(&m_pAviFile, lpszPathName, OF_READ, NULL);
	if (hr)
		return FALSE;

	// AVI 파일에 대한 정보를 얻는다.
	hr = AVIFileInfo(m_pAviFile, &aviFileInfo, sizeof(AVIFILEINFO));
	if (hr)
	{
		Close();
		return FALSE;
	}

	for (i = 0; i < (LONG)aviFileInfo.dwStreams; i++)
	{
		// AVI 파일과 관련된 스트림 인터페이스의 주소를 얻는다. 
		// fccType이 0L 으로 지정되면 모든 스트림을 검색한다.
		hr = AVIFileGetStream(m_pAviFile, &pAviTmpStream, 0L, i);
		if (hr)
			continue;

		// 위에서 받은 스트림 인터페이스에 대한 정보를 얻는다.
		hr = AVIStreamInfo(pAviTmpStream, &aviStreamInfo, sizeof(AVISTREAMINFO));
		if (hr)
			continue;

		if (aviStreamInfo.fccType == streamtypeVIDEO)
		{
			m_pVideoStream = pAviTmpStream;

			// AVI 스트림의 사용 참조 개수를 증가시킨다.
			AVIStreamAddRef(m_pVideoStream);

			// 전체 샘플 수 (VIDEO의 경우 전체 프레임 수)
			m_nTotalFrame = aviStreamInfo.dwLength;

			// 초당 샘플 수 (VIDEO의 경우 초당 프레임 수)
			m_nFrameRate = aviStreamInfo.dwRate / aviStreamInfo.dwScale;

			// 비디오 화면의 가로, 세로 크기
			RECT rc = aviStreamInfo.rcFrame;
			m_nWidth = rc.right - rc.left;
			m_nHeight = rc.bottom - rc.top;

			// 비디오 스트림으로부터 압축되지 않은 비디오 프레임을 준비한다.
			m_pVideoFrame = AVIStreamGetFrameOpen(m_pVideoStream, NULL);
			if (!m_pVideoFrame)
			{
				Close();
				return FALSE;
			}
		}
		else
		{
			// Video 스트림이 아닌 경우 사용하지 않고 그대로 반환
			AVIStreamRelease(pAviTmpStream);
		}
	}

	return TRUE;
}

/*************************************************************************
 * 
 * 이름 : Close()
 * 인자 : 없음.
 * 반환 : 없음.
 * 설명 : AVI 파일의 사용을 중지한다.
 * 
 ************************************************************************/

void IppAvi::Close()
{
	if (m_pVideoFrame != NULL)
	{
		AVIStreamGetFrameClose(m_pVideoFrame);
		m_pVideoFrame = NULL;
	}

	if (m_pVideoStream != NULL)
	{
		AVIStreamRelease(m_pVideoStream);
		m_pVideoStream = NULL;
	}

	if (m_pAviFile != NULL)
	{
		AVIFileRelease(m_pAviFile);
		m_pAviFile = NULL;
	}
}

/*************************************************************************
 * 
 * 이름 : Draw(HDC hDC, int nFrame)
 * 인자 : hDC    - 출력 장치의 DC 핸들
 *        nFrame - 출력할 프레임 번호
 * 반환 : 없음.
 * 설명 : 출력 장치 좌표 (0,0) 위치에 nFrame 번째 프레임을 출력한다.
 * 
 ************************************************************************/

void IppAvi::DrawFrame(HDC hDC, int nFrame)
{	
	BYTE* pDib = (BYTE*)AVIStreamGetFrame(m_pVideoFrame, nFrame);
	if (!pDib)
		return;

	LPBITMAPINFO    lpbi;
	LPSTR           lpDIBBits;

	lpbi = (LPBITMAPINFO)pDib;

	if (lpbi->bmiHeader.biBitCount == 8)
		lpDIBBits = (LPSTR)pDib + sizeof(BITMAPINFOHEADER) + 1024;
	else
		lpDIBBits = (LPSTR)pDib + sizeof(BITMAPINFOHEADER);

	::SetDIBitsToDevice(hDC,	// hDC
		0,						// DestX
		0,						// DestY
		m_nWidth,				// nSrcWidth
		m_nHeight,				// nSrcHeight
		0,						// SrcX
		0,						// SrcY
		0,						// nStartScan
		(WORD)m_nHeight,		// nNumScans
		lpDIBBits,				// lpBits
		lpbi,					// lpBitsInfo
		DIB_RGB_COLORS);		// wUsage
}

/*************************************************************************
 * 
 * 이름 : Draw(HDC hDC, int nFrame, int dx, int dy)
 * 인자 : hDC    - 출력 장치의 DC 핸들
 *        nFrame - 출력할 프레임 번호
 *        dx     - 출력 장치 위치의 x 좌표
 *        dy     - 출력 장치 위치의 y 좌표
 * 반환 : 없음.
 * 설명 : 출력 장치 좌표 (x,y) 위치에 nFrame 번째 프레임을 출력한다.
 * 
 ************************************************************************/

void IppAvi::DrawFrame(HDC hDC, int nFrame, int dx, int dy)
{
	DrawFrame(hDC, nFrame, dx, dy, GetWidth(), GetHeight());
}

/*************************************************************************
 * 
 * 이름 : Draw(HDC hDC, int nFrame, int dx, int dy, int dw, int dh, DWORD dwRop)
 * 인자 : hDC    - 출력 장치의 DC 핸들
 *        nFrame - 출력할 프레임 번호
 *        dx     - 출력할 위치의 x 좌표
 *        dy     - 출력할 위치의 y 좌표
 *        dw     - 출력할 공간의 가로 크기
 *        dh     - 출력할 공간의 세로 크기
 *        dwRop  - 래스터 오퍼레이션 지정.
 * 반환 : 없음.
 * 설명 : 출력 장치 좌표 (dx,dy) 위치에 (dw, dh) 크기로 nFrame 번째 프레임을 
 *        출력한다.
 * 
 ************************************************************************/

void IppAvi::DrawFrame(HDC hDC, int nFrame, int dx, int dy, int dw, int dh, DWORD dwRop)
{
	BYTE* pDib = (BYTE*)AVIStreamGetFrame(m_pVideoFrame, nFrame);
	if (!pDib)
		return;

	LPBITMAPINFO    lpbi;
	LPSTR           lpDIBBits;

	lpbi = (LPBITMAPINFO)pDib;

	if (lpbi->bmiHeader.biBitCount == 8)
		lpDIBBits = (LPSTR)pDib + sizeof(BITMAPINFOHEADER) + 1024;
	else
		lpDIBBits = (LPSTR)pDib + sizeof(BITMAPINFOHEADER);

	::SetStretchBltMode(hDC, COLORONCOLOR);
	::StretchDIBits(hDC,	// hDC
		dx,					// XDest
		dy,					// YDest
		dw,					// nDestWidth
		dh,					// nDestHeight
		0,					// XSrc
		0,					// YSrc
		m_nWidth,			// nSrcWidth
		m_nHeight,			// nSrcHeight
		lpDIBBits,			// lpBits
		lpbi,				// lpBitsInfo
		DIB_RGB_COLORS,		// wUsage
		SRCCOPY);			// dwROP
}

/*************************************************************************
 * 
 * 이름 : GetFrame(int nFrame, IppDib& dib)
 * 인자 : nFrame - 현재 프레임 번호
 *        dib    - 현재 프레임을 저장한 IppDib 객체
 * 반환 : 성공하면 TRUE, 실패하면 FALSE를 반환.
 * 설명 : nFrame 번째 프레임을 dib 에 복사한다. 
 * 
 ************************************************************************/

BOOL IppAvi::GetFrame(int nFrame, IppDib& dib)
{
	LPVOID lpDib;
	LPSTR  lpDIBBits;
	LPBITMAPINFO lpbi;

	lpDib = AVIStreamGetFrame(m_pVideoFrame, nFrame);
	lpbi = (LPBITMAPINFO)lpDib;

	if (lpbi->bmiHeader.biBitCount == 8)
		lpDIBBits = (LPSTR)lpDib + sizeof(BITMAPINFOHEADER) + 1024;
	else
		lpDIBBits = (LPSTR)lpDib + sizeof(BITMAPINFOHEADER);

	if (lpbi->bmiHeader.biBitCount == 8)
	{
		lpDIBBits = (LPSTR)lpDib + sizeof(BITMAPINFOHEADER) + 1024;

		dib.CreateGrayBitmap(m_nWidth, m_nHeight);
		memcpy(dib.GetBitmapInfoAddr(), lpDib, lpbi->bmiHeader.biSize + 1024 +
			lpbi->bmiHeader.biSizeImage);
	}
	else if (lpbi->bmiHeader.biBitCount == 24)
	{
		lpDIBBits = (LPSTR)lpDib + sizeof(BITMAPINFOHEADER);

		dib.CreateRgbBitmap(m_nWidth, m_nHeight);
		memcpy(dib.GetBitmapInfoAddr(), lpDib, lpbi->bmiHeader.biSize
			+ lpbi->bmiHeader.biSizeImage);
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}


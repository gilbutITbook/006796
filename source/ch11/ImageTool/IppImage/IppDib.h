/******************************************************************************
 *
 * IppDib.h
 *
 * Copyright (c) 2015~<current> by Sun-Kyoo Hwang <sunkyoo.ippbook@gmail.com>
 *
 * This source code is included in the book titled "Image Processing 
 * Programming By Visual C++ (2nd Edition)"
 *
 *****************************************************************************/

#pragma once

#include <windows.h>

class IppDib
{
public:
	// 생성자와 소멸자
	IppDib();
	IppDib(const IppDib& dib);
	~IppDib();

	// 비트맵 생성과 소멸
	BOOL CreateGrayBitmap(LONG nWidth, LONG nHeight);
	BOOL CreateRgbBitmap(LONG nWidth, LONG nHeight);
	void DestroyBitmap();

	// 파일 입출력
	BOOL Load(const char* filename);
	BOOL Save(const char* filename);

	// 비트맵 화면 출력
	void Draw(HDC hdc, int dx = 0, int dy = 0);
	void Draw(HDC hdc, int dx, int dy, int dw, int dh, DWORD dwRop = SRCCOPY);
	void Draw(HDC hdc, int dx, int dy, int dw, int dh, int sx, int sy, int sw,
		int sh, DWORD dwRop = SRCCOPY);

	// Windows 환경에서 클립보드로 복사하기 및 붙여넣기
	BOOL CopyToClipboard();
	BOOL PasteFromClipboard();

	// 대입 연산자 재정의
	IppDib& operator=(const IppDib& dib);
	
	// 비트맵 정보 반환
	LONG        GetWidth()    const { return m_nWidth; }
	LONG        GetHeight()	  const { return m_nHeight; }
	WORD        GetBitCount() const { return m_nBitCount; }
	DWORD       GetDibSize()  const { return m_nDibSize;  }
	LPBITMAPINFO GetBitmapInfoAddr() const { return (LPBITMAPINFO)m_pDib; }
	BYTE*       GetDIBitsAddr() const;
	int         GetPaletteNums() const;
	BOOL        IsValid() const { return (m_pDib != NULL); }

private:
	// BMP 파일 입출력
	BOOL        LoadBMP(const char* filename);
	BOOL        SaveBMP(const char* filename);

private:
	LONG    m_nWidth;      // 비트맵 가로 크기 (픽셀 단위)
	LONG    m_nHeight;     // 비트맵 세로 크기 (픽셀 단위)
	WORD    m_nBitCount;   // 픽셀 당 비트 수
	DWORD   m_nDibSize;    // DIB 전체 크기 (BITMAPINFOHEADER + 색상 테이블 + 픽셀 데이터)
	BYTE*   m_pDib;        // DIB 시작 주소 (BITMAPINFOHEADER 시작 주소)
};


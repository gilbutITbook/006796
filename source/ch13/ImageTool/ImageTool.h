
// ImageTool.h : ImageTool 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.


// CImageToolApp:
// 이 클래스의 구현에 대해서는 ImageTool.cpp을 참조하십시오.
//

class IppDib;

class CImageToolApp : public CWinAppEx
{
public:
	CImageToolApp();

public:
	IppDib* m_pNewDib;
	CMultiDocTemplate* m_pImageDocTemplate;

// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현입니다.
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnWindowCloseall();
};

extern CImageToolApp theApp;

// 전역 함수 선언
void AfxNewBitmap(IppDib& dib);
void AfxPrintInfo(CString message);
void AfxPrintInfo(LPCTSTR lpszFormat, ...);

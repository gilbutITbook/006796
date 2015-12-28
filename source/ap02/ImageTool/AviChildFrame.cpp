// AviChildFrame.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "AviChildFrame.h"


// CAviChildFrame

IMPLEMENT_DYNCREATE(CAviChildFrame, CMDIChildWndEx)

CAviChildFrame::CAviChildFrame()
{

}

CAviChildFrame::~CAviChildFrame()
{
}


BEGIN_MESSAGE_MAP(CAviChildFrame, CMDIChildWndEx)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CAviChildFrame 메시지 처리기입니다.




int CAviChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	DWORD dwCtrlStyle = TBSTYLE_FLAT | TBSTYLE_TOOLTIPS | CBRS_SIZE_DYNAMIC;
	DWORD dwStyle = AFX_DEFAULT_TOOLBAR_STYLE;
	const CRect rcBorders(1, 1, 1, 1);
	if (!m_wndToolBar.CreateEx(this, dwCtrlStyle, dwStyle, rcBorders, IDR_AVI_TYPE) ||
		!m_wndToolBar.LoadToolBar(IDR_AVI_TYPE))
	{
		TRACE0("도구 모음을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	m_wndToolBar.SetWindowText(_T("Video Control"));
	DockPane(&m_wndToolBar);

	return 0;
}

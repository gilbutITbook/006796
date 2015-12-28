#pragma once


// CAviChildFrame

class CAviChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CAviChildFrame)

public:
	CAviChildFrame();
	virtual ~CAviChildFrame();

protected:
	DECLARE_MESSAGE_MAP()

public:
	CMFCToolBar    m_wndToolBar;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};



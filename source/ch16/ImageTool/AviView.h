#pragma once

#include "IppImage\IppAvi.h"

// CAviView 뷰입니다.

class CAviView : public CScrollView
{
	DECLARE_DYNCREATE(CAviView)

protected:
	CAviView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CAviView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual void OnInitialUpdate();     // 생성된 후 처음입니다.

	DECLARE_MESSAGE_MAP()

public:
	IppAvi m_Avi;
	int m_nCurrentFrame;	// 현재 재생 위치
	BOOL m_bPlay;
	int m_nMode;	// 재생 모드

	BOOL AviFileOpen(LPCTSTR lpszPathName);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnVideoPlay();
	afx_msg void OnUpdateVideoPlay(CCmdUI *pCmdUI);
	afx_msg void OnVideoStop();
	afx_msg void OnUpdateVideoStop(CCmdUI *pCmdUI);
	afx_msg void OnVideoPause();
	afx_msg void OnUpdateVideoPause(CCmdUI *pCmdUI);
	afx_msg void OnVideoStart();
	afx_msg void OnVideoPrev();
	afx_msg void OnVideoNext();
	afx_msg void OnVideoEnd();
	afx_msg void OnVideoCapture();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnModeNormal();
	afx_msg void OnUpdateModeNormal(CCmdUI *pCmdUI);
	afx_msg void OnModeDifference();
	afx_msg void OnUpdateModeDifference(CCmdUI *pCmdUI);
	afx_msg void OnModeMotion();
	afx_msg void OnUpdateModeMotion(CCmdUI *pCmdUI);
};



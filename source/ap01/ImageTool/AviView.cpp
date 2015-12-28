// AviView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "AviView.h"
#include "MainFrm.h"

#include "IppImage\IppImage.h"
#include "IppImage\IppDib.h"
#include "IppImage\IppConvert.h"
#include "IppImage\IppColor.h"
#include "IppImage\IppEnhance.h"
#include "IppImage\IppFullSearch.h"


// CAviView

IMPLEMENT_DYNCREATE(CAviView, CScrollView)

CAviView::CAviView()
{
	m_nCurrentFrame = 0;
	m_bPlay = FALSE;
	m_nMode = 0;
}

CAviView::~CAviView()
{
}


BEGIN_MESSAGE_MAP(CAviView, CScrollView)
	ON_WM_TIMER()
	ON_COMMAND(ID_VIDEO_PLAY, &CAviView::OnVideoPlay)
	ON_UPDATE_COMMAND_UI(ID_VIDEO_PLAY, &CAviView::OnUpdateVideoPlay)
	ON_COMMAND(ID_VIDEO_STOP, &CAviView::OnVideoStop)
	ON_UPDATE_COMMAND_UI(ID_VIDEO_STOP, &CAviView::OnUpdateVideoStop)
	ON_COMMAND(ID_VIDEO_PAUSE, &CAviView::OnVideoPause)
	ON_UPDATE_COMMAND_UI(ID_VIDEO_PAUSE, &CAviView::OnUpdateVideoPause)
	ON_COMMAND(ID_VIDEO_START, &CAviView::OnVideoStart)
	ON_COMMAND(ID_VIDEO_PREV, &CAviView::OnVideoPrev)
	ON_COMMAND(ID_VIDEO_NEXT, &CAviView::OnVideoNext)
	ON_COMMAND(ID_VIDEO_END, &CAviView::OnVideoEnd)
	ON_COMMAND(ID_VIDEO_CAPTURE, &CAviView::OnVideoCapture)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_MODE_NORMAL, &CAviView::OnModeNormal)
	ON_UPDATE_COMMAND_UI(ID_MODE_NORMAL, &CAviView::OnUpdateModeNormal)
	ON_COMMAND(ID_MODE_DIFFERENCE, &CAviView::OnModeDifference)
	ON_UPDATE_COMMAND_UI(ID_MODE_DIFFERENCE, &CAviView::OnUpdateModeDifference)
	ON_COMMAND(ID_MODE_MOTION, &CAviView::OnModeMotion)
	ON_UPDATE_COMMAND_UI(ID_MODE_MOTION, &CAviView::OnUpdateModeMotion)
END_MESSAGE_MAP()


// CAviView 그리기입니다.

void CAviView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;

	if (m_Avi.IsValid())
	{
		sizeTotal.cx = m_Avi.GetWidth();
		sizeTotal.cy = m_Avi.GetHeight();
	}
	else
	{
		sizeTotal.cx = sizeTotal.cy = 100;
	}

	SetScrollSizes(MM_TEXT, sizeTotal);

	ResizeParentToFit(TRUE);
}

void CAviView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	
	if (m_Avi.IsValid())
	{
		switch (m_nMode)
		{
		case 0: // ID_MODE_NORMAL
			m_Avi.DrawFrame(pDC->m_hDC, m_nCurrentFrame);
			break;
		case 1: // ID_MODE_DIFFERENCE
			if (m_nCurrentFrame > 0)
			{
				IppDib dib1, dib2;
				m_Avi.GetFrame(m_nCurrentFrame - 1, dib1);
				m_Avi.GetFrame(m_nCurrentFrame, dib2);

				IppRgbImage img1, img2;
				IppDibToImage(dib1, img1);
				IppDibToImage(dib2, img2);

				IppByteImage imgGray1, imgGray2;
				imgGray1.Convert(img1);
				imgGray2.Convert(img2);

				IppByteImage imgDiff;
				IppDiff(imgGray1, imgGray2, imgDiff);

				IppDib dibDiff;
				IppImageToDib(imgDiff, dibDiff);
				
				dibDiff.Draw(pDC->m_hDC);
				break;
			}
			else
			{
				m_Avi.DrawFrame(pDC->m_hDC, m_nCurrentFrame);
			}
			break;
		case 2: // ID_MODE_MOTION
			if (m_nCurrentFrame > 0)
			{
				IppDib dib1, dib2;
				m_Avi.GetFrame(m_nCurrentFrame - 1, dib1);
				m_Avi.GetFrame(m_nCurrentFrame, dib2);

				IppRgbImage img1, img2;
				IppDibToImage(dib1, img1);
				IppDibToImage(dib2, img2);

				IppByteImage imgGray1, imgGray2;
				imgGray1.Convert(img1);
				imgGray2.Convert(img2);

				IppFullSearch fs;
				fs.SetImages(&imgGray1, &imgGray2);
				fs.FullSearch();

				IppByteImage imgMotion;
				fs.GetMotionImage(imgMotion);

				IppDib dibMotion;
				IppImageToDib(imgMotion, dibMotion);

				dibMotion.Draw(pDC->m_hDC);
				break;
			}
			else
			{
				m_Avi.DrawFrame(pDC->m_hDC, m_nCurrentFrame);
			}
			break;
		}

		// 메인 프레임 상태바에 프레임 번호 출력
		CMainFrame* pFrame = (CMainFrame *)AfxGetMainWnd();
		CString strText;
		strText.Format(_T("Frame: %d"), m_nCurrentFrame);
		pFrame->m_wndStatusBar.SetPaneText(1, strText);
	}
}


// CAviView 진단입니다.

#ifdef _DEBUG
void CAviView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CAviView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CAviView 메시지 처리기입니다.

BOOL CAviView::AviFileOpen(LPCTSTR lpszPathName)
{
	// AVI 파일을 열지 못한 경우, 메시지 출력하고 그냥 리턴
	if (!m_Avi.Open(lpszPathName))
	{
		AfxMessageBox(_T("Avi 파일을 열 수 없습니다."));
		return FALSE;
	}

	m_nCurrentFrame = 0;

	AfxPrintInfo(_T("[AVI 파일 열기] 파일 경로: %s, 가로 크기: %d픽셀, 세로 크기: %d픽셀, 전체 프레임 수: %d"),
		lpszPathName, m_Avi.GetWidth(), m_Avi.GetHeight(), m_Avi.GetTotalFrame());

	return TRUE;
}


void CAviView::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		m_nCurrentFrame++;

		if (m_nCurrentFrame >= m_Avi.GetTotalFrame())
		{
			m_nCurrentFrame = m_Avi.GetTotalFrame() - 1;
			KillTimer(1);
			m_bPlay = FALSE;
		}

		Invalidate(FALSE);
	}

	CScrollView::OnTimer(nIDEvent);
}


void CAviView::OnVideoPlay()
{
	if (m_Avi.IsValid())
	{
		m_bPlay = TRUE;
		SetTimer(1, 1000 / m_Avi.GetFrameRate(), NULL);
	}
}


void CAviView::OnUpdateVideoPlay(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Avi.IsValid());
	pCmdUI->SetCheck(m_bPlay);
}


void CAviView::OnVideoStop()
{
	m_bPlay = FALSE;
	KillTimer(1);
	m_nCurrentFrame = 0;
	Invalidate(FALSE);
}


void CAviView::OnUpdateVideoStop(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!m_bPlay && m_nCurrentFrame == 0);
}


void CAviView::OnVideoPause()
{
	m_bPlay = FALSE;
	KillTimer(1);
}


void CAviView::OnUpdateVideoPause(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!m_bPlay && m_nCurrentFrame != 0);
}


void CAviView::OnVideoStart()
{
	m_nCurrentFrame = 0;
	Invalidate(FALSE);
}


void CAviView::OnVideoPrev()
{
	if (m_nCurrentFrame > 0)
		m_nCurrentFrame--;
	Invalidate(FALSE);
}


void CAviView::OnVideoNext()
{
	if (m_nCurrentFrame < m_Avi.GetTotalFrame() - 1)
		m_nCurrentFrame++;
	Invalidate(FALSE);
}


void CAviView::OnVideoEnd()
{
	m_nCurrentFrame = m_Avi.GetTotalFrame() - 1;
	Invalidate(FALSE);
}


void CAviView::OnVideoCapture()
{
	IppDib dib;
	m_Avi.GetFrame(m_nCurrentFrame, dib);
	AfxPrintInfo(_T("[프레임 캡쳐] 프레임 번호: %d, 가로 크기: %d픽셀, 세로 크기: %d픽셀, 색상수: %d"),
		m_nCurrentFrame, dib.GetWidth(), dib.GetHeight(), 0x01 << dib.GetBitCount());
	AfxNewBitmap(dib);
}


BOOL CAviView::OnEraseBkgnd(CDC* pDC)
{
	CBrush br;
	br.CreateHatchBrush(HS_DIAGCROSS, RGB(200, 200, 200));
	FillOutsideRect(pDC, &br);

	return TRUE;       // Erased
}


void CAviView::OnModeNormal()
{
	m_nMode = 0;
	Invalidate(FALSE);
}


void CAviView::OnUpdateModeNormal(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nMode == 0);
}


void CAviView::OnModeDifference()
{
	m_nMode = 1;
	Invalidate(FALSE);
}


void CAviView::OnUpdateModeDifference(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nMode == 1);
}


void CAviView::OnModeMotion()
{
	m_nMode = 2;
	Invalidate(FALSE);
}


void CAviView::OnUpdateModeMotion(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nMode == 2);
}

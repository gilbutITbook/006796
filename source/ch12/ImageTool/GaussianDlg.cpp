// GaussianDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "GaussianDlg.h"
#include "afxdialogex.h"


// CGaussianDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGaussianDlg, CDialogEx)

CGaussianDlg::CGaussianDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GAUSSIAN, pParent)
	, m_fSigma(1.40f)
{

}

CGaussianDlg::~CGaussianDlg()
{
}

void CGaussianDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SIGMA_SLIDER, m_sliderSigma);
	DDX_Text(pDX, IDC_SIGMA_EDIT, m_fSigma);
	DDV_MinMaxFloat(pDX, m_fSigma, 0.20f, 5.00f);
}


BEGIN_MESSAGE_MAP(CGaussianDlg, CDialogEx)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_SIGMA_EDIT, &CGaussianDlg::OnEnChangeSigmaEdit)
END_MESSAGE_MAP()


// CGaussianDlg 메시지 처리기입니다.


BOOL CGaussianDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 슬라이더 컨트롤의 초기화
	m_sliderSigma.SetRange(10, 250);
	m_sliderSigma.SetTicFreq(20);
	m_sliderSigma.SetPageSize(20);
	m_sliderSigma.SetPos(static_cast<int>(m_fSigma * 50));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CGaussianDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// 슬라이드바에서 발생한 WM_HSCROLL 메시지인 경우 처리
	if (m_sliderSigma.GetSafeHwnd() == pScrollBar->GetSafeHwnd())
	{
		int pos = m_sliderSigma.GetPos();
		m_fSigma = (pos / 50.f);
		UpdateData(FALSE);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CGaussianDlg::OnEnChangeSigmaEdit()
{
	UpdateData(TRUE);
	m_sliderSigma.SetPos(static_cast<int>(m_fSigma * 50));
}

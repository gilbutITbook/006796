// GammaCorrectionDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "GammaCorrectionDlg.h"
#include "afxdialogex.h"


// CGammaCorrectionDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGammaCorrectionDlg, CDialogEx)

CGammaCorrectionDlg::CGammaCorrectionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GAMMA_CORRECTION, pParent)
	, m_fGamma(2.20f)
{

}

CGammaCorrectionDlg::~CGammaCorrectionDlg()
{
}

void CGammaCorrectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GAMMA_SLIDER, m_sliderGamma);
	DDX_Text(pDX, IDC_GAMMA_EDIT, m_fGamma);
	DDV_MinMaxFloat(pDX, m_fGamma, 0.20f, 5.00f);
}


BEGIN_MESSAGE_MAP(CGammaCorrectionDlg, CDialogEx)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_GAMMA_EDIT, &CGammaCorrectionDlg::OnEnChangeGammaEdit)
END_MESSAGE_MAP()


// CGammaCorrectionDlg 메시지 처리기입니다.


BOOL CGammaCorrectionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 슬라이더 컨트롤 초기화
	m_sliderGamma.SetRange(10, 250);
	m_sliderGamma.SetTicFreq(20);
	m_sliderGamma.SetPageSize(20);
	m_sliderGamma.SetPos(static_cast<int>(m_fGamma * 50));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CGammaCorrectionDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (m_sliderGamma.GetSafeHwnd() == pScrollBar->GetSafeHwnd())
	{
		int pos = m_sliderGamma.GetPos();
		m_fGamma = (pos / 50.f);
		UpdateData(FALSE);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CGammaCorrectionDlg::OnEnChangeGammaEdit()
{
	UpdateData(TRUE);
	m_sliderGamma.SetPos(static_cast<int>(m_fGamma * 50));
}

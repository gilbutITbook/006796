// RotateDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "RotateDlg.h"
#include "afxdialogex.h"


// CRotateDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CRotateDlg, CDialogEx)

CRotateDlg::CRotateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ROTATE, pParent)
	, m_nRotate(0)
	, m_fAngle(0.f)
{

}

CRotateDlg::~CRotateDlg()
{
}

void CRotateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_ROTATE1, m_nRotate);
	DDX_Text(pDX, IDC_ANGLE, m_fAngle);
	DDV_MinMaxFloat(pDX, m_fAngle, 0, 360);
}


BEGIN_MESSAGE_MAP(CRotateDlg, CDialogEx)
ON_EN_SETFOCUS(IDC_ANGLE, &CRotateDlg::OnEnSetfocusAngle)
ON_BN_CLICKED(IDC_ROTATE4, &CRotateDlg::OnBnClickedRotate4)
END_MESSAGE_MAP()


// CRotateDlg 메시지 처리기입니다.


void CRotateDlg::OnEnSetfocusAngle()
{
	m_nRotate = 3;
	UpdateData(FALSE);
}


void CRotateDlg::OnBnClickedRotate4()
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_ANGLE);
	pEdit->SetSel(0, -1);
	pEdit->SetFocus();
}

// FourierDescDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "FourierDescDlg.h"
#include "afxdialogex.h"


// CFourierDescDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CFourierDescDlg, CDialogEx)

CFourierDescDlg::CFourierDescDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FOURIER_DESCRIPTOR, pParent)
	, m_nPercent(100)
{

}

CFourierDescDlg::~CFourierDescDlg()
{
}

void CFourierDescDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PERCENT, m_nPercent);
	DDV_MinMaxInt(pDX, m_nPercent, 0, 100);
}


BEGIN_MESSAGE_MAP(CFourierDescDlg, CDialogEx)
END_MESSAGE_MAP()


// CFourierDescDlg 메시지 처리기입니다.

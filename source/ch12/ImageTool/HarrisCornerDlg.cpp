// HarrisCornerDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "HarrisCornerDlg.h"
#include "afxdialogex.h"


// CHarrisCornerDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CHarrisCornerDlg, CDialogEx)

CHarrisCornerDlg::CHarrisCornerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HARRIS_CORNER, pParent)
	, m_nHarrisTh(20000)
{

}

CHarrisCornerDlg::~CHarrisCornerDlg()
{
}

void CHarrisCornerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_HARRIS_THRESHOLD, m_nHarrisTh);
}


BEGIN_MESSAGE_MAP(CHarrisCornerDlg, CDialogEx)
END_MESSAGE_MAP()


// CHarrisCornerDlg 메시지 처리기입니다.

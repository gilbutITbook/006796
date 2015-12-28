// TranslateDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "TranslateDlg.h"
#include "afxdialogex.h"


// CTranslateDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTranslateDlg, CDialogEx)

CTranslateDlg::CTranslateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TRANSLATE, pParent)
	, m_nNewSX(0)
	, m_nNewSY(0)
{

}

CTranslateDlg::~CTranslateDlg()
{
}

void CTranslateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NEW_SX, m_nNewSX);
	DDX_Text(pDX, IDC_NEW_SY, m_nNewSY);
}


BEGIN_MESSAGE_MAP(CTranslateDlg, CDialogEx)
END_MESSAGE_MAP()


// CTranslateDlg 메시지 처리기입니다.

#pragma once


// CTranslateDlg 대화 상자입니다.

class CTranslateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTranslateDlg)

public:
	CTranslateDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTranslateDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRANSLATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_nNewSX;
	int m_nNewSY;
};

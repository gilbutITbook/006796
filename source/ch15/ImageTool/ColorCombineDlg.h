#pragma once
#include "afxwin.h"


// CColorCombineDlg 대화 상자입니다.

class CColorCombineDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CColorCombineDlg)

public:
	CColorCombineDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CColorCombineDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLOR_COMBINE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strColorSpace;
	CComboBox m_comboImage1;
	CComboBox m_comboImage2;
	CComboBox m_comboImage3;

	void* m_pDoc1;
	void* m_pDoc2;
	void* m_pDoc3;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};

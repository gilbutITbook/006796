#pragma once


// CResizeDlg 대화 상자입니다.

class CResizeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CResizeDlg)

public:
	CResizeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CResizeDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RESIZE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_nOldWidth;
	int m_nOldHeight;
	int m_nNewWidth;
	int m_nNewHeight;
	BOOL m_bAspectRatio;
	int m_nInterpolation;
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeNewWidth();
	afx_msg void OnEnChangeNewHeight();
	afx_msg void OnBnClickedAspectRatio();
};

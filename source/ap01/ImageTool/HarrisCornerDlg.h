#pragma once


// CHarrisCornerDlg 대화 상자입니다.

class CHarrisCornerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHarrisCornerDlg)

public:
	CHarrisCornerDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CHarrisCornerDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HARRIS_CORNER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_nHarrisTh;
};

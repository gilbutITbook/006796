#pragma once


// CFreqFilteringDlg 대화 상자입니다.

class CFreqFilteringDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFreqFilteringDlg)

public:
	CFreqFilteringDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CFreqFilteringDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FREQUENCY_FILTERING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_nFilterType;
	int m_nFilterShape;
	int m_nCutoff;
	CString m_strRange;
};

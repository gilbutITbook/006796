#pragma once


// CHistogramDlg 대화 상자입니다.

class CHistogramDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistogramDlg)

public:
	CHistogramDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CHistogramDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HISTOGRAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	int m_Histogram[256];
	void SetImage(IppDib* pDib);

	afx_msg void OnPaint();
};

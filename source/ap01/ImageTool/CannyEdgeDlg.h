#pragma once


// CCannyEdgeDlg 대화 상자입니다.

class CCannyEdgeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCannyEdgeDlg)

public:
	CCannyEdgeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCannyEdgeDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CANNY_EDGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	float m_fSigma;
	float m_fLowTh;
	float m_fHighTh;
};

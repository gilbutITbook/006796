#pragma once
#include "afxcmn.h"

#include "./IppImage/IppDib.h"

// CBinarizationDlg 대화 상자입니다.

class CBinarizationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBinarizationDlg)

public:
	CBinarizationDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CBinarizationDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BINARIZATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_sliderThreshold;
	int m_nThreshold;

	IppDib m_DibSrc; // 입력 영상의 축소 복사본
	IppDib m_DibRes; // m_nThreshold를 이용하여 m_DibSrc를 이진화한 영상
	void SetImage(IppDib& dib);
	void MakePreviewImage();

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeThresholdEdit();
};

#pragma once
#include "afxcmn.h"


// CBrightnessContrastDlg 대화 상자입니다.

class CBrightnessContrastDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBrightnessContrastDlg)

public:
	CBrightnessContrastDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CBrightnessContrastDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BRIGHTNESS_CONTRAST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	// 밝기 변화량
	CSliderCtrl m_sliderBrightness;
	int m_nBrightness;

	// 명암비 변화량 (%)
	CSliderCtrl m_sliderContrast;
	int m_nContrast;

	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeBrightnessEdit();
	afx_msg void OnEnChangeContrastEdit();
};

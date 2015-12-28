// BinarizationDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "BinarizationDlg.h"
#include "afxdialogex.h"

#include "IppImage\IppImage.h"
#include "IppImage\IppConvert.h"
#include "IppImage\IppGeometry.h"
#include "IppImage\IppSegment.h"


// CBinarizationDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CBinarizationDlg, CDialogEx)

CBinarizationDlg::CBinarizationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_BINARIZATION, pParent)
	, m_nThreshold(0)
{

}

CBinarizationDlg::~CBinarizationDlg()
{
}

void CBinarizationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_THRESHOLD_SLIDER, m_sliderThreshold);
	DDX_Text(pDX, IDC_THRESHOLD_EDIT, m_nThreshold);
	DDV_MinMaxInt(pDX, m_nThreshold, 0, 255);
}


BEGIN_MESSAGE_MAP(CBinarizationDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_THRESHOLD_EDIT, &CBinarizationDlg::OnEnChangeThresholdEdit)
END_MESSAGE_MAP()


// CBinarizationDlg 메시지 처리기입니다.

void CBinarizationDlg::SetImage(IppDib& dib)
{
	m_DibSrc = dib;

	// 반복적 이진화 방법을 이용한 (초기) 임계값 설정
	IppByteImage imgSrc;
	IppDibToImage(m_DibSrc, imgSrc);
	m_nThreshold = IppBinarizationIterative(imgSrc);
}

void CBinarizationDlg::MakePreviewImage()
{
	// 현재 임계값을 이용하여 이진화된 미리보기 영상 생성
	IppByteImage imgSrc, imgDst;
	IppDibToImage(m_DibSrc, imgSrc);
	IppBinarization(imgSrc, imgDst, m_nThreshold);
	IppImageToDib(imgDst, m_DibRes);
}


BOOL CBinarizationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 슬라이더 컨트롤을 초기화한다.
	m_sliderThreshold.SetRange(0, 255);
	m_sliderThreshold.SetTicFreq(32);
	m_sliderThreshold.SetPageSize(32);
	m_sliderThreshold.SetPos(m_nThreshold);

	// 픽쳐 컨트롤의 크기를 구한다.
	CRect rect;
	CWnd* pImageWnd = GetDlgItem(IDC_IMAGE_PREVIEW);
	pImageWnd->GetClientRect(rect);

	// 픽쳐 컨트롤의 크기에 맞게 입력 영상의 복사본의 크기를 조절한다.
	IppByteImage imgSrc, imgDst;
	IppDibToImage(m_DibSrc, imgSrc);
	IppResizeNearest(imgSrc, imgDst, rect.Width(), rect.Height());
	IppImageToDib(imgDst, m_DibSrc);

	// 초기 임계값에 의한 미리보기 이진화 영상 만들기
	MakePreviewImage();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CBinarizationDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	CPaintDC dcPreview(GetDlgItem(IDC_IMAGE_PREVIEW));
	m_DibRes.Draw(dcPreview.m_hDC, 0, 0);
}


void CBinarizationDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// 슬라이드바에서 발생한 WM_HSCROLL 메시지인 경우 처리
	if (m_sliderThreshold.GetSafeHwnd() == pScrollBar->GetSafeHwnd())
	{
		int nPos = m_sliderThreshold.GetPos();
		m_nThreshold = nPos;
		UpdateData(FALSE);

		// 현재 설정된 임계값을 이용하여 미리보기 영상의 이진화를 수행한다.
		MakePreviewImage();
		Invalidate(FALSE);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CBinarizationDlg::OnEnChangeThresholdEdit()
{
	// 에디트 컨트롤에서 글자가 바뀐 경우, 슬라이더 컨트롤의 위치를 조절한다.
	UpdateData(TRUE);
	m_sliderThreshold.SetPos(m_nThreshold);

	// 현재 설정된 임계값을 이용하여 미리보기 영상의 이진화를 수행한다.
	MakePreviewImage();
	Invalidate(FALSE);
}


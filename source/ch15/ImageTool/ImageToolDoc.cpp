
// ImageToolDoc.cpp : CImageToolDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImageTool.h"
#endif

#include "ImageToolDoc.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include <algorithm>
#include <functional>

#include <propkey.h>

#include "IppImage\IppImage.h"
#include "IppImage\IppConvert.h"
#include "IppImage\IppEnhance.h"
#include "IppImage\IppFilter.h"
#include "IppImage\IppGeometry.h"
#include "IppImage\IppFourier.h"
#include "IppImage\IppFeature.h"
#include "IppImage\IppColor.h"
#include "IppImage\IppSegment.h"

#include "FileNewDlg.h"
#include "BrightnessContrastDlg.h"
#include "GammaCorrectionDlg.h"
#include "HistogramDlg.h"
#include "ArithmeticLogicalDlg.h"
#include "GaussianDlg.h"
#include "AddNoiseDlg.h"
#include "DiffusionDlg.h"
#include "TranslateDlg.h"
#include "ResizeDlg.h"
#include "RotateDlg.h"
#include "FreqFilteringDlg.h"
#include "CannyEdgeDlg.h"
#include "HarrisCornerDlg.h"
#include "ColorCombineDlg.h"
#include "BinarizationDlg.h"
#include "FourierDescDlg.h"

#define CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img) \
	IppByteImage img; \
	IppDibToImage(m_Dib, img);

#define CONVERT_DIB_TO_RGBIMAGE(m_Dib, img) \
	IppRgbImage img; \
	IppDibToImage(m_Dib, img);

#define CONVERT_IMAGE_TO_DIB(img, dib) \
	IppDib dib; \
	IppImageToDib(img, dib);

#define SHOW_SPECTRUM_PHASE_IMAGE

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CImageToolDoc

IMPLEMENT_DYNCREATE(CImageToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageToolDoc, CDocument)
	ON_COMMAND(ID_WINDOW_DUPLICATE, &CImageToolDoc::OnWindowDuplicate)
	ON_COMMAND(ID_EDIT_COPY, &CImageToolDoc::OnEditCopy)
	ON_COMMAND(ID_IMAGE_INVERSE, &CImageToolDoc::OnImageInverse)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_INVERSE, &CImageToolDoc::OnUpdateImageInverse)
	ON_COMMAND(ID_BRIGHTNESS_CONTRAST, &CImageToolDoc::OnBrightnessContrast)
	ON_UPDATE_COMMAND_UI(ID_BRIGHTNESS_CONTRAST, &CImageToolDoc::OnUpdateBrightnessContrast)
	ON_COMMAND(ID_GAMMA_CORRECTION, &CImageToolDoc::OnGammaCorrection)
	ON_UPDATE_COMMAND_UI(ID_GAMMA_CORRECTION, &CImageToolDoc::OnUpdateGammaCorrection)
	ON_COMMAND(ID_VIEW_HISTOGRAM, &CImageToolDoc::OnViewHistogram)
	ON_UPDATE_COMMAND_UI(ID_VIEW_HISTOGRAM, &CImageToolDoc::OnUpdateViewHistogram)
	ON_COMMAND(ID_HISTO_STRETCHING, &CImageToolDoc::OnHistoStretching)
	ON_UPDATE_COMMAND_UI(ID_HISTO_STRETCHING, &CImageToolDoc::OnUpdateHistoStretching)
	ON_COMMAND(ID_HISTO_EQUALIZATION, &CImageToolDoc::OnHistoEqualization)
	ON_UPDATE_COMMAND_UI(ID_HISTO_EQUALIZATION, &CImageToolDoc::OnUpdateHistoEqualization)
	ON_COMMAND(ID_ARITHMETIC_LOGICAL, &CImageToolDoc::OnArithmeticLogical)
	ON_COMMAND(ID_BITPLANE_SLICING, &CImageToolDoc::OnBitplaneSlicing)
	ON_UPDATE_COMMAND_UI(ID_BITPLANE_SLICING, &CImageToolDoc::OnUpdateBitplaneSlicing)
	ON_COMMAND(ID_FILTER_MEAN, &CImageToolDoc::OnFilterMean)
	ON_UPDATE_COMMAND_UI(ID_FILTER_MEAN, &CImageToolDoc::OnUpdateFilterMean)
	ON_COMMAND(ID_FILTER_WEIGHTED_MEAN, &CImageToolDoc::OnFilterWeightedMean)
	ON_UPDATE_COMMAND_UI(ID_FILTER_WEIGHTED_MEAN, &CImageToolDoc::OnUpdateFilterWeightedMean)
	ON_COMMAND(ID_FILTER_GAUSSIAN, &CImageToolDoc::OnFilterGaussian)
	ON_UPDATE_COMMAND_UI(ID_FILTER_GAUSSIAN, &CImageToolDoc::OnUpdateFilterGaussian)
	ON_COMMAND(ID_FILTER_LAPLACIAN, &CImageToolDoc::OnFilterLaplacian)
	ON_UPDATE_COMMAND_UI(ID_FILTER_LAPLACIAN, &CImageToolDoc::OnUpdateFilterLaplacian)
	ON_COMMAND(ID_FILTER_UNSHARP_MASK, &CImageToolDoc::OnFilterUnsharpMask)
	ON_UPDATE_COMMAND_UI(ID_FILTER_UNSHARP_MASK, &CImageToolDoc::OnUpdateFilterUnsharpMask)
	ON_COMMAND(ID_FILTER_HIGHBOOST, &CImageToolDoc::OnFilterHighboost)
	ON_UPDATE_COMMAND_UI(ID_FILTER_HIGHBOOST, &CImageToolDoc::OnUpdateFilterHighboost)
	ON_COMMAND(ID_ADD_NOISE, &CImageToolDoc::OnAddNoise)
	ON_UPDATE_COMMAND_UI(ID_ADD_NOISE, &CImageToolDoc::OnUpdateAddNoise)
	ON_COMMAND(ID_FILTER_MEDIAN, &CImageToolDoc::OnFilterMedian)
	ON_UPDATE_COMMAND_UI(ID_FILTER_MEDIAN, &CImageToolDoc::OnUpdateFilterMedian)
	ON_COMMAND(ID_FILTER_DIFFUSION, &CImageToolDoc::OnFilterDiffusion)
	ON_UPDATE_COMMAND_UI(ID_FILTER_DIFFUSION, &CImageToolDoc::OnUpdateFilterDiffusion)
	ON_COMMAND(ID_IMAGE_TRANSLATION, &CImageToolDoc::OnImageTranslation)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_TRANSLATION, &CImageToolDoc::OnUpdateImageTranslation)
	ON_COMMAND(ID_IMAGE_RESIZE, &CImageToolDoc::OnImageResize)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_RESIZE, &CImageToolDoc::OnUpdateImageResize)
	ON_COMMAND(ID_IMAGE_ROTATE, &CImageToolDoc::OnImageRotate)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_ROTATE, &CImageToolDoc::OnUpdateImageRotate)
	ON_COMMAND(ID_IMAGE_MIRROR, &CImageToolDoc::OnImageMirror)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_MIRROR, &CImageToolDoc::OnUpdateImageMirror)
	ON_COMMAND(ID_IMAGE_FLIP, &CImageToolDoc::OnImageFlip)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_FLIP, &CImageToolDoc::OnUpdateImageFlip)
	ON_COMMAND(ID_FOURIER_DFT, &CImageToolDoc::OnFourierDft)
	ON_UPDATE_COMMAND_UI(ID_FOURIER_DFT, &CImageToolDoc::OnUpdateFourierDft)
	ON_COMMAND(ID_FOURIER_DFTRC, &CImageToolDoc::OnFourierDftrc)
	ON_UPDATE_COMMAND_UI(ID_FOURIER_DFTRC, &CImageToolDoc::OnUpdateFourierDftrc)
	ON_COMMAND(ID_FOURIER_FFT, &CImageToolDoc::OnFourierFft)
	ON_UPDATE_COMMAND_UI(ID_FOURIER_FFT, &CImageToolDoc::OnUpdateFourierFft)
	ON_COMMAND(ID_FREQ_FILTERING, &CImageToolDoc::OnFreqFiltering)
	ON_UPDATE_COMMAND_UI(ID_FREQ_FILTERING, &CImageToolDoc::OnUpdateFreqFiltering)
	ON_COMMAND(ID_EDGE_ROBERTS, &CImageToolDoc::OnEdgeRoberts)
	ON_UPDATE_COMMAND_UI(ID_EDGE_ROBERTS, &CImageToolDoc::OnUpdateEdgeRoberts)
	ON_COMMAND(ID_EDGE_PREWITT, &CImageToolDoc::OnEdgePrewitt)
	ON_UPDATE_COMMAND_UI(ID_EDGE_PREWITT, &CImageToolDoc::OnUpdateEdgePrewitt)
	ON_COMMAND(ID_EDGE_SOBEL, &CImageToolDoc::OnEdgeSobel)
	ON_UPDATE_COMMAND_UI(ID_EDGE_SOBEL, &CImageToolDoc::OnUpdateEdgeSobel)
	ON_COMMAND(ID_EDGE_CANNY, &CImageToolDoc::OnEdgeCanny)
	ON_UPDATE_COMMAND_UI(ID_EDGE_CANNY, &CImageToolDoc::OnUpdateEdgeCanny)
	ON_COMMAND(ID_HOUGH_LINE, &CImageToolDoc::OnHoughLine)
	ON_UPDATE_COMMAND_UI(ID_HOUGH_LINE, &CImageToolDoc::OnUpdateHoughLine)
	ON_COMMAND(ID_HARRIS_CORNER, &CImageToolDoc::OnHarrisCorner)
	ON_UPDATE_COMMAND_UI(ID_HARRIS_CORNER, &CImageToolDoc::OnUpdateHarrisCorner)
	ON_COMMAND(ID_COLOR_GRAYSCALE, &CImageToolDoc::OnColorGrayscale)
	ON_UPDATE_COMMAND_UI(ID_COLOR_GRAYSCALE, &CImageToolDoc::OnUpdateColorGrayscale)
	ON_COMMAND(ID_COLOR_SPLIT_RGB, &CImageToolDoc::OnColorSplitRgb)
	ON_UPDATE_COMMAND_UI(ID_COLOR_SPLIT_RGB, &CImageToolDoc::OnUpdateColorSplitRgb)
	ON_COMMAND(ID_COLOR_SPLIT_HSI, &CImageToolDoc::OnColorSplitHsi)
	ON_UPDATE_COMMAND_UI(ID_COLOR_SPLIT_HSI, &CImageToolDoc::OnUpdateColorSplitHsi)
	ON_COMMAND(ID_COLOR_SPLIT_YUV, &CImageToolDoc::OnColorSplitYuv)
	ON_UPDATE_COMMAND_UI(ID_COLOR_SPLIT_YUV, &CImageToolDoc::OnUpdateColorSplitYuv)
	ON_COMMAND(ID_COLOR_COMBINE_RGB, &CImageToolDoc::OnColorCombineRgb)
	ON_COMMAND(ID_COLOR_COMBINE_HSI, &CImageToolDoc::OnColorCombineHsi)
	ON_COMMAND(ID_COLOR_COMBINE_YUV, &CImageToolDoc::OnColorCombineYuv)
	ON_COMMAND(ID_COLOR_EDGE, &CImageToolDoc::OnColorEdge)
	ON_UPDATE_COMMAND_UI(ID_COLOR_EDGE, &CImageToolDoc::OnUpdateColorEdge)
	ON_COMMAND(ID_SEGMENT_BINARIZATION, &CImageToolDoc::OnSegmentBinarization)
	ON_UPDATE_COMMAND_UI(ID_SEGMENT_BINARIZATION, &CImageToolDoc::OnUpdateSegmentBinarization)
	ON_COMMAND(ID_SEGMENT_LABELING, &CImageToolDoc::OnSegmentLabeling)
	ON_UPDATE_COMMAND_UI(ID_SEGMENT_LABELING, &CImageToolDoc::OnUpdateSegmentLabeling)
	ON_COMMAND(ID_CONTOUR_TRACING, &CImageToolDoc::OnContourTracing)
	ON_UPDATE_COMMAND_UI(ID_CONTOUR_TRACING, &CImageToolDoc::OnUpdateContourTracing)
	ON_COMMAND(ID_MORPHOLOGY_EROSION, &CImageToolDoc::OnMorphologyErosion)
	ON_UPDATE_COMMAND_UI(ID_MORPHOLOGY_EROSION, &CImageToolDoc::OnUpdateMorphologyErosion)
	ON_COMMAND(ID_MORPHOLOGY_DILATION, &CImageToolDoc::OnMorphologyDilation)
	ON_UPDATE_COMMAND_UI(ID_MORPHOLOGY_DILATION, &CImageToolDoc::OnUpdateMorphologyDilation)
	ON_COMMAND(ID_MORPHOLOGY_OPENING, &CImageToolDoc::OnMorphologyOpening)
	ON_UPDATE_COMMAND_UI(ID_MORPHOLOGY_OPENING, &CImageToolDoc::OnUpdateMorphologyOpening)
	ON_COMMAND(ID_MORPHOLOGY_CLOSING, &CImageToolDoc::OnMorphologyClosing)
	ON_UPDATE_COMMAND_UI(ID_MORPHOLOGY_CLOSING, &CImageToolDoc::OnUpdateMorphologyClosing)
	ON_COMMAND(ID_GRAYMORPH_EROSION, &CImageToolDoc::OnGraymorphErosion)
	ON_UPDATE_COMMAND_UI(ID_GRAYMORPH_EROSION, &CImageToolDoc::OnUpdateGraymorphErosion)
	ON_COMMAND(ID_GRAYMORPH_DILATION, &CImageToolDoc::OnGraymorphDilation)
	ON_UPDATE_COMMAND_UI(ID_GRAYMORPH_DILATION, &CImageToolDoc::OnUpdateGraymorphDilation)
	ON_COMMAND(ID_GRAYMORPH_OPENING, &CImageToolDoc::OnGraymorphOpening)
	ON_UPDATE_COMMAND_UI(ID_GRAYMORPH_OPENING, &CImageToolDoc::OnUpdateGraymorphOpening)
	ON_COMMAND(ID_GRAYMORPH_CLOSING, &CImageToolDoc::OnGraymorphClosing)
	ON_UPDATE_COMMAND_UI(ID_GRAYMORPH_CLOSING, &CImageToolDoc::OnUpdateGraymorphClosing)
	ON_COMMAND(ID_FOURIER_DESCRIPTOR, &CImageToolDoc::OnFourierDescriptor)
	ON_UPDATE_COMMAND_UI(ID_FOURIER_DESCRIPTOR, &CImageToolDoc::OnUpdateFourierDescriptor)
	ON_COMMAND(ID_INVARIANT_MOMENTS, &CImageToolDoc::OnInvariantMoments)
	ON_UPDATE_COMMAND_UI(ID_INVARIANT_MOMENTS, &CImageToolDoc::OnUpdateInvariantMoments)
	ON_COMMAND(ID_ZERNIKE_MOMENTS, &CImageToolDoc::OnZernikeMoments)
	ON_UPDATE_COMMAND_UI(ID_ZERNIKE_MOMENTS, &CImageToolDoc::OnUpdateZernikeMoments)
	ON_COMMAND(ID_TEMPLATE_MATCHING, &CImageToolDoc::OnTemplateMatching)
	ON_UPDATE_COMMAND_UI(ID_TEMPLATE_MATCHING, &CImageToolDoc::OnUpdateTemplateMatching)
END_MESSAGE_MAP()


// CImageToolDoc 생성/소멸

CImageToolDoc::CImageToolDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CImageToolDoc::~CImageToolDoc()
{
}

BOOL CImageToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	BOOL ret = TRUE;

	if (theApp.m_pNewDib == NULL)
	{
		CFileNewDlg dlg;
		if (dlg.DoModal() == IDOK)
		{
			if (dlg.m_nType == 0) // 그레이스케일 비트맵
				ret = m_Dib.CreateGrayBitmap(dlg.m_nWidth, dlg.m_nHeight);
			else // 트루컬러 비트맵
				ret = m_Dib.CreateRgbBitmap(dlg.m_nWidth, dlg.m_nHeight);
		}
		else
		{
			ret = FALSE;
		}
	}
	else
	{
		m_Dib = *(theApp.m_pNewDib);
		theApp.m_pNewDib = NULL;
	}

	return ret;
}




// CImageToolDoc serialization

void CImageToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CImageToolDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CImageToolDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CImageToolDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CImageToolDoc 진단

#ifdef _DEBUG
void CImageToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImageToolDoc 명령


BOOL CImageToolDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	BOOL res = m_Dib.Load(CT2A(lpszPathName));
	if (res)
		AfxPrintInfo(_T("[파일 열기] 파일 경로: %s, 가로 크기: %d픽셀, 세로 크기: %d픽셀, 색상수: %d"),
			lpszPathName, m_Dib.GetWidth(), m_Dib.GetHeight(), 0x01 << m_Dib.GetBitCount());

	return res;
}


BOOL CImageToolDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	return m_Dib.Save(CT2A(lpszPathName));
}


void CImageToolDoc::OnWindowDuplicate()
{
	AfxNewBitmap(m_Dib);
}


void CImageToolDoc::OnEditCopy()
{
	if (m_Dib.IsValid())
		m_Dib.CopyToClipboard();
}


void CImageToolDoc::OnImageInverse()
{
	if (m_Dib.GetBitCount() == 8)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		IppInverse(img);
		CONVERT_IMAGE_TO_DIB(img, dib)

		AfxPrintInfo(_T("[반전] 입력 영상: %s"), GetTitle());
		AfxNewBitmap(dib);
	}
	else if (m_Dib.GetBitCount() == 24)
	{
		CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
		IppInverse(img);
		CONVERT_IMAGE_TO_DIB(img, dib)

		AfxPrintInfo(_T("[반전] 입력 영상: %s"), GetTitle());
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnUpdateImageInverse(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8 || m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnBrightnessContrast()
{
	CBrightnessContrastDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		if (m_Dib.GetBitCount() == 8)
		{
			CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
			IppBrightness(img, dlg.m_nBrightness);
			IppContrast(img, dlg.m_nContrast);
			CONVERT_IMAGE_TO_DIB(img, dib)

			AfxPrintInfo(_T("[밝기/명암비 조절] 입력 영상: %s, 밝기: %d, 명암비: %d%%"),
				GetTitle(), dlg.m_nBrightness, dlg.m_nContrast);
			AfxNewBitmap(dib);
		}
		else if (m_Dib.GetBitCount() == 24)
		{
			CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
			IppByteImage imgY, imgU, imgV;
			IppColorSplitYUV(img, imgY, imgU, imgV);
			IppBrightness(imgY, dlg.m_nBrightness);
			IppContrast(imgY, dlg.m_nContrast);

			IppRgbImage imgRes;
			IppColorCombineYUV(imgY, imgU, imgV, imgRes);
			CONVERT_IMAGE_TO_DIB(imgRes, dib)

			AfxPrintInfo(_T("[밝기/명암비 조절] 입력 영상: %s, 밝기: %d, 명암비: %d%%"),
				GetTitle(), dlg.m_nBrightness, dlg.m_nContrast);
			AfxNewBitmap(dib);
		}
	}
}


void CImageToolDoc::OnUpdateBrightnessContrast(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8 || m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnGammaCorrection()
{
	CGammaCorrectionDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		if (m_Dib.GetBitCount() == 8)
		{
			CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
			IppGammaCorrection(img, dlg.m_fGamma);
			CONVERT_IMAGE_TO_DIB(img, dib)

			AfxPrintInfo(_T("[감마 보정] 입력 영상: %s, 감마: %4.2f"), GetTitle(), dlg.m_fGamma);
			AfxNewBitmap(dib);
		}
		else if (m_Dib.GetBitCount() == 24)
		{
			CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
			IppByteImage imgY, imgU, imgV;
			IppColorSplitYUV(img, imgY, imgU, imgV);
			IppGammaCorrection(imgY, dlg.m_fGamma);

			IppRgbImage imgRes;
			IppColorCombineYUV(imgY, imgU, imgV, imgRes);
			CONVERT_IMAGE_TO_DIB(imgRes, dib)

			AfxPrintInfo(_T("[감마 보정] 입력 영상: %s, 감마: %4.2f"), GetTitle(), dlg.m_fGamma);
			AfxNewBitmap(dib);

		}
	}
}


void CImageToolDoc::OnUpdateGammaCorrection(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8 || m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnViewHistogram()
{
	CHistogramDlg dlg;
	dlg.SetImage(&m_Dib);
	dlg.DoModal();
}


void CImageToolDoc::OnUpdateViewHistogram(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnHistoStretching()
{
	if (m_Dib.GetBitCount() == 8)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		IppHistogramStretching(img);
		CONVERT_IMAGE_TO_DIB(img, dib)

		AfxPrintInfo(_T("[히스토그램 스트레칭] 입력 영상: %s"), GetTitle());
		AfxNewBitmap(dib);
	}
	else if (m_Dib.GetBitCount() == 24)
	{
		CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
		IppByteImage imgY, imgU, imgV;
		IppColorSplitYUV(img, imgY, imgU, imgV);
		IppHistogramStretching(imgY);

		IppRgbImage imgRes;
		IppColorCombineYUV(imgY, imgU, imgV, imgRes);
		CONVERT_IMAGE_TO_DIB(imgRes, dib)

		AfxPrintInfo(_T("[히스토그램 스트레칭] 입력 영상: %s"), GetTitle());
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnUpdateHistoStretching(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8 || m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnHistoEqualization()
{
	if (m_Dib.GetBitCount() == 8)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		IppHistogramEqualization(img);
		CONVERT_IMAGE_TO_DIB(img, dib)

		AfxPrintInfo(_T("[히스토그램 균등화] 입력 영상: %s"), GetTitle());
		AfxNewBitmap(dib);
	}
	else if (m_Dib.GetBitCount() == 24)
	{
		CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
		IppByteImage imgY, imgU, imgV;
		IppColorSplitYUV(img, imgY, imgU, imgV);
		IppHistogramEqualization(imgY);

		IppRgbImage imgRes;
		IppColorCombineYUV(imgY, imgU, imgV, imgRes);
		CONVERT_IMAGE_TO_DIB(imgRes, dib)

		AfxPrintInfo(_T("[히스토그램 균등화] 입력 영상: %s"), GetTitle());
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnUpdateHistoEqualization(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8 || m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnArithmeticLogical()
{
	CArithmeticLogicalDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CImageToolDoc* pDoc1 = (CImageToolDoc*)dlg.m_pDoc1;
		CImageToolDoc* pDoc2 = (CImageToolDoc*)dlg.m_pDoc2;

		CONVERT_DIB_TO_BYTEIMAGE(pDoc1->m_Dib, img1)
		CONVERT_DIB_TO_BYTEIMAGE(pDoc2->m_Dib, img2)
		IppByteImage img3;

		bool ret = false;

		switch (dlg.m_nFunction)
		{
		case 0: ret = IppAdd(img1, img2, img3);  break;
		case 1: ret = IppSub(img1, img2, img3);  break;
		case 2: ret = IppAve(img1, img2, img3);  break;
		case 3: ret = IppDiff(img1, img2, img3); break;
		case 4: ret = IppAND(img1, img2, img3);  break;
		case 5: ret = IppOR(img1, img2, img3);   break;
		}

		if (ret)
		{
			CONVERT_IMAGE_TO_DIB(img3, dib)

			TCHAR* op[] = { _T("덧셈"), _T("뺄셈"), _T("평균"), _T("차이"), _T("논리 AND"), _T("논리 OR") };
			AfxPrintInfo(_T("[산술 및 논리 연산] [%s] 입력 영상 #1: %s, 입력 영상 #2: %s"), 
				op[dlg.m_nFunction], pDoc1->GetTitle(), pDoc2->GetTitle());
			AfxNewBitmap(dib);
		}
		else
			AfxMessageBox(_T("영상의 크기가 다릅니다!"));
	}
}


void CImageToolDoc::OnBitplaneSlicing()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgPlane;

	for (int i = 0; i < 8; i++)
	{
		IppBitPlane(img, imgPlane, i);
		CONVERT_IMAGE_TO_DIB(imgPlane, dib)
		AfxNewBitmap(dib);
	}

	AfxPrintInfo(_T("[비트 평면 분할] 입력 영상: %s"), GetTitle());
}


void CImageToolDoc::OnUpdateBitplaneSlicing(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnFilterMean()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
	IppByteImage imgDst;
	IppFilterMean(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[평균 값 필터] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateFilterMean(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnFilterWeightedMean()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
	IppByteImage imgDst;
	IppFilterWeightedMean(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[가중 평균 값 필터] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateFilterWeightedMean(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnFilterGaussian()
{
	CGaussianDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		IppFloatImage imgDst;
		IppFilterGaussian(imgSrc, imgDst, dlg.m_fSigma);
		CONVERT_IMAGE_TO_DIB(imgDst, dib)

		AfxPrintInfo(_T("[가우시안 필터] 입력 영상: %s, Sigma: %4.2f"), GetTitle(), dlg.m_fSigma);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnUpdateFilterGaussian(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnFilterLaplacian()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
	IppByteImage imgDst;
	IppFilterLaplacian(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[라플라시안 필터] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateFilterLaplacian(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnFilterUnsharpMask()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
	IppByteImage imgDst;
	IppFilterUnsharpMask(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[언샤프 마스크 필터] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateFilterUnsharpMask(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnFilterHighboost()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
	IppByteImage imgDst;
	float alpha = 1.2f;
	IppFilterHighboost(imgSrc, imgDst, alpha);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[하이부스트 필터] 입력 영상: %s, alpha = %4.2f"), GetTitle(), alpha);
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateFilterHighboost(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnAddNoise()
{
	CAddNoiseDlg dlg;
	if( dlg.DoModal() == IDOK )
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		IppByteImage imgDst;

		if( dlg.m_nNoiseType == 0 )
			IppNoiseGaussian(imgSrc, imgDst, dlg.m_nAmount);
		else
			IppNoiseSaltNPepper(imgSrc, imgDst, dlg.m_nAmount);

		CONVERT_IMAGE_TO_DIB(imgDst, dib)

		TCHAR* noise[] = { _T("가우시안"), _T("소금&후추") };
		AfxPrintInfo(_T("[잡음 추가] 입력 영상: %s, 잡음 종류: %s, 잡음 양: %d"), 
			GetTitle(), noise[dlg.m_nNoiseType], dlg.m_nAmount);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnUpdateAddNoise(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnFilterMedian()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
	IppByteImage imgDst;
	IppFilterMedian(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[미디언 필터] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateFilterMedian(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnFilterDiffusion()
{
	CDiffusionDlg dlg;
	if( dlg.DoModal() == IDOK )
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		IppFloatImage imgDst;
		IppFilterDiffusion(imgSrc, imgDst, dlg.m_fLambda, dlg.m_fK, dlg.m_nIteration);
		CONVERT_IMAGE_TO_DIB(imgDst, dib)

		AfxPrintInfo(_T("[비등방성 확산 필터] 입력 영상: %s, Lambda: %4.2f, K: %4.2f, 반복 횟수: %d"), 
			GetTitle(), dlg.m_fLambda, dlg.m_fK, dlg.m_nIteration);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnUpdateFilterDiffusion(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnImageTranslation()
{
	CTranslateDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		IppByteImage imgDst;
		IppTranslate(imgSrc, imgDst, dlg.m_nNewSX, dlg.m_nNewSY);
		CONVERT_IMAGE_TO_DIB(imgDst, dib)

		AfxPrintInfo(_T("[이동 변환] 입력 영상: %s, 가로 이동: %d, 세로 이동: %d"),
			GetTitle(), dlg.m_nNewSX, dlg.m_nNewSY);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnUpdateImageTranslation(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnImageResize()
{
	CResizeDlg dlg;
	dlg.m_nOldWidth = m_Dib.GetWidth();
	dlg.m_nOldHeight = m_Dib.GetHeight();
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		IppByteImage imgDst;
		switch (dlg.m_nInterpolation)
		{
		case 0: IppResizeNearest(imgSrc, imgDst, dlg.m_nNewWidth, dlg.m_nNewHeight); break;
		case 1: IppResizeBilinear(imgSrc, imgDst, dlg.m_nNewWidth, dlg.m_nNewHeight); break;
		case 2: IppResizeCubic(imgSrc, imgDst, dlg.m_nNewWidth, dlg.m_nNewHeight); break;
		}

		CONVERT_IMAGE_TO_DIB(imgDst, dib)
		
		TCHAR* interpolation[] = { _T("최근방 이웃 보간법"), _T("양선형 보간법"), _T("3차 회선 보간법") };
		AfxPrintInfo(_T("[크기 변환] 입력 영상: %s, , 새 가로 크기: %d, 새 세로 크기: %d, 보간법: %s"),
			GetTitle(), dlg.m_nNewWidth, dlg.m_nNewHeight, interpolation[dlg.m_nInterpolation]);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnUpdateImageResize(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnImageRotate()
{
	CRotateDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		IppByteImage imgDst;
		switch (dlg.m_nRotate)
		{
		case 0: IppRotate90(imgSrc, imgDst); break;
		case 1: IppRotate180(imgSrc, imgDst); break;
		case 2: IppRotate270(imgSrc, imgDst); break;
		case 3: IppRotate(imgSrc, imgDst, (double)dlg.m_fAngle); break;
		}

		CONVERT_IMAGE_TO_DIB(imgDst, dib)

		TCHAR* rotate[] = { _T("90도"), _T("180도"), _T("270도") };
		if (dlg.m_nRotate != 3)
			AfxPrintInfo(_T("[회전 변환] 입력 영상: %s, 회전 각도: %s"), GetTitle(), rotate[dlg.m_nRotate]);
		else
			AfxPrintInfo(_T("[회전 변환] 입력 영상: %s, 회전 각도: %4.2f도"), GetTitle(), dlg.m_fAngle);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnUpdateImageRotate(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnImageMirror()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
	IppByteImage imgDst;
	IppMirror(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[좌우 대칭] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateImageMirror(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnImageFlip()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
	IppByteImage imgDst;
	IppFlip(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[상하 대칭] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateImageFlip(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnFourierDft()
{
	int w = m_Dib.GetWidth();
	int h = m_Dib.GetHeight();

	if (w * h > 128 * 128)
	{
		CString msg = _T("영상의 크기가 커서 시간이 오래 걸릴 수 있습니다.\n계속 하시겠습니까?");
		if (AfxMessageBox(msg, MB_OKCANCEL) != IDOK)
			return;
	}

	CWaitCursor wait;

	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)

	IppFourier fourier;
	fourier.SetImage(img);

	DWORD t1 = timeGetTime();
	fourier.DFT(1);

#ifdef SHOW_SPECTRUM_PHASE_IMAGE
	IppByteImage imgSpec;
	fourier.GetSpectrumImage(imgSpec);

	CONVERT_IMAGE_TO_DIB(imgSpec, dibSpec)
	AfxNewBitmap(dibSpec);

	IppByteImage imgPhase;
	fourier.GetPhaseImage(imgPhase);

	CONVERT_IMAGE_TO_DIB(imgPhase, dibPhase)
	AfxNewBitmap(dibPhase);
#endif

	fourier.DFT(-1);
	DWORD t2 = timeGetTime();

	IppByteImage img2;
	fourier.GetImage(img2);

	CONVERT_IMAGE_TO_DIB(img2, dib)

	AfxPrintInfo(_T("[푸리에변환/DFT] 입력 영상: %s, 입력 영상 크기: %dx%d, 처리 시간: %dmsec"),
		GetTitle(), w, h, t2 - t1);
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateFourierDft(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnFourierDftrc()
{
	int w = m_Dib.GetWidth();
	int h = m_Dib.GetHeight();

	if (w * h > 256 * 256)
	{
		CString msg = _T("영상의 크기가 커서 시간이 오래 걸릴 수 있습니다.\n계속 하시겠습니까?");
		if (AfxMessageBox(msg, MB_OKCANCEL) != IDOK)
			return;
	}

	CWaitCursor wait;

	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)

	IppFourier fourier;
	fourier.SetImage(img);

	DWORD t1 = timeGetTime();
	fourier.DFTRC(1);

#ifdef SHOW_SPECTRUM_PHASE_IMAGE
	IppByteImage imgSpec;
	fourier.GetSpectrumImage(imgSpec);

	CONVERT_IMAGE_TO_DIB(imgSpec, dibSpec)
	AfxNewBitmap(dibSpec);

	IppByteImage imgPhase;
	fourier.GetPhaseImage(imgPhase);

	CONVERT_IMAGE_TO_DIB(imgPhase, dibPhase)
	AfxNewBitmap(dibPhase);
#endif

	fourier.DFTRC(-1);
	DWORD t2 = timeGetTime();

	IppByteImage img2;
	fourier.GetImage(img2);

	CONVERT_IMAGE_TO_DIB(img2, dib)

	AfxPrintInfo(_T("[푸리에변환/DFTRC] 입력 영상: %s, 입력 영상 크기: %dx%d, 처리 시간: %dmsec"),
		GetTitle(), w, h, t2 - t1);
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateFourierDftrc(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnFourierFft()
{
	int w = m_Dib.GetWidth();
	int h = m_Dib.GetHeight();

	if (!IsPowerOf2(w) || !IsPowerOf2(h))
	{
		AfxMessageBox(_T("가로 또는 세로의 크기가 2의 승수가 아닙니다."));
		return;
	}

	CWaitCursor wait;

	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)

	IppFourier fourier;
	fourier.SetImage(img);

	DWORD t1 = timeGetTime();
	fourier.FFT(1);

#ifdef SHOW_SPECTRUM_PHASE_IMAGE
	IppByteImage imgSpec;
	fourier.GetSpectrumImage(imgSpec);

	CONVERT_IMAGE_TO_DIB(imgSpec, dibSpec)
	AfxNewBitmap(dibSpec);

	IppByteImage imgPhase;
	fourier.GetPhaseImage(imgPhase);

	CONVERT_IMAGE_TO_DIB(imgPhase, dibPhase)
	AfxNewBitmap(dibPhase);
#endif

	fourier.FFT(-1);
	DWORD t2 = timeGetTime();

	IppByteImage img2;
	fourier.GetImage(img2);

	CONVERT_IMAGE_TO_DIB(img2, dib)

	AfxPrintInfo(_T("[푸리에변환/FFT] 입력 영상: %s, 입력 영상 크기: %dx%d, 처리 시간: %dmsec"),
		GetTitle(), w, h, t2 - t1);
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateFourierFft(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnFreqFiltering()
{
	int w = m_Dib.GetWidth();
	int h = m_Dib.GetHeight();

	if (!IsPowerOf2(w) || !IsPowerOf2(h))
	{
		AfxMessageBox(_T("가로 또는 세로의 크기가 2의 승수가 아닙니다."));
		return;
	}

	CFreqFilteringDlg dlg;
	dlg.m_strRange.Format(_T("(0 ~ %d)"), min(w / 2, h / 2));
	if (dlg.DoModal() == IDOK)
	{
		CWaitCursor wait;
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)

		IppFourier fourier;
		fourier.SetImage(img);
		fourier.FFT(1);

		if (dlg.m_nFilterType == 0)
		{
			if (dlg.m_nFilterShape == 0)
				fourier.LowPassIdeal(dlg.m_nCutoff);
			else
				fourier.LowPassGaussian(dlg.m_nCutoff);
		}
		else
		{
			if (dlg.m_nFilterShape == 0)
				fourier.HighPassIdeal(dlg.m_nCutoff);
			else
				fourier.HighPassGaussian(dlg.m_nCutoff);
		}

		fourier.FFT(-1);

		IppByteImage img2;
		fourier.GetImage(img2);
		CONVERT_IMAGE_TO_DIB(img2, dib)

		TCHAR* type[] = { _T("저역 통과 필터"), _T("고역 통과 필터") };
		TCHAR* shape[] = { _T("이상적(Ideal)"), _T("가우시안(Gaussian)") };
		AfxPrintInfo(_T("[주파수 공간 필터링] 입력 영상: %s, 필터 종류: %s, 필터 모양: %s, 차단 주파수: %d"),
			GetTitle(), type[dlg.m_nFilterType], shape[dlg.m_nFilterShape], dlg.m_nCutoff);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnUpdateFreqFiltering(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnEdgeRoberts()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgEdge;
	IppEdgeRoberts(img, imgEdge);
	CONVERT_IMAGE_TO_DIB(imgEdge, dib)

	AfxPrintInfo(_T("[마스크 기반 엣지 검출/로버츠] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateEdgeRoberts(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnEdgePrewitt()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgEdge;
	IppEdgePrewitt(img, imgEdge);
	CONVERT_IMAGE_TO_DIB(imgEdge, dib)

	AfxPrintInfo(_T("[마스크 기반 엣지 검출/프리윗] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateEdgePrewitt(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnEdgeSobel()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgEdge;
	IppEdgeSobel(img, imgEdge);
	CONVERT_IMAGE_TO_DIB(imgEdge, dib)

	AfxPrintInfo(_T("[마스크 기반 엣지 검출/소벨] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateEdgeSobel(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnEdgeCanny()
{
	CCannyEdgeDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		IppByteImage imgEdge;
		IppEdgeCanny(img, imgEdge, dlg.m_fSigma, dlg.m_fLowTh, dlg.m_fHighTh);
		CONVERT_IMAGE_TO_DIB(imgEdge, dib)

		AfxPrintInfo(_T("[캐니 엣지 검출] 입력 영상: %s, sigma: %4.2f, Low Th: %4.2f, High Th: %4.2f"), 
			GetTitle(), dlg.m_fSigma, dlg.m_fLowTh, dlg.m_fHighTh);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnUpdateEdgeCanny(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnHoughLine()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgEdge;
	IppEdgeCanny(img, imgEdge, 1.4f, 30.f, 60.f);

	std::vector<IppLineParam> lines;
	IppHoughLine(imgEdge, lines);

	if (lines.size() == 0)
	{
		AfxMessageBox(_T("검출된 직선이 없습니다."));
		return;
	}

	std::sort(lines.begin(), lines.end());

	// 최대 10개의 직선만 화면에 그려줌.
	int cnt = min(10, lines.size());
	for (int i = 0; i < cnt; i++)
		IppDrawLine(img, lines[i], 255);

	CONVERT_IMAGE_TO_DIB(img, dib)

	AfxPrintInfo(_T("[허프 선 검출] 입력 영상: %s, 중요 직선: rho = %4.2f, angle = %4.2f, vote = %d"),
		GetTitle(), lines[0].rho, (lines[0].ang*180/3.14f), lines[0].vote);
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateHoughLine(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnHarrisCorner()
{
	CHarrisCornerDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		std::vector<IppPoint> corners;
		IppHarrisCorner(img, corners, dlg.m_nHarrisTh);

		BYTE** ptr = img.GetPixels2D();

		int x, y;
		for (IppPoint cp : corners)
		{
			x = cp.x;
			y = cp.y;

			ptr[y - 1][x - 1] = ptr[y - 1][x] = ptr[y - 1][x + 1] = 0;
			ptr[y][x - 1] = ptr[y][x] = ptr[y][x + 1] = 0;
			ptr[y + 1][x - 1] = ptr[y + 1][x] = ptr[y + 1][x + 1] = 0;
		}

		CONVERT_IMAGE_TO_DIB(img, dib)

		AfxPrintInfo(_T("[해리스 코너 검출] 입력 영상: %s, Threshold: %d, 검출된 코너 갯수: %d"), 
			GetTitle(), dlg.m_nHarrisTh, corners.size());
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnUpdateHarrisCorner(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnColorGrayscale()
{
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgColor)
	IppByteImage imgGray;
	imgGray.Convert(imgColor);
	CONVERT_IMAGE_TO_DIB(imgGray, dib)

	AfxPrintInfo(_T("[그레이스케일 변환] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateColorGrayscale(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnColorSplitRgb()
{
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgColor)
	IppByteImage imgR, imgG, imgB;
	IppColorSplitRGB(imgColor, imgR, imgG, imgB);
	CONVERT_IMAGE_TO_DIB(imgR, dibR)
	CONVERT_IMAGE_TO_DIB(imgG, dibG)
	CONVERT_IMAGE_TO_DIB(imgB, dibB)

	AfxPrintInfo(_T("[색상 평면 나누기/RGB] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dibR);
	AfxNewBitmap(dibG);
	AfxNewBitmap(dibB);
}


void CImageToolDoc::OnUpdateColorSplitRgb(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnColorSplitHsi()
{
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgColor)
	IppByteImage imgH, imgS, imgI;
	IppColorSplitHSI(imgColor, imgH, imgS, imgI);
	CONVERT_IMAGE_TO_DIB(imgH, dibH)
	CONVERT_IMAGE_TO_DIB(imgS, dibS)
	CONVERT_IMAGE_TO_DIB(imgI, dibI)

	AfxPrintInfo(_T("[색상 평면 나누기/HSI] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dibH);
	AfxNewBitmap(dibS);
	AfxNewBitmap(dibI);
}


void CImageToolDoc::OnUpdateColorSplitHsi(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnColorSplitYuv()
{
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgColor)
	IppByteImage imgY, imgU, imgV;
	IppColorSplitYUV(imgColor, imgY, imgU, imgV);
	CONVERT_IMAGE_TO_DIB(imgY, dibY)
	CONVERT_IMAGE_TO_DIB(imgU, dibU)
	CONVERT_IMAGE_TO_DIB(imgV, dibV)

	AfxPrintInfo(_T("[색상 평면 나누기/YUV] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dibY);
	AfxNewBitmap(dibU);
	AfxNewBitmap(dibV);
}


void CImageToolDoc::OnUpdateColorSplitYuv(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnColorCombineRgb()
{
	CColorCombineDlg dlg;
	dlg.m_strColorSpace = _T("RGB 색상 평면 합치기");
	if (dlg.DoModal() == IDOK)
	{
		CImageToolDoc* pDoc1 = (CImageToolDoc*)dlg.m_pDoc1;
		CImageToolDoc* pDoc2 = (CImageToolDoc*)dlg.m_pDoc2;
		CImageToolDoc* pDoc3 = (CImageToolDoc*)dlg.m_pDoc3;

		CONVERT_DIB_TO_BYTEIMAGE(pDoc1->m_Dib, imgR)
		CONVERT_DIB_TO_BYTEIMAGE(pDoc2->m_Dib, imgG)
		CONVERT_DIB_TO_BYTEIMAGE(pDoc3->m_Dib, imgB)

		IppRgbImage imgColor;
		if (IppColorCombineRGB(imgR, imgG, imgB, imgColor) == false)
		{
			AfxMessageBox(_T("영상의 크기가 다릅니다!"));
			return;
		}

		CONVERT_IMAGE_TO_DIB(imgColor, dib)

		AfxPrintInfo(_T("[색상 평면 합치기/RGB] 입력 영상: R: %s, G: %s, B: %s"), 
			pDoc1->GetTitle(), pDoc2->GetTitle(), pDoc3->GetTitle());
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnColorCombineHsi()
{
	CColorCombineDlg dlg;
	dlg.m_strColorSpace = _T("HSI 색상 평면 합치기");
	if (dlg.DoModal() == IDOK)
	{
		CImageToolDoc* pDoc1 = (CImageToolDoc*)dlg.m_pDoc1;
		CImageToolDoc* pDoc2 = (CImageToolDoc*)dlg.m_pDoc2;
		CImageToolDoc* pDoc3 = (CImageToolDoc*)dlg.m_pDoc3;

		CONVERT_DIB_TO_BYTEIMAGE(pDoc1->m_Dib, imgH)
		CONVERT_DIB_TO_BYTEIMAGE(pDoc2->m_Dib, imgS)
		CONVERT_DIB_TO_BYTEIMAGE(pDoc3->m_Dib, imgI)

		IppRgbImage imgColor;
		if (IppColorCombineHSI(imgH, imgS, imgI, imgColor) == false)
		{
			AfxMessageBox(_T("영상의 크기가 다릅니다!"));
			return;
		}

		CONVERT_IMAGE_TO_DIB(imgColor, dib)

		AfxPrintInfo(_T("[색상 평면 합치기/HSI] 입력 영상: H: %s, S: %s, I: %s"),
			pDoc1->GetTitle(), pDoc2->GetTitle(), pDoc3->GetTitle());
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnColorCombineYuv()
{
	CColorCombineDlg dlg;
	dlg.m_strColorSpace = _T("YUV 색상 평면 합치기");
	if (dlg.DoModal() == IDOK)
	{
		CImageToolDoc* pDoc1 = (CImageToolDoc*)dlg.m_pDoc1;
		CImageToolDoc* pDoc2 = (CImageToolDoc*)dlg.m_pDoc2;
		CImageToolDoc* pDoc3 = (CImageToolDoc*)dlg.m_pDoc3;

		CONVERT_DIB_TO_BYTEIMAGE(pDoc1->m_Dib, imgY)
		CONVERT_DIB_TO_BYTEIMAGE(pDoc2->m_Dib, imgU)
		CONVERT_DIB_TO_BYTEIMAGE(pDoc3->m_Dib, imgV)

		IppRgbImage imgColor;
		if (IppColorCombineYUV(imgY, imgU, imgV, imgColor) == false)
		{
			AfxMessageBox(_T("영상의 크기가 다릅니다!"));
			return;
		}

		CONVERT_IMAGE_TO_DIB(imgColor, dib)

		AfxPrintInfo(_T("[색상 평면 합치기/YUV] 입력 영상: Y: %s, U: %s, V: %s"),
			pDoc1->GetTitle(), pDoc2->GetTitle(), pDoc3->GetTitle());
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnColorEdge()
{
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
	IppByteImage imgEdge;
	IppColorEdge(img, imgEdge);
	CONVERT_IMAGE_TO_DIB(imgEdge, dib)

	AfxPrintInfo(_T("[컬러 엣지 검출] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateColorEdge(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnSegmentBinarization()
{
	CBinarizationDlg dlg;
	dlg.SetImage(m_Dib);
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		IppByteImage imgRes;
		IppBinarization(img, imgRes, dlg.m_nThreshold);
		CONVERT_IMAGE_TO_DIB(imgRes, dib)

		AfxPrintInfo(_T("[이진화] 입력 영상: %s, 임계값: %d"), GetTitle(), dlg.m_nThreshold);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnUpdateSegmentBinarization(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnSegmentLabeling()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppIntImage imgLabel;
	std::vector<IppLabelInfo> labels;
	int label_cnt = IppLabeling(img, imgLabel, labels);

	// 객체를 감싸는 사각형 그리기
	BYTE** ptr = img.GetPixels2D();
	for (IppLabelInfo& info : labels)
	{
		for (int j = info.miny; j <= info.maxy; j++)
			ptr[j][info.minx] = ptr[j][info.maxx] = 128;

		for (int i = info.minx; i <= info.maxx; i++)
			ptr[info.miny][i] = ptr[info.maxy][i] = 128;
	}

#if 0
	for (IppLabelInfo& info : labels)
	{
		// 각각의 객체를 새 창으로 띄우기
		IppByteImage imgObj(info.maxx - info.minx + 1, info.maxy - info.miny + 1);
		BYTE** pObj = imgObj.GetPixels2D();
		for (IppPoint& pt : info.pixels)
			pObj[pt.y - info.miny][pt.x - info.minx] = 255;

		CONVERT_IMAGE_TO_DIB(imgObj, dibObj)
		AfxNewBitmap(dibObj);
	}
#endif

	CONVERT_IMAGE_TO_DIB(img, dib)

	AfxPrintInfo(_T("[레이블링] 입력 영상: %s, 객체 개수: %d"), GetTitle(), label_cnt);
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateSegmentLabeling(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnContourTracing()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppIntImage imgLabel;
	std::vector<IppLabelInfo> labels;
	int label_cnt = IppLabeling(img, imgLabel, labels);

	IppByteImage imgContour(img.GetWidth(), img.GetHeight());
	BYTE** ptr = imgContour.GetPixels2D();
	for (IppLabelInfo& info : labels)
	{
		std::vector<IppPoint> cp;
		IppContourTracing(img, info.pixels[0].x, info.pixels[0].y, cp);

		for (IppPoint& pt : cp)
			ptr[pt.y][pt.x] = 255;
	}

	CONVERT_IMAGE_TO_DIB(imgContour, dib)

	AfxPrintInfo(_T("[외곽선 추적] 입력 영상: %s, 객체 개수: %d"), GetTitle(), label_cnt);
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateContourTracing(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnMorphologyErosion()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgDst;
	IppMorphologyErosion(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[이진 모폴로지/침식] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateMorphologyErosion(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnMorphologyDilation()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgDst;
	IppMorphologyDilation(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[이진 모폴로지/팽창] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateMorphologyDilation(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnMorphologyOpening()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgDst;
	IppMorphologyOpening(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[이진 모폴로지/열기] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateMorphologyOpening(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnMorphologyClosing()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgDst;
	IppMorphologyClosing(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[이진 모폴로지/닫기] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateMorphologyClosing(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnGraymorphErosion()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgDst;
	IppMorphologyGrayErosion(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[그레이스케일 모폴로지/침식] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateGraymorphErosion(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnGraymorphDilation()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgDst;
	IppMorphologyGrayDilation(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[그레이스케일 모폴로지/닫기] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateGraymorphDilation(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnGraymorphOpening()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgDst;
	IppMorphologyGrayOpening(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[그레이스케일 모폴로지/열기] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateGraymorphOpening(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnGraymorphClosing()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgDst;
	IppMorphologyGrayClosing(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[그레이스케일 모폴로지/닫기] 입력 영상: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateGraymorphClosing(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnFourierDescriptor()
{
	CFourierDescDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		IppIntImage imgLabel;
		std::vector<IppLabelInfo> labels;
		int label_cnt = IppLabeling(img, imgLabel, labels);

		IppByteImage imgContour;
		imgContour.CreateImage(img.GetWidth(), img.GetHeight());
		BYTE** ptr = imgContour.GetPixels2D();

		for (IppLabelInfo& info : labels)
		{
			std::vector<IppPoint> cp;
			IppFourierDescriptor(img, info.pixels[0].x, info.pixels[0].y, dlg.m_nPercent, cp);

			for (IppPoint pt : cp)
			{
				ptr[pt.y][pt.x] = 255;
			}
		}

		CONVERT_IMAGE_TO_DIB(imgContour, dib)

		AfxPrintInfo(_T("[푸리에 기술자] 입력 영상: %s, 객체 개수: %d, 복원 비율: %d%%"), 
			GetTitle(), label_cnt, dlg.m_nPercent);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnUpdateFourierDescriptor(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnInvariantMoments()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	double m[7] = { 0., };
	IppInvariantMoments(img, m);

	double mm[7] = { 0., };
	CString strMoments = _T("");
	for (int i = 0; i < 7; i++) 
	{
		if (m[i] == 0) continue;
		mm[i] = abs(log(abs(m[i])));
		strMoments.AppendFormat(_T("m[%d] = %6.3lf"), i + 1, mm[i]);
		if (i < 6) strMoments.Append(_T(", "));
	}

	AfxPrintInfo(_T("[불변 모멘트] 입력 영상: %s, 불변 모멘트(log): %s"),
		GetTitle(), strMoments);
}


void CImageToolDoc::OnUpdateInvariantMoments(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnZernikeMoments()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)

	AfxPrintInfo(_T("[저니키 모멘트] 입력 영상: %s, 저니키 모멘트(Mag) (n <= 6):"), GetTitle());

	double zr, zi;
	CString strMoments = _T("");
	for (int n = 0; n <= 6; n++)
	{
		strMoments = _T("  ");
		for (int m = (n % 2); m <= n; m += 2)
		{
			IppZernikeMoments(img, n, m, zr, zi);
			strMoments.AppendFormat(_T("|zm(%d, %d)| = %7.3lf"), n, m, hypot(zr, zi));
			if (m < n - 1) strMoments.Append(_T(", "));
		}
		AfxPrintInfo(strMoments);
	}
}


void CImageToolDoc::OnUpdateZernikeMoments(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnTemplateMatching()
{
	TCHAR szFilter[] = _T("Bitmap Files (*.BMP)|*.BMP|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	if (dlg.DoModal() != IDOK)
		return;

	CString pathName = dlg.GetPathName();
	IppDib dibTmpl;
	if (!dibTmpl.Load(CT2A(pathName)))
	{
		AfxMessageBox(_T("파일을 불러오지 못했습니다."));
		return;
	}
	AfxNewBitmap(dibTmpl);

	if (m_Dib.GetWidth() < dibTmpl.GetWidth() || m_Dib.GetHeight() < dibTmpl.GetHeight())
	{
		AfxMessageBox(_T("템플릿 영상의 크기가 입력 영상보다 큽니다."));
		return;
	}

	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	CONVERT_DIB_TO_BYTEIMAGE(dibTmpl, imgTmpl)
	IppIntImage imgMap;
	IppPoint dp = IppTemplateMatching(img, imgTmpl, imgMap);

#if 0
	{
		IppByteImage imgCvt(img.GetWidth(), img.GetHeight());
		BYTE* pCvt = imgCvt.GetPixels();
		int* pMap = imgMap.GetPixels();

		int max_value = 0;
		for (int i = 0; i < img.GetSize(); i++)
		{
			if (pMap[i] > max_value) max_value = pMap[i];
		}

		if (max_value != 0)
		{
			for (int i = 0; i < img.GetSize(); i++)
			{
				pCvt[i] = pMap[i] * 255 / max_value;
			}
		}

		CONVERT_IMAGE_TO_DIB(imgCvt, dibMap)
		AfxNewBitmap(dibMap);
	}
#endif

	{ // 입력 영상에 BOX 그리기
		int tw2 = imgTmpl.GetWidth() / 2;
		int th2 = imgTmpl.GetHeight() / 2;

		int minx = dp.x - tw2;
		int maxx = dp.x + tw2;
		int miny = dp.y - th2;
		int maxy = dp.y + th2;

		BYTE** ptr = img.GetPixels2D();

		for (int j = miny; j < maxy; j++)
			ptr[j][minx] = ptr[j][maxx] = 255;

		for (int i = minx; i < maxx; i++)
			ptr[miny][i] = ptr[maxy][i] = 255;
	}

	CONVERT_IMAGE_TO_DIB(img, dib)

	AfxPrintInfo(_T("[템플릿 매칭] 입력 영상: %s, 템플릿 영상: %s, 검출 좌표: (%d, %d)"),
		GetTitle(), dlg.GetFileName(), dp.x, dp.y);
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateTemplateMatching(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

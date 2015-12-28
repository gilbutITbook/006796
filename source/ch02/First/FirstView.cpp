
// FirstView.cpp : CFirstView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "First.h"
#endif

#include "FirstDoc.h"
#include "FirstView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFirstView

IMPLEMENT_DYNCREATE(CFirstView, CView)

BEGIN_MESSAGE_MAP(CFirstView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CFirstView 생성/소멸

CFirstView::CFirstView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CFirstView::~CFirstView()
{
}

BOOL CFirstView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CFirstView 그리기

void CFirstView::OnDraw(CDC* pDC)
{
	CFirstDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	int i, dan;
	CString str;

	for (dan = 2; dan <= 9; dan++)
	{
		for (i = 1; i <= 9; i++)
		{
			str.Format(_T("%dx%d=%d"), dan, i, dan * i);
			pDC->TextOut((dan - 2) * 70 + 20, 20 * i, str);
		}
	}
}


// CFirstView 인쇄

BOOL CFirstView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CFirstView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CFirstView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CFirstView 진단

#ifdef _DEBUG
void CFirstView::AssertValid() const
{
	CView::AssertValid();
}

void CFirstView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFirstDoc* CFirstView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFirstDoc)));
	return (CFirstDoc*)m_pDocument;
}
#endif //_DEBUG


// CFirstView 메시지 처리기


// FirstView.cpp : CFirstView 클래스의 구현
// 차일드 프레임 윈도우 안의 흰색 영역을 표현한다.
// 보통 CFirstDoc 클래스를 참조하여 데이터 화면의 출력을 담당한다.

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
//뷰 영역을 다시 그려야 할 필요가 있을 때 자동으로 실행
// 응용 프로그램의 크기가 변경되거나 다른 응용프로그램에 의해 일부 가려졌던
// 부분이 다시 전면으로 나올때 호출된다.
// 또는 내부 데이터가 변경되어 프로그래머가 Invalidate또는 UpdateAllViews
// 등의 함수를 사용하여 강제로 다시 그리기를 시도할 때도 OnDraw함수가 호출된다.
// 이 함수는 CView 클래스에 가상함수로 선언되어 있고,
// 실제 구현은 프로그래머가 생성한 CFirstView클래스에서 작성해야 한다.

void CFirstView::OnDraw(CDC* pDC)
{
	//OnDraw 함수의 인자를 보면 CDC클래스의 포인터를 넘겨주는 것을 볼수 있다.
	//CDC클래스는 윈도우에서 그림을 그릴때 필요한 디바이스 컨텍스트DC,
	//Device Context를 추상화한 클래스
	//디바이스 컨텍스트는 글씨, 그림, 비트맵등 출력에 관련된 모든 정보를 모아둔 데이터 구조체
	//현재 뷰 영역에 특정색의  선을 그리거나, 특정 폰트를 사용하여 글씨를 출력하려면
	//디바이스컨텍스트 정보를 변경해 주어야 한다.

	// GetDocument()함수는 해당 뷰 클래스와 관련된 도큐먼트 클래스의 포인터를 얻어 오는 함수
	// CFirst 클래스에서  GetDocument()함수를 호출하면
	//CFirstDoc 클래스의 포인터를 받을 수 있다.
	// 화면 출력은 도큐먼트 클래스가 가지고 있는 데이터를 기반으로 이루어짐
	CFirstDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);//pDoc포인터 값이 유효한지를 검사하는 코드
	if (!pDoc)
		return;

	//TODO:
	int i, dan;
	CString str;//CString 은 MFC에서 제공하는 문자열 처리 클래스
	//CString 클래스는 Format이라는 멤버 함수를 제공

	for (dan = 2; dan <= 9; dan++)
	{
		for (i = 1; i <= 9; i++)
		{
			str.Format(_T("%dx%d=%d"), dan, i, dan * i);
			pDC->TextOut((dan - 2) * 70 + 20, 20 * i, str);
		}
	}
}
//_T()매크로 사용: 유니코드 문자 집합을 사용하기 때문에
// TextOut함수 x,y 좌표와 문자열을 인자로 받아 
// 윈도우 내의(x,y)좌표에 문자열을 출력

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

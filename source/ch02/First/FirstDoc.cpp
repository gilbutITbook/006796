
// FirstDoc.cpp : CFirstDoc 클래스의 구현
// 데이타이 보관및 관리를 담당한다.
// 다중 문서 인터페이스의 경우 여러 개의 인스턴스가 생성될수 있다.

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "First.h"
#endif

#include "FirstDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CFirstDoc

IMPLEMENT_DYNCREATE(CFirstDoc, CDocument)

BEGIN_MESSAGE_MAP(CFirstDoc, CDocument)
END_MESSAGE_MAP()


// CFirstDoc 생성/소멸

CFirstDoc::CFirstDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CFirstDoc::~CFirstDoc()
{
}

// 새문서가 생성 될때 호출
// 사용자가 프로그램에서 [파일]>[새로 만들기]를 선택했을때 호출 된다.
// 새 도큐먼트가 생성될 때 필요한 초기화 작업은 OnNewDocument함수에 추가하면된다.
// 도큐먼트가 새로 생성되는 방법에는 완전히 새로운 도큐먼트를 만드는 경도 있지만,
// [파일]>[열기] 메뉴처럼 기존의 파일을 열면서 새 도큐먼트가 생성되는 경우도 있다.
// 이럴때는 도큐먼트 클래스에서 OnOpenDocument함수가 실행된다.
// 만약 프로그램에서 파일 열기 기능을 추가하고 싶은 경우에는 OnOpenDocument함수를
// 재정의해야한다.
BOOL CFirstDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CFirstDoc serialization

void CFirstDoc::Serialize(CArchive& ar)
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

//클래스 뷰 목록에는 나타나지만 실제로는 사용하지 않는다.
#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CFirstDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CFirstDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CFirstDoc::SetSearchContent(const CString& value)
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

// CFirstDoc 진단

#ifdef _DEBUG
void CFirstDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFirstDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CFirstDoc 명령

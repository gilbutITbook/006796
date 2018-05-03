
// FirstDoc.cpp : CFirstDoc Ŭ������ ����
// ����Ÿ�� ������ ������ ����Ѵ�.
// ���� ���� �������̽��� ��� ���� ���� �ν��Ͻ��� �����ɼ� �ִ�.

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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


// CFirstDoc ����/�Ҹ�

CFirstDoc::CFirstDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CFirstDoc::~CFirstDoc()
{
}

// �������� ���� �ɶ� ȣ��
// ����ڰ� ���α׷����� [����]>[���� �����]�� ���������� ȣ�� �ȴ�.
// �� ��ť��Ʈ�� ������ �� �ʿ��� �ʱ�ȭ �۾��� OnNewDocument�Լ��� �߰��ϸ�ȴ�.
// ��ť��Ʈ�� ���� �����Ǵ� ������� ������ ���ο� ��ť��Ʈ�� ����� �浵 ������,
// [����]>[����] �޴�ó�� ������ ������ ���鼭 �� ��ť��Ʈ�� �����Ǵ� ��쵵 �ִ�.
// �̷����� ��ť��Ʈ Ŭ�������� OnOpenDocument�Լ��� ����ȴ�.
// ���� ���α׷����� ���� ���� ����� �߰��ϰ� ���� ��쿡�� OnOpenDocument�Լ���
// �������ؾ��Ѵ�.
BOOL CFirstDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CFirstDoc serialization

void CFirstDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}

//Ŭ���� �� ��Ͽ��� ��Ÿ������ �����δ� ������� �ʴ´�.
#ifdef SHARED_HANDLERS

// ����� �׸��� �����մϴ�.
void CFirstDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// ������ �����͸� �׸����� �� �ڵ带 �����Ͻʽÿ�.
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

// �˻� ó���⸦ �����մϴ�.
void CFirstDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������ �����Ϳ��� �˻� �������� �����մϴ�.
	// ������ �κ��� ";"�� ���еǾ�� �մϴ�.

	// ��: strSearchContent = _T("point;rectangle;circle;ole object;");
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

// CFirstDoc ����

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


// CFirstDoc ���


// FirstView.cpp : CFirstView Ŭ������ ����
// ���ϵ� ������ ������ ���� ��� ������ ǥ���Ѵ�.
// ���� CFirstDoc Ŭ������ �����Ͽ� ������ ȭ���� ����� ����Ѵ�.

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CFirstView ����/�Ҹ�

CFirstView::CFirstView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CFirstView::~CFirstView()
{
}

BOOL CFirstView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CFirstView �׸���
//�� ������ �ٽ� �׷��� �� �ʿ䰡 ���� �� �ڵ����� ����
// ���� ���α׷��� ũ�Ⱑ ����ǰų� �ٸ� �������α׷��� ���� �Ϻ� ��������
// �κ��� �ٽ� �������� ���ö� ȣ��ȴ�.
// �Ǵ� ���� �����Ͱ� ����Ǿ� ���α׷��Ӱ� Invalidate�Ǵ� UpdateAllViews
// ���� �Լ��� ����Ͽ� ������ �ٽ� �׸��⸦ �õ��� ���� OnDraw�Լ��� ȣ��ȴ�.
// �� �Լ��� CView Ŭ������ �����Լ��� ����Ǿ� �ְ�,
// ���� ������ ���α׷��Ӱ� ������ CFirstViewŬ�������� �ۼ��ؾ� �Ѵ�.

void CFirstView::OnDraw(CDC* pDC)
{
	//OnDraw �Լ��� ���ڸ� ���� CDCŬ������ �����͸� �Ѱ��ִ� ���� ���� �ִ�.
	//CDCŬ������ �����쿡�� �׸��� �׸��� �ʿ��� ����̽� ���ؽ�ƮDC,
	//Device Context�� �߻�ȭ�� Ŭ����
	//����̽� ���ؽ�Ʈ�� �۾�, �׸�, ��Ʈ�ʵ� ��¿� ���õ� ��� ������ ��Ƶ� ������ ����ü
	//���� �� ������ Ư������  ���� �׸��ų�, Ư�� ��Ʈ�� ����Ͽ� �۾��� ����Ϸ���
	//����̽����ؽ�Ʈ ������ ������ �־�� �Ѵ�.

	// GetDocument()�Լ��� �ش� �� Ŭ������ ���õ� ��ť��Ʈ Ŭ������ �����͸� ��� ���� �Լ�
	// CFirst Ŭ��������  GetDocument()�Լ��� ȣ���ϸ�
	//CFirstDoc Ŭ������ �����͸� ���� �� �ִ�.
	// ȭ�� ����� ��ť��Ʈ Ŭ������ ������ �ִ� �����͸� ������� �̷����
	CFirstDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);//pDoc������ ���� ��ȿ������ �˻��ϴ� �ڵ�
	if (!pDoc)
		return;

	//TODO:
	int i, dan;
	CString str;//CString �� MFC���� �����ϴ� ���ڿ� ó�� Ŭ����
	//CString Ŭ������ Format�̶�� ��� �Լ��� ����

	for (dan = 2; dan <= 9; dan++)
	{
		for (i = 1; i <= 9; i++)
		{
			str.Format(_T("%dx%d=%d"), dan, i, dan * i);
			pDC->TextOut((dan - 2) * 70 + 20, 20 * i, str);
		}
	}
}
//_T()��ũ�� ���: �����ڵ� ���� ������ ����ϱ� ������
// TextOut�Լ� x,y ��ǥ�� ���ڿ��� ���ڷ� �޾� 
// ������ ����(x,y)��ǥ�� ���ڿ��� ���

// CFirstView �μ�

BOOL CFirstView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CFirstView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CFirstView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CFirstView ����

#ifdef _DEBUG
void CFirstView::AssertValid() const
{
	CView::AssertValid();
}

void CFirstView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFirstDoc* CFirstView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFirstDoc)));
	return (CFirstDoc*)m_pDocument;
}
#endif //_DEBUG


// CFirstView �޽��� ó����

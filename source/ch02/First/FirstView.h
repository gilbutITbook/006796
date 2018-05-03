// FirstView.h : CFirstView Ŭ������ �������̽�
//

#pragma once

// �������� ����� ����ϴ� �� Ŭ����
// �� ������ ���ϵ� ������ ���� ���� ��� �����̸� �� ���� �ȿ� �۾��� ����ϰų�
// �׸��� �׸��� �۾��� ���
// CFirstViewŬ������ ViewŬ������ �ڽ�Ŭ����
class CFirstView : public CView
{
protected: // serialization������ ��������ϴ�.
	CFirstView();
	DECLARE_DYNCREATE(CFirstView)

// Ư���Դϴ�.
public:
	CFirstDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CFirstView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // FirstView.cpp�� ����� ����
inline CFirstDoc* CFirstView::GetDocument() const
   { return reinterpret_cast<CFirstDoc*>(m_pDocument); }
#endif


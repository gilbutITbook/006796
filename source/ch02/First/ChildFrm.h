
// ChildFrm.h : CChildFrame Ŭ������ �������̽�
//

#pragma once
// CChildFrame Ŭ������ �� ������ ���δ� ���ϵ� ������ �����츦 ǥ���ϴ� Ŭ����
// ���� ���� �������̽��� �������α׷����� ���ϵ� ������ ������� ���� ������ ������
// �ȿ� ���� ���� ������ �� �ִ�.
// ��������� ���� ���� �������̽��� ���α׷������� CChildFrameŬ������ �������� �ʴ´�.
class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// �����Դϴ�.
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};

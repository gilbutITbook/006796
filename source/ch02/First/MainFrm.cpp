
// MainFrm.cpp : CMainFrame Ŭ������ ����
//

#include "stdafx.h"
#include "First.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame
// ���� ������ �������� ����� ����Ѵ�.
// �⺻������ ���� ǥ����, �޴�, ����, ���¹� â�� ��� ������ �����ȴ�.
// ���� ���� �������̽� ���α׷����� ���� ������ ������� ���α׷� ��ü��
// ���δ� Ʋ�� ������ �Ѵ�.
// ���α׷� �޴��� ����, ���¹�, �ּ�ȭ/�ִ�ȭ/�ݱ� ��ư�� ������ �ִ�.

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CMainFrame::~CMainFrame()
{
}

//���������� �����찡 ������ �� �ٷ� ȣ��Ǵ� �Լ�
// ���������� �������� �ʱ�ȭ �۾��� ��� �� �Լ����� �̷������.


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// ��� ������ �����쿡 �������� �ʿ��� �ʱ�ȭ �۾��� ���������Ѵ�.
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// ���α׷��� ���� ������ �����쿡�� ����� ���ٿ� ���¹� �����츦 ����� �۾��� �Ѵ�.
	// m_wndToolBar�� CToolBarŬ���� Ÿ������ ���ǵ� �����̰�, 
	// m_wndStatusBar�� CStatusBarŬ���� Ÿ���� ��� �����̴�.

	//���ٸ� �����ϱ� ���ؼ� CToolBarŬ������ ��� �Լ� CreateEX�� ���
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("���� ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}
	//���¹ٸ� �����ϱ� ���ؼ� CStatusBarŬ������ ��� �Լ� Create�� ���
	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("���� ǥ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: ���� ������ ��ŷ�� �� ���� �Ϸ��� �� �� ���� �����Ͻʽÿ�.
	//���ٿ� ���¹ٸ� ������ ���¹� ������ �����ų� ���ٸ� ���� ������ �����쿡 ��ŷ
	//�ϴ� �۾��� ������
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return TRUE;
}

// CMainFrame ����

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	// ����� �뵵���Լ�
	// ���α׷� ���ۿ��� ū ������ ��ġ�� �ʴ´�.
	CMDIFrameWnd::AssertValid();

}

void CMainFrame::Dump(CDumpContext& dc) const
{
	// ����� �뵵�� �Լ�
	// ���α׷� ���ۿ��� ū ������ ��ġ�� �ʴ´�.
	CMDIFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame �޽��� ó����


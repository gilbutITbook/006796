
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "First.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame
// 메인 프레임 윈도우의 기능을 담당한다.
// 기본적으로 제목 표시줄, 메뉴, 툴바, 상태바 창의 경계 등으로 구성된다.
// 다중 문서 인터페이스 프로그램에서 메인 프레임 윈도우는 프로그램 전체를
// 감싸는 틀의 역할을 한다.
// 프로그램 메뉴와 툴바, 상태바, 최소화/최대화/닫기 버튼을 가지고 있다.

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

//메인프레임 윈도우가 생성된 후 바로 호출되는 함수
// 메인프레임 윈도우의 초기화 작업은 모두 이 함수에서 이루어진다.


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// 모든 프레임 윈도우에 공통으로 필요한 초기화 작업을 먼저수행한다.
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// 프로그램의 메인 프레임 윈도우에서 사용할 툴바와 상태바 윈도우를 만드는 작업을 한다.
	// m_wndToolBar는 CToolBar클래스 타입으로 정의된 변수이고, 
	// m_wndStatusBar는 CStatusBar클래스 타입의 멤버 변수이다.

	//툴바를 생성하기 위해서 CToolBar클래스의 멤버 함수 CreateEX를 사용
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("도구 모음을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	//상태바를 생성하기 위해서 CStatusBar클래스의 멤버 함수 Create를 사용
	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 도구 모음을 도킹할 수 없게 하려면 이 세 줄을 삭제하십시오.
	//툴바와 상태바를 생성후 상태바 구역을 나누거나 툴바를 메인 프레임 윈도우에 도킹
	//하는 작업을 수행함
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	// 디버깅 용도용함수
	// 프로그램 동작에는 큰 영향을 끼치지 않는다.
	CMDIFrameWnd::AssertValid();

}

void CMainFrame::Dump(CDumpContext& dc) const
{
	// 디버깅 용도용 함수
	// 프로그램 동작에는 큰 영향을 끼치지 않는다.
	CMDIFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기


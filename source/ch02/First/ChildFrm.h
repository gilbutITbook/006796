
// ChildFrm.h : CChildFrame 클래스의 인터페이스
//

#pragma once
// CChildFrame 클래스는 뷰 영역을 감싸는 차일드 프레임 윈도우를 표현하는 클래스
// 다중 문서 인터페이스의 응용프로그램에서 차일드 프레임 윈도우는 메인 프레임 윈도우
// 안에 여러 개가 생성될 수 있다.
// 상대적으로 단일 문서 인터페이스의 프로그램에서는 CChildFrame클래스가 존재하지 않는다.
class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
};

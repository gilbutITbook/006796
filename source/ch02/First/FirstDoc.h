
// FirstDoc.h : CFirstDoc 클래스의 인터페이스
//


#pragma once

// 프로그램에서 사용되는 데이터의 입출력과 관리를 담당하는 도큐먼트의 기능을 구현한 클래스
// CFirstDoc클래스는 CDocument클래스로 부터 상속받아 만들어 진다.
class CFirstDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CFirstDoc();
	DECLARE_DYNCREATE(CFirstDoc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CFirstDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};

#ifndef __PAGE__
#define __PAGE__

class cPageBase														// 페이지 기본 클래스.
{
protected:
	DWORD					m_dwPageId;								// 페이지 아이디.

	cLinkedList<DWORD>		m_pDialogue;							// 페이지를 담는 링크드 리스트.

	int						m_nDialogueCount;						// 다이얼로그 카운트.
	
	int						m_nNextPageId;							// 다음 페이지 아이디.
	int						m_nPrevPageId;							// 이전 페이지 아이디.

	BYTE					mEmotion;								// 이모션.

public:
	cPageBase();													// 생성자 함수.
	virtual ~cPageBase();											// 소멸자 함수.

	virtual void Init( DWORD dwId, BYTE emotion = 0 );				// 초기화 함수.

	void AddDialogue(DWORD dwId);									// 다이얼로그 추가 함수.

	virtual void RemoveAll();										// 모두 비우기 함수.

	DWORD GetPageId() { return m_dwPageId; }						// 페이지 아이디를 반환하는 함수.
	DWORD GetDialogue() ;
	DWORD GetDialogue(int index) ;
	DWORD GetRandomDialogue();										// 랜덤 다이얼로그를 반환하는 함수.

	int GetDialogueCount() { return m_nDialogueCount; }				// 다이얼로그 카운트를 반환하는 함수.

	void SetNextPageId(int id) { m_nNextPageId = id; }				// 다음 페이지 아이디를 세팅하는 함수.
	void SetPrevPageId(int id) { m_nPrevPageId = id; }				// 이전 페이지 아이디를 세팅하는 함수.

	int GetNextPageId() { return m_nNextPageId; }					// 다음 페이지 아이디를 반환하는 함수.
	int GetPrevPageId() { return m_nPrevPageId; }					// 이전 페이지 아이디를 반환하는 함수.
	
	BYTE GetEmotion() { return mEmotion; }							// 이모션을 반환하는 함수.
};

class cPage : public cPageBase										// 기본 페이지 클래스를 상속받은 페이지 클래스.
{
protected:
	cLinkedList<HYPERLINK>	m_pHyperLink;							// 하이퍼 링크 리스트.

	int						m_nHyperLinkCount;						// 하이퍼 링크 카운트.

public:
	cPage();														// 생성자 함수.
	virtual ~cPage();												// 소멸자 함수.

	void AddHyperLink(HYPERLINK* pLink);							// 하이퍼 링크를 추가하는 함수.

	virtual void RemoveAll();										// 모두 비우기 함수.

	int GetHyperLinkCount() { return m_nHyperLinkCount; }			// 하이퍼 링크 카운트를 반환하는 함수.

	HYPERLINK* GetHyperText( DWORD dwIdx );							// 하이퍼 링크 정보를 반환하는 함수.
};

#endif
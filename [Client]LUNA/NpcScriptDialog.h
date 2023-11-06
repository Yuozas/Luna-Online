#ifndef __NPCSCRIPT_DIALOG__
#define __NPCSCRIPT_DIALOG__

#include "cListDialogEx.h"														// 리스트 다이얼로그 확장 클래스 헤더를 불러온다.
#include "cPage.h"																// 페이지 클래스 헤더를 불러온다.


class cButton ;																	// 버튼 클래스를 선언한다.
class cStatic;																	// 스태틱 클래스를 선언한다.
class cDialogueList ;															// 다이얼로그 리스트 클래스를 선언한다.
class cHyperTextList ;															// 하이퍼 텍스트 클래스를 선언한다.


class cNpcScriptDialog  : public cDialog										// 다이얼로그 클래스를 상속받은 NPC 스크립트 다이얼로그 클래스.
{
protected:
	DWORD			m_dwCurNpc;													// 현재 NPC 아이디.
	WORD			m_wNpcChxNum ;												// 현재 NPC CHX 번호.
	DWORD			m_dwCurPageId;												// 현재 페이지 아이디.
	DWORD			m_dwMainLinkPageId ;										// 메인 링크 페이지 아이디.

	HYPER			m_sHyper[MAX_REGIST_HYPERLINK];								// 최대 하이퍼 링크 등록 수 만큼 크기의 하이퍼 구조체 배열.
	HYPER			m_sConversationHyper[MAX_REGIST_HYPERLINK];					// 최대 하이퍼 링크 등록 수 만큼 크기의 대사 링크 구조체 배열.
	int				m_nHyperCount;
	int				m_nConversationHyperCount ;

	cListDialogEx*	m_pListDlg;													// NPC 대사 리스트 다이얼로그.
	cListDialogEx*	m_pLinkListDlg ;											// NPC와 관련 된 링크 리스트 다이얼로그.

	cButton*		m_pTopBtn ;													// [ 처음화면 ] 버튼.
	cButton*		m_pEndBtn ;													// [ 대화종료 ] 버튼.


	cStatic*		m_pTitleText;												// 타이틀 텍스트 스태틱.

	/*cPtrList		m_pConversationList ;
	cPtrList		m_pLinkedList ;*/

public:
	cNpcScriptDialog();															// 생성자 함수.
	virtual ~cNpcScriptDialog();												// 소멸자 함수.

	void Linking();																// 링크 함수.

	virtual void OnActionEvent(LONG lId, void* p, DWORD we) ;					// 버튼 이벤트를 처리할 함수.

	virtual void SetActive( BOOL val );											// 활성화 여부를 세팅하는 함수.

	void DisActiveEx();															// 비활성화 확장 함수.

	void ResetConversationList(cDialogueList* pList, DWORD dwMsg) ;				// 대화창을 다시 세팅 하는 함수.
	void ResetMainLinkList(cPage* pMainPage, cHyperTextList* pHyper, cListDialogEx* pListDlg) ;	// 링크 페이지를 다시 세팅하는 함수.
	void ResetConversationLinkList(cPage* pMainPage, cHyperTextList* pHyper) ;
	void SettingNpcImage(cPage* pMainPage, WORD npcChxNum) ;					// NPC 이미지와 배경을 세팅하는 함수.											

	BOOL OpenDialog( DWORD dwNpcId, WORD npcChxNum ) ;							// 다이얼로그를 여는 함수.

	void RefreshQuestLinkPage() ;
	BOOL OpenLinkPage( DWORD dwPageId );										// 링크 페이지를 여는 함수.

	void EndDialog();															// 다이얼로그를 종료하는 함수.

	void HyperLinkParser( DWORD dwIdx, BOOL bConversation );					// 하이퍼 링크 파서 함수.

public:
	void SetCurSelectedNpcId( DWORD dwId ) { m_dwCurNpc = dwId-1; }				// 현재 선택 된 NPC 아이디를 반환하는 함수.

	void SetDlgTitleText( char* strTitle );										// 다이얼로그 타이틀 텍스트를 세팅하는 함수.

	HYPER* GetHyperInfo( DWORD dwIdx );											// 하이퍼 링크 정보를 반환하는 함수.

	void OpenQuestLinkPage( DWORD dwPageId );									// 퀘스트 링크 페이지를 여는 함수.
	// 070917 LYW --- NpcScriptDialog : Add function to open page to change map.
	void OpenChangeMapLinkPage( DWORD dwPageId ) ;								// 맵 변경 링크 페이지를 여는 함수.
	// 070917 LYW --- NpcScriptDialog : Add function to refresh page to change map.
	void RefreshChangeMapLinkPage() ;

	DWORD	m_dwQuestIdx;														// 퀘스트 인덱스를 담는 변수.
};

#endif
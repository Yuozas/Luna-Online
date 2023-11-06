// Quest.h: interface for the CQuest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUEST_H__60770757_F904_4A60_A796_6E1092C9447D__INCLUDED_)
#define AFX_QUEST_H__60770757_F904_4A60_A796_6E1092C9447D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\[CC]Quest\QuestDefines.h"												// 퀘스트 정의 선언 헤더를 불러온다.

class CQuestInfo;																	// 퀘스트 정보 클래스를 선언한다.
class CSubQuestInfo;																// 서브 퀘스트 정보 클래스를 선언한다.

class CQuest																		// 퀘스트 클래스.
{
protected:
	CQuestInfo*			m_pQuestInfo;												// 퀘스트 정보를 담는 포인터.
	CSubQuestInfo*		m_CurSubQuestInfoArray[MAX_SUBQUEST];						// 서브 퀘스트 정보를 담는 배열.
	
	DWORD				m_dwSubQuestCount;											// 서브 퀘스트 개수.
	SUBQUEST			m_SubQuestTable[MAX_SUBQUEST];								// 서브 퀘스트 테이블.
	DWORD				m_dwSubQuestFlag;											// 서브 퀘스트 플래그.
	DWORD				m_dwData;													// 데이터 값.
	DWORD				m_dwTime;													// 시간.

	DWORD				m_dwCurSubQuestIdx ;										// 현재 진행중인 서브 퀘스트 인덱스.
	
	BOOL				m_bCheckTime;												// 시간 체크 여부 변수.
	BYTE				m_CheckType;												// 체크 타입 변수.
	stTIME				m_CheckTime;												// 시간 정보를 담는 변수.

public:
	CQuest( CQuestInfo* pQuestInfo );												// 생성자 함수.
	virtual ~CQuest();																// 소멸자 함수.

	void	Initialize();															// 초기화 함수.
	// set
	void	SetMainQuestData( DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime, BYTE bCheckType, DWORD dwCheckTime );	// 메인 퀘스트 데이터를 세팅하는 함수.
	void	SetSubQuestData( DWORD dwSubQuestIdx, DWORD dwSubData, DWORD dwSubTime );									// 서브 퀘스트 데이터를 세팅하는 함수.
	void	StartQuest( DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime );												// 퀘스트를 시작하는 함수.
	void	EndQuest( DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime );												// 퀘스트를 종료하는 함수.
	void	StartSubQuest( DWORD dwSubQuestIdx, DWORD dwSubQuestFlag, DWORD dwSubData, DWORD dwSubTime );				// 서브 퀘스트를 시작하는 함수.
	void	EndSubQuest( DWORD dwSubQuestIdx, DWORD dwSubQuestFlag, DWORD dwSubData, DWORD dwSubTime );					// 서브 퀘스트를 종료하는 함수.
	void	UpdateSubQuestData( DWORD dwSubQuestIdx, DWORD dwMaxCount, DWORD dwSubData, DWORD dwSubTime );				// 서브 퀘스트를 업데이트 하는 함수.
	// getfn
	inline DWORD	IsQuestComplete()		{ return m_dwData; }					// 퀘스트의 완료 여부를 반환하는 함수.
	inline DWORD	GetTime()				{ return m_dwTime; }					// 시간을 반환하는 함수.
	inline DWORD	GetEndParam()			{ return m_dwData; }					// 종료 파라메터를 반환하는 함수.
	inline DWORD	GetSubQuestFlag()		{ return m_dwSubQuestFlag; }			// 서브 퀘스트 플래그를 반환하는 함수.
	inline BOOL		IsTimeCheck()			{ return m_bCheckTime; }				// 시간 체크 여부를 반환하는 함수.
	inline BYTE		GetCheckType()			{ return m_CheckType; }					// 체크 타입을 반환하는 함수.
	inline stTIME*	GetCheckTime()			{ return &m_CheckTime; }				// 시간을 반환하는 함수.

	DWORD			GetQuestIdx();													// 퀘스트 인덱스를 반환하는 함수.
	BOOL			IsSubQuestComplete( DWORD dwSubQuestIdx );						// 서브 퀘스트의 완료 여부를 반환하는 함수.
	CSubQuestInfo*	GetSubQuestInfo( DWORD dwSubQuestIdx );							// 서브 퀘스트 정보를 반환하는 함수.
	SUBQUEST* GetSubQuest(int Index)		{ return &m_SubQuestTable[Index]; }		// 서브 퀘스트 정보를 반환하는 함수.

	// npc
	BOOL	IsNpcRelationQuest( DWORD dwNpcIdx );									// npc관련 퀘스트인지 반환하는 함수.
	DWORD	GetNpcScriptPage( DWORD dwNpcIdx );										// npc 스크립트 페이지를 반환하는 함수.
	DWORD	GetNpcMarkType( DWORD dwNpcIdx );										// npc 마크 타입을 반환하는 함수.
	int		IsQuestState( DWORD dwNpcIdx );											// 퀘스트 상태를 리턴하는 함수.
	
	// quest start item
	BOOL	CanItemUse( DWORD dwQuestIdx );											// 아이템 사용 여부를 리턴하는 함수.

	void	RegistCheckTime( BYTE bCheckType, DWORD dwCheckTime );					// 시간 체크를 등록하는 함수.
	void	UnRegistCheckTime();													// 시간 체크를 해제하는 함수.

	CSubQuestInfo* GetSubQuestInfoArray(int nIdx) { return m_CurSubQuestInfoArray[nIdx] ; }

	DWORD GetCurSubQuestIdx() { return m_dwCurSubQuestIdx ; }
};

#endif // !defined(AFX_QUEST_H__60770757_F904_4A60_A796_6E1092C9447D__INCLUDED_)

// SubQuestInfo.h: interface for the CSubQuestInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUBQUESTINFO_H__AC05E747_C207_4E04_8D35_09F423BA4A7E__INCLUDED_)
#define AFX_SUBQUESTINFO_H__AC05E747_C207_4E04_8D35_09F423BA4A7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "QuestDefines.h"															// 퀘스트 정의 선언 헤더파일을 불러온다.

class CQuestLimit;																	// 퀘스트 제한 클래스를 선언한다.
class CQuestTrigger;																// 퀘스트 트리거 클래스를 선언한다.
class CQuestNpcScript;																// 퀘스트 npc 스크립트 클래스를 선언한다.
class CQuestNpc;																	// 퀘스트 npc 클래스를 선언한다.

class CQuestGroup;																	// 퀘스트 그룹 클래스를 선언한다.
class CQuest;																		// 퀘스트 클래스를 선언한다.
class CQuestEvent;																	// 퀘스트 이벤트 클래스를 선언한다.

class CSubQuestInfo																	// 서브 퀘스트 정보 클래스.
{
protected:
	cPtrList	m_QuestLimitList;													// 퀘스트 제한 정보를 담는 리스트.
	cPtrList	m_QuestTriggerList;													// 퀘스트 트리거 정보를 담는 리스트.
	cPtrList	m_QuestNpcScriptList;												// 퀘스트 npc 스크립트 리스트.
	cPtrList	m_QuestNpcList;														// 퀘스트 npc 리스트.

	DWORD		m_dwMaxCount;														// 최대 카운트.

public:
	CSubQuestInfo();																// 생성자 함수.
	virtual ~CSubQuestInfo();														// 소멸자 함수.
	
	void	Release();																// 해제 함수.
	// script
	void	AddQuestLimit( CQuestLimit* pQuestLimit );								// 퀘스트 제한을 추가하는 함수.
	void	AddQuestTrigger( CQuestTrigger* pQuestTrigger );						// 퀘스트 트리거를 추가하는 함수.

	void	AddQuestNpcScipt( CQuestNpcScript* pQuestNpcScript );					// 퀘스트 npc 스크립트를 추가하는 함수.
	cPtrList* GetQuestNpcScriptList() { return &m_QuestNpcScriptList ; }

	void	AddQuestNpc( CQuestNpc* pQuestNpc );									// 퀘스트 npc를 추가하는 함수.

	// event
	void	OnQuestEvent( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest, CQuestEvent* pQuestEvent);	// 퀘스트 이벤트를 적용하는 함수.
	// npc
	BOOL	IsNpcRelationQuest( DWORD dwNpcIdx );									// npc와 관련된 퀘스트 인지 여부를 반환하는 함수.
	DWORD	GetNpcScriptPage( DWORD dwNpcIdx );										// npc  스크립트 페이지를 반환하는 함수.
	DWORD	GetNpcMarkType( DWORD dwNpcIdx );										// npc 마크 타입을 반환하는 함수.
	int		IsQuestState( DWORD dwNpcIdx );											// 퀘스트 상태를 확인하는 함수.
	
	// getfn
	cPtrList*	GetQuestNpcList()		{ return &m_QuestNpcList; }					// 퀘스트 npc 리스트를 반환하는 함수.
	
	// item use
	BOOL	CheckLimitCondition( DWORD dwQuestIdx );								// 제한 상태를 체크하는 함수.

	DWORD	GetMaxCount()						{ return m_dwMaxCount; }			// 최대 카운트를 반환하는 함수.
	void	SetMaxCount( DWORD dwMaxCount )		{ m_dwMaxCount = dwMaxCount; }		// 최대 카운트를 설정하는 함수.

	cPtrList* GetTriggerList() { return &m_QuestTriggerList ; }
};

#endif // !defined(AFX_SUBQUESTINFO_H__AC05E747_C207_4E04_8D35_09F423BA4A7E__INCLUDED_)

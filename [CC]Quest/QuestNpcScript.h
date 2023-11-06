// QuestNpcScript.h: interface for the CQuestNpcScript class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTNPCSCRIPT_H__D5E07727_465F_40AF_93D5_889FF980954F__INCLUDED_)
#define AFX_QUESTNPCSCRIPT_H__D5E07727_465F_40AF_93D5_889FF980954F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStrTokenizer;																			// 스트링 토크나이져 클래스를 선언한다.

class CQuestNpcScript																			// NPC 스크립트 클래스.
{
protected:
	DWORD		m_dwQuestIdx;																	// 퀘스트 인덱스를 담는 변수.
	DWORD		m_dwSubQuestIdx;																// 서브 퀘스트 인덱스를 담는 변수.
	cPtrList	m_NpcScriptList;																// NPC 스크립트 리스트.

public:
	CQuestNpcScript( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );			// 생성자 함수.
	virtual ~CQuestNpcScript();																	// 소멸자 함수.

	void	Release();																			// 해제 함수.
	// script
	cPtrList* GetNpcScriptList() { return &m_NpcScriptList ; }
	BOOL	ReadNpcScript( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );		// NPC 스크립트를 읽는 함수.
	// getfn
	DWORD	IsSameNpc( DWORD dwNpcIdx, DWORD* pQuestIdx, DWORD* pSubQuestIdx );					// npc 스크립트 페이지를 리턴하는 함수.
	DWORD	IsSameNpcMark( DWORD dwNpcIdx, DWORD* pQuestIdx, DWORD* pSubQuestIdx );				// npc 마크를 리턴하는 함수.
};

#endif // !defined(AFX_QUESTNPCSCRIPT_H__D5E07727_465F_40AF_93D5_889FF980954F__INCLUDED_)

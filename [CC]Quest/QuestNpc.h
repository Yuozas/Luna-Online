// QuestNpc.h: interface for the CQuestNpc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTNPC_H__98C86EC2_02F3_4878_8C8E_8D9AD352BCB9__INCLUDED_)
#define AFX_QUESTNPC_H__98C86EC2_02F3_4878_8C8E_8D9AD352BCB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CStrTokenizer;																		// 스트링 토크나이져 클래스를 선언한다.

class CQuestNpc																				// 퀘스트 NPC 클래스.
{
protected:
	DWORD		m_dwQuestIdx;																// 퀘스트 인덱스를 담는 변수.
	DWORD		m_dwSubQuestIdx;															// 서브 퀘스트 인덱스를 담는 변수.
	cPtrList	m_NpcDataList;																// NPC 데이터 리스트.

public:
	CQuestNpc( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );				// 생성자 함수.
	virtual ~CQuestNpc();																	// 소멸자 함수.

	void	Release();																		// 해제 함수.
	// script
	BOOL	ReadNpcData( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );	// NPC 데이터를 읽는 함수.
	
	// getfn
	cPtrList*	GetNpcDataList()		{ return &m_NpcDataList; }							// NPC 데이터 리스트를 반환하는 함수.
};

#endif // !defined(AFX_QUESTNPC_H__98C86EC2_02F3_4878_8C8E_8D9AD352BCB9__INCLUDED_)

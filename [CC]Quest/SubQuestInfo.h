// SubQuestInfo.h: interface for the CSubQuestInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUBQUESTINFO_H__AC05E747_C207_4E04_8D35_09F423BA4A7E__INCLUDED_)
#define AFX_SUBQUESTINFO_H__AC05E747_C207_4E04_8D35_09F423BA4A7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "QuestDefines.h"															// ����Ʈ ���� ���� ��������� �ҷ��´�.

class CQuestLimit;																	// ����Ʈ ���� Ŭ������ �����Ѵ�.
class CQuestTrigger;																// ����Ʈ Ʈ���� Ŭ������ �����Ѵ�.
class CQuestNpcScript;																// ����Ʈ npc ��ũ��Ʈ Ŭ������ �����Ѵ�.
class CQuestNpc;																	// ����Ʈ npc Ŭ������ �����Ѵ�.

class CQuestGroup;																	// ����Ʈ �׷� Ŭ������ �����Ѵ�.
class CQuest;																		// ����Ʈ Ŭ������ �����Ѵ�.
class CQuestEvent;																	// ����Ʈ �̺�Ʈ Ŭ������ �����Ѵ�.

class CSubQuestInfo																	// ���� ����Ʈ ���� Ŭ����.
{
protected:
	cPtrList	m_QuestLimitList;													// ����Ʈ ���� ������ ��� ����Ʈ.
	cPtrList	m_QuestTriggerList;													// ����Ʈ Ʈ���� ������ ��� ����Ʈ.
	cPtrList	m_QuestNpcScriptList;												// ����Ʈ npc ��ũ��Ʈ ����Ʈ.
	cPtrList	m_QuestNpcList;														// ����Ʈ npc ����Ʈ.

	DWORD		m_dwMaxCount;														// �ִ� ī��Ʈ.

public:
	CSubQuestInfo();																// ������ �Լ�.
	virtual ~CSubQuestInfo();														// �Ҹ��� �Լ�.
	
	void	Release();																// ���� �Լ�.
	// script
	void	AddQuestLimit( CQuestLimit* pQuestLimit );								// ����Ʈ ������ �߰��ϴ� �Լ�.
	void	AddQuestTrigger( CQuestTrigger* pQuestTrigger );						// ����Ʈ Ʈ���Ÿ� �߰��ϴ� �Լ�.

	void	AddQuestNpcScipt( CQuestNpcScript* pQuestNpcScript );					// ����Ʈ npc ��ũ��Ʈ�� �߰��ϴ� �Լ�.
	cPtrList* GetQuestNpcScriptList() { return &m_QuestNpcScriptList ; }

	void	AddQuestNpc( CQuestNpc* pQuestNpc );									// ����Ʈ npc�� �߰��ϴ� �Լ�.

	// event
	void	OnQuestEvent( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest, CQuestEvent* pQuestEvent);	// ����Ʈ �̺�Ʈ�� �����ϴ� �Լ�.
	// npc
	BOOL	IsNpcRelationQuest( DWORD dwNpcIdx );									// npc�� ���õ� ����Ʈ ���� ���θ� ��ȯ�ϴ� �Լ�.
	DWORD	GetNpcScriptPage( DWORD dwNpcIdx );										// npc  ��ũ��Ʈ �������� ��ȯ�ϴ� �Լ�.
	DWORD	GetNpcMarkType( DWORD dwNpcIdx );										// npc ��ũ Ÿ���� ��ȯ�ϴ� �Լ�.
	int		IsQuestState( DWORD dwNpcIdx );											// ����Ʈ ���¸� Ȯ���ϴ� �Լ�.
	
	// getfn
	cPtrList*	GetQuestNpcList()		{ return &m_QuestNpcList; }					// ����Ʈ npc ����Ʈ�� ��ȯ�ϴ� �Լ�.
	
	// item use
	BOOL	CheckLimitCondition( DWORD dwQuestIdx );								// ���� ���¸� üũ�ϴ� �Լ�.

	DWORD	GetMaxCount()						{ return m_dwMaxCount; }			// �ִ� ī��Ʈ�� ��ȯ�ϴ� �Լ�.
	void	SetMaxCount( DWORD dwMaxCount )		{ m_dwMaxCount = dwMaxCount; }		// �ִ� ī��Ʈ�� �����ϴ� �Լ�.

	cPtrList* GetTriggerList() { return &m_QuestTriggerList ; }
};

#endif // !defined(AFX_SUBQUESTINFO_H__AC05E747_C207_4E04_8D35_09F423BA4A7E__INCLUDED_)

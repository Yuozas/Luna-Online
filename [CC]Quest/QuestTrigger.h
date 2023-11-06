// QuestTrigger.h: interface for the CQuestTrigger class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTTRIGGER_H__E9012D36_44B8_469F_A5D6_4E42CDE29383__INCLUDED_)
#define AFX_QUESTTRIGGER_H__E9012D36_44B8_469F_A5D6_4E42CDE29383__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "QuestDefines.h"															// ����Ʈ ���� ���� ��������� �ҷ��´�.

class CQuestCondition;																// ����Ʈ ���� Ŭ������ �����Ѵ�.
class CStrTokenizer;																// ��Ʈ�� ��ũ������ Ŭ������ �����Ѵ�.

class CQuestGroup;																	// ����Ʈ �׷� Ŭ������ �����Ѵ�.
class CQuest;																		// ����Ʈ Ŭ������ �����Ѵ�.
class CQuestEvent;																	// ����Ʈ �̺�Ʈ Ŭ������ �����Ѵ�.

class CQuestTrigger																	// ����Ʈ Ʈ���� Ŭ����.
{
protected:
	CQuestCondition*	m_pQuestCondition;											// ����Ʈ ���� Ŭ���� ������.
	cPtrList			m_QuestExeList;												// ����Ʈ ���� ����Ʈ.
	DWORD				m_dwEndParam;												// ���� �Ķ����.

public:
	CQuestTrigger( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );	// ������ �Լ�.
	virtual ~CQuestTrigger();														// �Ҹ��� �Լ�.

	void	Release();																// ���� �Լ�.
	
	// script
	BOOL	ReadTrigger( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );							// Ʈ���Ÿ� �о���̴� �Լ�.

	// event
	BOOL	OnQuestEvent( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest, CQuestEvent* pQuestEvent );// ����Ʈ �̺�Ʈ�� ó���ϴ� �Լ�.	

	//
	DWORD	GetEndParam()		{ return m_dwEndParam;	}															// ���� �Ķ���͸� ��ȯ�ϴ� �Լ�.

	cPtrList* GetExeList() { return &m_QuestExeList ; }
};

#endif // !defined(AFX_QUESTTRIGGER_H__E9012D36_44B8_469F_A5D6_4E42CDE29383__INCLUDED_)

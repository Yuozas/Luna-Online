// QuestExecute_Quest.h: interface for the CQuestExecute_Quest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTEXECUTE_QUEST_H__57E0B691_1C0E_4CD1_9AFA_7466B934B556__INCLUDED_)
#define AFX_QUESTEXECUTE_QUEST_H__57E0B691_1C0E_4CD1_9AFA_7466B934B556__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "QuestExecute.h"														// ����Ʈ ���� ��� ������ �ҷ��´�.

class CQuestExecute_Quest : public CQuestExecute								// ����Ʈ ���� Ŭ������ ��ӹ��� ����Ʈ ���� ������ Ŭ����.
{
protected:	
	DWORD		m_dwExeQuestIdx;												// ���� ����Ʈ �ε���.
	DWORD		m_dwExeSubQuestIdx;												// ���� ���� ����Ʈ �ε���.

	// 080104 LYW --- QuestExecute_Quest : ����Ʈ�� �� �̵� ����� ����, �� �̵� ���� ����� �߰��Ѵ�.
	MAPTYPE		m_DestinationMapNum ;											// ������ �� ��ȣ�� ���� ����.

	float		m_fDestination_XPos ;											// ���������� X ��ǥ.
	float		m_fDestination_ZPos ;											// ���������� Z ��ǥ.

public:
	CQuestExecute_Quest( DWORD dwExecuteKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );	// ������ �Լ�.
	virtual ~CQuestExecute_Quest();																				// �Ҹ��� �Լ�.

	virtual BOOL	Execute( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest );					// ���� �Լ�.

	virtual int		CheckCondition( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest );

	DWORD			GetQuestExeIdx()		{ return m_dwExeQuestIdx;	}										// ���� ����Ʈ �ε��� ��ȯ �Լ�.
};

#endif // !defined(AFX_QUESTEXECUTE_QUEST_H__57E0B691_1C0E_4CD1_9AFA_7466B934B556__INCLUDED_)

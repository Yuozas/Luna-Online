// QuestExecute_Time.h: interface for the QuestExecute_Time class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTEXECUTE_TIME_H__2D255BAF_3708_4FDD_A755_9A7E9509CFE9__INCLUDED_)
#define AFX_QUESTEXECUTE_TIME_H__2D255BAF_3708_4FDD_A755_9A7E9509CFE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "QuestExecute.h"													// ����Ʈ ���� ��� ������ �ҷ��´�.

class CQuestExecute_Time : public CQuestExecute								// ����Ʈ ���� Ŭ������ ��ӹ��� ����Ʈ ���� �ð� Ŭ����.
{
	BYTE		m_bType;													// Ÿ��.
	DWORD		m_dwDay;													// ��¥.
	DWORD		m_dwHour;													// �ð�.
	DWORD		m_dwMin;													// ��.

public:
	CQuestExecute_Time( DWORD dwExecuteKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );	// ������ �Լ�.
	virtual ~CQuestExecute_Time();																				// �Ҹ��� �Լ�.

	virtual BOOL	Execute( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest );					// ���� �Լ�.

	virtual int		CheckCondition( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest );
};

#endif // !defined(AFX_QUESTEXECUTE_TIME_H__2D255BAF_3708_4FDD_A755_9A7E9509CFE9__INCLUDED_)

// QuestExecute_Count.h: interface for the CQuestExecute_Count class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTEXECUTE_COUNT_H__DE6A351F_3B48_4B9F_9724_EBAABAAD99B2__INCLUDED_)
#define AFX_QUESTEXECUTE_COUNT_H__DE6A351F_3B48_4B9F_9724_EBAABAAD99B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "QuestExecute.h"													// ����Ʈ ���� ��� ������ �ҷ��´�.

class CQuestExecute_Count : public CQuestExecute							// ����Ʈ ���� Ŭ������ ��ӹ��� ����Ʈ ���� ī��Ʈ Ŭ����.
{
protected:
	DWORD		m_dwRealSubQuestIdx;										// ���� ���� ����Ʈ �ε���.
	DWORD		m_dwMaxCount;												// �ִ� ī��Ʈ.
	DWORD		m_dwWeaponKind;												// ���� ����
	DWORD		m_dwParam1;													// �Ķ���� 1
	
public:
	CQuestExecute_Count( DWORD dwExecuteKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );	// ������ �Լ�
	virtual ~CQuestExecute_Count();																				// �Ҹ��� �Լ�.

	virtual BOOL	Execute( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest );					// ���� �Լ�.

	virtual int		CheckCondition( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest );
};

#endif // !defined(AFX_QUESTEXECUTE_COUNT_H__DE6A351F_3B48_4B9F_9724_EBAABAAD99B2__INCLUDED_)

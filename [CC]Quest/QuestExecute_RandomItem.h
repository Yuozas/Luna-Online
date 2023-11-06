// QuestExecute_RandomItem.h: interface for the CQuestExecute_RandomItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTEXECUTE_RANDOMITEM_H__E5FAF1F8_D9F2_4B21_9792_D7F488E99EF6__INCLUDED_)
#define AFX_QUESTEXECUTE_RANDOMITEM_H__E5FAF1F8_D9F2_4B21_9792_D7F488E99EF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "QuestExecute.h"														// ����Ʈ ���� ��� ������ �ҷ��´�.

struct RANDOMITEM																// ���� ������ ������ ��� ����ü.
{
	WORD	wItemIdx;															// ������ �ε����� ��� ����.
	WORD	wItemNum;															// ������ ���� ��� ����.
	WORD	wPercent;															// �ۼ�Ʈ�� ��� ����.
};

class CQuestExecute_RandomItem : public CQuestExecute							// ����Ʈ ���� Ŭ������ ��ӹ��� ����Ʈ ���� ���� ������ Ŭ����.
{
protected:
	DWORD			m_dwMaxItemCount;											// ������ �ִ� ī��Ʈ.
	DWORD			m_dwRandomItemCount;										// ���� ������ ī��Ʈ.
	RANDOMITEM*		m_pRandomItem;												// ���� ������ ������.
	
public:
	CQuestExecute_RandomItem( DWORD dwExecuteKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );	// ������ �Լ�.
	virtual ~CQuestExecute_RandomItem();																			// �Ҹ��� �Լ�.

	virtual BOOL	Execute( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest );						// ���� �Լ�.

	virtual int		CheckCondition( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest );

	RANDOMITEM*		GetRandomItem();																				// ���� ������ ������ ��ȯ�ϴ� �Լ�.
};

#endif // !defined(AFX_QUESTEXECUTE_RANDOMITEM_H__E5FAF1F8_D9F2_4B21_9792_D7F488E99EF6__INCLUDED_)

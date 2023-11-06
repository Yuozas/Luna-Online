// QuestExecute_Item.h: interface for the CQuestExecute_Item class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTEXECUTE_ITEM_H__7ED33FD7_DE37_44D7_80C3_E4EC290224C2__INCLUDED_)
#define AFX_QUESTEXECUTE_ITEM_H__7ED33FD7_DE37_44D7_80C3_E4EC290224C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "QuestExecute.h"															// ����Ʈ ���� ��� ������ �ҷ��´�.

class CQuestExecute_Item : public CQuestExecute										// ����Ʈ ���� Ŭ������ ��ӹ��� ����Ʈ ���� ������ Ŭ����.
{
protected:
	DWORD		m_dwItemKind;														// ������ ����.
	DWORD		m_dwItemNum;														// ������ ����.
	DWORD		m_dwItemProbability;												// ������ �Ӽ�?Ȯ��?
	DWORD		m_dwWeaponKind;														// ���� ����.

public:
	CQuestExecute_Item( DWORD dwExecuteKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	virtual ~CQuestExecute_Item();

	virtual BOOL	Execute( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest );

	virtual int		CheckCondition( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest );

	DWORD		GetItemKind()			{ return m_dwItemKind ; }
	DWORD		GetItemNum()			{ return m_dwItemNum ; }
	DWORD		GetItemProbability()	{ return m_dwItemProbability ; }
	DWORD		GetWeaponKind()			{ return m_dwWeaponKind ; }
};

#endif // !defined(AFX_QUESTEXECUTE_ITEM_H__7ED33FD7_DE37_44D7_80C3_E4EC290224C2__INCLUDED_)

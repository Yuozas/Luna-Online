// QuestExecute.h: interface for the CQuestExecute class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTEXECUTE_H__4FDEA86A_E9B2_40CC_A83D_F7864A04BDBA__INCLUDED_)
#define AFX_QUESTEXECUTE_H__4FDEA86A_E9B2_40CC_A83D_F7864A04BDBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "QuestDefines.h"													// ����Ʈ ���� ���� ����� �ҷ��´�.

enum ENUM_QUEST_EXC_ERROR
{
	e_EXC_ERROR_NO_ERROR = -1,
	e_EXC_ERROR_NO_PLAYERINFO = 0,
	e_EXC_ERROR_NO_QUESTGROUP, 
	e_EXC_ERROR_NO_QUEST,
	e_EXC_ERROR_NO_INVENTORYINFO,
	e_EXC_ERROR_NO_EXTRASLOT,
	e_EXC_ERROR_NO_EXTRASLOTQUEST,
	e_EXC_ERROR_NO_QUESTITEM,
	e_EXC_ERROR_NOT_ENOUGH_QUESTITEM,
	e_EXC_ERROR_NO_GIVEITEM,
	e_EXC_ERROR_NOT_ENOUGH_GIVEITEM,
	e_EXC_ERROR_NOT_ENOUGH_GIVEMONEY,
	e_EXC_ERROR_NOT_SAME_WEAPONKIND,
	e_EXC_ERROR_NOT_SAME_WEAPONKIND_COUNT,
	e_EXC_ERROR_NO_RANDOMITEMLIST,
	e_EXC_ERROR_NO_INVENTORYINFO_RANDOMITEM,
	e_EXC_ERROR_CANT_RECEIVE_MONEY,
} ;

class CStrTokenizer;														// ��Ʈ�� ��ũ������ Ŭ������ �����Ѵ�.
class CQuestGroup;															// ����Ʈ �׷� Ŭ������ �����Ѵ�.
class CQuest;																// ����Ʈ Ŭ������ �����Ѵ�.

class CQuestExecute															// ����Ʈ ���� Ŭ����.
{
protected:		
	DWORD	m_dwQuestIdx;													// ����Ʈ �ε����� ���� ����.
	DWORD	m_dwSubQuestIdx;												// ���� ����Ʈ �ε����� ���� ����.
	DWORD	m_dwExecuteKind;												// ����Ʈ ���� Ÿ���� ���� ����.

public:
	CQuestExecute( DWORD dwExecuteKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );	// ������ �Լ�.
	virtual ~CQuestExecute();																				// �Ҹ��� �Լ�.

	virtual BOOL	Execute( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest ) = 0;			// ���� �Լ�.

	virtual int		CheckCondition(PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest ) = 0;		// ������ ���� ���� üũ �Լ�.
	
	DWORD			GetQuestExecuteKind()		{ return m_dwExecuteKind;	}								// ���� Ÿ���� ��ȯ�ϴ� �Լ�.
};

#endif // !defined(AFX_QUESTEXECUTE_H__4FDEA86A_E9B2_40CC_A83D_F7864A04BDBA__INCLUDED_)

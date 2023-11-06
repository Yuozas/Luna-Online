// QuestLimit.h: interface for the CQuestLimit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTLIMIT_H__B72ED18D_6647_469A_B38E_3BD1B295B469__INCLUDED_)
#define AFX_QUESTLIMIT_H__B72ED18D_6647_469A_B38E_3BD1B295B469__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "QuestDefines.h"																		// ����Ʈ ���� ���� ��������� �ҷ��´�.

class CStrTokenizer;																			// ��Ʈ�� ��ũ������ Ŭ������ �����Ѵ�.
class CQuestGroup;																				// ����Ʈ �׷� Ŭ������ �����Ѵ�.
class CQuest;																					// ����Ʈ Ŭ������ �����Ѵ�.

class CQuestLimit																				// ����Ʈ ���� Ŭ����.
{
protected:
	cPtrList	m_QuestLimitKindList;															// ����Ʈ ���� ���� ����Ʈ.

public:
	CQuestLimit( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );				// ������ �Լ�.
	virtual ~CQuestLimit();																		// �Ҹ��� �Լ�.

	void	Release();																			// ���� �Լ�.
	// script
	BOOL	ReadLimitKind( CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );		// ���� ������ �д� �Լ�.
	// check
	BOOL	CheckLimit( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest );		// ����Ʈ ������ üũ�Ѵ�. - server
	BOOL	CheckLimit( DWORD dwQuestIdx, DWORD dwSubQuestIdx );								// ����Ʈ ������ üũ�Ѵ�. - client
};

#endif // !defined(AFX_QUESTLIMIT_H__B72ED18D_6647_469A_B38E_3BD1B295B469__INCLUDED_)

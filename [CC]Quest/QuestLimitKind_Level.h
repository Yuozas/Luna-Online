// QuestLimitKind_Level.h: interface for the CQuestLimitKind_Level class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTLIMITKIND_LEVEL_H__8B4328F3_3190_4E4D_82D4_184C42A6B07C__INCLUDED_)
#define AFX_QUESTLIMITKIND_LEVEL_H__8B4328F3_3190_4E4D_82D4_184C42A6B07C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "QuestLimitKind.h"											// ����Ʈ ����Ʈ ���� ����� �ҷ��´�.

class CQuestLimitKind_Level : public CQuestLimitKind				// ����Ʈ ����Ʈ ���� Ŭ������ ��� ����, ����Ʈ ����Ʈ ���� ���� Ŭ����.
{
protected:
	DWORD		m_dwMin;											// �ּ� ���� ���� ����.
	DWORD		m_dwMax;											// �ִ� ���� ���� ����.

public:
	CQuestLimitKind_Level( DWORD dwLimitKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );	// ������ �Լ�.
	virtual ~CQuestLimitKind_Level();																			// �Ҹ��� �Լ�.

	virtual BOOL	CheckLimit( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest );				// ����Ʈ ����Ʈ üũ�Լ�.
	virtual BOOL	CheckLimit( DWORD dwQuestIdx, DWORD dwSubQuestIdx );										// ����Ʈ ����Ʈ üũ�Լ�.
};

#endif // !defined(AFX_QUESTLIMITKIND_LEVEL_H__8B4328F3_3190_4E4D_82D4_184C42A6B07C__INCLUDED_)

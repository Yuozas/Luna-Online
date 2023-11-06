// QuestLimitKind.h: interface for the CQuestLimitKind class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTLIMITKIND_H__4048A25A_091D_4BEB_986E_3C324C0D32EF__INCLUDED_)
#define AFX_QUESTLIMITKIND_H__4048A25A_091D_4BEB_986E_3C324C0D32EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "QuestDefines.h"																		// ����Ʈ ���� ���� ��������� �ҷ��´�.

class CStrTokenizer;																			// ��Ʈ�� ��ũ������ Ŭ������ �����Ѵ�.
class CQuestGroup;																				// ����Ʈ �׷� Ŭ������ �����Ѵ�.
class CQuest;																					// ����Ʈ Ŭ������ �����Ѵ�.

class CQuestLimitKind																			// ����Ʈ ����Ʈ Ŭ����.
{
protected:
	DWORD	m_dwQuestIdx;																		// ����Ʈ �ε����� ���� ����.
	DWORD	m_dwSubQuestIdx;																	// ���� ����Ʈ �ε����� ���� ����.
	DWORD	m_dwLimitKind;																		// ����Ʈ ����Ʈ ������ ���� ����.
	
public:
	CQuestLimitKind( DWORD dwLimitKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );	// ������ �Լ�.
	virtual ~CQuestLimitKind();																				// �Ҹ��� �Լ�.

	virtual BOOL	CheckLimit( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest ) = 0;		// ����Ʈ ����Ʈ üũ �Լ�.
	virtual BOOL	CheckLimit( DWORD dwQuestIdx, DWORD dwSubQuestIdx ) = 0;								// ����Ʈ ����Ʈ üũ �Լ�.

	// 070424 LYW --- QuestLimitKind : Add function to return limit kind.
	DWORD GetLimitKind() { return m_dwLimitKind ; }															// ����Ʈ ����Ʈ ������ ��ȯ�ϴ� �Լ�.
};

#endif // !defined(AFX_QUESTLIMITKIND_H__4048A25A_091D_4BEB_986E_3C324C0D32EF__INCLUDED_)

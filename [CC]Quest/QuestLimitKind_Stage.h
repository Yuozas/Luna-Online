#pragma once
#include "questlimitkind.h"										// ����Ʈ ����Ʈ ���� ����� �ҷ��´�.

class CQuestLimitKind_Stage : public CQuestLimitKind			// ����Ʈ ����Ʈ ���� Ŭ������ ��ӹ��� ����Ʈ ����Ʈ ���� �������� Ŭ����.
{
	BYTE byClass ;												// Ŭ������ ���� ����.
	BYTE byRacial ;												// ������ ���� ����.
	BYTE byJobGrade ;											// �� �׷��̵带 ���� ����.
	BYTE byJobIndex ;											// �� �ε����� ���� ����.

	DWORD dwClassIndex ;										// Ŭ���� �ε����� ���� ����.

	DWORD* m_pClassIndex ;										// Ŭ���� �ε��� ������ ����.

	BYTE   m_byCount ;											// ī��Ʈ ����.

public:
	CQuestLimitKind_Stage(DWORD dwLimitKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx);		// ������ �Լ�.
	virtual ~CQuestLimitKind_Stage(void);																			// �Ҹ��� �Լ�.

	virtual BOOL	CheckLimit( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest );					// ����Ʈ üũ �Լ�.(����)
	virtual BOOL	CheckLimit( DWORD dwQuestIdx, DWORD dwSubQuestIdx );											// ����Ʈ üũ �Լ�.(Ŭ���̾�Ʈ)
};

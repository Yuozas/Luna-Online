// QuestExecute.cpp: implementation of the CQuestExecute class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"														// ǥ�� �ý��۰�, ������Ʈ�� ������, ���־��� �ش����� ���� �ش������� �ҷ��´�.
#include "QuestExecute.h"												// ����Ʈ ���� ��� ������ �ҷ��´�.

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// ������ �Լ�.
CQuestExecute::CQuestExecute( DWORD dwExecuteKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	m_dwQuestIdx = dwQuestIdx;											// ����Ʈ �ε����� �����Ѵ�.
	m_dwSubQuestIdx = dwSubQuestIdx;									// ���� ����Ʈ �ε����� �����Ѵ�.
	m_dwExecuteKind = dwExecuteKind;									// ���� ������ �����Ѵ�.
}
// �Ҹ��� �Լ�.
CQuestExecute::~CQuestExecute()
{
}


// QuestLimitKind.cpp: implementation of the CQuestLimitKind class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"															// ǥ�� �ý��۰�, ������Ʈ�� ������, ���־��� �ش����� ���� �ش������� �ҷ��´�.
#include "QuestLimitKind.h"													// ����Ʈ ����Ʈ ���� ��� ������ �ҷ��´�.

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// ������ �Լ�.
CQuestLimitKind::CQuestLimitKind( DWORD dwLimitKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	m_dwQuestIdx = dwQuestIdx;						// ����Ʈ �ε����� �����Ѵ�.
	m_dwSubQuestIdx = dwSubQuestIdx;				// ���� ����Ʈ �ε����� �����Ѵ�.
	m_dwLimitKind = dwLimitKind;					// ����Ʈ ����Ʈ ������ �����Ѵ�.
}

// �Ҹ��� �Լ�.
CQuestLimitKind::~CQuestLimitKind()
{
}
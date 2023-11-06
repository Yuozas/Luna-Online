// QuestCondition.cpp: implementation of the CQuestCondition class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"															// ǥ�� �ý��۰�, ������Ʈ�� ������, ���־��� �ش����� ���� �ش������� �ҷ��´�.
#include "QuestCondition.h"													// ����Ʈ ���� ����� �ҷ��´�.
#include "QuestDefines.h"													// ����Ʈ ���� ���� ����� �ҷ��´�.
#include "QuestEvent.h"														// ����Ʈ �̺�Ʈ ����� �ҷ��´�.

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// ������ �Լ�.
CQuestCondition::CQuestCondition( CQuestEvent* pQuestEvent, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	m_dwQuestIdx = dwQuestIdx;												// ����Ʈ �ε����� �����Ѵ�.
	m_dwSubQuestIdx = dwSubQuestIdx;										// ���� ����Ʈ �ε����� �����Ѵ�.
	m_pQuestEvent = pQuestEvent;											// ����Ʈ �̺�Ʈ�� �޴´�.
}
// �Ҹ��� �Լ�.
CQuestCondition::~CQuestCondition()
{
	if( m_pQuestEvent )														// ����Ʈ �̺�Ʈ ������ ��ȿ�ϸ�,
	{
		delete m_pQuestEvent;												// ����Ʈ �̺�Ʈ ������ �����Ѵ�.
	}
}

BOOL CQuestCondition::CheckCondition( CQuestEvent* pQuestEvent )			// ���¸� üũ�ϴ� �Լ�.
{
	if( m_pQuestEvent->m_dwQuestEventKind == eQuestEvent_HuntAll )			// ����Ʈ �̺�Ʈ ������ ��� ����̸�,
	{
		if( pQuestEvent->m_dwQuestEventKind == eQuestEvent_Hunt )			// ����Ʈ �̺�Ʈ ������ ��ɰ� ���ٸ�,
		{
			return TRUE;													// TRUE�� �����Ѵ�.
		}
		else																// ����Ʈ �̺�Ʈ�� ����� �ƴϸ�,
		{
			return FALSE;													// FALSE�� �����Ѵ�.
		}
	}

	return m_pQuestEvent->IsSame( pQuestEvent );							// ����Ʈ�� ������ ���θ� �����Ѵ�.
}
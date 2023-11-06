// QuestEvent.cpp: implementation of the CQuestEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"																	// ǥ�� �ý��۰�, ������Ʈ�� ������, ���־��� �ش����� ���� �ش������� �ҷ��´�.
#include "QuestEvent.h"																// ����Ʈ �̺�Ʈ ��� ������ �ҷ��´�.

#include "StrTokenizer.h"															// ��Ʈ�� ��ũ������ ����� �ҷ��´�.
#include "QuestScriptLoader.h"														// ����Ʈ ��ũ��Ʈ �δ� ����� �ҷ��´�.

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// ������ �Լ�.
CQuestEvent::CQuestEvent( DWORD dwQuestEventKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
: m_dwQuestEventKind(dwQuestEventKind)
{
	GetScriptParam( m_dwParam1, pTokens );											// �Ķ���� 1�� �����Ѵ�.
	GetScriptParam( m_lParam2, pTokens );											// �Ķ���� 2�� �����Ѵ�.
}
CQuestEvent::CQuestEvent( DWORD dwQuestEventKind, DWORD dwParam1, LONG lParam2 )	// ������ �Լ�.
{
	SetValue(dwQuestEventKind,dwParam1,lParam2);									// �Ķ���� ���� ������ ����Ʈ �̺�Ʈ�� �����Ѵ�.
}

CQuestEvent::~CQuestEvent()															// �Ҹ��� �Լ�.
{
}

void CQuestEvent::SetValue( CQuestEvent* pQuestEvent )								// ����Ʈ �̺�Ʈ�� �����ϴ� �Լ�.
{
	m_dwQuestEventKind = pQuestEvent->m_dwQuestEventKind;							// ����Ʈ �̺�Ʈ ������ �����Ѵ�.
	m_dwParam1 = pQuestEvent->m_dwParam1;											// �Ķ���� 1�� �����Ѵ�.
	m_lParam2 = pQuestEvent->m_lParam2;												// �Ķ���� 2�� �����Ѵ�.
}

void CQuestEvent::SetValue( DWORD dwQuestEventKind, DWORD dwParam1, LONG lParam2 )	// ����Ʈ �̺�Ʈ�� �����ϴ� �Լ�.
{
	m_dwQuestEventKind = dwQuestEventKind;											// ����Ʈ �̺�Ʈ �߷��� �����Ѵ�.
	m_dwParam1 = dwParam1;															// �Ķ���� 1�� �����Ѵ�.
	m_lParam2 = lParam2;															// �Ķ���� 2�� �����Ѵ�.
}

BOOL CQuestEvent::IsSame( CQuestEvent* pQuestEvent )								// ���� ����Ʈ �̺�Ʈ���� ���θ� �����ϴ� �Լ�.
{
	if( m_dwQuestEventKind != pQuestEvent->m_dwQuestEventKind )						// ����Ʈ �̺�Ʈ�� ���� ������,
	{
		return FALSE;																// FALSE ���� ó���� �Ѵ�.
	}

	if( m_dwParam1 != pQuestEvent->m_dwParam1 )										// �Ķ���� 1�� ���� ������,
	{
		return FALSE;																// FALSE ���� ó���� �Ѵ�.
	}

	if( m_lParam2 != pQuestEvent->m_lParam2 )										// �Ķ���� 2�� ���� ������,
	{
		return FALSE;																// FALSE ���� ó���� �Ѵ�.
	}

	return TRUE;																	// TRUE ���� ó���� �Ѵ�.
}

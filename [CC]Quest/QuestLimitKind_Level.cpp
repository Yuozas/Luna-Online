// QuestLimitKind_Level.cpp: implementation of the CQuestLimitKind_Level class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"														// ǥ�� �ý��۰�, ������Ʈ�� ������, ���־��� �ش����� ���� �ش������� �ҷ��´�.
#include "QuestLimitKind_Level.h"										// ����Ʈ ����Ʈ ���� ���� ����� �ҷ��´�.

#include "QuestScriptLoader.h"											// ����Ʈ ��ũ��Ʈ �δ� ����� �ҷ��´�.

#ifdef _MAPSERVER_														// �� ������ ���� �Ǿ� �ִٸ�,

#include "Player.h"														// �÷��̾� ��������� �ҷ��´�.

#else																	// �� ������ ������� �ʾҴٸ�,

#include "ObjectManager.h"												// ������Ʈ �Ŵ��� ����� �ҷ��´�.

#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// ������ �Լ�.
CQuestLimitKind_Level::CQuestLimitKind_Level( DWORD dwLimitKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
: CQuestLimitKind( dwLimitKind, pTokens, dwQuestIdx, dwSubQuestIdx )
{
	m_dwMin = m_dwMax = 0;												// �ּ�, �ִ� ���� ���� 0���� �����Ѵ�.

	if( m_dwLimitKind == eQuestLimitKind_Level )						// ����Ʈ ����Ʈ ������ ���� ���Ѱ� ���ٸ�,
	{
		GetScriptParam( m_dwMin, pTokens );								// �ּ� ���� ���� �޴´�.
		GetScriptParam( m_dwMax, pTokens );								// �ִ� ���� ���� �޴´�.
	}
	else if( m_dwLimitKind == eQuestLimitKind_Money )					// ����Ʈ ����Ʈ ������ �Ӵ� ���Ѱ� ���ٸ�,
	{
		GetScriptParam( m_dwMax, pTokens );								// �ִ� �Ӵ� ���� �޴´�.
	}
	else if( m_dwLimitKind == eQuestLimitKind_Stage )					// ����Ʈ ����Ʈ ������ ���������� ���ٸ�,
	{
		GetScriptParam( m_dwMin, pTokens );								// �ּ� ���� ���� �޴´�.
		GetScriptParam( m_dwMax, pTokens );								// �ִ� ���� ���� �޴´�.
	}
	else if( m_dwLimitKind == eQuestLimitKind_Attr )					// ����Ʈ ����Ʈ ���� Attr�� ������,
	{
		GetScriptParam( m_dwMin, pTokens );								// �ּ� ���� �޴´�.
	}
}

CQuestLimitKind_Level::~CQuestLimitKind_Level()							// �Ҹ��� �Լ�.
{
}

// ����Ʈ ����Ʈ üũ�Լ�.(����)
BOOL CQuestLimitKind_Level::CheckLimit( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest )
{
#ifdef _MAPSERVER_														// �ʼ����� ����Ǿ� �ִٸ�,

	switch( m_dwLimitKind )												// ����Ʈ ������ Ȯ���Ѵ�.
	{
	case eQuestLimitKind_Level:											// ����Ʈ ������ ������ ���ٸ�,
		{
			DWORD dwlevel = pPlayer->GetLevel();						// �÷��̾��� ������ �޴´�.

			if( dwlevel >= m_dwMin && dwlevel <= m_dwMax )				// �ּҷ��� �̻�, �ִ� ���� ���ϸ�,
			{
				return TRUE;											// TRUE�� �����Ѵ�.
			}
		}
		break;

	case eQuestLimitKind_Money:											// ����Ʈ ������ �ӴϿ� ���ٸ�,
		{
			MONEYTYPE Money = pPlayer->GetMoney();						// �÷��̾��� �Ӵϸ� �޴´�.

			if( Money >= m_dwMax )										// �÷��̾��� �Ӵϰ� �ִ� �ӴϺ��� ũ�ٸ�,
			{
				return TRUE;											// TRUE�� �����Ѵ�.
			}
		}
		break;

	case eQuestLimitKind_Stage:											// ����Ʈ ������ ���������� ���ٸ�,
		{
			BYTE bStage = pPlayer->GetStage();							// �÷��̾��� ���������� �޴´�. ( ��ܰ�, ���� �����ΰ�?)

			if( bStage == (BYTE)m_dwMin || bStage == (BYTE)m_dwMax )	// �ּ� �ܰ� �̻�, �ִ� �ܰ� ���ϸ�,
			{
				return TRUE;											// TRUE�� �����Ѵ�.
			}
		}
		break;
	}
#endif

	return FALSE;														// FALSE�� �����Ѵ�.
}

// ����Ʈ ����Ʈ üũ�Լ�.(Ŭ���̾�Ʈ)
BOOL CQuestLimitKind_Level::CheckLimit( DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
#ifndef _MAPSERVER_														// �ʼ����� ����Ǿ� ���� ������,

	switch( m_dwLimitKind )												// ����Ʈ ����Ʈ ������ Ȯ���Ѵ�.
	{
	case eQuestLimitKind_Level:											// ����Ʈ ����Ʈ ������ ���� �����̶��,
		{
			DWORD dwlevel = HERO->GetLevel();							// HERO�� ������ �޴´�.

			if( dwlevel >= m_dwMin && dwlevel <= m_dwMax )				// HERO�� ������ �ּ� ���� �̻�, �ִ� ���� ���ϸ�,
			{
				return TRUE;											// TRUE�� �����Ѵ�.
			}
		}
		break;

	case eQuestLimitKind_Money:											// ����Ʈ ����Ʈ ������ �Ӵ϶��,
		{
			MONEYTYPE Money = HERO->GetMoney();							// HERO�� �Ӵϸ� �޴´�.

			if( Money >= m_dwMax )										// HERO�� �Ӵϰ� �ִ� �Ӵ� �̻��̸�,
			{
				return TRUE;											// TRUE�� �����Ѵ�.
			}
		}
		break;

	case eQuestLimitKind_Stage:											// ����Ʈ ����Ʈ ������ ���������� ���ٸ�,
		{
			BYTE bStage = HERO->GetStage();								// HERO�� ���������� �޴´�.(��ܰ�, ������ΰ�?)

			if( bStage == (BYTE)m_dwMin || bStage == (BYTE)m_dwMax )	// �ּ� �ܰ� �̻�, �ִ� �ܰ� ���϶��,
			{
				return TRUE;											// TRUE�� �����Ѵ�.
			}
		}
		break;
	}
#endif

	return FALSE;														// FALSE�� �����Ѵ�.
}
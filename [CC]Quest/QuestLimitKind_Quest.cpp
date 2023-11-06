// QuestLimitKind_Quest.cpp: implementation of the CQuestLimitKind_Quest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"														// ǥ�� �ý��۰�, ������Ʈ�� ������, ���־��� �ش����� ���� �ش������� �ҷ��´�.
#include "QuestLimitKind_Quest.h"										// ����Ʈ ����Ʈ ����Ʈ ����� �ҷ��´�.

#include "QuestScriptLoader.h"											// ����Ʈ ����Ʈ ��ũ��Ʈ �δ� ����� �ҷ��´�.
#include "Quest.h"														// ����Ʈ ����� �ҷ��´�.

#ifdef _MAPSERVER_														// �� ������ ����Ǿ� �ִٸ�,

#include "QuestGroup.h"													// ����Ʈ �׷� ����� �ҷ��´�.

#else																	// �� ������ ����Ǿ� ���� �ʴٸ�,

#include "QuestManager.h"												// ����Ʈ �Ŵ��� ����� �ҷ��´�.

#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// ������ �Լ�.
CQuestLimitKind_Quest::CQuestLimitKind_Quest( DWORD dwLimitKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
: CQuestLimitKind( dwLimitKind, pTokens, dwQuestIdx, dwSubQuestIdx )
{
	GetScriptParam( m_dwIdx, pTokens );									// ����Ʈ �ε����� �޴´�.
}

// �Ҹ��� �Լ�.
CQuestLimitKind_Quest::~CQuestLimitKind_Quest()							
{
}

// ����Ʈ ����Ʈ üũ�Լ�.(����)
BOOL CQuestLimitKind_Quest::CheckLimit( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest )
{
	// 071011 LYW --- QuestLimitKind_Quest : Check parameter info.
	if( !pPlayer || !pQuestGroup || !pQuest ) return FALSE ;			// ���ڷ� �Ѿ�� ������ ��ȿ���� ������, FALSE ���� ó���� �Ѵ�.

#ifdef _MAPSERVER_														// �ʼ����� ����Ǿ� �ִٸ�,

	switch( m_dwLimitKind )												// ����Ʈ ����Ʈ ������ Ȯ���Ѵ�.
	{
	case eQuestLimitKind_Quest:											// ����Ʈ ������ ����Ʈ���,
		{
			if( pQuestGroup->GetQuest( m_dwIdx )->IsQuestComplete() )	// ����Ʈ �ε����� �Ϸ��� ����Ʈ���� üũ�Ѵ�.
			{
				return TRUE;											// TRUE�� �����Ѵ�.
			}
		}
		break;

	case eQuestLimitKind_SubQuest:										// ����Ʈ ������ ���� ����Ʈ���,
		{
			if( pQuest->IsSubQuestComplete( m_dwIdx ) )					// ����Ʈ �ε����� �Ϸ��� ���� ����Ʈ ���� üũ�Ѵ�.
			{
				return TRUE;											// TRUE�� �����Ѵ�.
			}
		}
		break;

	// 071011 LYW --- QuestLimitKind_Quest : Add quest limit to check running quest.
	case eQuestLimitKind_RunningQuest :									// �������� ����Ʈ�� üũ�ϴ� ����Ʈ���,
		{
			CQuest* pQuest = NULL ;										// ����Ʈ ������ ���� �����͸� �����Ѵ�.
			pQuest = pQuestGroup->GetQuest( m_dwIdx ) ;					// ����Ʈ ������ �޴´�.

			if( !pQuest ) return FALSE ;								// ����Ʈ ������ ��ȿ���� ������, false ���� ó���� �Ѵ�.

			if( pQuest->GetSubQuestFlag() == 0 ) return TRUE ;			// ����Ʈ�� ���� ����Ʈ �÷��װ� 0�� �ƴϸ�, true ���� ó���� �Ѵ�.
		}
		break ;
	}
#endif

	return FALSE;														// FALSE�� �����Ѵ�.
}

// ����Ʈ ����Ʈ üũ�Լ�.(Ŭ���̾�Ʈ)
BOOL CQuestLimitKind_Quest::CheckLimit( DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
#ifndef _MAPSERVER_														// �� ������ ������� �ʾҴٸ�,

	switch( m_dwLimitKind )												// ����Ʈ ����Ʈ ������ Ȯ���Ѵ�.
	{
	case eQuestLimitKind_Quest:											// ����Ʈ ������ ����Ʈ���,
		{
			if( QUESTMGR->GetQuest( m_dwIdx )->IsQuestComplete() )		// ����Ʈ �ε����� �Ϸ��� ����Ʈ ���� üũ�Ѵ�.
			{
				return TRUE;											// TRUE�� �����Ѵ�.
			}
		}
		break;

	case eQuestLimitKind_SubQuest:										// ����Ʈ ������ ���� ����Ʈ���,
		{
			if( QUESTMGR->GetQuest( dwQuestIdx )->IsSubQuestComplete( m_dwIdx ) )	// ����Ʈ �ε����� �Ϸ��� ��������Ʈ ���� üũ�Ѵ�.
			{
				return TRUE;											// TRUE�� �����Ѵ�.
			}
		}
		break;
	// 071011 LYW --- QuestLimitKind_Quest : Add quest limit to check running quest.
	case eQuestLimitKind_RunningQuest :									// �������� ����Ʈ�� üũ�ϴ� ����Ʈ���,
		{
			CQuest* pQuest = NULL ;										// ����Ʈ ������ ���� �����͸� �����Ѵ�.
			pQuest = QUESTMGR->GetQuest( m_dwIdx ) ;					// ����Ʈ ������ �޴´�.

			if( !pQuest ) return FALSE ;								// ����Ʈ ������ ��ȿ���� ������, false ���� ó���� �Ѵ�.

			if( pQuest->GetSubQuestFlag() == 0 ) return TRUE ;			// ����Ʈ�� ���� ����Ʈ �÷��װ� 0�� �ƴϸ�, true ���� ó���� �Ѵ�.
		}
		break ;
	}
#endif

	return FALSE;														// FALSE�� �����Ѵ�.
}

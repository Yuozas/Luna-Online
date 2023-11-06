// QuestExecute_Count.cpp: implementation of the CQuestExecute_Count class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"															// ǥ�� �ý��۰�, ������Ʈ�� ������, ���־��� �ش����� ���� �ش������� �ҷ��´�.
#include "QuestExecute_Count.h"												// ����Ʈ ���� ī��Ʈ ����� �ҷ��´�.

#include "QuestScriptLoader.h"												// ����Ʈ ��ũ��Ʈ �δ� ����� �ҷ��´�.

#ifdef _MAPSERVER_															// �ʼ����� ����Ǿ� �ִٸ�,

#include "QuestGroup.h"														// ����Ʈ �׷� ����� �ҷ��´�.

#include "Player.h"															// �÷��̾� ����� �ҷ��´�.

#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// ������ �Լ�.
CQuestExecute_Count::CQuestExecute_Count( DWORD dwExecuteKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
: CQuestExecute( dwExecuteKind, pTokens, dwQuestIdx, dwSubQuestIdx )
{
	m_dwMaxCount = m_dwWeaponKind = m_dwParam1 = 0;							// �ִ� ī��Ʈ, ���� ����, �Ķ���� 1�� 0���� �����Ѵ�.

	GetScriptParam( m_dwRealSubQuestIdx, pTokens );							// ���� ���� ����Ʈ �ε����� �޴´�.
	GetScriptParam( m_dwMaxCount, pTokens );								// �ִ� ī��Ʈ�� �޴´�.

	if( m_dwExecuteKind == eQuestExecute_AddCountFQW						// ���� Ÿ���� Ư�� ����� ī��Ʈ�� �߰��ϴ� ���̶��,
		|| m_dwExecuteKind == eQuestExecute_AddCountFW )
	{
		GetScriptParam( m_dwWeaponKind, pTokens );							// ���� Ÿ���� �޴´�.
	}

	if( m_dwExecuteKind == eQuestExecute_LevelGap ||						// ���� Ÿ���� �������� ���,
		m_dwExecuteKind == eQuestExecute_MonLevel )
	{
		GetScriptParam( m_dwWeaponKind, pTokens );							// ���� Ÿ���� �޴´�.
		GetScriptParam( m_dwParam1, pTokens );								// �Ķ���� 1�� �޴´�.
	}
}

// �Ҹ��� �Լ�.
CQuestExecute_Count::~CQuestExecute_Count()									
{
}

// ���� �Լ�.
BOOL CQuestExecute_Count::Execute( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest )
{
#ifdef _MAPSERVER_															// �� ������ ����Ǿ� �ִٸ�,

	switch( m_dwExecuteKind )												// ���� Ÿ���� Ȯ���Ѵ�.
	{
	case eQuestExecute_AddCount:											// ī��Ʈ �߰����,
		{
			// ����Ʈ �׷��� ī��Ʈ�� ���� ��Ű�� �Լ� ȣ��.
			pQuestGroup->AddCount( m_dwQuestIdx, m_dwRealSubQuestIdx, m_dwMaxCount );
		}
		break;

	case eQuestExecute_AddCountFQW:											// Ư�� ���� ī��Ʈ ���� ���,
		{
			// ����Ʈ �׷��� Ư�� ����� ���� ī��Ʈ�� ���� ��Ű�� �Լ� ȣ��.
			pQuestGroup->AddCountFromQWeapon( m_dwQuestIdx, m_dwRealSubQuestIdx, m_dwMaxCount, m_dwWeaponKind );
		}
		break;

	case eQuestExecute_AddCountFW:											// Ư�� ���� ī��Ʈ �������,
		{
			// ����Ʈ �׷��� Ư�� ����� ���� ī��Ʈ�� ���� ��Ű�� �Լ� ȣ��.
			pQuestGroup->AddCountFromWeapon( m_dwQuestIdx, m_dwRealSubQuestIdx, m_dwMaxCount, m_dwWeaponKind );
		}
		break;
	case eQuestExecute_LevelGap:											// ���� ���̶��,
		{
			// ����Ʈ �׷��� ���� ���̷� ���� ī��Ʈ�� ���� ��Ű�� �Լ� ȣ��.
			pQuestGroup->AddCountFromLevelGap( m_dwQuestIdx, m_dwRealSubQuestIdx, m_dwMaxCount, m_dwWeaponKind, m_dwParam1 );
		}
		break;
	case eQuestExecute_MonLevel:											// ���� ���� �̶��,
		{
			// ����Ʈ �׷��� ���� ������ ���� ī��Ʈ�� ���� ��Ű�� �Լ� ȣ��.
			pQuestGroup->AddCountFromMonLevel( m_dwQuestIdx, m_dwRealSubQuestIdx, m_dwMaxCount, m_dwWeaponKind, m_dwParam1 );
		}
		break;
	}
#endif

	return TRUE;															// TRUE�� �����Ѵ�.
}

int CQuestExecute_Count::CheckCondition( PLAYERTYPE* pPlayer,					// ����Ʈ �̺�Ʈ�� �����ϱ� ���� ������ �����ϴ��� üũ�ϴ� �Լ�.
							CQuestGroup* pQuestGroup, CQuest* pQuest )			
{
	int nErrorCode = e_EXC_ERROR_NO_ERROR ;										// ���� �ڵ带 ���� ������ �����ϰ� e_EXE_ERROR_NO_ERROR�� �����Ѵ�.

	if( !pPlayer )																// �÷��̾� ������ ��ȿ���� ������, 
	{
		nErrorCode = e_EXC_ERROR_NO_PLAYERINFO ;								// �÷��̾� ������ ���ٴ� ���� �ڵ带 �����Ѵ�.
		return nErrorCode ;														// ���� �ڵ带 return ó���� �Ѵ�.
	}

	if( !pQuestGroup )															// ����Ʈ �׷� ������ ��ȿ���� ������,
	{
		nErrorCode = e_EXC_ERROR_NO_QUESTGROUP ;								// ����Ʈ �׷� ������ ���ٴ� ���� �ڵ带 �����Ѵ�.
		return nErrorCode ;														// ���� �ڵ带 return ó���� �Ѵ�.
	}

	if( !pQuest )																// ����Ʈ ������ ��ȿ���� �ʴٸ�,
	{
		nErrorCode = e_EXC_ERROR_NO_QUEST ;										// ����Ʈ ������ ���ٴ� ���� �ڵ带 �����Ѵ�.
		return nErrorCode ;														// ���� �ڵ带 return ó���� �Ѵ�.
	}

#ifdef _MAPSERVER_																// �� ������ ����Ǿ� �ִٸ�,

	switch( m_dwExecuteKind )													// ���� Ÿ���� Ȯ���Ѵ�.
	{
	case eQuestExecute_AddCount:												// ī��Ʈ �߰����,
		{
			return nErrorCode ;													// ���� �ڵ带 return ó���� �Ѵ�.
		}
		break;

	case eQuestExecute_AddCountFQW:												// Ư�� ���⸦ üũ�ؼ� ����Ʈ �������� �޴� �����̸�,
		{
			if( pPlayer->GetWearedWeapon() != m_dwWeaponKind )					// �÷��̾ �����ϰ� �ִ� ����� �񱳴�� ���Ⱑ ���� ������,
			{
				nErrorCode = e_EXC_ERROR_NOT_SAME_WEAPONKIND_COUNT ;			// ���� ������ �޶� ����Ʈ ī��Ʈ �߰��� ���Ѵٴ� ���� �޽����� �����Ѵ�.
				return nErrorCode ;												// ���� �ڵ带 return ó���� �Ѵ�.
			}
		}
		break;

	case eQuestExecute_AddCountFW:												// ���� �迭�� üũ�ؼ� ����Ʈ �������� �޴� �����̸�,
		{
			return nErrorCode ;													// ���� �ڵ带 return ó���� �Ѵ�.

			// �� ó���� ������� �ʱ�� ����.
		}
		break;
	case eQuestExecute_LevelGap:												// ���� ���̶��,
		{
			return nErrorCode ;													// ���� �ڵ带 return ó���� �Ѵ�.
			/*int nMin = pPlayer->GetLevel() - pPlayer->GetKillMonsterLevel();
			int nMax = pPlayer->GetKillMonsterLevel() - pPlayer->GetLevel();
			if( nMin > 0 && nMin > (int)dwMinLevel )	return;
			if( nMax > 0 && nMax > (int)dwMaxLevel )	return;*/
		}
		break;
	case eQuestExecute_MonLevel:												// ���� ���� �̶��,
		{
			return nErrorCode ;													// ���� �ڵ带 return ó���� �Ѵ�.
			/*WORD wMonLevel = pPlayer->GetKillMonsterLevel();

			if( wMonLevel < dwMinLevel )	return;
			if( wMonLevel > dwMaxLevel )	return;*/
		}
		break;
	}

#endif //_MAPSERVER_

	return nErrorCode ;															// �⺻ ���� �����Ѵ�.
}

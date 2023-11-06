// QuestMapMgr.cpp: implementation of the CQuestMapMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestMapMgr.h"
#include "Player.h"
#include "RecallManager.h"
#include "ChannelSystem.h"
#include "QuestManager.h"
// desc_hseos_����Ʈ ��_01
// S ����Ʈ �� �߰� added by hseos 2007.11.25	2007.11.28
#include "../hseos/Date/SHDateManager.h"
#include "UserTable.h"
// E ����Ʈ �� �߰� added by hseos 2007.11.25	2007.11.28

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CQuestMapMgr);

CQuestMapMgr::CQuestMapMgr()
{
	m_bQuestMap = FALSE;
}

CQuestMapMgr::~CQuestMapMgr()
{
}

BOOL CQuestMapMgr::Init( WORD wMapNum )
{
	// desc_hseos_����Ʈ ��_01
	// S ����Ʈ �� �߰� added by hseos 2007.11.25
	// ..ç���� ���� ä��ó���� ����Ʈ ���� ä��ó���� ���
	// if( QUESTMAPNUM1 == wMapNum || QUESTMAPNUM2 == wMapNum )
	if( QUESTMAPNUM1 == wMapNum || QUESTMAPNUM2 == wMapNum || g_csDateManager.IsChallengeZone(wMapNum))
	// E ����Ʈ �� �߰� added by hseos 2007.11.25
	{
		m_bQuestMap = TRUE;
		CHANNELSYSTEM->InitQuestMapChannel();
	}
	else
		m_bQuestMap = FALSE;

	return m_bQuestMap;
}

void CQuestMapMgr::RemovePlayer( CPlayer* pPlayer )
{
	if( !m_bQuestMap )	return;

	// desc_hseos_����Ʈ ��_01
	// S ����Ʈ �� �߰� added by hseos 2007.11.28
	// ..ç���� ���� ��쿡�� 2���� �÷��̾� �� ���߿� ������ �÷��̾��� ��쿡 ä���� �����.
	// RECALLMGR->DeleteQuestRecallMonster( pPlayer->GetChannelID() );
	// CHANNELSYSTEM->DestroyQuestMapChannel( pPlayer->GetChannelID() );
	if (g_csDateManager.IsChallengeZoneHere())
	{
		// �⺻������ ���ʹ� ����. �� ç���� �����߿� 1���̶� �ʰ� ������ ����� ���ʹ� �������.
		RECALLMGR->DeleteChallengeZoneRecallMonster( pPlayer->GetChannelID() );

		// ��Ʈ�ʰ� ���Դ�.
		if (pPlayer->GetChallengeZonePartnerID())
		{
			// ���� ���� ������ �ٽ� Ȯ��
			CPlayer* pTargetPlayer = (CPlayer*)g_pUserTable->FindUser(pPlayer->GetChallengeZonePartnerID());
			if(pTargetPlayer)
			{
				// ��Ʈ�ʰ� ���� ���̱⿡ ä���� �������� �ʰ� ��Ʈ��ID �� �ʱ�ȭ�Ѵ�.
				// ��Ʈ�ʰ� ������ ���� �� �� �Լ��� ����Ǹ鼭 ä�� ���� ó���� �� ���̴�..
				pTargetPlayer->SetChallengeZonePartnerID(0);
			}
			else
			{
				// ��Ʈ�ʰ� �������� �и��ѵ� ������ ����?? �������̶�� ����� ������ �� �ȴ�..
				// ��·�� ��Ʈ�ʰ� ���� ���°� �ڽŵ� ������ ��Ȳ�̴� ä�� ����
				CHANNELSYSTEM->DestroyQuestMapChannel( pPlayer->GetChannelID() );
			}
		}
		// ��Ʈ�ʰ� �� ���԰ų�, ������.
		else
		{
			// ��Ʈ�ʰ� �����ٸ� ä���� �����.
			// ��Ʈ�ʰ� �� ������ ����, ��Ʈ�ʰ� ���� �� ���� ���� �ֱ� ������ ä���� �����.
			// ���� �� ������ �ʰ� ���� ��쿡�� ä���� ���� �Ҵ�޵��� ó���Ǿ� �ִ�.
			// ���� �ʰ� ���� ��Ʈ�ʴ� ç������ ���۵��� �ʴ´�. �׳� �� ���游..��
			CHANNELSYSTEM->DestroyQuestMapChannel( pPlayer->GetChannelID() );
		}
	}
	else
	{
		RECALLMGR->DeleteQuestRecallMonster( pPlayer->GetChannelID() );
		CHANNELSYSTEM->DestroyQuestMapChannel( pPlayer->GetChannelID() );
	}
	// E ����Ʈ �� �߰� added by hseos 2007.11.28
}

void CQuestMapMgr::DiePlayer( CPlayer* pPlayer )
{
	if( !m_bQuestMap )	return;

	pPlayer->SetReadyToRevive( FALSE );
//	QUESTMGR->AddQuestEvent( pPlayer, &CQuestEvent( eQuestEvent_Die, g_pServerSystem->GetMapNum(), 0 ) );
}
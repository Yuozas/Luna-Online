// PKManager.cpp: implementation of the CPKManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PKManager.h"
#include "UserTable.h"
#include "PackedData.h"
#include "Player.h"
#include "LootingManager.h"
#include "MapDBMsgParser.h"
#include "ItemManager.h"
#include "SkillTreeManager.h"

#include "..\[CC]Header\GameResourceManager.h"
#include "FameManager.h"
#include "GuildFieldWarMgr.h"
#include "EventMapMgr.h"
#include "PartyWarMgr.h"
#include "Battle.h"
#include "QuestMapMgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPKManager::CPKManager()
{
}

CPKManager::~CPKManager()
{

}

void CPKManager::Init( BOOL bPKAllow )
{
	m_bPKAllow = bPKAllow;

//---KES CHEAT PKEVENT
	m_bPKEvent = FALSE;
//-------------------
}

void CPKManager::SetPKAllowWithMsg( BOOL bAllow )
{
	if( m_bPKAllow == bAllow ) return;
	m_bPKAllow = bAllow;
	
	MSG_BYTE msg;
	msg.Category	= MP_PK;
	msg.Protocol	= MP_PK_PKALLOW_CHANGED;
	
	CObject* pObject = NULL;
	g_pUserTable->SetPositionUserHead();
	while( pObject = g_pUserTable->GetUserData() )
	{
		if( pObject->GetObjectKind() != eObjectKind_Player ) continue;

		if( bAllow == FALSE )
		{
			if( ((CPlayer*)pObject)->IsPKMode() )
				((CPlayer*)pObject)->PKModeOffForce();
			//루팅하던것도 캔슬?
		}

		msg.bData		= bAllow;
						
		((CPlayer*)pObject)->SendMsg( &msg, sizeof( msg ) );
	}
}

void CPKManager::DiePanelty( CPlayer* pDiePlayer, CObject* pAttacker )
{
	// questmap
	if( QUESTMAPMGR->IsQuestMap() )
		return;
	// eventmap
	if( EVENTMAPMGR->IsEventMap() )
		return;
	// guildfieldwar
//	if( pDiePlayer->IsDieFromGFW() )
//		return;

	// partywar
	//if( PARTYWARMGR->IsEnemy( pDiePlayer, (CPlayer*)pAttacker ) )
	//	return;
	//비무시엔 죽는 패널티가 없다. 비무로 정당하게 죽었다.
	if( pDiePlayer->m_bNeedRevive == FALSE )
		return;
	// RaMa - 05.05.20 문파전시에 죽어도 패널티 없음
	if( pDiePlayer->GetBattle()->GetBattleKind() == eBATTLE_KIND_GTOURNAMENT )
		return;

	//악명치 높을때 죽으면 받는 패널티.
	if( pDiePlayer->IsPKMode() )
	{
		pDiePlayer->SetDieForGFW( FALSE );	// PK모드이면 문파필드전으로 죽은게 아니다
		PKModeDiePanelty( pDiePlayer );
		PKModeTimeReduceByDie( pDiePlayer );
	}
	else
	{
		NoPKModeDiePanelty( pDiePlayer );
	}

	if( pAttacker->GetObjectKind() != eObjectKind_Player )
		return;

	//PK하면 받는 악명치
	PKPlayerKillPanelty( pDiePlayer, (CPlayer*)pAttacker );

	//척살당하면 받는 패널티.
	//PKPlayerKiiledByWantedPanelty( pDiePlayer, (CPlayer*)pAttacker );	//---KES PK 071202 아직 적용 안되었다
}


void CPKManager::PKPlayerKillPanelty( CPlayer* pDiePlayer, CPlayer* pAttacker )
{
	if( pAttacker->IsPKMode() )
	{
		if( !pDiePlayer->IsPKMode() )
		{
			//---KES PK 071124
			int AddBadFame = pAttacker->GetLevel() < pDiePlayer->GetLevel() ?
						  pDiePlayer->GetLevel() - pAttacker->GetLevel() + 15 :
						  ( pAttacker->GetLevel() - pDiePlayer->GetLevel() ) * 2 + 15;
			
			pAttacker->SetBadFame( pAttacker->GetBadFame() + AddBadFame );
			//----------------
			
			BadFameCharacterUpdate( pAttacker->GetID(), pAttacker->GetBadFame() );
			FAMEMGR->SendBadFameMsg( pAttacker, pAttacker->GetBadFame() );

			//추가
			pAttacker->SetPKContinueTime( pAttacker->GetPKContinueTime() + 10*60*1000 );	//살기 지속시간 증가 10분	//---KES PK 071124

			//MSG보내기
		}
	}
}

void CPKManager::PKPlayerKiiledByWantedPanelty( CPlayer* pDiePlayer, CPlayer* pAttacker )
{
	//die <--PKPlayer
	//attack <--has wanted player


	return;	//---KES PK 미적용


	if( pAttacker->IsWantedOwner( pDiePlayer->GetWantedIdx() ) )	//척살권을 가진상대에게 죽었다.
	{
//완화 : 악명치 감소 ========
		DWORD dwBadFame = pDiePlayer->GetBadFame();

		if( dwBadFame != 0 )
		{
			DWORD dwMinusBadFame = 0;
			
			if( dwBadFame < 50 )
				dwMinusBadFame = 2;
			else if( dwBadFame < 4000 )
				dwMinusBadFame = 5;
			else if( dwBadFame < 20000 )
				dwMinusBadFame = 10;
			else if( dwBadFame < 80000 )
				dwMinusBadFame = 30;
			else if( dwBadFame < 400000 )
				dwMinusBadFame = 50;
			else if( dwBadFame < 1600000 )
				dwMinusBadFame = 80;
			else if( dwBadFame < 8000000 )
				dwMinusBadFame = 100;
			else if( dwBadFame < 32000000 )
				dwMinusBadFame = 200;
			else if( dwBadFame < 100000000 )
				dwMinusBadFame = 300;
			else if( dwBadFame < 500000000 )
				dwMinusBadFame = 500;
			else
				dwMinusBadFame = 600;
			
			if( dwBadFame > dwMinusBadFame )
				pDiePlayer->SetBadFame( dwBadFame - dwMinusBadFame );
			else
				pDiePlayer->SetBadFame( 0 );	//0
			BadFameCharacterUpdate( pDiePlayer->GetID(), pDiePlayer->GetBadFame() );
			FAMEMGR->SendBadFameMsg( pDiePlayer, pDiePlayer->GetBadFame() );
		}
	}
}


//---KES PK 071202
void CPKManager::PKModeDiePanelty( CPlayer* pPlayer )	//pk모드 일때
{
	if( !pPlayer ) return;
// 기획내용(071201)
// 감소되는 경험치 = 3 * (INT(사망자 PK 수치 / 75) + 2), MAX. = 50	//--- 기획수정 5* 를 3* 으로 2008.1.4
// 감소되는 PK 모드 리미트 타임 = 10분 / 회

	//---중요! 경험치 보호 주문서 사용시 경험치 보호 된다!
	//---경험치 보호 주문서 처리 추가 해야함.

	DWORD dwBadFame = pPlayer->GetBadFame();

	if( dwBadFame > 0 )
	{
		EXPTYPE pointForLevel	= GAMERESRCMNGR->GetMaxExpPoint( pPlayer->GetLevel() );

		float fPenaltyRate		= (float)( 3 * ( ( dwBadFame / 75 ) + 2 ) ) / 100.f;
		if( fPenaltyRate > .50f ) fPenaltyRate = .50f;	//MAX 50%
	
		EXPTYPE dwPenaltyExp	= (float)pointForLevel * fPenaltyRate;

		if( dwPenaltyExp )
		{
			pPlayer->ReduceExpPoint( dwPenaltyExp , eExpLog_LoseByPKMode );

			// 080602 LYW --- Player : 경험치 수치 (__int32) 에서 (__int64) 사이즈로 변경 처리.
			//MSG_DWORD2 msg;
			//msg.Category	= MP_PK;
			//msg.Protocol	= MP_PK_EXPPENALTY;
			//msg.dwData1		= dwPenaltyExp;
			//msg.dwData2		= 1;	//이유 -  PK모드 킨상태
			//pPlayer->SendMsg( &msg, sizeof(msg) );

			MSG_DWORDEX2 msg;
			msg.Category	= MP_PK;
			msg.Protocol	= MP_PK_EXPPENALTY;
			msg.dweData1	= dwPenaltyExp;
			msg.dweData2	= 1;	//이유 -  PK모드 킨상태
			pPlayer->SendMsg( &msg, sizeof(msg) );
		}
	}

	//---아이템 파괴확률은 없다.
}

//---KES PK 071202
void CPKManager::PKModeTimeReduceByDie( CPlayer* pPlayer )
{
	//---죽음에 대한 PK모드 지속시간 감소
	DWORD dwPKContinueTime = pPlayer->GetPKContinueTime();
	if( dwPKContinueTime == 0 ) return;

    if( dwPKContinueTime > 10*60*1000 )
		dwPKContinueTime -= 10*60*1000;
	else
		dwPKContinueTime = 0;

	pPlayer->SetPKContinueTime( dwPKContinueTime );	
	//-----------------------------------
}

//---KES PK 071202
void CPKManager::NoPKModeDiePanelty( CPlayer* pPlayer ) //PKMode 아닐때
{
	if( !pPlayer ) return;

	//---중요! 경험치 보호 주문서 사용시 경험치 보호 된다!
	//---경험치 보호 주문서 처리 추가 해야함.

	//사망 시 감소되는 추가 경험치 = 2 * (INT(사망자 PK 수치 / 150) + 1), MAX. = 25		//---5* 를 2*로 기획수정 2008.1.4
	DWORD dwBadFame = pPlayer->GetBadFame();


	if( dwBadFame > 0 )
	{
		EXPTYPE pointForLevel	= GAMERESRCMNGR->GetMaxExpPoint( pPlayer->GetLevel() );

		float fPenaltyRate		= (float)( 1 * ( dwBadFame / 150 ) ) / 100.f; //(float)( 5 * ( ( dwBadFame / 150 ) ) ) / 100.f; <--합의하에 1뺌		PK 0227
		if( fPenaltyRate > .25f ) fPenaltyRate = .25f;	//MAX 25%
	
		EXPTYPE dwPenaltyExp	= (float)pointForLevel * fPenaltyRate;

		if( dwPenaltyExp )
		{
			pPlayer->ReduceExpPoint( dwPenaltyExp , eExpLog_LosebyBadFame );

			// 080602 LYW --- Player : 경험치 수치 (__int32) 에서 (__int64) 사이즈로 변경 처리.
			//MSG_DWORD2 msg;
			//msg.Category	= MP_PK;
			//msg.Protocol	= MP_PK_EXPPENALTY;
			//msg.dwData1		= dwPenaltyExp;
			//msg.dwData2		= 0; //이유	- PK모드 안킨 상태
			//pPlayer->SendMsg( &msg, sizeof(msg) );

			MSG_DWORDEX2 msg;
			msg.Category	= MP_PK;
			msg.Protocol	= MP_PK_EXPPENALTY;
			msg.dweData1	= dwPenaltyExp;
			msg.dweData2	= 0; //이유	- PK모드 안킨 상태
			pPlayer->SendMsg( &msg, sizeof(msg) );
		}
	}
}

//---KES PK 071202
void CPKManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch( Protocol )
	{
	case MP_PK_PKON_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			CPlayer* pPlayer	= (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if( !pPlayer ) break;

//---KES PKEVENT
			if( !IsPKAllow() )
			{
				if( pPlayer->GetUserLevel() <= eUSERLEVEL_GM )	//GM계정 이상이면
				{
					if( !IsPKEvent() )
					{
						MSG_BYTE msg;
						msg.Category	= MP_PK;
						msg.Protocol	= MP_PK_PKON_NACK;
						msg.bData		= ePKCODE_NOTALLAW;
						pPlayer->SendMsg( &msg, sizeof(msg) );
						break;
					}
				}
				else
				{
					MSG_BYTE msg;				//error code?
					msg.Category	= MP_PK;
					msg.Protocol	= MP_PK_PKON_NACK;
					msg.bData		= ePKCODE_NOTALLAW;
					pPlayer->SendMsg( &msg, sizeof(msg) );
					break;
				}
			}
//--------------

			int rt;
			if( ( rt = pPlayer->PKModeOn() ) == ePKCODE_OK )
			{
				MSG_DWORD msg;
				msg.Category	= MP_PK;
				msg.Protocol	= MP_PK_PKON_ACK;
				msg.dwData		= pmsg->dwObjectID;
				msg.dwObjectID	= pPlayer->GetID();

				PACKEDDATA_OBJ->QuickSend( pPlayer, &msg, sizeof(msg) );
			}
			else
			{
				MSG_BYTE msg;
				msg.Category	= MP_PK;
				msg.Protocol	= MP_PK_PKON_NACK;
				msg.bData		= rt;
				pPlayer->SendMsg( &msg, sizeof(msg) );
			}
		}
		break;

	case MP_PK_PKOFF_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			CPlayer* pPlayer	= (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if( !pPlayer ) break;

			if( pPlayer->PKModeOff() )
			{
				MSG_DWORD msg;
				msg.Category	= MP_PK;
				msg.Protocol	= MP_PK_PKOFF_ACK;
				msg.dwData		= pmsg->dwObjectID;
				msg.dwObjectID	= pPlayer->GetID();

				PACKEDDATA_OBJ->QuickSend( pPlayer, &msg, sizeof(msg) );
			}
			else
			{
				MSGBASE msg;
				msg.Category	= MP_PK;
				msg.Protocol	= MP_PK_PKOFF_NACK;
				pPlayer->SendMsg( &msg, sizeof(msg) );
			}
		}
		break;
		
	case MP_PK_LOOTING_SELECT_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CLootingRoom* pRoom = LOOTINGMGR->GetLootingRoom( pmsg->dwData1 );
			if( pRoom )
			{
				LOOTINGMGR->Loot( pmsg->dwData1, pmsg->dwData2 );
				
			}
			else
			{
				CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
				if( pPlayer )
				{
					MSG_DWORD2 msg;
					msg.Category	= MP_PK;
					msg.Protocol	= MP_PK_LOOTING_ERROR;
					msg.dwData1		= pmsg->dwData1;
					msg.dwData2		= eLOOTINGERROR_NO_LOOTINGROOM;
					pPlayer->SendMsg( &msg, sizeof(msg) );
				}
			}
		}
		break;
	
	case MP_PK_LOOTING_ENDLOOTING:
		{
			MSG_DWORD*	pmsg = (MSG_DWORD*)pMsg;
			LOOTINGMGR->CloseLootingRoom( pmsg->dwData, FALSE );
		}
	}	
}

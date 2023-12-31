// Battle.cpp: implementation of the CBattle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Battle.h"

#include "Player.h"
#include "BattleTeam.h"
#include "BattleObject.h"
#include "WantedManager.h"
#include "PartyManager.h"
#include "Party.h"

#include "PKManager.h"

#ifdef _MAPSERVER_
#include "Monster.h"
#include "pet.h"
#include "ItemDrop.h"
#include "ItemManager.h"
#include "../[cc]skill/server/manager/skillmanager.h"
#include "ChannelSystem.h"
#include "..\[CC]Quest\QuestEvent.h"
#include "QuestManager.h"
#include "EventMapMgr.h"
#include "GuildFieldWarMgr.h"
#include "PartyWarMgr.h"
#else
#include "ObjectManager.h"
#include "ChatManager.h"
#include "EventMapInfo.h"
#include "GuildFieldWar.h"
#include "PartyWar.h"
#include "ItemManager.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBattle::CBattle()
{
	m_BattleFlag = BATTLE_FLAG_NONE;
	m_bDestroyFlag = FALSE;
	
	ZeroMemory( m_TeamArray, sizeof(m_TeamArray) );
}

CBattle::~CBattle()
{
}
void CBattle::Initialize(BATTLE_INFO_BASE* pCreateInfo, CBattleTeam* pTeam1, CBattleTeam* pTeam2)
{	
	OnCreate(pCreateInfo,pTeam1,pTeam2);
	
	m_BattleInfo = *pCreateInfo;
	m_Team1 = pTeam1;
	m_Team2 = pTeam2;
	m_TeamObserver = NULL;

	m_bDestroyFlag = FALSE;

	m_WinnerTeam = eBattleTeam_Max;
	m_BattleFlag = BATTLE_FLAG_NONE;
	
}

void CBattle::StartBattle()
{
	OnFightStart();

#ifdef _MAPSERVER_

	MSG_DWORD msg;
	msg.Category = MP_BATTLE;
	msg.Protocol = MP_BATTLE_START_NOTIFY;
	msg.dwData = GetBattleID();

	for(int n=0;n<eBattleTeam_Max;++n)
	{
		if( m_TeamArray[n] )
			m_TeamArray[n]->SendTeamMsg(&msg,sizeof(msg));
	}

#endif
}


BOOL CBattle::IsEnemy(CObject* pOperator,CObject* pTarget)
{
	if(pOperator->GetObjectKind() & eObjectKind_Monster)
	{	// Operator 가 몬스터일경우
		if(pTarget->GetObjectKind() & eObjectKind_Monster)	// 몬스터는 적이 아님
			return FALSE;
#ifdef _MAPSERVER_
		if( pTarget->GetState() == eObjectState_Immortal )	//무적상태는 적이 아니다.
			return FALSE;
#endif
		
	}
	else if(pOperator->GetObjectKind() == eObjectKind_Player)
	{	// Operator 가 플레이어일경우
		if( pTarget->GetObjectKind() & eObjectKind_Monster )	// 몬스터는 적이다
		{
			return TRUE;
		}
		else if( pTarget->GetObjectKind() == eObjectKind_Player )
		{
#ifdef _MAPSERVER_
			if( pTarget->GetState() == eObjectState_Immortal )	//무적상태는 적이 아니다.
				return FALSE;

			if( ((CPlayer*)pOperator)->GetPartyIdx() != 0 )
			{
				CParty* pParty = PARTYMGR->GetParty( ((CPlayer*)pOperator)->GetPartyIdx() );
				if( pParty )
				if( pParty->IsPartyMember( pTarget->GetID() ) )
					return FALSE;	//파티원은 적이 아니다.

				// partywar
				//if( PARTYWARMGR->IsEnemy( (CPlayer*)pOperator, (CPlayer*)pTarget ) )
				//	return TRUE;
			}
			// Pk가 허용되거나 진추적주문서를 사용중이면
			if( PKMGR->IsPKAllow() )//|| ((CPlayer*)pOperator)->GetShopItemManager()->GetUsingItemInfo( eIncantation_Tracking_Jin ) )
			if( WANTEDMGR->IsOwner((CPlayer*)pOperator, ((CPlayer*)pTarget)->GetWantedIdx()) == TRUE )
				return TRUE;	//척살 대상자는 적이다.

			if( EVENTMAPMGR->IsEnemy( pOperator, pTarget, this->GetBattleID() ) )
				return TRUE;
			
			if( GUILDWARMGR->IsEnemyInField( (CPlayer*)pOperator, (CPlayer*)pTarget ) == TRUE )
				return TRUE;
#else
			if( pOperator == HERO )
			{
				if( PARTYMGR->IsPartyMember( pTarget->GetID() ) )
				{
					return FALSE;	//파티원은 적이 아니다.
				}
				if( PARTYWAR->IsEnemy( (CPlayer*)pTarget ) )
					return TRUE;

    //            // 진추적주문서를 사용중이면
				//if( PKMGR->IsPKAllow() || ITEMMGR->GetUsedItemInfo( eIncantation_Tracking_Jin ) )
				//if( WANTEDMGR->IsChrIDInList( pTarget->GetID() ) )
				//	return TRUE;	//척살 대상자는 적이다.

				if( EVENTMAP->IsEnemy( pTarget ) )
					return TRUE;

				if( GUILDWARMGR->IsEnemy( (CPlayer*)pTarget ) )
					return TRUE;
			}
#endif
//---KES CHEAT PKEVENT 아래 주석처리하여 허용지역 아니라도 이벤트 가능하도록
//			if( PKMGR->IsPKAllow() )
			{
				if( ((CPlayer*)pOperator)->IsPKMode() )				
				{
					if(pOperator == pTarget )
						return FALSE;
					else
						return TRUE;	//자신이 PK모드이면 자신을 뺀 나머지는 적이다.
				}
				if( ((CPlayer*)pTarget)->IsPKMode() )
					return TRUE;		//상대가 PK모드이면 적이다.
			}

			return FALSE;
		}
		else if(pTarget->GetObjectKind() == eObjectKind_SkillObject)
		{
			// YH2DO 같은편 벽은 안때려야 되는데... -_-a;
			cSkillObject* pSObj = (cSkillObject*)pTarget;
			return TRUE;
		}
		else if(pTarget->GetObjectKind() == eObjectKind_Pet )
		{
#ifdef _MAPSERVER_
			if( ( ( CPet* )pTarget )->GetMaster() )
			{
				return IsEnemy( pOperator, ( ( CPet* )pTarget )->GetMaster() );
			}
			else
				return FALSE;
#else
			if( ( ( CPet* )pTarget )->GetMaster() )
			{
				return IsEnemy( pOperator, ( ( CPet* )pTarget )->GetMaster() );
			}
			else
				return FALSE;
#endif
		}
		else	//나머지는 적이 아니다.
		{
			return FALSE;
		}
	}
	else if(pOperator->GetObjectKind() == eObjectKind_Pet )
	{
#ifdef _MAPSERVER_
		if( ( ( CPet* )pOperator )->GetMaster() )
		{
			return IsEnemy( ( ( CPet* )pOperator )->GetMaster(), pTarget );
		}
		else
			return FALSE;
#else
		if( ( ( CPet* )pOperator )->GetMaster() )
		{
			return IsEnemy( ( ( CPet* )pOperator )->GetMaster(), pTarget );
		}
		else
			return FALSE;
#endif
	}

	return TRUE;						// 나머진 모두 적
}
BOOL CBattle::IsFriend(CObject* pOperator,CObject* pTarget)
{
	if(pOperator->GetObjectKind() & eObjectKind_Monster)
	{	// Operator 가 몬스터일경우
		if((pTarget->GetObjectKind() & eObjectKind_Monster) == FALSE)		// 몬스터가 아니면 teammate가 아니다.
			return FALSE;
	}
	else if(pOperator->GetObjectKind() == eObjectKind_Player)
	{	// Operator 가 플레이어일경우
		DWORD PartyIdx = 0;
#ifdef _MAPSERVER_
		PartyIdx = ((CPlayer*)pOperator)->GetPartyIdx(); // 파티가 없으면 동료 없음
		if(PartyIdx == 0)
			return FALSE;
		if(((CPlayer*)pTarget)->GetPartyIdx() != PartyIdx)		// 파티가 다르면 FALSE
			return FALSE;
#endif
		if(pTarget->GetObjectKind() != eObjectKind_Player)		// 플레이어가 아니면 FALSE
			return FALSE;
	}

	return TRUE;						// 나머진 팀메이트
}

DWORD CBattle::GetBattleTeamID( CObject* pObject )
{
	for(int n=0; n<eBattleTeam_Max; ++n)
	{
		if( m_TeamArray[n] )
		if( m_TeamArray[n]->IsTeamMember( pObject ) )
			return n+1;
	}

	return 0;
}

#ifdef _MAPSERVER_
BOOL CBattle::AddObjectToBattle(CObject* pObject)
{
	for(int n=0;n<eBattleTeam_Max;++n)
	{
		if( m_TeamArray[n] )
		if( m_TeamArray[n]->IsAddableTeamMember(pObject))
		{
			pObject->SetBattle(GetBattleID(),n);

			BOOL rt = m_TeamArray[n]->AddTeamMember(pObject);
			ASSERT(rt);
			
			MSG_BATTLE_TEAMMEMBER_ADDDELETE msg;
			SetProtocol(&msg,MP_BATTLE,MP_BATTLE_TEAMMEMBER_ADD_NOTIFY);
			msg.Member.set(pObject->GetID(),pObject->GetObjectName());
			msg.Team = n;
			ToEachTeam(pTeam)
				pTeam->SendTeamMsg(&msg,sizeof(msg));
			EndToEachTeam

			OnTeamMemberAdd(n,pObject->GetID(),pObject->GetObjectName());

			if(pObject->GetObjectKind() == eObjectKind_Player)
				SendBattleInfo((CPlayer*)pObject);
			
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CBattle::IsAddableBattle(CObject* pObject)
{
	for(int n=0;n<eBattleTeam_Max;++n)
	{
		if(m_TeamArray[n])
		if(m_TeamArray[n]->IsAddableTeamMember(pObject))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CBattle::DeleteObjectFromBattle(CObject* pObject)
{
	ToEachTeam(pTeam)
		if(pTeam->DeleteTeamMember(pObject) == TRUE)
		{
			pObject->SetBattle(pObject->GetGridID(),0);
			
			if(OnTeamMemberDelete(TeamNumber,pObject->GetID(),pObject->GetObjectName()) == TRUE)
			{				
				MSG_BATTLE_TEAMMEMBER_ADDDELETE msg;
				SetProtocol(&msg,MP_BATTLE,MP_BATTLE_TEAMMEMBER_DELETE_NOTIFY);
				msg.Member.set(pObject->GetID(),pObject->GetObjectName());
				msg.Team = TeamNumber;
				ToEachTeam(pTeam)
					pTeam->SendTeamMsg(&msg,sizeof(msg));
				EndToEachTeam
			}			
			
			return TRUE;
		}
	EndToEachTeam

	return FALSE;
}

void CBattle::SendBattleInfo(CPlayer* pPlayer)
{
	static MSG_BATTLE_INFO msg;
	SetProtocol(&msg,MP_BATTLE,MP_BATTLE_INFO);
	GetBattleInfo(msg.info,&msg.msglen);	
	pPlayer->SendMsg(&msg,msg.GetMsgLength());
}


void CBattle::GetBattleInfo(char* pInfo,WORD* size)
{
	memcpy(pInfo,&m_BattleInfo,sizeof(BATTLE_INFO_BASE));
	*size = sizeof(BATTLE_INFO_BASE);
}

DWORD CBattle::GetTeamMemberNum(int i)
{
	if( m_TeamArray[i] ) 
		return m_TeamArray[i]->GetMemberNum();

	return 0;
}
#endif




// event func
void CBattle::OnCreate(BATTLE_INFO_BASE* pCreateInfo, CBattleTeam* pTeam1, CBattleTeam* pTeam2)
{
#ifdef _MAPSERVER_
	pCreateInfo->BattleTime = gCurTime;
#else
//	pCreateInfo->BattleTime = gCurTime - pCreateInfo->BattleTime;
	pCreateInfo->BattleTime = gCurTime;
#endif

	pCreateInfo->BattleState = eBATTLE_STATE_READY;
}
void CBattle::OnFightStart()
{
	m_BattleInfo.BattleTime = gCurTime;
	m_BattleInfo.BattleState = eBATTLE_STATE_FIGHT;
}
void CBattle::OnDestroy()
{
#ifdef _MAPSERVER_
	MSGBASE msg;
	SetProtocol(&msg,MP_BATTLE,MP_BATTLE_DESTROY_NOTIFY);
	ToEachTeam(pTeam)
		pTeam->SendTeamMsg(&msg,sizeof(msg));
		YHTPOSITION pos = pTeam->GetPositionHead();
		while(CObject* pObject = pTeam->GetNextTeamMember(&pos))
		{
			pTeam->DeleteTeamMember(pObject);
		}
	EndToEachTeam
#endif
}
void CBattle::OnTeamMemberAdd(int Team,DWORD MemberID,char* Name)
{
}
BOOL CBattle::OnTeamMemberDelete(int Team,DWORD MemberID,char* Name)
{
	// return TRUE를 하면 MP_BATTLE_TEAMMEMBER_DELETE_NOTIFY 메세지를 뿌리게 된다.
	return FALSE;
}
BOOL CBattle::OnTeamMemberDie(int Team,DWORD VictimMemberID,DWORD KillerID)
{
	return FALSE;
}
void CBattle::OnTeamObjectCreate(int Team,CBattleObject* pBattleObject)
{
}
void CBattle::OnTeamObjectDestroy(int Team,CBattleObject* pBattleObject)
{
}
void CBattle::OnTeamMemberRevive(int Team,CObject* pReviver)
{
}

void CBattle::OnMonsterDistribute(CMonster* pMonster,CObject* pKiller)
{
#ifdef _MAPSERVER_
	pMonster->Drop();
	
	if(pKiller->GetObjectKind() == eObjectKind_Player)
	{
		// jsd quest
		// 여기서 monsterkill event발생	
		CQuestEvent qe;
		qe.SetValue( eQuestEvent_Hunt, pMonster->m_MonsterInfo.MonsterKind, 0 );
		QUESTMGR->AddQuestEvent( (CPlayer*)pKiller, &qe );
		//
		((CPlayer*)pKiller)->SetKillMonsterLevel( pMonster->GetLevel() );
	}
	else if(pKiller->GetObjectKind() == eObjectKind_Pet)
	{
		CPlayer* pPlayer = (CPlayer*)(((CPet*)pKiller)->GetMaster());

		CQuestEvent qe;
		qe.SetValue( eQuestEvent_Hunt, pMonster->m_MonsterInfo.MonsterKind, 0 );
		QUESTMGR->AddQuestEvent( pPlayer, &qe );
		//
		pPlayer->SetKillMonsterLevel( pMonster->GetLevel() );
	}
#endif
}

void CBattle::OnTick()
{
	ySWITCH(m_BattleInfo.BattleState)
		yCASE(eBATTLE_STATE_READY)

		yCASE(eBATTLE_STATE_FIGHT)

		yCASE(eBATTLE_STATE_RESULT)
	
	yENDSWITCH
}

// 승패 판정
BOOL CBattle::Judge()
{
	return FALSE;	
}
void CBattle::Victory(int WinnerTeamNum,int LoserTeamNum)
{
#ifdef _MAPSERVER_
	MSG_DWORD2 msg;
	msg.Category = MP_BATTLE;
	msg.Protocol = MP_BATTLE_VICTORY_NOTIFY;
	msg.dwData1 = WinnerTeamNum;
	msg.dwData2 = LoserTeamNum;
	
	ToEachTeam(pTeam)
		pTeam->SendTeamMsg(&msg,sizeof(msg));
	EndToEachTeam

#endif

	m_WinnerTeam = WinnerTeamNum;
	m_BattleInfo.BattleState = eBATTLE_STATE_RESULT;
	m_BattleInfo.BattleTime = gCurTime;
}
void CBattle::Draw()
{
#ifdef _MAPSERVER_
	MSGBASE msg;
	msg.Category = MP_BATTLE;
	msg.Protocol = MP_BATTLE_DRAW_NOTIFY;
	
	ToEachTeam(pTeam)
		pTeam->SendTeamMsg(&msg,sizeof(msg));
	EndToEachTeam
#endif
		
	m_WinnerTeam = eBattleTeam_Max;
	m_BattleInfo.BattleState = eBATTLE_STATE_RESULT;
	m_BattleInfo.BattleTime = gCurTime;
}


// Render
#ifdef _CLIENT_

#include "interface/cFont.h"
void CBattle::Render()
{
//	RECT rect;
//	SetRect(&rect,30,100,500,150);
//	char temp[256];
//	sprintf(temp,"Default Battle");
//	CFONT_OBJ->RenderFont(0,temp,strlen(temp),&rect,0xffffffff);
}

#endif

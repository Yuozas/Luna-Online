// RegenManager.cpp: implementation of the CRegenManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RegenManager.h"
#include "RegenPrototype.h"
#include "AIGroupManager.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "AIGroupManager.h"
#include "TileManager.h"
#include "BossMonster.h"
#include "GridSystem.h"
//test
#include "MHError.h"
//test

#include "UserTable.h"
#include "Player.h"

// 필드보스 - 05.12 이영준
#include "FieldBossMonster.h"
#include "FieldSubMonster.h"
#include "AISystem.h"
// 080616 LUJ, 함정이 적 명단을 갖게하기 위해 참조
#include "Trap.h"
#include "MapDBMsgParser.h"

CRegenManager::CRegenManager()
{
	m_RegenPrototypeList.Initialize(500);
}

CRegenManager::~CRegenManager()
{
	Release();
}

void CRegenManager::Release()
{
	CRegenPrototype * pPrototype= NULL;
	m_RegenPrototypeList.SetPositionHead();
	while((pPrototype = m_RegenPrototypeList.GetData()))
	{
		delete pPrototype;
		pPrototype = NULL;
	}
	m_RegenPrototypeList.RemoveAll();
}

void CRegenManager::AddPrototype(CRegenPrototype * pType)
{
	m_RegenPrototypeList.Add(pType, pType->dwObjectID);
}
CRegenPrototype * CRegenManager::GetPrototype(DWORD dwID)
{
	return m_RegenPrototypeList.GetData(dwID);
}
void CRegenManager::RegenGroup(DWORD groupID, DWORD dwGridID)
{
	GROUPMGR->GetGroup(groupID, dwGridID)->ForceRegen();
}

BOOL CRegenManager::RegenObject(DWORD groupID, DWORD dwGridID, DWORD dwObjectID,BOOL bRandomPos)
{
	CAIGroup * pGroup = GROUPMGR->GetGroup(groupID, dwGridID);
	CRegenObject * pRegenInfo = pGroup->GetRegenObject(dwObjectID);
	if(pRegenInfo->GetObjectKind() == eObjectKind_BossMonster ||
	// 필드보스 - 05.12 이영준
	// 필드보스도 지정된 위치중에 랜덤으로 리젠되어야 하므로
	// 랜덤 설정을 끈다
	   pRegenInfo->GetObjectKind() == eObjectKind_FieldBossMonster)
		bRandomPos = FALSE;
	return RegenObject(dwObjectID, pRegenInfo->GetSubID(), dwGridID, 
		pRegenInfo->GetObjectKind(), 
		pRegenInfo->GetMonsterKind(), 
		pRegenInfo->GetPos(), 
		groupID, 0, 100, bRandomPos)
		? TRUE : FALSE;
}
void CRegenManager::RangePosAtOrig(VECTOR3 * OrigPos, int Range, VECTOR3 * OutPos)
{
	int ran=0;
	float dx=0,dz=0;
	ran = rand();
	dx = float(ran%Range) * (ran%2?1:-1);
	ran = rand();
	dz = float(ran%Range) * (ran%2?1:-1);
	OutPos->x = OrigPos->x + dx;
	OutPos->z = OrigPos->z + dz;
	
	if( OutPos->x < 0 )				OutPos->x = 0;
	else if( OutPos->x > 51199 )	OutPos->x = 51199;

	if( OutPos->z < 0 )				OutPos->z = 0;
	else if( OutPos->z > 51199 )	OutPos->z = 51199;
}

CMonster* CRegenManager::RegenObject( DWORD dwID, DWORD dwSubID, DWORD dwGridID, WORD wObjectKind, WORD wMonsterKind, VECTOR3 * vPos, DWORD dwGroupID, DWORD DropItemID /* = 0 */, DWORD dwDropRatio /* = 100  */,BOOL bRandomPos /* = TRUE */, BOOL bEventRegen /* = FALSE  */)
{
	/// 06. 08. 이영준
	/// 같은 아이디를 사용하는 객체가 있을경우
	/// 리젠을 시키지 않고 NULL값을 반환하자!
	if( g_pUserTable->FindUser( dwID ) )
	{
		ASSERT(0);
		return NULL;
	}

	// 필드보스일 경우
	if( wObjectKind == eObjectKind_FieldBossMonster )
	{
		// 필드보스 매니저에 리젠 위치가 1개 이상 등록되어 있다면
		if( FIELDBOSSMONMGR->GetRegenPositionCount() > 0 )
		{
			// 등록된 리젠 위치에서 랜덤으로 결정한다
			VECTOR3 pos = FIELDBOSSMONMGR->GetRegenPosition();
			vPos->x = pos.x;
			vPos->z = pos.z;
		}
	}

	VECTOR3 vOutPos;
	if(bRandomPos)
		RangePosAtOrig(vPos, MONSTER_REGEN_RANDOM_RANGE, &vOutPos);
	else
		vOutPos = *vPos;

	MAPTYPE MapNum = g_pServerSystem->GetGridSystem()->GetGridMapNum(dwGridID);
	vOutPos.x = ((DWORD)(vOutPos.x/TILECOLLISON_DETAIL)*TILECOLLISON_DETAIL);
	vOutPos.z = ((DWORD)(vOutPos.z/TILECOLLISON_DETAIL)*TILECOLLISON_DETAIL);

	switch( wObjectKind )
	{
	case eObjectKind_ToghterPlayMonster:
	case eObjectKind_SpecialMonster:
	case eObjectKind_Monster:
	// 080616 LUJ, 함정 추가
	case eObjectKind_Trap:
		{
			MONSTER_TOTALINFO MonsterInfo;
			BASEOBJECT_INFO Baseinfo;
			BASE_MONSTER_LIST * pMonsterListInfo = NULL;
			
			pMonsterListInfo = GAMERESRCMNGR->GetMonsterListInfo(wMonsterKind);
			if(pMonsterListInfo == NULL)
				return NULL;

			if(g_pServerSystem->GetMap()->CollisionTilePos(vOutPos.x,vOutPos.z,MapNum) == TRUE)
			{
		//		g_Console.LOG(4, "Regen Failed : ID: %d, NAME: %s", dwID, pMonsterListInfo->Name);
				return NULL;
			}

			Baseinfo.dwObjectID = dwID;
			SafeStrCpy(Baseinfo.ObjectName, pMonsterListInfo->Name, MAX_NAME_LENGTH+1);
			Baseinfo.BattleID = dwGridID;
	//		g_Console.LOG(4, "Regen Succeeded : CHANNEL:%d, GROUPID: %d, ID: %d, NAME: %s, (x,z):(%f,%f)", dwGridID, dwGroupID, dwID, pMonsterListInfo->Name, vOutPos.x, vOutPos.z);

			MonsterInfo.Life = pMonsterListInfo->Life;
			MonsterInfo.MonsterKind = wMonsterKind;
			MonsterInfo.Group = (WORD)dwGroupID;
			
			CMonster* pMonster = g_pServerSystem->AddMonster(dwSubID, &Baseinfo, &MonsterInfo, &vOutPos,wObjectKind);
			if(!pMonster)
				return NULL;

			pMonster->SetEventMob(bEventRegen);
			if(bEventRegen && dwGroupID==ITEMRECALL_GROUP_ID)	// 아이템소환몹은 비선공으로 설정한다.
				pMonster->SetForeAttack(FALSE);

			pMonster->SetDropItemID(DropItemID, dwDropRatio);

			return pMonster;

		}
		break;

	// 필드보스 - 05.12 이영준
	// 필드보스 리젠
	case eObjectKind_FieldBossMonster:
		{
			MONSTER_TOTALINFO MonsterInfo;
			BASEOBJECT_INFO Baseinfo;
			BASE_MONSTER_LIST * pMonsterListInfo = NULL;

			pMonsterListInfo = GAMERESRCMNGR->GetMonsterListInfo(wMonsterKind);
			if(g_pServerSystem->GetMap()->CollisionTilePos(vOutPos.x,vOutPos.z,MapNum) == TRUE)
			{
	//			g_Console.LOG(4, "BossMonster Regen Failed : ID: %d, NAME: %s", dwID, pMonsterListInfo->Name);
				return NULL;
			}

			Baseinfo.dwObjectID = dwID;
			Baseinfo.BattleID = dwGridID;
			SafeStrCpy(Baseinfo.ObjectName, pMonsterListInfo->Name, MAX_NAME_LENGTH+1);

			// 필드보스 리젠 확인 로그기록
			//g_Console.LOG(4, "FieldBossMonster Regen Succeeded : GridID: %d, GROUPID: %d, ID: %d, NAME: %s, (x,z):(%f,%f), Time: %d", dwGridID, dwGroupID, dwID, pMonsterListInfo->Name, vOutPos.x, vOutPos.z, gCurTime / 1000);
			//MHERROR->OutputFile("Debug.txt", MHERROR->GetStringArg("FieldBoss Regen = GridID: %d, gCurTime: %d", dwGridID, gCurTime));

			MonsterInfo.Life = pMonsterListInfo->Life;
			MonsterInfo.MonsterKind = wMonsterKind;
			MonsterInfo.Group = (WORD)dwGroupID;

			CFieldBossMonster * pMob = g_pServerSystem->AddFieldBossMonster(dwSubID, &Baseinfo, &MonsterInfo, &vOutPos);
			
			// 필드 보스 매니저에 등록
			FIELDBOSSMONMGR->AddFieldBossMonster(pMob);
			pMob->SetEventMob(bEventRegen);
			if(bEventRegen && dwGroupID==ITEMRECALL_GROUP_ID)	// 아이템소환몹은 비선공으로 설정한다.
				pMob->SetForeAttack(FALSE);

			return pMob;
		}
		break;
	// 필드보스부하 리젠
	case eObjectKind_FieldSubMonster:
		{
			MONSTER_TOTALINFO MonsterInfo;
			BASEOBJECT_INFO Baseinfo;
			BASE_MONSTER_LIST * pMonsterListInfo = NULL;

			pMonsterListInfo = GAMERESRCMNGR->GetMonsterListInfo(wMonsterKind);
			if(g_pServerSystem->GetMap()->CollisionTilePos(vOutPos.x,vOutPos.z,MapNum) == TRUE)
			{
	//			g_Console.LOG(4, "BossMonster Regen Failed : ID: %d, NAME: %s", dwID, pMonsterListInfo->Name);
				return NULL;
			}

			Baseinfo.dwObjectID = dwID;
			Baseinfo.BattleID = dwGridID;
			SafeStrCpy(Baseinfo.ObjectName, pMonsterListInfo->Name, MAX_NAME_LENGTH+1);

			// 필드보스부하 리젠 확인 로그기록
			//g_Console.LOG(4, "FieldSubMonster Regen Succeeded : GROUPID: %d, ID: %d, NAME: %s, (x,z):(%f,%f)", dwGroupID, dwID, pMonsterListInfo->Name, vOutPos.x, vOutPos.z);
			//MHERROR->OutputFile("Debug.txt", MHERROR->GetStringArg("FieldSub Regen = GridID: %d, gCurTime: %d", dwGridID, gCurTime));

			MonsterInfo.Life = pMonsterListInfo->Life;
			MonsterInfo.MonsterKind = wMonsterKind;
			MonsterInfo.Group = (WORD)dwGroupID;

			CFieldSubMonster * pMob = g_pServerSystem->AddFieldSubMonster(dwSubID, &Baseinfo, &MonsterInfo, &vOutPos);
			pMob->SetEventMob(bEventRegen);
			if(bEventRegen && dwGroupID==ITEMRECALL_GROUP_ID)	// 아이템소환몹은 비선공으로 설정한다.
				pMob->SetForeAttack(FALSE);

			return pMob;
		}
		break;

	case eObjectKind_BossMonster:
		{
			MONSTER_TOTALINFO MonsterInfo;
			BASEOBJECT_INFO Baseinfo;
			BASE_MONSTER_LIST * pMonsterListInfo = NULL;

			pMonsterListInfo = GAMERESRCMNGR->GetMonsterListInfo(wMonsterKind);
			if(g_pServerSystem->GetMap()->CollisionTilePos(vOutPos.x,vOutPos.z,MapNum) == TRUE)
			{
	//			g_Console.LOG(4, "BossMonster Regen Failed : ID: %d, NAME: %s", dwID, pMonsterListInfo->Name);
				return NULL;
			}

			Baseinfo.dwObjectID = dwID;
			Baseinfo.BattleID = dwGridID;
			SafeStrCpy(Baseinfo.ObjectName, pMonsterListInfo->Name, MAX_NAME_LENGTH+1);
			g_Console.LOG(4, "BossMonster Regen Succeeded : GROUPID: %d, ID: %d, NAME: %s, (x,z):(%f,%f)", dwGroupID, dwID, pMonsterListInfo->Name, vOutPos.x, vOutPos.z);
			MHERROR->OutputFile("Debug.txt", MHERROR->GetStringArg("Boss Regen = GridID: %d, gCurTime: %d", dwGridID, gCurTime));

			MonsterInfo.Life = pMonsterListInfo->Life;
			MonsterInfo.MonsterKind = wMonsterKind;
			MonsterInfo.Group = (WORD)dwGroupID;

			CBossMonster * pMob = g_pServerSystem->AddBossMonster(dwSubID, &Baseinfo, &MonsterInfo, &vOutPos);
			pMob->SetEventMob(bEventRegen);
			if(bEventRegen && dwGroupID==ITEMRECALL_GROUP_ID)	// 아이템소환몹은 비선공으로 설정한다.
				pMob->SetForeAttack(FALSE);
		
			/*
			//황릉수호장군 난주 습격 이벤트 코드
			if( pMob->GetMonsterKind() == 158 )
			{	
				pMob->AddSpeech(2, 7501);
				WORD temp = (WORD)pMob->GetGridID();

				TESTMSG msg;
				msg.Category = MP_SIGNAL;
				msg.Protocol = MP_SIGNAL_COMMONUSER;
				SafeStrCpy( msg.Msg, "<<이벤트 시작 : 난주 어딘가에 황릉수호장군이 출현하였습니다!!!>>", MAX_CHAT_LENGTH + 1 );
				
				g_pUserTable->SetPositionUserHead();
				
				while( CObject* pObject = g_pUserTable->GetUserData() )
				{
					if( pObject->GetObjectKind() != eObjectKind_Player ) continue;
					if( pObject->GetGridID() != temp ) continue;
										
					CPlayer* pReceiver = (CPlayer*)pObject;
					
					msg.dwObjectID = pReceiver->GetID();
					
					pReceiver->SendMsg( &msg, sizeof( msg ) );				
				}
			}
			*/
			return pMob;
			
		}
		break;
	case eObjectKind_Npc:
		{			
			BASEOBJECT_INFO Baseinfo;
			NPC_TOTALINFO NpcTotalInfo;
			STATIC_NPCINFO* pNpcInfo = GAMERESRCMNGR->GetStaticNpcInfo(wMonsterKind);
			if(pNpcInfo == 0)
			{
//				char buf[256];
//				sprintf(buf, "NO NPC Info - NpcKind: %d", wMonsterKind);
//				ASSERTMSG(0, buf);
				return NULL;
			}
			NPC_LIST* pNpcList = GAMERESRCMNGR->GetNpcList(pNpcInfo->wNpcJob);
			if(pNpcList == 0)
			{
//				char buf[256];
//				sprintf(buf, "NO NPC Info - NpcKind: %d", wMonsterKind);
//				ASSERTMSG(0, buf);
				return NULL;
			}
			if(g_pServerSystem->GetMap()->CollisionTilePos(vOutPos.x,vOutPos.z,MapNum) == TRUE)
			{
		//		g_Console.LOG(4, "Regen Failed : ID: %d, NpcUniqueIdx: %d", dwID, pNpcInfo->wNpcUniqueIdx);
				return NULL;
			}
			Baseinfo.dwObjectID = dwID;
			Baseinfo.BattleID = dwGridID;
			
			SafeStrCpy(Baseinfo.ObjectName, pNpcInfo->NpcName, MAX_NAME_LENGTH+1);
			NpcTotalInfo.Group = (WORD)dwGroupID;
			NpcTotalInfo.MapNum = MapNum;
			NpcTotalInfo.NpcJob = pNpcInfo->wNpcJob;
			NpcTotalInfo.NpcKind = pNpcList->NpcKind;
			NpcTotalInfo.NpcUniqueIdx = pNpcInfo->wNpcUniqueIdx;
		
			g_pServerSystem->AddNpc(&Baseinfo, &NpcTotalInfo, &vOutPos);
	//		g_Console.LOG(4, "NPC Regen Succeeded : ID: %d, UniqueIdx: %d, NAME: %s, (x,z):(%f,%f)", dwID, pNpcInfo->wNpcUniqueIdx, pNpcInfo->NpcName, vOutPos.x, vOutPos.z);

			return NULL;
		}
		break;
	default:
		{
			ASSERT(0);
			return NULL;
		}
	}
	return NULL;
}

// 080616 LUJ, 함정을 생성한다
BOOL CRegenManager::RegenTrap( CObject* object, DWORD monsterKind )
{
	CPlayer*					player		= ( CPlayer* )object;
	const BASE_MONSTER_LIST*	monsterList = GAMERESRCMNGR->GetMonsterListInfo( monsterKind );

	if( !	monsterList										||
		!	player											||
			player->GetObjectKind() != eObjectKind_Player	||
			// 080616 LUJ, 플레이어는 각각의 종류 별로 한 개씩만 함정을 만들 수 있다. 기획 사항
			player->IsHaveTrap( monsterKind ) )
	{
		return FALSE;
	}

	VECTOR3 position = { 0 };
	player->GetPosition( &position );

	CMonster* monster = RegenObject(
		g_pAISystem->GeneraterMonsterID(),
		g_pAISystem->GeneraterMonsterID(),
		player->GetGridID(),
		monsterList->ObjectKind,
		monsterList->MonsterKind,
		&position,
		EVENTRECALL_GROUP_ID,
		0,
		0,
		FALSE );

	if( ! monster )
	{
		return FALSE;
	}
	else if( monster->GetObjectKind() != eObjectKind_Trap )
	{
		monster->Die( 0 );
		return FALSE;
	}

	player->AddTrap( monsterKind );

	CTrap* trap = ( CTrap* )monster;

	trap->SetTimeOut( monsterList->Life * 1000 + gCurTime );
	trap->SetOwner( player->GetID() );
	WORD length = 0;
	// 080616 LUJ, 생성시킨 후 반드시 전송해줘야 소환한 플레이어에게 표시된다
	trap->SetAddMsg( 0, &length, 0, TRUE );

	// 080616 LUJ, 적군 대상을 DB에 요청한다. 이렇게 함으로써 함정 생성 과정을 가능한 짧게 끝낼 수 있다
	g_DB.FreeMiddleQuery(
		RAddEnemyToTrap,
		trap->GetID(),
		"EXEC DBO.MP_GUILD_ENEMY_SELECT %d, %d, %d",
		player->GetGuildIdx(),
		0,
		MAX_MIDDLEROW_NUM );

	return TRUE;
}
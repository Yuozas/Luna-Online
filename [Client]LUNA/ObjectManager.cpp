#include "stdafx.h"
#include "ObjectManager.h"

#include "AppearanceManager.h"
#include "MoveManager.h"
#include "ItemManager.h"
#include "cSkillTreeManager.h"
#include "CharacterCalcManager.h"
#include "ObjectStateManager.h"
#include "PeaceWarModeManager.h"
#include "PartyManager.h"
#include "Effect\EffectManager.h"
#include "ChatManager.h"
#include "WantedManager.h"

#include "GameIn.h"
#include "ObjectBalloon.h"

#include "MonsterGuageDlg.h"

#include "GuildFieldWar.h"
#include "BattleSystem_Client.h"
#include "..\[CC]BattleSystem\SiegeWar\Battle_SiegeWar.h"

// 070402 LYW --- Include cButton.
#include "./interface/cButton.h"

#include "CheatMsgParser.h"

// 070123 LYW --- Include header file.
#include "ObjectGuagen.h"

// 070402 LYW --- Include cStatic.
#include "interface/cStatic.h"
#include "interface/cWindowManager.h"

#include "MainGame.h"
#include "MHCamera.h"

#include "StatusIconDlg.h"
#include "cSkillTreeManager.h"
//#include "../[cc]skill/client/manager/Skillmanager.h"
#include "../[cc]skill/client/object/skillobject.h"

#include "UserInfoManager.h"

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.05.07	2007.09.13
#include "../hseos/Farm/SHFarmManager.h"
#include "NpcScriptDialog.h"
// E 농장시스템 추가 added by hseos 2007.05.07	2007.09.13

// 071210 LYW --- ObjectManager : 확장인벤토리 활성화 여부에 따른 인벤토리 헤더를 불러온다.
#include "../InventoryExDialog.h"

// 080411 LYW --- ObjectManager : 채팅방 매니져 헤더를 호출.
#include "./ChatRoomMgr.h"

// 080515 KTH
#include "../hseos/Farm/FarmAnimalDlg.h"

DWORD gHeroID = 0;
DWORD gUserID = 0;


GLOBALTON(CObjectManager);


CObjectManager::CObjectManager()
{
	m_pHero			= NULL;
	mpHeroPet		= NULL;
	mpHeroPetBackup = NULL;
	m_pCharCalcMgr	= CHARCALCMGR;
	m_ObjectTable.Initialize(1024);
	m_HideObjectTable.Initialize(10);
	
	m_PlayerPool.Init(MAX_PLAYEROBJECT_NUM, 50, "CPlayer");
	m_MonsterPool.Init(MAX_MONSTEROBJECT_NUM, 50, "CMonster");
	m_NpcPool.Init(MAX_NPCOBJECT_NUM, 50, "CNpc");
//	m_BossMonsterPool.Init(MAX_MONSTEROBJECT_NUM, 50, "CBossMonster");	//KES 보스가 MAX_MONSTEROBJECT_NUM개 필요한가?
	m_BossMonsterPool.Init(10, 5, "CBossMonster");	//KES
	m_MapObjectPool.Init(MAX_MAPOBJECT_NUM, 10, "CMapObject");
	m_PetPool.Init( MAX_PETOBJECT_NUM, 10, "CPet" );

	m_SelectedObjectID	= 0;
	m_pDPObjectName		= NULL;

	m_dwOverInfoOption	= eOO_NAME_CLICKEDONLY;
	m_bAllNameShow		= FALSE;

	for(int i = 0 ; i < eBOSSTYPE_MAX ; ++i)
		m_pBoss[i] = NULL;


	//trustpak 2005/06/27
	m_dwEffectOption = eOO_EFFECT_DEFAULT;

	m_SelectEffectHandle = 0;
	///
}

CObjectManager::~CObjectManager()
{
}


void CObjectManager::AddShadow(CObject* pObject)
{
	if(pObject->GetObjectKind() == eObjectKind_Player)
	{
		if(((CPlayer*)pObject)->m_CharacterInfo.bVisible == FALSE)
		{
			RemoveShadow(pObject);
			return;
		}
	}

	BYTE bShadow = GAMERESRCMNGR->m_GameDesc.bShadow;
	if(bShadow == MHSHADOW_CIRCLE || bShadow == MHSHADOW_DETAIL)
	{		
		if(pObject == HERO && bShadow == MHSHADOW_DETAIL)
			pObject->GetEngineObject()->EnableShadow(TRUE);
		else
		{
			pObject->GetEngineObject()->EnableShadow(FALSE);
			//pObject->m_ShadowObj.Init("sh.mod",NULL,eEngineObjectType_Effect,0,-1);
			//trustpak 2005/06/22
			pObject->m_ShadowObj.Init("sh.mod",NULL,eEngineObjectType_Shadow,0,-1);
			///
			pObject->m_ShadowObj.ApplyHeightField(TRUE);
		}
	}
	else
	{
		pObject->GetEngineObject()->EnableShadow(FALSE);
	}
}

void CObjectManager::RemoveShadow(CObject* pObject)
{
	if(pObject->m_ShadowObj.IsInited())
	{
		pObject->m_ShadowObj.Release();
	}
	pObject->GetEngineObject()->EnableShadow(FALSE);
}

void CObjectManager::RegisterHero(SEND_HERO_TOTALINFO * sendHeroTotalInfo)
{
	m_pHero = new CHero;
	m_pHero->Init(eObjectKind_Player, &sendHeroTotalInfo->BaseObjectInfo);
	m_pHero->InitPlayer(&sendHeroTotalInfo->ChrTotalInfo);
	m_pHero->InitHero(&sendHeroTotalInfo->HeroTotalInfo);
	gHeroID = m_pHero->GetID();
	m_pHero->InitObjectBalloon(ObjectBalloon_Name | ObjectBalloon_Chat | ObjectBalloon_Title | ObjectBalloon_MunpaMark );

	// 071210 LYW --- ObjectManager : 확장 인벤토리의 활성화 여부를 세팅한다.
	CInventoryExDialog* pInvenDlg = NULL ;
	pInvenDlg = GAMEIN->GetInventoryDialog() ;

	if( !pInvenDlg ) return ;

	pInvenDlg->InitExtended_Inventory( &sendHeroTotalInfo->ChrTotalInfo ) ;

	m_pHero->SetShopItemOptionInfo(&sendHeroTotalInfo->ShopItemOption);
	m_pHero->CalcAvatarOption();
	// 070124 LYW --- PeaceWar : Delete peacewar part.
	//PEACEWARMGR->InitPeace_WarMode(m_pHero);
	APPEARANCEMGR->InitAppearance(m_pHero);
	
	BASEMOVE_INFO moveInfo;
	moveInfo.SetFrom(&sendHeroTotalInfo->SendMoveInfo);	
	
	MOVEMGR->InitMove(m_pHero,&moveInfo);
	OBJECTSTATEMGR->InitObjectState(m_pHero);
//	MUSSANGMGR->Init();
	//ITEMMGR->InitOwnItem(&sendHeroTotalInfo->ItemTotalInfo);

	//SW051004 수정	//msg 크기를 최적화하여 받아 처리하는 부분
	CAddableInfoIterator iter(&sendHeroTotalInfo->AddableInfo);
	;
	for(	BYTE kind;
			kind = iter.GetInfoKind();
			iter.ShiftToNextData() )
	{
		if( CAddableInfoList::None == kind )
		{
			break;
		}

		switch( kind )
		{
		case (CAddableInfoList::ItemOption):
			{
				ITEM_OPTION OptionInfo[MAX_ITEM_OPTION_NUM+SLOT_STORAGE_NUM];	//iter.GetInfoData( 저장소를 요구한다.
				iter.GetInfoData(&OptionInfo);						//iter.GetInfoDataptr( 을 하나 만들던지..

				for( WORD i = 0; i < sendHeroTotalInfo->OptionNum; ++i )
				{
					ITEM_OPTION& option = OptionInfo[ i ];

					ITEMMGR->AddOption( option );
				}
			}
			break;
		case (CAddableInfoList::SkillTree):
			{
				SKILL_BASE SkillTreeInfo[ 1024 ];
				iter.GetInfoData( SkillTreeInfo );

				ASSERT( 1024 > sendHeroTotalInfo->SkillNum );

				SKILLTREEMGR->InitSkillInfo( SkillTreeInfo, sendHeroTotalInfo->SkillNum );
			}
		}
	}

	ITEMMGR->NetworkMsgParse(MP_ITEM_TOTALINFO_LOCAL, &sendHeroTotalInfo->ItemTotalInfo);
//	SKILLTREEMGR->NetworkMsgParse(MP_SKILLTREE_INFO_LOCAL, &sendHeroTotalInfo->SkillTreeInfo);
	

	m_ObjectTable.Add(m_pHero,m_pHero->GetID());


	// 인터페이스에 적용
	m_pHero->ApplyInterface();

	ApplyOverInfoOption( m_pHero );
	ApplyShadowOption( m_pHero );

//KES 040117
	m_pHero->GetEngineObject()->DisablePick();

	CHARCALCMGR->CalcCharStats(HERO);
	m_pHero->PassiveSkillCheckForWeareItem();

//	USERINFOMGR->LoadUserInfo( eUIK_USERQUEST );

	// 080411 LYW --- ObjectManager : 채팅방에서 채팅중이 었다면, 채팅 내용을 복구한다.
	//CHATROOMMGR->RestoreChat() ;

	return;
}



#ifndef _RESOURCE_WORK_NOT_APPLY_

CPlayer* CObjectManager::AddPlayerThroughCache(BASEOBJECT_INFO* pBaseObjectInfo,BASEMOVE_INFO* pMoveInfo,CHARACTER_TOTALINFO* pTotalInfo,SHOPITEMOPTION* pShopItemOption, SEND_CHARACTER_TOTALINFO* pMsg)
{
	CObject* pPreObj = GetObject(pBaseObjectInfo->dwObjectID);
	ASSERT(pPreObj == NULL);
	if(pPreObj)
	{
		RemoveObject(&pPreObj);
	}

	CPlayer* pPlayer = m_PlayerPool.Alloc();
	
	pPlayer->Init(eObjectKind_Player,pBaseObjectInfo);
	pPlayer->InitPlayer(pTotalInfo);
	pPlayer->InitObjectBalloon(ObjectBalloon_Name | ObjectBalloon_Chat | ObjectBalloon_Title | ObjectBalloon_MunpaMark );	
	pPlayer->SetShopItemOptionInfo( pShopItemOption );
	pPlayer->CalcAvatarOption();

	// 070124 LYW --- PeaceWar : Delete peacewar part.
	//PEACEWARMGR->InitPeace_WarMode(pPlayer);

	APPEARANCEMGR->ReserveToAppearObject(pPlayer, (void*)pMsg);

	MOVEMGR->InitMove(pPlayer,pMoveInfo);	

	m_ObjectTable.Add(pPlayer,pPlayer->GetID());

	// 070523 웅주. 다른 플레이어 접속시 하트 체크하기 위해 처리. 이동 정보 복사된 후 호출되어야 함
	pPlayer->OnStartObjectState( eObjectState_Connect );
	pPlayer->SetState( pBaseObjectInfo->ObjectState );
	return pPlayer;
}



CPlayer* CObjectManager::AddPlayer(BASEOBJECT_INFO* pBaseObjectInfo,BASEMOVE_INFO* pMoveInfo,CHARACTER_TOTALINFO* pTotalInfo,SHOPITEMOPTION* pShopItemOption)
{
	CObject* pPreObj = GetObject(pBaseObjectInfo->dwObjectID);
	if(pBaseObjectInfo->dwObjectID != 1 ) ASSERT(pPreObj == NULL);
	if(pPreObj)
	{
		RemoveObject(&pPreObj);
	}

	CPlayer* pPlayer = m_PlayerPool.Alloc();
	
	pPlayer->Init(eObjectKind_Player,pBaseObjectInfo);
	pPlayer->InitPlayer(pTotalInfo);
	pPlayer->InitObjectBalloon(ObjectBalloon_Name | ObjectBalloon_Chat | ObjectBalloon_Title | ObjectBalloon_MunpaMark );
	pPlayer->SetShopItemOptionInfo( pShopItemOption );
	pPlayer->CalcAvatarOption();

	// 070124 LYW --- PeaceWar : Delete peacewar part.
	//PEACEWARMGR->InitPeace_WarMode(pPlayer);
	APPEARANCEMGR->InitAppearance(pPlayer);

	MOVEMGR->InitMove(pPlayer,pMoveInfo);	
	OBJECTSTATEMGR->InitObjectState(pPlayer);

	m_ObjectTable.Add(pPlayer,pPlayer->GetID());

	ApplyOverInfoOption( pPlayer );
	ApplyShadowOption( pPlayer );

	if( pTotalInfo->HideLevel )
	{
		SetHide( pPlayer->GetID(), pPlayer->GetCharacterTotalInfo()->HideLevel );
	}
	return pPlayer;
}



#else



CPlayer* CObjectManager::AddPlayer(BASEOBJECT_INFO* pBaseObjectInfo,BASEMOVE_INFO* pMoveInfo,CHARACTER_TOTALINFO* pTotalInfo,SHOPITEMOPTION* pShopItemOption)
{
	CObject* pPreObj = GetObject(pBaseObjectInfo->dwObjectID);
	ASSERT(pPreObj == NULL);
	if(pPreObj)
	{
		RemoveObject(&pPreObj);
	}

	CPlayer* pPlayer = m_PlayerPool.Alloc();
	
	pPlayer->Init(eObjectKind_Player,pBaseObjectInfo);
	pPlayer->InitPlayer(pTotalInfo);
	pPlayer->InitObjectBalloon(ObjectBalloon_Name | ObjectBalloon_Chat | ObjectBalloon_Title | ObjectBalloon_MunpaMark );
	pPlayer->SetShopItemOptionInfo( pShopItemOption );
	pPlayer->CalcAvatarOption();

	// 070124 LYW --- PeaceWar : Delete peacewar part.
	//PEACEWARMGR->InitPeace_WarMode(pPlayer);
	APPEARANCEMGR->InitAppearance(pPlayer);

	MOVEMGR->InitMove(pPlayer,pMoveInfo);	
	OBJECTSTATEMGR->InitObjectState(pPlayer);

	m_ObjectTable.Add(pPlayer,pPlayer->GetID());

	ApplyOverInfoOption( pPlayer );
	ApplyShadowOption( pPlayer );

	return pPlayer;
}


#endif

CMonster* CObjectManager::AddMonster(BASEOBJECT_INFO* pBaseObjectInfo,BASEMOVE_INFO* pMoveInfo,MONSTER_TOTALINFO* pTotalInfo)
{
//	LOGFILE("MP_USERCONN_MONSTER_ADD\t%d\t%s",pBaseObjectInfo->dwObjectID,pBaseObjectInfo->ObjectName);

	CObject* pPreObj = GetObject(pBaseObjectInfo->dwObjectID);
	ASSERT(pPreObj == NULL);
	if(pPreObj)
	{
//		LOGFILE("Overlapped\t%s",OBJECTLOG(pPreObj));
		RemoveObject(&pPreObj);
	}
	ASSERT(pTotalInfo->MonsterKind != 0);

	CMonster* pMonster = m_MonsterPool.Alloc();

	// 071227 LUJ, 몬스터 정보가 없어 클라이언트가 튕기는 현상 방지
	WORD MonsterKind;
	{
		BASE_MONSTER_LIST* monsterList = GAMERESRCMNGR->GetMonsterListInfo( pTotalInfo->MonsterKind );

		MonsterKind = ( monsterList ? monsterList->ObjectKind : 0 );

		// 080611 LUJ, GM툴 상태에서만 표시하도록 함
#ifdef _GMTOOL_
		if( ! MonsterKind )
		{
			cprintf(
				"경고: 몬스터 정보(%d)를 찾지 못했습니다",
				pTotalInfo->MonsterKind );
		}	
#endif
	}

	BOOL IsFieldBoss = false;
	BOOL IsSpecialMonster = false;
	BOOL IsToghterPlayMonster = false;

	switch(MonsterKind)
	{
	case eObjectKind_FieldBossMonster: IsFieldBoss = true;           break;
	case eObjectKind_SpecialMonster:   IsSpecialMonster = true;      break;
	case eObjectKind_ToghterPlayMonster:IsToghterPlayMonster = true; break;
	default: break;
	}
	
	//다른 enum형을 집어 넣을경우 객체가 생성 되지 않는 버그가 있다.????
	//060710 - by wonju

	pMonster->Init(eObjectKind_Monster,pBaseObjectInfo);
	pMonster->InitMonster(pTotalInfo);
	pMonster->InitObjectBalloon(ObjectBalloon_Name | ObjectBalloon_Chat);

	APPEARANCEMGR->InitAppearance(pMonster);

	MOVEMGR->InitMove(pMonster,pMoveInfo);

	OBJECTSTATEMGR->InitObjectState(pMonster);

	m_ObjectTable.Add(pMonster,pMonster->GetID());

	ApplyOverInfoOption( pMonster );
	ApplyShadowOption( pMonster );

	if(IsFieldBoss) pMonster->SetObjectKind(eObjectKind_FieldBossMonster);
	else if(IsSpecialMonster) pMonster->SetObjectKind(eObjectKind_SpecialMonster);//Add 060710 - by wonju
	else if(IsToghterPlayMonster) pMonster->SetObjectKind(eObjectKind_ToghterPlayMonster);

	return pMonster;
}
CBossMonster* CObjectManager::AddBossMonster(BASEOBJECT_INFO* pBaseObjectInfo,BASEMOVE_INFO* pMoveInfo,MONSTER_TOTALINFO* pTotalInfo)
{
	CBossMonster * pBoss = NULL;
	CObject* pPreObj = GetObject(pBaseObjectInfo->dwObjectID);
	ASSERT(pPreObj == NULL);
	if(pPreObj)
	{
		RemoveObject(&pPreObj);
	}
	switch(pTotalInfo->MonsterKind)
	{
	case eBOSSKIND_TARINTUS:
		{
			pBoss = m_pBoss[eBOSSTYPE_TARINTUS] = m_BossMonsterPool.Alloc();
		}
		break;
	case eBOSSKIND_KIERRA:
		{
			pBoss = m_pBoss[eBOSSTYPE_KIERRA] = m_BossMonsterPool.Alloc();
		}
		break;
/*	case eBOSSKIND_EMPERIORTOMB_EVENT:
	case eBOSSKIND_EMPERIORTOMB:
		{
			pBoss = m_pBoss[eBOSSTYPE_EMPERIORTOMB] = m_BossMonsterPool.Alloc();
		}
		break;
	case eBOSSKIND_GENERAL:
		{
			pBoss = m_pBoss[eBOSSTYPE_GENERAL] = m_BossMonsterPool.Alloc();
		}
		break;
	case eBOSSTYPE_YUKJI:
		{
			pBoss = m_pBoss[eBOSSTYPE_YUKJI] = m_BossMonsterPool.Alloc();
		}
		break;
	case eBOSSKIND_MOOUN:
		{
			pBoss = m_pBoss[eBOSSTYPE_MOOUN] = m_BossMonsterPool.Alloc();
		}
		break;
	case eBOSSKIND_COWF:
		{
			pBoss = m_pBoss[eBOSSTYPE_COWF] = m_BossMonsterPool.Alloc();
		}
		break;
	case eBOSSKIND_COWL:
		{
			pBoss = m_pBoss[eBOSSTYPE_COWL] = m_BossMonsterPool.Alloc();
		}
		break;
	case eBOSSKIND_COWKING:
		{
			pBoss = m_pBoss[eBOSSTYPE_COWKING] = m_BossMonsterPool.Alloc();
		}
		break;*/
	default:
		ASSERT(0);
		
	}
	
	pBoss->Init(eObjectKind_BossMonster, pBaseObjectInfo);
	pBoss->InitMonster(pTotalInfo);
	pBoss->InitObjectBalloon(ObjectBalloon_Name | ObjectBalloon_Chat);

	APPEARANCEMGR->InitAppearance(pBoss);
	MOVEMGR->InitMove(pBoss,pMoveInfo);
	
	OBJECTSTATEMGR->InitObjectState(pBoss);

	m_ObjectTable.Add(pBoss,pBoss->GetID());

	ApplyOverInfoOption( pBoss );
	ApplyShadowOption( pBoss );	
	
	return pBoss;
}


CNpc* CObjectManager::AddNpc(BASEOBJECT_INFO* pBaseObjectInfo,BASEMOVE_INFO* pMoveInfo,NPC_TOTALINFO* pTotalInfo)
{
	CNpc* pNpc = m_NpcPool.Alloc();
	pNpc->Init(eObjectKind_Npc,pBaseObjectInfo);
	pNpc->InitNpc(pTotalInfo);
	pNpc->InitObjectBalloon(ObjectBalloon_Name);
	pNpc->m_MoveInfo.CurPosition.x = 0;
	pNpc->m_MoveInfo.CurPosition.y = 0;
	pNpc->m_MoveInfo.CurPosition.z = 0;

	APPEARANCEMGR->InitAppearance(pNpc);
	MOVEMGR->InitMove(pNpc,pMoveInfo);

	OBJECTSTATEMGR->InitObjectState(pNpc);
	
	m_ObjectTable.Add(pNpc,pNpc->GetID());

	ApplyOverInfoOption( pNpc );
	ApplyShadowOption( pNpc );

	return pNpc;
}

void CObjectManager::AddSkillObject(cSkillObject* pSObj)
{
	CObject* pPreObj = GetObject(pSObj->GetID());
	ASSERT(pPreObj == NULL);
	if(pPreObj)
	{
		RemoveObject(&pPreObj);
	}

	m_ObjectTable.Add(pSObj,pSObj->GetID());
}

void CObjectManager::RemoveAllObject()
{
	m_ObjectTable.SetPositionHead();
	CObject* pObject;
	while(pObject = m_ObjectTable.GetData())
	{
		RemoveObject(&pObject);
	}
	m_ObjectTable.RemoveAll();
	
	SetSelectedObjectID(0);
	m_bAllNameShow		= FALSE;
	
	if( GAMEIN->GetMonsterGuageDlg() )
		GAMEIN->GetMonsterGuageDlg()->SetActive(FALSE);
}

void CObjectManager::RemoveObject(CObject** ppObject)
{
	if(*ppObject)
	{
//		LOGFILE("RemoveObject\t%s",OBJECTLOG(*ppObject));

		m_ObjectTable.Remove((*ppObject)->GetID());

//		if(*ppObject == GetDPObject())
//			SetDPObject(NULL);

		if(m_SelectedObjectID == (*ppObject)->GetID() )
		{
			SetSelectedObjectID(0);
		}

		int kind = (*ppObject)->GetObjectKind();
		(*ppObject)->Release();
		if((*ppObject) == HERO)
		{
			delete (*ppObject);
			m_pHero = NULL;
		}
		else if( (*ppObject) == mpHeroPet )
		{
			delete (*ppObject);
			mpHeroPet = NULL;
		}
		else
		{
			switch(kind)
			{
			case eObjectKind_Player:
				m_PlayerPool.Free((CPlayer*)*ppObject);
				break;
			case eObjectKind_Monster:
			case eObjectKind_FieldBossMonster:
			case eObjectKind_FieldSubMonster:
				m_MonsterPool.Free((CMonster*)*ppObject);
				break;
			case eObjectKind_SpecialMonster:
				m_MonsterPool.Free((CMonster*)*ppObject);
				break;
			case eObjectKind_ToghterPlayMonster:
				m_MonsterPool.Free((CMonster*)*ppObject);
				break;
			case eObjectKind_Npc:
				m_NpcPool.Free((CNpc*)*ppObject);
				break;
			case eObjectKind_SkillObject:
				// DO Nothing
				break;
				
			case eObjectKind_BossMonster:
				m_BossMonsterPool.Free((CBossMonster*)*ppObject);
				break;
			case eObjectKind_MapObject:
			case eObjectKind_CastleGate:
				m_MapObjectPool.Free((CMapObject*)*ppObject);
				break;
			// desc_hseos_농장시스템_01
			// S 농장시스템 추가 added by hseos 2007.05.07
			case eObjectKind_FarmObj:
				m_csFarmRenderObjPool.Free((CSHFarmRenderObj*)*ppObject);
				break;
			// E 농장시스템 추가 added by hseos 2007.05.07
			case eObjectKind_Pet:
				m_PetPool.Free((CPet*)*ppObject);
			default:
				{
				ASSERT(0);
				}
			}
		}
		*ppObject = NULL;		
	}
}

void CObjectManager::AddGarbageObject(CObject* pObject)
{
	RemoveObject(&pObject);
}

void CObjectManager::ProcessGarbage()
{
	CObject* pObject;
	while(pObject = (CObject*)m_GarbageObjectList.RemoveTail())
	{
		RemoveObject(&pObject);
	}
}

CObject* CObjectManager::GetObject(DWORD dwObjectID)
{
	return m_ObjectTable.GetData(dwObjectID);
}



void CObjectManager::GetTargetInRange(VECTOR3* pPivotPos,float Radius,ObjectArray<LPOBJECT>* pObjectArray,BOOL bPK,WORD PARAM)
{
	pObjectArray->Init();
	m_ObjectTable.SetPositionHead();
	CObject* pObject;
	VECTOR3 ObjectPos;
	float dist;
	float dx,dz;
	while(pObject = m_ObjectTable.GetData())
	{
		if( pObject->GetObjectKind() == eObjectKind_Npc )
			continue;

		if(pObject->IsDied() == TRUE)
			continue;

		if(pObject->GetObjectKind() == eObjectKind_Monster )
		{
			if( ( ( CMonster* )pObject )->GetSInfo()->SpecialType == 2 )
			{
				continue;
			}
		}

		switch( PARAM )
		{
		case 1:
			{
				if( pObject == HERO )
					continue;

				if( pObject == HEROPET )
					continue;

				if( pObject->GetObjectKind() == eObjectKind_Player )
				{
					CBattle* pBattle = BATTLESYSTEM->GetBattle( HERO );
					if( !pBattle->IsEnemy( HERO, pObject ) )
						continue;
				}

				if( pObject->GetObjectKind() == eObjectKind_Pet )
				{
					CPet* pPet = ( CPet* )pObject;
					CPlayer* pPlayer = pPet->GetMaster();

					if( pPlayer )
					{
						CBattle* pBattle = BATTLESYSTEM->GetBattle( HERO );
						if( !pBattle->IsEnemy( HERO, pPlayer ) )
							continue;
					}
					else
					{
						continue;
					}
				}
			}
			break;
		case 2:
			{
				if( pObject != HERO && !PARTYMGR->IsPartyMember(pObject->GetID()) )
					continue;
			}
			break;
		case 3:
			{
				if( pObject->GetObjectKind() != eObjectKind_Player )
					continue;

				if( HERO->GetFamilyIdx() != ((CPlayer*)pObject)->GetFamilyIdx() )
					continue;
			}
			break;
		case 4:
			{
				if( pObject->GetObjectKind() != eObjectKind_Player )
					continue;

				if( HERO->GetGuildIdx() != ((CPlayer*)pObject)->GetGuildIdx() )
					continue;
			}
			break;
		case 5:
			{
				if( pObject->GetObjectKind() != eObjectKind_Player )
					continue;
			}
			break;
		case 6:
			{
				if( pObject->GetObjectKind() != eObjectKind_Player )
					continue;

				if( HERO == pObject )
					continue;
			}
			break;


		}

		
		pObject->GetPosition(&ObjectPos); 
		dx = (pPivotPos->x - ObjectPos.x);
		dz = (pPivotPos->z - ObjectPos.z);
		dist = sqrtf(dx*dx + dz*dz) - pObject->GetRadius();
		if(dist <= Radius)
			pObjectArray->AddObject(pObject);
	}
}

DWORD CObjectManager::GetSingleTargetInRange(VECTOR3* pPivotPos,float Radius,BOOL bPK,WORD PARAM)
{
	m_ObjectTable.SetPositionHead();
	CObject* pObject;
	VECTOR3 ObjectPos;
	float dist;
	float dx,dz;
	while(pObject = m_ObjectTable.GetData())
	{
		if( pObject->GetObjectKind() == eObjectKind_Npc )
			continue;

		if(pObject->IsDied() == TRUE)
			continue;

		if(pObject->GetObjectKind() == eObjectKind_Monster )
		{
			if( ( ( CMonster* )pObject )->GetSInfo()->SpecialType == 2 )
			{
				continue;
			}
		}

		switch( PARAM )
		{
		case 1:
			{
				if( pObject == HERO )
					continue;

				if( pObject == HEROPET )
					continue;

				if( pObject->GetObjectKind() == eObjectKind_Player )
				{
					CBattle* pBattle = BATTLESYSTEM->GetBattle( HERO );
					if( !pBattle->IsEnemy( HERO, pObject ) )
						continue;
				}
			}
			break;
		case 2:
			{
				if( pObject != HERO && !PARTYMGR->IsPartyMember(pObject->GetID()) )
					continue;
			}
			break;
		case 3:
			{
				if( pObject->GetObjectKind() != eObjectKind_Player )
					continue;

				if( HERO->GetFamilyIdx() != ((CPlayer*)pObject)->GetFamilyIdx() )
					continue;
			}
			break;
		case 4:
			{
				if( pObject->GetObjectKind() != eObjectKind_Player )
					continue;

				if( HERO->GetGuildIdx() != ((CPlayer*)pObject)->GetGuildIdx() )
					continue;
			}
			break;
		case 5:
			{
				if( pObject->GetObjectKind() != eObjectKind_Player )
					continue;
			}
			break;
		case 6:
			{
				if( pObject->GetObjectKind() != eObjectKind_Player )
					continue;

				if( HERO == pObject )
					continue;
			}
			break;
		}
		
		pObject->GetPosition(&ObjectPos); 
		dx = (pPivotPos->x - ObjectPos.x);
		dz = (pPivotPos->z - ObjectPos.z);
		dist = sqrtf(dx*dx + dz*dz) - pObject->GetRadius();
		if(dist <= Radius)
			return pObject->GetID();
	}

	return 0;
}

void CObjectManager::GetTargetInArea(CHero* pHero,CSkillArea* pSkillArea,ObjectArray<LPOBJECT>* pObjectArray,BOOL bPK,WORD PARAM)
{

}

CObject* CObjectManager::GetSelectedObject()
{
	if(m_SelectedObjectID == 0)
		return NULL;
	CObject* pObject = GetObject(m_SelectedObjectID);
	return pObject;
}

void CObjectManager::SetSelectedEffect(CObject* pObject)
{
	if( pObject )
	{
		if( m_SelectEffectHandle )
		{
			EFFECTMGR->ForcedEndEffect( m_SelectEffectHandle );
		}

		//m_SelectEffectHandle = EFFECTMGR->StartEffectProcess(364,pObject,NULL,0,0);
		TARGETSET set;
		set.pTarget = pObject;
		m_SelectEffectHandle = EFFECTMGR->StartEffectProcess(364,HERO,&set,1,pObject->GetID());
	}
	else
	{
		if( m_SelectEffectHandle )
		{
			EFFECTMGR->ForcedEndEffect( m_SelectEffectHandle );
		}
	}
}

void CObjectManager::SetSelectedObject(CObject* pObject)
{
	CMonsterGuageDlg* pGuageDlg = GAMEIN->GetMonsterGuageDlg() ;

	BOOL rt = SeletedLastObject(pObject, pGuageDlg) ;

	if( rt == FALSE ) return ;
	
	SetSelectedEffect( pObject );

	if( !pObject || !pGuageDlg )
	{
		SetSelectedObjectID(0);
		GAMEIN->GetPartyDialog()->SetClickedMemberID(0);
		return ;
	}

#ifdef _CHEATENABLE_
	CHEATMGR->SetCheatTargetObject(pObject);
#endif

	m_SelectedObjectID = pObject->GetID();
	pObject->OnSelected();

	pGuageDlg->SetName( pObject, pObject->GetObjectName() ) ; 

	BYTE objectKind = pObject->GetObjectKind() ;
	pGuageDlg->SetCurOBJKind(objectKind) ;

	switch(objectKind)
	{
	case eObjectKind_Player :	SelectedPlayer(pObject, pGuageDlg) ;	break ;

	case eObjectKind_Npc :		SelectedNpc(pObject, pGuageDlg) ;		break ;
	
	case eObjectKind_Monster:
	case eObjectKind_BossMonster:
	case eObjectKind_SpecialMonster:
	case eObjectKind_FieldBossMonster:
	case eObjectKind_FieldSubMonster:
		SelectedMonster(pObject, pGuageDlg) ;	break ;
	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.08.22
	case eObjectKind_FarmObj :	SelectedFarmObj(pObject, pGuageDlg);	break ;
	// E 농장시스템 추가 added by hseos 2007.08.22
	case eObjectKind_Pet:	SelectedPet(pObject, pGuageDlg) ;	break ;
	}
}

BOOL CObjectManager::SeletedLastObject(CObject* pObject, CMonsterGuageDlg* pGuageDlg)
{
	CObject* pLastSelectedObject = GetSelectedObject();

	if( pLastSelectedObject == pObject ) return FALSE ;

	if( pLastSelectedObject && pLastSelectedObject->IsInited() )
	{
		if(( CanOverInfoChange( pLastSelectedObject ) && !m_bAllNameShow ) ||
			( pLastSelectedObject->GetObjectKind() & eObjectKind_Monster ))
		{
			pLastSelectedObject->ShowObjectName( FALSE );
		}
		else
		{
			pLastSelectedObject->ShowObjectName( TRUE, GetObjectNameColor(pLastSelectedObject) );		
		}
		
		pLastSelectedObject->OnDeselected();
	}

	return TRUE ;
}

void CObjectManager::SelectedPlayer(CObject* pObject, CMonsterGuageDlg* pGuageDlg)
{
	CPlayer* pPlayer = ((CPlayer*)pObject) ;

	pGuageDlg->SetAlpha(255) ;
	pGuageDlg->SetActive( TRUE ) ;
	pGuageDlg->GetMonsterGuage()->SetActive( FALSE ) ;
	
	if( pPlayer->GetGuildIdx() != 0 )
	{
		pGuageDlg->GetGuildStatic()->SetActive( TRUE ) ;
		pGuageDlg->GetUnionStatic()->SetActive( TRUE ) ;

		pGuageDlg->SetGuildName( pPlayer->GetGuildName(), pPlayer->GetGuildUnionName() ) ;
	}
	else
	{
		pGuageDlg->GetGuildStatic()->SetActive( FALSE ) ;
		pGuageDlg->GetUnionStatic()->SetActive( FALSE ) ;
	}
}

void CObjectManager::SelectedNpc(CObject* pObject, CMonsterGuageDlg* pGuageDlg)
{
	CNpc* pNpc = ((CNpc*)pObject) ;

	DWORD dwSelectedID = m_SelectedObjectID ;

	pGuageDlg->ResetDlg() ;
//	SetSelectedObjectID(0) ;

	m_SelectedObjectID = dwSelectedID ;

	
	pGuageDlg->GetMonsterGuage()->SetActive( FALSE ) ;

#ifdef _DEBUG
	WINDOWMGR->GetDebugString()->SetNpc(pNpc) ;
#endif //_DEBUG

	WORD npcKind = pNpc->GetNpcKind();

	if( npcKind == 30 || npcKind == 31 || npcKind == 0 )
	{
		pGuageDlg->SetActive( FALSE ) ;
	}
	else
	{
		pGuageDlg->SetActive( TRUE ) ;
	}

	pGuageDlg->GetGuildStatic()->SetActive(FALSE) ;
	pGuageDlg->SetGuildName("", "") ;
}

void CObjectManager::SelectedPet(CObject* pObject, CMonsterGuageDlg* pGuageDlg)
{
	CPet* pPet = ((CPet*)pObject) ;

	DWORD dwSelectedID = m_SelectedObjectID ;

	pGuageDlg->SetActive( TRUE ) ;
	pGuageDlg->ResetDlg() ;
//	SetSelectedObjectID(0) ;

	m_SelectedObjectID = dwSelectedID ;

	pGuageDlg->GetMasterStatic()->SetActive( TRUE );
	pGuageDlg->GetMonsterGuage()->SetActive( FALSE ) ;

	pGuageDlg->GetGuildStatic()->SetActive(FALSE) ;
	pGuageDlg->SetGuildName("", "") ;
}

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.08.22
void CObjectManager::SelectedFarmObj(CObject* pObject, CMonsterGuageDlg* pGuageDlg)
{
 	// 집사 대화창이 떠 있으면 리턴~
   	if (g_csFarmManager.CLI_GetTargetObj() && 
 		GAMEIN->GetNpcScriptDialog()->IsActive())
	{
		return;
	}

	if (g_csFarmManager.CLI_GetTargetObj() &&
		GAMEIN->GetAnimalDialog()->IsActive())
	{
		return;
	}

	CSHFarmRenderObj* pFarmObj = (CSHFarmRenderObj*)pObject;

	DWORD dwSelectedID = m_SelectedObjectID ;

	pGuageDlg->ResetDlg() ;
//	SetSelectedObjectID(0) ;

	m_SelectedObjectID = dwSelectedID ;
	
//	pGuageDlg->GetMonsterGuage()->SetActive( FALSE ) ;
	pGuageDlg->SetActive( TRUE ) ;

	pGuageDlg->GetGuildStatic()->SetActive(FALSE) ;
	pGuageDlg->SetGuildName("", "") ;

	g_csFarmManager.CLI_SetTargetObj((CSHFarmRenderObj*)pObject);
}
// E 농장시스템 추가 added by hseos 2007.08.22

void CObjectManager::SelectedMonster(CObject* pObject, CMonsterGuageDlg* pGuageDlg)
{
	pGuageDlg->ResetDlg() ;
	pGuageDlg->SetActive( TRUE ) ;

	pGuageDlg->GetHPStatic()->SetActive( TRUE ) ;
	pGuageDlg->GetMonsterGuage()->SetActive( TRUE ) ;
	pGuageDlg->SetGuage( pObject ) ;
	pGuageDlg->GetGuildStatic()->SetActive(FALSE) ;
	pGuageDlg->SetGuildName("", "") ;
}

/*
void CObjectManager::SetSelectedObject(CObject* pObject)
{
	CObject* pLastSelectedObject = GetSelectedObject();
	if( pLastSelectedObject == pObject ) return;

	if( m_SelectEffectHandle )
	{
		EFFECTMGR->ForcedEndEffect( m_SelectEffectHandle );
	}

	if( pObject )
	{
		m_SelectEffectHandle = EFFECTMGR->StartEffectProcess(364,pObject,NULL,0,0);
	}

#ifdef _CHEATENABLE_
	//if(CHEATMGR->IsCheatEnable())
	{
		CHEATMGR->SetCheatTargetObject(pObject);
	}
#endif

	if( pLastSelectedObject && pLastSelectedObject->IsInited() )
	{
		if(( CanOverInfoChange( pLastSelectedObject ) && !m_bAllNameShow ) || ( pLastSelectedObject->GetObjectKind() & eObjectKind_Monster ))// && ( pLastSelectedObject->GetObjectKind() != eObjectKind_Npc) )
			pLastSelectedObject->ShowObjectName( FALSE );
		else
			pLastSelectedObject->ShowObjectName( TRUE, GetObjectNameColor(pLastSelectedObject) );		
		
		pLastSelectedObject->OnDeselected();
	}

	if( pObject )
	{
		// 070427 LYW --- ObjectManager : Add for test.
#ifdef _DEBUG
		if( pObject->GetObjectKind() == eObjectKind_Npc )
		{
			WINDOWMGR->GetDebugString()->SetNpc(((CNpc*)pObject)) ;
		}
#endif //_DEBUG

		m_SelectedObjectID = pObject->GetID();
		pObject->OnSelected();

		// 070123 LYW --- Active guage dialog.
 		CMonsterGuageDlg* pGuageDlg = GAMEIN->GetMonsterGuageDlg() ;

		if( pGuageDlg )
		{
			BYTE objectKind = pObject->GetObjectKind() ;
			WORD npcKind = ((CNpc*)pObject)->GetNpcKind();

			pGuageDlg->SetCurMode( objectKind ) ;

			if( npcKind == 30 || npcKind == 31 || npcKind == 0 )
			{
				pGuageDlg->SetActive( FALSE ) ;
				return ;
			}
			else
			{
				// 070418 LYW --- ObjectManager : Resetting monster dialog.
				pGuageDlg->ResetDlg() ;

				CParty* pParty = PARTYMGR->GetParty() ;
				
				if( pParty->IsPartyMember(pObject->GetID()) )
				{
					if( ((CPlayer*)pObject)->GetGuildIdx() )
					{
						pGuageDlg->GetGuildStatic()->SetActive( FALSE ) ;
					}
				}

				pGuageDlg->SetActive( TRUE ) ;
			}

			if( objectKind == eObjectKind_Player )
			{
				// 070125 LYW --- Delete this line for a moment.
				//pGuageDlg->SetFace() ;

				pGuageDlg->SetName( pObject, pObject->GetObjectName() ) ; 
				// 070424 LYW --- ObjectManager : Check party play.
				CParty* pParty = PARTYMGR->GetParty() ;
				if( !pParty->IsPartyMember((((CPlayer*)pObject)->GetID())) )
				{
					// 070426 LYW --- ObjectManager : Modified setting gild name.
					//pGuageDlg->SetGuildName( ((CPlayer*)pObject )->GetGuildName(), ((CPlayer*)pObject)->GetGuildUnionName() ) ;
					if( !((CPlayer*)pObject )->GetGuildIdx() )
					{
						pGuageDlg->SetGuildName("", "") ;
					}
					else
					{
						pGuageDlg->SetGuildName( ((CPlayer*)pObject )->GetGuildName(), ((CPlayer*)pObject)->GetGuildUnionName() ) ;
					}
				}
				else
				{
					pGuageDlg->SetGuildName("", "") ;
				}

				// If same party idx.
				// 070315 LYW --- ObjectManager : Modified function CheckTargetKind.
				//pGuageDlg->CheckTargetKind() ;
				pGuageDlg->CheckTargetKind(pObject) ;
				if( pGuageDlg->GetTargetState() == e_MEMBER )
				{
					// 070315 LYW --- ObjectManager : Modified parameter of SetLife function.
					//pGuageDlg->SetLife( ((CMonster*)pObject) ) ;
					// 070424 LYW --- ObjectManager : Setting life and mana.
				}
				else
				{			
					pGuageDlg->GetHPStatic()->SetActive( FALSE ) ;
					pGuageDlg->GetMonsterGuage()->SetActive( FALSE ) ;
					pGuageDlg->GetPlayerGuage()->SetActive( FALSE ) ;
				}

				pGuageDlg->GetMenuButton()->SetActive( TRUE ) ;

				pGuageDlg->CheckTargetAtt( ((CPlayer*)pObject ) ) ;
			}
			else
			{
				// 070125 LYW --- Delete this line for a moment.
				//pGuageDlg->SetFace() ;

				pGuageDlg->SetName( pObject, pObject->GetObjectName() ) ;

				pGuageDlg->GetGuildStatic()->SetActive( FALSE ) ;
				pGuageDlg->GetUnionStatic()->SetActive( FALSE ) ;

				pGuageDlg->GetLifeStatic()->SetActive( FALSE ) ;

				pGuageDlg->GetMenuButton()->SetActive( FALSE ) ;

				pGuageDlg->SetGuage( pObject ) ;
			}
		}
	}
	else
	{
		SetSelectedObjectID(0);
		GAMEIN->GetPartyDialog()->SetClickedMemberID(0);
	}
}
*/

void CObjectManager::SetOverInfoOption( DWORD dwOption )
{
	m_dwOverInfoOption = dwOption;
	ApplyOverInfoOptionToAll();
}

void CObjectManager::ApplyOverInfoOptionToAll()
{
	CObject* pObject;
	m_ObjectTable.SetPositionHead();
	while( pObject = m_ObjectTable.GetData() )
	{
		ApplyOverInfoOption( pObject );
	}
}

void CObjectManager::ApplyOverInfoOption( CObject* pObject )
{
	if( pObject == NULL ) return;																	// 들어온 오브젝트 정보가 유효한지 체크한다.

	pObject->SetOverInfoOption( 1 );																// 이름, 채팅, 상점 정보를 활성화 시킨다.
	
	if( pObject->GetObjectKind() == eObjectKind_Player )											// 오브젝트 타입이 플레이어 이면,
	{
#ifndef _GMTOOL_																					// GM 툴인지 체크한다.

		if( ((CPlayer*)pObject)->GetCharacterTotalInfo()->bVisible == FALSE )						// 플레이어의 캐릭터 정보가 HIDE 이면,
		{
			pObject->ShowObjectName( FALSE );														// 이름을 비활성화 시킨다.

			return;																					// 리턴 처리를 한다.
		}
#endif																								// GM 툴 처리 종료.

		if( GUILDWARMGR->IsEnemy( (CPlayer*)pObject ) )											// 길드 필드 전쟁의 적이면,
		{
			pObject->ShowObjectName( TRUE, NAMECOLOR_PKMODE );										// PK모드로 이름을 활성화 한다.

			return;																					// 리턴 처리를 한다.
		}
	}

	if( GetSelectedObject() != pObject )															// 선택된 오브젝트 정보와 들어온 오브젝트 정보가 같지 않으면,
	{
		if( pObject->GetObjectKind() == eObjectKind_Player )										// 오브젝트 타입이 플레이어와 같으면,
		{
			if( WANTEDMGR->IsChrIDInList( pObject->GetID() ) )										// 현상수배범 이라면,
			{
				pObject->ShowObjectName( TRUE, NAMECOLOR_WANTED );									// 현상수배로 이름을 활성화 한다.

				return;																				// 리턴 처리를 한다.
			}

			if( ((CPlayer*)pObject)->IsPKMode() || ((CPlayer*)pObject)->IsRestraintMode() )			// 플레이어가 결투모드거나, RestraintMode이면,
			{
				pObject->ShowObjectName( TRUE, NAMECOLOR_PKMODE );									// 결투모드로 이름을 활성화 한다.

				return;																				// 리턴 처리를 한다.
			}

			if( m_dwOverInfoOption & eOO_NAME_PARTY || m_bAllNameShow )								// 파티원 이름표시 옵션이거나, 다른 캐릭터 이름 표시 모드이면,
			{
				if( PARTYMGR->IsPartyMember( pObject->GetID() ) )									// 플레이어가 파티원이라면,
				{
					pObject->ShowObjectName( TRUE, NAMECOLOR_PARTY );								// 파티 이름으로 활성화 한다.

					return;																			// 리턴 처리를 한다.
				}
			}
			if( m_dwOverInfoOption & eOO_NAME_MUNPA || m_bAllNameShow )								// 길드원 이름표시 옵션이거나, 다른 캐릭터 이름 표시 모드이면,
			{
				if( HERO )																			// 히어로 정보가 유효한지 체크한다.
				{
					if( HERO->GetGuildIdx() != 0 )													// 히어로의 길드 인덱스가 유효한지 체크한다.
					{
						if( HERO->GetGuildIdx() == ((CPlayer*)pObject)->GetGuildIdx())				// 플레이어의 길드인덱스와 히어로의 길드 인덱스가 같다면,
						{
							pObject->ShowObjectName( TRUE, NAMECOLOR_MUNPA );						// 길드 이름으로 활성화 한다.

							if(((CPlayer*)pObject)->GetGuildMarkName() > 0)							// 길드 마크명이 있다면,
							{
								pObject->SetGuildMark();											// 길드 마크클 세팅한다.
							}

							return;																	// 리턴 처리를 한다.
						}
					}
				}
			}

			if( ( m_dwOverInfoOption & eOO_NAME_WHOLE ) || m_bAllNameShow )							// 다른 캐릭터 이름표시 옵션이거나, 이름을 모두 표시 상태이면,
			{
				pObject->ShowObjectName( TRUE );													// 플레이어의 이름을 활성화 한다.

				return;																				// 리턴 처리를 한다.
			}	
			
			pObject->ShowObjectName( FALSE );														// 오브젝트의 이름 보이기 여부를 FALSE로 세팅한다.
		}
		else if( pObject->GetObjectKind() == eObjectKind_Npc )										// 오브젝트가 NPC와 같으면,
		{
			if( m_bAllNameShow )																	// 이름을 보여주는 모드이면,
			{
				pObject->ShowObjectName( TRUE );													// 오브젝트 이름을 활성화 한다.
			}
			else																					// 이름을 보여주는 모드가 아니면,
			{
				pObject->ShowObjectName( FALSE );													// 오브젝트 이름을 비활성화 한다.
			}
		}
	}
	else																							// 선택된 오브젝트와 들어온 오브젝트 정보가 같으면,
	{
		pObject->ShowObjectName( TRUE, NAMECOLOR_SELECTED );										// 선택된 모드로 이름을 설정한다.
	}

	CNpc* pNpc = NULL ;																				// NPC 정보를 받을 포인터를 선언한다.

	pNpc = ( CNpc* )pObject ;																		// NPC 정보를 받는다.

	if( pNpc )																						// NPC 정보가 유효한지 체크한다.
	{
		if( pNpc->GetNpcKind() == 31 )																// NPC 종류가 31번과 같으면,
		{
			pObject->ShowObjectName( TRUE ) ;														// 이름을 활성화 한다.
		}
	}
}

//void CObjectManager::ApplyOverInfoOption( CObject* pObject )
//{
//	if( pObject == NULL ) return;																	
//	//overinfo on/off
//	pObject->SetOverInfoOption( 1 );	//아직 옵션이 없다.
//	
//	//overinfo show/hide
//	if( pObject->GetObjectKind() == eObjectKind_Player )
//	{
//#ifndef _GMTOOL_
//		if( ((CPlayer*)pObject)->GetCharacterTotalInfo()->bVisible == FALSE )//hide된 유닛 이름 안찍기.
//		{
//			pObject->ShowObjectName( FALSE );
//			return;
//		}
//#endif
////		if( ((CPlayer*)pObject)->IsPKMode() )
////		{
////			pObject->ShowObjectName( TRUE, NAMECOLOR_PKMODE );
////			return;
////		}
//		// guildfieldwar
//		if( GUILDWARMGR->IsEnemy( (CPlayer*)pObject ) )
//		{
//			pObject->ShowObjectName( TRUE, NAMECOLOR_PKMODE );
//			return;
//		}
//	}
//
//	if( GetSelectedObject() != pObject )
//	{
//		if( pObject->GetObjectKind() == eObjectKind_Player )
//		{
//			if( TRUE/*pObject != HERO || m_bAllNameShow*/ )
//			{
//				if( WANTEDMGR->IsChrIDInList( pObject->GetID() ) )
//				{
//					pObject->ShowObjectName( TRUE, NAMECOLOR_WANTED );
//					return;
//				}
//
//				if( ((CPlayer*)pObject)->IsPKMode() || ((CPlayer*)pObject)->IsRestraintMode() )
//				{
//					pObject->ShowObjectName( TRUE, NAMECOLOR_PKMODE );
//					return;
//				}
//
//				if( m_dwOverInfoOption & eOO_NAME_PARTY || m_bAllNameShow )
//				{
//					if( PARTYMGR->IsPartyMember( pObject->GetID() ) )
//					{
//						pObject->ShowObjectName( TRUE, NAMECOLOR_PARTY );
//						return;
//					}
//				}
//				if( m_dwOverInfoOption & eOO_NAME_MUNPA || m_bAllNameShow )
//				{
//					if( HERO )
//					if( HERO->GetGuildIdx() != 0 )
//					if( HERO->GetGuildIdx() == ((CPlayer*)pObject)->GetGuildIdx())
//					{
//						pObject->ShowObjectName( TRUE, NAMECOLOR_MUNPA );
//						if(((CPlayer*)pObject)->GetGuildMarkName() > 0)
//							pObject->SetGuildMark();
//						return;
//					}
//				}
//				if( ( m_dwOverInfoOption & eOO_NAME_WHOLE ) || m_bAllNameShow )
//				{
//					pObject->ShowObjectName( TRUE );
//					return;
//				}				
//			}
//			
//			pObject->ShowObjectName( FALSE );
//		}
//		else if( pObject->GetObjectKind() == eObjectKind_Npc )
//		{
//			if( m_bAllNameShow )
//				pObject->ShowObjectName( TRUE );
//			else
//				pObject->ShowObjectName( FALSE );
//			
//			//pObject->ShowObjectName( TRUE, NAMECOLOR_NPC );
//		}
//	}
//	else
//	{
//		pObject->ShowObjectName( TRUE, NAMECOLOR_SELECTED );		
//	}
//
//	// 070121 LYW --- Check npc kind.
//	CNpc* pNpc = ( CNpc* )pObject ;
//	if( pNpc->GetNpcKind() == 31 )
//	{
//		pObject->ShowObjectName( TRUE ) ;
//	}
//}

void CObjectManager::SetAllNameShow( BOOL bShow )
{ 
	if( m_bAllNameShow == bShow )
		return;

	m_bAllNameShow = bShow;
	
//	if( !( m_dwOverInfoOption & eOO_NAME_WHOLE ) )
	ApplyOverInfoOptionToAll();
}

void CObjectManager::OnMouseOver( CObject* pObject )
{
	if( pObject->GetID() != m_SelectedObjectID )
	if( !m_bAllNameShow || pObject->GetObjectKind() & eObjectKind_Monster )
	if( CanOverInfoChange( pObject ) )
	{
		//if(	!(pObject->GetObjectKind() & eObjectKind_Npc) )
			pObject->ShowObjectName( TRUE );
	}
}

void CObjectManager::OnMouseLeave( CObject* pObject )
{
	if( pObject->GetID() != m_SelectedObjectID )
	if( !m_bAllNameShow || pObject->GetObjectKind() & eObjectKind_Monster )
	if( CanOverInfoChange( pObject ) )
	{
		//if(	!(pObject->GetObjectKind() & eObjectKind_Npc) )
			pObject->ShowObjectName( FALSE );
	}
}

BOOL CObjectManager::CanOverInfoChange( CObject* pObject )
{
	if( pObject->GetObjectKind() == eObjectKind_Player )
	{
		if( WANTEDMGR->IsChrIDInList( pObject->GetID() ) ) return FALSE;
		if( ((CPlayer*)pObject)->IsPKMode() )		return FALSE;	
		if( ((CPlayer*)pObject)->IsRestraintMode() )	return FALSE;		// 공성견제
		if( m_dwOverInfoOption & eOO_NAME_WHOLE )	return FALSE;

		if( m_dwOverInfoOption & eOO_NAME_PARTY )
		if( PARTYMGR->IsPartyMember( pObject->GetID() ) )
			return FALSE;

		if( m_dwOverInfoOption & eOO_NAME_MUNPA )
		if( HERO )
		if( HERO->GetGuildIdx() != 0 )
		if( HERO->GetGuildMemberRank() > MUNPA_SEMIMEMBER )
		if( HERO->GetGuildIdx() == ((CPlayer*)pObject)->GetGuildIdx() && ((CPlayer*)pObject)->GetGuildMemberRank() > MUNPA_SEMIMEMBER )
			return FALSE;

		// guildfieldwar
		if( GUILDWARMGR->IsEnemy( (CPlayer*)pObject ) )
			return FALSE;
	}

	return TRUE;
}

DWORD CObjectManager::GetObjectNameColor( CObject* pObject )
{
	if( pObject->GetObjectKind() == eObjectKind_Player )
	{
		if( strncmp( pObject->GetObjectName(), "GM", 2 ) == 0 )
			return NAMECOLOR_GM;
		if( WANTEDMGR->IsChrIDInList( pObject->GetID() ) )
			return NAMECOLOR_WANTED;
		if( ((CPlayer*)pObject)->IsPKMode() || ((CPlayer*)pObject)->IsRestraintMode() )
			return NAMECOLOR_PKMODE;

		if( m_dwOverInfoOption & eOO_NAME_PARTY )
		if( PARTYMGR->IsPartyMember( pObject->GetID() ) )
			return NAMECOLOR_PARTY;

		if( m_dwOverInfoOption & eOO_NAME_MUNPA )
		if( HERO )
		if( HERO->GetGuildIdx() )
		if( HERO->GetGuildIdx() == ((CPlayer*)pObject)->GetGuildIdx())
			return NAMECOLOR_MUNPA;	

		if( GUILDWARMGR->IsEnemy( (CPlayer*)pObject ) )
			return NAMECOLOR_PKMODE;

		return NAMECOLOR_PLAYER;
	}
	/*
	else if( pObject->GetObjectKind() == eObjectKind_Npc )
	{
		return NAMECOLOR_NPC;
	}
	*/

	return NAMECOLOR_DEFAULT;
}

void CObjectManager::SetShadowOption( DWORD dwOption )
{
	m_dwShadowOption = dwOption;
	ApplyShadowOptionToAll();
}

void CObjectManager::ApplyShadowOptionToAll()
{
	CObject* pObject;
	m_ObjectTable.SetPositionHead();
	while( pObject = m_ObjectTable.GetData() )
	{
		ApplyShadowOption( pObject );
	}
}

void CObjectManager::ApplyShadowOption( CObject* pObject )
{
	// 070430 LYW --- ObjectManager : Deactive shadow to gate npc.
	if( ((CNpc*)pObject)->GetNpcKind() == 31 )
	{
		RemoveShadow( pObject );
		return ;
	}

	if( pObject == NULL ) return;

//Shadow on/off
	//임시로 끌수 있도록(공성전등)

//Shadow show/hide
	if( m_dwShadowOption & eOO_SHADOW_OTHERCHARACTER )
	{
		if( pObject->GetObjectKind() == eObjectKind_Player )
		if( pObject != HERO )
		{
			AddShadow( pObject );
			return;
		}
	}

	if( m_dwShadowOption & eOO_SHADOW_ONLYHERO  )
	{
		if( pObject == HERO )
		{
			AddShadow( pObject );
			return;
		}
	}

	if( m_dwShadowOption & eOO_SHADOW_MONSTER )
	{
		if( ( pObject->GetObjectKind() & eObjectKind_Monster ) || ( pObject->GetObjectKind() == eObjectKind_Npc ) )
		{
#ifdef _GMTOOL_
			if( MAINGAME->GetUserLevel() <= eUSERLEVEL_GM  && CAMERA->GetCameraMode() != eCM_EyeView )
			{
				if( 1.0f > pObject->GetEngineObject()->GetAlpha() )
				{
					RemoveShadow( pObject );
				}
				else
				{
					AddShadow( pObject );
				}
			}
			else
#endif

			// 080305 ggomgrak --- Visible로 체크하면 컬링된 오브젝트는 반영이 안되는 버그가 있으므로 주석처리함.
			//if( pObject->GetEngineObject()->IsVisible() )
			{
				AddShadow( pObject );
			}
			//else
			//{
			//	RemoveShadow( pObject );
			//}

			return;
		}

	}

	RemoveShadow( pObject );
}

void CObjectManager::ShowChatBalloon( CObject* pObject, char* pStrChat, DWORD dwColor, DWORD dwAliveTime )
{
	if( m_dwOverInfoOption & eOO_CHAT_NOBALLOON )
		return;

	if( !WINDOWMGR->IsOpendAllWindows() )
		return;

	if( pObject )
		pObject->ShowChatBalloon( TRUE, pStrChat, dwColor, dwAliveTime );
}

void CObjectManager::HideChatBalloonAll()
{
	CObject* pObject;
	
	m_ObjectTable.SetPositionHead();
	while( pObject = m_ObjectTable.GetData() )
	{
		pObject->ShowChatBalloon( FALSE, NULL );
	}
}

void CObjectManager::UpdateTargetList_Monster(cSkillObject* pSkillObject)
{
	
	CObject* pObject;
	
	m_ObjectTable.SetPositionHead();

	while(pObject = m_ObjectTable.GetData())
	{
		if((pObject->GetObjectKind() & eObjectKind_Monster) == FALSE)
			continue;

//		pSkillObject->UpdateTargetList(pObject);
	}
}

void CObjectManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol) 
	{
	case MP_BOSS_REST_START_NOTIFY:
		{
			MSG_DWORD * pmsg = (MSG_DWORD *)pMsg;
			switch(pmsg->dwData)
			{
			case eBOSSKIND_TARINTUS:
				{
					OBJECTSTATEMGR->StartObjectState(m_pBoss[eBOSSTYPE_TARINTUS], eObjectState_Rest);
					TARGETSET set;
					set.pTarget = m_pBoss[eBOSSTYPE_TARINTUS];
					m_hRestHandle[eBOSSTYPE_TARINTUS] = EFFECTMGR->StartEffectProcess(
						FindEffectNum("Boss_Tarintus_Recover.beff"), 
						m_pBoss[eBOSSTYPE_TARINTUS], NULL, 0, m_pBoss[eBOSSTYPE_TARINTUS]->GetID());
				}
				break;
/*			case eBOSSKIND_KIERRA:
				{
					OBJECTSTATEMGR->StartObjectState(m_pBoss[eBOSSTYPE_KIERRA], eObjectState_Rest);
					TARGETSET set;
					set.pTarget = m_pBoss[eBOSSTYPE_KIERRA];
					m_hRestHandle[eBOSSTYPE_KIERRA] = EFFECTMGR->StartEffectProcess(
						FindEffectNum("Boss_Tarintus_Recover.beff"), 
						m_pBoss[eBOSSTYPE_KIERRA], NULL, 0, m_pBoss[eBOSSTYPE_KIERRA]->GetID());
				}
				break;*/
/*			case eBOSSKIND_EMPERIORTOMB:
			case eBOSSKIND_EMPERIORTOMB_EVENT:	
				{
					OBJECTSTATEMGR->StartObjectState(m_pBoss[eBOSSTYPE_EMPERIORTOMB], eObjectState_Rest);
					TARGETSET set;
					set.pTarget = m_pBoss[eBOSSTYPE_EMPERIORTOMB];
					m_hRestHandle[eBOSSTYPE_EMPERIORTOMB] = EFFECTMGR->StartEffectProcess(
						FindEffectNum("Boss001_summon.beff"), 
						m_pBoss[eBOSSTYPE_EMPERIORTOMB], NULL, 0, m_pBoss[eBOSSTYPE_EMPERIORTOMB]->GetID());
				}
				break;
			case eBOSSKIND_MOOUN:
				{
//					OBJECTSTATEMGR->StartObjectState(m_pBoss[eBOSSTYPE_MOOUN], eObjectState_Rest);
//					TARGETSET set;
//					set.pTarget = m_pBoss[eBOSSTYPE_MOOUN];
//					m_hRestHandle[eBOSSTYPE_MOOUN] = EFFECTMGR->StartEffectProcess(
//						FindEffectNum("Boss001_summon.beff"), 
//						m_pBoss[eBOSSTYPE_MOOUN], NULL, 0, m_pBoss[eBOSSTYPE_MOOUN]->GetID());
				}
				break;
			case eBOSSKIND_COWF:
				{
//					OBJECTSTATEMGR->StartObjectState(m_pBoss[eBOSSTYPE_COWF], eObjectState_Rest);
//					TARGETSET set;
//					set.pTarget = m_pBoss[eBOSSTYPE_COWF];
//					m_hRestHandle[eBOSSTYPE_COWF] = EFFECTMGR->StartEffectProcess(
//						FindEffectNum("Boss001_summon.beff"), 
//						m_pBoss[eBOSSTYPE_COWF], NULL, 0, m_pBoss[eBOSSTYPE_COWF]->GetID());
				}
				break;
			case eBOSSKIND_COWL:
				{
//					OBJECTSTATEMGR->StartObjectState(m_pBoss[eBOSSTYPE_COWL], eObjectState_Rest);
//					TARGETSET set;
//					set.pTarget = m_pBoss[eBOSSTYPE_COWL];
//					m_hRestHandle[eBOSSTYPE_COWL] = EFFECTMGR->StartEffectProcess(
//						FindEffectNum("Boss001_summon.beff"), 
//						m_pBoss[eBOSSTYPE_COWL], NULL, 0, m_pBoss[eBOSSTYPE_COWL]->GetID());
				}
				break;
			case eBOSSKIND_COWKING:
				{
					OBJECTSTATEMGR->StartObjectState(m_pBoss[eBOSSTYPE_COWKING], eObjectState_Rest);
					TARGETSET set;
					set.pTarget = m_pBoss[eBOSSTYPE_COWKING];
					m_hRestHandle[eBOSSTYPE_COWKING] = EFFECTMGR->StartEffectProcess(
						FindEffectNum("M184_attack9.beff"), 
						m_pBoss[eBOSSTYPE_COWKING], NULL, 0, m_pBoss[eBOSSTYPE_COWKING]->GetID());
				}
				break;*/

			}
		}	
		break;
/*
			case MP_BOSS_REST_END_NOTIFY:
				{
					MSG_DWORD * pmsg = (MSG_DWORD *)pMsg;
					switch(pmsg->dwData)
					{
					case eBOSSKIND_EMPERIORTOMB:
						{
							OBJECTSTATEMGR->EndObjectState(m_pBoss[eBOSSTYPE_EMPERIORTOMB], eObjectState_Rest, 5000);
							EFFECTMGR->ForcedEndEffect(m_hRestHandle);
						}
						break;
					}
					
				}
				break;*/
		
	case MP_BOSS_RECALL_NOTIFY:
		{
			MSG_DWORD * pmsg = (MSG_DWORD *)pMsg;
			switch(pmsg->dwData)
			{
			case eBOSSKIND_TARINTUS:
				{
					OBJECTSTATEMGR->StartObjectState(m_pBoss[eBOSSTYPE_TARINTUS], eObjectState_Rest);
					TARGETSET set;
					set.pTarget = m_pBoss[eBOSSTYPE_TARINTUS];
					m_hRestHandle[eBOSSTYPE_TARINTUS] = EFFECTMGR->StartEffectProcess(
						FindEffectNum("BOSS_Tarintus_Summon.beff"), 
						m_pBoss[eBOSSTYPE_TARINTUS], NULL, 0, m_pBoss[eBOSSTYPE_TARINTUS]->GetID());
				}
				break;
			case eBOSSKIND_KIERRA:
				{
					OBJECTSTATEMGR->StartObjectState(m_pBoss[eBOSSTYPE_KIERRA], eObjectState_Rest);
					TARGETSET set;
					set.pTarget = m_pBoss[eBOSSTYPE_KIERRA];
					m_hRestHandle[eBOSSTYPE_TARINTUS] = EFFECTMGR->StartEffectProcess(
						FindEffectNum("BOSS_Kierra_Summon.beff"), 
						m_pBoss[eBOSSTYPE_KIERRA], NULL, 0, m_pBoss[eBOSSTYPE_KIERRA]->GetID());
				}
				break;
/*			case eBOSSKIND_EMPERIORTOMB:
			case eBOSSKIND_EMPERIORTOMB_EVENT:
				{
					OBJECTSTATEMGR->StartObjectState(m_pBoss[eBOSSTYPE_EMPERIORTOMB], eObjectState_Rest); // same ani
					TARGETSET set;
					set.pTarget = m_pBoss[eBOSSTYPE_EMPERIORTOMB];
					EFFECTMGR->StartEffectProcess(
						FindEffectNum("Boss001_summon.beff")
						, m_pBoss[eBOSSTYPE_EMPERIORTOMB], &set, 0, m_pBoss[eBOSSTYPE_EMPERIORTOMB]->GetID());
				}
				break;
			case eBOSSKIND_MOOUN:
				{
//					OBJECTSTATEMGR->StartObjectState(m_pBoss[eBOSSTYPE_MOOUN], eObjectState_Rest); // same ani
//					TARGETSET set;
//					set.pTarget = m_pBoss[eBOSSTYPE_MOOUN];
//					EFFECTMGR->StartEffectProcess(
//						FindEffectNum("Boss001_summon.beff")
//						, m_pBoss[eBOSSTYPE_MOOUN], &set, 0, m_pBoss[eBOSSTYPE_MOOUN]->GetID());
				}
				break;
			case eBOSSKIND_COWF:
				{
//					OBJECTSTATEMGR->StartObjectState(m_pBoss[eBOSSTYPE_COWF], eObjectState_Rest); // same ani
//					TARGETSET set;
//					set.pTarget = m_pBoss[eBOSSTYPE_COWF];
//					EFFECTMGR->StartEffectProcess(
//						FindEffectNum("Boss001_summon.beff")
//						, m_pBoss[eBOSSTYPE_COWF], &set, 0, m_pBoss[eBOSSTYPE_COWF]->GetID());
				}
				break;
			case eBOSSKIND_COWL:
				{
//					OBJECTSTATEMGR->StartObjectState(m_pBoss[eBOSSTYPE_COWL], eObjectState_Rest); // same ani
//					TARGETSET set;
//					set.pTarget = m_pBoss[eBOSSTYPE_COWL];
//					EFFECTMGR->StartEffectProcess(
//						FindEffectNum("Boss001_summon.beff")
//						, m_pBoss[eBOSSTYPE_COWL], &set, 0, m_pBoss[eBOSSTYPE_COWL]->GetID());
				}
				break;
			case eBOSSKIND_COWKING:
				{
					OBJECTSTATEMGR->StartObjectState(m_pBoss[eBOSSTYPE_COWKING], eObjectState_Rest); // same ani
					TARGETSET set;
					set.pTarget = m_pBoss[eBOSSTYPE_COWKING];
					EFFECTMGR->StartEffectProcess(
						FindEffectNum("M184_attack10.beff")
						, m_pBoss[eBOSSTYPE_COWKING], &set, 0, m_pBoss[eBOSSTYPE_COWKING]->GetID());
				}
				break;*/
			}
		}
		break;

		case MP_BOSS_LIFE_NOTIFY:
			{
				MSG_DWORD2 * pmsg = (MSG_DWORD2 *)pMsg;
				switch(pmsg->dwData2)
				{
				case eBOSSKIND_TARINTUS:
					{
						m_pBoss[eBOSSTYPE_TARINTUS]->SetLife(pmsg->dwData1);
					}
					break;
				case eBOSSKIND_KIERRA:
					{
						m_pBoss[eBOSSTYPE_KIERRA]->SetLife(pmsg->dwData1);
					}
					break;
/*				case eBOSSKIND_EMPERIORTOMB:
				case eBOSSKIND_EMPERIORTOMB_EVENT:
					{
						m_pBoss[eBOSSTYPE_EMPERIORTOMB]->SetLife(pmsg->dwData1);
					}
					break;
				case eBOSSKIND_MOOUN:
					{
						m_pBoss[eBOSSTYPE_MOOUN]->SetLife(pmsg->dwData1);
					}
					break;
				case eBOSSKIND_COWF:
					{
						m_pBoss[eBOSSTYPE_COWF]->SetLife(pmsg->dwData1);
					}
					break;
				case eBOSSKIND_COWL:
					{
						m_pBoss[eBOSSTYPE_COWL]->SetLife(pmsg->dwData1);
					}
					break;
				case eBOSSKIND_COWKING:
					{
						m_pBoss[eBOSSTYPE_COWKING]->SetLife(pmsg->dwData1);
					}
					break;*/
				}
			}
		break;
		case MP_BOSS_STAND_NOTIFY:
			{
				MSG_DWORD * pmsg = (MSG_DWORD *)pMsg;
				switch(pmsg->dwData)
				{
				case eBOSSKIND_TARINTUS:
					{
/*						TARGETSET set;
						set.pTarget = m_pBoss[eBOSSTYPE_TARINTUS];
						EFFECTMGR->StartEffectProcess(
							FindEffectNum("BOSS_Tarintus_Regen.beff")
							, m_pBoss[eBOSSTYPE_TARINTUS], &set, 0, m_pBoss[eBOSSTYPE_TARINTUS]->GetID());*/
					}
					break;
/*				case eBOSSKIND_EMPERIORTOMB:
				case eBOSSKIND_EMPERIORTOMB_EVENT:
					{						
						TARGETSET set;
						set.pTarget = m_pBoss[eBOSSTYPE_EMPERIORTOMB];
						EFFECTMGR->StartEffectProcess(
							FindEffectNum("Boss001_Stand.beff")
							, m_pBoss[eBOSSTYPE_EMPERIORTOMB], &set, 0, m_pBoss[eBOSSTYPE_EMPERIORTOMB]->GetID());
					}
					break;
				case eBOSSKIND_MOOUN:
					{
//						TARGETSET set;
//						set.pTarget = m_pBoss[eBOSSTYPE_MOOUN];
//						EFFECTMGR->StartEffectProcess(
//							FindEffectNum("Boss001_Stand.beff")
//							, m_pBoss[eBOSSTYPE_MOOUN], &set, 0, m_pBoss[eBOSSTYPE_MOOUN]->GetID());
					}
					break;
				case eBOSSKIND_COWF:
					{
//						TARGETSET set;
//						set.pTarget = m_pBoss[eBOSSTYPE_COWF];
//						EFFECTMGR->StartEffectProcess(
//							FindEffectNum("Boss001_Stand.beff")
//							, m_pBoss[eBOSSTYPE_COWF], &set, 0, m_pBoss[eBOSSTYPE_COWF]->GetID());
					}
					break;
				case eBOSSKIND_COWL:
					{
//						TARGETSET set;
//						set.pTarget = m_pBoss[eBOSSTYPE_COWL];
//						EFFECTMGR->StartEffectProcess(
//							FindEffectNum("Boss001_Stand.beff")
//							, m_pBoss[eBOSSTYPE_COWL], &set, 0, m_pBoss[eBOSSTYPE_COWL]->GetID());
					}
					break;
				case eBOSSKIND_COWKING:
					{
//						TARGETSET set;
//						set.pTarget = m_pBoss[eBOSSTYPE_COWKING];
//						EFFECTMGR->StartEffectProcess(
//							FindEffectNum("Boss001_Stand.beff")
//							, m_pBoss[eBOSSTYPE_COWKING], &set, 0, m_pBoss[eBOSSTYPE_COWKING]->GetID());
					}
					break;*/
				}				
			}
			break;
		case MP_BOSS_STAND_END_NOTIFY:
			{
				MSG_DWORD * pmsg = (MSG_DWORD *)pMsg;
				switch(pmsg->dwData)
				{
				case eBOSSKIND_TARINTUS:
					{
//						ShowObjectName( FALSE );
//						ShowChatBalloon( FALSE, NULL );

						if( m_hRestHandle[eBOSSTYPE_TARINTUS] )
							EFFECTMGR->ForcedEndEffect(m_hRestHandle[eBOSSTYPE_TARINTUS]);
					}
					break;
				case eBOSSKIND_KIERRA:
					{
						if( m_hRestHandle[eBOSSTYPE_KIERRA] )
							EFFECTMGR->ForcedEndEffect(m_hRestHandle[eBOSSTYPE_KIERRA]);
					}
					break;
/*				case eBOSSKIND_EMPERIORTOMB:
				case eBOSSKIND_EMPERIORTOMB_EVENT:
					{						
						if(m_hRestHandle[eBOSSTYPE_EMPERIORTOMB])
							EFFECTMGR->ForcedEndEffect(m_hRestHandle[eBOSSTYPE_EMPERIORTOMB]);
					}
					break;
				case eBOSSKIND_MOOUN:
					{
						if(m_hRestHandle[eBOSSTYPE_MOOUN])
							EFFECTMGR->ForcedEndEffect(m_hRestHandle[eBOSSTYPE_MOOUN]);
					}
					break;
				case eBOSSKIND_COWF:
					{
						if(m_hRestHandle[eBOSSTYPE_COWF])
							EFFECTMGR->ForcedEndEffect(m_hRestHandle[eBOSSTYPE_COWF]);
					}
					break;
				case eBOSSKIND_COWL:
					{
						if(m_hRestHandle[eBOSSTYPE_COWL])
							EFFECTMGR->ForcedEndEffect(m_hRestHandle[eBOSSTYPE_COWL]);
					}
					break;
				case eBOSSKIND_COWKING:
					{
						if(m_hRestHandle[eBOSSTYPE_COWKING])
							EFFECTMGR->ForcedEndEffect(m_hRestHandle[eBOSSTYPE_COWKING]);
					}
					break;*/
				}				
			}
			break;

		// 필드보스 - 05.12 이영준
		// 필드보스 및 부하 생명력 회복
		case MP_FIELD_LIFE_NOTIFY:
			{
				MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
				CMonster* pMonster = (CMonster*)GetObject(pmsg->dwData3);

				if(!pMonster)
					return;

				if( pMonster->GetMonsterKind() != pmsg->dwData2 )
					return;

				pMonster->SetLife( pmsg->dwData1 );
			}
			break;
		break;
	}
}

void CObjectManager::AllPlayerPKOff()
{
	m_ObjectTable.SetPositionHead();
	CObject* pObject = NULL;
	while( pObject = m_ObjectTable.GetData() )
	{
		if( pObject->GetObjectKind() != eObjectKind_Player ) continue;
		CPlayer* pPlayer = (CPlayer*)pObject;
		if( pPlayer->IsPKMode() == FALSE ) continue;

		pPlayer->SetPKMode( FALSE );

		if( HERO )
		if( pPlayer != HERO )
		{
			HERO->ClearTarget( pPlayer );
		}
	
		pPlayer->RemoveObjectEffect( PK_EFFECT_ID );
	}
	//캐릭터 오버인포 바꾸기
	ApplyOverInfoOptionToAll();
}


void CObjectManager::SetSelectedObjectID(DWORD dwId) 
{ 
	m_SelectedObjectID = dwId;
	SetSelectedEffect( NULL );
	if(m_SelectedObjectID == 0)
	{		
		if(GAMEIN->GetMonsterGuageDlg())
			GAMEIN->GetMonsterGuageDlg()->SetActive(FALSE);
	}
}

CMapObject* CObjectManager::AddMapObject(BASEOBJECT_INFO* pBaseObjectInfo, BASEMOVE_INFO* pMoveInfo, char* strData, float fScale, float fAngle )
{
	CObject* pPreObj = GetObject( pBaseObjectInfo->dwObjectID );
	if( pPreObj )
	{
		RemoveObject( &pPreObj );
	}

	CMapObject* pMapObject = m_MapObjectPool.Alloc();
	pMapObject->Init( eObjectKind_MapObject, pBaseObjectInfo );

	pMapObject->m_RotateInfo.Angle.SetAngleDeg( fAngle );
	APPEARANCEMGR->SetMapObjectAppearance( pMapObject, strData, fScale );
	MOVEMGR->InitMove( pMapObject, pMoveInfo, fAngle );	
	OBJECTSTATEMGR->InitObjectState( pMapObject );

	m_ObjectTable.Add( pMapObject, pMapObject->GetID() );

	ApplyOverInfoOption( pMapObject );
	ApplyShadowOption( pMapObject );

	return pMapObject;
}

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.05.07
CSHFarmRenderObj* CObjectManager::AddFarmRenderObj( BASEOBJECT_INFO* pBaseObjectInfo, BASEMOVE_INFO* pMoveInfo, CSHFarmRenderObj::stFarmRenderObjInfo* pFarmRenderObjInfo)
{
	CObject* pPreObj = GetObject(pBaseObjectInfo->dwObjectID);
	ASSERT(pPreObj == NULL);
	if(pPreObj)
	{
		RemoveObject(&pPreObj);
	}

	CSHFarmRenderObj* pFarmRenderObj = m_csFarmRenderObjPool.Alloc();

	pFarmRenderObj->Init(eObjectKind_FarmObj, pBaseObjectInfo, pFarmRenderObjInfo);
	pFarmRenderObj->InitObjectBalloon(ObjectBalloon_Name);
	pFarmRenderObj->m_MoveInfo.CurPosition.x = 0;
	pFarmRenderObj->m_MoveInfo.CurPosition.y = 0;
	pFarmRenderObj->m_MoveInfo.CurPosition.z = 0;

	APPEARANCEMGR->InitAppearance(pFarmRenderObj);

	pMoveInfo->SetMaxTargetPosIdx(0);
	MOVEMGR->InitMove(pFarmRenderObj, pMoveInfo);
	OBJECTSTATEMGR->InitObjectState(pFarmRenderObj);

	m_ObjectTable.Add(pFarmRenderObj, pFarmRenderObj->GetID());

	ApplyOverInfoOption(pFarmRenderObj);
	ApplyShadowOption(pFarmRenderObj);

	return pFarmRenderObj;
}
// E 농장시스템 추가 added by hseos 2007.05.07

CPet* CObjectManager::AddPet(DWORD MasterIdx, BASEOBJECT_INFO* pBaseObjectInfo,BASEMOVE_INFO* pMoveInfo,PET_OBJECT_INFO* pPetObjectInfo, ITEMBASE* pItemInfo)
{
	CObject* pPreObj = GetObject( pBaseObjectInfo->dwObjectID );
	ASSERT( pPreObj == NULL );
	if( pPreObj )
	{
		RemoveObject( &pPreObj );
	}

	CPet* pPet = m_PetPool.Alloc();

	pPet->Init( eObjectKind_Pet, pBaseObjectInfo );
	pPet->InitPet( pPetObjectInfo );
	pPet->InitObjectBalloon( ObjectBalloon_Name | ObjectBalloon_Chat );
	pPet->SetWearedItem( pItemInfo );
	pPet->SetMaster( MasterIdx );
	
	APPEARANCEMGR->InitAppearance( pPet );

	MOVEMGR->InitMove( pPet,pMoveInfo );

	OBJECTSTATEMGR->InitObjectState( pPet );

	m_ObjectTable.Add(pPet,pPet->GetID());

	ApplyOverInfoOption( pPet );
	ApplyShadowOption( pPet );

	return pPet;
}

CHeroPet* CObjectManager::AddHeroPet(BASEOBJECT_INFO* pBaseObjectInfo,BASEMOVE_INFO* pMoveInfo,PET_OBJECT_INFO* pPetObjectInfo, ITEMBASE* pItemInfo)
{
	mpHeroPet = new CHeroPet;
	mpHeroPetBackup = NULL;

	mpHeroPet->Init( eObjectKind_Pet, pBaseObjectInfo );
	mpHeroPet->InitPet( pPetObjectInfo );
	mpHeroPet->InitObjectBalloon( ObjectBalloon_Name | ObjectBalloon_Chat );
	mpHeroPet->SetWearedItem( pItemInfo );
	mpHeroPet->SetMaster( HEROID );
	
	APPEARANCEMGR->InitAppearance( mpHeroPet );

	MOVEMGR->InitMove( mpHeroPet, pMoveInfo );

	OBJECTSTATEMGR->InitObjectState( mpHeroPet );

	m_ObjectTable.Add( mpHeroPet, mpHeroPet->GetID() );

	ApplyOverInfoOption( mpHeroPet );
	ApplyShadowOption( mpHeroPet );

	return mpHeroPet;
}

void CObjectManager::RemoveHeroPet()
{
	m_ObjectTable.Remove( mpHeroPet->GetID() );

	APPEARANCEMGR->CancelAlphaProcess( mpHeroPet );
	APPEARANCEMGR->CancelReservation( mpHeroPet );

	mpHeroPetBackup = mpHeroPet;

	delete mpHeroPet;
	mpHeroPet = NULL;
}

void CObjectManager::RefreshSiegeNamePannel()
{
	if( BATTLESYSTEM->GetBattle(HERO)->GetBattleKind() != eBATTLE_KIND_SIEGEWAR )
		return;

	CObject* pObject = NULL;
	m_ObjectTable.SetPositionHead();
	while( pObject = m_ObjectTable.GetData() )
	{		
		if( pObject->GetObjectKind() == eObjectKind_Player )
		{
			CPlayer* pPlayer = (CPlayer*)pObject;
			((CBattle_SiegeWar*)BATTLESYSTEM->GetBattle(HERO))->SetSiegeWarName( 
				pPlayer, pPlayer->GetBattleTeam(), pPlayer->GetGuildIdx() );
		}
	}	
}

// 06. 02. 타겟팅 - 이영준
BOOL CObjectManager::TargetByName(const char* name)
{
	CObject* pObject = NULL;
	m_ObjectTable.SetPositionHead();
	while( pObject = m_ObjectTable.GetData() )
	{		
		if( ( pObject->GetObjectKind() == eObjectKind_Player ) ||
		    ( pObject->GetObjectKind() == eObjectKind_Npc ) )
		{
			if( strcmp(pObject->GetObjectName(), name) == 0 )
			{
				if(pObject->GetID() == HERO->GetID())
					return FALSE;

				SetSelectedObject(pObject);

				return TRUE;
			}
		}
	}

	return FALSE;
}

// 06. 05 HIDE NPC - 이영준
// NPC를 찾아 정보를 갱신해준다
void CObjectManager::HideNpc(WORD UniqueIdx, BOOL bOn)
{
	CObject* pObject = NULL;
	m_ObjectTable.SetPositionHead();
	while( pObject = m_ObjectTable.GetData() )
	{
		if( ( pObject->GetObjectKind() == eObjectKind_Npc ) )
		{
			if( ((CNpc*)pObject)->GetNpcUniqueIdx() == UniqueIdx )
			{

				if(bOn)
				{
#ifdef _GMTOOL_
					if( MAINGAME->GetUserLevel() <= eUSERLEVEL_GM  && CAMERA->GetCameraMode() != eCM_EyeView )
						pObject->GetEngineObject()->SetAlpha( 1.0f );
					else
#endif
						pObject->GetEngineObject()->Show();
					
					AddShadow(pObject);
				}
				else
				{
#ifdef _GMTOOL_
					if( MAINGAME->GetUserLevel() <= eUSERLEVEL_GM  && CAMERA->GetCameraMode() != eCM_EyeView )
						pObject->GetEngineObject()->SetAlpha( 0.3f );
					else
#endif
						pObject->GetEngineObject()->HideWithScheduling();

					if( pObject->GetID() == GetSelectedObjectID() )
						SetSelectedObject( NULL );

					RemoveShadow(pObject);
				}
			}
		}
	}
}

CObject* CObjectManager::GetObjectByName(const char* name)
{
	CObject* pObject = NULL;
	m_ObjectTable.SetPositionHead();
	while( pObject = m_ObjectTable.GetData() )
	{		
		if( strcmp(pObject->GetObjectName(), name) == 0 )
		{
			return pObject;
		}
	}

	return NULL;
}

// 070911 LYW --- ObjectManager : Add function to return object id.
DWORD CObjectManager::GetObjectByChatName(const char* name)
{
	DWORD dwObjectID = 0 ;

	CObject* pObject = NULL;
	m_ObjectTable.SetPositionHead();

	char* pName = NULL ;
	char* pCur  = NULL ;
	char* pNext = NULL ;

	int nTotalLen = 0 ;
	int nCurLen = 0 ;

	char nameBuf[128] ;
	char tempBuf[4] ;

	BYTE byObjectKind = 0 ;

	while( pObject = m_ObjectTable.GetData() )
	{		
		memset(nameBuf, 0, sizeof(nameBuf)) ;

		pName = pObject->GetObjectName() ;
		nTotalLen = strlen(pName) ;

		pCur  = pObject->GetObjectName() ;

		byObjectKind = pObject->GetObjectKind() ;

		for(int count = 0 ; count < nTotalLen ; )
		{
			if( byObjectKind != 2 )
			{
				if( *pCur == '^' )
				{
					pNext = pCur + 1 ;

					if( *pNext == 's' )
					{
						strcat(nameBuf, " ") ;
						pCur += 2 ;
						count += 2 ;

						continue ;
					}
				}
			}
			else
			{
				if( *pCur == ' ' )
				{
					strcat(nameBuf, " ") ;
					++pCur ;
					++count ;

					continue ;
				}
			}

			if( pCur + 1 != CharNext( CharPrev( pCur, pCur + 1 ) ) )
			{
				memset(tempBuf, 0, sizeof(tempBuf)) ;
				memcpy(tempBuf, pCur, sizeof(char)*2) ;
				

				strcat(nameBuf, tempBuf) ;
				pCur += 2 ;
				count += 2 ;
			}
			else
			{
				memcpy(nameBuf, pCur, sizeof(char)) ;
				++pCur ;
				++count ;
			}
		}

		if( strcmp(nameBuf, name) == 0 )
		{
			dwObjectID = pObject->GetID() ;

			break ;
		}
	}

	return dwObjectID ;
}

// 070911 LYW --- ObjectManager : Add function to return monster id.
DWORD CObjectManager::GetNearMonsterByName(const char* name)								// HERO로 부터 가장 가까이 있는 몬스터 아이디를 반환하는 함수.
{
	VECTOR3 vHeroPos;																		// HERO의 위치를 담을 벡터를 선언한다.
	HERO->GetPosition(&vHeroPos) ;															// HERO의 위치를 받는다.

	DWORD dwObjectID = 0 ;																	// 반환할 몬스터 아이디를 담을 변수를 선언하고 0으로 세팅한다.

	float fNearDistance = 2000.0f ;															// 가장 작은 거리 값을 담을 변수를 선언하고 거리를 20m로 세팅한다.
	float fDistance = 0.0f ;																// HERO와 몬스터의 거리 값을 담을 변수를 선언하고 0m로 세팅한다.

	CObject* pObject = NULL;
	m_ObjectTable.SetPositionHead();

	char* pName = NULL ;
	char* pCur  = NULL ;
	char* pNext = NULL ;

	int nTotalLen = 0 ;
	int nCurLen = 0 ;

	char nameBuf[128] ;
	char tempBuf[4] ;

	BYTE byObjectKind = 0 ;

	while( pObject = m_ObjectTable.GetData() )
	{		
		if( pObject->GetObjectKind() <= 2 ) continue ;

		fDistance = CalcDistanceXZ(&vHeroPos, &pObject->GetBaseMoveInfo()->CurPosition) ;	// HERO와 몬스터의 거리를 구한다.

		if( fNearDistance < fDistance ) continue ;											// HERO와 몬스터의 거리가 최소거리 보다 크면, continue 처리를 한다.

		fNearDistance = fDistance ;															// 최소 거리를 HERO와 몬스터의 거리로 세팅한다.

		dwObjectID = pObject->GetID() ;														// 현재 오브젝트의 아이디를 세팅한다.

		memset(nameBuf, 0, sizeof(nameBuf)) ;

		pName = pObject->GetObjectName() ;
		nTotalLen = strlen(pName) ;

		pCur  = pObject->GetObjectName() ;

		byObjectKind = pObject->GetObjectKind() ;

		for(int count = 0 ; count < nTotalLen ; )
		{
			if( byObjectKind != 2 )
			{
				if( *pCur == '^' )
				{
					pNext = pCur + 1 ;

					if( *pNext == 's' )
					{
						strcat(nameBuf, " ") ;
						pCur += 2 ;
						count += 2 ;

						continue ;
					}
				}
			}
			else
			{
				if( *pCur == ' ' )
				{
					strcat(nameBuf, " ") ;
					++pCur ;
					++count ;

					continue ;
				}
			}

			if( pCur + 1 != CharNext( CharPrev( pCur, pCur + 1 ) ) )
			{
				memset(tempBuf, 0, sizeof(tempBuf)) ;
				memcpy(tempBuf, pCur, sizeof(char)*2) ;
				

				strcat(nameBuf, tempBuf) ;
				pCur += 2 ;
				count += 2 ;
			}
			else
			{
				memcpy(nameBuf, pCur, sizeof(char)) ;
				++pCur ;
				++count ;
			}
		}

		if( strcmp(nameBuf, name) == 0 )
		{
			dwObjectID = pObject->GetID() ;

			break ;
		}
	}

	return dwObjectID ;
}


float CObjectManager::GetDistance( CObject* a, CObject* b )
{
	if( a != NULL || b != NULL )
	{
		assert( 0 );
		return 0;
	}
	
	const VECTOR3 locationVector = a->GetCurPosition() - b->GetCurPosition();

	return sqrt( pow( locationVector.x, 2.0f ) + pow( locationVector.y, 2.0f ) + pow( locationVector.z, 2.0f ) );
}

void CObjectManager::SetHide( DWORD id, WORD level )
{
	CObject* pObject = m_ObjectTable.GetData( id );

	if( !pObject )
	{
		return;
	}

	if( pObject->GetObjectKind() != eObjectKind_Player )
	{
		return;
	}

	CPlayer* pPlayer = ( CPlayer* )pObject;

	pPlayer->GetCharacterTotalInfo()->HideLevel = level;

	WORD DetectLevel = HERO->GetCharacterTotalInfo()->DetectLevel;

	if( level )
	{
		if( !m_HideObjectTable.GetData( pObject->GetID() ) )
		{
			m_HideObjectTable.Add( pObject, pObject->GetID() );
		}

		if( level <= DetectLevel )
		{
			pPlayer->GetEngineObject()->Show();

			pPlayer->GetEngineObject()->SetAlpha( 0.3f );

			pPlayer->GetCharacterTotalInfo()->bVisible = FALSE;				
		}
		else
		{
#ifdef _GMTOOL_
			if( MAINGAME->GetUserLevel() <= eUSERLEVEL_GM  && CAMERA->GetCameraMode() != eCM_EyeView )
				pPlayer->GetEngineObject()->SetAlpha( 0.3f );
			else
#endif
			{
				if( pPlayer == HERO )
				{
					pPlayer->GetEngineObject()->SetAlpha( 0.3f );
				}
				else
				{
					pPlayer->GetEngineObject()->HideWithScheduling();
				}
			}

			pPlayer->GetCharacterTotalInfo()->bVisible = FALSE;

			if( pPlayer->GetID() == GetSelectedObjectID() )
			{
				SetSelectedObject( NULL );	
			}
		}
	}
	else
	{
		m_HideObjectTable.Remove( pObject->GetID() );

		pPlayer->GetEngineObject()->SetAlpha( 1.0f );
		pPlayer->GetEngineObject()->Show();

		pPlayer->GetCharacterTotalInfo()->bVisible = TRUE;				
	}

	ApplyShadowOption(pPlayer);		
	ApplyOverInfoOption(pPlayer);
}

void CObjectManager::SetDetect( DWORD id, WORD level )
{
	if( id != HEROID )
	{
		return;
	}

	HERO->GetCharacterTotalInfo()->DetectLevel = level;

	CObject* pObject = NULL;
	m_HideObjectTable.SetPositionHead();
	
	while( pObject = m_HideObjectTable.GetData() )
	{
		CPlayer* pPlayer = ( CPlayer* )pObject;

		SetHide( pPlayer->GetID(), pPlayer->GetCharacterTotalInfo()->HideLevel );
	}
}


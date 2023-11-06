// ShopItemManager.cpp: implementation of the CShopItemManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UserTable.h"
#include "ItemManager.h"
#include "player.h"
#include "ShopItemManager.h"
#include "..\[CC]Header\GameResourceStruct.h"
#include "MapDBMsgParser.h"
/*어빌리티 삭제 - #include "AbilityManager.h"*/
#include "PackedData.h"
#include "..\[CC]Header\GameResourceManager.h"
//#include "CharacterCalcManager.h"
#include "EventMapMgr.h"
#include "WantedManager.h"
#include "Battle.h"
#include "SiegeWarMgr.h"
#include "WeatherManager.h"
#include "ChangeItemMgr.h"
#include "QuestMapMgr.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//extern BOOL g_bPlusTime;


CShopItemManager::CShopItemManager()
{
	m_UsingItemPool = new CMemoryPoolTempl<SHOPITEMWITHTIME>;
	m_UsingItemPool->Init( 50, 10, "ShopItemPool" );

	m_MovePointPool = new CMemoryPoolTempl<MOVEDATA>;
	m_MovePointPool->Init( 50, 10, "MovePointPool" );

	m_UsingItemTable.Initialize( 50 );
	m_MovePointTable.Initialize( 30 );
}

CShopItemManager::~CShopItemManager()
{
	Release();
	m_UsingItemPool->Release();
	m_MovePointPool->Release();
	delete m_UsingItemPool;
	delete m_MovePointPool;
}


void CShopItemManager::Init(CPlayer* pPlayer)
{	
	m_pPlayer = pPlayer;
	m_Updatetime = 0;
	m_Checktime = 0;
	m_DupIncantation = 0;
	m_DupCharm = 0;
	m_DupHerb = 0;
	m_DupSundries = 0;
	m_ProtectItemIdx = 0;
}


void CShopItemManager::Release()
{
	m_UsingItemTable.SetPositionHead();
	SHOPITEMWITHTIME* pShopItem = NULL;
	while( pShopItem = m_UsingItemTable.GetData() )
	{
		m_UsingItemPool->Free( pShopItem );
	}
	m_UsingItemTable.RemoveAll();

	m_MovePointTable.SetPositionHead();
	MOVEDATA* pData = NULL;
	while( pData = m_MovePointTable.GetData() )
	{
		m_MovePointPool->Free( pData );
	}
	m_MovePointTable.RemoveAll();
}


int CShopItemManager::UseShopItem( ITEMBASE* pItemBase, SHOPITEMUSEBASE UseBaseInfo, SHOPITEMBASE* pShopItem )
{
	if( !pItemBase )
		return eItemUseErr_Err;
	
	if( EVENTMAPMGR->IsEventMap() )
		return eItemUseErr_Err;

	// 보스몹이 있는 맵은 기억석으로 저장 할 수 없음.
/*	if( pItemBase->wIconIdx == eIncantation_MemoryStone &&
		( g_pServerSystem->GetMapNum() == BOSSMONSTER_MAP 		
		|| g_pServerSystem->GetMapNum() == SIEGEWARMGR->GetSiegeMapNum()
		|| g_pServerSystem->GetMapNum() == QUESTMAPNUM1
		|| g_pServerSystem->GetMapNum() == QUESTMAPNUM2
		|| g_pServerSystem->GetMapNum() == BOSSMONSTER_2NDMAP ) )
		return eItemUseErr_Err;
	
	// 05.05.20  문파토너먼트시에 사용불가능 아이템
	if( g_pServerSystem->GetMapNum() == Tournament )
	{
		if( pItemBase->wIconIdx == eIncantation_MemoryStone ||
			pItemBase->wIconIdx == eIncantation_OtherRevive )
			return eItemUseErr_Err;
	}
*/
	//공성전에서는 기억석 사용 못함
	if( m_pPlayer->GetBattle()->GetBattleKind() == eBATTLE_KIND_SIEGEWAR )
	{
		if( pItemBase->wIconIdx == eIncantation_MemoryStone )
			return eItemUseErr_Err;

		if( m_pPlayer->GetBattleID() == 2 ) //관람모드
		{
			if( pItemBase->wIconIdx == eIncantation_OtherRevive )
				return eItemUseErr_Err;
		}
	}

	ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(UseBaseInfo.ShopItemIdx);
	if(!pItemInfo)		return eItemUseErr_Err;

//	임시로 넣음 - 성대
/*	SHOPITEMWITHTIME* pAddShopItem = NULL;
	if( pItemBase->wIconIdx == 55134 || pItemBase->wIconIdx == 55142 )
	{
		pAddShopItem = m_UsingItemTable.GetData(55134);
		if( !pAddShopItem )
		{
			pAddShopItem = m_UsingItemTable.GetData(55142);
		}
	}
	else
	{
        pAddShopItem = m_UsingItemTable.GetData(pItemBase->wIconIdx);
	}*/
//
	SHOPITEMWITHTIME* pAddShopItem = NULL;
	pAddShopItem = m_UsingItemTable.GetData(pItemBase->wIconIdx);
/*
	if( pAddShopItem )
	{
		if( pItemInfo->SellPrice && pItemInfo->ItemType == 10 )
		{
			// 개인플러스타임 체크
			if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM && pItemInfo->MeleeAttackMin )
			{
				if( pAddShopItem->ShopItem.Remaintime )
					return eItemUseErr_AlreadyUse;
				else
				{
					stTIME ct;
					ct.value = GetCurTime();
					if( pAddShopItem->ShopItem.BeginTime.GetYear() == ct.GetYear() && 
						pAddShopItem->ShopItem.BeginTime.GetMonth() == ct.GetMonth() &&
						pAddShopItem->ShopItem.BeginTime.GetDay() == ct.GetDay() )
						return eItemuseErr_DontUseToday;
					else
					{
						// 기존에 사용했던 개인플러스타임 삭제
						ShopItemDeleteToDB( m_pPlayer->GetID(), pAddShopItem->ShopItem.ItemBase.dwDBIdx );
						m_UsingItemTable.Remove( pAddShopItem->ShopItem.ItemBase.wIconIdx );
						m_UsingItemPool->Free( pAddShopItem );
						pAddShopItem = NULL;
					}
				}
			}

			else
				return eItemUseErr_AlreadyUse;
		}
	}

  /*	if( pItemBase->wIconIdx == eIncantation_MemoryMove15 || 
  		pItemBase->wIconIdx == 55357 || pItemBase->wIconIdx == 55362 )
  	{
  		if( pAddShopItem )
  			goto Not_DupAbleCheck;
  	}*/

/*	// 중복사용 체크
	if( IsDupAble( pItemInfo ) == FALSE )
		return eItemUseErr_DontDupUse;	

//Not_DupAbleCheck:

	if( !pAddShopItem )
	{
		pAddShopItem = m_UsingItemPool->Alloc();	
		pAddShopItem->ShopItem.ItemBase = *pItemBase;
		pAddShopItem->ShopItem.Param = pItemInfo->SellPrice;
	}

	// 사용기간이 있는 아이템 Rarity()
	if( pAddShopItem->ShopItem.Param && pItemInfo->ItemType == 10 )
	{
		SYSTEMTIME systime;
		stTIME startime, usetime, endtime;
		GetLocalTime(&systime);
		startime.SetTime(systime.wYear-2000, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, 0);
		pAddShopItem->ShopItem.BeginTime = startime;
		
		if( pItemInfo->SellPrice == eShopItemUseParam_Realtime )
		{
			endtime = startime;
			DWORD day = pItemInfo->Rarity/(24*60);
			DWORD hour = (pItemInfo->Rarity%(24*60))/60;
			DWORD minute = (pItemInfo->Rarity%(24*60))%60;
			usetime.SetTime(0, 0, day, hour, minute, 0);
			endtime += usetime;	
			
			pAddShopItem->ShopItem.Remaintime = endtime.value;
			pAddShopItem->LastCheckTime = gCurTime;
		}
		else if( pItemInfo->SellPrice == eShopItemUseParam_Playtime )
		{
			DWORD remaintime = 0;
			remaintime = pItemInfo->Rarity*60000;
			pAddShopItem->ShopItem.Remaintime = remaintime;
			pAddShopItem->LastCheckTime = gCurTime;
		}
		else if( pItemInfo->SellPrice == eShopItemUseParam_Continue )
		{
			pAddShopItem->ShopItem.Remaintime = 0;
			pAddShopItem->LastCheckTime = 0;
		}
	}
	
	// 사용하면 바로 없어지는 아이템
	if( pItemInfo->ItemKind == eSHOP_ITEM_HERB )
	{
		if(EI_TRUE != ITEMMGR->DiscardItem( m_pPlayer, UseBaseInfo.ShopItemPos, UseBaseInfo.ShopItemIdx, 1 ))
		{
			return eItemUseErr_Err;
		}

		// 물약
		MSG_DWORD2	msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_EFFECT;
		msg.dwData1 = m_pPlayer->GetID();

		if( pItemInfo->Str > 0 )
			msg.dwData2 = eEffect_ShopItem_Life;
		else if( pItemInfo->Vit > 0 )
			msg.dwData2 = eEffect_ShopItem_NaeRuyk;
		else if( pItemInfo->Life > 0 )
			msg.dwData2 = eEffect_ShopItem_MaxLife;
		else if( pItemInfo->Mana > 0 )
			msg.dwData2 = eEffect_ShopItem_MaxNaeRuyk;
		else
			msg.dwData2 = 0;
		
		if( msg.dwData2 )
			PACKEDDATA_OBJ->QuickSend( m_pPlayer, &msg, sizeof(msg) );	


		// 순간회복
		if( pItemInfo->SellPrice == 0 )
		{
			if( pItemInfo->Str > 0 )
				m_pPlayer->AddLife( pItemInfo->Str, NULL );
			if( pItemInfo->Vit > 0 )
				m_pPlayer->AddMana( pItemInfo->Vit, NULL );
		}
		else
		{
			CalcShopItemOption( pAddShopItem->ShopItem.ItemBase.wIconIdx, TRUE );
			m_pPlayer->CalcState();
			UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), pAddShopItem->ShopItem.ItemBase.wIconIdx, pAddShopItem->ShopItem.ItemBase.dwDBIdx, pItemInfo->SellPrice, pAddShopItem->ShopItem.BeginTime.value, pAddShopItem->ShopItem.Remaintime );
			//
			m_UsingItemTable.Add(pAddShopItem, pAddShopItem->ShopItem.ItemBase.wIconIdx);
		}
	}
	// 주문서
	else if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION )
	{
		if( pItemInfo->ItemType == 10 )
		{
			// 레벨 제한 해제
			if( pItemInfo->LimitJob )
			{
				if( pItemInfo->LimitGender == 0 && m_pPlayer->GetLevel() > 50 )
					return eItemUseErr_Err;
				else if( pItemInfo->LimitGender == 1 && (m_pPlayer->GetLevel() < 51 || m_pPlayer->GetLevel() > 70 ) )
					return eItemUseErr_Err;
			}

			if(EI_TRUE != ITEMMGR->DiscardItem( m_pPlayer, UseBaseInfo.ShopItemPos, UseBaseInfo.ShopItemIdx, 1 ))
				return eItemUseErr_Err;			

			// 돈+경험치 보호, 레벨 제한 해제
			if( pItemInfo->Vit || pItemInfo->LimitJob )
			{
				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
				UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), pAddShopItem->ShopItem.ItemBase.wIconIdx, pAddShopItem->ShopItem.ItemBase.dwDBIdx, pItemInfo->Vit, pAddShopItem->ShopItem.BeginTime.value, pAddShopItem->ShopItem.Remaintime );
				m_UsingItemTable.Add(pAddShopItem, pAddShopItem->ShopItem.ItemBase.wIconIdx);
			}
			// 스탯 재분배
			else if( pItemInfo->ItemIdx == eIncantation_StatePoint )
			{
				// Insert Using Info
				UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), pAddShopItem->ShopItem.ItemBase.wIconIdx, pAddShopItem->ShopItem.ItemBase.dwDBIdx, pItemInfo->SellPrice, pAddShopItem->ShopItem.BeginTime.value, pItemInfo->Str );
				CalcShopItemOption( pItemInfo->ItemIdx, TRUE, pItemInfo->Str );
				m_UsingItemTable.Add(pAddShopItem, pAddShopItem->ShopItem.ItemBase.wIconIdx);
			}
			// 스킬포인트 재분배
			else if( pItemInfo->Life )
			{
				SHOPITEMWITHTIME* pUsedItem = m_UsingItemTable.GetData( eIncantation_SkPointRedist );
				if( pUsedItem )
				{
					CalcShopItemOption( pItemInfo->ItemIdx, TRUE, pItemInfo->Life );
					ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), eIncantation_SkPointRedist, m_pPlayer->GetShopItemStats()->SkillPoint );
				}
				else
				{
					CalcShopItemOption( pItemInfo->ItemIdx, TRUE, pItemInfo->Life );
					UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), eIncantation_SkPointRedist, pAddShopItem->ShopItem.ItemBase.dwDBIdx,
						m_pPlayer->GetShopItemStats()->SkillPoint, pAddShopItem->ShopItem.BeginTime.value, m_pPlayer->GetShopItemStats()->UseSkillPoint );
					m_UsingItemTable.Add(pAddShopItem, eIncantation_SkPointRedist);
				}
			}
			else if( pItemInfo->ItemIdx == eIncantation_MixUp )
			{
				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
				UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), pAddShopItem->ShopItem.ItemBase.wIconIdx, pAddShopItem->ShopItem.ItemBase.dwDBIdx, pItemInfo->SellPrice, pAddShopItem->ShopItem.BeginTime.value, pAddShopItem->ShopItem.Remaintime );
				m_UsingItemTable.Add(pAddShopItem, pAddShopItem->ShopItem.ItemBase.wIconIdx);
			}
			else
			{
				UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), pAddShopItem->ShopItem.ItemBase.wIconIdx, pAddShopItem->ShopItem.ItemBase.dwDBIdx, pItemInfo->SellPrice, pAddShopItem->ShopItem.BeginTime.value, pAddShopItem->ShopItem.Remaintime );
				m_UsingItemTable.Add(pAddShopItem, pAddShopItem->ShopItem.ItemBase.wIconIdx);
			}
		}
	}
	// 부적
	else if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM || pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM ||
		pItemInfo->ItemKind == eSHOP_ITEM_SUNDRIES )
	{
		// 사용제한레벨 추가
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM && pItemInfo->EquipKind )
		{
			if( m_pPlayer->GetMaxLevel() < pItemInfo->EquipKind )
				return eItemUseErr_Err;
		}
		
		if( pItemInfo->ItemType == 10 )
		if(EI_TRUE != ITEMMGR->DiscardItem( m_pPlayer, UseBaseInfo.ShopItemPos, UseBaseInfo.ShopItemIdx, 1 ))
		{
			return eItemUseErr_Err;
		}

		//
		MSG_DWORD2	msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_EFFECT;
		msg.dwData1 = m_pPlayer->GetID();

		if( pItemInfo->ItemIdx == eSundries_Boom_Butterfly )
		{
			msg.dwData2 = eEffect_ShopItem_Boom1;
			PACKEDDATA_OBJ->QuickSend( m_pPlayer, &msg, sizeof(msg) );	
		}
		else if( pItemInfo->ItemIdx == eSundries_Boom_Star )
		{
			msg.dwData2 = eEffect_ShopItem_Boom2;
			PACKEDDATA_OBJ->QuickSend( m_pPlayer, &msg, sizeof(msg) );	
		}
		//
		
		CalcShopItemOption( pAddShopItem->ShopItem.ItemBase.wIconIdx, TRUE );
		m_pPlayer->CalcState();
		
		if( pItemInfo->SellPrice && pItemInfo->ItemType==10 )
		{
			UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), pAddShopItem->ShopItem.ItemBase.wIconIdx, pAddShopItem->ShopItem.ItemBase.dwDBIdx, pItemInfo->SellPrice, pAddShopItem->ShopItem.BeginTime.value, pAddShopItem->ShopItem.Remaintime );
			m_UsingItemTable.Add(pAddShopItem, pAddShopItem->ShopItem.ItemBase.wIconIdx);
		}
	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION )
	{
		CalcShopItemOption( pAddShopItem->ShopItem.ItemBase.wIconIdx, TRUE );
		m_UsingItemTable.Add(pAddShopItem, pAddShopItem->ShopItem.ItemBase.wIconIdx);

		// 다른사람들에게 아이템 착용 정보를 보낸다.
		MSG_DWORD2 msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_AVATAR_PUTON;
		msg.dwData1 = m_pPlayer->GetID();
		msg.dwData2 = pItemInfo->ItemIdx;

		PACKEDDATA_OBJ->QuickSendExceptObjectSelf(m_pPlayer, &msg, sizeof(msg));
		goto LogNotUse;
	}
	//SW051129 Pet
	else if( pItemInfo->ItemKind == eSHOP_ITEM_PET )
	{
		if(pItemInfo->LimitLevel > m_pPlayer->GetMaxLevel())
		{
			return eItemUseErr_Err;
		}
		m_pPlayer->GetPetManager()->SummonPet(pItemBase->dwDBIdx);
		//m_pPlayer->GetPetManager()->SummonPet(PET_ID_START);
		//해당 유저의 펫 매니져에서 해당 팻을 찾아 현재팻으로 설정하고
		//해당 팻의 정보를 그리드에 보낸다.
		goto LogNotUse;
	}
	//SW060406 펫 장착
	else if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP )
	{
		//펫 소환중이 아니면
		if(!m_pPlayer->GetPetManager()->GetCurSummonPet())
		{
			return eItemUseErr_Err;
		}

		m_pPlayer->GetPetManager()->SetPetEquipOption(pItemInfo->ItemIdx, TRUE);
	}

	AddDupParam( pItemInfo );

	//
	LogItemMoney( m_pPlayer->GetID(), m_pPlayer->GetObjectName(), 0, "",
		eLog_ShopItemUse, m_pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
		pAddShopItem->ShopItem.ItemBase.wIconIdx, pAddShopItem->ShopItem.ItemBase.dwDBIdx, pAddShopItem->ShopItem.ItemBase.Position, 0, pAddShopItem->ShopItem.ItemBase.Durability, m_pPlayer->GetPlayerExpPoint());
	
LogNotUse:

	//
	memcpy(pShopItem, &pAddShopItem->ShopItem, sizeof(SHOPITEMBASE));

*/	return eItemUseSuccess;
}


BOOL CShopItemManager::UsedShopItem( ITEMBASE* pItemBase, DWORD Param, stTIME BeginTime, DWORD RemainTime )
{
/*	if( !pItemBase )
		return FALSE;

	if( m_UsingItemTable.GetData(pItemBase->wIconIdx) )
		return FALSE;

	SHOPITEMWITHTIME* ShopItem = m_UsingItemPool->Alloc();
	ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(pItemBase->wIconIdx);
	if( !pItemInfo )
	{
		// 특기치초기화 
		if( pItemBase->wIconIdx != eIncantation_SkPointRedist )
			return FALSE;
		else
		{
			SHOPITEMOPTION* pOption = m_pPlayer->GetShopItemStats();
			pOption->SkillPoint = Param;
			pOption->UseSkillPoint = RemainTime;
		}
	}

	ShopItem->ShopItem.ItemBase = *pItemBase;
	ShopItem->ShopItem.Param = Param;
	ShopItem->ShopItem.BeginTime = BeginTime;
	ShopItem->ShopItem.Remaintime = RemainTime;
	ShopItem->LastCheckTime = gCurTime;
/*
	if( pItemInfo && pItemInfo->SellPrice == eShopItemUseParam_Realtime )
	{
		// 종료시간을 체크한다.
		stTIME ctime, rtime;
		ctime.value = GetCurTime();
		rtime.SetTime( ShopItem->ShopItem.Remaintime );
		if( ctime > rtime )
		{
			if( pItemInfo->ItemType == 11 )
			{					
				if(EI_TRUE != ITEMMGR->DiscardItem( m_pPlayer, ShopItem->ShopItem.ItemBase.Position, ShopItem->ShopItem.ItemBase.wIconIdx, 1 ))
				{
					ASSERTMSG(0, "ShoItem - CheckEndTime");
					return FALSE;
				}
				if( pItemInfo->ItemKind == eSHOP_ITEM_EQUIP )
				{
					// 아이템 없앤다.
				}

			}

			ShopItemDeleteToDB( m_pPlayer->GetID(), ShopItem->ShopItem.ItemBase.dwDBIdx );
			
			LogItemMoney( m_pPlayer->GetID(), m_pPlayer->GetObjectName(), ctime.value, "",
				eLog_ShopItemUseEnd, m_pPlayer->GetMoney(eItemTable_Inventory), ShopItem->ShopItem.BeginTime.value, ShopItem->ShopItem.Remaintime,
				ShopItem->ShopItem.ItemBase.wIconIdx, ShopItem->ShopItem.ItemBase.dwDBIdx, ShopItem->ShopItem.ItemBase.Position, 0, ShopItem->ShopItem.ItemBase.Durability, m_pPlayer->GetPlayerExpPoint());

			return FALSE;
		}
	}

	
	m_UsingItemTable.Add(ShopItem, ShopItem->ShopItem.ItemBase.wIconIdx);

	if( pItemBase->wIconIdx == eIncantation_ProtectAll || pItemBase->wIconIdx == eIncantation_ProtectAll15 ||
		pItemBase->wIconIdx == 55363 || pItemBase->wIconIdx == eIncantation_ProtectAll1 ||
		pItemBase->wIconIdx == 55386 )
		CalcShopItemOption(pItemBase->wIconIdx, TRUE, ShopItem->ShopItem.Param);
	else
	{
		// 개인플러스타임이 종료되었으면 적용 안시킨다
		if( pItemInfo && pItemInfo->ItemKind == eSHOP_ITEM_CHARM && pItemInfo->MeleeAttackMin && RemainTime==0 )
			return TRUE;
		else
			CalcShopItemOption(pItemBase->wIconIdx, TRUE, ShopItem->ShopItem.Remaintime);
	}

	// 중복체크 파라미터 설정
	AddDupParam( pItemInfo );
*/	
	return TRUE;
}


void CShopItemManager::DeleteUsingShopItem( DWORD ItemIdx, BOOL bDelete )
{
	SHOPITEMWITHTIME* pShopItem = m_UsingItemTable.GetData( ItemIdx );
	if( !pShopItem )	 return;

	m_UsingItemTable.Remove( pShopItem->ShopItem.ItemBase.wIconIdx );
	m_UsingItemPool->Free( pShopItem );

	if( bDelete )
	{
		ITEMMGR->DiscardItem( m_pPlayer, pShopItem->ShopItem.ItemBase.Position, pShopItem->ShopItem.ItemBase.wIconIdx, 1 );
		CalcShopItemOption( pShopItem->ShopItem.ItemBase.wIconIdx, FALSE );
		SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_USEEND, pShopItem->ShopItem.ItemBase.wIconIdx);
	}

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( (WORD)ItemIdx );
	if( pItemInfo )
		DeleteDupParam( pItemInfo );

	// DB에 셋팅
	ShopItemDeleteToDB( m_pPlayer->GetID(), pShopItem->ShopItem.ItemBase.dwDBIdx );

	// Log추가
	stTIME curtime;
	curtime.value = GetCurTime();
	LogItemMoney( m_pPlayer->GetID(), m_pPlayer->GetObjectName(), curtime.value, "",
		eLog_ShopItemUseEnd, m_pPlayer->GetMoney(eItemTable_Inventory), pShopItem->ShopItem.BeginTime.value, pShopItem->ShopItem.Remaintime,
		pShopItem->ShopItem.ItemBase.wIconIdx, pShopItem->ShopItem.ItemBase.dwDBIdx, pShopItem->ShopItem.ItemBase.Position, 0, pShopItem->ShopItem.ItemBase.Durability, m_pPlayer->GetPlayerExpPoint());
}

void CShopItemManager::DeleteUsingShopItemInfo( DWORD ItemIdx )
{
	SHOPITEMWITHTIME* pShopItem = m_UsingItemTable.GetData( ItemIdx );
	if( !pShopItem )	 return;

	m_UsingItemTable.Remove( pShopItem->ShopItem.ItemBase.wIconIdx );
	m_UsingItemPool->Free( pShopItem );
}

void CShopItemManager::CheckEndTime()
{
	if( m_UsingItemTable.GetDataNum() == 0 )
		return;

	BOOL bUpdate = FALSE;
	m_Updatetime += gTickTime;
	if( m_Updatetime > 600000 )
	{
		m_Updatetime = 0;
		bUpdate = TRUE;
	}

	m_Checktime += gTickTime;
	if( !bUpdate && m_Checktime < 30000 )	
		return;


	stTIME curtime;
	ITEM_INFO* pItemInfo = NULL;
	SHOPITEMWITHTIME* pShopItem = NULL;
	curtime.value = GetCurTime();
	m_UsingItemTable.SetPositionHead();

	cPtrList listRemove;
	cPtrList listDel;
/*
	// 종료시간 조사
	while( pShopItem = m_UsingItemTable.GetData() )
	{
		pItemInfo = ITEMMGR->GetItemInfo( pShopItem->ShopItem.ItemBase.wIconIdx );
		if( !pItemInfo)		continue;

		// 현재시간
		if( pItemInfo->SellPrice == eShopItemUseParam_Realtime )
		{
			stTIME EndTime;
			EndTime.SetTime( pShopItem->ShopItem.Remaintime );
			if( curtime > EndTime )
			{
				CShopInvenSlot* pSlot = (CShopInvenSlot*)m_pPlayer->GetSlot( eItemTable_ShopInven );				
				
				if( pItemInfo->ItemType == 11 )
				{
					BOOL bfind = FALSE;
					// Item Position 확인
					for(int i=0; i<SLOT_SHOPINVEN_NUM/2; i++)
					{
						ITEMBASE* pItemBase = (ITEMBASE*)pSlot->GetItemInfoAbs( i+pSlot->GetStartPos() );
						if( !pItemBase )	continue;

						if( pItemBase->dwDBIdx == pShopItem->ShopItem.ItemBase.dwDBIdx )
						{
							pShopItem->ShopItem.ItemBase.Position = pItemBase->Position;
							bfind = TRUE;
							break;
						}
					}
					// 장비인지 확인
					if( !bfind )
					{
						CShopInvenSlot* pSlot = (CShopInvenSlot*)m_pPlayer->GetSlot( eItemTable_Weared );
						for(int i=0; i<SLOT_WEAR_NUM; i++)
						{
							ITEMBASE* pItemBase = (ITEMBASE*)pSlot->GetItemInfoAbs( i+pSlot->GetStartPos() );
							if( !pItemBase )	continue;

							if( pItemBase->dwDBIdx == pShopItem->ShopItem.ItemBase.dwDBIdx )
							{
								pShopItem->ShopItem.ItemBase.Position = pItemBase->Position;
								bfind = TRUE;
								break;
							}
						}
					}
					if( !bfind )
					{
						CPetWearSlot* pSlot = (CPetWearSlot*)m_pPlayer->GetSlot( eItemTable_PetWeared );
						for(int i=0; i<SLOT_PETWEAR_NUM; i++)
						{
							ITEMBASE* pItemBase = (ITEMBASE*)pSlot->GetItemInfoAbs( i+pSlot->GetStartPos() );
							if( !pItemBase )	continue;

							if( pItemBase->dwDBIdx == pShopItem->ShopItem.ItemBase.dwDBIdx )
							{
								pShopItem->ShopItem.ItemBase.Position = pItemBase->Position;
								bfind = TRUE;
								break;
							}
						}
					}
					if( !bfind )
					{
						char buf[256];
						sprintf(buf, "ShopItem Position NOT find - Makeup or Decoration [%d] [%d]", pShopItem->ShopItem.ItemBase.wIconIdx, pShopItem->ShopItem.ItemBase.Position );
						ASSERTMSG(0, buf);
						continue;
					}
					if(EI_TRUE != ITEMMGR->DiscardItem( m_pPlayer, pShopItem->ShopItem.ItemBase.Position, pShopItem->ShopItem.ItemBase.wIconIdx, 1 ))
					{
						char buf[256];
						sprintf(buf, "ShopItem Discard - Makeup or Decoration [%d] [%d]", pShopItem->ShopItem.ItemBase.wIconIdx, pShopItem->ShopItem.ItemBase.Position );
						ASSERTMSG(0, buf);
						continue;
					}
					
					if( pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION || pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP )
					{
						MSG_ITEM_DISCARD_ACK msg;
						msg.Category = MP_ITEM;
						msg.Protocol = MP_ITEM_DISCARD_ACK;
						msg.wItemIdx = pShopItem->ShopItem.ItemBase.wIconIdx;
						msg.TargetPos = pShopItem->ShopItem.ItemBase.Position;
						msg.ItemNum = 1;
						m_pPlayer->SendMsg( &msg, sizeof(msg) );
						
						TakeOffAvatarItem( pShopItem->ShopItem.ItemBase.wIconIdx, pShopItem->ShopItem.ItemBase.Position );
					}				
				}
				else
				{
					listDel.AddTail(pShopItem);
				}

				// player에게 셋팅
				CalcShopItemOption( pShopItem->ShopItem.ItemBase.wIconIdx, FALSE );
				SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_USEEND, pShopItem->ShopItem.ItemBase.wIconIdx);				
			}
			else
			{
				stTIME checksum;
				checksum = EndTime;
				checksum -= curtime;

				// 1분 미만
				if( !checksum.GetYear() && !checksum.GetMonth() && !checksum.GetDay() && !checksum.GetHour() && !checksum.GetMinute() )
					SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_ONEMINUTE, pShopItem->ShopItem.ItemBase.wIconIdx);				

				// 프리미엄 아이템 패널티 갱신추가
			}
		}
		// 플레이시간
		else if( pItemInfo->SellPrice == eShopItemUseParam_Playtime )
		{
			// 개인플러스타임이면
			if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM && pItemInfo->MeleeAttackMin )
			{
				// 플러스타임과 겹치면 시간이 안가도록
				if( pShopItem->ShopItem.Remaintime && gEventRate[pItemInfo->MeleeAttackMin] != gEventRateFile[pItemInfo->MeleeAttackMin] )
				{
					pShopItem->LastCheckTime = gCurTime;
					continue;
				}
				else if( pShopItem->ShopItem.Remaintime == 0 )
					continue;
			}
			
			int backuptime = pShopItem->ShopItem.Remaintime;
			int checksum = pShopItem->ShopItem.Remaintime - (gCurTime - pShopItem->LastCheckTime);
			if( checksum <= 0 )
			{				
				pShopItem->ShopItem.Remaintime = 0;
												  
				// player에게 셋팅
				CalcShopItemOption( pShopItem->ShopItem.ItemBase.wIconIdx, FALSE );
				SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_USEEND, pShopItem->ShopItem.ItemBase.wIconIdx);

				// 개인플러스타임이 삭제하지 않고 보관한다. (같은종류는 하루에 하나만 사용하도록 하기위해)
				if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM && pItemInfo->MeleeAttackMin )				
					ShopItemUpdatetimeToDB( m_pPlayer->GetID(), pShopItem->ShopItem.ItemBase.wIconIdx, 0 );
				else
				{
					ShopItemDeleteToDB( m_pPlayer->GetID(), pShopItem->ShopItem.ItemBase.dwDBIdx );
					listRemove.AddTail(pShopItem);
				}

				DeleteDupParam( pItemInfo );

				// Log추가
				LogItemMoney( m_pPlayer->GetID(), m_pPlayer->GetObjectName(), curtime.value, "",
					eLog_ShopItemUseEnd, m_pPlayer->GetMoney(eItemTable_Inventory), pShopItem->ShopItem.BeginTime.value, pShopItem->ShopItem.Remaintime,
					pShopItem->ShopItem.ItemBase.wIconIdx, pShopItem->ShopItem.ItemBase.dwDBIdx, pShopItem->ShopItem.ItemBase.Position, 0, pShopItem->ShopItem.ItemBase.Durability, m_pPlayer->GetPlayerExpPoint());
			}
			else if( pShopItem->ShopItem.Remaintime )
			{
				pShopItem->ShopItem.Remaintime -= (gCurTime - pShopItem->LastCheckTime);
				pShopItem->LastCheckTime = gCurTime;
			}

			if( bUpdate && checksum > 0 )
				ShopItemUpdatetimeToDB( m_pPlayer->GetID(), pShopItem->ShopItem.ItemBase.wIconIdx, pShopItem->ShopItem.Remaintime );
			if( backuptime > (MINUTETOSECOND*1000) && pShopItem->ShopItem.Remaintime <= (MINUTETOSECOND*1000) )
				SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_ONEMINUTE, pShopItem->ShopItem.ItemBase.wIconIdx);
		}
	}

	PTRLISTPOS pos = listDel.GetHeadPosition();
	while( pos )
	{
		SHOPITEMWITHTIME* p = (SHOPITEMWITHTIME*)listDel.GetNext(pos);
		DeleteUsingShopItem( p->ShopItem.ItemBase.wIconIdx );
	}
	listDel.RemoveAll();

	pos = listRemove.GetHeadPosition();
	while( pos )
	{
		SHOPITEMWITHTIME* p = (SHOPITEMWITHTIME*)listRemove.GetNext(pos);
		m_UsingItemTable.Remove( p->ShopItem.ItemBase.wIconIdx );
		m_UsingItemPool->Free(p);
	}
	listRemove.RemoveAll();
*/
	m_Checktime = 0;
}


void CShopItemManager::CheckAvatarEndtime()
{
	stTIME curtime;
	ITEM_INFO* pItemInfo = NULL;
	SHOPITEMWITHTIME* pShopItem = NULL;
	curtime.value = GetCurTime();
	m_UsingItemTable.SetPositionHead();
/*
	// 종료시간 조사
	while( pShopItem = m_UsingItemTable.GetData() )
	{
		pItemInfo = ITEMMGR->GetItemInfo( pShopItem->ShopItem.ItemBase.wIconIdx );
		if( !pItemInfo)		continue;

		// 현재시간
		if( pItemInfo->SellPrice == eShopItemUseParam_Realtime )
		{
			stTIME EndTime;
			EndTime.SetTime( pShopItem->ShopItem.Remaintime );
			if( curtime > EndTime )
			{
				// Item 지우고
				if(EI_TRUE != ITEMMGR->DiscardItem( m_pPlayer, pShopItem->ShopItem.ItemBase.Position, pShopItem->ShopItem.ItemBase.wIconIdx, 1 ))
				{
					ASSERTMSG(0, "ShoItem - CheckEndTime");
					continue;
				}
				// player에게 셋팅
				SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_USEEND, pShopItem->ShopItem.ItemBase.wIconIdx);
				ShopItemDeleteToDB( m_pPlayer->GetID(), pShopItem->ShopItem.ItemBase.dwDBIdx );

				// Log추가
				LogItemMoney( m_pPlayer->GetID(), m_pPlayer->GetObjectName(), curtime.value, "",
					eLog_ShopItemUseEnd, m_pPlayer->GetMoney(eItemTable_Inventory), pShopItem->ShopItem.BeginTime.value, pShopItem->ShopItem.Remaintime,
					pShopItem->ShopItem.ItemBase.wIconIdx, pShopItem->ShopItem.ItemBase.dwDBIdx, pShopItem->ShopItem.ItemBase.Position, 0, pShopItem->ShopItem.ItemBase.Durability, m_pPlayer->GetPlayerExpPoint());
			}
		}
	}*/
}


void CShopItemManager::SendMsgDwordToPlayer( DWORD Msg, DWORD Param )
{
	MSG_DWORD msg;
	msg.Category = MP_ITEM;
	msg.Protocol = (BYTE)Msg;
	msg.dwData = Param;
	
	m_pPlayer->SendMsg(&msg, sizeof(msg));

}


void CShopItemManager::UpdateLogoutToDB()
{
	stTIME curtime;
	SHOPITEMWITHTIME* pShopItem = NULL;
	ITEM_INFO* pItemInfo = NULL;
/*

	curtime.value = GetCurTime();
	m_UsingItemTable.SetPositionHead();

	// 종료시간 조사
	while( pShopItem = m_UsingItemTable.GetData() )
	{
		pItemInfo = ITEMMGR->GetItemInfo( pShopItem->ShopItem.ItemBase.wIconIdx );
		if( !pItemInfo )		continue;

		// 플레이시간
		if( pItemInfo->SellPrice == eShopItemUseParam_Playtime )
		{
			// 개인플러스타임이면
			if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM && pItemInfo->MeleeAttackMin )
			{
				// 플러스타임과 겹치면 시간이 안가도록
				if( pShopItem->ShopItem.Remaintime && gEventRate[pItemInfo->MeleeAttackMin] != gEventRateFile[pItemInfo->MeleeAttackMin] )
				{
					pShopItem->LastCheckTime = gCurTime;
					continue;
				}
				else if( pShopItem->ShopItem.Remaintime == 0 )
					continue;
			}
			
			DWORD checktime = gCurTime - pShopItem->LastCheckTime;
			if( checktime > 30000 )			checktime = 30000;
			//
			if( checktime >= pShopItem->ShopItem.Remaintime )
				pShopItem->ShopItem.Remaintime = 0;
			else
				pShopItem->ShopItem.Remaintime -= checktime;
			//
			ShopItemUpdatetimeToDB( m_pPlayer->GetID(), pShopItem->ShopItem.ItemBase.wIconIdx, pShopItem->ShopItem.Remaintime );
		}
	}*/
}



// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃RaMa - 04.11.08                                           ┃
// ┃Sub. ShopItem		                                       ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
BOOL CShopItemManager::CalcShopItemOption( DWORD wIdx, BOOL bAdd, DWORD Param )
{
	if( wIdx == 0 )			return FALSE;
/*	
	int calc = -1;
	if(bAdd)	calc=1;
	SHOPITEMOPTION* pShopItemOption = m_pPlayer->GetShopItemStats();
	if( !pShopItemOption )	return FALSE;

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( (WORD)wIdx );
	if(!pItemInfo)
	{
		if( wIdx != eIncantation_SkPointRedist )
		{
			char buf[256]= { 0, };
			sprintf(buf, "S ItemInfo가 없다. PlayerIdx[%d] Idx[%d]", m_pPlayer->GetID(), wIdx);
			ASSERTMSG(0, buf);
			return FALSE;
		}
		else
			return FALSE;
	}

/*	if( pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM )
	{
		// 경험치   GenGol
		if( pItemInfo->GenGol>0 )
		{
			pShopItemOption->AddExp += (pItemInfo->GenGol*calc);
			if(pShopItemOption->AddExp < 0)
				pShopItemOption->AddExp=0;			
		}
		// 아이템 드랍확률
		if( pItemInfo->MinChub>0 )
		{
			pShopItemOption->AddItemDrop += (pItemInfo->MinChub*calc);
			if(pShopItemOption->AddItemDrop < 0)
				pShopItemOption->AddItemDrop=0;
		}
		// 죽었을때 경험치와 돈 패널티 없는것		
	}*/
/*	if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION )
	{
		// 조합확률
		if( pItemInfo->ItemIdx == eIncantation_MixUp )
		{
			pShopItemOption->ItemMixSuccess += (10*calc);
			if(pShopItemOption->ItemMixSuccess < 0)
				pShopItemOption->ItemMixSuccess=0;
		}
		if( pItemInfo->ItemIdx == eIncantation_StatePoint )
		{
			if( Param > 0 )
				pShopItemOption->StatePoint = (WORD)Param;
			else
			{
				m_pPlayer->SetPlayerLevelUpPoint( m_pPlayer->GetPlayerLevelUpPoint()+pItemInfo->Str );
				pShopItemOption->StatePoint = 0;
				DeleteUsingShopItem( eIncantation_StatePoint );
			}
		}
		else if( pItemInfo->Life )
		{
			if( Param > 0 )
				pShopItemOption->SkillPoint += pItemInfo->Life;
		}
		else if( pItemInfo->Vit )
		{
			if( bAdd )
			{
				if( Param == 0 )
					pShopItemOption->ProtectCount = (char)pItemInfo->Vit;
				else if( Param )
					pShopItemOption->ProtectCount = (char)Param;

				m_ProtectItemIdx = pItemInfo->ItemIdx;
			}
			else
				m_ProtectItemIdx = 0;
		}
		else if( pItemInfo->LimitJob )
		{
			if( bAdd )
				pShopItemOption->EquipLevelFree += (BYTE)pItemInfo->LimitJob;
			else
			{
				pShopItemOption->EquipLevelFree -= (BYTE)pItemInfo->LimitJob;				
			}
			//CHARCALCMGR->CalcItemStats( m_pPlayer );
		}
	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM )
	{
		if( pItemInfo->Str>0 )
		{
			pShopItemOption->Str += (pItemInfo->Str*calc);
			if(pShopItemOption->Str < 0)
				pShopItemOption->Str=0;
			
			if( bAdd == FALSE )
				CHARCALCMGR->CalcCharStats( m_pPlayer );
		}
		if( pItemInfo->Dex>0 )
		{
			pShopItemOption->Dex += (pItemInfo->Dex*calc);
			if(pShopItemOption->Dex < 0)
				pShopItemOption->Dex=0;

			if( bAdd == FALSE )
				CHARCALCMGR->CalcCharStats( m_pPlayer );
		}
		if( pItemInfo->Vit>0 )
		{		
			pShopItemOption->Vit += (pItemInfo->Vit*calc);
			if(pShopItemOption->Vit < 0)
				pShopItemOption->Vit=0;

			if( bAdd == FALSE )
				CHARCALCMGR->CalcCharStats( m_pPlayer );
		}
		if( pItemInfo->Wis>0 )
		{
			pShopItemOption->Wis += (pItemInfo->Wis*calc);
			if(pShopItemOption->Wis < 0)
				pShopItemOption->Wis=0;

			if( bAdd == FALSE )
				CHARCALCMGR->CalcCharStats( m_pPlayer );
		}
		// 내공데미지   Life
		if( pItemInfo->Life>0 )
		{
			pShopItemOption->NeagongDamage += (char)(pItemInfo->Life*calc);
			if(pShopItemOption->NeagongDamage < 0)
				pShopItemOption->NeagongDamage=0;
		}
		// 무공특기치 1씩  Plus_SkillIdx
/*어빌리티 삭제 - 		if( pItemInfo->Mana>0 )
		{
			pShopItemOption->AddLevel += (pItemInfo->Mana*calc);
			if(pShopItemOption->AddLevel < 0)
				pShopItemOption->AddLevel=0;

			if( m_pPlayer->GetInited() )
				ABILITYMGR->UpdateAbilityState(ABILITYUPDATE_ABILITYLEVEL_CHANGED,0,m_pPlayer->GetAbilityGroup());
		}*/
/*		// 기본데미지  Plus_Value
		if( pItemInfo->LimitJob>0 )
		{
			pShopItemOption->ComboDamage += (pItemInfo->LimitJob*calc);
			if(pShopItemOption->ComboDamage < 0)
				pShopItemOption->ComboDamage=0;
		}
		// 크리티컬 확률   CriticalPercent
		if( pItemInfo->LimitGender>0 )
		{
			pShopItemOption->Critical += (pItemInfo->LimitGender*calc);
			if(pShopItemOption->Critical < 0)
				pShopItemOption->Critical=0;
		}
		// 크리티컬시 스턴확률    RangeAttackMin
		if( pItemInfo->LimitLevel>0 )
		{
			pShopItemOption->StunByCri += (pItemInfo->LimitLevel*calc);
			if(pShopItemOption->StunByCri < 0)
				pShopItemOption->StunByCri=0;
		}
		// 물리방어력
		if( pItemInfo->LimitStr>0 )
		{
			pShopItemOption->RegistPhys += (pItemInfo->LimitStr*calc);
			if(pShopItemOption->RegistPhys < 0)
				pShopItemOption->RegistPhys = 0;
		}
		// 속성방어력
		if( pItemInfo->LimitDex>0 )
		{
			pShopItemOption->RegistAttr += (pItemInfo->LimitDex*calc);
			if(pShopItemOption->RegistAttr < 0)
				pShopItemOption->RegistAttr = 0;
		}
		// 내력소모감소
		if( pItemInfo->LimitVit>0 )
		{
			// 플러스타임
			if( pItemInfo->MeleeAttackMin )
			{
				if( bAdd && gEventRate[pItemInfo->MeleeAttackMin] == gEventRateFile[pItemInfo->MeleeAttackMin] )
					pShopItemOption->PlustimeMana = (char)pItemInfo->LimitVit;
				else
					pShopItemOption->PlustimeMana = 0;
			}
			else
			{
				pShopItemOption->ManaSpend += (pItemInfo->LimitVit*calc);
				if(pShopItemOption->ManaSpend < 0 )
					pShopItemOption->ManaSpend = 0;
			}
		}
		// 경험치 증가
		if( pItemInfo->LimitWis>0 )
		{
			// 플러스타임
			if( pItemInfo->MeleeAttackMin )
			{				
				if( bAdd && gEventRate[pItemInfo->MeleeAttackMin] == gEventRateFile[pItemInfo->MeleeAttackMin] )
					pShopItemOption->PlustimeExp = (char)pItemInfo->LimitWis;
				else
					pShopItemOption->PlustimeExp = 0;
			}
			else
			{
				pShopItemOption->AddExp += (pItemInfo->LimitWis*calc);
				if(pShopItemOption->AddExp < 0)
					pShopItemOption->AddExp=0;
			}
		}
		// 수련치 증가
		if( pItemInfo->ItemGrade>0 )
		{
			// 플러스타임
			if( pItemInfo->MeleeAttackMin )
			{
				if( bAdd && gEventRate[pItemInfo->MeleeAttackMin] == gEventRateFile[pItemInfo->MeleeAttackMin] )
					pShopItemOption->PlustimeAbil = (char)pItemInfo->ItemGrade;
				else
					pShopItemOption->PlustimeAbil = 0;
			}
			else
			{
/*어빌리티 삭제 - 				pShopItemOption->AddAbility += (pItemInfo->ItemGrade*calc);
				if(pShopItemOption->AddAbility < 0)
					pShopItemOption->AddAbility=0;*/
/*			}
		}
		// 무공수련치 증가
		if( pItemInfo->RangeType>0 )
		{
			pShopItemOption->AddSkillExp += (pItemInfo->RangeType*calc);
			if(pShopItemOption->AddSkillExp < 0)
				pShopItemOption->AddSkillExp=0;
		}
		// 최대생명력 증가치
		if( pItemInfo->Plus_SkillIdx )
		{
			pShopItemOption->Life += (pItemInfo->Plus_SkillIdx*calc);
			if( pShopItemOption->Life <= 0 )
				pShopItemOption->Life = 0;	
			
			if( bAdd==FALSE )
				CHARCALCMGR->CalcCharLife(m_pPlayer);
		}
		// 최대내력 증가치
		if( pItemInfo->AllPlus_Kind )
		{
			pShopItemOption->Mana += (pItemInfo->AllPlus_Kind*calc);
			if( pShopItemOption->Mana <= 0 )
				pShopItemOption->Mana = 0;

			if( bAdd==FALSE )
				CHARCALCMGR->CalcCharMana(m_pPlayer);
		}
		// 경공딜레이
		if( pItemInfo->RangeAttackMin )
		{
			pShopItemOption->bKyungGong += (pItemInfo->RangeAttackMin*calc);
		}
		// 경공속도 상승
		if( pItemInfo->RangeAttackMax )
		{
			pShopItemOption->KyungGongSpeed += (pItemInfo->RangeAttackMax*calc);
		}
		// 강화 증폭 수치
		if( pItemInfo->CriticalPercent )
		{
			pShopItemOption->ReinforceAmp += (pItemInfo->CriticalPercent*calc);

			if( m_pPlayer->GetInited() )
				CHARCALCMGR->CalcItemStats( m_pPlayer );
		}
		// 아이템드랍확률 
		if( pItemInfo->PhyDef )
		{
			pShopItemOption->AddItemDrop += (pItemInfo->PhyDef*calc);
		}
		//////////////////////////////////////////////////////////////////////
		// 06. 07. 내공 적중(일격) - 이영준
		if( pItemInfo->ManaRecover > 0 )
		{
			pShopItemOption->Decisive += (pItemInfo->ManaRecover*calc);
			if(pShopItemOption->Decisive < 0)
				pShopItemOption->Decisive = 0;
		}
		//////////////////////////////////////////////////////////////////////
/*		// 내력회복룰 1.5배
		if( pItemInfo->manaRecoverRate>0 )
		{
			m_ShopItemOption.RecoverRate += (pItemInfo->manaRecoverRate*calc);
			if( m_ShopItemOption.RecoverRate<=0 )
				m_ShopItemOption.RecoverRate = 0;

		}*/
/*	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_HERB )
	{
		// 최대생명력 증가치
		if( pItemInfo->Life > 0 )
		{
			pShopItemOption->Life += (WORD)(pItemInfo->Life*calc);
			if( pShopItemOption->Life <= 0 )
				pShopItemOption->Life = 0;	
			
			if( bAdd==FALSE )
				CHARCALCMGR->CalcCharLife(m_pPlayer);
		}
		// 최대내력 증가치
		if( pItemInfo->Mana > 0 )
		{
			pShopItemOption->Mana += (pItemInfo->Mana*calc);
			if( pShopItemOption->Mana <= 0 )
				pShopItemOption->Mana = 0;

			if( bAdd==FALSE )
				CHARCALCMGR->CalcCharMana(m_pPlayer);
		}
	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION )
	{
		// CheckAvatarEquip( pItemInfo->ItemIdx, bAdd );
	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_SUNDRIES )
	{
		// 체인지아이템
		if( pItemInfo->Wis )
		{
			CHANGEITEMMGR->UseChangeItemFromShopItem( m_pPlayer, pItemInfo->Wis );
		}
	}
*/	return TRUE;
}


void CShopItemManager::SendShopItemUseInfo()
{
	SEND_SHOPITEM_USEDINFO msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_SHOPITEM_USEDINFO;
	msg.ItemCount = 0;

	SHOPITEMWITHTIME* pItem = NULL; 
	m_UsingItemTable.SetPositionHead();
	while( pItem = m_UsingItemTable.GetData() )
	{
		memcpy( &msg.Item[msg.ItemCount], &pItem->ShopItem, sizeof(SHOPITEMBASE) );
		++msg.ItemCount;
	}

	m_pPlayer->SendMsg(&msg, msg.GetSize());
}


BOOL CShopItemManager::AddMovePoint( MOVEDATA* pData )
{
	DWORD ValidCount = MAX_MOVEDATA_PERPAGE;
	if( m_UsingItemTable.GetData( eIncantation_MemoryMoveExtend ) ||
		m_UsingItemTable.GetData( eIncantation_MemoryMove2 ) )
		ValidCount = MAX_MOVEDATA_PERPAGE*MAX_MOVEPOINT_PAGE;

	if( m_MovePointTable.GetDataNum() >= ValidCount )
		return FALSE;

	MOVEDATA* pNewData = m_MovePointPool->Alloc();
	memcpy(pNewData, pData, sizeof(MOVEDATA));

	m_MovePointTable.Add( pNewData, pNewData->DBIdx );

	return TRUE;
}


BOOL CShopItemManager::DeleteMovePoint( DWORD DBIdx )
{
	MOVEDATA* pData = m_MovePointTable.GetData( DBIdx );

	if( !pData )
		return FALSE;

	m_MovePointTable.Remove( DBIdx );
	m_MovePointPool->Free( pData );

	return TRUE;
}


BOOL CShopItemManager::ReNameMovePoint( DWORD DBIdx, char* newName )
{
	MOVEDATA* pData = m_MovePointTable.GetData( DBIdx );

	if( !pData )
		return FALSE;

	memset(pData->Name, 0, MAX_SAVEDMOVE_NAME);
	strncpy(pData->Name, newName, MAX_SAVEDMOVE_NAME-1);

	return TRUE;
}


void CShopItemManager::ReleseMovePoint()
{
	m_MovePointTable.SetPositionHead();
	MOVEDATA* pData = NULL;
	while( pData = m_MovePointTable.GetData() )
	{
		m_MovePointPool->Free( pData );
	}
	m_MovePointTable.RemoveAll();
}


SHOPITEMWITHTIME* CShopItemManager::GetUsingItemInfo( DWORD ItemIdx )
{
	return m_UsingItemTable.GetData( ItemIdx );
}


void CShopItemManager::AddShopItemUse( SHOPITEMBASE* pShopItem )
{
	SHOPITEMWITHTIME* pAddShopItem = m_UsingItemPool->Alloc();

	memcpy( &pAddShopItem->ShopItem, pShopItem, sizeof(SHOPITEMBASE) );
	pAddShopItem->LastCheckTime = gCurTime;

	m_UsingItemTable.Add( pAddShopItem, pShopItem->ItemBase.wIconIdx );
}



BOOL CShopItemManager::PutOnAvatarItem( DWORD ItemIdx, WORD ItemPos, BOOL bCalcStats )
{
/*	DWORD* pAvatar = m_pPlayer->GetShopItemStats()->Avatar;
	if(!pAvatar)		return FALSE;
	AVATARITEM* pAvatarEquip = GAMERESRCMNGR->m_AvatarEquipTable.GetData( ItemIdx );
	if(!pAvatarEquip)	return FALSE;
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( ItemIdx );
	if(!pItemInfo)		return FALSE;
*/
/*
//****************************************************************************************************
//	예외처리
//****************************************************************************************************
	// 청동의나 적월포 이면
	if( pAvatarEquip->Position == eAvatar_Hat || pAvatarEquip->Position == eAvatar_Hair )
	{
		if( pAvatar[eAvatar_Dress] == 55541 || pAvatar[eAvatar_Dress] == 55542 ||
			pAvatar[eAvatar_Dress] == 57564 || pAvatar[eAvatar_Dress] == 57565 )
			return FALSE;
	}
//****************************************************************************************************
*/
/*	if( pAvatarEquip->Position == eAvatar_Hat && pAvatar[eAvatar_Dress] )
	{
		AVATARITEM* pDressEquip = GAMERESRCMNGR->m_AvatarEquipTable.GetData( pAvatar[eAvatar_Dress] );
		if( pDressEquip )
		{
			if( pDressEquip->Item[eAvatar_Hat] == 0 )
				return FALSE;
		}
	}

	
	SHOPITEMWITHTIME* pItem = NULL;
	for(int i=0; i<eAvatar_Weared_Gum; i++)
	{
		if( i >= eAvatar_Weared_Hair )
		{
			if( !pAvatarEquip->Item[i] )
				pAvatar[i] = 0;
		}
		else
		{
			if( i == pAvatarEquip->Position )
			{
				if( pAvatar[i] )
				{
					// 기존의 아이템이 벗겨졌으면 기본아이템을 다시 셋팅해준다.
					AVATARITEM* pTemp = GAMERESRCMNGR->m_AvatarEquipTable.GetData( pAvatar[i] );
					if( !pTemp )		continue;
					ITEM_INFO* ptInfo = ITEMMGR->GetItemInfo( pAvatar[i] );
					if(!ptInfo)		continue;

					for(int n=eAvatar_Weared_Hair; n<eAvatar_Weared_Gum; n++)
					{
						if( !pTemp->Item[n] )
							pAvatar[n] = 1;
					}

					ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), pAvatar[i], ptInfo->SellPrice );
					if( pItem = m_UsingItemTable.GetData( ItemIdx ) )
						pItem->ShopItem.Param = ptInfo->SellPrice;
				}
				
				pAvatar[i] = ItemIdx;
				
				ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), ItemIdx, eShopItemUseParam_EquipAvatar );
				if( pItem = m_UsingItemTable.GetData( ItemIdx ) )
					pItem->ShopItem.Param = eShopItemUseParam_EquipAvatar;
			}
			
			if( !pAvatarEquip->Item[i] && pAvatar[i] )
			{
				// 기존의 아이템이 벗겨졌으면 기본아이템을 다시 셋팅해준다.
				AVATARITEM* pTemp = GAMERESRCMNGR->m_AvatarEquipTable.GetData( pAvatar[i] );
				if( !pTemp )		continue;
				ITEM_INFO* ptInfo = ITEMMGR->GetItemInfo( pAvatar[i] );
				if(!ptInfo)		continue;
				
				for(int n=eAvatar_Weared_Hair; n<eAvatar_Weared_Gum; n++)
				{
					if( !pTemp->Item[n] )
						pAvatar[n] = 1;
				}				
				
				ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), pAvatar[i], ptInfo->SellPrice );
				if( pItem = m_UsingItemTable.GetData( ItemIdx ) )
					pItem->ShopItem.Param = ptInfo->SellPrice;
				pAvatar[i] = 0;
			}
		}
	}
*/
/*
//****************************************************************************************************
//	예외처리
//****************************************************************************************************
	// 드레스일때 곰돌이머리가 있으면 벗겨준다.
	if( ItemIdx == 55535 || ItemIdx == 55536 ||
		ItemIdx == 57575 || ItemIdx == 57576 ||
		ItemIdx == 57577 || ItemIdx == 57578 ||
		ItemIdx == 57579 || ItemIdx == 57580 )
	{
		if( pAvatar[eAvatar_Hat] == 55543 || pAvatar[eAvatar_Hat] == 55545 || pAvatar[eAvatar_Hat] == 55547 ||
			pAvatar[eAvatar_Hat] == 55572 || pAvatar[eAvatar_Hat] == 55574 || pAvatar[eAvatar_Hat] == 55580 || pAvatar[eAvatar_Hat] == 55582 )
		{
			ITEM_INFO* ptInfo = ITEMMGR->GetItemInfo( pAvatar[eAvatar_Hat] );
			if( ptInfo )
			{
				ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), pAvatar[eAvatar_Hat], ptInfo->SellPrice );
				if( pItem = m_UsingItemTable.GetData( pAvatar[eAvatar_Hat] ) )
					pItem->ShopItem.Param = ptInfo->SellPrice;
				pAvatar[eAvatar_Hat] = 0;
				pAvatar[eAvatar_Weared_Hair] = 1;
			}
		}
	}
	// 곰돌이머리면 드레스를 벗겨준다.
	else if( ItemIdx == 55543 || ItemIdx == 55545 || ItemIdx == 55547 || ItemIdx == 55572 || ItemIdx == 55574 || ItemIdx == 55580 || ItemIdx == 55582 )
	{
		if( pAvatar[eAvatar_Dress] == 55535 || pAvatar[eAvatar_Dress] == 55536 ||
			pAvatar[eAvatar_Dress] == 57575 || pAvatar[eAvatar_Dress] == 57576 ||
			pAvatar[eAvatar_Dress] == 57577 || pAvatar[eAvatar_Dress] == 57578 ||
			pAvatar[eAvatar_Dress] == 57579 || pAvatar[eAvatar_Dress] == 57580 )
		{
			ITEM_INFO* ptInfo = ITEMMGR->GetItemInfo( pAvatar[eAvatar_Dress] );
			if( ptInfo )
			{
				ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), pAvatar[eAvatar_Dress], ptInfo->SellPrice );
				if( pItem = m_UsingItemTable.GetData( pAvatar[eAvatar_Dress] ) )
					pItem->ShopItem.Param = ptInfo->SellPrice;
				pAvatar[eAvatar_Dress] = 0;
				pAvatar[eAvatar_Weared_Dress] = 1;
			}
		}
	}
//****************************************************************************************************
*/
/*
	if( ItemPos )
	{
		SEND_AVATARITEM_INFO msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_AVATAR_INFO;
		msg.PlayerId = m_pPlayer->GetID();
		msg.ItemIdx = ItemIdx;
		msg.ItemPos = ItemPos;
		memcpy( &msg.Avatar, pAvatar, sizeof(WORD)*eAvatar_Max );
		PACKEDDATA_OBJ->QuickSend( m_pPlayer, &msg, sizeof(msg) );
	}

	//
	CalcAvatarOption( bCalcStats );
*/
	return TRUE;
}


void CShopItemManager::TakeOffAvatarItem( DWORD ItemIdx, WORD ItemPos, BOOL bCalcStats  )
{
/*	DWORD* pAvatar = m_pPlayer->GetShopItemStats()->Avatar;
	if(!pAvatar)		return ;
	AVATARITEM* pAvatarEquip = GAMERESRCMNGR->m_AvatarEquipTable.GetData( ItemIdx );
	if(!pAvatarEquip)	return ;
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( ItemIdx );
	if(!pItemInfo)		return;
	

	if( pAvatar[pAvatarEquip->Position] != ItemIdx )
		return;

	SHOPITEMWITHTIME* pItem = NULL;
	for(int i=0; i<eAvatar_Max; i++)
	{
		if( pAvatar[i] )
		{
			if( i==pAvatarEquip->Position )
			{
				pAvatar[i] = 0;
				for(int n=eAvatar_Weared_Hair; n<eAvatar_Weared_Gum; n++)
				{
					if( !pAvatarEquip->Item[n] )
						pAvatar[n] = 1;
				}

				ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), ItemIdx, pItemInfo->SellPrice );
				if( pItem = m_UsingItemTable.GetData( ItemIdx ) )
					pItem->ShopItem.Param = pItemInfo->SellPrice;
			}
			if( pAvatarEquip->Item[i] == 0 )
			{
				// 같이 벗겨져야할 아바타 아이템들
				if( i>=eAvatar_Hat && i<eAvatar_Weared_Hair )
				{
					ITEM_INFO* poffinfo = ITEMMGR->GetItemInfo( pAvatar[i] );
					if(!poffinfo)		return;

					ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), pAvatar[i], poffinfo->SellPrice );
					if( pItem = m_UsingItemTable.GetData( pAvatar[i] ) )
						pItem->ShopItem.Param = poffinfo->SellPrice;
					pAvatar[i] = 0;
				}
			}
		}
	}
	
	/*
	// 가발류일때
	if( ( ItemIdx >= 55401 && ItemIdx <= 55412 ) ||
		( ItemIdx >= 57553 && ItemIdx <= 57556 ) ||
		( ItemIdx >= 57512 && ItemIdx <= 57515 ) )
	{
		if( pAvatar[eAvatar_Dress] == 55535 || pAvatar[eAvatar_Dress] == 55536 ||	//드레스
			pAvatar[eAvatar_Dress] == 57575 || pAvatar[eAvatar_Dress] == 57576 ||
			pAvatar[eAvatar_Dress] == 57577 || pAvatar[eAvatar_Dress] == 57578 ||
			pAvatar[eAvatar_Dress] == 57579 || pAvatar[eAvatar_Dress] == 57580 ||

			pAvatar[eAvatar_Dress] == 55533 || pAvatar[eAvatar_Dress] == 55534 ||	//체육복
			pAvatar[eAvatar_Dress] == 57569 || pAvatar[eAvatar_Dress] == 57570 ||
			pAvatar[eAvatar_Dress] == 57571 || pAvatar[eAvatar_Dress] == 57572 ||
			pAvatar[eAvatar_Dress] == 57573 || pAvatar[eAvatar_Dress] == 57574 )
		{
			pAvatar[eAvatar_Weared_Hat] = 0 ;
		}
	}
	*/
/*
	SEND_AVATARITEM_INFO msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_SHOPITEM_AVATAR_INFO;
	msg.PlayerId = m_pPlayer->GetID();
	msg.ItemIdx = ItemIdx;
	msg.ItemPos = ItemPos;
	memcpy( &msg.Avatar, pAvatar, sizeof(WORD)*eAvatar_Max );
	PACKEDDATA_OBJ->QuickSend( m_pPlayer, &msg, sizeof(msg) );	

	//	
	CalcAvatarOption( bCalcStats );*/
}


void CShopItemManager::CalcAvatarOption( BOOL bCalcStats )
{
//	DWORD* pAvatar = m_pPlayer->GetShopItemStats()->Avatar;
//	if(!pAvatar)		return ;
/*
	AVATARITEMOPTION* pAvatarOption = m_pPlayer->GetAvatarOption();
	memset( pAvatarOption, 0, sizeof(AVATARITEMOPTION) );

	for(int i=0; i<eAvatar_Max; i++)
	{
		if( i >= eAvatar_Weared_Hair )		break;
		if( pAvatar[i]==0 )					continue;

		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pAvatar[i] );
		if( !pItemInfo )		continue;

		// 근골
		if( pItemInfo->Str > 0 )
			pAvatarOption->Str += pItemInfo->Str;
		// 민첩
		if( pItemInfo->Dex > 0 )
			pAvatarOption->Dex += pItemInfo->Dex;
		// 체력
		if( pItemInfo->Vit > 0 )
			pAvatarOption->Vit += pItemInfo->Vit;
		// 심맥
		if( pItemInfo->Wis > 0 )
			pAvatarOption->Wis += pItemInfo->Wis;
		// 최대생명력
		if( pItemInfo->Life > 0 )
			pAvatarOption->Life += (WORD)pItemInfo->Life;
		// 최대내력
		if( pItemInfo->Mana > 0 )
			pAvatarOption->Mana += pItemInfo->Mana;
		// 무기공격력
		if( pItemInfo->MeleeAttackMin > 0 )
			pAvatarOption->Attack += pItemInfo->MeleeAttackMin;
		// 크리티컬수치
		if( pItemInfo->CriticalPercent > 0 )
			pAvatarOption->Critical += pItemInfo->CriticalPercent;
		// 반격확율
		if( pItemInfo->Plus_SkillIdx > 0 )
			pAvatarOption->CounterPercent += pItemInfo->Plus_SkillIdx;
		// 반격시 데미지율
		if( pItemInfo->Plus_Value > 0 )
			pAvatarOption->CounterDamage += pItemInfo->Plus_Value;
		// 경공딜레이
		if( pItemInfo->AllPlus_Kind == 1 )
			pAvatarOption->bKyungGong = 1;
		// 내력소모감소
		if( pItemInfo->LimitVit > 0 )
			pAvatarOption->ManaSpend += pItemInfo->LimitVit;
		// 내공 데미지
		if( pItemInfo->LimitJob > 0 )
			pAvatarOption->NeagongDamage += pItemInfo->LimitJob;
		// 외공 데미지
		if( pItemInfo->LimitGender > 0 )
			pAvatarOption->WoigongDamage += pItemInfo->LimitGender;
		// 몬스터 물리방어력 감소
		if( pItemInfo->LimitLevel > 0 )
			pAvatarOption->TargetPhyDefDown += pItemInfo->LimitLevel;
		// 몬스터 속성저항력 감소
		if( pItemInfo->LimitStr > 0 )
			pAvatarOption->TargetAttrDefDown += pItemInfo->LimitStr;
		// 몬스터 공격력 감소
		if( pItemInfo->LimitDex > 0 )
			pAvatarOption->TargetAtkDown += pItemInfo->LimitDex;
		// 회복량 추가 증가치
		if( pItemInfo->LimitWis )
			pAvatarOption->RecoverRate += pItemInfo->LimitWis;
		// 경공속도 상승
		if( pItemInfo->ItemGrade )
			pAvatarOption->KyunggongSpeed += pItemInfo->ItemGrade;
		// 묵혼게이지 충전속도
        if( pItemInfo->RangeType )
			pAvatarOption->MussangCharge += pItemInfo->RangeType;
		// 경공사용시 내력소모여부
		if( pItemInfo->EquipKind == 1 )
			pAvatarOption->NaeruykspendbyKG = 1;
		///////////////////////////////////////////////////////////////////////////////////////////////////
		// 06. 07 내공 적중(일격) - 이영준
		if( pItemInfo->ManaRecover > 0 )
			pAvatarOption->Decisive += pItemInfo->ManaRecover;
		///////////////////////////////////////////////////////////////////////////////////////////////////
		
	}

	// 2005 크리스마스 이벤트
	// 눈내릴때 추가옵션 적용
/*	if( WEATHERMGR->GetWeatherState() == eWS_Snow )
	{
		if(	(pAvatar[eAvatar_Dress] == EVENT_SHOPITEM_SNOWMAN_DRESS && pAvatar[eAvatar_Hat] == EVENT_SHOPITEM_SNOWMAN_HAT) ||
			(pAvatar[eAvatar_Dress] ==  EVENT_SHOPITEM_RUDOLP_DRESS && pAvatar[eAvatar_Hat] ==  EVENT_SHOPITEM_RUDOLP_HAT) ||
			(pAvatar[eAvatar_Dress] == EVENT_SHOPITEM_SNOWMAN_DRESS2 && pAvatar[eAvatar_Hat] == EVENT_SHOPITEM_SNOWMAN_HAT2) ||
			(pAvatar[eAvatar_Dress] ==  EVENT_SHOPITEM_RUDOLP_DRESS2 && pAvatar[eAvatar_Hat] ==  EVENT_SHOPITEM_RUDOLP_HAT2) ||
			(pAvatar[eAvatar_Dress] == EVENT_SHOPITEM_SNOWMAN_DRESS && pAvatar[eAvatar_Hat] == EVENT_SHOPITEM_SNOWMAN_HAT2) ||
			(pAvatar[eAvatar_Dress] ==  EVENT_SHOPITEM_RUDOLP_DRESS && pAvatar[eAvatar_Hat] ==  EVENT_SHOPITEM_RUDOLP_HAT2) ||
			(pAvatar[eAvatar_Dress] == EVENT_SHOPITEM_SNOWMAN_DRESS2 && pAvatar[eAvatar_Hat] == EVENT_SHOPITEM_SNOWMAN_HAT) ||
			(pAvatar[eAvatar_Dress] ==  EVENT_SHOPITEM_RUDOLP_DRESS2 && pAvatar[eAvatar_Hat] ==  EVENT_SHOPITEM_RUDOLP_HAT) )
		{
			pAvatarOption->Life += 100;
			pAvatarOption->Shield += 100;
			pAvatarOption->Naeruyk += 100;
		}
	}
*/
	if( bCalcStats )
		m_pPlayer->CalcState();
}


void CShopItemManager::DiscardAvatarItem( DWORD ItemIdx, WORD ItemPos )
{
/*	DWORD* pAvatar = m_pPlayer->GetShopItemStats()->Avatar;
	if(!pAvatar)		return ;
	AVATARITEM* pAvatarEquip = GAMERESRCMNGR->m_AvatarEquipTable.GetData( ItemIdx );
	if(!pAvatarEquip)	return ;
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( ItemIdx );
	if(!pItemInfo)		return;

	if( pAvatar[pAvatarEquip->Position] != ItemIdx )
		return;

	for(int i=0; i<eAvatar_Max; i++)
	{
		if( i==pAvatarEquip->Position && pAvatar[i] )
		{
			pAvatar[i] = 0;
			for(int n=eAvatar_Weared_Hair; n<eAvatar_Weared_Gum; n++)
			{
				if( !pAvatarEquip->Item[n] )
					pAvatar[n] = 1;
			}
						
			ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), ItemIdx, pItemInfo->SellPrice );
		}
	}

	SEND_AVATARITEM_INFO msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_SHOPITEM_AVATAR_DISCARD;
	msg.PlayerId = m_pPlayer->GetID();
	msg.ItemIdx = ItemIdx;
	msg.ItemPos = ItemPos;
	memcpy( &msg.Avatar, pAvatar, sizeof(WORD)*eAvatar_Max );
	PACKEDDATA_OBJ->QuickSend( m_pPlayer, &msg, sizeof(msg) );	

	//
	CalcAvatarOption();
*/}


//void CShopItemManager::CalcPlusTime( DWORD dwEventIdx, DWORD dwType )
//{	
///*	ITEM_INFO* pItem = NULL;
//	SHOPITEMWITHTIME* pShopItem = NULL;
///*
//	//
//	m_UsingItemTable.SetPositionHead();
//
//	while( pShopItem = m_UsingItemTable.GetData() )
//	{
//		pItem = ITEMMGR->GetItemInfo( pShopItem->ShopItem.ItemBase.wIconIdx );
//		if( !pItem )		continue;
//
//		if( pItem->ItemKind == eSHOP_ITEM_CHARM )
//		{
//			switch( dwType )
//			{
//			case 0:
//				{
//					if( pShopItem->ShopItem.Remaintime )
//					if( gEventRate[pItem->MeleeAttackMin] != gEventRateFile[pItem->MeleeAttackMin] )
//						CalcShopItemOption( pItem->ItemIdx, FALSE );
//				}
//				break;
//			case MP_CHEAT_PLUSTIME_ON:
//				{
//					if( dwEventIdx == pItem->MeleeAttackMin && pShopItem->ShopItem.Remaintime )
//					{
//						CalcShopItemOption( pItem->ItemIdx, FALSE );
//						return;
//					}
//				}
//				break;
//			case MP_CHEAT_PLUSTIME_OFF:
//				{
//					if( dwEventIdx == pItem->MeleeAttackMin && pShopItem->ShopItem.Remaintime )
//					{
//						pShopItem->LastCheckTime = gCurTime;
//						CalcShopItemOption( pItem->ItemIdx, TRUE );
//						return;
//					}
//				}
//				break;
//			case MP_CHEAT_PLUSTIME_ALLOFF:
//				{
//					if( pShopItem->ShopItem.Remaintime )
//					if( gEventRate[pItem->MeleeAttackMin] != gEventRateFile[pItem->MeleeAttackMin] )
//					{
//						pShopItem->LastCheckTime = gCurTime;
//						CalcShopItemOption( pItem->ItemIdx, TRUE );
//					}
//				}
//				break;
//			}
//		}
//	}	*/
//}


void CShopItemManager::AddDupParam( ITEM_INFO* pItemInfo )
{
	if( !pItemInfo )			return;
/*
	// 부적
	if( pItemInfo->AllPlus_Value )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->AllPlus_Value );
		if( !pDupOption )		return;

		if( pDupOption->Param & eDontDupUse_WoigongDamage )			// 귀신의 부적(외공데미지)
			m_DupCharm |= eDontDupUse_WoigongDamage;
		if( pDupOption->Param & eDontDupUse_NaegongDamage )			// 강시의 부적(내공데미지)
			m_DupCharm |= eDontDupUse_NaegongDamage;	
		if( pDupOption->Param & eDontDupUse_Exppoint )				// 플러스타임 경험치
			m_DupCharm |= eDontDupUse_Exppoint;	
		if( pDupOption->Param & eDontDupUse_Reinforce )				// 강화증폭
			m_DupCharm |= eDontDupUse_Reinforce;	
	}
	
	// 물약
	if( pItemInfo->SkillNum )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->SkillNum );
		if( !pDupOption )		return;
	}

	// 주문서
	if( pItemInfo->SkillType )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->SkillType );
		if( !pDupOption )		return;

		if( pDupOption->Param & eDontDupUse_MemoryMove )		// 기억이동 주문서
			m_DupIncantation |= eDontDupUse_MemoryMove;			
		if( pDupOption->Param & eDontDupUse_ProtectAll )		// 수호자의 주문서
			m_DupIncantation |= eDontDupUse_ProtectAll;
		if( pDupOption->Param & eDontDupUse_LevelCancel50 )		// 50렙이하 장비해제
			m_DupIncantation |= eDontDupUse_LevelCancel50;
		if( pDupOption->Param & eDontDupUse_LevelCancel70 )		// 70렙이하 장비해제
			m_DupIncantation |= eDontDupUse_LevelCancel70;
		if( pDupOption->Param & eDontDupUse_LevelCancel90 )		// 90렙이하 장비해제
			m_DupIncantation |= eDontDupUse_LevelCancel90;
		if( pDupOption->Param & eDontDupUse_ShowStorage )		// 창고소환주문서
			m_DupIncantation |= eDontDupUse_ShowStorage;
		if( pDupOption->Param & eDontDupUse_Chase )				// 추적주문서
			m_DupIncantation |= eDontDupUse_Chase;
		if( pDupOption->Param & eDontDupUse_TownMove )			// 마을이동주문서
			m_DupIncantation |= eDontDupUse_TownMove;
	}

	if( pItemInfo->LifeRecover )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->LifeRecover );
		if( !pDupOption )		return;

		if( pDupOption->Param & eDontDupUse_StreeStall )		// 노점개설
		{
			m_DupSundries |= eDontDupUse_StreeStall;
			m_pPlayer->GetShopItemStats()->bStreetStall = 1;
		}
	}*/
}


void CShopItemManager::DeleteDupParam( ITEM_INFO* pItemInfo )
{
	if( !pItemInfo )			return;
/*
	// 부적
	if( pItemInfo->AllPlus_Value )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->AllPlus_Value );
		if( !pDupOption )		return;

		if( (pDupOption->Param & eDontDupUse_WoigongDamage) &&
			(m_DupCharm & eDontDupUse_WoigongDamage) )				// 귀신의 부적(외공데미지)
			m_DupCharm ^= eDontDupUse_WoigongDamage;
		if( (pDupOption->Param & eDontDupUse_NaegongDamage) &&
			(m_DupCharm & eDontDupUse_NaegongDamage) )				// 강시의부적(내공데미지)
			m_DupCharm ^= eDontDupUse_NaegongDamage;	
		if( (pDupOption->Param & eDontDupUse_Exppoint) &&
			(m_DupCharm & eDontDupUse_Exppoint) )					// 플러스타임 경험치
			m_DupCharm ^= eDontDupUse_Exppoint;
		if( (pDupOption->Param & eDontDupUse_Reinforce) &&
			(m_DupCharm & eDontDupUse_Reinforce) )					// 강화증폭
			m_DupCharm ^= eDontDupUse_Reinforce;
	}
	
	// 물약
	if( pItemInfo->SkillNum )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->SkillNum );
		if( !pDupOption )		return;
	}

	// 주문서
	if( pItemInfo->SkillType )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->SkillType );
		if( !pDupOption )		return;

		if( (pDupOption->Param & eDontDupUse_MemoryMove) && 
			(m_DupIncantation & eDontDupUse_MemoryMove) )					// 기억이동 주문서
			m_DupIncantation ^= eDontDupUse_MemoryMove;			
		if( (pDupOption->Param & eDontDupUse_ProtectAll) &&
			(m_DupIncantation & eDontDupUse_ProtectAll) )					// 수호자의 주문서
			m_DupIncantation ^= eDontDupUse_ProtectAll;
	}

	if( pItemInfo->LifeRecover )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->LifeRecover );
		if( !pDupOption )		return;

		if( (pDupOption->Param & eDontDupUse_StreeStall) &&
			(m_DupSundries & eDontDupUse_StreeStall) )					// 노점개설
		{
			m_DupSundries ^= eDontDupUse_StreeStall;
			m_pPlayer->GetShopItemStats()->bStreetStall = 0;
		}
	}*/
}


BOOL CShopItemManager::IsDupAble( ITEM_INFO* pItemInfo )
{
	if( !pItemInfo )			return FALSE;
/*
	// 부적
	if( pItemInfo->AllPlus_Value )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->AllPlus_Value );
		if( !pDupOption )		return FALSE;

		if( (pDupOption->Param & eDontDupUse_WoigongDamage) &&			// 귀신의 부적(외공데미지)
			(m_DupCharm & eDontDupUse_WoigongDamage) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_NaegongDamage) &&			// 강시의 부적(내공데미지)
			(m_DupCharm & eDontDupUse_NaegongDamage) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_Exppoint) &&				// 플러스타임 경험치
			(m_DupCharm & eDontDupUse_Exppoint) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_Reinforce) &&				// 강화증폭
			(m_DupCharm & eDontDupUse_Reinforce) )
			return FALSE;
	}
	
	// 물약
	if( pItemInfo->SkillNum )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->SkillNum );
		if( !pDupOption )		return FALSE;
	}

	// 주문서
	if( pItemInfo->SkillType )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->SkillType );
		if( !pDupOption )		return FALSE;

		if( (pDupOption->Param & eDontDupUse_MemoryMove) &&			// 기억이동 주문서
			(m_DupIncantation & eDontDupUse_MemoryMove) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_ProtectAll) &&			// 수호자의 주문서
			(m_DupIncantation & eDontDupUse_ProtectAll) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_LevelCancel50) &&		// 수호자의 주문서
			(m_DupIncantation & eDontDupUse_LevelCancel50) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_LevelCancel70) &&		// 수호자의 주문서
			(m_DupIncantation & eDontDupUse_LevelCancel70) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_LevelCancel90) &&		// 수호자의 주문서
			(m_DupIncantation & eDontDupUse_LevelCancel90) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_ShowStorage) &&			// 창고소환 주문서
			(m_DupIncantation & eDontDupUse_ShowStorage) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_Chase) &&				// 추적 주문서
			(m_DupIncantation & eDontDupUse_Chase) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_TownMove) &&			// 마을이동 주문서
			(m_DupIncantation & eDontDupUse_TownMove) )
			return FALSE;
	}

	// 잡화류
	if( pItemInfo->LifeRecover )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->LifeRecover );
		if( !pDupOption )		return FALSE;

		if( (pDupOption->Param & eDontDupUse_StreeStall) &&			// 노점개설
			(m_DupSundries & eDontDupUse_StreeStall) )
			return FALSE;
	}
*/
	return TRUE;
}

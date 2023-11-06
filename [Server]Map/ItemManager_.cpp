#include "stdafx.h"
#include "ItemManager.h"
#include "ItemSlot.h"
#include "Player.h"
#include "Purse.h"
#include "ItemContainer.h"
#include "MHFile.h"
#include "MapDBMsgParser.h"
#include "UserTable.h"
#include "KyunggongManager.h"
#include "SkillTreeManager.h"
#include "CharacterCalcManager.h"
#include "GuildManager.h"
#include "GuildScore.h"
#include "CheckRoutine.h"
#include "MHError.h"
#include "PartyManager.h"
#include "LootingManager.h"
#include "QuestManager.h"
#include "BattleSystem_Server.h"
#include "..\[CC]Header\GameResourceStruct.h"
#include "ObjectStateManager.h"
#include "MHMap.h"
// 080923 LUJ, â�� ��ȯ�� ���� ����
#include "StorageManager.h"
#include "ChangeItemMgr.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "PackedData.h"
#include "Network.h"
#include "SiegeWarProfitMgr.h"
#include "WantedManager.h"
#include "MapNetworkMsgParser.h"

#include "ChannelSystem.h"

//081027 NYJ - ���ͼ�ȯ������
#include "AISystem.h"
#include "Npc.h"
#include "RecallManager.h"

#include "../[cc]skill/server/manager/skillmanager.h"

//#include "Distribute_Random.h"

// desc_hseos_����ý���_01
// S ����ý��� �߰� added by hseos 2007.05.15
#include "../hseos/SHMain.h"
// E ����ý��� �߰� added by hseos 2007.05.15

// desc_hseos_ü����������01
// S ü���������� �߰� added by hseos 2007.07.29
#include "../hseos/Common/SHMath.h"
// E ü���������� �߰� added by hseos 2007.07.29

#include "FriendManager.h"
// desc_hseos_����Ʈ ��_01
// S ����Ʈ �� �߰� added by hseos 2007.12.01
#include "../hseos/Date/SHDateManager.h"
// E ����Ʈ �� �߰� added by hseos 2007.12.01

// desc_hseos_�ֹε��
#include "../hseos/Residentregist/SHResidentRegistManager.h"

#include "MHFile.h"

#include "FriendManager.h"
#include "PetManager.h"
#include "Pet.h"

CItemManager::CItemManager()
{
	m_ItemInfoList.Initialize(MAX_ITEM_NUM);
	m_ItemArrayPool.Init(500, 100, "ItemArrayPool" );
	m_ArrayIndexCreator.Init(MAX_ITEM_NUM*10, 1);
	m_DealerTable.Initialize(32);
	m_AvatarEquipTable.Initialize(50);
	m_Key = 0;

	// desc_hseos_ü����������01
	// S ü���������� �߰� added by hseos 2007.07.29
	m_stChangeItemInfo = NULL;
	m_nChangeItemInfoNum = 0;
	// E ü���������� �߰� added by hseos 2007.07.29

	mOptionPool.Init( 5000, 5000, "Pool:ItemOption" );

	LoadSetScript();
	LoadMixScript();
	LoadReinforceScript();
	LoadDissolveScript();
	LoadEnchantScript();
	LoadMixSetting();
	LoadCoolTimeScript();
	LoadDropOptionScript();
	LoadApplyOptionScript();

	// 080414 LUJ, �ܾ� ���� ��ũ��Ʈ �ε�
	LoadBodyChangeScript();
}


CItemManager::~CItemManager()
{
	ITEM_INFO * pInfo = NULL;
	m_ItemInfoList.SetPositionHead();
	while(pInfo = m_ItemInfoList.GetData())
		delete pInfo;
	m_ItemInfoList.RemoveAll();

	DealerData* pDealer = NULL;
	m_DealerTable.SetPositionHead();
	while(pDealer = m_DealerTable.GetData())
	{
		POS pos = pDealer->m_DealItemList.GetFirstPos();
		while( pos )
		{
			DealerItem* pDI = pDealer->m_DealItemList.GetNextPos( pos );
			if( pDI ) delete pDI;
		}

		pDealer->m_DealItemList.DeleteAll();
		delete pDealer;
	}
	m_DealerTable.RemoveAll();


	for(
		OptionMap::iterator it = mOptionMap.begin();
		mOptionMap.end() != it;
		++it )
	{
		SAFE_DELETE( it->second );
	}
	
	AVATARITEM* pItem = NULL;
	m_AvatarEquipTable.SetPositionHead();
	while(pItem = m_AvatarEquipTable.GetData())
	{
		delete pItem;
	}
	m_AvatarEquipTable.RemoveAll();	

	m_ItemArrayPool.Release();
	m_ArrayIndexCreator.Release();

	// desc_hseos_ü����������01
	// S ü���������� �߰� added by hseos 2007.07.29
	if (m_stChangeItemInfo)
	{
		for(int i=0; i<m_nChangeItemInfoNum; i++)
		{
			for(int j=0; j<m_stChangeItemInfo[i].nResultNum; j++)
			{
				SAFE_DELETE_ARRAY(m_stChangeItemInfo[i].stResult[j].stResultBase);
			}
			SAFE_DELETE_ARRAY(m_stChangeItemInfo[i].stResult);
		}

		SAFE_DELETE_ARRAY(m_stChangeItemInfo);
	}
	// E ü���������� �߰� added by hseos 2007.07.29


	std::vector<stMonSummonGroupInfo*>::iterator iter;
	for(iter=m_vecMonSummonGroupInfo.begin(); iter!=m_vecMonSummonGroupInfo.end(); iter++)
	{
		if((*iter))
			SAFE_DELETE((*iter));
	}
}

ITEMOBTAINARRAYINFO *	CItemManager::Alloc(CPlayer * pPlayer, BYTE c, BYTE p, DWORD dwObjectID, DWORD dwFurnisherIdx, WORD wType, WORD ObtainNum, DBResult CallBackFunc, DBResultEx CallBackFuncEx)
{
	//return m_ItemArrayPool.Alloc();
	ITEMOBTAINARRAYINFO * pArrayInfo = m_ItemArrayPool.Alloc();
	pArrayInfo->ItemArray.Init( c, p, dwObjectID );
	pArrayInfo->ItemArray.wObtainCount = ObtainNum;
	pArrayInfo->wObtainArrayID = m_ArrayIndexCreator.GenerateIndex();
	pArrayInfo->CallBack = CallBackFunc;
	pArrayInfo->CallBackEx = CallBackFuncEx;
	pArrayInfo->dwFurnisherIdx = dwFurnisherIdx;
	pArrayInfo->wType = wType;
	pPlayer->AddArray(pArrayInfo);
	return pArrayInfo;
}
void CItemManager::Free(CPlayer * pPlayer, ITEMOBTAINARRAYINFO * info)
{
	info->ItemArray.Clear();
	pPlayer->RemoveArray(info->wObtainArrayID);
	m_ArrayIndexCreator.ReleaseIndex(info->wObtainArrayID);
	m_ItemArrayPool.Free( info );
}

BOOL CItemManager::IsPetSummonItem( DWORD wItemIdx )
{
	ITEM_INFO* pItemInfo = GetItemInfo(wItemIdx);

	if(!pItemInfo)	return FALSE;

	//	if(pItemInfo->ItemKind == eQUEST_ITEM_PET || pItemInfo->ItemKind == eSHOP_ITEM_PET)
	//		return TRUE;
	//	else
	return FALSE;
}


BOOL CItemManager::IsDupItem( DWORD wItemIdx )
{
	/*	switch( GetItemKind( wItemIdx ) )
	{
	case eYOUNGYAK_ITEM:
	case eYOUNGYAK_ITEM_PET:
	case eYOUNGYAK_ITEM_UPGRADE_PET:
	case eEXTRA_ITEM_JEWEL:
	case eEXTRA_ITEM_MATERIAL:
	case eEXTRA_ITEM_METAL:
	case eEXTRA_ITEM_BOOK:
	case eEXTRA_ITEM_HERB:
	case eEXTRA_ITEM_ETC:
	case eEXTRA_ITEM_USABLE:
	case eSHOP_ITEM_CHARM:
	case eSHOP_ITEM_HERB:
	return TRUE;
	case eSHOP_ITEM_SUNDRIES:
	{
	ITEM_INFO* pItem = GetItemInfo( wItemIdx );
	if( !pItem )			return FALSE;

	if( pItem->Wis )			// ShopItem���� ChangeItem �ΰ͵�
	return FALSE;
	else if( pItem->Vit )	// ����
	return FALSE;
	else if( wItemIdx == eSundries_Shout )
	return FALSE;			
	}
	return TRUE;
	case eSHOP_ITEM_INCANTATION:
	{
	if( wItemIdx == eIncantation_TownMove15 || wItemIdx == eIncantation_MemoryMove15 ||
	wItemIdx == eIncantation_TownMove7 || wItemIdx == eIncantation_MemoryMove7 ||
	wItemIdx == eIncantation_TownMove7_NoTrade || wItemIdx == eIncantation_MemoryMove7_NoTrade ||
	wItemIdx == 55357 || wItemIdx == 55362 || wItemIdx == eIncantation_MemoryMoveExtend ||
	wItemIdx == eIncantation_MemoryMove2 ||
	wItemIdx == eIncantation_ShowStorage || wItemIdx == eIncantation_ChangeName ||
	wItemIdx == eIncantation_ChangeName_Dntrade ||
	wItemIdx == eIncantation_Tracking || wItemIdx == eIncantation_Tracking_Jin ||
	wItemIdx == eIncantation_ChangeJob ||
	wItemIdx == eIncantation_ShowStorage7 || wItemIdx == eIncantation_ShowStorage7_NoTrade ||
	wItemIdx == eIncantation_Tracking7 ||  wItemIdx == eIncantation_Tracking7_NoTrade ||
	wItemIdx== eIncantation_SkillExtend ||	wItemIdx == eIncantation_StorageExtend ||
	wItemIdx == eIncantation_InvenExtend ||	wItemIdx == eIncantation_CharacterSlot
	)
	return FALSE;
	}
	return TRUE;
	}
	return FALSE;
	*/
const ITEM_INFO* info = GetItemInfo( wItemIdx );

return info && info->Stack;
}

void CItemManager::SendErrorMsg( CPlayer * pPlayer, MSG_ITEM_ERROR * msg, int msgSize, int ECode )
{
	msg->ECode = ECode;
	pPlayer->SendMsg(msg, msgSize);
}
void CItemManager::SendAckMsg( CPlayer * pPlayer, MSGBASE * msg, int msgSize)
{
	pPlayer->SendMsg(msg, msgSize);
}

void CItemManager::SendGuildErrorMsg( CPlayer * pPlayer, BYTE Protocol, int ECode )
{
	MSG_ITEM_ERROR msg;
	msg.Category = MP_ITEM;
	msg.Protocol = Protocol;
	msg.dwObjectID = pPlayer->GetID();
	msg.ECode = ECode;
	pPlayer->SendMsg(&msg, sizeof(msg));
}

int CItemManager::MoveItem( CPlayer * pPlayer, DWORD FromItemIdx, POSTYPE FromPos, DWORD ToItemIdx, POSTYPE& ToPos)//, DWORD wIconIdx)
{
//	CItemSlot * pFromSlotD	= pPlayer->GetSlot(FromPos);
//	MHERROR->OutputFile("itemtable.txt", pFromSlotD->_ToStringSlot());

	CItemSlot * pFromSlot	= pPlayer->GetSlot(FromPos);
	CItemSlot * pToSlot		= pPlayer->GetSlot(ToPos);

	// check hacking
	if( !CheckHackItemMove( pPlayer, pFromSlot, pToSlot ) )	return eItemUseErr_Move;

	if( LOOTINGMGR->IsLootedPlayer( pPlayer->GetID() ) )
	{
		return 10;
	}

	if(FromPos == ToPos)
		return 1;

	if(!CHKRT->ItemOf(pPlayer, FromPos, FromItemIdx,0,0,CB_EXIST|CB_ICONIDX))
		return 2;

	if(!CHKRT->ItemOf(pPlayer, ToPos, ToItemIdx,0,0,CB_ICONIDX))
		return 3;

	const ITEMBASE * pItem = pFromSlot->GetItemInfoAbs(FromPos);
	const ITEMBASE * pToItem = pToSlot->GetItemInfoAbs(ToPos);
	
	// �Ⱓ�� �ֹ����� ������϶� �̵��Ұ�
	// if( FromItemIdx == eIncantation_TownMove15 || FromItemIdx == eIncantation_MemoryMove15 )
	ITEM_INFO* pInfo = GetItemInfo( FromItemIdx );
	ITEM_INFO* pToInfo = GetItemInfo( ToItemIdx );

	// 071125 KTH --- Storage Move Item Check "â�� �������� ������ ��� �α��� Ÿ�� üũ ����� �������� ������ Ǯ�� �ִ� ���� ������ ����."
	if( pToSlot == pPlayer->GetSlot(eItemTable_Storage)  )
	{
		if( (pItem->nSealed == eITEM_TYPE_UNSEAL && pInfo->nTimeKind == eKIND_PLAYTIME) || pInfo->Deposit == eITEM_TYPE_DEPOSIT_NONE )
			return eItemUseErr_Move;
	}

	// ����â��� ������ ������ ���� â������ �������̸� ���â�� ������ ����.
	if( pToSlot == pPlayer->GetSlot(eItemTable_MunpaWarehouse) )
	{
		if( (pItem->nSealed == eITEM_TYPE_UNSEAL && pInfo->nTimeKind == eKIND_PLAYTIME) || 
			pInfo->Deposit == eITEM_TYPE_DEPOSIT_NONE || 
			pInfo->Deposit == eITEM_TYPE_DEPOSIT_ONLY_STORAGE )
			return eItemUseErr_Move;
	}

	// 071126 KTH --- ShopStorage(����)���δ� �������� �ȱ�� ����.
	if( pToSlot == pPlayer->GetSlot(eItemTable_Shop) )
	{
		return eItemUseErr_Move;
	}
	// 071126 KTH --- Seal ������ �������� ������ �� ����.
	if( pToSlot == pPlayer->GetSlot(eItemTable_Weared) )
	{
		if( pItem->nSealed == eITEM_TYPE_SEAL )
			return eItemUseErr_Move;
		if( pToItem->nSealed == eITEM_TYPE_SEAL )
			return eItemUseErr_Move;
	}
	if( pFromSlot == pPlayer->GetSlot(eItemTable_Weared) )
	{
		if( pItem->nSealed == eITEM_TYPE_SEAL )
			return eItemUseErr_Move;
		if( pToItem->nSealed == eITEM_TYPE_SEAL )
			return eItemUseErr_Move;
	}
	else if( !pFromSlot->IsEmpty(FromPos) )
	{
		if( pFromSlot == pPlayer->GetSlot(eItemTable_Storage) && 
			pToItem->nSealed == eITEM_TYPE_UNSEAL && 
			pToInfo->nTimeKind == eKIND_PLAYTIME )
		{
			return eItemUseErr_Move;
		}
		// 081015 KTH --- �̵��� �����ϴ°��� â���̰� �̵� �Ϸ��� ���� �������� �̵� �Ұ� �������̶�� â��� �ü� ����.
		if( pFromSlot == pPlayer->GetSlot(eItemTable_Storage) &&
			pToInfo != NULL &&
			pToInfo->Deposit == eITEM_TYPE_DEPOSIT_NONE)
			return eItemUseErr_Move;
	}

	if(!CanbeMoved(pItem->wIconIdx,ToPos,pPlayer))
	{
		ASSERT(0); //A������OoAI��u�ϡ̡�����c��?�ˢ硧u���� A�ϩ��ˡ���c C�ϡ��ˡ�A��i��I.
		return 9;
	}
	if(!pToSlot->IsEmpty(ToPos))
	{
		pItem = pToSlot->GetItemInfoAbs(ToPos);
		if(pItem)
		{
			// Shop�������۰� �Ϲ����������� ��ȯ�� ���� üũ�Ѵ�.
			// CanbeMoved���� �Ϸ��� �����.
			ITEM_INFO* pInfo = GetItemInfo( FromItemIdx );
			ITEM_INFO* pToInfo = GetItemInfo( ToItemIdx );
			if( !pInfo || !pToInfo )			return 10;

			if(CanbeMoved(pItem->wIconIdx, FromPos, pPlayer) == FALSE)
			{
				ASSERT(0); //A������OoAI��u�ϡ̡�����c��?�ˢ硧u���� A�ϩ��ˡ���c C�ϡ��ˡ�A��i��I.
				return 10;
			}
		}
	}
	// From A����AI����i��?�ˢ硧u���� ��u����AIAU Ao��?o
	// 1.
	ITEMBASE DelFromItem;
	ZeroMemory( &DelFromItem, sizeof( DelFromItem ) );
	
	if(pFromSlot->DeleteItemAbs(pPlayer, FromPos, &DelFromItem) != EI_TRUE)
	{
		// Ao��?i����tA ��u����AIAU(FromPosition)AI ��u�Ϩ��ˡ�U 
		return 5;
	}

	ClearQuickPos(&DelFromItem, ToPos);
	// ToTable��?�ˢ硧u���� ��u����AIAU Ao��?o
	// 2.`
	ITEMBASE DelToItem;
	ZeroMemory( &DelToItem, sizeof( DelToItem ) );

	DelToItem.dwDBIdx = 0;
	if(pToSlot->DeleteItemAbs(pPlayer, ToPos, &DelToItem) != EI_TRUE)
	{
		// 0->1
		DelFromItem.Position = ToPos;
		// 3.
		if(pToSlot->InsertItemAbs(pPlayer, ToPos, &DelFromItem) != EI_TRUE)
		{
			DelFromItem.Position = FromPos;
			if(pFromSlot->InsertItemAbs(pPlayer, FromPos, &DelFromItem) != EI_TRUE)
				ASSERT(0);
			return 6;
		}
	}
	else
	{
		SWAPVALUE(DelFromItem.Position, DelToItem.Position);
		ClearQuickPos(&DelToItem, FromPos);

		if(pToSlot->InsertItemAbs(pPlayer, ToPos, &DelFromItem) != EI_TRUE)
		{
			SWAPVALUE(DelFromItem.Position, DelToItem.Position);

			if(pFromSlot->InsertItemAbs(pPlayer, FromPos, &DelFromItem) != EI_TRUE)
				ASSERT(0);
			
			if(pToSlot->InsertItemAbs(pPlayer, ToPos, &DelToItem) != EI_TRUE)
				ASSERT(0);
			return 7;
		}

		if(pFromSlot->InsertItemAbs(pPlayer, FromPos, &DelToItem) != EI_TRUE)
		{
			SWAPVALUE(DelFromItem.Position, DelToItem.Position);

			if(pFromSlot->InsertItemAbs(pPlayer, FromPos, &DelFromItem) != EI_TRUE)
				ASSERT(0);
			ClearQuickPos(&DelToItem, ToPos);
			if(pToSlot->InsertItemAbs(pPlayer, ToPos, &DelToItem) != EI_TRUE)
				ASSERT(0);
			return 8;
		}
	}

	if( ItemMoveUpdateToDBbyTable(
			pPlayer,
			DelFromItem.dwDBIdx,
			DelFromItem.wIconIdx,
			DelFromItem.Position,
			DelToItem.dwDBIdx,
			DelToItem.Position ) )
	{
		// Log
		ItemMoveLog(FromPos, ToPos, pPlayer, &DelFromItem);
		if(DelToItem.dwDBIdx != 0)
			ItemMoveLog(ToPos, FromPos, pPlayer, &DelToItem);
	}

	if( FromPos == TP_WEAR_START + eWearedItem_Weapon ||
		ToPos == TP_WEAR_START + eWearedItem_Weapon )
	{
		const ITEMBASE* pWeaponItemBase = pPlayer->GetSlot( eItemTable_Weared )->GetItemInfoAbs( TP_WEAR_START + eWearedItem_Weapon );

		if( pWeaponItemBase && pWeaponItemBase->dwDBIdx )
		{
			const ITEM_OPTION& option = GetOption( *pWeaponItemBase );

			pPlayer->SetWeaponEnchant( option.mEnchant.mLevel );
            
			if( option.mItemDbIndex )
			{
				MSG_DWORDBYTE msg;

				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_ENCHANT_EFFECT_NOTIFY;
				msg.dwData  = pPlayer->GetID();
				msg.bData = option.mEnchant.mLevel;

				PACKEDDATA_OBJ->QuickSend( pPlayer, &msg, sizeof( MSG_DWORDBYTE ) );
			}
			// 080519 LYW --- ItemManager : ����Ʈ�� �ʿ� ���� �������̶�� �޽����� Ŭ���̾�Ʈ�� �����Ѵ�.
			else
			{
				MSG_DWORDBYTE msg;

				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_ENCHANT_EFFECT_NOTIFY;
				msg.dwData	 = pPlayer->GetID();
				msg.bData	 = 0;

				PACKEDDATA_OBJ->QuickSend( pPlayer, &msg, sizeof( MSG_DWORDBYTE ) );
			}
		}
		else
		{
			pPlayer->SetWeaponEnchant( 0 );
		}
	}
	
	return 0;
}
int CItemManager::MovePetItem( CPlayer * pPlayer, DWORD FromItemIdx, POSTYPE FromPos, DWORD ToItemIdx, POSTYPE& ToPos)
{
	CItemSlot * pFromSlot	= pPlayer->GetSlot(FromPos);
	CPet* pPet = pPlayer->GetCurPet();

	if( !pPet )
		return eItemUseErr_Err;

	const ITEMBASE * pFromItem = pFromSlot->GetItemInfoAbs(FromPos);
	const ITEMBASE * pToItem = pPet->GetWearedItem( ToPos );


	ITEMBASE FromItem;
	ITEMBASE ToItem;
	memset( &FromItem, 0, sizeof( ITEMBASE ) );
	memset( &ToItem, 0, sizeof( ITEMBASE ) );

	pFromSlot->DeleteItemAbs(pPlayer, FromPos, &FromItem);
	if( pToItem )
	{
		ToItem = *pToItem;
		pPet->SetWearedItem( ToPos, NULL );
	}

	ITEM_INFO* pFromInfo = GetItemInfo( FromItemIdx );
	ITEM_INFO* pToInfo = GetItemInfo( ToItemIdx );

	if( pFromSlot == pPlayer->GetSlot(eItemTable_Weared) )
	{
		return eItemUseErr_Move;
	}

	DWORD FromITemDBIdx = 0;
	DWORD ToItemDBIdx  = 0;

	if( FromItem.dwDBIdx )
	{
		FromITemDBIdx = FromItem.dwDBIdx;
		pPet->SetWearedItem( ToPos, &FromItem );

		// 080716 LUJ, �� ������ �̵� �α׸� �����Ѵ�
		LogItemMoney(
			pPlayer->GetID(),
			pPlayer->GetObjectName(),
			pPet->GetID(),
			pPet->GetObjectName(),
			eLog_ItemMoveFromPlayerToPet,
			pPlayer->GetMoney(),
			0,
			0,
			FromItemIdx,
			FromITemDBIdx,
			FromPos,
			ToPos,
			pFromInfo->Stack ? ( pFromItem ? pFromItem->Durability : 0 ) : 0,
			0 );
	}
	if( ToItem.dwDBIdx )
	{
		ToItemDBIdx = ToItem.dwDBIdx;
		pFromSlot->InsertItemAbs(pPlayer, FromPos, &ToItem);

		// 080716 LUJ, �� ������ �̵� �α׸� �����Ѵ�
		LogItemMoney(
			pPet->GetID(),
			pPet->GetObjectName(),
			pPlayer->GetID(),
			pPlayer->GetObjectName(),
			eLog_ItemMoveFromPetToPlayer,
			0,
			0,
			0,
			ToItemIdx,
			ToItemDBIdx,
			ToPos,
			FromPos,
			pToInfo->Stack ? ( pToItem ? pToItem->Durability : 0 ) : 0,
			0 );
	}

	ItemMoveUpdatePetInven( pPlayer->GetID(), pPlayer->GetUserID(), pPet->GetID(), FromITemDBIdx, FromPos, ToItemDBIdx, ToPos, 0 );

	return 0;
}

int CItemManager::DivideItem( CPlayer* pPlayer, DWORD ItemIdx,  POSTYPE FromPos, POSTYPE ToPos, DURTYPE FromDur, DURTYPE ToDur)
{
	if(FromPos == ToPos)
		return 1;

//---KES ItemDivide Fix 071020
//---������ ����� �κ��丮��(�������)
	//if(TP_INVENTORY_START <= FromPos && FromPos < TP_INVENTORY_END && TP_INVENTORY_START <= ToPos && ToPos < TP_INVENTORY_END)
	// 071210 KTH -- Ȯ���κ��丮���� �˻�.
	if(TP_INVENTORY_START <= FromPos && FromPos < (TP_INVENTORY_END + pPlayer->GetInventoryExpansionSize() ) && TP_INVENTORY_START <= ToPos && ToPos < (TP_INVENTORY_END + (pPlayer->GetInventoryExpansion() * TABCELL_INVENTORY_NUM ) ) )
	{
	}
	else if(TP_SHOPINVEN_START <= FromPos && FromPos < TP_SHOPINVEN_END && TP_SHOPINVEN_START <= ToPos && ToPos < TP_SHOPINVEN_END)
	{
	}
	else
	{
		return 1;
	}
//-----
	
	if(!CHKRT->ItemOf(pPlayer, FromPos, ItemIdx, FromDur+ToDur, 0, CB_EXIST|CB_ICONIDX|CB_DURA))
		return 2;

	if(CHKRT->ItemOf(pPlayer, ToPos,0,0,0,CB_EXIST))
		return 3;

	if( LOOTINGMGR->IsLootedPlayer( pPlayer->GetID() ) )
	{
		return 10;
	}

	CItemSlot * pFromSlot = pPlayer->GetSlot(FromPos);

	const ITEMBASE * FromItemBase = pFromSlot->GetItemInfoAbs( FromPos );

	//---KES ItemDivide Fix 071020
	//---���� ������ �������ε����� �˻�
	if(!IsDupItem(FromItemBase->wIconIdx))
	{
		return 4;
	}
	//----------------------------
	
	//////////////////////////////////////////////////////////////////////////
	//itemmaxcheck

	//---KES ItemDivide Fix 071020
	//---�ߺ��˻�� ������ ����. ��ģ������ 0�϶��� ���� �� ����.
	if( FromItemBase->Durability > MAX_YOUNGYAKITEM_DUPNUM )
		return 1;
	if( FromDur + ToDur > MAX_YOUNGYAKITEM_DUPNUM )
		return 1;
	if( FromDur > MAX_YOUNGYAKITEM_DUPNUM )
		return 1;
	if( ToDur > MAX_YOUNGYAKITEM_DUPNUM )
		return 1;
	if( FromDur == 0 ) return 1;
	if( ToDur == 0 ) return 1;
	//---------------------------
	//////////////////////////////////////////////////////////////////////////

	//---KES ItemDivide Fix 071020
	//---���� �ٿ��� ������Ʈ. ������ DB ���.
	if( pFromSlot->UpdateItemAbs(pPlayer, FromPos, FromItemBase->dwDBIdx, 0, 0, 0, FromDur, UB_DURA, SS_CHKDBIDX ) != EI_TRUE )
	{
		return 7;
	}

	ItemUpdatebyTable(pPlayer, FromItemBase->dwDBIdx, FromItemBase->wIconIdx, FromDur, FromPos, FromItemBase->QuickPosition );
	//----------------------------

	ITEMOBTAINARRAYINFO * pArrayInfo = Alloc(pPlayer, MP_ITEM, MP_ITEM_DIVIDE_ACK, pPlayer->GetID(), pPlayer->GetID(), eLog_ItemObtainDivide, 2, (DBResult)DivideItemDBResult);

	//---KES ItemDivide Fix 071020
	//---���� �������� �̹� �����Ѵ�. �ٽ� ���� �ʿ䰡 ����.
	/*
	pArrayInfo->ItemArray.AddItem( FromItemBase->dwDBIdx, 
									FromItemBase->wIconIdx, 
									FromDur, 
									FromItemBase->Position, 
									FromItemBase->QuickPosition,
									FromItemBase->ItemParam );
	*/

	//---�˻縦 �����ϱ� ���� ��ġ�� ���� �Ű��.
	/*
	if( pFromSlot->UpdateItemAbs(pPlayer, FromPos, 0, 0, 0, 0, FromDur, UB_DURA ) != EI_TRUE )
	{
		return 7;
	}
	//updateDB
	ItemUpdatebyTable(pPlayer, FromItemBase->dwDBIdx, FromItemBase->wIconIdx, FromDur, FromItemBase->Position, FromItemBase->QuickPosition);
	*/
	//----------------------------

	WORD EmptyCellPos[1];
	EmptyCellPos[0] = ToPos;
	WORD EmptyCellNum = 1;

	//---KES ItemDivide Fix 071020
	//---������ ������ �ϳ��� ����� ���� �ȴ�. +1����.
	//return ObtainItemEx(pPlayer, pArrayInfo, FromItemBase->wIconIdx, (WORD)ToDur, EmptyCellPos, EmptyCellNum, EmptyCellNum+1);
	return ObtainItemEx(pPlayer, pArrayInfo, FromItemBase->wIconIdx, (WORD)ToDur, EmptyCellPos, EmptyCellNum, EmptyCellNum);
	//------------------------------
}

void CItemManager::DivideItemDBResult(CPlayer* pPlayer, WORD ArrayID)
{
	ObtainItemDBResult(pPlayer, ArrayID);
}

int CItemManager::CombineItem( CPlayer* pPlayer, DWORD ItemIdx, POSTYPE FromPos, POSTYPE ToPos, DURTYPE & FromDur, DURTYPE & ToDur)
{
	if(FromPos == ToPos)
		return 1;

	if(!CHKRT->ItemOf(pPlayer, FromPos, ItemIdx,FromDur,0,CB_EXIST|CB_ICONIDX|CB_DURA))
		return 2;

	if(!CHKRT->ItemOf(pPlayer, ToPos, ItemIdx,ToDur,0,CB_EXIST|CB_ICONIDX|CB_DURA))
		return 3;

	if( LOOTINGMGR->IsLootedPlayer( pPlayer->GetID() ) )
	{
		return 10;
	}

	CItemSlot * pFromSlot = pPlayer->GetSlot(FromPos);
	CItemSlot * pToSlot = pPlayer->GetSlot(ToPos);

	const ITEMBASE * FromItemBase = pFromSlot->GetItemInfoAbs( FromPos );
	const ITEMBASE * ToItemBase = pToSlot->GetItemInfoAbs( ToPos );

	//---KES Item Combine Fix 071021
	//---���� ������ �����ͷ� �˻�
	if( !IsDupItem(FromItemBase->wIconIdx) || FromItemBase->wIconIdx != ToItemBase->wIconIdx )
	{
		return 4;
	}
	//-----------------------------

	// �����߰�
	if( (FromItemBase->ItemParam & ITEM_PARAM_SEAL) ||
		(ToItemBase->ItemParam & ITEM_PARAM_SEAL ) )
	{
		return eItemUseErr_Err;
	}
	
	//////////////////////////////////////////////////////////////////////////
	//---�߸��� ������ üũ
	if( FromItemBase->Durability > MAX_YOUNGYAKITEM_DUPNUM )
		return 1;
	if( ToItemBase->Durability > MAX_YOUNGYAKITEM_DUPNUM )
		return 1;
	//////////////////////////////////////////////////////////////////////////

	
	DURTYPE SumDur = FromItemBase->Durability + ToItemBase->Durability;
	if( SumDur > MAX_YOUNGYAKITEM_DUPNUM )
	{
		if(pFromSlot->UpdateItemAbs(pPlayer, FromPos, 0, 0, 0, 0, (SumDur - MAX_YOUNGYAKITEM_DUPNUM), UB_DURA ) != EI_TRUE )
		{
			return 5;
		}

		// 080605 LUJ, �α�
		LogItemMoney(
			pPlayer->GetID(),
			pPlayer->GetObjectName(),
			0,
			"*combine",
			eLog_ItemMoveInvenToInven,
			pPlayer->GetMoney(),
			0,
			0,
			FromItemBase->wIconIdx,
			FromItemBase->dwDBIdx,
			FromItemBase->Position,
			ToPos,
			FromItemBase->Durability,
			0 );

		if(pToSlot->UpdateItemAbs(pPlayer, ToPos, 0, 0, 0, 0, MAX_YOUNGYAKITEM_DUPNUM, UB_DURA ) != EI_TRUE)
		{
			return 6;
		}

		// 080605 LUJ, �α�
		LogItemMoney(
			pPlayer->GetID(),
			pPlayer->GetObjectName(),
			0,
			"*combine",
			eLog_ItemObtainMerge,
			pPlayer->GetMoney(),
			0,
			0,
			ToItemBase->wIconIdx,
			ToItemBase->dwDBIdx,
			ToItemBase->Position,
			FromPos,
			ToItemBase->Durability,
			0 );

		//DB
		// update2
		ItemCombineUpdateToDB(FromItemBase->dwDBIdx, FromItemBase->Durability, ToItemBase->dwDBIdx, ToItemBase->Durability, pPlayer->GetID());

		FromDur		= (SumDur - MAX_YOUNGYAKITEM_DUPNUM);
		ToDur		= MAX_YOUNGYAKITEM_DUPNUM;
	}
	else
	{
		ITEMBASE fromItemOut;
		if(pFromSlot->DeleteItemAbs(pPlayer,  FromPos, &fromItemOut ) != EI_TRUE)
		{
			return 7;
		}

		// 080605 LUJ, �α�
		LogItemMoney(
			pPlayer->GetID(),
			pPlayer->GetObjectName(),
			0,
			"*combine",
			eLog_ItemMoveInvenToInven,
			pPlayer->GetMoney(),
			0,
			0,
			fromItemOut.wIconIdx,
			fromItemOut.dwDBIdx,
			fromItemOut.Position,
			ToPos,
			fromItemOut.Durability,
			0 );
		
		if(pToSlot->UpdateItemAbs(pPlayer, ToPos, 0, 0, 0, 0, SumDur, UB_DURA ) != EI_TRUE)
		{
			return 8;
		}

		// 080605 LUJ, �α�
		LogItemMoney(
			pPlayer->GetID(),
			pPlayer->GetObjectName(),
			0,
			"*combine",
			eLog_ItemObtainMerge,
			pPlayer->GetMoney(),
			0,
			0,
			ToItemBase->wIconIdx,
			ToItemBase->dwDBIdx,
			ToItemBase->Position,
			FromPos,
			ToItemBase->Durability,
			0 );

		ItemCombineUpdateToDB(fromItemOut.dwDBIdx, 0, ToItemBase->dwDBIdx, ToItemBase->Durability, pPlayer->GetID());
		
		FromDur		= 0;
		ToDur		= SumDur;
	}
	return 0;
}

// description by hseos S
// ������ ������
// description by hseos E
int CItemManager::DiscardItem(CPlayer* pPlayer, POSTYPE whatPos, DWORD whatItemIdx, DURTYPE whatItemNum)
{
	CItemSlot * pSlot = pPlayer->GetSlot(whatPos);
	if( !pSlot )	return 1;

	const ITEMBASE * pItemBase = pSlot->GetItemInfoAbs(whatPos);
	
	if(pItemBase->wIconIdx != whatItemIdx) // discard just after move item... 
		return 4;
	
	// 081202 LUJ, ������ �������� ������ 0�� ��� ó���� �����ϵ��� �Ѵ�
	if(	IsDupItem(pItemBase->wIconIdx) )
	{
		const BOOL invalidQuantity = ( ( pItemBase->Durability > MAX_YOUNGYAKITEM_DUPNUM )  || ( ! pItemBase->Durability ) );

		if( invalidQuantity )
		{
			return 1;
		}
	}

	if(IsDupItem(pItemBase->wIconIdx) && (pItemBase->Durability > whatItemNum))
	{
		if( pSlot->UpdateItemAbs(pPlayer, whatPos, 0, 0, 0, 0, pItemBase->Durability - whatItemNum, UB_DURA ) != EI_TRUE )
		{
			return 2;
		}

		ItemUpdateToDB(pPlayer->GetID(), pItemBase->dwDBIdx, pItemBase->wIconIdx, 
				pItemBase->Durability, pItemBase->Position, pItemBase->QuickPosition );
	}
	else
	{
		// ItemShop
		ITEM_INFO* pItemInfo = GetItemInfo( pItemBase->wIconIdx );
		if( !pItemInfo )	return eItemUseErr_Discard;

		ITEMBASE DelItemOut;
//		SHOPITEMWITHTIME* pUsingItem = pPlayer->GetShopItemManager()->GetUsingItemInfo( whatItemIdx );
//		if( pUsingItem )
//		{
			//KES modify 060505
//			if( pUsingItem->ShopItem.ItemBase.dwDBIdx == pItemBase->dwDBIdx )
//			{
//				pPlayer->GetShopItemManager()->DeleteUsingShopItem( whatItemIdx );
//				if( pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION )
//					pPlayer->GetShopItemManager()->DiscardAvatarItem( whatItemIdx, whatPos );
//			}
//		}
		
		if( pSlot->DeleteItemAbs(pPlayer, whatPos, &DelItemOut, SS_LOCKOMIT ) != EI_TRUE )
			return 3;		

		//db
		// 071231 KTH -- GetID Add
		ItemDeleteToDB(DelItemOut.dwDBIdx);
		
		////SW050920 Rare
		//if(IsRareOptionItem(DelItemOut.wIconIdx, DelItemOut.RareIdx))
		//{
		//	if( FALSE == pPlayer->RemoveItemRareOption(DelItemOut.RareIdx) )
		//	{
		//		char szBuf[128];
		//		sprintf( szBuf, "DeleteRareItem_Error! PlayerID : %d\t ItemDBIdx : %d\n", pPlayer->GetID()
		//			,DelItemOut.wIconIdx );
		//		ASSERTMSG(0, szBuf);
		//	}
		//	CharacterItemRareOptionDelete(DelItemOut.RareIdx, DelItemOut.dwDBIdx);
		//}
		//pet

		/*
		if( IsPetSummonItem(DelItemOut.wIconIdx) )
		{
			pPlayer->GetPetManager()->DeletePet(DelItemOut.dwDBIdx);
		}
		*/
	}

	return 0;
}





// 071027 LYW --- ItemManager : Add function to discard item from quest group.
// ����Ʈ �׷쿡�� ���� ������ �ݳ� ó���� �ϱ� ���� �Լ�.
// ���� �ִ� ���� �� �ִ� ������ ���� 20�� �̱� ������, DURTYPE�� ������ int ������ ��ȯ�Ͽ� ����Ѵ�.
// DURTUPE���� ��� �� ��, ��� �Ŀ� ���� ������ ��û�� ���� �߸��� ��ġ�� �� �� �ֱ� ����.
// ���� ����Ʈ �׷쿡�� �ݳ� �� �� �ִ� �������� ������ int ���� ǥ�� ���� ������ �����Ѵ�.
// ���� �� ������ ������ �Ǿ� ���� �ʱ� ������, �� �����ۿ� ���� ó���� ó�� ���� �ʴ´�.
int CItemManager::QG_Discard_StackItem(CPlayer* pPlayer, POSTYPE whatPos, DWORD whatItemIdx, int* nGiveNum)
{
	// --------------------------------------------------------------------  ó�� �������� ���Ἲ ����.

	CItemSlot * pSlot = NULL ;												// ���� ������ ���� �����͸� �����Ѵ�.
	pSlot = pPlayer->GetSlot(whatPos) ;										// ��ġ�� �ش��ϴ� ���� ������ �޴´�.

	if( !pSlot ) return err_Invalid_CItemSlot ;								// ���� ������ ��ȿ���� ������, ���� �� return.

	const ITEMBASE * pItemBase = NULL ;										// ������ �⺻ ������ ���� �����͸� �����ϰ� null.
	pItemBase = pSlot->GetItemInfoAbs(whatPos) ;							// ��ġ�� �ش��ϴ� ������ �⺻������ �޴´�.

	if( !pItemBase ) return err_Invalid_ITEMBASE ;							// �⺻ ������ ������ ��ȿ���� ������, ���� �� return.
	
	if(pItemBase->wIconIdx != whatItemIdx)	return err_Not_Same_ItemIdx ;	// �ε����� ��ġ���� ������, ���� �� return.

	if( !IsDupItem(pItemBase->wIconIdx) ) return err_Not_StackItem ;		// ���� �������� �ƴϸ�, ���� �� return.

	int nCount = 0 ;														// ������ ���� ���� ������ �����ϰ� 0���� ����.
	nCount = (int)pItemBase->Durability ;									// ������ ������ ��´�.

	if(nCount > MAX_YOUNGYAKITEM_DUPNUM) return err_Over_ItemCount ;		// �׿� �ִ� ������ ���� �ִ���� ������, ���� �� return.

	// --------------------------------------------------------------------  �������� ����ϸ�,

	if(nCount > *nGiveNum)												
	{
		int nRemainCount = 0 ;												// ������ �ݳ� �� ���� ī��Ʈ ������ �����ϰ� 0���� �����Ѵ�.
		nRemainCount = nCount - *nGiveNum ;									// ���� ī��Ʈ�� �޴´�.

		if( nRemainCount <= 0 )												// ���� ���� 0���ϸ� �ȵȴ�!
		{
			return err_Invalid_RemainCount ;								// ���� �� return.
		}

		ERROR_ITEM eResult = EI_TRUE ;										// ���� ������Ʈ ����� ���� ������ �����ϰ� �ʱ�ȭ �Ѵ�.
		eResult = pSlot->UpdateItemAbs(pPlayer, whatPos, 0, 0, 0, 0, 
												nRemainCount, UB_DURA ) ;	// ��� ���� �޴´�.

		if( eResult != EI_TRUE )											// ������Ʈ�� ���� �ߴٸ�,
		{
			return eResult ;												// ���� �� return.
		}

		ItemUpdateToDB(pPlayer->GetID(), pItemBase->dwDBIdx, 
			pItemBase->wIconIdx, pItemBase->Durability, pItemBase->Position, 
			pItemBase->QuickPosition );									// DB�� ������ ������Ʈ ó���� �Ѵ�.

		*nGiveNum = 0 ;														// �ݳ� �� ������ ���� 0���� �����Ѵ�.

		MSG_EXCHANGE_DECREASEITEM Msg;										// ������ ���� ��ġ�� �����ϴ� �޽��� ����ü�� �����Ѵ�.
			
		Msg.Category = MP_QUEST;											// ī�װ��� ����Ʈ�� �����Ѵ�.
		Msg.Protocol = MP_QUEST_GIVEITEMS_ACK;								// ���������� ������ �ݳ� �������� �����Ѵ�.

		Msg.wAbsPosition = whatPos ;										// ������ �޽��� ����ü�� �������� �����Ѵ�.
		Msg.nItemCount = nRemainCount ;										// ���� ������ ���� �����Ѵ�.
		pPlayer->SendMsg( &Msg, sizeof(Msg) );								// �÷��̾�� �޽����� �����Ѵ�.

		return err_No_Error ;												// ���� ������ return.
	}

	// --------------------------------------------------------------------  ������ ���� �ݳ��� ���� ���ų� ������,

	else																	
	{
		ITEMBASE DelItemOut ;												// ���� ������ ����ü�� �����Ѵ�.

		ERROR_ITEM eResult = EI_TRUE ;										// ���� ������Ʈ ����� ���� ������ �����ϰ� �ʱ�ȭ �Ѵ�.
		eResult = pSlot->DeleteItemAbs(pPlayer, whatPos, 
										&DelItemOut, SS_LOCKOMIT ) ;		// �������� �����ϰ� ��� ���� �޴´�.

		if( eResult != EI_TRUE )											// ������ ���� �ߴٸ�,
		{
			return eResult ;												// ���� �� return.
		}	

		// 071231 KTH -- GetID add
		ItemDeleteToDB(DelItemOut.dwDBIdx) ;								// DB�� ������ ���� ó���� �Ѵ�.

		int nRemainCount = 0 ;												// ���� ������ ���� ������ �����ϰ� 0���� ����.
		nRemainCount = (*nGiveNum - (int)DelItemOut.Durability) ;			// ���� ������ �޴´�.

		if( nRemainCount < 0 )												// ���� ���� 0���ϸ� �ȵȴ�!
		{
			return err_Invalid_RemainCount ;								// ���� �� return.
		}

		*nGiveNum = nRemainCount ;											// ���� ������ �����Ѵ�.

		MSG_EXCHANGE_REMOVEITEM Msg;										// ������ ���� ��ġ�� �����ϴ� �޽��� ����ü�� �����Ѵ�.
			
		Msg.Category = MP_QUEST;											// ī�װ��� ����Ʈ�� �����Ѵ�.
		Msg.Protocol = MP_QUEST_GIVEITEM_ACK;								// ���������� ������ �ݳ� �������� �����Ѵ�.

		Msg.wAbsPosition = whatPos ;										// ������ �޽��� ����ü�� �������� �����Ѵ�.
		pPlayer->SendMsg( &Msg, sizeof(Msg) );								// �÷��̾�� �޽����� �����Ѵ�.

		return err_No_Error ;												// ���� ������ return.
	}
}





// 071027 LYW --- ItemManager : Add function to discard normal item from quest group.
// ����Ʈ �׷쿡�� �Ϲ�(������ �ʴ�) ������ �ݳ� ó���� �ϱ� ���� �Լ�.
// ���� �ִ� ���� �� �ִ� ������ ���� 20�� �̱� ������, DURTYPE�� ������ int ������ ��ȯ�Ͽ� ����Ѵ�.
// DURTUPE���� ��� �� ��, ��� �Ŀ� ���� ������ ��û�� ���� �߸��� ��ġ�� �� �� �ֱ� ����.
// ���� ����Ʈ �׷쿡�� �ݳ� �� �� �ִ� �������� ������ int ���� ǥ�� ���� ������ �����Ѵ�.
// ���� �� ������ ������ �Ǿ� ���� �ʱ� ������, �� �����ۿ� ���� ó���� ó�� ���� �ʴ´�.
int CItemManager::QG_Discard_NormalItem(CPlayer* pPlayer, POSTYPE whatPos, DWORD whatItemIdx, int* nGiveNum)
{
	// --------------------------------------------------------------------  ó�� �������� ���Ἲ ����.

	CItemSlot * pSlot = NULL ;												// ���� ������ ���� �����͸� �����Ѵ�.
	pSlot = pPlayer->GetSlot(whatPos) ;										// ��ġ�� �ش��ϴ� ���� ������ �޴´�.

	if( !pSlot ) return err_Invalid_CItemSlot ;								// ���� ������ ��ȿ���� ������, ���� �� return.

	const ITEMBASE * pItemBase = NULL ;										// ������ �⺻ ������ ���� �����͸� �����ϰ� null.
	pItemBase = pSlot->GetItemInfoAbs(whatPos) ;							// ��ġ�� �ش��ϴ� ������ �⺻������ �޴´�.

	if( !pItemBase ) return err_Invalid_ITEMBASE ;							// �⺻ ������ ������ ��ȿ���� ������, ���� �� return.
	
	if(pItemBase->wIconIdx != whatItemIdx)	return err_Not_Same_ItemIdx ;	// �ε����� ��ġ���� ������, ���� �� return.

	if( IsDupItem(pItemBase->wIconIdx) ) return err_Not_NormalItem ;		// �Ϲ� �������� �ƴϸ�, ���� �� return.

	// --------------------------------------------------------------------  �������� ����ϸ�,

	ITEMBASE DelItemOut ;													// ���� ������ ����ü�� �����Ѵ�.

	ERROR_ITEM eResult = EI_TRUE ;											// ���� ������Ʈ ����� ���� ������ �����ϰ� �ʱ�ȭ �Ѵ�.
	eResult = pSlot->DeleteItemAbs(pPlayer, whatPos, 
									&DelItemOut, SS_LOCKOMIT ) ;			// �������� �����ϰ� ��� ���� �޴´�.

	if( eResult != EI_TRUE )												// ������ ���� �ߴٸ�,
	{
		return eResult ;													// ���� �� return.
	}	

	// 071231 KTH -- GetID Add
	ItemDeleteToDB(DelItemOut.dwDBIdx) ;									// DB�� ������ ���� ó���� �Ѵ�.

	--*nGiveNum ;															// ������ ���� ���δ�.

	if( *nGiveNum < 0 )														// ���� ���� 0���ϸ� �ȵȴ�!
	{
		*nGiveNum = 0 ;														// ���� ī��Ʈ�� 0���� �����Ѵ�.

		return err_Invalid_RemainCount ;									// ���� �� return.
	}

	MSG_EXCHANGE_REMOVEITEM Msg;											// ������ ���� ��ġ�� �����ϴ� �޽��� ����ü�� �����Ѵ�.
		
	Msg.Category = MP_QUEST;												// ī�װ��� ����Ʈ�� �����Ѵ�.
	Msg.Protocol = MP_QUEST_GIVEITEM_ACK;									// ���������� ������ �ݳ� �������� �����Ѵ�.

	Msg.wAbsPosition = whatPos ;											// ������ �޽��� ����ü�� �������� �����Ѵ�.
	pPlayer->SendMsg( &Msg, sizeof(Msg) );									// �÷��̾�� �޽����� �����Ѵ�.

	return err_No_Error ;													// ���� ������ return.
}





int CItemManager::SellItem( CPlayer* pPlayer, POSTYPE whatPos, DWORD wSellItemIdx, DURTYPE sellItemDur, DWORD DealerIdx )
{
//	if(!CHKRT->StateOf(pPlayer, eObjectState_Deal))
//		return 1;

	if(!CHKRT->ItemOf(pPlayer, whatPos, wSellItemIdx,sellItemDur,0,CB_EXIST|CB_ICONIDX|CB_ENOUGHDURA))
		return 2;

	CItemSlot * pSlot = pPlayer->GetSlot(whatPos);
	ITEMBASE Item = *pSlot->GetItemInfoAbs(whatPos);

	//money check
	ITEM_INFO * pItemInfo = GetItemInfo( wSellItemIdx );
	if( !pItemInfo )	return 1;

	if( !pItemInfo->Sell )	return 1;
	MONEYTYPE money = 0;

	//////////////////////////////////////////////////////////////////////////
	//itemmaxcheck
	if( IsDupItem(wSellItemIdx) && sellItemDur > MAX_YOUNGYAKITEM_DUPNUM )
		return 1;
	//////////////////////////////////////////////////////////////////////////
	
	//SW050920 ����
	if( IsDupItem(wSellItemIdx) )
		money = pItemInfo->SellPrice * sellItemDur;
	else
		money = pItemInfo->SellPrice;

	money = SWPROFITMGR->AddProfitFromItemSell( money );

	if( !pPlayer->IsEnoughAdditionMoney(money) )
		return EI_MAXMONEY;

	if( LOOTINGMGR->IsLootedPlayer( pPlayer->GetID() ) )
	{
		return 10;
	}

	//process
	if(EI_TRUE != DiscardItem(pPlayer, whatPos, wSellItemIdx, sellItemDur))
	{
		//sendErroMsg
		return 3;
	}
	
	pPlayer->SetMoney( money, MONEY_ADDITION, 0, eItemTable_Inventory, eMoneyLog_GetStall, DealerIdx );
	
	// Log
	LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), DealerIdx, "",
		eLog_ItemSell, pPlayer->GetMoney(eItemTable_Inventory), 0, money,
		Item.wIconIdx , Item.dwDBIdx, Item.Position, 0, Item.Durability, pPlayer->GetPlayerExpPoint());	
	return 0;
}

int CItemManager::BuyItem( CPlayer* pPlayer, DWORD buyItemIdx, WORD buyItemNum, WORD DealerIdx )
{
//	if(!CHKRT->StateOf(pPlayer, eObjectState_Deal))
//		return 1;
	
	//DealerIdx��?�ˢ硧u���� ����A�ˡ�A ��u����AIAUAI�����ˢ�??
	if(!CHKRT->DearlerItemOf(DealerIdx, buyItemIdx))
		return 2;

	if( buyItemNum == 0 ) return EI_TRUE;	//0AI�ˡ�e���������ϩ�E~����a

	if( buyItemNum > MAX_ITEMBUY_NUM )
		buyItemNum = MAX_ITEMBUY_NUM;

	ITEM_INFO * pItemInfo	= GetItemInfo( buyItemIdx );
	CItemSlot * pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	WORD obtainItemNum = 0;
	MONEYTYPE ItemPrice = pItemInfo->BuyPrice;
//	if(g_pServerSystem->GetMap()->IsVillage() == FALSE)		// ������ �ƴϸ� 1.2��
//		ItemPrice = (MONEYTYPE)(ItemPrice*1.2);

	ItemPrice = SWPROFITMGR->AddProfitFromItemBuy( ItemPrice );

	obtainItemNum = GetCanBuyNumInMoney(pPlayer, buyItemNum, ItemPrice);
	if(obtainItemNum == 0)
		return NOT_MONEY; //��i����AI �ˡ��ϡ�AU�ˡ���I�ˡ�U.

	MONEYTYPE ItemFishPoint = pItemInfo->dwBuyFishPoint;
	if(0 < ItemFishPoint)
	{
		WORD wCanBuyNum = GetCanBuyNumInFishingPoint(pPlayer, buyItemNum, ItemFishPoint);
		if(wCanBuyNum < buyItemNum)
			return NOT_FISHINGPOINT;
	}

	WORD EmptyCellPos[255];
	WORD EmptyCellNum;
	obtainItemNum = GetCanBuyNumInSpace(pPlayer, pSlot, buyItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum);
	if(obtainItemNum == 0)
		return NOT_SPACE;	//AU�ˡ���c�����ˢ� ��u�Ϩ��ˡ�U.

	int rt;
	if( EI_TRUE == (rt = ObtainItemEx(pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_BUY_ACK, pPlayer->GetID(), DealerIdx, eLog_ItemObtainBuy, obtainItemNum, (DBResult)(BuyItemDBResult)), buyItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum)) )
	{
		// ��i��������e��ie
		ITEM_INFO * pTargetItem = GetItemInfo(buyItemIdx);
		pPlayer->SetMoney( ItemPrice*obtainItemNum, MONEY_SUBTRACTION, 0, eItemTable_Inventory, eMoneyLog_LoseStall, DealerIdx );

		if(0 < ItemFishPoint)
		{
			DWORD dwFishPoint = pPlayer->GetFishPoint() - ItemFishPoint;
			pPlayer->SetFishPoint(dwFishPoint);

			MSG_DWORD msgFishPoint;
			msgFishPoint.Category = MP_FISHING;
			msgFishPoint.Protocol = MP_FISHING_POINT_ACK;
			msgFishPoint.dwData   = dwFishPoint;
			pPlayer->SendMsg( &msgFishPoint, sizeof(msgFishPoint) );

			// 080808 LUJ, ���� ����ġ/���� �߰�
			Log_Fishing(
				pPlayer->GetID(),
				eFishingLog_UsePointForBuyItem,
				ItemFishPoint,
				pPlayer->GetFishPoint(),
				buyItemIdx,
				0,
				pPlayer->GetFishingExp(),
				pPlayer->GetFishingLevel() );
		}
	}

	return rt;
}

void CItemManager::BuyItemDBResult(CPlayer * pPlayer, WORD ArrayID )
{
	ObtainItemDBResult(pPlayer, ArrayID);
}

// �ʵ庸�� - 05.12 �̿���
// ���� ���ʹ� ������ ������ �ϳ����� ����ϰ� �Ǿ��ִ°���
// �ʵ庸���� �������� �������� ����ϹǷ� ������ ������ �ְ� ����
void CItemManager::MonsterObtainItem(CPlayer * pPlayer, DWORD obtainItemIdx, DWORD dwFurnisherIdx, WORD ItemNum)
{
	// �ʵ庸�� ������ ����� ���� ��������� �������� Ȯ�� 2 -> 128
	WORD EmptyCellPos[128];
	WORD EmptyCellNum;
	CItemSlot * pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	ITEM_INFO* pInfo = GetItemInfo( obtainItemIdx );
	if( !pInfo )			return;
//	if( pInfo->ItemKind & eSHOP_ITEM )
//		pSlot = pPlayer->GetSlot(eItemTable_ShopInven);
		
	BOOL bGetTwice = FALSE;
	BOOL bRare = FALSE;

	//static ITEM_RARE_OPTION_INFO RareOptionInfo;
	//memset(&RareOptionInfo, 0, sizeof(ITEM_RARE_OPTION_INFO));

	int rt = 0;

	// 070801 LYW --- ItemManager : ��ũ��Ʈ���� �Է��� ��� ī��Ʈ���� 1~n�� ������ ���� ������ �̾� ��� ������ �ٽ� �����Ѵ�.
	WORD wRandomItemCount = random(1, ItemNum) ;

	for( int i = 0; i < 1 + bGetTwice; ++i )
	{
		WORD obtainItemNum = GetCanBuyNumInSpace(pPlayer, pSlot, obtainItemIdx, wRandomItemCount, EmptyCellPos, EmptyCellNum);
		if(obtainItemNum == 0)
			return;

/*		if( pInfo->ItemKind & eSHOP_ITEM )
			rt = ObtainItemEx(pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_MONSTER_OBTAIN_NOTIFY, pPlayer->GetID(), dwFurnisherIdx, eLog_ItemObtainMonster, obtainItemNum, (DBResult)(ObtainItemDBResult), NULL), obtainItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum, 1);
		//SW050920 Rare
		else if( pInfo->ItemKind & eEQUIP_ITEM )
		{
			//ITEM_RARE_OPTION_INFO RareOptionInfo;
			//		memset(&RareOptionInfo, 0, sizeof(ITEM_RARE_OPTION_INFO));
			if( FALSE == bRare )
			{
				if( bRare = RAREGetRare( obtainItemIdx, &RareOptionInfo ) )
					rt = ObtainRareItem(pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_MONSTER_RAREOBTAIN_NOTIFY, pPlayer->GetID(), dwFurnisherIdx, eLog_ItemObtainMonster, obtainItemNum, NULL, (DBResultEx)(RareItemDBResult)), obtainItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, &RareOptionInfo );
				else
				{
					//��ȯ�Ǿ��� ���
					rt = ObtainItemEx(pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_MONSTER_OBTAIN_NOTIFY, pPlayer->GetID(), dwFurnisherIdx, eLog_ItemObtainMonster, obtainItemNum, (DBResult)(ObtainItemDBResult), NULL), obtainItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum, 0);
				}
			}
			else
			{
				rt = ObtainRareItem(pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_MONSTER_RAREOBTAIN_NOTIFY, pPlayer->GetID(), dwFurnisherIdx, eLog_ItemObtainMonster, obtainItemNum, NULL, (DBResultEx)(RareItemDBResult)), obtainItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, &RareOptionInfo );
			}
		}
		else
*/			rt = ObtainItemEx(pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_MONSTER_OBTAIN_NOTIFY, pPlayer->GetID(), dwFurnisherIdx, eLog_ItemObtainMonster, obtainItemNum, (DBResult)(ObtainItemDBResult), NULL), obtainItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum, 0);

		if(rt == 0 && pPlayer->GetPartyIdx())
		{
			PARTYMGR->SendObtainItemMsg(pPlayer, obtainItemIdx);
		}
		// 080114 KTH -- Ÿ�������� ��� ������ ȹ���� ���� ä�� �� �ִ� �����鿡�� �˸�.
		if( rt == 0 && dwFurnisherIdx == 261 )
		{
			MSG_USER_ADD_ITEM msg;
			msg.Category = MP_SIGNAL;
			msg.Protocol = MP_SIGNAL_USER_ADD_ITEM;
			

			sprintf(msg.CharacterName, pPlayer->GetObjectName());
			msg.dwIconIdx = obtainItemIdx;

			g_pUserTable->SetPositionUserHead();
			while( CObject* pObject = g_pUserTable->GetUserData() )
			{
				if( pObject->GetObjectKind() != eObjectKind_Player ) continue;
				
				CPlayer* pReceiver = (CPlayer*)pObject;
				if( pPlayer->GetChannelID() == pReceiver->GetChannelID() )
				{
					msg.dwObjectID = pReceiver->GetID();
 					pReceiver->SendMsg( &msg, sizeof(msg) );
				}
			}
		}

	}
	
	return;
	//return ObtainItem(pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_MONSTER_OBTAIN_NOTIFY, pPlayer->GetID(), 1, (DBResult)ObtainItemDBResult), obtainItemIdx, 1);
}

int CItemManager::CheatObtainItem( CPlayer* pPlayer, DWORD obtainItemIdx, WORD ItemNum )
{
	WORD EmptyCellPos[255];
	WORD EmptyCellNum;
	CItemSlot * pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	WORD obtainItemNum = GetCanBuyNumInSpace( pPlayer, pSlot, obtainItemIdx, ItemNum, EmptyCellPos, EmptyCellNum );
	if(obtainItemNum == 0)
		return 1;

	return ObtainItemEx( pPlayer, Alloc(pPlayer, MP_CHEAT, MP_CHEAT_ITEM_ACK, pPlayer->GetID(), 0, eLog_ItemObtainCheat, obtainItemNum, (DBResult)(ObtainItemDBResult)), obtainItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum );
}

// desc_hseos_���͹���01
// S ���͹��� �߰� added by hseos 2007.07.08
int CItemManager::ObtainMonstermeterItem( CPlayer* pPlayer, DWORD obtainItemIdx, WORD ItemNum )
{
	WORD EmptyCellPos[255];
	WORD EmptyCellNum;
	CItemSlot * pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	WORD obtainItemNum = GetCanBuyNumInSpace( pPlayer, pSlot, obtainItemIdx, ItemNum, EmptyCellPos, EmptyCellNum );
	if(obtainItemNum == 0)
		return 1;

	return ObtainItemEx( pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_MONSTER_OBTAIN_NOTIFY, pPlayer->GetID(), 0, eLog_ItemObtainMonstermeter, obtainItemNum, (DBResult)(ObtainItemDBResult)), obtainItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum );
}
// E ���͹��� �߰� added by hseos 2007.07.08

// desc_hseos_��ȥ_01
// S ��ȥ �߰� added by hseos 2007.12.13
int CItemManager::ObtainGeneralItem( CPlayer* pPlayer, DWORD obtainItemIdx, WORD ItemNum, WORD nObtainKind, BYTE nProtocal )
{
	WORD EmptyCellPos[255];
	WORD EmptyCellNum;
	CItemSlot * pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	WORD obtainItemNum = GetCanBuyNumInSpace( pPlayer, pSlot, obtainItemIdx, ItemNum, EmptyCellPos, EmptyCellNum );
	if(obtainItemNum == 0)
		return 1;

	return ObtainItemEx( pPlayer, Alloc(pPlayer, MP_ITEM, nProtocal, pPlayer->GetID(), 0, nObtainKind, obtainItemNum, (DBResult)(ObtainItemDBResult)), obtainItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum );
}
// E ��ȥ �߰� added by hseos 2007.12.13


int CItemManager::ObtainItemEx(CPlayer * pPlayer, ITEMOBTAINARRAYINFO * pArrayInfo, DWORD whatItemIdx, WORD whatItemNum, WORD * EmptyCellPos, WORD EmptyCellNum, WORD ArrayInfoUnitNum, WORD wSeal)
{
	DWORD obtainItemIdx = whatItemIdx;
	WORD obtainItemNum = whatItemNum;
	
//	CItemSlot * pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	//POSTYPE startPos = pSlot->GetStartPos();
	//POSTYPE EndPos = startPos +  pSlot->GetSlotNum();
	POSTYPE i = 0;
	
	//---KES ItemDivide Fix 071020
	ITEMBASE NewItemBase;
	memset( &NewItemBase, 0, sizeof(ITEMBASE) );

	// 071127 
	ITEM_INFO* pItemInfo = GetItemInfo(whatItemIdx);
/*
	NewItemBase.dwDBIdx			= 0;
	NewItemBase.wIconIdx		= 0;
	NewItemBase.Position		= 0;
	NewItemBase.QuickPosition	= 0;
	NewItemBase.Durability		= 0;
	NewItemBase.RareIdx			= 0;
*/
	//-----------------------------

	BOOL bDBReturn = FALSE;
	if(IsDupItem(whatItemIdx))
	{	
		for( i = 0 ; i < EmptyCellNum ; ++i )
		{
			CItemSlot * pSlot = pPlayer->GetSlot(EmptyCellPos[i]);
			const ITEMBASE * pItemBase = pSlot->GetItemInfoAbs(EmptyCellPos[i]);
			BOOL bIsEmpty = pSlot->IsEmpty(EmptyCellPos[i]);
			if( bIsEmpty )
			{
				bDBReturn = TRUE;
				if( obtainItemNum > MAX_YOUNGYAKITEM_DUPNUM )
				{
					NewItemBase.Position		= EmptyCellPos[i];
					if(pSlot->InsertItemAbs(pPlayer, EmptyCellPos[i], &NewItemBase, SS_PREINSERT) != EI_TRUE)
					{
						return 3;
						ASSERT(0);
					}
					ItemInsertToDB(pPlayer->GetID(), obtainItemIdx, MAX_YOUNGYAKITEM_DUPNUM, EmptyCellPos[i], MAKEDWORD(ArrayInfoUnitNum, pArrayInfo->wObtainArrayID), pItemInfo->wSeal);
				//	ItemInsertbyTable(pPlayer, obtainItemIdx, MAX_YOUNGYAKITEM_DUPNUM, EmptyCellPos[i], MAKEDWORD(ArrayInfoUnitNum, pArrayInfo->wObtainArrayID) );
					obtainItemNum -= MAX_YOUNGYAKITEM_DUPNUM;
				}
				else
				{
					NewItemBase.Position		= EmptyCellPos[i]; 
					if(pSlot->InsertItemAbs(pPlayer, EmptyCellPos[i], &NewItemBase, SS_PREINSERT) != EI_TRUE)
					{
						return 4;
						ASSERT(0);
					}
					
					//apply to db
					ItemInsertToDB(pPlayer->GetID(), obtainItemIdx, obtainItemNum, EmptyCellPos[i],  MAKEDWORD(ArrayInfoUnitNum, pArrayInfo->wObtainArrayID), pItemInfo->wSeal);
				//	ItemInsertbyTable(pPlayer, obtainItemIdx, obtainItemNum, EmptyCellPos[i], MAKEDWORD(ArrayInfoUnitNum, pArrayInfo->wObtainArrayID));
					obtainItemNum = 0;
				}
			}
			else if( !bIsEmpty &&
				pItemBase->wIconIdx == obtainItemIdx && 
				pItemBase->Durability < MAX_YOUNGYAKITEM_DUPNUM )	//$ �ߺ� ������ ä���
			{				
				if( pItemBase->Durability + obtainItemNum > MAX_YOUNGYAKITEM_DUPNUM )
				{
					obtainItemNum = obtainItemNum + (WORD)pItemBase->Durability - MAX_YOUNGYAKITEM_DUPNUM;
					if( pSlot->UpdateItemAbs(pPlayer, pItemBase->Position, 0, 0, 0, 0, MAX_YOUNGYAKITEM_DUPNUM, UB_DURA, SS_PREINSERT)  != EI_TRUE )
					{
						///SendErrorMsg();
						return 1;
					}
					
					pArrayInfo->ItemArray.AddItem( pItemBase->dwDBIdx, 
						pItemBase->wIconIdx, 
						pItemBase->Durability, 
						pItemBase->Position, 
						pItemBase->QuickPosition,
						pItemBase->ItemParam,
						0,
						pItemBase->nSealed);
					
					// to apply DB
					ItemUpdateToDB(pPlayer->GetID(), pItemBase->dwDBIdx, pItemBase->wIconIdx, 
								pItemBase->Durability, pItemBase->Position, pItemBase->QuickPosition );
					
					// '��?�ˢ硧u���� UpdateCI�ˡ�A ��u�Ϩ�������I��?�ˢ� ��������AI ��oU�ϡ�i����i 
					// ��oU�ϡ�i ��u����AIAUAC �ˡ������ˡ��ϡ̡ˡ���cAO��uO�ˡ�| ��uo��ui ��?I��u���� 
					// ��u���������ˢ�AC ��oo���������ˡ�| ��uc��ieC�ϡ�= 
				}
				else
				{
					if( pSlot->UpdateItemAbs(pPlayer, pItemBase->Position, 0, 0, 0, 0, pItemBase->Durability + obtainItemNum, UB_DURA, SS_PREINSERT )  != EI_TRUE )
					{
						//SendErrorMsg();
						return 2;
					}
					
					pArrayInfo->ItemArray.AddItem( pItemBase->dwDBIdx, 
						pItemBase->wIconIdx, 
						pItemBase->Durability, 
						pItemBase->Position, 
						pItemBase->QuickPosition,
						pItemBase->ItemParam,
						0,
						pItemBase->nSealed);
					
					// to apply DB
					ItemUpdateToDB(pPlayer->GetID(), pItemBase->dwDBIdx, pItemBase->wIconIdx, 
								pItemBase->Durability, pItemBase->Position, pItemBase->QuickPosition );
					
					obtainItemNum = 0;
				}
			}
			else
				ASSERT(0);
		}
		
		if( bDBReturn == FALSE )
		{
			pArrayInfo->CallBack( pPlayer, (WORD)pArrayInfo->wObtainArrayID );
			return 0;
		}
	}
	else	//$ �ߺ��Ұ� ������ ó��
	{
		for( i = 0 ; i < EmptyCellNum ; ++i )
		{
			CItemSlot * pSlot = pPlayer->GetSlot(EmptyCellPos[i]);
			if( pSlot->IsEmpty(EmptyCellPos[i]) )
			{
				NewItemBase.Position		= EmptyCellPos[i]; 
				if(pSlot->InsertItemAbs(pPlayer, EmptyCellPos[i], &NewItemBase, SS_PREINSERT) != EI_TRUE)
				{
					return 5;
					ASSERT(0);
				}
				--obtainItemNum;
				//CharacterItemInsert(1, pPlayer->GetID(), obtainItemIdx, MAX_YOUNGYAKITEM_DUPNUM, i, pArrayInfo->wInfoID);
//	bool GetRare(DWORD CharacterIdx, WORD ObtainItemIdx, POSTYPE Pos, ITEM_RARE_OPTION_INFO* pRareOptionInfo);

				//obtainItemIdx += 2;
				//DISTRIBUTERAND->ItemChangeAtLv(obtainItemIdx);


				ItemInsertToDB(pPlayer->GetID(), obtainItemIdx, 0, EmptyCellPos[i], MAKEDWORD(ArrayInfoUnitNum, pArrayInfo->wObtainArrayID), pItemInfo->wSeal);
			}
		}
	}

	if(obtainItemNum > 0)
	{
		ASSERT(0);
		return 6;
	}

	return 0;
}

// 080327 LUJ, ��� �ɼ� �α� �߰�
void CItemManager::ObtainItemDBResult(CPlayer * pPlayer, WORD ArrayID)
{
	ITEMOBTAINARRAYINFO * pArrayInfo = pPlayer->GetArray(ArrayID);

	for( int i = 0 ; i < pArrayInfo->ItemArray.ItemNum ; ++i )
	{
		ITEMBASE * item = pArrayInfo->ItemArray.GetItem( i );
		CItemSlot * pSlot = pPlayer->GetSlot(item->Position);

		//if(pSlot->InsertItemAbs(pPlayer, item->Position, item) != EI_TRUE)

		// 071218 LUJ, ���ԵǴ� �������� ����� Ǫ�� �޼ҵ带 ȣ���ؾ� �Ѵ�
		if( EI_TRUE != pSlot->InsertItemAbsFromDb( pPlayer, *item ) )
		{
			continue;
		}

		MONEYTYPE			ChangeMoney	= 0;
		const ITEM_INFO*	pInfo = ITEMMGR->GetItemInfo( item->wIconIdx );

		if(pArrayInfo->wType == eLog_ItemObtainBuy)
		{
			ChangeMoney = pInfo->BuyPrice;
		}

		switch( pArrayInfo->ItemArray.Protocol )
		{
		case MP_ITEM_MIX_GETITEM:
			{
				ITEMMGR->AddMixOption( *pPlayer, *item );
				break;
			}
		case MP_ITEM_MONSTER_OBTAIN_NOTIFY:
			{
				MSG_ITEM_OPTION optionMessage;
				{
					ZeroMemory( &optionMessage, sizeof( optionMessage ) );

					optionMessage.Category	= MP_ITEM;
					optionMessage.Protocol	= MP_ITEM_OPTION_ADD_ACK;
					optionMessage.mSize		= 1;
				}

				// 080526 LUJ, �������� �������� 0���� ȹ���� ��� �α׸� �����ϵ��� ��
				if(		pInfo->Stack &&
					!	item->Durability )
				{
					SYSTEMTIME time = { 0 };
					GetLocalTime( &time );

					char buffer[ MAX_PATH ] = { 0 };
					sprintf(
						buffer,
						"log/Zero.Qty.Item.%04d%02d%02d.log",
						time.wYear,
						time.wMonth,
						time.wDay );

					FILE* file = fopen( buffer, "r" );

					if( ! file )
					{
						file = fopen( buffer, "a" );

						if( file )
						{
							const char* title = "Time\tPlayer\tItem\tDBIdx\tMap\n";

							fwrite(
								title,
								sizeof( *title ),
								strlen( title ),
								file );
						}
					}

					fclose( file );
					file = fopen( buffer, "a" );

					if( file )
					{
						sprintf(
							buffer,
							"%02d:%02d:%02d\t%u\t%u\t%u\t%d\n",
							time.wHour,
							time.wMinute,
							time.wSecond,
							pPlayer->GetID(),
							item->wIconIdx,
							item->dwDBIdx,
							g_pServerSystem->GetMapNum() );

						fwrite(
							buffer,
							sizeof( *buffer ),
							strlen( buffer ),
							file );

						fclose( file );
					}
				}

				ITEM_OPTION& option = optionMessage.mOption[ 0 ];

				ITEMMGR->AddDropOption( *item, option );

				const ITEM_OPTION emptyOption = { 0 };

				if( ! memcmp( &emptyOption, &option, sizeof( emptyOption ) ) )
				{
					break;
				}

				option.mItemDbIndex	= item->dwDBIdx;

				if( memcmp( &emptyOption.mEnchant, &option.mEnchant, sizeof( emptyOption.mEnchant ) ) )
				{
					// 081118 LUJ, ĳ���� ��ȣ�� ������ ������ DB �����带 ������ Ű������ ���ǵ��� ��
					ItemOptionInsert( option, pPlayer->GetID() );
					LogItemOption(
						pPlayer->GetID(),
						item->dwDBIdx,
						option,
						eLog_ItemObtainMonster );
				}

				if( memcmp( &emptyOption.mDrop, &option.mDrop, sizeof( emptyOption.mDrop ) ) )
				{
					// 081118 LUJ, ĳ���� ��ȣ�� ������ ������ DB �����带 ������ Ű������ ���ǵ��� ��
					ItemDropOptionInsert( option, pPlayer->GetID() );
					// 080327 LUJ, �α�
					LogItemDropOption(
						option,
						eLog_ItemObtainMonster );
				}

				ITEMMGR->AddOption( option );
				
				pPlayer->SendMsg( &optionMessage, optionMessage.GetSize() );
				break;
			}
		}

		LogItemMoney(
			pArrayInfo->dwFurnisherIdx,
			"",
			pPlayer->GetID(),
			pPlayer->GetObjectName(),
			eLogitemmoney( pArrayInfo->wType ),
			0,
			pPlayer->GetMoney(eItemTable_Inventory),
			ChangeMoney,
			item->wIconIdx,
			item->dwDBIdx,
			0,
			item->Position,
			item->Durability,
			0 );

		/*
		if( pInfo->ItemKind == eQUEST_ITEM_PET )
		{
			g_pServerSystem->CreateNewPet(pPlayer, pInfo->ItemIdx, ItemInfo->dwDBIdx, pInfo->Plus_SkillIdx);
		}*/

/*		if( pInfo->ItemKind == eQUEST_ITEM_PET || pInfo->ItemKind == eSHOP_ITEM_PET )
		{
			PET_TOTALINFO* pPetInfo = pPlayer->GetPetManager()->GetPetTotalInfo(ItemInfo->dwDBIdx, eWithNULL );
			if( NULL == pPetInfo )
			{
				g_pServerSystem->CreateNewPet(pPlayer, pInfo->ItemIdx, ItemInfo->dwDBIdx, pInfo->Plus_SkillIdx);
			}
		}
*/
		// 06.09.12 RaMa - ġƮ�� ���� ������ �α� �߰�
		if( pArrayInfo->wType == eLog_ItemObtainCheat )
		{
			LogGMToolUse( pPlayer->GetID(), eGMLog_Item, eLog_ItemObtainCheat, 
				item->wIconIdx, item->dwDBIdx );
		}
	}

	pPlayer->SendMsg( &pArrayInfo->ItemArray, pArrayInfo->ItemArray.GetSize() );

	ITEMMGR->Free(pPlayer, pArrayInfo);
}


// 071211 KTH -- CPlayer �߰�
unsigned int CItemManager::GetTotalEmptySlotNum(CItemSlot* pSlot, CPlayer* pPlayer)
{
	//���� �ִ� ������ ����Ѵ�.

	// 071211 KTH -- �κ��丮�� Ȯ�� ������ ������ �˾� ���� ���� ����Ѵ�.
	//int	wInventoryExpansion = ( (SLOT_EXTENDED_INVENTORY1_NUM + SLOT_EXTENDED_INVENTORY2_NUM) - pPlayer->GetInventoryExpansionSize() );

	POSTYPE startPos = pSlot->GetStartPos();
	POSTYPE EndPos = startPos +  pSlot->GetSlotNum();//(pSlot->GetSlotNum() - wInventoryExpansion);
	POSTYPE i = 0;

	unsigned int totalemptyNum = 0;
	for( i = startPos ; i < EndPos ; ++i )
	{
		if(pSlot->IsEmpty(i)) totalemptyNum++;
	}

	return totalemptyNum;
}

//WORD CItemManager::CheckExtraSlot(CPlayer * pPlayer, CItemSlot * pSlot, DWORD whatItemIdx, DURTYPE whatItemNum, WORD * EmptyCellPos, WORD & EmptyCellNum)
//{
//	if( whatItemNum == 0 ) break;
//
//	DURTYPE EstItemNum = whatItemNum;
//	POSTYPE startPos = pSlot->GetStartPos();
//	POSTYPE EndPos = startPos +  pSlot->GetSlotNum();
//	POSTYPE i = 0;
//	EmptyCellNum = 0;
//
//	if(!IsDupItem(whatItemIdx))															// �������� ���� �������̸�,
//	{
//		for( i = startPos ; i < EndPos ; ++i )											// �κ��丮 ó�� ���� ������ ���� for���� ������.
//		{			
//			if(!pSlot->IsEmpty(i)) continue ;											// ��ġ�� �ش��ϴ� ������ �󽽷��� �ƴϸ�, continue ó���� �Ѵ�.
//
//			--EstItemNum;																// üũ �� ������ ���� ����.
//			EmptyCellPos[EmptyCellNum++] = i;											// ����ִ� ������ ���� ��ġ�� �����Ѵ�.
//		}
//	}
//	else
//	{
//		int nEnableCount = 0 ;
//		int nEmptySlotCount = 0 ;
//
//		for( i = startPos ; i < EndPos ; ++i )
//		{
//			if( pSlot->IsEmpty(i) ) ++nEmptySlotCount ;
//
//			const ITEMBASE * pItemBase = pSlot->GetItemInfoAbs(i);
//
//			if( pSlot->IsEmpty(i) || pSlot->IsLock(i) || 
//				pItemBase->wIconIdx != whatItemIdx && ||
//				pItemBase->Durability >= MAX_YOUNGYAKITEM_DUPNUM ) continue ;
//
//			if( pItemBase->Durability + EstItemNum > MAX_YOUNGYAKITEM_DUPNUM )
//			{
//				EstItemNum = EstItemNum + pItemBase->Durability - MAX_YOUNGYAKITEM_DUPNUM;
//
//				nEnableCount += EstItemNum ;
//			}
//		}
//
//		int nTotalCount = (nEmptySlotCount * MAX_YOUNGYAKITEM_DUPNUM) + nEnableCount ;
//
//		if( nTotalCount < whatItemNum ) return 0 ;
//	}
//
//	return 1 ;
//}


WORD CItemManager::CheckExtraSlot(CPlayer * pPlayer, CItemSlot * pSlot, DWORD whatItemIdx, DURTYPE whatItemNum, WORD * EmptyCellPos, WORD & EmptyCellNum)
{
	// 071212 KTH -- �κ��丮�� Ȯ�� ������ ������ �˾� ���� ���� ����Ѵ�.
	//int	wInventoryExpansion = ( (SLOT_EXTENDED_INVENTORY1_NUM + SLOT_EXTENDED_INVENTORY2_NUM) - pPlayer->GetInventoryExpansionSize() );

	DURTYPE EstItemNum = whatItemNum;
	POSTYPE startPos = pSlot->GetStartPos();
	//POSTYPE EndPos = startPos +  pSlot->GetSlotNum();
	// 071212 KTH -- 
	POSTYPE EndPos = startPos + pSlot->GetSlotNum();//(pSlot->GetSlotNum() - wInventoryExpansion);
	POSTYPE i = 0;
	EmptyCellNum = 0;

	if(!IsDupItem(whatItemIdx))
	{
		for( i = startPos ; i < EndPos ; ++i )
		{
			if( EstItemNum == 0 ) break;
			if(pSlot->IsEmpty(i))
			{
				--EstItemNum;
				EmptyCellPos[EmptyCellNum++] = i;
			}
		}
	}
	else
	{
		for( i = startPos ; i < EndPos ; ++i )
		{
			if( EstItemNum == 0 ) break;
			const ITEMBASE * pItemBase = pSlot->GetItemInfoAbs(i);
			if( !pSlot->IsEmpty(i) && !pSlot->IsLock(i) &&
				pItemBase->wIconIdx == whatItemIdx &&
				pItemBase->Durability < MAX_YOUNGYAKITEM_DUPNUM )
			{
				if( pItemBase->Durability + EstItemNum > MAX_YOUNGYAKITEM_DUPNUM )
					EstItemNum = EstItemNum + pItemBase->Durability - MAX_YOUNGYAKITEM_DUPNUM;
				else
					EstItemNum = 0;
				EmptyCellPos[EmptyCellNum++] = i;
			}
		}

		for( i = startPos ; i < EndPos ; ++i )
		{
			if( EstItemNum == 0 ) break;
			if(pSlot->IsEmpty(i))
			{
				if( EstItemNum > MAX_YOUNGYAKITEM_DUPNUM )
					EstItemNum -= MAX_YOUNGYAKITEM_DUPNUM;
				else
					EstItemNum = 0;
				EmptyCellPos[EmptyCellNum++] = i;
			}
		}
	}

	return (WORD)(whatItemNum - EstItemNum);
}

// rt : ����o�����Ϩ�������I8����I ��ii ��uo AO�ˡ�A �����ϩ���uo
WORD CItemManager::GetCanBuyNumInSpace(CPlayer * pPlayer, CItemSlot * pSlot, DWORD whatItemIdx, DURTYPE whatItemNum, WORD * EmptyCellPos, WORD & EmptyCellNum)
{
	// 071211 KTH -- �ٽ� ���� ������;
	//int	wInventoryExpansion = ( (SLOT_EXTENDED_INVENTORY1_NUM + SLOT_EXTENDED_INVENTORY2_NUM) - pPlayer->GetInventoryExpansionSize() );
/*	if( pPlayer->GetInventoryExpansion() == 0 )
		wInventoryExpansion = 40;
	else if( pPlayer->GetInventoryExpansion() == 1 )
		wInventoryExpansion = 20;*/

	DURTYPE EstItemNum = whatItemNum;
	POSTYPE startPos = pSlot->GetStartPos();
	POSTYPE EndPos = startPos + pSlot->GetSlotNum();//(pSlot->GetSlotNum() - wInventoryExpansion);
	POSTYPE i = 0;
	EmptyCellNum = 0;

	if(!IsDupItem(whatItemIdx))
	{
		for( i = startPos ; i < EndPos ; ++i )
		{
			if( EstItemNum == 0 ) break;
			if(pSlot->IsEmpty(i))
			{
				--EstItemNum;
				EmptyCellPos[EmptyCellNum++] = i;
			}
		}
	}
	else
	{
		for( i = startPos ; i < EndPos ; ++i )
		{
			if( EstItemNum == 0 ) break;
			const ITEMBASE * pItemBase = pSlot->GetItemInfoAbs(i);
			if( !pSlot->IsEmpty(i) && !pSlot->IsLock(i) &&
				pItemBase->wIconIdx == whatItemIdx &&
				pItemBase->Durability < MAX_YOUNGYAKITEM_DUPNUM )
			{
				if( pItemBase->Durability + EstItemNum > MAX_YOUNGYAKITEM_DUPNUM )
					EstItemNum = EstItemNum + pItemBase->Durability - MAX_YOUNGYAKITEM_DUPNUM;
				else
					EstItemNum = 0;
				EmptyCellPos[EmptyCellNum++] = i;
			}
		}

		for( i = startPos ; i < EndPos ; ++i )
		{
			if( EstItemNum == 0 ) break;
			if(pSlot->IsEmpty(i))
			{
				if( EstItemNum > MAX_YOUNGYAKITEM_DUPNUM )
					EstItemNum -= MAX_YOUNGYAKITEM_DUPNUM;
				else
					EstItemNum = 0;
				EmptyCellPos[EmptyCellNum++] = i;
			}
		}
	}

	return (WORD)(whatItemNum - EstItemNum);
}
WORD CItemManager::GetCanBuyNumInMoney(CPlayer * pPlayer, WORD buyNum, MONEYTYPE Price)
{
	MONEYTYPE money = pPlayer->GetMoney();

	if( money > (MONEYTYPE)(buyNum * Price) )
	{
		return buyNum;
	}
	else
	{
		// 080411 LUJ, 0 ������ ���ܸ� ���� ���� ������ ���� üũ
		return Price ? WORD( money / Price ) : buyNum;
	}	
}

WORD CItemManager::GetCanBuyNumInFishingPoint(CPlayer* pPlayer, WORD buyNum, MONEYTYPE Point)
{
	MONEYTYPE PlayerPoint = (MONEYTYPE)pPlayer->GetFishPoint();

	if(PlayerPoint > (MONEYTYPE)(buyNum * Point))
	{
		return buyNum;
	}
	else
	{
		return Point ? WORD(PlayerPoint / Point) : buyNum;
	}
}

// desc_hseos_����ý���_01
// S ����ý��� �߰� added by hseos 2007.09.11
ITEM_INFO* CItemManager::GetUseItemInfo(CPlayer* pPlayer, WORD TargetPos, DWORD wItemIdx)
{
	if(pPlayer->GetState() == eObjectState_Die)
		return NULL;
	if(!CHKRT->ItemOf(pPlayer, TargetPos, wItemIdx,0,0,CB_EXIST|CB_ICONIDX))
		return NULL;

	if(pPlayer->GetState() == eObjectState_Immortal )
		OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer, eObjectState_Immortal );

	CItemSlot * pSlot = pPlayer->GetSlot(TargetPos);
	if(pSlot == NULL)
	{
		return NULL;
	}

	const ITEMBASE * pItemBase = pSlot->GetItemInfoAbs(TargetPos);
	if (pItemBase == NULL)
	{
		return NULL;
	}

	ITEM_INFO * pItemInfo = GetItemInfo(wItemIdx);
	if (pItemInfo == NULL)
	{
		return NULL;
	}

	if (pItemBase->wIconIdx != pItemInfo->ItemIdx)
	{
		return NULL;
	}

	return pItemInfo;
}
// E ����ý��� �߰� added by hseos 2007.09.11

int CItemManager::UseItem( CPlayer* pPlayer, WORD TargetPos, DWORD wItemIdx )
{
	if(pPlayer->GetState() == eObjectState_Die)
		return eItemUseErr_Err;
	if(!CHKRT->ItemOf(pPlayer, TargetPos, wItemIdx,0,0,CB_EXIST|CB_ICONIDX))
		return eItemUseErr_Err;

	AbnormalStatus* pAbnormalStatus = pPlayer->GetAbnormalStatus();

	if( pAbnormalStatus->IsSlip ||
		pAbnormalStatus->IsStone ||
		pAbnormalStatus->IsStun ||
		pAbnormalStatus->IsParalysis || 
		pAbnormalStatus->IsFreezing )
		return eItemUseErr_Err;

	if(pPlayer->GetState() == eObjectState_Immortal )
		OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer, eObjectState_Immortal );

	CItemSlot * pSlot = pPlayer->GetSlot(TargetPos);

	if( ! pSlot )
	{
		return eItemUseErr_Err;
	}

	const ITEMBASE * pItemBase = pSlot->GetItemInfoAbs(TargetPos);
	const ITEM_INFO * pItemInfo = GetItemInfo(wItemIdx);

	if(	!	pItemBase	||
		!	pItemInfo )
	{
		return eItemUseErr_Err;
	}
	// 080411 LUJ, ������ ��� ������ üũ�ϵ��� �Ѵ�. ����: ��� ������ ���� �������� 0���� �����Ǿ� �ִ�
	else if(	pItemInfo->LimitLevel &&
				pItemInfo->LimitLevel > pPlayer->GetLevel() )
	{
		return eItemUseErr_Err;
	}

	const ITEMBASE previousItem = *pItemBase;

	// 071128 KTH --- ���� �ִ� �������� �ٷ� ��� �Ұ���
	if(pSlot == pPlayer->GetSlot(eItemTable_Shop) )
	{
		return eItemUseErr_Err;
	}

	// 071206 KTH --- â�� Ȯ������ ������ üũ
	if( pItemInfo->SupplyType == ITEM_KIND_EXTEND_STORAGE )
	{
		WORD storagenum = pPlayer->GetStorageNum();

		if( TAB_BASE_STORAGE_NUM > storagenum || storagenum >= TAB_STORAGE_NUM )
			return eItemUseErr_Err;
	}

	// 071122 LYW --- ItemManager : ���� ���� ó�� �߰�.
	if( pItemBase->nSealed == eITEM_TYPE_SEAL )
	{
		if( pItemInfo->SupplyType == ITEM_KIND_PET )
		{
			pSlot->ForcedUnseal(TargetPos);
			PETMGR->HatchPet( pPlayer, pItemBase );
			return eItemUseUnsealed;
		}
		else if(UseSealingItem(pPlayer, (ITEMBASE*)pItemBase/*&Item*/, *pItemInfo))
		{
			SEND_ITEM_BASEINFO msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_SHOPITEM_USE_ACK;
			msg.ItemBase = *pItemBase;
			msg.ItemPos = pItemBase->Position ;
			msg.ItemIdx = pItemBase->wIconIdx ;
			SendAckMsg(pPlayer, &msg, sizeof(msg));

			LogItemMoney(
				pPlayer->GetID(),
				pPlayer->GetObjectName(),
				0,
				"",
				eLog_ShopItemUnseal,
				pPlayer->GetMoney(),
				0,
				0,
				pItemBase->wIconIdx,
				pItemBase->dwDBIdx,
				pItemBase->Position,
				0,
				pItemBase->Durability,
				0 );

			//return eItemUseSuccess; // ���������ÿ��� ��������(eItemUseUnsealed)�� ������.
			return eItemUseUnsealed;
		}
		else
		{
			MSG_ITEM_ERROR msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_SHOPITEM_USE_NACK;
			msg.ECode = eItemUseErr_Err ;
			SendErrorMsg(pPlayer, &msg, sizeof(msg), eItemUseErr_Err);

			LogItemMoney(
				pPlayer->GetID(),
				pPlayer->GetObjectName(),
				0,
				"",
				eLog_ShopItemUnseal_Failed,
				pPlayer->GetMoney(),
				0,
				0,
				pItemBase->wIconIdx,
				pItemBase->dwDBIdx,
				pItemBase->Position,
				0,
				pItemBase->Durability,
				0 );

			return eItemUseErr_Err;
		}
	}


	// ��Ÿ�� ������ �ִ� �������̸� ���� �������� üũ�Ѵ�.
	{
		CoolTimeScript::ItemData::const_iterator it = mCoolTimeScript.mItemData.find( wItemIdx );

		if( mCoolTimeScript.mItemData.end() != it )
		{
			const CoolTimeScript::Unit& unit = it->second;

			if( ! pPlayer->AddCoolTime( unit.mIndex, unit.mTime ) )
			{
				return eItemUseErr_Err;
			}
		}
	}

	switch( pItemInfo->SupplyType )
	{
	case ITEM_KIND_PET_SKILL:					
	case ITEM_KIND_PET_COSTUME:				
	case ITEM_KIND_PET_EQUIP:					
	case ITEM_KIND_PET_EGG:					
	case ITEM_KIND_PET_FOOD:				
	case ITEM_KIND_PET_HP_RECOVER:			
	case ITEM_KIND_PET_MP_RECOVER:			
		{
			if( !pPlayer->GetCurPet() )
				return eItemUseErr_Err;
		}
		break;
	case ITEM_KIND_PET_EVOLUTION_PYHSIC_TYPE:	
	case ITEM_KIND_PET_EVOLUTION_MAGIC_TYPE:	
	case ITEM_KIND_PET_EVOLUTION_SUPPORT_TYPE:
		{
			if( !pPlayer->GetCurPet() )
				return eItemUseErr_Err;

			if( pPlayer->GetCurPet()->GetObjectInfo()->Level != PETMGR->GetGradeUpLevel( pPlayer->GetCurPet()->GetObjectInfo()->Grade ) )
				return eItemUseErr_Err;

			if( pPlayer->GetCurPet()->GetObjectInfo()->Type != ePetType_Basic &&
				pPlayer->GetCurPet()->GetObjectInfo()->Type != pItemInfo->SupplyType - ITEM_KIND_PET_EVOLUTION_PYHSIC_TYPE + 1 )
				return eItemUseErr_Err;
		}
		break;
	}

	if ( ITEM_KIND_PET == pItemInfo->SupplyType )
	{
		PETMGR->UsePetItem( pPlayer, pItemBase->dwDBIdx );
		return eItemUseSuccess;
	}
	else if( eItemCategory_Expend != pItemInfo->Category )
	{
		return eItemUseErr_Err;
	}
	else if( eItemCategory_Expend != pItemInfo->Category )
	{
		return eItemUseSuccess;
	}
	else if(pItemBase->Durability==0 && pItemInfo->dwUseTime==0 )
	{
		return eItemUseErr_Err;
	}

	// desc_hseos_ü����������01
	// S ü���������� �߰� added by hseos 2007.08.03
	// ..ü���� �������� ��쿡 �� ������ üũ�ؼ� �� ������ �ϳ��� ������
	// ..����� ���� �ʰ� �Ѵ�. �ϳ��� ������ �����ϰ� ������ ü����. ���� �������� ������ �����ϴ�
	// ..���������� ��ȯ�Ǵ� Ȯ���� �߻��ϴ���, �� ������ �ϳ��ۿ�(Ȥ�� ��ȯ ������ ���� �̸�) ���� ������ 
	// ..�ϳ��� ���޵ȴ�.
	BOOL bChangeItem = FALSE;
	if (IsChangeItem(pItemInfo->ItemIdx))
	{
		CItemSlot* pSlot = pPlayer->GetSlot(eItemTable_Inventory);
		if(NULL == pSlot) ASSERTMSG(0,"�ùٸ� ������ ������ ���� �� �����ϴ�.");

		// 071211 KTH -- pPlayer �߰�
		DWORD nNum = GetTotalEmptySlotNum(pSlot, pPlayer);
		// 071129 KTH -- ChangeItem ��Ͽ��� ��� �����Ұ��� ������ ������ ���� ���� �󽽷��� ���ٸ� return eItemUseErr_Err
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		DWORD ResultItemNum = 0;

		for(int i=0; i<m_nChangeItemInfoNum; i++)
		{
			if (pItemInfo->ItemIdx == m_stChangeItemInfo[i].nChangeItemID)
			{
				// ��ȿ �������ΰ�?
				// ..��ũ��Ʈ ������ �����̳� ��Ÿ����� ������ ��ȿ���� ���� �������̶�� ó������ �ʴ´�.
				ITEM_INFO* pstItem = GetItemInfo(pItemBase->wIconIdx);
				if (pstItem == NULL)
				{
					continue;
				}
				ResultItemNum = m_stChangeItemInfo[i].nResultNum;
			}
		}

		if(nNum == 0 || ResultItemNum > nNum )
			/////////////////////////////////////////////////////////////////////////////////////////////////////////
		{
			MSG_DWORD m;
			m.Category	= MP_ITEM;
			m.Protocol	= MP_ITEM_WORKING_FAIL;
			m.dwData	= pPlayer->GetID();

			PACKEDDATA_OBJ->QuickSend( pPlayer, &m, sizeof( m ) );

			return eItemUseErr_Err;
		}

		bChangeItem = TRUE;
	}
	// E ü���������� �߰� added by hseos 2007.08.03

	// 080220 KTH -- �ֹε�� �ʱ�ȭ DiscardItem ���� �̵�
	if (pItemInfo->SupplyType == ITEM_KIND_RESET_REGIDENT_REGIST)
	{
		if( !g_csResidentRegistManager.SRV_ResetRegistResident(pPlayer) )
			return eItemUseErr_Err;
	}

	if(pItemInfo->SupplyType == ITEM_KIND_SUMMON_MONSTER)
	{
		if(0 == m_vecMonSummonGroupInfo.size())
			return eItemUseErr_ForbiddenSummon;	// ��ũ��Ʈ�� �ε��Ǿ� ���� ������ ������.

		stMonSummonGroupInfo* pSummonGroup = m_vecMonSummonGroupInfo.at(pItemInfo->SupplyValue);
		if(pSummonGroup && pSummonGroup->bNotForbidden==false)
		{
			std::vector<int>::iterator iter;
			for(iter=m_vecMonSummonForbiddenMap.begin(); iter!=m_vecMonSummonForbiddenMap.end(); iter++)
			{
				if((*iter) == g_pServerSystem->GetMapNum())
					return eItemUseErr_ForbiddenSummon;
			}
		}
	}


	// 080522 NYJ --- ������ DiscardItem() ȣ������ �����Ѵ�.
	if(pItemInfo->SupplyType == ITEM_KIND_FISHING_FISH)
	{
		return eItemUseErr_Err;
	}
	// 080703 LUJ, �̵��� ��� ���¿��� ��ų �ʱ�ȭ�� �� �� ������ �Ѵ�.
	//				��ų �ʱ�ȭ�� �̵��� �����͸��� ������ ��쿡�� ���� �� ���� �����Ǽ��� �ȵǱ� ����
	else if( pItemInfo->SupplyType == ITEM_KIND_RESET_SKILL )
	{
		const ITEMBASE* weaponItem = GetItemInfoAbsIn( pPlayer, TP_WEAR_START + eWearedItem_Weapon );
		const ITEMBASE* shieldItem = GetItemInfoAbsIn( pPlayer, TP_WEAR_START + eWearedItem_Shield );

		const ITEM_INFO* weaponItemInfo	= ( weaponItem ? ITEMMGR->GetItemInfo( weaponItem->wIconIdx ) : 0 );
		const ITEM_INFO* shieldItemInfo	= ( shieldItem ? ITEMMGR->GetItemInfo( shieldItem->wIconIdx ) : 0 );

		if( weaponItemInfo &&
			shieldItemInfo &&
			weaponItemInfo->WeaponType == shieldItemInfo->WeaponType )
		{
			return eItemUseErr_Err;
		}
	}

	// 080326 NYJ --- �Ⱓ�� �������� �������� �ȵǹǷ� DiscardItem()�� �н��Ѵ�.
	if(!pItemInfo->nTimeKind && !pItemInfo->dwUseTime && pItemInfo->SupplyType != ITEM_KIND_PET )
	{
		if(EI_TRUE != DiscardItem(pPlayer, TargetPos, wItemIdx, 1))
		{
			return eItemUseErr_Err;
		}
	}

			// desc_hseos_����Ʈ ��_01
			// S ����Ʈ �� �߰� added by hseos 2007.12.01
			// ..Ư�� �������� Ư�� ��Ȳ���� ���Ұ�
			if (pItemInfo->SupplyType == ITEM_KIND_RETURN_SCROLL)
			{
				if (g_csDateManager.IsChallengeZoneHere())
				{
					return eItemUseErr_Err;
				}
			}
			// E ����Ʈ �� �߰� added by hseos 2007.12.01

	BYTE bEffectKind = 0;	//�񹫽� �ٸ�ĳ���Ϳ��Ե� ����Ʈ �����ֱ� ����

	switch( pItemInfo->SupplyType )
	{
		// 071204 LYW --- ItemManager : ���� �ʱ�ȭ ó�� �߰�.
	case ITEM_KIND_RESET_STATS :
		{
			UseItem_ResetStats(pPlayer);
		}
		break ;

		// 071204 LYW --- ItemManager : ��ų �ʱ�ȭ ó�� �߰�.
	case ITEM_KIND_RESET_SKILL :
		{
			UseItem_ResetSkill(pPlayer);
		}
		break ;

	case ITEM_KIND_HP_RECOVER:
		{
			CCharacterCalcManager::StartUpdateLife( pPlayer, pItemInfo->SupplyValue );

			bEffectKind = 1;//����� ȸ�� ����Ʈ
			break;
		}
	case ITEM_KIND_MP_RECOVER:
		{
			CCharacterCalcManager::StartUpdateMana( pPlayer, pItemInfo->SupplyValue );

			bEffectKind = 2;//���� ȸ�� ����Ʈ
			break;
		}
	case ITEM_KIND_HP_RECOVER_INSTANT:
		{
			CCharacterCalcManager::StartUpdateLife( pPlayer, pItemInfo->SupplyValue, 1, 0 );

			bEffectKind = 1;//����� ȸ�� ����Ʈ
			break;
		}
	case ITEM_KIND_MP_RECOVER_INSTANT:
		{
			CCharacterCalcManager::StartUpdateMana( pPlayer, pItemInfo->SupplyValue, 1, 0 );

			bEffectKind = 2;//���� ȸ�� ����Ʈ
			break;
		}
	case ITEM_KIND_HPMP_RECOVER:		//����ȸ����
		{
			CCharacterCalcManager::StartUpdateLife( pPlayer, pItemInfo->SupplyValue );
			CCharacterCalcManager::StartUpdateMana( pPlayer, pItemInfo->SupplyValue );
				bEffectKind = 3;
			break;
		}
	case ITEM_KIND_HPMP_RECOVER_INSTANT: //�������ȸ����
		{
			CCharacterCalcManager::StartUpdateLife( pPlayer, pItemInfo->SupplyValue, 1, 0 );
			CCharacterCalcManager::StartUpdateMana( pPlayer, pItemInfo->SupplyValue, 1, 0 );
				bEffectKind = 3;
			break;
		}
	case ITEM_KIND_RETURN_SCROLL:
		{
			//---KES PK Mode 080104
			//---KES AUTONOTE
			if( pPlayer->IsPKMode() || pPlayer->GetAutoNoteIdx() )
			{
				return eItemUseErr_Err;
			}
			//---------------------

			MSG_WORD message;
			message.dwObjectID	= pPlayer->GetID();
			message.Category	= MP_USERCONN;
			message.Protocol	= MP_USERCONN_RETURN_SYN;
			message.wData		= ( pItemInfo->SupplyValue ? WORD( pItemInfo->SupplyValue ) : pPlayer->GetPlayerLoginPoint() );

			UserConn_Return_Syn( pPlayer->GetAgentNum(), ( char* )&message );

			break;
		}
	// desc_hseos_����Ʈ ��_01
	// S ����Ʈ �� �߰� added by hseos 2007.12.01
	case ITEM_KIND_CHALLENGEZONE_ENTER_FREQ:
		{
			g_csDateManager.SRV_SetChallengeZoneEnterBonusFreq(pPlayer, pItemInfo->SupplyValue);
			break;
		}
	// E ����Ʈ �� �߰� added by hseos 2007.12.01

		// 071206 KTH -- â�� Ȯ�� ������ ���
	case ITEM_KIND_EXTEND_STORAGE:
		{
			const WORD storagenum = pPlayer->GetStorageNum();

			//TAB_STORAGE_NUM
			pPlayer->SetStorageNum( storagenum + 1 );
			StorageBuyStorage(pPlayer->GetID());

			MSG_BYTE msg;
			msg.Category = MP_STORAGE;
			msg.Protocol = MP_STORAGE_BUY_ACK;
			msg.bData	= storagenum + 1;
			pPlayer->SendMsg(&msg, sizeof(msg));
			break;
		}

	case ITEM_KIND_JOIN_EVENT:
		{
			UpdateEventFromGame( pPlayer->GetUserID(), pPlayer->GetObjectName() );
		}
		break;

	// 080215 KTH -- �ֹε�� �ʱ�ȭ // ���� �̵�
/*	case ITEM_KIND_RESET_REGIDENT_REGIST:
		{
			if( !g_csResidentRegistManager.SRV_ResetRegistResident(pPlayer) )
				return eItemUseErr_Err;
		}
		break;*/

	// 080607 LYW, â�� ��ȯ ������ ó�� �߰�.
	// 080923 LUJ, ������ â�� ���� �ڵ带 ����ϵ��� ��
	case ITEM_KIND_OPENSTORAGE_BYITEM :
		{
			if( ! pPlayer->GetStorageNum() )
			{	
				// 080923 LUJ, Ŭ���̾�Ʈ���� üũ�ϹǷ� ���� ó���� �ʿ����
				break;
			}	
			else if( ! pPlayer->IsGotWarehouseItems() )
			{	
				CharacterStorageItemInfo( pPlayer->GetID(), pPlayer->GetUserID(), 0 ) ;
				pPlayer->SetGotWarehouseItems( TRUE );
			}	
			else
			{	
				// 080923 LUJ, �ܿ� �ð��� �����ؾ� �Ѵ�. �׷��� ������ Ŭ���̾�Ʈ ������ ǥ�� ������ �߻��Ѵ�
				pPlayer->ProcessTimeCheckItem( TRUE );
				STORAGEMGR->SendStorageItemInfo( pPlayer );
			}	
		}
		break ;

	// 080611 LYW --- ItemManager : ��ų ����Ʈ ȹ�� �ֹ��� ó�� �߰�.
	case ITEM_KIND_GET_SKILLPOINT :
		{
			// 080612 LYW --- ItemManager : ������ ���α� ����� �߰�.
			//return UseItem_GetSkillPointByItem(pPlayer, (WORD)pItemInfo->SupplyValue) ;

			BYTE byResult = 0 ;
			byResult = UseItem_GetSkillPointByItem(pPlayer, (WORD)pItemInfo->SupplyValue) ;

			if(byResult == eItemUseSuccess)
			{
				char text[ MAX_NAME_LENGTH + 1 ] = { 0 };

				sprintf( text, "%dPoint", (WORD)pItemInfo->SupplyValue);

				LogItemMoney(
					pPlayer->GetID(),
					pPlayer->GetObjectName(),
					0,
					text,
					eLog_ItemUse,
					pPlayer->GetMoney(),
					0,
					0,
					previousItem.wIconIdx,
					previousItem.dwDBIdx,
					previousItem.Position,
					0,
					previousItem.Durability - 1,
					0 );
			}

			return byResult ;
		}
		break ;
	case ITEM_KIND_PET_FOOD:	
		{
			CPet* pPet = pPlayer->GetCurPet();
			pPet->FriendlyUp( pItemInfo->SupplyValue );
		}
		break;
	case ITEM_KIND_PET_HP_RECOVER:			
		{
			CPet* pPet = pPlayer->GetCurPet();
			pPet->SetLife( pPet->GetLife() + pItemInfo->SupplyValue );
		}
		break;
	case ITEM_KIND_PET_MP_RECOVER:
		{
			CPet* pPet = pPlayer->GetCurPet();
			pPet->SetMana( pPet->GetMana() + pItemInfo->SupplyValue );
		}
		break;
	case ITEM_KIND_PET_EVOLUTION_PYHSIC_TYPE:
		{
			CPet* pPet = pPlayer->GetCurPet();

			if( pPet->GetObjectInfo()->Type == ePetType_Basic )
			{
				pPet->SetType( ePetType_Physic );
				pPet->GradeUp();
			}
			else if( pPet->GetObjectInfo()->Type == ePetType_Physic )
			{
				pPet->GradeUp();
			}
			else
				return eItemUseErr_Err;
		}
		break;
	case ITEM_KIND_PET_EVOLUTION_MAGIC_TYPE:
		{
			CPet* pPet = pPlayer->GetCurPet();

			if( pPet->GetObjectInfo()->Type == ePetType_Basic )
			{
				pPet->SetType( ePetType_Magic );
				pPet->GradeUp();
			}
			else if( pPet->GetObjectInfo()->Type == ePetType_Magic )
			{
				pPet->GradeUp();
			}
			else
				return eItemUseErr_Err;
		}
		break;
	case ITEM_KIND_PET_EVOLUTION_SUPPORT_TYPE:
		{
			CPet* pPet = pPlayer->GetCurPet();

			if( pPet->GetObjectInfo()->Type == ePetType_Basic )
			{
				pPet->SetType( ePetType_Support );
				pPet->GradeUp();
			}
			else if( pPet->GetObjectInfo()->Type == ePetType_Support )
			{
				pPet->GradeUp();
			}
			else
				return eItemUseErr_Err;
		}
		break;

	/*
	case ITEM_KIND_SUMMON_NPC:
		{
			stSummonItemInfo* pSummonInfo = m_NpcSummonItemInfo.GetData(pItemInfo->SupplyValue);
			if(pSummonInfo)
			{
				VECTOR3 vOutPos;
				pPlayer->GetPosition(&vOutPos);

				DWORD dwGridID = pPlayer->GetChannelID();
				MAPTYPE MapNum = g_pServerSystem->GetMapNum();
				vOutPos.x = ((DWORD)(vOutPos.x/50.0f)*50.0f) + 100.0f;
				vOutPos.z = ((DWORD)(vOutPos.z/50.0f)*50.0f) + 100.0f;

				BASEOBJECT_INFO Baseinfo;
				NPC_TOTALINFO NpcTotalInfo;
				NPC_LIST* pNpcList = GAMERESRCMNGR->GetNpcInfo(pSummonInfo->summonindex);
				if(pNpcList == 0)
					return eItemUseErr_NoSummonInfo;

				Baseinfo.dwObjectID =  g_pAISystem->GeneraterMonsterID();
				Baseinfo.BattleID = dwGridID;
				
				SafeStrCpy(Baseinfo.ObjectName, pNpcList->Name, MAX_NAME_LENGTH+1);
				NpcTotalInfo.Group = 0;
				NpcTotalInfo.MapNum = MapNum;
				NpcTotalInfo.NpcJob = pNpcList->JobKind;
				NpcTotalInfo.NpcKind = pNpcList->NpcKind;
				NpcTotalInfo.NpcUniqueIdx = 0;

				CNpc* pNpc = g_pServerSystem->AddNpc(&Baseinfo, &NpcTotalInfo, &vOutPos);
				if(pNpc)
				{
					pNpc->SetDieTime(pSummonInfo->lifetime);	// �ڵ��Ҹ���� �ʵ���.
				}

				// �α� ���ܾ� �ǳ�?
			}
		}
		break;

	*/
	
	//081027 NYJ - ���ͼ�ȯ������
	case ITEM_KIND_SUMMON_MONSTER:
		{
			stMonSummonGroupInfo* pInfo = m_vecMonSummonGroupInfo.at(pItemInfo->SupplyValue);
			if(pInfo)
			{
				DWORD dwRand1 = rand() % 100;
				DWORD dwRand2 = rand() % 10000;
				DWORD dwResRand = dwRand1*10000 + dwRand2;
				DWORD dwDieTime = pInfo->dwDieTime;

				DWORD dwMonsterIdx = pInfo->MonSummon(dwResRand);

				if(dwMonsterIdx)
				{
					VECTOR3 vOutPos;
					pPlayer->GetPosition(&vOutPos);

					DWORD dwGridID = pPlayer->GetChannelID();
					MAPTYPE MapNum = g_pServerSystem->GetMapNum();
					vOutPos.x = ((DWORD)(vOutPos.x/50.0f)*50.0f) + 200.0f;
					vOutPos.z = ((DWORD)(vOutPos.z/50.0f)*50.0f) + 200.0f;

					int nRad = 10;
					RECALLMGR->EventRecall( pPlayer, dwMonsterIdx, 1, dwGridID, &vOutPos, nRad, 0,0, ITEMRECALL_GROUP_ID, dwDieTime);

					// �α� ���ܾ� �ǳ�?
				}
			}			
		}
		break;

	case ITEM_KIND_SUMMON_EFFECT:
		{
			MSG_DWORD2 msgEff;
			msgEff.Category		= MP_ITEM;
			msgEff.Protocol		= MP_ITEM_USEEFFECT_ACK;
			msgEff.dwData1		= pPlayer->GetID();
			msgEff.dwData2		= pItemInfo->SupplyValue;

			PACKEDDATA_OBJ->QuickSend( pPlayer, &msgEff, sizeof(msgEff) );
		}
		break;

	default : break ;
	}

	// desc_hseos_ü����������01
	// S ü���������� �߰� added by hseos 2007.08.03
	if (bChangeItem)
	{
		BOOL bResult = ProcChangeItem(pPlayer, pItemInfo->ItemIdx);
		if (bResult == FALSE)
		{
			return eItemUseErr_Err;
		}
		// ȿ���� ǥ���� �� �ֵ��� ����
		else
		{
			MSG_DWORD m;
			m.Category	= MP_ITEM;
			m.Protocol	= MP_ITEM_WORKING_SUCCESS;
			m.dwData	= pPlayer->GetID();

			PACKEDDATA_OBJ->QuickSend( pPlayer, &m, sizeof( m ) );
		}
	}
	// E ü���������� �߰� added by hseos 2007.08.03

	if( bEffectKind != 0 )
	{
		if( pPlayer->GetBattle()->GetBattleKind() == eBATTLE_KIND_SHOWDOWN )	//����
		{
			MSG_DWORDBYTE msg;
			msg.Category	= MP_ITEM;
			msg.Protocol	= MP_ITEM_USE_NOTIFY;
			msg.dwData		= pPlayer->GetID();
			msg.bData		= bEffectKind;

			PACKEDDATA_OBJ->QuickSend( pPlayer, &msg, sizeof(msg) );
		}
	}

	// 080627 LUJ, ���� �������� ��� ��� �α׸� ���⵵�� �Ѵ�
	if( pItemInfo->Shop )
	{
		LogItemMoney(
			pPlayer->GetID(),
			pPlayer->GetObjectName(),
			0,
			"",
			IsChangeItem( pItemInfo->ItemIdx ) ? eLog_ItemDestroybyChangeItem : eLog_ItemUse,
			pPlayer->GetMoney(),
			0,
			0,
			previousItem.wIconIdx,
			previousItem.dwDBIdx,
			previousItem.Position,
			0,
			pItemInfo->Stack ? previousItem.Durability - 1 : 0,
			0 );
	}

	return eItemUseSuccess;
}


// 071122 LYW --- ItemManager : ���� ������ ��� �Լ�.
BOOL CItemManager::UseSealingItem( CPlayer* pPlayer, ITEMBASE* pItemBase, const ITEM_INFO& ItemInfo )
{
	if( !pPlayer ) return FALSE ;
	if( !pItemBase ) return FALSE ;
	
	if( pItemBase->nSealed == eITEM_TYPE_SEAL_NORMAL ) return FALSE;

	gCurTime;
	// 071123 KTH --- ItemManager : <ItemBase : RemainSecond Setting> "ItemBase�� ItemInfo���� ���ð��� ������ �����Ͽ��ش�."
	pItemBase->nRemainSecond = ItemInfo.dwUseTime;

	// 071123 KTH --- ItemManager : <ItemBase : nSealed Fix> "ItemBase�� ���� ������ �����Ͽ� �ش�."
	pItemBase->nSealed = eITEM_TYPE_UNSEAL;

	// 071123 KTH --- ItemManager : <Database Fix : RemainSecond & Sealed> "���� �ð��� ���� ���¸� ������ �� ���� ����."
	if( ItemInfo.nTimeKind == eKIND_REALTIME )
	{
		UnSealItemByRealTime(pPlayer->GetID(), pItemBase->dwDBIdx, ItemInfo.dwUseTime);
		return TRUE;
	}
	else
	{
		UnSealItemByGameTime(pPlayer->GetID(), pItemBase->dwDBIdx, ItemInfo.dwUseTime );
		return TRUE;
	}
	// 071125 KTH --- delete : <Player : AddTimeCheckItem> "������ �˻� ����� ����"
/*	if( ItemInfo.nTimeKind == eKIND_REALTIME )		// ���� �������� ��� �ð��� ó��.
	{
		UnSealItemByRealTime(pPlayer->GetID(), pItemBase->dwDBIdx, ItemInfo.dwUseTime) ;

		pPlayer->AddTimeCheckItem( (ITEMBASE*)pItemBase );

		return TRUE;
	}
	else
	{
		UnSealItemByGameTime(pPlayer->GetID(), pItemBase->dwDBIdx, ItemInfo.dwUseTime) ;

		pPlayer->AddTimeCheckItem( (ITEMBASE*)pItemBase );

		return TRUE;
	}*/
	return FALSE;
}

BOOL CItemManager::isSealItem(const ITEMBASE* pItemBase)
{
	if( pItemBase->nSealed == eITEM_TYPE_SEAL)
		return TRUE;

	return FALSE;
}


// 080228 LUJ, ���� ���� ��ȣ ��� �߰�
eLogitemmoney CItemManager::Mix( CPlayer* player, const MSG_ITEM_MIX_SYN2& input, const CBackupSlot* backupSlot )
{
	// �߰��� ������:	�� ���Կ� ���� DB �׼����� ����ȭ���� ���� ���, DBó���� ���������� �����Ƿ�
	//					�ſ� �����ϴ�.
	//
	// �ذ�å:			���� ������ �����صΰ� �޸𸮿� ���� ������Ʈ�Ѵ�. ������ ������ ��� ������
	//					������ �������� �ٷ� �޸𸮸� �����Ѵ�. ���������� ������ ���� ������ �����
	//					������ ���Ͽ� �� ���Կ� ���� �ϳ��� ���۸� ��û�Ѵ�
	//					���� DB �ε����� ��Ȱ���� �� �־� �ӵ��� ���� ���ڵ� ������ �ּ�ȭ�ȴ�.

	const DWORD		resultIndex				= input.mResultIndex;
	DWORD			mixSize					= input.mMixSize;
	const POSTYPE	inventoryStartPosition	= TP_INVENTORY_START;
	const POSTYPE	inventoryEndPosition	= TP_INVENTORY_END + player->GetInventoryExpansionSize();

	CItemSlot* playerSlot = player->GetSlot( eItemTable_Inventory );

	if( ! backupSlot	||
		! playerSlot	||
		! mixSize )
	{
		return eLog_itemMixBadRequest;
	}

	ITEMBASE sourceItem;
	{
		const ITEMBASE* item = playerSlot->GetItemInfoAbs( input.mSourceItemPosition );

		if( !	item	||
			item->dwDBIdx != input.mSourceItemDbIndex )
		{
			return eLog_itemMixBadRequest;
		}

		sourceItem = *item;
	}

	const MixScript* script	= GetMixScript( sourceItem.wIconIdx );

	if( ! script )
	{
		return eLog_itemMixBadRequest;
	}

	const ItemMixResult* mixResult = 0;
	{
		MixScript::const_iterator it	= script->begin();

		if( script->size() <= resultIndex )
		{
			return eLog_ItemMixBadCondition;
		}

		std::advance( it, resultIndex );
		mixResult = &( *it );
	}

	if(	player->GetLevel() < mixResult->mRequiredLevel	||
		player->GetMoney() < mixResult->mMoney * mixSize )
	{
		return eLog_ItemMixBadCondition;
	}

	// 080228 LUJ, ��ȣ ��� �÷���
	BOOL isProtection = FALSE;

	// 080228 LUJ, ��ȣ �������� ���� ��� �Ҹ��Ѵ�
	if( input.mProtectionItem.wIconIdx )
	{
		const ITEMBASE* item = playerSlot->GetItemInfoAbs( input.mProtectionItem.Position );

		if( !	item											||
				item->dwDBIdx != input.mProtectionItem.dwDBIdx	||
			!	IsMixProtectionItem( *item ) )
		{
			return eLog_ItemMixBadCondition;
		}

		const ITEM_INFO* info = GetItemInfo( item->wIconIdx );

		if( ! info )
		{
			return eLog_ItemMixBadCondition;
		}

		if( info->Stack &&
			1 < item->Durability )
		{
			if( EI_TRUE != playerSlot->UpdateItemAbs(
				player,
				item->Position,
				item->dwDBIdx,
				item->wIconIdx,
				item->Position,
				item->QuickPosition,
				item->Durability - 1 ) )
			{
				return eLog_ItemMixBadCondition;
			}
		}
		else
		{
			if( EI_TRUE != playerSlot->DeleteItemAbs(
				player,
				item->Position,
				0 ) )
			{
				return eLog_ItemMixBadCondition;
			}
		}

		isProtection = TRUE;
	}
	
	// 080227 LUJ, ����и� ��� �Ӽ����� �߰�
	enum
	{
		TypeSuccess,
		TypeFailure,
		TypeBigFailure,		
	}
	resultType = ( mixResult->mSuccessRate < mRandom.GetFloat() ? TypeFailure : TypeSuccess );
	
	// 080228 LUJ, ��ȣ ����� ��� �����ϴ��� ��Ḧ ���� �ʵ��� �Ѵ�
	if(	TypeSuccess == resultType ||
		TypeFailure == resultType && ! isProtection )
	{
		// ��� ����� ��Ȯ�� ��ġ�ϴ��� Ȯ���Ѵ�. ������ �������� ������ �ε����� 0���� �����Ͽ� �޽����� ���� ���̴�
		for(
			ItemMixResult::Material::const_iterator material_it = mixResult->mMaterial.begin();
			mixResult->mMaterial.end() != material_it;
			++material_it )
		{
			const DWORD itemIndex		= material_it->first;
			DWORD		needQuantity	= material_it->second * mixSize;

			for(
				POSTYPE position = inventoryStartPosition;
				inventoryEndPosition > position;
			++position )
			{
				if( ! needQuantity )
				{
					break;
				}

				const ITEMBASE* item = playerSlot->GetItemInfoAbs( position );

				if( !	item			||
					!	item->dwDBIdx	||
					item->wIconIdx	!= itemIndex )
				{
					continue;
				}

				if( IsDupItem( item->wIconIdx ) )
				{
					const DWORD quantity = item->Durability;

					if( item->Durability > needQuantity )
					{
						if( EI_TRUE != playerSlot->UpdateItemAbs(
							player,
							item->Position,
							item->dwDBIdx,
							item->wIconIdx,
							item->Position,
							item->QuickPosition,
							item->Durability - needQuantity ) )
						{
							//return eLog_ItemMixSystemFault;
							continue;
						}
					}
					else
					{
						if( EI_TRUE != playerSlot->DeleteItemAbs(
							player,
							item->Position,
							0 ) )
						{
							//return eLog_ItemMixSystemFault;
							continue;
						}
					}

					needQuantity -= min( quantity, needQuantity );
				}
				else
				{
					if( EI_TRUE != playerSlot->DeleteItemAbs( player, item->Position, 0 ) )
					{
						//return eLog_ItemMixSystemFault;
						continue;
					}

					--needQuantity;
				}
			}

			if( needQuantity )
			{
				return eLog_ItemMixBadCondition;
			}
		}
	}

	if( TypeFailure == resultType )
	{
		// ������ Ȯ���� �� ��Ḧ ���� �� �ִ�
		// 080314 LUJ, ����� �� 0�� ��ġ�� �������� �սǵǴ� �������� �־�, ����и� �߻����� �ʵ��� �Ѵ�
		const float bigFailedRate	= 0.0f;
		//const float bigFailedRate	= 2.0f;

		// 080228 LUJ, ���� ��ȣ�� ��� Ű �������� ���� �ʵ��� ��
		if(	!	isProtection &&
				mRandom.GetFloat() < bigFailedRate )
		{
			resultType = TypeBigFailure;
		}
		 
		// 080131 LUJ
		// 080314 LUJ, ���� ���� �αװ� �� ���� ǥ�õǵ��� ��
		LogItemMoney(
			player->GetID(),
			player->GetObjectName(),
			0,
			isProtection ? "*protect" : "",
			TypeBigFailure == resultType ? eLog_ItemMixBigFail : eLog_ItemMixFail,
			player->GetMoney(),
			0,
			0,
			sourceItem.wIconIdx,
			sourceItem.dwDBIdx,
			sourceItem.Position,
			0,
			0,
			0 );

		if(	TypeBigFailure == resultType )
		{			
			const ITEMBASE* item = playerSlot->GetItemInfoAbs( input.mSourceItemPosition );

			if( ! item )
			{
				return eLog_ItemMixSystemFault;
			}

			if( IsDupItem( item->wIconIdx ) )
			{
				if( 1 == item->Durability )
				{
					if( EI_TRUE != playerSlot->DeleteItemAbs( player, item->Position, 0 ) )
					{
						return eLog_ItemMixSystemFault;
					}
				}
				else if( EI_TRUE != playerSlot->UpdateItemAbs(
					player,
					item->Position,
					item->dwDBIdx,
					item->wIconIdx,
					item->Position,
					item->QuickPosition,
					item->Durability - 1 ) )
				{
					return eLog_ItemMixSystemFault;
				}
			}
			else 
			{
				if( EI_TRUE != playerSlot->DeleteItemAbs( player, item->Position, 0 ) )
				{
					return eLog_ItemMixSystemFault;
				}
			}
		}
	}
	else
	{
		// �ɼ��� ���� �Ϲ� ������ ����
		// �ϴ� �ִ��� ��ġ��
		if( IsDupItem( mixResult->mItemIndex ) )
		{
			for(
				POSTYPE position = inventoryStartPosition;
				inventoryEndPosition > position;
				++position )
			{
				if( ! mixSize )
				{
					break;
				}

				const ITEMBASE* item = playerSlot->GetItemInfoAbs( position );

				if( !	item									||
					!	item->dwDBIdx							||
						item->wIconIdx != mixResult->mItemIndex	||
					!	IsDupItem( item->wIconIdx )				||
						MAX_YOUNGYAKITEM_DUPNUM == item->Durability )
				{
					continue;
				}
				else if( item->Durability > MAX_YOUNGYAKITEM_DUPNUM )
				{
					return eLog_ItemMixBadQuantity;
				}

				const DURTYPE enableSize = MAX_YOUNGYAKITEM_DUPNUM - item->Durability;

				if( EI_TRUE != playerSlot->UpdateItemAbs(
					player,
					item->Position,
					item->dwDBIdx,
					item->wIconIdx,
					item->Position,
					item->QuickPosition,
					item->Durability + min( enableSize, mixSize ),
					UB_DURA,
					SS_CHKDBIDX ) )
				{
					//return eLog_ItemMixSystemFault;
					continue;
				}

				mixSize -= min( enableSize, mixSize );
			}
		}

		// ���� ������ �� ������ ����
		if( mixSize )
		{
			WORD	emptyPosition[ SLOT_MAX_INVENTORY_NUM ] = { 0 };
			WORD	emptyCount;
			{
				if( IsDupItem( mixResult->mItemIndex ) )
				{
					emptyCount = max( 1, WORD( mixSize ) / MAX_YOUNGYAKITEM_DUPNUM );
				}
				else
				{
					emptyCount = WORD( mixSize );
				}
			}

			playerSlot->GetEmptyCell( emptyPosition, emptyCount );

			while( emptyCount-- )
			{
				const POSTYPE position = emptyPosition[ emptyCount ];

				ITEMBASE item;
				ZeroMemory( &item, sizeof( item ) );

				item.wIconIdx	= mixResult->mItemIndex;
				item.Position	= position;

				if( IsDupItem( mixResult->mItemIndex ) )
				{
					const DURTYPE enableSize = min( mixSize, MAX_YOUNGYAKITEM_DUPNUM );

					mixSize			-=	enableSize;
					item.Durability	=	enableSize;
				}
				else
				{
					--mixSize;
					item.Durability	= 0;
				}

				const ITEMBASE* backupItem = backupSlot->GetItem( position );
				
				// 071218 LUJ,	�� �����̶� ������ �̷� ������ �ѹ� �� ��ȸ����. DB �ε����� ��Ȱ�������ν�
				//				���� insert�� ����� ���ϰ�, update�� ���� �� �ִ�
				if( backupItem )
				{
					item.dwDBIdx = backupItem->dwDBIdx;

					// 071218 LUJ,	UpdateItemAbs�� DB �ε����� ������Ʈ���� �ʴ´�.
					//				���� InsertItemAbs�� ���� ����� �ɸ��� �ʵ��� �����Ѵ�
					if( EI_TRUE != playerSlot->InsertItemAbs(
						player,
						position,
						&item ) )
					{
						//return eLog_ItemMixInsufficentSpace;

						continue;
					}

				}
				else
				{
					// 071218 LUJ, DB �ε����� 0�� ��� ���� ���������� �ν��ϹǷ�, �ϴ� �������� �־�����
					item.dwDBIdx = UINT_MAX;

					if( EI_TRUE != playerSlot->InsertItemAbs(
						player,
						position,
						&item,
						SS_PREINSERT ) )
					{
						//return eLog_ItemMixInsufficentSpace;

						continue;
					}
				}
			}
		}

		if( mixSize )
		{
			return eLog_ItemMixInsufficentSpace;
		}
	}

	player->SetMoney( mixResult->mMoney * input.mMixSize, MONEY_SUBTRACTION );

	MSG_ITEM_MIX_ACK message;
	{
		ZeroMemory( &message, sizeof( message ) );

		switch( resultType )
		{
		case TypeSuccess:
			{
				message.Protocol = MP_ITEM_MIX_SUCCEED_ACK;
				break;
			}
		case TypeFailure:
			{
				message.Protocol = MP_ITEM_MIX_FAILED_ACK;
				break;
			}
		case TypeBigFailure:
			{
				message.Protocol = MP_ITEM_MIX_BIGFAILED_ACK;
				break;
			}
		// 080227 LUJ, ����� ���� ��� ó���� �� ���� ������
		default:
			{
				ASSERT( 0 );
				break;
			}
		}
		
		message.Category			= MP_ITEM;
		message.mSourceItemDbIndex	= input.mSourceItemDbIndex;
		message.mSourceItemPosition	= input.mSourceItemPosition;
		message.mMixSize			= input.mMixSize;
		message.mResultIndex		= input.mResultIndex;
	}

	// ���Ե� �޽����� �����Ѵ�. �ٸ� DB �ε����� ���������� ��� ���̴�
	// ���� ������ �������� ������ �ε����� ����. ������Ʈ�� �������� �����۸� �ش�ȴ�
	for( POSTYPE position = inventoryStartPosition; inventoryEndPosition > position; ++position )
	{
		const ITEMBASE*	item;
		{
			item = playerSlot->GetItemInfoAbs( position );

			// DB �ε����� ������ �� ����
			if( ! item ||
				! item->dwDBIdx )
			{
				item = 0;
			}
		}

		const ITEMBASE*	backupItem = backupSlot->GetItem( position );

		if( ! item )
		{
			// �������� ���µ�, �̷� ���Կ��� ������ ��� ������ ��Ȳ
			// �Ѵ� ���� ��� ������Ʈ�� �ʿ� ����
			if( backupItem )
			{
				// 071231 KTH -- GetID Add
				ItemDeleteToDB( backupItem->dwDBIdx );

				message.mItem[ message.mSize++ ].dwDBIdx = backupItem->dwDBIdx;

				// 071220 LUJ
				LogItemMoney(
					player->GetID(),
					player->GetObjectName(),
					0,
					"",
					eLog_ItemMixUse,
					player->GetMoney(),
					0,
					0,
					backupItem->wIconIdx,
					backupItem->dwDBIdx,
					backupItem->Position,
					0,
					backupItem->Durability,
					0 );
			}

			continue;
		}

		// �������� �ִµ�, �̷� ���Կ� ���� ��� ���ԵǴ� ��Ȳ
		if( ! backupItem )
		{
			const ITEMOBTAINARRAYINFO* itemObtainArray = Alloc( 
				player,
				MP_ITEM,
				MP_ITEM_MIX_GETITEM,
				player->GetID(),
				0,
				eLog_ItemMixSuccess,
				1,
				( DBResult )( ObtainItemDBResult ) );

			ItemInsertToDB(
				player->GetID(),
				item->wIconIdx,
				item->Durability,
				item->Position,
				MAKEDWORD( 1, itemObtainArray->wObtainArrayID ) );

			// �������� �����س��� ���� ����
			message.mItem[ message.mSize++ ] = *item;

			// 071220 LUJ, DB������ ���� ���� �����ǰų� ��ҵ� �� �����Ƿ� ��û ������ �α׸� ������
			LogItemMoney(
				player->GetID(),
				player->GetObjectName(),
				0,
				"",
				eLog_ItemMixSuccess,
				player->GetMoney(),
				0,
				0,
				item->wIconIdx,
				item->dwDBIdx,
				item->Position,
				0,
				item->Durability,
				0 );

			continue;
		}

		// �������� �ִµ�, �̷� ���԰� ������ ������ �ٸ��� ������Ʈ�� ��Ȳ
		if( item->wIconIdx		!= backupItem->wIconIdx ||
			item->Durability	!= backupItem->Durability )
		{
			ItemUpdateToDB(
				player->GetID(),
				item->dwDBIdx,
				item->wIconIdx,
				item->Durability,
				item->Position,
				item->QuickPosition );

			message.mItem[ message.mSize++ ] = *item;

			// 071220, LUJ. ������ �ٸ��� mirrorItem�� �Һ�Ǿ�, item�� ��� �����
			if( item->wIconIdx != backupItem->wIconIdx )
			{
				LogItemMoney(
					player->GetID(),
					player->GetObjectName(),
					0,
					"",
					eLog_ItemMixUse,
					player->GetMoney(),
					0,
					0,
					backupItem->wIconIdx,
					backupItem->dwDBIdx,
					backupItem->Position,
					0,
					backupItem->Durability,
					0 );

				LogItemMoney(
					player->GetID(),
					player->GetObjectName(),
					0,
					"",
					eLog_ItemMixSuccess,
					player->GetMoney(),
					0,
					0,
					item->wIconIdx,
					item->dwDBIdx,
					item->Position,
					0,
					item->Durability,
					0 );

				AddMixOption( *player, *item );
			}
			else if( IsDupItem( item->wIconIdx ) )
			{
				const BOOL isUsed = ( backupItem->Durability > item->Durability );

				LogItemMoney(
					player->GetID(),
					player->GetObjectName(),
					0,
					"",
					isUsed ? eLog_ItemMixUse : eLog_ItemMixSuccess,
					player->GetMoney(),
					0,
					0,
					item->wIconIdx,
					item->dwDBIdx,
					item->Position,
					0,
					abs( int( backupItem->Durability ) - int( item->Durability ) ),
					0 );
			}
			else
			{
				ASSERT( 0 && "update error" );
			}
		}
	}

	player->SendMsg( &message, message.GetSize() );

	return resultType == TypeSuccess ? eLog_ItemMixSuccess : eLog_ItemMixFail;
}


// 080402 LUJ,	���� �� �ɼ��� ���� �ʴ� �������� ���� �ɼ��� �ʱ�ȭ�ؾ� �Ѵ�. �� �ɼ��� Ŭ���̾�Ʈ���� ���� �ʱ�ȭ����
// 080601 LUJ,	���� �� ����� �ɼ��� ��ӵ��� �ʵ���, �� �ɼ� ���̶� �ص� DB�� ����Ѵ�.
void CItemManager::AddMixOption( CPlayer& player, const ITEMBASE& item ) 
{
	// ������ ������ ���, ��� �������� ������ �ɼ� ��ġ�� �ο��޴´�

	const ITEM_INFO* info = GetItemInfo( item.wIconIdx );

	if( !	info ||
			eEquipType_None == info->EquipType )
	{
		return;
	}
	
	MixSetting::eEquippment equippmentType = MixSetting::eEquippmentNone;

	// ������ ���
	switch( info->WeaponType )
	{
	case eWeaponType_Sword:
		{
			equippmentType = MixSetting::eEquippmentSword;
			break;
		}
	case eWeaponType_Mace:
		{
			equippmentType = MixSetting::eEquippmentMace;
			break;
		}
	case eWeaponType_Axe:
		{
			equippmentType = MixSetting::eEquippmentAxe;
			break;
		}
	case eWeaponType_Dagger:
		{
			equippmentType = MixSetting::eEquippmentDagger;
			break;
		}
	case eWeaponType_Staff:
		{
			equippmentType = MixSetting::eEquippmentStaff;
			break;
		}
	case eWeaponType_Bow:
		{
			equippmentType = MixSetting::eEquippmentBow;
			break;
		}
	}

	// ��ȣ���� ���
	switch( info->ArmorType )
	{
	case eArmorType_Robe:
		{
			equippmentType = MixSetting::eEquippmentRobe;
			break;
		}
	case eArmorType_Leather:
		{
			equippmentType = MixSetting::eEquippmentLightArmor;
			break;
		}
	case eArmorType_Metal:
		{
			equippmentType = MixSetting::eEquippmentHeavyArmor;
			break;
		}
	}

	MSG_ITEM_OPTION message;
	{
		ZeroMemory( &message, sizeof( message ) );

		message.Category		= MP_ITEM;
		message.Protocol		= MP_ITEM_OPTION_ADD_ACK;
		message.mSize			= 1;
	}

	ITEM_OPTION& option = message.mOption[ 0 ];

	// 080601 LUJ, ���ǿ� ������ ��� �ɼ��� �ο��Ѵ�
	{
		MixSetting::Equippment::const_iterator it = mMixSetting.mEquippment.find( equippmentType );
		
		if( mMixSetting.mEquippment.end() != it )
		{
			const MixSetting::Ability& ability = it->second;

			const int maxRand	= 100;
			const int key		= rand() % ( maxRand + 1 );

			MixSetting::Ability::const_iterator		ability_it	= ability.lower_bound( key );
			MixSetting::RangeMap::const_iterator	range_it	= mMixSetting.mRangeMap.lower_bound( info->LimitLevel );

			if( ability.end()				!= ability_it &&
				mMixSetting.mRangeMap.end()	!= range_it )
			{
				const MixSetting::Range&	range		= range_it->second;
				const MixSetting::eAbility	abilityType = ability_it->second;

				// ��ġ ���� �ȿ��� ���� ���� ���Ѵ�
				const DWORD			different	= range.mMax - range.mMin;
				const DWORD			randomValue	= ( range.mMin + rand() % ( different + 1 ) );
				ITEM_OPTION::Mix&	mix			= option.mMix;

				switch( abilityType )
				{
				case MixSetting::eAbilityStrength:
					{
						mix.mStrength		= randomValue;
						break;
					}
				case MixSetting::eAbilityDexterity:
					{
						mix.mDexterity		= randomValue;
						break;
					}
				case MixSetting::eAbilityIntelligence:
					{
						mix.mIntelligence	= randomValue;
						break;
					}
				case MixSetting::eAbilityVitality:
					{
						mix.mVitality		= randomValue;
						break;
					}
				case MixSetting::eAbilityWisdom:
					{
						mix.mWisdom			= randomValue;
						break;
					}
				}
			}
		}
	}

	option.mItemDbIndex	= item.dwDBIdx;
	SafeStrCpy( option.mMix.mMadeBy, player.GetObjectName(), sizeof( option.mMix.mMadeBy ) );

	// 081118 LUJ, ĳ���� ��ȣ�� ������ ������ DB �����带 ������ Ű������ ���ǵ��� ��
	ItemOptionInsert( option, player.GetID() );
	// 080314 LUJ, ��� �ɼ��� �������� �� �����Ƿ� �� ���� ����Ἥ �������� �ʱ�ȭ�Ѵ�
	// 081118 LUJ, ĳ���� ��ȣ�� ������ ������ DB �����带 ������ Ű������ ���ǵ��� ��
	ItemDropOptionInsert( option, player.GetID() );
	
	AddOption( option );

	LogItemOption(
		player.GetID(),
		item.dwDBIdx,
		option,
		eLog_ItemMixSuccess );

	player.SendMsg( &message, message.GetSize() );
}


const ITEMBASE * CItemManager::GetItemInfoAbsIn(CPlayer* pPlayer, POSTYPE Pos)
{
	CItemSlot * pSlot = pPlayer->GetSlot(Pos);

	return pSlot ? pSlot->GetItemInfoAbs(Pos) : 0;
}


// 080228 LUJ, ��ȣ ��� ����
eLogitemmoney CItemManager::Reinforce( CPlayer* player, const MSG_ITEM_REINFORCE_SYN& input, const CBackupSlot* backupSlot )
{
	CItemSlot* playerSlot = player->GetSlot( eItemTable_Inventory );

	if( ! playerSlot	||
		! backupSlot )
	{
		return eLog_ItemReinforceSourceInvalid;
	}

	const ITEMBASE* sourceItem = playerSlot->GetItemInfoAbs( input.mSourceItemPosition );

	if( !	sourceItem										||
			sourceItem->dwDBIdx != input.mSourceItemDbIndex	||
			IsDupItem( sourceItem->wIconIdx ) )
	{
		return eLog_ItemReinforceSourceInvalid;
	}

	const ITEM_INFO* sourceItemInfo	= GetItemInfo( sourceItem->wIconIdx );

	if( ! sourceItemInfo )
	{
		return eLog_ItemReinforceMaterialSourceItemCannnotReinforce;
	}

	const POSTYPE	inventoryStartPosition	= TP_INVENTORY_START;
	const POSTYPE	inventoryEndPosition	= TP_INVENTORY_END + player->GetInventoryExpansionSize();
	
	// Ű: ������ �ε���, ��: ����
	typedef std::map< DWORD, DWORD >	MaterialMap;
	MaterialMap							materialMap;

	// ��� �Ұ�
	{
		DURTYPE	quantity = 0;

		for( DWORD i = 0; i < input.mSize; ++i )
		{
			const ITEMBASE& clientItem	= input.mItem[ i ];
			const ITEMBASE* serverItem	= playerSlot->GetItemInfoAbs( clientItem.Position );

			if( !	serverItem											||
				!	IsDupItem( serverItem->wIconIdx )					||
					serverItem->wIconIdx	!=	clientItem.wIconIdx		||
					serverItem->dwDBIdx		!=	clientItem.dwDBIdx		||
					serverItem->Durability	!=	clientItem.Durability )
			{
				return eLog_ItemReinforceMaterialIsInvalid;
			}
			else if( EI_TRUE != playerSlot->DeleteItemAbs(
						player,
						serverItem->Position,
						0 ) )
			{
				return eLog_ItemReinforceMaterialIsInvalid;				
			}

			materialMap[ clientItem.wIconIdx  ] += clientItem.Durability;

			quantity += clientItem.Durability;
		}

		const DURTYPE maxQuantity = 100;

		if( maxQuantity < quantity )
		{
			return eLog_ItemReinforceDataHaveTooManyMaterials;
		}
	}

	// 080228 LUJ, ��ȣ�� �ɼ�
	ITEM_OPTION::Reinforce protectedOption = { 0 };

	// 080228 LUJ, ������ ��� ��ȣ ��带 �����Ѵ�
	if( input.mProtectionItem.wIconIdx )
	{
		// 080228 LUJ, ��ȣ ������ ���
		{
			const ITEMBASE* item = playerSlot->GetItemInfoAbs( input.mProtectionItem.Position );

			if( !	item ||
				!	IsReinforceProtectionItem( *item ) )
			{
				return eLog_ItemReinforceMaterialIsInvalid;
			}

			const ITEM_INFO* info = GetItemInfo( item->wIconIdx );

			if( ! info )
			{
				return eLog_ItemReinforceMaterialIsInvalid;
			}

			if( info->Stack	&&
				1 < item->Durability )
			{
				if( EI_TRUE != playerSlot->UpdateItemAbs( player, item->Position, item->dwDBIdx, item->wIconIdx, item->Position, item->QuickPosition, item->Durability - 1 ) )
				{
					return eLog_ItemReinforceUpdatingItemIsFailed;
				}
			}
			else
			{
				if( EI_TRUE != playerSlot->DeleteItemAbs( player, item->Position, 0 ) )
				{
					return eLog_ItemReinforceUpdatingItemIsFailed;
				}
			}
		}		

		// 080228 LUJ, ��ȣ ������ ��� ���� �˻�: �ɼ��� ���� ��� �� �Ӽ��� �־����
		{
			ITEM_OPTION option = GetOption( *sourceItem );

			// ��æƮ�� ���� ������
			{
				const EnchantScript* enchantScript = GetEnchantScript( option.mEnchant.mIndex );

				if( enchantScript )
				{
					AddEnchantValue( option, enchantScript->mAbility, -1 * sourceItemInfo->EnchantDeterm * option.mEnchant.mLevel );
				}
			}

			protectedOption = option.mReinforce;

			// 080228 LUJ, ��ȣ�� �ɼ��� ���� ��� ��ȭ���� �ʴ´�
			{
				const ITEM_OPTION::Reinforce emptyOption = { 0 };

				if( ! memcmp( &emptyOption, &protectedOption, sizeof( emptyOption ) ) )
				{
					return eLog_ItemReinforceSourceInvalid;
				}
			}

			int count = 0;

			count = ( protectedOption.mStrength			? ++count : count );
			count = ( protectedOption.mDexterity		? ++count : count );
			count = ( protectedOption.mVitality			? ++count : count );
			count = ( protectedOption.mIntelligence		? ++count : count );
			count = ( protectedOption.mWisdom			? ++count : count );
			count = ( protectedOption.mLife				? ++count : count );
			count = ( protectedOption.mMana				? ++count : count );
			count = ( protectedOption.mManaRecovery		? ++count : count );
			count = ( protectedOption.mLifeRecovery		? ++count : count );
			count = ( protectedOption.mPhysicAttack		? ++count : count );
			count = ( protectedOption.mPhysicDefence	? ++count : count );
			count = ( protectedOption.mMagicAttack		? ++count : count );
			count = ( protectedOption.mMagicDefence		? ++count : count );	
			count = ( protectedOption.mCriticalRate		? ++count : count );
			count = ( protectedOption.mCriticalDamage	? ++count : count );
			count = ( protectedOption.mMoveSpeed		? ++count : count );
			count = ( protectedOption.mEvade			? ++count : count );
			count = ( protectedOption.mAccuracy			? ++count : count );

			if( 1 < count )
			{
				return eLog_ItemReinforceMaterialSourceItemCannnotReinforce;
			}
		}

		// 080228 LUJ, ��ȣ ������ ��� ������ ��ȭ �ɼ��� ���� ��ġ�� ������ ��ȭ��Ű�� �Ӽ��� ��ġ�ؾ���
		{
			// 080228 LUJ, ��ȣ ������ ��� ���� �˻�: �� ������ ������ ���Ǿ�� ��. ����־ �ȵ�
			if( materialMap.empty()	||
				1 < materialMap.size() )
			{
				return eLog_ItemReinforceMaterialSourceItemCannnotReinforce;
			}

			MaterialMap::const_iterator it = materialMap.begin();

			const ReinforceScript* script = GetReinforceScript( it->first );

			if( ! script )
			{
				return eLog_ItemReinforceMaterialSourceItemCannnotReinforce;
			}

			// 080228 LUJ, �̸��� �ʱ�ȭ���� ������, ���� �� �׻� �����Ѵ�
			ZeroMemory( &( protectedOption.mMadeBy ), sizeof( protectedOption.mMadeBy ) );
			
			ITEM_OPTION::Reinforce option = { 0 };

			switch( script->mType )
			{
			case ReinforceScript::eTypeStrength:
				{
					option.mStrength = protectedOption.mStrength;
					break;
				}
			case ReinforceScript::eTypeDexterity:
				{
					option.mDexterity = protectedOption.mDexterity;
					break;
				}
			case ReinforceScript::eTypeVitality:
				{
					option.mVitality = protectedOption.mVitality;
					break;
				}
			case ReinforceScript::eTypeWisdom:
				{
					option.mWisdom = protectedOption.mWisdom;
					break;
				}
			case ReinforceScript::eTypeIntelligence:
				{
					option.mIntelligence = protectedOption.mIntelligence;
					break;
				}
			case ReinforceScript::eTypeLife:
				{
					option.mLife = protectedOption.mLife;
					break;
				}
			case ReinforceScript::eTypeLifeRecovery:
				{
					option.mLifeRecovery = protectedOption.mLifeRecovery;
					break;
				}
			case ReinforceScript::eTypeMana:
				{
					option.mMana = protectedOption.mMana;
					break;
				}
			case ReinforceScript::eTypeManaRecovery:
				{
					option.mManaRecovery = protectedOption.mManaRecovery;
					break;
				}
			case ReinforceScript::eTypePhysicAttack:
				{
					option.mPhysicAttack = protectedOption.mPhysicAttack;
					break;
				}
			case ReinforceScript::eTypePhysicDefence:
				{
					option.mPhysicDefence = protectedOption.mPhysicDefence;
					break;
				}
			case ReinforceScript::eTypeMagicAttack:
				{
					option.mMagicAttack = protectedOption.mMagicAttack;
					break;
				}
			case ReinforceScript::eTypeMagicDefence:
				{
					option.mMagicDefence = protectedOption.mMagicDefence;
					break;
				}
			case ReinforceScript::eTypeMoveSpeed:
				{
					option.mMoveSpeed = protectedOption.mMoveSpeed;
					break;
				}
			case ReinforceScript::eTypeEvade:
				{
					option.mEvade = protectedOption.mEvade;
					break;
				}
			case ReinforceScript::eTypeAccuracy:
				{
					option.mAccuracy = protectedOption.mAccuracy;
					break;
				}
			case ReinforceScript::eTypeCriticalRate:
				{
					option.mCriticalRate = protectedOption.mCriticalRate;
					break;
				}
			case ReinforceScript::eTypeCriticalDamage:
				{
					option.mCriticalDamage = protectedOption.mCriticalDamage;
					break;
				}
			}

			if( memcmp( &option, &protectedOption, sizeof( option ) ) )
			{
				return eLog_ItemReinforceMaterialSourceItemCannnotReinforce;
			}
		}
	}

	ITEM_OPTION		option		= { 0 };
	const BOOL		isRareItem	= IsRare( *sourceItemInfo );

	for(
		MaterialMap::const_iterator it = materialMap.begin();
		materialMap.end() != it;
		++it )
	{
		const DWORD				itemIndex	= it->first;
		const DWORD				quantity	= it->second;
		const ReinforceScript*	script		= GetReinforceScript( itemIndex );

		// 0~100������ �������� ���ϰ�, ������ ���� ������ 2�� ������ ���Ѱ��� ��´�
		// 080115 LUJ, ��ȭ ���� ����(��PM�� ��û)
		DWORD value = 0;

		if( 0 < ( rand() % 101 - sourceItemInfo->LimitLevel / 2 ) )
		{
			const float random = float( rand() % 100 + 1 );

			value = min( script->mMax, DWORD( floor( 0.5f + script->mBias * random * float( quantity ) / 100.0f ) ) );
		}

		ITEM_OPTION::Reinforce&	reinforce	= option.mReinforce;

		switch( script->mType )
		{
		case ReinforceScript::eTypeStrength:
			{
				reinforce.mStrength			= max( protectedOption.mStrength, value );
				break;
			}
		case ReinforceScript::eTypeDexterity:
			{
				reinforce.mDexterity		= max( protectedOption.mDexterity, value );
				break;
			}
		case ReinforceScript::eTypeVitality:
			{
				reinforce.mVitality			= max( protectedOption.mVitality, value );
				break;
			}
		case ReinforceScript::eTypeWisdom:
			{
				reinforce.mWisdom			= max( protectedOption.mWisdom, value );
				break;
			}
		case ReinforceScript::eTypeIntelligence:
			{
				reinforce.mIntelligence		= max( protectedOption.mIntelligence, value );
				break;
			}
		case ReinforceScript::eTypeLife:
			{
				reinforce.mLife				= max( protectedOption.mLife, value );
				break;
			}
		case ReinforceScript::eTypeLifeRecovery:
			{
				reinforce.mLifeRecovery		= max( protectedOption.mLifeRecovery, value );
				break;
			}
		case ReinforceScript::eTypeMana:
			{
				reinforce.mMana				= max( protectedOption.mMana, value );
				break;
			}
		case ReinforceScript::eTypeManaRecovery:
			{
				reinforce.mManaRecovery		= max( protectedOption.mManaRecovery, value );
				break;
			}
		case ReinforceScript::eTypePhysicAttack:
			{
				reinforce.mPhysicAttack		= max( protectedOption.mPhysicAttack, value );
				break;
			}
		case ReinforceScript::eTypePhysicDefence:
			{
				reinforce.mPhysicDefence	= max( protectedOption.mPhysicDefence, value );
				break;
			}
		case ReinforceScript::eTypeMagicAttack:
			{
				reinforce.mMagicAttack		= max( protectedOption.mMagicAttack, value );
				break;
			}
		case ReinforceScript::eTypeMagicDefence:
			{
				reinforce.mMagicDefence		= max( protectedOption.mMagicDefence, value );
				break;
			}
		case ReinforceScript::eTypeMoveSpeed:
			{
				reinforce.mMoveSpeed		= max( protectedOption.mMoveSpeed, value );
				break;
			}
		case ReinforceScript::eTypeEvade:
			{
				reinforce.mEvade			= max( protectedOption.mEvade, value );
				break;
			}
		case ReinforceScript::eTypeAccuracy:
			{
				reinforce.mAccuracy			= max( protectedOption.mAccuracy, value );
				break;
			}
		case ReinforceScript::eTypeCriticalRate:
			{
				reinforce.mCriticalRate		= max( protectedOption.mCriticalRate, value );
				break;
			}
		case ReinforceScript::eTypeCriticalDamage:
			{
				reinforce.mCriticalDamage	= max( protectedOption.mCriticalDamage, value );
				break;
			}
		default:
			{
				ASSERT( 0 && "invaild script. check it now" );
				break;
			}
		}
	}

	BOOL isFailed = FALSE;

	// ��ȭ�� �ɼǰ��� �ִ��� üũ�ϰ� ������ DB�� ��������
	{
		const ITEM_OPTION emptyOption = { 0 };

		ASSERT( sizeof( emptyOption ) == sizeof( option ) );

		// �޸� ���� ��ȭ ���� ��ġ��. �� ��ȭ ����.
		if( ! memcmp( &option, &emptyOption, sizeof( option ) ) )
		{
			LogItemMoney(
				player->GetID(),
				player->GetObjectName(),
				0,
				"",
				eLog_ItemReinforceFail, 
				player->GetMoney(),
				0,
				0,
				sourceItem->wIconIdx,
				sourceItem->dwDBIdx,
				sourceItem->Position,
				sourceItem->Position,
				sourceItem->Durability,
				0 );

			isFailed = TRUE;
		}
		// ���� ������ �ɼ� ó��. ��æƮ�� �������� ���� ��ġ�� ��ȭ �Ŀ��� �����ؾ� �Ѵ�.
		else
		{
			const ITEM_OPTION& previousOption = GetOption( *sourceItem );

			option.mItemDbIndex	= sourceItem->dwDBIdx;

			if( previousOption.mItemDbIndex )
			{
				// ��æƮ�� �������� ���� �߰��� �����־�� �Ѵ�
				{
					option.mEnchant = previousOption.mEnchant;

					const int				value	= sourceItemInfo->EnchantDeterm * option.mEnchant.mLevel;
					const EnchantScript*	script	= GetEnchantScript( option.mEnchant.mIndex );

					if( script )
					{
						AddEnchantValue( option, script->mAbility, value );
					}
				}			

				// ���� �ɼ� ����
				{
					option.mMix		= previousOption.mMix;
					option.mDrop	= previousOption.mDrop;
				}
			}

			SafeStrCpy(
				option.mReinforce.mMadeBy,
				player->GetObjectName(),
				sizeof( option.mReinforce.mMadeBy ) );

			// 081118 LUJ, ĳ���� ��ȣ�� ������ ������ DB �����带 ������ Ű������ ���ǵ��� ��
			ItemOptionInsert( option, player->GetID() );
			AddOption( option );

			// 080428 LUJ, �α�
			LogItemOption(
				player->GetID(),
				option.mItemDbIndex,
				option,
				eLog_ItemReinforceSuccess );
			// 081202 LUJ, �븸�� ��û���� ��ȭ ������ �������� �α׸� �ۼ��Ѵ�
			LogItemMoney(
				player->GetID(),
				player->GetObjectName(),
				0,
				"*reinforce",
				eLog_ItemReinforceSuccess, 
				player->GetMoney(),
				0,
				0,
				sourceItem->wIconIdx,
				sourceItem->dwDBIdx,
				sourceItem->Position,
				sourceItem->Position,
				sourceItem->Durability,
				0 );
		}
	}

	MSG_ITEM_REINFORCE_ACK message;
	{
		ZeroMemory( &message, sizeof( message ) );

		message.Category			= MP_ITEM;
		message.Protocol			= ( isFailed ? MP_ITEM_REINFORCE_FAILED_ACK : MP_ITEM_REINFORCE_SUCCEED_ACK );
		message.mSourceItemDbIndex	= input.mSourceItemDbIndex;
	}

	// ���Ե� �޽����� �����Ѵ�. �ٸ� DB �ε����� ���������� ��� ���̴�
	// ���� ������ �������� ������ �ε����� ����. ������Ʈ�� �������� �����۸� �ش�ȴ�
	for( POSTYPE position = inventoryStartPosition; inventoryEndPosition > position; ++position )
	{
		const ITEMBASE*	item;
		{
			item = playerSlot->GetItemInfoAbs( position );

			// DB �ε����� ������ �� ����
			if( ! item ||
				! item->dwDBIdx )
			{
				item = 0;
			}
		}

		const ITEMBASE*	backupItem = backupSlot->GetItem( position );

		if( ! item )
		{
			// �������� ���µ�, �̷� ���Կ��� ������ ��� ������ ��Ȳ
			// �Ѵ� ���� ��� ������Ʈ�� �ʿ� ����
			if( backupItem )
			{
				// 071231 KTH -- GetID add
				ItemDeleteToDB( backupItem->dwDBIdx );

				message.mItem[ message.mSize++ ].dwDBIdx = backupItem->dwDBIdx;

				// 071220 LUJ
				LogItemMoney(
					player->GetID(),
					player->GetObjectName(),
					0,
					"",
					eLog_ItemDestroyReinforce,
					player->GetMoney(),
					0,
					0,
					backupItem->wIconIdx,
					backupItem->dwDBIdx,
					backupItem->Position,
					0,
					backupItem->Durability,
					0 );
			}

			continue;
		}

		// �������� �ִµ�, �̷� ���԰� ������ ������ �ٸ��� ������Ʈ�� ��Ȳ
		if( item->wIconIdx		!= backupItem->wIconIdx ||
			item->Durability	!= backupItem->Durability )
		{
			ItemUpdateToDB(
				player->GetID(),
				item->dwDBIdx,
				item->wIconIdx,
				item->Durability,
				item->Position,
				item->QuickPosition );

			message.mItem[ message.mSize++ ] = *item;

			// 071220, LUJ. ������ �ٸ��� mirrorItem�� �Һ�Ǿ�, item�� ��� �����
			if( item->wIconIdx != backupItem->wIconIdx )
			{
				LogItemMoney(
					player->GetID(),
					player->GetObjectName(),
					0,
					"",
					eLog_ItemDestroyReinforce,
					player->GetMoney(),
					0,
					0,
					backupItem->wIconIdx,
					backupItem->dwDBIdx,
					backupItem->Position,
					0,
					backupItem->Durability,
					0 );

				LogItemMoney(
					player->GetID(),
					player->GetObjectName(),
					0,
					"",
					eLog_ItemReinforceSuccess,
					player->GetMoney(),
					0,
					0,
					item->wIconIdx,
					item->dwDBIdx,
					item->Position,
					0,
					item->Durability,
					0 );
			}
			else if( IsDupItem( item->wIconIdx ) )
			{
				const BOOL isUsed = ( backupItem->Durability > item->Durability );

				LogItemMoney(
					player->GetID(),
					player->GetObjectName(),
					0,
					"",
					isUsed ? eLog_ItemDestroyReinforce : eLog_ItemReinforceSuccess,
					player->GetMoney(),
					0,
					0,
					item->wIconIdx,
					item->dwDBIdx,
					item->Position,
					0,
					item->Durability,
					0 );
			}
			else
			{
				ASSERT( 0 && "update error" );
			}
		}
	}
	
	player->SendMsg( &message, message.GetSize() );

	// ��� ����
	if( isFailed )
	{
		return eLog_ItemReinforceFail;
	}
	else
	{
		MSG_ITEM_REINFORCE_OPTION_ACK m;
		m.Category		= MP_ITEM;
		m.Protocol		= MP_ITEM_REINFORCE_OPTION_ACK;
		m.mItemDbIndex	= sourceItem->dwDBIdx;
		m.mOption		= GetOption( *sourceItem );

		player->SendMsg( &m, sizeof( m ) );
		
		return eLog_ItemReinforceSuccess;
	}	
}


// 080228 LUJ, ��ȣ ������ ��� �� ��æƮ ���� �ÿ��� �Ҹ���� �ʵ��� ��
// 080320 LUJ, ��æƮ ���� ��ȣ ��� �߰�
MP_PROTOCOL_ITEM CItemManager::Enchant(CPlayer* player, const MSG_ITEM_ENCHANT_SYN* message, eLogitemmoney& log )
{
	const DWORD		sourceIndex			= message->wItemIdx;
	const DWORD		materialIndex		= message->wMaterialItemIdx;
	const POSTYPE	sourcePosition		= message->ItemPos;
	const POSTYPE	materialPosition	= message->MaterialItemPos;

	if( !	player																				||
		!	CHKRT->ItemOf(player, sourcePosition, sourceIndex, 0,0, CB_EXIST|CB_ICONIDX)		||
		!	CHKRT->ItemOf(player, materialPosition, materialIndex, 0,0, CB_EXIST|CB_ICONIDX)	||
		!	player->IsInventoryPosition( sourcePosition ) )
	{
		log = eLog_ItemEnchantHacking;

		return MP_ITEM_ENCHANT_NACK;
	}

	const ITEM_INFO* sourceInfo	= GetItemInfo( sourceIndex );

	if( !	sourceInfo					||
		!	sourceInfo->IsEnchant		||
			sourceInfo->Stack			||
		!	IsDupItem( materialIndex ) )
	{
		log = eLog_ItemEnchantHacking;

		return MP_ITEM_ENCHANT_NACK;
	}

	const EnchantScript* script = GetEnchantScript( materialIndex );

	if( !	script												||
			script->mItemLevel.mMin > sourceInfo->LimitLevel	||
			script->mItemLevel.mMax < sourceInfo->LimitLevel	)
	{
		log = eLog_ItemEnchantHacking;

		return MP_ITEM_ENCHANT_NACK;
	}

	CItemSlot* inventory = player->GetSlot( eItemTable_Inventory );

	// 080228 LUJ, ��ȣ ������ ������
	const EnchantProtection::Data* protectionData = 0;
	
	// 080228 LUJ, ��ȣ �������� ���� ��� ���
	if( message->mProtectionItem.wIconIdx )
	{
		const ITEMBASE* sourceItem = inventory->GetItemInfoAbs( sourcePosition );

		if( ! sourceItem )
		{
			return MP_ITEM_ENCHANT_NACK;
		}
		
		EnchantProtection::ItemMap::const_iterator it = mEnchantProtection.mItemMap.find( message->mProtectionItem.wIconIdx );

		// 080320 LUJ, ��æƮ ��ȣ ���¶�� ���� ������ �´��� üũ
		if( mEnchantProtection.mItemMap.end() != it )
		{
			const ITEM_OPTION& option = GetOption( *sourceItem );

			protectionData = &( it->second );

			if( option.mEnchant.mLevel < protectionData->mEnchantLevelRange.mMin ||
				option.mEnchant.mLevel > protectionData->mEnchantLevelRange.mMax )
			{
				return MP_ITEM_ENCHANT_NACK;
			}
			// 080328 LUJ, ������ ���� ���� üũ
			else if(	sourceInfo->LimitLevel < protectionData->mItemLimitLevelRange.mMin	||
						sourceInfo->LimitLevel > protectionData->mItemLimitLevelRange.mMax )
			{
				return MP_ITEM_ENCHANT_NACK;
			}
		}

		const ITEMBASE* item = inventory->GetItemInfoAbs( message->mProtectionItem.Position );

		if( !	item											||
				item->dwDBIdx != message->mProtectionItem.dwDBIdx )
		{
			return MP_ITEM_ENCHANT_NACK;
		}
		else if( EI_TRUE != DiscardItem(
			player,
			item->Position,
			item->wIconIdx, 1 ) )
		{
			return MP_ITEM_ENCHANT_NACK;
		}
	const ITEMBASE* protectionItem = inventory->GetItemInfoAbs(message->mProtectionItem.Position);

	if(protectionItem->wIconIdx != message->mProtectionItem.wIconIdx)
	{
		return MP_ITEM_ENCHANT_NACK;
	}
		// 080428 LUJ, ��ȣ ������ ��� �α�
		LogItemMoney(
			player->GetID(),
			player->GetObjectName(),
			0,
			"*protect",
			eLog_ItemUse,
			player->GetMoney(),
			0,
			0,
			message->mProtectionItem.wIconIdx,
			message->mProtectionItem.dwDBIdx,
			message->mProtectionItem.Position,
			0,
			1,
			0 );
	}

	ITEMBASE materialForLog;
	ZeroMemory( &materialForLog, sizeof( materialForLog ) );
	{
		const ITEMBASE* material = inventory->GetItemInfoAbs( materialPosition );

		if( material )
		{
			materialForLog = *material;
		}
	}
	
	// ��� �ı�
	if( EI_TRUE != DiscardItem( player, materialPosition, materialIndex, 1 ) )
	{
		log = eLog_ItemEnchantServerError;

		return MP_ITEM_ENCHANT_NACK;
	}
	// ���� ��ġ ���� ���� �˻�
	else
	{
		const eEquipSlot slot = eEquipSlot( sourceInfo->EquipSlot + 1 );

		if( script->mSlot.end() == script->mSlot.find( slot ) )
		{
			log = eLog_ItemEnchantHacking;

			return MP_ITEM_ENCHANT_NACK;
		}
	}

	// ��æƮ�� �ҿ�� ������ �α�
	LogItemMoney(
		player->GetID(),
		player->GetObjectName(),
		0,
		"",
		eLog_ItemUse, 
		player->GetMoney(),
		0,
		0,
		materialForLog.wIconIdx,
		materialForLog.dwDBIdx,
		materialForLog.Position,
		0, 
		1,
		0 );

	const ITEMBASE*	source	= inventory->GetItemInfoAbs( sourcePosition );
	
	ITEMBASE sourceForLog;
	{
		if( source )
		{
			sourceForLog = *source;
		}
		else
		{
			ZeroMemory( &sourceForLog, sizeof( sourceForLog ) );
		}
	}

	ITEM_OPTION	option	= GetOption( sourceForLog );

	// ���� ���� ����
	{		
		// 071226 LUJ,	��æƮ ��ġ ���
		//				����: value�� 0.00x���� ������ õ������ ������ ������ ������Ѿ� ��
		const float basis			= 0.80f;

		// 080328 LUJ, �߰� Ȯ�� ����. �׷� ���� Ÿ���� int�� ����
		const int value			= int( 1000.0f * pow( basis, option.mEnchant.mLevel ) ) + ( protectionData ? protectionData->mAddedPercent : 0 );
		const int randomValue	= rand() % 1000;

		if( randomValue > value )
		{
			// 080328 LUJ, ��ȣ ������ �� ������ ���, ���̳ʽ� ���ʽ��� �����Ѵ�
			if( protectionData )
			{
				// 080328 LUJ, std::map::lower_bound()�� ����ϸ� �������� ���� ���� �� �ִ�. �ڼ��� ������ STL Ʃ�丮�� ����
				EnchantProtection::Data::BonusMap::const_iterator bonus_it = protectionData->mMinusBonusMap.lower_bound( mRandom.GetFloat() );

				if( protectionData->mMinusBonusMap.end() != bonus_it )
				{
					const BYTE previousEnchantLevel = option.mEnchant.mLevel;

					// 080328 LUJ,	��æƮ ���� ����.
					//				����! ���� �� ���� ������ ���� ��æƮ �������� ���� �� �ִ�. �̶��� ���� ������ �����ؼ��� �ȵȴ�
					{
						const int bonusLevel	= bonus_it->second;
                        const int enchantLevel	= BYTE( max( int( option.mEnchant.mLevel ) + bonusLevel, int( protectionData->mMinimumEnchantLevel ) ) );

						option.mEnchant.mLevel	= min( enchantLevel, previousEnchantLevel );
					}

					// 080328 LUJ, ���ҵ� ��ŭ ��æƮ ������ ����
					AddEnchantValue( option, script->mAbility, -1 * ( previousEnchantLevel - option.mEnchant.mLevel ) * sourceInfo->EnchantDeterm );

					// 080328 LUJ, ��æƮ ������ ���ٸ� ��æƮ ������ �ʱ�ȭ����
					if( ! option.mEnchant.mLevel )
					{
						ZeroMemory( &option.mEnchant, sizeof( option.mEnchant ) );
					}
					
					// 080320 LUJ, �޸�/DB ������Ʈ
					AddOption( option );
					// 081118 LUJ, ĳ���� ��ȣ�� ������ ������ DB �����带 ������ Ű������ ���ǵ��� ��
					ItemOptionInsert( option, player->GetID() );
					// 080428 LUJ, �α�
					LogItemOption(
						player->GetID(),
						option.mItemDbIndex,
						option,
						eLog_ItemEnchantDestroy );

					MSG_ITEM_OPTION message;
					ZeroMemory( &message, sizeof( message ) );
					message.Category					= MP_ITEM;
					message.Protocol					= MP_ITEM_OPTION_ADD_ACK;
					message.mOption[ message.mSize++ ]	= option;

					player->SendMsg( &message, message.GetSize() );
				}

				return MP_ITEM_ENCHANT_FAILED_ACK;
			}

			// ��� ������ �ı�
			else if( EI_TRUE != DiscardItem( player, sourcePosition, sourceIndex, 1 ) )
			{
				log = eLog_ItemEnchantServerError;

				return MP_ITEM_ENCHANT_NACK;
			}

			// ��� ������ �ı� �α�
			LogItemMoney(
				player->GetID(),
				player->GetObjectName(),
				0,
				"",
				eLog_ItemEnchantDestroy, 
				player->GetMoney(),
				0,
				0,
				sourceForLog.wIconIdx,
				sourceForLog.dwDBIdx,
				sourceForLog.Position,
				0, 
				1,
				0 );

			return MP_ITEM_ENCHANT_FAILED_ACK;
		}
	}

	// 080328 LUJ, ��æƮ ������ 1 ������Ű��, ��ȣ ������ �� ���ʽ� ������ �߰��� ������Ų��
	{
		const int previousEnchantLevel = option.mEnchant.mLevel;

		++option.mEnchant.mLevel;
		option.mEnchant.mIndex	= materialIndex;

		// 080328 LUJ, ��æƮ ��ȣ �����϶� ������ ��� �߰� ���ʽ��� �ο��Ѵ�
		if( protectionData )
		{
			// 080328 LUJ, std::map::lower_bound()�� ����ϸ� �������� ���� ���� �� �ִ�. �ڼ��� ������ STL Ʃ�丮�� ����
			EnchantProtection::Data::BonusMap::const_iterator it = protectionData->mPlusBonusMap.lower_bound( mRandom.GetFloat() );

			const int bonusLevel	= ( protectionData->mPlusBonusMap.end() == it ? 0 : it->second );
			option.mEnchant.mLevel	= BYTE( min( int( option.mEnchant.mLevel ) + bonusLevel, int( script->mEnchantMaxLevel ) ) );
		}	

		AddEnchantValue( option, script->mAbility, ( option.mEnchant.mLevel - previousEnchantLevel ) * sourceInfo->EnchantDeterm );
	}

	CItemSlot* sourceSlot = player->GetSlot( sourcePosition );

	if( EI_TRUE != sourceSlot->UpdateItemAbs( player, sourcePosition, 0, 0, 0, 0, 0, UB_DURA ) )
	{
		log = eLog_ItemEnchantItemUpdateFailed;

		return MP_ITEM_ENCHANT_NACK;
	}

	SafeStrCpy( option.mEnchant.mMadeBy, player->GetObjectName(), sizeof( option.mEnchant.mMadeBy ) );

	option.mItemDbIndex	= sourceForLog.dwDBIdx;

	// 081118 LUJ, ĳ���� ��ȣ�� ������ ������ DB �����带 ������ Ű������ ���ǵ��� ��
	ItemOptionInsert( option, player->GetID() );
	AddOption( option );

	// 080428 LUJ, �α�
	LogItemOption(
		player->GetID(),
		option.mItemDbIndex,
		option,
		eLog_ItemEnchantSuccess );

	// �޽��� ����
	{
		MSG_ITEM_ENCHANT_ACK m;
		m.Category		= MP_ITEM;
		m.Protocol		= MP_ITEM_ENCHANT_SUCCESS_ACK;
		m.TargetPos		= sourcePosition;
		m.OptionInfo	= option;

		player->SendMsg( &m, sizeof( m ) );
	}

	{
		char text[ MAX_NAME_LENGTH + 1 ] = { 0 };

		sprintf( text, "enchant:%d", option.mEnchant.mLevel );

		// ��æƮ ���� �α�
		LogItemMoney(
			player->GetID(),
			player->GetObjectName(),
			0,
			text,
			eLog_ItemEnchantSuccess, 
			player->GetMoney(),
			0,
			0,
			sourceForLog.wIconIdx,
			sourceForLog.dwDBIdx,
			sourceForLog.Position,
			0, 
			1,
			0 );
	}

	return MP_ITEM_ENCHANT_SUCCESS_ACK;
}


ITEM_INFO * CItemManager::GetItemInfo(DWORD wItemIdx)
{
	return m_ItemInfoList.GetData(wItemIdx);
}


void CItemManager::LoadItemList()
{
	ITEM_INFO * pInfo = NULL;
	CMHFile file;
	if(!file.Init("System/Resource/ItemList.bin", "rb"))
		return;

	while(1)
	{
		if(file.IsEOF())
			break;

		DWORD ItemIdx = file.GetDword();
		if(m_ItemInfoList.GetData(ItemIdx) == NULL)
		{
			pInfo = new ITEM_INFO;
			SetItemInfo(ItemIdx,pInfo,&file);
			m_ItemInfoList.Add(pInfo, pInfo->ItemIdx);
		}
		else
		{
			ASSERTMSG(0, "LoadItemlist");
		}
	}
	file.Release();
}

// desc_hseos_ü����������01
// S ü���������� �߰� added by hseos 2007.07.29
// -------------------------------------------------------------------------------------------------------------------------------------
//  LoadScriptFileDataChangeItem Method													 		   ü���� ������ ���� ��ũ��Ʈ ���� �ε�
//
BOOL CItemManager::LoadScriptFileDataChangeItem()
{
	char szFile[MAX_PATH] = "./System/Resource/ChangeItem.bin";
	char szLine[MAX_PATH], szTxt[MAX_PATH];
	int	 nKind = 0, nResult = 0, nResultBase = 0;
	CMHFile fp;

	// ü���� ������ �ִ� ���� ���
	fp.Init(szFile, "rb");
	if(!fp.IsInited())
	{
		return FALSE;
	}

	while(TRUE)
	{
		if (fp.IsEOF()) break;
		fp.GetLine(szLine, sizeof(szLine));
		if (strstr(szLine, "//") ||
			IsEmptyLine(szLine))
		{
			continue;			
		}
		else if (strstr(szLine, "END_KIND")) 
		{
			m_nChangeItemInfoNum++;
			continue;
		}
	}

	fp.Release();

	m_stChangeItemInfo = new stCHANGEITEM[m_nChangeItemInfoNum];
	ZeroMemory(m_stChangeItemInfo, sizeof(stCHANGEITEM)*m_nChangeItemInfoNum);

	// ü���� ������ ��� �ִ� ���� ���
	fp.Init(szFile, "rb");
	if(!fp.IsInited())
	{
		return FALSE;
	}
	while(TRUE)
	{
		if (fp.IsEOF()) break;
		fp.GetLine(szLine, sizeof(szLine));
		if (strstr(szLine, "//") ||
			IsEmptyLine(szLine))
		{
			continue;			
		}
		else if (strncmp(szLine, "-", 1) == 0)
		{
			m_stChangeItemInfo[nKind].nResultNum++;
			continue;
		}
		else if (strstr(szLine, "END_KIND"))
		{
			m_stChangeItemInfo[nKind].nResultNum++;
			m_stChangeItemInfo[nKind].stResult = new stCHANGEITEM_RESULT[m_stChangeItemInfo[nKind].nResultNum];
			ZeroMemory(m_stChangeItemInfo[nKind].stResult, sizeof(*m_stChangeItemInfo[nKind].stResult)*m_stChangeItemInfo[nKind].nResultNum);
			nKind++;
			continue;
		}
	}

	fp.Release();

	// ü���� ������ ����� ������ �ִ� ���� ���
	nKind = 0; 
	nResult = 0;
	fp.Init(szFile, "rb");
	if(!fp.IsInited())
	{
		return FALSE;
	}
	while(TRUE)
	{
		if (fp.IsEOF()) break;
		fp.GetLine(szLine, sizeof(szLine));
		if (strstr(szLine, "//") ||
			IsEmptyLine(szLine))
		{
			continue;			
		}
		else if (strncmp(szLine, "-", 1) == 0)
		{
			if (nResult == 0) m_stChangeItemInfo[nKind].stResult[nResult].nResultBaseNum--;
			m_stChangeItemInfo[nKind].stResult[nResult].stResultBase = new stCHANGEITEM_RESULT_BASE[m_stChangeItemInfo[nKind].stResult[nResult].nResultBaseNum];
			ZeroMemory(m_stChangeItemInfo[nKind].stResult[nResult].stResultBase, sizeof(*m_stChangeItemInfo[nKind].stResult[nResult].stResultBase)*m_stChangeItemInfo[nKind].stResult[nResult].nResultBaseNum);
			nResult++;
			continue;			
		}
		else if (strstr(szLine, "END_KIND"))
		{
			if (nResult == 0) m_stChangeItemInfo[nKind].stResult[nResult].nResultBaseNum--;
			m_stChangeItemInfo[nKind].stResult[nResult].stResultBase = new stCHANGEITEM_RESULT_BASE[m_stChangeItemInfo[nKind].stResult[nResult].nResultBaseNum];
			ZeroMemory(m_stChangeItemInfo[nKind].stResult[nResult].stResultBase, sizeof(*m_stChangeItemInfo[nKind].stResult[nResult].stResultBase)*m_stChangeItemInfo[nKind].stResult[nResult].nResultBaseNum);
			nResult = 0;
			nKind++;
			continue;
		}

		m_stChangeItemInfo[nKind].stResult[nResult].nResultBaseNum++;
	}

	fp.Release();


	// ������ �б�
	nKind = 0;
	nResult = 0;
	nResultBase = -1;
	fp.Init(szFile, "rb");
	if(!fp.IsInited())
	{
		return FALSE;
	}
	while(TRUE)
	{
		if (fp.IsEOF()) break;
		fp.GetLine(szLine, sizeof(szLine));
		if (strstr(szLine, "//") ||
			IsEmptyLine(szLine))
		{
			continue;			
		}
		else if (strncmp(szLine, "-", 1) == 0)
		{
			nResult++;
			nResultBase = 0;
			continue;
		}
		else if (strstr(szLine, "END_KIND"))
		{
			nKind++;
			nResult = 0;
			nResultBase = -1;
			continue;
		}

		if (nResultBase == -1)
		{
			// ������ �б�
			sscanf(szLine, "%u", &m_stChangeItemInfo[nKind].nChangeItemID);
		}
		else
		{
			// ������ �б�
			sscanf(szLine, "%s %u %u %u %u", szTxt, 
				&m_stChangeItemInfo[nKind].stResult[nResult].stResultBase[nResultBase].nValue01, 
				&m_stChangeItemInfo[nKind].stResult[nResult].stResultBase[nResultBase].nValue02, 
				&m_stChangeItemInfo[nKind].stResult[nResult].stResultBase[nResultBase].nValue03, 
				&m_stChangeItemInfo[nKind].stResult[nResult].stResultBase[nResultBase].nProbaValue);

			if (nResultBase > 0)
			{
				m_stChangeItemInfo[nKind].stResult[nResult].stResultBase[nResultBase].nProbaValue += 
					m_stChangeItemInfo[nKind].stResult[nResult].stResultBase[nResultBase-1].nProbaValue;
			}

			// ��� ����
			if		(stricmp(szTxt, "ITEM") == 0)	m_stChangeItemInfo[nKind].stResult[nResult].stResultBase[nResultBase].eResultKind = TRK_ITEM;
			else if (stricmp(szTxt, "MONEY") == 0)	m_stChangeItemInfo[nKind].stResult[nResult].stResultBase[nResultBase].eResultKind = TRK_MONEY;
		}
		nResultBase++;
	}

	fp.Release();

	FILE* fpCheck = fopen("ChangeItemCheck.txt", "wt");
	for(int i=0; i<m_nChangeItemInfoNum; i++)
	{
		fprintf(fpCheck, "%u\n", m_stChangeItemInfo[i].nChangeItemID);
		for(int j=0; j<m_stChangeItemInfo[i].nResultNum; j++)
		{
			fprintf(fpCheck, "-\n");
			for(int k=0; k<m_stChangeItemInfo[i].stResult[j].nResultBaseNum; k++)
			{
				char szText[10000];

				sprintf(szText, "%u	%u	%u	%u	%u\n",	
					m_stChangeItemInfo[i].stResult[j].stResultBase[k].eResultKind,
					m_stChangeItemInfo[i].stResult[j].stResultBase[k].nValue01,
					m_stChangeItemInfo[i].stResult[j].stResultBase[k].nValue02,
					m_stChangeItemInfo[i].stResult[j].stResultBase[k].nValue03,
					m_stChangeItemInfo[i].stResult[j].stResultBase[k].nProbaValue);
				fprintf(fpCheck, szText);
			}
		}
	}
	fclose(fpCheck);

	return TRUE;
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  IsChangeItem Method																								 ü���� ������ �ΰ�?
//
BOOL CItemManager::IsChangeItem(DWORD nItemID)
{
	if (m_stChangeItemInfo)
	{
		for(int i=0; i<m_nChangeItemInfoNum; i++)
		{
			if (nItemID == m_stChangeItemInfo[i].nChangeItemID)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ProcChangeItem Method																							  ü���� ������ ó��
//
BOOL CItemManager::ProcChangeItem(CPlayer* pPlayer, DWORD nItemID)
{
	if (m_stChangeItemInfo)
	{
		for(int i=0; i<m_nChangeItemInfoNum; i++)
		{
			if (nItemID == m_stChangeItemInfo[i].nChangeItemID)
			{
				// ��ȿ �������ΰ�?
				// ..��ũ��Ʈ ������ �����̳� ��Ÿ����� ������ ��ȿ���� ���� �������̶�� ó������ �ʴ´�.
				ITEM_INFO* pstItem = GetItemInfo(nItemID);
				if (pstItem == NULL)
				{
					continue;
				}

				// ��� ����
				for(int j=0; j<m_stChangeItemInfo[i].nResultNum; j++)
				{
					//DWORD nProba = CSHMath::GetRandomNum(0, 9999);
					// 080924 KTH -- ChangeItem Random Ratio Fix
					DWORD nProba;
					nProba = CSHMath::GetRandomNum(0, 9999);
					nProba += CSHMath::GetRandomNum(0, 99) * 10000;
					
					for(int k=0; k<m_stChangeItemInfo[i].stResult[j].nResultBaseNum; k++)
					{
						switch(m_stChangeItemInfo[i].stResult[j].stResultBase[k].eResultKind)
						{
							// ������
						case TRK_ITEM:
							{
								CItemSlot* pSlot = pPlayer->GetSlot(eItemTable_Inventory);
								if(NULL == pSlot)
								{
									return FALSE;
								}

								// 071211 KTH -- pPlayer�߰�
								DWORD nNum = GetTotalEmptySlotNum(pSlot, pPlayer);
								if(nNum == 0)
								{
									return FALSE;
								}

								if (nProba < m_stChangeItemInfo[i].stResult[j].stResultBase[k].nProbaValue ||
									k == m_stChangeItemInfo[i].stResult[j].nResultBaseNum-1)
								{
									int nItemID  = m_stChangeItemInfo[i].stResult[j].stResultBase[k].nValue01;
									int nItemNum1 = m_stChangeItemInfo[i].stResult[j].stResultBase[k].nValue02;
									int nItemNum2 = m_stChangeItemInfo[i].stResult[j].stResultBase[k].nValue03;

									if (m_stChangeItemInfo[i].stResult[j].stResultBase[k].nValue03 == 0)
									{
										nItemNum1 = 1;
										nItemNum2 = m_stChangeItemInfo[i].stResult[j].stResultBase[k].nValue02;
									}

									int nItemNum = CSHMath::GetRandomNum(nItemNum1, nItemNum2);
									//ObtainMonstermeterItem(pPlayer, nItemID, nItemNum);
									ObtainItemFromChangeItem( pPlayer, nItemID, nItemNum );

									goto NEXT_RESULT;
								}
							}
							break;
							// ��
						case TRK_MONEY:
							{
								if (nProba < m_stChangeItemInfo[i].stResult[j].stResultBase[k].nProbaValue ||
									k == m_stChangeItemInfo[i].stResult[j].nResultBaseNum-1)
								{
									int nMoney = m_stChangeItemInfo[i].stResult[j].stResultBase[k].nValue01;
									if( pPlayer->SetMoney(nMoney, MONEY_ADDITION, MF_OBTAIN, eItemTable_Inventory, eMoneyLog_GetMonstermeter) != nMoney )
									{
										MSGBASE msg;
										msg.Category = MP_ITEM;
										msg.Protocol = MP_ITEM_MONEY_ERROR;
										msg.dwObjectID = pPlayer->GetID();

										pPlayer->SendMsg(&msg, sizeof(msg));
										return FALSE;
									}
									goto NEXT_RESULT;
								}
							}
							break;
						}
					}
NEXT_RESULT:;
				}
			}
		}
	}

	return TRUE;
}
// E ü���������� �߰� added by hseos 2007.07.29


void CItemManager::ReloadItemList()
{
	CMHFile file;

	if(!file.Init("System/Resource/ItemList.bin", "rb"))
	{
		return;
	}

	ITEM_INFO* pInfo;

	while(pInfo = m_ItemInfoList.GetData())
	{
		delete pInfo;
	}

	m_ItemInfoList.RemoveAll();

	while( ! file.IsEOF() )
	{
		DWORD ItemIdx = file.GetDword();
		pInfo = m_ItemInfoList.GetData(ItemIdx);

		if(pInfo)
		{
			SetItemInfo(ItemIdx,pInfo,&file);
		}
		else
		{
			pInfo = new ITEM_INFO;
			SetItemInfo(ItemIdx,pInfo,&file);
			m_ItemInfoList.Add(pInfo, pInfo->ItemIdx);
		}
	}
}


void CItemManager::SetItemInfo(DWORD ItemIdx,ITEM_INFO* pInfo,CMHFile* pFile)
{
	pInfo->ItemIdx = ItemIdx;
	SafeStrCpy( pInfo->ItemName, pFile->GetString(), MAX_ITEMNAME_LENGTH+1 );

	pInfo->ItemTooltipIdx = pFile->GetWord();
	pInfo->Image2DNum = pFile->GetWord();
//	pInfo->wIconIdx = pFile->GetWord();
	pInfo->Part3DType = pFile->GetWord();
	pInfo->Part3DModelNum = pFile->GetWord();	

	pInfo->Shop = pFile->GetWord(); // �����۸� ����
	pInfo->Category = pFile->GetWord();			

	pInfo->Grade = pFile->GetWord();			

	pInfo->LimitRace = pFile->GetWord();
	pInfo->LimitGender = pFile->GetWord();		
	pInfo->LimitLevel = pFile->GetLevel();	

	pInfo->Stack = pFile->GetWord();
	pInfo->Trade = pFile->GetWord();
	pInfo->Deposit = pFile->GetWord();
	pInfo->Sell = pFile->GetWord();
	pInfo->Decompose = pFile->GetWord();

	pInfo->DecomposeIdx = pFile->GetDword();
	pInfo->Time = pFile->GetDword();

	pInfo->BuyPrice = pFile->GetDword();			
	pInfo->SellPrice = pFile->GetDword();			

	pInfo->EquipType = pFile->GetWord();
//	pInfo->EquipSlot = pFile->GetWord() - 1;

	pInfo->Durability = pFile->GetDword();
	pFile->GetDword();

	pInfo->Repair = pFile->GetWord();

	pInfo->WeaponAnimation = pFile->GetWord();
	pInfo->WeaponType = pFile->GetWord();		

	pInfo->PhysicAttack = pFile->GetInt();
	pInfo->MagicAttack = pFile->GetInt();

	pInfo->ArmorType = pFile->GetWord();
	pInfo->AccessaryType = pFile->GetWord();

	pInfo->PhysicDefense = pFile->GetInt();
	pInfo->MagicDefense = pFile->GetInt();

	pInfo->IsEnchant		= pFile->GetBool();
	pInfo->Improvement		= pFile->GetWord();

	pInfo->EnchantValue		= pFile->GetDword();
	pInfo->EnchantDeterm	= pFile->GetDword();

	pInfo->ImprovementStr = pFile->GetInt();			
	pInfo->ImprovementDex = pFile->GetInt();			
	pInfo->ImprovementVit = pFile->GetInt();		//10
	pInfo->ImprovementInt = pFile->GetInt();
	pInfo->ImprovementWis = pFile->GetInt();			
	pInfo->ImprovementLife = pFile->GetInt();			
	pInfo->ImprovementMana = pFile->GetInt();			

	pInfo->ImprovementSkill = pFile->GetDword();
	pInfo->Skill = pFile->GetDword();
	pInfo->SupplyType	= pFile->GetWord();			
	pInfo->SupplyValue	= pFile->GetDword();
	pInfo->kind			= ITEM_INFO::eKind( pFile->GetDword() );

	pInfo->RequiredBuff = pFile->GetDword();
	pInfo->Battle = pFile->GetBool();
	pInfo->Peace = pFile->GetBool();
	pInfo->Move = pFile->GetBool();
	pInfo->Stop = pFile->GetBool();
	pInfo->Rest = pFile->GetBool();

	// 071116 LYW --- GameResourceStruct : ����, �ð����� �߰�.
	pInfo->wSeal		= pFile->GetWord() ;
	pInfo->nTimeKind	= pFile->GetInt() ;
	pInfo->dwUseTime	= pFile->GetDword() ;


	// 080426 NYJ --- ���� ����Ʈ �߰�
	pInfo->dwBuyFishPoint	= pFile->GetDword() ;
	pInfo->EquipSlot = pFile->GetWord() - 1;
//	pInfo->Part3DType = pFile->GetWord();
}

void CItemManager::NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg )
{
	MSGBASE * pmsg = (MSGBASE *)pMsg;

	switch(Protocol)
	{
	case MP_ITEM_PET_MOVE_SYN:
		{
			MSG_ITEM_MOVE_SYN*	pmsg	= (MSG_ITEM_MOVE_SYN *)pMsg;
			CPlayer*			pPlayer = ( CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			int					rt		= 0;

			if( ! pPlayer )
			{
				return;
			}

			if( EI_TRUE == MovePetItem(pPlayer, pmsg->wFromItemIdx, pmsg->FromPos, pmsg->wToItemIdx, pmsg->ToPos) )
			{
				MSG_ITEM_MOVE_ACK message;
				memcpy(&message, pMsg, sizeof(MSG_ITEM_MOVE_SYN));
				message.Protocol = MP_ITEM_PET_MOVE_ACK;

				pPlayer->SendMsg( &message, sizeof( message ) );
			}
			else
			{
				MSG_ITEM_ERROR message;
				message.Category	= MP_ITEM;
				message.Protocol	= MP_ITEM_ERROR_NACK;
				message.dwObjectID	= pPlayer->GetID();
				message.ECode		= eItemUseErr_Move;

				pPlayer->SendMsg( &message, sizeof( message ) );
			}
		}
		break;
	case MP_ITEM_MOVE_SYN:
		{
			MSG_ITEM_MOVE_SYN*	pmsg	= (MSG_ITEM_MOVE_SYN *)pMsg;
			CPlayer*			pPlayer = ( CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			int					rt		= 0;
			const ITEM_INFO*	pInfo	= GetItemInfo( pmsg->wFromItemIdx );
			//const ITEM_INFO*	pIcon	= GetItemInfo( pmsg->wIconIdx );

			if( ! pPlayer )
			{
				return;
			}
			else if( ! pInfo )
			{
				MSG_ITEM_ERROR message;
				message.Category	= MP_ITEM;
				message.Protocol	= MP_ITEM_ERROR_NACK;
				message.dwObjectID	= pPlayer->GetID();
				message.ECode		= eItemUseErr_Move;

				pPlayer->SendMsg( &message, sizeof( message ) );
				return;
			}
			/*else if( ! pIcon )
			{
				MSG_ITEM_ERROR message;
				message.Category	= MP_ITEM;
				message.Protocol	= MP_ITEM_ERROR_NACK;
				message.dwObjectID	= pPlayer->GetID();
				message.ECode		= eItemUseErr_Move;

				pPlayer->SendMsg( &message, sizeof( message ) );
				return;
			}*/

			// 080703 LUJ, �̵����� �߰��Ǿ� ����������. �̵��� �����͸� ��ų�� ������ ������ ���� �� ���� �� ���� �� �� �ִ�
			//
			//				- ����� �Ѽհ�/�ܰ˸� �����ϴ�
			//				- ����/���� ���Կ� ���� ������ ���⸦ ����
			//				- ���� ���Կ� �������� �ִ� ��쿡�� ���� ���Կ� ���⸦ ������ �� �ִ�
			switch( pmsg->ToPos )
			{
				// 080703 LUJ, ���� �������� ������ �̵� �� ó��
			case TP_WEAR_START + eWearedItem_Weapon:
				{	
					const ITEMBASE*		rightItem		= GetItemInfoAbsIn( pPlayer, TP_WEAR_START + eWearedItem_Shield );
					const ITEM_INFO*	rightItemInfo	= GetItemInfo( rightItem ? rightItem->wIconIdx : 0 );
					
					// 080703 LUJ, ���⸸ ������ �� �ִ�
					if( ePartType_OneHand != pInfo->Part3DType	&&
						ePartType_TwoHand != pInfo->Part3DType )
					{
						rt = MP_ITEM_ERROR_NACK;
						break;
					}
					// 080703 LUJ, ���� ĭ�� ��������� ��� �������� ���� ����
					else if( ! rightItemInfo )
					{
						break;
					}
					/*else if( rightItem->wIconIdx != pInfo->EquipSlot )
					{
						rt = MP_ITEM_ERROR_NACK;
						break;
					}*/
					// 080703 LUJ, ��� ������ ��� �����տ� �������� ������ ���� �Ұ�
					else if( IsTwoHand( pmsg->wFromItemIdx ) )
					{
						rt = MP_ITEM_ERROR_NACK;
						break;
					}
					// 080703 LUJ, ���а� ������ ��� �Ѽ� ���⸸ ������ �� �ִ�
					else if( ePartType_Shield == rightItemInfo->Part3DType )
					{
						if( ePartType_OneHand != pInfo->Part3DType  )
						{
							rt = MP_ITEM_ERROR_NACK;
						}

						break;
					}
					// 080703 LUJ, �����տ� ���Ⱑ ������ ��� ���� ������ �����ϴ�
					else if( ePartType_OneHand == rightItemInfo->Part3DType )
					{
						if( rightItemInfo->WeaponType != pInfo->WeaponType )
						{
							rt = MP_ITEM_ERROR_NACK;
						}

						break;
					}
					
					break;
				}
				// 080703 LUJ, ���� �������� �̵� �� ó��
			case TP_WEAR_START + eWearedItem_Shield:
				{
					const ITEMBASE*		leftItem			= GetItemInfoAbsIn( pPlayer, TP_WEAR_START + eWearedItem_Weapon );
					const ITEM_INFO*	leftItemInfo		= GetItemInfo( leftItem ? leftItem->wIconIdx : 0 );
					
					// 080703 LUJ, ���� ������ �� ���
					if( ! leftItemInfo )
					{
						// 080703 LUJ, ���а� �ƴ� ��� ���� �Ұ�
						if( ePartType_Shield != pInfo->Part3DType )
						{
							rt = MP_ITEM_ERROR_NACK;
							break;
						}
					}
					// 080703 LUJ, �μչ���� ������ �� ����
					else if(	ePartType_TwoHand == leftItemInfo->Part3DType ||
								ePartType_TwoHand == pInfo->Part3DType //||
								// 090521 ONS �ű����� ���� Ÿ�� �Ӽ� �߰�
							//	ePartType_TwoBlade == pInfo->Part3DType
								)
					{
						rt = MP_ITEM_ERROR_NACK;
						break;
					}
					// 080703 LUJ, ��� ���Ⱑ ���ٸ� ���д� �׻� ���� ����
					else if( ePartType_Shield == pInfo->Part3DType )
					{
						break;
					}
					// 080703 LUJ, �̵��� ���°� �ƴϸ� ����
					else if( ! pPlayer->GetHeroTotalInfo()->bUsingTwoBlade )
					{
						rt = MP_ITEM_ERROR_NACK;
						break;
					}
					// 080703 LUJ, ���⸦ ������ ��� ������ ���� �͸� ���Ǿ�� �Ѵ�
					//				���� �̵��� ����� �������� ������ �ʴ´�
					else if(	pInfo->WeaponType	!= leftItemInfo->WeaponType ||
								eWeaponType_Staff	== leftItemInfo->WeaponType )
					{
						rt = MP_ITEM_ERROR_NACK;
						break;
					}
					// 090910 pdy ���н��� ���������� äũ �߰� 
					// ���⽽��->���н��� �̵��ǰ�� äũ�� ���ؾ� �Ѵ�.
					else if( pmsg->FromPos == TP_WEAR_START + eWearedItem_Weapon )
					{
						const ITEMBASE*		rightItem		= GetItemInfoAbsIn( pPlayer, TP_WEAR_START + eWearedItem_Shield );
						const ITEM_INFO*	rightItemInfo	= GetItemInfo( rightItem ? rightItem->wIconIdx : 0 );

						//���н����� ����ְų� ���н����� ���Ⱑ �������Ⱑ �ƴϸ� �Ұ�
						if( ! rightItemInfo )
						{
							rt = MP_ITEM_ERROR_NACK;
							break;
						}
						else if( leftItemInfo->WeaponType !=  rightItemInfo->WeaponType )
						{
							rt = MP_ITEM_ERROR_NACK;
							break;
						}
					}

					break;
				}
			default:
				{
					// 080703 LUJ, ���� ���Կ��� �������� ���� ��, �̵��� ������ ��� �� �� ����
					//				���� ���Կ��� ���Ⱑ ���� ��� �ִϸ��̼��� �ſ� ����ϴ�
					if( TP_WEAR_START + eWearedItem_Weapon == pmsg->FromPos )
					{
						const ITEMBASE*		item		= GetItemInfoAbsIn( pPlayer, TP_WEAR_START + eWearedItem_Shield );
						const ITEM_INFO*	itemInfo	= ( item ? GetItemInfo( item->wIconIdx ) : 0 );
						
						if( itemInfo &&
							itemInfo->WeaponType == pInfo->WeaponType )
						{
							rt = MP_ITEM_ERROR_NACK;
						}
					}

					break;
				}
			}

			if( MP_ITEM_ERROR_NACK == rt ||
				EI_TRUE != MoveItem(pPlayer, pmsg->wFromItemIdx, pmsg->FromPos, pmsg->wToItemIdx, pmsg->ToPos )  )
			{
				MSG_ITEM_ERROR message;
				message.Category	= MP_ITEM;
				message.Protocol	= MP_ITEM_ERROR_NACK;
				message.dwObjectID	= pPlayer->GetID();
				message.ECode		= eItemUseErr_Move;

				pPlayer->SendMsg( &message, sizeof( message ) );
			}
			else
			{
				MSG_ITEM_MOVE_ACK message;
				ZeroMemory( &message, sizeof( message ) );
				message.Category		= MP_ITEM;
				message.Protocol		= MP_ITEM_MOVE_ACK;
				message.FromPos			= pmsg->FromPos;
				message.ToPos			= pmsg->ToPos;
				message.wFromItemIdx	= pmsg->wFromItemIdx;
				message.wToItemIdx		= pmsg->wToItemIdx;

				pPlayer->SendMsg( &message, sizeof( message ) );
			}
		}
		break;
	//case MP_ITEM_MOVE_SYN:
	//	{
	//		MSG_ITEM_MOVE_SYN*	pmsg	= (MSG_ITEM_MOVE_SYN *)pMsg;
	//		CPlayer*			pPlayer = ( CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	//		int					rt		= 0;
	//		const ITEM_INFO*	pInfo	= GetItemInfo( pmsg->wFromItemIdx );

	//		if( ! pPlayer )
	//		{
	//			return;
	//		}
	//		else if( ! pInfo )
	//		{
	//			MSG_ITEM_ERROR message;
	//			message.Category	= MP_ITEM;
	//			message.Protocol	= MP_ITEM_ERROR_NACK;
	//			message.dwObjectID	= pPlayer->GetID();
	//			message.ECode		= eItemUseErr_Move;

	//			pPlayer->SendMsg( &message, sizeof( message ) );
	//			return;
	//		}

	//		// 080703 LUJ, �̵����� �߰��Ǿ� ����������. �̵��� �����͸� ��ų�� ������ ������ ���� �� ���� �� ���� �� �� �ִ�
	//		//
	//		//				- ����� �Ѽհ�/�ܰ˸� �����ϴ�
	//		//				- ����/���� ���Կ� ���� ������ ���⸦ ����
	//		//				- ���� ���Կ� �������� �ִ� ��쿡�� ���� ���Կ� ���⸦ ������ �� �ִ�
	//		switch( pmsg->ToPos )
	//		{
	//			// 080703 LUJ, ���� �������� ������ �̵� �� ó��
	//		case TP_WEAR_START + eWearedItem_Weapon:
	//			{
	//				const ITEMBASE*		rightItem		= GetItemInfoAbsIn( pPlayer, TP_WEAR_START + eWearedItem_Shield );
	//				const ITEM_INFO*	rightItemInfo	= GetItemInfo( rightItem ? rightItem->wIconIdx : 0 );

	//				// 080703 LUJ, ���⸸ ������ �� �ִ�
	//				if( ePartType_OneHand != pInfo->Part3DType	&&
	//					ePartType_TwoHand != pInfo->Part3DType )
	//				{
	//					rt = MP_ITEM_ERROR_NACK;
	//					break;
	//				}
	//				// 080703 LUJ, ���� ĭ�� ��������� ��� �������� ���� ����
	//				else if( ! rightItemInfo )
	//				{
	//					break;
	//				}
	//				// 080703 LUJ, ��� ������ ��� �����տ� �������� ������ ���� �Ұ�
	//				else if( IsTwoHand( pmsg->wFromItemIdx ) )
	//				{
	//					rt = MP_ITEM_ERROR_NACK;
	//					break;
	//				}
	//				// 080703 LUJ, ���а� ������ ��� �Ѽ� ���⸸ ������ �� �ִ�
	//				else if( ePartType_Shield == rightItemInfo->Part3DType )
	//				{
	//					if( ePartType_OneHand != pInfo->Part3DType  )
	//					{
	//						rt = MP_ITEM_ERROR_NACK;
	//					}

	//					break;
	//				}
	//				// 080703 LUJ, �����տ� ���Ⱑ ������ ��� ���� ������ �����ϴ�
	//				else if( ePartType_OneHand == rightItemInfo->Part3DType )
	//				{
	//					if( rightItemInfo->WeaponType != pInfo->WeaponType )
	//					{
	//						rt = MP_ITEM_ERROR_NACK;
	//					}

	//					break;
	//				}
	//			const ITEMBASE* protectionItem = inventory->GetItemInfoAbs(message->mProtectionItem.Position);

	//			if(protectionItem->wIconIdx != message->mProtectionItem.wIconIdx)
	//				{
	//					return MP_ITEM_ENCHANT_NACK;
	//				}

	//				break;
	//			}
	//			// 080703 LUJ, ���� �������� �̵� �� ó��
	//		case TP_WEAR_START + eWearedItem_Shield:
	//			{
	//				const ITEMBASE*		leftItem			= GetItemInfoAbsIn( pPlayer, TP_WEAR_START + eWearedItem_Weapon );
	//				const ITEM_INFO*	leftItemInfo		= GetItemInfo( leftItem ? leftItem->wIconIdx : 0 );
	//				
	//				// 080703 LUJ, ���� ������ �� ���
	//				if( ! leftItemInfo )
	//				{
	//					// 080703 LUJ, ���а� �ƴ� ��� ���� �Ұ�
	//					if( ePartType_Shield != pInfo->Part3DType )
	//					{
	//						rt = MP_ITEM_ERROR_NACK;
	//						break;
	//					}
	//				}
	//				// 080703 LUJ, �μչ���� ������ �� ����
	//				else if(	ePartType_TwoHand == leftItemInfo->Part3DType ||
	//							ePartType_TwoHand == pInfo->Part3DType )
	//				{
	//					rt = MP_ITEM_ERROR_NACK;
	//					break;
	//				}
	//				// 080703 LUJ, ��� ���Ⱑ ���ٸ� ���д� �׻� ���� ����
	//				else if( ePartType_Shield == pInfo->Part3DType )
	//				{
	//					break;
	//				}
	//				// 080703 LUJ, �̵��� ���°� �ƴϸ� ����
	//				else if( ! pPlayer->GetHeroTotalInfo()->bUsingTwoBlade )
	//				{
	//					rt = MP_ITEM_ERROR_NACK;
	//					break;
	//				}
	//				// 080703 LUJ, ���⸦ ������ ��� ������ ���� �͸� ���Ǿ�� �Ѵ�
	//				//				���� �̵��� ����� �������� ������ �ʴ´�
	//				else if(	pInfo->WeaponType	!= leftItemInfo->WeaponType ||
	//							eWeaponType_Staff	== leftItemInfo->WeaponType )
	//				{
	//					rt = MP_ITEM_ERROR_NACK;
	//					break;
	//				}

	//				break;
	//			}
	//		default:
	//			{
	//				// 080703 LUJ, ���� ���Կ��� �������� ���� ��, �̵��� ������ ��� �� �� ����
	//				//				���� ���Կ��� ���Ⱑ ���� ��� �ִϸ��̼��� �ſ� ����ϴ�
	//				if( TP_WEAR_START + eWearedItem_Weapon == pmsg->FromPos )
	//				{
	//					const ITEMBASE*		item		= GetItemInfoAbsIn( pPlayer, TP_WEAR_START + eWearedItem_Shield );
	//					const ITEM_INFO*	itemInfo	= ( item ? GetItemInfo( item->wIconIdx ) : 0 );
	//					
	//					if( itemInfo &&
	//						itemInfo->WeaponType == pInfo->WeaponType )
	//					{
	//						rt = MP_ITEM_ERROR_NACK;
	//					}
	//				}

	//				break;
	//			}
	//		}

	//		if( MP_ITEM_ERROR_NACK == rt ||
	//			EI_TRUE != MoveItem(pPlayer, pmsg->wFromItemIdx, pmsg->FromPos, pmsg->wToItemIdx, pmsg->ToPos )  )
	//		{
	//			MSG_ITEM_ERROR message;
	//			message.Category	= MP_ITEM;
	//			message.Protocol	= MP_ITEM_ERROR_NACK;
	//			message.dwObjectID	= pPlayer->GetID();
	//			message.ECode		= eItemUseErr_Move;

	//			pPlayer->SendMsg( &message, sizeof( message ) );
	//		}
	//		else
	//		{
	//			MSG_ITEM_MOVE_ACK message;
	//			ZeroMemory( &message, sizeof( message ) );
	//			message.Category		= MP_ITEM;
	//			message.Protocol		= MP_ITEM_MOVE_ACK;
	//			message.FromPos			= pmsg->FromPos;
	//			message.ToPos			= pmsg->ToPos;
	//			message.wFromItemIdx	= pmsg->wFromItemIdx;
	//			message.wToItemIdx		= pmsg->wToItemIdx;

	//			pPlayer->SendMsg( &message, sizeof( message ) );
	//		}
	//	}
	//	break;
	case MP_ITEM_COMBINE_SYN:
		{			
			MSG_ITEM_COMBINE_SYN * pmsg = (MSG_ITEM_COMBINE_SYN *)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL) return;
			int rt = 0;
			if(EI_TRUE == (rt = CombineItem(pPlayer, pmsg->wItemIdx, pmsg->FromPos, pmsg->ToPos, pmsg->FromDur, pmsg->ToDur)))
			{
				MSG_ITEM_COMBINE_ACK msg;
				memcpy(&msg, pmsg, sizeof(MSG_ITEM_COMBINE_SYN));
				msg.Protocol = MP_ITEM_COMBINE_ACK;
				SendAckMsg(pPlayer, &msg, sizeof(msg));
			}
			else
			{
				MSG_ITEM_ERROR msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_ERROR_NACK;
				msg.dwObjectID = pPlayer->GetID();
				msg.ECode = eItemUseErr_Combine;
				SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
			}
		}
		break;
	case MP_ITEM_DIVIDE_SYN:
		{

			MSG_ITEM_DIVIDE_SYN * pmsg = (MSG_ITEM_DIVIDE_SYN *)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL) return;
			int rt = 0;
			if(EI_TRUE == (rt = DivideItem(pPlayer, pmsg->wItemIdx, pmsg->FromPos, pmsg->ToPos, pmsg->FromDur, pmsg->ToDur)))
			{

			}
			else
			{
				MSG_ITEM_ERROR msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_ERROR_NACK;
				msg.dwObjectID = pPlayer->GetID();
				msg.ECode = eItemUseErr_Divide;
				SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
			}

		}
		break;
	case MP_ITEM_DISCARD_SYN:
		{
			MSG_ITEM_DISCARD_SYN * pmsg = (MSG_ITEM_DISCARD_SYN *)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL) return;

			CItemSlot * pSlot = pPlayer->GetSlot(pmsg->TargetPos);
			if(!pSlot) return;
			ITEMBASE Item = *pSlot->GetItemInfoAbs(pmsg->TargetPos);

			int rt = 0;
			if( LOOTINGMGR->IsLootedPlayer( pPlayer->GetID() ) )
			{
				rt = 10;
				MSG_ITEM_ERROR msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_ERROR_NACK;
				msg.dwObjectID = pPlayer->GetID();
				msg.ECode = eItemUseErr_Discard;
				SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
				break;
			}

			if(EI_TRUE == (rt = DiscardItem(pPlayer, pmsg->TargetPos, pmsg->wItemIdx, pmsg->ItemNum)))
			{
				MSG_ITEM_DISCARD_ACK msg;
				memcpy(&msg, pmsg, sizeof(MSG_ITEM_DISCARD_SYN));
				msg.Protocol = MP_ITEM_DISCARD_ACK;
				SendAckMsg(pPlayer, &msg, sizeof(msg));

				// Log
				LogItemMoney(pmsg->dwObjectID, pPlayer->GetObjectName(), 0, "",
					eLog_ItemDiscard, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
					Item.wIconIdx, Item.dwDBIdx,  Item.Position, 0,
					Item.Durability, pPlayer->GetPlayerExpPoint());
			}
			else
			{
				MSG_ITEM_ERROR msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_DISCARD_NACK;
				msg.dwObjectID = pPlayer->GetID();
				msg.ECode = rt;
				SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
			}
		}
		break;
	case MP_ITEM_BUY_SYN:
		{
			MSG_ITEM_BUY_SYN * pmsg = (MSG_ITEM_BUY_SYN *)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL) return;

			// checkhacknpc
			if( CheckHackNpc( pPlayer, pmsg->wDealerIdx ) == FALSE )
			{
				MSG_ITEM_ERROR msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_BUY_NACK;
				msg.dwObjectID = pPlayer->GetID();
				SendErrorMsg( pPlayer, &msg, sizeof(msg), NOT_EXIST );
				return;
			}

			int rt = 0;
			if(EI_TRUE == (rt = BuyItem(pPlayer, pmsg->wBuyItemIdx, pmsg->BuyItemNum, pmsg->wDealerIdx)))
			{
			}
			else
			{
				MSG_ITEM_ERROR msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_BUY_NACK;
				msg.dwObjectID = pPlayer->GetID();
				SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
			}
		}
		break;
	case MP_ITEM_SELL_SYN:
		{
			MSG_ITEM_SELL_SYN * pmsg = (MSG_ITEM_SELL_SYN *)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL) return;

			// checkhacknpc
			if( CheckHackNpc( pPlayer, pmsg->wDealerIdx ) == FALSE )
			{
				MSG_ITEM_ERROR msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SELL_NACK;
				msg.dwObjectID = pPlayer->GetID();
				SendErrorMsg( pPlayer, &msg, sizeof(msg), NOT_EXIST );
				return;
			}

			int rt = 0;
			if(EI_TRUE == (rt = SellItem(pPlayer, pmsg->TargetPos, pmsg->wSellItemIdx, pmsg->SellItemNum, pmsg->wDealerIdx)))
			{
				MSG_ITEM_SELL_ACK msg;
				memcpy(&msg, pmsg, sizeof(MSG_ITEM_SELL_SYN));
				msg.Protocol = MP_ITEM_SELL_ACK; 
				SendAckMsg(pPlayer, &msg, sizeof(msg));
			}
			else
			{
				MSG_ITEM_ERROR msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SELL_NACK;
				msg.dwObjectID = pPlayer->GetID();
				SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
			}

		}
		break;
	case MP_ITEM_USE_SYN:
		{
			MSG_ITEM_USE_SYN * pmsg = (MSG_ITEM_USE_SYN *)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL) return;

			BYTE rt = UseItem(pPlayer, pmsg->TargetPos, pmsg->wItemIdx);
			if(rt == eItemUseSuccess)
			{
				MSG_ITEM_USE_ACK msg;
				memcpy(&msg, pmsg, sizeof(MSG_ITEM_USE_SYN));
				msg.Protocol = MP_ITEM_USE_ACK;
				SendAckMsg(pPlayer, &msg, sizeof(msg));
			}
			else if(rt == eItemUseUnsealed)
			{
				//���������ÿ��� MP_ITEM_USE_ACK��Ŷ�� ������ �ʴ´�.
			}
			// 080609 LYW --- ItemManager : â�� ��ȯ �������� ��� �����ÿ���, ��� ���� �޽����� �������� �ʴ´�.
			// �Ⱓ�� �������̱� ������, ���� �� ������ ����մϴ�~ ��� �޽��� ����� �ʿ� ����.
			else if(rt == eItemUse_OpenStorage_Success) return ;
			else if(rt == eItemUse_OpenStorage_byItem_Failed_Storage_IsZero)
			{
				// â�� ��ȯ ���� �޽����� �����Ѵ�.
				MSG_ITEM_ERROR msg ;
				memset(&msg, 0, sizeof(MSG_ITEM_ERROR)) ;

				msg.Category	= MP_ITEM ;
				msg.Protocol	= MP_ITEM_USE_NACK ;
				msg.dwObjectID	= pPlayer->GetID() ;

				msg.ECode		= eItemUse_OpenStorage_byItem_Failed_Storage_IsZero ;

				pPlayer->SendMsg(&msg, sizeof(msg)) ;
			}
			else
			{
				MSG_ITEM_ERROR msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_USE_NACK;
				msg.dwObjectID = pPlayer->GetID();
				msg.ECode = rt;
				SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
			}
		}
		break;
	case MP_ITEM_MIX_SYN:
		{			
			MSG_ITEM_MIX_SYN2*	message = ( MSG_ITEM_MIX_SYN2* )pMsg;
			CPlayer*			player	= ( CPlayer* )			g_pUserTable->FindUser(message->dwObjectID);

			if( ! player )
			{
				return;
			}

			CItemSlot* slot = player->GetSlot( eItemTable_Inventory );

			if( ! slot )
			{
				return;
			}

			const POSTYPE	inventoryStartPosition	= TP_INVENTORY_START;
			const POSTYPE	inventoryEndPosition	= TP_INVENTORY_END + player->GetInventoryExpansionSize();

			// �κ��丮 ������ �����صξ��ٰ�. ������ �� ����Ѵ�
			CBackupSlot backupSlot( *slot );

			const eLogitemmoney log = Mix( player, *message, &backupSlot );

			switch( log )
			{
			case eLog_ItemMixSuccess:
				{
					MSG_DWORD message;
					message.Category	= MP_ITEM;
					message.Protocol	= MP_ITEM_WORKING_SUCCESS;
					message.dwData		= player->GetID();

					PACKEDDATA_OBJ->QuickSend( player, &message, sizeof( message ) );
					break;
				}
			case eLog_ItemMixFail:
				{
					MSG_DWORD message;
					message.Category	= MP_ITEM;
					message.Protocol	= MP_ITEM_WORKING_FAIL;
					message.dwData		= player->GetID();

					PACKEDDATA_OBJ->QuickSend( player, &message, sizeof( message ) );
					break;
				}
			default:
				{
					backupSlot.Restore();

					{
						MSG_DWORD message;
						message.Category	= MP_ITEM;
						message.Protocol	= MP_ITEM_MIX_NACK;
						message.dwData		= log;

						player->SendMsg( &message, sizeof( message ) );
					}

					// Ÿ�ο��� �ִϸ��̼��� ǥ��
					{
						MSG_DWORD message;
						message.Category	= MP_ITEM;
						message.Protocol	= MP_ITEM_WORKING_FAIL;
						message.dwData	= player->GetID();

						PACKEDDATA_OBJ->QuickSend( player, &message, sizeof( message ) );
					}

					break;
				}
			}
		}
		break;
	case MP_ITEM_ENCHANT_SYN:
		{
			const MSG_ITEM_ENCHANT_SYN* m		= ( MSG_ITEM_ENCHANT_SYN* )pMsg;
			CPlayer*					player	= ( CPlayer* )g_pUserTable->FindUser( m->dwObjectID );

			if( ! player )
			{
				return;
			}

			eLogitemmoney log = eLog_Max;

			switch( Enchant( player, m, log ) )
			{
			case MP_ITEM_ENCHANT_SUCCESS_ACK:
				{
					// ȿ���� ǥ���� �� �ֵ��� ����
					{
						MSG_DWORD message;
						message.Category	= MP_ITEM;
						message.Protocol	= MP_ITEM_WORKING_SUCCESS;
						message.dwData		= player->GetID();

						PACKEDDATA_OBJ->QuickSend( player, &message, sizeof( message ) );
					}

					break;
				}
			case MP_ITEM_ENCHANT_FAILED_ACK:
				{
					{
						MSGBASE message;
						message.Category	= MP_ITEM;
						message.Protocol	= MP_ITEM_ENCHANT_FAILED_ACK;

						player->SendMsg( &message, sizeof( message ) );
					}					

					// ȿ���� ǥ���� �� �ֵ��� ����
					{
						MSG_DWORD message;
						message.Category	= MP_ITEM;
						message.Protocol	= MP_ITEM_WORKING_FAIL;
						message.dwData	= player->GetID();

						PACKEDDATA_OBJ->QuickSend( player, &message, sizeof( message ) );
					}

					break;
				}
			case MP_ITEM_ENCHANT_NACK:
				{
					MSG_DWORD message;
					message.Category	= MP_ITEM;
					message.Protocol	= MP_ITEM_ENCHANT_NACK;
					message.dwData		= log;

					player->SendMsg( &message, sizeof( message ) );
					break;
				}
			}
		}
		break;
	case MP_ITEM_REINFORCE_SYN:
		{
			const MSG_ITEM_REINFORCE_SYN* m = ( MSG_ITEM_REINFORCE_SYN* )pMsg;

			CPlayer* player = (CPlayer *)g_pUserTable->FindUser( m->dwObjectID );

			if( ! player )
			{
				return;
			}

			CItemSlot* slot = player->GetSlot( eItemTable_Inventory );

			if( ! slot )
			{
				return;
			}

			CBackupSlot backupSlot( *slot );

			eLogitemmoney log = Reinforce( player, *m, &backupSlot );

			switch( log )
			{
			case eLog_ItemReinforceSuccess:
				{
					MSG_DWORD message;
					message.Category	= MP_ITEM;
					message.Protocol	= MP_ITEM_WORKING_SUCCESS;
					message.dwData		= player->GetID();

					PACKEDDATA_OBJ->QuickSend( player, &message, sizeof( message ) );
					break;
				}
			case eLog_ItemReinforceFail:
				{
					MSG_DWORD message;
					message.Category	= MP_ITEM;
					message.Protocol	= MP_ITEM_WORKING_FAIL;
					message.dwData		= player->GetID();

					PACKEDDATA_OBJ->QuickSend( player, &message, sizeof( message ) );
					break;
				}
			default:
				{
					backupSlot.Restore();

					MSG_ITEM_ERROR message;
					message.Category	= MP_ITEM;
					message.Protocol	= MP_ITEM_REINFORCE_NACK;
					message.dwObjectID	= player->GetID();
					message.ECode		= log;

					player->SendMsg( &message, sizeof( message ) );
					break;
				}
			}			
		}
		break;
	/*case MP_ITEM_REINFORCE_SYN:
		{
			const MSG_ITEM_REINFORCE_SYN* m = ( MSG_ITEM_REINFORCE_SYN* )pMsg;

			CPlayer* player = (CPlayer *)g_pUserTable->FindUser( m->dwObjectID );

			if( ! player )
			{
				return;
			}

			CItemSlot* slot = player->GetSlot( eItemTable_Inventory );

			if( ! slot )
			{
				return;
			}

			CBackupSlot backupSlot( *slot );

			eLogitemmoney log = Reinforce( player, *m, &backupSlot );

			switch( log )
			{
			case eLog_ItemReinforceSuccess:
				{
					MSG_DWORD message;
					message.Category	= MP_ITEM;
					message.Protocol	= MP_ITEM_WORKING_SUCCESS;
					message.dwData		= player->GetID();

					PACKEDDATA_OBJ->QuickSend( player, &message, sizeof( message ) );
					break;
				}
			case eLog_ItemReinforceFail:
				{
					MSG_DWORD message;
					message.Category	= MP_ITEM;
					message.Protocol	= MP_ITEM_WORKING_FAIL;
					message.dwData		= player->GetID();

					PACKEDDATA_OBJ->QuickSend( player, &message, sizeof( message ) );
					break;
				}
			default:
				{
					backupSlot.Restore();

					MSG_ITEM_ERROR message;
					message.Category	= MP_ITEM;
					message.Protocol	= MP_ITEM_REINFORCE_NACK;
					message.dwObjectID	= player->GetID();
					message.ECode		= log;

					player->SendMsg( &message, sizeof( message ) );
					break;
				}
			}			
		}
		break;*/

	case MP_ITEM_DEALER_SYN:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)
				return;

			// checkhacknpc
			if( CheckHackNpc( pPlayer, pmsg->wData ) == FALSE )
				return;

			// AO��oA
			MSG_WORD msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_DEALER_ACK;
			msg.dwObjectID = pmsg->dwObjectID;
			msg.wData = pmsg->wData;
			pPlayer->SendMsg(&msg,sizeof(msg));

		}
		break;
	case MP_ITEM_GUILD_MOVE_SYN:
		{
			MSG_ITEM_MOVE_SYN * pmsg = (MSG_ITEM_MOVE_SYN *)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL) return;

			if(!CanMovetoGuildWare(pmsg->FromPos, pmsg->ToPos, pPlayer))
			{				
				SendGuildErrorMsg(
					pPlayer,
					MP_ITEM_GUILD_MOVE_NACK,
					4);
				return;
			}
			int rt = 0;
			if(EI_TRUE == (rt = MoveItem(pPlayer, pmsg->wFromItemIdx, pmsg->FromPos, pmsg->wToItemIdx, pmsg->ToPos)))
			{
				//	AddGuildItemOption(pmsg->FromPos, pmsg->ToPos, pPlayer);
				MSG_ITEM_MOVE_ACK msg;
				memcpy(&msg, pmsg, sizeof(MSG_ITEM_MOVE_SYN));
				msg.Protocol = MP_ITEM_GUILD_MOVE_ACK;
				pPlayer->SendMsg(
					&msg,
					sizeof(msg));
			}
			else
			{
				SendGuildErrorMsg(
					pPlayer,
					MP_ITEM_GUILD_MOVE_NACK,
					rt);
			}
		}
		break;
	//case MP_ITEM_GUILD_MOVE_SYN:
	//	{
	//		MSG_ITEM_MOVE_SYN * pmsg = (MSG_ITEM_MOVE_SYN *)pMsg;
	//		CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	//		if(pPlayer == NULL) return;

	//		if(!CanMovetoGuildWare(pmsg->FromPos, pmsg->ToPos, pPlayer))
	//		{				
	//			ASSERT(0); //A������OoAI��u�ϡ̡�����c��?�ˢ硧u���� A�ϩ��ˡ���c C�ϡ��ˡ�A��i��I.
	//			SendGuildErrorMsg(pPlayer, MP_ITEM_GUILD_MOVE_NACK, 4);
	//			return;
	//		}
	//		int rt = 0;
	//		if(EI_TRUE == (rt = MoveItem(pPlayer, pmsg->wFromItemIdx, pmsg->FromPos, pmsg->wToItemIdx, pmsg->ToPos)))
	//		{
	//			//	AddGuildItemOption(pmsg->FromPos, pmsg->ToPos, pPlayer);
	//			MSG_ITEM_MOVE_ACK msg;
	//			memcpy(&msg, pmsg, sizeof(MSG_ITEM_MOVE_SYN));
	//			msg.Protocol = MP_ITEM_GUILD_MOVE_ACK;
	//			SendAckMsg(pPlayer, &msg, sizeof(msg));
	//		}
	//		else
	//		{
	//			SendGuildErrorMsg(pPlayer, MP_ITEM_GUILD_MOVE_NACK, rt);
	//		}
	//	}
	//	break;
	case MP_ITEM_STORAGE_ITEM_INFO_SYN:
		{
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);

			if( !pPlayer )	return;

			if( CheckHackNpc( pPlayer, pmsg->wData1, pmsg->wData2 ) == FALSE )
				return;
			//SW ��ȭ/���� ���� �ް� ���̽� ���� �޴´�. ������ ������?

			if( pPlayer->IsGotWarehouseItems() == TRUE )	//�������� �޴����̰ų� �̹� �޾Ҵ�.
				return;

			pPlayer->SetGotWarehouseItems( TRUE );
			
			CharacterStorageItemInfo( pPlayer->GetID(), pPlayer->GetUserID(), 0 );
		}
		break;
	case MP_ITEM_USE_FOR_QUESTSTART_SYN:
		{
			MSG_ITEM_USE_SYN * pmsg = (MSG_ITEM_USE_SYN *)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL) return;

			BYTE rt = UseItem(pPlayer, pmsg->TargetPos, pmsg->wItemIdx);
			if(rt == eItemUseSuccess)
			{	
				MSG_ITEM_USE_ACK msg;
				memcpy(&msg, pmsg, sizeof(MSG_ITEM_USE_SYN));
				msg.Protocol = MP_ITEM_USE_ACK;
				SendAckMsg(pPlayer, &msg, sizeof(msg));
			}
			else
			{
				MSG_ITEM_ERROR msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_USE_NACK;
				msg.dwObjectID = pPlayer->GetID();
				msg.ECode = eItemUseErr_Quest;
				SendErrorMsg(pPlayer, &msg, sizeof(msg), eItemUseErr_Quest);
			}
		}
		break;

	case MP_ITEM_DISSOLVE_SYN:
		{
			const MSG_ITEM_DISSOLVE_SYN2*	input	= ( MSG_ITEM_DISSOLVE_SYN2* )pMsg;
			CPlayer*						player	= ( CPlayer* )g_pUserTable->FindUser( pmsg->dwObjectID );

			if( ! player )
			{
				return;
			}

			CItemSlot* slot = player->GetSlot( eItemTable_Inventory );

			if( ! slot )
			{
				return;
			}

			CBackupSlot backupSlot( *slot );

			const POSTYPE	inventoryStartPosition	= TP_INVENTORY_START;
			const POSTYPE	inventoryEndPosition	= TP_INVENTORY_END + player->GetInventoryExpansionSize();

			const eLogitemmoney log = Dissolve( player, *input, &backupSlot );

			if( eLog_ItemDissolveSuccess == log )
			{
				return;
			}

			backupSlot.Restore();

			{
				MSG_DWORD message;
				message.Category	= MP_ITEM;
				message.Protocol	= MP_ITEM_DISSOLVE_NACK;
				message.dwData		= log;

				player->SendMsg( &message, sizeof( message ) );
			}
		}
		break;
	case MP_ITEM_USE_CHANGEITEM_SYN:
		{	
			CPlayer* player = ( CPlayer* )g_pUserTable->FindUser( pmsg->dwObjectID );
	
			if( !	player ||
					player->GetObjectKind() != eObjectKind_Player )
			{ 
				break;
			}
	
			const MSG_ITEM_USE_SYN*	message			= ( MSG_ITEM_USE_SYN* )pmsg;
			const DWORD				itemIndex		=			message->wItemIdx;
			const POSTYPE			itemPosition	= POSTYPE(	message->TargetPos );
	
			CItemSlot* slot = player->GetSlot( eItemTable_Inventory );
	
			if( ! slot )
			{	
				break;
			}
	
			const ITEMBASE* itemBase = slot->GetItemInfoAbs( itemPosition );
	
			if( !	itemBase	||
					itemBase->wIconIdx	!= itemIndex )
			{	
				break;
			}
	
			const ITEM_INFO* itemInfo = GetItemInfo( itemIndex );
	
			if( ! itemInfo ||
				! IsChangeItem( itemIndex ) )
			{	
				break;
			}
			// 080820 LUJ, ȹ�� �� ��Ÿ�� �������� ��Ÿ���� ���Ƽ��� ��� �ȵ�
			/*else if( ITEM_KIND_COOLTIME == itemInfo->SupplyType )
			{	
				const DWORD lastCheckedTick = itemBase->ItemParam;
				const BOOL	isCoolTime		= ( int( gCurTime - lastCheckedTick ) < itemBase->nRemainSecond * 1000 );
	
				if( isCoolTime )
				{
					break;
				}
			}*/
	
			// 080820 LUJ, ������ ��ȯ
			if( ! ProcChangeItem( player, itemIndex ) )
			{	
				break;
			}
	
			LogItemMoney(
				player->GetID(),
				player->GetObjectName(),
				0,
				"",
				eLog_ItemDestroybyChangeItem,
				player->GetMoney(),
				0,
				0,
				itemIndex,
				itemBase->dwDBIdx,
				itemPosition,
				0,
				0,
				0 );
	
			DiscardItem( player, itemPosition, itemIndex, 1 );
	
			// 080820 LUJ, ��� ����
			/*{	
				MSG_ITEM_USE_ACK message;
				message.Category	= MP_ITEM;
				message.Protocol	= MP_ITEM_USE_ACK;
				message.TargetPos	= itemPosition;
				message.dwItemIdx	= itemIndex;
				message.eResult		= eItemUseSuccess;
	
				player->SendMsg( &message, sizeof( message ) );
			}*/
		}
		break;
	//case MP_ITEM_USE_CHANGEITEM_SYN:	// event item
	//	{
	//		MSG_ITEM_USE_SYN* pmsg = (MSG_ITEM_USE_SYN*)pMsg;
	//		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	//		if( pPlayer == NULL ) return;

	//		int rt = CHANGEITEMMGR->UseChangeItem( pPlayer, pmsg->TargetPos, pmsg->wItemIdx );
	//		if( rt == 0 )		// not use
	//		{
	//			MSG_ITEM_ERROR msg;
	//			msg.Category = MP_ITEM;
	//			msg.Protocol = MP_ITEM_USE_NACK;
	//			msg.dwObjectID = pPlayer->GetID();
	//			msg.ECode = rt;
	//			SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
	//		}
	//	}
	//	break;
	case MP_ITEM_WORKING_START:
		{
			MSG_DWORD* message = ( MSG_DWORD* )pMsg;

			CObject* object = g_pUserTable->FindUser( message->dwObjectID );

			if( object )
			{
				PACKEDDATA_OBJ->QuickSend( object, message, sizeof( *message ) );
			}
		}
		break;

	case MP_ITEM_WORKING_STARTEX:
		{
			MSG_DWORD2* message = ( MSG_DWORD2* )pMsg;

			CObject* object = g_pUserTable->FindUser( message->dwObjectID );

			if( object )
			{
				PACKEDDATA_OBJ->QuickSend( object, message, sizeof( *message ) );
			}
		}
		break;

		// ������ ���� ó�� ����.
	case MP_ITEM_SHOPITEM_UNSEAL_SYN:
		{
			MSG_DWORD* pmsg = NULL ;
			pmsg = (MSG_DWORD*)pMsg ;

			CPlayer * pPlayer = NULL ;
			pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID) ;

			if( !pPlayer ) return ;

			//MSG_DWORD msg ;
			//msg.Category = MP_ITEM ;
			//msg.dwData = pmsg->dwData ;
			//msg.Protocol = MP_ITEM_SHOPITEM_UNSEAL_ACK ;

			//pPlayer->SendMsg( &msg, sizeof(msg) ) ;

			MSG_DWORD msg ;
			msg.Category = MP_ITEM ;
			msg.dwData = pmsg->dwData ;

			if( ItemUnsealing(pPlayer, (POSTYPE)pmsg->dwData) )
			{
				msg.Protocol = MP_ITEM_SHOPITEM_UNSEAL_ACK ;
			}
			else
			{
				msg.Protocol = MP_ITEM_SHOPITEM_UNSEAL_NACK ;
			}

			pPlayer->SendMsg( &msg, sizeof(msg) ) ;
		}
		break;

	case MP_ITEM_SHOPITEM_USE_SYN:
		{
			MSG_SHOPITEM_USE_SYN * pmsg = (MSG_SHOPITEM_USE_SYN *)pMsg ;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID) ;
			if(pPlayer == NULL) return ;

			SEND_SHOPITEM_BASEINFO msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_SHOPITEM_USE_ACK;
			msg.ShopItemPos = pmsg->UseBaseInfo.TargetItemPos;
			msg.ShopItemIdx = pmsg->UseBaseInfo.TargetItemIdx;
			SendAckMsg(pPlayer, &msg, sizeof(msg));
		}
		break ;

	case MP_ITEM_SHOPITEM_INFO_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);

			if( !pPlayer )
				return;

			//pPlayer->SetShopItemInit( FALSE );
			// 071208 KTH -- ������ �� �������� ����� ��񿡼� �����´�! 
			//			if ( pPlayer->GetShopItemInit() == FALSE )
			CharacterShopItemInfo(pPlayer->GetID(), 0);
			//			else
			//				pPlayer->SendShopItemInfo();
		}
		break;
	// 071201 KTH - ���̵� ��ũ�� ���
	case MP_ITEM_MAPMOVE_SCROLL_SYN:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

			MAPTYPE mapType = (MAPTYPE)pmsg->dwData1;
			POSTYPE pos = (POSTYPE)pmsg->dwData2;
			DWORD itemIdx = pmsg->dwData3;
			ITEM_INFO* pItemInfo = GetItemInfo( itemIdx );

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);

			if(pPlayer == NULL || pItemInfo->SupplyType != ITEM_KIND_MAP_MOVE_SCROLL)
				return;

			//---KES PK Mode 080104
			//---KES AUTONOTE
			if( pPlayer->IsPKMode() || pPlayer->GetAutoNoteIdx() )
			{
				MSG_DWORD msg;
				msg.Category	= MP_ITEM;
				msg.Protocol	= MP_ITEM_MAPMOVE_SCROLL_NACK;
				msg.dwData		= pPlayer->GetGridID() - 1;
				pPlayer->SendMsg( &msg, sizeof( msg ) );
				return;
			}
			//--------------------

			CItemSlot * pSlot = pPlayer->GetSlot(pos);

			if( ! pSlot )
			{
				return;
			}

			const ITEMBASE * pItemBase = pSlot->GetItemInfoAbs(pos);

			if( ! pItemBase || !CHKRT->ItemOf(pPlayer, pos, pItemBase->wIconIdx,0,0,CB_EXIST|CB_ICONIDX) || pItemBase->wIconIdx != itemIdx )
			{
				return;
			}

			const MAPCHANGE_INFO* ChangeInfo = GAMERESRCMNGR->GetMapChangeInfoForDestMapNum( mapType );

			if( CHANNELSYSTEM->GetChannelID( pPlayer->GetChannelID() - 1 ) == 0 && !CHANNELSYSTEM->IsQuestMap() || !ChangeInfo )
			{
				MSG_DWORD msg;
				msg.Category	= MP_ITEM;
				msg.Protocol	= MP_ITEM_MAPMOVE_SCROLL_NACK;
				msg.dwData		= pPlayer->GetGridID() - 1;
				pPlayer->SendMsg( &msg, sizeof( msg ) );
				return;
			}

			const ITEMBASE	previousItem		= *pItemBase;
			const MAPTYPE	previousMapNumber	= pPlayer->GetCurMapNum();

			// 071208 KTH -- ���������� 0�� ��쿡�� Decrease�Ͽ� �ش�. ���� �Ǿ��ִ� �������� ����� �Ұ���.
			if( pItemBase->nSealed != 2 )
			{
				if( pItemBase->nSealed == 1 || UseItem(pPlayer, pos, itemIdx) != eItemUseSuccess )
				{
					MSG_DWORD msg;
					msg.Category	= MP_ITEM;
					msg.Protocol	= MP_ITEM_MAPMOVE_SCROLL_NACK;
					msg.dwData		= pPlayer->GetGridID() - 1;
					pPlayer->SendMsg( &msg, sizeof( msg ) );
					return;
				}
			}

			if( ! Move_ChangeMap(pPlayer, mapType) )
			{
				return;
			}
		}
		break;
	// 071203 KTH -- ���������� ��ų �߰�
	case MP_ITEM_SKILL_SYN:
		{
			MSG_ITEM_SKILL_START_SYN* pmsg = (MSG_ITEM_SKILL_START_SYN*)pMsg;
			MSG_SKILL_START_SYN SkillStartSyn = pmsg->SkillStartSyn;
			POSTYPE				pos = pmsg->Pos;
			DWORD				itemIdx = pmsg->Idx;
			ITEM_INFO* pItemInfo = GetItemInfo( itemIdx );
			CPlayer* pPlayer;
			pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);

			if( pPlayer == NULL )
				return;

			if( pItemInfo == NULL )
				return;

			if(SkillStartSyn.SkillIdx != pItemInfo->SupplyValue )
				return;

			// 071220 LUJ
			ITEMBASE previousItem;
			{
				const ITEMBASE* item = GetItemInfoAbsIn( pPlayer, pos );

				if( item )
				{
					previousItem = *item;
				}
			}

			if( !CHKRT->ItemOf(pPlayer, pos, previousItem.wIconIdx,0,0,CB_EXIST|CB_ICONIDX) )
			{
				return;
			}

			MSG_DWORD errorMessage;
			ZeroMemory( &errorMessage, sizeof( errorMessage ) );
			errorMessage.Category	= MP_ITEM;
			errorMessage.Protocol	= MP_ITEM_USE_NACK;
			errorMessage.dwData		= pPlayer->GetGridID() - 1;

			// 081202 LUJ, �������� �˻簡 �ʿ��� ��� OR�� ����� �ȵ�. OR�� � ������ ������ ������ �����Ϸ��� ������
			if( eItemUseSuccess != UseItem(pPlayer, pos, itemIdx) )
			{
				pPlayer->SendMsg( &errorMessage, sizeof( errorMessage ) );
				return;
			}
			else if( SKILL_SUCCESS != SKILLMGR->OnSkillStartSyn(&SkillStartSyn, &previousItem) )
			{
				pPlayer->SendMsg( &errorMessage, sizeof( errorMessage ) );
				return;
			}

			{
				MSG_ITEM_USE_ACK msg;
				//memcpy(&msg, NULL, sizeof(MSG_ITEM_USE_SYN));
				msg.Category	= MP_ITEM;
				msg.Protocol = MP_ITEM_SKILL_ACK;
				msg.TargetPos = pos;
				msg.wItemIdx = itemIdx;
				SendAckMsg(pPlayer, &msg, sizeof(msg));
			}
		}
		break;
	// �κ� �丮 Ȯ��
	case MP_ITEM_INCREASE_INVENTORY_SYN:
		{
			MSG_ITEM_USE_SYN* pmsg = (MSG_ITEM_USE_SYN*)pMsg;
			POSTYPE				pos = pmsg->TargetPos;
			DWORD				itemIdx = pmsg->wItemIdx;
			ITEM_INFO* pItemInfo = GetItemInfo( itemIdx );
			CPlayer* pPlayer;
			pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);

			if( pPlayer == NULL )
				return;

			ITEMBASE previousItem;
			{
				const ITEMBASE* item = GetItemInfoAbsIn( pPlayer, pos );

				if( item )
				{
					previousItem = *item;
				}
			}			

			if( !CHKRT->ItemOf(pPlayer, pos, previousItem.wIconIdx,0,0,CB_EXIST|CB_ICONIDX) )
			{
				return;
			}

			if( ! pPlayer->IncreaseInventoryExpansion() )
			{
				MSG_DWORD msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_INCREASE_INVENTORY_NACK;
				msg.dwData = 1; // 1�� ���̻� �ø� �������� ���� �ÿ� ������.
				pPlayer->SendMsg(&msg, sizeof(msg));
				return;
			}
			else if( UseItem(pPlayer, pos, itemIdx) != eItemUseSuccess)
			{
				MSG_DWORD msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_INCREASE_INVENTORY_NACK;
				msg.dwData = 0; // 0�� ������ ����� ���� �Ͽ����� ������.
				pPlayer->SendMsg(&msg, sizeof(msg));
				return;
			}
			else
			{
				MSG_ITEM_USE_ACK msg;
				msg.Category	= MP_ITEM;
				msg.Protocol = MP_ITEM_SKILL_ACK;
				msg.TargetPos = pos;
				msg.wItemIdx = itemIdx;
				SendAckMsg(pPlayer, &msg, sizeof(msg));
			}
		}
		break;
	case MP_ITEM_CHANGE_CHARACTER_NAME_SYN:
		{
			MSG_CHANGE_CHARACTER_NAME_SYN* pmsg = (MSG_CHANGE_CHARACTER_NAME_SYN*)pMsg;
			POSTYPE				pos = pmsg->TargetPos;
			DWORD				itemIdx = pmsg->wItemIdx;
			ITEM_INFO* pItemInfo = GetItemInfo( itemIdx );

			CPlayer* pPlayer;
			pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);

			if( pPlayer == NULL )
				return;

			const ITEMBASE* item = GetItemInfoAbsIn( pPlayer, pos );

			// 080124 KTH -- PKMode�϶����� ��� �Ұ�
			if( ! item || !CHKRT->ItemOf(pPlayer, pos, item->wIconIdx,0,0,CB_EXIST|CB_ICONIDX) || pPlayer->IsPKMode() )
			{
				return;
			}
			
			if( pPlayer->GetPartyIdx() != 0 )
			{
				MSG_DWORD msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_CHANGE_CHARACTER_NAME_NACK;
				msg.dwData = 3; // 3�� ��Ƽ�� ������ �����ִ�.
				pPlayer->SendMsg(&msg, sizeof(msg));
				return;
			}
			else if( pPlayer->GetFamilyIdx() != 0 )
			{
				MSG_DWORD msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_CHANGE_CHARACTER_NAME_NACK;
				msg.dwData = 2; // 2�� �йи��� ������ �Ǿ��ִ�.
				pPlayer->SendMsg(&msg, sizeof(msg));
				return;
			}
			else if( pPlayer->GetGuildIdx() != 0 )
			{
				MSG_DWORD msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_CHANGE_CHARACTER_NAME_NACK;
				msg.dwData = 4; // 4�� ��� ������ �Ǿ��ִ�.
				pPlayer->SendMsg(&msg, sizeof(msg));
				return;
			}

			ChangeCharacterName(pmsg->changeName, pmsg->dwObjectID, itemIdx, pos);

		}
		break;
	case MP_ITEM_APPLY_OPTION_SYN:
		{
			const MSG_DWORD2* message = ( MSG_DWORD2* )pMsg;

			CPlayer* player = ( CPlayer* )g_pUserTable->FindUser( message->dwObjectID );

			if( ! player )
			{
				return;
			}

			CItemSlot* slot = player->GetSlot( eItemTable_Inventory );

			if( ! slot )
			{
				return;
			}

            const ITEMBASE* sourceItem	= slot->GetItemInfoAbs( POSTYPE( message->dwData1 ) );
			const ITEMBASE* targetItem	= slot->GetItemInfoAbs( POSTYPE( message->dwData2 ) );

			if( !	sourceItem ||
				!	targetItem  )
			{
				return;
			}

			const ITEMBASE savedSourceItem = *sourceItem;

			ITEM_OPTION option = GetOption( *targetItem );
			{
				option.mItemDbIndex = targetItem->dwDBIdx;

				ZeroMemory( &option.mDrop, sizeof( option.mDrop ) );

				ApplyOption( *sourceItem, *targetItem, option );
			}

			{
				const ITEM_OPTION emptyOption = { 0 };

				if( ! memcmp( &option.mDrop, &emptyOption.mDrop, sizeof( option.mDrop ) ) )
				{
					// ����Ʈ ����
					{
						MSG_DWORD message;
						message.Category	= MP_ITEM;
						message.Protocol	= MP_ITEM_WORKING_FAIL;
						message.dwData		= player->GetID();

						PACKEDDATA_OBJ->QuickSend( player, &message, sizeof( message ) );
					}

					{
						MSGBASE message;
						message.Category	= MP_ITEM;
						message.Protocol	= MP_ITEM_APPLY_OPTION_NACK;

						player->SendMsg( &message, sizeof( message ) );
					}
					
					return;
				}
			}

			if( EI_TRUE != DiscardItem( player, sourceItem->Position, sourceItem->wIconIdx, 1 ) )
			{
				MSGBASE message;
				message.Category	= MP_ITEM;
				message.Protocol	= MP_ITEM_APPLY_OPTION_NACK;

				player->SendMsg( &message, sizeof( message ) );
				return;
			}

			AddOption( option );
			// 081118 LUJ, ĳ���� ��ȣ�� ������ ������ DB �����带 ������ Ű������ ���ǵ��� ��
			ItemDropOptionInsert( option, player->GetID() );
			// 080328 LUJ, ��� �ɼ� �α�
			LogItemDropOption(
				option,
				eLog_ShopItemUse );

			// 080627 LUJ, ����� ������ �α�
			{
				const ITEM_INFO* info = GetItemInfo( savedSourceItem.wIconIdx );

				LogItemMoney(
					player->GetID(),
					player->GetObjectName(),
					0,
					"*applyOption",
					eLog_ItemUse,
					player->GetMoney(),
					0,
					0,
					savedSourceItem.wIconIdx,
					savedSourceItem.dwDBIdx,
					savedSourceItem.Position,
					0,
					info->Stack ? savedSourceItem.Durability - 1 : 0,
					0 );

				// 081027 LUJ, � �ɼǿ� �����ߴ��� ǥ���Ѵ�
				LogItemMoney(
					player->GetID(),
					player->GetObjectName(),
					0,
					"*applyOption",
					eLog_ItemDropOptionAdd,
					player->GetMoney(),
					0,
					0,
					targetItem->wIconIdx,
					targetItem->dwDBIdx,
					targetItem->Position,
					0,
					targetItem->Durability,
					0 );
			}

			{
				MSG_ITEM_OPTION message;
				message.Category		= MP_ITEM;
				message.Protocol		= MP_ITEM_OPTION_ADD_ACK;
				message.mSize			= 1;
				message.mOption[ 0 ]	= option;

				player->SendMsg( &message, message.GetSize() );
			}

			{
				MSG_DWORD message;
				message.Category	= MP_ITEM;
				message.Protocol	= MP_ITEM_APPLY_OPTION_ACK;
				message.dwData		= targetItem->dwDBIdx;

				player->SendMsg( &message, sizeof( message ) );
			}

			// ����Ʈ ����
			{
				MSG_DWORD message;
				message.Category	= MP_ITEM;
				message.Protocol	= MP_ITEM_WORKING_SUCCESS;
				message.dwData		= player->GetID();

				PACKEDDATA_OBJ->QuickSend( player, &message, sizeof( message ) );
			}

			break;
		}
		// 080215 LUJ, ���� �ɼ� ����
	//case MP_ITEM_APPLY_OPTION_SYN:
	//	{
	//		const MSG_DWORD2* message = ( MSG_DWORD2* )pMsg;

	//		CPlayer* player = ( CPlayer* )g_pUserTable->FindUser( message->dwObjectID );

	//		if( ! player )
	//		{
	//			return;
	//		}

	//		CItemSlot* slot = player->GetSlot( eItemTable_Inventory );

	//		if( ! slot )
	//		{
	//			return;
	//		}

 //           const ITEMBASE* sourceItem	= slot->GetItemInfoAbs( POSTYPE( message->dwData1 ) );
	//		const ITEMBASE* targetItem	= slot->GetItemInfoAbs( POSTYPE( message->dwData2 ) );

	//		if( !	sourceItem ||
	//			!	targetItem  )
	//		{
	//			return;
	//		}

	//		const ITEMBASE savedSourceItem = *sourceItem;

	//		ITEM_OPTION option = GetOption( *targetItem );
	//		{
	//			option.mItemDbIndex = targetItem->dwDBIdx;

	//			ZeroMemory( &option.mDrop, sizeof( option.mDrop ) );

	//			ApplyOption( *sourceItem, *targetItem, option );
	//		}

	//		{
	//			const ITEM_OPTION emptyOption = { 0 };

	//			if( ! memcmp( &option.mDrop, &emptyOption.mDrop, sizeof( option.mDrop ) ) )
	//			{
	//				// ����Ʈ ����
	//				{
	//					MSG_DWORD message;
	//					message.Category	= MP_ITEM;
	//					message.Protocol	= MP_ITEM_WORKING_FAIL;
	//					message.dwData		= player->GetID();

	//					PACKEDDATA_OBJ->QuickSend( player, &message, sizeof( message ) );
	//				}

	//				{
	//					MSGBASE message;
	//					message.Category	= MP_ITEM;
	//					message.Protocol	= MP_ITEM_APPLY_OPTION_NACK;

	//					player->SendMsg( &message, sizeof( message ) );
	//				}
	//				
	//				return;
	//			}
	//		}

	//		if( EI_TRUE != DiscardItem( player, sourceItem->Position, sourceItem->wIconIdx, 1 ) )
	//		{
	//			MSGBASE message;
	//			message.Category	= MP_ITEM;
	//			message.Protocol	= MP_ITEM_APPLY_OPTION_NACK;

	//			player->SendMsg( &message, sizeof( message ) );
	//			return;
	//		}

	//		AddOption( option );
	//		// 081118 LUJ, ĳ���� ��ȣ�� ������ ������ DB �����带 ������ Ű������ ���ǵ��� ��
	//		ItemDropOptionInsert( option, player->GetID() );
	//		// 080328 LUJ, ��� �ɼ� �α�
	//		LogItemDropOption(
	//			option,
	//			eLog_ShopItemUse );

	//		// 080627 LUJ, ����� ������ �α�
	//		{
	//			const ITEM_INFO* info = GetItemInfo( savedSourceItem.wIconIdx );

	//			LogItemMoney(
	//				player->GetID(),
	//				player->GetObjectName(),
	//				0,
	//				"*applyOption",
	//				eLog_ItemUse,
	//				player->GetMoney(),
	//				0,
	//				0,
	//				savedSourceItem.wIconIdx,
	//				savedSourceItem.dwDBIdx,
	//				savedSourceItem.Position,
	//				0,
	//				info->Stack ? savedSourceItem.Durability - 1 : 0,
	//				0 );

	//			// 081027 LUJ, � �ɼǿ� �����ߴ��� ǥ���Ѵ�
	//			LogItemMoney(
	//				player->GetID(),
	//				player->GetObjectName(),
	//				0,
	//				"*applyOption",
	//				eLog_ItemDropOptionAdd,
	//				player->GetMoney(),
	//				0,
	//				0,
	//				targetItem->wIconIdx,
	//				targetItem->dwDBIdx,
	//				targetItem->Position,
	//				0,
	//				targetItem->Durability,
	//				0 );
	//		}

	//		{
	//			MSG_ITEM_OPTION message;
	//			message.Category		= MP_ITEM;
	//			message.Protocol		= MP_ITEM_OPTION_ADD_ACK;
	//			message.mSize			= 1;
	//			message.mOption[ 0 ]	= option;

	//			player->SendMsg( &message, message.GetSize() );
	//		}

	//		{
	//			MSG_DWORD message;
	//			message.Category	= MP_ITEM;
	//			message.Protocol	= MP_ITEM_APPLY_OPTION_ACK;
	//			message.dwData		= targetItem->dwDBIdx;

	//			player->SendMsg( &message, sizeof( message ) );
	//		}

	//		// ����Ʈ ����
	//		{
	//			MSG_DWORD message;
	//			message.Category	= MP_ITEM;
	//			message.Protocol	= MP_ITEM_WORKING_SUCCESS;
	//			message.dwData		= player->GetID();

	//			PACKEDDATA_OBJ->QuickSend( player, &message, sizeof( message ) );
	//		}

	//		break;
	//	}
	// 071221 KTH -- ��ġ ��Ｎ
	/*case MP_ITEM_MEMORY_MAP_SYN:
		{
			MSG_USE_MEMORY_STONE_SYN* pmsg = (MSG_USE_MEMORY_STONE_SYN*)pMsg;
			POSTYPE				pos = pmsg->TargetPos;
			DWORD				itemIdx = pmsg->wItemIdx;

			CPlayer* pPlayer;
			pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);

			if( pPlayer == NULL )
				return;

			ITEMBASE previousItem;
			{
				const ITEMBASE* item = GetItemInfoAbsIn( pPlayer, pos );

				if( item )
				{
					previousItem = *item;
				}
			}

			if( pPlayer->m_MemoryStoneInfoMap.find(pmsg->MemoryStoneInfo.byIdx) != pPlayer->m_MemoryStoneInfoMap.end() )
			{
				MSG_DWORD msg;
				msg.Category	=	MP_ITEM;
				msg.Protocol	=	MP_ITEM_MEMORY_MAP_NACK;
				msg.dwData = 1; // 1�� �̹� �ε����� ���� ��. �ߺ� �Ұ�.
				pPlayer->SendMsg(&msg, sizeof(msg));
				return;
			}
			else if( !GAMERESRCMNGR->IsMemorizableMap(pmsg->MemoryStoneInfo.wMapIdx) )	// 071221 KTH -- �����Ҽ� �ִ� ������� üũ
			{
				MSG_DWORD msg;
				msg.Category	=	MP_ITEM;
				msg.Protocol	=	MP_ITEM_MEMORY_MAP_NACK;
				msg.dwData = 2; // 2�� ��� �Ұ����� ���� ���.
				pPlayer->SendMsg(&msg, sizeof(msg));
				return;
			}
			else if( pPlayer->m_MemoryStoneInfoMap.size() > 20 )	// 071221 KTH -- 20�� �̻��� �����Ҽ� ����.
			{
				MSG_DWORD msg;
				msg.Category	=	MP_ITEM;
				msg.Protocol	=	MP_ITEM_MEMORY_MAP_NACK;
				msg.dwData = 3; // 2�� ��� ��Ұ� 20�� �̻��� ���.
				pPlayer->SendMsg(&msg, sizeof(msg));
				return;
			}
			else if( UseItem(pPlayer, pos, itemIdx) != eItemUseSuccess )
			{
				MSG_DWORD msg;
				msg.Category	= MP_ITEM;
				msg.Protocol	= MP_ITEM_SKILL_NACK;
				msg.dwData		= 0;
				pPlayer->SendMsg( &msg, sizeof( msg ) );
				return;
			}

			//MemoryStoneInfoAdd(pPlayer->GetID(), pmsg->MemoryStoneInfo.memoryName, pmsg->MemoryStoneInfo.wMapIdx, pmsg->MemoryStoneInfo.curPos.x, pmsg->MemoryStoneInfo.curPos.z, pmsg->MemoryStoneInfo.byIdx);

			pPlayer->m_MemoryStoneInfoMap.insert(std::pair<BYTE, st_MemoryStoneInfo>(pmsg->MemoryStoneInfo.byIdx, pmsg->MemoryStoneInfo));

			{
				LogItemMoney(
					pPlayer->GetID(),
					pPlayer->GetObjectName(),
					0,
					"",
					eLog_ItemUse,
					pPlayer->GetMoney(),
					0,
					0,
					previousItem.wIconIdx,
					previousItem.dwDBIdx,
					previousItem.Position,
					0,
					previousItem.Durability - 1,
					0 );
			}			
		}
		break;*/

		case MP_ITEM_SHOPITEM_SHOUT_SYN:
		{
			SEND_SHOUTBASE_ITEMINFO* pmsg = (SEND_SHOUTBASE_ITEMINFO*)pMsg;

			POSTYPE itempos = (POSTYPE)pmsg->dwItemPos;
			DWORD itemIdx = pmsg->dwItemIdx;
			ITEM_INFO* pItemInfo = GetItemInfo( itemIdx );

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);

			if(pPlayer == NULL || pItemInfo->SupplyType != ITEM_KIND_WORLDSHOUT)
				return;


			CItemSlot * pSlot = pPlayer->GetSlot(itempos);
			if( ! pSlot )
				return;

			const ITEMBASE * pItemBase = pSlot->GetItemInfoAbs(itempos);

			// ������ ��ȿ�� üũ
			if( ! pItemBase || pItemBase->nSealed == eITEM_TYPE_SEAL || eItemUseSuccess != UseItem(pPlayer, (WORD)pmsg->dwItemPos, pmsg->dwItemIdx))
			{
				MSG_DWORD msg;
				msg.Category	= MP_ITEM;
				msg.Protocol	= MP_ITEM_SHOPITEM_SHOUT_NACK;
				msg.dwData		= pPlayer->GetGridID() - 1;
				pPlayer->SendMsg( &msg, sizeof( msg ) );
				return;
			}

			// ��Ŷ���� �� ��ȯ
			SEND_SHOUTBASE_ITEMINFO msgforServer;
			memcpy(&msgforServer, pmsg, sizeof(SEND_SHOUTBASE_ITEMINFO));
			msgforServer.Category = MP_CHAT;
			msgforServer.Protocol = MP_CHAT_SHOUT_ACK;
			msgforServer.dwItemIdx = (DWORD)-1;	//Item�����ʵ带 ��ȿȭ
			msgforServer.dwItemPos = (DWORD)-1;	//Item�����ʵ带 ��ȿȭ
			
			// ��� Agent������ ����
			g_Network.Broadcast2AgentServer((char*)&msgforServer, msgforServer.GetMsgLength());

			// �������� ����뺸
			MSG_ITEM_USE_ACK msgforUser;
			msgforUser.Category = MP_ITEM;
			msgforUser.Protocol = MP_ITEM_USE_ACK;
			msgforUser.dwObjectID = pmsg->dwObjectID;
			msgforUser.TargetPos = (POSTYPE)pmsg->dwItemPos;
			msgforUser.wItemIdx = pmsg->dwItemIdx;
			SendAckMsg(pPlayer, &msgforUser, sizeof(msgforUser));
		}
		break;
		// 080414 LUJ, �ܾ� ��ȯ
	case MP_ITEM_SHOPITEM_CHARCHANGE_SYN:
		{
			const MSG_CHARACTER_CHANGE* message = ( MSG_CHARACTER_CHANGE* )pMsg;

			// �÷��̾� ���� �˻�
			CPlayer* player = ( CPlayer* )g_pUserTable->FindUser( message->dwObjectID );

			if( ! player )
			{
				break;
			}

			CItemSlot* slot = player->GetSlot( eItemTable_Inventory );

			if( ! slot )
			{
				break;
			}

			const ITEMBASE* item = slot->GetItemInfoAbs( message->mItem.Position );

			// 080414 LUJ, ������ ��ȿ�� �˻�. Ŭ���̾�Ʈ���� ������ ������ ��ġ�ؾ��Ѵ�
			{
				if( !	item ||
						memcmp( item, &message->mItem, sizeof( *item ) ) ||
						mBodyChangeScriptListMap.end() == mBodyChangeScriptListMap.find( item->wIconIdx ) )
				{
					break;
				}
				
				const CHARACTERCHANGE_INFO& info		= message->mInfo;
				const BodyChangeScriptList& scriptList	= mBodyChangeScriptListMap[ item->wIconIdx ];

				BOOL isValidHair = FALSE;
				BOOL isValidFace = FALSE;

				// 080414 LUJ, ����Ʈ�� ��ȯ�ؼ� �÷��̾��� ���� �Ͽ� ���� ���� ������ TRUE�� �����س��´�
				for(
					BodyChangeScriptList::const_iterator it = scriptList.begin();
					scriptList.end() != it;
					++it )
				{
					const BodyChangeScript& script = *it;

					if( script.mGender	!= player->GetGender()	||
						script.mRace	!= player->GetRacial() )
					{
						continue;
					}

					switch( script.mPart )
					{
					case ePartType_Face:
						{
							// 080414 LUJ, �˻縦 ������� ���� ��츸 ��ȿ�� üũ
							if( ! isValidFace )
							{
								isValidFace = ( script.mValue == info.FaceType );
							}

							break;
						}
					case ePartType_Hair:
						{
							// 080414 LUJ, �˻縦 ������� ���� ��츸 ��ȿ�� üũ
							if( ! isValidHair )
							{
                                // 080523 LUJ, ���� �߸� ���ϴ� ���� �Ӹ��� ����
								isValidHair = ( script.mValue == info.HairType );
							}
							
							break;
						}
					}
				}

				// 080414 LUJ, Ŭ���̾�Ʈ���� ������ ���� �������� ����
				if( ! isValidFace ||
					! isValidHair )
				{
					break;
				}
			}

			// 080414 LUJ, DB�� �����Ѵ�. �÷��̾ �����ӵǹǷ� �޸𸮴� �ٽ� �ε��� ���̴�
			if( IsDupItem( item->wIconIdx ) &&
				1 < item->Durability )
			{
				ItemUpdateToDB(
					player->GetID(),
					item->dwDBIdx,
					item->wIconIdx,
					item->Durability - 1,
					item->Position,
					item->QuickPosition );				
			}
			else
			{
				ItemDeleteToDB( item->dwDBIdx );
			}

			// 080414 LUJ, �α�
			LogItemMoney(
				player->GetID(),
				player->GetObjectName(),
				0,
				"*change",
				eLog_ItemUse,
				player->GetMoney(),
				0,
				0,
				item->wIconIdx,
				item->dwDBIdx,
				item->Position,
				0,
				item->Durability,
				0 );

			// 080414 LUJ, ����� �� �÷��̾� ������ ����ǹǷ� �޸𸮸� ������Ʈ���ָ� �ȴ�
			{
				CHARACTER_TOTALINFO info = player->GetCharacterTotalInfo();

				info.HairType	= message->mInfo.HairType;
				info.FaceType	= message->mInfo.FaceType;

				player->InitCharacterTotalInfo( &info );
			}

			// 080414 LUJ, Ŭ���̾�Ʈ�� ����� ������ �ʴ´�. �����ӵ� ���̱� ����
		}
		break;
	case MP_ITEM_GIFT_EVENT_NOTIFY:
		{
			MSG_DWORD4* pmsg = ( MSG_DWORD4* )pMsg;

			DWORD characteridx = pmsg->dwData1;
			DWORD eventidx = pmsg->dwData2;
			DWORD itemidx = pmsg->dwData3;
			DWORD itemcount = pmsg->dwData4;

			CPlayer* pPlayer = ( CPlayer* )g_pUserTable->FindUser( characteridx );

			if( !pPlayer )
			{
				FILE* pFile = fopen( "./log/GiftEventLog.txt", "a+" );
				fprintf( pFile, "%u\tInvalid User(CItemManager::NetworkMsgParse) - character_idx : %u\n", gCurTime, characteridx );
				fclose( pFile );
				return;
			}
			if( !GetItemInfo( itemidx ) )
			{
				FILE* pFile = fopen( "./log/GiftEventLog.txt", "a+" );
				fprintf( pFile, "%u\tInvalid Item(CItemManager::NetworkMsgParse) - item_idx : %u\n", gCurTime, itemidx );
				fclose( pFile );
				return;
			}
			DWORD useridx = pPlayer->GetUserID();
			GiftItemInsert( characteridx, useridx, eventidx, itemidx, itemcount, IsDupItem( itemidx ) );
		}
		break;

	default : break ;
	}
}


BOOL CItemManager::ItemMoveUpdateToDBbyTable(CPlayer* pPlayer, DWORD dwfromDBIdx, DWORD dwfromIconIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos)
{
	if(TP_INVENTORY_START <= frompos && frompos < TP_WEAR_END)
	{
		ItemMoveUpdateToDB(pPlayer->GetID(), dwfromDBIdx, frompos, dwtoDBIdx, topos, pPlayer->GetUserID(), pPlayer->GetGuildIdx());
	}
	else if(TP_GUILDWAREHOUSE_START <= frompos && frompos < TP_GUILDWAREHOUSE_END)
	{
		ItemMoveGuildUpdateToDB(pPlayer->GetID(), pPlayer->GetGuildIdx(), dwfromDBIdx, frompos, dwtoDBIdx, topos);
	}
	else if(TP_STORAGE_START <= frompos && frompos < TP_STORAGE_END)
	{
		ItemMoveStorageUpdateToDB(pPlayer->GetID(), pPlayer->GetUserID(), dwfromDBIdx, frompos, dwtoDBIdx, topos);
	}
/*	else if(TP_SHOPITEM_START <= frompos && frompos < TP_SHOPITEM_END)
	{
		ItemMoveShopUpdateToDB(pPlayer->GetID(), pPlayer->GetUserID(), dwfromDBIdx, frompos, dwtoDBIdx, topos);
	}	*/
	else if(TP_SHOPINVEN_START <= frompos && frompos < TP_SHOPINVEN_END)
	{
		ItemMoveUpdateToDB(pPlayer->GetID(), dwfromDBIdx, frompos, dwtoDBIdx, topos, pPlayer->GetUserID(), pPlayer->GetGuildIdx());
	}
	else
	{
		ASSERT(0);
		return FALSE;
	}
	return TRUE;
}

void CItemManager::ItemUpdatebyTable(CPlayer* pPlayer, DWORD dwDBIdx, DWORD wIconIdx, DURTYPE FromDur, POSTYPE Position, POSTYPE QuickPosition)
{
	if(TP_INVENTORY_START <= Position && Position < TP_WEAR_END)
	{
		ItemUpdateToDB( pPlayer->GetID(), dwDBIdx, wIconIdx, FromDur, Position, QuickPosition );
	}
	else if(TP_SHOPINVEN_START <= Position && Position < TP_SHOPINVEN_END)
	{
		ItemUpdateToDB( pPlayer->GetID(), dwDBIdx, wIconIdx, FromDur, Position, QuickPosition );
	}
	else
	{
		ASSERT(0);
	}
}

BOOL CItemManager::CanMovetoGuildWare(POSTYPE FromPos, POSTYPE ToPos, CPlayer* pPlayer)
{
	//CItemSlot * pFromSlot		= pPlayer->GetSlot(FromPos);
	CItemSlot * pToSlot			= pPlayer->GetSlot(ToPos);

	const DWORD		guildIndex	= pPlayer->GetGuildIdx();
	CGuildScore*	guild		= GUILDMGR->GetGuildScore( guildIndex );

	if( !	guild ||
			guild->GetWarehouseUser() != pPlayer->GetID() )
	{
		return FALSE;
	}

	const CGuildManager::LevelSetting* setting = GUILDMGR->GetLevelSetting( guild->GetLevel() );

	if( setting )
	{
		const DWORD size = TABCELL_GUILDWAREHOUSE_NUM * setting->mWarehouseSize;

		if( TP_GUILDWAREHOUSE_START <= ToPos && ToPos < TP_GUILDWAREHOUSE_START + size )
		{
			return pToSlot->IsEmpty(ToPos);
		}
	}

	return FALSE;
}

void CItemManager::ClearQuickPos(ITEMBASE* pItem, POSTYPE ToPos)
{
	if((TP_GUILDWAREHOUSE_START <= ToPos && ToPos < TP_GUILDWAREHOUSE_END) 
		|| (TP_STORAGE_START <= ToPos && ToPos < TP_STORAGE_END))
	{
		pItem->QuickPosition = 0;
		return;
	}
}
DealerData * CItemManager::GetDealer(WORD wDealerIdx)
{
	return m_DealerTable.GetData(wDealerIdx);
}
void CItemManager::LoadDealerItem()
{
	char buff[2048];
	char mapname[256];
	char npcname[256];
//	DWORD DealerKey;
	BOOL IsItem = FALSE;
	// 070426	LYW --- ItemManager : Modified item index.
	//WORD ItemIdx;
	DWORD ItemIdx;
	
	DealerItem* pItem;
	DealerData* pCurDealer = NULL;
	char seps[]   = "\t\n";
	char* token;
	int count = 0;

	CMHFile fp;
	fp.Init("./System/Resource/DealItem.bin","rb");

	while(1)
	{
		if(fp.IsEOF())
		{
			break;
		}
		count++;
		WORD map_num = fp.GetWord();
		fp.GetString(mapname);
		WORD npc_kind = fp.GetWord();
		fp.GetString(npcname);
		WORD npc_index = fp.GetWord();
		WORD point_x = fp.GetWord();
		WORD point_z = fp.GetWord();
		WORD angle = fp.GetWord();
		BYTE tabnum = fp.GetByte();
		BYTE Pos = 0;
		
		fp.GetLine(buff,2048);

		token = strtok( buff, seps );
		if(token == NULL)
			continue;
		token = strtok( NULL, seps );
		ItemIdx = atoi(token);

		pCurDealer = m_DealerTable.GetData(npc_index);
		if(pCurDealer == NULL)
		{
			pCurDealer = new DealerData;
			m_DealerTable.Add(pCurDealer,npc_index);
		}
		
		if( ItemIdx != 0 )
		{
			pItem = new DealerItem;
			ASSERT(tabnum);
			pItem->Tab = tabnum-1;
			pItem->Pos = Pos++;
			pItem->ItemIdx = ItemIdx;
			pCurDealer->m_DealItemList.AddTail(pItem);
		}
		else
		{
			Pos++;
		}

		while( 1 )
		{
			token = strtok( NULL, seps );
			if(token == NULL)
				break;
			token = strtok( NULL, seps );
			ItemIdx = atoi(token);
			
			if( ItemIdx != 0 )
			{
				pItem = new DealerItem;
				pItem->Tab = tabnum-1;
				pItem->Pos = Pos++;
				pItem->ItemIdx = ItemIdx;
				pCurDealer->m_DealItemList.AddTail(pItem);
			}
			else
			{
				Pos++;
			}
		}
	}
	fp.Release();
}

int CItemManager::ObtainItemFromQuest( CPlayer* pPlayer, DWORD wItemKind, DWORD dwItemNum )
{
	WORD EmptyCellPos[255];
	WORD EmptyCellNum;
	CItemSlot* pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	WORD obtainItemNum = GetCanBuyNumInSpace( pPlayer, pSlot, wItemKind, dwItemNum, EmptyCellPos, EmptyCellNum );
	if( obtainItemNum == 0 )
		return 1;

	return ObtainItemEx( pPlayer, Alloc(pPlayer, MP_QUEST, MP_QUEST_TAKEITEM_ACK,
						 pPlayer->GetID(), 0, eLog_ItemObtainQuest, obtainItemNum, (DBResult)(ObtainItemDBResult)),
						 wItemKind, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum );
	return 0;
}

int CItemManager::ObtainItemFromChangeItem( CPlayer* pPlayer, DWORD wItemKind, WORD wItemNum )
{
	WORD EmptyCellPos[255];
	WORD EmptyCellNum;

	CItemSlot* pSlot = pPlayer->GetSlot(eItemTable_Inventory);

	WORD wSeal = 0;
	ITEM_INFO* pItemInfo = GetItemInfo( wItemKind );
	if( !pItemInfo )		return 2;
/*	if( pItemInfo->ItemKind & eSHOP_ITEM )
	{
		pSlot = pPlayer->GetSlot(eItemTable_ShopInven);
		bSeal = 1;
	}
*/
	
	WORD obtainItemNum = GetCanBuyNumInSpace( pPlayer, pSlot, wItemKind, wItemNum, EmptyCellPos, EmptyCellNum );
	if( obtainItemNum == 0 )
		return 1;

	return ObtainItemEx( pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_USE_CHANGEITEM_ACK,
						 pPlayer->GetID(), 0, eLog_ItemObtainFromChangeItem, obtainItemNum, (DBResult)(ObtainItemDBResult)),
						 wItemKind, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum, wSeal );
}

BOOL CItemManager::CanbeMoved(DWORD wIconIdx,POSTYPE pos, CPlayer* pPlayer)
{
	//if(TP_INVENTORY_START<= pos && pos < TP_INVENTORY_END)
	//	071210 KTH -- Ȯ���κ��丮���� �˻�
	if(TP_INVENTORY_START<= pos && pos < (TP_INVENTORY_END + pPlayer->GetInventoryExpansionSize() ))
	{
		ITEM_INFO * pInfo = GetItemInfo(wIconIdx);
		if(!pInfo) return FALSE;
/*		if( pInfo->ItemKind & eSHOP_ITEM )
			return FALSE;
*/
		return TRUE;
	}
	else if(TP_WEAR_START <= pos && pos < TP_WEAR_END)
	{
		ITEM_INFO * pInfo = GetItemInfo(wIconIdx);
		if(!pInfo) return FALSE;
/*		if( !( pInfo->ItemKind & eEQUIP_ITEM ) )
		{
			if( ( pInfo->ItemKind & eQUEST_ITEM ) )
			{
			}
			else if( ( pInfo->ItemKind == eSHOP_ITEM_EQUIP ) )
			{
			}
			else
				return FALSE;
		}
*/		if(pInfo->EquipSlot == eWearedItem_Earring1)
		{	
			if(pos-TP_WEAR_START != eWearedItem_Earring1 && pos-TP_WEAR_START != eWearedItem_Earring2)
				return FALSE;
		}
		else if(pInfo->EquipSlot == eWearedItem_Ring1)
		{	
			if(pos-TP_WEAR_START != eWearedItem_Ring1 && pos-TP_WEAR_START != eWearedItem_Ring2)
				return FALSE;
		}
		// 080703 LUJ, �̵��� ���°� �ƴ� ��� ���� �Ұ�
		else if( pos-TP_WEAR_START != pInfo->EquipSlot )
		{
			if( ! pPlayer->GetHeroTotalInfo()->bUsingTwoBlade )
				return FALSE;
		}
		
		return CanEquip(pInfo, pPlayer);
		

	}
	
	// 080513 KTH -- â�� Ȯ�� ũ�⸦ ������ �ȱ�� �ִ��� üũ �Ѵ�.
	//else if(TP_STORAGE_START <= pos && pos < TP_STORAGE_END)
	else if( TP_STORAGE_START <= pos && pos < TP_STORAGE_START + (pPlayer->GetStorageNum()*TABCELL_STORAGE_NUM) )//TP_STORAGE_END)
	{
		return TRUE;
	}
	else if(TP_GUILDWAREHOUSE_START <= pos && pos < TP_GUILDWAREHOUSE_END)
		return TRUE;
	else if(TP_SHOPITEM_START <= pos && pos < TP_SHOPITEM_END)
	{
		ITEM_INFO * pInfo = GetItemInfo(wIconIdx);
		if(!pInfo) return FALSE;
/*		if( !(pInfo->ItemKind & eSHOP_ITEM) )
			return FALSE;
*/		return TRUE;
	}
	else if(TP_SHOPINVEN_START <= pos && pos < TP_SHOPINVEN_END)
	{		
		ITEM_INFO * pInfo = GetItemInfo(wIconIdx);
		if(!pInfo) return FALSE;
/*		if( !(pInfo->ItemKind & eSHOP_ITEM) )
			return FALSE;		
*/		return TRUE;
	}
	else
		return FALSE;
	
}

BOOL CItemManager::CanEquip(ITEM_INFO* pInfo, CPlayer* pPlayer)
{
	if(pInfo->LimitLevel > pPlayer->GetLevel())
	{
//		if( pPlayer->GetShopItemStats()->EquipLevelFree )
//		{
//			int gap = pInfo->LimitLevel - pPlayer->GetLevel();
//			gap -= pPlayer->GetShopItemStats()->EquipLevelFree;
//			if( gap > 0 )
//				return FALSE;
//		}
//		else
			return FALSE;
	}
	/*else if(pInfo->EquipType > pPlayer->GetWearedItemIdx())
	{
		ITEM_INFO * pInfo = GetItemInfo(wIconIdx);*/

	// ����U����A.��oIA�ˡ�,��oE�ˡ�����,A��u����A
/*	if(pInfo->LimitStr > pPlayer->GetStrength())
	{
		return FALSE;
	}
	if(pInfo->LimitDex > pPlayer->GetDexterity())
	{
		return FALSE;
	}
	if(pInfo->LimitWis > pPlayer->GetWisdom())
	{
		return FALSE;
	}
	if(pInfo->LimitVit > pPlayer->GetVitality())
	{
		return FALSE;
	}

	// stage
/*	if( pInfo->LimitJob != eItemStage_Normal )
	{
		if(!CHKRT->ItemStageOf(pPlayer, (BYTE)pInfo->LimitJob))
			return FALSE;
	}		
*/
	return TRUE;	
}

void CItemManager::ItemMoveLog(POSTYPE FromPos, POSTYPE ToPos, CPlayer* pPlayer, ITEMBASE* pItem)
{
	DWORD FromID = 0;
	DWORD ToID = 0;
	eLogitemmoney Logtype = eLog_ItemNone;
	MONEYTYPE FromTotalMoney =0;
	MONEYTYPE ToTotalMoney = 0;
	char FromName[MAX_NAME_LENGTH+1];
	char ToName[MAX_NAME_LENGTH+1];

	if(TP_INVENTORY_START<= FromPos && FromPos < TP_WEAR_END)
	{
		FromID = pPlayer->GetID();
		FromTotalMoney = pPlayer->GetMoney();
		
		if(TP_STORAGE_START <= ToPos && ToPos < TP_STORAGE_END)
		{
			ToID = pPlayer->GetUserID();
			Logtype = eLog_ItemMoveInvenToStorage;
			ToTotalMoney = pPlayer->GetMoney(eItemTable_Storage);
			SafeStrCpy(FromName, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);
			SafeStrCpy(ToName, "#Storage", MAX_NAME_LENGTH+1);
		}
		else if(TP_GUILDWAREHOUSE_START <= ToPos && ToPos < TP_GUILDWAREHOUSE_END)
		{
			ToID = pPlayer->GetGuildIdx();
			Logtype = eLog_ItemMoveInvenToGuild;
			SafeStrCpy(FromName, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);
			SafeStrCpy(ToName, "#Munpa", MAX_NAME_LENGTH+1);
		}
		else
		{
			return;
		}
	}
	else if(TP_STORAGE_START <= FromPos && FromPos < TP_STORAGE_END)
	{
		if(TP_INVENTORY_START<= ToPos && ToPos < TP_WEAR_END)
		{
			FromID = pPlayer->GetUserID();
			FromTotalMoney = pPlayer->GetMoney(eItemTable_Storage);
			ToTotalMoney = pPlayer->GetMoney();
			ToID = pPlayer->GetID();
			Logtype = eLog_ItemMoveStorageToInven;
			
			SafeStrCpy(FromName, "#Storage", MAX_NAME_LENGTH+1);
			SafeStrCpy(ToName, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);
		}
//		else if(TP_STORAGE_START<= ToPos && ToPos < TP_STORAGE_END)
//		{
//			FromID = pPlayer->GetUserID();
//			FromTotalMoney = pPlayer->GetMoney(eItemTable_Storage);
//			ToTotalMoney = pPlayer->GetMoney();
//			ToID = pPlayer->GetID();
//			Logtype = eLog_ItemMoveStorageToStorage;

//			SafeStrCpy(FromName, "#Storage", MAX_NAME_LENGTH+1);
//			SafeStrCpy(ToName, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);
//		}
		else
		{
			ASSERT(0); return;			
		}

	}
	else if(TP_GUILDWAREHOUSE_START <= FromPos && FromPos < TP_GUILDWAREHOUSE_END)
	{
		FromID			= pPlayer->GetGuildIdx();
		ToID			= pPlayer->GetID();
		ToTotalMoney	= pPlayer->GetMoney();
		SafeStrCpy(FromName, "#Munpa", MAX_NAME_LENGTH+1);
		SafeStrCpy(ToName, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);

		if(TP_INVENTORY_START<= ToPos && ToPos < TP_WEAR_END)
		{			
			Logtype = eLog_ItemMoveGuildToInven;
		}
//		else if(TP_GUILDWAREHOUSE_START <= ToPos && ToPos < TP_GUILDWAREHOUSE_END)
//		{
//			Logtype = eLog_ItemMoveGuild;
//		}
		else
		{
			ASSERT(0);
			return;
		}
	}
	else if(TP_SHOPITEM_START <= FromPos && FromPos < TP_SHOPITEM_END)
	{
		if(TP_INVENTORY_START<= ToPos && ToPos < TP_WEAR_END)
		{
			FromID			= pPlayer->GetID();
			ToID			= pPlayer->GetID();
			ToTotalMoney	= 0;
			Logtype			= eLog_ShopItemMoveToInven;
			SafeStrCpy( FromName,	"#ShopInven",				sizeof( FromName ) );
			SafeStrCpy( ToName,		pPlayer->GetObjectName(),	sizeof( ToName ) );
		}
		else
		{
			return;
		}		
	}
	else 
		return; 

	// ��� â�� �α״� ���� ��쿡 �����
	if(	eLog_ItemMoveInvenToGuild	== Logtype	||
		eLog_ItemMoveGuildToInven	== Logtype	||
		eLog_ItemMoveGuild			== Logtype)
	{
		// 081201 LUJ, ��� â�� �α� Ÿ���� �˱� ������ ����
		InsertLogGuildWarehouse( pPlayer, FromPos, pItem, 0, Logtype );
	}

	LogItemMoney(FromID, FromName, ToID, ToName,
		Logtype, FromTotalMoney, ToTotalMoney, 0,
			pItem->wIconIdx, pItem->dwDBIdx,  FromPos, ToPos, pItem->Durability, 0 );
}


WORD CItemManager::GetWeaponKind( DWORD wWeapon )
{
	WORD rt = 0;
	ITEM_INFO* pItemInfo = m_ItemInfoList.GetData( wWeapon );
	if( pItemInfo )
		rt = pItemInfo->WeaponType;

	return rt;	
}

void CItemManager::GetItemKindType( DWORD wItemIdx, WORD* wKind, WORD* wType )
{
	ITEM_INFO* pItemInfo = m_ItemInfoList.GetData( wItemIdx );
	if( pItemInfo )
	{
//		*wKind = pItemInfo->ItemKind;
//		*wType = pItemInfo->ItemType;
	}
	else
	{
		*wKind = 0;
		*wType = 0;
	}
}

DWORD CItemManager::MakeNewKey()
{
	if(m_Key == 0)
		++m_Key;
	return m_Key++; 
}


// 071121 LYW --- ItemManager : �� ������ ��� óġ ����.
int CItemManager::UseShopItem( CPlayer* pPlayer, SHOPITEMUSEBASE Info, SHOPITEMBASE* pShopItem )
{
//	if( !pPlayer ) return eItemUseErr_Err ;
//
//	if( !pShopItem ) return eItemUseErr_Err ;
//
//	BYTE byState = eObjectState_None ;
//	byState = pPlayer->GetState() ;
//
//	if(byState == eObjectState_Die) return eItemUseErr_Err ;
//
////	BOOL bResult = TRUE ;
////	bResult = CHKRT->ItemOf(pPlayer, Info.ShopItemPos, Info.ShopItemIdx,0,0,CB_EXIST|CB_ICONIDX) ;
//
////	if( !bResult ) return eItemUseErr_Err ;
//	
//	if(byState == eObjectState_Immortal )
//		OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer, eObjectState_Immortal ) ;
//
//	CItemSlot * pSlot = NULL ;
//	pSlot = pPlayer->GetSlot(Info.ShopItemPos) ;
//
//	if( !pSlot ) return eItemUseErr_Err ;
//
//	if( pSlot->IsLock(Info.ShopItemPos) ) return eItemUseErr_Lock ;
//
//	const ITEMBASE * pItemBase = NULL ;
//	pItemBase = pSlot->GetItemInfoAbs(Info.ShopItemPos) ;
//
////	if( !pItemBase ) return eItemUseErr_Err ;
//
//	ITEM_INFO * pItemInfo = NULL ;
//	pItemInfo = GetItemInfo(Info.ShopItemIdx) ;
//
//	if( !pItemInfo ) return eItemUseErr_Err ;
//
//	if( pItemInfo->bSeal )
//	{
//		return pPlayer->GetChargeItemManager()->UseShopItem( (ITEMBASE*)pItemBase, Info, pShopItem ) ;
//	}
//	else
//	{
//		ASSERT(0);
//		return eItemUseErr_Err ;
//	}

	return eItemUseSuccess ;
}


//int CItemManager::UseShopItem( CPlayer* pPlayer, SHOPITEMUSEBASE Info, SHOPITEMBASE* pShopItem )
//{
//	if(pPlayer->GetState() == eObjectState_Die)
//		return eItemUseErr_Err;
//	if(!CHKRT->ItemOf(pPlayer, Info.ShopItemPos, Info.ShopItemIdx,0,0,CB_EXIST|CB_ICONIDX))
//		return eItemUseErr_Err;
//	
//	if(pPlayer->GetState() == eObjectState_Immortal )
//		OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer, eObjectState_Immortal );
//
//	CItemSlot * pSlot = pPlayer->GetSlot(Info.ShopItemPos);
//	if( pSlot && pSlot->IsLock(Info.ShopItemPos) )
//		return eItemUseErr_Lock;
//	const ITEMBASE * pItemBase = pSlot->GetItemInfoAbs(Info.ShopItemPos);
//	ITEM_INFO * pItemInfo = GetItemInfo(Info.ShopItemIdx);
//
//	// RaMa - 04.11.04
///*	if( pItemInfo && pItemInfo->ItemKind & eSHOP_ITEM )
//	{
//		return pPlayer->GetShopItemManager()->UseShopItem( (ITEMBASE*)pItemBase, Info, pShopItem );
//	}
//	else
//*/	{
//		ASSERT(0);
//		return eItemUseErr_Err;
//	}
//
//	return eItemUseSuccess;
//}


BOOL CItemManager::IsUseAbleShopItem( CPlayer* pPlayer, DWORD ItemIdx, POSTYPE ItemPos )
{
	if(pPlayer->GetState() == eObjectState_Die)
		return FALSE;
	if(!CHKRT->ItemOf(pPlayer, ItemPos, ItemIdx,0,0,CB_EXIST|CB_ICONIDX))
		return FALSE;
	
	if(pPlayer->GetState() == eObjectState_Immortal )
		OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer, eObjectState_Immortal );

	CItemSlot * pSlot = pPlayer->GetSlot(ItemPos);
	if( pSlot && pSlot->IsLock(ItemPos) )
		return FALSE;

	const ITEMBASE* pItemBase = pSlot->GetItemInfoAbs( ItemPos );
	if(!pItemBase)		return FALSE;
	if(pItemBase->wIconIdx != ItemIdx)
		return FALSE;
	
	ITEM_INFO * pItemInfo = GetItemInfo(ItemIdx);
/*	if( pItemInfo && ( pItemInfo->ItemKind & eSHOP_ITEM ) )
	{
		return TRUE;
	}
*/
	return FALSE;
}


void CItemManager::ShopItemUseUpgrade( ITEM_INFO* pShopItemInfo, CPlayer* pPlayer, POSTYPE ItemPos, DWORD wItemIdx)
{
	if(!CHKRT->ItemOf(pPlayer, ItemPos, wItemIdx,0,0,CB_EXIST|CB_ICONIDX))
			return;
	const ITEMBASE* pSrcItem = GetItemInfoAbsIn( pPlayer, ItemPos );
	if(!pSrcItem)		return;

	DWORD tdw = pSrcItem->wIconIdx%10;
	if(tdw >= 9)
	{
		// ���̻� ��ȭ�� �� ����
		
		return;
	}

	DWORD plusoption = 0;
	// ������ Ȯ���� �����ϰ�,

	if( (plusoption+tdw) > 9 )
		plusoption = 1;

	wItemIdx = (WORD)(pSrcItem->wIconIdx + plusoption);


	// ���ο� �������� �ִ´�.
	CItemSlot * pSlot = pPlayer->GetSlot(ItemPos);
	WORD flag = UB_ICONIDX;		
	if( !IsDupItem( (WORD)(pSrcItem->wIconIdx+plusoption) ) )
		flag |= UB_DURA;

	if(EI_TRUE != pSlot->UpdateItemAbs( pPlayer, ItemPos, 0, (WORD)(pSrcItem->wIconIdx+plusoption),0,0,0, flag, SS_LOCKOMIT))
		return;

	const ITEMBASE * pItemBase = pSlot->GetItemInfoAbs(ItemPos);
	ItemUpdateToDB(pPlayer->GetID(), pItemBase->dwDBIdx, pItemBase->wIconIdx,
				pItemBase->Durability, pItemBase->Position, pItemBase->QuickPosition);

	LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ShopItemUse_MixSuccess, 
		pPlayer->GetMoney(), pSrcItem->wIconIdx+plusoption, pPlayer->GetMoney(), pItemBase->wIconIdx, pItemBase->dwDBIdx, pItemBase->Position, pItemBase->Position, pItemBase->Durability,
		pPlayer->GetPlayerExpPoint());		

		/*
	// ���� �̵� �ֹ���
	else if( pShopItemInfo->ItemGrade == 3 )
	{
		// dwParam -> �ʳѹ�
	}
	// ��ȯ �ֹ���
	else if( pShopItemInfo->ItemGrade == 4 )
	{
		// dwParam -> �ʳѹ�
	}
	// ���� �ڷ���Ʈ �ֹ���
	else if( pShopItemInfo->ItemGrade == 5 )
	{
		// dwParam -> �ʳѹ�
	}
	// ���� �ڷ���Ʈ �ֹ���
	else if( pShopItemInfo->ItemGrade == 6 )
	{
		// dwParam -> �ʳѹ�
	}
	// ���� �ູ
	else if( pShopItemInfo->ItemGrade == 8 )
	{
	}
	// ������ �ູ
	else if( pShopItemInfo->ItemGrade == 9 )
	{
	}
	// ���� Ȯ�� ��� �ֹ���
	else if( pShopItemInfo->ItemGrade == 10 )
	{
	}
	// ��Ȱ�ֹ���
	else if( pShopItemInfo->ItemGrade == 11 )
	{
		// dwParam -> ��Ȱ ��Ű���� ������ Idx
	}
	*/
}


// RaMa
BOOL CItemManager::ItemUnsealing(CPlayer* pPlayer, POSTYPE absPos)
{
	//ITEMBASE* pItemBase = NULL ;
	//pItemBase = (ITEMBASE*)GetItemInfoAbsIn(pPlayer, absPos) ;

	//if(!pItemBase) return FALSE ;

	//CItemSlot* pItemSlot = NULL ;
	//pItemSlot = pPlayer->GetSlot( absPos ) ; 	

	//if( !pItemSlot ) return FALSE ;

	//if( pItemSlot->IsLock( absPos ) ) return FALSE ;
	//
	////ITEMBASE ItemBase = *pItemBase;	
	//if( pItemBase->ItemParam !=  eITEM_PARAM_SEAL ) return FALSE ;

	//ITEM_INFO* pItemInfo = NULL ;
	//pItemInfo = GetItemInfo( pItemBase->wIconIdx ) ;

	//if( !pItemInfo ) return FALSE ;
	//
	//ITEMBASE ItemBase ;
	//memcpy( &ItemBase, pItemBase, sizeof(ITEMBASE) ) ;

	//SYSTEMTIME systime ;
	//stTIME startime, usetime, endtime ;
	//GetLocalTime(&systime) ;
	//startime.SetTime
	//(
	//	systime.wYear-2000
	//,	systime.wMonth
	//,	systime.wDay
	//,	systime.wHour
	//,	systime.wMinute
	//,	0
	//) ;
	//
	//endtime = startime ;

	//DWORD dwRemainMinute = pItemInfo->nUseTime ;

	//DWORD day = dwRemainMinute/(24*60) ;
	//DWORD hour = (dwRemainMinute%(24*60))/60 ;
	//DWORD minute = (dwRemainMinute%(24*60))%60 ;
	//usetime.SetTime(0, 0, day, hour, minute, 0) ;
	//endtime += usetime ;
	//
	//ItemBase.dwRemainMinute = endtime.value ;
	//ItemBase.ItemParam = eITEM_PARAM_UNSEAL ;

	////
	//UsingShopItemUpdateToDB
	//( 
	//	0
	//,	pPlayer->GetID()
	//,	ItemBase.wIconIdx
	//,	ItemBase.dwDBIdx
	//,	pItemInfo->SellPrice
	//,	0
	//,	ItemBase.dwRemainMinute
	//) ;
	//
	//pPlayer->GetChargeItemManager()->AddShopItemUse( &ItemBase ) ;

	//SEND_SHOPITEM_USEDINFO msg ;
	//msg.Category = MP_ITEM ;
	//msg.Protocol = MP_ITEM_SHOPITEM_USEDINFO ;
	//msg.ItemCount = 1 ;
	//msg.Item[0] = ItemBase ;					
	//pPlayer->SendMsg( &msg, msg.GetSize() ) ;
	//
	//LogItemMoney
	//(
	//	pPlayer->GetID()
	//,	pPlayer->GetObjectName()
	//,	0
	//,	""
	//,	eLog_ShopItemUse
	//,	pPlayer->GetMoney(eItemTable_Inventory)
	//,	0
	//,	0
	//,	ItemBase.wIconIdx
	//,	ItemBase.dwDBIdx
	//,	ItemBase.Position
	//,	0
	//,	ItemBase.Durability
	//,	pPlayer->GetPlayerExpPoint()
	//) ;	

	//pItemBase->ItemParam = eITEM_PARAM_UNSEAL ;			

	//ShopItemParamUpdateToDB
	//(
	//	pPlayer->GetID()
	//,	pItemBase->dwDBIdx
	//,	pItemBase->ItemParam
	//) ;			

	return TRUE ;
}
BOOL CItemManager::CheckHackNpc( CPlayer* pPlayer, WORD wNpcIdx, WORD wParam )
{
	if( wParam == eNpcParam_ShowpStorage )
		return TRUE;


	STATIC_NPCINFO* pInfo = GAMERESRCMNGR->GetStaticNpcInfo( wNpcIdx );
	if( pInfo == NULL )	return FALSE;

	VECTOR3 vPos;
	pPlayer->GetPosition( &vPos );
	float dist = CalcDistanceXZ( &vPos, &pInfo->vPos );
	if( dist > 3000.0f )	return FALSE;

	return TRUE;
}


int CItemManager::CheatObtainShopItem( CPlayer* pPlayer, DWORD obtainItemIdx, DWORD ItemNum )
{
	WORD EmptyCellPos[255];
	WORD EmptyCellNum;
	CItemSlot * pSlot = pPlayer->GetSlot(eItemTable_ShopInven);
	WORD obtainItemNum = GetCanBuyNumInSpace( pPlayer, pSlot, obtainItemIdx, ItemNum, EmptyCellPos, EmptyCellNum );
	if(obtainItemNum == 0)
		return 1;

	return ObtainItemEx( pPlayer, Alloc(pPlayer, MP_CHEAT, MP_CHEAT_ITEM_ACK, pPlayer->GetID(), 0, eLog_ItemObtainCheat, obtainItemNum, (DBResult)(ObtainItemDBResult)), obtainItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum, 1 );
}

BOOL CItemManager::CheckHackItemMove( CPlayer* pPlayer, CItemSlot* pFromSlot, CItemSlot* pToSlot )
{
	//	if( pPlayer->GetShopItemManager()->GetUsingItemInfo( eIncantation_ShowStorage )			||
	//		pPlayer->GetShopItemManager()->GetUsingItemInfo( eIncantation_ShowStorage7 )		||
	//		pPlayer->GetShopItemManager()->GetUsingItemInfo( eIncantation_ShowStorage7_NoTrade ) )
	//	{
	//		return TRUE;
	//	}

	// 070808 ����, ������϶��� üũ���� �ʵ��� �Ѵ�. ���߿� ������
#ifndef _DEBUG
	VECTOR3 vPos;
	pPlayer->GetPosition( &vPos );

	CItemSlot* pGuildSlot = pPlayer->GetSlot( eItemTable_MunpaWarehouse );
	CItemSlot* pWareSlot = pPlayer->GetSlot( eItemTable_Storage );

	if( pFromSlot == pGuildSlot || pToSlot == pGuildSlot || pFromSlot == pWareSlot || pToSlot == pWareSlot )
	{
		if( !	g_pServerSystem->GetMap()->IsVillage() ||
			!	GAMERESRCMNGR->IsInWareNpcPosition( &vPos ) )
		{
			return FALSE;
		}
	}
#endif

	return TRUE;
}


int CItemManager::GetSetItemSize( CPlayer* player, const SetScript* script )
{
	if( ! script || ! player )
	{
		ASSERT( 0 );
		return 0;
	}

	int size = 0;
	DWORD previousIdx = (DWORD) 0;

	for( POSTYPE part = TP_WEAR_START; part < TP_WEAR_END; ++part )
	{
		const ITEMBASE * itemBase = GetItemInfoAbsIn(player, part );

		
		if( itemBase &&
			itemBase->dwDBIdx )
		{
			const ITEM_INFO * itemInfo = GetItemInfo( itemBase->wIconIdx );

			if( itemInfo &&
				script->mItem.end() != script->mItem.find( itemInfo->ItemIdx ) )
			{
				if(previousIdx != (DWORD)itemInfo->ItemIdx)
				{
					++size;
					previousIdx = (DWORD)itemInfo->ItemIdx;

				}
			}
		}
	}
	return size;
}


const SetScript* CItemManager::GetSetScript( DWORD itemIndex ) const
{
	SetScriptMap::const_iterator it = mSetScriptMap.find( itemIndex );

	return mSetScriptMap.end() == it ? 0 : it->second;
}


void CItemManager::LoadSetScript()
{
	SetItemName setName;

	// ��Ʈ �̸��� �ε�����
	{
		CMHFile file;

		if( file.Init( "data/interface/windows/setItemName.bin", "rb" ) )
		{
			for(	TCHAR line[ MAX_PATH ];
					! file.IsEOF();
					ZeroMemory( line, sizeof( line ) ) )
			{
				file.GetLine( line, sizeof( line ) / sizeof( TCHAR ) );

				const TCHAR*	separator	= _T( " \"\t\n" );
				const TCHAR*	token		= _tcstok( line, separator );
				const DWORD		index		= _tcstoul( token, 0, 10 );

				if( setName.end() != setName.find( index ) )
				{
					TCHAR buffer[ 80 ];
					_stprintf( buffer, "%d���� �ߺ��Ǿ����ϴ�. ���߿� ���� ���� ��ȣ�� �̸��� ���õ˴ϴ�", index );

					ASSERT( 0 && buffer );
					continue;
				}

				std::string &	name		= setName[ index ];
				const TCHAR*	position = line;

				while( token = _tcstok( 0, separator ) )
				{
					name		+= token;
					position	+= _tcslen( token );

					if( 0 == *( position + 1 ) )
					{
						break;
					}

					name += ' ';
				}
			}
		}
	}

	CMHFile file;

	if( ! file.Init( "system/resource/setItem.bin", "rb" ) )
	{
		ASSERT( 0 && "��Ʈ ������ ��ũ��Ʈ�� �����ϴ�: setItem.bin" );
		return;
	}

	std::list< std::string >	text;

	SetScript*		script		= 0;
	int				openCount	= 0;

	for(	TCHAR line[ MAX_PATH ];
			! file.IsEOF();
			ZeroMemory( line, sizeof( line ) ) )
	{
		file.GetLine( line, sizeof( line ) / sizeof( TCHAR ) );

		const int length = _tcslen( line );

		if( 0 == length )
		{
			continue;
		}

		// �߰��� �ּ� ����
		{
			for( int i = 0; i < length - 1; ++i )
			{
				const char a = line[ i ];
				const char b = line[ i + 1 ];

				if( '/' == a &&
					'/' == b )
				{
					line[ i ] = 0;
					break;
				}
			}
		}

		text.push_back( line );

		const TCHAR* separator	= _T( " \n\t=,+\"()" );
		const TCHAR* token		= _tcstok( line, separator );
		const TCHAR* markBegin	= _T( "{" );
		const TCHAR* markEnd	= _T( "}" );

		if( 0 == token )
		{
			text.pop_back();
		}		
		else if( 0 == _tcsicmp( token, "set" ) )
		{
			if( script )
			{
				ASSERT( 0 && "��Ʈ ��ũ��Ʈ�� �ùٷ� �Ľ̵��� ���߽��ϴ�" );
				return;
			}

			mSetScript.push_back( SetScript() );

			script = &( mSetScript.back() );

			while( token = _tcstok( 0, separator ) )
			{
				const int index = _ttoi( token );

				script->mItem.insert( index );

				if( mSetScriptMap.end() != mSetScriptMap.find( index ) )
				{
					_stprintf( line, "������ �ε����� �ߺ��Ǿ����ϴ�. %s", text.back().c_str() );
					ASSERT( 0 && line );
					return;
				}

				mSetScriptMap[ index ] = script;
			}

			text.pop_back();
		}
		else if( 0 == _tcsnicmp( markBegin, token, strlen( markBegin ) ) )
		{
			++openCount;
		}
		else if( 0 == _tcsnicmp( markEnd, token, strlen( markEnd ) ) )
		{
			if( 0 == --openCount )
			{
				if( ! script )
				{
					ASSERT( 0 && "��Ʈ ��ũ��Ʈ���� ��ȣ�� ���� �ʴ� ������ �߰ߵǾ����ϴ�" );
					return;
				}

				ParseSetScript( text, *script, setName );

				// ������ �˻�
				{
					if( script->mName.empty() )
					{
						ASSERT( 0 && "��Ʈ ��ũ��Ʈ���� ��Ʈ �̸��� �������� �ʾҽ��ϴ�" );
					}

					if( script->mAbility.empty() )
					{
						ASSERT( 0 && "��Ʈ ��ũ��Ʈ���� �ο��� �ɷ��� ���� ��Ʈ�� �ֽ��ϴ�" );
					}

					if( script->mItem.empty() )
					{
						ASSERT( 0 && "��Ʈ ��ũ��Ʈ���� ������ �ε��� ���� ������ ��Ʈ�� �ֽ��ϴ�" );
					}
				}

				script = 0;
				text.clear();
			}
		}
	}

	if( openCount )
	{
		ASSERT( 0 && "��Ʈ ��ũ��Ʈ�� {} ����� �ùٷ� ������ �ʾҽ��ϴ�" );
	}
}


void CItemManager::ParseSetScript(std::list< std::string >& text, SetScript& data, const SetItemName& name )
{
	int					openCount	= 0;
	SetScript::Element*	element		= 0;
	TCHAR				line[ MAX_PATH ];

	for(	std::list< std::string >::const_iterator it = text.begin();
		text.end() != it;
		++it )
	{
		_tcscpy( line, it->c_str() );

		const TCHAR* separator	= _T( " \n\t=,+\"()" );
		const TCHAR* token		= _tcstok( line, separator );

		// �̸� �Ľ�
		if( 0 == token )
		{
			continue;
		}

		const TCHAR* markBegin	= _T( "{" );
		const TCHAR* markEnd	= _T( "}" );

		if( 0 == _tcsicmp( "name", token ) )
		{
			token					= _tcstok( 0, separator );
			const DWORD nameIndex	= _tcstoul( token, 0, 10 );

			SetItemName::const_iterator name_it = name.find( nameIndex );

			if( name.end() != name_it )
			{
				data.mName = name_it->second;
			}
		}
		// ������ �ɷ� �Ľ�
		else if( 0 == _tcsicmp( "size", token ) )
		{
			const size_t size = _tcstoul( _tcstok( 0, separator ), 0, 10 );

			if( element ) 
			{
				ASSERT( 0 && "��Ʈ ��ũ��Ʈ�� ����� �ùٷ� ������ �ʾҽ��ϴ�" );
				return;
			}
			else if( data.mItem.size() < size )
			{
				_stprintf( line, "��Ʈ�� ������ ������ �������� �� ���� size�� �����Ǿ����ϴ�. %s", it->c_str() );
				ASSERT( 0 && line );
				continue;
			}

			element = &( data.mAbility[ size ] );

			{
				PlayerStat& stat = element->mStat;
				ZeroMemory( &stat, sizeof( stat ) );
			}
		}
		else if( 0 == _tcsnicmp( markBegin, token, strlen( markBegin ) ) )
		{
			++openCount;
		}
		else if( 0 == _tcsnicmp( markEnd, token, strlen( markEnd ) ) )
		{
			--openCount;

			element = 0;
		}
		else if( element )
		{
			PlayerStat& stat = element->mStat;

			if( 0 == _tcsicmp( token, "physic_attack" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mPhysicAttack.mPercent = value / 100.0f;
				}
				else
				{
					stat.mPhysicAttack.mPlus	= value;
				}
			}
			else if( 0 == _tcsicmp( token, "magic_attack" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mMagicAttack.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mMagicAttack.mPlus		= value;
				}
			}
			else if( 0 == _tcsicmp( token, "physic_defense" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mPhysicDefense.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mPhysicDefense.mPlus		= value;
				}
			}
			else if( 0 == _tcsicmp( token, "magic_defense" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mMagicDefense.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mMagicDefense.mPlus	= value;
				}
			}
			else if( 0 == _tcsicmp( token, "strength" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mStrength.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mStrength.mPlus	= value;
				}
			}
			else if( 0 == _tcsicmp( token, "dexterity" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mDexterity.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mDexterity.mPlus		= value;
				}
			}
			else if( 0 == _tcsicmp( token, "vitality" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mVitality.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mVitality.mPlus	= value;
				}
			}
			else if( 0 == _tcsicmp( token, "intelligence" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mIntelligence.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mIntelligence.mPlus	= value;
				}
			}
			else if( 0 == _tcsicmp( token, "wisdom" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mWisdom.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mWisdom.mPlus	= value;
				}
			}
			else if( 0 == _tcsicmp( token, "life" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mLife.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mLife.mPlus	= value;
				}
			}
			else if( 0 == _tcsicmp( token, "mana" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mMana.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mMana.mPlus	= value;
				}
			}
			else if( 0 == _tcsicmp( token, "recovery_life" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mRecoveryLife.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mRecoveryLife.mPlus	= value;
				}
			}
			else if( 0 == _tcsicmp( token, "recovery_mana" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mRecoveryMana.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mRecoveryMana.mPlus	= value;
				}
			}
			else if( 0 == _tcsicmp( token, "accuracy" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mAccuracy.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mAccuracy.mPlus	= value;
				}
			}
			else if( 0 == _tcsicmp( token, "evade" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mEvade.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mEvade.mPlus		= value;
				}
			}
			else if( 0 == _tcsicmp( token, "critical_rate" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mCriticalRate.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mCriticalRate.mPlus	= value;
				}
			}
			else if( 0 == _tcsicmp( token, "critical_damage" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mCriticalDamage.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mCriticalDamage.mPlus		= value;
				}
			}
			else if( 0 == _tcsicmp( token, "move_speed" ) )
			{
				const float value = float( _tstof( token = _tcstok( 0, separator ) ) );

				if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
				{
					stat.mMoveSpeed.mPercent	= value / 100.0f;
				}
				else
				{
					stat.mMoveSpeed.mPlus		= value;
				}
			}
			else if( 0 == _tcsicmp( token, "skill" ) )
			{
				while( token = _tcstok( 0, separator ) )
				{
					const DWORD index	= _tcstoul( token, 0, 10 );
					const BYTE level	= BYTE( _ttoi( _tcstok( 0, separator ) ) );

					if( element->mSkill.end() != element->mSkill.find( index ) )
					{
						_stprintf( line, "��Ʈ ��ũ��Ʈ���� ��ų�� �ߺ��Ǿ����ϴ�. %s", it->c_str() );
						ASSERT( 0 && line );
					}

					element->mSkill[ index ] = level;
				}
			}
			else
			{
				_stprintf( line, "��Ʈ ��ũ��Ʈ���� ���� �ʴ� Ű���尡 �߰ߵǾ����ϴ�. %s", it->c_str() );
				ASSERT( 0 && line );
			}
		}
	}

	if( openCount || element )
	{
		ASSERT( 0 && "��Ʈ ��ũ��Ʈ�� {} ����� �ùٷ� ������ �ʾҽ��ϴ�" );
	}
}


void CItemManager::LoadMixSetting()
{
	CMHFile file;

	if( ! file.Init( "System/Resource/mix_setting.bin", "rb" ) )
	{
		ASSERT( 0 );
		return;
	}

	std::list< std::string >	text;

	int	openCount = 0;

	enum eTag
	{
		eTagNone,
		eTagRate,
		eTagValue,
	}
	mTag = eTagNone;

	for(	TCHAR line[ MAX_PATH ];
			! file.IsEOF();
			ZeroMemory( line, sizeof( line ) ) )
	{
		file.GetLine( line, sizeof( line ) / sizeof( TCHAR ) );

		const int length = _tcslen( line );

		if( ! length )
		{
			continue;
		}

		// �߰��� �ּ� ����
		{
			for( int i = 0; i < length - 1; ++i )
			{
				const char a = line[ i ];
				const char b = line[ i + 1 ];

				if( '/' == a &&
					'/' == b )
				{
					line[ i ] = 0;
					break;
				}
			}
		}

		text.push_back( line );

		const TCHAR* separator	= _T( " ~\n\t=,+\"()%" );
		const TCHAR* token		= _tcstok( line, separator );
		const TCHAR* markBegin	= _T( "{" );
		const TCHAR* markEnd	= _T( "}" );

		if( 0 == token )
		{
			text.pop_back();
		}		
		else if( 0 == _tcsicmp( token, "rate" ) )
		{
			mTag = eTagRate;

			text.pop_back();
		}
		else if( 0 == _tcsicmp( token, "value" ) )
		{
			mTag = eTagValue;

			text.pop_back();
		}
		else if( 0 == _tcsnicmp( markBegin, token, strlen( markBegin ) ) )
		{
			++openCount;
		}
		else if( 0 == _tcsnicmp( markEnd, token, strlen( markEnd ) ) )
		{
			if( 0 < --openCount )
			{
				continue;
			}

			switch( mTag )
			{
			case eTagRate:
				{
					DWORD					step	= 0;
					MixSetting::Ability*	ability = 0;

					for( std::list< std::string >::const_iterator it = text.begin(); text.end() != it; ++it )
					{
						strcpy( line, it->c_str() );

						token = _tcstok( line, separator );

						// ����
						if( ! strcmpi( token, "sword" ) )
						{
							ability = &( mMixSetting.mEquippment[ MixSetting::eEquippmentSword ] );

							step = 0;
						}
						else if( ! strcmpi( token, "axe" ) )
						{
							ability = &( mMixSetting.mEquippment[ MixSetting::eEquippmentAxe ] );

							step = 0;
						}
						else if( ! strcmpi( token, "mace" ) )
						{
							ability = &( mMixSetting.mEquippment[ MixSetting::eEquippmentMace ] );

							step = 0;
						}
						else if( ! strcmpi( token, "dagger" ) )
						{
							ability = &( mMixSetting.mEquippment[ MixSetting::eEquippmentDagger ] );

							step = 0;
						}
						else if( ! strcmpi( token, "bow" ) )
						{
							ability = &( mMixSetting.mEquippment[ MixSetting::eEquippmentBow ] );

							step = 0;
						}
						else if( ! strcmpi( token, "staff" ) )
						{
							ability = &( mMixSetting.mEquippment[ MixSetting::eEquippmentStaff ] );

							step = 0;
						}
						else if( ! strcmpi( token, "robe" ) )
						{
							ability = &( mMixSetting.mEquippment[ MixSetting::eEquippmentRobe ] );

							step = 0;
						}
						else if( ! strcmpi( token, "light_armor" ) )
						{
							ability = &( mMixSetting.mEquippment[ MixSetting::eEquippmentLightArmor ] );

							step = 0;
						}
						else if( ! strcmpi( token, "heavy_armor" ) )
						{
							ability = &( mMixSetting.mEquippment[ MixSetting::eEquippmentHeavyArmor ] );

							step = 0;
						}

						// �ɷ�ġ
						if( ! strcmpi( token, "strength" ) )
						{
							step += atoi( _tcstok( 0, separator ) );

							( *ability )[ step ] = MixSetting::eAbilityStrength;
						}
						else if( ! strcmpi( token, "intelligence" ) )
						{
							step += atoi( _tcstok( 0, separator ) );

							( *ability )[ step ] = MixSetting::eAbilityIntelligence;
						}
						else if( ! strcmpi( token, "dexterity" ) )
						{
							step += atoi( _tcstok( 0, separator ) );

							( *ability )[ step ] = MixSetting::eAbilityDexterity;
						}
						else if( ! strcmpi( token, "vitality" ) )
						{
							step += atoi( _tcstok( 0, separator ) );

							( *ability )[ step ] = MixSetting::eAbilityVitality;
						}
						else if( ! strcmpi( token, "wisdom" ) )
						{
							step += atoi( _tcstok( 0, separator ) );

							( *ability )[ step ] = MixSetting::eAbilityWisdom;
						}
					}

					break;
				}
			case eTagValue:
				{
					MixSetting::Range* range = 0;

					for( std::list< std::string >::const_iterator it = text.begin(); text.end() != it; ++it )
					{
						strcpy( line, it->c_str() );

						token = _tcstok( line, separator );

						if( ! strcmpi( "level", token ) )
						{
							const DWORD minLevel = atoi( _tcstok( 0, separator ) );
							const DWORD maxLevel = atoi( _tcstok( 0, separator ) );

							range = &( mMixSetting.mRangeMap[ maxLevel ] );
						}
						else if( ! strcmpi( "range", token ) )
						{
							const DWORD minRange = atoi( _tcstok( 0, separator ) );
							const DWORD maxRange = atoi( _tcstok( 0, separator ) );

							range->mMin	= minRange;
                            range->mMax	= maxRange;
						}
					}

					break;
				}
			case eTagNone:
				{
					ASSERT( 0 );
					break;
				}
			}

			text.clear();
		}
	}

	if( openCount )
	{
		ASSERT( 0 && "MixSetting.bin�� {} ����� �ùٷ� ������ �ʾҽ��ϴ�" );
	}
}


// 080228 LUJ, ��ȣ �������� ���� ��ũ��Ʈ �Ľ� ����
void CItemManager::LoadMixScript()
{
	CMHFile file;

	if( ! file.Init( "System/Resource/ItemMix.bin", "rb" ) )
	{
		ASSERT( 0 );
		return;
	}

	char buffer[ MAX_PATH * 5 ];

	enum Type
	{
		TypeNone,
		TypeProtection,
		TypeResult,
	}
	type = TypeNone;

	while( ! file.IsEOF() )
	{
		file.GetLine( buffer, sizeof( buffer ) );

		const int length = _tcslen( buffer );

		if( ! length )
		{
			continue;
		}

		// �߰��� �ּ� ����
		for( int i = 0; i < length - 1; ++i )
		{
			const char a = buffer[ i ];
			const char b = buffer[ i + 1 ];

			if( '/' == a &&
				'/' == b )
			{
				buffer[ i ] = 0;
				break;
			}
		}

		const char*		separator	= "~\n\t(), ";
		const char*		token		= strtok( buffer, separator );

		if( ! token )
		{
			continue;
		}
		else if( ! stricmp( "#protection", token ) )
		{
			type = TypeProtection;
			continue;
		}
		else if( ! stricmp( "#result", token ) )
		{
			type = TypeResult;
			continue;
		}

		switch( type )
		{
		case TypeProtection:
			{
				const DWORD itemIndex = atoi( token );

				mMixProtectionItemSet.insert( itemIndex );
				break;
			}
		case TypeResult:
			{
				const DWORD		itemIndex	= atoi( token );
				MixScript&		data		= mMixScriptMap[ itemIndex ];

				data.push_back( ItemMixResult() );

				ItemMixResult& result = data.back();

				result.mMaterial[ itemIndex ] = 1;

				{
					result.mItemIndex		= strtoul( strtok( 0, separator ), 0, 10 );
					result.mRequiredLevel	= WORD( atoi( strtok( 0, separator ) ) );
					result.mMoney			= strtoul( strtok( 0, separator ), 0, 10 );

					const float rate		= float( atof( strtok( 0, separator ) ) / 10000 );
					result.mSuccessRate		= min( 1.0f, rate );
				}

				while( true )
				{
					const char* token1 = strtok( 0, separator );
					const char* token2 = strtok( 0, separator );

					if( ! token1 || ! token2 )
					{
						break;
					}

					const DWORD index		= strtoul( token1, 0, 10 );
					const DWORD	quantity	= strtoul( token2, 0, 10 );

					if( 0 < quantity )
					{
						result.mMaterial[ index ] += quantity;
					}
				}

				break;
			}
		}
	}
}


const MixScript* CItemManager::GetMixScript( DWORD itemIndex ) const
{
	MixScriptMap::const_iterator it = mMixScriptMap.find( itemIndex );

	return mMixScriptMap.end() == it ? 0 : &( it->second );
};


CItemManager* CItemManager::GetInstance()
{
	static CItemManager instance;

	return &instance;
}


const DissolveScript* CItemManager::GetDissolveScript( DWORD itemIndex ) const
{
	DissolveScriptMap::const_iterator it = mDissolveScriptMap.find( itemIndex );

	return mDissolveScriptMap.end() == it ? 0 : &( it->second );
}


void CItemManager::LoadDissolveScript()
{
	CMHFile file;

	if( ! file.Init( "System/Resource/itemDissolve.bin", "rb" ) )
	{
		ASSERTMSG( 0, "���ؽ�ũ��Ʈ(itemDissolve.bin) �� �����ϴ�" );
		return;
	}

	char buffer[ 1024 ];

	while( ! file.IsEOF() )
	{
		file.GetLine( buffer, sizeof( buffer ) );

		const char* separator	= "\t\n ,";
		const char* token		= strtok( buffer, separator );
		const DWORD itemIndex	= strtoul( token, 0, 10 );

		DissolveScript& script = mDissolveScriptMap[ itemIndex ];

		script.mLevel			= strtoul( strtok( 0, separator ), 0, 10 ) ;

		const DWORD staticResultSize = 5;

		for( DWORD i = 0; i < staticResultSize; ++i )
		{
			const char* token1		= strtok( 0, separator );
			const char* token2		= strtok( 0, separator );

			ASSERT( token1 && token2 );

			const DWORD index		= strtoul( token1, 0, 10 );
			const DWORD quantity	= strtoul( token2, 0, 10 );

			if( index && quantity )
			{
				script.mStaticResult.push_back( DissolveScript::Result() );

				DissolveScript::Result& result = script.mStaticResult.back();

				result.mItemIndex	= index;
				result.mQuantity	= quantity;
			}
		}

		// Ȯ���� ���� �������� �����ϴ� ���� ����� ó���ϱ� ���� ��ٷӴ�.
		// ������ ���� ������ ���̴�. ���� �Ľ� ����� �����Ѵ�. �� ���� �������� 
		// ������ ���鼭 �ʿ� ������Ų ������ �����Ѵ�(���� 10000�� ���� �ʵ��� ����).
		// ���� ��� 10%, 30%, 60% Ȯ���� �ִٸ�, �ʿ� 1000, 4000, 10000���� �����Ѵ�.
		// �׸��� ����� ���� ���� �������� ���� �� ���� lower_bound()�� �ش�Ǵ� ���� ã�Ƴ���.
		{
			const DWORD maxRate = 10000;

			for( DWORD rate = 0; rate < maxRate; )
			{
				const char* token1	= strtok( 0, separator );
				const char* token2	= strtok( 0, separator );
				const char* token3	= strtok( 0, separator );

				if( ! token1 || ! token2 || ! token3 )
				{
					break;
				}

				const DWORD itemIndex	= strtoul( token1, 0, 10 );
				const DWORD quantity	= strtoul( token2, 0, 10 );
				const DWORD percent		= strtoul( token3, 0, 10 );

				if( ! itemIndex || ! quantity || ! percent )
				{
					break;
				}

				rate = min( maxRate, rate + percent );

				DissolveScript::Result& result = script.mDynamicResult[ rate ];

				result.mItemIndex	= itemIndex;
				result.mQuantity	= quantity;
			}

			if( !	script.mDynamicResult.empty()	&&
					script.mDynamicResult.end() == script.mDynamicResult.find( maxRate ) )
			{
				ASSERT( 0 && "���� ��ũ��Ʈ�� Ȯ�� ���� ���� 10000�� ���� �ʽ��ϴ�. �ùٸ� ����� ������ �� �����ϴ�" );
			}
		}

		/*
		// �˻�: ���� ����� ��������� ��ġ�� ���� �ִ��� �˻��Ѵ�
		{
			for( DissolveScript::StaticResult::const_iterator it = script.mStaticResult.begin();
				script.mStaticResult.end() != it;
				++it )
			{
				const DissolveScript::Result& staticResult = *it;

                if( script.mDynamicResult.end() != script.mDynamicResult.find( staticResult.mItemIndex ) )
				{
					ASSERT( 0 );

					int i = 0;
					++i;
				}
			}
		}
		*/
	}
}


eLogitemmoney CItemManager::Dissolve( CPlayer* player, const MSG_ITEM_DISSOLVE_SYN2& input, const CBackupSlot* backupSlot )
{
	/*
	�䱸 ����:	�����Ҷ� �����Ǵ� ��ᰡ �κ��丮�� �ִ� ���� ���� ���ľ���
	ó�� ����:	���ش� ���� ����� ���� ����� �־� Ŭ���̾�Ʈ���� �Ϻ��ϰ� ����� �� ����
	���� Ŭ���̾�Ʈ���� ���� ����� ����ϰ�, �������� ���� ����� ����ϵ��� ��
	���� ���� �ý��ۿ����� �������� ���ŵ� ��쿡�� ������ Ŭ���̾�Ʈ�� �������� �����Ƿ�,
	���ŵ� ��츸 �ջ��Ͽ� Ŭ���̾�Ʈ�� �����Ѵ�.
	�Ѱ���:		��� ������ �� �κ����� �и��ʿ� ���� ������ ���� �������� ���Ѵ�
	- ���� ����� ���� ����� ������ ��ᰡ ���� ��� ���������� �������� �ʴ´�

	���:
	- ���� ���: �������� �����ϸ� �׻� ��� ��Ḧ ������
	- ���� ���: �������� ������ ������ ���Ƿ� ��� ��Ḧ ������
	*/

	if( ! player )
	{
		return eLog_ItemDissolvePlayerNotExist;
	}

	CInventorySlot*	playerSlot	= ( CInventorySlot* )player->GetSlot( eItemTable_Inventory );

	if( ! backupSlot	||
		! playerSlot )
	{
		return eLog_ItemDissolvePlayserHasNoInventory;
	}

	// ���� ������ �˻�
	ITEMBASE sourceItem;
	{
		const ITEMBASE* item	= playerSlot->GetItemInfoAbs( input.mSourceItemPosition );

		if(	!	item										||
			item->dwDBIdx	!= input.mSourceItemDbIndex	||
			item->Position	!= input.mSourceItemPosition )
		{
			return eLog_ItemDissolveItemDataNotMatched;
		}

		sourceItem = *item;
	}

	const DissolveScript* script = GetDissolveScript( sourceItem.wIconIdx );

	if( ! script )
	{
		return eLog_ItemDissolveServerHaveNoDissolvingData;
	}
	else if( script->mLevel > player->GetLevel() )
	{
		return eLog_ItemDissolveItNeedMoreLevel;
	}

	const POSTYPE	inventoryStartPosition	= TP_INVENTORY_START;
	const POSTYPE	inventoryEndPosition	= TP_INVENTORY_END + player->GetInventoryExpansionSize();

	// ���� �������� �����Ѵ�
	{
		const ITEMBASE* item = playerSlot->GetItemInfoAbs( input.mSourceItemPosition );

		if( ! item )
		{
			return eLog_ItemDissolveSourceItemNotExist;
		}

		if( IsDupItem( sourceItem.wIconIdx ) &&
			sourceItem.Durability > 1 )
		{
			if( EI_TRUE != playerSlot->UpdateItemAbs(
				player,
				item->Position,
				item->dwDBIdx,
				item->wIconIdx,
				item->Position,
				item->QuickPosition,
				item->Durability - 1 ) )
			{
				return eLog_ItemDissolveServerError;
			}
		}
		else
		{
			if( EI_TRUE != playerSlot->DeleteItemAbs(
				player,
				input.mSourceItemPosition,
				0 ) )
			{
				return eLog_ItemDissolveServerError;
			}
		}
	}

	// ����/���� ����� �����ϱ� ���� �����̳�. ����� �Ѳ����� ���Կ� ������Ʈ�� ���̴�
	typedef DissolveScript::StaticResult	ResultList;
	ResultList								resultList( script->mStaticResult );

	// ���� ����� ���� ��� �����Ͽ� �����Ѵ�
	// �� ����: ���� ����
	//
	// ����:	��� Ȯ���� ���� 10000�� �ȴ�.
	// ����:	�������� ������ Ȯ���� 0~10000 ������ �������� �����Ѵ�. ������ ���� 0~10000 ������ ���ǰ��� �� ��, �־��� ������
	//			���ų� ū �ݺ��ڸ� ���� std::map�� lower_bound() �޼ҵ带 ����Ͽ� �ش��ϴ� �ݺ��ڸ� ���� �ȴ�.
	// ����:	�������� ���� Ȯ���� ���� ���϶� 20%, ���϶� 30%, ���϶� 50%�� ����.
	//
	//			<- �� -><- �� -><- �� ->
	//			0	  2000	  5000	  10000
	//
	//			�������� 1000�� ���Դٸ� ����, 9000�� ������ ���� ����ȴ�. ������ Ȯ���� ������ Ŭ���� ��������.
	if( ! script->mDynamicResult.empty() )
	{
		const DWORD	random	= rand();
		const DWORD maxRate = 10000;
		const DWORD key		= random % ( maxRate + 1 );

		DissolveScript::DynamicResult::const_iterator it = script->mDynamicResult.lower_bound( key );

		// Ȯ������ ���� ���� ����� �ϳ� �����ϰ�, ���� ���� ���� ���� �ȿ��� �����ǵ��� ����
		if( script->mDynamicResult.end() == it )
		{
			return eLog_ItemDissolveServerError;
		}

		DissolveScript::Result result;
		{
			result.mItemIndex	= it->second.mItemIndex;
			result.mQuantity	= random % it->second.mQuantity + 1;
		}

		resultList.push_back( result );
	}

	// ����� ������Ʈ�Ѵ�.
	for(
		ResultList::const_iterator it = resultList.begin();
		resultList.end() != it;
		++it )
	{
		const DissolveScript::Result& result = *it;

		DURTYPE	dissolveSize = result.mQuantity;

		// �ϴ� �ִ��� ��ġ��
		if( IsDupItem( result.mItemIndex ) )
		{
			for(
				POSTYPE position = inventoryStartPosition;
				inventoryEndPosition > position;
				++position )
			{
				if( ! dissolveSize )
				{
					break;
				}

				const ITEMBASE* item = playerSlot->GetItemInfoAbs( position );

				if( !	item								||
					!	item->dwDBIdx						||
						item->wIconIdx != result.mItemIndex	||
					!	IsDupItem( item->wIconIdx )			||
						MAX_YOUNGYAKITEM_DUPNUM == item->Durability )
				{
					continue;
				}
				else if( item->Durability > MAX_YOUNGYAKITEM_DUPNUM )
				{
					return eLog_ItemMixBadQuantity;
				}

				const DURTYPE enableSize = MAX_YOUNGYAKITEM_DUPNUM - item->Durability;

				if( EI_TRUE != playerSlot->UpdateItemAbs(
					player,
					item->Position,
					item->dwDBIdx,
					item->wIconIdx,
					item->Position,
					item->QuickPosition,
					item->Durability + min( enableSize, dissolveSize ),
					UB_DURA,
					SS_CHKDBIDX ) )
				{
					//return eLog_ItemDissolveServerError;

					continue;
				}

				dissolveSize -= min( enableSize, dissolveSize );
			}
		}

		if( dissolveSize )
		{
			WORD	emptyPosition[ SLOT_MAX_INVENTORY_NUM ] = { 0 };
			WORD	emptyCount;
			{
				if( IsDupItem( result.mItemIndex ) )
				{
					emptyCount = WORD( dissolveSize ) / MAX_YOUNGYAKITEM_DUPNUM + 1;
				}
				else
				{
					emptyCount = WORD( dissolveSize );
				}
			}

			playerSlot->GetEmptyCell( emptyPosition, emptyCount );

			while( emptyCount-- )
			{
				const POSTYPE position = emptyPosition[ emptyCount ];

				ITEMBASE item;
				ZeroMemory( &item, sizeof( item ) );

				item.wIconIdx	= result.mItemIndex;
				item.Position	= position;

				if( IsDupItem( result.mItemIndex ) )
				{
					const DURTYPE enableSize = min( dissolveSize, MAX_YOUNGYAKITEM_DUPNUM );

					dissolveSize	-=	enableSize;
					item.Durability	=	enableSize;
				}
				else
				{
					--dissolveSize;
					item.Durability	= 0;
				}

				const ITEMBASE* backupItem = backupSlot->GetItem( position );

				// 071218 LUJ,	�� �����̶� ������ �̷� ������ �ѹ� �� ��ȸ����. DB �ε����� ��Ȱ�������ν�
				//				���� insert�� ����� ���ϰ�, update�� ���� �� �ִ�
				if( backupItem )
				{
					item.dwDBIdx = backupItem->dwDBIdx;

					// 071218 LUJ,	UpdateItemAbs�� DB �ε����� ������Ʈ���� �ʴ´�.
					//				���� InsertItemAbs�� ���� ����� �ɸ��� �ʵ��� �����Ѵ�
					if( EI_TRUE != playerSlot->InsertItemAbs(
						player,
						position,
						&item ) )
					{
						return eLog_ItemDissolveInsufficentSpace;
					}

				}
				else
				{
					// 071218 LUJ, DB �ε����� 0�� ��� ���� ���������� �ν��ϹǷ�, �ϴ� �������� �־�����
					item.dwDBIdx = UINT_MAX;

					if( EI_TRUE != playerSlot->InsertItemAbs(
						player,
						position,
						&item,
						SS_PREINSERT ) )
					{
						return eLog_ItemDissolveInsufficentSpace;
					}
				}
			}
		}

		if( dissolveSize )
		{
			return eLog_ItemDissolveInsufficentSpace;
		}
	}

	MSG_ITEM_DISSOLVE_ACK message;
	{
		ZeroMemory( &message, sizeof( message ) );

		message.Category			= MP_ITEM;
		message.Protocol			= MP_ITEM_DISSOLVE_ACK;
		message.mSourceItemDbIndex	= input.mSourceItemDbIndex;
		message.mSourceItemPosition	= input.mSourceItemPosition;
	}

	// ���Ե� �޽����� �����Ѵ�. �ٸ� DB �ε����� ���������� ��� ���̴�
	// ���� ������ �������� ������ �ε����� ����. ������Ʈ�� �������� �����۸� �ش�ȴ�
	for(
		POSTYPE position = inventoryStartPosition;
		inventoryEndPosition > position;
		++position )
	{
		const ITEMBASE*	item;
		{
			item = playerSlot->GetItemInfoAbs( position );

			// DB �ε����� ������ �� ����
			if( ! item ||
				! item->dwDBIdx )
			{
				item = 0;
			}
		}

		const ITEMBASE*	backupItem = backupSlot->GetItem( position );

		if( ! item )
		{
			// �������� ���µ�, �̷� ���Կ��� ������ ��� ������ ��Ȳ
			// �Ѵ� ���� ��� ������Ʈ�� �ʿ� ����
			if( backupItem )
			{
				// 071231 KTH -- GetID add
				ItemDeleteToDB( backupItem->dwDBIdx );

				message.mItem[ message.mSize++ ].dwDBIdx = backupItem->dwDBIdx;

				// 071220 LUJ
				LogItemMoney(
					player->GetID(),
					player->GetObjectName(),
					0,
					"",
					eLog_ItemDestroyDissolution,
					player->GetMoney(),
					0,
					0,
					backupItem->wIconIdx,
					backupItem->dwDBIdx,
					backupItem->Position,
					0,
					backupItem->Durability,
					0 );
			}

			continue;
		}

		// �������� �ִµ�, �̷� ���Կ� ���� ��� ���ԵǴ� ��Ȳ
		if( ! backupItem )
		{
			const ITEMOBTAINARRAYINFO* itemObtainArray = Alloc( 
				player,
				MP_ITEM,
				MP_ITEM_DISSOLVE_GETITEM,
				player->GetID(),
				player->GetID(),
				eLog_ItemObtainDissolve,
				1,
				( DBResult )( ObtainItemDBResult ) );

			ItemInsertToDB(
				player->GetID(),
				item->wIconIdx,
				item->Durability,
				item->Position,
				MAKEDWORD( 1, itemObtainArray->wObtainArrayID ) );

			// �������� �����س��� ���� ����
			message.mItem[ message.mSize++ ] = *item;

			// 071220 LUJ
			LogItemMoney(
				player->GetID(),
				player->GetObjectName(),
				0,
				"",
				eLog_ItemObtainDissolve,
				player->GetMoney(),
				0,
				0,
				item->wIconIdx,
				item->dwDBIdx,
				item->Position,
				0,
				item->Durability,
				0 );

			continue;
		}

		// �������� �ִµ�, �̷� ���԰� ������ ������ �ٸ��� ������Ʈ�� ��Ȳ
		if( item->wIconIdx		!= backupItem->wIconIdx ||
			item->Durability	!= backupItem->Durability )
		{
			ItemUpdateToDB(
				player->GetID(),
				item->dwDBIdx,
				item->wIconIdx,
				item->Durability,
				item->Position,
				item->QuickPosition );

			// 080610 LUJ, ���� �ɼ��� ���� �� �����Ƿ� �ɼ��� �����, �� �ɼ��� �����
			{
				ITEM_OPTION emptyOption		= { 0 };
				emptyOption.mItemDbIndex	= item->dwDBIdx;

				// 081118 LUJ, ĳ���� ��ȣ�� ������ ������ DB �����带 ������ Ű������ ���ǵ��� ��
				ItemOptionInsert( emptyOption, player->GetID() );
				ItemDropOptionInsert( emptyOption, player->GetID() );
				RemoveOption( item->dwDBIdx );
			}

			message.mItem[ message.mSize++ ] = *item;

			// 071220, LUJ. ������ �ٸ��� mirrorItem�� �Һ�Ǿ�, item�� ��� �����
			if( item->wIconIdx != backupItem->wIconIdx )
			{
				LogItemMoney(
					player->GetID(),
					player->GetObjectName(),
					0,
					"",
					eLog_ItemDestroyDissolution,
					player->GetMoney(),
					0,
					0,
					backupItem->wIconIdx,
					backupItem->dwDBIdx,
					backupItem->Position,
					0,
					backupItem->Durability,
					0 );

				LogItemMoney(
					player->GetID(),
					player->GetObjectName(),
					0,
					"",
					eLog_ItemDissolveSuccess,
					player->GetMoney(),
					0,
					0,
					item->wIconIdx,
					item->dwDBIdx,
					item->Position,
					0,
					item->Durability,
					0 );
			}
			else if( IsDupItem( item->wIconIdx ) )
			{
				const BOOL isUsed = backupItem->Durability > item->Durability;

				LogItemMoney(
					player->GetID(),
					player->GetObjectName(),
					0,
					"",
					isUsed ? eLog_ItemDestroyDissolution : eLog_ItemDissolveSuccess,
					player->GetMoney(),
					0,
					0,
					item->wIconIdx,
					item->dwDBIdx,
					item->Position,
					0,
					abs( int( backupItem->Durability ) - int( item->Durability ) ),
					0 );
			}
			else
			{
				ASSERT( 0 && "update error" );
			}
		}
	}

	player->SendMsg( &message, message.GetSize() );

	// ȿ���� ǥ���� �� �ֵ��� ����
	{
		MSG_DWORD m;
		m.Category	= MP_ITEM;
		m.Protocol	= MP_ITEM_WORKING_SUCCESS;
		m.dwData	= player->GetID();

		PACKEDDATA_OBJ->QuickSend( player, &m, sizeof( m ) );
	}

	// 080314 LUJ, �ɼ��� ����� �°���� �ʵ��� �����Ѵ�
	{
		RemoveOption( input.mSourceItemDbIndex );
		ItemOptionDelete( input.mSourceItemDbIndex );
	}

	return eLog_ItemDissolveSuccess;
}


const ReinforceScript* CItemManager::GetReinforceScript( DWORD itemIndex ) const
{
	ReinforceScriptMap::const_iterator it = mReinforceScriptMap.find( itemIndex );

	return mReinforceScriptMap.end() == it ? 0 : &( it->second );
}


// 080228 LUJ, ��ȣ �������� ���� ��ũ��Ʈ �Ľ� ����
void CItemManager::LoadReinforceScript()
{
	CMHFile file;

	if( ! file.Init( "system/resource/itemReinforce.bin", "rb" ) )
	{
		ASSERTMSG( 0, "��ȭ ������ ��ũ��Ʈ(itemReinforce.bin)�� �����" );
		return;
	}

	char buffer[ MAX_PATH * 5 ];

	enum Type
	{
		TypeNone,
		TypeProtection,
		TypeResult,
	}
	type = TypeNone;

	while( ! file.IsEOF() )
	{
		file.GetLine( buffer, sizeof( buffer ) );

		const int length = _tcslen( buffer );

		if( ! length )
		{
			continue;
		}

		// �߰��� �ּ� ����
		for( int i = 0; i < length - 1; ++i )
		{
			const char a = buffer[ i ];
			const char b = buffer[ i + 1 ];

			if( '/' == a &&
				'/' == b )
			{
				buffer[ i ] = 0;
				break;
			}
		}

		const char*		separator	= "~\n\t(), ";
		const char*		token		= strtok( buffer, separator );

		if( ! token )
		{
			continue;
		}
		else if( ! stricmp( "#protection", token ) )
		{
			type = TypeProtection;
			continue;
		}
		else if( ! stricmp( "#result", token ) )
		{
			type = TypeResult;
			continue;
		}

		switch( type )
		{
		case TypeProtection:
			{
				const DWORD itemIndex = atoi( token );

				mReinforceProtectionItemSet.insert( itemIndex );
				break;
			}
		case TypeResult:
			{
				const DWORD itemIndex = atoi( token );
				ASSERTMSG( mReinforceScriptMap.end() == mReinforceScriptMap.find( itemIndex ), "it's duplicated item index" );

				ReinforceScript& script = mReinforceScriptMap[ itemIndex ];

				const std::string reinforce = strtok( 0, separator );

				script.mMax		=		 atoi( strtok( 0, separator ) );
				script.mBias	= float( atof( strtok( 0, separator ) ) );
				script.mForRare	= ! strcmpi( "true", strtok( 0, separator ) );

				// ��ȭ ������ ������ ���� ����
				{
					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_hand );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Shield );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Armor );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Helmet );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Glove );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Boots );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Ring1 );
						script.mSlot.insert( eEquipSlot_Ring2 );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Necklace );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Earring1 );
						script.mSlot.insert( eEquipSlot_Earring2 );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Belt );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Band );
					}
				}

				// ��ȭ ������ �ɷ� ����
				{
					if( ! strcmpi( "strength", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypeStrength;
					}
					else if( ! strcmpi( "dexterity", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypeDexterity;
					}
					else if( ! strcmpi( "vitality", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypeVitality;
					}
					else if( ! strcmpi( "intelligence", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypeIntelligence;
					}
					else if( ! strcmpi( "wisdom", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypeWisdom;
					}
					else if( ! strcmpi( "mana", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypeMana;
					}
					else if( ! strcmpi( "recovery_mana", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypeManaRecovery;
					}
					else if( ! strcmpi( "life", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypeLife;
					}
					else if( ! strcmpi( "recovery_life", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypeLifeRecovery;
					}
					else if( ! strcmpi( "physic_defense", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypePhysicDefence;
					}
					else if( ! strcmpi( "physic_attack", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypePhysicAttack;
					}
					else if( ! strcmpi( "magic_defense", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypeMagicDefence;
					}
					else if( ! strcmpi( "magic_attack", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypeMagicAttack;
					}
					else if( ! strcmpi( "move_speed", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypeMoveSpeed;
					}
					else if( ! strcmpi( "evade", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypeEvade;
					}
					else if( ! strcmpi( "accuracy", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypeAccuracy;
					}
					else if( ! strcmpi( "critical_rate", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypeCriticalRate;
					}
					else if( ! strcmpi( "critical_damage", reinforce.c_str() ) )
					{
						script.mType	= ReinforceScript::eTypeCriticalDamage;
					}
					else
					{
						ASSERTMSG( 0, "itemReinforce.bin�� ���ǵǾ� ���� ���� Ű���尡 �ֽ��ϴ�" );

						mReinforceScriptMap.erase( itemIndex );
					}
				}

				break;
			}
		}
	}
}


BOOL CItemManager::IsRare( const ITEM_INFO& info ) const
{
	switch( info.kind )
	{
	case ITEM_INFO::eKindMasterPiece:
	case ITEM_INFO::eKindSetMasterPiece:
	case ITEM_INFO::eKindRare:
	case ITEM_INFO::eKindSetRare:
	case ITEM_INFO::eKindUnique:
	case ITEM_INFO::eKindSetUnique:
		{
			return TRUE;
		}
	}

	return FALSE;
}


const EnchantScript* CItemManager::GetEnchantScript( DWORD itemIndex ) const
{
	EnchantScriptMap::const_iterator it = mEnchantScriptMap.find( itemIndex );

	return mEnchantScriptMap.end() == it ? 0 : &( it->second );
}


const EnchantScript* CItemManager::GetEnchantScript( eEquipSlot slot ) const
{
	SlotToEnchantScript::const_iterator it = mSlotToEnchantScript.find( slot );

	return mSlotToEnchantScript.end() == it ? 0 : it->second;
}


// 080228 LUJ, ��ȣ �������� ���� ��ũ��Ʈ �Ľ� ����
// 080320 LUJ, ��æƮ ��ġ�� ������ ��ȣ ��� �߰�
// 080328 LUJ, ��æƮ ��ȣ ������ ���� �� �÷���/���̳ʽ� ���ʽ� �߰�
void CItemManager::LoadEnchantScript()
{
	CMHFile file;

	if( ! file.Init( "system/resource/itemEnchant.bin", "rb" ) )
	{
		ASSERTMSG( 0, "������ ���� ��ũ��Ʈ(itemReinforce.bin)�� �����" );
		return;
	}

	char buffer[ MAX_PATH * 5 ] = { 0 };

	enum Type
	{
		TypeNone,
		TypeProtectionAll,
		TypeResult,
	}
	type = TypeNone;

	while( ! file.IsEOF() )
	{
		file.GetLine( buffer, sizeof( buffer ) );

		const int length = _tcslen( buffer );

		if( ! length )
		{
			continue;
		}

		// �߰��� �ּ� ����
		for( int i = 0; i < length - 1; ++i )
		{
			const char a = buffer[ i ];
			const char b = buffer[ i + 1 ];

			if( '/' == a &&
				'/' == b )
			{
				buffer[ i ] = 0;
				break;
			}
		}

		const char*		separator	= "~\n\t(), :";
		const char*		token		= strtok( buffer, separator );

		if( ! token )
		{
			continue;
		}
		else if( ! stricmp( "#protection", token ) )
		{
			type = TypeProtectionAll;
			continue;
		}
		else if( ! stricmp( "#result", token ) )
		{
			type = TypeResult;
			continue;
		}		

		switch( type )
		{
			// 080320 LUJ, ��æƮ ��ġ�� �����ϰ� ��ȣ�ϴ� ����
			// 080328 LUJ, ������ ���� ������ ��ȣ ���ǿ� ���Խ�Ű��, �÷���/���̳ʽ� ���ʽ� Ȯ���� �߰�
		case TypeProtectionAll:
			{
				const DWORD itemIndex = atoi( token );

				EnchantProtection::Data& data = mEnchantProtection.mItemMap[ itemIndex ];
				ZeroMemory( &data.mEnchantLevelRange,	sizeof( data.mEnchantLevelRange ) );
				ZeroMemory( &data.mItemLimitLevelRange, sizeof( data.mItemLimitLevelRange ) );

				data.mEnchantLevelRange.mMin	= 		atoi( strtok( 0, separator ) );
				data.mEnchantLevelRange.mMax	= 		atoi( strtok( 0, separator ) );

				data.mItemLimitLevelRange.mMin	= 		atoi( strtok( 0, separator ) );
				data.mItemLimitLevelRange.mMax	= 		atoi( strtok( 0, separator ) );

				data.mMinimumEnchantLevel		=		atoi( strtok( 0, separator ) );
				data.mAddedPercent				= int(	atof( strtok( 0, separator ) ) / 100.0f * 1000.0f );

				// 080328 LUJ, ��æƮ �÷���/���̳ʽ� ���ʽ� Ȯ�� ���� �����
				;
				for(
					float plusRate	= 0, minusRate = 0;
					; )
				{
					const char* token1	= strtok( 0, separator );
					const char* token2	= strtok( 0, separator );

					if( ! token1 ||
						! token2 )
					{
						break;
					}

					const int	level	=			atoi( token1 );
					const float	rate	= float(	atof( token2 ) / 100.0f );

					if( 0 < level )
					{
						data.mPlusBonusMap[ plusRate += rate ] = level;
					}
					else if( 0 > level )
					{
						data.mMinusBonusMap[ minusRate += rate ] = level;
					}
				}

				break;
			}
		case TypeResult:
			{
				const DWORD itemIndex = atoi( token );

				ASSERT( mEnchantScriptMap.end() == mEnchantScriptMap.find( itemIndex ) );

				EnchantScript& script = mEnchantScriptMap[ itemIndex ];

				script.mItemIndex		= itemIndex;
				script.mItemLevel.mMin	= atoi( strtok( 0, separator ) );
				script.mItemLevel.mMax	= atoi( strtok( 0, separator ) );
				script.mEnchantMaxLevel	= atoi( strtok( 0, separator ) );

				for(
					DWORD abilitySize = 2;
					abilitySize--;
				)
					// ��ȭ ������ �ɷ� ����
				{
					token = strtok( 0, separator );

					if( ! strcmpi( "strength", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeStrength );
					}
					else if( ! strcmpi( "dexterity", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeDexterity );
					}
					else if( ! strcmpi( "vitality", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeVitality );
					}
					else if( ! strcmpi( "intelligence", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeIntelligence );
					}
					else if( ! strcmpi( "wisdom", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeWisdom );
					}
					else if( ! strcmpi( "mana", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeMana );
					}
					else if( ! strcmpi( "recovery_mana", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeManaRecovery );
					}
					else if( ! strcmpi( "life", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeLife );
					}
					else if( ! strcmpi( "recovery_life", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeLifeRecovery );
					}
					else if( ! strcmpi( "physic_defense", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypePhysicDefence );
					}
					else if( ! strcmpi( "physic_attack", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypePhysicAttack );
					}
					else if( ! strcmpi( "magic_defense", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeMagicDefence );
					}
					else if( ! strcmpi( "magic_attack", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeMagicAttack );
					}
					else if( ! strcmpi( "move_speed", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeMoveSpeed );
					}
					else if( ! strcmpi( "evade", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeEvade );
					}
					else if( ! strcmpi( "accuracy", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeAccuracy );
					}
					else if( ! strcmpi( "critical_rate", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeCriticalRate );
					}
					else if( ! strcmpi( "critical_damage", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeCriticalDamage );
					}
					else if( ! strcmpi( "none", token ) )
					{
						// to do nothing
					}
					else
					{
						ASSERTMSG( 0, "itemEnchant.bin�� ���ǵǾ� ���� ���� Ű���尡 �ֽ��ϴ�" );

						mEnchantScriptMap.erase( itemIndex );
					}
				}

				// ���� ������ ������ ���� ����
				{
					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_hand );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Shield );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Armor );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Helmet );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Glove );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Boots );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Ring1 );
						script.mSlot.insert( eEquipSlot_Ring2 );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Necklace );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Earring1 );
						script.mSlot.insert( eEquipSlot_Earring2 );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Belt );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Band );
					}
				}

				break;
			}
		}
	}
}


void CItemManager::AddEnchantValue( ITEM_OPTION& option, const EnchantScript::Ability& ability, int value ) const
{
	for(	EnchantScript::Ability::const_iterator it = ability.begin();
			ability.end() != it;
			++it )
	{
		ITEM_OPTION::Reinforce& reinforce = option.mReinforce;

		switch( EnchantScript::eType( *it ) )
		{
		case ReinforceScript::eTypeStrength:
			{
				reinforce.mStrength			+= value;
				break;
			}
		case ReinforceScript::eTypeDexterity:
			{
				reinforce.mDexterity		+= value;
				break;
			}
		case ReinforceScript::eTypeVitality:
			{
				reinforce.mVitality			+= value;
				break;
			}
		case ReinforceScript::eTypeWisdom:
			{
				reinforce.mWisdom			+= value;
				break;
			}
		case ReinforceScript::eTypeIntelligence:
			{
				reinforce.mIntelligence		+= value;
				break;
			}
		case ReinforceScript::eTypeLife:
			{
				reinforce.mLife				+= value;
				break;
			}
		case ReinforceScript::eTypeLifeRecovery:
			{
				reinforce.mLifeRecovery		+= value;
				break;
			}
		case ReinforceScript::eTypeMana:
			{
				reinforce.mMana				+= value;
				break;
			}
		case ReinforceScript::eTypeManaRecovery:
			{
				reinforce.mManaRecovery		+= value;
				break;
			}
		case ReinforceScript::eTypePhysicAttack:
			{
				reinforce.mPhysicAttack		+= value;
				break;
			}
		case ReinforceScript::eTypePhysicDefence:
			{
				reinforce.mPhysicDefence	+= value;
				break;
			}
		case ReinforceScript::eTypeMagicAttack:
			{
				reinforce.mMagicAttack		+= value;
				break;
			}
		case ReinforceScript::eTypeMagicDefence:
			{
				reinforce.mMagicDefence		+= value;
				break;
			}
		case ReinforceScript::eTypeMoveSpeed:
			{
				reinforce.mMoveSpeed		+= value;
				break;
			}
		case ReinforceScript::eTypeEvade:
			{
				reinforce.mEvade			+= value;
				break;
			}
		case ReinforceScript::eTypeAccuracy:
			{
				reinforce.mAccuracy			+= value;
				break;
			}
		case ReinforceScript::eTypeCriticalRate:
			{
				reinforce.mCriticalRate		+= value;
				break;
			}
		case ReinforceScript::eTypeCriticalDamage:
			{
				reinforce.mCriticalDamage	+= value;
				break;
			}
		default:
			{
				ASSERT( 0 && "invaild script. check it now" );
				break;
			}
		}
	}
}


void CItemManager::LoadCoolTimeScript()
{
	CMHFile file;

	if( ! file.Init( "system/resource/itemCoolTime.bin", "rb" ) )
	{
		ASSERTMSG( 0, "������ ��Ÿ�� ��ũ��Ʈ(itemCoolTime.bin)�� �����" );
		return;
	}

	int		openCount	= 0;
	DWORD	groupIndex	= 1;

	std::list< std::string >	text;

	for(	TCHAR line[ MAX_PATH ];
		! file.IsEOF();
		ZeroMemory( line, sizeof( line ) ) )
	{
		file.GetLine( line, sizeof( line ) / sizeof( TCHAR ) );

		const int length = _tcslen( line );

		if( ! length )
		{
			continue;
		}

		// �߰��� �ּ� ����
		{
			for( int i = 0; i < length - 1; ++i )
			{
				const char a = line[ i ];
				const char b = line[ i + 1 ];

				if( '/' == a &&
					'/' == b )
				{
					line[ i ] = 0;
					break;
				}
			}
		}

		text.push_back( line );

		const TCHAR* separator	= _T( " ~\n\t=,+\"()%" );
		const TCHAR* token		= _tcstok( line, separator );
		const TCHAR* markBegin	= _T( "{" );
		const TCHAR* markEnd	= _T( "}" );

		if( 0 == token ||
			0 == _tcsicmp( "group", token ) )
		{
			text.pop_back();
		}
		else if( 0 == _tcsnicmp( markBegin, token, strlen( markBegin ) ) )
		{
			text.pop_back();

			++openCount;
		}
		else if( 0 == _tcsnicmp( markEnd, token, strlen( markEnd ) ) )
		{
			text.pop_back();

			if( ! --openCount )
			{
				for(	std::list< std::string >::const_iterator it = text.begin();
					text.end() != it;
					++it )
				{
					// �Ľ�����... ù��°�� ������ �ε���, ������ ��Ÿ��
					char buffer[ MAX_PATH ];

					_tcscpy( buffer, it->c_str() );

					const DWORD itemIndex	= strtoul( _tcstok( buffer, separator ), 0, 10 );
					const DWORD coolTime	= atoi( _tcstok( 0, separator ) ) * 1000;

					CoolTimeScript::Unit& unit = mCoolTimeScript.mItemData[ itemIndex ];

					unit.mIndex	= groupIndex;
					unit.mTime	= coolTime;

					{
						CoolTimeScript::ItemIndex& indexSet =  mCoolTimeScript.mGroupData[ groupIndex ];

						indexSet.insert( itemIndex );
					}
				}

				text.clear();
				++groupIndex;
			}
			else
			{
				ASSERT( 0 );
				return;
			}
		}
	}
}


void CItemManager::Process()
{
	//ProcessCoolTime();
}


//void CItemManager::ProcessCoolTime()
//{
//	// ��Ÿ�� üũ. ���� �ð� ���� ���ĵǾ� �����Ƿ� ���� ������ �ִ� �͸� üũ�Ѵ�.
//	if( mCoolTimeQueue.empty() )
//	{
//		return;
//	}
//
//	const CoolTimeSort::Tick& tick = mCoolTimeQueue.top();
//
//	// ������ ���ٸ� ó�� ������ ����.
//	if( ! g_pUserTable->FindUser( tick.mPlayerIndex ) )
//	{
//		mCoolTimeQueue.pop();
//		return;
//	}
//
//	const DWORD	tickCount	= GetTickCount();
//	
//	// ���� �ð� ���� ���ĵ� �켱���� ť�� ���� ������ �ִ� ������ �ð��� ����Ǿ����� üũ����
//	if( tick.mBegin > tick.mEnd && tickCount < tick.mBegin && tickCount > tick.mEnd ||	// �����÷�
//		tick.mBegin < tick.mEnd && tick.mEnd < tickCount )								// �Ϲ�
//	{
//		UsingCoolTimeGroup::iterator it = mUsingCoolTimeGroup.find( tick.mPlayerIndex );
//
//		if( mUsingCoolTimeGroup.end() != it )
//		{
//			CoolTimeGroup& group = it->second;
//
//			group.erase( tick.mCoolTimeGroup );
//
//			if( group.empty() )
//			{
//				mUsingCoolTimeGroup.erase( it );
//			}
//		}
//
//		mCoolTimeQueue.pop();
//	}
//}


//BOOL CItemManager::IsCoolTime( DWORD playerIndex, DWORD itemIndex ) const
//{
//	UsingCoolTimeGroup::const_iterator it =  mUsingCoolTimeGroup.find( playerIndex );
//
//	if( mUsingCoolTimeGroup.end() == it )
//	{
//		return FALSE;
//	}
//
//	const CoolTimeGroup& group = it->second;
//
//	return group.end() != group.find( GetCoolTimeGroupIndex( itemIndex ) );
//}
//
//
//BOOL CItemManager::AddCoolTime( DWORD playerIndex, DWORD itemIndex )
//{
//	const DWORD groupIndex = GetCoolTimeGroupIndex( itemIndex );
//
//	if( ! groupIndex )
//	{
//		return FALSE;
//	}
//
//	// �ش� ��Ÿ�� �׷��� ��������� ó���Ѵ�.
//	{
//		CoolTimeGroup& group = mUsingCoolTimeGroup[ playerIndex ];
//
//		group.insert( groupIndex );
//	}
//
//	// ť�� �־� ��� ���� ������ �ڵ����� �����ǵ��� �Ѵ�.
//	{
//		CoolTimeScript::ItemData::const_iterator it = mCoolTimeScript.mItemData.find( itemIndex );
//
//		if( mCoolTimeScript.mItemData.end() == it )
//		{
//			ASSERT( 0 );
//			return FALSE;
//		}
//
//		const CoolTimeScript::Unit& unit = it->second;
//
//		CoolTimeSort::Tick	tick;
//		const DWORD			tickCount	= GetTickCount();
//
//		tick.mCoolTimeGroup	= groupIndex;
//		tick.mBegin			= tickCount;
//		tick.mEnd			= tickCount + unit.mTime;
//		tick.mPlayerIndex	= playerIndex;
//
//		mCoolTimeQueue.push( tick );
//	}
//
//	return TRUE;
//}


DWORD CItemManager::GetCoolTimeGroupIndex( DWORD itemIndex ) const
{
	CoolTimeScript::ItemData::const_iterator it = mCoolTimeScript.mItemData.find( itemIndex );

	if( mCoolTimeScript.mItemData.end() == it )
	{
		return 0;
	}

	const CoolTimeScript::Unit& unit = it->second;

	return unit.mIndex;
}


void CItemManager::RemoveCoolTime( DWORD playerIndex )
{
	mUsingCoolTimeGroup.erase( playerIndex );
}


//void CItemManager::RemoveCoolTime( DWORD playerIndex, DWORD itemIndex )
//{
//	const DWORD groupIndex = GetCoolTimeGroupIndex( itemIndex );
//
//	if( groupIndex )
//	{
//		CoolTimeGroup& group = mUsingCoolTimeGroup[ playerIndex ];
//
//		group.erase( groupIndex );
//
//		if( group.empty() )
//		{
//			mUsingCoolTimeGroup.erase( playerIndex );
//		}
//	}
//}

// 071201 KTH - �� �̵� �ֹ��� ���� ó�� �ϴºκ�.
BOOL CItemManager::Move_ChangeMap(CPlayer* pPlayer, MAPTYPE mapType) 
{
	if(pPlayer == NULL)
		return FALSE;
	
	VECTOR3 vector3 = *GAMERESRCMNGR->GetRevivePoint( mapType );

	MAPCHANGE_INFO* ChangeInfo = GAMERESRCMNGR->GetMapChangeInfoForDestMapNum( mapType );
//	if( !ChangeInfo ) return;	//nackif( !ChangeInfo ) return;	//nack*/

	if( CHANNELSYSTEM->GetChannelID( pPlayer->GetChannelID() - 1 ) == 0 && !CHANNELSYSTEM->IsQuestMap() || !ChangeInfo )
	{
		MSG_DWORD msg;
		msg.Category	= MP_ITEM;
		msg.Protocol	= MP_ITEM_MAPMOVE_SCROLL_NACK;
		msg.dwData		= pPlayer->GetGridID() - 1;
		pPlayer->SendMsg( &msg, sizeof( msg ) ); 
		return FALSE;
	}

	SaveMapChangePointUpdate( pPlayer->GetID(), ChangeInfo->Kind );

	// RaMa - 04.12.14
	pPlayer->UpdateLogoutToDB(FALSE);

	VECTOR3 pos;

	int temp;
	temp = rand() % 500 - 250;
	pos.x = vector3.x + temp;
	temp = rand() % 500 - 250;
	pos.z = vector3.z + temp;
	pos.y = 0;

	pPlayer->SetMapMoveInfo( mapType, (DWORD)pos.x, (DWORD)pos.z);

	// 081218 LUJ, ���� ���� �÷��̾� ������ �����س��´�
	const DWORD			playerIndex		= pPlayer->GetID();
	const DWORD			userIndex		= pPlayer->GetUserID();
	const MONEYTYPE		inventoryMoney	= pPlayer->GetMoney( eItemTable_Inventory );
	const MONEYTYPE		storageMoney	= pPlayer->GetMoney( eItemTable_Storage );

	g_pServerSystem->RemovePlayer( playerIndex );
	
	// 081218 LUJ, ������Ʈ���� �� �̵��� ���� ����� ��� ������Ʈ���� ���� ������ ���Եȴ�.
	//			�̸� ���� ���� ���ν��� ó�� �Ϸ� �� �����Ѵ�
	UpdatePlayerOnMapOut(
		playerIndex,
		userIndex,
		inventoryMoney,
		storageMoney,
		mapType,
		-1 );
	return TRUE;	
}


BOOL CItemManager::IsTwoHand( DWORD itemIndex )
{
	const ITEM_INFO* info = GetItemInfo( itemIndex );

	return info	&& (
		info->WeaponAnimation == eWeaponAnimationType_TwoHand	||
		info->WeaponAnimation == eWeaponAnimationType_Staff		||
		info->WeaponAnimation == eWeaponAnimationType_Bow		||
		info->WeaponAnimation == eWeaponAnimationType_Gun );
}


// 071204 LYW --- ItemManager : ���� �ʱ�ȭ �Լ� �߰�.
BOOL CItemManager::UseItem_ResetStats(CPlayer* pPlayer, BOOL bCheat)
{
	// ������ ���� ó���� �� �Լ� ���� ��ƾ���� ó�� �ϱ� ������, �� �Լ������� ó�� ���� �ʴ´�.

	if( !pPlayer ) return FALSE;

	// 080124 KTH -- PKMode ���϶����� ��� �Ұ�
	if( pPlayer->IsPKMode() )
		return FALSE;

	// 071206 KTH -- ���� �ʱ�ȭ �߿� ��ȯ�� �ȵǵ��� Lock�� �ɾ��ش�.
	pPlayer->SetResetStat(true);

	// ����, ���� �� �� ������ �⺻ ���� �迭�� �����Ѵ�.
	int StrArray[2][3] = { {16, 12,  8}, { 13, 11,  7} } ;					// �� ��ġ.
	int DexArray[2][3] = { {10, 14,  7}, { 12, 15,  8} } ;					// ��ø ��ġ.
	int VitArray[2][3] = { {14, 12, 11}, { 13, 11,  9} } ;					// �ǰ� ��ġ.
	int IntArray[2][3] = { { 6,  7, 13}, {  7,  8, 14} } ;					// ���� ��ġ.
	int WisArray[2][3] = { { 7,  8, 14}, {  8,  8, 15} } ;					// ���� ��ġ.

	int nRacial = pPlayer->GetRacial() ;									// ���� ��ġ�� �޴´�.
	int nJopNum = pPlayer->GetJop(0) ;										// ���� ��ȣ�� �޴´�.

	if( nJopNum != 0 )
	{
		--nJopNum ;
	}

	int Str = StrArray[nRacial][nJopNum] * 5 ;								// �⺻ �� ��ġ�� �޴´�.
	int Dex = DexArray[nRacial][nJopNum] * 5 ;								// �⺻ ��ø ��ġ�� �޴´�.
	int Vit = VitArray[nRacial][nJopNum] * 5 ;								// �⺻ �ǰ� ��ġ�� �޴´�.
	int Int = IntArray[nRacial][nJopNum] * 5 ;								// �⺻ ���� ��ġ�� �޴´�.
	int Wis = WisArray[nRacial][nJopNum] * 5 ;								// �⺻ ���� ��ġ�� �޴´�.

	int nDefaultPoint = 0 ;													// �⺻ ���� ����Ʈ�� ���� ������ �����ϰ� 0����.
	nDefaultPoint = (Str + Dex + Vit + Int + Wis) ;							// �⺻ ���� �� ����Ʈ�� ���Ѵ�.

	LEVELTYPE maxLevel = 0 ;
	maxLevel = pPlayer->GetMaxLevel() ;

	int nNewPoint = 0 ;

	if( maxLevel > 1 )
	{
		nNewPoint = maxLevel - 1 ;						// �ʱ�ȭ ������ �ܿ� ����Ʈ�� ���Ѵ�.
	}
	else
	{
		nNewPoint = 0 ;
	}

	HERO_TOTALINFO* pTotalInfo = pPlayer->Get_HeroTotalInfo() ;

	if( !pTotalInfo )
	{
		MSGBASE msg ;
		memset(&msg, 0, sizeof(MSGBASE)) ;

		msg.Category	= MP_ITEM ;
		msg.Protocol	= MP_ITEM_RESET_STATS_NACK ;
		msg.dwObjectID	= pPlayer->GetID() ;

		pPlayer->SendMsg(&msg, sizeof(MSGBASE)) ;

		return FALSE;
	}

	pTotalInfo->Str = Str ;
	pTotalInfo->Dex = Dex ;
	pTotalInfo->Vit = Vit ;
	pTotalInfo->Int = Int ;
	pTotalInfo->Wis = Wis ;

	pTotalInfo->LevelUpPoint = nNewPoint ;

	DB_UpdateToDefaultStats( pPlayer->GetID(), Str, Dex, Vit, Int, Wis, nNewPoint ) ;

	// 071206 KTH -- ���� �ʱ�ȭ�� ���� �Ŀ� ������ ������ �� �ֵ��� Lock�� Ǯ���ش�.
	pPlayer->SetResetStat(false);

	return TRUE;
}


// 071204 LYW --- ItemManager : ��ų �ʱ�ȭ �Լ� �߰�.
BOOL CItemManager::UseItem_ResetSkill(CPlayer* pPlayer, BOOL bCheat)
{
	// ������ ���� ó���� �� �Լ� ���� ��ƾ���� ó�� �ϱ� ������, �� �Լ������� ó�� ���� �ʴ´�.

	if( !pPlayer )
	{
		return FALSE;
	}
	
	// 080124 KTH -- PKMode ���϶����� ��� �Ұ�
	if( pPlayer->IsPKMode() )
		return FALSE;

	// 071206 KTH -- ��ų�� ������Ʈ ���� �ʵ��� ��� �ش�.
	pPlayer->SetResetSkill(true);

	//---KES ��ų�ʱ�ȭ��	071207
	cSkillTree* pSkillTree = pPlayer->GetSkillTree();
	if( !pSkillTree )
	{
		//���� ó���� �ʿ��Ѱ�? �����۸� ������� �ʱ�ȭ�� �ȵǴ� ��찡 ���� �� �ִ�?
		return FALSE;
	}

	DWORD dwTotalSkillMoney = 0;

	pSkillTree->SetPositionHead();
	while( SKILL_BASE* pSkill = pSkillTree->GetSkillBase() )
	{
		if( pSkill->dwDBIdx )	//��� ��ų
		{
			dwTotalSkillMoney += GAMERESRCMNGR->GetSkillMoney( pSkill->wSkillIdx + pSkill->Level - 1 );
		}
		else	//��Ʈ�� ����� ��ų�̹Ƿ� stored���� ã�ƿ´�.
		{
			SKILL_BASE* pStoredSkill = pPlayer->GetStoredSkill( pSkill->wSkillIdx );
			if( pStoredSkill )
				dwTotalSkillMoney += GAMERESRCMNGR->GetSkillMoney( pStoredSkill->wSkillIdx + pStoredSkill->Level - 1 );
		}
	}

	pPlayer->SetRetrunSkillMoney( dwTotalSkillMoney );
	//------------------------------------------

	DB_ResetToDefaultSkill( pPlayer->GetID() ) ;

	return TRUE;
}



const ITEM_OPTION& CItemManager::GetOption( const ITEMBASE& item ) const
{
	OptionMap::const_iterator it = mOptionMap.find( item.dwDBIdx );

	if( mOptionMap.end() == it )
	{
		const static ITEM_OPTION emptyOption = { 0 };

		return emptyOption;
	}

	return *( it->second );
}


void CItemManager::AddOption( const ITEM_OPTION& option )
{
	OptionMap::iterator it = mOptionMap.find( option.mItemDbIndex );

	if( mOptionMap.end() != it )
	{
		*( it->second ) = option;
		return;
	}

	ITEM_OPTION* memory = mOptionPool.Alloc();
	*memory = option;

	mOptionMap.insert( std::make_pair( option.mItemDbIndex, memory ) );
}


void CItemManager::RemoveOption( const ITEMBASE& item )
{
	RemoveOption( item.dwDBIdx );
}


void CItemManager::RemoveOption( DWORD itemDbIndex )
{
	OptionMap::iterator it = mOptionMap.find( itemDbIndex );

	if( mOptionMap.end() == it )
	{
		return;
	}

	mOptionPool.Free( it->second );
	mOptionMap.erase( it );
}


void CItemManager::AddDropOption( const ITEMBASE& item, ITEM_OPTION& option )
{
	/*
	080130 LUJ,	��ӵǴ� �����ۿ� �������� �ɼ��� �����Ѵ�
				���� ū ������ Ȯ�� ó���� ���� 32bit ũ���� ���ǰ��� �����Ѵ�. �� ���ǰ���
				�̿��Ͽ� Ȯ���ʿ��� �ش�Ǵ� ���Ҹ� ������(std::map::lower_bound()�� STL ���۷��� ����)
				���� ���Ұ� ������ Ȯ�� ������ ��� ������ �����Ѵ�.
	*/
	
	const ITEM_INFO* info = GetItemInfo( item.wIconIdx );

	if( !	info ||
			eEquipType_None == info->EquipType )
	{
		return;
	}

	const LONGLONG key = GetItemKey( eEquipSlot( info->EquipSlot + 1 ), eWeaponType( info->WeaponType ), eArmorType( info->ArmorType ), eWeaponAnimationType( info->WeaponAnimation ) );

	DropOptionScriptMap::const_iterator script_it = mDropOptionScriptMap.find( key );

	if( mDropOptionScriptMap.end() == script_it )
	{
		return;
	}

	const DropOptionScript& script = *( script_it->second );

	// ��æƮ ����
	{
		DWORD level = 0;
		{
			DropOptionScript::EnchantLevelMap::const_iterator it = script.mEnchantLevelMap.lower_bound( mRandom.GetFloat() );

			if( script.mEnchantLevelMap.end() != it )
			{
				level = it->second;
			}
		}

		const EnchantScript* enchantScript = GetEnchantScript( eEquipSlot( info->EquipSlot + 1 ) );

		if( level && enchantScript )
		{
			option.mEnchant.mIndex	= enchantScript->mItemIndex;
			option.mEnchant.mLevel	= BYTE( level );

			AddEnchantValue( option, enchantScript->mAbility, info->EnchantDeterm * level );
		}
	}

	// ��� �ɼ� ����
	{
		DropOptionScript::OptionSizeMap::const_iterator size_it = script.mOptionSizeMap.lower_bound( mRandom.GetFloat() );

		if( script.mOptionSizeMap.end() != size_it )
		{
			DWORD size = min( size_it->second, sizeof( option.mDrop.mValue ) / sizeof( *( option.mDrop.mValue ) ) );

			while( size-- )
			{
				DropOptionScript::LevelMap::const_iterator level_it = script.mLevelMap.lower_bound( info->LimitLevel );

				if( script.mLevelMap.end() == level_it )
				{
					continue;
				}

				const DropOptionScript::OptionMap& optionMap = level_it->second;

				DropOptionScript::OptionMap::const_iterator option_it = optionMap.lower_bound( mRandom.GetFloat() );

				if( optionMap.end() == option_it )
				{
					continue;
				}

				const DropOptionScript::Option& scriptOption	= option_it->second;
				const float						gap				= abs( scriptOption.mBeginValue - scriptOption.mEndValue );

				if( ITEM_OPTION::Drop::KeyNone != scriptOption.mKey &&
					0 < gap )
				{
					ITEM_OPTION::Drop::Value& value = option.mDrop.mValue[ size ];

					value.mValue	= Round( scriptOption.mBeginValue + gap * mRandom.GetFloat(), 3 );
					value.mKey		= scriptOption.mKey;
				}
			}
		}
	}
}


void CItemManager::LoadDropOptionScript()
{
	/*
	080130 LUJ,	��ũ��Ʈ�� ���� �ٵ�� �����ȴ�. { �� ������ ������ �о��� �ؽ�Ʈ��
				���� �����ϰ� �Ľ��Ѵ�. }�� ������ ������ �о��� �ؽ�Ʈ�� �ٵ�� �����ϰ�
				��忡 ���� �Ľ��Ѵ�.
	*/
	
	CMHFile file;

	if( ! file.Init( "system/resource/ItemDropOption.bin", "rb" ) )
	{
		return;
	}

	char line[ 1024 ] = { 0 };

	typedef std::list< std::string >	TextList;
	TextList							textList;

	DropOptionScript::OptionMap* optionMap = 0;

	enum ScriptState
	{
		ScriptNone,
		ScriptBody,
		ScriptEnchant,
		ScriptEnchantLevel,
		ScriptOption,
		ScriptOptionSize,
		ScriptOptionLevel,
	}
	scriptState = ScriptNone;

	int openCount = 0;

	DropOptionScript* script = 0;

	while( ! file.IsEOF() )
	{
		file.GetLine( line, sizeof( line ) );

		const int length = strlen( line );

		// �߰��� �ּ� ����
		for( int i = 0; i < length - 1; ++i )
		{
			const char a = line[ i ];
			const char b = line[ i + 1 ];

			if( '/' == a &&
				'/' == b )
			{
				line[ i ] = 0;
				break;
			}
		}

		textList.push_back( line );

		const char* separator	= " \n\t=,+\"~()";
		const char* token		= strtok( line, separator );
		const char* markBegin	= "{";
		const char* markEnd		= "}";

		if( ! token )
		{
			textList.pop_back();
		}
		// ���ο� Ŭ������ ������ ��带 �Ľ��Ѵ�
		else if( 0 == strnicmp( markBegin, token, strlen( markBegin ) ) )
		{
			textList.pop_back();

			++openCount;

			switch( scriptState )
			{
			case ScriptNone:
				{
					{
						mDropOptionScriptList.push_back( DropOptionScript() );

						script = &( mDropOptionScriptList.back() );
						scriptState = ScriptBody;
					}					

					for(
						TextList::const_iterator it = textList.begin();
						textList.end() != it;
						++it )
					{
						eArmorType	armorType	= eArmorType_None;
						eWeaponType	weaponType	= eWeaponType_None;
						eEquipType	equipType	= eEquipType_None;
						eEquipSlot	equipSlot	= eEquipSlot_None;

						SafeStrCpy( line, it->c_str(), sizeof( line ) );

						for(
							token = strtok( line, separator );
							token;
							token = strtok( 0, separator ) )
						{
							if( ! stricmp( token, "sword" ) )
							{
								equipSlot	= eEquipSlot_hand;
								weaponType	= eWeaponType_Sword;

								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( eEquipSlot_hand, eWeaponType_Sword, eArmorType_None, eWeaponAnimationType_None ), script ) );
							}
							else if( ! stricmp( token, "axe" ) )
							{
								equipSlot	= eEquipSlot_hand;
								weaponType	= eWeaponType_Axe;

								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( eEquipSlot_hand, eWeaponType_Axe, eArmorType_None, eWeaponAnimationType_None ), script ) );
							}
							else if( ! stricmp( token, "mace" ) )
							{
								equipSlot	= eEquipSlot_hand;
								weaponType	= eWeaponType_Mace;

								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( eEquipSlot_hand, eWeaponType_Mace, eArmorType_None, eWeaponAnimationType_None ), script ) );
							}
							else if( ! stricmp( token, "dagger" ) )
							{
								equipSlot	= eEquipSlot_hand;
								weaponType	= eWeaponType_Dagger;

								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( eEquipSlot_hand, eWeaponType_Dagger, eArmorType_None, eWeaponAnimationType_None ), script ) );
							}
							else if( ! stricmp( token, "bow" ) )
							{
								equipSlot	= eEquipSlot_hand;
								weaponType	= eWeaponType_Bow;

								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( eEquipSlot_hand, eWeaponType_Bow, eArmorType_None, eWeaponAnimationType_None ), script ) );
							}
							else if( ! stricmp( token, "staff" ) )
							{
								equipSlot	= eEquipSlot_hand;
								weaponType	= eWeaponType_Staff;

								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( eEquipSlot_hand, eWeaponType_Staff, eArmorType_None, eWeaponAnimationType_None ), script ) );
							}
							else if( ! stricmp( token, "armor" ) )
							{
								equipSlot	= eEquipSlot_Armor;

								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( eEquipSlot_Armor, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ), script ) );
							}
							else if( ! stricmp( token, "helmet" ) )
							{
								equipSlot	= eEquipSlot_Helmet;

								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( eEquipSlot_Helmet, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ), script ) );
							}
							else if( ! stricmp( token, "gloves" ) )
							{
								equipSlot	= eEquipSlot_Glove;

								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( eEquipSlot_Glove, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ), script ) );
							}
							else if( ! stricmp( token, "shoes" ) )
							{
								equipSlot	= eEquipSlot_Boots;

								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( eEquipSlot_Boots, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ), script ) );
							}
							else if( ! stricmp( token, "shield" ) )
							{
								equipSlot	= eEquipSlot_Shield;

								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( eEquipSlot_Shield, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ), script ) );
							}
							else if( ! stricmp( token, "ring" ) )
							{
								equipSlot	= eEquipSlot_Ring1;

								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( eEquipSlot_Ring1, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ), script ) );
								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( eEquipSlot_Ring2, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ), script ) );
							}
							else if( ! stricmp( token, "necklace" ) )
							{
								equipSlot	= eEquipSlot_Necklace;

								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( eEquipSlot_Necklace, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ), script ) );
							}
							else if( ! stricmp( token, "earring" ) )
							{
								equipSlot	= eEquipSlot_Earring1;

								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( eEquipSlot_Earring1, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ), script ) );
								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( eEquipSlot_Earring2, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ), script ) );
							}
							else if( ! stricmp( token, "belt" ) )
							{
								equipSlot	= eEquipSlot_Belt;

								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( eEquipSlot_Belt, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ), script ) );
							}
							else if( ! stricmp( token, "band" ) )
							{
								equipSlot	= eEquipSlot_Band;

								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( eEquipSlot_Band, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ), script ) );
							}						
							else if( ! stricmp( token, "glasses" ) )
							{
								equipSlot	= eEquipSlot_Glasses;

								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( eEquipSlot_Glasses, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ), script ) );
							}
							else if( ! stricmp( token, "mask" ) )
							{
								equipSlot	= eEquipSlot_Mask;

								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( eEquipSlot_Mask, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ), script ) );
							}
							else if( ! stricmp( token, "costume_body" ) )
							{
								equipSlot	= eEquipSlot_Costume_Dress;

								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( eEquipSlot_Costume_Dress, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ), script ) );
							}
							else if( ! stricmp( token, "costume_foot" ) )
							{
								equipSlot	= eEquipSlot_Costume_Shoes;

								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( eEquipSlot_Costume_Shoes, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ), script ) );
							}
							else if( ! stricmp( token, "costume_hand" ) )
							{
								equipSlot	= eEquipSlot_Costume_Glove;

								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( eEquipSlot_Costume_Glove, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ), script ) );
							}
							else if( ! stricmp( token, "costume_head" ) )
							{
								equipSlot	= eEquipSlot_Costume_Head;

								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( eEquipSlot_Costume_Head, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ), script ) );
							}
							// 080602 LUJ, ������ '����' ������ ���� �߰�
							else if( ! stricmp( token, "wing" ) )
							{
								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( eEquipSlot_Wing, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ), script ) );
							}
							else if( ! stricmp( token, "one_hand" ) )
							{
								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( equipSlot, weaponType, eArmorType_None, eWeaponAnimationType_OneHand ), script ) );
							}
							else if( ! stricmp( token, "two_hand" ) )
							{
								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( equipSlot, weaponType, eArmorType_None, eWeaponAnimationType_TwoHand ), script ) );
							}
							else if( ! stricmp( token, "dagger" ) )
							{
								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( equipSlot, weaponType, eArmorType_None, eWeaponAnimationType_TwoBlade ), script ) );
							}
							else if( ! stricmp( token, "staff" ) )
							{
								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( equipSlot, weaponType, eArmorType_None, eWeaponAnimationType_Staff ), script ) );
							}
							else if( ! stricmp( token, "bow" ) )
							{
								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( equipSlot, weaponType, eArmorType_None, eWeaponAnimationType_Bow ), script ) );
							}
							else if( ! stricmp( token, "gun" ) )
							{
								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( equipSlot, weaponType, eArmorType_None, eWeaponAnimationType_Gun ), script ) );
							}
							else if( ! stricmp( token, "two_blade" ) )
							{
								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( equipSlot, weaponType, eArmorType_None, eWeaponAnimationType_TwoBlade ), script ) );
							}
							else if( ! stricmp( token, "robe" ) )
							{
								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( equipSlot, weaponType, eArmorType_Robe, eWeaponAnimationType_None ), script ) );
							}
							else if( ! stricmp( token, "light_armor" ) )
							{
								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( equipSlot, weaponType, eArmorType_Leather, eWeaponAnimationType_None ), script ) );
							}
							else if( ! stricmp( token, "heavy_armor" ) )
							{
								mDropOptionScriptMap.insert( std::make_pair( GetItemKey( equipSlot, weaponType, eArmorType_Metal, eWeaponAnimationType_None ), script ) );
							}
							else if( ! stricmp( token, "all" ) )
							{
								switch( equipSlot )
								{
									// ����
								case eEquipSlot_hand:
									{
										mDropOptionScriptMap.insert( std::make_pair( GetItemKey( eEquipSlot_hand, weaponType, eArmorType_None, eWeaponAnimationType_OneHand ), script ) );
										mDropOptionScriptMap.insert( std::make_pair( GetItemKey( eEquipSlot_hand, weaponType, eArmorType_None, eWeaponAnimationType_TwoHand ), script ) );
										mDropOptionScriptMap.insert( std::make_pair( GetItemKey( eEquipSlot_hand, weaponType, eArmorType_None, eWeaponAnimationType_TwoBlade ), script ) );
										mDropOptionScriptMap.insert( std::make_pair( GetItemKey( eEquipSlot_hand, weaponType, eArmorType_None, eWeaponAnimationType_Bow ), script ) );
										mDropOptionScriptMap.insert( std::make_pair( GetItemKey( eEquipSlot_hand, weaponType, eArmorType_None, eWeaponAnimationType_Staff ), script ) );
										mDropOptionScriptMap.insert( std::make_pair( GetItemKey( eEquipSlot_hand, weaponType, eArmorType_None, eWeaponAnimationType_Gun ), script ) );
										mDropOptionScriptMap.insert( std::make_pair( GetItemKey( eEquipSlot_hand, weaponType, eArmorType_None, eWeaponAnimationType_Dagger ), script ) );
										break;
									}
								case eEquipSlot_Armor:
								case eEquipSlot_Helmet:
								case eEquipSlot_Glove:
								case eEquipSlot_Boots:								
									{
										mDropOptionScriptMap.insert( std::make_pair( GetItemKey( equipSlot, eWeaponType_None, eArmorType_Robe, eWeaponAnimationType_None ), script ) );
										mDropOptionScriptMap.insert( std::make_pair( GetItemKey( equipSlot, eWeaponType_None, eArmorType_Leather, eWeaponAnimationType_None ), script ) );
										mDropOptionScriptMap.insert( std::make_pair( GetItemKey( equipSlot, eWeaponType_None, eArmorType_Metal, eWeaponAnimationType_None ), script ) );
										break;
									}
								}
							}
						}
					}					

					scriptState = ScriptBody;
					break;
				}
			case ScriptBody:
				{
					for(
						TextList::const_iterator it = textList.begin();
						textList.end() != it;
						++it )
					{
						SafeStrCpy( line, it->c_str(), sizeof( line ) );

						token = strtok( line, separator );

						if( ! token )
						{
							continue;
						}
						else if( ! stricmp( token, "enchant" ) )
						{
							scriptState = ScriptEnchant;
						}
						else if( ! stricmp( token, "option") )
						{
							scriptState	= ScriptOption;
						}
					}

					break;
				}
			case ScriptEnchant:
				{
					for(
						TextList::const_iterator it = textList.begin();
						textList.end() != it;
						++it )
					{
						SafeStrCpy( line, it->c_str(), sizeof( line ) );

						token = strtok( line, separator );

						if( ! token )
						{
							continue;
						}
						else if( ! stricmp( token, "size" ) )
						{
							scriptState = ScriptEnchantLevel;
						}
					}

					break;
				}
			case ScriptOption:
				{
					for(
						TextList::const_iterator it = textList.begin();
						textList.end() != it;
						++it )
					{
						SafeStrCpy( line, it->c_str(), sizeof( line ) );

						token = strtok( line, separator );

						if( ! token )
						{
							continue;
						}
						else if( ! stricmp( token, "size" ) )
						{
							scriptState = ScriptOptionSize;
						}
						else if( ! stricmp( token, "level" ) )
						{
							scriptState = ScriptOptionLevel;

							const char* minLevel	= strtok( 0, separator );
							const char* maxLevel	= strtok( 0, separator );

							if( ! minLevel ||
								! maxLevel )
							{
								break;
							}

							optionMap = &( script->mLevelMap[ atoi( maxLevel ) ] );
						}
					}

					break;
				}
			}

			textList.clear();
		}
		else if( 0 == strnicmp( markEnd, token, strlen( markEnd ) ) )
		{
			textList.pop_back();

			--openCount;

			switch( scriptState )
			{
			case ScriptBody:
				{					
					script		= 0;
					scriptState	= ScriptNone;
					
					break;
				}
			case ScriptEnchant:
			case ScriptOption:
				{
					scriptState = ScriptBody;
					break;
				}
			case ScriptEnchantLevel:
				{
					scriptState = ScriptEnchant;

					float step = 0.0f;

					for(
						TextList::const_iterator it = textList.begin();
						textList.end() != it;
						++it )
					{
						SafeStrCpy( line, it->c_str(),  sizeof( line ) );

						const char* level	= strtok( line, separator );
						const float rate	= float( atof( strtok( 0, separator ) ) );

						if( ! level ||
							! rate )
						{
							continue;
						}

						step = step + rate / 100.0f;

						script->mEnchantLevelMap.insert( std::make_pair( step, atoi( level ) ) );
					}

					break;
				}
			case ScriptOptionSize:
				{
					scriptState = ScriptOption;

					float step = 0.0f;

					for(
						TextList::const_iterator it = textList.begin();
						textList.end() != it;
						++it )
					{
						SafeStrCpy( line, it->c_str(),  sizeof( line ) );

						const char* size	= strtok( line, separator );
						const float rate	= float( atof( strtok( 0, separator ) ) );

						if( ! size ||
							! rate )
						{
							continue;
						}

						step = step + rate / 100.0f;

						script->mOptionSizeMap.insert( std::make_pair( step, atoi( size ) ) );
					}

					break;
				}
			case ScriptOptionLevel:
				{
					scriptState = ScriptOption;

					DropOptionScript::Option option;
					ZeroMemory( &option, sizeof( option ) );

					float step = 0;

					for(
						TextList::const_iterator it = textList.begin();
						textList.end() != it;
						++it )
					{
						SafeStrCpy( line, it->c_str(),  sizeof( line ) );

						const char*	type		= strtok( line, separator );
						const float rate		= float( atof( strtok( 0,	separator ) ) );
						const char* beginValue	= strtok( 0,	separator );
						const char* endValue	= strtok( 0,	separator );

						if( ! type		||
							! rate		||
							! beginValue	||
							! endValue	)
						{
							continue;
						}
						
						step = step + rate / 100.0f;

						{							
							if( '%' == beginValue[ strlen( beginValue ) - 1 ] )
							{
								option.mBeginValue	= float( atof( beginValue ) ) / 100.0f;
							}
							else
							{
								option.mBeginValue	= float( atof( beginValue ) );
							}

							if( '%' == endValue[ strlen( endValue ) - 1 ] )
							{
								option.mEndValue	= float( atof( endValue ) ) / 100.0F;
							}
							else
							{
								option.mEndValue	= float( atof( endValue ) );
							}
						}						
						
						if( ! stricmp( type, "plus_strength" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusStrength;
						}
						else if( ! stricmp( type, "plus_intelligence" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusIntelligence;
						}
						else if( ! stricmp( type, "plus_dexterity" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusDexterity;
						}
						else if( ! stricmp( type, "plus_wisdom" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusWisdom;
						}
						else if( ! stricmp( type, "plus_vitality" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusVitality;
						}
						else if( ! stricmp( type, "plus_physic_attack" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusPhysicalAttack;
						}
						else if( ! stricmp( type, "plus_physic_defence" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusPhysicalDefence;
						}
						else if( ! stricmp( type, "plus_magic_attack" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusMagicalAttack;
						}
						else if( ! stricmp( type, "plus_magic_defence" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusMagicalDefence;
						}
						else if( ! stricmp( type, "plus_critical_rate" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusCriticalRate;
						}
						else if( ! stricmp( type, "plus_critical_damage" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusCriticalDamage;
						}
						else if( ! stricmp( type, "plus_accuracy" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusAccuracy;
						}
						else if( ! stricmp( type, "plus_evade" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusEvade;
						}
						else if( ! stricmp( type, "plus_move_speed" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusMoveSpeed;
						}
						else if( ! stricmp( type, "plus_life" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusLife;
						}
						else if( ! stricmp( type, "plus_mana" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusMana;
						}
						else if( ! stricmp( type, "plus_recovery_life" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusLifeRecovery;
						}
						else if( ! stricmp( type, "plus_recovery_mana" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusManaRecovery;
						}
						else if( ! stricmp( type, "percent_strength" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentStrength;
						}
						else if( ! stricmp( type, "percent_intelligence" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentIntelligence;
						}
						else if( ! stricmp( type, "percent_dexterity" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentDexterity;
						}
						else if( ! stricmp( type, "percent_wisdom" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentWisdom;
						}
						else if( ! stricmp( type, "percent_vitality" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentVitality;
						}
						else if( ! stricmp( type, "percent_physic_attack" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentPhysicalAttack;
						}
						else if( ! stricmp( type, "percent_physic_defence" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentPhysicalDefence;
						}
						else if( ! stricmp( type, "percent_magic_attack" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentMagicalAttack;
						}
						else if( ! stricmp( type, "percent_magic_defence" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentMagicalDefence;
						}
						else if( ! stricmp( type, "percent_critical_rate" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentCriticalRate;
						}
						else if( ! stricmp( type, "percent_critical_damage" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentCriticalDamage;
						}
						else if( ! stricmp( type, "percent_accuracy" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentAccuracy;
						}
						else if( ! stricmp( type, "percent_evade" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentEvade;
						}
						else if( ! stricmp( type, "percent_move_speed" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentMoveSpeed;
						}
						else if( ! stricmp( type, "percent_life" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentLife;
						}
						else if( ! stricmp( type, "percent_mana" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentMana;
						}
						else if( ! stricmp( type, "percent_recovery_life" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentLifeRecovery;
						}
						else if( ! stricmp( type, "percent_recovery_mana" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentManaRecovery;
						}

						optionMap->insert( std::make_pair( step, option ) );
					}

					break;
				}
			}

			textList.clear();
		}
	}

	if( openCount )
	{
		ASSERT( 0 );
	}

	//// 080221 LUJ, �ε��� �ɼ� ǥ��
	//for(
	//	DropOptionScriptList::const_iterator script_it = mDropOptionScriptList.begin();
	//	mDropOptionScriptList.end() != script_it;
	//	++script_it )
	//{
	//	const DropOptionScript& script = *script_it;

	//	// option size map
	//	{
	//		cprintf( "option size\n" );
	//		cprintf( "\t rate \t size \n" );

	//		for(
	//			DropOptionScript::OptionSizeMap::const_iterator it = script.mOptionSizeMap.begin();
	//			script.mOptionSizeMap.end() != it;
	//			++it )
	//		{
	//			cprintf( "\t %f \t %d\n", it->first, it->second );
	//		}

	//		cprintf( "\n" );
	//	}
	//	
	//	// option map as level
	//	{
	//		for(
	//			DropOptionScript::LevelMap::const_iterator level_it = script.mLevelMap.begin();
	//			script.mLevelMap.end() != level_it;
	//			++level_it )
	//		{
	//			const DropOptionScript::OptionMap& optionMap = level_it->second;
	//			
	//			cprintf( "level: %d\n", level_it->first );

	//			for(
	//				DropOptionScript::OptionMap::const_iterator it = optionMap.begin();
	//				optionMap.end() != it;
	//				++it )
	//			{
	//				const DropOptionScript::Option& option = it->second;

	//				cprintf( "\t %f \t %d: %f ~ %f\n", it->first, option.mKey, option.mBeginValue, option.mEndValue );
	//			}

	//			cprintf( "\n" );
	//		}
	//	}
	//}
}


void CItemManager::LoadApplyOptionScript()
{
	/*
	080215 LUJ, ��ũ��Ʈ �Ľ� ����� LoadDropOptionScrit() ����
	*/

	CMHFile file;

	if( ! file.Init( "system/resource/ItemApplyOption.bin", "rb" ) )
	{
		return;
	}

	char line[ 1024 ] = { 0 };

	typedef std::list< std::string >	TextList;
	TextList							textList;

	enum ScriptState
	{
		ScriptNone,
		ScriptItem,
		ScriptTarget,
		ScriptTargetLevel,
		ScriptTargetType,
		ScriptOption,
		ScriptOptionSize,
		ScriptOptionType,
	}
	scriptState = ScriptNone;

	int openCount = 0;

	ApplyOptionScript* script = 0;

	while( ! file.IsEOF() )
	{
		file.GetLine( line, sizeof( line ) );

		const int length = strlen( line );

		// �߰��� �ּ� ����
		for( int i = 0; i < length - 1; ++i )
		{
			const char a = line[ i ];
			const char b = line[ i + 1 ];

			if( '/' == a &&
				'/' == b )
			{
				line[ i ] = 0;
				break;
			}
		}

		textList.push_back( line );

		const char* separator	= " \n\t=,+\"~()";
		const char* token		= strtok( line, separator );
		const char* markBegin	= "{";
		const char* markEnd		= "}";

		if( ! token )
		{
			textList.pop_back();
		}
		// ���ο� Ŭ������ ������ ��带 �Ľ��Ѵ�
		else if( 0 == strnicmp( markBegin, token, strlen( markBegin ) ) )
		{
			textList.pop_back();

			switch( scriptState )
			{
			case ScriptNone:
				{
					++openCount;

					scriptState = ScriptItem;

					for(
						TextList::const_iterator it = textList.begin();
						textList.end() != it;
						++it )
					{
						SafeStrCpy( line, it->c_str(), sizeof( line ) );

						token = strtok( line, separator );

						if( ! stricmp( token, "item" ) )
						{
							const char* itemIndex = strtok( 0, separator );

							script = &( mApplyOptionScriptMap[ atoi( itemIndex ) ] );

							ZeroMemory( &( script->mLevel ), sizeof( script->mLevel ) );
						}
					}

					break;
				}
			case ScriptItem:
				{
					for(
						TextList::const_iterator it = textList.begin();
						textList.end() != it;
						++it )
					{
						SafeStrCpy( line, it->c_str(), sizeof( line ) );

						token = strtok( line, separator );

						if( ! stricmp( token, "target" ) )
						{
							scriptState = ScriptTarget;
						}
						else if( ! stricmp( token, "option" ) )
						{
							scriptState = ScriptOption;
						}
					}

					break;
				}
			case ScriptTarget:
				{
					for(
						TextList::const_iterator it = textList.begin();
						textList.end() != it;
						++it )
					{
						SafeStrCpy( line, it->c_str(), sizeof( line ) );

						token = strtok( line, separator );

						if( ! stricmp( token, "level" ) )
						{
							scriptState = ScriptTargetLevel;
						}
						else if( ! stricmp( token, "type" ) )
						{
							scriptState = ScriptTargetType;
						}
					}

					break;
				}
			case ScriptOption:
				{
					for(
						TextList::const_iterator it = textList.begin();
						textList.end() != it;
						++it )
					{
						SafeStrCpy( line, it->c_str(), sizeof( line ) );

						token = strtok( line, separator );

						if( ! stricmp( token, "size" ) )
						{
							scriptState = ScriptOptionSize;
						}
						else if( ! stricmp( token, "type" ) )
						{
							scriptState = ScriptOptionType;
						}
					}

					break;
				}
			}

			textList.clear();
		}
		else if( 0 == strnicmp( markEnd, token, strlen( markEnd ) ) )
		{
			textList.pop_back();

			--openCount;

			switch( scriptState )
			{
			case ScriptItem:
				{
					script		= 0;
					scriptState	= ScriptNone;
					break;
				}
			case ScriptTarget:
				{
					scriptState	= ScriptItem;
					break;
				}
			case ScriptOption:
				{
					scriptState = ScriptItem;
					break;
				}
			case ScriptTargetLevel:
				{
					scriptState	= ScriptTarget;

					for(
						TextList::const_iterator it = textList.begin();
						textList.end() != it;
						++it )
					{
						SafeStrCpy( line, it->c_str(),  sizeof( line ) );

						script->mLevel.mMin	= atoi( strtok( line, separator ) );
						script->mLevel.mMax	= atoi( strtok( 0, separator ) );
					}

					break;
				}
			case ScriptTargetType:
				{
					scriptState	= ScriptTarget;

					for(
						TextList::const_iterator it = textList.begin();
						textList.end() != it;
						++it )
					{
						eArmorType	armorType	= eArmorType_None;
						eWeaponType	weaponType	= eWeaponType_None;
						eEquipType	equipType	= eEquipType_None;
						eEquipSlot	equipSlot	= eEquipSlot_None;

						SafeStrCpy( line, it->c_str(), sizeof( line ) );

						for(
							token = strtok( line, separator );
							token;
							token = strtok( 0, separator ) )
						{
							if( ! stricmp( token, "sword" ) )
							{
								equipSlot	= eEquipSlot_hand;
								weaponType	= eWeaponType_Sword;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_hand, eWeaponType_Sword, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "axe" ) )
							{
								equipSlot	= eEquipSlot_hand;
								weaponType	= eWeaponType_Axe;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_hand, eWeaponType_Axe, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "mace" ) )
							{
								equipSlot	= eEquipSlot_hand;
								weaponType	= eWeaponType_Mace;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_hand, eWeaponType_Mace, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "dagger" ) )
							{
								equipSlot	= eEquipSlot_hand;
								weaponType	= eWeaponType_Dagger;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_hand, eWeaponType_Dagger, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "bow" ) )
							{
								equipSlot	= eEquipSlot_hand;
								weaponType	= eWeaponType_Bow;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_hand, eWeaponType_Bow, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "staff" ) )
							{
								equipSlot	= eEquipSlot_hand;
								weaponType	= eWeaponType_Staff;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_hand, eWeaponType_Staff, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "armor" ) )
							{
								equipSlot	= eEquipSlot_Armor;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Armor, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "helmet" ) )
							{
								equipSlot	= eEquipSlot_Helmet;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Helmet, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "gloves" ) )
							{
								equipSlot	= eEquipSlot_Glove;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Glove, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "shoes" ) )
							{
								equipSlot	= eEquipSlot_Boots;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Boots, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "shield" ) )
							{
								equipSlot	= eEquipSlot_Shield;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Shield, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "ring" ) )
							{
								equipSlot	= eEquipSlot_Ring1;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Ring1, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Ring2, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "necklace" ) )
							{
								equipSlot	= eEquipSlot_Necklace;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Necklace, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "earring" ) )
							{
								equipSlot	= eEquipSlot_Earring1;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Earring1, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Earring2, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "belt" ) )
							{
								equipSlot	= eEquipSlot_Belt;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Belt, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "band" ) )
							{
								equipSlot	= eEquipSlot_Band;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Band, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}						
							else if( ! stricmp( token, "glasses" ) )
							{
								equipSlot	= eEquipSlot_Glasses;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Glasses, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "mask" ) )
							{
								equipSlot	= eEquipSlot_Mask;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Mask, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "costume_body" ) )
							{
								equipSlot	= eEquipSlot_Costume_Dress;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Costume_Dress, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "costume_foot" ) )
							{
								equipSlot	= eEquipSlot_Costume_Shoes;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Costume_Shoes, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "costume_hand" ) )
							{
								equipSlot	= eEquipSlot_Costume_Glove;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Costume_Glove, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "costume_head" ) )
							{
								equipSlot	= eEquipSlot_Costume_Head;

								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Costume_Head, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}
							// 080602 LUJ, ������ '����' ������ ���� �߰�
							else if( ! stricmp( token, "wing" ) )
							{
								script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_Wing, eWeaponType_None, eArmorType_None, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "one_hand" ) )
							{
								script->mTargetTypeSet.insert( GetItemKey( equipSlot, weaponType, eArmorType_None, eWeaponAnimationType_OneHand ) );
							}
							else if( ! stricmp( token, "two_hand" ) )
							{
								script->mTargetTypeSet.insert( GetItemKey( equipSlot, weaponType, eArmorType_None, eWeaponAnimationType_TwoHand ) );
							}
							else if( ! stricmp( token, "dagger" ) )
							{
								script->mTargetTypeSet.insert( GetItemKey( equipSlot, weaponType, eArmorType_None, eWeaponAnimationType_TwoBlade ) );
							}
							else if( ! stricmp( token, "staff" ) )
							{
								script->mTargetTypeSet.insert( GetItemKey( equipSlot, weaponType, eArmorType_None, eWeaponAnimationType_Staff ) );
							}
							else if( ! stricmp( token, "bow" ) )
							{
								script->mTargetTypeSet.insert( GetItemKey( equipSlot, weaponType, eArmorType_None, eWeaponAnimationType_Bow ) );
							}
							else if( ! stricmp( token, "gun" ) )
							{
								script->mTargetTypeSet.insert( GetItemKey( equipSlot, weaponType, eArmorType_None, eWeaponAnimationType_Gun ) );
							}
							else if( ! stricmp( token, "two_blade" ) )
							{
								script->mTargetTypeSet.insert( GetItemKey( equipSlot, weaponType, eArmorType_None, eWeaponAnimationType_TwoBlade ) );
							}
							else if( ! stricmp( token, "robe" ) )
							{
								script->mTargetTypeSet.insert( GetItemKey( equipSlot, weaponType, eArmorType_Robe, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "light_armor" ) )
							{
								script->mTargetTypeSet.insert( GetItemKey( equipSlot, weaponType, eArmorType_Leather, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "heavy_armor" ) )
							{
								script->mTargetTypeSet.insert( GetItemKey( equipSlot, weaponType, eArmorType_Metal, eWeaponAnimationType_None ) );
							}
							else if( ! stricmp( token, "all" ) )
							{
								switch( equipSlot )
								{
									// ����
								case eEquipSlot_hand:
									{
										script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_hand, weaponType, eArmorType_None, eWeaponAnimationType_OneHand ) );
										script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_hand, weaponType, eArmorType_None, eWeaponAnimationType_TwoHand ) );
										script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_hand, weaponType, eArmorType_None, eWeaponAnimationType_TwoBlade ) );
										script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_hand, weaponType, eArmorType_None, eWeaponAnimationType_Bow ) );
										script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_hand, weaponType, eArmorType_None, eWeaponAnimationType_Staff ) );
										script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_hand, weaponType, eArmorType_None, eWeaponAnimationType_Gun ) );
										script->mTargetTypeSet.insert( GetItemKey( eEquipSlot_hand, weaponType, eArmorType_None, eWeaponAnimationType_Dagger ) );
										break;
									}
								case eEquipSlot_Armor:
								case eEquipSlot_Helmet:
								case eEquipSlot_Glove:
								case eEquipSlot_Boots:								
									{
										script->mTargetTypeSet.insert( GetItemKey( equipSlot, eWeaponType_None, eArmorType_Robe, eWeaponAnimationType_None ) );
										script->mTargetTypeSet.insert( GetItemKey( equipSlot, eWeaponType_None, eArmorType_Leather, eWeaponAnimationType_None ) );
										script->mTargetTypeSet.insert( GetItemKey( equipSlot, eWeaponType_None, eArmorType_Metal, eWeaponAnimationType_None ) );
										break;
									}
								}
							}
						}
					}					

					break;
				}
			case ScriptOptionSize:
				{
					scriptState	= ScriptOption;

					float step = 0.0f;

					for(
						TextList::const_iterator it = textList.begin();
						textList.end() != it;
						++it )
					{
						SafeStrCpy( line, it->c_str(),  sizeof( line ) );

						const char* size	= strtok( line, separator );
						const float rate	= float( atof( strtok( 0, separator ) ) );

						if( ! size ||
							! rate )
						{
							continue;
						}

						step = step + rate / 100.0f;

						script->mOptionSizeMap.insert( std::make_pair( step, atoi( size ) ) );
					}

					break;
				}
			case ScriptOptionType:
				{
					scriptState = ScriptOption;

					ApplyOptionScript::Option option;
					ZeroMemory( &option, sizeof( option ) );

					float step = 0;

					for(
						TextList::const_iterator it = textList.begin();
						textList.end() != it;
						++it )
					{
						SafeStrCpy( line, it->c_str(),  sizeof( line ) );

						const char* type		= strtok( line, separator );
						const float rate		= float( atof( strtok( 0,	separator ) ) );
						const char* beginValue	= strtok( 0,	separator );
						const char* endValue	= strtok( 0,	separator );

						if( ! type		||
							! rate		||
							! beginValue	||
							! endValue	)
						{
							continue;
						}

						step = step + rate / 100.0f;

						{							
							if( '%' == beginValue[ strlen( beginValue ) - 1 ] )
							{
								option.mBeginValue	= float( atof( beginValue ) ) / 100.0f;
							}
							else
							{
								option.mBeginValue	= float( atof( beginValue ) );
							}

							if( '%' == endValue[ strlen( endValue ) - 1 ] )
							{
								option.mEndValue	= float( atof( endValue ) ) / 100.0F;
							}
							else
							{
								option.mEndValue	= float( atof( endValue ) );
							}
						}						

						if( ! stricmp( type, "plus_strength" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusStrength;
						}
						else if( ! stricmp( type, "plus_intelligence" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusIntelligence;
						}
						else if( ! stricmp( type, "plus_dexterity" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusDexterity;
						}
						else if( ! stricmp( type, "plus_wisdom" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusWisdom;
						}
						else if( ! stricmp( type, "plus_vitality" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusVitality;
						}
						else if( ! stricmp( type, "plus_physic_attack" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusPhysicalAttack;
						}
						else if( ! stricmp( type, "plus_physic_defence" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusPhysicalDefence;
						}
						else if( ! stricmp( type, "plus_magic_attack" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusMagicalAttack;
						}
						else if( ! stricmp( type, "plus_magic_defence" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusMagicalDefence;
						}
						else if( ! stricmp( type, "plus_critical_rate" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusCriticalRate;
						}
						else if( ! stricmp( type, "plus_critical_damage" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusCriticalDamage;
						}
						else if( ! stricmp( type, "plus_accuracy" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusAccuracy;
						}
						else if( ! stricmp( type, "plus_evade" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusEvade;
						}
						else if( ! stricmp( type, "plus_move_speed" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusMoveSpeed;
						}
						else if( ! stricmp( type, "plus_life" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusLife;
						}
						else if( ! stricmp( type, "plus_mana" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusMana;
						}
						else if( ! stricmp( type, "plus_recovery_life" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusLifeRecovery;
						}
						else if( ! stricmp( type, "plus_recovery_mana" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPlusManaRecovery;
						}
						else if( ! stricmp( type, "percent_strength" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentStrength;
						}
						else if( ! stricmp( type, "percent_intelligence" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentIntelligence;
						}
						else if( ! stricmp( type, "percent_dexterity" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentDexterity;
						}
						else if( ! stricmp( type, "percent_wisdom" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentWisdom;
						}
						else if( ! stricmp( type, "percent_vitality" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentVitality;
						}
						else if( ! stricmp( type, "percent_physic_attack" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentPhysicalAttack;
						}
						else if( ! stricmp( type, "percent_physic_defence" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentPhysicalDefence;
						}
						else if( ! stricmp( type, "percent_magic_attack" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentMagicalAttack;
						}
						else if( ! stricmp( type, "percent_magic_defence" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentMagicalDefence;
						}
						else if( ! stricmp( type, "percent_critical_rate" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentCriticalRate;
						}
						else if( ! stricmp( type, "percent_critical_damage" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentCriticalDamage;
						}
						else if( ! stricmp( type, "percent_accuracy" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentAccuracy;
						}
						else if( ! stricmp( type, "percent_evade" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentEvade;
						}
						else if( ! stricmp( type, "percent_move_speed" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentMoveSpeed;
						}
						else if( ! stricmp( type, "percent_life" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentLife;
						}
						else if( ! stricmp( type, "percent_mana" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentMana;
						}
						else if( ! stricmp( type, "percent_recovery_life" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentLifeRecovery;
						}
						else if( ! stricmp( type, "percent_recovery_mana" ) )
						{
							option.mKey = ITEM_OPTION::Drop::KeyPercentManaRecovery;
						}

						script->mOptionTypeMap.insert( std::make_pair( step, option ) );
					}

					break;
				}
			}

			textList.clear();
		}
	}

	if( openCount )
	{
		ASSERT( 0 );
	}

	//for(
	//	ApplyOptionScriptMap::const_iterator script_it = mApplyOptionScriptMap.begin();
	//	mApplyOptionScriptMap.end() != script_it;
	//	++script_it )
	//{
	//	const DWORD					itemIndex	= script_it->first;
	//	const ApplyOptionScript&	script		= script_it->second;

	//	cprintf( "itemIndex: %d\n", itemIndex );
	//	cprintf( "\t target level: %d ~ %d\n", script.mLevel.mMin, script.mLevel.mMax );

	//	/*
	//	cprintf( "\t target type:\n" );
 //       
	//	for(
	//		ApplyOptionScript::TargetTypeSet::const_iterator it = script.mTargetTypeSet.begin();
	//		script.mTargetTypeSet.end() != it;
	//		++it )
	//	{
	//		cprintf( "\t\t %u\t", *it );
	//	}
	//	*/

	//	cprintf( "\n" );
	//	cprintf( "\t option size:\n" );

	//	for(
	//		ApplyOptionScript::OptionSizeMap::const_iterator it = script.mOptionSizeMap.begin();
	//		script.mOptionSizeMap.end() != it;
	//		++it )
	//	{
	//		cprintf( "\t\t %f\t %d\n", it->first, it->second );
	//	}

	//	cprintf( "\n" );
	//	cprintf( "\t option rate:\n" );
	//	cprintf( "\t\t rate\t key\t min ~ max:\n" );

	//	for(
	//		ApplyOptionScript::OptionTypeMap::const_iterator it = script.mOptionTypeMap.begin();
	//		script.mOptionTypeMap.end() != it;
	//		++it )
	//	{
	//		const ApplyOptionScript::Option& option = it->second;

	//		cprintf(
	//			"\t\t %f\t %d\t %f~%f\n",
	//			it->first,
	//			option.mKey,
	//			option.mBeginValue,
	//			option.mEndValue );
	//	}
	//}
}


LONGLONG CItemManager::GetItemKey( eEquipSlot slot, eWeaponType weapon, eArmorType armor, eWeaponAnimationType animation ) const
{
	const short key1	= short( slot );
	const short key2	= short( weapon );
	const short key3	= short( armor );
	const short key4	= short( animation );

	LONGLONG key = 0;

	ASSERT( sizeof( key ) == sizeof( key1 ) + sizeof( key2 ) + sizeof( key3 ) + sizeof( key4 ) );

	memcpy(	( short* )&key + 0, &key1, sizeof( key1 ) );
	memcpy(	( short* )&key + 1, &key2, sizeof( key2 ) );
	memcpy(	( short* )&key + 2, &key3, sizeof( key3 ) );
	memcpy(	( short* )&key + 3, &key4, sizeof( key4 ) );

	return key;
}


void CItemManager::ApplyOption( const ITEMBASE& sourceItem, const ITEMBASE& targetItem, ITEM_OPTION& option )
{
	/*
	�ҽ� �������� �̿��Ͽ� ��� �����ۿ� �ɼ��� �����Ѵ�
	*/
	ApplyOptionScriptMap::const_iterator script_it = mApplyOptionScriptMap.find( sourceItem.wIconIdx );

	if( mApplyOptionScriptMap.end() == script_it )
	{
		return;
	}

	const ApplyOptionScript& script = script_it->second;

	const ITEM_INFO* info = GetItemInfo( targetItem.wIconIdx );

	if( !	info ||
			eEquipType_None == info->EquipType )
	{
		return;
	}

	// ���� ������ ���� �����ΰ�
	if( script.mLevel.mMin > info->LimitLevel ||
		script.mLevel.mMax < info->LimitLevel )
	{
		return;
	}

	// ���� ������ Ÿ������ üũ�Ѵ�
	{
		const LONGLONG key = GetItemKey(
			eEquipSlot( info->EquipSlot + 1 ),
			eWeaponType( info->WeaponType ),
			eArmorType( info->ArmorType ),
			eWeaponAnimationType( info->WeaponAnimation ) );

		if( script.mTargetTypeSet.end() == script.mTargetTypeSet.find( key ) )
		{
			return;
		}
	}
	
	int size = 1;

	// �ɼ� ������ ���´�. �ּ� 1��
	{
		ApplyOptionScript::OptionSizeMap::const_iterator it = script.mOptionSizeMap.lower_bound( mRandom.GetFloat() );

		if( script.mOptionSizeMap.end() != it )
		{
			size = max( 1, it->second );
		}
	}
	
	// �ɼ��� �����ϰ� �ο��Ѵ�
	while( size-- )
	{
		const ApplyOptionScript::Option* scriptOption = 0;

		// ��� �ɼ� ����
		do 
		{
			ApplyOptionScript::OptionTypeMap::const_iterator it = script.mOptionTypeMap.lower_bound( mRandom.GetFloat() );

            if( script.mOptionTypeMap.end() != it )
			{
				scriptOption = &( it->second );
			}
		}
		while( !	scriptOption ||
					scriptOption->mKey == ITEM_OPTION::Drop::KeyNone );

		ITEM_OPTION::Drop::Value& value = option.mDrop.mValue[ size ];

		const float gap = abs( scriptOption->mBeginValue - scriptOption->mEndValue );

		// 080407 LUJ, Ư�� �Ҽ��� �Ʒ��� �����Ͽ� �Ǽ� ������ ���Ѵ�
		value.mValue	= Round( scriptOption->mBeginValue + gap * mRandom.GetFloat(), 3 );
		value.mKey		= scriptOption->mKey;
	}	
}


// 080228 LUJ, ���� ��ȣ ������ ����
BOOL CItemManager::IsMixProtectionItem( const ITEMBASE& item ) const
{
	return mMixProtectionItemSet.end() != mMixProtectionItemSet.find( item.wIconIdx );
}


// 080228 LUJ, ��ȭ ��ȣ ������ ����
BOOL CItemManager::IsReinforceProtectionItem( const ITEMBASE& item ) const
{
	return mReinforceProtectionItemSet.end() != mReinforceProtectionItemSet.find( item.wIconIdx );
}

// 080414 LUJ, �ܾ� ��ȯ ��ũ��Ʈ�� �д´�
void CItemManager::LoadBodyChangeScript()
{
	CMHFile file;

	if( ! file.Init( "system/resource/ItemBodyChange.bin", "rb" ) )
	{
		return;
	}

	char line[ MAX_PATH * 5 ] = { 0 };

	enum ScriptState
	{
		StateNone,
		StateItem,
		StateRace,
		StatePart,
		StateGender,
	}
	scriptState = StateNone;

	int						openCount	= 0;
	BodyChangeScriptList*	scriptList	= 0;
	BodyChangeScript		script		= { 0 };

	typedef std::list< std::string >	TextList;
	TextList							textList;

	while( ! file.IsEOF() )
	{
		file.GetLine( line, sizeof( line ) );

		const int length = strlen( line );

		// �߰��� �ּ� ����
		for( int i = 0; i < length - 1; ++i )
		{
			const char a = line[ i ];
			const char b = line[ i + 1 ];

			if( '/' == a &&
				'/' == b )
			{
				line[ i ] = 0;
				break;
			}
		}

		textList.push_back( line );

		const char* separator	= " \n\t=,+\"~()";
		const char* token		= strtok( line, separator );
		const char* markBegin	= "{";
		const char* markEnd		= "}";

		if( ! token )
		{
			textList.pop_back();
		}
		// ���ο� Ŭ������ ������ ��带 �Ľ��Ѵ�
		else if( 0 == strnicmp( markBegin, token, strlen( markBegin ) ) )
		{
			textList.pop_back();

			switch( scriptState )
			{
			case StateNone:
				{
					++openCount;

					scriptState = StateItem;

					for(
						TextList::const_iterator it = textList.begin();
						textList.end() != it;
					++it )
					{
						SafeStrCpy( line, it->c_str(), sizeof( line ) );

						token = strtok( line, separator );

						if( ! stricmp( token, "item" ) )
						{
							const char* itemIndex = strtok( 0, separator );

							scriptList = &( mBodyChangeScriptListMap[ atoi( itemIndex ) ] );
						}
					}

					break;
				}
			case StateItem:
				{
					for(
						TextList::const_iterator it = textList.begin();
						textList.end() != it;
					++it )
					{
						SafeStrCpy( line, it->c_str(), sizeof( line ) );

						token = strtok( line, separator );

						if( ! stricmp( token, "elf" ) )
						{
							scriptState		= StateRace;
							script.mRace	= RaceType_Elf;
						}
						else if( ! stricmp( token, "human" ) )
						{
							scriptState		= StateRace;
							script.mRace	= RaceType_Human;
						}
					}

					break;
				}
			case StateRace:
				{
					for(
						TextList::const_iterator it = textList.begin();
						textList.end() != it;
					++it )
					{
						SafeStrCpy( line, it->c_str(), sizeof( line ) );

						token = strtok( line, separator );

						if( ! stricmp( token, "hair" ) )
						{
							scriptState		= StatePart;							
							script.mPart	= ePartType_Hair;
						}
						else if( ! stricmp( token, "face" ) )
						{
							scriptState		= StatePart;							
							script.mPart	= ePartType_Face;
						}
					}

					break;
				}
			case StatePart:
				{
					for(
						TextList::const_iterator it = textList.begin();
						textList.end() != it;
					++it )
					{
						SafeStrCpy( line, it->c_str(), sizeof( line ) );

						token = strtok( line, separator );

						if( ! stricmp( token, "female" ) )
						{
							scriptState		= StateGender;							
							script.mGender	= GENDER_FEMALE;
						}
						else if( ! stricmp( token, "male" ) )
						{
							scriptState		= StateGender;							
							script.mGender	= GENDER_MALE;
						}
					}

					break;
				}				
			}

			// 080511 LUJ, ��ũ��Ʈ�� �ùٷ� �Ľ̵ǵ��� ����
			textList.clear();
		}
		else if( 0 == strnicmp( markEnd, token, strlen( markEnd ) ) )
		{
			textList.pop_back();

			--openCount;

			switch( scriptState )
			{
			case StateItem:
				{
					ZeroMemory( &script, sizeof( script ) );
					scriptList	= 0;

					scriptState	= StateNone;					
					break;
				}
			case StateRace:
				{
					scriptState	= StateItem;
					break;
				}
			case StatePart:
				{
					scriptState	= StateRace;
					break;
				}
			case StateGender:
				{
					scriptState	= StatePart;

					for(
						TextList::const_iterator it = textList.begin();
						textList.end() != it;
					++it )
					{
						SafeStrCpy( line, it->c_str(), sizeof( line ) );

						for(
							const char* key = strtok( line, separator );
							key;
						key = strtok( 0, separator ) )
						{
							const char* value = strtok( 0, separator );

							if( ! value )
							{
								continue;
							}

							if( ! stricmp( "value", key ) )
							{
								script.mValue	= atoi( value );
							}
							else if( ! stricmp( "text", key ) )
							{
								script.mText	= atoi( value );
							}
						}

						if( scriptList )
						{
							scriptList->push_back( script );
						}						
					}

					break;
				}
			}

			textList.clear();
		}
	}

	if( openCount )
	{
		ASSERT( 0 );
	}
}

//081027 NYJ - ���ͼ�ȯ������
void CItemManager::LoadMonSummonItemInfo()
{
	CMHFile file;
	if(!file.Init("system/Resource/MonSummonItem.bin", "rb"))
		return;

	char string[256] = {0,};
	WORD wGroupIdx;
	// 081211 LYW --- ItemManager : ggomgrak��û���� dwAccumRate�� ���������� 
	// WORD->DWORD�� �����ϰ� ��������wAccumRate->dwAccumRate�� ������.
    DWORD dwAccumRate;
	bool bBrace = false;
	stMonSummonGroupInfo* pAddGroup = NULL;

	while(!file.IsEOF())
	{
		file.GetString(string);
		if(string[0] == '@')
		{
			file.GetLineX(string, 256);
			continue;
		}
		else if(0==strcmp(string, "#FORBIDDENMAP"))
		{
			DWORD mapnum = file.GetDword();
			m_vecMonSummonForbiddenMap.push_back(mapnum);
		}
		else if(0==strcmp(string, "#GROUP"))
		{
			pAddGroup = new stMonSummonGroupInfo;
			if(pAddGroup)
				wGroupIdx = file.GetWord();
		}
		else if(0==strcmp(string, "{"))
		{
			dwAccumRate = 0;
			bBrace = true;
		}
		else if(0==strcmp(string, "}"))
		{
			if(bBrace)
			{
				pAddGroup->GroupIdx = wGroupIdx;
				m_vecMonSummonGroupInfo.push_back(pAddGroup);
				pAddGroup = NULL;
				bBrace = false;
			}
		}
		else if(0==strcmp(string, "#NOTFORBIDDENMAP"))
		{
			DWORD value = file.GetDword();
			if(pAddGroup && value)
				pAddGroup->bNotForbidden = true;
		}
		else if(0==strcmp(string, "#DIETIME"))
		{
			DWORD value = file.GetDword();
			if(pAddGroup && value)
				pAddGroup->dwDieTime = value;
		}
		else if(0==strcmp(string, "#BOSS"))
		{
			stMonSummonInfo* pAddData = new stMonSummonInfo;
			pAddData->MonsterIdx = file.GetDword();
			pAddData->minRate = dwAccumRate;
			dwAccumRate += file.GetDword();
			pAddData->maxRate = dwAccumRate;

			pAddGroup->vecBossMosSummonGroup.push_back(pAddData);
		}
		else if(0==strcmp(string, "#NORMAL"))
		{
			stMonSummonInfo* pAddData = new stMonSummonInfo;
			pAddData->MonsterIdx = file.GetDword();
			pAddData->minRate = 0;
			pAddData->maxRate = 0;

			pAddGroup->vecMonSummonGroup.push_back(pAddData);
		}
	}
	file.Release();
}





//=================================================================================================
// NAME : UseItem_GetSkillPointByItem
// DESC : 080611 LYW --- ItemManager : ��ų����Ʈ ȹ�� �ֹ��� �߰�.
//=================================================================================================
BYTE CItemManager::UseItem_GetSkillPointByItem(CPlayer* pPlayer, WORD wPoint)
{
	// ���� ���� üũ.
	ASSERT(pPlayer) ;
	if(!pPlayer) return eItemUseErr_Err ;


	// ����Ʈ ��ȿ ��ġ üũ.
	ASSERT(wPoint < 65535) ;
	if(wPoint >= 65535) return eItemUseErr_Err ;


	// ��ų ����Ʈ ������Ʈ.
	pPlayer->SetSkillPoint(wPoint, FALSE) ;

	return eItemUseSuccess ;
}

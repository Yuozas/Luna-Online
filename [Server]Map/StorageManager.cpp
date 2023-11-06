// StorageManager.cpp: implementation of the CStorageManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StorageManager.h"
#include "MapDBMsgParser.h"
#include "Player.h"
#include "UserTable.h"
#include "Network.h"
#include "MHFile.h"
#include "ItemManager.h"
#include "..\[CC]Header\GameResourceManager.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStorageManager::CStorageManager()
{
}

CStorageManager::~CStorageManager()
{
}

void CStorageManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch( Protocol )
	{
	case MP_STORAGE_BUY_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer)	return;
			
			// 070324 LYW --- StorageManager : Delete function CheckHack of NetworkMsgParse of CStorageManager.
			//if( !CheckHack( pPlayer ) )	return;
			
			BuyStorageSyn(pPlayer);
		}
		break;
	case MP_STORAGE_PUTIN_MONEY_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer)	return;

			// 080608 LYW --- StorageManager : â�� ��ȯ �������� ����鼭, â�� npc �տ����� �ƴ�, �ʵ� 
			// ��𿡼��� â�� ����� �� �ְ� �Ǿ���. ������, ��/��� �� �Ÿ� üũ�� �ϴ� CheckHack �Լ� ȣ���� �����Ѵ�.
			//if( !CheckHack( pPlayer ) )	return;

			PutInMoneyStorage(pPlayer, (DWORD)pmsg->dwData);
		}
		break;
	case MP_STORAGE_PUTOUT_MONEY_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer)	return;

			// 080608 LYW --- StorageManager : â�� ��ȯ �������� ����鼭, â�� npc �տ����� �ƴ�, �ʵ� 
			// ��𿡼��� â�� ����� �� �ְ� �Ǿ���. ������, ��/��� �� �Ÿ� üũ�� �ϴ� CheckHack �Լ� ȣ���� �����Ѵ�.
			//if( !CheckHack( pPlayer ) )	return;

			PutOutMoneyStorage(pPlayer, (DWORD)pmsg->dwData);
		}
		break;
	}

}

void CStorageManager::LoadStorageList()
{
	CMHFile file;
	if(file.Init("./System/Resource/StorageListInfo.bin","rb") == FALSE)
		return;
	
	int count = 0;
	for(int n=0;n<MAX_STORAGELIST_NUM;++n)
	{
		m_StorageListInfo[count].MaxCellNum = 30*(count+1);
		m_StorageListInfo[count].MaxMoney = file.GetDword();
		m_StorageListInfo[count].BuyPrice = file.GetDword();
		
		++count;
	}
	
	file.Release();

}

void CStorageManager::SendStorageItemInfo(CPlayer* pPlayer)
{
	SEND_STORAGE_ITEM_INFO StorageItem;
	memset(&StorageItem, 0, sizeof(SEND_STORAGE_ITEM_INFO));
	StorageItem.Category = MP_ITEM;
	StorageItem.Protocol = MP_ITEM_STORAGE_ITEM_INFO_ACK;

	// 081010 LUJ, �ɼ��� ������ ����ü ����
	MSG_ITEM_OPTION optionMessage;
	ZeroMemory( &optionMessage, sizeof( optionMessage ) );
	optionMessage.Category	= MP_ITEM;
	optionMessage.Protocol	= MP_ITEM_OPTION_ADD_ACK;

	CItemSlot * pSlot = pPlayer->GetSlot(eItemTable_Storage);
	StorageItem.money = pSlot->GetPurse()->GetPurseCurMoney();
	
	POSTYPE StartPos = TP_STORAGE_START;
	
	for(int i=0;i<SLOT_STORAGE_NUM;++i)
	{
		if( pSlot->IsEmpty(StartPos+i) == FALSE )
		{
			 StorageItem.StorageItem[i]= *pSlot->GetItemInfoAbs(StartPos+i);

			ITEMBASE& item = StorageItem.StorageItem[i];

			const ITEM_OPTION& option = ITEMMGR->GetOption( item );

			if( option.mItemDbIndex )
			{
				optionMessage.mOption[ optionMessage.mSize++ ] = option;
			}

			const DWORD maxOptionSize = ( sizeof( optionMessage.mOption ) / sizeof( *( optionMessage.mOption ) ) );

			// 081010 LUJ, â�� ������ �ɼ� �޽��� ������ ũ�Ƿ� �ִ� ������ �������� �� ���۽�Ű�� �������� �غ� �Ѵ�
			if( maxOptionSize == optionMessage.mSize )
			{
				pPlayer->SendMsg( &optionMessage, optionMessage.GetSize() );
				ZeroMemory( &optionMessage.mOption, sizeof( optionMessage.mOption ) );

				optionMessage.mSize = 0;
			}
		}
	}
	
	// 081010 LUJ, �����ִ� �ɼ��� �����Ѵ�
	if( optionMessage.mSize )
	{
		pPlayer->SendMsg( &optionMessage, optionMessage.GetSize() );
	}

	pPlayer->SendMsg(&StorageItem, StorageItem.GetSize());
}

void CStorageManager::BuyStorageSyn(CPlayer* pPlayer)
{
	WORD storagenum = pPlayer->GetStorageNum();

	STORAGELISTINFO* pBuyStorageInfo = GetStorageInfo(storagenum+1);

	if( storagenum >= TAB_BASE_STORAGE_NUM || pPlayer->GetMoney() < pBuyStorageInfo->BuyPrice )
	{
		MSGBASE msg;
		msg.Category = MP_STORAGE;
		msg.Protocol = MP_STORAGE_BUY_NACK;
		pPlayer->SendMsg(&msg, sizeof(msg));
	}
	else
	{
		if(pPlayer->GetStorageNum() == 0) //ó�� ������ �׳� �ʱ�ȭ
		{
			STORAGE_ITEM StorageItem;
			memset(&StorageItem, 0, sizeof(STORAGE_ITEM));
			
			pPlayer->InitStorageItemInfo(&StorageItem);
		}
		pPlayer->SetMoney( pBuyStorageInfo->BuyPrice, MONEY_SUBTRACTION, 0, eItemTable_Inventory, eMoneyLog_LoseStorageBuy, 0);

		pPlayer->SetStorageNum( storagenum+1 );
		pPlayer->SetMaxPurseMoney(eItemTable_Storage, GetStorageInfo(pPlayer->GetStorageNum())->MaxMoney);
		StorageBuyStorage(pPlayer->GetID()); //db update		
		
		MSG_BYTE msg;
		msg.Category = MP_STORAGE;
		msg.Protocol = MP_STORAGE_BUY_ACK;
		msg.bData	= storagenum+1;
		pPlayer->SendMsg(&msg, sizeof(msg));
	}
}

void CStorageManager::PutInMoneyStorage(CPlayer* pPlayer, MONEYTYPE setMoney)
{
	MONEYTYPE storagemon = pPlayer->GetMoney(eItemTable_Storage);
	MONEYTYPE maxmon = pPlayer->GetMaxPurseMoney(eItemTable_Storage);
	
	if(pPlayer->GetMoney() < setMoney)
	{
		setMoney = pPlayer->GetMoney();
	}
	if( maxmon - storagemon < setMoney )
	{
		setMoney = maxmon - storagemon;
	}	
	
	if(setMoney == 0)
	{
		MSGBASE msg;
		msg.Category = MP_STORAGE;
		msg.Protocol = MP_STORAGE_PUTIN_MONEY_NACK;
		
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
		
	setMoney = pPlayer->SetMoney(setMoney, MONEY_SUBTRACTION, 0, eItemTable_Inventory, eMoneyLog_LoseStorage, pPlayer->GetUserID());
	if(setMoney == 0)
	{
		ASSERTMSG(0, "Must Check!");
		return;
	}
	pPlayer->SetMoney(setMoney, MONEY_ADDITION, 0, eItemTable_Storage);
	

	StorageMoneyUpdateToDB(pPlayer->GetUserID(), pPlayer->GetMoney(eItemTable_Storage), pPlayer->GetID());	//dbUpdate

	InsertLogMoney( eMoneyLog_LoseStorage, pPlayer->GetID(), pPlayer->GetMoney(), setMoney, pPlayer->GetUserID(), pPlayer->GetMoney(eItemTable_Storage) );
	
	//YH
	LogItemMoney( pPlayer->GetID(), pPlayer->GetObjectName(), pPlayer->GetUserID(), "#Storage",
				eLog_ItemMoveInvenToStorage, pPlayer->GetMoney(), pPlayer->GetMoney(eItemTable_Storage),
				setMoney, 0, 0, 0, 0, 0, pPlayer->GetPlayerExpPoint());

	MSG_DWORD msg;
	msg.Category = MP_STORAGE;
	msg.Protocol = MP_STORAGE_PUTIN_MONEY_ACK;
	msg.dwData	= pPlayer->GetMoney(eItemTable_Storage);

	pPlayer->SendMsg(&msg, sizeof(msg));
}

void CStorageManager::PutOutMoneyStorage(CPlayer* pPlayer, MONEYTYPE getMoney)
{
	DWORDEX getmoney = getMoney;
	DWORDEX storagemon = pPlayer->GetMoney(eItemTable_Storage);
	DWORDEX maxmon = pPlayer->GetMaxPurseMoney(eItemTable_Inventory); //�κ��丮�� �� �ű�ϱ�.
	
	if( storagemon < getmoney)
	{
		getmoney = storagemon;
	}
	
	if(getmoney + pPlayer->GetMoney() > MAX_INVENTORY_MONEY)
	{
		MSG_DWORD msg;
		msg.Category = MP_STORAGE;
		msg.Protocol = MP_STORAGE_PUTOUT_MONEY_NACK;
		msg.dwData	= pPlayer->GetMoney(eItemTable_Storage);

		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	else if(getmoney + pPlayer->GetMoney() > maxmon)
	{
		getmoney = maxmon - pPlayer->GetMoney();
	}

	if(getmoney == 0)
		return;
	getmoney = pPlayer->SetMoney(getmoney, MONEY_SUBTRACTION, 0, eItemTable_Storage);
	if(getmoney == 0) //purse���� ���� �������� 
	{
		ASSERTMSG(0, "Must Check!");
		return;
	}
	pPlayer->SetMoney(getmoney, MONEY_ADDITION, 0, eItemTable_Inventory, eMoneyLog_GetStorage, pPlayer->GetUserID());
	

	StorageMoneyUpdateToDB(pPlayer->GetUserID(), pPlayer->GetMoney(eItemTable_Storage), pPlayer->GetID());	//dbUpdate
	
	InsertLogMoney( eMoneyLog_GetStorage, pPlayer->GetID(), pPlayer->GetMoney(), getmoney, pPlayer->GetUserID(), pPlayer->GetMoney(eItemTable_Storage) );

	//YH
	LogItemMoney( pPlayer->GetUserID(), "#Storage", pPlayer->GetID(), pPlayer->GetObjectName(), 
				eLog_ItemMoveStorageToInven, pPlayer->GetMoney(eItemTable_Storage), pPlayer->GetMoney(),
				getmoney, 0, 0, 0, 0, 0, pPlayer->GetPlayerExpPoint());

	MSG_DWORD msg;
	msg.Category = MP_STORAGE;
	msg.Protocol = MP_STORAGE_PUTOUT_MONEY_ACK;
	msg.dwData	= pPlayer->GetMoney(eItemTable_Storage);

	pPlayer->SendMsg(&msg, sizeof(msg));
}

STORAGELISTINFO* CStorageManager::GetStorageInfo(int i)
{
	ASSERT(i>0);
	ASSERT(i<=MAX_STORAGELIST_NUM);
	return &m_StorageListInfo[i-1]; 
}

BOOL CStorageManager::CheckHack( CPlayer* pPlayer )
{
//	if( pPlayer->GetShopItemManager()->GetUsingItemInfo( eIncantation_ShowStorage ) != NULL ||
//		pPlayer->GetShopItemManager()->GetUsingItemInfo( eIncantation_ShowStorage7 ) != NULL ||
//				pPlayer->GetShopItemManager()->GetUsingItemInfo( eIncantation_ShowStorage7_NoTrade ) != NULL )
//		return TRUE;

	VECTOR3 vPos;
	pPlayer->GetPosition( &vPos );
	if( !GAMERESRCMNGR->IsInWareNpcPosition( &vPos ) )	return FALSE;

	return TRUE;
}

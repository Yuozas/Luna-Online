#include "stdafx.h"
#include "GuildWarehouse.h"
#include "Player.h"
#include "UserTable.h"
#include "ItemManager.h"
#include "GuildManager.h"


CGuildWarehouse::CGuildWarehouse()
{
}

CGuildWarehouse::~CGuildWarehouse()
{
	for(int i=0;i<TAB_GUILDWAREHOUSE_NUM;++i)
	{
//		PTRLISTSEARCHSTART(m_WarehousePlayerList[i],DWORD*,pInfo)
//			delete pInfo;
//		PTRLISTSEARCHEND
			
		m_WarehousePlayerList[i].RemoveAll();
	}

	m_GuildWarePurse.Release();
}

void CGuildWarehouse::Init(MONEYTYPE GuildMoney)
{
	ZeroMemory( m_TotalItemBase, sizeof( m_TotalItemBase ) );
	ZeroMemory( m_TotalSlotInfo, sizeof( m_TotalSlotInfo ) );

	CMunpaWareSlot::Init(TP_GUILDWAREHOUSE_START, SLOT_GUILDWAREHOUSE_NUM, m_TotalItemBase, m_TotalSlotInfo);
	CreatePurse(&m_GuildWarePurse, this, GuildMoney, 4000000000);
}

void CGuildWarehouse::AddWarehousePlayer(DWORD PlayerID, BYTE TabNum)
{
	// ���� ���̵� ������ �߰����� �ʴ´�.
	PTRLISTPOS pos = m_WarehousePlayerList[TabNum].GetHeadPosition();
	while(pos)
	{
		DWORD PID = (DWORD)m_WarehousePlayerList[TabNum].GetAt(pos);
		if(PID == PlayerID)
		{
			return;
		}

		m_WarehousePlayerList[TabNum].GetNext(pos);
	}

	m_WarehousePlayerList[TabNum].AddTail((void*)PlayerID);	
}

void CGuildWarehouse::RemoveWarehousePlayer(DWORD PlayerID, BYTE TabNum)
{
	PTRLISTPOS pos = m_WarehousePlayerList[TabNum].GetHeadPosition();
	while(pos)
	{
		DWORD PID = (DWORD)m_WarehousePlayerList[TabNum].GetAt(pos);
		if(PID == PlayerID)
		{
			m_WarehousePlayerList[TabNum].RemoveAt(pos);
			break;
		}

		m_WarehousePlayerList[TabNum].GetNext(pos);
	}
}


void CGuildWarehouse::SendtoWarehousePlayer(BYTE TabNum, MSGBASE* msg, int size, DWORD ExceptID)
{
	PTRLISTPOS pos = m_WarehousePlayerList[TabNum].GetHeadPosition();
	while(pos)
	{
		DWORD PlayerID = (DWORD)m_WarehousePlayerList[TabNum].GetAt(pos);
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(PlayerID);
		if(!pPlayer)
			m_WarehousePlayerList[TabNum].RemoveAt(pos);
		else if(PlayerID != ExceptID)
			pPlayer->SendMsg(msg, size);
		m_WarehousePlayerList[TabNum].GetNext(pos);
	}
}

//void CGuildWarehouse::SendtoWarehousePlayerAll(MSGBASE* msg, int size)
//{
//	for(int i=0; i<TAB_GUILDWAREHOUSE_NUM; ++i)
//	{
//		PTRLISTPOS pos = m_WarehousePlayerList[i].GetHeadPosition();
//		while(pos)
//		{
//			DWORD PlayerID = (DWORD)m_WarehousePlayerList[i].GetAt(pos);
//			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(PlayerID);
//			if(!pPlayer)
//				m_WarehousePlayerList[i].RemoveAt(pos);
//			else 
//				pPlayer->SendMsg(msg, size);
//			m_WarehousePlayerList[i].GetNext(pos);
//		}
//	}
//}

//void CGuildWarehouse::EnterWareHouse(CPlayer* pPlayer,BYTE TabNum)
//{
//	AddWarehousePlayer(pPlayer->GetID(),TabNum);
//
//	SEND_MUNPA_WAREHOUSE_ITEM_INFO msg;
//	memset(&msg, 0, sizeof(SEND_MUNPA_WAREHOUSE_ITEM_INFO));
//	msg.Category = MP_ITEM;
//	msg.Protocol = MP_ITEM_GUILD_WAREHOUSE_INFO_ACK;
//	CMunpaWareSlot::GetItemInfoTab(TabNum,msg.info.WarehouseItem);
//	msg.info.money = m_GuildWarePurse.GetPurseCurMoney();
//	msg.bTabNum = TabNum;
//
//	ITEM_OPTION MunpaWareItemOption[TABCELL_GUILDWAREHOUSE_NUM];
//	//ITEM_RARE_OPTION_INFO MunpaWareItemRareOption[TABCELL_GUILDWAREHOUSE_NUM];	
//	WORD OptionNum = 0;
//	WORD RareOptionNum = 0;
//	
//	for(int i=0;i<TABCELL_GUILDWAREHOUSE_NUM;++i)
//	{
//		if(msg.info.WarehouseItem[i].Durability > MAX_YOUNGYAKITEM_DUPNUM)
//		{
//			if(ITEMMGR->IsOptionItem(msg.info.WarehouseItem[i].wIconIdx, msg.info.WarehouseItem[i].Durability))
//			{
//				const ITEM_OPTION* pOptionInfo = GUILDMGR->GetItemOption(msg.info.WarehouseItem[i].Durability);
//
//				if(pOptionInfo)
//				{
//					MunpaWareItemOption[OptionNum] = *pOptionInfo;
//					++OptionNum;
//				}
//			}
//		}
//		////SW050920 Rare
//		//if( msg.info.WarehouseItem[i].RareIdx )
//		//{
//		//	if( ITEMMGR->IsRareOptionItem(msg.info.WarehouseItem[i].wIconIdx, msg.info.WarehouseItem[i].RareIdx) )
//		//	{
//		//		ITEM_RARE_OPTION_INFO* pRareOptionInfo = GUILDMGR->GetItemRareOption( msg.info.WarehouseItem[i].RareIdx );
//
//		//		if(pRareOptionInfo)
//		//		{
//		//			MunpaWareItemRareOption[RareOptionNum] = *pRareOptionInfo;
//		//			++RareOptionNum;
//		//		}
//
//		//	}
//		//}
//	}
//
//	if( OptionNum )
//	{
//		msg.AddableInfo.AddInfo( CAddableInfoList::ItemOption, sizeof( MunpaWareItemOption[ 0 ] )*OptionNum, MunpaWareItemOption );
//		msg.wOptionCount = OptionNum;
//	}
//	//if( RareOptionNum )
//	//{
//	//	msg.AddableInfo.AddInfo( CAddableInfoList::ItemRareOption, sizeof( MunpaWareItemRareOption[ 0 ] )*RareOptionNum, MunpaWareItemRareOption );
//	//	msg.wRareOptionCount = RareOptionNum;
//	//}
//
//	pPlayer->SendMsg(&msg,msg.GetSize());
//}
//
//void CGuildWarehouse::LeaveWareHouse(DWORD PlayerID,BYTE TabNum)
//{
//	RemoveWarehousePlayer(PlayerID,TabNum);
//}

BOOL CGuildWarehouse::InitGuildItem(ITEMBASE* pInfo)
{
	return CMunpaWareSlot::InsertItemAbs(NULL, pInfo->Position,pInfo);
}

BOOL CGuildWarehouse::IsEmpty(POSTYPE absPos)
{
	return CMunpaWareSlot::IsEmpty(absPos);
}


ERROR_ITEM CGuildWarehouse::InsertItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItem, WORD state)
{
	ERROR_ITEM rt = CMunpaWareSlot::InsertItemAbs(pPlayer, absPos,pItem, state);
	if(rt != EI_TRUE)
		return rt;
	if(pPlayer)
	{
		MSG_ITEM_WITH_OPTION msg;
		ZeroMemory( &msg, sizeof( msg ) );
		
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_ADDTO_GUILDWARE_NOTIFY;
		msg.ItemInfo = *pItem;
		msg.OptionInfo	= ITEMMGR->GetOption( *pItem );

		BYTE TabNum = CMunpaWareSlot::GetTabNum(absPos);
		SendtoWarehousePlayer(TabNum, &msg, sizeof( msg ), pPlayer->GetID());
	}
	return rt;
}

ERROR_ITEM CGuildWarehouse::DeleteItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItemOut, WORD state)
{
	ERROR_ITEM rt = CMunpaWareSlot::DeleteItemAbs(pPlayer, absPos,pItemOut);
	if(rt != EI_TRUE)
		return rt;
	if(pPlayer)
	{
		MSG_ITEM msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_DELETEFROM_GUILDWARE_NOTIFY;
		msg.ItemInfo = *pItemOut;
		
		BYTE TabNum = CMunpaWareSlot::GetTabNum(absPos);
		SendtoWarehousePlayer(TabNum,&msg,sizeof(msg),pPlayer->GetID());
	}
	return rt;
}

ERROR_ITEM CGuildWarehouse::UpdateItemAbs(CPlayer * pPlayer, POSTYPE whatAbsPos, DWORD dwDBIdx, WORD wItemIdx, POSTYPE position, POSTYPE quickPosition, DURTYPE Dur, WORD flag, WORD state, DWORD RareDBIdx)
{
	return CMunpaWareSlot::UpdateItemAbs(pPlayer, whatAbsPos, dwDBIdx, wItemIdx, position, quickPosition, Dur, flag, state, RareDBIdx);
}

ITEMBASE const * CGuildWarehouse::GetItemInfoAbs(POSTYPE absPos)
{
	if( !CMunpaWareSlot::IsPosIn(absPos) )	return NULL;

	return  &m_ItemBaseArray[absPos-m_StartAbsPos];	
}

MONEYTYPE CGuildWarehouse::GetMaxMoney()
{
	return m_pPurse->GetMaxMoney();
}

MONEYTYPE CGuildWarehouse::SetMoney( MONEYTYPE ChangeValue, BYTE bOper, BYTE MsgFlag )
{
	CPurse* pPurse = GetPurse();
	if( !pPurse) return FALSE;

	if( bOper == MONEY_SUBTRACTION )
	{
		return pPurse->Subtraction( ChangeValue, MsgFlag );
	}
	else
	{
		return pPurse->Addition( ChangeValue, MsgFlag );
	}

	return 0;	
}

BOOL CGuildWarehouse::IsEmptyAndMoney()
{
	if( m_pPurse->GetPurseCurMoney() )	return FALSE;

	for( int i = 0; i < SLOT_GUILDWAREHOUSE_NUM; ++i )
	{
		if( m_TotalItemBase[i].dwDBIdx )
			return FALSE;
	}
	return TRUE;
}

void CGuildWarehouse::AddItemInfoWaitingMember( CPlayer* pPlayer, BYTE TabNum )
{
	AddWarehousePlayer(pPlayer->GetID(),TabNum);
}

//void CGuildWarehouse::SendItemInfoToAllWaitingMembers()
//{
//	SEND_MUNPA_WAREHOUSE_ITEM_INFO msg;
//	memset(&msg, 0, sizeof(SEND_MUNPA_WAREHOUSE_ITEM_INFO));
//	msg.Category = MP_ITEM;
//	msg.Protocol = MP_ITEM_GUILD_WAREHOUSE_INFO_ACK;
//	CMunpaWareSlot::GetItemInfoTab( 0,msg.info.WarehouseItem);
//	msg.info.money = m_GuildWarePurse.GetPurseCurMoney();
//	msg.bTabNum = 0;
//
//	ITEM_OPTION MunpaWareItemOption[TABCELL_GUILDWAREHOUSE_NUM];
//	//ITEM_RARE_OPTION_INFO MunpaWareItemRareOption[TABCELL_GUILDWAREHOUSE_NUM];	
//	WORD OptionNum = 0;
//	//WORD RareOptionNum = 0;
//	
//	for(int i=0;i<TABCELL_GUILDWAREHOUSE_NUM;++i)
//	{
//		if(msg.info.WarehouseItem[i].Durability > MAX_YOUNGYAKITEM_DUPNUM)
//		{
//			if(ITEMMGR->IsOptionItem(msg.info.WarehouseItem[i].wIconIdx, msg.info.WarehouseItem[i].Durability))
//			{
//				const ITEM_OPTION* pOptionInfo = GUILDMGR->GetItemOption(msg.info.WarehouseItem[i].Durability);
//				if(pOptionInfo)
//				{
//					MunpaWareItemOption[OptionNum] = *pOptionInfo;
//					++OptionNum;
//				}
//			}
//		}
//		////SW050920 Rare
//		//if( msg.info.WarehouseItem[i].RareIdx )
//		//{
//		//	if( ITEMMGR->IsRareOptionItem(msg.info.WarehouseItem[i].wIconIdx, msg.info.WarehouseItem[i].RareIdx) )
//		//	{
//		//		ITEM_RARE_OPTION_INFO* pRareOptionInfo = GUILDMGR->GetItemRareOption( msg.info.WarehouseItem[i].RareIdx );
//		//		if(pRareOptionInfo)
//		//		{
//		//			MunpaWareItemRareOption[RareOptionNum] = *pRareOptionInfo;
//		//			++RareOptionNum;
//		//		}
//
//		//	}
//		//}
//	}
//
//	if( OptionNum )
//	{
//		msg.AddableInfo.AddInfo( CAddableInfoList::ItemOption, sizeof( MunpaWareItemOption[ 0 ] )*OptionNum, MunpaWareItemOption );
//		msg.wOptionCount = OptionNum;
//	}
//	//if( RareOptionNum )
//	//{
//	//	msg.AddableInfo.AddInfo( CAddableInfoList::ItemRareOption, sizeof(ITEM_OPTION)*RareOptionNum, MunpaWareItemRareOption );
//	//	msg.wRareOptionCount = RareOptionNum;
//	//}
//
//	SendtoWarehousePlayerAll(&msg, sizeof(msg));
//}

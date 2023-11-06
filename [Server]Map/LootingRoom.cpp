// LootingRoom.cpp: implementation of the CLootingRoom class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LootingRoom.h"
#include "Player.h"
#include "InventorySlot.h"
#include "WearSlot.h"
//#include "cLinkedList.h"
#include "ItemManager.h"
#include "MapDBMsgParser.h"
#include "UserTable.h"
#include "CharMove.h"
#include "LootingManager.h"
#include "..\[CC]Header\GameResourceManager.h"

//---KES CHEAT PKEVENT
#include "PKManager.h"
//--------------------

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLootingRoom::CLootingRoom()
{
	m_dwDiePlayer	= 0;
	m_dwAttacker	= 0;

	m_dwLootingStartTime = 0;
}

CLootingRoom::~CLootingRoom()
{

}

void CLootingRoom::SetLootingStart()
{
	m_dwLootingStartTime = gCurTime;
}

//---KES PK 071202
void CLootingRoom::MakeLootingRoom( CPlayer* pDiePlayer, CPlayer* pAttackPlayer )
{
	m_dwDiePlayer	= pDiePlayer->GetID();
	m_dwAttacker	= pAttackPlayer->GetID();
	ZeroMemory( m_LootingItemArray, sizeof(m_LootingItemArray) );

	int nInvenItemNum = 0;
	int nWearedItemNum = 0;

	if( pDiePlayer->IsPKMode() )
	{
		nInvenItemNum = 1 + pDiePlayer->GetBadFame() / 500;		//---KES �⺻ 4������ 6���� ��ȹ���� 2008.1.4	PK 0227
		if( nInvenItemNum > 5 ) nInvenItemNum = 5;				//---KES �ƽ� 7������ 10���� ��ȹ���� 2008.1.4	PK 0227
		nWearedItemNum = pDiePlayer->GetBadFame() / 1000;		//---KES �⺻ 6������ 4���� ��ȹ���� 2008.1.4	PK 0227
		if( nWearedItemNum > 2 ) nWearedItemNum = 2;			//---KES �ƽ� 10������ 7���� ��ȹ���� 2008.1.4	PK 0227
	}
	else
	{
		nInvenItemNum = pAttackPlayer->GetBadFame() / 20000;			//---KES �⺻ 4������ 1���� ��ȹ���� 2008.1.4	PK 0227
		if( nInvenItemNum > 1 ) nInvenItemNum = 1;						//---KES �ƽ� 7������ 4���� ��ȹ���� 2008.1.4	PK 0227
		nWearedItemNum = 0; //pAttackPlayer->GetBadFame() / 300;		//---KES �⺻ 1������ 0���� ��ȹ���� 2008.1.4	PK 0227
		//if( nWearedItemNum > 1 ) nWearedItemNum = 1;					//---KES �ƽ� 3������ 1���� ��ȹ���� 2008.1.4	PK 0227
	}

//---GM �� �����ڴ� ���ô����� �ʴ´�. (EVENT�ÿ��� ����)
//---KES CHEAT PKEVENT
	if( pDiePlayer->GetUserLevel() > eUSERLEVEL_GM || g_pServerSystem->IsTestServer() || PKMGR->IsPKEvent() )
	{
		CInventorySlot* pInventory	= (CInventorySlot*)pDiePlayer->GetSlot(eItemTable_Inventory);
		CWearSlot* pWear			= (CWearSlot*)pDiePlayer->GetSlot( eItemTable_Weared );
		MONEYTYPE HasMoney			= pDiePlayer->GetMoney();
		
		EXPTYPE MaxExp	= GAMERESRCMNGR->GetMaxExpPoint( pDiePlayer->GetLevel() );
		//EXPTYPE HasExp	= pDiePlayer->GetPlayerExpPoint();
		
		int i;
		int nInvenCount = 0;
		int nWearCount = 0;
		//---
		cConstLinkedList<int> ListNumber;
		for( i = 0 ; i < PKLOOTING_ITEM_NUM ; ++i )
			ListNumber.AddTail( i );
		
		int nIndex1, nIndex2, nPos;
		const ITEMBASE* pItemBase;
		if( pInventory->GetItemCount() )
		{
			//---
			cConstLinkedList<const ITEMBASE*> ListInvenItem;
			for( i = TP_INVENTORY_START ; i < TP_EXTENDED_INVENTORY2_END ; ++i )
			{
				pItemBase = pInventory->GetItemInfoAbs(i);
				if( !pItemBase ) continue;
				if( pItemBase->nSealed != 0 ) continue;

				ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pItemBase->wIconIdx );
				if( !pItemInfo ) continue;
				
				if( pItemInfo->Shop == 0 )		//�Ϲݾ�����
				if( pItemInfo->Trade == 1 )		//�ŷ�����
				if( !pInventory->IsEmpty( i ) )
				if( !pInventory->IsLock( i ) )
				{					
					ListInvenItem.AddTail( pInventory->GetItemInfoAbs(i) );
				}
			}
			//---
			
			while( ListInvenItem.GetCount() )
			{
				if( nInvenCount >= nInvenItemNum )
					break;
				
				nIndex1		= rand()%ListInvenItem.GetCount();
				pItemBase	= ListInvenItem.GetAt( nIndex1 );
				nIndex2		= rand()%ListNumber.GetCount();
				nPos		= ListNumber.GetAt( nIndex2 );
				m_LootingItemArray[nPos].nKind		= eLI_ITEM;
				m_LootingItemArray[nPos].dwData		= pItemBase->Position;
				
				ListInvenItem.DeleteAt( nIndex1 );
				ListNumber.DeleteAt( nIndex2 );

				++nInvenCount;
			}
			ListInvenItem.DeleteAll();
		}
		//---KES CHEAT PKEVENT
		if( pWear->GetItemCount() && ( pDiePlayer->GetUserLevel() > eUSERLEVEL_GM || g_pServerSystem->IsTestServer() ) )
		{
			//---
			cConstLinkedList<const ITEMBASE*> ListWearItem;
			for( i = TP_WEAR_START ; i < TP_WEAR_END - 4 ; ++i )	//-4 �ڽ�Ƭ������ ����
			{
				pItemBase = pWear->GetItemInfoAbs(i);
				if( !pItemBase ) continue;
				if( pItemBase->nSealed != 0 ) continue;

				ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pItemBase->wIconIdx );
				if( !pItemInfo ) continue;

				if( pItemInfo->Shop == 0 )		//�Ϲݾ�����
				if( pItemInfo->Trade == 1 )		//�ŷ�����
				if( !pWear->IsEmpty( i ) )
				if( !pWear->IsLock( i ) )
				{
					ListWearItem.AddTail( pWear->GetItemInfoAbs(i) );
				}
			}
			//---

			while( ListWearItem.GetCount() )
			{
				if( nWearCount >= nWearedItemNum )
					break;

				nIndex1		= rand()%ListWearItem.GetCount();
				pItemBase	= ListWearItem.GetAt( nIndex1 );
				nIndex2		= rand()%ListNumber.GetCount();
				nPos		= ListNumber.GetAt( nIndex2 );
				m_LootingItemArray[nPos].nKind		= eLI_ITEM;
				m_LootingItemArray[nPos].dwData		= pItemBase->Position;
				
				ListWearItem.DeleteAt( nIndex1 );
				ListNumber.DeleteAt( nIndex2 );

				++nWearCount;
			}
			ListWearItem.DeleteAll();
		}

		if( pDiePlayer->IsPKMode() )	//pk��带 Ų ������ ��츸
		{
			//����ġ �ָӴϿ� �����Ǿ� �ִ� % = 5 * (INT(����� PK ��ġ / 150) + 2), MAX. = 30
			//��ȹ���� 2008.1.15    % = 2 * (INT(����� PK ��ġ / 150) + 1), MAX. = 10
			float fExpPenalty = (float)( 2 * ( ( pDiePlayer->GetBadFame() / 150 ) + 1 ) ) / 100.f;
			if( fExpPenalty > .10f ) fExpPenalty = .10f;

			EXPTYPE LootExp = (EXPTYPE)( (float)MaxExp * fExpPenalty );
			
			//---KES CHEAT PKEVENT
			if( LootExp	&& ( pDiePlayer->GetUserLevel() > eUSERLEVEL_GM || g_pServerSystem->IsTestServer() ) )
			{
				//����ġ �ָӴϴ� 1�������� ��ȹ���� 2008.1.15
//				for( i = 0 ; i < 10 - nInvenCount ; ++i )	//����ġ �ָӴ� ����: 10 ? (�κ��丮 ������ �� ���õǴ� ����)
//				{
					nIndex2		= rand()%ListNumber.GetCount();
					nPos		= ListNumber.GetAt( nIndex2 );
					m_LootingItemArray[nPos].nKind		= eLI_EXP;
					m_LootingItemArray[nPos].dwData		= LootExp;
					ListNumber.DeleteAt( nIndex2 );
//				}
			}

			if( HasMoney > 0 )
			{
				//��� �ָӴϿ� �����Ǿ� �ִ� % = 5 * (INT(����� PK ��ġ / 150) + 2), MAX. = 30
				float fMoneyPenalty = (float)( 5 * ( ( pDiePlayer->GetBadFame() / 150 ) + 2 ) ) / 100.f;
				if( fMoneyPenalty > .30f ) fMoneyPenalty = .30f;

				MONEYTYPE LootMoney = (MONEYTYPE)( (float)HasMoney * fMoneyPenalty );

				if( LootMoney )
				{
					for( i = 0 ; i < 10 - nWearCount ; ++i )	//��� �ָӴ� ����: 10 ? (���� ������ �� ���õǴ� ����)
					{
						nIndex2		= rand()%ListNumber.GetCount();
						nPos		= ListNumber.GetAt( nIndex2 );
						m_LootingItemArray[nPos].nKind		= eLI_MONEY;
						m_LootingItemArray[nPos].dwData		= LootMoney;
						ListNumber.DeleteAt( nIndex2 );
					}
				}
			}
		}
		ListNumber.DeleteAll();
	}

	m_nChance = LOOTINGMGR->GetLootingChance( pAttackPlayer, pDiePlayer );
	m_nItemLootCount = LOOTINGMGR->GetLootingItemNum( pAttackPlayer, pDiePlayer );

	m_dwLootingStartTime = gCurTime;

	m_nLootedItemCount = 0;
}

BOOL CLootingRoom::IsTimeOut()
{
	if( gCurTime - m_dwLootingStartTime > PKLOOTING_LIMIT_TIME + PKLOOTING_DLG_DELAY_TIME + 3000 )
		return TRUE;

	return FALSE;
}

BOOL CLootingRoom::Loot( int nArrayNum, BOOL bForce )
{
	CPlayer* pAttackPlayer	= (CPlayer*)g_pUserTable->FindUser( m_dwAttacker );
	CPlayer* pDiePlayer		= (CPlayer*)g_pUserTable->FindUser( m_dwDiePlayer );
	if( !pAttackPlayer || !pDiePlayer )
		return FALSE;

	DWORD dwErrorCode = eLOOTINGERROR_OK;
	float fDist = CalcDistanceXZ( CCharMove::GetPosition( pAttackPlayer), CCharMove::GetPosition( pDiePlayer) );

	if( m_nChance <= 0 )
	{
		dwErrorCode = eLOOTINGERROR_NOMORE_CHANCE;
		goto LOOTING_ERROR;
	}
	if( m_nItemLootCount <= 0 )
	{
		dwErrorCode = eLOOTINGERROR_NOMORE_ITEMLOOTCOUNT;
		goto LOOTING_ERROR;
	}
	if( nArrayNum >= PKLOOTING_ITEM_NUM || nArrayNum < 0 )
	{
		dwErrorCode = eLOOTINGERROR_INVALID_POSITION;
		goto LOOTING_ERROR;
	}
	if( m_LootingItemArray[nArrayNum].nKind == eLI_SELECTED )
	{
		dwErrorCode = eLOOTINGERROR_ALREADY_SELECTED;
		goto LOOTING_ERROR;
	}

	if( fDist > PK_LOOTING_DISTANCE + 500 )
	{
		if( !bForce )
		{
			dwErrorCode	= eLOOTINGERROR_OVER_DISTANCE;
			goto LOOTING_ERROR;
		}
	}

	--m_nChance;

	if( m_LootingItemArray[nArrayNum].nKind == eLI_NONE )
	{
		if( !bForce )
		{
			MSG_DWORD2 msg;
			msg.Category	= MP_PK;
			msg.Protocol	= MP_PK_LOOTING_NOLOOTING;
			msg.dwData1		= m_dwDiePlayer;
			msg.dwData2		= nArrayNum;
			pAttackPlayer->SendMsg( &msg, sizeof(msg) );
		}

		return TRUE;
	}
	
	if( m_LootingItemArray[nArrayNum].nKind == eLI_ITEM )
	{
		//////////////////////////////////////////
		//AttackPlayerAC
		CItemSlot* pAInven	= pAttackPlayer->GetSlot( eItemTable_Inventory );
		WORD wEmptyPos;
		if( pAInven->GetEmptyCell( &wEmptyPos ) == 0 )
		{
			//msg
			MSG_DWORD msg;
			msg.Category	= MP_PK;
			msg.Protocol	= MP_PK_LOOTING_NOINVENSPACE;
			msg.dwData		= m_dwDiePlayer;
			pAttackPlayer->SendMsg( &msg, sizeof(msg) );
		}
		else
		{
			/////////////////////////
			//DiePlayer
			CItemSlot* pDSlot = pDiePlayer->GetSlot( (POSTYPE)m_LootingItemArray[nArrayNum].dwData );
			ITEMBASE ItemBase;
			if( EI_TRUE != pDSlot->DeleteItemAbs( pDiePlayer, (POSTYPE)m_LootingItemArray[nArrayNum].dwData, &ItemBase ) )
				return TRUE;
			
			MSG_EXCHANGE_REMOVEITEM RemoveMsg;
			RemoveMsg.Category		= MP_PK;
			RemoveMsg.Protocol		= MP_PK_LOOTING_ITEMLOOTED;
			RemoveMsg.wAbsPosition	= (POSTYPE)m_LootingItemArray[nArrayNum].dwData;
			pDiePlayer->SendMsg( &RemoveMsg, sizeof( RemoveMsg ) );
			
			//////////////////////////
			//AttackPlayer
			ItemBase.Position		= wEmptyPos;
			ItemBase.QuickPosition	= 0;
			ItemUpdateToDB( m_dwAttacker, ItemBase.dwDBIdx, ItemBase.wIconIdx, ItemBase.Durability,
				ItemBase.Position, ItemBase.QuickPosition );
		
			int rt = pAInven->InsertItemAbs( NULL, ItemBase.Position, &ItemBase, SS_LOCKOMIT );
			ASSERT( rt == EI_TRUE );
			
			LogItemMoney(m_dwDiePlayer, pDiePlayer->GetObjectName(), m_dwAttacker, pAttackPlayer->GetObjectName(),
				eLog_ItemObtainPK, pDiePlayer->GetMoney(eItemTable_Inventory), pAttackPlayer->GetMoney(eItemTable_Inventory), 
				0, ItemBase.wIconIdx,  ItemBase.dwDBIdx, 0, ItemBase.Position, ItemBase.Durability, pDiePlayer->GetPlayerExpPoint());

			MSG_LOOTINGIEM AddMsg;
			ZeroMemory( &AddMsg, sizeof( AddMsg ) );

			AddMsg.Category			= MP_PK;
			AddMsg.Protocol			= MP_PK_LOOTING_ITEMLOOTING;
			AddMsg.dwDiePlayerIdx	= m_dwDiePlayer;
			AddMsg.ItemInfo			= ItemBase;
			AddMsg.wAbsPosition		= nArrayNum;
			AddMsg.sOptionInfo		= ITEMMGR->GetOption( ItemBase );
			
			pAttackPlayer->SendMsg( &AddMsg, sizeof( AddMsg ) );

			++m_nLootedItemCount;
		}
	}
	else if( m_LootingItemArray[nArrayNum].nKind == eLI_MONEY )
	{
		MONEYTYPE AMoney	= pAttackPlayer->GetMoney();
		MONEYTYPE DMoney	= pDiePlayer->GetMoney();
		// 080602 LYW --- Player : ����ġ ��ġ (__int32) ���� (__int64) ������� ���� ó��.
		//MONEYTYPE LootMoney = m_LootingItemArray[nArrayNum].dwData;
		MONEYTYPE LootMoney = (DWORD)m_LootingItemArray[nArrayNum].dwData;

		ASSERT( DMoney >= LootMoney );

		MONEYTYPE Real = pAttackPlayer->SetMoney( LootMoney, MONEY_ADDITION, 0, eItemTable_Inventory, eMoneyLog_GetPKLooting, m_dwDiePlayer );
		pDiePlayer->SetMoney( Real, MONEY_SUBTRACTION, 0, eItemTable_Inventory, eMoneyLog_LosePKLooted, m_dwAttacker );

		MSG_DWORD3 MoneyMsg;
		MoneyMsg.Category	= MP_PK;
		MoneyMsg.dwData1	= m_dwDiePlayer;
		MoneyMsg.dwData2	= nArrayNum;
		MoneyMsg.dwData3	= Real;
		
		MoneyMsg.Protocol	= MP_PK_LOOTING_MOENYLOOTING;
		pAttackPlayer->SendMsg( &MoneyMsg, sizeof( MoneyMsg ) );
		MoneyMsg.Protocol	= MP_PK_LOOTING_MOENYLOOTED;
		pDiePlayer->SendMsg( &MoneyMsg, sizeof( MoneyMsg ) );

		LogItemMoney(m_dwDiePlayer, pDiePlayer->GetObjectName(), m_dwAttacker, pAttackPlayer->GetObjectName(),
				eLog_MoneyObtainPK, pDiePlayer->GetMoney(eItemTable_Inventory), pAttackPlayer->GetMoney(eItemTable_Inventory), 
				LootMoney, 0,  0, 0, 0, 0, pDiePlayer->GetPlayerExpPoint());

		++m_nLootedItemCount;
	}
	else if( m_LootingItemArray[nArrayNum].nKind == eLI_EXP )
	{
		EXPTYPE LootExp = m_LootingItemArray[nArrayNum].dwData;
		if( pDiePlayer->GetLevel() == 1 )
		{
			EXPTYPE DExp	= pDiePlayer->GetPlayerExpPoint();
			if( LootExp > DExp )
				LootExp = DExp;
		}

		pDiePlayer->ReduceExpPoint( LootExp , eExpLog_LosebyLooting );
		pAttackPlayer->AddPlayerExpPoint( LootExp );

		// 080602 LYW --- Player : ����ġ ��ġ (__int32) ���� (__int64) ������� ���� ó��.
		//MSG_DWORD3 ExpMsg;
		//ExpMsg.Category = MP_PK;
		//ExpMsg.dwData1	= m_dwDiePlayer;
		//ExpMsg.dwData2	= nArrayNum;
		//ExpMsg.dwData3	= LootExp;
		//ExpMsg.Protocol = MP_PK_LOOTING_EXPLOOTING;
		//pAttackPlayer->SendMsg( &ExpMsg, sizeof( ExpMsg ) );
		//ExpMsg.Protocol = MP_PK_LOOTING_EXPLOOTED;
		//pDiePlayer->SendMsg( &ExpMsg, sizeof( ExpMsg ) );

		//LogItemMoney(m_dwDiePlayer, pDiePlayer->GetObjectName(), m_dwAttacker, pAttackPlayer->GetObjectName(),
		//		eLog_ExpObtainPK, pDiePlayer->GetMoney(eItemTable_Inventory), pAttackPlayer->GetMoney(eItemTable_Inventory), 
		//		LootExp, 0, 0, 0, 0, 0, pDiePlayer->GetPlayerExpPoint());

		MSG_DWORDEX3 ExpMsg;
		ExpMsg.Category = MP_PK;
		ExpMsg.dweData1	= m_dwDiePlayer;
		ExpMsg.dweData2	= nArrayNum;
		ExpMsg.dweData3	= LootExp;
		ExpMsg.Protocol = MP_PK_LOOTING_EXPLOOTING;
		pAttackPlayer->SendMsg( &ExpMsg, sizeof( ExpMsg ) );
		ExpMsg.Protocol = MP_PK_LOOTING_EXPLOOTED;
		pDiePlayer->SendMsg( &ExpMsg, sizeof( ExpMsg ) );

		LogItemMoney(m_dwDiePlayer, pDiePlayer->GetObjectName(), m_dwAttacker, pAttackPlayer->GetObjectName(),
				eLog_ExpObtainPK, pDiePlayer->GetMoney(eItemTable_Inventory), pAttackPlayer->GetMoney(eItemTable_Inventory), 
				(MONEYTYPE)LootExp, 0, 0, 0, 0, 0, pDiePlayer->GetPlayerExpPoint());

		++m_nLootedItemCount;
	}

	m_LootingItemArray[nArrayNum].nKind		= eLI_SELECTED;
	m_LootingItemArray[nArrayNum].dwData	= 0;
	--m_nItemLootCount;

	return TRUE;

LOOTING_ERROR:

	if( !bForce )
	{
		MSG_DWORD2 msg;
		msg.Category	= MP_PK;
		msg.Protocol	= MP_PK_LOOTING_ERROR;
		msg.dwData1		= m_dwDiePlayer;
		msg.dwData2		= dwErrorCode;//errorcode
		pAttackPlayer->SendMsg( &msg, sizeof(msg) );
	}

	return TRUE;
}

void CLootingRoom::EndLootingRoomMsg( BOOL bMsgToAttacker )
{
	CPlayer* pAttackPlayer	= (CPlayer*)g_pUserTable->FindUser( m_dwAttacker );
	CPlayer* pDiePlayer		= (CPlayer*)g_pUserTable->FindUser( m_dwDiePlayer );

	MSG_DWORD EndMsg;
	EndMsg.Category		= MP_PK;
	EndMsg.Protocol		= MP_PK_LOOTING_ENDLOOTING;
	EndMsg.dwData		= m_dwDiePlayer;

	if( bMsgToAttacker )
	if( pAttackPlayer )
		pAttackPlayer->SendMsg( &EndMsg, sizeof( EndMsg ) );
	
	if( pDiePlayer )
		pDiePlayer->SendMsg( &EndMsg, sizeof( EndMsg ) );
}


void CLootingRoom::DiePlayerExitPanelty()
{
	m_nChance = PKLOOTING_ITEM_NUM;	//MAX
	int nPos = 0;
	while( m_nItemLootCount )
	{
		if( m_LootingItemArray[nPos].nKind == eLI_ITEM ||
			m_LootingItemArray[nPos].nKind == eLI_MONEY ||
			m_LootingItemArray[nPos].nKind == eLI_EXP )
		{
			if( Loot( nPos, TRUE ) == FALSE )
				break;
		}
		++nPos;
		if( nPos >= PKLOOTING_ITEM_NUM ) break;
	}

	//
	LOOTINGMGR->CloseLootingRoom( m_dwDiePlayer, TRUE );
}

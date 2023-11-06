#include "stdafx.h"
#include "ExchangeRoom.h"
#include "ExchangeManager.h"
#include "mapdbmsgparser.h"
#include "Player.h"
#include "ItemManager.h"
#include "..\[CC]Header\GameResourceStruct.h"

CExchangeRoom::CExchangeRoom()
{
	
}

CExchangeRoom::~CExchangeRoom()
{

}

void CExchangeRoom::Init( CPlayer* pPlayer1, CPlayer* pPlayer2 )
{
	sEXCHANGECONTAINER ExchangeContainer;

	m_ExchangeData[0].pPlayer		= pPlayer1;
	m_ExchangeData[0].bExchange		= FALSE;
	m_ExchangeData[0].bLock			= FALSE;
	m_ExchangeData[0].dwMoney		= 0;
	m_ExchangeData[0].nAddItemNum	= 0;
	ZeroMemory( m_ExchangeData[0].ItemInfo, sizeof(ITEMBASE)*MAX_EXCHANGEITEM );

	m_ExchangeData[1].pPlayer		= pPlayer2;
	m_ExchangeData[1].bExchange		= FALSE;
	m_ExchangeData[1].bLock			= FALSE;
	m_ExchangeData[1].dwMoney		= 0;
	m_ExchangeData[1].nAddItemNum	= 0;
	ZeroMemory( m_ExchangeData[1].ItemInfo, sizeof(ITEMBASE)*MAX_EXCHANGEITEM );

	ExchangeContainer.pRoom		= this;

	ExchangeContainer.nMyIndex	= 0;
	ExchangeContainer.pOpPlayer = pPlayer2;
	pPlayer1->SetExchangeContainer( &ExchangeContainer );

	ExchangeContainer.nMyIndex	= 1;
	ExchangeContainer.pOpPlayer = pPlayer1;
	pPlayer2->SetExchangeContainer( &ExchangeContainer );

	m_nExchangeState = EXCS_WAITING;
}

void CExchangeRoom::Exit()
{
//item lock Ǯ��
	CItemSlot* pItemSlot	= NULL;
	POSTYPE	ItemPos;
	
	for( int j = 0 ; j < 2 ; ++j )
	for( int i = 0 ; i < m_ExchangeData[j].nAddItemNum ; ++i )
	{
		ItemPos		= m_ExchangeData[j].ItemInfo[i].Position;
		pItemSlot	= m_ExchangeData[j].pPlayer->GetSlot( ItemPos );
		if( pItemSlot )
			pItemSlot->SetLock( ItemPos, FALSE );
	}	

//player data �ʱ�ȭ
	sEXCHANGECONTAINER ExchangeContainer;	//�ʱ�ȭ��
	if( m_ExchangeData[0].pPlayer )
		m_ExchangeData[0].pPlayer->SetExchangeContainer( &ExchangeContainer );
	if( m_ExchangeData[1].pPlayer )
		m_ExchangeData[1].pPlayer->SetExchangeContainer( &ExchangeContainer );
//
}

void CExchangeRoom::Lock( int nIndex, BOOL bLock )
{
	if( bLock )
		m_ExchangeData[nIndex].bLock = TRUE;
	else
	{
		m_ExchangeData[0].bLock = FALSE;
		m_ExchangeData[1].bLock = FALSE;

		m_ExchangeData[0].bExchange = FALSE;
		m_ExchangeData[1].bExchange = FALSE;		
	}
}

BOOL CExchangeRoom::IsAllLock()
{
	if( m_ExchangeData[0].bLock && m_ExchangeData[1].bLock )
		return TRUE;

	return FALSE;
}

void CExchangeRoom::SetExchange( int nIndex )
{
	m_ExchangeData[nIndex].bExchange = TRUE;
}

BOOL CExchangeRoom::IsAllExchange()
{
	if( m_ExchangeData[0].bExchange && m_ExchangeData[1].bExchange )
		return TRUE;

	return FALSE;
}

BOOL CExchangeRoom::AddItem( int nPlayerIndex, ITEMBASE* pItemInfo )
{
	if( IsLocked( nPlayerIndex ) ) return FALSE;
	if( m_ExchangeData[nPlayerIndex].nAddItemNum >= MAX_EXCHANGEITEM )
		return FALSE;

	if( pItemInfo->QuickPosition != 0 ) return FALSE;

//�������� ������ LOCK
	// RaMa
	eITEMTABLE Table = eItemTable_Inventory;
	ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( pItemInfo->wIconIdx );
	if(!pInfo)		return FALSE;

	CItemSlot* pItemSlot = NULL;	
/*	if( pInfo->ItemKind & eSHOP_ITEM )
	{
		Table = eItemTable_ShopInven;
		pItemSlot = (CShopInvenSlot*)m_ExchangeData[nPlayerIndex].pPlayer->GetSlot( eItemTable_ShopInven );

		if( pInfo->ItemType == 11 )
		if( !(pItemInfo->ItemParam & ITEM_PARAM_SEAL) )
			return FALSE;
		
		// �ŷ��Ұ� ������
		if( pInfo->ItemKind == eSHOP_ITEM_EQUIP && pInfo->BuyPrice )
			return FALSE;
		else if( pInfo->MeleeAttackMax )
			return FALSE;
	}
	else
*/		pItemSlot = (CInventorySlot*)m_ExchangeData[nPlayerIndex].pPlayer->GetSlot( eItemTable_Inventory );	 
	

	if( pItemSlot )
	{
/*		if( GetItemKind( pItemInfo->wIconIdx ) == eYOUNGYAK_ITEM )
		{
			if( pItemSlot->CheckQPosForItemIdx( pItemInfo->wIconIdx ) == FALSE )
				return FALSE;
		}
		else
*/		{
			if( pItemSlot->GetItemInfoAbs( pItemInfo->Position )->QuickPosition != 0 )
				return FALSE;
		}

		if( pItemSlot->IsLock( pItemInfo->Position ) ) return FALSE;
		pItemSlot->SetLock( pItemInfo->Position, TRUE );
	}
//
	m_ExchangeData[nPlayerIndex].ItemInfo[m_ExchangeData[nPlayerIndex].nAddItemNum] = *pItemInfo;
	++m_ExchangeData[nPlayerIndex].nAddItemNum;

	return TRUE;
}

BOOL CExchangeRoom::DelItem( int nPlayerIndex, POSTYPE pos )
{
	ASSERT( pos >=0 && pos < m_ExchangeData[nPlayerIndex].nAddItemNum );
	
	if( IsLocked( nPlayerIndex ) ) return FALSE;
	if( m_ExchangeData[nPlayerIndex].nAddItemNum <= 0 )
		return FALSE;
	
//�������� ������ UNLOCK
	POSTYPE ItemAbsPos = m_ExchangeData[nPlayerIndex].ItemInfo[pos].Position;

	CItemSlot* pItemSlot = m_ExchangeData[nPlayerIndex].pPlayer->GetSlot( ItemAbsPos );
	if( pItemSlot )
		pItemSlot->SetLock( ItemAbsPos, FALSE );
//

	--m_ExchangeData[nPlayerIndex].nAddItemNum;

	for( int i = pos ; i < m_ExchangeData[nPlayerIndex].nAddItemNum ; ++i )
	{
		m_ExchangeData[nPlayerIndex].ItemInfo[i] = m_ExchangeData[nPlayerIndex].ItemInfo[i+1];
	}

	return TRUE;
}

DWORD CExchangeRoom::InputMoney( int nPlayerIndex, DWORD dwMoney )
{
	if( dwMoney > m_ExchangeData[nPlayerIndex].pPlayer->GetMoney() )
		dwMoney = m_ExchangeData[nPlayerIndex].pPlayer->GetMoney();

	m_ExchangeData[nPlayerIndex].dwMoney = dwMoney;

	return dwMoney;
}

int CExchangeRoom::DoExchange()
{
	CInventorySlot* pInventory[2];
	CShopInvenSlot* pShopInventory[2];
	pInventory[0] = (CInventorySlot*)m_ExchangeData[0].pPlayer->GetSlot( eItemTable_Inventory );
	pInventory[1] = (CInventorySlot*)m_ExchangeData[1].pPlayer->GetSlot( eItemTable_Inventory );
	pShopInventory[0] = (CShopInvenSlot*)m_ExchangeData[0].pPlayer->GetSlot( eItemTable_ShopInven );
	pShopInventory[1] = (CShopInvenSlot*)m_ExchangeData[1].pPlayer->GetSlot( eItemTable_ShopInven );

	if( pInventory[0] == NULL || pInventory[1] == NULL || pShopInventory[0] == NULL || pShopInventory[1] == NULL )
		return eEE_ERROR;

	//���ڸ�üũ	//������ �ڸ��� ����Ѵ�?(ok)
	WORD EmptySellPos[2][MAX_EXCHANGEITEM];
	WORD ShopEmptySellPos[2][MAX_EXCHANGEITEM];
	WORD sOut;

	// RaMa
	int i, j;
	ITEM_INFO* pItemInfo = NULL;
	WORD shopitemcount[2] = { 0, };
	WORD itemcount[2] = { 0, };

	for( j=0; j<2; j++ )
	{
		for( i = 0 ; i < m_ExchangeData[j].nAddItemNum ; i++ )
		{
			pItemInfo = ITEMMGR->GetItemInfo( m_ExchangeData[j].ItemInfo[i].wIconIdx );
			if(!pItemInfo)
				return eEE_ERROR;

//			if( pItemInfo->ItemKind & eSHOP_ITEM )
//				++shopitemcount[j];
//			else
				++itemcount[j];
		}
	}
	//

	if( itemcount[1] > itemcount[0] )
	{
		sOut = pInventory[0]->GetEmptyCell( EmptySellPos[0], itemcount[1] - itemcount[0] );
		if( sOut != itemcount[1] - itemcount[0] )
			return eEE_NOTENOUGHSPACE;
	}
	if( itemcount[0] > itemcount[1] )
	{
		sOut = pInventory[1]->GetEmptyCell( EmptySellPos[1], itemcount[0] - itemcount[1] );
		if( sOut != itemcount[0] - itemcount[1] )
			return eEE_NOTENOUGHSPACE;
	}
	if( shopitemcount[1] > shopitemcount[0] )
	{
		sOut = pShopInventory[0]->GetEmptyCell( ShopEmptySellPos[0], shopitemcount[1] - shopitemcount[0] );
		if( sOut != shopitemcount[1] - shopitemcount[0] )
			return eEE_NOTENOUGHSPACE;
	}
	if( shopitemcount[0] > shopitemcount[1] )
	{
		sOut = pShopInventory[1]->GetEmptyCell( ShopEmptySellPos[1], shopitemcount[0] - shopitemcount[1] );
		if( sOut != shopitemcount[0] - shopitemcount[1] )
			return eEE_NOTENOUGHSPACE;
	}


/*
	if( m_ExchangeData[1].nAddItemNum > m_ExchangeData[0].nAddItemNum )
	{
		sOut = pInventory[0]->GetEmptyCell( EmptySellPos[0], itemcount[1] - itemcount[0] );
		if( sOut != itemcount[1] - itemcount[0] )
			return eEE_NOTENOUGHSPACE;
		sOut = pShopInventory[0]->GetEmptyCell( ShopEmptySellPos[0], shopitemcount[1] - shopitemcount[0] );
		if( sOut != shopitemcount[1] - shopitemcount[0] )
			return eEE_NOTENOUGHSPACE;
	}
	if( m_ExchangeData[0].nAddItemNum > m_ExchangeData[1].nAddItemNum )
	{
		sOut = pInventory[1]->GetEmptyCell( EmptySellPos[1], itemcount[0] - itemcount[1] );
		if( sOut != itemcount[0] - itemcount[1] )
			return eEE_NOTENOUGHSPACE;
		sOut = pShopInventory[1]->GetEmptyCell( ShopEmptySellPos[1], shopitemcount[0] - shopitemcount[1] );
		if( sOut != shopitemcount[0] - shopitemcount[1] )
			return eEE_NOTENOUGHSPACE;
	}
*/
	
	//��üũ
	if( m_ExchangeData[0].dwMoney > m_ExchangeData[0].pPlayer->GetMoney() )
		return eEE_NOTENOUGHMONEY;
	if( m_ExchangeData[1].dwMoney > m_ExchangeData[1].pPlayer->GetMoney() )
		return eEE_NOTENOUGHMONEY;

	if( m_ExchangeData[1].dwMoney + m_ExchangeData[0].pPlayer->GetMoney() > MAX_INVENTORY_MONEY )
		return eEE_MAXMONEY;
	if( m_ExchangeData[0].dwMoney + m_ExchangeData[1].pPlayer->GetMoney() > MAX_INVENTORY_MONEY )
		return eEE_MAXMONEY;

	
///////////////////////////////////////////////
//������ �˻� �ڵ� ����.. ���ڸ��� �� ����ֳ�?	�׽�Ʈ �ʿ�.
	for( j = 0 ; j < 2 ; ++ j )
	{
		for( i = 0 ; i < m_ExchangeData[j].nAddItemNum ; ++i )
		{
			//const ITEMBASE* pItemBase = pInventory[j]->GetItemInfoAbs( m_ExchangeData[j].ItemInfo[i].Position );
			// RaMa
			const ITEMBASE* pItemBase = NULL;
			pItemInfo = ITEMMGR->GetItemInfo( m_ExchangeData[j].ItemInfo[i].wIconIdx );			
//			if( pItemInfo->ItemKind & eSHOP_ITEM )
//				pItemBase = pShopInventory[j]->GetItemInfoAbs( m_ExchangeData[j].ItemInfo[i].Position );
//			else
				pItemBase = pInventory[j]->GetItemInfoAbs( m_ExchangeData[j].ItemInfo[i].Position );

			if( pItemBase->dwDBIdx != m_ExchangeData[j].ItemInfo[i].dwDBIdx ||
				pItemBase->wIconIdx != m_ExchangeData[j].ItemInfo[i].wIconIdx ||
				pItemBase->Durability != m_ExchangeData[j].ItemInfo[i].Durability ||
				pItemBase->Position != m_ExchangeData[j].ItemInfo[i].Position )
				return eEE_ERROR;			//���� �κ��丮�� ��ȯâ�� ������ �ٸ���.

			if( ITEMMGR->IsDupItem(pItemBase->wIconIdx) )
			if( pItemBase->Durability > MAX_YOUNGYAKITEM_DUPNUM/*MAX_DURABILITY*/ )
				return eEE_ERROR;
		}
	}

	
	
//�����·� ������ �����Ѱ�?
/////////////
//EXCHANGE!!!
	for( j = 0 ; j < 2 ; ++ j )
	{
		for( i = 0 ; i < m_ExchangeData[j].nAddItemNum ; ++i )
		{
			ITEMBASE ItemBase;
//			pInventory[j]->DeleteItemAbs( NULL, m_ExchangeData[j].ItemInfo[i].Position, &ItemBase, SS_LOCKOMIT );	//�߿�! üũ.			
			pItemInfo = ITEMMGR->GetItemInfo( m_ExchangeData[j].ItemInfo[i].wIconIdx );
//			if( pItemInfo->ItemKind & eSHOP_ITEM )
//				pShopInventory[j]->DeleteItemAbs( NULL, m_ExchangeData[j].ItemInfo[i].Position, &ItemBase, SS_LOCKOMIT );	//�߿�! üũ.
//			else
				pInventory[j]->DeleteItemAbs( NULL, m_ExchangeData[j].ItemInfo[i].Position, &ItemBase, SS_LOCKOMIT );	//�߿�! üũ.


			//���� �޼���..	//��Ƽ� �Ѳ����� ��������.
			MSG_EXCHANGE_REMOVEITEM RemoveMsg;
			RemoveMsg.Category = MP_EXCHANGE;
			RemoveMsg.Protocol = MP_EXCHANGE_REMOVE;
			RemoveMsg.wAbsPosition = m_ExchangeData[j].ItemInfo[i].Position;
			m_ExchangeData[j].pPlayer->SendMsg( &RemoveMsg, sizeof( RemoveMsg ) );
		}
	}

/*
	sOut = pInventory[0]->GetEmptyCell( EmptySellPos[0], m_ExchangeData[1].nAddItemNum );
	if( sOut != m_ExchangeData[1].nAddItemNum )
		return FALSE;

	sOut = pInventory[1]->GetEmptyCell( EmptySellPos[1], m_ExchangeData[0].nAddItemNum );
	if( sOut != m_ExchangeData[0].nAddItemNum  )
		return FALSE;
*/	

	for( j = 0 ; j < 2 ; ++ j )
	{
		for( i = 0 ; i < m_ExchangeData[j].nAddItemNum ; ++i )
		{
			POSTYPE EmptyPos;
			/*if( pInventory[!j]->GetEmptyCell( &EmptyPos ) == 0 ) //����� ������ ����üũ�ϱ�
			{
				ASSERT(0);
				continue;
			}*/
			// RaMa
			pItemInfo = ITEMMGR->GetItemInfo( m_ExchangeData[j].ItemInfo[i].wIconIdx );
/*			if( pItemInfo->ItemKind & eSHOP_ITEM )
			{
				if( pShopInventory[!j]->GetEmptyCell( &EmptyPos ) == 0 ) //����� ������ ����üũ�ϱ�
				{
					ASSERT(0);
					continue;
				}
			}
			else
*/			{
				if( pInventory[!j]->GetEmptyCell( &EmptyPos ) == 0 ) //����� ������ ����üũ�ϱ�
				{
					ASSERT(0);
					continue;
				}
			}


			ItemUpdateToDB( m_ExchangeData[!j].pPlayer->GetID(), m_ExchangeData[j].ItemInfo[i].dwDBIdx,
							m_ExchangeData[j].ItemInfo[i].wIconIdx, m_ExchangeData[j].ItemInfo[i].Durability,
							EmptyPos, 0 );

			m_ExchangeData[j].ItemInfo[i].Position = EmptyPos;

/////

			// RaMa
			int rt;
//			if( pItemInfo->ItemKind & eSHOP_ITEM )
//				rt = pShopInventory[!j]->InsertItemAbs( NULL, EmptyPos, &m_ExchangeData[j].ItemInfo[i], SS_LOCKOMIT ); //Itembase���� position�� �ٲ��.
//			else
				rt = pInventory[!j]->InsertItemAbs( NULL, EmptyPos, &m_ExchangeData[j].ItemInfo[i], SS_LOCKOMIT ); //Itembase���� position�� �ٲ��.
			ASSERT( rt == EI_TRUE );

			//�߰� �޼���...	//��Ƽ� �ѹ��� ������ �ɷ� ��ü����. confirm
			MSG_ITEM InsertMsg;
			InsertMsg.Category = MP_EXCHANGE;
			InsertMsg.Protocol = MP_EXCHANGE_INSERT;
			InsertMsg.ItemInfo = m_ExchangeData[j].ItemInfo[i];
			m_ExchangeData[!j].pPlayer->SendMsg( &InsertMsg, sizeof( MSG_ITEM ) );

			LogItemMoney(m_ExchangeData[j].pPlayer->GetID(), m_ExchangeData[j].pPlayer->GetObjectName(), 
						m_ExchangeData[1-j].pPlayer->GetID(), m_ExchangeData[1-j].pPlayer->GetObjectName(),
						eLog_Exchange, m_ExchangeData[j].pPlayer->GetMoney(), m_ExchangeData[1-j].pPlayer->GetMoney(), 
						0, m_ExchangeData[j].ItemInfo[i].wIconIdx, m_ExchangeData[j].ItemInfo[i].dwDBIdx, m_ExchangeData[j].ItemInfo[i].Position, 0, m_ExchangeData[j].ItemInfo[i].Durability,
						m_ExchangeData[j].pPlayer->GetPlayerExpPoint());
		}
	}

	DWORD	dwMoney = 0;
	MSG_DWORD MoneyMsg;
	MoneyMsg.Category	= MP_EXCHANGE;
	MoneyMsg.Protocol	= MP_EXCHANGE_SETMONEY;

	int FromNum =0;
	for( j = 0 ; j < 2 ; ++j )
	{
		if( m_ExchangeData[j].dwMoney > m_ExchangeData[!j].dwMoney )	//���� �ǵ��� �� ����.
		{
			dwMoney = m_ExchangeData[j].dwMoney - m_ExchangeData[!j].dwMoney;
			m_ExchangeData[j].pPlayer->SetMoney( dwMoney, MONEY_SUBTRACTION, 0, eItemTable_Inventory, eMoneyLog_LoseExchange, m_ExchangeData[1-j].pPlayer->GetID() );
			FromNum = j;
		}
		else if( m_ExchangeData[j].dwMoney < m_ExchangeData[!j].dwMoney )
		{
			dwMoney = m_ExchangeData[!j].dwMoney - m_ExchangeData[j].dwMoney;
			m_ExchangeData[j].pPlayer->SetMoney( dwMoney, MONEY_ADDITION, 0, eItemTable_Inventory, eMoneyLog_GetExchange, m_ExchangeData[1-j].pPlayer->GetID() );
		}
		else
		{
			continue;
		}

//		MoneyMsg.dwData	= m_ExchangeData[j].pPlayer->GetMoney();
//		m_ExchangeData[j].pPlayer->SendMsg( &MoneyMsg, sizeof( MoneyMsg ) );
	}

	LogItemMoney(m_ExchangeData[FromNum].pPlayer->GetID(), m_ExchangeData[FromNum].pPlayer->GetObjectName(),
		m_ExchangeData[!FromNum].pPlayer->GetID(), m_ExchangeData[!FromNum].pPlayer->GetObjectName(),
		eLog_Exchange, m_ExchangeData[FromNum].pPlayer->GetMoney(), m_ExchangeData[!FromNum].pPlayer->GetMoney(), 
		dwMoney, 0, 0, 0, 0, 0,
		m_ExchangeData[FromNum].pPlayer->GetPlayerExpPoint());

	return eEE_OK;
}

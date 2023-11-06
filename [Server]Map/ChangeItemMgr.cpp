// ChangeItemMgr.cpp: implementation of the CChangeItemMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChangeItemMgr.h"
#include "MHFile.h"
#include "../[CC]Header/GameResourceStruct.h"
#include "ItemManager.h"
#include "Player.h"
#include "ObjectStateManager.h"
#include "MapDBMsgParser.h"
#include "CheckRoutine.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChangeItemMgr::CChangeItemMgr()
{

}

CChangeItemMgr::~CChangeItemMgr()
{
	Release();
}

void CChangeItemMgr::Init()
{
	m_ChangeItemList.Initialize( 300 );
	m_MultiChangeItemList.Initialize( 100 );
	LoadItemChange();
}

void CChangeItemMgr::Release()
{
	sCHANGEITEM* pData = NULL;
	m_ChangeItemList.SetPositionHead();
	while( pData = m_ChangeItemList.GetData() )
	{
//		delete[] pData->pItemUnit;
		delete pData;
	}
	m_ChangeItemList.RemoveAll();	

	sMULTICHANGEITEM* pData1 = NULL;
	m_MultiChangeItemList.SetPositionHead();
	while( pData1 = m_MultiChangeItemList.GetData() )
	{
		delete[] pData1->pChangeItem;
		delete pData1;
	}
	m_MultiChangeItemList.RemoveAll();	
}

BOOL CChangeItemMgr::LoadItemChange()
{
	CMHFile file;
	if( !file.Init( "./System/Resource/ItemChangeList.bin", "rb" ) )
		return FALSE;
	while( 1 )
	{
		if( file.IsEOF() )
			break;
		sCHANGEITEM* pData = new sCHANGEITEM;
		pData->wItemIdx = file.GetWord();
		pData->wMaxToItem = file.GetWord();
		pData->pItemUnit = new sCHANGEITEMUNIT[pData->wMaxToItem];
		for( WORD i = 0; i < pData->wMaxToItem; ++i )
		{
			pData->pItemUnit[i].wToItemIdx = file.GetWord();
			pData->pItemUnit[i].dwToItemDur = file.GetDword();
			pData->pItemUnit[i].dwPercent = file.GetDword();
		}

		m_ChangeItemList.Add( pData, pData->wItemIdx );
	}
	file.Release();

	if( !file.Init( "./System/Resource/ItemChangeListMulti.bin", "rb" ) )
		return FALSE;
	while( 1 )
	{
		if( file.IsEOF() )
			break;
		sMULTICHANGEITEM* pData = new sMULTICHANGEITEM;
		pData->wItemIdx = file.GetWord();
		pData->wLimitLevel = file.GetWord();
		pData->wMaxSet = file.GetWord();
		pData->pChangeItem = new sCHANGEITEM[pData->wMaxSet];
		for( WORD i = 0; i < pData->wMaxSet; ++i )
		{
			pData->pChangeItem[i].wItemIdx = pData->wItemIdx;
			pData->pChangeItem[i].wMaxToItem = file.GetWord();
			pData->pChangeItem[i].pItemUnit = new sCHANGEITEMUNIT[pData->pChangeItem[i].wMaxToItem];
			for( WORD j = 0; j < pData->pChangeItem[i].wMaxToItem; ++j )
			{
				pData->pChangeItem[i].pItemUnit[j].wToItemIdx = file.GetWord();
				pData->pChangeItem[i].pItemUnit[j].dwToItemDur = file.GetDword();
				pData->pChangeItem[i].pItemUnit[j].dwPercent = file.GetDword();
			}
		}

		pData->nMaxItemSpace = ChangedTotalItemNum(pData,pData->wMaxSet);

		m_MultiChangeItemList.Add( pData, pData->wItemIdx );
	}
	file.Release();

	return TRUE;	
}

int CChangeItemMgr::UseChangeItem( CPlayer* pPlayer, WORD TargetPos, DWORD wItemIdx )
{
	if( pPlayer->GetState() == eObjectState_Die )
		return 0;
	if( !CHKRT->ItemOf( pPlayer, TargetPos, wItemIdx, 0, 0, CB_EXIST|CB_ICONIDX) )
		return 0;
	if( pPlayer->GetState() == eObjectState_Immortal )
		OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer, eObjectState_Immortal );

	CItemSlot* pSlot = pPlayer->GetSlot( TargetPos );
	const ITEMBASE* pItemBase = pSlot->GetItemInfoAbs( TargetPos );
	ITEMBASE Item = *pItemBase;
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( wItemIdx );
	
/*	if( pItemInfo )
	if( pItemInfo->ItemKind & eCHANGE_ITEM )
	{
		if( m_MultiChangeItemList.GetData( wItemIdx ) )
		{
			return UseMultiChangeItem( pPlayer, TargetPos, wItemIdx, &Item );
		}
		else
		{
			return UseNormalChangeItem( pPlayer, TargetPos, wItemIdx, &Item );
		}
	}
	else
		return 0;
*/
	return 1;
}

int CChangeItemMgr::UseNormalChangeItem( CPlayer* pPlayer, WORD TargetPos, DWORD wItemIdx, ITEMBASE* pItem )
{
	//�÷��̾��� �����ִ� ���� ������ �˾Ƴ���.
	CItemSlot* pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	if(NULL == pSlot) ASSERTMSG(0,"�ùٸ� ������ ������ ���� �� �����ϴ�.");

	// 071211 KTH -- pPlayer �߰�
	unsigned int nNum = ITEMMGR->GetTotalEmptySlotNum(pSlot, pPlayer);

	sCHANGEITEMUNIT* pItemUnit = GetItemUnitFromCalPercent( wItemIdx );
	if( !pItemUnit )	return 0;

	unsigned int haveSpaceItemtotalcount = 0;
	
	if( pItemUnit->wToItemIdx == 7999 ) //money
	{

	}
	else if( pItemUnit->wToItemIdx == 7997 )	// ����ġ
	{

	}
	else
	{
		if(ITEMMGR->IsDupItem(pItemUnit->wToItemIdx))
		{
			//���� ������ �˾Ƴ�� 5�γ�����.
			unsigned int devidecount = pItemUnit->dwToItemDur/5;

			haveSpaceItemtotalcount += devidecount;
		}
		else
		{
			haveSpaceItemtotalcount += pItemUnit->dwToItemDur;
		}
	}

	if( (nNum+1)  < haveSpaceItemtotalcount)
	{
		//������ �����ϴٴ� �޽����� ������.
		MSG_WORD msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_CHANGEITEM_NACK;
		msg.wData = NOT_SPACE;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return 0;
	}
	

	if( EI_TRUE != ITEMMGR->DiscardItem( pPlayer, TargetPos, wItemIdx, 1 ) )
		return 0;
	LogItemMoney( pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
				  eLog_ItemDestroybyChangeItem, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
				  pItem->wIconIdx, pItem->dwDBIdx, pItem->Position, 0, pItem->Durability, pPlayer->GetPlayerExpPoint() );	

	// send item use
	MSG_ITEM_USE_ACK msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_USE_ACK;
	msg.dwObjectID = pPlayer->GetID();
	msg.TargetPos = TargetPos;
	msg.wItemIdx = wItemIdx;
	ITEMMGR->SendAckMsg( pPlayer, &msg, sizeof(msg) );

	if( pItemUnit->wToItemIdx == 7999 )		// money
	{
		pPlayer->SetMoney( pItemUnit->dwToItemDur, MONEY_ADDITION, 64 );	//�ӽ� 64
	}
	/*�����Ƽ ���� - else if( pItemUnit->wToItemIdx == 7997 )	// ����ġ
	{
		pPlayer->AddAbilityExp( pItemUnit->dwToItemDur );
	}*/
	else if( pItemUnit->wToItemIdx == 7998 )
	{
		// event 050203
		MSG_NAME msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_EVENTITEM_USE;
		SafeStrCpy( msg.Name, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1 );
//		strcpy( msg.Name, pPlayer->GetObjectName() );
		pPlayer->SendMsg( &msg, sizeof(msg) );
	}
	else if( pItemUnit->wToItemIdx == 7996 )
	{
		MSG_NAME msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_EVENTITEM_USE2;
		SafeStrCpy( msg.Name, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1 );
		pPlayer->SendMsg( &msg, sizeof(msg) );
	}
	else
		ITEMMGR->ObtainItemFromChangeItem( pPlayer, pItemUnit->wToItemIdx, (WORD)pItemUnit->dwToItemDur );

	return 1;
}

unsigned int CChangeItemMgr::ChangedTotalItemNum(sMULTICHANGEITEM* pMultiChangeItem,WORD wMaxSet)//060613 Add by wonju 
{
	unsigned int haveSpaceItemtotalcount = 0;

	sCHANGEITEMUNIT* pItemUnit = NULL;
	for( WORD i = 0; i < pMultiChangeItem->wMaxSet; ++i )
	{
		DWORD nMin = 0;

		if( pMultiChangeItem->pChangeItem[i].wMaxToItem == 1 )
		{
			haveSpaceItemtotalcount++;
		}
		else
			pItemUnit = GetMaxSpaceItemRef( &pMultiChangeItem->pChangeItem[i] ,nMin);

		if( pItemUnit )
		{
			if( pItemUnit->wToItemIdx == 7999 ) //money
			{
				
			}
			else if( pItemUnit->wToItemIdx == 7997 )	// ����ġ
			{
				
			}
			else
			{
				if(ITEMMGR->IsDupItem(pItemUnit->wToItemIdx)) //���ذ� �Ǵ� ���̸�
				{
					if(nMin > (pItemUnit->dwToItemDur/5)) //���ذ� �ȵǴ� ������ ���� �ִ밪���� �۴ٸ�
					{
						haveSpaceItemtotalcount += nMin; //���ذ� �ȵǴ� ������ ���� �ִ밪�� �ִ´�.
					}
					else
					{
						haveSpaceItemtotalcount += pItemUnit->dwToItemDur/5;
					}
				}
				else//������ �� ���ٸ� �״�� ����
				{
					haveSpaceItemtotalcount += pItemUnit->dwToItemDur;
				}

				
			}
		}
	}

	return haveSpaceItemtotalcount;
}

int CChangeItemMgr::UseMultiChangeItem( CPlayer* pPlayer, WORD TargetPos, DWORD wItemIdx, ITEMBASE* pItem )
{
	CItemSlot* pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	if(NULL == pSlot) ASSERTMSG(0,"�ùٸ� ������ ������ ���� �� �����ϴ�.");

	// 071211 KTH -- pPlayer �߰�
	unsigned int nNum = ITEMMGR->GetTotalEmptySlotNum(pSlot, pPlayer);

	sMULTICHANGEITEM* pMultiChangeItem = m_MultiChangeItemList.GetData( wItemIdx );

	 
	//��Ʈ�� �����Ǿ� �ִ� �������� ������ �������� �Ǵ��ϰ�
	//������ �������� ��� �ִ����� �Ǵ��Ѵ�.
	

	//�������� ������ ����� ����
	unsigned int haveSpaceItemtotalcount = pMultiChangeItem->nMaxItemSpace;

	if( (nNum+1)  < haveSpaceItemtotalcount)
	{
		//������ �����ϴٴ� �޽����� ������.
		MSG_WORD msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_CHANGEITEM_NACK;
		msg.wData = NOT_SPACE;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return 0;
	}

	if( pPlayer->GetMaxLevel() < pMultiChangeItem->wLimitLevel )	return 0;		
	if( EI_TRUE != ITEMMGR->DiscardItem( pPlayer, TargetPos, wItemIdx, 1 ) )	return 0;

	// log
	LogItemMoney( pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
				  eLog_ItemDestroybyChangeItem, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
				  pItem->wIconIdx, pItem->dwDBIdx, pItem->Position, 0, pItem->Durability, pPlayer->GetPlayerExpPoint() );	

	// send item use
	MSG_ITEM_USE_ACK msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_USE_ACK;
	msg.dwObjectID = pPlayer->GetID();
	msg.TargetPos = TargetPos;
	msg.wItemIdx = wItemIdx;
	ITEMMGR->SendAckMsg( pPlayer, &msg, sizeof(msg) );

	sCHANGEITEMUNIT* pItemUnit = NULL;
	for( WORD i = 0; i < pMultiChangeItem->wMaxSet; ++i )
	{
		if( pMultiChangeItem->pChangeItem[i].wMaxToItem == 1 )
			pItemUnit = &pMultiChangeItem->pChangeItem[i].pItemUnit[0];
		else
			pItemUnit = GetMultiItemUnitFromCalPercent( &pMultiChangeItem->pChangeItem[i] );

		if( pItemUnit )
		{
			if( pItemUnit->wToItemIdx == 7999 )
			{
				pPlayer->SetMoney( pItemUnit->dwToItemDur, MONEY_ADDITION, 64 );
			}
			/*�����Ƽ ���� - else if( pItemUnit->wToItemIdx == 7997 )	// ����ġ
			{
				pPlayer->AddAbilityExp( pItemUnit->dwToItemDur );
			}*/
			else
			{
				ITEMMGR->ObtainItemFromChangeItem( pPlayer, pItemUnit->wToItemIdx, (WORD)pItemUnit->dwToItemDur );
			}
		}
	}

	return 1;
}

sCHANGEITEMUNIT* CChangeItemMgr::GetItemUnitFromCalPercent( DWORD wItemIdx )
{
	sCHANGEITEM* pData = m_ChangeItemList.GetData( wItemIdx );
	if( pData == NULL ) return NULL;

	DWORD RandRate = rand()%MAX_CHANGE_RATE;

	DWORD FromPercent = 0;
	DWORD ToPercent = 0;

	for( WORD i = 0; i < pData->wMaxToItem; ++i )
	{
		FromPercent = ToPercent;
		ToPercent += pData->pItemUnit[i].dwPercent;
		if( FromPercent <= RandRate && RandRate < ToPercent )
		{
			return &pData->pItemUnit[i];
		}
	}

	return NULL;
}

sCHANGEITEMUNIT* CChangeItemMgr::GetMaxSpaceItemRef(sCHANGEITEM* pSet , DWORD& nMin )
{
	//���� ���� ������ �����ϴ� �������� �����͸� �����ش�.
	DWORD MaxSpaceCount = 0;
	WORD MaxSpaceItemIdx = 0;
	for( WORD i = 0; i < pSet->wMaxToItem; ++i )
	{
		if(MaxSpaceCount < pSet->pItemUnit[i].dwToItemDur)
		{
			MaxSpaceCount = pSet->pItemUnit[i].dwToItemDur;
			MaxSpaceItemIdx = i;
		}

		//�������� �ʴ� �������� �ִ밪�� ���Ѵ�.
		if(FALSE == ITEMMGR->IsDupItem(pSet->pItemUnit[i].wToItemIdx))
		{
			if(nMin < pSet->pItemUnit[i].dwToItemDur)
				nMin = pSet->pItemUnit[i].dwToItemDur;
		}
	}

	return &pSet->pItemUnit[MaxSpaceItemIdx];
}

sCHANGEITEMUNIT* CChangeItemMgr::GetMultiItemUnitFromCalPercent( sCHANGEITEM* pSet )
{
	DWORD RandRate = rand()%MAX_CHANGE_RATE;	


	DWORD FromPercent = 0;
	DWORD ToPercent = 0;

	for( WORD i = 0; i < pSet->wMaxToItem; ++i )
	{
		FromPercent = ToPercent;
		ToPercent += pSet->pItemUnit[i].dwPercent;
		if( FromPercent <= RandRate && RandRate < ToPercent )
		{
			return &pSet->pItemUnit[i];
		}
	}

	return NULL;
}


//////////////////////////////////////////////////////////////////////////
// [START] RaMa - 06.04.19  Shop���������� ChangeItem���
//////////////////////////////////////////////////////////////////////////
int	CChangeItemMgr::UseChangeItemFromShopItem( CPlayer* pPlayer, DWORD wItemIdx )
{
	if( pPlayer->GetState() == eObjectState_Die )
		return 0;
	if( pPlayer->GetState() == eObjectState_Immortal )
		OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer, eObjectState_Immortal );

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( wItemIdx );

/*	if( pItemInfo )
		if( pItemInfo->ItemKind == eSHOP_ITEM_SUNDRIES )
		{
			if( m_MultiChangeItemList.GetData( wItemIdx ) )
			{
				return UseMultiChangeItemFromShopItem( pPlayer, wItemIdx );
			}
			else
			{
				return UseNormalChangeItemFromShopItem( pPlayer, wItemIdx );
			}
		}
		else
			return 0;
*/
	return 1;
}

int CChangeItemMgr::UseNormalChangeItemFromShopItem( CPlayer* pPlayer, DWORD wItemIdx )
{
	//�÷��̾��� �����ִ� ���� ������ �˾Ƴ���.
	CItemSlot* pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	if(NULL == pSlot) ASSERTMSG(0,"�ùٸ� ������ ������ ���� �� �����ϴ�.");

	// 071211 KTH -- pPlayer �߰�
	unsigned int nNum = ITEMMGR->GetTotalEmptySlotNum(pSlot, pPlayer);

	sCHANGEITEMUNIT* pItemUnit = GetItemUnitFromCalPercent( wItemIdx );
	if( !pItemUnit )	return 0;

	unsigned int haveSpaceItemtotalcount = 0;

	if( pItemUnit->wToItemIdx == 7999 ) //money
	{

	}
	else if( pItemUnit->wToItemIdx == 7997 )	// ����ġ
	{

	}
	else
	{
		if(ITEMMGR->IsDupItem(pItemUnit->wToItemIdx))
		{
			//���� ������ �˾Ƴ�� 5�γ�����.
			unsigned int devidecount = pItemUnit->dwToItemDur/5;

			haveSpaceItemtotalcount += devidecount;
		}
		else
		{
			haveSpaceItemtotalcount += pItemUnit->dwToItemDur;
		}
	}

	if( (nNum+1)  < haveSpaceItemtotalcount)
	{
		//������ �����ϴٴ� �޽����� ������.
		MSG_WORD msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_CHANGEITEM_NACK;
		msg.wData = NOT_SPACE;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return 0;
	}

	if( pItemUnit->wToItemIdx == 7999 )		// money
	{
		pPlayer->SetMoney( pItemUnit->dwToItemDur, MONEY_ADDITION );
	}
	/*�����Ƽ ���� - else if( pItemUnit->wToItemIdx == 7997 )	// ����ġ
	{
		pPlayer->AddAbilityExp( pItemUnit->dwToItemDur );
	}*/
	else if( pItemUnit->wToItemIdx == 7998 )
	{
		// event 050203
		MSG_NAME msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_EVENTITEM_USE;
		SafeStrCpy( msg.Name, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1 );
		//		strcpy( msg.Name, pPlayer->GetObjectName() );
		pPlayer->SendMsg( &msg, sizeof(msg) );
	}
	else if( pItemUnit->wToItemIdx == 7996 )
	{
		MSG_NAME msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_EVENTITEM_USE2;
		SafeStrCpy( msg.Name, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1 );
		pPlayer->SendMsg( &msg, sizeof(msg) );
	}
	else
		ITEMMGR->ObtainItemFromChangeItem( pPlayer, pItemUnit->wToItemIdx, (WORD)pItemUnit->dwToItemDur );

	return 1;
}

int CChangeItemMgr::UseMultiChangeItemFromShopItem( CPlayer* pPlayer, DWORD wItemIdx )
{
	CItemSlot* pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	if(NULL == pSlot) ASSERTMSG(0,"�ùٸ� ������ ������ ���� �� �����ϴ�.");

	// 071211 KTH -- pPlayer �߰�
	unsigned int nNum = ITEMMGR->GetTotalEmptySlotNum(pSlot, pPlayer);

	sMULTICHANGEITEM* pMultiChangeItem = m_MultiChangeItemList.GetData( wItemIdx );

	//��Ʈ�� �����Ǿ� �ִ� �������� ������ �������� �Ǵ��ϰ�
	//������ �������� ��� �ִ����� �Ǵ��Ѵ�.


	//�������� ������ ����� ����
	unsigned int haveSpaceItemtotalcount = pMultiChangeItem->nMaxItemSpace;

	if( (nNum+1)  < haveSpaceItemtotalcount)
	{
		//������ �����ϴٴ� �޽����� ������.
		MSG_WORD msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_CHANGEITEM_NACK;
		msg.wData = NOT_SPACE;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return 0;
	}

	if( pPlayer->GetMaxLevel() < pMultiChangeItem->wLimitLevel )	return 0;		

	sCHANGEITEMUNIT* pItemUnit = NULL;
	for( WORD i = 0; i < pMultiChangeItem->wMaxSet; ++i )
	{
		if( pMultiChangeItem->pChangeItem[i].wMaxToItem == 1 )
			pItemUnit = &pMultiChangeItem->pChangeItem[i].pItemUnit[0];
		else
			pItemUnit = GetMultiItemUnitFromCalPercent( &pMultiChangeItem->pChangeItem[i] );

		if( pItemUnit )
		{
			if( pItemUnit->wToItemIdx == 7999 )
			{
				pPlayer->SetMoney( pItemUnit->dwToItemDur, MONEY_ADDITION );
			}
			/*�����Ƽ ���� - else if( pItemUnit->wToItemIdx == 7997 )	// ����ġ
			{
				pPlayer->AddAbilityExp( pItemUnit->dwToItemDur );
			}*/
			else
			{
				ITEMMGR->ObtainItemFromChangeItem( pPlayer, pItemUnit->wToItemIdx, (WORD)pItemUnit->dwToItemDur );
			}
		}
	}

	return 1;
}
//////////////////////////////////////////////////////////////////////////
// [END] RaMa - 06.04.19  Shop���������� ChangeItem���
//////////////////////////////////////////////////////////////////////////


/*
int CChangeItemMgr::UseChangeItem( CPlayer* pPlayer, WORD TargetPos, WORD wItemIdx )
{
	if( pPlayer->GetState() == eObjectState_Die )
		return 0;
	if( !CHKRT->ItemOf( pPlayer, TargetPos, wItemIdx, 0, 0, CB_EXIST|CB_ICONIDX) )
		return 0;
	if( pPlayer->GetState() == eObjectState_Immortal )
		OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer, eObjectState_Immortal );

	CItemSlot* pSlot = pPlayer->GetSlot( TargetPos );
	const ITEMBASE* pItemBase = pSlot->GetItemInfoAbs( TargetPos );
	ITEMBASE Item = *pItemBase;
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( wItemIdx );
	
	if( pItemInfo )
	if( pItemInfo->ItemKind == eCHANGE_ITEM )
	{
		sCHANGEITEMUNIT* pItemUnit = GetItemUnitFromCalPercent( wItemIdx );
		if( pItemUnit )
		{
			if( EI_TRUE != ITEMMGR->DiscardItem( pPlayer, TargetPos, wItemIdx, 1 ) )
				return 0;

			LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
			eLog_ItemDestroybyChangeItem, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
			Item.wIconIdx, Item.dwDBIdx, Item.Position, 0,
			Item.Durability, pPlayer->GetPlayerExpPoint());	

			// send item use
			MSG_ITEM_USE_ACK msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_USE_ACK;
			msg.dwObjectID = pPlayer->GetID();
			msg.TargetPos = TargetPos;
			msg.wItemIdx = wItemIdx;
			ITEMMGR->SendAckMsg( pPlayer, &msg, sizeof(msg) );

			if( pItemUnit->wToItemIdx == 7999 )		// money
			{
				pPlayer->SetMoney( pItemUnit->wToItemDur, MONEY_ADDITION, MF_OBTAIN );
			}
			else if( pItemUnit->wToItemIdx == 7998 )
			{
//				// event 050106
//				MSGBASE msg;
//				msg.Category = MP_USERCONN;
//				msg.Protocol = MP_USERCONN_EVENTITEM_USE;
//				pPlayer->SendMsg( &msg, sizeof(msg) );
				// event 050203
				MSG_NAME msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_EVENTITEM_USE;
				strcpy( msg.Name, pPlayer->GetObjectName() );
				pPlayer->SendMsg( &msg, sizeof(msg) );
			}
			else
				ITEMMGR->ObtainItemFromChangeItem( pPlayer, pItemUnit->wToItemIdx, pItemUnit->wToItemDur );
		}
		else
			return 0;		
	}
	else
		return 0;

	return 1;
}
*/


/*
// ChangeItemMgr.cpp: implementation of the CChangeItemMgr class.
CChangeItemMgr::CChangeItemMgr()
{

}

CChangeItemMgr::~CChangeItemMgr()
{
	Release();
}

void CChangeItemMgr::Init()
{
	m_ChangeItemList.Initialize( 300 );
	m_MultiChangeItemList.Initialize( 100 );
	LoadItemChange();
}

void CChangeItemMgr::Release()
{
	sCHANGEITEM* pData = NULL;
	m_ChangeItemList.SetPositionHead();
	while( pData = m_ChangeItemList.GetData() )
	{
		delete[] pData->pItemUnit;
		delete pData;
	}
	m_ChangeItemList.RemoveAll();	

	sMULTICHANGEITEM* pData1 = NULL;
	m_MultiChangeItemList.SetPositionHead();
	while( pData1 = m_MultiChangeItemList.GetData() )
	{
		delete[] pData1->pSingleItemUnit;
		delete[] pData1->pMultiItemUnit;
		delete pData1;
	}
	m_MultiChangeItemList.RemoveAll();	
}

BOOL CChangeItemMgr::LoadItemChange()
{
	CMHFile file;
	if( !file.Init( "./System/Resource/ItemChangeList.bin", "rb" ) )
		return FALSE;
	while( 1 )
	{
		if( file.IsEOF() )
			break;
		sCHANGEITEM* pData = new sCHANGEITEM;
		pData->wItemIdx = file.GetWord();
		pData->wMaxToItem = file.GetWord();
		pData->pItemUnit = new sCHANGEITEMUNIT[pData->wMaxToItem];
		for( WORD i = 0; i < pData->wMaxToItem; ++i )
		{
			pData->pItemUnit[i].wToItemIdx = file.GetWord();
			pData->pItemUnit[i].wToItemDur = file.GetWord();
			pData->pItemUnit[i].wPercent = file.GetWord();
		}

		m_ChangeItemList.Add( pData, pData->wItemIdx );
	}
	file.Release();

	if( !file.Init( "./System/Resource/ItemChangeListMulti.bin", "rb" ) )
		return FALSE;
	while( 1 )
	{
		if( file.IsEOF() )
			break;
		sMULTICHANGEITEM* pData = new sMULTICHANGEITEM;
		pData->wItemIdx = file.GetWord();
		pData->wLimitLevel = file.GetWord();
		pData->wMaxSingle = file.GetWord();
		pData->wMaxMulti = file.GetWord();
		pData->pSingleItemUnit = new sCHANGEITEMUNIT[pData->wMaxSingle];
		pData->pMultiItemUnit = new sCHANGEITEMUNIT[pData->wMaxMulti];
		WORD s = 0;
		WORD m = 0;
		for( WORD i = 0; i < pData->wMaxSingle+pData->wMaxMulti; ++i )
		{
			sCHANGEITEMUNIT unit;
			unit.wToItemIdx = file.GetWord();
			unit.wToItemDur = file.GetWord();
			unit.wPercent = file.GetWord();
			if( unit.wPercent == 0 )
			{
				pData->pSingleItemUnit[s] = unit;
				++s;
			}
			else
			{
				pData->pMultiItemUnit[m] = unit;
				++m;
			}
		}

		m_MultiChangeItemList.Add( pData, pData->wItemIdx );
	}
	file.Release();

	return TRUE;	
}

int CChangeItemMgr::UseChangeItem( CPlayer* pPlayer, WORD TargetPos, WORD wItemIdx )
{
	if( pPlayer->GetState() == eObjectState_Die )
		return 0;
	if( !CHKRT->ItemOf( pPlayer, TargetPos, wItemIdx, 0, 0, CB_EXIST|CB_ICONIDX) )
		return 0;
	if( pPlayer->GetState() == eObjectState_Immortal )
		OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer, eObjectState_Immortal );

	CItemSlot* pSlot = pPlayer->GetSlot( TargetPos );
	const ITEMBASE* pItemBase = pSlot->GetItemInfoAbs( TargetPos );
	ITEMBASE Item = *pItemBase;
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( wItemIdx );
	
	if( pItemInfo )
	if( pItemInfo->ItemKind == eCHANGE_ITEM )
	{
		if( m_MultiChangeItemList.GetData( wItemIdx ) )
		{
			return UseMultiChangeItem( pPlayer, TargetPos, wItemIdx, &Item );
		}
		else
		{
			return UseNormalChangeItem( pPlayer, TargetPos, wItemIdx, &Item );
		}
	}
	else
		return 0;

	return 1;
}

int CChangeItemMgr::UseNormalChangeItem( CPlayer* pPlayer, WORD TargetPos, WORD wItemIdx, ITEMBASE* pItem )
{
	sCHANGEITEMUNIT* pItemUnit = GetItemUnitFromCalPercent( wItemIdx );
	if( !pItemUnit )	return 0;

	if( EI_TRUE != ITEMMGR->DiscardItem( pPlayer, TargetPos, wItemIdx, 1 ) )
		return 0;
	LogItemMoney( pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
				  eLog_ItemDestroybyChangeItem, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
				  pItem->wIconIdx, pItem->dwDBIdx, pItem->Position, 0, pItem->Durability, pPlayer->GetPlayerExpPoint() );	

	// send item use
	MSG_ITEM_USE_ACK msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_USE_ACK;
	msg.dwObjectID = pPlayer->GetID();
	msg.TargetPos = TargetPos;
	msg.wItemIdx = wItemIdx;
	ITEMMGR->SendAckMsg( pPlayer, &msg, sizeof(msg) );

	if( pItemUnit->wToItemIdx == 7999 )		// money
	{
		pPlayer->SetMoney( pItemUnit->wToItemDur, MONEY_ADDITION, MF_OBTAIN );
	}
	else if( pItemUnit->wToItemIdx == 7998 )
	{
		// event 050203
		MSG_NAME msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_EVENTITEM_USE;
		strcpy( msg.Name, pPlayer->GetObjectName() );
		pPlayer->SendMsg( &msg, sizeof(msg) );
	}
	else
		ITEMMGR->ObtainItemFromChangeItem( pPlayer, pItemUnit->wToItemIdx, pItemUnit->wToItemDur );

	return 1;
}

int CChangeItemMgr::UseMultiChangeItem( CPlayer* pPlayer, WORD TargetPos, WORD wItemIdx, ITEMBASE* pItem )
{
	sMULTICHANGEITEM* pMultiChangeItem = m_MultiChangeItemList.GetData( wItemIdx );

	if( pPlayer->GetMaxLevel() < pMultiChangeItem->wLimitLevel )	return 0;		
	if( EI_TRUE != ITEMMGR->DiscardItem( pPlayer, TargetPos, wItemIdx, 1 ) )	return 0;
	// log
	LogItemMoney( pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
				  eLog_ItemDestroybyChangeItem, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
				  pItem->wIconIdx, pItem->dwDBIdx, pItem->Position, 0, pItem->Durability, pPlayer->GetPlayerExpPoint() );	

	// send item use
	MSG_ITEM_USE_ACK msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_USE_ACK;
	msg.dwObjectID = pPlayer->GetID();
	msg.TargetPos = TargetPos;
	msg.wItemIdx = wItemIdx;
	ITEMMGR->SendAckMsg( pPlayer, &msg, sizeof(msg) );

	for( WORD i = 0; i < pMultiChangeItem->wMaxSingle; ++i )
	{
		if( pMultiChangeItem->pSingleItemUnit[i].wToItemIdx == 7999 )
		{
			pPlayer->SetMoney( pMultiChangeItem->pSingleItemUnit[i].wToItemDur, MONEY_ADDITION, MF_OBTAIN );
		}
		else
		{
			ITEMMGR->ObtainItemFromChangeItem( pPlayer, pMultiChangeItem->pSingleItemUnit[i].wToItemIdx, pMultiChangeItem->pSingleItemUnit[i].wToItemDur );
		}		
	}

	sCHANGEITEMUNIT* pItemUnit = GetMultiItemUnitFromCalPercent( pMultiChangeItem );
	if( !pItemUnit )	return 0;

	ITEMMGR->ObtainItemFromChangeItem( pPlayer, pItemUnit->wToItemIdx, pItemUnit->wToItemDur );

	return 1;
}

sCHANGEITEMUNIT* CChangeItemMgr::GetItemUnitFromCalPercent( WORD wItemIdx )
{
	sCHANGEITEM* pData = m_ChangeItemList.GetData( wItemIdx );

	WORD RandRate = rand()%10001;	
	WORD FromPercent = 0;
	WORD ToPercent = 0;

	for( WORD i = 0; i < pData->wMaxToItem; ++i )
	{
		FromPercent = ToPercent;
		ToPercent += pData->pItemUnit[i].wPercent;
		if( FromPercent <= RandRate && RandRate < ToPercent )
		{
			return &pData->pItemUnit[i];
		}
	}

	return NULL;
}

sCHANGEITEMUNIT* CChangeItemMgr::GetMultiItemUnitFromCalPercent( sMULTICHANGEITEM* pMultiChangeItem )
{
	WORD RandRate = rand()%10001;	
	WORD FromPercent = 0;
	WORD ToPercent = 0;

	for( WORD i = 0; i < pMultiChangeItem->wMaxMulti; ++i )
	{
		FromPercent = ToPercent;
		ToPercent += pMultiChangeItem->pMultiItemUnit[i].wPercent;
		if( FromPercent <= RandRate && RandRate < ToPercent )
		{
			return &pMultiChangeItem->pMultiItemUnit[i];
		}
	}

	return NULL;
}

int CChangeItemMgr::UseChangeItem( CPlayer* pPlayer, WORD TargetPos, WORD wItemIdx )
{
	if( pPlayer->GetState() == eObjectState_Die )
		return 0;
	if( !CHKRT->ItemOf( pPlayer, TargetPos, wItemIdx, 0, 0, CB_EXIST|CB_ICONIDX) )
		return 0;
	if( pPlayer->GetState() == eObjectState_Immortal )
		OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer, eObjectState_Immortal );

	CItemSlot* pSlot = pPlayer->GetSlot( TargetPos );
	const ITEMBASE* pItemBase = pSlot->GetItemInfoAbs( TargetPos );
	ITEMBASE Item = *pItemBase;
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( wItemIdx );
	
	if( pItemInfo )
	if( pItemInfo->ItemKind == eCHANGE_ITEM )
	{
		sCHANGEITEMUNIT* pItemUnit = GetItemUnitFromCalPercent( wItemIdx );
		if( pItemUnit )
		{
			if( EI_TRUE != ITEMMGR->DiscardItem( pPlayer, TargetPos, wItemIdx, 1 ) )
				return 0;

			LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
			eLog_ItemDestroybyChangeItem, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
			Item.wIconIdx, Item.dwDBIdx, Item.Position, 0,
			Item.Durability, pPlayer->GetPlayerExpPoint());	

			// send item use
			MSG_ITEM_USE_ACK msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_USE_ACK;
			msg.dwObjectID = pPlayer->GetID();
			msg.TargetPos = TargetPos;
			msg.wItemIdx = wItemIdx;
			ITEMMGR->SendAckMsg( pPlayer, &msg, sizeof(msg) );

			if( pItemUnit->wToItemIdx == 7999 )		// money
			{
				pPlayer->SetMoney( pItemUnit->wToItemDur, MONEY_ADDITION, MF_OBTAIN );
			}
			else if( pItemUnit->wToItemIdx == 7998 )
			{
//				// event 050106
//				MSGBASE msg;
//				msg.Category = MP_USERCONN;
//				msg.Protocol = MP_USERCONN_EVENTITEM_USE;
//				pPlayer->SendMsg( &msg, sizeof(msg) );
				// event 050203
				MSG_NAME msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_EVENTITEM_USE;
				strcpy( msg.Name, pPlayer->GetObjectName() );
				pPlayer->SendMsg( &msg, sizeof(msg) );
			}
			else
				ITEMMGR->ObtainItemFromChangeItem( pPlayer, pItemUnit->wToItemIdx, pItemUnit->wToItemDur );
		}
		else
			return 0;		
	}
	else
		return 0;

	return 1;
}
*/

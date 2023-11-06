// PrivateWarehouseDialog.cpp: implementation of the CPrivateWarehouseDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PrivateWarehouseDialog.h"
#include "ItemManager.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "GameIn.h"
#include "cDivideBox.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "./Input/UserInput.h"
#include "StorageDialog.h"
#include "InventoryExDialog.h"
#include "ChatManager.h"
#include "petweareddialog.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrivateWarehouseDialog::CPrivateWarehouseDialog()
{
	m_type		= WT_ICONGRIDDIALOG;
	m_nIconType = WT_ITEM;	//이 그리드다이얼로그에서 다룰 아이템 종류
}

CPrivateWarehouseDialog::~CPrivateWarehouseDialog()
{
	
}


void CPrivateWarehouseDialog::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID)
{
	cDialog::Init(x,y,wid,hei,basicImage,ID);
	m_type = WT_ICONGRIDDIALOG;

}

BOOL CPrivateWarehouseDialog::AddItem(ITEMBASE* pItemInfo)
{
	if( pItemInfo->dwDBIdx == 0 )
	{
		DEBUGMSG( 0, "Item DB idx == 0" );
		return FALSE;
	}

	CItem * newItem = ITEMMGR->MakeNewItem( pItemInfo,"CPrivateWarehouseDialog::AddItem" );
	if(newItem == NULL)
		return FALSE;
	
	return AddItem( newItem );
}


BOOL CPrivateWarehouseDialog::AddItem(CItem* pItem)
{
	ASSERT(pItem);
	//색 변환
	ITEMMGR->RefreshItem( pItem );

	POSTYPE relPos = GetRelativePosition(pItem->GetPosition());
	return AddIcon(relPos, pItem);
}

BOOL CPrivateWarehouseDialog::DeleteItem(POSTYPE Pos,CItem** ppItem)
{
	BYTE num = GAMEIN->GetStorageDialog()->GetSelectedStorage();
	WORD A = TP_STORAGE_START;
	WORD B = TABCELL_STORAGE_NUM*(num);
	if(!IsAddable(Pos - A-B))
		return DeleteIcon(Pos-TP_STORAGE_START-TABCELL_STORAGE_NUM*(num), (cIcon **)ppItem);
	else
		return FALSE;
}

BOOL CPrivateWarehouseDialog::FakeMoveIcon(LONG x, LONG y, cIcon * icon)
{	
	ASSERT(icon);
	
	if(icon->GetType() == WT_ITEM)
	{
		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( ((CItem*)icon)->GetItemIdx() );
		if( !pItemInfo )
			return FALSE;
/*
		if( pItemInfo->Shop )
		{
			const ITEMBASE* pItemBase = NULL ;
			pItemBase = ((CItem*)icon)->GetItemBaseInfo() ;

			if( !pItemBase ) return FALSE ;

			if( pItemBase->nSealed == eITEM_TYPE_UNSEAL )
			{
				return FALSE;
			}
		}
*/		FakeMoveItem(x, y, (CItem *)icon);
	}
	
	return FALSE;
}

void CPrivateWarehouseDialog::FakeMoveItem( LONG mouseX, LONG mouseY, CItem * pFromItem )
{
	WORD ToPos=0;

	const ITEMBASE& pInfo = pFromItem->GetItemBaseInfo();
	if( !GetPositionForXYRef(mouseX, mouseY, ToPos ) )	
		return;

	BYTE num = GAMEIN->GetStorageDialog()->GetSelectedStorage();
	ToPos = ToPos+TP_STORAGE_START + TABCELL_STORAGE_NUM*(num); // 절대위치 넘어 옴

	CItem * pToItem = GetItemForPos( ToPos );
	
	if( pFromItem->GetParent() == ( cObject* )GAMEIN->GetPetWearedDlg() )
	{
		if( pToItem )
		{
			GAMEIN->GetPetWearedDlg()->FakeMoveIcon( 0, 0, pToItem );
		}
		else
		{
			MSG_PET_ITEM_MOVE_SYN msg;
			msg.Category = MP_PET;
			msg.Protocol = MP_PET_ITEM_MOVE_PLAYER_SYN;
			msg.dwObjectID = HEROID;
			msg.PlayerPos = ToPos;
			msg.PlayerItemIdx = 0;
			msg.PetPos = pFromItem->GetPosition();
			msg.PetItemIdx = pFromItem->GetItemIdx();

			NETWORK->Send( &msg, sizeof( msg ) );
		}
	
		return;
	}
	//---KES Item Combine Fix 071021
	if( ITEMMGR->IsDupItem( pFromItem->GetItemIdx() ) )
	{
		//---나누는 것은 허용하지 말자. 이전에 복사가 일어난 적이 있었다.
		/*
		if( KEYBOARD->GetKeyPressed(KEY_MENU) && !( pToItem ) )
		{
			FakeItemDivide( ToPos, pFromItem, pToItem );
			return;
		}
		else*/
		if( ( pToItem && pFromItem->GetItemIdx() == pToItem->GetItemIdx() ) )
		{
			FakeItemCombine( ToPos, pFromItem, pToItem );
			return;
		}
	}
	//-----------------------------------------------

	if(pToItem)
	if(pToItem->IsLocked() == TRUE)
		return;
	FakeGeneralItemMove( ToPos, pFromItem, pToItem );
}

POSTYPE CPrivateWarehouseDialog::GetRelativePosition( POSTYPE absPos)
{
	return ( absPos - TP_STORAGE_START ) %	TABCELL_STORAGE_NUM;
}
 
CItem * CPrivateWarehouseDialog::GetItemForPos(POSTYPE absPos)
{
	if(TP_STORAGE_START <= absPos && absPos < TP_STORAGE_END)
	{
		int num  = GAMEIN->GetStorageDialog()->GetStorageNum(absPos);
		return (CItem *)GetIconForIdx(absPos-TP_STORAGE_START-TABCELL_STORAGE_NUM*num);
	}

	return NULL;
}

void CPrivateWarehouseDialog::FakeGeneralItemMove( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem)
{
	if(CanBeMoved(pFromItem, ToPos) == FALSE)
		return;
	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos( pFromItem->GetPosition() );
	if(FromIdx >= eItemTable_Max) return;

	BYTE ToIdx = ITEMMGR->GetTableIdxForAbsPos( ToPos );
	if (ToIdx >= eItemTable_Max) return;

	if(pFromItem->GetPosition() == ToPos)
		return;
	if(pToItem && CanBeMoved( pToItem, pFromItem->GetPosition() ) == FALSE )
	{
		return;
	}	

	// 071204 LUJ	출발지 혹은 목적지가 인벤토리 무기 슬롯이고 양손 무기가 장착하려 시도할 때,
	//				방패가 있을 때는 이동을 막아야 한다
	{
		CItem*	shield	= ITEMMGR->GetItemofTable( eItemTable_Inventory, TP_WEAR_START + eEquipSlot_Shield - 1 );

		struct
		{
			BOOL operator() ( DWORD itemIndex ) const
			{
				const ITEM_INFO* info = ITEMMGR->GetItemInfo( itemIndex );

				return info	&& (
					info->WeaponAnimation == eWeaponAnimationType_TwoHand	||
					info->WeaponAnimation == eWeaponAnimationType_Staff		||
					info->WeaponAnimation == eWeaponAnimationType_Bow		||
					info->WeaponAnimation == eWeaponAnimationType_Gun		||
					info->WeaponAnimation == eWeaponAnimationType_TwoBlade );
			}
		}
		IsTwoHand;		
		
		if( (	IsTwoHand( pToItem ? pToItem->GetItemIdx() : 0 )	||
				IsTwoHand( pFromItem->GetItemIdx() ) )						&&
			(	TP_WEAR_START + eEquipSlot_hand - 1	== ToPos				||
				TP_WEAR_START + eEquipSlot_hand - 1 == pFromItem->GetPosition() )	&&
			( shield && shield->GetDBIdx() ) )
		{
			return;
		}
	}

	MSG_ITEM_MOVE_SYN msg;
	msg.Category			= MP_ITEM;
	msg.Protocol			= MP_ITEM_MOVE_SYN;
	msg.dwObjectID			= HEROID;

	msg.FromPos				= pFromItem->GetPosition();
	msg.wFromItemIdx		= pFromItem->GetItemIdx();
	msg.ToPos				= ToPos;
	msg.wToItemIdx			= (pToItem?pToItem->GetItemIdx():0);

	NETWORK->Send( &msg, sizeof(msg) );
}

void CPrivateWarehouseDialog::FakeItemDivide( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem )
{
	if( !CanBeMoved( pFromItem, ToPos ) )
		return;
	if( pFromItem->GetPosition() == ToPos )
		return;

	GAMEIN->GetStorageDialog()->SetDividMsg( pFromItem, ToPos );
	// Locking
//	pFromItem->SetMovable( FALSE );
	
	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos( pFromItem->GetPosition() );
	if(FromIdx >= eItemTable_Max) return;

	GAMEIN->GetInventoryDialog()->SetDisable(TRUE);

	CStorageDialog* pStorageDlg = GAMEIN->GetStorageDialog();
	pStorageDlg->SetDisable(TRUE);

	cDivideBox * pDivideBox = WINDOWMGR->DivideBox( DBOX_DIVIDE_INV, (LONG)pFromItem->GetAbsX(), (LONG)pFromItem->GetAbsY(), OnFakeStorageItemDivideOk, OnFakeStorageItemDivideCancel, pStorageDlg, pFromItem, CHATMGR->GetChatMsg(185) );
	pDivideBox->SetValue( 1 );
	pDivideBox->SetMaxValue( MAX_YOUNGYAKITEM_DUPNUM );	
}

void CPrivateWarehouseDialog::OnFakeStorageItemDivideCancel( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	cDialog * ToDlg = ( cDialog * )vData1;
	ToDlg->SetDisable(FALSE);
	GAMEIN->GetInventoryDialog()->SetDisable(FALSE);
}

void CPrivateWarehouseDialog::OnFakeStorageItemDivideOk( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	CStorageDialog * ptDlg = ( CStorageDialog * )vData1;
	cDialog * pfDlg = (cDialog *)vData2;
	if( param1 == 0 ) 
	{
		ptDlg->SetDisable(FALSE);
		GAMEIN->GetInventoryDialog()->SetDisable(FALSE);
		return;
	}

	if( ptDlg->m_divideMsg.FromDur > param1 )
	{
		ptDlg->m_divideMsg.ToDur			= param1;
		ptDlg->m_divideMsg.FromDur		= ptDlg->m_divideMsg.FromDur - param1;
		NETWORK->Send( &ptDlg->m_divideMsg, sizeof(ptDlg->m_divideMsg) );
	}
	else
	{
		//아이템 이동
		ptDlg->FakeGeneralItemMove(ptDlg->m_divideMsg.ToPos, (CItem *)vData2, NULL);
		ptDlg->SetDisable(FALSE);
		GAMEIN->GetInventoryDialog()->SetDisable(FALSE);
	}
}

void CPrivateWarehouseDialog::FakeItemCombine( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem )
{
	if( pFromItem->GetPosition() == pToItem->GetPosition() )
		return;
	if( pFromItem->GetItemIdx() != pToItem->GetItemIdx() )
		return;

	if( CanBeMoved( pFromItem, ToPos ) == FALSE )
		return;

	if( pToItem && CanBeMoved( pToItem, pFromItem->GetPosition() ) == FALSE )
		return;

	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos( pFromItem->GetPosition() );
	if(FromIdx >= eItemTable_Max) return;

	BYTE ToIdx = ITEMMGR->GetTableIdxForAbsPos( ToPos );
	if(ToIdx >= eItemTable_Max) return;

	MSG_ITEM_COMBINE_SYN msg;
	msg.Category			= MP_ITEM;
	msg.Protocol			= MP_ITEM_COMBINE_SYN;
	msg.dwObjectID			= HEROID;
	msg.FromPos				= pFromItem->GetPosition();
	msg.ToPos				= pToItem->GetPosition();
	msg.wItemIdx			= pFromItem->GetItemIdx();
	msg.FromDur				= pFromItem->GetDurability();
	msg.ToDur				= pToItem->GetDurability();

	NETWORK->Send( &msg, sizeof(msg) );
}

BOOL CPrivateWarehouseDialog::CanBeMoved(CItem* pItem,POSTYPE pos)
{
	ASSERT(pItem);
	if(pItem)
	{
		POSTYPE fromPos = pItem->GetPosition();
		POSTYPE toPos = pos;

		// 080410 LUJ, 인벤->창고 이동을 막는다
		if( eItemTable_Weared == ITEMMGR->GetTableIdxForAbsPos( fromPos ) )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 76 ) );
			return FALSE;
		}

	}
	if(TP_STORAGE_START <= pos && pos < TP_STORAGE_END)
	{
		// 무조건 TRUE
		return TRUE;
	}
/*	else if(TP_MUNPAWAREHOUSE_START <= pos && pos < TP_MUNPAWAREHOUSE_END)
	{
		// 무조건 TRUE
		return TRUE;
	}*/
	else if(TP_INVENTORY_START <= pos && pos < (TP_INVENTORY_END + ( TABCELL_INVENTORY_NUM * HERO->Get_HeroExtendedInvenCount() )))
	{
		// 무조건 TRUE
		return TRUE;
	}
	else if(TP_WEAR_START <= pos && pos < TP_WEAR_END)
	{
		// 장착하는 아이탬이 아니면 FALSE
/*		if( !( pItem->GetItemKind() & eEQUIP_ITEM ) )
			return FALSE;

		// 들어가야 할자리가 아니면 FALSE
		ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
		if(!pInfo) return FALSE;
		if(pInfo->EquipKind == eWearedItem_Earring1)
		{	
			if(pos-TP_WEAR_START != eWearedItem_Earring1 && pos-TP_WEAR_START != eWearedItem_Earring2)
				return FALSE;
		}
		else
		{
			if(pos-TP_WEAR_START != pInfo->EquipKind)
				return FALSE;
		}
*/		

		BOOL bResult = ITEMMGR->CanEquip(pItem->GetItemIdx()) ;

		if( bResult == FALSE )
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(825), pItem->GetItemInfo()->ItemName) ;	// "레벨이 낮아 착용 할 수 없습니다."
		}

		return bResult;
	}
	else
		return FALSE;
}
	

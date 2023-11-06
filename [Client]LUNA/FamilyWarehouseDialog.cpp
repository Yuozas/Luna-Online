// FamilyWarehouseDialog.cpp: implementation of the CFamilyWarehouseDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FamilyWarehouseDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cIconGridDialog.h"
#include "./Interface/cPushupButton.h"
#include "./Interface/cStatic.h"
#include "ItemManager.h"
#include "ChatManager.h"
//#include "FamilyManager.h"
#include "./Input/UserInput.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "GameIn.h"
#include "NpcScriptDialog.h"
#include "./interface/cWindowManager.h"
#include "cDivideBox.h"
#include "InventoryExDialog.h"
#include "./MHMap.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFamilyWarehouseDialog::CFamilyWarehouseDialog()
{
	m_type = WT_FAMILYWAREHOUSEDLG;
	m_Lock = FALSE;
	m_InitInfo = FALSE;
}

CFamilyWarehouseDialog::~CFamilyWarehouseDialog()
{

}

void CFamilyWarehouseDialog::Linking()
{
	m_pFamilyMoney = (cStatic *)GetWindowForID(FDW_MONEY);
}

void CFamilyWarehouseDialog::SetActive(BOOL val)
{
	if(val == TRUE)
	{
		if(GetLock() == TRUE)
			return;
		SetLock(TRUE);
		//FAMILYMGR->FamilyWarehouseInfoSyn(GetCurTabNum());
		if(GAMEIN->GetInventoryDialog()->IsActive() == FALSE)
			GAMEIN->GetInventoryDialog()->SetActive(TRUE);
			
	}
	else
	{
		if(HERO == 0)
			return;
		if( ( HERO->GetState() == eObjectState_Deal ) && (GAMEIN->GetNpcScriptDialog()->IsActive() == FALSE))
		{
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
		}
		//FAMILYMGR->FamilyWarehouseLeave(GetCurTabNum());
		cDivideBox * pDivideBox = (cDivideBox *)WINDOWMGR->GetWindowForID( DBOX_INPUT_MONEY );
		if( pDivideBox )
		{
			pDivideBox->SetActive(FALSE);
			WINDOWMGR->AddListDestroyWindow( pDivideBox );
		}

	}
	cTabDialog::SetActive(val);
}

DWORD CFamilyWarehouseDialog::ActionEvent(CMouse * mouseInfo)
{
	// 아이템을 받는 중이면 기다림. 
	// 아이템 다 받으면 락 풀기 
	// 다른 버튼이 선택되면 현재 탭 MSG_BYTE로 MP_ITEM_FAMILY_WAREHOUSE_LEAVE 보냄 
	// 버튼 눌려지면 아이템 보내달라고 요청. 받기 전에 다른 탭 못 누르게 락 
	//FAMILYMGR->FamilyWarehouseInfoSyn();
	if(GetLock())
		return NULL;
	return cTabDialog::ActionEvent( mouseInfo );
}

void CFamilyWarehouseDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if(!m_bActive) return;
	if(GetLock())
		return;
	if(we & WE_PUSHDOWN)
	{	
		//FAMILYMGR->FamilyWarehouseLeave(!(lId - GDW_WAREHOUSE1BTN));
		//FAMILYMGR->FamilyWarehouseInfoSyn((BYTE)(lId - GDW_WAREHOUSE1BTN));
		SetLock(TRUE);
	}
}

void CFamilyWarehouseDialog::Render()
{
	cDialog::RenderWindow();
	cDialog::RenderComponent();
	cTabDialog::RenderTabComponent();
}

void CFamilyWarehouseDialog::Add(cWindow * window)
{
	if(window->GetType() == WT_PUSHUPBUTTON)
		AddTabBtn(curIdx1++, (cPushupButton * )window);
	else if(window->GetType() == WT_ICONGRIDDIALOG)
	{
		AddTabSheet(curIdx2++, window);
		((cIconGridDialog*)window)->SetDragOverIconType( WT_ITEM );
	}
	else 
		cDialog::Add(window);
}

BOOL CFamilyWarehouseDialog::FakeMoveIcon(LONG x, LONG y, cIcon * icon)
{
	ASSERT(icon);
	if( m_bDisable )
		return FALSE;

	if(icon->GetType() == WT_ITEM)
	{
		return FakeMoveItem(x, y, (CItem *)icon);
	}

	return FALSE;
}

BOOL CFamilyWarehouseDialog::FakeMoveItem( LONG mouseX, LONG mouseY, CItem * pFromItem )
{
	// changeitem 중에 이동안되는거
/*	if( pFromItem->GetItemKind() == eCHANGE_ITEM_LOCK )
	{
		return FALSE;
	}
*/
	WORD ToPos=0;
	
	if( !GetPositionForXYRef( 0, mouseX, mouseY, ToPos ) )	// 절대위치 넘어 옴
		return FALSE;
	if( pFromItem->IsLocked() ) 
		return FALSE;
	CItem * pToItem = GetItemForPos( ToPos );
	if( pToItem )
	{
		// 자동 교환 안됨
		return FALSE;
	}

	const ITEMBASE& itemBase = pFromItem->GetItemBaseInfo();

	if(TP_FAMILYWAREHOUSE_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_FAMILYWAREHOUSE_END)
		return FakeGeneralItemMove( ToPos, pFromItem, pToItem );
	else if(TP_INVENTORY_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < (TP_INVENTORY_END + ( TABCELL_INVENTORY_NUM * HERO->Get_HeroExtendedInvenCount() ))) 
		return FakeGeneralItemMove( ToPos, pFromItem, pToItem );
	else if(TP_WEAR_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_WEAR_END)
		return FakeGeneralItemMove( ToPos, pFromItem, pToItem );

	return FALSE;
}

BOOL CFamilyWarehouseDialog::FakeGeneralItemMove( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem )
{
	BYTE FromTable = ITEMMGR->GetTableIdxForAbsPos( pFromItem->GetPosition() );
	if(FromTable >= eItemTable_Max) return FALSE;
	if( pToItem )
	{
		return FALSE;
	}
	if(FromTable == eItemTable_Storage)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(47) );
		return FALSE;
	}

	BYTE ToTable = ITEMMGR->GetTableIdxForAbsPos( ToPos );
	if(ToTable >= eItemTable_Max) return FALSE;


	MSG_ITEM_MOVE_SYN msg;
	msg.Category			= MP_ITEM;
	msg.Protocol			= MP_ITEM_FAMILY_MOVE_SYN;
	msg.dwObjectID			= HEROID;

	msg.FromPos				= pFromItem->GetPosition();
	msg.wFromItemIdx		= pFromItem->GetItemIdx();
	msg.ToPos				= ToPos;
	msg.wToItemIdx			= (pToItem?pToItem->GetItemIdx():0);

	if( msg.FromPos == msg.ToPos )
		return FALSE;

	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Storage);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_FamilyWarehouse);

	NETWORK->Send( &msg, sizeof(msg) );
	return TRUE;
}

BOOL CFamilyWarehouseDialog::CanBeMoved(CItem* pItem,POSTYPE toPos)
{
	if(pItem == 0)
		return FALSE;
	
	POSTYPE fromPos = pItem->GetPosition();
	
	if(TP_FAMILYWAREHOUSE_START<= toPos && toPos < TP_FAMILYWAREHOUSE_END)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CFamilyWarehouseDialog::GetPositionForXYRef(BYTE param, LONG x, LONG y, WORD& pos)
{
	WORD position=0;										// 상대 위치

	cIconGridDialog * gridDlg = (cIconGridDialog *)GetTabSheet(GetCurTabNum());
	if(gridDlg->GetPositionForXYRef(x, y, position))
	{
		pos = position+TP_FAMILYWAREHOUSE_START + TABCELL_FAMILYWAREHOUSE_NUM * GetCurTabNum();		// -> 절대 위치로 바꿈
		return TRUE;
	}
	return FALSE;
}

CItem * CFamilyWarehouseDialog::GetItemForPos(POSTYPE absPos)
{
	if(TP_FAMILYWAREHOUSE_START <= absPos && absPos < TP_FAMILYWAREHOUSE_END)
	{
		int TabIndex = GetCurTabNum();
		cIconGridDialog * Dlg = (cIconGridDialog *)GetTabSheet(TabIndex);
		if(absPos < (TP_FAMILYWAREHOUSE_START+TABCELL_FAMILYWAREHOUSE_NUM*TabIndex))
			return NULL;
		return (CItem *)Dlg->GetIconForIdx(absPos-TP_FAMILYWAREHOUSE_START-TABCELL_FAMILYWAREHOUSE_NUM*TabIndex);
	}
	return NULL;
}


BOOL CFamilyWarehouseDialog::AddItem(ITEMBASE* pItemInfo)
{
	if(pItemInfo->dwDBIdx == 0)
	{
		ASSERTMSG(0,"길드창고-아이템 디비인덱스가 0. Invalid FamilyWarehouse Index.");
		return FALSE;
	}
	CItem* pItem = ITEMMGR->GetItem(pItemInfo->dwDBIdx);
	if(pItem == 0)
		pItem = ITEMMGR->MakeNewItem(pItemInfo,"CFamilyWarehouseDialog::AddItem");
	ASSERT(pItem);

	return AddItem(pItem);
}

BOOL CFamilyWarehouseDialog::AddItem(CItem* pItem)
{
	ITEMMGR->RefreshItem(pItem);
	POSTYPE relPos = GetRelativePosition(pItem->GetPosition());
	int tabIndex = GetTabIndex( pItem->GetPosition() );
	cIconGridDialog * dlg = (cIconGridDialog *)GetTabSheet(tabIndex);
	return dlg->AddIcon(relPos, pItem);
}

BOOL CFamilyWarehouseDialog::DeleteItem(POSTYPE Pos,CItem** ppItem)
{
	int TabIndex = GetTabIndex( Pos );
	cIconGridDialog * Dlg = (cIconGridDialog *)GetTabSheet(TabIndex);
	if(!Dlg->IsAddable(Pos - TP_FAMILYWAREHOUSE_START-TABCELL_FAMILYWAREHOUSE_NUM*TabIndex))
		return Dlg->DeleteIcon(Pos-TP_FAMILYWAREHOUSE_START-TABCELL_FAMILYWAREHOUSE_NUM*TabIndex, (cIcon **)ppItem);
	else
		return FALSE;

}

POSTYPE CFamilyWarehouseDialog::GetRelativePosition( POSTYPE absPos)
{
	return ( absPos - TP_FAMILYWAREHOUSE_START ) % TABCELL_FAMILYWAREHOUSE_NUM;
}

WORD CFamilyWarehouseDialog::GetTabIndex( POSTYPE absPos )
{
	return ( absPos - TP_FAMILYWAREHOUSE_START ) / TABCELL_FAMILYWAREHOUSE_NUM;
}

void CFamilyWarehouseDialog::SetLock(BOOL val)
{
	m_Lock = val;
}

BOOL CFamilyWarehouseDialog::GetLock()
{
	return m_Lock;
}

void CFamilyWarehouseDialog::SetWarehouseInfo(SEND_MUNPA_WAREHOUSE_ITEM_INFO* pInfo)
{
	cIconGridDialog * dlg = (cIconGridDialog *)GetTabSheet(pInfo->bTabNum);

	CItem * pItem = NULL;
	for(int i=0; i<TABCELL_FAMILYWAREHOUSE_NUM; ++i)
	{
	//	if(DeleteItem(TP_FAMILYWAREHOUSE_START+i+TABCELL_FAMILYWAREHOUSE_NUM*GetCurTabNum(), &FromItem);
		
		if(GAMEIN->GetFamilyWarehouseDlg()->DeleteItem(TP_FAMILYWAREHOUSE_START+i+TABCELL_FAMILYWAREHOUSE_NUM*GetCurTabNum(), &pItem) == TRUE)
			ITEMMGR->ItemDelete(pItem);
		if(pInfo->info.WarehouseItem[i].dwDBIdx != 0)
			AddItem(&pInfo->info.WarehouseItem[i]);
	}
	SetMoney(pInfo->info.money);

	SetLock(FALSE);
}

void CFamilyWarehouseDialog::PutInOutMoney(int vals)
{	
	CInventoryExDialog* pInvenDlg = GAMEIN->GetInventoryDialog();

	cStatic* pFamilyMoney = (cStatic*)WINDOWMGR->GetWindowForIDEx(GDW_MONEY);

	if(vals)
	{
		cDivideBox * pDivideBox = WINDOWMGR->DivideBox( DBOX_INPUT_MONEY, (LONG)(pFamilyMoney->GetAbsX()-50) , (LONG)(pFamilyMoney->GetAbsY()-100), OnPutInMoneyFamilyOk, NULL, pInvenDlg, this, CHATMGR->GetChatMsg(230) );
	}
	else
	{
		cDivideBox * pDivideBox = WINDOWMGR->DivideBox( DBOX_INPUT_MONEY, (LONG)(pFamilyMoney->GetAbsX()-50) , (LONG)(pFamilyMoney->GetAbsY()-100), OnPutOutMoneyFamilyOk, NULL, pInvenDlg, this, CHATMGR->GetChatMsg(231) );
	}
}


void CFamilyWarehouseDialog::OnPutInMoneyFamilyOk( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	//// 070801 웅주, 예전 패밀리 제거하며 기존 소스 주석 처리
	//if(param1 == 0)	return;

	//if(HERO->GetMoney() < param1)
	//	param1 = HERO->GetMoney();

	//if( HERO->GetState() != eObjectState_Deal )
	//	return;
	//if( FAMILYMGR->GetLocation() != MAP->GetMapNum() )
	//	return;
	//if( !GAMEIN->GetFamilyWarehouseDlg()->IsActive() )
	//	return;

	//MSG_DWORD msg;
	//msg.Category = MP_FAMILY;
	//msg.Protocol = MP_FAMILY_PUTIN_MONEY_SYN;
	//msg.dwObjectID = HEROID;
	//msg.dwData = param1;
	//
	//NETWORK->Send(&msg, sizeof(msg));
}

void CFamilyWarehouseDialog::PutInOutMoneyResult(MSG_DWORD3 * msg)
{
	if(HEROID == msg->dwData1)
	{
		HERO->SetMoney(msg->dwData2);
	//	GAMEIN->GetCharacterDialog()->UpdateData();
		
	}
	SetMoney(msg->dwData3);
}

void CFamilyWarehouseDialog::OnPutOutMoneyFamilyOk( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	//// 070801 웅주, 예전 패밀리 제거하며 기존 소스 주석 처리
	//if(param1 == 0)	return;

	//if( HERO->GetState() != eObjectState_Deal )
	//	return;
	//if( FAMILYMGR->GetLocation() != MAP->GetMapNum() )
	//	return;
	//if( !GAMEIN->GetFamilyWarehouseDlg()->IsActive() )
	//	return;

	//MSG_DWORD msg;
	//msg.Category = MP_FAMILY;
	//msg.Protocol = MP_FAMILY_PUTOUT_MONEY_SYN;
	//msg.dwObjectID = HEROID;
	//msg.dwData = param1;
	//
	//NETWORK->Send(&msg, sizeof(msg));
}

void CFamilyWarehouseDialog::SetMoney(MONEYTYPE money)
{
	m_pFamilyMoney->SetStaticText(AddComma(money));
}

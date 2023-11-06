// StorageDialog.cpp: implementation of the CStorageDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StorageDialog.h"
#include "PrivateWarehouseDialog.h"

#include "WindowIDEnum.h"
#include "./Interface/cPushupButton.h"
#include "./Interface/cListCtrl.h"
#include "./Interface/cStatic.h"
#include "cDivideBox.h"
#include "cMsgBox.h"

#include "ObjectManager.h"
#include "ItemManager.h"
#include "ObjectStateManager.h"
#include "./Interface/cWindowManager.h"
#include "MHFile.h"
#include "ChatManager.h"

// 070326 LYW --- StorageDialog : Include header file of inventory.
#include "InventoryExDialog.h"

// 070326 LYW --- StorageDialog : Include GameIn.
#include "GameIn.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStorageDialog::CStorageDialog()
{
	m_type = WT_STORAGEDLG;
	m_CurStorageMode = -1;
	for(int i=0; i<TAB_STORAGE_NUM; ++i)
		m_StorageItemDlg[i] = NULL;
	m_StorageNum = 0;
	memset(&m_StorageListInfo, 0, sizeof(STORAGE_LISTINFO));
	m_bItemInit = FALSE;
}

CStorageDialog::~CStorageDialog()
{
	for(int n=0;n<eStorageMode_Max;++n)
	{
		m_StorageCtlListArray[n].RemoveAll();
	}

	m_pStorageListInfo->DeleteAllItems();
}

void CStorageDialog::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID)
{
	cDialog::Init(x,y,wid,hei,basicImage,ID);
	m_type = WT_STORAGEDLG;
	LoadStorageListInfo();
}

void CStorageDialog::LoadStorageListInfo()
{
	CMHFile file;
	file.Init("System/Resource/StorageListInfo.bin","rb");
	if(file.IsInited() == FALSE)
	{
		ASSERTMSG(0, "Fail To Load a File : StorageListInfo.");
		return;
	}

	int Count = MAX_BASESTORAGELIST_NUM;

	for(int n=0;n<Count;++n)
	{
		m_StorageListInfo.Storage[n].MaxCellNum = 36*(n+1);
		m_StorageListInfo.Storage[n].MaxMoney = file.GetDword();
		m_StorageListInfo.Storage[n].BuyPrice = file.GetDword();
	}

	file.Release();
}

void CStorageDialog::Linking()
{
	m_pMoney = (cStatic*)GetWindowForID(PYO_MONEY); 

	
	//////////////////////////////////////////////////////////////////////////
	//StorageListInfo
	m_pStorageListInfo = (cListCtrl*)GetWindowForID(PYO_STORAGELCTRL);
	m_StorageCtlListArray[eStorageMode_StorageListInfo].AddTail(m_pStorageListInfo);
	m_StorageCtlListArray[eStorageMode_StorageListInfo].AddTail((cButton*)GetWindowForID(PYO_BUYSTORAGEBTN));
	m_StorageCtlListArray[eStorageMode_StorageListInfo].AddTail((cButton*)GetWindowForID(PYO_NOBUYBTN));
	m_StorageCtlListArray[eStorageMode_StorageListInfo].AddTail((cStatic*)GetWindowForID(PYO_STORAGEINFO1));
	m_StorageCtlListArray[eStorageMode_StorageListInfo].AddTail((cStatic*)GetWindowForID(PYO_STORAGEINFO2));
	m_StorageCtlListArray[eStorageMode_StorageListInfo].AddTail((cStatic*)GetWindowForID(PYO_STORAGEINFO3));
	m_pStorageHaveStc = (cStatic*)GetWindowForID(PYO_STORAGEHAVESTC);
	m_StorageCtlListArray[eStorageMode_StorageListInfo].AddTail(m_pStorageHaveStc);
	m_StorageCtlListArray[eStorageMode_StorageListInfo].AddTail((cStatic*)GetWindowForID(PYO_STORAGEHAVE));
	//////////////////////////////////////////////////////////////////////////
	//noWare
	m_StorageCtlListArray[eStorageMode_NoWare].AddTail((cStatic*)GetWindowForID(PYO_NOWARE1));
	m_StorageCtlListArray[eStorageMode_NoWare].AddTail((cStatic*)GetWindowForID(PYO_NOWARE));
	m_StorageCtlListArray[eStorageMode_NoWare].AddTail((cButton*)GetWindowForID(PYO_STORAGELISTBTN));
	m_StorageCtlListArray[eStorageMode_NoWare].AddTail((cButton*)GetWindowForID(PYO_NOBUYSTORAGEBTN));
	
	//////////////////////////////////////////////////////////////////////////
	
	for(int i=0; i<MAX_STORAGELIST_NUM; ++i)
	{
		m_pStorageBtn[i] = (cPushupButton*)GetWindowForID(PYO_STORAGE1BTN+i);

		m_StorageItemDlg[i] = (CPrivateWarehouseDialog*)GetWindowForID(PYO_WAREITEM1+i);
		m_StorageCtlListArray[eStorageMode_StorageWare1+i].AddTail(m_StorageItemDlg[i]);
		m_StorageCtlListArray[eStorageMode_StorageWare1+i].AddTail((cButton*)GetWindowForID(PYO_PUTINMONEYBTN));
		m_StorageCtlListArray[eStorageMode_StorageWare1+i].AddTail((cButton*)GetWindowForID(PYO_PUTOUTMONEYBTN));
		m_StorageCtlListArray[eStorageMode_StorageWare1+i].AddTail((cStatic*)GetWindowForID(PYO_MONEYIMG)); //동전 그림
		m_StorageCtlListArray[eStorageMode_StorageWare1+i].AddTail((cStatic*)GetWindowForID(PYO_MONEYBACKIMG));
		m_StorageCtlListArray[eStorageMode_StorageWare1+i].AddTail(m_pMoney); //바탕 이미지
		m_StorageCtlListArray[eStorageMode_StorageWare1+i].AddTail((cStatic*)GetWindowForID(PYO_WAREIMAGE));
	}
	
	SetStorageListInfo();
}

void CStorageDialog::SetStorageListInfo()
{
	m_pStorageListInfo->DeleteAllItems();
	
	int Count = MAX_BASESTORAGELIST_NUM;

	for(BYTE i=0; i<Count; ++i)
	{
		RITEM * ritem = new RITEM(4);
		sprintf(ritem->pString[0], "%d", i+1);
		sprintf(ritem->pString[1], "%d", m_StorageListInfo.Storage[i].MaxCellNum);

//		sprintf(ritem->pString[2], "%d", m_StorageListInfo.Storage[i].MaxMoney);
//		sprintf(ritem->pString[3], "%d", m_StorageListInfo.Storage[i].BuyPrice);
		sprintf(ritem->pString[2], "%s", AddComma(m_StorageListInfo.Storage[i].MaxMoney));

		sprintf(ritem->pString[3], "%s", AddComma(m_StorageListInfo.Storage[i].BuyPrice));
		
//		if((HERO->GetStorageNum() - i) > 0 )
//		{
//			for(int n =0; n<4; ++n)
//				ritem->rgb[n] = RGBA_MAKE(180, 180, 180, 255);
//		}
		m_pStorageListInfo->InsertItem(i, ritem);
	}
}

void CStorageDialog::SetMode(int mode)
{
	m_CurStorageMode = mode;
}

void CStorageDialog::ShowStorageMode(int mode)
{
	if(m_CurStorageMode != -1)
	{
		SetActiveStorageMode(m_CurStorageMode,FALSE);
	}
	
	m_CurStorageMode = mode;
	SetActiveStorageMode(mode,TRUE);
		
	RefreshDlg();
}

void CStorageDialog::AddStorageMode(int mode)
{
	switch(mode)
	{
	case eStorageMode_StorageListInfo:
		{
			m_pStorageHaveStc->SetStaticValue(HERO->GetStorageNum());
		}
		break;
	case eStorageMode_StorageWare1: case eStorageMode_StorageWare2: case eStorageMode_StorageWare3:
	case eStorageMode_StorageWare4: case eStorageMode_StorageWare5:
		{
			SelectedBtnPushDown(mode - eStorageMode_StorageWare1);
			SetSelectedStorage(mode-eStorageMode_StorageWare1);
		}
		break;
	}
}

void CStorageDialog::SetActiveStorageMode(int mode,BOOL bActive)
{
	PTRLISTSEARCHSTART(m_StorageCtlListArray[mode],cWindow*,pWin)
		// 070325 LYW --- StorageDialog : Check window.
		//pWin->SetActive(bActive);
		if(pWin)
		{
			pWin->SetActive(bActive);
		}
	PTRLISTSEARCHEND
}

void CStorageDialog::RefreshDlg()
{
	if(m_CurStorageMode != eStorageMode_StorageListInfo)
	{
		for(int i=0; i<HERO->GetStorageNum(); ++i)
			m_pStorageBtn[i]->SetActive(TRUE);
	}
	else
	{
		for(int i=0; i<MAX_STORAGELIST_NUM; ++i)
			m_pStorageBtn[i]->SetActive(FALSE);
	}

}

void CStorageDialog::MoveItem(MSG_ITEM_MOVE_SYN* pInfo)
{
}

void CStorageDialog::SelectedBtnPushDown(int i)
{
	for(int n=0; n<MAX_STORAGELIST_NUM; ++n)
	{
		if(n == i)
			m_pStorageBtn[n]->SetPush(TRUE);
		else
			m_pStorageBtn[n]->SetPush(FALSE);
	}		
}

BOOL CStorageDialog::FakeMoveIcon(LONG x, LONG y, cIcon * icon)
{
	// 071125 LYW --- ExchangeDialog : 봉인 해제 된 아이템 창고 보관 불가 처리.
	const ITEMBASE& pItemBase = ((CItem*)icon)->GetItemBaseInfo() ;

	if( pItemBase.nSealed == eITEM_TYPE_UNSEAL )
	{
		ITEM_INFO* pInfo = NULL ;
		pInfo = ITEMMGR->GetItemInfo( pItemBase.wIconIdx ) ;

		if( !pInfo ) return FALSE ;

		if( pInfo->nTimeKind != eKIND_REALTIME )
		{
			WINDOWMGR->MsgBox(MBI_SEALITEM_STORAGE, MBT_OK, CHATMGR->GetChatMsg(1173));

			return FALSE ;
		}
	}
	
	if(m_CurStorageMode >= eStorageMode_NoWare)
	{
		char buf[128];
		sprintf(buf, "CurStorageMode : %d", m_CurStorageMode);
		ASSERTMSG(0,buf);
		return FALSE;
	}

	if(m_CurStorageMode == 0)
		return FALSE;

	return m_StorageItemDlg[m_CurStorageMode-1]->FakeMoveIcon(x,y,icon);
}


BOOL CStorageDialog::AddItem(ITEMBASE* pItemInfo)
{
	int Num = GetStorageNum(pItemInfo->Position);
	return m_StorageItemDlg[Num]->AddItem(pItemInfo);
}

BOOL CStorageDialog::AddItem(CItem* pItem)
{	
	int Num = GetStorageNum(pItem->GetPosition());
	return m_StorageItemDlg[Num]->AddItem(pItem);
}

BOOL CStorageDialog::DeleteItem(POSTYPE Pos,CItem** ppItem)
{
	int Num = GetStorageNum(Pos);
	return m_StorageItemDlg[Num]->DeleteItem(Pos, ppItem);
}

void CStorageDialog::DeleteAllStorageItem()
{
	for(POSTYPE i=0; i<TABCELL_STORAGE_NUM; ++ i)
	{
		for(int j=0; j<MAX_STORAGELIST_NUM; ++j)
			m_StorageItemDlg[j]->DeleteItem(TP_STORAGE_START+i+j*TABCELL_STORAGE_NUM, NULL);
	}
}

int CStorageDialog::GetStorageNum(POSTYPE absPos)
{
	return ( absPos - TP_STORAGE_START ) / TABCELL_STORAGE_NUM;
}

/*
void CStorageDialog::SetStorageItem(SEND_STORAGE_ITEM_INFO* pInfo)
{
	// taiyo 안쓰임
	return;

	for(int i=0;i<pInfo->wCount;++i)
	{
		if( pInfo->StorageItem[i].dwDBIdx != 0 )
		{
			int Num = (pInfo->StorageItem[i].Position-TP_STORAGE_START)/TABCELL_STORAGE_NUM;
			m_StorageItemDlg[Num]->AddItem(&pInfo->StorageItem[i]);
		}
	}
	char temp[50] = {0,};
	sprintf(temp,"%14u", pInfo->money);
	m_pMoney->SetStaticText(temp);

}
*/


void CStorageDialog::SetStorageMoney(DWORD Money)
{
	m_pMoney->SetStaticText( AddComma( Money ) );
}

CItem * CStorageDialog::GetItemForPos(POSTYPE absPos)
{
	if(TP_STORAGE_START <= absPos && absPos < TP_STORAGE_END)
	{
		int num = GetStorageNum(absPos);
		return (CItem *)m_StorageItemDlg[num]->GetIconForIdx(absPos-TP_STORAGE_START-TABCELL_STORAGE_NUM*num);
	}
	else
		return NULL;
}

void CStorageDialog::SetActive( BOOL val )
{
	if( m_bDisable ) return;

	if(val == FALSE && m_StorageItemDlg[0] != NULL)
	{
		for(int i=0; i<TAB_STORAGE_NUM; ++i)
			m_StorageItemDlg[i]->SetActive(val);
	}
	if( val == FALSE)
	{
		if( HERO->GetState() == eObjectState_Deal)
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

		cDivideBox * pDivideBox = (cDivideBox *)WINDOWMGR->GetWindowForID( DBOX_INPUT_MONEY );
		if( pDivideBox )
		{
			pDivideBox->SetDisable(TRUE);
			pDivideBox->SetActive(FALSE);
			WINDOWMGR->AddListDestroyWindow( pDivideBox );
		}

		StorageMsgBoxClr();
	}
	cDialog::SetActive(val);

	// 070326 LYW --- StorageDialog : Close inventory.
	if( !val )
	{
		CInventoryExDialog* pWindow = GAMEIN->GetInventoryDialog() ;

		if( pWindow->IsActive() )
		{
			VECTOR2* pPos = pWindow->GetPrevPos() ;
			pWindow->SetAbsXY( (LONG)pPos->x, (LONG)pPos->y ) ;

			pWindow->SetActive( FALSE ) ;
		}
	}
}

void CStorageDialog::SetDividMsg( CItem* pFromItem, POSTYPE ToPos )
{
	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos( pFromItem->GetPosition() );
	if(FromIdx >= eItemTable_Max) return;

	BYTE ToIdx = ITEMMGR->GetTableIdxForAbsPos( ToPos );
	if(ToIdx >= eItemTable_Max) return;

	m_divideMsg.Category			= MP_ITEM;
	m_divideMsg.Protocol			= MP_ITEM_DIVIDE_SYN;
	m_divideMsg.dwObjectID			= HEROID;
	m_divideMsg.FromPos				= pFromItem->GetPosition();
	m_divideMsg.FromDur				= pFromItem->GetDurability();
	m_divideMsg.ToPos				= ToPos;
	m_divideMsg.ToDur				= 0;
	m_divideMsg.wItemIdx			= pFromItem->GetItemIdx();
}

void CStorageDialog::SetSelectedStorage(BYTE StorageNum)
{	
	m_StorageNum = StorageNum;
	SelectedBtnPushDown(StorageNum);
}

void CStorageDialog::FakeGeneralItemMove( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem)
{
	int Num = GetStorageNum(ToPos);
	m_StorageItemDlg[Num]->FakeGeneralItemMove(ToPos, pFromItem, pToItem);
}

int CStorageDialog::CanBuyStorage()
{
	BYTE storagenum = HERO->GetStorageNum();
	if(storagenum >= TAB_STORAGE_NUM)
	{
		ASSERT(0);
		return 3;
	}
	
	if(HERO->GetMoney() < m_StorageListInfo.Storage[HERO->GetStorageNum()].BuyPrice)
		return 1;
	return 0;
}

void CStorageDialog::StorageMsgBoxClr()
{
	cMsgBox * pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID(MBI_STORAGEBUY);
	if(pMsgBox)
		WINDOWMGR->AddListDestroyWindow(pMsgBox);
}

// 070326 LYW --- StorageDialog : Add function to active or not.
void CStorageDialog::ShowStorageDlg( BOOL val )
{
	SetAbsXY( (LONG)m_relPos.x, (LONG)m_relPos.y ) ;

	SetActive( val ) ;

	CInventoryExDialog* pWindow = GAMEIN->GetInventoryDialog() ;

	VECTOR2 vPos = {0, } ;
	vPos.x = pWindow->GetAbsX() ;
	vPos.y = pWindow->GetAbsY() ;

	memcpy( pWindow->GetPrevPos(), &vPos, sizeof(VECTOR2) ) ;

	if( val )
	{
		pWindow->SetAbsXY((LONG)(m_relPos.x + 10) + GetWidth(), (LONG)m_relPos.y ) ;

		if( !pWindow->IsActive() )pWindow->SetActive( TRUE ) ;
	}
}
#include "stdafx.h"															
#include ".\ItemMallBtnDlg.h"											

#include "../WindowIDEnum.h"												

#include "./Interface/cButton.h"											

#include "GameIn.h"															
#include "ObjectManager.h"

#include "InventoryExDialog.h"
#include "ItemShopDialog.h"

cItemMallBtnDlg::cItemMallBtnDlg(void)										
{
	m_pButton = NULL ;														

	m_nAlpha = 255 ;														
}

cItemMallBtnDlg::~cItemMallBtnDlg(void)										
{
}


void cItemMallBtnDlg::Linking() 											
{
	m_pButton = (cButton*)GetWindowForID(ITM_NOTICEBTN) ;					
}


void cItemMallBtnDlg::OnActionEvent(LONG lId, void* p, DWORD we ) 			
{
	if( lId != ITM_NOTICEBTN ) return ;	

	if( GAMEIN->GetItemShopDialog()->IsActive() )
	{
		GAMEIN->GetItemShopDialog()->SetActive( FALSE );
	}
	else
	{
		GAMEIN->GetItemShopDialog()->SetActive( TRUE );
		GAMEIN->GetInventoryDialog()->SetActive( TRUE );

		MSGBASE msg;																

		msg.Category = MP_ITEM;														
		msg.Protocol = MP_ITEM_SHOPITEM_INFO_SYN;									
		msg.dwObjectID = HERO->GetID();												

		NETWORK->Send(&msg, sizeof(msg));											
	}
}

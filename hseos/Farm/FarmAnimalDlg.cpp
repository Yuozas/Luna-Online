#include "stdafx.h"
#include "FarmAnimalDlg.h"
#include "WindowIDEnum.h"
#include ".\Interface\cWindowManager.h"
#include ".\interface\cIconDialog.h"
#include ".\interface\cGuageBar.h"
#include "ItemManager.h"
#include "GameIn.h"
#include "ObjectGuagen.h"
#include "ObjectManager.h"
#include "InventoryExDialog.h"
#include "ObjectStateManager.h"
#include "cScriptManager.h"
#include "../hseos/Farm/SHFarmManager.h"

int* CAnimalDialog::m_nImg = NULL;

CAnimalDialog::CAnimalDialog(void)
{
	m_pAnimal = NULL;
	m_bLockGetItemBtn = FALSE;
	m_CurImage.Clear();
}

CAnimalDialog::~CAnimalDialog(void)
{
}

void CAnimalDialog::Linking()
{
	m_pSlotNum				=	(cStatic*)GetWindowForID(FARM_ANIMAL_SLOT_NUM);
	m_pName					=	(cStatic*)GetWindowForID(FARM_ANIMAL_NAME);
	m_pContentmentGuage		=	(CObjectGuagen*)GetWindowForID(FARM_ANIMAL_CONTENTMENT_GUAGE);
	m_pContentment			=	(cStatic*)GetWindowForID(FARM_ANIMAL_CONTENTMENT);
	m_pHPGuage				=	(CObjectGuagen*)GetWindowForID(FARM_ANIMAL_HP_GUAGE);
	m_pHP					=	(cStatic*)GetWindowForID(FARM_ANIMAL_HP);
	m_pInterestGuage		=	(CObjectGuagen*)GetWindowForID(FARM_ANIMAL_INTEREST_GUAGE);
	m_pInterest				=	(cStatic*)GetWindowForID(FARM_ANIMAL_INTEREST);
	m_pImage				=	(cStatic*)GetWindowForID(FARM_ANIMAL_IMAGE);
	m_pGetItemBtn			=	(cPushupButton*)GetWindowForID(FARM_ANIMAL_GET_ITEM_BTN);

/*	for(int i = 0; i < CAnimal::ANIMAL_KIND_MAX; i++)
	{
		char szfile[64] = {0,};
		switch( i )
		{
		case CAnimal::ANIMAL_KIND_MELEE:
			sprintf(szfile,"./Data/Interface/2DImage/image/가축_양.tif");
			break;
		case CAnimal::ANIMAL_KIND_MAGIC:
			sprintf(szfile,"./Data/Interface/2DImage/image/가축_용.tif");
			break;
		case CAnimal::ANIMAL_KIND_BUFF:
			sprintf(szfile,"./Data/Interface/2DImage/image/가축_거북.tif");
			break;
		}
		m_ImageAnimal[i].LoadSprite(szfile);
		m_ImageAnimal[i].GetImageOriginalSize(&m_ImageAnimalVT);
	}*/

	GAMEIN->GetAnimalDialog()->GetItemBtn()->SetActive(FALSE);
}

void CAnimalDialog::SetActive(BOOL val)
{
	if( val )
	{
		OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);
	}
	else
		OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

	if( m_bDisable ) return;

	cDialog::SetActive(val);
}

void CAnimalDialog::Render()
{
	if(!m_bActive) return;	
	cDialog::RenderWindow();
	cDialog::RenderComponent();
}

BOOL CAnimalDialog::FakeMoveIcon(LONG x, LONG y, cIcon * pOrigIcon)
{
	//if( WT_ITEM != pOrigIcon->GetType() || pOrigIcon->IsLocked() || m_bDisable)
	//{
	//	return FALSE;
	//}

	//if(FISHINGMGR->IsActive())
	//{
	//	CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg(1529) );
	//	return FALSE;
	//}

	//CItem * pOrigItem = (CItem *)pOrigIcon;

	//// 인벤토리의 아이템만 허용된다
	//if( ! ITEMMGR->IsEqualTableIdxForPos(eItemTable_Inventory, pOrigItem->GetPosition() ) )
	//{
	//	CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 787 ) );
	//	return FALSE;
	//}

	//const DWORD				itemIndex	= pOrigItem->GetItemIdx();
	//const ITEM_INFO*		info		= ITEMMGR->GetItemInfo( itemIndex );
	//
	//// 미끼만 처리
	//if(ITEM_KIND_FISHING_BAIT == info->SupplyType)
	//{
	//	if(FISHINGMGR->GetFishingLevel() < info->Grade)
	//	{
	//		//등급제한
	//		CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg(1530) );
	//		return FALSE;
	//	}

	//	CBaseItem* pPrevItem = (CBaseItem*)m_pIconBait->GetIconForIdx(0);
	//	if(pPrevItem)
	//	{
	//		CItem* pItem = (CItem*)m_ItemBait.GetLinkItem();
	//		if(pItem)
	//			GAMEIN->GetInventoryDialog()->SetMultipleItemLock(pItem->GetItemIdx(), FALSE);

	//		SetBaitItem(NULL);
	//	}

	//	if(!m_pIconBait->IsAddable(0))
	//		return FALSE;

	//	SetBaitItem(pOrigItem);

	//	ITEMMGR->AddToolTip( &m_ItemBait );
	//	m_pIconBait->AddIcon( 0, &m_ItemBait);

	//	GAMEIN->GetInventoryDialog()->SetMultipleItemLock(pOrigItem->GetItemIdx(), TRUE);

	//	return FALSE;
	//}
	//
	return FALSE;
}

VOID CAnimalDialog::SetSlotNum(char* pText)
{
	if( pText )
	{
		m_pSlotNum->SetStaticText(pText);
	}
}

VOID CAnimalDialog::SetName(char* pText)
{
	if( pText )
	{
		m_pName->SetStaticText(pText);
	}
}

VOID CAnimalDialog::SetContentment(char* pText)
{
	if( pText )
	{
		m_pContentment->SetStaticText(pText);
	}
}

VOID CAnimalDialog::SetInterest(char* pText)
{
	if( pText )
	{
		m_pInterest->SetStaticText(pText);
	}
}

VOID CAnimalDialog::SetContentmentValue(GUAGEVAL val, DWORD estTime)
{
	if( m_pContentmentGuage )
	{
		m_pContentmentGuage->SetValue(val, estTime);
	}
}

VOID CAnimalDialog::SetInterestValue(GUAGEVAL val, DWORD estTime)
{
	if( m_pInterestGuage )
	{
		m_pInterestGuage->SetValue(val, estTime);
	}
}

VOID CAnimalDialog::SetAnimal(CAnimal* pAnimal)
{
	if( pAnimal )
	{
		m_pAnimal = pAnimal;

		RefreshAnimalInfo();
	}
}

VOID CAnimalDialog::SetHP(char* pText)
{
	if( m_pHP )
	{
		m_pHP->SetStaticText(pText);
	}
}

VOID CAnimalDialog::RefreshAnimalInfo()
{
	if( IsActive() )
	{
		if( m_pAnimal )
		{
			char text[256];

			memset(text, NULL, sizeof(text));
			sprintf( text, "%d/%d", m_pAnimal->GetContentment(), CAnimal::ANIMAL_STEP_MAX_CONTENTMENT);
			GAMEIN->GetAnimalDialog()->SetContentment(text);
			GAMEIN->GetAnimalDialog()->SetContentmentValue((GUAGEVAL)m_pAnimal->GetContentment()/(GUAGEVAL)CAnimal::ANIMAL_STEP_MAX_CONTENTMENT, 0);

			memset(text, NULL, sizeof(text));
			sprintf( text, "%d/%d", m_pAnimal->GetInterest(), CAnimal::ANIMAL_STEP_MAX_INTEREST);
			GAMEIN->GetAnimalDialog()->SetInterest(text);
			GAMEIN->GetAnimalDialog()->SetInterestValue((GUAGEVAL)m_pAnimal->GetInterest()/(GUAGEVAL)CAnimal::ANIMAL_STEP_MAX_INTEREST, 0);

			memset(text, NULL, sizeof(text));
			sprintf( text, "%d/%d", m_pAnimal->GetLife(), CAnimal::ANIMAL_STEP_MAX_LIFE );
			GAMEIN->GetAnimalDialog()->SetHP(text);
			GAMEIN->GetAnimalDialog()->SetHPValue((GUAGEVAL)m_pAnimal->GetLife()/(GUAGEVAL)CAnimal::ANIMAL_STEP_MAX_LIFE, 0);

			memset(text, NULL, sizeof(text));
			sprintf( text, "%d", m_pAnimal->GetID()+1 );
			GAMEIN->GetAnimalDialog()->SetSlotNum(text);

			memset(text, NULL, sizeof(text));
			sprintf( text, "%s", m_pAnimal->GetRenderObj()->GetObjectName() );
			GAMEIN->GetAnimalDialog()->SetName(text);

			if( m_pAnimal->GetContentment() == 100 && m_pAnimal->GetInterest() >= 10 )
			{
				if( !GAMEIN->GetAnimalDialog()->GetItemBtn()->IsActive() )
					GAMEIN->GetAnimalDialog()->GetItemBtn()->SetActive(TRUE);
			}
			else
			{
				if( GAMEIN->GetAnimalDialog()->GetItemBtn()->IsActive() )
					GAMEIN->GetAnimalDialog()->GetItemBtn()->SetActive(FALSE);
			}

			cImage img;
			SCRIPTMGR->GetImage( GAMEIN->GetAnimalDialog()->GetImgNum(m_pAnimal->GetKind()), &img, PFT_ITEMPATH);
			GAMEIN->GetAnimalDialog()->m_pImage->SetBasicImage(&img);
			GAMEIN->GetAnimalDialog()->m_pImage->SetActive(TRUE);
		}
	}
}

VOID CAnimalDialog::SetHPValue(GUAGEVAL val, DWORD estTime)
{
	if( m_pHPGuage )
	{
		m_pHPGuage->SetValue(val, estTime);
	}
}

void CAnimalDialog::OnActionEvent(LONG id, void* p, DWORD event)
{
	// 함수 파라메터 체크.
	ASSERT(p) ;

	if(!p)
	{
		return ;
	}

	// Check id & Handling message.
	switch(id)
	{
	case FARM_ANIMAL_GET_ITEM_BTN:
		{
			//if (GetRenderObjKind(m_pcsSelFarmObj->GetInfo()->nKind) == RENDER_ANIMAL_KIND)
			//{
				g_csFarmManager.CLI_RequestAnimalGetItem();
				//m_stSelFarmObj.
			//}
			//g_csFarmManager.GetFarmZoneID
		}
		break;
	case FARM_ANIMAL_CLOSE_BTN:
		GAMEIN->GetAnimalDialog()->SetActive(FALSE);
		break;

	default : break ;
	}
}

VOID CAnimalDialog::SetAnimalImgInfo(int nState, int nAnimalKind, int nAnimalImg)
{
	if( nState == 0 )
	{
		g_csFarmManager.LoadFarmInfo(0);
		SAFE_DELETE_ARRAY(m_nImg);
		m_nImg = new int[CSHFarmManager::RENDER_ANIMAL_KIND_NUM];
		ZeroMemory(m_nImg, sizeof(int)*CSHFarmManager::RENDER_ANIMAL_KIND_NUM);
	}
	else if( nState == 1 )
	{
		ASSERT( nAnimalKind <= CSHFarmManager::RENDER_ANIMAL_KIND_NUM );
		m_nImg[nAnimalKind] = nAnimalImg;
	}
}

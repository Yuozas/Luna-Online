#include "stdafx.h"
#include "InventoryExDialog.h"
#include "WindowIDEnum.h"
#include "Interface/cWindowManager.h"
#include "Interface/cIconGridDialog.h"
#include "Interface/cStatic.h"
#include "Interface/cPushupButton.h"
#include "ObjectStateManager.h"

#include "GameIn.h"

#include "ItemManager.h"
#include "ObjectManager.h"
#include "KyungGongManager.h"
#include "cSkillTreeManager.h"
#include "ChatManager.h"

#include "DealItem.h"
#include "cDivideBox.h"
#include "./Audio/MHAudioManager.h"

#include "DealDialog.h"

#include "./Input/UserInput.h"

//#include "StorageDialog.h"
//#include "GuildWarehouseDialog.h"
//#include "GuildNoteDlg.h"
//#include "UnionNoteDlg.h"

// 061220 LYW --- Delete this code.
//#include "MainBarDialog.h"
#include "PKManager.h"
#include "cMsgBox.h"
#include "QuestManager.h"
#include "MoveDialog.h"
#include "PointSaveDialog.h"
#include "QuickSlotDlg.h"
#include "ProgressDialog.h"
#include "SkillPointRedist.h"
#include "ChaseinputDialog.h"
#include "SealDialog.h"
#include "ChangeJobDialog.h"
//#include "ReinforceResetDlg.h"
//#include "ReinforceDlg.h"
//#include "MixDialog.h"
#include "NameChangeDialog.h"
#include "NameChangeNotifyDlg.h"
//#include "RareCreateDialog.h"
#include "StreetStallManager.h"
#include "ShoutDialog.h"

// 080218 LUJ
#include "ApplyOptionDialog.h"

#include "StreetStallManager.h"
#include "ShoutDialog.h"

#include "ShowdownManager.h"
#include "MoveManager.h"
#include "./Interface/cScriptManager.h"

#include "NpcScriptManager.h"
#include "MHMap.h"
#include "SiegeWarMgr.h"

// 080228 LUJ, ��ȣ ������ ó��
#include "MixDialog.h"
#include "ReinforceDlg.h"
#include "EnchantDialog.h"

// desc_hseos_����ý���_01
// S ����ý��� �߰� added by hseos 2007.05.15
#include "../hseos/Farm/SHFarmManager.h"
// E ����ý��� �߰� added by hseos 2007.05.15

// 071022 LYW --- InventoryExDialog : Include header file of tutorial manager.
#include "TutorialManager.h"

// 071228 LYW --- InventoryExDialog : �̸� ���� â ��� ȣ��.
#include "../cChangeNameDialog.h"

// 080102 LYW --- InventoryExDialog : �йи� �Ŵ��� ��� ȣ��.
#include "../hseos/Family/SHFamilyManager.h"

// 080414 LUJ, �ܾ� ��ȯ â
#include "BodyChangeDialog.h"

#include "FishingDialog.h"
#include "FishingManager.h"

#include "PetManager.h"
#include "PetResurrectionDialog.h"
#include "PetWearedDialog.h"
#include "./interface/cResourceManager.h"


CInventoryExDialog::CInventoryExDialog()
{
	m_type				= WT_INVENTORYDIALOG;
	m_pWearedDlg		= NULL;
	m_pMoneyEdit		= NULL;
	m_fSpread			= FALSE;
	m_iInventoryState	= eITEMDEFAULT_STATE;
	m_pItemShopInven	= NULL;
	m_pItemShopBtn		= NULL;
	m_bShopInven		= FALSE;

	m_BtnPushstartTime		= 0;
	m_BtnPushDelayTime		= 500;
	m_bPushTabWithMouseOver = FALSE;
	
	m_pSelectedShopItem = NULL;

	// 070423 LYW --- InventoryExDialog : Add background image of inventory.
	for( BYTE count = 0 ; count < MAX_BACKGROUND ; ++count )
	{
		m_pBackground[count] = NULL ;
	}

	// 070426 LYW --- InveltoryExDialog : Setting prevPosition.
	DISPLAY_INFO	dispInfo ;

	GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;

	switch( dispInfo.dwWidth )
	{
	case 800 :
		{
			m_PrevPos.x = 380 ;
			m_PrevPos.y = 80 ;
		}
		break ;

	case 1024 :
		{
			m_PrevPos.x = 476 ;
			m_PrevPos.y = 160 ;
		}
		break ;

	case 1280 :
		{
			m_PrevPos.x = 615 ;
			m_PrevPos.y = 240 ;
		}
		break ;
	}

	// 071204 LYW --- InventoryExDialog : ���� ���� ���� ��� �� ������ ������ �߰�.
	// ���� ���� ������ ����ϰ� ���� ��, �ٽ� �ѹ� ���� ���� ������ ����ϸ�, �޽����� ����.
	// �޽��� ó�� �ο���, Yes�� �����ϸ�, ��� ��û�� �����µ�, �̶� ������ ������ �ʿ��ϴ�.
	// ������, �������� �� ���Կ� ����ϰ� �κ��� ������ �ʰ� ����� ��찡 ����µ�, 
	// �̶� GetCurSelectedItem()���� ����� �� �����͸� �ѱ��� ���ϱ� ������ 
	// �� ���� ��� �� �ӽ� ������ �����͸� ���� �� ����Ѵ�.
	m_pQuickSelectedItem = NULL ;
}

CInventoryExDialog::~CInventoryExDialog()
{
	m_pWearedDlg		= NULL;
	m_pItemShopInven	= NULL;
	m_pItemShopBtn		= NULL;
}

void CInventoryExDialog::ReleaseInventory()
{
	//�ؽ� ���̺� �����ؼ� �����?
	//�׸��� ������ dlg -> delete;
	//�����쿡 adddestroylist
}

void CInventoryExDialog::Render()
{
	cDialog::RenderWindow();
	cDialog::RenderComponent();
	cTabDialog::RenderTabComponent();
}
void CInventoryExDialog::Add(cWindow * window)
{
	// 071210 LYW --- InventoryExDialog : �κ��丮 Ȯ�忡 ���� Ȯ��������� Ȱ��ȭ ���� ó��.
	if(window->GetType() == WT_PUSHUPBUTTON)
	{
		BYTE byCurIdx = 0 ;
		byCurIdx = curIdx1 ;

		AddTabBtn(curIdx1++, (cPushupButton * )window);

        if( byCurIdx >= 2 )
		{
			//window->SetActive(FALSE) ;
			((cPushupButton*)window)->SetActive(FALSE) ;
		}
	}
	else if(window->GetType() == WT_ICONGRIDDIALOG)
	{
		BYTE byCurIdx = 0 ;
		byCurIdx = curIdx2 ;

		AddTabSheet(curIdx2++, window);
		((cIconGridDialog*)window)->SetDragOverIconType( WT_ITEM );

		if( byCurIdx >= 2 )
		{
			//window->SetActive(FALSE) ;
			((cIconGridDialog*)window)->SetActive(FALSE) ;
		}
	}
	else 
		cDialog::Add(window);

	/*if(window->GetType() == WT_PUSHUPBUTTON && window->GetID() != IN_SHOPITEMBTN)
		AddTabBtn(curIdx1++, (cPushupButton * )window);
	else if(window->GetType() == WT_ICONGRIDDIALOG && window->GetID() != IN_SHOPITEMGRID)
	{
		AddTabSheet(curIdx2++, window);
		((cIconGridDialog*)window)->SetDragOverIconType( WT_ITEM );
	}
	else 
		cDialog::Add(window);*/

	//if(window->GetType() == WT_PUSHUPBUTTON)
	//	AddTabBtn(curIdx1++, (cPushupButton * )window);
	//else if(window->GetType() == WT_ICONGRIDDIALOG)
	//{
	//	AddTabSheet(curIdx2++, window);
	//	((cIconGridDialog*)window)->SetDragOverIconType( WT_ITEM );
	//}
	//else 
	//	cDialog::Add(window);
}


DWORD CInventoryExDialog::ActionEvent(CMouse * mouseInfo)
{
	//������ �巡�����϶�.. �ٸ� ������ �̵��ؾ��Ѵ�.
	if( WINDOWMGR->IsDragWindow() && WINDOWMGR->GetDragDlg() )
	{
		if( !m_bShopInven && WINDOWMGR->GetDragDlg()->GetType() == WT_ITEM )
		{
			DWORD we = WE_NULL;
			if(!m_bActive) return we;
			we = cDialog::ActionEvent(mouseInfo);

			BOOL bMouseOver = FALSE;
			for( int i = 0 ; i < m_bTabNum ; i++ )
			{
				m_ppPushupTabBtn[i]->ActionEvent( mouseInfo );
				
				if( we & WE_MOUSEOVER )
				if( m_ppPushupTabBtn[i]->IsActive() && m_ppPushupTabBtn[i]->PtInWindow( mouseInfo->GetMouseX(), mouseInfo->GetMouseY() ) )
				{
					bMouseOver = TRUE;
					
					if( m_bPushTabWithMouseOver )
					{
						if( i != m_bSelTabNum )
						{
							SelectTab(i);		//������Ű��(����â����..)
							m_bSelTabNum = i;							
						}
					}
					else
					{
						if( m_BtnPushstartTime == 0 )
							m_BtnPushstartTime = gCurTime;
						else if( gCurTime - m_BtnPushstartTime > m_BtnPushDelayTime )
							m_bPushTabWithMouseOver = TRUE;
					}				
				}
			}
			
			if( !bMouseOver )
				m_BtnPushstartTime = 0;
			
			we |= m_ppWindowTabSheet[m_bSelTabNum]->ActionEvent( mouseInfo );
			return we;
		}
	}

	m_BtnPushstartTime		= 0;
	m_bPushTabWithMouseOver = FALSE;
	return cTabDialog::ActionEvent( mouseInfo );	
}


WORD CInventoryExDialog::GetTabIndex( POSTYPE absPos )
{
	return ( absPos - TP_INVENTORY_START ) / TABCELL_INVENTORY_NUM;
}

void CInventoryExDialog::RefreshInvenItem()
{
	if(!HERO)
		return;

	BYTE TabNum = GetTabNum();
	for( BYTE i = 0 ; i < TabNum ; ++i )
	{
		cIconGridDialog* gridDlg = (cIconGridDialog*)GetTabSheet(i);

		for( WORD j = 0 ; j < gridDlg->GetCellNum() ; ++j )
		{
			CItem * pItem = (CItem *)gridDlg->GetIconForIdx(TP_INVENTORY_START+j);
			
			if(pItem)
				ITEMMGR->RefreshItem( pItem );
		}
	}

	// �������� ��� ����
	for( WORD j = eWearedItem_Weapon ; j < eWearedItem_Max ; ++j )
	{
		CItem * pItem = GetItemForPos( TP_WEAR_START + j );
		if(pItem)
			ITEMMGR->RefreshItem( pItem );
	}
}

BOOL CInventoryExDialog::AddItem(ITEMBASE * itemBase)
{
	if( itemBase->dwDBIdx == 0 )
	{
//		DEBUGMSG( 0, "Item DB idx == 0" );
		return FALSE;
	}

	CItem * newItem = ITEMMGR->MakeNewItem( itemBase, "AddItem" );
	if(newItem == NULL)
		return FALSE;

	// 071125 LYW --- InventoryExDialog : �ð� �������� ���, ���� �ð� ����.
	if( itemBase->nSealed == eITEM_TYPE_UNSEAL )
	{
		newItem->SetItemBaseInfo( *itemBase ) ;
	}
	
	return AddItem( newItem );
}

// ���� Dura�� ���� ���� �ش�.���ҽ� ��������.
BOOL CInventoryExDialog::AddItem(CItem* pItem)
{
	if(pItem == NULL)
	{
		ASSERT(pItem);
		return FALSE;
	}

	//�� ��ȯ
	ITEMMGR->RefreshItem( pItem );
	//add �ϴ� �����۸� refresh �ϸ� �ȴ�?

	// 071210 LYW --- InventoryExDialog : �Ʒ� �ҽ����� �������� ������ ��� �޾Ƽ� �����ϴ� ����,
	// �ѹ� �޾Ƽ� ��� ����ϵ��� ������.
	POSTYPE pos = 0 ;
	pos = pItem->GetPosition() ;
	BOOL bRes = FALSE;


	if(TP_INVENTORY_START <= pos && pos < (TP_INVENTORY_END + ( TABCELL_INVENTORY_NUM * HERO->Get_HeroExtendedInvenCount() )))
	{
		
		// �κ��丮 
		int tabIndex = GetTabIndex( pItem->GetPosition() );
		cIconGridDialog * dlg = (cIconGridDialog *)GetTabSheet(tabIndex);

//		WORD relPos = pItem->GetPosition()-INVENTORY_STARTPOSITION-CELLNUMPERPAGE*tabIndex;
		WORD relPos = pos-TP_INVENTORY_START-TABCELL_INVENTORY_NUM*tabIndex;
		CItem * item = (CItem * )dlg->GetIconForIdx(relPos);
		if(item)
		{
			ASSERT(0);
			return FALSE;
		}
		else
		{
			//return dlg->AddIcon(relPos, pItem);
			bRes = dlg->AddIcon(relPos, pItem);
			ITEMMGR->UpdateDlgCoolTime(pItem->GetItemIdx());	// 080326 NYJ --- ��Ÿ�Ӱ���
			return bRes;
		}
	}
	else if(TP_WEAR_START <= pos && pos < TP_WEAR_END)
	{
		// ����â
		//return m_pWearedDlg->AddItem(pos-TP_WEAR_START, pItem);
		bRes = m_pWearedDlg->AddItem(pos-TP_WEAR_START, pItem);
		ITEMMGR->UpdateDlgCoolTime(pItem->GetItemIdx());	// 080326 NYJ --- ��Ÿ�Ӱ���
		return bRes;
	}
	else if(TP_SHOPINVEN_START <= pos && pos < TP_SHOPINVEN_END-TABCELL_SHOPINVEN_NUM)
	{
		//return m_pItemShopInven->AddItem( pItem );
		bRes = m_pItemShopInven->AddItem( pItem );
		ITEMMGR->UpdateDlgCoolTime(pItem->GetItemIdx());	// 080326 NYJ --- ��Ÿ�Ӱ���
		return bRes;
	}
	else
		return FALSE;
}

BOOL CInventoryExDialog::DeleteItem(POSTYPE Pos,CItem** ppItem)
{
//	if(INVENTORY_STARTPOSITION <= Pos && Pos < INVENTORY_ENDPOSITION)
	if(TP_INVENTORY_START <= Pos && Pos < (TP_INVENTORY_END + ( TABCELL_INVENTORY_NUM * HERO->Get_HeroExtendedInvenCount() )))
	{
		int TabIndex = GetTabIndex( Pos );
		cIconGridDialog * Dlg = (cIconGridDialog *)GetTabSheet(TabIndex);
		if(!Dlg->IsAddable(Pos - TP_INVENTORY_START-TABCELL_INVENTORY_NUM*TabIndex))
			return Dlg->DeleteIcon(Pos-TP_INVENTORY_START-TABCELL_INVENTORY_NUM*TabIndex, (cIcon **)ppItem);
//		if(!Dlg->IsAddable(Pos - INVENTORY_STARTPOSITION-CELLNUMPERPAGE*TabIndex))
//			return Dlg->DeleteIcon(Pos-INVENTORY_STARTPOSITION-CELLNUMPERPAGE*TabIndex, (cIcon **)ppItem);
		else
			return FALSE;
	}
//	else if(WEAR_STARTPOSITION <= Pos && Pos < WEAR_ENDPOSITION)
	else if(TP_WEAR_START <= Pos && Pos < TP_WEAR_END)
	{
//		if(!m_pWearedDlg->IsAddable(Pos - WEAR_STARTPOSITION))
//			return m_pWearedDlg->DeleteItem(Pos-WEAR_STARTPOSITION, (cIcon **)ppItem);
		if(!m_pWearedDlg->IsAddable(Pos - TP_WEAR_START))
			return m_pWearedDlg->DeleteItem(Pos-TP_WEAR_START, (cIcon **)ppItem);
		else
		{
			return FALSE;
		}
	}
	else if(TP_SHOPINVEN_START <= Pos && Pos < TP_SHOPINVEN_END)
	{
		if(!m_pItemShopInven->IsAddable(Pos-TP_SHOPINVEN_START))
			return m_pItemShopInven->DeleteItem(Pos, ppItem);
		else
		{
			return FALSE;
		}
	}
	else
		return FALSE;
}
BOOL CInventoryExDialog::UpdateItemDurabilityAdd(POSTYPE absPos, int dur)
{
//	if(INVENTORY_STARTPOSITION <= absPos && absPos < INVENTORY_ENDPOSITION)
	if(TP_INVENTORY_START <= absPos && absPos < (TP_INVENTORY_END + ( TABCELL_INVENTORY_NUM * HERO->Get_HeroExtendedInvenCount() )))
	{
		int TabIndex = GetTabIndex( absPos );
		cIconGridDialog * Dlg = (cIconGridDialog *)GetTabSheet(TabIndex);
//		WORD relPos = absPos-INVENTORY_STARTPOSITION-CELLNUMPERPAGE*TabIndex;
		WORD relPos = absPos-TP_INVENTORY_START-TABCELL_INVENTORY_NUM*TabIndex;
		CItem * item = (CItem * )Dlg->GetIconForIdx(relPos);
		item->SetDurability(item->GetDurability()+dur);
		return TRUE;
	}
	else if(TP_SHOPINVEN_START <= absPos && absPos < TP_SHOPINVEN_END)
	{
		CItem* pItem = (CItem*)m_pItemShopInven->GetIconForIdx(absPos-TP_SHOPINVEN_START);
		if(pItem)
		{
			pItem->SetDurability(pItem->GetDurability()+dur);
			return TRUE;
		}
		else
			return FALSE;
	}
	else
		return FALSE;
}
CItem * CInventoryExDialog::GetItemForPos(POSTYPE absPos)
{
//	if(INVENTORY_STARTPOSITION <= absPos && absPos < INVENTORY_ENDPOSITION)
	if(TP_INVENTORY_START <= absPos && absPos < (TP_INVENTORY_END + ( TABCELL_INVENTORY_NUM * HERO->Get_HeroExtendedInvenCount() )))
	{
		int TabIndex = GetTabIndex( absPos );
		cIconGridDialog * Dlg = (cIconGridDialog *)GetTabSheet(TabIndex);
		return (CItem *)Dlg->GetIconForIdx(absPos-TP_INVENTORY_START-TABCELL_INVENTORY_NUM*TabIndex);
//		return (CItem *)Dlg->GetIconForIdx(absPos-INVENTORY_STARTPOSITION-CELLNUMPERPAGE*TabIndex);
	}
//	else if(WEAR_STARTPOSITION <= absPos && absPos < WEAR_ENDPOSITION)
	else if(TP_WEAR_START <= absPos && absPos < TP_WEAR_END)
	{
//		return (CItem *)m_pWearedDlg->GetIconForIdx(absPos-WEAR_STARTPOSITION);
		return (CItem *)m_pWearedDlg->GetIconForIdx(absPos-TP_WEAR_START);
	}
	else if(TP_SHOPINVEN_START <= absPos && absPos < TP_SHOPINVEN_END)
	{
		return (CItem*)m_pItemShopInven->GetIconForIdx(absPos-TP_SHOPINVEN_START);
	}
	else
		return NULL;
}

void CInventoryExDialog::UseItem(CItem* pItem)
{
	ASSERT(pItem);

	CHero* hero = OBJECTMGR->GetHero();
	ASSERT( hero );

	if( WINDOWMGR->IsDragWindow() && WINDOWMGR->GetDragDlg() )
	if( WINDOWMGR->GetDragDlg()->GetType() == WT_ITEM)
		return;

	{
		const DWORD coolTime = pItem->GetRemainedCoolTime();

		if( coolTime )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1083 ), coolTime / 1000.0f );
			return;
		}
	}
//KES...
	if( m_bDisable )	return;		//�κ� �𽺿��̺��̸� ������ ��� �Ұ���

	if( pItem->GetItemInfo()->EquipType == eEquipType_Pet )
	{
		if( !HEROPET )
			return;

		GAMEIN->GetPetWearedDlg()->FakeMoveIcon( 0, 0, pItem );
		return;
	}	
//KES EXCHANGE 031002

//	
	if( hero->GetState() == eObjectState_Die )	//���� ���¿����� ������ ��� ���Ѵ�.
		return;

	// 080411 LUJ, ������ ��� ������ üũ�ϵ��� �Ѵ�. ����: ��� ������ ���� �������� 0���� �����Ǿ� �ִ�
	{
		const ITEM_INFO* info = pItem->GetItemInfo();

		if( info				&&
			info->LimitLevel	&&
			info->LimitLevel > hero->GetLevel() )
		{
			return;
		}
	}
	
	if( pItem->GetItemInfo()->SupplyType != ITEM_KIND_PET && pItem->IsLocked() ) return;

	// desc_hseos_����ý���_01
	// S ����ý��� �߰� added by hseos 2007.09.18
	// ..���� ��� ���� �� ����
	if( g_csFarmManager.CLI_GetIngCharMotion() )
		return;
	// E ����ý��� �߰� added by hseos 2007.09.18

	if( !pItem->GetItemInfo()->Battle && HERO->GetObjectBattleState() == eObjectBattleState_Battle )
		return;

	if( !pItem->GetItemInfo()->Peace && HERO->GetObjectBattleState() == eObjectBattleState_Peace )
		return;

	if( !pItem->GetItemInfo()->Move && HERO->GetBaseMoveInfo()->bMoving )
		return;

	if( !pItem->GetItemInfo()->Stop && !HERO->GetBaseMoveInfo()->bMoving )
		return;

	if( pItem->GetItemInfo()->SupplyType == ITEM_KIND_PET_NORMAL_RESURRECTION ||
		pItem->GetItemInfo()->SupplyType == ITEM_KIND_PET_SPECIAL_RESURRECTION )
	{
		CPetResurrectionDialog* window = GAMEIN->GetPetResurrectionDlg();

		if(		window &&
			!	window->IsActive() )
		{
			window->SetUsedItem( pItem );
			window->SetActive( TRUE );
		}

		return;
	}

    // 080218 LUJ, �ɼ� �ο� �������̸� â�� ǥ���Ѵ�
	if( ITEMMGR->IsApplyOptionItem( pItem->GetItemIdx() ) )
	{
		CApplyOptionDialog* window = ( CApplyOptionDialog* )WINDOWMGR->GetWindowForID( APPLY_OPTION_DIALOG );

		if(		window &&
			!	window->IsActive() )
		{
			window->SetSourceItem( *pItem );
			window->SetActive( TRUE );
		}

		return;
	}
	// 080228 LUJ, ���� ��ȣ �������� ��� ���� â ǥ��
	else if( ITEMMGR->IsMixProtectionItem( pItem->GetItemBaseInfo() ) )
	{
		CMixDialog* window = ( CMixDialog* )WINDOWMGR->GetWindowForID( ITMD_MIXDLG );

		if(	window )
		{
			window->SetProtectedActive( *pItem );
		}

		return;
	}

	// 080228 LUJ, ��ȭ ��ȣ �������� ��� ��ȭ â ǥ��
	else if( ITEMMGR->IsReinforceProtectionItem( pItem->GetItemBaseInfo() ) )
	{
		CReinforceDlg* window = ( CReinforceDlg* )WINDOWMGR->GetWindowForID( ITR_REINFORCEDLG );

		if(	window )
		{
			window->SetProtectedActive( *pItem );
		}

		return;
	}
	// 080228 LUJ, ��æƮ ��ȣ �������� ��� ��æƮ â ǥ��
	// 080320 LUJ, ��æƮ ���� ��ȣ/��ü ��ȣ ���¿� ���� �ý��� �޽��� �и�
	else
	{
		typedef CItemManager::EnchantProtection	EnchantProtection;

		const EnchantProtection&							protection	= ITEMMGR->GetEnchantProtection();
		const EnchantProtection::ItemMap::const_iterator	it			= protection.mItemMap.find( pItem->GetItemIdx() );

		CEnchantDialog* window = ( CEnchantDialog* )WINDOWMGR->GetWindowForID( ITD_UPGRADEDLG );

		// 080328 LUJ, ��æƮ ��ȣ ������ �� ������ ��
		if(	window &&
			protection.mItemMap.end() != it )
		{
			CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 1454 ) );
			
			window->SetProtectedActive( *pItem );
			return;
		}		
	}

	// 080414 LUJ, �ܾ� ��ȯ �������� ��� �ش� â ǥ��
	{
		typedef CItemManager::BodyChangeScriptListMap BodyChangeScriptListMap;

		const BodyChangeScriptListMap&			scriptListMap	= ITEMMGR->GetBodyChangeScriptListMap();
		BodyChangeScriptListMap::const_iterator	it				= scriptListMap.find(  pItem->GetItemIdx() );

		CBodyChangeDialog*	window = ( CBodyChangeDialog* )WINDOWMGR->GetWindowForID( BODY_CHANGE_DIALOG );

		if(	window &&
			scriptListMap.end() != it )
		{
			window->SetActive( TRUE, *pItem );

			SetActive( FALSE );
			ITEMMGR->SetDisableDialog( TRUE, eItemTable_Inventory );
			return;
		}
	}

	switch( pItem->GetItemInfo()->SupplyType )
	{
	case ITEM_KIND_PET_SKILL:					
	case ITEM_KIND_PET_COSTUME:				
	case ITEM_KIND_PET_EQUIP:					
	case ITEM_KIND_PET_EGG:					
	case ITEM_KIND_PET_FOOD:				
	case ITEM_KIND_PET_HP_RECOVER:			
	case ITEM_KIND_PET_MP_RECOVER:			
		{
			if( !HEROPET )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(360));
				return;
			}
		}
		break;
	case ITEM_KIND_PET_EVOLUTION_PYHSIC_TYPE:	
	case ITEM_KIND_PET_EVOLUTION_MAGIC_TYPE:	
	case ITEM_KIND_PET_EVOLUTION_SUPPORT_TYPE:
		{
			if( !HEROPET )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(360));
				return;
			}
			if( HEROPET->GetLevel() != PETMGR->GetGradeUpLevel( HEROPET->GetGrade() ) )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(789));
				return;
			}

			if( HEROPET->GetType() != ePetType_Basic &&
				HEROPET->GetType() != pItem->GetItemInfo()->SupplyType - ITEM_KIND_PET_EVOLUTION_PYHSIC_TYPE + 1 )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1577));
				return;
			}
		}
		break;
	}

	switch( pItem->GetItemInfo()->Category )
	{
	case eItemCategory_Equip:
		{
			if( OBJECTSTATEMGR->CheckAvailableState(hero,eCAS_EQUIPITEM) == FALSE )
			{
				CAction act;
				act.InitAction(eActionKind_EquipItem,(void*)pItem->GetDBIdx(),0,NULL);
				hero->SetNextAction(&act);
				return;
			}

			// ���� ������	
			switch( GetTableIdxPosition( pItem->GetPosition() ) )
			{
			case eItemTable_Inventory:
				{
					if(ITEMMGR->CanEquip(pItem->GetItemIdx()))
					{
						ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
						if(!pInfo) return ;

						WORD EquipToPos = pInfo->EquipSlot;
						CItem * pToItem = GetItemForPos( EquipToPos + TP_WEAR_START );
					
						if( EquipToPos == eEquipSlot_Ring1 - 1 )
						{
							if( !GetItemForPos( eEquipSlot_Ring2 - 1 + TP_WEAR_START ) )
							{
								pToItem = GetItemForPos( eEquipSlot_Ring2 - 1 + TP_WEAR_START );
								EquipToPos = eEquipSlot_Ring2 - 1;
							}
						}
						else if( EquipToPos == eEquipSlot_Earring1 - 1 )
						{
							if( !GetItemForPos( eEquipSlot_Earring2 - 1 + TP_WEAR_START ) )
							{
								pToItem = GetItemForPos( eEquipSlot_Earring2 - 1 + TP_WEAR_START );
								EquipToPos = eEquipSlot_Earring2 - 1;
							}
						}
						else if( EquipToPos == eEquipSlot_Shield - 1 )
						{
							if( hero->GetWeaponAniType() == eWeaponAnimationType_TwoHand ||
								hero->GetWeaponAniType() == eWeaponAnimationType_Staff ||
								hero->GetWeaponAniType() == eWeaponAnimationType_Bow ||
								hero->GetWeaponAniType() == eWeaponAnimationType_Gun ||
								hero->GetWeaponAniType() == eWeaponAnimationType_TwoBlade )						
							{
								CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(772) );
								return;
							}
						}
						
						FakeGeneralItemMove(EquipToPos + TP_WEAR_START, pItem, pToItem);
/*
						if( pInfo->WeaponAnimation == eWeaponAnimationType_TwoHand ||
							pInfo->WeaponAnimation == eWeaponAnimationType_Staff ||
							pInfo->WeaponAnimation == eWeaponAnimationType_Bow ||
							pInfo->WeaponAnimation == eWeaponAnimationType_Gun ||
							pInfo->WeaponAnimation == eWeaponAnimationType_TwoBlade )
						{
							CItem* pShield = GetItemForPos( eEquipSlot_Shield - 1 + TP_WEAR_START );

							if( pShield )
							{
								POSTYPE emptyPos;
								if( GetBlankPositionRestrictRef( emptyPos ) )
								{
									if( emptyPos > pItem->GetPosition() )
									{
										emptyPos = pItem->GetPosition();
									}
								}
								else
								{
									emptyPos = pItem->GetPosition();
								}

								FakeGeneralItemMove(emptyPos, pShield, NULL);
							}
						}
						*/
					}
					else
					{
						if(pItem->GetItemInfo()->WeaponType == eWeaponType_FishingPole)
							CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1530)) ;
						else
							CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(825), pItem->GetItemInfo()->ItemName) ;	// "������ ���� ���� �� �� �����ϴ�."
					}
				}
				break;
			case eItemTable_Weared:
				{
					POSTYPE emptyPos;
					if( GetBlankPositionRestrictRef( emptyPos ) )
					{
						CItem * pToItem = GetItemForPos( emptyPos );
						FakeGeneralItemMove(emptyPos, pItem, pToItem);
					}
				}
				break;
			}
		}
		break;
	case eItemCategory_Expend:
		{
			// desc_hseos_����ý���_01
			// S ����ý��� �߰� added by hseos 2007.05.14	2007.08.21
			// ..���� ������ �Ϻδ� Ÿ���� �����ϰ� ����ؾ� �ϱ� ������ ������ ���� �ٸ���. ���� �߰�
			const ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());

			if( ! pInfo )
			{
				return;
			}

			switch( pInfo->SupplyType )
			{
			case ITEM_KIND_BUFF_ITEMS :
				{
					if( HERO->GetState()==eObjectState_Fishing || HERO->GetState()==eObjectState_FishingResult )
					{
						WINDOWMGR->MsgBox( MBI_CANTUSE_MAPMOVESCROLL, MBT_OK, CHATMGR->GetChatMsg(789) ) ;
						return ;
					}

					Set_QuickSelectedItem(pItem) ;

					if( ITEMMGR->IsUsingItem(pItem) )
					{
						WINDOWMGR->MsgBox( MBI_FORCE_USEBUFFITEM_AREYOUSURE, MBT_YESNO, CHATMGR->GetChatMsg(1224) ) ;
						return ;
					}

					ASSERT(pInfo) ;
					char tempBuf[128] = {0, } ;
					char newItemName[128] = {0, } ;

					char* pPersent = NULL ;

					pPersent = strchr( pInfo->ItemName, '%' ) ;
					if( !pPersent )
					{
						WINDOWMGR->MsgBox( MBI_USEBUFFITEM_AREYOUSURE, MBT_YESNO, CHATMGR->GetChatMsg(1244), pInfo->ItemName ) ;
					}
					else
					{
						strncpy(tempBuf, pInfo->ItemName, strlen(pInfo->ItemName) - strlen(pPersent)) ;

						sprintf(newItemName, "%s%c", tempBuf, '%') ;
						strcat(newItemName, pPersent) ;

						memset(tempBuf, 0, sizeof(tempBuf)) ;
						sprintf(tempBuf, CHATMGR->GetChatMsg(1244), newItemName) ;

						WINDOWMGR->MsgBox( MBI_USEBUFFITEM_AREYOUSURE, MBT_YESNO, tempBuf ) ;
					}
				}
				return ;

			// 071203 LYW --- InventoryExDialog : ���� �ʱ�ȭ ������ �߰�.
			case ITEM_KIND_RESET_STATS :
				{
					if( HERO->GetState() != eObjectState_None )
					{
						WINDOWMGR->MsgBox( MBI_CANTUSE_MAPMOVESCROLL, MBT_OK, CHATMGR->GetChatMsg(789) ) ;
						return ;
					}

					// 080124 KTH -- ������ ����϶����� ����� �� �����ϴ�.
					if( HERO->IsPKMode() )
					{
						WINDOWMGR->MsgBox( MBI_CANTUSE_MAPMOVESCROLL, MBT_OK, CHATMGR->GetChatMsg(1263) ) ;
						return;
					}

					pItem->SetLock(TRUE) ;
					Set_QuickSelectedItem(pItem) ;
					WINDOWMGR->MsgBox( MBI_RESETSTATS_AREYOUSURE, MBT_YESNO, CHATMGR->GetChatMsg(1223) ) ;
					return ;
				}
				return ;

			// 071203 LYW --- InventoryExDialog : ��ų �ʱ�ȭ ������ �߰�.
			case ITEM_KIND_RESET_SKILL :
				{
					if( HERO->GetState() != eObjectState_None )
					{
						WINDOWMGR->MsgBox( MBI_CANTUSE_MAPMOVESCROLL, MBT_OK, CHATMGR->GetChatMsg(789) ) ;
						return ;
					}
					
					// 080124 KTH -- ������ ����϶����� ����� �� �����ϴ�.
					if( HERO->IsPKMode() )
					{
						WINDOWMGR->MsgBox( MBI_CANTUSE_MAPMOVESCROLL, MBT_OK, CHATMGR->GetChatMsg(1263) ) ;
						return;
					}

					// 080703 LUJ, �̵��� ��� ������ ���·� ��ų �ʱ�ȭ�� �� ���, �ʿ��� ��ų��
					//				���µ��� �̵��� ���°� �ȴ�. �� ������ �̵����� ��쿡�� ��ų �ʱ�ȭ��
					//				�� �� ����
					{
						const ITEMBASE* weaponItem = ITEMMGR->GetItemInfoAbsIn( HERO, TP_WEAR_START + eWearedItem_Weapon );
						const ITEMBASE* shieldItem = ITEMMGR->GetItemInfoAbsIn( HERO, TP_WEAR_START + eWearedItem_Shield );
						
						const ITEM_INFO* weaponItemInfo	= ( weaponItem ? ITEMMGR->GetItemInfo( weaponItem->wIconIdx ) : 0 );
						const ITEM_INFO* shieldItemInfo	= ( shieldItem ? ITEMMGR->GetItemInfo( shieldItem->wIconIdx ) : 0 );

						if( weaponItemInfo &&
							shieldItemInfo &&
							weaponItemInfo->WeaponType == shieldItemInfo->WeaponType )
						{
							// 080714 LUJ, ���ҽ��� �߸� ������Ʈ�Ǿ� �޽��� ��ȣ�� ����
							CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1590 ) );
							return;
						}
					}

					pItem->SetLock(TRUE) ;
					Set_QuickSelectedItem(pItem) ;
					WINDOWMGR->MsgBox( MBI_RESETSKILL_AREYOUSURE, MBT_YESNO, CHATMGR->GetChatMsg(1222) ) ;
					return ;
				}
				return ;

			// 080312 NYJ --- ��ü��ġ��(����Ȯ����) ������
			case ITEM_KIND_WORLDSHOUT:
				{
					ITEMMGR->UseItem_WorldShout(pItem);
				}
				return;

			case ITEM_KIND_FISHING_BAIT:
				{
					CFishingDialog*	window = GAMEIN->GetFishingDlg();

					if(	window && !FISHINGMGR->IsActive())
					{
						//  ���� â�� �������¸� ����â ����
						window->SetActive( TRUE );
						window->ChangeBaitItem(pItem);
					}
					else if( window && FISHINGMGR->IsActive())
					{
						// ���� â�� �������¸� �̳���� or ����
						window->ChangeBaitItem(pItem);
					}
				}
				return;

			case ITEM_KIND_FISHING_FISH:
				{
					CHATMGR->AddMsg( CTC_EXPENDEDITEM, CHATMGR->GetChatMsg(772));
					return;
				}
				return;

			case ITEM_KIND_RESET_REGIDENT_REGIST :
				{
					if( HERO->GetState() != eObjectState_None )
					{
						WINDOWMGR->MsgBox( MBI_CANTUSE_MAPMOVESCROLL, MBT_OK, CHATMGR->GetChatMsg(789) ) ;
						return ;
					}
					
					// 080124 KTH -- ������ ����϶����� ����� �� �����ϴ�.
					if( HERO->IsPKMode() )
					{
						WINDOWMGR->MsgBox( MBI_CANTUSE_MAPMOVESCROLL, MBT_OK, CHATMGR->GetChatMsg(1263) ) ;
						return;
					}

					pItem->SetLock(TRUE) ;
					Set_QuickSelectedItem(pItem) ;
					WINDOWMGR->MsgBox( MBI_RESET_REGIDENTREGIST, MBT_YESNO, CHATMGR->GetChatMsg(1244), pItem->GetItemInfo()->ItemName) ;
					return ;
				}
				return ;

			// 071201 LYW --- InventoryExDialog : �� �̵� �ֹ��� ó�� �߰�.
			case ITEM_KIND_MAP_MOVE_SCROLL :
				{
					if( HERO->GetState() != eObjectState_None || HERO->IsPKMode() )
					{
						WINDOWMGR->MsgBox( MBI_CANTUSE_MAPMOVESCROLL, MBT_OK, CHATMGR->GetChatMsg(789) ) ;
						return ;
					}

					ITEMMGR->UseItem_MapMoveScroll(pItem) ;
				}
				return ;

			// 071206 LYW --- InventoryExDialog : â�� Ȯ�� �ֹ��� ó�� �߰�.
			case ITEM_KIND_EXTEND_STORAGE :
				{
					BYTE byNum = 0 ;
					byNum = HERO->GetStorageNum() ;

					if( 2 > byNum || byNum >= MAX_STORAGELIST_NUM )
					{
						WINDOWMGR->MsgBox( MBI_RESETSKILL_AREYOUSURE, MBT_OK, CHATMGR->GetChatMsg(1243) ) ;
						return ;
					}

					ITEMMGR->UseItem_ExtendStorage(pItem) ;
				}
				return ;

			// 071210 LYW --- InventoryExDialog : �κ��丮 Ȯ�� �ֹ��� ó�� �߰�.
			case ITEM_KIND_EXTEND_INVEN :
				{
					// HERO�� �κ��丮 ������ ���� �´�.
					int nInvenCount = HERO->Get_HeroExtendedInvenCount() ;

					if( HERO->GetState() != eObjectState_None )
					{
						WINDOWMGR->MsgBox( MBI_CANTUSE_MAPMOVESCROLL, MBT_OK, CHATMGR->GetChatMsg(789) ) ;
						return ;
					}

					if( nInvenCount >= 2)
					{
						WINDOWMGR->MsgBox( NULL, MBT_OK, CHATMGR->GetChatMsg(1248) ) ;
						return ;
					}
					else
					{
						pItem->SetLock(TRUE) ;
						Set_QuickSelectedItem(pItem) ;

						WINDOWMGR->MsgBox( MBI_EXTENDINVEN_AREYOUSURE, MBT_YESNO, CHATMGR->GetChatMsg(1247) ) ;
						return ;
					}
				}
				return ;

			// 071228 LYW --- InventoryExDialog : �̸� ���� ������ ��� ó�� �߰�.
			case ITEM_KIND_CHANGE_NAME :
				{
					if( HERO->GetState() != eObjectState_None )
					{
						WINDOWMGR->MsgBox( NULL, MBT_OK, CHATMGR->GetChatMsg(789) ) ;
						return ;
					}

					if( HERO->GetPartyIdx() != 0 )
					{
						WINDOWMGR->MsgBox( NULL, MBT_OK, CHATMGR->GetChatMsg(1258) ) ;
						return ;
					}

					if( HERO->GetGuildIdx() != 0 )
					{
						WINDOWMGR->MsgBox( NULL, MBT_OK, CHATMGR->GetChatMsg(1257) ) ;
						return ;
					}

					CSHFamily* pFamily = NULL ;
					pFamily = HERO->GetFamily() ;

					if( pFamily )
					{
						if( pFamily->Get()->nMasterID != 0 )
						{
							WINDOWMGR->MsgBox( NULL, MBT_OK, CHATMGR->GetChatMsg(1259) ) ;
							return ;
						}
					}

					pItem->SetLock(TRUE) ;
					Set_QuickSelectedItem(pItem) ;

					cChangeNameDialog* pChangeNameDlg = NULL ;
					pChangeNameDlg = GAMEIN->GetChangeNameDlg() ;

					if( !pChangeNameDlg ) return ;

					pChangeNameDlg->Clear_NameBox() ;
					pChangeNameDlg->SetActive(TRUE) ;
				}
				return ;

				// ..���۹� �ɱ�
			case ITEM_KIND_FARM_PLANT:
				g_csFarmManager.CLI_RequestPlantCropInGarden(pItem->GetPosition(), pItem->GetItemIdx());
				return;
			// ..���۹� ��� �ֱ�
			case ITEM_KIND_FARM_MANURE:
				g_csFarmManager.CLI_RequestManureCropInGarden(pItem->GetPosition(), pItem->GetItemIdx());
				return;
			// ..���۹� ��Ȯ
			case ITEM_KIND_FARM_HARVEST:
				g_csFarmManager.CLI_RequestHarvestropInGarden(pItem->GetPosition(), pItem->GetItemIdx());
				return;
			// 080421 KTH 
			case ITEM_KIND_ANIMAL:
				//if( g_csFarmManager.CLI_GetTargetObj()->GetInfo()->nKind == CSHFarmManager::RENDER_BASIC_ANIMAL_CAGE_SLOT_KIND )
				{
					pItem->SetLock(TRUE) ;
					Set_QuickSelectedItem(pItem) ;
					WINDOWMGR->MsgBox( MBI_FARM_ANIMAL_INSTALL, MBT_YESNO, CHATMGR->GetChatMsg( 1571 ) );
					//g_csFarmManager.CLI_RequestInstallAnimalInAnimalCage(pItem->GetPosition(), pItem->GetItemIdx());
				}
				return;
			case ITEM_KIND_GET_SKILLPOINT :
				{
					ITEMMGR->GetSkillPointByItem(pItem) ;
				}
				return ;

			// 080607 LYW --- InventoryExDialog : â�� ��ȯ ������ ��� �߰�.
			case ITEM_KIND_OPENSTORAGE_BYITEM : 
				{
					ITEMMGR->OpenStorageByItem(pItem) ;
				}
				return ;


			// 080415 KTH ---
			// ..���� ��� �ֱ�
			case ITEM_KIND_ANIMAL_FOOD:
				g_csFarmManager.CLI_RequestFeedAnimalInCage(pItem->GetPosition(), pItem->GetItemIdx());
				return;
			//080515 KTH
			// ..���� û�� ������
			case ITEM_KIND_ANIMAL_CLEANER:
				g_csFarmManager.CLI_RequestCleaningAnimalInCage(pItem->GetPosition(), pItem->GetItemIdx());
				return;
			// ..��� û��
			/*case ITEM_KIND_FARM_CLEANNING:
				return;*/
			//---------------

			}
			// E ����ý��� �߰� added by hseos 2007.05.14	2007.08.21

			{
				mMessage.Category = MP_ITEM;
				mMessage.Protocol = MP_ITEM_USE_SYN;
				mMessage.dwObjectID = hero->GetID();
				mMessage.TargetPos = pItem->GetPosition();
				mMessage.wItemIdx = pItem->GetItemIdx();
			
				if( ITEMMGR->IsChangeItem( pItem->GetItemIdx() ) )
				{
					SetDisable( TRUE );

					// 071213 KTH -- ������ 5�ʿ��� 2�ʷ� �ٲ�.
					//const DWORD changeSecond = 2000;

					//SetCoolTime( pItem->GetItemIdx(), changeSecond );
					SetCoolTime( pItem->GetItemIdx(), WAITMILISECOND );

					{
						cQuickSlotDlg* dialog = ( cQuickSlotDlg* )WINDOWMGR->GetWindowForID( QI1_QUICKSLOTDLG );
						ASSERT( dialog );

						//dialog->SetCoolTime( pItem->GetItemIdx(), changeSecond );
						dialog->SetCoolTime( pItem->GetItemIdx(), WAITMILISECOND );

						// 080706 LYW --- InventoryExDialog : Ȯ�� �� ���� �߰� ó��.
						cQuickSlotDlg* dialog2 = NULL ;
						dialog2 = ( cQuickSlotDlg* )WINDOWMGR->GetWindowForID( QI2_QUICKSLOTDLG ) ;
						ASSERT(dialog2) ;

						if(dialog2)
						{
							dialog2->SetCoolTime( pItem->GetItemIdx(), WAITMILISECOND ) ;
						}
					}

					{
						CProgressDialog* dialog = ( CProgressDialog* )WINDOWMGR->GetWindowForID( PROGRESS_DIALOG );
						ASSERT( dialog );

						dialog->Wait( CProgressDialog::eActionChange );
					}
				}
				else if( pItem->GetItemInfo()->SupplyType == ITEM_KIND_PET )
				{
					if( HEROPET && pItem->GetDBIdx() != HEROPET->GetItemDBIdx() )
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1598) );
						return;
					}

					if( PETMGR->GetLastSummonTime() != 0 && gCurTime - PETMGR->GetLastSummonTime() < PETMGR->GetSummonPenaltyDelay() )
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1597) );
						return;
					}

					if( HERO->GetObjectBattleState() == eObjectBattleState_Battle )
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1599) );
						return;
					}

					PET_OBJECT_INFO* pObjectInfo = PETMGR->GetPetObjectInfo( pItem->GetDBIdx() );

					if( !pObjectInfo || pObjectInfo->State == ePetState_Die )
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(789) );
						return;
					}
			
					if( !HEROPET && HERO->GetLevel() < PETMGR->GetSummonPlayerLevel( pObjectInfo->Grade ) )
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1600) );
						return;
					}

					SetDisable( TRUE );

					// 071213 KTH -- ������ 5�ʿ��� 2�ʷ� �ٲ�.
					//const DWORD changeSecond = 2000;

					//SetCoolTime( pItem->GetItemIdx(), changeSecond );
					SetCoolTime( pItem->GetItemIdx(), WAITMILISECOND );

					{
						cQuickSlotDlg* dialog = ( cQuickSlotDlg* )WINDOWMGR->GetWindowForID( QI1_QUICKSLOTDLG );
						ASSERT( dialog );

						//dialog->SetCoolTime( pItem->GetItemIdx(), changeSecond );
						dialog->SetCoolTime( pItem->GetItemIdx(), WAITMILISECOND );
					}

					{
						CProgressDialog* dialog = ( CProgressDialog* )WINDOWMGR->GetWindowForID( PROGRESS_DIALOG );
						ASSERT( dialog );

						if( HEROPET )
						{
							dialog->SetText( RESRCMGR->GetMsg( 1007 ) );
						}
						else
						{
							dialog->SetText( RESRCMGR->GetMsg( 1006 ) );
						}
						dialog->Wait( CProgressDialog::eActionPetSummon );
					}
				}
				else if( pItem->GetItemInfo()->SupplyType == ITEM_KIND_PET_EVOLUTION_PYHSIC_TYPE ||
						 pItem->GetItemInfo()->SupplyType == ITEM_KIND_PET_EVOLUTION_MAGIC_TYPE ||
						 pItem->GetItemInfo()->SupplyType == ITEM_KIND_PET_EVOLUTION_SUPPORT_TYPE )
				{
					SetDisable( TRUE );

					WINDOWMGR->MsgBox( MBI_PETGRADEUP, MBT_YESNO, CHATMGR->GetChatMsg(1585) ) ;
				}
				else
				{
					NETWORK->Send( &mMessage, sizeof( mMessage ) );
				}
			}
		}
		break;
	default:
		{
		}
		break;
	}

/// �����ڵ�
/*
	eITEM_KINDBIT bits = pItem->GetItemKind();
	if(bits & eYOUNGYAK_ITEM)
	{
		// if eventmap
		if( GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter )
			return;

		if( bits == eYOUNGYAK_ITEM_PET )
		{
			//if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
			//	return;		

			//�� ��ȯ üũ
			//if( HEROPET == NULL )
			if( PETMGR->GetCurSummonPet() == NULL )
			{
				// 070126 LYW --- Inventory : Modified message number.
				//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1263));
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(344));
				return;
			}
			//�� ���¹̳� Ǯ üũ	//�Һ񰡴�
// 			if( PETMGR->IsCurPetStaminaFull() )
// 			{
// 				return;
// 			}
		}
		//�����۸��� ��ȹ����	06/02/22
		else if( bits == eYOUNGYAK_ITEM_UPGRADE_PET )
		{
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
				return;

			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );

			GAMEIN->GetPetUpgradeDialog()->SetUpgradeItemInfo(pItem->GetPosition(),pItem->GetItemIdx());
			
			BOOL Grade = FALSE;

			if( pItem->GetItemIdx() == 36001 )	Grade = ePUDG_Grade2;
			else if( pItem->GetItemIdx() == 36002 )	Grade = ePUDG_Grade3;
			GAMEIN->GetPetUpgradeDialog()->SetActive(Grade);
			return;
		}

		else if( bits == eYOUNGYAK_ITEM_REVIVAL_PET )
		{
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
				return;

			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );

			GAMEIN->GetPetRevivalDialog()->SetShopItemInfo(pItem->GetItemIdx(),pItem->GetPosition());

			BOOL Grade = FALSE;

			if( pItem->GetItemIdx() == 36003 )	Grade = ePRDG_Grade1;
			else if( pItem->GetItemIdx() == 36004 )	Grade = ePRDG_Grade2;
			else if( pItem->GetItemIdx() == 36005 )	Grade = ePRDG_Grade3;

			GAMEIN->GetPetRevivalDialog()->SetActiveWithGradeTxt(Grade);
			return;
		}

		if(pItem->GetItemIdx() == 10700)
		{
		}

		// �������� ���
		MSG_ITEM_USE_SYN msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_USE_SYN;
		msg.dwObjectID = HEROID;
		msg.TargetPos = pItem->GetPosition();
		msg.wItemIdx = pItem->GetItemIdx();
		/*
		msg.wTableIdx = eItemTable_Inventory;
		msg.wIconIdx = pItem->GetItemIdx();
		msg.wAbsPosition = pItem->GetPosition();
		*/
/*
		NETWORK->Send(&msg,sizeof(msg));

#ifdef TAIWAN_LOCAL
		if( pItem->GetItemIdx() == 53074 )	//�߱� �̺�Ʈ �ӽ�!! event temp
		{
			CHATMGR->AddMsg( CTC_CHEAT_1, "8888888888888888888888888888888888888888888888888888888888" );
			CHATMGR->AddMsg( CTC_CHEAT_1, "8 \" ī��Ԫ�����ر���ף�                                 8" );
			CHATMGR->AddMsg( CTC_CHEAT_1, "8 ��������ף���ͼ���������Զ����ԲԲ�Ϻ�����! \"          8" );
			CHATMGR->AddMsg( CTC_CHEAT_1, "8888888888888888888888888888888888888888888888888888888888" );
		}
#endif
	}
	else if(bits & eCHANGE_ITEM)	// change item
	{
		if( bits == eCHANGE_ITEM_LOCK )
		{
			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( pItem->GetItemIdx() );
			if( !pInfo )	return;
			if( HERO->GetLevel() < pInfo->ItemType )	return;
		}
		MSG_ITEM_USE_SYN msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_USE_CHANGEITEM_SYN;
		msg.dwObjectID = HEROID;
		msg.TargetPos = pItem->GetPosition();
		msg.wItemIdx = pItem->GetItemIdx();
	
		NETWORK->Send(&msg,sizeof(msg));
	}
	else if(bits & eEQUIP_ITEM)
	{
		if( OBJECTSTATEMGR->CheckAvailableState(HERO,eCAS_EQUIPITEM) == FALSE )
		{
			CAction act;
			act.InitAction(eActionKind_EquipItem,(void*)pItem->GetDBIdx(),0,NULL);
			HERO->SetNextAction(&act);
			return;
		}

		// ���� ������	
		switch( GetTableIdxPosition( pItem->GetPosition() ) )
		{
		case eItemTable_Inventory:
			{
				if(ITEMMGR->CanEquip(pItem->GetItemIdx()))
				{
					ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
					if(!pInfo) return ;
	/*					
					if( pInfo->LimitJob )
					{
						if( pInfo->LimitJob != HERO->GetCharacterTotalInfo()->Job[0] )
						{
							return;
						}
					}
	*/				
	/*				switch( pInfo->ItemKind )
					{
					case eEQUIP_ITEM_TWOHANDED:
					case eEQUIP_ITEM_BOW:
					case eEQUIP_ITEM_STAFF:
						{
							CItem* pShield = GetItemForPos( TP_WEAR_START + eWearedItem_Shield );

							if( pShield )
								UseItem( pShield );
						}
						break;
					case eEQUIP_ITEM_SHIELD:
						{
							CItem* pWeapon = GetItemForPos( TP_WEAR_START + eWearedItem_Weapon );

							if( pWeapon )
							{
								switch( pWeapon->GetItemInfo()->ItemKind )
								{
								case eEQUIP_ITEM_TWOHANDED:
								case eEQUIP_ITEM_BOW:
								case eEQUIP_ITEM_STAFF:
									{
										return;
									}
									break;
								}
							}
						}
						break;
					}
*/
/*					// ����2ó��
					WORD EquipToPos = pInfo->EquipKind;
					if(pInfo->EquipKind == eWearedItem_Earring1)
					{
//						if(IsExist(EquipToPos+WEAR_STARTPOSITION))
						if(IsExist(EquipToPos+TP_WEAR_START))
						{
//							if(!IsExist(EquipToPos+1+WEAR_STARTPOSITION))
							if(!IsExist(EquipToPos+1+TP_WEAR_START))
								EquipToPos++;
						}
					}
					CItem * pToItem = GetItemForPos( EquipToPos + TP_WEAR_START );
					FakeGeneralItemMove(EquipToPos + TP_WEAR_START, pItem, pToItem);
					/*
					MSG_ITEM_MOVE_SYN msg;
					msg.Category			= MP_ITEM;
					msg.Protocol			= MP_ITEM_MOVE_SYN;
					msg.dwObjectID			= HEROID;

					msg.FromPos				= pItem->GetPosition();
					msg.wFromItemIdx		= pItem->GetItemIdx();
					msg.ToPos				= EquipToPos + TP_WEAR_START;
					msg.wToItemIdx			= 0;

					if(msg.FromPos == msg.ToPos)
						return;
					NETWORK->Send(&msg,sizeof(msg));
					*/
/*				}
			}
			break;
		case eItemTable_Weared:
			{
				POSTYPE emptyPos;
				if( GetBlankPositionRestrictRef( emptyPos ) )
				{
					CItem * pToItem = GetItemForPos( emptyPos );
					FakeGeneralItemMove(emptyPos, pItem, pToItem);
					/*
					
					MSG_ITEM_MOVE_SYN msg;
					msg.Category			= MP_ITEM;
					msg.Protocol			= MP_ITEM_MOVE_SYN;
					msg.dwObjectID			= HEROID;

					msg.FromPos				= pItem->GetPosition();
					msg.wFromItemIdx		= pItem->GetItemIdx();
					msg.ToPos				= emptyPos;
					msg.wToItemIdx			= 0;

					if(msg.FromPos == msg.ToPos)
						return;

					NETWORK->Send(&msg,sizeof(msg));*/
/*					
				}
			}
			break;
		}
		
	}
	else if(bits & eSKILL_ITEM)
	{
		//if(!GAMEIN->GetSkillDialog()->CanBeMoved())
		//	return;
/*		if(pItem->GetItemIdx() == 10750)
		{
			GAMEIN->GetSkillOptionClearDlg()->SetItem(pItem);
			GAMEIN->GetSkillOptionClearDlg()->SetActive(TRUE);
			return;
		}
		if(SKILLTREEMGR->GetSkillBySkillIdx(pItem->GetItemInfo()->SkillNum) != NULL)
				return ;

		SKILL_CHANGE_INFO * pSkillChangeInfo = SKILLMGR->GetSkillChangeInfo(pItem->GetItemInfo()->SkillNum);
		if(pSkillChangeInfo)
		{
			if(SKILLTREEMGR->GetSkillBySkillIdx(pSkillChangeInfo->wTargetSkillIdx))
				return;
		}
		
		if(bits == eSKILL_ITEM_JINBUB)
		{
			if(!ITEMMGR->CanConvertToSkill(pItem->GetItemIdx(),SKILLTYPE_JINBUB))
				return;
		}
		else if(bits == eSKILL_ITEM_KYUNGGONG)		// ����� ��츸 ����޴������� ó���Ѵ�.
		{
			//if(!ITEMMGR->CanConvertToSkill(pItem->GetItemIdx(),SKILLTYPE_KYUNGGONG))
			//	return;
			if(KYUNGGONGMGR->IsSetableKyungGong(HERO,pItem) == FALSE)
				return;
		}
		else
		{
			// ���� ���
			if(!ITEMMGR->CanConvertToSkill(pItem->GetItemIdx(),SKILLTYPE_NORMAL))
				return;				
		}

		MSG_ITEM_USE_SYN msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_USE_SYN;
		msg.dwObjectID = HEROID;
		msg.wItemIdx = pItem->GetItemIdx();
		msg.TargetPos = pItem->GetPosition();

		NETWORK->Send(&msg,sizeof(msg));
*//*	}
	else if(bits & eEXTRA_ITEM)
	{
		/*
		//���� : ������ ��޾��� ���� ��޼� ���
		if(m_iInventoryState == eITEMUPGRADE_STATE)
		{
//			GAMEIN->GetChatDialog()->AddChatMsg(WHOLE,RGBA_MAKE(255,0,0,255), "���� ��޼��� ������Դϴ�.");
			CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(171) );
			return;
		}
		
		if(bits == eEXTRA_ITEM_UPGRADE || bits == eEXTRA_ITEM_UPGRADE+1)
		{		
			MSG_ITEM_USE_SYN msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_USE_SYN;
			msg.dwObjectID = HEROID;
			msg.wItemIdx = pItem->GetItemIdx();
			msg.TargetPos = pItem->GetPosition();
		
			NETWORK->Send(&msg,sizeof(msg));
		}*/
/*
		if(bits & eEXTRA_ITEM_USABLE)
		{	
		*/
/*		if(pItem->GetItemIdx() == 50510)
		{
			GAMEIN->GetGuildNoteDlg()->Show(pItem);
		}

		if(pItem->GetItemIdx() == 50511)
		{
			GAMEIN->GetUnionNoteDlg()->Show(pItem);
		}

		/*
			MSG_ITEM_USE_SYN msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_USE_SYN;
			msg.dwObjectID = HEROID;
			msg.wItemIdx = pItem->GetItemIdx();
			msg.TargetPos = pItem->GetPosition();

			NETWORK->Send(&msg,sizeof(msg));
		}
*/
/*	}
	else if(bits & eEXTRA_ITEM_UPGRADE)
	{

	}
	else if(bits & eQUEST_ITEM)
	{
		if( bits == eQUEST_ITEM_START )			// ���� ����Ʈ ������.
		{
			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( pItem->GetItemIdx() );
			if( !pInfo )
				return;
			if( !QUESTMGR->IsQuestStarted( pInfo->ItemType ) )
				// 070126 LYW --- Inventory : Modified message number.
				//m_pQuestMsgBox = WINDOWMGR->MsgBox(MBI_QUEST_NONE, MBT_OK, CHATMGR->GetChatMsg(651));
				m_pQuestMsgBox = WINDOWMGR->MsgBox(MBI_QUEST_NONE, MBT_OK, CHATMGR->GetChatMsg(345));
			else
				// 070126 LYW --- Inventory : Modified message number.
				//m_pQuestMsgBox = WINDOWMGR->MsgBox(MBI_QUESTSTART, MBT_YESNO, CHATMGR->GetChatMsg(650));
				m_pQuestMsgBox = WINDOWMGR->MsgBox(MBI_QUESTSTART, MBT_YESNO, CHATMGR->GetChatMsg(346));

			m_pTempQuestItem = pItem;
		}
		else if( bits == eQUEST_ITEM_PET )	//����Ʈ���� ��� �� ������.
		{
			if(2 == HERO->GetBattleTeam())
			{
				// 070126 LYW --- Inventory : Modified message number.
				//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1279) );
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(364) );
				return;
			}

			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( pItem->GetItemIdx() );
			if( !pInfo )
				return;

			//���ȯ ��ȿ�ð� üũ
			if( FALSE == PETMGR->CheckResummonAvailable() )
			{
				//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1254) );
				// 070126 LYW --- Inventory : Modified message number.
				//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1254), DWORD(PETMGR->GetPetResummonRestTime()/1000) );
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(348), DWORD(PETMGR->GetPetResummonRestTime()/1000) );
				return;
			}

			//�� ��ȯ�� �ٸ��� ��ȯ �Ұ�
			if( PETMGR->GetCurSummonPet() && FALSE == PETMGR->IsCurPetSummonItem(pItem->GetDBIdx()) )
			{
				// 070126 LYW --- Inventory : Modified message number.
				//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1255) );
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(349) );
				return;
			}

			//����ִ� �� ���� üũ
			if( FALSE == PETMGR->CheckPetAlive(pItem->GetDBIdx()) )
			{
				// 070126 LYW --- Inventory : Modified message number.
				//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1249));
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(350));
				return;
			}
			//���(��ȯ)���� ���� üũ
			if(pInfo->LimitLevel > HERO->GetMaxLevel())
			{
				// 070126 LYW --- Inventory : Modified message number.
				//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1242));
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(351));
				return;
			}

			//ĳ���� �̵� ����
			MOVEMGR->HeroMoveStop();

			MSG_ITEM_USE_SYN msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_USE_SYN;
			msg.dwObjectID = HEROID;
			msg.wItemIdx = pItem->GetItemIdx();
			msg.TargetPos = pItem->GetPosition();

			NETWORK->Send(&msg,sizeof(msg));
		}
		else if( bits == eQUEST_ITEM_EQUIP )	// ���������� ����Ʈ ������.
		{
			if( OBJECTSTATEMGR->CheckAvailableState(HERO,eCAS_EQUIPITEM) == FALSE )
			{
				CAction act;
				act.InitAction(eActionKind_EquipItem,(void*)pItem->GetDBIdx(),0,NULL);
				HERO->SetNextAction(&act);
				return;
			}
			
			switch( GetTableIdxPosition( pItem->GetPosition() ) )
			{
			case eItemTable_Inventory:
				{
					if( ITEMMGR->CanEquip( pItem->GetItemIdx() ) )
					{
						ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
						if( !pInfo ) return;

						WORD EquipToPos = pInfo->EquipKind;
						CItem* pToItem = GetItemForPos( EquipToPos + TP_WEAR_START );
						FakeGeneralItemMove(EquipToPos + TP_WEAR_START, pItem, pToItem);
					}
				}	
				break;
			case eItemTable_Weared:
				{
					POSTYPE emptyPos;
					if( GetBlankPositionRestrictRef( emptyPos ) )
					{
						CItem * pToItem = GetItemForPos( emptyPos );
						FakeGeneralItemMove(emptyPos, pItem, pToItem);
					}
				}
				break;
			}
		}
	}
	else if(bits & eSHOP_ITEM)
	{
		if( pItem->GetItemParam() & ITEM_PARAM_SEAL )
		{
			// 070126 LYW --- Inventory : Modified message number.
			//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(751) );
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(352) );
			return;
		}

		// if eventmap
		if( GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter )
			return;

		ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
		if(!pInfo)		return;


		if( bits == eSHOP_ITEM_EQUIP )
		{
			switch( GetTableIdxPosition( pItem->GetPosition() ) )
			{
			case eItemTable_ShopInven:
				{
					if( ITEMMGR->CanEquip( pItem->GetItemIdx() ) )
					{
						ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
						if(!pInfo) return ;

						// ����2ó��
						WORD EquipToPos = pInfo->EquipKind;
						if(pInfo->EquipKind == eWearedItem_Earring1)
						{
							if(IsExist(EquipToPos+TP_WEAR_START))
							{
								if(!IsExist(EquipToPos+1+TP_WEAR_START))
									EquipToPos++;
							}
						}
						CItem * pToItem = GetItemForPos( EquipToPos + TP_WEAR_START );
						FakeGeneralItemMove(EquipToPos + TP_WEAR_START, pItem, pToItem);
						return;
					}
				}
				break;
			case eItemTable_Weared:
				{
					POSTYPE emptyPos;
					if( m_pItemShopInven->GetBlankPositionRestrictRef( emptyPos ) )
					{
						CItem * pToItem = GetItemForPos( emptyPos );
						FakeGeneralItemMove(emptyPos, pItem, pToItem);
						return;
					}
				}
				break;
			}
		}


		if( pInfo->ItemIdx == eIncantation_MemoryStone )
		{
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
				return;			

			DWORD Validsavenum = MAX_MOVEDATA_PERPAGE;
			if( ITEMMGR->GetUsedItemInfo( eIncantation_MemoryMoveExtend ) ||
				ITEMMGR->GetUsedItemInfo( eIncantation_MemoryMove2 ) )
				Validsavenum = MAX_MOVEDATA_PERPAGE*MAX_MOVEPOINT_PAGE;

			if( GAMEIN->GetMoveDialog()->GetSavedCount() >= Validsavenum )
			{
				// 070126 LYW --- Inventory : Modified message number.
				//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(758) );
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(353) );
				return;
			}

			if( MAP->GetMapNum() == BOSSMONSTER_MAP || MAP->GetMapNum() == QuestRoom ||
				MAP->GetMapNum() == Tournament || MAP->GetMapNum() == BOSSMONSTER_2NDMAP )
				return;

			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Storage);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);

			GAMEIN->GetPointSaveDialog()->SetItemToMapServer( pItem->GetItemIdx(), pItem->GetPosition() );
			GAMEIN->GetPointSaveDialog()->SetDialogStatus( TRUE );
			GAMEIN->GetPointSaveDialog()->SetActive( TRUE );

			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );
		}
		else if( pInfo->ItemIdx == eIncantation_TownMove || pInfo->ItemIdx == eIncantation_MemoryMove )
		{
			//if( HERO->GetState() != eObjectState_None )
			//SW050907
			if(HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
			{
				return;
			}
			
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Storage);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);

			GAMEIN->GetMoveDialog()->SetItemToMapServer( pItem->GetItemIdx(), pItem->GetPosition(), TRUE );
			
			if( pInfo->ItemIdx == eIncantation_TownMove )
			{
				GAMEIN->GetMoveDialog()->SetTownMoveView( TRUE );
				GAMEIN->GetMoveDialog()->SetActive( TRUE );
			}
			else
			{
				GAMEIN->GetMoveDialog()->SetTownMoveView( FALSE );

				MSGBASE msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_MPINFO;
				msg.dwObjectID = HERO->GetID();
				NETWORK->Send( &msg, sizeof(msg) );
			}

			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );
		}
		else if( pInfo->ItemIdx == eIncantation_OtherRevive )
		{
			//if( HERO->GetState() != eObjectState_None )
			//SW050907 ����
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
			{
				return;
			}


			CObject* pObject = OBJECTMGR->GetSelectedObject();
			if( pObject == NULL || pObject == HERO || pObject->GetObjectKind() != eObjectKind_Player )
			{
				// 070126 LYW --- Inventory : Modified message number.
				//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(763) );
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(354) );
				return;
			}

			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Storage);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);

			char buf[128] = { 0, };
			// 070126 LYW --- Inventory : Modified message number.
			//wsprintf( buf, CHATMGR->GetChatMsg(761), pObject->GetObjectName() );
			wsprintf( buf, CHATMGR->GetChatMsg(355), pObject->GetObjectName() );
			m_pSelectedShopItem = pItem;

			WINDOWMGR->MsgBox( MBI_OTHERREVIVE, MBT_YESNO, buf );
		}
		else if( pInfo->ItemIdx == eIncantation_ItemSeal )
		{
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
			{
				return;
			}

			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );

			if( GAMEIN->GetSealDlg() )
			{
				GAMEIN->GetSealDlg()->SetItemInfo( pItem->GetItemIdx(), pItem->GetPosition() );
				GAMEIN->GetSealDlg()->SetActive( TRUE );
			}
		}
		else if( pInfo->ItemIdx == eIncantation_ReinforceReset )
		{
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
			{
				return;
			}

			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );

			if( GAMEIN->GetReinforceResetDlg() )
			{
				GAMEIN->GetReinforceResetDlg()->SetItemInfo( pItem->GetItemIdx(), pItem->GetPosition() );
				GAMEIN->GetReinforceResetDlg()->SetActive( TRUE );
			}
		}		
		else if( pInfo->ItemKind == eSHOP_ITEM_MAKEUP || pInfo->ItemKind == eSHOP_ITEM_DECORATION )
		{
			AVATARITEM* pAvatarItem = GAMERESRCMNGR->m_AvatarEquipTable.GetData( pInfo->ItemIdx );
			if(!pAvatarItem)
				return;

			if( pAvatarItem->Gender < 2 && pAvatarItem->Gender != HERO->GetGender() )
			{
				// 070126 LYW --- Inventory : Modified message number.
				//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(778) );
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(357) );
				return;
			}			
			else
			{
				// �������� ���
				if( HERO->GetState() == eObjectState_Move )
					MOVEMGR->HeroMoveStop();
				//else if( HERO->GetState() != eObjectState_None )
				//SW050907 ����
				else if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
				{
					// 070126 LYW --- Inventory : Modified message number.
					//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(798) );
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(358) );
					return;
				}

				MSG_DWORD_WORD msg;
				msg.Category = MP_ITEM;
				if( pItem->GetUseParam() )
					msg.Protocol = MP_ITEM_SHOPITEM_AVATAR_TAKEOFF;
				else
					msg.Protocol = MP_ITEM_SHOPITEM_AVATAR_USE_SYN;
				msg.dwObjectID = HEROID;
				msg.dwData = pItem->GetItemIdx();
				msg.wData = pItem->GetPosition();				
				NETWORK->Send(&msg,sizeof(msg));
			}
		}
		else if( pInfo->ItemKind == eSHOP_ITEM_INCANTATION && pInfo->Life )
		{
			//if( HERO->GetState() != eObjectState_None )
			//SW050907 ����
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
			{
				return;
			}
			
			char buf[128];
			// 070126 LYW --- Inventory : Modified message number.
			//sprintf( buf, CHATMGR->GetChatMsg(737), pInfo->Life );
			sprintf( buf, CHATMGR->GetChatMsg(359), pInfo->Life );
			WINDOWMGR->MsgBox( MBI_SKPOINTREDIST_USE, MBT_YESNO, buf );			
		}
		else if( pInfo->ItemIdx == eIncantation_TownMove15 ||
			pInfo->ItemIdx == eIncantation_MemoryMove15 ||
			pInfo->ItemIdx == eIncantation_TownMove7 ||
			pInfo->ItemIdx == eIncantation_TownMove7_NoTrade ||
			pInfo->ItemIdx == eIncantation_MemoryMove7 ||
			pInfo->ItemIdx == eIncantation_MemoryMove7_NoTrade ||
			pInfo->ItemIdx == 55357 ||
			pInfo->ItemIdx == 55362 ||
			pInfo->ItemIdx == eIncantation_MemoryMoveExtend ||
			pInfo->ItemIdx == eIncantation_MemoryMove2 )
		{
//			if( HERO->GetState() != eObjectState_None )
//				return;
			//SW050907 ����
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
				return;
			
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Storage);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);
			
			GAMEIN->GetMoveDialog()->SetItemToMapServer( pItem->GetItemIdx(), pItem->GetPosition(), TRUE );
			
			if( pInfo->ItemIdx == eIncantation_TownMove15 ||
				pInfo->ItemIdx == eIncantation_TownMove7 || 
				pInfo->ItemIdx == eIncantation_TownMove7_NoTrade )
			{
				GAMEIN->GetMoveDialog()->SetTownMoveView( TRUE );
				GAMEIN->GetMoveDialog()->SetActive( TRUE );
			}
			else
			{
				GAMEIN->GetMoveDialog()->SetTownMoveView( FALSE );

				MSGBASE msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_MPINFO;
				msg.dwObjectID = HERO->GetID();
				NETWORK->Send( &msg, sizeof(msg) );
			}

			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );
		}
		else if( pInfo->ItemIdx == eIncantation_ShowStorage
				|| pInfo->ItemIdx == eIncantation_ShowStorage7
				|| pInfo->ItemIdx == eIncantation_ShowStorage7_NoTrade )
		{
			//if( HERO->GetState() != eObjectState_None )
			//SW050907 ����
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
				return;

			NPCSCRIPTMGR->StartNpcBusiness( 0, eNpcParam_ShowpStorage );
		}
		else if( pInfo->ItemIdx == eSundries_RareItemCreate50 ||
			pInfo->ItemIdx == eSundries_RareItemCreate70 ||
			pInfo->ItemIdx == eSundries_RareItemCreate90 )
		{
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
				return;

			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );

			GAMEIN->GetRareCreateDlg()->SetShopItemInfo( pItem->GetItemIdx(), pItem->GetPosition() );
			GAMEIN->GetRareCreateDlg()->SetActive( TRUE );
			//pItem->SetLock( TRUE );

		}
		else if( pInfo->ItemIdx == eIncantation_PetRevival )
		{//�� ��Ȱ �ֹ���
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
				return;

			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );

			GAMEIN->GetPetRevivalDialog()->SetShopItemInfo( pItem->GetItemIdx(), pItem->GetPosition() );
			GAMEIN->GetPetRevivalDialog()->SetActive( TRUE );
		}
		else if( pInfo->ItemIdx == eIncantation_PetGrade2
			|| pInfo->ItemIdx == eIncantation_PetGrade3 )
		{
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
				return;

			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );

			GAMEIN->GetPetUpgradeDialog()->SetUpgradeItemInfo(pItem->GetPosition(),pItem->GetItemIdx());

			BOOL Grade = FALSE;

			if( pItem->GetItemIdx() == eIncantation_PetGrade2 )	Grade = ePUDG_Grade2;
			else if( pItem->GetItemIdx() == eIncantation_PetGrade3 )	Grade = ePUDG_Grade3;
			GAMEIN->GetPetUpgradeDialog()->SetActive(Grade);
			return;
		}
		else if( pInfo->ItemKind == eSHOP_ITEM_SUNDRIES && pInfo->Str )
		{
			// ��ȭ��ȣ
			if( pItem->GetItemIdx() >= 55633 && pItem->GetItemIdx() <= 55635 )
			{
				GAMEIN->GetReinforceDialog()->Release();
				GAMEIN->GetReinforceDialog()->SetActiveRecursive( FALSE );
				GAMEIN->GetReinforceDialog()->SetShopItemInfo( pItem->GetItemIdx(), pItem->GetPosition() );
				GAMEIN->GetReinforceDialog()->SetActiveRecursive( TRUE );
				pItem->SetLock( TRUE );
			}
			// ���պ�ȣ
			if( pItem->GetItemIdx() >= 55636 && pItem->GetItemIdx() <= 55638 )
			{
				GAMEIN->GetMixDialog()->Close();
				GAMEIN->GetMixDialog()->SetActiveRecursive( FALSE );
				GAMEIN->GetMixDialog()->SetShopItemInfo( pItem->GetItemIdx(), pItem->GetPosition() );
				GAMEIN->GetMixDialog()->SetActiveRecursive( TRUE );
				pItem->SetLock( TRUE );
			}
			return;
		}
		else if( pInfo->ItemKind == eSHOP_ITEM_SUNDRIES && pInfo->Vit )
		{
			if(GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter || 
				GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)
				return;

			if( ( HERO->GetState() == eObjectState_None || HERO->GetState() == eObjectState_Immortal )
					&& !SHOWDOWNMGR->IsShowdown() )
			{
				STREETSTALLMGR->ChangeDialogState();
			}
		}
		else if( pInfo->ItemIdx == eIncantation_ChangeJob )
		{
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
				return;
//			if( HERO->GetStage() != eStage_Hwa && HERO->GetStage() != eStage_Geuk )
//				return;
		
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Storage);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);
			
			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );
			GAMEIN->GetChangeJobDlg()->SetActive( TRUE );
			GAMEIN->GetChangeJobDlg()->SetItemInfo( pItem->GetPosition(), pItem->GetDBIdx() );
		}
		else if( pInfo->ItemIdx == eIncantation_ChangeName || pInfo->ItemIdx == eIncantation_ChangeName_Dntrade )
		{				
			//if( HERO->IsPKMode() || SHOWDOWNMGR->IsShowdown() || PKMGR->IsPKLooted() || HERO->GetState() != eObjectState_None )
			//SW050907 ����
			if( HERO->IsPKMode() || SHOWDOWNMGR->IsShowdown() || PKMGR->IsPKLooted() || ( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal ) )
				return;

			GAMEIN->GetNameChangeNotifyDlg()->SetActive( TRUE );
			GAMEIN->GetInventoryDialog()->SetDisable( TRUE );
		}

		else if( pInfo->ItemIdx == eSundries_Shout || pInfo->ItemIdx == eSundries_Shout_Once 
			|| pInfo->ItemIdx == eSundries_Shout_Once_NoTrade )
		{
			GAMEIN->GetShoutDlg()->SetActive( TRUE );
			GAMEIN->GetShoutDlg()->SetItemInfo( pInfo->ItemIdx, pItem->GetPosition() );
			return;
		}

		else if( pInfo->ItemKind == eSHOP_ITEM_PET_EQUIP )
		{
			//���ȯ���� Ȯ���ϰ�
			if( PETMGR->GetCurSummonPet() == NULL )
			{
				// 070126 LYW --- Inventory : Modified message number.
				//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1263));
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(360));
				return;
			}
			//���κ� Ȱ��ȭ Ȯ���ϰ�
//			SW060424 ��ȹ����� ��û
//			if( FALSE == ((cWindow*)(GAMEIN->GetPetInventoryDialog()))->IsActive() )
//			{
//				return;
//			}
			//�� ���� ���� ��� ������
			CPetWearedExDialog* pPetEquipDlg = ((CPetInventoryDlg*)GAMEIN->GetPetInventoryDialog())->GetPetWearedDlg();

			//�̵������ش�.
			POSTYPE	EmptyPos = 0;
			if(pPetEquipDlg->GetBlankPositionRestrictRef(EmptyPos))
			{
				CItem* pToItem = GAMEIN->GetPetInventoryDialog()->GetItemForPos(EmptyPos);
				FakeGeneralItemMove(EmptyPos, pItem, pToItem);
				return;
			}
			else
			{
				//������������ Full �̶�� �޽���
				// 070126 LYW --- Inventory : Modified message number.
				//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1287) );
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(361) );
				return;
			}
		}
		else
		{
			if( pInfo->ItemKind == eSHOP_ITEM_CHARM && pInfo->EquipKind )
			{
				if( HERO->GetMaxLevel() < pInfo->EquipKind )
				{
					// 070126 LYW --- Inventory : Modified message number.
					//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(931) );
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(362) );
					return;
				}
			}
			else if( pInfo->ItemKind == eSHOP_ITEM_INCANTATION && pInfo->LimitJob )
			{
				// ���� ���� ����
				if( pInfo->LimitGender == 0 && HERO->GetLevel() > 50 )
				{
					// 070126 LYW --- Inventory : Modified message number.
					//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1082) );
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(363) );
					return;
				}
				else if( pInfo->LimitGender == 1 && (HERO->GetLevel() <51 || HERO->GetLevel() > 70 ) )
				{
					// 070126 LYW --- Inventory : Modified message number.
					//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1082) );
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(363) );
					return;
				}
			}

			if( pInfo->ItemKind == eSHOP_ITEM_PET )
			{
				if(2 == HERO->GetBattleTeam())
				{
					// 070126 LYW --- Inventory : Modified message number.
					//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1279) );
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(364) );
					return;
				}

				//���ȯ ��ȿ�ð� üũ
				if( FALSE == PETMGR->CheckResummonAvailable() )
				{
					//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1254) );
					// 070126 LYW --- Inventory : Modified message number.
					//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1254), DWORD(PETMGR->GetPetResummonRestTime()/1000) );
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(348), DWORD(PETMGR->GetPetResummonRestTime()/1000) );
					return;
				}

				//�� ��ȯ�� �ٸ��� ��ȯ �Ұ�
				if( PETMGR->GetCurSummonPet() && FALSE == PETMGR->IsCurPetSummonItem(pItem->GetDBIdx()) )
				{
					// 070126 LYW --- Inventory : Modified message number.
					//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1255) );
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(349) );
					return;
				}

				//����ִ� �� ���� üũ
				if( FALSE == PETMGR->CheckPetAlive(pItem->GetDBIdx()) )
				{
					// 070126 LYW --- Inventory : Modified message number.
					//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1249));
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(350));
					return;
				}
				//���(��ȯ)���� ���� üũ
				if(pInfo->EquipKind > HERO->GetMaxLevel())
				{
					return;
				}
				
				//ĳ���� �̵� ����
				MOVEMGR->HeroMoveStop();
			}

			// �������� ���
			MSG_SHOPITEM_USE_SYN msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_SHOPITEM_USE_SYN;
			msg.dwObjectID = HEROID;
			msg.UseBaseInfo.TargetItemPos = pItem->GetPosition();
			msg.UseBaseInfo.TargetItemIdx = pItem->GetItemIdx();
			msg.UseBaseInfo.ShopItemPos = pItem->GetPosition();
			msg.UseBaseInfo.ShopItemIdx = pItem->GetItemIdx();
			
			NETWORK->Send(&msg,sizeof(msg));
		}
	}
	else
	{
		ASSERT(0);
	}
*/
}

void CInventoryExDialog::SetActive(BOOL val)
{
	// 080221 LYW --- MacroManager : �κ��丮 Ȯ�忡 ���� ó��.
	int nExtendedInven = 0 ;
	nExtendedInven = HERO->Get_HeroExtendedInvenCount() ;

	ShowTab(2,FALSE) ;
	ShowTab(3,FALSE) ;

	switch(nExtendedInven)
	{
	case 1 : ShowTab(2,TRUE) ; break ;
	case 2 :
		{
			ShowTab(2,TRUE) ;
			ShowTab(3,TRUE) ;
		}
		break ;

	default : break ;
	}

	if( !m_bDisable )
	{
		if(val == FALSE)
		{
			CDealDialog * dDlg = GAMEIN->GetDealDialog();
			if(dDlg)
				if(dDlg->IsShow())
					dDlg->SetActive(FALSE);

//			CStorageDialog* pStorageDlg = GAMEIN->GetStorageDialog();
//			if(pStorageDlg->IsActive())
//				pStorageDlg->SetActive(FALSE);

			{				
				// send event to CGuildWarehouseDialog
				cDialog* dialog = WINDOWMGR->GetWindowForID( GDW_WAREHOUSEDLG );
				ASSERT( dialog );

				if( dialog->IsActive() )
				{
					dialog->SetActive( FALSE );
				}
			}
			
//			SetAbsX( GetAbsX() - 1 );	//�ӽ�.. ������ �������¿��� �����·� ���̰� �ϱ� ����
		}
		else
		{
//			SetAbsX( GetAbsX() + 1 );
		}

		cTabDialog::SetActive(val);





//--- main bar icon
//	CMainBarDialog* pDlg = GAMEIN->GetMainInterfaceDialog();
//	if( pDlg )
//		pDlg->SetPushBarIcon( OPT_INVENTORYDLGICON, m_bActive );

		if( m_bShopInven )
		{
			OnShopItemBtn();
		}
		else
		{
//			m_pItemShopBtn->SetPush(FALSE);
//			m_pItemShopInven->SetActive(FALSE);
		}
	}

	// 070423 LYW --- InventoryExDialog : Check background image.
	CheckBackground() ;

	/*GetTabBtn(0)->SetActive(FALSE);
	GetTabSheet(0)->SetActive(FALSE);
	GetTabBtn(1)->SetActive(FALSE);
	GetTabSheet(1)->SetActive(FALSE);*/

	// �κ� â�� ������ �Ʒ� â�鵵 ����.
	if( ! val )
	{
		cDialog* mixDialog			= WINDOWMGR->GetWindowForID( ITMD_MIXDLG );
		cDialog* enchantDialog		= WINDOWMGR->GetWindowForID( ITD_UPGRADEDLG );
		cDialog* reinforceDialog	= WINDOWMGR->GetWindowForID( ITR_REINFORCEDLG );
		cDialog* dissoloveDialog	= WINDOWMGR->GetWindowForID( DIS_DISSOLUTIONDLG );

		mixDialog->SetActive( FALSE );
		enchantDialog->SetActive( FALSE );
		reinforceDialog->SetActive( FALSE );
		dissoloveDialog->SetActive( FALSE );
	}

	// 071022 LYW --- InventoryExDialog : Check open dialog.
	TUTORIALMGR->Check_OpenDialog(this->GetID(), val) ;
}


void CInventoryExDialog::OnShopItemBtn()
{	
	for(int i=0; i<m_bTabNum; i++)
	{
		m_ppPushupTabBtn[i]->SetPush(FALSE);
		m_ppWindowTabSheet[i]->SetActive(FALSE);				
	}
	
	//m_pItemShopBtn->SetPush(TRUE);
	//m_pItemShopInven->SetActive(TRUE);
	//m_bShopInven = TRUE;

}

void CInventoryExDialog::OffShopItemBtn(LONG lId)
{
	//if(m_pItemShopBtn->IsPushed() == FALSE)
		return;

	//m_pItemShopBtn->SetPush(FALSE);
	//m_pItemShopInven->SetActive(FALSE);

	for(int i=0; i<m_bTabNum; i++)
	{
		if(lId == m_ppPushupTabBtn[i]->GetID())
			SelectTab(i);
	}

	//m_bShopInven = FALSE;
}





//---------------------------------------------------------------------------------------------------------------------------
//	071114 LYW --- IventoryExDialog : ������ Ŭ�� ó�� ����.
//---------------------------------------------------------------------------------------------------------------------------
void CInventoryExDialog::OnActionEvnet(LONG lId, void * p, DWORD we)
{
	if( !p ) return ;

	if(we == WE_LBTNDBLCLICK)
	{	
		CItem * pItem = NULL ;

		if(lId == IN_WEAREDDLG)
		{
			if( !m_pWearedDlg ) return ;

			WORD wIconIdx = 0 ;
			wIconIdx = (WORD)m_pWearedDlg->GetCurSelCellPos() ;

			pItem = (CItem*)m_pWearedDlg->GetIconForIdx( wIconIdx ) ;
		}
		else
		{
			cIconGridDialog * gridDlg = NULL ;
			gridDlg = (cIconGridDialog *)GetTabSheet(GetCurTabNum()) ;

			if( !gridDlg ) return ;

			pItem = (CItem*)gridDlg->GetIconForIdx((WORD)gridDlg->GetCurSelCellPos()) ;
		}

		if( !pItem ) return ;

		//if( pItem->GetItemParam() == eITEM_PARAM_SEAL )
		if( pItem->GetItemBaseInfo().nSealed == eITEM_TYPE_SEAL )
		{
			if( pItem->IsLocked() ) return ;

			WINDOWMGR->MsgBox(MBI_SHOPITEM_LOCK, MBT_YESNO, CHATMGR->GetChatMsg( 1178 )) ;

			return ;
		}
		else
		{
			UseItem(pItem) ;
		}
	}
	else if(we == WE_LBTNCLICK)
	{
		CDealDialog * pDealDlg = NULL ;
		pDealDlg = GAMEIN->GetDealDialog() ;

		if( !pDealDlg ) return ;

		if( !pDealDlg->IsShow() ) return ;

		if(m_iInventoryState == eITEMDEAL_STATE)
		{		
			CItem * pItemTT = NULL ;
			pItemTT = GetCurSelectedItem(INV) ;

			if( !pItemTT )
			{
				pDealDlg->SetMoney(0, 2) ;
				return ;
			}
			
			int itemIDX = pItemTT->GetItemIdx() ;

			ITEM_INFO * pItem = NULL ;
			pItem = ITEMMGR->GetItemInfo(itemIDX) ;

			ASSERT(pItem) ;

			DWORD SellPrice = 0 ;
			SellPrice = SWPROFIT->CalTexRateForSell( pItem->SellPrice ) ;

			pDealDlg->SetMoney(SellPrice, 0) ;
		}
	}
	else if(we == WE_RBTNCLICK)
	{
		CDealDialog* pDlg = NULL ;
		pDlg = GAMEIN->GetDealDialog() ;

		if( !pDlg ) return ;

		if( !pDlg->IsActive() ) return ;

		cIconGridDialog * gridDlg = NULL ;
		gridDlg = (cIconGridDialog *)GetTabSheet(GetCurTabNum()) ;

		if( !gridDlg ) return ;

		WORD wIconIdx = 0 ;
		wIconIdx = (WORD)gridDlg->GetCurSelCellPos() ;

		CItem * pItem = NULL;
		pItem = (CItem*)gridDlg->GetIconForIdx(wIconIdx) ;

		pDlg->SellItem(pItem) ;
	}
}


//void CInventoryExDialog::OnActionEvnet(LONG lId, void * p, DWORD we)
//{
//	if( !p ) return ;
//
//	if(we == WE_LBTNDBLCLICK)
//	{	
//		CItem * pItem = NULL ;
//
//		if(lId == IN_WEAREDDLG)
//		{
//			if( !m_pWearedDlg ) return ;
//
//			WORD wIconIdx = 0 ;
//			wIconIdx = (WORD)m_pWearedDlg->GetCurSelCellPos() ;
//
//			pItem = (CItem*)m_pWearedDlg->GetIconForIdx( wIconIdx ) ;
//		}
//		else
//		{
//			cIconGridDialog * gridDlg = NULL ;
//			gridDlg = (cIconGridDialog *)GetTabSheet(GetCurTabNum()) ;
//
//			if( !gridDlg ) return ;
//
//			pItem = (CItem*)gridDlg->GetIconForIdx((WORD)gridDlg->GetCurSelCellPos()) ;
//		}
//
//		if( !pItem ) return ;
//
//		if( pItem->GetItemParam() & ITEM_PARAM_SEAL )
//		{
//			if( pItem->IsLocked() ) return ;
//
//			WINDOWMGR->MsgBox(MBI_SHOPITEM_LOCK, MBT_YESNO, "�������� ������ �����Ͻðڽ��ϱ�?") ;
//
//			return ;
//		}
//		else
//		{
//			UseItem(pItem) ;
//		}
//	}
//	else if(we == WE_LBTNCLICK)
//	{
//		CDealDialog * pDealDlg = NULL ;
//		pDealDlg = GAMEIN->GetDealDialog() ;
//
//		if( !pDealDlg ) return ;
//
//		if( !pDealDlg->IsShow() ) return ;
//
//		if(m_iInventoryState == eITEMDEAL_STATE)
//		{		
//			CItem * pItemTT = NULL ;
//			pItemTT = GetCurSelectedItem(INV) ;
//
//			if( !pItemTT )
//			{
//				pDealDlg->SetMoney(0, 2) ;
//				return ;
//			}
//			
//			int itemIDX = pItemTT->GetItemIdx() ;
//
//			ITEM_INFO * pItem = NULL ;
//			pItem = ITEMMGR->GetItemInfo(itemIDX) ;
//
//			ASSERT(pItem) ;
//
//			DWORD SellPrice = 0 ;
//			SellPrice = SWPROFIT->CalTexRateForSell( pItem->SellPrice ) ;
//
//			pDealDlg->SetMoney(SellPrice, 0) ;
//		}
//	}
//	else if(we == WE_RBTNCLICK)
//	{
//		CDealDialog* pDlg = NULL ;
//		pDlg = GAMEIN->GetDealDialog() ;
//
//		if( !pDlg ) return ;
//
//		if( !pDlg->IsActive() ) return ;
//
//		cIconGridDialog * gridDlg = NULL ;
//		gridDlg = (cIconGridDialog *)GetTabSheet(GetCurTabNum()) ;
//
//		if( !gridDlg ) return ;
//
//		WORD wIconIdx = 0 ;
//		wIconIdx = (WORD)gridDlg->GetCurSelCellPos() ;
//
//		CItem * pItem = NULL;
//		pItem = (CItem*)gridDlg->GetIconForIdx(wIconIdx) ;
//
//		pDlg->SellItem(pItem) ;
//	}
//}

CItem * CInventoryExDialog::GetCurSelectedItem(BYTE type)
{
	switch(type)
	{
	case INV:
		{
			// INVENTORY
			cIconGridDialog * gridDlg = (cIconGridDialog *)GetTabSheet(GetCurTabNum());
			if(gridDlg->GetCurSelCellPos() == -1) 
				return NULL;
			return (CItem *)gridDlg->GetIconForIdx((WORD)gridDlg->GetCurSelCellPos());
		}
		break;
	case WEAR:
		{
			// WEARED
			if(m_pWearedDlg->GetCurSelCellPos() == -1) 
				return NULL;
			return (CItem *)m_pWearedDlg->GetIconForIdx((WORD)m_pWearedDlg->GetCurSelCellPos());
		}
		break;
	case SHOPIVN:
		{
			return m_pItemShopInven->GetItemForPos((POSTYPE)(m_pItemShopInven->GetCurSelCellPos()+TP_SHOPINVEN_START));
		}
		break;
	default:
		{
			ASSERT(0);
		}
	}
	return NULL;
}

BOOL CInventoryExDialog::GetBlankPositionRestrictRef(WORD & absPos)
{
	for(int i = 0 ; i < GetTabNum() ; ++i)
	{
		cIconGridDialog * gridDlg = (cIconGridDialog *)GetTabSheet(i);
		for(int j = 0 ; j < gridDlg->GetCellNum() ; ++j)
		{
			if(gridDlg->IsAddable(j))
			{
//				absPos = INVENTORY_STARTPOSITION+i*CELLNUMPERPAGE+j;
				absPos = TP_INVENTORY_START+i*TABCELL_INVENTORY_NUM+j;
				return TRUE;
			}
		}
		
	}
	return FALSE;
}

int CInventoryExDialog::GetBlankNum()
{
	int nBlankNum = 0;
	BYTE TabNum = GetTabNum();
	for( BYTE i = 0 ; i < TabNum ; ++i )
	{
		cIconGridDialog* gridDlg = (cIconGridDialog*)GetTabSheet(i);

		for( WORD j = 0 ; j < gridDlg->GetCellNum() ; ++j )
		{
			if( gridDlg->IsAddable(j) )
				++nBlankNum;
		}
	}

	return nBlankNum;
}

BOOL CInventoryExDialog::FakeMoveIcon(LONG x, LONG y, cIcon * icon)
{
	ASSERT(icon);
	if( m_bDisable )	return FALSE;

	if( PKMGR->IsPKLooted() )	return FALSE;	//�׾����� �ٸ�����.. //KES 040801

	if( icon->GetType() == WT_STALLITEM || icon->GetType() == WT_EXCHANGEITEM ) return FALSE;
	
	if(icon->GetType() == WT_ITEM)
	{
		return FakeMoveItem(x, y, (CItem *)icon);
	}
	else if(icon->GetType() == WT_DEALITEM)
	{
		FakeBuyItem(x, y, (CDealItem *)icon);
		AUDIOMGR->Play(58, HERO);
	}
	return FALSE;
}

void CInventoryExDialog::FakeBuyItem( LONG mouseX, LONG mouseY, CDealItem * pFromItem )
{
//	if( pFromItem )
//		GAMEIN->GetDealDialog()->FakeBuyItem( pFromItem->GetAbsX(), pFromItem->GetAbsY(), pFromItem->GetItemIdx() );
	if( pFromItem )
		GAMEIN->GetDealDialog()->FakeBuyItem( (LONG)pFromItem->GetAbsX(), (LONG)pFromItem->GetAbsY(), pFromItem );
}

// ����â ���� �̵�
void CInventoryExDialog::MoveItem(POSTYPE FromPos, POSTYPE ToPos )
{
	CItem * FromItem = NULL;
	CItem * ToItem = NULL;

	DeleteItem(FromPos, &FromItem);
	DeleteItem(ToPos, &ToItem);

	if(FromItem)
	{
		FromItem->SetPosition(ToPos);
		if(!AddItem(FromItem))
			assert(0);
	}

	if(ToItem)
	{
		ToItem->SetPosition(FromPos);
		if(!AddItem(ToItem))
			assert(0);
	}
}

BOOL CInventoryExDialog::FakeMoveItem( LONG mouseX, LONG mouseY, CItem * pFromItem )
{
	WORD ToPos=0;
	
	if( !GetPositionForXYRef( 0, mouseX, mouseY, ToPos ) )	// ������ġ �Ѿ� ��
		return FALSE;

	CItem * pToItem = GetItemForPos( ToPos );
	
	//sound
	if(ITEMMGR->GetTableIdxForAbsPos(ToPos)==eItemTable_Weared)
		AUDIOMGR->Play(59, HERO);
	else
		AUDIOMGR->Play(58, HERO);

//KES EXCHANGE 031002
	if( pToItem )
	if( pToItem->IsLocked() ) return FALSE;
	
	if( pFromItem->IsLocked() ) return FALSE;

	if( pFromItem->GetParent() == ( cObject* )GAMEIN->GetPetWearedDlg() )
	{
		if( pToItem )
		{
			RECT rt;
			GAMEIN->GetPetWearedDlg()->GetIconCellRect( pFromItem->GetPosition(), rt );
			GAMEIN->GetPetWearedDlg()->FakeMoveIcon( rt.left, rt.top, pToItem );
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
	
		return FALSE;
	}
	
	//---KES Item Combine Fix 071021
	if(TP_STORAGE_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_STORAGE_END)
	{
		if( ITEMMGR->IsDupItem( pFromItem->GetItemIdx() ) )
		{
			if( pToItem )
			if( pToItem->GetItemIdx() == pFromItem->GetItemIdx() )
			{
				return FakeItemCombine( ToPos, pFromItem, pToItem );
			}
		}

		if( ToPos == ( TP_WEAR_START + eEquipSlot_Shield - 1 ) )
		{
			switch( HERO->GetWeaponAniType() )
			{
			case eWeaponAnimationType_TwoHand:
			case eWeaponAnimationType_Staff:
			case eWeaponAnimationType_Bow:
			case eWeaponAnimationType_Gun:
			case eWeaponAnimationType_TwoBlade:
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(772) );
					return FALSE;
				}
				break;
			}
		}

		return FakeGeneralItemMove( ToPos, pFromItem, pToItem );
	}
	else if(TP_SHOPITEM_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_SHOPITEM_END)
	{
		return FakeGeneralItemMove( ToPos, pFromItem, pToItem );
	}
/*	
	else if(TP_SHOPITEM_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_SHOPITEM_END)
		return m_pItemShopInven->FakeGeneralItemMove( ToPos, pFromItem, pToItem );
	else if(TP_SHOPINVEN_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_SHOPINVEN_END)
	{
		if( ITEMMGR->IsDupItem( pFromItem->GetItemIdx() ) )
		{
			if( IsDivideFunctionKey() && !( pToItem ) )			
				return FakeItemDivide( ToPos, pFromItem, pToItem );
			else if( ( pToItem && pFromItem->GetItemIdx() == pToItem->GetItemIdx() ) )			
				return FakeItemCombine( ToPos, pFromItem, pToItem );
			else
				return m_pItemShopInven->FakeGeneralItemMove( ToPos, pFromItem, pToItem );
		}
	}
*/

	if( ITEMMGR->IsDupItem( pFromItem->GetItemIdx() ) )
	{
		if( IsDivideFunctionKey() && !( pToItem ) )
		{
			return FakeItemDivide( ToPos, pFromItem, pToItem );
		}
		else if( ( pToItem && pFromItem->GetItemIdx() == pToItem->GetItemIdx() ) )
		{
			return FakeItemCombine( ToPos, pFromItem, pToItem );
		}
	}
	
/*	if( m_bShopInven && TP_SHOPITEM_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_SHOPITEM_END)
		return m_pItemShopInven->FakeGeneralItemMove( ToPos, pFromItem, pToItem );	
	else
		return FakeGeneralItemMove( ToPos, pFromItem, pToItem );*/

	if( ToPos == ( TP_WEAR_START + eEquipSlot_Shield - 1 ) )
	{
		switch( HERO->GetWeaponAniType() )
		{
		case eWeaponAnimationType_TwoHand:
		case eWeaponAnimationType_Staff:
		case eWeaponAnimationType_Bow:
		case eWeaponAnimationType_Gun:
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(772) );
				return FALSE;
			}
			break;
		}
	}
		
	return FakeGeneralItemMove( ToPos, pFromItem, pToItem );
}

BOOL CInventoryExDialog::IsDivideFunctionKey()
{
	return KEYBOARD->GetKeyPressed(KEY_MENU);
}

BOOL CInventoryExDialog::FakeGeneralItemMove( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem )
{
	if( CanBeMoved( pFromItem, ToPos ) == FALSE )
		return FALSE;

	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos( pFromItem->GetPosition() );
	if(FromIdx >= eItemTable_Max) return FALSE;
	if( pToItem && CanBeMoved( pToItem, pFromItem->GetPosition() ) == FALSE )
	{
		return FALSE;
	}
	if(pToItem && (FromIdx == eItemTable_MunpaWarehouse))
	{
		if(FromIdx == eItemTable_GuildWarehouse)
			// 070126 LYW --- Inventory : Modified message number.
			//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(53) );
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(365) );
		return FALSE;
	}
	if(pToItem && (FromIdx == eItemTable_Storage))
	{
		if( !pToItem->GetItemInfo()->Deposit )
			return FALSE;
	}

	BYTE ToIdx = ITEMMGR->GetTableIdxForAbsPos( ToPos );
	if(ToIdx >= eItemTable_Max) return FALSE;


	MSG_ITEM_MOVE_SYN msg;
	msg.Category			= MP_ITEM;
	msg.Protocol			= MP_ITEM_MOVE_SYN;
	msg.dwObjectID			= HEROID;

	msg.FromPos				= pFromItem->GetPosition();
	msg.wFromItemIdx		= pFromItem->GetItemIdx();
	msg.ToPos				= ToPos;
	msg.wToItemIdx			= (pToItem?pToItem->GetItemIdx():0);

 	if( msg.FromPos == msg.ToPos )
		return FALSE;

	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Storage);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);

	NETWORK->Send( &msg, sizeof(msg) );
	return TRUE;
}

BOOL CInventoryExDialog::FakeItemDivide( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem )
{
	if( !CanBeMoved( pFromItem, ToPos ) )
		return FALSE;
	if( pFromItem->GetPosition() == ToPos )
		return FALSE;
	if( pFromItem->IsLocked() )
		return FALSE;

	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos( pFromItem->GetPosition() );
	if(FromIdx >= eItemTable_Max) return FALSE;

	BYTE ToIdx = ITEMMGR->GetTableIdxForAbsPos( ToPos );
	if(ToIdx >= eItemTable_Max) return FALSE;
	
	if(pFromItem->GetDurability() == 1)
	{
		return FakeGeneralItemMove(ToPos, pFromItem, pToItem);
	}

	m_divideMsg.Category			= MP_ITEM;
	m_divideMsg.Protocol			= MP_ITEM_DIVIDE_SYN;
	m_divideMsg.dwObjectID			= HEROID;

	m_divideMsg.wItemIdx = pFromItem->GetItemIdx();
	m_divideMsg.FromPos = pFromItem->GetPosition();
	m_divideMsg.FromDur = pFromItem->GetDurability();
	m_divideMsg.ToPos = ToPos;
	m_divideMsg.ToDur = 0;

	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Storage);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_ShopInven);

	// 070126 LYW --- Inventory : Modified message number.
	//cDivideBox * pDivideBox = WINDOWMGR->DivideBox( DBOX_DIVIDE_INV, (LONG)pFromItem->GetAbsX(), (LONG)pFromItem->GetAbsY(), OnFakeItemDivideOk, OnFakeItemDivideCancel, this, pFromItem, CHATMGR->GetChatMsg(185) );
	cDivideBox * pDivideBox = WINDOWMGR->DivideBox( DBOX_DIVIDE_INV, (LONG)pFromItem->GetAbsX(), (LONG)pFromItem->GetAbsY(), OnFakeItemDivideOk, OnFakeItemDivideCancel, this, pFromItem, CHATMGR->GetChatMsg(185) );
	pDivideBox->SetMaxValue( MAX_YOUNGYAKITEM_DUPNUM );
	pDivideBox->SetValue( 0 );
	//pDivideBox->SetValue( m_divideMsg.FromDur );
	
	return FALSE;
		
}

void CInventoryExDialog::OnFakeItemDivideCancel( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Storage);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
}

void CInventoryExDialog::OnFakeItemDivideOk( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	CInventoryExDialog * tDlg = ( CInventoryExDialog * )vData1;
	if( param1 == 0 )
	{
		OnFakeItemDivideCancel(0,NULL,0,NULL,NULL);
		return;
	}

	WORD TargetTableIdx = ITEMMGR->GetTableIdxForAbsPos(tDlg->m_divideMsg.FromPos);
	CItem * pTargetItem = ITEMMGR->GetItemofTable(TargetTableIdx, tDlg->m_divideMsg.FromPos);
	if(!pTargetItem)
	{
		OnFakeItemDivideCancel(0,NULL,0,NULL,NULL);
		return ;
	}
	if(pTargetItem->IsLocked())
	{
		OnFakeItemDivideCancel(0,NULL,0,NULL,NULL);
		return ;
	}

	if(tDlg->m_divideMsg.wItemIdx != pTargetItem->GetItemIdx())
	{
		OnFakeItemDivideCancel(0,NULL,0,NULL,NULL);
		return ;
	}
	if(tDlg->m_divideMsg.FromDur != pTargetItem->GetDurability())
	{
		OnFakeItemDivideCancel(0,NULL,0,NULL,NULL);
		return ;
	}
	CItem * pToItem = ITEMMGR->GetItemofTable(ITEMMGR->GetTableIdxForAbsPos(tDlg->m_divideMsg.ToPos), tDlg->m_divideMsg.ToPos);
	if(pToItem)
	{
		OnFakeItemDivideCancel(0,NULL,0,NULL,NULL);
		return ;
	}

	if( tDlg->m_divideMsg.FromDur > param1 )
	{
		tDlg->m_divideMsg.ToDur			= param1;
		tDlg->m_divideMsg.FromDur		= tDlg->m_divideMsg.FromDur - param1;

		//---KES ItemDivide Fix 071020
		ITEMMGR->SetDivideItemInfo( tDlg->m_divideMsg.FromPos, tDlg->m_divideMsg.ToDur );
		//----------------------------

		NETWORK->Send( &tDlg->m_divideMsg, sizeof(tDlg->m_divideMsg) );
	}
	else
	{
		//������ �̵�
		tDlg->FakeGeneralItemMove(tDlg->m_divideMsg.ToPos, (CItem *)vData2, NULL);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Storage);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	}
}

BOOL CInventoryExDialog::FakeItemCombine( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem )
{
	if( pFromItem->GetPosition() == pToItem->GetPosition() )
		return FALSE;
	if( pFromItem->GetItemIdx() != pToItem->GetItemIdx() )
		return FALSE;

	if( CanBeMoved( pFromItem, ToPos ) == FALSE )
		return FALSE;

	if( pToItem && CanBeMoved( pToItem, pFromItem->GetPosition() ) == FALSE )
		return FALSE;

	if( pFromItem->IsLocked() || pToItem->IsLocked() )
		return FALSE;

	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos( pFromItem->GetPosition() );
	if(FromIdx >= eItemTable_Max) return FALSE;

	BYTE ToIdx = ITEMMGR->GetTableIdxForAbsPos( ToPos );
	if(ToIdx >= eItemTable_Max) return FALSE;

	MSG_ITEM_COMBINE_SYN msg;
	msg.Category			= MP_ITEM;
	msg.Protocol			= MP_ITEM_COMBINE_SYN;
	msg.dwObjectID			= HEROID;

	msg.FromPos				= pFromItem->GetPosition();
	msg.ToPos				= pToItem->GetPosition();
	msg.wItemIdx			= pFromItem->GetItemIdx();
	msg.FromDur				= pFromItem->GetDurability();
	msg.ToDur				= pToItem->GetDurability();

	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Storage);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);


	NETWORK->Send( &msg, sizeof(msg) );

	return TRUE;
}	

void CInventoryExDialog::Linking()
{
	if(!m_fSpread)
	{
		m_pWearedDlg = (CWearedExDialog *)GetWindowForID(IN_WEAREDDLG);
		m_pMoneyEdit = (cStatic *)GetWindowForID(IN_MONEYEDIT);
//		m_pItemShopInven = (CItemShopInven*)GetWindowForID(IN_SHOPITEMGRID);
//		m_pItemShopBtn = (cPushupButton*)GetWindowForID(IN_SHOPITEMBTN);
//		m_pItemShopBtn->SetActive(FALSE);
//		cImage Image;
//		SCRIPTMGR->GetImage(63, &Image, PFT_HARDPATH);
//		m_pItemShopBtn->AddToolTip( CHATMGR->GetChatMsg(731), RGB_HALF(255,255,255), &Image, TTCLR_DEFAULT );
//		m_pItemShopInven->SetActive(FALSE);

		// 070423 LYW --- InventoryExDialog : Add background image of inventory.
		for( BYTE count = 0 ; count < MAX_BACKGROUND ; ++count )
		{
			m_pBackground[count] = (cStatic*)GetWindowForID(IN_BACKGROUND1+count) ;
			m_pBackground[count]->SetActive( FALSE ) ;
		}

		m_fSpread = TRUE;
	}
}

CItem * CInventoryExDialog::GetItemForIdx(DWORD wItemIdx)
{
	// RaMa - 04.12.09
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(wItemIdx);
/*	if(pItemInfo->ItemKind & eSHOP_ITEM)
	{
		for(int i=0; i<m_pItemShopInven->GetCellNum(); i++)
		{
			if( !m_pItemShopInven->IsAddable(i) )
			{
				CItem* pItem = (CItem*)m_pItemShopInven->GetIconForIdx(i);
				if(pItem->GetItemIdx() == wItemIdx && !pItem->IsLocked())
					return pItem;

			}
		}
	}
*/
	BYTE TabNum = GetTabNum();
	for( BYTE i = 0 ; i < TabNum ; ++i )
	{
		cIconGridDialog* gridDlg = (cIconGridDialog*)GetTabSheet(i);

		for( WORD j = 0 ; j < gridDlg->GetCellNum() ; ++j )
		{
			if(!gridDlg->IsAddable(j) )
			{
				CItem * pItem = (CItem *)gridDlg->GetIconForIdx(TP_INVENTORY_START+j/*+TABCELL_INVENTORY_NUM*i*/);
				if(pItem->GetItemIdx() == wItemIdx && pItem->GetItemBaseInfo().nSealed != eITEM_PARAM_UNSEAL)
					return pItem;
			}
		}
	}
	return NULL;
}

DURTYPE	CInventoryExDialog::GetTotalItemDurability(DWORD wItemIdx)
{
	DURTYPE DurSum = 0;

	// RaMa - 04.12.09
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(wItemIdx);
/*	if(pItemInfo)
	if(pItemInfo->ItemKind & eSHOP_ITEM)
	{
		for(int i=0; i<m_pItemShopInven->GetCellNum(); i++)
		{
			if( !m_pItemShopInven->IsAddable(i) )
			{
				CItem* pItem = (CItem*)m_pItemShopInven->GetIconForIdx(i);
				if(pItem->GetItemIdx() == wItemIdx && !(pItem->GetItemParam()&ITEM_PARAM_SEAL))
					DurSum += pItem->GetDurability();
			}
		}

		return DurSum;
	}
*/
	BYTE TabNum = GetTabNum();
	for( BYTE i = 0 ; i < TabNum ; ++i )
	{
		cIconGridDialog* gridDlg = (cIconGridDialog*)GetTabSheet(i);

		for( WORD j = 0 ; j < gridDlg->GetCellNum() ; ++j )
		{
			if(!gridDlg->IsAddable(j) )
			{
				CItem * pItem = (CItem *)gridDlg->GetIconForIdx(TP_INVENTORY_START+j/*+TABCELL_INVENTORY_NUM*i*/);

				if(pItem->GetItemIdx() == wItemIdx)
					DurSum += pItem->GetDurability();
			}
		}
	}

	return DurSum;
}
// 0(all),1(inv),2
BOOL CInventoryExDialog::GetPositionForXYRef(BYTE param, LONG x, LONG y, WORD& pos)
{
	WORD position=0;										// ��� ��ġ

	if(param != 2)
	{
		if(m_bShopInven)
		{
			if( m_pItemShopInven->GetPositionForXYRef(x, y, position) )
			{
				pos = position+TP_SHOPINVEN_START;
				return TRUE;
			}		
		}
		else
		{
			cIconGridDialog * gridDlg = (cIconGridDialog *)GetTabSheet(GetCurTabNum());
			if(gridDlg->GetPositionForXYRef(x, y, position))
			{
				//pos = position+INVENTORY_STARTPOSITION+CELLNUMPERPAGE*GetCurTabNum();		// -> ���� ��ġ�� �ٲ�
				pos = position+TP_INVENTORY_START+TABCELL_INVENTORY_NUM*GetCurTabNum();		// -> ���� ��ġ�� �ٲ�
				return TRUE;
			}
		}
	}

	if(param != 1)
	if(m_pWearedDlg->GetPositionForXYRef(x, y, position))
	{
//		pos = position+WEAR_STARTPOSITION;					// -> ���� ��ġ�� �ٲ�
		pos = position+TP_WEAR_START;					// -> ���� ��ġ�� �ٲ�
		return TRUE;
	}
	
	return FALSE;
}

BOOL CInventoryExDialog::CheckItemLockForItemIdx(DWORD wItemIdx)
{
	BYTE TabNum = GetTabNum();
	for( BYTE i = 0 ; i < TabNum ; ++i )
	{
		cIconGridDialog* gridDlg = (cIconGridDialog*)GetTabSheet(i);

		for( WORD j = 0 ; j < gridDlg->GetCellNum() ; ++j )
		{
			if(!gridDlg->IsAddable(j) )
			{
				CItem * pItem = (CItem *)gridDlg->GetIconForIdx(TP_INVENTORY_START+j);
				
				if( pItem->GetItemIdx() == wItemIdx && pItem->IsLocked() )
					return FALSE;
			}
		}
	}

	return TRUE;
}

void CInventoryExDialog::SetMoney(DWORD Money)
{
	m_pMoneyEdit->SetStaticText(AddComma(Money));
}

BOOL CInventoryExDialog::IsExist(POSTYPE abs_pos)
{
	BOOL bExist = FALSE;
	if(TP_INVENTORY_START <= abs_pos && abs_pos < (TP_INVENTORY_END + ( TABCELL_INVENTORY_NUM * HERO->Get_HeroExtendedInvenCount() )))
	{
		int tabIndex = GetTabIndex( abs_pos );		
		cIconGridDialog * dlg = (cIconGridDialog *)GetTabSheet(tabIndex);
		if(dlg->GetIconForIdx(abs_pos-TABCELL_INVENTORY_NUM*tabIndex-TP_INVENTORY_START))
			bExist = TRUE;
	}
	else if(TP_WEAR_START <= abs_pos && abs_pos < TP_WEAR_END)
	{
		if( m_pWearedDlg->GetIconForIdx(abs_pos - TP_WEAR_START) )
			bExist = TRUE;
	}

	return bExist;
}

// 080703 LUJ, �̵����� ������. �ߺ� �ڵ带 �����ϰ� ������.
BOOL CInventoryExDialog::CanBeMoved( CItem* pItem, POSTYPE toPos )
{
	if( ! pItem )
	{
		return FALSE;
	}

	const POSTYPE		fromPos				= pItem->GetPosition();
	const ITEM_INFO*	movingItemInfo		= pItem->GetItemInfo();

	if( ! movingItemInfo )
	{
		return FALSE;
	}	
	else if( TP_WEAR_START <= toPos && toPos < TP_WEAR_END )
	{
		// 080410 LUJ, â�� -> ��� �̵��� ���´�
		switch( ITEMMGR->GetTableIdxForAbsPos( fromPos ) )
		{
		case eItemTable_Storage:
		case eItemTable_Shop:
		case eItemTable_GuildWarehouse:
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 76 ) );
				return FALSE;
			}
		}

		if( pItem->GetItemBaseInfo().nSealed == eITEM_TYPE_SEAL ) 
		{
			WINDOWMGR->MsgBox( MBI_SEALITEM_CANTEQUIP, MBT_OK, CHATMGR->GetChatMsg( 1177 ) );
			return FALSE ;
		}
		// 080703 LUJ, ���� ���Կ� �������� �ִ� ��� ������ ��ġ�ϴ��� �˻��Ѵ�
		else if( TP_WEAR_START == toPos )
		{
			// 080703 LUJ, ���⸸ ���� ����
			if( ePartType_OneHand	!= movingItemInfo->Part3DType	&&
				ePartType_TwoHand	!= movingItemInfo->Part3DType )
			{
				return FALSE;
			}

			CItem*				shieldItem		= GetItemForPos( TP_WEAR_START + eWearedItem_Shield );
			const ITEM_INFO*	shieldItemInfo	= ( shieldItem ? shieldItem->GetItemInfo() : 0 );

			// 080703 LUJ, ���� ���Կ� �������� ����
			if( shieldItemInfo )
			{
				// 080703 LUJ, ��� ������ ��� �翬 ����
				if( ePartType_TwoHand == movingItemInfo->Part3DType )
				{
					return FALSE;
				}
				// 080703 LUJ, ���а� �ƴϰ� ���Ⱑ ���� ��� ������ ��ġ�ؾ� ���� ����
				else if(	shieldItemInfo->Part3DType	!= ePartType_Shield		&&
							shieldItemInfo->EquipType	!= movingItemInfo->EquipType )
				{
					return FALSE;
				}
			}
		}
		// 080703 LUJ, ���� ���Կ� ���⸦ ������ �� �ֵ��� üũ�Ѵ�
		else if( TP_WEAR_START + eWearedItem_Shield == toPos )
		{
			CItem*				weaponItem			= GetItemForPos( TP_WEAR_START + eWearedItem_Weapon );
			const ITEM_INFO*	weaponItemInfo		= ( weaponItem ? weaponItem->GetItemInfo() : 0 );
			const BOOL			isEnableTwoBlade	= HERO->GetHeroTotalInfo()->bUsingTwoBlade;

			// 080703 LUJ, ���� ���Կ� �������� �ִ� ���, ���� ������ ������ �� �ִ�
			if( weaponItemInfo )
			{
				// 080703 LUJ, ��� ����� ���� �Ұ�
				if( ePartType_TwoHand == weaponItemInfo->Part3DType ||
					ePartType_TwoHand == movingItemInfo->Part3DType )
				{
					return FALSE;
				}				
				// 080703 LUJ, �̵��� �����Ͱ� ���µ� ���⸦ �����Ϸ� �� ���
				else if( ePartType_Shield != movingItemInfo->Part3DType )
				{
					if( ! isEnableTwoBlade )
					{
						// 080714 LUJ, ���ҽ��� �߸� ������Ʈ�Ǿ� �޽��� ��ȣ�� ����
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1591 ) );
						return FALSE;
					}
					else if( movingItemInfo->WeaponType	!= weaponItemInfo->WeaponType )
					{
						// 080714 LUJ, ���ҽ��� �߸� ������Ʈ�Ǿ� �޽��� ��ȣ�� ����
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1592 ) );
						return FALSE;
					}
					else if( movingItemInfo->WeaponType == eWeaponType_Staff )
					{
						// 080714 LUJ, ���ҽ��� �߸� ������Ʈ�Ǿ� �޽��� ��ȣ�� ����
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1593 ) );
						return FALSE;
					}
				}
			}
			// 080703 LUJ, ���� ������ �� ��� ���и� ������ �� �ִ�
			else if( ePartType_Shield != movingItemInfo->Part3DType )
			{
				CHATMGR->AddMsg(
					CTC_SYSMSG,
					// 080714 LUJ, ���ҽ��� �߸� ������Ʈ�Ǿ� �޽��� ��ȣ�� ����
					isEnableTwoBlade ? CHATMGR->GetChatMsg( 1594 ) : CHATMGR->GetChatMsg( 1591 ) );
				
				return FALSE;
			}
		}
		else if( movingItemInfo->EquipSlot == eWearedItem_Earring1 )
		{	
			if(toPos-TP_WEAR_START != eWearedItem_Earring1 && toPos-TP_WEAR_START != eWearedItem_Earring2)
				return FALSE;
		}
		else if( movingItemInfo->EquipSlot == eWearedItem_Ring1 )
		{
			if(toPos-TP_WEAR_START != eWearedItem_Ring1 && toPos-TP_WEAR_START != eWearedItem_Ring2)
				return FALSE;
		}		
		else if( movingItemInfo->EquipSlot == eWearedItem_Earring1 )
		{	
			if( toPos - TP_WEAR_START != eWearedItem_Earring1 && toPos - TP_WEAR_START != eWearedItem_Earring2)
			{
				return FALSE;
			}
		}
		else if( movingItemInfo->EquipSlot == eWearedItem_Ring1 )
		{	
			if( toPos - TP_WEAR_START != eWearedItem_Ring1 && toPos - TP_WEAR_START != eWearedItem_Ring2)
			{
				return FALSE;
			}
		}
		else if( movingItemInfo->EquipSlot + TP_WEAR_START != toPos )
		{
			return FALSE;
		}

		const BOOL isEnableEquip = ITEMMGR->CanEquip(pItem->GetItemIdx() );

		if( ! isEnableEquip )
		{
			if( movingItemInfo->WeaponType == eWeaponType_FishingPole )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1530 ) );
			}
			else
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 825 ), movingItemInfo->ItemName );
			}

			return FALSE;
		}
	}
	// 080703 LUJ, ���� ���Կ��� �������� ���� ��, �̵��� ������ ��� �� �� ����
	//				���� ���Կ��� ���Ⱑ ���� ��� �ִϸ��̼��� �ſ� ����ϴ�
	else if( fromPos == TP_WEAR_START + eWearedItem_Weapon )
	{
		CItem* item = GetItemForPos( TP_WEAR_START + eWearedItem_Shield );

		if( item &&
			item->GetItemInfo()->WeaponType == movingItemInfo->WeaponType )
		{
			// 080714 LUJ, ���ҽ��� �߸� ������Ʈ�Ǿ� �޽��� ��ȣ�� ����
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1595 ) );
			return FALSE;
		}
	}
	// 071210 LYW --- InventoryExDialog : �κ��丮 Ȯ�忡 ���� ó���� �Ѵ�.
	else if( TP_INVENTORY_START<= toPos && toPos < ( TP_INVENTORY_END + ( TABCELL_INVENTORY_NUM * HERO->Get_HeroExtendedInvenCount() ) ) )
	{
		if( TP_SHOPITEM_START <= fromPos && fromPos < TP_SHOPITEM_END )
		{
			return TRUE;
		}
		else if( TP_STORAGE_START <= fromPos && fromPos < TP_STORAGE_END )
		{
			return TRUE ;
		}
	}	
	else if( TP_STORAGE_START <= toPos && toPos < TP_STORAGE_END )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

	if( movingItemInfo->Category == eItemCategory_Equip && ! OBJECTSTATEMGR->CheckAvailableState( HERO, eCAS_EQUIPITEM ) )
	{
		CAction act;
		act.InitAction(
			eActionKind_EquipItem,
			( void* )pItem->GetDBIdx(),
			0,
			NULL );

		HERO->SetNextAction( &act );
		return FALSE;
	}

	return TRUE;
}

void CInventoryExDialog::ItemUseForQuestStart()
{
	if( m_pTempQuestItem )
	{
		// quest �������� ���
		MSG_ITEM_USE_SYN msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_USE_FOR_QUESTSTART_SYN;
		msg.dwObjectID = HEROID;
		msg.TargetPos = m_pTempQuestItem->GetPosition();
		msg.wItemIdx = m_pTempQuestItem->GetItemIdx();

		NETWORK->Send(&msg,sizeof(msg));
	}
}





//---------------------------------------------------------------------------------------------------------------------------
//	071114 LYW --- IventoryExDialog : ������ ���� ���� ��û.
//---------------------------------------------------------------------------------------------------------------------------
void CInventoryExDialog::ItemUnSealingSync()
{
	CItem* pItem = NULL ;
	pItem = GetCurSelectedItem(eItemTable_Inventory) ;

	if( !pItem ) return ;

	const ITEMBASE& pItemBase = pItem->GetItemBaseInfo() ;

	if( pItemBase.nSealed != eITEM_TYPE_SEAL ) return ;	
	
	if( pItem->IsLocked() ) return ;

	MSG_ITEM_USE_SYN msg;
	msg.Category = MP_ITEM ;
	msg.Protocol = MP_ITEM_USE_SYN ;
	msg.dwObjectID = HEROID ;
	msg.wItemIdx = pItem->GetItemIdx() ;
	msg.TargetPos = pItem->GetPosition() ;

	NETWORK->Send(&msg,sizeof(msg)) ;
}


//---------------------------------------------------------------------------------------------------------------------------
//	071119 LYW --- IventoryExDialog : ������ ���� ���� ó��.
//---------------------------------------------------------------------------------------------------------------------------
void CInventoryExDialog::ItemUnSealing(DWORD absPos)
{
	CItem* pItem = ITEMMGR->GetItemofTable((WORD)eItemTable_Inventory, (POSTYPE)absPos) ;

	if( !pItem ) return ;

	ITEMPARAM Param = pItem->GetItemBaseInfo().ItemParam ;
	pItem->SetItemParam( eITEM_PARAM_UNSEAL ) ;

	// ������ �ٽ� �����ش�.
	ITEM_INFO* pItemInfo = NULL ;
	pItemInfo = ITEMMGR->GetItemInfo( pItem->GetItemIdx() ) ;

	if( !pItemInfo ) return ;

	ITEMMGR->AddToolTip( pItem ) ;

	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(366) ) ;
}


void CInventoryExDialog::SetShopItemInvenBtn( BOOL val )
{
	//m_pItemShopBtn->SetActive( val );
}

// 070423 LYW ---- InventoryExDialog : Add functions to setting background.
void CInventoryExDialog::SetActiveBackground(BYTE idx)
{
	if( idx >= 0 && idx < MAX_BACKGROUND )
	{
		m_pBackground[idx]->SetActive(TRUE) ;
	}
}

void CInventoryExDialog::CheckBackground() 
{
	CHero* pHero = HERO ;

	if(pHero == NULL ) return ;

	CHARACTER_TOTALINFO* pTotalInfo = pHero->GetCharacterTotalInfo() ;
	
	if(pTotalInfo)
	{
		BYTE byRacial = pTotalInfo->Race ;
		BYTE byGender = pTotalInfo->Gender ;

		for( BYTE count = 0 ; count < MAX_BACKGROUND ; ++count )
		{
			m_pBackground[count]->SetActive( FALSE ) ;
		}

		if( !byRacial )
		{
			if( !byGender )
			{
				m_pBackground[0]->SetActive( TRUE ) ;
			}
			else
			{
				m_pBackground[1]->SetActive( TRUE ) ;
			}
		}
		else
		{
			if( !byGender )
			{
				m_pBackground[2]->SetActive( TRUE ) ;
			}
			else
			{
				m_pBackground[3]->SetActive( TRUE ) ;
			}
		}
	}
}


void CInventoryExDialog::SetCoolTime( DWORD itemIndex, DWORD miliSecond )
{
	const DWORD groupIndex = ITEMMGR->GetCoolTimeGroupIndex( itemIndex );

	for( BYTE tabIndex = 0; tabIndex < GetTabNum(); ++tabIndex )
	{
		cIconGridDialog* tab = ( cIconGridDialog* )GetTabSheet( tabIndex );
		
		if( ! tab )
		{
			continue;
		}

		for( WORD iconIndex = 0; iconIndex < tab->GetCellNum(); ++iconIndex )
		{
			cIcon* icon = tab->GetIconForIdx( iconIndex );

			if(		icon &&
				(	icon->GetData() == itemIndex ||
					groupIndex && ITEMMGR->GetCoolTimeGroupIndex( icon->GetData() ) == groupIndex ) )
			{
				icon->SetCoolTime( miliSecond );
			}
		}
	}
}

void CInventoryExDialog::SetCoolTimeAbs( DWORD itemIndex, DWORD dwBegin, DWORD dwEnd, DWORD miliSecond )
{
	const DWORD groupIndex = ITEMMGR->GetCoolTimeGroupIndex( itemIndex );

	for( BYTE tabIndex = 0; tabIndex < GetTabNum(); ++tabIndex )
	{
		cIconGridDialog* tab = ( cIconGridDialog* )GetTabSheet( tabIndex );
		
		if( ! tab )
		{
			continue;
		}

		for( WORD iconIndex = 0; iconIndex < tab->GetCellNum(); ++iconIndex )
		{
			cIcon* icon = tab->GetIconForIdx( iconIndex );

			if(		icon &&
				(	icon->GetData() == itemIndex ||
					groupIndex && ITEMMGR->GetCoolTimeGroupIndex( icon->GetData() ) == groupIndex ) )
			{
				icon->SetCoolTimeAbs( dwBegin, dwEnd, miliSecond );
			}
		}
	}
}


void CInventoryExDialog::Send()
{
	MSG_ITEM_USE_SYN message( mMessage );

	NETWORK->Send( &message, sizeof( message ) );
}


void CInventoryExDialog::Restore()
{
	SetDisable( FALSE );

	CItem* item = GetItemForPos( mMessage.TargetPos );

	if( item )
	{
		item->SetCoolTime( 0 );

		cQuickSlotDlg* dialog = ( cQuickSlotDlg* )WINDOWMGR->GetWindowForID( QI1_QUICKSLOTDLG );
		ASSERT( dialog );

		dialog->SetCoolTime( item->GetItemIdx(), 0 );


		// 080706 LYW --- InventoryExDialog : Ȯ�� �� ���� �߰� ó��.
		cQuickSlotDlg* dialog2 = NULL ;
		dialog2 = ( cQuickSlotDlg* )WINDOWMGR->GetWindowForID( QI2_QUICKSLOTDLG ) ;
		ASSERT(dialog2) ;

		if(dialog2)
		{
			dialog2->SetCoolTime( item->GetItemIdx(), 0 ) ;
		}
	}
}






// 071210 LYW --- CInventoryExDialog : Ȯ�� �κ��丮�� Ȱ��ȭ ���θ� �����ϴ� �Լ� �߰�.
void CInventoryExDialog::InitExtended_Inventory( CHARACTER_TOTALINFO* pInfo )
{
	if( !pInfo ) return ;

	if( pInfo->wInventoryExpansion == 0 ) return ;

	if( pInfo->wInventoryExpansion == 1 )
	{
		ShowTab(2,TRUE) ;

		HERO->Set_HeroExtendedInvenCount(1) ;
	}
	else if( pInfo->wInventoryExpansion == 2 )
	{
		ShowTab(2,TRUE) ;
		ShowTab(3,TRUE) ;

		HERO->Set_HeroExtendedInvenCount(2) ;
	}
}

/*
// 080213 KTH -- ClearInventory
BOOL CInventoryExDialog::ClearInventory()
{
	for( int i = TP_INVENTORY_START; i < (TP_INVENTORY_END + (TABCELL_INVENTORY_NUM * HERO->Get_HeroExtendedInvenCount())); i++ )
	{
			ITEMMGR->DeleteItemofTable( eItemTable_Inventory, i, false );
			int TabIndex = GetTabIndex( i );
			cIconGridDialog * Dlg = (cIconGridDialog *)GetTabSheet(TabIndex);
			cIcon* pIcon = Dlg->GetIconForIdx(i-TP_FAMILYWAREHOUSE_START-TABCELL_FAMILYWAREHOUSE_NUM*TabIndex);
			if(!Dlg->IsAddable(i - TP_INVENTORY_START-TABCELL_INVENTORY_NUM*TabIndex))
				if( Dlg->DeleteIcon(i-TP_INVENTORY_START-TABCELL_INVENTORY_NUM*TabIndex, (cIcon**)pIcon) )
				{
					continue;
				}
			else
				return FALSE;
	}
//						CItem* pOutItem = NULL;
//						DeleteItem( pmsg->TargetPos, &pOutItem);
	}

	return TRUE;
}
*/

void CInventoryExDialog::SetMultipleItemLock(DWORD dwItemIdx, BOOL bVal)
{
	for( BYTE tabIndex = 0; tabIndex < GetTabNum(); ++tabIndex )
	{
		cIconGridDialog* tab = ( cIconGridDialog* )GetTabSheet( tabIndex );
		
		if( ! tab )
		{
			continue;
		}

		for( WORD iconIndex = 0; iconIndex < tab->GetCellNum(); ++iconIndex )
		{
			cIcon* icon = tab->GetIconForIdx( iconIndex );

			if(icon && icon->GetData()==dwItemIdx)
			{
				icon->SetLock(bVal);
			}
		}
	}
}

void CInventoryExDialog::PetGradeUp()
{
	SetCoolTime( mMessage.wItemIdx, WAITMILISECOND );

	{
		cQuickSlotDlg* dialog = ( cQuickSlotDlg* )WINDOWMGR->GetWindowForID( QI1_QUICKSLOTDLG );
		ASSERT( dialog );

		dialog->SetCoolTime( mMessage.wItemIdx, WAITMILISECOND );
	}

	{
		CProgressDialog* dialog = ( CProgressDialog* )WINDOWMGR->GetWindowForID( PROGRESS_DIALOG );
		ASSERT( dialog );

		dialog->SetText( RESRCMGR->GetMsg( 1008 ) );
		dialog->Wait( CProgressDialog::eActionPetGradeUp );
	}
}

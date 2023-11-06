#include "stdafx.h"
#include "StreetStallManager.h"
#include "Item.h"
#include "itemmanager.h"
#include "GameIn.h"
#include "ExchangeItem.h"
#include "BuyItem.h"
#include "WindowIdEnum.h"
#include "./interface/cWindowManager.h"
#include "objectmanager.h"
#include "objectstatemanager.h"
#include "cMsgBox.h"
#include "ChatManager.h"
#include "MoveManager.h"
#include "Showdownmanager.h"

#include "StallKindSelectDlg.h"
#include "StreetBuyStall.h"

#include "StreetStall.h"
#include "InventoryExDialog.h"

#include "FilteringTable.h"
#include "MHCamera.h"
#include "Quickmanager.h"

#include "BattleSystem_Client.h"

// 070223 LYW --- StreetStallManager : Modified message number.

// 070504 LYW --- StreetStallManager : Include appearance manager.
#include "AppearanceManager.h"

// 071012 LYW --- StreetStallManager : Include static.
#include "./interface/cStatic.h"
#include "./Interface/cResourceManager.h"


GLOBALTON(CStreetStallManager);
CStreetStallManager::CStreetStallManager()
{
	m_bOpenMsgBox = TRUE;
	m_wStallKind = eSK_NULL;

	m_nLinkedItemNum = 0;
	m_IconIndexCreator.Init(MAX_ITEMICON_NUM, IG_STREETSTALLITEM_STARTINDEX);
}

CStreetStallManager::~CStreetStallManager()
{
	// 071213 LYW --- CStreetStallManager : �ε��� ���׷����� ���� ó�� �߰�.
	m_IconIndexCreator.Release() ;
}

void CStreetStallManager::Init()
{
	m_bOpenMsgBox = TRUE;
	m_wStallKind = eSK_NULL;

	m_nLinkedItemNum = 0;


	//KES
	//	m_posWantBuy	= 0;
	m_dwWantBuyNum	= 0;
	m_dwWantSellNum	= 0;

//---KES �����˻� 2008.3.11
	m_strSearchWord[0] = 0;	//�ʱ�ȭ
//---------------	
}

//---KES �����˻� 2008.3.11
BOOL CStreetStallManager::IsSearchWordIn( const char* strSrc )
{
	if( strSrc == NULL ) return FALSE;
	if( m_strSearchWord[0] == 0 ) return FALSE; //�˻��ܾ� �̵��

	if( strstr( strSrc, m_strSearchWord ) )
		return TRUE;
	
	return FALSE;
}

void CStreetStallManager::SearchWordInArea()
{
	CYHHashTable<CObject>& ObjectTable = OBJECTMGR->GetObjectTable();

	ObjectTable.SetPositionHead();

	while( CObject* pObject = ObjectTable.GetData() )
	{
		if( eObjectKind_Player != pObject->GetObjectKind() )
			continue;

		((CPlayer*)pObject)->RefreshStreetStallTitle();
	}	
}
//-------------------------

BOOL CStreetStallManager::LinkItem( CItem* pItem, ITEMBASE* pBase )
{
	CExchangeItem* pExItem = new CExchangeItem;
	int pos;

	pExItem->InitItem( pItem, 0, m_IconIndexCreator.GenerateIndex() );
	pExItem->SetType( WT_STALLITEM );
	pExItem->SetDBId( pBase->dwDBIdx );
	pExItem->SetDurability( pBase->Durability );

	// 071126 LYW --- StreetStallManager : ���� �������̸�, ���� ��ũ Ȱ�� ó��.
	const ITEMBASE& itemBase = pItem->GetItemBaseInfo() ;

	if( itemBase.nSealed == eITEM_TYPE_SEAL )
	{
		pExItem->SetSeal(TRUE) ;
	}

	pos = GAMEIN->GetStreetStallDialog()->AddItem( pExItem );

	if( pos == -1 ) 
	{
		SAFE_DELETE(pExItem);
		return FALSE;
	}
	WINDOWMGR->AddWindow( pExItem );

	pExItem->SetPosition( pos );

	pItem->SetLock( TRUE );
	pItem->SetLinkPosition( pos );

	return TRUE;
}

BOOL CStreetStallManager::LinkItem( POSTYPE pos, ITEMBASE ItemInfo, WORD Volume, DWORD Money)
{
	if(pos < 0 || pos >= SLOT_STREETSTALL_NUM) return FALSE;

	CBuyItem* pBuyItem = new CBuyItem;

	pBuyItem->InitItem( ItemInfo, Volume, Money );
	pBuyItem->SetType( WT_STALLITEM );

	if( !GAMEIN->GetStreetBuyStallDialog()->AddItem( pos, pBuyItem ) ) 
	{
		SAFE_DELETE(pBuyItem);
		return FALSE;
	}
	WINDOWMGR->AddWindow( pBuyItem );

	return TRUE;
}

void CStreetStallManager::UnlinkItem( CBuyItem* pItem )
{
	WINDOWMGR->AddListDestroyWindow( pItem );
}

//void CStreetStallManager::UnlinkItem( CExchangeItem* pItem )
//{
//	if( pItem->GetLinkItem() == NULL )
//		return;
//
//	pItem->GetLinkItem()->SetLinkPosition( 0 );
//	pItem->GetLinkItem()->SetLinkItem( NULL );
//	pItem->GetLinkItem()->SetLock( FALSE );
//	WINDOWMGR->AddListDestroyWindow( pItem );
//}

void CStreetStallManager::UnlinkItem( CExchangeItem* pItem )
{
	if( !pItem ) return ;

	CInventoryExDialog * pDlg = NULL ;
	pDlg = GAMEIN->GetInventoryDialog();

	if( !pDlg ) return ;

	if( pItem->GetLinkItem() == NULL )
		return;

	pItem->GetLinkItem()->SetLinkPosition( 0 );
	pItem->GetLinkItem()->SetLinkItem( NULL );
	pItem->GetLinkItem()->SetLock( FALSE );
	WINDOWMGR->AddListDestroyWindow( pItem );

	CItem* pDeleteItem = pDlg->GetItemForPos( pItem->GetLinkPos() ) ;

	if( !pDeleteItem ) return ;

	pDeleteItem->SetZeroCount(FALSE) ;
}


void CStreetStallManager::ToggleHero_StreetStallMode( BOOL bOpen )
{	
	if( !bOpen )
		//	if( OBJECTSTATEMGR->GetObjectState(HERO) == eObjectState_StreetStall_Owner )
	{
		if(OBJECTSTATEMGR->IsEndStateSetted(HERO) == TRUE)
			return;
		Toggle_StreetStallMode(HERO, FALSE);

		DWORD time = HERO->GetEngineObject()->GetAnimationTime(eMotion_StreetStall_End);
		CAMERA->SetCharState( eCS_Normal, time );

		m_wStallKind = eSK_NULL;
	}
	else
	{
		//050324 �̹� ���¸� ������ ���� �����̹Ƿ� �Ʒ��� �ּ�ó�� �ߴ�.
		//		// ���°� None �϶��� �� �� �ִ�.
		//		if(HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
		//			return;

		//050324 �̹� ���¸� ������ ���� �����̹Ƿ� �Ʒ��� �ּ�ó�� �ߴ�.
		//		Toggle_StreetStallMode(HERO, TRUE);

		DWORD time = HERO->GetEngineObject()->GetAnimationTime(eMotion_StreetStall_Start);
		CAMERA->SetCharState( eCS_Sit, time );

		m_strOldTitle[0] = 0;
	}
}

void CStreetStallManager::Toggle_StreetStallMode(CObject* pObject, BOOL bStreet)
{
	if(bStreet == TRUE)
	{
		OBJECTSTATEMGR->StartObjectState(pObject, eObjectState_StreetStall_Owner);
	}
	else 
	{
		//		if( pObject == HERO )
		//		{
		//			DWORD time = pObject->GetEngineObject()->GetAnimationTime(eMotion_StreetStall_End);
		//			OBJECTSTATEMGR->EndObjectState(pObject, eObjectState_StreetStall_Owner,time);			
		//		}
		//		else
		//		{
		DWORD time = pObject->GetEngineObject()->GetAnimationTime(eMotion_StreetStall_End);
		OBJECTSTATEMGR->EndObjectState(pObject, eObjectState_StreetStall_Owner,time);

		// 070504 LYW --- StreetStallManager : Setting to show weapon.
		APPEARANCEMGR->ShowWeapon( ((CPlayer*)pObject) ) ;
		//		}
	}
}

void CStreetStallManager::OpenStreetStall()
{
	BOOL isOpen = FALSE;

	// ����/��æƮ/��ȭ/���� ���� ��� ���� ���� �Ұ�
	{
		cDialog* mixDialog			= WINDOWMGR->GetWindowForID( ITMD_MIXDLG );
		cDialog* enchantDialog		= WINDOWMGR->GetWindowForID( ITD_UPGRADEDLG );
		cDialog* reinforceDialog	= WINDOWMGR->GetWindowForID( ITR_REINFORCEDLG );
		cDialog* dissoloveDialog	= WINDOWMGR->GetWindowForID( DIS_DISSOLUTIONDLG );

		ASSERT( mixDialog && enchantDialog && reinforceDialog && dissoloveDialog );

		isOpen =	mixDialog->IsActive()		||
					enchantDialog->IsActive()	||
					reinforceDialog->IsActive()	||
					dissoloveDialog->IsActive();
	}

	if( ( HERO->GetState() != eObjectState_None &&
		HERO->GetState() != eObjectState_Immortal )
		|| SHOWDOWNMGR->IsShowdown()		
		|| BATTLESYSTEM->GetBattle(HERO)->GetBattleKind() == eBATTLE_KIND_GTOURNAMENT 
		|| BATTLESYSTEM->GetBattle(HERO)->GetBattleKind() == eBATTLE_KIND_SIEGEWAR
		|| isOpen )
	{
		m_bOpenMsgBox = TRUE;
		m_wStallKind = eSK_NULL;
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(473) );
		return;
	}

	MSG_STREETSTALL_TITLE msg;
	msg.Category = MP_STREETSTALL;
	msg.Protocol = MP_STREETSTALL_OPEN_SYN;
	msg.dwObjectID = HEROID;
	msg.StallKind = m_wStallKind;
	//	strcpy( msg.Title, GAMEIN->GetStreetStallDialog()->GetTitle() );

	// LYJ 051017 ���Գ����� �߰�

	if(m_wStallKind == eSK_SELL)
		GAMEIN->GetStreetStallDialog()->GetTitle( msg.Title );
	else if(m_wStallKind == eSK_BUY)
		GAMEIN->GetStreetBuyStallDialog()->GetTitle( msg.Title );

	msg.StallKind = m_wStallKind;

	NETWORK->Send( &msg, sizeof(MSG_STREETSTALL_TITLE) );

	//050324 ������ ���۵Ǹ鼭 �ٷ� ������Ʈ ����
	OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_StreetStall_Owner);
}

void CStreetStallManager::CloseStreetStall()
{
	if((m_wStallKind == eSK_SELL)&&( GAMEIN->GetStreetStallDialog()->GetDlgState() == eSDS_BUY ))
	{	//�Ǹ� ������ �Խ�Ʈ�� ������ ���
		MSG_DWORD msg;
		msg.Category = MP_STREETSTALL;
		msg.Protocol = MP_STREETSTALL_GUESTOUT_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData = GAMEIN->GetStreetStallDialog()->GetStallOwnerId();
		NETWORK->Send( &msg, sizeof(MSG_DWORD) );
	}
	else if((m_wStallKind == eSK_BUY)&&( GAMEIN->GetStreetBuyStallDialog()->GetDlgState() == eBSDS_SELL ))
	{	//���� ������ �Խ�Ʈ�� ������ ���
		MSG_DWORD msg;
		msg.Category = MP_STREETSTALL;
		msg.Protocol = MP_STREETSTALL_GUESTOUT_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData = GAMEIN->GetStreetBuyStallDialog()->GetStallOwnerId();
		NETWORK->Send( &msg, sizeof(MSG_DWORD) );
	}
	else if(((m_wStallKind == eSK_SELL)&&( GAMEIN->GetStreetStallDialog()->GetDlgState() ==  eSDS_OPENED )) ||
		((m_wStallKind == eSK_BUY)&&( GAMEIN->GetStreetBuyStallDialog()->GetDlgState() ==  eBSDS_OPENED )))
	{	//���� ������ ���
		MSGBASE msg;
		msg.Category = MP_STREETSTALL;
		msg.dwObjectID = HEROID;
		msg.Protocol = MP_STREETSTALL_CLOSE_SYN;
		NETWORK->Send( &msg, sizeof(MSGBASE) );
	}
}

void CStreetStallManager::EnterStreetStall( DWORD OwnerId )
{
	if( HERO->GetState() == eObjectState_StreetStall_Guest )
	{
		// ���� ������ �ȵǴµ� ^^;;;
		// ASSERT(0);
		return;
	}

	if(GAMEIN->GetStallKindSelectDialog()->IsActive())
		return;

	//KES 040924
	OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_StreetStall_Guest );

	MSG_DWORD msg;
	msg.Category = MP_STREETSTALL;
	msg.Protocol = MP_STREETSTALL_GUESTIN_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData = OwnerId;
	NETWORK->Send(&msg, sizeof(MSG_DWORD));
}
//////////////////////////////////////////////////////////////////////////
//�Ǹ� ������ ��� �Լ�
void CStreetStallManager::RegistItemEx( CItem* pItem , DWORD money )
{
	// 071125 LYW --- StreetStallManager : pItem ���Ἲ �˻�.
	if( !pItem ) return ;

	//���� �����ε� �Ǹ� �������� ����Ϸ� �Ѵ�
	if(m_wStallKind == eSK_BUY)
		return;		

	MSG_REGISTITEMEX msg;
	msg.Category = MP_STREETSTALL;
	msg.Protocol = MP_STREETSTALL_FAKEREGISTITEM_SYN;
	msg.dwObjectID = HEROID;
	msg.ItemInfo = pItem->GetItemBaseInfo();
	msg.dwData = m_nLinkedItemNum++;
	msg.dwMoney = money;

	NETWORK->Send( &msg, sizeof(MSG_REGISTITEMEX) );
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//���� ������ ��� �Լ�
void CStreetStallManager::RegistItemEx( ITEMBASE ItemInfo, WORD wAbsPosition, WORD Volume, DWORD money )
{
	//�Ǹ� �����ε� ���� ������ ����Ϸ� �Ѵ�
	if(m_wStallKind == eSK_SELL)
		return;		

	MSG_REGISTBUYITEMEX msg;
	msg.Category = MP_STREETSTALL;
	msg.Protocol = MP_STREETSTALL_FAKEREGISTBUYITEM_SYN;
	msg.dwObjectID = HEROID;
	msg.ItemInfo = ItemInfo;
	msg.dwData = m_nLinkedItemNum++;
	msg.dwMoney = money;
	msg.wVolume = Volume;
	msg.wAbsPosition = wAbsPosition; 

	NETWORK->Send( &msg, sizeof(MSG_REGISTBUYITEMEX) );
}
//////////////////////////////////////////////////////////////////////////

void CStreetStallManager::EditTitle()
{
	//	char* strTitle = GAMEIN->GetStreetStallDialog()->GetTitle();
	char strTitle[MAX_STREETSTALL_TITLELEN + 1];

	if(m_wStallKind == eSK_SELL)
		GAMEIN->GetStreetStallDialog()->GetTitle( strTitle );
	else if(m_wStallKind == eSK_BUY)
		GAMEIN->GetStreetBuyStallDialog()->GetTitle( strTitle );

	//	GAMEIN->GetStreetStallDialog()->GetTitle( strTitle );

	//����Ǿ��� üũ�ѹ� �ϴ°��� ���ڴ�. ���� �ȵǾ����� �ٸ�ó�� ���ϰ� �ٷ� ����.
	if( strcmp( m_strOldTitle, strTitle ) == 0 )
		return;

	if( FILTERTABLE->FilterChat( strTitle ) )	//FILTERING  KES
	{
		//���� ����Ͽ���.
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(501) );
	}
	else
	{
		MSG_STREETSTALL_TITLE msg;
		msg.Category = MP_STREETSTALL;
		msg.Protocol = MP_STREETSTALL_EDITTITLE_SYN;
		msg.StallKind = m_wStallKind;
		msg.dwObjectID = HEROID;
		SafeStrCpy( msg.Title, strTitle, MAX_STREETSTALL_TITLELEN+1 );

		NETWORK->Send( &msg, sizeof(MSG_STREETSTALL_TITLE) );

		SafeStrCpy( m_strOldTitle, strTitle, MAX_STREETSTALL_TITLELEN + 1 );	//���� Ÿ��Ʋ ���
	}
}

//////////////////////////////////////////////////////////////////////////
// ������ �Ǹ� �Լ�
BOOL CStreetStallManager::SellItem()
{
	CStreetBuyStall* pDlg = GAMEIN->GetStreetBuyStallDialog();
	if( pDlg == NULL || pDlg->IsActive() == FALSE )
		return FALSE;

	CBuyItem* pBuyItem = (CBuyItem*)pDlg->GetCurSelectedItem();

	if( !pBuyItem || !pBuyItem->IsActive() ) 
		return FALSE;

	CItem* pItem = pDlg->GetSellItem();

	if( pItem == NULL )
	{
		pDlg->SetData( NULL );
		pDlg->SetDisable( FALSE );
		return FALSE;
	}

	m_dwWantSellNum = 1;

	cMsgBox* pBox = WINDOWMGR->MsgBox( MBI_STREETSELLMSG, MBT_YESNO, CHATMGR->GetChatMsg(339), pItem->GetItemInfo()->ItemName, AddComma( pBuyItem->GetMoney() ) );

	return TRUE;
}

BOOL CStreetStallManager::SellItemDur( DWORD dwNum )
{
	CStreetBuyStall* pDlg = GAMEIN->GetStreetBuyStallDialog();
	if( pDlg == NULL || pDlg->IsActive() == FALSE )
		return FALSE;

	CBuyItem* pBuyItem = (CBuyItem*)pDlg->GetCurSelectedItem();

	if( !pBuyItem || !pBuyItem->IsActive() ) 
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(502) );

		return FALSE;
	}

	CItem* pItem = pDlg->GetSellItem();

	if( pItem == NULL )
	{
		pDlg->SetData( NULL );
		pDlg->SetDisable( FALSE );
		return FALSE;
	}

	// �ȷ��� ������ �κ��� �ִ� �������� ũ�ٸ�
	// �κ��� �ִ� �������� ����
	if( pItem->GetDurability() < dwNum )
		dwNum = pItem->GetDurability();

	m_dwWantSellNum = dwNum;

	char temp[32];
	SafeStrCpy( temp, AddComma( pDlg->GetItemMoney( pDlg->GetCurSelectedItemNum() ) ), 32 );

	cMsgBox* pBox = WINDOWMGR->MsgBox( MBI_STREETSELLMSG, MBT_YESNO, CHATMGR->GetChatMsg(340), pItem->GetItemInfo()->ItemName, dwNum, temp, AddComma( pBuyItem->GetMoney() * dwNum) );

	return TRUE;
}
// ������ �Ǹ� �Լ�
//////////////////////////////////////////////////////////////////////////

void CStreetStallManager::BuyItem()
{
	CStreetStall* pDlg = GAMEIN->GetStreetStallDialog();
	if( pDlg == NULL )				return;
	if( pDlg->IsActive() == FALSE ) return;

	POSTYPE pos = pDlg->GetCurSelectedItemNum();
	DWORD dwCheckDBIdx = pDlg->GetCheckDBIdx();

	//KES pos �� dwNum�� ����ϰ� ����.
	//	m_posWantBuy = pos;
	m_dwWantBuyNum = 1;

	CExchangeItem* pItem = (CExchangeItem*)pDlg->GetCurSelectedItem();

	if( pItem == NULL )
	{
		pDlg->SetData( NULL );
		pDlg->SetDisable( FALSE );
		return;
	}

	if( pDlg->GetItemMoney( pos ) > HERO->GetMoney() ) 
	{
		// MsgBox ó�� (�������� �����ϴ�.) 355
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(324) );
		pDlg->SetData( NULL );
		pDlg->SetDisable( FALSE );
		return;
	}
	if( GAMEIN->GetInventoryDialog()->GetBlankNum() == 0 )
	{
		// MsgBox ó�� (�κ��� �����ִ�.) 354
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(474) );
		pDlg->SetData( NULL );
		pDlg->SetDisable( FALSE );
		return;
	}
	if( pItem->GetDBId() != dwCheckDBIdx )
	{
		// ���� �ȸ� �������̴�. 358
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(478) );
		pDlg->SetData( NULL );
		pDlg->SetDisable( FALSE );
		return;
	}
	/*
	ITEMBASE base;
	base.dwDBIdx = pItem->GetDBId();
	base.wIconIdx = pItem->GetData();
	if( ITEMMGR->IsDupItem(base.wIconIdx) )
	base.Durability = 1;

	m_BuyMsg.Category = MP_STREETSTALL;
	m_BuyMsg.Protocol = MP_STREETSTALL_BUYITEM_SYN;
	m_BuyMsg.dwObjectID = HEROID;
	m_BuyMsg.StallOwnerID = pDlg->GetStallOwnerId();
	m_BuyMsg.StallPos = pos;
	m_BuyMsg.ItemInfo = base;
	m_BuyMsg.dwBuyPrice = pDlg->GetItemMoney( pos );
	*/
	cMsgBox* pBox = WINDOWMGR->MsgBox( MBI_STREETBUYMSG, MBT_YESNO, CHATMGR->GetChatMsg(342), pItem->GetItemName(), AddComma( pDlg->GetItemMoney( pos ) ) );
}

void CStreetStallManager::BuyItemDur( POSTYPE pos, DWORD dwNum )
{
	CStreetStall* pDlg = GAMEIN->GetStreetStallDialog();
	if( pDlg == NULL )				return;
	if( pDlg->IsActive() == FALSE ) return;

	DWORD dwCheckDBIdx = pDlg->GetCheckDBIdx();

	//KES pos �� dwNum�� ����ϰ� ����.
	//	m_posWantBuy = pos;
	m_dwWantBuyNum = dwNum;

	//CExchangeItem* pItem = (CExchangeItem*)pDlg->GetCurSelectedItem();

	CExchangeItem* pItem = (CExchangeItem*)pDlg->GetItem( (POSTYPE)pos );
	if( pItem == NULL )
	{
		pDlg->SetData( NULL );
		pDlg->SetDisable( FALSE );
		return;
	}

	if( pDlg->GetItemMoney( pos )*dwNum > HERO->GetMoney() ) 
	{
		// MsgBox ó�� (�������� �����ϴ�.) 355
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(324) );
		pDlg->SetData( NULL );
		pDlg->SetDisable( FALSE );
		return;
	}
	if( GAMEIN->GetInventoryDialog()->GetBlankNum() == 0 )
	{
		// MsgBox ó�� (�κ��� �����ִ�.)354
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(474) );
		pDlg->SetData( NULL );
		pDlg->SetDisable( FALSE );
		return;
	}
	if( pItem->GetDBId() != dwCheckDBIdx )
	{
		// ���� �ȸ� �������̴�. 358
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(478) );
		pDlg->SetData( NULL );
		pDlg->SetDisable( FALSE );
		return;
	}
	/*	
	ITEMBASE base;
	base.dwDBIdx = pItem->GetDBId();
	base.wIconIdx = pItem->GetData();
	base.Durability = dwNum;			// �� ����..

	m_BuyMsg.Category = MP_STREETSTALL;
	m_BuyMsg.Protocol = MP_STREETSTALL_BUYITEM_SYN;
	m_BuyMsg.dwObjectID = HEROID;
	m_BuyMsg.StallOwnerID = pDlg->GetStallOwnerId();
	m_BuyMsg.StallPos = pos;
	m_BuyMsg.ItemInfo = base;
	m_BuyMsg.dwBuyPrice = pDlg->GetItemMoney( pos );
	*/
	char temp[32];
	SafeStrCpy( temp, AddComma( pDlg->GetItemMoney( pos ) ), 32 );
	cMsgBox* pBox = WINDOWMGR->MsgBox( MBI_STREETBUYMSG, MBT_YESNO, CHATMGR->GetChatMsg(341), pItem->GetItemName(), dwNum, temp, AddComma( pDlg->GetItemMoney( pos )*dwNum) );
}

void CStreetStallManager::ItemStatus( CExchangeItem* pItem, BOOL bLock )
{
	//	CExchangeItem* pItem = (CExchangeItem*)GAMEIN->GetStreetStallDialog()->GetCurSelectedItem();
	if( !pItem ) return;

	//	POSTYPE pos = GAMEIN->GetStreetStallDialog()->GetCurSelectedItemNum();
	POSTYPE pos = pItem->GetPosition();

	GAMEIN->GetStreetStallDialog()->SetDisable( TRUE );

	MSG_STREETSTALL_ITEMSTATUS msg;
	msg.Category = MP_STREETSTALL;
	msg.dwObjectID = HEROID;

	if( bLock )
	{
		msg.Protocol		= MP_STREETSTALL_LOCKITEM_SYN;
		msg.wAbsPosition	= pos;
		msg.ItemInfo		= pItem->GetLinkItem()->GetItemBaseInfo();
		msg.dwMoney			= GAMEIN->GetStreetStallDialog()->GetItemMoney( pos );
	}
	else
	{
		msg.Protocol = MP_STREETSTALL_UNLOCKITEM_SYN;
		msg.wAbsPosition = pos;
		msg.ItemInfo = pItem->GetLinkItem()->GetItemBaseInfo();
		msg.dwMoney = GAMEIN->GetStreetStallDialog()->GetItemMoney( pos );
	}

	NETWORK->Send( &msg, sizeof(MSG_STREETSTALL_ITEMSTATUS) );
}

void CStreetStallManager::ItemStatus( POSTYPE pos, BOOL bLock )
{
	//	CExchangeItem* pItem = (CExchangeItem*)GAMEIN->GetStreetStallDialog()->GetItem(pos);
	CExchangeItem* pItem;

	pItem = (CExchangeItem*)GAMEIN->GetStreetStallDialog()->GetItem(pos);

	if( !pItem ) return;

	MSG_STREETSTALL_ITEMSTATUS msg;
	msg.Category = MP_STREETSTALL;
	msg.dwObjectID = HEROID;

	if( bLock )
	{
		msg.Protocol = MP_STREETSTALL_LOCKITEM_SYN;
		msg.wAbsPosition = pos;
		msg.ItemInfo = pItem->GetLinkItem()->GetItemBaseInfo();
		//		msg.dwMoney = GAMEIN->GetStreetStallDialog()->GetItemMoney( pos );

		msg.dwMoney = GAMEIN->GetStreetStallDialog()->GetItemMoney( pos );
	}
	else
	{
		msg.Protocol = MP_STREETSTALL_UNLOCKITEM_SYN;
		msg.wAbsPosition = pos;
		msg.ItemInfo = pItem->GetLinkItem()->GetItemBaseInfo();
		msg.dwMoney = 0;
	}

	NETWORK->Send( &msg, sizeof(MSG_STREETSTALL_ITEMSTATUS) );
}

/*
void CStreetStallManager::ChangeDialogState()
{
CStreetStall* pStall = GAMEIN->GetStreetStallDialog();
DWORD state = pStall->GetDlgState();

if( !m_bOpenMsgBox ) return;

if( state == eSDS_NOT_OPENED )
{
if( !WINDOWMGR->MsgBox( MBI_MAKESTREETSTALL, MBT_YESNO, CHATMGR->GetChatMsg(340) ) )
return;

m_bOpenMsgBox = FALSE;
}

if( state == eSDS_OPENED )
{
if( !pStall->IsActive() )
GAMEIN->GetStreetStallDialog()->ShowSellStall();
}
}
*/
void CStreetStallManager::ChangeDialogState()
{
	switch(m_wStallKind)
	{
	case eSK_NULL:
		{
			GAMEIN->GetStallKindSelectDialog()->Show();

			m_bOpenMsgBox = FALSE;
		}
		return;
	case eSK_SELL:
		{
			CStreetStall* pStall = GAMEIN->GetStreetStallDialog();
			DWORD state = pStall->GetDlgState();

			if( !m_bOpenMsgBox ) return;

			if( state == eSDS_NOT_OPENED )
			{
				GAMEIN->GetStallKindSelectDialog()->Show();

				m_bOpenMsgBox = FALSE;
			}

			if( state == eSDS_OPENED )
			{
				if( !pStall->IsActive() )
					GAMEIN->GetStreetStallDialog()->ShowSellStall();
			}
		}
		return;
	case eSK_BUY:
		{
			CStreetBuyStall* pBuyStall = GAMEIN->GetStreetBuyStallDialog();
			DWORD state = pBuyStall->GetDlgState();

			if( !m_bOpenMsgBox ) return;

			if( state == eBSDS_NOT_OPENED )
			{
				GAMEIN->GetStallKindSelectDialog()->Show();

				m_bOpenMsgBox = FALSE;
			}

			if( state == eBSDS_OPENED )
			{
				if( !pBuyStall->IsActive() )
					GAMEIN->GetStreetBuyStallDialog()->ShowBuyStall();
			}
		}
		return;
	default:
		return;
	}
}

void CStreetStallManager::LinkBuyItem( STREETSTALL_INFO* pStallInfo )
{
	CStreetStall* pDlg = GAMEIN->GetStreetStallDialog();
	int i;

	CAddableInfoIterator iter(&pStallInfo->AddableInfo);
	BYTE AddInfoKind;
	while((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
	{
		switch(AddInfoKind)
		{
		case(CAddableInfoList::ItemOption):
			{
				ITEM_OPTION OptionInfo[SLOT_STREETSTALL_NUM];
				iter.GetInfoData(&OptionInfo);
				
				for( WORD i = 0; i < pStallInfo->count; ++i )
				{
					const ITEM_OPTION& option = OptionInfo[ i ];

					ITEMMGR->AddOption( option );
				}
			}
			break;
		}
		iter.ShiftToNextData();
	}

	for( i=0;i<SLOT_STREETSTALL_NUM;++i)
	{
		const STREETSTALLITEM& stallItem = pStallInfo->Item[ i ];

		if( stallItem.Fill )
		{
			char buf[32] = {0,};
			char buf2[32] = {0,};

			CExchangeItem* pExItem = new CExchangeItem;
			cImage tmpImage;
			pExItem->Init( 0, 0, DEFAULT_ICONSIZE, DEFAULT_ICONSIZE, 
				ITEMMGR->GetIconImage( stallItem.wIconIdx, &tmpImage ), m_IconIndexCreator.GenerateIndex() );
			pExItem->SetData( stallItem.wIconIdx );
			pExItem->SetDBId( stallItem.dwDBIdx );
			pExItem->SetDurability( stallItem.Durability );

			// 071215 LYW --- StreetStallManager : ���ο��� ����.
			if( stallItem.nSeal  == eITEM_TYPE_SEAL )
			{
				pExItem->SetSeal( TRUE ) ;
			}
			else
			{
				pExItem->SetSeal( FALSE ) ;
			}

			pExItem->SetItemParam( stallItem.ItemParam );

			{
				ITEMMGR->AddToolTip( pExItem );
				/*const ITEM_OPTION* pOption = ITEMMGR->GetOption( stallItem.wIconIdx, stallItem.Durability ) ;

				ITEMMGR->AddToolTip( pExItem, pOption, stallItem.dwDBIdx ) ;*/
			}

			//��������
			if( stallItem.Locked  )
			{
				if( ITEMMGR->IsDupItem(stallItem.wIconIdx) )
				{
					wsprintf( buf, CHATMGR->GetChatMsg(30), AddComma( stallItem.money ) );
					wsprintf( buf2, CHATMGR->GetChatMsg(504), AddComma( stallItem.Durability * stallItem.money ) );
				}
				else
					wsprintf( buf, CHATMGR->GetChatMsg(35), AddComma( stallItem.money ) );
			}
			else
				wsprintf( buf, CHATMGR->GetChatMsg(503) );
			pExItem->AddToolTipLine( "" );

			pExItem->AddToolTipLine( buf, TTTC_BUYPRICE );
			if(buf2[0])
				pExItem->AddToolTipLine( buf2, TTTC_BUYPRICE );

			pExItem->SetLock( !stallItem.Locked );
			pExItem->SetMovable( FALSE );

			// 071127 LYW --- StreetStallManager : ���� ������ ��ũ ó��.
            ITEM_INFO* pInfo = NULL ;
			pInfo = ITEMMGR->GetItemInfo(stallItem.wIconIdx) ;            

			ASSERT(pInfo) ;

			if( pInfo->wSeal != eITEM_TYPE_SEAL_NORMAL )
			{
				pExItem->SetSeal(TRUE) ;
			}

			int pos = pDlg->AddItem( pExItem );
			if( pos == -1 ) 
			{
				SAFE_DELETE(pExItem);
				continue;
			}

			WINDOWMGR->AddWindow( pExItem );
			pExItem->SetPosition( pos );

			pDlg->RegistMoney( i, stallItem.money );

			m_nLinkedItemNum++;
		}
	}

	pDlg->EditTitle( pStallInfo->Title );
	pDlg->SetStallOwnerId( pStallInfo->StallOwnerID );

	// 071012 LYW --- StreetStallManager : Add owner name to stall window.		// �Ǹ����� �̸��� �����ϴ� ó�� �߰�.
	cStatic* pOwnerTitle = NULL ;												// static �����͸� �����ϰ� null ó���� �Ѵ�.
	pOwnerTitle = pDlg->GetOwnerTitle() ;										// ����â�� owner title static ������ �޴´�.

	if( pOwnerTitle )															// owner title static �� ������ ��ȿ�ϸ�,
	{
		CPlayer* pOwner = NULL ;												// �Ǹ����� ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
		pOwner = (CPlayer*)OBJECTMGR->GetObject( pStallInfo->StallOwnerID );	// ���� �������� owner ���̵�� �Ǹ��� ������ �޴´�.

		if( pOwner )															// owner�� ������ ��ȿ�ϸ�,
		{
			char tempBuf[32] = {0, } ;											// title�� ������ �ӽ� ���۸� �����Ѵ�.
			//sprintf(tempBuf, "%s���� �ǸŻ���", pOwner->GetObjectName()) ;		// title�� �����Ѵ�.
			sprintf(tempBuf, CHATMGR->GetChatMsg(1473), pOwner->GetObjectName()) ;		// title�� �����Ѵ�.
			pOwnerTitle->SetStaticText(tempBuf) ;								// owner title�� �����Ѵ�.
		}
		else																	// owner�� ������ ��ȿ���� ������,
		{
			pOwnerTitle->SetStaticText(RESRCMGR->GetMsg(TEXTNUM_OWNERTITLE)) ;	// ������ �Է����� �����Ѵ�.
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// ���� ������ �Խ�Ʈ�� ���� ���� ������ �޾� ���� �Լ�
void CStreetStallManager::LinkSellItem( STREETSTALL_INFO* pStallInfo )
{
	CStreetBuyStall* pDlg = GAMEIN->GetStreetBuyStallDialog();
	char buf[32] = {0,};
	char buf2[32] = {0,};
	int i;
	ITEMBASE ItemInfo;

	memset(&ItemInfo, 0, sizeof(ITEMBASE));

	for( i=0;i<SLOT_STREETSTALL_NUM;++i)
	{
		if( pStallInfo->Item[i].Fill )
		{
			CBuyItem* pBuyItem = new CBuyItem;

			cImage tmpImage;

			ItemInfo.wIconIdx = pStallInfo->Item[i].wIconIdx;
			ItemInfo.ItemParam = pStallInfo->Item[i].ItemParam;
			ItemInfo.nSealed = pStallInfo->Item[i].nSeal;

			pBuyItem->Init( 0, 0, DEFAULT_ICONSIZE, DEFAULT_ICONSIZE, 
				ITEMMGR->GetIconImage( pStallInfo->Item[i].wIconIdx, &tmpImage ), m_IconIndexCreator.GenerateIndex() );

			pBuyItem->InitItem( ItemInfo, pStallInfo->Item[i].wVolume, pStallInfo->Item[i].money );
			pBuyItem->SetData( pStallInfo->Item[i].wIconIdx );

			//ITEMMGR->AddToolTip( pBuyItem, 0, 0 );
			ITEMMGR->AddToolTip( pBuyItem );

			char buf[32];
			char buf2[32] = {0,};
			char buf3[32] = {0,};

			if(pStallInfo->Item[i].wVolume > 1)
			{
				wsprintf( buf, CHATMGR->GetChatMsg(30), AddComma( pStallInfo->Item[i].money ) );
				wsprintf( buf2, CHATMGR->GetChatMsg(504), AddComma( pStallInfo->Item[i].money * pStallInfo->Item[i].wVolume ));
				wsprintf( buf3, CHATMGR->GetChatMsg(1427), AddComma( pStallInfo->Item[i].wVolume ) );
			}
			else
				wsprintf( buf, CHATMGR->GetChatMsg(35), AddComma( pStallInfo->Item[i].money ) );	
			pBuyItem->AddToolTipLine( "" );

			if( buf3[0] )
			{
				pBuyItem->AddToolTipLine( buf3, TTTC_BUYPRICE );
			}

			pBuyItem->AddToolTipLine( buf, TTTC_BUYPRICE );
			if(buf2[0])
				pBuyItem->AddToolTipLine( buf2, TTTC_BUYPRICE );

			pBuyItem->SetMovable( FALSE );
			pBuyItem->SetLock(pStallInfo->Item[i].Locked);

			if( !pDlg->AddItem( i, pBuyItem ) ) 
			{
				SAFE_DELETE(pBuyItem);
				continue;
			}

			WINDOWMGR->AddWindow( pBuyItem );

			pDlg->RegistMoney( i, pStallInfo->Item[i].money );

			m_nLinkedItemNum++;
		}
	}

	pDlg->EditTitle( pStallInfo->Title );
	pDlg->SetStallOwnerId( pStallInfo->StallOwnerID );
	pDlg->SetCurSelectedItemNum( -1 );
}
//////////////////////////////////////////////////////////////////////////

void CStreetStallManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{	
	if(HERO == NULL)
		return;
	switch( Protocol ) 
	{
	case MP_STREETSTALL_OPEN_ACK:							StreetStall_Open_Ack( pMsg ) ;							break;
	case MP_STREETSTALL_OPEN_NACK:							StreetStall_Open_Nack() ;								break;
	case MP_STREETSTALL_CLOSE_ACK:							StreetStall_Close_Ack()  ;								break;
	case MP_STREETSTALL_CLOSE_NACK:																					break;
	case MP_STREETSTALL_FAKEREGISTITEM_ACK:					StreetStall_Fakeregistitem_Ack( pMsg ) ;				break;
	case MP_STREETSTALL_FAKEREGISTITEM_NACK:				StreetStall_Fakeregistitem_Nack( pMsg ) ;				break;
	case MP_STREETSTALL_FAKEREGISTITEM:						StreetStall_Fakeregistitem( pMsg ) ;					break;
	case MP_STREETSTALL_FAKEREGISTBUYITEM_ACK:				StreetStall_Fakeregistbuyitem_Ack( pMsg ) ;				break;
	case MP_STREETSTALL_FAKEREGISTBUYITEM_NACK:				StreetStall_Fakeregistbuyitem_Nack( pMsg ) ;			break;
	case MP_STREETSTALL_FAKEREGISTBUYITEM:					StreetStall_Fakeregistbuyitem( pMsg ) ;					break;
	case MP_STREETSTALL_LOCKITEM_ACK:						StreetStall_Lockitem_Ack( pMsg ) ;						break;
	case MP_STREETSTALL_LOCKITEM_NACK:						StreetStall_Lockitem_Nack() ;							break;
	case MP_STREETSTALL_LOCKITEM:							StreetStall_Lockitem( pMsg ) ;							break;
	case MP_STREETSTALL_UNLOCKITEM_ACK:						StreetStall_Unlockitem_Ack( pMsg ) ;					break;
	case MP_STREETSTALL_UNLOCKITEM_NACK:					StreetStall_Unlockitem_Nack() ;							break;
	case MP_STREETSTALL_UNLOCKITEM:							StreetStall_Unlockitem( pMsg ) ;						break;
	case MP_STREETSTALL_EDITTITLE_ACK:						StreetStall_Edittitle_Ack( pMsg ) ;						break;
	case MP_STREETSTALL_EDITTITLE_NACK:						StreetStall_Edittitle_Nack() ;							break;
	case MP_STREETSTALL_EDITTITLE:							StreetStall_Edittitle( pMsg ) ;							break;
	case MP_STREETSTALL_BUYITEM_ACK:						StreetStall_Buyitem_Ack( pMsg ) ;						break;
	case MP_STREETSTALL_DELETEITEM_ACK:						StreetStall_Deleteitem_Ack( pMsg ) ;					break;
	case MP_STREETSTALL_DELETEITEM_NACK:																			break;
	case MP_STREETSTALL_DELETEITEM:							StreetStall_Deleteitem( pMsg ) ;						break;
	case MP_STREETSTALL_BUYITEM_NACK:						StreetStall_Buyitem_Nack( pMsg ) ;						break;
	case MP_STREETSTALL_SELLITEM_ACK:						StreetStall_Sellitem_Ack( pMsg ) ;						break;
	case MP_STREETSTALL_SELLITEM_NACK:						StreetStall_Sellitem_Nack( pMsg ) ;						break;
	case MP_STREETSTALL_SELLITEM:							StreetStall_Sellitem( pMsg ) ;							break;
	case MP_STREETSTALL_SELLITEM_ERROR:						StreetStall_Sellitem_error( pMsg ) ;					break;
	case MP_STREETSTALL_BUYITEM:							StreetStall_Buyitem( pMsg ) ;							break;
	case MP_STREETSTALL_BUYITEM_ERROR:						StreetStall_Buyitem_error( pMsg ) ;						break;
	case MP_STREETSTALL_UPDATEITEM:							StreetStall_Updateitem( pMsg ) ;						break;
	case MP_STREETSTALL_GUESTIN_ACK:						StreetStall_Guestin_Ack( pMsg ) ;						break;
	case MP_STREETSTALL_GUESTIN_NACK:						StreetStall_Guestin_Nack() ;							break;
	case MP_STREETSTALL_GUESTOUT_ACK:						StreetStall_Guestout_Ack() ;							break;
	case MP_STREETSTALL_GUESTOUT_NACK:						StreetStall_Guestout_Nack() ;							break;
	case MP_STREETSTALL_START:								StreetStall_Start( pMsg ) ;								break;
	case MP_STREETSTALL_END:								StreetStall_End( pMsg ) ;								break;
	case MP_STREETSTALL_CLOSE:								StreetStall_Close( pMsg ) ;								break;
	case MP_STREETSTALL_UPDATE:								StreetStall_Update( pMsg ) ;							break;
	case MP_STREETSTALL_UPDATEEND:							StreetStall_UpdateEnd() ;								break;
	case MP_STREETSTALL_UPDATE_ACK:							StreetStall_Update_Ack( pMsg ) ;						break;
	case MP_STREETSTALL_UPDATE_NACK:						StreetStall_Update_Nack() ;								break;
	case MP_STREETSTALL_UPDATEEND_ACK:																				break;
	case MP_STREETSTALL_UPDATEEND_NACK:																				break;
	case MP_STREETSTALL_MESSAGE:							StreetStall_Message( pMsg ) ;							break;
	}	
}


void CStreetStallManager::SendBuyItemMsg()
{
	CStreetStall* pDlg	= GAMEIN->GetStreetStallDialog();
	if( pDlg == NULL )				return;
	if( pDlg->IsActive() == FALSE ) return;

	DWORD dwCheckDBIdx	= pDlg->GetCheckDBIdx();
	DURTYPE dwNum			= m_dwWantBuyNum;

	CExchangeItem* pItem = pDlg->FindItem( dwCheckDBIdx );
	//	CExchangeItem* pItem = (CExchangeItem*)pDlg->GetItem( (POSTYPE)pos );

	if( pItem == NULL )
	{
		pDlg->SetData( NULL );
		pDlg->SetDisable( FALSE );
		return;
	}

	POSTYPE pos			= pItem->GetPosition();

	if( pDlg->GetItemMoney( pos )*dwNum > HERO->GetMoney() ) 
	{
		// MsgBox ó�� (�������� �����ϴ�.) 355
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(324) );
		pDlg->SetData( NULL );
		pDlg->SetDisable( FALSE );
		return;
	}
	if( GAMEIN->GetInventoryDialog()->GetBlankNum() == 0 )
	{
		// MsgBox ó�� (�κ��� �����ִ�.)354
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(474) );
		pDlg->SetData( NULL );
		pDlg->SetDisable( FALSE );
		return;
	}
	if( pItem->GetDBId() != dwCheckDBIdx )
	{
		// ���� �ȸ� �������̴�. 358
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(478) );
		pDlg->SetData( NULL );
		pDlg->SetDisable( FALSE );
		return;
	}

	ITEMBASE base;
	base.dwDBIdx = pItem->GetDBId();
	// 070430 LYW --- StreetStallmanager : Modified word to dword item index.
	//base.wIconIdx = (WORD)pItem->GetData();
	base.wIconIdx = pItem->GetData();
	base.Durability = dwNum;			// �� ����..

	// RaMa - Param	
	//	base.ItemParam = pItem->GetItemParam();
	//	base.ItemParam ^= ITEM_PARAM_SEAL;	
	//	base.ItemParam = 0;

	STREETSTALL_BUYINFO BuyMsg;

	BuyMsg.Category = MP_STREETSTALL;
	BuyMsg.Protocol = MP_STREETSTALL_BUYITEM_SYN;
	BuyMsg.dwObjectID = HEROID;
	BuyMsg.StallOwnerID = pDlg->GetStallOwnerId();
	BuyMsg.StallPos = pos;
	BuyMsg.ItemInfo = base;
	BuyMsg.dwBuyPrice = pDlg->GetItemMoney( pos );


	//	if( m_BuyMsg.Protocol == 0 )
	//	{
	//		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(358) );
	//		return;
	//	}

	NETWORK->Send( &BuyMsg, sizeof(STREETSTALL_BUYINFO) );
	//	m_BuyMsg.Protocol = 0;

	//KES 040801 �κ��丮 ���ɱ�
	ITEMMGR->SetDisableDialog( TRUE, eItemTable_Inventory );
	ITEMMGR->SetDisableDialog( TRUE, eItemTable_ShopInven );
}

BOOL CStreetStallManager::SendSellItemMsg()
{
	CStreetBuyStall* pDlg	= GAMEIN->GetStreetBuyStallDialog();
	if( pDlg == NULL )				return FALSE;
	if( pDlg->IsActive() == FALSE ) return FALSE;

	DURTYPE dwNum = m_dwWantSellNum;

	CBuyItem* pBuyItem = (CBuyItem*)pDlg->GetCurSelectedItem();
	if( !pBuyItem )
		return FALSE;

	CItem* pItem = pDlg->GetSellItem();

	POSTYPE pos = pDlg->GetCurSelectedItemNum();

	if( pItem == NULL )
	{
		pDlg->SetData( NULL );
		pDlg->SetDisable( FALSE );
		return FALSE;
	}

	ITEMBASE base;

	memset(&base, 0, sizeof(ITEMBASE));

	base.dwDBIdx = pItem->GetDBIdx();
	// 070430 LYW --- StreetStallmanager : Modified word to dword item index.
	base.wIconIdx = (DWORD)pItem->GetData();
	base.Durability = dwNum;
	base.Position = pItem->GetPosition();
	base.ItemParam = pItem->GetItemParam();
	base.nSealed = pItem->GetItemBaseInfo().nSealed;

	STREETSTALL_BUYINFO SellMsg;

	SellMsg.Category = MP_STREETSTALL;
	SellMsg.Protocol = MP_STREETSTALL_SELLITEM_SYN;
	SellMsg.dwObjectID = HEROID;
	SellMsg.StallOwnerID = pDlg->GetStallOwnerId();
	SellMsg.StallPos = pos;
	SellMsg.ItemInfo = base;
	SellMsg.dwBuyPrice = pDlg->GetItemMoney( pos );

	NETWORK->Send( &SellMsg, sizeof(STREETSTALL_BUYINFO) );

	//KES 040801 �κ��丮 ���ɱ�
	ITEMMGR->SetDisableDialog( TRUE, eItemTable_Inventory );
	ITEMMGR->SetDisableDialog( TRUE, eItemTable_ShopInven );

	pDlg->DelSellItem();

	return TRUE;
}


void CStreetStallManager::StreetStall_Open_Ack( void* pMsg ) 
{
	MSG_STREETSTALL_TITLE* msg = (MSG_STREETSTALL_TITLE*)pMsg;

	ToggleHero_StreetStallMode( TRUE );
	//			GAMEIN->GetStreetStallDialog()->RegistTitle( msg->Title, TRUE );
	//			HERO->SetState( eObjectState_StreetStall_Owner );

	GAMEIN->GetInventoryDialog()->SetActive(TRUE);
	//			GAMEIN->GetStreetStallDialog()->ShowSellStall();

	m_wStallKind = msg->StallKind;	// ���� ����

	// 071012 LYW --- StreetStallManager : Modified.						// ����â�� owner�� �̸��� �߰��ϴ� �۾������� ����.
	if(m_wStallKind == eSK_SELL)											// �Ǹ� ������ ��, 
	{
		CStreetStall* pStallDlg = NULL ;									// ����â�� ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
		pStallDlg = GAMEIN->GetStreetStallDialog() ;						// ����â�� ������ �޴´�.

		if( pStallDlg )														// ���� â ������ ��ȿ�ϸ�,
		{
			pStallDlg->RegistTitle( msg->Title, TRUE );						// Ÿ��Ʋ�� �߰��Ѵ�.
			pStallDlg->ShowSellStall();										// �Ǹų����� �����ش�.

			cStatic* pOwnerTitle = NULL ;									// owner title�� ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
			pOwnerTitle = pStallDlg->GetOwnerTitle() ;						// owner title�� ������ �޴´�.

			if( pOwnerTitle )												// owner title�� ������ ��ȿ�ϸ�,
			{
				pOwnerTitle->SetStaticText(RESRCMGR->GetMsg(TEXTNUM_OWNERTITLE)) ;	// ������ �Է����� �����Ѵ�.
			}
		}
	}
	else if(m_wStallKind == eSK_BUY)
	{
		GAMEIN->GetStreetBuyStallDialog()->RegistTitle( msg->Title, TRUE );
		GAMEIN->GetStreetBuyStallDialog()->ShowBuyStall();
	}

	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(463) );
}


void CStreetStallManager::StreetStall_Open_Nack() 
{
	//			GAMEIN->GetStreetStallDialog()->RegistTitle( NULL, FALSE );

	if(m_wStallKind == eSK_SELL)
		GAMEIN->GetStreetStallDialog()->RegistTitle( NULL, FALSE );
	else if(m_wStallKind == eSK_BUY)
		GAMEIN->GetStreetBuyStallDialog()->RegistTitle( NULL, FALSE );

	//			HERO->SetState( eObjectState_None );
	//050324 ���¸� �̸� ������ ���� ���� �ǵ���
	if( HERO->GetState() == eObjectState_StreetStall_Owner )
		OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_StreetStall_Owner );

	//---
	m_bOpenMsgBox = TRUE;
	m_wStallKind = eSK_NULL;	// ���� ���� �ʱ�ȭ
}


void CStreetStallManager::StreetStall_Close_Ack() 
{
	//			GAMEIN->GetStreetStallDialog()->OnCloseStall();

	if(m_wStallKind == eSK_SELL)
	{
		GAMEIN->GetStreetStallDialog()->OnCloseStall();
		HERO->ShowStreetStallTitle( FALSE, NULL );
	}
	else if(m_wStallKind == eSK_BUY)
	{
		GAMEIN->GetStreetBuyStallDialog()->OnCloseStall();
		HERO->ShowStreetBuyStallTitle( FALSE, NULL );
	}

	GAMEIN->GetInventoryDialog()->SetActive(FALSE);

	ToggleHero_StreetStallMode( FALSE );

	m_nLinkedItemNum = 0;

	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(464) );
	m_bOpenMsgBox = TRUE;
	m_wStallKind = eSK_NULL;	// ���� ���� �ʱ�ȭ
}


//void CStreetStallManager::StreetStall_Fakeregistitem_Ack( void* pMsg ) 
//{
//	MSG_LINKITEMEX* msg = (MSG_LINKITEMEX*)pMsg;
//
//	CItem* pItem = (CItem*)GAMEIN->GetInventoryDialog()->GetItemForPos( msg->ItemInfo.Position );
//	if( !pItem )
//	{
//		ASSERT(0);
//		return;
//	}
//	GAMEIN->GetStreetStallDialog()->SetDisable( FALSE );
//	if( LinkItem( pItem, &msg->ItemInfo ) )
//		GAMEIN->GetStreetStallDialog()->ChangeItemStatus( &msg->ItemInfo, msg->dwMoney, FALSE, 1 );	//TRUE
//}

void CStreetStallManager::StreetStall_Fakeregistitem_Ack( void* pMsg ) 
{
	if( !pMsg ) return ;

	CInventoryExDialog* pInvenDlg = NULL ;
	pInvenDlg = GAMEIN->GetInventoryDialog() ;

	if( !pInvenDlg ) return ;

	CStreetStall* pStreetStallDlg = NULL ;
	pStreetStallDlg = GAMEIN->GetStreetStallDialog() ;

	if( !pStreetStallDlg ) return ;

	MSG_LINKITEMEX* msg = (MSG_LINKITEMEX*)pMsg;

	CItem* pItem = (CItem*)pInvenDlg->GetItemForPos( msg->ItemInfo.Position );

	if( !pItem )
	{
		ASSERT(0);
		return;
	}

	pItem->SetZeroCount(TRUE) ;

	pStreetStallDlg->SetDisable( FALSE );

	if( LinkItem( pItem, &msg->ItemInfo ) )
	{
		pStreetStallDlg->ChangeItemStatus( &msg->ItemInfo, msg->dwMoney, FALSE, 1 );
	}
}


void CStreetStallManager::StreetStall_Fakeregistitem_Nack( void* pMsg ) 
{
	MSG_DWORD2* msg = (MSG_DWORD2*)pMsg;

	--m_nLinkedItemNum; 

	CItem* pItem = (CItem*)GAMEIN->GetInventoryDialog()->GetItemForPos( (POSTYPE)msg->dwData1 );
	if(!pItem)
	{
		ASSERT(0);
		return;
	}

	// 071125 LYW --- StreetStallManager : ���� ���� �� ������ �Ǹ� ���� ó�� �߰�.
	const ITEMBASE& itemBase = pItem->GetItemBaseInfo() ;

	if( itemBase.nSealed == eITEM_TYPE_UNSEAL )
	{
		WINDOWMGR->MsgBox(MBI_SEALITEM_STREETSTALL, MBT_OK, CHATMGR->GetChatMsg(1175));
	}
	else
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(469) );
	}

	if( msg->dwData2 != 1 ) // �̹� ��ϵǾ� �ִٴ� ���� �޼����� �ƴϸ�..
	{
		pItem->SetLock( FALSE );
		//---KES StreetStall Fix 071020
		GAMEIN->GetStreetStallDialog()->SetData(NULL);
		//-----------------------------
	}

	GAMEIN->GetStreetStallDialog()->SetDisable( FALSE );
}


void CStreetStallManager::StreetStall_Fakeregistitem( void* pMsg ) 
{
	if( OBJECTSTATEMGR->GetObjectState(HERO) != eObjectState_StreetStall_Guest) return;

	SEND_LINKITEM_TOTALINFO* msg = (SEND_LINKITEM_TOTALINFO*)pMsg;

	if( msg->ItemInfo.wIconIdx == 0 )
	{
		ASSERT(msg->ItemInfo.wIconIdx);
		return;
	}

	ITEMMGR->AddOption(msg->sOptionInfo);
	
	CExchangeItem* pExItem = new CExchangeItem;
	cImage tmpImage;
	pExItem->Init( 0, 0, DEFAULT_ICONSIZE, DEFAULT_ICONSIZE, 
		ITEMMGR->GetIconImage( msg->ItemInfo.wIconIdx, &tmpImage ), m_IconIndexCreator.GenerateIndex() );
	pExItem->SetData( msg->ItemInfo.wIconIdx );
	pExItem->SetDBId( msg->ItemInfo.dwDBIdx);
	pExItem->SetDurability( msg->ItemInfo.Durability );
	pExItem->SetItemParam( msg->ItemInfo.ItemParam );

	// 071215 LYW --- StreetStallManager : ���ο��� ����.
	if( msg->ItemInfo.nSealed  == eITEM_TYPE_SEAL )
	{
		pExItem->SetSeal( TRUE ) ;
	}
	else
	{
		pExItem->SetSeal( FALSE ) ;
	}

	pExItem->SetLock( TRUE );
	pExItem->SetMovable( FALSE );

	//			if( msg->count != 0 ) // �ɼ��� �ִٸ�
	//				ITEMMGR->AddToolTip( pExItem , ITEMMGR->GetOption(msg->ItemInfo.Durability));
	ITEMMGR->AddToolTip( pExItem );

	/*
	char buf[32];
	if( ITEMMGR->IsDupItem(msg->ItemInfo.wIconIdx) )
	wsprintf( buf, CHATMGR->GetChatMsg(199), AddComma( msg->dwMoney ) );
	else
	wsprintf( buf, CHATMGR->GetChatMsg(214), AddComma( msg->dwMoney ) );

	pExItem->AddToolTipLine( buf, TTTC_BUYPRICE );
	*/
	char buf[32];
	wsprintf( buf, CHATMGR->GetChatMsg(503) );

	pExItem->AddToolTipLine( buf, TTTC_BUYPRICE );

	int pos = GAMEIN->GetStreetStallDialog()->AddItem( pExItem );
	if( pos == -1 ) 
	{
		SAFE_DELETE(pExItem);
		return;
	}

	WINDOWMGR->AddWindow( pExItem );
	pExItem->SetPosition( pos );
	GAMEIN->GetStreetStallDialog()->ChangeItemStatus( &msg->ItemInfo, msg->dwMoney, FALSE );
}


void CStreetStallManager::StreetStall_Fakeregistbuyitem_Ack( void* pMsg ) 
{
	MSG_LINKBUYITEMEX* msg = (MSG_LINKBUYITEMEX*)pMsg;

	GAMEIN->GetStreetBuyStallDialog()->SetDisable( FALSE );
	if( LinkItem( msg->wAbsPosition, msg->ItemInfo, msg->wVolume, msg->dwMoney ) )
		GAMEIN->GetStreetBuyStallDialog()->ChangeItemStatus( msg->wAbsPosition, msg->wVolume, msg->dwMoney);
}


void CStreetStallManager::StreetStall_Fakeregistbuyitem_Nack( void* pMsg ) 
{
	MSG_DWORD2* msg = (MSG_DWORD2*)pMsg;

	--m_nLinkedItemNum; 
	switch(msg->dwData2) 
	{
	case 0:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(469) );
		break;
	case 1:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(511) );
		break;
	case 2:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(512) );
		break;
	}


	GAMEIN->GetStreetBuyStallDialog()->SetDisable( FALSE );
}


void CStreetStallManager::StreetStall_Fakeregistbuyitem( void* pMsg ) 
{
	if( OBJECTSTATEMGR->GetObjectState(HERO) != eObjectState_StreetStall_Guest) return;

	SEND_LINKBUYITEM_TOTALINFO* msg = (SEND_LINKBUYITEM_TOTALINFO*)pMsg;

	if( msg->ItemInfo.wIconIdx == 0 )
	{
		ASSERT(msg->ItemInfo.wIconIdx);
		return;
	}

	CBuyItem* pBuyItem = new CBuyItem;
	cImage tmpImage;

	ITEMBASE ItemInfo;

	memset(&ItemInfo, 0, sizeof(ITEMBASE));

	ItemInfo.wIconIdx = msg->ItemInfo.wIconIdx;
	ItemInfo.ItemParam = msg->ItemInfo.ItemParam;
	ItemInfo.nSealed = msg->ItemInfo.nSealed;

	pBuyItem->Init( 0, 0, DEFAULT_ICONSIZE, DEFAULT_ICONSIZE, 
		ITEMMGR->GetIconImage( msg->ItemInfo.wIconIdx, &tmpImage ), m_IconIndexCreator.GenerateIndex() );
	pBuyItem->InitItem(ItemInfo, msg->wVolume, msg->dwMoney);
	pBuyItem->SetData( msg->ItemInfo.wIconIdx );

	pBuyItem->SetMovable( FALSE );
	ITEMMGR->AddToolTip( pBuyItem );

	//������ ��� �Ǿ� �ֵ� �������� �����ϰ� �ٽ� �ø���
	CBuyItem* pItem = (CBuyItem*)GAMEIN->GetStreetBuyStallDialog()->GetItem(msg->wAbsPosition);

	if( !pItem )
		GAMEIN->GetStreetBuyStallDialog()->DeleteItem(msg->wAbsPosition);

	if( !GAMEIN->GetStreetBuyStallDialog()->AddItem( msg->wAbsPosition, pBuyItem ) ) 
	{
		SAFE_DELETE(pBuyItem);
		return;
	}

	WINDOWMGR->AddWindow( pBuyItem );
	GAMEIN->GetStreetBuyStallDialog()->ChangeItemStatus( msg->wAbsPosition, msg->wVolume, msg->dwMoney);
	GAMEIN->GetStreetBuyStallDialog()->SetCurSelectedItemNum(-1);
}


void CStreetStallManager::StreetStall_Lockitem_Ack( void* pMsg )
{
	MSG_STREETSTALL_ITEMSTATUS* msg = (MSG_STREETSTALL_ITEMSTATUS*)pMsg;
	GAMEIN->GetStreetStallDialog()->ChangeItemStatus( &msg->ItemInfo, msg->dwMoney, TRUE );
	GAMEIN->GetStreetStallDialog()->SetDisable( FALSE );
}


void CStreetStallManager::StreetStall_Lockitem_Nack() 
{
	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(470) );
	GAMEIN->GetStreetStallDialog()->SetDisable( FALSE );
}


void CStreetStallManager::StreetStall_Lockitem( void* pMsg ) 
{
	if( OBJECTSTATEMGR->GetObjectState(HERO) != eObjectState_StreetStall_Guest) return;

	MSG_STREETSTALL_ITEMSTATUS* msg = (MSG_STREETSTALL_ITEMSTATUS*)pMsg;
	GAMEIN->GetStreetStallDialog()->ChangeItemStatus( &msg->ItemInfo, msg->dwMoney, TRUE );
}


void CStreetStallManager::StreetStall_Unlockitem_Ack( void* pMsg ) 
{
	MSG_STREETSTALL_ITEMSTATUS* msg = (MSG_STREETSTALL_ITEMSTATUS*)pMsg;
	GAMEIN->GetStreetStallDialog()->ChangeItemStatus( &msg->ItemInfo, msg->dwMoney, FALSE, 2 );
	GAMEIN->GetStreetStallDialog()->SetDisable( FALSE );
}


void CStreetStallManager::StreetStall_Unlockitem_Nack() 
{
	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(471) );
	GAMEIN->GetStreetStallDialog()->SetDisable( FALSE );
}


void CStreetStallManager::StreetStall_Unlockitem( void* pMsg ) 
{
	if( OBJECTSTATEMGR->GetObjectState(HERO) != eObjectState_StreetStall_Guest) return;

	MSG_STREETSTALL_ITEMSTATUS* msg = (MSG_STREETSTALL_ITEMSTATUS*)pMsg;
	GAMEIN->GetStreetStallDialog()->ChangeItemStatus( &msg->ItemInfo, msg->dwMoney, FALSE );
}


void CStreetStallManager::StreetStall_Edittitle_Ack( void* pMsg ) 
{
	MSG_STREETSTALL_TITLE* msg = (MSG_STREETSTALL_TITLE*)pMsg;
	//			GAMEIN->GetStreetStallDialog()->RegistTitle( msg->Title, TRUE );

	if(msg->StallKind == eSK_SELL)
		GAMEIN->GetStreetStallDialog()->RegistTitle( msg->Title, TRUE );
	else if(msg->StallKind == eSK_BUY)
		GAMEIN->GetStreetBuyStallDialog()->RegistTitle( msg->Title, TRUE );

	m_wStallKind = msg->StallKind;
}


void CStreetStallManager::StreetStall_Edittitle_Nack() 
{
	//			GAMEIN->GetStreetStallDialog()->RegistTitle( NULL, FALSE );

	if(m_wStallKind == eSK_SELL)
		GAMEIN->GetStreetStallDialog()->RegistTitle( NULL, FALSE );
	else if(m_wStallKind == eSK_BUY)
		GAMEIN->GetStreetBuyStallDialog()->RegistTitle( NULL, FALSE );
}


void CStreetStallManager::StreetStall_Edittitle( void* pMsg )
{
	//			if( OBJECTSTATEMGR->GetObjectState(HERO) != eObjectState_StreetStall_Guest) return;

	MSG_STREETSTALL_TITLE* msg = (MSG_STREETSTALL_TITLE*)pMsg;

	CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( msg->dwObjectID );

	if(msg->StallKind == eSK_SELL)
		pPlayer->SetStreetStallTitle( msg->Title );
	else if(msg->StallKind == eSK_BUY)
		pPlayer->SetStreetBuyStallTitle( msg->Title );

	if( HERO->GetState() == eObjectState_StreetStall_Guest )
	{
		//				if( GAMEIN->GetStreetStallDialog()->GetStallOwnerId() == msg->dwObjectID )
		//				{
		//					GAMEIN->GetStreetStallDialog()->EditTitle( msg->Title );
		//				}

		if(msg->StallKind == eSK_SELL)
		{
			if( GAMEIN->GetStreetStallDialog()->GetStallOwnerId() == msg->dwObjectID )
			{
				GAMEIN->GetStreetStallDialog()->EditTitle( msg->Title );
			}
		}
		else if(msg->StallKind == eSK_BUY)
		{
			if( GAMEIN->GetStreetBuyStallDialog()->GetStallOwnerId() == msg->dwObjectID )
			{
				GAMEIN->GetStreetBuyStallDialog()->EditTitle( msg->Title );
			}
		}
	}
}


void CStreetStallManager::StreetStall_Buyitem_Ack( void* pMsg ) 
{
	MSG_ITEMEX* msg = (MSG_ITEMEX*)pMsg;

	ITEMMGR->AddOption( msg->sOptionInfo );

	CItem* pItem = ITEMMGR->MakeNewItem(&msg->ItemInfo,"MP_STREETSTALL_BUYITEM_ACK");
	BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(pItem);
	QUICKMGR->RefreshQickItem();
	//			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(341), pItem->GetItemInfo()->ItemName );

	ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );
	ITEMMGR->SetDisableDialog( FALSE, eItemTable_ShopInven );
	GAMEIN->GetStreetStallDialog()->SetDisable( FALSE );
	GAMEIN->GetStreetStallDialog()->SetData(NULL);
}


void CStreetStallManager::StreetStall_Deleteitem_Ack( void* pMsg )
{
	MSG_LINKITEM* msg = (MSG_LINKITEM*)pMsg;

	//			GAMEIN->GetStreetStallDialog()->DeleteItem( &msg->ItemInfo );

	if(m_wStallKind == eSK_SELL)
		GAMEIN->GetStreetStallDialog()->DeleteItem( &msg->ItemInfo );
	else if(m_wStallKind == eSK_BUY)
		GAMEIN->GetStreetBuyStallDialog()->DeleteItem( msg->wAbsPosition );

	--m_nLinkedItemNum;
}


void CStreetStallManager::StreetStall_Deleteitem( void* pMsg ) 
{
	if( OBJECTSTATEMGR->GetObjectState(HERO) != eObjectState_StreetStall_Guest) return;

	MSG_LINKITEM* msg = (MSG_LINKITEM*)pMsg;

	--m_nLinkedItemNum;

	ITEMMGR->RemoveOption(msg->ItemInfo.dwDBIdx);

	if(m_wStallKind == eSK_SELL)
	{
		GAMEIN->GetStreetStallDialog()->DeleteItem( &msg->ItemInfo );
		GAMEIN->GetStreetStallDialog()->SetCurSelectedItemNum(-1);
		GAMEIN->GetStreetStallDialog()->ResetDlgData();
	}
	else if(m_wStallKind == eSK_BUY)
	{
		GAMEIN->GetStreetBuyStallDialog()->DeleteItem( msg->wAbsPosition );
		GAMEIN->GetStreetBuyStallDialog()->SetCurSelectedItemNum(-1);
		GAMEIN->GetStreetBuyStallDialog()->ResetDlgData();
	}
}


void CStreetStallManager::StreetStall_Buyitem_Nack( void* pMsg )
{
	MSG_WORD* msg  = (MSG_WORD*)pMsg;

	switch(msg->wData)
	{
	case NOT_EXIST:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(478) );
		break;
	case NOT_MONEY:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(324) );
		break;
	case NOT_SPACE:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(474) );
		break;
	case NOT_OWNERMONEY:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(484) );
		break;
	}

	ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );
	ITEMMGR->SetDisableDialog( FALSE, eItemTable_ShopInven );
	GAMEIN->GetStreetStallDialog()->SetDisable( FALSE );
	GAMEIN->GetStreetStallDialog()->SetData(NULL);
}


void CStreetStallManager::StreetStall_Sellitem_Ack( void* pMsg ) 
{
	MSG_SELLITEM* msg = (MSG_SELLITEM*)pMsg;

	CItem* pItem = (CItem*)GAMEIN->GetInventoryDialog()->GetItemForPos( msg->ItemInfo.Position );
	if( !pItem )
	{
		ASSERT(0);
		return;
	}

	if( msg->count == 0 )
	{
		CItem* pOutItem = NULL;
		ITEMMGR->DeleteItem( msg->ItemInfo.Position, &pOutItem );
	}
	else 
	{
		pItem->SetDurability( msg->count );
		pItem->SetLock( FALSE );
	}

	ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );
	ITEMMGR->SetDisableDialog( FALSE, eItemTable_ShopInven );
	GAMEIN->GetStreetBuyStallDialog()->SetDisable( FALSE );
	GAMEIN->GetStreetBuyStallDialog()->SetData(NULL);
}


void CStreetStallManager::StreetStall_Sellitem_Nack( void* pMsg ) 
{
	MSG_WORD2* msg  = (MSG_WORD2*)pMsg;

	switch(msg->wData1)
	{
	case NOT_EXIST:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(516) );
		break;
	case NOT_MONEY:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(515) );
		break;
	case NOT_SPACE:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(514) );
		break;
	}

	CItem* pItem = (CItem*)GAMEIN->GetInventoryDialog()->GetItemForPos( msg->wData2 );
	if(pItem)	pItem->SetLock(FALSE);

	ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );
	ITEMMGR->SetDisableDialog( FALSE, eItemTable_ShopInven );
	GAMEIN->GetStreetBuyStallDialog()->SetDisable( FALSE );
	GAMEIN->GetStreetBuyStallDialog()->SetData(NULL);
}


void CStreetStallManager::StreetStall_Sellitem( void* pMsg )
{
	MSG_SELLITEM* msg = (MSG_SELLITEM*)pMsg;

	CItem* pItem = (CItem*)GAMEIN->GetInventoryDialog()->GetItemForPos( msg->ItemInfo.Position );
	if( !pItem )
	{
		ASSERT(0);
		return;
	}

	//			CPlayer* pPlayer = (CPlayer*) OBJECTMGR->GetObject( msg->dwData );
	//			if( pPlayer )
	//				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(342),  pPlayer->GetObjectName(),  pItem->GetItemInfo()->ItemName );

	if( msg->count == 0 )
	{
		CItem* pOutItem = NULL;
		ITEMMGR->DeleteItem( msg->ItemInfo.Position, &pOutItem );

		GAMEIN->GetStreetStallDialog()->DeleteItem( &msg->ItemInfo );
		--m_nLinkedItemNum;

		pItem->SetZeroCount(FALSE) ;
	}
	else //÷���ϴ� ���
	{
		pItem->SetDurability( msg->count );
		GAMEIN->GetStreetStallDialog()->ResetItemInfo( msg->ItemInfo.dwDBIdx, msg->count );
	}
}


void CStreetStallManager::StreetStall_Sellitem_error( void* pMsg )
{
	MSG_WORD* msg = (MSG_WORD*)pMsg;
	ItemStatus(msg->wData, FALSE);
	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(485) );
}


void CStreetStallManager::StreetStall_Buyitem( void* pMsg ) 
{
	MSG_BUYITEM* msg = (MSG_BUYITEM*)pMsg;

	//if( msg->count )
	ITEMMGR->AddOption( msg->sOptionInfo );

	CItem* pItem = ITEMMGR->MakeNewItem(&msg->ItemInfo,"MP_STREETSTALL_BUYITEM");
	//pItem->SetPosition(msg->ItemInfo.Position);
	BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(pItem);
	QUICKMGR->RefreshQickItem();
}


void CStreetStallManager::StreetStall_Buyitem_error( void* pMsg ) 
{
	MSG_WORD* msg = (MSG_WORD*)pMsg;

	switch(msg->wData) 
	{
	case NOT_SPACE:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(474) );
		break;
	case NOT_MONEY:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(324) );
		break;
	}
}


void CStreetStallManager::StreetStall_Updateitem( void* pMsg )
{
	ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );
	ITEMMGR->SetDisableDialog( FALSE, eItemTable_ShopInven );
	if( OBJECTSTATEMGR->GetObjectState(HERO) != eObjectState_StreetStall_Guest 
		&& OBJECTSTATEMGR->GetObjectState(HERO) != eObjectState_StreetStall_Owner ) 
		return;

	MSG_SELLITEM* msg = (MSG_SELLITEM*)pMsg;

	if(m_wStallKind == eSK_SELL)
	{
		CExchangeItem* pExItem = (CExchangeItem*)GAMEIN->GetStreetStallDialog()->GetItem(msg->ItemInfo.dwDBIdx);
		ASSERT( pExItem );
		if( pExItem == NULL ) return;

		pExItem->SetDurability( msg->count );

/*
		BOOL bState = FALSE;

		if( GAMEIN->GetStreetStallDialog()->GetDlgState() == eSDS_BUY ) bState = TRUE;
*/
		//---KES ���� 071205 �� FALSE �ΰ�. �𸣰ڴ�......==a �ϴ� TRUE�� �ٲ㺻��.
		BOOL bState = TRUE;

		GAMEIN->GetStreetStallDialog()->ChangeItemStatus(pExItem->GetPosition(), 
			GAMEIN->GetStreetStallDialog()->GetItemMoney(pExItem->GetPosition()),
			bState);

		GAMEIN->GetStreetStallDialog()->ResetDlgData();
	}
	else if(m_wStallKind == eSK_BUY)
	{
		CBuyItem* pBuyItem = (CBuyItem*)GAMEIN->GetStreetBuyStallDialog()->GetItem((POSTYPE)msg->dwData);
		ASSERT( pBuyItem );
		if( pBuyItem == NULL ) return;

		pBuyItem->SetVolume( (WORD)msg->count );
		GAMEIN->GetStreetBuyStallDialog()->ResetDlgData();
	}
}


void CStreetStallManager::StreetStall_Guestin_Ack( void* pMsg ) 
{
	STREETSTALL_INFO* msg = (STREETSTALL_INFO*)pMsg;

	//			LinkBuyItem( msg );
	//			GAMEIN->GetStreetStallDialog()->ShowBuyStall();

	m_wStallKind = msg->StallKind;

	if(m_wStallKind == eSK_SELL)
	{
		LinkBuyItem( msg );
		GAMEIN->GetStreetStallDialog()->ShowBuyStall();
	}
	else if(m_wStallKind == eSK_BUY)
	{
		LinkSellItem( msg );
		GAMEIN->GetStreetBuyStallDialog()->ShowSellStall();
	}

	// 071012 LYW --- StreetStallManager : Deactive inventory.			// �մ��� �ʼ��� �κ��� ���� �ʿ䰡 �����Ͽ�, ���� ����.
	//GAMEIN->GetInventoryDialog()->SetActive(TRUE);

	if( HERO->GetState() != eObjectState_StreetStall_Guest )
		OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_StreetStall_Guest);

	//??? ��? ��? KES
	CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( msg->StallOwnerID );
	if( pPlayer )
	{
		pPlayer->SetStreetStallTitle( msg->Title );
		if(m_wStallKind == eSK_SELL)
		{
			pPlayer->SetStreetStallTitle( msg->Title );
		}
		else if(m_wStallKind == eSK_BUY)
		{
			pPlayer->SetStreetBuyStallTitle( msg->Title );
		}

	}
}


void CStreetStallManager::StreetStall_Guestin_Nack() 
{
	//KES 040924
	OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_StreetStall_Guest );
}


void CStreetStallManager::StreetStall_Guestout_Ack() 
{
	//			GAMEIN->GetStreetStallDialog()->OnCloseStall(TRUE);	

	if(m_wStallKind == eSK_SELL)
		GAMEIN->GetStreetStallDialog()->OnCloseStall(TRUE);	
	else if(m_wStallKind == eSK_BUY)
		GAMEIN->GetStreetBuyStallDialog()->OnCloseStall(TRUE);	

	GAMEIN->GetInventoryDialog()->SetActive(FALSE);

	OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_StreetStall_Guest);

	m_wStallKind = eSK_NULL;
	m_nLinkedItemNum = 0;
}


void CStreetStallManager::StreetStall_Guestout_Nack()
{
	//KES 040924
	OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_StreetStall_Guest );
}


void CStreetStallManager::StreetStall_Start( void* pMsg )
{
	CObject* pObject;
	MSG_STREETSTALL_TITLE* pmsg = (MSG_STREETSTALL_TITLE*)pMsg;
	pObject = OBJECTMGR->GetObject(pmsg->dwObjectID);
	ASSERT(pObject);
	Toggle_StreetStallMode(pObject, TRUE);

	if(pmsg->StallKind == eSK_SELL)
		((CPlayer*)pObject)->ShowStreetStallTitle( TRUE, pmsg->Title );
	else if(pmsg->StallKind == eSK_BUY)
		((CPlayer*)pObject)->ShowStreetBuyStallTitle( TRUE, pmsg->Title );
}


void CStreetStallManager::StreetStall_End( void* pMsg ) 
{
	CObject* pObject;
	MSG_STREETSTALL_TITLE* pmsg = (MSG_STREETSTALL_TITLE*)pMsg;
	pObject = OBJECTMGR->GetObject(pmsg->dwObjectID);
	ASSERT(pObject);

	if( pObject->GetID() != HERO->GetID() )
		Toggle_StreetStallMode(pObject, FALSE);
	else
	{
		//				GAMEIN->GetStreetStallDialog()->OnCloseStall();

		if(pmsg->StallKind == eSK_SELL)
			GAMEIN->GetStreetStallDialog()->OnCloseStall();
		else if(pmsg->StallKind == eSK_BUY)
			GAMEIN->GetStreetBuyStallDialog()->OnCloseStall();

		m_nLinkedItemNum = 0;
		m_bOpenMsgBox = TRUE;
		m_wStallKind = eSK_NULL;

		GAMEIN->GetInventoryDialog()->SetActive(FALSE);
	}
	if(pmsg->StallKind == eSK_SELL)
		((CPlayer*)pObject)->ShowStreetStallTitle( FALSE, NULL );
	else if(pmsg->StallKind == eSK_BUY)
		((CPlayer*)pObject)->ShowStreetBuyStallTitle( FALSE, NULL );
}


void CStreetStallManager::StreetStall_Close( void* pMsg ) 
{
	if( OBJECTSTATEMGR->GetObjectState(HERO) != eObjectState_StreetStall_Guest) return;

	MSGBASE* msg = (MSGBASE*)pMsg;

	//			CStreetStall* pDlg = GAMEIN->GetStreetStallDialog();
	//			pDlg->OnCloseStall(TRUE);

	if(m_wStallKind == eSK_SELL)
		GAMEIN->GetStreetStallDialog()->OnCloseStall(TRUE);
	else if(m_wStallKind == eSK_BUY)
		GAMEIN->GetStreetBuyStallDialog()->OnCloseStall(TRUE);

	m_nLinkedItemNum = 0;

	GAMEIN->GetInventoryDialog()->SetActive(FALSE);

	OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_StreetStall_Guest);
}


void CStreetStallManager::StreetStall_Update( void* pMsg )
{
	MSG_WORD* msg = (MSG_WORD*)pMsg;

	CBuyItem* pItem = (CBuyItem*)GAMEIN->GetStreetBuyStallDialog()->GetItem( msg->wData );

	if(pItem)
	{
		pItem->SetActive( FALSE );
		pItem->SetLock( TRUE );
	}

	GAMEIN->GetStreetBuyStallDialog()->ResetDlgData();
}


void CStreetStallManager::StreetStall_UpdateEnd() 
{
	for(int i = 0; i < SLOT_STREETBUYSTALL_NUM; i++)
	{
		CBuyItem* pItem = (CBuyItem*)GAMEIN->GetStreetBuyStallDialog()->GetItem( i );

		if(pItem)
		{
			pItem->SetActive( TRUE );
			pItem->SetLock( FALSE );
		}
	}

	GAMEIN->GetStreetBuyStallDialog()->ResetDlgData();
}


void CStreetStallManager::StreetStall_Update_Ack( void* pMsg ) 
{
	MSG_WORD* msg = (MSG_WORD*)pMsg;

	POSTYPE pos = msg->wData;
	BUY_REG_INFO RegInfo = GAMEIN->GetStreetBuyStallDialog()->GetBuyRegInfo(pos);

	GAMEIN->GetBuyRegDialog()->SetRegInfo(RegInfo);
	ITEM_INFO* pItem = GAMEIN->GetBuyRegDialog()->GetSelectItemInfo();

	if(!pItem)
		return;

	char buf[32] = {0,};
	char buf2[32] = {0,};

	SafeStrCpy(buf,AddComma(RegInfo.Money), 32);
	SafeStrCpy(buf2,AddComma(RegInfo.Money * RegInfo.Volume), 32);

	if(RegInfo.Volume > 1)
	{
		WINDOWMGR->MsgBox(MBI_REGEDITMSG, MBT_YESNO, CHATMGR->GetChatMsg(510), 
			pItem->ItemName,
			RegInfo.Volume,
			buf, buf2);

	}
	else
	{
		WINDOWMGR->MsgBox(MBI_REGEDITMSG, MBT_YESNO, CHATMGR->GetChatMsg(509), 
			pItem->ItemName,
			RegInfo.Volume,
			buf);
	}

	GAMEIN->GetBuyRegDialog()->InitRegInfo();
}


void CStreetStallManager::StreetStall_Update_Nack()
{
	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(522) );
}


void CStreetStallManager::StreetStall_Message( void* pMsg )
{
	MSG_STALLMSG* msg = (MSG_STALLMSG*)pMsg;

	ITEM_INFO* pItem = ITEMMGR->GetItemInfo( msg->ItemIdx );

	CPlayer* pPlayer = NULL;

	if( HEROID == msg->SellerID )
	{
		pPlayer = (CPlayer*) OBJECTMGR->GetObject( msg->BuyerID );

		if( pPlayer )
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(519),  pPlayer->GetObjectName(),  pItem->ItemName, msg->count, AddComma(msg->money));
	}
	else if( HEROID == msg->BuyerID )
	{
		pPlayer = (CPlayer*) OBJECTMGR->GetObject( msg->SellerID );

		if( pPlayer )
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(520),  pPlayer->GetObjectName(),  pItem->ItemName, msg->count, AddComma(msg->money));
	}
}






















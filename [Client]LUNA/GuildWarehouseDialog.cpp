#include "stdafx.h"
#include "GuildWarehouseDialog.h"
#include "WindowIDEnum.h"
#include "Interface/cIconGridDialog.h"
#include "Interface/cPushupButton.h"
#include "Interface/cStatic.h"
#include "ItemManager.h"
#include "ChatManager.h"
//#include "GuildManager.h"
#include "Input/UserInput.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "GameIn.h"
//#include "NpcScriptDialog.h"
#include "interface/cWindowManager.h"
//#include "cDivideBox.h"
//#include "InventoryExDialog.h"
//#include "MHMap.h"


// 071208 LYW --- GuildWarehouseDialog : �˸� �޽��� ��¿� ��� �߰�.
#include "cMsgBox.h"

#include "InventoryExDialog.h"



DWORD ConvertTextToValue( const char* text )
{
	char buffer[ MAX_PATH ];
	char* p = buffer;

	while( *text )
	{
		if( ',' != *text )
		{
			*p++ = *text;
		}

		++text;
	}

	*p = 0;

	return strtoul( buffer, 0, 10 );
}



CGuildWarehouseDialog::CGuildWarehouseDialog()
{
	m_type = WT_GUILDWAREHOUSEDLG;
	m_Lock = FALSE;
	m_InitInfo = FALSE;
}


CGuildWarehouseDialog::~CGuildWarehouseDialog()
{}


void CGuildWarehouseDialog::Linking()
{
	m_pGuildMoney = (cStatic *)GetWindowForID(GDW_MONEY);
	ASSERT( m_pGuildMoney );
}

void CGuildWarehouseDialog::SetActive( BOOL val )
{
	CHero* hero = OBJECTMGR->GetHero();

	if( ! hero )
	{
		return;
	}

	const BOOL lastIsActive = m_bActive;

	cTabDialog::SetActive(val);

	if( val )
	{
		OBJECTSTATEMGR->StartObjectState( hero, eObjectState_Deal );

		//if( GetLock() )
		//{
		//	return;
		//}

		//SetLock(TRUE);

		//GUILDMGR->GuildWarehouseInfoSyn(GetCurTabNum());

		SetTabSize( mTabSize );

		// �κ��丮�� �Բ� �߾ӿ� ǥ�ý�Ų��
		{
			cDialog* inventory = WINDOWMGR->GetWindowForID( IN_INVENTORYDLG );
			ASSERT( inventory );

			const DISPLAY_INFO& screen = GAMERESRCMNGR->m_GameDesc.dispInfo;

			const DWORD x = ( screen.dwWidth - m_width - inventory->GetWidth() ) / 2;
			const DWORD y = ( screen.dwHeight - m_height ) / 2;

			SetAbsXY( x, y );

			inventory->SetAbsXY( x + m_width, y );
			inventory->SetActive( TRUE );
		}

		//if(GAMEIN->GetInventoryDialog()->IsActive() == FALSE)
		//{
		//	GAMEIN->GetInventoryDialog()->SetActive(TRUE);
		//}			
	}
	else
	{
		OBJECTSTATEMGR->EndObjectState( hero, eObjectState_Deal );

		//if( ( HERO->GetState() == eObjectState_Deal ) && (GAMEIN->GetNpcScriptDialog()->IsActive() == FALSE))
		//{
		//	OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
		//}

		//GUILDMGR->GuildWarehouseLeave(GetCurTabNum());

		if( lastIsActive )
		{
			MSGBASE message;
			message.Category	= MP_GUILD;
			message.Protocol	= MP_GUILD_CLOSE_WAREHOUSE;
			message.dwObjectID	= HEROID;

			NETWORK->Send( &message, sizeof( message ) );
		}

		// �Ա� â�� ������
		{
			cDialog* dialog = WINDOWMGR->GetWindowForID( DBOX_INPUT_MONEY );

			if( dialog )
			{
				dialog->SetActive(FALSE);

				WINDOWMGR->AddListDestroyWindow( dialog );
			}
		}

		// �κ��丮 â�� ������
		{
			cDialog* dialog = WINDOWMGR->GetWindowForID( IN_INVENTORYDLG );
			ASSERT( dialog );

			dialog->SetActive( FALSE );
		}
	}	
}

//DWORD CGuildWarehouseDialog::ActionEvent(CMouse * mouseInfo)
//{
//	// �������� �޴� ���̸� ��ٸ�. 
//	// ������ �� ������ �� Ǯ�� 
//	// �ٸ� ��ư�� ���õǸ� ���� �� MSG_BYTE�� MP_ITEM_GUILD_WAREHOUSE_LEAVE ���� 
//	// ��ư �������� ������ �����޶�� ��û. �ޱ� ���� �ٸ� �� �� ������ �� 
//	//GUILDMGR->GuildWarehouseInfoSyn();
//	//if(GetLock())
//	//	return NULL;
//	return cTabDialog::ActionEvent( mouseInfo );
//}

void CGuildWarehouseDialog::OnActionEvent(LONG id, void * p, DWORD we)
{
	//switch( id )
	//{
	// 071218 LUJ, ��� â���� �� ����� ��� ����
	//case GDW_PUTINMONEYBTN:
	//	{
	//		cWindow* pInvenDlg		= WINDOWMGR->GetWindowForID( IN_INVENTORYDLG );
	//		cWindow* pGuildMoney	= WINDOWMGR->GetWindowForIDEx( GDW_MONEY );

	//		ASSERT( pInvenDlg && pGuildMoney );

	//		WINDOWMGR->DivideBox(
	//			DBOX_INPUT_MONEY,
	//			(LONG)(pGuildMoney->GetAbsX()-50),
	//			(LONG)(pGuildMoney->GetAbsY()-100),
	//			OnPutInMoneyGuildOk,
	//			NULL,
	//			pInvenDlg,
	//			this,
	//			CHATMGR->GetChatMsg( 40 ) );
	//		break;
	//	}
	//case GDW_PUTOUTMONEYBTN:
	//	{
	//		cWindow* pInvenDlg		= WINDOWMGR->GetWindowForID( IN_INVENTORYDLG );
	//		cWindow* pGuildMoney	= WINDOWMGR->GetWindowForIDEx( GDW_MONEY );

	//		ASSERT( pInvenDlg && pGuildMoney );

	//		WINDOWMGR->DivideBox(
	//			DBOX_INPUT_MONEY,
	//			(LONG)(pGuildMoney->GetAbsX()-50),
	//			(LONG)(pGuildMoney->GetAbsY()-100),
	//			OnPutOutMoneyGuildOk,
	//			NULL,
	//			pInvenDlg,
	//			this,
	//			CHATMGR->GetChatMsg( 41 ) );
	//		break;
	//	}
	//}
}

void CGuildWarehouseDialog::Render()
{
	cDialog::RenderWindow();
	cDialog::RenderComponent();
	cTabDialog::RenderTabComponent();
}


void CGuildWarehouseDialog::Add(cWindow * window)
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


BOOL CGuildWarehouseDialog::FakeMoveIcon(LONG x, LONG y, cIcon * icon)
{
	ASSERT(icon);
	if( m_bDisable )
	{
		return FALSE;
	}
	else if(icon->GetType() == WT_ITEM)
	{
		return FakeMoveItem(x, y, (CItem *)icon);
	}

	return FALSE;
}


BOOL CGuildWarehouseDialog::FakeMoveItem( LONG mouseX, LONG mouseY, CItem * pFromItem )
{
	// 071208 LYW --- GuildWarehouseDialog : ��� â�� ���� ���� ������ �߰��� �� ������ ó��.
	if( !pFromItem ) return FALSE ;

	const ITEMBASE& pInfo = pFromItem->GetItemBaseInfo();

	if( pInfo.nSealed == eITEM_TYPE_UNSEAL )
	{
		WINDOWMGR->MsgBox(MBI_SEALITEM_STORAGE, MBT_OK, CHATMGR->GetChatMsg(1246)) ;
		return FALSE ;
	}


	// changeitem �߿� �̵��ȵǴ°�
	if( !pFromItem->GetItemInfo()->Deposit )
	{
		return FALSE;
	}

	WORD ToPos=0;
	
	if( !GetPositionForXYRef( 0, mouseX, mouseY, ToPos ) )	// ������ġ �Ѿ� ��
		return FALSE;
	if( pFromItem->IsLocked() ) 
		return FALSE;
	CItem * pToItem = GetItemForPos( ToPos );
	if( pToItem )
	{
		// �ڵ� ��ȯ �ȵ�
		return FALSE;
	}

	if(TP_GUILDWAREHOUSE_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_GUILDWAREHOUSE_END)
		return FakeGeneralItemMove( ToPos, pFromItem, pToItem );
	else if(TP_INVENTORY_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < (TP_INVENTORY_END + ( TABCELL_INVENTORY_NUM * HERO->Get_HeroExtendedInvenCount() ))) 
		return FakeGeneralItemMove( ToPos, pFromItem, pToItem );
	else if(TP_WEAR_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_WEAR_END)
		return FakeGeneralItemMove( ToPos, pFromItem, pToItem );

	return FALSE;
}


BOOL CGuildWarehouseDialog::FakeGeneralItemMove( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem )
{
	BYTE FromTable = ITEMMGR->GetTableIdxForAbsPos( pFromItem->GetPosition() );
	if(FromTable >= eItemTable_Max) return FALSE;
	if( pToItem )
	{
		return FALSE;
	}
	
	// 080410 LUJ, ��� �������� â�� ���� ����
	if( eItemTable_Storage	== FromTable	||
		eItemTable_Weared	== FromTable )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 76 ) );
		return FALSE;
	}

	BYTE ToTable = ITEMMGR->GetTableIdxForAbsPos( ToPos );
	if(ToTable >= eItemTable_Max) return FALSE;


	MSG_ITEM_MOVE_SYN msg;
	msg.Category			= MP_ITEM;
	msg.Protocol			= MP_ITEM_GUILD_MOVE_SYN;
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


BOOL CGuildWarehouseDialog::CanBeMoved(CItem* pItem,POSTYPE toPos)
{
	if(pItem == 0)
		return FALSE;
	
	POSTYPE fromPos = pItem->GetPosition();
	
	if(TP_GUILDWAREHOUSE_START<= toPos && toPos < TP_GUILDWAREHOUSE_END)
	{
		return TRUE;
	}
	return FALSE;
}


BOOL CGuildWarehouseDialog::GetPositionForXYRef(BYTE param, LONG x, LONG y, WORD& pos)
{
	WORD position=0;										// ��� ��ġ

	cIconGridDialog * gridDlg = (cIconGridDialog *)GetTabSheet(GetCurTabNum());
	if(gridDlg->GetPositionForXYRef(x, y, position))
	{
		pos = position+TP_GUILDWAREHOUSE_START + TABCELL_GUILDWAREHOUSE_NUM * GetCurTabNum();		// -> ���� ��ġ�� �ٲ�
		return TRUE;
	}
	return FALSE;
}


CItem * CGuildWarehouseDialog::GetItemForPos(POSTYPE absPos)
{
	if(TP_GUILDWAREHOUSE_START <= absPos && absPos < TP_GUILDWAREHOUSE_END)
	{
		int TabIndex = GetCurTabNum();
		cIconGridDialog * Dlg = (cIconGridDialog *)GetTabSheet(TabIndex);
		if(absPos < (TP_GUILDWAREHOUSE_START+TABCELL_GUILDWAREHOUSE_NUM*TabIndex))
			return NULL;
		return (CItem *)Dlg->GetIconForIdx(absPos-TP_GUILDWAREHOUSE_START-TABCELL_GUILDWAREHOUSE_NUM*TabIndex);
	}
	return NULL;
}


BOOL CGuildWarehouseDialog::AddItem( const ITEMBASE* pItemInfo)
{
	if(pItemInfo->dwDBIdx == 0)
	{
		ASSERTMSG(0,"���â��-������ ����ε����� 0. Invalid GuildWarehouse Index.");
		return FALSE;
	}
	CItem* pItem = ITEMMGR->GetItem(pItemInfo->dwDBIdx);
	if(pItem == 0)
		pItem = ITEMMGR->MakeNewItem( pItemInfo,"CGuildWarehouseDialog::AddItem");
	ASSERT(pItem);

	// 071207 LYW --- GuildWarehouseDialog : ��� â�� ������ ���ο��� ����.
	pItem->SetSeal( pItemInfo->nSealed == eITEM_TYPE_SEAL );

	return AddItem( pItem );
}


BOOL CGuildWarehouseDialog::AddItem(CItem* pItem)
{
	ITEMMGR->RefreshItem(pItem);
	POSTYPE relPos = GetRelativePosition(pItem->GetPosition());
	int tabIndex = GetTabIndex( pItem->GetPosition() );
	cIconGridDialog * dlg = (cIconGridDialog *)GetTabSheet(tabIndex);
	return dlg->AddIcon(relPos, pItem);
}


BOOL CGuildWarehouseDialog::DeleteItem(POSTYPE Pos,CItem** ppItem)
{
	int TabIndex = GetTabIndex( Pos );
	cIconGridDialog * Dlg = (cIconGridDialog *)GetTabSheet(TabIndex);
	if(!Dlg->IsAddable(Pos - TP_GUILDWAREHOUSE_START-TABCELL_GUILDWAREHOUSE_NUM*TabIndex))
		return Dlg->DeleteIcon(Pos-TP_GUILDWAREHOUSE_START-TABCELL_GUILDWAREHOUSE_NUM*TabIndex, (cIcon **)ppItem);
	else
		return FALSE;

}


POSTYPE CGuildWarehouseDialog::GetRelativePosition( POSTYPE absPos)
{
	return ( absPos - TP_GUILDWAREHOUSE_START ) % TABCELL_GUILDWAREHOUSE_NUM;
}


WORD CGuildWarehouseDialog::GetTabIndex( POSTYPE absPos )
{
	return ( absPos - TP_GUILDWAREHOUSE_START ) / TABCELL_GUILDWAREHOUSE_NUM;
}


void CGuildWarehouseDialog::SetLock(BOOL val)
{
	m_Lock = val;
}


BOOL CGuildWarehouseDialog::GetLock() const
{
	return m_Lock;
}


void CGuildWarehouseDialog::SetWarehouseInfo(SEND_MUNPA_WAREHOUSE_ITEM_INFO* pInfo)
{
	cIconGridDialog * dlg = (cIconGridDialog *)GetTabSheet(pInfo->bTabNum);

	CItem * pItem = NULL;
	for(int i=0; i<TABCELL_GUILDWAREHOUSE_NUM; ++i)
	{
	//	if(DeleteItem(TP_GUILDWAREHOUSE_START+i+TABCELL_GUILDWAREHOUSE_NUM*GetCurTabNum(), &FromItem);
		
		if(GAMEIN->GetGuildWarehouseDlg()->DeleteItem(TP_GUILDWAREHOUSE_START+i+TABCELL_GUILDWAREHOUSE_NUM*GetCurTabNum(), &pItem) == TRUE)
			ITEMMGR->ItemDelete(pItem);
		if(pInfo->info.WarehouseItem[i].dwDBIdx != 0)
			AddItem(&pInfo->info.WarehouseItem[i]);
	}
	SetMoney(pInfo->info.money);

	SetLock(FALSE);
}


// 071218 LUJ, ��� â�� ����� ��� ����
//void CGuildWarehouseDialog::OnPutInMoneyGuildOk( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
//{
//	CHero* hero = OBJECTMGR->GetHero();
//	ASSERT( hero );
//
//	if( ! param1 )
//	{
//		return;
//	}
//
//	param1 = min( param1, hero->GetMoney() );
//
//	const double MaxMoney	= MAX_INVENTORY_MONEY;
//	cStatic*	money		= ( cStatic* )WINDOWMGR->GetWindowForIDEx( GDW_MONEY );
//	ASSERT( money );
//
//	const DWORD guildMOney	= ConvertTextToValue( money->GetStaticText() );
//	
//	if( MaxMoney < double( param1 ) + double( guildMOney ) )
//	{
//		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1097 ), AddComma( DWORD( MaxMoney ) ) );
//	}
//	else if( hero->GetState() == eObjectState_Deal )
//	{
//		MSG_DWORD msg;
//		msg.Category	= MP_GUILD;
//		msg.Protocol	= MP_GUILD_PUTIN_MONEY_SYN;
//		msg.dwObjectID	= hero->GetID();
//		msg.dwData		= param1;
//
//		NETWORK->Send(&msg, sizeof(msg));
//	}
//}


// 071218 LUJ, ��� â�� ����� ��� ����
//void CGuildWarehouseDialog::OnPutOutMoneyGuildOk( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
//{
//	CHero* hero = OBJECTMGR->GetHero();
//	ASSERT( hero );
//
//	if( ! param1 )
//	{
//		return;
//	}
//	else if( hero->GetState() == eObjectState_Deal )
//	{
//		const double MaxMoney	= MAX_INVENTORY_MONEY;
//
//		cStatic* money = ( cStatic* )WINDOWMGR->GetWindowForIDEx( GDW_MONEY );
//		ASSERT( money );
//
//		const DWORD guildMoney = ConvertTextToValue( money->GetStaticText() );
//		
//		if( MaxMoney < double( hero->GetMoney() ) + double( param1 ) )
//		{
//			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1098 ), AddComma( DWORD( MaxMoney ) ) );
//		}
//		else
//		{
//			MSG_DWORD msg;
//			msg.Category	= MP_GUILD;
//			msg.Protocol	= MP_GUILD_PUTOUT_MONEY_SYN;
//			msg.dwObjectID	= hero->GetID();
//			msg.dwData		= min( param1, guildMoney );
//
//			NETWORK->Send(&msg, sizeof(msg));
//		}
//	}
//}


void CGuildWarehouseDialog::SetMoney(MONEYTYPE money)
{
	m_pGuildMoney->SetStaticText(AddComma(money));
}


void CGuildWarehouseDialog::SetTabSize( USHORT size )
{
	ASSERT( size <= GetTabNum() );

	mTabSize = min( GetTabNum(), size );

	for( BYTE i = 0; i < size; ++i )
	{
		cWindow*		sheet	= GetTabSheet( i );
		cPushupButton*	button	= GetTabBtn( i );

		ASSERT( sheet && button );

		sheet->SetActive( TRUE );
		button->SetActive( TRUE );
	}

	for( USHORT i = size; i < GetTabNum(); ++i )
	{
		cWindow*		sheet	= GetTabSheet( BYTE( i ) );
		cPushupButton*	button	= GetTabBtn( BYTE( i ) );

		ASSERT( sheet && button );

		sheet->SetActive( FALSE );
		button->SetActive( FALSE );
	}
}


void CGuildWarehouseDialog::AddItem( const MSG_GUILD_ITEM* message )
{
	cIconGridDialog* sheet1 = ( cIconGridDialog* )GetTabSheet( 0 );
	ASSERT( sheet1 );

	// Ŭ����
	for( BYTE sheetIndex = 0; sheetIndex < GetTabNum(); ++sheetIndex )
	{
		cIconGridDialog* sheet = ( cIconGridDialog* )GetTabSheet( sheetIndex );
		ASSERT( sheet );

		for( WORD i = 0; i < sheet->GetCellNum(); ++i )
		{
			CItem* item;

			if( DeleteItem( TP_GUILDWAREHOUSE_START + i + sheetIndex * sheet->GetCellNum(), &item ) )
			{
				ITEMMGR->ItemDelete( item );
			}
		}
	}

	for( DWORD i = 0; i < message->mSize; ++i )
	{
		const ITEMBASE& item = message->mItem[ i ];

		if( item.dwDBIdx )
		{
			AddItem( &item );
		}
	}	
}


DWORD CGuildWarehouseDialog::GetMoney() const
{
	return m_pGuildMoney ? m_pGuildMoney->GetStaticValue() : 0;
}

#include "stdafx.h"
#include "DissolveDialog.h"
#include "WindowIDEnum.h"
#include "Interface/cScriptManager.h"
#include "ItemManager.h"
#include "Interface/cIconDialog.h"
#include "Interface/cIconGridDialog.h"
#include "ObjectManager.h"
//#include "GameIn.h"
#include "InventoryExDialog.h"
#include "ChatManager.h"
#include "cWindowManager.h"
//#include "objectstatemanager.h"
//#include "StreetStallManager.h"
#include "cPushupButton.h"
#include "VirtualItem.h"
#include "ItemShow.h"
#include "cResourceManager.h"
#include "ProgressDialog.h"
#include "FishingManager.h"
#include "GameIn.h"
#include "FishingDialog.h"
#include "FishingPointDialog.h"


enum eIconPosition
{
	eIconPositionTarget,	// ���� ���
	eIconPositionSpecial,	// Ư�� ���
};


CDissolveDialog::CDissolveDialog() :
mTabButton1		( 0 ),
mTabButton2		( 0 ),
mTabButton3		( 0 ),
mSpecialIcon	( new CItemShow ),
mBaseDialog		( 0 )
{
	cImage image;
	SCRIPTMGR->GetImage( 0, &image, PFT_HARDPATH );

	// ���� ��� ������
	{
		ITEMBASE base;
		ZeroMemory( &base, sizeof( base ) );

		mItemIcon = new CItem( &base );

		mItemIcon->SetToolTip( "", RGB_HALF( 255, 255, 255), &image );
		mItemIcon->SetMovable(FALSE);
	}

	// Ư�� ��� ������
	{
		mSpecialIcon->SetToolTip( "", RGB_HALF( 255, 255, 255), &image );
		mSpecialIcon->SetMovable(FALSE);
	}
}


CDissolveDialog::~CDissolveDialog()
{
	Release();

	SAFE_DELETE( mItemIcon );
	SAFE_DELETE( mSpecialIcon );
}


void CDissolveDialog::Linking()
{
	Release();

	mBaseDialog			= ( cIconDialog* )GetWindowForID(DIS_BASEICONDLG);

	// �� ��Ʈ �� ��ư ���ε�
	{
		const DWORD tabSize = 3;

		mTabSheet.reserve( tabSize );

		{
			cIconGridDialog* dialog = (cIconGridDialog* )GetWindowForID( DIS_ICONGRIDDLG1 );
			mTabSheet.push_back( dialog );
		}

		{
			cIconGridDialog* dialog = (cIconGridDialog* )GetWindowForID( DIS_ICONGRIDDLG2 );
			mTabSheet.push_back( dialog );
		}

		{
			cIconGridDialog* dialog = (cIconGridDialog* )GetWindowForID( DIS_ICONGRIDDLG3 );
			mTabSheet.push_back( dialog );
		}

		mTabButton1		= ( cPushupButton* )GetWindowForID( DIS_NUMBTN1 );
		mTabButton2		= ( cPushupButton* )GetWindowForID( DIS_NUMBTN2 );
		mTabButton3		= ( cPushupButton* )GetWindowForID( DIS_NUMBTN3 );
	}

	// ������ �ʱ�ȭ
	{
		int size = 0;

		for(	TabSheet::const_iterator it = mTabSheet.begin();
			mTabSheet.end() != it;
			++it )
		{
			cIconGridDialog* dialog = *it;

			ASSERT( dialog && "��ũ��Ʈ�� �� �������� ������ �׸��� â�� ������ ����. �� �����;���" );

			size += dialog->GetCellNum();
		}

		mResultIcon.reserve( size );

		cImage image;
		SCRIPTMGR->GetImage( 0, &image, PFT_HARDPATH );

		while( size-- )
		{
			CItemShow* item = new CItemShow;

			item->SetToolTip( "", RGB_HALF( 255, 255, 255), &image );
			item->SetMovable(FALSE);

			mResultIcon.push_back( item );
		}
	}
}


void CDissolveDialog::OnActionEvent( LONG id, void * p, DWORD we )
{
	switch( id )
	{
	case DIS_OKBTN:
		{
			Submit( TRUE );
			break;
		}
	case DIS_CANCELBTN:
		{
			RemoveItem();
			RemoveResult();
			SetActive( FALSE );
			break;
		}
	}
}


BOOL CDissolveDialog::FakeMoveIcon( LONG x, LONG y, cIcon * icon )
{
	ASSERT( icon );

	if(	WT_ITEM	!= icon->GetType()	||
		icon->IsLocked() )
	{
		return FALSE;
	}

	CItem* item = ( CItem* )icon;

	if( item->IsLocked() )
	{
		return FALSE;
	}

	// �κ��丮�̿ܿ� ������ FALSE
	if( ! ITEMMGR->IsEqualTableIdxForPos( eItemTable_Inventory, item->GetPosition() ) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(787) );
		return FALSE;
	}

	const DissolveScript* script = ITEMMGR->GetDissolveScript( item->GetItemIdx() );
	if( ! script )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(816) );	//"������ �� ���� ������ �Դϴ�."
		return FALSE;
	}

	{
		CHero* hero = OBJECTMGR->GetHero();

		if( script->mLevel > hero->GetLevel() )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 819 ), script->mLevel );
			return FALSE;
		}
	}	

	//if( item->GetDurability() > 1 ) //������ �������� �ȵȴ�.
	//{
	//	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(817) );	//"������ �������� ������ �� �����ϴ�."
	//	return FALSE;		
	//}

	RemoveItem();
	RemoveResult();

	AddItem( item );
	AddResult( item->GetItemIdx() );

	return FALSE;
}


void CDissolveDialog::AddItem( CItem* addedItem )
{
	const DWORD index = addedItem->GetItemIdx();

	ITEMBASE itemBase;

	{
		ZeroMemory( &itemBase, sizeof( itemBase ) );
		itemBase.wIconIdx	= index;
		itemBase.Position	= addedItem->GetPosition();
		itemBase.Durability	= addedItem->GetDurability();
		itemBase.dwDBIdx	= addedItem->GetDBIdx();
	}	

	cImage image;

	mItemIcon->Init(
		0,
		0,
		DEFAULT_ICONSIZE,
		DEFAULT_ICONSIZE,
		ITEMMGR->GetIconImage( index, &image ),
		0 );

	mItemIcon->SetIconType( eIconType_AllItem );
	mItemIcon->SetData( index );
	mItemIcon->SetItemBaseInfo( itemBase );

	ITEMMGR->AddToolTip( mItemIcon );

	if( mBaseDialog->IsAddable( eIconPositionTarget ) )
	{
		mBaseDialog->AddIcon( eIconPositionTarget, mItemIcon );
	}

	if( ITEMMGR->IsDupItem( addedItem->GetItemIdx() ) )
	{
		addedItem->SetDurability( addedItem->GetDurability() - 1 );

		mItemIcon->SetDurability( 1 );
		mItemIcon->SetVisibleDurability( TRUE );
	}
	else
	{
		mItemIcon->SetVisibleDurability( FALSE );
	}

	//addedItem->SetUseParam( 1 );
	addedItem->SetLock( TRUE );
}


void CDissolveDialog::RemoveItem()
{
	if( ! mBaseDialog->IsAddable( 0 ) )
	{
		mBaseDialog->DeleteIcon( 0,  0 );

		CItem* original = ITEMMGR->GetItem( mItemIcon->GetDBIdx() );

		if( original )
		{
			original->SetLock( FALSE );
			//original->SetUseParam( 0 );

			if( ITEMMGR->IsDupItem( original->GetItemIdx() ) )
			{
				original->SetDurability( original->GetDurability() + mItemIcon->GetDurability() );
			}

			// 080108 LUJ, ���¿� ���� �ʱ�ȭ�Ѵ�
			ITEMMGR->RefreshItem( original );
		}
	}
}


void CDissolveDialog::AddResult( DWORD itemIndex )
{
	const DissolveScript* script = ITEMMGR->GetDissolveScript( itemIndex );

	if( ! script )
	{
		return;
	}

	typedef DissolveScript::Result			Result;
	typedef DissolveScript::StaticResult	StaticResult;

	DWORD		index	= 0;
	const DWORD	tabSize	= mTabSheet.size();

	// ���� ��� ǥ��
	for(	StaticResult::const_iterator it = script->mStaticResult.begin();
		script->mStaticResult.end() != it;
		++it, ++index )
	{
		if( mResultIcon.size() <= index )
		{
			ASSERT( 0 );
			break;
		}

		const Result&		result		= *it;
		ASSERT( index < mResultIcon.size() );

		CItemShow*			icon		= mResultIcon[ index ];
		const DWORD			tabNumber	= index / mResultIcon.size() / mTabSheet.size();
		ASSERT( tabNumber < mTabSheet.size() );

		cIconGridDialog*	dialog		= mTabSheet[ tabNumber ];
		const WORD			iconIndex	= WORD( index % dialog->GetCellNum() );

		if( dialog->AddIcon( iconIndex, icon ) )
		{
			icon->SetData( result.mItemIndex );
			icon->Init( result.mItemIndex, result.mQuantity );
		}
		else
		{
			ASSERT( 0 );
			return;
		}

		ITEMMGR->AddToolTip( icon );
	}

	// �� �ʱ�ȭ
	{
		mTabSheet[ 0 ]->SetCurSelCellPos( -1 );
	}

	typedef DissolveScript::DynamicResult	DynamicResult;

	if( ! script->mDynamicResult.empty() )
	{
		if( mResultIcon.size() <= index )
		{
			ASSERT( 0 );
			return;
		}

		if( mBaseDialog->AddIcon( eIconPositionSpecial, mSpecialIcon ) )
		{
			// ������ ����
			{
				cImage image;

				SCRIPTMGR->GetImage( 143, &image, PFT_ITEMPATH );
				mSpecialIcon->SetBasicImage( &image );
			}

			// ��� ����
			{
				cImage image;

				SCRIPTMGR->GetImage( 0, &image, PFT_HARDPATH );
				mSpecialIcon->SetToolTip( "", RGBA_MAKE(255, 255, 255, 255), &image, TTCLR_ITEM_CANEQUIP );
			}			

			// ������ �Ŵ���..
			for(	DynamicResult::const_iterator it = script->mDynamicResult.begin();
				script->mDynamicResult.end() != it;
				++it )
			{
				const Result&		result	= it->second;
				const ITEM_INFO*	info	= ITEMMGR->GetItemInfo( result.mItemIndex );
				ASSERT( info );

				mSpecialIcon->AddToolTipLine( info->ItemName );
			}

			mSpecialIcon->AddToolTipLine( "" );
			mSpecialIcon->AddToolTipLine( RESRCMGR->GetMsg( 631 ), TTTC_DESCRIPTION );
		}
		else
		{
			ASSERT( 0 );
			return;
		}
	}
}


void CDissolveDialog::RemoveResult()
{
	for(  DWORD i = 0 ; i < mTabSheet.size() ; ++i )
	{
		cIconGridDialog* dialog = mTabSheet[ i ];
		ASSERT( dialog );

		for( int iconIndex = 0; iconIndex < dialog->GetCellNum(); ++iconIndex )
		{
			dialog->DeleteIcon( iconIndex, 0 );
		}
	}

	mBaseDialog->DeleteIcon( eIconPositionSpecial, 0 );
}


void CDissolveDialog::Submit( BOOL isCheck )
{
	CItem*			sourceItem				= ( CItem* )mBaseDialog->GetIconForIdx( 0 );
	const POSTYPE	inventoryStartPosition	= TP_INVENTORY_START;
	const POSTYPE	inventoryEndPosition	= TP_INVENTORY_END + TABCELL_INVENTORY_NUM * HERO->Get_HeroExtendedInvenCount();

	if( ! sourceItem )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1081 ) );
		return;
	}

	ZeroMemory( &mMessage, sizeof( mMessage ) );

	MSG_ITEM_DISSOLVE_SYN::Slot*	result	= mMessage.mSlot;

	//������ �� ������ Ȯ������
	{
		const DissolveScript* script = ITEMMGR->GetDissolveScript( mItemIcon->GetItemIdx() );

		if( ! script )
		{
			ASSERT( 0 );

			ZeroMemory( &mMessage, sizeof( mMessage ) );
			return;
		}

		CInventoryExDialog* inventoryDialog = ( CInventoryExDialog* )WINDOWMGR->GetWindowForID( IN_INVENTORYDLG );
		ASSERT( inventoryDialog );

		typedef MSG_ITEM_DISSOLVE_SYN::Slot	Slot;
		typedef std::map< DWORD, Slot >		InventoryMap;
		InventoryMap						inventoryMap;

		// �κ��丮 ������ �����Ѵ�. ���� ����� ����
		for(
			POSTYPE position = inventoryStartPosition;
			inventoryEndPosition > position;
			++position )
		{
			CItem* item = inventoryDialog->GetItemForPos( position );

			if( ! item )
			{
				continue;
			}

			const BOOL	isDuplicate	= ITEMMGR->IsDupItem( item->GetItemIdx() );

			// 071213, LUJ ���ش���� ���� �ȵȴ�... DB������ ���� ���� ����->��� ���� ������ ó���� ����
			//			������ ó���Ǿ� ���Ե� �� �����Ǵ� ��찡 �ִ�. 
			// ���� ����� ����. 
			// ��ġ�� �������� 0���̸�, �����ϸ鼭 �����ǹǷ� �� ������ ��������
			// ��ġ�� ���� ��쿡�� ���صǸ鼭 �����ǹǷ� ���� �ش� �������� �� ������ �����ص� �ȴ�.
			//if( sourceItem->GetDBIdx() == item->GetDBIdx() &&
			//	( !	isDuplicate	||
			//		isDuplicate && 0 == item->GetDurability() ) )
			//{
			//	continue;
			//}

			Slot& slot	= inventoryMap[ position ];

			// ���Կ� ���� �������� ������ ���� �ش� �������� ���Ƶ�, �ִ� ��ġ�� ���� �ʵ��� �Ѵ�
			// �̷��� ó�� ���� ������, ���� �ִ� ������ �Ѿ �������� �߰ߵ� ��� ��ø ������ ����� ��������.
			const DURTYPE quantity =  min( item->GetDurability(), MAX_YOUNGYAKITEM_DUPNUM );

			slot.mQuantity	= ( isDuplicate ? quantity : 1 );
			slot.mItemIndex	= item->GetItemIdx();
			slot.mPosition	= position;			
		}

		// ���� ���� ��� ���� ������ �˻��Ѵ�.
		for(
			DissolveScript::StaticResult::const_iterator it = script->mStaticResult.begin();
			script->mStaticResult.end() != it;
			++it )
		{
			const DissolveScript::Result& dissolveResult = *it;

			if( ! ITEMMGR->GetItemInfo( dissolveResult.mItemIndex ) )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, "error: script for dissolving includes invalid item" );

				ZeroMemory( &mMessage, sizeof( mMessage ) );
				return;
			}

			const BOOL	isDuplicate		= ITEMMGR->IsDupItem( dissolveResult.mItemIndex );
			DWORD		dissolveSize	= dissolveResult.mQuantity;

			// �������� �������� ������ �κ��丮 ���Կ� ��ġ�� �õ�����
			if( isDuplicate )
			{
				for(
					InventoryMap::iterator inner = inventoryMap.begin();
					inventoryMap.end() != inner && dissolveSize;
					++inner )
				{
					Slot& slot = inner->second;

					// 071213 LUJ, ���� �� ���� ���Կ� �������� ����/������ ��찡 �ִ�. �̶� DB�� ���� �ɸ� ���
					//				����->���� ������ ó������ �ʰ�, ����->���� ������ ó���Ǿ� �������� �������
					//				��찡 �����. ���� ���� ��� ������ ��꿡�� �����Ѵ�.
					if( slot.mItemIndex != dissolveResult.mItemIndex ||
						slot.mPosition	== sourceItem->GetPosition() )
					{
						continue;
					}

					const DWORD enableSize = MAX_YOUNGYAKITEM_DUPNUM - slot.mQuantity;

					if( ! enableSize )
					{
						continue;
					}

					result->mItemIndex	= slot.mItemIndex;
					result->mPosition	= slot.mPosition;
					result->mQuantity	= min( enableSize, dissolveSize );

					dissolveSize		-= result->mQuantity;

					// �ٸ� ��ᵵ �� ������ ����ؾ��ϹǷ�, �κ��丮�� ���Ž�������.
					slot.mQuantity		+= result->mQuantity;

					++result;
				}
			}

			// �� ������ ���ų���
			for(
				POSTYPE position = inventoryStartPosition;
				inventoryEndPosition > position;
				++position )
			{
				if( ! dissolveSize )
				{
					break;
				}
				else if( inventoryMap.end() != inventoryMap.find( position ) )
				{
					continue;
				}

				Slot& slot = inventoryMap[ position ];

				slot.mItemIndex	= dissolveResult.mItemIndex;
				slot.mPosition	= position;

				if( isDuplicate )
				{
					slot.mQuantity	= min( MAX_YOUNGYAKITEM_DUPNUM, dissolveSize );
				}
				else
				{
					slot.mQuantity	= 1;
				}

				dissolveSize	-= slot.mQuantity;

				*result++ = slot;
			}

			// ���� ����
			if( dissolveSize )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 818 ) );

				ZeroMemory( &mMessage, sizeof( mMessage ) );
				return;
			}
		}
	}

	//mIsDissolving = TRUE;

	SetDisable( TRUE );

	mMessage.Category				= MP_ITEM;
	mMessage.Protocol				= MP_ITEM_DISSOLVE_SYN;
	mMessage.dwObjectID				= HEROID;
	mMessage.mSourceItemPosition	= sourceItem->GetPosition();
	mMessage.mSize					= result - mMessage.mSlot;

	// ���� ǥ�ð� ���� �� ������ �۾��� ��û�� ���̴�
	if( isCheck )
	{
		CProgressDialog* dialog = ( CProgressDialog* )WINDOWMGR->GetWindowForID( PROGRESS_DIALOG );
		ASSERT( dialog );

		dialog->Wait( CProgressDialog::eActionDissolve );
	}
}


void CDissolveDialog::EndDissolution( BOOL bSuccess )
{
	SetDisable( FALSE );

	ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );
}


void CDissolveDialog::SetActiveRecursive( BOOL isActive )
{
	for(	TabSheet::iterator it = mTabSheet.begin();
		mTabSheet.end() != it;
		++it )
	{
		cIconGridDialog* dialog = *it;
		ASSERT( dialog );

		dialog->SetActiveRecursive( isActive );
	}

	ASSERT( mTabButton1 && mTabButton2 && mTabButton3 );

	mTabButton1->SetActive( isActive );
	mTabButton2->SetActive( isActive );
	mTabButton3->SetActive( isActive );

	cDialog::SetActiveRecursive( isActive );
}


void CDissolveDialog::Release()
{
	mTabSheet.clear();

	{
		for(	ResultIcon::iterator it = mResultIcon.begin();
			mResultIcon.end() != it;
			++it )
		{
			CItemShow* item = *it;

			SAFE_DELETE( item );
		}

		mResultIcon.empty();
	}
}


void CDissolveDialog::Add( cWindow* window )
{
	if( window->GetType() == WT_PUSHUPBUTTON )
		AddTabBtn( curIdx1++, (cPushupButton * )window );
	else if( window->GetType() == WT_ICONGRIDDIALOG )
	{
		AddTabSheet( curIdx2++, window );
		((cIconGridDialog*)window)->SetDragOverIconType( WT_ITEM );
	}
	else 
		cDialog::Add(window);
}


void CDissolveDialog::Render()
{
	cDialog::RenderWindow();
	cDialog::RenderComponent();	
	cTabDialog::RenderTabComponent();
}


void CDissolveDialog::SetActive( BOOL isActive )
{
	if( isActive )
	{
		// �ٸ� �۾��� ó�� ���� ��쿡�� â�� ��� �� ����
		{
			cDialog* dialog = WINDOWMGR->GetWindowForID( PROGRESS_DIALOG );
			ASSERT( dialog );

			if( dialog->IsDisable() &&
				dialog->IsActive() )
			{
				CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 1082 ) );
				return;
			}
		}

		// NPC ������ �� ���¿����� �Ұ�. 
		// �Ǹ� ������ �� ���¿��� �� �� ����: �������� ��������, �������� ��� ó���� Ŭ���̾�Ʈ�� ������� �����Ƿ� ��õ������ �����ۿ� ����
		// ���� ���� �õ��� �����ؾ� �Ѵ�.
		// �ŷ� ��/â�� ����� ���� �Ұ���. ���� �������� ����
		{
			cDialog* npcScriptDialog			= WINDOWMGR->GetWindowForID( NSI_SCRIPTDLG );
			cDialog* npcShopDialog				= WINDOWMGR->GetWindowForID( DE_DEALERDLG );
			cDialog* stallShopDialog			= WINDOWMGR->GetWindowForID( SSI_STALLDLG );
			cDialog* exchangeDialog				= WINDOWMGR->GetWindowForID( XCI_DIALOG );
			cDialog* privateWarehouseDialog		= WINDOWMGR->GetWindowForID( PYO_STORAGEDLG );
			cDialog* guildWarehouseDialog		= WINDOWMGR->GetWindowForID( GDW_WAREHOUSEDLG );
			cDialog* itemMallWarehouseDialog	= WINDOWMGR->GetWindowForID( ITMALL_BASEDLG );

			ASSERT( npcScriptDialog && npcShopDialog && stallShopDialog && exchangeDialog );
			ASSERT( privateWarehouseDialog && guildWarehouseDialog && itemMallWarehouseDialog );

			if( npcScriptDialog->IsActive() )
			{
				return;
			}
			else if( npcShopDialog->IsActive() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1077 ) );
				return;
			}
			else if( stallShopDialog->IsActive() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1078 ) );
				return;
			}
			else if( exchangeDialog->IsActive() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1079 ) );
				return;
			}
			// 071211 LUJ â�� ǥ�õ� ��� ��� �Ұ�
			else if(	privateWarehouseDialog->IsActive()	||
				guildWarehouseDialog->IsActive()	||
				itemMallWarehouseDialog->IsActive() )
			{
				CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 1082 ) );
				return;
			}
			// 080429 NYJ --- ������ ���Ұ�
			else if(GAMEIN->GetFishingDlg()->IsPushedStartBtn())
			{
				CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 1082 ) );
				return;
			}
			// 080429 NYJ --- ��������Ʈ��ȯ�� ���Ұ�
			else if(GAMEIN->GetFishingPointDlg()->IsActive() )
			{
				CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 1082 ) );
				return;
			}
		}

		// ��ȭ/��æƮ/����/���� â�� ���ÿ� �ߴ� ���� ���´�
		{
			cDialog* mixDialog			= WINDOWMGR->GetWindowForID( ITMD_MIXDLG );
			cDialog* enchantDialog		= WINDOWMGR->GetWindowForID( ITD_UPGRADEDLG );
			cDialog* reinforceDialog	= WINDOWMGR->GetWindowForID( ITR_REINFORCEDLG );

			ASSERT( mixDialog && enchantDialog && reinforceDialog );

			mixDialog->SetActive( FALSE );
			enchantDialog->SetActive( FALSE );
			reinforceDialog->SetActive( FALSE );
		}

		// �κ��丮�� �Բ� �߾ӿ� ǥ�ý�Ų��
		{
			cDialog* inventory = WINDOWMGR->GetWindowForID( IN_INVENTORYDLG );
			ASSERT( inventory );

			const DISPLAY_INFO& screen	= GAMERESRCMNGR->m_GameDesc.dispInfo;
			const DWORD			space	= 140;
			const DWORD			x		= ( screen.dwWidth - m_width - inventory->GetWidth() - space ) / 2;
			const DWORD			y		= ( screen.dwHeight - max( inventory->GetHeight(), m_height ) ) / 2;

			SetAbsXY( x + space + inventory->GetWidth(), y );

			inventory->SetAbsXY( x, y );
			inventory->SetActive( TRUE );
		}

		if( ! mBaseDialog->IsAddable( 0 ) )
		{
			mBaseDialog->DeleteIcon( 0, 0 );

			CItem* item = ITEMMGR->GetItem( mItemIcon->GetDBIdx() );

			if( ! item )
			{
				return;
			}

			//if( ITEMMGR->IsUseableItem( HERO, item->GetItemInfo() ) )
			//{
			//	item->SetUseParam( 0 );
			//}

			// 080108 LUJ, �÷��̾� ���¿� ���� �ʱ�ȭ�ؾ��Ѵ�
			ITEMMGR->RefreshItem( item );

			item->SetLock( FALSE );

			if( ITEMMGR->IsDupItem( item->GetItemIdx() ) )
			{
				item->SetDurability( item->GetDurability() + mItemIcon->GetDurability() );
			}

			RemoveResult();
		}
	}
	else
	{
		// 071213 LUJ, ���� �۾� ��� ���� ��� �����ؾ��Ѵ�. ���� ��� ������ ��Ұ� ���� ���ÿ� �̷��� ���
		//				���Կ� �ø� �� ��ᰡ �����Ǹ鼭 �������� �߸��ȴ�
		{
			CProgressDialog* dialog = ( CProgressDialog* )WINDOWMGR->GetWindowForID( PROGRESS_DIALOG );
			ASSERT( dialog );

			if(  dialog->IsDisable() )
			{
				return;
			}
		}

		RemoveItem();
		RemoveResult();
	}

	cTabDialog::SetActive( isActive );
}


void CDissolveDialog::Succeed( const MSG_ITEM_DISSOLVE_ACK& message )
{
	typedef std::map< const ITEM_INFO*, DURTYPE >	ResultMap;
	ResultMap										resultMap;

	// ���� ����� �ؽ�Ʈ�� ǥ���Ѵ�
	for( DWORD i = 0; i < message.mSize; ++i )
	{
		const ITEMBASE&		itemBase	= message.mItem[ i ];
		const ITEM_INFO*	itemInfo	= ITEMMGR->GetItemInfo( itemBase.wIconIdx );

		if( ! itemInfo )
		{
			continue;
		}

		DURTYPE quantity;
		{
			if( ITEMMGR->IsDupItem( itemBase.wIconIdx ) )
			{
				// �����ϴ� �������̸�.. ������ �þ ��ŭ�� �ݿ�
				CItem* item = ITEMMGR->GetItem( itemBase.dwDBIdx );

				if( item )
				{
					// ���� ������ �������� ������ �پ��� ���� ���ط� ���� �����̹Ƿ� ����� ǥ�õ� �ʿ� ����
					if( item->GetItemIdx() == itemBase.wIconIdx )
					{
						if( item->GetDurability() > itemBase.Durability )
						{
							continue;
						}

						quantity = abs( int( itemBase.Durability ) - int( item->GetDurability() ) );
					}
					else
					{
						quantity = itemBase.Durability;
					}
				}
				else
				{
					quantity = itemBase.Durability;
				}
			}
			else
			{
				quantity = 1;
			}
		}

		 resultMap[ itemInfo ] += quantity;
	}

	// ���ش�� ������ �����´�
	const char* sourceItemName;
	{
		CItem* item = ITEMMGR->GetItem( message.mSourceItemDbIndex );
		
		if( ! item )
		{
			ASSERT( 0 );
			return ;
		}

		const ITEM_INFO* itemInfo = ITEMMGR->GetItemInfo( item->GetItemIdx() );
		
		if( ! itemInfo )
		{
			ASSERT( 0 );
			return;
		}

		sourceItemName = itemInfo->ItemName;
	}

	for( ResultMap::const_iterator it = resultMap.begin(); resultMap.end() != it; ++it )
	{
		const ITEM_INFO* 	itemInfo	= it->first;
		const DURTYPE		quantity	= it->second;
		
		CHATMGR->AddMsg(
			CTC_OPERATEITEM,
			CHATMGR->GetChatMsg( 821 ),
			//sourceItemName,
			"",
			itemInfo->ItemName,
			quantity );
	}	

	Restore();
}


void CDissolveDialog::Fail()
{
	Restore();
}


void CDissolveDialog::Restore()
{
	//mIsDissolving	= FALSE;

	ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );

	if( HERO->GetState() == eObjectState_Die )
	{
		SetActive( FALSE );
	}

	SetDisable( FALSE );
}


void CDissolveDialog::Send()
{
	Submit( FALSE );

	static const MSG_ITEM_DISSOLVE_SYN emptyMessage;

	if( ! memcmp( &emptyMessage, &mMessage, sizeof( mMessage ) ) )
	{
		return;
	}

	//MSG_ITEM_DISSOLVE_SYN message( mMessage );

	MSG_ITEM_DISSOLVE_SYN2 message;
	{
		ZeroMemory( &message, sizeof( message ) );

		CItem* sourceItem = ( CItem* )mBaseDialog->GetIconForIdx( eIconPositionTarget );

		if( ! sourceItem )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 791 ) );
			return ;
		}

		message.Category			= MP_ITEM;
		message.Protocol			= MP_ITEM_DISSOLVE_SYN;
		message.dwObjectID			= HEROID;
		message.mSourceItemPosition	= sourceItem->GetPosition();
		message.mSourceItemDbIndex	= sourceItem->GetDBIdx();
	}

	NETWORK->Send( &message, sizeof( message ) );

	SetDisable( FALSE );

	RemoveItem();
	RemoveResult();
}

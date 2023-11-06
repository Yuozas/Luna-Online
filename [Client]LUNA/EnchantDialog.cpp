#include "stdafx.h"
#include "EnchantDialog.h"
#include "WindowIDEnum.h"
#include "ChatManager.h"
#include "interface/cIconDialog.h"
#include "Interface/cScriptManager.h"
#include "Interface/cWindowManager.h"
#include "interface/cStatic.h"
#include "InventoryExDialog.h"
#include "Item.h"
#include "VirtualItem.h"
#include "ItemManager.h"
#include "ObjectManager.h"
#include "ProgressDialog.h"
#include "FishingManager.h"
#include "GameIn.h"
#include "FishingDialog.h"
#include "FishingPointDialog.h"


CEnchantDialog::CEnchantDialog() :
mIconDialog( 0 ),
mSourceItem( new CVirtualItem ),
// 080228 LUJ, 일반/보호 모드 표시위한 컨트롤 초기화
mTitle( 0 ),
mProtectTitle( 0 ),
mProtectSymbol( 0 )
{
	cImage image;

	SCRIPTMGR->GetImage( 0, &image, PFT_HARDPATH );

	// 인챈트할 대상 아이템 아이콘
	{
		ITEMBASE base;
		ZeroMemory( &base, sizeof( base ) );

		mMaterialItem = new CItem( &base );

		mMaterialItem->SetToolTip( "", RGB_HALF( 255, 255, 255), &image );
		mMaterialItem->SetMovable( FALSE );
	}

	// 주문서 아이콘
	{
		mSourceItem->SetToolTip( "", RGB_HALF( 255, 255, 255), &image );
		mSourceItem->SetMovable(FALSE);
	}

	// 080228 LUJ, 보호 아이템 정보 초기화
	ZeroMemory( &mProtectionItem, sizeof( mProtectionItem ) );
}


CEnchantDialog::~CEnchantDialog()
{
	SAFE_DELETE( mSourceItem );
	SAFE_DELETE( mMaterialItem );
}


void CEnchantDialog::Linking()
{
	mIconDialog		= ( cIconDialog* )	GetWindowForID( ITD_UPGRADEGRID );
	mResult			= ( cStatic* )		GetWindowForID( ITD_UPGRADE_RESULT );
	
	// 080228 LUJ, 일반/보호 모드 표시 용 인터페이스 링크
	mTitle			= GetWindowForID( ITD_UPGRADE_TITLE );
	mProtectTitle	= GetWindowForID( ITD_UPGRADE_PROTECT_TITLE );
	mProtectSymbol	= GetWindowForID( ITD_UPGRADE_PROTECT_SYMBOL );
}


BOOL CEnchantDialog::FakeMoveIcon(LONG x, LONG y, cIcon * pOrigIcon)
{
	if( WT_ITEM != pOrigIcon->GetType() ||
		pOrigIcon->IsLocked()			||
		m_bDisable )
	{
		return FALSE;
	}

	CItem * pOrigItem = (CItem *)pOrigIcon;

	// 인벤토리의 아이템만 허용된다
	if( ! ITEMMGR->IsEqualTableIdxForPos(eItemTable_Inventory, pOrigItem->GetPosition() ) )
	{
		CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 787 ) );
		return FALSE;
	}

	const DWORD				itemIndex	= pOrigItem->GetItemIdx();
	const ITEM_INFO*		info		= ITEMMGR->GetItemInfo( itemIndex );
	
	// 인챈트 가능한 아이템이 올라오면
	if( info->IsEnchant )
	{
		// 080228 LUJ, 보호 모드일 때 인챈트 가능한 아이템인지 체크한다
		if( mProtectionItem.dwDBIdx )
		{
			const ITEM_OPTION& option = ITEMMGR->GetOption( pOrigItem->GetItemBaseInfo() );

			const CItemManager::EnchantProtection&						protection	= ITEMMGR->GetEnchantProtection();
			CItemManager::EnchantProtection::ItemMap::const_iterator	it			= protection.mItemMap.find( mProtectionItem.wIconIdx );

			if( protection.mItemMap.end() != it )
			{
				const CItemManager::EnchantProtection::Data& data = it->second;

				if( option.mEnchant.mLevel < data.mEnchantLevelRange.mMin ||
					option.mEnchant.mLevel > data.mEnchantLevelRange.mMax )
				{
					CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 1453 ) );
					return FALSE;
				}
				// 080328 LUJ, 보호 가능한 아이템 장착 레벨 체크
				else if(	info->LimitLevel < data.mItemLimitLevelRange.mMin ||
							info->LimitLevel > data.mItemLimitLevelRange.mMax )
				{
					CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 1421 ) );
					return FALSE;
				}
			}
		}		

		CVirtualItem* item = ( CVirtualItem* )mIconDialog->GetIconForIdx( 1 );

		if(		item &&
			!	IsEnchantable(	ITEMMGR->GetItemInfoAbsIn( HERO, pOrigItem->GetPosition() ), 
								ITEMMGR->GetEnchantScript( item->GetItemIdx() ) ) )
		{
			return FALSE;
		}
		else
		{
			if( ! mIconDialog->IsAddable( 0 ) )
			{
				CItem* old = ( CItem* )( mSourceItem->GetLinkItem() );

				if( ! old )
				{
					return FALSE;
				}

				//if( ITEMMGR->IsUseableItem( HERO, old->GetItemInfo() ) )
				//{
				//	old->SetUseParam( 0 );
				//}

				// 080108 LUJ, 플레이어 상태에 따라 초기화해야한다
				ITEMMGR->RefreshItem( old );

				old->SetLock( FALSE );

				mIconDialog->DeleteIcon( 0, 0 );
			}

			// 정보 설정 및 툴팁 달자...
			{
				mSourceItem->SetData( pOrigItem->GetItemIdx() );
				mSourceItem->SetLinkItem( pOrigItem );

				ITEMMGR->AddToolTip( mSourceItem );

				mIconDialog->AddIcon( 0, mSourceItem );
			}	

			//pOrigItem->SetUseParam( 1 );
			pOrigItem->SetLock( TRUE );
		}
		
		return FALSE;
	}
	
	const EnchantScript* script	= ITEMMGR->GetEnchantScript( itemIndex );

	if( ! script )
	{
		CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 906 ) );
		return FALSE;
	}

	{
		CVirtualItem* item = ( CVirtualItem* )mIconDialog->GetIconForIdx( 0 );
		
		if(		item &&
			!	IsEnchantable(	ITEMMGR->GetItemInfoAbsIn( HERO, item->GetSrcPosition() ),
								script ) )
		{
			return FALSE;
		}
	}

	// 이미 주문서 칸에 아이템이 등록되어 있으면 복구
	if( ! mIconDialog->IsAddable( 1 ) )
	{
		CItem* original = ITEMMGR->GetItem( mMaterialItem->GetDBIdx() );

		if( original )
		{
			//if( ITEMMGR->IsUseableItem( HERO, original->GetItemInfo() ) )
			//{
			//	original->SetUseParam( 0 );
			//}

			// 080108 LUJ, 플레이어 상태에 따라 초기화해야한다
			ITEMMGR->RefreshItem( original );

			original->SetLock( FALSE );

			if( ITEMMGR->IsDupItem( original->GetItemIdx() ) )
			{
				const DWORD quantity = original->GetDurability() + mMaterialItem->GetDurability();
				original->SetDurability( quantity );
			}
		}

		mIconDialog->DeleteIcon( 1, 0 );
	}

	// 주문서 등록
	{
		pOrigItem->SetLock( TRUE );
		//pOrigItem->SetUseParam( 1 );

		if( ITEMMGR->IsDupItem( itemIndex ) )
		{
			pOrigItem->SetDurability( ITEMMGR->IsDupItem( itemIndex ) ? pOrigItem->GetDurability() - 1 : 0 );
			pOrigItem->SetVisibleDurability( TRUE );
		}
		else
		{
			pOrigItem->SetVisibleDurability( FALSE );
		}

		{
			ITEMBASE itemBase;
			ZeroMemory( &itemBase, sizeof( itemBase ) );
			itemBase.wIconIdx	= itemIndex;
			itemBase.Position	= pOrigItem->GetPosition();
			itemBase.Durability	= ITEMMGR->IsDupItem( itemIndex ) ? 1 : 0;
			itemBase.dwDBIdx	= pOrigItem->GetDBIdx();

			cImage image;

			mMaterialItem->Init(
				0,
				0,
				DEFAULT_ICONSIZE,
				DEFAULT_ICONSIZE,
				ITEMMGR->GetIconImage( itemIndex, &image ),
				0 );

			mMaterialItem->SetIconType( eIconType_AllItem );
			mMaterialItem->SetData( itemIndex );
			mMaterialItem->SetItemBaseInfo( itemBase );
			mMaterialItem->SetVisibleDurability( ITEMMGR->IsDupItem( itemIndex ) );
			
			ITEMMGR->AddToolTip( mMaterialItem );
			mIconDialog->AddIcon( 1, mMaterialItem );
		}
	}
	
	return FALSE;
}


void CEnchantDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	switch( lId )
	{
	case ITD_UPGRADE_OKBTN:
		{
			if( m_bDisable )
			{
				CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 992 ) );
				break;
			}
			else if( mIconDialog->IsAddable( 0 ) )
			{
				CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 905 ) );
				break;
			}
			else if( mIconDialog->IsAddable( 1 ) )
			{
				CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 906 ) );
				break;
			}

			// 080228 LUJ, 초기화
			ZeroMemory( &mMessage, sizeof( mMessage ) );
			
			mMessage.Category			= MP_ITEM;
			mMessage.Protocol			= MP_ITEM_ENCHANT_SYN;
			mMessage.dwObjectID			= HEROID;
			mMessage.ItemPos			= mSourceItem->GetSrcPosition();
			mMessage.wItemIdx			= mSourceItem->GetSrcItemIdx();
			mMessage.MaterialItemPos	= mMaterialItem->GetPosition();
			mMessage.wMaterialItemIdx	= mMaterialItem->GetItemIdx();

			// 080228 LUJ, 보호 아이템 정보 설정
			if( mProtectionItem.wIconIdx )
			{
				ICONBASE& protectionItem = mMessage.mProtectionItem;

				protectionItem.dwDBIdx	= mProtectionItem.dwDBIdx;
				protectionItem.wIconIdx	= mProtectionItem.wIconIdx;
				protectionItem.Position	= mProtectionItem.Position;
			}

			ITEMMGR->SetDisableDialog( TRUE, eItemTable_Inventory );
			SetDisable( TRUE );

			// 진행 표시가 끝난 후 서버에 작업을 요청할 것이다
			{
				CProgressDialog* dialog = ( CProgressDialog* )WINDOWMGR->GetWindowForID( PROGRESS_DIALOG );
				ASSERT( dialog );

				dialog->Wait( CProgressDialog::eActionEnchant );
			}
			
			break;
		}
	case ITD_UPGRADE_CANCELBTN:
		{
			if( m_bDisable )
			{
				CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 992 ) );
				break;
			}

			SetActive( FALSE );
			break;
		}
	}
}


BOOL CEnchantDialog::IsEnchantable( const ITEMBASE* item, const EnchantScript* script ) const
{
	const DWORD			itemIndex	= item->wIconIdx;
	const ITEM_INFO*	info		= ITEMMGR->GetItemInfo( itemIndex );

	if( !	info							||
		!	info->IsEnchant					||
			ITEMMGR->IsDupItem( itemIndex )	||
		!	script )
	{
		CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 907 ) );
		return FALSE;
	}

	const ITEM_OPTION& option = ITEMMGR->GetOption( *item );

	// 인챈트 최고 레벨에 도달했으면 불가
	if( option.mEnchant.mLevel == script->mEnchantMaxLevel )
	{
		CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 908 ) );
		return FALSE;
	}
	// 아이템이 인챈트 할 수 있는 범위인지 체크
	else if(	script->mItemLevel.mMin > info->LimitLevel ||
				script->mItemLevel.mMax	< info->LimitLevel )
	{
		CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 907 ) );
		return FALSE;
	}
	
	const eEquipSlot slot = eEquipSlot( info->EquipSlot + 1 );

	if( script->mSlot.end() == script->mSlot.find( slot ) )
	{
		CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 907 ) );
		return FALSE;
	}

	return TRUE;
}


void CEnchantDialog::SetActive( BOOL isActive )
{
	// 다른 작업을 처리 중인 경우에는 창을 띄울 수 없다
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

	if( isActive )
	{
		// NPC 상점을 연 상태에서도 불가. 
		// 판매 상점을 연 상태에서 열 수 없음: 아이템이 겹쳐지나, 아이템의 잠금 처리가 클라이언트에 저장되지 않으므로 원천적으로 아이템에 대한
		// 동시 접근 시도를 차단해야 한다.
		// 거래 중/창고 사용일 때는 불가함. 위와 마찬가지 이유
		{
			cDialog* npcScriptDialog	= WINDOWMGR->GetWindowForID( NSI_SCRIPTDLG );
			cDialog* npcShopDialog		= WINDOWMGR->GetWindowForID( DE_DEALERDLG );
			cDialog* stallShopDialog	= WINDOWMGR->GetWindowForID( SSI_STALLDLG );
			cDialog* exchangeDialog		= WINDOWMGR->GetWindowForID( XCI_DIALOG );
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
				CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 1077 ) );
				return;
			}
			else if( stallShopDialog->IsActive() )
			{
				CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 1078 ) );
				return;
			}
			else if( exchangeDialog->IsActive() )
			{
				CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 1079 ) );
				return;
			}
			// 071211 LUJ 창고가 표시된 경우 사용 불가
			else if(	privateWarehouseDialog->IsActive()	||
				guildWarehouseDialog->IsActive()	||
				itemMallWarehouseDialog->IsActive() )
			{
				CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 1082 ) );
				return;
			}
			// 080429 NYJ --- 낚시중 사용불가
			else if(GAMEIN->GetFishingDlg()->IsPushedStartBtn())
			{
				CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 1082 ) );
				return;
			}
			// 080429 NYJ --- 낚시포인트교환중 사용불가
			else if(GAMEIN->GetFishingPointDlg()->IsActive() )
			{
				CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 1082 ) );
				return;
			}
		}

		// 강화/인챈트/조합/분해/노점판매 창이 동시에 뜨는 것을 막는다
		{
			cDialog* mixDialog			= WINDOWMGR->GetWindowForID( ITMD_MIXDLG );
			cDialog* DissoloveDialog	= WINDOWMGR->GetWindowForID( DIS_DISSOLUTIONDLG );
			cDialog* reinforceDialog	= WINDOWMGR->GetWindowForID( ITR_REINFORCEDLG );
			
			ASSERT( mixDialog && DissoloveDialog && reinforceDialog );

			mixDialog->SetActive( FALSE );
			DissoloveDialog->SetActive( FALSE );
			reinforceDialog->SetActive( FALSE );
		}

		// 인벤토리와 함께 중앙에 표시시킨다
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

		// 초기화
		{
			mResult->SetStaticText( "" );
		}

		// 080228 LUJ, 일반 모드 표시
		{
			if( mTitle )
			{
				mTitle->SetActive( TRUE );
			}

			if( mProtectTitle )
			{
				mProtectTitle->SetActive( FALSE );
			}

			if( mProtectSymbol )
			{
				mProtectSymbol->SetActive( FALSE );
			}
		}
	}
	
	{
		if( ! mIconDialog->IsAddable( 0 ) )
		{
			CItem* item = ( CItem* )( mSourceItem->GetLinkItem() );
			ASSERT( item );

			if( ! item )
			{
				return;
			}

			item->SetLock( FALSE );
			
			mIconDialog->DeleteIcon( 0, 0 );
		}

		// 재료 아이템 복구
		if( ! mIconDialog->IsAddable( 1 ) )
		{
			CItem*	original = ITEMMGR->GetItem( mMaterialItem->GetDBIdx() );

			if( original )
			{
				if( ITEMMGR->IsDupItem( mMaterialItem->GetItemIdx() ) )
				{
					original->SetDurability( mMaterialItem->GetDurability() + original->GetDurability() );
				}

				original->SetLock( FALSE );
			}

			mIconDialog	->DeleteIcon( 1, 0 );
		}

		// 080228 LUJ, 보호 아이템을 초기화한다
		{
			CItem* item = ITEMMGR->GetItem( mProtectionItem.dwDBIdx );

			if( item )
			{
				item->SetLock( FALSE );

				ZeroMemory( &mProtectionItem, sizeof( mProtectionItem ) );
			}
		}
	}

	cDialog::SetActive( isActive );
}


// 080228 LUJ, 보호 아이템이 있을 경우의 성공 처리
void CEnchantDialog::Succeed( const MSG_ITEM_ENCHANT_ACK& message )
{
	Restore();

	{
		ITEMMGR->AddOption( message.OptionInfo );
		RemoveMaterial();

		{
			CItem* item = ( CItem* )mSourceItem->GetLinkItem();

			ITEMMGR->AddToolTip( mSourceItem );
			ITEMMGR->AddToolTip( item );

			const ITEM_INFO* info = ITEMMGR->GetItemInfo( item->GetItemIdx() );
			ASSERT( info );

			CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 913 ), info->ItemName );

			mResult->SetStaticText( CHATMGR->GetChatMsg( 974 ) );
		}	
	}

	// 080228 LUJ, 보호 아이템을 사용한 경우 창을 닫는다
	if( mMessage.mProtectionItem.dwDBIdx )
	{
		CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 1452 ) );

		CItem* item = ITEMMGR->GetItem( mMessage.mProtectionItem.dwDBIdx );

		if( item )
		{
			SetActive( FALSE );

			if( item->GetItemInfo()->Stack &&
				1 < item->GetDurability() )
			{
				item->SetDurability( item->GetDurability() - 1 );
			}
			else
			{
				CItem* deletedItem = 0;

				ITEMMGR->DeleteItem( item->GetPosition(), &deletedItem );
			}
		}
	}
}


// 080228 LUJ, 보호 아이템이 있을 경우의 실패 처리 추가
// 080320 LUJ, 인챈트 제외 보호 모드(인챈트 수치가 초기화됨) 처리
void CEnchantDialog::Fail()
{
	Restore();
	RemoveMaterial();
	
	// 080228 LUJ, 보호 아이템을 사용한 경우 창을 닫는다
	if( mMessage.mProtectionItem.dwDBIdx )
	{
		CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 1450 ) );

		CItem* item = ITEMMGR->GetItem( mMessage.mProtectionItem.dwDBIdx );

		if( item )
		{
			SetActive( FALSE );

			if( item->GetItemInfo()->Stack &&
				1 < item->GetDurability() )
			{
				item->SetDurability( item->GetDurability() - 1 );
			}
			else
			{
				CItem* deletedItem = 0;

				ITEMMGR->DeleteItem( item->GetPosition(), &deletedItem );
			}
		}

		// 080320 LUJ,	인챈트 제외 보호 모드때는 옵션이 변경된다. 툴팁을 갱신해주자
		{
			CItem* item = ( CItem* )mSourceItem->GetLinkItem();

			ITEMMGR->AddToolTip( mSourceItem );
			ITEMMGR->AddToolTip( item );
		}
	}
	else
	{
		mIconDialog->DeleteIcon( 0, 0 );

		CItem* deletedItem = 0;

		ITEMMGR->DeleteItem( mSourceItem->GetSrcPosition(), &deletedItem );

		const ITEM_INFO* info = deletedItem->GetItemInfo();

		if( info )
		{
			CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 914 ), info->ItemName );
		}
	}

	mResult->SetStaticText( CHATMGR->GetChatMsg( 975 ) );
}


void CEnchantDialog::Restore()
{
	SetDisable( FALSE );

	ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );
}


void CEnchantDialog::RemoveMaterial()
{
	CInventoryExDialog* inventory = ( CInventoryExDialog* )WINDOWMGR->GetWindowForID( IN_INVENTORYDLG );
	CItem*				deletedItem;
	ASSERT( inventory );

	CItem* item = inventory->GetItemForPos( mMaterialItem->GetPosition() );

	if( ! item )
	{
		ASSERT( 0 && "Why did it deleted already?" );
		return;
	}
	else if( ! item->GetDurability() )
	{
		ITEMMGR->DeleteItem( mMaterialItem->GetPosition(), &deletedItem );
	}

    //item->SetUseParam( 0 );
	item->SetLock( FALSE );

	// 080108 LUJ, 상태에 따라 초기화한다
	ITEMMGR->RefreshItem( item );

	mIconDialog->DeleteIcon( 1, 0 );
}


void CEnchantDialog::Send()
{
	// 080228 LUJ, 보호 아이템 사용 정보가 서버에서 전송하지 않으므로 클라이언트 정보를 활용하도록 복사해서 쓴다
	MSG_ITEM_ENCHANT_SYN message( mMessage );

	NETWORK->Send( &message, sizeof( message ) );
}


// 080228 LUJ, 보호 아이템 설정
void CEnchantDialog::SetProtectedActive( CItem& item )
{
	SetActive( TRUE );

	// 080228 LUJ, 보호 모드 표시
	{
		if( mTitle )
		{
			mTitle->SetActive( FALSE );
		}

		if( mProtectTitle )
		{
			mProtectTitle->SetActive( TRUE );
		}

		if( mProtectSymbol )
		{
			mProtectSymbol->SetActive( TRUE );
		}
	}

	mProtectionItem = item.GetItemBaseInfo();
	item.SetLock( TRUE );
}
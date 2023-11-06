#include "stdafx.h"
#include "MixDialog.h"
#include "ChatManager.h"
#include "interface/cIconGridDialog.h"
#include "WindowIDEnum.h"
#include "Item.h"
#include "itemShow.h"
#include "ItemManager.h"
#include "InventoryExDialog.h"
#include "cIconDialog.h"
#include "interface/cStatic.h"
#include "Interface/cScriptManager.h"
#include "ObjectManager.h"
#include "Interface/cWindowManager.h"
#include "objectstatemanager.h"
#include "StreetStallManager.h"
#include "interface/cListDialog.h"
#include "cResourceManager.h"
#include "cDivideBox.h"
#include "cMsgBox.h"
#include "ProgressDialog.h"
#include "FishingManager.h"
#include "GameIn.h"
#include "FishingDialog.h"
#include "FishingPointDialog.h"


const DWORD DisableColor( RGBA_MAKE( 255, 100, 100, 0 ) );
const DWORD EnableColor( RGBA_MAKE( 255, 255, 255, 0 ) );


CMixDialog::CMixDialog() :
mDescription	( 0 ),
mBaseDialog		( 0 ),
mIconGridDialog	( 0 ),
// 080228 LUJ, 일반/보호 모드 표시위한 컨트롤 초기화
mTitle( 0 ),
mProtectTitle( 0 ),
mProtectSymbol( 0 )
{
	// 080228 LUJ, 보호 아이템 정보 초기화
	ZeroMemory( &mProtectionItem, sizeof( mProtectionItem ) );
}


CMixDialog::~CMixDialog()
{
	// don't call Release() 
	// pre delete item 

	Release();
}


void CMixDialog::SetActiveRecursive(BOOL val)
{
	if( (STREETSTALLMGR->GetStallKind() == eSK_BUY && OBJECTSTATEMGR->GetObjectState(HERO) == eObjectState_StreetStall_Owner) )
	{
		CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg(789) );
		return;
	}
}


void CMixDialog::Linking()
{
	Release();

	mDescription	= ( cListDialog*		)GetWindowForID( ITMD_MIXDESCRIPTION );
	mBaseDialog		= ( cIconDialog*		)GetWindowForID(ITMD_MIXICONDLG);
	mIconGridDialog	= ( cIconGridDialog*	)GetWindowForID(ITMD_MIXICONGRIDDLG);

	// 아이콘을 초기화한다
	{
		cImage imgToolTip;
		SCRIPTMGR->GetImage( 0, &imgToolTip, PFT_HARDPATH );

		// 초기 재료 및 결과 아이콘
		{
			int size = mBaseDialog->GetCellNum();

			mItemIcon.reserve( size );

			ITEMBASE base;
			ZeroMemory( &base, sizeof( base ) );

			while( size-- )
			{
				CItem* item = new CItem( &base );

				item->SetToolTip( "", RGB_HALF( 255, 255, 255), &imgToolTip );
				item->SetMovable(FALSE);

				mItemIcon.push_back( item );
			}
		}

		// 추가 재료 아이콘
		{
			int size = mIconGridDialog->GetCellNum();

			mResultIcon.reserve( size );

			while( size-- )
			{
				CItemShow* item = new CItemShow;

				item->SetToolTip( "", RGB_HALF( 255, 255, 255), &imgToolTip );
				item->SetMovable(FALSE);
				item->SetVisibleDurability( FALSE );

				mResultIcon.push_back( item );
			}
		}
	}

	// 080228 LUJ, 일반/보호 모드 표시위한 컨트롤 링크
	{
		mTitle			= GetWindowForID( ITMD_MIXICONTITLE );
		mProtectTitle	= GetWindowForID( ITMD_MIX_PROTECT_TITLE );
		mProtectSymbol	= GetWindowForID( ITMD_MIX_PROTECT_SYMBOL );
	}
}


BOOL CMixDialog::FakeMoveIcon( LONG x, LONG y, cIcon* icon )
{
	if( !	icon						||
			WT_ITEM != icon->GetType()	||
			icon->IsLocked() )
	{
		return FALSE;
	}

	CItem* item = ( CItem* )icon;

	// 인벤토리이외에 아이템 FALSE
	if( ! ITEMMGR->IsEqualTableIdxForPos( eItemTable_Inventory, item->GetPosition() ) )
	{
		CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 1168 ) );
		return FALSE;
	}
	
	const ITEM_INFO* info = item->GetItemInfo();

	if( ! info )
	{
		return FALSE;
	}

	{
		if( ! ITEMMGR->GetMixScript( item->GetItemIdx() ) )
		{
			CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 782 ) );
			return FALSE;
		}

		// 클리어
		{
			RemoveResult();

			// 등록된 재료가 있으면 복구해야한다
			if( ! mBaseDialog->IsAddable( ePositionSource ) )
			{
				RemoveItem();
			}

			mBaseDialog->DeleteIcon( ePositionResult, 0 );
		}

		item->SetLock( TRUE );

		if( info->Stack )
		{
			item->SetDurability( item->GetDurability() - 1 );
			item->SetVisibleDurability( TRUE );
		}
		else
		{
			item->SetVisibleDurability( FALSE );
		}

		{
			CItem* sourceItem = mItemIcon[ ePositionSource ];
			cImage image;

			sourceItem->Init(
				0,
				0,
				DEFAULT_ICONSIZE,
				DEFAULT_ICONSIZE,
				ITEMMGR->GetIconImage( item->GetItemIdx(), &image ),
				0 );

			sourceItem->SetIconType( eIconType_AllItem );
			sourceItem->SetData( item->GetItemIdx() );
			sourceItem->SetItemBaseInfo( item->GetItemBaseInfo() );
			sourceItem->SetVisibleDurability( info->Stack );
			sourceItem->SetDurability( info->Stack ? 1 : 0 );

			ITEMMGR->AddToolTip( sourceItem );
			mBaseDialog->AddIcon( ePositionSource, sourceItem );
		}

		AddResult( item->GetItemIdx() );
	}

	return FALSE;
}


void CMixDialog::PutDescription( const ItemMixResult* result )
{	
	const ITEM_INFO* info = ITEMMGR->GetItemInfo( result->mItemIndex );
	ASSERT( info );

	//CItem*	pBasicItem = ( CItem* )mBaseDialog->GetIconForIdx(ePositionSource);

	const std::string prefixProduct	( RESRCMGR->GetMsg( 626 ) ); // "제작품목: ";
	const std::string prefixLevel	( RESRCMGR->GetMsg( 627 ) ); // "제한레벨: ";
	const std::string prefixMaterial( RESRCMGR->GetMsg( 628 ) ); // "필요재료: ";
	const std::string prefixMoney	( RESRCMGR->GetMsg( 629 ) ); // "제작비용: ";
	//const std::string prefixSuccess	( "성공확률:%.1f%%" );
	const std::string prefixSuccess	( CHATMGR->GetChatMsg(1464) );
	const std::string postfixList	( RESRCMGR->GetMsg( 630 ) ); // "개";

	CHero*		hero = OBJECTMGR->GetHero();
	const DWORD textColor( RGBA_MAKE( 255, 255, 255, 0 ) );
	char		line[ MAX_TEXT_SIZE ];

	mDescription->RemoveAll();

	// 제작 품목 표시
	{
		sprintf( line, "%s%s", prefixProduct.c_str(), info->ItemName );

		mDescription->AddItemAutoLine( line, ITEMMGR->GetItemNameColor( *info ) );
	}

	// 레벨 표시
	{
		if( result->mRequiredLevel > hero->GetLevel() )
		{
			sprintf( line, "%s%d", prefixLevel.c_str(), result->mRequiredLevel );
			mDescription->AddItemAutoLine( line, DisableColor );
		}
	}

	// 비용 표시
	{
		sprintf( line, "%s%s", prefixMoney.c_str(), AddComma( result->mMoney ) );

		mDescription->AddItemAutoLine( line, ( result->mMoney > hero->GetMoney() ? DisableColor : EnableColor ) );
	}

	// 성공률
	{
		sprintf( line, prefixSuccess.c_str(), result->mSuccessRate * 100.0f );

		mDescription->AddItemAutoLine( line, textColor );
	}

	typedef std::list< std::string >	Description;
	Description							description;

	// 재료 표시
	{
		sprintf( line, "%s", prefixMaterial.c_str() );

		mDescription->AddItemAutoLine( line, textColor );

		std::string tab;

		// 재료 표시의 절반 만큼을 여백으로 한다
		for( int i = strlen( prefixMaterial.c_str() ) / 2; i--; )
		{
			tab += " ";
		}

		CItem* source = mItemIcon[ ePositionSource ];

		const DWORD sourceItemIndex		= source->GetItemIdx();
		const DWORD sourceItemQuantity	= max( 1, source->GetDurability() );

		for(	ItemMixResult::Material::const_iterator it = result->mMaterial.begin();
			result->mMaterial.end() != it;
			++it )
		{
			const DWORD itemIndex	= it->first;
			const DWORD quantity	= it->second;

			const ITEM_INFO* materialInfo = ITEMMGR->GetItemInfo( itemIndex );

			sprintf( line, "%s%s %d%s", tab.c_str(), materialInfo->ItemName, quantity, postfixList.c_str() );

			mDescription->AddItemAutoLine( line, textColor );
		}
	}

	mDescription->SetPosition( 0 );
}


void CMixDialog::AddResult( const DWORD itemIndex )
{
	const MixScript* script = ITEMMGR->GetMixScript( itemIndex );

	if( ! script )
	{
		return;
	}

	CItem*	raw		= mItemIcon[ ePositionSource ];
	DWORD	index	= 0;

	for(	MixScript::const_iterator it = script->begin();
			script->end() != it;
			++it, ++index )
	{
		if( mResultIcon.size() <= index )
		{
			ASSERT( 0 && "재료 개수가 그리드 내 아이콘보다 많음" );
			break;
		}

		CItemShow*				itemShow	= mResultIcon[ index ];
		const ItemMixResult&	result		= *it;
		const DWORD				itemIndex	= result.mItemIndex;

		itemShow->SetData( itemIndex );
		itemShow->Init( itemIndex, 0 );		

		// 레벨 체크하여 조합 불가 아이템은 붉은 기운이 돌게함.
		{
			CHero* hero = OBJECTMGR->GetHero();

			if( hero->GetLevel() < result.mRequiredLevel ||
				hero->GetMoney() < result.mMoney )
			{
				itemShow->SetImageRGB( DisableColor );
			}
			else
			{
				itemShow->SetImageRGB( EnableColor );
			}

		}

		ITEMMGR->AddToolTip( itemShow );

		mIconGridDialog->AddIcon( WORD( index ), itemShow );
	}

	if( 1 < script->size() )
	{
		mDescription->AddItemAutoLine( CHATMGR->GetChatMsg( 788 ), RGBA_MAKE( 255, 255, 255, 0 ) );
	}
	// 조합 결과가 1개일때는 자동 선택
	else if( 1 == script->size() )
	{
		mIconGridDialog->SetCurSelCellPos( 0 );

		OnActionEvent( ITMD_MIXICONGRIDDLG, this, WE_LBTNCLICK );	
	}
	else
	{
		ASSERT( 0 && "It do not need data that have nothing" );
	}
}


void CMixDialog::RemoveResult()
{
	mDescription->RemoveAll();

	for( int i = 0 ; i < mIconGridDialog->GetCellNum() ; ++i )
	{
		mIconGridDialog->DeleteIcon( i, 0 );
	}
}


void CMixDialog::Submit( DURTYPE mixSize, BOOL isCheck )
{
	if( ! mixSize )
	{
		CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 1085 ) );
		return;
	}

	// 기본 아이템 존재?
	CItem* sourceItem = ( CItem* )mBaseDialog->GetIconForIdx(ePositionSource);

	if( ! sourceItem )
	{
		CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg(791) );
		return ;
	}

	const DWORD			sourceItemIndex			= sourceItem->GetItemIdx();
	const MixScript*	script					= ITEMMGR->GetMixScript( sourceItemIndex );
	const POSTYPE		inventoryStartPosition	= TP_INVENTORY_START;
	const POSTYPE		inventoryEndPosition	= TP_INVENTORY_END + TABCELL_INVENTORY_NUM * HERO->Get_HeroExtendedInvenCount();

	// 선택된 것이 없을 경우 처리: 조합 결과가 1 종류일때는 자동으로 처리한다.
	if( -1 == mIconGridDialog->GetCurSelCellPos() )
	{
		if( 1 < script->size() )
		{
			CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg(783) );
			return ;
		}
		else
		{
			mIconGridDialog->SetCurSelCellPos( 0 );
		}
	}

	const SIZE_T			index	= SIZE_T( mIconGridDialog->GetCurSelCellPos() );
	const ItemMixResult*	result	= 0;
	{
		MixScript::const_iterator it = script->begin();

		if( script->size() < index )
		{
			CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 1271 ) );
			return;
		}

		std::advance( it, index );

		if( script->end() == it )
		{
			return;
		}

		result = &( *it );
	}

	CHero* hero = OBJECTMGR->GetHero();
	ASSERT( hero && result );

	if( result->mMoney * mixSize > HERO->GetMoney() )
	{
		CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 324 ) );
		return ;
	}
	else if( hero->GetLevel() < result->mRequiredLevel )
	{
		CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 792 ) );
		return;
	}

	ZeroMemory( &mMessage, sizeof( mMessage ) );

	const DWORD					sourceItemDBIndex	= sourceItem->GetDBIdx();
	MSG_ITEM_MIX_SYN::Material*	material			= mMessage.mMaterial;
	ItemMixResult::Material		requiredMaterial;

	// 필요 개수를 채우자... 
	for(
		ItemMixResult::Material::const_iterator it = result->mMaterial.begin();
		result->mMaterial.end() != it;
		++it )
	{
		const DWORD		itemIndex	= it->first;
		const DURTYPE	quantity	= it->second;

		requiredMaterial[ itemIndex ] = quantity * mixSize;

		if( ! ITEMMGR->GetItemInfo( itemIndex ) )
		{
			CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 1271 ) );

			ZeroMemory( &mMessage, sizeof( mMessage ) );
			return;
		}
	}

	// 등록한 슬롯부터 조사하여 최대한 개수를 채운다.
	{
		CItem* original = ITEMMGR->GetItem( sourceItemDBIndex );

		if( original													&&
			requiredMaterial.end() != requiredMaterial.find( sourceItemIndex ) )
		{
			mMessage.mSourceIndex	= 0;								
			material->mItemIndex	= sourceItemIndex;
			material->mPosition		= original->GetPosition();

			DWORD& needQuantity	= requiredMaterial[ sourceItemIndex ];

			if( ITEMMGR->IsDupItem( sourceItemIndex ) )
			{
				const DWORD quantity	= original->GetDurability() + sourceItem->GetDurability();

				material->mQuantity		= min( quantity, needQuantity );
				needQuantity			-= material->mQuantity;
			}
			else
			{
				// 옵션하고 개수를 저장하는데 이 필드를 같이 쓰고 있다는 사실을 잊지말자. 
				material->mQuantity	= original->GetDurability();

				--needQuantity;
			}

			if( ! needQuantity )
			{
				requiredMaterial.erase( sourceItemIndex );
			}

			++material;
			++mMessage.mSourceSize;
		}
		else
		{
			ASSERT( 0 && "등록된 아이템이 재료에 포함되어 있지 않는 것은 허용되지 않는 상황" );

			ZeroMemory( &mMessage, sizeof( mMessage ) );
			return;
		}
	}

	CInventoryExDialog* inventory = ( CInventoryExDialog* )WINDOWMGR->GetWindowForID( IN_INVENTORYDLG );
	ASSERT( inventory );

	// 재료 정보를 수집한다
	for(
		POSTYPE position = inventoryStartPosition;
		inventoryEndPosition > position;		
		++position )
	{
		if( requiredMaterial.empty() )
		{
			break;
		}

		CItem* item = inventory->GetItemForPos( position );

		if( ! item )
		{
			continue;
		}

		const DWORD	itemIndex	= item->GetItemIdx();
		const BOOL	isDuplicate = ITEMMGR->IsDupItem( itemIndex );
		const DWORD quantity	= ( isDuplicate ? item->GetDurability() : 1 );

		if( 1 > quantity													||
			requiredMaterial.end() == requiredMaterial.find( itemIndex )	||
			item->IsLocked() )
		{
			continue;
		}

		DWORD& remainedQuantity = requiredMaterial[ itemIndex ];

		// 남은 개수에서 현재 슬롯의 개수를 뺀다
		// 남은 개수가 0이하인 경우 슬롯의 개수를 이전 남은 개수로 한다.
		{
			const DWORD previousRemainedQuantity = remainedQuantity;

			remainedQuantity		= max( 0, int( remainedQuantity - quantity ) );

			//material->mPosition		= item->GetPosition();
			material->mPosition		= position;
			material->mItemIndex	= itemIndex;
			material->mQuantity		= ( isDuplicate ? min( previousRemainedQuantity, quantity ) : 1 );

			if( sourceItemDBIndex == item->GetDBIdx() )
			{
				mMessage.mSourceIndex = material - mMessage.mMaterial;

				if( isDuplicate )
				{
					++material->mQuantity;
				}
			}

			++mMessage.mSourceSize;
			++material;
		}

		if( ! remainedQuantity )
		{
			requiredMaterial.erase( itemIndex );
		}			
	}

	// 저장 위치를 계산한다.
	{
		typedef MSG_ITEM_MIX_SYN::Material	Material;
		typedef std::map< DWORD, Material > InventoryMap;
		InventoryMap						inventoryMap;

		// 인벤토리 정보 복사
		for(
			POSTYPE position = inventoryStartPosition;
			inventoryEndPosition > position;
			++position )
		{
			CItem* item = inventory->GetItemForPos( position );

			if( item )
			{
				const BOOL isDuplicate = ITEMMGR->IsDupItem( item->GetItemIdx() );

				Material& slot = inventoryMap[ position ];

				slot.mItemIndex	= item->GetItemIdx();
				slot.mPosition	= position;

				if( isDuplicate )
				{
					// 재료 한개가 조합창에 등록되어있으므로 더해주어야 한다.
					slot.mQuantity	= ( sourceItemDBIndex == item->GetDBIdx() ? item->GetDurability() + 1 : item->GetDurability() );
				}
				else
				{
					slot.mQuantity	= 1;
				}
			}
		}

		// 재료 정보를 조회해가면서 인벤토리 정보를 갱신한다(소요된 재료를 차감한다)
		for( DWORD i = mMessage.mSourceSize; i--; )
		{
			const Material& source		= mMessage.mMaterial[ i ];
			const POSTYPE	position	= source.mPosition;

			if( inventoryMap.end() != inventoryMap.find( position ) )
			{
				Material& slot = inventoryMap[ position ];
				ASSERT( slot.mQuantity >= source.mQuantity );

				slot.mQuantity -= source.mQuantity;

				if( ! slot.mQuantity )
				{
					inventoryMap.erase( position );
				}
			}
		}

		MSG_ITEM_MIX_SYN::Material*	resultSlot			= mMessage.mMaterial + mMessage.mSourceSize;
		int							resultSize			= int( mixSize );
		const BOOL					isResultDuplicate	= ITEMMGR->IsDupItem( result->mItemIndex );

		// 겹치는 아이템이면
		if( isResultDuplicate )
		{
			// 일단 같은 아이템 인덱스를 찾아 여유공간만큼 때려 넣는다
			for(
				POSTYPE position = inventoryStartPosition;
				inventoryEndPosition > position;
				++position )
			{
				if( ! resultSize )
				{
					break;
				}
				else if( inventoryMap.end() == inventoryMap.find( position ) )
				{
					continue;
				}

				const Material& slot		= inventoryMap[ position ];
				const int		enableSize	= int( MAX_YOUNGYAKITEM_DUPNUM - slot.mQuantity );

				// 071213 LUJ,	재료 아이템인 경우 결과를 업데이트할 곳으로 선정하지 않는다
				//				DB 랙으로 삽입/삭제가 뒤바뀌어 진행될 경우 성공해도 아이템을 삭제하는 경우가 발생한다
				if(	result->mItemIndex != slot.mItemIndex	||
					0 >=	enableSize						||
					position == sourceItem->GetPosition() )
				{
					continue;
				}

				resultSlot->mItemIndex	= result->mItemIndex;
				resultSlot->mQuantity	= min( resultSize, enableSize );
				resultSlot->mPosition	= position;

				resultSize	-= resultSlot->mQuantity;

				++resultSlot;
			}
		}

		// 겹치는 아이템이 아니거나, 남은 겹치는 아이템이 있을 경우 채운다.
		for(
			POSTYPE position = inventoryStartPosition;
			inventoryEndPosition > position;
			++position )
		{
			if( ! resultSize )
			{
				break;
			}
			else if( inventoryMap.end() != inventoryMap.find( position ) )
			{
				continue;
			}
			Material& slot	= inventoryMap[ position ];

			resultSlot->mItemIndex	= result->mItemIndex;
			resultSlot->mQuantity	= ( isResultDuplicate ? min( resultSize, MAX_YOUNGYAKITEM_DUPNUM ) : 1 );
			resultSlot->mPosition	= position;

			resultSize				-= resultSlot->mQuantity;
			++resultSlot;
		}

		if( resultSize )
		{
			CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 910 ) );

			ZeroMemory( &mMessage, sizeof( mMessage ) );
			return;
		}

		mMessage.mResultSize = resultSlot - ( mMessage.mMaterial + mMessage.mSourceSize );
	}

	if( ! requiredMaterial.empty() )
	{
		CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 784 ) );

		ZeroMemory( &mMessage, sizeof( mMessage ) );
		return;
	}

	{
		mMessage.Category			= MP_ITEM;
		mMessage.Protocol			= MP_ITEM_MIX_SYN;
		mMessage.dwObjectID			= hero->GetID();
		mMessage.mResultIndex		= index;
		mMessage.mMixSize			= mixSize;
	}

	// 080228 LUJ, 보호 아이템 정보 설정
	if( mProtectionItem.wIconIdx )
	{
		ICONBASE& protectionItem = mMessage.mProtectionItem;

		protectionItem.dwDBIdx	= mProtectionItem.dwDBIdx;
		protectionItem.wIconIdx	= mProtectionItem.wIconIdx;
		protectionItem.Position	= mProtectionItem.Position;
	}

	if( isCheck )
	{
		if( 1 < mixSize )
		{
			const ITEM_INFO* itemInfo = ITEMMGR->GetItemInfo( result->mItemIndex );
			ASSERT( itemInfo );

			WINDOWMGR->MsgBox( MBI_ITEM_MIX_CONFIRM, MBT_YESNO, CHATMGR	->GetChatMsg( 1086 ), itemInfo->ItemName, mixSize );
		}
		// 진행 표시가 끝난 후 서버에 작업을 요청할 것이다
		else
		{
			CProgressDialog* dialog = ( CProgressDialog* )WINDOWMGR->GetWindowForID( PROGRESS_DIALOG );
			ASSERT( dialog );

			dialog->Wait( CProgressDialog::eActionMix );
		}

		SetDisable( TRUE );
	}
}


void CMixDialog::OnActionEvent( LONG lId, void * p, DWORD we )
{
	switch( lId )
	{
		// 그리드 창의 아이콘 클릭
	case ITMD_MIXICONGRIDDLG:
		{
			if(we == WE_LBTNCLICK)
			{
				const LONG position = mIconGridDialog->GetCurSelCellPos();

				// 빈 슬롯 클릭
				if( mIconGridDialog->IsAddable( WORD( position ) ) )
				{
					mDescription->RemoveAll();
					return;
				}

				CItem* item	= ( CItem* )mBaseDialog->GetIconForIdx( ePositionSource );

				if( ! item )
				{
					mDescription->RemoveAll();
					return;
				}

				const ItemMixResult* mixResult = ITEMMGR->GetMixResult( item->GetItemIdx(), position );

				PutDescription( mixResult );
			}
		}
		break;
		// 조합 시도
	case ITMD_MIXOKBTN:
		{
			cIcon* sourceItem = mBaseDialog->GetIconForIdx( 0 );

			if( ! sourceItem )
			{
				return;
			}

			const MixScript*	script				= ITEMMGR->GetMixScript( sourceItem->GetData() );
			const WORD			selectedPosition	= WORD( mIconGridDialog->GetCurSelCellPos() );

			if( -1 == selectedPosition			||
				script->size() < DWORD( selectedPosition ) )
			{
				return;
			}

			cIcon* selectedItem = mIconGridDialog->GetIconForIdx( selectedPosition );

			if( ! selectedItem )
			{
				ASSERT( 0 );
				return;
			}
			// 겹치지 않는 아이템이면 바로 조합 시도
			else if( ! ITEMMGR->IsDupItem( selectedItem->GetData() ) )
			{
				Submit( 1, TRUE );
			}
			// 겹치는 아이템이면 분할 상자를 표시하여 몇개나 만들것인지 결정하게 한다.
			else
			{
				cWindow* window = GetWindowForID( ITMD_MIXOKBTN );
				ASSERT( window );

				cDivideBox * divideBox = WINDOWMGR->DivideBox( 
					DBOX_DIVIDE_INV,
					( LONG )window->GetAbsX(),
					( LONG )window->GetAbsY(),
					SubmitDivide,
					CancelDivide,
					this,
					selectedItem,
					CHATMGR->GetChatMsg(185) );

				// 071213 LYW --- MixDialog : 디바이드 박스 NULL 참조 방어코드 추가.
				if( !divideBox ) return ;

				divideBox->SetMaxValue( MAX_YOUNGYAKITEM_DUPNUM );
				divideBox->SetValue( 1 );
			}
		}
		break;
		// 취소
	case ITMD_MIXCANCELBTN:
		{
			//CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 785 ) );

			SetActive( FALSE );
		}
		break;		
	}
}


void CMixDialog::Release()
{
	{
		for(	ResultIcon::iterator it = mResultIcon.begin();
			mResultIcon.end() != it;
			++it )
		{
			CItemShow* item = *it;

			SAFE_DELETE( item );
		}

		mResultIcon.clear();
	}

	{
		for(	ItemIcon::iterator it = mItemIcon.begin();
			mItemIcon.end() != it;
			++it )
		{
			CItem* item = *it;

			SAFE_DELETE( item );
		}

		mItemIcon.clear();
	}	
}


// 080228 LUJ, 보호 아이템이 적용된 경우 소모한다
void CMixDialog::Succeed( const MSG_ITEM_MIX_ACK& message )
{
	Restore();

	// 080228 LUJ, 보호 아이템을 사용한 경우 창을 닫는다
	if( mMessage.mProtectionItem.dwDBIdx )
	{
		CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 1452 ) );

		CItem* item = ITEMMGR->GetItem( mMessage.mProtectionItem.dwDBIdx );

		if( item )
		{
			SetActive( FALSE );
		}
	}

	mBaseDialog->DeleteIcon( ePositionSource, 0 );

	RemoveResult();

	const MixScript* script;
	{
		CItem* item		= mItemIcon[ ePositionSource ];
		CItem* original = ITEMMGR->GetItem( item->GetDBIdx() );

		if( ! original )
		{
			ASSERT( 0 );
			return;
		}

		script	= ITEMMGR->GetMixScript( original->GetItemIdx() );

		if( ! script )
		{
			ASSERT( 0 && "서버측 조합 코드를 즉시 점검할 것" );
			return;
		}
	}

	// 조합 결과를 두번째 칸에 표시한다.
	const ItemMixResult* mixResult = 0;
	{
		MixScript::const_iterator it = script->begin();

		if( script->size() <= message.mResultIndex )
		{
			ASSERT( 0 && "서버측 조합 코드를 즉시 점검할 것" );
			return;
		}

		std::advance( it, message.mResultIndex );
		mixResult = &( *it );

		if( ! mixResult )
		{
			ASSERT( 0 && "서버측 조합 코드를 즉시 점검할 것" );
			return;
		}
	}

	// 결과를 두번째 아이콘에 바인딩한다
	{
		CItem* item = mItemIcon[ ePositionResult ];

		ITEMBASE itemBase;
		ZeroMemory( &itemBase, sizeof( itemBase ) );

		itemBase.wIconIdx	= mixResult->mItemIndex;
		itemBase.Durability	= ( ITEMMGR->IsDupItem( itemBase.wIconIdx ) ? mMessage.mMixSize : 0 );

		/*
		080227 LUJ,	조합 결과에 옵션을 표시하기 위해서는 DB 인덱스를 설정해야 하며, 이를 위해 조합 아이템이 생성되는 경우를 알아야한다.

		경우 1. 생성
			DB에서 생성된 경우 Item_Mix_GetItem()을 통해 아이템 정보가 다시 갱신된다. 따라서, DB 인덱스를 설정할 필요가 없다
			이 경우, DBIndex에는 -1값이 들어있다.

		경우 2. 대체
			DB의 비동기적 실행으로 인한 처리 순서 꼬임을 막기 위해, 재료를 다 소모한 슬롯이 있을 경우 그 슬롯을 재활용하고
			DB 인덱스를 그대로 사용한다. 이때는 MSG_ITEM_MIX_ACK::mSourceItemDbIndex를 
			사용하면 된다.

		결론: 따라서, MSG_ITEM_MIX_ACK::mSourceItemDbIndex로 DB 인덱스를 설정해놓으면 새로운 옵션이 있을 경우 다시 갱신될 것이다.
		*/
		itemBase.dwDBIdx	= message.mSourceItemDbIndex;

		cImage image;

		item->Init(
			0,
			0,
			DEFAULT_ICONSIZE,
			DEFAULT_ICONSIZE,
			ITEMMGR->GetIconImage( itemBase.wIconIdx, &image ),
			0 );

		item->SetIconType( eIconType_AllItem );
		item->SetData( itemBase.wIconIdx );
		item->SetItemBaseInfo( itemBase );

		ITEMMGR->AddToolTip( item );
		mBaseDialog->AddIcon( ePositionResult, item );
	}
}


// 080228 LUJ, 보호 모드 처리 추가
void CMixDialog::Fail( const MSG_ITEM_MIX_ACK& message )
{
	Restore();

	// 080228 LUJ, 보호 아이템을 사용한 경우 창을 닫는다
	if( mMessage.mProtectionItem.dwDBIdx )
	{
		CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 1449 ) );

		CItem* item = ITEMMGR->GetItem( mMessage.mProtectionItem.dwDBIdx );

		if( item )
		{
			SetActive( FALSE );
		}
	}
	else
	{
		CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 812 ) );
	}

	// 대실패일 경우 호출된다. 최초 재료가 삭제되므로 지워야한다
	if( MP_ITEM_MIX_BIGFAILED_ACK == message.Protocol )
	{
		mBaseDialog->DeleteIcon( ePositionSource, 0 );
	}
}


void CMixDialog::SetActive( BOOL isActive )
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
			cDialog* DissoloveDialog	= WINDOWMGR->GetWindowForID( DIS_DISSOLUTIONDLG );
			cDialog* enchantDialog		= WINDOWMGR->GetWindowForID( ITD_UPGRADEDLG );
			cDialog* reinforceDialog	= WINDOWMGR->GetWindowForID( ITR_REINFORCEDLG );

			ASSERT( DissoloveDialog && enchantDialog && reinforceDialog );

			DissoloveDialog->SetActive( FALSE );
			enchantDialog->SetActive( FALSE );
			reinforceDialog->SetActive( FALSE );
		}

		mIconGridDialog->SetCurSelCellPos( -1 );

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
		RemoveResult();

		mBaseDialog->DeleteIcon( ePositionResult, 0 );

		if( ! mBaseDialog->IsAddable( ePositionSource ) )
		{
			RemoveItem();

			mBaseDialog->DeleteIcon( ePositionSource, 0 );
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


void CMixDialog::Restore()
{
	//mIsMixing	 = FALSE;

	ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );

	if( HERO->GetState() == eObjectState_Die )
	{
		SetActive( FALSE );
	}

	SetDisable( FALSE );
}


void CMixDialog::SubmitDivide( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	CMixDialog* dialog = ( CMixDialog* )vData1;

	dialog->Submit( param1, TRUE );
}


void CMixDialog::CancelDivide( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{}


void CMixDialog::Send()
{
	Submit( mMessage.mMixSize, FALSE );

	static const MSG_ITEM_MIX_SYN emptyMessage;

	if( ! memcmp( &emptyMessage, &mMessage, sizeof( emptyMessage ) ) )
	{
		return;
	}

	MSG_ITEM_MIX_SYN2 message;
	{
		ZeroMemory( &message, sizeof( message ) );

		CItem* sourceItem = ( CItem* )mBaseDialog->GetIconForIdx(ePositionSource);

		if( ! sourceItem )
		{
			CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 791 ) );
			return ;
		}

		message.Category			= MP_ITEM;
		message.Protocol			= MP_ITEM_MIX_SYN;
		message.dwObjectID			= HEROID;
		message.mSourceItemPosition	= sourceItem->GetPosition();
		message.mSourceItemDbIndex	= sourceItem->GetDBIdx();
		message.mMixSize			= mMessage.mMixSize;
		message.mResultIndex		= mMessage.mResultIndex;
		message.mProtectionItem		= mMessage.mProtectionItem;
	}

	NETWORK->Send( &message, sizeof( message ) );	

	{
		mIconGridDialog->SetCurSelCellPos( -1 );

		SetDisable( FALSE );

		RemoveItem();
	}	
}


void CMixDialog::Refresh()
{
	if( mBaseDialog->IsAddable( ePositionSource ) )
	{
		return;
	}

	CItem* source = mItemIcon[ ePositionSource ];

	if( source )
	{
		RemoveResult();
		AddResult( source->GetItemIdx() );
	}
}


// 080227 LUJ, 조합 결과를 갱신한다
void CMixDialog::RefreshResult( const ITEMBASE& itemBase )
{
	CItem* icon	= mItemIcon[ ePositionResult ];

	if( icon )
	{
		icon->SetItemBaseInfo( itemBase );

		ITEMMGR->AddToolTip( icon );
	}
}


void CMixDialog::RemoveItem()
{
	CItem* item		= mItemIcon[ ePositionSource ];
	CItem* original = ITEMMGR->GetItem( item->GetDBIdx() );

	// 아이콘 복구
	if( original )
	{
		original->SetLock( FALSE );

		if( ITEMMGR->IsDupItem( original->GetItemIdx() ) )
		{
			const DWORD quantity = original->GetDurability() + item->GetDurability();
			original->SetDurability( quantity );
		}

		// 080108 LUJ, 상태에 따라 초기화한다
		ITEMMGR->RefreshItem( original );
	}

	mBaseDialog->DeleteIcon( ePositionSource, 0 );
}


// 080228 LUJ, 보호 아이템 설정
void CMixDialog::SetProtectedActive( CItem& item )
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

	CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 1456 ) );
}

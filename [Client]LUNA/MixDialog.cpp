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
// 080228 LUJ, �Ϲ�/��ȣ ��� ǥ������ ��Ʈ�� �ʱ�ȭ
mTitle( 0 ),
mProtectTitle( 0 ),
mProtectSymbol( 0 )
{
	// 080228 LUJ, ��ȣ ������ ���� �ʱ�ȭ
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

	// �������� �ʱ�ȭ�Ѵ�
	{
		cImage imgToolTip;
		SCRIPTMGR->GetImage( 0, &imgToolTip, PFT_HARDPATH );

		// �ʱ� ��� �� ��� ������
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

		// �߰� ��� ������
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

	// 080228 LUJ, �Ϲ�/��ȣ ��� ǥ������ ��Ʈ�� ��ũ
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

	// �κ��丮�̿ܿ� ������ FALSE
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

		// Ŭ����
		{
			RemoveResult();

			// ��ϵ� ��ᰡ ������ �����ؾ��Ѵ�
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

	const std::string prefixProduct	( RESRCMGR->GetMsg( 626 ) ); // "����ǰ��: ";
	const std::string prefixLevel	( RESRCMGR->GetMsg( 627 ) ); // "���ѷ���: ";
	const std::string prefixMaterial( RESRCMGR->GetMsg( 628 ) ); // "�ʿ����: ";
	const std::string prefixMoney	( RESRCMGR->GetMsg( 629 ) ); // "���ۺ��: ";
	//const std::string prefixSuccess	( "����Ȯ��:%.1f%%" );
	const std::string prefixSuccess	( CHATMGR->GetChatMsg(1464) );
	const std::string postfixList	( RESRCMGR->GetMsg( 630 ) ); // "��";

	CHero*		hero = OBJECTMGR->GetHero();
	const DWORD textColor( RGBA_MAKE( 255, 255, 255, 0 ) );
	char		line[ MAX_TEXT_SIZE ];

	mDescription->RemoveAll();

	// ���� ǰ�� ǥ��
	{
		sprintf( line, "%s%s", prefixProduct.c_str(), info->ItemName );

		mDescription->AddItemAutoLine( line, ITEMMGR->GetItemNameColor( *info ) );
	}

	// ���� ǥ��
	{
		if( result->mRequiredLevel > hero->GetLevel() )
		{
			sprintf( line, "%s%d", prefixLevel.c_str(), result->mRequiredLevel );
			mDescription->AddItemAutoLine( line, DisableColor );
		}
	}

	// ��� ǥ��
	{
		sprintf( line, "%s%s", prefixMoney.c_str(), AddComma( result->mMoney ) );

		mDescription->AddItemAutoLine( line, ( result->mMoney > hero->GetMoney() ? DisableColor : EnableColor ) );
	}

	// ������
	{
		sprintf( line, prefixSuccess.c_str(), result->mSuccessRate * 100.0f );

		mDescription->AddItemAutoLine( line, textColor );
	}

	typedef std::list< std::string >	Description;
	Description							description;

	// ��� ǥ��
	{
		sprintf( line, "%s", prefixMaterial.c_str() );

		mDescription->AddItemAutoLine( line, textColor );

		std::string tab;

		// ��� ǥ���� ���� ��ŭ�� �������� �Ѵ�
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
			ASSERT( 0 && "��� ������ �׸��� �� �����ܺ��� ����" );
			break;
		}

		CItemShow*				itemShow	= mResultIcon[ index ];
		const ItemMixResult&	result		= *it;
		const DWORD				itemIndex	= result.mItemIndex;

		itemShow->SetData( itemIndex );
		itemShow->Init( itemIndex, 0 );		

		// ���� üũ�Ͽ� ���� �Ұ� �������� ���� ����� ������.
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
	// ���� ����� 1���϶��� �ڵ� ����
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

	// �⺻ ������ ����?
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

	// ���õ� ���� ���� ��� ó��: ���� ����� 1 �����϶��� �ڵ����� ó���Ѵ�.
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

	// �ʿ� ������ ä����... 
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

	// ����� ���Ժ��� �����Ͽ� �ִ��� ������ ä���.
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
				// �ɼ��ϰ� ������ �����ϴµ� �� �ʵ带 ���� ���� �ִٴ� ����� ��������. 
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
			ASSERT( 0 && "��ϵ� �������� ��ῡ ���ԵǾ� ���� �ʴ� ���� ������ �ʴ� ��Ȳ" );

			ZeroMemory( &mMessage, sizeof( mMessage ) );
			return;
		}
	}

	CInventoryExDialog* inventory = ( CInventoryExDialog* )WINDOWMGR->GetWindowForID( IN_INVENTORYDLG );
	ASSERT( inventory );

	// ��� ������ �����Ѵ�
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

		// ���� �������� ���� ������ ������ ����
		// ���� ������ 0������ ��� ������ ������ ���� ���� ������ �Ѵ�.
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

	// ���� ��ġ�� ����Ѵ�.
	{
		typedef MSG_ITEM_MIX_SYN::Material	Material;
		typedef std::map< DWORD, Material > InventoryMap;
		InventoryMap						inventoryMap;

		// �κ��丮 ���� ����
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
					// ��� �Ѱ��� ����â�� ��ϵǾ������Ƿ� �����־�� �Ѵ�.
					slot.mQuantity	= ( sourceItemDBIndex == item->GetDBIdx() ? item->GetDurability() + 1 : item->GetDurability() );
				}
				else
				{
					slot.mQuantity	= 1;
				}
			}
		}

		// ��� ������ ��ȸ�ذ��鼭 �κ��丮 ������ �����Ѵ�(�ҿ�� ��Ḧ �����Ѵ�)
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

		// ��ġ�� �������̸�
		if( isResultDuplicate )
		{
			// �ϴ� ���� ������ �ε����� ã�� ����������ŭ ���� �ִ´�
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

				// 071213 LUJ,	��� �������� ��� ����� ������Ʈ�� ������ �������� �ʴ´�
				//				DB ������ ����/������ �ڹٲ�� ����� ��� �����ص� �������� �����ϴ� ��찡 �߻��Ѵ�
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

		// ��ġ�� �������� �ƴϰų�, ���� ��ġ�� �������� ���� ��� ä���.
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

	// 080228 LUJ, ��ȣ ������ ���� ����
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
		// ���� ǥ�ð� ���� �� ������ �۾��� ��û�� ���̴�
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
		// �׸��� â�� ������ Ŭ��
	case ITMD_MIXICONGRIDDLG:
		{
			if(we == WE_LBTNCLICK)
			{
				const LONG position = mIconGridDialog->GetCurSelCellPos();

				// �� ���� Ŭ��
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
		// ���� �õ�
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
			// ��ġ�� �ʴ� �������̸� �ٷ� ���� �õ�
			else if( ! ITEMMGR->IsDupItem( selectedItem->GetData() ) )
			{
				Submit( 1, TRUE );
			}
			// ��ġ�� �������̸� ���� ���ڸ� ǥ���Ͽ� ��� ��������� �����ϰ� �Ѵ�.
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

				// 071213 LYW --- MixDialog : ����̵� �ڽ� NULL ���� ����ڵ� �߰�.
				if( !divideBox ) return ;

				divideBox->SetMaxValue( MAX_YOUNGYAKITEM_DUPNUM );
				divideBox->SetValue( 1 );
			}
		}
		break;
		// ���
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


// 080228 LUJ, ��ȣ �������� ����� ��� �Ҹ��Ѵ�
void CMixDialog::Succeed( const MSG_ITEM_MIX_ACK& message )
{
	Restore();

	// 080228 LUJ, ��ȣ �������� ����� ��� â�� �ݴ´�
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
			ASSERT( 0 && "������ ���� �ڵ带 ��� ������ ��" );
			return;
		}
	}

	// ���� ����� �ι�° ĭ�� ǥ���Ѵ�.
	const ItemMixResult* mixResult = 0;
	{
		MixScript::const_iterator it = script->begin();

		if( script->size() <= message.mResultIndex )
		{
			ASSERT( 0 && "������ ���� �ڵ带 ��� ������ ��" );
			return;
		}

		std::advance( it, message.mResultIndex );
		mixResult = &( *it );

		if( ! mixResult )
		{
			ASSERT( 0 && "������ ���� �ڵ带 ��� ������ ��" );
			return;
		}
	}

	// ����� �ι�° �����ܿ� ���ε��Ѵ�
	{
		CItem* item = mItemIcon[ ePositionResult ];

		ITEMBASE itemBase;
		ZeroMemory( &itemBase, sizeof( itemBase ) );

		itemBase.wIconIdx	= mixResult->mItemIndex;
		itemBase.Durability	= ( ITEMMGR->IsDupItem( itemBase.wIconIdx ) ? mMessage.mMixSize : 0 );

		/*
		080227 LUJ,	���� ����� �ɼ��� ǥ���ϱ� ���ؼ��� DB �ε����� �����ؾ� �ϸ�, �̸� ���� ���� �������� �����Ǵ� ��츦 �˾ƾ��Ѵ�.

		��� 1. ����
			DB���� ������ ��� Item_Mix_GetItem()�� ���� ������ ������ �ٽ� ���ŵȴ�. ����, DB �ε����� ������ �ʿ䰡 ����
			�� ���, DBIndex���� -1���� ����ִ�.

		��� 2. ��ü
			DB�� �񵿱��� �������� ���� ó�� ���� ������ ���� ����, ��Ḧ �� �Ҹ��� ������ ���� ��� �� ������ ��Ȱ���ϰ�
			DB �ε����� �״�� ����Ѵ�. �̶��� MSG_ITEM_MIX_ACK::mSourceItemDbIndex�� 
			����ϸ� �ȴ�.

		���: ����, MSG_ITEM_MIX_ACK::mSourceItemDbIndex�� DB �ε����� �����س����� ���ο� �ɼ��� ���� ��� �ٽ� ���ŵ� ���̴�.
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


// 080228 LUJ, ��ȣ ��� ó�� �߰�
void CMixDialog::Fail( const MSG_ITEM_MIX_ACK& message )
{
	Restore();

	// 080228 LUJ, ��ȣ �������� ����� ��� â�� �ݴ´�
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

	// ������� ��� ȣ��ȴ�. ���� ��ᰡ �����ǹǷ� �������Ѵ�
	if( MP_ITEM_MIX_BIGFAILED_ACK == message.Protocol )
	{
		mBaseDialog->DeleteIcon( ePositionSource, 0 );
	}
}


void CMixDialog::SetActive( BOOL isActive )
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

	if( isActive )
	{
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

		// ��ȭ/��æƮ/����/����/�����Ǹ� â�� ���ÿ� �ߴ� ���� ���´�
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

		// 080228 LUJ, �Ϲ� ��� ǥ��
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

		// 080228 LUJ, ��ȣ �������� �ʱ�ȭ�Ѵ�
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


// 080227 LUJ, ���� ����� �����Ѵ�
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

	// ������ ����
	if( original )
	{
		original->SetLock( FALSE );

		if( ITEMMGR->IsDupItem( original->GetItemIdx() ) )
		{
			const DWORD quantity = original->GetDurability() + item->GetDurability();
			original->SetDurability( quantity );
		}

		// 080108 LUJ, ���¿� ���� �ʱ�ȭ�Ѵ�
		ITEMMGR->RefreshItem( original );
	}

	mBaseDialog->DeleteIcon( ePositionSource, 0 );
}


// 080228 LUJ, ��ȣ ������ ����
void CMixDialog::SetProtectedActive( CItem& item )
{
	SetActive( TRUE );

	// 080228 LUJ, ��ȣ ��� ǥ��
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

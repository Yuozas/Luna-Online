#include "stdafx.h"
#include "ApplyOptionDialog.h"
#include "WindowIDEnum.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "cScriptManager.h"
#include "cWindowManager.h"
#include "interface/cIconDialog.h"
#include "interface/cStatic.h"
#include "ItemManager.h"
#include "ProgressDialog.h"
#include "FishingManager.h"
#include "GameIn.h"
#include "FishingDialog.h"
#include "FishingPointDialog.h"


CApplyOptionDialog::CApplyOptionDialog() :
mIconDialog( 0 ),
mResultStatic( 0 ),
mTargetItem( 0 ),
mSourceItem( 0 )
{
	// ��� ������ ������
	{
		cImage image;

		SCRIPTMGR->GetImage( 0, &image, PFT_HARDPATH );

		ITEMBASE base;
		ZeroMemory( &base, sizeof( base ) );

		mTargetItem = new CItem( &base );

		mTargetItem->SetToolTip( "", RGB_HALF( 255, 255, 255), &image );
		mTargetItem->SetMovable( FALSE );
	}
}


CApplyOptionDialog::~CApplyOptionDialog()
{
	mIconDialog->DeleteIcon( 0, 0 );

	SAFE_DELETE( mTargetItem );
}


void CApplyOptionDialog::Linking()
{
	mIconDialog		= ( cIconDialog* )	GetWindowForID( APPLY_OPTION_ICON_DIALOG );
	mResultStatic	= ( cStatic* )		GetWindowForID( APPLY_OPTION_RESULT_STATIC );
}


void CApplyOptionDialog::SetSourceItem( CItem& item )
{
	mSourceItem = &item;
}


BOOL CApplyOptionDialog::FakeMoveIcon(LONG x, LONG y, cIcon* targetIcon )
{
	if( !	targetIcon							||
			WT_ITEM != targetIcon->GetType()	||
			targetIcon->IsLocked()				||
			IsDisable()							||
		!	mSourceItem )
	{
		return FALSE;
	}

	CItem*				targetItem = ( CItem* )targetIcon;
	const ITEM_INFO*	info = targetItem->GetItemInfo();

	if( !	info ||
			info->Stack )
	{
		return FALSE;
	}
	// �κ��丮�� �����۸� ���ȴ�
	else if( ! ITEMMGR->IsEqualTableIdxForPos( eItemTable_Inventory, targetItem->GetPosition() ) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 787 ) );
		return FALSE;
	}
	
	// ������ �� �ִ� ���������� �˻�
	{
		const LONGLONG itemKey = ITEMMGR->GetItemKey(
			eEquipSlot( info->EquipSlot + 1 ),
			eWeaponType( info->WeaponType ),
			eArmorType( info->ArmorType ),
			eWeaponAnimationType( info->WeaponAnimation ) );

		const ApplyOptionScript* script	= ITEMMGR->GetApplyOptionScript( mSourceItem->GetItemIdx() );

		if( !	script									||
				script->mLevel.mMin > info->LimitLevel	||
				script->mLevel.mMax < info->LimitLevel	||
				script->mTargetTypeSet.end() == script->mTargetTypeSet.find( itemKey ) )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1421 ) );
			return FALSE;
		}
	}
	
	// ������ ĭ�� �̹� �������� �ִٸ� �����Ѵ�
	{
		cIcon* icon = mIconDialog->GetIconForIdx( 0 );

		if( icon == mTargetItem )
		{
			CItem* item = ITEMMGR->GetItem( mTargetItem->GetDBIdx() );

			if( item )
			{
				item->SetLock( FALSE );
			}
		}
	}

	// â�� �������� ǥ���Ѵ�
	{
		cImage image;

		mTargetItem->Init(
			0,
			0,
			DEFAULT_ICONSIZE,
			DEFAULT_ICONSIZE,
			ITEMMGR->GetIconImage( targetItem->GetItemIdx(), &image ),
			0 );

		mTargetItem->SetItemBaseInfo( targetItem->GetItemBaseInfo() );

		ITEMMGR->AddToolTip( mTargetItem );

		mIconDialog->DeleteIcon( 0, 0 );
		mIconDialog->AddIcon( 0, mTargetItem );
	}
	
	targetItem->SetLock( TRUE );

	return FALSE;
}


void CApplyOptionDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	switch( lId )
	{
	case APPLY_OPTION_SUBMIT_BUTTON:
		{
			if( mTargetItem != mIconDialog->GetIconForIdx( 0 ) )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, "Empty" );
				break;
			}
			
			const ITEM_INFO* info = mTargetItem->GetItemInfo();

			if( !	mSourceItem	||
				!	info		||
					info->Stack )
			{
				return;
			}

			mMessage.Category	= MP_ITEM;
			mMessage.Protocol	= MP_ITEM_APPLY_OPTION_SYN;
			mMessage.dwObjectID	= HERO->GetID();
			mMessage.dwData1	= mSourceItem->GetPosition();
			mMessage.dwData2	= mTargetItem->GetPosition();

			ITEMMGR->SetDisableDialog( TRUE, eItemTable_Inventory );
			SetDisable( TRUE );

			// ���� ǥ�ð� ���� �� ������ �۾��� ��û�� ���̴�
			{
				CProgressDialog* dialog = ( CProgressDialog* )WINDOWMGR->GetWindowForID( PROGRESS_DIALOG );
				ASSERT( dialog );

				dialog->Wait( CProgressDialog::eActionApplyOption );
			}
			
			break;
		}
	case APPLY_OPTION_CANCEL_BUTTON:
	case APPLY_OPTION_DIALOG:
		{
			if( m_bDisable )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 992 ) );
				break;
			}

			SetActive( FALSE );

			{
				cIcon* icon = mIconDialog->GetIconForIdx( 0 );

				if( icon == mTargetItem )
				{
					CItem* item = ITEMMGR->GetItem( mTargetItem->GetDBIdx() );

					if( item )
					{
						item->SetLock( FALSE );
					}
				}
			}

			if( mSourceItem )
			{
				mSourceItem->SetLock( FALSE );
			}

			break;
		}
	}
}


void CApplyOptionDialog::SetActive( BOOL isActive )
{
	if( isActive )
	{
		if(		IsActive() ||
			!	mSourceItem )
		{
			return;
		}

		mSourceItem->SetLock( TRUE );

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

		// ��ȭ/��æƮ/����/����/�����Ǹ� â�� ���ÿ� �ߴ� ���� ���´�
		{
			cDialog* mixDialog			= WINDOWMGR->GetWindowForID( ITMD_MIXDLG );
			cDialog* DissoloveDialog	= WINDOWMGR->GetWindowForID( DIS_DISSOLUTIONDLG );
			cDialog* reinforceDialog	= WINDOWMGR->GetWindowForID( ITR_REINFORCEDLG );
			
			ASSERT( mixDialog && DissoloveDialog && reinforceDialog );

			mixDialog->SetActive( FALSE );
			DissoloveDialog->SetActive( FALSE );
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

		// �ʱ�ȭ
		{
			mIconDialog->DeleteIcon( 0, 0 );
			mResultStatic->SetStaticText( "" );
		}		
	}
	else
	{
		if( mSourceItem )
		{
			mSourceItem->SetLock( FALSE );
		}

		{
			CItem* item = ITEMMGR->GetItem( mTargetItem->GetDBIdx() );

			if( item )
			{
				item->SetLock( FALSE );
			}
		}

		{
			CProgressDialog* dialog = ( CProgressDialog* )WINDOWMGR->GetWindowForID( PROGRESS_DIALOG );
			ASSERT( dialog );

			if(  dialog->IsDisable() )
			{
				return;
			}
		}
	}

	cDialog::SetActive( isActive );
}


void CApplyOptionDialog::Succeed()
{
	ITEMMGR->AddToolTip( mTargetItem );

	CItem* item = ITEMMGR->GetItem( mTargetItem->GetDBIdx() );

	if( item )
	{
		ITEMMGR->AddToolTip( item );
	}

	mResultStatic->SetStaticText( CHATMGR->GetChatMsg( 1422 ) );

	Restore();

	// ����� ���� ������ ������ �����Ѵ�
	if( mSourceItem->GetItemInfo()->Stack &&
		1 < mSourceItem->GetDurability())
	{
		mSourceItem->SetDurability( mSourceItem->GetDurability() - 1 );

		ITEMMGR->AddToolTip( mSourceItem );
	}
	else
	{
		{
			CItem* item = 0;

			ITEMMGR->DeleteItem( mSourceItem->GetPosition(), &item );

			mSourceItem = 0;
		}		

		{
			CItem* item = ITEMMGR->GetItem( mTargetItem->GetDBIdx() );

			if( item )
			{
				item->SetLock( FALSE );
			}
		}

		SetActive( FALSE );
	}

	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1419 ), mTargetItem->GetItemInfo()->ItemName );
}


void CApplyOptionDialog::Fail()
{
	mResultStatic->SetStaticText( CHATMGR->GetChatMsg( 1423 ) );

	Restore();

	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1420 ), mTargetItem->GetItemInfo()->ItemName );
}


void CApplyOptionDialog::Restore()
{
	SetDisable( FALSE );

	ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );
}


void CApplyOptionDialog::Send()
{
	NETWORK->Send( &mMessage, sizeof( mMessage ) );
}
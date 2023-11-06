// ItemDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "client.h"
#include "ItemDialog.h"
#include "ItemLogDialog.h"
#include "ItemAddDialog.h"
#include "DateDialog.h"
#include "QuantityDialog.h"
#include "ItemOptionLogDialog.h"
#include "ItemPositionDialog.h"


// CItemDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CItemDialog, CDialog)
CItemDialog::CItemDialog( CclientApp& application, CWnd* pParent /*=NULL*/)
	: CDialog(CItemDialog::IDD, pParent)
	, mSelectedItemDbIndex( 0 )
	, mApplication( application )
	, mStorageSize( 0 )
	, mExtendedInventorySize( 0 )
	, mPlayerIndex( 0 )	
	, mUserIndex( 0 )
{
	EnableAutomation();
}

CItemDialog::~CItemDialog()
{
}


void CItemDialog::OnFinalRelease()
{
	// 자동화 개체에 대한 마지막 참조가 해제되면 OnFinalRelease가
	// 호출됩니다. 기본 클래스에서 자동으로 개체를 삭제합니다.
	// 기본 클래스를 호출하기 전에 개체에 필요한 추가 정리 작업을
	// 추가하십시오.

	CDialog::OnFinalRelease();
}

void CItemDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ITEM_INVENTORY_LIST, mInventoryListCtrl);
	DDX_Control(pDX, IDC_ITEM_DATA_FRAME2, mDataFrame);
	DDX_Control(pDX, IDC_ITEM_END_TIME_STATIC, mEndTimeStatic);
	DDX_Control(pDX, IDC_ITEM_REMAIN_TIME_STATIC, mRemainTimeStatic);
	DDX_Control(pDX, IDC_ITEM_STORAGE_SIZE_COMBO, mStorageSizeCombo);
	DDX_Control(pDX, IDC_ITEM_STORAGE_MONEY_EDIT, mStorageMoneyEdit);
	DDX_Control(pDX, IDC_ITEM_TAB, mTabCtrl);
	DDX_Control(pDX, IDC_ITEM_ADD_BUTTON, mItemAddButton);
	DDX_Control(pDX, IDC_ITEM_STORAGE_UPDATE_BUTTON, mStorageUpdateButton);
}


BEGIN_MESSAGE_MAP(CItemDialog, CDialog)
	ON_NOTIFY(HDN_ITEMCLICK, 0, OnHdnItemclickItemDialogInventory)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ITEM_INVENTORY, OnLvnItemchangedItemDialogInventory)
	//ON_BN_CLICKED(IDC_ITEM_DATA_UPDATE, OnBnClickedItemDialogDataUpdate)
	ON_BN_CLICKED(IDC_ITEM_DATA_UPDATE_BUTTON, OnBnClickedItemDialogDataUpdateButton)
	//ON_NOTIFY(LVN_ITEMCHANGED, IDC_ITEM_STORAGE, OnLvnItemchangedItemStorage)
	ON_BN_CLICKED(IDC_ITEM_DATA_DELETE_BUTTON, OnBnClickedItemDataDeleteButton)
	ON_BN_CLICKED(IDC_ITEM_INVENTORY_LOG_BUTTON, OnBnClickedItemInventoryLogButton)
	//ON_BN_CLICKED(IDC_ITEM_WAREHOUSE_LOG_BUTTON2, OnBnClickedItemWarehouseLogButton2)
	ON_BN_CLICKED(IDC_ITEM_ADD_BUTTON, OnBnClickedItemAddButton)
	//ON_EN_SETFOCUS(IDC_ITEM_END_TIME_STATIC, OnEnSetfocusItemEndTimeStatic)
	//ON_EN_SETFOCUS(IDC_ITEM_REMAIN_TIME_STATIC, OnEnSetfocusItemRemainTimeStatic)
	ON_BN_CLICKED(IDC_ITEM_END_TIME_BUTTON, OnBnClickedItemEndTimeButton)
	ON_BN_CLICKED(IDC_ITEM_REMAIN_TIME_BUTTON, OnBnClickedItemRemainTimeButton)
	ON_NOTIFY(TCN_SELCHANGE, IDC_ITEM_TAB, OnTcnSelchangeItemTab)
	ON_BN_CLICKED(IDC_ITEM_STORAGE_UPDATE_BUTTON, OnBnClickedItemStorageUpdateButton)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CItemDialog, CDialog)
END_DISPATCH_MAP()

// 참고: IID_IItemDialog에 대한 지원을 추가하여
// VBA에서 형식 안정적인 바인딩을 지원합니다. 
// 이 IID는 .IDL 파일에 있는 dispinterface의 GUID와 일치해야 합니다.

// {50406DEA-E1CA-484E-AF7D-0C8310FCB000}
static const IID IID_IItemDialog =
{ 0x50406DEA, 0xE1CA, 0x484E, { 0xAF, 0x7D, 0xC, 0x83, 0x10, 0xFC, 0xB0, 0x0 } };

BEGIN_INTERFACE_MAP(CItemDialog, CDialog)
	INTERFACE_PART(CItemDialog, IID_IItemDialog, Dispatch)
END_INTERFACE_MAP()


void CItemDialog::Parse( DWORD serverIndex, const MSGROOT* message )
{
	mServerIndex = serverIndex;

	switch( message->Protocol )
	{
	case MP_RM_ITEM_GET_ACK:
		{
			const MSG_RM_ITEM* m = ( MSG_RM_ITEM* )message;

			for( DWORD i = 0; i < m->mSize; ++i )
			{
				const Item& item = m->mData[ i ];

				if( mItemMap.end() == mItemMap.find( item.dwDBIdx ) )
				{
					mItemMap.insert( std::make_pair( item.dwDBIdx, item ) );
				}
				else
				{
					ASSERT( 0 );
				}
			}

			{
				mTabCtrl.EnableWindow( TRUE );
				mInventoryListCtrl.EnableWindow( TRUE );
				mItemAddButton.EnableWindow( TRUE );
			}			

			RefreshInventory();
		
			break;
		}
	case MP_RM_ITEM_GET_OPTION_ACK:
		{
			const MSG_RM_ITEM_OPTION* m = ( MSG_RM_ITEM_OPTION* )message;

			for( DWORD i = 0; i < m->mSize; ++i )
			{
				const ITEM_OPTION& option = m->mData[ i ];

				if( mOptionMap.end() == mOptionMap.find( option.mItemDbIndex ) )
				{
					mOptionMap[ option.mItemDbIndex ] = option;
				}
				else
				{
					ASSERT( 0 && "duplicated option index found" );
				}
			}

			//ResetGrid();

			break;
		}	
	case MP_RM_ITEM_SET_ACK:
		{
			const MSG_RM_ITEM_UPDATE*	m		= ( MSG_RM_ITEM_UPDATE* )message;
			ITEMBASE*					item	= GetItem( m->mItem.dwDBIdx );

			if( ! item )
			{
				ASSERT( 0 );
				return;
			}
			
			*item = m->mItem;

			{
				CString textUpdateSucceed;
				textUpdateSucceed.LoadString( IDS_STRING28 );

				MessageBox( textUpdateSucceed, "", MB_OK );
			}			

			// 리스트 컨트롤 업데이트
			RefreshInventory();
			ResetGrid();

			break;
		}
	case MP_RM_ITEM_SET_OPTION_ACK:
		{
			const MSG_RM_ITEM_UPDATE_OPTION* m = ( MSG_RM_ITEM_UPDATE_OPTION* )message;

			const ITEM_OPTION&	option	= m->mOption;
			ITEMBASE*			item	= GetItem( mSelectedItemDbIndex );

			if( ! item )
			{
				ASSERT( 0 );
				return;
			}

			item->Durability = option.mItemDbIndex;

			if( mApplication.IsDuplicateItem( item->wIconIdx ) )
			{
				ASSERT( 0 && "Client and server data is not mismatched" );

				return;
			}

			// 이전 아이템 옵션이 있다면 지우자	
			mOptionMap.erase( item->Durability );

			ASSERT( mOptionMap.end() == mOptionMap.find( option.mItemDbIndex ) );
			mOptionMap[ item->dwDBIdx ] = option;

			RefreshInventory();
			ResetGrid();

			{
				CString textUpdateSucceed;
				textUpdateSucceed.LoadString( IDS_STRING28 );

				MessageBox( textUpdateSucceed, "", MB_OK );
			}

			break;
		}
	case MP_RM_ITEM_DELETE_ACK:
		{
			const MSG_DWORD* m = ( MSG_DWORD* )message;
			
			{
				const DWORD itemDbIndex = m->dwData;
				
				mOptionMap.erase( itemDbIndex );
				mItemMap.erase( itemDbIndex );
			}

			RefreshInventory();
			ResetGrid();

			{
				CString textItemWasRemoved;
				textItemWasRemoved.LoadString( IDS_STRING64 );

				MessageBox( textItemWasRemoved, "", MB_OK );
			}
			
			break;
		}
	case MP_RM_ITEM_DELETE_OPTION_ACK:
		{
			const MSG_DWORD* m = ( MSG_DWORD* )message;
			
			{
				const DWORD itemDbIndex	= m->dwData;
				
				mOptionMap.erase( itemDbIndex );
			}

			RefreshInventory();
			ResetGrid();

			{
				CString textOptionIsCleared;
				textOptionIsCleared.LoadString( IDS_STRING65 );

				MessageBox( textOptionIsCleared, "", MB_OK );
			}
			
			break;
		}
	case MP_RM_ITEM_GET_OPTION_NACK:
		{
			// 정보가 없을 수 있음
			break;
		}
	case MP_RM_ITEM_SET_NACK:
	case MP_RM_ITEM_SET_OPTION_NACK:
	case MP_RM_ITEM_DELETE_OPTION_NACK:
		{
			CString textUpdateIsFailed;
			textUpdateIsFailed.LoadString( IDS_STRING29 );

			MessageBox( textUpdateIsFailed, "", MB_OK );
			break;
		}
	case MP_RM_ITEM_ADD_ACK:
		{
			const MSG_RM_ITEM_ADD* m = ( MSG_RM_ITEM_ADD* )message;

			const Item& item = m->mItem;

			{
				AddItem( item );

				ASSERT( mItemMap.end() == mItemMap.find( item.dwDBIdx ) );

				mItemMap[ item.dwDBIdx ] = item;
			}

			{
				CString textItemAdd;
				textItemAdd.LoadString( IDS_STRING344 );

				// 080304 LUJ, 로그
				mApplication.Log(
					textItemAdd,
					mPlayerName,
					mPlayerIndex,
					mApplication.GetItemPositionType( item.mArea, item.Position ),
					item.Position,
					mApplication.GetItemName( item.wIconIdx ),
					item.dwDBIdx,
					max( 1, item.Durability ) );
			}

			{
				CString textCreatingItemIsSucceed;
				textCreatingItemIsSucceed.LoadString( IDS_STRING66 );

				MessageBox( textCreatingItemIsSucceed, "", MB_ICONINFORMATION | MB_OK );
			}
			
			break;
		}
	case MP_RM_ITEM_ADD_NACK:
		{
			CString textCreatingItemIsFailed;
			textCreatingItemIsFailed.LoadString( IDS_STRING67 );

			MessageBox( textCreatingItemIsFailed, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_ITEM_UPDATE_REMAIN_TIME_ACK:
		{
			const MSG_INT2* m				= ( MSG_INT2* )message;
			const DWORD		itemDbIndex		= m->nData1;
			const int		remainedSecond	= m->nData2;

			ITEMBASE* item = GetItem( itemDbIndex );

			if( ! item )
			{
				break;
			}

			mRemainTimeStatic.SetWindowText( GetDateString( item->nRemainSecond	= remainedSecond ) );
			break;
		}
	case MP_RM_ITEM_UPDATE_REMAIN_TIME_NACK:
		{
			CString textUpdateIsFailed;
			textUpdateIsFailed.LoadString( IDS_STRING29 );

			MessageBox( textUpdateIsFailed, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_ITEM_UPDATE_END_TIME_ACK:
		{
			const MSG_INT2* m				= ( MSG_INT2* )message;
			const DWORD		itemDbIndex		= m->nData1;
			const int		remainedSecond	= m->nData2;

			ITEMBASE* item = GetItem( itemDbIndex );

			if( ! item )
			{
				break;
			}

			item->nRemainSecond = remainedSecond;

			RefreshInventory();

			//mEndTimeStatic.SetWindowText( GetDateString( item->nRemainSecond = remainedSecond ) );

			break;
		}
	case MP_RM_ITEM_UPDATE_END_TIME_NACK:
		{
			CString textUpdateIsFailed;
			textUpdateIsFailed.LoadString( IDS_STRING29 );

			MessageBox( textUpdateIsFailed, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_ITEM_GET_STORAGE_ACK:
		{
			{
				const MSG_DWORD3* m = ( MSG_DWORD3* )message;

				CString text;
				text.Format( "%d", m->dwData2 );

				mStorageMoneyEdit.SetWindowText( text );
				mStorageSizeCombo.SetCurSel( m->dwData1 );

				mStorageSize			= m->dwData1;
				mExtendedInventorySize	= m->dwData3;
			}			

			// 잠궜던 컨트롤을 활성화시킨다
			{
				mStorageSizeCombo.EnableWindow( TRUE );
				mStorageMoneyEdit.EnableWindow( TRUE );
				mStorageUpdateButton.EnableWindow( TRUE );
			}

			break;
		}
	case MP_RM_ITEM_SET_STORAGE_ACK:
		{
			const MSG_DWORD2* m = ( MSG_DWORD2* )message;

			mStorageSizeCombo.SetCurSel( m->dwData1 );

			CString text;
			text.Format( "%d", m->dwData2 );

			mStorageSize = m->dwData1;

			mStorageMoneyEdit.SetWindowText( text );

			{
				CString textUpdateIsSucceed;
				textUpdateIsSucceed.LoadString( IDS_STRING28 );

				MessageBox( textUpdateIsSucceed, "", MB_OK | MB_ICONINFORMATION );
			}
			
			break;
		}
	case MP_RM_ITEM_NACK_BY_AUTH:
		{
			CString textYouHaveNoAuthority;
			textYouHaveNoAuthority.LoadString( IDS_STRING18 );

			MessageBox( textYouHaveNoAuthority, "", MB_OK | MB_ICONERROR );
			break;
		}
	default:
		{
			ASSERT( 0 );
			break;
		}
	}
}
BOOL CItemDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	CDataExchange dx( this, TRUE );
	DoDataExchange( &dx );

	SetIcon( AfxGetApp()->LoadIcon( IDR_MAINFRAME ), FALSE );
	SetIcon( AfxGetApp()->LoadIcon( IDR_MAINFRAME ), TRUE );

	// list control initialization
	{
		CRect rect;
		const DWORD addStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;

		{
			int step = -1;

			mInventoryListCtrl.GetWindowRect( &rect );

			CString textDbIndex;
			textDbIndex.LoadString( IDS_STRING13 );
			mInventoryListCtrl.InsertColumn( ++step, textDbIndex, LVCFMT_LEFT, 0 );

			CString textIndex;
			textIndex.LoadString( IDS_STRING22 );
			mInventoryListCtrl.InsertColumn( ++step, textIndex, LVCFMT_LEFT, 0 );

			CString textType;
			textType.LoadString( IDS_STRING4 );
			mInventoryListCtrl.InsertColumn( ++step, textType, LVCFMT_LEFT, int( rect.Width() * 0.14f ) );

			CString textPosition;
			textPosition.LoadString( IDS_STRING68 );
			mInventoryListCtrl.InsertColumn( ++step, textPosition, LVCFMT_LEFT, int( rect.Width() * 0.13f ) );

			CString textName;
			textName.LoadString( IDS_STRING20 );
			mInventoryListCtrl.InsertColumn( ++step, textName, LVCFMT_LEFT, int( rect.Width() * 0.44f ) );

			CString textQuantity;
			textQuantity.LoadString( IDS_STRING42 );
			mInventoryListCtrl.InsertColumn( ++step, textQuantity, LVCFMT_LEFT, int( rect.Width() * 0.12f ) );

			CString textInformation;
			textInformation.LoadString( IDS_STRING69 );
			mInventoryListCtrl.InsertColumn( ++step, textInformation, LVCFMT_LEFT, int( rect.Width() * 0.3f ) );
			
			mInventoryListCtrl.SetExtendedStyle( mInventoryListCtrl.GetExtendedStyle() | addStyle );
		}
	}

	// grid control initialization
	{
		CRect gridRect;
		{
			CRect windowRect;
			GetWindowRect( windowRect );

			CRect frameRect;
			mDataFrame.GetWindowRect( frameRect );

			gridRect.left	= abs( frameRect.left - windowRect.left );
			gridRect.top	= abs( frameRect.top - windowRect.top ) - 10;
			
			gridRect.right	= gridRect.left + frameRect.Width();
			gridRect.bottom	= gridRect.top	+ frameRect.Height();

			// scaling
			{
				// 90%
				const float scale	= 0.9f;
				const LONG	width	= LONG( gridRect.Width() * scale );
				const LONG	height	= LONG( gridRect.Height() * scale );

				gridRect.left	+= abs( frameRect.Width() - width ) / 2;
				gridRect.right	-= abs( frameRect.Width() - width ) / 2;

				//gridRect.top	-= abs( gridRect.Height() - height ) / 2;
				//gridRect.bottom	= gridRect.top + height;
			}
		}	

		const int titleColumn = int( gridRect.Width() * 0.7f );
		const int valueColumn = int( gridRect.Width() * 0.3f );
		const int columnWidth = titleColumn + valueColumn;

		{
			CString textQuantity;
			textQuantity.LoadString( IDS_STRING42 );

			CString textPosition;
			textPosition.LoadString( IDS_STRING68 );

			const char* title[] = {
				textQuantity,
				textPosition,
				""	// it must end with empty string 
			};

			CclientApp::CreateGrid( this, &mItemGrid, IDC_ITEM_OPTION_GRID, gridRect, title, FALSE );

			mItemGrid.SetColumnWidth( 0, titleColumn );
			mItemGrid.SetColumnWidth( 1, valueColumn );
			//mItemGrid.SetFixedColumnCount( mItemGrid.GetColumnCount() );

			//mItemGrid.ExpandToFit();
		}

		{
			CString textStrength;
			textStrength.LoadString( IDS_STRING70 );

			CString textDexterity;
			textDexterity.LoadString( IDS_STRING71 );

			CString textHealth;
			textHealth.LoadString( IDS_STRING72 );
	
			CString textIntelligence;
			textIntelligence.LoadString( IDS_STRING73 );
			CString textWisdom;
			textWisdom.LoadString( IDS_STRING74 );

			CString textLife;
			textLife.LoadString( IDS_STRING75 );

			CString textMana;
			textMana.LoadString( IDS_STRING76 );

			CString textLifeRecovery;
			textLifeRecovery.LoadString( IDS_STRING77 );

			CString textManaRecovery;
			textManaRecovery.LoadString( IDS_STRING78 );

			CString textPhysicalAttack;
			textPhysicalAttack.LoadString( IDS_STRING79 );

			CString textPhysicalDefence;
			textPhysicalDefence.LoadString( IDS_STRING80 );

			CString textMagicalAttack;
			textMagicalAttack.LoadString( IDS_STRING81 );

			CString textMagicalDefence;
			textMagicalDefence.LoadString( IDS_STRING82 );

			CString textMoveSpeed;
			textMoveSpeed.LoadString( IDS_STRING83 );

			CString textEvade;
			textEvade.LoadString( IDS_STRING84 );

			CString textAccuracy;
			textAccuracy.LoadString( IDS_STRING85 );

			CString textCriticalRate;
			textCriticalRate.LoadString( IDS_STRING86 );

			CString textCriticalValue;
			textCriticalValue.LoadString( IDS_STRING87 );

			CString textMaker;
			textMaker.LoadString( IDS_STRING88 );

			CString textLevel;
			textLevel.LoadString( IDS_STRING5 );

			CString textReinforce;
			textReinforce.LoadString( IDS_STRING89 );

			CString textMix;
			textMix.LoadString( IDS_STRING90 );

			CString textEnchant;
			textEnchant.LoadString( IDS_STRING91 );

			const CString title[] = {
				textReinforce + ": " + textStrength + "*",
				textReinforce + ": " + textDexterity + "*",
				textReinforce + ": " + textHealth + "*",
				textReinforce + ": " + textIntelligence + "*",
				textReinforce + ": " + textWisdom + "*",
				textReinforce + ": " + textLife + "*",
				textReinforce + ": " + textMana + "*",
				textReinforce + ": " + textLifeRecovery + "*",
				textReinforce + ": " + textManaRecovery + "*",
				textReinforce + ": " + textPhysicalAttack + "*",
				textReinforce + ": " + textPhysicalDefence + "*",
				textReinforce + ": " + textMagicalAttack + "*",
				textReinforce + ": " + textMagicalDefence + "*",
				textReinforce + ": " + textMoveSpeed + "*",
				textReinforce + ": " + textEvade + "*",
				textReinforce + ": " + textAccuracy + "*",
				textReinforce + ": " + textCriticalRate + "*",
				textReinforce + ": " + textCriticalValue + "*",
				textReinforce + ": " + textMaker + "*",
				textMix + ": " + textStrength + "*",
				textMix + ": " + textIntelligence + "*",
				textMix + ": " + textDexterity + "*",
				textMix + ": " + textWisdom + "*",
				textMix + ": " + textHealth + "*",
				textMix + ": " + textMaker + "*",
				textEnchant + ": " + textLevel + "*",
				textEnchant	+ ": " + textMaker + "*",
				""	// it must end with empty string
			};

			const int titleColumn = int( gridRect.Width() * 0.75f );
			const int valueColumn = int( gridRect.Width() * 0.25f );
			const int columnWidth = titleColumn + valueColumn;

			CclientApp::CreateGrid( this, &mOptionGrid, IDC_ITEM_DATA_GRID, gridRect, title, FALSE );

			mOptionGrid.SetColumnWidth( 0, titleColumn );
			mOptionGrid.SetColumnWidth( 1, valueColumn );
			mOptionGrid.SetFixedColumnCount( mOptionGrid.GetColumnCount() );

			mOptionGrid.ExpandToFit();
		}
	}

	// 콤보 박스
	{
		CString		text;

		for( DWORD i = 0; i <= TAB_STORAGE_NUM; ++i )
		{
			text.Format( "%d", i );

			mStorageSizeCombo.AddString( text );
		}

		CRect rect;
		mStorageSizeCombo.SetCurSel( 0 );
		mStorageSizeCombo.GetWindowRect( rect );
		mStorageSizeCombo.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );
	}

	// 에디트 컨트롤
	{
		mStorageMoneyEdit.SetWindowText( "0" );
	}

	// 탭 컨트롤
	{
		CString textInformation;
		textInformation.LoadString( IDS_STRING69 );
		mTabCtrl.InsertItem( mTabCtrl.GetItemCount(), textInformation );

		CString textOption;
		textOption.LoadString( IDS_STRING55 );
		mTabCtrl.InsertItem( mTabCtrl.GetItemCount(), textOption );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CItemDialog::AddItem( const Item& item )
{
	AddItem( item, mInventoryListCtrl );
}


void CItemDialog::AddItem( const Item& item, CListCtrl& listCtrl )
{
	const int	row		= listCtrl.GetItemCount();
	int			step	= 0;
	CString		text;

	text.Format( "%d", item.dwDBIdx );
	listCtrl.InsertItem( row, text, 0 );

	text.Format( "%d", item.wIconIdx );
	listCtrl.SetItemText( row, ++step, text );

	CString infomation;

	if(	Item::AreaCashStorage == item.mArea )
	{
		CString textBuying;
		textBuying.LoadString( IDS_STRING98 );
		infomation += textBuying;
	}

	listCtrl.SetItemText( row, ++step, mApplication.GetItemPositionType( item.mArea, item.Position ) );

	text.Format( "%d", item.Position );
	listCtrl.SetItemText( row, ++step, text );
	listCtrl.SetItemText( row, ++step, mApplication.GetItemName( item.wIconIdx ) );

	const BOOL isDuplicated = mApplication.IsDuplicateItem( item.wIconIdx );

	{
		++step;

		if( isDuplicated )
		{
			text.Format( "%d", item.Durability );

			listCtrl.SetItemText( row, step, text );
		}	
	}	

	{
		++step;

		if( ! isDuplicated )
		{
			if( mOptionMap.end() != mOptionMap.find( item.dwDBIdx ) )
			{
				CString textOption;
				textOption.LoadString( IDS_STRING99 );
				infomation += textOption;
			}

			switch( item.nSealed )
			{
			case eITEM_TYPE_SEAL:
				{
					CString textSealing;
					textSealing.LoadString( IDS_STRING100 );
					infomation += textSealing;
					break;
				}
			case eITEM_TYPE_UNSEAL:
				{
					CString textUnsealing;
					textUnsealing.LoadString( IDS_STRING101 );
					infomation += textUnsealing;
					break;
				}
			}

			if( item.nRemainSecond || item.LastCheckTime )
			{
				CString textTime;
				textTime.LoadString( IDS_STRING102 );
				infomation += textTime;
			}

			listCtrl.SetItemText( row, step, infomation );
		}		
	}
}


void CItemDialog::OnHdnItemclickItemDialogInventory(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	*pResult = 0;
}


void CItemDialog::OnLvnItemchangedItemDialogInventory(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 리스트 컨트롤의 아이템을 선택했을 때 들어온다

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	*pResult = 0;

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	mSelectedItemDbIndex	= atoi( mInventoryListCtrl.GetItemText( pNMLV->iItem, 0 ) );

	PutData( mSelectedItemDbIndex );

	{
		CItemLogDialog* dialog = mApplication.GetItemLogDialog();
		ASSERT( dialog );

		dialog->SetItemDbIndex( mServerIndex, mSelectedItemDbIndex );
	}

	//mTabCtrl.SetCurSel( 0 );
	NMHDR n;
	LONG_PTR p;
	OnTcnSelchangeItemTab( &n, &p );
}


void CItemDialog::OnBnClickedItemDialogDataUpdateButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	switch( mTabCtrl.GetCurSel() )
	{
	// item update
	case 0:
		{
			const ITEMBASE* item = GetItem( mSelectedItemDbIndex );

			if( ! item )
			{
				CString textThereIsNoTarget;
				textThereIsNoTarget.LoadString( IDS_STRING31 );

				MessageBox( textThereIsNoTarget, "", MB_OK );
				return;
			}

			const POSTYPE position = atoi( mItemGrid.GetItemText( 1, 1 ) );

			// 위치 중복 검사
			for( int row = 0; row < mInventoryListCtrl.GetItemCount(); ++row )
			{
				const DWORD itemDbIndex = atoi( mInventoryListCtrl.GetItemText( row, 0 ) );

				if( itemDbIndex == item->dwDBIdx )
				{
					continue;
				}

				if( position == atoi( mInventoryListCtrl.GetItemText( row, 3 ) ) )
				{
					CString textPositionIsDuplicated;
					textPositionIsDuplicated.LoadString( IDS_STRING105 );

					MessageBox( textPositionIsDuplicated, "", MB_OK | MB_ICONERROR );
					return;
				}
			}

			// 인벤토리의 아이템은 인벤토리 내에서만...
			// 창고 아이템은 창고 내에서만...
			if( position != item->Position )
			{
				CString textPositionIsOver;
				textPositionIsOver.LoadString( IDS_STRING106 );

				if( !	(	TP_INVENTORY_START	<= position			&& TP_INVENTORY_END		> position		 )	&&
					(	TP_INVENTORY_START	<= item->Position	&& TP_INVENTORY_END		> item->Position ) )
				{
					MessageBox( textPositionIsOver, "", MB_OK | MB_ICONERROR );
					return;
				}
				else if( !	(	TP_STORAGE_START	<= position			&& TABCELL_STORAGE_NUM * mStorageSize		> position			)	&&
					(	TP_STORAGE_START	<= item->Position	&& TABCELL_STORAGE_NUM * mStorageSize		> item->Position	) )
				{
					MessageBox( textPositionIsOver, "", MB_OK | MB_ICONERROR );
					return;
				}
				else if( !	(	TP_EXTENDED_INVENTORY1_START	<= position			&& TP_EXTENDED_INVENTORY1_END		> position		 )	&&
					(	TP_EXTENDED_INVENTORY1_START	<= item->Position	&& TP_EXTENDED_INVENTORY1_END		> item->Position ) )
				{
					MessageBox( textPositionIsOver, "", MB_OK | MB_ICONERROR );
					return;
				}
				else if( !	(	TP_EXTENDED_INVENTORY2_START	<= position			&& TP_EXTENDED_INVENTORY2_END		> position		 )	&&
					(	TP_EXTENDED_INVENTORY2_START	<= item->Position	&& TP_EXTENDED_INVENTORY2_END		> item->Position ) )
				{
					MessageBox( textPositionIsOver, "", MB_OK | MB_ICONERROR );
					return;
				}
				else if( !	(	TP_WEAR_START	<= position			&& TP_WEAR_END		> position		 )	&&
					(	TP_WEAR_START	<= item->Position	&& TP_WEAR_END		> item->Position ) )
				{
					MessageBox( textPositionIsOver, "", MB_OK | MB_ICONERROR );
					return;
				}
				else if( !	(	TP_SHOPITEM_START	<= position			&& TP_SHOPITEM_END		> position		 )	&&
					(	TP_SHOPITEM_START	<= item->Position	&& TP_SHOPITEM_END		> item->Position ) )
				{
					MessageBox( textPositionIsOver, "", MB_OK | MB_ICONERROR );
					return;
				}
			}

			MSG_RM_ITEM_UPDATE message;
			message.Category			= MP_RM_ITEM;
			message.Protocol			= MP_RM_ITEM_SET_SYN;
			message.dwObjectID			= mPlayerIndex;
			message.mItem				= *item;
			message.mItem.Position		= position;	

			if( mApplication.IsDuplicateItem( item->wIconIdx ) )
			{
				message.mItem.Durability	= min( atoi( mItemGrid.GetItemText( 0, 1 ) ), MAX_YOUNGYAKITEM_DUPNUM );

				if( ! message.mItem.Durability )
				{
					MessageBox( "수량이 최소 1개는 되어야 합니다", "", MB_OK | MB_ICONERROR );
					return;
				}
			}

			if( ! memcmp( item, &message.mItem, sizeof( *item ) ) )
			{
				// 변경된 내역이 없음
				return;
			}

			// 080304 LUJ, 로그
			{
				CString textUpdateItem;
				textUpdateItem.LoadString( IDS_STRING345 );

				mApplication.Log(
					textUpdateItem,
					mPlayerName,
					mPlayerIndex,
					mApplication.GetItemName( message.mItem.wIconIdx ),
					message.mItem.dwDBIdx,
					message.mItem.Durability,
					message.mItem.Position );
			}

			mApplication.Send( mServerIndex, message, sizeof( message ) );
			
			break;
		}
	// option update
	case 1:
		{
			const ITEMBASE* item = GetItem( mSelectedItemDbIndex );

			if( ! item )
			{
				CString textThereIsNoTarget;
				textThereIsNoTarget.LoadString( IDS_STRING31 );

				MessageBox( textThereIsNoTarget, "", MB_OK );
				return;
			}

			const DWORD		quantity	= atoi( mItemGrid.GetItemText( 0, 1 ) );

			if( mApplication.IsDuplicateItem( item->wIconIdx ) )
			{
				CString textItemCannotHaveOption;

				CString textItIsInvalidTarget;
				textItIsInvalidTarget.LoadString( IDS_STRING103 );

				MessageBox( textItIsInvalidTarget, "", MB_OK );
				return;
			}

			MSG_RM_ITEM_UPDATE_OPTION message;
			{
				ZeroMemory( &message, sizeof( message ) );

				message.Category	= MP_RM_ITEM;
				message.Protocol	= MP_RM_ITEM_SET_OPTION_SYN;
				message.dwObjectID	= mPlayerIndex;
			}

			if( mOptionMap.end() != mOptionMap.find( item->dwDBIdx ) )
			{
				message.mOption = mOptionMap[ item->dwDBIdx ];
			}

			// 값 저장
			{
				int cell = -1;

				message.mOption.mItemDbIndex = item->dwDBIdx;

				{
					ITEM_OPTION::Reinforce& data = message.mOption.mReinforce;

					data.mStrength			= atoi( mOptionGrid.GetItemText( ++cell, 1 ) );
					data.mDexterity			= atoi( mOptionGrid.GetItemText( ++cell, 1 ) );
					data.mVitality			= atoi( mOptionGrid.GetItemText( ++cell, 1 ) );
					data.mIntelligence		= atoi( mOptionGrid.GetItemText( ++cell, 1 ) );
					data.mWisdom			= atoi( mOptionGrid.GetItemText( ++cell, 1 ) );
					data.mLife				= atoi( mOptionGrid.GetItemText( ++cell, 1 ) );
					data.mMana				= atoi( mOptionGrid.GetItemText( ++cell, 1 ) );
					data.mLifeRecovery		= atoi( mOptionGrid.GetItemText( ++cell, 1 ) );
					data.mManaRecovery		= atoi( mOptionGrid.GetItemText( ++cell, 1 ) );
					data.mPhysicAttack		= atoi( mOptionGrid.GetItemText( ++cell, 1 ) );
					data.mPhysicDefence		= atoi( mOptionGrid.GetItemText( ++cell, 1 ) );
					data.mMagicAttack		= atoi( mOptionGrid.GetItemText( ++cell, 1 ) );
					data.mMagicDefence		= atoi( mOptionGrid.GetItemText( ++cell, 1 ) );
					data.mMoveSpeed			= atoi( mOptionGrid.GetItemText( ++cell, 1 ) );
					data.mEvade				= atoi( mOptionGrid.GetItemText( ++cell, 1 ) );
					data.mAccuracy			= atoi( mOptionGrid.GetItemText( ++cell, 1 ) );
					data.mCriticalRate		= atoi( mOptionGrid.GetItemText( ++cell, 1 ) );
					data.mCriticalDamage	= atoi( mOptionGrid.GetItemText( ++cell, 1 ) );
					StringCopySafe( data.mMadeBy, mOptionGrid.GetItemText( ++cell, 1 ), sizeof( data.mMadeBy ) );
				}

				{
					ITEM_OPTION::Mix& data = message.mOption.mMix;

					data.mStrength			= atoi( mOptionGrid.GetItemText( ++cell, 1 ) );
					data.mIntelligence		= atoi( mOptionGrid.GetItemText( ++cell, 1 ) );
					data.mDexterity			= atoi( mOptionGrid.GetItemText( ++cell, 1 ) );
					data.mWisdom			= atoi( mOptionGrid.GetItemText( ++cell, 1 ) );
					data.mVitality			= atoi( mOptionGrid.GetItemText( ++cell, 1 ) );
					StringCopySafe( data.mMadeBy, mOptionGrid.GetItemText( ++cell, 1 ), sizeof( data.mMadeBy ) );
				}

				// 아이템에 적용가능한 인챈트 인덱스를 얻어내고 레벨이 적당한지 체크해야한다
				{
					ITEM_OPTION::Enchant& data = message.mOption.mEnchant;

					data.mLevel	= BYTE( atoi( mOptionGrid.GetItemText( ++cell, 1 ) ) );

					if( data.mLevel )
					{
						if( ! data.mIndex )
						{
							data.mIndex	= mApplication.GetEnchantItemIndex( item->wIconIdx );
						}

						StringCopySafe( data.mMadeBy, mOptionGrid.GetItemText( ++cell, 1 ), sizeof( data.mMadeBy ) );

						const EnchantScript* script = mApplication.GetEnchantScript( data.mIndex );

						if( ! script )
						{
							CString textItIsInvalidTarget;
							textItIsInvalidTarget.LoadString( IDS_STRING103 );

							MessageBox( textItIsInvalidTarget, "", MB_OK );
							return;
						}
						else if( data.mLevel > script->mEnchantMaxLevel )
						{
							CString textItIsUnableToUpdateMoreEnchantLevel;
							textItIsUnableToUpdateMoreEnchantLevel.LoadString( IDS_STRING104 );

							CString text;
							text.Format( textItIsUnableToUpdateMoreEnchantLevel, script->mEnchantMaxLevel );

							MessageBox( text, 0, MB_OK );

							data.mLevel = BYTE( script->mEnchantMaxLevel );
						}
					}
				}
			}

			// 변경된 내역이 있는지 검사
			{
				OptionMap::const_iterator it = mOptionMap.find( item->dwDBIdx );

				if( mOptionMap.end() != it )
				{
					const ITEM_OPTION& previousOption = it->second;

					if( ! memcmp( &message.mOption, &previousOption, sizeof( previousOption ) ) )
					{
						return;
					}
				}
			}

			// 080304 LUJ, 로그
			{
				const ITEM_OPTION::Reinforce&	reinforce	= message.mOption.mReinforce;
				const ITEM_OPTION::Mix&			mix			= message.mOption.mMix;
				const ITEM_OPTION::Enchant&		enchant		= message.mOption.mEnchant;

				CString textItemOptionUpdate;
				textItemOptionUpdate.LoadString( IDS_STRING347 );

				mApplication.Log(
					textItemOptionUpdate,
					mPlayerName,
					mPlayerIndex,
					mApplication.GetItemName( item->wIconIdx ),
					item->dwDBIdx,
					reinforce.mStrength,
					reinforce.mDexterity,
					reinforce.mVitality,
					reinforce.mIntelligence,
					reinforce.mWisdom,
					reinforce.mLife,
					reinforce.mMana,
					reinforce.mLifeRecovery,
					reinforce.mManaRecovery,
					reinforce.mPhysicAttack,
					reinforce.mPhysicDefence,
					reinforce.mMagicAttack,
					reinforce.mMagicDefence,
					reinforce.mMoveSpeed,
					reinforce.mEvade,
					reinforce.mAccuracy,
					reinforce.mCriticalRate,
					reinforce.mCriticalDamage,
					CString( "" ) == reinforce.mMadeBy ? "*" : reinforce.mMadeBy,
					mix.mStrength,
					mix.mIntelligence,
					mix.mDexterity,
					mix.mWisdom,
					mix.mVitality,
					CString( "" ) == mix.mMadeBy ? "*" : mix.mMadeBy,
					enchant.mIndex,
					enchant.mLevel,
					CString( "" ) == enchant.mMadeBy ? "*" : enchant.mMadeBy );
			}			

			mApplication.Send( mServerIndex, message, sizeof( message ) );
			break;
		}
	}	
}


void CItemDialog::RefreshInventory()
{
	typedef std::multimap< POSTYPE, Item >	DataMap;
	DataMap										dataMap;

	// 위치 별로 정렬하기 위해 다시 컨테이너에 담는다
	for( ItemMap::const_iterator it = mItemMap.begin(); mItemMap.end() != it; ++it )
	{
		const Item& item = it->second;

		dataMap.insert( std::make_pair( item.Position, item ) );
	}

	typedef std::map< POSTYPE, int >	DuplicatedMap;
	DuplicatedMap						duplicatedMap;

	mInventoryListCtrl.DeleteAllItems();

	for(
		DataMap::const_iterator it = dataMap.begin();
		dataMap.end() != it;
		++it )
	{
		const Item& item = it->second;

		AddItem( item, mInventoryListCtrl );

		// 캐쉬 창고의 아이템 위치는 모두 0으로 DB에 세팅되어 있으므로, 중복 위치를 계산하지 않는다
		if( Item::AreaCashStorage == item.mArea )
		{
			continue;
		}

		++duplicatedMap[ item.Position ];
	}

	CString text;

	for(
		DuplicatedMap::const_iterator it = duplicatedMap.begin();
		duplicatedMap.end() != it;
		++it )
	{
		const POSTYPE	position	= it->first;
		const int		count		= it->second;

		if( 1 < count )
		{
			CString number;

			number.Format( "[%d]", position );

			text += number;
		}
	}

	if( ! text.IsEmpty() )
	{
		CString textPositionIsDuplicated;
		textPositionIsDuplicated.LoadString( IDS_STRING105 );

		MessageBox( textPositionIsDuplicated + text, "", MB_ICONERROR | MB_OK );
	}
}


void CItemDialog::ResetGrid()
{
	// 아이템 그리드 클리어
	{
		mItemGrid.ClearCells( CCellRange( 0, 1, mItemGrid.GetRowCount() - 1, 1 ) );
		mItemGrid.SetFixedColumnCount( mItemGrid.GetColumnCount() );
		mItemGrid.SetEditable( FALSE);
		mItemGrid.ShowWindow( SW_HIDE );
	}

	// 옵션 그리드 클리어
	{
		mOptionGrid.ClearCells( CCellRange( 0, 1, mOptionGrid.GetRowCount() - 1, 1 ) );
		mOptionGrid.SetFixedColumnCount( mOptionGrid.GetColumnCount() );
		mOptionGrid.SetEditable( FALSE );
		mOptionGrid.ShowWindow( SW_HIDE );
	}

	// 버튼 초기화
	{
		mItemGrid.ShowWindow( SW_HIDE );
		mOptionGrid.ShowWindow( SW_HIDE );
	}
}


void CItemDialog::PutData( DWORD itemDbIndex )
{
	//mSelectedPosition = atoi( mInventoryListCtrl.GetItemText( pNMLV->iItem, 0 ) );
	
	const ITEMBASE* item = GetItem( itemDbIndex );

	if( ! item )
	{
		ASSERT( 0 );
		return;
	}

	ResetGrid();

	//mItemUpdateButton.ShowWindow( SW_SHOW );

	const BOOL isDuplicated = mApplication.IsDuplicateItem( item->wIconIdx );

	// 아이템 기본 정보 처리
	{
		mItemGrid.ClearCells( CCellRange( 0, 1, mItemGrid.GetRowCount() - 1, 1 ) );
		mItemGrid.SetFixedColumnCount( 1 );
		mItemGrid.SetEditable( TRUE);
		//mItemGrid.ShowWindow( SW_SHOW );
		mItemGrid.SetFixedColumnCount( 1 );

		const DWORD	style	= DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
		GV_ITEM		cell	= { 0 };
		{
			cell.nFormat	= style;
			cell.col		= 1;
			cell.row		= 0;
			cell.mask		= GVIF_TEXT;
		}

		CclientApp::SetCellValue( item->Durability, mItemGrid, cell, isDuplicated ? GVIS_MODIFIED : GVIS_READONLY );
		++cell.row;

		CclientApp::SetCellValue( item->Position, mItemGrid, cell, GVIS_MODIFIED );
		++cell.row;
	}

	// 옵션 처리
	if( ! isDuplicated )
	{
		// 유료 아이템 정보 표시
		{
			const ITEM_INFO* info = mApplication.GetItemScript( item->wIconIdx );

			if( ! info )
			{
				return;
			}

			switch( info->nTimeKind )
			{
			case eKIND_REALTIME:
				{
					mEndTimeStatic.SetWindowText( GetDateString( item->nRemainSecond ) );
					mRemainTimeStatic.SetWindowText( "" );
					break;
				}
			case eKIND_PLAYTIME:
				{
					mEndTimeStatic.SetWindowText( "" );
					mRemainTimeStatic.SetWindowText( GetDateString( item->LastCheckTime ) );
					break;
				}
			default:
				{
					ASSERT( 0 );

					mEndTimeStatic.SetWindowText( "" );
					mRemainTimeStatic.SetWindowText( "" );
					break;
				}
			}
		}

		{
			mOptionGrid.ClearCells( CCellRange( 0, 1, mOptionGrid.GetRowCount() - 1, 1 ) );
			mOptionGrid.SetFixedColumnCount( 1 );
			mOptionGrid.SetEditable( TRUE );
			//mOptionGrid.ShowWindow( SW_SHOW );
		}

		OptionMap::const_iterator it = mOptionMap.find( item->dwDBIdx );

		if( mOptionMap.end() == it )
		{
			return;
		}

		const ITEM_OPTION& option = it->second;

		const DWORD	style	= DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
		GV_ITEM		cell	= { 0 };
		{
			cell.nFormat	= style;
			cell.col		= 1;
			cell.row		= 0;
			cell.mask		= GVIF_TEXT;
		}

		// 강화 옵션 정보
		{
			const ITEM_OPTION::Reinforce& data = option.mReinforce;

			CclientApp::SetCellValue( data.mStrength, mOptionGrid, cell, GVIS_MODIFIED );
			++cell.row;

			CclientApp::SetCellValue( data.mDexterity, mOptionGrid, cell, GVIS_MODIFIED );
			++cell.row;

			CclientApp::SetCellValue( data.mVitality, mOptionGrid, cell, GVIS_MODIFIED );
			++cell.row;

			CclientApp::SetCellValue( data.mIntelligence, mOptionGrid, cell, GVIS_MODIFIED );
			++cell.row;

			CclientApp::SetCellValue( data.mWisdom, mOptionGrid, cell, GVIS_MODIFIED );
			++cell.row;

			CclientApp::SetCellValue( data.mLife, mOptionGrid, cell, GVIS_MODIFIED );
			++cell.row;

			CclientApp::SetCellValue( data.mMana, mOptionGrid, cell, GVIS_MODIFIED );
			++cell.row;

			CclientApp::SetCellValue( data.mLifeRecovery, mOptionGrid, cell, GVIS_MODIFIED );
			++cell.row;

			CclientApp::SetCellValue( data.mManaRecovery, mOptionGrid, cell, GVIS_MODIFIED );
			++cell.row;

			CclientApp::SetCellValue( data.mPhysicAttack, mOptionGrid, cell, GVIS_MODIFIED );
			++cell.row;

			CclientApp::SetCellValue( data.mPhysicDefence, mOptionGrid, cell, GVIS_MODIFIED );
			++cell.row;

			CclientApp::SetCellValue( data.mMagicAttack, mOptionGrid, cell, GVIS_MODIFIED );
			++cell.row;

			CclientApp::SetCellValue( data.mMagicDefence, mOptionGrid, cell, GVIS_MODIFIED );
			++cell.row;

			CclientApp::SetCellValue( data.mMoveSpeed, mOptionGrid, cell, GVIS_MODIFIED );
			++cell.row;

			CclientApp::SetCellValue( data.mEvade, mOptionGrid, cell, GVIS_MODIFIED );
			++cell.row;

			CclientApp::SetCellValue( data.mAccuracy, mOptionGrid, cell, GVIS_MODIFIED );
			++cell.row;

			CclientApp::SetCellValue( data.mCriticalRate, mOptionGrid, cell, GVIS_MODIFIED );
			++cell.row;

			CclientApp::SetCellValue( data.mCriticalDamage, mOptionGrid, cell, GVIS_MODIFIED );
			++cell.row;

			CclientApp::SetCellValue( data.mMadeBy, mOptionGrid, cell, GVIS_MODIFIED );
			++cell.row;
		}

		// 조합 옵션
		{
			const ITEM_OPTION::Mix& data = option.mMix;

			CclientApp::SetCellValue( data.mStrength, mOptionGrid, cell, GVIS_MODIFIED );
			++cell.row;

			CclientApp::SetCellValue( data.mIntelligence, mOptionGrid, cell, GVIS_MODIFIED );
			++cell.row;

			CclientApp::SetCellValue( data.mDexterity, mOptionGrid, cell, GVIS_MODIFIED );
			++cell.row;

			CclientApp::SetCellValue( data.mWisdom, mOptionGrid, cell, GVIS_MODIFIED );
			++cell.row;

			CclientApp::SetCellValue( data.mVitality, mOptionGrid, cell, GVIS_MODIFIED );
			++cell.row;

			CclientApp::SetCellValue( data.mMadeBy, mOptionGrid, cell, GVIS_MODIFIED );
			++cell.row;
		}

		// 인챈트 옵션
		{
			const ITEM_OPTION::Enchant& data = option.mEnchant;

			CclientApp::SetCellValue( data.mLevel, mOptionGrid, cell, GVIS_MODIFIED );
			++cell.row;

			CclientApp::SetCellValue( data.mMadeBy, mOptionGrid, cell, GVIS_MODIFIED );
			++cell.row;
		}
	}		
}


ITEMBASE* CItemDialog::GetItem( DWORD itemDbIndex )
{
	ItemMap::iterator it = mItemMap.find( itemDbIndex );

	if( mItemMap.end() != it )
	{
		return &( it->second );
	}

	return 0;
}


void CItemDialog::OnBnClickedItemDataDeleteButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	switch( mTabCtrl.GetCurSel() )
	{
	// item delete
	case 0:
		{
			const ITEMBASE* item = GetItem( mSelectedItemDbIndex );

			if( ! item )
			{
				CString textThereIsNoTarget;
				textThereIsNoTarget.LoadString( IDS_STRING31 );

				MessageBox( textThereIsNoTarget, "", MB_ICONWARNING | MB_OK );
				return;
			}

			CString textDoYouDeleteItem;
			textDoYouDeleteItem.LoadString( IDS_STRING108 );

			if( IDNO == MessageBox( textDoYouDeleteItem, "", MB_ICONWARNING | MB_YESNO ) )
			{
				return;
			}

			// 080304 LUJ, 로그
			{
				CString textItemRemove;
				textItemRemove.LoadString( IDS_STRING348 );

				mApplication.Log(
					textItemRemove,
					mPlayerName,
					mPlayerIndex,
					mApplication.GetItemName( item->wIconIdx ),
					item->dwDBIdx );
			}

			MSG_DWORD message;
			{
				message.Category	= MP_RM_ITEM;
				message.Protocol	= MP_RM_ITEM_DELETE_SYN;
				message.dwObjectID	= mPlayerIndex;
				message.dwData		= item->dwDBIdx;
			}			
			
			mApplication.Send( mServerIndex, message, sizeof( message ) );
			break;
		}
	// option delete
	case 1:
		{
			const ITEMBASE* item = GetItem( mSelectedItemDbIndex );

			if( ! item )
			{
				ASSERT( 0 );
				return;
			}

			if(	mApplication.IsDuplicateItem( item->wIconIdx )	||
				mOptionMap.end() == mOptionMap.find( item->dwDBIdx ) )
			{
				return;
			}

			CString textDoYouClearOption;
			textDoYouClearOption.LoadString( IDS_STRING107 );

			if( IDNO == MessageBox( textDoYouClearOption, "", MB_ICONWARNING | MB_YESNO ) )
			{
				return;
			}

			// 080304 LUJ, 로그
			{
				CString textItemOptionRemove;
				textItemOptionRemove.LoadString( IDS_STRING349 );

				mApplication.Log(
					textItemOptionRemove,
					mPlayerName,
					mPlayerIndex,
					mApplication.GetItemName( item->wIconIdx ),
					item->dwDBIdx );
			}			

			MSG_DWORD message;
			{
				message.Category	= MP_RM_ITEM;
				message.Protocol	= MP_RM_ITEM_DELETE_OPTION_SYN;
				message.dwObjectID	= mPlayerIndex;
				message.dwData		= item->dwDBIdx;
			}			
			
			mApplication.Send( mServerIndex, message, sizeof( message ) );
			break;
		}
	}
}


void CItemDialog::Request( DWORD serverIndex, const CString& playerName, DWORD playerIndex, DWORD userIndex )
{
	mServerIndex	= serverIndex;
	mPlayerName		= playerName;
	mPlayerIndex	= playerIndex;
	mUserIndex		= userIndex;
	
	// 아이템 정보창은 피호출 처지이므로 누구에 의해 호출되었는지 알 수 없다. 타이틀을 직접 세팅해주자
	{
		CString textTitle;
		textTitle.LoadString( IDS_STRING109 );

		CString	text;
		text.Format( textTitle, mApplication.GetServerName( serverIndex ), playerName );

		CDialog* dialog = mApplication.GetItemDialog();
		ASSERT( dialog );

		dialog->SetWindowText( text );
	}

	mItemMap.clear();
	mOptionMap.clear();
	ResetGrid();

	// 컨트롤을 잠가놓고, 결과가 오면 활성화시킨다
	{
		mItemAddButton.EnableWindow( FALSE );
		mInventoryListCtrl.EnableWindow( FALSE );
		mTabCtrl.EnableWindow( FALSE );
		mStorageSizeCombo.EnableWindow( FALSE );
		mStorageMoneyEdit.EnableWindow( FALSE );
		mStorageUpdateButton.EnableWindow( FALSE );
	}

	{
		MSG_DWORD message;

		message.Category	= MP_RM_ITEM;
		message.Protocol	= MP_RM_ITEM_GET_SYN;
		message.dwObjectID	= playerIndex;
		message.dwData		= userIndex;
		
		mApplication.Send( serverIndex, message, sizeof( message ) );
	}
}


void CItemDialog::OnBnClickedItemInventoryLogButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	switch( mTabCtrl.GetCurSel() )
	{
	// item log
	case 0:
		{
			POSITION	position	= mInventoryListCtrl.GetFirstSelectedItemPosition();
			const DWORD	row			= mInventoryListCtrl.GetNextSelectedItem( position );

			const CString	itemDbIndex	= mInventoryListCtrl.GetItemText( row, 0 );

			if( itemDbIndex.IsEmpty() )
			{
				CString textThereIsNoTarget;
				textThereIsNoTarget.LoadString( IDS_STRING31 );

				MessageBox( textThereIsNoTarget, "", MB_ICONERROR | MB_OK );
				return;
			}

			CItemLogDialog* dialog = mApplication.GetItemLogDialog();
			ASSERT( dialog );

			dialog->SetWindowPos( &CWnd::wndTop, -1, -1, -1, -1, SWP_NOSIZE | SWP_NOMOVE );
			dialog->Request(
				mServerIndex,
				mInventoryListCtrl.GetItemText( row, 4 ), atoi( itemDbIndex ) );			
			break;
		}
	// option log
	case 1:
		{
			POSITION	position	= mInventoryListCtrl.GetFirstSelectedItemPosition();
			const DWORD	row			= mInventoryListCtrl.GetNextSelectedItem( position );

			const CString itemDbIndex	= mInventoryListCtrl.GetItemText( row, 0 );
			const CString itemName		= mInventoryListCtrl.GetItemText( row, 4 );

			CItemOptionLogDialog* dialog = mApplication.GetItemOptionLogDialog();
			ASSERT( dialog );

			dialog->SetWindowPos( &CWnd::wndTop, -1, -1, -1, -1, SWP_NOSIZE | SWP_NOMOVE );
            dialog->Request(
				mApplication.GetServerName( mServerIndex ),
				itemName,
				atoi( itemDbIndex ) );
			break;
		}
	}
}


void CItemDialog::OnBnClickedItemAddButton()
{
	Item item;
	ZeroMemory( &item, sizeof( item ) );

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	{
		POSTYPE itemInventoryPosition		= USHRT_MAX;
		POSTYPE itemPrivateStoragePosition	= USHRT_MAX;
		POSTYPE itemCashStoragePosition		= USHRT_MAX;

		std::set< DWORD > positionSet;

		for(
			int row = 0;
			row < mInventoryListCtrl.GetItemCount();
			++row )
		{
			const CString index = mInventoryListCtrl.GetItemText( row, 3 );

			positionSet.insert( atoi( index ) );
		}

		for(
			POSTYPE i = TP_INVENTORY_START;
			i < TP_INVENTORY_END + TABCELL_INVENTORY_NUM * mExtendedInventorySize;
			++i )
		{
			if( positionSet.end() == positionSet.find( i ) )
			{
				itemInventoryPosition = i;
				break;
			}
		}

		for(
			POSTYPE i = TP_STORAGE_START;
			i < TP_STORAGE_START + TABCELL_STORAGE_NUM * mStorageSize;
			++i )
		{
			if( positionSet.end() == positionSet.find( i ) )
			{
				itemPrivateStoragePosition = i;
				break;
			}
		}

		for(
			POSTYPE i = TP_SHOPITEM_START;
			i < TP_SHOPITEM_END;
			++i )
		{
			if( positionSet.end() == positionSet.find( i ) )
			{
				itemCashStoragePosition = i;
				break;
			}
		}

		if( USHRT_MAX == itemInventoryPosition		&&
			USHRT_MAX == itemPrivateStoragePosition	&&
			USHRT_MAX == itemCashStoragePosition )
		{
			CString textInventoryIsFull;
			textInventoryIsFull.LoadString( IDS_STRING110 );

			MessageBox( textInventoryIsFull, "", MB_ICONERROR | MB_OK );
			return;
		}

		CItemPositionDialog dialog;

		if( IDCANCEL == dialog.DoModal( itemInventoryPosition, itemPrivateStoragePosition, itemCashStoragePosition ) )
		{
			return;
		}

		if( Item::AreaNone == dialog.GetAreaType() )
		{
			CString textInventoryIsFull;
			textInventoryIsFull.LoadString( IDS_STRING110 );

			MessageBox( textInventoryIsFull, "", MB_ICONERROR | MB_OK );
			return;
		}

		item.mArea = dialog.GetAreaType();

		switch( dialog.GetAreaType() )
		{
		case Item::AreaInventory:
			{
				item.Position	= itemInventoryPosition;
				break;
			}
		case Item::AreaPrivateStorage:
			{
				item.Position	= itemPrivateStoragePosition;
				break;
			}
		case Item::AreaCashStorage:
			{
				item.Position	= itemCashStoragePosition;
				break;
			}
		}		
	}
	
	{
		CItemAddDialog dialog( mApplication );

		//if( IDCANCEL == dialog->DoModal( mApplication.GetServerName( mServerIndex ), mPlayerName, mPlayerIndex ) )
		if( IDCANCEL == dialog.DoModal() )
		{
			return;
		}

		item.wIconIdx = dialog.GetSelectedItemIndex();
	}

	const ITEM_INFO* info = mApplication.GetItemScript( item.wIconIdx );

	if( ! info )
	{
		return;
	}

	item.nSealed = info->wSeal;

	if( info->Stack )
	{
		CQuantityDialog dialog;

		if( IDOK != dialog.DoModal() )
		{
			return;
		}

		item.Durability = min( MAX_YOUNGYAKITEM_DUPNUM, dialog.GetQuantity() );
	}

	{
		CString textTitle;
		textTitle.LoadString( IDS_STRING111 );

		CString text;
		text.Format(
			textTitle,
			item.Position,
			mApplication.GetItemName( item.wIconIdx ) );

		if( IDNO == MessageBox( text, "", MB_ICONINFORMATION | MB_YESNO ) )
		{
			return;
		}
	}	

	{
		MSG_RM_ITEM_ADD message;
		ZeroMemory( &message, sizeof( message ) );

		message.Category		= MP_RM_ITEM;
		message.Protocol		= MP_RM_ITEM_ADD_SYN;
		message.mPlayerIndex	= mPlayerIndex;
		message.mUserIndex		= mUserIndex;
		message.mItem			= item;

		mApplication.Send( mServerIndex, message, sizeof( message ) );
	}	
}


CString CItemDialog::GetDateString( DWORD second ) const
{
	CString text;

	if( ! second )
	{
		return text;
	}

	const DWORD	minute	= 60;
	const DWORD hour	= 60 * minute;
	const DWORD day		= 24 * hour;
	
	{
		const DWORD value			= second / day;

		if( value )
		{
			CString textDayFormat;
			textDayFormat.LoadString( IDS_STRING112 );

			CString valueText;
			valueText.Format( textDayFormat, value );

			text += valueText + " ";
		}

		second %= day;
	}

	{
		const DWORD value			= second / hour;

		if( value )
		{
			CString textTimeFormat;
			textTimeFormat.LoadString( IDS_STRING113 );

			CString valueText;
			valueText.Format( textTimeFormat, value );

			text += valueText + " ";
		}

		second %= hour;
	}

	{
		const DWORD value			= second / minute;

		if( value )
		{
			CString textMinuteFormat;
			textMinuteFormat.LoadString( IDS_STRING114 );

			CString valueText;
			valueText.Format( textMinuteFormat, value );

			text += valueText + " ";
		}

		second %= minute;
	}

	{
		if( second )
		{
			CString textSecondFormat;
			textSecondFormat.LoadString( IDS_STRING115 );

			CString valueText;
			valueText.Format( textSecondFormat, second );

			text += valueText + " ";
		}
	}

	return text;
}


void CItemDialog::OnBnClickedItemEndTimeButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	const ITEMBASE* item = GetItem( mSelectedItemDbIndex );

	if( ! item )
	{
		return;
	}

	const ITEM_INFO* info = mApplication.GetItemScript( item->wIconIdx );

	if( ! info )
	{
		return;
	}
	else if( eKIND_REALTIME != info->nTimeKind )
	{
		return;
	}

	CDateDialog dialog;

	if( IDCANCEL == dialog.DoModal( item->nRemainSecond, GetDateString( item->nRemainSecond ) ) )
	{
		return;
	}
	else if( dialog.GetSecond() == item->nRemainSecond )
	{
		return;
	}
	// 080304 LUJ, 잔여 시간이 없는 아이템은 값을 수정할 수 없다
	else if( ! item->nRemainSecond )
	{
		return;
	}
	
	// 080304 LUJ, 로그
	{
		CString textItemValidTimeUpdate;
		textItemValidTimeUpdate.LoadString( IDS_STRING346 );

		mApplication.Log(
			textItemValidTimeUpdate,
			mPlayerName,
			mPlayerIndex,
			mApplication.GetItemName( item->wIconIdx ),
			item->dwDBIdx,
			dialog.GetSecond() - item->nRemainSecond );
	}	
	
	{
		MSG_INT2 message;
		message.Category	= MP_RM_ITEM;
		message.Protocol	= MP_RM_ITEM_UPDATE_END_TIME_SYN;
		message.nData1		= item->dwDBIdx;
		message.nData2		= dialog.GetSecond() - item->nRemainSecond;

		mApplication.Send( mServerIndex, message, sizeof( message ) );
	}
}

void CItemDialog::OnBnClickedItemRemainTimeButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	const ITEMBASE* item = GetItem( mSelectedItemDbIndex );

	if( ! item )
	{
		return;
	}

	const ITEM_INFO* info = mApplication.GetItemScript( item->wIconIdx );

	if( ! info )
	{
		return;
	}
	else if( eKIND_PLAYTIME != info->nTimeKind )
	{
		return;
	}

	CDateDialog dialog;

	if( IDCANCEL == dialog.DoModal( item->LastCheckTime, GetDateString( item->LastCheckTime ) ) )
	{
		return;
	}
	else if( dialog.GetSecond() == item->LastCheckTime )
	{
		return;
	}

	// 080304 LUJ, 로그
	{
		CString textItemEndTimeUpdate;
		textItemEndTimeUpdate.LoadString( IDS_STRING350 );

		mApplication.Log(
			textItemEndTimeUpdate,
			mPlayerName,
			mPlayerIndex,
			mApplication.GetItemName( item->wIconIdx ),
			item->dwDBIdx,
			dialog.GetSecond() - item->LastCheckTime );
	}	
	
	{
		MSG_INT2 message;
		message.Category	= MP_RM_ITEM;
		message.Protocol	= MP_RM_ITEM_UPDATE_REMAIN_TIME_SYN;
		message.nData1		= item->dwDBIdx;
		message.nData2		= dialog.GetSecond() - item->LastCheckTime;

		mApplication.Send( mServerIndex, message, sizeof( message ) );
	}
}


void CItemDialog::OnTcnSelchangeItemTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	switch( mTabCtrl.GetCurSel() )
	{
	case 0:
		{
			mItemGrid.ShowWindow( SW_SHOW );
			mOptionGrid.ShowWindow( SW_HIDE );
			break;
		}
	case 1:
		{
			mItemGrid.ShowWindow( SW_HIDE );
			mOptionGrid.ShowWindow( SW_SHOW );
			break;
		}
	}
}


void CItemDialog::OnBnClickedItemStorageUpdateButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	MSG_DWORD2 message;
	{
		ZeroMemory( &message, sizeof( message ) );

		DWORD size;
		{
			CString text;
			mStorageSizeCombo.GetLBText( mStorageSizeCombo.GetCurSel(), text );

			size = min( TAB_STORAGE_NUM, atoi( text ) );
		}
		
		DWORD money;
		{
			CString text;
			mStorageMoneyEdit.GetWindowText( text );

			money = atoi( text );			
		}		

		message.Category	= MP_RM_ITEM;
		message.Protocol	= MP_RM_ITEM_SET_STORAGE_SYN;
		message.dwObjectID	= mPlayerIndex;
		message.dwData1		= size;
		message.dwData2		= money;

		// 080304 LUJ, 로그
		{
			CString textStorageUpdate;
			textStorageUpdate.LoadString( IDS_STRING351 );

			mApplication.Log(
				textStorageUpdate,
				mPlayerName,
				mPlayerIndex,
				size,
				money );
		}	
	}

	mApplication.Send( mServerIndex, message, sizeof( message ) );
}
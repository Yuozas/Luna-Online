// ItemFindDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "client.h"
#include "ItemFindDialog.h"
#include "ItemDialog.h"


// CItemFindDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CItemFindDialog, CDialog)
CItemFindDialog::CItemFindDialog( CclientApp& application, CWnd* pParent /*=NULL*/)
	: CDialog(CItemFindDialog::IDD, pParent)
	, mApplication( application )
{
	EnableAutomation();

}

CItemFindDialog::~CItemFindDialog()
{
}

void CItemFindDialog::OnFinalRelease()
{
	// �ڵ�ȭ ��ü�� ���� ������ ������ �����Ǹ� OnFinalRelease��
	// ȣ��˴ϴ�. �⺻ Ŭ�������� �ڵ����� ��ü�� �����մϴ�.
	// �⺻ Ŭ������ ȣ���ϱ� ���� ��ü�� �ʿ��� �߰� ���� �۾���
	// �߰��Ͻʽÿ�.

	CDialog::OnFinalRelease();
}

void CItemFindDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control( pDX, IDC_ITEM_FIND_MORE_SEARCH_BUTTON, mMoreFindButton );
	DDX_Control( pDX, IDC_ITEM_FIND_DATA_LIST2, mItemListctrl );
	DDX_Control( pDX, IDC_ITEM_FIND_RESULT_LIST, mResultListCtrl );
	DDX_Control( pDX, IDC_ITEM_FIND_SERVER_COMBO, mServerComboBox );	
	DDX_Control( pDX, IDC_ITEM_FIND_KEYWORD, mKeywordEdit );
	DDX_Control(pDX, IDC_ITEM_FIND_VIEW_PLAYER, mViewPlayerButton);
	DDX_Control(pDX, IDC_ITEM_FIND_PAGE_STATIC, mPageStatic);
	DDX_Control(pDX, IDC_ITEM_FIND_PREV_PAGE_BUTTON, mPagePrevButton);
	DDX_Control(pDX, IDC_ITEM_FIND_NEXT_PAGE_BUTTON, mPageNextButton);
	DDX_Control(pDX, IDC_ITEM_FIND_RESULT_STATIC, mResultStatic);
	DDX_Control(pDX, IDC_ITEM_FIND_SEARCH_BUTTON, mFindButton);
}


BEGIN_MESSAGE_MAP(CItemFindDialog, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ITEM_FIND_DATA_LIST2, OnLvnItemchangedItemFindDataList)
	//ON_BN_CLICKED(IDC_ITEM_FIND_SEARCH, OnBnClickedItemFindSearch)
	//ON_NOTIFY(LVN_ITEMCHANGED, IDC_ITEM_FIND_RESULT_LIST, OnLvnItemchangedItemFindResultList)
	ON_BN_CLICKED(IDC_ITEM_FIND_PREV_PAGE_BUTTON, OnBnClickedItemFindPrevPageButton)
	ON_BN_CLICKED(IDC_ITEM_FIND_NEXT_PAGE_BUTTON, OnBnClickedItemFindNextPageButton)
	ON_BN_CLICKED(IDC_ITEM_FIND_MORE_SEARCH_BUTTON, OnBnClickedItemFindMoreSearchButton)
	ON_BN_CLICKED(IDC_ITEM_FIND_VIEW_PLAYER, OnBnClickedItemFindViewPlayer)
	ON_BN_CLICKED(IDC_ITEM_FIND_SEARCH_BUTTON, OnBnClickedItemFindSearchButton)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CItemFindDialog, CDialog)
END_DISPATCH_MAP()

// ����: IID_IItemFindDialog�� ���� ������ �߰��Ͽ�
// VBA���� ���� �������� ���ε��� �����մϴ�. 
// �� IID�� .IDL ���Ͽ� �ִ� dispinterface�� GUID�� ��ġ�ؾ� �մϴ�.

// {29008F9C-A7AA-496A-9588-1BA2DC2D6FE5}
static const IID IID_IItemFindDialog =
{ 0x29008F9C, 0xA7AA, 0x496A, { 0x95, 0x88, 0x1B, 0xA2, 0xDC, 0x2D, 0x6F, 0xE5 } };

BEGIN_INTERFACE_MAP(CItemFindDialog, CDialog)
	INTERFACE_PART(CItemFindDialog, IID_IItemFindDialog, Dispatch)
END_INTERFACE_MAP()


// CItemFindDialog �޽��� ó�����Դϴ�.


BOOL CItemFindDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	SetIcon( AfxGetApp()->LoadIcon( IDR_MAINFRAME ), FALSE );
	SetIcon( AfxGetApp()->LoadIcon( IDR_MAINFRAME ), TRUE );

	// ���� ����� ä���
	{
		CRect rect;
		mServerComboBox.GetWindowRect( rect );
		mServerComboBox.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );

		mApplication.SetServerComboBox( mServerComboBox );
	}	

	// ����Ʈ �ʱ�ȭ
	{
		const DWORD addStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
		CRect rect;

		// ������ ����� ä���
		{
			mItemListctrl.GetClientRect( rect );

			CString textIndex;
			textIndex.LoadString( IDS_STRING22 );
			mItemListctrl.InsertColumn( 0, textIndex, LVCFMT_LEFT, 0 );

			CString textName;
			textName.LoadString( IDS_STRING20 );
			mItemListctrl.InsertColumn( 1, textName, LVCFMT_LEFT, int( rect.Width() * 0.7f ) );

			CString textLevel;
			textLevel.LoadString( IDS_STRING5 );
			mItemListctrl.InsertColumn( 2, textLevel, LVCFMT_LEFT, int( rect.Width() * 0.18f ) );

			mItemListctrl.SetExtendedStyle( mItemListctrl.GetExtendedStyle() | addStyle );

			mApplication.SetItemList( mItemListctrl );
		}

		// ��� ����� �ʱ�ȭ
		{
			mResultListCtrl.GetClientRect( rect );

			CString textDbIndex;
			textDbIndex.LoadString( IDS_STRING13 );
			mResultListCtrl.InsertColumn( 0, textDbIndex, LVCFMT_LEFT, 0 );

			CString textName;
			textName.LoadString( IDS_STRING20 );
			mResultListCtrl.InsertColumn( 1, textName, LVCFMT_LEFT, int( rect.Width() * 0.7f ) );

			CString textQuantity;
			textQuantity.LoadString( IDS_STRING42 );
			mResultListCtrl.InsertColumn( 2, textQuantity, LVCFMT_LEFT, int( rect.Width() * 0.18f ) );

			mResultListCtrl.SetExtendedStyle( mResultListCtrl.GetExtendedStyle() | addStyle );

			//mApplication.SetItemList( mResultListCtrl );
		}
	}
	
	// ��� ���� �ʱ�ȭ
	{
		mPagePrevButton.EnableWindow( FALSE );
		mPageNextButton.EnableWindow( FALSE );
		mPageStatic.SetWindowText( "" );

		mViewPlayerButton.EnableWindow( FALSE );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CItemFindDialog::OnLvnItemchangedItemFindDataList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	*pResult = 0;

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	mKeywordEdit.SetWindowText( mItemListctrl.GetItemText( pNMLV->iItem, 0 ) );
}


void CItemFindDialog::Parse( DWORD serverIndex, const MSGROOT* message )
{
	//ShowWindow( SW_SHOW );

	switch( message->Protocol )
	{
	case MP_RM_ITEM_FIND_OWNER_ACK:
		{
			const MSG_RM_ITEM_OWNER* m = ( MSG_RM_ITEM_OWNER* )message;

			if( mResultListCtrl.GetItemCount() )
			{
				if( ! m->mSize )
				{
					CString textThereIsNoResult;
					textThereIsNoResult.LoadString( IDS_STRING1 );

					MessageBox( textThereIsNoResult, "", MB_ICONINFORMATION | MB_OK );
					return;
				}				
			}
			else if( ! m->mSize )
			{
				mViewPlayerButton.EnableWindow( FALSE );

				mResultStatic.SetWindowText( "" );

				CString textThereIsNoResult;
				textThereIsNoResult.LoadString( IDS_STRING1 );

				MessageBox( textThereIsNoResult, "", MB_ICONINFORMATION | MB_OK );
				return;
			}
			
			// add page
			{
				Page& page = mPageMap[ DWORD( mPageMap.size() ) ];

				for( DWORD i = 0; i < m->mSize; ++i )
				{
					const MSG_RM_ITEM_OWNER::Player& player = m->mPlayer[ i ];

					if( page.end() != page.find( player.mIndex ) )
					{
						ASSERT( 0 && "Duplicated index is found" );
						continue;
					}

					Player& data	= page[ player.mIndex ];
					data.mItemIndex	= m->mItemIndex;
					data.mItemSize	= player.mItemSize;
					data.mName		= player.mName;
				}
			}

			PutPage( DWORD( mPageMap.size() - 1 ) );

			const DWORD maxSize = sizeof( m->mPlayer ) / sizeof( m->mPlayer[ 0 ] );

			// ����/��Ʈ�� ����
			{
				mMoreFindButton.EnableWindow( maxSize == mResultListCtrl.GetItemCount() );

				CString text;
				text.Format( "%d/%d", mPageMap.size(), mPageMap.size() );
				mPageStatic.SetWindowText( text );

				mPageNextButton.EnableWindow( FALSE );
				mPagePrevButton.EnableWindow( 1 < mPageMap.size() );

				mViewPlayerButton.EnableWindow( TRUE );
			}

			{
				const size_t size = max( mPageMap.size() - 1, 0 ) * maxSize + mResultListCtrl.GetItemCount();

				CString textFormat;
				textFormat.LoadString( IDS_STRING116 );

				CString text;
				text.Format( textFormat, mApplication.GetItemName( m->mItemIndex ), mSelectedServerName, size );

				mResultStatic.SetWindowText( " " + text );
			}

			break;
		}
	case MP_RM_ITEM_FIND_OWNER_NACK:
		{
			ASSERT( 0 && "system fault detected" );
			break;
		}
	case MP_RM_ITEM_FIND_OWNER_NACK_BY_AUTH:
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


void CItemFindDialog::PutPage( DWORD pageIndex )
{
	//CDataExchange dx( this, TRUE );
	//DoDataExchange( &dx );

	mResultListCtrl.DeleteAllItems();

	if( mPageMap.end() == mPageMap.find( pageIndex ) )
	{
		ASSERT( 0 );
		return;
	}

	const Page& page = mPageMap[ pageIndex ];

	CString	text;
	int		i = 0;

	for( Page::const_iterator it = page.begin(); page.end() != it; ++it, ++i )
	{
		const DWORD		playerIndex = it->first;
		const Player&	player		= it->second;

		text.Format( "%d", playerIndex );
        mResultListCtrl.InsertItem( i, text, 0 );

		mResultListCtrl.SetItemText( i, 1, player.mName );

		text.Format( "%d", player.mItemSize );
		mResultListCtrl.SetItemText( i, 2, text );
	}

	//UpdateData( TRUE );
}


int CItemFindDialog::GetCurrentPage() const
{
	CString text;
	mPageStatic.GetWindowText( text );

	int i = 0;
	CString token = text.Tokenize( "/ ", i );

	return token.IsEmpty() ? 0 : atoi( token ) - 1;
}



void CItemFindDialog::OnBnClickedItemFindPrevPageButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// ���� �������� �̵�

	int page = GetCurrentPage();

	if( 1 >  page )
	{
		return;
	}

	PutPage( --page );

	if( ! page )
	{
		mPagePrevButton.EnableWindow( FALSE );
	}

	mPageNextButton.EnableWindow( TRUE );

	{
		CString text;
		text.Format( "%d/%d", page + 1, mPageMap.size() );
		mPageStatic.SetWindowText( text );
	}
}


void CItemFindDialog::OnBnClickedItemFindNextPageButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// ���� �������� �̵�

	int			page	= GetCurrentPage();
	const int	maxPage	= max( 0, int( mPageMap.size() ) - 1 );

	if( maxPage == page )
	{
		return;
	}

	PutPage( ++page );

	if( maxPage == page )
	{
		mPageNextButton.EnableWindow( FALSE );
	}

	mPagePrevButton.EnableWindow( TRUE );

	{
		CString text;
		text.Format( "%d/%d", page + 1, mPageMap.size() );
		mPageStatic.SetWindowText( text );
	}
}

void CItemFindDialog::OnBnClickedItemFindMoreSearchButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// ��� ã�⸦ Ŭ��

	if( mPageMap.empty() )
	{
		return;
	}

	const Page& page = mPageMap[ DWORD( mPageMap.size() ) - 1 ];

	if( page.empty() )
	{
		return;
	}

	Page::const_reverse_iterator it = page.rbegin();

	const DWORD		playerIndex = it->first;
	const Player	player		= it->second;

	// ����
	{
		MSG_DWORD2 message;

		message.Category	= MP_RM_ITEM_FIND;
		message.Protocol	= MP_RM_ITEM_FIND_OWNER_SYN;
		//message.dwObjectID	= mApplication.GetPlayerIndex();
		message.dwData1		= player.mItemIndex;
		message.dwData2		= playerIndex;

		mApplication.Send( mSelectedServerName, message, sizeof( message ) );
	}
}
void CItemFindDialog::OnBnClickedItemFindViewPlayer()
{
	/*
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	POSITION	position	= mResultListCtrl.GetFirstSelectedItemPosition();
	const int	selectedRow	= mResultListCtrl.GetNextSelectedItem( position );

	const DWORD playerIndex = atoi( mResultListCtrl.GetItemText( selectedRow, 0 ) );

	if( ! playerIndex )
	{
		CString textThereIsNoTarget;
		textThereIsNoTarget.LoadString( IDS_STRING31);

		MessageBox( textThereIsNoTarget, "", MB_ICONWARNING | MB_OK );
		return;
	}

	{
		CItemDialog* dialog = mApplication.GetItemDialog();
		ASSERT( dialog );

		const CString playerIndex	= mResultListCtrl.GetItemText( selectedRow, 0 );
		const CString playerName	= mResultListCtrl.GetItemText( selectedRow, 1 );

		dialog->Request( mApplication.GetServerIndex( mSelectedServerName ), playerName, atoi( playerIndex ) );
	}
	*/
}


void CItemFindDialog::OnBnClickedItemFindSearchButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CString text;
	mKeywordEdit.GetWindowText( text );

	const DWORD itemIndex = atoi( text );

	if( ! strlen( mApplication.GetItemName( itemIndex ) ) )
	{
		CString textItIsNoExistedItem;
		textItIsNoExistedItem.LoadString( IDS_STRING33 );

		MessageBox( textItIsNoExistedItem, "", MB_ICONQUESTION | MB_OK );
		return;
	}

	// ������ ���� �ʱ�ȭ
	{
		mPagePrevButton.EnableWindow( FALSE );
		mPageNextButton.EnableWindow( FALSE );
		mPageStatic.SetWindowText( "" );
	}

	//mResultListCtrl.DeleteAllItems();
	mPageMap.clear();

	// ����
	{
		MSG_DWORD2 message;

		message.Category	= MP_RM_ITEM_FIND;
		message.Protocol	= MP_RM_ITEM_FIND_OWNER_SYN;
		//message.dwObjectID	= mApplication.GetPlayerIndex();
		message.dwData1		= itemIndex;
		message.dwData2		= 0;

		//CNetwork& network = CNetwork::GetInstance();

		mServerComboBox.GetLBText( mServerComboBox.GetCurSel(), mSelectedServerName );

		mApplication.Send( mSelectedServerName, message, sizeof( message ) );
	}	
}
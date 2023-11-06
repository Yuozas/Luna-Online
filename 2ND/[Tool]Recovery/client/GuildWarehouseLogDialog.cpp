// GuildItemLogDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "client.h"
#include "GuildWarehouseLogDialog.h"
#include "NewUserDialog.h"
#include "ItemDialog.h"
#include ".\guildwarehouselogdialog.h"


// CGuildWarehouseLogDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CGuildWarehouseLogDialog, CDialog)
CGuildWarehouseLogDialog::CGuildWarehouseLogDialog( CclientApp& application, CWnd* pParent /*=NULL*/)
	: CDialog(CGuildWarehouseLogDialog::IDD, pParent)
	, mApplication( application )
{
	EnableAutomation();

}

CGuildWarehouseLogDialog::~CGuildWarehouseLogDialog()
{
}

void CGuildWarehouseLogDialog::OnFinalRelease()
{
	// �ڵ�ȭ ��ü�� ���� ������ ������ �����Ǹ� OnFinalRelease��
	// ȣ��˴ϴ�. �⺻ Ŭ�������� �ڵ����� ��ü�� �����մϴ�.
	// �⺻ Ŭ������ ȣ���ϱ� ���� ��ü�� �ʿ��� �߰� ���� �۾���
	// �߰��Ͻʽÿ�.

	CDialog::OnFinalRelease();
}

void CGuildWarehouseLogDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GUILD_ITEM_LOG_RESULT_STATIC, mResultStatic);
	DDX_Control(pDX, IDC_GUILD_ITEM_LOG_RESULT_PROGRESS, mResultProgressCtrl);
	DDX_Control(pDX, IDC_GUILD_ITEM_LOG_RESULT_LIST, mResultListCtrl);
	DDX_Control(pDX, IDC_GUILD_ITEM_LOG_FIND_BUTTON, mFindButton);
	DDX_Control(pDX, IDC_GUILD_ITEM_LOG_STOP_BUTTON, mStopButton);
	DDX_Control(pDX, IDC_GUILD_WAREHOUSE_BEGIN_DATE_CTRL, mBeginDateCtrl);
	DDX_Control(pDX, IDC_GUILD_WAREHOUSE_END_DATE_CTRL, mEndDateCtrl);
	DDX_Control(pDX, IDC_GUILD_WAREHOUSE_BEGIN_TIME_CTRL, mBeginTimeCtrl);
	DDX_Control(pDX, IDC_GUILD_WAREHOUSE_END_TIME_CTRL, mEndTimeCtrl);
}


BEGIN_MESSAGE_MAP(CGuildWarehouseLogDialog, CDialog)
	ON_BN_CLICKED(IDC_GUILD_ITEM_LOG_FIND_BUTTON, OnBnClickedGuildItemLogFindButton)
	ON_BN_CLICKED(IDC_GUILD_ITEM_LOG_STOP_BUTTON, OnBnClickedGuildItemLogStopButton)
	ON_BN_CLICKED(IDC_GUILD_ITEM_LOG_EXCEL_BUTTON, OnBnClickedGuildItemLogExcelButton)
	ON_BN_CLICKED(IDC_GUILD_ITEM_LOG_PLAYER_BUTTON, OnBnClickedGuildItemLogPlayerButton)
	ON_BN_CLICKED(IDC_GUILD_ITEM_LOG_ITEM_BUTTON, OnBnClickedGuildItemLogPlayerButton2)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CGuildWarehouseLogDialog, CDialog)
END_DISPATCH_MAP()

// ����: IID_IGuildItemLogDialog�� ���� ������ �߰��Ͽ�
// VBA���� ���� �������� ���ε��� �����մϴ�. 
// �� IID�� .IDL ���Ͽ� �ִ� dispinterface�� GUID�� ��ġ�ؾ� �մϴ�.

// {74AE2142-05E1-49EA-BBED-C50C6C17A4FB}
static const IID IID_IGuildItemLogDialog =
{ 0x74AE2142, 0x5E1, 0x49EA, { 0xBB, 0xED, 0xC5, 0xC, 0x6C, 0x17, 0xA4, 0xFB } };

BEGIN_INTERFACE_MAP(CGuildWarehouseLogDialog, CDialog)
	INTERFACE_PART(CGuildWarehouseLogDialog, IID_IGuildItemLogDialog, Dispatch)
END_INTERFACE_MAP()


// CGuildWarehouseLogDialog �޽��� ó�����Դϴ�.


BOOL CGuildWarehouseLogDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	// ����Ʈ ��Ʈ�� �ʱ�ȭ
	{
		const DWORD addStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
		CRect rect;

		// ������ ����� ä���
		{
			int step = -1;

			mResultListCtrl.GetClientRect( rect );

			CString textDate;
			textDate.LoadString( IDS_STRING3 );
			mResultListCtrl.InsertColumn( ++step, textDate,			LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

			CString textType;
			textType.LoadString( IDS_STRING4 );
			mResultListCtrl.InsertColumn( ++step, textType,			LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

			CString textPlayer;
			textPlayer.LoadString( IDS_STRING54 );
			mResultListCtrl.InsertColumn( ++step, textPlayer,		LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

			CString textName;
			textName.LoadString( IDS_STRING20 );
			mResultListCtrl.InsertColumn( ++step, textName,			LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

			CString textDbIndex;
			textDbIndex.LoadString( IDS_STRING13 );
			mResultListCtrl.InsertColumn( ++step, textDbIndex,	LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

			CString textQuantity;
			textQuantity.LoadString( IDS_STRING42 );
			mResultListCtrl.InsertColumn( ++step, textQuantity,		LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

			CString textOption;
			textOption.LoadString( IDS_STRING55 );
			mResultListCtrl.InsertColumn( ++step, textOption,		LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

			CString textStartPosition;
			textStartPosition.LoadString( IDS_STRING56 );
			mResultListCtrl.InsertColumn( ++step, textStartPosition,	LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

			CString textEndPosition;
			textEndPosition.LoadString( IDS_STRING57 );
			mResultListCtrl.InsertColumn( ++step, textEndPosition,	LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

			CString textMoney;
			textMoney.LoadString( IDS_STRING58 );
			mResultListCtrl.InsertColumn( ++step, textMoney,		LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

			CString textMemo;
			textMemo.LoadString( IDS_STRING17 );
			mResultListCtrl.InsertColumn( ++step, textMemo,			LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

			mResultListCtrl.SetExtendedStyle( mResultListCtrl.GetExtendedStyle() | addStyle );
		}
	}

	// �޺��ڽ� �ʱ�ȭ
	{		
	}

	// ��ư �ʱ�ȭ
	{
		mStopButton.EnableWindow( FALSE );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


INT_PTR CGuildWarehouseLogDialog::DoModal( DWORD serverIndex, const CString& guildName, DWORD guildIndex )
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	mServerIndex	= serverIndex;
	mGuildName		= guildName;
	mGuildIndex		= guildIndex;

	return CDialog::DoModal();
}


void CGuildWarehouseLogDialog::OnBnClickedGuildItemLogFindButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	mLogIndexSet.clear();

	MSG_RM_GUILD_LOG_REQUEST message;

	message.Category	= MP_RM_GUILD_WAREHOUSE_LOG;
	message.Protocol	= MP_RM_GUILD_WAREHOUSE_LOG_SIZE_SYN;
	message.mGuildIndex	= mGuildIndex;

	{
		CTime date;
		CTime time;

		mBeginDateCtrl.GetTime( date );
		mBeginTimeCtrl.GetTime( time );

		sprintf( message.mBeginDate, "%02d%02d%02d %02d:%02d:%02d",
			max( 0, date.GetYear() - 2000 ),
			date.GetMonth(),
			date.GetDay(),
			time.GetHour(),
			time.GetMinute(),
			time.GetSecond() );

		mEndDateCtrl.GetTime( date );
		mEndTimeCtrl.GetTime( time );

		sprintf( message.mEndDate, "%02d%02d%02d %02d:%02d:%02d",
			max( 0, date.GetYear() - 2000 ),
			date.GetMonth(),
			date.GetDay(),
			time.GetHour(),
			time.GetMinute(),
			time.GetSecond() );
	}

	mApplication.Send( mServerIndex, message, sizeof( message ) );
}


void CGuildWarehouseLogDialog::OnBnClickedGuildItemLogStopButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	MSGROOT message;
	message.Category	= MP_RM_GUILD_WAREHOUSE_LOG;
	message.Protocol	= MP_RM_GUILD_WAREHOUSE_LOG_STOP_SYN;

	mApplication.Send( mServerIndex, message, sizeof( message ) );
}


void CGuildWarehouseLogDialog::OnBnClickedGuildItemLogExcelButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	CString caption;
	GetWindowText( caption );

	CString title;
	title.Format( "%s-%s", caption, mGuildName );
	
	CString textLog;
	textLog.LoadString( IDS_STRING10 );
	CclientApp::SaveToExcel( title, textLog, mResultListCtrl );
}


void CGuildWarehouseLogDialog::Parse( DWORD serverIndex, const MSGROOT* message )
{
	mServerIndex = serverIndex;

	switch( message->Protocol )
	{
	case MP_RM_GUILD_WAREHOUSE_LOG_GET_ACK:
		{
			CString text;
			const MSG_RM_GUILD_WAREHOUSE_LOG* m = ( MSG_RM_GUILD_WAREHOUSE_LOG* )message;

			for( DWORD i = 0; i < m->mSize; ++i )
			{
				const MSG_RM_GUILD_WAREHOUSE_LOG::Log&	data	= m->mLog[ i ];
				const DWORD								row		= mResultListCtrl.GetItemCount();
				int										step	= 0;

				if( mLogIndexSet.end() == mLogIndexSet.find( data.mLogIndex ) )
				{
					mLogIndexSet.insert( data.mLogIndex );
				}
				else
				{
					continue;
				}

				mResultListCtrl.InsertItem( row, data.mDate, 0 );
				mResultListCtrl.SetItemText( row, ++step, mApplication.GetLogText( data.mType ) );

				text.Format( "%d", data.mPlayerIndex );
				mResultListCtrl.SetItemText( row, ++step, text );

				// ������ ���� ǥ��
				{
					const ITEMBASE& item = data.mItem;

					mResultListCtrl.SetItemText( row, ++step, mApplication.GetItemName( item.wIconIdx ) );
				
					text.Format( "%d", item.dwDBIdx );
					mResultListCtrl.SetItemText( row, ++step, text );

					if( mApplication.IsDuplicateItem( item.wIconIdx ) )
					{
						text.Format( "%d", item.Durability );
						mResultListCtrl.SetItemText( row, ++step, text );
					}
					else if( item.Durability )
					{
						mResultListCtrl.SetItemText( row, ++step, "*" );
					}

					text.Format( "%d", data.mItemFromPosition );
					mResultListCtrl.SetItemText( row, ++step, text );

					text.Format( "%d", item.Position );
					mResultListCtrl.SetItemText( row, ++step, text );
				}

				text.Format( "%u", data.mMoney );
				mResultListCtrl.SetItemText( row, ++step, text );
				
				mResultListCtrl.SetItemText( row, ++step, data.mMemo );
			}

			{
				int minRange;
				int maxRange;
				mResultProgressCtrl.GetRange( minRange, maxRange );

				mResultProgressCtrl.SetPos( mResultProgressCtrl.GetPos() + int( m->mSize ) );

				text.Format( "%d/%d", mResultProgressCtrl.GetPos(), maxRange );
				mResultStatic.SetWindowText( text );

				if( mResultListCtrl.GetItemCount() == maxRange )
				{
					mFindButton.EnableWindow( TRUE );
					mStopButton.EnableWindow( FALSE );
				}
			}

			break;
		}
	case MP_RM_GUILD_WAREHOUSE_LOG_SIZE_ACK:
		{
			const MSG_DWORD* m = ( MSG_DWORD* )message;

			const DWORD size = m->dwData;

			CString text;
			text.Format( "0/%d", size );

			mResultStatic.SetWindowText( text );
			mResultProgressCtrl.SetRange32( 0, size );
			mResultProgressCtrl.SetPos( 0 );
			mFindButton.EnableWindow( FALSE );
			mStopButton.EnableWindow( TRUE );

			mResultListCtrl.DeleteAllItems();
			break;
		}
	case MP_RM_GUILD_WAREHOUSE_LOG_SIZE_NACK:
		{
			CString textThereIsNoResult;
			textThereIsNoResult.LoadString( IDS_STRING1 );

			MessageBox( textThereIsNoResult, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_GUILD_WAREHOUSE_LOG_STOP_ACK:
		{
			mFindButton.EnableWindow( TRUE );
			mStopButton.EnableWindow( FALSE );

			CString textSearchWasStopped;
			textSearchWasStopped.LoadString( IDS_STRING2 );

			MessageBox( textSearchWasStopped, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_GUILD_WAREHOUSE_LOG_NACK_BY_AUTH:
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



void CGuildWarehouseLogDialog::OnBnClickedGuildItemLogPlayerButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	POSITION	position = mResultListCtrl.GetFirstSelectedItemPosition();
	const DWORD row		 = mResultListCtrl.GetNextSelectedItem( position );

	const CString playerIndex = mResultListCtrl.GetItemText( row, 2 );

	if( playerIndex.IsEmpty() )
	{
		CString textThereIsNoTarget;
		textThereIsNoTarget.LoadString( IDS_STRING31 );

		MessageBox( textThereIsNoTarget, "", MB_ICONERROR | MB_OK );
		return;
	}
	else if( "0" == playerIndex )
	{
		return;
	}

	{
		CNewUserDialog* dialog = mApplication.GetNewUserDialog();
		ASSERT( dialog );

		dialog->FindPlayer( mServerIndex, atoi( playerIndex ) );
	}
}


void CGuildWarehouseLogDialog::OnBnClickedGuildItemLogPlayerButton2()
{
	/*
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	POSITION	position = mResultListCtrl.GetFirstSelectedItemPosition();
	const DWORD row		 = mResultListCtrl.GetNextSelectedItem( position );

	const CString playerIndex = mResultListCtrl.GetItemText( row, 2 );

	if( playerIndex.IsEmpty() )
	{
		CString textThereIsNoTarget;
		textThereIsNoTarget.LoadString( IDS_STRING31 );

		MessageBox( textThereIsNoTarget, "", MB_ICONERROR | MB_OK );
		return;
	}
	else if( "0" == playerIndex )
	{
		return;
	}

	{
		CItemDialog* dialog = mApplication.GetItemDialog();
		ASSERT( dialog );

		dialog->Request( mServerIndex, playerIndex, atoi( playerIndex ) );
	}
	*/
}


void CGuildWarehouseLogDialog::OnCancel()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	mResultListCtrl.DeleteAllItems();

	CDialog::OnCancel();
}

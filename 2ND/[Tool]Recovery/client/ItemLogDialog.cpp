// ItemLogDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "client.h"
#include "ItemLogDialog.h"
#include "ItemOptionLogDialog.h"
#include "ItemAddDialog.h"
#include ".\itemlogdialog.h"


// CItemLogDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CItemLogDialog, CDialog)
CItemLogDialog::CItemLogDialog( CclientApp& application, CWnd* pParent /*=NULL*/)
	: CDialog(CItemLogDialog::IDD, pParent)
	, mApplication( application )
{
	EnableAutomation();
}

CItemLogDialog::~CItemLogDialog()
{
}

void CItemLogDialog::OnFinalRelease()
{
	// 자동화 개체에 대한 마지막 참조가 해제되면 OnFinalRelease가
	// 호출됩니다. 기본 클래스에서 자동으로 개체를 삭제합니다.
	// 기본 클래스를 호출하기 전에 개체에 필요한 추가 정리 작업을
	// 추가하십시오.

	CDialog::OnFinalRelease();
}

void CItemLogDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_ITEM_LOG_COMBO, mServerCombo);
	DDX_Control(pDX, IDC_ITEM_LOG_DBIDX_EDIT, mKeywordEdit);
	DDX_Control(pDX, IDC_ITEM_LOG_FIND_BUTTON, mFindButton);
	DDX_Control(pDX, IDC_ITEM_LOG_STOP_BUTTON, mStopButton);
	DDX_Control(pDX, IDC_ITEM_LOG_EXCEL_BUTTON, mExcelButton);
	DDX_Control(pDX, IDC_ITEM_LOG_RESULT_STATIC, mResultStatic);
	DDX_Control(pDX, IDC_ITEM_LOG_RESULT_PROGRESS, mResultProgress);
	DDX_Control(pDX, IDC_ITEM_LOG_RESULT_LIST, mResultListCtrl);
	DDX_Control(pDX, IDC_ITEM_LOG_COMBO2, mServerCombo);
	DDX_Control(pDX, IDC_ITEM_LOG_ITEM_INDEX_RADIO, mDbIndexRadio);
	DDX_Control(pDX, IDC_ITEM_LOG_DB_INDEX_RADIO, mItemIndexRadio);
	DDX_Control(pDX, IDC_ITEM_LOG_PLAYER_DB_IDX_RADIO, mPlayerIndexRadio);
	DDX_Control(pDX, IDC_ITEM_LOG_BEGIN_DATE_PICKER, mBeginDateCtrl);
	DDX_Control(pDX, IDC_ITEM_LOG_BEGIN_TIME_PICKER, mBeginTimeCtrl);
	DDX_Control(pDX, IDC_DATETIMEPICKER3, mEndDateCtrl);
	DDX_Control(pDX, IDC_DATETIMEPICKER4, mEndTimeCtrl);
}


BEGIN_MESSAGE_MAP(CItemLogDialog, CDialog)
	ON_BN_CLICKED(IDC_ITEM_LOG_FIND_BUTTON, OnBnClickedItemLogFindButton)
	ON_BN_CLICKED(IDC_ITEM_LOG_STOP_BUTTON, OnBnClickedItemLogStopButton)
	ON_BN_CLICKED(IDC_ITEM_LOG_EXCEL_BUTTON, OnBnClickedItemLogExcelButton)
	ON_BN_CLICKED(IDC_ITEM_LOG_OPTION_LOG_BUTTON, OnBnClickedItemLogOptionLogButton)
	ON_BN_CLICKED(IDC_ITEM_LOG_DB_INDEX_RADIO, OnBnClickedItemLogDbIndexRadio)
	ON_BN_CLICKED(IDC_ITEM_LOG_COPY_BUTTON, OnBnClickedItemLogCopyButton)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CItemLogDialog, CDialog)
END_DISPATCH_MAP()

// 참고: IID_IItemLogDialog에 대한 지원을 추가하여
// VBA에서 형식 안정적인 바인딩을 지원합니다. 
// 이 IID는 .IDL 파일에 있는 dispinterface의 GUID와 일치해야 합니다.

// {9D35A2B8-64F8-4C54-82FB-AAAEF47B4B51}
static const IID IID_IItemLogDialog =
{ 0x9D35A2B8, 0x64F8, 0x4C54, { 0x82, 0xFB, 0xAA, 0xAE, 0xF4, 0x7B, 0x4B, 0x51 } };

BEGIN_INTERFACE_MAP(CItemLogDialog, CDialog)
	INTERFACE_PART(CItemLogDialog, IID_IItemLogDialog, Dispatch)
END_INTERFACE_MAP()


// CItemLogDialog 메시지 처리기입니다.

void CItemLogDialog::OnBnClickedItemLogFindButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	MSG_RM_ITEM_LOG_REQUEST message;
	ZeroMemory( &message, sizeof( message ) );

	message.Category		= MP_RM_ITEM_LOG;
	message.Protocol		= MP_RM_ITEM_LOG_GET_SIZE_SYN;
	message.mRequestTick	= mRequestTick = GetTickCount();

	{
		CString index;
		mKeywordEdit.GetWindowText( index );

		if( index.IsEmpty() )
		{
			CString textThereIsNoTarget;
			textThereIsNoTarget.LoadString( IDS_STRING31 );

			MessageBox( textThereIsNoTarget, "", MB_ICONERROR | MB_OK );
			return;
		}

		if( mItemIndexRadio.GetCheck() )
		{
			message.mItemIndex = atoi( index );
		}
		else if( mDbIndexRadio.GetCheck() )
		{
			message.mItemDbIndex = atoi( index );
		}
		else if( mPlayerIndexRadio.GetCheck() )
		{
			message.mPlayerIndex = atoi( index );
		}
		else
		{
			ASSERT( 0 );
			return;
		}
	}
	
	{	
		CTime date;
		mBeginDateCtrl.GetTime( date );

		CTime beginTime;
		CTime endTime;

		mBeginTimeCtrl.GetTime( beginTime );
		mEndTimeCtrl.GetTime( endTime );

		if( CTime() == beginTime )
		{
			beginTime = CTime( date.GetYear(), date.GetMonth(), date.GetDay(), 0, 0, 0 );
		}

		if( CTime() == endTime )
		{
			endTime = CTime( date.GetYear(), date.GetMonth(), date.GetDay(), 23, 59, 59 );
		}

		if( beginTime >= endTime )
		{
			CString textBeginDateAndEndDateIsSame;
			textBeginDateAndEndDateIsSame.LoadString( IDS_STRING237 );

			MessageBox( textBeginDateAndEndDateIsSame, "", MB_ICONWARNING | MB_OK );
			return;
		}
		
		sprintf( message.mBeginDate, "%02d%02d%02d %02d:%02d:%02d",
			max( 0, date.GetYear() - 2000 ),
			date.GetMonth(),
			date.GetDay(),
			beginTime.GetHour(),
			beginTime.GetMinute(),
			beginTime.GetSecond() );

		sprintf( message.mEndDate, "%02d%02d%02d %02d:%02d:%02d",
			max( 0, date.GetYear() - 2000 ),
			date.GetMonth(),
			date.GetDay(),
			endTime.GetHour(),
			endTime.GetMinute(),
			endTime.GetSecond() );
	}

	mLogMap.clear();

	CString serverName;
	mServerCombo.GetLBText( mServerCombo.GetCurSel(), serverName );

	mApplication.Send( serverName, message, sizeof( message ) );
}


void CItemLogDialog::OnBnClickedItemLogStopButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	MSGROOT message;
	message.Category	= MP_RM_ITEM_LOG;
	message.Protocol	= MP_RM_ITEM_LOG_GET_STOP_SYN;

	mApplication.Send( mServerIndex, message, sizeof( message ) );
}


void CItemLogDialog::OnBnClickedItemLogExcelButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 엑셀로 저장
	CString dbIndex;
	mKeywordEdit.GetWindowText( dbIndex );

	CString serverName;
	mServerCombo.GetLBText( mServerCombo.GetCurSel(), serverName );


	CString caption;
	GetWindowText( caption );

	CString textItemLog;
	textItemLog.LoadString( IDS_STRING117 );

	CString title;
	title.Format( "%s-%s", textItemLog, dbIndex );
	
	CString textLog;
	textLog.LoadString( IDS_STRING10 );

	CclientApp::SaveToExcel( title, textLog, mResultListCtrl );
}


BOOL CItemLogDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	SetIcon( AfxGetApp()->LoadIcon( IDR_MAINFRAME ), FALSE );
	SetIcon( AfxGetApp()->LoadIcon( IDR_MAINFRAME ), TRUE );

	// 리스트 컨트롤 초기화
	{
		const DWORD addStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
		CRect rect;

		// 아이템 목록을 채운다
		{
			int step = -1;

			mResultListCtrl.GetClientRect( rect );

			CString textDate;
			textDate.LoadString( IDS_STRING3 );
			mResultListCtrl.InsertColumn( ++step, textDate, LVCFMT_LEFT, int( rect.Width() * 0.2f ) );

			CString textType;
			textType.LoadString( IDS_STRING4 );
			mResultListCtrl.InsertColumn( ++step, textType, LVCFMT_LEFT, int( rect.Width() * 0.2f ) );

			CString textItemDbIndex;
			textItemDbIndex.LoadString( IDS_STRING13 );
			mResultListCtrl.InsertColumn( ++step, textItemDbIndex, LVCFMT_LEFT, int( rect.Width() * 0.1f ) );

			CString textItemIndex;
			textItemIndex.LoadString( IDS_STRING22 );
			mResultListCtrl.InsertColumn( ++step, textItemIndex, LVCFMT_LEFT, int( rect.Width() * 0.0f ) );

			CString textName;
			textName.LoadString( IDS_STRING20 );
			mResultListCtrl.InsertColumn( ++step, textName, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );

			CString textQuantity;
			textQuantity.LoadString( IDS_STRING42 );
			mResultListCtrl.InsertColumn( ++step, textQuantity, LVCFMT_LEFT, int( rect.Width() * 0.2f ) );

			CString textSendPosition;
			textSendPosition.LoadString( IDS_STRING56 );
			mResultListCtrl.InsertColumn( ++step, textSendPosition, LVCFMT_LEFT, int( rect.Width() * 0.2f ) );

			CString textReceivedPosition;
			textReceivedPosition.LoadString( IDS_STRING57 );
			mResultListCtrl.InsertColumn( ++step, textReceivedPosition, LVCFMT_LEFT, int( rect.Width() * 0.2f ) );

			CString textSentPlayer;
			textSentPlayer.LoadString( IDS_STRING118 );
			mResultListCtrl.InsertColumn( ++step, textSentPlayer, LVCFMT_LEFT, int( rect.Width() * 0.2f ) );

			CString textSentPlayerIndex;
			textSentPlayerIndex.LoadString( IDS_STRING119 );
			mResultListCtrl.InsertColumn( ++step, textSentPlayerIndex, LVCFMT_LEFT, int( rect.Width() * 0.2f ) );

			CString textSentPlayerMoney;
			textSentPlayerMoney.LoadString( IDS_STRING120 );
			mResultListCtrl.InsertColumn( ++step, textSentPlayerMoney, LVCFMT_LEFT, int( rect.Width() * 0.2f ) );

			CString textReceivedPlayer;
			textReceivedPlayer.LoadString( IDS_STRING121 );
			mResultListCtrl.InsertColumn( ++step, textReceivedPlayer, LVCFMT_LEFT, int( rect.Width() * 0.2f ) );

			CString textReceivedPlayerIndex;
			textReceivedPlayerIndex.LoadString( IDS_STRING122 );
			mResultListCtrl.InsertColumn( ++step, textReceivedPlayerIndex, LVCFMT_LEFT, int( rect.Width() * 0.2f ) );

			CString textReceivedPlayerMoney;
			textReceivedPlayerMoney.LoadString( IDS_STRING123 );
			mResultListCtrl.InsertColumn( ++step, textReceivedPlayerMoney, LVCFMT_LEFT, int( rect.Width() * 0.2f ) );

			CString textVariatedMoney;
			textVariatedMoney.LoadString( IDS_STRING124 );
			mResultListCtrl.InsertColumn( ++step, textVariatedMoney, LVCFMT_LEFT, int( rect.Width() * 0.2f ) );

			CString textExperience;
			textExperience.LoadString( IDS_STRING9 );
			mResultListCtrl.InsertColumn( ++step, textExperience, LVCFMT_LEFT, int( rect.Width() * 0.2f ) );

			mResultListCtrl.SetExtendedStyle( mResultListCtrl.GetExtendedStyle() | addStyle );
		}
	}

	// 스테틱 초기화
	{
		mResultStatic.SetWindowText( "" );
	}

	// 콤보 초기화
	{
		CRect rect;
		mServerCombo.GetWindowRect( rect );
		mServerCombo.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );

		mApplication.SetServerComboBox( mServerCombo );
	}

	// 버튼 초기화
	{
		mStopButton.EnableWindow( FALSE );
	}

	// 라디오 버튼 초기화
	{
		mItemIndexRadio.SetCheck( BST_UNCHECKED );
		mDbIndexRadio.SetCheck( BST_CHECKED );
		mPlayerIndexRadio.SetCheck( BST_UNCHECKED );
	}

	// initialize CDateTimeCtrl
	{
		mEndDateCtrl.ShowWindow( SW_HIDE );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CItemLogDialog::Parse( DWORD serverIndex, const MSGROOT* message )
{
	mServerIndex = serverIndex;

	switch( message->Protocol )
	{
	case MP_RM_ITEM_LOG_GET_ACK:
		{
			CString text;
			const MSG_RM_ITEM_LOG* m = ( MSG_RM_ITEM_LOG* )message;

			if( m->mRequestTick != mRequestTick )
			{
				return;
			}

			for( DWORD i = 0; i < m->mSize; ++i )
			{
				const MSG_RM_ITEM_LOG::Log& data	= m->mLog[ i ];
				const DWORD					row		= mResultListCtrl.GetItemCount();
				int							step	= 0;

				mLogMap.insert( std::make_pair( data.mLogIndex, data ) );
			}

			PutLog();
			
			{
				int minRange;
				int maxRange;
				mResultProgress.GetRange( minRange, maxRange );

				mResultProgress.SetPos( mResultProgress.GetPos() + int( m->mSize ) );

				text.Format( "%d/%d", mResultProgress.GetPos(), maxRange );
				mResultStatic.SetWindowText( text );

				if( mResultListCtrl.GetItemCount() == maxRange )
				{
					mFindButton.EnableWindow( TRUE );
					mStopButton.EnableWindow( FALSE );
				}
			}

			if( m->mSize )
			{
				mItemIndex = m->mLog[ 0 ].mItem.mIndex;

				CString textFormat;
				textFormat.LoadString( IDS_STRING125 );

				CString text;
				text.Format( textFormat, mApplication.GetItemName( mItemIndex ) );

				SetWindowText( text );
			}
			else
			{
				CString textItIsNotExistedItem;
				textItIsNotExistedItem.LoadString(IDS_STRING63 );

				SetWindowText( textItIsNotExistedItem );
			}

			break;
		}
	case MP_RM_ITEM_LOG_GET_SIZE_ACK:
		{
			const MSG_DWORD* m = ( MSG_DWORD* )message;

			const DWORD size = m->dwData;

			CString text;
			text.Format( "0/%d", size );
			mResultStatic.SetWindowText( text );

			mResultProgress.SetRange32( 0, size );
			mResultProgress.SetPos( 0 );

			mResultListCtrl.DeleteAllItems();
			break;
		}
	case MP_RM_ITEM_LOG_GET_SIZE_NACK:
		{			
			CString textThereIsNoResult;
			textThereIsNoResult.LoadString( IDS_STRING1 );

			MessageBox( textThereIsNoResult, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_ITEM_LOG_GET_STOP_ACK:
		{
			mFindButton.EnableWindow( TRUE );
			mStopButton.EnableWindow( FALSE );

			CString textSearchWasStopped;
			textSearchWasStopped.LoadString( IDS_STRING2 );

			MessageBox( textSearchWasStopped, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_ITEM_LOG_NACK_BY_AUTH:
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


void CItemLogDialog::OnBnClickedItemLogOptionLogButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	if( ! mDbIndexRadio.GetCheck() )
	{
		CString textYouShouldCheckDbIndexRadio;
		textYouShouldCheckDbIndexRadio.LoadString( IDS_STRING179 );

		MessageBox( textYouShouldCheckDbIndexRadio, "", MB_ICONERROR | MB_OK );
	}
	else if( ! mResultListCtrl.GetItemCount() )
	{
		CString textThereIsNoResult;
		textThereIsNoResult.LoadString( IDS_STRING1 );

		MessageBox( textThereIsNoResult, "", MB_ICONERROR | MB_OK );
		return;
	}


	CString itemDbIndex;
	mKeywordEdit.GetWindowText( itemDbIndex );

	if( itemDbIndex.IsEmpty() )
	{
		CString textKeywordIsEmpty;
		textKeywordIsEmpty.LoadString( IDS_STRING32 );

		MessageBox( textKeywordIsEmpty, "", MB_ICONERROR | MB_OK );
		return;
	}

	const CString itemName	= ( mResultListCtrl.GetItemCount() ? mResultListCtrl.GetItemText( 0, 9 ) : "" );
	
	{
		CItemOptionLogDialog* dialog = mApplication.GetItemOptionLogDialog();
		ASSERT( dialog );

		CString serverName;
		mServerCombo.GetLBText( mServerCombo.GetCurSel(), serverName );

		dialog->Request( serverName, itemName, atoi( itemDbIndex ) );
	}
}


void CItemLogDialog::Request( DWORD serverIndex, const CString& itemName, DWORD itemDbIndex )
{
	CString text;
	text.Format( "%d", itemDbIndex );

	mKeywordEdit.SetWindowText( text );

	CString textTileFormat;
	textTileFormat.LoadString( IDS_STRING125 );

	text.Format( textTileFormat, itemName );
	SetWindowText( text );

	mServerCombo.SetCurSel(
		mServerCombo.FindString(
			0,
			mApplication.GetServerName( serverIndex ) ) );

	ShowWindow( SW_SHOW );
}


void CItemLogDialog::OnCancel()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	mResultListCtrl.DeleteAllItems();
	
	CDialog::OnCancel();
}


void CItemLogDialog::SetItemDbIndex( DWORD serverIndex, DWORD itemDbIndex )
{
	{
		mServerIndex = serverIndex;

		mServerCombo.SetCurSel( mServerCombo.FindString( -1, mApplication.GetServerName( serverIndex ) ) );
	}

	{
		CString text;
		text.Format( "%d", itemDbIndex );

		mKeywordEdit.SetWindowText( text );
	}	

	{
		mItemIndexRadio.SetCheck( BST_UNCHECKED );
		mDbIndexRadio.SetCheck( BST_CHECKED );
		mPlayerIndexRadio.SetCheck( BST_UNCHECKED );
	}	
}


void CItemLogDialog::SetPlayerIndex( DWORD serverIndex, DWORD playerIndex )
{
	{
		mServerIndex = serverIndex;

		mServerCombo.SetCurSel( mServerCombo.FindString( -1, mApplication.GetServerName( serverIndex ) ) );
	}

	{
		CString text;
		text.Format( "%d", playerIndex );

		mKeywordEdit.SetWindowText( text );
	}	

	{
		mItemIndexRadio.SetCheck( BST_UNCHECKED );
		mDbIndexRadio.SetCheck( BST_UNCHECKED );
		mPlayerIndexRadio.SetCheck( BST_CHECKED );
	}
}


void CItemLogDialog::OnBnClickedItemLogDbIndexRadio()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 아이템 번호를 입력받을 모달 창을 표시한다

	CItemAddDialog dialog( mApplication );
	
	if( IDCANCEL == dialog.DoModal() )
	{
		return;
	}

	const DWORD itemIndex = dialog.GetSelectedItemIndex();

	if( ! itemIndex )
	{
		return;
	}

	CString text;
	text.Format( "%d", itemIndex );
	mKeywordEdit.SetWindowText( text );
}


void CItemLogDialog::OnBnClickedItemLogCopyButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	mApplication.SaveToClipBoard( mResultListCtrl );
}


void CItemLogDialog::PutLog()
{
	mResultListCtrl.DeleteAllItems();

	CString text;

	for(
		LogMap::const_iterator it = mLogMap.begin();
		mLogMap.end() != it;
		++it )
	{
		const MSG_RM_ITEM_LOG::Log& log = it->second;

		const int	row		= mResultListCtrl.GetItemCount();
		int			step	= -1;

		mResultListCtrl.InsertItem( row, log.mDate, ++step );
		mResultListCtrl.SetItemText( row, ++step, mApplication.GetLogText( log.mType ) );

		{
			const MSG_RM_ITEM_LOG::Item& item = log.mItem;

			text.Format( "%d", item.mDbIndex );
			mResultListCtrl.SetItemText( row, ++step, text );

			text.Format( "%d", item.mIndex );
			mResultListCtrl.SetItemText( row, ++step, text );

			mResultListCtrl.SetItemText( row, ++step, mApplication.GetItemName( item.mIndex ) );

			text.Format( "%d", item.mQuantity );
			mResultListCtrl.SetItemText( row, ++step, text );

			text.Format( "%d", item.mFromPosition );
			mResultListCtrl.SetItemText( row, ++step, text );

			text.Format( "%d", item.mToPosition );
			mResultListCtrl.SetItemText( row, ++step, text );
		}

		{
			const MSG_RM_ITEM_LOG::Player& player = log.mFromPlayer;

			mResultListCtrl.SetItemText( row, ++step, player.mName );

			text.Format( "%d", player.mIndex );
			mResultListCtrl.SetItemText( row, ++step, text );

			text.Format( "%u", player.mMoney );
			mResultListCtrl.SetItemText( row, ++step, text );
		}

		{
			const MSG_RM_ITEM_LOG::Player& player = log.mToPlayer;

			mResultListCtrl.SetItemText( row, ++step, player.mName );

			text.Format( "%d", player.mIndex );
			mResultListCtrl.SetItemText( row, ++step, text );

			text.Format( "%u", player.mMoney );
			mResultListCtrl.SetItemText( row, ++step, text );
		}

		text.Format( "%u", log.mMoneyVariation );
		mResultListCtrl.SetItemText( row, ++step, text );

		text.Format( "%d", log.mExperience );
		mResultListCtrl.SetItemText( row, ++step, text );
	}
}
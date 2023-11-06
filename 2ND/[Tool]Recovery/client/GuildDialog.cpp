// GuildDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "client.h"
#include "GuildDialog.h"
#include "PlayerDialog.h"
#include "GuildLogDialog.h"
#include "GuildWarehouseLogDialog.h"
#include "GuildScoreLogDialog.h"
#include ".\guilddialog.h"

// CGuildDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGuildDialog, CDialog)
CGuildDialog::CGuildDialog( CclientApp& application, CWnd* pParent /*=NULL*/)
	: CDialog(CGuildDialog::IDD, pParent)
	, mApplication( application )
	, mServerIndex( 0 )
{
	EnableAutomation();

	ZeroMemory( &mDataMessage, sizeof( mDataMessage ) );
}


CGuildDialog::~CGuildDialog()
{
}

void CGuildDialog::OnFinalRelease()
{
	// 자동화 개체에 대한 마지막 참조가 해제되면 OnFinalRelease가
	// 호출됩니다. 기본 클래스에서 자동으로 개체를 삭제합니다.
	// 기본 클래스를 호출하기 전에 개체에 필요한 추가 정리 작업을
	// 추가하십시오.

	CDialog::OnFinalRelease();
}

void CGuildDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//DDX_Control(pDX, IDC_GUILD_SERVER_COMBO, mServerCombo);
	DDX_Control(pDX, IDC_GUILD_KEYWORD_EDIT, mKeywordEdit);
	DDX_Control(pDX, IDC_GUILD_FIND_BUTTON, mFindButton);
	DDX_Control(pDX, IDC_GUILD_RESULT_COMBO, mResultCombo);
	DDX_Control(pDX, IDC_GUILD_DELETE_GUILD_CHECK, mDeletedGuildCheck);
	DDX_Control(pDX, IDC_GUILD_SPECIFIC_DATA_BUTTON, mSpecificDataButton);
	DDX_Control(pDX, IDC_GUILD_NAME_EDIT, mNameEdit);
	DDX_Control(pDX, IDC_GUILD_BUILD_DATE_EDIT, mBuildDateEdit);
	DDX_Control(pDX, IDC_GUILD_LEVEL_EDIT, mLevelEdit);
	DDX_Control(pDX, IDC_GUILD_SCORE_EDIT, mScoreEdit);
	DDX_Control(pDX, IDC_GUILD_ALLIANCE_NAME_EDIT, mAllianceEdit);
	DDX_Control(pDX, IDC_GUILD_DATA_UPDATE_BUTTON, mDataUpdateButton);
	DDX_Control(pDX, IDC_GUILD_STORE_LIST, mStoreItemList);
	DDX_Control(pDX, IDC_GUILD_MONEY_EDIT, mMoneyEdit);
	DDX_Control(pDX, IDC_GUILD_MONEY_UPDATE_BUTTON, mMoneyUpdateButton);
	DDX_Control(pDX, IDC_GUILD_STORE_LOG_BUTTON, mStoreLogButton);
	DDX_Control(pDX, IDC_GUILD_MEMBER_LIST, mMemberList);
	DDX_Control(pDX, IDC_GUILD_SELECTED_MEMBER_BUTTON, mViewMemberButton);
	DDX_Control(pDX, IDC_GUILD_ALLIANCE_LIST, mFriendlyList);
	DDX_Control(pDX, IDC_GUILD_FRIENDLY_DATA_BUTTON, mFriendlyDataButton);
	DDX_Control(pDX, IDC_GUILD_ENEMY_LIST, mEnemyList);
	DDX_Control(pDX, IDC_GUILD_ENEMY_DATA_BUTTON, mEnemeyDataButton);
	DDX_Control(pDX, IDC_GUILD_ALLIANCE_ENTRY_DATE_EDIT, mAllianceEntryDateEdit);
	DDX_Control(pDX, IDC_GUILD_RESULT_SIZE_STATIC, mResultSizeStatic);
	DDX_Control(pDX, IDC_GUILD_SERVER_COMBO2, mServerCombo);
	DDX_Control(pDX, IDC_GUILD_RANK_COMBO, mRankCombo);
}


BEGIN_MESSAGE_MAP(CGuildDialog, CDialog)
	//ON_EN_CHANGE(IDC_GUILD_UNION_NAME_EDIT2, OnEnChangeGuildUnionNameEdit2)
	ON_BN_CLICKED(IDC_GUILD_FIND_BUTTON, OnBnClickedGuildFindButton)
	ON_BN_CLICKED(IDC_GUILD_SPECIFIC_DATA_BUTTON, OnBnClickedGuildSpecificDataButton)
	ON_BN_CLICKED(IDC_GUILD_DATA_UPDATE_BUTTON, OnBnClickedGuildDataUpdateButton)
	ON_BN_CLICKED(IDC_GUILD_MONEY_UPDATE_BUTTON, OnBnClickedGuildMoneyUpdateButton)
	ON_BN_CLICKED(IDC_GUILD_SELECTED_MEMBER_BUTTON, OnBnClickedGuildSelectedMemberButton)
	ON_BN_CLICKED(IDC_GUILD_FRIENDLY_DATA_BUTTON, OnBnClickedGuildFriendlyDataButton)
	ON_BN_CLICKED(IDC_GUILD_ENEMY_DATA_BUTTON, OnBnClickedGuildEnemyDataButton)
	ON_BN_CLICKED(IDC_GUILD_LOG_BUTTON, OnBnClickedGuildLogButton)
	ON_BN_CLICKED(IDC_GUILD_STORE_LOG_BUTTON, OnBnClickedGuildStoreLogButton)
	ON_BN_CLICKED(IDC_GUILD_SCORE_LOG_BUTTON, OnBnClickedGuildScoreLogButton)
	ON_BN_CLICKED(IDC_GUILD_MEMBER_MODIFY_BUTTON, OnBnClickedGuildMemberModifyButton)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_GUILD_MEMBER_LIST, OnLvnItemchangedGuildMemberList)
	ON_CBN_SELCHANGE(IDC_GUILD_RANK_COMBO, OnCbnSelchangeGuildRankCombo)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_GUILD_MEMBER_LIST, OnLvnColumnclickGuildMemberList)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CGuildDialog, CDialog)
END_DISPATCH_MAP()

// 참고: IID_IGuildDialog에 대한 지원을 추가하여
// VBA에서 형식 안정적인 바인딩을 지원합니다. 
// 이 IID는 .IDL 파일에 있는 dispinterface의 GUID와 일치해야 합니다.

// {BC3B6879-2097-4B93-8867-49649BB53B46}
static const IID IID_IGuildDialog =
{ 0xBC3B6879, 0x2097, 0x4B93, { 0x88, 0x67, 0x49, 0x64, 0x9B, 0xB5, 0x3B, 0x46 } };

BEGIN_INTERFACE_MAP(CGuildDialog, CDialog)
	INTERFACE_PART(CGuildDialog, IID_IGuildDialog, Dispatch)
END_INTERFACE_MAP()


// CGuildDialog 메시지 처리기입니다.


void CGuildDialog::Parse( DWORD serverIndex, const MSGROOT* message )
{
	mServerIndex = serverIndex;

	switch( message->Protocol )
	{
	case MP_RM_GUILD_GET_LIST_ACK:
		{
			const MSG_RM_GUILD_LIST* m = ( MSG_RM_GUILD_LIST* )message;
			
			CString text;

			mResultCombo.ResetContent();

			for( DWORD i = 0; i < m->mSize; ++i )
			{
				const MSG_RM_GUILD_LIST::Guild& data = m->mGuild[ i ];

				CString textTitleFormat;
				textTitleFormat.LoadString( IDS_STRING35 );
				text.Format( textTitleFormat, data.mName, data.mSize, data.mLevel );

				mResultCombo.AddString( text );
			}

			mResultCombo.EnableWindow( 0 < m->mSize );

			if( m->mSize )
			{
				text.Format( "%d", m->mSize );
				mResultSizeStatic.SetWindowText( text );

				mResultCombo.SetCurSel( 0 );
			}
			else
			{
				mResultSizeStatic.SetWindowText( "" );

				CString textThereIsNoResult;
				textThereIsNoResult.LoadString( IDS_STRING1 );
				
				MessageBox( textThereIsNoResult, "", MB_ICONINFORMATION | MB_OK );
			}

			break;
		}
	case MP_RM_GUILD_GET_LIST_NACK:
		{
			break;
		}
	case MP_RM_GUILD_GET_DATA_ACK:
		{
			mServerCombo.SetCurSel( mServerIndex );

			//const MSG_RM_GUILD_DATA* m = ( MSG_RM_GUILD_DATA* )message;
			memcpy( &mDataMessage, message, sizeof( mDataMessage ) );

			mGuildIndex = mDataMessage.mGuildIndex;

			CString text;

			mNameEdit.SetWindowText( mDataMessage.mGuildName );

			mBuildDateEdit.SetWindowText( mDataMessage.mBuiltDate );

			text.Format( "%d", mDataMessage.mLevel );
			mLevelEdit.SetWindowText( text );

			text.Format( "%d", mDataMessage.mScore );
			mScoreEdit.SetWindowText( text );

			mAllianceEdit.SetWindowText( mDataMessage.mAllianceName );

			text.Format( "%u", mDataMessage.mMoney );
			mMoneyEdit.SetWindowText( text );

			if( mDataMessage.mAllianceEntryDate )
			{
				stTIME time;
				time.SetTime( mDataMessage.mAllianceEntryDate );

				text.Format( "%d.%d.%d %d:%d:%d",
					2000 + time.GetYear(),
					time.GetMonth(),
					time.GetDay(),
					time.GetHour(),
					time.GetMinute(),
					time.GetSecond() );
				mAllianceEntryDateEdit.SetWindowText( text );
			}

			break;
		}
	case MP_RM_GUILD_GET_DATA_NACK:
		{
			break;
		}
	case MP_RM_GUILD_GET_STORE_ACK:
		{
			const MSG_RM_GUILD_STORE* m = ( MSG_RM_GUILD_STORE* )message;

			CString text;

			mStoreItemList.DeleteAllItems();

			for( DWORD i = 0; i < m->mSize; ++i )
			{
				const MSG_RM_GUILD_STORE::Item& data = m->mItem[ i ];

				text.Format( "%d", data.mDbIndex );
				mStoreItemList.InsertItem( i, text, 0 );

				mStoreItemList.SetItemText( i, 1, mApplication.GetItemName( data.mIndex ) );

				if( mApplication.IsDuplicateItem( data.mIndex ) )
				{
					text.Format( "%d", data.mQuantity );
					mStoreItemList.SetItemText( i, 2, text );
				}
			}

			break;
		}
	case MP_RM_GUILD_GET_MEMBER_ACK:
		{
			const MSG_RM_GUILD_MEMBER* m = ( MSG_RM_GUILD_MEMBER* )message;

			CString text;

			mMemberList.DeleteAllItems();

			for( DWORD i = 0; i < m->mSize; ++i )
			{
				const MSG_RM_GUILD_MEMBER::Player& data = m->mPlayer[ i ];

				text.Format( "%d", data.mIndex );
				mMemberList.InsertItem( i, text, 0 );

				mMemberList.SetItemText( i, 1, mApplication.GetRankName( data.mRank ) );
				mMemberList.SetItemText( i, 2, data.mName );

				text.Format( "%d", data.mLevel );
				mMemberList.SetItemText( i, 3, text );

				mMemberList.SetItemText( i, 4, data.mDate );
			}

			mRankCombo.SetCurSel( 0 );

			break;
		}
	case MP_RM_GUILD_GET_FRIENDLY_ACK:
		{
			const MSG_RM_GUILD_LIST* m = ( MSG_RM_GUILD_LIST* )message;

			mFriendlyList.DeleteAllItems();

			CString text;

			for( DWORD i = 0; i < m->mSize; ++i )
			{
				const MSG_RM_GUILD_LIST::Guild& data = m->mGuild[ i ];

				text.Format( "%d", data.mIndex );
				mFriendlyList.InsertItem( i, text, 0 );

				mFriendlyList.SetItemText( i, 1, data.mName );

				text.Format( "%d", data.mSize );
				mFriendlyList.SetItemText( i, 2, text );

				text.Format( "%d", data.mLevel );
				mFriendlyList.SetItemText( i, 3, text );
			}

			mFriendlyDataButton.EnableWindow( 0 < m->mSize );
			break;
		}
	case MP_RM_GUILD_GET_ENEMY_ACK:
		{
			const MSG_RM_GUILD_LIST* m = ( MSG_RM_GUILD_LIST* )message;

			mEnemyList.DeleteAllItems();

			CString text;

			for( DWORD i = 0; i < m->mSize; ++i )
			{
				const MSG_RM_GUILD_LIST::Guild& data = m->mGuild[ i ];

				text.Format( "%d", data.mIndex );
				mEnemyList.InsertItem( i, text, 0 );

				mEnemyList.SetItemText( i, 1, data.mName );

				text.Format( "%d", data.mSize );
				mEnemyList.SetItemText( i, 2, text );

				text.Format( "%d", data.mLevel );
				mEnemyList.SetItemText( i, 3, text );
			}

			mEnemeyDataButton.EnableWindow( 0 < m->mSize );
			break;
		}
	case MP_RM_GUILD_SET_DATA_ACK:
		{
			CString textUpdateSucceed;
			textUpdateSucceed.LoadString( IDS_STRING28 );

			MessageBox( textUpdateSucceed, "", MB_ICONINFORMATION | MB_OK );
			break;
		}
	case MP_RM_GUILD_SET_DATA_NACK:
		{
			CString textUpdateFailed;
			textUpdateFailed.LoadString( IDS_STRING29 );

			MessageBox( textUpdateFailed, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_GUILD_SET_RANK_ACK:
		{
			const MSG_DWORD*	m			= ( MSG_DWORD* )message;
			const CString		rankName	= mApplication.GetRankName( m->dwData );
			const DWORD			playerIndex = m->dwObjectID;

			for( int row = 0; row < mMemberList.GetItemCount(); ++row )
			{
				if( atoi( mMemberList.GetItemText( row, 0 ) ) != playerIndex )
				{
					continue;
				}

				mMemberList.SetItemText( row, 1, rankName );

				mRankCombo.SetCurSel( mRankCombo.FindString( 0, rankName ) );

				CString textFormat;
				textFormat.LoadString( IDS_STRING36 );

				CString text;
				text.Format(
					textFormat,
					mMemberList.GetItemText( row, 2 ),
					rankName );

				MessageBox( text, "", MB_ICONINFORMATION | MB_OK );

				break;
			}
			
			break;
		}
	case MP_RM_GUILD_SET_RANK_NACK:
		{
			mRankCombo.SetCurSel( 0 );

			CString textRankUpdateIsFailed;
			textRankUpdateIsFailed.LoadString( IDS_STRING37 );

			MessageBox( textRankUpdateIsFailed, "", MB_ICONINFORMATION | MB_OK );
			break;
		}
	case MP_RM_GUILD_KICK_ACK:
		{
			const MSGBASE*	m			= ( MSGBASE* )message;
			const DWORD		playerindex = m->dwObjectID;

			for( int row = 0; row < mMemberList.GetItemCount(); ++row )
			{
				if( playerindex == atoi( mMemberList.GetItemText( row, 0 ) ) )
				{
					CString textFormat;
					textFormat.LoadString( IDS_STRING38 );

					CString text;
					text.Format( textFormat,
						mMemberList.GetItemText( row, 2 ) );

					MessageBox( text, "", MB_ICONINFORMATION | MB_OK );

					mMemberList.DeleteItem( row );
					break;
				}
			}

			break;
		}
	case MP_RM_GUILD_KICK_NACK:
		{
			CString textKickingIsFailedBySystemFault;
			textKickingIsFailedBySystemFault.LoadString( IDS_STRING39 );

			MessageBox( textKickingIsFailedBySystemFault, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_GUILD_KICK_NACK_BY_NO_GUILD:
		{
			CString textKickingIsFailedByNoMember;
			textKickingIsFailedByNoMember.LoadString( IDS_STRING40 );

			MessageBox( textKickingIsFailedByNoMember, "", MB_ICONERROR | MB_OK );
			
			break;
		}
	case MP_RM_GUILD_KICK_NACK_BY_ONE_MEMBER:
		{
			const MSGBASE*	m			= ( MSGBASE* )message;
			const DWORD		playerIndex = m->dwObjectID;

			for( int row = 0; row < mMemberList.GetItemCount(); ++row )
			{
				if( playerIndex == atoi( mMemberList.GetItemText( row, 0 ) ) )
				{
					const CString rank = mMemberList.GetItemText( row, 2 );

					mRankCombo.SetCurSel( mRankCombo.FindString( 0, rank ) );
					break;
				}
			}

			CString textKickingIsFailedByMemberIsOne;
			textKickingIsFailedByMemberIsOne.LoadString( IDS_STRING41 );
			MessageBox( textKickingIsFailedByMemberIsOne, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_GUILD_NACK_BY_AUTH:
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


BOOL CGuildDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	SetIcon( AfxGetApp()->LoadIcon( IDR_MAINFRAME ), FALSE );
	SetIcon( AfxGetApp()->LoadIcon( IDR_MAINFRAME ), TRUE );

	// 리스트 컨트롤 초기화
	{
		const DWORD addStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
		CRect		rect;

		// 창고 아이템 리스트
		{
			int step = -1;

			mStoreItemList.GetClientRect( rect );

			CString textDbIndex;
			textDbIndex.LoadString( IDS_STRING13 );
			mStoreItemList.InsertColumn( ++step, textDbIndex, LVCFMT_LEFT, int( rect.Width() * 0.3f ) );

			CString textName;
			textName.LoadString( IDS_STRING20 );
			mStoreItemList.InsertColumn( ++step, textName, LVCFMT_LEFT, int( rect.Width() * 0.5f ) );

			CString textQuantity;
			textQuantity.LoadString( IDS_STRING42 );
			mStoreItemList.InsertColumn( ++step, textQuantity, LVCFMT_LEFT, int( rect.Width() * 0.18f ) );
			
			mStoreItemList.SetExtendedStyle( mStoreItemList.GetExtendedStyle() | addStyle );
		}

		// 길드 회원 리스트
		{
			int step = -1;

			mMemberList.GetClientRect( rect );

			CString textDbIndex;
			textDbIndex.LoadString( IDS_STRING13 );
			mMemberList.InsertColumn( ++step, textDbIndex, LVCFMT_LEFT, 0 );

			CString textRank;
			textRank.LoadString( IDS_STRING43 );
			mMemberList.InsertColumn( ++step, textRank,	LVCFMT_LEFT, int( rect.Width() * 0.28f ) );

			CString textName;
			textName.LoadString( IDS_STRING20 );
			mMemberList.InsertColumn( ++step, textName,	LVCFMT_LEFT, int( rect.Width() * 0.5f ) );

			CString textLevel;
			textLevel.LoadString( IDS_STRING5 );
			mMemberList.InsertColumn( ++step, textLevel,	LVCFMT_LEFT, int( rect.Width() * 0.2f ) );

			CString textJoinedDate;
			textJoinedDate.LoadString( IDS_STRING44 );
			mMemberList.InsertColumn( ++step, textJoinedDate, LVCFMT_LEFT, int( rect.Width() * 0.5f ) );
			
			mMemberList.SetExtendedStyle( mMemberList.GetExtendedStyle() | addStyle );
		}

		// 동맹 길드 리스트
		{
			int step = -1;

			mFriendlyList.GetClientRect( rect );

			CString textDbIndex;
			textDbIndex.LoadString( IDS_STRING13 );
			mFriendlyList.InsertColumn( ++step, textDbIndex, LVCFMT_LEFT, 0 );

			CString textName;
			textName.LoadString( IDS_STRING20 );
			mFriendlyList.InsertColumn( ++step, textName, LVCFMT_LEFT, int( rect.Width() * 0.7f ) );

			CString textMember;
			textMember.LoadString( IDS_STRING45 );
			mFriendlyList.InsertColumn( ++step, textMember, LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

			mFriendlyList.SetExtendedStyle( mFriendlyList.GetExtendedStyle() | addStyle );
		}

		// 적 길드 리스트
		{
			int step = -1;

			mEnemyList.GetClientRect( rect );

			CString textDbIndex;
			textDbIndex.LoadString( IDS_STRING13 );
			mEnemyList.InsertColumn( ++step, textDbIndex, LVCFMT_LEFT, 0 );

			CString textName;
			textName.LoadString( IDS_STRING20 );
			mEnemyList.InsertColumn( ++step, textName, LVCFMT_LEFT, int( rect.Width() * 0.7f ) );

			CString textMember;
			textMember.LoadString( IDS_STRING45 );
			mEnemyList.InsertColumn( ++step, textMember, LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

			mEnemyList.SetExtendedStyle( mEnemyList.GetExtendedStyle() | addStyle );
		}
	}

	// 에디트 컨트롤 초기화
	{
		mKeywordEdit.SetLimitText( MAX_NAME_LENGTH );
	}

	// 콤보 컨트롤 초기화
	{
		CRect rect;

		{			
			mResultCombo.GetWindowRect( rect );
			mResultCombo.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );
		}
		
		{
			mServerCombo.GetWindowRect( rect );
			mServerCombo.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );
			mApplication.SetServerComboBox( mServerCombo );
		}

		{
			mRankCombo.GetWindowRect( rect );
			mRankCombo.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );
			mApplication.SetRankCombo( mRankCombo );
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


CString CGuildDialog::GetServerName() const
{
	CString name;

	mServerCombo.GetLBText( mServerCombo.GetCurSel(), name );

	return name;
}

//void CGuildDialog::OnEnChangeGuildUnionNameEdit2()
//{
//	// TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
//	// CDialog::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
//	// CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
//	// 이 알림을 보내지 않습니다.
//
//	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
//}

void CGuildDialog::OnBnClickedGuildFindButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 키워드로 길드명 검색

	CString keyword;

	mKeywordEdit.GetWindowText( keyword );

    if( keyword.IsEmpty() )
	{
		CString textKeywordIsEmpty;
		textKeywordIsEmpty.LoadString( IDS_STRING32 );
		MessageBox( textKeywordIsEmpty, "", MB_ICONERROR | MB_OK );
		return;
	}

	MSG_NAME_DWORD message;
	{
		ZeroMemory( &message, sizeof( message ) );

		message.Category	= MP_RM_GUILD;
		message.Protocol	= MP_RM_GUILD_GET_LIST_SYN;
		strncpy( message.Name, keyword, sizeof( message.Name ) );
		message.dwData		= mDeletedGuildCheck.GetCheck();
	}

	mApplication.ConvertWildCardForDb( message.Name );	
	mApplication.Send( GetServerName(), message, sizeof( message ) );
	
	//mApplication.SetWait( TRUE );
}


void CGuildDialog::OnBnClickedGuildSpecificDataButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 선택된 길드의 상세 정보를 살펴봄

	CString text;

	mResultCombo.GetLBText( mResultCombo.GetCurSel(), text );

	int i = 0;
	const CString guildName( text.Tokenize( " ", i ) );

	if( guildName.IsEmpty() )
	{
		CString textThereIsNoTarget;
		textThereIsNoTarget.LoadString( IDS_STRING31 );
		MessageBox( textThereIsNoTarget, "", MB_ICONERROR | MB_OK );
		return;
	}

	MSG_NAME_DWORD message;
	{
		ZeroMemory( &message, sizeof( message ) );

		message.Category	= MP_RM_GUILD;
		message.Protocol	= MP_RM_GUILD_GET_DATA_SYN;
		strncpy( message.Name, guildName, sizeof( message.Name ) );
	}

	mApplication.Send( mServerIndex, message, sizeof( message ) );
}


void CGuildDialog::OnBnClickedGuildDataUpdateButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 서버의 길드 정보를 변경함
	// 길드 이름이나 연합 이름은 모든 맵 서버를 끄지 않으면 바뀌지 않는다...

	CString score;
	CString	level;
	CString	money;
	CString	guildName;
	CString	allianceName;
	
	mScoreEdit.GetWindowText( score );
	mLevelEdit.GetWindowText( level );
	mMoneyEdit.GetWindowText( money );
	mNameEdit.GetWindowText( guildName );
	mAllianceEdit.GetWindowText( allianceName );

	MSG_RM_GUILD_DATA message( mDataMessage );
	{
		message.Category	= MP_RM_GUILD;
		message.mLevel		= atoi( level );
		message.mScore		= atoi( score );
		message.mMoney		= atoi( money );
		strncpy( message.mGuildName, guildName, sizeof( message.mGuildName ) );
		strncpy( message.mAllianceName, allianceName, sizeof( message.mAllianceName ) );
	}

	if( ! memcmp( &message, &mDataMessage, sizeof( mDataMessage ) ) )
	{
		return;
	}
	
	CString textWarning;
	textWarning.LoadString( IDS_STRING46 );

	if( IDNO == MessageBox( textWarning, "", MB_ICONWARNING | MB_YESNO ) )
	{
		return;
	}
	
	message.Protocol = MP_RM_GUILD_SET_DATA_SYN;

	mApplication.Send( mServerIndex, message, sizeof( message ) );
}


void CGuildDialog::OnBnClickedGuildMoneyUpdateButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 길드의 돈을 변경함

	OnBnClickedGuildDataUpdateButton();
}


void CGuildDialog::OnBnClickedGuildSelectedMemberButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 선택된 멤버의 정보를 살펴봄

	POSITION		position		= mMemberList.GetFirstSelectedItemPosition();
	const DWORD		selectedRow		= mMemberList.GetNextSelectedItem( position );
	const CString	playerIndex		= mMemberList.GetItemText( selectedRow, 0 );

	// 플레이어 창의 제목 변경
	{
		const CString playerName = mMemberList.GetItemText( selectedRow, 2 );

		if( playerName.IsEmpty() )
		{
			CString textThereIsNoTarget;
			textThereIsNoTarget.LoadString( IDS_STRING31 );

			MessageBox( textThereIsNoTarget, "", MB_ICONERROR | MB_OK );
			return;
		}

		CPlayerDialog* dialog = mApplication.GetPlayerDialog();

		if( dialog )
		{
			dialog->Request( mServerIndex, playerName, atoi( playerIndex ) );
		}
	}
}


void CGuildDialog::OnBnClickedGuildFriendlyDataButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 선택된 아군 길드의 정보를 봄

	POSITION position = mFriendlyList.GetFirstSelectedItemPosition();

	const CString guildIndex = mFriendlyList.GetItemText( mFriendlyList.GetNextSelectedItem( position ), 0 );

	if( guildIndex.IsEmpty() )
	{
		CString textThereIsNoTarget;
		textThereIsNoTarget.LoadString( IDS_STRING31 );

		MessageBox( textThereIsNoTarget, "", MB_ICONERROR | MB_OK );
		return;
	}

	MSG_NAME_DWORD message;
	{
		ZeroMemory( &message, sizeof( message ) );

		message.Category	= MP_RM_GUILD;
		message.Protocol	= MP_RM_GUILD_GET_DATA_SYN;
		message.dwData		= atoi( guildIndex );
	}

	mApplication.Send( mServerIndex, message, sizeof( message ) );
}


void CGuildDialog::OnBnClickedGuildEnemyDataButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 선택된 적군 길드의 정보를 봄

	POSITION position = mEnemyList.GetFirstSelectedItemPosition();

	const CString guildIndex = mEnemyList.GetItemText( mEnemyList.GetNextSelectedItem( position ), 0 );

	if( guildIndex.IsEmpty() )
	{
		CString textThereIsNoTarget;
		textThereIsNoTarget.LoadString( IDS_STRING31 );

		MessageBox( textThereIsNoTarget, "", MB_ICONERROR | MB_OK );
		return;
	}

	MSG_NAME_DWORD message;
	{
		ZeroMemory( &message, sizeof( message ) );

		message.Category	= MP_RM_GUILD;
		message.Protocol	= MP_RM_GUILD_GET_DATA_SYN;
		message.dwData		= atoi( guildIndex );
	}

	mApplication.Send( mServerIndex, message, sizeof( message ) );
}


void CGuildDialog::Request( const CString& serverName, const CString& playerName, DWORD playerIndex )
{
	
}


void CGuildDialog::OnBnClickedGuildLogButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString name;
	mNameEdit.GetWindowText( name );

	if( name.IsEmpty() )
	{
		CString textThereIsNoTarget;
		textThereIsNoTarget.LoadString( IDS_STRING31 );

		MessageBox( textThereIsNoTarget, "", MB_ICONERROR | MB_OK );
		return;
	}

	CGuildLogDialog* dialog = mApplication.GetGuildLogDialog();
	ASSERT( dialog );

	dialog->DoModal( mServerIndex, name, mGuildIndex );
	//dialog->Request( mApplication.GetServerName( mServerIndex ), name, mGuildIndex );
}


void CGuildDialog::OnBnClickedGuildStoreLogButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString name;
	mNameEdit.GetWindowText( name );

	if( name.IsEmpty() )
	{
		CString textThereIsNoTarget;
		textThereIsNoTarget.LoadString( IDS_STRING31 );

		MessageBox( textThereIsNoTarget, "", MB_ICONERROR | MB_OK );
		return;
	}

	CGuildWarehouseLogDialog* dialog = mApplication.GetGuildItemLogDialog();
	ASSERT( dialog );

	dialog->DoModal( mServerIndex, name, mGuildIndex );
}


void CGuildDialog::OnBnClickedGuildScoreLogButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CGuildScoreLogDialog* dialog = mApplication.GetGuildScoreLogDialog();
	ASSERT( dialog );

	CString guildName;
	mNameEdit.GetWindowText( guildName );

	if( guildName.IsEmpty() )
	{
		CString textThereIsNoTarget;
		textThereIsNoTarget.LoadString( IDS_STRING31 );

		MessageBox( textThereIsNoTarget, "", MB_ICONERROR | MB_OK );
		return;
	}

	dialog->DoModal( mApplication.GetServerName( mServerIndex ), guildName, mGuildIndex );
}


void CGuildDialog::OnBnClickedGuildMemberModifyButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 직위 변경

	// 각 직위 간의 값 차이가 10임을 이용하자
	const DWORD rank = mRankCombo.GetCurSel() * abs( GUILD_STUDENT - GUILD_MEMBER );

	// 플레이어 인덱스 세팅
	CString	playerRank;
	CString	playerName;
	DWORD	playerIndex = 0;
	{
		POSITION		position	= mMemberList.GetFirstSelectedItemPosition();	
		const DWORD		row			= mMemberList.GetNextSelectedItem( position );
		const CString	index		= mMemberList.GetItemText( row, 0 );

		if( index.IsEmpty() )
		{
			CString textThereIsNoTarget;
			textThereIsNoTarget.LoadString( IDS_STRING31 );

			MessageBox( textThereIsNoTarget, "", MB_ICONERROR | MB_OK );
			return;
		}
		else if( mApplication.GetRankName( rank ) == mMemberList.GetItemText( row, 1 ) )
		{
			return;
		}

		playerIndex	= atoi( index );
		playerName	= mMemberList.GetItemText( row, 2 );
		playerRank	= mMemberList.GetItemText( row, 1 );
	}

	// 탈퇴 처리
	if( rank == GUILD_NOTMEMBER )
	{
		if( 1 == mMemberList.GetItemCount() )
		{
			CString textKickIsFailedByMemberIsOne;
			textKickIsFailedByMemberIsOne.LoadString( IDS_STRING41 );

			MessageBox( textKickIsFailedByMemberIsOne, "", MB_ICONERROR | MB_OK );
			return;
		}

		CString textWarningAboutKicking;
		textWarningAboutKicking.LoadString( IDS_STRING48 );

		CString text;
		text.Format(
			textWarningAboutKicking,
			playerName );

		if( IDNO == MessageBox( text, "", MB_ICONWARNING | MB_YESNO ) )
		{
			mRankCombo.SetCurSel( mRankCombo.FindString( 0, playerRank ) );
			return;
		}

		MSGBASE message;
		message.Category	= MP_RM_GUILD;
		message.Protocol	= MP_RM_GUILD_KICK_SYN;
		message.dwObjectID	= playerIndex;

		mApplication.Send( mServerIndex, message, sizeof( message ) );
		return;
	}
    
	const CString	masterRank		= mApplication.GetRankName( GUILD_MASTER );
	CString			masterName;
	DWORD			masterIndex		= 0;
	{
		for( int row = 0; mMemberList.GetItemCount() > row; ++row )
		{
			const CString rankName = mMemberList.GetItemText( row, 1 );

			if( masterRank == rankName )
			{
				masterIndex = atoi( mMemberList.GetItemText( row, 0 ) );
				masterName	= mMemberList.GetItemText( row, 2 );
			}
		}
	}

	const CString playerNewRank = mApplication.GetRankName( rank );

	if( masterIndex )
	{
		CString text;

		// 현재 마스터 직위를 낮출 때 경고 표시
		if( masterIndex == playerIndex )
		{
			CString textWarningThatMasterChangeToLowerRank;
			textWarningThatMasterChangeToLowerRank.LoadString( IDS_STRING48 );

			text.Format(
				textWarningThatMasterChangeToLowerRank,
				playerName,
				playerNewRank,
				masterRank );

			if( IDNO == MessageBox( text, "", MB_ICONWARNING | MB_YESNO ) )
			{
				mRankCombo.SetCurSel( mRankCombo.FindString( 0, playerRank ) );
				return;
			}
		}
		// 마스터가 아닌 회원을 마스터로 할 때 경고
		else if( GUILD_MASTER == rank )
		{
			CString textWarningThatTwoMasterIs;
			textWarningThatTwoMasterIs.LoadString( IDS_STRING49 );

			text.Format(
				textWarningThatTwoMasterIs,
				masterName,
				masterRank );

			MessageBox( text, "", MB_ICONWARNING | MB_OK );
			mRankCombo.SetCurSel( mRankCombo.FindString( 0, playerRank ) );
			return;
		}
		// 일반 경우
		else
		{
			CString textWarningAboutChangeRank;
			textWarningAboutChangeRank.LoadString( IDS_STRING50 );
			text.Format(
				textWarningAboutChangeRank,
				playerNewRank,
				playerName );

			if( IDNO == MessageBox( text, "", MB_ICONWARNING | MB_YESNO ) )
			{
				mRankCombo.SetCurSel( mRankCombo.FindString( 0, playerRank ) );
				return;
			}
		}
	}
	else
	{
		CString textWarningAboutChangeRank;
		textWarningAboutChangeRank.LoadString( IDS_STRING50 );

		CString text;
		text.Format(
			textWarningAboutChangeRank,
			playerNewRank,
			playerName );

		if( IDNO == MessageBox( text, "", MB_ICONWARNING | MB_YESNO ) )
		{
			mRankCombo.SetCurSel( mRankCombo.FindString( 0, playerRank ) );
			return;
		}
	}

	MSG_DWORD2 message;
	message.Category	= MP_RM_GUILD;
	message.Protocol	= MP_RM_GUILD_SET_RANK_SYN;
	message.dwObjectID	= playerIndex;
	message.dwData1		= mGuildIndex;
	message.dwData2		= rank;
	
	mApplication.Send( mServerIndex, message, sizeof( message ) );
}


void CGuildDialog::OnLvnItemchangedGuildMemberList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	CString rankname;
	{
		POSITION position = mMemberList.GetFirstSelectedItemPosition();

		rankname = mMemberList.GetItemText( mMemberList.GetNextSelectedItem( position ), 1 );

		if( rankname.IsEmpty() )
		{
			return;
		}
	}

	mRankCombo.SetCurSel( mRankCombo.FindString( 0, rankname ) );
}


void CGuildDialog::OnCbnSelchangeGuildRankCombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//// 멤버 목록이 있는데도 직위를 선택하지 않았으면 기본값으로 세팅해야한다.
	//if( mMemberList.GetItemCount() )
	//{
	//	if( ! mRankCombo.GetCurSel() )
	//	{
	//		POSITION position = mMemberList.GetFirstSelectedItemPosition();

	//		const CString rankName = mMemberList.GetItemText( mMemberList.GetNextSelectedItem( position ), 1 );

	//		mRankCombo.SetCurSel( mRankCombo.FindString( 0, rankName ) );
	//	}
	//}
	// 회원 목록이 없으면 무조건 기본값
	if( ! mMemberList.GetItemCount() )
	{
		mRankCombo.SetCurSel( 0 );
	}
}



void CGuildDialog::OnLvnColumnclickGuildMemberList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem= &(pDispInfo)->item;
	
	if (pItem->mask & LVIF_TEXT) //valid text buffer?
	{
		
	}
}

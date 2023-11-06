// NewUserDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "client.h"
#include "NewUserDialog.h"
#include "QuestDialog.h"
#include "SkillDialog.h"
#include "FamilyDialog.h"
#include "ItemDialog.h"
#include "PlayerDialog.h"
#include "UserDialog.h"
#include ".\newuserdialog.h"


bool SortByStandIndex( const CNewUserDialog::Player& lhs, const CNewUserDialog::Player& rhs )
{
	if( lhs.mStandIndex == rhs.mStandIndex )
	{
		return lhs.mPlayerIndex < rhs.mPlayerIndex;
	}
	
	return lhs.mStandIndex < rhs.mStandIndex;
}


// CNewUserDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNewUserDialog, CDialog)
CNewUserDialog::CNewUserDialog( CclientApp& application, CWnd* pParent /*=NULL*/)
: CDialog(CNewUserDialog::IDD, pParent)
, mApplication( application )
{
	EnableAutomation();
}


CNewUserDialog::~CNewUserDialog()
{
}


void CNewUserDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control( pDX, IDC_NEWUSER_NAME_RADIO, mNameRadioButton );
	DDX_Control( pDX, IDC_NEWUSER_ID_RADIO, mIdRadioButton );
	DDX_Control( pDX, IDC_NEWUSER_SERVER_COMBO, mServerComboBox );
	DDX_Control( pDX, IDC_NEWUSER_RESULT_COMBO, mPlayerComboBox );

	DDX_Text( pDX, IDC_NEWUSER_KEYWORD_EDIT, mKeyword );
	DDX_Control(pDX, IDC_NEWUSER_RESULT_SIZE_STATIC2, mPlayerSizeStatic);
	DDX_Control(pDX, IDC_NEWUSER_CHARACTER_RADIO, mPlayerRadioButton);
	DDX_Control(pDX, IDC_NEWUSER_PLAYER_ID_RADIO, mIndexRadioButton);
	DDX_Control(pDX, IDC_NEWUSER_USER_COMBO, mUserComboBox);
	DDX_Control(pDX, IDC_NEWUSER_USER_SIZE_STATIC, mUserSizeStatic);
	DDX_Control(pDX, IDC_NEWUSER_KEYWORD_EDIT, mKeywordEdit);
}


BEGIN_MESSAGE_MAP(CNewUserDialog, CDialog)
	ON_BN_CLICKED(IDC_NEWUSER_SEARCH_BUTTON, OnBnClickedNewuserSearchButton)
	ON_COMMAND(ID_MENU_NEWUSER_DIALOG, OnMenuNewuserDialog)
	ON_BN_CLICKED(IDC_NEWUSER_PLAYER_BUTTON, OnBnClickedNewuserPlayerButton)
	ON_BN_CLICKED(IDC_NEWUSER_ITEM_BUTTON, OnBnClickedNewuserItemButton)
	ON_BN_CLICKED(IDC_NEWUSER_SKILL_BUTTON, OnBnClickedNewuserSkillButton)
	ON_BN_CLICKED(IDC_NEWUSER_FAMILY_BUTTON, OnBnClickedNewuserFamilyButton)
	ON_BN_CLICKED(IDC_NEWUSER_BUTTON_QUEST, OnBnClickedNewuserButtonQuest)
	//ON_EN_CHANGE(IDC_NEWUSER_KEYWORD_EDIT, OnEnChangeNewuserKeywordEdit)
	//	ON_WM_KEYDOWN()
	//ON_BN_CLICKED(IDC_NEWUSER_BUTTON_QUEST2, OnBnClickedNewuserButtonQuest2)
	ON_CBN_SELCHANGE(IDC_NEWUSER_USER_COMBO, OnCbnSelchangeNewuserUserCombo)
	ON_BN_CLICKED(IDC_NEWUSER_USER_DATA_BUTTON, OnBnClickedNewuserUserDataButton)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CNewUserDialog, CDialog)
END_DISPATCH_MAP()

// 참고: IID_INewUserDialog에 대한 지원을 추가하여
// VBA에서 형식 안정적인 바인딩을 지원합니다. 
// 이 IID는 .IDL 파일에 있는 dispinterface의 GUID와 일치해야 합니다.

// {14588389-2684-4AC5-90DA-170D99459F17}
static const IID IID_INewUserDialog =
{ 0x14588389, 0x2684, 0x4AC5, { 0x90, 0xDA, 0x17, 0xD, 0x99, 0x45, 0x9F, 0x17 } };

BEGIN_INTERFACE_MAP(CNewUserDialog, CDialog)
	INTERFACE_PART(CNewUserDialog, IID_INewUserDialog, Dispatch)
END_INTERFACE_MAP()


// CNewUserDialog 메시지 처리기입니다.
void CNewUserDialog::Parse( DWORD serverIndex, const MSGROOT* message )
{
	mServerIndex = serverIndex;

	switch( message->Protocol )
	{
	case MP_RM_SEARCH_ACK:
		{
			const MSG_RM_OPEN_ACK* m = ( MSG_RM_OPEN_ACK* )message;

			if( ! m->mSize )
			{
				break;
			}

			CString name( m->mData[ 0 ].mUserName );
			name.TrimRight();

			PlayerList& playerList = mUserMap[ mUserComboBox.FindString( 0, name ) ];

			// 캐릭터는 삭제되어있을 수도, 존재할 수도 있다. A가 생성했다 삭제한 것을
			// B가 역시 생성했다 삭제할 수도 있다. 캐릭터명이 같은데도 서로 상이한
			// 유저 정보를 얻는 과정에서 부득이하게 중복된 검색 결과가 서버에서 전송된다
			// 이를 걸러내야한다. 이를 위해 플레이어 인덱스가 담긴 셋을 만들어 
			// 중복 여부를 검사한다
			stdext::hash_set< DWORD > playerIndexSet;
			{
				for(
					PlayerList::const_iterator it = playerList.begin();
					playerList.end() != it;
					++it )
				{
					const Player& player = *it;

					playerIndexSet.insert( player.mPlayerIndex );
				}
			}

			for( DWORD i = 0; i < m->mSize; ++i )
			{
				const MSG_RM_OPEN_ACK::Data& data = m->mData[ i ];

				Player player = { 0 };
				{
					player.mPlayerIndex	= data.mPlayerIndex;
					player.mServerIndex	= serverIndex;
					player.mUserIndex	= data.mUserIndex;
					player.mStandIndex	= data.mStandIndex;
					player.mName		= data.mPlayerName;
				}

				if( playerIndexSet.end() == playerIndexSet.find( player.mPlayerIndex ) )
				{
					playerList.push_back( player );
				}
			}

			playerList.sort( SortByStandIndex );

			OnCbnSelchangeNewuserUserCombo();
			break;
		}
	case MP_RM_SEARCH_USER_NAME_ACK:
		{
			const MSG_NAME* m = ( MSG_NAME* )message;

			if( ! *m->Name )
			{
				break;
			}

			mUserList.push_back( User() );
			User& user = mUserList.back();

			user.mName		= m->Name;
			user.mUserIndex	= m->dwObjectID;

			user.mName.TrimRight();

			if( -1 < mUserComboBox.FindString( 0, user.mName ) )
			{
				break;
			}

			mUserComboBox.AddString( user.mName );
			mUserComboBox.SetCurSel( 0 );

			{
				CString text;
				text.Format( "%d", mUserComboBox.GetCount() );

				mUserSizeStatic.SetWindowText( text );
			}

			break;
		}
	case MP_RM_SEARCH_NACK:
		{
			CString textThereIsNoResult;
			textThereIsNoResult.LoadString( IDS_STRING1 );

			MessageBox( textThereIsNoResult, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_SEARCH_NACK_BY_AUTH:
		{
			CString textYouHaveNoAuthority;
			textYouHaveNoAuthority.LoadString( IDS_STRING18 );

			MessageBox( textYouHaveNoAuthority, "", MB_OK | MB_ICONERROR );
			break;
		}
	default:
		{
			ASSERT( 0 && "It's not defined protocol" );
			break;
		}
	}
}


BOOL CNewUserDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon( AfxGetApp()->LoadIcon( IDR_MAINFRAME ), FALSE );
	SetIcon( AfxGetApp()->LoadIcon( IDR_MAINFRAME ), TRUE );

	{
		mPlayerSizeStatic.SetWindowText( "" );
		mUserSizeStatic.SetWindowText( "" );
	}

	// 라디오 버튼
	{
		mNameRadioButton.SetCheck( BST_UNCHECKED );
		mIdRadioButton.SetCheck( BST_CHECKED );
		mIndexRadioButton.SetCheck( BST_UNCHECKED );
		mPlayerRadioButton.SetCheck( BST_UNCHECKED );
	}

	{
		mKeywordEdit.SetLimitText( MAX_NAME_LENGTH );
	}

	{
		CRect rect;

		{
			mServerComboBox.GetWindowRect( rect );
			mServerComboBox.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );

			mApplication.SetServerComboBox( mServerComboBox );
		}

		{			
			mPlayerComboBox.GetWindowRect( rect );
			mPlayerComboBox.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );
		}

		{
			mUserComboBox.GetWindowRect( rect );
			mUserComboBox.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}




void CNewUserDialog::OnBnClickedNewuserSearchButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if( mApplication.GetWaitDialog()->GetSafeHwnd() )
	{
		return;
	}

	CDataExchange dx( this, TRUE );
	DoDataExchange( &dx );

	if( mKeyword.IsEmpty() )
	{
		CString textKeywordIsEmpty;
		textKeywordIsEmpty.LoadString( IDS_STRING32 );

		MessageBox( textKeywordIsEmpty, "", MB_OK );
		return;
	}
	else if( MAX_NAME_LENGTH < mKeyword.GetLength() )
	{
		ASSERT( 0 );
	}	

	// 서버에서 데이터를 단속적으로 보내서 추가하므로, 결과가 오기전에 초기화해야한다
	{
		mPlayerSizeStatic.SetWindowText( "0" );
		mPlayerComboBox.ResetContent();		
		
		mUserSizeStatic.SetWindowText( "0" );
		mUserComboBox.ResetContent();
	}

	//mApplication.SetWait( TRUE );

	{
		MSG_RM_OPEN_SYN message;

		message.Category	= MP_RM_SEARCH;
		message.Protocol	= MP_RM_SEARCH_SYN;

		if( BST_CHECKED == mIdRadioButton.GetCheck() )
		{
			message.mType	= MSG_RM_OPEN_SYN::LoginId;
		}
		else if( BST_CHECKED == mNameRadioButton.GetCheck() )
		{
			message.mType	= MSG_RM_OPEN_SYN::RealName;
		}
		else if( BST_CHECKED == mPlayerRadioButton.GetCheck() )
		{
			message.mType	= MSG_RM_OPEN_SYN::PlayerName;
		}
		else if( BST_CHECKED == mIndexRadioButton.GetCheck() )
		{
			message.mType	= MSG_RM_OPEN_SYN::PlayerIndex;

			if( ! atoi( mKeyword ) )
			{
				ASSERT( 0 && "Input number more than 0" );
				return;
			}
		}
		else
		{
			ASSERT( 0 && "You should select one radio check at least" );
			return;
		}

		// 초기화
		{
			//mPlayerMap.clear();
			//mPlayerComboMap.clear();

			//mPlayerComboBox.ResetContent();
			//mPlayerSizeStatic.SetWindowText( "" );
			//mUserSizeStatic.SetWindowText( "" );

			mUserMap.clear();
			mUserList.clear();
		}

		_tcscpy( message.mKeyword, mKeyword.GetString() );
		
		CclientApp::ConvertWildCardForDb( message.mKeyword );

		CString serverName;
		mServerComboBox.GetLBText( mServerComboBox.GetCurSel(), serverName );

		mApplication.Send( serverName, message, sizeof( message ) );
	}
}


void CNewUserDialog::OnMenuNewuserDialog()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	ShowWindow( SW_SHOW );
}

void CNewUserDialog::OnBnClickedNewuserPlayerButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	const Player& player = GetPlayer();

	if( ! player.mPlayerIndex )
	{
		CString textThereIsNoTarget;
		textThereIsNoTarget.LoadString( IDS_STRING31 );

		MessageBox( textThereIsNoTarget, "", MB_OK );
		return;
	}

	{
		CPlayerDialog* dialog = mApplication.GetPlayerDialog();
		ASSERT( dialog );

		dialog->Request( GetServerIndex(), player.mName, player.mPlayerIndex );
	}
}

void CNewUserDialog::OnBnClickedNewuserItemButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	const Player& player = GetPlayer();

	if( ! player.mPlayerIndex )
	{
		CString textThereIsNoTarget;
		textThereIsNoTarget.LoadString( IDS_STRING31 );

		MessageBox( textThereIsNoTarget, "", MB_OK );
		return;
	}

	{
		CItemDialog* dialog = mApplication.GetItemDialog();
		ASSERT( dialog );

		dialog->Request( GetServerIndex(), player.mName, player.mPlayerIndex, player.mUserIndex );
	}
}


const CNewUserDialog::Player& CNewUserDialog::GetPlayer() const
{
	static const Player emptyPlayer;

	UserMap::const_iterator user_it = mUserMap.find( mUserComboBox.GetCurSel() );

	if( mUserMap.end() == user_it )
	{
		return emptyPlayer;
	}

	const PlayerList& playerList = user_it->second;

	if( playerList.size() <= size_t( mPlayerComboBox.GetCurSel() ) )
	{
		return emptyPlayer;
	}

	PlayerList::const_iterator it = playerList.begin();

	std::advance( it, mPlayerComboBox.GetCurSel() );

	return *it;
}


//CString CNewUserDialog::GetPlayerName() const
//{
//	int i = 0;
//
//	if( mPlayerComboBox.GetCount() )
//	{
//		CString name;
//
//		mPlayerComboBox.GetLBText( mPlayerComboBox.GetCurSel(), name );
//
//		return name.Tokenize( "() \t ", i );
//	}
//
//	return "";
//}


DWORD CNewUserDialog::GetServerIndex() const
{
	CString serverName;

	mServerComboBox.GetLBText( mServerComboBox.GetCurSel(), serverName );

	return mApplication.GetServerIndex( serverName );
}


//DWORD CNewUserDialog::GetPlayerIndex() const
//{
//	// 사용자 콤보에 정보가 있으면 이걸 기준으로 조회한다(동일한 플레이어 이름이 있을 수 있으므로)
//
//	UserMap::const_iterator user_it = mUserMap.find( mUserComboBox.GetCurSel() );
//
//	if( mUserMap.end() == user_it )
//	{
//		return 0;
//	}
//
//	const PlayerList& playerList = user_it->second;
//
//	if( playerList.size() <= size_t( mPlayerComboBox.GetCurSel() ) )
//	{
//		return 0;
//	}
//
//	PlayerList::const_iterator it = playerList.begin();
//	
//	std::advance( it, mPlayerComboBox.GetCurSel() );
//
//	const Player& player = *it;
//
//	return player.mPlayerIndex;
//}


void CNewUserDialog::OnBnClickedNewuserSkillButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	const Player& player = GetPlayer();

	if( ! player.mPlayerIndex )
	{
		CString textThereIsNoTarget;
		textThereIsNoTarget.LoadString( IDS_STRING31 );

		MessageBox( textThereIsNoTarget, "", MB_OK );
		return;
	}

	{
		CSkillDialog* dialog = mApplication.GetSkillDialog();
		ASSERT( dialog );

		dialog->Request( GetServerIndex(), player.mName, player.mPlayerIndex );
	}
}


void CNewUserDialog::OnBnClickedNewuserFamilyButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	const Player& player = GetPlayer();

	if( ! player.mPlayerIndex )
	{
		CString textThereIsNoTarget;
		textThereIsNoTarget.LoadString( IDS_STRING31 );

		MessageBox( textThereIsNoTarget, "", MB_OK );
		return;
	}

	{
		CFamilyDialog* dialog = mApplication.GetFamilyDialog();
		ASSERT( dialog );

		dialog->Request( GetServerIndex(), player.mName, player.mPlayerIndex );
	}
}


void CNewUserDialog::OnBnClickedNewuserButtonQuest()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	const Player& player = GetPlayer();

	if( ! player.mPlayerIndex )
	{
		CString textThereIsNoTarget;
		textThereIsNoTarget.LoadString( IDS_STRING31 );

		MessageBox( textThereIsNoTarget, "", MB_OK );
		return;
	}

	CQuestDialog* dialog = mApplication.GetQuestDialog();
	ASSERT( dialog );

	dialog->Request( GetServerIndex(), player.mName, player.mPlayerIndex );
}


void CNewUserDialog::OnCbnSelchangeNewuserUserCombo()
{
	//RefreshPlayerCombo();

	UserMap::const_iterator user_it = mUserMap.find( mUserComboBox.GetCurSel() );

	if( mUserMap.end() == user_it )
	{
		return;
	}

	const PlayerList& playerList = user_it->second;

	{
		CString	text;
		text.Format( "%d", playerList.size() );

		mPlayerSizeStatic.SetWindowText( text );
	}

	mPlayerComboBox.ResetContent();

	for(
		PlayerList::const_iterator it = playerList.begin();
		playerList.end() != it;
		++it )
	{
		const Player& player = *it;

		CString text;
		text.Format( "%s %s", player.mName, 5 > player.mStandIndex ? "" : "x" );

		mPlayerComboBox.AddString( text );
	}

	// 자동으로 검색항목을 선택 상태로 놓는다
	{
		const CString keyword = mKeyword.GetString();

		if( mPlayerRadioButton.GetCheck() )
		{					
			{
				const int index = mPlayerComboBox.FindString( -1, keyword );

				if( -1 < index )
				{
					mPlayerComboBox.SetCurSel( index );
				}
			}

			{
				CString text;
				text.Format( "%s ×", keyword );

				const int index = mPlayerComboBox.FindString( -1, text );

				if( -1 < index )
				{
					mPlayerComboBox.SetCurSel( index );
				}
			}
		}
	}

	mPlayerComboBox.SetCurSel( 0 );
}


void CNewUserDialog::OnBnClickedNewuserUserDataButton()
{
	CString userName;

	mUserComboBox.GetLBText( mUserComboBox.GetCurSel(), userName );

	if( userName.IsEmpty() )
	{
		return;
	}

	DWORD userIndex = 0;

	for( UserList::const_iterator it = mUserList.begin(); mUserList.end() != it; ++it )
	{
		const User& user = *it;

		if( user.mName == userName )
		{
			userIndex = user.mUserIndex;
			break;
		}
	}

	if( ! userIndex )
	{
		return;
	}

	CUserDialog* dialog = mApplication.GetUserDialog();
	ASSERT( dialog );

	dialog->Request( GetServerIndex(), userName, userIndex );
}


void CNewUserDialog::FindPlayer( DWORD serverIndex, DWORD playerIndex )
{
	{
		//mPlayerMap.clear();
		mPlayerComboBox.ResetContent();
	}

	{
		MSG_RM_OPEN_SYN message;

		message.Category	= MP_RM_SEARCH;
		message.Protocol	= MP_RM_SEARCH_SYN;
		message.mType		= MSG_RM_OPEN_SYN::PlayerIndex;
		_tcscpy( message.mKeyword, mKeyword.GetString() );

		mApplication.Send( mServerIndex, message, sizeof( message ) );
	}	
}
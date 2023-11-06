// UserDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "client.h"
#include "UserDialog.h"
#include "PlayerDialog.h"
#include "UserRestoreDialog.h"
#include "UserCreateDialog.h"


// CUserDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CUserDialog, CDialog)
CUserDialog::CUserDialog( CclientApp& application, CWnd* pParent /*=NULL*/)
	: CDialog(CUserDialog::IDD, pParent)
	, mApplication( application )
{
	EnableAutomation();

}

CUserDialog::~CUserDialog()
{
}

void CUserDialog::OnFinalRelease()
{
	// �ڵ�ȭ ��ü�� ���� ������ ������ �����Ǹ� OnFinalRelease��
	// ȣ��˴ϴ�. �⺻ Ŭ�������� �ڵ����� ��ü�� �����մϴ�.
	// �⺻ Ŭ������ ȣ���ϱ� ���� ��ü�� �ʿ��� �߰� ���� �۾���
	// �߰��Ͻʽÿ�.

	CDialog::OnFinalRelease();
}

void CUserDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_USER_LOGID_EDIT, midEdit);
	//DDX_Control(pDX, IDC_USER_PASSWORD_CHANGE_BUTTON, mPasswordChangeButton);
	//DDX_Control(pDX, IDC_USER_PASSWORD_HISTORY_BUTTON, mPasswordHistoryButton);
	DDX_Control(pDX, IDC_USER_CREATED_DATE_EDIT, mJoinedDateEdit);
	DDX_Control(pDX, IDC_USER_LOGOUT_DATE_EDIT, mLogoutDateEdit);
	//DDX_Control(pDX, IDC_USER_PLAY_TIME_EDIT, mPlayTimeEdit);
	//DDX_Control(pDX, IDC_USER_LEVEL_COMBO, mLevelCombo);
	DDX_Control(pDX, IDC_USER_CREATED_PLAYER_LIST, mLivePlayerListCtrl);
	//DDX_Control(pDX, IDC_USER_DELETED_PLAYER_LIST, mDeletedPlayerListCtrl);
	//DDX_Control(pDX, IDC_USER_CREATED_PLAYER_BUTTON, mLivePlayerButton);
	//DDX_Control(pDX, IDC_USER_DELETED_PLAYER_BUTTON, mDeletedPlayerButton);
	//DDX_Control(pDX, IDC_USER_LOGID_EDIT2, midEdit);
	//DDX_Control(pDX, IDC_USER_IS_CONNECT_STATIC, mIsConnectStatic);
	//DDX_Control(pDX, IDC_USER_CONNECT_STATIC, mConnectStatic);
	DDX_Control(pDX, IDC_USER_AGENT_STATIC2, mAgentStatic);
	DDX_Control(pDX, IDC_USER_DELETED_PLAYER_LIST2, mDeletedPlayerListCtrl);
	DDX_Control(pDX, IDC_USER_LIVE_PLAYER_BUTTON, mLivePlayerButton);
	DDX_Control(pDX, IDC_USER_DELETED_PLAYER_BUTTON2, mDeletedPlayerButton);
	DDX_Control(pDX, IDC_USER_LOGID_EDIT3, midEdit);
	DDX_Control(pDX, IDC_USER_CONNECT_STATIC2, mConnectStatic);
	DDX_Control(pDX, IDC_NAME_STATIC, mNameStatic);
	DDX_Control(pDX, IDC_USER_LEVEL_COMBO, mLevelCombo);
}


BEGIN_MESSAGE_MAP(CUserDialog, CDialog)
	//ON_BN_CLICKED(IDC_USER_PASSWORD_CHANGE_BUTTON, OnBnClickedUserPasswordChangeButton)
	//ON_BN_CLICKED(IDC_USER_PASSWORD_HISTORY_BUTTON, OnBnClickedUserPasswordHistoryButton)
	ON_BN_CLICKED(IDC_USER_CREATED_PLAYER_BUTTON, OnBnClickedUserLivePlayerButton)
	ON_BN_CLICKED(IDC_USER_DELETED_PLAYER_BUTTON, OnBnClickedUserDeletedPlayerButton)
	ON_BN_CLICKED(IDC_USER_UPDATE_BUTTON, OnBnClickedUserUpdateButton)
	ON_BN_CLICKED(IDC_USER_CREATE_BUTTON, OnBnClickedUserCreateButton)
	ON_BN_CLICKED(IDC_USER_RESTORE_BUTTON, OnBnClickedUserRestoreButton)
	ON_BN_CLICKED(IDC_USER_DELETE_BUTTON, OnBnClickedUserDeleteButton)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CUserDialog, CDialog)
END_DISPATCH_MAP()

// ����: IID_IUserDialog�� ���� ������ �߰��Ͽ�
// VBA���� ���� �������� ���ε��� �����մϴ�. 
// �� IID�� .IDL ���Ͽ� �ִ� dispinterface�� GUID�� ��ġ�ؾ� �մϴ�.

// {F335733F-FD60-4843-B9CD-9FB81EF04BB5}
static const IID IID_IUserDialog =
{ 0xF335733F, 0xFD60, 0x4843, { 0xB9, 0xCD, 0x9F, 0xB8, 0x1E, 0xF0, 0x4B, 0xB5 } };

BEGIN_INTERFACE_MAP(CUserDialog, CDialog)
	INTERFACE_PART(CUserDialog, IID_IUserDialog, Dispatch)
END_INTERFACE_MAP()


// CUserDialog �޽��� ó�����Դϴ�.

//void CUserDialog::OnBnClickedUserPasswordChangeButton()
//{
//	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//}
//
//
//void CUserDialog::OnBnClickedUserPasswordHistoryButton()
//{
//	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//}


void CUserDialog::OnBnClickedUserLivePlayerButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	POSITION		position	= mLivePlayerListCtrl.GetFirstSelectedItemPosition();
	const DWORD		row			= mLivePlayerListCtrl.GetNextSelectedItem( position );
	const CString	playerIndex	= mLivePlayerListCtrl.GetItemText( row, 0 );

	if( playerIndex.IsEmpty() )
	{
		CString textSelectTarget;
		textSelectTarget.LoadString( IDS_STRING31 );

		MessageBox( textSelectTarget, "", MB_ICONERROR | MB_OK );
		return;
	}

	const CString	playerName	= mLivePlayerListCtrl.GetItemText( row, 1 );

	CPlayerDialog* dialog = mApplication.GetPlayerDialog();
	ASSERT( dialog );

	dialog->Request( mServerIndex, playerName, atoi( playerIndex ) );
}


void CUserDialog::OnBnClickedUserDeletedPlayerButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	POSITION		position	= mDeletedPlayerListCtrl.GetFirstSelectedItemPosition();
	const DWORD		row			= mDeletedPlayerListCtrl.GetNextSelectedItem( position );
	const CString	playerIndex	= mDeletedPlayerListCtrl.GetItemText( row, 0 );

	if( playerIndex.IsEmpty() )
	{
		CString textSelectTarget;
		textSelectTarget.LoadString( IDS_STRING31 );

		MessageBox( textSelectTarget, "", MB_ICONERROR | MB_OK );
		return;
	}

	const CString	playerName	= mDeletedPlayerListCtrl.GetItemText( row, 1 );

	CPlayerDialog* dialog = mApplication.GetPlayerDialog();
	ASSERT( dialog );

	dialog->Request( mServerIndex, playerName, atoi( playerIndex ) );
}


BOOL CUserDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	SetIcon( AfxGetApp()->LoadIcon( IDR_MAINFRAME ), FALSE );
	SetIcon( AfxGetApp()->LoadIcon( IDR_MAINFRAME ), TRUE );

	// ����Ʈ ��Ʈ�� ����
	{
		const DWORD addStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
		CRect		rect;

		{
			int step = -1;

			mLivePlayerListCtrl.GetClientRect( rect );
			mLivePlayerListCtrl.SetExtendedStyle( mLivePlayerListCtrl.GetExtendedStyle() | addStyle );

			CString textDbIndex;
			textDbIndex.LoadString( IDS_STRING13 );
			mLivePlayerListCtrl.InsertColumn( ++step, textDbIndex, LVCFMT_LEFT, 0 );

			CString textName;
			textName.LoadString( IDS_STRING20 );
			mLivePlayerListCtrl.InsertColumn( ++step, textName, LVCFMT_LEFT, int( rect.Width() * 0.67f )  );
		}

		{
			int step = -1;

			mDeletedPlayerListCtrl.GetClientRect( rect );
			mDeletedPlayerListCtrl.SetExtendedStyle( mDeletedPlayerListCtrl.GetExtendedStyle() | addStyle );

			CString textDbIndex;
			textDbIndex.LoadString( IDS_STRING13 );
			mDeletedPlayerListCtrl.InsertColumn( ++step, textDbIndex, LVCFMT_LEFT, 0 );

			CString textName;
			textName.LoadString( IDS_STRING20 );
			mDeletedPlayerListCtrl.InsertColumn( ++step, textName, LVCFMT_LEFT, int( rect.Width() * 0.67f )  );
		}
	}

	// �޺� �ڽ� ����
	{
		CRect rect;
		mLevelCombo.GetWindowRect( rect );
		mLevelCombo.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );

		mApplication.SetUserLevelCombo( mLevelCombo );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CUserDialog::Request( DWORD serverIndex, const CString& userName, DWORD userIndex )
{
	mServerIndex	= serverIndex;
	mUserName		= userName;
	mUserIndex		= userIndex;
	
	{
		CString textTitleFormat;
		textTitleFormat.LoadString( IDS_STRING212 );

		CString	text;
		text.Format( textTitleFormat, mApplication.GetServerName( serverIndex ), userName );

		SetWindowText( text );
	}

	mLivePlayerListCtrl.DeleteAllItems();
	mDeletedPlayerListCtrl.DeleteAllItems();

	// ������ ���������� ��Ʈ���� ��ٴ�
	{
		mLivePlayerListCtrl.EnableWindow( FALSE );
		mDeletedPlayerListCtrl.EnableWindow( FALSE );
		mLevelCombo.EnableWindow( FALSE );
	}

	{
		MSG_NAME message;
		ZeroMemory( &message, sizeof( message ) );

		message.Category	= MP_RM_USER;
		message.Protocol	= MP_RM_USER_GET_DATA_SYN;
		message.dwObjectID	= userIndex;
		StringCopySafe( message.Name, userName, sizeof( message.Name ) );
		
		mApplication.Send( mServerIndex, message, sizeof( message ) );
	}
}


void CUserDialog::Parse( DWORD serverIndex, const MSGROOT* message )
{
	mServerIndex = serverIndex;

	switch( message->Protocol )
	{
	case MP_RM_USER_GET_DATA_ACK:
		{
			memcpy( &mReceivedData, message, sizeof( mReceivedData ) );

			mJoinedDateEdit.SetWindowText( mReceivedData.mJoinedDate );
			mLogoutDateEdit.SetWindowText( mReceivedData.mLogedoutDate );

			mLevelCombo.SetCurSel( mReceivedData.mLevel - 1 );
			mLevelCombo.EnableWindow( TRUE );

			{
				CString text;
				const MSG_RM_USER_DATA::Connection& data = mReceivedData.mConnection;

				if( *data.mIp )
				{
					mConnectStatic.SetWindowText( CString( data.mIp ) + "���� ���� ��" );

					CString textWhichAgentUsed;
					textWhichAgentUsed.LoadString( IDS_STRING213 );

					text.Format( textWhichAgentUsed, data.mAgent );
						
					mAgentStatic.SetWindowText( text );
				}
				else
				{
					CString textDisconnect;
					textDisconnect.LoadString( IDS_STRING214 );

					mConnectStatic.SetWindowText( textDisconnect );
					mAgentStatic.SetWindowText( "" );
				}
			}

			{				
				int start = 0;
				midEdit.SetWindowText( CString( mReceivedData.mId ).Tokenize( " ", start ) );
				midEdit.SetSel( 0, midEdit.GetWindowTextLength() );
			}

			{
				int start = 0;
				mNameStatic.SetWindowText( CString( mReceivedData.mName ).Tokenize( " ", start ) );
			}

			break;
		}
	case MP_RM_USER_GET_PLAYER_LIST_ACK:
		{
			const MSG_RM_OPEN_ACK* m = ( MSG_RM_OPEN_ACK* )message;

			CString text;

			for( DWORD i = 0; i < m->mSize; ++i )
			{
				const MSG_RM_OPEN_ACK::Data& data = m->mData[ i ];

				CListCtrl&	listCtrl	= ( 5 > data.mStandIndex ? mLivePlayerListCtrl : mDeletedPlayerListCtrl );
				const DWORD row			= listCtrl.GetItemCount();
				int			step		= -1;

				text.Format( "%d", data.mPlayerIndex );
				listCtrl.InsertItem( row, text, ++step );

				listCtrl.SetItemText( row, ++step, data.mPlayerName );
			}

			// ������ ���������� ��ɴ� ��Ʈ���� Ȱ��ȭ
			{
				mLivePlayerListCtrl.EnableWindow( TRUE );
				mDeletedPlayerListCtrl.EnableWindow( TRUE );
			}

			break;
		}
	case MP_RM_USER_GET_DATA_NACK:
		{
			CString textThereIsNoResult;
			textThereIsNoResult.LoadString( IDS_STRING1 );

			MessageBox( textThereIsNoResult, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_USER_SET_DATA_ACK:
		{
			CString textUpdateWasSucceed;
			textUpdateWasSucceed.LoadString( IDS_STRING28 );

			MessageBox( textUpdateWasSucceed, "", MB_OK | MB_ICONINFORMATION );

			const MSG_DWORD2* m = ( MSG_DWORD2* )message;

			mReceivedData.mLevel = m->dwData2;
			break;
		}
	case MP_RM_USER_DELETE_NACK:
	case MP_RM_USER_SET_DATA_NACK:
		{
			CString textUpdateWasFailed;
			textUpdateWasFailed.LoadString( IDS_STRING28 );

			MessageBox( textUpdateWasFailed, "", MB_ICONERROR | MB_OK );

			Parse( mServerIndex, &mReceivedData );
			break;
		}
	case MP_RM_USER_DELETE_ACK:
		{
			const MSGBASE* m = ( MSGBASE* )message;

			// �÷��̾ Ȱ�� ����Ʈ���� ���� �÷��̾� ����Ʈ�� �̵���Ų��
			{
				POSITION	position	= mLivePlayerListCtrl.GetFirstSelectedItemPosition();
				const int	targetRow	= mLivePlayerListCtrl.GetNextSelectedItem( position );
				const int	destRow		= mDeletedPlayerListCtrl.GetItemCount();

				mDeletedPlayerListCtrl.InsertItem( destRow, mLivePlayerListCtrl.GetItemText( targetRow, 0 ), 0 );
				mDeletedPlayerListCtrl.SetItemText( destRow, 1, mLivePlayerListCtrl.GetItemText( targetRow, 1 ) );
				
				mLivePlayerListCtrl.DeleteItem( targetRow );
			}

			break;
		}
	case MP_RM_USER_DELETE_NACK_BY_FAMILY:
		{
			CString textMemberCanDeleteAfterSecedingFromFamily;
			textMemberCanDeleteAfterSecedingFromFamily.LoadString( IDS_STRING215 );

			MessageBox( textMemberCanDeleteAfterSecedingFromFamily, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_USER_DELETE_NACK_BY_PARTY:
		{
			CString textMemberCanDeleteAfterSecedingFromParty;
			textMemberCanDeleteAfterSecedingFromParty.LoadString( IDS_STRING216 );

			MessageBox( textMemberCanDeleteAfterSecedingFromParty, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_USER_DELETE_NACK_BY_GUILD:
		{
			CString textMemberCanDeleteAfterSecedingFromGuild;
			textMemberCanDeleteAfterSecedingFromGuild.LoadString( IDS_STRING217 );

			MessageBox( textMemberCanDeleteAfterSecedingFromGuild, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_USER_CREATE_ACK:
		{
			Request( mServerIndex, mUserName, mUserIndex );

			CString textUpdateWasSucceed;
			textUpdateWasSucceed.LoadString( IDS_STRING28 );

			MessageBox( textUpdateWasSucceed, "", MB_ICONINFORMATION | MB_OK );
			break;
		}
	case MP_RM_USER_CREATE_NACK:
	case MP_RM_USER_RESTORE_NACK:
		{
			CString textUpdateWasFailed;
			textUpdateWasFailed.LoadString( IDS_STRING29 );

			MessageBox( textUpdateWasFailed, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_USER_CREATE_NACK_BY_DUPLICATED_NAME:
		{
			CString textPlayerWhoHaveSameNameExist;
			textPlayerWhoHaveSameNameExist.LoadString( IDS_STRING218 );

			MessageBox( textPlayerWhoHaveSameNameExist, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_USER_CREATE_NACK_BY_GUILD:
		{
			CString textNameEqualToSomeGuild;
			textNameEqualToSomeGuild.LoadString( IDS_STRING219 );

			MessageBox( textNameEqualToSomeGuild, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_USER_CREATE_NACK_BY_FULL:
		{
			CString textYouCanNotCreatePlayerAnyMore;
			textYouCanNotCreatePlayerAnyMore.LoadString( IDS_STRING220 );

			MessageBox( textYouCanNotCreatePlayerAnyMore, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_USER_RESTORE_NACK_BY_FULL:
		{
			CString textMoreSpaceNeedToRestorePlayer;
			textMoreSpaceNeedToRestorePlayer.LoadString( IDS_STRING221 );

			MessageBox( textMoreSpaceNeedToRestorePlayer, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_USER_RESTORE_NACK_BY_NOT_EXIST:
		{
			CString textPlayerIsNotExisted;
			textPlayerIsNotExisted.LoadString( IDS_STRING222 );

			MessageBox( textPlayerIsNotExisted, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_USER_RESTORE_ACK:
		{
			const MSG_NAME* m = ( MSG_NAME* )message;

			{
				CUserDialog* dialog = mApplication.GetUserDialog();
				ASSERT( dialog );

				dialog->RestorePlayer( m->dwObjectID, m->Name );
			}

			break;
		}
	case MP_RM_USER_RESTORE_NACK_BY_DUPLICATED_NAME:
	case MP_RM_USER_RESTORE_NACK_BY_GUILD:
	case MP_RM_USER_RESTORE_NACK_BY_FAMILY:
		{
			const MSG_NAME* m = ( MSG_NAME* )message;

			{
				CString textWarningAboutDuplicatedName;
				textWarningAboutDuplicatedName.LoadString( IDS_STRING224 );

				CString text;
				text.Format( textWarningAboutDuplicatedName, m->Name );
				//mDescriptionStatic.SetWindowText( text );
			}			

			//mNameEdit.SetWindowText( mPlayerName );
			break;
		}
	case MP_RM_USER_NACK_BY_AUTH:
		{
			CString textYouHaveNoAuthority;
			textYouHaveNoAuthority.LoadString( IDS_STRING18 );

			MessageBox( textYouHaveNoAuthority, "", MB_OK | MB_ICONERROR );
			break;
		}
	default:
		{
			ASSERT( 0 && "It is no defined protocol" );
			break;
		}
	}
}


void CUserDialog::OnBnClickedUserUpdateButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	const DWORD level = mLevelCombo.GetCurSel() + 1;

	MSG_DWORD2	message;
	message.Category	= MP_RM_USER;
	message.Protocol	= MP_RM_USER_SET_DATA_SYN;
	message.dwObjectID	= mUserIndex;
	message.dwData1		= mReceivedData.mIndex;
	message.dwData2		= level;

	mApplication.Send( mServerIndex, message, sizeof( message ) );
}


void CUserDialog::OnBnClickedUserCreateButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if( MAX_CHARACTER_NUM <= mLivePlayerListCtrl.GetItemCount() )
	{
		CString textItIsMaximumForSpaceOfPlayer;
		textItIsMaximumForSpaceOfPlayer.LoadString( IDS_STRING224 );

		MessageBox( textItIsMaximumForSpaceOfPlayer, "", MB_ICONERROR | MB_OK );
		return;
	}

	/*
	{
		CUserCreateDialog* dialog = mApplication.GetUserCreateDialog();
		ASSERT( dialog );

		dialog->DoModal( mServerIndex, mReceivedData.mIndex, mReceivedData.mId );
	}
	*/

	CUserCreateDialog dialog( mApplication );

	if( IDOK != dialog.DoModal() )
	{
		return;
	}

    CHARACTERMAKEINFO message;
	{
		ZeroMemory( &message, sizeof( message ) );

		message.Category	= MP_RM_USER;
		message.Protocol	= MP_RM_USER_CREATE_SYN;
		message.UserID		= mUserIndex;
		message.FaceType	= dialog.GetFace();
		message.HairType	= dialog.GetHair();
		message.JobType		= dialog.GetJob();
		message.SexType		= dialog.GetGender();
		message.RaceType	= dialog.GetRace();
		StringCopySafe( message.Name, dialog.GetName(), sizeof( message.Name ) );
	}

	mApplication.Send( mServerIndex, message, sizeof( message ) );
}


void CUserDialog::OnBnClickedUserRestoreButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	const int maxPlayerSpaceSize = 5;

	if( maxPlayerSpaceSize < mLivePlayerListCtrl.GetItemCount() )
	{
		CString textItIsMaximumForSpaceOfPlayer;
		textItIsMaximumForSpaceOfPlayer.LoadString( IDS_STRING224 );

		MessageBox( textItIsMaximumForSpaceOfPlayer, "", MB_ICONERROR | MB_OK );
		return;
	}

	POSITION	position	= mDeletedPlayerListCtrl.GetFirstSelectedItemPosition();
	const int	row			= mDeletedPlayerListCtrl.GetNextSelectedItem( position );

	const CString playerName	= mDeletedPlayerListCtrl.GetItemText( row, 1 );
	const CString playerIndex	= mDeletedPlayerListCtrl.GetItemText( row, 0 );

	if( playerName.IsEmpty() )
	{
		CString textSelectTarget;
		textSelectTarget.LoadString( IDS_STRING31 );

		MessageBox( textSelectTarget, "", MB_ICONERROR | MB_OK );
		return;
	}

	CUserRestoreDialog dialog( mApplication );

	if(	IDOK != dialog.DoModal( playerName ) )
	{
		return;
	}

	

	MSG_NAME message;
	{
		message.Category	= MP_RM_USER;
		message.Protocol	= MP_RM_USER_RESTORE_SYN;
		message.dwObjectID	= atoi( playerIndex );
		StringCopySafe( message.Name, dialog.GetName(), sizeof( message.Name ) );
	}

	mApplication.Send( mServerIndex, message, sizeof( message ) );
}


void CUserDialog::OnBnClickedUserDeleteButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// ���õ� ĳ���Ͱ� �־����

	POSITION	position	= mLivePlayerListCtrl.GetFirstSelectedItemPosition();
	const int	row			= mLivePlayerListCtrl.GetNextSelectedItem( position );

	const CString playerName	= mLivePlayerListCtrl.GetItemText( row, 1 );
	const CString playerIndex	= mLivePlayerListCtrl.GetItemText( row, 0 );

	if( playerName.IsEmpty() )
	{
		CString textSelectTarget;
		textSelectTarget.LoadString( IDS_STRING31 );

		MessageBox( textSelectTarget, "", MB_ICONERROR | MB_OK );
		return;
	}

	{
		CString textWarningAboutDeletePlayer;
		textWarningAboutDeletePlayer.LoadString( IDS_STRING225 );

		CString text;
		text.Format(
			textWarningAboutDeletePlayer,
			playerName );

		if( IDNO == MessageBox( text, "", MB_ICONWARNING | MB_YESNO ) )
		{
			return;
		}
	}

	{
		MSGBASE message;
		message.Category	= MP_RM_USER;
		message.Protocol	= MP_RM_USER_DELETE_SYN;
		message.dwObjectID	= atoi( playerIndex );

		mApplication.Send( mServerIndex, message, sizeof( message ) );
	}
}


void CUserDialog::RestorePlayer( DWORD playerIndex, const CString& name )
{
	for( int row = 0; row < mDeletedPlayerListCtrl.GetItemCount(); ++row )
	{
		if( playerIndex != atoi( mDeletedPlayerListCtrl.GetItemText( row, 0 ) ) )
		{
			continue;
		}
		
		const int	destRow		= mLivePlayerListCtrl.GetItemCount();

		mLivePlayerListCtrl.InsertItem( destRow, mDeletedPlayerListCtrl.GetItemText( row, 0 ), 0 );
		mLivePlayerListCtrl.SetItemText( destRow, 1, name );

		mDeletedPlayerListCtrl.DeleteItem( row );
		break;
	}
}
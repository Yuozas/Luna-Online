// LoginDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "client.h"
#include "LoginDialog.h"
#include "..\common\version.h"


// CLoginDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CLoginDialog, CDialog)
CLoginDialog::CLoginDialog( CclientApp& application, CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDialog::IDD, pParent)
	, mApplication( application )
{
	EnableAutomation();
}

CLoginDialog::~CLoginDialog()
{}

void CLoginDialog::OnFinalRelease()
{
	// �ڵ�ȭ ��ü�� ���� ������ ������ �����Ǹ� OnFinalRelease��
	// ȣ��˴ϴ�. �⺻ Ŭ�������� �ڵ����� ��ü�� �����մϴ�.
	// �⺻ Ŭ������ ȣ���ϱ� ���� ��ü�� �ʿ��� �߰� ���� �۾���
	// �߰��Ͻʽÿ�.

	CDialog::OnFinalRelease();
}

void CLoginDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//DDX_Text( pDX, IDC_LOGIN_ID, mIdEdit );
	//DDX_Text( pDX, IDC_LOGIN_PASSWORD, mPasswordEdit );

	//DDX_Control( pDX, IDC_LOGIN_ID, mIdEdit );
	DDX_Control( pDX, IDC_LOGIN_PASSWORD, mPasswordEdit );
	//DDX_Control(pDX, IDC_LOGIN_VERSION_STATIC, mVersionStatic);
	DDX_Control(pDX, IDC_LOGIN_SERVER_LIST, mServerListCtrl);
	DDX_Control(pDX, IDC_LOGIN_VERSION_STATIC, mVersionStatic);
	DDX_Control(pDX, IDC_LOGIN_ID2, mIdEdit);
	DDX_Control(pDX, IDC_LOGIN_CONNECT_BUTTON, mSubmitButton);
	DDX_Control(pDX, IDCANCEL, mCancelButton);
}


BEGIN_MESSAGE_MAP(CLoginDialog, CDialog)
	//ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_BN_CLICKED(IDC_LOGIN_CONNECT_BUTTON, OnBnClickedLoginConnectButton)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CLoginDialog, CDialog)
END_DISPATCH_MAP()

// ����: IID_ILoginDialog�� ���� ������ �߰��Ͽ�
// VBA���� ���� �������� ���ε��� �����մϴ�. 
// �� IID�� .IDL ���Ͽ� �ִ� dispinterface�� GUID�� ��ġ�ؾ� �մϴ�.

// {E8293FEB-4841-4E9F-9297-89C994942D83}
static const IID IID_ILoginDialog =
{ 0xE8293FEB, 0x4841, 0x4E9F, { 0x92, 0x97, 0x89, 0xC9, 0x94, 0x94, 0x2D, 0x83 } };

BEGIN_INTERFACE_MAP(CLoginDialog, CDialog)
	INTERFACE_PART(CLoginDialog, IID_ILoginDialog, Dispatch)
END_INTERFACE_MAP()


// CLoginDialog �޽��� ó�����Դϴ�.

//void CLoginDialog::OnOK()
//{
//	CDialog::OnOK();
//}


void CLoginDialog::Parse( DWORD serverIndex, const MSGROOT* message )
{
	switch( message->Protocol )
	{
	case MP_RM_CONNECT_ACK:
		{
			static size_t count;

			if( mApplication.GetConnectionSize() == ++count )
			{
				// 080304 LUJ, �α�
				{
					CString id;
					mIdEdit.GetWindowText( id );

					const TCHAR* line = _T( "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *" );

					mApplication.Log( line );
					mApplication.Log(
						"\tWelcome to Recovery world: %s",
						id );
					mApplication.Log( line );
				}

				CString textWarning;
				textWarning.LoadString( IDS_STRING127 );

				MessageBox( textWarning, "", MB_ICONWARNING | MB_OK );

				OnOK();

				{
					CDialog* dialog = ( CDialog* )( mApplication.GetNewUserDialog() );
					ASSERT( dialog );

					dialog->ShowWindow( SW_SHOW );
				}
			}
			
			break;
		}
	case MP_RM_CONNECT_NACK:
		{
			MSG_DWORD* m = ( MSG_DWORD* )message;

			switch( m->dwData )
			{
			case 1:
				{
					CString textItIsNoConnectableIp;
					textItIsNoConnectableIp.LoadString( IDS_STRING130 );

					MessageBox( textItIsNoConnectableIp, "", MB_ICONERROR | MB_OK );

					OnCancel();
					break;
				}
			case 2:
			case 3:
				{
					CString textItIsWrongIdOrPassword;
					textItIsWrongIdOrPassword.LoadString( IDS_STRING134 );

					mVersionStatic.SetWindowText( textItIsWrongIdOrPassword );

					{
						mIdEdit.EnableWindow( TRUE );
						mPasswordEdit.EnableWindow( TRUE );
						mSubmitButton.EnableWindow( TRUE );
					}

					break;
				}
			default:
				{
					CString textItIsConnectionFault;
					textItIsConnectionFault.LoadString( IDS_STRING131 );

					MessageBox( textItIsConnectionFault, "", MB_ICONERROR | MB_OK );

					OnCancel();
					break;
				}
			}
			
			break;
		}
	case MP_RM_CONNECT_NACK_BY_IP:
	case MP_RM_CONNECT_NACK_BY_ID:
		{
			CString textYouHaveLoggedAlready;
			textYouHaveLoggedAlready.LoadString( IDS_STRING132 );

			MessageBox( textYouHaveLoggedAlready, "", MB_ICONERROR | MB_OK );

			OnCancel();
			break;
		}
	case MP_RM_CONNECT_NACK_BY_AUTH:
		{
			CString textYouAreBlocked;
			textYouAreBlocked.LoadString( IDS_STRING133 );

			MessageBox( textYouAreBlocked, "", MB_OK | MB_ICONERROR );

			OnCancel();
			break;
		}
	case MP_RM_CONNECT_NACK_BY_VERSION:
		{
			const MSG_INT2* m = ( MSG_INT2* )message;

			CString text;
			{
				CString textInvalidVersionFound;
				textInvalidVersionFound.LoadString( IDS_STRING343 );

				text.Format(
					textInvalidVersionFound,
					mApplication.GetServerName( serverIndex ),
					m->nData1,
					m->nData2 );
			}

			MessageBox( text, "", MB_OK | MB_ICONERROR );

			mApplication.Log( text );

            OnCancel();            			
			break;
		}
	default:
		{
			ASSERT( 0 );
			break;
		}
	}
}


BOOL CLoginDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

#ifdef _DEBUG
	mIdEdit.SetWindowText( "ehei" );
	mPasswordEdit.SetWindowText( "1488" );
#endif

	SetIcon( AfxGetApp()->LoadIcon( IDR_MAINFRAME ), FALSE );
	SetIcon( AfxGetApp()->LoadIcon( IDR_MAINFRAME ), TRUE );

	CString text;

	CString textBuildData;
	textBuildData.LoadString( IDS_STRING135 );

	text.Format( textBuildData, __DATE__, __TIME__ );
	mVersionStatic.SetWindowText( text );
	
	mIdEdit.SetFocus();

	// ����Ʈ ��Ʈ�� ����
	{
		const DWORD addStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
		CRect rect;

		{
			int step = -1;

			mServerListCtrl.GetWindowRect( rect );
			mServerListCtrl.SetExtendedStyle( mServerListCtrl.GetExtendedStyle() | addStyle );

			CString textIndex;
			textIndex.LoadString( IDS_STRING22 );
			mServerListCtrl.InsertColumn( ++step, textIndex, LVCFMT_LEFT, 0 );

			CString textName;
			textName.LoadString( IDS_STRING20 );
			mServerListCtrl.InsertColumn( ++step, textName, LVCFMT_LEFT, int( rect.Width() * 0.4f ) );

			CString textIpAddress;
			textIpAddress.LoadString( IDS_STRING136 );
			mServerListCtrl.InsertColumn( ++step, textIpAddress, LVCFMT_LEFT, int( rect.Width() * 0.35f ) );

			CString textEnable;
			textEnable.LoadString( IDS_STRING25 );
			mServerListCtrl.InsertColumn( ++step, textEnable, LVCFMT_LEFT, int( rect.Width() * 0.14f ) );
			
			mApplication.SetServerListCtrl( mServerListCtrl );
		}
	}

	// ���� ���¸� ������Ʈ�Ѵ�.
	mApplication.UpdateServerListCtrl( mServerListCtrl );

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CLoginDialog::OnBnClickedLoginConnectButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if( ! mApplication.GetConnectionSize() )
	{
		CString textAnyConnectableServerDoNotExist;
		textAnyConnectableServerDoNotExist.LoadString( IDS_STRING137 );

		MessageBox( textAnyConnectableServerDoNotExist, "", MB_ICONERROR | MB_OK );
		return;
	}

	CDataExchange dx( this, TRUE );
	DoDataExchange( &dx );

	CString id;
	CString password;

	mIdEdit.GetWindowText( id );
	mPasswordEdit.GetWindowText( password );

	if( id.IsEmpty() || password.IsEmpty() )
	{
		CString textIdOrPasswordIsEmpty;
		textIdOrPasswordIsEmpty.LoadString( IDS_STRING138 );

		MessageBox( textIdOrPasswordIsEmpty, "", MB_OK );
		return;
	}

	{
		mIdEdit.EnableWindow( FALSE );
		mPasswordEdit.EnableWindow( FALSE );
		mSubmitButton.EnableWindow( FALSE );
	}

	{
		//char hostName[ MAX_PATH ] = { 0 };
		//gethostname( hostName, sizeof(hostName) );

		MSG_RM_LOGIN message;
		{
			ZeroMemory( &message, sizeof( message ) );
			message.Category = MP_RM_CONNECT;
			message.Protocol = MP_RM_CONNECT_SYN;

			//const HOSTENT FAR* lphostent = ::gethostbyname( hostName );
			//_tcscpy( message.mAddress, inet_ntoa( * ( ( in_addr* )lphostent->h_addr ) ) );
			_tcscpy( message.mId,	id );
			_tcscpy( message.mPass, password );

			{
				MSG_RM_LOGIN::Version& version = message.mVersion;

				version.mMajor	= MAJORVERSION;
				version.mMinor	= MINORVERSION;
			}
		}
		
		mApplication.SendAll( message, sizeof( message ) );
	}
}
// PermissionDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "client.h"
#include "PermissionDialog.h"
#include ".\permissiondialog.h"


// CPermissionDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPermissionDialog, CDialog)
CPermissionDialog::CPermissionDialog( CclientApp& application, CWnd* pParent /*=NULL*/)
	: CDialog(CPermissionDialog::IDD, pParent),
		mApplication( application )
{
	EnableAutomation();

}

CPermissionDialog::~CPermissionDialog()
{
}

void CPermissionDialog::OnFinalRelease()
{
	// �ڵ�ȭ ��ü�� ���� ������ ������ �����Ǹ� OnFinalRelease��
	// ȣ��˴ϴ�. �⺻ Ŭ�������� �ڵ����� ��ü�� �����մϴ�.
	// �⺻ Ŭ������ ȣ���ϱ� ���� ��ü�� �ʿ��� �߰� ���� �۾���
	// �߰��Ͻʽÿ�.

	CDialog::OnFinalRelease();
}

void CPermissionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PERMISSION_IPADDRESS, mIpAddressCtrl);
}


BEGIN_MESSAGE_MAP(CPermissionDialog, CDialog)
	ON_BN_CLICKED(IDC_PERMISSION_ADD_BUTTON, OnBnClickedPermissionAddButton)
	ON_BN_CLICKED(IDC_PERMISSION_REMOVE_BUTTON, OnBnClickedPermissionRemoveButton)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CPermissionDialog, CDialog)
END_DISPATCH_MAP()

// ����: IID_IPermissionDialog�� ���� ������ �߰��Ͽ�
// VBA���� ���� �������� ���ε��� �����մϴ�. 
// �� IID�� .IDL ���Ͽ� �ִ� dispinterface�� GUID�� ��ġ�ؾ� �մϴ�.

// {E7861B05-F869-48A8-B029-C11065B5BA1F}
static const IID IID_IPermissionDialog =
{ 0xE7861B05, 0xF869, 0x48A8, { 0xB0, 0x29, 0xC1, 0x10, 0x65, 0xB5, 0xBA, 0x1F } };

BEGIN_INTERFACE_MAP(CPermissionDialog, CDialog)
	INTERFACE_PART(CPermissionDialog, IID_IPermissionDialog, Dispatch)
END_INTERFACE_MAP()


// CPermissionDialog �޽��� ó�����Դϴ�.

void CPermissionDialog::OnBnClickedPermissionAddButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	const CString address( GetIpAddress() );

	{
		MSG_NAME message;
		ZeroMemory( &message, sizeof( message ) );

		message.Category	= MP_RM_PERMISSION;
		message.Protocol	= MP_RM_PERMISSION_ADD_SYN;
		_tcscpy( message.Name, address );
		
		mApplication.Send( mServerIndex, message, sizeof( message ) );
	}	
}


void CPermissionDialog::OnBnClickedPermissionRemoveButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	BYTE field0;
	BYTE field1;
	BYTE field2;
	BYTE field3;

	mIpAddressCtrl.GetAddress( field0, field1, field2, field3 );

	if( ! field0 &&
		! field1 &&
		! field2 &&
		! field3 )
	{
		CString textInputAddress;
		textInputAddress.LoadString( IDS_STRING145 );

		MessageBox( textInputAddress, "", MB_ICONERROR | MB_OK );
		return;
	}

	{
		MSG_NAME message;

		message.Category	= MP_RM_PERMISSION;
		message.Protocol	= MP_RM_PERMISSION_REMOVE_SYN;
		sprintf( message.Name, "%d.%d.%d.%d", field0, field1, field2, field3 );

		mApplication.Send( mServerIndex, message, sizeof( message ) );
	}	
}


void CPermissionDialog::Parse( DWORD serverIndex, const MSGROOT* message )
{
	mServerIndex = serverIndex;

	switch( message->Protocol )
	{
	case MP_RM_PERMISSION_ADD_ACK:
		{
			CString textIpAddressIsAllowed;
			textIpAddressIsAllowed.LoadString( IDS_STRING157 );

			MessageBox( textIpAddressIsAllowed, "", MB_ICONINFORMATION | MB_OK );
			break;
		}
	case MP_RM_PERMISSION_ADD_NACK:
		{
			CString textIpAddressIsAllowedAlready;
			textIpAddressIsAllowedAlready.LoadString( IDS_STRING158 );

			MessageBox( textIpAddressIsAllowedAlready, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_PERMISSION_REMOVE_ACK:
		{
			CString textIpAddressIsRemoved;
			textIpAddressIsRemoved.LoadString( IDS_STRING159 );

			MessageBox( textIpAddressIsRemoved, "", MB_ICONINFORMATION | MB_OK );
			break;
		}
	case MP_RM_PERMISSION_REMOVE_NACK:
		{
			CString textIpAddressIsNotExisted;
			textIpAddressIsNotExisted.LoadString( IDS_STRING160 );

			MessageBox( textIpAddressIsNotExisted, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_PERMISSION_NACK_BY_AUTH:
		{
			CString textYouHaveNoAuthority;
			textYouHaveNoAuthority.LoadString( IDS_STRING18 );

			MessageBox( textYouHaveNoAuthority, "", MB_ICONERROR | MB_OK );
			break;
		}
	default:
		{
			ASSERT( 0 );
			break;
		}
	}
}


void CPermissionDialog::Request( DWORD serverIndex )
{
	mServerIndex = serverIndex;

	ShowWindow( SW_SHOW );
}


CString CPermissionDialog::GetIpAddress()
{
	BYTE field0;
	BYTE field1;
	BYTE field2;
	BYTE field3;

	mIpAddressCtrl.GetAddress( field0, field1, field2, field3 );	

	CString text;

	text.Format( "%d.%d.%d.%d", field0, field1, field2, field3 );

	return text;
}
// NameInputDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "client.h"
#include "UserRestoreDialog.h"
//#include "UserDialog.h"
//#include ".\userrestoredialog.h"


// CUserRestoreDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CUserRestoreDialog, CDialog)
CUserRestoreDialog::CUserRestoreDialog( CclientApp& application, CWnd* pParent /*=NULL*/)
	: CDialog(CUserRestoreDialog::IDD, pParent)
	, mApplication( application )
{
	EnableAutomation();
}

CUserRestoreDialog::~CUserRestoreDialog()
{
}

void CUserRestoreDialog::OnFinalRelease()
{
	// �ڵ�ȭ ��ü�� ���� ������ ������ �����Ǹ� OnFinalRelease��
	// ȣ��˴ϴ�. �⺻ Ŭ�������� �ڵ����� ��ü�� �����մϴ�.
	// �⺻ Ŭ������ ȣ���ϱ� ���� ��ü�� �ʿ��� �߰� ���� �۾���
	// �߰��Ͻʽÿ�.

	CDialog::OnFinalRelease();
}

void CUserRestoreDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NAME_INPUT_EDIT, mNameEdit);
}


BEGIN_MESSAGE_MAP(CUserRestoreDialog, CDialog)
	//ON_BN_CLICKED(IDC_USER_RESTORE_SUBMIT_BUTTON, OnBnClickedUserRestoreSubmitButton)
	//ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CUserRestoreDialog, CDialog)
END_DISPATCH_MAP()

// ����: IID_INameInputDialog�� ���� ������ �߰��Ͽ�
// VBA���� ���� �������� ���ε��� �����մϴ�. 
// �� IID�� .IDL ���Ͽ� �ִ� dispinterface�� GUID�� ��ġ�ؾ� �մϴ�.

// {3484BB64-9DB7-45D4-AF86-758A2EFC25E5}
static const IID IID_INameInputDialog =
{ 0x3484BB64, 0x9DB7, 0x45D4, { 0xAF, 0x86, 0x75, 0x8A, 0x2E, 0xFC, 0x25, 0xE5 } };

BEGIN_INTERFACE_MAP(CUserRestoreDialog, CDialog)
	INTERFACE_PART(CUserRestoreDialog, IID_INameInputDialog, Dispatch)
END_INTERFACE_MAP()


// CUserRestoreDialog �޽��� ó�����Դϴ�.


//CString CUserRestoreDialog::GetName() const
//{
//	//CString name;
//
//	//mNameEdit.GetWindowText( name );
//
//	//return name;
//	return mPlayerName;
//}


INT_PTR CUserRestoreDialog::DoModal( const CString& playerName )
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	mPlayerName = playerName;

	return CDialog::DoModal();
}


BOOL CUserRestoreDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	{
		mNameEdit.SetLimitText( MAX_NAME_LENGTH );
		mNameEdit.SetWindowText( mPlayerName );
		mNameEdit.SetSel( 0, mNameEdit.GetWindowTextLength() );
		mNameEdit.SetFocus();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CUserRestoreDialog::OnOK()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString name;

	mNameEdit.GetWindowText( name );

	if( name.IsEmpty() )
	{
		CString textEmptyValueFounded;
		textEmptyValueFounded.LoadString( IDS_STRING185 );

		MessageBox( textEmptyValueFounded, "", MB_ICONERROR | MB_OK );
		return;
	}

	mPlayerName = name;
	
	CDialog::OnOK();
}


const char* CUserRestoreDialog::GetName() const
{
	return mPlayerName;
}
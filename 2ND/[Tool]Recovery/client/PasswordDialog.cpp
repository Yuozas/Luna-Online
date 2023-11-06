// PasswordDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "client.h"
#include "PasswordDialog.h"
#include ".\passworddialog.h"


// CPasswordDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPasswordDialog, CDialog)
CPasswordDialog::CPasswordDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPasswordDialog::IDD, pParent)
{
	EnableAutomation();

}

CPasswordDialog::~CPasswordDialog()
{
}

void CPasswordDialog::OnFinalRelease()
{
	// �ڵ�ȭ ��ü�� ���� ������ ������ �����Ǹ� OnFinalRelease��
	// ȣ��˴ϴ�. �⺻ Ŭ�������� �ڵ����� ��ü�� �����մϴ�.
	// �⺻ Ŭ������ ȣ���ϱ� ���� ��ü�� �ʿ��� �߰� ���� �۾���
	// �߰��Ͻʽÿ�.

	CDialog::OnFinalRelease();
}

void CPasswordDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PASSWORD_PREV_EDIT, mPreviousPasswordEdit);
	DDX_Control(pDX, IDC_PASSWORD_NEXT_EDIT, mNextPasswordEdit);
	DDX_Control(pDX, IDC_PASSWORD_NEXT_CONFIRM_EDIT, mConfirmPasswordEdit);
}


BEGIN_MESSAGE_MAP(CPasswordDialog, CDialog)
	//ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CPasswordDialog, CDialog)
END_DISPATCH_MAP()

// ����: IID_IPasswordDialog�� ���� ������ �߰��Ͽ�
// VBA���� ���� �������� ���ε��� �����մϴ�. 
// �� IID�� .IDL ���Ͽ� �ִ� dispinterface�� GUID�� ��ġ�ؾ� �մϴ�.

// {75C97B48-E9A7-4909-84F5-1AE4525A95D7}
static const IID IID_IPasswordDialog =
{ 0x75C97B48, 0xE9A7, 0x4909, { 0x84, 0xF5, 0x1A, 0xE4, 0x52, 0x5A, 0x95, 0xD7 } };

BEGIN_INTERFACE_MAP(CPasswordDialog, CDialog)
	INTERFACE_PART(CPasswordDialog, IID_IPasswordDialog, Dispatch)
END_INTERFACE_MAP()


// CPasswordDialog �޽��� ó�����Դϴ�.


void CPasswordDialog::OnOK()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	CString previousPassword;
	mPreviousPasswordEdit.GetWindowText( previousPassword );

	CString nextPassword;
	{
		mNextPasswordEdit.GetWindowText( nextPassword );

		CString password;
		mConfirmPasswordEdit.GetWindowText( password );

		if( nextPassword != password )
		{
			mConfirmPasswordEdit.SetWindowText( "" );
			mNextPasswordEdit.SetWindowText( "" );

			CString textToConfirmPasswordIsWrong;
			textToConfirmPasswordIsWrong.LoadString( IDS_STRING153 );

			MessageBox( textToConfirmPasswordIsWrong, "", MB_OK | MB_ICONERROR );
			return;
		}
	}

	const int minimumSize = 6;

	if( minimumSize > nextPassword.GetLength() )
	{
		CString textPasswordMustHaveSpecifiedSize;
		textPasswordMustHaveSpecifiedSize.LoadString( IDS_STRING154 );

		CString text;
		text.Format( textPasswordMustHaveSpecifiedSize, minimumSize );

		MessageBox( text, "", MB_OK | MB_ICONERROR );
		return;
	}
	
	{
		for(
			const char* ch = nextPassword.GetString();
			*ch;
			++ch )
		{
			if(		IsDBCSLeadByte( *ch )	||
				!	isalnum( *ch ) )
			{
				CString textItIsAllowToUsePassword;
				textItIsAllowToUsePassword.LoadString( IDS_STRING155 );

				MessageBox( textItIsAllowToUsePassword, "", MB_OK | MB_ICONERROR );
				return;
			}

			if( ! isalpha( *ch ) &&
				! isdigit( *ch ) )
			{
				CString textYouShouldToMakePasswordWithMixingEnglishAndNumeric;
				textYouShouldToMakePasswordWithMixingEnglishAndNumeric.LoadString( IDS_STRING156 );

				MessageBox( textYouShouldToMakePasswordWithMixingEnglishAndNumeric, "", MB_OK | MB_ICONERROR );
				return;
			}
		}
	}

	mPreviousPassword	= previousPassword;
	mNextPassword		= nextPassword;

	CDialog::OnOK();
}


const char* CPasswordDialog::GetPreviousPassword() const
{
	return mPreviousPassword;
}


const char* CPasswordDialog::GetNextPassword() const
{
	return mNextPassword;
}
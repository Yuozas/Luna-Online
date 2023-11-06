// UserCreateDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "client.h"
#include "UserCreateDialog.h"
#include ".\usercreatedialog.h"
//#include "UserDialog.h"


// CUserCreateDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CUserCreateDialog, CDialog)
CUserCreateDialog::CUserCreateDialog( CclientApp& application, CWnd* pParent /*=NULL*/)	
	: CDialog(CUserCreateDialog::IDD, pParent)
	, mApplication( application )

{
	EnableAutomation();
}

CUserCreateDialog::~CUserCreateDialog()
{
}

void CUserCreateDialog::OnFinalRelease()
{
	// �ڵ�ȭ ��ü�� ���� ������ ������ �����Ǹ� OnFinalRelease��
	// ȣ��˴ϴ�. �⺻ Ŭ�������� �ڵ����� ��ü�� �����մϴ�.
	// �⺻ Ŭ������ ȣ���ϱ� ���� ��ü�� �ʿ��� �߰� ���� �۾���
	// �߰��Ͻʽÿ�.

	CDialog::OnFinalRelease();
}

void CUserCreateDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_USER_CREATE_RACE_COMBO, mRaceCombo);
	DDX_Control(pDX, IDC_USER_CREATE_JOB_COMBO, mJobCombo);
	DDX_Control(pDX, IDC_USER_CREATE_JOB_GENDER_COMBO, mGenderCombo);
	DDX_Control(pDX, IDC_USER_CREATE_HAIR_COMBO, mHairCombo);
	DDX_Control(pDX, IDC_USER_CREATE_FACE_COMBO, mFaceCombo);
	DDX_Control(pDX, IDC_USER_CREATE_NAME_EDIT, mNameEdit);
}


BEGIN_MESSAGE_MAP(CUserCreateDialog, CDialog)
	//ON_BN_CLICKED(IDC_USER_CREATE_BUTTON, OnBnClickedUserCreateButton)
	//ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CUserCreateDialog, CDialog)
END_DISPATCH_MAP()

// ����: IID_IUserCreateDialog�� ���� ������ �߰��Ͽ�
// VBA���� ���� �������� ���ε��� �����մϴ�. 
// �� IID�� .IDL ���Ͽ� �ִ� dispinterface�� GUID�� ��ġ�ؾ� �մϴ�.

// {10193BC3-1521-440A-92DB-FBDEF26FDF85}
static const IID IID_IUserCreateDialog =
{ 0x10193BC3, 0x1521, 0x440A, { 0x92, 0xDB, 0xFB, 0xDE, 0xF2, 0x6F, 0xDF, 0x85 } };

BEGIN_INTERFACE_MAP(CUserCreateDialog, CDialog)
	INTERFACE_PART(CUserCreateDialog, IID_IUserCreateDialog, Dispatch)
END_INTERFACE_MAP()


// CUserCreateDialog �޽��� ó�����Դϴ�.

BOOL CUserCreateDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	// edit box
	{
		mNameEdit.SetLimitText( MAX_NAME_LENGTH );
	}

	// �޺� �ڽ� ����
	{
		CRect		rect;

		// ����
		{
			mRaceCombo.GetWindowRect( rect );
			mRaceCombo.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );

			CString textHuman;
			textHuman.LoadString( IDS_STRING205 );
			mRaceCombo.AddString( textHuman );

			CString textElf;
			textElf.LoadString( IDS_STRING206 );
			mRaceCombo.AddString( textElf );

			mRaceCombo.SetCurSel( 0 );
		}

		// Ŭ����
		{
			mJobCombo.GetWindowRect( rect );
			mJobCombo.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );

			CString textFighter;
			textFighter.LoadString( IDS_STRING207 );
			mJobCombo.AddString( textFighter );

			CString textMage;
			textMage.LoadString( IDS_STRING208 );
			mJobCombo.AddString( textMage );

			CString textRogue;
			textRogue.LoadString( IDS_STRING209 );
			mJobCombo.AddString( textRogue );

			mJobCombo.SetCurSel( 0 );
		}

		// ����
		{
			mGenderCombo.GetWindowRect( rect );
			mGenderCombo.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );

			CString textMale;
			textMale.LoadString( IDS_STRING178 );
			mGenderCombo.AddString( textMale );

			CString textFemale;
			textFemale.LoadString( IDS_STRING177 );
			mGenderCombo.AddString( textFemale );

			mGenderCombo.SetCurSel( 0 );
		}

		// �Ӹ�
		{
			mHairCombo.GetWindowRect( rect );
			mHairCombo.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );

			CString textHead;
			textHead.LoadString( IDS_STRING210 );

			mHairCombo.AddString( textHead + "1" );
			mHairCombo.AddString( textHead + "2" );
			mHairCombo.AddString( textHead + "3" );
			mHairCombo.AddString( textHead + "4" );
			mHairCombo.AddString( textHead + "5" );
			mHairCombo.AddString( textHead + "6" );
			mHairCombo.AddString( textHead + "7" );
			mHairCombo.AddString( textHead + "8" );
			mHairCombo.AddString( textHead + "9" );
			mHairCombo.AddString( textHead + "10" );
			
			mHairCombo.SetCurSel( 0 );
		}

		// ��
		{
			mFaceCombo.GetWindowRect( rect );
			mFaceCombo.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );

			CString textFace;
			textFace.LoadString( IDS_STRING211 );

			mFaceCombo.AddString( textFace + "1" );
			mFaceCombo.AddString( textFace + "2" );
			mFaceCombo.AddString( textFace + "3" );
			mFaceCombo.AddString( textFace + "4" );
			mFaceCombo.AddString( textFace + "5" );
			mFaceCombo.AddString( textFace + "6" );
			mFaceCombo.AddString( textFace + "7" );
			mFaceCombo.AddString( textFace + "8" );
			mFaceCombo.AddString( textFace + "9" );
			mFaceCombo.AddString( textFace + "10" );

			mFaceCombo.SetCurSel( 0 );
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CUserCreateDialog::OnOK()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString name;
	mNameEdit.GetWindowText( name );

	if( name.IsEmpty() )
	{
		CString textEmptyValueExist;
		textEmptyValueExist.LoadString( IDS_STRING185 );

		MessageBox( textEmptyValueExist, "", MB_ICONERROR | MB_OK );
		return;
	}

	CDialog::OnOK();

	mRace	= mRaceCombo.GetCurSel();
	mJob	= mJobCombo.GetCurSel() + 1;
	mGender	= mGenderCombo.GetCurSel();
	mHair	= mHairCombo.GetCurSel();
	mFace	= mFaceCombo.GetCurSel();
	mName	= name;
}


int CUserCreateDialog::GetRace() const
{
	return mRace;
}


int CUserCreateDialog::GetJob() const
{
	return mJob;
}


int CUserCreateDialog::GetGender() const
{
	return mGender;
}


int CUserCreateDialog::GetHair() const
{
	return mHair;
}


int	CUserCreateDialog::GetFace() const
{
	return mFace;
}


const char* CUserCreateDialog::GetName() const
{
	return mName;
}
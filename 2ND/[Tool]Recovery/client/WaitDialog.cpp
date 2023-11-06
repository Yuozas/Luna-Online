// WaitDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "client.h"
#include "WaitDialog.h"
#include "PictureEx.h"
#include ".\waitdialog.h"


const int timerId = WM_USER + 12345;


// CWaitDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CWaitDialog, CDialog)
CWaitDialog::CWaitDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CWaitDialog::IDD, pParent)
	, mMaxSecond( 30 )
	, mIsWaiting( false )
{
	EnableAutomation();

}

CWaitDialog::~CWaitDialog()
{
}

void CWaitDialog::OnFinalRelease()
{
	// �ڵ�ȭ ��ü�� ���� ������ ������ �����Ǹ� OnFinalRelease��
	// ȣ��˴ϴ�. �⺻ Ŭ�������� �ڵ����� ��ü�� �����մϴ�.
	// �⺻ Ŭ������ ȣ���ϱ� ���� ��ü�� �ʿ��� �߰� ���� �۾���
	// �߰��Ͻʽÿ�.

	KillTimer( timerId );

	CDialog::OnFinalRelease();
}

void CWaitDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WAIT_GIF, m_Picture);
	DDX_Control(pDX, IDC_WAITING_TIME_STATIC, mTimeStatic);
	DDX_Control(pDX, IDCANCEL, mCancelButton);}


BEGIN_MESSAGE_MAP(CWaitDialog, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CWaitDialog, CDialog)
END_DISPATCH_MAP()

// ����: IID_IWaitDialog�� ���� ������ �߰��Ͽ�
// VBA���� ���� �������� ���ε��� �����մϴ�. 
// �� IID�� .IDL ���Ͽ� �ִ� dispinterface�� GUID�� ��ġ�ؾ� �մϴ�.

// {367CA3B6-B67D-45F7-A925-2ED77EA80882}
static const IID IID_IWaitDialog =
{ 0x367CA3B6, 0xB67D, 0x45F7, { 0xA9, 0x25, 0x2E, 0xD7, 0x7E, 0xA8, 0x8, 0x82 } };

BEGIN_INTERFACE_MAP(CWaitDialog, CDialog)
	INTERFACE_PART(CWaitDialog, IID_IWaitDialog, Dispatch)
END_INTERFACE_MAP()


// CWaitDialog �޽��� ó�����Դϴ�.


BOOL CWaitDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon( AfxGetApp()->LoadIcon( IDR_MAINFRAME ), FALSE );
	SetIcon( AfxGetApp()->LoadIcon( IDR_MAINFRAME ), TRUE );

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	if (m_Picture.Load( MAKEINTRESOURCE( IDR_WAIT_LOADING_GIF ), _T( "GIF" ) ) )
	{
		m_Picture.Draw();
	}

	// Ÿ�̸�
	{
		SetTimer( timerId, 1000, 0 );
	}

	mSecond = 0;

	mCancelButton.ShowWindow( SW_HIDE );

	mIsWaiting = true;

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CWaitDialog::OnTimer(UINT nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	if( nIDEvent == timerId )
	{
		if( mMaxSecond == ++mSecond )
		{
			mCancelButton.ShowWindow( SW_SHOW );

			KillTimer( timerId );

			mTimeStatic.SetWindowText( "" );

			mIsWaiting = false;
		}
		else
		{
			CString textHowManySeconedLimited;
			textHowManySeconedLimited.LoadString( IDS_STRING226 );

			CString text;
			text.Format( textHowManySeconedLimited, mMaxSecond - mSecond );

			mTimeStatic.SetWindowText( text );
		}
	}

	CDialog::OnTimer(nIDEvent);
}


LRESULT CWaitDialog::SendMessage( UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_CLOSE:
		{
			mIsWaiting = false;
			break;
		}
	}

	return CDialog::SendMessage( message, wParam, lParam );
}


void CWaitDialog::OnCancel()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	if( mIsWaiting )
	{
		return;
	}

  	CDialog::OnCancel();
}


void CWaitDialog::OnOK()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	if( mIsWaiting )
	{
		return;
	}

	CDialog::OnOK();
}

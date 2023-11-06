// NameLogDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "client.h"
#include "NameLogDialog.h"
#include ".\namelogdialog.h"


// CNameLogDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CNameLogDialog, CDialog)
CNameLogDialog::CNameLogDialog( CclientApp& application, CWnd* pParent /*=NULL*/)
	: CDialog(CNameLogDialog::IDD, pParent)
	, mApplication( application )
{
	EnableAutomation();

}

CNameLogDialog::~CNameLogDialog()
{
}

void CNameLogDialog::OnFinalRelease()
{
	// �ڵ�ȭ ��ü�� ���� ������ ������ �����Ǹ� OnFinalRelease��
	// ȣ��˴ϴ�. �⺻ Ŭ�������� �ڵ����� ��ü�� �����մϴ�.
	// �⺻ Ŭ������ ȣ���ϱ� ���� ��ü�� �ʿ��� �߰� ���� �۾���
	// �߰��Ͻʽÿ�.

	CDialog::OnFinalRelease();
}

void CNameLogDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NAME_LOG_LIST_CTRL, mResultListCtrl);
}


BEGIN_MESSAGE_MAP(CNameLogDialog, CDialog)
	ON_BN_CLICKED(IDC_NAME_LOG_CLIPBOARD_BUTTON, OnBnClickedNameLogClipboardButton)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CNameLogDialog, CDialog)
END_DISPATCH_MAP()

// ����: IID_INameLogDialog�� ���� ������ �߰��Ͽ�
// VBA���� ���� �������� ���ε��� �����մϴ�. 
// �� IID�� .IDL ���Ͽ� �ִ� dispinterface�� GUID�� ��ġ�ؾ� �մϴ�.

// {01297EB0-F503-41B5-A4B9-263443659C9B}
static const IID IID_INameLogDialog =
{ 0x1297EB0, 0xF503, 0x41B5, { 0xA4, 0xB9, 0x26, 0x34, 0x43, 0x65, 0x9C, 0x9B } };

BEGIN_INTERFACE_MAP(CNameLogDialog, CDialog)
	INTERFACE_PART(CNameLogDialog, IID_INameLogDialog, Dispatch)
END_INTERFACE_MAP()


// CNameLogDialog �޽��� ó�����Դϴ�.


INT_PTR CNameLogDialog::DoModal( DWORD serverIndex, DWORD playerIndex )
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	mPlayerIndex	= playerIndex;
	mServerIndex	= serverIndex;

	return CDialog::DoModal();
}



void CNameLogDialog::Parse( DWORD serverIndex, const MSGROOT* message )
{
	mServerIndex = serverIndex;

	switch( message->Protocol )
	{
	case MP_RM_NAME_LOG_GET_ACK:
		{
			const MSG_RM_NAME_LOG* m = ( MSG_RM_NAME_LOG* )message;

			for( DWORD i = 0; i < m->mSize; ++i )
			{
				const MSG_RM_NAME_LOG::Log& log = m->mLog[ i ];

				mLogMap.insert( std::make_pair( log.mIndex, log ) );
			}

			mResultListCtrl.DeleteAllItems();

			for(
				LogMap::const_iterator it = mLogMap.begin();
				mLogMap.end() != it;
				++it )
			{
				const MSG_RM_NAME_LOG::Log& log = it->second;

				const int row = mResultListCtrl.GetItemCount();

				mResultListCtrl.InsertItem( row, log.mDate, 0 );

				CString text;
				text.Format( "%s > %s", log.mPreviousName, log.mName );
				mResultListCtrl.SetItemText( row, 1, text );
			}

			if( mLogMap.empty() )
			{
				CString textThereIsNoResult;
				textThereIsNoResult.LoadString( IDS_STRING1 );

				MessageBox( textThereIsNoResult, "", MB_OK | MB_ICONINFORMATION );
			}

			break;
		}
	default:
		{
			ASSERT( 0 );
			break;
		}
	}
}


BOOL CNameLogDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	// initialize list ctrl
	{
		const DWORD addStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
		CRect rect;

		// ������ ����� ä���
		{
			int step = -1;

			mResultListCtrl.GetClientRect( rect );

			CString textDate;
			textDate.LoadString( IDS_STRING3 );
			mResultListCtrl.InsertColumn( ++step, textDate, LVCFMT_LEFT, int( rect.Width() * 0.4f ) );

			CString textName;
			textName.LoadString( IDS_STRING20 );
			mResultListCtrl.InsertColumn( ++step, textName, LVCFMT_LEFT, int( rect.Width() * 0.6f ) );

			mResultListCtrl.SetExtendedStyle( mResultListCtrl.GetExtendedStyle() | addStyle );
		}
	}		

	{
		MSGBASE message;
		{
			ZeroMemory( &message, sizeof( message ) );

			message.Category	= MP_RM_NAME_LOG;
			message.Protocol	= MP_RM_NAME_LOG_GET_SYN;
			message.dwObjectID	= mPlayerIndex;
		}	

		mApplication.Send( mServerIndex, message, sizeof( message ) );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CNameLogDialog::OnBnClickedNameLogClipboardButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	mApplication.SaveToClipBoard( mResultListCtrl );
}

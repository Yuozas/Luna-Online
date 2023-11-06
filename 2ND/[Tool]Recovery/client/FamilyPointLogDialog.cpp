// FamilyPointLogDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "client.h"
#include "FamilyPointLogDialog.h"
#include ".\familypointlogdialog.h"


// CFamilyPointLogDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CFamilyPointLogDialog, CDialog)
CFamilyPointLogDialog::CFamilyPointLogDialog( CclientApp& application, CWnd* pParent /*=NULL*/)
	: CDialog(CFamilyPointLogDialog::IDD, pParent)
	, mApplication( application )
{
	EnableAutomation();

}

CFamilyPointLogDialog::~CFamilyPointLogDialog()
{
}

void CFamilyPointLogDialog::OnFinalRelease()
{
	// �ڵ�ȭ ��ü�� ���� ������ ������ �����Ǹ� OnFinalRelease��
	// ȣ��˴ϴ�. �⺻ Ŭ�������� �ڵ����� ��ü�� �����մϴ�.
	// �⺻ Ŭ������ ȣ���ϱ� ���� ��ü�� �ʿ��� �߰� ���� �۾���
	// �߰��Ͻʽÿ�.

	CDialog::OnFinalRelease();
}

void CFamilyPointLogDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FAMILY_POINT_LOG_FIND_BUTTON, mFindButton);
	DDX_Control(pDX, IDC_FAMILY_POINT_LOG_STOP_BUTTON, mStopButton);
	DDX_Control(pDX, IDC_FAMILY_POINT_LOG_RESULT_STATIC, mResultStatic);
	DDX_Control(pDX, IDC_FAMILY_POINT_LOG_RESULT_PROGRESS, mResultProgressCtrl);
	DDX_Control(pDX, IDC_FAMILY_POINT_LOG_EXCEL_BUTTON, mExcelButton);
	DDX_Control(pDX, IDC_FAMILY_POINT_LOG_RESULT_LIST, mResultListCtrl);
	DDX_Control(pDX, IDC_FAMILY_POINT_LOG_BEGIN_DATE_CTRL, mBeginDateCtrl);
	DDX_Control(pDX, IDC_FAMILY_POINT_LOG_BEGIN_TIME_CTRL, mBeginTimeCtrl);
	DDX_Control(pDX, IDC_FAMILY_POINT_LOG_END_DATE_CTRL, mEndDateCtrl);
	DDX_Control(pDX, IDC_FAMILY_POINT_LOG_END_TIME_CTRL, mEndTimeCtrl);
}


BEGIN_MESSAGE_MAP(CFamilyPointLogDialog, CDialog)
	ON_BN_CLICKED(IDC_FAMILY_POINT_LOG_FIND_BUTTON, OnBnClickedFamilyPointLogFindButton)
	ON_BN_CLICKED(IDC_FAMILY_POINT_LOG_STOP_BUTTON, OnBnClickedFamilyPointLogStopButton)
	ON_BN_CLICKED(IDC_FAMILY_POINT_LOG_EXCEL_BUTTON, OnBnClickedFamilyPointLogExcelButton)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CFamilyPointLogDialog, CDialog)
END_DISPATCH_MAP()

// ����: IID_IFamilyPointLogDialog�� ���� ������ �߰��Ͽ�
// VBA���� ���� �������� ���ε��� �����մϴ�. 
// �� IID�� .IDL ���Ͽ� �ִ� dispinterface�� GUID�� ��ġ�ؾ� �մϴ�.

// {68CEA986-E6E4-4A17-A5E5-6B676B6FB682}
static const IID IID_IFamilyPointLogDialog =
{ 0x68CEA986, 0xE6E4, 0x4A17, { 0xA5, 0xE5, 0x6B, 0x67, 0x6B, 0x6F, 0xB6, 0x82 } };

BEGIN_INTERFACE_MAP(CFamilyPointLogDialog, CDialog)
	INTERFACE_PART(CFamilyPointLogDialog, IID_IFamilyPointLogDialog, Dispatch)
END_INTERFACE_MAP()


// CFamilyPointLogDialog �޽��� ó�����Դϴ�.

void CFamilyPointLogDialog::OnBnClickedFamilyPointLogFindButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	mLogIndexSet.clear();

	MSG_RM_LOG_REQUEST message;

	message.Category	= MP_RM_FAMILY_POINT_LOG;
	message.Protocol	= MP_RM_FAMILY_POINT_LOG_SIZE_SYN;
	message.dwObjectID	= mFamilyIndex;

	{
		CTime date;
		CTime time;

		mBeginDateCtrl.GetTime( date );
		mBeginTimeCtrl.GetTime( time );

		sprintf( message.mBeginDate, "%02d%02d%02d %02d:%02d:%02d",
			max( 0, date.GetYear() - 2000 ),
			date.GetMonth(),
			date.GetDay(),
			time.GetHour(),
			time.GetMinute(),
			time.GetSecond() );

		mEndDateCtrl.GetTime( date );
		mEndTimeCtrl.GetTime( time );

		sprintf( message.mEndDate, "%02d%02d%02d %02d:%02d:%02d",
			max( 0, date.GetYear() - 2000 ),
			date.GetMonth(),
			date.GetDay(),
			time.GetHour(),
			time.GetMinute(),
			time.GetSecond() );
	}

	mApplication.Send( mServerName, message, sizeof( message ) );
}

void CFamilyPointLogDialog::OnBnClickedFamilyPointLogStopButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	MSGROOT message;
	message.Category	= MP_RM_FAMILY_POINT_LOG;
	message.Protocol	= MP_RM_FAMILY_POINT_LOG_STOP_SYN;

	mApplication.Send( mServerName, message, sizeof( message ) );
}

void CFamilyPointLogDialog::OnBnClickedFamilyPointLogExcelButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CString caption;
	GetWindowText( caption );

	CString title;
	title.Format( "%s-%s", caption, mFamilyName );

	CString textLog;
	textLog.LoadString( IDS_STRING10 );

	CclientApp::SaveToExcel( title, textLog, mResultListCtrl );
}

BOOL CFamilyPointLogDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	// ����Ʈ ��Ʈ�� �ʱ�ȭ
	{
		const DWORD addStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
		CRect rect;

		// ������ ����� ä���
		{
			int step = -1;

			mResultListCtrl.GetClientRect( rect );

			CString textDate;
			textDate.LoadString( IDS_STRING3 );
			mResultListCtrl.InsertColumn( ++step, textDate, LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

			CString textType;
			textType.LoadString( IDS_STRING4 );
			mResultListCtrl.InsertColumn( ++step, textType, LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

			CString textPoint;
			textPoint.LoadString( IDS_STRING34 );
			mResultListCtrl.InsertColumn( ++step, textPoint, LVCFMT_LEFT, int( rect.Width() * 0.25f ) );
			
			CString textMemo;
			textMemo.LoadString( IDS_STRING17 );
			mResultListCtrl.InsertColumn( ++step, textMemo, LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

			mResultListCtrl.SetExtendedStyle( mResultListCtrl.GetExtendedStyle() | addStyle );
		}
	}

	// �޺��ڽ� �ʱ�ȭ
	{
	}

	// ��ư �ʱ�ȭ
	{
		mStopButton.EnableWindow( FALSE );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CFamilyPointLogDialog::Parse( DWORD serverIndex, const MSGROOT* message )
{
	mServerIndex = serverIndex;

	switch( message->Protocol )
	{
	case MP_RM_FAMILY_POINT_LOG_GET_ACK:
		{
			CString text;
			const MSG_RM_FAMILY_POINT_LOG* m = ( MSG_RM_FAMILY_POINT_LOG* )message;

			for( DWORD i = 0; i < m->mSize; ++i )
			{
				const MSG_RM_FAMILY_POINT_LOG::Log& data	= m->mLog[ i ];
				const DWORD							row		= mResultListCtrl.GetItemCount();
				int									step	= 0;

				//mResultListCtrl.InsertItem( row, GetLogType( data.mType ), 0 );
				mResultListCtrl.InsertItem( row, data.mDate, 0 );

				mResultListCtrl.SetItemText( row, ++step, mApplication.GetLogText( data.mType ) );

				text.Format( "%d", data.mPoint );
				mResultListCtrl.SetItemText( row, ++step, text );

				//text.Format( "%d", data.mMonsterExp );
				//mResultListCtrl.SetItemText( row, ++step, text );

				//text.Format( "%d", data.mLoginMember );
				//mResultListCtrl.SetItemText( row, ++step, text );
		
				mResultListCtrl.SetItemText( row, ++step, data.mMemo );
			}

			{
				int minRange;
				int maxRange;
				mResultProgressCtrl.GetRange( minRange, maxRange );

				mResultProgressCtrl.SetPos( mResultProgressCtrl.GetPos() + int( m->mSize ) );

				text.Format( "%d/%d", mResultProgressCtrl.GetPos(), maxRange );
				mResultStatic.SetWindowText( text );

				if( mResultListCtrl.GetItemCount() == maxRange )
				{
					mFindButton.EnableWindow( TRUE );
					mStopButton.EnableWindow( FALSE );
				}
			}

			break;
		}
	case MP_RM_FAMILY_POINT_LOG_SIZE_ACK:
		{
			const MSG_DWORD* m = ( MSG_DWORD* )message;

			const DWORD size = m->dwData;

			CString text;
			text.Format( "0/%d", size );

			mResultStatic.SetWindowText( text );
			mResultProgressCtrl.SetRange32( 0, size );
			mResultProgressCtrl.SetPos( 0 );
			mFindButton.EnableWindow( FALSE );
			mStopButton.EnableWindow( TRUE );

			mResultListCtrl.DeleteAllItems();
			break;
		}
	case MP_RM_FAMILY_POINT_LOG_SIZE_NACK:
		{
			CString textThereIsNoResult;
			textThereIsNoResult.LoadString( IDS_STRING1 );

			MessageBox( textThereIsNoResult, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_FAMILY_POINT_LOG_STOP_ACK:
		{
			mFindButton.EnableWindow( TRUE );
			mStopButton.EnableWindow( FALSE );

			CString textSearchWasStopped;
			textSearchWasStopped.LoadString( IDS_STRING2 );

			MessageBox( textSearchWasStopped, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_FAMILY_POINT_LOG_NACK_BY_AUTH:
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


INT_PTR CFamilyPointLogDialog::DoModal( const CString& serverName, const CString& familyName, DWORD familyIndex )
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	mServerName	= serverName;
	mFamilyName	= familyName;
	mFamilyIndex	= familyIndex;

	return CDialog::DoModal();
}


void CFamilyPointLogDialog::OnCancel()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	mResultListCtrl.DeleteAllItems();

	CDialog::OnCancel();
}

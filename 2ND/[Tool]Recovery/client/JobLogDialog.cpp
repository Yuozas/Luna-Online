// JobLogDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "client.h"
#include "JobLogDialog.h"
#include ".\joblogdialog.h"


// CJobLogDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CJobLogDialog, CDialog)
CJobLogDialog::CJobLogDialog( CclientApp& application, CWnd* pParent /*=NULL*/)
	: CDialog(CJobLogDialog::IDD, pParent)
	, mApplication( application )
{
	EnableAutomation();

}

CJobLogDialog::~CJobLogDialog()
{
}

void CJobLogDialog::OnFinalRelease()
{
	// �ڵ�ȭ ��ü�� ���� ������ ������ �����Ǹ� OnFinalRelease��
	// ȣ��˴ϴ�. �⺻ Ŭ�������� �ڵ����� ��ü�� �����մϴ�.
	// �⺻ Ŭ������ ȣ���ϱ� ���� ��ü�� �ʿ��� �߰� ���� �۾���
	// �߰��Ͻʽÿ�.

	CDialog::OnFinalRelease();
}

void CJobLogDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_JOB_LOG_FIND_BUTTON, mFindButton);
	DDX_Control(pDX, IDC_JOB_LOG_STOP_BUTTON, mStopButton);
	DDX_Control(pDX, IDC_JOB_LOG_RESULT_PROGRESS, mResultProgressCtrl);
	DDX_Control(pDX, IDC_JOB_LOG_EXCEL_BUTTON, mExcelButton);
	DDX_Control(pDX, IDC_JOB_LOG_RESULT_LIST, mResultListCtrl);
	DDX_Control(pDX, IDC_JOB_LOG_RESULT_STATIC, mResultStatic);
	DDX_Control(pDX, IDC_JOB_LOG_BEGIN_DATE_CTRL, mBeginDateCtrl);
	DDX_Control(pDX, IDC_JOB_LOG_BEGIN_TIME_CTRL, mBeginTimeCtrl);
	DDX_Control(pDX, IDC_JOB_LOG_END_DATE_CTRL, mEndDateCtrl);
	DDX_Control(pDX, IDC_JOB_LOG_END_TIME_CTRL, mEndTimeCtrl);
}


BEGIN_MESSAGE_MAP(CJobLogDialog, CDialog)
	ON_BN_CLICKED(IDC_JOB_LOG_FIND_BUTTON, OnBnClickedJobLogFindButton)
	ON_BN_CLICKED(IDC_JOB_LOG_STOP_BUTTON, OnBnClickedJobLogStopButton)
	ON_BN_CLICKED(IDC_JOB_LOG_EXCEL_BUTTON, OnBnClickedJobLogExcelButton)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CJobLogDialog, CDialog)
END_DISPATCH_MAP()

// ����: IID_IJobLogDialog�� ���� ������ �߰��Ͽ�
// VBA���� ���� �������� ���ε��� �����մϴ�. 
// �� IID�� .IDL ���Ͽ� �ִ� dispinterface�� GUID�� ��ġ�ؾ� �մϴ�.

// {823721EA-758F-4342-B20E-EFB21D4F2FD9}
static const IID IID_IJobLogDialog =
{ 0x823721EA, 0x758F, 0x4342, { 0xB2, 0xE, 0xEF, 0xB2, 0x1D, 0x4F, 0x2F, 0xD9 } };

BEGIN_INTERFACE_MAP(CJobLogDialog, CDialog)
	INTERFACE_PART(CJobLogDialog, IID_IJobLogDialog, Dispatch)
END_INTERFACE_MAP()


// CJobLogDialog �޽��� ó�����Դϴ�.

void CJobLogDialog::OnBnClickedJobLogFindButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	mLogIndexSet.clear();

	MSG_RM_LOG_REQUEST message;

	message.Category	= MP_RM_JOB_LOG;
	message.Protocol	= MP_RM_JOB_LOG_SIZE_SYN;
	message.dwObjectID	= mPlayerIndex;

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

void CJobLogDialog::OnBnClickedJobLogStopButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	MSGROOT message;
	message.Category	= MP_RM_JOB_LOG;
	message.Protocol	= MP_RM_JOB_LOG_STOP_SYN;

	mApplication.Send( mServerName, message, sizeof( message ) );
}

void CJobLogDialog::OnBnClickedJobLogExcelButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CString caption;
	GetWindowText( caption );

	CString title;
	title.Format( "%s-%s", caption, mPlayerName );

	CString textLog;
	textLog.LoadString( IDS_STRING10 );
	CclientApp::SaveToExcel( title, textLog,mResultListCtrl );
}

BOOL CJobLogDialog::OnInitDialog()
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
			mResultListCtrl.InsertColumn( ++step, textDate, LVCFMT_LEFT, int( rect.Width() * 0.32f ) );

			CString textJob;
			textJob.LoadString( IDS_STRING126 );
			mResultListCtrl.InsertColumn( ++step, textJob, LVCFMT_LEFT, int( rect.Width() * 0.32f ) );
			
			CString textMemo;
			textMemo.LoadString( IDS_STRING17 );
			mResultListCtrl.InsertColumn( ++step, textMemo, LVCFMT_LEFT, int( rect.Width() * 0.32f ) );

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



void CJobLogDialog::Parse( DWORD serverIndex, const MSGROOT* message )
{
	mServerIndex = serverIndex;

	switch( message->Protocol )
	{
	case MP_RM_JOB_LOG_GET_ACK:
		{
			CString text;
			const MSG_RM_JOB_LOG* m = ( MSG_RM_JOB_LOG* )message;

			for( DWORD i = 0; i < m->mSize; ++i )
			{
				const MSG_RM_JOB_LOG::Log&	data	= m->mLog[ i ];
				const DWORD					row		= mResultListCtrl.GetItemCount();
				int							step	= 0;

				const DWORD index = data.mClassIndex * 1000 + 100 + data.mJobGrade * 10 + data.mJobIndex;

				mResultListCtrl.InsertItem( row, data.mDate, 0 );

				mResultListCtrl.SetItemText( row, ++step, mApplication.GetJobName( index ) );

				//text.Format( "%d", data.mJobGrade );
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
	case MP_RM_JOB_LOG_SIZE_ACK:
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
	case MP_RM_JOB_LOG_SIZE_NACK:
		{
			CString textThereIsNoResult;
			textThereIsNoResult.LoadString( IDS_STRING1 );
			MessageBox( textThereIsNoResult, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_JOB_LOG_STOP_ACK:
		{
			mFindButton.EnableWindow( TRUE );
			mStopButton.EnableWindow( FALSE );

			CString textSearchWasStopped;
			textSearchWasStopped.LoadString( IDS_STRING2 );

			MessageBox( textSearchWasStopped, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_JOB_LOG_NACK_BY_AUTH:
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


INT_PTR CJobLogDialog::DoModal( const CString& serverName, const CString& playerName, DWORD playerIndex )
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	mServerName		= serverName;
	mPlayerName		= playerName;
	mPlayerIndex	= playerIndex;

	return CDialog::DoModal();
}


void CJobLogDialog::OnCancel()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	mResultListCtrl.DeleteAllItems();
	
	CDialog::OnCancel();
}

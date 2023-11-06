// StatLogDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "client.h"
#include "StatLogDialog.h"
#include ".\statlogdialog.h"


// CStatLogDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CStatLogDialog, CDialog)
CStatLogDialog::CStatLogDialog( CclientApp& application, CWnd* pParent /*=NULL*/)
	: CDialog(CStatLogDialog::IDD, pParent)
	, mApplication( application )
{
	EnableAutomation();

}

CStatLogDialog::~CStatLogDialog()
{
}

void CStatLogDialog::OnFinalRelease()
{
	// �ڵ�ȭ ��ü�� ���� ������ ������ �����Ǹ� OnFinalRelease��
	// ȣ��˴ϴ�. �⺻ Ŭ�������� �ڵ����� ��ü�� �����մϴ�.
	// �⺻ Ŭ������ ȣ���ϱ� ���� ��ü�� �ʿ��� �߰� ���� �۾���
	// �߰��Ͻʽÿ�.

	CDialog::OnFinalRelease();
}

void CStatLogDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STAT_LOG_FIND_BUTTON, mFindButton);
	DDX_Control(pDX, IDC_STAT_LOG_STOP_BUTTON, mStopButton);
	DDX_Control(pDX, IDC_STAT_LOG_RESULT_STATIC, mResultStatic);
	DDX_Control(pDX, IDC_STAT_LOG_RESULT_PROGRESS, mResultProgress);
	DDX_Control(pDX, IDC_STAT_LOG_EXCEL_BUTTON, mExcelButton);
	DDX_Control(pDX, IDC_STAT_LOG_RESULT_LIST, mResultListCtrl);
	DDX_Control(pDX, IDC_STAT_LOG_BEGIN_DATE_CTRL, mBeginDateCtrl);
	DDX_Control(pDX, IDC_STAT_LOG_BEGIN_TIME_CTRL, mBeginTimeCtrl);
	DDX_Control(pDX, IDC_STAT_LOG_END_DATE_CTRL, mEndTimeCtrl);
	DDX_Control(pDX, IDC_STAT_LOG_END_TIME_CTRL, mEndDateCtrl);
}


BEGIN_MESSAGE_MAP(CStatLogDialog, CDialog)
	ON_BN_CLICKED(IDC_STAT_LOG_FIND_BUTTON, OnBnClickedStatLogFindButton)
	ON_BN_CLICKED(IDC_STAT_LOG_STOP_BUTTON, OnBnClickedStatLogStopButton)
	ON_BN_CLICKED(IDC_STAT_LOG_EXCEL_BUTTON, OnBnClickedStatLogExcelButton)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CStatLogDialog, CDialog)
END_DISPATCH_MAP()

// ����: IID_IStatLogDialog�� ���� ������ �߰��Ͽ�
// VBA���� ���� �������� ���ε��� �����մϴ�. 
// �� IID�� .IDL ���Ͽ� �ִ� dispinterface�� GUID�� ��ġ�ؾ� �մϴ�.

// {C9121AF3-05D8-4DCF-B6C3-EF2D1FEF247F}
static const IID IID_IStatLogDialog =
{ 0xC9121AF3, 0x5D8, 0x4DCF, { 0xB6, 0xC3, 0xEF, 0x2D, 0x1F, 0xEF, 0x24, 0x7F } };

BEGIN_INTERFACE_MAP(CStatLogDialog, CDialog)
	INTERFACE_PART(CStatLogDialog, IID_IStatLogDialog, Dispatch)
END_INTERFACE_MAP()


// CStatLogDialog �޽��� ó�����Դϴ�.


void CStatLogDialog::Parse( DWORD serverIndex, const MSGROOT* message )
{
	mServerIndex = serverIndex;

	switch( message->Protocol )
	{
	case MP_RM_STAT_LOG_GET_ACK:
		{
			CString text;
			const MSG_RM_STAT_LOG* m = ( MSG_RM_STAT_LOG* )message;
			
			for( DWORD i = 0; i < m->mSize; ++i )
			{
				const MSG_RM_STAT_LOG::Log& data	= m->mLog[ i ];
				const DWORD					row		= mResultListCtrl.GetItemCount();
				int							step	= 0;

				if( mLogIndexSet.end() == mLogIndexSet.find( data.mLogIndex ) )
				{
					mLogIndexSet.insert( data.mLogIndex );
				}
				else
				{
					continue;
				}

				mResultListCtrl.InsertItem( row, data.mDate, 0 );
				
				text.Format( "%d", data.mLevel );
				mResultListCtrl.SetItemText( row, ++step, text );

				text.Format( "%d", data.mStrength );
				mResultListCtrl.SetItemText( row, ++step, text );

				text.Format( "%d", data.mDexterity );
				mResultListCtrl.SetItemText( row, ++step, text );

				text.Format( "%d", data.mVitality );
				mResultListCtrl.SetItemText( row, ++step, text );

				text.Format( "%d", data.mIntelligence );
				mResultListCtrl.SetItemText( row, ++step, text );

				text.Format( "%d", data.mWisdom );
				mResultListCtrl.SetItemText( row, ++step, text );

				text.Format( "%d", data.mExperience );
				mResultListCtrl.SetItemText( row, ++step, text );
			}

			{
				int minRange;
				int maxRange;
				mResultProgress.GetRange( minRange, maxRange );

				mResultProgress.SetPos( mResultProgress.GetPos() + int( m->mSize ) );

				text.Format( "%d/%d", mResultProgress.GetPos(), maxRange );
				mResultStatic.SetWindowText( text );

				if( mResultListCtrl.GetItemCount() == maxRange )
				{
					mFindButton.EnableWindow( TRUE );
					mStopButton.EnableWindow( FALSE );
				}
			}

			break;
		}
	case MP_RM_STAT_LOG_SIZE_ACK:
		{
			const MSG_DWORD* m = ( MSG_DWORD* )message;

			const DWORD size = m->dwData;

			CString text;
			text.Format( "0/%d", size );

			mResultStatic.SetWindowText( text );
			mResultProgress.SetRange32( 0, size );
			mResultProgress.SetPos( 0 );
			mFindButton.EnableWindow( FALSE );
			mStopButton.EnableWindow( TRUE );

			mResultListCtrl.DeleteAllItems();
			break;
		}
	case MP_RM_STAT_LOG_SIZE_NACK:
		{
			CString textThereIsNoResult;
			textThereIsNoResult.LoadString( IDS_STRING1 );

			MessageBox( textThereIsNoResult, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_STAT_LOG_STOP_ACK:
		{
			mFindButton.EnableWindow( TRUE );
			mStopButton.EnableWindow( FALSE );

			CString textSearchWasStopped;
			textSearchWasStopped.LoadString( IDS_STRING2 );

			MessageBox( textSearchWasStopped, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_STAT_LOG_NACK_BY_AUTH:
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


BOOL CStatLogDialog::OnInitDialog()
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
			mResultListCtrl.InsertColumn( ++step, textDate, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );

			CString textLevel;
			textLevel.LoadString( IDS_STRING5 );
			mResultListCtrl.InsertColumn( ++step, textLevel, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );

			CString textStrength;
			textStrength.LoadString( IDS_STRING70 );
			mResultListCtrl.InsertColumn( ++step, textStrength, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );

			CString textDexterity;
			textDexterity.LoadString( IDS_STRING71 );
			mResultListCtrl.InsertColumn( ++step, textDexterity, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );

			CString textVitality;
			textVitality.LoadString( IDS_STRING72 );
			mResultListCtrl.InsertColumn( ++step, textVitality, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );

			CString textIntelligence;
			textIntelligence.LoadString( IDS_STRING73 );
			mResultListCtrl.InsertColumn( ++step, textIntelligence, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );

			CString textWisdom;
			textWisdom.LoadString( IDS_STRING74 );
			mResultListCtrl.InsertColumn( ++step, textWisdom, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );

			CString textExperience;
			textExperience.LoadString( IDS_STRING9 );
			mResultListCtrl.InsertColumn( ++step, textExperience, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );
			
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


void CStatLogDialog::OnBnClickedStatLogFindButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	mLogIndexSet.clear();

	MSG_RM_STAT_LOG_REQUEST message;

	message.Category		= MP_RM_STAT_LOG;
	message.Protocol		= MP_RM_STAT_LOG_SIZE_SYN;
	message.mPlayerIndex	= mPlayerIndex;

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


void CStatLogDialog::OnBnClickedStatLogStopButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	MSGROOT message;
	message.Category	= MP_RM_STAT_LOG;
	message.Protocol	= MP_RM_STAT_LOG_STOP_SYN;

	mApplication.Send( mServerName, message, sizeof( message ) );
}


void CStatLogDialog::OnBnClickedStatLogExcelButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CString caption;
	GetWindowText( caption );

	CString title;
	title.Format( "%s-%s", caption, mPlayerName );

	CString textLog;
	textLog.LoadString( IDS_STRING10 );

	CclientApp::SaveToExcel( title, textLog, mResultListCtrl );
}


INT_PTR CStatLogDialog::DoModal( const CString& serverName, const CString& playerName, DWORD playerIndex )
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	mServerName		= serverName;
	mPlayerName		= playerName;
	mPlayerIndex	= playerIndex;

	return CDialog::DoModal();
}


void CStatLogDialog::OnCancel()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	mResultListCtrl.DeleteAllItems();

	CDialog::OnCancel();
}

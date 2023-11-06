// OperatorLogDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "client.h"
#include "OperatorLogDialog.h"
#include ".\operatorlogdialog.h"


// COperatorLogDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(COperatorLogDialog, CDialog)
COperatorLogDialog::COperatorLogDialog( CclientApp& application, CWnd* pParent /*=NULL*/)
	: CDialog(COperatorLogDialog::IDD, pParent)
	, mApplication( application )
	, mRequestTick( 0 )
{
	EnableAutomation();

}

COperatorLogDialog::~COperatorLogDialog()
{
}

void COperatorLogDialog::OnFinalRelease()
{
	// �ڵ�ȭ ��ü�� ���� ������ ������ �����Ǹ� OnFinalRelease��
	// ȣ��˴ϴ�. �⺻ Ŭ�������� �ڵ����� ��ü�� �����մϴ�.
	// �⺻ Ŭ������ ȣ���ϱ� ���� ��ü�� �ʿ��� �߰� ���� �۾���
	// �߰��Ͻʽÿ�.

	CDialog::OnFinalRelease();
}

void COperatorLogDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OPERATOR_LOG_BEGIN_DATE_CTRL, mBeginDateCtrl);
	DDX_Control(pDX, IDC_OPERATOR_LOG_END_DATE_CTRL, mEndDateCtrl);
	DDX_Control(pDX, IDC_OPERATOR_LOG_BEGIN_TIME_CTRL, mBeginTimeCtrl);
	DDX_Control(pDX, IDC_OPERATOR_LOG_END_TIME_CTRL, mEndTimeCtrl);
	DDX_Control(pDX, IDC_OPERATOR_LOG_RESULT_PROGRESS, mProgressCtrl);
	DDX_Control(pDX, IDC_OPERATOR_LOG_RESULT_LIST, mResultListCtrl);
	DDX_Control(pDX, IDC_OPERATOR_LOG_FIND_BUTTON, mFindButton);
	DDX_Control(pDX, IDC_OPERATOR_LOG_STOP_BUTTON, mStopButton);
	DDX_Control(pDX, IDC_OPERATOR_LOG_SERVER_COMBO, mServerCombo);
	DDX_Control(pDX, IDC_QUEST_LOG_RESULT_STATIC, mResultStatic);
	//DDX_Control(pDX, IDC_OPERATOR_LOG_ID_EDIT, mIdEdit);
	DDX_Control(pDX, IDC_OPERATOR_LOG_COMBO, mOperatorCombo);
}


BEGIN_MESSAGE_MAP(COperatorLogDialog, CDialog)
	ON_BN_CLICKED(IDC_OPERATOR_LOG_FIND_BUTTON, OnBnClickedRecoveryLogFindButton)
	ON_BN_CLICKED(IDC_OPERATOR_LOG_STOP_BUTTON, OnBnClickedRecoveryLogStopButton)
	ON_BN_CLICKED(IDC_OPERATOR_LOG_EXCEL_BUTTON, OnBnClickedRecoveryLogExcelButton)
	ON_BN_CLICKED(IDC_OPERATOR_LOG_CLIPBOARD_BUTTON, OnBnClickedRecoveryLogClipboardButton)
	ON_CBN_SELCHANGE(IDC_OPERATOR_LOG_COMBO, OnCbnSelchangeOperatorLogCombo)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(COperatorLogDialog, CDialog)
END_DISPATCH_MAP()

// ����: IID_IOperatorLogDialog�� ���� ������ �߰��Ͽ�
// VBA���� ���� �������� ���ε��� �����մϴ�. 
// �� IID�� .IDL ���Ͽ� �ִ� dispinterface�� GUID�� ��ġ�ؾ� �մϴ�.

// {77818545-E722-4148-A542-6C469CBAF5F4}
static const IID IID_IOperatorLogDialog =
{ 0x77818545, 0xE722, 0x4148, { 0xA5, 0x42, 0x6C, 0x46, 0x9C, 0xBA, 0xF5, 0xF4 } };

BEGIN_INTERFACE_MAP(COperatorLogDialog, CDialog)
	INTERFACE_PART(COperatorLogDialog, IID_IOperatorLogDialog, Dispatch)
END_INTERFACE_MAP()


// COperatorLogDialog �޽��� ó�����Դϴ�.

void COperatorLogDialog::OnBnClickedRecoveryLogFindButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	MSG_RM_LOG_REQUEST message;
	{
		message.Category		= MP_RM_OPERATOR_LOG;
		message.Protocol		= MP_RM_OPERATOR_LOG_SIZE_SYN;
		message.mRequestTick	= mRequestTick = GetTickCount();

		//SafeStrCpy( message.mOperatorId, text, sizeof( message.mOperatorId ) );
	}

	mLogMap.clear();
	
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

	CString serverName;
	{
		mServerCombo.GetLBText( mServerCombo.GetCurSel(), serverName );
	}

	mApplication.Send( serverName, message, sizeof( message ) );
}

void COperatorLogDialog::OnBnClickedRecoveryLogStopButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void COperatorLogDialog::OnBnClickedRecoveryLogExcelButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CString title;
	GetWindowText( title );

	CString textLog;
	textLog.LoadString( IDS_STRING10 );
	mApplication.SaveToExcel( title, textLog, mResultListCtrl );
}

void COperatorLogDialog::OnBnClickedRecoveryLogClipboardButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	mApplication.SaveToClipBoard( mResultListCtrl );
}


BOOL COperatorLogDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	// edit box
	//{
	//	mIdEdit.SetLimitText( MAX_NAME_LENGTH );
	//}

	// combo box
	{
		CRect rect;

		{
			mServerCombo.GetWindowRect( rect );
			mServerCombo.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );

			mApplication.SetServerComboBox( mServerCombo );
		}

		{
			mOperatorCombo.GetWindowRect( rect );
			mOperatorCombo.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );
		}
	}

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

			CString textOperator;
			textOperator.LoadString( IDS_STRING20 );
			mResultListCtrl.InsertColumn( ++step, textOperator, LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

			{
				CString textKey;
				textKey.LoadString( IDS_STRING151 );

				CString textValue;
				textValue.LoadString( IDS_STRING152 );

				mResultListCtrl.InsertColumn( ++step, textKey + "1", LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

				mResultListCtrl.InsertColumn( ++step, textValue + "1", LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

				mResultListCtrl.InsertColumn( ++step, textKey + "2", LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

				mResultListCtrl.InsertColumn( ++step, textValue + "2", LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

				mResultListCtrl.InsertColumn( ++step, textKey + "3", LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

				mResultListCtrl.InsertColumn( ++step, textValue + "3", LVCFMT_LEFT, int( rect.Width() * 0.25f ) );
			}
			

			mResultListCtrl.SetExtendedStyle( mResultListCtrl.GetExtendedStyle() | addStyle );
		}
	}

	// ��ư �ʱ�ȭ
	{
		mStopButton.EnableWindow( FALSE );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

INT_PTR COperatorLogDialog::DoModal()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	return CDialog::DoModal();
}


void COperatorLogDialog::Parse( DWORD serverIndex, const MSGROOT* message )
{
	switch( message->Protocol )
	{
	case MP_RM_OPERATOR_LOG_ACK:
		{
			const MSG_RM_OPERATOR_LOG* m = ( MSG_RM_OPERATOR_LOG* )message;

			if( mRequestTick != m->mRequestTick )
			{
				break;
			}

			mOperatorCombo.ResetContent();

			{
				const char* operatorAll = "...";

				if( 0 > mOperatorCombo.FindString( 0, operatorAll ) )
				{
					mOperatorCombo.AddString( operatorAll );
				}
			}

			for( size_t i = 0; i < m->mSize; ++i )
			{
				const MSG_RM_OPERATOR_LOG::Log& log = m->mLog[ i ];

				mLogMap[ log.mIndex ] = log;

				if( -1 < mOperatorCombo.FindString( 0, log.mOperator ) )
				{
					continue;
				}

				mOperatorCombo.AddString( log.mOperator );
			}

			mOperatorCombo.SetCurSel( 0 );

			OnCbnSelchangeOperatorLogCombo();
			//PutLog();

			{
				CString text;

				int minRange;
				int maxRange;
				mProgressCtrl.GetRange( minRange, maxRange );

				mProgressCtrl.SetPos( mProgressCtrl.GetPos() + int( m->mSize ) );

				text.Format( "%d/%d", mProgressCtrl.GetPos(), maxRange );
				mResultStatic.SetWindowText( text );

				if( mResultListCtrl.GetItemCount() == maxRange )
				{
					mFindButton.EnableWindow( TRUE );
					mStopButton.EnableWindow( FALSE );
				}
			}

			break;
		}
	case MP_RM_OPERATOR_LOG_SIZE_ACK:
		{
			const MSG_RM_LOG_SIZE* m = ( MSG_RM_LOG_SIZE* )message;

			if( mRequestTick != m->mRequestTick )
			{
				break;
			}

			CString text;
			text.Format( "0/%d", m->mSize );

			mResultStatic.SetWindowText( text );
			mProgressCtrl.SetRange32( 0, int( m->mSize ) );
			mProgressCtrl.SetPos( 0 );
			mFindButton.EnableWindow( FALSE );
			mStopButton.EnableWindow( TRUE );

			mResultListCtrl.DeleteAllItems();
			break;
		}
	case MP_RM_OPERATOR_LOG_SIZE_NACK:
		{
			CString textThereIsNoResult;
			textThereIsNoResult.LoadString( IDS_STRING1 );

			MessageBox( textThereIsNoResult, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_OPERATOR_LOG_NACK_BY_AUTH:
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


void COperatorLogDialog::OnCbnSelchangeOperatorLogCombo()
{
	mResultListCtrl.DeleteAllItems();

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString operatorId;
	{
		mOperatorCombo.GetLBText( mOperatorCombo.GetCurSel(), operatorId );
	}

	for(
		LogMap::const_iterator it = mLogMap.begin();
		mLogMap.end() != it;
		++it )
	{		
		const MSG_RM_OPERATOR_LOG::Log&	log	= it->second;

		if( operatorId	!= "..."	&&
			operatorId	!= log.mOperator )
		{
			continue;
		}

		int			step	= -1;
		const int	row		= mResultListCtrl.GetItemCount();

		mResultListCtrl.InsertItem( row, log.mDate, ++step );
		mResultListCtrl.SetItemText( row, ++step, mApplication.GetText( log.mType ) );
		mResultListCtrl.SetItemText( row, ++step, log.mOperator );
		mResultListCtrl.SetItemText( row, ++step, log.mKey1 );
		mResultListCtrl.SetItemText( row, ++step, log.mValue1 );
		mResultListCtrl.SetItemText( row, ++step, log.mKey2 );
		mResultListCtrl.SetItemText( row, ++step, log.mValue2 );
		mResultListCtrl.SetItemText( row, ++step, log.mKey3 );
		mResultListCtrl.SetItemText( row, ++step, log.mValue3 );
	}
}

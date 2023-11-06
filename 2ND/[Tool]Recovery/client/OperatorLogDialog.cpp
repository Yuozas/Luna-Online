// OperatorLogDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "client.h"
#include "OperatorLogDialog.h"
#include ".\operatorlogdialog.h"


// COperatorLogDialog 대화 상자입니다.

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
	// 자동화 개체에 대한 마지막 참조가 해제되면 OnFinalRelease가
	// 호출됩니다. 기본 클래스에서 자동으로 개체를 삭제합니다.
	// 기본 클래스를 호출하기 전에 개체에 필요한 추가 정리 작업을
	// 추가하십시오.

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

// 참고: IID_IOperatorLogDialog에 대한 지원을 추가하여
// VBA에서 형식 안정적인 바인딩을 지원합니다. 
// 이 IID는 .IDL 파일에 있는 dispinterface의 GUID와 일치해야 합니다.

// {77818545-E722-4148-A542-6C469CBAF5F4}
static const IID IID_IOperatorLogDialog =
{ 0x77818545, 0xE722, 0x4148, { 0xA5, 0x42, 0x6C, 0x46, 0x9C, 0xBA, 0xF5, 0xF4 } };

BEGIN_INTERFACE_MAP(COperatorLogDialog, CDialog)
	INTERFACE_PART(COperatorLogDialog, IID_IOperatorLogDialog, Dispatch)
END_INTERFACE_MAP()


// COperatorLogDialog 메시지 처리기입니다.

void COperatorLogDialog::OnBnClickedRecoveryLogFindButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void COperatorLogDialog::OnBnClickedRecoveryLogExcelButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString title;
	GetWindowText( title );

	CString textLog;
	textLog.LoadString( IDS_STRING10 );
	mApplication.SaveToExcel( title, textLog, mResultListCtrl );
}

void COperatorLogDialog::OnBnClickedRecoveryLogClipboardButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	mApplication.SaveToClipBoard( mResultListCtrl );
}


BOOL COperatorLogDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

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

	// 리스트 컨트롤 초기화
	{
		const DWORD addStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
		CRect rect;

		// 아이템 목록을 채운다
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

	// 버튼 초기화
	{
		mStopButton.EnableWindow( FALSE );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

INT_PTR COperatorLogDialog::DoModal()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

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

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

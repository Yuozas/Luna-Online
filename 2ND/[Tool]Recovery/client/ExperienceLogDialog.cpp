// ExperienceDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "client.h"
#include "ExperienceLogDialog.h"
#include ".\experiencelogdialog.h"


// CExperienceLogDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CExperienceLogDialog, CDialog)
CExperienceLogDialog::CExperienceLogDialog( CclientApp& application, CWnd* pParent /*=NULL*/)
	: CDialog(CExperienceLogDialog::IDD, pParent)
	, mApplication( application )
{
	EnableAutomation();

}

CExperienceLogDialog::~CExperienceLogDialog()
{
}

void CExperienceLogDialog::OnFinalRelease()
{
	// 자동화 개체에 대한 마지막 참조가 해제되면 OnFinalRelease가
	// 호출됩니다. 기본 클래스에서 자동으로 개체를 삭제합니다.
	// 기본 클래스를 호출하기 전에 개체에 필요한 추가 정리 작업을
	// 추가하십시오.

	CDialog::OnFinalRelease();
}

void CExperienceLogDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXP_RESULT_STATIC, mResultStatic);
	DDX_Control(pDX, IDC_EXP_RESULT_LIST, mResultListCtrl);
	DDX_Control(pDX, IDC_EXP_FIND_BUTTON, mFindButton);
	DDX_Control(pDX, IDC_EXP_STOP_BUTTON, mStopButton);
	DDX_Control(pDX, IDC_EXP_PROGRESS_CTRL, mResultProgressCtrl);
	DDX_Control(pDX, IDC_EXP_LOG_BEGIN_DATE_CTRL, mBeginDateCtrl);
	DDX_Control(pDX, IDC_EXP_LOG_BEGIN_TIME_CTRL, mBeginTimeCtrl);
	DDX_Control(pDX, IDC_EXP_LOG_END_DATE_CTRL, mEndDateCtrl);
	DDX_Control(pDX, IDC_EXP_LOG_END_TIME_CTRL, mEndTimeCtrl);
}


BEGIN_MESSAGE_MAP(CExperienceLogDialog, CDialog)
	ON_BN_CLICKED(IDC_EXP_FIND_BUTTON, OnBnClickedExpFindButton)
	ON_BN_CLICKED(IDC_EXP_STOP_BUTTON, OnBnClickedExpStopButton)
	ON_BN_CLICKED(IDC_EXP_EXCEL_BUTTON, OnBnClickedExpExcelButton)
	//ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CExperienceLogDialog, CDialog)
END_DISPATCH_MAP()

// 참고: IID_IExperienceDialog에 대한 지원을 추가하여
// VBA에서 형식 안정적인 바인딩을 지원합니다. 
// 이 IID는 .IDL 파일에 있는 dispinterface의 GUID와 일치해야 합니다.

// {B0D5C9C4-B895-4D14-89C2-A48618171986}
static const IID IID_IExperienceDialog =
{ 0xB0D5C9C4, 0xB895, 0x4D14, { 0x89, 0xC2, 0xA4, 0x86, 0x18, 0x17, 0x19, 0x86 } };

BEGIN_INTERFACE_MAP(CExperienceLogDialog, CDialog)
	INTERFACE_PART(CExperienceLogDialog, IID_IExperienceDialog, Dispatch)
END_INTERFACE_MAP()


// CExperienceLogDialog 메시지 처리기입니다.


void CExperienceLogDialog::Parse( DWORD serverIndex, const MSGROOT* message )
{
	mServerIndex = serverIndex;

	switch( message->Protocol )
	{
	case MP_RM_EXP_LOG_GET_SIZE_ACK:
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
	case MP_RM_EXP_LOG_GET_SIZE_NACK:
		{			
			CString textThereIsNoResult;

			textThereIsNoResult.LoadString( IDS_STRING1 );
			
			MessageBox( textThereIsNoResult, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_EXP_LOG_GET_ACK:
		{
			const MSG_RM_EXP_LOG* m = ( MSG_RM_EXP_LOG* )message;
			CString text;

			for( DWORD i = 0; i < m->mSize; ++i )
			{
				const MSG_RM_EXP_LOG::Log&	data	= m->mLog[ i ];
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

				text.Format( "%d", data.mType );
				mResultListCtrl.SetItemText( row, ++step, text );

				text.Format( "%d", data.mGrade );
				mResultListCtrl.SetItemText( row, ++step, text );

				text.Format( "%d", data.mVariation );
				mResultListCtrl.SetItemText( row, ++step, text );

				text.Format( "%d", data.mKillerKind );
				mResultListCtrl.SetItemText( row, ++step, text );

				text.Format( "%d", data.mKillerIndex );
				mResultListCtrl.SetItemText( row, ++step, text );

				text.Format( "%d", data.mExperience );
				mResultListCtrl.SetItemText( row, ++step, text );
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
	case MP_RM_EXP_LOG_GET_STOP_ACK:
		{
			mFindButton.EnableWindow( TRUE );
			mStopButton.EnableWindow( FALSE );

			CString textSearchWasStopped;
			textSearchWasStopped.LoadString( IDS_STRING2 );

			MessageBox( textSearchWasStopped, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_EXP_LOG_GET_STOP_NACK:
		{
			ASSERT( 0 );
			break;
		}
	default:
		{
			break;
		}
	}
}


void CExperienceLogDialog::OnBnClickedExpFindButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	mLogIndexSet.clear();

	MSG_RM_LOG_REQUEST message;
	{
		ZeroMemory( &message, sizeof( message ) );

		message.Category	= MP_RM_EXP_LOG;
		message.Protocol	= MP_RM_EXP_LOG_GET_SIZE_SYN;
		message.dwObjectID	= mPlayerIndex;
	}

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


void CExperienceLogDialog::OnBnClickedExpStopButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MSGROOT	message;

	message.Category	= MP_RM_EXP_LOG;
	message.Protocol	= MP_RM_EXP_LOG_GET_STOP_SYN;

	mApplication.Send( mServerIndex, message, sizeof( message ) );
}


BOOL CExperienceLogDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	// 리스트 컨트롤 초기화
	{
		const DWORD addStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
		CRect rect;

		{
			int step = -1;

			mResultListCtrl.GetWindowRect( rect );

			CString textDate;
			textDate.LoadString( IDS_STRING3 );
			mResultListCtrl.InsertColumn( ++step, textDate, LVCFMT_LEFT, int( rect.Width() * 0.29f ) );

			CString textCategory;
			textCategory.LoadString( IDS_STRING4 );
			mResultListCtrl.InsertColumn( ++step, textCategory, LVCFMT_LEFT, int( rect.Width() * 0.1f ) );

			CString textLevel;
			textLevel.LoadString( IDS_STRING5 );			
			mResultListCtrl.InsertColumn( ++step, textLevel, LVCFMT_LEFT, int( rect.Width() * 0.1f ) );

			CString textVariety;
			textVariety.LoadString( IDS_STRING6 );
			mResultListCtrl.InsertColumn( ++step, textVariety, LVCFMT_LEFT, int( rect.Width() * 0.1f ) );

			CString textKillerType;
			textKillerType.LoadString( IDS_STRING7 );
			mResultListCtrl.InsertColumn( ++step, textKillerType, LVCFMT_LEFT, int( rect.Width() * 0.1f ) );

			CString textKillerIndex;
			textKillerIndex.LoadString( IDS_STRING8 );
			mResultListCtrl.InsertColumn( ++step, textKillerIndex, LVCFMT_LEFT, int( rect.Width() * 0.1f ) );

			CString textExperience;
			textExperience.LoadString( IDS_STRING9 );
			mResultListCtrl.InsertColumn( ++step, textExperience, LVCFMT_LEFT, int( rect.Width() * 0.2f ) );
			
			mResultListCtrl.SetExtendedStyle( mResultListCtrl.GetExtendedStyle() | addStyle );
		}
	}

	// 콤보박스 초기화
	{
	}

	// 버튼 초기화
	{
		mStopButton.EnableWindow( FALSE );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
void CExperienceLogDialog::OnBnClickedExpExcelButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	CString caption;
	GetWindowText( caption );

	CString title;
	title.Format( "%s-%s", caption, mPlayerName );
	
	{
		CString textLog;
		textLog.LoadString( IDS_STRING10 );

		CclientApp::SaveToExcel( title, textLog, mResultListCtrl );
	}	
}


INT_PTR CExperienceLogDialog::DoModal( const CString& serverName, const CString& playerName, DWORD playerIndex )
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	mServerName		= serverName;
	mPlayerName		= playerName;
	mPlayerIndex	= playerIndex;

	return CDialog::DoModal();
}


//void CExperienceLogDialog::OnBnClickedCancel()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	OnCancel();
//}

void CExperienceLogDialog::OnCancel()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	mResultListCtrl.DeleteAllItems();

	CDialog::OnCancel();
}
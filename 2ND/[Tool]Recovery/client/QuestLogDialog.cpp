	// QuestLogDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "client.h"
#include "QuestLogDialog.h"
#include ".\questlogdialog.h"


// CQuestLogDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CQuestLogDialog, CDialog)
CQuestLogDialog::CQuestLogDialog( CclientApp& application, CWnd* pParent /*=NULL*/)
	: CDialog(CQuestLogDialog::IDD, pParent)
	, mApplication( application )
{
	EnableAutomation();

}

CQuestLogDialog::~CQuestLogDialog()
{
}

void CQuestLogDialog::OnFinalRelease()
{
	// 자동화 개체에 대한 마지막 참조가 해제되면 OnFinalRelease가
	// 호출됩니다. 기본 클래스에서 자동으로 개체를 삭제합니다.
	// 기본 클래스를 호출하기 전에 개체에 필요한 추가 정리 작업을
	// 추가하십시오.

	CDialog::OnFinalRelease();
}

void CQuestLogDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_QUEST_LOG_FIND_BUTTON, mFindButton);
	DDX_Control(pDX, IDC_QUEST_LOG_STOP_BUTTON, mStopButton);
	DDX_Control(pDX, IDC_QUEST_LOG_RESULT_STATIC, mResultStatic);
	DDX_Control(pDX, IDC_QUEST_LOG_RESULT_PROGRESS, mResultProgressCtrl);
	DDX_Control(pDX, IDC_QUEST_LOG_EXCEL_BUTTON, mExcelButton);
	DDX_Control(pDX, IDC_QUEST_LOG_RESULT_LIST, mResultListCtrl);
	DDX_Control(pDX, IDC_QUEST_LOG_BEGIN_DATE_CTRL, mBeginDateCtrl);
	DDX_Control(pDX, IDC_QUEST_LOG_BEGIN_TIME_CTRL, mBeginTimeCtrl);
	DDX_Control(pDX, IDC_QUEST_LOG_END_DATE_CTRL, mEndDateCtrl);
	DDX_Control(pDX, IDC_QUEST_LOG_END_TIME_CTRL, mEndTimeCtrl);
}


BEGIN_MESSAGE_MAP(CQuestLogDialog, CDialog)
	ON_BN_CLICKED(IDC_QUEST_LOG_FIND_BUTTON, OnBnClickedQuestLogFindButton)
	ON_BN_CLICKED(IDC_QUEST_LOG_STOP_BUTTON, OnBnClickedQuestLogStopButton)
	ON_BN_CLICKED(IDC_QUEST_LOG_EXCEL_BUTTON, OnBnClickedQuestLogExcelButton)
	ON_BN_CLICKED(IDC_QUEST_LOG_CLIPBOARD_BUTTON, OnBnClickedQuestLogClipboardButton)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CQuestLogDialog, CDialog)
END_DISPATCH_MAP()

// 참고: IID_IQuestLogDialog에 대한 지원을 추가하여
// VBA에서 형식 안정적인 바인딩을 지원합니다. 
// 이 IID는 .IDL 파일에 있는 dispinterface의 GUID와 일치해야 합니다.

// {F2EEA8AB-12A0-4E1F-A09D-58AA0A25E402}
static const IID IID_IQuestLogDialog =
{ 0xF2EEA8AB, 0x12A0, 0x4E1F, { 0xA0, 0x9D, 0x58, 0xAA, 0xA, 0x25, 0xE4, 0x2 } };

BEGIN_INTERFACE_MAP(CQuestLogDialog, CDialog)
	INTERFACE_PART(CQuestLogDialog, IID_IQuestLogDialog, Dispatch)
END_INTERFACE_MAP()


// CQuestLogDialog 메시지 처리기입니다.

BOOL CQuestLogDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
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

			CString textQuest;
			textQuest.LoadString( IDS_STRING190 );
			
			CString textIndex;
			textIndex.LoadString( IDS_STRING22 );

			CString textMain;
			textMain.LoadString( IDS_STRING191 );
			mResultListCtrl.InsertColumn( ++step, textQuest + " " + textMain + " " + textIndex, LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

			CString textSub;
			textSub.LoadString( IDS_STRING192 );
			mResultListCtrl.InsertColumn( ++step, textQuest + " " + textSub + " " + textIndex, LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

			CString textName;
			textName.LoadString( IDS_STRING20 );
			mResultListCtrl.InsertColumn( ++step, textName, LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

			CString textValue;
			textValue.LoadString( IDS_STRING152 );
			mResultListCtrl.InsertColumn( ++step, textValue + "1", LVCFMT_LEFT, int( rect.Width() * 0.25f ) );
			mResultListCtrl.InsertColumn( ++step, textValue + "2", LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

			CString textMemo;
			textMemo.LoadString( IDS_STRING17 );
			mResultListCtrl.InsertColumn( ++step, textMemo, LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

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

	//// 대화상자 제목
	//{
	//	CString text;
	//	text.Format( "%s: %s님의 퀘스트 로그",
	//		mApplication.GetServerName( mServerIndex ),
	//		mPlayerName );

	//	SetWindowText( text );
	//}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


INT_PTR CQuestLogDialog::DoModal( DWORD serverIndex, const CString& playerName, DWORD playerIndex )
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	mServerIndex	= serverIndex;
	mPlayerName		= playerName;
	mPlayerIndex	= playerIndex;

	return CDialog::DoModal();
}

void CQuestLogDialog::OnBnClickedQuestLogFindButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	mLogIndexSet.clear();

	MSG_RM_LOG_REQUEST message;

	message.Category	= MP_RM_QUEST_LOG;
	message.Protocol	= MP_RM_QUEST_LOG_SIZE_SYN;
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

	mApplication.Send( mServerIndex, message, sizeof( message ) );
}

void CQuestLogDialog::OnBnClickedQuestLogStopButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	MSGROOT message;
	message.Category	= MP_RM_QUEST_LOG;
	message.Protocol	= MP_RM_QUEST_LOG_STOP_SYN;

	mApplication.Send( mServerIndex, message, sizeof( message ) );
}

void CQuestLogDialog::OnBnClickedQuestLogExcelButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString caption;
	GetWindowText( caption );

	CString title;
	title.Format( "%s-%s", caption, mPlayerName );

	CString textLog;
	textLog.LoadString( IDS_STRING10 );
	CclientApp::SaveToExcel( title, textLog, mResultListCtrl );
}


void CQuestLogDialog::Parse( DWORD serverIndex, const MSGROOT* message )
{
	mServerIndex = serverIndex;

	switch( message->Protocol )
	{
	case MP_RM_QUEST_LOG_GET_ACK:
		{
			CString text;
			const MSG_RM_QUEST_LOG* m = ( MSG_RM_QUEST_LOG* )message;

			for( DWORD i = 0; i < m->mSize; ++i )
			{
				const MSG_RM_QUEST_LOG::Log& data = m->mLog[ i ];

				const DWORD row		= mResultListCtrl.GetItemCount();
				int			step	= 0;

				if( mLogIndexSet.end() == mLogIndexSet.find( data.mLogIndex ) )
				{
					mLogIndexSet.insert( data.mLogIndex );
				}
				else
				{
					continue;
				}

				mResultListCtrl.InsertItem( row, data.mDate, 0 );
				mResultListCtrl.SetItemText( row, ++step, mApplication.GetLogText( data.mType ) );

				text.Format( "%d", data.mMainQuestIndex );
				mResultListCtrl.SetItemText( row, ++step, text );

				text.Format( "%d", data.mSubQuestIndex );
				mResultListCtrl.SetItemText( row, ++step, text );

				mResultListCtrl.SetItemText( row, ++step, mApplication.GetQuestTitle( data.mMainQuestIndex, data.mSubQuestIndex ) );

				text.Format( "%d", data.mValue1 );
				mResultListCtrl.SetItemText( row, ++step, text );

				text.Format( "%d", data.mValue2 );
				mResultListCtrl.SetItemText( row, ++step, text );

				mResultListCtrl.SetItemText( row, ++step, data.mMemo );
			}

			{
				int minRange;
				int maxRange;
				mResultProgressCtrl.GetRange( minRange, maxRange );

				mResultProgressCtrl.SetPos( mResultProgressCtrl.GetPos() + int( m->mSize ) );

				text.Format( "%d/%d", mResultProgressCtrl.GetPos(), maxRange );
				mResultStatic.SetWindowText( text );

				if( mResultListCtrl.GetItemCount() >= maxRange )
				{
					mFindButton.EnableWindow( TRUE );
					mStopButton.EnableWindow( FALSE );
				}
			}

			break;
		}
	case MP_RM_QUEST_LOG_SIZE_ACK:
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
	case MP_RM_QUEST_LOG_SIZE_NACK:
		{
			CString textThereIsNoResult;
			textThereIsNoResult.LoadString( IDS_STRING1 );

			MessageBox( textThereIsNoResult, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_QUEST_LOG_STOP_ACK:
		{
			mFindButton.EnableWindow( TRUE );
			mStopButton.EnableWindow( FALSE );

			CString textSearchWasStopped;
			textSearchWasStopped.LoadString( IDS_STRING2 );

			MessageBox( textSearchWasStopped, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_QUEST_LOG_NACK_BY_AUTH:
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


void CQuestLogDialog::OnBnClickedQuestLogClipboardButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	mApplication.SaveToClipBoard( mResultListCtrl );
}

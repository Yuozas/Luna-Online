// NameLogDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "client.h"
#include "NameLogDialog.h"
#include ".\namelogdialog.h"


// CNameLogDialog 대화 상자입니다.

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
	// 자동화 개체에 대한 마지막 참조가 해제되면 OnFinalRelease가
	// 호출됩니다. 기본 클래스에서 자동으로 개체를 삭제합니다.
	// 기본 클래스를 호출하기 전에 개체에 필요한 추가 정리 작업을
	// 추가하십시오.

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

// 참고: IID_INameLogDialog에 대한 지원을 추가하여
// VBA에서 형식 안정적인 바인딩을 지원합니다. 
// 이 IID는 .IDL 파일에 있는 dispinterface의 GUID와 일치해야 합니다.

// {01297EB0-F503-41B5-A4B9-263443659C9B}
static const IID IID_INameLogDialog =
{ 0x1297EB0, 0xF503, 0x41B5, { 0xA4, 0xB9, 0x26, 0x34, 0x43, 0x65, 0x9C, 0x9B } };

BEGIN_INTERFACE_MAP(CNameLogDialog, CDialog)
	INTERFACE_PART(CNameLogDialog, IID_INameLogDialog, Dispatch)
END_INTERFACE_MAP()


// CNameLogDialog 메시지 처리기입니다.


INT_PTR CNameLogDialog::DoModal( DWORD serverIndex, DWORD playerIndex )
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

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

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	// initialize list ctrl
	{
		const DWORD addStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
		CRect rect;

		// 아이템 목록을 채운다
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
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CNameLogDialog::OnBnClickedNameLogClipboardButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	mApplication.SaveToClipBoard( mResultListCtrl );
}

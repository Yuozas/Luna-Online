// FamilyCropLogDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "client.h"
#include "FamilyCropLogDialog.h"
#include ".\familycroplogdialog.h"


// CFamilyCropLogDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CFamilyCropLogDialog, CDialog)
CFamilyCropLogDialog::CFamilyCropLogDialog( CclientApp& application, CWnd* pParent /*=NULL*/)
	: CDialog(CFamilyCropLogDialog::IDD, pParent)
	, mApplication( application )
{
	EnableAutomation();

}

CFamilyCropLogDialog::~CFamilyCropLogDialog()
{
}

void CFamilyCropLogDialog::OnFinalRelease()
{
	// �ڵ�ȭ ��ü�� ���� ������ ������ �����Ǹ� OnFinalRelease��
	// ȣ��˴ϴ�. �⺻ Ŭ�������� �ڵ����� ��ü�� �����մϴ�.
	// �⺻ Ŭ������ ȣ���ϱ� ���� ��ü�� �ʿ��� �߰� ���� �۾���
	// �߰��Ͻʽÿ�.

	CDialog::OnFinalRelease();
}

void CFamilyCropLogDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FAMILY_CROP_LOG_FIND_BUTTON, mFindButton);
	DDX_Control(pDX, IDC_FAMILY_CROP_LOG_STOP_BUTTON, mStopButton);
	DDX_Control(pDX, IDC_FAMILY_CROP_LOG_RESULT_STATIC, mResultStatic);
	DDX_Control(pDX, IDC_FAMILY_CROP_LOG_RESULT_PROGRESS, mResultProgressCtrl);
	DDX_Control(pDX, IDC_FAMILY_CROP_LOG_EXCEL_BUTTON, mExcelButton);
	DDX_Control(pDX, IDC_FAMILY_CROP_LOG_RESULT_LIST, mResultListCtrl);
	DDX_Control(pDX, IDC_FARM_CROP_LOG_BEGIN_DATE_CTRL, mBeginDateCtrl);
	DDX_Control(pDX, IDC_FARM_CROP_LOG_BEGIN_TIME_CTRL, mBeginTimeCtrl);
	DDX_Control(pDX, IDC_FARM_CROP_LOG_END_DATE_CTRL, mEndDateCtrl);
	DDX_Control(pDX, IDC_FARM_CROP_LOG_END_TIME_CTRL, mEndTimeCtrl);
}


BEGIN_MESSAGE_MAP(CFamilyCropLogDialog, CDialog)
	ON_BN_CLICKED(IDC_FAMILY_CROP_LOG_FIND_BUTTON, OnBnClickedFamilyCropLogFindButton)
	ON_BN_CLICKED(IDC_FAMILY_CROP_LOG_STOP_BUTTON, OnBnClickedFamilyCropLogStopButton)
	ON_BN_CLICKED(IDC_FAMILY_CROP_LOG_EXCEL_BUTTON, OnBnClickedFamilyCropLogExcelButton)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CFamilyCropLogDialog, CDialog)
END_DISPATCH_MAP()

// ����: IID_IFamilyCropLogDialog�� ���� ������ �߰��Ͽ�
// VBA���� ���� �������� ���ε��� �����մϴ�. 
// �� IID�� .IDL ���Ͽ� �ִ� dispinterface�� GUID�� ��ġ�ؾ� �մϴ�.

// {FF5EB3AA-3C7C-4A2E-B8B3-16B7C1B389E3}
static const IID IID_IFamilyCropLogDialog =
{ 0xFF5EB3AA, 0x3C7C, 0x4A2E, { 0xB8, 0xB3, 0x16, 0xB7, 0xC1, 0xB3, 0x89, 0xE3 } };

BEGIN_INTERFACE_MAP(CFamilyCropLogDialog, CDialog)
	INTERFACE_PART(CFamilyCropLogDialog, IID_IFamilyCropLogDialog, Dispatch)
END_INTERFACE_MAP()


// CFamilyCropLogDialog �޽��� ó�����Դϴ�.

void CFamilyCropLogDialog::OnBnClickedFamilyCropLogFindButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	mLogIndexSet.clear();

	MSG_RM_LOG_REQUEST message;

	message.Category	= MP_RM_FAMILY_CROP_LOG;
	message.Protocol	= MP_RM_FAMILY_CROP_LOG_SIZE_SYN;
	message.dwObjectID	= mFarmIndex;

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

void CFamilyCropLogDialog::OnBnClickedFamilyCropLogStopButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	MSGROOT message;
	message.Category	= MP_RM_FAMILY_CROP_LOG;
	message.Protocol	= MP_RM_FAMILY_CROP_LOG_STOP_SYN;

	mApplication.Send( mServerIndex, message, sizeof( message ) );
}

void CFamilyCropLogDialog::OnBnClickedFamilyCropLogExcelButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CString caption;
	GetWindowText( caption );

	CString title;
	title.Format( "%s-%s", caption, mFarmName );

	{
		CString textLog;
		textLog.LoadString( IDS_STRING10 );

		CclientApp::SaveToExcel( title, textLog, mResultListCtrl );
	}
}

BOOL CFamilyCropLogDialog::OnInitDialog()
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

			CString textCategory;
			textCategory.LoadString( IDS_STRING4 );
			mResultListCtrl.InsertColumn( ++step, textCategory, LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

			CString textFarmIndex;
			textFarmIndex.LoadString( IDS_STRING11 );
			mResultListCtrl.InsertColumn( ++step, textFarmIndex, LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

			CString textOwner;
			textOwner.LoadString( IDS_STRING24 );
			mResultListCtrl.InsertColumn( ++step, textOwner, LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

			CString textCropDbIndex;				
			textCropDbIndex.LoadString( IDS_STRING27 );
			mResultListCtrl.InsertColumn( ++step, textCropDbIndex, LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

			CString textName;
			textName.LoadString( IDS_STRING20 );
			mResultListCtrl.InsertColumn( ++step, textName, LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

			CString textLife;
			textLife.LoadString( IDS_STRING15 );
			mResultListCtrl.InsertColumn( ++step, textLife, LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

			CString textQualityy;
			textQualityy.LoadString( IDS_STRING16 );
			mResultListCtrl.InsertColumn( ++step, textQualityy, LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

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


void CFamilyCropLogDialog::Parse( DWORD serverIndex, const MSGROOT* message )
{
	mServerIndex = serverIndex;

	switch( message->Protocol )
	{
	case MP_RM_FAMILY_CROP_LOG_GET_ACK:
		{
			CString text;
			const MSG_RM_FAMILY_CROP_LOG* m = ( MSG_RM_FAMILY_CROP_LOG* )message;

			for( DWORD i = 0; i < m->mSize; ++i )
			{
				const MSG_RM_FAMILY_CROP_LOG::Log&	data	= m->mLog[ i ];
				const DWORD							row		= mResultListCtrl.GetItemCount();
				int									step	= 0;

				//mResultListCtrl.InsertItem( row, GetLogType( data.mType ), 0 );

				mResultListCtrl.InsertItem( row, data.mDate, 0 );

				mResultListCtrl.SetItemText( row, ++step, mApplication.GetLogText( data.mType ) );

				text.Format( "%d", data.mFarmIndex );
				mResultListCtrl.SetItemText( row, ++step, text );

				text.Format( "%d", data.mOwnerIndex );
				mResultListCtrl.SetItemText( row, ++step, text );

				text.Format( "%d", data.mCropIndex );
				mResultListCtrl.SetItemText( row, ++step, text );

				mResultListCtrl.SetItemText( row, ++step, mApplication.GetCropname( data.mCropKind, data.mCropStep ) );

				text.Format( "%d", data.mCropLife );
				mResultListCtrl.SetItemText( row, ++step, text );
				
				text.Format( "%d", data.mCropSeedGrade );
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

				if( mResultListCtrl.GetItemCount() == maxRange )
				{
					mFindButton.EnableWindow( TRUE );
					mStopButton.EnableWindow( FALSE );
				}
			}

			break;
		}
	case MP_RM_FAMILY_CROP_LOG_SIZE_ACK:
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
	case MP_RM_FAMILY_CROP_LOG_SIZE_NACK:
		{
			CString textThereIsNoResult;
			textThereIsNoResult.LoadString( IDS_STRING1 );

			MessageBox( textThereIsNoResult, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_FAMILY_CROP_LOG_STOP_ACK:
		{
			mFindButton.EnableWindow( TRUE );
			mStopButton.EnableWindow( FALSE );

			CString textSearchWasStopped;
			textSearchWasStopped.LoadString( IDS_STRING2 );

			MessageBox( textSearchWasStopped, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_FAMILY_CROP_LOG_NACK_BY_AUTH:
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


INT_PTR CFamilyCropLogDialog::DoModal( DWORD serverIndex, const CString& farmName, DWORD farmIndex )
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	mServerIndex	= serverIndex;
	mFarmName		= farmName;
	mFarmIndex		= farmIndex;

	return CDialog::DoModal();
}


void CFamilyCropLogDialog::OnCancel()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	mResultListCtrl.DeleteAllItems();

	CDialog::OnCancel();
}

// ItemOptionLogDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "client.h"
#include "ItemOptionLogDialog.h"
#include ".\itemoptionlogdialog.h"


// CItemOptionLogDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CItemOptionLogDialog, CDialog)
CItemOptionLogDialog::CItemOptionLogDialog( CclientApp& application, CWnd* pParent /*=NULL*/)
	: CDialog(CItemOptionLogDialog::IDD, pParent)
	, mApplication( application )
{
	EnableAutomation();

}

CItemOptionLogDialog::~CItemOptionLogDialog()
{
}

void CItemOptionLogDialog::OnFinalRelease()
{
	// 자동화 개체에 대한 마지막 참조가 해제되면 OnFinalRelease가
	// 호출됩니다. 기본 클래스에서 자동으로 개체를 삭제합니다.
	// 기본 클래스를 호출하기 전에 개체에 필요한 추가 정리 작업을
	// 추가하십시오.

	CDialog::OnFinalRelease();
}

void CItemOptionLogDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ITEM_OPTION_LOG_FIND_BUTTON, mFindButton);
	DDX_Control(pDX, IDC_ITEM_OPTION_LOG_STOP_BUTTON, mStopButton);
	DDX_Control(pDX, IDC_ITEM_OPTION_LOG_RESULT_PROGRESS, mResultProgressCtrl);
	DDX_Control(pDX, IDC_ITEM_OPTION_LOG_RESULT_STATIC, mResultStatic);
	DDX_Control(pDX, IDC_ITEM_OPTION_LOG_EXCEL_BUTTON, mExcelButton);
	DDX_Control(pDX, IDC_ITEM_OPTION_LOG_RESULT_LIST, mResultListCtrl);
	DDX_Control(pDX, IDC_ITEM_OPTION_LOG_BEGIN_DATE_CTRL, mBeginDateCtrl);
	DDX_Control(pDX, IDC_ITEM_OPTION_LOG_END_DATE_CTRL, mEndDateCtrl);
	DDX_Control(pDX, IDC_ITEM_OPTION_LOG_BEGIN_TIME_CTRL, mBeginTimeCtrl);
	DDX_Control(pDX, IDC_ITEM_OPTION_LOG_END_TIME_CTRL, mEndTimeCtrl);
	DDX_Control(pDX, IDC_ITEM_OPTION_LOG_DB_IDX_EDIT, mItemDbIndexEdit);
	DDX_Control(pDX, IDC_ITEM_OPTION_LOG_SERVER_COMBO, mServerCombo);
}


BEGIN_MESSAGE_MAP(CItemOptionLogDialog, CDialog)
	ON_BN_CLICKED(IDC_ITEM_OPTION_LOG_FIND_BUTTON, OnBnClickedItemOptionLogFindButton)
	ON_BN_CLICKED(IDC_ITEM_OPTION_LOG_STOP_BUTTON, OnBnClickedItemOptionLogStopButton)
	ON_BN_CLICKED(IDC_ITEM_OPTION_LOG_EXCEL_BUTTON, OnBnClickedItemOptionLogExcelButton)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CItemOptionLogDialog, CDialog)
END_DISPATCH_MAP()

// 참고: IID_IItemOptionLogDialog에 대한 지원을 추가하여
// VBA에서 형식 안정적인 바인딩을 지원합니다. 
// 이 IID는 .IDL 파일에 있는 dispinterface의 GUID와 일치해야 합니다.

// {36114F63-8E4A-42DA-A8B3-285D806A50B8}
static const IID IID_IItemOptionLogDialog =
{ 0x36114F63, 0x8E4A, 0x42DA, { 0xA8, 0xB3, 0x28, 0x5D, 0x80, 0x6A, 0x50, 0xB8 } };

BEGIN_INTERFACE_MAP(CItemOptionLogDialog, CDialog)
	INTERFACE_PART(CItemOptionLogDialog, IID_IItemOptionLogDialog, Dispatch)
END_INTERFACE_MAP()


// CItemOptionLogDialog 메시지 처리기입니다.

void CItemOptionLogDialog::OnBnClickedItemOptionLogFindButton()
{
	CString itemDbIndex;
	mItemDbIndexEdit.GetWindowText( itemDbIndex );

	if( ! atoi( itemDbIndex ) )
	{
		return;
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	mLogIndexSet.clear();

	MSG_RM_ITEM_OPTION_LOG_REQUEST message;

	message.Category		= MP_RM_ITEM_OPTION_LOG;
	message.Protocol		= MP_RM_ITEM_OPTION_LOG_SIZE_SYN;
	message.mItemDbIndex	= atoi( itemDbIndex );

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
	mServerCombo.GetLBText( mServerCombo.GetCurSel(), serverName );

	mApplication.Send(
		serverName,
		message,
		sizeof( message ) );
}

void CItemOptionLogDialog::OnBnClickedItemOptionLogStopButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	MSGROOT message;
	message.Category	= MP_RM_ITEM_OPTION_LOG;
	message.Protocol	= MP_RM_ITEM_OPTION_LOG_STOP_SYN;

	mApplication.Send( mServerIndex, message, sizeof( message ) );
}

void CItemOptionLogDialog::OnBnClickedItemOptionLogExcelButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	CString caption;
	GetWindowText( caption );

	CString title;
	title.Format( "%s-%s", caption, mItemName );
	
	CString textLog;
	textLog.LoadString( IDS_STRING10 );

	CclientApp::SaveToExcel( title, textLog, mResultListCtrl );
}


BOOL CItemOptionLogDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	// 리스트 컨트롤 초기화
	{
		const DWORD addStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
		CRect rect;

		// 아이템 목록을 채운다
		{
			int column = -1;

			mResultListCtrl.GetClientRect( rect );

			CString textDate;
			textDate.LoadString( IDS_STRING3 );
			mResultListCtrl.InsertColumn( ++column, textDate, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );

			CString textType;
			textType.LoadString( IDS_STRING4 );
			mResultListCtrl.InsertColumn( ++column, textType, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );

			CString textItemDbIndex;
			textItemDbIndex.LoadString( IDS_STRING13 );
			mResultListCtrl.InsertColumn( ++column, textItemDbIndex, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );
			
			CString textStrength;
			textStrength.LoadString( IDS_STRING70 );

			CString textDexterity;
			textDexterity.LoadString( IDS_STRING71 );

			CString textVitality;
			textVitality.LoadString( IDS_STRING72 );

			CString textIntelligence;
			textIntelligence.LoadString( IDS_STRING73 );

			CString textWisdom;
			textWisdom.LoadString( IDS_STRING74 );

			CString textLife;
			textLife.LoadString( IDS_STRING75 );

			CString textMana;
			textMana.LoadString( IDS_STRING76 );

			CString textLifeRecovery;
			textLifeRecovery.LoadString( IDS_STRING77 );

			CString textManaRecovery;
			textManaRecovery.LoadString( IDS_STRING78 );

			CString textPhysicalAttack;
			textPhysicalAttack.LoadString( IDS_STRING79 );

			CString textPhysicalDefence;
			textPhysicalDefence.LoadString( IDS_STRING80 );

			CString textMagicalAttack;
			textMagicalAttack.LoadString( IDS_STRING81 );

			CString textMagicalDefence;
			textMagicalDefence.LoadString( IDS_STRING82 );

			CString textMoveSpeed;
			textMoveSpeed.LoadString( IDS_STRING83 );

			CString textEvade;
			textEvade.LoadString( IDS_STRING84 );

			CString textAccuracy;
			textAccuracy.LoadString( IDS_STRING85 );

			CString textCriticalDamage;
			textCriticalDamage.LoadString( IDS_STRING86 );

			CString textCriticalRate;
			textCriticalRate.LoadString( IDS_STRING87 );
			
			CString textReinforce;
			textReinforce.LoadString( IDS_STRING89 );


			mResultListCtrl.InsertColumn( ++column, textReinforce + ": " + textStrength, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );
			mResultListCtrl.InsertColumn( ++column, textReinforce + ": " + textDexterity, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );
			mResultListCtrl.InsertColumn( ++column, textReinforce + ": " + textVitality, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );
			mResultListCtrl.InsertColumn( ++column, textReinforce + ": " + textIntelligence, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );
			mResultListCtrl.InsertColumn( ++column, textReinforce + ": " + textWisdom, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );
			mResultListCtrl.InsertColumn( ++column, textReinforce + ": " + textLife, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );
			mResultListCtrl.InsertColumn( ++column, textReinforce + ": " + textMana, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );
			mResultListCtrl.InsertColumn( ++column, textReinforce + ": " + textManaRecovery, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );
			mResultListCtrl.InsertColumn( ++column, textReinforce + ": " + textLifeRecovery, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );
			mResultListCtrl.InsertColumn( ++column, textReinforce + ": " + textPhysicalAttack, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );
			mResultListCtrl.InsertColumn( ++column, textReinforce + ": " + textPhysicalDefence, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );
			mResultListCtrl.InsertColumn( ++column, textReinforce + ": " + textMagicalAttack, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );
			mResultListCtrl.InsertColumn( ++column, textReinforce + ": " + textMagicalDefence, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );
			mResultListCtrl.InsertColumn( ++column, textReinforce + ": " + textCriticalDamage, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );
			mResultListCtrl.InsertColumn( ++column, textReinforce + ": " + textCriticalRate, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );
			mResultListCtrl.InsertColumn( ++column, textReinforce + ": " + textMoveSpeed, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );
			mResultListCtrl.InsertColumn( ++column, textReinforce + ": " + textEvade, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );
			mResultListCtrl.InsertColumn( ++column, textReinforce + ": " + textAccuracy, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );

            CString textMix;
			textMix.LoadString( IDS_STRING90 );

			mResultListCtrl.InsertColumn( ++column, textMix + ": " + textStrength, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );
			mResultListCtrl.InsertColumn( ++column, textMix + ": " + textIntelligence, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );
			mResultListCtrl.InsertColumn( ++column, textMix + ": " + textDexterity, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );
			mResultListCtrl.InsertColumn( ++column, textMix + ": " + textWisdom, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );
			mResultListCtrl.InsertColumn( ++column, textMix + ": " + textVitality, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );

			CString textEnchant;
			textEnchant.LoadString( IDS_STRING91 );

			CString textLevel;
			textLevel.LoadString( IDS_STRING5 );

			mResultListCtrl.InsertColumn( ++column, textEnchant + ": " + textLevel, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );
			
			CString textMemo;
			textMemo.LoadString( IDS_STRING17 );
			mResultListCtrl.InsertColumn( ++column, textMemo, LVCFMT_LEFT, int( rect.Width() * 0.15f ) );

			mResultListCtrl.SetExtendedStyle( mResultListCtrl.GetExtendedStyle() | addStyle );
		}
	}

	// 콤보박스 초기화
	{
		mApplication.SetServerComboBox( mServerCombo );
	}

	// 버튼 초기화
	{
		mStopButton.EnableWindow( FALSE );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CItemOptionLogDialog::Parse( DWORD serverIndex, const MSGROOT* message )
{
	mServerIndex = serverIndex;

	switch( message->Protocol )
	{
	case MP_RM_ITEM_OPTION_LOG_GET_ACK:
		{
			CString text;
			const MSG_RM_ITEM_OPTION_LOG* m = ( MSG_RM_ITEM_OPTION_LOG* )message;

			for( DWORD i = 0; i < m->mSize; ++i )
			{
				const MSG_RM_ITEM_OPTION_LOG::Log& data = m->mLog[ i ];

				const DWORD row		= mResultListCtrl.GetItemCount();
				int			column	= -1;

				if( mLogIndexSet.end() == mLogIndexSet.find( data.mLogIndex ) )
				{
					mLogIndexSet.insert( data.mLogIndex );
				}
				else
				{
					continue;
				}

				mResultListCtrl.InsertItem( row, data.mDate, ++column );

				mResultListCtrl.SetItemText( row, ++column, mApplication.GetLogText( data.mType ) );

				text.Format( "%d", data.mItemDbIndex );
				mResultListCtrl.SetItemText( row, ++column, text );

				{
					const ITEM_OPTION::Reinforce& reinforce = data.mOption.mReinforce;

					text.Format( "%d", reinforce.mStrength );
					mResultListCtrl.SetItemText( row, ++column, text );

					text.Format( "%d", reinforce.mDexterity );
					mResultListCtrl.SetItemText( row, ++column, text );

					text.Format( "%d", reinforce.mVitality );
					mResultListCtrl.SetItemText( row, ++column, text );

					text.Format( "%d", reinforce.mIntelligence );
					mResultListCtrl.SetItemText( row, ++column, text );

					text.Format( "%d", reinforce.mWisdom );
					mResultListCtrl.SetItemText( row, ++column, text );

					text.Format( "%d", reinforce.mLife );
					mResultListCtrl.SetItemText( row, ++column, text );

					text.Format( "%d", reinforce.mMana );
					mResultListCtrl.SetItemText( row, ++column, text );

					text.Format( "%d", reinforce.mManaRecovery );
					mResultListCtrl.SetItemText( row, ++column, text );

					text.Format( "%d", reinforce.mLifeRecovery );
					mResultListCtrl.SetItemText( row, ++column, text );

					text.Format( "%d", reinforce.mPhysicAttack );
					mResultListCtrl.SetItemText( row, ++column, text );

					text.Format( "%d", reinforce.mPhysicDefence );
					mResultListCtrl.SetItemText( row, ++column, text );

					text.Format( "%d", reinforce.mMagicAttack );
					mResultListCtrl.SetItemText( row, ++column, text );

					text.Format( "%d", reinforce.mMagicDefence );
					mResultListCtrl.SetItemText( row, ++column, text );

					text.Format( "%d", reinforce.mCriticalRate );
					mResultListCtrl.SetItemText( row, ++column, text );

					text.Format( "%d", reinforce.mCriticalDamage );
					mResultListCtrl.SetItemText( row, ++column, text );

					text.Format( "%d", reinforce.mMoveSpeed );
					mResultListCtrl.SetItemText( row, ++column, text );

					text.Format( "%d", reinforce.mEvade );
					mResultListCtrl.SetItemText( row, ++column, text );

					text.Format( "%d", reinforce.mAccuracy );
					mResultListCtrl.SetItemText( row, ++column, text );
				}
				
				{
					const ITEM_OPTION::Mix& mix = data.mOption.mMix;

					text.Format( "%d", mix.mStrength );
					mResultListCtrl.SetItemText( row, ++column, text );

					text.Format( "%d", mix.mIntelligence );
					mResultListCtrl.SetItemText( row, ++column, text );

					text.Format( "%d", mix.mDexterity );
					mResultListCtrl.SetItemText( row, ++column, text );

					text.Format( "%d", mix.mWisdom );
					mResultListCtrl.SetItemText( row, ++column, text );

					text.Format( "%d", mix.mVitality );
					mResultListCtrl.SetItemText( row, ++column, text );
				}

				{
					const ITEM_OPTION::Enchant& enchant = data.mOption.mEnchant;

					text.Format( "%d", enchant.mLevel );
					mResultListCtrl.SetItemText( row, ++column, text );
				}

				mResultListCtrl.SetItemText( row, ++column, data.mMemo );
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
	case MP_RM_ITEM_OPTION_LOG_SIZE_ACK:
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
	case MP_RM_ITEM_OPTION_LOG_SIZE_NACK:
		{
			CString textThereIsNoResult;
			textThereIsNoResult.LoadString( IDS_STRING1 );

			MessageBox( textThereIsNoResult, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_ITEM_OPTION_LOG_STOP_ACK:
		{
			mFindButton.EnableWindow( TRUE );
			mStopButton.EnableWindow( FALSE );

			CString textSearchWasStopped;
			textSearchWasStopped.LoadString( IDS_STRING2 );

			MessageBox( textSearchWasStopped, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_ITEM_OPTION_LOG_NACK_BY_AUTH:
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


//INT_PTR CItemOptionLogDialog::DoModal( const CString& serverName, const CString& itemName, DWORD itemDbIndex )
//{
//	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
//
//	mServerIndex		= serverName;
//	mItemName		= itemName;
//	mItemDbIndex	= itemDbIndex;
//
//	return CDialog::DoModal();
//}


void CItemOptionLogDialog::Request( const CString& serverName, const CString& itemName, DWORD itemDbIndex )
{
	mServerCombo.SetCurSel( mServerCombo.FindString( 0, serverName ) );

	mItemName = itemName;

	{
		CString text;
		text.Format( "%d", itemDbIndex );

		mItemDbIndexEdit.SetWindowText( text );
	}

	ShowWindow( SW_SHOW );
}


void CItemOptionLogDialog::OnCancel()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	mResultListCtrl.DeleteAllItems();

	CDialog::OnCancel();
}

// PlayerDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "client.h"
#include "PlayerDialog.h"
#include "ExperienceLogDialog.h"
#include "StatLogDialog.h"
#include "UserDialog.h"
#include "JobLogDialog.h"
#include "ItemLogDialog.h"
#include "NameLogDialog.h"


// CPlayerDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPlayerDialog, CDialog)
CPlayerDialog::CPlayerDialog( CclientApp& application, CWnd* pParent /*=NULL*/)
	: CDialog(CPlayerDialog::IDD, pParent)
	, mApplication( application )
{
	EnableAutomation();

	ZeroMemory( &mReceivedDataMessage, sizeof( mReceivedDataMessage ) );
}


CPlayerDialog::~CPlayerDialog()
{
}

void CPlayerDialog::OnFinalRelease()
{
	// 자동화 개체에 대한 마지막 참조가 해제되면 OnFinalRelease가
	// 호출됩니다. 기본 클래스에서 자동으로 개체를 삭제합니다.
	// 기본 클래스를 호출하기 전에 개체에 필요한 추가 정리 작업을
	// 추가하십시오.

	CDialog::OnFinalRelease();
}

void CPlayerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//DDX_GridControl( pDX, IDC_PLAYER_GRID, mDataGrid );
	//DDX_Control(pDX, IDC_PLAYER_JOB_LIST, mJobList);
	//DDX_Control(pDX, IDC_PLAYER_JOB_CATEGORY_STATIC, mJobCategoryStatic);
	//DDX_Control(pDX, IDC_PLAYER_LICENSE_AGE_COMBO, mLicenseAgeEdit);
	DDX_Control(pDX, IDC_PLAYER_LICENSE_LOVE_EDIT, mLoveScoreEdit);
	DDX_Control(pDX, IDC_LICENSE_GOOD_TREE, mGoodThingsTree);
	DDX_Control(pDX, IDC_LICENSE_BAD_TREE, mBadThingsTree);
	DDX_Control(pDX, IDC_PLAYER_LICENSE_UPDATE_BUTTNO, mLicenseUpdateButton);
	DDX_Control(pDX, IDC_PLAYER_LICENSE_AGE_EDIT, mAgeEdit);
	//DDX_Control(pDX, IDC_PLAYER_GUILD_BUTTON, mGuildButton);
	DDX_Control(pDX, IDC_PLAYER_USER_NAME_EDIT, mUserNameEdit);
	DDX_Control(pDX, IDC_PLAYER_LICENSE_AREA_COMBO2, mAreaCombo);
	DDX_Control(pDX, IDC_PLAYER_GUILD_BUTTON, mGuildButton);
	DDX_Control(pDX, IDC_PLAYER_JOB1_COMBO, mJob1Combo);
	DDX_Control(pDX, IDC_PLAYER_JOB2_COMBO, mJob2Combo);
	DDX_Control(pDX, IDC_PLAYER_JOB3_COMBO, mJob3Combo);
	DDX_Control(pDX, IDC_PLAYER_JOB4_COMBO, mJob4Combo);
	DDX_Control(pDX, IDC_PLAYER_JOB5_COMBO, mJob5Combo);
	DDX_Control(pDX, IDC_PLAYER_JOB6_COMBO, mJob6Combo);
	DDX_Control(pDX, IDC_PLAYER_DATA_TAB, mTabCtrl);
	DDX_Control(pDX, IDC_PLAYER_ITEM, mDataSubmitButton);
	DDX_Control(pDX, IDC_PLAYER_NAME_UPDATE_BUTTON, mExtendedDataSubmitButton);
}


BEGIN_MESSAGE_MAP(CPlayerDialog, CDialog)
	ON_COMMAND(ID_VIEW_STATUS_BAR, OnViewStatusBar)
//	ON_BN_CLICKED(IDC_PLAYER_DIALOG_UPDATE, OnBnClickedPlayerDialogUpdate)
	ON_BN_CLICKED(IDC_PLAYER_ITEM, OnBnClickedPlayerItem)
	ON_BN_CLICKED(IDC_PLAYER_LICENSE_UPDATE_BUTTNO, OnBnClickedPlayerLicenseUpdateButtno)
	ON_BN_CLICKED(IDC_PLAYER_EXP_LOG_BUTTON, OnBnClickedPlayerExpLogButton)
	ON_BN_CLICKED(IDC_PLAYER_STAT_LOG_BUTTON, OnBnClickedPlayerStatLogButton)
	ON_BN_CLICKED(IDC_PLAYER_GUILD_BUTTON, OnBnClickedPlayerGuildButton)
	ON_BN_CLICKED(IDC_PLAYER_USER_VIEW_BUTTON, OnBnClickedPlayerUserViewButton)
	//ON_BN_CLICKED(IDC_PLAYER_JOB_LOG_BUTTON, OnBnClickedPlayerJobLogButton)
	ON_BN_CLICKED(IDC_PLAYER_GUILD_BUTTON2, OnBnClickedPlayerJobButton)
	ON_BN_CLICKED(IDC_PLAYER_ITEM_LOG_BUTTON, OnBnClickedPlayerItemLogButton)
	//ON_BN_CLICKED(IDC_PLAYER_ITEM_ADD_BUTTON, OnBnClickedPlayerItemAddButton)
	ON_BN_CLICKED(IDC_PLAYER_NAME_UPDATE_BUTTON, OnBnClickedPlayerNameUpdateButton)
	ON_BN_CLICKED(IDC_PLAYER_JOB_SUBMIT_BUTTON, OnBnClickedPlayerJobSubmitButton)
	ON_CBN_SELCHANGE(IDC_PLAYER_JOB1_COMBO, OnCbnSelchangePlayerJob1Combo)
	ON_CBN_SELCHANGE(IDC_PLAYER_JOB2_COMBO, OnCbnSelchangePlayerJob2Combo)
	ON_CBN_SELCHANGE(IDC_PLAYER_JOB3_COMBO, OnCbnSelchangePlayerJob3Combo)
	ON_CBN_SELCHANGE(IDC_PLAYER_JOB4_COMBO, OnCbnSelchangePlayerJob4Combo)
	ON_CBN_SELCHANGE(IDC_PLAYER_JOB5_COMBO, OnCbnSelchangePlayerJob5Combo)
	ON_CBN_SELCHANGE(IDC_PLAYER_JOB6_COMBO, OnCbnSelchangePlayerJob6Combo)
	ON_NOTIFY(TCN_SELCHANGE, IDC_PLAYER_DATA_TAB, OnTcnSelchangePlayerDataTab)
	ON_BN_CLICKED(IDC_PLAYER_NAME_LOG_BUTTON, OnBnClickedPlayerNameLogButton)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CPlayerDialog, CDialog)
END_DISPATCH_MAP()

// 참고: IID_IPlayerDialog에 대한 지원을 추가하여
// VBA에서 형식 안정적인 바인딩을 지원합니다. 
// 이 IID는 .IDL 파일에 있는 dispinterface의 GUID와 일치해야 합니다.

// {3B39A217-C6E8-45A3-813A-7B5AB700101B}
static const IID IID_IPlayerDialog =
{ 0x3B39A217, 0xC6E8, 0x45A3, { 0x81, 0x3A, 0x7B, 0x5A, 0xB7, 0x0, 0x10, 0x1B } };

BEGIN_INTERFACE_MAP(CPlayerDialog, CDialog)
	INTERFACE_PART(CPlayerDialog, IID_IPlayerDialog, Dispatch)
END_INTERFACE_MAP()


BOOL CPlayerDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon( AfxGetApp()->LoadIcon( IDR_MAINFRAME ), FALSE );
	SetIcon( AfxGetApp()->LoadIcon( IDR_MAINFRAME ), TRUE );

	// combo initialization
	{
		{
			typedef CclientApp::LicenseAreaMap AreaMap;

			const AreaMap& areaMap = mApplication.GetLicenseAreaMap();

			for( AreaMap::const_iterator it = areaMap.begin(); areaMap.end() != it; ++it )
			{
				CString text;
				text.Format( "%02d %s", it->first, it->second );

				mAreaCombo.AddString( text );
			}

			CRect rect;
			mAreaCombo.GetWindowRect( rect );
			mAreaCombo.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );
		}

		// 직업 콤보
		{
			CRect rect;
			mJob1Combo.GetWindowRect( rect );
			mJob1Combo.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );

			mJob2Combo.GetWindowRect( rect );
			mJob2Combo.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );

			mJob3Combo.GetWindowRect( rect );
			mJob3Combo.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );

			mJob4Combo.GetWindowRect( rect );
			mJob4Combo.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );

			mJob5Combo.GetWindowRect( rect );
			mJob5Combo.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );

			mJob6Combo.GetWindowRect( rect );
			mJob6Combo.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );
		}
	}

	// tree initialization
	{
		typedef CclientApp::LicenseFavoriteMap FavoriteMap;
		
		const FavoriteMap& favoriteMap = mApplication.GetLicenseFavoriteMap();

		for( FavoriteMap::const_iterator it = favoriteMap.begin(); favoriteMap.end() != it; ++it )
		{
			CString text;
			text.Format( "%d %s", it->first, it->second );
			
			mGoodThingsTree.InsertItem( text, 0, 0, TVI_ROOT, TVI_LAST );
			mBadThingsTree.InsertItem( text, 0, 0, TVI_ROOT, TVI_LAST );
		}

		// 체크박스가 최초에 그려지거나 안 그려질 경우가 발생한다.
		// 이를 해결하는 건 일종의 꼼수지만, 다른 방법은 아직 없다.
		// 
		// http://www.codeguru.com/forum/archive/index.php/t-197692.html
		{
			mGoodThingsTree.ModifyStyle( TVS_CHECKBOXES, 0 );
			mGoodThingsTree.ModifyStyle( 0, TVS_CHECKBOXES );

			mBadThingsTree.ModifyStyle( TVS_CHECKBOXES, 0 );
			mBadThingsTree.ModifyStyle( 0, TVS_CHECKBOXES );
		}
	}

	// grid initialization
	{
		CRect gridRect;
		{
			CRect windowRect;
			GetWindowRect( windowRect );

			CRect tabRect;
			mTabCtrl.GetWindowRect( tabRect );

			CRect buttonRect;
			mDataSubmitButton.GetWindowRect( buttonRect );

			gridRect.left	= abs( tabRect.left - windowRect.left );
			gridRect.top	= abs( tabRect.top - windowRect.top );

			gridRect.right	= gridRect.left + tabRect.Width();
			gridRect.bottom	= gridRect.top	+ tabRect.Height() - buttonRect.Height();

			// scaling
			{
				// 90%
				const float scale	= 0.9f;
				const LONG	width	= LONG( gridRect.Width() * scale );
				const LONG	height	= LONG( gridRect.Height() * scale );

				gridRect.left	+= abs( tabRect.Width() - width ) / 2;
				gridRect.right	-= abs( tabRect.Width() - width ) / 2;

				//gridRect.top	-= abs( gridRect.Height() - height ) / 2;
				gridRect.bottom	= gridRect.top + height;
			}
		}

		// create numeric grid
		{
			CString textGender;
			textGender.LoadString( IDS_STRING92 );

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

			CString textLevel;
			textLevel.LoadString( IDS_STRING5 );

			CString textMaxLevel;
			textMaxLevel.LoadString( IDS_STRING93 );

			CString textExperience;
			textExperience.LoadString( IDS_STRING9 );

			CString textStatPoint;
			textStatPoint.LoadString( IDS_STRING94 );

			CString textGold;
			textGold.LoadString( IDS_STRING95 );

			CString textPlayTime;
			textPlayTime.LoadString( IDS_STRING161 );

			CString textSkillPoint;
			textSkillPoint.LoadString( IDS_STRING96 );

			CString textExtendedInventory;
			textExtendedInventory.LoadString( IDS_STRING162 );

			CString textMap;
			textMap.LoadString( IDS_STRING163 );

			CString textLocation;
			textLocation.LoadString( IDS_STRING68 );

			// *은 수정 가능한 값을 나타냄
			const CString title[] = { 
					textGender + "*",
					textStrength + "*",
					textDexterity + "*",
					textVitality + "*",
					textIntelligence + "*",
					textWisdom + "*",
					textLife + "*",
					textMana + "*",
					textLevel + "*",
					textMaxLevel + "*",
					textExperience + "*",
					textStatPoint + "*",
					textGold + "*",
					textPlayTime + "*",
					textSkillPoint + "*",
					textExtendedInventory + "*",
					textMap  + "*",
					"X" + textLocation + "*",
					"Y" + textLocation + "*",
					""	// it must end with empty string
			};

			const int titleColumn = int( gridRect.Width() * 0.6f );
			const int valueColumn = int( gridRect.Width() * 0.4f );
			const int columnWidth = titleColumn + valueColumn;

			//CclientApp::CreateGrid( this, &mDataGrid, IDC_PLAYER_GRID, CRect( 0, 0, columnWidth, 0 ), title );

			CclientApp::CreateGrid( this, &mDataGrid, IDC_PLAYER_GRID, gridRect, title,FALSE );

			mDataGrid.SetColumnWidth( 0, titleColumn );
			mDataGrid.SetColumnWidth( 1, valueColumn );

			mDataGrid.ExpandToFit();
		}

		// create string grid
		{
			CString textName;
			textName.LoadString( IDS_STRING20 );

			CString textGuild;
			textGuild.LoadString( IDS_STRING164 );

			CString textFamily;
			textFamily.LoadString( IDS_STRING165 );

			CString textNick;
			textNick.LoadString( IDS_STRING21 );

			CString textLastConnection;
			textLastConnection.LoadString( IDS_STRING166 );

			CString textIsEnableConnection;
			textIsEnableConnection.LoadString( IDS_STRING167 );

			CString textOriginalName;
			textOriginalName.LoadString( IDS_STRING168 );

			CString textCreatedDate;
			textCreatedDate.LoadString( IDS_STRING169 );

			CString textCreatedIp;
			textCreatedIp.LoadString( IDS_STRING170 );

			CString textDeletedDate;
			textDeletedDate.LoadString( IDS_STRING171 );

			CString textDeletedIp;
			textDeletedIp.LoadString( IDS_STRING172 );

			CString textRestoredDate;
			textRestoredDate.LoadString( IDS_STRING173 );

			CString textRestoredIp;
			textRestoredIp.LoadString( IDS_STRING174 );

			// *은 수정 가능한 값을 나타냄
			const CString title[] = {
					textName + "*",
					textGuild + " " + textNick + "*",
					textFamily + " " + textNick + "*",
					textLastConnection,
					textIsEnableConnection,
					textOriginalName,
					textCreatedDate,
					textCreatedIp,
					textDeletedDate,
					textDeletedIp,
					textRestoredDate,
					textRestoredIp,
					""	// it must end with empty string
			};

			const int titleColumn = int( gridRect.Width() * 0.35f );
			const int valueColumn = int( gridRect.Width() * 0.65f );
			const int columnWidth = titleColumn + valueColumn;

			CclientApp::CreateGrid( this, &mExtendedDataGrid, IDC_PLAYER_EXTENDED_GRID, gridRect, title, FALSE );

			mExtendedDataGrid.SetColumnWidth( 0, titleColumn );
			mExtendedDataGrid.SetColumnWidth( 1, valueColumn );
		}
	}

	// tab initialization
	{
		CString textGameInformation;
		textGameInformation.LoadString( IDS_STRING175 );
		mTabCtrl.InsertItem( mTabCtrl.GetItemCount(), textGameInformation );

		CString textUsingInformation;
		textUsingInformation.LoadString( IDS_STRING176 );
		mTabCtrl.InsertItem( mTabCtrl.GetItemCount(), textUsingInformation );

		mTabCtrl.SetCurSel( 0 );

		mExtendedDataGrid.ShowWindow( SW_HIDE );
		mExtendedDataSubmitButton.ShowWindow( SW_HIDE );
	}

	return TRUE;
}


void CPlayerDialog::Parse( DWORD serverIndex, const MSGROOT* message )
{
	mServerIndex = serverIndex;

	switch( message->Protocol )
	{
	case MP_RM_PLAYER_GET_DATA_ACK:
		{
			const MSG_RM_PLAYER_DATA* m = ( MSG_RM_PLAYER_DATA* )message;
			mReceivedDataMessage = *m;

			mDataGrid.ClearCells( CCellRange( 0, 1 ) );

			const DWORD style	= DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
			GV_ITEM		cell	= { 0 };
			
			cell.nFormat	= style;
			cell.col		= 1;
			cell.row		= 0;
			cell.mask		= GVIF_TEXT;

			{
				const MSG_RM_PLAYER_DATA::Player& player = mReceivedDataMessage.mPlayer;

				cell.strText.Format( "%s", player.mGender ? "여성" : "남성" );
				mDataGrid.SetItem( &cell );
				mDataGrid.SetItemState( cell.row, cell.col, GVIS_READONLY );
				++cell.row;	

				cell.strText.Format( "%d", player.mStrength );
				mDataGrid.SetItem( &cell );
				mDataGrid.SetItemState( cell.row, cell.col, GVIS_MODIFIED );
				++cell.row;	

				cell.strText.Format( "%d", player.mDexterity );
				mDataGrid.SetItemState( cell.row, cell.col, GVIS_MODIFIED );
				mDataGrid.SetItem( &cell );
				++cell.row;	

				cell.strText.Format( "%d", player.mVitality );
				mDataGrid.SetItemState( cell.row, cell.col, GVIS_MODIFIED );
				mDataGrid.SetItem( &cell );
				++cell.row;	

				cell.strText.Format( "%d", player.mIntelligence );
				mDataGrid.SetItemState( cell.row, cell.col, GVIS_MODIFIED );
				mDataGrid.SetItem( &cell );
				++cell.row;	

				cell.strText.Format( "%d", player.mWisdom );
				mDataGrid.SetItem( &cell );
				++cell.row;	

				cell.strText.Format( "%d", player.mLife );
				mDataGrid.SetItem( &cell );
				++cell.row;	

				cell.strText.Format( "%d", player.mMana );
				mDataGrid.SetItem( &cell );
				++cell.row;	

				cell.strText.Format( "%d", player.mGrade );
				mDataGrid.SetItem( &cell );
				++cell.row;

				cell.strText.Format( "%d", player.mMaxGrade );
				mDataGrid.SetItem( &cell );
				++cell.row;

				cell.strText.Format( "%d", player.mExp );
				mDataGrid.SetItem( &cell );
				++cell.row;

				cell.strText.Format( "%d", player.mGradeUpPoint );
				mDataGrid.SetItem( &cell );
				++cell.row;

				cell.strText.Format( "%u", player.mMoney );
				mDataGrid.SetItem( &cell );
				++cell.row;

				cell.strText.Format( "%u", player.mPlayTime );
				mDataGrid.SetItemState( cell.row, cell.col, GVIS_READONLY );
				mDataGrid.SetItem( &cell );
				++cell.row;

				cell.strText.Format( "%d", player.mSkillPoint );
				mDataGrid.SetItemState( cell.row, cell.col, GVIS_MODIFIED );
				mDataGrid.SetItem( &cell );
				++cell.row;

				cell.strText.Format( "%d", player.mExtendedInventorySize );
				mDataGrid.SetItemState( cell.row, cell.col, GVIS_MODIFIED );
				mDataGrid.SetItem( &cell );
				++cell.row;
			}

			{
				const MSG_RM_PLAYER_DATA::Position& position = mReceivedDataMessage.mPosition;

				cell.strText.Format( "%d", position.mMap );
				mDataGrid.SetItemState( cell.row, cell.col, GVIS_MODIFIED );
				mDataGrid.SetItem( &cell );
				++cell.row;	

				cell.strText.Format( "%d", position.mX );
				mDataGrid.SetItem( &cell );
				++cell.row;

				cell.strText.Format( "%d", position.mY );
				mDataGrid.SetItem( &cell );
				++cell.row;
			}

			{
				const MSG_RM_PLAYER_DATA::Job& job = mReceivedDataMessage.mJob;

				{
					const DWORD race = 1;
					DWORD		step = 0;

					mApplication.SetJobCombo( mJob1Combo, job.mData1, race, ++step, 1 );
					mApplication.SetJobCombo( mJob2Combo, job.mData1, race, ++step, job.mData2 );
					mApplication.SetJobCombo( mJob3Combo, job.mData1, race, ++step, job.mData3 );
					mApplication.SetJobCombo( mJob4Combo, job.mData1, race, ++step, job.mData4 );
					mApplication.SetJobCombo( mJob5Combo, job.mData1, race, ++step, job.mData5 );
					mApplication.SetJobCombo( mJob6Combo, job.mData1, race, ++step, job.mData6 );
				}
			}

			{
				const MSG_RM_PLAYER_DATA::Guild& guild = mReceivedDataMessage.mGuild;

				CString text;

				if( guild.mIndex )
				{
					CString textGuild;
					textGuild.LoadString( IDS_STRING164 );

					CString textData;
					textData.LoadString( IDS_STRING69 );

					text.Format( textGuild + textData + " %s ", guild.mName );
					mGuildButton.SetWindowText( text );
					mGuildButton.EnableWindow( TRUE );
				}
				else
				{
					CString textThereIsNoResult;
					textThereIsNoResult.LoadString( IDS_STRING1 );

					mGuildButton.SetWindowText( textThereIsNoResult );
					mGuildButton.EnableWindow( FALSE );
				}
			}

			{
				mTabCtrl.EnableWindow( TRUE );
			}

			break;
		}
	case MP_RM_PLAYER_GET_EXTENDED_DATA_ACK:
		{
			const MSG_RM_PLAYER_EXTENDED_DATA* m = ( MSG_RM_PLAYER_EXTENDED_DATA* )message;
			mReceivedExtendedMessage = *m;

			mExtendedDataGrid.ClearCells( CCellRange( 0, 1 ) );

			const DWORD style	= DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
			GV_ITEM		cell	= { 0 };

			cell.nFormat	= style;
			cell.col		= 1;
			cell.row		= 0;
			cell.mask		= GVIF_TEXT;

			{
				const MSG_RM_PLAYER_EXTENDED_DATA::Player& player = mReceivedExtendedMessage.mPlayer;

				cell.strText.Format( "%s", player.mName );
				mExtendedDataGrid.SetItem( &cell );
				//mExtendedDataGrid.SetItemState( cell.row, cell.col, GVIS_READONLY );
				mExtendedDataGrid.SetItemState( cell.row, cell.col, GVIS_MODIFIED );
				++cell.row;

				cell.strText.Format( "%s", player.mGuildNick );
				mExtendedDataGrid.SetItem( &cell );
				mExtendedDataGrid.SetItemState( cell.row, cell.col, GVIS_MODIFIED );
				++cell.row;

				cell.strText.Format( "%s", player.mFamilyNick );
				mExtendedDataGrid.SetItem( &cell );
				mExtendedDataGrid.SetItemState( cell.row, cell.col, GVIS_READONLY );
				++cell.row;

				cell.strText.Format( "%s", player.mLastPlayedTime );
				mExtendedDataGrid.SetItem( &cell );
				mExtendedDataGrid.SetItemState( cell.row, cell.col, GVIS_READONLY );
				++cell.row;
			}

			{
				const MSG_RM_PLAYER_EXTENDED_DATA::History& history = mReceivedExtendedMessage.mHistory;

				CString textEnable;
				textEnable.LoadString( IDS_STRING25 );

				CString textUnable;
				textUnable.LoadString( IDS_STRING26 );

				cell.strText.Format( "%s", history.mIsEnable ? textEnable : textUnable );
				mExtendedDataGrid.SetItem( &cell );
				mExtendedDataGrid.SetItemState( cell.row, cell.col, GVIS_READONLY );
				++cell.row;

				cell.strText.Format( "%s", history.mOriginalName );
				mExtendedDataGrid.SetItem( &cell );
				mExtendedDataGrid.SetItemState( cell.row, cell.col, GVIS_READONLY );
				++cell.row;

				cell.strText.Format( "%s", history.mCreateDate );
				mExtendedDataGrid.SetItem( &cell );
				mExtendedDataGrid.SetItemState( cell.row, cell.col, GVIS_READONLY );
				++cell.row;

				cell.strText.Format( "%s", history.mCreateIp );
				mExtendedDataGrid.SetItem( &cell );
				mExtendedDataGrid.SetItemState( cell.row, cell.col, GVIS_READONLY );
				++cell.row;

				cell.strText.Format( "%s", history.mDeletedDate );
				mExtendedDataGrid.SetItem( &cell );
				mExtendedDataGrid.SetItemState( cell.row, cell.col, GVIS_READONLY );
				++cell.row;

				cell.strText.Format( "%s", history.mDeletedIp );
				mExtendedDataGrid.SetItem( &cell );
				mExtendedDataGrid.SetItemState( cell.row, cell.col, GVIS_READONLY );
				++cell.row;

				cell.strText.Format( "%s", history.mRecoveredDate );
				mExtendedDataGrid.SetItem( &cell );
				mExtendedDataGrid.SetItemState( cell.row, cell.col, GVIS_READONLY );
				++cell.row;

				cell.strText.Format( "%s", history.mRecoveredIp );
				mExtendedDataGrid.SetItem( &cell );
				mExtendedDataGrid.SetItemState( cell.row, cell.col, GVIS_READONLY );
				++cell.row;
			}

			break;
		}
	case MP_RM_PLAYER_GET_LICENSE_ACK:
		{
			{
				mAgeEdit.EnableWindow( TRUE );
				mAreaCombo.EnableWindow( TRUE );
				mLoveScoreEdit.EnableWindow( TRUE );
				mGoodThingsTree.EnableWindow( TRUE );
				mBadThingsTree.EnableWindow( TRUE );
				mLicenseUpdateButton.EnableWindow( TRUE );
			}

			memcpy( &mReceivedLicenseMessage, message, sizeof( mReceivedLicenseMessage ) );

			const DATE_MATCHING_INFO& data	= mReceivedLicenseMessage.mDateMatching;

			{
 				CString text;
				text.Format( "%d", data.dwAge );
				mAgeEdit.SetWindowText( text );

				//mLicenseAgeEdit.SetCurSel( data.dwAge );
				mAreaCombo.SetCurSel( data.dwRegion );

				text.Format( "%d", data.dwGoodFeelingDegree );
				mLoveScoreEdit.SetWindowText( text );
			}

			// 트리 컨트롤의 체크 박스 세팅
			{
				typedef CclientApp::LicenseFavoriteMap FavoriteMap;
				
				struct
				{
					void operator() ( const FavoriteMap& favoriteMap, CTreeCtrl& tree, DWORD index ) const
					{
						FavoriteMap::const_iterator it = favoriteMap.find( index );

						if( favoriteMap.end() == it )
						{
							return;
						}

						CString text;
						text.Format( "%d %s", it->first, it->second );

						for(	HTREEITEM item = tree.GetFirstVisibleItem();
								item;
								item = tree.GetNextVisibleItem( item ) )
						{
							const CString currentText = tree.GetItemText( item );

							if( text == currentText )
							{
								tree.SetCheck( item );

								return;
							}
						}
					}
				}
				SetTreeCheck;

				const FavoriteMap& favoriteMap = mApplication.GetLicenseFavoriteMap();

				SetTreeCheck( favoriteMap, mGoodThingsTree, data.pdwGoodFeeling[ 0 ] );
				SetTreeCheck( favoriteMap, mGoodThingsTree, data.pdwGoodFeeling[ 1 ] );
				SetTreeCheck( favoriteMap, mGoodThingsTree, data.pdwGoodFeeling[ 2 ] );
				SetTreeCheck( favoriteMap, mBadThingsTree, data.pdwBadFeeling[ 0 ] );
				SetTreeCheck( favoriteMap, mBadThingsTree, data.pdwBadFeeling[ 1 ] );

				mGoodThingsTree.InvalidateRect( 0, FALSE );
			}

			break;
		}
	case MP_RM_PLAYER_GET_USER_ACK:
		{
			const MSG_DWORD_NAME* m = ( MSG_DWORD_NAME* )message;

			int start = 0;

			mUserNameEdit.SetWindowText( CString( m->Name ).Tokenize( " ", start ) );
			mUserNameEdit.SetSel( 0, mUserNameEdit.GetWindowTextLength() );
			mUserIndex = m->dwData;
			break;
		}
	case MP_RM_PLAYER_GET_USER_NACK:
		{
			ASSERT( 0 && "There is no id for login" );
			break;
		}
	case MP_RM_PLAYER_GET_LICENSE_NACK:
		{
			{
				mAgeEdit.EnableWindow( FALSE );
				mAreaCombo.EnableWindow( FALSE );
				mLoveScoreEdit.EnableWindow( FALSE );
				mGoodThingsTree.EnableWindow( FALSE );
				mBadThingsTree.EnableWindow( FALSE );
				mLicenseUpdateButton.EnableWindow( FALSE );
			}

			break;
		}
	case MP_RM_PLAYER_SET_DATA_ACK:
	case MP_RM_PLAYER_SET_EXTENDED_DATA_ACK:
	case MP_RM_PLAYER_SET_LICENSE_ACK:
		{
			CString textUpdateWasSucceed;
			textUpdateWasSucceed.LoadString( IDS_STRING28 );

			MessageBox( textUpdateWasSucceed, "", MB_OK );
			break;
		}
	case MP_RM_PLAYER_SET_LICENSE_NACK:
	case MP_RM_PLAYER_SET_DATA_NACK:
		{
			CString textUpdateWasFailed;
			textUpdateWasFailed.LoadString( IDS_STRING29 );

			MessageBox( textUpdateWasFailed, "", MB_OK );

			Parse( mServerIndex, &mReceivedDataMessage );
			break;
		}
	case MP_RM_PLAYER_SET_EXTENDED_DATA_NACK:
		{
			CString textSameNameIsExisted;
			textSameNameIsExisted.LoadString( IDS_STRING180 );
			MessageBox( textSameNameIsExisted, "", MB_OK );

			Parse( mServerIndex, &mReceivedExtendedMessage );
			break;
		}
	case MP_RM_PLAYER_NACK_BY_AUTH:
		{
			CString textYouHaveNoAuthority;
			textYouHaveNoAuthority.LoadString( IDS_STRING18 );

			MessageBox( textYouHaveNoAuthority, "", MB_OK | MB_ICONERROR );
			break;
		}
	default:
		{
			ASSERT( 0 && "It's not defined message" );
			break;
		}
	}
}
void CPlayerDialog::OnViewStatusBar()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	ShowWindow( SW_SHOW );
}

//void CPlayerDialog::OnBnClickedPlayerDialogUpdate()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.	
//}

void CPlayerDialog::OnBnClickedPlayerItem()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 서버에 값 변경을 요청한다
	MSG_RM_PLAYER_DATA message;
	message = mReceivedDataMessage;

	// 셀의 데이터를 모두 읽는다. 주의: 순서를 잘 맞춰야한다...
	// 변경되어서는 안될 데이터는 셀에 read only 처리를 하자
	{
		MSG_RM_PLAYER_DATA::Player& player = message.mPlayer;

		//player.mGender			= atoi( mDataGrid.GetItemText( 0, 1 ) );
		player.mStrength				= atoi( mDataGrid.GetItemText( 1, 1 ) );
		player.mDexterity				= atoi( mDataGrid.GetItemText( 2, 1 ) );
		player.mVitality				= atoi( mDataGrid.GetItemText( 3, 1 ) );
		player.mIntelligence			= atoi( mDataGrid.GetItemText( 4, 1 ) );
		player.mWisdom					= atoi( mDataGrid.GetItemText( 5, 1 ) );
		player.mLife					= atoi( mDataGrid.GetItemText( 6, 1 ) );
		player.mMana					= atoi( mDataGrid.GetItemText( 7, 1 ) );
		player.mGrade					= atoi( mDataGrid.GetItemText( 8, 1 ) );
		player.mMaxGrade				= atoi( mDataGrid.GetItemText( 9, 1 ) );
		player.mExp						= atoi( mDataGrid.GetItemText( 10, 1 ) );
		player.mGradeUpPoint			= atoi( mDataGrid.GetItemText( 11, 1 ) );
		player.mMoney					= strtoul( mDataGrid.GetItemText( 12, 1 ), 0, 10 );
		//player.mPlayTime				= strtoul( mDataGrid.GetItemText( 13, 1 ), 0, 10 );
		player.mSkillPoint				= atoi( mDataGrid.GetItemText( 14, 1 ) );
		player.mExtendedInventorySize	= atoi( mDataGrid.GetItemText( 15, 1 ) );
	}

	{
		MSG_RM_PLAYER_DATA::Position& position = message.mPosition;

		position.mMap	= atoi( mDataGrid.GetItemText( 16, 1 ) );
		position.mX		= atoi( mDataGrid.GetItemText( 17, 1 ) );
		position.mY		= atoi( mDataGrid.GetItemText( 18, 1 ) );
	}

	if( ! memcmp( &message, &mReceivedDataMessage, sizeof( message ) ) )
	{
		return;
	}

	message.Category	= MP_RM_PLAYER;
	message.Protocol	= MP_RM_PLAYER_SET_DATA_SYN;
	message.dwObjectID	= mPlayerIndex;

	mApplication.Send( mServerIndex, message, sizeof( message ) );
}


void CPlayerDialog::Request( DWORD serverIndex, const CString& playerName, DWORD playerIndex )
{
	mServerIndex	= serverIndex;

	Request( mApplication.GetServerName( mServerIndex ), playerName, playerIndex );
}


void CPlayerDialog::Request( const CString& serverName, const CString& playerName, DWORD playerIndex )
{
	mPlayerIndex				= playerIndex;
	mPlayerName					= playerName;
	mServerName					= serverName;
	
	{
		CString	textTitleFormat;
		textTitleFormat.LoadString( IDS_STRING181 );

		CString text;
		text.Format( textTitleFormat, mServerName, playerName );

		SetWindowText( text );
	}

	// 데이터가 오기전까지 컨트롤을 잠근다
	{
		mTabCtrl.EnableWindow( FALSE );
	}

	{
		MSG_RM_GET_PLAYER_SYN message;
		ZeroMemory( &message, sizeof( message ) );

		message.Category		= MP_RM_PLAYER;
		message.Protocol		= MP_RM_PLAYER_GET_DATA_SYN;
		message.mPlayerIndex	= playerIndex;

		mApplication.Send( mServerIndex, message, sizeof( message ) );
	}
}


void CPlayerDialog::OnBnClickedPlayerLicenseUpdateButtno()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	MSG_RM_PLAYER_LOVE	message;
	{
		ASSERT( sizeof( message ) == sizeof( mReceivedLicenseMessage ) );

		memcpy( &message, &mReceivedLicenseMessage, sizeof( message ) );

		message.Category	= MP_RM_PLAYER;
		message.Protocol	= MP_RM_PLAYER_SET_LICENSE_SYN;
	}

	DATE_MATCHING_INFO&	data	= message.mDateMatching;

	{
		CString text;
		mAgeEdit.GetWindowText( text);
		data.dwAge		= atoi( text );

		mAreaCombo.GetLBText( mAreaCombo.GetCurSel(), text );
		int start = 0;
		data.dwRegion	= atoi( text.Tokenize( " ", start ) );

		mLoveScoreEdit.GetWindowText( text );
		data.dwGoodFeelingDegree	= atoi( text );
	}

	std::set< DWORD > favorite;

	// 선호 항목 트리 컨트롤의 값을 담는다
	{
		int			i		= 0;
		const DWORD size	= sizeof( data.pdwGoodFeeling ) / sizeof( *data.pdwGoodFeeling );

        for(	HTREEITEM item = mGoodThingsTree.GetFirstVisibleItem();
				item;
				item = mGoodThingsTree.GetNextVisibleItem( item ) )
		{
			if( ! mGoodThingsTree.GetCheck( item ) )
			{
				continue;
			}
			else if( i > size )
			{
				CString textSelectSomeFavoriteThing;
				textSelectSomeFavoriteThing.LoadString( IDS_STRING182 );

				CString text;
				text.Format( textSelectSomeFavoriteThing, size );

				MessageBox( text, "", MB_ICONERROR | MB_OK );
				return;
			}

			CString text	= mGoodThingsTree.GetItemText( item );
			int		start	= 0;

			const DWORD index = atoi( text.Tokenize( " ", start ) );
			data.pdwGoodFeeling[ i ] = index;

			favorite.insert( index );

			++i;
		}

		if( i < size )
		{
			CString textSelectSomeFavoriteThing;
			textSelectSomeFavoriteThing.LoadString( IDS_STRING182 );

			CString text;
			text.Format( textSelectSomeFavoriteThing, size );

			MessageBox( text, "", MB_ICONERROR | MB_OK );
			return;
		}
	}

	// 비선호 항목 트리 컨트롤의 값을 담는다
	{
		int			i		= 0;
		const DWORD size	= sizeof( data.pdwBadFeeling ) / sizeof( *data.pdwBadFeeling );

		for(	HTREEITEM item = mBadThingsTree.GetFirstVisibleItem();
				item;
				item = mBadThingsTree.GetNextVisibleItem( item ) )
		{
			if( ! mBadThingsTree.GetCheck( item ) )
			{
				continue;
			}
			else if( i > size )
			{				
				CString textSelectSomeUnfavoriteThing;
				textSelectSomeUnfavoriteThing.LoadString( IDS_STRING183 );

				CString text;
				text.Format( textSelectSomeUnfavoriteThing, size );

				MessageBox( text, "", MB_ICONERROR | MB_OK );
				return;
			}

			CString text			= mBadThingsTree.GetItemText( item );
			int		start			= 0;
			const DWORD index = atoi( text.Tokenize( " ", start ) );

			if( favorite.end() != favorite.find( index ) )
			{
				CString textDoNotSelectFavoriteAndUnfavoriteThingTogether;
				textDoNotSelectFavoriteAndUnfavoriteThingTogether.LoadString( IDS_STRING184 );

				MessageBox( textDoNotSelectFavoriteAndUnfavoriteThingTogether, "", MB_ICONERROR | MB_OK );
				return;
			}

			data.pdwBadFeeling[ i ] = index;

			++i;
		}

		if( i < size )
		{
			CString textSelectSomeUnfavoriteThing;
			textSelectSomeUnfavoriteThing.LoadString( IDS_STRING183 );

			CString text;
			text.Format( textSelectSomeUnfavoriteThing, size );

			MessageBox( text, "", MB_ICONERROR | MB_OK );
			return;
		}
	}

	ASSERT( sizeof( message ) == sizeof( mReceivedLicenseMessage ) );

	if( ! memcmp( &message, &mReceivedLicenseMessage, sizeof( message ) ) )
	{
		return;
	}

	mApplication.Send( mServerIndex, message, sizeof( message ) );
}


void CPlayerDialog::OnBnClickedPlayerExpLogButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CExperienceLogDialog* dialog = mApplication.GetExperienceDialog();
	ASSERT( dialog );

	dialog->DoModal( mApplication.GetServerName( mServerIndex ), mPlayerName, mPlayerIndex );
}


void CPlayerDialog::OnBnClickedPlayerStatLogButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CStatLogDialog* dialog = mApplication.GetStatLogDialog();
	ASSERT( dialog );

	dialog->DoModal( mApplication.GetServerName( mServerIndex ), mPlayerName, mPlayerIndex );
}


void CPlayerDialog::OnBnClickedPlayerGuildButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	const char* name = mReceivedDataMessage.mGuild.mName;

	if( ! *name )
	{
		CString textThereIsNoResult;
		textThereIsNoResult.LoadString( IDS_STRING1 );

		MessageBox( textThereIsNoResult, "", MB_ICONERROR | MB_OK );
		return;
	}

	MSG_NAME_DWORD message;
	{
		ZeroMemory( &message, sizeof( message ) );

		message.Category	= MP_RM_GUILD;
		message.Protocol	= MP_RM_GUILD_GET_DATA_SYN;
		strncpy( message.Name, name, sizeof( message.Name ) );
	}

	mApplication.Send( mServerIndex, message, sizeof( message ) );
}


void CPlayerDialog::OnBnClickedPlayerUserViewButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString userName;
	mUserNameEdit.GetWindowText( userName );

	CUserDialog* dialog = mApplication.GetUserDialog();
	ASSERT( dialog );

	dialog->Request( mServerIndex, userName, mUserIndex );
}


void CPlayerDialog::OnBnClickedPlayerJobButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CJobLogDialog* dialog = mApplication.GetJobLogDialog();
	ASSERT( dialog );

	dialog->DoModal( mApplication.GetServerName( mServerIndex ), mPlayerName, mPlayerIndex );
}


void CPlayerDialog::OnBnClickedPlayerItemLogButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CItemLogDialog* dialog = mApplication.GetItemLogDialog();
	ASSERT( dialog );

	dialog->SetPlayerIndex( mServerIndex, mPlayerIndex );
	dialog->ShowWindow( SW_SHOW );
}


void CPlayerDialog::OnBnClickedPlayerNameUpdateButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	MSG_RM_PLAYER_EXTENDED_DATA message = mReceivedExtendedMessage;
	{
		message.dwObjectID	= mPlayerIndex;

		const CString name		( mExtendedDataGrid.GetItemText( 0, 1 ) );
		const CString guildNick	( mExtendedDataGrid.GetItemText( 1, 1 ) );
		const CString familyNick( mExtendedDataGrid.GetItemText( 2, 1 ) );

		_tcscpy( message.mPlayer.mName,			name );
		_tcscpy( message.mPlayer.mGuildNick,	guildNick );
		_tcscpy( message.mPlayer.mFamilyNick,	familyNick );

		if( ! memcmp( &mReceivedExtendedMessage, &message, sizeof( message ) ) )
		{
			return;
		}
	}

	message.Protocol	= MP_RM_PLAYER_SET_EXTENDED_DATA_SYN;
	message.mPlayer.mIndex	= mPlayerIndex;
	
	mApplication.Send( mServerIndex, message, sizeof( message ) );
}


void CPlayerDialog::OnBnClickedPlayerJobSubmitButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	MSG_RM_PLAYER_DATA message = mReceivedDataMessage;
	
	{
		// 이해를 위해서는 클래스 설정 참조할것
		MSG_RM_PLAYER_DATA::Job& job = message.mJob;

		//job.mIndex	= mJob1Combo.GetCurSel();	// 계열
		job.mData1	= mReceivedDataMessage.mJob.mData1;
		job.mData2	= mJob2Combo.GetCurSel();	// 2차 직업: 계열에 따라 인덱스로 정해짐
		job.mData3	= mJob3Combo.GetCurSel();	// 3차 직업
		job.mData4	= mJob4Combo.GetCurSel();	// 4차 직업
		job.mData5	= mJob5Combo.GetCurSel();	// 5차 직업
		job.mData6	= mJob6Combo.GetCurSel();	// 6차 직업

		// 계열 차수. 값이 있는 필드 위치를 알려준다.
		job.mIndex = 1 + ( job.mData2 ? 1 : 0 ) + ( job.mData3 ? 1 : 0 ) + ( job.mData4 ? 1 : 0 ) + ( job.mData5 ? 1 : 0 ) ;
		
		const DWORD maxJobSize	= 6;
		BOOL		isVaild		= FALSE;

		// 중간에 빈 값이 있는지 체크한다
		// 스위치 문을 goto처럼 사용하여 각 직업 차수에 값이 있는지 체크한다.
		switch( maxJobSize - job.mIndex )
		{
		case 0:
			if( ! job.mData6 )
			{
				break;
			}
		case 1:
			if( ! job.mData5 )
			{
				break;
			}
		case 2:
			if( ! job.mData4 )
			{
				break;
			}
		case 3:
			if( ! job.mData3 )
			{
				break;
			}
		case 4:
			if( ! job.mData2 )
			{
				break;
			}
		case 5:
			if( ! job.mData1 )
			{
				break;
			}

			isVaild = TRUE;
		}

		if( ! isVaild )
		{
			CString textFillAllData;
			textFillAllData.LoadString( IDS_STRING185 );

			MessageBox( textFillAllData, "", MB_ICONERROR | MB_OK );

			Parse( mServerIndex, &mReceivedDataMessage );
			return;
		}
	}

	// 변경된 내역이 없으면 바꾸지 않는다
	if( ! memcmp( &message, &mReceivedDataMessage, sizeof( message ) ) )
	{
		return;
	}

	CString textWarningBeforeChangingJob;
	textWarningBeforeChangingJob.LoadString( IDS_STRING186 );
    
	if( IDNO == MessageBox( textWarningBeforeChangingJob, "", MB_ICONINFORMATION | MB_OK ) )
	{
		return;
	}
	
	message.Protocol	= MP_RM_PLAYER_SET_DATA_SYN;
	message.dwObjectID	= mPlayerIndex;
	
	mApplication.Send( mServerIndex, message, sizeof( message ) );
}


void CPlayerDialog::OnCbnSelchangePlayerJob1Combo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 첫번째 직업은 무조건 선택되어야 한다
	if( ! mJob1Combo.GetCurSel() )
	{
		mJob1Combo.SetCurSel( 1 );
		return;
	}
}

void CPlayerDialog::OnCbnSelchangePlayerJob2Combo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 이전 직업이 선택되어야 변경 가능
	if( ! mJob1Combo.GetCurSel() )
	{
		mJob2Combo.SetCurSel( 0 );
		return;
	}
	else if( ! mJob2Combo.GetCurSel() )
	{
		mJob3Combo.SetCurSel( 0 );
		mJob4Combo.SetCurSel( 0 );
		mJob5Combo.SetCurSel( 0 );
		mJob6Combo.SetCurSel( 0 );
	}
}

void CPlayerDialog::OnCbnSelchangePlayerJob3Combo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 이전 직업이 선택되어야 변경 가능
	if( ! mJob2Combo.GetCurSel() )
	{
		mJob3Combo.SetCurSel( 0 );
		return;
	}
	else if( ! mJob3Combo.GetCurSel() )
	{
		mJob4Combo.SetCurSel( 0 );
		mJob5Combo.SetCurSel( 0 );
		mJob6Combo.SetCurSel( 0 );
	}
}

void CPlayerDialog::OnCbnSelchangePlayerJob4Combo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 이전 직업이 선택되어야 변경 가능
	if( ! mJob3Combo.GetCurSel() )
	{
		mJob4Combo.SetCurSel( 0 );
		return;
	}
	else if( ! mJob4Combo.GetCurSel() )
	{
		mJob5Combo.SetCurSel( 0 );
		mJob6Combo.SetCurSel( 0 );
	}
}

void CPlayerDialog::OnCbnSelchangePlayerJob5Combo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 이전 직업이 선택되어야 변경 가능
	if( ! mJob4Combo.GetCurSel() )
	{
		mJob5Combo.SetCurSel( 0 );
		return;
	}
	else if( ! mJob5Combo.GetCurSel() )
	{
		mJob6Combo.SetCurSel( 0 );
	}
}

void CPlayerDialog::OnCbnSelchangePlayerJob6Combo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 이전 직업이 선택되어야 변경 가능
	if( ! mJob5Combo.GetCurSel() )
	{
		mJob6Combo.SetCurSel( 0 );
		return;
	}
}


void CPlayerDialog::OnTcnSelchangePlayerDataTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	mDataGrid.ShowWindow( SW_HIDE );
	mExtendedDataGrid.ShowWindow( SW_HIDE );

	mDataSubmitButton.ShowWindow( SW_HIDE );
	mExtendedDataSubmitButton.ShowWindow( SW_HIDE );	

	switch( mTabCtrl.GetCurSel() )
	{
	case 0:
		{
			mDataGrid.ShowWindow( SW_SHOW );
			mDataSubmitButton.ShowWindow( SW_SHOW );
			break;
		}
	case 1:
		{
			mExtendedDataGrid.ShowWindow( SW_SHOW );
			mExtendedDataSubmitButton.ShowWindow( SW_SHOW );
			break;
		}
	}
}


void CPlayerDialog::OnBnClickedPlayerNameLogButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CNameLogDialog* dialog = mApplication.GetNameLogDialog();
	ASSERT( dialog );

	dialog->DoModal( mServerIndex, mPlayerIndex );
}
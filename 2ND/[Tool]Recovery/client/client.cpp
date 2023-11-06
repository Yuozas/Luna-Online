// client.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "client.h"
#include "MainFrm.h"
#include "LoginDialog.h"
#include "NewUserDialog.h"
#include "Network.h"
#include "PlayerDialog.h"
#include "ItemDialog.h"
#include "WaitDialog.h"
#include "SkillDialog.h"
#include "ItemFindDialog.h"
#include "FamilyDialog.h"
#include "QuestDialog.h"
#include "QuestLogDialog.h"
#include "GuildDialog.h"
#include "ExperienceLogDialog.h"
#include "ItemLogDialog.h"
#include "StatLogDialog.h"
#include "SkillLogDialog.h"
#include "ItemOptionLogDialog.h"
#include "GuildLogDialog.h"
#include "GuildWarehouseLogDialog.h"
#include "UserDialog.h"
#include "JobLogDialog.h"
#include "GuildScoreLogDialog.h"
#include "FamilyCropLogDialog.h"
#include "FamilyPointLogDialog.h"
#include "ItemAddDialog.h"
#include "PermissionDialog.h"
#include "OperatorDialog.h"
#include "PasswordDialog.h"
#include "OperatorLogDialog.h"
#include "NameLogDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CclientApp

BEGIN_MESSAGE_MAP(CclientApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, CclientApp::OnFileNew)
	ON_COMMAND(ID_MENU_NEWUSER_DIALOG, OnMenuNewuserDialog)
	ON_COMMAND(ID_MENU_ITEM_SEARCH_DIALOG, OnMenuItemSearchDialog)
	ON_COMMAND(ID_MENU_GUILD_DIALOG, OnMenuGuildDialog)
	ON_COMMAND(ID__MENU_ITEM_LOG_DIALOG, OnMenuItemLogDialog)
	ON_COMMAND(ID_MENU_ITEM_OPTION_LOG, OnMenuItemOptionLogDialog)
	ON_COMMAND(ID_MENU_FAMILY_DIALOG, OnMenuFamliyDialog)
	ON_COMMAND(ID_MENU_PERMISSION_DIALOG, OnMenuPermissionDialog)
	ON_COMMAND(ID_MENU_OPERATOR_DIALOG, OnMenuOperatorDialog)
	ON_COMMAND(ID_MENU_PASSWORD_DIALOG, OnMenuPasswordDialog)
	ON_COMMAND(ID_MENU_OPERATOR_LOG, OnMenuOperatorLogDialog)	
END_MESSAGE_MAP()


// CclientApp 생성

CclientApp::CclientApp() :
mIsWaiting( FALSE )
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
	LoadItemScript();
	LoadEnchantScript();
	LoadSkillScript();
	LoadQuestScript();
	LoadQuestItemScript();
	LoadQuestStringScript();
	LoadInterfaceMessage();
	LoadLicenseScript();
	LoadTip();
	LoadFarmScript();
}


CclientApp::~CclientApp()
{
	for(
		DialogList::iterator it = mDialogList.begin();
		mDialogList.end() != it;
		++it )
	{
		CDialog* dialog = *it;

		SAFE_DELETE( dialog );
	}
}


// 유일한 CclientApp 개체입니다.

CclientApp theApp;

// CclientApp 초기화

BOOL CclientApp::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControls()가 필요합니다. 
	// InitCommonControls()를 사용하지 않으면 창을 만들 수 없습니다.
	InitCommonControls();

	CWinApp::InitInstance();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));
	LoadStdProfileSettings(4);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.
	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// create and load the frame with its resources
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);
	// The one and only window has been initialized, so show and update it
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand

	// 리스트 컨테이너에 담아 소멸에 대한 부담을 줄이자
	{
		mDialogList.push_back( mWaitDialog				= new CWaitDialog);
		mDialogList.push_back( mLoginDialog				= new CLoginDialog( *this ) );
		mDialogList.push_back( mNewUserDialog			= new CNewUserDialog( *this ) );
		mDialogList.push_back( mPlayerDialog 			= new CPlayerDialog( *this ) );
		mDialogList.push_back( mItemDialog 				= new CItemDialog( *this ) );
		mDialogList.push_back( mSkillDialog 			= new CSkillDialog( *this ) );
		mDialogList.push_back( mItemFindDialog 			= new CItemFindDialog( *this ) );
		mDialogList.push_back( mFamilyDialog 			= new CFamilyDialog( *this ) );
		mDialogList.push_back( mQuestDialog 			= new CQuestDialog( *this ) );
		mDialogList.push_back( mQuestLogDialog 			= new CQuestLogDialog( *this ) );
		mDialogList.push_back( mExperienceLogDialog 	= new CExperienceLogDialog( *this ) );
		mDialogList.push_back( mItemLogDialog 			= new CItemLogDialog( *this ) );
		mDialogList.push_back( mStatLogDialog 			= new CStatLogDialog( *this ) );
		mDialogList.push_back( mSkillLogDialog 			= new CSkillLogDialog( *this ) );
		mDialogList.push_back( mItemOptionLogDialog		= new CItemOptionLogDialog( *this ) );
		mDialogList.push_back( mGuildDialog 	 		= new CGuildDialog( *this ) );
		mDialogList.push_back( mGuildLogDialog 		 	= new CGuildLogDialog( *this ) );
		mDialogList.push_back( mGuildWarehouseLogDialog = new CGuildWarehouseLogDialog( *this ) );
		mDialogList.push_back( mUserDialog				= new CUserDialog( *this ) );
		mDialogList.push_back( mJobLogDialog 			= new CJobLogDialog( *this ) );
		mDialogList.push_back( mGuildScoreLogDialog 	= new CGuildScoreLogDialog( *this ) );
		mDialogList.push_back( mFamilyCropLogDialog 	= new CFamilyCropLogDialog( *this ) );
		mDialogList.push_back( mFamilyPointLogDialog 	= new CFamilyPointLogDialog( *this ) );
		//mDialogList.push_back( mUserCreateDialog 		= new CUserCreateDialog( *this ) );
		//mDialogList.push_back( mUserRestoreDialog 		= new CUserRestoreDialog( *this ) );
		mDialogList.push_back( mPermissionDialog 		= new CPermissionDialog( *this ) );
		mDialogList.push_back( mOperatorDialog			= new COperatorDialog( *this ) );
		mDialogList.push_back( mOperatorLogDialog		= new COperatorLogDialog( *this ) );
		mDialogList.push_back( mNameLogDialog			= new CNameLogDialog( *this ) );
	}

	// 모달 속성이 아닌 윈도우만 Create()를 호출해준다
	{
		mNewUserDialog->Create( IDD_NEWUSERDIALOG );
		mPlayerDialog->Create( IDD_PLAYERDIALOG );
		mItemDialog->Create( IDD_ITEMDIALOG );
		mSkillDialog->Create( IDD_SKILLDIALOG );
		mItemFindDialog->Create( IDD_ITEMFINDDIALOG );
		mFamilyDialog->Create( IDD_FAMILYDIALOG );
		mQuestDialog->Create( IDD_QUESTDIALOG );
		mGuildDialog->Create( IDD_GUILDDIALOG );
		mItemLogDialog->Create( IDD_ITEMLOGDIALOG );
		mItemOptionLogDialog->Create( IDD_ITEMOPTIONLOGDIALOG );
		mUserDialog->Create( IDD_USERDIALOG );
		mPermissionDialog->Create( IDD_PERMISSIONDIALOG );
		mOperatorDialog->Create( IDD_OPERATORDIALOG );
	}

	// 맨 마지막에 실행되어야 한다
	{
		if( IDCANCEL == mLoginDialog->DoModal() )
		{
			return FALSE;
		}
	}

	return TRUE;
}


// CclientApp 메시지 처리기

int CclientApp::ExitInstance() 
{
	//TODO: 추가한 추가 리소스를 처리합니다.
	if (m_hMDIMenu != NULL)
		FreeResource(m_hMDIMenu);
	if (m_hMDIAccel != NULL)
		FreeResource(m_hMDIAccel);

	return CWinApp::ExitInstance();
}


void CclientApp::OnFileNew() 
{
	// 플레이어가 사용자를 선택하고 그 세부 데이터가 오면 자식창을 만들어 표시한다

	//CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
	// 새 MDI 자식 창을 만듭니다.
	//pFrame->CreateNewChild(
	//	//RUNTIME_CLASS(CChildFrame), IDR_clientTYPE, m_hMDIMenu, m_hMDIAccel);
	//	RUNTIME_CLASS(CPlayerFormView), IDR_clientTYPE, m_hMDIMenu, m_hMDIAccel);

	//if( mNewUserDialog )
	//{
	//	mNewUserDialog->DoModal();
	//}	

	// TODO: change to SDI at rest
	//CWinApp::OnFileNew();

	//CPlayerDialog dialog;

	//dialog.DoModal();
}


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원

// 구현
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CclientApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


BOOL CclientApp::PreTranslateMessage( MSG* message) 
{
	switch( message->message )
	{
	case WM_SOCKEVENTMSG:
		{
			ISC_BaseNetwork* lpNet = CNetwork::GetInstance().GetBaseNetwork();

			if( lpNet )
			{
				lpNet->SocketEventProc();
			}

			return TRUE;
		}
		break;
	}

	return CWinApp::PreTranslateMessage( message );
}


CDialog* CclientApp::GetWaitDialog()
{
	return mWaitDialog;
}


CNewUserDialog*	CclientApp::GetNewUserDialog()
{
	return mNewUserDialog;
}


CItemDialog* CclientApp::GetItemDialog()
{
	return mItemDialog;
}


CSkillDialog* CclientApp::GetSkillDialog()
{
	return mSkillDialog;
}


CPlayerDialog* CclientApp::GetPlayerDialog()
{
	return mPlayerDialog;
}


CQuestDialog* CclientApp::GetQuestDialog()
{
	return mQuestDialog;
}


CQuestLogDialog* CclientApp::GetQuestLogDialog()
{
	return mQuestLogDialog;
}


CFamilyDialog* CclientApp::GetFamilyDialog()
{
	return mFamilyDialog;
}


CExperienceLogDialog* CclientApp::GetExperienceDialog()
{
	return mExperienceLogDialog;
}


CStatLogDialog* CclientApp::GetStatLogDialog()
{
	return mStatLogDialog;
}


CSkillLogDialog* CclientApp::GetSkillLogDialog()
{
	return mSkillLogDialog;
}


CItemLogDialog* CclientApp::GetItemLogDialog()
{
	return mItemLogDialog;
}


CItemOptionLogDialog* CclientApp::GetItemOptionLogDialog()
{
	return mItemOptionLogDialog;
}


CGuildLogDialog* CclientApp::GetGuildLogDialog()
{
	return mGuildLogDialog;
}


CGuildWarehouseLogDialog* CclientApp::GetGuildItemLogDialog()
{
	return mGuildWarehouseLogDialog;
}


CUserDialog* CclientApp::GetUserDialog()
{
	return mUserDialog;
}


CJobLogDialog* CclientApp::GetJobLogDialog()
{
	return mJobLogDialog;
}


CGuildScoreLogDialog* CclientApp::GetGuildScoreLogDialog()
{
	return mGuildScoreLogDialog;
}


CFamilyCropLogDialog* CclientApp::GetFamilyCropLogDialog()
{
	return mFamilyCropLogDialog;
}


CFamilyPointLogDialog* CclientApp::GetFamilyPointLogDialog()
{
	return mFamilyPointLogDialog;
}


CPermissionDialog* CclientApp::GetPermissionDialog()
{
	return mPermissionDialog;
}


CNameLogDialog* CclientApp::GetNameLogDialog()
{
	return mNameLogDialog;
}


void CclientApp::Parse( DWORD serverIndex, const MSGROOT* message )
{
	if( mWaitDialog->GetSafeHwnd() )
	{
		mWaitDialog->SendMessage( WM_CLOSE );

		mIsWaiting = FALSE;
	}

	switch( message->Category )
	{
	case MP_RM_CONNECT:
		{
			mLoginDialog->Parse( serverIndex, message );
			break;
		}
	case MP_RM_SEARCH:
		{
			mNewUserDialog->Parse( serverIndex, message );
			break;
		}
	case MP_RM_PLAYER:
		{
			mPlayerDialog->Parse( serverIndex, message );
			mPlayerDialog->SetWindowPos( &CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE );
			mPlayerDialog->ShowWindow( SW_SHOW );
			break;
		}
	case MP_RM_ITEM:
		{
			mItemDialog->Parse( serverIndex, message );
			mItemDialog->SetWindowPos( &CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE );
			mItemDialog->ShowWindow( SW_SHOW );
			break;
		}
	case MP_RM_ITEM_FIND:
		{
			mItemFindDialog->Parse( serverIndex, message );
			break;
		}
	case MP_RM_SKILL:
		{
			mSkillDialog->Parse( serverIndex, message );
			mSkillDialog->SetWindowPos( &CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE );
			mSkillDialog->ShowWindow( SW_SHOW );
			break;
		}
	case MP_RM_FAMILY:
		{
			mFamilyDialog->Parse( serverIndex, message );
			mFamilyDialog->SetWindowPos( &CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE );
			mFamilyDialog->ShowWindow( SW_SHOW );
			break;
		}
	case MP_RM_QUEST:
		{
			mQuestDialog->Parse( serverIndex, message );
			mQuestDialog->SetWindowPos( &CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE );
			mQuestDialog->ShowWindow( SW_SHOW );
			break;
		}
	case MP_RM_QUEST_LOG:
		{
			if( mQuestLogDialog->GetSafeHwnd() )
			{
				mQuestLogDialog->Parse( serverIndex, message );
			}

			break;
		}
	case MP_RM_GUILD:
		{
			mGuildDialog->Parse( serverIndex, message );
			mGuildDialog->SetWindowPos( &CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE );
			mGuildDialog->ShowWindow( SW_SHOW );
			break;
		}
	case MP_RM_EXP_LOG:
		{
			if( mExperienceLogDialog->GetSafeHwnd() )
			{
				mExperienceLogDialog->Parse( serverIndex, message );
			}
			
			break;
		}
	case MP_RM_ITEM_LOG:
		{
			if( mItemLogDialog->GetSafeHwnd() )
			{
				mItemLogDialog->Parse( serverIndex, message );
			}
			
			break;
		}
	case MP_RM_STAT_LOG:
		{
			if( mStatLogDialog->GetSafeHwnd() )
			{
				mStatLogDialog->Parse( serverIndex, message );
			}
			
			break;
		}
	case MP_RM_SKILL_LOG:
		{
			if( mSkillLogDialog->GetSafeHwnd() )
			{
				mSkillLogDialog->Parse( serverIndex, message );
			}
			
			break;
		}
	case MP_RM_ITEM_OPTION_LOG:
		{
			if( mItemOptionLogDialog->GetSafeHwnd() )
			{
				mItemOptionLogDialog->Parse( serverIndex, message );
			}
			
			break;
		}
	case MP_RM_GUILD_LOG:
		{
			if( mGuildLogDialog->GetSafeHwnd() )
			{
				mGuildLogDialog->Parse( serverIndex, message );
			}
			
			break;
		}
	case MP_RM_GUILD_WAREHOUSE_LOG:
		{
			if( mGuildWarehouseLogDialog->GetSafeHwnd() )
			{
				mGuildWarehouseLogDialog->Parse( serverIndex, message );
			}
			
			break;
		}
	case MP_RM_USER:
		{
			mUserDialog->Parse( serverIndex, message );
			mUserDialog->SetWindowPos( &CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE );
			mUserDialog->ShowWindow( SW_SHOW );
			break;
		}
	//case MP_RM_USER_CREATE:
	//	{
	//		mUserCreateDialog->Parse( serverIndex, message );
	//		mUserCreateDialog->SetWindowPos( &CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE );
	//		mUserCreateDialog->ShowWindow( SW_SHOW );
	//		break;
	//	}
	//case MP_RM_USER_RESTORE:
	//	{
	//		mUserRestoreDialog->Parse( serverIndex, message );
	//		mUserRestoreDialog->SetWindowPos( &CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE );
	//		mUserRestoreDialog->ShowWindow( SW_SHOW );
	//		break;
	//	}
	case MP_RM_JOB_LOG:
		{
			if( mJobLogDialog->GetSafeHwnd() )
			{
				mJobLogDialog->Parse( serverIndex, message );
			}

			break;
		}
	case MP_RM_GUILD_SCORE_LOG:
		{
			if( mGuildScoreLogDialog->GetSafeHwnd() )
			{
				mGuildScoreLogDialog->Parse( serverIndex, message );
			}
			
			break;
		}
	case MP_RM_FAMILY_CROP_LOG:
		{
			if( mFamilyCropLogDialog->GetSafeHwnd() )
			{
				mFamilyCropLogDialog->Parse( serverIndex, message );
			}
			
			break;
		}
	case MP_RM_FAMILY_POINT_LOG:
		{
			if( mFamilyPointLogDialog->GetSafeHwnd() )
			{
				mFamilyPointLogDialog->Parse( serverIndex, message );
			}
			
			break;
		}
	case MP_RM_PERMISSION:
		{
			mPermissionDialog->ShowWindow( SW_SHOW );
			mPermissionDialog->SetWindowPos( &CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE );
			mPermissionDialog->Parse( serverIndex, message );
			break;
		}
	case MP_RM_OPERATOR:
		{
			mOperatorDialog->ShowWindow( SW_SHOW );
			mOperatorDialog->SetWindowPos( &CWnd::wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE );
			mOperatorDialog->Parse( serverIndex, message );
			break;
		}
	case MP_RM_OPERATOR_LOG:
		{
			if( mOperatorLogDialog->GetSafeHwnd() )
			{
				mOperatorLogDialog->Parse( serverIndex, message );
			}
			
			break;
		}
	case MP_RM_NAME_LOG:
		{
			if( mNameLogDialog->GetSafeHwnd() )
			{
				mNameLogDialog->Parse( serverIndex, message );
			}

			break;
		}
	case MP_RM_PASSWORD:
		{
			const MSGROOT* m = ( MSGROOT* )message;

			switch( m->Protocol )
			{
			case MP_RM_PASSWORD_SET_ACK:
				{
					CString textUpdateWasSucceed;
					textUpdateWasSucceed.LoadString( IDS_STRING28 );

					MessageBox( 0, textUpdateWasSucceed, "", MB_OK | MB_ICONINFORMATION );
					break;
				}
			case MP_RM_PASSWORD_SET_NACK:
				{
					CString textUpdateWasSucceed;
					textUpdateWasSucceed.LoadString( IDS_STRING29 );

					MessageBox( 0, textUpdateWasSucceed, "", MB_OK | MB_ICONERROR );
					break;
				}
			default:
				{
					ASSERT( 0 );
					break;
				}
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


void CclientApp::LoadItemScript()
{
	// read from itemlist.bin

	CMHFile file;

	if(! file.Init("System/Resource/ItemList.bin", "rb"))
	{
		ASSERT( 0 );
		return;
	}

	while( ! file.IsEOF() )
	{
		const DWORD index = file.GetDword();

		if( mItemMap.end() != mItemMap.find( index ) )
		{
			ASSERT( 0 );
			continue;
		}

		ITEM_INFO& item = mItemMap[ index ];
		ZeroMemory( &item, sizeof( item ) );

		// extract from script
		{
			item.ItemIdx = index;

			{
				CString text( file.GetString() );
				CString token;
				CString name;

				int start = 0;

				do 
				{
					token = text.Tokenize( "^s", start );

					name += token + " ";
					
				} while( ! token.IsEmpty() );

				StringCopySafe( item.ItemName, name, sizeof( item.ItemName ) );
			}
			
			item.ItemTooltipIdx = file.GetWord();
			item.Image2DNum = file.GetWord();	
			item.Part3DType = file.GetWord();
			item.Part3DModelNum = file.GetWord();	

			item.Shop = file.GetWord(); // 아이템몰 여부
			item.Category = file.GetWord();			

			item.Grade = file.GetWord();			

			item.LimitRace = file.GetWord();
			item.LimitGender = file.GetWord();		
			item.LimitLevel = file.GetLevel();	

			item.Stack = file.GetWord();
			item.Trade = file.GetWord();
			item.Deposit = file.GetWord();
			item.Sell = file.GetWord();
			item.Decompose = file.GetWord();

			item.DecomposeIdx = file.GetDword();
			item.Time = file.GetDword();

			item.BuyPrice = file.GetDword();			
			item.SellPrice = file.GetDword();			

			item.EquipType = file.GetWord();
			item.EquipSlot = file.GetWord() - 1;

			item.Durability = file.GetDword();
			file.GetDword();

			item.Repair = file.GetWord();

			item.WeaponAnimation = file.GetWord();
			item.WeaponType = file.GetWord();		

			item.PhysicAttack = file.GetWord();
			item.MagicAttack = file.GetWord();

			item.ArmorType = file.GetWord();
			item.AccessaryType = file.GetWord();

			item.PhysicDefense = file.GetWord();
			item.MagicDefense = file.GetWord();

			item.IsEnchant		= file.GetBool();
			item.Improvement		= file.GetWord();

			item.EnchantValue		= file.GetDword();
			item.EnchantDeterm	= file.GetDword();

			item.ImprovementStr = file.GetWord();			
			item.ImprovementDex = file.GetWord();			
			item.ImprovementVit = file.GetWord();		//10
			item.ImprovementInt = file.GetWord();
			item.ImprovementWis = file.GetWord();			
			item.ImprovementLife = file.GetDword();			
			item.ImprovementMana = file.GetWord();			

			item.ImprovementSkill = file.GetDword();
			item.Skill = file.GetDword();
			item.SupplyType	= file.GetWord();			
			item.SupplyValue	= file.GetDword();
			item.kind			= ITEM_INFO::eKind( file.GetDword() );

			item.RequiredBuff = file.GetDword();
			item.Battle = file.GetBool();
			item.Peace = file.GetBool();
			item.Move = file.GetBool();
			item.Stop = file.GetBool();
			item.Rest = file.GetBool();

			item.wSeal			= file.GetWord();
			item.nTimeKind		= file.GetInt();
			item.dwUseTime		= file.GetDword();
			//item.RequiredBuff	= file.GetDword();

			//char buffer[ MAX_PATH * 5 ];
			//file.GetLine( buffer, sizeof( buffer ) );
		}
	}
}


void CclientApp::LoadEnchantScript()
{
	CMHFile file;

	if( ! file.Init( "system/resource/itemEnchant.bin", "rb" ) )
	{
		return;
	}

	char buffer[ MAX_PATH * 5 ];

	enum Type
	{
		TypeNone,
		TypeProtection,
		TypeResult,
	}
	type = TypeNone;

	while( ! file.IsEOF() )
	{
		file.GetLine( buffer, sizeof( buffer ) );

		const int length = int( _tcslen( buffer ) );

		if( ! length )
		{
			continue;
		}

		// 중간의 주석 제거
		for( int i = 0; i < length - 1; ++i )
		{
			const char a = buffer[ i ];
			const char b = buffer[ i + 1 ];

			if( '/' == a &&
				'/' == b )
			{
				buffer[ i ] = 0;
				break;
			}
		}

		const char*		separator	= "~\n\t(), ";
		const char*		token		= strtok( buffer, separator );

		if( ! token )
		{
			continue;
		}
		else if( ! stricmp( "#protection", token ) )
		{
			type = TypeProtection;
			continue;
		}
		else if( ! stricmp( "#result", token ) )
		{
			type = TypeResult;
			continue;
		}

		switch( type )
		{
		case TypeProtection:
			{
				//const DWORD itemIndex = atoi( token );

				//mEnchantProtectionItemSet.insert( itemIndex );
				break;
			}
		case TypeResult:
			{
				const DWORD itemIndex = atoi( token );

				ASSERT( mEnchantScriptMap.end() == mEnchantScriptMap.find( itemIndex ) );

				EnchantScript& script = mEnchantScriptMap[ itemIndex ];

				script.mItemIndex		= itemIndex;
				script.mItemLevel.mMin	= atoi( strtok( 0, separator ) );
				script.mItemLevel.mMax	= atoi( strtok( 0, separator ) );
				script.mEnchantMaxLevel	= atoi( strtok( 0, separator ) );

				for(
					DWORD abilitySize = 2;
					abilitySize--;
				)
					// 강화 가능한 능력 설정
				{
					token = strtok( 0, separator );

					if( ! strcmpi( "strength", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeStrength );
					}
					else if( ! strcmpi( "dexterity", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeDexterity );
					}
					else if( ! strcmpi( "vitality", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeVitality );
					}
					else if( ! strcmpi( "intelligence", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeIntelligence );
					}
					else if( ! strcmpi( "wisdom", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeWisdom );
					}
					else if( ! strcmpi( "mana", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeMana );
					}
					else if( ! strcmpi( "recovery_mana", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeManaRecovery );
					}
					else if( ! strcmpi( "life", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeLife );
					}
					else if( ! strcmpi( "recovery_life", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeLifeRecovery );
					}
					else if( ! strcmpi( "physic_defense", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypePhysicDefence );
					}
					else if( ! strcmpi( "physic_attack", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypePhysicAttack );
					}
					else if( ! strcmpi( "magic_defense", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeMagicDefence );
					}
					else if( ! strcmpi( "magic_attack", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeMagicAttack );
					}
					else if( ! strcmpi( "move_speed", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeMoveSpeed );
					}
					else if( ! strcmpi( "evade", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeEvade );
					}
					else if( ! strcmpi( "accuracy", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeAccuracy );
					}
					else if( ! strcmpi( "critical_rate", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeCriticalRate );
					}
					else if( ! strcmpi( "critical_damage", token ) )
					{
						script.mAbility.insert( EnchantScript::eTypeCriticalDamage );
					}
					else if( ! strcmpi( "none", token ) )
					{
						// to do nothing
					}
					else
					{
						mEnchantScriptMap.erase( itemIndex );
					}
				}

				// 업글 가능한 아이템 종류 세팅
				{
					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_hand );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Shield );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Armor );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Helmet );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Glove );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Boots );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Ring1 );
						script.mSlot.insert( eEquipSlot_Ring2 );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Necklace );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Earring1 );
						script.mSlot.insert( eEquipSlot_Earring2 );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Belt );
					}

					if( const std::string( "1" ) == strtok( 0, separator ) )
					{
						script.mSlot.insert( eEquipSlot_Band );
					}
				}

				break;
			}
		}
	}
}


const ITEM_INFO* CclientApp::GetItemScript( DWORD itemIndex ) const
{
	ItemMap::const_iterator it = mItemMap.find( itemIndex );

	return mItemMap.end() == it ? 0 : &it->second;
}


const char* CclientApp::GetItemName( DWORD itemIndex ) const
{
	const ITEM_INFO* item = GetItemScript( itemIndex );

	return item ? item->ItemName : "?";
}


const EnchantScript* CclientApp::GetEnchantScript( DWORD itemIndex ) const
{
	EnchantScriptMap::const_iterator it = mEnchantScriptMap.find( itemIndex );

	return mEnchantScriptMap.end() == it ? 0 : &( it->second );
}


DWORD CclientApp::GetEnchantItemIndex( DWORD itemIndex ) const
{
	const ITEM_INFO* item = GetItemScript( itemIndex );

	if( ! item )
	{
		return 0;
	}

	const eEquipSlot slot = eEquipSlot( item->EquipSlot + 1 );

	for( EnchantScriptMap::const_iterator it = mEnchantScriptMap.begin(); mEnchantScriptMap.end() != it; ++it )
	{
		const EnchantScript& script = it->second;

		if( script.mSlot.end() != script.mSlot.find( slot ) )
		{
			return it->first;
		}
	}

	return 0;
}


BOOL CclientApp::IsDuplicateItem( DWORD itemIndex ) const
{
	const ITEM_INFO* item = GetItemScript( itemIndex );

	//ASSERT( item );

	return item ? item->Stack: FALSE;
}


void CclientApp::CreateGrid( CWnd* parent, CGridCtrl* grid, UINT id, const CRect& rect, const CString* title, BOOL autoHeight )
{
	std::list< CString > titleList;

	// string must end with ""(empty string)
	for( ; ; ++title )
	{
		if( title->IsEmpty() )
		{
			break;
		}

		titleList.push_back( *title );
	}

	const DWORD rowHeight = 25;

	// make grid control
	{
		CRect gridRect( rect );

		if( autoHeight )
		{
			gridRect.bottom = gridRect.top + int( titleList.size() ) * rowHeight + 5;
		}

		grid->Create( gridRect, parent, id, AFX_WS_DEFAULT_VIEW );
		grid->SetEditable( TRUE );
		grid->SetListMode( FALSE );
		grid->EnableDragAndDrop( FALSE );
		grid->SetRowCount( int( titleList.size() ) );
		grid->SetColumnCount( 2 );
		//grid->SetFixedRowCount( 0 );
		grid->SetFixedColumnCount( 1 );
		//grid->SetItemState(1,1, grid->GetItemState(1,1) | GVIS_READONLY);
		grid->SetRowResize( FALSE );
		grid->SetColumnResize( FALSE );
	}

	// set grid with title
	{
		GV_ITEM	item	= { 0 };
		{
			item.mask			= GVIF_TEXT|GVIF_FORMAT;
			item.nFormat		= DT_LEFT|DT_WORDBREAK;
			item.row			= 0;
		}		

		for( std::list< CString >::const_iterator it = titleList.begin(); titleList.end() != it; ++it )
		{
			const CString& text = *it;

			item.strText.Format( "%s", text );

			grid->SetItem( &item );
			grid->SetRowHeight( item.row, rowHeight );

			++item.row;
		}
	}
}


void CclientApp::CreateGrid( CWnd* parent, CGridCtrl* grid, UINT id, const CRect& rect, const char** title, BOOL autoHeight )
{
	std::list< CString > titleList;

	// string must end with ""(empty string)
	for( ; ; ++title )
	{
		const CString text( *title );

		if( text.IsEmpty() )
		{
			break;
		}

		titleList.push_back( text );
	}

	const DWORD rowHeight = 25;

	// make grid control
	{
		CRect gridRect( rect );

		if( autoHeight )
		{
			gridRect.bottom = gridRect.top + int( titleList.size() ) * rowHeight + 5;
		}

		grid->Create( gridRect, parent, id, AFX_WS_DEFAULT_VIEW );
		grid->SetEditable( TRUE );
		grid->SetListMode( FALSE );
		grid->EnableDragAndDrop( FALSE );
		grid->SetRowCount( int( titleList.size() ) );
		grid->SetColumnCount( 2 );
		//grid->SetFixedRowCount( 0 );
		grid->SetFixedColumnCount( 1 );
		//grid->SetItemState(1,1, grid->GetItemState(1,1) | GVIS_READONLY);
		grid->SetRowResize( FALSE );
		grid->SetColumnResize( FALSE );
	}

	// set grid with title
	{
		GV_ITEM	item	= { 0 };
		{
			item.mask			= GVIF_TEXT|GVIF_FORMAT;
			item.nFormat		= DT_LEFT|DT_WORDBREAK;
			item.row			= 0;
		}		

		for( std::list< CString >::const_iterator it = titleList.begin(); titleList.end() != it; ++it )
		{
			const CString& text = *it;

			item.strText.Format( "%s", text );

			grid->SetItem( &item );
			grid->SetRowHeight( item.row, rowHeight );

			++item.row;
		}
	}
}


DWORD CclientApp::GetServerIndex( const char* serverName ) const
{
	return CNetwork::GetInstance().GetServerIndex( serverName );
}


CString CclientApp::GetServerName( DWORD serverIndex ) const
{
	return CNetwork::GetInstance().GetServerName( serverIndex );
}



//CString CclientApp::GetPlayerName() const
//{
//	ASSERT( mNewUserDialog );
//
//	return mNewUserDialog->GetPlayerName();
//}


//DWORD CclientApp::GetPlayerIndex() const
//{
//	ASSERT( mNewUserDialog );
//
//	return mNewUserDialog->GetPlayerIndex();
//}


void CclientApp::SetCellValue( DWORD value, CGridCtrl& grid, GV_ITEM& cell, int state )
{
	cell.strText.Format( "%d", value );
	grid.SetItem( &cell );
	grid.SetItemState( cell.row, cell.col, state );
}


void CclientApp::SetCellValue( const char* value, CGridCtrl& grid, GV_ITEM& cell, int state )
{
	cell.strText.Format( "%s", value );
	grid.SetItem( &cell );
	grid.SetItemState( cell.row, cell.col, state );
}


void CclientApp::SetWait( BOOL isWait )
{
	if( mIsWaiting = isWait &&
		! mWaitDialog->GetSafeHwnd() )
	{
		mWaitDialog->DoModal();
	}
	else
	{
		mWaitDialog->EndModalState();
	}	
}


void CclientApp::LoadSkillScript()
{
	// normal skill
	{
		CMHFile file;

		if( ! file.Init( "System/Resource/SkillList.bin","rb" ) )
		{
			ASSERT( 0 );
			return;
		}

		char buffer[ MAX_PATH * 5 ];

		while( ! file.IsEOF() )
		{
			file.GetLine( buffer, sizeof( buffer ) );

			const char* separator = "\t ";

			const char* token1 = strtok( buffer, separator );
			const char* token2 = strtok( 0, separator );

			if( ! token1 || ! token2 )
			{
				continue;
			}

			const DWORD skillIndex = atoi( token1 );

			if( mSkillScriptMap.end() != mSkillScriptMap.find( skillIndex ) )
			{
				ASSERT( 0 && "Duplicated index is found" );
				continue;
			}

			SkillScript& script = mSkillScriptMap[ skillIndex ];

			{
				CString text( token2 );
				CString token;
				CString name;

				int start = 0;

				do 
				{
					token = text.Tokenize( "^s", start );

					name += token + " ";

				} while( ! token.IsEmpty() );

				script.mName	= name;
			}

			script.mIndex		= skillIndex;
			script.mIsPassive	= FALSE;

			{
				const DWORD index = skillIndex / 100 * 100 + 1;

				++mSkillSizeMap[ index ];
			}

			// level
			strtok( 0, separator );

			// image
			strtok( 0, separator );
			// tip
			strtok( 0, separator );

			// operator effect
			strtok( 0, separator );
			// target effect
			strtok( 0, separator );
			// skill effect
			strtok( 0, separator );

			// animation time
			strtok( 0, separator );
			// casting time
			strtok( 0, separator );
			// cool time
			strtok( 0, separator );
			// delay time
			strtok( 0, separator );
			// delay type
			strtok( 0, separator );

			// battle
			strtok( 0, separator );
			// peace
			strtok( 0, separator );
			// move
			strtok( 0, separator );
			// stop
			strtok( 0, separator );
			// rest
			strtok( 0, separator );

			// required buff
			strtok( 0, separator );

			// target
			strtok( 0, separator );

			// kind
			strtok( 0, separator );
			// attribute
			strtok( 0, separator );

			// equip item #0
			strtok( 0, separator );
			// equip item #1
			strtok( 0, separator );

			// area
			strtok( 0, separator );
			// area target
			strtok( 0, separator );
			// area data #0
			strtok( 0, separator );
			// area data #1
			strtok( 0, separator );
			// area data #2
			strtok( 0, separator );

			// unit
			strtok( 0, separator );
			// unit data type
			strtok( 0, separator );
			// unit data
			strtok( 0, separator );

			// success
			strtok( 0, separator );
			// cancel
			strtok( 0, separator );

			// range
			strtok( 0, separator );
			
			// item count #0
			strtok( 0, separator );
			// item count #1
			strtok( 0, separator );
			// item #0
			strtok( 0, separator );
			// item #1			
			strtok( 0, separator );
			// money
			strtok( 0, separator );
			// mana
			strtok( 0, separator );
			// life
			strtok( 0, separator );

			// train point
			script.mPoint = atoi( strtok( 0, separator ) );
			// train money
			script.mMoney = atoi( strtok( 0, separator ) );
			
			// buff 0 .. MAX_BUFF_COUNT 
			for( int i = 0; i < MAX_BUFF_COUNT; ++i )
			{
				// buff
				const DWORD buffIndex	= atoi( strtok( 0, separator ) );

				// buff rate
				const DWORD buffRate	= atoi( strtok( 0, separator ) );

				if( buffIndex || buffRate )
				{
					script.mIsPassive = TRUE;
				}
			}			
		}
	}

	/*
	// buff skill
	{
		CMHFile file;

		if( ! file.Init("System/Resource/Skill_Buff_List.bin","rb") )
		{
			//ASSERTMSG(0,"SkillList를 로드하지 못했습니다.");
			return;
		}

		char buffer[ MAX_PATH * 5 ];

		while( ! file.IsEOF() )
		{
			file.GetLine( buffer, sizeof( buffer ) );

			const char* separator = "\t ";

			const char* token1 = strtok( buffer, separator );
			const char* token2 = strtok( 0, separator );

			if( ! token1 || ! token2 )
			{
				continue;
			}

			const DWORD index = atoi( token1 );

			if( mSkillScriptMap.end() != mSkillScriptMap.find( index ) )
			{
				ASSERT( 0 && "Duplicated index is found" );
				continue;
			}

			SkillScript& script = mSkillScriptMap[ index ];

			{
				CString text( token2 );
				CString token;
				CString name;

				int start = 0;

				do 
				{
					token = text.Tokenize( "^s", start );

					name += token + " ";

				} while( ! token.IsEmpty() );

				script.mName	= name;
			}

			script.mIndex		= index;
			script.mIsPassive	= TRUE;

			{
				const DWORD index = script.mIndex / 100 * 100 + 1;

				++mSkillSizeMap[ index ];
			}
		}
	}
	*/
}


const SkillScript* CclientApp::GetSkillScript( DWORD skillIndex ) const
{
	SkillScriptMap::const_iterator it = mSkillScriptMap.find( skillIndex );

	return mSkillScriptMap.end() == it ? 0 : &it->second;
}


DWORD CclientApp::GetSkillSize( DWORD skillIndex ) const
{
	const DWORD index = skillIndex / 100 * 100 + 1;

	SkillSizeMap::const_iterator it = mSkillSizeMap.find( index );

	return mSkillSizeMap.end() == it ? 0 : it->second;
}


void CclientApp::OnMenuNewuserDialog()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	mNewUserDialog->ShowWindow( mNewUserDialog->IsWindowVisible() ? SW_HIDE : SW_SHOW );
}


void CclientApp::OnMenuItemSearchDialog()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	mItemFindDialog->ShowWindow( mItemFindDialog->IsWindowVisible() ? SW_HIDE : SW_SHOW );
}


void CclientApp::OnMenuGuildDialog()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	mGuildDialog->ShowWindow( mGuildDialog->IsWindowVisible() ? SW_HIDE : SW_SHOW );
}


void CclientApp::OnMenuItemLogDialog()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	mItemLogDialog->ShowWindow( mItemLogDialog->IsWindowVisible() ? SW_HIDE : SW_SHOW );
}


void CclientApp::OnMenuItemOptionLogDialog()
{
	mItemOptionLogDialog->ShowWindow( mItemOptionLogDialog->IsWindowVisible() ? SW_HIDE : SW_SHOW );
}


void CclientApp::OnMenuFamliyDialog()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	mFamilyDialog->ShowWindow( mFamilyDialog->IsWindowVisible() ? SW_HIDE : SW_SHOW );
}


void CclientApp::OnMenuOperatorDialog()
{
	const CNetwork::ConnectionMap&	connection	= CNetwork::GetInstance().GetConnectionMap();

	CNetwork::ConnectionMap::const_iterator it = connection.begin();

	if( connection.end() == it )
	{
		CString textServerForRequestIsNotFound;
		textServerForRequestIsNotFound.LoadString( IDS_STRING227 );

		MessageBox( 0, textServerForRequestIsNotFound, "", MB_ICONERROR | MB_OK );
		return;
	}

	const DWORD	serverIndex = it->first;

	mOperatorDialog->Request( serverIndex );
	
	//mOperatorDialog->ShowWindow( mOperatorDialog->IsWindowVisible() ? SW_HIDE : SW_SHOW );	
}


void CclientApp::OnMenuPasswordDialog()
{
	DWORD serverIndex;
	{
		const CNetwork::ConnectionMap&	connection	= CNetwork::GetInstance().GetConnectionMap();

		CNetwork::ConnectionMap::const_iterator it = connection.begin();

		if( connection.end() == it )
		{
			CString textAnyServerNotFoundToRequest;
			textAnyServerNotFoundToRequest.LoadString( IDS_STRING227 );

			MessageBox( 0, textAnyServerNotFoundToRequest, "", MB_ICONERROR | MB_OK );
			return;
		}

		serverIndex = it->first;
	}

	CPasswordDialog dialog;

	if( IDOK != dialog.DoModal() )
	{
		return;
	}

	const CString previousPassword	= dialog.GetPreviousPassword();
	const CString nextPassword		= dialog.GetNextPassword();

	if( previousPassword.IsEmpty() ||
		nextPassword.IsEmpty() )
	{
		return;
	}

	{
		MSG_NAME2 message;

		message.Category	= MP_RM_PASSWORD;
		message.Protocol	= MP_RM_PASSWORD_SET_SYN;
		
		StringCopySafe( message.str1, previousPassword, sizeof( message.str1 ) );
		StringCopySafe( message.str2, nextPassword,		sizeof( message.str2 ) );

		Send( serverIndex, message, sizeof( message ) );
	}
}


void CclientApp::OnMenuOperatorLogDialog()
{
	ASSERT( mOperatorLogDialog );

	mOperatorLogDialog->DoModal();
}


void CclientApp::OnMenuPermissionDialog()
{
	const CNetwork::ConnectionMap&	connection	= CNetwork::GetInstance().GetConnectionMap();

	CNetwork::ConnectionMap::const_iterator it = connection.begin();

	if( connection.end() == it )
	{
		CString textAnyServerNotFoundToRequest;
		textAnyServerNotFoundToRequest.LoadString( IDS_STRING227 );

		MessageBox( 0, textAnyServerNotFoundToRequest, "", MB_ICONERROR | MB_OK );
		return;
	}

	const DWORD	serverIndex = it->first;
	
	ASSERT( mPermissionDialog );
	mPermissionDialog->Request( serverIndex );
}


const QuestScript* CclientApp::GetQuestScript( DWORD mainQuestIndex, DWORD subQuestIndex ) const
{
	const DWORD index = MAKELONG( mainQuestIndex, subQuestIndex );

	QuestScriptMap::const_iterator it = mQuestScriptMap.find( index );

	return mQuestScriptMap.end() == it ? 0 : &( it->second );
}


void CclientApp::LoadQuestScript()
{
	CMHFile file;

	if( ! file.Init( "./system/Resource/QuestScript.bin", "rb") )
	{
		ASSERT( 0 );
		return;
	}

	DWORD mainIndex = 0;

	QuestScript* script = 0;

	while( ! file.IsEOF() )
	{
		const CString keyword = file.GetString();

		if( "$QUEST" == keyword )
		{
			mainIndex = file.GetDword();
		}
		else if( "$SUBQUEST" == keyword )
		{
			const DWORD subIndex	= file.GetDword();
			const DWORD	index		= MAKELONG( mainIndex, subIndex );

			if( mQuestScriptMap.end() != mQuestScriptMap.find( index ) )
			{
				ASSERT( 0 && "Duplicated index is found" );
				return;
			}

			//ZeroMemory( script, sizeof( *script ) );

			// 초기화
			{
				script = &( mQuestScriptMap[ index ] );

				script->mMonsterIndex		= 0;
				script->mMonsterHuntCount	= 0;
			}
		}
		else if( "@HUNT" == keyword )
		{
			script->mMonsterIndex = file.GetDword();
		}
		else if( "*TAKEQUESTITEM" == keyword )
		{
			const DWORD itemIndex = file.GetDword();
			
			script->mItemMap[ itemIndex ] = 0;
		}
		else if( "@COUNT" == keyword )
		{
			file.GetDword();

			const DURTYPE count = file.GetDword();

			for( QuestScript::ItemMap::iterator it = script->mItemMap.begin();
				script->mItemMap.end() != it;
				++it )
			{
				it->second = count;
			}

			if( script->mMonsterIndex )
			{
				script->mMonsterHuntCount = count;
			}
		}
		else if( "*GIVEQUESTITEM" == keyword )
		{
			ASSERT( script );

			const DWORD itemIndex	= file.GetDword();
			const DWORD quantity	= file.GetDword();

			script->mItemMap[ itemIndex ] = quantity;
		}
	}
}


void CclientApp::LoadQuestItemScript()
{
	CMHFile file;

	if( ! file.Init( "./system/Resource/QuestItemList.bin", "rb") )
	{
		ASSERT( 0 );
		return;
	}

	char buffer[ MAX_PATH * 5 ];

	while( ! file.IsEOF() )
	{
		file.GetLine( buffer, sizeof( buffer ) );

		const char* separator = "\t ";

		const char* token1 = strtok( buffer, separator );
		const char* token2 = strtok( 0, separator );
		
		if( ! token1 || ! token2  )
		{
			continue;
		}

		const DWORD itemIndex	= atoi( token1 );
		//const char* name		= token2;

		QuestItem& item = mQuestItemMap[ itemIndex ];

		{
			CString text( token2 );
			CString token;
			
			int start = 0;

			do 
			{
				token = text.Tokenize( "^s", start );

				item.mName += token + " ";

			} while( ! token.IsEmpty() );
		}

		item.mMainQuestIndex	= atoi( strtok( 0, separator ) );
		item.mSubQuestIndex		= atoi( strtok( 0, separator ) );
		strtok( 0, separator );
		strtok( 0, separator );

		{
			CString text( strtok( 0, separator ) );
			CString token;

			int start = 0;

			do 
			{
				token = text.Tokenize( "^s", start );

				item.mDescription += token + " ";

			} while( ! token.IsEmpty() );
		}

		//mQuestItemMap.insert( std::make_pair( itemIndex, name ) );
	}
}


void CclientApp::LoadQuestStringScript()
{
	CMHFile file;

	if( ! file.Init( "./system/Resource/QuestString.bin", "rb") )
	{
		ASSERT( 0 );
		return;
	}

	char buffer[ MAX_PATH * 5 ];
	//char line[ MAX_PATH * 5 ];

	QuestString* data = 0;

	while( ! file.IsEOF() )
	{
		file.GetLine( buffer, sizeof( buffer ) );
		//_tcscpy( line, buffer );
		CString line( buffer );

		const char* separator	= "\t ";
		const char* token		= strtok( buffer, separator );

		if( ! token )
		{
			continue;
		}
		else if( ! strcmpi( token, "$SUBQUESTSTR" ) )
		{
			const DWORD mainQuestIndex	= atoi( strtok( 0, separator ) );
			const DWORD subQuestIndex	= atoi( strtok( 0, separator ) );
			const DWORD index			= MAKELONG( mainQuestIndex, subQuestIndex );

			if( mQuestStringMap.end() != mQuestStringMap.find( index ) )
			{
				ASSERT( 0 && "Duplicated index is found" );
			}

			data = &( mQuestStringMap[ index ] );

			data->mMainQuestIndex	= mainQuestIndex;
			data->mSubQuestIndex	= subQuestIndex;
		}
		else if( ! strcmpi( token, "#TITLE" ) )
		{
			ASSERT( data );

			const char* keyword		= "#TITLE";
			const int	position	= line.Find( keyword );
			
			// 예약어가 있는 위치를 알아내서 그 길이만큼 뛰어넘는다
			CString title( line.GetString() + position + strlen( keyword ) );

			data->mTitle = title.TrimLeft();
		}
		else if( ! strcmpi( token, "#DESC" ) )
		{
			ASSERT( data );

			file.GetLine( buffer, sizeof( buffer ) );
			file.GetLine( buffer, sizeof( buffer ) );

			// 문자열 앞단의 탭 문자를 제거하자
			data->mDescription = strtok( buffer, "\t" );
		}
	}
}


const CclientApp::QuestStringMap& CclientApp::GetQuestStringMap() const
{
	return mQuestStringMap;
}


const char* CclientApp::GetQuestTitle( DWORD mainQuestIndex, DWORD subQuestIndex ) const
{
	const DWORD index = MAKELONG( mainQuestIndex, subQuestIndex );

	QuestStringMap::const_iterator it = mQuestStringMap.find( index );

	return mQuestStringMap.end() == it ? "" : it->second.mTitle;
}


const char* CclientApp::GetQuestDescription( DWORD mainQuestIndex, DWORD subQuestIndex ) const
{
	const DWORD index = MAKELONG( mainQuestIndex, subQuestIndex );

	QuestStringMap::const_iterator it = mQuestStringMap.find( index );

	return mQuestStringMap.end() == it ? "?" : it->second.mDescription;
}


const char* CclientApp::GetQuestItemName( DWORD questItemIndex ) const
{
	QuestItemMap::const_iterator it = mQuestItemMap.find( questItemIndex );

	if( mQuestItemMap.end() == it )
	{
		return "";
	}

	const QuestItem& item = it->second;

	return item.mName;
}


const char* CclientApp::GetQuestItemDescription( DWORD questItemIndex ) const
{
	QuestItemMap::const_iterator it = mQuestItemMap.find( questItemIndex );

	if( mQuestItemMap.end() == it )
	{
		return "";
	}

	const QuestItem& item = it->second;

	return item.mDescription;
}


const CclientApp::QuestItemMap& CclientApp::GetQuestItemMap() const
{
	return mQuestItemMap;
}


DWORD CclientApp::GetMainQuestIndex( DWORD questItemIndex ) const
{
	QuestItemMap::const_iterator it = mQuestItemMap.find( questItemIndex );

	if( mQuestItemMap.end() == it )
	{
		return 0;
	}

	const QuestItem& item = it->second;

	return item.mMainQuestIndex;
}


DWORD CclientApp::GetSubQuestIndex( DWORD questItemIndex ) const
{
	QuestItemMap::const_iterator it = mQuestItemMap.find( questItemIndex );

	if( mQuestItemMap.end() == it )
	{
		return 0;
	}

	const QuestItem& item = it->second;

	return item.mSubQuestIndex;
}


void CclientApp::SetItemList( CListCtrl& listCtrl ) const
{
	listCtrl.DeleteAllItems();

	CString text;
	int index = 0;

	for( ItemMap::const_iterator it = mItemMap.begin(); mItemMap.end() != it; ++it, ++index )
	{
		const ITEM_INFO& item = it->second;
		
		text.Format( "%d", item.ItemIdx );
		listCtrl.InsertItem( index, text, 0 );
		listCtrl.SetItemText( index, 1, item.ItemName );

		text.Format( "%d", item.LimitLevel );
		listCtrl.SetItemText( index, 2, text );
	}
}


void  CclientApp::SetItemList( CListCtrl& listCtrl, eItemCategory category ) const
{
	listCtrl.DeleteAllItems();

	CString text;
	int index = 0;

	for( ItemMap::const_iterator it = mItemMap.begin(); mItemMap.end() != it; ++it )
	{
		const ITEM_INFO& item = it->second;

		if( item.Category != category )
		{
			continue;
		}

		const DWORD row		= listCtrl.GetItemCount();
		int			step	= 0;

		text.Format( "%d", item.ItemIdx );
		listCtrl.InsertItem( row, text, 0 );
		listCtrl.SetItemText( row, ++step, item.ItemName );

		text.Format( "%d", item.LimitLevel );
		listCtrl.SetItemText( row, ++step, text );
	}
}


void CclientApp::SetServerComboBox( CComboBox& combo ) const
{
	const CNetwork::ServerMap&		serverMap	= CNetwork::GetInstance().GetServerMap();
	const CNetwork::ConnectionMap&	connection	= CNetwork::GetInstance().GetConnectionMap();

	combo.ResetContent();

	for( CNetwork::ConnectionMap::const_iterator it = connection.begin(); connection.end() != it; ++it )
	{
		const DWORD							serverIndex = it->first;
		CNetwork::ServerMap::const_iterator find_it		= serverMap.find( serverIndex );

		if( serverMap.end() == find_it )
		{
			ASSERT( 0 && "It's no defined server" );
			continue;
		}

		const CNetwork::Server& server = find_it->second;

		combo.AddString( server.mName );
	}

	if( connection.size() )
	{
		combo.SetCurSel( 0 );
	}
}


void CclientApp::Send( const CString& serverName, const MSGROOT& message, size_t size )
{
	CNetwork& network = CNetwork::GetInstance();

	const DWORD serverIndex = network.GetServerIndex( serverName );

	Send( serverIndex, message, size );
}


void CclientApp::Send( DWORD serverIndex, const MSGROOT& message, size_t size )
{
	CNetwork& network = CNetwork::GetInstance();

	if( network.GetServerName( serverIndex ).IsEmpty() )
	{
		CString textAnyServerNotFoundToRequest;
		textAnyServerNotFoundToRequest.LoadString( IDS_STRING227 );

		MessageBox( 0, textAnyServerNotFoundToRequest, "", MB_ICONERROR | MB_OK );
		return;
	}

	network.Send( serverIndex, message, size );

	SetWait( TRUE );
}


void CclientApp::SendAll( const MSGROOT& message, size_t size )
{
	CNetwork::GetInstance().SendAll( message, size );

	SetWait( TRUE );
}


void CclientApp::ConvertWildCardForDb( char* text )
{
	// DB는 와일드카드 문자가 윈도우 운영체제와 다르므로 바꿔주자

	for( char* character = text; *character; ++character )
	{
		char& c = *character;

		if( '?' == c )
		{
			c = '_';
		}
		else if( '*' == c )
		{
			c = '%';
		}
	}
}


void CclientApp::LoadInterfaceMessage()
{
	CMHFile file;

	if( ! file.Init( "data/interface/windows/interfaceMsg.bin", "rb" ) )
	{
		ASSERT( 0 );
		return;
	}

	char line[ MAX_PATH * 5 ];

	while( ! file.IsEOF() )
	{
		file.GetLine( line, sizeof( line ) );

		const char* separator	= "\t ";
		const char* token		= strtok( line, separator );

		if( ! token )
		{
			continue;
		}

		const DWORD index = atoi( token );

		if( mInterfaceMessage.end() != mInterfaceMessage.find( index ) )
		{
			ASSERT( 0 && "Duplicated index is found" );
			continue;
		}

		CString& text = mInterfaceMessage[ index ];

        while( token = strtok( 0, separator ) )
		{
			text += token;
			text += ' ';
		}
	}
}


const char* CclientApp::GetInterfaceMessage( DWORD index ) const
{
	InterfaceMessage::const_iterator it = mInterfaceMessage.find( index );

	if( mInterfaceMessage.end() == it )
	{
		return "";
	}

	return it->second;
}


const char* CclientApp::GetJobName( DWORD code ) const
{
	DWORD index = 0;

	// [Client]Luna의 CProfileDlg::UpdateProfile()에서 발췌
	switch( code )
	{
	case 1111:	index = 364;	break;
	case 1121:	index = 365;	break;
	case 1122:	index = 366;	break;
	case 1131:	index = 367;	break;
	case 1132:	index = 368;	break;
	case 1133:	index = 369;	break;
	case 1141:	index = 370;	break;
	case 1142:	index = 371;	break;
	case 1143:	index = 372;	break;
	case 1144:	index = 373;	break;
	case 1151:	index = 374;	break;
	case 1152:	index = 375;	break;
	case 1153:	index = 376;	break;
	case 1154:	index = 377;	break;
	case 1155:	index = 378;	break;
	case 1156:	index = 379;	break;
	case 1161:	index = 380;	break;
	case 1162:	index = 381;	break;
	case 1163:	index = 382;	break;

	case 1211:	index = 364;	break;
	case 1221:	index = 365;	break;
	case 1222:	index = 366;	break;
	case 1231:	index = 367;	break;
	case 1232:	index = 368;	break;
	case 1233:	index = 369;	break;
	case 1241:	index = 370;	break;
	case 1242:	index = 371;	break;
	case 1243:	index = 372;	break;
	case 1244:	index = 373;	break;
	case 1251:	index = 374;	break;
	case 1252:	index = 375;	break;
	case 1253:	index = 376;	break;
	case 1254:	index = 377;	break;
	case 1255:	index = 378;	break;
	case 1256:	index = 379;	break;
	case 1261:	index = 380;	break;
	case 1262:	index = 381;	break;
	case 1263:	index = 382;	break;

	case 2111:	index = 383;	break;
	case 2121:	index = 384;	break;
	case 2122:	index = 385;	break;
	case 2131:	index = 386;	break;
	case 2132:	index = 387;	break;
	case 2133:	index = 388;	break;
	case 2141:	index = 389;	break;
	case 2142:	index = 390;	break;
	case 2143:	index = 391;	break;
	case 2144:	index = 392;	break;
	case 2151:	index = 393;	break;
	case 2152:	index = 394;	break;
	case 2153:	index = 395;	break;
	case 2154:	index = 396;	break;
	case 2161:	index = 397;	break;
	case 2162:	index = 398;	break;
	case 2163:	index = 399;	break;

	case 2211:	index = 383;	break;
	case 2221:	index = 384;	break;
	case 2222:	index = 385;	break;
	case 2231:	index = 386;	break;
	case 2232:	index = 387;	break;
	case 2233:	index = 388;	break;
	case 2241:	index = 389;	break;
	case 2242:	index = 390;	break;
	case 2243:	index = 391;	break;
	case 2244:	index = 392;	break;
	case 2251:	index = 393;	break;
	case 2252:	index = 394;	break;
	case 2253:	index = 395;	break;
	case 2254:	index = 396;	break;
	case 2261:	index = 397;	break;
	case 2262:	index = 398;	break;
	case 2263:	index = 399;	break;

	case 3111:	index = 400;	break;
	case 3121:	index = 401;	break;
	case 3122:	index = 402;	break;
	case 3131:	index = 403;	break;
	case 3132:	index = 404;	break;
	case 3133:	index = 405;	break;
	case 3141:	index = 406;	break;
	case 3142:	index = 407;	break;
	case 3143:	index = 408;	break;
	case 3144:	index = 409;	break;
	case 3151:	index = 410;	break;
	case 3152:	index = 411;	break;
	case 3153:	index = 412;	break;
	case 3154:	index = 413;	break;
	case 3155:	index = 414;	break;
	case 3161:	index = 415;	break;
	case 3162:	index = 416;	break;
	case 3163:	index = 417;	break;

	case 3211:	index = 400;	break;
	case 3221:	index = 401;	break;
	case 3222:	index = 402;	break;
	case 3231:	index = 403;	break;
	case 3232:	index = 404;	break;
	case 3233:	index = 405;	break;
	case 3241:	index = 406;	break;
	case 3242:	index = 407;	break;
	case 3243:	index = 408;	break;
	case 3244:	index = 409;	break;
	case 3251:	index = 410;	break;
	case 3252:	index = 411;	break;
	case 3253:	index = 412;	break;
	case 3254:	index = 413;	break;
	case 3255:	index = 414;	break;
	case 3261:	index = 415;	break;
	case 3262:	index = 416;	break;
	case 3263:	index = 417;	break;
	}

	return GetInterfaceMessage( index );
}


void CclientApp::LoadLicenseScript()
{
	char buffer[ MAX_PATH * 5 ];

	CMHFile file;

	if( ! file.Init( "system/resource/ResidentRegist.bin", "rb" ) )
	{
		ASSERT( 0 );
		return;
	}

	// 성향
	while( ! file.IsEOF() )
	{
		const CString keyword = file.GetString();

		if( "END_KIND" == keyword )
		{
			break;
		}
		else if( "//" == keyword )
		{
			file.GetLine( buffer, sizeof( buffer ) );
			continue;
		}

		const DWORD index = atoi( keyword );
		ASSERT( mLicenseFavoriteMap.end() == mLicenseFavoriteMap.find( index ) );

		mLicenseFavoriteMap.insert( std::make_pair( index, file.GetString() ) );
	}

	// 지역
	while( ! file.IsEOF() )
	{
		const CString keyword = file.GetString();

		if( "END_KIND" == keyword )
		{
			break;
		}
		else if( "//" == keyword )
		{
			file.GetLine( buffer, sizeof( buffer ) );
			continue;
		}

		const DWORD index = atoi( keyword );
		ASSERT( mLicenseAreaMap.end() == mLicenseAreaMap.find( index ) );

		mLicenseAreaMap.insert( std::make_pair( index, file.GetString() ) );
	}

	// 나이
	while( ! file.IsEOF() )
	{
		const CString keyword = file.GetString();

		if( "END_KIND" == keyword )
		{
			break;
		}
		else if( "//" == keyword )
		{
			file.GetLine( buffer, sizeof( buffer ) );
			continue;
		}

		const DWORD index = atoi( keyword );
		ASSERT( mLicenseAgeMap.end() == mLicenseAgeMap.find( index ) );

		CString text;
		text.Format( "%d~%d", atoi( file.GetString() ), atoi( file.GetString() ) );

		mLicenseAgeMap.insert( std::make_pair( index, text ) );
	}
}


const CclientApp::LicenseFavoriteMap& CclientApp::GetLicenseFavoriteMap() const
{
	return mLicenseFavoriteMap;
}


const CclientApp::LicenseAreaMap& CclientApp::GetLicenseAreaMap() const
{
	return mLicenseAreaMap;
}


const CclientApp::LicenseAgeMap& CclientApp::GetLicenseAgeMap() const
{
	return mLicenseAgeMap;
}


const CclientApp::SkillSizeMap& CclientApp::GetSkillSizeMap() const
{
	return mSkillSizeMap;
}


void CclientApp::LoadTip()
{
	CMHFile file;

	if( ! file.Init( "data/interface/windows/ToolTipMsg.bin", "rb" ) )
	{
		ASSERT( 0 );
		return;
	}

	char buffer[ MAX_PATH * 5 ];

	while( ! file.IsEOF() )
	{
		const CString keyword = file.GetString();

		if( "#Msg" == keyword )
		{
			const DWORD index = file.GetDword();

			file.GetLine( buffer, sizeof( buffer ) );
			file.GetLine( buffer, sizeof( buffer ) );
			file.GetLine( buffer, sizeof( buffer ) );

			mSkillTipMap.insert( std::make_pair( index, buffer ) );

			file.GetLine( buffer, sizeof( buffer ) );
		}
	}
}


const char* CclientApp::GetTip( DWORD skillIndex ) const
{
	SkillTipMap::const_iterator it = mSkillTipMap.find( skillIndex );

	return mSkillTipMap.end() == it ? "" : it->second;
}


void CclientApp::SetDateCombo( CComboBox& yearCombo, CComboBox& monthCombo, CComboBox& dayCombo, BOOL isToday )
{
	CRect rect;

	{
		yearCombo.ResetContent();
		yearCombo.GetWindowRect( rect );
		yearCombo.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );

		monthCombo.ResetContent();
		monthCombo.GetWindowRect( rect );
		monthCombo.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );

		dayCombo.ResetContent();
		dayCombo.GetWindowRect( rect );
		dayCombo.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );
	}

	// 값 세팅
	{
		CString text;

		for( DWORD year = 7; 20 >= year; ++year )
		{
			text.Format( "%02d", year );

			yearCombo.AddString( text );
		}

		for( DWORD month = 1; 12 >= month; ++month )
		{
			text.Format( "%d", month );

			monthCombo.AddString( text );
		}

		for( DWORD day = 1; 31 >= day; ++day )
		{
			text.Format( "%d", day );

			dayCombo.AddString( text );
		}
	}

	if( isToday )
	{
		const CTime time = CTime::GetCurrentTime();

		yearCombo.SetCurSel( time.GetYear() - 2007 );
		monthCombo.SetCurSel( time.GetMonth() - 1 );
		dayCombo.SetCurSel( time.GetDay() - 1 );
	}
	else
	{
		yearCombo.SetCurSel( 0 );
		monthCombo.SetCurSel( 5 );
		dayCombo.SetCurSel( 14 );
	}
}


void CclientApp::SaveToExcel( const CString& fileName, const CString& tableName, const CListCtrl& listCtrl )
{
	if( ! listCtrl.GetItemCount() )
	{
		CString textThereIsNoResult;
		textThereIsNoResult.LoadString( IDS_STRING1 );

		MessageBox( 0, textThereIsNoResult, "", MB_ICONERROR | MB_OK );
		return;
	}	

	// 참조
	// 
	// http://dualist.tistory.com/entry/MFC-%EC%97%91%EC%85%80-%ED%8C%8C%EC%9D%BC%EB%A1%9C-%EC%A0%80%EC%9E%A5%ED%95%98%EA%B8%B0

	//CFileDialog dialog( FALSE, "xls", fileName, OFN_OVERWRITEPROMPT, "*.xls" );
	CFileDialog dialog( FALSE, "xls", fileName, OFN_OVERWRITEPROMPT, "*.xls" );

	if( dialog.DoModal() != IDOK )
	{
		return;
	}

	CString sql;

	sql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",
		"MICROSOFT EXCEL DRIVER (*.XLS)", dialog.GetPathName(), dialog.GetPathName() );

	CDatabase database;

	if( ! database.OpenEx( sql, CDatabase::noOdbcDialog ) )
	{
		CString textWarninForSavingToExcel;
		textWarninForSavingToExcel.LoadString( IDS_STRING228 );

		MessageBox( 0, textWarninForSavingToExcel, "", MB_ICONERROR | MB_OK );
		return;
	}

	TRY
	{
		sql.Format( "DROP TABLE %s", tableName );
		database.ExecuteSQL( sql );
	}
	CATCH_ALL( e )
	{
	}
	END_CATCH_ALL;

	typedef std::pair< CString, CString > Field;
	typedef std::list< Field > FieldList;

	const CHeaderCtrl* pHeaderCtrl = ( CHeaderCtrl* )listCtrl.GetDlgItem( 0 );

	if( ! pHeaderCtrl )
	{
		return;
	}

	const int columnSize = pHeaderCtrl->GetItemCount();

	// 헤더를 조립한다
	typedef std::list< CString >	HeaderList;
	HeaderList						headerList;
	{
		char buffer[ MAX_PATH * 5 ];

		HDITEM item;
		{
			item.mask		= HDI_TEXT;
			item.pszText	= buffer;
			item.cchTextMax = sizeof( buffer );
		}

		for( int column = 0; column < columnSize; ++column )
		{
			pHeaderCtrl->GetItem( column, &item );

			// 공백 제거
			{
				CString text( item.pszText );
				CString token;
				CString name;

				int start = 0;

				do 
				{
					token = text.Tokenize( " ", start );

					name += token;

				} while( ! token.IsEmpty() );

				name.TrimRight();

				headerList.push_back( name );
			}
		}
	}

	// 테이블을 만들기위한 구문을 조립한다
	{
		sql = "CREATE TABLE " + tableName + "( ";

		for(
			HeaderList::const_iterator it = headerList.begin();
			headerList.end() != it; )
		{
			const CString& header = *it;
			
			sql += "\"" + header + "\" " + "TEXT";

			// 필드의 끝 처리
			if( headerList.end() == ++it )
			{
				sql += " )";
			}
			else
			{
				sql += ", ";
			}
		}

		database.ExecuteSQL( sql );
	}

	// 삽입 구문의 필드 부분을 조립한다
	{
		sql = "INSERT INTO " + tableName + "( ";

		for(
			HeaderList::const_iterator it = headerList.begin();
			headerList.end() != it;
		)
		{
			const CString& header = *it;

			sql += "\"" + header + "\"";

			// 필드의 끝 처리
			if( headerList.end() == ++it  )
			{
				sql += " ) VALUES ( ";			
			}
			else
			{
				sql += ", ";
			}
		}
	}

	// 삽입 구문의 값 부분을 조립하고 삽입한다.
	for( int row = 0; listCtrl.GetItemCount() > row; ++row )
	{
		CString statement;

		for( int column = 0; column < columnSize; ++column )
		{
			const CString& value	= listCtrl.GetItemText( row, column );

			// 문자열 정보인 경우 쿼터 부호로 감싼다.

			statement += "\'" + value + "\'";			

			// 필드 끝 처리
			if( columnSize == column + 1 )
			{
				statement += " )";		
			}
			else
			{
				statement += ", ";
			}
		}

		database.ExecuteSQL( sql + statement );
	}

	{
		CString textLogSavedToWhere;
		textLogSavedToWhere.LoadString( IDS_STRING229 );

		CString text;
		text.Format( textLogSavedToWhere, dialog.GetPathName() );

		MessageBox(
			0,
			text,
			"",
			MB_ICONINFORMATION | MB_OK );
	}
}


void CclientApp::SaveToClipBoard( CListCtrl& listCtrl )
{
	const CHeaderCtrl* pHeaderCtrl = listCtrl.GetHeaderCtrl();

	if( ! pHeaderCtrl )
	{
		return;
	}

	const int columnSize = pHeaderCtrl->GetItemCount();

	CString text;

	{
		HDITEM item;
		char buffer[ MAX_PATH * 5 ];

		item.mask		= HDI_TEXT;
		item.pszText	= buffer;
		item.cchTextMax = sizeof( buffer );

		for( int column = 0; column < columnSize; ++column )
		{			
			pHeaderCtrl->GetItem( column, &item );

			text += CString( item.pszText ) + "\t";
		}		

		text += "\r\n";
	}
	
	{
		POSITION position = listCtrl.GetFirstSelectedItemPosition();

		while( position )
		{
			const int row = listCtrl.GetNextSelectedItem( position );

			for( int column = 0; column < columnSize; ++column )
			{
				text += listCtrl.GetItemText( row, column ) + "\t";
			}

			text += "\r\n";
		}
	}
	
	// 클립보드에 저장
	{
		HGLOBAL hGlobal = GlobalAlloc(GHND | GMEM_SHARE, text.GetLength() + 1 );

		PSTR pGlobal = ( PSTR )GlobalLock( hGlobal );

		_tcscpy( pGlobal, text );

		GlobalUnlock( hGlobal );

		OpenClipboard( AfxGetApp()->GetMainWnd()->GetSafeHwnd() );
		EmptyClipboard();
		SetClipboardData( CF_TEXT, hGlobal );

		CloseClipboard();
	}
}


const CString CclientApp::GetRankName( DWORD rank ) const
{
	switch( rank )
	{
	case GUILD_MASTER:
		{
			CString textMaster;
			textMaster.LoadString( IDS_STRING230 );

			return textMaster;
		}
	case GUILD_VICEMASTER:
		{
			CString textViceMaster;
			textViceMaster.LoadString( IDS_STRING231 );

			return textViceMaster;
		}
	case GUILD_SENIOR:
		{
			CString textOlder;
			textOlder.LoadString( IDS_STRING232 );

			return textOlder;
		}
	case GUILD_JUNIOR:
		{
			CString textSenior;
			textSenior.LoadString( IDS_STRING233 );

			return textSenior;
		}
	case GUILD_MEMBER:
		{
			CString textJunior;
			textJunior.LoadString( IDS_STRING234 );

			return textJunior;
		}
	case GUILD_STUDENT:
		{
			CString textStudent;
			textStudent.LoadString( IDS_STRING235 );

			return textStudent;
		}
	default:
		{
			return "?";
		}
	}
}


void CclientApp::SetRankCombo( CComboBox& combo ) const
{
	combo.ResetContent();

	CString textNoMember;
	textNoMember.LoadString( IDS_STRING236 );

	combo.AddString( textNoMember );

	// 길드 직위 간의 값 차이가 일정한 것을 이용하여 모든 직위를 세팅한다
	for(
		DWORD i = GUILD_STUDENT;
		GUILD_MASTER >= i;
		i += abs( GUILD_MEMBER - GUILD_STUDENT ) )
	{
		combo.AddString( GetRankName( i ) );
	}
}


void CclientApp::SetServerListCtrl( CListCtrl& listCtrl )
{
	listCtrl.DeleteAllItems();

	const CNetwork::ServerMap&		serverMap	= CNetwork::GetInstance().GetServerMap();

	CString text;

	for( CNetwork::ServerMap::const_iterator it = serverMap.begin(); serverMap.end() != it; ++it )
	{
		const DWORD				serverIndex = it->first;
		const CNetwork::Server& server		= it->second;

		const DWORD row		= listCtrl.GetItemCount();
		int			step	= 0;

		text.Format( "%d", serverIndex );
		listCtrl.InsertItem( row, text, 0 );

		listCtrl.SetItemText( row, ++step, server.mName );
		listCtrl.SetItemText( row, ++step, server.mIP );

		listCtrl.SetItemText( row, ++step, "x" );
	}
}


void CclientApp::UpdateServerListCtrl( CListCtrl& listCtrl )
{
	const CNetwork::ConnectionMap&	connection	= CNetwork::GetInstance().GetConnectionMap();

	for( int i = 0; i < listCtrl.GetItemCount(); ++i )
	{
		const CString serverIndex = listCtrl.GetItemText( i, 0 );

		if( connection.end() != connection.find( atoi( serverIndex ) ) )
		{
			listCtrl.SetItemText( i, 3, "o ");
		}
	}
}


size_t CclientApp::GetConnectionSize() const
{
	const size_t size = CNetwork::GetInstance().GetConnectionMap().size();

	return size;
}


const CString CclientApp::GetLogText( eFamilyLog type ) const
{
	switch( type )
	{
	case eLog_FamilyPoint:
	case eLog_FamilyPointAdd:
		{
			CString textGetPoint;
			textGetPoint.LoadString( IDS_STRING237 );

			return textGetPoint;
		}
	case eLog_FamilyFarmPutCrop:
		{
			CString textSeed;
			textSeed.LoadString( IDS_STRING238 );

			return textSeed;
		}
	case eLog_FamilyFarmDieCrop:
		{
			CString textDied;
			textDied.LoadString( IDS_STRING239 );

			return textDied;
		}
	case eLog_FamilyFarmGrowCrop:
		{
			CString textGrow;
			textGrow.LoadString( IDS_STRING240 );

			return textGrow;
		}
	case eLog_FamilyFarmCareCrop:
		{
			CString textCare;
			textCare.LoadString( IDS_STRING241 );

			return textCare;
		}
	case eLog_FamilyFarmHarvestCrop:
		{
			CString textHarvest;
			textHarvest.LoadString( IDS_STRING242 );

			return textHarvest;
		}
	case eLog_FamilyFarmLostRight:	// 권리 상실(패밀리 탈퇴 등으로 인한 권리 상실)
		{
			CString textFamily;
			textFamily.LoadString( IDS_STRING165 );

			CString textSecede;
			textSecede.LoadString( IDS_STRING244 );

			return textFamily + ": " + textSecede;
		}
	case eLog_FamilyFarmDisband:		// 농장 해체(패밀리 해산으로 인한)
		{
			CString textFamily;
			textFamily.LoadString( IDS_STRING165 );

			CString textDisband;
			textDisband.LoadString( IDS_STRING248 );

			return textFamily + ": " + textDisband;
		}
	case eLog_FamilyFarmTax:
		{
			CString textTaxDelayed;
			textTaxDelayed.LoadString( IDS_STRING310 );
			
			return textTaxDelayed;
		}
	// unindentified log
	case eLog_FamilyFarmMystery:
	default:
		{
			return "?";
		}
	}
}


const CString CclientApp::GetLogText( eGuildLog type ) const
{
	switch( type )
	{
	case eGuildLog_MemberSecede:
		{
			CString textGuild;
			textGuild.LoadString( IDS_STRING164 );

			CString textSecede;
			textSecede.LoadString( IDS_STRING244 );

			return textSecede + ": " + textGuild;
		}
	case eGuildLog_MemberBan:
		{
			CString textGuild;
			textGuild.LoadString( IDS_STRING164 );

			CString textKick;
			textKick.LoadString( IDS_STRING245 );

			return textKick + ": " + textGuild;
		}
	case eGuildLog_MemberAdd:
		{
			CString textJoin;
			textJoin.LoadString( IDS_STRING246 );

			CString textJunior;
			textJunior.LoadString( IDS_STRING234 );

			return textJoin + ": " + textJunior;
		}
	case eGuildLog_StudentAdd:
		{
			CString textJoin;
			textJoin.LoadString( IDS_STRING246 );

			CString textStudent;
			textStudent.LoadString( IDS_STRING235 );

			return textJoin + ": " + textStudent;
		}
	case eGuildLog_MasterChangeRank:
		{
			CString textChangeRank;
			textChangeRank.LoadString( IDS_STRING43 );

			return textChangeRank;
		}
	case eLogGuild_GuildCreate:
		{
			CString textGuild;
			textGuild.LoadString( IDS_STRING164 );

			CString textBuild;
			textBuild.LoadString( IDS_STRING247 );

			return textBuild + ": " + textGuild;
		}
	case eGuildLog_GuildBreakUp:
		{
			CString textGuild;
			textGuild.LoadString( IDS_STRING164 );

			CString textDisband;
			textDisband.LoadString( IDS_STRING248 );

			return textDisband + ": " + textGuild;
		}
	case eGuildLog_GuildLevelUp:
		{
			CString textLevelUp;
			textLevelUp.LoadString( IDS_STRING249 );

			return textLevelUp;
		}
	case eGuildLog_GuildLevelDown:
		{
			CString textLevelDown;
			textLevelDown.LoadString( IDS_STRING250 );

			return textLevelDown;
		}
	case eGuildLog_CreateUnion:
		{
			CString textAlliance;
			textAlliance.LoadString( IDS_STRING250 );

			CString textBuild;
			textBuild.LoadString( IDS_STRING247 );

			return textBuild + ": " + textAlliance;
		}
	case eGuildLog_DestroyUnion:
		{
			CString textAlliance;
			textAlliance.LoadString( IDS_STRING250 );

			CString textDisband;
			textDisband.LoadString( IDS_STRING248 );

			return textDisband + ": " + textAlliance;
		}
	case eGuildLog_AddUnion:
		{
			CString textAlliance;
			textAlliance.LoadString( IDS_STRING250 );

			CString textAdd;
			textAdd.LoadString( IDS_STRING252 );

			return textAdd + ": " + textAlliance;
		}
	case eGuildLog_RemoveUnion:	
		{
			CString textAlliance;
			textAlliance.LoadString( IDS_STRING250 );

			CString textRemove;
			textRemove.LoadString( IDS_STRING253 );

			return textRemove + ": " + textAlliance;
		}
	case eGuildLog_SecedeUnion:
		{
			CString textAlliance;
			textAlliance.LoadString( IDS_STRING250 );

			CString textKick;
			textKick.LoadString( IDS_STRING245 );

			return textKick + ": " + textAlliance;
		}
	case eGuildLog_GM_ChangeRank:
		{
			CString textChangeRank;
			textChangeRank.LoadString( IDS_STRING43 );

			CString textRecovery;
			textRecovery.LoadString( IDS_STRING263 );

			return textChangeRank + textRecovery;
		}
	case eGuildLog_TimeScoreAdd:
		{
			CString textGetScore;
			textGetScore.LoadString( IDS_STRING253 );

			CString textRegular;
			textRegular.LoadString( IDS_STRING255 );

			return textGetScore + ": " + textRegular;
		}
	case eGuildLog_ScoreAddByKill:
		{
			CString textGetScore;
			textGetScore.LoadString( IDS_STRING253 );

			CString textKill;
			textKill.LoadString( IDS_STRING256 );

			return textGetScore + ": " + textKill;
		}
	case eGuildLog_ScoreRemoveByMurder:
		{
			CString textLostScore;
			textLostScore.LoadString( IDS_STRING254 );

			CString textKill;
			textKill.LoadString( IDS_STRING256 );

			return textLostScore + ": " + textKill;
		}
	case eGuildLog_ScoreAddByWin:
		{
			CString textGetScore;
			textGetScore.LoadString( IDS_STRING253 );

			CString textWinWar;
			textWinWar.LoadString( IDS_STRING257 );

			return textGetScore + ": " + textWinWar;
		}
	case eGuildLog_ScoreRemoveByDefeat:
		{
			CString textLostScore;
			textLostScore.LoadString( IDS_STRING254 );

			CString textLoseWar;
			textLoseWar.LoadString( IDS_STRING258 );

			return textLostScore + ": " + textLoseWar;
		}
	case eGuildLog_ScoreCheat:
		{
			CString textSetScore;
			textSetScore.LoadString( IDS_STRING259 );

			CString textCheat;
			textCheat.LoadString( IDS_STRING262 );

			return textSetScore + ": " + textCheat;
		}
	case eGuildLog_ScoreLevelUp:
		{
			CString textUseScore;
			textUseScore.LoadString( IDS_STRING260 );

			CString textLevelUp;
			textLevelUp.LoadString( IDS_STRING249 );

			return textUseScore + ": " + textLevelUp;
		}
	default:
		{
			return "?";
		}
	}
}


const CString CclientApp::GetLogText( eLogSkill type ) const
{
	switch( type )
	{
	case eLog_SkillLearn:
		{
			CString textAdd;
			textAdd.LoadString( IDS_STRING252 );

			return textAdd;
		}
	case eLog_SkillDiscard:
		{
			CString textRemove;
			textRemove.LoadString( IDS_STRING253 );

			return textRemove;
		}
	case eLog_SkillLevelup:
		{
			CString textLevelUp;
			textLevelUp.LoadString( IDS_STRING249 );

			return textLevelUp;
		}
	case eLog_SkillCheatLevelup:
		{
			CString textLevelDown;
			textLevelDown.LoadString( IDS_STRING250 );

			CString textCheat;
			textCheat.LoadString( IDS_STRING262 );
            
			return textLevelDown + ": " + textCheat;
		}
	case eLog_SkillLearn_RM:
		{
			CString textAdd;
			textAdd.LoadString( IDS_STRING252 );

			CString textRecovery;
			textRecovery.LoadString( IDS_STRING262 );
			
			return textAdd + ": " + textRecovery;
		}
	case eLog_SkillLevel_RM:
		{
			CString textLevelUpdate;
			textLevelUpdate.LoadString( IDS_STRING263 );

			CString textRecovery;
			textRecovery.LoadString( IDS_STRING262 );

			return textLevelUpdate + ": " + textRecovery;
		}
	case eLog_SkillRemove_RM:
		{
			CString textRemove;
			textRemove.LoadString( IDS_STRING253 );

			CString textRecovery;
			textRecovery.LoadString( IDS_STRING262 );

			return textRemove + ": " + textRecovery;
		}
	case eLog_SkillGetPoint:
		{
			CString textAdd;
			textAdd.LoadString( IDS_STRING252 );

			CString textSkillPoint;
			textSkillPoint.LoadString( IDS_STRING96 );

			return textAdd + ": " + textSkillPoint;
		}
	default:
		{
			return "?";
		}
	}
}


const CString CclientApp::GetLogText( eLogitemmoney type ) const
{
	switch( type )
	{
	case eLog_ItemUse:
		{
			CString textUse;
			textUse.LoadString( IDS_STRING270 );

			return textUse;
		}
	case eLog_ItemObtainMix:
	case eLog_ItemMixSuccess:
		{
			CString textAdd;
			textAdd.LoadString( IDS_STRING252 );

			CString textMix;
			textMix.LoadString( IDS_STRING90 );

			return textAdd + ": " + textMix;
		}
	case eLog_ItemMixUse:
		{
			CString textUse;
			textUse.LoadString( IDS_STRING270 );

			CString textMix;
			textMix.LoadString( IDS_STRING90 );

			return textUse+ ": " + textMix;
		}
	case eLog_ItemObtainDivide:
		{
			CString textDivide;
			textDivide.LoadString( IDS_STRING288 );

			return textDivide;
		}
	case eLog_ItemSell:
		{
			CString textSell;
			textSell.LoadString( IDS_STRING268 );

			return textSell;
		}
	case eLog_ItemObtainBuy:
		{
			CString textBuy;
			textBuy.LoadString( IDS_STRING267 );

			return textBuy;
		}
	case eLog_ItemObtainMonster:
		{
			CString textHunt;
			textHunt.LoadString( IDS_STRING287 );

			return textHunt;
		}
	case eLog_ItemObtainCheat:
		{
			CString textCheat;
			textCheat.LoadString( IDS_STRING262 );

			return textCheat;
		}
	case eLog_ItemObtainMonstermeter:
		{
			CString textMonsterMeter;
			textMonsterMeter.LoadString( IDS_STRING286 );

			return textMonsterMeter;
		}
	case eLog_ItemMixBigFail:
		{
			CString textMix;
			textMix.LoadString( IDS_STRING90 );

			CString textBigFailed;
			textBigFailed.LoadString( IDS_STRING285 );

			return textMix + ": " + textBigFailed;
		}
	case eLog_ItemMixFail:
		{
			CString textMix;
			textMix.LoadString( IDS_STRING90 );

			CString textFailure;
			textFailure.LoadString( IDS_STRING265 );

			return textMix + ": " + textFailure;
		}
	case eLog_ItemReinforceFail:
		{
			CString textReinforce;
			textReinforce.LoadString( IDS_STRING89 );

			CString textFail;
			textFail.LoadString( IDS_STRING266 );
				 
			return textReinforce + ": " + textFail;
		}
	case eLog_ItemDestroyReinforce:
		{
			CString textUse;
			textUse.LoadString( IDS_STRING270 );

			CString textReinforce;
			textReinforce.LoadString( IDS_STRING89 );

			return textReinforce + ": " + textUse;
		}
	case eLog_ItemDiscard:
		{
			CString textRemove;
			textRemove.LoadString( IDS_STRING253 );

			return textRemove;
		}
	case eLog_ItemObtainQuest:
		{
			CString textQuest;
			textQuest.LoadString( IDS_STRING190 );

			CString textAdd;
			textAdd.LoadString( IDS_STRING252 );

			return textQuest + ": " + textAdd;
		}
	case eLog_ItemObtainFromChangeItem:
		{
			CString textChange;
			textChange.LoadString( IDS_STRING284 );

			CString textAdd;
			textAdd.LoadString( IDS_STRING252 );

			return textChange + ": " + textAdd;
		}
	case eLog_ItemDestroybyChangeItem:
		{
			CString textChange;
			textChange.LoadString( IDS_STRING284 );

			CString textUse;
			textUse.LoadString( IDS_STRING270 );

			return textChange + ": " + textUse;
		}
	case eLog_ItemMoveStorageToStorage:
		{
			CString textStorage;
			textStorage.LoadString( IDS_STRING282 );

			return textStorage + "->" + textStorage;
		}
	case eLog_ItemMoveInvenToInven:
		{
			CString textInventory;
			textInventory.LoadString( IDS_STRING281 );

			return textInventory + "->" + textInventory;
		}
	case eLog_ItemMoveInvenToStorage:
		{
			CString textInventory;
			textInventory.LoadString( IDS_STRING281 );

			CString textStorage;
			textStorage.LoadString( IDS_STRING282 );
				 
			return textInventory + "->" + textStorage;
		}
	case eLog_ItemMoveInvenToGuild:
		{
			CString textInventory;
			textInventory.LoadString( IDS_STRING281 );

			CString textGuild;
			textGuild.LoadString( IDS_STRING164 );

			return textInventory + "->" + textGuild;
		}
	case eLog_ItemMoveInvenToPetInven:
		{
			CString textInventory;
			textInventory.LoadString( IDS_STRING281 );

			CString textPet;
			textPet.LoadString( IDS_STRING280 );

			return textInventory + "->" + textPet;
		}
	case eLog_ItemMoveStorageToInven:
		{
			CString textStorage;
			textStorage.LoadString( IDS_STRING282 );

			CString textInventory;
			textInventory.LoadString( IDS_STRING281 );

			return textStorage + "->" + textInventory;
		}
	case eLog_ItemMoveGuildToInven:
		{
			CString textGuild;
			textGuild.LoadString( IDS_STRING164 );

			CString textInventory;
			textInventory.LoadString( IDS_STRING281 );

			return textGuild + "->" + textInventory;
		}
	case eLog_ItemMoveGuild:
		{
			CString textGuild;
			textGuild.LoadString( IDS_STRING164 );

			return textGuild + "->" + textGuild;
		}
	case eLog_ShopItemMoveToInven:
		{
			CString textCashStorage;
			textCashStorage.LoadString( IDS_STRING283 );

			CString textInventory;
			textInventory.LoadString( IDS_STRING281 );

			return textCashStorage + "->" + textInventory;
		}
	case eLog_ItemMovePetInvenToInven:
		{
			CString textPet;
			textPet.LoadString( IDS_STRING280 );

			CString textInventory;
			textInventory.LoadString( IDS_STRING281 );

			return textPet + "->" + textInventory;
		}
	case eLog_ItemDestroyDissolution:
		{
			CString textDissolve;
			textDissolve.LoadString( IDS_STRING277 );

			CString textUse;
			textUse.LoadString( IDS_STRING270 );

			return textDissolve + ": " + textUse;
		}
	case eLog_ItemDissolveAddDuplicated:
	case eLog_ItemDissolveSuccess:
	case eLog_ItemObtainDissolve:
		{
			CString textAdd;
			textAdd.LoadString( IDS_STRING252 );

			CString textDissolve;
			textDissolve.LoadString( IDS_STRING277 );

			return textDissolve + ": " + textAdd;
		}
	case eLog_ItemDestroyDeleteQuest:
	case eLog_ItemDestroyGiveQuest:
		{
			CString textQuest;
			textQuest.LoadString( IDS_STRING190 );

			CString textUse;
			textUse.LoadString( IDS_STRING270 );

			return textQuest + ": " + textUse;
		}
	case eLog_ItemObtainPK:
		{
			CString textKill;
			textKill.LoadString( IDS_STRING257 );

			return textKill;
		}
	case eLog_ItemDestroyByWanted:
		{
			CString textUse;
			textUse.LoadString( IDS_STRING270 );

			CString textWanted;
			textWanted.LoadString( IDS_STRING278 );

			return textWanted + ": " + textUse;
		}
	case eLog_ItemDestroyByBadFame:
		{
			CString textBadFame;
			textBadFame.LoadString( IDS_STRING279 );

			return textBadFame;
		}
	case eLog_ItemReinforceSuccess:
		{
			CString textReinforce;
			textReinforce.LoadString( IDS_STRING89 );

			return textReinforce;
		}
	case eLog_ItemEnchantSuccess:
		{
			CString textEnchant;
			textEnchant.LoadString( IDS_STRING91 );

			return textEnchant;
		}
	case eLog_ItemEnchantUse:
		{
			CString textUse;
			textUse.LoadString( IDS_STRING270 );

			CString textEnchant;
			textEnchant.LoadString( IDS_STRING91 );

			return textEnchant + ": " + textUse;
		}
	case eLog_ItemEnchantDestroy:
		{
			CString textUse;
			textUse.LoadString( IDS_STRING270 );

			CString textEnchant;
			textEnchant.LoadString( IDS_STRING91 );

			CString textFail;
			textFail.LoadString( IDS_STRING266 );

			return textEnchant + ": " + textFail + ": " + textUse;
		}
	case eLog_Item:
	case eLog_ItemAdd:
		{
			CString textAdd;
			textAdd.LoadString( IDS_STRING252 );

			CString textRecovery;
			textRecovery.LoadString( IDS_STRING263 );

			return textRecovery + ": " + textAdd;
		}
	case eLog_ItemRemove:
		{
			CString textRemove;
			textRemove.LoadString( IDS_STRING253 );

			CString textRecovery;
			textRecovery.LoadString( IDS_STRING263 );

			return textRecovery + ": " + textRemove;
		}
	case eLog_ItemOptionAdd_RM:
		{
			CString textAdd;
			textAdd.LoadString( IDS_STRING252 );

			CString textOption;
			textOption.LoadString( IDS_STRING55 );

			CString textRecovery;
			textRecovery.LoadString( IDS_STRING263 );

			return textRecovery + ": " + textAdd + ": " + textOption;
		}
	case eLog_ItemOptionRemove_RM:
		{
			CString textOption;
			textOption.LoadString( IDS_STRING55 );

			CString textRemove;
			textRemove.LoadString( IDS_STRING253 );

			CString textRecovery;
			textRecovery.LoadString( IDS_STRING263 );

			return textRecovery + ": " + textRemove + ": " + textOption;
		}
	case eLog_StreetStallBuyAll:
		{
			CString textStall;
			textStall.LoadString( IDS_STRING269 );

			CString textBuy;
			textBuy.LoadString( IDS_STRING267 );

			return textStall + ": " + textBuy;
		}
	case eLog_StreetStallBuyDivide:
		{
			CString textStall;
			textStall.LoadString( IDS_STRING269 );

			CString textBuy;
			textBuy.LoadString( IDS_STRING267 );

			CString textPartial;
			textPartial.LoadString( IDS_STRING276 );

			return textStall + ": " + textBuy + ": " + textPartial;
		}
	case eLog_StreetStallSellDivide:
		{
			CString textStall;
			textStall.LoadString( IDS_STRING269 );

			CString textSell;
			textSell.LoadString( IDS_STRING268 );

			CString textPartial;
			textPartial.LoadString( IDS_STRING276 );

			return textStall + ": " + textSell + ": " + textPartial;
		}
	case eLog_Exchange:
		{
			CString textExchange;
			textExchange.LoadString( IDS_STRING275 );

			return textExchange;
		}
	case eLog_ShopItemUse:
		{
			CString textUse;
			textUse.LoadString( IDS_STRING270 );

			CString textCashItem;
			textCashItem.LoadString( IDS_STRING271 );

			return textCashItem + ": " + textUse;
		}
	case eLog_ShopItemUseEnd:
		{
			CString textFinish;
			textFinish.LoadString( IDS_STRING292 );

			CString textCashItem;
			textCashItem.LoadString( IDS_STRING271 );

			return textCashItem + ": " + textFinish;
		}
	case eLog_ShopItemProtectAll:
		{
			CString textProtect;
			textProtect.LoadString( IDS_STRING274 );

			CString textCashItem;
			textCashItem.LoadString( IDS_STRING271 );

			return textCashItem + ": " + textProtect;
		}
	case eLog_ShopItemSeal:
		{
			CString textSeal;
			textSeal.LoadString( IDS_STRING272 );

			return textSeal;
		}
	case eLog_ShopItemUnseal:
		{
			CString textUnseal;
			textUnseal.LoadString( IDS_STRING273 );

			CString textSuccess;
			textSuccess.LoadString( IDS_STRING265 );
            
			return textUnseal + ": " + textSuccess;
		}
	case eLog_ShopItemUnseal_Failed:
		{
			CString textUnseal;
			textUnseal.LoadString( IDS_STRING273 );

			CString textFail;
			textFail.LoadString( IDS_STRING266 );

			return textUnseal + ": " + textFail;
		}
	case eLog_ShopItem_ReinforceReset:
		{
			CString textResetReinforce;
			textResetReinforce.LoadString( IDS_STRING89 );

			CString textCashItem;
			textCashItem.LoadString( IDS_STRING271 );

			return textResetReinforce + ": " + textCashItem;
		}
	case eLog_ShopItemGetCheat:
		{
			CString textCheat;
			textCheat.LoadString( IDS_STRING262 );

			CString textAdd;
			textAdd.LoadString( IDS_STRING252 );

			CString textCashItem;
			textCashItem.LoadString( IDS_STRING271 );

			return textCheat + ": " + textAdd + ": " + textCashItem;
		}
	case eLog_ShopItemUse_MixSuccess:
		{
			CString textMix;
			textMix.LoadString( IDS_STRING90 );

			CString textSuccess;
			textSuccess.LoadString( IDS_STRING265 );

			CString textCashItem;
			textCashItem.LoadString( IDS_STRING271 );

			return textCashItem + ": " + textMix + ": " + textSuccess;
		}
	case eLog_ItemSkillLearn:
		{
			CString textSkillAdd;
			textSkillAdd.LoadString( IDS_STRING321 );

			return textSkillAdd;
		}
	default:
		{
			return "?";
		}
	}
}


const CString CclientApp::GetLogText( eQuestLog type ) const
{
	switch( type )
	{
	case eQuestLog_GM:
		{
			CString textRecovery;
			textRecovery.LoadString( IDS_STRING263 );

			return textRecovery;
		}
	case eQuestLog_Main_Begin:
		{
			CString textMainQuest;
			textMainQuest.LoadString( IDS_STRING289 );

			CString textBegin;
			textBegin.LoadString( IDS_STRING291 );

			return textMainQuest + ": " + textBegin;
		}
	case eQuestLog_Sub_Begin:
		{
			CString textSubQuest;
			textSubQuest.LoadString( IDS_STRING290 );

			CString textBegin;
			textBegin.LoadString( IDS_STRING291 );

			return textSubQuest + ": " + textBegin;
		}
	case eQuestLog_Main_End:
		{
			CString textMainQuest;
			textMainQuest.LoadString( IDS_STRING289 );

			CString textEnd;
			textEnd.LoadString( IDS_STRING292 );

			return textMainQuest + ": " + textEnd;
		}
	case eQuestLog_Sub_End:
		{
			CString textSubQuest;
			textSubQuest.LoadString( IDS_STRING290 );

			CString textEnd;
			textEnd.LoadString( IDS_STRING292 );

			return textSubQuest + ": " + textEnd;
		}
	case eQuestLog_Main_Delete:
		{
			CString textMainQuest;
			textMainQuest.LoadString( IDS_STRING289 );

			CString textGiveUp;
			textGiveUp.LoadString( IDS_STRING293 );

			return textMainQuest + ": " + textGiveUp;
		}
	case eQuestLog_Sub_Delete:
		{
			CString textSubQuest;
			textSubQuest.LoadString( IDS_STRING290 );

			CString textGiveUp;
			textGiveUp.LoadString( IDS_STRING293 );

			return textSubQuest + ": " + textGiveUp;
		}
	case eQuestLOg_ItemTake:
		{
			CString textItem;
			textItem.LoadString( IDS_STRING189 );

			CString textAdd;
			textAdd.LoadString( IDS_STRING252 );

			return textAdd + ": " + textItem;
		}
	case eQuestLOg_ItemGive:
		{
			CString textItem;
			textItem.LoadString( IDS_STRING189 );

			CString textGive;
			textGive.LoadString( IDS_STRING294 );

			return textGive + ": " + textItem;
		}
	case eQuestLOg_ItemUpdate:
		{
			CString textItem;
			textItem.LoadString( IDS_STRING189 );

			CString textUpdate;
			textUpdate.LoadString( IDS_STRING295 );

			return textUpdate + ": " + textItem;
		}
	default:
		{
			return "?";
		}
	}
}


void CclientApp::SetJobCombo( CComboBox& comboBox, DWORD jobType, DWORD race, DWORD jobLevel, DWORD jobIndex )
{
	comboBox.ResetContent();

	comboBox.AddString( "..." );
	
	for( DWORD index = 1; index < 9; ++index )
	{
		const DWORD code	= jobType * 1000 + ( race + 1 ) * 100 + jobLevel * 10 + index;

		const char* name = CclientApp::GetJobName( code );

		if( *name )
		{
			comboBox.AddString( name );
		}
		else
		{
			break;
		}
	}

	comboBox.SetCurSel( jobIndex );
}


const char* CclientApp::GetCropname( DWORD cropIndex, DWORD cropLevel ) const
{
	FarmCropMap::const_iterator it = mFarmCropMap.find( MAKELONG( cropIndex, cropLevel ) );

	return mFarmCropMap.end() == it ? "" : it->second;
}


void CclientApp::LoadFarmScript()
{
	CMHFile file;

	if(! file.Init("System/Resource/FarmRenderObjName.bin", "rb"))
	{
		ASSERT( 0 );
		return;
	}

	char buffer[ MAX_PATH * 5 ];

	int index = 0;
	int level = 0;

	CString line;

	while( ! file.IsEOF() )
	{
		file.GetLine( buffer, sizeof( buffer ) );

		line = buffer;

		const char* separator	= "\t ";
		const char* token		= strtok( buffer, separator );

		if( !	token )
		{
			++index;
			level = 0;
			continue;
		}
		else if( ! strcmpi( "//", token ) )
		{
			continue;
		}

		mFarmCropMap.insert( std::make_pair( MAKELONG( index, ++level ), line ) );
	}
}


void CclientApp::SetUserLevelCombo( CComboBox& combo ) const
{
	combo.ResetContent();

	CString textGod;
	textGod.LoadString( IDS_STRING296 );

	CString textProgrammer;
	textProgrammer.LoadString( IDS_STRING297 );

	CString textDeveloper;
	textDeveloper.LoadString( IDS_STRING298 );

	CString textManager;
	textManager.LoadString( IDS_STRING299 );

	CString textSuperUser;
	textSuperUser.LoadString( IDS_STRING300 );

	CString textUser;
	textUser.LoadString( IDS_STRING301 );

	CString textSubUser;
	textSubUser.LoadString( IDS_STRING302 );

	CString textBlockedUser;
	textBlockedUser.LoadString( IDS_STRING303 );

	combo.AddString( textGod );
	combo.AddString( textProgrammer );
	combo.AddString( textDeveloper );
	combo.AddString( textManager );
	combo.AddString( textSuperUser );
	combo.AddString( textUser );
	combo.AddString( textSubUser );
	combo.AddString( textBlockedUser );
}


DWORD CclientApp::GetItemIndex( const CString& name ) const
{
	for( ItemMap::const_iterator it = mItemMap.begin(); mItemMap.end() != it; ++it )
	{
		const ITEM_INFO& item = it->second;

		if( item.ItemName == name )
		{
			return item.ItemIdx;
		}
	}

	return 0;
}


void CclientApp::SetItemCombo( CComboBox& combo, const CString& keyword ) const
{
	combo.ResetContent();

	for( ItemMap::const_iterator it = mItemMap.begin(); mItemMap.end() != it; ++it )
	{
		const ITEM_INFO& item = it->second;

		const std::string name( item.ItemName );

		if( std::string::npos != name.find( keyword ) )
		{
			combo.AddString( item.ItemName );
		}
	}

	if( combo.GetCount() )
	{
		combo.SetCurSel( 0 );
	}
}


const CString CclientApp::GetItemPositionType( Item::Area area, POSTYPE position ) const
{
	/*	
	if(	TP_GUILDWAREHOUSE_START	<= position &&
		TP_GUILDWAREHOUSE_END		> position )
	{
		CString textGuild;
		textGuild.LoadString( IDS_STRING164 );

		CString textStorage;
		textStorage.LoadString( IDS_STRING282 );

		return textGuild + " " + textStorage;
	}
	else if(	TP_FAMILYWAREHOUSE_START	<= position &&
				TP_FAMILYWAREHOUSE_END		> position )
	{
		CString textFamily;
		textFamily.LoadString( IDS_STRING165 );

		CString textStorage;
		textStorage.LoadString( IDS_STRING282 );

		return textFamily + " " + textStorage;
	}
	*/

	switch( area )
	{
	case Item::AreaInventory:
		{
			if( TP_WEAR_START	<= position	&&
				TP_WEAR_END	> position )
			{
				CString textEquippment;
				textEquippment.LoadString( IDS_STRING53 );

				return textEquippment;
			}
			else if(	TP_INVENTORY_START	<= position	&&
						TP_INVENTORY_END	> position )
			{
				CString textInventory;
				textInventory.LoadString( IDS_STRING281 );

				return textInventory;
			}
			else if(	TP_EXTENDED_INVENTORY1_START	<= position	&&
						TP_EXTENDED_INVENTORY1_END		> position )
			{
				CString textExtendedInventory;
				textExtendedInventory.LoadString( IDS_STRING304 );

				return textExtendedInventory + " 1";
			}
			else if(	TP_EXTENDED_INVENTORY2_START	<= position	&&
						TP_EXTENDED_INVENTORY2_END		> position )
			{
				CString textExtendedInventory;
				textExtendedInventory.LoadString( IDS_STRING304 );

				return textExtendedInventory + "2";
			}

			return "?";
		}
	case Item::AreaCashStorage:
		{
			CString textCashStorage;
			textCashStorage.LoadString( IDS_STRING283 );

			return textCashStorage;
		}
	case Item::AreaPrivateStorage:
		{
			CString textStroage;
			textStroage.LoadString( IDS_STRING282 );

			return textStroage;
		}
	}

	return "?";
}


const CString CclientApp::GetText( eGM_POWER power ) const
{
	switch( power )
	{
	case eGM_POWER_MASTER:
		{
			CString textManager;
			textManager.LoadString( IDS_STRING307 );

			return textManager;
		}
	case eGM_POWER_MONITOR:
		{
			CString textMonitor;
			textMonitor.LoadString( IDS_STRING305 );

			return textMonitor;
		}
	case eGM_POWER_PATROLLER:
		{
			CString textStaff;
			textStaff.LoadString( IDS_STRING306 );

			return textStaff;
		}
	case eGM_POWER_AUDITOR:
		{
			return "auditor";
		}
	case eGM_POWER_EVENTER:
		{
			return "eventer";
		}
	default:
		{
			return "?";
		}
	}
}


const CString CclientApp::GetText( RecoveryLog log ) const
{
	switch( log )
	{
	case RecoveryLogSetPlayer:
		{
			CString textPlayerWasChanged;
			textPlayerWasChanged.LoadString( IDS_STRING309 );

			return textPlayerWasChanged;
		}
	case RecoveryLogSetExtendedPlayer:
		{
			CString textPlayerExtendedWasUpdated;
			textPlayerExtendedWasUpdated.LoadString( IDS_STRING311 );

			return textPlayerExtendedWasUpdated;
		}
	case RecoveryLogSetLicense:
		{
			CString textPlayerMatchingWasUpdated;
			textPlayerMatchingWasUpdated.LoadString( IDS_STRING312 );

			return textPlayerMatchingWasUpdated;
		}
	case RecoveryLogSetItem:
		{
			CString textItemWasUpdated;
			textItemWasUpdated.LoadString( IDS_STRING313 );

			return textItemWasUpdated;
		}
	case RecoveryLogSetItemOption:
		{
			CString textItemOptionWasUpdated;
			textItemOptionWasUpdated.LoadString( IDS_STRING314 );

			return textItemOptionWasUpdated;
		}
	case RecoveryLogRemoveItem:
		{
			CString textItemWasRemoved;
			textItemWasRemoved.LoadString( IDS_STRING315 );

			return textItemWasRemoved;
		}
	case RecoveryLogRemoveItemOption:
		{
			CString textItemOptionWasRemoved;
			textItemOptionWasRemoved.LoadString( IDS_STRING316 );

			return textItemOptionWasRemoved;
		}
	case RecoveryLogAddItem:
		{
			CString textItemWasAdded;
			textItemWasAdded.LoadString( IDS_STRING317 );

			return textItemWasAdded;
		}
	case RecoveryLogUpdateItemEndTime:
		{
			CString textItemEndTimeWasUpdated;
			textItemEndTimeWasUpdated.LoadString( IDS_STRING318 );

			return textItemEndTimeWasUpdated;
		}
	case RecoveryLogUpdateItemRemainTime:
		{
			CString textItemRemainedTimeWasUpdated;
			textItemRemainedTimeWasUpdated.LoadString( IDS_STRING319 );

			return textItemRemainedTimeWasUpdated;
		}
	case RecoveryLogSetItemStorage:
		{
			CString textStorageWasUpdated;
			textStorageWasUpdated.LoadString( IDS_STRING320 );

			return textStorageWasUpdated;
		}
	case RecoveryLogAddSkill:
		{
			CString textSkillWasAdded;
			textSkillWasAdded.LoadString( IDS_STRING321 );

			return textSkillWasAdded;
		}
	case RecoveryLogRemoveSkill:
		{
			CString textSkillWasRemoved;
			textSkillWasRemoved.LoadString( IDS_STRING322 );

			return textSkillWasRemoved;
		}
	case RecoveryLogSetSkill:
		{
			CString textSkillWasUpdated;
			textSkillWasUpdated.LoadString( IDS_STRING323 );

			return textSkillWasUpdated;
		}
	case RecoveryLogSetFamily:
		{
			CString textFamilyWasUpdated;
			textFamilyWasUpdated.LoadString( IDS_STRING324 );

			return textFamilyWasUpdated;
		}
	case RecoveryLogRemoveQuest:
		{
			CString textQuestWasRemoved;
			textQuestWasRemoved.LoadString( IDS_STRING325 );

			return textQuestWasRemoved;
		}
	case RecoveryLogFinishSubQuest:
		{
			CString textQuestWasRemoved;
			textQuestWasRemoved.LoadString( IDS_STRING326 );

			return textQuestWasRemoved;
		}
	case RecoveryLogSetQuestItem:
		{
			CString textQuestItemWasUpdated;
			textQuestItemWasUpdated.LoadString( IDS_STRING327 );

			return textQuestItemWasUpdated;
		}
	case RecoveryLogRemoveQuestItem:
		{
			CString textQuestItemWasRemoved;
			textQuestItemWasRemoved.LoadString( IDS_STRING328 );

			return textQuestItemWasRemoved;
		}
	case RecoveryLogSetGuild:
		{
			CString textGuildWasUpdated;
			textGuildWasUpdated.LoadString( IDS_STRING329 );

			return textGuildWasUpdated;
		}
	case RecoveryLogSetGuildRank:
		{
			CString textGuildRankWasUpdated;
			textGuildRankWasUpdated.LoadString( IDS_STRING330 );

			return textGuildRankWasUpdated;
		}
	case RecoveryLogKickGuildMember:
		{
			CString textGuildMemberWasKicked;
			textGuildMemberWasKicked.LoadString( IDS_STRING331 );

			return textGuildMemberWasKicked;
		}
	case RecoveryLogSetUser:
		{
			CString textUseWasUpdated;
			textUseWasUpdated.LoadString( IDS_STRING332 );

			return textUseWasUpdated;
		}
	case RecoveryLogAddPlayerToUser:
		{
			CString textPlayerWasAdded;
			textPlayerWasAdded.LoadString( IDS_STRING333 );

			return textPlayerWasAdded;
		}
	case RecoveryLogRemovePlayerFromUser:
		{
			CString textPlayerWasRemoved;
			textPlayerWasRemoved.LoadString( IDS_STRING334 );

			return textPlayerWasRemoved;
		}
	case RecoveryLogRestorePlayerToUser:
		{
			CString textPlayerWasRecovered;
			textPlayerWasRecovered.LoadString( IDS_STRING335 );

			return textPlayerWasRecovered;
		}
	case RecoveryPermissionAddIp:
		{
			CString textConnectableIpWasAdded;
			textConnectableIpWasAdded.LoadString( IDS_STRING336 );

			return textConnectableIpWasAdded;
		}
	case RecoveryPermissionRemoveIp:
		{
			CString textConnectableIpWasRemoved;
			textConnectableIpWasRemoved.LoadString( IDS_STRING337 );

			return textConnectableIpWasRemoved;
		}
	case RecoveryOperatorAdd:
		{
			CString textOperatorWasAdded;
			textOperatorWasAdded.LoadString( IDS_STRING338 );

			return textOperatorWasAdded;
		}
	case RecoveryOperatorRemove:
		{
			CString textOperatorWasRemoved;
			textOperatorWasRemoved.LoadString( IDS_STRING339 );

			return textOperatorWasRemoved;
		}
	case RecoveryOperatorSet:
		{
			CString textOperatorWasUpdated;
			textOperatorWasUpdated.LoadString( IDS_STRING340 );

			return textOperatorWasUpdated;
		}
	case RecoveryOperatorAddIp:
		{
			CString textOperatorIpWasAdded;
			textOperatorIpWasAdded.LoadString( IDS_STRING341 );

			return textOperatorIpWasAdded;
		}
	case RecoveryOperatorRemoveIp:
		{
			CString textOperatorIpWasRemoved;
			textOperatorIpWasRemoved.LoadString( IDS_STRING342 );

			return textOperatorIpWasRemoved;
		}
	default:
		{
			return "?";
		}
	}
}


// 080304 LUJ, 로그를 표시한다
void CclientApp::Log( const TCHAR* text, ... )
{
	CMainFrame* frame = dynamic_cast< CMainFrame* >( m_pMainWnd );

	if( ! frame )
	{
		ASSERT( 0 );
		return;
	}

	TCHAR buffer[ 2048 ] = { 0 };
	{
		va_list argument;
		va_start( argument, text );
		_vstprintf( buffer, text, argument);
		va_end( argument);
	}

	const CTime time( CTime::GetCurrentTime() );

	CString log;

	log.Format(
		_T( "[%04d.%02d.%02d %02d:%02d:%02d] %s\n" ),
		time.GetYear(),
		time.GetMonth(),
		time.GetDay(),
		time.GetHour(),
		time.GetMinute(),
		time.GetSecond(),
		buffer );

	frame->Log( log );

	// 파일 출력
	{
		CString name;
		{
			const CString prefix( _T( "RecoveryClient" ) );
			CString infix;
			infix.Format(
				_T( "%04d%02d%02d" ),
				time.GetYear(),
				time.GetMonth(),
				time.GetDay() );
			const CString postfix( _T( "log" ) );

			name = prefix + "." + infix + "." + postfix;
		}		

		CStdioFile file;

		// 쓰기 우선으로 열되, 없으면 생성한다
		if( !	file.Open( name, CFile::modeWrite  ) &&
			!	file.Open( name, CFile::modeCreate | CFile::modeWrite ) )
		{
			return;
		}

		file.Seek( file.GetLength(), 0 );
		file.WriteString( log );
	}
}
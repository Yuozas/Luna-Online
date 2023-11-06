// MainTitle.cpp: implementation of the CMainTitle class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "MainTitle.h"
#include "MainGame.h"

#include "cWindowSystemFunc.h"
#include "WindowIDEnum.h"
#include "MHCamera.h"

#include "AppearanceManager.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cEditBox.h"
#include "./Interface/cResourceManager.h"
#include "../[CC]Header/GameResourceManager.h"
#include "ItemManager.h"
#include "KyungGongManager.h"
#include "ChatManager.h"
#include "MHTimeManager.h"

#include "FilteringTable.h"
#include "cMsgBox.h"

#include "mhMap.h"
#include "./Input/Mouse.h"
#include "./Input/Keyboard.h"
#include "MotionManager.h"
#include "ServerListDialog.h"
#include "CharSelect.h"
//#include "DissolutionManager.h"
#include "cMonsterSpeechManager.h"

#ifdef _GMTOOL_
#include "GMToolManager.h"
#endif

#ifdef TAIWAN_LOCAL
#include "BillingManager.h"
#endif

#include "GMNotifyManager.h"
#include "ObjectManager.h"
#include "MoveManager.h"

#include "UserInfoManager.h"



// 061225 LYW --- Add for test.
#include "./Input/UserInput.h"

#include "../[cc]skill/client/manager/skillmanager.h"
// 070216 LYW --- MainTitle : Include header file of conduct manager.
#include "ConductManager.h"
// desc_hseos_성별선택01
// S 성별선택 추가 added by hseos 2007.06.16
#include "CharMakeManager.h"
// E 성별선택 추가 added by hseos 2007.06.16

// 080109 LYW --- MainTitle : 숫자 패드 추가 처리.
#include "./Interface/cNumberPadDialog.h"

#include "PetManager.h"

#define _LOGOWINDOW_

#ifdef _NPROTECT_
#include "NProtectManager.h"
#endif

//#include "../[CC]Header/GameResourceManager.h"
#include "../Interface/cScriptManager.h"
#include "GlobalEventFunc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern char g_DistributeAddr[16];
extern WORD g_DistributePort;
extern char g_AgentAddr[16];
extern WORD g_AgentPort;
extern char g_szHeroIDName[];
extern char g_szHeroKey[];
extern int	g_nServerSetNum;
extern char g_CLIENTVERSION[];
extern ScriptCheckValue g_Check;
extern HWND _g_hWnd;
extern BOOL alreadyDecypte;
#define ALPHA_PROCESS_TIME	1200
#define WAIT_CONNECT_TIME	600000	//에이젼트로 접속기다림 시간 90초

// 061225 LYW --- Add for test.
/*
VECTOR3 v3Eye	= { 11896.298828f, 6459.166992f, 4867.166992f };	
VECTOR3 v3Rot	= { 87.848961f, 89.454964f, 0.0f };
*/
/*
VECTOR3 v3Eye	= { 8996.299f, 5059.17f, 6567.167f };	
VECTOR3 v3Rot	= { 84.899f, 88.665f, 0.0f };
*/


void SendOutMsg()
{
	MSGBASE msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_REQUEST_DISTOUT;
	NETWORK->Send(&msg, sizeof(msg));
}

BOOL bFirst;
GLOBALTON(CMainTitle)
CMainTitle::CMainTitle()
{
#ifdef TAIWAN_LOCAL
	m_pAdvice		= NULL;
#endif
	m_pLogoWindow	= NULL;
	m_bInit			= FALSE;

	m_pServerListDlg = NULL;
	memset( m_DistributeAddr, 0, 16 );
	m_DistributePort = 0;
	m_bServerList = FALSE;

	m_bDisconntinToDist = FALSE;
	m_bDynamicDlg = FALSE;
}

CMainTitle::~CMainTitle()
{	
}

static BOOL g_bResourceLoaded = FALSE;

//cImageSelf image;

BOOL CMainTitle::Init(void* pInitParam)
{
	// 080507 LUJ, 리소스 관리자를 통해 읽어오도록 한다
	{
		IMAGE_NODE* topBar = RESRCMGR->GetInfo( 24 );
		
		if( topBar )
		{
			m_imageLoginBarTop.LoadSprite( topBar->szFileName );
		}

		IMAGE_NODE* bottomBar = RESRCMGR->GetInfo( 25 );

		if( bottomBar )
		{
			m_imageLoginBarBottom.LoadSprite( bottomBar->szFileName );
		}
	}
	ScriptCheckValue check;
	SetScriptCheckValue( check );

	if( check.mValue != g_Check.mValue )
	{
#ifndef _GMTOOL_1
		// 090120 LUJ, 패치 버전을 최초로 돌린다
		{
			const char* const	versionFile = "LunaVerInfo.ver";
			FILE* const			file		= fopen( versionFile, "r+" );
			if( file )
			{
				// 090109 LUJ, 4글자로 구성된 버전 헤더를 파일에서 읽어와 문자열 변수에 담는다
				const size_t headerSize = 4;
				char header[ headerSize + 1 ] = { 0 };
				fread(
					header,
					headerSize,
					sizeof( *header ),
					file );

				// 090109 LUJ, 전체 패치를 수행할 버전 번호를 만든다
				char versionText[ MAX_PATH ] = { 0 };
				sprintf(
					versionText,
					"%s00000000",
					header );
				// 090109 LUJ, 패치 버전 문자열을 파일에 쓴다
				fseek(
					file,
					0,
					SEEK_SET );
				fwrite(
					versionText,
					sizeof( *versionText ),
					strlen( versionText ),
					file );
				fclose( file );
			}
		}

		PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
		return FALSE;
#endif
	}
	m_pLogoWindow	= NULL;
	m_bInit			= FALSE;
	m_pServerListDlg = NULL;
	memset( m_DistributeAddr, 0, 16 );
	m_DistributePort = 0;
	//
	
	m_bDisconntinToDist = FALSE;
	m_bNoDiconMsg		= FALSE;	//KES 유저가 접속을 끊었을때는 메세지를 안띄우기위한 변수
	m_bDynamicDlg = FALSE;


	NETWORK->SetCurState(this);

	// 게임리소스 로드///////////////////////////////////////////////////////////////////////

	if(MAP->IsInited() == FALSE)
	{
		// 061214 LYW --- Change map.
		//MAP->InitMap(103);
		// 061225 LYW --- Change title map.
		//MAP->InitMap(25);
		MAP->InitMap(200) ;
	}

	if(g_bResourceLoaded == FALSE)
	{
		EFFECTMGR->Init();	//KES MainGame.cpp에서 옮겨보았다. 이미지라도 뜬 상태에서 오래걸리는게 낫다.

		FILTERTABLE->Init();
		CHATMGR->Init();
		MON_SPEECHMGR->Init();		
		//이걸 아래로 내렸더니 경로가 바뀌어서 파일을 로드할 수 없다.
		//아래중 경로를 바꾸어 놓는 것이 있으면 제자리로 돌려놓으시오~.
		MOTIONMGR->LoadMotionList();
		//SKILLMGR->Init();
		// 070216 LYW --- MainTitle : Initialize conduct manager.
		CONDUCTMGR->InitInfoTable() ;
		
		ITEMMGR->LoadItemList();
		ITEMMGR->LoadItemToolTipList();
		ITEMMGR->LoadItemEffectList();
		ITEMMGR->LoadGiftEvent();

		PETMGR->LoadPetInfo();

		//ITEMMGR->LoadRareItemInfo();
		
		GAMERESRCMNGR->LoadMonsterList();
		GAMERESRCMNGR->LoadMonsterInfoInMapList(); //060608 - Add by wonju
		GAMERESRCMNGR->LoadMapChangeArea(); //060608 - Add by wonju
		GAMERESRCMNGR->LoadUserCommendList(); //060727 - Add by wonju
	//	GAMERESRCMNGR->LoadSMonsterList();
		GAMERESRCMNGR->LoadNpcChxList();
		GAMERESRCMNGR->LoadNpcList(GAMERESRCMNGR->m_NpcList);
		GAMERESRCMNGR->LoadExpPoint();
		GAMERESRCMNGR->LoadFishingExpPoint();		// 080424 NYJ --- 낚시경험치
		GAMERESRCMNGR->LoadPreDataTable();
		// JSD
		GAMERESRCMNGR->LoadServerList();
		
		//
		GAMERESRCMNGR->LoadShopItemDupList();

		KYUNGGONGMGR->Init();
		
		APPEARANCEMGR->Init();
		//GAMERESRCMNGR->PreLoadData();	//조정이 필요 KES 070805
		//DISSOLUTIONMGR->Init();	//KES


		g_bResourceLoaded = TRUE;
	}
	/////////////////////////////////////////////////////////////////////////////////////////

	/*
	VECTOR3 vPivot;
	vPivot.x = 25408;
	vPivot.y = 6545;
	vPivot.z = 4004;

	CAMERA->InitCamera(0, 0.0f, -1.0f, 2000, &vPivot);// 거리 조정 
	CAMERA->SetCameraMode(eCM_LOGIN);
	CAMERA->SetCurCamera(0);
	*/

	// MHCamera 는 넣은 값을 뭔가의 계산을 통해 변형해서 쓰기 때문에 
	// 직접 카메라를 세팅하였다.
	//VECTOR3 v3Eye = { 19025.675781f, 156.450745f, 370.196259f };	
	// 061214 LYW --- Change camera position.
	/*
	VECTOR3 v3Eye = { 19025.675781f, 10.0f, 370.196259f };	
	VECTOR3 v3Rot = { 0.198968f, -0.010472f, 0.0f };
	*/
	/*
	VECTOR3 v3Eye = { 20000.0f, 2000.0f, 12000.0f };	
	VECTOR3 v3Rot = { 0.0f, 1050.0f, 0.0f };
	*/
	/*
	VECTOR3 v3Eye	= { 16000.0f, 3000.0f, 31500.0f };	
	VECTOR3 v3Rot	= { -0.15f, 0.02f, 0.0f };
	*/
	/*
	VECTOR3 v3Eye	= { 8996.299f, 5059.17f, 6567.167f };	
	VECTOR3 v3Rot	= { 84.899f, 88.665f, 0.0f };
	*/
	VECTOR3 v3Eye	= { 11896.298828f, 6459.166992f, 4867.166992f };	
	VECTOR3 v3Rot	= { 87.848961f, 89.454964f, 0.0f };
	g_pExecutive->GetGeometry()->SetCameraPos(&v3Eye, 0);
	g_pExecutive->GetGeometry()->SetCameraAngleRad(&v3Rot, 0);

	CAMERA->SetCameraMode(eCM_LOGIN);
	CAMERA->SetCurCamera(0);

/* test
	MHCAMERAPOS MCPos;
	MCPos.m_AngleX = -2.0f;
	MCPos.m_AngleY = 17.0f;
	MCPos.m_Dist = 2000;
	MCPos.m_Pivot.x = 27740;
	MCPos.m_Pivot.y = 4960;
	MCPos.m_Pivot.z = 9104;	
	CAMERA->SetMovePos(eCP_CHARSEL, &MCPos);

	CAMERA->InitCamera(0, -2.0f, 17.0f, 2000, &MCPos.m_Pivot);// 거리 조정 
	CAMERA->SetCameraMode(eCM_LOGIN);
	CAMERA->SetCurCamera(0);	

	// 캐릭터 생성 
	vPivot.x = 24480;
	vPivot.y = 6045;
	vPivot.z = 9604;
	MCPos.m_AngleX = -5.0f;
	MCPos.m_AngleY = 0.0f;
	MCPos.m_Dist = 2000;
	MCPos.m_Pivot = vPivot;
	CAMERA->SetMovePos(eCP_CHARMAKE, &MCPos);
//*/
	
	WINDOWMGR->SetcbDropProcess(cbDragDropProcess);
//	WINDOWMGR->CreatMousePoint();
//

	CreateMainTitle_m();	
	m_pLogoWindow = WINDOWMGR->GetWindowForID( MT_LOGODLG );
	if( m_pLogoWindow )
		m_pLogoWindow->SetActive( TRUE );

#ifdef TAIWAN_LOCAL
	m_pAdvice = WINDOWMGR->GetWindowForID( CNA_CNADVICEDLG );
#endif

	m_pServerListDlg = (CServerListDialog*)WINDOWMGR->GetWindowForID( SL_SERVERLISTDLG );
	if( m_pServerListDlg )
	{
		m_pServerListDlg->Linking();
		m_pServerListDlg->SetActive( FALSE );
	}
//
	WINDOWMGR->AfterInit();

///	MHTIMEMGR->Process();
	m_bWaitConnectToAgent = FALSE;
	bFirst = TRUE;
	
	
#ifndef _LOGOWINDOW_
	m_pLogoWindow = NULL;	
	m_bServerList = TRUE;
#endif
	
#ifdef TAIWAN_LOCAL
	BILLINGMGR->Init();	
#endif

	//trustpak 2005/04/01
	
#ifndef TAIWAN_LOCAL
	//InitMainTitleMonsters();
#endif
	///

	// 070208 LYW --- MainTitle : Add function to setting position and scale of image.
	SetPositionScale() ;

//---KES GM툴 로그아웃 처리 추가
//	GMTOOLMGR->LogOut();
//------------------------------

#ifdef _UI_SERVERSET_
//	SCRIPTMGR->GetImage( 144, &m_ServerSetDesc );
#endif

	return TRUE;
}

// 070208 LYW --- MainTitle : Add function to setting position and scale of image.
void CMainTitle::SetPositionScale()
{
	DISPLAY_INFO	dispInfo ;

	GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;

	switch( dispInfo.dwWidth )
	{
	case 800 :
		{
			m_scale.x = 0.79f ;
			m_scale.y = 0.75f ;

			m_v2BottomPos.y = 504.0f ;
		}
		break ;

	case 1024 :
		{
			m_scale.x = 1.0f ;
			m_scale.y = 1.0f ;

			m_v2BottomPos.y = 640.0f ;
		}
		break ;

	case 1280 :
		{
			m_scale.x = 1.25f ;
			m_scale.y = 1.25f ;

			m_v2BottomPos.y = 864.0f ;
		}
		break ;
	}
}


void CMainTitle::Release(CGameState* pNextGameState)
{
	//image.Release();	

	m_imageLoginBarTop.Release();
	m_imageLoginBarBottom.Release();	


#ifndef TAIWAN_LOCAL
	//trustpak 2005/04/01	

	//RemoveMainTitleMonsters();

	///
#endif

	WINDOWMGR->DestroyWindowAll();
	RESRCMGR->ReleaseResource(24);

	if(pNextGameState == NULL)
		MAP->Release();



}

void CameraRotate(CKeyboard& Keyboard,CMouse& Mouse);
void CameraWheelZoom(CKeyboard& Keyboard,CMouse& Mouse);
#include "ObjectGuagen.h"
void CMainTitle::Process()
{
#ifndef TAIWAN_LOCAL
	//trustpak 2005/04/01
//	ProcessMainTitleMonsters();
	///
#endif

	//엔진이상 -_-
	if(bFirst == TRUE)	//이렇게 해도 깨지는데?
	{
		bFirst = FALSE;
		CAMERA->MouseRotate(1,0);
		m_dwStartTime = MHTIMEMGR->GetNewCalcCurTime();
	}

	if( m_pLogoWindow )
	{
		DWORD dwElapsed = MHTIMEMGR->GetNewCalcCurTime() - m_dwStartTime;
		
		if( dwElapsed > ALPHA_PROCESS_TIME )
		{
			m_pLogoWindow->SetAlpha( 0 );
			WINDOWMGR->AddListDestroyWindow( m_pLogoWindow );
			m_pLogoWindow = NULL;
			
#ifdef TAIWAN_LOCAL
			m_pAdvice->SetActive( TRUE );
#else
			m_bServerList = TRUE;
#endif
		}
		else
		{
			m_pLogoWindow->SetAlpha( (BYTE)(255 - ( dwElapsed * 255 / ALPHA_PROCESS_TIME )) );
		}
	}

	if( m_bServerList )
	{
		m_pServerListDlg->SetActive( TRUE );
		m_bServerList = FALSE;
	}

	// Dist Server에 접속되었다면...
	if( m_bInit )
	{
		if( g_szHeroIDName[0] )
		{
			if(	!m_bWaitConnectToAgent )
			{
				ScriptCheckValue check;
				SetScriptCheckValue( check );

				if( check.mValue != g_Check.mValue )

				{
#ifndef _GMTOOL_1
					// 090120 LUJ, 패치 버전을 최초로 돌린다
					{
						const char* const	versionFile = "LunaVerInfo.ver";
						FILE* const			file		= fopen( versionFile, "r+" );

						if( file )
						{
							// 090109 LUJ, 4글자로 구성된 버전 헤더를 파일에서 읽어와 문자열 변수에 담는다
							const size_t headerSize = 4;
							char header[ headerSize + 1 ] = { 0 };
							fread(
								header,
								headerSize,
								sizeof( *header ),
								file );

							// 090109 LUJ, 전체 패치를 수행할 버전 번호를 만든다
							char versionText[ MAX_PATH ] = { 0 };
							sprintf(
								versionText,
								"%s00000000",
								header );
							// 090109 LUJ, 패치 버전 문자열을 파일에 쓴다
							fseek(
								file,
								0,
								SEEK_SET );
							fwrite(
								versionText,
								sizeof( *versionText ),
								strlen( versionText ),
								file );
							fclose( file );
						}
					}

					PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
					return;
#endif

				}
/*				MSG_LOGIN_SYN msg ;														// 로그인 정보를 담을 메시지 구조체를 선언한다.
				memset(&msg, 0, sizeof(MSG_LOGIN_SYN)) ;								// 메시지 구조체를 초기화 한다.

				msg.Category = MP_USERCONN ;											// 카테고리와 프로토콜을 세팅한다.
				msg.Protocol = MP_USERCONN_LOGIN_SYN ;

				SafeStrCpy(msg.id, g_szHeroIDName, 21 ) ;				// 아이디 세팅.
				SafeStrCpy(msg.pw, g_szHeroKey, 21 ) ;					// 비밀번호 세팅.

				SafeStrCpy(msg.Version,g_CLIENTVERSION, 16 ) ;							// 클라이언트 버전 세팅.

				msg.AuthKey = TITLE->GetDistAuthKey() ;									// 인증키를 담는다.

				NETWORK->Send(&msg,sizeof(msg)) ;										// 메시지를 전송한다.

				//---KES 로그인 실패후 재 로그인이 잘 안되던 것 수정
				NETWORK->m_bDisconnecting = FALSE;
				//---------------------------------------------
*/
				TITLE->StartWaitConnectToAgent( TRUE ) ;								// 에이전트 연결을 기다린다.

				//USERINFOMGR->SetUserID( g_szHeroIDName ) ;								// 유저 아이디를 설정한다.
			cDialog* pDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
				if ( !pDlg->IsActive() )
				{
					pDlg->SetActive(true);
				}
			}
		}
		else
		{
/*			if( m_bDynamicDlg == FALSE )
			{
				cDialog* pDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
				if( pDlg )
					if( !pDlg->IsActive() )
					{
						pDlg->SetDisable( FALSE );
						pDlg->SetActive( TRUE );
						cEditBox* pEdit = (cEditBox*)pDlg->GetWindowForID( MT_IDEDITBOX );
						pEdit->SetFocusEdit( TRUE );

						// 080109 LYW --- MainTitle : 2차 비밀번호에 따른 인터페이스 출력.
						cDialog* pPad = NULL ;
						pPad = WINDOWMGR->GetWindowForID( NUMBERPAD_DLG ) ;

						if( !pPad )
						{
							//MessageBox(NULL, "Failed to receive number pad.", "login", MB_OK) ;
						}
						else
						{
							pPad->SetActive(TRUE) ;
						}
					}	
			}
*/
			//접속기다리는 중이라면
			if( m_bWaitConnectToAgent == TRUE )
			{
				DWORD dwElapsed = gCurTime - m_dwWaitTime;

				if( dwElapsed > WAIT_CONNECT_TIME )
				{
					//취소버튼을 누른것 처럼
					cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID( MBI_WAIT_LOGINCHECK );
					if( pMsgBox )
						pMsgBox->ForceClose();

					m_bWaitConnectToAgent = FALSE;

					// 070125 LYW --- MainTitle : Modified message number.
					//WINDOWMGR->MsgBox( MBI_TIMEOVER_TOCONNECT, MBT_OK, CHATMGR->GetChatMsg( 445 ) );
					WINDOWMGR->MsgBox( MBI_TIMEOVER_TOCONNECT, MBT_OK, CHATMGR->GetChatMsg( 302 ) );
					//서버리스트를 감추자???
					m_pServerListDlg->SetActive( FALSE );
					m_bServerList = FALSE;

					// 080111 LYW --- MainTitle : 숫자 패드 비활성화 처리.
					cDialog* pDlg = NULL ;
					pDlg = WINDOWMGR->GetWindowForID( NUMBERPAD_DLG ) ;

					if( !pDlg ) return ;
					pDlg->SetActive(FALSE) ;
				}
			}
			else
			{
				if( m_bDynamicDlg == FALSE )
				{
					cDialog* pDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
					if( pDlg )
						if( !pDlg->IsActive() )
						{
							pDlg->SetDisable( FALSE );
							pDlg->SetActive( TRUE );
							cEditBox* pEdit = (cEditBox*)pDlg->GetWindowForID( MT_IDEDITBOX );
							pEdit->SetFocusEdit( TRUE );

							// 080109 LYW --- MainTitle : 2차 비밀번호에 따른 인터페이스 출력.
							cDialog* pPad = NULL ;
							pPad = WINDOWMGR->GetWindowForID( NUMBERPAD_DLG ) ;

							if( !pPad )
							{
								//MessageBox(NULL, "Failed to receive number pad.", "login", MB_OK) ;
							}
							else
							{
								pPad->SetActive(TRUE) ;
							}
						}	
				}
			}
		}
/*
		if( m_bDisconntinToDist == TRUE ) // wait for disconnect
		{
			DWORD dwElapsed = gCurTime - m_dwDiconWaitTime;
			if( dwElapsed > 10 * 1000 ) //10 sec
			{
				if( NETWORK->IsConnected() )
				{
					NETWORK->Disconnect();
				}

				m_dwDiconWaitTime = gCurTime;
			}
		}
*/
	}	

	// 061217 LYW --- Add for test.
	/*
	MHCAMERADESC* pCurCamera = CAMERA->GetCameraDesc() ;
	if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_W ) )
	{
		v3Eye.z += 50.0f ;
	}
	// 뒤
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_S ) )
	{
		v3Eye.z -= 50.0f ;
	}
	// 상
	if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_Q ) )
	{
		v3Eye.y += 50.0f ;
	}
	// 하
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_E ) )
	{
		v3Eye.y -= 50.0f ;
	}
	// 좌
	if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_A ) )
	{
		v3Eye.x -= 50.0f ;
	}
	// 우
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_D ) )
	{
		v3Eye.x += 50.0f ;
	}

	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_F ) )
	{
		v3Eye.x = 14046.298827f ;
		v3Eye.y = 6609.169922f ;
		v3Eye.z = 5667.166992f ;
		v3Rot.x = 87.848958f ;
		v3Rot.y = 89.564987f ;
		v3Rot.z = 0.0f ;
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_K ) )
	{
		v3Rot.x -= 0.01f ;
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_L ) )
	{
		v3Rot.x += 0.01f ;
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_O ) )
	{
		v3Rot.y -= 0.01f ;
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_P ) )
	{
		v3Rot.y += 0.01f ;
	}

	g_pExecutive->GetGeometry()->SetCameraPos(&v3Eye, 0);
	g_pExecutive->GetGeometry()->SetCameraAngleRad(&v3Rot, 0);
	// test end.
	*/
}


void CMainTitle::BeforeRender()
{
	g_pExecutive->GetRenderer()->BeginRender(0,0,0);
	g_pExecutive->GetRenderer()->EndRender();
	MAP->Process( gCurTime );
}
void CMainTitle::AfterRender()
{	
	m_imageLoginBarTop.RenderSprite(&m_scale,0,0,&m_v2TopPos,0xffffffff);
	m_imageLoginBarBottom.RenderSprite(&m_scale,0,0,&m_v2BottomPos,0xffffffff);	

#ifdef _UI_SERVERSET_
	DISPLAY_INFO	dispInfo;
	GAMERESRCMNGR->GetDispInfo( &dispInfo );
	VECTOR2 vPos = { dispInfo.dwWidth - (m_ServerSetDesc.GetImageRect()->right - m_ServerSetDesc.GetImageRect()->left) - 15 , 15 };

	m_ServerSetDesc.RenderSprite( NULL, NULL, 0.f, &vPos, 0xffffffff );
#endif

	WINDOWMGR->Render();

	// 061217 LYW --- Add for test.
	/*
	char tempStr[ 128 ] ;
	RECT rc ;
	rc.left = 10 ;
	rc.top = 500 ;
	rc.right = 1000 ;
	rc.bottom = 700 ;

	sprintf( tempStr, "xpos = %f", v3Eye.x ) ;
	CFONT_OBJ->RenderFont(0, tempStr, strlen( tempStr ), &rc, RGB( 255, 0, 0 ) ) ;
	sprintf( tempStr, "ypos = %f", v3Eye.y ) ;
	rc.top += 20 ;
	CFONT_OBJ->RenderFont(0, tempStr, strlen( tempStr ), &rc, RGB( 255, 0, 0 ) ) ;
	sprintf( tempStr, "zpos = %f", v3Eye.z ) ;
	rc.top += 20 ;
	CFONT_OBJ->RenderFont(0, tempStr, strlen( tempStr ), &rc, RGB( 255, 0, 0 ) ) ;
	// end test.

	sprintf( tempStr, "Rotate X = %f", v3Rot.x ) ;
	rc.top += 20 ;
	CFONT_OBJ->RenderFont(0, tempStr, strlen( tempStr ), &rc, RGB( 0, 255, 0 ) ) ;

	sprintf( tempStr, "Rotate Y = %f", v3Rot.y ) ;
	rc.top += 20 ;
	CFONT_OBJ->RenderFont(0, tempStr, strlen( tempStr ), &rc, RGB( 0, 255, 0 ) ) ;

	sprintf( tempStr, "Rotate Z = %f", v3Rot.z ) ;
	rc.top += 20 ;
	CFONT_OBJ->RenderFont(0, tempStr, strlen( tempStr ), &rc, RGB( 0, 255, 0 ) ) ;
	*/
}
void CMainTitle::NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg)
{
	switch(Category)
	{
	case MP_USERCONN:
		{
			switch(Protocol) 
			{
#ifdef _GMTOOL_
			case MP_CHEAT_CHECKIP_ACK:
				{
					m_bInit = TRUE;
				}
				break;
			case MP_CHEAT_CHECKIP_NACK:
				{
					Cheat_CheckIp_Nack() ;
				}
				break;
#endif
			case MP_USERCONN_USE_DYNAMIC_ACK:
				{
					UserConn_Use_Dynamic_Ack( pMsg ) ;
				}
				break;
			case MP_USERCONN_USE_DYNAMIC_NACK:
				{
//					WINDOWMGR->MsgBox( MBI_CANTUSEDYNAMIC, MBT_OK, CHATMGR->GetChatMsg( 702 ) );
//					//msgbox
				}
				break;

			case MP_USERCONN_DIST_CONNECTSUCCESS:
				{
					UserConn_Dist_ConnectSuccess( pMsg ) ;
				}
				return;
			case MP_USERCONN_LOGIN_ACK:
				{
					UserConn_Login_Ack( pMsg ) ;
				}
				break;
			case MP_USERCONN_SERVER_NOTREADY:
				{
					UserConn_Server_NotReady() ;
				}
				break;

			case MP_USERCONN_LOGIN_NACK:
				{
					UserConn_Login_Nack( pMsg ) ;
				}
				break;
			case MP_USERCONN_CHARACTERLIST_ACK:
				{
					UserConn_CharacterList_Ack( pMsg ) ;
				}
				return;
			case MP_USERCONN_CHARACTERLIST_NACK:
				{
					UserConn_CharacterList_Nack() ;
				}
				return;

			case MP_USERCONN_REMAINTIME_NOTIFY:
				{
					UserConn_RemainTime_Notify( pMsg ) ;
				}
				break;
				
			case MP_USERCONN_AGENT_CONNECTSUCCESS:
				{
					UserConn_Agent_ConnectSuccess( pMsg ) ;
				}
				break;
			// desc_hseos_성별선택01
			// S 성별선택 추가 added by hseos 2007.06.15
			case MP_USERCONN_USER_SEXKIND:
				{
					UserConn_Character_SexKind(pMsg);
				}
				return;
			// E 성별선택 추가 added by hseos 2007.06.15
			}
		}
		break;
	case MP_CHEAT:
		{
			switch(Protocol) 
			{
				case MP_CHEAT_EVENTNOTIFY_ON:
				{
					Cheat_EventNotify_On( pMsg ) ;
				}
				break;
		
				case MP_CHEAT_EVENTNOTIFY_OFF:
				{
					Cheat_EventNotify_Off( pMsg ) ;
				}	
				break;
			}
		}
		break;		
	}
//	LOG(EC_UNKNOWN_PROTOCOL);
}



void CMainTitle::OnLoginError(DWORD errorcode,DWORD dwParam)
{
	//접속중입니다 메세지박스 지우기
	cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID( MBI_WAIT_LOGINCHECK );
	if( pMsgBox )
		WINDOWMGR->AddListDestroyWindow( pMsgBox );

	m_bWaitConnectToAgent = FALSE;

	// 080111 LYW --- MainTitle : 숫자 패드 비활성화 처리.
	cDialog* pDlg = NULL ;
	pDlg = WINDOWMGR->GetWindowForID( NUMBERPAD_DLG ) ;

	if( !pDlg ) return ;

	if( errorcode != LOGIN_ERROR_WRONGIDPW)
	{
		pDlg->SetActive(FALSE) ;
	}
	
	ySWITCH(errorcode)
		yCASE(LOGIN_ERROR_INVALID_VERSION)
			Login_Error_Invalid_Version() ;
		yCASE(LOGIN_ERROR_OVERLAPPEDLOGIN)
		{
			for (int i=0; i<500; i++)
				{
					if(i<499)
					{
						MSGBASE msg;
						msg.Category = MP_USERCONN;
						msg.Protocol = MP_USERCONN_FORCE_DISCONNECT_OVERLAPLOGIN;
						NETWORK->Send(&msg,sizeof(msg));
					}
					else if (i==499)
					{
						//void* p;
						//MT_LogInOkBtnFunc( MT_OKBTN, p, WE_BTNCLICK );
			Login_Error_OverLappedLogin() ;	
					}
				}
		}
		yCASE(LOGIN_ERROR_OVERLAPPEDLOGININOTHERSERVER)
			Login_Error_OverLappedLoginiNotherServer( dwParam ) ;
		yCASE(LOGIN_ERROR_NOAGENTSERVER)
			Login_Error_NoAgentServer() ;			
		yCASE(LOGIN_ERROR_NODISTRIBUTESERVER)
			Login_Error_NoDistributeServer() ;
		yCASE(LOGIN_ERROR_INVALIDUSERLEVEL)
			Login_Error_InvalidUserLevel() ;
		yCASE(LOGIN_ERROR_DISTSERVERISBUSY)
			Login_Error_DistServerIsBusy() ;
		yCASE(LOGIN_ERROR_WRONGIDPW)
			Login_Error_WrongIdPw() ;
		yCASE(LOGIN_ERROR_BLOCKUSERLEVEL)
			Login_Error_BlockUserLevel( dwParam ) ;			
		yCASE(LOGIN_ERROR_INVALID_IP)
			Login_Error_Invalid_Ip() ;
		yCASE(LOGIN_ERROR_DISTCONNET_ERROR)
			Login_Error_DistConnet_Error() ;
		yCASE(LOGIN_ERROR_MAXUSER)
			Login_Error_MaxUser() ;			
		yCASE(LOGIN_ERROR_MINOR_INADULTSERVER)
			Login_Error_Minor_InadultServer() ;
		yCASE(LOGIN_ERROR_SECEDEDACCOUNT)
			Login_Error_SecededAccount() ;			
		yCASE(LOGIN_ERROR_NOT_CLOSEBETAWINNER)
			Login_Error_Not_CloseBetaWinner() ;
		yCASE(LOGIN_ERROR_NOREMAINTIME)
			Login_Error_NoreMainTime() ;
		yCASE(LOGIN_ERROR_NOIPREGEN)
			Login_Error_NoIpRegen() ;	
		// 080111 LYW --- MainTitle : 보안 비밀번호 관련 에러 처리 추가.
		yCASE(LOGIN_ERROR_INVALID_SEC_PASS)
			Login_Error_Invalid_Sec_Pass() ;	
		yCASE(LOGIN_ERROR_CHANGE_SEC_PASS)
			Login_Error_Change_Sec_Pass() ;	
 		yCASE(LOGIN_ERROR_FILE_CRC)
 			Login_Error_File_CRC() ;
		yCASE(LOGIN_ERROR_LOGINSESSION_INVALID)
			Login_Error_LoginSession_Invalid();
	yENDSWITCH
}

void CMainTitle::OnDisconnect()
{
	if(m_bDisconntinToDist)		//에이젼트로 접속을 위한 끊김
	{
		if(NETWORK->ConnectToServer(g_AgentAddr, g_AgentPort) == FALSE)
		{
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_VERCHKERROR, MBT_OK, CHATMGR->GetChatMsg(7));
			cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( pIDDlg && pMsgBox )
			{
				pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
				// 080111 LYW --- GlovalEventFunc : 숫자 패드 비활성화 처리.
				cDialog* pDlg = NULL ;
				pDlg = WINDOWMGR->GetWindowForID( NUMBERPAD_DLG ) ;

				if( !pDlg ) return ;

				pDlg->SetActive(FALSE) ;
			}
		}
		
		m_bDisconntinToDist = FALSE;
	}
	else if( !m_bNoDiconMsg )	//유저의 강제 접속 끊음이 아니면
	{
		OnLoginError(LOGIN_ERROR_DISTCONNET_ERROR, 0);

		//로그인창 디스에이블 추가.
		cDialog* pDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
		if( pDlg )
		{
			pDlg->SetDisable( TRUE );
		}

		//KES 아래로 (모든경우에 대하여 암호화 해제 필요)
		//NETWORK->ReleaseKey();
	}

	//---KES Distribute Encrypt 071003
	NETWORK->ReleaseKey();
	//--------------------------------

	m_bNoDiconMsg = FALSE;
}

void CMainTitle::ConnectToServer( int index )
{
	if( index > -1 )
	{
		SEVERLIST* pServerList = GAMERESRCMNGR->m_ServerList;
		if( !pServerList[index].bEnter )	return;

		m_pServerListDlg->SetDisable( TRUE );
			
		if( NETWORK->ConnectToServer( pServerList[index].DistributeIP, pServerList[index].DistributePort ) == FALSE )
		{
			OnLoginError( LOGIN_ERROR_NODISTRIBUTESERVER, 0 );
		}

		m_pServerListDlg->SetDisable( FALSE );
		m_pServerListDlg->SetActive( FALSE );
		
		g_nServerSetNum = pServerList[index].ServerNo;
	}
	m_ConnectionServerNo = index;
} 

void CMainTitle::ShowServerList()
{
	m_bServerList = TRUE;

	cDialog* pDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
	if( pDlg )
	{
		pDlg->SetDisable( FALSE );
		pDlg->SetActive( FALSE );
	}
	pDlg = WINDOWMGR->GetWindowForID(MT_DYNAMICDLG);
	if( pDlg )
	{
		pDlg->SetDisable( FALSE );
		pDlg->SetActive( FALSE );
	}
	m_bDynamicDlg = FALSE;

	if( NETWORK->IsConnected() )
		NETWORK->Disconnect();
	m_bInit = FALSE;

	//---KES Distribute Encrypt 071003
	NETWORK->ReleaseKey();
	m_bWaitConnectToAgent = FALSE;
	//--------------------------------
}

void CMainTitle::StartWaitConnectToAgent( BOOL bStart )
{
	m_dwWaitTime			= gCurTime;
	m_bWaitConnectToAgent	= bStart;
}


//trustpak 2005/04/01

#ifndef TAIWAN_LOCAL

void CMainTitle::InitMainTitleMonsters(void)
{	
	srand( (unsigned)time( NULL ) );

	//SetRect(&m_rcMainTitleMonsterArea, 17800, 3000, 20300, 3500);

	SetRect(&m_rcMainTitleMonsterArea, 18000, 2400, 20000, 3600);

	memset(m_apMonster, 0, sizeof(m_apMonster));
	memset(m_av3Target, 0, sizeof(m_av3Target));	
	memset(m_adwLastActionTime, 0, sizeof(m_adwLastActionTime));
	memset(m_aiLastAction, 0, sizeof(m_aiLastAction));


	SEND_MONSTER_TOTALINFO		info;
	VECTOR3						pos;
	BASEMOVE_INFO				bminfo;
	CMonster*					pMonster = NULL;


	DWORD dwID = 10;
	DWORD dwIndex = 0;	

	
	memset(&info,0,sizeof(info));

	info.BaseObjectInfo.dwObjectID = dwID++;
	info.BaseObjectInfo.BattleID = gChannelNum+1;	

	sprintf(info.BaseObjectInfo.ObjectName,"MainTitleMonster01");

	pos.x = 18447.0f;
	pos.y = 0.0f;
	pos.z = 3200.0f;

	m_av3LastPosition[dwIndex] = pos;

	info.MoveInfo.CurPos.Compress(&pos);
	
	bminfo.SetFrom(&info.MoveInfo);	
	bminfo.SetCurTargetPosIdx(0);
	bminfo.SetMaxTargetPosIdx(0);
	bminfo.SetTargetPosition(0, pos);
	info.TotalInfo.MonsterKind = 143;										// 꼬꼬
	
	m_apMonster[dwIndex] = OBJECTMGR->AddMonster(&info.BaseObjectInfo,&bminfo,&info.TotalInfo);	
	m_adwLastActionTime[dwIndex] = gCurTime;

	//trustpak 2005/04/19
	MOVEMGR->InitMove(m_apMonster[dwIndex], &bminfo);
	///

	++dwIndex;

	
	pos.x += 250;	
	pos.z -= 100;

	m_av3LastPosition[dwIndex] = pos;

	memset(&info,0,sizeof(info));

	info.BaseObjectInfo.dwObjectID = dwID++;
	info.BaseObjectInfo.BattleID = gChannelNum+1;

	sprintf(info.BaseObjectInfo.ObjectName,"MainTitleMonster02");	

	info.MoveInfo.CurPos.Compress(&pos);

	bminfo.SetFrom(&info.MoveInfo);
	bminfo.SetCurTargetPosIdx(0);
	bminfo.SetMaxTargetPosIdx(0);
	bminfo.SetTargetPosition(0, pos);
	info.TotalInfo.MonsterKind = 143;

	m_apMonster[dwIndex] = OBJECTMGR->AddMonster(&info.BaseObjectInfo,&bminfo,&info.TotalInfo);
	m_adwLastActionTime[dwIndex] = gCurTime;

	//trustpak 2005/04/19
	MOVEMGR->InitMove(m_apMonster[dwIndex], &bminfo);
	///

	++dwIndex;

	


	pos.x += 350;	

	m_av3LastPosition[dwIndex] = pos;

	memset(&info,0,sizeof(info));

	info.BaseObjectInfo.dwObjectID = dwID++;
	info.BaseObjectInfo.BattleID = gChannelNum+1;

	sprintf(info.BaseObjectInfo.ObjectName,"MainTitleMonster03");	

	info.MoveInfo.CurPos.Compress(&pos);

	bminfo.SetFrom(&info.MoveInfo);
	bminfo.SetCurTargetPosIdx(0);
	bminfo.SetMaxTargetPosIdx(0);
	bminfo.SetTargetPosition(0, pos);
	info.TotalInfo.MonsterKind = 143;											

	m_apMonster[dwIndex] = OBJECTMGR->AddMonster(&info.BaseObjectInfo,&bminfo,&info.TotalInfo);
	m_adwLastActionTime[dwIndex] = gCurTime;

	//trustpak 2005/04/19
	MOVEMGR->InitMove(m_apMonster[dwIndex], &bminfo);
	///
	++dwIndex;	

}


void CMainTitle::RemoveMainTitleMonsters(void)
{
	int i = 0;
	for (i = 0; i < _MAX_MAINTITLE_MONSTER_NUM; i++)
	{
		OBJECTMGR->AddGarbageObject(m_apMonster[i]);
		m_apMonster[i] = NULL;
	}
}

BOOL IsNear(VECTOR3* pv3Pos1, VECTOR3* pv3Pos2, int iMoveMode)
{
	float fCheckRange = 0.0f;


	if(iMoveMode == eMoveMode_Run)
	{
		fCheckRange = 15.0f;
	}
	else 
	{
		fCheckRange = 5.0f;
	}

	if (fCheckRange > fabs(pv3Pos1->x - pv3Pos2->x) &&
		fCheckRange > fabs(pv3Pos1->z - pv3Pos2->z))
	{
		return TRUE;
	}

	return FALSE;
}

//trustpak 2005/04/19
void CMainTitle::ProcessMainTitleMonsters(void)
{	
	const int	c_aiReactionTime[_MAX_MAINTITLE_MONSTER_NUM] = { 900, 1300, 1500 };
	

	VECTOR3		pos = { 0.0f, 0.0f, 0.0f };	
	CMonster*	pMonster = NULL;
	BYTE		byState = 0;

	int i = 0;
	for (i = 0; i < _MAX_MAINTITLE_MONSTER_NUM; i++)	
	{
		pMonster = m_apMonster[i];	
		byState = pMonster->GetState();
		
		if (eObjectState_None == byState)
		{
			// 아직 선택을 다시 할 시간이 안됐으면 보류.
			
			if (m_adwLastActionTime[i] + c_aiReactionTime[i] >= gCurTime)
			{
				continue;
			}			

			// Action choice. 
			// 0 : 뛰기
			// 1, 2 : 걷기			
			// 나머지 : 멈춰있기

			const int c_iMoveModeRun = 0;			

			int iRand = rand();
			int iAction = iRand / ( RAND_MAX / 5 );	

			if (2 < iAction)
			{
				m_adwLastActionTime[i] = gCurTime;
				MOVEMGR->SetMonsterMoveType(pMonster, eMA_STAND);
				continue;
			}

			// 전에 이동을 했으면 한번 멈췄다가 다시 이동하게 한다.

			if (0 != m_aiLastAction[i])
			{
				m_adwLastActionTime[i] = gCurTime;
				MOVEMGR->SetMonsterMoveType(pMonster, eMA_STAND);
				m_aiLastAction[i] = !m_aiLastAction[i];
				continue;				
			}

			// 타겟을 정한다. 이동 범위는 350 , 350 이내로.

			int iMoveX = rand() / (RAND_MAX / 350);
			int iMoveZ = rand() / (RAND_MAX / 350);

			if (60 > iMoveX)
			{
				iMoveX = 60;
			}

			if (40 > iMoveZ)
			{
				iMoveZ = 40;
			}
			
			if (0== rand() / (RAND_MAX / 2) )
			{
				iMoveX = -iMoveX;
			}

			if (0 == rand() / (RAND_MAX /2) )
			{
				iMoveZ = -iMoveZ;
			}									
			
			pMonster->GetPosition(&pos);			


			// 만일 위치가 0, 0, 0 으로 튀어버렸으면 마지막으로 멈추었던 위치로 값을 조정해준다. 

			if (0.0f == pos.x && 0.0f == pos.z)
			{
				pMonster->SetPosition(&m_av3LastPosition[i]);
			}


			// 타겟이 될 곳이 움직일 수 있는 범위 바깥인지 검사.

			if ( (m_rcMainTitleMonsterArea.left	>= (int)pos.x + iMoveX)  )
			{
				iMoveX = 0;
			}

			if ( (m_rcMainTitleMonsterArea.right <= (int)pos.x + iMoveX) )
			{
				iMoveX = 0;
			}

			if ( (m_rcMainTitleMonsterArea.top >= (int)pos.z + iMoveZ) )
			{			

				iMoveZ = 0;
			}

			if ( (m_rcMainTitleMonsterArea.bottom	<= (int)pos.z + iMoveZ) )
			{
				iMoveZ = 0;
			}			


			if (0 == iMoveX && 0 == iMoveZ)
			{
				continue;
			}

			pos.x += (float)iMoveX;		
			pos.z += (float)iMoveZ;
			
			// 타겟 지점을 저장해 둠.
			m_av3Target[i] = pos;

			
			// 행동을 지정.

			//if (c_iMoveModeRun == iAction)
			//{
			//	MOVEMGR->SetMonsterMoveType(pMonster, eMA_RUNAWAY);
			//}
			//else // c_iMoveModeRun
			//{			
				MOVEMGR->SetMonsterMoveType(pMonster, eMA_WALKAROUND);
			//}
						
			MOVEMGR->StartMoveEx(pMonster,gCurTime, &pos);
			
			m_adwLastActionTime[i] = gCurTime;
			m_aiLastAction[i] = !m_aiLastAction[i];
		}
		else		
		{
			VECTOR3 v3Pos;						
			pMonster->GetPosition(&v3Pos);

			if (TRUE == IsNear(&v3Pos, &m_av3Target[i], MOVEMGR->GetMoveMode(pMonster)))
			{
				MOVEMGR->MoveStop(pMonster, &v3Pos);
				MOVEMGR->SetMonsterMoveType(pMonster, eMA_STAND);
				

				// 현재 위치를 타겟으로 갱신해줌. 그렇지 않으면 위치 0, 0, 0 으로 튀어버리는 경우가 있음. 
	
				BASEMOVE_INFO bmif;
				
				pMonster->GetBaseMoveInfo(&bmif);
				bmif.SetTargetPosition(&m_av3Target[i]);
				MOVEMGR->InitMove(pMonster, &bmif);				
				
				
				// 위치가 0, 0, 0 으로 튀어버릴 때 위치를 되돌려주기 위해 현재 위치를 저장해 놓는다. 
				m_av3LastPosition[i] = v3Pos;

					
			}
		}	
	}
	
}



//trustpak 2005/04/19
//original code

/*
void CMainTitle::ProcessMainTitleMonsters(void)
{	
	const int	c_aiReactionTime[_MAX_MAINTITLE_MONSTER_NUM] = { 900, 1300, 1500 };
	

	VECTOR3		pos = { 0.0f, 0.0f, 0.0f };	
	CMonster*	pMonster = NULL;
	BYTE		byState = 0;

	int i = 0;
	for (i = 0; i < _MAX_MAINTITLE_MONSTER_NUM; i++)	
	{
		pMonster = m_apMonster[i];	
		byState = pMonster->GetState();
		
		if (eObjectState_None == byState)
		{
			// 아직 선택을 다시 할 시간이 안됐으면 보류.

			if (m_adwLastActionTime[i] + c_aiReactionTime[i] >= gCurTime)
			{
				continue;
			}

			// Action choice. 
			// 0 : 뛰기
			// 1, 2 : 걷기			
			// 나머지 : 멈춰있기

			const int c_iMoveModeRun = 0;			

			int iRand = rand();
			int iAction = iRand / ( RAND_MAX / 5 );	

			if (2 < iAction)
			{
				m_adwLastActionTime[i] = gCurTime;
				MOVEMGR->SetMonsterMoveType(pMonster, eMA_STAND);
				continue;
			}

			// 전에 이동을 했으면 한번 멈췄다가 다시 이동하게 한다.

			if (0 != m_aiLastAction[i])
			{
				m_adwLastActionTime[i] = gCurTime;
				MOVEMGR->SetMonsterMoveType(pMonster, eMA_STAND);
				m_aiLastAction[i] = !m_aiLastAction[i];
				continue;				
			}

			// 타겟을 정한다. 이동 범위는 350 , 350 이내로.

			int iMoveX = rand() / (RAND_MAX / 350);
			int iMoveZ = rand() / (RAND_MAX / 350);

			if (60 > iMoveX)
			{
				iMoveX = 60;
			}

			if (40 > iMoveZ)
			{
				iMoveZ = 40;
			}
			
			if (0== rand() / (RAND_MAX / 2) )
			{
				iMoveX = -iMoveX;
			}

			if (0 == rand() / (RAND_MAX /2) )
			{
				iMoveZ = -iMoveZ;
			}			
						
			pMonster->GetPosition(&pos);

			// 타겟이 될 곳이 움직일 수 있는 범위 바깥인지 검사.

			if ( (m_rcMainTitleMonsterArea.left	>= (int)pos.x + iMoveX)  )
			{
				//iMoveX = m_rcMainTitleMonsterArea.left - (int)pos.x + iMoveX;

				iMoveX = 0;
			}

			if ( (m_rcMainTitleMonsterArea.right <= (int)pos.x + iMoveX) )
			{
				//iMoveX = (int)pos.x + iMoveX - m_rcMainTitleMonsterArea.right;

				iMoveX = 0;
			}

			if ( (m_rcMainTitleMonsterArea.top >= (int)pos.z + iMoveZ) )
			{
				//iMoveZ = m_rcMainTitleMonsterArea.top - (int)pos.z + iMoveZ;

				iMoveZ = 0;
			}

			if ( (m_rcMainTitleMonsterArea.bottom	<= (int)pos.z + iMoveZ) )
			{
				//iMoveZ = (int)pos.z + iMoveZ - m_rcMainTitleMonsterArea.bottom;

				iMoveZ = 0;
			}	

			// 너무 수치가 작으면 움직임이 어색함으로 보간.
			
			//if (30 > iMoveX)
			//{
			//	iMoveX = 0;
			//}

			//if (30 > iMoveZ)
			//{
			//	iMoveZ = 0;
			//}
			


			if (0 == iMoveX && 0 == iMoveZ)
			{
				continue;
			}

			pos.x += (float)iMoveX;		
			pos.z += (float)iMoveZ;
			
			// 타겟 지점을 저장해 둠.
			m_av3Target[i] = pos;

			
			// 행동을 지정.

			if (c_iMoveModeRun == iAction)
			{
				MOVEMGR->SetMonsterMoveType(pMonster, eMA_RUNAWAY);
			}
			else // c_iMoveModeRun
			{			
				MOVEMGR->SetMonsterMoveType(pMonster, eMA_WALKAROUND);
			}
						
			MOVEMGR->StartMoveEx(pMonster,gCurTime, &pos);		
			
			m_adwLastActionTime[i] = gCurTime;
			m_aiLastAction[i] = !m_aiLastAction[i];
		}
		else		
		{
			VECTOR3 v3Pos;
			pMonster->GetPosition(&v3Pos);

			if (TRUE == IsNear(&v3Pos, &m_av3Target[i], MOVEMGR->GetMoveMode(pMonster)))
			{	
				MOVEMGR->SetMonsterMoveType(pMonster, eMA_STAND);
				MOVEMGR->EndMove(pMonster);					
			}
		}	
	}
	
}
*/

#endif
//=================================================================================================
//	NAME		: Cheat_CheckIp_Nack()
//	PURPOSE		: Process network message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CMainTitle::Cheat_CheckIp_Nack()
{
	if( NETWORK->IsConnected() )
		NETWORK->Disconnect();

	WINDOWMGR->MsgBox( MBI_NOT_GM, MBT_NOBTN, CHATMGR->GetChatMsg( 303 ) );
}


//=================================================================================================
//	NAME		: UserConn_Use_Dynamic_Ack()
//	PURPOSE		: Process network message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CMainTitle::UserConn_Use_Dynamic_Ack( void* pMsg )
{
	StartWaitConnectToAgent( FALSE );
	cDialog* pDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
	if( pDlg )
	{
		pDlg->SetDisable( FALSE );
		pDlg->SetActive( FALSE );
	}
	cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID( MBI_WAIT_LOGINCHECK );
	if( pMsgBox )
	{
		pMsgBox->SetDisable( FALSE );
		pMsgBox->SetActive( FALSE );
	}

	MSG_USE_DYNAMIC_ACK* pmsg = (MSG_USE_DYNAMIC_ACK*)pMsg;
	m_bDynamicDlg = TRUE;
	pDlg = WINDOWMGR->GetWindowForID(MT_DYNAMICDLG);

	if( pDlg )
	{
		pDlg->SetDisable( FALSE );
		pDlg->SetActive( TRUE );

		cStatic* pStc = (cStatic*)pDlg->GetWindowForID( MT_STC_DYNAMICID );
		pStc->SetStaticText( g_szHeroIDName );
		pStc = (cStatic*)pDlg->GetWindowForID( MT_STC_COORD );
		pStc = (cStatic*)pDlg->GetWindowForID( MT_STC_COORD_MAT );
		pStc->SetStaticText( pmsg->mat );						
		((cEditBox*)pDlg->GetWindowForID(MT_DYNAMICPWDEDITBOX))->SetFocusEdit( TRUE );
		((cEditBox*)pDlg->GetWindowForID(MT_DYNAMICPWDEDITBOX))->SetEditText( "" );
	}	
}


//=================================================================================================
//	NAME		: UserConn_Dist_ConnectSuccess()
//	PURPOSE		: Process network message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CMainTitle::UserConn_Dist_ConnectSuccess( void* pMsg )
{
	//---KES Distribute Encryp 071003
	//MSGBASE* pmsg = (MSGBASE*)pMsg;
	MSG_DIST_CONNECTSUCCESS* pmsg = (MSG_DIST_CONNECTSUCCESS*)pMsg;
	m_DistAuthKey = pmsg->dwObjectID;
	NETWORK->SetKey( pmsg->eninit, pmsg->deinit );	//crypt
	//-------------------------------

	SetBlock(FALSE);

	m_bInit = TRUE;
}


//=================================================================================================
//	NAME		: UserConn_Login_Ack()
//	PURPOSE		: Process network message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CMainTitle::UserConn_Login_Ack( void* pMsg )
{
	MSG_LOGIN_ACK* pmsg = (MSG_LOGIN_ACK*)pMsg;

	strcpy(g_AgentAddr,pmsg->agentip);
	g_AgentPort = pmsg->agentport;
	m_UserIdx	= pmsg->userIdx;
	MAINGAME->SetUserLevel( (int)pmsg->cbUserLevel );
#ifdef _GMTOOL_
	if( pmsg->cbUserLevel <= eUSERLEVEL_GM )
		GMTOOLMGR->CanUse( TRUE );
#endif
	gUserID = m_UserIdx;
	USERINFOMGR->SetSaveFolderName( gUserID );

	m_bDisconntinToDist = TRUE;
	m_dwDiconWaitTime	= gCurTime;

	SendOutMsg();
	
#ifdef _NPROTECT_
	NPROTECTMGR->UserIDSend( USERINFOMGR->GetUserID() );
#endif
}


//=================================================================================================
//	NAME		: UserConn_Server_NotReady()
//	PURPOSE		: Process network message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CMainTitle::UserConn_Server_NotReady()
{
	NoDisconMsg();
	OnLoginError( LOGIN_ERROR_NODISTRIBUTESERVER, 0 );
}


//=================================================================================================
//	NAME		: UserConn_Login_Nack()
//	PURPOSE		: Process network message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CMainTitle::UserConn_Login_Nack( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	OnLoginError(pmsg->dwData1,pmsg->dwData2);
}


//=================================================================================================
//	NAME		: UserConn_CharacterList_Ack()
//	PURPOSE		: Process network message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CMainTitle::UserConn_CharacterList_Ack( void* pMsg )
{
	SEND_CHARSELECT_INFO* pmsg = (SEND_CHARSELECT_INFO*)pMsg;
#ifdef _CRYPTCHECK_ 
	NETWORK->SetKey( pmsg->eninit, pmsg->deinit );
#endif
	if( NETWORK->IsConnected() )
		MAINGAME->SetGameState(eGAMESTATE_CHARSELECT, (void *)pmsg, sizeof(SEND_CHARSELECT_INFO));
}


//=================================================================================================
//	NAME		: UserConn_CharacterList_Nack()
//	PURPOSE		: Process network message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CMainTitle::UserConn_CharacterList_Nack()
{
	// 케릭정보를 받는데 실패했습니다.
	cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(5));
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
	if( pIDDlg && pMsgBox )
	{
		pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
	}
}


//=================================================================================================
//	NAME		: UserConn_RemainTime_Notify()
//	PURPOSE		: Process network message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CMainTitle::UserConn_RemainTime_Notify( void* pMsg )
{
#ifdef TAIWAN_LOCAL	
	MSG_INT* pmsg = (MSG_INT*)pMsg;
	BILLINGMGR->SetUseRemainTime(TRUE);
	BILLINGMGR->SetRemainTime( pmsg->nData );
#endif //TAIWAN_LOCAL	
}


//=================================================================================================
//	NAME		: UserConn_Agent_ConnectSuccess()
//	PURPOSE		: Process network message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CMainTitle::UserConn_Agent_ConnectSuccess( void* pMsg )
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
					
	MSG_DWORD2 msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHARACTERLIST_SYN;
	msg.dwData1 = m_UserIdx;
	msg.dwData2 = m_DistAuthKey;
	NETWORK->Send(&msg,sizeof(msg));
}

//=================================================================================================
//	NAME		: Cheat_EventNotify_On()
//	PURPOSE		: Process network message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CMainTitle::Cheat_EventNotify_On( void* pMsg )
{
	MSG_EVENTNOTIFY_ON* pmsg = (MSG_EVENTNOTIFY_ON*)pMsg;

	NOTIFYMGR->SetEventNotifyStr( pmsg->strTitle, pmsg->strContext );
	NOTIFYMGR->SetEventNotify( TRUE );

	NOTIFYMGR->SetEventNotifyChanged( TRUE );

	NOTIFYMGR->ResetEventApply();
	for(int i=0; i<eEvent_Max; ++i)
	{
		if( pmsg->EventList[i] )
			NOTIFYMGR->SetEventApply( i );
	}
}


//=================================================================================================
//	NAME		: Cheat_EventNotify_Off()
//	PURPOSE		: Process network message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CMainTitle::Cheat_EventNotify_Off( void* pMsg )
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	NOTIFYMGR->SetEventNotify( FALSE );
	NOTIFYMGR->SetEventNotifyChanged( FALSE );
}


//=================================================================================================
//	NAME		: Login_Error_Invalid_Version()
//	PURPOSE		: Process error message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CMainTitle::Login_Error_Invalid_Version()
{
	cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(9) );
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
	if( pIDDlg && pMsgBox )
	{
		pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
	}

	SendOutMsg();
	NoDisconMsg();
}


//=================================================================================================
//	NAME		: Login_Error_OverLappedLogin()
//	PURPOSE		: Process error message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CMainTitle::Login_Error_OverLappedLogin()
{
	cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_OVERLAPPEDLOGIN, MBT_YESNO, CHATMGR->GetChatMsg(8) );
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
	if( pIDDlg && pMsgBox )
	{
		pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
	}
}


//=================================================================================================
//	NAME		: Login_Error_OverLappedLoginiNotherServer()
//	PURPOSE		: Process error message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CMainTitle::Login_Error_OverLappedLoginiNotherServer( DWORD dwParam )
{
	char serverName[128] = {0,};
	for(int n=0;n<30;++n)
	{
		if(GAMERESRCMNGR->m_ServerList[n].ServerNo == dwParam)
		{
			strcpy(serverName,GAMERESRCMNGR->m_ServerList[n].ServerName);
			break;
		}
	}
	//SEVERLIST* pServerList = GAMERESRCMNGR->m_ServerList;
	cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_OVERLAPPEDLOGININOTHERSERVER, MBT_OK, CHATMGR->GetChatMsg(1),
										serverName, serverName);
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
	if( pIDDlg && pMsgBox )
	{
		pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
	}
	NoDisconMsg();
}


//=================================================================================================
//	NAME		: Login_Error_NoAgentServer()
//	PURPOSE		: Process error message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CMainTitle::Login_Error_NoAgentServer()
{
	cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORRETURN, MBT_OK, CHATMGR->GetChatMsg(7) );
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
	if( pIDDlg && pMsgBox )
	{
		pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
	}
	
	SendOutMsg();
	NoDisconMsg();
}


//=================================================================================================
//	NAME		: Login_Error_NoDistributeServer()
//	PURPOSE		: Process error message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CMainTitle::Login_Error_NoDistributeServer()
{
	cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORRETURN, MBT_OK, CHATMGR->GetChatMsg(7) );
}


//=================================================================================================
//	NAME		: Login_Error_InvalidUserLevel()
//	PURPOSE		: Process error message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CMainTitle::Login_Error_InvalidUserLevel()
{
	cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORRETURN, MBT_OK, CHATMGR->GetChatMsg(6));
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
	if( pIDDlg && pMsgBox )
	{
		pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
	}
	
	SendOutMsg();
	NoDisconMsg();
}


//=================================================================================================
//	NAME		: Login_Error_DistServerIsBusy()
//	PURPOSE		: Process error message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CMainTitle::Login_Error_DistServerIsBusy()
{
	cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORRETURN, MBT_OK, CHATMGR->GetChatMsg(304));
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
	if( pIDDlg && pMsgBox )
	{
		pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
	}
	
	SendOutMsg();
	NoDisconMsg();
}


//=================================================================================================
//	NAME		: Login_Error_WrongIdPw()
//	PURPOSE		: Process error message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CMainTitle::Login_Error_WrongIdPw()
{
	cEditBox* pEdit = (cEditBox*)WINDOWMGR->GetWindowForIDEx( MT_PWDEDITBOX );
	pEdit->SetFocusEdit( FALSE );
	cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_PASSERROR, MBT_OK, CHATMGR->GetChatMsg(52) );
	//id/pw창 리드온리 혹은 디스에이블로
	//메시지 박스 클릭시에 id/pw비워주고 포커스 id에 주기
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
	if( pIDDlg && pMsgBox )
	{
		pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
	}
}


//=================================================================================================
//	NAME		: Login_Error_BlockUserLevel()
//	PURPOSE		: Process error message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CMainTitle::Login_Error_BlockUserLevel( DWORD dwRemainTime )
{
	char buf[256] = {0,};
	if( dwRemainTime > 0 )
	{
		int hour = dwRemainTime / 60;
		int min = dwRemainTime - hour * 60;

		//wsprintf( buf, "입장할 수 없는 계정입니다. %d시간 %d분 이후에 입장할 수 있습니다.", hour, min );
		wsprintf( buf, CHATMGR->GetChatMsg( 1462 ), hour, min ) ;
	}
	else
	{
		wsprintf( buf, CHATMGR->GetChatMsg( 305 ) );
	}

//	cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg( 305 ) );
	cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, buf );
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
	if( pIDDlg && pMsgBox )
	{
		pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
	}

	SendOutMsg();
	NoDisconMsg();
}


//=================================================================================================
//	NAME		: Login_Error_Invalid_Ip()
//	PURPOSE		: Process error message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CMainTitle::Login_Error_Invalid_Ip()
{
	cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_PASSERROR, MBT_OK, CHATMGR->GetChatMsg( 306 ) );
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
	if( pIDDlg && pMsgBox )
	{
		pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
	}
}


//=================================================================================================
//	NAME		: Login_Error_DistConnet_Error()
//	PURPOSE		: Process error message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CMainTitle::Login_Error_DistConnet_Error()
{
	cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_DISTCONNECT_ERROR, MBT_OK, CHATMGR->GetChatMsg(216) );
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
	if( pIDDlg && pMsgBox )
	{
		pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
	}
}


//=================================================================================================
//	NAME		: Login_Error_MaxUser()
//	PURPOSE		: Process error message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CMainTitle::Login_Error_MaxUser()
{
	cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORRETURN, MBT_OK, CHATMGR->GetChatMsg(307) );
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
	if( pIDDlg && pMsgBox )
	{
		pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
	}
}


//=================================================================================================
//	NAME		: Login_Error_Minor_InadultServer()
//	PURPOSE		: Process error message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CMainTitle::Login_Error_Minor_InadultServer()
{
	cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORRETURN, MBT_OK, CHATMGR->GetChatMsg(308) );
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
	if( pIDDlg && pMsgBox )
	{
		pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
	}
}


//=================================================================================================
//	NAME		: Login_Error_SecededAccount()
//	PURPOSE		: Process error message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CMainTitle::Login_Error_SecededAccount()
{
	cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(309) );
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
	if( pIDDlg && pMsgBox )
	{
		pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
	}
	
	SendOutMsg();
	NoDisconMsg();
}


//=================================================================================================
//	NAME		: Login_Error_Not_CloseBetaWinner()
//	PURPOSE		: Process error message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CMainTitle::Login_Error_Not_CloseBetaWinner()
{
	cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(310) );
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
	if( pIDDlg && pMsgBox )
	{
		pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
	}

	SendOutMsg();
	NoDisconMsg();
}


//=================================================================================================
//	NAME		: Login_Error_NoreMainTime()
//	PURPOSE		: Process error message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CMainTitle::Login_Error_NoreMainTime()
{
	cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, "You have not remain time, please recharge ASAP" );

	cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
	if( pIDDlg && pMsgBox )
	{
		pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
	}

	SendOutMsg();
	NoDisconMsg();
}


//=================================================================================================
//	NAME		: Login_Error_NoIpRegen()
//	PURPOSE		: Process error message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CMainTitle::Login_Error_NoIpRegen()
{
	cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(311) );
	cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
	if( pIDDlg && pMsgBox )
	{
		pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
	}

	SendOutMsg();
	NoDisconMsg();
}

// desc_hseos_성별선택01
// S 성별선택 추가 added by hseos 2007.06.16
void CMainTitle::UserConn_Character_SexKind(void *pMsg)
{
	MSG_DWORD* pPacket = (MSG_DWORD*)pMsg;

	CHARMAKEMGR->SetCharSexKindFromDB(pPacket->dwData);
}
// E 성별선택 추가 added by hseos 2007.06.16






//=================================================================================================
//	NAME		: Login_Error_Invalid_Sec_Pass()
//	PURPOSE		: Process error message.
//	DATE		: January 11 2008 LYW
//	ATTENTION	: 
//=================================================================================================
void CMainTitle::Login_Error_Invalid_Sec_Pass()
{
	cMsgBox* pMsgBox = NULL ;
	pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(1265) ) ;

	if( !pMsgBox ) return ;

	cDialog* pIDDlg = NULL ;
	pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG ) ;

	if( !pIDDlg ) return ;

	pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() ) ;

	SendOutMsg() ;
	NoDisconMsg() ;
}





//=================================================================================================
//	NAME		: Login_Error_Change_Sec_Pass()
//	PURPOSE		: Process error message.
//	DATE		: January 11 2008 LYW
//	ATTENTION	: 
//=================================================================================================
void CMainTitle::Login_Error_Change_Sec_Pass()
{	
	cMsgBox* pMsgBox = NULL ;
	pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(1266) ) ;

	if( !pMsgBox ) return ;

	cDialog* pIDDlg = NULL ;
	pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG ) ;

	if( !pIDDlg ) return ;

	pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() ) ;

	SendOutMsg() ;
	NoDisconMsg() ;
}

void CMainTitle::Login_Error_File_CRC()
{
//#ifndef _GMTOOL_
	 // 090120 LUJ, 패치 버전을 최초로 돌린다
	 {
		 const char* const	versionFile = "LunaVerInfo.ver";
		 FILE* const			file		= fopen( versionFile, "r+" );

		 if( file )
		 {
			 // 090109 LUJ, 4글자로 구성된 버전 헤더를 파일에서 읽어와 문자열 변수에 담는다
			 const size_t headerSize = 4;
			 char header[ headerSize + 1 ] = { 0 };
			 fread(
				 header,
				 headerSize,
				 sizeof( *header ),
				 file );

			 // 090109 LUJ, 전체 패치를 수행할 버전 번호를 만든다

			 char versionText[ MAX_PATH ] = { 0 };
			 sprintf(
				 versionText,
				 "%s00000000",
				 header );
			 // 090109 LUJ, 패치 버전 문자열을 파일에 쓴다
			 fseek(
				 file,
				 0,
				 SEEK_SET );
			 fwrite(
				 versionText,
				 sizeof( *versionText ),
				 strlen( versionText ),
				 file );
			 fclose( file );
		 }
	 }

	 cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg( 9 ) );
	 cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
	 if( pIDDlg && pMsgBox )
	 {
		 pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
	 }

	 SendOutMsg();
	 NoDisconMsg();
//#endif
}

void CMainTitle::Login_Error_LoginSession_Invalid()
{
	cMsgBox* pMsgBox = NULL ;
	pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg( 2221 ) ); // "로그인 정보에 오류가 있습니다. 다시 로그인해주십시오"
	if( !pMsgBox ) return ;

	cDialog* pIDDlg = NULL ;
	pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG ) ;
	if( !pIDDlg ) return ;

	pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() ) ;

	SendOutMsg() ;
	NoDisconMsg() ;
}

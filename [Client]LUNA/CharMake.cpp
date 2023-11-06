#include "stdafx.h"
#include "CharMake.h"

#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cEditBox.h"
#include "./Interface/cResourceManager.h"
#include "./Interface/cPushupButton.h"

#include "ObjectManager.h"
#include "GameResourceManager.h"
#include "MHCamera.h"
#include "MHTimeManager.h"
#include "MAINGAME.h"

#include "cWindowSystemFunc.h"
#include "ChatManager.h"
#include "cMsgBox.h"
#include "HelperManager.h"

// 061217 LYW --- Delete this code.
//#include "CharMakeDialog.h"
#include "CharMakeNewDlg.h"
#include "CharMakeManager.h"

#include "mhMap.h"
#include "./Input/UserInput.h"

// 061218 LYW --- Add for test.
#include "./interface/cFont.h"

//#include "Gamein.h"
#include "GMNotifyManager.h"
//#include "EventNotifyDialog.h"

// 070523 LYW --- CharMake : Include appearance manager.
#include "AppearanceManager.h"
#include "cStatic.h"

// 061214 LYW --- Change character position.
//VECTOR3				g_v3CreatePos = { 3600.0f, 0.0f, 1850.0f };
//VECTOR3				g_v3CreatePos = { 20140.0f, 1700.0f, 39350.0f };

// 061218 LYW --- Add for test.
/*
float fCharAngle ;
*/

GLOBALTON(CCharMake)
CCharMake::CCharMake()
{
	// 062117 LYW --- Delete this code.
	//m_pCharMakeDlg = NULL;
	m_pCharMakeNewDlg = NULL ;
	m_pCharMakeCamera = NULL;

	// 061217 LYW --- Add for test.
	m_v3Eye.x	 = 20600.0f ;
	m_v3Eye.y	 =  1900.0f ;
	m_v3Eye.z	 = 39950.0f ;
	m_v3Rotate.x = -0.2f ;
	m_v3Rotate.y = -2.0f ;
	m_v3Rotate.z =  0.0f ; 
	m_vCharPos.x = 20140.0f ;
	m_vCharPos.y =  1700.0f ;
	m_vCharPos.z = 39350.0f ;
	m_fCharAngle = 1.7f ;
	// end test.

	// 061218 LYW --- Add vector for camera.
	/*
	m_v3Eye_Elf.x		= 20200.0f ;
	m_v3Eye_Elf.y		=  1250.0f ;
	m_v3Eye_Elf.z		= 34500.0f ;
	m_v3Rotate_Elf.x	= 0.0f ;
	m_v3Rotate_Elf.y	= 0.3f ;
	m_v3Rotate_Elf.z	= 0.0f ;
	m_vElfPos.x			= 20365.0f ;
	m_vElfPos.y			=  1165.0f ;
	m_vElfPos.z			= 35060.0f ;


	m_v3Eye_Human.x		= 20600.0f ;
	m_v3Eye_Human.y		=  1900.0f ;
	m_v3Eye_Human.z		= 39950.0f ;
	m_v3Rotate_Human.x  = -0.2f ;
	m_v3Rotate_Human.y  = -2.0f ;
	m_v3Rotate_Human.z  =  0.0f ; 
	m_vHumanPos.x		= 20210.0f ;
	m_vHumanPos.y		=  1740.0f ;
	m_vHumanPos.z		= 39760.0f ;

	m_fElfAngle			= -0.3f ;
	m_fHumanAngle		= 2.0f ;
	*/

	m_v3Eye_Elf.x		= 9009.0f ;
	m_v3Eye_Elf.y		= 5055.0f ;
	m_v3Eye_Elf.z		= 6567.0f ;
	m_v3Rotate_Elf.x	= -0.08f ;
	m_v3Rotate_Elf.y	= -1.54f ;
	m_v3Rotate_Elf.z	= 0.0f ;
	m_vElfPos.x			= 8765.0f ;
	m_vElfPos.y			= 5000.0f ;
	m_vElfPos.z			= 6570.0f ;


	m_v3Eye_Human.x		= 9009.0f ;
	m_v3Eye_Human.y		= 5055.0f ;
	m_v3Eye_Human.z		= 6567.0f ;
	m_v3Rotate_Human.x  = -0.08f ;
	m_v3Rotate_Human.y  = -1.54f ;
	m_v3Rotate_Human.z  = 0.0f ;
	m_vHumanPos.x		= 8765.0f ;
	m_vHumanPos.y		= 5000.0f ;
	m_vHumanPos.z		= 6570.0f ;

	m_fElfAngle			= 1.7f ;
	m_fHumanAngle		= 1.7f ;

	// 061218 LYW --- Add variable have order.
	m_byOrder = e_SCENE_IN ;
	// 070522 LYW --- Delete change scene.
	//m_byChangeSceneMode = 0xFF ;
	m_dwAlphaCount = 0 ;

	//m_bEnterGame = FALSE;
}

CCharMake::~CCharMake()
{
	// 061218 LYW --- Add alpha screen.
	if( m_pAlphaScreen )
	{
		delete m_pAlphaScreen ;
		m_pAlphaScreen = NULL ;
	}
}

BOOL CCharMake::Init(void* pInitParam)
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

	if(MAP->IsInited() == FALSE)
	{
		// 061214 LYW --- Change map.
		//MAP->InitMap(103);
		// 061225 LYW --- Change title map.
		//MAP->InitMap(25);
		MAP->InitMap(200) ;
	}
	
	NETWORK->SetCurState(this);

	// 다이얼로그 생성
	// 061215 LYW --- Delete this code for a moment.
	//CreateCharMakeDlg();

	// 061214 LYW --- CharMake : Add new function to create dialog for character property.
	CreateCharMakeNewDlg() ;
	

	CHARMAKEMGR->Init();
	// 061218 LYW ---- Change position of character.
	//CHARMAKEMGR->CreateNewCharacter(&g_v3CreatePos);	

	// 061218 LYW --- Setting creation character part.	
	//g_pExecutive->GetGeometry()->SetCameraPos(&m_v3Eye_Human, 0);
	//g_pExecutive->GetGeometry()->SetCameraAngleRad(&m_v3Rotate_Human, 0);

	// 061215 LYW --- Delete this code for a moment.
//	m_pCharMakeDlg->ChangeComboStatus( 0 ); // 콤보 셋팅을 기본 남자로 셋팅한다.

	// 도우미 초기화
//	HELPERMGR->Init();
//	HELPERMGR->ShowRespect( 1, FALSE );

	WINDOWMGR->AfterInit();
	
//카메라 세팅
/*	vPivot.x = 23058 - 600; // + 뒤로  - 앞으로
	vPivot.y = 530;
	vPivot.z = 31780 + 420; // + 오른쪽으로  - 왼쪽으로
	CAMERA->InitCamera(0, -4.0f, 90.0f, 4215, &vPivot);// 거리 조정 
*/	

/*	이동안시킬땐 이걸루..
	VECTOR3 vPivot;
	vPivot.x = 24480;
	vPivot.y = 6020;
	vPivot.z = 9884;

	CAMERA->InitCamera(0, -6.0f, 0.0f, 2000, &vPivot);// 거리 조정 
//*/

	//VECTOR3 v3Eye		= { 3427.488525f, 410.0f, 1218.216919f };	
	// 061214 LYW --- Change camera position.
	/*
	VECTOR3 v3Eye		= { 3427.488525f + 71.680961f, 410.0f -118.823507f, 1218.216919f + 237.419061f};	
	VECTOR3 v3Rotate	= { -0.446804f, 0.293215f, 0.0f };
	*/
	/*
	VECTOR3 v3Eye		= { 28000.0f, 3000.0f, 15500.0f };	
	VECTOR3 v3Rotate	= { 0.05f, 0.52f, 0.0f };
	*/
	/*
	VECTOR3 v3Eye		= { 20200.0f, 1250.0f, 34500.0f };	
	VECTOR3 v3Rotate	= { 0.1f, 0.3f, 0.0f };
	*/
	memcpy( &m_v3Eye, &m_v3Eye_Human, sizeof(VECTOR3) ) ;
	memcpy( &m_v3Rotate, &m_v3Rotate_Human, sizeof(VECTOR3) ) ;
	memcpy( &m_vCharPos, &m_vHumanPos, sizeof(VECTOR3 ) ) ;
	m_fCharAngle = m_fHumanAngle ;
	
	CAMERA->SetCurCamera(0);
	CAMERA->SetCameraMode(eCM_LOGIN);	
	// 061226 LYW --- Setting fov to camera.
	CAMERA->SetCameraFov(45.0f) ;
	
	// 061218 LYW --- Delete this code for a moment.
	/*
	if (NULL == m_pCharMakeCamera)
	{
		m_pCharMakeCamera = new CCharMakeCamera;
		m_pCharMakeCamera->InitCamera(g_pExecutive, &v3Eye, &v3Rotate, 0);
	}
	*/

	// 061218 LYW --- Add for test.
	if (NULL == m_pCharMakeCamera)
	{
		m_pCharMakeCamera = new CCharMakeCamera;
		m_pCharMakeCamera->InitCamera(g_pExecutive, &m_v3Eye, &m_v3Rotate, 0);
	}
	// end test.

	// 070208 LYW --- CharMake : Add function to setting position and scale of image.
	SetPositionScale() ;
/*
	ZeroMemory( &mRotation, sizeof( mRotation ) );

	{
		CMouse* mouse = g_UserInput.GetMouse();
		ASSERT( mouse );

		mPosition.mX		= mouse->GetMouseX();
		mPosition.mY		= mouse->GetMouseY();
		mPosition.mIsDrag	= FALSE;
	}
*/
	//m_bEnterGame = FALSE;

	return TRUE;
}


// 070208 LYW --- CharMake : Add function to setting position and scale of image.
void CCharMake::SetPositionScale()
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

void CCharMake::Release(CGameState* pNextGameState)
{
	m_imageLoginBarTop.Release();
	m_imageLoginBarBottom.Release();	

	if (NULL != m_pCharMakeCamera)
	{
		delete m_pCharMakeCamera;
		m_pCharMakeCamera = NULL;
	}

	OBJECTMGR->RemoveAllObject();
	
	WINDOWMGR->DestroyWindowAll();
	RESRCMGR->ReleaseResource(12);
	CHARMAKEMGR->Release();
//	HELPERMGR->Release();
		
	if(pNextGameState == NULL || m_bEnterGame )
		MAP->Release();
			
}

void CCharMake::Process()
{	
	if(IsBlock()) return;

	if(	!CHARMAKEMGR->GetNewPlayer() ) // 이자리에 꼬옥 놓아 두세요... LBS
	{
		// 061218 LYW --- Change vector for character position.
		//CHARMAKEMGR->CreateNewCharacter(&g_v3CreatePos);	
		CHARMAKEMGR->CreateNewCharacter(&m_vCharPos);	
	}
	
	// 061226 LYW --- Delete this code.
	//CHARMAKEMGR->ApplyScale();

	// 061218 LYW --- Add function to change scene.
	// 070522 LYW --- Delete change scene.
	/*
	if( m_bChangeScene )
	{
		ChangeScene( m_byChangeSceneMode ) ;
	}
	*/

///	MHTIMEMGR->Process();
		
	/*
	if(CAMERA->GetGotoPosition() != -1)
		CAMERA->Process();
		*/	

	// 061218 LYW --- Delete this code for a moment.
	/*
	static int s_iLastY = MOUSE->GetMouseY();
	static int s_iLastX = MOUSE->GetMouseX();	
	
	if( MOUSE->RButtonDrag() )
	{
		//CHARMAKEMGR->NewCharacterRotate( MOUSE->GetMouseAxisX() );		

		int iCurrentY		= MOUSE->GetMouseY();
		int iCurrentX		= MOUSE->GetMouseX();

		if (abs(iCurrentY - s_iLastY) > abs(iCurrentX - s_iLastX) )
		{
			GXOBJECT_HANDLE gxh = CHARMAKEMGR->GetNewPlayer()->GetEngineObject()->GetGXOHandle();		
			m_pCharMakeCamera->RotateCamera(gxh, 0, s_iLastY, 0, iCurrentY);		
		}
		else
		{
			CHARMAKEMGR->NewCharacterRotate( MOUSE->GetMouseAxisX() );
		}	

		s_iLastY = iCurrentY;
		s_iLastX = iCurrentX;		
	}
	
	int iWheel	= MOUSE->GetWheel();
	if (0 < iWheel)
	{
		m_pCharMakeCamera->ZoomInCamera();			
	}
	else if (0 > iWheel)
	{		
		m_pCharMakeCamera->ZoomOutCamera();		
	}
	*/
	// 061217 LYW --- Add for test.
	/*
	MHCAMERADESC* pCurCamera = CAMERA->GetCameraDesc() ;
	if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_W ) )
	{
		m_v3Eye.z += 1.0f ;
	}
	// 뒤
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_S ) )
	{
		m_v3Eye.z -= 1.0f ;
	}
	// 상
	if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_Q ) )
	{
		m_v3Eye.y += 1.0f ;
	}
	// 하
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_E ) )
	{
		m_v3Eye.y -= 1.0f ;
	}
	// 좌
	if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_A ) )
	{
		m_v3Eye.x -= 1.0f ;
	}
	// 우
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_D ) )
	{
		m_v3Eye.x += 1.0f ;
	}

	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_F ) )
	{
		m_v3Eye.x = 20200.0f ;
		m_v3Eye.y = 1250.0f ;
		m_v3Eye.z = 34500.0f ;
		m_v3Rotate.x = 0.1f ;
		m_v3Rotate.y = 0.3f ;
		m_v3Rotate.z = 0.0f ;
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_V ) )
	{
		m_v3Eye.x = 20600.0f ;
		m_v3Eye.y = 1900.0f ;
		m_v3Eye.z = 39950.0f ;
		m_v3Rotate.x =  -0.2f ;
		m_v3Rotate.y = -2.0f ;
		m_v3Rotate.z =  0.0f ;
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_B ) )
	{
		m_vCharPos.x	= 20210.0f ; 
		m_vCharPos.y = 1740.0f ;
		m_vCharPos.z = 39760.0f ;
		m_fCharAngle  = 2.6f ;
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_T ) )
	{
		if( m_fCharAngle > 0 )
		{
			m_fCharAngle-- ;
		}
		else
		{
			m_fCharAngle = 3 ;
		}
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_U ) )
	{
		if( m_fCharAngle < 3 )
		{
			m_fCharAngle++ ;
		}
		else
		{
			m_fCharAngle = 0 ;
		}
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_G ) )
	{
		m_fCharAngle -= 0.1f ;
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_H ) )
	{
		m_fCharAngle += 0.1f ;
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_K ) )
	{
		m_v3Rotate.x -= 0.01f ;
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_L ) )
	{
		m_v3Rotate.x += 0.01f ;
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_O ) )
	{
		m_v3Rotate.y -= 0.01f ;
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_P ) )
	{
		m_v3Rotate.y += 0.01f ;
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_LEFT ) )
	{
		m_vCharPos.x -= 5.0f ;
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_RIGHT ) )
	{
		m_vCharPos.x += 5.0f ;
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_UP ) )
	{
		m_vCharPos.z -= 5.0f ;
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_DOWN ) )
	{
		m_vCharPos.z += 5.0f ;
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_Z ) )
	{
		m_vCharPos.y -= 5.0f ;
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_X ) )
	{
		m_vCharPos.y += 5.0f ;
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_1 ) )
	{
		m_fCharAngle -= 0.1f ;
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_2 ) )
	{
		m_fCharAngle += 0.1f ;
	}
	//m_fAngle

	g_pExecutive->GetGeometry()->SetCameraPos(&m_v3Eye, 0);
	g_pExecutive->GetGeometry()->SetCameraAngleRad(&m_v3Rotate, 0);
	// 061225 LYW --- Add for test.				
	VECTOR3 scale ;
	scale.x = 0.3f ;
	scale.y = 0.3f ;
	scale.z = 0.3f ;
	CHARMAKEMGR->GetNewPlayer()->GetEngineObject()->SetScale(&scale) ;

	CHARMAKEMGR->GetNewPlayer()->SetAngle( m_fCharAngle ) ;
	CHARMAKEMGR->GetNewPlayer()->SetPosition(&m_vCharPos ) ;

	// test end.
	*/

	ProcessDragRotation();
}

void CCharMake::BeforeRender()
{
	g_pExecutive->GetRenderer()->BeginRender(0,0,0);
	g_pExecutive->GetRenderer()->EndRender();
	MAP->Process( gCurTime );
}

void CCharMake::AfterRender()
{	
	m_imageLoginBarTop.RenderSprite(&m_scale,0,0,&m_v2TopPos,0xffffffff);
	m_imageLoginBarBottom.RenderSprite(&m_scale,0,0,&m_v2BottomPos,0xffffffff);	

	WINDOWMGR->Render();

	// 061217 LYW --- Add for test.
	/*
	char tempStr[ 128 ] ;
	RECT rc ;
	rc.left = 10 ;
	rc.top = 500 ;
	rc.right = 1000 ;
	rc.bottom = 700 ;

	sprintf( tempStr, "xpos = %f", m_v3Eye.x ) ;
	CFONT_OBJ->RenderFont(0, tempStr, strlen( tempStr ), &rc, RGB( 255, 0, 0 ) ) ;
	sprintf( tempStr, "ypos = %f", m_v3Eye.y ) ;
	rc.top += 20 ;
	CFONT_OBJ->RenderFont(0, tempStr, strlen( tempStr ), &rc, RGB( 255, 0, 0 ) ) ;
	sprintf( tempStr, "zpos = %f", m_v3Eye.z ) ;
	rc.top += 20 ;
	CFONT_OBJ->RenderFont(0, tempStr, strlen( tempStr ), &rc, RGB( 255, 0, 0 ) ) ;
	// end test.

	//  Angle.
	sprintf( tempStr, "Rotate X = %f", m_v3Rotate.x ) ;
	rc.top += 20 ;
	CFONT_OBJ->RenderFont(0, tempStr, strlen( tempStr ), &rc, RGB( 0, 255, 0 ) ) ;

	sprintf( tempStr, "Rotate Y = %f", m_v3Rotate.y ) ;
	rc.top += 20 ;
	CFONT_OBJ->RenderFont(0, tempStr, strlen( tempStr ), &rc, RGB( 0, 255, 0 ) ) ;

	sprintf( tempStr, "Rotate Z = %f", m_v3Rotate.z ) ;
	rc.top += 20 ;
	CFONT_OBJ->RenderFont(0, tempStr, strlen( tempStr ), &rc, RGB( 0, 255, 0 ) ) ;


	sprintf( tempStr, "CharPos X = %f", m_vCharPos.x ) ;
	rc.top += 20 ;
	CFONT_OBJ->RenderFont(0, tempStr, strlen( tempStr ), &rc, RGB( 0, 0, 255 ) ) ;

	sprintf( tempStr, "CharPos Y = %f", m_vCharPos.y ) ;
	rc.top += 20 ;
	CFONT_OBJ->RenderFont(0, tempStr, strlen( tempStr ), &rc, RGB( 0, 0, 255 ) ) ;

	sprintf( tempStr, "CharPos Z = %f", m_vCharPos.z ) ;
	rc.top += 20 ;
	CFONT_OBJ->RenderFont(0, tempStr, strlen( tempStr ), &rc, RGB( 0, 0, 255 ) ) ;

	sprintf( tempStr, "CharAngle = %f", m_fCharAngle ) ;
	rc.top += 20 ;
	CFONT_OBJ->RenderFont(0, tempStr, strlen( tempStr ), &rc, RGB( 0, 0, 255 ) ) ;
	*/
}

void CCharMake::NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg)
{
	switch(Category) 
	{
	case MP_USERCONN:
		{
			switch(Protocol) 
			{
			case MP_USERCONN_CHARACTER_NAMECHECK_NACK:
				{
					UserConn_Character_NameCheck_Nack( pMsg ) ;
				}
				return;
			case MP_USERCONN_CHARACTER_NAMECHECK_ACK:
				{
					UserConn_Character_NameCheck_Ack() ;
				}
				return;
			case MP_USERCONN_CHARACTER_MAKE_ACK:
				{
					UserConn_Character_Make_Ack( pMsg ) ;
				}
				return;
			case MP_USERCONN_CHARACTER_MAKE_NACK:
				{
					UserConn_Character_Make_Nack() ;
				}
				return;
			case MP_USERCONN_CHARACTERLIST_ACK:
				{
					UserConn_CharacterList_Ack( pMsg ) ;
				}
				return;
			// desc_hseos_성별선택01
			// S 성별선택 추가 added by hseos 2007.06.15
			case MP_USERCONN_USER_SEXKIND:
				{
					int a =0;
				}
				return;
			// E 성별선택 추가 added by hseos 2007.06.15
			}
		}
		return;
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


void CCharMake::DisplayNotice( int MsgNum )
{
	WINDOWMGR->MsgBox( MBI_CHARMAKEERROR, MBT_OK, CHATMGR->GetChatMsg(MsgNum) );
	SetDisableDlg( TRUE );
}

void CCharMake::SetDisableDlg( BOOL val )
{
	// 061217 LYW --- Delete this code.
	/*
	if( m_pCharMakeDlg )
		m_pCharMakeDlg->SetDisable( val );
		*/
	if( m_pCharMakeNewDlg )
		m_pCharMakeNewDlg->SetDisable( val ) ;
}


//////////////////////////////////////////////////////////////////////////


/*
	//CPlayer* CObjectManager::AddPlayer(BASEOBJECT_INFO* pBaseObjectInfo,BASEMOVE_INFO* pMoveInfo,CHARACTER_TOTALINFO* pTotalInfo)

struct BASEOBJECT_INFO
{
	DWORD dwObjectID;
	char ObjectName[MAX_NAME_LENGTH+1];
	BYTE ObjectState;
};

struct BASEMOVE_INFO
{
	VECTOR3 CurPosition;
	VECTOR3 TargetPosition;

	bool bMoving;
	bool MoveMode;		 // 0:뛰기 1:걷기

	WORD KyungGongIdx;	// 경공Idx 0: 경공중이 아니다  !0: 사용중인 경공번호
};
struct CHARACTER_TOTALINFO
{
	WORD	Life;							// 생명력
	WORD	MaxLife;						// 최대 생명력
	
	char	NickName[MAX_NAME_LENGTH+1];	// 별칭(호)
	char	HonorName[MAX_NAME_LENGTH+1];	// 의호(?)

	BYTE	Gender;							// 성별
	BYTE	FaceType;						// 얼굴번호
	BYTE	HairType;						// 머리모양 번호

	WORD	WearedItemIdx[eWearedItem_Max];	// 착용한 아이탬들의 종류

	BYTE	Job;							// 직업
	BYTE	Stage;							// 경지
	BYTE	Level;							// 레벨
	BYTE	Map;							// 맵


	bool	bPeace;							// 전/평화모드 FALSE 면 전투 TRUE 면 평화

};
*/


//=================================================================================================
// NAME			: SetChangeScene()
// PURPOSE		: // 061218 LYW --- Change scene.
// ATTENTION	:
//=================================================================================================	
// 070522 LYW --- Delete change scene.
/*
void CCharMake::ChangeScene( BYTE mode ) 
{	
	switch( m_byOrder )
	{
		case e_SCENE_IN :
		{
			if( CHARMAKEMGR->GetCharMakeNewDlg()->GetAlphaWindow() )
			{
				if( m_dwAlphaCount < 300 )
				{
					if( m_dwAlphaCount <= 100 ) CHARMAKEMGR->GetCharMakeNewDlg()->GetAlphaWindow()->SetOptionAlpha( m_dwAlphaCount ) ;
					if( m_dwAlphaCount <= 255 ) CHARMAKEMGR->GetCharMakeNewDlg()->GetAlphaWindow()->SetAlpha( (BYTE)m_dwAlphaCount ) ;
					m_dwAlphaCount+= 5 ;
				}
				else if( m_dwAlphaCount >= 300 )
				{
					m_byOrder = e_SCENE_CHANGING ;
					DWORD abc = RGBA_MAKE(200, 200, 200, 255 ) ;
				}
			}
		}
		break ;

		case e_SCENE_CHANGING :
		{
			if( mode == e_HUMAN )
			{
				memcpy( &m_v3Eye, &m_v3Eye_Human, sizeof(VECTOR3) ) ;
				memcpy( &m_v3Rotate, &m_v3Rotate_Human, sizeof(VECTOR3) ) ;
				memcpy( &m_vCharPos, &m_vHumanPos, sizeof(VECTOR3 ) ) ;
				m_fCharAngle = m_fHumanAngle ;
			}
			else
			{
				memcpy( &m_v3Eye, &m_v3Eye_Elf, sizeof(VECTOR3) ) ;
				memcpy( &m_v3Rotate, &m_v3Rotate_Elf, sizeof(VECTOR3) ) ;
				memcpy( &m_vCharPos, &m_vElfPos, sizeof(VECTOR3 ) ) ;
				m_fCharAngle = m_fElfAngle ;
			}

			// 061219 LYW --- Setting camera position and character position.
			g_pExecutive->GetGeometry()->SetCameraPos(&m_v3Eye, 0);
			g_pExecutive->GetGeometry()->SetCameraAngleRad(&m_v3Rotate, 0);
			CHARMAKEMGR->GetNewPlayer()->SetAngle( m_fCharAngle ) ;
			CHARMAKEMGR->GetNewPlayer()->SetPosition(&m_vCharPos ) ;

			m_byOrder = e_SCENE_OUT ;
		}
		break ;

		case e_SCENE_OUT :
		{
			if( CHARMAKEMGR->GetCharMakeNewDlg()->GetAlphaWindow() )
			{
				if( m_dwAlphaCount > 0 )
				{
					if( m_dwAlphaCount <= 100 )CHARMAKEMGR->GetCharMakeNewDlg()->GetAlphaWindow()->SetOptionAlpha( m_dwAlphaCount ) ;
					if( m_dwAlphaCount <= 255 )CHARMAKEMGR->GetCharMakeNewDlg()->GetAlphaWindow()->SetAlpha( (BYTE)m_dwAlphaCount ) ;
					m_dwAlphaCount-= 5 ;
				}
				else if( m_dwAlphaCount <= 0 )
				{
					m_byOrder = e_SCENE_IN ;
					m_dwAlphaCount = 0 ;

					m_bChangeScene = FALSE ;

					CHARMAKEMGR->GetCharMakeNewDlg()->SetDisable( FALSE ) ;
				}
			}
		}
		break ;
	}
}
*/


//=================================================================================================
//	NAME		: UserConn_Character_NameCheck_Nack()
//	PURPOSE		: Process error message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CCharMake::UserConn_Character_NameCheck_Nack( void* pMsg )
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;

	CHARMAKEMGR->GetCharacterMakeInfo()->bDuplCheck = FALSE;
	cEditBox * editboxName = (cEditBox *)WINDOWMGR->GetWindowForIDEx(CM_IDEDITBOX);
	editboxName->SetEditText("");

	switch(pmsg->wData)
	{
	case 2:
		DisplayNotice( 14 );
		break;
	case 3:
		DisplayNotice( 14 );
		break;
	case 4:
		DisplayNotice( 14 );
		break;
	default:
		DisplayNotice( 14 );
		break;
	}

	SetDisableDlg( TRUE );

	// 070619 LYW --- GlobalEventFunc : Setting To Disable Gender button.
	// 성별을 선택할 수 없도록 하기 위해 Disable로 세팅해 준다.
	GetCharMakeNewDlg()->GetPushUpButton(e_PB_MAN)->SetDisable(TRUE) ;
	GetCharMakeNewDlg()->GetPushUpButton(e_PB_WOMAN)->SetDisable(TRUE) ;
}


//=================================================================================================
//	NAME		: UserConn_Character_NameCheck_Ack()
//	PURPOSE		: Process error message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CCharMake::UserConn_Character_NameCheck_Ack()
{
	CHARMAKEMGR->GetCharacterMakeInfo()->bDuplCheck =TRUE;
	
	DisplayNotice( 13 );
	SetDisableDlg( FALSE );

	// 070619 LYW --- GlobalEventFunc : Setting To Disable Gender button.
	// 성별을 선택할 수 없도록 하기 위해 Disable로 세팅해 준다.
	GetCharMakeNewDlg()->GetPushUpButton(e_PB_MAN)->SetDisable(TRUE) ;
	GetCharMakeNewDlg()->GetPushUpButton(e_PB_WOMAN)->SetDisable(TRUE) ;
}


//=================================================================================================
//	NAME		: UserConn_Character_Make_Nack()
//	PURPOSE		: Process error message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CCharMake::UserConn_Character_Make_Nack()
{
	DisplayNotice( 16 );

	SetDisableDlg( FALSE );
}


//=================================================================================================
//	NAME		: UserConn_CharacterList_Ack()
//	PURPOSE		: Process error message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CCharMake::UserConn_CharacterList_Ack( void* pMsg )
{
	SEND_CHARSELECT_INFO* pmsg = (SEND_CHARSELECT_INFO*)pMsg;
	SetDisableDlg( FALSE );
	MAINGAME->SetGameState(eGAMESTATE_CHARSELECT, (void *)pmsg, sizeof(SEND_CHARSELECT_INFO));									
	
	CAMERA->MoveStart(eCP_CHARSEL);		
}


//=================================================================================================
//	NAME		: Cheat_EventNotify_On()
//	PURPOSE		: Process error message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CCharMake::Cheat_EventNotify_On( void* pMsg  )
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
//	PURPOSE		: Process error message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CCharMake::Cheat_EventNotify_Off( void* pMsg )
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	NOTIFYMGR->SetEventNotify( FALSE );
	NOTIFYMGR->SetEventNotifyChanged( FALSE );
}


void CCharMake::ProcessDragRotation()
{
	CMouse*		mouse	= g_UserInput.GetMouse();
	CPlayer*	player	= CHARMAKEMGR->GetNewPlayer();

	if( ! mouse		||
		! player )
	{
		return;
	}

	if( mouse->RButtonDrag() || mouse->LButtonDrag())
	{
		int x = mouse->GetMouseAxisX();

		player->SetAngle( player->GetAngle() - x * 0.01f );
	}

/*
	const int	x	= mouse->GetMouseX();
	const int	y	= mouse->GetMouseY();

	if( mouse->LButtonDown() )
	{
		mPosition.mIsDrag	= TRUE;
		mPosition.mX		= mouse->GetMouseX();
		mPosition.mY		= mouse->GetMouseY();
	}
	else if( mouse->LButtonUp() )
	{
		mPosition.mIsDrag	= FALSE;
	}
	
	if( ! mPosition.mIsDrag ||
		( mPosition.mX == x && mPosition.mY == y ) )
	{
		if( mRotation.mRadian )
		{
			player->SetAngle( player->GetAngle() + mRotation.mSpeed );

			if( 0 > ( mRotation.mRadian -= abs( mRotation.mSpeed ) ) )
			{
				mRotation.mRadian = 0;
			}
		}

		return;
	}
	
	// 마우스를 클릭한채로 이동했으면 라디안 값을 구해 캐릭터를 회전시킨다.
	{
		//const DISPLAY_INFO&		screen		= GAMERESRCMNGR->m_GameDesc.dispInfo;
		const float				vectorSize	= sqrtf( pow( float( mPosition.mX - x ), 2 ) + pow( float( mPosition.mY - y ), 2 ) );
		const float				ratio		= vectorSize / 50.0f; // / screen.dwWidth * 50.0f;
		const float				direction	= ( x > mPosition.mX ? 1.0f : -1.0f );

		mRotation.mRadian		= 2.0f * PI * ratio;
		mRotation.mSpeed		= direction * ratio;

		mPosition.mX			= x;
		mPosition.mY			= y;
	}
	*/

	/*
	static int prevMouseX = -1;
	static int prevMouseY = -1;

	// 마우스가 클릭된 지점을 저장한다.
	if( (	mouse->LButtonDown() || mouse->RButtonDown() ) &&
			prevMouseX == -1 )
	{
		prevMouseX	= mouse->GetMouseX();
		prevMouseY	= mouse->GetMouseY();
	}
	// 저장된 지점이 있고 버튼을 떼었으면, 두 점으로 벡터 크기를 구한다.
	// 이 값에 대한 화면 비율을 2PI에 곱하여 회전할 라디안 값을 구하고,
	// 이를 기반으로 회전 속도도 구해낸다.
	else if(	(	mouse->LButtonDown()	|| mouse->RButtonDown() )	&&
				prevMouseX != -1 )
	{
		const DISPLAY_INFO&		screen		= GAMERESRCMNGR->m_GameDesc.dispInfo;
		const float				vectorSize	= sqrtf( pow( float( prevMouseX - mouse->GetMouseX() ), 2 ) + pow( float( prevMouseY - mouse->GetMouseY() ), 2 ) );
		const float				ratio		= vectorSize / screen.dwWidth;
		const float				direction	= ( prevMouseX < mouse->GetMouseX() ? 1.0f : -1.0f );

		mRotation.mRadian		= 2.0f * PI * ratio;
		mRotation.mSpeed		= direction * ratio / 2.0f;

		// 다시 드래그 지점을 구할 수 있도록 초기화한다
		prevMouseX	= -1;
	}
	// 라디안 값이 있을 동안 캐릭터를 회전시킨다.
	else if( mRotation.mRadian )
	{
		player->SetAngle( player->GetAngle() + mRotation.mSpeed );

		if( 0 > ( mRotation.mRadian -= abs( mRotation.mSpeed ) ) )
		{
			mRotation.mRadian = 0;
		}
	}
	*/
}

void CCharMake::UserConn_Character_Make_Ack( void* pMsg )
{
/*	MSG_DWORD* pmsg = ( MSG_DWORD* )pMsg;

	GameLoadingParam loadingParam;
	loadingParam.selectedCharNum = pmsg->dwData;
	loadingParam.m_MapNum = 19;

	MAINGAME->SetGameState(eGAMESTATE_GAMELOADING, (void *)&loadingParam, sizeof(GameLoadingParam));

	CAMERA->ResetInterPolation();

	gChannelNum = 0;

	m_bEnterGame = TRUE;*/
}

// CharSelect.cpp: implementation of the CCharSelect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CharSelect.h"
#include "MainGame.h"
#include "MainTitle.h"
#include "cWindowSystemFunc.h"
#include "WindowIDEnum.h"
#include "./interface/cWindowManager.h"
#include "./Interface/cPushupButton.h"
#include "./Interface/cResourceManager.h"
#include "ObjectManager.h"
#include "MHTimeManager.h"
#include "ChatManager.h"
#include "OptionManager.h"
#include "MHMap.h"
#include "MHCamera.h"
#include "./Interface/cScriptManager.h"

#include "UserInfoManager.h"

// 061215 LYW --- Include profile dialog's header file.
#include "ProfileDlg.h"

// 061218 LYW --- Include certificate dialog's header file.
#include "CertificateDlg.h"

// 061217 LYW --- Add for test.
#include "../4DyuchiGRX_common/math.inl"
#include "CharMakeManager.h"
// test end.

#include "cMsgBox.h"
#include "./Input/UserInput.h"

//#include "Gamein.h"
#include "GMNotifyManager.h"
//#include "EventNotifyDialog.h"

// 070201 LYW --- Include game resorce manager.
#include "../[CC]Header/GameResourceManager.h"

#ifdef TAIWAN_LOCAL
#include "BillingManager.h"
#endif


#define CHARSELECT_STAND_POSX	27930
#define CHARSELECT_STAND_POSZ	7834
#define CHARSELECT_STAND_GAP	145
#define CHARSELECT_STAND_HEIGHT	100

// 061218 LYW --- Change define number for character sloat.
//#define _MAX_CHARACTER_NUM 4
#define _MAX_CHARACTER_NUM 5

// 061214 LYW --- Change character position.
/*
const VECTOR3 c_MAN_STAND_POS[_MAX_CHARACTER_NUM] = 
{ 
	{ 1969.792f, 0.0f, 1863.346f },
	{ 2077.243f, 0.0f, 1702.041f },
	{ 2274.891f, 0.0f, 1709.814f },
	{ 2321.619f, 0.0f, 1564.172f } 
};

const VECTOR3 c_WOMAN_STAND_POS[_MAX_CHARACTER_NUM] = 
{		
	{ 1969.792f, 0.0f, 1863.346f },
	{ 2077.243f, 0.0f, 1702.041f },
	{ 2274.891f, 0.0f, 1709.814f },
	{ 2321.619f, 0.0f, 1564.172f }
};
*/
VECTOR3 c_MAN_STAND_POS[_MAX_CHARACTER_NUM] = 
{ 
	// 070125 LYW --- Character slot : Modified this part.
	/*
	{ 11441.0f, 5000.0f, 8516.0f },
	{ 11462.0f, 5000.0f, 8601.0f },
	{ 11519.0f, 5000.0f, 8674.0f },
	{ 11601.0f, 5000.0f, 8671.0f }
	*/
	{ 11441.0f, 5000.0f, 8516.0f },
	{ 11462.0f, 5000.0f, 8601.0f },
	{ 11519.0f, 5000.0f, 8674.0f },
	{ 11601.0f, 5000.0f, 8671.0f }, 
	{ 11637.0f, 5000.0f, 8596.0f } 
};

VECTOR3 c_WOMAN_STAND_POS[_MAX_CHARACTER_NUM] = 
{	
	// 070125 LYW --- Character slot : Modified this part.
	/*
	{ 11441.0f, 5000.0f, 8516.0f },
	{ 11462.0f, 5000.0f, 8601.0f },
	{ 11519.0f, 5000.0f, 8674.0f },
	{ 11601.0f, 5000.0f, 8671.0f }
	*/
	{ 11441.0f, 5000.0f, 8516.0f },
	{ 11462.0f, 5000.0f, 8601.0f },
	{ 11519.0f, 5000.0f, 8674.0f },
	{ 11601.0f, 5000.0f, 8671.0f }, 
	{ 11637.0f, 5000.0f, 8596.0f } 
};

// 061218 LYW --- Add for test.

float fCharAngle[ _MAX_CHARACTER_NUM ] ;
int nCurAngle ;

VECTOR3 g_scale;
int g_ncount ;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CCharSelect)
CCharSelect::CCharSelect()
{
	m_CurSelectedPlayer = -1;
	m_pDoublcClickedPlayer = NULL;
//	m_bDoublcClicked	= FALSE;

	ZeroMemory( m_pPlayer, sizeof(CPlayer*)*MAX_CHARACTER_NUM );

	// 061215 LYW --- Delete this code for a moment.
	//m_pSelectDlg = NULL;

	// 061215 LYW --- Add profile dialog.
	m_pProfile = NULL ;
	
	m_pChannelDlg = NULL;

	m_ExtraCharacterCount = 0;

	m_bEnterGame = FALSE;

	// 061217 LYW --- Add for test.
	m_v3Eye.x = 11530.0f ;
	m_v3Eye.y = 5070.0f ;
	m_v3Eye.z = 7980.0f ;
	m_v3Rotate.x = 0.02f ;
	m_v3Rotate.y = 6.32f ;
	m_v3Rotate.z = 0.0f ;
	// end test.

	// 061218 LYW --- Add for test.

	fCharAngle[0] =  0.2f ;
	fCharAngle[1] =  0.2f ;
	fCharAngle[2] =  0.1f ;
	fCharAngle[3] = -0.1f ;
	nCurAngle = 0 ;
	// end test.
	/*
	// 061217 LYW --- Add for test.
	m_v3Eye.x = 11531.048f ;
	m_v3Eye.y = 5077.773f ;
	m_v3Eye.z = 7976.678f ;
	m_v3Rotate.x = 91.658f ;
	m_v3Rotate.y = -2.299f ;
	m_v3Rotate.z = 0.286f ;
	// end test.

	// 061218 LYW --- Add for test.
	fCharAngle[0] =  0.3f ;
	fCharAngle[1] =  0.2f ;
	fCharAngle[2] =  0.1f ;
	fCharAngle[3] = -0.1f ;
	nCurAngle = 0  ;
	*/

	/*
	g_scale.x = 1.0f ;	
	g_scale.y = 1.0f ;	
	g_scale.z = 1.0f ;	
	g_ncount = 0 ;
	*/

	// 070313 LYW --- CharSelect : Initialize array.
	for( int i=0; i < 5; ++i )
	{
		m_nRandom[i] = 0 ;
	}

	// 071114, 웅주. 채널 개수 초기화
	{
		const DWORD size = sizeof( m_MaxChannel ) / sizeof( *m_MaxChannel );

		for( DWORD i = 0; i < size; ++i )
		{
			m_MaxChannel[ i ] = 10;
		}
	}
}

CCharSelect::~CCharSelect()
{
	
}

BOOL CCharSelect::Init(void* pInitParam)
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

	//YH2DO -_-
	if(MAP->IsInited() == FALSE)
	{
		// 061214 LYW --- Change map.
		//MAP->InitMap(103);
		// 061225 LYW --- Change title map.
		//MAP->InitMap(25);
		MAP->InitMap(200) ;
	}		

//	ZeroMemory( m_pPlayer, MAX_CHARACTER_NUM * sizeof(sizeof(CPlayer*)) );
	ZeroMemory( m_pPlayer, sizeof(CPlayer*)*MAX_CHARACTER_NUM );	//KES 070601 : sizeof(sizeof(CPlayer*)) 이상한 코드
	
	NETWORK->SetCurState(this);

	NewCreateCharSelect_m();
	// 061215 LYW --- Delete this code for a moment.
	//m_pSelectDlg = WINDOWMGR->GetWindowForID( CS_CHARSELECTDLG );
	CreateChannelDlg();
	m_pChannelDlg = (CChannelDialog*)WINDOWMGR->GetWindowForID( CHA_CHANNELDLG );
	if(m_pChannelDlg)
	{
		m_pChannelDlg->Linking();
	}

	// 061215 LYW --- Add profile dialog for character information.
	CreateProfileDlg() ;
	m_pProfile = ( CProfileDlg* )WINDOWMGR->GetWindowForID( CP_PROFILEDLG ) ;

	// 061218 LYW --- Add dialog for certificate some condition.
	CreateCertificateDlg() ;
	m_pCertificateDlg->SetActive( FALSE ) ;


	WINDOWMGR->AfterInit();
	ASSERT(pInitParam);
	SEND_CHARSELECT_INFO * pmsg = (SEND_CHARSELECT_INFO *)pInitParam;
	NetworkMsgParse(pmsg->Category, pmsg->Protocol, pInitParam);

	//////////////////////////////////////////////////////////////////////////
	// 그림자 처리를 위해
	VECTOR3 pos = {0,0,0};
	float xpos = CHARSELECT_STAND_POSX;
	float zpos = CHARSELECT_STAND_POSZ;
	float gap = CHARSELECT_STAND_GAP;
	for(int n=0;n<MAX_CHARACTER_NUM;++n)
	{
//		pos.x += CHARSELECT_STAND_POSX;
//		pos.y += CHARSELECT_STAND_HEIGHT;		
//		pos.z += CHARSELECT_STAND_POSZ + n * CHARSELECT_STAND_GAP + 100;
		
		pos.x = xpos + (n *120);
		pos.y = CHARSELECT_STAND_HEIGHT;
		pos.z = zpos + 30 * n;
	}
//	pos.x /= 4.f;
//	pos.y /= 4.f;
//	pos.z /= 4.f;
	
	MAP->SetShadowPivotPos(&pos);
	//////////////////////////////////////////////////////////////////////////
	
//KES 030827 옮겨옴 <-- 파싱함수로 부터
	InitCameraInCharSelect();
	
	m_CurSelectedPlayer = -1;
	SelectPlayer( -1 );
	m_pDoublcClickedPlayer = NULL;
	// 061215 LYW --- Delete this code for a moment.
	//SetDisablePick( FALSE );

	m_bBackToMainTitle = FALSE;
//	m_ExtraCharacterCount = 0;
//	m_bCharacterSlotInit = FALSE;

	m_bEnterGame = FALSE;

	// 070208 LYW --- CharSelect : Add function to setting position and scale of image.
	SetPositionScale() ;

	// 071001 LYW --- CharSelect : Initialize whisper name of chatting manager.
	CHATMGR->InitializeWhisperName() ;

	return TRUE;
}


// 070208 LYW --- CharSelect : Add function to setting position and scale of image.
void CCharSelect::SetPositionScale()
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

void CCharSelect::Release(CGameState* pNextGameState)
{
	m_imageLoginBarTop.Release();
	m_imageLoginBarBottom.Release();	

	OBJECTMGR->RemoveAllObject();
	WINDOWMGR->DestroyWindowAll();
	RESRCMGR->ReleaseResource(11);
		
	if(pNextGameState == NULL || m_bEnterGame )
		MAP->Release();
			
	//map release()하는 곳이 없다?

}

void CCharSelect::Process()
{
///	MHTIMEMGR->Process();	

	/*
	if(CAMERA->GetGotoPosition() != -1)
		CAMERA->Process();
		*/		
	// 061214 LYW --- Move position of character while walking.
	/*
	if(m_pPlayer[0])
	{
		if( m_pPlayer[0]->GetEngineObject()->GetCurMotion() == eMotion_Sel1Afterstop )
		{
			VECTOR3 playerPos ;
			m_pPlayer[0]->GetPosition( &playerPos ) ;
			
			if( playerPos.z > 14800.0f )
			{
				playerPos.z -= 5.0 ;
				m_pPlayer[0]->SetPosition( &playerPos ) ;
			}
			else
			{
				//pPlayer->ChangeBaseMotion(eMotion_Sel1Before);//+4*standpoint);
				m_pPlayer[0]->ChangeMotion(eMotion_Peace_Standard,0);//+4*standpoint, 0);
			}
			
			if( playerPos.x > 31500.0f )
			{
				playerPos.x -= 5.0 ;
				m_pPlayer[0]->SetPosition( &playerPos ) ;
			}
		}
	}
	*/

	if( m_pDoublcClickedPlayer )
	{
		int iMotionNum = m_pDoublcClickedPlayer->GetEngineObject()->GetCurMotion();

		if( iMotionNum == Motion_SelAfterstop )
		{
			SendMsgGetChannelInfo();

			// 카메라 이동보간 리셋, 하이트필드 적용
//			CAMERA->ResetInterPolation();

			m_pDoublcClickedPlayer = NULL;
		}
	}	

//KES
	if( WINDOWMGR->IsMouseInputProcessed() ) return;	//dialog가 눌렸다.
	if( m_bDiablePick ) return;							//메세지박스가 떴다.
//

//마우스로 클릭
	if( MOUSE->LButtonDoubleClick() )
	{
		// 캐릭터 선택화면에서는 특별히 bone box 로 picking한다.
		//CPlayer* pPlayer = (CPlayer*)GetSelectedObject( MOUSE->GetMouseEventX(), MOUSE->GetMouseEventY());
		CPlayer* pPlayer = (CPlayer*)GetSelectedObjectBoneCheck( MOUSE->GetMouseEventX(), MOUSE->GetMouseEventY());

		if( pPlayer && m_pDoublcClickedPlayer == NULL )
		if( pPlayer == GetCurSelectedPlayer() )
		{
			m_pDoublcClickedPlayer = pPlayer;
			SetDisablePick( TRUE );
		}
	}
	else if( MOUSE->LButtonDown()  )
	{		
		if( GetCurSelectedPlayer() )		
		{
			int iMotionNumber = GetCurSelectedPlayer()->GetEngineObject()->GetCurMotion();

			if( iMotionNumber != eMotion_Sel1Afterstop &&
				iMotionNumber != eMotion_Sel2Afterstop &&
				iMotionNumber != eMotion_Sel3Afterstop &&
				iMotionNumber != eMotion_Sel4Afterstop &&
				iMotionNumber != eMotion_Sel5Afterstop)		
		
			{
				return;				
			}

		}

		//CPlayer* pPlayer = (CPlayer*)GetSelectedObject( MOUSE->GetMouseEventX(), MOUSE->GetMouseEventY());
		CPlayer* pPlayer = (CPlayer*)GetSelectedObjectBoneCheck( MOUSE->GetMouseEventX(), MOUSE->GetMouseEventY());

		if( pPlayer && m_pDoublcClickedPlayer == NULL )
		{
			for( int i = 0 ; i < MAX_CHARACTER_NUM ; ++i )
			{
				if( m_pPlayer[i] == pPlayer )
				{
					SelectPlayer( i );
					break;
				}
			}
		}
	}

	// 070313 LYW --- CharSelect : Check eye time.
	for( int count = 0 ; count < 5 ; ++count )
	{
		if( m_pPlayer[count] )
		{
			if( m_pPlayer[count]->GetEyeTime()->Check() == 0 )
			{
				m_pPlayer[count]->InitializeEye() ;
			}
		}
	}

	// 061217 LYW --- Add for test.
	/*
	MHCAMERADESC* pCurCamera = CAMERA->GetCameraDesc() ;
	if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_W ) )
	{
		m_v3Eye.z += 10.0f ;
	}
	// 뒤
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_S ) )
	{
		m_v3Eye.z -= 10.0f ;
	}
	// 상
	if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_Q ) )
	{
		m_v3Eye.y += 10.0f ;
	}
	// 하
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_E ) )
	{
		m_v3Eye.y -= 10.0f ;
	}
	// 좌
	if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_A ) )
	{
		m_v3Eye.x -= 10.0f ;
	}
	// 우
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_D ) )
	{
		m_v3Eye.x += 10.0f ;
	}

	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_F ) )
	{
		m_v3Eye.x = 11550.0f ;
		m_v3Eye.y = 5050.0f ;
		m_v3Eye.z = 8150.0f ;
		m_v3Rotate.x = 0.0f ;
		m_v3Rotate.y = 6.29f ;
		m_v3Rotate.z = 0.0f ;
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_T ) )
	{
		if( nCurAngle > 0 )
		{
			nCurAngle-- ;
		}
		else
		{
			nCurAngle = 3 ;
		}
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_U ) )
	{
		if( nCurAngle < 3 )
		{
			nCurAngle++ ;
		}
		else
		{
			nCurAngle = 0 ;
		}
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_G ) )
	{
		fCharAngle[nCurAngle] -= 0.1f ;
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_H ) )
	{
		fCharAngle[nCurAngle] += 0.1f ;
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
		c_MAN_STAND_POS[nCurAngle].x -= 1.0f ;
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_RIGHT ) )
	{
		c_MAN_STAND_POS[nCurAngle].x += 1.0f ;
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_UP ) )
	{
		c_MAN_STAND_POS[nCurAngle].z -= 1.0f ;
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_DOWN ) )
	{
		c_MAN_STAND_POS[nCurAngle].z += 1.0f ;
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_Z ) )
	{
		c_MAN_STAND_POS[nCurAngle].y -= 1.0f ;
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_X ) )
	{
		c_MAN_STAND_POS[nCurAngle].y += 1.0f ;
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_3 ) )
	{
		g_scale.y -= 0.1f ;

		m_pPlayer[nCurAngle]->GetEngineObject()->SetScale(&g_scale) ;
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_4 ) )
	{
		g_scale.y += 0.1f ;

		m_pPlayer[nCurAngle]->GetEngineObject()->SetScale(&g_scale) ;
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_5 ) )
	{
		g_scale.x -= 0.1f ;

		m_pPlayer[nCurAngle]->GetEngineObject()->SetScale(&g_scale) ;
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_6 ) )
	{
		g_scale.x += 0.1f ;

		m_pPlayer[nCurAngle]->GetEngineObject()->SetScale(&g_scale) ;
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_7 ) )
	{
		if( g_ncount > 0 ) g_ncount-- ;
		else g_ncount = 0 ;

		m_pPlayer[0]->ChangeMotion(eMotion_Ungijosik_Ing + g_ncount, 0);
	}
	else if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_8 ) )
	{
		if( g_ncount < eMotion_Tactic_Excute ) g_ncount++ ;
		else g_ncount = eMotion_Tactic_Excute ;

		m_pPlayer[0]->ChangeMotion(eMotion_Ungijosik_Ing + g_ncount, 0);
	}
	if( g_UserInput.GetMouse()->MButtonPressed() )
	{
		if( g_ncount > 0 ) g_ncount-- ;
		else g_ncount = 0 ;

		m_pPlayer[0]->ChangeMotion(eMotion_Peace_Standard,0);
		m_pPlayer[0]->ChangeBaseMotion(eMotion_Peace_Standard + g_ncount);
	}
	if( g_UserInput.GetMouse()->RButtonDoubleClick() )
	{
		if( g_ncount < eMotion_Tactic_Excute ) g_ncount++ ;
		else g_ncount = eMotion_Tactic_Excute ;

		m_pPlayer[0]->ChangeMotion(eMotion_Peace_Standard,0);
		m_pPlayer[0]->ChangeBaseMotion(eMotion_Peace_Standard + g_ncount);
	}

	g_pExecutive->GetGeometry()->SetCameraPos(&m_v3Eye, 0);
	g_pExecutive->GetGeometry()->SetCameraAngleRad(&m_v3Rotate, 0);

	if( m_pPlayer[nCurAngle] )
	{
		m_pPlayer[nCurAngle]->SetAngle(fCharAngle[nCurAngle]);
		m_pPlayer[nCurAngle]->SetPosition(&c_MAN_STAND_POS[nCurAngle] ) ;
	}
	// test end.
	*/
	
}

void CCharSelect::BeforeRender()
{
	g_pExecutive->GetRenderer()->BeginRender(0,0,0);
	g_pExecutive->GetRenderer()->EndRender();
	MAP->Process( gCurTime );
}

void CCharSelect::AfterRender()
{
	m_imageLoginBarTop.RenderSprite(&m_scale,0,0,&m_v2TopPos,0xffffffff);
	m_imageLoginBarBottom.RenderSprite(&m_scale,0,0,&m_v2BottomPos,0xffffffff);	

	WINDOWMGR->Render();
	// 061217 LYW --- Add for test.
	/*
	char tempStr[ 128 ] ;
	RECT rc ;
	rc.left = 10 ;
	rc.top = 300 ;
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

	// 061218 LYW --- Add for test.
	//fCharAngle
	sprintf( tempStr, "1:Angle = %f", fCharAngle[0] ) ;
	rc.top += 20 ;
	CFONT_OBJ->RenderFont(0, tempStr, strlen( tempStr ), &rc, RGB( 255, 0, 0 ) ) ;

	sprintf( tempStr, "2:Angle = %f", fCharAngle[1] ) ;
	rc.top += 20 ;
	CFONT_OBJ->RenderFont(0, tempStr, strlen( tempStr ), &rc, RGB( 255, 0, 0 ) ) ;

	sprintf( tempStr, "3:Angle = %f", fCharAngle[2] ) ;
	rc.top += 20 ;
	CFONT_OBJ->RenderFont(0, tempStr, strlen( tempStr ), &rc, RGB( 255, 0, 0 ) ) ;

	sprintf( tempStr, "4:Angle = %f", fCharAngle[3] ) ;
	rc.top += 20 ;
	CFONT_OBJ->RenderFont(0, tempStr, strlen( tempStr ), &rc, RGB( 255, 0, 0 ) ) ;

	sprintf( tempStr, "5:Angle = %f", fCharAngle[4] ) ;
	rc.top += 20 ;
	CFONT_OBJ->RenderFont(0, tempStr, strlen( tempStr ), &rc, RGB( 255, 0, 0 ) ) ;

	sprintf( tempStr, "nCurAngle = %d", nCurAngle ) ;
	rc.top += 20 ;
	CFONT_OBJ->RenderFont(0, tempStr, strlen( tempStr ), &rc, RGB( 255, 0, 0 ) ) ;

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

	sprintf( tempStr, "CharPos X = %f", c_MAN_STAND_POS[nCurAngle].x ) ;
	rc.top += 20 ;
	CFONT_OBJ->RenderFont(0, tempStr, strlen( tempStr ), &rc, RGB( 0, 0, 255 ) ) ;

	sprintf( tempStr, "CharPos Y = %f", c_MAN_STAND_POS[nCurAngle].y ) ;
	rc.top += 20 ;
	CFONT_OBJ->RenderFont(0, tempStr, strlen( tempStr ), &rc, RGB( 0, 0, 255 ) ) ;

	sprintf( tempStr, "CharPos Z = %f", c_MAN_STAND_POS[nCurAngle].z ) ;
	rc.top += 20 ;
	CFONT_OBJ->RenderFont(0, tempStr, strlen( tempStr ), &rc, RGB( 0, 0, 255 ) ) ;

	sprintf( tempStr, "CharAngle = %f", fCharAngle[nCurAngle] ) ;
	rc.top += 20 ;
	CFONT_OBJ->RenderFont(0, tempStr, strlen( tempStr ), &rc, RGB( 0, 0, 255 ) ) ;

	sprintf( tempStr, "Width Z = %f", g_scale.x ) ;
	rc.top += 20 ;
	CFONT_OBJ->RenderFont(0, tempStr, strlen( tempStr ), &rc, RGB( 0, 0, 255 ) ) ;

	sprintf( tempStr, "Height = %f", g_scale.y ) ;
	rc.top += 20 ;
	CFONT_OBJ->RenderFont(0, tempStr, strlen( tempStr ), &rc, RGB( 0, 0, 255 ) ) ;

	sprintf( tempStr, "Current Motion = %d", g_ncount+1 ) ;
	rc.top += 20 ;
	CFONT_OBJ->RenderFont(0, tempStr, strlen( tempStr ), &rc, RGB( 0, 0, 255 ) ) ;
	*/
	
}

CPlayer* CCharSelect::GetCurSelectedPlayer()
{
	if(m_CurSelectedPlayer == -1)
		return NULL;
	CPlayer* pPlayer = m_pPlayer[m_CurSelectedPlayer];
		return pPlayer;
}

void CCharSelect::NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg)
{
	switch(Category) 
	{
	case MP_USERCONN:
		{
			switch(Protocol) 
			{
			case MP_USERCONN_REMAINTIME_NOTIFY:
				{
					UserConn_RemainTime_Notify( pMsg ) ;
				}
				break;
			case MP_USERCONN_CHARACTERLIST_ACK:
				{	
					UserConn_CharacterList_Ack( pMsg ) ;
				}				
				return;
			case MP_USERCONN_CHARACTERLIST_NACK:
				{
					LOG(EC_CHARACTERLIST_FAILED);
				}				
				return;
			case MP_USERCONN_CHARACTER_REMOVE_ACK:
				{
					UserConn_Character_Remove_Ack() ;
				}
				return;
			case MP_USERCONN_CHARACTER_REMOVE_NACK:
				{
					UserConn_Character_Remove_Nack( pMsg ) ;
				}
				return;

			case MP_USERCONN_DISCONNECT_ACK:
				{
					UserConn_DisConnect_Ack() ;
				}
				return;
			case MP_USERCONN_CHANNELINFO_ACK:
				{
					UserConn_ChannelInfo_Ack( pMsg ) ;
				}
				return;
			case MP_USERCONN_CHANNELINFO_NACK:
				{
					UserConn_ChannelInfo_Nack( pMsg ) ;
				}
				return;
			// desc_hseos_성별선택01
			// S 성별선택 추가 added by hseos 2007.06.17
			case MP_USERCONN_USER_SEXKIND:
				{
					UserConn_Character_SexKind(pMsg);
				}
				return;
			// E 성별선택 추가 added by hseos 2007.06.17
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
	}
	
//	LOG(EC_UNKNOWN_PROTOCOL);
}

void CCharSelect::InitCameraInCharSelect()
{
	//최종 세팅될 결과
//	VECTOR3 vPivot;

//* 카메라 이동
	/*
	if(CAMERA->IsMoved() == FALSE)
	{
		vPivot.x = 27740;
		vPivot.y = 4960;
		vPivot.z = 9104;

		CAMERA->InitCamera(0, -2.0f, 17.0f, 2000, &vPivot);// 거리 조정 

		//
		MHCAMERAPOS CPos;
		CPos.m_AngleX = -2.0f;
		CPos.m_AngleY = 17.0f;
		CPos.m_Dist = 2000;
		CPos.m_Pivot = vPivot;
		CAMERA->SetMovePos(eCP_CHARSEL, &CPos);

		vPivot.x = 24480;
		vPivot.y = 6045;
		vPivot.z = 9854;	
		CPos.m_AngleX = -6.0f;
		CPos.m_AngleY = 0.0f;
		CPos.m_Dist = 2000;
		CPos.m_Pivot = vPivot;
		CAMERA->SetMovePos(eCP_CHARMAKE, &CPos);
	}
//*/

	/*if(CAMERA->IsMoved() == FALSE)
	{			
		vPivot.x = 1545.0f;
		vPivot.y = 310.0f;
		vPivot.z = 1047.269f;

		CAMERA->InitCamera(0, RADTODEG(-0.076794f), RADTODEG(0.649262f), 2000, &vPivot);// 거리 조정 
		*/

		//MHCamera Object를 이용하면 좌표 계산이 의도한대로 되지 않는다. 그래서 직접 Camera를 세팅.
		// 061214 LYW --- Change camera position.
		/*
		VECTOR3 v3Eye		= { 1899.600586f, 460.0f, 864.214050f };
		VECTOR3 v3Rotate	= { -0.335103f, 0.300197f, 0.0f	};
		*/
		// 061217 LYW --- Change for test.
		/*
		VECTOR3 v3Eye		= { 16500.0f, 1200.0f, 35000.0f };	
		VECTOR3 v3Rotate	= { 0.0f, 0.2f, 0.0f };

		g_pExecutive->GetGeometry()->SetCameraPos(&v3Eye, 0);
		g_pExecutive->GetGeometry()->SetCameraAngleRad(&v3Rotate, 0);
		*/
		g_pExecutive->GetGeometry()->SetCameraPos(&m_v3Eye, 0);
		g_pExecutive->GetGeometry()->SetCameraAngleRad(&m_v3Rotate, 0);

		// 061218 LYW --- Setting direction of character
		for( int i=0; i<4; i++ )
		{
			if( m_pPlayer[i] )m_pPlayer[i]->SetAngle( fCharAngle[nCurAngle] ) ;
		}
		// end test.

		/*
		MHCAMERAPOS CPos;
		CPos.m_AngleX = -0.237365f;
		CPos.m_AngleY = 0.293216f;
		CPos.m_Dist = 2000;
		CPos.m_Pivot = vPivot;
		CAMERA->SetMovePos(eCP_CHARSEL, &CPos);

	}*/
	

/* 이동안시킬땐 이걸루..
	vPivot.x = 27740;
	vPivot.y = 4960;
	vPivot.z = 9104;

	CAMERA->InitCamera(0, -2.0f, 17.0f, 2000, &vPivot);// 거리 조정 
//*/
	// 061217 LYW --- Delete this code.
	/*
	CAMERA->SetCameraMode(eCM_LOGIN);
	CAMERA->SetCurCamera(0);
	*/

	//카메라 기능... 타겟위치로의 애니메이션(카메라, 피벗) 필요.	

/*
	VECTOR3 vPivot;
	vPivot.x = 23558;
	vPivot.y = 671;
	vPivot.z = 31780;

	CAMERA->InitCamera(0, 0.0f, 90.0f, 5115, &vPivot);// 거리 조정 
	CAMERA->SetCameraMode(eCM_LOGIN);
*/	/// 메인타이틀의 세팅상황
	
/*
	VECTOR3 pos;

//	pos.x = 25400; //값을 늘이면 우측으로 
	pos.x = 25750;
//	pos.y = -100;
	pos.y = -70;
	pos.z = 39900;
	
	CAMERA->InitCamera(0, 0.17f, 0, 10500, &pos);// 거리 조정 
//	CAMERA->InitCamera(0, 2.0, 0, 10500, &pos);// 거리 조정 
	CAMERA->SetCameraMode(eCM_LOGIN);

	CAMERA->SetCurCamera(0);
	
	if(m_bReturnOfCharMake == FALSE)
	{
//		CAMERA->ZoomDistance(0, -1200.f, 1700);		
		CAMERA->ZoomDistance(0, -0550.f, 1700);			
		CAMERA->SetTargetAngleX(0,0.8f,1700);
		CAMERA->SetTargetAngleY(0,0.25f,1700);
		CAMERA->SetTargetAngleY(0,0.6f,0);
	}
	else
	{
//		CAMERA->ZoomDistance(0, 900, 1700); // pjs 카메라 뒤로 이동 [5/13/2003]
		CAMERA->ZoomDistance(0, 1200, 1700);
	}
*/


}

void CCharSelect::SetCurSelctedPlayer(int num)
{	
	cPushupButton * charBtn[MAX_CHARACTER_NUM];

	if(num == -1)
	{
		for(int n=0;n<MAX_CHARACTER_NUM;++n)
		{
			charBtn[n] = (cPushupButton *)WINDOWMGR->GetWindowForIDEx(MT_FIRSTCHOSEBTN+n);
			charBtn[n]->SetPush(FALSE);
		}
		m_CurSelectedPlayer = -1;
		return;
	}
	for(int n=0;n<MAX_CHARACTER_NUM;++n)
	{
		charBtn[n] = (cPushupButton *)WINDOWMGR->GetWindowForIDEx(MT_FIRSTCHOSEBTN+n);
		if(n == num)
			charBtn[n]->SetPush(TRUE);
		else
			charBtn[n]->SetPush(FALSE);
	}
	m_CurSelectedPlayer = num;
}

void CCharSelect::SelectPlayer(int num)
{
	// 071210 LYW --- CharSelect : 캐릭터 삭제 메시지가 떠 있을 때는 캐릭터 선택이 되지 않도록 한다.
	cMsgBox* pMsgBox = NULL ;
	pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID( MBI_DELETECHAR ) ;

	if( pMsgBox )
	{
		if( pMsgBox->IsActive() )
		{
			return ;
		}
	}

    // 071210 LYW --- CharSelect : 캐릭터 삭제 번호가 뜬 상태에서는 캐릭터 선택이 되지 않도록 한다.
	if( m_pCertificateDlg->IsActive() )
	{
		return ;
	}

	if( GetCurSelectedPlayer() )
	{
		int iMotionNum = GetCurSelectedPlayer()->GetEngineObject()->GetCurMotion();

		if (iMotionNum != eMotion_Sel1Afterstop &&
			iMotionNum != eMotion_Sel2Afterstop &&
			iMotionNum != eMotion_Sel3Afterstop &&
			iMotionNum != eMotion_Sel4Afterstop &&
			iMotionNum != eMotion_Sel5Afterstop)
		{
			cPushupButton * charBtn = (cPushupButton *)WINDOWMGR->GetWindowForIDEx(MT_FIRSTCHOSEBTN + num);
			charBtn->SetPush( FALSE );
			return;
		}
	}
	
	if( num == m_CurSelectedPlayer) 
	{
		return;
	}

	if(num != -1)
	if(m_pPlayer[num] == NULL)
	{
			cPushupButton * charBtn[MAX_CHARACTER_NUM];
			charBtn[num] = (cPushupButton *)WINDOWMGR->GetWindowForIDEx(MT_FIRSTCHOSEBTN+num);
			charBtn[num]->SetPush(FALSE);
			return;

//		cPushupButton * charBtn[4];
//		charBtn[num] = (cPushupButton *)WINDOWMGR->GetWindowForIDEx(MT_FIRSTCHOSEBTN+num);
//		charBtn[num]->SetPush(FALSE);
//		return;
	}

/*
	if(num == m_CurSelectedPlayer)
	{
		//서있는 애를 뒤돌아가게 하고 
		SetCurSelctedPlayer(-1);
		PlayDeselectedMotion(num);
		return;
	}
*/	
	PlayDeselectedMotion(m_CurSelectedPlayer);
	PlaySelectedMotion(num);

	SetCurSelctedPlayer(num);
	
	// 061215 LYW --- Update profile.
	if( m_pPlayer[num] != NULL )
	{
		m_pProfile->UpdateProfile( m_pPlayer[num]->GetObjectName(), m_pPlayer[num]->GetCharacterTotalInfo() ) ; 
	}
}

BOOL CCharSelect::IsFull()
{
	// 061226 LYW --- Check user whether has charge slot or not.
	// 070125 LYW --- CharMake slot : Modified this line for a moment.
	//BOOL bHasChargeSlot = FALSE ;
	BOOL bHasChargeSlot = TRUE ;
	BYTE bySlotCount	= 0x00 ;
	if( bHasChargeSlot )
	{
		bySlotCount = 0x05 ;
	}
	else
	{
		bySlotCount = 0x03 ;
	}

	for( int n=0; n<bySlotCount; ++n )
	{
		if(m_pPlayer[n] == NULL)
			return FALSE;
	}



	// 061226 LYW --- Delete this code.
	/*
	for(int n=0;n<MAX_CHARACTER_NUM;++n)
	{
		if(m_pPlayer[n] == NULL)
			return FALSE;
	}
	*/

	return TRUE;
}

BOOL CCharSelect::EnterGame()
{
	CPlayer* pPlayer = GetCurSelectedPlayer();
	if(pPlayer == NULL)
		return FALSE;
	GameLoadingParam loadingParam;
	loadingParam.selectedCharNum = pPlayer->GetID();
	loadingParam.m_MapNum = pPlayer->GetCharacterTotalInfo()->LoginMapNum;

	MAINGAME->SetGameState(eGAMESTATE_GAMELOADING, (void *)&loadingParam, sizeof(GameLoadingParam));

	m_bEnterGame = TRUE;

	// 카메라 이동보간 리셋, 하이트필드 적용
	CAMERA->ResetInterPolation();

	return TRUE;
}

void CCharSelect::DeleteCharacter()
{
	CPlayer* pPlayer = GetCurSelectedPlayer();
	if(pPlayer == NULL)
		return;
		
	MSG_DWORD  msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHARACTER_REMOVE_SYN;
	msg.dwData = pPlayer->GetID();
	NETWORK->Send(&msg,sizeof(msg));
}

void CCharSelect::PlaySelectedMotion(int num)
{
	if(num == -1)
		return;
	if(m_pPlayer[num] == NULL)
		return;

	// 061226 LYW --- Change motion of character.
	/*
	m_pPlayer[num]->ChangeMotion(eMotion_sel1After+4*num,0);
	m_pPlayer[num]->ChangeBaseMotion(eMotion_Sel1Afterstop+4*num);
	*/
	int baseMotion = 0 ;
	int motion = 0 ;

	switch( num )
	{
	case 0 :
		{
			motion		= Motion_SelAfter ;
			baseMotion	= Motion_SelAfterstop ;
		}
		break ;

	case 1 :
		{
			motion		= Motion_SelAfter ;
			baseMotion	= Motion_SelAfterstop ;
		}
		break ;

	case 2 :
		{
			motion		= Motion_SelAfter ;
			baseMotion	= Motion_SelAfterstop ;
		}
		break ;

	case 3 :
		{
			motion		= Motion_SelAfter ;
			baseMotion	= Motion_SelAfterstop ;
		}
		break ;

	case 4 :
		{
			motion		= Motion_SelAfter ;
			baseMotion	= Motion_SelAfterstop ;
		}
		break ;

	default :
		break ;
	}

	m_pPlayer[num]->ChangeMotion(motion,0);
	m_pPlayer[num]->ChangeBaseMotion(baseMotion);

//KES 030904
//	m_pPlayer[num]->DisPlayObjectName(RGB_HALF(255,255,0), 1000000000 );
}
void CCharSelect::PlayDeselectedMotion(int num)
{
	if(num == -1)
		return;
	if(m_pPlayer[num] == NULL)
		return;
	// 061226 LYW --- Change motion of character.
	/*
	m_pPlayer[num]->ChangeMotion(eMotion_Sel1Back+4*num,0);
	m_pPlayer[num]->ChangeBaseMotion(eMotion_Sel1Before+4*num);
	*/
	int baseMotion = 0 ;
	int motion = 0 ;

	switch( num )
	{
	case 0 :
		{
			motion		= Motion_SelBack ;
			baseMotion	= Motion_SelBefore ;
		}
		break ;

	case 1 :
		{
			motion		= Motion_SelBack ;
			baseMotion	= Motion_SelBefore ;
		}
		break ;

	case 2 :
		{
			motion		= Motion_SelBack ;
			baseMotion	= Motion_SelBefore ;
		}
		break ;

	case 3 :
		{
			motion		= Motion_SelBack ;
			baseMotion	= Motion_SelBefore ;
		}
		break ;

	case 4 :
		{
			motion		= Motion_SelBack ;
			baseMotion	= Motion_SelBefore ;
		}
		break ;

	default :
		break ;
	}

	m_pPlayer[num]->ChangeMotion(motion,0);
	m_pPlayer[num]->ChangeBaseMotion(baseMotion);

//KES 030904
//	m_pPlayer[num]->DisPlayObjectName(RGB_HALF(255,255,255), 1000000000 );
}

void CCharSelect::DisplayNotice(int MsgNum)
{
	WINDOWMGR->MsgBox( MBI_CHARSELERROR, MBT_OK, CHATMGR->GetChatMsg(MsgNum) );
	SetDisablePick( TRUE );
}


void CCharSelect::SetDisablePick( BOOL val )
{
	// 061215 LYW --- Delete this code for a moment.
	/*
	m_bDiablePick = val;
	if( m_pSelectDlg )
		m_pSelectDlg->SetDisable( val );
		*/
	// 061212 LYW --- Add profile dialog for character information.
	/*
	if( m_pProfileDlg )
		m_pProfileDlg->SetDisable( val ) ;
		*/

	// 061215 LYW --- Disable dialog to pick.
	if( m_pProfile ) m_pProfile->SetDisable( val ) ;

	// 061219 LYW --- Disable all dialog when called this function.
	cDialog* pDlg = ( cDialog* )WINDOWMGR->GetWindowForID(CS_CHARSELECTDLG) ;
	if( pDlg ) pDlg->SetDisable( val ) ;
	
}

void CCharSelect::BackToMainTitle()
{
	MSGBASE msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_DISCONNECT_SYN;
	NETWORK->Send(&msg,sizeof(msg));

	SetBlock(TRUE);
	
	gUserID = 0;
	
	SetDisablePick(TRUE);
	m_bBackToMainTitle = TRUE;
}

void CCharSelect::OnDisconnect()
{
//	if( !m_bBlockScreen )			//backtomaintitle일때는 안찍는다.
//		CGameState::OnDisconnect();

	if( m_bBackToMainTitle )
	{
		SetBlock(FALSE);
		OBJECTMGR->RemoveAllObject();	//여기서 안지우고 release에서 지우면 maintitle화면에 잠시 보인다.
		MAINGAME->SetGameState(eGAMESTATE_TITLE);	//에이젼트가 끊나면..
		m_bBackToMainTitle = FALSE;
		NETWORK->ReleaseKey();
	}
	else
	{
		CGameState::OnDisconnect();
	}


	// do nothing
}

void CCharSelect::SendMsgGetChannelInfo()
{
	CPlayer* pPlayer = GetCurSelectedPlayer();
	if(pPlayer == NULL)
	{
		DisplayNotice(18);
		return;
	}

	MSGBASE msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHANNELINFO_SYN;
	msg.dwObjectID = pPlayer->GetID();
	
	NETWORK->Send( &msg, sizeof(msg) );
}


//=================================================================================================
//	NAME		: UserConn_RemainTime_Notify()
//	PURPOSE		: Process error message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CCharSelect::UserConn_RemainTime_Notify( void* pMsg )
{
#ifdef TAIWAN_LOCAL
	MSG_INT* pmsg = (MSG_INT*)pMsg;
	BILLINGMGR->SetUseRemainTime(TRUE);
	BILLINGMGR->SetRemainTime( pmsg->nData );	//
#endif //TAIWAN_LOCAL
}


//=================================================================================================
//	NAME		: UserConn_CharacterList_Ack()
//	PURPOSE		: Process error message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CCharSelect::UserConn_CharacterList_Ack( void* pMsg )
{
	SEND_CHARSELECT_INFO* pmsg = (SEND_CHARSELECT_INFO*)pMsg;

	memset(m_pPlayer,0,sizeof(CPlayer*)*MAX_CHARACTER_NUM);

	VECTOR3 pos;

	DWORD basiccolor = RGBA_MAKE(255, 255, 255,255);
	DWORD overcolor  = RGBA_MAKE(255,255,0,255);
	DWORD presscolor = RGBA_MAKE(255,255,0,255);

	int t = -1;
	for(int i = 0; i < pmsg->CharNum ; i++)
	{
		WORD standpoint = pmsg->StandingArrayNum[i];
		
		// YH 일단 선택창에서 무기 빼놓음.
		pmsg->ChrTotalInfo[i].WearedItemIdx[eWearedItem_Weapon] = 0;
		pmsg->ChrTotalInfo[i].MunpaID = 0;
		pmsg->ChrTotalInfo[i].MarkName = 0;
		
		m_pPlayer[standpoint] = OBJECTMGR->AddPlayer(&pmsg->BaseObjectInfo[i],NULL,&pmsg->ChrTotalInfo[i],NULL);
		CPlayer* pPlayer = m_pPlayer[standpoint];						

		pPlayer->GetEngineObject()->ApplyHeightField(FALSE);
		pPlayer->SetOverInfoOption(0);

		if (1 == pPlayer->GetGender())
		{
			pos = c_WOMAN_STAND_POS[standpoint];
		}
		else
		{
			pos = c_MAN_STAND_POS[standpoint];
		}		
							
		char LevelName[255];

		cPushupButton * btn = (cPushupButton *)WINDOWMGR->GetWindowForIDEx(MT_FIRSTCHOSEBTN+standpoint);
		wsprintf(LevelName,"[Lv%2d] %-16s",pmsg->ChrTotalInfo[i].Level,pmsg->BaseObjectInfo[i].ObjectName );
		btn->SetText(LevelName, basiccolor, overcolor, presscolor);
		cImage ToolTipImg;
		SCRIPTMGR->GetImage( 0, &ToolTipImg, PFT_HARDPATH );
		{
			btn->SetToolTip( GetMapName(pmsg->ChrTotalInfo[i].LoginMapNum), RGB_HALF(255,255,255), &ToolTipImg, TTCLR_ITEM_CANEQUIP );
		}
		

		if( standpoint == 4 )	//마지막 캐릭터의 경우 하드코딩 조정
		{
			pos.x -= ( pmsg->ChrTotalInfo[standpoint].Height - 1.0f ) * 200.f;
			pos.z += ( pmsg->ChrTotalInfo[standpoint].Height - 1.0f ) * 100.f;
		}

		pPlayer->SetPosition(&pos);						
		
		pPlayer->SetAngle(0.0f);

		pPlayer->ChangeBaseMotion(Motion_SelBefore);
		pPlayer->ChangeMotion(Motion_SelBefore, 0);
		
		pPlayer->ShowObjectName( FALSE );
		pPlayer->GetEngineObject()->EnableShadow(TRUE);

		VECTOR3 scale ;
		scale.x = 0.4f ;
		scale.y = 0.4f ;
		scale.z = 0.4f ;
		pPlayer->GetEngineObject()->SetScale(&scale) ;
		pPlayer->SetAngle(fCharAngle[standpoint]);						
		pPlayer->SetPosition( &c_MAN_STAND_POS[standpoint] ) ;

		// 070313 LYW --- CharSelect : Setting random of character for close eye.
		int whileCount = 0 ;
		m_nRandom[i] = random(0, 8) ;
		while(whileCount<6)
		{
			if( whileCount != 0 )
			{
				if( m_nRandom[i] == m_nRandom[whileCount] )
				{
					m_nRandom[i] = random(0, 8) ;
				}
				else
				{
					break ;
				}
			}
			++whileCount ;
		}
		pPlayer->GetEyeTime()->Init(m_nRandom[i]*500) ;
		pPlayer->GetEyeTime()->Start() ;
	}

	InitCameraInCharSelect();
}


//=================================================================================================
//	NAME		: UserConn_Character_Remove_Ack()
//	PURPOSE		: Process error message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CCharSelect::UserConn_Character_Remove_Ack()
{
	CPlayer* pPlayer = GetCurSelectedPlayer();
	if(!pPlayer)
	{
		ASSERTMSG(0, CHATMGR->GetChatMsg(20));
		return;
	}	

	USERINFOMGR->DeleteUserQuestInfo(pPlayer) ;
	
	cPushupButton * editboxName = (cPushupButton *)WINDOWMGR->GetWindowForIDEx(MT_FIRSTCHOSEBTN + m_CurSelectedPlayer);
	editboxName->SetText("",RGBA_MAKE(255,255,255,255),NULL,NULL);
	editboxName->SetPush(FALSE);
	
	m_pPlayer[m_CurSelectedPlayer] = NULL;
	m_CurSelectedPlayer = -1;
	OBJECTMGR->AddGarbageObject(pPlayer);

	SelectPlayer(-1);
	SetDisablePick( FALSE );

	// 080226 LYW --- Refresh profile dialog.
	CHARSELECT->GetProfileDlg()->RefreshProfile() ;
}


//=================================================================================================
//	NAME		: UserConn_Character_Remove_Nack()
//	PURPOSE		: Process error message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CCharSelect::UserConn_Character_Remove_Nack( void* pMsg )
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	if( pmsg->dwData == 4 )
		WINDOWMGR->MsgBox( MBI_DELETECHARFAIL, MBT_OK, CHATMGR->GetChatMsg(316) );
	else
		WINDOWMGR->MsgBox( MBI_DELETECHARFAIL, MBT_OK, CHATMGR->GetChatMsg(317) );

	SetDisablePick( FALSE );
}


//=================================================================================================
//	NAME		: UserConn_DisConnect_Ack()
//	PURPOSE		: Process error message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CCharSelect::UserConn_DisConnect_Ack()
{
	SetDisablePick( TRUE );
	m_bBackToMainTitle = TRUE;
}


//=================================================================================================
//	NAME		: UserConn_ChannelInfo_Ack()
//	PURPOSE		: Process error message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CCharSelect::UserConn_ChannelInfo_Ack( void* pMsg )
{
	MSG_CHANNEL_INFO * pmsg = (MSG_CHANNEL_INFO*)pMsg;
	GetChannelDialog()->SetChannelList(pmsg);
	SetDisablePick(TRUE);
	
	// 체널이 하나이면 바로 들어가게 함
	if(pmsg->Count == 1)
	{
		GetChannelDialog()->OnConnect();	
		GetChannelDialog()->SetActive(FALSE);
	}
}


//=================================================================================================
//	NAME		: UserConn_ChannelInfo_Nack()
//	PURPOSE		: Process error message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CCharSelect::UserConn_ChannelInfo_Nack( void* pMsg )
{
	MSG_BYTE* msg = (MSG_BYTE*)pMsg;
	if( msg->bData == 0 )
	{
		WINDOWMGR->MsgBox( MBI_MAPSERVER_CLOSE, MBT_OK, CHATMGR->GetChatMsg(213) );
	}
	else
	{
		WINDOWMGR->MsgBox( MBI_MAPSERVER_CLOSE, MBT_OK, CHATMGR->GetChatMsg(318) );
	}
}


//=================================================================================================
//	NAME		: Cheat_EventNotify_On()
//	PURPOSE		: Process error message.
//	DATE		: February 5 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CCharSelect::Cheat_EventNotify_On( void* pMsg )
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
void CCharSelect::Cheat_EventNotify_Off( void* pMsg )
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	NOTIFYMGR->SetEventNotify( FALSE );
	NOTIFYMGR->SetEventNotifyChanged( FALSE );
}

// desc_hseos_성별선택01
// S 성별선택 추가 added by hseos 2007.06.17
void CCharSelect::UserConn_Character_SexKind(void *pMsg)
{
	MSG_DWORD* pPacket = (MSG_DWORD*)pMsg;

	CHARMAKEMGR->SetCharSexKindFromDB(pPacket->dwData);
}
// E 성별선택 추가 added by hseos 2007.06.17

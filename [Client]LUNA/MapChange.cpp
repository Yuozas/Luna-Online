// MapChange.cpp: implementation of the CMapChange class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MapChange.h"

#include "MainGame.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cResourceManager.h"
#include "cWindowSystemFunc.h"

#include "mhMap.h"
#include "./Input/Mouse.h"
#include "./Input/Keyboard.h"
#include "PathManager.h"
#include "LoadingDlg.h"
#include "./Interface/cScriptManager.h"
#include "./Input/UserInput.h"
#include "UserInfoManager.h"
#include "ChatManager.h"

#include "GMNotifyManager.h"
#include "GuildTournamentMgr.h"
#include "SiegeWarMgr.h"

// 070121 LYW --- Include header files.
#include "./EFFECT/EffectManager.h"

// 070202 LYW --- Include header file.
#include "../[CC]Header/GameResourceManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLOBALTON(CMapChange)
CMapChange::CMapChange()
{
	m_pDlg = NULL;
	m_GameInInitKind = eGameInInitKind_Login;
	m_bThreadTerminated = FALSE;
	m_bMapLoadComplete = FALSE;
	m_bFirst = FALSE;
	hThread = NULL;
	m_MapNum = 0;

}

CMapChange::~CMapChange()
{

}

BOOL CMapChange::Init(void* pInitParam)
{
	m_MapNum = *((MAPTYPE*)pInitParam);
	
//	CreateGameLoading_m();

/*	char filename[256];
	sprintf(filename,"Data/Interface/2DImage/image/maploadingimage%02d.tga",m_MapNum);
	if(m_LoadingImage.LoadSprite(filename) == FALSE)
	{
		sprintf(filename,"Data/Interface/2DImage/image/maploadingimage00.tga");
		m_LoadingImage.LoadSprite(filename);
	}
	m_pDlg = (CLoadingDlg *)CreateGameLoading_m();*/
	CreateLoadingDlg(m_MapNum);
	WINDOWMGR->AfterInit();

	m_bThreadTerminated=FALSE;
	m_bMapLoadComplete=FALSE;
	NETWORK->SetCurState(this);
	
	m_bFirst = TRUE;

	g_UserInput.SetInputFocus( FALSE ); 	//로딩중엔 인풋 처리 안함. 무시.

//	if(m_MapNum == 44) //수련장 맵 번호 
//		USERINFOMGR->SetMapChage( FALSE );
//	else

	USERINFOMGR->SetMapChage( TRUE );		//맵이동인 경우에만 처리할것이 있음

	// 070208 LYW --- MapChange : Add function to setting position and scale of image.
	//SetPositionScale() ;
	SCRIPTMGR->GetImage( 4, &m_BackImage  );	
	m_BackPos.x = 0.0f;
	m_BackPos.y = 0.0f;

	return TRUE;
}


// 070208 LYW --- MapChange : Add function to setting position and scale of image.
void CMapChange::SetPositionScale()
{

	DISPLAY_INFO	dispInfo ;

	GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;

	switch( dispInfo.dwWidth )
	{
	case 800 :
		{
			m_BackScale.x = 800.0f;
			m_BackScale.y = 600.0f;
			m_scale.x = 0.78f ;
			m_scale.y = 0.78f ;
			m_tr.x = 0 ;
			m_tr.y = 0 ;
		}
		break ;

	case 1024 :
		{
			m_BackScale.x = 1024.0f;
			m_BackScale.y = 768.0f;
			m_scale.x = 1.0f ;
			m_scale.y = 1.0f ;
			m_tr.x = 0 ;
			m_tr.y = 0 ;
		}
		break ;

	case 1280 :
		{
			m_BackScale.x = 1280.0f;
			m_BackScale.y = 1024.0f;
			m_scale.x = 1.0f ;
			m_scale.y = 1.0f ;
			m_tr.x = (1280 - 1024) / 2 ;
			m_tr.y = (1024 - 768) / 2 ;
		}
		break ;
	}
}


void CMapChange::Release(CGameState* pNextGameState)
{
	m_LoadingImage.Release();
	WINDOWMGR->DestroyWindowAll();
//	WINDOWMGR->AddListDestroyWindow( m_pDlg );
	m_pDlg = NULL;
	RESRCMGR->ReleaseResource(28);	//28:로딩이미지레이어
}

void CMapChange::Process()
{
	EnterGame();

	// 070121 LYW --- Modified this part.
	/*
	if(m_bFirst)
		m_bFirst = FALSE;
	else
	{
		if(m_bThreadTerminated == FALSE)
		{
			// Loading...
			
			m_bThreadTerminated = TRUE;
		}
		if(m_bMapLoadComplete == FALSE)
		{
			MAP->InitMap(m_MapNum);
			PATHMGR->SetMap(MAP, m_MapNum, MAP->GetTileWidth());
//			MINIMAP->InitMiniMap(m_MapNum);
			m_bMapLoadComplete = TRUE;
		}
	}
	*/

	if(m_bFirst)
		m_bFirst = FALSE;
	else
	{
		if(m_bThreadTerminated == FALSE)
		{
//			SKILLMGR->Init();
			
			///LOGFILE("Init EffectManager");
//			EFFECTMGR->Init();
			
			m_bThreadTerminated = TRUE;
		}
		if(m_MapNum != (MAPTYPE)(-1) && m_bMapLoadComplete == FALSE && m_bThreadTerminated)
		{
			//LOGFILE("MAP->InitMap(m_MapNum);");
			MAP->InitMap(m_MapNum);

			//LOGFILE("PATHMGR->SetMap(MAP, m_MapNum,MAP->GetTileWidth());");
			PATHMGR->SetMap(MAP, m_MapNum,MAP->GetTileWidth());
//			MINIMAP->InitMiniMap(m_MapNum);
			
			m_bMapLoadComplete = TRUE;
		}
	}
	
	return;
}

void CMapChange::BeforeRender()
{
	
}

void CMapChange::AfterRender()
{
	WINDOWMGR->Render();
	
	m_BackImage.RenderSprite(&m_BackScale,NULL,0,&m_BackPos,0xffffffff);
	m_LoadingImage.RenderSprite(&m_scale,NULL,0,&m_tr,0xffffffff);

	VECTOR2 tip, sz;
	tip.x = 550;
	tip.y = 30;
	sz.x = 0.859375;
	sz.y = 0.8984375;
}

void CMapChange::NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg)
{
	switch(Category)
	{
	case MP_FRIEND:
		break;
		
	case MP_PARTY:
		break;

	case MP_CHAT:
		CHATMGR->NetworkMsgParse(Protocol, pMsg);
		break;

	case MP_MUNPA:
		break;

	case MP_HACKCHECK:
		break;
	case MP_SIGNAL:
		break;
	case MP_GTOURNAMENT:
		GTMGR->NetworkMsgParse(Protocol, pMsg);
		break;
	case MP_JACKPOT:
		break;
	case MP_SIEGEWAR:
		SIEGEMGR->NetworkMsgParse(Protocol, pMsg);
		break;

	case MP_CHEAT:
		{
			switch(Protocol) 
			{
				case MP_CHEAT_EVENTNOTIFY_ON:
				{
					MSG_EVENTNOTIFY_ON* pmsg = (MSG_EVENTNOTIFY_ON*)pMsg;

					NOTIFYMGR->SetEventNotifyStr( pmsg->strTitle, pmsg->strContext );
					NOTIFYMGR->SetEventNotify( TRUE );

//					if( GAMEIN->GetEventNotifyDialog() )
//					{
//						GAMEIN->GetEventNotifyDialog()->SetTitle( pmsg->strTitle );
//						GAMEIN->GetEventNotifyDialog()->SetContext( pmsg->strContext );
//						GAMEIN->GetEventNotifyDialog()->SetActive( TRUE );
//						NOTIFYMGR->SetEventNotifyChanged( FALSE );
//						//사운드?
//					}
//					else
//					{
						NOTIFYMGR->SetEventNotifyChanged( TRUE );
//					}

					NOTIFYMGR->ResetEventApply();
					for(int i=0; i<eEvent_Max; ++i)
					{
						if( pmsg->EventList[i] )
							NOTIFYMGR->SetEventApply( i );
					}
				}
				break;
		
				case MP_CHEAT_EVENTNOTIFY_OFF:
				{
					MSGBASE* pmsg = (MSGBASE*)pMsg;
					NOTIFYMGR->SetEventNotify( FALSE );
					NOTIFYMGR->SetEventNotifyChanged( FALSE );
//					if( GAMEIN->GetEventNotifyDialog() )
//						GAMEIN->GetEventNotifyDialog()->SetActive( FALSE );
				}	
				break;

			}
		}
		break;

	default:
		{
			char temp[256];
			sprintf(temp,"Category:%d Protocol:%d",Category,Protocol);
			ASSERTMSG(0,temp);
//			LOG(EC_UNKNOWN_PROTOCOL);
		}
	}
}

void CMapChange::EnterGame()
{
	if(m_bMapLoadComplete && m_bThreadTerminated && !m_bDisconnected )
	{
		MAINGAME->SetGameState(eGAMESTATE_GAMEIN,&m_GameInInitKind,sizeof(int));
	}
}

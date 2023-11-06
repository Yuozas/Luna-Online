// GameLoading.cpp: implementation of the CGameLoading class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameLoading.h"
#include "MainGame.h"
#include "GameIn.h"
#include "mhMap.h"
#include "cWindowSystemFunc.h"

#include "./WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cResourceManager.h"
#include "./Input/Mouse.h"
#include "./Input/Keyboard.h"
#include "PathManager.h"
#include "LoadingDlg.h"
#include "./Interface/cScriptManager.h"
#include "cMsgBox.h"
#include "./Input/UserInput.h"
#include "UserInfoManager.h"
#include "ChatManager.h"

// 070201 LYW --- Include game resorce manager.
#include "../[CC]Header/GameResourceManager.h"

#include "GMNotifyManager.h"
#include "./Effect/EffectManager.h"

extern HWND _g_hWnd;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CGameLoading)
DWORD WINAPI LoadingThread(void *p);			// 로딩 스레드
CGameLoading::CGameLoading()
{
	m_bThreadTerminated=FALSE;
	m_bMapLoadComplete=FALSE;
	m_MapNum = -1;

	m_pDlg	= NULL;


}

CGameLoading::~CGameLoading()
{

}

BOOL CGameLoading::Init(void* pInitParam)
{
	// 로그인,캐릭선택,캐릭생성에서 사용하는 멥 삭제 : 왜 여기서해?KES
//	MAP->Release();

	m_bThreadTerminated=FALSE;
	m_bMapLoadComplete=FALSE;
	NETWORK->SetCurState(this);

	GameLoadingParam * lParam = (GameLoadingParam *)pInitParam;

	MSG_WORD	msg;
	msg.Category	= MP_USERCONN;
	msg.Protocol	= MP_USERCONN_CHARACTERSELECT_SYN;
	msg.dwObjectID	= lParam->selectedCharNum;
	msg.wData		= gChannelNum;
	NETWORK->Send(&msg, sizeof(msg));
/*
	char filename[256];

	sprintf(filename,"Data/Interface/2DImage/image/maploadingimage%02d.tga",lParam->m_MapNum);
	if(m_LoadingImage.LoadSprite(filename) == FALSE)
	{
		sprintf(filename,"Data/Interface/2DImage/image/maploadingimage00.tga");
		m_LoadingImage.LoadSprite(filename);
	}

	m_pDlg = (CLoadingDlg *)CreateGameLoading_m();
*/	
	CreateInitLogo_m();
	WINDOWMGR->AfterInit();

	m_bFirst = TRUE;
	m_MapNum = -1;

	g_UserInput.SetInputFocus( FALSE );	//로딩중엔 인풋 처리 안함. 무시.

	USERINFOMGR->SetMapChage( FALSE );//맵이동이 아닌경우에 처리할것이 있음

	// 070208 LYW --- CharMake : Add function to setting position and scale of image.
	//SetPositionScale() ;
	SCRIPTMGR->GetImage( 4, &m_BackImage  );	
	m_BackPos.x = 0.0f;
	m_BackPos.y = 0.0f;

	return TRUE;
}


// 070208 LYW --- CharMake : Add function to setting position and scale of image.
void CGameLoading::SetPositionScale()
{
	m_tr.x = 0 ;
	m_tr.y = 0 ;

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

void CGameLoading::Release(CGameState* pNextGameState)
{
	m_LoadingImage.Release();
	//LOGFILE("WINDOWMGR->DestroyWindowAll();");
	WINDOWMGR->DestroyWindowAll();
//	WINDOWMGR->AddListDestroyWindow( m_pDlg );
	m_pDlg = NULL;
	//LOGFILE("RESRCMGR->ReleaseResource(28)");
	RESRCMGR->ReleaseResource(28);//로딩 이미지 레이어 번호
}

void CGameLoading::Process()
{
//	if(WAIT_OBJECT_0 == WaitForSingleObject(hThread, 0) )//||
		//WAIT_ABANDONED == WaitForSingleObject(hThread, 0) )
	{
//		CloseHandle(hThread);
//		m_bThreadTerminated = TRUE;
		EnterGame();
	}

	if(m_bFirst)
		m_bFirst = FALSE;
	else
	{
		if(m_bThreadTerminated == FALSE)
		{
//			SKILLMGR->Init();
			
			//LOGFILE("Init EffectManager");
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
void CGameLoading::BeforeRender()
{
}
void CGameLoading::AfterRender()
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

void CGameLoading::NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg)
{
	switch(Category)
	{
	case MP_USERCONN:
		{
			switch(Protocol) 
			{
			case MP_USERCONN_CHARACTERSELECT_ACK:
				{
					UserConn_CharacterSelect_Ack( pMsg ) ;
				}
				return;

			case MP_USERCONN_CHARACTERSELECT_NACK:
				{
					UserConn_CharacterSelect_Nack() ;
				}
				return;
			}
		}
		break;
	case MP_CHAT:
		CHATMGR->NetworkMsgParse(Protocol, pMsg);
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
	case MP_JACKPOT:
		break;
	}
//	LOG(EC_UNKNOWN_PROTOCOL);
}

void CGameLoading::EnterGame()
{
	if(m_bMapLoadComplete && m_bThreadTerminated && !m_bDisconnected )
	{
		//여기서 게임로딩?
//		GAMEIN->InitForGame();
		int initKind = eGameInInitKind_Login;
		MAINGAME->SetGameState(eGAMESTATE_GAMEIN,&initKind,sizeof(int));
	}
}


//=================================================================================================
//	NAME		: UserConn_CharacterSelect_Ack()
//	PURPOSE		: Process network message of loading part.
//	DATE		: February 6 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CGameLoading::UserConn_CharacterSelect_Ack( void* pMsg )
{
	MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
	m_MapNum = pmsg->bData;
}


//=================================================================================================
//	NAME		: UserConn_CharacterSelect_Nack()
//	PURPOSE		: Process network message of loading part.
//	DATE		: February 6 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CGameLoading::UserConn_CharacterSelect_Nack()
{
	LOG(EC_MAPSERVER_CLOSED);
	PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
}


//=================================================================================================
//	NAME		: Cheat_EventNotify_On()
//	PURPOSE		: Process network message of loading part.
//	DATE		: February 6 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CGameLoading::Cheat_EventNotify_On( void* pMsg )
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
//	PURPOSE		: Process network message of loading part.
//	DATE		: February 6 2007 LYW
//	ATTENTION	: 
//=================================================================================================
void CGameLoading::Cheat_EventNotify_Off( void* pMsg )
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	NOTIFYMGR->SetEventNotify( FALSE );
	NOTIFYMGR->SetEventNotifyChanged( FALSE );
}

/////////////////////// 로딩 스레드 /////////////////////////////////////
DWORD WINAPI LoadingThread(void *p)
{

	return 0;
}
//////////////////////////////////////////////////////////////////////////

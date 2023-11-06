///////////////////////////////////////
// CScreenShotDlg.cpp: implementation of the CScreenShotDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScreenShotDlg.h"
#include "WindowIDEnum.h"
#include "ObjectManager.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cStatic.h"
#include "./Interface/cScriptManager.h"
#include "gamein.h"
#include "MHTimeManager.h"
#include "GameResourceManager.h"

extern int g_nServerSetNum;
//extern char g_ServerSetName[255];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScreenShotDlg::CScreenShotDlg()
{
	m_type = WT_SCREENSHOT_DLG;
	m_sttime = NULL;
}

CScreenShotDlg::~CScreenShotDlg()
{
}


void CScreenShotDlg::Linking()
{

	SCRIPTMGR->GetImage( 138, &m_Image, PFT_HARDPATH ); //배경이미지 로드

	//캐릭터가 서버에 로그인한 시간 출력 static
	m_sttime = new cStatic; 
	if( m_sttime )
	{
		m_sttime->Init( 10, 10, 0, 0, NULL, -1 );
		m_sttime->SetShadow( TRUE );
		m_sttime->SetFontIdx( 0 );
		m_sttime->SetAlign( TXT_LEFT );
		m_sttime->SetStaticText( "" );
		
		Add( m_sttime );
	}

	//캐릭터가 플레이한 시간 출력 static
	m_playtime = new cStatic;
	if( m_playtime )
	{
		m_playtime->Init( 10, 27, 0, 0, NULL, -1 );
		m_playtime->SetShadow( TRUE );
		m_playtime->SetFontIdx( 0 );
		m_playtime->SetAlign( TXT_LEFT );
		m_playtime->SetStaticText( "" );
		
		Add( m_playtime );
	}

}


void CScreenShotDlg::SetActive( BOOL val, DWORD HeroID )
{
	cDialog::SetActive( val );

	ViewDatetime(HeroID);
}


void CScreenShotDlg::ViewDatetime(DWORD HeroID)
{

	char timetext[255];			// 시간포멧후 static에 출력할 변수
	DWORD currenttime;			// 클라이언트 접속후 현재까지 진행된 시간	
	DWORD currentdate;			// 클라이언트 접속후 현재까지 진행된 날짜
	DWORD totalcurrenttime;		// 날짜를 시간단위로 변경 (토탈시간)
	DWORD clientlogindate;		// 캐릭터 로그인 당시 클라이언트시계의 날짜
	DWORD clientlogintime;		// 캐릭터 로그인 당시 클라이언트시계의 시간
	DWORD totalclientlogintime; // 날짜를 시간단위로 변경 (토탈시간)
	

	serverti = GAMEIN->GetLoginTime();		// 캐릭터가 서버에 로그인 시간
	GAMEIN->GetClientLoginTime(clientlogindate,clientlogintime);	
	totalclientlogintime = (clientlogindate * 1000 * 60 * 60 * 24) + clientlogintime;

	currentdate	= MHTIMEMGR->GetMHDate();	
	currenttime = MHTIMEMGR->GetMHTime();
	totalcurrenttime = (currentdate * 1000 * 60 * 60 * 24) + currenttime;
		
	//서버 로그인 시간 출력

	// RaMa - 서버명 출력
	char servername[64];
	memset( servername, 0, sizeof(char)*64 );
	strncpy( servername, GAMERESRCMNGR->GetServerSetName(), 32 );

	sprintf(timetext, "[%s] %02d-%02d-%02d %02d:%02d:%02d",	servername,
		serverti.wYear, serverti.wMonth, serverti.wDay, 
		serverti.wHour, serverti.wMinute, serverti.wSecond);
	m_sttime->SetStaticText(timetext);
	
	//캐릭터 플레이 시간 출력
	sprintf(timetext, "[PLAYTIME] %2d Day %02d:%02d:%02d", 
		(totalcurrenttime-totalclientlogintime)/1000/60/60/24,
		((totalcurrenttime-totalclientlogintime)/1000/60/60)%24,
		((totalcurrenttime-totalclientlogintime)/1000/60)%60,
		((totalcurrenttime-totalclientlogintime)/1000)%60);
	m_playtime->SetStaticText(timetext);
	
	m_bShow = TRUE;

}

void CScreenShotDlg::Render()
{
	if( m_bShow )
	{
		cDialog::Render();
	}
}

void CScreenShotDlg::Setserverti(SYSTEMTIME servertime)
{
	serverti = servertime;
}

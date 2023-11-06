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

	SCRIPTMGR->GetImage( 138, &m_Image, PFT_HARDPATH ); //����̹��� �ε�

	//ĳ���Ͱ� ������ �α����� �ð� ��� static
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

	//ĳ���Ͱ� �÷����� �ð� ��� static
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

	char timetext[255];			// �ð������� static�� ����� ����
	DWORD currenttime;			// Ŭ���̾�Ʈ ������ ������� ����� �ð�	
	DWORD currentdate;			// Ŭ���̾�Ʈ ������ ������� ����� ��¥
	DWORD totalcurrenttime;		// ��¥�� �ð������� ���� (��Ż�ð�)
	DWORD clientlogindate;		// ĳ���� �α��� ��� Ŭ���̾�Ʈ�ð��� ��¥
	DWORD clientlogintime;		// ĳ���� �α��� ��� Ŭ���̾�Ʈ�ð��� �ð�
	DWORD totalclientlogintime; // ��¥�� �ð������� ���� (��Ż�ð�)
	

	serverti = GAMEIN->GetLoginTime();		// ĳ���Ͱ� ������ �α��� �ð�
	GAMEIN->GetClientLoginTime(clientlogindate,clientlogintime);	
	totalclientlogintime = (clientlogindate * 1000 * 60 * 60 * 24) + clientlogintime;

	currentdate	= MHTIMEMGR->GetMHDate();	
	currenttime = MHTIMEMGR->GetMHTime();
	totalcurrenttime = (currentdate * 1000 * 60 * 60 * 24) + currenttime;
		
	//���� �α��� �ð� ���

	// RaMa - ������ ���
	char servername[64];
	memset( servername, 0, sizeof(char)*64 );
	strncpy( servername, GAMERESRCMNGR->GetServerSetName(), 32 );

	sprintf(timetext, "[%s] %02d-%02d-%02d %02d:%02d:%02d",	servername,
		serverti.wYear, serverti.wMonth, serverti.wDay, 
		serverti.wHour, serverti.wMinute, serverti.wSecond);
	m_sttime->SetStaticText(timetext);
	
	//ĳ���� �÷��� �ð� ���
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

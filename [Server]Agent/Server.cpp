// Server.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <crtdbg.h>
#include "ServerSystem.h"
#ifdef _MAPSERVER_
#include "GameResourceManager.h"
#endif
#ifdef __AGENTSERVER__
#ifdef _HACK_SHIELD_
#include "HackShieldManager.h"
#endif
#endif

#ifdef _NPROTECT_
//#include "ggsrv25.h"
#include "NProtectManager.h"
#endif

#ifdef _NPROTECT_
GGAUTHS_API void NpLog(int mode, char* msg);
GGAUTHS_API void GGAuthUpdateCallback(PGG_UPREPORT report);
#endif

HANDLE g_hMap = NULL;
char g_szMapName[64];
void LeaveServerApplication();
BOOL EnterServerApplication();
char * GetCommandLineArg(int idx, char * szCommandLine);
char g_szHeroIDName[17] = { 0, };

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	CreateDirectory( "./Log", NULL );

#ifdef _HACK_SHIELD_
	char path[256] = {0,};
	GetCurrentDirectory(256, path);
	strcat(path, "\\CRC\\HackShield.crc");

	if( _AntiCpSvr_Initialize(path) != ERROR_SUCCESS )
	{
		MessageBox(NULL,"_AntiCpSvr_Initialize Fail",NULL,NULL);
		return 0;
	}
#endif

WORD mapnum = atoi(lpCmdLine);

#ifdef _NPROTECT_
	if( NPROTECTMGR->Init(mapnum) != ERROR_SUCCESS )
	{
		MessageBox(NULL,"GameGuard Init Fail",NULL,NULL);
		return 0;
	}
#endif

//	SEND_CHARACTER_TOTALINFO ss;
//	SEND_MONSTER_TOTALINFO mm;

	
#ifdef _MAPSERVER_
	#ifndef _MURIMNET_

	GAMERESRCMNGR->SetLoadMapNum(mapnum);
	if(mapnum == 0)
	{
		MessageBox(NULL,"Give Parameter as a Map Number",NULL,NULL);
		return 0;
	}
	wsprintf(g_szMapName, "MapServer_%d", mapnum); 
	if(!EnterServerApplication()) return 0;
	#endif
#elif _DISTRIBUTESERVER_
	wsprintf(g_szMapName, "DistributeServer_%d", mapnum); 
	if(!EnterServerApplication()) return 0;
#elif _AGENTSERVER
	wsprintf(g_szMapName, "AgentServer_%d", mapnum); 
	if(!EnterServerApplication()) return 0;
#endif

	g_pServerSystem = new CServerSystem;
	g_pServerSystem->Start(mapnum);
	g_pServerSystem->End();
	delete g_pServerSystem;
	g_pServerSystem = NULL;
	
	LeaveServerApplication();

#ifdef _HACK_SHIELD_
	_AntiCpSvr_Finalize();
#endif
#ifdef _NPROTECT_
	NPROTECTMGR->Release();
#endif
	return 0;
}

char * GetCommandLineArg(int idx, char * szCommandLine)
{
	char sep[]   = " ";
	char line[255];
//	strcpy(line, szCommandLine);
	SafeStrCpy( line, szCommandLine, 255 );
	char * token = strtok( line, sep );
	while( token != NULL && idx != 0)
	{
		token = strtok( NULL, sep );
		--idx;
	}
	return token;
}

BOOL EnterServerApplication()
{//taiyo
    g_hMap = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, 1024, g_szMapName);
    if(g_hMap != NULL && GetLastError() == ERROR_ALREADY_EXISTS) 
    { 
		CloseHandle(g_hMap); 
		g_hMap = NULL;
		return FALSE;
    }
	
	return TRUE;
}

void LeaveServerApplication()
{//taiyo
	if(g_hMap)
	{
		CloseHandle(g_hMap);
		g_hMap = NULL;
	}	
}

#ifdef _NPROTECT_
GGAUTHS_API void NpLog(int mode, char* msg)
{
	NPROTECTMGR->NpLog(mode, msg);
}

GGAUTHS_API void GGAuthUpdateCallback(PGG_UPREPORT report)
{
	NPROTECTMGR->GGAuthUpdateCallback(report);
}
#endif

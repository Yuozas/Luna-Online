// ServerSystem.cpp: implementation of the CServerSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerSystem.h"
#include "Console.h"
#include "Network.h"
#include "DataBase.h"
#include "CommonNetworkMsgParser.h"
#include "DistributeNetworkMsgParser.h"
#include "DistributeDBMsgParser.h"

#include "Usertable.h"
#include "Servertable.h"
#include <MemoryPoolTempl.h>
#include "BootManager.h"
#include "UserManager.h"
#include "MHFile.h"
#include "MHTimeManager.h"
#include "..\[CC]ServerModule\MiniDumper.h"


LPCTSTR g_SERVER_VERSION = "FORCED__USER";
extern int g_nServerSetNum;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//#define _DBTHREAD
/*
DWORD _stdcall ProcessDBMessageThread(void* p)
{
	while(1)
	{
		Sleep(5);
		
		g_DB.ProcessingDBMessage();

		if(g_pServerSystem->m_DBThreadTerminate)
			break;
	}
	return 0;
}
*/
void __stdcall ProcessDBMessage(DWORD dwEventIndex)
{
	g_DB.ProcessingDBMessage();
}

void __stdcall ReceivedMsgFromServer(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
void __stdcall ReceivedMsgFromUser(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
void __stdcall OnAcceptServer(DWORD dwConnectionIndex);
void __stdcall OnDisconnectServer(DWORD dwConnectionIndex);
void __stdcall OnAcceptUser(DWORD dwConnectionIndex);
void __stdcall OnDisconnectUser(DWORD dwConnectionIndex);

void __stdcall fConnectionCheck(DWORD dwEventIndex);
void __stdcall GameProcess(DWORD dwEventIndex);

void ButtonProc1();
void ButtonProc2();
void ButtonProc3();
void OnCommand(char* szCommand);


HWND g_hWnd;

/*
HHOOK hHook;

LRESULT WINAPI CallWndProc(int nCode, WPARAM wParam, LPARAM lParam) 
{ 
    CWPSTRUCT* lpWp = (CWPSTRUCT*)lParam;

	if(lpWp->message == WM_CLOSE)
	{
		DefWindowProc(g_hWnd, lpWp->message, lpWp->wParam, lpWp->lParam);
		PostQuitMessage(0);
	}
    
    return CallNextHookEx(hHook, nCode, wParam, lParam); 
} 
*/


// taiyo 
// �����̳��� �ӵ������ ���� ���� ���� ������~!
CServerSystem * g_pServerSystem = NULL;
CMemoryPoolTempl<USERINFO> g_UserInfoPool;

CServerSystem::CServerSystem()
{
	// servertable usertable �ʱ�ȭ ---------------------
	g_UserInfoPool.Init(3000, 500,"USERINFO");
	g_pUserTable = new CUserTable;
	g_pUserTable->Init(3000/3);
	g_pServerTable = new CServerTable;
	g_pServerTable->Init(50);

	CoInitialize(NULL);

	m_bUserFireWall = FALSE;
	ZeroMemory( m_strFireWallIP, sizeof(m_strFireWallIP) );
	
	m_Nation = eNATION_KOREA;
}

CServerSystem::~CServerSystem()
{
	CoUninitialize();
}

extern DWORD g_dwMaxUser;

void CServerSystem::Start(WORD ServerNum)
{	
//AO��A ��IAA
	SetNation();

	FILE* fpMC = fopen( "MaxUserCount.txt", "r" );
	if(fpMC)
	{
		fscanf( fpMC, "%d", &g_dwMaxUser );
		fclose( fpMC );
	}

	BOOTMNGR->AddSelfBootList(DISTRIBUTE_SERVER, ServerNum, g_pServerTable);
	gUserMGR.LoadDistributeDesc();	//�̰� �б����ؼ� �������� �ʿ��ѰŶ�.

//	m_IdxCreater = ICCreate();
//	ICInitialize(m_IdxCreater,5000);
	
	//////////////////////////////////////////////////////////////////////////
	// �ܼ� �ʱ�ȭ
	// ��ư ����.�ܺ��Է¿� ��ư�� 28�� �غ�Ǿ��ִ�.�Ѱ��� ������� ���� ��� pCustomButton �ʵ带 NULL�� ä���.
	
	MENU_CUSTOM_INFO pMenu[3];
	strcpy(pMenu[0].szMenuName, "Button1");
	pMenu[0].cbMenuCommand = ButtonProc1;
	strcpy(pMenu[1].szMenuName, "Button2");
	pMenu[1].cbMenuCommand = ButtonProc2;
	strcpy(pMenu[2].szMenuName, "Button3");
	pMenu[2].cbMenuCommand = ButtonProc3;

	if(!g_Console.Init(3,pMenu,OnCommand))
		MessageBox(NULL,"Fail initialize Console",0,0);

	char windowTitleMsg[64];
	sprintf(windowTitleMsg,"Distribute Server_%d", ServerNum);

	g_hWnd = GetActiveWindow();
	SetWindowText(g_hWnd, windowTitleMsg);


//	DWORD    dwProcessID = NULL;
//	DWORD    dwTreadID = ::GetWindowThreadProcessId(g_hWnd, &dwProcessID );

//	HOOKPROC hProc;
//    hProc = CallWndProc;
//    hHook = ::SetWindowsHookEx(WH_CALLWNDPROC, hProc, (HINSTANCE)NULL, dwTreadID);


	//////////////////////////////////////////////////////////////////////////
	// ��Ʈ��ũ ����

	/// ��Ʈ��ũ �ļ� ����-------------------------------
	g_pServerMsgParser[0] = NULL;

	for(int i=1 ; i<MP_MAX ; ++i)
	{
		g_pServerMsgParser[i] = ErrorMsgParser;
	}

	for(i=1 ; i<MP_MAX ; ++i)
	{
		g_pUserMsgParser[i] = ErrorMsgParser;
	}

	g_pServerMsgParser[MP_SERVER]	= MP_DISTRIBUTESERVERMsgParser;
	g_pServerMsgParser[MP_POWERUP]	= MP_POWERUPMsgParser;
	g_pServerMsgParser[MP_USERCONN] = MP_USERCONNMsgParser;
	// 080408 LYW --- ServerSystem : Add a network message parser for chatroom system.
	g_pServerMsgParser[MP_CHATROOM]	= MP_CHATROOMMsgParser ;
	
	g_pServerMsgParser[MP_MORNITORMAPSERVER] = MP_MonitorMsgParser;

	g_pUserMsgParser[MP_USERCONN]	= MP_USERCONNMsgParser;
	g_pUserMsgParser[MP_AUTOPATCH]	= MP_DISTRIBUTEAUTOPATCHMsgParser;

	// ServerList �о���� ------------------------

	//g_pListCollect = new CListCollect;
	//g_pListCollect->LoadServerInfo();
	//g_pListCollect->LoadDistributeNum();

	/*
	CUSTOM_EVENT ev[3];
	ev[0].dwPeriodicTime	= 100;
	ev[0].pEventFunc		= GameProcess;
	//---KES DB Process Fix 071114
	ev[1].dwPeriodicTime	= 1000 * 30;
	ev[1].pEventFunc		= fConnectionCheck;
//	ev[1].dwPeriodicTime	= 100;
	//----------------------------
//	ev[1].pEventFunc		= ProcessDBMessage;
//	ev[2].dwPeriodicTime	= 1000 * 30;
//	ev[2].pEventFunc		= fConnectionCheck;
	*/

	CUSTOM_EVENT ev[ 1 ] = { 0 };
	{
		ev[ 0 ].dwPeriodicTime	= 10;
		ev[ 0 ].pEventFunc		= GameProcess;
	}
		
	DESC_NETWORK desc;
	desc.OnAcceptServer = OnAcceptServer;
	desc.OnDisconnectServer = OnDisconnectServer;
	desc.OnAcceptUser = OnAcceptUser;
	desc.OnDisconnectUser = OnDisconnectUser;
	desc.OnRecvFromServerTCP = ReceivedMsgFromServer;
	desc.OnRecvFromUserTCP = ReceivedMsgFromUser;

	desc.dwCustomDefineEventNum	= sizeof( ev ) / sizeof( *ev );
	//desc.dwCustomDefineEventNum = 2;
	desc.pEvent = ev;
	desc.dwMainMsgQueMaxBufferSize = 20480000;	//
	desc.dwMaxServerNum = 100;
	desc.dwMaxUserNum = 3000;
	desc.dwServerBufferSizePerConnection = 512000;
	desc.dwServerMaxTransferSize = 65535;
	desc.dwUserBufferSizePerConnection = 128000;
	desc.dwUserMaxTransferSize = 65535;
	desc.dwConnectNumAtSameTime = 3000;
	desc.dwFlag = 0;//NETDDSC_DEBUG_LOG;

	g_Network.Init(&desc);
	//g_Network.Start();
	g_Console.LOG(4, "��") ;
	g_Console.LOG(4, "[ Server Version : %s ]", g_SERVER_VERSION) ;
	g_Console.LOG(4, "��") ;
	// Booting
//	BOOTMNGR->AddSelfBootList(DISTRIBUTE_SERVER, ServerNum, g_pServerTable);//���� �Űܳ���--;
	if(!BOOTMNGR->StartServer(&g_Network, g_pServerTable->GetSelfServer()))
	{
		MessageBox(NULL,"Server Initialize Failed (StartServer)",NULL,NULL);
		return;
	}

	BOOTMNGR->AddBootListINI(DISTRIBUTE_SERVER, 0, g_pServerTable);

	if(!BOOTMNGR->ConnectToMS(&g_Network, g_pServerTable))
	{
		SERVERINFO info;
		info.wServerKind = MONITOR_SERVER;
		OnConnectServerFail(&info);		
	}

	//////////////////////////////////////////////////////////////////////////
	// DB ����
	DWORD maxthread = 2;
	DWORD maxqueryinsametime = 1024;
	FILE* fp = fopen("DistributeDBInfo.txt","r");
	if(fp)
	{
		fscanf(fp,"%d %d",&maxthread,&maxqueryinsametime);
		fclose(fp);
	}
	if(g_DB.Init(maxthread,maxqueryinsametime,TRUE) == FALSE)
		MessageBox(NULL,"DataBase Initializing Failed",0,0);

	m_DBThreadTerminate = FALSE;
#ifdef _DBTHREAD
	CreateThread(0,0,ProcessDBMessageThread,0,0,&m_DBThreadID);
#endif
	
	
	// �α��� üũ ���̺� �ʱ�ȭ
	LoginCheckInit();
	
//	LoadFireWallIP();

	// Dump
	//MiniDumper md(MiniDumper::DUMP_LEVEL_2);
	MiniDumper md(MiniDumper::DUMP_LEVEL_0);

	// 100226 pdy ��ũ��Ʈäũ�� ������ Ű���� �ѹ��� �е��� ����
	SetScriptCheckValue( m_ScriptCheckValue );//disable script check


	g_Console.LOG(4, "-----------   DISTRIBUTE SERVER START  -----------------");
	// �� �κ��� �������� �޽��� ����.CONSOLE�� ����ϵ� ������ �޽��� ������� ¥�� ������ε� �̺�Ʈ�� ����ؾ���
	// �ϹǷ� ������ �޽��� ���� �ۼ��� �ϰ� ���� �ʴٸ� �� �޼ҵ带 ����Ѵ�.�� ��� ���� �޽��� ������ ������ ����� 
	// �����Ѵ�.�� �޼ҵ�� I4DyuchiCONSOLE ���̾�α��� x��ư�� ���������� �������� �ʴ´�.�� �޼ҵ尡 �����Ѵٸ� ��
	// �α׷��� �����ϴ� ���̴�.
	
	g_bReady = TRUE;

	g_Console.WaitMessage();
}

void CServerSystem::LoadFireWallIP()
{
	CMHFile file;
	if( !file.Init( "firewallip.txt", "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
		return;

	while(1)
	{
		if( file.IsEOF() )
			break;

		if( file.GetInt() == g_nServerSetNum )
		{
			SafeStrCpy( m_strFireWallIP, file.GetString(), 16 );
			if( *m_strFireWallIP != 0 )
				m_bUserFireWall = TRUE;

			break;
		}
	}

	file.Release();	
}

void CServerSystem::ConnectionCheck()
{	// YH ���� 30�ʸ��� �ѹ��� ����
	DWORD _60sec = 60*1000;
	USERINFO* pInfo;
	DWORD elapsedtime;

	if(g_bReady == FALSE)
		return;
	
	cPtrList removelist;

	g_pUserTable->SetPositionHead();
	while(pInfo = g_pUserTable->GetData())
	{
		if(pInfo->dwConnectionIndex != 0)
		{
			// ���� ������ ����� �̷����� ���� ���
			elapsedtime = gCurTime - pInfo->dwLastConnectionCheckTime;
			if( elapsedtime > _60sec * 2 )
			{
				if(pInfo->bConnectionCheckFailed)
				{
					//!!�׽�Ʈ �� 30�� ���� ������ �ٲٰ� �����~!
					//pInfo->dwLastConnectionCheckTime += 1000 * 10;	//OnDisconnectUser( ���� �ޱ� ���� �ݺ� ����.
					removelist.AddTail(pInfo);
					continue;
				}
				else
				{
					pInfo->bConnectionCheckFailed = TRUE;
					SendConnectionCheckMsg(pInfo);
					pInfo->dwLastConnectionCheckTime = gCurTime;
				}
			}
		}
	}

	PTRLISTPOS pos = removelist.GetHeadPosition();
	while( pos )
	{
		USERINFO* p = (USERINFO*)removelist.GetNext(pos);
		ASSERT(p->dwConnectionIndex);
//		LoginCheckDelete(p->dwUserID);		// �α���üũ ���̺��� ����
//		g_pServerSystem->ReleaseAuthKey(p->dwUniqueConnectIdx);
//		g_pUserTable->RemoveUser(p->dwConnectionIndex);
//		memset(p,0,sizeof(USERINFO));
//		g_UserInfoPool.Free(p);
		DisconnectUser( p->dwConnectionIndex );
	}
	removelist.RemoveAll();
}

void CServerSystem::SendConnectionCheckMsg(USERINFO* pInfo)
{
	MSGBASE msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CONNECTION_CHECK;
	g_Network.Send2User(pInfo->dwConnectionIndex,(char*)&msg,sizeof(msg));
}

void CServerSystem::Process()
{
	MHTIMEMGR_OBJ->Process();

	//---KES DB Process Fix 071114
	//g_DB.ProcessingQueue();
	//----------------------------
}


void CServerSystem::End()
{
//	UnhookWindowsHookEx(hHook);
	m_DBThreadTerminate = TRUE;
//	ICRelease(m_IdxCreater);
	
	g_Network.Release();

	g_DB.Release();

	g_Console.Release();

//	if(g_pListCollect)
//	{
//		delete g_pListCollect;
//		g_pListCollect = NULL;
//	}
	if(g_pUserTable)
	{
		delete g_pUserTable;
		g_pUserTable = NULL;
	}
	if(g_pServerTable)
	{
		delete g_pServerTable;
		g_pServerTable = NULL;
	}
	CoFreeUnusedLibraries();
}

DWORD CServerSystem::MakeAuthKey()
{
	//return ICAllocIndex(m_IdxCreater) + 1;
	static DWORD ID = 1;//GetTickCount()*GetTickCount();
	if(ID == 0)
		++ID;
	return ID++;
}
void CServerSystem::ReleaseAuthKey(DWORD key)
{
	//ICFreeIndex(m_IdxCreater,key-1);
}

void CServerSystem::SetNation()
{
	CMHFile file;
	if( !file.Init( "LocalizingInfo.txt", "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
		return;

	if( strcmp( file.GetString(), "*NATION" ) == 0 )
	{
		if( strcmp( file.GetString(), "CHINA" ) == 0 )
		{
			m_Nation = eNATION_CHINA;
		}
	}

	file.Release();
}


// global function

void __stdcall OnConnectServerSuccess(DWORD dwConnectionIndex, void* pVoid)
{
	SERVERINFO* info = (SERVERINFO*)pVoid;
	info->dwConnectionIndex = dwConnectionIndex;
	if(info->wServerKind == MONITOR_SERVER)
	{
		BOOTMNGR->NotifyBootUpToMS(&g_Network);
		g_Console.LOG(4, "Connected to the MS : %s, %d, (%d)", info->szIPForServer, info->wPortForServer, dwConnectionIndex);
	}
	else
	{
		BOOTMNGR->SendConnectSynMsg(&g_Network, dwConnectionIndex, g_pServerTable);
		g_Console.LOG(4, "Connected to the Server : %s, %d, (%d)", info->szIPForServer, info->wPortForServer, dwConnectionIndex);
	}

/*
	SERVERINFO* info = (SERVERINFO*)pVoid;
	if(!dwConnectionIndex) return;

	info->dwConnectionIndex = dwConnectionIndex;					// �̰� ����� �����ϳ�? üũ
	MSG_WORD msg;
	msg.Category = MP_SERVER;
	msg.Protocol = MP_SERVER_PWRUP;
	msg.wData = g_pServerTable->GetSelfServer()->wPortForServer;
	g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));*/

	//if(g_pServerTable->m_MaxServerConnectionIndex < dwIndex)			//??????
			//g_pServerTable->m_MaxServerConnectionIndex = dwIndex;	
}

void __stdcall OnDisconnectServer(DWORD dwConnectionIndex)
{
	g_Console.LOG(4, "Server Disconnected : ConnectionIndex %d", dwConnectionIndex);//pjslocal

	// �������� ����
	SERVERINFO * delInfo = g_pServerTable->RemoveServer(dwConnectionIndex);
	if(!delInfo)
	{
		//BootManager�۵��� �Ϸ����� �ʾ��� �� ������ ���� ��� �߻�,(�ùٸ� ����)
		ASSERT(0);
		return;
	}
	delete delInfo;
}

void __stdcall OnConnectServerFail(void* pVoid)
{	
	SERVERINFO* info = (SERVERINFO*)pVoid;
	if(info->wServerKind == MONITOR_SERVER)
	{
//		BOOTMNGR->AddBootListINI(DISTRIBUTE_SERVER, 0, g_pServerTable);
		BOOTMNGR->BactchConnectToMap(&g_Network, g_pServerTable);
		g_Console.LOG(4, "Failed to Connect to the MS : %s, %d", info->szIPForServer, info->wPortForServer);
	}
	else
	{
		//not process
		g_Console.LOG(4, "Failed to Connect to the Server : %s, %d", info->szIPForServer, info->wPortForServer);
		BOOTMNGR->RemoveBootList(g_pServerTable, info->wPortForServer);
	}	
}
void __stdcall OnAcceptServer(DWORD dwConnectionIndex)
{// not used
	char strr[255];
	wsprintf(strr, "%d �� connectionIndex ���� ���� ", dwConnectionIndex);//pjslocal

	g_Console.LOG(4, strr);
}

void __stdcall OnAcceptUser(DWORD dwConnectionIndex)
{	
	if(g_bReady == FALSE)
	{
		// �ʱ�ȭ�� ������ �ȵƴµ� ���°��.
		MSGBASE send;
		send.Category = MP_USERCONN;
		send.Protocol = MP_USERCONN_SERVER_NOTREADY;
		send.dwObjectID = 0;
		g_Network.Send2User(dwConnectionIndex, (char *)&send, sizeof(send));

		DisconnectUser(dwConnectionIndex);
		return;
	}

	USERINFO* pPreInfo = g_pUserTable->FindUser( dwConnectionIndex );
	if( pPreInfo )
	{
		g_pUserTable->RemoveUser( dwConnectionIndex );
		memset(pPreInfo, 0, sizeof(USERINFO));
		g_UserInfoPool.Free( pPreInfo );
	}

	DWORD authkey = g_pServerSystem->MakeAuthKey();
	USERINFO * info = g_UserInfoPool.Alloc();
	memset(info, 0, sizeof(USERINFO));
	info->dwConnectionIndex = dwConnectionIndex;
	info->dwUniqueConnectIdx = authkey;
	info->dwLastConnectionCheckTime = gCurTime;	//SW051107 �߰�
	g_pUserTable->AddUser(info,dwConnectionIndex);

//---KES Distribute Encrypt 071003
/*
	MSGBASE send;
	send.Category = MP_USERCONN;
	send.Protocol = MP_USERCONN_DIST_CONNECTSUCCESS;
	send.dwObjectID = authkey;
	g_Network.Send2User(dwConnectionIndex, (char *)&send, sizeof(send));
*/
	MSG_DIST_CONNECTSUCCESS send;
	send.Category = MP_USERCONN;
	send.Protocol = MP_USERCONN_DIST_CONNECTSUCCESS;
	send.dwObjectID = authkey;

	info->crypto.Create();	// key ����
	send.eninit = *info->crypto.GetEnKey();
	send.deinit = *info->crypto.GetDeKey();

	g_Network.Send2User(dwConnectionIndex, (char *)&send, sizeof(send));

	info->crypto.SetInit( TRUE );		// init on	
//--------------------------------

//	g_Console.Log(eLogDisplay,4, "OnAcceptUser : Client Connected - Idx:%d, AuthKey:%d, Total(%d)",dwConnectionIndex,authkey, g_pUserTable->GetUserCount());
//	g_Console.Log(eLogFile,4, "OnAcceptUser : Client Connected - Idx:%d, AuthKey:%d, Total(%d)",dwConnectionIndex,authkey, g_pUserTable->GetUserCount());
}

void __stdcall OnDisconnectUser(DWORD dwConnectionIndex)
{
	g_pUserTable->OnDisconnectUser(dwConnectionIndex);
//	g_Console.Log(eLogDisplay,4, "OnAcceptUser : Client Disconnected - Idx:%d, Total(%d)",dwConnectionIndex, g_pUserTable->GetUserCount());
//	g_Console.Log(eLogFile,4, "OnAcceptUser : Client Disconnected - Idx:%d, Total(%d)",dwConnectionIndex, g_pUserTable->GetUserCount());
}

void __stdcall ReceivedMsgFromServer(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	MSGROOT* pTempMsg = reinterpret_cast<MSGROOT*>(pMsg);
	
	if( g_pServerMsgParser[pTempMsg->Category] == NULL ||
		pTempMsg->Category >= MP_MAX ||
		pTempMsg->Category == 0)
		return;

	g_pServerMsgParser[pTempMsg->Category](dwConnectionIndex, pMsg, dwLength);
}

void __stdcall ReceivedMsgFromUser(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	MSGROOT* pTempMsg = reinterpret_cast<MSGROOT*>(pMsg);
//	ServerTraffic * Msg = reinterpret_cast<ServerTraffic*>(pMsg);
	ASSERT(g_pUserMsgParser[pTempMsg->Category]);

//---KES Distribute Encrypt 071003
	USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);

	if( pInfo )
	{
		if( pInfo->cbCheckSum == 0 )
		{
			pInfo->cbCheckSum = pTempMsg->CheckSum;
		}
		else if( pInfo->cbCheckSum != pTempMsg->CheckSum )
		{
			OnDisconnectUser( dwConnectionIndex );
			DisconnectUser( dwConnectionIndex );
			return;
		}

		++pInfo->cbCheckSum;

		int headerSize = sizeof( MSGROOT );

		if( !pInfo->crypto.Decrypt( ( char * )pTempMsg +  headerSize, dwLength - headerSize ) )
		{
			ASSERTMSG(0,"Decrypt Error");
			OnDisconnectUser( dwConnectionIndex );
			DisconnectUser( dwConnectionIndex );
			return;
		}
		char aaa = pInfo->crypto.GetDeCRCConvertChar();
		if( pTempMsg->Code !=  aaa )
		{
			ASSERTMSG(0,"Decrypt CRC Error");
			OnDisconnectUser( dwConnectionIndex );
			DisconnectUser( dwConnectionIndex );
			return;
		}
	}

//--------------------------------

	if( g_pUserMsgParser[pTempMsg->Category] == NULL ||
		pTempMsg->Category >= MP_MAX ||
		pTempMsg->Category == 0)
		return;

	g_pUserMsgParser[pTempMsg->Category](dwConnectionIndex, pMsg, dwLength);
}

//SW051107 �߰�
void __stdcall fConnectionCheck()
{
	g_pServerSystem->ConnectionCheck();
}

void __stdcall GameProcess(DWORD dwEventIndex)
{
	try
	{
		g_DB.ProcessingDBMessage();

		// 071226 LUJ, ���� ���μ���
		{
			const DWORD		space			= 100;
			const DWORD		tick			= GetTickCount();
			static DWORD	lastCheckedTick = tick;
			static DWORD	nextCheckTick	= tick + space;;

			if( lastCheckedTick > nextCheckTick && tick < lastCheckedTick && tick > nextCheckTick ||	// �����÷�
				lastCheckedTick < nextCheckTick && nextCheckTick < tick )								// �Ϲ�
			{
				g_pServerSystem->Process();

				lastCheckedTick	= tick;
				nextCheckTick	= tick + space;
			}
		}

		// 071226 LUJ, ���� üũ
		{
			const DWORD		space			= 1000 * 30;
			const DWORD		tick			= GetTickCount();
			static DWORD	lastCheckedTick = tick;
			static DWORD	nextCheckTick	= tick + space;;

			if( lastCheckedTick > nextCheckTick && tick < lastCheckedTick && tick > nextCheckTick ||	// �����÷�
				lastCheckedTick < nextCheckTick && nextCheckTick < tick )								// �Ϲ�
			{
				fConnectionCheck();

				lastCheckedTick	= tick;
				nextCheckTick	= tick + space;
			}
		}
	}
	catch ( ... )
	{
	}	
}

// console interface function
void ButtonProc1()
{
	//SendMessage(g_hWnd, WM_CLOSE, 0, 0);
}

void ButtonProc2()
{
	g_Console.LOG(eLogDisplay, "UserCount : %d", g_pUserTable->GetUserCount());
}

void ButtonProc3()
{

}

void OnCommand(char* szCommand)
{

}

BOOL CServerSystem::IsInvalidCharInclude( char* pStr )
{
	while( *pStr )
	{
		BOOL bOk = FALSE;

		if( IsDBCSLeadByte( *pStr ) )
		{
			++pStr;
		}
		else
		{
			//����
			if( ( *pStr >= 'A' && *pStr <= 'Z' ) || ( *pStr >= 'a' && *pStr <= 'z' ) )
				bOk = TRUE;
			//����
			else if( *pStr >= '0' && *pStr <= '9' )
				bOk = TRUE;
		}

		++pStr;

		if( bOk == FALSE )
		{
			return TRUE;
		}
	}

	return FALSE;
}


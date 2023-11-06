// ServerSystem.cpp: implementation of the CServerSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerSystem.h"
#include "Network.h"
#include "CommonNetworkMsgParser.h"
#include "MapNetworkMsgParser.h"

#include "Usertable.h"
#include "Servertable.h"

#include "ObjectFactory.h"
#include "UserTable.h"
#include "GridSystem.h"
#include "MapDBMsgParser.h"
#include "CharMove.h"
#include "Player.h"
#include "Monster.h"
#include "BossMonster.h"
#include "BossMonsterManager.h"
#include "BossRewardsManager.h"
#include "Npc.h"
#include "Object.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "MHTimeManager.h"
#include "ItemManager.h"
#include "TileManager.h"
#include "RegenNPC.h"
#include "MHFile.h"
#include "CharacterCalcManager.h"
#include "PartyManager.h"
#include "ObjectStateManager.h"
#include "ItemDrop.h"
#include "MapDBMsgParser.h"
#include "AISystem.h"
#include "BattleSystem_Server.h"
#include "ChannelSystem.h"

//#include "Respwan.h"
#include "KyungGongManager.h"

//#include "ExchangeManager.h"
#include "StreetSTallManager.h "
#include "StorageManager.h"
#include "FriendManager.h"
#include "./Condition/ConditionManager.h"
#include "FameManager.h"
#include "BootManager.h"
#include "PathManager.h"
/*어빌리티 삭제 - 이영준 #include "AbilityManager.h"
#include "AbilityUpdater_CheckLevelUp.h"
#include "AbilityUpdater_DataBase.h"
#include "AbilityUpdater_ApplyData.h"*/
#include "RegenPrototype.h"
#include "RegenManager.h"
#include "ShowdownManager.h"
#include "PKManager.h"
#include "LootingManager.h"

//#include "DissolutionManager.h"
#include "ChangeItemMgr.h"
#include "EventMapMgr.h"
#include "GuildFieldWarMgr.h"
#include "PartyWarMgr.h"

#include "MemoryChecker.h"
#include "GuildTournamentMgr.h"
#include "SiegeWarMgr.h"
//SW_TEST
#include "cJackpotManager.h"
// jsd quest
#include "QuestManager.h"
#include "..\[CC]Quest\QuestEvent.h"
#include "QuestRegenMgr.h"
#include "QuestMapMgr.h"

#include "StatisticsManager.h"
#include "GuildManager.h"

#include "cMonsterSpeechManager.h"
//#include "RarenessManager.h"
#include "SiegeWarProfitMgr.h"
#include "SiegeWarMgr.h"
#include "MapObject.h"
#include "..\[CC]ServerModule\MiniDumper.h"

// Weather System
#include "WeatherManager.h"

// PetManager

// 필드보스 - 05.12 이영준
#include "FieldBossMonsterManager.h"
#include "FieldBossMonster.h"
#include "FieldSubMonster.h"

#include "../[cc]skill/server/manager/skillmanager.h"
#include "../[cc]skill/server/object/skillobject.h"

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.04.11
#include "../hseos/SHMain.h"
// E 농장시스템 추가 added by hseos 2007.04.11

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2008.01.22
#include "../hseos/Farm/SHFarmManager.h"
// E 농장시스템 추가 added by hseos 2008.01.22

// desc_hseos_몬스터미터01
// S 몬스터미터 추가 added by hseos 2007.07.09
#include "../hseos/Monstermeter/SHMonstermeterManager.h"
// E 몬스터미터 추가 added by hseos 2007.07.09

// desc_hseos_예외처리01
// S 예외처리 추가 added by hseos 2007.07.21
#include "../hseos/Debug/SHDebug.h"
// E 예외처리 추가 added by hseos 2007.07.21

// desc_hseos_데이트 존_01
// S 데이트 존 추가 added by hseos 2007.11.26
#include "../hseos/Date/SHDateManager.h"
// E 데이트 존 추가 added by hseos 2007.11.26\

//---KES AUTONOTE
#include "AutoNoteManager.h"
//#include "PunishManager.h"
//---------------
#include "FishingManager.h"

#include "Pet.h"
#include "PetManager.h"

#ifdef _DEBUG
#include <mmsystem.h>
#endif
LPCTSTR g_SERVER_VERSION = "FORCED__USER";
// 080813 LUJ, 수정된 inetwork 호출 형식에 따라 변경
void __stdcall ProcessDBMessage(DWORD dwEventIndex);
void __stdcall ReceivedMsgFromServer(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
void __stdcall ReceivedMsgFromUser(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
void __stdcall OnAcceptServer(DWORD dwConnectionIndex);
void __stdcall OnDisconnectServer(DWORD dwConnectionIndex);
void __stdcall OnAcceptUser(DWORD dwConnectionIndex);
void __stdcall OnDisconnectUser(DWORD dwConnectionIndex);

void GameProcess();

void ButtonProc1();
void ButtonProc2();
void ButtonProc3();
void OnCommand(char* szCommand);


BOOL g_bCloseWindow = FALSE;
HWND g_hWnd;

int	g_nHackCheckNum = 15;
int	g_nHackCheckWriteNum = 15;

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

// 071218 LUJ, 3번 버튼 선택 시 프로세스 타임을 표시하기 위한 용도
struct ProcessTime
{
	DWORD	mCurrentTick;
	DWORD	mPreviousTick;
	DWORD	mMaxSpace;
	float	mAverageSpace;
	DWORD	mSpace;
	DWORD	mTotalSpace;
	DWORD	mCount;
}
processTime;


// taiyo 
CServerSystem * g_pServerSystem = NULL;

CServerSystem::CServerSystem()
{
	CoInitialize(NULL);
	
	g_pUserTable = new CUserTable;
	g_pUserTable->Init(2000);
	g_pServerTable = new CServerTable;
	g_pServerTable->Init(50);
	g_pObjectFactory = new CObjectFactory;
	g_pObjectFactory->Init();
	m_pGridSystem = new CGridSystem;
	m_pGridSystem->Init();

	m_Nation = eNATION_KOREA;

	m_mpWaitExitPlayer = new CMemoryPoolTempl<WAIT_EXIT_PLAYER_INFO>;
	m_mpWaitExitPlayer->Init( 100, 10, "WAIT_EXIT_PLAYER_INFO" );
	
	m_dwQuestTime = 0;
	m_bQuestTime = FALSE;

	m_bCompletionChrUpdate = FALSE;

//-- for test
	m_dwMainProcessTime = 0;

	// 070809 웅주, 길드 정보를 모두 읽기 전까지 이 값이 FALSE로 되어있도록 되어 있으나.. 
	//				앞으로 길드 정보는 필요한 시점에만 로드할 것이다. 따라서 이 값은 초기화 후 참이 된다.
	//				길드 정보의 지연 로딩 작업이 모두 끝나면 이 플래그 자체가 필요없다.
	m_start	= TRUE;

	m_bTestServer = FALSE;
}

CServerSystem::~CServerSystem()
{
	CoUninitialize();

	PTRLISTPOS pos = m_listWaitExitPlayer.GetHeadPosition();
	WAIT_EXIT_PLAYER_INFO* pInfo = NULL;
	while( pos )
	{
		WAIT_EXIT_PLAYER_INFO* pInfo = (WAIT_EXIT_PLAYER_INFO*)m_listWaitExitPlayer.GetNext(pos);
		m_mpWaitExitPlayer->Free( pInfo );
	}
	m_listWaitExitPlayer.RemoveAll();

	SAFE_DELETE( m_mpWaitExitPlayer );

	delete g_pUserTable;
	delete g_pServerTable;
	delete g_pObjectFactory;
	delete m_pGridSystem;
}

void CServerSystem::Start(WORD ServerNum)
{	
	m_wMapNum = ServerNum;
//AO½A ·IAA
	SetNation();



	BOOTMNGR->AddSelfBootList(MAP_SERVER, ServerNum, g_pServerTable);

	//////////////////////////////////////////////////////////////////////////
	// AU¨uO AE¡¾aE¡ⓒ
	// ⓒoo¨¡¡Æ ¨uⓒød.￠?U¨￢IAO¡¤A￠?e ⓒoo¨¡¡ÆAI 28¡Æⓒø A¨ª¨￢n￥iC¨uiAO￠￥U.CN¡Æⓒø￥i￥i ¡ic￠?eCIAo ¨uE; ¡Æⓒ¡￠?i pCustomButton CE￥ia￠￢| NULL¡¤I A￠´￠?i￠￥U.
	MENU_CUSTOM_INFO pMenu[3];
	strcpy(pMenu[0].szMenuName, "Assert MsgBox");
	pMenu[0].cbMenuCommand = ButtonProc1;
	strcpy(pMenu[1].szMenuName, "Load HackCheck");
	pMenu[1].cbMenuCommand = ButtonProc2;
	//strcpy(pMenu[2].szMenuName, "Kill AllMonster");
	strcpy(pMenu[2].szMenuName, "Put time");
	pMenu[2].cbMenuCommand = ButtonProc3;

	if(!g_Console.Init(3,pMenu,OnCommand))
		MessageBox(NULL,"Console initializing Failed",0,0);//pjslocal

	g_hWnd = GetActiveWindow();

	char TitleText[128];
	sprintf(TitleText, "MAP%d(%s)", ServerNum, GetMapName(ServerNum) );
	SetWindowText(g_hWnd, TitleText);
	
	//DWORD    dwProcessID = NULL;
    //DWORD    dwTreadID = ::GetWindowThreadProcessId(g_hWnd, &dwProcessID );

	// load hackcheck
	LoadHackCheck();

	FILE* fpstart = fopen("serverStart.txt","w");
#define STARTLOG(a)	fprintf(fpstart,#a);	a;
//	STARTLOG(CONDITIONMGR->Initial());
//	STARTLOG(CONDITIONMGR->LoadConditionList());
	STARTLOG(SKILLMGR->Init());

	STARTLOG(GAMERESRCMNGR->LoadMapChangeArea(GAMERESRCMNGR->m_MapChangeArea));
	STARTLOG(GAMERESRCMNGR->LoadLoginPoint(GAMERESRCMNGR->m_LoginPoint));
	STARTLOG(GAMERESRCMNGR->LoadMonsterList());
#ifdef _TESTCLIENT_
	STARTLOG(GAMERESRCMNGR->LoadSMonsterList());
#endif
	STARTLOG(BOSSMONMGR->LoadSummonInfoList());
	STARTLOG(BOSSMONMGR->LoadBossMonsterInfoList());
	STARTLOG(BOSSREWARDSMGR->LoadBossRewardsInfo());
	
	// 필드보스 - 05.12 이영준
	STARTLOG(FIELDBOSSMONMGR->Init());

	STARTLOG(GAMERESRCMNGR->LoadNpcList(GAMERESRCMNGR->m_NpcList));
	STARTLOG(GAMERESRCMNGR->LoadNpcRegen(REGENNPC_OBJ->m_NpcRegen));
	STARTLOG(GAMERESRCMNGR->LoadStaticNpc());
	// 06. 05 HIDE NPC - 이영준
	STARTLOG(GAMERESRCMNGR->LoadHideNpcList());	
	STARTLOG(GAMERESRCMNGR->LoadSkillMoney());	
	STARTLOG(GAMERESRCMNGR->LoadExpPoint());
	STARTLOG(GAMERESRCMNGR->LoadFishingExpPoint());
	STARTLOG(GAMERESRCMNGR->LoadPlayerxMonsterPoint());
	STARTLOG(KYUNGGONGMGR->Init());

	STARTLOG(STORAGEMGR->LoadStorageList());
	STARTLOG(ITEMMGR->LoadItemList());	
	STARTLOG(ITEMMGR->LoadDealerItem());

	STARTLOG(ITEMMGR->LoadMonSummonItemInfo());

	//STARTLOG(ITEMMGR->LoadSetScript());
	//STARTLOG(ITEMMGR->LoadMixScript());
	//STARTLOG(ITEMMGR->LoadDissolveScript());
	//STARTLOG(ITEMMGR->LoadReinforceScript());
	// desc_hseos_체인지아이템01
	// S 체인지아이템 추가 added by hseos 2007.07.29
	STARTLOG(ITEMMGR->LoadScriptFileDataChangeItem());	
	// E 체인지아이템 추가 added by hseos 2007.07.29

	/*어빌리티 삭제 - 이영준 STARTLOG(GAMERESRCMNGR->LoadTaticAbilityInfo());*/
	STARTLOG(GAMERESRCMNGR->LoadAvatarEquipList());	
	STARTLOG(GAMERESRCMNGR->LoadShopItemDupList());

	//아이템 변화율 파일 로드
	STARTLOG(GAMERESRCMNGR->LoadItemChangeRateofLv()); //Add 060705 by wonju
	STARTLOG(GAMERESRCMNGR->LoadItemChangeRateofAttrib()); //Add 060707 wonju

	//SW050901
	STARTLOG(MON_SPEECHMGR->LoadMonSpeechInfoList());
	//SW050916
	//STARTLOG(RAREITEMMGR->LoadRareItemOptionInfo());
	//STARTLOG(RAREITEMMGR->LoadRareItemInfo());
	//STARTLOG(RAREITEMMGR->LoadRareValueProbList());
	//SW051129
/*어빌리티 삭제 - 이영준 	
	STARTLOG(ABILITYMGR->Init());
	STARTLOG(ABILITYMGR->AddUpdater(CAbilityUpdater_CheckLevelUp::GetInstance()));
	STARTLOG(ABILITYMGR->AddUpdater(CAbilityUpdater_Database::GetInstance()));
	STARTLOG(ABILITYMGR->AddUpdater(CAbilityUpdater_ApplyData::GetInstance()));
*/
	STARTLOG(PETMGR->LoadPetInfo());
	STARTLOG(ITEMDROP_OBJ->Init());
	
	STARTLOG(m_Map.InitMap(ServerNum));	
	STARTLOG(PATHMGR->SetMap(&m_Map, ServerNum, m_Map.GetTileWidth(ServerNum)));
	STARTLOG(PKMGR->Init( m_Map.IsPKAllow() ));	//map InitCN EA￠?￠® ⓒoY￥ia¨oA E¡IAaCI¨oA￠?A.

	// 
	STARTLOG(LoadEventRate("./System/Resource/droprate.txt"));

	// quest
	STARTLOG(QUESTMGR->LoadQuestScript());
	STARTLOG(QUESTREGENMGR->LoadData());	

	STARTLOG(CHANGEITEMMGR->Init());

	SWPROFITMGR->LoadMapInfo( ServerNum );

//---KES Aggro 070918
//---어그로관련 메모리풀 초기화
	CMonster::InitAggroMemoryPool();
//-------------------

//---KES AUTONOTE
	AUTONOTEMGR->Init();
//---------------

//	STATISTICSMGR->Init();

	//////////////////////////////////////////////////////////////////////////
	// ¡ÆOAO ¨oA¨o¨￢AU AE¡¾aE¡ⓒ
	//ITEMDROP_OBJ->Init();
	
	
	//////////////////////////////////////////////////////////////////////////
	// ⓒø¡¿¨¡￠c￠?oA¨I ¨uⓒød

	/// ⓒø¡¿¨¡￠c￠?oA¨I ¨¡A¨u¡ⓒ ¨uⓒød-------------------------------
	for(int i=0 ; i<MP_MAX ; ++i)
		g_pServerMsgParser[i] = NULL;
	for(i=0 ; i<MP_MAX ; ++i)
		g_pUserMsgParser[i] = NULL;
	g_pServerMsgParser[MP_SERVER] = MP_MAPSERVERMsgParser;
	g_pServerMsgParser[MP_POWERUP] = MP_POWERUPMsgParser;
	g_pServerMsgParser[MP_CHAR] = MP_CHARMsgParser;
	g_pServerMsgParser[MP_MAP] = NULL;
	g_pServerMsgParser[MP_ITEM] = MP_ITEMMsgParser;
	g_pServerMsgParser[MP_CHAT] = MP_CHATMsgParser;
	g_pServerMsgParser[MP_USERCONN] = MP_USERCONNMsgParser;
	g_pServerMsgParser[MP_MOVE] = MP_MOVEMsgParser;
	g_pServerMsgParser[MP_SKILLTREE] = MP_SKILLTREEMsgParser;
	g_pServerMsgParser[MP_AUCTIONBOARD] = NULL;
	g_pServerMsgParser[MP_CHEAT] = MP_CHEATMsgParser;
	g_pServerMsgParser[MP_QUICK] = MP_QUICKMsgParser;
	g_pServerMsgParser[MP_PACKEDDATA] = NULL;
	g_pServerMsgParser[MP_PARTY] = MP_PARTYMsgParser;
	// 070124 LYW --- PeaceWar : Delete peacewar part.
	//g_pServerMsgParser[MP_PEACEWARMODE] = MP_PEACEWARMODEMsgParser;
	g_pServerMsgParser[MP_UNGIJOSIK] = MP_UNGIJOSIKMsgParser;
	g_pServerMsgParser[MP_AUCTION] = NULL;
	g_pServerMsgParser[MP_SKILL] = MP_SkillMsgParser;
	g_pServerMsgParser[MP_STORAGE] = MP_STORAGEMsgParser;
	g_pServerMsgParser[MP_BATTLE] = MP_BattleMsgParser;
	g_pServerMsgParser[MP_CHAR_REVIVE] = MP_REVIVEMsgParser;
	g_pServerMsgParser[MP_WANTED] = MP_WANTEDMsgParser;
	g_pServerMsgParser[MP_JOURNAL] = MP_JOURNALMsgParser;

//KES EXCHANGE 030922
	g_pServerMsgParser[MP_EXCHANGE] = MP_EXCHANGEMsgParser;
	
// LBS 03.09.25 ⓒøea¡io
	g_pServerMsgParser[MP_STREETSTALL] = MP_STREETSTALLMsgParser;
	g_pServerMsgParser[MP_OPTION] = MP_OPTIONMsgParser;

// LBS 03.12.24 NPC￠?IAC ¡ioE¡IAU￠?e
	g_pServerMsgParser[MP_NPC] = MP_NPCMsgParser;
	
	g_pServerMsgParser[MP_MURIMNET] = MP_MurimnetMsgParser;

	g_pServerMsgParser[MP_QUEST] = MP_QUESTMsgParser;
	g_pServerMsgParser[MP_MORNITORMAPSERVER] = MP_MonitorMsgParser;

	g_pServerMsgParser[MP_PK] = MP_PKMsgParser;
	g_pServerMsgParser[MP_HACKCHECK] = MP_HACKCHECKMsgParser;
	g_pServerMsgParser[MP_SOCIETYACT] = MP_SOCIETYACTMsgParser;
	g_pServerMsgParser[MP_GUILD] = MP_GUILDMsgParser;
	g_pServerMsgParser[MP_GUILD_WAR] = MP_GUILDFIELDWARMsgParser;
	g_pServerMsgParser[MP_PARTYWAR] = MP_PARTYWARMsgParser;
	g_pServerMsgParser[MP_GTOURNAMENT] = MP_GTOURNAMENTMsgParser;
	g_pServerMsgParser[MP_GUILD_UNION] = MP_GUILUNIONMsgParser;
	g_pServerMsgParser[MP_SIEGEWAR] = MP_SIEGEWARMsgParser;
	g_pServerMsgParser[MP_SIEGEWAR_PROFIT] = MP_SIEGEWARPROFITMsgParser;

	// Weather System
	g_pServerMsgParser[MP_WEATHER] = MP_WEATHERMsgParser;

	// 070228 LYW --- ServerSystem : Add category for face part.
	g_pServerMsgParser[MP_FACIAL] = MP_FACIALMsgParser;

	// 070228 LYW --- ServerSystem : Add category for emotion part.
	g_pServerMsgParser[MP_EMOTION] = MP_EMOTIONMsgParser ;

	g_pServerMsgParser[MP_FAMILY] = MP_FAMILYMsgParser;

	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.04.11
	g_pServerMsgParser[MP_FARM] = MP_FARM_MsgParser;
	// E 농장시스템 추가 added by hseos 2007.04.11

	// desc_hseos_주민등록01
	// S 주민등록 추가 added by hseos 2007.06.04
	g_pServerMsgParser[MP_RESIDENTREGIST] = MP_RESIDENTREGIST_MsgParser;
	// E 주민등록 추가 added by hseos 2007.06.04

	// 071023 LYW --- ServerSystem : Add category for tutorial part.
	g_pServerMsgParser[MP_TUTORIAL] = MP_TUTORIALMsgParser ;

	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.19
	g_pServerMsgParser[MP_DATE] = MP_DATE_MsgParser;
	// E 데이트 존 추가 added by hseos 2007.11.19

	//---KES AUTONOTE
	g_pServerMsgParser[MP_AUTONOTE] = MP_AUTONOTE_MsgParser;
	//---------------

	// 080327 NYJ --- 낚시시스템
	g_pServerMsgParser[MP_FISHING] = MP_FISHING_ServerMsgParser;

	g_pServerMsgParser[MP_PET] = MP_PET_MsgParser;

	// channel for normal, eventmap, questmap
	STARTLOG( CHANNELSYSTEM->Init( ServerNum, m_Map.GetChannelNum() ) );

	// AI CwI¨u¨u¨u¡ⓒ
	g_pAISystem = new CAISystem;
	STARTLOG(g_pAISystem->LoadAIGroupList());

	/*
	// 071226 LUJ 주석 처리
	CUSTOM_EVENT ev[3];
	if( g_pServerSystem->GetMap()->IsVillage() )
		ev[0].dwPeriodicTime	= 10;
	else
		ev[0].dwPeriodicTime	= 10;

	ev[0].pEventFunc		= GameProcess;
	//---KES DB Process Fix 071114
	ev[1].dwPeriodicTime	= 1000*60*5;
	ev[1].pEventFunc		= _5minGameProcess;
//	ev[1].dwPeriodicTime	= 100;
	//----------------------------
//	ev[1].pEventFunc		= ProcessDBMessage;
//	ev[2].dwPeriodicTime	= 1000*60*5;
//	ev[2].pEventFunc		= _5minGameProcess;
	*/
	
	// 071226 LUJ,	DLL에서 하나의 함수만 주기적으로 호출되도록 함. 이에 따라 _5minGameProcess(), ProcessDBMessage()가
	//				모두 GameProcess() 내에서 호출되도록 함
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
	//desc.dwCustomDefineEventNum = 2;
	// 071226 LUJ
	desc.dwCustomDefineEventNum	= ( sizeof( ev ) / sizeof( *ev ) );
	desc.pEvent = ev;
	
	desc.dwMainMsgQueMaxBufferSize = 20480000;
	desc.dwMaxServerNum = 50;
	desc.dwMaxUserNum = 10;
	desc.dwServerBufferSizePerConnection = 512000;
	desc.dwServerMaxTransferSize = 65535;
	desc.dwUserBufferSizePerConnection = 65535;
	desc.dwUserMaxTransferSize = 65535;
	desc.dwConnectNumAtSameTime = 100;
	desc.dwFlag = 0;

	STARTLOG(g_Network.Init(&desc));
	g_Console.LOG(4, "　") ;
	g_Console.LOG(4, "[ Server Version : %s ]", g_SERVER_VERSION) ;
	g_Console.LOG(4, "　") ;
	if(!BOOTMNGR->StartServer(&g_Network, g_pServerTable->GetSelfServer()))
	{
		//ASSERT(0);
	}
	if(!BOOTMNGR->ConnectToMS(&g_Network, g_pServerTable))
	{
		SERVERINFO info;
		info.wServerKind = MONITOR_SERVER;
		OnConnectServerFail(&info);		
	}

	//////////////////////////////////////////////////////////////////////////
	// DB ¨uⓒød
	DWORD maxthread = g_pServerSystem->GetMap()->GetMaxDBThread();
	DWORD maxqueryinsametime = g_pServerSystem->GetMap()->GetMaxQueryInSameTime();
/*	FILE* fp = fopen("MapDBInfo.txt","r");
	if(fp)
	{
		fscanf(fp,"%d %d",&maxthread,&maxqueryinsametime);
		fclose(fp);
	}
*/	if(g_DB.Init(maxthread,maxqueryinsametime,FALSE) == FALSE)
		MessageBox(NULL,"DataBase Initializing Failed",0,0);


	// guildfieldwar
	GUILDWARMGR->Init();
	PARTYWARMGR->Init();

	// jackpot
	STARTLOG(JACKPOTMGR->Init(ServerNum));
	STARTLOG(JACKPOTMGR->LoadJackpotInfo());

	// SiegeWar
	SIEGEWARMGR->Init();
	
	// Guild Tournament
	GTMGR->Init();

	// Weather System
	// 080328 NYJ --- 낚시시스템추가로 날씨관리자 사용
	WEATHERMGR->Init();
	FISHINGMGR->Init();

	PartyLoad(0);
	// ⓒo￠c¨¡A￠￢￠c¨o¨￢¨¡￠c
	WantedLoad(0);

	GuildResetWarehouse( m_wMapNum );
	
	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.04.30	2007.09.07
	g_csFarmManager.Init(m_wMapNum);
//	FarmLoadFarmInfo(m_wMapNum);
	// E 농장시스템 추가 added by hseos 2007.04.30	2007.09.07

	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.26
	g_csDateManager.LoadChallengeMonsterInfo(m_wMapNum);
	// E 데이트 존 추가 added by hseos 2007.11.26

	m_dwQuestTime = gCurTime;
	m_bQuestTime = FALSE;

//	MiniDumper md(MiniDumper::DUMP_LEVEL_2);
	MiniDumper md(MiniDumper::DUMP_LEVEL_0);


	g_Console.LOG(4, "-----------   MAP SERVER START  -----------------");
	g_Console.LOG(4, "-----------   Map Number %d     -----------------", GAMERESRCMNGR->GetLoadMapNum());
	// AI ¨￢I¨￢¨￠: )￥i￥i￠?iAC ￠￢¨­¨oAAo ¡¤cCA.CONSOLE; ¡ic￠?eCI￥ic )￥i￥i￠?i ￠￢¨­¨oAAo ¡¾aⓒoY8¡¤I A¡I￥ic ¨ui￠Oⓒ÷¨oA8¡¤I￥ic AI¨￢¡I¨¡￠c￠￢| ￠￥e¡¾aC¨ª¨uⓒ￢CI
	// CIⓒoC¡¤I ¡¾IAu: ￠￢¨­¨oAAo ¡¤cCA AU¨u¨￢; CI¡Æi ¨oIAo ¨uE￠￥U￠￢e AI ￠￢¨­¨uO￥ia￠￢| ¡ic￠?eCN￠￥U.¡¾¡¿ ￠￥a¡Æi ￠￥a: ￠￢¨­¨oAAo ¡¤cCI ￥i￠?AICN ¡¾a￠￥E; 
	// ¨uoCaCN￠￥U.AI ￠￢¨­¨uO￥ia￠￥A I4DyuchiCONSOLE ￠￥UAI¨uo¡¤I¡¾¡¿AC xⓒoo¨¡¡Æ; ￠￥¨I￠￢|￠O¡×¡¾iAo ￠￢￠cAICIAo ¨uE￠￥A￠￥U.AI ￠￢¨­¨uO￥ia¡Æ￠® ￠￢￠cAICN￠￥U￠￢e CA
	// ¡¤I¡¾¡¿¡¤¡IAI ~¡¤aCI￠￥A ¡ÆIAI￠￥U.

	g_bReady = TRUE;

	fclose(fpstart);
	g_Console.WaitMessage();
}

void CServerSystem::End()
{
	//UnhookWindowsHookEx(hHook);
	SetStart(FALSE);

	g_Network.Release();

	/*
	g_pUserTable->SetPositionHead();
	CObject* pObject;
	while(pObject = g_pUserTable->GetData())
	{	
		if(pObject->GetObjectKind() == eObjectKind_Player)
			g_pServerSystem->RemovePlayer(pObject->GetID(), FALSE);
			//FALSE: 실제로 UserTable에서는 지우지 않는다.
	}
	g_pUserTable->RemoveAll();
	*/

	//SW060608 DBUpdate Safely
	if( TRUE != GetCharUpdateCompletion() )
	{//이곳에서의 처리는 장담할 수 없다. 쿼리 큐 리스트에 남은 채로 셧다운 가능하다고 본다.
		HandlingBeforeServerEND();	//(in) g_DB.Release();
	}

	SKILLMGR->Release();

//	m_objectIdxGen.Release();

	//g_DB.Release();

	g_Console.Release();

	BATTLESYSTEM->Release();

	SAFE_DELETE( g_pUserTable );
	SAFE_DELETE( g_pServerTable );
	SAFE_DELETE( g_pAISystem );
	SAFE_DELETE( g_pObjectFactory );
	SAFE_DELETE( m_pGridSystem );

//---KES Aggro 070918
//---어그로관련 메모리풀 릴리스
	CMonster::ReleaseAggroMemoryPool();
//-------------------

//---KES AUTONOTE
	AUTONOTEMGR->Release();
//---------------

//	CONDITIONMGR->Release();
	m_Map.Release();
	CoFreeUnusedLibraries();
}

void CServerSystem::Process()
{
	static BOOL bLoadFarm = TRUE;
	static DWORD FarmLoadTime = 0;

	// 071218 LUJ
	processTime.mPreviousTick	= GetTickCount();

	if(g_bCloseWindow)
	{
		SendMessage(g_hWnd, WM_CLOSE, 0,0);
		g_bCloseWindow = FALSE;
		SetStart(FALSE);
	}

	if( bLoadFarm )
	{
		if( gCurTime - FarmLoadTime > 1000 )
		{
			FarmLoadTime = gCurTime;
			if( !FarmLoadFarmInfoNew(m_wMapNum) )
			{
				bLoadFarm = FALSE;
			}
		}
	}

	if(GetStart() == FALSE)
		return;

	MHTIMEMGR_OBJ->Process();

	if(!g_pUserTable)
		return;

	static DWORD dwLastProcessTime = 0;

	m_pGridSystem->GridProcess();

	if( gCurTime > m_dwQuestTime+60000 )
	{
		m_dwQuestTime = gCurTime;
		m_bQuestTime = TRUE;
	}

	// 080602 NYJ --- 날씨, 낚시 Process()
	WEATHERMGR->Process();
	FISHINGMGR->Process();

	g_pUserTable->SetPositionUserHead();
	CObject* pObject;
	while(pObject = (CObject*)g_pUserTable->GetUserData())
	{
		if(pObject->GetInited() == FALSE)
			continue;
//		if(pObject->GetObjectKind() == eObjectKind_Pet )
//			continue;	//주인의 프로세스에서 처리. 종속적.

		OBJECTSTATEMGR_OBJ->StateProcess(pObject);
		CCharMove::MoveProcess(pObject);

		CCharacterCalcManager::UpdateLife(pObject);

		if(pObject->GetObjectKind() == eObjectKind_Player)
		{
			CPlayer* pPlayer = (CPlayer*)pObject;
			CCharacterCalcManager::ProcessLife(pPlayer);
			CCharacterCalcManager::UpdateMana(pPlayer);	
			CCharacterCalcManager::ProcessMana(pPlayer);

			pPlayer->QuestProcess();
			pPlayer->CheckMussangTime();
			if( m_bQuestTime )
			{
				// add quest event
				QUESTMGR->AddQuestEvent( pPlayer, &CQuestEvent( eQuestEvent_Time, 0, 0 ) );
			}

			pPlayer->ProcessTimeCheckItem( FALSE );


			// 낚시관련처리 : FISHINGMGR::Process() 가 호출된 이후에 사용되어야 함.
			//                유저테이블을 중복순회 하지 않기 위해 이곳에서 처리함.
			FISHINGMGR->ProcessPlayer(pPlayer);
		}
		else if(pObject->GetObjectKind() == eObjectKind_Monster)
		{
			CMonster* pMonster = (CMonster*)pObject;
			pMonster->Process();
		}
		else if(pObject->GetObjectKind() == eObjectKind_FieldSubMonster)
		{
			CFieldSubMonster* pMonster = (CFieldSubMonster*)pObject;
			pMonster->Process();
		}
		else if(pObject->GetObjectKind() == eObjectKind_FieldBossMonster)
		{
			CFieldBossMonster* pMonster = (CFieldBossMonster*)pObject;
			pMonster->Process();
		}
		else if(pObject->GetObjectKind() == eObjectKind_BossMonster)
		{
			((CBossMonster*)pObject)->Process();
			CCharacterCalcManager::ProcessLifeBoss((CBossMonster *)pObject);
		}
		else if(pObject->GetObjectKind() == eObjectKind_Npc)
		{
			CNpc* pNpc = (CNpc*)pObject;
			pNpc->Process();
		}
		else if(pObject->GetObjectKind() == eObjectKind_Pet)
		{
			CPet* pPet = (CPet*)pObject;
			CCharacterCalcManager::ProcessPetLife(pPet);
			CCharacterCalcManager::ProcessPetMana(pPet);
			pPet->FriendlyProcess();
		}
		
	}

	FISHINGMGR->ChangeMissionState();	// 유저테이블 순회후에 미션상태 변경을 해야함.
	FIELDBOSSMONMGR->Process();
	
	SKILLMGR->Process();

	//---KES DB Process Fix 071114
	//g_DB.ProcessingQueue();	//ProcessingDBMessage()를 한번씩 해주면 어떨까?
	//----------------------------
	
	BATTLESYSTEM->Process();

	if(g_pAISystem)
		g_pAISystem->Process();
	
	LOOTINGMGR->ProcessTimeOutCheck();
	QUESTMGR->Process();

//	EVENTMAPMGR->Process();

//	PARTYWARMGR->Process();


	//SW050719
//COMMENT:JACKPOT
//	JACKPOTMGR->Process();

	//SW060719 문파포인트
	GUILDMGR->Process();

	ITEMMGR->Process();

	// Guild Tournament
//	GTMGR->Process();

	// SiegeWar
//	SIEGEWARMGR->Process();
//	SWPROFITMGR->ProcessDBUpdateProfitMoney();

	ProcessWaitExitPlayer();

	m_bQuestTime = FALSE;

//--- Test
	m_dwMainProcessTime = gCurTime - dwLastProcessTime;
	dwLastProcessTime = gCurTime;

	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.04.12
	CSHMain::MainLoop();
	// E 농장시스템 추가 added by hseos 2007.04.12

//---KES AUTONOTE
	AUTONOTEMGR->Process();
//---------------
	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.27
	// ..맨 아래에 이전 LocalTime 을 저장해 둔다. 꼭 맨 아래에 있어야 함. 그렇지 않고
	// ..이 함수보다 아래에 위치하는 함수가 있을 경우 그 함수에서 OldLocalTime 을 사용하면 OldLocalTime 값이 설정되지 않음.
	MHTIMEMGR_OBJ->ProcOldLocalTime();
	// E 데이트 존 추가 added by hseos 2007.11.27

	// 071218 LUJ
	{
		processTime.mCurrentTick	=	GetTickCount();
		processTime.mSpace			=	processTime.mCurrentTick - processTime.mPreviousTick;
		processTime.mMaxSpace		=	max( processTime.mSpace, processTime.mMaxSpace );
		processTime.mTotalSpace		+=	processTime.mSpace;
		processTime.mAverageSpace	=	float( processTime.mTotalSpace ) / ++processTime.mCount;
	}
}

//void CServerSystem::_5minProcess()
//{
//	GUILDMGR->GUILDMGR_5MinProcess();
//	//	FAMEMGR->Process();
////	STATISTICSMGR->Process();
//}

void CServerSystem::SetStart( BOOL state )
{
	m_start = state;
	if( m_start )
	{
		g_Console.LOG( 4, "-----------   MAP SERVER LOAD DATA FROM DB  -----------------");
	}
}

// desc_hseos_데이트 존_01
// S 데이트 존 추가 added by hseos 2007.11.28
// CPlayer* CServerSystem::AddPlayer(DWORD dwPlayerID,DWORD dwAgentNum,DWORD UniqueIDinAgent,int ChannelNum)
CPlayer* CServerSystem::AddPlayer(DWORD dwPlayerID,DWORD dwAgentNum,DWORD UniqueIDinAgent,int ChannelNum, UINT nChallengeZoneSection)
// E 데이트 존 추가 added by hseos 2007.11.28
{
	CObject* pPreObj = g_pUserTable->FindUser(dwPlayerID);
	ASSERT(pPreObj == NULL);
	if(pPreObj)
	{
		pPreObj->SetNotInited();
		RemovePlayer(dwPlayerID);
	}

	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.25	2007.11.28
	// ..에이전트에서 별도로 관리하는 챌린지 존 채널ID가 있다. 채널시스템 내부에서 사용하기 위해 설정
	BOOL bStartChallengeZone = FALSE;
	CHANNELSYSTEM->SetChallengeZoneAgentChannelID(ChannelNum);
	// E 데이트 존 추가 added by hseos 2007.11.25	2007.11.28

	DWORD num = 0;
	if( QUESTMAPMGR->IsQuestMap() )
		// desc_hseos_데이트 존_01
		// S 데이트 존 추가 added by hseos 2007.11.28
		// num = CHANNELSYSTEM->CreateQuestMapChannel();
		num = CHANNELSYSTEM->CreateQuestMapChannel(dwPlayerID);
		// E 데이트 존 추가 added by hseos 2007.11.28
	else
		num = CHANNELSYSTEM->GetChannelID(ChannelNum);
	if( !num )	return NULL;

	BASEOBJECT_INFO binfo;
	memset(&binfo,0,sizeof(binfo));
	binfo.dwObjectID = dwPlayerID;
	CPlayer* pPlayer = (CPlayer*)g_pObjectFactory->MakeNewObject(eObjectKind_Player,dwAgentNum, &binfo);
	pPlayer->SetUniqueIDinAgent(UniqueIDinAgent);
	pPlayer->SetBattleID(num);
	pPlayer->SetGridID(num);
	pPlayer->SetChannelID(num);
	pPlayer->SetCurChannel(ChannelNum);
	pPlayer->SetInitState(PLAYERINITSTATE_ONLY_ADDED,0);

	CHANNELSYSTEM->IncreasePlayerNum(pPlayer->GetChannelID());

//	pPlayer->m_pDateMatching	=	0;


	g_pUserTable->AddUser(pPlayer,pPlayer->GetID());
//	g_Console.LOG(4, "User Connected : ID %d, Total(%d)", pPlayer->GetID(), g_pUserTable->GetUserCount());

	//SW050719	//NeedConfirm
//	JACKPOTMGR->SendMsgAddUserTotalMoney(pPlayer);	//SW050729 AGENT에서

	return pPlayer;
}

/*
CPlayer* CServerSystem::AddPlayer(DWORD dwPlayerID,DWORD dwAgentNum,DWORD UniqueIDinAgent,int ChannelNum)
{
	CObject* pPreObj = g_pUserTable->FindUser(dwPlayerID);
	ASSERT(pPreObj == NULL);
	if(pPreObj)
	{
		pPreObj->SetNotInited();
		RemovePlayer(dwPlayerID);
	}

	DWORD num = CHANNELSYSTEM->GetChannelID(ChannelNum);
	if(!num) 
		return NULL;

	BASEOBJECT_INFO binfo;
	memset(&binfo,0,sizeof(binfo));
	binfo.dwObjectID = dwPlayerID;
	CPlayer* pPlayer = (CPlayer*)g_pObjectFactory->MakeNewObject(eObjectKind_Player,dwAgentNum, &binfo);
	pPlayer->SetUniqueIDinAgent(UniqueIDinAgent);
	pPlayer->SetBattleID(num);
	pPlayer->SetGridID(num);
	pPlayer->SetChannelID(num);
	pPlayer->SetInitState(PLAYERINITSTATE_ONLY_ADDED,0);

	CHANNELSYSTEM->IncreasePlayerNum(pPlayer->GetChannelID());

	g_pUserTable->AddUser(pPlayer,pPlayer->GetID());
//	g_Console.LOG(4, "User Connected : ID %d, Total(%d)", pPlayer->GetID(), g_pUserTable->GetUserCount());
	return pPlayer;
}
*/

CPlayer* CServerSystem::InitPlayerInfo(BASEOBJECT_INFO* pBaseObjectInfo,CHARACTER_TOTALINFO* pTotalInfo,HERO_TOTALINFO* pHeroInfo)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pBaseObjectInfo->dwObjectID);
	if(pPlayer == NULL)
		return NULL;
	pBaseObjectInfo->BattleID = pPlayer->GetBattleID();
	ASSERT(pPlayer->GetID() == pBaseObjectInfo->dwObjectID);
	pPlayer->Init(eObjectKind_Player,pPlayer->GetAgentNum(),pBaseObjectInfo);
	pPlayer->InitCharacterTotalInfo(pTotalInfo);
	pPlayer->InitHeroTotalInfo(pHeroInfo);

	return pPlayer;
}

CMonster* CServerSystem::AddMonster(DWORD dwSubID, BASEOBJECT_INFO* pBaseObjectInfo,MONSTER_TOTALINFO* pTotalInfo,VECTOR3* pPos,WORD wObjectKind)
{
	ASSERT(g_pUserTable->FindUser(pBaseObjectInfo->dwObjectID) == NULL);

	EObjectKind objectkind = eObjectKind_Monster;
	
	// 080616 LUJ, 함정 추가
	switch( wObjectKind )
	{
	case eObjectKind_SpecialMonster:
	case eObjectKind_ToghterPlayMonster:
	case eObjectKind_Trap:
		{
			objectkind = EObjectKind( wObjectKind );
			break;
		}
	}

	CMonster* pMonster = (CMonster*)g_pObjectFactory->MakeNewObject(objectkind,0, pBaseObjectInfo);
	pMonster->SetSubID(dwSubID);
	pMonster->InitMonster(pTotalInfo);

	pMonster->SetGridID(pBaseObjectInfo->BattleID);

	pMonster->SetInited();
	CCharMove::InitMove(pMonster,pPos);

	g_pUserTable->AddUser(pMonster,pMonster->GetID());

	//AISystem
	g_pAISystem->AddObject(pMonster);
	
	CBattle* pBattle = BATTLESYSTEM->GetBattle(pMonster->GetBattleID());
	if(pBattle->GetBattleKind() != eBATTLE_KIND_NONE)
		BATTLESYSTEM->AddObjectToBattle(pBattle, pMonster);

	return pMonster;
}

CBossMonster* CServerSystem::AddBossMonster(DWORD dwSubID, BASEOBJECT_INFO* pBaseObjectInfo,MONSTER_TOTALINFO* pTotalInfo,VECTOR3* pPos)
{
	ASSERT(g_pUserTable->FindUser(pBaseObjectInfo->dwObjectID) == NULL);

	CBossMonster* pBossMonster = (CBossMonster*)g_pObjectFactory->MakeNewObject(eObjectKind_BossMonster,0, pBaseObjectInfo);
	pBossMonster->SetSubID(dwSubID);
	pBossMonster->InitMonster(pTotalInfo);
	//pMonster->SetSMonsterList();
	pBossMonster->SetGridID(pBaseObjectInfo->BattleID);
	pBossMonster->SetInited();
	
	CCharMove::InitMove(pBossMonster,pPos);
	

	g_pUserTable->AddUser(pBossMonster,pBossMonster->GetID());



	g_pAISystem->AddObject(pBossMonster); 
	BOSSMONMGR->SetBossInfo(pBossMonster);
	
	CBattle* pBattle = BATTLESYSTEM->GetBattle(pBossMonster->GetBattleID());
	if(pBattle->GetBattleKind() != eBATTLE_KIND_NONE)
		BATTLESYSTEM->AddObjectToBattle(pBattle, pBossMonster);

	return pBossMonster;
}

// 필드보스 - 05.12 이영준
// 필드보스 추가 함수
CFieldBossMonster* CServerSystem::AddFieldBossMonster(DWORD dwSubID, BASEOBJECT_INFO* pBaseObjectInfo,MONSTER_TOTALINFO* pTotalInfo,VECTOR3* pPos)
{
	ASSERT(g_pUserTable->FindUser(pBaseObjectInfo->dwObjectID) == NULL);

	CFieldBossMonster* pFieldBossMonster = (CFieldBossMonster*)g_pObjectFactory->MakeNewObject(eObjectKind_FieldBossMonster,0, pBaseObjectInfo);
	pFieldBossMonster->SetSubID(dwSubID);
	pFieldBossMonster->InitMonster(pTotalInfo);
	pFieldBossMonster->SetGridID(pBaseObjectInfo->BattleID);
	pFieldBossMonster->SetInited();
	
	CCharMove::InitMove(pFieldBossMonster,pPos);

	g_pUserTable->AddUser(pFieldBossMonster,pFieldBossMonster->GetID());

	g_pAISystem->AddObject(pFieldBossMonster); 
		
	CBattle* pBattle = BATTLESYSTEM->GetBattle(pFieldBossMonster->GetBattleID());
	if(pBattle->GetBattleKind() != eBATTLE_KIND_NONE)
		BATTLESYSTEM->AddObjectToBattle(pBattle, pFieldBossMonster);

	return pFieldBossMonster;
}

// 필드보스 - 05.12 이영준
// 필드보스 부하 추가 함수
CFieldSubMonster* CServerSystem::AddFieldSubMonster(DWORD dwSubID, BASEOBJECT_INFO* pBaseObjectInfo,MONSTER_TOTALINFO* pTotalInfo,VECTOR3* pPos)
{
	ASSERT(g_pUserTable->FindUser(pBaseObjectInfo->dwObjectID) == NULL);

	CFieldSubMonster* pFieldSubMonster = (CFieldSubMonster*)g_pObjectFactory->MakeNewObject(eObjectKind_FieldSubMonster,0, pBaseObjectInfo);
	pFieldSubMonster->SetSubID(dwSubID);
	pFieldSubMonster->InitMonster(pTotalInfo);
	pFieldSubMonster->SetGridID(pBaseObjectInfo->BattleID);
	pFieldSubMonster->SetInited();
	
	CCharMove::InitMove(pFieldSubMonster,pPos);

	g_pUserTable->AddUser(pFieldSubMonster,pFieldSubMonster->GetID());

	g_pAISystem->AddObject(pFieldSubMonster); 
		
	CBattle* pBattle = BATTLESYSTEM->GetBattle(pFieldSubMonster->GetBattleID());
	if(pBattle->GetBattleKind() != eBATTLE_KIND_NONE)
		BATTLESYSTEM->AddObjectToBattle(pBattle, pFieldSubMonster);

	return pFieldSubMonster;
}

CNpc* CServerSystem::AddNpc(BASEOBJECT_INFO* pBaseObjectInfo,NPC_TOTALINFO* pTotalInfo,VECTOR3* pPos)
{
	ASSERT(g_pUserTable->FindUser(pBaseObjectInfo->dwObjectID) == NULL);

	CNpc* pNpc = (CNpc*)g_pObjectFactory->MakeNewObject(eObjectKind_Npc,0, pBaseObjectInfo);
	pNpc->InitNpc(pTotalInfo);
	pNpc->SetGridID(pBaseObjectInfo->BattleID);
	pNpc->SetInited();
	CCharMove::InitMove(pNpc,pPos);

	g_pUserTable->AddUser(pNpc,pNpc->GetID());
	
	CBattle* pBattle = BATTLESYSTEM->GetBattle(pNpc->GetBattleID());
	if(pBattle->GetBattleKind() != eBATTLE_KIND_NONE)
		BATTLESYSTEM->AddObjectToBattle(pBattle, pNpc);

	return pNpc;
}

cSkillObject* CServerSystem::AddSkillObject(cSkillObject* pSkillObj,VECTOR3* pPos)
{
	ASSERT(g_pUserTable->FindUser(pSkillObj->GetID()) == NULL);

	pSkillObj->SetInited();
	CCharMove::InitMove(pSkillObj,pPos);

	g_pUserTable->AddUser(pSkillObj,pSkillObj->GetID());
	
	return pSkillObj;
}

CMapObject* CServerSystem::AddMapObject(DWORD Kind, BASEOBJECT_INFO* pBaseObjectInfo, MAPOBJECT_INFO* pMOInfo, VECTOR3* pPos)
{
	ASSERT(g_pUserTable->FindUser(pBaseObjectInfo->dwObjectID) == NULL);

	CMapObject* pMapObject = (CMapObject*)g_pObjectFactory->MakeNewObject((EObjectKind)Kind,0, pBaseObjectInfo);
	
	pMapObject->InitMapObject( pMOInfo );
	pMapObject->SetGridID( pBaseObjectInfo->BattleID );
	pMapObject->SetInited();
	CCharMove::InitMove( pMapObject, pPos );

	g_pUserTable->AddUser( pMapObject, pMapObject->GetID() );
	
	CBattle* pBattle = BATTLESYSTEM->GetBattle(pMapObject->GetBattleID());
	if(pBattle->GetBattleKind() != eBATTLE_KIND_NONE)
		BATTLESYSTEM->AddObjectToBattle( pBattle, pMapObject );

	return pMapObject;
}

/*
CItemObject* CServerSystem::AddItemObject(ITEMBASE* pItemInfo,VECTOR3* pPos)
{
	//ASSERT(GetNpc(pBaseObjectInfo->dwObjectID) == NULL);

	CItemObject* pItemObject = (CItemObject*)g_pObjectFactory->MakeNewObject(eObjectKind_Item,0,pItemInfo->MapNum,NULL);
	pItemObject->InitItem(pItemInfo);

	CCharMove::InitMove(pItemObject,pPos);

	m_ItemObjectTable.Add(pItemObject,pItemObject->GetID());
	pItemObject->SetInited();
	return pItemObject;
}
*/	

//SW060608 DBUpdate Safely
void CServerSystem::HandlingBeforeServerEND()
{
	//SetUnableProcess(TRUE);				//1. Process 리턴 & DBReturnMsg 처리 안함.

	g_Console.LOG(4, "[0/4]Start Working For Map Server Shutdown" );

	RemoveServerForKind(AGENT_SERVER);	//2. AgentSrv 끊기.
	g_Console.LOG(4, "[1/4]Agent Disconnecting...Done" );

	//3. =======Object 처리
	g_pUserTable->SetPositionHead();
	CObject* pObject;
	while(pObject = g_pUserTable->GetData())
	{	
		if(pObject->GetObjectKind() == eObjectKind_Player)
			g_pServerSystem->RemovePlayer(pObject->GetID(), FALSE);
			//FALSE: 실제로 UserTable에서는 지우지 않는다.
		else
		{
			CCharMove::ReleaseMove(pObject);
			g_pObjectFactory->ReleaseObject(pObject);
			if(pObject->GetObjectKind() == eObjectKind_Monster ||
				pObject->GetObjectKind() == eObjectKind_BossMonster )
			{
				if(g_pAISystem)
					g_pAISystem->RemoveObject(pObject->GetID());
			}
		}
	}
	g_Console.LOG(4, "[2/4]Start Object Handling (Update/Delete)...Done" );

	g_pUserTable->RemoveAll();
	g_Console.LOG(4, "[3/4]Removing All Object on UserTable...Done" );

	g_Console.LOG(4, "[4/4]Start DB Release..." );
	g_DB.Release();
	g_Console.LOG(4, "[4/4]Release...Done" );

	SetCharUpdateCompletion(TRUE);
}


void CServerSystem::RemovePlayer(DWORD dwPlayerID, BOOL bRemoveFromUserTable )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(dwPlayerID);
	if(!pPlayer)
		return;
	ASSERT(pPlayer->GetObjectKind() == eObjectKind_Player);

//	FRIENDMGR->UserLogOut(pPlayer->GetID());
	EXCHANGEMGR->UserLogOut(pPlayer);
	STREETSTALLMGR->UserLogOut(pPlayer);
	SHOWDOWNMGR->UserLogOut(pPlayer);
	LOOTINGMGR->LootingCancel(pPlayer);	//PKLOOT
	BOSSMONMGR->UserLogOut(pPlayer->GetID(), pPlayer->GetGridID());
	SIEGEWARMGR->UserLogOut(pPlayer);

	// 071122
	pPlayer->ProcessTimeCheckItem( TRUE );

	// YH d¨￢￠￢ ¨u¡A￥i¡IAI¨¡￠c￠￥A AE¡¾aE¡ⓒ¡Æ￠® f￠￥e¡¤I ￥iE /Au¡Æ￠® ⓒø¨￡¡Æ¡I￠O¡×￠￢￠￢ C¨ªA¨ª￠￥U.
	if(pPlayer->GetInited() == TRUE)
	{
		//Ac¡iy ¡Æu¡¤A ¡Æⓒ¡CeA￠®, ¡iy￠￢i¡¤A, ⓒø¡i¡¤A ¡¾iAO. (Data￠￢| AuAaCI¡¾a Au￠?￠® ¡Æⓒ¡CeA￠®￠?￠® ￠￥eCN ¨￢I¨￢¨￠; Aⓒø￠￢￠cC¨ª¨uⓒ￢ CO￥ii.)
		//Result￥iC¨ui¨u¡ⓒ KINE_NONEAI ￥iC¨uu￠￥U￠￢e?
		if(pPlayer->GetState() == eObjectState_Die)
		{
			if( ( pPlayer->GetBattle()->GetBattleKind() == eBATTLE_KIND_NONE && pPlayer->IsPenaltyByDie() ) ||
				pPlayer->GetBattle()->GetBattleKind() == eBATTLE_KIND_SIEGEWAR )
			{
				pPlayer->ReviveLogInPenelty(); 
			}
			else	//¨￢nⓒo¡iAⓒ￢ A¡¿¨uu￠￥U.	//¨￢nⓒo¡iAⓒ￢ ￠￢o¨o¨￢AI￠?￠®¡ÆO A¡¿; ¨uo￥i￥i AO￠￥U. confirm
			{
				pPlayer->ReviveAfterShowdown( FALSE );	//°a°uA³¸®¸¸.. msgº¸³≫Ao ¾E´A´U.
			}
		}

		// Aⓒø￠￢?d¨￢￠￢ ¨u¡A￥i¡IAI¨¡￠c///////////////////////
		if( pPlayer->IsPKMode() )
			pPlayer->SetPKModeEndtime();

		CharacterHeroInfoUpdate(pPlayer);
		CharacterTotalInfoUpdate(pPlayer);
		CharacterSkillTreeInfoUpdate(pPlayer);
		Option_Update(pPlayer);
		///////////////////////////////////////////

		CCharMove::ReleaseMove(pPlayer);

//		g_Console.Log(eLogDisplay, 4, "DisconnectUser : %d %s", dwPlayerID, pPlayer->GetObjectName());
		
		BOOL rt = BATTLESYSTEM->DeleteObjectFromBattle(pPlayer);

		MonsterMeter_Save(pPlayer->GetID(), 0, 0, pPlayer->GetMonstermeterInfo()->nPlayTimeTotal, pPlayer->GetMonstermeterInfo()->nKillMonsterNumTotal);

		if( pPlayer->IsEmergency() )
		{
			VECTOR3* ppos;
			VECTOR3 pos;

			ppos = GAMERESRCMNGR->GetRevivePoint();

			int temp;
			temp = rand() % 500 - 250;
			pos.x = ppos->x + temp;
			temp = rand() % 500 - 250;
			pos.z = ppos->z + temp;
			pos.y = 0;

			pPlayer->SetMapMoveInfo( GAMERESRCMNGR->GetLoadMapNum(), ( DWORD )( pos.x ), ( DWORD )( pos.z ) );
		}
	}
	else		
	{
		CCharMove::ReleaseMove(pPlayer);
	}

	CHANNELSYSTEM->DecreasePlayerNum(pPlayer->GetChannelID());
	
	UnRegistLoginMapInfo(dwPlayerID); //db￠?￠® CoAc ￠￢EⓒoⓒªE¡I 08¡¤I ¨uA¨¡A
	
	EVENTMAPMGR->RemoveMember( pPlayer, 0, pPlayer->GetChannelID() );
	PARTYWARMGR->RemovePlayer( pPlayer );
	QUESTMAPMGR->RemovePlayer( pPlayer );

	MAPTYPE map;
	DWORD	pos_x;
	DWORD	pos_z;

	pPlayer->GetMapMoveInfo( map, pos_x, pos_z );

	if( map == 0 && pos_x == 0 && pos_z == 0 )
	{
		VECTOR3 pos;
		
		pPlayer->GetPosition( &pos );

		map = GAMERESRCMNGR->GetLoadMapNum();
		
		pos_x = (DWORD)pos.x;
		pos_z = (DWORD)pos.z;
	}
	else
	{
		/// 맵 번호가 0인 경우 귀환지로 보내버리자?
		if( map == 0 )
		{
			LOGINPOINT_INFO* ReturnInfo = NULL;

			ReturnInfo = GAMERESRCMNGR->GetLoginPointInfo( pPlayer->GetPlayerLoginPoint() );

			/// 귀환지가 없다? 그럼 안전지대로...
			if( !ReturnInfo )
			{
				VECTOR3* ppos;
				VECTOR3 pos;

				ppos = GAMERESRCMNGR->GetRevivePoint();

				int temp;
				temp = rand() % 500 - 250;
				pos.x = ppos->x + temp;
				temp = rand() % 500 - 250;
				pos.z = ppos->z + temp;
				pos.y = 0;		

				map = GAMERESRCMNGR->GetLoadMapNum();
				pos_x = ( DWORD )( pos.x );
				pos_z = ( DWORD )( pos.z );
			}
			else
			{
				VECTOR3 RandPos;

				int temp;
				temp = rand() % 500 - 250;
				RandPos.x = ReturnInfo->CurPoint[0].x + temp;
				temp = rand() % 500 - 250;
				RandPos.z = ReturnInfo->CurPoint[0].z + temp;
				RandPos.y = 0;

				map = ReturnInfo->MapNum;
				pos_x = ( DWORD )( RandPos.x );
				pos_z = ( DWORD )( RandPos.z );
			}
		}

		/// 이동 좌표가 이상하다면... 안전지대로...
		if( pos_x == 0 || pos_x > 51200 || pos_z == 0 || pos_z > 51200 )
		{
			VECTOR3* ppos;
			VECTOR3 pos;

			ppos = GAMERESRCMNGR->GetRevivePoint();

			int temp;
			temp = rand() % 500 - 250;
			pos.x = ppos->x + temp;
			temp = rand() % 500 - 250;
			pos.z = ppos->z + temp;
			pos.y = 0;		

			map = GAMERESRCMNGR->GetLoadMapNum();
			pos_x = ( DWORD )( pos.x );
			pos_z = ( DWORD )( pos.z );
		}
	}

	if(pPlayer->GetCurMapNum() == GTMAPNUM)
	{		
		VECTOR3* ppos;
		VECTOR3 pos;
		ppos = GAMERESRCMNGR->GetRevivePoint(GTRETURNMAPNUM);

		int temp;
		temp = rand() % 500 - 250;
		pos.x = ppos->x + temp;
		temp = rand() % 500 - 250;
		pos.z = ppos->z + temp;
		pos.y = 0;		

		pos_x = ( DWORD )( pos.x );
		pos_z = ( DWORD )( pos.z );

		CharacterLogoutPointUpdate( pPlayer->GetID(), GTRETURNMAPNUM, pos_x, pos_z );
	}
	else
	{
		CharacterLogoutPointUpdate( pPlayer->GetID(), map, pos_x, pos_z );
	}

	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.29	2007.12.05
	// ..맵 이탈시 DB의 챌린지 존 입장 회수 초기화가 필요할 경우.. CSHDateManager::SRV_ProcChallengeZoneEnterFreq 함수의 설명 참조
	if (pPlayer->GetChallengeZoneNeedSaveEnterFreq())
	{
		ChallengeZone_EnterFreq_Save(pPlayer->GetID(), 0, pPlayer->GetChallengeZoneEnterBonusFreq());
		pPlayer->SetChallengeZoneNeedSaveEnterFreq(FALSE);
	}
	// ..챌린지 존 끝
	g_csDateManager.SRV_EndChallengeZone(pPlayer, CSHDateManager::CHALLENGEZONE_END_PARTNER_OUT);
	// E 데이트 존 추가 added by hseos 2007.11.29	2007.12.05

	// 080804 LUJ, 객체 해제는 항상 최후에 수행해야 한다. pPlayer로 수행할 코드가 있을 경우 이 코드 앞에 넣어야 한다
	{
		g_pObjectFactory->ReleaseObject( pPlayer );

		if( bRemoveFromUserTable )
		{
			g_pUserTable->RemoveUser( dwPlayerID );
		}
	}
}


CPet* CServerSystem::AddPet(BASEOBJECT_INFO* pBaseObjectInfo, PET_OBJECT_INFO* pPetObjectInfo, VECTOR3* pPos, CPlayer* pPlayer)
{
	CObject* pPreObj = g_pUserTable->FindUser( pPetObjectInfo->ID );
	ASSERT( pPreObj == NULL );
	if( pPreObj )
	{
		char log[256];
		sprintf( log, "./log/pet%02d.txt", g_pServerSystem->GetMapNum() );
		FILE* fp = fopen( log, "a+" );

		if( fp )
		{
			fprintf( fp, "READD:\t%d\n", pPreObj );
			fclose( fp );
		}

		pPreObj->SetNotInited();
		RemovePet( pPetObjectInfo->ID );
	}

	CPet* pPet = ( CPet* )g_pObjectFactory->MakeNewObject( eObjectKind_Pet, 0, pBaseObjectInfo );
	pPet->SetInited();
	pPet->SetGridID( pPlayer->GetGridID() );
	pPet->SetMaster( pPlayer );
	pPet->SetObjectInfo( pPetObjectInfo );
	pPlayer->SetCurPet( pPet );
	PETMGR->AddPetObject( pPet );

	g_pUserTable->AddUser( pPet, pPet->GetID() );

	char log[256];
	sprintf( log, "./log/pet%02d.txt", g_pServerSystem->GetMapNum() );
	FILE* fp = fopen( log, "a+" );

	if( fp )
	{
		fprintf( fp, "ADD:\t%d\n", pPet );
		fclose( fp );
	}

	return pPet;
}

void CServerSystem::RemovePet( DWORD dwPetObjectID )
{
	CObject* pObject = g_pUserTable->FindUser(dwPetObjectID);
	ASSERT(pObject);
	if(!pObject)	return;

	char log[256];
	sprintf( log, "./log/pet%02d.txt", g_pServerSystem->GetMapNum() );
	FILE* fp = fopen( log, "a+" );

	if( fp )
	{
		fprintf( fp, "REMOVE:\t%d\n", pObject );
		fclose( fp );
	}

	CCharMove::ReleaseMove(pObject);

	PETMGR->RemovePetObject( ( CPet* )pObject );

	g_pObjectFactory->ReleaseObject(pObject);
	g_pUserTable->RemoveUser(dwPetObjectID);
}

void CServerSystem::RemoveMonster(DWORD dwMonster)
{
	CObject* pObject = g_pUserTable->FindUser(dwMonster);
	ASSERT(pObject);
	if(!pObject)	return;

	CCharMove::ReleaseMove(pObject);
	
	BOOL rt = BATTLESYSTEM->DeleteObjectFromBattle(pObject);
	ASSERT(rt);

	if(g_pAISystem)
	g_pAISystem->RemoveObject(dwMonster);
	
	g_pObjectFactory->ReleaseObject(pObject);
	g_pUserTable->RemoveUser(dwMonster);
}


void CServerSystem::RemoveBossMonster(DWORD dwMonster)
{
	CObject* pObject = g_pUserTable->FindUser(dwMonster);
	ASSERT(pObject);
	if(!pObject)	return;

	CCharMove::ReleaseMove(pObject);
	
	BOOL rt = BATTLESYSTEM->DeleteObjectFromBattle(pObject);
	ASSERT(rt);

	g_pObjectFactory->ReleaseObject(pObject);

	if(g_pAISystem)
		g_pAISystem->RemoveObject(dwMonster);

	g_pUserTable->RemoveUser(dwMonster);


//CONDITIONMGR->RemoveListToPool(dwMonster);

}

void CServerSystem::RemoveNpc(DWORD dwNpcID)
{
	CObject* pObject = g_pUserTable->FindUser(dwNpcID);
	ASSERT(pObject);
	if(!pObject)	return;
	
	CCharMove::ReleaseMove(pObject);
	
	BOOL rt = BATTLESYSTEM->DeleteObjectFromBattle(pObject);
	ASSERT(rt);
	if(!pObject)	return;

	g_pObjectFactory->ReleaseObject(pObject);
	g_pUserTable->RemoveUser(dwNpcID);
}

void CServerSystem::RemoveSkillObject(DWORD SkillObjectID)
{
	CObject* pObject = g_pUserTable->FindUser(SkillObjectID);
	//ASSERT(pObject);
	if(!pObject)	return;
	
	CCharMove::ReleaseMove(pObject);
	g_pObjectFactory->ReleaseObject(pObject);
	g_pUserTable->RemoveUser(SkillObjectID);
}

void CServerSystem::RemoveMapObject( DWORD MapObjID )
{
	CObject* pObject = g_pUserTable->FindUser(MapObjID);
	ASSERT(pObject);
	if(!pObject)	return;
	
	CCharMove::ReleaseMove(pObject);
	
	BOOL rt = BATTLESYSTEM->DeleteObjectFromBattle(pObject);
	ASSERT(rt);
	if(!pObject)	return;

	g_pObjectFactory->ReleaseObject(pObject);
	g_pUserTable->RemoveUser(MapObjID);
}


void CServerSystem::SendToOne(CObject* pObject,void* pMsg,int MsgLen)
{
	g_Network.Send2Server(pObject->GetAgentNum(),(char*)pMsg,MsgLen);
}

void CServerSystem::ReloadResourceData()	// case MP_CHEAT_RELOADING:
{
	//////////////////////////////////////////////////////////////////////////
	//SkillList
	SKILLMGR->Release();
	SKILLMGR->Init();
	
	//////////////////////////////////////////////////////////////////////////
	//¡Æⓒ¡CeA￠®
	GAMERESRCMNGR->LoadPlayerxMonsterPoint();

	//////////////////////////////////////////////////////////////////////////
	//ItemList
	ITEMMGR->ReloadItemList();
	
	//////////////////////////////////////////////////////////////////////////
	//Regen
	//￠￢o¨o¨￢AI Remove
	g_pUserTable->SetPositionUserHead();
	CObject* pObject;
	while(pObject = (CObject*)g_pUserTable->GetUserData())
	{
		if(pObject->GetObjectKind() == eObjectKind_Monster)
		{
			RemoveMonster(pObject->GetID());
		}
		else if(pObject->GetObjectKind() == eObjectKind_BossMonster)
		{
			RemoveBossMonster(pObject->GetID());
		}
		else if(pObject->GetObjectKind() == eObjectKind_Npc)
		{
			RemoveNpc(pObject->GetID());
		}
		
	}
	g_pAISystem->RemoveAllList();

	REGENMGR->Release();
	GROUPMGR->Release();
	

	//////////////////////////////////////////////////////////////////////////
	//MonsterList
	GAMERESRCMNGR->ResetMonsterList();
	GAMERESRCMNGR->LoadMonsterList();
	GAMERESRCMNGR->LoadSMonsterList();

	//SW050901
	MON_SPEECHMGR->LoadMonSpeechInfoList();
	//SW050916
	//RAREITEMMGR->LoadRareItemOptionInfo();
	//RAREITEMMGR->LoadRareItemInfo();
	//RAREITEMMGR->LoadRareValueProbList();
//	MON_SPEECHMGR->LoadSpeechList();
	//////////////////////////////////////////////////////////////////////////



	//ItemDrop
	ITEMDROP_OBJ->Init();
	
	// reload
	g_pAISystem->LoadAIGroupList();

}


void CServerSystem::AddWaitExitPlayer( DWORD dwAgentConnetionIndex, DWORD dwPlayerID )
{
	WAIT_EXIT_PLAYER_INFO* pInfo = m_mpWaitExitPlayer->Alloc();
	pInfo->dwAgentConnectionIndex = dwAgentConnetionIndex;
	pInfo->dwPlayerID = dwPlayerID;
	pInfo->dwStartTime = gCurTime;
	m_listWaitExitPlayer.AddTail( pInfo );
}

void CServerSystem::ProcessWaitExitPlayer()
{
	if( m_listWaitExitPlayer.GetCount() == 0 ) return;

	WAIT_EXIT_PLAYER_INFO* pInfo = (WAIT_EXIT_PLAYER_INFO*)m_listWaitExitPlayer.GetHead();
	if( pInfo )
	{
		if( gCurTime - pInfo->dwStartTime >= 5*1000 )	//10초--->5초로 변경(2005.11.1)
		{
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pInfo->dwPlayerID );
			if( pPlayer )
			{
				DWORD dwObjectID = pPlayer->GetID();
				DWORD dwUserID = pPlayer->GetUserID();

				// RaMa - 04.12.14
				pPlayer->UpdateLogoutToDB();

				PARTYMGR->UserLogOut( pPlayer );
				FRIENDMGR->UserLogOut( dwObjectID );
				GUILDMGR->UserLogOut( pPlayer );

				g_pServerSystem->RemovePlayer( dwObjectID );
				//
				ConfirmUserOut( pInfo->dwAgentConnectionIndex, dwUserID, dwObjectID, 2 );
			}

			m_mpWaitExitPlayer->Free( pInfo );
			m_listWaitExitPlayer.RemoveHead();
		}
	}
}

void CServerSystem::SetNation()
{
	CMHFile file;
	if( file.Init( "LocalizingInfo.txt", "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
	{

		if( strcmp( file.GetString(), "*NATION" ) == 0 )
		{
			if( strcmp( file.GetString(), "CHINA" ) == 0 )
			{
				m_Nation = eNATION_CHINA;
			}
		}

		file.Release();
	}

	CMHFile file2;
	if( file2.Init( "_TESTSERVER", "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
	{
		m_bTestServer = TRUE;
		file2.Release();
	}
}

void CServerSystem::RemoveServerForKind( WORD wSrvKind )
{
	g_pServerTable->RemoveServerForKind(wSrvKind);
}


void CServerSystem::LoadHackCheck()
{
	CMHFile file;
	if( !file.Init( "ServerSet/HackCheck.txt", "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
	{
		g_Console.LOG(4, "HackCheckNum : %d, HackCheckWriteNum : %d", g_nHackCheckNum, g_nHackCheckWriteNum );
		return;
	}

	char temp[256] = {0, };
	while( 1 )
	{
		if( file.IsEOF() )	break;

		file.GetString( temp );
		if( strcmp( temp, "*HACKCHECKNUM" ) == 0 )
		{
			g_nHackCheckNum = file.GetInt();
		}
		else if( strcmp( temp, "*HACKCHECKWRITENUM" ) == 0 )
		{
			g_nHackCheckWriteNum = file.GetInt();
		}
	}

	file.Release();

	g_Console.LOG(4, "HackCheckNum : %d, HackCheckWriteNum : %d", g_nHackCheckNum, g_nHackCheckWriteNum );
}

// 080813 LUJ, 수정된 inetwork 호출 형식에 따라 변경
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
	/*/// Ao￠?i¡ÆIAO taiyo
	if(!dwConnectionIndex) return;

	info->dwConnectionIndex = dwConnectionIndex;					// AI¡ÆO f￠￥e¡¤I ￥i￠?AUCIⓒø¨￡? A¨uA¨I
	MSG_WORD msg;
	msg.Category = MP_SERVER;
	msg.Protocol = MP_SERVER_PWRUP;
	msg.wData = g_pServerTable->GetSelfServer()->wPortForServer;
	g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));
	//*/

	//PACKDATA￠?￠®¨u¡ⓒ ¡ic￠?eCI￠￥A ¨￢?¨uo
	if(g_pServerTable->GetMaxServerConnectionIndex() < dwConnectionIndex)
			g_pServerTable->SetMaxServerConnectionIndex(dwConnectionIndex);
}

// 080813 LUJ, 수정된 inetwork 호출 형식에 따라 변경
void __stdcall OnConnectServerFail(void* pVoid)
{
	SERVERINFO* info = (SERVERINFO*)pVoid;
	if(info->wServerKind == MONITOR_SERVER)
	{
		BOOTMNGR->AddBootListINI(MAP_SERVER, GAMERESRCMNGR->GetLoadMapNum(), g_pServerTable);
		BOOTMNGR->BactchConnectToMap(&g_Network, g_pServerTable);
		g_Console.LOG(4, "Failed to Connect to the MS : %s, %d", info->szIPForServer, info->wPortForServer);
	}
	else
	{
		g_Console.LOG(4, "Failed to Connect to the Server : %s, %d", info->szIPForServer, info->wPortForServer);
		BOOTMNGR->RemoveBootList(g_pServerTable, info->wPortForServer);
	}	
}

// 080813 LUJ, 수정된 inetwork 호출 형식에 따라 변경
void __stdcall OnAcceptServer(DWORD dwConnectionIndex)
{
	g_Console.LOG(4, "Server Connected : ConnectionIndex %d", dwConnectionIndex);
}

// 080813 LUJ, 수정된 inetwork 호출 형식에 따라 변경
void __stdcall OnDisconnectServer(DWORD dwConnectionIndex)
{
	if( g_pServerSystem->GetStart() == FALSE ) return;

	g_Console.LOG(4, "Server Disconnected : ConnectionIndex %d", dwConnectionIndex);

	SERVERINFO * delInfo = g_pServerTable->RemoveServer(dwConnectionIndex);
	if(!delInfo)
	{
		ASSERT(0);
	}
	delete delInfo;


	// taiyo
	g_pUserTable->SetPositionUserHead();
	CObject * info = NULL;
	cPtrList list;

	while(info = g_pUserTable->GetUserData())
	{
		if(info->GetObjectKind() == eObjectKind_Player)
		{
			CPlayer * ply = (CPlayer *)info;
			if(ply->GetAgentNum() == dwConnectionIndex)
			{
				SaveMapChangePointUpdate(ply->GetID(), 0);
				//g_pServerSystem->RemovePlayer(ply->GetID());
				list.AddTail( ply );
			}
		}
	}
	PTRLISTPOS pos = list.GetHeadPosition();
	while( pos )
	{
		CPlayer* p = (CPlayer*)list.GetNext(pos);
		g_pServerSystem->RemovePlayer(p->GetID());
	}
	list.RemoveAll();
}

// 080813 LUJ, 수정된 inetwork 호출 형식에 따라 변경
void __stdcall OnAcceptUser(DWORD dwConnectionIndex)
{
}

// 080813 LUJ, 수정된 inetwork 호출 형식에 따라 변경
void __stdcall OnDisconnectUser(DWORD dwConnectionIndex)
{

}

// 080813 LUJ, 수정된 inetwork 호출 형식에 따라 변경
void __stdcall ProcessDBMessage(DWORD dwEventIndex)
{
	g_DB.ProcessingDBMessage();
}

// 080813 LUJ, 수정된 inetwork 호출 형식에 따라 변경
void __stdcall ReceivedMsgFromServer(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	MSGROOT* pTempMsg = reinterpret_cast<MSGROOT*>(pMsg);
	ASSERT(pTempMsg->Category < MP_MAX);
	
	if( g_pServerMsgParser[pTempMsg->Category] == NULL ||
		pTempMsg->Category >= MP_MAX ||
		pTempMsg->Category == 0)
		return;

	// desc_hseos_예외처리01
	// S 예외처리 추가 added by hseos 2007.07.21
	// g_pServerMsgParser[pTempMsg->Category](dwConnectionIndex, pMsg, dwLength);
	if (CSHDebug::GetExceptionProcGrade())
	{
		try
		{
			g_pServerMsgParser[pTempMsg->Category](dwConnectionIndex, pMsg, dwLength);
		}
		catch(...)
		{
			char szTxt[256];
			sprintf(szTxt, "M:%02d  ReceivedMsgFromServer, %u, %u, %u, %u", g_pServerSystem->GetMapNum(), dwConnectionIndex, pTempMsg->Category, pTempMsg->Protocol, dwLength);
			CSHDebug::LogExceptionError(szTxt);
		}
	}
	else
	{
		g_pServerMsgParser[pTempMsg->Category](dwConnectionIndex, pMsg, dwLength);
	}
	// E 예외처리 추가 added by hseos 2007.07.21
}

// 080813 LUJ, 수정된 inetwork 호출 형식에 따라 변경
void __stdcall ReceivedMsgFromUser(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	MSGROOT* pTempMsg = reinterpret_cast<MSGROOT*>(pMsg);
	ASSERT(pTempMsg->Category < MP_MAX);
	ASSERT(g_pUserMsgParser[pTempMsg->Category]);
	
	if( g_pUserMsgParser[pTempMsg->Category] == NULL ||
		pTempMsg->Category >= MP_MAX ||
		pTempMsg->Category == 0)
		return;

	// desc_hseos_예외처리01
	// S 예외처리 추가 added by hseos 2007.07.21
	// g_pUserMsgParser[pTempMsg->Category](dwConnectionIndex, pMsg, dwLength);
	if (CSHDebug::GetExceptionProcGrade())
	{
		try
		{
			g_pUserMsgParser[pTempMsg->Category](dwConnectionIndex, pMsg, dwLength);
		}
		catch(...)
		{
			char szTxt[256];
			sprintf(szTxt, "M:%02d  ReceivedMsgFromUser, %u, %u, %u, %u", g_pServerSystem->GetMapNum(), dwConnectionIndex, pTempMsg->Category, pTempMsg->Protocol, dwLength);
			CSHDebug::LogExceptionError(szTxt);
		}
	}
	else
	{
		g_pUserMsgParser[pTempMsg->Category](dwConnectionIndex, pMsg, dwLength);
	}
	// E 예외처리 추가 added by hseos 2007.07.21
}

// 080813 LUJ, 수정된 inetwork 호출 형식에 따라 변경
void __stdcall GameProcess( DWORD dwEventIndex )
{
	try
	{
		g_DB.ProcessingDBMessage();

		// 071226 LUJ, 메인 프로세스
		{
			const DWORD		space			= 100;
			const DWORD		tick			= GetTickCount();
			static DWORD	lastCheckedTick = tick;
			static DWORD	nextCheckTick	= tick + space;;

			if( lastCheckedTick > nextCheckTick && tick < lastCheckedTick && tick > nextCheckTick ||	// 오버플로
				lastCheckedTick < nextCheckTick && nextCheckTick < tick )								// 일반
			{
				g_pServerSystem->Process();

				lastCheckedTick	= tick;
				nextCheckTick	= tick + space;

//#ifdef _DEBUG
//				char text[ MAX_PATH ];
//				sprintf( text, "g_pServerSystem->Process(): tick: %d, step: %.0f\n", tick, abs( float( lastCheckedTick ) - float( nextCheckTick ) ) );
//				OutputDebugStr( text );
//#endif
			}
		}

		// 071226 LUJ, 5분 함수 호출
		{
			const DWORD		space			= 1000 * 60 * 5;
			const DWORD		tick			= GetTickCount();
			static DWORD	lastCheckedTick = tick;
			static DWORD	nextCheckTick	= tick + space;;

			if( lastCheckedTick > nextCheckTick && tick < lastCheckedTick && tick > nextCheckTick ||	// 오버플로
				lastCheckedTick < nextCheckTick && nextCheckTick < tick )								// 일반
			{
				_5minGameProcess();

				lastCheckedTick	= tick;
				nextCheckTick	= tick + space;

//#ifdef _DEBUG
//				char text[ MAX_PATH ];
//				sprintf( text, "_5minGameProcess(): tick: %d, step: %.0f\n", tick, abs( float( lastCheckedTick ) - float( nextCheckTick ) ) );
//				OutputDebugStr( text );
//#endif
			}
		}
	}
	catch ( ... )
	{
		// hseos 2007.07.21
		if (CSHDebug::GetExceptionProcGrade())
		{
			char szTxt[ MAX_PATH ];
			sprintf(szTxt, "M:%02d  GameProcess", g_pServerSystem->GetMapNum());
			CSHDebug::LogExceptionError(szTxt);
		}

		ASSERT( 0 );
	}	
}


void _5minGameProcess()
{
	ConnectionCheck();
	//g_pServerSystem->_5minProcess();
}


// console interface function
void ButtonProc1()
{
	g_Console.LOG( 4, "QueryCount: %d, GetDB: %p", g_DB.GetQueryQueueCount(), g_DB.GetCurDB() );
/*
	g_bAssertMsgBox = !g_bAssertMsgBox;
	if(g_bAssertMsgBox)
		g_Console.LOG(4,"Assert MsgBox is On");
	else
		g_Console.LOG(4,"Assert MsgBox is Off");
*/
}


extern BOOL g_bWriteQuery;
void ButtonProc2()
{
	g_pServerSystem->LoadHackCheck();

	/*
		g_bWriteQuery = !g_bWriteQuery;
			if(g_bWriteQuery)
				g_Console.LOG(4,"g_bWriteQuery is True");
			else
				g_Console.LOG(4,"g_bWriteQuery is False");*/		
}

void ButtonProc3()
{
	if( g_pServerSystem )
	{
		//g_Console.LOG( 4, "MAIN PROCESS TIME : %d",  );

		g_Console.LOG(
			4,
			"%d: space: %dms(avg: %0.1fms, max: %dms) process time: %dms",
			processTime.mCount,
			processTime.mSpace,
			processTime.mAverageSpace,
			processTime.mMaxSpace,
			g_pServerSystem->GetMainProcessTime() );
	}
/*
	CObject* pInfo = NULL;
	g_pUserTable->SetPositionHead();
	while(pInfo = g_pUserTable->GetData())
	{
		if(pInfo->GetObjectKind() != eObjectKind_Player)
		{
			if(pInfo->GetGridID() == 1)
			{
				pInfo->Die(NULL);
			}
		} 
	}
*/
}

void OnCommand(char* szCommand)
{
	// desc_hseos_몬스터미터01
	// S 몬스터미터 추가 added by hseos 2007.07.09
	if (stricmp(szCommand, "MONSTERMETER_LOAD_REWARD_SCRIPT") == 0 || 
		stricmp(szCommand, "MM_LRS") == 0)
	{
		g_csMonstermeterManager.LoadScriptFileData();
		g_Console.LOG(4, "- Done Cmd:MONSTERMETER_LOAD_REWARD_SCRIPT -");
	}
	// E 몬스터미터 추가 added by hseos 2007.07.09

	// desc_hseos_예외처리01
	// S 예외처리 추가 added by hseos 2007.07.21
	if (stricmp(szCommand, "CHECK_TRYCATCH") == 0)
	{
		CSHDebug::SetExceptionProcGrade(!CSHDebug::GetExceptionProcGrade());
		g_Console.LOG(4, "- TryCatchGrade: %d", CSHDebug::GetExceptionProcGrade());
	}
	// E 예외처리 추가 added by hseos 2007.07.21

	// desc_hseos_체인지아이템01
	// S 체인지아이템 추가 added by hseos 2007.07.30
	if (stricmp(szCommand, "CHANGEITEM_LOAD") == 0)
	{
		ITEMMGR->LoadScriptFileDataChangeItem();
		g_Console.LOG(4, "- Done Cmd:CHANGEITEM_LOAD");
	}
	// E 체인지아이템 추가 added by hseos 2007.07.30
}

BOOL LoadEventRate(char* strFileName)
{
	for(int i=0; i<eEvent_Max; ++i)	
	{
		gEventRate[i] = 1.f;
		gEventRateFile[i] = 1.f;
	}

	CMHFile file;
	// 080118 KTH -- 일반 모드로 파일을 연다.
	if( !file.Init( strFileName, "r", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR) )
		return FALSE;

	char Token[256];
	
// RaMa -04.11.24
	while( !file.IsEOF() )
	{
		file.GetString(Token);

		if(strcmp(Token,"#EXP") == 0)	
		{			
			//gExpRate = file.GetFloat();
			gEventRateFile[eEvent_ExpRate] = file.GetFloat();
		}
		else if(strcmp(Token, "#ABIL") == 0)
		{
			//gAbilRate = file.GetFloat();
			gEventRateFile[eEvent_AbilRate] = file.GetFloat();
		}
		else if(strcmp(Token,"#ITEM") == 0)
		{
			//gItemRate = file.GetFloat();
			gEventRateFile[eEvent_ItemRate] = file.GetFloat();
		}
		else if(strcmp(Token,"#MONEY") == 0)
		{
			//gMoneyRate = file.GetFloat();
			gEventRateFile[eEvent_MoneyRate] = file.GetFloat();
		}
		else if(strcmp(Token,"#GETMONEY") == 0)
		{
			//gGetMoney = file.GetFloat();
			gEventRateFile[eEvent_GetMoney] = file.GetFloat();
		}
		else if(strcmp(Token,"#DAMAGERECIVE") == 0)
		{
			//gDamageReciveRate = file.GetFloat();
			gEventRateFile[eEvent_DamageReciveRate] = file.GetFloat();
		}
		else if(strcmp(Token,"#DAMAGERATE") == 0)
		{
			//gDamageRate = file.GetFloat();
			gEventRateFile[eEvent_DamageRate] = file.GetFloat();
		}
		else if(strcmp(Token,"#MANASPEND") == 0)
		{
			//gNaeRuykRate = file.GetFloat();
			gEventRateFile[eEvent_ManaRate] = file.GetFloat();
		}
		else if(strcmp(Token,"#UNGISPEED") == 0)
		{
			//gUngiSpeed = file.GetFloat();
			gEventRateFile[eEvent_UngiSpeed] = file.GetFloat();
		}
		else if(strcmp(Token,"#PARTYEXP") == 0)
		{
			//gPartyExpRate = file.GetFloat();
			gEventRateFile[eEvent_PartyExpRate] = file.GetFloat();
		}
		else if(strcmp(Token,"#SKILLEXPRATE") == 0)
		{
			gEventRateFile[eEvent_SkillExp] = file.GetFloat();
		}		
	}


	file.Release();

	for(i=0; i<eEvent_Max; ++i)	
		gEventRate[i] = gEventRateFile[i];

	return TRUE;
}

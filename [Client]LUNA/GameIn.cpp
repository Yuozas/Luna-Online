
#include "stdafx.h"
#include "GameIn.h"
//#ifdef _CHEATENABLE_
#include "CheatMsgParser.h"
//#endif	//_CHEATENABLE_

#include "MainGame.h"
#include "WindowIdEnum.h"
#include "./Interface/cWindowManager.h"
#include "BattleSystem_client.h"

// 070216 LYW --- GameIn : Include header file of conduct manager.
#include "ConductManager.h"
#include "ObjectManager.h"
#include "ItemManager.h"
#include "MoveManager.h"
#include "MHTimeManager.h"
#include "QuickManager.h"
#include "PeaceWarModeManager.h"
#include "PartyManager.h"
#include "UngijosikManager.h"
#include "ObjectActionManager.h"
#include "GuildManager.h"
#include "KyungGongManager.h"
#include "StreetStallManager.h"
#include "StorageManager.h"
#include "FriendManager.h"
#include "NoteManager.h"
#include "ChatManager.h"

// 070228 LYW --- GameIn : Include header file of facial manager.
#include "FacialManager.h"

//050630SW
#include "cChatTipManager.h"
//-
#include "ExchangeManager.h"
#include "NpcScriptManager.h"
//#include "HelpDicManager.h"
#include "HelperManager.h"
#include "./Audio/MHAudioManager.h"
#include "OptionManager.h"
#include "WantedManager.h"
#include "MHCamera.h"
#include "./Input/UserInput.h"
#include "MouseCursor.h"
#include "JournalManager.h"

// 061220 LYW --- Delete this code.
//#include "MainBarDialog.h"
#include "CharStateDialog.h"
#include "MiniMapDlg.h"
#include "PartyIconManager.h"
#include "ReviveDialog.h"
// 061220 LYW --- Delete this code.
//#include "MainBarDialog.h"
#include "WantNpcDialog.h"

#include "mhMap.h"
#include "cMsgBox.h"

#include "MotionManager.h"

#include "MurimNet.h"
#include "CQuestManager.h"
#include "GameEventManager.h"
#include "objectstatemanager.h"
#include "CharacterCalcManager.h"
#include "PKManager.h"
#include "GMNotifyManager.h"
#include "ReinforceDlg.h"
#include "NpcScriptDialog.h"
//#include "MixManager.h"
#include "StatusIconDlg.h"
#include "PetStatusIconDialog.h"
#include "CharSelect.h"

#include "Showdownmanager.h"

#include "UserInfoManager.h"

//#include "cJackpotDialog.h"

#include "ExitManager.h"
#include "HackCheck.h"
#include "SocietyActManager.h"
#include "GTScoreInfoDialog.h"

#ifdef _GMTOOL_
#include "GMToolManager.h"
#endif

#include "Quest.h"
#include "QuestManager.h"

#include "MapChange.h"
#include "EventMapInfo.h"
#include "GuildFieldWar.h"
#include "PartyWar.h"
#include "PartyWarDialog.h"

#include "ItemShopDialog.h"
#include "MoveDialog.h"
#include "EventNotifyDialog.h"

#include "StorageDialog.h"
#include "GuildTournamentMgr.h"
//SW050715
#include "cJackpotManager.h"

#include "SkillDelayManager.h"
#include "GuildUnion.h"
#include "SeigeWarDialog.h"
#include "SiegeWarMgr.h"
#include "GuageDialog.h"

#include "MussangManager.h"
#include "WeatherManager.h"
// 061219 LYW --- Include main system dialog's header file.
#include "MainSystemDlg.h"

// 070122 LYW --- Include header file.
#include "cResourceManager.h"

// 070417 LYW --- Include CommonCalcFunc's header file.
#include "..\[CC]Header\CommonCalcFunc.h"

#include "./Interface/GameNotifyManager.h"

//스크린샷
#include "ScreenShotDlg.h"

#include "DateMatchingDlg.h"			// 데이트 매칭 다이얼로그 2007/03/26 - 이진영

//---KES AUTONOTE
#include "AutoNoteManager.h"
//--------------

#include "./ChatRoomMgr.h"
#include "FishingManager.h"

#ifdef TAIWAN_LOCAL
#include "BillingManager.h"
#endif

#include "cSkillTreeManager.h"
#include "../[cc]skill/client/manager/skillmanager.h"

// 080310 LUJ, 플레이어 레벨 변경에 따라 스킬 학습 창을 업데이트하기 위해 불러온다
#include "cSkillTrainingDlg.h"

// 070503 LYW --- GameIn : Include family manager.
//#include "FamilyManager.h"
#include "FamilyDialog.h"

#ifndef _RESOURCE_WORK_NOT_APPLY_
#include "AppearanceManager.h"
#endif

#ifdef _TESTCLIENT_
#include "AppearanceManager.h"
#include "PathManager.h"

void InitTestCharacters(void);
void InitTestHero();
void InitTestMonster();
CMonster * g_pMonster = NULL;
CBossMonster * g_pBossMonster = NULL;


#endif

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.04.12
#include "../hseos/SHMain.h"
#include "../hseos/Farm/SHFarmManager.h"
// E 농장시스템 추가 added by hseos 2007.04.12

// desc_hseos_패밀리01
// S 패밀리 추가 added by hseos 2007.07.11
#include "../hseos/Family/SHFamilyManager.h"
// E 패밀리 추가 added by hseos 2007.07.11

// desc_hseos_예외처리01
// S 예외처리 추가 added by hseos 2007.08.04
#include "../hseos/Debug/SHDebug.h"
// E 예외처리 추가 added by hseos 2007.08.04

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.08.24	2007.09.10	2007.10.23
#include "../hseos/Farm/SHFarmBuyDlg.h"
#include "../hseos/Farm/SHFarmUpgradeDlg.h"
#include "../hseos/Farm/SHFarmManageDlg.h"
// E 농장시스템 추가 added by hseos 2007.08.24	2007.09.10	2007.10.23

// 071017 LYW --- GameIn : Include tutorial manager.
#include "TutorialManager.h"

// 071201 LYW --- GameIn : Include map move dialog.
#include "cMapMoveDialog.h"

// 080109 LYW --- GameIn : 숫자 패드 다이얼로그 추가.
#include "./Interface/cNumberPadDialog.h"
// desc_hseos_데이트 존_01
// S 데이트 존 추가 added by hseos 2007.11.14  2007.11.15	2007.11.16	2008.01.24
#include "../hseos/Date/SHDateZoneListDlg.h"
#include "../hseos/Date/SHChallengeZoneListDlg.h"
#include "../hseos/Date/SHDateManager.h"
#include "../hseos/Date/SHChallengeZoneClearNo1Dlg.h"
// E 데이트 존 추가 added by hseos 2007.11.14  2007.11.15	2007.11.16	2008.01.24

// desc_hseos_결혼_01
// S 결혼 추가 added by hseos 2007.12.12
#include "../hseos/Marriage/SHMarriageManager.h"
// E 결혼 추가 added by hseos 2007.12.12

#include "PetManager.h"

#ifdef _NPROTECT_
#include "NProtectManager.h"
#endif

DWORD g_TempHeroID;	//수련장에서 씀
extern HWND _g_hWnd;

GLOBALTON(CGameIn)

#ifdef _NPROTECT_
#include "NProtectManager.h"
#endif

CGameIn::CGameIn()
{
	m_bGameInAcked	= FALSE;
	m_bInitForGame	= FALSE;
	m_GameInInitKind = eGameInInitKind_Login;

	m_pInventoryDlg		= NULL;
	m_pExchangeDlg		= NULL;
	m_pMixDlg			= NULL;
	m_pDealDlg			= NULL;
	m_pCharDlg			= NULL;
	// 061204 LYW --- Delete Preexistence Chatting Dialog.
	/*
	m_pChatDlg			= NULL;
	*/

	// 061127 LYW --- Add New Chatting Class Pointer In GameIn class.
	m_pChattingDlg		= NULL ;
	// 061130 LYW --- Add New Outside Chatting Clas Pointer In GameIn class.
	m_pOSChattingDlg	= NULL ;

	// 061219 LYW --- Add main system dialog.
	m_pMainSystemDlg	= NULL ;

//	m_pJackpotDlg		= NULL;
	// 061220 LYW --- Delete this code.
	//m_pMainDlg			= NULL;
	m_pDebugDlg			= NULL;
// LYJ 051017 구입노점상 추가
	m_pStallKindSelectDlg	= NULL;
	m_pStreetStallDlg	= NULL;
	m_pNpcScriptDlg		= NULL;
	m_pChatOptionDlg	= NULL;
	m_pOptionDlg		= NULL;
	m_pExitDlg			= NULL;

	// desc_hseos_몬스터미터_01
	// S 몬스터미터 추가 added by hseos 2007.04.09
	m_pMonstermeterDlg	= NULL;
	// E 몬스터미터 추가 added by hseos 2007.04.09

	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.08.24	2007.09.10	2007.10.23
	m_pFarmBuyDlg		= NULL;
	m_pFarmUpgradeDlg	= NULL;
	m_pFarmManageDlg	= NULL;
	// E 농장시스템 추가 added by hseos 2007.08.24	2007.09.10	2007.10.23

	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.14  2007.11.15	2008.01.24
	m_pDateZoneListDlg			= NULL;
	m_pChallengeZoneListDlg		= NULL;
	m_pChallengeZoneClearNo1Dlg	= NULL;
	// E 데이트 존 추가 added by hseos 2007.11.14  2007.11.15	2008.01.24

	m_pCharStateDlg		= NULL;

	m_pAuctionBoard		= NULL;

	m_MoneyDlg			= NULL;
	m_pQuickDlg			= NULL;
	m_pGuageDlg			= NULL;

	m_pMiniMapDlg		= NULL;
	m_pBigMapDlg		= NULL;

	m_pStorageDialog		= NULL;
	m_pNoteDialog		= NULL;
	m_pFriendDialog		= NULL;
	m_pMiniNoteDialog	= NULL;
	m_pMiniFriendDialog	= NULL;
	m_pReviveDialog		= NULL;
	m_pWantNpcDialog	= NULL;

	m_pMonsterGuageDlg	= NULL;
	
	m_pBailDlg			= NULL;
	m_pDissolveDig	= NULL;
	m_pEventNotifyDlg	= NULL;
	
	m_pItemShopDialog	= NULL;
	m_pMoveDialog		= NULL;
	m_pPointSaveDialog	= NULL;	

	// 070619 LYW --- Deactive SkillPointRedist.
	// 현재 사용하지 않아서 비활성화.
	//m_pSkPointDlg		= NULL;
	m_pSkPointNotifyDlg = NULL;

	m_MovePoint = 0;
	m_MoveMap	= 0;
	m_GuildIdx	= 0;
	m_SiegeWarMapNum	= 0;

	m_pShoutDlg			= NULL;
	// 061204 LYW --- Delete Preexistence Chatting Dialog.
	/*
	m_pShoutchatDlg		= NULL;
	*/
	m_pFishingDlg		= NULL;
	m_pFishingGaugeDlg	= NULL;
	m_pFishingPointDlg	= NULL;

	m_pChaseDlg			= NULL;
	m_pChaseinputDlg	= NULL;
	m_pNameChangeDlg	= NULL;
	m_pNameChangeNotifyDlg	= NULL;

	m_pDateMatchingDlg	 = NULL ;			// 데이트 매칭 다이얼로그 2007/03/26 - 이진영
	// 070605 LYW --- GameIn : Add identification dialog.
	m_pIdentificationDlg = NULL ;
	// 070605 LYW --- GameIn : Add favor icon dialog.
	m_pFavorIconDlg		= NULL ;
	// 070607 LYW --- GameIn : Add my info dialog.
	m_pMatchMyInfoDlg	= NULL ;
	// 070607 LYW --- GameIn : Add partner info dialog.
	m_pMatchPartnerInfoDlg	= NULL ;
	// 070607 LYW --- GameIn : Add record dialog.
	m_pMatchRecordDlg	= NULL ;

	/////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환
	m_pSkillOptionChangeDlg = NULL;
	m_pSkillOptionClearDlg = NULL;
	/////////////////////////////////////////////////////////////

	m_bDieInBossMap = FALSE;
	m_nCountInBossMap = 0;
	m_dwTimeInBossMap = 0;
	
	//캐릭터 로그인 시간변수(서버,클라이언트) 초기화
	memset(&GameinServerTime, 0, sizeof(GameinServerTime));
	m_Clientdate = 0;
	m_Clienttime = 0;

	m_pGameNotifyDlg = NULL ;

    // 071017 LYW --- GameIn : Initialize tutorial dialog pointer.
	m_TutorialDlg = NULL ;

	// 071017 LYW --- GameIn : Initialize helper dialog pointer.
	m_HelpDlg = NULL ;

	// 071023 LYW --- GameIn : Initialize notice button dialog of tutorial.
	m_TutorialBtnDlg = NULL ;

	// 071130 LYW --- GameIn : Add member to map move dialog.
	m_MapMoveDlg = NULL ;

	// 071227 LYW --- GameIn : 캐릭터 이름 변경창 추가.
	m_pChangeNameDlg = NULL ;

	// 080109 LYW --- GameIn : 숫자 패드 다이얼로그 추가.
	m_pNumberPadDlg = NULL ;

//---KES AUTONOTE
	m_pAutoNoteDlg = NULL;
	m_pAutoAnswerDlg = NULL;
//---------------

	// 080403 LYW --- GameIn : Initialize dialogs to null.
	m_pChatRoomMainDlg		= NULL ;
	m_pChatRoomCreateDlg	= NULL ;
	m_pChatRoomDlg			= NULL ;
	m_pChatRoomGuestListDlg	= NULL ;
	m_pChatRoomOptionDlg	= NULL ;
	m_pChatRoomJoinDlg		= NULL ;
	//m_pChatRoomLobbyDlg		= NULL ;

	//m_pCookDlg		= NULL;
}

CGameIn::~CGameIn()
{

}

#ifdef _TESTCLIENT_
void InitForTestClient()
{	
	MOTIONMGR->LoadMotionList();
	
	CHATMGR->Init();
	
	//LOGFILE("SkillManager Init");
	SKILLMGR->Init();
	
	//LOGFILE("LoadTestClientInfo");
	GAMERESRCMNGR->LoadTestClientInfo();
	
	//LOGFILE("APPEARANCEMGR->Init()");
	APPEARANCEMGR->Init();
	

	//LOGFILE("KYUNGGONGMGR->Init()");
	KYUNGGONGMGR->Init();
	//LOGFILE("GAMERESRCMNGR->LoadMonsterList()");
	GAMERESRCMNGR->LoadMonsterList();
//	LOGFILE("GAMERESRCMNGR->LoadSMonsterList()");
//	GAMERESRCMNGR->LoadSMonsterList();
	//LOGFILE("GAMERESRCMNGR->LoadNpcChxList()");
	GAMERESRCMNGR->LoadNpcChxList();
	//LOGFILE("GAMERESRCMNGR->LoadNpcList(GAMERESRCMNGR->m_NpcList)");
	GAMERESRCMNGR->LoadNpcList(GAMERESRCMNGR->m_NpcList);
	
	//LOGFILE("ITEMMGR->LoadItemList()");
	ITEMMGR->LoadItemList();
	//LOGFILE("ITEMMGR->LoadItemTooltipList()");
	ITEMMGR->LoadItemToolTipList();
	//LOGFILE("ITEMMGR->LoadRareItemInfo()");
	//ITEMMGR->LoadRareItemInfo();
	
	//LOGFILE("MAP->InitMap(GAMERESRCMNGR->m_TestClientInfo.Map)");
	MAP->InitMap(GAMERESRCMNGR->m_TestClientInfo.Map);
	PATHMGR->SetMap(MAP, GAMERESRCMNGR->m_TestClientInfo.Map, MAP->GetTileWidth());
//	LOGFILE("WINDOWMGR->CreatMousePoint()");
//	WINDOWMGR->CreatMousePoint();
	
//	LOGFILE("EFFECTMGR->Init()");		
//	EFFECTMGR->Init();

	GAMEIN->m_bGameInAcked = TRUE;

	//LOGFILE("OPTIONMGR->ApplySettings()");
	OPTIONMGR->ApplySettings();
}
#endif

BOOL CGameIn::InitForGame()
{
#ifdef	_DEBUG
	DWORD	dwStartTick	=	GetTickCount();
#endif

	m_bInitForGame = TRUE;

	WINDOWMGR->CreateGameIn();

	
	//LOGFILE("HELPERMGR->Init()");
//	HELPERMGR->Init();

	WINDOWMGR->AfterInit();
	
//	if( MAP->GetMapNum() != EVENTMAPNUM )
	USERINFOMGR->LoadUserInfo( eUIK_INTERFACE );

//----------
//	LOGFILE("MACROMGR->Init()");
//	MACROMGR->Init();
//	MIXMGR->LoadMixList();

//	LOGFILE("NPCSCRIPTMGR->Init()");
//	NPCSCRIPTMGR->Init();
//	LOGFILE("HELPDICMGR->Init()");
//	HELPDICMGR->Init();
//----------

//´UAaA° AøÆA¼³A¤A§CØ¼­. ¸AAⓒ·IinitAIEA E￡Aa.
	// 061220 LYW --- Delete this code.
	/*
	if( GetMainInterfaceDialog() )
		GetMainInterfaceDialog()->Refresh();
		*/
	if( GetCharStateDialog() )
		GetCharStateDialog()->Refresh();

	// 061204 LYW --- Delete Preexistence Chatting Dialog.
	//LOGFILE("CHATMGR->SetChatDialog( (CChatDialog*)WINDOWMGR->GetWindowForID( CTI_DLG ) )");
	//CHATMGR->SetChatDialog( (CChatDialog*)WINDOWMGR->GetWindowForID( CTI_DLG ) );
	// 061128 LYW --- Add New Chatting System to GameIn Class.
	// 070103 LYW --- Modified this code.
	//CHATMGR->SetChattingDlg( ( CChattingDlg* )WINDOWMGR->GetWindowForID( CHATTING_DLG ) ) ;
	CHATMGR->SetChattingDlg( GAMEIN->GetChattingDlg() ) ;
	CHATMGR->RestoreChatList();	
	ITEMMGR->ClearCoolTimeInfo();

	// 080411 LYW --- GameIn : 채팅방에서 채팅중이 었다면, 채팅 내용을 복구한다.
	CHATROOMMGR->RestoreChat() ;

	QUESTMGR->InitializeQuest();
	PKMGR->Init();

	m_bDieInBossMap = FALSE;
	m_nCountInBossMap = 0;
	m_dwTimeInBossMap = 0;
	
	GUILDUNION->Clear();


	//SW050825 그래픽옵션Tab: GraphicAutoCtrlProcess()
	//LOGFILE("InitForGameIn()");
	OPTIONMGR->InitForGameIn();

	//CFamilyDialog* pFamilyDialog = GetFamilyDlg() ;

	//if( pFamilyDialog )
	//{
	//	pFamilyDialog->LoadClassIcon() ;
	//}

#ifdef	_DEBUG
	DWORD	dwFuncTick	=	GetTickCount() - dwStartTick;

	char	buf[1024];
	sprintf( buf, "%s() spent %d Ticks\n", __FUNCTION__, dwFuncTick);
	OutputDebugString( buf);
#endif

	return TRUE;
}

BOOL CGameIn::Init(void* pInitParam)
{
	int GameInInitKind = *(int*)pInitParam;
	m_GameInInitKind = GameInInitKind;
	m_bGameInAcked = FALSE;

	// 080618 NYJ --- 현재채널정보는 gChannelNum 값을 얻어서 사용하는 방식으로 변경하여
	//                농장에 입장시 채널고정 시키는 아래의 코드는 주석처리함.

	//// desc_hseos_농장시스템_01
	//// S 농장시스템 추가 added by hseos 2007.09.19
	//// ..농장 맵이라면 채널을 무조건 0 으로 한다. 미니맵의 초기화전에 수행되야하기 때문에 여기에 위치함.
	//BOOL bInitFarmChannel = FALSE;
	//if (GameInInitKind == eGameInInitKind_Login) 
	//{
	//	bInitFarmChannel = TRUE;
	//	}
	//g_csFarmManager.CLI_ChangeChannel(bInitFarmChannel);
	//// E 농장시스템 추가 added by hseos 2007.09.19

//#	WINDOWMGR->CreateGameIn();
//#	WINDOWMGR->AfterInit();

#ifdef _TESTCLIENT_
	InitForTestClient();
#endif

	InitForGame();


#ifndef _RESOURCE_WORK_NOT_APPLY_

	APPEARANCEMGR->EnbaleReservation();

#endif

	//test
//	QUESTMGR->DeleteMapNpcList();

	//////////////////////////////////////////////////////////////////////////

	//LOGFILE("MAP->LoadStaticNpc(MAP->GetMapNum())");
	if(MAP->GetMapNum() != 0)
	{
		MAP->LoadStaticNpc(MAP->GetMapNum());

		MAP->LoadNpcForQuestDialog() ;
	}


	SHOWDOWNMGR->Init();		//AE±aE­
	EXCHANGEMGR->Init();	//AE±aE­
	STREETSTALLMGR->Init();
	FRIENDMGR->Init();
//	WANTEDMGR->Init();
	PARTYMGR->Init();
//	JOURNALMGR->Init();
	GUILDMGR->Init();
//	EVENTMAP->Init();
//	GUILDWARMGR->Init();
//	PARTYWAR->Init();
//	SIEGEMGR->Init();
//	SWPROFIT->Init();
//	MUSSANGMGR->Init();
//	WEATHERMGR->Init();
	FISHINGMGR->Init();
	SKILLTREEMGR->Init();
//	EFFECTMGR->Init();

	// 070216 LYW --- GameIn : Regist conduct.
	CONDUCTMGR->RegistConduct() ;

	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.04.12
	CSHMain::Init();
	// E 농장시스템 추가 added by hseos 2007.04.12

#ifdef _TESTCLIENT_
	//LOGFILE("InitTestHero()");
	InitTestHero();
	//LOGFILE("InitTestMonster()");
	InitTestMonster();
	//LOGFILE("InitTestCharacters()");
	InitTestCharacters();
#endif
	//LOGFILE("GAMEIN INIT COMPLETE");
	
#ifdef _GMTOOL_
	if( GMTOOLMGR->IsShowing() )
		GMTOOLMGR->ShowGMDialog( TRUE );
#endif

	
	NETWORK->SetCurState(this);

	switch(GameInInitKind)
	{
	case eGameInInitKind_Login:
	case eGameInInitKind_MapChange:
		{
			MSG_DWORD msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_GAMEIN_SYN;
			msg.dwData	 = gChannelNum;
			NETWORK->Send( &msg, sizeof(msg) );
		}
		break;
	case eGameInInitKind_SuryunEnter:
		{
			MSGBASE msg;
			msg.Category = MP_SURYUN;
			msg.Protocol = MP_SURYUN_ENTER_SYN;
			msg.dwObjectID = g_TempHeroID;
			NETWORK->Send( &msg, sizeof(msg) );
		}
		break;
	case eGameInInitKind_SuryunLeave:
		{
			MSGBASE msg;
			msg.Category = MP_SURYUN;
			msg.Protocol = MP_SURYUN_LEAVE_SYN;
			msg.dwObjectID = g_TempHeroID;
			NETWORK->Send( &msg, sizeof(msg) );
		}
		break;
		
	case eGameInInitKind_MovePos:
		{
			MSG_DWORD2 msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_GAMEINPOS_SYN;
			msg.dwObjectID = g_TempHeroID;
			msg.dwData1 = gChannelNum;
			msg.dwData2 = m_MovePoint;

			NETWORK->Send( &msg, sizeof(msg) );			
		}
		break;

	case eGameInInitKind_EventMapEnter:
		{
			MSG_DWORD3 msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_ENTER_EVENTMAP_SYN;
			msg.dwObjectID = g_TempHeroID;
			msg.dwData1 = EVENTMAP->GetEventMapNum();
			msg.dwData2 = EVENTMAP->GetChannelNum();
			msg.dwData3 = EVENTMAP->GetTeamNum();

			NETWORK->Send( &msg, sizeof(msg) );
		}
		break;
	case eGameInInitKind_GTEnter:
		{
			MSG_DWORD3 msg;
			msg.Category = MP_GTOURNAMENT;
			msg.Protocol = MP_GTOURNAMENT_BATTLEJOIN_SYN;
			msg.dwObjectID = g_TempHeroID;
			msg.dwData1 = m_GuildIdx;
			msg.dwData2 = 0;
			msg.dwData3 = m_MoveMap;
			NETWORK->Send( &msg, sizeof(msg) );
			
			m_MoveMap = 0;
		}
		break;
	case eGameInInitKind_GTLeave:
		{
			MSGBASE msg;
			msg.Category = MP_GTOURNAMENT;
			msg.Protocol = MP_GTOURNAMENT_LEAVE_SYN;
			msg.dwObjectID = g_TempHeroID;
			NETWORK->Send( &msg, sizeof(msg) );
		}
		break;
	case eGameInInitKind_GTObserverEnter:
		{
			MSG_DWORD3 msg;
			msg.Category = MP_GTOURNAMENT;
			msg.Protocol = MP_GTOURNAMENT_OBSERVERJOIN_SYN;
			msg.dwObjectID = g_TempHeroID;
			msg.dwData1 = m_GuildIdx;
			msg.dwData2 = m_BattleIdx;
			msg.dwData3 = m_MoveMap;
			NETWORK->Send( &msg, sizeof(msg) );
		}
		break;
	case eGameInInitKind_SWEnter:
		{
			MSG_DWORD4 msg;
			msg.Category = MP_SIEGEWAR;
			msg.Protocol = MP_SIEGEWAR_BATTLEJOIN_SYN;
			msg.dwObjectID = g_TempHeroID;
			msg.dwData1 = m_GuildIdx;			// Guild Idx
			msg.dwData2 = m_MoveMap;			// ReturnMapNum
			msg.dwData3 = 0;					// bObserver
			msg.dwData4 = m_SiegeWarMapNum;		// SiegeWarMapNum
			NETWORK->Send( &msg, sizeof(msg) );
			
			m_MoveMap = 0;
			m_SiegeWarMapNum = 0;
		}
		break;
	case eGameInInitKind_SWLeave:
		{
			MSGBASE msg;
			msg.Category = MP_SIEGEWAR;
			msg.Protocol = MP_SIEGEWAR_LEAVE_SYN;
			msg.dwObjectID = g_TempHeroID;
			NETWORK->Send( &msg, sizeof(msg) );
		}
		break;
	case eGameInInitKind_SWObserverEnter:
		{
			MSG_DWORD4 msg;
			msg.Category = MP_SIEGEWAR;
			msg.Protocol = MP_SIEGEWAR_OBSERVERJOIN_SYN;
			msg.dwObjectID = g_TempHeroID;
			msg.dwData1 = m_GuildIdx;			// Guild Idx
			msg.dwData2 = m_MoveMap;			// ReturnMapNum
			msg.dwData3 = 1;					// bObserver
			msg.dwData4 = m_SiegeWarMapNum;		// SiegeWarMapNum
			NETWORK->Send( &msg, sizeof(msg) );
			
			m_MoveMap = 0;
			m_SiegeWarMapNum = 0;
		}
		break;
	default:
		ASSERT(0);
	}

	return TRUE;
}

void CGameIn::ReleaseForGame()
{
	if( !m_bInitForGame ) return;

	WINDOWMGR->DestroyWindowAll();
//	HELPERMGR->Release();


	//TESTQUESTMNG->Release();
	m_bInitForGame = FALSE;

	m_pInventoryDlg			= NULL;
	m_pExchangeDlg			= NULL;
	m_pMixDlg				= NULL;
	m_pDealDlg				= NULL;
	m_pCharDlg				= NULL;
	// 061204 LYW --- Delete Preexistence Chatting Dialog.
	/*
	m_pChatDlg				= NULL;
	*/

	// 061127 LYW --- Add New Chatting Class Pointer In GameIn class.
	m_pChattingDlg			= NULL ;
	// 061130 LYW --- Add New Outside Chatting Clas Pointer In GameIn class.
	m_pOSChattingDlg		= NULL ;

	// 061219 LYW --- Add main system dialog.
	m_pMainSystemDlg		= NULL ;

//	m_pJackpotDlg			= NULL;
	// 061220 LYW --- Delete this code.
	//m_pMainDlg				= NULL;
	m_pDebugDlg				= NULL;
// LYJ 051017 구입노점상 추가
	m_pStallKindSelectDlg	= NULL;
	m_pStreetStallDlg		= NULL;
	m_pNpcScriptDlg			= NULL;
	m_pChatOptionDlg		= NULL;
	m_pOptionDlg			= NULL;
	m_pExitDlg				= NULL;
	m_pCharStateDlg			= NULL;

	m_pAuctionBoard			= NULL;

	m_MoneyDlg				= NULL;
	m_pQuickDlg				= NULL;
	m_pGuageDlg				= NULL;

	m_pMiniMapDlg			= NULL;
	m_pBigMapDlg			= NULL;

	m_pStorageDialog			= NULL;
	m_pNoteDialog			= NULL;
	m_pFriendDialog			= NULL;
	m_pMiniNoteDialog		= NULL;
	m_pMiniFriendDialog		= NULL;
	m_pReviveDialog			= NULL;
	m_pWantNpcDialog		= NULL;

	m_pMonsterGuageDlg		= NULL;

	m_pReinforceDlg			= NULL;

	m_pBailDlg				= NULL;
	m_pDissolveDig		= NULL;
	m_pEventNotifyDlg		= NULL;
	
	m_pShoutDlg				= NULL;
	// 061204 LYW --- Delete Preexistence Chatting Dialog.
	/*
	m_pShoutchatDlg			= NULL;
	*/

	m_pFishingDlg		= NULL;

	m_pChaseDlg				= NULL;
	m_pChaseinputDlg		= NULL;
	
	m_pNameChangeDlg		= NULL;
	m_pNameChangeNotifyDlg	= NULL;	
}

void CGameIn::Release(CGameState* pNextGameState)
{
#ifndef _RESOURCE_WORK_NOT_APPLY_

	APPEARANCEMGR->DisableReservation();

#endif

//	WEATHERMGR->Release();
//
	BATTLESYSTEM->ReleaseBattle();
//

	STATUSICONDLG->Release();
	
	//KES 020828 AO½A·I...
//	WINDOWMGR->GetMouseWindow()->SetActive( TRUE );
	CURSOR->SetActive( FALSE );
	// 070402 LYW --- Modified function to setting chatting dialog.
	//CHATMGR->SetChatDialog( NULL );
	CHATMGR->SetChattingDlg( NULL );

	///¿ⓒ±a±iAo AO½A
	AUDIOMGR->StopAll();
	QUICKMGR->Release();
	ITEMMGR->Release();

	SKILLTREEMGR->Release();
	QUESTMGR1->Release();
	PKMGR->Release();

//	MINIMAP->Release();
	
	if(m_GameInInitKind != eGameInInitKind_SuryunEnter && m_GameInInitKind != eGameInInitKind_EventMapEnter)
		USERINFOMGR->SaveUserInfo( eUIK_USERSTATE | eUIK_INTERFACE | eUIK_USERQUEST );	//HERO°¡ ¾ø¾iAo±a Au¿¡.

	EFFECTMGR->RemoveAllEffect();
	OBJECTMGR->RemoveAllObject();
	
//	EFFECTMGR->Release();

//#	WINDOWMGR->DestroyWindowAll();
//#	HELPERMGR->Release();
	
/*
	RESRCMGR->ReleaseResource(14);
	RESRCMGR->ReleaseResource(15);
	RESRCMGR->ReleaseResource(98);
	RESRCMGR->ReleaseResource(30);
	RESRCMGR->ReleaseResource(40);
*/
	MAP->Release();

	GUILDWARMGR->Release();

	// 	
	PICONMGR->DeleteAllPartyIcon();

	ReleaseForGame();
	
	
//	EVENTMAP->Release();



#ifdef _GMTOOL_
	//disable
	GMTOOLMGR->ShowGMDialog( FALSE );
#endif

	SKILLMGR->ReleaseAllSkillObject();

//RESRCMGR->ReleaseResource();
	
	g_pExecutive->UnloadAllPreLoadedGXObject(0);	//NULLAß°¡

	g_TempHeroID = HEROID;
	HEROID = 0;
	gUserID = 0;
}


void CGameIn::Process()
{
	if(!m_bGameInAcked)
		return;

	GAMEEVENTMGR->Process();

	CHATTIPMGR->Process();
	
	GetMiniMapDialog()->Process();

	EXITMGR->Process();
	ITEMMGR->Process();
	SKILLMGR->Process();
//	GetSWEngraveDlg()->Process();
//	GetSWTimeDlg()->Process();

//	ProcessInBossMap();

	FISHINGMGR->Process();

	OPTIONMGR->GraphicAutoCtrlProcess();

	//---KES TEST 너무 지속적으로 돌고 있는 것은 아닐까?
	// 070121 LYW --- Call function to check potal position.
	if( MOVEMGR->IsMoving(HERO) && !HERO->IsPKMode() && !PKMGR->IsPKLooted() && !HERO->IsDied() )
	{
		VECTOR3 vPosition ;
		HERO->GetPosition(&vPosition) ;

		WORD curMapNum = HERO->GetCharacterTotalInfo()->CurMapNum ;

		 WORD mapNum = GAMERESRCMNGR->CheckMapChange( &vPosition, curMapNum ) ;
		if( mapNum != 65535 ) g_UserInput.GetHeroMove()->ChangeMap( mapNum );
	}
	//--------------------------------------------


	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.04.12
	CSHMain::MainLoop();
	// E 농장시스템 추가 added by hseos 2007.04.12
#ifdef _NPROTECT_
	NPROTECTMGR->Check();
#endif

#ifdef _NPROTECT_
	NPROTECTMGR->Check();
#endif

	//---KES TEST 너무 지속적으로 돌고 있는 것은 아닐까?
	// 071017 LYW --- GameIn : Call function of tutorial manager.
	// 080502 LYW --- GameIn : 튜토리얼의 Process 호출 삭제.
	// 튜토리얼의 발동/완료 조건을 이벤트 발생 시 각각 체크하도록 변경.
	//TUTORIALMGR->Process() ;
	//---------------------------------------------

}

void CGameIn::BeforeRender()
{
	MAP->Process(gCurTime);
}

void CGameIn::AfterRender()
{
	if(m_bGameInAcked)
	{
		EFFECTMGR->Process(gCurTime);

//		STATUSICONDLG->Render();	//windowmanager에서 해준다(툴팁때문에)
		BATTLESYSTEM->Render();
//		WEATHERMGR->Render();
		WINDOWMGR->Render();
		NOTIFYMGR->Render();	//KES
//		EVENTMAP->Render();

//		PARTYWAR->Render();

		// 070818 LYW --- GameIn : 게임 관련 공지(GM용 말고, 퀘스트 공지등)를 출력하기 위해 추가함.
		//m_pGameNotifyDlg->Render() ;	
		GAMENOTIFYMGR->Render() ;

		// desc_hseos_농장시스템_01
		// S 농장시스템 추가 added by hseos 2007.09.07
		CSHMain::Render();
		// E 농장시스템 추가 added by hseos 2007.09.07
	}
}

// description by hseos S
// 서버->클라이언트 통신 parsing 을 위한 등록 부분
// description by hseos E
void CGameIn::NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg)
{
	//FILE* fp = fopen( "recv.txt", "a+" );
	//if( fp )
	//{
	//	fprintf( fp, "%d\t%d\n", Category, Protocol );
	//	fclose( fp );
	//}

	switch(Category)	//¿i¼±¼øA§¸|....
	{
	case MP_MOVE:			MOVEMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_SKILL:			SKILLMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_CHAR:			CHARCALCMGR->NetworkMsgParse(Protocol, pMsg);			break;
	case MP_WEATHER:		WEATHERMGR->NetworkMsgParse(Protocol,pMsg);				break;
	case MP_USERCONN:		UserConn_NetworkMsgParse( Protocol, pMsg ) ;			break ;
/*
	case MP_CHAR_REVIVE:
		{
			switch(Protocol)
			{
			case MP_CHAR_REVIVE_LOGINSPOT_ACK:
				{
					MOVE_POS* pmsg = (MOVE_POS*)pMsg;
					VECTOR3 pos;
					CObject* pObject = OBJECTMGR->GetObject(pmsg->dwMoverID);
					pmsg->cpos.Decompress(&pos);
					OBJECTACTIONMGR->Revive(pObject,&pos);
					GAMEIN->GetReviveDialog()->SetDisable(FALSE);
				}
				break;

			case MP_CHAR_REVIVE_PRESENTSPOT_ACK: 
				{
					MOVE_POS* pmsg = (MOVE_POS*)pMsg;
					VECTOR3 pos;
					CObject* pObject = OBJECTMGR->GetObject(pmsg->dwMoverID);
					pmsg->cpos.Decompress(&pos);
					OBJECTACTIONMGR->Revive(pObject,&pos);
					GAMEIN->GetReviveDialog()->SetDisable(FALSE);					

				}
				break;
			}
		}
		break;
*/
    case MP_PARTY:			PARTYMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_QUICK:			QUICKMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_ITEM:			ITEMMGR->NetworkMsgParse(Protocol,pMsg);				break;
	case MP_SKILLTREE:		SKILLTREEMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_CHAT:			CHATMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_BATTLE:			BATTLESYSTEM->NetworkMsgParse(Protocol, pMsg);			break;
	// 070124 LYW --- PeaceWar : Delete peacewar part.
	//case MP_PEACEWARMODE:	PEACEWARMGR->NetworkMsgParse(Protocol, pMsg);			break;
//	case MP_UNGIJOSIK:		UNGIMGR->NetworkMsgParse(Protocol, pMsg);				break;
//	case MP_KYUNGGONG:		KYUNGGONGMGR->NetworkMsgParse(Protocol, pMsg);			break;
	case MP_EXCHANGE:		EXCHANGEMGR->NetworkMsgParse(Protocol, pMsg);			break;
	case MP_STREETSTALL:	STREETSTALLMGR->NetworkMsgParse(Protocol, pMsg);		break;
	case MP_STORAGE:			STORAGEMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_FRIEND:			FRIENDMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_NOTE:			NOTEMGR->NetworkMsgParse(Protocol, pMsg);				break;
	// 080107 KTH -- 
	case MP_BOSSMONSTER:	OBJECTMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_QUEST:			
		{

			QUESTMGR1->NetworkMsgParse(Protocol, pMsg);				
			QUESTMGR->NetworkMsgParse( Protocol, pMsg );
		}
		break;
	case MP_PK:				PKMGR->NetworkMsgParse(Protocol, pMsg);					break;
//	case MP_WANTED:			WANTEDMGR->NetworkMsgParse(Protocol, pMsg);				break;	
	case MP_CHEAT:			CHEATMGR->NetworkMsgParse(Category,Protocol,(MSGBASE*)pMsg);	break;	
//	case MP_JOURNAL:		JOURNALMGR->NetworkMsgParse(Protocol, pMsg);				break;	
	// 070330 LYW --- GameIn : Active network message parser of guild manager.
	case MP_GUILD:			GUILDMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_GUILD_UNION:	GUILDUNION->NetworkMsgParse(Protocol, pMsg);			break;
	case MP_NPC:			NPC_NetworkMsgParse( Protocol, pMsg ) ;					break ;
	case MP_SIGNAL:			SIGNAL_NetworkMsgParse( Protocol, pMsg ) ;				break ;
	// desc_hseos_패밀리01
	// S 패밀리 추가 added by hseos 2007.07.11
	/*
	// 070503 LYW --- GameIn : Add message parset of family.
	case MP_FAMILY :			FAMILYMGR->NetworkMsgParse( Protocol, pMsg ) ;			break ;
	*/
	// E 패밀리 추가 added by hseos 2007.07.11
/*어빌리티 삭제 - 	case MP_SOCIETYACT:	SOCIETYACTMGR->NetworkMsgParse( Protocol, pMsg ); break;*/
/*
	case MP_MURIMNET:
		{
			switch( Protocol )
			{
			case MP_MURIMNET_CHANGETOMURIMNET_ACK:
				{
					MSGBASE* pmsg = (MSGBASE*)pMsg;
					
					MAPTYPE dwMapNum = MAP->GetMapNum();
					MAINGAME->SetGameState( eGAMESTATE_MURIMNET, &dwMapNum, 4 );
				}
				break;

			case MP_MURIMNET_CHANGETOMURIMNET_NACK:
				{
					MessageBox(NULL, "Fail To Connect To MurimServer.",0,0);
				}
				break;

			case MP_MURIMNET_RETURNTOMURIMNET_ACK:
				{
					MSGBASE* pmsg = (MSGBASE*)pMsg;
					
					MAINGAME->SetGameState( eGAMESTATE_MURIMNET );
				}
				break;

			case MP_MURIMNET_RETURNTOMURIMNET_NACK:
				{
					MessageBox(NULL, "Fail To Come Back To MurimServer. Connect Again Plz",0,0);
					
//					MAINGAME->SetGameState( eGAMESTATE_END );
					PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
				}
				break;

			case MP_MURIMNET_MURIMSERVERIN_ACK:
				{
					SEND_HERO_TOTALINFO * pmsg = (SEND_HERO_TOTALINFO *)pMsg;
					OBJECTMGR->RegisterHero(pmsg);
					
					GAMEIN->GetMiniMapDialog()->AddHeroIcon(HERO);
										
					CAMERA->Init((CObject*)OBJECTMGR->GetHero(),30,0,1000);

					CAMERA->SetTargetAngleX(0,29.9f,0);					
				}
				break;

			case MP_MURIMNET_MURIMSERVERIN_NACK:
				{
				}
				break;
			}
		}*/
	case MP_GUILD_WAR:		GUILDWARMGR->NetworkMsgParse( Protocol, pMsg );		break;
	case MP_PARTYWAR:			PARTYWAR->NetworkMsgParse( Protocol, pMsg );			break;
//	case MP_GTOURNAMENT:		GTMGR->NetworkMsgParse( Protocol, pMsg);					break;

//	case MP_JACKPOT:			
//		{
//			if(GAMEIN->GetMiniMapDialog())
//			JACKPOTMGR->NetworkMsgParse( Protocol, pMsg );
//		}
//		break;
//	case MP_SIEGEWAR:			SIEGEMGR->NetworkMsgParse( Protocol, pMsg );	break;	
//	case MP_SIEGEWAR_PROFIT:	SWPROFIT->NetworkMsgParse( Protocol, pMsg );	break;
	// 070228 LYW --- GameIn : Add network message parser for facial part.
	case MP_FACIAL :		FACIALMGR->NetworkMsgParse( Protocol, pMsg );		break;
	// 070228 LYW --- GameIn : Add network message parser for emotion part.
	case MP_EMOTION :		CONDUCTMGR->NetworkMsgParse( Protocol, pMsg );		break;

	//case MP_DATEMATCHING :	m_pDateMatchingDlg->NetworkMsgParse( Protocol, (MSGBASE*)pMsg);	break;				// Date matching 2007.04.18 - 이진영

	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.04.12
	case MP_FARM :				g_csFarmManager.CLI_ParseAnswerFromSrv(pMsg);				break;
	// E 농장시스템 추가 added by hseos 2007.04.12

	// desc_hseos_주민등록01
	// S 주민등록 추가 added by hseos 2007.06.04
	case MP_RESIDENTREGIST :	g_csResidentRegistManager.CLI_ParseAnswerFromSrv(pMsg);		break;
	// E 주민등록 추가 added by hseos 2007.06.04

	// desc_hseos_패밀리01
	// S 패밀리 추가 added by hseos 2007.07.11
	case MP_FAMILY :			g_csFamilyManager.CLI_ParseAnswerFromSrv(pMsg);				break;
	// E 패밀리 추가 added by hseos 2007.07.11

	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.16
	case MP_DATE:				g_csDateManager.CLI_ParseAnswerFromSrv(pMsg);				break;
	// E 데이트 존 추가 added by hseos 2007.11.16

	// 071023 LYW --- GameIn : Add network parser of tutorial.
	case MP_TUTORIAL :			TUTORIALMGR->NetworkMsgParse( Protocol, pMsg ) ;			break ;

	// desc_hseos_결혼_01
	// S 결혼 추가 added by hseos 2007.12.12
//	case MP_MARRIAGE:			g_csMarriageManager.CLI_ParseAnswerFromSrv(pMsg);			break;
	// E 결혼 추가 added by hseos 2007.12.12

//---KES AUTONOTE
	case MP_AUTONOTE:			AUTONOTEMGR->NetworkMsgParse( Protocol, pMsg );				break;
//---------------

	case MP_CHATROOM :			CHATROOMMGR->NetworkMsgParser(pMsg) ;						break ;
	case MP_FISHING:			FISHINGMGR->NetworkMsgParse( Protocol, pMsg );				break;
	case MP_PET:				PETMGR->NetworkMsgParse( Protocol, pMsg );					break;
	case MP_OPTION:				OPTIONMGR->NetworkMsgParse( Protocol, pMsg );				break;
	}
}

void CGameIn::OnDisconnect()
{
	m_bDisconnected = TRUE;
	if( cMsgBox::IsInit() )	//°OAOA¤AoCE¿a? confirm
	{
		// 070122 LYW --- Modified this line.
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_SERVERDISCONNECT, MBT_OK, CHATMGR->GetChatMsg( 212 ) );
		//cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_FAILED_NPC_CHANGE_MAP, MBT_OK, CHATMGR->GetChatMsg( 212 ) );
	}
	else
	{
		MessageBox( NULL, "Disconnected To Server.\nThe Game is Closing.", 0, 0 );
		//MAINGAME->SetGameState(eGAMESTATE_END);
		PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
	}
}

void CGameIn::ProcessInBossMap()
{
//	if(	MAP->GetMapNum() != BOSSMONSTER_MAP && MAP->GetMapNum() != BOSSMONSTER_2NDMAP )	return;
	if( !m_bDieInBossMap )			return;
	
	if( gCurTime > m_dwTimeInBossMap )
	{
		m_dwTimeInBossMap = gCurTime + 1000;
		// 070129 LYW --- Map change : Modified message number.
		//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(930), m_nCountInBossMap );
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(393), m_nCountInBossMap );
		--m_nCountInBossMap;
	}
	if( m_nCountInBossMap <= 0 )
		m_bDieInBossMap = FALSE;
}


//캐릭터 서버 로그인 시간
void	CGameIn::SetLoginTime(SYSTEMTIME Time)
{
	GameinServerTime = Time;
}

SYSTEMTIME	CGameIn::GetLoginTime()
{
	return GameinServerTime;
}

//캐릭터 클라이언트 로그인 시간
void	CGameIn::GetClientLoginTime(DWORD& date, DWORD& time)
{
	date = m_Clientdate;
	time = m_Clienttime;
}

void	CGameIn::SetClientLoginTime(DWORD date, DWORD time)
{
	m_Clientdate = date;
	m_Clienttime = time;
}


void CGameIn::UserConn_NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol)
	{
	case MP_USERCONN_OBJECT_REMOVE:				UserConn_Object_Remove( pMsg ) ;							break;
	case MP_USERCONN_MONSTER_DIE:				UserConn_Monster_Die( pMsg ) ;								break;
	case MP_USERCONN_NPC_DIE:					UserConn_Monster_Die( pMsg ) ;								break;
	case MP_USERCONN_CHARACTER_DIE:				UserConn_Character_Die( pMsg ) ;							break;		
	// 070417 LYW --- GameIn : Add network message down exp and level.
	case MP_USERCONN_CHARACTER_APPLYEXP_NOTICE :	UserConn_ApplyExp( pMsg ) ;								break;
	case MP_USERCONN_CHARACTER_DOWNEXP_NOTICE :		UserConn_DownExp( pMsg ) ;								break;
	case MP_USERCONN_CHARACTER_DOWNLEVEL_NOTICE :	UserConn_DownLevel( pMsg ) ;							break;
	case MP_USERCONN_READY_TO_REVIVE:			UserConn_Ready_To_Revive() ;								break;
	case MP_USERCONN_CHARACTER_REVIVE:			UserConn_Character_Revive( pMsg ) ;							break;		
	case MP_USERCONN_CHARACTER_REVIVE_NACK:		UserConn_Character_Revive_Nack( pMsg ) ;					break;
	case MP_USERCONN_NPC_ADD:					UserConn_Npc_Add( pMsg ) ;									break;
	case MP_USERCONN_GRIDINIT:					UserConn_GridInit() ;										break;
	case MP_USERCONN_GAMEIN_ACK:				UserConn_GameIn_Ack( pMsg ) ;								break;
	case MP_USERCONN_GAMEIN_NACK:				__asm int 3;												break;
	case MP_USERCONN_CHARACTER_ADD:				UserConn_Character_Add( pMsg ) ;							break;
	case MP_USERCONN_MONSTER_ADD:				UserConn_Monster_Add( pMsg ) ;								break;	
	case MP_USERCONN_BOSSMONSTER_ADD:			UserConn_BossMonster_Add( pMsg ) ;							break;	
	case MP_USERCONN_CHANGEMAP_ACK:				UserConn_ChangeMap_Ack( pMsg ) ;							break;
	// 070918 LYW --- GameIn : Add process to change map through the npc.
	case MP_USERCONN_NPC_CHANGEMAP_ACK :		UserConn_Npc_ChangeMap_Ack( pMsg ) ;						break;
	case MP_USERCONN_NPC_CHANGEMAP_NACK :		UserConn_Npc_ChangeMap_Nack() ;									break;
	case MP_USERCONN_CHEAT_CHANGEMAP_ACK:		UserConn_Cheat_ChangeMap_Ack( pMsg ) ;						break;					
	case MP_USERCONN_CHANGEMAP_NACK:			UserConn_ChangeMap_Nack() ;									break;
	case MP_USERCONN_CHARACTERLIST_ACK:			UserConn_CharacterList_Ack( pMsg ) ;						break;
	case MP_USERCONN_BACKTOCHARSEL_ACK:			MAINGAME->SetGameState( eGAMESTATE_CHARSELECT );			break;
	case MP_USERCONN_BACKTOCHARSEL_NACK:		UserConn_BackToCharSel_Nack() ;								break;
	case MP_USERCONN_MAPDESC:					UserConn_MapDesc( pMsg ) ;									break;
	case MP_USERCONN_SETVISIBLE:				UserConn_SetVisible( pMsg ) ;								break;
	case MP_USERCONN_MAP_OUT_TO_EVENTMAP:		UserConn_Map_Out_To_EventMap( pMsg ) ;						break;
	case MP_USERCONN_MAP_OUT_TO_EVENTBEFOREMAP:	UserConn_Map_Out_To_EventBeforeMap() ;						break;			
	case MP_USERCONN_EVENT_READY:				EVENTMAP->Ready();											break;
	case MP_USERCONN_EVENT_START:				EVENTMAP->Start();											break;
	case MP_USERCONN_EVENT_END:					EVENTMAP->End( (MSG_DWORD3*)pMsg );							break;					
	case MP_USERCONN_EVENTITEM_USE:				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(396) );	break;
	case MP_USERCONN_EVENTITEM_USE2:			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(396) );	break;
	case MP_USERCONN_CASTLEGATE_ADD:
		{
			//SEND_GATE_TOTALINFO* pmsg = (SEND_GATE_TOTALINFO*)pMsg;

			//SIEGEMGR->CreateCastleGate( pmsg );
		}
		break;
	case MP_USERCONN_READY_TO_REVIVE_BY_GFW:
		{
			if( MAP->GetMapNum() != EVENTMAPNUM )
			{
				cDialog* dialog = WINDOWMGR->GetWindowForID( GD_REVIVALDLG );
				ASSERT( dialog );

				dialog->SetActive( TRUE );
				break;
			}
			break;
		}
	case MP_USERCONN_PET_ADD:		UserConn_Pet_Add( pMsg );		break;
	case MP_USERCONN_PET_REMOVE:	UserConn_Pet_Remove( pMsg );	break;

	}
}


void CGameIn::UserConn_Object_Remove( void* pMsg )
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	CObject* pObject = OBJECTMGR->GetObject(pmsg->dwData);
	if(pObject == NULL)
	{
		return;
	}

	if(pObject->GetObjectKind() == eObjectKind_Player)
	{
		pObject->OnStartObjectState( eObjectState_Exit );
	}

#ifndef _RESOURCE_WORK_NOT_APPLY_

	APPEARANCEMGR->CancelAlphaProcess(pObject);
	APPEARANCEMGR->CancelReservation(pObject);
#endif

	OBJECTMGR->AddGarbageObject(pObject);
}

void CGameIn::UserConn_Monster_Die( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	CObject* pObject = OBJECTMGR->GetObject(pmsg->dwData2);
	if(!pObject)
		return;
	pObject->SetDieFlag();

	// Game event check
	if( pmsg->dwData1 == HERO->GetID() )
	{
		GAMEEVENTMGR->AddEvent(eGameEvent_Hunt);
	}
}


void CGameIn::UserConn_Character_Die( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	CObject* pObject = OBJECTMGR->GetObject(pmsg->dwData2); //dwData2 = playerID
	if(!pObject)
		return;
	pObject->SetDieFlag();
}

// 070418 LYW --- GameIn : Add function to notice apply exp.
void CGameIn::UserConn_ApplyExp( void* pMsg )
{
	if( !pMsg ) return ;

	// 080602 LYW --- Player : 경험치 수치 (__int32) 에서 (__int64) 사이즈로 변경 처리.
	//MSG_DWORD2* pmsg = NULL ;
	//pmsg = ( MSG_DWORD2* )pMsg ;

	MSG_DWORDEX2* pmsg = NULL ;
	pmsg = ( MSG_DWORDEX2* )pMsg ;

	if( !pmsg ) return ;

	// 080104 --- GameIn : [ ~% + ~% ]의 경험치~~  --->  ~%의 경험치를 얻었습니다.

	/*Status* pStatus = NULL ;
	pStatus = HERO->GetRateBuffStatus() ;

	ASSERT(pStatus) ;

	HERO->SetExpPoint( pmsg->dwData1 ) ;

	DWORD dwExp = pmsg->dwData2 ;

	if( pStatus->GetExp )
	{
		DWORD dwBonus = (dwExp/(100+pStatus->GetExp))*pStatus->GetExp ;

		DWORD dwDefaultExp = 0 ;
		dwDefaultExp = dwExp - dwBonus ;

		CHATMGR->AddMsg( CTC_GETEXP, CHATMGR->GetChatMsg(1242), dwExp, dwDefaultExp, dwBonus ) ;
	}
	else
	{
		CHATMGR->AddMsg( CTC_GETEXP, CHATMGR->GetChatMsg(120), dwExp ) ;
	}

	MSG_DWORD2* pmsg = ( MSG_DWORD2* )pMsg ;*/

	HERO->SetExpPoint( pmsg->dweData1 ) ;

	CHATMGR->AddMsg( CTC_GETEXP, CHATMGR->GetChatMsg(120), (float)pmsg->dweData2 ) ;
}

// 070418 LYW --- GameIn : Add function to notice down exp.
void CGameIn::UserConn_DownExp( void* pMsg )														// 경험치 하락 메시지를 처리하는 함수.
{
	// 080602 LYW --- Player : 경험치 수치 (__int32) 에서 (__int64) 사이즈로 변경 처리.
	//MSG_DWORD3* pmsg = ( MSG_DWORD3* )pMsg ;														// 메시지를 받는다.
	MSG_DWORDEX3* pmsg = ( MSG_DWORDEX3* )pMsg ;														// 메시지를 받는다.

	if(pmsg->dweData3)																				// dwData3가 TURE이면,
	{
		CHATMGR->AddMsg( CTC_GETEXP, CHATMGR->GetChatMsg(848), pmsg->dweData1 ) ;					// 제자리 부활 추가 경험치 하락 메시지를 출력한다.
	}
	else																							// dwData3가 FALSE이면,
	{
		CHATMGR->AddMsg( CTC_GETEXP, CHATMGR->GetChatMsg(849), pmsg->dweData1 ) ;					// 경험치 하락 메시지를 출력한다.
	}

	HERO->SetExpPoint(pmsg->dweData2) ;																// 플레이어의 경험치를 세팅한다.
}

// 070418 LYW --- GameIn : Add function to notice down level.
void CGameIn::UserConn_DownLevel( void* pMsg )														// 레벨 다운 메시지를 처리하는 함수.
{
	// 080602 LYW --- Player : 경험치 수치 (__int32) 에서 (__int64) 사이즈로 변경 처리.
	//MSG_DWORD4* pmsg = ( MSG_DWORD4* )pMsg ;														// 메시지를 받는다.
	MSG_DWORDEX4* pmsg = ( MSG_DWORDEX4* )pMsg ;														// 메시지를 받는다.

	HERO->SetLevel(pmsg->dweData1) ;																	// 플레이어의 레벨을 세팅한다.
	HERO->SetExpPoint(pmsg->dweData2) ;																// 플레이어의 경험치를 세팅한다.

	if(pmsg->dweData4)																				// dwData4가 TRUE 이면,
	{
		CHATMGR->AddMsg( CTC_GETEXP, CHATMGR->GetChatMsg(848), pmsg->dweData3 ) ;					// 제자리 부활 추가 경험치 하락 메시지를 출력한다.
	}
	else																							// dwData4가 FALSE이면,
	{
		CHATMGR->AddMsg( CTC_GETEXP, CHATMGR->GetChatMsg(849), pmsg->dweData3 ) ;					// 경험치 하락 메시지를 출력한다.
	}

	CHATMGR->AddMsg( CTC_GETEXP, CHATMGR->GetChatMsg(657)) ;										// 레벨다운 메시지를 출력한다.

	// 080310 LUJ, 스킬 창의 정보를 갱신한다
	{
		cSkillTrainingDlg* dialog = GAMEIN->GetSkillTrainingDlg();

		if( dialog &&
			dialog->IsActive() )
		{
			dialog->OpenDialog();
			dialog->SetSkillInfo();
		}
	}
}

void CGameIn::UserConn_Ready_To_Revive()
{
	if( MAP->GetMapNum() == EVENTMAPNUM )
		return;
	GAMEIN->GetReviveDialog()->SetActive(TRUE);

/*	if( MAP->GetMapNum() == BOSSMONSTER_MAP || MAP->GetMapNum() == BOSSMONSTER_2NDMAP )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(392) );

		m_bDieInBossMap = TRUE;
		m_dwTimeInBossMap = gCurTime + 50000;
		m_nCountInBossMap = 10;
	}
*/
}


void CGameIn::UserConn_Character_Revive( void* pMsg )									// 캐릭터 부활을 처리하는 함수.
{
	MOVE_POS* pmsg = (MOVE_POS*)pMsg;													// 메시지를 받는다.
	VECTOR3 pos;																		// 위치를 나타내는 벡터를 선언한다.

	CObject* pObject = OBJECTMGR->GetObject(pmsg->dwMoverID);							// 메시지의 아이디로 오브젝트를 받는다.

	if( pObject )																		// 오브젝트 정보가 유효한지 체크한다.
	{
		pmsg->cpos.Decompress(&pos);													// 부활 위치 정보를 받는다.

		OBJECTACTIONMGR->Revive(pObject,&pos);											// 정해진 위치로 오브젝트를 부활시킨다.

		if( pObject == HERO )															// 오브젝트 정보가 본인(HERO)와 같으면,
		{
			GAMEIN->GetReviveDialog()->SetDisable(FALSE);								// 부활창의 비활성화를 해제하고, 
			GAMEIN->GetReviveDialog()->SetActive(FALSE);								// 화면에서 지운다.
			
			cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_REVIVECONFIRM );				// 윈도우 매니져에서 부활 메시지 창을 받는다.

			if( pDlg )																	// 부활 메시지 창이 정보가 유효하면,
			{
				WINDOWMGR->AddListDestroyWindow( pDlg );								// 윈도우 매니져에 파괴할 윈도우 목록으로 메시지 창을 추가한다.
			}

			m_bDieInBossMap = FALSE;													// 보스맵에서 죽음 여부를 FALSE로 세팅한다.
		}
	}
}


void CGameIn::UserConn_Character_Revive_Nack( void* pMsg )
{
	MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
	GAMEIN->GetReviveDialog()->SetDisable(FALSE);
}


void CGameIn::UserConn_Npc_Add( void* pMsg )
{
	SEND_NPC_TOTALINFO* pmsg = (SEND_NPC_TOTALINFO*)pMsg;
			
	BASEMOVE_INFO moveInfo;
	moveInfo.AddedMoveSpeed = 0.f;
	moveInfo.SetFrom(&pmsg->MoveInfo);
	CAddableInfoIterator iter(&pmsg->AddableInfo);
	BYTE AddInfoKind;
	while((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
	{
		ySWITCH(AddInfoKind)
			yCASE(CAddableInfoList::MoveInfo)
				COMPRESSEDTARGETPOS tpos;
				iter.GetInfoData(&tpos);
				moveInfo.bMoving = TRUE;
				moveInfo.SetMaxTargetPosIdx(tpos.PosNum);
				ASSERT( tpos.PosNum <= MAX_CHARTARGETPOSBUF_SIZE );
				moveInfo.SetCurTargetPosIdx(0);
				for(int n=0;n<tpos.PosNum;++n)
					tpos.pos[n].Decompress(moveInfo.GetTargetPosition(n));
			yCASE(CAddableInfoList::ShowdownInfo)
			yCASE(CAddableInfoList::StreetStall)
		yENDSWITCH
		
		iter.ShiftToNextData();
	}				
	
	CNpc* pNpc = OBJECTMGR->AddNpc(&pmsg->BaseObjectInfo,&moveInfo,&pmsg->TotalInfo);
	MOVEMGR->SetAngle(pNpc,pmsg->Angle,0);

	// Npc Add
	QUESTMGR->SetNpcData( pNpc );

	if(pmsg->bLogin)
	{
		TARGETSET set;
		set.pTarget = pNpc;
		//EFFECTMGR->StartEffectProcess(eEffect_MonRegen,pMon,&set,0,pMon->GetID());
		EFFECTMGR->StartEffectProcess(eEffect_NewCharacter,pNpc,&set,0,pNpc->GetID());
	}
}


void CGameIn::UserConn_GridInit()
{
	DeleteFile("DEBUG.txt");
	m_bGameInAcked = TRUE;

	g_UserInput.GetKeyboard()->ClearKeyState();
	g_UserInput.GetKeyboard()->UpdateKeyboardState();
	g_UserInput.SetInputFocus( TRUE );
	USERINFOMGR->LoadUserInfo( eUIK_USERSTATE );
}


void CGameIn::UserConn_GameIn_Ack( void* pMsg )
{
	ASSERT(!m_bGameInAcked);

	SEND_HERO_TOTALINFO * pmsg = (SEND_HERO_TOTALINFO *)pMsg;
	ASSERT(!(pmsg->SendMoveInfo.CurPos.wx == 0 && pmsg->SendMoveInfo.CurPos.wz == 0));

	//캐릭터 서버 로그인시간, 캐릭터 클라이언트 로그인시간 기록
	GAMEIN->SetLoginTime(pmsg->ServerTime);
	GAMEIN->SetClientLoginTime(MHTIMEMGR->GetMHDate(), MHTIMEMGR->GetMHTime());

	OBJECTMGR->RegisterHero(pmsg);

	//OPTIONMGR->UpdateHideFlagFromDB(pmsg->ChrTotalInfo.HideParts);
	//090218 modified by xb
	
	GAMEIN->GetMiniMapDialog()->AddHeroIcon(HERO);

	EFFECTMGR->StartHeroEffectProcess(eEffect_NewCharacter);

	//option setting
	OPTIONMGR->ApplySettings();	//½A¾ß..
	OPTIONMGR->SendOptionMsg();

	FRIENDMGR->LogInNotify();
	
	VECTOR2 sidpos;
	sidpos.x = 260;
	sidpos.y = 0;
	STATUSICONDLG->Init(HERO,&sidpos,8);

	// event add
	GAMEEVENTMGR->AddEvent(eGameEvent_MapChange, MAP->GetMapNum());
	
	QUESTMGR->ProcessNpcMark();

#ifdef TAIWAN_LOCAL
	if( m_GameInInitKind == eGameInInitKind_Login )
		BILLINGMGR->DisplayRemainTime();
#endif

	// eventmap
	EVENTMAP->AddHero();

	//event notify
	if( GAMEIN->GetEventNotifyDialog() )
	{
		if( NOTIFYMGR->IsEventNotifyUse() && ( NOTIFYMGR->IsEventNotifyChanged() || GAMEIN->GetEventNotifyDialog()->IsActive() ) )
		{
			GAMEIN->GetEventNotifyDialog()->SetTitle( NOTIFYMGR->GetEventNotifyTitle() );
			GAMEIN->GetEventNotifyDialog()->SetContext( NOTIFYMGR->GetEventNotifyContext() );
			GAMEIN->GetEventNotifyDialog()->SetActive( TRUE );
			//사운드?
			NOTIFYMGR->SetEventNotifyChanged( FALSE );
		}
		else
		{
			GAMEIN->GetEventNotifyDialog()->SetActive( FALSE );
		}
	}

	//ITEMMGR->RefreshStatsBuffIcon();

	CAMERA->Init(HERO,30,0,1000);
	CAMERA->SetTargetAngleX(0,29.9f,0);

	// 070313 LYW --- GameIn : Initialize hero's eye.
	HERO->InitializeEye() ;

	// 071227 LUJ
	HERO->SetStorageNum( pmsg->StorageSize );


	// 받아온 데이트 매칭 정보 ui에 기록.
//	if( m_pDateMatchingDlg)
//		m_pDateMatchingDlg->SetMyInfo( pmsg);
	// 070607 LYW --- GameIn : Setting matting info.
 	if( &pmsg->ChrTotalInfo.DateMatching.bIsValid )
	{
		GAMEIN->GetDateMatchingDlg()->UpdateMyInfo(&pmsg->ChrTotalInfo.DateMatching) ;
	}

	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.09.07
	// ..초기화
	g_csFarmManager.Init(MAP->GetMapNum());
	// E 농장시스템 추가 added by hseos 2007.09.07

	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.30
	g_csDateManager.Init();
	// E 데이트 존 추가 added by hseos 2007.11.30

	if( m_GameInInitKind == eGameInInitKind_Login )
	{
		MSGBASE msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_GIFT_EVENT_START;
		msg.dwObjectID = HERO->GetID();
		NETWORK->Send( &msg, sizeof( msg ) );
	}

	// 080418 LYW --- GameIn : 캐릭터의 채팅 상태를 초기화.
	//CHATROOMMGR->Set_ChatRoomState(e_State_Away_From_Chatroom) ;

	// 080502 LYW --- GameIn : 튜토리얼 중, 처음 게임에 접속 시 발생하는 튜토리얼 체크.
	if(!TUTORIALMGR->IsCompleteTutorial(e_TT_FirstGameIn))
	{
		TUTORIALMGR->Start_Specification_Tutorial(e_TT_FirstGameIn) ;
	}
}


void CGameIn::UserConn_Character_Add(void* pMsg )
{
	#ifndef _RESOURCE_WORK_NOT_APPLY_

	SEND_CHARACTER_TOTALINFO* pmsg = (SEND_CHARACTER_TOTALINFO*)pMsg;
	BASEMOVE_INFO moveInfo;	
	moveInfo.AddedMoveSpeed = 0.f;
	moveInfo.SetFrom(&pmsg->MoveInfo);

	CAddableInfoIterator iter(&pmsg->AddableInfo);
	BYTE AddInfoKind;
	while((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
	{
		ySWITCH(AddInfoKind)
			yCASE(CAddableInfoList::MoveInfo)
				COMPRESSEDTARGETPOS tpos;
				iter.GetInfoData(&tpos);
				moveInfo.bMoving = TRUE;
				moveInfo.SetMaxTargetPosIdx(tpos.PosNum);
				ASSERT( tpos.PosNum <= MAX_CHARTARGETPOSBUF_SIZE );
				moveInfo.SetCurTargetPosIdx(0);
				for(int n=0;n<tpos.PosNum;++n)
					tpos.pos[n].Decompress(moveInfo.GetTargetPosition(n));							
		yENDSWITCH
		
		iter.ShiftToNextData();
	}

	CPlayer* pPlayer = OBJECTMGR->AddPlayerThroughCache(&pmsg->BaseObjectInfo,&moveInfo,&pmsg->TotalInfo,&pmsg->ShopItemOption, pmsg);

	
	// Add Player가 완료된 시점에서 PostAddPlayer()가 호출됩니다. 
	// Effect등을 붙일 때 Player model 을 미리 불러버리는 통에 
	// 어쩔 수 없이 흐름을 바꿨습니다. 양해해 주십시오. 
					

	// eventmap
	EVENTMAP->AddPlayer( pPlayer );

#else

	SEND_CHARACTER_TOTALINFO* pmsg = (SEND_CHARACTER_TOTALINFO*)pMsg;
	BASEMOVE_INFO moveInfo;
	moveInfo.AddedMoveSpeed = 0.f;
	BOOL bShowdown = FALSE;
	BOOL bPartyWar = FALSE;
	WORD wPartyTeam = 0;
	WORD StallKind = 0;
	COMPRESSEDPOS ShowdownPos;
	static char StreetStallTitle[MAX_STREETSTALL_TITLELEN+1] = {0,};
	moveInfo.SetFrom(&pmsg->MoveInfo);
	CAddableInfoIterator iter(&pmsg->AddableInfo);
	BYTE AddInfoKind;
	while((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
	{
		ySWITCH(AddInfoKind)
			yCASE(CAddableInfoList::MoveInfo)
				COMPRESSEDTARGETPOS tpos;
				iter.GetInfoData(&tpos);
				moveInfo.bMoving = TRUE;
				moveInfo.SetMaxTargetPosIdx(tpos.PosNum);
				ASSERT( tpos.PosNum <= MAX_CHARTARGETPOSBUF_SIZE );
				moveInfo.SetCurTargetPosIdx(0);
				for(int n=0;n<tpos.PosNum;++n)
					tpos.pos[n].Decompress(moveInfo.GetTargetPosition(n));
			yCASE(CAddableInfoList::ShowdownInfo)
				bShowdown = TRUE;
				iter.GetInfoData(&ShowdownPos);
			yCASE(CAddableInfoList::StreetStall)
				StallKind = eSK_SELL;
				iter.GetInfoData(StreetStallTitle);
			yCASE(CAddableInfoList::StreetBuyStall)
				StallKind = eSK_BUY;
				iter.GetInfoData(StreetStallTitle);
			yCASE(CAddableInfoList::PartyWar)
				bPartyWar = TRUE;
				iter.GetInfoData( &wPartyTeam );
		yENDSWITCH
		
		iter.ShiftToNextData();
	}

	CPlayer* pPlayer = OBJECTMGR->AddPlayer(&pmsg->BaseObjectInfo,&moveInfo,&pmsg->TotalInfo,&pmsg->ShopItemOption);

	if( pmsg->TotalInfo.bPKMode )
	{
		//OBJECTEFFECTDESC desc(FindEffectNum("maintain_PK_S.beff"));
		OBJECTEFFECTDESC desc( 39 );	//---KES PK
		pPlayer->AddObjectEffect( PK_EFFECT_ID, &desc, 1 );
	}

	//SW051112 무쌍모드
//	if((pPlayer->GetCharacterTotalInfo()->bVisible == FALSE)
//		||(!pPlayer->GetSingleSpecialState(eSingleSpecialState_Hide)))
//	{
//		if( pmsg->TotalInfo.bMussangMode )
//		{
//			OBJECTEFFECTDESC desc(1035);
//			pPlayer->AddObjectEffect( MUSSANG_EFFECT_ID1, &desc, 1, pPlayer);
//		}
//	}
	//////////////////////////////////////////////////////////////////////////


	if(pmsg->bLogin)
	{
		TARGETSET set;
		set.pTarget = pPlayer;
		EFFECTMGR->StartEffectProcess(eEffect_NewCharacter,pPlayer,&set,0,pPlayer->GetID());
		pPlayer->ChangeBaseMotion( pPlayer->GetStandardMotion() );
	}
	else
	{
		if( pmsg->BaseObjectInfo.ObjectState == eObjectState_StreetStall_Owner )
		{
			if(StallKind == eSK_SELL)
				pPlayer->ShowStreetStallTitle( TRUE, StreetStallTitle );
			else if(StallKind == eSK_BUY)
				pPlayer->ShowStreetBuyStallTitle( TRUE, StreetStallTitle );
			
		}
	}

	// 해당 효과를 제거해달라는 기획팀 요구로 인해 제거
	//if( bShowdown )
	//{
	//	if( pmsg->BaseObjectInfo.BattleTeam == eBattleTeam1 )
	//	{
	//		OBJECTEFFECTDESC desc(FindEffectNum("maintain_dual_oneself_s.beff"));
	//		pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );	//SW HERO는 옵션 설정 무관하게 이펙트 생성을 위한..설정
	//	}
	//	else if( pmsg->BaseObjectInfo.BattleTeam == eBattleTeam2 )
	//	{
	//		OBJECTEFFECTDESC desc(FindEffectNum("maintain_dual_enemy_s.beff"));
	//		pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );
	//	}
	//}

	//if( bPartyWar )
	//{
	//	if( wPartyTeam == 1 )
	//	{
	//		OBJECTEFFECTDESC desc(FindEffectNum("maintain_dual_oneself_s.beff"));
	//		pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );
	//	}
	//	else if( wPartyTeam == 2 )
	//	{
	//		OBJECTEFFECTDESC desc(FindEffectNum("maintain_dual_enemy_s.beff"));
	//		pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );
	//	}
	//}

	// eventmap
	EVENTMAP->AddPlayer( pPlayer );

#endif
}

void CGameIn::UserConn_Monster_Add(void* pMsg )
{
	SEND_MONSTER_TOTALINFO* pmsg = (SEND_MONSTER_TOTALINFO*)pMsg;
	if(pmsg->TotalInfo.MonsterKind > 10000)
	{
		ASSERT(0);
	}

	BASEMOVE_INFO moveInfo;
	moveInfo.AddedMoveSpeed = 0.f;
	moveInfo.SetFrom(&pmsg->MoveInfo);
	CAddableInfoIterator iter(&pmsg->AddableInfo);
	BYTE AddInfoKind;
	while((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
	{
		ySWITCH(AddInfoKind)
			yCASE(CAddableInfoList::MoveInfo)
				COMPRESSEDTARGETPOS tpos;
				iter.GetInfoData(&tpos);
				moveInfo.bMoving = TRUE;
				moveInfo.SetMaxTargetPosIdx(tpos.PosNum);
				ASSERT( tpos.PosNum <= MAX_CHARTARGETPOSBUF_SIZE );
				moveInfo.SetCurTargetPosIdx(0);
				for(int n=0;n<tpos.PosNum;++n)
					tpos.pos[n].Decompress(moveInfo.GetTargetPosition(n));
			yCASE(CAddableInfoList::ShowdownInfo)
			yCASE(CAddableInfoList::StreetStall)
		yENDSWITCH
		
		iter.ShiftToNextData();
	}
	
#ifdef TAIWAN_LOCAL
	//임시로! 몬스터이름을 서버에서 안보내주도록 하자!
	SafeStrCpy( pmsg->BaseObjectInfo.ObjectName,
		GAMERESRCMNGR->GetMonsterListInfo( pmsg->TotalInfo.MonsterKind )->Name,
		MAX_NAME_LENGTH+1 );
#endif
	
	CMonster* pMon = OBJECTMGR->AddMonster(&pmsg->BaseObjectInfo,&moveInfo,&pmsg->TotalInfo);
						
	//////////////////////////////////////////////////////////////////////////

	if(pmsg->bLogin)
	{
		TARGETSET set;
		set.pTarget = pMon;
		//EFFECTMGR->StartEffectProcess(eEffect_MonRegen,pMon,&set,0,pMon->GetID());
		EFFECTMGR->StartEffectProcess(eEffect_NewCharacter,pMon,&set,0,pMon->GetID());
	}
}


void CGameIn::UserConn_BossMonster_Add(void* pMsg )
{
	SEND_MONSTER_TOTALINFO* pmsg = (SEND_MONSTER_TOTALINFO*)pMsg;
	if(pmsg->TotalInfo.MonsterKind > 10000)
	{
		ASSERT(0);
	}
	
	BASEMOVE_INFO moveInfo;
	moveInfo.AddedMoveSpeed = 0.f;
	moveInfo.SetFrom(&pmsg->MoveInfo);
	CAddableInfoIterator iter(&pmsg->AddableInfo);
	BYTE AddInfoKind;
	while((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
	{
		ySWITCH(AddInfoKind)
			yCASE(CAddableInfoList::MoveInfo)
				COMPRESSEDTARGETPOS tpos;
				iter.GetInfoData(&tpos);
				moveInfo.bMoving = TRUE;
				moveInfo.SetMaxTargetPosIdx(tpos.PosNum);
				ASSERT( tpos.PosNum <= MAX_CHARTARGETPOSBUF_SIZE );
				moveInfo.SetCurTargetPosIdx(0);
				for(int n=0;n<tpos.PosNum;++n)
					tpos.pos[n].Decompress(moveInfo.GetTargetPosition(n));
			yCASE(CAddableInfoList::ShowdownInfo)
			yCASE(CAddableInfoList::StreetStall)
		yENDSWITCH
		
		iter.ShiftToNextData();
	}
	
#ifdef TAIWAN_LOCAL
	//임시로! 몬스터이름을 서버에서 안보내주도록 하자!
	SafeStrCpy( pmsg->BaseObjectInfo.ObjectName,
		GAMERESRCMNGR->GetMonsterListInfo( pmsg->TotalInfo.MonsterKind )->Name,
		MAX_NAME_LENGTH+1 );
#endif
	
	CBossMonster* pMon = OBJECTMGR->AddBossMonster(&pmsg->BaseObjectInfo,&moveInfo,&pmsg->TotalInfo);
						
	//////////////////////////////////////////////////////////////////////////

	if(pmsg->bLogin)
	{
		TARGETSET set;
		set.pTarget = pMon;
		//EFFECTMGR->StartEffectProcess(eEffect_MonRegen,pMon,&set,0,pMon->GetID());
		//EFFECTMGR->StartEffectProcess(eEffect_NewCharacter,pMon,&set,0,pMon->GetID()); 

		// 080108 KTH -- 보스 몬스터의 등장 연출
		switch( pMon->GetMonsterKind() )
		{
		case eBOSSKIND_TARINTUS:
			{
				// 080108 KTH -- 등장 애니메이션을 설정해 준다.
				EFFECTMGR->StartEffectProcess(
					FindEffectNum("BOSS_Tarintus_Regen.beff"), 
					pMon, NULL, 0, pMon->GetID());
				// 080109 KTH -- 오브젝트를 선택되지 않도록 설정한다.
				pMon->GetEngineObject()->DisablePick();
				// 080109 KTH -- 등장할 동안의 시간을 정해 준다.
				pMon->SetOpenningTime(8000);

				break;
			}
			break;
		default:
			EFFECTMGR->StartEffectProcess(eEffect_NewCharacter,pMon,&set,0,pMon->GetID());
			break;
		}
	}
}


void CGameIn::UserConn_ChangeMap_Ack(void* pMsg )
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	DWORD map = pmsg->dwData;
	WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, MBT_NOBTN, RESRCMGR->GetMsg( 290 ) );
	MAPCHANGE->SetGameInInitKind(eGameInInitKind_MapChange);
	MAINGAME->SetGameState(eGAMESTATE_MAPCHANGE,&map,4);
	// 070623 LYW --- GameIn : SaveChatList.
	CHATMGR->SaveChatList() ;

	// 080411 LYW --- GameIn : 채팅방에서 채팅중이었다면, 채팅내용을 저장한다.
	CHATROOMMGR->SaveChat() ;
}

// 070918 LYW --- GameIn : Add function to change map through the npc.
void CGameIn::UserConn_Npc_ChangeMap_Ack(void* pMsg)
{
	if( !pMsg ) return ;

	cNpcScriptDialog* pDlg = GAMEIN->GetNpcScriptDialog() ;
	if( !pDlg ) return ;

	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg ;

	DWORD dwMapNum = pmsg->dwData ;

	WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, MBT_NOBTN, RESRCMGR->GetMsg( 290 ) );

	MAPCHANGE->SetGameInInitKind(eGameInInitKind_MapChange);
	MAINGAME->SetGameState(eGAMESTATE_MAPCHANGE,&dwMapNum,4);

	pDlg->EndDialog() ;
	pDlg->SetActive(FALSE) ;

	CHATMGR->SaveChatList() ;

	// 080411 LYW --- GameIn : 채팅방에서 채팅중이었다면, 채팅내용을 저장한다.
	CHATROOMMGR->SaveChat() ;
}


void CGameIn::UserConn_Cheat_ChangeMap_Ack(void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	DWORD map = pmsg->dwData1;

//---KES 채널이동 수정
	gChannelNum = pmsg->dwData2;
	gCheatMove = TRUE;
//--------------------
    

	// 071208 LYW --- GameIn : 맵 이동 스크롤 사용시, 맵 이동 하면서 이쪽으로 들어오기 때문에, 
	// 메시지 번호를 700 -> 206으로 수정한다.
	WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, MBT_NOBTN, CHATMGR->GetChatMsg( 206 ) );
	
	MAPCHANGE->SetGameInInitKind(eGameInInitKind_MapChange);
	MAINGAME->SetGameState(eGAMESTATE_MAPCHANGE,&map,4);
	CHATMGR->SaveChatList();

	// 080411 LYW --- GameIn : 채팅방에서 채팅중이었다면, 채팅내용을 저장한다.
	CHATROOMMGR->SaveChat() ;
}


void CGameIn::UserConn_ChangeMap_Nack()
{
	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(388) );
	g_UserInput.SetInputFocus(TRUE);
}

void CGameIn::UserConn_Npc_ChangeMap_Nack()
{
	WINDOWMGR->MsgBox( MBI_FAILED_NPC_CHANGE_MAP, MBT_OK, CHATMGR->GetChatMsg( 56 ) );
	g_UserInput.SetInputFocus(TRUE);

	cNpcScriptDialog* pDlg = GAMEIN->GetNpcScriptDialog() ;

	if( !pDlg ) return ;

	pDlg->EndDialog() ;
	pDlg->SetActive(FALSE) ;
}


void CGameIn::UserConn_CharacterList_Ack(void* pMsg )
{
	SEND_CHARSELECT_INFO* pmsg = (SEND_CHARSELECT_INFO*)pMsg;
	MAINGAME->SetGameState(eGAMESTATE_CHARSELECT, (void *)pmsg, sizeof(SEND_CHARSELECT_INFO));
	//ITEMMGR->DeleteShopItemInfo();
}


void CGameIn::UserConn_BackToCharSel_Nack()
{
	WINDOWMGR->AddListDestroyWindow( WINDOWMGR->GetWindowForID( MBI_NOBTNMSGBOX ) );
	OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Enter );//AO½A
}


void CGameIn::UserConn_MapDesc( void* pMsg )
{
	MSG_WORD3* pmsg = (MSG_WORD3*)pMsg;
	MAP->SetVillage( pmsg->wData1 );
	PKMGR->SetPKAllow( pmsg->wData2 );
	gChannelNum = (int)pmsg->wData3;
	GAMEIN->GetMiniMapDialog()->SetChannel(gChannelNum);
    	
	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.12.03
	// ..챌린지 존은 메세지를 따로 쓴다.
	if (g_csDateManager.IsChallengeZone(MAP->GetMapNum()))
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1221 ) );
		return;
	}
	// E 데이트 존 추가 added by hseos 2007.12.03

#ifdef TAIWAN_LOCAL					
	if( MAP->GetMapNum() == EVENTMAPNUM || MAP->GetMapNum() == Tournament || 
		MAP->GetMapNum() == QuestRoom )
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 394 ), GetMapName( MAP->GetMapNum() ) );
	else
	{
//		if( MAP->IsVillage() )
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 384 ), gChannelNum+1, GetMapName( MAP->GetMapNum() ) );
//		else
//			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 385 ), gChannelNum+1, GetMapName( MAP->GetMapNum() ) );
	}
#else
/*	if( MAP->GetMapNum() == EVENTMAPNUM || MAP->GetMapNum() == Tournament || 
		MAP->GetMapNum() == QuestRoom )
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 394 ), GetMapName( MAP->GetMapNum() ) );
	else
*/	{
//		if( MAP->IsVillage() )
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 384 ), GetMapName( MAP->GetMapNum() ), gChannelNum+1 );
//		else
//		{
//			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 385 ), GetMapName( MAP->GetMapNum() ), gChannelNum+1 );
//		}
	}

#endif

	if( PKMGR->IsPKAllow() )
	{
//		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 383 ) );
	}

	// 06. 03. 문파공지 - 이영준
//	if(HERO->GetGuildIdx() && GUILDMGR->GetGuildNotice())
//		CHATMGR->AddMsg(CTC_GUILD_NOTICE, CHATMGR->GetChatMsg(395), GUILDMGR->GetGuildNotice());
}


void CGameIn::UserConn_SetVisible( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	CObject* pObj = OBJECTMGR->GetObject(pmsg->dwData1);
	if( !pObj ) // AIºIºÐ¿¡¼­ NULL°ª AuA¶(E®AI¿a!)
	{
		ASSERT(0);
		return;
	}
	
	if(pObj->GetObjectKind() != eObjectKind_Player)
	{
		ASSERT(0);
		return;
	}

	CPlayer* ppla = (CPlayer*)pObj;
	if(pmsg->dwData2)
	{
#ifdef _GMTOOL_
		if( MAINGAME->GetUserLevel() <= eUSERLEVEL_GM )
			ppla->GetEngineObject()->SetAlpha( 1.0f );
		else
#endif						
			ppla->GetEngineObject()->Show();
		
		ppla->GetCharacterTotalInfo()->bVisible = true;
	}
	else
	{
#ifdef _GMTOOL_
		if( MAINGAME->GetUserLevel() <= eUSERLEVEL_GM )
			ppla->GetEngineObject()->SetAlpha( 0.3f );
		else
#endif						
			ppla->GetEngineObject()->HideWithScheduling();

		ppla->GetCharacterTotalInfo()->bVisible = false;
	}
}


void CGameIn::UserConn_Map_Out_To_EventMap( void* pMsg )
{
	MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
	DWORD map = pmsg->dwData1;

	EVENTMAP->SetData( pmsg->dwData1, pmsg->dwData2, pmsg->dwData3 );
	if( MAP->GetMapNum() != 58 )
		EVENTMAP->m_dwBeforeMapNum = MAP->GetMapNum();

	WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, MBT_NOBTN, CHATMGR->GetChatMsg( 390 ) );
	
	MAPCHANGE->SetGameInInitKind(eGameInInitKind_EventMapEnter);
	MAINGAME->SetGameState(eGAMESTATE_MAPCHANGE,&map,4);
	CHATMGR->SaveChatList();

	// 080411 LYW --- GameIn : 채팅방에서 채팅중이었다면, 채팅내용을 저장한다.
	CHATROOMMGR->SaveChat() ;
}


void CGameIn::UserConn_Map_Out_To_EventBeforeMap()
{
	WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, MBT_NOBTN, CHATMGR->GetChatMsg( 390 ) );

	MSG_WORD2 msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_MAP_OUT_WITHMAPNUM;
	msg.dwObjectID = HEROID;
	msg.wData1 = (WORD)EVENTMAP->m_dwBeforeMapNum;
	msg.wData2 = -1;

	NETWORK->Send(&msg, sizeof(msg));	
}


void CGameIn::NPC_NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol)
	{
	case MP_NPC_SPEECH_ACK:
		{
			Npc_Speech_Ack( pMsg ) ;
			// 071021 LYW --- GameIn : Check tutorial condition.
			TUTORIALMGR->Check_NpcTalk(pMsg) ;								// 대화를 시작한 npc를 체크하기 위한 함수.
		}
		break;
	case MP_NPC_SPEECH_NACK:
		{
			Npc_Speech_Nack( pMsg ) ;
		}
		break;
	case MP_NPC_CLOSEBOMUL_ACK:
		{
			Npc_CloseBomul_Ack() ;
		}
		break;
	case MP_NPC_DIE_ACK:
		{
			Npc_Die_Ack( pMsg ) ;
		}
		break;
		
	case MP_NPC_DOJOB_NACK:
		{
			Npc_DoJob_Nack() ;			
		}
		break;

	case MP_NPC_CHECK_HACK_ACK:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			NPCSCRIPTMGR->StartNpcBusiness(pmsg->dwData3);
		}
		break;
	case MP_NPC_CHECK_HACK_NACK:
		{
			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

			MSG_DWORD *pmsg = (MSG_DWORD*)pMsg;
			if(pmsg->dwData == 271) // bomul
			{
				NPCSCRIPTMGR->SetSelectedNpc(NULL);
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(391) );
			}
		}
		break;

	default : break ;
	}
}


void CGameIn::Npc_Speech_Ack( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	CNpc* pNpc = (CNpc*)OBJECTMGR->GetObject(pmsg->dwData1);

	if( !pNpc )
	{
		ASSERT(0);
		if( HERO->GetState() == eObjectState_Deal )
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
		return;
	}

	switch( pNpc->GetNpcKind() )
	{
	case 30:
		{
			cMsgBox* pBox = WINDOWMGR->MsgBox( MBI_SELECTLOGINPOINT, MBT_YESNO, CHATMGR->GetChatMsg(381), pNpc->GetObjectName() );
			if( pBox )
				pBox->SetParam( pNpc->GetID() );

			GAMEEVENTMGR->AddEvent(eGameEvent_LoginPoint);
		}
		break;
	case 23:  // bomul
		{	
			NPCSCRIPTMGR->StartNpcScript( pNpc );
			GAMEEVENTMGR->AddEvent(eGameEvent_NpcClick);	
		}
		break; 
//	case 16:		// 공성맵 각인 상징물
//		{
//			CGuild* pGuild = NULL;
//			// 각인시작		
//			if( HERO->GetGuildMemberRank() != GUILD_MASTER )
//			{
//				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1010) );								
//				goto EngraveFailed;
//			}
//			pGuild = GUILDMGR->GetGuild();
//			if( pGuild && pGuild->GetLevel() != 5 )
//			{
//				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1011) );
//				goto EngraveFailed;
//			}
//			if( HERO->GetBattleTeam() != eBattleTeam2 )
//			{
//				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1057) );
//				goto EngraveFailed;
//			}
//			
//			WINDOWMGR->MsgBox( MBI_SW_ENGRAVE_SYN, MBT_YESNO, CHATMGR->GetChatMsg(1046) );
//			break;
//			
//EngraveFailed:
//			if( HERO->GetState() == eObjectState_Deal )
//					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
//		}
//		break;

	default:
		{
			NPCSCRIPTMGR->StartNpcScript( pNpc );
			GAMEEVENTMGR->AddEvent(eGameEvent_NpcClick);

			// 070326 LYW --- GameIn : Setting LastNpc.
			NPCSCRIPTMGR->SetLastNpc( pNpc ) ;
		}
		break;
	}
}


void CGameIn::Npc_Speech_Nack( void* pMsg )
{
	if( HERO->GetState() == eObjectState_Deal )
		OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

	MSG_DWORD *pmsg = (MSG_DWORD*)pMsg;
	if(pmsg->dwData == 271) // bomul
	{
		NPCSCRIPTMGR->SetSelectedNpc(NULL);
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(391) );
	}
}


void CGameIn::Npc_CloseBomul_Ack()
{
	if(GetNpcScriptDialog()->IsActive())
	{
		NPCSCRIPTMGR->SetSelectedNpc(NULL);
		GetNpcScriptDialog()->SetActive(FALSE);						
	}
}


void CGameIn::Npc_Die_Ack( void* pMsg )
{
	MSG_DWORD * pmsg = (MSG_DWORD*)pMsg;
	CNpc* pNpc = NPCSCRIPTMGR->GetSelectedNpc();
	if(pNpc)
	{
		if(pNpc->GetID() == pmsg->dwData)
		{
			if(GetNpcScriptDialog()->IsActive())
			{
				NPCSCRIPTMGR->SetSelectedNpc(NULL);
				GetNpcScriptDialog()->SetActive(FALSE);						
			}
		}
	}
}


void CGameIn::Npc_DoJob_Nack()
{
	NPCSCRIPTMGR->SetSelectedNpc(NULL);
	GetNpcScriptDialog()->SetActive(FALSE);						
	if( HERO->GetState() == eObjectState_Deal )
		OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
}


void CGameIn::SIGNAL_NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol)
	{
	case MP_SIGNAL_COMMONUSER:
		{
			Signal_CommonUser( pMsg ) ;
		}
		break;
	case MP_SIGNAL_USER_ADD_ITEM:
		{
			Signal_UserAddItem( pMsg ) ;
		}
		break;
	case MP_SIGNAL_ONEUSER:
		break;
	case MP_SIGNAL_SYSTEM:
		break;
	case MP_SIGNAL_BATTLE:	//AO½A
//				{
//					TESTMSG *data = (TESTMSG *)pMsg;
//					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(24), data->Msg );
//				}
		break;
	case MP_SIGNAL_SHOWDOWN_RESULT:	//AO½A
		{
			Signal_ShowDown_Result( pMsg ) ;
		}
		break;				
	}
}


void CGameIn::Signal_CommonUser( void* pMsg )
{
	TESTMSG *data = (TESTMSG *)pMsg;
			
	CHATMGR->AddMsg( CTC_OPERATOR, data->Msg );
}
// 081114 KTH -- 아이템 획득을 알림
void CGameIn::Signal_UserAddItem( void* pMsg )
{
	MSG_USER_ADD_ITEM* msg = (MSG_USER_ADD_ITEM*)pMsg;
	char MessageBuff[MAX_TEMPBUFF] = { 0, };

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(msg->dwIconIdx);
	char	szItemName[33];
	_parsingKeywordString(pItemInfo->ItemName, szItemName);

	//sprintf(MessageBuff, "[%s]님이 [%s] 아이템을 획득 하였습니다.", msg->CharacterName, szItemName);
	wsprintf(MessageBuff, CHATMGR->GetChatMsg(1267), msg->CharacterName, szItemName);

	CHATMGR->AddMsg( CTC_OPERATOR, MessageBuff );
}

void CGameIn::Signal_ShowDown_Result( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	CObject* pWinner = OBJECTMGR->GetObject(pmsg->dwData1);
	CObject* pLoser = OBJECTMGR->GetObject(pmsg->dwData2);
	if( pWinner == NULL || pLoser == NULL ) return;
	
	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(389),
	pWinner->GetObjectName(), pLoser->GetObjectName(), pWinner->GetObjectName() );
}

void CGameIn::UserConn_Pet_Add( void* pMsg )
{
	MSG_PET_ADD* pmsg = (MSG_PET_ADD*)pMsg;

	BASEMOVE_INFO moveInfo;
	moveInfo.AddedMoveSpeed = 0.f;
	moveInfo.SetFrom(&pmsg->MoveInfo);
	CAddableInfoIterator iter(&pmsg->AddableInfo);
	BYTE AddInfoKind;
	while((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
	{
		ySWITCH(AddInfoKind)
			yCASE(CAddableInfoList::MoveInfo)
			COMPRESSEDTARGETPOS tpos;
		iter.GetInfoData(&tpos);
		moveInfo.bMoving = TRUE;
		moveInfo.SetMaxTargetPosIdx(tpos.PosNum);
		ASSERT( tpos.PosNum <= MAX_CHARTARGETPOSBUF_SIZE );
		moveInfo.SetCurTargetPosIdx(0);
		for(int n=0;n<tpos.PosNum;++n)
			tpos.pos[n].Decompress(moveInfo.GetTargetPosition(n));
		yENDSWITCH

			iter.ShiftToNextData();
	}

	CPet* pPet = NULL;

	if( pmsg->MasterIdx == HEROID )
	{
		if( pmsg->IsLogin )
		{
			pPet = ( CPet* )OBJECTMGR->AddHeroPet( &( pmsg->BaseObjectInfo ), &moveInfo, &( pmsg->PetObjectInfo ), pmsg->ItemInfo );
		}
	}
	else
	{
		pPet = OBJECTMGR->AddPet( pmsg->MasterIdx, &( pmsg->BaseObjectInfo ), &moveInfo, &( pmsg->PetObjectInfo ), pmsg->ItemInfo );
	}

	if( pmsg->IsLogin )
	{
		TARGETSET set;
		set.pTarget = pPet;
		EFFECTMGR->StartEffectProcess(PETMGR->GetSummonEffect(),pPet,&set,0,pPet->GetID());
	}
}

void CGameIn::UserConn_Pet_Remove( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	CObject* pPet = OBJECTMGR->GetObject(pmsg->dwData1);
	CObject* pMaster = OBJECTMGR->GetObject(pmsg->dwData2);

	if( pPet == NULL )
	{
		return;
	}

	if( pMaster == HERO )
	{
		OBJECTMGR->RemoveHeroPet();
		return;
	}

#ifndef _RESOURCE_WORK_NOT_APPLY_

	APPEARANCEMGR->CancelAlphaProcess( pPet );
	APPEARANCEMGR->CancelReservation( pPet );
#endif

	OBJECTMGR->AddGarbageObject( pPet );
}


#ifdef _TESTCLIENT_

void InitTestHero()
{
	DWORD id = 1;
	{	// AOAI°ø
		SEND_HERO_TOTALINFO info;
		memset(&info,0,sizeof(info));
		info.BaseObjectInfo.dwObjectID = id++;
		strcpy( info.BaseObjectInfo.ObjectName, "DARK");
		VECTOR3 pos;
		pos.x = GAMERESRCMNGR->m_TestClientInfo.x;
		pos.z = GAMERESRCMNGR->m_TestClientInfo.z;
		info.SendMoveInfo.CurPos.Compress(&pos);
		info.ChrTotalInfo.Race = GAMERESRCMNGR->m_TestClientInfo.Race;
		info.ChrTotalInfo.Gender = GAMERESRCMNGR->m_TestClientInfo.Gender;
		info.ChrTotalInfo.JobGrade = 1;
		info.ChrTotalInfo.Job[0] = GAMERESRCMNGR->m_TestClientInfo.Job;
		info.ChrTotalInfo.Life = 1000;
		info.ChrTotalInfo.HairType = GAMERESRCMNGR->m_TestClientInfo.HairType;
		info.ChrTotalInfo.FaceType = GAMERESRCMNGR->m_TestClientInfo.FaceType;
		info.ChrTotalInfo.MaxLife = 1000;
		info.HeroTotalInfo.Mana = 1000;
		info.HeroTotalInfo.MaxMana = 1000;
		// 080626 LYW --- Hero : 최대 레벨 제한 수치가 숫자 였던 것을 Define 문으로 수정함.
		//info.ChrTotalInfo.Level = 99;
		info.ChrTotalInfo.Level = MAX_CHARACTER_LEVEL_NUM;
		info.ChrTotalInfo.Height = 1;
		info.ChrTotalInfo.Width = 1;
		info.ChrTotalInfo.WearedItemIdx[eWearedItem_Weapon] = GAMERESRCMNGR->m_TestClientInfo.WeaponIdx;
		info.ChrTotalInfo.WearedItemIdx[eWearedItem_Dress] = GAMERESRCMNGR->m_TestClientInfo.DressIdx;
		info.ChrTotalInfo.WearedItemIdx[eWearedItem_Hat] = GAMERESRCMNGR->m_TestClientInfo.HatIdx;
		info.ChrTotalInfo.WearedItemIdx[eWearedItem_Shoes] = GAMERESRCMNGR->m_TestClientInfo.ShoesIdx;
		info.ChrTotalInfo.WearedItemIdx[eWearedItem_Shield] = GAMERESRCMNGR->m_TestClientInfo.ShieldIdx;
		info.HeroTotalInfo.KyungGongGrade = 2604;
		info.ChrTotalInfo.bVisible = true;
/*		
		info.SkillTreeInfo.skill[0].dwDBIdx = 1;
		info.SkillTreeInfo.skill[0].bWear = FALSE;
		info.SkillTreeInfo.skill[0].ExpPoint = 500;
		info.SkillTreeInfo.skill[0].Position = TP_SKILL1_START;
		info.SkillTreeInfo.skill[0].QuickPosition = 0;
		info.SkillTreeInfo.skill[0].Level = 1;
		info.SkillTreeInfo.skill[0].wIconIdx = GAMERESRCMNGR->m_TestClientInfo.SkillIdx;
		
		info.SkillTreeInfo.skill[1].dwDBIdx = 2;
		info.SkillTreeInfo.skill[1].bWear = FALSE;
		info.SkillTreeInfo.skill[1].ExpPoint = 200;
		info.SkillTreeInfo.skill[1].Position = TP_SKILL1_START+1;
		info.SkillTreeInfo.skill[1].QuickPosition = 0;
		info.SkillTreeInfo.skill[1].Level = 0;
		info.SkillTreeInfo.skill[1].wIconIdx = 200;
		
		info.SkillTreeInfo.skill[2].dwDBIdx = 3;
		info.SkillTreeInfo.skill[2].bWear = FALSE;
		info.SkillTreeInfo.skill[2].ExpPoint = 1000;
		info.SkillTreeInfo.skill[2].Position = TP_SKILL1_START+2;
		info.SkillTreeInfo.skill[2].QuickPosition = 0;
		info.SkillTreeInfo.skill[2].Level = 2;
		info.SkillTreeInfo.skill[2].wIconIdx = 302;
*/
		OBJECTMGR->RegisterHero(&info);
		GAMEIN->GetMiniMapDialog()->AddHeroIcon(HERO);
		
		CAMERA->Init((CObject*)OBJECTMGR->GetHero(),30,0,1000);
		
		VECTOR2 sidpos;
		sidpos.x = 140;
		sidpos.y = 0;
		STATUSICONDLG->Init(HERO,&sidpos,5);


		BASEOBJECT_INFO obinfo;
		BASEMOVE_INFO moinfo;
		memset( &obinfo, 0, sizeof(obinfo) );		
		memset( &moinfo, 0, sizeof(moinfo) );

		obinfo.dwObjectID = 100;
		obinfo.dwUserID = 5572;
		sprintf( obinfo.ObjectName, "pet-01" );
		obinfo.BattleID = gChannelNum+2;

		moinfo.CurPosition = HERO->GetCurPosition();
		moinfo.CurPosition.x -= 150;

	
	//	CPetBase* pPet = OBJECTMGR->AddPet( &obinfo, &moinfo );
	//	HERO->SetPet( pPet );
	}
}


void InitTestMonster()
{
	DWORD id = 10;
	float x,z;
	x = GAMERESRCMNGR->m_TestClientInfo.x + 25;
	z = GAMERESRCMNGR->m_TestClientInfo.z + 500 + 25;


	DWORD temp =0;
	for(int n=0;n<GAMERESRCMNGR->m_TestClientInfo.MonsterNum;++n)
	{
		SEND_MONSTER_TOTALINFO info;
		memset(&info,0,sizeof(info));
		info.BaseObjectInfo.dwObjectID = id++;
		info.BaseObjectInfo.BattleID = gChannelNum+1;
		sprintf(info.BaseObjectInfo.ObjectName,"Monster%02d",temp++);
		VECTOR3 pos;
		pos.x = x;
		pos.z = z;
		info.MoveInfo.CurPos.Compress(&pos);
		BASEMOVE_INFO bminfo;
		bminfo.SetFrom(&info.MoveInfo);
		info.TotalInfo.MonsterKind = GAMERESRCMNGR->m_TestClientInfo.MonsterKind;
		g_pMonster = OBJECTMGR->AddMonster(&info.BaseObjectInfo,&bminfo,&info.TotalInfo);
		
		x+= 100;
	}

	temp =0;
	for(n=0;n<GAMERESRCMNGR->m_TestClientInfo.BossMonsterNum;++n)
	{
		SEND_MONSTER_TOTALINFO info;
		memset(&info,0,sizeof(info));
		info.BaseObjectInfo.dwObjectID = id++;
		sprintf(info.BaseObjectInfo.ObjectName,"BossMonster%02d",temp++);
		VECTOR3 pos;
		pos.x = x;
		pos.z = z;
		info.MoveInfo.CurPos.Compress(&pos);
		BASEMOVE_INFO bminfo;
		bminfo.SetFrom(&info.MoveInfo);
		info.TotalInfo.MonsterKind = GAMERESRCMNGR->m_TestClientInfo.BossMonsterKind;
		g_pBossMonster = OBJECTMGR->AddBossMonster(&info.BaseObjectInfo,&bminfo,&info.TotalInfo);
	//	g_pBossMonster->LoadBossMonster(GAMERESRCMNGR->m_TestClientInfo.BossInfofile);
		
		x+= 100;
	}

	x = GAMERESRCMNGR->m_TestClientInfo.x + 25;
	z -= 200;
	int lastnp = 0;
	for(n=0;n<GAMERESRCMNGR->m_TestClientInfo.CharacterNum;++n)
	{
		SEND_CHARACTER_TOTALINFO cinfo;
		memset(&cinfo,0,sizeof(cinfo));
		cinfo.BaseObjectInfo.dwObjectID = id++;
		sprintf(cinfo.BaseObjectInfo.ObjectName,"Character%02d",temp++);
		VECTOR3 pos;
		pos.x = x;
		pos.z = z;
		cinfo.MoveInfo.CurPos.Compress(&pos);
		BASEMOVE_INFO bminfo;
		bminfo.SetFrom(&cinfo.MoveInfo);
		cinfo.TotalInfo.bVisible = 1;
		cinfo.TotalInfo.Height = 1;
		cinfo.TotalInfo.Width = 1;
		cinfo.TotalInfo.bVisible = 1;
		cinfo.TotalInfo.Gender = 1;
		cinfo.TotalInfo.WearedItemIdx[eWearedItem_Dress] = 0;
		cinfo.TotalInfo.WearedItemIdx[eWearedItem_Weapon] = 0;
		OBJECTMGR->AddPlayer(&cinfo.BaseObjectInfo,&bminfo,&cinfo.TotalInfo,NULL);
		
		x+= 100;
		if(n/50 != lastnp)
		{
			lastnp = n/50;
			x = GAMERESRCMNGR->m_TestClientInfo.x + 25;
			z -= 200;
		}
	}
	
/*
	SEND_MONSTER_TOTALINFO info;
	memset(&info,0,sizeof(info));
	info.BaseObjectInfo.dwObjectID = id++;
	sprintf(info.BaseObjectInfo.ObjectName,"BossMonster%02d",temp++);
	info.BaseMoveInfo.CurPosition.x = x;
	info.BaseMoveInfo.CurPosition.z = z;
	info.TotalInfo.MonsterKind = 65;
	g_pBossMonster = OBJECTMGR->AddBossMonster(&info.BaseObjectInfo,&info.BaseMoveInfo,&info.TotalInfo);
*/
}

//trustpak
void InitTestCharacters(void)
{	
	DWORD id = 1000;
	DWORD temp =0;

	//for(n=0;n<GAMERESRCMNGR->m_TestClientInfo.CharacterNum;++n)
	DWORD n = 0;
	for (n = 0; n < GAMERESRCMNGR->m_TestClientInfo.dwTestCharacterNo;n++)
	{
		SEND_CHARACTER_TOTALINFO cinfo;
		memset(&cinfo,0,sizeof(cinfo));
		cinfo.BaseObjectInfo.dwObjectID = id++;
		sprintf(cinfo.BaseObjectInfo.ObjectName,"Test Character%02d",temp++);
		
		VECTOR3 pos;
		pos.x = GAMERESRCMNGR->m_TestClientInfo.aTestCharacterInfo[n].fX;
		pos.y = GAMERESRCMNGR->m_TestClientInfo.aTestCharacterInfo[n].fY;
		pos.z = GAMERESRCMNGR->m_TestClientInfo.aTestCharacterInfo[n].fZ;
		cinfo.MoveInfo.CurPos.Compress(&pos);

		BASEMOVE_INFO bminfo;
		bminfo.SetFrom(&cinfo.MoveInfo);
		cinfo.TotalInfo.bVisible = 1;
		cinfo.TotalInfo.Height = 1;
		cinfo.TotalInfo.Width = 1;
		cinfo.TotalInfo.bVisible = 1;
		cinfo.TotalInfo.WearedItemIdx[eWearedItem_Dress] = 23000 + rand()%150;
		cinfo.TotalInfo.WearedItemIdx[eWearedItem_Weapon] = 11000 + rand()%150;		
		cinfo.TotalInfo.Gender = GAMERESRCMNGR->m_TestClientInfo.aTestCharacterInfo[n].byGender;

		OBJECTMGR->AddPlayer(&cinfo.BaseObjectInfo,&bminfo,&cinfo.TotalInfo,NULL);		
		CObject * pObj = OBJECTMGR->GetObject(cinfo.BaseObjectInfo.dwObjectID);
		pObj->SetAngle(GAMERESRCMNGR->m_TestClientInfo.aTestCharacterInfo[n].fAngle);		
	}	
}

#endif

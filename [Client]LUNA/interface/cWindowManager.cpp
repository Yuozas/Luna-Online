#include "stdafx.h"
#include "cWindowManager.h"
#include "cScriptManager.h"
#include "GameResourceManager.h"
#include "../GlobalEventFunc.h"

#include "..\CharMake.h"
#include "..\MainDialog.h"
#include "..\GameIn.h"
#include "..\WindowIDEnum.h"
#include "..\ObjectManager.h"
#include "..\CharacterDialog.h"
#include "..\ItemManager.h"
#include "..\InventoryExDialog.h"
#include "..\MoneyDlg.h"
#include "cTabDialog.h"
#include "..\AlertDlg.h"
#include "cWindowFunc.h"
#include "../MousePointer.h"
#include "../input/cIMEWnd.h"
#include "../input/Mouse.h"
#include "cAnimationManager.h"
#include "../MiniMapDlg.h"
#include "../BigMapDlg.h"
#include "../MHMap.h"
#include "../cMsgBox.h"
#include "../cDivideBox.h"
#include "cWindowSystemFunc.h"
#include "mhFile.h"

// 061220 LYW --- Delete this code.
//#include "MainBarDialog.h"
#include "GuildCreateDialog.h"
#include "GuildDialog.h"
#include "GuildInviteDialog.h"
#include "GuildMarkDialog.h"
#include "GuildLevelUpDialog.h"
#include "GuildNickNameDialog.h"
#include "GuildRankDialog.h"
#include "GuildWarehouseRankDialog.h"
#include "GuildWarehouseDialog.h"
#include "StorageDialog.h"
#include "ChatOptionDialog.h"
#include "OptionDialog.h"
#include "CharStateDialog.h"
#include "MenuSlotDialog.h"

#include "NoteDialog.h"
#include "FriendDialog.h"
#include "MiniFriendDialog.h"
#include "MiniNoteDialog.h"
#include "ReviveDialog.h"
#include "ExchangeDialog.h"

// LYJ 051017 구입노점상 추가
#include "StallKindSelectDlg.h"
#include "StreetBuyStall.h"
#include "BuyRegDialog.h"

#include "StreetStall.h"
#include "NpcScriptDialog.h"
#include "HelpDialog.h"
#include "DealDialog.h"
#include "InventoryExDialog.h"
#include "EnchantDialog.h"
#include "MixDialog.h"
#include "DissolveDialog.h"
#include "ReinforceDlg.h"
#include "ApplyOptionDialog.h"
#include "ProgressDialog.h"
#include "WantNpcDialog.h"
#include "WantRegistDialog.h"
#include "QuestTotalDialog.h"
#include "QuestDialog.h"
#include "QuestQuickViewDialog.h"
#include "WantedDialog.h"
#include "JournalDialog.h"
#include "PartyCreateDlg.h"
#include "PartyInviteDlg.h"
#include "BailDialog.h"

#include "EventNotifyDialog.h"
#include "ItemShopDialog.h"
#include "MoveDialog.h"
#include "PointSaveDialog.h"

#include "./Input/UserInput.h"
#include "MouseCursor.h"
#include "StatusIconDlg.h"
#include "petstatusicondialog.h"

#include "GuildFieldWarDialog.h"

#include "SkillPointRedist.h"
#include "SkillPointNotify.h"

#include "PartyWarDialog.h"

#include "ShoutDialog.h"
#include "FishingDialog.h"
#include "FishingGaugeDialog.h"
#include "FishingPointDialog.h"
#include "ChaseDialog.h"
#include "ChaseinputDialog.h"
#include "NameChangeDialog.h"
#include "NameChangeNotifyDlg.h"

// Guild Tournament
#include "GTRegistDialog.h"
#include "GTRegistcancelDialog.h"
#include "GTStandingDialog.h"
#include "GTBattleListDialog.h"
#include "GTScoreInfoDialog.h"

// seigewar
#include "SeigeWarDialog.h"

#include "SealDialog.h"
#include "ChangeJobDialog.h"
#include "GuageDialog.h"
//#include "ReinforceResetDlg.h"
//#include "RareCreateDialog.h"

//#include "cJackpotDialog.h"
#include "ReinforceGuideDialog.h"
#include "TipBrowserDlg.h"
#include "KeySettingTipDlg.h"
#include "cIMEEx.h"

#include "GuildNoteDlg.h"
#include "UnionNoteDlg.h"

#include "GuildNoticeDlg.h"

#include "guildjoindialog.h"

// 061127 LYW --- Include New Chatting Dialog's Header File.
#include "ChattingDlg.h"
// 061130 LYW --- Include Outside Chatting Dialog's Header File.

// 061219 LYW --- Include main system dialog's header file.
#include "MainSystemDlg.h"

#include "MonsterGuageDlg.h"

// 070124 LYW --- Include heaer file.
#include "QuickSlotDlg.h"

#include "FamilyCreateDialog.h"
#include "FamilyDialog.h"
#include "FamilyInviteDialog.h"
#include "FamilyMarkDialog.h"
//#include "FamilyLevelUpDialog.h"
#include "FamilyNickNameDialog.h"
//#include "FamilyRankDialog.h"
//#include "FamilyWarehouseDialog.h"

#ifdef _HACK_SHIELD_
#include "HackShieldManager.h"
#endif

//screenshot
#include "ScreenShotDlg.h"

#include "GuildPlusTimeDialog.h"

#include "cSkillTreeDlg.h"
#include "NpcImageDlg.h"
#include "QuickDlg.h"

#include "DateMatchingDlg.h"			// 데이트 매칭.

#include "cSkillTrainingDlg.h"

// desc_hseos_몬스터미터01
// S 몬스터미터 추가 added by hseos 2007.05.30
#include "../hseos/Monstermeter/SHMonstermeterDlg.h"
// E 몬스터미터 추가 added by hseos 2007.05.30

#include "./Interface/GameNotifyManager.h"

#include "QuestManager.h"

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.08.24	2007.09.10	2007.10.23	
#include "../hseos/Farm/SHFarmBuyDlg.h"
#include "../hseos/Farm/SHFarmUpgradeDlg.h"
#include "../hseos/Farm/SHFarmManageDlg.h"
// E 농장시스템 추가 added by hseos 2007.08.24	2007.09.10	2007.10.23

// desc_hseos_데이트 존_01
// S 데이트 존 추가 added by hseos 2007.11.14  2007.11.15	2008.01.24
#include "../hseos/Date/SHDateZoneListDlg.h"
#include "../hseos/Date/SHChallengeZoneListDlg.h"
#include "../hseos/Date/SHChallengeZoneClearNo1Dlg.h"
// E 데이트 존 추가 added by hseos 2007.11.14  2007.11.15	2008.01.24

// 071017 LYW --- cWindowManager : Include tutorialdialog.
#include "TutorialDlg.h"
#include "ItemMallBtnDlg.h"
#include "ItemPopupDlg.h"

// 071017 LYW --- cWindowManager : Include Helper dialog.
#include "HelperDlg.h"

// 071023 LYW --- cWindowManager : Include tutorial button dialog.
#include "TutorialBtnDlg.h"

// 071201 LYW --- cWindowManager : Include map move dialog.
#include "cMapMoveDialog.h"

// 071227 LYW --- cWindowManager : 캐릭터 이름 변경 다이얼로그 추가.
#include "cChangeNameDialog.h"

// 080109 LYW --- cWindowManager : 숫자 패드 추가 처리.
#include "./Interface/cNumberPadDialog.h"


//---KES AUTONOTE
#include "AutoNoteDlg.h"
#include "AutoAnswerDlg.h"
//--------------

//---KES 상점검색
#include "StoreSearchDlg.h"
//---------------


//---BROWSER
#include "ItemShopDlg.h"

// 080414 LUJ, 외양 변경 창
#include "BodyChangeDialog.h"


// 080403 LYW --- cWindowManager : Include dialogs for chatroom system.
#include "./ChatRoomMainDlg.h"
#include "./ChatRoomCreateDlg.h"
#include "./ChatRoomDlg.h"
#include "./ChatRoomGuestListDlg.h"
#include "./ChatRoomOptionDlg.h"
#include "./ChatRoomJoinDlg.h"
//#include "./ChatRoomLobbyDlg.h"

#include "../hseos/Farm/FarmAnimalDlg.h"

#include "WeatherDialog.h"

#include "PetInfoDialog.h"
#include "PetStateDialog.h"
#include "PetWearedDialog.h"
#include "PetResurrectionDialog.h"
extern HWND _g_hWnd;

GLOBALTON(cWindowManager);


cWindowManager::cWindowManager()
{
//	m_pScriptManager	= NULL;				// SCRIPTMGR로 대체.
	m_pWindowList		= NULL;
	m_pIconDlgListRef	= NULL;
	m_pEditBoxListRef	= NULL;
	m_pDestroyWindowRef = NULL;
	m_pScreenTextList	= NULL;

	// 070124 LYW --- Add list for save id of actived.
	m_pActivedWindowList = NULL ;

	m_OldDragFlag		= FALSE;
//	ID_SEED = 0;	//?
//	m_CommonDlgIDIndex.Init(MAX_COMMONDLGID_NUM, 500);			//start index 5000 tmp
	m_pMousePointWindow = NULL;
	m_pGuageWindow		= NULL;
	m_pMonsterGuageWindow = NULL;
	m_fComposition		= FALSE;
	cbDragnDropProcess	= NULL;
	
	m_pFocusedEdit		= NULL;
	
	m_id			= -1;
	m_pDragDialog	= NULL;

//==== for rec.
	m_bHideInterface = FALSE;


	// 070124 LYW --- Add variable for check that opend all windows.
	m_bOpendAllWindows	 = TRUE ;

	m_bMouseInWindow = FALSE ;
}

cWindowManager::~cWindowManager()
{
//	Release();	//PJH 여기서 실행하면 두번 Release하게 된다
}
void cWindowManager::Release()
{
	DestroyWindowAll();
//	SAFE_DELETE(m_pScriptManager);			// SCRIPTMGR로 대체.
	SAFE_DELETE(m_pIconDlgListRef);
	SAFE_DELETE(m_pEditBoxListRef);
	SAFE_DELETE(m_pScreenTextList);
	SAFE_DELETE(m_pPlayerChatTooltip);
	SAFE_DELETE(m_pWindowList);
	SAFE_DELETE(m_pDestroyWindowRef);
	SAFE_DELETE(m_pMousePointWindow);
	SAFE_DELETE(m_pGuageWindow);
	SAFE_DELETE(m_pMonsterGuageWindow);

	// 070124 LYW --- Add list for save id of actived.
	SAFE_DELETE(m_pActivedWindowList) ;
	
	CFONT_OBJ->Release();
	RemoveFontResourceEx( "./Data/Interface/Windows/MSJH.ttf", FR_PRIVATE, 0 );
}

void cWindowManager::Init()
{
	AddFontResourceEx( "./Data/Interface/Windows/MSJH.ttf", FR_PRIVATE, 0 );
//	m_pScriptManager = new cScriptManager;		// SCRIPTMGR로 대체.
	m_pWindowList = new cPtrList;
	m_pEditBoxListRef = new cPtrList;
	m_pIconDlgListRef = new cPtrList;
	m_pDestroyWindowRef = new cPtrList;
	m_pScreenTextList = new cPtrList;
	m_pPlayerChatTooltip = new cPtrList;

	// 070124 LYW --- Add list for save id of actived.
	m_pActivedWindowList = new cPtrList ;
	
	LOGFONT	font;

	int hiegthadjustment = 0;
	int nNumerator = GetDeviceCaps(GetDC( _g_hWnd ), LOGPIXELSY) ;	// (LOGPIXELSY : 인치당 픽셀수, 즉 해상도를 조사한다.) DC가 참조하는 장치의 여러가지 속성이나 능력을 조사하여 조사된 값을 리턴한다.

	font.lfHeight = -MulDiv(8, nNumerator, 72);						// 첫번째 인수와 두번째 인수를 곱하고, 그 결과를 세번째 인수로 나눈 결과를 리턴해준다.(오버플로나, 나누기 값이 0인경우는 -1을리턴한다.)
	font.lfHeight -= hiegthadjustment;
	font.lfWidth = -font.lfHeight / 2;
	font.lfEscapement = 0;
	font.lfOrientation = 0;
	font.lfWeight = FW_NORMAL;
	font.lfItalic = 0;
	font.lfUnderline = 0;
	font.lfStrikeOut = 0;
	font.lfCharSet = HANGUL_CHARSET;
	font.lfOutPrecision = 0;
	font.lfClipPrecision = 0;
	font.lfQuality = CLEARTYPE_QUALITY;//CLEARTYPE_QUALITY
	font.lfPitchAndFamily = FF_MODERN;
	lstrcpy(font.lfFaceName,"Microsoft Jhenghei");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT0);

	//font.lfHeight = -18;
	//font.lfWidth = -font.lfHeight / 2;
	//lstrcpy(font.lfFaceName,"Microsoft Jhenghei");
	//CFONT_OBJ->CreateFontObject(&font, cFont::FONT8);

	font.lfHeight = -MulDiv(9, nNumerator, 72);
	font.lfWidth = -font.lfHeight / 2;
	font.lfUnderline = 1;
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT4);

	font.lfUnderline = 0;
	font.lfHeight = -14;
	font.lfHeight -= hiegthadjustment;
	font.lfWidth = -font.lfHeight / 2;
	font.lfWeight = FW_BOLD;
	lstrcpy(font.lfFaceName,"Microsoft Jhenghei");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT1);

	font.lfUnderline = 0;
	font.lfHeight = -14;
	font.lfHeight -= hiegthadjustment;
	font.lfWidth = -font.lfHeight / 2;
	font.lfWeight = 0;
	lstrcpy(font.lfFaceName,"Microsoft Jhenghei");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT2);
	
	font.lfHeight = -MulDiv(35, GetDeviceCaps(GetDC( _g_hWnd ), LOGPIXELSY), 72);
	font.lfHeight -= hiegthadjustment;
	font.lfWidth = -font.lfHeight / 2;
	lstrcpy(font.lfFaceName,"Microsoft Jhenghei");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT3);


		// 070329 LYW --- cWindowManager : Add font type.
	font.lfHeight = -MulDiv(14, GetDeviceCaps(GetDC( _g_hWnd ), LOGPIXELSY), 72);
	font.lfHeight -= hiegthadjustment;
	font.lfWidth = -font.lfHeight / 2;
	font.lfWeight = FW_BOLD ;
	lstrcpy(font.lfFaceName,"Microsoft Jhenghei");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT5);

    // 퀘스트 공지 출력용 폰트 추가.
	nNumerator = GetDeviceCaps(GetDC( _g_hWnd ), LOGPIXELSY) ;	// (LOGPIXELSY : 인치당 픽셀수, 즉 해상도를 조사한다.) DC가 참조하는 장치의 여러가지 속성이나 능력을 조사하여 조사된 값을 리턴한다.

	font.lfHeight = -MulDiv(11, nNumerator, 72);						// 첫번째 인수와 두번째 인수를 곱하고, 그 결과를 세번째 인수로 나눈 결과를 리턴해준다.(오버플로나, 나누기 값이 0인경우는 -1을리턴한다.)
	font.lfHeight -= hiegthadjustment;
	font.lfWidth = -font.lfHeight / 2;
	font.lfEscapement = 0;
	font.lfOrientation = 0;
	font.lfWeight = FW_BOLD;
	font.lfItalic = 0;
	font.lfUnderline = 0;
	font.lfStrikeOut = 0;
	font.lfCharSet = HANGUL_CHARSET;
	font.lfOutPrecision = 0;
	font.lfQuality = CLEARTYPE_QUALITY;
	font.lfPitchAndFamily = 0;
	lstrcpy(font.lfFaceName,"Microsoft Jhenghei");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT6);

	// 071017 LYW --- cWindowManager : Add font style to render tutorial text.
	nNumerator = GetDeviceCaps(GetDC( _g_hWnd ), LOGPIXELSY) ;	// (LOGPIXELSY : 인치당 픽셀수, 즉 해상도를 조사한다.) DC가 참조하는 장치의 여러가지 속성이나 능력을 조사하여 조사된 값을 리턴한다.

	font.lfHeight = -MulDiv(17, nNumerator, 72);						// 첫번째 인수와 두번째 인수를 곱하고, 그 결과를 세번째 인수로 나눈 결과를 리턴해준다.(오버플로나, 나누기 값이 0인경우는 -1을리턴한다.)
	font.lfHeight -= hiegthadjustment;
	font.lfWidth = -font.lfHeight / 2;
	font.lfEscapement = 0;
	font.lfOrientation = 0;
	font.lfWeight = FW_NORMAL;
	font.lfItalic = 0;
	font.lfUnderline = 0;
	font.lfStrikeOut = 0;
	font.lfCharSet = HANGUL_CHARSET;
	font.lfOutPrecision = 0;
	font.lfQuality = CLEARTYPE_QUALITY;
	font.lfPitchAndFamily = 0;
	lstrcpy(font.lfFaceName,"Microsoft Jhenghei");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT7);
//END FONT SETTING


//KES MESSAGEBOX 030827
	cMsgBox::InitMsgBox();
	//dividebox도?
}

void cWindowManager::CreatMousePoint()
{

}


void cWindowManager::CreateGameIn()
{
	// 071024 LYW --- cWindowManager : Re initialize dialogs of tutorial manager.
	TUTORIALMGR->Reset_MemberDlg() ;
#ifdef	_DEBUG
	DWORD	dwStartTick	=	GetTickCount();
#endif

	CreateMonsterGuageDlg();

	CreateCharDlg();
	CreateSkillTreeDlg();
	CreateInventoryDlg();
	CreateRecvExchangeDlg();
	
	CreateHeroGuage();
	CreateDealDlg();
	//CreateMixDlg();
	// 061204 LYW --- Delete Preexistence Chatting Dialog.
	/*
	CreateChatDlg();
	*/
	// SW050715
//	CreateJackpotDlg();

	// LYJ 051017 구입노점상 추가
	CreateStallKindSelectDlg();
	CreateStreetBuyStallDlg();	// 080219 KTH
	CreateBuyRegDlg();

	CreateStreetStallDlg();
	CreateNpcScriptDlg();
//	CreateHelpDlg();

	CreateQuickDlg();
	CreatePartyDlg();
	CreatePartyCreateDlg();
	CreatePartyInviteDlg();
	CreateGuildCreateDlg();
	CreateGuildNickNameDlg();
	CreateGuildRankDlg();
	CreateGuildDlg();
	CreateGuildInviteDlg();
	CreateGuildMarkDlg();
	CreateGuildLevelupDlg();
	CreateGuildWarehouseDlg();

	// 070831 웅주, 길드전 중 사망시 부활 안내
	{
		cWindow* dialog = GetDlgInfoFromFile( "Data/Interface/Windows/GuildRevival.bin", "rb" );
		ASSERT( dialog );

		AddWindow( dialog );
	}

	CreateMiniMapDlg();
	CreateStorageDlg();
	CreateFriendDlg();
	CreateMiniFriendDlg();
	CreateNoteDlg();
	CreateMiniNoteDlg();
	CreateReviveDlg();
//	CreateWantNpcDlg();
//	CreateWantRegistDlg();
	CreateQuestTotalDlg();
	CreateQuestquickDlg() ;

	CreateOptionDlg();
//	CreateEventNotifyDlg();

   	// 061127 LYW --- Add Create Chatting Dialog Function For New Chatting System.
	CreateChattingDlg() ;
	// 061219 LYW --- Add main system dialog.
	CreateMainSystemDlg() ;

//KES
#ifdef _TESTCLIENT_	
//	CreateMacroDlg();
//	CreateChatOptionDlg();
#endif

	CreateExitDlg();

	// desc_hseos_몬스터미터_01
	// S 몬스터미터 추가 added by hseos 2007.04.09
	CreateMonstermeterDlg();
	// E 몬스터미터 추가 added by hseos 2007.04.09

	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.08.24	2007.09.10	2007.10.23
	CreateFarmBuyDlg();
	CreateFarmUpgradeDlg();
	CreateFarmManageDlg();
	// E 농장시스템 추가 added by hseos 2007.08.24	2007.09.10	2007.10.23

	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.14  2007.11.15	2008.01.24
	CreateDateZoneListDlg();
	CreateChallengeZoneListDlg();
	CreateChallengeZoneClearNo1Dlg();
	// E 데이트 존 추가 added by hseos 2007.11.14  2007.11.15	2008.01.24

	CreateExchangeDlg();
//	CreateCharStateDlg();

	CreateMixDlg();
	CreateEnchantDlg();
	CreateDissolveDlg();
	CreateReinforceDlg();
	CreateApplyOptionDlg();
	CreateReinforceGuideDlg();
	CreateProgressDlog();

	// 06. 02. 강화 재료 안내 인터페이스 추가 - 이영준
	//CreateReinforceResetDlg();	// ?
	//CreateRareCreateDlg();		// ?

//KES 040510
//	CreateMenuSlotDlg();


	// RaMa
	CreateItemShopDlg();
//	CreateMoveDlg();
//	CreatePointSaveDlg();
//	CreateBailDlg();
	
//	CreateMallNotcieDlg();

	// guildfieldwar
	CreateGuildFieldWarDlg();

//	CreateSkPointDlg();
//	CreateSkPointNotifyDlg();

	// partywar
//	CreatePartyWarDlg();

//	CreateShoutDlg();
	// 061204 LYW --- Delete Preexistence Chatting Dialog.
	/*
	CreateShoutchatDlg();
	*/
//	CreateChaseDlg();
//	CreateChaseinputDlg();
//	CreateNameChangeDlg();
//	CreateNameChangeNotifyDlg();

	// Guild Tournament
//	CreateGTRegistDlg();
//	CreateGTRegistcancelDlg();
//	CreateGTStandingDlg();
//	CreateGTBattleListDlg();
//	CreateGTScoreInfoDlg();

	// seigewar
//	CreateSeigeWarDlg();

//	CreateItemSealDlg();

	//
	//CreateChangeJobDlg();


	// 06. 02. 내정보창 팁보기 추가 - 이영준
//	CreateTipBrowserDlgDlg();
	// 070614 LYW --- cWindowManager : Active key setting tip dialog.
	CreateKeySettingTipDlg();

	CreateGuildNoteDlg();
	CreateUnionNoteDlg();

	CreateGuildNoticeDlg();
	CreateGuildWarehouseRankDlg();

	//SW060713 문하생
	CreateGuildInvitationKindSelectionDlg();

	//screenshot
	// 070123 LYW --- Active this function.
	// 070124 LYW --- Delete this function.
	//ScreenShotDlg();

	// 2007/03/15 데이트 매칭 다이얼로그 추가-이진영
	CreateDateMatchingDlg();

	// 070605 LYW --- cWindowManager : Add function to create identification.
	CreateIdentificationDlg() ;

	// 070605 LYW --- cWindowManager : Add function to create favor icon dialog.
	CreateFavorIconDlg() ;

	CreateNpcImageDlg();

	//패밀리 시스템 구현

	CreateSkillTrainingDlg();

	CreateFamilyCreateDlg();
	CreateFamilyNickNameDlg();
//	CreateFamilyRankDlg();
	CreateFamilyDlg();
	CreateFamilyInviteDlg();
	CreateFamilyMarkDlg();
//	CreateFamilyLevelupDlg();
//	CreateFamilyWarehouseDlg();

	//CreateGameNotifyDlg() ;

	// 071017 LYW --- cWindowManager : Create tutorial dialog.
	CreateTutorialDlg() ;

	// 071017 LYW --- cWindowManager : Create Help link dialog.
	CreateHelpDlg() ;

    // 071023 LYW --- cWidnowManager : Create tutorial button dialog.
	CreateTutorialBtnDlg() ;
	CreateItemMallBtnDlg() ;
	CreateItemPopupDlg() ;

	// 071201 LYW --- cWindowManager : Create map move dialog.
	CreateMapMoveDlg() ;

	// 071227 LYW --- cWindowManager : 캐릭터 이름 변경 창 생성.
	CreateChangeNameDlg() ;

//---KES AUTONOTE
	CreateAutoNoteDlg();
	CreateAutoAnswerDlg();
//---------------
//---KES 상점검색
	CreateStoreSearchDlg();
//---------------

//---BROWSER
	//CreateItemShopBrowserDlg();

	CreateShoutDlg();	// 080313 NYJ --- 전체외치기 다이얼로그 추가

	// 080414 LUJ, 외양 변경 창 생성
	CreateBodyChangeDlg();

	// 080421 NYJ --- 낚시 다이얼로그
	CreateFishingDlg();
	CreateFishingGaugeDlg();
	CreateFishingPointDlg();

	// 080505 NYJ --- 날씨 다이얼로그
	CreateWeatherDlg();

// 080403 LYW --- cWindowManager : Add functions to create dialog for chatroom system.
	CreateChatRoomMainDlg() ;
	CreateChatRoomCreateDlg() ;
	CreateChatRoomDlg() ;
	CreateChatRoomGuestListDlg() ;
	CreateChatRoomOptionDlg() ;
	CreateChatRoomJoinDlg() ;
	//CreateChatRoomLobbyDlg() ;

	// 080509 KTH 
	CreateFarmAnimalDlg();

	CreatePetDialog();
#ifdef	_DEBUG
	DWORD	dwEndTick	=	GetTickCount();
	DWORD	dwFuncTick	=	dwEndTick - dwStartTick;
	char	buf[1024];
	sprintf( buf, "%s() spent %d Ticks\n", __FUNCTION__, dwFuncTick);
	OutputDebugString( buf);
#endif
}

void cWindowManager::CreatePartyWarDlg()
{
	cWindow* window = NULL;
	// 070116 LYW --- Modified bin file name.
	//window = GetDlgInfoFromFile( "Data/Interface/Windows/PartyWarDlg.bin", "rb" );
	window = GetDlgInfoFromFile( "Data/Interface/Windows/PartyWarSet.bin", "rb" );

	AddWindow( window );
	GAMEIN->SetPartyWarDlg( (CPartyWarDialog*)window );
	GAMEIN->GetPartyWarDlg()->Linking();
}

void cWindowManager::CreateGuildFieldWarDlg()
{
	cWindow* window = NULL;
	window = GetDlgInfoFromFile( "Data/Interface/Windows/GFWarDeclare.bin", "rb" );
	if( window)
	{
		AddWindow( window );
		GAMEIN->SetGFWarDeclareDlg( (CGFWarDeclareDlg*)window );
		GAMEIN->GetGFWarDeclareDlg()->Linking();
	}

	window = GetDlgInfoFromFile( "Data/Interface/Windows/GFWarResult.bin", "rb" );
	if( window)
	{
		AddWindow( window );
		GAMEIN->SetGFWarResultDlg( (CGFWarResultDlg*)window );
		GAMEIN->GetGFWarResultDlg()->Linking();
	}

	window = GetDlgInfoFromFile( "Data/Interface/Windows/GFWarInfo.bin", "rb" );
	if( window)
	{
		AddWindow( window );
		GAMEIN->SetGFWarInfoDlg( (CGFWarInfoDlg*)window );
		GAMEIN->GetGFWarInfoDlg()->Linking();
	}
	
	window = GetDlgInfoFromFile( "Data/Interface/Windows/GuildWarInfo.bin", "rb" );
	if( window)
	{
		AddWindow( window );
		GAMEIN->SetGuildWarInfoDlg( (CGuildWarInfoDlg*)window );
		GAMEIN->GetGuildWarInfoDlg()->Linking();
	}

	//Add 060803 by wonju-------------------------------------------
	window = GetDlgInfoFromFile( "Data/Interface/Windows/GuildPlusTime.bin", "rb" );
	if( window)
	{
		AddWindow( window );
		GAMEIN->SetGuildPlusTimeDlg( (CGuildPlusTimeDlg*)window );
		GAMEIN->GetGuildPlusTimeDlg()->Linking();
	}
	//---------------------------------------------------------------
}


void cWindowManager::CreateDissolveDlg()
{
	cWindow * window = GetDlgInfoFromFile( "Data/Interface/Windows/DissolveDialog.bin", "rb" );
	AddWindow(window);

	CDissolveDialog* pDlg = (CDissolveDialog*)window;
	GAMEIN->SetDissolveDialog( pDlg );
	pDlg->Linking();
}


void cWindowManager::CreateEnchantDlg()
{
	CEnchantDialog* window = ( CEnchantDialog* )GetDlgInfoFromFile("Data/Interface/Windows/EnchantDialog.bin", "rb");

	AddWindow(window);
	window->Linking();
}


void cWindowManager::CreateProgressDlog()
{
	CProgressDialog* window = ( CProgressDialog* )GetDlgInfoFromFile("Data/Interface/Windows/ProgressDialog.bin", "rb");

	AddWindow(window);
	window->Linking();
}


void cWindowManager::CreateReinforceDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/ReinforceDialog.bin", "rb");
	//	m_pGuageWindow = (cDialog *)window;
	AddWindow(window);
	GAMEIN->SetReinforceDialog((CReinforceDlg * )window);
	GAMEIN->GetReinforceDialog()->Linking();
}


void cWindowManager::CreateApplyOptionDlg()
{
	CApplyOptionDialog* window = ( CApplyOptionDialog* )GetDlgInfoFromFile( "Data/Interface/Windows/ApplyOptionDialog.bin", "rb" );

	if( window )
	{
		AddWindow( window );
		window->Linking();
	}
}


void cWindowManager::CreateMixDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/MixDialog.bin", "rb");
//	m_pGuageWindow = (cDialog *)window;
	AddWindow(window);
	GAMEIN->SetMixDialog((CMixDialog * )window);
	GAMEIN->GetMixDialog()->Linking();
}


void cWindowManager::CreateHeroGuage()
{
	// 070116 LYW --- Modified bin file name of CGuageDialog.
	//cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/char_main.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/CharGage.bin", "rb");
//	m_pGuageWindow = (cDialog *)window;
	AddWindow(window);
	GAMEIN->SetGuageDlg((CGuageDialog * )window);
	GAMEIN->GetGuageDlg()->Linking();
}

void cWindowManager::CreateQuickDlg()
{
	// 070117 LYW --- Modified bin file name.
	//cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/q_slot.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/QuickSlot.bin", "rb");
	ASSERT(window);
	AddWindow(window);

	cQuickDlg* pDlg = ( cQuickDlg* )window;
	pDlg->Linking();
	GAMEIN->SetQuickDlg( pDlg );
}

void cWindowManager::CreateMenuSlotDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/MenuSlot.bin", "rb");
	ASSERT(window);
	AddWindow(window);
	GAMEIN->SetMenuSlotDialog((CMenuSlotDialog * )window);
	GAMEIN->GetMenuSlotDialog()->Linking();
}

void cWindowManager::CreatePartyDlg() 
{
	// 070116 LYW --- Modified this part.
	/*
	cWindow * member1window = GetDlgInfoFromFile("Data/Interface/Windows/partymember1dlg.bin", "rb");
	cWindow * member2window = GetDlgInfoFromFile("Data/Interface/Windows/partymember2dlg.bin", "rb");
	cWindow * member3window = GetDlgInfoFromFile("Data/Interface/Windows/partymember3dlg.bin", "rb");
	cWindow * member4window = GetDlgInfoFromFile("Data/Interface/Windows/partymember4dlg.bin", "rb");
	cWindow * member5window = GetDlgInfoFromFile("Data/Interface/Windows/partymember5dlg.bin", "rb");
	cWindow * member6window = GetDlgInfoFromFile("Data/Interface/Windows/partymember6dlg.bin", "rb");
	cWindow * partybtnwindow = GetDlgInfoFromFile("Data/Interface/Windows/PartyBtnDialog.bin", "rb");
	*/
	cWindow * member1window = GetDlgInfoFromFile("Data/Interface/Windows/partymember1.bin", "rb");
	cWindow * member2window = GetDlgInfoFromFile("Data/Interface/Windows/partymember2.bin", "rb");
	cWindow * member3window = GetDlgInfoFromFile("Data/Interface/Windows/partymember3.bin", "rb");
	cWindow * member4window = GetDlgInfoFromFile("Data/Interface/Windows/partymember4.bin", "rb");
	cWindow * member5window = GetDlgInfoFromFile("Data/Interface/Windows/partymember5.bin", "rb");
	cWindow * member6window = GetDlgInfoFromFile("Data/Interface/Windows/partymember6.bin", "rb");
	cWindow * partybtnwindow = GetDlgInfoFromFile("Data/Interface/Windows/PartySet.bin", "rb");
	
	ASSERT(member1window);	ASSERT(member2window);	ASSERT(member3window);
	ASSERT(member4window);	ASSERT(member5window);	ASSERT(member6window);
	ASSERT(partybtnwindow);
	
	AddWindow(member1window);	AddWindow(member2window);	AddWindow(member3window);
	AddWindow(member4window);	AddWindow(member5window);	AddWindow(member6window);
	AddWindow(partybtnwindow);
	
	GAMEIN->GetPartyDialog()->RegistMemberDlg(0, (CPartyMemberDlg*)member1window);	
	GAMEIN->GetPartyDialog()->RegistMemberDlg(1, (CPartyMemberDlg*)member2window);
	GAMEIN->GetPartyDialog()->RegistMemberDlg(2, (CPartyMemberDlg*)member3window);
	GAMEIN->GetPartyDialog()->RegistMemberDlg(3, (CPartyMemberDlg*)member4window);
	GAMEIN->GetPartyDialog()->RegistMemberDlg(4, (CPartyMemberDlg*)member5window);
	GAMEIN->GetPartyDialog()->RegistMemberDlg(5, (CPartyMemberDlg*)member6window);

	GAMEIN->GetPartyDialog()->RegistBtnDlg((CPartyBtnDlg*)partybtnwindow);

} 

void cWindowManager::CreatePartyCreateDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/PartyCreate.bin", "rb");

	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetPartyCreateDialog((CPartyCreateDlg * )window);
	GAMEIN->GetPartyCreateDialog()->Linking();

}

void cWindowManager::CreatePartyInviteDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/PartyInvite.bin", "rb");

	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetPartyInviteDialog((CPartyInviteDlg * )window);
	GAMEIN->GetPartyInviteDialog()->Linking();

}

void cWindowManager::CreateGuildCreateDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/GuildCreate.bin", "rb");
	if( window)		// 임시로..
	{
		ASSERT(window);
		AddWindow(window);
		
		GAMEIN->SetGuildCreateDlg((CGuildCreateDialog * )window);
		GAMEIN->GetGuildCreateDlg()->Linking();
		
		window = GetDlgInfoFromFile("Data/Interface/Windows/GuildUnionCreate.bin", "rb");
		
		ASSERT(window);
		AddWindow(window);
		
		GAMEIN->SetGuildUnionCreateDlg((CGuildUnionCreateDialog * )window);
		GAMEIN->GetGuildUnionCreateDlg()->Linking();
	}
}

void cWindowManager::CreateGuildDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/Guild.bin", "rb");
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetGuildDlg((CGuildDialog * )window);
	GAMEIN->GetGuildDlg()->Linking();
}

void cWindowManager::CreateGuildInviteDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/GuildInvite.bin", "rb");
	if( window)
	{
		ASSERT(window);
		AddWindow(window);
		
		GAMEIN->SetGuildInviteDlg((CGuildInviteDialog * )window);
		GAMEIN->GetGuildInviteDlg()->Linking();
	}
}

void cWindowManager::CreateGuildMarkDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/GuildMark.bin", "rb");
	if( window)
	{
		ASSERT(window);
		AddWindow(window);
		
		GAMEIN->SetGuildMarkDlg((CGuildMarkDialog * )window);
		GAMEIN->GetGuildMarkDlg()->Linking();
	}
}

void cWindowManager::CreateGuildLevelupDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/GuildLevelUp.bin", "rb");
	if( window)
	{
		ASSERT(window);
		AddWindow(window);
		
		GAMEIN->SetGuildLevelUpDlg((CGuildLevelUpDialog * )window);
		GAMEIN->GetGuildLevelUpDlg()->Linking();
	}
}

void cWindowManager::CreateGuildNickNameDlg()
{
	CGuildNickNameDialog* dialog = ( CGuildNickNameDialog* )GetDlgInfoFromFile("Data/Interface/Windows/GuildNickName.bin", "rb");
	ASSERT( dialog );

	AddWindow( dialog );

	GAMEIN->SetGuildNickNameDlg( dialog );
	dialog->Linking();
}


void cWindowManager::CreateGuildRankDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/GuildRank.bin", "rb");
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetGuildRankDlg((CGuildRankDialog * )window);
	GAMEIN->GetGuildRankDlg()->Linking();
}

void cWindowManager::CreateGuildWarehouseDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/GuildWarehouse.bin", "rb");
	
	ASSERT(window);
	AddWindow(window);
		
	GAMEIN->SetGuildWarehouseDlg((CGuildWarehouseDialog * )window);
	GAMEIN->GetGuildWarehouseDlg()->Linking();
}

void cWindowManager::CreateStorageDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/Storage.bin", "rb");
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetStorageDialog((CStorageDialog * )window);
	GAMEIN->GetStorageDialog()->Linking();
}

void cWindowManager::CreateWantNpcDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/WantNpc.bin", "rb");
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetWantNpcDialog((CWantNpcDialog * )window);
	GAMEIN->GetWantNpcDialog()->Linking();
}

void cWindowManager::CreateWantRegistDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/WantRegist.bin", "rb");

	ASSERT(window);
	AddWindow(window);

	GAMEIN->SetWantRegistDialog((CWantRegistDialog * )window);
	GAMEIN->GetWantRegistDialog()->Linking();

}

void cWindowManager::CreateMiniMapDlg()
{
	// 070116 LYW --- Modified this line.
	//cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/minimap.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/MiniMap.bin", "rb");

	ASSERT(window);
	AddWindow(window);

	GAMEIN->SetMiniMapDialog((CMiniMapDlg * )window);
	GAMEIN->GetMiniMapDialog()->Linking();
	GAMEIN->GetMiniMapDialog()->InitMiniMap(MAP->GetMapNum());
	
	// 070116 LYW --- Modified this line.
	//cWindow * window2 = GetDlgInfoFromFile("Data/Interface/Windows/bigmap.bin", "rb");
	cWindow * window2 = GetDlgInfoFromFile("Data/Interface/Windows/BigMap.bin", "rb");

	ASSERT(window2);
	AddWindow(window2);

	GAMEIN->SetBigMapDialog((CBigMapDlg * )window2);
	if( GAMEIN->GetBigMapDialog() )
	{
		GAMEIN->GetBigMapDialog()->Linking();
		GAMEIN->GetBigMapDialog()->InitBigMap(MAP->GetMapNum());
	}
}

// 061127 LYW --- Add Create Chatting Dialog Function For New Chatting System.
void cWindowManager::CreateChattingDlg()
{
	cWindow * window = GetDlgInfoFromFile( "Data/Interface/Windows/ChatWindow.bin", "rb" ) ;
	AddWindow( window ) ;
	GAMEIN->SetChattingDlg( ( CChattingDlg* )window ) ;
	GAMEIN->GetChattingDlg()->Linking() ;
}

// 061219 LYW --- Add main system dialog.
void cWindowManager::CreateMainSystemDlg()
{
	cWindow* window = GetDlgInfoFromFile( "Data/Interface/Windows/Menu.bin", "rb" ) ;
	AddWindow( window ) ;
	GAMEIN->SetMainSystemDlg( ( CMainSystemDlg* )window ) ;

	GAMEIN->GetMainSystemDlg()->Linking() ;
}

void cWindowManager::CreateOptionDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/Option.bin", "rb");
	AddWindow(window);
	GAMEIN->SetOptionDialog((COptionDialog * )window);
	GAMEIN->GetOptionDialog()->Linking();
}

void cWindowManager::CreateExitDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/MainMenu.bin", "rb");
	AddWindow(window);
	GAMEIN->SetExitDialog((CExitDialog * )window);
}

// desc_hseos_몬스터미터_01
// S 몬스터미터 추가 added by hseos 2007.04.09
void cWindowManager::CreateMonstermeterDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/MonsterKill.bin", "rb");
	AddWindow(window);
	GAMEIN->SetMonstermeterDlg((CSHMonstermeterDlg * )window);
	GAMEIN->GetMonstermeterDlg()->Linking();
}
// E 몬스터미터 추가 added by hseos 2007.04.09

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.08.24	2007.09.10	2007.10.23
void cWindowManager::CreateFarmBuyDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/Farm_Get.bin", "rb");
	AddWindow(window);
	GAMEIN->SetFarmBuyDlg((CSHFarmBuyDlg * )window);
	GAMEIN->GetFarmBuyDlg()->Linking();
}

void cWindowManager::CreateFarmUpgradeDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/Farm_Upgrade.bin", "rb");
	AddWindow(window);
	GAMEIN->SetFarmUpgradeDlg((CSHFarmUpgradeDlg * )window);
	GAMEIN->GetFarmUpgradeDlg()->Linking();
}

void cWindowManager::CreateFarmManageDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/FarmManage.bin", "rb");
	AddWindow(window);
	GAMEIN->SetFarmManageDlg((CSHFarmManageDlg * )window);
	GAMEIN->GetFarmManageDlg()->Linking();
}
// E 농장시스템 추가 added by hseos 2007.08.24	2007.09.10	2007.10.23

// desc_hseos_데이트 존_01
// S 데이트 존 추가 added by hseos 2007.11.14  2007.11.15	2008.01.24
void cWindowManager::CreateDateZoneListDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/DateZoneListDlg.bin", "rb");
	AddWindow(window);
	GAMEIN->SetDateZoneListDlg((CSHDateZoneListDlg * )window);
	GAMEIN->GetDateZoneListDlg()->Linking();
}

void cWindowManager::CreateChallengeZoneListDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/ChallengeZoneListDlg.bin", "rb");
	AddWindow(window);
	GAMEIN->SetChallengeZoneListDlg((CSHChallengeZoneListDlg * )window);
	GAMEIN->GetChallengeZoneListDlg()->Linking();
}

void cWindowManager::CreateChallengeZoneClearNo1Dlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/ChallengeZoneClearNo1Dlg.bin", "rb");
	AddWindow(window);
	GAMEIN->SetChallengeZoneClearNo1Dlg((CSHChallengeZoneClearNo1Dlg * )window);
	GAMEIN->GetChallengeZoneClearNo1Dlg()->Linking();
}
// E 데이트 존 추가 added by hseos 2007.11.14  2007.11.15	2008.01.24

void cWindowManager::CreateCharStateDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/29.bin", "rb");
	AddWindow(window);
	GAMEIN->SetCharStateDialog((CCharStateDialog*)window);
	GAMEIN->GetCharStateDialog()->Linking();
}

void cWindowManager::CreateNoteDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/Note.bin", "rb");
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetNoteDialog((CNoteDialog * )window);
	GAMEIN->GetNoteDialog()->Linking();
}

void cWindowManager::CreateFriendDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/Friend.bin", "rb");
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetFriendDialog((CFriendDialog * )window);
	GAMEIN->GetFriendDialog()->Linking();
}


void cWindowManager::CreateMiniFriendDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/FriendInvite.bin","rb");
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetMiniFriendDialog((CMiniFriendDialog * )window);
	GAMEIN->GetMiniFriendDialog()->Linking();
}

void cWindowManager::CreateMiniNoteDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/NoteWrite.bin", "rb");
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetMiniNoteDialog((CMiniNoteDialog * )window);
	GAMEIN->GetMiniNoteDialog()->Linking();
}

void cWindowManager::CreateReviveDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/Revival.bin", "rb");
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetReviveDialog((CReviveDialog * )window);
	GAMEIN->GetReviveDialog()->Linking();
}

void cWindowManager::CreateCharDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/charInfo.bin", "rb");

	AddWindow(window);
	GAMEIN->SetCharacterDialog((CCharacterDialog * )window);
	GAMEIN->GetCharacterDialog()->Linking();
}
void cWindowManager::CreateSkillTreeDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/Skill.bin", "rb");
	AddWindow(window);
	
	cSkillTreeDlg* pSkillTreeDlg = ( cSkillTreeDlg* )window;
	GAMEIN->SetSkillTreeDlg( pSkillTreeDlg );
	pSkillTreeDlg->Linking();
}

void cWindowManager::CreateInventoryDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/Inven.bin", "rb");
	AddWindow(window);

	GAMEIN->SetInventoryDialog((CInventoryExDialog * )window);
	GAMEIN->GetInventoryDialog()->Linking();
}


//KES DIALOG 030918 //EXCHANGEDLG 
void cWindowManager::CreateExchangeDlg()
{
	cWindow* window = GetDlgInfoFromFile( "Data/Interface/Windows/Trade.bin", "rb" );
	AddWindow(window);
	GAMEIN->SetExchangeDialog( (CExchangeDialog*)window );
	GAMEIN->GetExchangeDialog()->Linking();
}


void cWindowManager::CreateRecvExchangeDlg()
{
	
}

// LYJ 051017 구입노점상 추가
void cWindowManager::CreateStallKindSelectDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/StallOption.bin", "rb");
	AddWindow(window);
	GAMEIN->SetStallKindSelectDialog((CStallKindSelectDlg * )window);
	GAMEIN->GetStallKindSelectDialog()->Linking();
}

void cWindowManager::CreateStreetBuyStallDlg()
{
	//cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/30in", "rb");
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/StallBuy.bin", "rb");
	AddWindow(window);
	GAMEIN->SetStreetBuyStallDialog((CStreetBuyStall * )window);
	GAMEIN->GetStreetBuyStallDialog()->Linking();
}
// 080219 KTH -- 
void cWindowManager::CreateBuyRegDlg()
{
	//cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/32.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/StallBuyList.bin", "rb");
	AddWindow(window);
	GAMEIN->SetBuyRegDialog((CBuyRegDialog * )window);
	GAMEIN->GetBuyRegDialog()->Linking();
}

//LBS 03.09.18
void cWindowManager::CreateStreetStallDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/StallSell.bin", "rb");
	AddWindow(window);
	GAMEIN->SetStreetStallDialog((CStreetStall * )window);
	GAMEIN->GetStreetStallDialog()->Linking();
}

void cWindowManager::CreateNpcScriptDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/NPCChat.bin", "rb");
	AddWindow(window);
	GAMEIN->SetNpcScriptDialog((cNpcScriptDialog * )window);
	GAMEIN->GetNpcScriptDialog()->Linking();
}

void cWindowManager::CreateDealDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/NPCShop.bin", "rb");
	AddWindow(window);
	GAMEIN->SetDealDialog((CDealDialog * )window);
	GAMEIN->GetDealDialog()->Linking();

	CMHFile fp;
	fp.Init("./System/Resource/DealItem.bin","rb");
	GAMEIN->GetDealDialog()->LoadDealerItem(&fp);
	fp.Release();

}

void cWindowManager::CreateQuestTotalDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/Quest.bin", "rb");
	AddWindow(window);

	CQuestTotalDialog * pDlg = (CQuestTotalDialog * )window;
	GAMEIN->SetQuestTotalDialog(pDlg);

	CQuestDialog* pQuestDlg = (CQuestDialog*)pDlg->GetTabSheet(0);
	GAMEIN->SetQuestDialog(pQuestDlg);
	pQuestDlg->Linking();


	QUESTMGR->SetQuestDialog(pQuestDlg) ;

	/*CWantedDialog* pWantedDlg = (CWantedDialog*)pDlg->GetTabSheet(1);
	GAMEIN->SetWantedDialog(pWantedDlg);
	pWantedDlg->Linking();
	
	CJournalDialog* pJournalDlg = (CJournalDialog*)pDlg->GetTabSheet(2);
	GAMEIN->SetJournalDialog(pJournalDlg);
	pJournalDlg->Linking();*/
}

void cWindowManager::CreateQuestquickDlg()
{
	cWindow* window = GetDlgInfoFromFile("./Data/Interface/Windows/QuestQuickView.bin", "rb") ;

	ASSERT(window) ;

	AddWindow(window) ;

	CQuestQuickViewDialog* pDlg = (CQuestQuickViewDialog*)window ;
	GAMEIN->SetQuestQuickViewDialog(pDlg) ;
	CQuestQuickViewDialog* pQuickDlg = GAMEIN->GetQuestQuickViewDialog() ;

	pQuickDlg->Linking() ;

	QUESTMGR->SetQuickViewDialog(pQuickDlg) ;
}

void cWindowManager::CreateBailDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/bail.bin", "rb");
	AddWindow(window);

	CBailDialog* pDlg = (CBailDialog*)window;
	GAMEIN->SetBailDialog(pDlg);
	pDlg->Linking();
}

void cWindowManager::CreateEventNotifyDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/GMOpenTalk.bin", "rb");
	AddWindow(window);

	CEventNotifyDialog* pDlg = (CEventNotifyDialog*)window;
	GAMEIN->SetEventNotifyDialog(pDlg);
	pDlg->Linking();
}

void cWindowManager::CreateMallNotcieDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/Itemmall.bin", "rb");
	AddWindow(window);

	CMallNoticeDialog* pDlg = (CMallNoticeDialog*)window;
	GAMEIN->SetMallNoticeDialog(pDlg);
//	pDlg->Linking();
}


void cWindowManager::CreateItemShopDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/ItemMallWarehouse.bin", "rb");
	AddWindow(window);

	CItemShopDialog* pDlg = (CItemShopDialog*)window;
	GAMEIN->SetItemShopDialog(pDlg);
	if( pDlg )
		pDlg->Linking();
}


void cWindowManager::CreateMoveDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/SaveMove.bin", "rb");
	AddWindow(window);

	CMoveDialog* pDlg = (CMoveDialog*)window;
	GAMEIN->SetMoveDialog(pDlg);
	if( pDlg )
		pDlg->Linking();
}


void cWindowManager::CreatePointSaveDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/ChangeName.bin", "rb");
	AddWindow(window);

	CPointSaveDialog* pDlg = (CPointSaveDialog*)window;
	GAMEIN->SetPointSaveDialog(pDlg);
	if( pDlg )
		pDlg->Linking();
}

//void cWindowManager::CreateSkPointDlg()
//{
//	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/Skillpointagain.bin", "rb");
//	AddWindow(window);
//
//	CSkillPointRedist* pDlg = (CSkillPointRedist*)window;
//	GAMEIN->SetSkPointDlg(pDlg);
//	if( pDlg )
//		pDlg->Linking();
//}

void cWindowManager::CreateSkPointNotifyDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/Skillpointop.bin", "rb");
	AddWindow(window);

	CSkillPointNotify* pDlg = (CSkillPointNotify*)window;
	GAMEIN->SetSkPointNotifydlg(pDlg);
	if( pDlg )
	{
		pDlg->Linking();
		pDlg->InitTextArea();
	}
}


//void cWindowManager::CreateShoutDlg()
//{
//	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/ch_allworldcreate.bin", "rb");
//	AddWindow(window);
//
//	CShoutDialog* pDlg = (CShoutDialog*)window;
//	GAMEIN->SetShoutDlg(pDlg);
//	pDlg->Linking();	
//}

void cWindowManager::CreateChaseDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/seechase.bin", "rb");

	AddWindow(window);

	CChaseDialog* pDlg = (CChaseDialog*)window;
	GAMEIN->SetChaseDlg(pDlg);
	pDlg->Linking();
}


void cWindowManager::CreateChaseinputDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/chase.bin", "rb");
	AddWindow(window);

	CChaseinputDialog* pDlg = (CChaseinputDialog*)window;
	GAMEIN->SetChaseinputDlg(pDlg);
	pDlg->Linking();
}

void cWindowManager::CreateNameChangeDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/chnamechange.bin", "rb");
	AddWindow(window);

	CNameChangeDialog* pDlg = (CNameChangeDialog*)window;
	GAMEIN->SetNameChangeDlg(pDlg);
	pDlg->Linking();
}


void cWindowManager::CreateNameChangeNotifyDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/opchnamechange.bin", "rb");
	AddWindow(window);

	CNameChangeNotifyDlg* pDlg = (CNameChangeNotifyDlg*)window;
	GAMEIN->SetNameChangeNotifyDlg(pDlg);
}


void cWindowManager::CreateGTRegistDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/GDTENTRY.bin", "rb");
	AddWindow(window);

	CGTRegistDialog* pDlg = (CGTRegistDialog*)window;
	GAMEIN->SetGTRegistDlg(pDlg);
	pDlg->Linking();
}


void cWindowManager::CreateGTRegistcancelDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/GDTCANCEL.bin", "rb");
	AddWindow(window);

	CGTRegistcancelDialog* pDlg = (CGTRegistcancelDialog*)window;
	GAMEIN->SetGTRegistcancelDlg(pDlg);
	pDlg->Linking();
}


void cWindowManager::CreateGTStandingDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/GDTournament.bin", "rb");
	AddWindow(window);

	CGTStandingDialog* pDlg = (CGTStandingDialog*)window;
	GAMEIN->SetGTStandingDlg(pDlg);
	pDlg->Linking();
}


void cWindowManager::CreateGTBattleListDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/GDViewTournament.bin", "rb");
	AddWindow(window);

	CGTBattleListDialog* pDlg = (CGTBattleListDialog*)window;
	GAMEIN->SetGTBattleListDlg(pDlg);
	pDlg->Linking();
}


void cWindowManager::CreateGTScoreInfoDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/EventMapCount.bin", "rb");
	AddWindow(window);

	CGTScoreInfoDialog* pDlg = (CGTScoreInfoDialog*)window;
	GAMEIN->SetGTScoreInfoDlg(pDlg);
	pDlg->Linking();
}

void cWindowManager::CreateSeigeWarDlg()
{
	cWindow* window = NULL;
	window = GetDlgInfoFromFile("Data/Interface/Windows/SWTimeReg.bin", "rb");
	AddWindow(window);

	CSWTimeRegDlg* pTimeRegDlg = (CSWTimeRegDlg*)window;
	GAMEIN->SetSWTimeRegDlg(pTimeRegDlg);
	pTimeRegDlg->Linking();

	window = GetDlgInfoFromFile("Data/Interface/Windows/SWProtectReg.bin", "rb");
	AddWindow(window);

	CSWProtectRegDlg* pProRegDlg = (CSWProtectRegDlg*)window;
	GAMEIN->SetSWProtectRegDlg(pProRegDlg);
	pProRegDlg->Linking();

	window = GetDlgInfoFromFile("Data/Interface/Windows/SWInfo.bin", "rb");
	AddWindow(window);

	CSWInfoDlg* pInfoDlg = (CSWInfoDlg*)window;
	GAMEIN->SetSWInfoDlg(pInfoDlg);
	pInfoDlg->Linking();
	
	window = GetDlgInfoFromFile("Data/Interface/Windows/SWProfit.bin", "rb");
	AddWindow(window);

	CSWProfitDlg* pProfitDlg = (CSWProfitDlg*)window;
	GAMEIN->SetSWProfitDlg(pProfitDlg);
	pProfitDlg->Linking();	

	window = GetDlgInfoFromFile("Data/Interface/Windows/Engrave.bin", "rb");
	AddWindow(window);

	CSWEngraveDialog* pEngraveDlg = (CSWEngraveDialog*)window;
	GAMEIN->SetSWEngraveDlg(pEngraveDlg);
	pEngraveDlg->Linking();	


	window = GetDlgInfoFromFile("Data/Interface/Windows/SWCount.bin", "rb");
	AddWindow(window);

	CSWTimeDialog* pSWTimeDlg = (CSWTimeDialog*)window;
	GAMEIN->SetSWTimeDlg(pSWTimeDlg);
	pSWTimeDlg->Linking();	


	window = GetDlgInfoFromFile("Data/Interface/Windows/SWStartTime.bin", "rb");
	AddWindow(window);

	CSWStartTimeDialog* pSTDlg = (CSWStartTimeDialog*)window;
	GAMEIN->SetSWStartTimeDlg(pSTDlg);
	pSTDlg->Linking();	
}

void cWindowManager::CreateItemSealDlg()
{
	cWindow* window = NULL;

	window = GetDlgInfoFromFile("Data/Interface/Windows/ItemLock.bin", "rb");
	AddWindow(window);

	CSealDialog* pDlg = (CSealDialog*)window;
	GAMEIN->SetSealDlg( pDlg );
	pDlg->Linking();
}

void cWindowManager::CreateChangeJobDlg()
{
	cWindow* window = NULL;

	window = GetDlgInfoFromFile("Data/Interface/Windows/Changejob.bin", "rb");
	AddWindow(window);

	CChangeJobDialog* pDlg = (CChangeJobDialog*)window;
	GAMEIN->SetChangeJobDlg( pDlg );
}

//void cWindowManager::CreateReinforceResetDlg()
//{
//	cWindow* window = NULL;
//
//	window = GetDlgInfoFromFile("Data/Interface/Windows/ReinforceDefault.bin", "rb");
//	AddWindow(window);
//
//	if( window )
//	{
//		CReinforceResetDlg* pDlg = (CReinforceResetDlg*)window;
//		pDlg->Linking();
//		GAMEIN->SetReinforceResetDlg( pDlg );
//	}
//}

//void cWindowManager::CreateRareCreateDlg()
//{
//	cWindow* window = NULL;
//
//	window = GetDlgInfoFromFile("Data/Interface/Windows/RareCreate.bin", "rb");
//	AddWindow(window);
//
//	CRareCreateDialog* pDlg = (CRareCreateDialog*)window;
//	pDlg->Linking();
//	GAMEIN->SetRareCreateDialog( pDlg );
//}

// 06. 02. 강화 재료 안내 인터페이스 추가 - 이영준
void cWindowManager::CreateReinforceGuideDlg()
{
	/*cWindow* window = GetDlgInfoFromFile("Data/Interface/Windows/ReinforceGuideDialog.bin", "rb");
	AddWindow(window);

	CReinforceGuideDialog* pDlg = (CReinforceGuideDialog*)window;
	pDlg->Linking();
	GAMEIN->SetReinforceGuideDlg( pDlg );*/

	CReinforceGuideDialog* dialog = ( CReinforceGuideDialog* )( GetDlgInfoFromFile( "Data/Interface/Windows/ReinforceGuideDialog.bin", "rb" ) );
	AddWindow( dialog );

	dialog->Linking();
	GAMEIN->SetReinforceGuideDlg( dialog );
}

// 06. 02. 내정보창 팁보기 추가 - 이영준
void cWindowManager::CreateTipBrowserDlgDlg()
{
	cWindow* window = NULL;

	window = GetDlgInfoFromFile("Data/Interface/Windows/TipBrowser.bin", "rb");
	AddWindow(window);

	CTipBrowserDlg* pDlg = (CTipBrowserDlg*)window;
	pDlg->Linking();
	GAMEIN->SetTipBrowserDlg( pDlg );
}

void cWindowManager::CreateKeySettingTipDlg()
{
	cWindow* window = NULL;

	window = GetDlgInfoFromFile("Data/Interface/Windows/KeySetting.bin", "rb");
	AddWindow(window);

	CKeySettingTipDlg* pDlg = (CKeySettingTipDlg*)window;
	pDlg->Linking();
	GAMEIN->SetKeySettingTipDlg( pDlg );
}

void cWindowManager::CreateGuildNoteDlg()
{
	cWindow* window = NULL;

	window = GetDlgInfoFromFile("Data/Interface/Windows/GuildNote.bin", "rb");
	if( window)
	{
		AddWindow(window);
		CGuildNoteDlg* pDlg = (CGuildNoteDlg*)window;
		pDlg->Linking();
		GAMEIN->SetGuildNoteDlg( pDlg );
	}
}

void cWindowManager::CreateUnionNoteDlg()
{
	cWindow* window = NULL;

	window = GetDlgInfoFromFile("Data/Interface/Windows/AllyNote.bin", "rb");
	if( window)
	{
		AddWindow(window);

		CUnionNoteDlg* pDlg = (CUnionNoteDlg*)window;
		pDlg->Linking();
		GAMEIN->SetUnionNoteDlg( pDlg );
	}
}

// 06. 03. 문파공지 - 이영준
void cWindowManager::CreateGuildNoticeDlg()
{
	CGuildNoticeDlg* dialog = ( CGuildNoticeDlg* )GetDlgInfoFromFile("Data/Interface/Windows/GuildNotice.bin", "rb");
	ASSERT( dialog );

	AddWindow( dialog );

	dialog->Linking();
	GAMEIN->SetGuildNoticeDlg( dialog );
}


void cWindowManager::CreateGuildWarehouseRankDlg()
{
	CGuildWarehouseRankDialog* dialog = ( CGuildWarehouseRankDialog* )GetDlgInfoFromFile( "Data/Interface/Windows/GuildWHRank.bin", "rb" );
	ASSERT( dialog );

	AddWindow( dialog );

	dialog->Linking();
}


void cWindowManager::ScreenShotDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/screenshotdlg.bin", "rb");
	AddWindow(window);

	CScreenShotDlg* pDlg = (CScreenShotDlg*)window;
	GAMEIN->SetScreenShotDlg(pDlg);
	pDlg->Linking();
}

void cWindowManager::CreateNpcImageDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/NpcImage.bin", "rb");
	AddWindow(window);

	cNpcImageDlg* pDlg = (cNpcImageDlg*)window;
	GAMEIN->SetNpcImageDlg(pDlg);

	// 070620 LYW --- cWindowManager : Linking controls.
	// 해상도에 따른 npc 이미지의 처리를 위해 상하 이미지를 링킹하기로 한다.
	pDlg->Linking() ;
}

void cWindowManager::AfterInit()
{
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while(pos)
	{
		cDialog * win = (cDialog *)m_pWindowList->GetNext(pos);
		if(win->GetType() & WT_ICONDIALOG || win->GetType() & WT_ICONGRIDDIALOG)
			m_pIconDlgListRef->AddTail(win);
		else if(win->GetType() & WT_TABDIALOG)
		{
			cTabDialog * tabDlg = (cTabDialog *)win;
			
			for( int i = 0; i < tabDlg->GetTabNum(); ++i )
			{
				if(tabDlg->GetTabSheet(i)->GetType() & WT_ICONDIALOG || tabDlg->GetTabSheet(i)->GetType() & WT_ICONGRIDDIALOG)
					m_pIconDlgListRef->AddTail(tabDlg->GetTabSheet(i));
			}
		}
		PTRLISTPOS pos2 = win->m_pComponentList.GetHeadPosition();
		while(pos2)
		{
			cWindow * win2 = (cWindow *)win->m_pComponentList.GetNext(pos2);
			if(win2->GetType() == WT_EDITBOX)
			{
				m_pEditBoxListRef->AddTail((void *)win2);
			}
			else if(win2->GetType() & WT_ICONDIALOG || win2->GetType() & WT_ICONGRIDDIALOG)
			{
				m_pIconDlgListRef->AddTail(win2);
			}
		}
	}
}

////////////////////////////////////////////////////
//CUserInput에서 Key Input이 들어왔을 시에 호출해 줌
void cWindowManager::KeyboardInput( CKeyboard* keyInfo )
{
	PTRLISTPOS pos = m_pWindowList->GetTailPosition();
	cWindow * window;
	DWORD Action = WE_NULL;

	while(pos)
	{
		window = (cWindow *)m_pWindowList->GetAt(pos);

		if(window->IsActive() && !window->IsDepend() )
		{
			Action = window->ActionKeyboardEvent( keyInfo );

			if( Action & WE_CLOSEWINDOW )
				window->SetActive(FALSE);

			if( Action != WE_NULL ) break;	//키인풋이 처리되면 중지
		}

		m_pWindowList->GetPrev(pos);
	}


//////////////////////////
}

void cWindowManager::SetNextEditFocus()
{
	if( m_pFocusedEdit )
	{
		cEditBox * win;
		PTRLISTPOS posCur = m_pEditBoxListRef->Find( m_pFocusedEdit );
		PTRLISTPOS pos = posCur;

		m_pEditBoxListRef->GetNext(pos);
		
		while( pos != posCur )
		{
			if( pos == NULL )
				pos = m_pEditBoxListRef->GetHeadPosition();

			win = (cEditBox *)m_pEditBoxListRef->GetNext(pos);
			
			if( win->IsActive() && win->GetParent() == m_pFocusedEdit->GetParent() )
			{
				// 070103 LYW --- Modified this code.
				//m_pFocusedEdit->SetFocus( FALSE );
				m_pFocusedEdit->SetFocusEdit( FALSE ) ;
				win->SetFocusEdit( TRUE );
				break;
			}
		}
	}
}

void cWindowManager::BackDragWindow()
{
	cDialog * srcDlg = GetWindowForID(m_id);
	if(!srcDlg) return;
	srcDlg->SetAbsXY((LONG)m_posBeforeDrag.x, (LONG)m_posBeforeDrag.y);	// SetDragEnd()가 있는데 필요한가???체크필

//	if( !m_pDragDialog ) return;
//	m_pDragDialog->SetAbsXY((LONG)m_posBeforeDrag.x, (LONG)m_posBeforeDrag.y);	// SetDragEnd()가 있는데 필요한가???체크필
	DragWindowNull();
}

void cWindowManager::Preprocess()
{
	if( m_OldDragFlag )
	{
		BOOL LeftUp = MOUSE->LButtonUp();
		
		if( LeftUp )
		{
			LONG ClickUpX = MOUSE->GetMouseEventX();
			LONG ClickUpY = MOUSE->GetMouseEventY();

			if( cbDragnDropProcess )
				cbDragnDropProcess( ClickUpX, ClickUpY, m_id, (LONG)m_posBeforeDrag.x, (LONG)m_posBeforeDrag.y);

			SetDragEnd();
		}
		else if( m_pDragDialog )
		{
			LONG x = MOUSE->GetMouseX();
			LONG y = MOUSE->GetMouseY();

//			cDialog * dlg = GetWindowForID( m_id );
//			if( dlg == NULL )
//				return;

			m_pDragDialog->SetAbsXY( x-m_OldX, y-m_OldY );
			// 070201 LYW --- Limited window area.
			//RECT rect = { 0, 0, 1280, 1024 };		//해상도 변경가능한가? confirm
			DISPLAY_INFO	dispInfo ;

			GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;

			RECT rect = { 0, 0, dispInfo.dwWidth, dispInfo.dwHeight };

			VECTOR2 magPos;
			magPos.x = m_pDragDialog->GetAbsX();
			magPos.y = m_pDragDialog->GetAbsY();
			if( cAnimationManager::MagProcess( &rect, (cWindow*)m_pDragDialog, &magPos ) )
				m_pDragDialog->SetAbsXY( magPos.x, magPos.y );
		}
	}
}

void cWindowManager::AddListDestroyWindow(cWindow * window)
{
	if( window == NULL ) return;

	// yh DestroyList에 같은 윈도우가 2번 들어가 있어서 다운 [4/3/2004]
	PTRLISTSEARCHSTART((*m_pDestroyWindowRef),cWindow*,pWindow)
		if(pWindow == window)
			return;
	PTRLISTSEARCHEND

	m_pDestroyWindowRef->AddTail(window);
	window->SetDestroy();
}

void cWindowManager::DestroyWindowProcess()
{
	PTRLISTPOS pos = m_pDestroyWindowRef->GetHeadPosition();
	while(pos)
	{
		cWindow * window = (cWindow *)m_pDestroyWindowRef->GetNext(pos);
		
		//DeleteWindowForID(window->GetID());
		DeleteWindow( window );
	}
	m_pDestroyWindowRef->RemoveAll();
}

void cWindowManager::OnWindowEvent(LONG lId, void * p, DWORD we)
{
	// 061220 LYW --- Delete this code.
	/*
	if( we & WE_CLOSEWINDOW )
	{
		GAMEIN->GetMainInterfaceDialog()->PushMenuID( lId, FALSE );
	}
	*/
}

void cWindowManager::Process()
{
	ANIMGR->FadeProcess();
 
	// 말 풍선
	PTRLISTPOS pos = m_pPlayerChatTooltip->GetHeadPosition();
	while(pos)
	{
		cWindow * window = (cWindow *)m_pPlayerChatTooltip->GetAt(pos);
		window->ActionEvent( MOUSE );
		
		if( !window->IsActive() )
			m_pPlayerChatTooltip->RemoveAt(pos);

		m_pPlayerChatTooltip->GetNext(pos);
	}
}

void cWindowManager::MouseInput( CMouse* pMouse )
{
//KES INTERFACE 031008
	m_bMouseOverUsed	= FALSE;
	m_bMouseDownUsed	= FALSE;
	m_bMouseUpUsed		= FALSE;
//	m_bKeyInputUsed		= FALSE;
	cWindow* pMouseOverWindow = NULL;
//
	m_MouseInputProcessed = FALSE;
	DestroyWindowProcess();
	Preprocess();
	
	cWindow * TopWindow = NULL;
	PTRLISTPOS TopPos = NULL;
	PTRLISTPOS pos = m_pWindowList->GetTailPosition();
	cWindow * window;
	
	while(pos)
	{
		window = (cWindow *)m_pWindowList->GetAt(pos);
////
		LONG Action=0;
		if( window->IsActive() )
		{
			if( !window->IsDepend() ) //KES INTERFACE 031010 //depend인것은 iconGridDialog가 한다.
				Action = window->ActionEvent( pMouse );
			//if(cbProcess)
				//cbProcess();
			
		}
		if((Action & WE_CLOSEWINDOW))
			window->SetActive(FALSE);
		else if(Action & WE_TOPWINDOW)
		{
			TopPos = pos;
			TopWindow = window;
		}
		if( Action & WE_LBTNCLICK || Action & WE_RBTNCLICK || 
			Action & WE_LBTNDBLCLICK || Action & WE_RBTNDBLCLICK)
		{
			m_MouseInputProcessed	= TRUE;
			m_bMouseDownUsed		= TRUE;
		}
		if( Action & WE_MOUSEOVER )
		{
			m_MouseInputProcessed	= TRUE;
			m_bMouseOverUsed		= TRUE;
			pMouseOverWindow		= window;
		}
//		if( Action & WE_RETURN )
//		{
//			m_bKeyInputUsed = TRUE;
//		}

		m_pWindowList->GetPrev(pos);
	}

	if(TopWindow && TopPos)
	{
		if( TopWindow->GetType() != WT_CHATTING )
		{
			m_pWindowList->RemoveAt(TopPos);			//중간에서 빼고

			if( TopWindow->IsAlwaysTop() )
				m_pWindowList->AddTail(TopWindow);		//끝에 넣고
			else
			{
				pos = m_pWindowList->GetTailPosition();		//메세지박스를 젤 위에 두기 위해

				while( pos )
				{
					cWindow* pWindow = (cWindow*)m_pWindowList->GetAt(pos);
					if( pWindow )
						if( !pWindow->IsAlwaysTop() )	break;
					m_pWindowList->GetPrev( pos );
				}

				if( pos )
					m_pWindowList->InsertAfter( pos, TopWindow );
				else 
					m_pWindowList->AddHead( TopWindow );
			}
		}
	}
	
	if(m_pGuageWindow)
	{
		m_pGuageWindow->ActionEvent(pMouse);
	}
	if(m_pMonsterGuageWindow)
	{
		m_pMonsterGuageWindow->ActionEvent(pMouse);
	}
	if(m_pMousePointWindow)
	{
		m_pMousePointWindow->SetAbsXY(pMouse->GetMouseX(), pMouse->GetMouseY());
//		m_pMousePointWindow->ActionEvent(pMouse);
	}
/*
	if( pMouseOverWindow )
	{
		if( pMouseOverWindow->GetType() & WT_ICON )
			CURSOR->SetCursor( eCURSOR_ITEMSELECT );
		else
			CURSOR->SetCursor( eCURSOR_DEFAULT );
	}
*/
}


void cWindowManager::Render()
{
	PTRLISTPOS pos = m_pScreenTextList->GetHeadPosition();
	while(pos)
	{
		cMultiLineText* mlwindow = (cMultiLineText*)m_pScreenTextList->GetNext(pos);
		mlwindow->Render();
	}

	// 말 풍선
	// taiyo : ObjectID를 셋팅하기 때문에 문제가 되지 않는가???
	pos = m_pPlayerChatTooltip->GetHeadPosition();
	cWindow* window;

	while(pos)
	{
		cWindow* window = (cWindow*)m_pPlayerChatTooltip->GetNext(pos);
		window->Render();
	}

	SetToolTipWindow( NULL ); //NULL로 하고, 아래 윈도 랜더할때 윈도들이 세팅한다.

	pos = m_pWindowList->GetHeadPosition();
//	pGeometry->BeginRender(0,0,DONOT_CLEAR_FRAMEBUFFER|DONOT_CLEAR_ZBUFFER);


//드래그 중인 dialog를 맨위로.. 
//AlwaysTop과 사이에 넣자.... 완전 하드코딩 KES

//	cDialog* dragDlg = GetWindowForID( m_id );	//getwindowforid find all files

	//drag중인 icon이 있을 때는 allwaystop이 나올때 까지 맨 나중에 찍어주어 위로 올라오도록 한다.

	STATUSICONDLG->Render();
	PETSTATUSICONDLG->Render();

	if( m_OldDragFlag )
	{
		BOOL bRenderDragDlg = FALSE;

		while(pos)
		{
			window = (cWindow *)m_pWindowList->GetNext(pos);

			if( window->IsAlwaysTop() )
			if( window->IsActive() )
			if( !bRenderDragDlg )
			if( !m_pDragDialog->IsAlwaysTop() )
			{
				m_pDragDialog->Render();
				bRenderDragDlg = TRUE;
			}

			if( !window->IsDepend() && window->GetID() != m_id )
				window->Render();
		}

		if( !bRenderDragDlg && m_pDragDialog)
			m_pDragDialog->Render();
	}
	else
	{
		while(pos)
		{
			window = (cWindow *)m_pWindowList->GetNext(pos);
			if( !window->IsDepend() )
				window->Render();
		}
	}

	
//////////////////
	if( !m_OldDragFlag )	//드래그중인 윈도우가 없을 때
	if( m_pToolTipWindow )
	{
		m_pToolTipWindow->Render();
	}
	
	if(m_pGuageWindow)
		m_pGuageWindow->Render();
	if(m_pMonsterGuageWindow)
		m_pMonsterGuageWindow->Render();
	if(m_pMousePointWindow)
		m_pMousePointWindow->Render();

//	pGeometry->EndRender();
#ifdef _TESTCLIENT_
	extern g_bDisplayFPS;
	if(g_bDisplayFPS)
	{
		RECT rect;
		SetRect(&rect,30,100,500,150);
		char temp[256];
		VECTOR3 pos;
		HERO->GetPosition(&pos);
		sprintf(temp,"x: %f z : %f", pos.x, pos.z);
		CFONT_OBJ->RenderFont(0,temp,strlen(temp),&rect,0xffffffff);
	}
#endif

// 070426 LYW --- cWindowManager : Add debug info part.
#ifdef _DEBUG
	m_DebugString.print_debug_string() ;
#endif //_DEBUG

}

void cWindowManager::DestroyWindowAll()
{
	if(!m_pWindowList)return;

	DestroyWindowProcess();
	
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while(pos)
	{
		cWindow * win = (cWindow *)m_pWindowList->GetNext(pos);
		SAFE_DELETE(win);
	}
	m_pWindowList->RemoveAll();

	if(m_pGuageWindow)
	{
		SAFE_DELETE(m_pGuageWindow);
	}
	if(m_pMonsterGuageWindow)
	{
		SAFE_DELETE(m_pMonsterGuageWindow);
	}
	
	DeleteEditBoxListAll();
	DeleteIconDlgListAll();
	DeleteChatTooltipListAll();

	m_pDragDialog = NULL;

}
void cWindowManager::CloseWindowForID(LONG id)
{
	if(!m_pWindowList) return;
	
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while(pos)
	{
		cWindow * win = (cWindow *)m_pWindowList->GetNext(pos);
		if(win->GetID() == id)
		{
			win->SetActive(FALSE);
			break;
		}
	}
}
void cWindowManager::DeleteIconDlgListAll()
{
	if(!m_pIconDlgListRef)return;
	m_pIconDlgListRef->RemoveAll();
}
void cWindowManager::DeleteEditBoxListAll()
{
	if(!m_pEditBoxListRef)return;
	m_pEditBoxListRef->RemoveAll();
}
void cWindowManager::DeleteChatTooltipListAll()
{
	if(!m_pPlayerChatTooltip)return;
	m_pPlayerChatTooltip->RemoveAll();
}

void cWindowManager::DeleteWindowForID(LONG id)
{
	if(!m_pWindowList) return;

	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while(pos)
	{
		cWindow * win = (cWindow *)m_pWindowList->GetAt(pos);
		if( win->GetID() == id )
		{
			m_pWindowList->RemoveAt(pos);
			SAFE_DELETE(win);
			if( m_id == id ) SetDragEnd();	//KES추가
			return;
		}
		m_pWindowList->GetNext(pos);
	}
}


void cWindowManager::DeleteWindow( cWindow* pWindow )
{
	if(!m_pWindowList) return;

	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while(pos)
	{
		cWindow* win = (cWindow *)m_pWindowList->GetAt(pos);
		if( win == pWindow )
		{
			m_pWindowList->RemoveAt(pos);
			if( m_id == win->GetID() ) SetDragEnd();	//KES추가
			SAFE_DELETE(win);
			return;
		}
		m_pWindowList->GetNext(pos);
	}
}


void cWindowManager::RemoveChatTooltipWindow(LONG playerID)
{
	if(!m_pPlayerChatTooltip) return;
	PTRLISTPOS pos = m_pPlayerChatTooltip->GetHeadPosition();
	while(pos)
	{
		cWindow * win = (cWindow *)m_pPlayerChatTooltip->GetAt(pos);
		if(win->GetID() == playerID)
		{
			m_pPlayerChatTooltip->RemoveAt(pos);
			break;
		}
		m_pPlayerChatTooltip->GetNext(pos);
	}
	return ;
}

void cWindowManager::RemoveChatTooltipWindow( cWindow* pWindow )
{
	if(m_pPlayerChatTooltip)
		m_pPlayerChatTooltip->Remove( pWindow );
}

cDialog * cWindowManager::GetWindowForID(LONG id)
{

	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while(pos)
	{
		cDialog * win = (cDialog *)m_pWindowList->GetNext(pos);
		if(win->GetID() == id)
			return win;
	}
	return NULL;
}


cWindow * cWindowManager::GetWindowForIDEx(LONG id)
{
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while(pos)
	{
		cDialog * win = (cDialog *)m_pWindowList->GetNext(pos);

		if(win->GetID() == id)
			return (cWindow *)win;
		else
		{
			PTRLISTPOS pos2 = win->m_pComponentList.GetHeadPosition();
			while(pos2)
			{
				cWindow * win2 = (cWindow *)win->m_pComponentList.GetNext(pos2);
				if(win2->GetID() == id)
				{
					return win2;
				}
			}
		}
	}
	return NULL;
}

cDialog * cWindowManager::GetWindowForXY(LONG x, LONG y)
{
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while(pos)
	{
		cDialog * win = (cDialog *)m_pWindowList->GetNext(pos);
		if(win->IsActive() == FALSE)
			continue;
		if(win->PtInWindow(x,y))
			return win;
	}
	return NULL;
}
cDialog * cWindowManager::GetWindowForXYExceptIcon(LONG x, LONG y)
{
	
//	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	PTRLISTPOS pos = m_pWindowList->GetTailPosition();
	while(pos)
	{
//		cDialog * win = (cDialog *)m_pWindowList->GetNext(pos);
		cDialog * win = (cDialog *)m_pWindowList->GetPrev(pos);
		if(win->IsActive() == FALSE || win->GetType() & WT_ICON)
			continue;
		if(win->PtInWindow(x,y))
			return win;
	}
	return NULL;
}

cDialog * cWindowManager::GetWindowForXYExceptID(LONG x, LONG y, LONG id)
{
	//꼬리부터로...바꾸기
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while(pos)
	{
		cDialog * win = (cDialog *)m_pWindowList->GetNext(pos);
		if(win->GetID() == id || win->IsActive() == FALSE)
			continue;
		if(win->PtInWindow(x,y))
			return win;
	}
	return NULL;
}

cIconDialog * cWindowManager::GetIconDialogForXY(LONG x, LONG y)
{
	//꼬리부터로...바꾸기
	PTRLISTPOS pos = m_pIconDlgListRef->GetHeadPosition();
	while(pos)
	{
		cDialog * win = (cDialog *)m_pIconDlgListRef->GetNext(pos);
		if(win->IsActive() == FALSE)
			continue;
		if(win->PtInWindow(x,y))
			return (cIconDialog *)win;
	}
	return NULL;
}


//KES 030825
void cWindowManager::SetOptionAlpha( DWORD dwAlpha )
{
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while(pos)
	{
		cWindow * win = (cWindow *)m_pWindowList->GetNext(pos);
		win->SetOptionAlpha( dwAlpha );
	}
}


cMsgBox* cWindowManager::MsgBox( LONG lMBId, int nMBType, char* pStrMsg, ... )
{
	if( !cMsgBox::IsInit() ) return NULL;

	if( WINDOWMGR->GetWindowForID( lMBId ) != NULL )
		return NULL;
	
	cMsgBox* pMsgBox = new cMsgBox;

	char msg[256] = {0,};

	if( pStrMsg != NULL ) 
	{
		va_list argList;

		va_start(argList, pStrMsg);
		vsprintf(msg, pStrMsg, argList);
		va_end(argList);
	}

//	pMsgBox->MsgBox( lMBId, nMBType, pStrMsg, cbMBFunc );
	pMsgBox->MsgBox( lMBId, nMBType, msg, MessageBox_Func );
	pMsgBox->SetAlwaysTop( TRUE );	//메세지 박스는 항상위. 싫으면 밖으로 빼기.
	AddWindow( pMsgBox );

	LONG lX = (LONG)pMsgBox->GetAbsX();
	LONG lY = (LONG)pMsgBox->GetAbsY();
	PositioningDlg( lX, lY );
	pMsgBox->SetAbsXY( lX, lY );
	pMsgBox->SetActive( TRUE );

	return pMsgBox;
}

void cWindowManager::PositioningDlg( LONG& rX, LONG& rY )
{
	int nCount = 0;
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while( pos )
	{
		cWindow* pWindow = (cWindow*)m_pWindowList->GetNext( pos );
		if( (LONG)pWindow->GetAbsX() == rX )
		if( (LONG)pWindow->GetAbsY() == rY )
		{
			rX += 20;
			rY += 20;
			if( ++nCount >= 10 )//한없이 내려가는것 방지
			{
				rX -= 200;
				rY -= 200;
				nCount = 0;
			}
		}
	}
}

cDivideBox * cWindowManager::DivideBox( LONG lId, LONG x, LONG y, cbDivideFUNC cbFc1, cbDivideFUNC cbFc2, void * vData1, void* vData2, char * strTitle )
{
	if( WINDOWMGR->GetWindowForID( lId ) != NULL )
		return NULL;

	cDivideBox * window = (cDivideBox * )GetDlgInfoFromFile("Data/Interface/Windows/DivideBox.bin", "rb" );
	
	window->CreateDivideBox( x, y, lId, cbFc1, cbFc2, vData1, vData2, strTitle );
	window->SetActive( TRUE );
	window->SetAlwaysTop( TRUE );

	AddWindow( window );

	return window;
}


void cWindowManager::SetWindowTop( cWindow* win )
{
	PTRLISTPOS pos = m_pWindowList->Find( win );

	if( pos == NULL ) return;

	m_pWindowList->RemoveAt( pos );
	
	if( win->IsAlwaysTop() )
		m_pWindowList->AddTail(win);						//끝에 넣고
	else
	{
		pos = m_pWindowList->GetTailPosition();		//메세지박스를 젤 위에 두기 위해
		
		while( pos )
		{
			cWindow* pWindow = (cWindow*)m_pWindowList->GetAt(pos);
			if( pWindow )
			if( !pWindow->IsAlwaysTop() )	break;

			m_pWindowList->GetPrev(pos);
		}
		
		if( pos )
			m_pWindowList->InsertAfter( pos, win );
		else 
			m_pWindowList->AddHead( win );
	}
}


///////////////////
//Tab키 처리를 위해	//focus를 줄때 안줄때
void cWindowManager::SetFocusedEdit( cWindow* pEdit, BOOL bFocus )
{
	if( bFocus )
	{
		if( m_pFocusedEdit == pEdit ) return;

#ifdef _HACK_SHIELD_
		HS_PauseService();
#endif

#ifdef TAIWAN_LOCAL
		HIMC hIMC = ImmGetContext( _g_hWnd );
		ImmSetOpenStatus( hIMC, TRUE );
		ImmReleaseContext( _g_hWnd, hIMC );

		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
#elif defined _HK_LOCAL_
		HIMC hIMC = ImmGetContext( _g_hWnd );
		ImmSetOpenStatus( hIMC, TRUE );
		ImmReleaseContext( _g_hWnd, hIMC );

		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
#elif defined _JAPAN_LOCAL_	
		HIMC hIMC = ImmGetContext( _g_hWnd );

		if( m_pFocusedEdit == NULL && m_bOpenStatus )
		{
			ImmSetOpenStatus( hIMC, TRUE );
		}

		if( pEdit->GetType() == WT_EDITBOX || pEdit->GetType() == WT_SPIN )
		{
			if( ((cEditBox*)pEdit)->GetValidCheckMethod() == VCM_NUMBER )
			{
				m_bOpenStatus = ImmGetOpenStatus( hIMC );
				ImmSetOpenStatus( hIMC, FALSE );
			}
		}

		ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
		ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );

		ImmReleaseContext( _g_hWnd, hIMC );
#endif

		m_pFocusedEdit = pEdit;
	}
	else
	{

#ifndef _JAPAN_LOCAL_
		if( m_pFocusedEdit == pEdit )
		{
			m_pFocusedEdit = NULL;

#ifdef TAIWAN_LOCAL
			HIMC hIMC = ImmGetContext( _g_hWnd );
			ImmSetOpenStatus( hIMC, FALSE );
			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );

			ImmReleaseContext( _g_hWnd, hIMC );

			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
#endif
#ifdef _HK_LOCAL_
			HIMC hIMC = ImmGetContext( _g_hWnd );
			ImmSetOpenStatus( hIMC, FALSE );
			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );

			ImmReleaseContext( _g_hWnd, hIMC );

			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
#endif

#ifdef _HACK_SHIELD_
		HS_ResumeService();
#endif

		}
#else		//defined _JAPAN_LOCAL_
		
		HIMC hIMC = ImmGetContext( _g_hWnd );

		if( m_pFocusedEdit == pEdit )
		{
			m_pFocusedEdit = NULL;
			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );

			if( pEdit->GetType() == WT_EDITBOX || pEdit->GetType() == WT_SPIN )
			{
				if( ((cEditBox*)pEdit)->GetValidCheckMethod() != VCM_NUMBER )
				{
					m_bOpenStatus = ImmGetOpenStatus( hIMC );
				}

				ImmSetOpenStatus( hIMC, FALSE );
			}
			else
			{
				m_bOpenStatus = ImmGetOpenStatus( hIMC );
				ImmSetOpenStatus( hIMC, FALSE );
			}
		}
		else
		{
			if( pEdit->GetType() == WT_EDITBOX || pEdit->GetType() == WT_SPIN )
			{
				if( ((cEditBox*)pEdit)->GetValidCheckMethod() == VCM_NUMBER )
				{
					ImmSetOpenStatus( hIMC, m_bOpenStatus );
				}
			}
		}

		ImmReleaseContext( _g_hWnd, hIMC );
#endif

	}
}


BOOL cWindowManager::CloseAllAutoCloseWindows()
{
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	cWindow* pWin;
	BOOL bClose = FALSE;

	//delete된 아이콘들이 아직 리스트에서 안없어졌을때 뻑이난다(?)
	while(pos)
	{
		pWin = (cWindow*)m_pWindowList->GetNext(pos);

		if( pWin->IsAutoClose() && pWin->IsActive() )
		{
			pWin->SetActive( FALSE );
			bClose = TRUE;
		}
	}

	// desc_hseos_ESC01
	// S ESC 추가 added by hseos 2007.06.19
	// ..ESC 누르면 타겟도 사라지게 한다.
	if (OBJECTMGR->GetSelectedObject())
	{
		OBJECTMGR->GetSelectedObject()->OnDeselected();
		OBJECTMGR->SetSelectedObject(NULL);
	}
	// E ESC 추가 added by hseos 2007.06.19

	return bClose;
}


cWindow* cWindowManager::GetDlgInfoFromFile (char * filePath, char* mode )
{
//	return m_pScriptManager->GetDlgInfoFromFile(filePath, mode);
	return SCRIPTMGR->GetDlgInfoFromFile(filePath, mode);	// SCRIPTMGR로 대체.
	
}


cMsgBox* cWindowManager::GetFirstMsgBox()
{
	PTRLISTPOS pos = m_pWindowList->GetTailPosition();
	cWindow * window;
	
	while(pos)
	{
//		window = (cWindow *)m_pWindowList->GetAt(pos);
//		m_pWindowList->GetPrev(pos);
		window = (cWindow *)m_pWindowList->GetPrev(pos);
		if( !window ) continue;
		
		if( window->GetType()  == WT_MSGBOX )
			return (cMsgBox*)window;
	}

	return NULL;
}

void cWindowManager::CloseAllMsgBox()
{
	PTRLISTSEARCHSTART( (*m_pWindowList), cWindow*, pWindow )
		if( pWindow )
		if( pWindow->GetType() == WT_MSGBOX )
		{
			((cMsgBox*)pWindow)->ForceClose();
		}
	PTRLISTSEARCHEND	
}

//사용에 주의하시오! 심각한 오류를 불러 일으킬수 있음. 제작:강은성
void cWindowManager::CloseAllMsgBoxNoFunc()
{
	PTRLISTSEARCHSTART( (*m_pWindowList), cWindow*, pWindow )
		if( pWindow )
		if( pWindow->GetType() == WT_MSGBOX )
		{
			AddListDestroyWindow( pWindow );			
		}
	PTRLISTSEARCHEND	
}



//////////////////////////////////////////////////////////////////////////
// yh edit 
BOOL cWindowManager::CloseAllWindow()
{
	WINDOWMGR->m_pActivedWindowList->RemoveAll();

	//GAMEIN->GetGuageDlg()->SetActive(FALSE);
	BOOL rt = FALSE;
	PTRLISTSEARCHSTART((*m_pWindowList),cWindow*,pWindow)
		if(pWindow->IsActive() == TRUE)
		{
			// 070124 LYW --- Call function to save id of actived window.
			SaveAllActivedWindow( pWindow ) ;

			rt = TRUE;
			pWindow->SetActive(FALSE);
			m_bHideInterface = TRUE;
		}
	PTRLISTSEARCHEND

	return rt;
}
void cWindowManager::ShowBaseWindow()
{
	GAMEIN->GetChattingDlg()->SetActive(TRUE);
	GAMEIN->GetGuageDlg()->SetActive(TRUE);
	GAMEIN->GetQuickDlg()->SetActive(TRUE);

	// 070124 LYW --- Active other dialog.
	GAMEIN->GetMainSystemDlg()->SetActive( TRUE ) ;
	GAMEIN->GetMiniMapDialog()->SetActive( TRUE ) ;
	GAMEIN->GetQuickDlg()->GetSlotDlg(0)->SetActive( TRUE ) ;
	GAMEIN->GetQuickDlg()->GetExSlotDlg(1)->SetActive( GAMEIN->GetQuickDlg()->GetExSlotDlg(1)->IsActive() ) ;
	if( GAMEIN->GetMonsterGuageDlg()->IsActive() )
	{
		GAMEIN->GetMonsterGuageDlg()->SetActive( TRUE ) ;
	}
	// desc_hseos_몬스터미터01
	// S 몬스터미터 추가 added by hseos 2007.07.26
	GAMEIN->GetMonstermeterDlg()->SetActive(TRUE);
	// E 몬스터미터 추가 added by hseos 2007.07.26
	m_bHideInterface = FALSE;
}
void cWindowManager::ToggleShowInterface()
{
	if(CloseAllWindow() == FALSE)
		ShowBaseWindow();
}

void cWindowManager::CreateGuildInvitationKindSelectionDlg()
{
	CGuildJoinDialog* dialog = ( CGuildJoinDialog* )GetDlgInfoFromFile("Data/Interface/Windows/JoinOption.bin", "rb");
	ASSERT( dialog );

	AddWindow( dialog );

	dialog->Linking();
	GAMEIN->SetGuildJoinDialog( dialog );
}

// 070124 LYW --- Add functino to save windows that actived.
void cWindowManager::SaveAllActivedWindow( cWindow* win )
{
	// 080214 LYW --- cWindowManager : [ 변환 중 입니다. ] 이 창은, 일정 시간이 지나면 자동으로 사라져야 한다.
	// 따라서, 일정 시간 후 사라져야 하는 윈도우는, 활성화 중인 윈도우에 포함시키지 않는다.
	// Ctrl + z 나 npc 대화 모드로 전환 시, 이런(일정 시간 후 자동으로 비활성화 되는 윈도우) 윈도우는, 
	// 비 활성화 중이던 창들을 활성화 시킬 때, 살아있게 된다. 일정 시간이 지나면 사라져야 함.

	if( !win ) return ;

	DWORD dwWindowID = 0 ;
	dwWindowID = win->GetID() ;
    
	if( dwWindowID == PROGRESS_DIALOG ) return ;

	//if( m_pActivedWindowList->GetCount() > 0 )
	//{
		void* key = ( void* )( dwWindowID ) ;

		PTRLISTPOS pos = m_pActivedWindowList->Find( key ) ;

		if( ! pos )
		{
			m_pActivedWindowList->AddTail( key ) ;
		}
	//}
	//else
	//{
	//	m_pActivedWindowList->RemoveAll() ;
	//	m_pActivedWindowList->AddTail( ((void*)win->GetID()) ) ;
	//}
}

void cWindowManager::ShowAllActivedWindow()
{
	//GAMEIN->GetGuageDlg()->SetActive(TRUE);

	for( PTRLISTPOS position = m_pActivedWindowList->GetHeadPosition(); position; )
	{
		const LONG windowId = ( LONG )m_pActivedWindowList->GetNext( position );

		cDialog* dialog = GetWindowForID( windowId );

		if( dialog )
		{
			dialog->SetActive( TRUE );
		}
	}

	/*
	PTRLISTSEARCHSTART( (*m_pActivedWindowList), LONG, lId )
		if( lId )
		{
			cWindow* pWindow = ( cWindow* )GetWindowForID( lId ) ;
			
			if( pWindow )
			{
				pWindow->SetActive( TRUE ) ;
			}
		}
	PTRLISTSEARCHEND
	*/
}
// 2007/03/15 데이트 매칭 다이얼로그 추가-이진영
void cWindowManager::CreateDateMatchingDlg()
{
	CDateMatchingDlg*	pDlg	=	(CDateMatchingDlg*)GetDlgInfoFromFile("Data/Interface/Windows/DateMatchingDlg.bin", "rb");

	if( pDlg )
	{
		AddWindow( pDlg ) ;

		pDlg->Linking() ;

		GAMEIN->SetDateMatchingDlg( pDlg ) ;
	}

	//CDateMatchingDlg*	pDlg	=	(CDateMatchingDlg*)GetDlgInfoFromFile("Data/Interface/Windows/DateMatchingDlg.bin", "rb");				// 메인 다이얼로그
	//if( pDlg)
	//{
	//	CDateMatchingUpperDlg*	pUpper	=	(CDateMatchingUpperDlg*)GetDlgInfoFromFile("Data/Interface/Windows/DateMatchingUpperDlg.bin", "rb");				// 데이트 매칭 중 윗쪽 탭 다이얼로그.
	//	if( pUpper == 0)
	//		_asm int 3;
	//	pDlg->Add( pUpper);
	//	pDlg->SetUpperDlg( pUpper);

	//	CDateMatchingLowerDlg*	pLower	=	(CDateMatchingLowerDlg*)GetDlgInfoFromFile("Data/Interface/Windows/DateMatchingLowerDlg.bin", "rb");				// 데이트 매칭 중 윗쪽 탭 다이얼로그.
	//	if( pLower == 0)
	//		_asm int 3;
	//	pDlg->Add( pLower);
	//	pDlg->SetLowerDlg( pLower);


	//	AddWindow( pDlg);					// 메니저에 등록한다.

	//	pDlg->InitMyDlg();

	//	GAMEIN->SetDateMatchingDlg( pDlg);
	//}


	//cWindow*	pUpper	=	NULL;
	//pUpper		=	GetDlgInfoFromFile("Data/Interface/Windows/DateMatchingUpperDlg.bin", "rb");		// 다이얼로그 생성.
	//if( pUpper)		// 아직 테스트중.
	//{
	//	CDateMatchingDlg*	pDlg	=	new	CDateMatchingDlg;
	//	pDlg->Add( pUpper);

	//
	//	AddWindow( pDlg);																				// 메니져에 등록.

	//	// 링크는 아직 없다.
	//}
	//else
	//{
	//	_asm nop;
	//}
}

// 070605 LYW --- cWindowManager : Add function to create identification.
void cWindowManager::CreateIdentificationDlg()
{
	cWindow* window = GetDlgInfoFromFile("Data/Interface/Windows/IdentificationDlg.bin", "rb") ;

	if( window )
	{
		AddWindow(window) ;

		CIdentification* pDlg = (CIdentification*)window ;

		pDlg->Linking() ;
		pDlg->LoadFavorItem() ;

		GAMEIN->SetIdentificationDlg(pDlg) ;
	}
}

// 070605 LYW --- cWindowManager : Add function to create favor icon dialog.
void cWindowManager::CreateFavorIconDlg()
{
	cWindow* window = GetDlgInfoFromFile("Data/Interface/Windows/FavorIconDlg.bin", "rb") ;

	if( window )
	{
		AddWindow(window) ;

		CFavorIcon* pDlg = (CFavorIcon*)window ;

		pDlg->Linking() ;

		pDlg->LoadFavorItem() ;
		pDlg->ShowFavorItem() ;

		GAMEIN->SetFavorIconDlg(pDlg) ;
	}
}

void cWindowManager::CreateSkillTrainingDlg()
{
	cWindow* window = GetDlgInfoFromFile("Data/Interface/Windows/SKILLTRN.bin", "rb");
	AddWindow(window);

	cSkillTrainingDlg* pDlg = (cSkillTrainingDlg*)window;
	pDlg->Linking();
	GAMEIN->SetSkillTrainingDlg( pDlg );
}


void cWindowManager::CreateFamilyCreateDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/FamilyCreate.bin", "rb");
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetFamilyCreateDlg((CFamilyCreateDialog * )window);
	GAMEIN->GetFamilyCreateDlg()->Linking();
/*	
	window = GetDlgInfoFromFile("Data/Interface/Windows/FamilyUnionCreate.bin", "rb");
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetFamilyUnionCreateDlg((CFamilyUnionCreateDialog * )window);
	GAMEIN->GetFamilyUnionCreateDlg()->Linking();
*/
}

void cWindowManager::CreateFamilyDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/Family.bin", "rb");
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetFamilyDlg((CFamilyDialog * )window);
	GAMEIN->GetFamilyDlg()->Linking();
}

void cWindowManager::CreateFamilyInviteDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/FamilyInvite.bin", "rb");
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetFamilyInviteDlg((CFamilyInviteDialog * )window);
	GAMEIN->GetFamilyInviteDlg()->Linking();
}

void cWindowManager::CreateFamilyMarkDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/FamilyMark.bin", "rb");
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetFamilyMarkDlg((CFamilyMarkDialog * )window);
	GAMEIN->GetFamilyMarkDlg()->Linking();
}

//void cWindowManager::CreateGameNotifyDlg()
//{
//	cWindow * window = GetDlgInfoFromFile("./Data/Interface/Windows/GameNotifyDlg.bin", "rb") ;
//
//	ASSERT(window) ;
//
//	AddWindow(window) ;
//
//	GAMEIN->SetGameNotifyDlg((CGameNotifyDlg*)window) ;
//	GAMEIN->GetGameNotifyDlg()->Linking() ;
//
//	GAMENOTIFYMGR->SetNotifyDlg(GAMEIN->GetGameNotifyDlg()) ;
//}

/*
void cWindowManager::CreateFamilyLevelupDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/FamilyLevelUp.bin", "rb");
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetFamilyLevelUpDlg((CFamilyLevelUpDialog * )window);
	GAMEIN->GetFamilyLevelUpDlg()->Linking();
}
*/
void cWindowManager::CreateFamilyNickNameDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/FamilyNickName.bin", "rb");
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetFamilyNickNameDlg((CFamilyNickNameDialog * )window);
	GAMEIN->GetFamilyNickNameDlg()->Linking();
}
/*
void cWindowManager::CreateFamilyRankDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/FamilyRank.bin", "rb");
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetFamilyRankDlg((CFamilyRankDialog * )window);
	GAMEIN->GetFamilyRankDlg()->Linking();
}

void cWindowManager::CreateFamilyWarehouseDlg()
{
	cWindow * window = GetDlgInfoFromFile("Data/Interface/Windows/FamilyWarehouse.bin", "rb");
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetFamilyWarehouseDlg((CFamilyWarehouseDialog * )window);
	GAMEIN->GetFamilyWarehouseDlg()->Linking();
}
*/

// 071017 LYW --- cWindowManager : Add function to create tutorial dialog.
void cWindowManager::CreateTutorialDlg()
{
	cWindow* window = GetDlgInfoFromFile("Data/Interface/Windows/TutorialDlg.bin", "rb") ;

	ASSERT(window) ;
	AddWindow(window) ;

	GAMEIN->SetTutorialDlg((cTutorialDlg*)window) ;
	GAMEIN->GetTutorialDlg()->Linking() ;

	TUTORIALMGR->Set_TutorialDlg(GAMEIN->GetTutorialDlg()) ;
}





// 071017 LYW --- cWindowManager : Add function to create help dialog.
void cWindowManager::CreateHelpDlg()
{
	cWindow* window = GetDlgInfoFromFile("Data/Interface/Windows/HelpDlg.bin", "rb") ;

	ASSERT(window) ;
	AddWindow(window) ;

	GAMEIN->SetHelpDlg((cHelperDlg*)window) ;
	GAMEIN->GetHelpDlg()->Linking() ;

	TUTORIALMGR->Set_HelperDlg(GAMEIN->GetHelpDlg()) ;
}





// 070123 LYW --- cWindowManager : Add function to create notice button dialog of tutorial.
void cWindowManager::CreateTutorialBtnDlg()
{
	cWindow* window = GetDlgInfoFromFile("Data/Interface/Windows/TutorialBtnDlg.bin", "rb") ;

	ASSERT(window) ;
	AddWindow(window) ;

	GAMEIN->SetTutorialBtnDlg((cTutorialBtnDlg*)window) ;
	GAMEIN->GetTutorialBtnDlg()->Linking() ;

	TUTORIALMGR->Set_TutorialBtnDlg(GAMEIN->GetTutorialBtnDlg()) ;
}


void cWindowManager::CreateItemMallBtnDlg()
{
	cWindow* window = GetDlgInfoFromFile("Data/Interface/Windows/ItemMallBtnDlg.bin", "rb") ;

	ASSERT(window) ;
	AddWindow(window) ;

	GAMEIN->SetItemMallBtnDlg((cItemMallBtnDlg*)window) ;
	GAMEIN->GetItemMallBtnDlg()->Linking() ;
}

void cWindowManager::CreateItemPopupDlg()
{
	cWindow* window = GetDlgInfoFromFile("Data/Interface/Windows/ItemPopupDlg.bin", "rb") ;

	ASSERT(window) ;
	AddWindow(window) ;

	GAMEIN->SetItemPopupDlg((cItemPopupDlg*)window) ;
	GAMEIN->GetItemPopupDlg()->Linking() ;
}


// 071130 LYW --- cWindowManager : Add function to create map move dialog.
void cWindowManager::CreateMapMoveDlg()
{
	cWindow* window = GetDlgInfoFromFile("Data/Interface/Windows/MapMoveScrollDlg.bin", "rb") ;

	ASSERT(window) ;
	AddWindow(window) ;

	GAMEIN->SetMapMoveDlg((cMapMoveDialog*)window) ;
	GAMEIN->GetMapMoveDlg()->Linking() ;
}





// 071227 LYW --- cWindowManager : 캐릭터 이름 변경 다이얼로그 추가.
//=========================================================================
//	NAME : CreateChangeNameDlg
//	DESC : The function to create dialog to change hero name.
//=========================================================================
void cWindowManager::CreateChangeNameDlg()
{
	cWindow* pWindow = NULL ;
	pWindow = GetDlgInfoFromFile("Data/Interface/Windows/ChangeNameDlg.bin", "rb") ;

	ASSERT(pWindow) ;
	AddWindow(pWindow) ;

	GAMEIN->SetChangeNameDlg((cChangeNameDialog*)pWindow) ;

	cChangeNameDialog* pDlg = NULL ;
	pDlg = GAMEIN->GetChangeNameDlg() ;

	if( !pDlg )
	{
		MessageBox( NULL, "Failed to receive dialog to change name", "create dialog to change name", MB_OK ) ;
		ASSERT(pDlg) ;
		return ;
	}

	pDlg->Linking() ;
}





//=========================================================================
//	NAME : CreateNumberPadDlg
//	DESC : The function to create dialog for number pad. 080109 LYW
//=========================================================================
void cWindowManager::CreateNumberPadDlg()
{
//	return;

	cWindow* pWindow = NULL ;
	pWindow = GetDlgInfoFromFile("Data/Interface/Windows/NumberPad.bin", "rb") ;

	ASSERT(pWindow) ;
	AddWindow(pWindow) ;

	GAMEIN->SetNumberPadDlg((cNumberPadDialog*)pWindow) ;

	cNumberPadDialog* pDlg = NULL ;
	pDlg = GAMEIN->GetNumberPadDlg() ;

	if( !pDlg )
	{
		MessageBox( NULL, "Failed to receive number pad dialog ", "Create number pad dialog.", MB_OK ) ;
		ASSERT(pDlg) ;

		return ;
	}

	pDlg->Linking() ;
}

//---KES AUTONOTE
void cWindowManager::CreateAutoNoteDlg()
{
	cWindow* window = GetDlgInfoFromFile( "Data/Interface/Windows/AutoNoteDlg.bin", "rb" );
	AddWindow(window);
	GAMEIN->SetAutoNoteDlg( (CAutoNoteDlg*)window );
	GAMEIN->GetAutoNoteDlg()->Linking();
}

void cWindowManager::CreateAutoAnswerDlg()
{
	cWindow* window = GetDlgInfoFromFile( "Data/Interface/Windows/AutoAnswerDlg.bin", "rb" );
	AddWindow(window);
	GAMEIN->SetAutoAnswerDlg( (CAutoAnswerDlg*)window );
	GAMEIN->GetAutoAnswerDlg()->Linking();
}
//--------------

//---KES 상점검색
void cWindowManager::CreateStoreSearchDlg()
{
	cWindow* window = GetDlgInfoFromFile( "Data/Interface/Windows/StoreSearchDlg.bin", "rb" );
	AddWindow(window);
	GAMEIN->SetStoreSearchDlg( (CStoreSearchDlg*)window );
	GAMEIN->GetStoreSearchDlg()->Linking();
}

//---BROWSER
void cWindowManager::CreateItemShopBrowserDlg()
{
	cWindow* window = GetDlgInfoFromFile( "Data/Interface/Windows/ItemShopDlg.bin", "rb" );
	AddWindow(window);
	GAMEIN->SetItemShopDlg( (CItemShopDlg*)window );
	GAMEIN->GetItemShopDlg()->Linking();
}

// 080313 NYJ --- 전체외치기 다이얼로그 추가
void cWindowManager::CreateShoutDlg()
{
	cWindow* window = GetDlgInfoFromFile("Data/Interface/Windows/ShoutDlg.bin", "rb" );
	AddWindow(window);

	GAMEIN->SetShoutDlg((CShoutDialog*)window) ;	

	CShoutDialog* pDlg = NULL ;
	pDlg = GAMEIN->GetShoutDlg();

	if( !pDlg )
	{
		MessageBox( NULL, "Failed to receive dialog to Shout", "create dialog to change name", MB_OK ) ;
		ASSERT(pDlg) ;
		return ;
	}

	pDlg->Linking() ;
}

// 080414 LUJ, 외양 변경 창 생성
void cWindowManager::CreateBodyChangeDlg()
{
	CBodyChangeDialog* dialog = ( CBodyChangeDialog* )GetDlgInfoFromFile("Data/Interface/Windows/BodyChangeDialog.bin", "rb" );

	if( ! dialog )
	{
		return;
	}

	AddWindow( dialog );
	dialog->Linking();	
}


// 080403 LYW --- cWindowManager : Add functions to create dialog for chatroom system.
void cWindowManager::CreateChatRoomMainDlg()
{
	cWindow* pWindow = NULL ;
	pWindow = GetDlgInfoFromFile("Data/Interface/Windows/ChatRoomMainDlg.bin", "rb") ;

	ASSERT(pWindow) ;

	if(!pWindow)
	{
		MessageBox(	NULL, "FRDFF", "CCRMD", MB_OK) ;
		return ;
	}

	AddWindow(pWindow) ;

	GAMEIN->SetChatRoomMainDlg((CChatRoomMainDlg*)pWindow) ;

	CChatRoomMainDlg* pDlg = NULL ;
	pDlg = GAMEIN->GetChatRoomMainDlg() ;

	ASSERT(pDlg) ;

	if(!pDlg)
	{
		MessageBox(	NULL, "FRDFG", "CCRMD", MB_OK) ;
		return ;
	}

	pDlg->Linking() ;
}

void cWindowManager::CreateChatRoomCreateDlg()
{
	cWindow* pWindow = NULL ;
	pWindow = GetDlgInfoFromFile("Data/Interface/Windows/ChatRoomCreateDlg.bin", "rb") ;

	ASSERT(pWindow) ;

	if(!pWindow)
	{
		MessageBox(	NULL, "FRDFF", "CCRCD", MB_OK) ;
		return ;
	}

	AddWindow(pWindow) ;

	GAMEIN->SetChatRoomCreateDlg((CChatRoomCreateDlg*)pWindow) ;

	CChatRoomCreateDlg* pDlg = NULL ;
	pDlg = GAMEIN->GetChatRoomCreateDlg() ;

	ASSERT(pDlg) ;

	if(!pDlg)
	{
		MessageBox(	NULL, "FRDFG", "CCRCD", MB_OK) ;
		return ;
	}

	pDlg->Linking() ;
}

void cWindowManager::CreateChatRoomDlg()
{
	cWindow* pWindow = NULL ;
	pWindow = GetDlgInfoFromFile("Data/Interface/Windows/ChatRoomDlg.bin", "rb") ;

	ASSERT(pWindow) ;

	if(!pWindow)
	{
		MessageBox(	NULL, "FRDFF", "CCRD", MB_OK) ;
		return ;
	}

	AddWindow(pWindow) ;

	GAMEIN->SetChatRoomDlg((CChatRoomDlg*)pWindow) ;

	CChatRoomDlg* pDlg = NULL ;
	pDlg = GAMEIN->GetChatRoomDlg() ;

	ASSERT(pDlg) ;

	if(!pDlg)
	{
		MessageBox(	NULL, "FRDFG", "CCRD", MB_OK) ;
		return ;
	}

	pDlg->Linking() ;
}

void cWindowManager::CreateChatRoomGuestListDlg() 
{
	cWindow* pWindow = NULL ;
	pWindow = GetDlgInfoFromFile("Data/Interface/Windows/ChatRoomGuestListDlg.bin", "rb") ;

	ASSERT(pWindow) ;

	if(!pWindow)
	{
		MessageBox(	NULL, "FRDFF", "CCRGLD", MB_OK) ;
		return ;
	}

	AddWindow(pWindow) ;

	GAMEIN->SetChatRoomGuestListDlg((CChatRoomGuestListDlg*)pWindow) ;

	CChatRoomGuestListDlg* pDlg = NULL ;
	pDlg = GAMEIN->GetChatRoomGuestListDlg() ;

	ASSERT(pDlg) ;

	if(!pDlg)
	{
		MessageBox(	NULL, "FRDFG", "CCRGLD", MB_OK) ;
		return ;
	}

	pDlg->Linking() ;
}

void cWindowManager::CreateChatRoomOptionDlg() 
{
	cWindow* pWindow = NULL ;
	pWindow = GetDlgInfoFromFile("Data/Interface/Windows/ChatRoomOptionDlg.bin", "rb") ;

	ASSERT(pWindow) ;

	if(!pWindow)
	{
		MessageBox(	NULL, "FRDFF", "CCROD", MB_OK) ;
		return ;
	}

	AddWindow(pWindow) ;

	GAMEIN->SetChatRoomOptionDlg((CChatRoomOptionDlg*)pWindow) ;

	CChatRoomOptionDlg* pDlg = NULL ;
	pDlg = GAMEIN->GetChatRoomOptionDlg() ;

	ASSERT(pDlg) ;

	if(!pDlg)
	{
		MessageBox(	NULL, "FRDFG", "CCROD", MB_OK) ;
		return ;
	}

	pDlg->Linking() ;
}

void cWindowManager::CreateChatRoomJoinDlg() 
{
	cWindow* pWindow = NULL ;
	pWindow = GetDlgInfoFromFile("Data/Interface/Windows/ChatRoomJoinDlg.bin", "rb") ;

	ASSERT(pWindow) ;

	if(!pWindow)
	{
		MessageBox(	NULL, "FRDFF", "CCRJD", MB_OK) ;
		return ;
	}
AddWindow(pWindow) ;

	GAMEIN->SetChatRoomJoinDlg((CChatRoomJoinDlg*)pWindow) ;

	CChatRoomJoinDlg* pDlg = NULL ;
	pDlg = GAMEIN->GetChatRoomJoinDlg() ;

	ASSERT(pDlg) ;

	if(!pDlg)
	{
		MessageBox(	NULL, "FRDFG", "CCRJD", MB_OK) ;
		return ;
	}

	pDlg->Linking() ;
}

//void cWindowManager::CreateChatRoomLobbyDlg()
//{
//	cWindow* pWindow = NULL ;
//	pWindow = GetDlgInfoFromFile("Data/Interface/Windows/ChatRoomLobbyDlg.bin", "rb") ;
//
//	ASSERT(pWindow) ;
//
//	if(!pWindow)
//	{
//		MessageBox(	NULL, "FRDFF", "CCRLD", MB_OK) ;
//		return ;
//	}
//
//	AddWindow(pWindow) ;
//
//	GAMEIN->SetChatRoomLobbyDlg((CChatRoomLobbyDlg*)pWindow) ;
//
//	CChatRoomLobbyDlg* pDlg = NULL ;
//	pDlg = GAMEIN->GetChatRoomLobbyDlg() ;
//
//	ASSERT(pDlg) ;
//
//	if(!pDlg)
//	{
//		MessageBox(	NULL, "FRDFG", "CCRLD", MB_OK) ;
//		return ;
//	}
//
//	pDlg->Linking() ;
//}

// 080421 NYJ --- 낚시 다이얼로그 추가
void cWindowManager::CreateFishingDlg()
{
	CFishingDialog* dialog = ( CFishingDialog* )GetDlgInfoFromFile("Data/Interface/Windows/FishingDlg.bin", "rb" );

	if( ! dialog )
	{
		return;
	}

	AddWindow( dialog );
	GAMEIN->SetFishingDlg(dialog);
	GAMEIN->GetFishingDlg()->Linking();
}

void cWindowManager::CreateFishingGaugeDlg()
{
	CFishingGaugeDialog* dialog = ( CFishingGaugeDialog* )GetDlgInfoFromFile("Data/Interface/Windows/FishingProgressDialog.bin", "rb" );

	if( ! dialog )
	{
		return;
	}

	AddWindow( dialog );
	GAMEIN->SetFishingGaugeDlg(dialog);
	GAMEIN->GetFishingGaugeDlg()->Linking();
}

void cWindowManager::CreateFishingPointDlg()
{
	CFishingPointDialog* dialog = ( CFishingPointDialog* )GetDlgInfoFromFile("Data/Interface/Windows/FishingPointDlg.bin", "rb" );

	if( ! dialog )
	{
		return;
	}

	AddWindow( dialog );
	GAMEIN->SetFishingPointDlg(dialog);
	GAMEIN->GetFishingPointDlg()->Linking();
}

void cWindowManager::CreateWeatherDlg()
{
	CWeatherDialog* dialog = ( CWeatherDialog* )GetDlgInfoFromFile("Data/Interface/Windows/WeatherDlg.bin", "rb" );

	if( ! dialog )
	{
		return;
	}

	AddWindow( dialog );
	GAMEIN->SetWeatherDlg(dialog);
	GAMEIN->GetWeatherDlg()->Linking();
}

void cWindowManager::CreateFarmAnimalDlg()
{
	CAnimalDialog* dialog = ( CAnimalDialog* )GetDlgInfoFromFile("Data/Interface/Windows/FarmAnimalCageDlg.bin", "rb" );

	if( ! dialog )
	{
		return;
	}

	AddWindow( dialog );
	GAMEIN->SetAnimalDialog(dialog);
	GAMEIN->GetAnimalDialog()->Linking();
}

void cWindowManager::CreatePetDialog()
{
	CPetInfoDialog* pInfoDlg = ( CPetInfoDialog* )GetDlgInfoFromFile("Data/Interface/Windows/PetInfo.bin", "rb" );

	if( ! pInfoDlg )
	{
		return;
	}

	AddWindow( pInfoDlg );

	pInfoDlg->Linking();
	GAMEIN->SetPetInfoDlg( pInfoDlg );

	cWindow* pDlg = GetDlgInfoFromFile("Data/Interface/Windows/PetUI.bin", "rb" );

	if( !pDlg )
	{
		return;
	}

	AddWindow( pDlg );
	GAMEIN->SetPetUIDlg( (cDialog*)pDlg );

	CPetResurrectionDialog* pResDlg = ( CPetResurrectionDialog* )GetDlgInfoFromFile("Data/Interface/Windows/PetResDialog.bin", "rb" );

	if( ! pResDlg )
	{
		return;
	}

	AddWindow( pResDlg );

	pResDlg->Linking();
	GAMEIN->SetPetResurrectionDlg( pResDlg );
}


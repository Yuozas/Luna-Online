#pragma once


#include "cWindowDef.h"


//#ifdef _DEBUG
#include "cDebugString.h"
//#endif //_DEBUG

/********************************************************************/
/*	window는 dialog만을 delete하고 나머지는 dialog안에서 delete한다.
/********************************************************************/
class cWindow;
class cImage;
class cDialog;
class cIconDialog;
class cMultiLineText;
class CMousePointer;
class cScriptManager;
class cAnimationManager;
class cMsgBox;
class cDivideBox;
class CKeyboard;
class CMouse;

#define WINDOWMGR	USINGTON(cWindowManager)
typedef void (*cbDROPPROCESSFUNC)(LONG curDragX, LONG curDragY, LONG id, LONG beforeDragX, LONG beforeDragY);

class cWindowManager  
{
public:
	cWindowManager();
	virtual ~cWindowManager();

	void Init();
	void AfterInit();
	void Preprocess();
	void DestroyWindowProcess();
	void AddListDestroyWindow(cWindow * window);
	void Process();
	void Render();
	cbDROPPROCESSFUNC cbDragnDropProcess;
	void SetcbDropProcess(cbDROPPROCESSFUNC func) { cbDragnDropProcess = func; }
	cDialog * GetWindowForID(LONG id);
	cWindow * GetWindowForIDEx(LONG id);
	cDialog * GetWindowForXY(LONG x, LONG y);
	cDialog * GetWindowForXYExceptIcon(LONG x, LONG y);
	cDialog * GetWindowForXYExceptID(LONG x, LONG y, LONG id);
	cIconDialog * GetIconDialogForXY(LONG x, LONG y);
	void CloseWindowForID(LONG id);
	void DestroyWindowAll();
	void DeleteWindowForID(LONG id);
	void DeleteWindow( cWindow* pWindow );
	void DeleteEditBoxListAll();
	void DeleteIconDlgListAll();
	void DeleteChatTooltipListAll();
	void DeleteSSTitletipListAll();
	void Release();

	cbWindowProcess cbProcess;

//	cScriptManager * m_pScriptManager;			// SCRIPTMGR 로 대체.
	// ResourceManager Related
	cWindow * GetDlgInfoFromFile(char * filePath, char* mode = "rt");

	cImage * GetImagePath(int idx, char * path, int size)
	{
		//return m_pScriptManager->GetImagePath(idx,path,size);
		return NULL;
	}
	/*
	cImageBase * GetImageOfFile(char * filePath, int layer)
	{
		return m_pScriptManager->GetImageOfFile(filePath, layer);
	}
	*/
	cImage * GetImageOfFile(char * filePath, int layer)
	{
		//return m_pScriptManager->GetImageOfFile(filePath, layer);
		return NULL;
	}
//	MAKESINGLETON(cWindowManager);

	///////////////////////////////////////////////////////////
	// it's called different function according to game state
	void CreateGameIn();
	void CreateCharDlg();
	void CreateSkillTreeDlg();
	void CreateSkillSuryunDlg();
	void CreateInventoryDlg();
	void CreateExchangeDlg();
	void CreateRecvExchangeDlg();
	void CreateMixDlg();
	void CreateProgressDlog();
	void CreateDealDlg();
	// 061204 LYW --- Delete Preexistence Chatting Dialog.
	//void CreateChatDlg();
	// 061127 LYW --- Add Create Chatting Dialog Function For New Chatting System.
	void CreateChattingDlg() ;
	// 061219 LYW --- Add main system dialog.
	void CreateMainSystemDlg() ;
//SW050722 attach to minimap
//	//SW050715
//	void CreateJackpotDlg();
	//void CreateDebugdlg();
	void CreateHeroGuage();
	void CreateQuickDlg();
	void CreatePartyDlg();
	void CreatePartyCreateDlg();
	void CreatePartyInviteDlg();
	void CreateGuildCreateDlg();
	void CreateGuildDlg();
	void CreateGuildInviteDlg();
	void CreateGuildMarkDlg();
	void CreateGuildLevelupDlg();
	void CreateGuildNickNameDlg();
	void CreateGuildRankDlg();
	void CreateGuildWarehouseDlg();
	void CreateStorageDlg();
	void CreateNoteDlg();
	void CreateFriendDlg();
	void CreateMiniFriendDlg();
	void CreateMiniMapDlg();
	void CreateMiniNoteDlg();
	void CreateReviveDlg();
	void CreateWantNpcDlg();
	void CreateWantRegistDlg();
	void CreateQuestTotalDlg();
	void CreateQuestquickDlg() ;
	void CreateMPMissionDlg();
	void CreateMPNoticeDlg();
	void CreateMPGuageDlg();

	// LYJ 051017 구입노점상 추가
	void CreateStallKindSelectDlg();
	void CreateStreetBuyStallDlg();
	void CreateBuyRegDlg();

// LBS 03.10.16//////////////////
	void CreateStreetStallDlg();
	void CreateNpcScriptDlg();
/////////////////////////////////

//KES
	void CreateMacroDlg();
//	void CreateMacroModeDlg();
	void CreateChatOptionDlg();
	void CreateOptionDlg();
	void CreateExitDlg();
	void CreateCharStateDlg();
	void CreateMenuSlotDlg();

	void CreateEnchantDlg();
	void CreateReinforceDlg();
	void CreateDissolveDlg();

	// 080218 LUJ, 옵션 부여 창
	void CreateApplyOptionDlg();

	void CreateBailDlg();		
	void CreateEventNotifyDlg();
	void CreateMallNotcieDlg();

	void CreateItemShopDlg();
	void CreateMoveDlg();
	void CreatePointSaveDlg();
	
	// guildfieldwar
	void CreateGuildFieldWarDlg();

	void CreateSkPointDlg();
	void CreateSkPointNotifyDlg();
	
	// partywar
	void CreatePartyWarDlg();
	
	// shout
	//void CreateShoutDlg();
	// 061204 LYW --- Delete Preexistence Chatting Dialog.
	/*
	void CreateShoutchatDlg();
	*/

	//
	void CreateChaseDlg();
	void CreateChaseinputDlg();

	//
	void CreateNameChangeDlg();
	void CreateNameChangeNotifyDlg();

	void CreateGTRegistDlg();
	void CreateGTRegistcancelDlg();
	void CreateGTStandingDlg();
	void CreateGTBattleListDlg();
	void CreateGTScoreInfoDlg();

	void CreateSeigeWarDlg();

	void CreateItemSealDlg();
	void CreateChangeJobDlg();
	//void CreateReinforceResetDlg();
	//void CreateRareCreateDlg();

	// 06. 02. 강화 재료 안내 인터페이스 추가 - 이영준
	void CreateReinforceGuideDlg();
	// 06. 02. 내정보창 팁보기 추가 - 이영준
	void CreateTipBrowserDlgDlg();
	void CreateKeySettingTipDlg();

	void CreateGuildNoteDlg();
	void CreateUnionNoteDlg();
	// 06. 03. 문파공지 - 이영준
	void CreateGuildNoticeDlg();

	// 070816 웅주, 길드 창고 이용권한 부여
	void CreateGuildWarehouseRankDlg();

	void CreateGuildInvitationKindSelectionDlg();

	//screenshot
	void ScreenShotDlg();
	void CreateNpcImageDlg();


	// 2007/03/15 데이트 매칭 다이얼로그 추가 - 이진영
	void CreateDateMatchingDlg();

	// 070605 LYW --- cWindowManager : Add function to create identification.
	void CreateIdentificationDlg() ;

	// 070605 LYW --- cWindowManager : Add function to create favor icon dialog.
	void CreateFavorIconDlg() ;

	// desc_hseos_몬스터미터_01
	// S 몬스터미터 추가 added by hseos 2007.04.09
	void CreateMonstermeterDlg();
	// E 몬스터미터 추가 added by hseos 2007.04.09

	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.08.24	2007.09.10	2007.10.23
	void CreateFarmBuyDlg();
	void CreateFarmUpgradeDlg();
	void CreateFarmManageDlg();
	// E 농장시스템 추가 added by hseos 2007.08.24	2007.09.10	2007.10.23

	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.14  2007.11.15	2008.01.24
	void CreateDateZoneListDlg();
	void CreateChallengeZoneListDlg();
	void CreateChallengeZoneClearNo1Dlg();
	// E 데이트 존 추가 added by hseos 2007.11.14  2007.11.15	2008.01.24

	void CreateSkillTrainingDlg();

	void CreateFamilyCreateDlg();
	void CreateFamilyDlg();
	void CreateFamilyInviteDlg();
	void CreateFamilyMarkDlg();
//	void CreateFamilyLevelupDlg();
	void CreateFamilyNickNameDlg();
//	void CreateFamilyRankDlg();
//	void CreateFamilyWarehouseDlg();

	//void CreateGameNotifyDlg() ;

	// 071017 LYW --- cWindowManager : Add function to create tutorial dialog.
	void CreateTutorialDlg() ;

	// 071017 LYW --- cWindowManager : Add function to create help dialog.
	void CreateHelpDlg() ;

	// 070123 LYW --- cWindowManager : Add function to create notice button dialog of tutorial.
	void CreateTutorialBtnDlg() ;
	void CreateItemMallBtnDlg() ;
	void CreateItemPopupDlg();

	// 071130 LYW --- cWindowManager : Add function to create map move dialog.
	void CreateMapMoveDlg() ;

	// 071227 LYW --- cWindowManager : 캐릭터 이름 변경 다이얼로그 추가.
	void CreateChangeNameDlg() ;

	// 080109 LYW --- cWindowManager : 숫자 패드 다이얼로그 추가.
	void CreateNumberPadDlg() ;

	//---KES AUTONOTE
	void CreateAutoNoteDlg();
	void CreateAutoAnswerDlg();
	//---------------

	//---KES 상점검색
	void CreateStoreSearchDlg();
	//---------------

	//---BROWSER
	void CreateItemShopBrowserDlg();
	//---------------

	// 080414 KTH
	void CreatePetStorageDlg();
	//-----------

	void CreateShoutDlg();	// 080313 NYJ --- 전체외치기 다이얼로그 추가

	// 080403 LYW --- cWindowManager : Add functions to create dialog for chatroom system.
	void CreateChatRoomMainDlg() ;
	void CreateChatRoomCreateDlg() ;
	void CreateChatRoomDlg() ;
	void CreateChatRoomGuestListDlg() ;
	void CreateChatRoomOptionDlg() ;
	void CreateChatRoomJoinDlg() ;
	//void CreateChatRoomLobbyDlg() ;

	void CreateFishingDlg();// 080421 NYJ --- 낚시 다이얼로그 추가
	void CreateFishingGaugeDlg(); // 080422 NYJ --- 낚시게이지 다이얼로그 추가
	void CreateFishingPointDlg(); // 080508 NYJ --- 낚시포인트 다이얼로그 추가

	void CreateWeatherDlg();	// 080505 NYJ --- 날씨 다이얼로그 추가

	// 080414 LUJ, 외양 변경 창
	void CreateBodyChangeDlg();

	// 080509 KTH -- 가축 관리 다이얼 로그
	void CreateFarmAnimalDlg();

	void CreatePetDialog();

	cMsgBox* MsgBox( LONG lMBId, int nMBType, char* pStrMsg, ... );	
	cDivideBox * DivideBox( LONG lId, LONG x, LONG y, cbDivideFUNC cbFc1, cbDivideFUNC cbFc2, void * vData1, void * vData2, char* strTitle );
//

//pjs

	void CreatMousePoint();

	///////////////////////////////////////////////////////////

	//++++ Common dialog +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// dynamic create and auto release
//	cWindow * CreateMoneyDlg();
//	cWindow * CreateAlertDlg(char * text, WORD wType, void (*cbFunc)(LONG lId, void * p, DWORD we), void * object);
	cPtrList * m_pDestroyWindowRef;					// reference
//	CIndexGenerator m_CommonDlgIDIndex;
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	LONG ID_SEED;
	inline void AddWindow(cWindow * window)
	{ 
#ifdef _DEBUG
		if(!window)
		{
			char buff[256];
			sprintf(buff, "window가 NULL입니다. 스크립트의 오류이거나 스크립트 PATH를 확인하세요");//pjslocal
			LOGEX(buff, PT_MESSAGEBOX);
			return;
		}
		PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
		while(pos)
		{
			
			cWindow * win = (cWindow *)m_pWindowList->GetAt(pos);
			if(win == window)
			{
				char buff[256];
				sprintf(buff, "[AddWindow()] : 이미 동일한 다이얼로그가 존재합니다.");
				LOGEX(buff, PT_MESSAGEBOX);
			}
			m_pWindowList->GetNext(pos);
		}
#endif 
		m_pWindowList->AddTail((void *)window); 
	}


	void AddChatTooltip(cWindow * window)
	{
		m_pPlayerChatTooltip->AddTail(window);
	}
	void RemoveChatTooltipWindow(LONG playerID);	//필요없어지면 지운다. confirm
	void RemoveChatTooltipWindow( cWindow* pWindow );

	cPtrList * m_pWindowList;					
	cPtrList * m_pIconDlgListRef;					// reference
	cPtrList * m_pEditBoxListRef;					// for tab and shift+tab key	// reference
	cPtrList * m_pScreenTextList;					// 매 프레임마다 추가되고 삭제됨
	cPtrList * m_pPlayerChatTooltip;

	// 070124 LYW --- Add list for save id of actived.
	cPtrList * m_pActivedWindowList ;

	void SetDragStart(LONG sX, LONG sY, LONG x, LONG y, LONG id)
	{
		m_id = id;
		m_pDragDialog = GetWindowForID( id );
		m_OldX = x; m_OldY = y;
		m_posBeforeDrag.x=(float)sX;
		m_posBeforeDrag.y=(float)sY;
		m_OldDragFlag = TRUE;
	}
	void SetDragEnd()
	{
		//m_id=-1;
		//m_pDragDialog = NULL;
		m_OldX = -1; m_OldY = -1;
		m_OldDragFlag = FALSE;
	}

	void BackDragWindow();
	void DragWindowNull()
	{
		m_pDragDialog = NULL;
	}

	BOOL IsDragWindow() { return m_OldDragFlag;	}
	
	BOOL CloseAllWindow();
	void ShowBaseWindow();

	// 070124 LYW --- Add functino to save windows that actived.
	void SaveAllActivedWindow( cWindow* win ) ;
	void ShowAllActivedWindow() ;

	void SetOpendAllWindows( BOOL val ) { m_bOpendAllWindows = val ; }
	BOOL IsOpendAllWindows() { return m_bOpendAllWindows ; }


public:
	void ToggleShowInterface();
	
	BOOL m_OldDragFlag;
	void SetComposition(BOOL val){ m_fComposition = val; }
	BOOL IsComposition(){ return m_fComposition; }
	CMousePointer * GetMouseWindow() { return m_pMousePointWindow;	}

//KES
	cWindow* GetFocusedEdit() { return m_pFocusedEdit; }
	void SetFocusedEdit( cWindow* pEdit, BOOL bFocus );
	void SetNextEditFocus();
	void SetOptionAlpha( DWORD dwAlpha );
	void SetWindowTop( cWindow* win );
	void SetToolTipWindow( cMultiLineText* toolTip ) { m_pToolTipWindow = toolTip; }

//KES INTERFACE 031008
	BOOL IsMouseInputProcessed()	{ return m_MouseInputProcessed; }

	BOOL IsMouseOverUsed()			{ return m_bMouseOverUsed; }
	BOOL IsMouseDownUsed()			{ return m_bMouseDownUsed; }
	BOOL IsMouseUpUsed()			{ return m_bMouseUpUsed; }
//	BOOL IsKeyInputUsed()			{ return m_bKeyInputUsed; }
	
	void SetMouseInputProcessed()	{ m_MouseInputProcessed = TRUE; }
	void SetMouseOverUsed()			{ m_bMouseOverUsed = TRUE; }
	void SetMouseDownUsed()			{ m_bMouseDownUsed = TRUE; }
	void SetMouseUpUsed()			{ m_bMouseUpUsed = TRUE; }

	void KeyboardInput( CKeyboard* keyInfo );
	void MouseInput( CMouse* pMouse );
	
	cMsgBox* GetFirstMsgBox();
	void CloseAllMsgBox();
	void CloseAllMsgBoxNoFunc();
	void PositioningDlg( LONG& rX, LONG& rY );
///////////

	BOOL CloseAllAutoCloseWindows();
/////
//	cDialog* GetDragDlg() { return GetWindowForID( m_id ); }
	cDialog* GetDragDlg() { return m_pDragDialog; }


	// 윈도우들의 이벤트를 받는 부분
	static void OnWindowEvent(LONG lId, void * p, DWORD we);
//
protected:

	BOOL			m_MouseInputProcessed;
	BOOL			m_fComposition;

	cDialog*		m_pGuageWindow;
	cWindow*		m_pMonsterGuageWindow;
	CMousePointer*	m_pMousePointWindow;

//KES 030819
	cWindow*		m_pFocusedEdit;
	cMultiLineText* m_pToolTipWindow;
	BOOL			m_bMouseOverUsed;
	BOOL			m_bMouseDownUsed;
	BOOL			m_bMouseUpUsed;
	
//	BOOL			m_bKeyInputUsed;
//

	//for drag
	LONG		m_id;
	cDialog*	m_pDragDialog;	//KES추가

	LONG		m_OldX;
	LONG		m_OldY;
	VECTOR2		m_posBeforeDrag;

//==== for rec.
	BOOL		m_bHideInterface;

	// 070124 LYW --- Add variable for check that opend all windows.
	BOOL		m_bOpendAllWindows ;

public :
	BOOL		m_bMouseInWindow ;

	void SetMouseInWindow(BOOL val) 
	{
		m_bMouseInWindow = val ; 
	}
	BOOL IsMouseInWindow() { return m_bMouseInWindow ; }

public:
	BOOL		IsInterfaceHided() { return m_bHideInterface; }
// 070426 LYW --- cWindowManager : Add for test.
//#ifdef _DEBUG
private :
	cDebugString m_DebugString ;
public :
	cDebugString* GetDebugString() { return &m_DebugString ; }
//#endif //_DEBUG
};

EXTERNGLOBALTON(cWindowManager);

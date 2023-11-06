#include "stdafx.h"
#ifdef _CHEATENABLE_
#include "CheatMsgParser.h"
#endif	//_CHEATENABLE_

#ifdef _GMTOOL_
#include "GMToolManager.h"
#include "EventMapInfo.h"
#endif

#include "WindowIdEnum.h"
#include "interface/cWindowManager.h"
#include "../GlobalEventFunc.h"
#include "MacroManager.h"
#include "GameIn.h"
#include "MurimNet.h"
#include "QuickManager.h"
#include "MoveManager.h"
#include "PeaceWarModeManager.h"
#include "UngijosikManager.h"

#include "ObjectManager.h"
/*#include "StorageManager.h"
#include "FriendManager.h"
*/
#include "WantedManager.h"

#include "MainGame.h"
#include "MHCamera.h"
//#include "MinimapDialog.h"


#include "ChatManager.h"
//050630SW
#include "cChatTipManager.h"
//-
#include "input/cIMEWnd.h"

#include "input/UserInput.h"
#include "HelpDicManager.h"

#include "ShowdownManager.h"
#include "StreetStallManager.h"
#include "ExchangeManager.h"
#include "MouseCursor.h"

#include "HelpDialog.h"
#include "NoteManager.h"
#include "FriendManager.h"
//#include "StorageDialog.h"
//#include "NoteDialog.h"
//#include "FriendDialog.h"
#include "QuickDlg.h"
#include "QuickSlotDlg.h"
// 061220 LYW --- Delete this code.
//#include "MainBarDialog.h"
#include "CharacterDialog.h"
#include "InventoryExDialog.h"
#include "OptionDialog.h"
#include "ReviveDialog.h"
//#include "GuildCreateDialog.h"
//#include "GuildDialog.h"
//#include "GuildInviteDialog.h"
//#include "GuildMarkDialog.h"
//#include "GuildNickNameDialog.h"
//#include "GuildRankDialog.h"
//#include "GuildWarehouseDialog.h"
//#include "GuildManager.h"
//#include "GuildMarkManager.h"

#include "BattleSystem_Client.h"
#include "PKManager.h"
#include "QuestTotaldialog.h"
#include "QuestManager.h"
#include "MiniMapDlg.h"
#include "BigmapDlg.h"
#include "BailDialog.h"
#include "WindowIDEnum.h"
#include "cMsgBox.h"
#include "PartyIconManager.h"
#include "WantNpcDialog.h"
#include "FreeImage/FreeImage.h"

#include "ItemShopDialog.h"
#include "StatusIconDlg.h"
#include "MoveDialog.h"
#include "PointSaveDialog.h"
#include "ItemManager.h"
#include "ObjectStateManager.h"
#include "DissolveDialog.h"

#include "NoteDialog.h"
#include "SkillPointRedist.h"
#include "AppearanceManager.h"
#include "ChaseDialog.h"
#include "ChaseinputDialog.h"
#include "ShoutDialog.h"
#include "NameChangeNotifyDlg.h"
#include "EventNotifyDialog.h"
// Guild Tournament
#include "GTRegistDialog.h"
#include "GTRegistcancelDialog.h"
#include "GTStandingDialog.h"
#include "GTBattleListDialog.h"
#include "GTScoreInfoDialog.h"
//#include "ReinforceResetDlg.h"

#include "SealDialog.h"
#include "SeigeWarDialog.h"
#include "SiegeWarMgr.h"
#include "MussangManager.h"
#include "KeySettingTipDlg.h"

//screenshot
#include "ScreenShotDlg.h"
#include "cSkillTreeDlg.h"
#include "ExitDialog.h"

// 070122 LYW --- Include header file.
#include "cResourceManager.h"

// 070124 LYW --- Include header file.
#include "QuestDialog.h"
#include "..\[CC]Quest/QuestString.h"

#include "../FamilyDialog.h"

#include "../[cc]skill/client/manager/skillmanager.h"
#include "../[cc]skill/client/info/skillinfo.h"
#include "../[cc]skill/client/info/activeskillinfo.h"

#include "../DateMatchingDlg.h"

#include "Party.h"
#include "PartyManager.h"

// 080117 LYW --- MacroManager : 챌린지 맵인지 알아보기 위해 호출.
#include "../mhMap.h"
// 080117 LYW --- MacroManager : 챌린지 맵에서 ESC 키를 막기위해 호출.
#include "../hseos/Date/SHDateManager.h"


//---KES AUTO	//임시
#include "AutoNoteDlg.h"
#include "AutoAnswerDlg.h"
//-----------

#include "./ChatRoomMgr.h"
//#include "./ChatRoomMainDlg.h"
//#include "./ChatRoomDlg.h"

// 080428 NYJ --- 낚시관련
#include "FishingManager.h"
#include "FishingDialog.h"

#include "PetInfoDialog.h"


extern BOOL g_bActiveApp;

#ifdef _TESTCLIENT_																	// 테스트 클라이언트라면,

#include "MHMap.h"																	// 맵 헤더를 불러온다.
#include "ObjectManager.h"															// 오브젝트 매니져 헤더를 불러온다.
#include "EFFECT/EffectManager.h"													// 이펙트 매니져 해더를 불러온다.
#include "GameResourceManager.h"													// 게임 리소스 매니져 해더를 불러온다.
#include "interface/cFont.h"														// 폰트 해더를 불러온다.

void InitTestHero();																// 테스트용 히어로를 초기화 한다.
void InitTestMonster();																// 테스트용 몬스터를 초기화 한다.
void InitTestCharacters(void);														// 테스트용 캐릭터 파트를 초기화 한다.

#endif																				// 테스트 클라이언트 처리 종료.


extern BOOL g_bDisplayFPS;															// 프레임을 출력할지 여부를 담은 변수.
extern int g_nServerSetNum;															// 서버셋 번호를 담은 변수.

GLOBALTON(CMacroManager)

CMacroManager::CMacroManager()														// 매크로 매니져 생성자 함수.
{
	ZeroMemory( m_MacroKey, sizeof( m_MacroKey ) );									// 매크로키 구조체를 제로 메모리 한다.
	ZeroMemory( m_DefaultKey, sizeof( m_DefaultKey ) );								// 디폴트키 구조체를 제로 메모리 한다.

	ZeroMemory( m_MacroPressKey, sizeof( m_MacroPressKey ) );						// 매크로 프레스키를 제로 메모리 한다.
	ZeroMemory( m_DefaultPressKey, sizeof( m_DefaultPressKey ) );					// 디폴트 프레스키를 제로 메모리 한다.

	m_nMacroMode	= MM_CHAT;														// 매크로 모드를 채팅으로 한다.
	m_bChatMode		= TRUE;															// 채팅모드를 TRUE로 세팅한다.
	m_bPause		= FALSE;														// 포즈를 FALSE로 세팅한다.
	m_bInVisibleCursor = FALSE;														// 커서의 출력여부를 FALSE로 세팅한다.

	m_nCurTargetIdx = 0 ;															// 자동 타겟의 인덱스를 0으로 세팅한다.
	m_dwTargetID = 0 ;																// 자동 타겟의 아이디를 0으로 세팅한다.
}

CMacroManager::~CMacroManager()														// 매크로 매니져 소멸자 함수.
{
}


void CMacroManager::KeyboardInput_Normal( BOOL bDock, CKeyboard* keyInfo )
{
	if( !g_bActiveApp || m_bPause )													// 프로그램이 활성화 되지 않았거나, 포즈상태면,
	{
		return;																		// 리턴한다.							
	}
	
	if(HEROID == 0)																	// 히어로 아이디가 0과 같으면,
	{
		return;																		// 리턴한다.
	}

	if( MAINGAME->GetCurStateNum() != eGAMESTATE_GAMEIN )							// 메인 게임의 상태가 게임 인 상태가 아니면,
	{
		return;																		// 리턴한다.
	}

	//if (keyInfo->GetKeyPressed(88) == 1)											//f12를 누를경우 시계 다이얼로그를 보인다.
	//{
	//	PlayMacro(ME_SCREENCAPTURE) ;												//키클릭후 랜더링이 작동하므로 키다운시 렌더링하고 키업시 스크린샷 저장.
	//}

	bDock = FALSE;																	// 도킹여부를 담을 변수를 FALSE로 세팅한다.

	if( CIMEWND->IsDocking() )														// IMEWND가 도킹중이라면,
	{
		if( m_nMacroMode == MM_MACRO )												// 매크로 모드가 단축키 우선모드라면,
		{
			bDock = TRUE;															// 도킹 변수를 TRUE로 세팅한다.
		}
	}
	
	int nSysKey = MSK_NONE;															// 시스템키를 MSK_NONE으로 세팅한다.

	if( keyInfo->GetKeyPressed( KEY_MENU ) )										// 메뉴키가 눌렸다면,
	{
		nSysKey = 0;																// 시스템키를 0으로 세팅하고,
		nSysKey |= MSK_ALT;															// ALT키를 추가한다.
	}
	if( keyInfo->GetKeyPressed( KEY_CONTROL ) )										// 컨트롤 키가 눌렸다면,
	{
		nSysKey &= ~MSK_NONE;														// 시스템 키를 MSK_NONE의 역으로 세팅한다.
		nSysKey |= MSK_CTRL;														// 시스템 키에 CTRL키를 추가한다.
	}
	if( keyInfo->GetKeyPressed( KEY_SHIFT ) )										// 시프트키가 눌렸다면,
	{
		nSysKey &= ~MSK_NONE;														// 시스템 키를 MSK_NONE의 역으로 세팅한다.
		nSysKey |= MSK_SHIFT;														// 시스템 키에 SHIFT를 추가한다.
	}

	
	for( int i = 0 ; i < ME_COUNT ; ++i )											// 매크로 총 개수 만큼 포문을 돌린다.
	{
		if( bDock && !m_MacroKey[m_nMacroMode][i].bAllMode )						// 도킹중이고, 현재 매크로키가 bAllMode가 아니면,
		{
			continue;																// 컨티뉴한다.
		}

		if( m_MacroKey[m_nMacroMode][i].nSysKey & nSysKey )							// 위의 특수키들과 매크로 키가 일치하면,
		{
			if( m_MacroKey[m_nMacroMode][i].bUp )									// 현재 매크로키의 bUp이 TRUE와 같으면,
			{
				if( keyInfo->GetKeyUp( m_MacroKey[m_nMacroMode][i].wKey ) )			// 매크로키가 업되었다면,
				{
					PlayMacro( i );													// 매크로를 플레이한다.
				}
			}
			else																	// 현재 매크로키의 bUp이 FALSE와 같으면,
			{
				if( keyInfo->GetKeyDown( m_MacroKey[m_nMacroMode][i].wKey ) )		// 매크로 키가 다운되었다면,
				{
					PlayMacro( i );													// 매크로를 플레이 한다.
				}
			}
		}
	}
}

void CMacroManager::KeyboardInput_GMTool( BOOL bDock, CKeyboard* keyInfo )
{
#if defined(_GMTOOL_)
	if( (keyInfo->GetKeyDown(KEY_PADENTER) || (keyInfo->GetKeyDown(KEY_RETURN)))  // 엔터키와 ALT키가 눌렸거나,
		&& keyInfo->GetKeyPressed(KEY_MENU) )										// 숫자패드의 엔터키와 ALT키가 눌렸으면,
	{
		if( GMTOOLMGR->IsShowing() )												// GM툴이 보이고 있는상태라면,
		{
			GMTOOLMGR->ShowGMDialog( FALSE, TRUE );									// GM툴을 숨긴다.
		}
		else																		// GM툴이 숨겨진 상태라면,
		{
			GMTOOLMGR->ShowGMDialog( TRUE, TRUE );									// GM툴을 꺼낸다.
		}
	}
	else if( keyInfo->GetKeyDown(KEY_SUBTRACT) && keyInfo->GetKeyPressed(KEY_MENU) )// ALT키와 숫자패드의 -키가 눌렸다면,
	{
		MSGBASE msg;																// 기본 메시지 구조체를 선언한다.

		msg.Category	= MP_CHEAT;													// 카테고리를 치트로 세팅한다.
		msg.Protocol	= MP_CHEAT_AGENTCHECK_SYN;									// 프로토콜을 에이전트 체크 싱크로  세팅한다.
		msg.dwObjectID	= HEROID;													// 오브젝트 아이디에 히어로 아이디를 세팅한다.

		NETWORK->Send( &msg, sizeof(msg) );											// 메시지를 전송한다.
	}
#endif
}

void CMacroManager::KeyboardInput_CheatEnable( BOOL bDock, CKeyboard* keyInfo )
{	
#ifdef _TESTCLIENT_	
	KeyboardInput_Cheat_TestClient( bDock, keyInfo ) ;								// 테스트 클라이언트 일 때 처리를 한다.
#else
	KeyboardInput_Cheat_NotTestClient( bDock, keyInfo ) ;							// 테스트 클라이언트가 아닐 때 처리를 한다.
#endif // _TESTCLIENT_
}

void CMacroManager::KeyboardInput_Cheat_TestClient( BOOL bDock, CKeyboard* keyInfo )
{
#ifdef _TESTCLIENT_	

	if(keyInfo->GetKeyDown(KEY_1) && keyInfo->GetKeyPressed(KEY_CONTROL))			// CTRL키와 1번키가 눌렸다면,
	{
		SKILLMGR->Release();														// 스킬매니져를 해제한다.
		SKILLMGR->Init();															// 스킬 매니져를 초기화 한다.
	}

	if(keyInfo->GetKeyDown(KEY_2) && keyInfo->GetKeyPressed(KEY_CONTROL))			// CTRL키와 2번키가 눌렸다면,
	{
		EFFECTMGR->Release();														// 이펙트 매니져를 해제한다.
		EFFECTMGR->Init();															// 이펙트 매니져를 초기화 한다.
	}

	if(keyInfo->GetKeyDown(KEY_3) && keyInfo->GetKeyPressed(KEY_CONTROL))			// CTRL키와 3번키가 눌렸다면,
	{
		EFFECTMGR->RemoveAllEffect();												// 모든 이펙트를 해제 한다.

		GAMERESRCMNGR->LoadTestClientInfo();										// 테스트 클라이언트 정보를 로딩한다.

		OBJECTMGR->RemoveAllObject();												// 모든 오브젝트를 해제한다.

		GAMERESRCMNGR->ResetMonsterList();											// 몬스터 리스트를 다시 세팅한다.
		GAMERESRCMNGR->LoadMonsterList();											// 몬스터 리스트를 로딩한다.

		InitTestHero();																// 테스트 히어로를 초기화 한다.
		InitTestMonster();															// 테스트 몬스터를 초기화 한다.
		InitTestCharacters();														// 테스트 캐릭터를 초기화 한다.
	}

	if(keyInfo->GetKeyDown(KEY_4) && keyInfo->GetKeyPressed(KEY_CONTROL))			// CTRL키와 4번키가 눌렸다면,
	{
		MAP->LoadMapDesc(MAP->GetMapNum());											// 맵 정보를 로딩한다.
		MAP->ApplyMapDesc();														// 맵 정보를 적용한다.
	}
	if(keyInfo->GetKeyDown(KEY_ESCAPE))												// ESC키가 눌렸다면,
	{
		EFFECTMGR->SetEndFlagAllEffect();											// 모든 이펙트의 종료 플레그를 세팅한다.
		OBJECTMGR->SetSelectedObject(NULL);											// 선택된 오브젝트를 해제 한다.
	}

	if(keyInfo->GetKeyDown(KEY_SUBTRACT))											// 숫자패드의 -키가 눌렸다면,
	{
		MAINGAME->SetGameState( eGAMESTATE_MURIMNET );								// 게임 상태를 eGAMESTATE_MURIMNET로 세팅한다.
	}

	static BOOL bWireFrame = FALSE;													// 와이어 프레임 여부를 FALSE로 세팅한다.

	if(keyInfo->GetKeyDown(KEY_F12) && keyInfo->GetKeyPressed(KEY_MENU))			// ALT키와 F12가 눌렸다면,
	{
		if( !bWireFrame )															// 와이어 프레임 여부가 FALSE라면,
		{			
			g_pExecutive->GetRenderer()->SetRenderMode(2);							// 렌더모드를 와이어 프레임으로 한다.

			bWireFrame = TRUE;														// 와이어 프레임 여부를 TRUE로 세팅한다.
		}
		else																		// 와이어 프레임 여부가 TRUE라면,
		{
			g_pExecutive->GetRenderer()->SetRenderMode(0);							// 렌더모드를 솔리드로 한다.

			bWireFrame = FALSE;														// 와이어 프레임 여부를 FALSE로 세팅한다.
		}
	}

	if(keyInfo->GetKeyDown(KEY_R) && keyInfo->GetKeyPressed(KEY_MENU))				// ALT와 R키가 눌렸다면,
	{
		EFFECTMGR->StartEffectProcess(1034,HERO,NULL,0,0);							// 1034 이펙트를 시작한다.
	}

	if(keyInfo->GetKeyDown(KEY_4) && keyInfo->GetKeyPressed(KEY_MENU))				// ALT키와 4번 키가 눌렸다면,
	{
		EFFECTMGR->StartHeroEffectProcess(FindEffectNum("Mall_eff_heal.beff"));		// 힐 이펙트를 시작한다.
	}

	if(keyInfo->GetKeyDown(KEY_5) && keyInfo->GetKeyPressed(KEY_MENU))				// ALT키와 5번 키가 눌렸다면,
	{
		EFFECTMGR->StartHeroEffectProcess(FindEffectNum("m_ba_070.beff"));			// 이펙트를 시작한다.
	}

	if(keyInfo->GetKeyDown(KEY_7) && keyInfo->GetKeyPressed(KEY_MENU))				// ALT키와 7번 키가 눌렸다면,
	{
		EFFECTMGR->StartHeroEffectProcess(eEffect_ShopItem_Revive);					// 샵아이템 부활 이펙트를 시작한다.
	}

	if(keyInfo->GetKeyDown(KEY_X) && keyInfo->GetKeyPressed(KEY_CONTROL))			// CTRL키와 X키가 눌렸다면,
	{
		CWantNpcDialog* pDlg = GAMEIN->GetWantNpcDialog() ;							// WantNpcDialog 정보를 받아온다.

		if( pDlg )																	// 다이얼로그 정보가 유효한지 체크한다.
		{
			pDlg->SetActive(TRUE);													// 다이얼로그를 활성화 한다.
			pDlg->ShowWantMode(eWantNpcMode_Npc);									// eWantNpcMode_Npc모드로 세팅한다.
		}
		
		SEND_WANTNPCLIST Info;														// 원하는 NPC리스트 구조체를 선언한다.
		Info.TotalPage = 6;															// 페이지를 설정한다.
		char buf[64];																// 임시 버퍼를 선언한다.
		
		for(int i=0; i<WANTUNIT_PER_PAGE; ++i)										// 페이지 당 원하는 유닛 수만큼 포문을 돌린다.
		{
			sprintf(buf, "아톰%d", i);												// 이름을 설정한다.
			SafeStrCpy(Info.WantNpcList[i].WantedName, buf, MAX_NAME_LENGTH+1);		// 각종 정보를 세팅한다.
			Info.WantNpcList[i].Prize = i+100;										// 
			Info.WantNpcList[i].VolunteerNum = i+50;
			Info.WantNpcList[i].WantedIDX = i+1;
		}

		GAMEIN->GetWantNpcDialog()->SetWantedList(&Info);							// 정보를 다이얼로그에 세팅한다.
		
	}

	if(keyInfo->GetKeyDown(KEY_W))													// W키가 눌렸다면,
	{
		CActionTarget Target;														// 액션 타겟을 선언한다.

		cSkillInfo* pInfo = NULL ;													// 스킬 인포를 받을 포인터를 선언한다.
		
		pInfo = SKILLMGR->GetSkillInfo(GAMERESRCMNGR->m_TestClientInfo.SkillIdx);	// 테스트 클라이언트 스킬인덱스로 스킬 정보를 받는다.

		if( !pInfo )																// 스킬 정보가 유효하지 않다면,
		{
			return;																	// 리턴한다.
		}

		cActiveSkillInfo* pSkillInfo = ( cActiveSkillInfo* )pInfo;					// 액티브 스킬 정보를 받는다.

		if(pSkillInfo->GetSkillInfo()->Target == 1)									// 스킬의 타겟이 1과 같으면,
		{
			CObject * targetObj = OBJECTMGR->GetSelectedObject();					// 선택한 타겟의 오브젝트 정보를 받는다.

			if( targetObj )															// 오브젝트 정보가 유효한지 체크한다.
			{
				Target.InitActionTarget(targetObj,NULL);							// 타겟을 대상으로 액션을 초기화 한다.
			}
		}
		else if(pSkillInfo->GetSkillInfo()->Target == 0)							// 스킬의 타겟이 0과 같으면,
		{
			Target.InitActionTarget(HERO,NULL);										// 히어로를 대상으로 액션을 초기화 한다.
		}
		
		SKILLMGR->OnSkillCommand( HERO, &Target, TRUE );							// 스킬 매니져 명령을 실행한다.
	}

	if(keyInfo->GetKeyDown(KEY_E))													// E키가 눌렸다면,
	{
		CObject * targetObj = OBJECTMGR->GetSelectedObject();						// 선택한 타겟의 오브젝트 정보를 받는다.

		if( targetObj )																// 오브젝터 정보가 유효한지 체크한다.
		{
			TARGETSET Target;														// 타겟 셋 구조체를 선언한다.

			Target.pTarget = targetObj;												// 타겟을 설정한다.
			memset(&Target.ResultInfo,0,sizeof(RESULTINFO));						// 결과 정보를 메모리 셋한다.
			Target.ResultInfo.bCritical = rand()%2 ? TRUE : FALSE;					// 크리티컬 여부를 설정한다.
			Target.ResultInfo.RealDamage = rand() % 50;								// 리얼 데미지를 설정한다.
			Target.ResultInfo.CounterDamage = 0;									// 카운터 데미지를 설정한다.

			// 이펙트를 시작한다.
			EFFECTMGR->StartEffectProcess(GAMERESRCMNGR->m_TestClientInfo.Effect,HERO,&Target,1,targetObj->GetID(),EFFECT_FLAG_HEROATTACK);
		}
	}

	if(keyInfo->GetKeyDown(KEY_4))													// 4번키가 눌렸다면,
	{
		CObject * targetObj = OBJECTMGR->GetSelectedObject();						// 타겟 오브젝트 정보를 받아온다.

		if( targetObj )																// 오브젝트 정보가 유효한지 체크한다.
		{
			VECTOR3 pos;															// 벡터를 선언한다.

			targetObj->GetPosition(&pos);											// 타겟의 위치를 받아온다.
			pos.z -= 3000;															// Z값을 설정한다.

			MOVEMGR->SetMonsterMoveType(targetObj,eMA_WALKAROUND);					// 몬스터의 이동 타입을 eMA_WALKAROUND로 설정한다.
			MOVEMGR->StartMoveEx(targetObj,gCurTime,&pos);							// 이동을 시작한다.
		}
	}

	if(keyInfo->GetKeyDown(KEY_5))													// 5번키가 눌련다면,
	{
		CObject * targetObj = OBJECTMGR->GetSelectedObject();						// 타겟 오브젝트 정보를 받아온다.

		if( targetObj )																// 오브젝트 정보가 유효한지 체크한다.
		{
			VECTOR3 pos;															// 벡터를 선언한다.

			targetObj->GetPosition(&pos);											// 타겟의 위치를 받아온다.
			pos.z -= 3000;															// Z값을 설정한다.

			MOVEMGR->SetMonsterMoveType(targetObj,eMA_PERSUIT);						// 몬스터의 이동 타입을 eMA_PERSUIT로 설정한다.
			MOVEMGR->StartMoveEx(targetObj,gCurTime,&pos);							// 이동을 시작한다.
		}
	}

	if(keyInfo->GetKeyDown(KEY_6))													// 6번키가 눌렸다면,
	{
		CObject * targetObj = OBJECTMGR->GetSelectedObject();						// 선택한 타겟 오브젝트 정보를 받아온다.

		if( targetObj )																// 오브젝트 정보가 유효한지 체크한다.
		{
			VECTOR3 pos;															// 벡터를 선언한다.

			targetObj->GetPosition(&pos);											// 타겟의 위치를 받아온다.
			pos.z -= 3000;															// Z값을 설정한다.

			MOVEMGR->SetMonsterMoveType(targetObj,eMA_RUNAWAY);						// 몬스터의 이동 타입을 eMA_RUNAWAY로 설정한다.
			MOVEMGR->StartMoveEx(targetObj,gCurTime,&pos);							// 이동을 시작한다.
		}
	}
#endif // _TESTCLIENT_
}

void CMacroManager::KeyboardInput_Cheat_NotTestClient( BOOL bDock, CKeyboard* keyInfo )
{
#if defined(_GMTOOL_)
	if(keyInfo->GetKeyDown(KEY_1) && keyInfo->GetKeyPressed(KEY_MENU))				// ALT키와 1번키가 눌렸다면,
	{
		CMoveDialog* pdlg = GAMEIN->GetMoveDialog();								// MoveDialog 정보를 받아온다.

		if( pdlg )																	// 다이얼로그 정보가 유효한지 체크한다.
		{
			pdlg->SetActive(!pdlg->IsActive());										// 다이얼로그를 활성,비활성화 한다.
		}
	}

	if(keyInfo->GetKeyDown(KEY_2) && keyInfo->GetKeyPressed(KEY_MENU))				// ALT키와 2번키가 눌렸다면,
	{
		CItemShopDialog* pdlg = GAMEIN->GetItemShopDialog();						// 아이템샵 다이얼로그를 불러온다.

		if( pdlg )																	// 다이얼로그 정보가 유효한지 체크한다.
		{
			pdlg->SetActive(!pdlg->IsActive());										// 다이얼로그를 활성, 비활성화 한다.
		}
	}
	if(keyInfo->GetKeyDown(KEY_3) && keyInfo->GetKeyPressed(KEY_MENU))				// ALT키와 2번키가 눌렸다면,
	{
		CPointSaveDialog* pdlg = GAMEIN->GetPointSaveDialog();						// 포인트 저장 다이얼로그를 불러 온다.

		if( pdlg )																	// 다이얼로그 정보가 유효한지 체크한다.
		{
			pdlg->SetActive(!pdlg->IsActive());										// 다이얼로그 활성, 비활성화 한다.
		}
	}

	//if(keyInfo->GetKeyDown(KEY_D) && keyInfo->GetKeyPressed(KEY_CONTROL))			// CTRL키와 D키가 눌렸다면,
	//{
	//	CDissolutionDialog* pDlg = GAMEIN->GetDissolutionDialog() ;					// CDissolutionDialog 다이얼로그를 받아온다.
	//	
	//	if( pDlg )																	// 다이얼로그 정보가 유효한지 체크한다.
	//	{
	//		pDlg->SetActive( !pDlg->IsActive() ) ;									// 다이얼로그를 활성, 비활성화 한다.
	//	}
	//}
	
	if(keyInfo->GetKeyDown(KEY_F1) && keyInfo->GetKeyPressed(KEY_CONTROL))			// CTRL키와 F1키가 눌렸다면,
	{
		g_bDisplayFPS = !g_bDisplayFPS;												// 프레임을 출력한다.
	}
	
	if(keyInfo->GetKeyDown(KEY_BACK) && keyInfo->GetKeyPressed(KEY_MENU))			// ALT키와 BACK 키가 눌리면,
	{
		WINDOWMGR->ToggleShowInterface();											// 인터페이스를 토글 시킨다.
	}

	if(keyInfo->GetKeyDown(KEY_P) && keyInfo->GetKeyPressed(KEY_MENU))				// ALT키와 P키가 눌리면,
	{
		m_bInVisibleCursor ^= TRUE;													// 커서 출력여부를 세팅한다.
		CURSOR->SetCheatMode( m_bInVisibleCursor );									// 커서를 치트모드로 세팅한다.
	}

	if( keyInfo->GetKeyDown(KEY_C) && keyInfo->GetKeyPressed(KEY_CONTROL))	// ALT키와 C키가 눌리면,
	{
		if( CAMERA->GetCameraMode() == eCM_EyeView )								// 카메라 모드가 EyeView와 같으면,
		{
			CAMERA->SetCameraMode( eCM_Free );										// 카메라 모드를 Free모드로 세팅한다.

			if( !WINDOWMGR->IsOpendAllWindows() )									// 모든 창들이 열려 있지 않으면,
			{
				WINDOWMGR->ShowAllActivedWindow() ;									// 활성화 된 모든 창을 열어준다.

				WINDOWMGR->SetOpendAllWindows( TRUE ) ;								// 모든 윈도우가 열린 것으로 세팅한다.

				WINDOWMGR->ShowBaseWindow() ;										// 기본 윈도우들을 보여준다.
			}

		}
		else																		// 카메라 모드가 EyeView와 같지 않으면,
		{
			CAMERA->SetCameraMode( eCM_EyeView );									// 카메라 모드를 EyeView로 세팅한다.

			if( WINDOWMGR->IsOpendAllWindows() )									// 모든 윈도우가 열려 있으면,
			{
				//WINDOWMGR->m_pActivedWindowList->RemoveAll() ;						// 활성화 된 창들을 담고 있는 리스트를 모두 비운다.

				WINDOWMGR->CloseAllWindow() ;										// 모든 윈도우를 닫는다.

				WINDOWMGR->SetOpendAllWindows( FALSE ) ;							// 모든 윈도우가 닫힌 것으로 세팅한다.
			}
		}
	}

	//---KES PK 071124 임시 PK모드 ON/OFF
	/*
	if(keyInfo->GetKeyDown(KEY_Z) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		if( !HERO->IsPKMode() )
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_PKMODE, MBT_YESNO, CHATMGR->GetChatMsg( 1197 ) );
		else
			PKMGR->HEROPKModeOff();
	}

	if(keyInfo->GetKeyDown(KEY_X) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		//cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_PKMODE, MBT_YESNO, "살기를 드러내시겠습니까? 기본 30분이상 지속되며, 불이익이 있을 수 있습니다." );
		PKMGR->CreateLootingDlg( 0 );
		PKMGR->SetPKLooting( TRUE );
	}
	*/
	//----------------------------------------------------------------

	//---KES AUTONOTE 임시 Dialog체크
/*
	if(keyInfo->GetKeyDown(KEY_Z) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		GAMEIN->GetAutoNoteDlg()->SetActive(TRUE);
	}

	if(keyInfo->GetKeyDown(KEY_X) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		GAMEIN->GetAutoAnswerDlg()->SetActive(TRUE);

		char strColor[4][16] = { "RED", "BLUE", "YELLOW", "WHITE" };	

		DWORD dwColorTable[4] = { 0, 1, 2, 3 };
		DWORD dwUnitKey[4];
		for( int i = 0 ; i < 4 ; ++i )
		{
			dwUnitKey[i] = rand()%4;
		}

		int swap = dwColorTable[dwUnitKey[3]];	dwColorTable[dwUnitKey[3]] = dwColorTable[dwUnitKey[2]];	dwColorTable[dwUnitKey[2]] = swap;
		swap = dwColorTable[dwUnitKey[2]];		dwColorTable[dwUnitKey[2]] = dwColorTable[dwUnitKey[1]];	dwColorTable[dwUnitKey[1]] = swap;
		swap = dwColorTable[dwUnitKey[1]];		dwColorTable[dwUnitKey[1]] = dwColorTable[dwUnitKey[0]];	dwColorTable[dwUnitKey[0]] = swap;

		GAMEIN->GetAutoAnswerDlg()->Shuffle( dwColorTable );
	}
*/
	//-------------------------------

	if( bDock == TRUE) return;														// 현재 도킹중이라면, 리턴한다.

	if(keyInfo->GetKeyDown(KEY_I) && keyInfo->GetKeyPressed(KEY_MENU))				// ALT키와 I키가 눌렸다면,
	{
		MSGBASE msg;																// 기본 메시지 구조체를 선언한다.

		msg.Category = MP_ITEM;														// 카테고리를 아이템으로 세팅한다.
		msg.Protocol = MP_ITEM_SHOPITEM_INFO_SYN;									// 프로토콜을 샵 아이템 정보 싱크로 한다.
		msg.dwObjectID = HERO->GetID();												// 오브젝트 아이디로 히어로 아이디를 세팅한다.

		NETWORK->Send(&msg, sizeof(msg));											// 메시지를 전송한다.
	}

	//if(keyInfo->GetKeyDown(KEY_N) && keyInfo->GetKeyPressed(KEY_CONTROL))			// CTRL키와 N키가 눌렸다면,
	//{
	//	CReinforceResetDlg* pDlg = GAMEIN->GetReinforceResetDlg() ;					// 강화 초기화 다이얼로그를 받아온다.

	//	if( pDlg )																	// 다이얼로그 정보가 유효한지 체크한다.
	//	{
	//		pDlg->SetActive( !GAMEIN->GetReinforceResetDlg()->IsActive() );			// 다이얼로그를 활성, 비활성화 한다.
	//	}
	//}
#else
	// 일반 클라이언트 일 때는 필요없으므로..
#endif
}

void CMacroManager::KeyboardInput( CKeyboard* keyInfo )
{
	BOOL bDock = FALSE;																// 도킹여부를 담을 변수를 FALSE로 세팅한다.

	KeyboardInput_Normal( bDock, keyInfo ) ;										// 일반적인 키보드 입력 처리를 한다.

#ifdef _GMTOOL_																		// GM툴 처리를 시작한다.

	KeyboardInput_GMTool( bDock, keyInfo ) ;										// GM툴일 때 키보드 입력 처리를 한다.

#endif																				// GM툴 처리 종료.


#ifdef _CHEATENABLE_																// 치트모드일 때 처리를 시작한다.

	KeyboardInput_CheatEnable( bDock, keyInfo ) ;									// 치트모드일 때 키보드 입력 처리를 한다.

#endif	//_CHEATENABLE_																// 치트모드일 때 처리를 종료 한다.

}

void CMacroManager::KeyboardPressInput( CKeyboard* keyInfo )
{
	if( !g_bActiveApp || m_bPause )	return;											// 프로그램이 활성화 중이지 않거나, 포즈상태이면 리턴한다.

	if( MAINGAME->GetCurStateNum() != eGAMESTATE_GAMEIN ) return;					// 게임인 상태가 아이면 리턴한다.

	if( CIMEWND->IsDocking() )														// 도킹상태이면, 
	{
		return;																		// 리턴한다.
	}

	int nSysKey = MSK_NONE;															// 시스템 키 값을 기본으로 세팅한다.

	if( keyInfo->GetKeyPressed( KEY_CONTROL ) )		nSysKey |= MSK_CTRL;			// 컨트롤 키가 눌렸으면, 컨트롤 키를 추가한다.
	if( keyInfo->GetKeyPressed( KEY_MENU ) )		nSysKey |= MSK_ALT;				// 알트 키가 눌렸으면, 알트키를 추가한다.
	if( keyInfo->GetKeyPressed( KEY_SHIFT ) )		nSysKey |= MSK_SHIFT;			// 시프트 키가 눌렸으면, 시프트키를 추가한다.


	for( int i = 0 ; i < MPE_COUNT ; ++i )											// 매크로 프레스 이벤트 수만큼 포문을 돌린다.
	{
		if( m_MacroPressKey[m_nMacroMode][i].nSysKey & nSysKey						// 현재 매크로 키와 시스템 키를 확인하고, 매크로 키가 눌렸으면,
			&& keyInfo->GetKeyPressed( m_MacroPressKey[m_nMacroMode][i].wKey ) )
		{
			PlayMacroPress( i );													// 매크로를 실행한다.
		}
	}		
}

void CMacroManager::GetMacro( int nMacroMode, sMACRO* pMacro )
{
	memcpy( pMacro, m_MacroKey[nMacroMode], sizeof(sMACRO) * ME_COUNT );			// 들어온 매크로 모드로 들어온 매크로 구조체에 매크로 정보를 복사한다.
}

void CMacroManager::SetMacro( int nMacroMode, sMACRO* pMacro )
{
	memcpy( m_MacroKey[nMacroMode], pMacro, sizeof(sMACRO) * ME_COUNT );			// 들어온 매크로 모드로 들어온 매크로 구조체를 멤버 매크로 구조체에 복사한다.
}

void CMacroManager::GetDefaultMacro( int nMacroMode, sMACRO* pMacro )
{
	memcpy( pMacro, m_DefaultKey[nMacroMode], sizeof(sMACRO) * ME_COUNT );			// 디폴트 매크로를 들어온 매크로 구조체에 복사한다.
}

void CMacroManager::SetMacroMode( int nMacroMode )
{
	m_nMacroMode = nMacroMode;														// 인자로 들어온 매크로 모드를 멤버 매크로 모드 변수에 복사한다.

	if( m_nMacroMode == MM_CHAT )													// 매크로모드가 채팅모드와 같다면,
	{
		m_bChatMode = TRUE;															// 채팅모드 변수를 TRUE로 세팅한다.
	}
	else																			// 매크로모드가 채팅모드와 같지 않다면,
	{
		m_bChatMode = FALSE;														// 채팅모드 변수를 FALSE로 세팅한다.
	}
}

void CMacroManager::LoadUserMacro()
{
	SetDefaultMacro( MM_MACRO );													// 디폴트 매크로를 단축키 우선 모드로 세팅한다.
	SetDefaultMacro( MM_CHAT );														// 디폴트 매크로를 채팅 우선 모드로 세팅한다.
	SetMacroMode( MM_MACRO );														// 매크로 모드를 단축키 우선 모드로 세팅한다.
}


BOOL CMacroManager::LoadMacro( LPCTSTR strPath )
{
	HANDLE	hFile;																	// 파일 핸들을 선언한다.

	hFile = CreateFile( strPath, GENERIC_READ, 0, NULL,								// 읽기모드로 들어온 패스의 파일을 읽어 핸들을 넘겨 받는다.
						OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

	if( hFile == INVALID_HANDLE_VALUE )												// 핸들을 받는데 실패했다면,
		return FALSE;																// 리턴처리를 한다.

	DWORD dwRead;																	// 실제로 읽은 바이트 수를 리턴받기 위한 출력용 인수.
	
	if( !ReadFile( hFile, m_MacroKey,												// 매크로키 구조체에, 매크로 키 사이즈 만큼 읽어들인다.
		sizeof( m_MacroKey ), &dwRead, NULL ) )										// 읽어들이는게 실패하면,
	{
		CloseHandle( hFile );														// 핸들을 닫는다.
		return FALSE;																// FALSE 리턴 처리를 한다.
	}

	if( !ReadFile( hFile, m_MacroPressKey,											// m_MacroPressKey구조체에 m_MacroPressKey 사이즈 만큼 읽어들인다.
		sizeof( m_MacroPressKey ), &dwRead, NULL ) )								// 읽어들이는게 실패하면,
	{
		CloseHandle( hFile );														// 핸들을 닫는다.
		return FALSE;																// FALSE 리턴 처리를 한다.
	}
	
	CloseHandle( hFile );															// 핸들을 닫는다.

	return TRUE;																	// TRUE 리턴 처리를 한다.
}

void CMacroManager::SaveUserMacro()
{
	SaveMacro( "./INI/MacroUserSet.mcr" );											// 매크로를 저장한다.
}

BOOL CMacroManager::SaveMacro( LPCTSTR strPath )
{
	HANDLE	hFile;																	// 파일 핸들을 선언한다.

	hFile=CreateFile( strPath, GENERIC_WRITE, 0, NULL,								// 정해진 패스로 쓰기모드로 파일 핸들을 받아온다.
						CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	if( hFile == INVALID_HANDLE_VALUE )												// 파일 핸들을 받는데 실패하면,
		return FALSE;																// FALSE 리턴 처리를 한다.
	
	DWORD dwWritten;																// 실제로 쓴 바이트 수를 리턴받기 위한 출력용 인수.
	
	if( !WriteFile( hFile, m_MacroKey,												// m_MacroKey 구조체에 담긴 정보를 m_MacroKey 구조체 사이즈 만큼 쓴다.
		sizeof( m_MacroKey ), &dwWritten, NULL ) )									// 쓰기에 실패한다면,
	{
		CloseHandle( hFile );														// 핸들을 닫는다.
		return FALSE;																// FALSE 리턴 처리를 한다.
	}

	if( !WriteFile( hFile, m_MacroPressKey,											// m_MacroPressKey 구조체에 담긴 정보를 m_MacroPressKey 구조체 사이즈 만큼 쓴다.
		sizeof( m_MacroPressKey ), &dwWritten, NULL ) )								// 쓰기에 실패한다면,
	{
		CloseHandle( hFile );														// 핸들을 닫는다.
		return FALSE;																// FALSE 리턴처리를 한다.
	}

	CloseHandle( hFile );															// 핸들을 닫는다.
	return TRUE;																	// TRUE 리턴 처리를 한다.
}

void CMacroManager::PlayMacro( int nMacroEvent )
{
	int nMacroMode = OPTIONMGR->GetGameOption()->nMacroMode ;						// 채팅 우선모드인지, 단축키 우선모드인지 체크한다.

	//if( nMacroMode == 0 ) return ;												// 매크로모드가 0과 같으면 리턴 처리를 한다.

	if( CAMERA->GetCameraMode() == eCM_EyeView )									// 카메라 모드가 eCM_EyeView와 같다면,
	{
		return;																		// 리턴 처리를 한다.
	}

	switch( nMacroEvent )															// 매크로 이벤트를 확인한다.
	{
		case ME_USE_QUICKITEM_1_01: 												// 메인 퀵슬롯 1번키 사용
		case ME_USE_QUICKITEM_1_02: 												// 메인 퀵슬롯 2번키 사용
		case ME_USE_QUICKITEM_1_03: 												// 메인 퀵슬롯 3번키 사용
		case ME_USE_QUICKITEM_1_04: 												// 메인 퀵슬롯 4번키 사용
		case ME_USE_QUICKITEM_1_05: 												// 메인 퀵슬롯 5번키 사용
		case ME_USE_QUICKITEM_1_06: 												// 메인 퀵슬롯 6번키 사용
		case ME_USE_QUICKITEM_1_07: 												// 메인 퀵슬롯 7번키 사용
		case ME_USE_QUICKITEM_1_08: 												// 메인 퀵슬롯 8번키 사용
		case ME_USE_QUICKITEM_1_09: 												// 메인 퀵슬롯 9번키 사용
		case ME_USE_QUICKITEM_1_10:	PM_UseMainQuickSlotItem(nMacroEvent) ;	break ;	// 메인 퀵슬롯10번키 사용

		case ME_USE_QUICKITEM_2_01: 												// 서브 퀵슬롯 1번키 사용 
		case ME_USE_QUICKITEM_2_02: 			 									// 서브 퀵슬롯 2번키 사용 
		case ME_USE_QUICKITEM_2_03: 			 									// 서브 퀵슬롯 3번키 사용 
		case ME_USE_QUICKITEM_2_04: 			 									// 서브 퀵슬롯 4번키 사용 
		case ME_USE_QUICKITEM_2_05: 			 									// 서브 퀵슬롯 5번키 사용 
		case ME_USE_QUICKITEM_2_06: 			 									// 서브 퀵슬롯 6번키 사용 
		case ME_USE_QUICKITEM_2_07: 			 									// 서브 퀵슬롯 7번키 사용 
		case ME_USE_QUICKITEM_2_08: 			 									// 서브 퀵슬롯 8번키 사용 
		case ME_USE_QUICKITEM_2_09: 			 									// 서브 퀵슬롯 9번키 사용 
		case ME_USE_QUICKITEM_2_10:	PM_UseSubQuickSlotItem(nMacroEvent) ;	break ;	// 서브 퀵슬롯10번키 사용 

		case ME_CHANGE_QUICKSLOT_01: 												// 메인 퀵슬롯을 1번으로 세팅함.
		case ME_CHANGE_QUICKSLOT_02: 												// 메인 퀵슬롯을 2번으로 세팅함.
		case ME_CHANGE_QUICKSLOT_03: 												// 메인 퀵슬롯을 3번으로 세팅함.
		case ME_CHANGE_QUICKSLOT_04: 												// 메인 퀵슬롯을 4번으로 세팅함.
		case ME_CHANGE_QUICKSLOT_05: 												// 메인 퀵슬롯을 5번으로 세팅함.
		case ME_CHANGE_QUICKSLOT_06: 												// 메인 퀵슬롯을 6번으로 세팅함.
		case ME_CHANGE_QUICKSLOT_07: 												// 메인 퀵슬롯을 7번으로 세팅함.
		case ME_CHANGE_QUICKSLOT_08:	PM_Change_MainSlotNum(nMacroEvent) ;break ;	// 메인 퀵슬롯을 8번으로 세팅함.

		case ME_INCREASE_MAIN_QUICKSLOT_NUM:														// 메인 퀵슬롯 번호를 증가 시킨다.
		case ME_DECREASE_MAIN_QUICKSLOT_NUM: PM_SetSlotPage(QI1_QUICKSLOTDLG, nMacroEvent) ; break ;// 메인 퀵슬롯 번호를 감소 시킨다.

		// 080707 LYW --- MacroManager : 확장 슬롯 단축키는 막는다.
		//case ME_INCREASE_SUB_QUICKSLOT_NUM: 														// 서브 퀵슬롯 번호를 증가 시킨다.
		//case ME_DECREASE_SUB_QUICKSLOT_NUM:  PM_SetSlotPage(QI2_QUICKSLOTDLG, nMacroEvent) ; break ;// 서브 퀵슬롯 번호를 감소 시킨다.

		case ME_SELECT_MYSELF: HERO->ShowObjectName( TRUE, NAMECOLOR_SELECTED ); OBJECTMGR->SetSelectedObject( HERO ); break ;// 내 자신을 선택한다.

		case ME_SELECT_PARTYMEMBER_1: 												// 첫 번째 파티 멤버를 선택한다.
		case ME_SELECT_PARTYMEMBER_2: 												// 두 번째 파티 멤버를 선택한다.
		case ME_SELECT_PARTYMEMBER_3: 												// 세 번째 파티 멤버를 선택한다.
		case ME_SELECT_PARTYMEMBER_4: 												// 네 번째 파티 멤버를 선택한다.
		case ME_SELECT_PARTYMEMBER_5: 												// 다섯 번째 파티 멤버를 선택한다.
		case ME_SELECT_PARTYMEMBER_6:	PM_SelectPartyMember(nMacroEvent) ;break ;	// 여섯 번째 파티 멤버를 선택한다.

		case ME_SELECT_NEAR_TARGET:		PM_SetAutoTarget() ;				break ;	// 가장 가까이에 있는 타겟을 선택한다.

		case ME_SCREENCAPTURE:					PM_ScreenCapture() ;		break ;	// 스크린을 캡쳐한다.

		case ME_TOGGLE_EXITDLG:					PM_Toggle_ExitDlg() ;		break ;	// 종료 메시지 창을 띄운다.

		case ME_TOGGLE_ALLINTERFACE:			PM_Toggle_AllInterface() ;	break ;	// 모든 인터페이스를 토글한다.
		
		case ME_TOGGLE_SKILLDLG:				PM_Toggle_SkillDlg() ;		break ;	// 스킬창을 토글한다.

		case ME_TOGGLE_FAMILYDLG:				PM_Toggle_FamilyDlg() ;		break ;	// 패밀리 창을 토글한다.

		case ME_TOGGLE_INVENTORYDLG:			PM_Toggle_InventoryDlg() ;  break ;	// 인벤토리를 토글한다.

		case ME_TOGGLE_HELP_SHOW:				PM_Toggle_ShowKeyTip() ;	break ;	// 도움말을 토글한다.

		case ME_TOGGLE_MINIMAP:					PM_Toggle_MiniMap() ;		break ;	// 미니맵을 토글한다.

		case ME_TOGGLE_BIGMAP:					PM_Toggle_BigMap() ;		break ;	// 빅맵을 토글한다.

		case ME_TOGGLE_OPTIONDLG:				PM_Toggle_OptionDlg() ;		break ;	// 옵션 창을 토글한다.

		case ME_TOGGLE_GUILDDLG:				PM_Toggle_GuildDlg() ;		break ;	// 길드 창을 토글한다.

		case ME_TOGGLE_QUESTDLG:				PM_Toggle_QuestDlg() ;		break ;	// 퀘스트 창을 토글한다.

		case ME_TOGGLE_CHARINFODLG:				PM_Toggle_CharInfoDlg() ;	break ;	// 캐릭터 정보창을 토글한다.

		case ME_WALK_RUN:						MOVEMGR->ToggleHeroMoveMode() ;	break ;	// 걷기/뛰기 모드를 전환한다.

		case ME_TOGGLE_CAMERAVIEWMODE:			CAMERA->ToggleCameraViewMode(); break ;	// 카메라 뷰 모드를 전환한다.

		case ME_TOGGLE_MATCHINGDLG:				PM_Toggle_MatchingDlg() ;		break ;	// 매칭 창을 토글한다.

		case ME_TOGGLE_FRIENDDLG:				PM_Toggle_FriendDlg() ;		break ;		// 친구 목록을 토글한다.
		case ME_TOGGLE_NOTEDLG:					PM_Toggle_NoteDlg() ;		break ;		// 쪽지 창을 토글한다.

		// 071025 LYW --- MacroManager : Add macro for help dialog.
		case ME_TOGGLE_HELPDLG :				PM_Toggle_HelpDlg() ;		break ;		// 도움말 창을 토글한다.

		case ME_FISHING_PULLINGBTN:				GAMEIN->GetFishingDlg()->Fishing_Pullling();	break;

		// 080429 LYW --- MacroManager : Add macro for chatroom system.
		case ME_TOGGLE_CHATROOM_LISTDLG :		PM_Toggle_ChatRoomListDlg() ; break ;	// 채팅방 리스트창을 토글한다.
		case ME_TOGGLE_CHATROOM_ROOMDLG :		PM_Toggle_ChatRoomDlg() ;	  break ;	// 채팅방을 토클한다.
		case ME_SETFOCUS_CHATROOM_CHATTING :	PM_SetFocus_ChatRoom_Chat() ; break ;	// 채팅창과 채팅방 사이로 에디트 박스 포커스를 이동시킨다.


		case ME_PET_INFO : 	PM_Toggle_Pet_Info();	break;

		default : break ;
	}
}

void CMacroManager::PM_Toggle_ExitDlg()
{
	cSkillObject* pSkill = HERO->GetCurrentSkill();
	
	if( pSkill )
	{
		SKILLMGR->SkillCancel( pSkill );
	}
	else if(GAMEIN->GetFishingDlg()->IsPushedStartBtn())		// 낚시중이면 낚시만 중단한다.
	{
		GAMEIN->GetFishingDlg()->Fishing_Cancel();
	}
	else if( !WINDOWMGR->CloseAllAutoCloseWindows() )									// 자동으로 닫히는 윈도우들을 닫는 것이 실패하면,?
	{
		// 080117 LYW --- MacroManager : 챌린지 맵에서는 ESC키 기능을 막는다.
		if (!g_csDateManager.IsChallengeZone(MAP->GetMapNum()))
		{
			CExitDialog * pWindow = GAMEIN->GetExitDialog();							// 종료 메시지 윈도우 정보를 받아온다.

			if(pWindow)																	// 종료 메시지 윈도우 정보가 유효하면,
			{
				pWindow->SetActive(!pWindow->IsActive()) ;								// 종료 메시지 윈도우를 활성, 비활성화 한다.
			}
		}
	}	

	CObject* pObject = NULL ;														// 오브젝트 포인터를 선언 및 초기화 한다.

	pObject = OBJECTMGR->GetObject(m_dwTargetID) ;									// 선택된 오브젝트 정보를 받아온다.

	if( pObject )																	// 오브젝트 정보가 유효하다면,
	{
		pObject->ShowObjectName( FALSE, NAMECOLOR_DEFAULT );						// 오브젝트 이름을 디폴트 색상으로 세팅한다.

		OBJECTMGR->SetSelectedObject(NULL) ;										// 오브젝트 선택을 해제 한다.
	}
}

void CMacroManager::PM_ScreenCapture()
{
	DIRECTORYMGR->SetLoadMode(eLM_Root);											// 로드 모드를 eLM_Root로 세팅한다.

	CreateDirectory( "ScreenShot", NULL );											// 스크린 샷 디렉토리를 생성한다.
	
	WORD wSrv = 0, wY = 0, wM = 0, wD = 0, wN = 0;									// 연도를 담을 변수들을 선언한다.
	WORD wHH = 0, wMM = 0, wSS = 0;													// 시간대를 담을 변수들을 선언한다.

	char fname[256];																// 파일명을 담을 임시 버퍼를 선언한다.

	SYSTEMTIME ti;																	// 시간 정보를 담을 구조체를 선언한다.
	GetLocalTime( &ti );															// 로컬시간을 받아온다.

	HANDLE hFile = CreateFile( "screenshot/ScreenShot.cnt", GENERIC_READ, 0, NULL,	// 읽기모드로 ScreenShot.cnt를 읽어 파일핸들을 넘겨 받는다.
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

	DWORD dwSize = 0;																// 사이즈를 담을 변수를 선언한다.

	if( hFile != INVALID_HANDLE_VALUE )												// 핸들을 넘겨받는데 성공했다면,
	{
		ReadFile( hFile, &wSrv, sizeof(wSrv), &dwSize, NULL );						// 서버셋 번호를 받는다.
		ReadFile( hFile, &wY, sizeof(wY), &dwSize, NULL );							// 년도를 받는다.
		ReadFile( hFile, &wM, sizeof(wM), &dwSize, NULL );							// 월을 받는다.
		ReadFile( hFile, &wD, sizeof(wD), &dwSize, NULL );							// 요일을 받는다.
		ReadFile( hFile, &wHH, sizeof(wHH), &dwSize, NULL );						// 시간을 받는다.
		ReadFile( hFile, &wMM, sizeof(wMM), &dwSize, NULL );						// 분을 받는다.
		ReadFile( hFile, &wSS, sizeof(wSS), &dwSize, NULL );						// 초를 받는다.
		ReadFile( hFile, &wN, sizeof(wN), &dwSize, NULL );							// 밀리 세컨드 값을 받는다.
		
		CloseHandle( hFile );														// 파일 핸들을 닫는다.

		if( !( wSrv == (WORD)g_nServerSetNum && ti.wYear == wY &&					// 읽어들인 정보들이
			ti.wMonth == wM && ti.wDay == wD && ti.wHour == wHH &&					// 로컬 시간대와 같지 않으면,
			ti.wMinute == wMM && ti.wSecond == wSS) )
		{
			wSrv = g_nServerSetNum; wY = ti.wYear; wM = ti.wMonth;					// GetLocalTime()으로 받은 정보로 변수들을 세팅한다.
			wD = ti.wDay; wN = 1; 
			wHH = ti.wHour; wMM = ti.wMinute; wSS = ti.wSecond;
		}					
	}
	else																			// 핸들을 받는데 실패했다면,
	{
		wSrv = g_nServerSetNum; wY = ti.wYear; wM = ti.wMonth;						// GetLocalTime()으로 받은 정보로 변수들을 세팅한다.
		wD = ti.wDay; wN = 1; 
		wHH = ti.wHour; wMM = ti.wMinute; wSS = ti.wSecond;
	}


	sprintf(fname,"ScreenShot/Luna_%02d_%02d%02d%02d_%02d%02d%02d_%03d.jpg",		// 파일명을 위에서 설정한 정보들을 합하여 설정한다.
		wSrv, wY%100, wM, wD, wHH, wMM, wSS, wN );

	BOOL rt = g_pExecutive->GetRenderer()->CaptureScreen(fname);					// 파일명으로 스크린을 캡쳐한다.

	if( rt )																		// 캡쳐한 결과가 성공이면,
	{
		hFile = CreateFile( "screenshot/ScreenShot.cnt", GENERIC_WRITE, 0, NULL,	// 쓰기모드로 ScreenShot.cnt 파일을 쓰고 
						CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );				// 파일 핸들을 넘겨 받는다.

		if( hFile != INVALID_HANDLE_VALUE )											// 파일 쓰기에 성공했다면,
		{
			++wN;																	// 밀리 세컨드 값을 증가시킨다.

			WriteFile( hFile, &wSrv, sizeof(wSrv), &dwSize, NULL );					// 서버셋 번호를 쓴다.
			WriteFile( hFile, &wY, sizeof(wY), &dwSize, NULL );						// 연도를 기록한다.
			WriteFile( hFile, &wM, sizeof(wM), &dwSize, NULL );						// 달을 기록한다.
			WriteFile( hFile, &wD, sizeof(wD), &dwSize, NULL );						// 요일을 기록한다.
			WriteFile( hFile, &wHH, sizeof(wHH), &dwSize, NULL );					// 시간을 기록한다.
			WriteFile( hFile, &wMM, sizeof(wMM), &dwSize, NULL );					// 분을 기록한다.
			WriteFile( hFile, &wSS, sizeof(wSS), &dwSize, NULL );					// 초를 기록한다.
			WriteFile( hFile, &wN, sizeof(wN), &dwSize, NULL );						// 밀리 초를 기록한다.

			CloseHandle( hFile );													// 파일 핸들을 닫는다.
		}

		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(387), fname );				// 스크린을 캡쳐 했다는 메시지를 출력한다.
	}
	else																			// 파일 쓰기에 실패했다면,
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(386) );					// 스크린 캡쳐 실패 메시지를 출력한다.
	}

	FIBITMAP* pBitmap = FreeImage_Load(FIF_TARGA, fname, TARGA_DEFAULT);			// 프리 이미지 정보를 로드한다.
	
	if(pBitmap)																		// 이미지 정보가 유효하면,
	{
		DeleteFile(fname);															// 파일을 삭제한다.

		int len = strlen(fname);													// 파일명의 길이를 구한다.

		fname[len-3] = 'j';															// 파일명에 jpg를 추가한다.
		fname[len-2] = 'p';
		fname[len-1] = 'g';

		FreeImage_SaveJPEG(pBitmap,fname,JPEG_QUALITYSUPERB);						// jpg 이미지로 저장을 한다.

		FreeImage_Unload(pBitmap);													// 받아온 이미지 정보를 반환한다.
	}
}

void CMacroManager::PM_UseMainQuickSlotItem( int nMacroEvent )
{
	int slotNum = -1 ;																// 슬롯 번호를 세팅한다.

	switch( nMacroEvent )															// 매크로 이벤트를 확인한다.
	{
	case ME_USE_QUICKITEM_1_01:		slotNum = 0 ;	break ;							// 슬롯 번호를 0으로 세팅한다.
	case ME_USE_QUICKITEM_1_02:		slotNum = 1 ;	break ;							// 슬롯 번호를 1으로 세팅한다.
	case ME_USE_QUICKITEM_1_03:		slotNum = 2 ;	break ;							// 슬롯 번호를 2으로 세팅한다.
	case ME_USE_QUICKITEM_1_04:		slotNum = 3 ;	break ;							// 슬롯 번호를 3으로 세팅한다.
	case ME_USE_QUICKITEM_1_05:		slotNum = 4 ;	break ;							// 슬롯 번호를 4으로 세팅한다.
	case ME_USE_QUICKITEM_1_06:		slotNum = 5 ;	break ;							// 슬롯 번호를 5으로 세팅한다.
	case ME_USE_QUICKITEM_1_07:		slotNum = 6 ;	break ;							// 슬롯 번호를 6으로 세팅한다.
	case ME_USE_QUICKITEM_1_08:		slotNum = 7 ;	break ;							// 슬롯 번호를 7으로 세팅한다.
	case ME_USE_QUICKITEM_1_09:		slotNum = 8 ;	break ;							// 슬롯 번호를 8으로 세팅한다.
	case ME_USE_QUICKITEM_1_10:		slotNum = 9 ;	break ;							// 슬롯 번호를 9으로 세팅한다.

	default :																		// 그 외의 이벤트는
		break ;																		// 아무것도 하지 않는다.
	}

	if( slotNum != -1 )																// 슬롯 번호를 체크한다.				
	{	
//		const DWORD remainedTick = 200;

		// 연속된 키 입력이 버퍼에 쌓여 서버에 연속적으로 요청하는 경우를 막는다.
//		static DWORD beginTick	= GetTickCount();
//		static DWORD endTick	= beginTick + remainedTick;

//		const DWORD currentTick = GetTickCount();

//		if( ( beginTick < endTick && endTick < currentTick ) ||
//			( beginTick > endTick && beginTick > currentTick && endTick < currentTick ) )
//		{
			QUICKMGR->UseQuickItem( GAMEIN->GetQuickDlg()->GetSlotDlg( 0 )->GetCurSheet(), slotNum );	// 세팅 된 슬롯 번호를 사용한다.

//			beginTick = currentTick;
//			endTick		= beginTick + remainedTick;
//		}
//		else
//		{
			//CHATMGR->AddMsg( CTC_SYSMSG, "1초 후 다시 입력하세요" );
//		}
	}
}

void CMacroManager::PM_UseSubQuickSlotItem( int nMacroEvent )
{
	// 080704 LYW --- MacroManager : 확장 슬롯 추가 처리.
	// 단축창 정보를 받는다.
	cQuickDlg* pQuickDlg = NULL ;
	pQuickDlg = GAMEIN->GetQuickDlg() ;
	if(!pQuickDlg) return ;

    // 확장 슬롯 정보를 받는다.
	cQuickSlotDlg* pExSlotDlg = NULL ;
	pExSlotDlg = pQuickDlg->GetExSlotDlg(0) ;
	if(!pExSlotDlg) return ;

	// 현재 탭 번호를 받는다.
	WORD wTab = 0 ;
	wTab = pExSlotDlg->GetCurSheet() ;

	int slotNum = -1 ;																// 슬롯 번호를 세팅한다.
																					
	switch( nMacroEvent )															// 매크로 이벤트를 확인한다.
	{																				
	case ME_USE_QUICKITEM_2_01:		slotNum = 0 ;	break ;							// 슬롯 번호를 0으로 세팅한다.
	case ME_USE_QUICKITEM_2_02:		slotNum = 1 ;	break ;							// 슬롯 번호를 1으로 세팅한다.
	case ME_USE_QUICKITEM_2_03:		slotNum = 2 ;	break ;							// 슬롯 번호를 2으로 세팅한다.
	case ME_USE_QUICKITEM_2_04:		slotNum = 3 ;	break ;							// 슬롯 번호를 3으로 세팅한다.
	case ME_USE_QUICKITEM_2_05:		slotNum = 4 ;	break ;							// 슬롯 번호를 4으로 세팅한다.
	case ME_USE_QUICKITEM_2_06:		slotNum = 5 ;	break ;							// 슬롯 번호를 5으로 세팅한다.
	case ME_USE_QUICKITEM_2_07:		slotNum = 6 ;	break ;							// 슬롯 번호를 6으로 세팅한다.
	case ME_USE_QUICKITEM_2_08:		slotNum = 7 ;	break ;							// 슬롯 번호를 7으로 세팅한다.
	case ME_USE_QUICKITEM_2_09:		slotNum = 8 ;	break ;							// 슬롯 번호를 8으로 세팅한다.
	case ME_USE_QUICKITEM_2_10:		slotNum = 9 ;	break ;							// 슬롯 번호를 9으로 세팅한다.
																					
	default :																		// 그 외의 이벤트는
		break ;																		// 아무것도 하지 않는다.
	}																				
																					
	if( slotNum != -1 )																// 슬롯 번호를 체크한다.				
	{		
		// 080704 LYW --- MacroManager : 확장 슬롯 추가 처리.
		//QUICKMGR->UseQuickItem( GAMEIN->GetQuickDlg()->GetSlotDlg( 1 )->GetCurSheet(), slotNum );	// 세팅 된 슬롯 번호를 사용한다.
		QUICKMGR->UseQuickItem( wTab+MAX_SLOTPAGE, slotNum );	// 세팅 된 슬롯 번호를 사용한다.
	}
}

void CMacroManager::PM_Change_MainSlotNum( int nMacroEvent )
{
	int slotNum = -1 ;																// 슬롯 번호를 세팅한다.
																					
	switch( nMacroEvent )															// 매크로 이벤트를 확인한다.
	{																				
	case ME_CHANGE_QUICKSLOT_01:		slotNum = 0 ;	break ;						// 슬롯 번호를 0으로 세팅한다.
	case ME_CHANGE_QUICKSLOT_02:		slotNum = 1 ;	break ;						// 슬롯 번호를 1으로 세팅한다.
	case ME_CHANGE_QUICKSLOT_03:		slotNum = 2 ;	break ;						// 슬롯 번호를 2으로 세팅한다.
	case ME_CHANGE_QUICKSLOT_04:		slotNum = 3 ;	break ;						// 슬롯 번호를 3으로 세팅한다.
	case ME_CHANGE_QUICKSLOT_05:		slotNum = 4 ;	break ;						// 슬롯 번호를 4으로 세팅한다.
	case ME_CHANGE_QUICKSLOT_06:		slotNum = 5 ;	break ;						// 슬롯 번호를 5으로 세팅한다.
	case ME_CHANGE_QUICKSLOT_07:		slotNum = 6 ;	break ;						// 슬롯 번호를 6으로 세팅한다.
	case ME_CHANGE_QUICKSLOT_08:		slotNum = 7 ;	break ;						// 슬롯 번호를 7으로 세팅한다.
																					
	default :																		// 그 외의 이벤트는
		break ;																		// 아무것도 하지 않는다.
	}																				
																					
	if( slotNum != -1 )																// 슬롯 번호를 체크한다.				
	{
		cQuickSlotDlg* pSlot = ( cQuickSlotDlg* )WINDOWMGR->GetWindowForID( QI1_QUICKSLOTDLG ) ;	// 메인 슬롯을 얻어온다.

		if(pSlot)																	// 슬롯 정보가 유효한지 체크한다.
		{
			pSlot->SelectPage( (WORD)slotNum );										// 페이지를 세팅한다.
		}
	}
}

void CMacroManager::PM_SetSlotPage( DWORD dwSlotID, int nMacroEvent )
{
	cQuickSlotDlg* pSlot = ( cQuickSlotDlg* )WINDOWMGR->GetWindowForID( dwSlotID ) ;// 메인 슬롯을 얻어온다.

	if(pSlot)																		// 슬롯 정보가 유효한지 체크한다.
	{
		WORD curPage = pSlot->GetCurSheet() ;										// 현재 페이지를 받아온다.

		switch( nMacroEvent )														// 매크로 이벤트를 확인한다.
		{
		case ME_INCREASE_MAIN_QUICKSLOT_NUM :										// 페이지를 증가시키는 이벤트라면,
		case ME_INCREASE_SUB_QUICKSLOT_NUM :
			{
				if( curPage >= MAX_SLOTPAGE-1 )										// 페이지가 마지막 페이지라면,
				{
					curPage = 0 ;													// 마지막 페이지로 세팅한다.
				}
				else																// 그렇지 않을경우,
				{
					curPage = curPage+1 ;											// 페이지를 하나 증가시킨다.
				}
			}
			break ;

		case ME_DECREASE_MAIN_QUICKSLOT_NUM :										// 페이지를 감소시키는 이벤트라면,
		case ME_DECREASE_SUB_QUICKSLOT_NUM :
			{
				if( curPage <= 0 )													// 페이지가 최소 페이지라면,
				{
					curPage = MAX_SLOTPAGE-1 ;										// 첫 페이지로 세팅한다.
				}
				else																// 그렇지 않을경우,
				{
					curPage = curPage-1 ;											// 페이지를 하나 감소시킨다.
				}
			}
			break ;
		}

		pSlot->SelectPage( curPage );												// 페이지를 세팅한다.
	}
}

void CMacroManager::PM_SelectPartyMember( int nMacroEvent )
{
	CParty* pParty = PARTYMGR->GetParty() ;												// 파티 정보를 받아온다.

	DWORD dwMasterID = pParty->GetMasterIdx() ;

	if( pParty )																		// 파티 정보가 유효한지 체크한다.
	{
		DWORD dwMemberID = 0 ;															// 멤버 아이디를 초기화 한다.

		if( dwMasterID == HEROID )
		{
			switch( nMacroEvent )															// 메크로 이벤트를 확인한다.
			{
			case ME_SELECT_PARTYMEMBER_1 :	dwMemberID = pParty->GetMemberID(1) ;	break ;	// 첫 번째 멤버 아이디를 받아온다.
			case ME_SELECT_PARTYMEMBER_2 :	dwMemberID = pParty->GetMemberID(2) ;	break ;	// 두 번째 멤버 아이디를 받아온다.
			case ME_SELECT_PARTYMEMBER_3 :	dwMemberID = pParty->GetMemberID(3) ;	break ;	// 세 번째 멤버 아이디를 받아온다.
			case ME_SELECT_PARTYMEMBER_4 :	dwMemberID = pParty->GetMemberID(4) ;	break ;	// 네 번째 멤버 아이디를 받아온다.
			case ME_SELECT_PARTYMEMBER_5 :	dwMemberID = pParty->GetMemberID(5) ;	break ;	// 다섯 번째 멤버 아이디를 받아온다.
			case ME_SELECT_PARTYMEMBER_6 :	dwMemberID = pParty->GetMemberID(6) ;	break ;	// 여섯 번째 멤버 아이디를 바아온다.
			}
		}
		else
		{
			DWORD dwCurMemberID ;
			DWORD dwMemberOrder[7] = {0, } ;
			int orderCount = 0 ;

			for( int count = 0 ; count < 7 ; ++count )
			{
				dwCurMemberID = 0 ;
				dwCurMemberID = pParty->GetMemberID(count) ;

				if( dwCurMemberID == HEROID ) continue ;

				dwMemberOrder[orderCount] = dwCurMemberID ;
				++orderCount ;
			}

			switch( nMacroEvent )															// 메크로 이벤트를 확인한다.
			{
			case ME_SELECT_PARTYMEMBER_1 :	dwMemberID = dwMemberOrder[0] ;	break ;	// 첫 번째 멤버 아이디를 받아온다.
			case ME_SELECT_PARTYMEMBER_2 :	dwMemberID = dwMemberOrder[1] ;	break ;	// 두 번째 멤버 아이디를 받아온다.
			case ME_SELECT_PARTYMEMBER_3 :	dwMemberID = dwMemberOrder[2] ;	break ;	// 세 번째 멤버 아이디를 받아온다.
			case ME_SELECT_PARTYMEMBER_4 :	dwMemberID = dwMemberOrder[3] ;	break ;	// 네 번째 멤버 아이디를 받아온다.
			case ME_SELECT_PARTYMEMBER_5 :	dwMemberID = dwMemberOrder[4] ;	break ;	// 다섯 번째 멤버 아이디를 받아온다.
			case ME_SELECT_PARTYMEMBER_6 :	dwMemberID = dwMemberOrder[5] ;	break ;	// 여섯 번째 멤버 아이디를 바아온다.
			}
		}

		if( dwMemberID != 0 )															// 멤버 아이디를 검사한다.
		{
			GAMEIN->GetPartyDialog()->SetClickedMemberID(dwMemberID);					// 해당하는 아이디의 멤버를 선택한다.
		}
	}
}
void CMacroManager::PM_Toggle_AllInterface()
{
	// 071205 LUJ	NPC 이미지 창에서 자체적으로 모든 창을 토글한다. 따라서 해당 창이 열려있을 때
	//				토글 동작을 수행하면 단독으로 표시되어야할 NPC 창 위에 다른 창들이 겹쳐서 
	//				표시되는 문제가 발생한다
	{
		cDialog* dialog = WINDOWMGR->GetWindowForID( NSI_SCRIPTDLG );
		ASSERT( dialog );

		if( dialog->IsActive() )
		{
			return;
		}
	}

	if( WINDOWMGR->IsOpendAllWindows() )											// 윈도우들이 열려있는 상태라면,
	{
		//WINDOWMGR->m_pActivedWindowList->RemoveAll() ;								// 열려있는 윈도우를 담고있는 리스트를 모두 비운다.
		WINDOWMGR->CloseAllWindow() ;												// 모든 윈도우를 닫는다.

		WINDOWMGR->SetOpendAllWindows( FALSE ) ;									// 모든 윈도우가 닫혀있는 상태로 세팅한다.
	}
	else																			// 윈도우들이 닫혀있는 상태라면,
	{
		WINDOWMGR->ShowAllActivedWindow() ;											// 활성화 된 모든 윈도우들을 보여준다.

		WINDOWMGR->SetOpendAllWindows( TRUE ) ;										// 모든 윈도우가 열려있는 상태로 세팅한다.

		WINDOWMGR->ShowBaseWindow() ;												// 기본적으로 보이는 윈도우들을 보여준다.
	}
}

void CMacroManager::PM_Toggle_SkillDlg()
{
	cSkillTreeDlg* pDlg = GAMEIN->GetSkillTreeDlg() ;								// 스킬창 정보를 받아온다.

	if( pDlg )																		// 스킬창 정보가 유효한지 체크한다.
	{
		pDlg->SetActive( !pDlg->IsActive() );										// 스킬창을 활성, 비활성화 한다.
	}
}

void CMacroManager::PM_Toggle_FamilyDlg()
{
	// 070621 LYW --- MacroManager : 패밀리 기능이 알파에 들어가지 않기 때문에 토글키 처리 수정.
	//if(HERO->GetFamilyIdx() != 0)													// 히어로의 패밀리 인덱스를 받아온다.
	//{
	//	//GAMEIN->GetFamilyDlg()->SetActive(!(GAMEIN->GetFamilyDlg()->IsActive()));
	//	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 780 ));
	//}
	//else
	//{
	//	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 369 ));
	//}

	CSHFamily* pFamily = HERO->GetFamily() ;	

	if( pFamily && pFamily->Get()->nMasterID != 0 )
	{
		CFamilyDialog* pDlg = GAMEIN->GetFamilyDlg() ;

		if( pDlg )
		{
			if( pDlg->IsActive() )
			{
				pDlg->SetActive(FALSE) ;
			}
			else
			{
				pDlg->UpdateAllInfo() ;
				pDlg->SetActive(TRUE) ;
			}
		}
	}
	else
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 369 ));
	}
}

void CMacroManager::PM_Toggle_InventoryDlg()
{
	CInventoryExDialog* pDlg = GAMEIN->GetInventoryDialog() ;						// 인벤토리 다이얼로그 정보를 받아온다.

	if( !pDlg ) return ;															// 다이얼로그 정보가 유효한지 체크한다.
	
	pDlg->SetActive( !pDlg->IsActive() ) ;											// 다이얼로그를 활성, 비활성화 한다.
}

void CMacroManager::PM_Toggle_ShowKeyTip()
{
	CKeySettingTipDlg* pDlg = GAMEIN->GetKeySettingTipDlg() ;						// 키 팁 다이얼로그 정보를 받아온다.

	if( pDlg )																		// 다이얼로그 정보가 유효한지 체크한다.
	{
		pDlg->SetMode( m_nMacroMode ) ;												// 매크로 모드를 설정한다.

		pDlg->SetActive(!pDlg->IsActive()) ;										// 다이얼로그를 활성, 비활성화 한다.
	}
}

void CMacroManager::PM_Toggle_MiniMap()
{
	CMiniMapDlg* pDlg = GAMEIN->GetMiniMapDialog() ;								// 미니맵 다이얼로그 정보를 받아온다.

	if( pDlg )																		// 다이얼로그 정보가 유효한지 체크한다.
	{
		pDlg->SetActive( !pDlg->IsActive() ) ;										// 다이얼로그를 활성, 비활성화 한다.
	}
}

void CMacroManager::PM_Toggle_BigMap()
{
	CBigMapDlg* pDlg = GAMEIN->GetBigMapDialog() ;									// 빅맵 다이얼로그 정보를 받아온다.

	if( pDlg )																		// 다이얼로그 정보가 유효한지 체크한다.
	{
		pDlg->SetActive( !pDlg->IsActive() ) ;										// 다이얼로그를 활성, 비활성화 한다.
	}
}

void CMacroManager::PM_Toggle_OptionDlg()
{
	COptionDialog* pDlg = GAMEIN->GetOptionDialog() ;								// 옵션 다이얼로그 정보를 받아온다.

	if( pDlg )																		// 다이얼로그 정보가 유효한지 체크한다.
	{
		pDlg->SetActive( !pDlg->IsActive() ) ;										// 다이얼로그를 활성, 비활성화 한다.
	}
}

void CMacroManager::PM_Toggle_GuildDlg() 
{
	// CGuildDialog;
	cDialog* dialog = WINDOWMGR->GetWindowForID( GD_GUILDDLG );
	ASSERT( dialog );

	dialog->SetActive( ! dialog->IsActive() );
}

//void CMacroManager::PM_Toggle_QuestDlg()
//{
//	CQuestTotalDialog* pDlg = GAMEIN->GetQuestTotalDialog() ;						// 퀘스트 다이얼로그 정보를 받아온다.
//
//	if( pDlg )																		// 다이얼로그 정보가 유효한지 체크한다.
//	{
//		pDlg->SetActive( !pDlg->IsActive() ) ;										// 다이얼로그를 활성, 비활성화 한다.
//
//		CQuestDialog* pQuestDialog = GAMEIN->GetQuestDialog() ;
//
//		if( pQuestDialog )
//		{
//			CQuestString* pString = NULL ;
//			pString = pQuestDialog->QuestTitleString(1) ;
//
//			if( pString )
//			{
//				pQuestDialog->SetSelectedQuestID( pString->GetQuestID() ) ;
//			}
//
//			pQuestDialog->QuestListReset() ;
//			pQuestDialog->GetQuestList()->ResetGuageBarPos() ;
//
//			pQuestDialog->QuestDescView(pString) ;
//			pQuestDialog->GetQuestDescList()->ResetGuageBarPos() ;
//		}
//	}
//}

void CMacroManager::PM_Toggle_QuestDlg()
{
	CQuestTotalDialog* pDlg = GAMEIN->GetQuestTotalDialog() ;						// 퀘스트 다이얼로그 정보를 받아온다.

	if( pDlg )																		// 다이얼로그 정보가 유효한지 체크한다.
	{
		pDlg->SetActive( !pDlg->IsActive() ) ;										// 다이얼로그를 활성, 비활성화 한다.

		CQuestDialog* pQuestDialog = GAMEIN->GetQuestDialog() ;

		if( pQuestDialog )
		{
			pQuestDialog->RefreshQuestList() ;

			CQuestString* pString = NULL ;

			if( pQuestDialog->GetQuestCount() > 0 )
			{
				DWORD dwQuestIdx = 0 ;
				dwQuestIdx = pQuestDialog->GetSelectedQuestID() ;

				if( dwQuestIdx < 1 ) 
				{
					dwQuestIdx = pQuestDialog->GetQuestIdx(1) ;
					pQuestDialog->SetSelectedQuestIdx(dwQuestIdx) ;
				}
			}

			pQuestDialog->RefreshQuestList() ;
		}
	}
}

void CMacroManager::PM_Toggle_CharInfoDlg()
{
	CCharacterDialog* pDlg = GAMEIN->GetCharacterDialog() ;

	if( pDlg )																		// 다이얼로그 정보가 유효한지 체크한다.
	{
		pDlg->RefreshFamilyInfo() ;													// 패밀리 정보를 새로고침 한다.
		pDlg->SetActive( !pDlg->IsActive() ) ;										// 다이얼로그를 활성, 비활성화 한다.
	}
}

void CMacroManager::PM_Toggle_MatchingDlg()
{
	const DATE_MATCHING_INFO& dateInfo = HERO->GetDateMatchingInfo() ;				// 매칭 정보를 받아온다.

	if(!dateInfo.bIsValid)															// 매칭 정보가 유효하지 않으면,
	{
		//CHATMGR->AddMsg(CTC_SYSMSG, "주민등록증을 발급 받아야 사용하실 수 있습니다.") ;
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 1284 ) );
	}
	else																			// 매칭 정보가 유효하다면,
	{
		CDateMatchingDlg* pDlg = GAMEIN->GetDateMatchingDlg() ;						// 매칭 다이얼로그 정보를 받아온다.

		if( pDlg )																	// 다이얼로그 정보가 유효한지 체크한다.
		{
			if( pDlg->IsActive() )													// 다이얼로그가 열려 있다면,
			{
				CDateMatchingChatDlg* pChatDlg = NULL ;								// 매칭 채팅 다이얼로그 정보를 받을 포인터 변수를 선언한다.

				pChatDlg = GAMEIN->GetDateMatchingDlg()->GetChatingDlg() ;			// 매칭 다이얼로그 정보를 받아온다.

				if( pChatDlg )														// 다이얼로그 정보가 유효한지 체크한다.
				{
					if( pChatDlg->IsOnChatMode() )									// 채팅 중이라면,
					{
						// notice end chatting.
						WINDOWMGR->MsgBox( MBI_END_CHATTING_CLOSE, MBT_YESNO, CHATMGR->GetChatMsg( 1304 ) );
					}
					else															// 채팅 중이 아니라면,
					{
						pDlg->SetActive(FALSE) ;									// 다이얼로그를 비활성화 한다.
					}
				}
			}
			else																	// 채팅 다이얼로그다 비활성화 중이라면,
			{
				pDlg->GetPartnerListDlg()->SettingControls() ;						// 파트너 리스트의 컨드롤들을 세팅한다.
				//pDlg->GetPartnerListDlg()->UpdateInfo() ;
				pDlg->SetActive(TRUE) ;												// 채팅 다이얼로그를 활성화 한다.
			}
		}
	}
}

void CMacroManager::PM_SetAutoTarget()
{
	cPtrList* pList = MOVEMGR->GetAutoTargetList() ;								// 자동 타겟 리스트를 받아온다.

	if( pList )																		// 리스트 정보가 유효한지 체크한다.
	{
		int nTargetCount = pList->GetCount() ;										// 리스트의 아이템수가 몇개인지 받는다.

		if( nTargetCount > 0 )														// 리스트에 타겟 대상이 담겨 있다면,
		{
			CObject* pObject = NULL ;												// 오브젝트 정보를 받을 포인터를 선언한다.

			DWORD* pObjectID = NULL ;												// 오브젝트 아이디를 받을 포인터를 선언한다.

			PTRLISTPOS pos = NULL ;													// 리스트상의 위치를 나타내는 포인터를 선언한다.

			if( m_nCurTargetIdx >= nTargetCount )									// 타겟 인덱스가 대상 수보다 크면,
			{
				m_nCurTargetIdx = 0 ;												// 타겟 인덱스를 맨 처음으로 세팅한다.
			}

			int nEscapeCount = 0 ;

			pos = pList->GetHeadPosition() ;

			/*while(pos)
			{
				pObjectID = NULL ;
				pObjectID = (DWORD*)pList->GetNext(pos) ;

				if( pObjectID )
				{
					pObject = OBJECTMGR->GetObject(*pObjectID) ;

					if( pObject )
					{
						if( pObject->GetState() != eObjectState_Die )
						{
							if( *pObjectID != m_dwTargetID )
							{
								OBJECTMGR->SetSelectedObject(pObject) ;	

								pObject->ShowObjectName( TRUE, RGB_HALF( 255, 141, 39 )) ;

								m_dwTargetID = *pObjectID ;

								return ;
							}
						}
					}
				}
			}*/

			while(nEscapeCount < 5)
			{
				pos = pList->FindIndex(m_nCurTargetIdx) ;

				if( pos )
				{
					pObjectID = (DWORD*)pList->GetAt(pos) ;								// 위치에 해당하는 오브젝트 아이디를 받아온다.

					if( pObjectID )														// 아이디 정보가 유효한지 체크한다.
					{
						pObject = OBJECTMGR->GetObject(*pObjectID);						// 오브젝트 정보를 받아온다.

						if( pObject )													// 오브젝트 정보가 유효한지 체크한다.
						{
							if( pObject->GetState() != eObjectState_Die )				// 오브젝트가 죽은 상태가 아니라면,
							{
								OBJECTMGR->SetSelectedObject(pObject) ;					// 오브젝트를 선택한다.

								pObject->ShowObjectName( TRUE, RGB_HALF( 255, 141, 39 )); // 이름을 빨간색으로 세팅한다.
								
								if( nTargetCount > m_nCurTargetIdx )					// 현재 타겟 인덱스보다 타겟 대상이 많으면,
								{
									++m_nCurTargetIdx ;									// 타겟 인덱스를 증가 시킨다.
								}
								else													// 그렇지 않으면,
								{
									m_nCurTargetIdx = 0 ;								// 타겟 인덱스를 0으로 세팅한다.
								}

								m_dwTargetID = *pObjectID ;								// 선택된 타겟의 아이디를 저장한다.

								break ;
							}
							else
							{
								if( nTargetCount > m_nCurTargetIdx )					// 현재 타겟 인덱스보다 타겟 대상이 많으면,
								{
									++m_nCurTargetIdx ;									// 타겟 인덱스를 증가 시킨다.
								}
								else													// 그렇지 않으면,
								{
									m_nCurTargetIdx = 0 ;								// 타겟 인덱스를 0으로 세팅한다.
								}
							}
						}
						else
						{
							if( nTargetCount > m_nCurTargetIdx )					// 현재 타겟 인덱스보다 타겟 대상이 많으면,
							{
								++m_nCurTargetIdx ;									// 타겟 인덱스를 증가 시킨다.
							}
							else													// 그렇지 않으면,
							{
								m_nCurTargetIdx = 0 ;								// 타겟 인덱스를 0으로 세팅한다.
							}
						}
					}
					else
					{
						if( m_nCurTargetIdx >= nTargetCount )									// 타겟 인덱스가 대상 수보다 크면,
						{
							m_nCurTargetIdx = 0 ;												// 타겟 인덱스를 맨 처음으로 세팅한다.
						}
						else
						{
							++m_nCurTargetIdx ;
						}
					}
				}

				++nEscapeCount ;
			}
		}
	}
}





void CMacroManager::PM_Toggle_FriendDlg()
{
	cDialog* dialog = WINDOWMGR->GetWindowForID( FRI_FRIENDDLG );
	ASSERT( dialog );

	if( dialog->IsActive() )
	{
		dialog->SetActive( FALSE );
	}
	else
	{
		FRIENDMGR->FriendListSyn( 1 );
	}
}





void CMacroManager::PM_Toggle_NoteDlg()
{
	cDialog* dialog = WINDOWMGR->GetWindowForID( NOTE_NOTEDLG );
	ASSERT( dialog );

	if( dialog->IsActive() )
	{
		dialog->SetActive( FALSE );
	}
	// 쪽지 창을 열려면 일단 서버에 목록을 요청해야한다.
	else
	{
		NOTEMGR->NoteListSyn( 1, eMode_NormalNote );
	}
}





void CMacroManager::PM_Toggle_HelpDlg()
{
	cDialog* pDlg = WINDOWMGR->GetWindowForID(HELP_DLG) ;
	ASSERT( pDlg ) ;

	pDlg->SetActive( !pDlg->IsActive() ) ;
}





// 080429 LYW --- MacroManager : Add functions to toggle chatroom part.
void CMacroManager::PM_Toggle_ChatRoomListDlg()
{
	cDialog* pDlg = NULL ;
	pDlg = WINDOWMGR->GetWindowForID(CRMD_DLG) ;

	ASSERT(pDlg) ;

	if(!pDlg)
	{
		CHATROOMMGR->Throw_Error("Failed to receive crmd!!", __FUNCTION__) ;
		return ;
	}

	//pDlg->SetActive(!pDlg->IsActive()) ;
	if(pDlg->IsActive())
	{
		pDlg->SetActive(FALSE) ;
	}
	else
	{
		CHATROOMMGR->ToggleChatRoomMainDlg() ;
	}
}





void CMacroManager::PM_Toggle_ChatRoomDlg()
{
	if(!CHATROOMMGR->GetMyRoomOwnerIdx())
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1498)) ;
		return ;
	}

	// 채팅방 다이얼로그.
	cDialog* pDlg = NULL ;
	pDlg = WINDOWMGR->GetWindowForID(CRD_DLG) ;

	ASSERT(pDlg) ;

	if(!pDlg)
	{
		CHATROOMMGR->Throw_Error("Failed to receive crd!!", __FUNCTION__) ;
		return ;
	}

	pDlg->SetActive(!pDlg->IsActive()) ;

	// 참여자 창.
	cDialog* pGuestDlg = NULL ;
	pGuestDlg = WINDOWMGR->GetWindowForID(CRGLD_DLG) ;

	ASSERT(pGuestDlg) ;

	if(!pGuestDlg)
	{
		CHATROOMMGR->Throw_Error("Failed to receive crd!!", __FUNCTION__) ;
		return ;
	}

	if(pGuestDlg->IsActive()) pGuestDlg->SetActive(FALSE) ;
	//pGuestDlg->SetActive(!pDlg->IsActive()) ;

	// 옵션 창.
	cDialog* pOptionDlg = NULL ;
	pOptionDlg = WINDOWMGR->GetWindowForID(CROD_DLG) ;

	ASSERT(pOptionDlg) ;

	if(!pOptionDlg)
	{
		CHATROOMMGR->Throw_Error("Failed to receive crd!!", __FUNCTION__) ;
		return ;
	}

	if(pOptionDlg->IsActive()) pOptionDlg->SetActive(FALSE) ;
	//pOptionDlg->SetActive(!pDlg->IsActive()) ;
}





void CMacroManager::PM_SetFocus_ChatRoom_Chat()
{
	// 채팅방 참여중인지 체크.
	if(!CHATROOMMGR->GetMyRoomOwnerIdx())
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1498)) ;
		return ;
	}



	// 채팅방 다이얼로그.
	cDialog* pDlg = NULL ;
	pDlg = WINDOWMGR->GetWindowForID(CRD_DLG) ;

	ASSERT(pDlg) ;

	if(!pDlg)
	{
		CHATROOMMGR->Throw_Error("Failed to receive crd!!", __FUNCTION__) ;
		return ;
	}

	// 채팅방이 비활성화 되어있다면, return 처리를 한다.
	if(!pDlg->IsActive()) return ;



	// 채팅방의 에디트박스 받기.
	cEditBox* roomBox = NULL ;
	roomBox = CHATROOMMGR->GetChatRoomInputBox() ;

	ASSERT(roomBox) ;

	if(!roomBox)
	{
		CHATROOMMGR->Throw_Error("Failed to receive creb!!", __FUNCTION__) ;
		return ;
	}



	// 일반 채팅창 받기.
	CChattingDlg* pChatDlg = NULL ;
	pChatDlg = GAMEIN->GetChattingDlg() ;

	ASSERT(pChatDlg) ;

	if(!pChatDlg)
	{
		CHATROOMMGR->Throw_Error("Failed to receive creb!!", __FUNCTION__) ;
		return ;
	}

	// 채팅창의 에디트 박스 받기.
	cEditBox* chatBox = NULL ;
	chatBox = pChatDlg->GetInputBox() ;

	ASSERT(pChatDlg) ;

	if(!pChatDlg)
	{
		CHATROOMMGR->Throw_Error("Failed to receive cteb!!", __FUNCTION__) ;
		return ;
	}



	// 채팅방과 채팅창의 포커스 이동 처리.
	if(roomBox->IsFocus())
	{
		roomBox->SetFocus(FALSE) ;
		roomBox->SetFocusEdit(FALSE) ;
		CIMEWND->SetDockingEx(roomBox->GetIME(), FALSE) ;
		chatBox->SetFocus(TRUE) ;
		chatBox->SetFocusEdit(TRUE) ;
		CIMEWND->SetDockingEx(chatBox->GetIME(), TRUE) ;
	}
	else if(chatBox->IsFocus())
	{
		chatBox->SetFocus(FALSE) ;
		chatBox->SetFocusEdit(FALSE) ;
		CIMEWND->SetDockingEx(chatBox->GetIME(), FALSE) ;
		roomBox->SetFocus(TRUE) ;
		roomBox->SetFocusEdit(TRUE) ;
		CIMEWND->SetDockingEx(roomBox->GetIME(), TRUE) ;
	}
	else
	{
		roomBox->SetFocus(TRUE) ;
		roomBox->SetFocusEdit(TRUE) ;
		CIMEWND->SetDockingEx(roomBox->GetIME(), TRUE) ;
	}
}


void CMacroManager::PM_Toggle_Pet_Info()
{
	if( !HEROPET )
		return;

	CPetInfoDialog* pDlg = GAMEIN->GetPetInfoDlg();

	if( !pDlg )
	{
		return;
	}

	if( pDlg->IsActive() )
	{
		pDlg->SetActive( FALSE );
	}
	else
	{
		pDlg->SetActive( TRUE );
	}
}


void CMacroManager::PlayMacroPress( int nMacroEvent )
{
	float Rate = 1.f;																// 회전 값.

	if( g_UserInput.GetKeyboard()->GetKeyPressed(KEY_RSHIFT) )						// 오른쪽 시프트가 눌렸다면,
	{
		Rate = 0.3f;																// 회전 값을 0.3f로 세팅한다.
	}

	switch( nMacroEvent )															// 인자로 들어온 매크로 이벤트를 확인한다.
	{
		case MPE_CAMERAMOVE_ZOOMIN:													// 줌인 매크로라면,
			{

#ifdef _GMTOOL_																		// GM툴일 때 처리를 시작한다.

				if( CAMERA->GetCameraMode() == eCM_EyeView )						// 카메라 모드가 eCM_EyeView와 같다면,
				{
					CAMERA->Fly( 0.8 * gTickTime*Rate*5 );							// 케미라의 fly 함수를 실행한다.				
				}
				else																// 카메라 모드가 eCM_EyeView와 같지 않다면,

#endif																				// GM툴 처리를 종료 한다.

					CAMERA->ZoomDistance( 0, -0.8 * gTickTime*Rate, 0 );			// 주어진 식으로 카메라 줌 거리를 조정한다.

			}
			break;

		case MPE_CAMERAMOVE_ZOOMOUT:												// 줌 아웃 매크로라면,
			{

#ifdef _GMTOOL_																		// GM툴 처리를 시작한다.

				if( CAMERA->GetCameraMode() == eCM_EyeView )						// 카메라 모드가 eCM_EyeView와 같다면,
				{
					CAMERA->Fly( -0.8 * gTickTime*Rate*5 );							// 주어진 값으로 카메라의 Fly 함수를 실행한다.
				}
				else																// 카메라 모드가 eCM_EyeView와 같지 않다면,

#endif																				// GM처리를 종료 한다.

					CAMERA->ZoomDistance( 0, 0.8 * gTickTime*Rate, 0 );				// 주어진 식으로 카메라 줌 거리를 조정한다.
			}
			break;

		case MPE_CAMERAMOVE_UP:														// 카메라 업 매크로라면,
			CAMERA->MouseRotate( 0, 0.3 * gTickTime*Rate );							// 카메라의 마우스 회전 처리를 한다.
			break;

		case MPE_CAMERAMOVE_DOWN:													// 카메라 다운 매크로라면,
			CAMERA->MouseRotate( 0, -0.3 * gTickTime*Rate );						// 카메라의 마우스 회전 처리를 한다.
			break;

		case MPE_CAMERAMOVE_RIGHT:													// 카메라 라이트 매크로라면,
			CAMERA->MouseRotate( -0.3 * gTickTime*Rate, 0 );						// 카메라의 마우스 회전 처리를 한다.
			break;

		case MPE_CAMERAMOVE_LEFT:													// 카메라 레프트 매크로라면,
			CAMERA->MouseRotate( 0.3 * gTickTime*Rate, 0 );							// 카메라의 마우스 회전 처리를 한다.
			break;

	}
}

void CMacroManager::Init()
{

	sMACRO DefaultMacro[MM_COUNT][ME_COUNT] = {
		{
			{ MSK_NONE,		KEY_F1,	FALSE, FALSE },									// 메인 퀵슬롯 1번키 사용
			{ MSK_NONE,		KEY_F2,	FALSE, FALSE },									// 메인 퀵슬롯 2번키 사용
			{ MSK_NONE,		KEY_F3,	FALSE, FALSE },									// 메인 퀵슬롯 3번키 사용
			{ MSK_NONE,		KEY_F4,	FALSE, FALSE },									// 메인 퀵슬롯 4번키 사용
			{ MSK_NONE,		KEY_F5,	FALSE, FALSE },									// 메인 퀵슬롯 5번키 사용
			{ MSK_NONE,		KEY_F6,	FALSE, FALSE },									// 메인 퀵슬롯 6번키 사용
			{ MSK_NONE,		KEY_F7,	FALSE, FALSE },									// 메인 퀵슬롯 7번키 사용
			{ MSK_NONE,		KEY_F8,	FALSE, FALSE },									// 메인 퀵슬롯 8번키 사용
			{ MSK_NONE,		KEY_F9,	FALSE, FALSE },									// 메인 퀵슬롯 9번키 사용
			{ MSK_NONE,		KEY_F10,FALSE, FALSE },									// 메인 퀵슬롯10번키 사용

			{ MSK_CTRL,		KEY_F1,	FALSE, FALSE },									// 서브 퀵슬롯 1번키 사용 
			{ MSK_CTRL,		KEY_F2,	FALSE, FALSE },									// 서브 퀵슬롯 2번키 사용 
			{ MSK_CTRL,		KEY_F3,	FALSE, FALSE },									// 서브 퀵슬롯 3번키 사용 
			{ MSK_CTRL,		KEY_F4,	FALSE, FALSE },									// 서브 퀵슬롯 4번키 사용 
			{ MSK_CTRL,		KEY_F5,	FALSE, FALSE },									// 서브 퀵슬롯 5번키 사용 
			{ MSK_CTRL,		KEY_F6,	FALSE, FALSE },									// 서브 퀵슬롯 6번키 사용 
			{ MSK_CTRL,		KEY_F7,	FALSE, FALSE },									// 서브 퀵슬롯 7번키 사용 
			{ MSK_CTRL,		KEY_F8,	FALSE, FALSE },									// 서브 퀵슬롯 8번키 사용 
			{ MSK_CTRL,		KEY_F9,	FALSE, FALSE },									// 서브 퀵슬롯 9번키 사용 
			{ MSK_CTRL,		KEY_F10,FALSE, FALSE },									// 서브 퀵슬롯10번키 사용 

			{ MSK_ALT,		KEY_1,	FALSE, FALSE },									// 메인 퀵슬롯을 1번으로 세팅함.
			{ MSK_ALT,		KEY_2,	FALSE, FALSE },									// 메인 퀵슬롯을 2번으로 세팅함.
			{ MSK_ALT,		KEY_3,	FALSE, FALSE },									// 메인 퀵슬롯을 3번으로 세팅함.
			{ MSK_ALT,		KEY_4,	FALSE, FALSE },									// 메인 퀵슬롯을 4번으로 세팅함.
			{ MSK_ALT,		KEY_5,	FALSE, FALSE },									// 메인 퀵슬롯을 5번으로 세팅함.
			{ MSK_ALT,		KEY_6,	FALSE, FALSE },									// 메인 퀵슬롯을 6번으로 세팅함.
			{ MSK_ALT,		KEY_7,	FALSE, FALSE },									// 메인 퀵슬롯을 7번으로 세팅함.
			{ MSK_ALT,		KEY_8,	FALSE, FALSE },									// 메인 퀵슬롯을 8번으로 세팅함.

			{ MSK_ALT,		KEY_OEM_3, FALSE, FALSE },								// 메인 퀵슬롯 번호를 증가 시킨다.
			{ MSK_CTRL,		KEY_OEM_3, FALSE, FALSE },								// 메인 퀵슬롯 번호를 감소 시킨다.

			{ MSK_ALT | MSK_SHIFT,	KEY_OEM_3, FALSE, FALSE },						// 서브 퀵슬롯 번호를 증가 시킨다.
			{ MSK_CTRL | MSK_SHIFT,	KEY_OEM_3, FALSE, FALSE },						// 서브 퀵슬롯 번호를 감소 시킨다.

			{ MSK_ALT,		KEY_F1,	FALSE, FALSE },									// 내 자신을 선택한다.

			{ MSK_ALT,		KEY_F2,	FALSE, FALSE },									// 첫 번째 파티 멤버를 선택한다.
			{ MSK_ALT,		KEY_F3,	FALSE, FALSE },									// 두 번째 파티 멤버를 선택한다.
			{ MSK_ALT,		KEY_F4,	FALSE, FALSE },									// 세 번째 파티 멤버를 선택한다.
			{ MSK_ALT,		KEY_F5,	FALSE, FALSE },									// 네 번째 파티 멤버를 선택한다.
			{ MSK_ALT,		KEY_F6,	FALSE, FALSE },									// 다섯 번째 파티 멤버를 선택한다.
			{ MSK_ALT,		KEY_F7,	FALSE, FALSE },									// 여섯 번째 파티 멤버를 선택한다.

			{ MSK_NONE,		KEY_TAB,	FALSE, FALSE },								// 가장 가까이에 있는 타겟을 선택한다.

			{ MSK_ALT,		KEY_F12,	FALSE, FALSE },								// 스크린을 캡쳐한다.

			{ MSK_NONE,		KEY_ESCAPE,	FALSE, FALSE },								// 종료 메시지 창을 띄운다.

			{ MSK_CTRL,		KEY_Z,	FALSE, FALSE },									// 모든 인터페이스를 토글한다.

			{ MSK_ALT,		KEY_S,	FALSE, FALSE },									// 스킬창을 토글한다.

			{ MSK_ALT,		KEY_F,	FALSE, FALSE },									// 패밀리 창을 토글한다.

			{ MSK_ALT,		KEY_I,	FALSE, FALSE },									// 인벤토리를 토글한다.

			{ MSK_ALT,		KEY_H,	FALSE, FALSE },									// 도움말을 토글한다.

			{ MSK_ALT,		KEY_M,	FALSE, FALSE },									// 미니맵을 토글한다.

			{ MSK_ALT,		KEY_W,	FALSE, FALSE },									// 빅맵을 토글한다.

			{ MSK_ALT,		KEY_O,	FALSE, FALSE },									// 옵션 창을 토글한다.

			{ MSK_ALT,		KEY_G,	FALSE, FALSE },									// 길드 창을 토글한다.

			{ MSK_ALT,		KEY_Q,	FALSE, FALSE },									// 퀘스트 창을 토글한다.

			{ MSK_ALT,		KEY_C,	FALSE, FALSE },									// 캐릭터 정보창을 토글한다.

			{ MSK_ALT,		KEY_R,	FALSE, FALSE },									// 걷기/뛰기 모드를 전환한다.

			{ MSK_ALT,		KEY_INSERT,	FALSE, FALSE },								// 카메라 뷰 모드를 전환한다.

			{ MSK_ALT,		KEY_D,	FALSE, FALSE },									// 매칭 창을 토글한다.
			{ MSK_ALT,		KEY_B,	FALSE, FALSE },									// 친구 창을 토글한다.
			{ MSK_ALT,		KEY_N,	FALSE, FALSE },									// 쪽지 창을 토글한다.

			// 071025 LYW --- MacroManager : Add toggle key for help dialog.
			{ MSK_ALT,		KEY_T,	FALSE, FALSE },									// 도움말 창을 토글한다.
			{ MSK_CTRL,		KEY_CONTROL,	FALSE, FALSE },							// 낚시 낚기버튼

			// 080429 LYW --- MacroManager : Add toggle key for chatroom system.
			{ MSK_NONE,		KEY_HOME, FALSE, FALSE },								// 채팅방 리스트창을 토글한다.
			{ MSK_CTRL,		KEY_HOME, FALSE, FALSE },								// 채팅방을 토글한다.
			{ MSK_CTRL,		KEY_END,  TRUE, FALSE },								// 채팅방과 채팅창 사이의 포커스를 이동시킨다.

			{ MSK_ALT,		KEY_P,	FALSE, FALSE },									// 펫정보창
		},
		{			
			{ MSK_NONE,		KEY_1,	FALSE, FALSE },									// 메인 퀵슬롯 1번키 사용
			{ MSK_NONE,		KEY_2,	FALSE, FALSE },									// 메인 퀵슬롯 2번키 사용
			{ MSK_NONE,		KEY_3,	FALSE, FALSE },									// 메인 퀵슬롯 3번키 사용
			{ MSK_NONE,		KEY_4,	FALSE, FALSE },									// 메인 퀵슬롯 4번키 사용
			{ MSK_NONE,		KEY_5,	FALSE, FALSE },									// 메인 퀵슬롯 5번키 사용
			{ MSK_NONE,		KEY_6,	FALSE, FALSE },									// 메인 퀵슬롯 6번키 사용
			{ MSK_NONE,		KEY_7,	FALSE, FALSE },									// 메인 퀵슬롯 7번키 사용
			{ MSK_NONE,		KEY_8,	FALSE, FALSE },									// 메인 퀵슬롯 8번키 사용
			{ MSK_NONE,		KEY_9,	FALSE, FALSE },									// 메인 퀵슬롯 9번키 사용
			{ MSK_NONE,		KEY_0, FALSE, FALSE },									// 메인 퀵슬롯10번키 사용

			{ MSK_CTRL,		KEY_1,	FALSE, FALSE },									// 서브 퀵슬롯 1번키 사용 
			{ MSK_CTRL,		KEY_2,	FALSE, FALSE },									// 서브 퀵슬롯 2번키 사용 
			{ MSK_CTRL,		KEY_3,	FALSE, FALSE },									// 서브 퀵슬롯 3번키 사용 
			{ MSK_CTRL,		KEY_4,	FALSE, FALSE },									// 서브 퀵슬롯 4번키 사용 
			{ MSK_CTRL,		KEY_5,	FALSE, FALSE },									// 서브 퀵슬롯 5번키 사용 
			{ MSK_CTRL,		KEY_6,	FALSE, FALSE },									// 서브 퀵슬롯 6번키 사용 
			{ MSK_CTRL,		KEY_7,	FALSE, FALSE },									// 서브 퀵슬롯 7번키 사용 
			{ MSK_CTRL,		KEY_8,	FALSE, FALSE },									// 서브 퀵슬롯 8번키 사용 
			{ MSK_CTRL,		KEY_9,	FALSE, FALSE },									// 서브 퀵슬롯 9번키 사용 
			{ MSK_CTRL,		KEY_0, FALSE, FALSE },									// 서브 퀵슬롯10번키 사용 

			{ MSK_ALT,		KEY_1,	FALSE, FALSE },									// 메인 퀵슬롯을 1번으로 세팅함.
			{ MSK_ALT,		KEY_2,	FALSE, FALSE },									// 메인 퀵슬롯을 2번으로 세팅함.
			{ MSK_ALT,		KEY_3,	FALSE, FALSE },									// 메인 퀵슬롯을 3번으로 세팅함.
			{ MSK_ALT,		KEY_4,	FALSE, FALSE },									// 메인 퀵슬롯을 4번으로 세팅함.
			{ MSK_ALT,		KEY_5,	FALSE, FALSE },									// 메인 퀵슬롯을 5번으로 세팅함.
			{ MSK_ALT,		KEY_6,	FALSE, FALSE },									// 메인 퀵슬롯을 6번으로 세팅함.
			{ MSK_ALT,		KEY_7,	FALSE, FALSE },									// 메인 퀵슬롯을 7번으로 세팅함.
			{ MSK_ALT,		KEY_8,	FALSE, FALSE },									// 메인 퀵슬롯을 8번으로 세팅함.

			{ MSK_NONE,		KEY_OEM_3, FALSE, FALSE },								// 메인 퀵슬롯 번호를 증가 시킨다.
			{ MSK_CTRL,		KEY_OEM_3, FALSE, FALSE },								// 메인 퀵슬롯 번호를 감소 시킨다.

			{ MSK_NONE | MSK_SHIFT,	KEY_OEM_3, FALSE, FALSE },						// 서브 퀵슬롯 번호를 증가 시킨다.
			{ MSK_CTRL | MSK_SHIFT,	KEY_OEM_3, FALSE, FALSE },						// 서브 퀵슬롯 번호를 감소 시킨다.

			{ MSK_NONE,		KEY_F1,	FALSE, FALSE },									// 내 자신을 선택한다.

			{ MSK_NONE,		KEY_F2,	FALSE, FALSE },									// 첫 번째 파티 멤버를 선택한다.
			{ MSK_NONE,		KEY_F3,	FALSE, FALSE },									// 두 번째 파티 멤버를 선택한다.
			{ MSK_NONE,		KEY_F4,	FALSE, FALSE },									// 세 번째 파티 멤버를 선택한다.
			{ MSK_NONE,		KEY_F5,	FALSE, FALSE },									// 네 번째 파티 멤버를 선택한다.
			{ MSK_NONE,		KEY_F6,	FALSE, FALSE },									// 다섯 번째 파티 멤버를 선택한다.
			{ MSK_NONE,		KEY_F7,	FALSE, FALSE },									// 여섯 번째 파티 멤버를 선택한다.

			{ MSK_NONE,		KEY_TAB,	FALSE, FALSE },								// 가장 가까이에 있는 타겟을 선택한다.

			{ MSK_NONE,		KEY_F12,	FALSE, FALSE },								// 스크린을 캡쳐한다.

			{ MSK_NONE,		KEY_ESCAPE,	FALSE, FALSE },								// 종료 메시지 창을 띄운다.

			{ MSK_CTRL,		KEY_Z,	FALSE, FALSE },									// 모든 인터페이스를 토글한다.

			{ MSK_NONE,		KEY_S,	FALSE, FALSE },									// 스킬창을 토글한다.

			{ MSK_NONE,		KEY_F,	FALSE, FALSE },									// 패밀리 창을 토글한다.

			{ MSK_NONE,		KEY_I,	FALSE, FALSE },									// 인벤토리를 토글한다.

			{ MSK_NONE,		KEY_H,	FALSE, FALSE },									// 도움말을 토글한다.

			{ MSK_NONE,		KEY_M,	FALSE, FALSE },									// 미니맵을 토글한다.

			{ MSK_NONE,		KEY_W,	FALSE, FALSE },									// 빅맵을 토글한다.

			{ MSK_NONE,		KEY_O,	FALSE, FALSE },									// 옵션 창을 토글한다.

			{ MSK_NONE,		KEY_G,	FALSE, FALSE },									// 길드 창을 토글한다.

			{ MSK_NONE,		KEY_Q,	FALSE, FALSE },									// 퀘스트 창을 토글한다.

			{ MSK_NONE,		KEY_C,	FALSE, FALSE },									// 캐릭터 정보창을 토글한다.

			{ MSK_NONE,		KEY_R,	FALSE, FALSE },									// 걷기/뛰기 모드를 전환한다.

			{ MSK_NONE,		KEY_INSERT,	FALSE, FALSE },								// 카메라 뷰 모드를 전환한다.

			{ MSK_NONE,		KEY_D,	FALSE, FALSE },									// 매칭 창을 토글한다.													
			{ MSK_NONE,		KEY_B,	FALSE, FALSE },									// 친구 창을 토글한다.
			{ MSK_NONE,		KEY_N,	FALSE, FALSE },									// 쪽지 창을 토글한다.

			// 071025 LYW --- MacroManager : Add toggle key for help dialog.
			{ MSK_NONE,		KEY_T,	FALSE, FALSE },									// 도움말 창을 토글한다.

			{ MSK_CTRL,		KEY_CONTROL,	FALSE, FALSE },							// 낚시 낚기버튼
			

			// 080429 LYW --- MacroManager : Add toggle key for chatroom system.
			{ MSK_NONE,		KEY_HOME, FALSE, FALSE },								// 채팅방 리스트창을 토글한다.
			{ MSK_CTRL,		KEY_HOME, FALSE, FALSE },								// 채팅방을 토글한다.
			{ MSK_CTRL,		KEY_END,  TRUE, FALSE },								// 채팅방과 채팅창 사이의 포커스를 이동시킨다.
			
			{ MSK_NONE,		KEY_P,	FALSE, FALSE },									// 펫정보창
		}
	};

	sMACRO DefaultPressMacro[MM_COUNT][MPE_COUNT] = {
		{
			{ MSK_NONE,		KEY_PGUP,	FALSE, FALSE },								//	MPE_CAMERAMOVE_ZOOMIN
			{ MSK_NONE,		KEY_PGDN,	FALSE, FALSE },								//	MPE_CAMERAMOVE_ZOOMOUT

			{ MSK_NONE,		KEY_UP,		FALSE, FALSE },								//	MPE_CAMERAMOVE_UP
			{ MSK_NONE,		KEY_DOWN,	FALSE, FALSE },								//	MPE_CAMERAMOVE_DOWN
			{ MSK_NONE,		KEY_RIGHT,	FALSE, FALSE },								//	MPE_CAMERAMOVE_RIGHT
			{ MSK_NONE,		KEY_LEFT,	FALSE, FALSE },								//	MPE_CAMERAMOVE_LEFT
		},
		{
			{ MSK_NONE,		KEY_PGUP,	FALSE, FALSE },								//	MPE_CAMERAMOVE_ZOOMIN
			{ MSK_NONE,		KEY_PGDN,	FALSE, FALSE },								//	MPE_CAMERAMOVE_ZOOMOUT

			{ MSK_NONE,		KEY_UP,		FALSE, FALSE },								//	MPE_CAMERAMOVE_UP
			{ MSK_NONE,		KEY_DOWN,	FALSE, FALSE },								//	MPE_CAMERAMOVE_DOWN
			{ MSK_NONE,		KEY_RIGHT,	FALSE, FALSE },								//	MPE_CAMERAMOVE_RIGHT
			{ MSK_NONE,		KEY_LEFT,	FALSE, FALSE },								//	MPE_CAMERAMOVE_LEFT			
		}
	};

	memcpy( m_DefaultKey, DefaultMacro, sizeof( DefaultMacro ) );
	memcpy( m_DefaultPressKey, DefaultPressMacro, sizeof( DefaultPressMacro ) );

	LoadUserMacro();
}


void CMacroManager::SetDefaultMacro( int nMacroMode )
{
	memcpy( m_MacroKey[nMacroMode], m_DefaultKey[nMacroMode], sizeof( sMACRO ) * ME_COUNT );

	memcpy( m_MacroPressKey[nMacroMode], m_DefaultPressKey[nMacroMode], sizeof( sMACRO ) * MPE_COUNT );
}


int CMacroManager::IsUsableKey( CKeyboard* keyInfo, WORD* wKey, BOOL bUserMode )
{
	static WORD wUsableSysKey[] =
	{
		KEY_CONTROL,
		KEY_MENU,
		KEY_SHIFT
	};

	static WORD wUsableCombineKey[] =
	{
		KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0,
		KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M,
		KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,
		KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6,
		KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,

		KEY_EQUALS, KEY_SEMICOLON, KEY_OEM_3, KEY_SLASH, KEY_LBRACKET, KEY_RBRACKET, KEY_BACKSLASH,
		KEY_APOSTROPHE, KEY_MINUS, KEY_EQUALS, /*KEY_OEM_102,*/ KEY_COMMA, KEY_PERIOD, 

		KEY_SPACE, KEY_BACK, KEY_INSERT, KEY_DELETE, KEY_HOME, KEY_END, KEY_PGUP, KEY_PGDN,
		KEY_TAB, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,

		KEY_PAD0, KEY_PAD1, KEY_PAD2, KEY_PAD3, KEY_PAD4, KEY_PAD5, KEY_PAD6, KEY_PAD7, KEY_PAD8, KEY_PAD9,
		KEY_MULTIPLY, KEY_ADD, KEY_SUBTRACT, KEY_DECIMAL, KEY_DIVIDE
	};
	
	static WORD wUsableKey[] =
	{
		KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6,
		KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,

		KEY_SPACE, KEY_BACK, KEY_INSERT, KEY_DELETE, KEY_HOME, KEY_END, KEY_PGUP, KEY_PGDN,
		KEY_TAB, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,
	};

	static BOOL bSysKeyDown = FALSE;
	int i;

	if( ( keyInfo->GetKeyDown( KEY_TAB ) || keyInfo->GetKeyDown( KEY_F4 ) )
		  && keyInfo->GetKeyPressed( KEY_MENU ) )
	{
		return 0;
	}


	if( bSysKeyDown || bUserMode == 1 )
	{
		for( i = 0 ; i < sizeof( wUsableCombineKey ) / sizeof( WORD ) ; ++i )
		{
			if( keyInfo->GetKeyDown( wUsableCombineKey[i] ) )
			{
				*wKey = wUsableCombineKey[i];
				return 1;
			}
		}
	}
	else
	{
		for( i = 0 ; i < sizeof( wUsableKey ) / sizeof( WORD ) ; ++i )
		{
			if( keyInfo->GetKeyDown( wUsableKey[i] ) )
			{
				*wKey = wUsableKey[i];
				return 1;
			}
		}
	}

	for( i = 0 ; i < sizeof( wUsableSysKey ) / sizeof( WORD ) ; ++i )
	{
		if( keyInfo->GetKeyPressed( wUsableSysKey[i] ) )
		{
			*wKey = wUsableSysKey[i];
			bSysKeyDown = TRUE;
			return 2;
		}
	}

	*wKey = 0x0000;
	bSysKeyDown = FALSE;
	return 0;
}


//void CMacroManager::KeyboardInput( CKeyboard* keyInfo )
//{
//	if( !g_bActiveApp || m_bPause )	return;											
//	
//	if(HEROID == 0)
//		return;
//
//	if( MAINGAME->GetCurStateNum() != eGAMESTATE_GAMEIN ) return;	//AOAuE­ ¹UA¸·I≫≪´U confirm
//
//	//f12를 누를경우 시계 다이얼로그를 보인다
//	//키클릭후 랜더링이 작동하므로 키다운시 렌더링하고 키업시 스크린샷 저장
//	if (keyInfo->GetKeyPressed(88) == 1) 
//	{
//		// 070124 LYW --- Delete this line.
//		//GAMEIN->GetScreenShotDlg()->SetActive( TRUE, gHeroID );  // 스크린캡쳐 시간dlg 
//		// 070123 LYW --- Call function to capture screenshot.
//		PlayMacro(ME_SCREENCAPTURE) ;
//	}
//
//	BOOL bDock = FALSE;
//	if( CIMEWND->IsDocking() )
//	{
//		if( m_nMacroMode == MM_MACRO )
//			//return;
//			bDock = TRUE;
//	}
//	
//	int nSysKey = MSK_NONE;
//
//	if( keyInfo->GetKeyPressed( KEY_MENU ) )
//	{
//		nSysKey = 0;		//alt
//		nSysKey |= MSK_ALT;
//	}
//	if( keyInfo->GetKeyPressed( KEY_CONTROL ) )
//	{
//		nSysKey &= ~MSK_NONE;
//		nSysKey |= MSK_CTRL;
//	}
//	if( keyInfo->GetKeyPressed( KEY_SHIFT ) )
//	{
//		nSysKey &= ~MSK_NONE;
//		nSysKey |= MSK_SHIFT;
//	}
//
//	
//	for( int i = 0 ; i < ME_COUNT ; ++i )
//	{
//		if( bDock && !m_MacroKey[m_nMacroMode][i].bAllMode )
//			continue;		
//
//		if( m_MacroKey[m_nMacroMode][i].nSysKey & nSysKey )
//		{
//			if( m_MacroKey[m_nMacroMode][i].bUp )
//			{
//				if( keyInfo->GetKeyUp( m_MacroKey[m_nMacroMode][i].wKey ) )
//					PlayMacro( i );
//			}
//			else
//			{
//				if( keyInfo->GetKeyDown( m_MacroKey[m_nMacroMode][i].wKey ) )
//					PlayMacro( i );
//			}
//		}
//	}
//
//
//#ifdef _GMTOOL_
//	if( keyInfo->GetKeyDown(KEY_PADENTER) && keyInfo->GetKeyPressed(KEY_MENU) )
//	{
//		if( GMTOOLMGR->IsShowing() )
//			GMTOOLMGR->ShowGMDialog( FALSE, TRUE );
//		else
//			GMTOOLMGR->ShowGMDialog( TRUE, TRUE );
//	}
//	else if( keyInfo->GetKeyDown(KEY_SUBTRACT) && keyInfo->GetKeyPressed(KEY_MENU) )
//	{
//		MSGBASE msg;
//		msg.Category	= MP_CHEAT;
//		msg.Protocol	= MP_CHEAT_AGENTCHECK_SYN;
//		msg.dwObjectID	= HEROID;
//		NETWORK->Send( &msg, sizeof(msg) );
//	}
//	
//#endif
//
//
//#ifdef _CHEATENABLE_
///*	
////========= 해킹테스트
////	case eGameInInitKind_Login:
////	case eGameInInitKind_MapChange:
//	if( keyInfo->GetKeyDown(KEY_1) && keyInfo->GetKeyPressed(KEY_MENU) && keyInfo->GetKeyPressed(KEY_CONTROL) )
//	{
//		MSG_DWORD msg;
//		msg.Category = MP_USERCONN;
//		msg.Protocol = MP_USERCONN_GAMEIN_SYN;
//		msg.dwData	 = gChannelNum;
//		NETWORK->Send( &msg, sizeof(msg) );
//	}
//
////	case eGameInInitKind_SuryunEnter:
//	if( keyInfo->GetKeyDown(KEY_2) && keyInfo->GetKeyPressed(KEY_MENU) && keyInfo->GetKeyPressed(KEY_CONTROL) )
//	{
//		MSGBASE msg;
//		msg.Category = MP_SURYUN;
//		msg.Protocol = MP_SURYUN_ENTER_SYN;
//		msg.dwObjectID = HEROID;
//		NETWORK->Send( &msg, sizeof(msg) );
//	}
//
////	case eGameInInitKind_SuryunLeave:
//	if( keyInfo->GetKeyDown(KEY_3) && keyInfo->GetKeyPressed(KEY_MENU) && keyInfo->GetKeyPressed(KEY_CONTROL) )
//	{
//		MSGBASE msg;
//		msg.Category = MP_SURYUN;
//		msg.Protocol = MP_SURYUN_LEAVE_SYN;
//		msg.dwObjectID = HEROID;
//		NETWORK->Send( &msg, sizeof(msg) );
//	}
//		
////	case eGameInInitKind_MovePos:
//	if( keyInfo->GetKeyDown(KEY_4) && keyInfo->GetKeyPressed(KEY_MENU) && keyInfo->GetKeyPressed(KEY_CONTROL) )
//	{
//		MSG_DWORD2 msg;
//		msg.Category = MP_USERCONN;
//		msg.Protocol = MP_USERCONN_GAMEINPOS_SYN;
//		msg.dwObjectID = HEROID;
//		msg.dwData1 = gChannelNum;
//		msg.dwData2 = 1;
//
//		NETWORK->Send( &msg, sizeof(msg) );			
//	}
//
////	case eGameInInitKind_EventMapEnter:
//	if( keyInfo->GetKeyDown(KEY_5) && keyInfo->GetKeyPressed(KEY_MENU) && keyInfo->GetKeyPressed(KEY_CONTROL) )
//	{
//		MSG_DWORD3 msg;
//		msg.Category = MP_USERCONN;
//		msg.Protocol = MP_USERCONN_ENTER_EVENTMAP_SYN;
//		msg.dwObjectID = HEROID;
//		msg.dwData1 = EVENTMAP->GetEventMapNum();
//		msg.dwData2 = EVENTMAP->GetChannelNum();
//		msg.dwData3 = EVENTMAP->GetTeamNum();
//		NETWORK->Send( &msg, sizeof(msg) );
//	}
////================
//*/
//
//#ifdef _TESTCLIENT_
//
//	if(keyInfo->GetKeyDown(KEY_1) && keyInfo->GetKeyPressed(KEY_CONTROL))
//	{
//		SKILLMGR->Release();
//		SKILLMGR->Init();
//	}
//	if(keyInfo->GetKeyDown(KEY_2) && keyInfo->GetKeyPressed(KEY_CONTROL))
//	{
//		EFFECTMGR->Release();
//		EFFECTMGR->Init();
//	}
//	if(keyInfo->GetKeyDown(KEY_3) && keyInfo->GetKeyPressed(KEY_CONTROL))
//	{
//		EFFECTMGR->RemoveAllEffect();
//		GAMERESRCMNGR->LoadTestClientInfo();
//		OBJECTMGR->RemoveAllObject();
//		GAMERESRCMNGR->ResetMonsterList();
//		GAMERESRCMNGR->LoadMonsterList();
////		GAMERESRCMNGR->LoadSMonsterList();
//		InitTestHero();
//		InitTestMonster();
//		InitTestCharacters();
//	}
//	if(keyInfo->GetKeyDown(KEY_4) && keyInfo->GetKeyPressed(KEY_CONTROL))
//	{
//		MAP->LoadMapDesc(MAP->GetMapNum());
//		MAP->ApplyMapDesc();
//	}
//	if(keyInfo->GetKeyDown(KEY_ESCAPE))
//	{
//		EFFECTMGR->SetEndFlagAllEffect();
//		OBJECTMGR->SetSelectedObject(NULL);
//	}
//	if(keyInfo->GetKeyDown(KEY_SUBTRACT))
//	{
//		MAINGAME->SetGameState( eGAMESTATE_MURIMNET );
//	}
//
//	static BOOL bWireFrame = FALSE;
//	if(keyInfo->GetKeyDown(KEY_F12) && keyInfo->GetKeyPressed(KEY_MENU))
//	{
//		if( !bWireFrame )
//		{			
//			g_pExecutive->GetRenderer()->SetRenderMode(2);
//			bWireFrame = TRUE;
//		}
//		else
//		{
//			g_pExecutive->GetRenderer()->SetRenderMode(0);
//			bWireFrame = FALSE;
//		}
//	}
//
//	if(keyInfo->GetKeyDown(KEY_R) && keyInfo->GetKeyPressed(KEY_MENU))	
//	{
//		/*
//		HERO->GetEngineObject()->ChangeMotion(eMotion_Item_Teleport_1, FALSE);		
//		OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_ItemUse);
//		OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_ItemUse, HERO->GetEngineObject()->GetAnimationTime(eMotion_Item_Teleport_1)) ;
//		HERO->SetItemUseMotionIdx( eMotion_Item_Teleport_1 );*/
////		EFFECTMGR->StartHeroEffectProcess(FindEffectNum("Mall_eff_heal.beff"));	
////		EFFECTMGR->StartHeroEffectProcess(FindEffectNum("maintain_eff_jackpot.beff"));	
//
////		EFFECTMGR->StartEffectProcess(eEffect_LevelUpSentence,HERO,NULL,0,0);
//		EFFECTMGR->StartEffectProcess(1034,HERO,NULL,0,0);
////		EFFECTMGR->StartHeroEffectProcess(FindEffectNum("m_ba_080.beff"));
////		EFFECTMGR->StartEffectProcess(eEffect_Jackpot,HERO,NULL,0,0);
//
////		OBJECTEFFECTDESC desc(FindEffectNum("maintain_eff_jackpot.beff"));
////		pPlayer->AddObjectEffect( JACKPOT_EFFECTID, &desc, 1 );
//		
//	}
//
//	if(keyInfo->GetKeyDown(KEY_4) && keyInfo->GetKeyPressed(KEY_MENU))	
//	{
//		//OBJECTEFFECTDESC desc(eEffect_ShopItem_Life);
//		//HERO->AddObjectEffect(STATE_EFFECT_ID,&desc,1);
//		EFFECTMGR->StartHeroEffectProcess(FindEffectNum("Mall_eff_heal.beff"));	
//		//EFFECTMGR->StartHeroEffectProcess(eEffect_ShopItem_Life);
//	}
//	if(keyInfo->GetKeyDown(KEY_5) && keyInfo->GetKeyPressed(KEY_MENU))
//	{
//		//EFFECTMGR->StartHeroEffectProcess(eEffect_ShopItem_Life);
//		EFFECTMGR->StartHeroEffectProcess(FindEffectNum("m_ba_070.beff"));
//	}
//	if(keyInfo->GetKeyDown(KEY_7) && keyInfo->GetKeyPressed(KEY_MENU))	
//	{
//		EFFECTMGR->StartHeroEffectProcess(eEffect_ShopItem_Revive);
//	}
//	
//#else
//	
//	if(keyInfo->GetKeyDown(KEY_1) && keyInfo->GetKeyPressed(KEY_MENU))
//	{
//		CMoveDialog* pdlg = GAMEIN->GetMoveDialog();
//		if( pdlg )
//		{
//			pdlg->SetActive(!pdlg->IsActive());
//		}
//	}
//	if(keyInfo->GetKeyDown(KEY_2) && keyInfo->GetKeyPressed(KEY_MENU))
//	{
//		CItemShopDialog* pdlg = GAMEIN->GetItemShopDialog();
//		if( pdlg )
//		{
//			pdlg->SetActive(!pdlg->IsActive());
//		}
//	}
//	if(keyInfo->GetKeyDown(KEY_3) && keyInfo->GetKeyPressed(KEY_MENU))
//	{
//		CPointSaveDialog* pdlg = GAMEIN->GetPointSaveDialog();
//		if( pdlg )
//		{
//			pdlg->SetActive(!pdlg->IsActive());
//		}
//	}
//
//#endif //_TESTCLIENT_
//
//	// jsd test
///*	
//	if( keyInfo->GetKeyDown( KEY_Z ) )
//	{
//		MSG_BATTLE_INFO msg;
//		BATTLE_INFO_BASE info;
//		info.BattleID = 1;
//		info.BattleKind = eBATTLE_KIND_MURIMFIELD;
//		info.BattleState = 0;
//		info.BattleTime = 0;
//		BATTLESYSTEM->CreateBattle( &info );
//	}
//*/
//
//
//
//#ifndef _GMTOOL_
//	if( !CHEATMGR->IsCheatEnable() ) return;
//#endif
//
//	if(keyInfo->GetKeyDown(KEY_G) && keyInfo->GetKeyPressed(KEY_CONTROL))
//	{
//		CPetUpgradeDialog* pdlg = GAMEIN->GetPetUpgradeDialog();
//		if( pdlg )
//			pdlg->SetActive( !pdlg->IsActive() );
//	}
//
//	if(keyInfo->GetKeyDown(KEY_R) && keyInfo->GetKeyPressed(KEY_CONTROL))
//	{
//		CPetRevivalDialog* pdlg = GAMEIN->GetPetRevivalDialog();
//		if( pdlg )
//			pdlg->SetActive( !pdlg->IsActive() );
//	}
//
//	if(keyInfo->GetKeyDown(KEY_D) && keyInfo->GetKeyPressed(KEY_CONTROL))
//	{
//		GAMEIN->GetDissolutionDialog()->SetActive(TRUE);
//	}
//	
//	if(keyInfo->GetKeyDown(KEY_F1) && keyInfo->GetKeyPressed(KEY_CONTROL))
//	{
//		g_bDisplayFPS = !g_bDisplayFPS;
//	}
//	//if(keyInfo->GetKeyDown(KEY_F2) && keyInfo->GetKeyPressed(KEY_CONTROL))
//	//{
//	//	GAMEIN->GetMixDialog()->SetActiveRecursive(GAMEIN->GetUpgradeDialog()->IsActive()^TRUE);
//	//}
//	
//	if(keyInfo->GetKeyDown(KEY_BACK) && keyInfo->GetKeyPressed(KEY_MENU))
//	{
//		WINDOWMGR->ToggleShowInterface();
//	}
//
//	// ¸¶¿i½º Æ÷AIAI A×½ºÆ®
//	if(keyInfo->GetKeyDown(KEY_P) && keyInfo->GetKeyPressed(KEY_MENU))
//	{
//		m_bInVisibleCursor ^= TRUE;
//		CURSOR->SetCheatMode( m_bInVisibleCursor );
///*
//		if(m_bInVisibleCursor)
//		{
//			CURSOR->SetCheatMode( TRUE );
//			CURSOR->SetCursor(eCURSOR_DEFAULTCHEAT);
//			CURSOR->SetActive(TRUE);
//		}
//		else
//		{
//			CURSOR->SetCursor(eCURSOR_DEFAULT);
//			CURSOR->SetActive(TRUE);
//		}	
//*/
//	}
//
//	if( keyInfo->GetKeyDown(KEY_C) && keyInfo->GetKeyPressed(KEY_MENU))
//	{
//		if( CAMERA->GetCameraMode() == eCM_EyeView )
//		{
//			CAMERA->SetCameraMode( eCM_Free );
//
//			if( !WINDOWMGR->IsOpendAllWindows() )
//			{
//				WINDOWMGR->ShowAllActivedWindow() ;
//
//				WINDOWMGR->SetOpendAllWindows( TRUE ) ;
//
//				WINDOWMGR->ShowBaseWindow() ;
//			}
//
//		}
//		else
//		{
//			CAMERA->SetCameraMode( eCM_EyeView );
//
//			if( WINDOWMGR->IsOpendAllWindows() )
//			{
//				WINDOWMGR->m_pActivedWindowList->RemoveAll() ;
//				WINDOWMGR->CloseAllWindow() ;
//
//				WINDOWMGR->SetOpendAllWindows( FALSE ) ;
//			}
//		}
//	}
//	
//	// 070402 LYW --- Delete function ToggleMonsterChat.
//	/*
//	if( keyInfo->GetKeyDown(KEY_X) && keyInfo->GetKeyPressed(KEY_MENU))
//	{
//		CHATMGR->ToggleMonsterChat();
//	}
//	*/
//
//#ifndef _TESTCLIENT_
//	static BOOL bWireFrame = FALSE;
//#endif
///*	if(keyInfo->GetKeyDown(KEY_F12) && keyInfo->GetKeyPressed(KEY_MENU))
//	{
//		if( !bWireFrame )
//		{			
//			g_pExecutive->GetRenderer()->SetRenderMode(2);
//			bWireFrame = TRUE;
//		}
//		else
//		{
//			g_pExecutive->GetRenderer()->SetRenderMode(0);
//			bWireFrame = FALSE;
//		}
//	}*/
//
//	if(keyInfo->GetKeyDown(KEY_A) && keyInfo->GetKeyPressed(KEY_MENU))
//	{
//		CSWTimeDialog* pdlg = GAMEIN->GetSWTimeDlg();
//		if( pdlg )
//		{
//			pdlg->SetActive( !pdlg->IsActive() );
//			pdlg->SetGuildName( "그레이트형님이셔" );
//		}
//	}
//	if(keyInfo->GetKeyDown(KEY_S) && keyInfo->GetKeyPressed(KEY_MENU))
//	{
//		CGTRegistcancelDialog* pdlg = GAMEIN->GetGTRegistcancelDlg();
//		if( pdlg )
//			pdlg->SetActive( !pdlg->IsActive() );
//	}
//	if(keyInfo->GetKeyDown(KEY_L) && keyInfo->GetKeyPressed(KEY_MENU))
//	{
//		CGTStandingDialog* pdlg = GAMEIN->GetGTStandingDlg();
//		if( pdlg )
//		{
//			REGISTEDGUILDINFO gi;
///*			gi.Ranking = eGTStanding_4TH;
//			gi.Position = 4;
//			gi.GuildIdx = 1;
//			strcpy( gi.GuildName, "길다다일" );
//			pdlg->AddGuildInfo( &gi );
//			gi.Ranking = eGTStanding_2ND;
//			gi.Position = 13;			
//			gi.GuildIdx = 2;
//			strcpy( gi.GuildName, "길다다이" );
//			pdlg->AddGuildInfo( &gi );
//			gi.Ranking = eGTStanding_3RD;
//			gi.Position = 22;
//			gi.GuildIdx = 3;
//			strcpy( gi.GuildName, "길다다삼" );
//			pdlg->AddGuildInfo( &gi );
//			gi.Ranking = eGTStanding_1ST;
//			gi.Position = 31;
//			gi.GuildIdx = 4;
//			strcpy( gi.GuildName, "길다다사" );
//			pdlg->AddGuildInfo( &gi );*/
//			gi.Ranking = eGTStanding_8TH;
//			gi.Position = 0;
//			gi.GuildIdx = 2;
//			strcpy( gi.GuildName, "라마사마일" );
//			pdlg->AddGuildInfo( &gi );
//			gi.Ranking = eGTStanding_32TH;
//			gi.Position = 1;
//			gi.GuildIdx = 4;
//			strcpy( gi.GuildName, "라마사마이" );
//			pdlg->AddGuildInfo( &gi );
//			gi.Ranking = eGTStanding_16TH;
//			gi.Position = 2;
//			gi.GuildIdx = 6;
//			strcpy( gi.GuildName, "라마사마삼" );
//			pdlg->AddGuildInfo( &gi );
//			gi.Ranking = eGTStanding_32TH;
//			gi.Position = 3;
//			gi.GuildIdx = 8;
//			strcpy( gi.GuildName, "라마사마사" );
//			pdlg->AddGuildInfo( &gi );
//			gi.Ranking = eGTStanding_32TH;
//			gi.Position = 4;			
//			gi.GuildIdx = 10;
//			strcpy( gi.GuildName, "라마사마오" );
//			pdlg->AddGuildInfo( &gi );
//			gi.Ranking = eGTStanding_4TH;
//			gi.Position = 5;			
//			gi.GuildIdx = 12;
//			strcpy( gi.GuildName, "라마사마육" );
//			pdlg->AddGuildInfo( &gi );
//			gi.Ranking = eGTStanding_16TH;
//			gi.Position = 6;			
//			gi.GuildIdx = 14;
//			strcpy( gi.GuildName, "라마사마칠" );
//			pdlg->AddGuildInfo( &gi );
//			gi.Ranking = eGTStanding_32TH;
//			gi.Position = 7;			
//			gi.GuildIdx = 16;
//			strcpy( gi.GuildName, "라마사마팔" );
//			pdlg->AddGuildInfo( &gi );
//			pdlg->SetActive( !pdlg->IsActive() );
//			pdlg->RefreshGuildInfo();
//		}
//	}
//	if(keyInfo->GetKeyDown(KEY_J) && keyInfo->GetKeyPressed(KEY_MENU))
//	{
//		GTBATTLE_INFO bt1, bt2;
//		bt1.Group = 0;
//		sprintf( bt1.GuildName1, "라마님만세" );
//		sprintf( bt1.GuildName2, "라마님짱" );
//		bt2.Group = 1;
//		sprintf( bt2.GuildName1, "크헤헤" );
//		sprintf( bt2.GuildName2, "아오" );
//
//		CGTBattleListDialog* pdlg = GAMEIN->GetGTBattleListDlg();
//		if( pdlg )
//		{
//			pdlg->SetPlayOff( TRUE );
//			pdlg->AddBattleInfo( &bt1 );
//			pdlg->AddBattleInfo( &bt2 );
//			pdlg->AddBattleInfo( &bt2 );
//			pdlg->AddBattleInfo( &bt2 );
//			pdlg->AddBattleInfo( &bt2 );
//			pdlg->AddBattleInfo( &bt2 );
//			pdlg->AddBattleInfo( &bt2 );
//			pdlg->AddBattleInfo( &bt2 );
//			pdlg->AddBattleInfo( &bt2 );
//			pdlg->AddBattleInfo( &bt2 );
//			pdlg->AddBattleInfo( &bt2 );
//			pdlg->AddBattleInfo( &bt2 );
//			pdlg->AddBattleInfo( &bt2 );
//			pdlg->AddBattleInfo( &bt2 );
//			pdlg->AddBattleInfo( &bt2 );
//			pdlg->AddBattleInfo( &bt2 );
//			pdlg->AddBattleInfo( &bt2 );
//			pdlg->AddBattleInfo( &bt2 );
//			pdlg->AddBattleInfo( &bt2 );
//			pdlg->AddBattleInfo( &bt2 );
//			pdlg->AddBattleInfo( &bt2 );
//			pdlg->AddBattleInfo( &bt2 );
//			pdlg->AddBattleInfo( &bt2 );
//			pdlg->AddBattleInfo( &bt2 );
//			pdlg->AddBattleInfo( &bt2 );
//			pdlg->RefreshBattleList();
//			pdlg->SetActive( !pdlg->IsActive() );
//		}
//	}
//	if(keyInfo->GetKeyDown(KEY_H) && keyInfo->GetKeyPressed(KEY_MENU))
//	{
//		CGTScoreInfoDialog* pdlg = GAMEIN->GetGTScoreInfoDlg();
//		if( pdlg )
//			pdlg->SetActive( !pdlg->IsActive() );
//	}
//	if(keyInfo->GetKeyDown(KEY_G) && keyInfo->GetKeyPressed(KEY_MENU))
//	{
//		if( GAMEIN->GetEventNotifyDialog()->IsActive() == FALSE )
//		{
//			GAMEIN->GetEventNotifyDialog()->SetTitle( "플러스타임" );
//			GAMEIN->GetEventNotifyDialog()->SetContext( "경험치2배" );
//			GAMEIN->GetEventNotifyDialog()->SetTitle( "플러스타임" );
//			GAMEIN->GetEventNotifyDialog()->SetContext( "경험치2배" );
//
//			GAMEIN->GetEventNotifyDialog()->SetActive( TRUE );
//		}
//		else
//			GAMEIN->GetEventNotifyDialog()->SetActive( FALSE );
//	}	
//	if(keyInfo->GetKeyDown(KEY_K) && keyInfo->GetKeyPressed(KEY_MENU))
//	{
//	}
//
//#ifdef _GMTOOL_
//	if( !CHEATMGR->IsCheatEnable() ) return;
//#endif
//
//	if( bDock == TRUE) return;
//
//	if(keyInfo->GetKeyDown(KEY_I) && keyInfo->GetKeyPressed(KEY_MENU))
//	{
//		MSGBASE msg;
//		msg.Category = MP_ITEM;
//		msg.Protocol = MP_ITEM_SHOPITEM_INFO_SYN;
//		msg.dwObjectID = HERO->GetID();
//		NETWORK->Send(&msg, sizeof(msg));
//	}
//	
//	//¹®ÆAA×½ºÆ®
//// 	if(keyInfo->GetKeyDown(KEY_G) && keyInfo->GetKeyPressed(KEY_CONTROL))
//// 	{
//// 		GAMEIN->GetSkPointDlg()->SetActive( !GAMEIN->GetSkPointDlg()->IsActive() );
//// 	}
//	
////////////////////////////////////////////////////////////////////////////
////for test
//#ifdef _TESTCLIENT_
//	//C￥±¹ A×½ºÆ®
//	if(keyInfo->GetKeyDown(KEY_X) && keyInfo->GetKeyPressed(KEY_CONTROL))
//	{
//		GAMEIN->GetWantNpcDialog()->SetActive(TRUE);
//		GAMEIN->GetWantNpcDialog()->ShowWantMode(eWantNpcMode_Npc);
//		
//		SEND_WANTNPCLIST Info;
//		Info.TotalPage = 6;
//		char buf[64];
//		
//		for(int i=0; i<WANTUNIT_PER_PAGE; ++i)
//		{
//			sprintf(buf, "아톰%d", i);
//			SafeStrCpy(Info.WantNpcList[i].WantedName, buf, MAX_NAME_LENGTH+1);
//			Info.WantNpcList[i].Prize = i+100;
//			Info.WantNpcList[i].VolunteerNum = i+50;
//			Info.WantNpcList[i].WantedIDX = i+1;
//		}
//		GAMEIN->GetWantNpcDialog()->SetWantedList(&Info);
//		
//	}
//#endif
////////////////////////////////////////////////////////////////////////////
//	
//	//AEAo A×½ºÆ®
//#ifndef _TESTCLIENT_
//	if(keyInfo->GetKeyDown(KEY_N) && keyInfo->GetKeyPressed(KEY_CONTROL))
//	{
//		if( GAMEIN->GetReinforceResetDlg() )
//		{
//			GAMEIN->GetReinforceResetDlg()->SetActive( !GAMEIN->GetReinforceResetDlg()->IsActive() );
//		}
//	}
//	//Co≫o¹u A×½ºÆ®
//	if(keyInfo->GetKeyDown(KEY_Y) && keyInfo->GetKeyPressed(KEY_CONTROL))
//	{
////		GAMEIN->GetGuildWarehouseDlg()->SetActive(!GAMEIN->GetGuildWarehouseDlg()->IsActive());
//		MSG_DWORD msg;
//		msg.Category = MP_GTOURNAMENT;
//		msg.Protocol = MP_GTOURNAMENT_CHEAT;
//		msg.dwObjectID = HEROID;
//		msg.dwData = 2;
//		NETWORK->Send( &msg, sizeof(msg) );
//	}
//	if(keyInfo->GetKeyDown(KEY_Q) && keyInfo->GetKeyPressed(KEY_CONTROL))
//	{
////		GAMEIN->GetQuestTotalDialog()->SetActive(TRUE);
//		MSGBASE msg;
//		msg.Category = MP_GTOURNAMENT;
//		msg.Protocol = MP_GTOURNAMENT_REGIST_SYN;
//		msg.dwObjectID = HEROID;
//		NETWORK->Send( &msg, sizeof(msg) );
//	}
//	if(keyInfo->GetKeyDown(KEY_A) && keyInfo->GetKeyPressed(KEY_CONTROL))
//	{
//		MSG_DWORD msg;
//		msg.Category = MP_GTOURNAMENT;
//		msg.Protocol = MP_GTOURNAMENT_CHEAT;
//		msg.dwObjectID = HEROID;
//		msg.dwData = 1;
//		NETWORK->Send( &msg, sizeof(msg) );
//	}
//#endif	
//#ifdef _TESTCLIENT_
////050630SW
////	if( keyInfo->GetKeyDown(KEY_6) && keyInfo->GetKeyPressed(KEY_MENU) ) 
////	{
////		CHATTIPMGR->SetActive( TRUE );
////		CHATTIPMGR->SetCurHeroInfo(1);
////	}
////	if( keyInfo->GetKeyDown(KEY_7) && keyInfo->GetKeyPressed(KEY_MENU) ) 
////	{
////		CHATTIPMGR->SetActive( TRUE );
////		CHATTIPMGR->SetCurHeroInfo(25);
////	}
////	if( keyInfo->GetKeyDown(KEY_8) && keyInfo->GetKeyPressed(KEY_MENU) ) 
////	{
////		CHATTIPMGR->SetActive( TRUE );
////		CHATTIPMGR->SetCurHeroInfo(30);
////	}
////-	
//	
//	if(keyInfo->GetKeyDown(KEY_T) && keyInfo->GetKeyPressed(KEY_CONTROL))
//	{
//	//	SKILLMGR->MakeSAT();
//	}
//	if(keyInfo->GetKeyDown(KEY_W))
//	{
//		CActionTarget Target;
//		cSkillInfo* pInfo = SKILLMGR->GetSkillInfo(GAMERESRCMNGR->m_TestClientInfo.SkillIdx);		
//
//		if( !pInfo )
//		{
//			return;
//		}
///*
//		if( pInfo->GetSkillKind() == SKILLKIND_BUFF || pInfo->GetSkillKind() == SKILLKIND_PASSIVE )
//		{
//			return;
//		}
//*/
//		cActiveSkillInfo* pSkillInfo = ( cActiveSkillInfo* )pInfo;
//
//		if(pSkillInfo->GetSkillInfo()->Target == 1)			// ≫o´e
//		{
//			CObject * targetObj = OBJECTMGR->GetSelectedObject();
//			if( targetObj )
//			{
//				Target.InitActionTarget(targetObj,NULL);
//			}/*		// A¸°UAI ¾ø´A °æ¿i ¸¶¿i½º Æ÷AIAI AO´A °÷¿¡ ½ºA³A≫ ¾²´A°A
//			else if(pInfo->IsMultipleTargetSkill() == TRUE)
//			{
//				VECTOR3* pPos = GetPickedPosition(MOUSE->GetMouseX(),MOUSE->GetMouseY());
//				if(pPos == NULL)
//					return;
//				Target.InitActionTarget(pPos,NULL);
//			}*/
//			else
//			{
//				//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(400) );	
//#ifndef _TESTCLIENT_
//				GAMEIN->GetQuestTotalDialog()->SetActive(TRUE);
//#endif
//				return;
//			}
//		}
//		else if(pSkillInfo->GetSkillInfo()->Target == 0)		// AU½A
//		{
//			Target.InitActionTarget(HERO,NULL);		// AU½AA≫ A¸°UA¸·I
//		}
//		
//		SKILLMGR->OnSkillCommand( HERO, &Target, TRUE );
//	}
//	if(keyInfo->GetKeyDown(KEY_E))
//	{
//		CObject * targetObj = OBJECTMGR->GetSelectedObject();
//		if( targetObj )
//		{
//			TARGETSET Target;
//			Target.pTarget = targetObj;
//			memset(&Target.ResultInfo,0,sizeof(RESULTINFO));
//			Target.ResultInfo.bCritical = rand()%2 ? TRUE : FALSE;
//			Target.ResultInfo.RealDamage = rand() % 50;
//			Target.ResultInfo.CounterDamage = 0;
//			//Target.TargetData = rand() % 100;
//			EFFECTMGR->StartEffectProcess(GAMERESRCMNGR->m_TestClientInfo.Effect,HERO,&Target,1,targetObj->GetID(),EFFECT_FLAG_HEROATTACK);
//		}
//	}
//	if(keyInfo->GetKeyDown(KEY_4))
//	{
//		CObject * targetObj = OBJECTMGR->GetSelectedObject();
//		if( targetObj )
//		{
//			VECTOR3 pos;
//			targetObj->GetPosition(&pos);
//			pos.z -= 3000;
//			MOVEMGR->SetMonsterMoveType(targetObj,eMA_WALKAROUND);
//			MOVEMGR->StartMoveEx(targetObj,gCurTime,&pos);
//		}
//	}
//	if(keyInfo->GetKeyDown(KEY_5))
//	{
//		CObject * targetObj = OBJECTMGR->GetSelectedObject();
//		if( targetObj )
//		{
//			VECTOR3 pos;
//			targetObj->GetPosition(&pos);
//			pos.z -= 3000;
//			MOVEMGR->SetMonsterMoveType(targetObj,eMA_PERSUIT);
//			MOVEMGR->StartMoveEx(targetObj,gCurTime,&pos);
//		}
//	}
//	if(keyInfo->GetKeyDown(KEY_6))
//	{
//		CObject * targetObj = OBJECTMGR->GetSelectedObject();
//		if( targetObj )
//		{
//			VECTOR3 pos;
//			targetObj->GetPosition(&pos);
//			pos.z -= 3000;
//			MOVEMGR->SetMonsterMoveType(targetObj,eMA_RUNAWAY);
//			MOVEMGR->StartMoveEx(targetObj,gCurTime,&pos);
//		}
//	}
//#endif
//
//
//
//#endif	//_CHEATENABLE_
//
//}

//---press´A CN¹ø¸¸ A¼AⓒCØ¾ßCI¹C·I μu·I ≫≪´U.
//void CMacroManager::KeyboardPressInput( CKeyboard* keyInfo )
//{
//	if( !g_bActiveApp || m_bPause )	return;
//
//	if( MAINGAME->GetCurStateNum() != eGAMESTATE_GAMEIN ) return;
//	if( CIMEWND->IsDocking() )
//	{
////		if( m_nMacroMode == MM_MACRO )	//A¤ÆA¿i¼±AI¶oμμ °E¸°´U..CoAc´A..--;
//			return;
//	}
//
//	int nSysKey = MSK_NONE;
//
//	if( keyInfo->GetKeyPressed( KEY_CONTROL ) )		nSysKey |= MSK_CTRL;
//	if( keyInfo->GetKeyPressed( KEY_MENU ) )		nSysKey |= MSK_ALT;
//	if( keyInfo->GetKeyPressed( KEY_SHIFT ) )		nSysKey |= MSK_SHIFT;
//
//
//	for( int i = 0 ; i < MPE_COUNT ; ++i )
//	{
//		if( m_MacroPressKey[m_nMacroMode][i].nSysKey & nSysKey
//			&& keyInfo->GetKeyPressed( m_MacroPressKey[m_nMacroMode][i].wKey ) )
//		{
//			PlayMacroPress( i );
//		}
//	}		
//}
//
//void CMacroManager::GetMacro( int nMacroMode, sMACRO* pMacro )
//{
//	memcpy( pMacro, m_MacroKey[nMacroMode], sizeof(sMACRO) * ME_COUNT );
//}
//
////void CMacroManager::GetPressMacro( int nMacroMode, sMACRO* pMacro )
//
//void CMacroManager::SetMacro( int nMacroMode, sMACRO* pMacro )
//{
//	memcpy( m_MacroKey[nMacroMode], pMacro, sizeof(sMACRO) * ME_COUNT );
//}
//
////void CMacroManager::SetPressMacro( int nMacroMode, sMACRO* pMacro )
//
//void CMacroManager::GetDefaultMacro( int nMacroMode, sMACRO* pMacro )
//{
//	memcpy( pMacro, m_DefaultKey[nMacroMode], sizeof(sMACRO) * ME_COUNT );
//}
//
////void CMacroManager::GetDefaultPressMacro( int nMacroMode, sMACRO* pMacro )
//
//void CMacroManager::SetMacroMode( int nMacroMode )
//{
//	m_nMacroMode = nMacroMode;
//
//	if( m_nMacroMode == MM_CHAT )
//		m_bChatMode = TRUE;
//	else
//		m_bChatMode = FALSE;
//}
//
//void CMacroManager::LoadUserMacro()
//{
////	if( !LoadMacro( "./INI/MacroUserSet.mcr" ) )	//ÆAAI¸iA≫ ¹U²U±i?
////	{
////		SetDefaultMacro( MM_MACRO );
////		SetMacroMode( MM_MACRO );
////		SaveUserMacro();
////	}
//
////AO½AAUμa/////////////////////
//	SetDefaultMacro( MM_MACRO );
//	SetDefaultMacro( MM_CHAT );
//	SetMacroMode( MM_MACRO );
////	SaveUserMacro();
//////////////////////////////////
//}
//
//
//BOOL CMacroManager::LoadMacro( LPCTSTR strPath )
//{
//	HANDLE	hFile;
//
//	hFile = CreateFile( strPath, GENERIC_READ, 0, NULL, 
//						OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
//
//	if( hFile == INVALID_HANDLE_VALUE )
//		return FALSE;
//
//	DWORD dwRead;
//	
////	if( !ReadFile( hFile, &m_nMacroMode, sizeof(int), &dwRead, NULL ) )
////	{
////		CloseHandle( hFile );
////		return FALSE;
////	}
//
////	if( !ReadFile( hFile, &m_nUserSetMode, sizeof(int), &dwRead, NULL ) )
////	{
////		CloseHandle( hFile );
////		return FALSE;
////	}
//
//	if( !ReadFile( hFile, m_MacroKey,
//		sizeof( m_MacroKey ), &dwRead, NULL ) )
//	{
//		CloseHandle( hFile );
//		return FALSE;
//	}
//
//	if( !ReadFile( hFile, m_MacroPressKey,
//		sizeof( m_MacroPressKey ), &dwRead, NULL ) )
//	{
//		CloseHandle( hFile );
//		return FALSE;
//	}
//	
//	CloseHandle( hFile );
//
////	if( m_nMacroMode == MM_CHAT )
////		m_bChatMode = TRUE;
////	else
////		m_bChatMode = FALSE;
//
//	return TRUE;
//}
//
//void CMacroManager::SaveUserMacro()
//{
//	SaveMacro( "./INI/MacroUserSet.mcr" );
//}
//
//BOOL CMacroManager::SaveMacro( LPCTSTR strPath )
//{
//	HANDLE	hFile;
//
//	hFile=CreateFile( strPath, GENERIC_WRITE, 0, NULL, 
//						CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
//
//	if( hFile == INVALID_HANDLE_VALUE )
//		return FALSE;
//	
//	DWORD dwWritten;
//	
////	if( !WriteFile( hFile, &m_nMacroMode, sizeof(int), &dwWritten, NULL ) )
////	{
////		CloseHandle( hFile );
////		return FALSE;
////	}
//
////	if( !WriteFile( hFile, &m_nUserSetMode, sizeof(int), &dwWritten, NULL ) )
////	{
////		CloseHandle( hFile );
////		return FALSE;
////	}
//	
//	if( !WriteFile( hFile, m_MacroKey,
//		sizeof( m_MacroKey ), &dwWritten, NULL ) )
//	{
//		CloseHandle( hFile );
//		return FALSE;
//	}
//
//	if( !WriteFile( hFile, m_MacroPressKey,
//		sizeof( m_MacroPressKey ), &dwWritten, NULL ) )
//	{
//		CloseHandle( hFile );
//		return FALSE;
//	}
//
//	CloseHandle( hFile );
//	return TRUE;	
//}
//
//
////KES ¾E¸²
////¿¡μðÆ®¸| ≫c¿eCI´A ´UAI¾o·I±×´A ¹Yμa½A SetActiveRecursive·I ´UAI¾o·I±×¸| ´Y¾ÆAO¼¼¿a.
//void CMacroManager::PlayMacro( int nMacroEvent )
//{
//	int nMacroMode = OPTIONMGR->GetGameOption()->nMacroMode ;									// 채팅 우선모드인지, 단축키 우선모드인지 체크한다.
//
//	if( nMacroMode == 0 ) return ;
//
//	if( CAMERA->GetCameraMode() == eCM_EyeView )
//	{
//		return;
//	}
//
//	switch( nMacroEvent )
//	{
//	// desc_hseos_소스분석용테스트_01
//	// S 소스분석용테스트 추가 added by hseos 2007.04.09
//	// 게임종료 확인 창
//	// E 소스분석용테스트 추가 added by hseos 2007.04.09
//	case ME_TOGGLE_EXITDLG :
//		{
//			if( !WINDOWMGR->CloseAllAutoCloseWindows() )
//			{
//				// 070507 LYW --- MacroManager : Modified active message box.
//				//WINDOWMGR->MsgBox( MBI_EXIT, MBT_YESNO, RESRCMGR->GetMsg( 263 ) );
//
//				cDialog* pWindow = WINDOWMGR->GetWindowForID(MBI_EXIT) ;
//
//				if(pWindow)
//				{
//					if( pWindow->IsActive() )
//					{
//						pWindow->SetActive(FALSE) ;
//					}
//					else
//					{
//						pWindow->SetActive(TRUE) ;
//					}
//				}
//				else
//				{
//					WINDOWMGR->MsgBox( MBI_EXIT, MBT_YESNO, RESRCMGR->GetMsg( 263 ) );
//				}
//			}			
//		}
//		break ;
//
//	case ME_SELECT_PARTYMEMBER1 :
//		{
//		}
//		break ;
//
//	case ME_SELECT_PARTYMEMBER2 : 
//		{
//		}
//		break ;
//
//	case ME_SELECT_PARTYMEMBER3 : 
//		{
//		}
//		break ;
//
//	case ME_SELECT_PARTYMEMBER4 : 
//		{
//		}
//		break ;
//
//	case ME_SELECT_PARTYMEMBER5 : 
//		{
//		}
//		break ;
//
//	case ME_SELECT_PARTYMEMBER6 : 
//		{
//		}
//		break ;
//
//	case ME_SCREENCAPTURE :
//		{
//			DIRECTORYMGR->SetLoadMode(eLM_Root);
//
//			CreateDirectory( "ScreenShot", NULL );
//
//			
//			WORD wSrv = 0, wY = 0, wM = 0, wD = 0, wN = 0;
//			WORD wHH = 0, wMM = 0, wSS = 0;		
//			char fname[256];
//
//			SYSTEMTIME ti;
//			GetLocalTime( &ti );
//
//			HANDLE hFile = CreateFile( "screenshot/ScreenShot.cnt", GENERIC_READ, 0, NULL, 
//						OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
//
//			DWORD dwSize = 0;
//
//			if( hFile != INVALID_HANDLE_VALUE )
//			{
//				ReadFile( hFile, &wSrv, sizeof(wSrv), &dwSize, NULL );
//				ReadFile( hFile, &wY, sizeof(wY), &dwSize, NULL );
//				ReadFile( hFile, &wM, sizeof(wM), &dwSize, NULL );
//				ReadFile( hFile, &wD, sizeof(wD), &dwSize, NULL );
//				ReadFile( hFile, &wHH, sizeof(wHH), &dwSize, NULL );
//				ReadFile( hFile, &wMM, sizeof(wMM), &dwSize, NULL );
//				ReadFile( hFile, &wSS, sizeof(wSS), &dwSize, NULL );
//				ReadFile( hFile, &wN, sizeof(wN), &dwSize, NULL );
//				
//				CloseHandle( hFile );
//
//				if( !( wSrv == (WORD)g_nServerSetNum && ti.wYear == wY && ti.wMonth == wM && ti.wDay == wD 
//					&& ti.wHour == wHH && ti.wMinute == wMM && ti.wSecond == wSS) )
//				{
//					wSrv = g_nServerSetNum; wY = ti.wYear; wM = ti.wMonth; wD = ti.wDay; wN = 1; 
//					wHH = ti.wHour; wMM = ti.wMinute; wSS = ti.wSecond;
//				}					
//			}
//			else
//			{
//				wSrv = g_nServerSetNum; wY = ti.wYear; wM = ti.wMonth; wD = ti.wDay; wN = 1; 
//				wHH = ti.wHour; wMM = ti.wMinute; wSS = ti.wSecond;
//			}
//
//
//			sprintf(fname,"ScreenShot/Luna_%02d_%02d%02d%02d_%02d%02d%02d_%03d.jpg", wSrv, wY%100, wM, wD, wHH, wMM, wSS, wN );
//			BOOL rt = g_pExecutive->GetRenderer()->CaptureScreen(fname);
//
//			if( rt )
//			{
//				hFile = CreateFile( "screenshot/ScreenShot.cnt", GENERIC_WRITE, 0, NULL, 
//								CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
//
//				if( hFile != INVALID_HANDLE_VALUE )
//				{
//					++wN;
//					WriteFile( hFile, &wSrv, sizeof(wSrv), &dwSize, NULL );
//					WriteFile( hFile, &wY, sizeof(wY), &dwSize, NULL );
//					WriteFile( hFile, &wM, sizeof(wM), &dwSize, NULL );
//					WriteFile( hFile, &wD, sizeof(wD), &dwSize, NULL );
//					WriteFile( hFile, &wHH, sizeof(wHH), &dwSize, NULL );
//					WriteFile( hFile, &wMM, sizeof(wMM), &dwSize, NULL );
//					WriteFile( hFile, &wSS, sizeof(wSS), &dwSize, NULL );
//					WriteFile( hFile, &wN, sizeof(wN), &dwSize, NULL );							
//
//					CloseHandle( hFile );						
//				}
//
//				// 070129 LYW --- ScreenShot : Modified message number.
//				//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(201), fname );
//				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(387), fname );
//			}
//			else
//			{
//				// 070129 LYW --- ScreenShot : Modified message number.
//				//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(200) );
//				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(386) );
//			}
//
//			FIBITMAP* pBitmap = FreeImage_Load(FIF_TARGA, fname, TARGA_DEFAULT);
//			
//			if(pBitmap)
//			{
//				DeleteFile(fname);
//				int len = strlen(fname);
//				fname[len-3] = 'j';
//				fname[len-2] = 'p';
//				fname[len-1] = 'g';
//				FreeImage_SaveJPEG(pBitmap,fname,JPEG_QUALITYSUPERB);
//				FreeImage_Unload(pBitmap);
//			}
//
//			// 070124 LYW --- Delete this line.
//			//GAMEIN->GetScreenShotDlg()->SetActive( FALSE, 0 ); 
//		}
//		break ;
//
//	case ME_USE_QUICKITEM_1_01:
//			{
//				QUICKMGR->UseQuickItem( GAMEIN->GetQuickDlg()->GetSlotDlg( 0 )->GetCurSheet(), 0 );
//			}
//			break;
//
//	case ME_USE_QUICKITEM_1_02:
//			{
//				QUICKMGR->UseQuickItem( GAMEIN->GetQuickDlg()->GetSlotDlg( 0 )->GetCurSheet(), 1 );
//			}
//			break;
//
//	case ME_USE_QUICKITEM_1_03:
//			{
//				QUICKMGR->UseQuickItem( GAMEIN->GetQuickDlg()->GetSlotDlg( 0 )->GetCurSheet(), 2 );
//			}
//			break;
//
//	case ME_USE_QUICKITEM_1_04:
//			{
//				QUICKMGR->UseQuickItem( GAMEIN->GetQuickDlg()->GetSlotDlg( 0 )->GetCurSheet(), 3 );
//			}
//			break;
//
//	case ME_USE_QUICKITEM_1_05:
//			{
//				QUICKMGR->UseQuickItem( GAMEIN->GetQuickDlg()->GetSlotDlg( 0 )->GetCurSheet(), 4 );
//			}
//			break;
//
//	case ME_USE_QUICKITEM_1_06:
//			{
//				QUICKMGR->UseQuickItem( GAMEIN->GetQuickDlg()->GetSlotDlg( 0 )->GetCurSheet(), 5 );
//			}
//			break;
//
//	case ME_USE_QUICKITEM_1_07:
//			{
//				QUICKMGR->UseQuickItem( GAMEIN->GetQuickDlg()->GetSlotDlg( 0 )->GetCurSheet(), 6 );
//			}
//			break;
//
//	case ME_USE_QUICKITEM_1_08:
//			{
//				QUICKMGR->UseQuickItem( GAMEIN->GetQuickDlg()->GetSlotDlg( 0 )->GetCurSheet(), 7 );
//			}
//			break;
//
//	case ME_USE_QUICKITEM_1_09:
//			{
//				QUICKMGR->UseQuickItem( GAMEIN->GetQuickDlg()->GetSlotDlg( 0 )->GetCurSheet(), 8 );
//			}
//			break;
//
//	case ME_USE_QUICKITEM_1_10:
//			{
//				QUICKMGR->UseQuickItem( GAMEIN->GetQuickDlg()->GetSlotDlg( 0 )->GetCurSheet(), 9 );
//			}
//			break;
//
//	case ME_TOGGLE_ALLINTERFACE :
//		{
//			//WINDOWMGR->CloseAllWindow();
//
//			// 070124 LYW --- Modified this part.
//			/*
//			if(WINDOWMGR->CloseAllWindow() == FALSE)
//			{
//				WINDOWMGR->ShowBaseWindow();
//			}
//			*/
//
//			if( WINDOWMGR->IsOpendAllWindows() )
//			{
//				WINDOWMGR->m_pActivedWindowList->RemoveAll() ;
//				WINDOWMGR->CloseAllWindow() ;
//
//				WINDOWMGR->SetOpendAllWindows( FALSE ) ;
//			}
//			else
//			{
//				WINDOWMGR->ShowAllActivedWindow() ;
//
//				WINDOWMGR->SetOpendAllWindows( TRUE ) ;
//
//				WINDOWMGR->ShowBaseWindow() ;
//			}
//		}
//		break ;
//
//	case ME_TOGGLE_CHATTINGDLG : 
//		{
//			/*
//			if( GAMEIN->GetChattingDlg()->IsActivedOSDLG() )
//			{
//				if( GAMEIN->GetChattingDlg()->IsActive() )
//				{
//					GAMEIN->GetChattingDlg()->SetActive( FALSE ) ;
//					GAMEIN->GetOSChattingDlg()->SetActive( FALSE ) ;
//				}
//				else
//				{
//					GAMEIN->GetChattingDlg()->SetActive( TRUE ) ;
//					GAMEIN->GetOSChattingDlg()->SetActive( TRUE ) ;
//				}
//			}
//			else
//			{
//				if( GAMEIN->GetChattingDlg()->IsActive() )
//				{
//					GAMEIN->GetChattingDlg()->SetActive( FALSE ) ;
//				}
//				else
//				{
//					GAMEIN->GetChattingDlg()->SetActive( TRUE ) ;
//				}
//			}
//			*/
//		}
//		break ;
//
//	case ME_TOGGLE_AUTOATTACK : 
//		{
//			// 070503 LYW --- Delete A key.
//			//g_UserInput.GetHeroMove()->AttackSelectedObject();
//		}
//		break ;
//
//	case ME_TOGGLE_SKILLDLG : 
//		{
//			GAMEIN->GetSkillTreeDlg()->SetActive( !( GAMEIN->GetSkillTreeDlg()->IsActive() ) );
//		}
//		break ;
//
//	/*case ME_TOGGLE_KEY_SETTING_TIP : 
//		{
//		}
//		break ;*/
//
//	case ME_TOGGLE_FAMILYDLG : 
//		{
//			// 070621 LYW --- MacroManager : 패밀리 기능이 알파에 들어가지 않기 때문에 토글키 처리 수정.
//			if(HERO->GetFamilyIdx() != 0)
//			{
//				//GAMEIN->GetFamilyDlg()->SetActive(!(GAMEIN->GetFamilyDlg()->IsActive()));
//				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 780 ));
//			}
//			else
//			{
//				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 369 ));
//			}
//		}
//		break ;
//
//	case ME_SEND_MEMO : 
//		{
//		}
//		break ;
//
//	case ME_TOGGLE_INVENTORYDLG :
//		{
//			GAMEIN->GetInventoryDialog()->SetActive( !( GAMEIN->GetInventoryDialog()->IsActive() ) );
//		}
//		break ;
//
//		// 070514 LYW --- MacroManager ; Modified hkey.
//	/*case ME_TOGGLE_HELP :
//		{
//			
//		}
//		break ;*/
//
//		/*case ME_TOGGLE_HELP_HIDE:
//			{
//				if( CAMERA->GetCameraMode() != eCM_EyeView )
//					GAMEIN->GetKeySettingTipDlg()->SetActive(FALSE);
//			}
//			break;*/
//		case ME_TOGGLE_HELP_SHOW:
//			{
//				//if( CAMERA->GetCameraMode() != eCM_EyeView )
//				//{
//				//	/*GAMEIN->GetKeySettingTipDlg()->SetMode( m_nMacroMode );
//				//	GAMEIN->GetKeySettingTipDlg()->SetActive(TRUE);*/
//
//				//	CKeySettingTipDlg* pDlg = GAMEIN->GetKeySettingTipDlg() ;
//
//				//	if( pDlg )
//				//	{
//				//		pDlg->SetMode( m_nMacroMode ) ;
//
//				//		pDlg->SetActive(!pDlg->IsActive()) ;
//				//	}
//				//}
//
//				CKeySettingTipDlg* pDlg = GAMEIN->GetKeySettingTipDlg() ;
//
//				if( pDlg )
//				{
//					pDlg->SetMode( m_nMacroMode ) ;
//
//					pDlg->SetActive(!pDlg->IsActive()) ;
//				}
//			}
//
//	case ME_TOGGLE_FRIENDLIST :
//		{
//		}
//		break ;
//
//	case ME_TOGGLE_MINIMAP :
//		{
//			GAMEIN->GetMiniMapDialog()->SetActive( !(GAMEIN->GetMiniMapDialog()->IsActive()) ) ;
//		}
//		break ;
//
//	case ME_TOGGLE_BIGMAP :
//		{
//			GAMEIN->GetBigMapDialog()->SetActive( !(GAMEIN->GetBigMapDialog()->IsActive()) ) ;
//		}
//		break ;
//
//	case ME_TOGGLE_OPTIONDLG :
//		{
//			COptionDialog* pDlg = GAMEIN->GetOptionDialog() ;
//
//			if( pDlg )
//			{
//				pDlg->SetActive( !pDlg->IsActive() ) ;
//			}				
//		}
//		break ;
//
//	case ME_TOGGLE_GUILDDLG : 
//		{
//			// 070330 LYW --- MacroManager : Apply macro to guild.
//			if(HERO->GetGuildIdx() != 0)
//			{
//				GAMEIN->GetGuildDlg()->SetActive(!(GAMEIN->GetGuildDlg()->IsActive()));
//			}
//			else
//			{
//				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 578 ));
//			}
//		}
//		break ;
//
//	case ME_TOGGLE_QUESTDLG : 
//		{
//			GAMEIN->GetQuestTotalDialog()->SetActive( !(GAMEIN->GetQuestTotalDialog()->IsActive()) ) ;
//		}
//		break ;
//
//	case ME_SIT_STANCE : 
//		{
//		}
//		break ;
//
//	case ME_TOGGLE_CHARINFODLG : 
//		{
//			GAMEIN->GetCharacterDialog()->SetActive( !(GAMEIN->GetCharacterDialog()->IsActive()) ) ;
//		}
//		break ;
//
//	case ME_TOGGLE_ALLIANCEDLG : 
//		{
//		}
//		break ;
//
//	case ME_WALK_RUN :
//		{
//			MOVEMGR->ToggleHeroMoveMode();
//		}
//		break ;
//
//		// 070124 LYW --- Add macro event for camera view mode.
//	case ME_TOGGLE_CAMERAVIEWMODE :
//		{
//			CAMERA->ToggleCameraViewMode();
//		}
//		break ;
//
//		// 070607 LYW --- Add macro event to open matching dialog.
//	case ME_TOGGLE_MATCHINGDLG :
//		{
//			// check identification.
//			const DATE_MATCHING_INFO& dateInfo = HERO->GetDateMatchingInfo() ;
//
//			if(!dateInfo.bIsValid)
//			{
//				// notice can't open dialog.
//				WINDOWMGR->MsgBox( MBI_IDENTIFICATION_ERROR, MBT_OK, "주민등록증을 발급 받아야 사용하실 수 있습니다.") ;
//			}
//			else
//			{
//				// open dialog.
//				CDateMatchingDlg* pDlg = GAMEIN->GetDateMatchingDlg() ;
//
//				if( pDlg )
//				{
//					if( pDlg->IsActive() )
//					{
//						// check chatting.
//						CDateMatchingChatDlg* pChatDlg = GAMEIN->GetDateMatchingDlg()->GetChatingDlg() ;
//						if( pChatDlg )
//						{
//							if( pChatDlg->IsOnChatMode() )
//							{
//								// notice end chatting.
//								WINDOWMGR->MsgBox( MBI_END_CHATTING_CLOSE, MBT_YESNO, "대화를 종료 하시겠습니까?" );
//							}
//							else
//							{
//								pDlg->SetActive(FALSE) ;
//							}
//						}
//					}
//					else
//					{
//						pDlg->GetPartnerListDlg()->SettingControls() ;
//						//pDlg->GetPartnerListDlg()->UpdateInfo() ;
//						pDlg->SetActive(TRUE) ;
//					}
//				}
//			}
//		}
//		break ;
//	}
//
//	// 070123 LYW --- Modified this part.
//	/*
//	switch( nMacroEvent )
//	{
//#ifndef _TESTCLIENT_
//		case ME_USE_QUICKITEM_1_01:
//			{
//				QUICKMGR->UseQuickItem( GAMEIN->GetQuickDlg()->GetSlotDlg( 0 )->GetCurSheet(), 0 );
//			}
//			break;
//		case ME_USE_QUICKITEM_1_02:
//			{
//				QUICKMGR->UseQuickItem( GAMEIN->GetQuickDlg()->GetSlotDlg( 0 )->GetCurSheet(), 1 );
//			}
//			break;
//		case ME_USE_QUICKITEM_1_03:
//			{
//				QUICKMGR->UseQuickItem( GAMEIN->GetQuickDlg()->GetSlotDlg( 0 )->GetCurSheet(), 2 );
//			}
//			break;
//		case ME_USE_QUICKITEM_1_04:
//			{
//				QUICKMGR->UseQuickItem( GAMEIN->GetQuickDlg()->GetSlotDlg( 0 )->GetCurSheet(), 3 );
//			}
//			break;
//		case ME_USE_QUICKITEM_1_05:
//			{
//				QUICKMGR->UseQuickItem( GAMEIN->GetQuickDlg()->GetSlotDlg( 0 )->GetCurSheet(), 4 );
//			}
//			break;
//		case ME_USE_QUICKITEM_1_06:
//			{
//				QUICKMGR->UseQuickItem( GAMEIN->GetQuickDlg()->GetSlotDlg( 0 )->GetCurSheet(), 5 );
//			}
//			break;
//		case ME_USE_QUICKITEM_1_07:
//			{
//				QUICKMGR->UseQuickItem( GAMEIN->GetQuickDlg()->GetSlotDlg( 0 )->GetCurSheet(), 6 );
//			}
//			break;
//		case ME_USE_QUICKITEM_1_08:
//			{
//				QUICKMGR->UseQuickItem( GAMEIN->GetQuickDlg()->GetSlotDlg( 0 )->GetCurSheet(), 7 );
//			}
//			break;
//
//		case ME_SELECT_QUICKSLOT_1_NEXT:
//			{
//			}
//			break;
//		case ME_SELECT_QUICKSLOT_1_PREV:
//			{
//			}
//			break;
//
//		case ME_USE_QUICKITEM_2_01:
//			{
//				QUICKMGR->UseQuickItem( GAMEIN->GetQuickDlg()->GetSlotDlg( 1 )->GetCurSheet(), 0 );
//			}
//			break;
//		case ME_USE_QUICKITEM_2_02:
//			{
//				QUICKMGR->UseQuickItem( GAMEIN->GetQuickDlg()->GetSlotDlg( 1 )->GetCurSheet(), 1 );
//			}
//			break;
//		case ME_USE_QUICKITEM_2_03:
//			{
//				QUICKMGR->UseQuickItem( GAMEIN->GetQuickDlg()->GetSlotDlg( 1 )->GetCurSheet(), 2 );
//			}
//			break;
//		case ME_USE_QUICKITEM_2_04:
//			{
//				QUICKMGR->UseQuickItem( GAMEIN->GetQuickDlg()->GetSlotDlg( 1 )->GetCurSheet(), 3 );
//			}
//			break;
//		case ME_USE_QUICKITEM_2_05:
//			{
//				QUICKMGR->UseQuickItem( GAMEIN->GetQuickDlg()->GetSlotDlg( 1 )->GetCurSheet(), 4 );
//			}
//			break;
//		case ME_USE_QUICKITEM_2_06:
//			{
//				QUICKMGR->UseQuickItem( GAMEIN->GetQuickDlg()->GetSlotDlg( 1 )->GetCurSheet(), 5 );
//			}
//			break;
//		case ME_USE_QUICKITEM_2_07:
//			{
//				QUICKMGR->UseQuickItem( GAMEIN->GetQuickDlg()->GetSlotDlg( 1 )->GetCurSheet(), 6 );
//			}
//			break;
//		case ME_USE_QUICKITEM_2_08:
//			{
//				QUICKMGR->UseQuickItem( GAMEIN->GetQuickDlg()->GetSlotDlg( 1 )->GetCurSheet(), 7 );
//			}
//			break;
//
//		case ME_SELECT_QUICKSLOT_2_NEXT:
//			{
//			}
//			break;
//		case ME_SELECT_QUICKSLOT_2_PREV:
//			{
//			}
//			break;
//
//			// 061220 LYW --- Delete this code.
//
//		case ME_TOGGLE_INVENTORYDLG:
//			{
//				GAMEIN->GetInventoryDialog()->SetActive( !( GAMEIN->GetInventoryDialog()->IsActive() ) );
//			}
//			break;
//
//		case ME_TOGGLE_CHARACTERDLG:
//			{
//				GAMEIN->GetCharacterDialog()->SetActive( !( GAMEIN->GetCharacterDialog()->IsActive() ) );
//			}
//			break;
//
//		case ME_TOGGLE_SKILLDLG:
//			{
//				GAMEIN->GetSkillTreeDlg()->SetActive( !( GAMEIN->GetSkillTreeDlg()->IsActive() ) );
//			}
//			break;
//
////		case ME_TOGGLE_PARTYDLG:
////			GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_PARTYDLGICON);
////			break;
//
//		case ME_TOGGLE_MUNPADLG:
////			GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_GUILDDLGICON);
//			break;
//
//		case ME_TOGGLE_QUESTDLG:
////			GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_QUESTDLGICON);			
//			break;
//		case ME_TOGGLE_EXITDLG:
//
//			if( !WINDOWMGR->CloseAllAutoCloseWindows() )
//			{
//				// 070122 Modified this line.
//			//WINDOWMGR->MsgBox( MBI_EXIT, MBT_YESNO, CHATMGR->GetChatMsg( 280 ) );
//			WINDOWMGR->MsgBox( MBI_EXIT, MBT_YESNO, RESRCMGR->GetMsg( 263 ) );
//			}			
//
//			break;
//
////		case ME_APPLY_EXCHANGE:
////			{
//				//GAMEIN->GetMainInterfaceDialog()->PushDirectMenu( OPT_EXCHANGEDLGICON );
//
//				//KES SECURITY-CHECK
////				if(ABILITYMGR->CheckAbilityLevel(ABILITYINDEX_EXCHANGE,HERO->GetAbilityGroup(),1) == FALSE)
////				{
////					CHATMGR->AddMsg( CTC_ABILITYUPGRADE_FAILED, CHATMGR->GetChatMsg( 139 ));
////				}
////				else
////				{
////					EXCHANGEMGR->ApplyExchange();
////				}
////			}
////			break;
//
////		case ME_OPEN_STREETSTALL:
////			{
////				GAMEIN->GetMainInterfaceDialog()->PushDirectMenu( OPT_STREETSTALLDLGICON );
////				if(ABILITYMGR->CheckAbilityLevel(ABILITYINDEX_STALL,HERO->GetAbilityGroup(),1) == FALSE)
////				{
////					CHATMGR->AddMsg( CTC_ABILITYUPGRADE_FAILED, CHATMGR->GetChatMsg( 139 ));
////				}
////				else
////				{
////					if( HERO->GetState() == eObjectState_None && !SHOWDOWNMGR->IsShowdown() )
////					{
////						STREETSTALLMGR->ChangeDialogState();
////					}
////				}
////			}
////			break;
//
//			// 061220 LYW --- Delete this code.		
////		case ME_TOGGLE_OPTIONDLG:
////			{
////				GAMEIN->GetMainInterfaceDialog()->PushDirectMenu( OPT_OPTIONDLGICON );
////			}
////			break;
//
////		case ME_CLOSE_ALLWINDOW:
////			{
////				GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_CLOSEALLDLGICON);
////				WINDOWMGR->CloseAllAutoCloseWindows();
////			}
////			break;
//
//			// 061220 LYW --- Delete this code.
////		case ME_TOGGLE_FRIENDLIST:
////			{
////				GAMEIN->GetMainInterfaceDialog()->PushDirectMenu( OPT_FRIENDDLGICON );
////			}
////			break;
////
////		case ME_SEND_MEMO:
////			{
////				GAMEIN->GetMainInterfaceDialog()->PushDirectMenu( OPT_NOTEDLGICON );
////			}
////			break;
////			
////		case ME_APPLY_SHOWDOWN:
////			{
////				GAMEIN->GetMainInterfaceDialog()->PushDirectMenu( OPT_SHOWDOWNICON );
////			}
////			break;
//
////		case ME_CLOSE_ACTIVEDLG:
////			break;
//#endif
//		case ME_TOGGLE_MOVEMODE:
//			MOVEMGR->ToggleHeroMoveMode();
//			break;
//
//		case ME_TOGGLE_PEACEWARMODE:
//			PEACEWARMGR->ToggleHeroPeace_WarMode();
//			break;
//
////		case ME_TOGGLE_UNGIMODE:
////			//TODOABIL
////			if(ABILITYMGR->CheckAbilityLevel(ABILITYINDEX_UNGI,HERO->GetAbilityGroup(),1) == FALSE)
////			{
////				CHATMGR->AddMsg( CTC_ABILITYUPGRADE_FAILED, CHATMGR->GetChatMsg( 139 ));
////				return;
////			}
//			
////			UNGIMGR->ToggleHero_UngiMode();
////			break;
//
//
////		case ME_TOGGLE_KYUNGGONG:
////			{
////				//TODOABIL
////				HERO->SetToggleKyungGongMode();
////				if( HERO->GetKyungGongMode() == TRUE && HERO->GetKyungGongGrade() == 0)		// °æ°øμi±ÞAI 0AI¸e °æ°ø ¸ø¾¸
////				{
////					HERO->SetToggleKyungGongMode();
////					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(143) );
////				}
////				
////				HERO->ToggleKyungGongMode();
////			}
////			break;
////			
//		case ME_TOGGLE_AUTOATTACK:
//		case ME_TOGGLE_AUTOATTACK2:
//			//KES 031007
//			g_UserInput.GetHeroMove()->AttackSelectedObject();
//			break;
//#ifndef _TESTCLIENT_
//		case ME_TOGGLE_HELP:
//
//			// 061220 LYW --- Delete this code.
//			//GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_HELPDLGICON);
////			if( !GAMEIN->GetHelpDialog()->IsActive() )
////				GAMEIN->GetHelpDialog()->OpenDialog();
////			else
////				GAMEIN->GetHelpDialog()->SetActive( FALSE );
//			break;
//
//		case ME_TOGGLE_MINIMAP:
//			// 061220 LYW --- Delete this code.
//			//GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_MINIMAPDLGICON);
//			break;
//#endif
//		case ME_TOGGLE_CAMERAVIEWMODE:
//			CAMERA->ToggleCameraViewMode();
//			break;
//
////		case ME_CAMERAMODE_BACKVIEW:
////			CAMERA->SetCameraMode(eCM_FollowView);
////			break;
//
////		case ME_CAMERAMODE_QUARTERVIEW:
////			CAMERA->SetCameraMode(eCM_QuarterView);
////			break;
//
////		case ME_CAMERAMODE_LOADVIEW:
////			break;
////		case ME_CAMERAMODE_SAVEVIEW:
////			break;
////		case ME_CAMERAMOVE_LEFT:
////			break;
////		case ME_CAMERAMOVE_RIGHT:
////			break;
////		case ME_CAMERAMOVE_UP:
////			break;
////		case ME_CAMERAMOVE_DOWN:
////			break;
////		case ME_CAMERAMOVE_ZOOMIN:
////			break;
////		case ME_CAMERAMOVE_ZOOMOUT:
////			break;
//
//		case ME_SCREENCAPTURE:
//			{
//				DIRECTORYMGR->SetLoadMode(eLM_Root);
////				mkdir("ScreenShot");
//
//				CreateDirectory( "ScreenShot", NULL );
//
//				
//				WORD wSrv = 0, wY = 0, wM = 0, wD = 0, wN = 0;
//				WORD wHH = 0, wMM = 0, wSS = 0;		//06.08.10 [PKH] 저장파일에 시간표시
//				char fname[256];
//				//char fnametest[256];
////
//				SYSTEMTIME ti;
//				//GetSystemTime( &ti );
//				GetLocalTime( &ti );
//
//				HANDLE hFile = CreateFile( "screenshot/ScreenShot.cnt", GENERIC_READ, 0, NULL, 
//							OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
//
//				DWORD dwSize = 0;
//
//				if( hFile != INVALID_HANDLE_VALUE )
//				{
//					ReadFile( hFile, &wSrv, sizeof(wSrv), &dwSize, NULL );
//					ReadFile( hFile, &wY, sizeof(wY), &dwSize, NULL );
//					ReadFile( hFile, &wM, sizeof(wM), &dwSize, NULL );
//					ReadFile( hFile, &wD, sizeof(wD), &dwSize, NULL );
//					ReadFile( hFile, &wHH, sizeof(wHH), &dwSize, NULL );
//					ReadFile( hFile, &wMM, sizeof(wMM), &dwSize, NULL );
//					ReadFile( hFile, &wSS, sizeof(wSS), &dwSize, NULL );
//					ReadFile( hFile, &wN, sizeof(wN), &dwSize, NULL );
//					
//					CloseHandle( hFile );
//
//					if( !( wSrv == (WORD)g_nServerSetNum && ti.wYear == wY && ti.wMonth == wM && ti.wDay == wD 
//						&& ti.wHour == wHH && ti.wMinute == wMM && ti.wSecond == wSS) )
//					{
//						wSrv = g_nServerSetNum; wY = ti.wYear; wM = ti.wMonth; wD = ti.wDay; wN = 1; 
//						wHH = ti.wHour; wMM = ti.wMinute; wSS = ti.wSecond;
//					}					
//				}
//				else
//				{
//					wSrv = g_nServerSetNum; wY = ti.wYear; wM = ti.wMonth; wD = ti.wDay; wN = 1; 
//					wHH = ti.wHour; wMM = ti.wMinute; wSS = ti.wSecond;
//				}
//
//
//				sprintf(fname,"ScreenShot/DarkStory_%02d_%02d%02d%02d_%02d%02d%02d_%03d.jpg", wSrv, wY%100, wM, wD, wHH, wMM, wSS, wN );
//				BOOL rt = g_pExecutive->GetRenderer()->CaptureScreen(fname);
//				//jpg·I º?E?CØAO´ø°¡, ¸Þ¸ð¸®·I¶oμμ ³N°UAO¸e AA°U´U.
//
////jpg·I º?E?CIAU( bmpAÐ°i, jpg·I º?E?CI°i, jpgAuAaCI°i, bmpAo¿i°i --;;;;;; )
//
////
//
//
//
//				if( rt )
//				{
//					hFile = CreateFile( "screenshot/ScreenShot.cnt", GENERIC_WRITE, 0, NULL, 
//									CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
//
//					if( hFile != INVALID_HANDLE_VALUE )
//					{
//						++wN;
//						WriteFile( hFile, &wSrv, sizeof(wSrv), &dwSize, NULL );
//						WriteFile( hFile, &wY, sizeof(wY), &dwSize, NULL );
//						WriteFile( hFile, &wM, sizeof(wM), &dwSize, NULL );
//						WriteFile( hFile, &wD, sizeof(wD), &dwSize, NULL );
//						WriteFile( hFile, &wHH, sizeof(wHH), &dwSize, NULL );
//						WriteFile( hFile, &wMM, sizeof(wMM), &dwSize, NULL );
//						WriteFile( hFile, &wSS, sizeof(wSS), &dwSize, NULL );
//						WriteFile( hFile, &wN, sizeof(wN), &dwSize, NULL );							
//
//						CloseHandle( hFile );						
//					}
//
//					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(201), fname );
//				}
//				else
//				{
//					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(200) );
//				}
//
////				FIBITMAP* pBitmap = FreeImage_LoadTARGA(fname);
//				FIBITMAP* pBitmap = FreeImage_Load(FIF_TARGA, fname, TARGA_DEFAULT);
//				
//				if(pBitmap)
//				{
//					DeleteFile(fname);
//					int len = strlen(fname);
//					fname[len-3] = 'j';
//					fname[len-2] = 'p';
//					fname[len-1] = 'g';
//					FreeImage_SaveJPEG(pBitmap,fname,JPEG_QUALITYSUPERB);
//					FreeImage_Unload(pBitmap);
//				}
//
//				//현재시간 다이얼로그 OFF 
//				//F12버튼을 뗄경우 스크린샷을 찍는다
//				//macromanager.cpp 179라인에서 현재시간 다이얼로그 ON
//				GAMEIN->GetScreenShotDlg()->SetActive( FALSE, 0 ); // 스크린캡쳐 시간dlg 
//
//			}
//			break;
//		case ME_SHOWALLNAME_ON:
//			{
//				OBJECTMGR->SetAllNameShow( TRUE );
//			}
//			break;
//		case ME_SHOWALLNAME_OFF:
//			{
//				OBJECTMGR->SetAllNameShow( FALSE );
//			}
//			break;
//#ifndef _TESTCLIENT_
//		case ME_CHANGE_WHISPERLIST_UP:
//			{
//				// 061204 LYW --- Modified Preexistence Chatting Dialog.
//				//if( GAMEIN->GetChatDialog() )
//				//if( GAMEIN->GetChatDialog()->GetChatEditBox() )
//				// 070102 LYW --- Delete this code.
////				
////				if( GAMEIN->GetChattingDlg() )
////				if( GAMEIN->GetChattingDlg()->GetChatEditBox() )
////				{
////					//if( GAMEIN->GetChatDialog()->GetChatEditBox()->IsFocus() )
////					if( GAMEIN->GetChattingDlg()->GetChatEditBox()->IsFocus() )
////					{
////						CHATMGR->ChangeWhisperUser( TRUE );
////					}
////				}
////				
//			}
//			break;
//		case ME_CHANGE_WHISPERLIST_DOWN:
//			{
//				// 061204 LYW --- Modified Preexistence Chatting Dialog.
//				//if( GAMEIN->GetChatDialog() )
//				//if( GAMEIN->GetChatDialog()->GetChatEditBox() )
//				// 070102 LYW --- Delete this code.
////				
////				if( GAMEIN->GetChattingDlg() )
////				if( GAMEIN->GetChattingDlg()->GetChatEditBox() )
////				{
////					//if( GAMEIN->GetChatDialog()->GetChatEditBox()->IsFocus() )
////					if( GAMEIN->GetChattingDlg()->GetChatEditBox()->IsFocus() )
////					{
////						CHATMGR->ChangeWhisperUser( FALSE );
////					}
////				}
////				
////			}
//			break;
//
//		case ME_TOGGLE_BIGMAP:
//			{
//#ifdef _GMTOOL_
//				if( CAMERA->GetCameraMode() != eCM_EyeView )
//#endif
//				if( GAMEIN->GetBigMapDialog() )
//					GAMEIN->GetBigMapDialog()->SetActive( !GAMEIN->GetBigMapDialog()->IsActive() );
//			}
//			break;
//		case ME_MUSSANG_ON:
//			{
//				if(!HERO->IsDied())
//					MUSSANGMGR->SendMsgMussangOn();
//			}
//			break;
//		case ME_TOGGLE_KEY_SETTING_TIP_HIDE:
//			{
//				if( CAMERA->GetCameraMode() != eCM_EyeView )
//					GAMEIN->GetKeySettingTipDlg()->SetActive(FALSE);
//			}
//			break;
//		case ME_TOGGLE_KEY_SETTING_TIP_SHOW:
//			{
//				if( CAMERA->GetCameraMode() != eCM_EyeView )
//				{
//					GAMEIN->GetKeySettingTipDlg()->SetMode( m_nMacroMode );
//					GAMEIN->GetKeySettingTipDlg()->SetActive(TRUE);
//				}
//			}
//			break;
//#endif
//	}
//	*/
//}
//
//void CMacroManager::PlayMacroPress( int nMacroEvent )
//{
//	float Rate = 1.f;
//
//	if( g_UserInput.GetKeyboard()->GetKeyPressed(KEY_RSHIFT) )		//RShift is Pressed
//		Rate = 0.3f;
//
//	switch( nMacroEvent )
//	{
//		case MPE_CAMERAMOVE_ZOOMIN:
//#ifdef _GMTOOL_
//			if( CAMERA->GetCameraMode() == eCM_EyeView )
//				CAMERA->Fly( 0.8 * gTickTime*Rate*5 );
//			else
//#endif
//				CAMERA->ZoomDistance( 0, -0.8 * gTickTime*Rate, 0 );
//
//			break;
//
//		case MPE_CAMERAMOVE_ZOOMOUT:
//#ifdef _GMTOOL_
//			if( CAMERA->GetCameraMode() == eCM_EyeView )
//				CAMERA->Fly( -0.8 * gTickTime*Rate*5 );
//			else
//#endif
//				CAMERA->ZoomDistance( 0, 0.8 * gTickTime*Rate, 0 );
//			break;
//
//		case MPE_CAMERAMOVE_UP:
//			CAMERA->MouseRotate( 0, 0.3 * gTickTime*Rate );
//			break;
//
//		case MPE_CAMERAMOVE_DOWN:
//			CAMERA->MouseRotate( 0, -0.3 * gTickTime*Rate );
//			break;
//
//		case MPE_CAMERAMOVE_RIGHT:
//			CAMERA->MouseRotate( -0.3 * gTickTime*Rate, 0 );
//			break;
//
//		case MPE_CAMERAMOVE_LEFT:
//			CAMERA->MouseRotate( 0.3 * gTickTime*Rate, 0 );
//			break;
//
//	}
//}
//
//void CMacroManager::Init()
//{
//
//	sMACRO DefaultMacro[MM_COUNT][ME_COUNT] = {
//		{
//			// 070123 LYW --- Modified this part.
//			/*
//			{ MSK_NONE,		KEY_ESCAPE,		TRUE , FALSE },			//	ME_TOGGLE_EXITDLG
//
//			{ MSK_NONE,		KEY_F1,	FALSE, FALSE },				//	ME_USE_QUICKITEM_1_01,
//			{ MSK_NONE,		KEY_F2,	FALSE, FALSE },				//	ME_USE_QUICKITEM_1_02,
//			{ MSK_NONE,		KEY_F3,	FALSE, FALSE },				//	ME_USE_QUICKITEM_1_03,
//			{ MSK_NONE,		KEY_F4,	FALSE, FALSE },				//	ME_USE_QUICKITEM_1_04,
//			{ MSK_NONE,		KEY_F5,	FALSE, FALSE },				//	ME_USE_QUICKITEM_1_05,
//			{ MSK_NONE,		KEY_F6,	FALSE, FALSE },				//	ME_USE_QUICKITEM_1_06,
//			{ MSK_NONE,		KEY_F7,	FALSE, FALSE },				//	ME_USE_QUICKITEM_1_07,
//			{ MSK_NONE,		KEY_F8,	FALSE, FALSE },				//	ME_USE_QUICKITEM_1_08,
//																	
//			{ MSK_NONE,		KEY_F9,	FALSE, FALSE },				//	ME_SELECT_QUICKSLOT_1_NEXT,
//			{ MSK_NONE,		KEY_F10,FALSE, FALSE },				//	ME_SELECT_QUICKSLOT_1_PREV,
//																	
//			{ MSK_ALT,		KEY_F1,	FALSE, FALSE },				//	ME_USE_QUICKITEM_2_01,
//			{ MSK_ALT,		KEY_F2,	FALSE, FALSE },				//	ME_USE_QUICKITEM_2_02,
//			{ MSK_ALT,		KEY_F3,	FALSE, FALSE },				//	ME_USE_QUICKITEM_2_03,
//			{ MSK_ALT,		KEY_F4,	FALSE, FALSE },				//	ME_USE_QUICKITEM_2_04,
//			{ MSK_ALT,		KEY_F5,	FALSE, FALSE },				//	ME_USE_QUICKITEM_2_05,
//			{ MSK_ALT,		KEY_F6,	FALSE, FALSE },				//	ME_USE_QUICKITEM_2_06,
//			{ MSK_ALT,		KEY_F7,	FALSE, FALSE },				//	ME_USE_QUICKITEM_2_07,
//			{ MSK_ALT,		KEY_F8,	FALSE, FALSE },				//	ME_USE_QUICKITEM_2_08,
//																	
//			{ MSK_ALT,		KEY_F9,	FALSE, FALSE },				//	ME_SELECT_QUICKSLOT_2_NEXT,
//			{ MSK_ALT,		KEY_F10,FALSE, FALSE },				//	ME_SELECT_QUICKSLOT_2_PREV,
//
//			{ MSK_ALT,		KEY_S,	FALSE, FALSE },				//	ME_TOGGLE_SKILLDLG
//			{ MSK_ALT,		KEY_I,	FALSE, FALSE },				//	ME_TOGGLE_INVENTORYDLG
//			{ MSK_ALT,		KEY_C,	FALSE, FALSE },				//	ME_TOGGLE_CHARACTERDLG
//			{ MSK_ALT,		KEY_G,	FALSE, FALSE },				//	ME_TOGGLE_MUNPADLG
//
//			{ MSK_ALT,		KEY_M,	FALSE, FALSE },				//	ME_TOGGLE_MINIMAP
//
//			{ MSK_ALT,		KEY_Q,	FALSE, FALSE },				//	ME_TOGGLE_QUESTDLG
//
//			{ MSK_ALT,		KEY_O,	FALSE, FALSE },				//	ME_TOGGLE_OPTIONDLG
//			{ MSK_ALT,		KEY_F,	FALSE, FALSE },				//	ME_TOGGLE_FRIENDLIST
//			{ MSK_ALT,		KEY_N,	FALSE, FALSE },				//	ME_SEND_MEMO
//
//			{ MSK_ALT,		KEY_R,	FALSE, FALSE },				//	ME_TOGGLE_MOVEMODE			---AO½A
//
//			{ MSK_ALT,		KEY_Z,			FALSE, FALSE },				//	ME_TOGGLE_PEACEWARMODE
//
//			{ MSK_CTRL,		KEY_CONTROL, TRUE, FALSE },				//	ME_TOGGLE_AUTOATTACK
//			{ MSK_ALT,		KEY_A,	FALSE, FALSE },					//	ME_TOGGLE_AUTOATTACK2
//
//			{ MSK_ALT,		KEY_H,	FALSE, FALSE },				//	ME_TOGGLE_HELP
//
//			{ MSK_NONE,		KEY_INSERT,	FALSE, FALSE },			//	ME_CAMERAMODE_FREEVIEW
//
//			{ MSK_NONE,		KEY_F12,	FALSE, FALSE },				//	ME_SCREENCAPTURE
//
//			{ MSK_SHIFT,	KEY_SHIFT,  FALSE, FALSE },				//ME_SHOWALLNAME_ON
//			{ MSK_ALL,		KEY_SHIFT,  TRUE,	TRUE },				//ME_SHOWALLNAME_OFF
//			{ MSK_NONE,		KEY_UP,		TRUE,	FALSE },			//ME_CHANGE_WHISPERLIST_UP
//			{ MSK_NONE,		KEY_DOWN,	TRUE,	FALSE },			//ME_CHANGE_WHISPERLIST_DOWN
//			{ MSK_ALT,		KEY_W,		FALSE,	FALSE },			//ME_TOGGLE_BIGMAP
//			{ MSK_ALT,		KEY_P,		FALSE,	FALSE },			//ME_MUSSANG_ON
//			{ MSK_ALT,		KEY_D,		FALSE,	FALSE },			//  ME_TOGGLE_KEY_SETTING_TIP_SHOW
//			{ MSK_ALT,		KEY_D,		FALSE,	TRUE },			//	ME_TOGGLE_KEY_SETTING_TIP_HIDE
//			*/
//
//			{ MSK_NONE,		KEY_ESCAPE,		TRUE , FALSE },			//	ME_TOGGLE_EXITDLG
//
//			{ MSK_NONE,		KEY_F1,	FALSE, FALSE },				//	
//			{ MSK_NONE,		KEY_F2,	FALSE, FALSE },				//	
//			{ MSK_NONE,		KEY_F3,	FALSE, FALSE },				//	
//			{ MSK_NONE,		KEY_F4,	FALSE, FALSE },				//	
//			{ MSK_NONE,		KEY_F5,	FALSE, FALSE },				//	
//			{ MSK_NONE,		KEY_F6,	FALSE, FALSE },				//	
//
//			{ MSK_NONE,		KEY_F12, FALSE, FALSE },			//	
//
//			{ MSK_NONE,		KEY_1,	FALSE, FALSE },				//
//			{ MSK_NONE,		KEY_2,	FALSE, FALSE },				//
//			{ MSK_NONE,		KEY_3,	FALSE, FALSE },				//
//			{ MSK_NONE,		KEY_4,	FALSE, FALSE },				//
//			{ MSK_NONE,		KEY_5,	FALSE, FALSE },				//
//			{ MSK_NONE,		KEY_6,	FALSE, FALSE },				//
//			{ MSK_NONE,		KEY_7,	FALSE, FALSE },				//
//			{ MSK_NONE,		KEY_8,	FALSE, FALSE },				//
//			{ MSK_NONE,		KEY_9,	FALSE, FALSE },				//
//			{ MSK_NONE,		KEY_0,	FALSE, FALSE },				//
//
//			{ MSK_CTRL,		KEY_Z,	FALSE, FALSE },				//
//
//			{ MSK_CTRL,		KEY_C,	FALSE, FALSE },				//
//
//			{ MSK_NONE,		KEY_A,	FALSE, FALSE },				//
//
//			{ MSK_NONE,		KEY_S,	FALSE, FALSE },				//
//
//			//{ MSK_NONE,		KEY_D,	FALSE, FALSE },				//
//
//			{ MSK_NONE,		KEY_F,	FALSE, TRUE },				//
//
//			{ MSK_NONE,		KEY_T,	FALSE, TRUE },				//
//
//			{ MSK_NONE,		KEY_I,	FALSE, TRUE },				//
//
//			// 070614 LYW --- MacroManager : Modified key setting.
//			//{ MSK_NONE,		KEY_H,	FALSE, TRUE },				//
//			{ MSK_NONE,		KEY_H,		FALSE,	FALSE },		//  ME_TOGGLE_HELP_SHOW
//			//{ MSK_NONE,		KEY_H,		FALSE,	TRUE },			//	ME_TOGGLE_HELP_HIDE
//
//			{ MSK_NONE,		KEY_N,	FALSE, TRUE },				//
//
//			{ MSK_NONE,		KEY_M,	FALSE, TRUE },				//
//
//			{ MSK_NONE,		KEY_W,	FALSE, TRUE },				//
//
//			{ MSK_NONE,		KEY_O,	FALSE, TRUE },				//
//
//			{ MSK_NONE,		KEY_G,	FALSE, TRUE },				//
//
//			{ MSK_NONE,		KEY_Q,	FALSE, TRUE },				//
//
//			{ MSK_NONE,		KEY_X,	FALSE, TRUE },				//
//
//			{ MSK_NONE,		KEY_C,	FALSE, TRUE },				//
//
//			{ MSK_NONE,		KEY_J,	FALSE, TRUE },				//
//
//			{ MSK_NONE,		KEY_R,	FALSE, TRUE },				//
//
//			// 070124 LYW --- Add macro event for camera view mode.
//			{ MSK_NONE,		KEY_INSERT,	TRUE, FALSE },
//
//			// 070606 LYW --- Add macro event to open matching dialog.
//			//{ MSK_NONE,		KEY_E, FALSE, TRUE },
//			{ MSK_NONE,		KEY_D,	FALSE, FALSE },				//
//
//		},
//
//		//DEFAULT_CHAT
//		//CoAc ≫c¿eAßAI ¸AAⓒ·I A°·I AO½A ¼¼ÆA....
//		{
//			// 070124 LYW --- Modified this part.
//			/*
//			{ MSK_NONE,		KEY_ESCAPE,		TRUE, FALSE },			//	ME_TOGGLE_EXITDLG
//
//			{ MSK_NONE,		KEY_F1,	FALSE, FALSE },				//	ME_USE_QUICKITEM_1_01,
//			{ MSK_NONE,		KEY_F2,	FALSE, FALSE },				//	ME_USE_QUICKITEM_1_02,
//			{ MSK_NONE,		KEY_F3,	FALSE, FALSE },				//	ME_USE_QUICKITEM_1_03,
//			{ MSK_NONE,		KEY_F4,	FALSE, FALSE },				//	ME_USE_QUICKITEM_1_04,
//			{ MSK_NONE,		KEY_F5,	FALSE, FALSE },				//	ME_USE_QUICKITEM_1_05,
//			{ MSK_NONE,		KEY_F6,	FALSE, FALSE },				//	ME_USE_QUICKITEM_1_06,
//			{ MSK_NONE,		KEY_F7,	FALSE, FALSE },				//	ME_USE_QUICKITEM_1_07,
//			{ MSK_NONE,		KEY_F8,	FALSE, FALSE },				//	ME_USE_QUICKITEM_1_08,
//																	
//			{ MSK_NONE,		KEY_F9,	FALSE, FALSE },				//	ME_SELECT_QUICKSLOT_1_NEXT,
//			{ MSK_NONE,		KEY_F10,FALSE, FALSE },				//	ME_SELECT_QUICKSLOT_1_PREV,
//																	
//			{ MSK_ALT,		KEY_F1,	FALSE, FALSE },				//	ME_USE_QUICKITEM_2_01,
//			{ MSK_ALT,		KEY_F2,	FALSE, FALSE },				//	ME_USE_QUICKITEM_2_02,
//			{ MSK_ALT,		KEY_F3,	FALSE, FALSE },				//	ME_USE_QUICKITEM_2_03,
//			{ MSK_ALT,		KEY_F4,	FALSE, FALSE },				//	ME_USE_QUICKITEM_2_04,
//			{ MSK_ALT,		KEY_F5,	FALSE, FALSE },				//	ME_USE_QUICKITEM_2_05,
//			{ MSK_ALT,		KEY_F6,	FALSE, FALSE },				//	ME_USE_QUICKITEM_2_06,
//			{ MSK_ALT,		KEY_F7,	FALSE, FALSE },				//	ME_USE_QUICKITEM_2_07,
//			{ MSK_ALT,		KEY_F8,	FALSE, FALSE },				//	ME_USE_QUICKITEM_2_08,
//																	
//			{ MSK_ALT,		KEY_F9,	FALSE, FALSE },				//	ME_SELECT_QUICKSLOT_2_NEXT,
//			{ MSK_ALT,		KEY_F10,FALSE, FALSE },				//	ME_SELECT_QUICKSLOT_2_PREV,
//
//			{ MSK_NONE,		KEY_S,	FALSE, FALSE },				//	ME_TOGGLE_SKILLDLG
//			{ MSK_NONE,		KEY_I,	FALSE, FALSE },				//	ME_TOGGLE_INVENTORYDLG
//			{ MSK_NONE,		KEY_C,	FALSE, FALSE },				//	ME_TOGGLE_CHARACTERDLG
//			{ MSK_NONE,		KEY_G,	FALSE, FALSE },				//	ME_TOGGLE_MUNPADLG
//
//			{ MSK_NONE,		KEY_M,	FALSE, FALSE },				//	ME_TOGGLE_MINIMAP
//
//			{ MSK_NONE,		KEY_Q,	FALSE, FALSE },				//	ME_TOGGLE_QUESTDLG
//
//			{ MSK_NONE,		KEY_O,	FALSE, FALSE },				//	ME_TOGGLE_OPTIONDLG
//			{ MSK_NONE,		KEY_F,	FALSE, FALSE },				//	ME_TOGGLE_FRIENDLIST
//			{ MSK_NONE,		KEY_N,	FALSE, FALSE },				//	ME_SEND_MEMO
//
//			{ MSK_NONE,		KEY_R,	FALSE, FALSE },				//	ME_TOGGLE_MOVEMODE
//
//			{ MSK_NONE,		KEY_Z,		FALSE, FALSE },				//	ME_TOGGLE_PEACEWARMODE
//
//			{ MSK_CTRL,		KEY_CONTROL, TRUE, FALSE },				//	ME_TOGGLE_AUTOATTACK
//			{ MSK_NONE,		KEY_A,		FALSE, FALSE },				//	ME_TOGGLE_AUTOATTACK2
//
//			{ MSK_NONE,		KEY_H,		FALSE, FALSE },				//	ME_TOGGLE_HELP
//
//			{ MSK_NONE,		KEY_INSERT,	TRUE, FALSE },			//	ME_CAMERAMODE_FREEVIEW/BACKVIRE TOGGLE
//
//			{ MSK_NONE,		KEY_F12,	TRUE, TRUE },				//	ME_SCREENCAPTURE
//
//			{ MSK_SHIFT,	KEY_SHIFT,  FALSE, FALSE },				//ME_SHOWALLNAME_ON
//			{ MSK_ALL,		KEY_SHIFT,  TRUE, TRUE },				//ME_SHOWALLNAME_OFF
//			{ MSK_NONE,		KEY_UP,		TRUE,	FALSE },			//ME_CHANGE_WHISPERLIST_UP
//			{ MSK_NONE,		KEY_DOWN,	TRUE,	FALSE },			//ME_CHANGE_WHISPERLIST_DOWN
//			{ MSK_NONE,		KEY_W,		FALSE,	FALSE },			//ME_TOGGLE_BIGMAP
//			{ MSK_NONE,		KEY_P,		FALSE,	FALSE },			//ME_MUSSANG_ON
//			{ MSK_NONE,		KEY_D,		FALSE,	FALSE },			//  ME_TOGGLE_KEY_SETTING_TIP_SHOW
//			{ MSK_NONE,		KEY_D,		FALSE,	TRUE },			//	ME_TOGGLE_KEY_SETTING_TIP_HIDE
//			*/
//			
//			{ MSK_NONE,		KEY_ESCAPE,		TRUE , FALSE },			//	ME_TOGGLE_EXITDLG
//
//			{ MSK_NONE,		KEY_F1,	FALSE, FALSE },				//	
//			{ MSK_NONE,		KEY_F2,	FALSE, FALSE },				//	
//			{ MSK_NONE,		KEY_F3,	FALSE, FALSE },				//	
//			{ MSK_NONE,		KEY_F4,	FALSE, FALSE },				//	
//			{ MSK_NONE,		KEY_F5,	FALSE, FALSE },				//	
//			{ MSK_NONE,		KEY_F6,	FALSE, FALSE },				//	
//
//			{ MSK_NONE,		KEY_F12, FALSE, FALSE },			//	
//
//			{ MSK_NONE,		KEY_1,	FALSE, FALSE },				//
//			{ MSK_NONE,		KEY_2,	FALSE, FALSE },				//
//			{ MSK_NONE,		KEY_3,	FALSE, FALSE },				//
//			{ MSK_NONE,		KEY_4,	FALSE, FALSE },				//
//			{ MSK_NONE,		KEY_5,	FALSE, FALSE },				//
//			{ MSK_NONE,		KEY_6,	FALSE, FALSE },				//
//			{ MSK_NONE,		KEY_7,	FALSE, FALSE },				//
//			{ MSK_NONE,		KEY_8,	FALSE, FALSE },				//
//			{ MSK_NONE,		KEY_9,	FALSE, FALSE },				//
//			{ MSK_NONE,		KEY_0,	FALSE, FALSE },				//
//
//			{ MSK_CTRL,		KEY_Z,	FALSE, FALSE },				//
//
//			{ MSK_CTRL,		KEY_C,	FALSE, FALSE },				//
//
//			{ MSK_NONE,		KEY_A,	FALSE, FALSE },				//
//
//			{ MSK_NONE,		KEY_S,	FALSE, FALSE },				//
//
//			//{ MSK_NONE,		KEY_D,	FALSE, FALSE },				//
//
//			{ MSK_NONE,		KEY_F,	FALSE, FALSE },				//
//
//			{ MSK_NONE,		KEY_T,	FALSE, FALSE },				//
//
//			{ MSK_NONE,		KEY_I,	FALSE, FALSE },				//
//
//			// 070614 LYW --- MacroManager : Modified key setting.
//			//{ MSK_NONE,		KEY_H,	FALSE, TRUE },				//
//			{ MSK_NONE,		KEY_H,		FALSE,	FALSE },		//  ME_TOGGLE_HELP_SHOW
//			//{ MSK_NONE,		KEY_H,		FALSE,	TRUE },			//	ME_TOGGLE_HELP_HIDE
//
//			{ MSK_NONE,		KEY_N,	FALSE, FALSE },				//
//
//			{ MSK_NONE,		KEY_M,	FALSE, FALSE },				//
//
//			{ MSK_NONE,		KEY_W,	FALSE, FALSE },				//
//
//			{ MSK_NONE,		KEY_O,	FALSE, FALSE },				//
//
//			{ MSK_NONE,		KEY_G,	FALSE, FALSE },				//
//
//			{ MSK_NONE,		KEY_Q,	FALSE, FALSE },				//
//
//			{ MSK_NONE,		KEY_X,	FALSE, FALSE },				//
//
//			{ MSK_NONE,		KEY_C,	FALSE, FALSE },				//
//
//			{ MSK_NONE,		KEY_J,	FALSE, FALSE },				//
//
//			{ MSK_NONE,		KEY_R,	FALSE, FALSE },				//
//
//			// 070124 LYW --- Add macro event for camera view mode.
//			{ MSK_NONE,		KEY_INSERT,	TRUE, FALSE },
//
//			// 070606 LYW --- Add macro event to open matching dialog.
//			//{ MSK_NONE,		KEY_E, FALSE, FALSE }, 
//			{ MSK_NONE,		KEY_D,	FALSE, FALSE },				//
//		}
//	};
//
//	sMACRO DefaultPressMacro[MM_COUNT][MPE_COUNT] = {
//		{
//			{ MSK_NONE,		KEY_PGUP,	FALSE, FALSE },				//	MPE_CAMERAMOVE_ZOOMIN
//			{ MSK_NONE,		KEY_PGDN,	FALSE, FALSE },				//	MPE_CAMERAMOVE_ZOOMOUT
//
//			{ MSK_NONE,		KEY_UP,		FALSE, FALSE },				//	MPE_CAMERAMOVE_UP
//			{ MSK_NONE,		KEY_DOWN,	FALSE, FALSE },				//	MPE_CAMERAMOVE_DOWN
//			{ MSK_NONE,		KEY_RIGHT,	FALSE, FALSE },				//	MPE_CAMERAMOVE_RIGHT
//			{ MSK_NONE,		KEY_LEFT,	FALSE, FALSE },				//	MPE_CAMERAMOVE_LEFT
//		},
//		{
//			{ MSK_NONE,		KEY_PGUP,	FALSE, FALSE },				//	MPE_CAMERAMOVE_ZOOMIN
//			{ MSK_NONE,		KEY_PGDN,	FALSE, FALSE },				//	MPE_CAMERAMOVE_ZOOMOUT
//
//			{ MSK_NONE,		KEY_UP,		FALSE, FALSE },				//	MPE_CAMERAMOVE_UP
//			{ MSK_NONE,		KEY_DOWN,	FALSE, FALSE },				//	MPE_CAMERAMOVE_DOWN
//			{ MSK_NONE,		KEY_RIGHT,	FALSE, FALSE },			//	MPE_CAMERAMOVE_RIGHT
//			{ MSK_NONE,		KEY_LEFT,	FALSE, FALSE },				//	MPE_CAMERAMOVE_LEFT
//		}
//	};
//
//////////////////////////////////////////////////
//
////	memcpy( m_MacroKey[MM_DEFAULT_CHAT], DefaultMacro, sizeof( sMACRO ) * ME_COUNT * 2 );
//
//	memcpy( m_DefaultKey, DefaultMacro, sizeof( DefaultMacro ) );
//	memcpy( m_DefaultPressKey, DefaultPressMacro, sizeof( DefaultPressMacro ) );
//
//	LoadUserMacro();
//}
//
//
//void CMacroManager::SetDefaultMacro( int nMacroMode )
//{
////	m_nMacroMode	= MM_DEFAULT_CHAT;
////	m_bChatMode		= TRUE;
////	m_nMacroMode	= MM_DEFAULT_GENERAL;
////	m_bChatMode		= FALSE;
//
////	m_nMacroMode	= MM_CHAT;
////	m_bChatMode		= TRUE;
//
//	memcpy( m_MacroKey[nMacroMode], m_DefaultKey[nMacroMode], sizeof( sMACRO ) * ME_COUNT );
//
//	memcpy( m_MacroPressKey[nMacroMode], m_DefaultPressKey[nMacroMode], sizeof( sMACRO ) * MPE_COUNT );
//}
//
//
//int CMacroManager::IsUsableKey( CKeyboard* keyInfo, WORD* wKey, BOOL bUserMode )
//{
//	static WORD wUsableSysKey[] =
//	{
//		KEY_CONTROL,
//		KEY_MENU,
//		KEY_SHIFT
//	};
//
//	static WORD wUsableCombineKey[] =
//	{
//		KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0,
//		KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M,
//		KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,
//		KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6,
//		KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,
//
//		KEY_EQUALS, KEY_SEMICOLON, KEY_OEM_3, KEY_SLASH, KEY_LBRACKET, KEY_RBRACKET, KEY_BACKSLASH,
//		KEY_APOSTROPHE, KEY_MINUS, KEY_EQUALS, /*KEY_OEM_102,*/ KEY_COMMA, KEY_PERIOD, 
//
//		KEY_SPACE, KEY_BACK, KEY_INSERT, KEY_DELETE, KEY_HOME, KEY_END, KEY_PGUP, KEY_PGDN,
//		KEY_TAB, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,
//
//		KEY_PAD0, KEY_PAD1, KEY_PAD2, KEY_PAD3, KEY_PAD4, KEY_PAD5, KEY_PAD6, KEY_PAD7, KEY_PAD8, KEY_PAD9,
//		KEY_MULTIPLY, KEY_ADD, KEY_SUBTRACT, KEY_DECIMAL, KEY_DIVIDE
//	};
//	
//	static WORD wUsableKey[] =
//	{
//		KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6,
//		KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,
//
//		KEY_SPACE, KEY_BACK, KEY_INSERT, KEY_DELETE, KEY_HOME, KEY_END, KEY_PGUP, KEY_PGDN,
//		KEY_TAB, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,
//	};
//
//	static BOOL bSysKeyDown = FALSE;
//	int i;
//
//	//½A½ºAUA° ¸OAu E®AI( alt+tab, alt+f4 ) //altA° ¸·±a
//	if( ( keyInfo->GetKeyDown( KEY_TAB ) || keyInfo->GetKeyDown( KEY_F4 ) )
//		  && keyInfo->GetKeyPressed( KEY_MENU ) )
//	{
//		return 0;
//	}
//
//
//	if( bSysKeyDown || bUserMode == 1 )
//	{
//		for( i = 0 ; i < sizeof( wUsableCombineKey ) / sizeof( WORD ) ; ++i )
//		{
//			if( keyInfo->GetKeyDown( wUsableCombineKey[i] ) )
//			{
//				*wKey = wUsableCombineKey[i];
//				return 1;
//			}
//		}
//	}
//	else
//	{
//		for( i = 0 ; i < sizeof( wUsableKey ) / sizeof( WORD ) ; ++i )
//		{
//			if( keyInfo->GetKeyDown( wUsableKey[i] ) )
//			{
//				*wKey = wUsableKey[i];
//				return 1;
//			}
//		}
//	}
//
//	for( i = 0 ; i < sizeof( wUsableSysKey ) / sizeof( WORD ) ; ++i )
//	{
//		if( keyInfo->GetKeyPressed( wUsableSysKey[i] ) )
//		{
//			*wKey = wUsableSysKey[i];
//			bSysKeyDown = TRUE;
//			return 2;
//		}
//	}
//
//	*wKey = 0x0000;
//	bSysKeyDown = FALSE;
//	return 0;
//}


void CMacroManager::CheckToggleState( CObject* player )
{
	if( !	player			||
			player != HERO	||
			WINDOWMGR->IsOpendAllWindows() )
	{
		return;
	}

	// 071204 LUJ	스크린샷 촬영위해 인터페이스가 토글 중인데 이동을 시도할 경우
	//				정지 상태에서만 이용 가능한 인터페이스를 닫아야 한다

	// 거래창
	struct
	{
		void operator() ( cDialog* dialog )
		{
			cPtrList* list = WINDOWMGR->m_pActivedWindowList;

			if( ! dialog ||
				! list )
			{
				return;
			}

			void* key = ( void* )( dialog->GetID() );

			PTRLISTPOS	position = list->Find( key );

			list->RemoveAt( position );
		}
	}
	RemoveSavedWindow;

	// 개인 창고창
	// 길드 창고창
	// 개인 상점창
	// 교환 창
	// NPC 대화창

	RemoveSavedWindow( ( cDialog* )GAMEIN->GetStorageDialog() );
	RemoveSavedWindow( ( cDialog* )GAMEIN->GetGuildWarehouseDlg() );
	RemoveSavedWindow( ( cDialog* )GAMEIN->GetExchangeDialog() );
	RemoveSavedWindow( ( cDialog* )GAMEIN->GetNpcScriptDialog() );
	RemoveSavedWindow( ( cDialog* )GAMEIN->GetStreetBuyStallDialog() );
	RemoveSavedWindow( ( cDialog* )GAMEIN->GetStreetStallDialog() );
	RemoveSavedWindow( ( cDialog* )GAMEIN->GetDealDialog() );
}

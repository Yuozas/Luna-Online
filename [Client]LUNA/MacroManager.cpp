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

// 080117 LYW --- MacroManager : ç���� ������ �˾ƺ��� ���� ȣ��.
#include "../mhMap.h"
// 080117 LYW --- MacroManager : ç���� �ʿ��� ESC Ű�� �������� ȣ��.
#include "../hseos/Date/SHDateManager.h"


//---KES AUTO	//�ӽ�
#include "AutoNoteDlg.h"
#include "AutoAnswerDlg.h"
//-----------

#include "./ChatRoomMgr.h"
//#include "./ChatRoomMainDlg.h"
//#include "./ChatRoomDlg.h"

// 080428 NYJ --- ���ð���
#include "FishingManager.h"
#include "FishingDialog.h"

#include "PetInfoDialog.h"


extern BOOL g_bActiveApp;

#ifdef _TESTCLIENT_																	// �׽�Ʈ Ŭ���̾�Ʈ���,

#include "MHMap.h"																	// �� ����� �ҷ��´�.
#include "ObjectManager.h"															// ������Ʈ �Ŵ��� ����� �ҷ��´�.
#include "EFFECT/EffectManager.h"													// ����Ʈ �Ŵ��� �ش��� �ҷ��´�.
#include "GameResourceManager.h"													// ���� ���ҽ� �Ŵ��� �ش��� �ҷ��´�.
#include "interface/cFont.h"														// ��Ʈ �ش��� �ҷ��´�.

void InitTestHero();																// �׽�Ʈ�� ����θ� �ʱ�ȭ �Ѵ�.
void InitTestMonster();																// �׽�Ʈ�� ���͸� �ʱ�ȭ �Ѵ�.
void InitTestCharacters(void);														// �׽�Ʈ�� ĳ���� ��Ʈ�� �ʱ�ȭ �Ѵ�.

#endif																				// �׽�Ʈ Ŭ���̾�Ʈ ó�� ����.


extern BOOL g_bDisplayFPS;															// �������� ������� ���θ� ���� ����.
extern int g_nServerSetNum;															// ������ ��ȣ�� ���� ����.

GLOBALTON(CMacroManager)

CMacroManager::CMacroManager()														// ��ũ�� �Ŵ��� ������ �Լ�.
{
	ZeroMemory( m_MacroKey, sizeof( m_MacroKey ) );									// ��ũ��Ű ����ü�� ���� �޸� �Ѵ�.
	ZeroMemory( m_DefaultKey, sizeof( m_DefaultKey ) );								// ����ƮŰ ����ü�� ���� �޸� �Ѵ�.

	ZeroMemory( m_MacroPressKey, sizeof( m_MacroPressKey ) );						// ��ũ�� ������Ű�� ���� �޸� �Ѵ�.
	ZeroMemory( m_DefaultPressKey, sizeof( m_DefaultPressKey ) );					// ����Ʈ ������Ű�� ���� �޸� �Ѵ�.

	m_nMacroMode	= MM_CHAT;														// ��ũ�� ��带 ä������ �Ѵ�.
	m_bChatMode		= TRUE;															// ä�ø�带 TRUE�� �����Ѵ�.
	m_bPause		= FALSE;														// ��� FALSE�� �����Ѵ�.
	m_bInVisibleCursor = FALSE;														// Ŀ���� ��¿��θ� FALSE�� �����Ѵ�.

	m_nCurTargetIdx = 0 ;															// �ڵ� Ÿ���� �ε����� 0���� �����Ѵ�.
	m_dwTargetID = 0 ;																// �ڵ� Ÿ���� ���̵� 0���� �����Ѵ�.
}

CMacroManager::~CMacroManager()														// ��ũ�� �Ŵ��� �Ҹ��� �Լ�.
{
}


void CMacroManager::KeyboardInput_Normal( BOOL bDock, CKeyboard* keyInfo )
{
	if( !g_bActiveApp || m_bPause )													// ���α׷��� Ȱ��ȭ ���� �ʾҰų�, ������¸�,
	{
		return;																		// �����Ѵ�.							
	}
	
	if(HEROID == 0)																	// ����� ���̵� 0�� ������,
	{
		return;																		// �����Ѵ�.
	}

	if( MAINGAME->GetCurStateNum() != eGAMESTATE_GAMEIN )							// ���� ������ ���°� ���� �� ���°� �ƴϸ�,
	{
		return;																		// �����Ѵ�.
	}

	//if (keyInfo->GetKeyPressed(88) == 1)											//f12�� ������� �ð� ���̾�α׸� ���δ�.
	//{
	//	PlayMacro(ME_SCREENCAPTURE) ;												//ŰŬ���� �������� �۵��ϹǷ� Ű�ٿ�� �������ϰ� Ű���� ��ũ���� ����.
	//}

	bDock = FALSE;																	// ��ŷ���θ� ���� ������ FALSE�� �����Ѵ�.

	if( CIMEWND->IsDocking() )														// IMEWND�� ��ŷ���̶��,
	{
		if( m_nMacroMode == MM_MACRO )												// ��ũ�� ��尡 ����Ű �켱�����,
		{
			bDock = TRUE;															// ��ŷ ������ TRUE�� �����Ѵ�.
		}
	}
	
	int nSysKey = MSK_NONE;															// �ý���Ű�� MSK_NONE���� �����Ѵ�.

	if( keyInfo->GetKeyPressed( KEY_MENU ) )										// �޴�Ű�� ���ȴٸ�,
	{
		nSysKey = 0;																// �ý���Ű�� 0���� �����ϰ�,
		nSysKey |= MSK_ALT;															// ALTŰ�� �߰��Ѵ�.
	}
	if( keyInfo->GetKeyPressed( KEY_CONTROL ) )										// ��Ʈ�� Ű�� ���ȴٸ�,
	{
		nSysKey &= ~MSK_NONE;														// �ý��� Ű�� MSK_NONE�� ������ �����Ѵ�.
		nSysKey |= MSK_CTRL;														// �ý��� Ű�� CTRLŰ�� �߰��Ѵ�.
	}
	if( keyInfo->GetKeyPressed( KEY_SHIFT ) )										// ����ƮŰ�� ���ȴٸ�,
	{
		nSysKey &= ~MSK_NONE;														// �ý��� Ű�� MSK_NONE�� ������ �����Ѵ�.
		nSysKey |= MSK_SHIFT;														// �ý��� Ű�� SHIFT�� �߰��Ѵ�.
	}

	
	for( int i = 0 ; i < ME_COUNT ; ++i )											// ��ũ�� �� ���� ��ŭ ������ ������.
	{
		if( bDock && !m_MacroKey[m_nMacroMode][i].bAllMode )						// ��ŷ���̰�, ���� ��ũ��Ű�� bAllMode�� �ƴϸ�,
		{
			continue;																// ��Ƽ���Ѵ�.
		}

		if( m_MacroKey[m_nMacroMode][i].nSysKey & nSysKey )							// ���� Ư��Ű��� ��ũ�� Ű�� ��ġ�ϸ�,
		{
			if( m_MacroKey[m_nMacroMode][i].bUp )									// ���� ��ũ��Ű�� bUp�� TRUE�� ������,
			{
				if( keyInfo->GetKeyUp( m_MacroKey[m_nMacroMode][i].wKey ) )			// ��ũ��Ű�� ���Ǿ��ٸ�,
				{
					PlayMacro( i );													// ��ũ�θ� �÷����Ѵ�.
				}
			}
			else																	// ���� ��ũ��Ű�� bUp�� FALSE�� ������,
			{
				if( keyInfo->GetKeyDown( m_MacroKey[m_nMacroMode][i].wKey ) )		// ��ũ�� Ű�� �ٿ�Ǿ��ٸ�,
				{
					PlayMacro( i );													// ��ũ�θ� �÷��� �Ѵ�.
				}
			}
		}
	}
}

void CMacroManager::KeyboardInput_GMTool( BOOL bDock, CKeyboard* keyInfo )
{
#if defined(_GMTOOL_)
	if( (keyInfo->GetKeyDown(KEY_PADENTER) || (keyInfo->GetKeyDown(KEY_RETURN)))  // ����Ű�� ALTŰ�� ���Ȱų�,
		&& keyInfo->GetKeyPressed(KEY_MENU) )										// �����е��� ����Ű�� ALTŰ�� ��������,
	{
		if( GMTOOLMGR->IsShowing() )												// GM���� ���̰� �ִ»��¶��,
		{
			GMTOOLMGR->ShowGMDialog( FALSE, TRUE );									// GM���� �����.
		}
		else																		// GM���� ������ ���¶��,
		{
			GMTOOLMGR->ShowGMDialog( TRUE, TRUE );									// GM���� ������.
		}
	}
	else if( keyInfo->GetKeyDown(KEY_SUBTRACT) && keyInfo->GetKeyPressed(KEY_MENU) )// ALTŰ�� �����е��� -Ű�� ���ȴٸ�,
	{
		MSGBASE msg;																// �⺻ �޽��� ����ü�� �����Ѵ�.

		msg.Category	= MP_CHEAT;													// ī�װ��� ġƮ�� �����Ѵ�.
		msg.Protocol	= MP_CHEAT_AGENTCHECK_SYN;									// ���������� ������Ʈ üũ ��ũ��  �����Ѵ�.
		msg.dwObjectID	= HEROID;													// ������Ʈ ���̵� ����� ���̵� �����Ѵ�.

		NETWORK->Send( &msg, sizeof(msg) );											// �޽����� �����Ѵ�.
	}
#endif
}

void CMacroManager::KeyboardInput_CheatEnable( BOOL bDock, CKeyboard* keyInfo )
{	
#ifdef _TESTCLIENT_	
	KeyboardInput_Cheat_TestClient( bDock, keyInfo ) ;								// �׽�Ʈ Ŭ���̾�Ʈ �� �� ó���� �Ѵ�.
#else
	KeyboardInput_Cheat_NotTestClient( bDock, keyInfo ) ;							// �׽�Ʈ Ŭ���̾�Ʈ�� �ƴ� �� ó���� �Ѵ�.
#endif // _TESTCLIENT_
}

void CMacroManager::KeyboardInput_Cheat_TestClient( BOOL bDock, CKeyboard* keyInfo )
{
#ifdef _TESTCLIENT_	

	if(keyInfo->GetKeyDown(KEY_1) && keyInfo->GetKeyPressed(KEY_CONTROL))			// CTRLŰ�� 1��Ű�� ���ȴٸ�,
	{
		SKILLMGR->Release();														// ��ų�Ŵ����� �����Ѵ�.
		SKILLMGR->Init();															// ��ų �Ŵ����� �ʱ�ȭ �Ѵ�.
	}

	if(keyInfo->GetKeyDown(KEY_2) && keyInfo->GetKeyPressed(KEY_CONTROL))			// CTRLŰ�� 2��Ű�� ���ȴٸ�,
	{
		EFFECTMGR->Release();														// ����Ʈ �Ŵ����� �����Ѵ�.
		EFFECTMGR->Init();															// ����Ʈ �Ŵ����� �ʱ�ȭ �Ѵ�.
	}

	if(keyInfo->GetKeyDown(KEY_3) && keyInfo->GetKeyPressed(KEY_CONTROL))			// CTRLŰ�� 3��Ű�� ���ȴٸ�,
	{
		EFFECTMGR->RemoveAllEffect();												// ��� ����Ʈ�� ���� �Ѵ�.

		GAMERESRCMNGR->LoadTestClientInfo();										// �׽�Ʈ Ŭ���̾�Ʈ ������ �ε��Ѵ�.

		OBJECTMGR->RemoveAllObject();												// ��� ������Ʈ�� �����Ѵ�.

		GAMERESRCMNGR->ResetMonsterList();											// ���� ����Ʈ�� �ٽ� �����Ѵ�.
		GAMERESRCMNGR->LoadMonsterList();											// ���� ����Ʈ�� �ε��Ѵ�.

		InitTestHero();																// �׽�Ʈ ����θ� �ʱ�ȭ �Ѵ�.
		InitTestMonster();															// �׽�Ʈ ���͸� �ʱ�ȭ �Ѵ�.
		InitTestCharacters();														// �׽�Ʈ ĳ���͸� �ʱ�ȭ �Ѵ�.
	}

	if(keyInfo->GetKeyDown(KEY_4) && keyInfo->GetKeyPressed(KEY_CONTROL))			// CTRLŰ�� 4��Ű�� ���ȴٸ�,
	{
		MAP->LoadMapDesc(MAP->GetMapNum());											// �� ������ �ε��Ѵ�.
		MAP->ApplyMapDesc();														// �� ������ �����Ѵ�.
	}
	if(keyInfo->GetKeyDown(KEY_ESCAPE))												// ESCŰ�� ���ȴٸ�,
	{
		EFFECTMGR->SetEndFlagAllEffect();											// ��� ����Ʈ�� ���� �÷��׸� �����Ѵ�.
		OBJECTMGR->SetSelectedObject(NULL);											// ���õ� ������Ʈ�� ���� �Ѵ�.
	}

	if(keyInfo->GetKeyDown(KEY_SUBTRACT))											// �����е��� -Ű�� ���ȴٸ�,
	{
		MAINGAME->SetGameState( eGAMESTATE_MURIMNET );								// ���� ���¸� eGAMESTATE_MURIMNET�� �����Ѵ�.
	}

	static BOOL bWireFrame = FALSE;													// ���̾� ������ ���θ� FALSE�� �����Ѵ�.

	if(keyInfo->GetKeyDown(KEY_F12) && keyInfo->GetKeyPressed(KEY_MENU))			// ALTŰ�� F12�� ���ȴٸ�,
	{
		if( !bWireFrame )															// ���̾� ������ ���ΰ� FALSE���,
		{			
			g_pExecutive->GetRenderer()->SetRenderMode(2);							// ������带 ���̾� ���������� �Ѵ�.

			bWireFrame = TRUE;														// ���̾� ������ ���θ� TRUE�� �����Ѵ�.
		}
		else																		// ���̾� ������ ���ΰ� TRUE���,
		{
			g_pExecutive->GetRenderer()->SetRenderMode(0);							// ������带 �ָ���� �Ѵ�.

			bWireFrame = FALSE;														// ���̾� ������ ���θ� FALSE�� �����Ѵ�.
		}
	}

	if(keyInfo->GetKeyDown(KEY_R) && keyInfo->GetKeyPressed(KEY_MENU))				// ALT�� RŰ�� ���ȴٸ�,
	{
		EFFECTMGR->StartEffectProcess(1034,HERO,NULL,0,0);							// 1034 ����Ʈ�� �����Ѵ�.
	}

	if(keyInfo->GetKeyDown(KEY_4) && keyInfo->GetKeyPressed(KEY_MENU))				// ALTŰ�� 4�� Ű�� ���ȴٸ�,
	{
		EFFECTMGR->StartHeroEffectProcess(FindEffectNum("Mall_eff_heal.beff"));		// �� ����Ʈ�� �����Ѵ�.
	}

	if(keyInfo->GetKeyDown(KEY_5) && keyInfo->GetKeyPressed(KEY_MENU))				// ALTŰ�� 5�� Ű�� ���ȴٸ�,
	{
		EFFECTMGR->StartHeroEffectProcess(FindEffectNum("m_ba_070.beff"));			// ����Ʈ�� �����Ѵ�.
	}

	if(keyInfo->GetKeyDown(KEY_7) && keyInfo->GetKeyPressed(KEY_MENU))				// ALTŰ�� 7�� Ű�� ���ȴٸ�,
	{
		EFFECTMGR->StartHeroEffectProcess(eEffect_ShopItem_Revive);					// �������� ��Ȱ ����Ʈ�� �����Ѵ�.
	}

	if(keyInfo->GetKeyDown(KEY_X) && keyInfo->GetKeyPressed(KEY_CONTROL))			// CTRLŰ�� XŰ�� ���ȴٸ�,
	{
		CWantNpcDialog* pDlg = GAMEIN->GetWantNpcDialog() ;							// WantNpcDialog ������ �޾ƿ´�.

		if( pDlg )																	// ���̾�α� ������ ��ȿ���� üũ�Ѵ�.
		{
			pDlg->SetActive(TRUE);													// ���̾�α׸� Ȱ��ȭ �Ѵ�.
			pDlg->ShowWantMode(eWantNpcMode_Npc);									// eWantNpcMode_Npc���� �����Ѵ�.
		}
		
		SEND_WANTNPCLIST Info;														// ���ϴ� NPC����Ʈ ����ü�� �����Ѵ�.
		Info.TotalPage = 6;															// �������� �����Ѵ�.
		char buf[64];																// �ӽ� ���۸� �����Ѵ�.
		
		for(int i=0; i<WANTUNIT_PER_PAGE; ++i)										// ������ �� ���ϴ� ���� ����ŭ ������ ������.
		{
			sprintf(buf, "����%d", i);												// �̸��� �����Ѵ�.
			SafeStrCpy(Info.WantNpcList[i].WantedName, buf, MAX_NAME_LENGTH+1);		// ���� ������ �����Ѵ�.
			Info.WantNpcList[i].Prize = i+100;										// 
			Info.WantNpcList[i].VolunteerNum = i+50;
			Info.WantNpcList[i].WantedIDX = i+1;
		}

		GAMEIN->GetWantNpcDialog()->SetWantedList(&Info);							// ������ ���̾�α׿� �����Ѵ�.
		
	}

	if(keyInfo->GetKeyDown(KEY_W))													// WŰ�� ���ȴٸ�,
	{
		CActionTarget Target;														// �׼� Ÿ���� �����Ѵ�.

		cSkillInfo* pInfo = NULL ;													// ��ų ������ ���� �����͸� �����Ѵ�.
		
		pInfo = SKILLMGR->GetSkillInfo(GAMERESRCMNGR->m_TestClientInfo.SkillIdx);	// �׽�Ʈ Ŭ���̾�Ʈ ��ų�ε����� ��ų ������ �޴´�.

		if( !pInfo )																// ��ų ������ ��ȿ���� �ʴٸ�,
		{
			return;																	// �����Ѵ�.
		}

		cActiveSkillInfo* pSkillInfo = ( cActiveSkillInfo* )pInfo;					// ��Ƽ�� ��ų ������ �޴´�.

		if(pSkillInfo->GetSkillInfo()->Target == 1)									// ��ų�� Ÿ���� 1�� ������,
		{
			CObject * targetObj = OBJECTMGR->GetSelectedObject();					// ������ Ÿ���� ������Ʈ ������ �޴´�.

			if( targetObj )															// ������Ʈ ������ ��ȿ���� üũ�Ѵ�.
			{
				Target.InitActionTarget(targetObj,NULL);							// Ÿ���� ������� �׼��� �ʱ�ȭ �Ѵ�.
			}
		}
		else if(pSkillInfo->GetSkillInfo()->Target == 0)							// ��ų�� Ÿ���� 0�� ������,
		{
			Target.InitActionTarget(HERO,NULL);										// ����θ� ������� �׼��� �ʱ�ȭ �Ѵ�.
		}
		
		SKILLMGR->OnSkillCommand( HERO, &Target, TRUE );							// ��ų �Ŵ��� ����� �����Ѵ�.
	}

	if(keyInfo->GetKeyDown(KEY_E))													// EŰ�� ���ȴٸ�,
	{
		CObject * targetObj = OBJECTMGR->GetSelectedObject();						// ������ Ÿ���� ������Ʈ ������ �޴´�.

		if( targetObj )																// �������� ������ ��ȿ���� üũ�Ѵ�.
		{
			TARGETSET Target;														// Ÿ�� �� ����ü�� �����Ѵ�.

			Target.pTarget = targetObj;												// Ÿ���� �����Ѵ�.
			memset(&Target.ResultInfo,0,sizeof(RESULTINFO));						// ��� ������ �޸� ���Ѵ�.
			Target.ResultInfo.bCritical = rand()%2 ? TRUE : FALSE;					// ũ��Ƽ�� ���θ� �����Ѵ�.
			Target.ResultInfo.RealDamage = rand() % 50;								// ���� �������� �����Ѵ�.
			Target.ResultInfo.CounterDamage = 0;									// ī���� �������� �����Ѵ�.

			// ����Ʈ�� �����Ѵ�.
			EFFECTMGR->StartEffectProcess(GAMERESRCMNGR->m_TestClientInfo.Effect,HERO,&Target,1,targetObj->GetID(),EFFECT_FLAG_HEROATTACK);
		}
	}

	if(keyInfo->GetKeyDown(KEY_4))													// 4��Ű�� ���ȴٸ�,
	{
		CObject * targetObj = OBJECTMGR->GetSelectedObject();						// Ÿ�� ������Ʈ ������ �޾ƿ´�.

		if( targetObj )																// ������Ʈ ������ ��ȿ���� üũ�Ѵ�.
		{
			VECTOR3 pos;															// ���͸� �����Ѵ�.

			targetObj->GetPosition(&pos);											// Ÿ���� ��ġ�� �޾ƿ´�.
			pos.z -= 3000;															// Z���� �����Ѵ�.

			MOVEMGR->SetMonsterMoveType(targetObj,eMA_WALKAROUND);					// ������ �̵� Ÿ���� eMA_WALKAROUND�� �����Ѵ�.
			MOVEMGR->StartMoveEx(targetObj,gCurTime,&pos);							// �̵��� �����Ѵ�.
		}
	}

	if(keyInfo->GetKeyDown(KEY_5))													// 5��Ű�� ���ôٸ�,
	{
		CObject * targetObj = OBJECTMGR->GetSelectedObject();						// Ÿ�� ������Ʈ ������ �޾ƿ´�.

		if( targetObj )																// ������Ʈ ������ ��ȿ���� üũ�Ѵ�.
		{
			VECTOR3 pos;															// ���͸� �����Ѵ�.

			targetObj->GetPosition(&pos);											// Ÿ���� ��ġ�� �޾ƿ´�.
			pos.z -= 3000;															// Z���� �����Ѵ�.

			MOVEMGR->SetMonsterMoveType(targetObj,eMA_PERSUIT);						// ������ �̵� Ÿ���� eMA_PERSUIT�� �����Ѵ�.
			MOVEMGR->StartMoveEx(targetObj,gCurTime,&pos);							// �̵��� �����Ѵ�.
		}
	}

	if(keyInfo->GetKeyDown(KEY_6))													// 6��Ű�� ���ȴٸ�,
	{
		CObject * targetObj = OBJECTMGR->GetSelectedObject();						// ������ Ÿ�� ������Ʈ ������ �޾ƿ´�.

		if( targetObj )																// ������Ʈ ������ ��ȿ���� üũ�Ѵ�.
		{
			VECTOR3 pos;															// ���͸� �����Ѵ�.

			targetObj->GetPosition(&pos);											// Ÿ���� ��ġ�� �޾ƿ´�.
			pos.z -= 3000;															// Z���� �����Ѵ�.

			MOVEMGR->SetMonsterMoveType(targetObj,eMA_RUNAWAY);						// ������ �̵� Ÿ���� eMA_RUNAWAY�� �����Ѵ�.
			MOVEMGR->StartMoveEx(targetObj,gCurTime,&pos);							// �̵��� �����Ѵ�.
		}
	}
#endif // _TESTCLIENT_
}

void CMacroManager::KeyboardInput_Cheat_NotTestClient( BOOL bDock, CKeyboard* keyInfo )
{
#if defined(_GMTOOL_)
	if(keyInfo->GetKeyDown(KEY_1) && keyInfo->GetKeyPressed(KEY_MENU))				// ALTŰ�� 1��Ű�� ���ȴٸ�,
	{
		CMoveDialog* pdlg = GAMEIN->GetMoveDialog();								// MoveDialog ������ �޾ƿ´�.

		if( pdlg )																	// ���̾�α� ������ ��ȿ���� üũ�Ѵ�.
		{
			pdlg->SetActive(!pdlg->IsActive());										// ���̾�α׸� Ȱ��,��Ȱ��ȭ �Ѵ�.
		}
	}

	if(keyInfo->GetKeyDown(KEY_2) && keyInfo->GetKeyPressed(KEY_MENU))				// ALTŰ�� 2��Ű�� ���ȴٸ�,
	{
		CItemShopDialog* pdlg = GAMEIN->GetItemShopDialog();						// �����ۼ� ���̾�α׸� �ҷ��´�.

		if( pdlg )																	// ���̾�α� ������ ��ȿ���� üũ�Ѵ�.
		{
			pdlg->SetActive(!pdlg->IsActive());										// ���̾�α׸� Ȱ��, ��Ȱ��ȭ �Ѵ�.
		}
	}
	if(keyInfo->GetKeyDown(KEY_3) && keyInfo->GetKeyPressed(KEY_MENU))				// ALTŰ�� 2��Ű�� ���ȴٸ�,
	{
		CPointSaveDialog* pdlg = GAMEIN->GetPointSaveDialog();						// ����Ʈ ���� ���̾�α׸� �ҷ� �´�.

		if( pdlg )																	// ���̾�α� ������ ��ȿ���� üũ�Ѵ�.
		{
			pdlg->SetActive(!pdlg->IsActive());										// ���̾�α� Ȱ��, ��Ȱ��ȭ �Ѵ�.
		}
	}

	//if(keyInfo->GetKeyDown(KEY_D) && keyInfo->GetKeyPressed(KEY_CONTROL))			// CTRLŰ�� DŰ�� ���ȴٸ�,
	//{
	//	CDissolutionDialog* pDlg = GAMEIN->GetDissolutionDialog() ;					// CDissolutionDialog ���̾�α׸� �޾ƿ´�.
	//	
	//	if( pDlg )																	// ���̾�α� ������ ��ȿ���� üũ�Ѵ�.
	//	{
	//		pDlg->SetActive( !pDlg->IsActive() ) ;									// ���̾�α׸� Ȱ��, ��Ȱ��ȭ �Ѵ�.
	//	}
	//}
	
	if(keyInfo->GetKeyDown(KEY_F1) && keyInfo->GetKeyPressed(KEY_CONTROL))			// CTRLŰ�� F1Ű�� ���ȴٸ�,
	{
		g_bDisplayFPS = !g_bDisplayFPS;												// �������� ����Ѵ�.
	}
	
	if(keyInfo->GetKeyDown(KEY_BACK) && keyInfo->GetKeyPressed(KEY_MENU))			// ALTŰ�� BACK Ű�� ������,
	{
		WINDOWMGR->ToggleShowInterface();											// �������̽��� ��� ��Ų��.
	}

	if(keyInfo->GetKeyDown(KEY_P) && keyInfo->GetKeyPressed(KEY_MENU))				// ALTŰ�� PŰ�� ������,
	{
		m_bInVisibleCursor ^= TRUE;													// Ŀ�� ��¿��θ� �����Ѵ�.
		CURSOR->SetCheatMode( m_bInVisibleCursor );									// Ŀ���� ġƮ���� �����Ѵ�.
	}

	if( keyInfo->GetKeyDown(KEY_C) && keyInfo->GetKeyPressed(KEY_CONTROL))	// ALTŰ�� CŰ�� ������,
	{
		if( CAMERA->GetCameraMode() == eCM_EyeView )								// ī�޶� ��尡 EyeView�� ������,
		{
			CAMERA->SetCameraMode( eCM_Free );										// ī�޶� ��带 Free���� �����Ѵ�.

			if( !WINDOWMGR->IsOpendAllWindows() )									// ��� â���� ���� ���� ������,
			{
				WINDOWMGR->ShowAllActivedWindow() ;									// Ȱ��ȭ �� ��� â�� �����ش�.

				WINDOWMGR->SetOpendAllWindows( TRUE ) ;								// ��� �����찡 ���� ������ �����Ѵ�.

				WINDOWMGR->ShowBaseWindow() ;										// �⺻ ��������� �����ش�.
			}

		}
		else																		// ī�޶� ��尡 EyeView�� ���� ������,
		{
			CAMERA->SetCameraMode( eCM_EyeView );									// ī�޶� ��带 EyeView�� �����Ѵ�.

			if( WINDOWMGR->IsOpendAllWindows() )									// ��� �����찡 ���� ������,
			{
				//WINDOWMGR->m_pActivedWindowList->RemoveAll() ;						// Ȱ��ȭ �� â���� ��� �ִ� ����Ʈ�� ��� ����.

				WINDOWMGR->CloseAllWindow() ;										// ��� �����츦 �ݴ´�.

				WINDOWMGR->SetOpendAllWindows( FALSE ) ;							// ��� �����찡 ���� ������ �����Ѵ�.
			}
		}
	}

	//---KES PK 071124 �ӽ� PK��� ON/OFF
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
		//cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_PKMODE, MBT_YESNO, "��⸦ �巯���ðڽ��ϱ�? �⺻ 30���̻� ���ӵǸ�, �������� ���� �� �ֽ��ϴ�." );
		PKMGR->CreateLootingDlg( 0 );
		PKMGR->SetPKLooting( TRUE );
	}
	*/
	//----------------------------------------------------------------

	//---KES AUTONOTE �ӽ� Dialogüũ
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

	if( bDock == TRUE) return;														// ���� ��ŷ���̶��, �����Ѵ�.

	if(keyInfo->GetKeyDown(KEY_I) && keyInfo->GetKeyPressed(KEY_MENU))				// ALTŰ�� IŰ�� ���ȴٸ�,
	{
		MSGBASE msg;																// �⺻ �޽��� ����ü�� �����Ѵ�.

		msg.Category = MP_ITEM;														// ī�װ��� ���������� �����Ѵ�.
		msg.Protocol = MP_ITEM_SHOPITEM_INFO_SYN;									// ���������� �� ������ ���� ��ũ�� �Ѵ�.
		msg.dwObjectID = HERO->GetID();												// ������Ʈ ���̵�� ����� ���̵� �����Ѵ�.

		NETWORK->Send(&msg, sizeof(msg));											// �޽����� �����Ѵ�.
	}

	//if(keyInfo->GetKeyDown(KEY_N) && keyInfo->GetKeyPressed(KEY_CONTROL))			// CTRLŰ�� NŰ�� ���ȴٸ�,
	//{
	//	CReinforceResetDlg* pDlg = GAMEIN->GetReinforceResetDlg() ;					// ��ȭ �ʱ�ȭ ���̾�α׸� �޾ƿ´�.

	//	if( pDlg )																	// ���̾�α� ������ ��ȿ���� üũ�Ѵ�.
	//	{
	//		pDlg->SetActive( !GAMEIN->GetReinforceResetDlg()->IsActive() );			// ���̾�α׸� Ȱ��, ��Ȱ��ȭ �Ѵ�.
	//	}
	//}
#else
	// �Ϲ� Ŭ���̾�Ʈ �� ���� �ʿ�����Ƿ�..
#endif
}

void CMacroManager::KeyboardInput( CKeyboard* keyInfo )
{
	BOOL bDock = FALSE;																// ��ŷ���θ� ���� ������ FALSE�� �����Ѵ�.

	KeyboardInput_Normal( bDock, keyInfo ) ;										// �Ϲ����� Ű���� �Է� ó���� �Ѵ�.

#ifdef _GMTOOL_																		// GM�� ó���� �����Ѵ�.

	KeyboardInput_GMTool( bDock, keyInfo ) ;										// GM���� �� Ű���� �Է� ó���� �Ѵ�.

#endif																				// GM�� ó�� ����.


#ifdef _CHEATENABLE_																// ġƮ����� �� ó���� �����Ѵ�.

	KeyboardInput_CheatEnable( bDock, keyInfo ) ;									// ġƮ����� �� Ű���� �Է� ó���� �Ѵ�.

#endif	//_CHEATENABLE_																// ġƮ����� �� ó���� ���� �Ѵ�.

}

void CMacroManager::KeyboardPressInput( CKeyboard* keyInfo )
{
	if( !g_bActiveApp || m_bPause )	return;											// ���α׷��� Ȱ��ȭ ������ �ʰų�, ��������̸� �����Ѵ�.

	if( MAINGAME->GetCurStateNum() != eGAMESTATE_GAMEIN ) return;					// ������ ���°� ���̸� �����Ѵ�.

	if( CIMEWND->IsDocking() )														// ��ŷ�����̸�, 
	{
		return;																		// �����Ѵ�.
	}

	int nSysKey = MSK_NONE;															// �ý��� Ű ���� �⺻���� �����Ѵ�.

	if( keyInfo->GetKeyPressed( KEY_CONTROL ) )		nSysKey |= MSK_CTRL;			// ��Ʈ�� Ű�� ��������, ��Ʈ�� Ű�� �߰��Ѵ�.
	if( keyInfo->GetKeyPressed( KEY_MENU ) )		nSysKey |= MSK_ALT;				// ��Ʈ Ű�� ��������, ��ƮŰ�� �߰��Ѵ�.
	if( keyInfo->GetKeyPressed( KEY_SHIFT ) )		nSysKey |= MSK_SHIFT;			// ����Ʈ Ű�� ��������, ����ƮŰ�� �߰��Ѵ�.


	for( int i = 0 ; i < MPE_COUNT ; ++i )											// ��ũ�� ������ �̺�Ʈ ����ŭ ������ ������.
	{
		if( m_MacroPressKey[m_nMacroMode][i].nSysKey & nSysKey						// ���� ��ũ�� Ű�� �ý��� Ű�� Ȯ���ϰ�, ��ũ�� Ű�� ��������,
			&& keyInfo->GetKeyPressed( m_MacroPressKey[m_nMacroMode][i].wKey ) )
		{
			PlayMacroPress( i );													// ��ũ�θ� �����Ѵ�.
		}
	}		
}

void CMacroManager::GetMacro( int nMacroMode, sMACRO* pMacro )
{
	memcpy( pMacro, m_MacroKey[nMacroMode], sizeof(sMACRO) * ME_COUNT );			// ���� ��ũ�� ���� ���� ��ũ�� ����ü�� ��ũ�� ������ �����Ѵ�.
}

void CMacroManager::SetMacro( int nMacroMode, sMACRO* pMacro )
{
	memcpy( m_MacroKey[nMacroMode], pMacro, sizeof(sMACRO) * ME_COUNT );			// ���� ��ũ�� ���� ���� ��ũ�� ����ü�� ��� ��ũ�� ����ü�� �����Ѵ�.
}

void CMacroManager::GetDefaultMacro( int nMacroMode, sMACRO* pMacro )
{
	memcpy( pMacro, m_DefaultKey[nMacroMode], sizeof(sMACRO) * ME_COUNT );			// ����Ʈ ��ũ�θ� ���� ��ũ�� ����ü�� �����Ѵ�.
}

void CMacroManager::SetMacroMode( int nMacroMode )
{
	m_nMacroMode = nMacroMode;														// ���ڷ� ���� ��ũ�� ��带 ��� ��ũ�� ��� ������ �����Ѵ�.

	if( m_nMacroMode == MM_CHAT )													// ��ũ�θ�尡 ä�ø��� ���ٸ�,
	{
		m_bChatMode = TRUE;															// ä�ø�� ������ TRUE�� �����Ѵ�.
	}
	else																			// ��ũ�θ�尡 ä�ø��� ���� �ʴٸ�,
	{
		m_bChatMode = FALSE;														// ä�ø�� ������ FALSE�� �����Ѵ�.
	}
}

void CMacroManager::LoadUserMacro()
{
	SetDefaultMacro( MM_MACRO );													// ����Ʈ ��ũ�θ� ����Ű �켱 ���� �����Ѵ�.
	SetDefaultMacro( MM_CHAT );														// ����Ʈ ��ũ�θ� ä�� �켱 ���� �����Ѵ�.
	SetMacroMode( MM_MACRO );														// ��ũ�� ��带 ����Ű �켱 ���� �����Ѵ�.
}


BOOL CMacroManager::LoadMacro( LPCTSTR strPath )
{
	HANDLE	hFile;																	// ���� �ڵ��� �����Ѵ�.

	hFile = CreateFile( strPath, GENERIC_READ, 0, NULL,								// �б���� ���� �н��� ������ �о� �ڵ��� �Ѱ� �޴´�.
						OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

	if( hFile == INVALID_HANDLE_VALUE )												// �ڵ��� �޴µ� �����ߴٸ�,
		return FALSE;																// ����ó���� �Ѵ�.

	DWORD dwRead;																	// ������ ���� ����Ʈ ���� ���Ϲޱ� ���� ��¿� �μ�.
	
	if( !ReadFile( hFile, m_MacroKey,												// ��ũ��Ű ����ü��, ��ũ�� Ű ������ ��ŭ �о���δ�.
		sizeof( m_MacroKey ), &dwRead, NULL ) )										// �о���̴°� �����ϸ�,
	{
		CloseHandle( hFile );														// �ڵ��� �ݴ´�.
		return FALSE;																// FALSE ���� ó���� �Ѵ�.
	}

	if( !ReadFile( hFile, m_MacroPressKey,											// m_MacroPressKey����ü�� m_MacroPressKey ������ ��ŭ �о���δ�.
		sizeof( m_MacroPressKey ), &dwRead, NULL ) )								// �о���̴°� �����ϸ�,
	{
		CloseHandle( hFile );														// �ڵ��� �ݴ´�.
		return FALSE;																// FALSE ���� ó���� �Ѵ�.
	}
	
	CloseHandle( hFile );															// �ڵ��� �ݴ´�.

	return TRUE;																	// TRUE ���� ó���� �Ѵ�.
}

void CMacroManager::SaveUserMacro()
{
	SaveMacro( "./INI/MacroUserSet.mcr" );											// ��ũ�θ� �����Ѵ�.
}

BOOL CMacroManager::SaveMacro( LPCTSTR strPath )
{
	HANDLE	hFile;																	// ���� �ڵ��� �����Ѵ�.

	hFile=CreateFile( strPath, GENERIC_WRITE, 0, NULL,								// ������ �н��� ������� ���� �ڵ��� �޾ƿ´�.
						CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	if( hFile == INVALID_HANDLE_VALUE )												// ���� �ڵ��� �޴µ� �����ϸ�,
		return FALSE;																// FALSE ���� ó���� �Ѵ�.
	
	DWORD dwWritten;																// ������ �� ����Ʈ ���� ���Ϲޱ� ���� ��¿� �μ�.
	
	if( !WriteFile( hFile, m_MacroKey,												// m_MacroKey ����ü�� ��� ������ m_MacroKey ����ü ������ ��ŭ ����.
		sizeof( m_MacroKey ), &dwWritten, NULL ) )									// ���⿡ �����Ѵٸ�,
	{
		CloseHandle( hFile );														// �ڵ��� �ݴ´�.
		return FALSE;																// FALSE ���� ó���� �Ѵ�.
	}

	if( !WriteFile( hFile, m_MacroPressKey,											// m_MacroPressKey ����ü�� ��� ������ m_MacroPressKey ����ü ������ ��ŭ ����.
		sizeof( m_MacroPressKey ), &dwWritten, NULL ) )								// ���⿡ �����Ѵٸ�,
	{
		CloseHandle( hFile );														// �ڵ��� �ݴ´�.
		return FALSE;																// FALSE ����ó���� �Ѵ�.
	}

	CloseHandle( hFile );															// �ڵ��� �ݴ´�.
	return TRUE;																	// TRUE ���� ó���� �Ѵ�.
}

void CMacroManager::PlayMacro( int nMacroEvent )
{
	int nMacroMode = OPTIONMGR->GetGameOption()->nMacroMode ;						// ä�� �켱�������, ����Ű �켱������� üũ�Ѵ�.

	//if( nMacroMode == 0 ) return ;												// ��ũ�θ�尡 0�� ������ ���� ó���� �Ѵ�.

	if( CAMERA->GetCameraMode() == eCM_EyeView )									// ī�޶� ��尡 eCM_EyeView�� ���ٸ�,
	{
		return;																		// ���� ó���� �Ѵ�.
	}

	switch( nMacroEvent )															// ��ũ�� �̺�Ʈ�� Ȯ���Ѵ�.
	{
		case ME_USE_QUICKITEM_1_01: 												// ���� ������ 1��Ű ���
		case ME_USE_QUICKITEM_1_02: 												// ���� ������ 2��Ű ���
		case ME_USE_QUICKITEM_1_03: 												// ���� ������ 3��Ű ���
		case ME_USE_QUICKITEM_1_04: 												// ���� ������ 4��Ű ���
		case ME_USE_QUICKITEM_1_05: 												// ���� ������ 5��Ű ���
		case ME_USE_QUICKITEM_1_06: 												// ���� ������ 6��Ű ���
		case ME_USE_QUICKITEM_1_07: 												// ���� ������ 7��Ű ���
		case ME_USE_QUICKITEM_1_08: 												// ���� ������ 8��Ű ���
		case ME_USE_QUICKITEM_1_09: 												// ���� ������ 9��Ű ���
		case ME_USE_QUICKITEM_1_10:	PM_UseMainQuickSlotItem(nMacroEvent) ;	break ;	// ���� ������10��Ű ���

		case ME_USE_QUICKITEM_2_01: 												// ���� ������ 1��Ű ��� 
		case ME_USE_QUICKITEM_2_02: 			 									// ���� ������ 2��Ű ��� 
		case ME_USE_QUICKITEM_2_03: 			 									// ���� ������ 3��Ű ��� 
		case ME_USE_QUICKITEM_2_04: 			 									// ���� ������ 4��Ű ��� 
		case ME_USE_QUICKITEM_2_05: 			 									// ���� ������ 5��Ű ��� 
		case ME_USE_QUICKITEM_2_06: 			 									// ���� ������ 6��Ű ��� 
		case ME_USE_QUICKITEM_2_07: 			 									// ���� ������ 7��Ű ��� 
		case ME_USE_QUICKITEM_2_08: 			 									// ���� ������ 8��Ű ��� 
		case ME_USE_QUICKITEM_2_09: 			 									// ���� ������ 9��Ű ��� 
		case ME_USE_QUICKITEM_2_10:	PM_UseSubQuickSlotItem(nMacroEvent) ;	break ;	// ���� ������10��Ű ��� 

		case ME_CHANGE_QUICKSLOT_01: 												// ���� �������� 1������ ������.
		case ME_CHANGE_QUICKSLOT_02: 												// ���� �������� 2������ ������.
		case ME_CHANGE_QUICKSLOT_03: 												// ���� �������� 3������ ������.
		case ME_CHANGE_QUICKSLOT_04: 												// ���� �������� 4������ ������.
		case ME_CHANGE_QUICKSLOT_05: 												// ���� �������� 5������ ������.
		case ME_CHANGE_QUICKSLOT_06: 												// ���� �������� 6������ ������.
		case ME_CHANGE_QUICKSLOT_07: 												// ���� �������� 7������ ������.
		case ME_CHANGE_QUICKSLOT_08:	PM_Change_MainSlotNum(nMacroEvent) ;break ;	// ���� �������� 8������ ������.

		case ME_INCREASE_MAIN_QUICKSLOT_NUM:														// ���� ������ ��ȣ�� ���� ��Ų��.
		case ME_DECREASE_MAIN_QUICKSLOT_NUM: PM_SetSlotPage(QI1_QUICKSLOTDLG, nMacroEvent) ; break ;// ���� ������ ��ȣ�� ���� ��Ų��.

		// 080707 LYW --- MacroManager : Ȯ�� ���� ����Ű�� ���´�.
		//case ME_INCREASE_SUB_QUICKSLOT_NUM: 														// ���� ������ ��ȣ�� ���� ��Ų��.
		//case ME_DECREASE_SUB_QUICKSLOT_NUM:  PM_SetSlotPage(QI2_QUICKSLOTDLG, nMacroEvent) ; break ;// ���� ������ ��ȣ�� ���� ��Ų��.

		case ME_SELECT_MYSELF: HERO->ShowObjectName( TRUE, NAMECOLOR_SELECTED ); OBJECTMGR->SetSelectedObject( HERO ); break ;// �� �ڽ��� �����Ѵ�.

		case ME_SELECT_PARTYMEMBER_1: 												// ù ��° ��Ƽ ����� �����Ѵ�.
		case ME_SELECT_PARTYMEMBER_2: 												// �� ��° ��Ƽ ����� �����Ѵ�.
		case ME_SELECT_PARTYMEMBER_3: 												// �� ��° ��Ƽ ����� �����Ѵ�.
		case ME_SELECT_PARTYMEMBER_4: 												// �� ��° ��Ƽ ����� �����Ѵ�.
		case ME_SELECT_PARTYMEMBER_5: 												// �ټ� ��° ��Ƽ ����� �����Ѵ�.
		case ME_SELECT_PARTYMEMBER_6:	PM_SelectPartyMember(nMacroEvent) ;break ;	// ���� ��° ��Ƽ ����� �����Ѵ�.

		case ME_SELECT_NEAR_TARGET:		PM_SetAutoTarget() ;				break ;	// ���� �����̿� �ִ� Ÿ���� �����Ѵ�.

		case ME_SCREENCAPTURE:					PM_ScreenCapture() ;		break ;	// ��ũ���� ĸ���Ѵ�.

		case ME_TOGGLE_EXITDLG:					PM_Toggle_ExitDlg() ;		break ;	// ���� �޽��� â�� ����.

		case ME_TOGGLE_ALLINTERFACE:			PM_Toggle_AllInterface() ;	break ;	// ��� �������̽��� ����Ѵ�.
		
		case ME_TOGGLE_SKILLDLG:				PM_Toggle_SkillDlg() ;		break ;	// ��ųâ�� ����Ѵ�.

		case ME_TOGGLE_FAMILYDLG:				PM_Toggle_FamilyDlg() ;		break ;	// �йи� â�� ����Ѵ�.

		case ME_TOGGLE_INVENTORYDLG:			PM_Toggle_InventoryDlg() ;  break ;	// �κ��丮�� ����Ѵ�.

		case ME_TOGGLE_HELP_SHOW:				PM_Toggle_ShowKeyTip() ;	break ;	// ������ ����Ѵ�.

		case ME_TOGGLE_MINIMAP:					PM_Toggle_MiniMap() ;		break ;	// �̴ϸ��� ����Ѵ�.

		case ME_TOGGLE_BIGMAP:					PM_Toggle_BigMap() ;		break ;	// ����� ����Ѵ�.

		case ME_TOGGLE_OPTIONDLG:				PM_Toggle_OptionDlg() ;		break ;	// �ɼ� â�� ����Ѵ�.

		case ME_TOGGLE_GUILDDLG:				PM_Toggle_GuildDlg() ;		break ;	// ��� â�� ����Ѵ�.

		case ME_TOGGLE_QUESTDLG:				PM_Toggle_QuestDlg() ;		break ;	// ����Ʈ â�� ����Ѵ�.

		case ME_TOGGLE_CHARINFODLG:				PM_Toggle_CharInfoDlg() ;	break ;	// ĳ���� ����â�� ����Ѵ�.

		case ME_WALK_RUN:						MOVEMGR->ToggleHeroMoveMode() ;	break ;	// �ȱ�/�ٱ� ��带 ��ȯ�Ѵ�.

		case ME_TOGGLE_CAMERAVIEWMODE:			CAMERA->ToggleCameraViewMode(); break ;	// ī�޶� �� ��带 ��ȯ�Ѵ�.

		case ME_TOGGLE_MATCHINGDLG:				PM_Toggle_MatchingDlg() ;		break ;	// ��Ī â�� ����Ѵ�.

		case ME_TOGGLE_FRIENDDLG:				PM_Toggle_FriendDlg() ;		break ;		// ģ�� ����� ����Ѵ�.
		case ME_TOGGLE_NOTEDLG:					PM_Toggle_NoteDlg() ;		break ;		// ���� â�� ����Ѵ�.

		// 071025 LYW --- MacroManager : Add macro for help dialog.
		case ME_TOGGLE_HELPDLG :				PM_Toggle_HelpDlg() ;		break ;		// ���� â�� ����Ѵ�.

		case ME_FISHING_PULLINGBTN:				GAMEIN->GetFishingDlg()->Fishing_Pullling();	break;

		// 080429 LYW --- MacroManager : Add macro for chatroom system.
		case ME_TOGGLE_CHATROOM_LISTDLG :		PM_Toggle_ChatRoomListDlg() ; break ;	// ä�ù� ����Ʈâ�� ����Ѵ�.
		case ME_TOGGLE_CHATROOM_ROOMDLG :		PM_Toggle_ChatRoomDlg() ;	  break ;	// ä�ù��� ��Ŭ�Ѵ�.
		case ME_SETFOCUS_CHATROOM_CHATTING :	PM_SetFocus_ChatRoom_Chat() ; break ;	// ä��â�� ä�ù� ���̷� ����Ʈ �ڽ� ��Ŀ���� �̵���Ų��.


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
	else if(GAMEIN->GetFishingDlg()->IsPushedStartBtn())		// �������̸� ���ø� �ߴ��Ѵ�.
	{
		GAMEIN->GetFishingDlg()->Fishing_Cancel();
	}
	else if( !WINDOWMGR->CloseAllAutoCloseWindows() )									// �ڵ����� ������ ��������� �ݴ� ���� �����ϸ�,?
	{
		// 080117 LYW --- MacroManager : ç���� �ʿ����� ESCŰ ����� ���´�.
		if (!g_csDateManager.IsChallengeZone(MAP->GetMapNum()))
		{
			CExitDialog * pWindow = GAMEIN->GetExitDialog();							// ���� �޽��� ������ ������ �޾ƿ´�.

			if(pWindow)																	// ���� �޽��� ������ ������ ��ȿ�ϸ�,
			{
				pWindow->SetActive(!pWindow->IsActive()) ;								// ���� �޽��� �����츦 Ȱ��, ��Ȱ��ȭ �Ѵ�.
			}
		}
	}	

	CObject* pObject = NULL ;														// ������Ʈ �����͸� ���� �� �ʱ�ȭ �Ѵ�.

	pObject = OBJECTMGR->GetObject(m_dwTargetID) ;									// ���õ� ������Ʈ ������ �޾ƿ´�.

	if( pObject )																	// ������Ʈ ������ ��ȿ�ϴٸ�,
	{
		pObject->ShowObjectName( FALSE, NAMECOLOR_DEFAULT );						// ������Ʈ �̸��� ����Ʈ �������� �����Ѵ�.

		OBJECTMGR->SetSelectedObject(NULL) ;										// ������Ʈ ������ ���� �Ѵ�.
	}
}

void CMacroManager::PM_ScreenCapture()
{
	DIRECTORYMGR->SetLoadMode(eLM_Root);											// �ε� ��带 eLM_Root�� �����Ѵ�.

	CreateDirectory( "ScreenShot", NULL );											// ��ũ�� �� ���丮�� �����Ѵ�.
	
	WORD wSrv = 0, wY = 0, wM = 0, wD = 0, wN = 0;									// ������ ���� �������� �����Ѵ�.
	WORD wHH = 0, wMM = 0, wSS = 0;													// �ð��븦 ���� �������� �����Ѵ�.

	char fname[256];																// ���ϸ��� ���� �ӽ� ���۸� �����Ѵ�.

	SYSTEMTIME ti;																	// �ð� ������ ���� ����ü�� �����Ѵ�.
	GetLocalTime( &ti );															// ���ýð��� �޾ƿ´�.

	HANDLE hFile = CreateFile( "screenshot/ScreenShot.cnt", GENERIC_READ, 0, NULL,	// �б���� ScreenShot.cnt�� �о� �����ڵ��� �Ѱ� �޴´�.
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

	DWORD dwSize = 0;																// ����� ���� ������ �����Ѵ�.

	if( hFile != INVALID_HANDLE_VALUE )												// �ڵ��� �Ѱܹ޴µ� �����ߴٸ�,
	{
		ReadFile( hFile, &wSrv, sizeof(wSrv), &dwSize, NULL );						// ������ ��ȣ�� �޴´�.
		ReadFile( hFile, &wY, sizeof(wY), &dwSize, NULL );							// �⵵�� �޴´�.
		ReadFile( hFile, &wM, sizeof(wM), &dwSize, NULL );							// ���� �޴´�.
		ReadFile( hFile, &wD, sizeof(wD), &dwSize, NULL );							// ������ �޴´�.
		ReadFile( hFile, &wHH, sizeof(wHH), &dwSize, NULL );						// �ð��� �޴´�.
		ReadFile( hFile, &wMM, sizeof(wMM), &dwSize, NULL );						// ���� �޴´�.
		ReadFile( hFile, &wSS, sizeof(wSS), &dwSize, NULL );						// �ʸ� �޴´�.
		ReadFile( hFile, &wN, sizeof(wN), &dwSize, NULL );							// �и� ������ ���� �޴´�.
		
		CloseHandle( hFile );														// ���� �ڵ��� �ݴ´�.

		if( !( wSrv == (WORD)g_nServerSetNum && ti.wYear == wY &&					// �о���� ��������
			ti.wMonth == wM && ti.wDay == wD && ti.wHour == wHH &&					// ���� �ð���� ���� ������,
			ti.wMinute == wMM && ti.wSecond == wSS) )
		{
			wSrv = g_nServerSetNum; wY = ti.wYear; wM = ti.wMonth;					// GetLocalTime()���� ���� ������ �������� �����Ѵ�.
			wD = ti.wDay; wN = 1; 
			wHH = ti.wHour; wMM = ti.wMinute; wSS = ti.wSecond;
		}					
	}
	else																			// �ڵ��� �޴µ� �����ߴٸ�,
	{
		wSrv = g_nServerSetNum; wY = ti.wYear; wM = ti.wMonth;						// GetLocalTime()���� ���� ������ �������� �����Ѵ�.
		wD = ti.wDay; wN = 1; 
		wHH = ti.wHour; wMM = ti.wMinute; wSS = ti.wSecond;
	}


	sprintf(fname,"ScreenShot/Luna_%02d_%02d%02d%02d_%02d%02d%02d_%03d.jpg",		// ���ϸ��� ������ ������ �������� ���Ͽ� �����Ѵ�.
		wSrv, wY%100, wM, wD, wHH, wMM, wSS, wN );

	BOOL rt = g_pExecutive->GetRenderer()->CaptureScreen(fname);					// ���ϸ����� ��ũ���� ĸ���Ѵ�.

	if( rt )																		// ĸ���� ����� �����̸�,
	{
		hFile = CreateFile( "screenshot/ScreenShot.cnt", GENERIC_WRITE, 0, NULL,	// ������� ScreenShot.cnt ������ ���� 
						CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );				// ���� �ڵ��� �Ѱ� �޴´�.

		if( hFile != INVALID_HANDLE_VALUE )											// ���� ���⿡ �����ߴٸ�,
		{
			++wN;																	// �и� ������ ���� ������Ų��.

			WriteFile( hFile, &wSrv, sizeof(wSrv), &dwSize, NULL );					// ������ ��ȣ�� ����.
			WriteFile( hFile, &wY, sizeof(wY), &dwSize, NULL );						// ������ ����Ѵ�.
			WriteFile( hFile, &wM, sizeof(wM), &dwSize, NULL );						// ���� ����Ѵ�.
			WriteFile( hFile, &wD, sizeof(wD), &dwSize, NULL );						// ������ ����Ѵ�.
			WriteFile( hFile, &wHH, sizeof(wHH), &dwSize, NULL );					// �ð��� ����Ѵ�.
			WriteFile( hFile, &wMM, sizeof(wMM), &dwSize, NULL );					// ���� ����Ѵ�.
			WriteFile( hFile, &wSS, sizeof(wSS), &dwSize, NULL );					// �ʸ� ����Ѵ�.
			WriteFile( hFile, &wN, sizeof(wN), &dwSize, NULL );						// �и� �ʸ� ����Ѵ�.

			CloseHandle( hFile );													// ���� �ڵ��� �ݴ´�.
		}

		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(387), fname );				// ��ũ���� ĸ�� �ߴٴ� �޽����� ����Ѵ�.
	}
	else																			// ���� ���⿡ �����ߴٸ�,
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(386) );					// ��ũ�� ĸ�� ���� �޽����� ����Ѵ�.
	}

	FIBITMAP* pBitmap = FreeImage_Load(FIF_TARGA, fname, TARGA_DEFAULT);			// ���� �̹��� ������ �ε��Ѵ�.
	
	if(pBitmap)																		// �̹��� ������ ��ȿ�ϸ�,
	{
		DeleteFile(fname);															// ������ �����Ѵ�.

		int len = strlen(fname);													// ���ϸ��� ���̸� ���Ѵ�.

		fname[len-3] = 'j';															// ���ϸ� jpg�� �߰��Ѵ�.
		fname[len-2] = 'p';
		fname[len-1] = 'g';

		FreeImage_SaveJPEG(pBitmap,fname,JPEG_QUALITYSUPERB);						// jpg �̹����� ������ �Ѵ�.

		FreeImage_Unload(pBitmap);													// �޾ƿ� �̹��� ������ ��ȯ�Ѵ�.
	}
}

void CMacroManager::PM_UseMainQuickSlotItem( int nMacroEvent )
{
	int slotNum = -1 ;																// ���� ��ȣ�� �����Ѵ�.

	switch( nMacroEvent )															// ��ũ�� �̺�Ʈ�� Ȯ���Ѵ�.
	{
	case ME_USE_QUICKITEM_1_01:		slotNum = 0 ;	break ;							// ���� ��ȣ�� 0���� �����Ѵ�.
	case ME_USE_QUICKITEM_1_02:		slotNum = 1 ;	break ;							// ���� ��ȣ�� 1���� �����Ѵ�.
	case ME_USE_QUICKITEM_1_03:		slotNum = 2 ;	break ;							// ���� ��ȣ�� 2���� �����Ѵ�.
	case ME_USE_QUICKITEM_1_04:		slotNum = 3 ;	break ;							// ���� ��ȣ�� 3���� �����Ѵ�.
	case ME_USE_QUICKITEM_1_05:		slotNum = 4 ;	break ;							// ���� ��ȣ�� 4���� �����Ѵ�.
	case ME_USE_QUICKITEM_1_06:		slotNum = 5 ;	break ;							// ���� ��ȣ�� 5���� �����Ѵ�.
	case ME_USE_QUICKITEM_1_07:		slotNum = 6 ;	break ;							// ���� ��ȣ�� 6���� �����Ѵ�.
	case ME_USE_QUICKITEM_1_08:		slotNum = 7 ;	break ;							// ���� ��ȣ�� 7���� �����Ѵ�.
	case ME_USE_QUICKITEM_1_09:		slotNum = 8 ;	break ;							// ���� ��ȣ�� 8���� �����Ѵ�.
	case ME_USE_QUICKITEM_1_10:		slotNum = 9 ;	break ;							// ���� ��ȣ�� 9���� �����Ѵ�.

	default :																		// �� ���� �̺�Ʈ��
		break ;																		// �ƹ��͵� ���� �ʴ´�.
	}

	if( slotNum != -1 )																// ���� ��ȣ�� üũ�Ѵ�.				
	{	
//		const DWORD remainedTick = 200;

		// ���ӵ� Ű �Է��� ���ۿ� �׿� ������ ���������� ��û�ϴ� ��츦 ���´�.
//		static DWORD beginTick	= GetTickCount();
//		static DWORD endTick	= beginTick + remainedTick;

//		const DWORD currentTick = GetTickCount();

//		if( ( beginTick < endTick && endTick < currentTick ) ||
//			( beginTick > endTick && beginTick > currentTick && endTick < currentTick ) )
//		{
			QUICKMGR->UseQuickItem( GAMEIN->GetQuickDlg()->GetSlotDlg( 0 )->GetCurSheet(), slotNum );	// ���� �� ���� ��ȣ�� ����Ѵ�.

//			beginTick = currentTick;
//			endTick		= beginTick + remainedTick;
//		}
//		else
//		{
			//CHATMGR->AddMsg( CTC_SYSMSG, "1�� �� �ٽ� �Է��ϼ���" );
//		}
	}
}

void CMacroManager::PM_UseSubQuickSlotItem( int nMacroEvent )
{
	// 080704 LYW --- MacroManager : Ȯ�� ���� �߰� ó��.
	// ����â ������ �޴´�.
	cQuickDlg* pQuickDlg = NULL ;
	pQuickDlg = GAMEIN->GetQuickDlg() ;
	if(!pQuickDlg) return ;

    // Ȯ�� ���� ������ �޴´�.
	cQuickSlotDlg* pExSlotDlg = NULL ;
	pExSlotDlg = pQuickDlg->GetExSlotDlg(0) ;
	if(!pExSlotDlg) return ;

	// ���� �� ��ȣ�� �޴´�.
	WORD wTab = 0 ;
	wTab = pExSlotDlg->GetCurSheet() ;

	int slotNum = -1 ;																// ���� ��ȣ�� �����Ѵ�.
																					
	switch( nMacroEvent )															// ��ũ�� �̺�Ʈ�� Ȯ���Ѵ�.
	{																				
	case ME_USE_QUICKITEM_2_01:		slotNum = 0 ;	break ;							// ���� ��ȣ�� 0���� �����Ѵ�.
	case ME_USE_QUICKITEM_2_02:		slotNum = 1 ;	break ;							// ���� ��ȣ�� 1���� �����Ѵ�.
	case ME_USE_QUICKITEM_2_03:		slotNum = 2 ;	break ;							// ���� ��ȣ�� 2���� �����Ѵ�.
	case ME_USE_QUICKITEM_2_04:		slotNum = 3 ;	break ;							// ���� ��ȣ�� 3���� �����Ѵ�.
	case ME_USE_QUICKITEM_2_05:		slotNum = 4 ;	break ;							// ���� ��ȣ�� 4���� �����Ѵ�.
	case ME_USE_QUICKITEM_2_06:		slotNum = 5 ;	break ;							// ���� ��ȣ�� 5���� �����Ѵ�.
	case ME_USE_QUICKITEM_2_07:		slotNum = 6 ;	break ;							// ���� ��ȣ�� 6���� �����Ѵ�.
	case ME_USE_QUICKITEM_2_08:		slotNum = 7 ;	break ;							// ���� ��ȣ�� 7���� �����Ѵ�.
	case ME_USE_QUICKITEM_2_09:		slotNum = 8 ;	break ;							// ���� ��ȣ�� 8���� �����Ѵ�.
	case ME_USE_QUICKITEM_2_10:		slotNum = 9 ;	break ;							// ���� ��ȣ�� 9���� �����Ѵ�.
																					
	default :																		// �� ���� �̺�Ʈ��
		break ;																		// �ƹ��͵� ���� �ʴ´�.
	}																				
																					
	if( slotNum != -1 )																// ���� ��ȣ�� üũ�Ѵ�.				
	{		
		// 080704 LYW --- MacroManager : Ȯ�� ���� �߰� ó��.
		//QUICKMGR->UseQuickItem( GAMEIN->GetQuickDlg()->GetSlotDlg( 1 )->GetCurSheet(), slotNum );	// ���� �� ���� ��ȣ�� ����Ѵ�.
		QUICKMGR->UseQuickItem( wTab+MAX_SLOTPAGE, slotNum );	// ���� �� ���� ��ȣ�� ����Ѵ�.
	}
}

void CMacroManager::PM_Change_MainSlotNum( int nMacroEvent )
{
	int slotNum = -1 ;																// ���� ��ȣ�� �����Ѵ�.
																					
	switch( nMacroEvent )															// ��ũ�� �̺�Ʈ�� Ȯ���Ѵ�.
	{																				
	case ME_CHANGE_QUICKSLOT_01:		slotNum = 0 ;	break ;						// ���� ��ȣ�� 0���� �����Ѵ�.
	case ME_CHANGE_QUICKSLOT_02:		slotNum = 1 ;	break ;						// ���� ��ȣ�� 1���� �����Ѵ�.
	case ME_CHANGE_QUICKSLOT_03:		slotNum = 2 ;	break ;						// ���� ��ȣ�� 2���� �����Ѵ�.
	case ME_CHANGE_QUICKSLOT_04:		slotNum = 3 ;	break ;						// ���� ��ȣ�� 3���� �����Ѵ�.
	case ME_CHANGE_QUICKSLOT_05:		slotNum = 4 ;	break ;						// ���� ��ȣ�� 4���� �����Ѵ�.
	case ME_CHANGE_QUICKSLOT_06:		slotNum = 5 ;	break ;						// ���� ��ȣ�� 5���� �����Ѵ�.
	case ME_CHANGE_QUICKSLOT_07:		slotNum = 6 ;	break ;						// ���� ��ȣ�� 6���� �����Ѵ�.
	case ME_CHANGE_QUICKSLOT_08:		slotNum = 7 ;	break ;						// ���� ��ȣ�� 7���� �����Ѵ�.
																					
	default :																		// �� ���� �̺�Ʈ��
		break ;																		// �ƹ��͵� ���� �ʴ´�.
	}																				
																					
	if( slotNum != -1 )																// ���� ��ȣ�� üũ�Ѵ�.				
	{
		cQuickSlotDlg* pSlot = ( cQuickSlotDlg* )WINDOWMGR->GetWindowForID( QI1_QUICKSLOTDLG ) ;	// ���� ������ ���´�.

		if(pSlot)																	// ���� ������ ��ȿ���� üũ�Ѵ�.
		{
			pSlot->SelectPage( (WORD)slotNum );										// �������� �����Ѵ�.
		}
	}
}

void CMacroManager::PM_SetSlotPage( DWORD dwSlotID, int nMacroEvent )
{
	cQuickSlotDlg* pSlot = ( cQuickSlotDlg* )WINDOWMGR->GetWindowForID( dwSlotID ) ;// ���� ������ ���´�.

	if(pSlot)																		// ���� ������ ��ȿ���� üũ�Ѵ�.
	{
		WORD curPage = pSlot->GetCurSheet() ;										// ���� �������� �޾ƿ´�.

		switch( nMacroEvent )														// ��ũ�� �̺�Ʈ�� Ȯ���Ѵ�.
		{
		case ME_INCREASE_MAIN_QUICKSLOT_NUM :										// �������� ������Ű�� �̺�Ʈ���,
		case ME_INCREASE_SUB_QUICKSLOT_NUM :
			{
				if( curPage >= MAX_SLOTPAGE-1 )										// �������� ������ ���������,
				{
					curPage = 0 ;													// ������ �������� �����Ѵ�.
				}
				else																// �׷��� �������,
				{
					curPage = curPage+1 ;											// �������� �ϳ� ������Ų��.
				}
			}
			break ;

		case ME_DECREASE_MAIN_QUICKSLOT_NUM :										// �������� ���ҽ�Ű�� �̺�Ʈ���,
		case ME_DECREASE_SUB_QUICKSLOT_NUM :
			{
				if( curPage <= 0 )													// �������� �ּ� ���������,
				{
					curPage = MAX_SLOTPAGE-1 ;										// ù �������� �����Ѵ�.
				}
				else																// �׷��� �������,
				{
					curPage = curPage-1 ;											// �������� �ϳ� ���ҽ�Ų��.
				}
			}
			break ;
		}

		pSlot->SelectPage( curPage );												// �������� �����Ѵ�.
	}
}

void CMacroManager::PM_SelectPartyMember( int nMacroEvent )
{
	CParty* pParty = PARTYMGR->GetParty() ;												// ��Ƽ ������ �޾ƿ´�.

	DWORD dwMasterID = pParty->GetMasterIdx() ;

	if( pParty )																		// ��Ƽ ������ ��ȿ���� üũ�Ѵ�.
	{
		DWORD dwMemberID = 0 ;															// ��� ���̵� �ʱ�ȭ �Ѵ�.

		if( dwMasterID == HEROID )
		{
			switch( nMacroEvent )															// ��ũ�� �̺�Ʈ�� Ȯ���Ѵ�.
			{
			case ME_SELECT_PARTYMEMBER_1 :	dwMemberID = pParty->GetMemberID(1) ;	break ;	// ù ��° ��� ���̵� �޾ƿ´�.
			case ME_SELECT_PARTYMEMBER_2 :	dwMemberID = pParty->GetMemberID(2) ;	break ;	// �� ��° ��� ���̵� �޾ƿ´�.
			case ME_SELECT_PARTYMEMBER_3 :	dwMemberID = pParty->GetMemberID(3) ;	break ;	// �� ��° ��� ���̵� �޾ƿ´�.
			case ME_SELECT_PARTYMEMBER_4 :	dwMemberID = pParty->GetMemberID(4) ;	break ;	// �� ��° ��� ���̵� �޾ƿ´�.
			case ME_SELECT_PARTYMEMBER_5 :	dwMemberID = pParty->GetMemberID(5) ;	break ;	// �ټ� ��° ��� ���̵� �޾ƿ´�.
			case ME_SELECT_PARTYMEMBER_6 :	dwMemberID = pParty->GetMemberID(6) ;	break ;	// ���� ��° ��� ���̵� �پƿ´�.
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

			switch( nMacroEvent )															// ��ũ�� �̺�Ʈ�� Ȯ���Ѵ�.
			{
			case ME_SELECT_PARTYMEMBER_1 :	dwMemberID = dwMemberOrder[0] ;	break ;	// ù ��° ��� ���̵� �޾ƿ´�.
			case ME_SELECT_PARTYMEMBER_2 :	dwMemberID = dwMemberOrder[1] ;	break ;	// �� ��° ��� ���̵� �޾ƿ´�.
			case ME_SELECT_PARTYMEMBER_3 :	dwMemberID = dwMemberOrder[2] ;	break ;	// �� ��° ��� ���̵� �޾ƿ´�.
			case ME_SELECT_PARTYMEMBER_4 :	dwMemberID = dwMemberOrder[3] ;	break ;	// �� ��° ��� ���̵� �޾ƿ´�.
			case ME_SELECT_PARTYMEMBER_5 :	dwMemberID = dwMemberOrder[4] ;	break ;	// �ټ� ��° ��� ���̵� �޾ƿ´�.
			case ME_SELECT_PARTYMEMBER_6 :	dwMemberID = dwMemberOrder[5] ;	break ;	// ���� ��° ��� ���̵� �پƿ´�.
			}
		}

		if( dwMemberID != 0 )															// ��� ���̵� �˻��Ѵ�.
		{
			GAMEIN->GetPartyDialog()->SetClickedMemberID(dwMemberID);					// �ش��ϴ� ���̵��� ����� �����Ѵ�.
		}
	}
}
void CMacroManager::PM_Toggle_AllInterface()
{
	// 071205 LUJ	NPC �̹��� â���� ��ü������ ��� â�� ����Ѵ�. ���� �ش� â�� �������� ��
	//				��� ������ �����ϸ� �ܵ����� ǥ�õǾ���� NPC â ���� �ٸ� â���� ���ļ� 
	//				ǥ�õǴ� ������ �߻��Ѵ�
	{
		cDialog* dialog = WINDOWMGR->GetWindowForID( NSI_SCRIPTDLG );
		ASSERT( dialog );

		if( dialog->IsActive() )
		{
			return;
		}
	}

	if( WINDOWMGR->IsOpendAllWindows() )											// ��������� �����ִ� ���¶��,
	{
		//WINDOWMGR->m_pActivedWindowList->RemoveAll() ;								// �����ִ� �����츦 ����ִ� ����Ʈ�� ��� ����.
		WINDOWMGR->CloseAllWindow() ;												// ��� �����츦 �ݴ´�.

		WINDOWMGR->SetOpendAllWindows( FALSE ) ;									// ��� �����찡 �����ִ� ���·� �����Ѵ�.
	}
	else																			// ��������� �����ִ� ���¶��,
	{
		WINDOWMGR->ShowAllActivedWindow() ;											// Ȱ��ȭ �� ��� ��������� �����ش�.

		WINDOWMGR->SetOpendAllWindows( TRUE ) ;										// ��� �����찡 �����ִ� ���·� �����Ѵ�.

		WINDOWMGR->ShowBaseWindow() ;												// �⺻������ ���̴� ��������� �����ش�.
	}
}

void CMacroManager::PM_Toggle_SkillDlg()
{
	cSkillTreeDlg* pDlg = GAMEIN->GetSkillTreeDlg() ;								// ��ųâ ������ �޾ƿ´�.

	if( pDlg )																		// ��ųâ ������ ��ȿ���� üũ�Ѵ�.
	{
		pDlg->SetActive( !pDlg->IsActive() );										// ��ųâ�� Ȱ��, ��Ȱ��ȭ �Ѵ�.
	}
}

void CMacroManager::PM_Toggle_FamilyDlg()
{
	// 070621 LYW --- MacroManager : �йи� ����� ���Ŀ� ���� �ʱ� ������ ���Ű ó�� ����.
	//if(HERO->GetFamilyIdx() != 0)													// ������� �йи� �ε����� �޾ƿ´�.
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
	CInventoryExDialog* pDlg = GAMEIN->GetInventoryDialog() ;						// �κ��丮 ���̾�α� ������ �޾ƿ´�.

	if( !pDlg ) return ;															// ���̾�α� ������ ��ȿ���� üũ�Ѵ�.
	
	pDlg->SetActive( !pDlg->IsActive() ) ;											// ���̾�α׸� Ȱ��, ��Ȱ��ȭ �Ѵ�.
}

void CMacroManager::PM_Toggle_ShowKeyTip()
{
	CKeySettingTipDlg* pDlg = GAMEIN->GetKeySettingTipDlg() ;						// Ű �� ���̾�α� ������ �޾ƿ´�.

	if( pDlg )																		// ���̾�α� ������ ��ȿ���� üũ�Ѵ�.
	{
		pDlg->SetMode( m_nMacroMode ) ;												// ��ũ�� ��带 �����Ѵ�.

		pDlg->SetActive(!pDlg->IsActive()) ;										// ���̾�α׸� Ȱ��, ��Ȱ��ȭ �Ѵ�.
	}
}

void CMacroManager::PM_Toggle_MiniMap()
{
	CMiniMapDlg* pDlg = GAMEIN->GetMiniMapDialog() ;								// �̴ϸ� ���̾�α� ������ �޾ƿ´�.

	if( pDlg )																		// ���̾�α� ������ ��ȿ���� üũ�Ѵ�.
	{
		pDlg->SetActive( !pDlg->IsActive() ) ;										// ���̾�α׸� Ȱ��, ��Ȱ��ȭ �Ѵ�.
	}
}

void CMacroManager::PM_Toggle_BigMap()
{
	CBigMapDlg* pDlg = GAMEIN->GetBigMapDialog() ;									// ��� ���̾�α� ������ �޾ƿ´�.

	if( pDlg )																		// ���̾�α� ������ ��ȿ���� üũ�Ѵ�.
	{
		pDlg->SetActive( !pDlg->IsActive() ) ;										// ���̾�α׸� Ȱ��, ��Ȱ��ȭ �Ѵ�.
	}
}

void CMacroManager::PM_Toggle_OptionDlg()
{
	COptionDialog* pDlg = GAMEIN->GetOptionDialog() ;								// �ɼ� ���̾�α� ������ �޾ƿ´�.

	if( pDlg )																		// ���̾�α� ������ ��ȿ���� üũ�Ѵ�.
	{
		pDlg->SetActive( !pDlg->IsActive() ) ;										// ���̾�α׸� Ȱ��, ��Ȱ��ȭ �Ѵ�.
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
//	CQuestTotalDialog* pDlg = GAMEIN->GetQuestTotalDialog() ;						// ����Ʈ ���̾�α� ������ �޾ƿ´�.
//
//	if( pDlg )																		// ���̾�α� ������ ��ȿ���� üũ�Ѵ�.
//	{
//		pDlg->SetActive( !pDlg->IsActive() ) ;										// ���̾�α׸� Ȱ��, ��Ȱ��ȭ �Ѵ�.
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
	CQuestTotalDialog* pDlg = GAMEIN->GetQuestTotalDialog() ;						// ����Ʈ ���̾�α� ������ �޾ƿ´�.

	if( pDlg )																		// ���̾�α� ������ ��ȿ���� üũ�Ѵ�.
	{
		pDlg->SetActive( !pDlg->IsActive() ) ;										// ���̾�α׸� Ȱ��, ��Ȱ��ȭ �Ѵ�.

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

	if( pDlg )																		// ���̾�α� ������ ��ȿ���� üũ�Ѵ�.
	{
		pDlg->RefreshFamilyInfo() ;													// �йи� ������ ���ΰ�ħ �Ѵ�.
		pDlg->SetActive( !pDlg->IsActive() ) ;										// ���̾�α׸� Ȱ��, ��Ȱ��ȭ �Ѵ�.
	}
}

void CMacroManager::PM_Toggle_MatchingDlg()
{
	const DATE_MATCHING_INFO& dateInfo = HERO->GetDateMatchingInfo() ;				// ��Ī ������ �޾ƿ´�.

	if(!dateInfo.bIsValid)															// ��Ī ������ ��ȿ���� ������,
	{
		//CHATMGR->AddMsg(CTC_SYSMSG, "�ֹε������ �߱� �޾ƾ� ����Ͻ� �� �ֽ��ϴ�.") ;
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 1284 ) );
	}
	else																			// ��Ī ������ ��ȿ�ϴٸ�,
	{
		CDateMatchingDlg* pDlg = GAMEIN->GetDateMatchingDlg() ;						// ��Ī ���̾�α� ������ �޾ƿ´�.

		if( pDlg )																	// ���̾�α� ������ ��ȿ���� üũ�Ѵ�.
		{
			if( pDlg->IsActive() )													// ���̾�αװ� ���� �ִٸ�,
			{
				CDateMatchingChatDlg* pChatDlg = NULL ;								// ��Ī ä�� ���̾�α� ������ ���� ������ ������ �����Ѵ�.

				pChatDlg = GAMEIN->GetDateMatchingDlg()->GetChatingDlg() ;			// ��Ī ���̾�α� ������ �޾ƿ´�.

				if( pChatDlg )														// ���̾�α� ������ ��ȿ���� üũ�Ѵ�.
				{
					if( pChatDlg->IsOnChatMode() )									// ä�� ���̶��,
					{
						// notice end chatting.
						WINDOWMGR->MsgBox( MBI_END_CHATTING_CLOSE, MBT_YESNO, CHATMGR->GetChatMsg( 1304 ) );
					}
					else															// ä�� ���� �ƴ϶��,
					{
						pDlg->SetActive(FALSE) ;									// ���̾�α׸� ��Ȱ��ȭ �Ѵ�.
					}
				}
			}
			else																	// ä�� ���̾�α״� ��Ȱ��ȭ ���̶��,
			{
				pDlg->GetPartnerListDlg()->SettingControls() ;						// ��Ʈ�� ����Ʈ�� ����ѵ��� �����Ѵ�.
				//pDlg->GetPartnerListDlg()->UpdateInfo() ;
				pDlg->SetActive(TRUE) ;												// ä�� ���̾�α׸� Ȱ��ȭ �Ѵ�.
			}
		}
	}
}

void CMacroManager::PM_SetAutoTarget()
{
	cPtrList* pList = MOVEMGR->GetAutoTargetList() ;								// �ڵ� Ÿ�� ����Ʈ�� �޾ƿ´�.

	if( pList )																		// ����Ʈ ������ ��ȿ���� üũ�Ѵ�.
	{
		int nTargetCount = pList->GetCount() ;										// ����Ʈ�� �����ۼ��� ����� �޴´�.

		if( nTargetCount > 0 )														// ����Ʈ�� Ÿ�� ����� ��� �ִٸ�,
		{
			CObject* pObject = NULL ;												// ������Ʈ ������ ���� �����͸� �����Ѵ�.

			DWORD* pObjectID = NULL ;												// ������Ʈ ���̵� ���� �����͸� �����Ѵ�.

			PTRLISTPOS pos = NULL ;													// ����Ʈ���� ��ġ�� ��Ÿ���� �����͸� �����Ѵ�.

			if( m_nCurTargetIdx >= nTargetCount )									// Ÿ�� �ε����� ��� ������ ũ��,
			{
				m_nCurTargetIdx = 0 ;												// Ÿ�� �ε����� �� ó������ �����Ѵ�.
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
					pObjectID = (DWORD*)pList->GetAt(pos) ;								// ��ġ�� �ش��ϴ� ������Ʈ ���̵� �޾ƿ´�.

					if( pObjectID )														// ���̵� ������ ��ȿ���� üũ�Ѵ�.
					{
						pObject = OBJECTMGR->GetObject(*pObjectID);						// ������Ʈ ������ �޾ƿ´�.

						if( pObject )													// ������Ʈ ������ ��ȿ���� üũ�Ѵ�.
						{
							if( pObject->GetState() != eObjectState_Die )				// ������Ʈ�� ���� ���°� �ƴ϶��,
							{
								OBJECTMGR->SetSelectedObject(pObject) ;					// ������Ʈ�� �����Ѵ�.

								pObject->ShowObjectName( TRUE, RGB_HALF( 255, 141, 39 )); // �̸��� ���������� �����Ѵ�.
								
								if( nTargetCount > m_nCurTargetIdx )					// ���� Ÿ�� �ε������� Ÿ�� ����� ������,
								{
									++m_nCurTargetIdx ;									// Ÿ�� �ε����� ���� ��Ų��.
								}
								else													// �׷��� ������,
								{
									m_nCurTargetIdx = 0 ;								// Ÿ�� �ε����� 0���� �����Ѵ�.
								}

								m_dwTargetID = *pObjectID ;								// ���õ� Ÿ���� ���̵� �����Ѵ�.

								break ;
							}
							else
							{
								if( nTargetCount > m_nCurTargetIdx )					// ���� Ÿ�� �ε������� Ÿ�� ����� ������,
								{
									++m_nCurTargetIdx ;									// Ÿ�� �ε����� ���� ��Ų��.
								}
								else													// �׷��� ������,
								{
									m_nCurTargetIdx = 0 ;								// Ÿ�� �ε����� 0���� �����Ѵ�.
								}
							}
						}
						else
						{
							if( nTargetCount > m_nCurTargetIdx )					// ���� Ÿ�� �ε������� Ÿ�� ����� ������,
							{
								++m_nCurTargetIdx ;									// Ÿ�� �ε����� ���� ��Ų��.
							}
							else													// �׷��� ������,
							{
								m_nCurTargetIdx = 0 ;								// Ÿ�� �ε����� 0���� �����Ѵ�.
							}
						}
					}
					else
					{
						if( m_nCurTargetIdx >= nTargetCount )									// Ÿ�� �ε����� ��� ������ ũ��,
						{
							m_nCurTargetIdx = 0 ;												// Ÿ�� �ε����� �� ó������ �����Ѵ�.
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
	// ���� â�� ������ �ϴ� ������ ����� ��û�ؾ��Ѵ�.
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

	// ä�ù� ���̾�α�.
	cDialog* pDlg = NULL ;
	pDlg = WINDOWMGR->GetWindowForID(CRD_DLG) ;

	ASSERT(pDlg) ;

	if(!pDlg)
	{
		CHATROOMMGR->Throw_Error("Failed to receive crd!!", __FUNCTION__) ;
		return ;
	}

	pDlg->SetActive(!pDlg->IsActive()) ;

	// ������ â.
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

	// �ɼ� â.
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
	// ä�ù� ���������� üũ.
	if(!CHATROOMMGR->GetMyRoomOwnerIdx())
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1498)) ;
		return ;
	}



	// ä�ù� ���̾�α�.
	cDialog* pDlg = NULL ;
	pDlg = WINDOWMGR->GetWindowForID(CRD_DLG) ;

	ASSERT(pDlg) ;

	if(!pDlg)
	{
		CHATROOMMGR->Throw_Error("Failed to receive crd!!", __FUNCTION__) ;
		return ;
	}

	// ä�ù��� ��Ȱ��ȭ �Ǿ��ִٸ�, return ó���� �Ѵ�.
	if(!pDlg->IsActive()) return ;



	// ä�ù��� ����Ʈ�ڽ� �ޱ�.
	cEditBox* roomBox = NULL ;
	roomBox = CHATROOMMGR->GetChatRoomInputBox() ;

	ASSERT(roomBox) ;

	if(!roomBox)
	{
		CHATROOMMGR->Throw_Error("Failed to receive creb!!", __FUNCTION__) ;
		return ;
	}



	// �Ϲ� ä��â �ޱ�.
	CChattingDlg* pChatDlg = NULL ;
	pChatDlg = GAMEIN->GetChattingDlg() ;

	ASSERT(pChatDlg) ;

	if(!pChatDlg)
	{
		CHATROOMMGR->Throw_Error("Failed to receive creb!!", __FUNCTION__) ;
		return ;
	}

	// ä��â�� ����Ʈ �ڽ� �ޱ�.
	cEditBox* chatBox = NULL ;
	chatBox = pChatDlg->GetInputBox() ;

	ASSERT(pChatDlg) ;

	if(!pChatDlg)
	{
		CHATROOMMGR->Throw_Error("Failed to receive cteb!!", __FUNCTION__) ;
		return ;
	}



	// ä�ù�� ä��â�� ��Ŀ�� �̵� ó��.
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
	float Rate = 1.f;																// ȸ�� ��.

	if( g_UserInput.GetKeyboard()->GetKeyPressed(KEY_RSHIFT) )						// ������ ����Ʈ�� ���ȴٸ�,
	{
		Rate = 0.3f;																// ȸ�� ���� 0.3f�� �����Ѵ�.
	}

	switch( nMacroEvent )															// ���ڷ� ���� ��ũ�� �̺�Ʈ�� Ȯ���Ѵ�.
	{
		case MPE_CAMERAMOVE_ZOOMIN:													// ���� ��ũ�ζ��,
			{

#ifdef _GMTOOL_																		// GM���� �� ó���� �����Ѵ�.

				if( CAMERA->GetCameraMode() == eCM_EyeView )						// ī�޶� ��尡 eCM_EyeView�� ���ٸ�,
				{
					CAMERA->Fly( 0.8 * gTickTime*Rate*5 );							// �ɹ̶��� fly �Լ��� �����Ѵ�.				
				}
				else																// ī�޶� ��尡 eCM_EyeView�� ���� �ʴٸ�,

#endif																				// GM�� ó���� ���� �Ѵ�.

					CAMERA->ZoomDistance( 0, -0.8 * gTickTime*Rate, 0 );			// �־��� ������ ī�޶� �� �Ÿ��� �����Ѵ�.

			}
			break;

		case MPE_CAMERAMOVE_ZOOMOUT:												// �� �ƿ� ��ũ�ζ��,
			{

#ifdef _GMTOOL_																		// GM�� ó���� �����Ѵ�.

				if( CAMERA->GetCameraMode() == eCM_EyeView )						// ī�޶� ��尡 eCM_EyeView�� ���ٸ�,
				{
					CAMERA->Fly( -0.8 * gTickTime*Rate*5 );							// �־��� ������ ī�޶��� Fly �Լ��� �����Ѵ�.
				}
				else																// ī�޶� ��尡 eCM_EyeView�� ���� �ʴٸ�,

#endif																				// GMó���� ���� �Ѵ�.

					CAMERA->ZoomDistance( 0, 0.8 * gTickTime*Rate, 0 );				// �־��� ������ ī�޶� �� �Ÿ��� �����Ѵ�.
			}
			break;

		case MPE_CAMERAMOVE_UP:														// ī�޶� �� ��ũ�ζ��,
			CAMERA->MouseRotate( 0, 0.3 * gTickTime*Rate );							// ī�޶��� ���콺 ȸ�� ó���� �Ѵ�.
			break;

		case MPE_CAMERAMOVE_DOWN:													// ī�޶� �ٿ� ��ũ�ζ��,
			CAMERA->MouseRotate( 0, -0.3 * gTickTime*Rate );						// ī�޶��� ���콺 ȸ�� ó���� �Ѵ�.
			break;

		case MPE_CAMERAMOVE_RIGHT:													// ī�޶� ����Ʈ ��ũ�ζ��,
			CAMERA->MouseRotate( -0.3 * gTickTime*Rate, 0 );						// ī�޶��� ���콺 ȸ�� ó���� �Ѵ�.
			break;

		case MPE_CAMERAMOVE_LEFT:													// ī�޶� ����Ʈ ��ũ�ζ��,
			CAMERA->MouseRotate( 0.3 * gTickTime*Rate, 0 );							// ī�޶��� ���콺 ȸ�� ó���� �Ѵ�.
			break;

	}
}

void CMacroManager::Init()
{

	sMACRO DefaultMacro[MM_COUNT][ME_COUNT] = {
		{
			{ MSK_NONE,		KEY_F1,	FALSE, FALSE },									// ���� ������ 1��Ű ���
			{ MSK_NONE,		KEY_F2,	FALSE, FALSE },									// ���� ������ 2��Ű ���
			{ MSK_NONE,		KEY_F3,	FALSE, FALSE },									// ���� ������ 3��Ű ���
			{ MSK_NONE,		KEY_F4,	FALSE, FALSE },									// ���� ������ 4��Ű ���
			{ MSK_NONE,		KEY_F5,	FALSE, FALSE },									// ���� ������ 5��Ű ���
			{ MSK_NONE,		KEY_F6,	FALSE, FALSE },									// ���� ������ 6��Ű ���
			{ MSK_NONE,		KEY_F7,	FALSE, FALSE },									// ���� ������ 7��Ű ���
			{ MSK_NONE,		KEY_F8,	FALSE, FALSE },									// ���� ������ 8��Ű ���
			{ MSK_NONE,		KEY_F9,	FALSE, FALSE },									// ���� ������ 9��Ű ���
			{ MSK_NONE,		KEY_F10,FALSE, FALSE },									// ���� ������10��Ű ���

			{ MSK_CTRL,		KEY_F1,	FALSE, FALSE },									// ���� ������ 1��Ű ��� 
			{ MSK_CTRL,		KEY_F2,	FALSE, FALSE },									// ���� ������ 2��Ű ��� 
			{ MSK_CTRL,		KEY_F3,	FALSE, FALSE },									// ���� ������ 3��Ű ��� 
			{ MSK_CTRL,		KEY_F4,	FALSE, FALSE },									// ���� ������ 4��Ű ��� 
			{ MSK_CTRL,		KEY_F5,	FALSE, FALSE },									// ���� ������ 5��Ű ��� 
			{ MSK_CTRL,		KEY_F6,	FALSE, FALSE },									// ���� ������ 6��Ű ��� 
			{ MSK_CTRL,		KEY_F7,	FALSE, FALSE },									// ���� ������ 7��Ű ��� 
			{ MSK_CTRL,		KEY_F8,	FALSE, FALSE },									// ���� ������ 8��Ű ��� 
			{ MSK_CTRL,		KEY_F9,	FALSE, FALSE },									// ���� ������ 9��Ű ��� 
			{ MSK_CTRL,		KEY_F10,FALSE, FALSE },									// ���� ������10��Ű ��� 

			{ MSK_ALT,		KEY_1,	FALSE, FALSE },									// ���� �������� 1������ ������.
			{ MSK_ALT,		KEY_2,	FALSE, FALSE },									// ���� �������� 2������ ������.
			{ MSK_ALT,		KEY_3,	FALSE, FALSE },									// ���� �������� 3������ ������.
			{ MSK_ALT,		KEY_4,	FALSE, FALSE },									// ���� �������� 4������ ������.
			{ MSK_ALT,		KEY_5,	FALSE, FALSE },									// ���� �������� 5������ ������.
			{ MSK_ALT,		KEY_6,	FALSE, FALSE },									// ���� �������� 6������ ������.
			{ MSK_ALT,		KEY_7,	FALSE, FALSE },									// ���� �������� 7������ ������.
			{ MSK_ALT,		KEY_8,	FALSE, FALSE },									// ���� �������� 8������ ������.

			{ MSK_ALT,		KEY_OEM_3, FALSE, FALSE },								// ���� ������ ��ȣ�� ���� ��Ų��.
			{ MSK_CTRL,		KEY_OEM_3, FALSE, FALSE },								// ���� ������ ��ȣ�� ���� ��Ų��.

			{ MSK_ALT | MSK_SHIFT,	KEY_OEM_3, FALSE, FALSE },						// ���� ������ ��ȣ�� ���� ��Ų��.
			{ MSK_CTRL | MSK_SHIFT,	KEY_OEM_3, FALSE, FALSE },						// ���� ������ ��ȣ�� ���� ��Ų��.

			{ MSK_ALT,		KEY_F1,	FALSE, FALSE },									// �� �ڽ��� �����Ѵ�.

			{ MSK_ALT,		KEY_F2,	FALSE, FALSE },									// ù ��° ��Ƽ ����� �����Ѵ�.
			{ MSK_ALT,		KEY_F3,	FALSE, FALSE },									// �� ��° ��Ƽ ����� �����Ѵ�.
			{ MSK_ALT,		KEY_F4,	FALSE, FALSE },									// �� ��° ��Ƽ ����� �����Ѵ�.
			{ MSK_ALT,		KEY_F5,	FALSE, FALSE },									// �� ��° ��Ƽ ����� �����Ѵ�.
			{ MSK_ALT,		KEY_F6,	FALSE, FALSE },									// �ټ� ��° ��Ƽ ����� �����Ѵ�.
			{ MSK_ALT,		KEY_F7,	FALSE, FALSE },									// ���� ��° ��Ƽ ����� �����Ѵ�.

			{ MSK_NONE,		KEY_TAB,	FALSE, FALSE },								// ���� �����̿� �ִ� Ÿ���� �����Ѵ�.

			{ MSK_ALT,		KEY_F12,	FALSE, FALSE },								// ��ũ���� ĸ���Ѵ�.

			{ MSK_NONE,		KEY_ESCAPE,	FALSE, FALSE },								// ���� �޽��� â�� ����.

			{ MSK_CTRL,		KEY_Z,	FALSE, FALSE },									// ��� �������̽��� ����Ѵ�.

			{ MSK_ALT,		KEY_S,	FALSE, FALSE },									// ��ųâ�� ����Ѵ�.

			{ MSK_ALT,		KEY_F,	FALSE, FALSE },									// �йи� â�� ����Ѵ�.

			{ MSK_ALT,		KEY_I,	FALSE, FALSE },									// �κ��丮�� ����Ѵ�.

			{ MSK_ALT,		KEY_H,	FALSE, FALSE },									// ������ ����Ѵ�.

			{ MSK_ALT,		KEY_M,	FALSE, FALSE },									// �̴ϸ��� ����Ѵ�.

			{ MSK_ALT,		KEY_W,	FALSE, FALSE },									// ����� ����Ѵ�.

			{ MSK_ALT,		KEY_O,	FALSE, FALSE },									// �ɼ� â�� ����Ѵ�.

			{ MSK_ALT,		KEY_G,	FALSE, FALSE },									// ��� â�� ����Ѵ�.

			{ MSK_ALT,		KEY_Q,	FALSE, FALSE },									// ����Ʈ â�� ����Ѵ�.

			{ MSK_ALT,		KEY_C,	FALSE, FALSE },									// ĳ���� ����â�� ����Ѵ�.

			{ MSK_ALT,		KEY_R,	FALSE, FALSE },									// �ȱ�/�ٱ� ��带 ��ȯ�Ѵ�.

			{ MSK_ALT,		KEY_INSERT,	FALSE, FALSE },								// ī�޶� �� ��带 ��ȯ�Ѵ�.

			{ MSK_ALT,		KEY_D,	FALSE, FALSE },									// ��Ī â�� ����Ѵ�.
			{ MSK_ALT,		KEY_B,	FALSE, FALSE },									// ģ�� â�� ����Ѵ�.
			{ MSK_ALT,		KEY_N,	FALSE, FALSE },									// ���� â�� ����Ѵ�.

			// 071025 LYW --- MacroManager : Add toggle key for help dialog.
			{ MSK_ALT,		KEY_T,	FALSE, FALSE },									// ���� â�� ����Ѵ�.
			{ MSK_CTRL,		KEY_CONTROL,	FALSE, FALSE },							// ���� �����ư

			// 080429 LYW --- MacroManager : Add toggle key for chatroom system.
			{ MSK_NONE,		KEY_HOME, FALSE, FALSE },								// ä�ù� ����Ʈâ�� ����Ѵ�.
			{ MSK_CTRL,		KEY_HOME, FALSE, FALSE },								// ä�ù��� ����Ѵ�.
			{ MSK_CTRL,		KEY_END,  TRUE, FALSE },								// ä�ù�� ä��â ������ ��Ŀ���� �̵���Ų��.

			{ MSK_ALT,		KEY_P,	FALSE, FALSE },									// ������â
		},
		{			
			{ MSK_NONE,		KEY_1,	FALSE, FALSE },									// ���� ������ 1��Ű ���
			{ MSK_NONE,		KEY_2,	FALSE, FALSE },									// ���� ������ 2��Ű ���
			{ MSK_NONE,		KEY_3,	FALSE, FALSE },									// ���� ������ 3��Ű ���
			{ MSK_NONE,		KEY_4,	FALSE, FALSE },									// ���� ������ 4��Ű ���
			{ MSK_NONE,		KEY_5,	FALSE, FALSE },									// ���� ������ 5��Ű ���
			{ MSK_NONE,		KEY_6,	FALSE, FALSE },									// ���� ������ 6��Ű ���
			{ MSK_NONE,		KEY_7,	FALSE, FALSE },									// ���� ������ 7��Ű ���
			{ MSK_NONE,		KEY_8,	FALSE, FALSE },									// ���� ������ 8��Ű ���
			{ MSK_NONE,		KEY_9,	FALSE, FALSE },									// ���� ������ 9��Ű ���
			{ MSK_NONE,		KEY_0, FALSE, FALSE },									// ���� ������10��Ű ���

			{ MSK_CTRL,		KEY_1,	FALSE, FALSE },									// ���� ������ 1��Ű ��� 
			{ MSK_CTRL,		KEY_2,	FALSE, FALSE },									// ���� ������ 2��Ű ��� 
			{ MSK_CTRL,		KEY_3,	FALSE, FALSE },									// ���� ������ 3��Ű ��� 
			{ MSK_CTRL,		KEY_4,	FALSE, FALSE },									// ���� ������ 4��Ű ��� 
			{ MSK_CTRL,		KEY_5,	FALSE, FALSE },									// ���� ������ 5��Ű ��� 
			{ MSK_CTRL,		KEY_6,	FALSE, FALSE },									// ���� ������ 6��Ű ��� 
			{ MSK_CTRL,		KEY_7,	FALSE, FALSE },									// ���� ������ 7��Ű ��� 
			{ MSK_CTRL,		KEY_8,	FALSE, FALSE },									// ���� ������ 8��Ű ��� 
			{ MSK_CTRL,		KEY_9,	FALSE, FALSE },									// ���� ������ 9��Ű ��� 
			{ MSK_CTRL,		KEY_0, FALSE, FALSE },									// ���� ������10��Ű ��� 

			{ MSK_ALT,		KEY_1,	FALSE, FALSE },									// ���� �������� 1������ ������.
			{ MSK_ALT,		KEY_2,	FALSE, FALSE },									// ���� �������� 2������ ������.
			{ MSK_ALT,		KEY_3,	FALSE, FALSE },									// ���� �������� 3������ ������.
			{ MSK_ALT,		KEY_4,	FALSE, FALSE },									// ���� �������� 4������ ������.
			{ MSK_ALT,		KEY_5,	FALSE, FALSE },									// ���� �������� 5������ ������.
			{ MSK_ALT,		KEY_6,	FALSE, FALSE },									// ���� �������� 6������ ������.
			{ MSK_ALT,		KEY_7,	FALSE, FALSE },									// ���� �������� 7������ ������.
			{ MSK_ALT,		KEY_8,	FALSE, FALSE },									// ���� �������� 8������ ������.

			{ MSK_NONE,		KEY_OEM_3, FALSE, FALSE },								// ���� ������ ��ȣ�� ���� ��Ų��.
			{ MSK_CTRL,		KEY_OEM_3, FALSE, FALSE },								// ���� ������ ��ȣ�� ���� ��Ų��.

			{ MSK_NONE | MSK_SHIFT,	KEY_OEM_3, FALSE, FALSE },						// ���� ������ ��ȣ�� ���� ��Ų��.
			{ MSK_CTRL | MSK_SHIFT,	KEY_OEM_3, FALSE, FALSE },						// ���� ������ ��ȣ�� ���� ��Ų��.

			{ MSK_NONE,		KEY_F1,	FALSE, FALSE },									// �� �ڽ��� �����Ѵ�.

			{ MSK_NONE,		KEY_F2,	FALSE, FALSE },									// ù ��° ��Ƽ ����� �����Ѵ�.
			{ MSK_NONE,		KEY_F3,	FALSE, FALSE },									// �� ��° ��Ƽ ����� �����Ѵ�.
			{ MSK_NONE,		KEY_F4,	FALSE, FALSE },									// �� ��° ��Ƽ ����� �����Ѵ�.
			{ MSK_NONE,		KEY_F5,	FALSE, FALSE },									// �� ��° ��Ƽ ����� �����Ѵ�.
			{ MSK_NONE,		KEY_F6,	FALSE, FALSE },									// �ټ� ��° ��Ƽ ����� �����Ѵ�.
			{ MSK_NONE,		KEY_F7,	FALSE, FALSE },									// ���� ��° ��Ƽ ����� �����Ѵ�.

			{ MSK_NONE,		KEY_TAB,	FALSE, FALSE },								// ���� �����̿� �ִ� Ÿ���� �����Ѵ�.

			{ MSK_NONE,		KEY_F12,	FALSE, FALSE },								// ��ũ���� ĸ���Ѵ�.

			{ MSK_NONE,		KEY_ESCAPE,	FALSE, FALSE },								// ���� �޽��� â�� ����.

			{ MSK_CTRL,		KEY_Z,	FALSE, FALSE },									// ��� �������̽��� ����Ѵ�.

			{ MSK_NONE,		KEY_S,	FALSE, FALSE },									// ��ųâ�� ����Ѵ�.

			{ MSK_NONE,		KEY_F,	FALSE, FALSE },									// �йи� â�� ����Ѵ�.

			{ MSK_NONE,		KEY_I,	FALSE, FALSE },									// �κ��丮�� ����Ѵ�.

			{ MSK_NONE,		KEY_H,	FALSE, FALSE },									// ������ ����Ѵ�.

			{ MSK_NONE,		KEY_M,	FALSE, FALSE },									// �̴ϸ��� ����Ѵ�.

			{ MSK_NONE,		KEY_W,	FALSE, FALSE },									// ����� ����Ѵ�.

			{ MSK_NONE,		KEY_O,	FALSE, FALSE },									// �ɼ� â�� ����Ѵ�.

			{ MSK_NONE,		KEY_G,	FALSE, FALSE },									// ��� â�� ����Ѵ�.

			{ MSK_NONE,		KEY_Q,	FALSE, FALSE },									// ����Ʈ â�� ����Ѵ�.

			{ MSK_NONE,		KEY_C,	FALSE, FALSE },									// ĳ���� ����â�� ����Ѵ�.

			{ MSK_NONE,		KEY_R,	FALSE, FALSE },									// �ȱ�/�ٱ� ��带 ��ȯ�Ѵ�.

			{ MSK_NONE,		KEY_INSERT,	FALSE, FALSE },								// ī�޶� �� ��带 ��ȯ�Ѵ�.

			{ MSK_NONE,		KEY_D,	FALSE, FALSE },									// ��Ī â�� ����Ѵ�.													
			{ MSK_NONE,		KEY_B,	FALSE, FALSE },									// ģ�� â�� ����Ѵ�.
			{ MSK_NONE,		KEY_N,	FALSE, FALSE },									// ���� â�� ����Ѵ�.

			// 071025 LYW --- MacroManager : Add toggle key for help dialog.
			{ MSK_NONE,		KEY_T,	FALSE, FALSE },									// ���� â�� ����Ѵ�.

			{ MSK_CTRL,		KEY_CONTROL,	FALSE, FALSE },							// ���� �����ư
			

			// 080429 LYW --- MacroManager : Add toggle key for chatroom system.
			{ MSK_NONE,		KEY_HOME, FALSE, FALSE },								// ä�ù� ����Ʈâ�� ����Ѵ�.
			{ MSK_CTRL,		KEY_HOME, FALSE, FALSE },								// ä�ù��� ����Ѵ�.
			{ MSK_CTRL,		KEY_END,  TRUE, FALSE },								// ä�ù�� ä��â ������ ��Ŀ���� �̵���Ų��.
			
			{ MSK_NONE,		KEY_P,	FALSE, FALSE },									// ������â
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
//	if( MAINGAME->GetCurStateNum() != eGAMESTATE_GAMEIN ) return;	//AOAuE�� ��UA����I��좥U confirm
//
//	//f12�� ������� �ð� ���̾�α׸� ���δ�
//	//ŰŬ���� �������� �۵��ϹǷ� Ű�ٿ�� �������ϰ� Ű���� ��ũ���� ����
//	if (keyInfo->GetKeyPressed(88) == 1) 
//	{
//		// 070124 LYW --- Delete this line.
//		//GAMEIN->GetScreenShotDlg()->SetActive( TRUE, gHeroID );  // ��ũ��ĸ�� �ð�dlg 
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
////========= ��ŷ�׽�Ʈ
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
//	// ���Ң�i���� ����AIAI A����������
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
//			pdlg->SetGuildName( "�׷���Ʈ�����̼�" );
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
//			strcpy( gi.GuildName, "��ٴ���" );
//			pdlg->AddGuildInfo( &gi );
//			gi.Ranking = eGTStanding_2ND;
//			gi.Position = 13;			
//			gi.GuildIdx = 2;
//			strcpy( gi.GuildName, "��ٴ���" );
//			pdlg->AddGuildInfo( &gi );
//			gi.Ranking = eGTStanding_3RD;
//			gi.Position = 22;
//			gi.GuildIdx = 3;
//			strcpy( gi.GuildName, "��ٴٻ�" );
//			pdlg->AddGuildInfo( &gi );
//			gi.Ranking = eGTStanding_1ST;
//			gi.Position = 31;
//			gi.GuildIdx = 4;
//			strcpy( gi.GuildName, "��ٴٻ�" );
//			pdlg->AddGuildInfo( &gi );*/
//			gi.Ranking = eGTStanding_8TH;
//			gi.Position = 0;
//			gi.GuildIdx = 2;
//			strcpy( gi.GuildName, "�󸶻縶��" );
//			pdlg->AddGuildInfo( &gi );
//			gi.Ranking = eGTStanding_32TH;
//			gi.Position = 1;
//			gi.GuildIdx = 4;
//			strcpy( gi.GuildName, "�󸶻縶��" );
//			pdlg->AddGuildInfo( &gi );
//			gi.Ranking = eGTStanding_16TH;
//			gi.Position = 2;
//			gi.GuildIdx = 6;
//			strcpy( gi.GuildName, "�󸶻縶��" );
//			pdlg->AddGuildInfo( &gi );
//			gi.Ranking = eGTStanding_32TH;
//			gi.Position = 3;
//			gi.GuildIdx = 8;
//			strcpy( gi.GuildName, "�󸶻縶��" );
//			pdlg->AddGuildInfo( &gi );
//			gi.Ranking = eGTStanding_32TH;
//			gi.Position = 4;			
//			gi.GuildIdx = 10;
//			strcpy( gi.GuildName, "�󸶻縶��" );
//			pdlg->AddGuildInfo( &gi );
//			gi.Ranking = eGTStanding_4TH;
//			gi.Position = 5;			
//			gi.GuildIdx = 12;
//			strcpy( gi.GuildName, "�󸶻縶��" );
//			pdlg->AddGuildInfo( &gi );
//			gi.Ranking = eGTStanding_16TH;
//			gi.Position = 6;			
//			gi.GuildIdx = 14;
//			strcpy( gi.GuildName, "�󸶻縶ĥ" );
//			pdlg->AddGuildInfo( &gi );
//			gi.Ranking = eGTStanding_32TH;
//			gi.Position = 7;			
//			gi.GuildIdx = 16;
//			strcpy( gi.GuildName, "�󸶻縶��" );
//			pdlg->AddGuildInfo( &gi );
//			pdlg->SetActive( !pdlg->IsActive() );
//			pdlg->RefreshGuildInfo();
//		}
//	}
//	if(keyInfo->GetKeyDown(KEY_J) && keyInfo->GetKeyPressed(KEY_MENU))
//	{
//		GTBATTLE_INFO bt1, bt2;
//		bt1.Group = 0;
//		sprintf( bt1.GuildName1, "�󸶴Ը���" );
//		sprintf( bt1.GuildName2, "�󸶴�¯" );
//		bt2.Group = 1;
//		sprintf( bt2.GuildName1, "ũ����" );
//		sprintf( bt2.GuildName2, "�ƿ�" );
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
//			GAMEIN->GetEventNotifyDialog()->SetTitle( "�÷���Ÿ��" );
//			GAMEIN->GetEventNotifyDialog()->SetContext( "����ġ2��" );
//			GAMEIN->GetEventNotifyDialog()->SetTitle( "�÷���Ÿ��" );
//			GAMEIN->GetEventNotifyDialog()->SetContext( "����ġ2��" );
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
//	//���稡AA����������
//// 	if(keyInfo->GetKeyDown(KEY_G) && keyInfo->GetKeyPressed(KEY_CONTROL))
//// 	{
//// 		GAMEIN->GetSkPointDlg()->SetActive( !GAMEIN->GetSkPointDlg()->IsActive() );
//// 	}
//	
////////////////////////////////////////////////////////////////////////////
////for test
//#ifdef _TESTCLIENT_
//	//C�͡��� A����������
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
//			sprintf(buf, "����%d", i);
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
//	//AEAo A����������
//#ifndef _TESTCLIENT_
//	if(keyInfo->GetKeyDown(KEY_N) && keyInfo->GetKeyPressed(KEY_CONTROL))
//	{
//		if( GAMEIN->GetReinforceResetDlg() )
//		{
//			GAMEIN->GetReinforceResetDlg()->SetActive( !GAMEIN->GetReinforceResetDlg()->IsActive() );
//		}
//	}
//	//Co��o��u A����������
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
//		if(pSkillInfo->GetSkillInfo()->Target == 1)			// ��o��e
//		{
//			CObject * targetObj = OBJECTMGR->GetSelectedObject();
//			if( targetObj )
//			{
//				Target.InitActionTarget(targetObj,NULL);
//			}/*		// A����UAI ������A �Ʃ���i ���Ң�i���� ����AIAI AO��A �ơ����� ����A��A�� ������A��A
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
//		else if(pSkillInfo->GetSkillInfo()->Target == 0)		// AU��A
//		{
//			Target.InitActionTarget(HERO,NULL);		// AU��AA�� A����UA����I
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

//---press��A CN�������� A��A��C������CI��C��I ��u��I ��좥U.
//void CMacroManager::KeyboardPressInput( CKeyboard* keyInfo )
//{
//	if( !g_bActiveApp || m_bPause )	return;
//
//	if( MAINGAME->GetCurStateNum() != eGAMESTATE_GAMEIN ) return;
//	if( CIMEWND->IsDocking() )
//	{
////		if( m_nMacroMode == MM_MACRO )	//A����A��i����AI��o��� ��E���Ƣ�U..CoAc��A..--;
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
////	if( !LoadMacro( "./INI/MacroUserSet.mcr" ) )	//��AAI��iA�� ��U��U��i?
////	{
////		SetDefaultMacro( MM_MACRO );
////		SetMacroMode( MM_MACRO );
////		SaveUserMacro();
////	}
//
////AO��AAU��a/////////////////////
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
////KES ��E����
////�����쩣���碬| ��c��eCI��A ��UAI��o��I������A ��Y��a��A SetActiveRecursive��I ��UAI��o��I������| ��Y����AO������a.
//void CMacroManager::PlayMacro( int nMacroEvent )
//{
//	int nMacroMode = OPTIONMGR->GetGameOption()->nMacroMode ;									// ä�� �켱�������, ����Ű �켱������� üũ�Ѵ�.
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
//	// desc_hseos_�ҽ��м����׽�Ʈ_01
//	// S �ҽ��м����׽�Ʈ �߰� added by hseos 2007.04.09
//	// �������� Ȯ�� â
//	// E �ҽ��м����׽�Ʈ �߰� added by hseos 2007.04.09
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
//			// 070621 LYW --- MacroManager : �йи� ����� ���Ŀ� ���� �ʱ� ������ ���Ű ó�� ����.
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
//				WINDOWMGR->MsgBox( MBI_IDENTIFICATION_ERROR, MBT_OK, "�ֹε������ �߱� �޾ƾ� ����Ͻ� �� �ֽ��ϴ�.") ;
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
//								WINDOWMGR->MsgBox( MBI_END_CHATTING_CLOSE, MBT_YESNO, "��ȭ�� ���� �Ͻðڽ��ϱ�?" );
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
////				if( HERO->GetKyungGongMode() == TRUE && HERO->GetKyungGongGrade() == 0)		// �Ʃ��Ʃ���i����AI 0AI��e �Ʃ��Ʃ� ��������
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
//				WORD wHH = 0, wMM = 0, wSS = 0;		//06.08.10 [PKH] �������Ͽ� �ð�ǥ��
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
//				//jpg��I ��?E?C��AO�����Ƣ�, �����������硤I��o��� ��N��UAO��e AA��U��U.
//
////jpg��I ��?E?CIAU( bmpA����i, jpg��I ��?E?CI��i, jpgAuAaCI��i, bmpAo��i��i --;;;;;; )
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
//				//����ð� ���̾�α� OFF 
//				//F12��ư�� ����� ��ũ������ ��´�
//				//macromanager.cpp 179���ο��� ����ð� ���̾�α� ON
//				GAMEIN->GetScreenShotDlg()->SetActive( FALSE, 0 ); // ��ũ��ĸ�� �ð�dlg 
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
//			{ MSK_ALT,		KEY_R,	FALSE, FALSE },				//	ME_TOGGLE_MOVEMODE			---AO��A
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
//		//CoAc ��c��eA��AI ��AA�ϡ�I A�ơ�I AO��A ������A....
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
//	//��A����AUA�� ��OAu E��AI( alt+tab, alt+f4 ) //altA�� ������a
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

	// 071204 LUJ	��ũ���� �Կ����� �������̽��� ��� ���ε� �̵��� �õ��� ���
	//				���� ���¿����� �̿� ������ �������̽��� �ݾƾ� �Ѵ�

	// �ŷ�â
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

	// ���� â��â
	// ��� â��â
	// ���� ����â
	// ��ȯ â
	// NPC ��ȭâ

	RemoveSavedWindow( ( cDialog* )GAMEIN->GetStorageDialog() );
	RemoveSavedWindow( ( cDialog* )GAMEIN->GetGuildWarehouseDlg() );
	RemoveSavedWindow( ( cDialog* )GAMEIN->GetExchangeDialog() );
	RemoveSavedWindow( ( cDialog* )GAMEIN->GetNpcScriptDialog() );
	RemoveSavedWindow( ( cDialog* )GAMEIN->GetStreetBuyStallDialog() );
	RemoveSavedWindow( ( cDialog* )GAMEIN->GetStreetStallDialog() );
	RemoveSavedWindow( ( cDialog* )GAMEIN->GetDealDialog() );
}

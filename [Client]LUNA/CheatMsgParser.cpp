#include "stdafx.h"
#include "ChatManager.h"
#include "CheatMsgParser.h"
#include "objectmanager.h"
#include "Movemanager.h"
#include "ItemManager.h"
#include "DebugDlg.h"
#include "GameIn.h"
#include "MAINGAME.h"
#include "CharacterCalcManager.h"
#include "PeaceWarModeManager.h"
#include "ObjectStateManager.h"
#include "./Audio/MHAudioManager.h"
#include "FilteringTable.h"
#ifdef _GMTOOL_
#include "GMToolManager.h"
#endif

#include "mhfile.h"

#include "InventoryExDialog.h"
#include "WindowIdEnum.h"
#include "./Interface/cWindowManager.h"
#include "cDivideBox.h"
#include "QuickManager.h"
#include "UserInfoManager.h"

#include "MapChange.h"
#include "EventMapInfo.h"
#include "GMNotifyManager.h"
#include "EventNotifyDialog.h"

//#include "GuildFieldWarDialog.h"
#include "PartyWar.h"
//#include "GuildCreateDialog.h"
#include "GuildUnion.h"

#include "cJackpotManager.h"
#include "MHMap.h"

#ifdef _GMTOOL_
#include "MHCamera.h"
#endif

#include "CharacterDialog.h"
#include "SeigeWarDialog.h"
#include "SiegeWarMgr.h"


#include "Party.h"

#include "NpcImageDlg.h"
#include "DealDialog.h"
#include "ShowdownManager.h"

// 070313 LYW --- CheatMsgParser : Include facial manager.
#include "FacialManager.h"
// 070314 LYW --- CheatMsgParser : Include party manager.
#include "PartyManager.h"
#include "cMsgBox.h"

// desc_hseos_몬스터미터_01
// S 몬스터미터 추가 added by hseos 2007.04.09
#include "../hseos/Monstermeter/SHMonstermeterDlg.h"
// E 몬스터미터 추가 added by hseos 2007.04.09

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.04.11
#include "../hseos/Debug/SHDebug.h"
// E 농장시스템 추가 added by hseos 2007.04.11

#include "cSkillTrainingDlg.h"

#include "FamilyCreateDialog.h"
#include "FamilyDialog.h"
#include "FamilyInviteDialog.h"
#include "FamilyNickNameDialog.h"

// 070426 LYW --- CheatMsgParser : Add for test debug.
//#ifdef _DEBUG
#include "cDebugString.h"
//#endif //_DEBUG

// 071129 LYW --- CheatMsgParser : 스킬트리 매니져 인크루드.
#include "cSkillTreeManager.h"


#include "GlobalEventFunc.h"
#include "GuildMarkDialog.h"

//---KES AUTONOTE 임시
#include "AutoNoteManager.h"
//-------------------

GLOBALTON(CCheatMsgParser)

#ifdef _CHEATENABLE_

BOOL CheatFunc(char* cheat)
{
	char code[256];
	char buf[256] = {0,};
	sscanf(cheat,"%s",code);
	strupr(code);

//	//KES CHEAT!!!
/*
	if(strcmp(code,"MAPNAME")==0)
	{
		if( strcmp( USERINFOMGR->GetUserID(), "mkteam2" ) != 0 
			&& strcmp( USERINFOMGR->GetUserID(), "MKTEAM2" ) != 0
			&& strcmp( USERINFOMGR->GetUserID(), "andyamy" ) != 0 )///
			return FALSE;

		//MAPTYPE mapnum;
		sscanf(cheat,"%s %s",code,buf);

		WORD mapnum = GetMapNumForName( buf );

		if( mapnum == 0 )
			return TRUE;

		MSG_NAME_DWORD2 msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_CHANGEMAP_SYN;
		msg.dwObjectID	= HEROID;
		msg.dwData1		= (DWORD)mapnum;
		msg.dwData2		= (DWORD)gChannelNum;
		msg.Name[0]		= 0;		
		NETWORK->Send( &msg, sizeof(msg) );

		return TRUE;
	}
//
*/
	
	// desc_hseos_치트키01
	// S 치트키 추가 added by hseos 2007.06.21
	// 070427 LYW --- CheatMsgparser : Modified cheat command.
#if defined(_GMTOOL_)
	//if( strcmp(code,"MONEYBEGETSMONEY") == 0 )
	if( strcmp(code,"AA") == 0 )
	{
		CHEATMGR->SetCheatEnable( TRUE );
		
		CHATMGR->AddMsg( CTC_CHEAT_1, "01001010010101110100101010101010010010011" );
		CHATMGR->AddMsg( CTC_CHEAT_1, "01001 [ Welcome To the Real World ] 11011" );
		CHATMGR->AddMsg( CTC_CHEAT_1, "11001101010100111010100001010010101111011" );
		
		return TRUE;
	}
	// 071106 LYW --- CheatMsgParser : Add a routine to check command to setting enable cheat mode.
	else if( strcmp( code, "ㅁㅁ") == 0 )
	{
		CHEATMGR->SetCheatEnable( TRUE );
		
		CHATMGR->AddMsg( CTC_CHEAT_1, "01001010010101110100101010101010010010011" );
		CHATMGR->AddMsg( CTC_CHEAT_1, "01001 [ Welcome To the Real World ] 11011" );
		CHATMGR->AddMsg( CTC_CHEAT_1, "11001101010100111010100001010010101111011" );
		
		return TRUE;
	}
#endif
	// E 치트키 추가 added by hseos 2007.06.21

	// desc_hseos_소스분석용테스트_01
	// S 소스분석용테스트 추가 added by hseos 2007.04.09
	/*
	if (stricmp(code, "HSEOS_TEST2") == 0)
	{
		GAMEIN->GetMonstermeterDlg()->SetActive( TRUE );
	}
	*/
	// E 소스분석용테스트 추가 added by hseos 2007.04.09

	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.04.12
	CSHDebug::ParseTextInput(cheat, code);
	// E 농장시스템 추가 added by hseos 2007.04.12

	if( !CHEATMGR->IsCheatEnable() ) return FALSE;


	if(strcmp(code,"===")==0)
	{
		CObject* pObj = OBJECTMGR->GetSelectedObject();
		if( pObj == NULL ) return TRUE;

		if( !( pObj->GetObjectKind() & eObjectKind_Monster ) &&
			( pObj->GetObjectKind() != eObjectKind_Npc ) )
			return TRUE;

		if( strlen(cheat) > 4 )
		{
			TESTMSGID msg;
			msg.Category = MP_CHEAT;
			msg.Protocol = MP_CHEAT_MOB_CHAT;
			msg.dwSenderID = pObj->GetID();
			SafeStrCpy( msg.Msg, cheat+4, MAX_CHAT_LENGTH+1 );
			NETWORK->Send(&msg,msg.GetMsgLength());
		}

		return TRUE;
	}

//---KES CHEAT PKEVENT
	else if( strcmp(code, "PKEVENT" )==0 )
	{
		int on;
		sscanf(cheat,"%s %d",code,&on);
		if( on != 0 && on !=1 ) return TRUE;

		MSG_BYTE msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_PKEVENT;
		msg.dwObjectID	= HEROID;
		msg.bData		= (BYTE)on;
		NETWORK->Send(&msg,sizeof(msg));

		return TRUE;
	}
//--------------
//---KES CHEAT CHANGESIZE
	else if( strcmp(code, "CHANGESIZE" )==0 )
	{
		CObject* pObject = OBJECTMGR->GetSelectedObject();
		
		if( pObject->GetObjectKind() == eObjectKind_Player )
		{
			DWORD size;
			sscanf(cheat, "%s %u",code,&size);
			if( size > 300 || size < 30 ) return TRUE;

			MSG_DWORD2 msg;
			msg.Category	= MP_CHEAT;
			msg.Protocol	= MP_CHEAT_CHANGESIZE;
			msg.dwObjectID	= HEROID;
			msg.dwData1		= pObject->GetID();
			msg.dwData2		= size;
			NETWORK->Send(&msg,sizeof(msg));
		}

		return TRUE;
	}
//-----------------
//---KES AUTONOTE 임시
/*
	else if( strcmp( code, "AUTOASK" )== 0 )
	{
		CObject* pObject = OBJECTMGR->GetSelectedObject();
		if( pObject )
		if( pObject->GetObjectKind() == eObjectKind_Player )
		{
			AUTONOTEMGR->AskToAutoUser( pObject->GetID(), 1 );
		}
		return TRUE;
	}	
	else if( strcmp( code, "AUTOANSWER" )== 0 )
	{
		int ans[4];
		sscanf( cheat, "%s %d %d %d %d", code, &ans[0], &ans[1], &ans[2], &ans[3] );

		AUTONOTEMGR->AnswerToQuestion( ans[0], ans[1], ans[2], ans[3] );
		return TRUE;
	}
*/
//--------------------
	else if( strcmp( code, "RETURNTO" )== 0 )
	{
		int point;
		sscanf(cheat,"%s %d",code,&point);

		MSG_WORD msg;
		msg.dwObjectID = HEROID;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_RETURN_SYN;
		msg.wData = (WORD)point;
		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}
	else if( strcmp( code, "RETURN" )== 0 )
	{
		WORD point;

		point = HERO->GetCharacterTotalInfo()->LoginPoint_Index;
		MSG_WORD msg;
		msg.dwObjectID = HEROID;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_RETURN_SYN;
		msg.wData = point;
		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}
	else if(strcmp(code,"CAMERAMODE")==0)
	{
		int onoff;
		sscanf(cheat,"%s %d",code,&onoff);

		if(onoff)
			HERO->GetEngineObject()->Show();
		else
			HERO->GetEngineObject()->HideWithScheduling();

		MSG_DWORD2 msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_SETVISIBLE;
		msg.dwObjectID = HEROID;
		msg.dwData1 = HEROID;
		msg.dwData2 = (DWORD)onoff;
		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}
	else if(strcmp(code, "JPMONEY")==0)
	{
		DWORD money;
		sscanf(cheat, "%s %d", code, &money);

		if( !money || money > 999999999 ) return FALSE;

		JACKPOTMGR->SetJPTotalMoneyTest(money);
//		MSG_DWORD msg;
//		msg.Category = MP_JACKPOT;
//		msg.Protocol = MP_JACKPOT_CHEAT_MAPTOTALMONEY;
//		msg.dwObjectID = HEROID;
//		msg.dwData = HEROID;
//		NETWORK->Send(&Msg,sizeof(msg));
		return TRUE;
	}

	else if(strcmp(code, "WANAJACKPOT")==0)
	{
//		char Name[MAX_NAME_LENGTH+1];
		DWORD	PrizeKind = 0;
		sscanf(cheat, "%s %d", code, &PrizeKind);

		if( 1 > PrizeKind || PrizeKind > 3 ) return FALSE;
		/*
		//SW050802
		//사용자를 위한.. 내부적으로 등수는 0, 1, 2 순
		*/
		PrizeKind--;

		MSG_DWORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_JACKPOT_GETPRIZE;
		msg.dwObjectID = HEROID;
		msg.dwData = PrizeKind;

		NETWORK->Send(&msg,sizeof(msg));

		return TRUE;
	}

	else if(strcmp(code, "ONOFFJACKPOT")==0)
	{
		DWORD	bOn = TRUE;
		sscanf(cheat, "%s %d", code, &bOn);

		if( !(bOn == TRUE || bOn == FALSE) ) return FALSE;

		MSG_DWORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_JACKPOT_ONOFF;
		msg.dwObjectID = HEROID;
		msg.dwData = bOn;

		NETWORK->Send(&msg,sizeof(msg));

		char* arS[2] = {"OFF", "ON"};

		CHATMGR->AddMsg( CTC_CHEAT_5, "JACKPOT ACTIVATION %s", arS[bOn] );

		return TRUE;
	}
	
	else if(strcmp(code, "JPMONEYPERMONSTER") == 0 )
	{
		DWORD	ChipsPerMon = 0;
		sscanf(cheat, "%s %d", code, &ChipsPerMon);
		
		if( ChipsPerMon < 0 || ChipsPerMon > 1000 ) return FALSE;

		MSG_DWORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_JACKPOT_MONEYPERMONSTER;
		msg.dwObjectID = HEROID;
		msg.dwData = ChipsPerMon;

		NETWORK->Send(&msg,sizeof(msg));

		CHATMGR->AddMsg( CTC_CHEAT_5, "SET JP_CHIP AS %d", ChipsPerMon );

		return TRUE;
	}
	//SW050809 강PM님 지시
	else if(strcmp(code, "JACKPOTSETTING") == 0 )
	{
		DWORD PrizeKind = 0;
		DWORD PrizeProb = 0;
		DWORD PrizePercent = 0;

		sscanf(cheat, "%s %d %d %d", code, &PrizeKind, &PrizeProb, &PrizePercent);

		if( PrizeKind < 1 || PrizeKind > 3 ) return FALSE;
		if( PrizeProb < 0 || PrizeProb > 100000000 ) return FALSE;
		if( PrizePercent < 0 || PrizePercent > 100 ) return FALSE;

		MSG_DWORD3 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_JACKPOT_PROBABILITY;
		msg.dwObjectID = HEROID;
		msg.dwData1 = PrizeKind - 1;	// 내부적으로 1등은 0...
		msg.dwData2 = PrizeProb;
		msg.dwData3 = PrizePercent;

		NETWORK->Send(&msg,sizeof(msg));

		CHATMGR->AddMsg( CTC_CHEAT_5, "SET CHANGED...OK" );

		return TRUE;
	}
	// 강PM님 지시 프로토콜 줄이기
	else if(strcmp(code, "JACKPOTCONTROL") == 0 )
	{
		char Dum[128] = {0,};
		DWORD OderFlag = 0;

		sscanf(cheat, "%s %s %d", code, Dum, &OderFlag);
		strupr(Dum);

		MSG_DWORD2 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_JACKPOT_CONTROL;
		msg.dwObjectID = HEROID;

		enum {TIMELENGTH, };	//CommonHeader 로~

		if(strcmp(Dum, "TIME") == 0 )	//Agent에 세팅
		{
			if( OderFlag < 0 || OderFlag > 3600 )	// sec 단위
				return FALSE;

			msg.dwData1 = TIMELENGTH;
			msg.dwData2 = OderFlag * 1000;

		}
		else
		{
			return FALSE;
		}

		NETWORK->Send(&msg,sizeof(msg));

		CHATMGR->AddMsg( CTC_CHEAT_5, "DO AS CONTROL...OK" );

		return TRUE;
	}
	//SW051112 무쌍모드
	else if(strcmp(code, "MUSSANG")==0)
	{
		MSGBASE msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_MUSSANG_READY;
		msg.dwObjectID = HEROID;
		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}

	else if(strcmp(code,"MAP")==0)
	{
		int mapnum;
		sscanf(cheat,"%s %d",code,&mapnum);

		MSG_NAME_DWORD2 msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_CHANGEMAP_SYN;
		msg.dwObjectID	= HEROID;
		msg.dwData1		= (DWORD)mapnum;
		msg.dwData2		= (DWORD)gChannelNum;
		msg.Name[0]		= 0;		
		NETWORK->Send( &msg, sizeof(msg) );

		return TRUE;
	}
	else if(strcmp(code,"LIFE")==0)
	{		
		int life;
		sscanf(cheat,"%s %d",code,&life);

#ifdef _TESTCLIENT_
//		CHARCALCMGR->Set_Life(HERO,life);
#else
		MSG_WORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_LIFE_SYN;
		msg.dwObjectID = HEROID;
		msg.wData = (WORD)life;
		NETWORK->Send(&msg,sizeof(msg));
#endif

		return TRUE;
	}
	else if(strcmp(code,"RELOAD")==0)
	{
		MSGBASE msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_RELOADING;
		msg.dwObjectID = HEROID;
		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}
	else if(strcmp(code,"MP")==0)
	{		
		int mp;
		sscanf(cheat,"%s %d",code,&mp);

		MSG_WORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_MP_SYN;
		msg.dwObjectID = HEROID;
		msg.wData = (WORD)mp;
		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}
	else if(strcmp(code,"ITEM")==0)
	{
		int item;
		sscanf(cheat,"%s %d",code,&item);

		MSG_WORD2 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_ITEM_SYN;
		msg.dwObjectID = HEROID;
		msg.wData1 = (WORD)item;
		msg.wData2 = 1;
		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}
	else if(strcmp(code,"ITEMS")==0)
	{
		int item;
		int num;
		sscanf(cheat,"%s %d %d",code,&item, &num);

		MSG_WORD2 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_ITEM_SYN;
		msg.dwObjectID = HEROID;
		msg.wData1 = (WORD)item;
		msg.wData2 = (WORD)num;
		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}
	else if(strcmp(code, "DS") == 0)
	{
		CDebugDlg* pDbg = GAMEIN->GetDebugDlg();
		pDbg->SetActive(TRUE);
		return TRUE;
	}
	else if(strcmp(code, "DH") == 0)
	{
		CDebugDlg* pDbg = GAMEIN->GetDebugDlg();
		pDbg->SetActive(FALSE);
		return TRUE;
	}
	else if(strcmp(code, "MANA") == 0)
	{
		int mp;
		sscanf(cheat,"%s %d",code,&mp);

		MSG_WORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_MANA_SYN;
		msg.dwObjectID = HEROID;
		msg.wData = (WORD)mp;
		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;

	}
	/*
	else if(strcmp(code, "MUNPAFIELD_REQ") == 0)
	{
		DWORD munpaid;
		sscanf(cheat,"%s %d",code,&munpaid);

		MSG_DWORD msg;
		msg.Category = MP_BATTLE;
		msg.Protocol = MP_BATTLE_MUNPAFIELD_REQUEST_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData = munpaid;
		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}
	*/
	else if(strcmp(code, "SKILLLEVEL") == 0)
	{
		int skill = 0;
		int level = 0;
		
		sscanf(cheat,"%s %d %d",code,&skill,&level);

		MSG_WORD2 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_UPDATE_SKILL_SYN;
		msg.dwObjectID = HEROID;
		msg.wData1 = (WORD)skill;
		msg.wData2 = (WORD)level;
		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}
	else if(strcmp(code, "SKILLEXCUTE") == 0)
	{
		int skill = 0;
		sscanf(cheat,"%s %d",code,&skill);
		// 070129 LYW --- Cheat Key : Return true.
		return TRUE ;
	}
	else if(strcmp(code, "MONEY") == 0)
	{
		DWORD money;
		sscanf(cheat,"%s %d",code,&money);

		MSG_DWORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_MONEY_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData = money;
		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}
	else if(strcmp(code, "CLEARSTATE") == 0)
	{
		OBJECTSTATEMGR->EndObjectState(HERO, HERO->GetState());
		
		return TRUE;
	}
	else if(strcmp(code, "VOLUME") == 0)
	{
		float m,s,b;
		sscanf(cheat,"%s %f %f %f",code,&m,&s,&b);

		AUDIOMGR->SetMasterVolume(m);
		AUDIOMGR->SetSoundVolume(s);
		AUDIOMGR->SetBGMVolume(b);

		return TRUE;
	}
//murim-net test
	else if(strcmp(code, "MURIMNET") == 0)
	{
		MAINGAME->SetGameState( eGAMESTATE_MURIMNET );
		return TRUE;
	}
	else if(strcmp(code, "LEVELUP") == 0)
	{
		int val = 1;
		sscanf(cheat,"%s %d",code,&val);
		if(val <= HERO->GetLevel())
			val = HERO->GetLevel()+1;
		MSG_WORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_LEVELUP_SYN;
		msg.dwObjectID = HEROID;
		msg.wData = (WORD)val;
		NETWORK->Send(&msg,sizeof(msg));
		
		return TRUE;
	}
	// 070411 LYW --- CheaMsgParser : Add cheat to level down.
	else if(strcmp(code, "LEVELDOWN") == 0)
	{
		int val = 1;
		sscanf(cheat,"%s %d",code,&val);
		if(val <= HERO->GetLevel())
			val = HERO->GetLevel()-1;
		MSG_WORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_LEVELDOWN_SYN;
		msg.dwObjectID = HEROID;
		msg.wData = (WORD)val;
		NETWORK->Send(&msg,sizeof(msg));
		
		return TRUE;
	}

	else if(strcmp(code, "STR") == 0)
	{
		int val = 0;
		sscanf(cheat,"%s %d",code,&val);
		if(val == 0)
			return TRUE;
		if((val < 1) || (val > 9999))
			val = 1;
		MSG_WORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_STR_SYN;
		msg.dwObjectID = HEROID;
		msg.wData = (WORD)val;
		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}
	else if(strcmp(code, "DEX") == 0)
	{
		int val = 0;
		sscanf(cheat,"%s %d",code,&val);
		if(val == 0)
			return TRUE;
		if((val < 1) || (val > 9999))
			val = 1;
		MSG_WORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_DEX_SYN;
		msg.dwObjectID = HEROID;
		msg.wData = (WORD)val;
		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}
	else if(strcmp(code, "VIT") == 0)
	{
		int val = 0;
		sscanf(cheat,"%s %d",code,&val);
		if(val == 0)
			return TRUE;
		if((val < 1) || (val > 9999))
			val = 1;
		MSG_WORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_VIT_SYN;
		msg.dwObjectID = HEROID;
		msg.wData = (WORD)val;
		NETWORK->Send(&msg,sizeof(msg));

		return TRUE;
	}
	else if(strcmp(code, "WIS") == 0)
	{
		int val = 0;
		sscanf(cheat,"%s %d",code,&val);
		if(val == 0)
			return TRUE;
		if((val < 1) || (val > 9999))
			val = 1;
		MSG_WORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_WIS_SYN;
		msg.dwObjectID = HEROID;
		msg.wData = (WORD)val;
		NETWORK->Send(&msg,sizeof(msg));

		return TRUE;
	}
	else if(strcmp(code, "INT") == 0)
	{
		int val = 0;
		sscanf(cheat,"%s %d",code,&val);
		if(val == 0)
			return TRUE;
		if((val < 1) || (val > 9999))
			val = 1;
		MSG_WORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_INT_SYN;
		msg.dwObjectID = HEROID;
		msg.wData = (WORD)val;
		NETWORK->Send(&msg,sizeof(msg));

		return TRUE;
	}	else if( strcmp(code, "MOVETO") == 0 )
	{
		sscanf(cheat, "%s %s", code, buf);
		//buf-A³¸?AI¸i

		if( strncmp( HERO->GetObjectName(), buf, MAX_NAME_LENGTH+1 ) == 0  )	//AU±a AU½A
			return TRUE;

		MSG_NAME msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_MOVETOCHAR_SYN;
		msg.dwObjectID	= HEROID;
		SafeStrCpy( msg.Name, buf, MAX_NAME_LENGTH + 1 );
		NETWORK->Send(&msg, sizeof(msg));
		return TRUE;
	}
	else if( strcmp(code, "WHEREIS") == 0 )
	{
		sscanf(cheat,"%s %s", code, buf);

//		if( strncmp( HERO->GetObjectName(), buf, MAX_NAME_LENGTH+1 ) == 0  )	//AU±a AU½A
//			return TRUE;
		if( (FILTERTABLE->IsInvalidCharInclude((unsigned char*)buf)) == TRUE )
		{
			return TRUE;
		}
		MSG_NAME msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_WHEREIS_SYN;
		msg.dwObjectID	= HEROID;
		SafeStrCpy( msg.Name, buf, MAX_NAME_LENGTH+1 );
		
		NETWORK->Send(&msg,sizeof(msg));

		return TRUE;
	}
	else if( strcmp( code, "BAN") == 0 )
	{
		sscanf(cheat, "%s %s", code, buf);
		//buf-A³¸?AI¸i

		if( strncmp( HERO->GetObjectName(), buf, MAX_NAME_LENGTH+1 ) == 0  )	//AU±a AU½A
			return TRUE;
		if( (FILTERTABLE->IsInvalidCharInclude((unsigned char*)buf)) == TRUE )
			return TRUE;

		MSG_NAME msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_BANCHARACTER_SYN;
		msg.dwObjectID	= HEROID;
		SafeStrCpy( msg.Name, buf, MAX_NAME_LENGTH+1 );

		NETWORK->Send(&msg,sizeof(msg));
		
		return TRUE;
	}
	else if( strcmp( code, "BLOCK") == 0 )
	{
		sscanf(cheat, "%s %s", code, buf);
		if( strncmp( HERO->GetObjectName(), buf, MAX_NAME_LENGTH+1 ) == 0  )	//AU±a AU½A
			return TRUE;

		MSG_NAME_DWORD msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_BLOCKCHARACTER_SYN;
		msg.dwObjectID	= HEROID;
		msg.dwData		= 1;	//block
		SafeStrCpy( msg.Name, buf, MAX_NAME_LENGTH+1 );

		NETWORK->Send(&msg,sizeof(msg));

		return TRUE;
	}
	else if( strcmp( code, "NONBLOCK" ) == 0 )
	{
		sscanf(cheat, "%s %s", code, buf);
		if( strncmp( HERO->GetObjectName(), buf, MAX_NAME_LENGTH+1 ) == 0  )	//AU±a AU½A
			return TRUE;

		MSG_NAME_DWORD msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_BLOCKCHARACTER_SYN;
		msg.dwObjectID	= HEROID;
		msg.dwData		= 0;	//nonblock
		SafeStrCpy( msg.Name, buf, MAX_NAME_LENGTH+1 );

		NETWORK->Send(&msg,sizeof(msg));

		return TRUE;
	}
	else if( strcmp( code, "PKMODE" ) == 0 )
	{
		int nAllow = -1;
		sscanf(cheat, "%s %s %d", code, buf, &nAllow );
		if( nAllow != 0 && nAllow != 1 )	return TRUE;
		WORD wMapNum = GetMapNumForName( buf );
		if( wMapNum == 0 )	return TRUE;
		
		MSG_WORD2 msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_PKALLOW_SYN;
		msg.dwObjectID	= HEROID;
		msg.wData1		= wMapNum;
		msg.wData2		= (WORD)nAllow;

		NETWORK->Send(&msg,sizeof(msg));

		return TRUE;
	}	
	else if( strcmp( code, "KILLMONSTER" ) == 0 )
	{
		MSGBASE msg;
		msg.Category	= MP_CHEAT;
//		msg.Protocol	= MP_CHEAT_KILL_ALLMONSTER_SYN;
		msg.dwObjectID	= HEROID;
		
		NETWORK->Send(&msg,sizeof(msg));

		return TRUE;
	}
	else if(strcmp(code, "PARTYINFO") == 0)
	{
		DWORD PartyIdx;
#ifdef TAIWAN_LOCAL
		PartyIdx = HERO->GetPartyIdx();
#else
		sscanf(cheat,"%s %d",code,&PartyIdx);
#endif
		MSG_DWORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_PARTYINFO_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData = PartyIdx;
		NETWORK->Send(&msg,sizeof(msg));
		CHATMGR->AddMsg( CTC_SYSMSG, "Party Info Cheat");
		return TRUE;
	}
	else if( strcmp( code, "TIMESET" ) == 0 )
	{
		int nTime;
		sscanf(cheat, "%s %d", code, &nTime );

		MSG_INT msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_ADDTIME;
		msg.nData = nTime;
		NETWORK->Send( &msg, sizeof(msg) );
		return TRUE;
	}
//	else if( strcmp( code, "EVENT" ) == 0 )
//	{
//		int nAllow = -1;
//		int nCount = 30;
//		sscanf(cheat, "%s %d %d", code, &nAllow, &nCount);
//		if( nAllow == -1 ) return TRUE;
//		
//		if( nCount > 30 )
//			nCount = 30;
//
//		CHEATMGR->EventMonsterRegen(nAllow, nCount);
//	}
	else if( strcmp( code, "EVENTMAP" ) == 0 )
	{
		char name[MAX_NAME_LENGTH+1];
		int map;
		int channel;
		int team;
		sscanf( cheat,"%s %s %d %d %d", code, name, &map, &channel, &team );

		MSG_NAME_DWORD3 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_CHANGE_EVENTMAP_SYN;
		msg.dwObjectID = HEROID;
		SafeStrCpy( msg.Name, name, MAX_NAME_LENGTH+1 );
		msg.dwData1 = map;
		msg.dwData2 = channel;
		msg.dwData3 = team;
		
		NETWORK->Send( &msg, sizeof(msg) );
		return TRUE;
	}
	else if( strcmp( code, "EVENTSTART" ) == 0 )
	{
		int map;
		int channel;
		sscanf( cheat,"%s %d %d %d", code, &map, &channel );

		MSG_DWORD2 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_EVENT_START_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData1 = map;
		msg.dwData2 = channel;
		
		NETWORK->Send( &msg, sizeof(msg) );
		return TRUE;
	}
	else if( strcmp( code, "EVENTREADY" ) == 0 )
	{
		int map;
		int channel;
		sscanf( cheat,"%s %d %d %d", code, &map, &channel );

		MSG_DWORD2 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_EVENT_READY_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData1 = map;
		msg.dwData2 = channel;
		
		NETWORK->Send( &msg, sizeof(msg) );
		return TRUE;
	}

	else if( strcmp( code, "GETSHOPITEM" ) == 0 )
	{
		int item = 0;
		sscanf( cheat,"%s %d", code, &item );

		if( item < 55000 )
			return FALSE;

		MSG_WORD Msg;
		Msg.Category = MP_CHEAT;
		Msg.Protocol = MP_CHEAT_GETSHOPITEM;
		Msg.dwObjectID = HEROID;
		Msg.wData = (WORD)item;
		
		NETWORK->Send( &Msg, sizeof(Msg) );
		return TRUE;
	}
	else if( strcmp( code, "PARTYWARSUGGEST" ) == 0 )
	{
		PARTYWAR->SendPartyWarSuggest();
		// 070129 LYW --- Cheat Key : Return true.
		return TRUE ;
	}
	else if( strcmp( code, "PARTYWARLOCK" ) == 0 )
	{
		PARTYWAR->SendPartyWarLock( TRUE );
		// 070129 LYW --- Cheat Key : Return true.
		return TRUE ;
	}
	else if( strcmp( code, "PARTYWARDLG" ) == 0 )
	{
		PARTYWAR->TestPartyWarDialog();
		// 070129 LYW --- Cheat Key : Return true.
		return TRUE ;
	}
/*	else if( strcmp( code, "WJSWLR" ) == 0 )
	{
		int stage = 0;
		sscanf( cheat, "%s %d", code, &stage );

		MSG_BYTE Msg;
		Msg.Category = MP_CHEAT;
		Msg.Protocol = MP_CHEAT_STAGE_SYN;
		Msg.dwObjectID = HEROID;
		Msg.bData = (BYTE)stage;
		
		NETWORK->Send( &Msg, sizeof(Msg) );
		return TRUE;
	}
*/	else if( strcmp( code, "QHTMAHQZNPTMXM" ) == 0 )
	{
		MSGBASE Msg;
		Msg.Category = MP_CHEAT;
		Msg.Protocol = MP_CHEAT_BOSSMOB_SYN;
		Msg.dwObjectID = HEROID;
		NETWORK->Send( &Msg, sizeof(Msg) );
		CHATMGR->AddMsg( CTC_SYSMSG, "Boss Quest Cheat" );
		return TRUE;
		
	}
	else if( strcmp( code, "TOURNAMENTINFO" ) == 0 )
	{
		DWORD param = 0;
		sscanf( cheat, "%s %d", code, &param );

		MSG_DWORD msg;
		msg.Category = MP_GTOURNAMENT;
		msg.Protocol = MP_GTOURNAMENT_CHEAT;
		msg.dwObjectID = HEROID;
		msg.dwData = param;
		NETWORK->Send( &msg, sizeof(msg) );
		return TRUE;
	}
//	else if( strcmp( code, "SIEGEWAR" ) == 0 )
//	{
//		DWORD mapnum = 0;
//		switch( MAP->GetMapNum() )
//		{
//		case jangan:
//			mapnum = jangan_siege;
//			break;
//		case nakyang:
//			mapnum = nakyang_siege;
//			break;
//		case gaebong:
//			mapnum = gaebong_siege;
//			break;
//		case bukkyung:
//			mapnum = bukkyung_siege;
//			break;
//		case jangan_siege:
//		case nakyang_siege:
//		case gaebong_siege:
//		case bukkyung_siege:
//			mapnum = 0;
//			break;
//		default:
//			return FALSE;
//		}
//
//
//		DWORD param1 = 0;
//		sscanf( cheat, "%s %d", code, &param1 );
//
//		MSG_DWORD4 msg;
//		msg.Category = MP_SIEGEWAR;
//		msg.Protocol = MP_SIEGEWAR_CHEAT;
//		msg.dwObjectID = HEROID;
//		msg.dwData1 = param1;
//		msg.dwData2 = MAP->GetMapNum();
//		msg.dwData3 = mapnum;
//		msg.dwData4 = HERO->GetGuildIdx();
//		NETWORK->Send( &msg, sizeof(msg) );
//		return TRUE;
//	}
	else if( strcmp( code, "WNTHRTJD" ) == 0 )
	{
		int wAttr = 0;
		sscanf( cheat, "%s %d", code, &wAttr );

		MSG_WORD Msg;
		Msg.Category = MP_CHEAT;
		Msg.Protocol = MP_CHEAT_CHANGE_MAINATTR_SYN;
		Msg.dwObjectID = HEROID;
		Msg.wData = (WORD)wAttr;
		NETWORK->Send( &Msg, sizeof(Msg) );
		return TRUE;
	}
	else if( strcmp( code, "QNTHRTJD" ) == 0 )
	{
		int wAttr = 0;
		sscanf( cheat, "%s %d", code, &wAttr );
		
		MSG_WORD Msg;
		Msg.Category = MP_CHEAT;
		Msg.Protocol = MP_CHEAT_CHANGE_SUBATTR_SYN;
		Msg.dwObjectID = HEROID;
		Msg.wData = (WORD)wAttr;
		NETWORK->Send( &Msg, sizeof(Msg) );
		// 070129 LYW --- Cheat Key : Return true.
		return TRUE ;
	}
	else if( strcmp( code, "SWPROFITDLG" ) == 0 )
	{
		GAMEIN->GetSWProfitDlg()->ShowProfitDlg( 10, 10000 );
		// 070129 LYW --- Cheat Key : Return true.
		return TRUE ;
	}
	else if( strcmp( code, "SWPROFITINFO" ) == 0 )
	{
		SWPROFIT->SendProfitInfoSyn();
		// 070129 LYW --- Cheat Key : Return true.
		return TRUE ;
	}
	else if( strcmp( code, "SWPROTECTREG" ) == 0 )
	{
		GAMEIN->GetSWProtectRegDlg()->Test();
		// 070129 LYW --- Cheat Key : Return true.
		return TRUE ;
	}
	else if( strcmp( code, "SWSTARTTIMEDLG" ) == 0 )
	{
		GAMEIN->GetSWStartTimeDlg()->Test();
		// 070129 LYW --- Cheat Key : Return true.
		return TRUE ;
	}
	else if( strcmp( code, "UVP" ) == 0 )
	{
		int count = 0;
		sscanf( cheat, "%s %d", code, &count );

		if(count < 1)
			count = 1;

		MSGBASE msg;
		msg.Category = 0;

		for(int i = 0; i < count; i++)
			NETWORK->Send( &msg, sizeof(msg) );

		return TRUE;
	}
	else if( strcmp( code, "VP" ) == 0 )
	{
		int count = 0;
		sscanf( cheat, "%s %d", code, &count );

		if(count < 1)
			count = 1;

		MSGBASE msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = 255;

		for(int i = 0; i < count; i++)
			NETWORK->Send( &msg, sizeof(msg) );

		return TRUE;
	}
	else if( strcmp( code, "PARTY" ) == 0 )
	{
		char name[32];
		int option;

		sscanf( cheat, "%s %s %d", code, name, &option );

		CObject* pObject = OBJECTMGR->GetObjectByName( name );

		if( pObject == NULL )
		{
			return TRUE;
		}

		if( pObject->GetObjectKind() != eObjectKind_Player )
		{
			return TRUE;
		}

		MSG_DWORDBYTE msg;

		msg.Category = MP_PARTY;
		msg.Protocol = MP_PARTY_CREATE_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData = pObject->GetID();
		msg.bData = (BYTE)option;

		NETWORK->Send(&msg,sizeof(msg));

		return TRUE;
	}
	else if( strcmp( code, "NPC" ) == 0 )
	{
		int idx;
		int emotion;

		sscanf( cheat, "%s %d %d", code, &idx, &emotion );

		if( idx == 0 )
		{
			GAMEIN->GetNpcImageDlg()->SetActive( FALSE );
		}
		else
		{
			GAMEIN->GetNpcImageDlg()->SetNpc( ( WORD )idx );
			GAMEIN->GetNpcImageDlg()->SetEmotion( ( BYTE )emotion );
			GAMEIN->GetNpcImageDlg()->SetActive( TRUE );
		}
		return TRUE;
	}
	else if( strcmp( code, "HELMET" ) == 0 )
	{
		OBJECTEFFECTDESC desc(FindEffectNum("item_head_01.beff"));
		
		HERO->AddObjectEffect( HELMET_EFFECT, &desc, 1, HERO);
		// 070129 LYW --- Cheat Key : Return true.
		return TRUE ;
	}
	else if( strcmp( code, "SHIELD" ) == 0 )
	{
		OBJECTEFFECTDESC desc(FindEffectNum("item_shi_01.beff"));
		
		HERO->AddObjectEffect( SHIELD_EFFECT, &desc, 1, HERO);
		// 070129 LYW --- Cheat Key : Return true.
		return TRUE ;
	}
	else if( strcmp( code, "FACE" ) == 0 )
	{
		int emotion;

		sscanf( cheat, "%s %d", code, &emotion );

		HERO->GetEngineObject()->SetMaterialIndex( emotion );
		// 070129 LYW --- Cheat Key : Return true.
		return TRUE ;
	}
	else if( strcmp( code, "SHOP" ) == 0 )
	{
/*		int npc;

		sscanf( cheat, "%s %d", code, &npc );
		GAMEIN->GetDealDialog()->ShowDealer( npc );
		*/
		GAMEIN->GetDealDialog()->SetActive( TRUE );
		// 070129 LYW --- Cheat Key : Return true.
		return TRUE ;
	}
	else if( strcmp( code, "PVP" ) == 0 )
	{
		SHOWDOWNMGR->ApplyShowdown();
		// 070129 LYW --- Cheat Key : Return true.
		return TRUE ;
	}
	else if( strcmp( code, "MOTION" ) == 0 )
	{
		int motionNum = 0 ;
		sscanf( cheat, "%s %d", code, &motionNum ) ;

		HERO->GetEngineObject()->ChangeMotion(motionNum,FALSE);
		// 070129 LYW --- Cheat Key : Return true.
		return TRUE ;
	}
	// 070313 LYW --- CheatMsgParser : Add cheat key for change face.
	else if( strcmp( code, "FACE3" ) == 0 )
	{
		FACIALMGR->ChangeHeroFace(3) ;

		return TRUE ;
	}
	// 070313 LYW --- CheatMsgParser : Add cheat key for close eye.
	else if( strcmp( code, "CLOSEEYE" ) == 0 )
	{
		HERO->InitializeEye() ;

		return TRUE ;
	}
	// 070314 LYW --- CheatMsgParser : Add cheat key for party part.
	else if( strcmp( code, "PARTY_OPTION" ) == 0 )
	{
		GAMEIN->GetPartyDialog()->ShowOption();

		return TRUE ;
	}
	else if( strcmp( code, "PARTY_INVITE" ) == 0 )
	{
		if( HEROID != PARTYMGR->GetMasterID() )	return FALSE ;
			
		// partywar
		if( PARTYWAR->IsPartyWar() )
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(885));
		}

		CObject* pObject = OBJECTMGR->GetSelectedObject();
		if(!pObject)
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(18));
		}
		else 
		{
			if(pObject->GetObjectKind() == eObjectKind_Player)
				PARTYMGR->AddPartyMemberSyn(OBJECTMGR->GetSelectedObject()->GetID());
			else
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(18));
		}

		return TRUE ;
	}
	else if( strcmp( code, "PARTY_KICK" ) == 0 )
	{
		if( HEROID != PARTYMGR->GetMasterID() )	return FALSE ;

		// partywar
		if( PARTYWAR->IsPartyWar() )
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(885));
			return FALSE ;
		}

		DWORD DeleteID = GAMEIN->GetPartyDialog()->GetClickedMemberID();
		if(DeleteID == 0)
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(250));
			return FALSE ;
		}
		PARTYMGR->BanPartyMemberSyn(DeleteID);

		return TRUE ;
	}
	else if( strcmp( code, "PARTY_HANDOVER" ) == 0 )
	{
		if( HEROID != PARTYMGR->GetMasterID() )	return FALSE ;

		// partywar
		if( PARTYWAR->IsPartyWar() )
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(885));
			return FALSE ;
		}

		DWORD DeleteID = GAMEIN->GetPartyDialog()->GetClickedMemberID();
		if(DeleteID == 0)
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(250));
			return FALSE ;
		}
		PARTYMGR->BanPartyMemberSyn(DeleteID);

		return TRUE ;
	}
	else if( strcmp( code, "PARTY_DESTROY" ) == 0 )
	{
		printf("PARTY_DESTROY") ;
		if( HEROID != PARTYMGR->GetMasterID() )	return FALSE ;

		// partywar
		if( PARTYWAR->IsPartyWar() )
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(885));
			return FALSE ;
		}

		if(PARTYMGR->IsProcessing() == TRUE)
			return FALSE ;
		WINDOWMGR->MsgBox(MBI_PARTYBREAKUP, MBT_YESNO, CHATMGR->GetChatMsg(301));

		return TRUE ;
	}
	else if( strcmp( code, "PARTY_BATTLE" ) == 0 )
	{
		if( HEROID != PARTYMGR->GetMasterID() )	return FALSE ;

		PARTYWAR->SendPartyWarSuggest();

		return TRUE ;
	}
	else if( strcmp( code, "PARTY_EXIT" ) == 0 )
	{
		// partywar
		if( PARTYWAR->IsPartyWar() )
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(885));
			return FALSE ;
		}

		if( HEROID != PARTYMGR->GetMasterID() )
			WINDOWMGR->MsgBox( MBI_PARTY_SECEDE, MBT_YESNO, CHATMGR->GetChatMsg(60));	

		return TRUE ;
	}
	else if( strcmp( code, "PARTY_INFO" ) == 0 )
	{
		GAMEIN->GetPartyDialog()->ShowMember();

		return TRUE ;
	}
	else if( strcmp( code, "PARTY_CHANGEPICKUP" ) == 0 )
	{
		int option = 0 ;

		sscanf( cheat, "%s %d", code, &option );

		MSG_DWORD2 msg;
		msg.Category = MP_PARTY;
		msg.Protocol = MP_PARTY_CHANGE_PICKUPITEM_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData1 = option;
		msg.dwData2 = HERO->GetPartyIdx();
		NETWORK->Send(&msg, sizeof(msg));

		return TRUE ;
	}
	else if(strcmp(code, "FISHINGEXP") == 0)
	{
		DWORD dwExp;
		sscanf(cheat,"%s %d",code,&dwExp);

		MSG_DWORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_FISHING_FISHINGEXP;
		msg.dwObjectID = HEROID;
		msg.dwData = dwExp;
		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}
	else if(strcmp(code, "FISHPOINT") == 0)
	{
		DWORD dwPoint;
		sscanf(cheat,"%s %d",code,&dwPoint);

		MSG_DWORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_FISHING_FISHPOINT;
		msg.dwObjectID = HEROID;
		msg.dwData = dwPoint;
		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}




#ifdef _GMTOOL_
	// 080115 KTH -- PVP 데미지 비율
	else if( strcmp( code, "PVP_DAMAGE_SET" ) == 0 )
	{
		int DamagePercent = 100;
		sscanf( cheat, "%s %d", code, &DamagePercent );

		MSG_BYTE msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_PVP_DAMAGE_RATE_SYN;
		msg.dwObjectID	= HERO->GetID();
		msg.bData		= (BYTE)DamagePercent;

		NETWORK->Send( &msg, sizeof(msg) );

		return TRUE;
	}
#endif
	// 070415 LYW --- CheatMsgParser : Add cheat message for change stage.
	else if( strcmp( code, "CHANGE_STAGE" ) == 0 )
	{
		int stage = 0 ;

		sscanf( cheat, "%s %d", code, &stage ) ;

		BYTE byValueA = stage / 10 ;
		BYTE byValueB = stage % 10 ;

		MSG_BYTE2 msg ;
		msg.Category	= MP_QUEST ;
		msg.Protocol	= MP_QUEST_CHANGESTAGE_SYN ;
		msg.dwObjectID	= HERO->GetID() ;
		msg.bData1		= byValueA ;
		msg.bData2		= byValueB ;

		NETWORK->Send( &msg, sizeof(msg) );

		/*
		MSG_BYTE msg ;

		msg.Category	= MP_QUEST ;
		msg.Protocol	= MP_QUEST_CHANGESTAGE_SYN ;
		msg.dwObjectID	= HERO->GetID() ;
		msg.bData		= (BYTE)stage ;

		NETWORK->Send( &msg, sizeof(msg) );
		*/
		
		return TRUE ;
	}
	/*
	else if( strcmp( code, "CHANGE_CLASSINDEX" ) == 0 )
	{
		int classIdx = 0 ;
		sscanf( cheat, "%s %d", code, &classIdx ) ;

		MSG_BYTE msg ;

		msg.Category	= MP_QUEST ;
		msg.Protocol	= MP_QUEST_CHANGECLASSIDX_SYN ;
		msg.dwObjectID	= HERO->GetID() ;
		msg.bData		= (BYTE)classIdx ;

		NETWORK->Send( &msg, sizeof(msg) );

		return TRUE ;
	}
	*/

	// 071106 LYW --- CheatMsgParser : Add cheat key - Setting stats to default.
	else if	( (strcmp( code, "DFST" ) == 0)	|| (strcmp( code, "ㅇㄹㄴㅅ" ) == 0) )
	{
		MSGBASE Msg ;														// 메시지 구조체를 선언한다.

		Msg.Category	= MP_CHEAT ;										// 카테고리를 치트로 세팅한다.
		Msg.Protocol	= MP_CHEAT_DEFAULT_STATS_SYN ;						// 프로토콜을 기본 스탯 요청으로 세팅한다.
		Msg.dwObjectID	= HEROID ;											// HERO id를 세팅한다.

		NETWORK->Send( &Msg, sizeof(Msg) ) ;								// 메시지를 전송한다.

		return TRUE ;														// true return.
	}

	// 071106 LYW --- CheatMsgParser : Add cheat key - Setting skill to default.
	else if( (strcmp( code, "DFSK") == 0) || (strcmp( code, "ㅇㄹ나") == 0) )
	{
		MSGBASE Msg ;														// 메시지 구조체를 선언한다.

		Msg.Category	= MP_CHEAT ;										// 카테고리를 치트로 세팅한다.
		Msg.Protocol	= MP_CHEAT_DEFAULT_SKILL_SYN ;						// 프로토콜을 기본 스킬 요청으로 세팅한다.
		Msg.dwObjectID	= HEROID ;											// HERO id를 세팅한다.

		NETWORK->Send( &Msg, sizeof(Msg) ) ;								// 메시지를 전송한다.

		return TRUE ;														// true return.
	}

	else if( strcmp( code, "SKILLTRAINING" ) == 0 )
	{
		GAMEIN->GetSkillTrainingDlg()->OpenDialog();
		return TRUE;
	}
	else if( strcmp( code, "FAMILYCREATE" ) == 0 )
	{
		GAMEIN->GetFamilyCreateDlg()->SetActive( TRUE );
		return TRUE;
	}
	else if( strcmp( code, "FAMILYINFO" ) == 0 )
	{
		GAMEIN->GetFamilyDlg()->SetActive( TRUE );
		return TRUE;
	}
	else if( strcmp( code, "FAMILYINVITE" ) == 0 )
	{
		GAMEIN->GetFamilyInviteDlg()->SetActive( TRUE );
		return TRUE;
	}
	else if( strcmp( code, "FAMILYNICKNAME" ) == 0 )
	{
		GAMEIN->GetFamilyNickNameDlg()->SetActive( TRUE );
		return TRUE;
	}
	// 070426 LYW --- CheatMsgParser : Add for test debug.
//#ifdef _DEBUG
	else if( strcmp( code, "SD" ) == 0 )
	{
		int command = -2 ;
		int onoff   = -1 ;
		int Xpos = -1 ;
		int Ypos = -1 ;

		sscanf( cheat, "%s %d %d %d %d", code, &command, &onoff, &Xpos, &Ypos ) ;

		return StartDebug(command, onoff, Xpos, Ypos) ;
	}
	// 0712112 KTH -- ResetInventory
	else if( strcmp( code, "RESET_INVENTORY" ) == 0 )
	{
		MSGBASE msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_RESET_INVENTORY_SYN;
		msg.dwObjectID = HEROID;

		NETWORK->Send( &msg, sizeof(msg) );

		HERO->Set_HeroExtendedInvenCount(0);

		CInventoryExDialog* pDlg = NULL ;
		pDlg = GAMEIN->GetInventoryDialog() ;

		if( pDlg )
		{
			pDlg->ShowTab(2,FALSE) ;
			pDlg->ShowTab(3,FALSE) ;

			pDlg->SelectTab(0) ;
		}

		return TRUE;
	}
	else if( strcmp( code, "SD_OPTION" ) == 0 )
	{
		int index = 1 ;
		int option = 0 ;

		sscanf( cheat, "%s %d %d", code, &index, &option ) ;

		WINDOWMGR->GetDebugString()->SetOption(index, option) ;
		return TRUE;
	}
	else if( strcmp( code, "SP" ) == 0 )
	{
		DWORD point = 0;
		sscanf( cheat, "%s %d", code, &point ) ;	

		MSG_DWORD Msg;
		Msg.Category = MP_CHEAT;
		Msg.Protocol = MP_CHEAT_SKILLPOINT_SYN;
		Msg.dwObjectID = HEROID;
		Msg.dwData = point;
		NETWORK->Send( &Msg, sizeof(Msg) );
		return TRUE;
	}
	else if( strcmp( code, "GOD" ) == 0 )
	{
		DWORD god = 0;
		sscanf( cheat, "%s %d", code, &god ) ;	

		MSG_DWORD2 Msg;
		Msg.Category = MP_CHEAT;
		Msg.Protocol = MP_CHEAT_GOD_SYN;
		Msg.dwObjectID = HEROID;

		DWORD id = OBJECTMGR->GetSelectedObjectID();

		if( !id )
		{
			id = HEROID;
		}

		Msg.dwData1 = id;
		Msg.dwData2 = god;
		NETWORK->Send( &Msg, sizeof(Msg) );
		return TRUE;
	}
	else if( strcmp( code, "PETLEVEL" ) == 0 )
	{
		int val = 1;
		sscanf(cheat,"%s %d",code,&val);
		if(val <= HEROPET->GetLevel())
			val = HEROPET->GetLevel()+1;
		MSG_WORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_PET_LEVELUP_SYN;
		msg.dwObjectID = HEROID;
		msg.wData = (WORD)val;
		NETWORK->Send(&msg,sizeof(msg));
		
		return TRUE;
	}
	else if( strcmp( code, "FF" ) == 0 )
	{
		MSG_WORD msg1;
		msg1.Category = MP_CHEAT;
		msg1.Protocol = MP_CHEAT_LEVELUP_SYN;
		msg1.dwObjectID = HEROID;
		msg1.wData = 40;
		NETWORK->Send(&msg1,sizeof(msg1));

		MSG_DWORD Msg2;
		Msg2.Category = MP_CHEAT;
		Msg2.Protocol = MP_CHEAT_SKILLPOINT_SYN;
		Msg2.dwObjectID = HEROID;
		Msg2.dwData = 300;
		NETWORK->Send( &Msg2, sizeof(Msg2) );

		MSG_DWORD msg3;
		msg3.Category	= MP_CHEAT;
		msg3.Protocol	= MP_CHEAT_MONEY_SYN;
		msg3.dwObjectID	= HEROID;
		msg3.dwData		= 1000000;
		NETWORK->Send( &msg3, sizeof(msg3) );		

		DWORD itemidx[59] = {
11000001,	11000002,	11000003,	11000004,	11000005,	11000006,	11000007,	11000008,			//8	
11000094,	11000095,	11000096,	11000097,	11000098,	11000099,	11000100,				//7	
12000063,	12000064,	12000065,	12000066,	12000067,	12000068,	12000069,	12000070,	12000071,	12000072,	12000073,	12000074,
12000156,	12000157,	12000158,	12000159,	12000160,	12000161,	12000162,	12000163,	12000164,	12000165,	12000166,	12000167,
12000249,	12000250,	12000251,	12000252,	12000253,	12000254,	12000255,	12000256,	12000257,	12000258,	12000259,	12000260, // 36
12000280,	12000281,	12000282,	12000283,	12000284,	12000285,	12000286,	12000287	// 8			
		};

		for( int i = 0; i < 59; i++ )
		{
			if( itemidx[i] == 0 )
				continue;

			MSG_DWORD_WORD msg4;
			msg4.Category	= MP_CHEAT;
			msg4.Protocol	= MP_CHEAT_ITEM_SYN;
			msg4.dwObjectID	= HEROID;
			msg4.dwData		= itemidx[i];
			msg4.wData		= 1;
			NETWORK->Send(&msg4,sizeof(msg4));
		}

		DWORD skillidx[36] 
		= {
1100101,	1100201,	1100301,	1100401,	1100501,	1500101,	1300101,	1300201,	1301001,	1301101,	1301201,	1400101,
1101101,	1101201,	1100601,	1100701,	1500201,	1500301,	1300301,	1300401,	1300501,	1300601,	1400201,	1101301,
1100801,	1100901,	1101001,	1500401,	1300701,	1300801,	1300901,	1301001,	1301101,	1301201,	1400101,	1101401
		};


		for( int i = 0; i < 36; i++ )
		{
			if( skillidx[i] == 0 )
				continue;
			MSG_SKILL_UPDATE msg5;
			msg5.Category = MP_SKILLTREE;
			msg5.Protocol = MP_SKILLTREE_UPDATE_SYN;
			msg5.dwObjectID = HEROID;
			msg5.SkillBase.wSkillIdx = skillidx[i];
			msg5.SkillBase.Level = 1;

			NETWORK->Send( &msg5, sizeof( msg5 ) );
		}
		return TRUE;

	}
	else if( strcmp( code, "RR" ) == 0 )
	{
		MSG_WORD msg1;
		msg1.Category = MP_CHEAT;
		msg1.Protocol = MP_CHEAT_LEVELUP_SYN;
		msg1.dwObjectID = HEROID;
		msg1.wData = 40;
		NETWORK->Send(&msg1,sizeof(msg1));

		MSG_DWORD Msg2;
		Msg2.Category = MP_CHEAT;
		Msg2.Protocol = MP_CHEAT_SKILLPOINT_SYN;
		Msg2.dwObjectID = HEROID;
		Msg2.dwData = 300;
		NETWORK->Send( &Msg2, sizeof(Msg2) );

		MSG_DWORD msg3;
		msg3.Category	= MP_CHEAT;
		msg3.Protocol	= MP_CHEAT_MONEY_SYN;
		msg3.dwObjectID	= HEROID;
		msg3.dwData		= 1000000;
		NETWORK->Send( &msg3, sizeof(msg3) );		

		DWORD itemidx[52] = {
11000187,	11000188,	11000189,	11000190,	11000191,	11000192,	11000193,	11000194,				
11000218,	11000219,	11000220,	11000221,	11000222,	11000223,	11000224,	11000225,	//16			
12000032,	12000033,	12000034,	12000035,	12000036,	12000037,	12000038,	12000039,	12000040,	12000041,	12000042,	12000043,
12000125,	12000126,	12000127,	12000128,	12000129,	12000130,	12000131,	12000132,	12000133,	12000134,	12000135,	12000136,
12000218,	12000219,	12000220,	12000221,	12000222,	12000223,	12000224,	12000225,	12000226,	12000227,	12000228,	12000229
		};

		for( int i = 0; i < 52; i++ )
		{
			if( itemidx[i] == 0 )
				continue;

			MSG_DWORD_WORD msg4;
			msg4.Category	= MP_CHEAT;
			msg4.Protocol	= MP_CHEAT_ITEM_SYN;
			msg4.dwObjectID	= HEROID;
			msg4.dwData		= itemidx[i];
			msg4.wData		= 1;
			NETWORK->Send(&msg4,sizeof(msg4));
		}

		DWORD skillidx[27] 
		= {
2100101,	2100201,	2100301,	2100401,	2100501,	2500101,	2500201,	2500301,	2300101,
2100601,	2100701,	1400201,	2101001,	2101101,	2300201,	2300801,	2300701,	1400101,
2100801,	2100901,	2500301,	2500401,	2101201,	2300301,	2300401,	2300501,	2300601

		};


		for( int i = 0; i < 27; i++ )
		{
			if( skillidx[i] == 0 )
				continue;
			MSG_SKILL_UPDATE msg5;
			msg5.Category = MP_SKILLTREE;
			msg5.Protocol = MP_SKILLTREE_UPDATE_SYN;
			msg5.dwObjectID = HEROID;
			msg5.SkillBase.wSkillIdx = skillidx[i];
			msg5.SkillBase.Level = 1;

			NETWORK->Send( &msg5, sizeof( msg5 ) );
		}
		return TRUE;	}
	else if( strcmp( code, "MM" ) == 0 )
	{
		MSG_WORD msg1;
		msg1.Category = MP_CHEAT;
		msg1.Protocol = MP_CHEAT_LEVELUP_SYN;
		msg1.dwObjectID = HEROID;
		msg1.wData = 40;
		NETWORK->Send(&msg1,sizeof(msg1));

		MSG_DWORD Msg2;
		Msg2.Category = MP_CHEAT;
		Msg2.Protocol = MP_CHEAT_SKILLPOINT_SYN;
		Msg2.dwObjectID = HEROID;
		Msg2.dwData = 300;
		NETWORK->Send( &Msg2, sizeof(Msg2) );

		MSG_DWORD msg3;
		msg3.Category	= MP_CHEAT;
		msg3.Protocol	= MP_CHEAT_MONEY_SYN;
		msg3.dwObjectID	= HEROID;
		msg3.dwData		= 1000000;
		NETWORK->Send( &msg3, sizeof(msg3) );		

		DWORD itemidx[52] = {
11000249,	11000250,	11000251,	11000252,	11000253,	11000254,	11000255,	11000256,				
12000001,	12000002,	12000003,	12000004,	12000005,	12000006,	12000007,	12000008,	12000009,	12000010,	12000011,	12000012,
12000094,	12000095,	12000096,	12000097,	12000098,	12000099,	12000100,	12000101,	12000102,	12000103,	12000104,	12000105,
12000188,	12000189,	12000190,	12000191,	12000192,	12000193,	12000194,	12000195,	12000196,	12000197,	12000198,	12000199,
12000280,	12000281,	12000282,	12000283,	12000284,	12000285,	12000286,	12000287				
		};

		for( int i = 0; i < 59; i++ )
		{
			if( itemidx[i] == 0 )
				continue;

			MSG_DWORD_WORD msg4;
			msg4.Category	= MP_CHEAT;
			msg4.Protocol	= MP_CHEAT_ITEM_SYN;
			msg4.dwObjectID	= HEROID;
			msg4.dwData		= itemidx[i];
			msg4.wData		= 1;
			NETWORK->Send(&msg4,sizeof(msg4));
		}

		DWORD skillidx[36] 
		= {
3200101,	3200201,	3200301,	3200401,	3200501,	3200601,	3200701,	3500101,	3500201,	3500301,	3500401,	3300101,
3200801,	3200901,	3201001,	3500202,	3500501,	3500301,	3500601,	1300501,	1400401,	3201701,	3300201,	1400301,
3201101,	3201201,	3201301,	3201401,	3201501,	3201601,	3500102,	3300601,	3201801,	3300301,	3300401,	3300501
		};


		for( int i = 0; i < 36; i++ )
		{
			if( skillidx[i] == 0 )
				continue;
			MSG_SKILL_UPDATE msg5;
			msg5.Category = MP_SKILLTREE;
			msg5.Protocol = MP_SKILLTREE_UPDATE_SYN;
			msg5.dwObjectID = HEROID;
			msg5.SkillBase.wSkillIdx = skillidx[i];
			msg5.SkillBase.Level = 1;

			NETWORK->Send( &msg5, sizeof( msg5 ) );
		}
		return TRUE;	
	}
	else if( strcmp( code, "SS" ) == 0 )
	{
		if( HERO->GetCharacterTotalInfo()->Job[0] == 1 )
		{
		MSG_WORD msg1;
		msg1.Category = MP_CHEAT;
		msg1.Protocol = MP_CHEAT_LEVELUP_SYN;
		msg1.dwObjectID = HEROID;
		msg1.wData = 40;
		NETWORK->Send(&msg1,sizeof(msg1));

		MSG_DWORD Msg2;
		Msg2.Category = MP_CHEAT;
		Msg2.Protocol = MP_CHEAT_SKILLPOINT_SYN;
		Msg2.dwObjectID = HEROID;
		Msg2.dwData = 300;
		NETWORK->Send( &Msg2, sizeof(Msg2) );

		MSG_DWORD msg3;
		msg3.Category	= MP_CHEAT;
		msg3.Protocol	= MP_CHEAT_MONEY_SYN;
		msg3.dwObjectID	= HEROID;
		msg3.dwData		= 1000000;
		NETWORK->Send( &msg3, sizeof(msg3) );		

		DWORD itemidx[59] = {
11000001,	11000002,	11000003,	11000004,	11000005,	11000006,	11000007,	11000008,			//8	
11000094,	11000095,	11000096,	11000097,	11000098,	11000099,	11000100,				//7	
12000063,	12000064,	12000065,	12000066,	12000067,	12000068,	12000069,	12000070,	12000071,	12000072,	12000073,	12000074,
12000156,	12000157,	12000158,	12000159,	12000160,	12000161,	12000162,	12000163,	12000164,	12000165,	12000166,	12000167,
12000249,	12000250,	12000251,	12000252,	12000253,	12000254,	12000255,	12000256,	12000257,	12000258,	12000259,	12000260, // 36
12000280,	12000281,	12000282,	12000283,	12000284,	12000285,	12000286,	12000287	// 8			
		};

		for( int i = 0; i < 59; i++ )
		{
			if( itemidx[i] == 0 )
				continue;

			MSG_DWORD_WORD msg4;
			msg4.Category	= MP_CHEAT;
			msg4.Protocol	= MP_CHEAT_ITEM_SYN;
			msg4.dwObjectID	= HEROID;
			msg4.dwData		= itemidx[i];
			msg4.wData		= 1;
			NETWORK->Send(&msg4,sizeof(msg4));
		}

		DWORD skillidx[36] 
		= {
1100101,	1100201,	1100301,	1100401,	1100501,	1500101,	1300101,	1300201,	1301001,	1301101,	1301201,	1400101,
1101101,	1101201,	1100601,	1100701,	1500201,	1500301,	1300301,	1300401,	1300501,	1300601,	1400201,	1101301,
1100801,	1100901,	1101001,	1500401,	1300701,	1300801,	1300901,	1301001,	1301101,	1301201,	1400101,	1101401
		};


		for( int i = 0; i < 36; i++ )
		{
			if( skillidx[i] == 0 )
				continue;
			MSG_SKILL_UPDATE msg5;
			msg5.Category = MP_SKILLTREE;
			msg5.Protocol = MP_SKILLTREE_UPDATE_SYN;
			msg5.dwObjectID = HEROID;
			msg5.SkillBase.wSkillIdx = skillidx[i];
			msg5.SkillBase.Level = 1;

			NETWORK->Send( &msg5, sizeof( msg5 ) );
		}
		return TRUE;
		}
		else if( HERO->GetCharacterTotalInfo()->Job[0] == 2 )
		{
		MSG_WORD msg1;
		msg1.Category = MP_CHEAT;
		msg1.Protocol = MP_CHEAT_LEVELUP_SYN;
		msg1.dwObjectID = HEROID;
		msg1.wData = 40;
		NETWORK->Send(&msg1,sizeof(msg1));

		MSG_DWORD Msg2;
		Msg2.Category = MP_CHEAT;
		Msg2.Protocol = MP_CHEAT_SKILLPOINT_SYN;
		Msg2.dwObjectID = HEROID;
		Msg2.dwData = 300;
		NETWORK->Send( &Msg2, sizeof(Msg2) );

		MSG_DWORD msg3;
		msg3.Category	= MP_CHEAT;
		msg3.Protocol	= MP_CHEAT_MONEY_SYN;
		msg3.dwObjectID	= HEROID;
		msg3.dwData		= 1000000;
		NETWORK->Send( &msg3, sizeof(msg3) );		

		DWORD itemidx[52] = {
11000187,	11000188,	11000189,	11000190,	11000191,	11000192,	11000193,	11000194,				
11000218,	11000219,	11000220,	11000221,	11000222,	11000223,	11000224,	11000225,	//16			
12000032,	12000033,	12000034,	12000035,	12000036,	12000037,	12000038,	12000039,	12000040,	12000041,	12000042,	12000043,
12000125,	12000126,	12000127,	12000128,	12000129,	12000130,	12000131,	12000132,	12000133,	12000134,	12000135,	12000136,
12000218,	12000219,	12000220,	12000221,	12000222,	12000223,	12000224,	12000225,	12000226,	12000227,	12000228,	12000229
		};

		for( int i = 0; i < 52; i++ )
		{
			if( itemidx[i] == 0 )
				continue;

			MSG_DWORD_WORD msg4;
			msg4.Category	= MP_CHEAT;
			msg4.Protocol	= MP_CHEAT_ITEM_SYN;
			msg4.dwObjectID	= HEROID;
			msg4.dwData		= itemidx[i];
			msg4.wData		= 1;
			NETWORK->Send(&msg4,sizeof(msg4));
		}

		DWORD skillidx[27] 
		= {
2100101,	2100201,	2100301,	2100401,	2100501,	2500101,	2500201,	2500301,	2300101,
2100601,	2100701,	1400201,	2101001,	2101101,	2300201,	2300801,	2300701,	1400101,
2100801,	2100901,	2500301,	2500401,	2101201,	2300301,	2300401,	2300501,	2300601

		};


		for( int i = 0; i < 27; i++ )
		{
			if( skillidx[i] == 0 )
				continue;
			MSG_SKILL_UPDATE msg5;
			msg5.Category = MP_SKILLTREE;
			msg5.Protocol = MP_SKILLTREE_UPDATE_SYN;
			msg5.dwObjectID = HEROID;
			msg5.SkillBase.wSkillIdx = skillidx[i];
			msg5.SkillBase.Level = 1;

			NETWORK->Send( &msg5, sizeof( msg5 ) );
		}
		return TRUE;
		}
		else if( HERO->GetCharacterTotalInfo()->Job[0] == 3 ) 
		{
		MSG_WORD msg1;
		msg1.Category = MP_CHEAT;
		msg1.Protocol = MP_CHEAT_LEVELUP_SYN;
		msg1.dwObjectID = HEROID;
		msg1.wData = 40;
		NETWORK->Send(&msg1,sizeof(msg1));

		MSG_DWORD Msg2;
		Msg2.Category = MP_CHEAT;
		Msg2.Protocol = MP_CHEAT_SKILLPOINT_SYN;
		Msg2.dwObjectID = HEROID;
		Msg2.dwData = 300;
		NETWORK->Send( &Msg2, sizeof(Msg2) );

		MSG_DWORD msg3;
		msg3.Category	= MP_CHEAT;
		msg3.Protocol	= MP_CHEAT_MONEY_SYN;
		msg3.dwObjectID	= HEROID;
		msg3.dwData		= 1000000;
		NETWORK->Send( &msg3, sizeof(msg3) );		

		DWORD itemidx[52] = {
11000249,	11000250,	11000251,	11000252,	11000253,	11000254,	11000255,	11000256,				
12000001,	12000002,	12000003,	12000004,	12000005,	12000006,	12000007,	12000008,	12000009,	12000010,	12000011,	12000012,
12000094,	12000095,	12000096,	12000097,	12000098,	12000099,	12000100,	12000101,	12000102,	12000103,	12000104,	12000105,
12000188,	12000189,	12000190,	12000191,	12000192,	12000193,	12000194,	12000195,	12000196,	12000197,	12000198,	12000199,
12000280,	12000281,	12000282,	12000283,	12000284,	12000285,	12000286,	12000287				
		};

		for( int i = 0; i < 59; i++ )
		{
			if( itemidx[i] == 0 )
				continue;

			MSG_DWORD_WORD msg4;
			msg4.Category	= MP_CHEAT;
			msg4.Protocol	= MP_CHEAT_ITEM_SYN;
			msg4.dwObjectID	= HEROID;
			msg4.dwData		= itemidx[i];
			msg4.wData		= 1;
			NETWORK->Send(&msg4,sizeof(msg4));
		}

		DWORD skillidx[36] 
		= {
3200101,	3200201,	3200301,	3200401,	3200501,	3200601,	3200701,	3500101,	3500201,	3500301,	3500401,	3300101,
3200801,	3200901,	3201001,	3500202,	3500501,	3500301,	3500601,	1300501,	1400401,	3201701,	3300201,	1400301,
3201101,	3201201,	3201301,	3201401,	3201501,	3201601,	3500102,	3300601,	3201801,	3300301,	3300401,	3300501
		};


		for( int i = 0; i < 36; i++ )
		{
			if( skillidx[i] == 0 )
				continue;
			MSG_SKILL_UPDATE msg5;
			msg5.Category = MP_SKILLTREE;
			msg5.Protocol = MP_SKILLTREE_UPDATE_SYN;
			msg5.dwObjectID = HEROID;
			msg5.SkillBase.wSkillIdx = skillidx[i];
			msg5.SkillBase.Level = 1;

			NETWORK->Send( &msg5, sizeof( msg5 ) );
		}
		return TRUE;	
		}

	}
	///else if( ! strcmpi( code, "guildcreate" ) )
	//{
	//	cDialog* dialog = WINDOWMGR->GetWindowForID( GD_CREATEDLG );
	//	ASSERT( dialog );

	//	dialog->SetActive( TRUE );
	//}
	//else if( ! strcmpi( code, "guildLevelup" ) )
	//{
	//	cDialog* dialog = WINDOWMGR->GetWindowForID( GD_LEVELUPDLG );
	//	ASSERT( dialog );

	//	dialog->SetActive( TRUE );
	//}
	//else if( ! strcmpi( code, "guildmark" ) )
	//{
	//	cDialog* dialog = WINDOWMGR->GetWindowForID( GDM_MARKREGISTDLG );
	//	ASSERT( dialog );

	//	dialog->SetActive( TRUE );
	//}
	// 길드 정보를 초기화한다
	else if(strcmp(code, "MUNPACLEAR") == 0)
	{
		MSGBASE msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_MUNPADATECLR_SYN;
		msg.dwObjectID = HEROID;
		NETWORK->Send(&msg,sizeof(msg));
		HERO->SetGuildEntryDate("2004.01.01");
		return TRUE;
	}
	//SW060719 길드 포인트
	//else if(strcmp(code, "GUILDPOINT")==0)	//문포
	//{
	//	CHero* hero = OBJECTMGR->GetHero();
	//	ASSERT( hero );

	//	if( ! hero->GetGuildIdx() )
	//	{
	//		CHATMGR->AddMsg( CTC_SYSMSG, "길드에 소속되어 있지 않습니다" );
	//		return TRUE;
	//	}

	//	DWORD GuildPoint = 0;
	//	sscanf(cheat, "%s %d", code, &GuildPoint);

	//	MSG_DWORD3 msg;
	//	msg.Category	= MP_CHEAT;
	//	msg.Protocol	= MP_CHEAT_GUILDPOINT_SYN;
	//	msg.dwObjectID	= hero->GetID();
	//	msg.dwData1		= hero->GetID();
	//	msg.dwData2		= hero->GetGuildIdx();
	//	msg.dwData3		= GuildPoint;

	//	NETWORK->Send(&msg,sizeof(msg));
	//	return TRUE;
	//}
	else if( strcmp( code, "GUILDUNIONCLEAR" ) == 0 )
	{
		MSG_DWORD Msg;
		Msg.Category = MP_CHEAT;
		Msg.Protocol = MP_CHEAT_GUILDUNION_CLEAR;
		Msg.dwObjectID = HEROID;
		Msg.dwData = 0;
		NETWORK->Send( &Msg, sizeof(Msg) );
		return TRUE;
	}
	// guildfieldwar
	//else if( strcmp( code, "GUILDFIELDWAR0" ) == 0 )
	//{
	//	GAMEIN->GetGFWarDeclareDlg()->SetActive( TRUE );

	//	return TRUE;
	//}
	//else if( strcmp( code, "GUILDFIELDWAR1" ) == 0 )
	//{
	//	GUILDINFO temp;
	//	strcpy( temp.GuildName, "Test" );
	//	temp.MapNum = 17;
	//	temp.GuildLevel = 3;
	//	strcpy( temp.MasterName, "Tester닷" );
	//	GAMEIN->GetGFWarResultDlg()->ShowDeclare( &temp );

	//	return TRUE;
	//}
	//else if( strcmp( code, "GUILDFIELDWAR2" ) == 0 )
	//{
	//	GAMEIN->GetGFWarResultDlg()->ShowResult( 0 );

	//	return TRUE;
	//}
	//else if( strcmp( code, "GUILDFIELDWAR3" ) == 0 )
	//{
	//	GAMEIN->GetGFWarInfoDlg()->ShowSurrend();
	//	
	//	return TRUE;
	//}	
	//else if( strcmp( code, "GUILDFIELDWAR4" ) == 0 )
	//{
	//	GAMEIN->GetGFWarResultDlg()->ShowResult( 1 );
	//	
	//	return TRUE;
	//}
	//else if( strcmp( code, "GUILDFIELDWAR5" ) == 0 )
	//{
	//	GAMEIN->GetGFWarInfoDlg()->ShowSuggest();
	//	
	//	return TRUE;
	//}

	////문파사냥몬스터
	//else if(strcmp(code, "ANSAHQ")==0)	//문몹
	//{
	//	DWORD PlayerID = 0;
	//	DWORD GuildIdx = 0;
	//	CObject* TObj = CHEATMGR->GetCheatTargetObject();
	//	if(TObj)
	//	{
	//		if(TObj->GetObjectKind() != eObjectKind_Player && TObj->GetObjectKind() != eObjectKind_Npc)
	//		{
	//			CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : TargetKind is not a Player");
	//			return TRUE;
	//		}

	//		if(TObj->GetObjectKind() == eObjectKind_Player)
	//		{
	//			PlayerID = ((CPlayer*)TObj)->GetID();
	//			GuildIdx = ((CPlayer*)TObj)->GetGuildIdx();
	//		}
	//		else if(TObj->GetObjectKind() == eObjectKind_Npc)
	//		{
	//			PlayerID = HEROID;
	//			GuildIdx = HERO->GetGuildIdx();
	//		}

	//		if( 0 == GuildIdx )
	//		{
	//			CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : TargetPlayer is not in a guild");
	//			return TRUE;
	//		}
	//	}
	//	else
	//	{
	//		CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : NonTarget" );
	//		return TRUE;
	//	}

	//	DWORD dwGuildMonsterCount = 0;
	//	sscanf(cheat, "%s %d", code, &dwGuildMonsterCount);

	//	MSG_DWORD3 msg;
	//	msg.Category = MP_CHEAT;
	//	msg.Protocol = MP_CHEAT_GUILDHUNTED_MONSTERCOUNT_SYN;
	//	msg.dwObjectID = HEROID;
	//	msg.dwData1 = PlayerID;
	//	msg.dwData2 = GuildIdx;
	//	msg.dwData3 = dwGuildMonsterCount;

	//	NETWORK->Send(&msg,sizeof(msg));
	//	return TRUE;		
	//}
	//else if( ! strcmpi( code, "guildwarehouse" ) )
	//{
	//	MSGBASE message;

	//	message.Category	= MP_GUILD;
	//	message.Protocol	= MP_GUILD_OPEN_WAREHOUSE_SYN;
	//	message.dwObjectID	= HEROID;

	//	NETWORK->Send( &message, sizeof( message ) );
	//}
	else if( ! strcmpi( code, "unionmark" ) )
	{
		GAMEIN->GetGuildMarkDlg()->ShowGuildUnionMark();
	}
	//else if( strcmp( code, "CR_R" ) == 0 )
	//{
	//	int nPage = 0 ;
	//	sscanf( cheat, "%s %d", code, &nPage ) ;

	//	MSG_BYTE msg ;
	//	memset(&msg, 0, sizeof(MSG_BYTE)) ;

	//	msg.Category	= MP_CHATROOM ;
	//	msg.Protocol	= MP_CHATROOM_ROOM_SYN ;
	//	msg.dwObjectID	= HEROID ;

	//	msg.bData		= (BYTE)nPage ;

	//	NETWORK->Send(&msg, sizeof(MSG_BYTE)) ;

	//	return TRUE ;
	//}
	//else if( strcmp( code, "CR_L" ) == 0 )
	//{
	//	int nPage = 0 ;
	//	sscanf( cheat, "%s %d", code, &nPage ) ;

	//	MSG_BYTE msg ;
	//	memset(&msg, 0, sizeof(MSG_BYTE)) ;

	//	msg.Category	= MP_CHATROOM ;
	//	msg.Protocol	= MP_CHATROOM_LOBBY_SYN ;
	//	msg.dwObjectID	= HEROID ;

	//	msg.bData		= (BYTE)nPage ;

	//	NETWORK->Send(&msg, sizeof(MSG_BYTE)) ;

	//	return TRUE ;
	//}
	//else if( strcmp( code, "CR_CR" ) == 0 )
	//{
	//	MSG_CHATROOM_CREATE_ROOM msg ;
	//	memset(&msg, 0, sizeof(MSG_CHATROOM_CREATE_ROOM)) ;

	//	msg.Category	= MP_CHATROOM ;
	//	msg.Protocol	= MP_CHATROOM_CREATE_ROOM_SYN ;
	//	msg.dwObjectID	= HEROID ;

	//	msg.bySecretMode	= 1 ;
	//	msg.byRoomType		= 2 ;

	//	msg.byTotalGuestCount = 20 ;

	//	msg.wCode		= 1123 ;

	//	strcpy(msg.title, "채팅방 테스트 - ㅋㅋㅋ") ;

	//	NETWORK->Send(&msg, sizeof(MSG_CHATROOM_CREATE_ROOM)) ;

	//	return TRUE ;
	//}
	//else if( strcmp( code, "CR_JR" ) == 0 )
	//{
	//	MSG_CHATROOM_JOIN_ROOM msg ;
	//	memset(&msg, 0, sizeof(MSG_CHATROOM_JOIN_ROOM)) ;

	//	msg.Category	= MP_CHATROOM ;
	//	msg.Protocol	= MP_CHATROOM_JOIN_ROOM_SYN ;
	//	msg.dwObjectID	= HEROID ;

	//	msg.dwOwnerIdx	= HEROID ;
	//	msg.wCode		= 1111 ;

	//	NETWORK->Send(&msg, sizeof(MSG_CHATROOM_JOIN_ROOM)) ;

	//	return TRUE ;
	//}	
	//else if( strcmp( code, "CR_SOR" ) == 0 )
	//{
	//	//MSG_DWORD msg ;
	//	//memset(&msg, 0, sizeof(MSG_DWORD)) ;

	//	//msg.Category	= MP_CHATROOM ;
	//	//msg.Protocol	= MP_CHATROOM_OUT_ROOM_SYN ;
	//	//msg.dwObjectID	= HEROID ;

	//	//msg.dwData		= HEROID ;

	//	//NETWORK->Send(&msg, sizeof(MSG_DWORD)) ;

	//	return TRUE ;
	//}		
	//else if( strcmp( code, "CR_CO" ) == 0 )
	//{
	//	//MSG_CHATROOM_CREATEINFO msg ;
	//	//memset(&msg, 0, sizeof(MSG_CHATROOM_CREATEINFO)) ;

	//	//msg.Category	= MP_CHATROOM ;
	//	//msg.Protocol	= MP_CHATROOM_CHANGE_OPTION_SYN ;
	//	//msg.dwObjectID	= HEROID ;

	//	//msg.byClosed	= 0 ;
	//	//msg.byType		= 1 ;

	//	//msg.byTotalGuestCount = 15 ;

	//	//msg.wCode		= 3211 ;

	//	//strcpy(msg.szTitle, "ZZZZ - CHATTING ROOM") ;

	//	//NETWORK->Send(&msg, sizeof(MSG_CHATROOM_CREATEINFO)) ;

	//	return TRUE ;
	//}
	//else if( strcmp( code, "CR_NM" ) == 0 )
	//{
	//	//MSG_CHATROOM_NORMAL_MSG msg ;
	//	//memset(&msg, 0, sizeof(MSG_CHATROOM_NORMAL_MSG)) ;

	//	//msg.Category	= MP_CHATROOM ;
	//	//msg.Protocol	= MP_CHATROOM_CHATMSG_NORMAL_SYN ;
	//	//msg.dwObjectID	= HEROID ;

	//	//msg.dwOwnerIdx	= HEROID ;
	//	//strcpy(msg.Msg, "Send to normal message~!!") ;

	//	//NETWORK->Send(&msg, sizeof(MSG_CHATROOM_NORMAL_MSG)) ;

	//	return TRUE ;
	//}
	//else if( strcmp( code, "CR_WM" ) == 0 )
	//{
	//	//MSG_CHATROOM_WHISPER_MSG msg ;
	//	//memset(&msg, 0, sizeof(MSG_CHATROOM_WHISPER_MSG)) ;

	//	//msg.Category	= MP_CHATROOM ;
	//	//msg.Protocol	= MP_CHATROOM_CHATMSG_WHISPER_SYN ;
	//	//msg.dwObjectID	= HEROID ;

	//	//msg.dwOwnerIdx	= HEROID ;
	//	//strcpy(msg.Msg, "Send to whisper message~!!") ;

	//	//NETWORK->Send(&msg, sizeof(MSG_CHATROOM_WHISPER_MSG)) ;

	//	return TRUE ;
	//}
	else if( strcmp( code, "CR_TESTROOM" ) == 0 )
	{
		int nCount = 0 ;
		sscanf( cheat, "%s %d", code, &nCount ) ;

		MSG_WORD msg ;
		memset(&msg, 0, sizeof(MSG_WORD)) ;

		msg.Category	= MP_CHATROOM ;
		msg.Protocol	= MP_CHATROOM_CHEAT_CREATE_ROOM_SYN ;
		msg.dwObjectID	= HEROID ;

		msg.wData		= (WORD)nCount ;

		NETWORK->Send(&msg, sizeof(MSG_WORD)) ;

		return TRUE ;
	}
	else if( strcmp( code, "CR_REMOVEALL_ROOM" ) == 0 )
	{
		MSGBASE msg ;
		memset(&msg, 0, sizeof(MSGBASE)) ;

		msg.Category	= MP_CHATROOM ;
		msg.Protocol	= MP_CHATROOM_CHEAT_REMOVE_ALL_ROOM_SYN ;
		msg.dwObjectID	= HEROID ;

		NETWORK->Send(&msg, sizeof(MSGBASE)) ;

		return TRUE ;
	}
	else if( strcmp( code, "CR_TESTUSER" ) == 0 )
	{
		int nCount = 0 ;
		sscanf( cheat, "%s %d", code, &nCount ) ;

		MSG_WORD msg ;
		memset(&msg, 0, sizeof(MSG_WORD)) ;

		msg.Category	= MP_CHATROOM ;
		msg.Protocol	= MP_CHATROOM_CHEAT_CREATE_USER_SYN ;
		msg.dwObjectID	= HEROID ;

		msg.wData		= (WORD)nCount ;

		NETWORK->Send(&msg, sizeof(MSG_WORD)) ;

		return TRUE ;
	}
	else if( strcmp( code, "CR_REMOVEALL_ROOM" ) == 0 )
	{
		MSGBASE msg ;
		memset(&msg, 0, sizeof(MSGBASE)) ;

		msg.Category	= MP_CHATROOM ;
		msg.Protocol	= MP_CHATROOM_CHEAT_REMOVE_ALL_USER_SYN ;
		msg.dwObjectID	= HEROID ;

		NETWORK->Send(&msg, sizeof(MSGBASE)) ;

		return TRUE ;
	}
	else if( strcmp( code, "CR_ADDGUEST_MYROOM" ) == 0 )
	{
		int nCount = 0 ;
		sscanf( cheat, "%s %d", code, &nCount ) ;

		MSGBASE msg ;
		memset(&msg, 0, sizeof(MSGBASE)) ;

		msg.Category	= MP_CHATROOM ;
		msg.Protocol	= MP_CHATROOM_CHEAT_ADD_USER_TO_MY_ROOM_SYN ;
		msg.dwObjectID	= HEROID ;

		NETWORK->Send(&msg, sizeof(MSGBASE)) ;

		return TRUE ;
	}

	return FALSE;
}

// 070427 LYW --- CheatMsgParser : Add for test.
//#ifdef _DEBUG
BOOL StartDebug(int command, int onoff, int Xpos, int Ypos)
{
	if( command < -1 )
	{
		CHATMGR->AddMsg(CTC_SYSMSG, "You have to input first number more than -1.") ;
		CHATMGR->AddMsg(CTC_SYSMSG, "0:Active selected list , -1:DeActive selected list.") ;		

		return FALSE ;
	}

	if( onoff > 1 || onoff < 0 )
	{
		CHATMGR->AddMsg(CTC_SYSMSG, "You have to input second number '0' or '1'.") ;
		CHATMGR->AddMsg(CTC_SYSMSG, "0:Active selected list , 1:DeActive selected list.") ;		
		
		return FALSE ;
	}

	BOOL bOnOff = (BOOL)onoff ;

	switch(command)
	{
	case -1 : 
		{
			WINDOWMGR->GetDebugString()->SetActive(FALSE) ; 
			CHATMGR->AddMsg(CTC_SYSMSG, "DeActived debug string.") ;
		}
		break ;

	case  0 : 
		{
			WINDOWMGR->GetDebugString()->SetActive(TRUE) ;

			CHATMGR->AddMsg(CTC_SYSMSG, "DebugString is DeActive now.") ;
			CHATMGR->AddMsg(CTC_SYSMSG, "Actived debug string.") ;
			CHATMGR->AddMsg(CTC_SYSMSG, "Select list and on off.") ;
			CHATMGR->AddMsg(CTC_SYSMSG, "-1 : deactive debug string.") ;
			CHATMGR->AddMsg(CTC_SYSMSG, "0 : active debug string.") ;
			CHATMGR->AddMsg(CTC_SYSMSG, "1 : npc total info.") ;
			CHATMGR->AddMsg(CTC_SYSMSG, "2 : npc list info.") ;
			CHATMGR->AddMsg(CTC_SYSMSG, "3 : hero class info.") ;
			CHATMGR->AddMsg(CTC_SYSMSG, "4 : player class info.") ;
			CHATMGR->AddMsg(CTC_SYSMSG, "5 : hero's hp mp info.") ;
			CHATMGR->AddMsg(CTC_SYSMSG, "6 : hero's experience info.") ;
			CHATMGR->AddMsg(CTC_SYSMSG, "7 : item's info.") ;
			CHATMGR->AddMsg(CTC_SYSMSG, "EX : /sd 3 1 100 100 <- active print info of hero's class to 100 by 100 position.") ;
			CHATMGR->AddMsg(CTC_SYSMSG, "EX : /sd 3 0  <- de active print info.") ;
		}
		break ;

	case  1 :
		{
			WINDOWMGR->GetDebugString()->set_debug_string(e_print_npc_totalinfo, bOnOff ) ;

			if( Xpos > -1 && Ypos > -1)
			{
				VECTOR2 vPos = {(float)Xpos, (float)Ypos} ;
				WINDOWMGR->GetDebugString()->SetPos(e_print_npc_totalinfo, &vPos) ;
			}
		}
		break ;
	case  2 :
		{
			WINDOWMGR->GetDebugString()->set_debug_string(e_print_npc_listinfo, bOnOff ) ;

			if( Xpos > -1 && Ypos > -1)
			{
				VECTOR2 vPos = {(float)Xpos, (float)Ypos} ;
				WINDOWMGR->GetDebugString()->SetPos(e_print_npc_listinfo, &vPos) ;
			}
		}
		break ;

	case 3 :
		{
			WINDOWMGR->GetDebugString()->SetPlayer(((CPlayer*)HERO)) ;

			WINDOWMGR->GetDebugString()->set_debug_string(e_print_player_totalInfo, bOnOff ) ;

			if( Xpos > -1 && Ypos > -1)
			{
				VECTOR2 vPos = {(float)Xpos, (float)Ypos} ;
				WINDOWMGR->GetDebugString()->SetPos(e_print_player_totalInfo, &vPos) ;
			}
		}
		break ;

	case 4 :
		{
			WINDOWMGR->GetDebugString()->SetPlayer(((CPlayer*)OBJECTMGR->GetSelectedObject())) ;

			WINDOWMGR->GetDebugString()->set_debug_string(e_print_player_totalInfo, bOnOff ) ;

			if( Xpos > -1 && Ypos > -1)
			{
				VECTOR2 vPos = {(float)Xpos, (float)Ypos} ;
				WINDOWMGR->GetDebugString()->SetPos(e_print_player_totalInfo, &vPos) ;
			}
		}
		break ;

	case 5 :
		{
			// 캐릭터의 클래스 인덱스로 요청을 보내는 것으로 끝.

			if( Xpos > -1 && Ypos > -1)
			{
				VECTOR2 vPos = {(float)Xpos, (float)Ypos} ;
				WINDOWMGR->GetDebugString()->SetPos(e_print_hpmp_determination, &vPos) ;
			}

			if( !bOnOff )
			{
				WINDOWMGR->GetDebugString()->set_debug_string(e_print_hpmp_determination, bOnOff ) ;

				return TRUE ;
			}

			CHARACTER_TOTALINFO totalInfo ;
			memset(&totalInfo, 0, sizeof(CHARACTER_TOTALINFO)) ;
			((CPlayer*)HERO)->GetCharacterTotalInfo(&totalInfo) ;

			BYTE base_class = 0 ;
			BYTE race		= totalInfo.Race+1 ;
			BYTE job_grade	= totalInfo.JobGrade ;
			BYTE cur_class	= 0 ;

			switch( totalInfo.Job[job_grade-1] )
			{
			case 1 : base_class = 1 ; break ;
			case 2 : base_class = 2 ; break ;
			case 3 : base_class = 3 ; break ;
			}

			if( job_grade == 1 )
			{
				cur_class = 1 ;
			}
			else
			{
				base_class = totalInfo.Job[job_grade] ;
			}

			DWORD dwClassIndex = (base_class * 1000) + ( race * 100 ) + ( job_grade * 10 ) + cur_class ;

			MSG_DWORD msg ;

			msg.Category	= MP_CHEAT ;
			msg.Protocol	= MP_CHEAT_HPMP_DETERMINATION_SYN ;
			msg.dwObjectID	= HERO->GetID() ;
			msg.dwData		= dwClassIndex ;

			NETWORK->Send( &msg, sizeof(msg) );
		}
		break ;

	case 6 :
		{
			WINDOWMGR->GetDebugString()->SetPlayer(((CPlayer*)OBJECTMGR->GetSelectedObject())) ;

			WINDOWMGR->GetDebugString()->set_debug_string(e_print_palyer_experience, bOnOff ) ;

			if( Xpos > -1 && Ypos > -1)
			{
				VECTOR2 vPos = {(float)Xpos, (float)Ypos} ;
				WINDOWMGR->GetDebugString()->SetPos(e_print_palyer_experience, &vPos) ;
			}
		}
		break ;

	case 7 :
		{
			WINDOWMGR->GetDebugString()->set_debug_string(e_print_itemInfo, bOnOff ) ;

			if( Xpos > -1 && Ypos > -1)
			{
				VECTOR2 vPos = {(float)Xpos, (float)Ypos} ;
				WINDOWMGR->GetDebugString()->SetPos(e_print_itemInfo, &vPos) ;
			}
		}
		break ;

	case 8 :
		{
			WINDOWMGR->GetDebugString()->set_debug_string(e_print_dropitem, bOnOff ) ;

			if( Xpos > -1 && Ypos > -1)
			{
				VECTOR2 vPos = {(float)Xpos, (float)Ypos} ;
				WINDOWMGR->GetDebugString()->SetPos(e_print_dropitem, &vPos) ;
			}

			MSG_BYTE msg ;

			msg.Category = MP_CHEAT ;
			msg.Protocol = MP_CHEAT_DROPITEM_INFO_SYN ;
			msg.dwObjectID = HERO->GetID() ;
			msg.bData	   = bOnOff ;

			NETWORK->Send( &msg, sizeof(msg) );
		}
		break ;
	}
	return TRUE ;
}
//#endif //_DEBUG

#endif //_CHEATENABLE_


CCheatMsgParser::CCheatMsgParser()
{
	m_bCheatEnable = FALSE;

	m_pTargetObj = NULL;
#ifdef _TESTCLIENT_
	m_bCheatEnable = TRUE;
#endif

	m_wItemOptionKind		= 0;
	m_bReadyToOptionCheat	= FALSE;
	m_wTargetItemIdx		= 0;
	m_ItemAbsPos			= 0;
	m_OptionCount			= 0;
}

CCheatMsgParser::~CCheatMsgParser()
{

}

/*
void CCheatMsgParser::LoadItem()
{
	CMHFile file;

	if(!file.Init("./System/Resource/eventitem.txt", "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR)) return;

	WORD idx = 0;
	int num;
	while(1)
	{
		if(file.IsEOF()) break;

		num = file.GetInt(); // ¼±AA ¿E¼CAC °¹¼o
		m_itemCount[idx] = num;

		for( int i=0;i<num;++i)
			strcpy(m_eventitem[idx][i].strName, file.GetString());

		++idx;
	}

	m_bEvent = TRUE;
}
*/



void CCheatMsgParser::NetworkMsgParse(BYTE Category,BYTE Protocol,MSGBASE* pMsg)
{
	ASSERT(Category == MP_CHEAT);

	switch(Protocol) 
	{
#ifdef _CHEATENABLE_
	case MP_CHEAT_CHANGEMAP_ACK:				Cheat_ChangeMap_Ack( pMsg ) ;					break ;
	//case MP_CHEAT_CHANGEMAP_NACK:				Cheat_ChangeMap_Nack( pMsg ) ;					break ;
	case MP_CHEAT_WHEREIS_ACK:					Cheat_WhereIs_Ack( pMsg ) ;						break ;
	case MP_CHEAT_WHEREIS_MAPSERVER_ACK:		Cheat_WhereIs_MapServer_Ack( pMsg ) ;			break ;
	case MP_CHEAT_WHEREIS_NACK:					Cheat_WhereIs_Nack( pMsg ) ;					break ;
	case MP_CHEAT_BLOCKCHARACTER_ACK:			Cheat_BlockCharacter_Ack( pMsg ) ;				break ;
	case MP_CHEAT_BLOCKCHARACTER_NACK:			Cheat_BlockCharacter_Nack( pMsg ) ;				break ;
	case MP_CHEAT_BANCHARACTER_ACK:				Cheat_BanCharacter_Ack( pMsg ) ;				break ;
	//case MP_CHEAT_BANCHARACTER_NACK:			Cheat_BanCharacter_Nack( pMsg ) ;				break ;
	case MP_CHEAT_ITEM_ACK:						Cheat_Item_Ack( pMsg ) ;						break ;
	case MP_CHEAT_AGENTCHECK_ACK:				Cheat_AgentCheck_Ack( pMsg ) ;					break ;
#ifdef _DEBUG
	case MP_CHEAT_HPMP_DETERMINATION_ACK :		Cheat_HPMP_Determination_Ack( pMsg ) ;			break ;
	case MP_CHEAT_HPMP_DETERMINATION_NACK :		Cheat_HPMP_Determination_Nack( pMsg ) ;			break ;
	case MP_CHEAT_EXPERIENCE_NOTICE :			Cheat_Experience_Notice( pMsg ) ;				break ;
	case MP_CHEAT_DROPMONEY_INFO_ACK :			Cheat_DropMoney_Info_Ack( pMsg ) ;				break ;
	case MP_CHEAT_DROPITEM_INFO_ACK :			Cheat_DropItem_Info_Ack( pMsg ) ;				break ;
#endif // _DEBUG

#endif	//_CHEATENABLE_
	case MP_CHEAT_MOVE_ACK:						Cheat_Move_Ack( pMsg ) ;						break ;
	case MP_CHEAT_HIDE_ACK:						Cheat_Hide_Ack( pMsg ) ;						break ;
	case MP_CHEAT_LIFE_ACK:																		break ;		
	case MP_CHEAT_MP_ACK:																		break ;

//---KES CHEAT CHANGESIZE
	case MP_CHEAT_CHANGESIZE:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwData1 );
			if( !pPlayer ) return;

			float fSize = (float)pmsg->dwData2 / 100.f;
			if( fSize > 3.0f || fSize < .3f ) return;

			pPlayer->SetCharacterSize( fSize );
		}
		break;
//-----------------------
#ifdef _GMTOOL_
	case MP_CHEAT_GM_LOGIN_ACK:					Cheat_GM_Login_Ack( pMsg ) ;					break ;
	case MP_CHEAT_GM_LOGIN_NACK:				Cheat_GM_Login_Nack( pMsg ) ;					break ;
	case MP_CHEAT_PARTYINFO_ACK:				Cheat_PartyInfo_Ack( pMsg ) ;					break ;
	case MP_CHEAT_EVENTMAP_RESULT:				Cheat_EventMap_Result( pMsg ) ;					break ;
	case MP_CHEAT_EVENTNOTIFY_ON:				Cheat_EventNotify_On( pMsg ) ;					break ;
	case MP_CHEAT_EVENTNOTIFY_OFF:				Cheat_EventNotify_Off( pMsg ) ;					break ;
	case MP_CHEAT_NPCHIDE_ACK:					Cheat_NpcHide_Ack( pMsg ) ;						break ;
	case MP_CHEAT_NPCHIDE_NACK:																	break ;
	case MP_CHEAT_DEFAULT_STATS_ACK :			Cheat_Default_Stats_Ack( pMsg ) ;				break ;
	case MP_CHEAT_DEFAULT_STATS_NACK :			Cheat_Default_Stats_Nack( pMsg ) ;				break ;
	case MP_CHEAT_DEFAULT_SKILL_ACK :			Cheat_Default_Skill_Ack( pMsg ) ;				break ;
	case MP_CHEAT_DEFAULT_SKILL_NACK :			Cheat_Default_Skill_Nack( pMsg ) ;				break ;
	// 080115 KTH -- PVP 데미지 설정 결과
	case MP_CHEAT_PVP_DAMAGE_RATE_ACK :			Cheat_PvP_Damage_Rate( pMsg );					break ;
#endif // _GMTOOL_
	default:																					break ;
	}
}



// 071128 LYW --- CheatMsgParser : 
#ifdef _CHEATENABLE_
	void CCheatMsgParser::Cheat_ChangeMap_Ack( void* pMsg )
	{
		MSG_WORD* pmsg = (MSG_WORD*)pMsg;
		MAPTYPE mapnum = pmsg->wData;
		MAPCHANGE->SetGameInInitKind(eGameInInitKind_MapChange);
		MAINGAME->SetGameState(eGAMESTATE_MAPCHANGE,&mapnum,4);
	}

	//void CCheatMsgParser::Cheat_ChangeMap_Nack( void* pMsg ) 
	//{
	//	// Not Use.
	//}

	void CCheatMsgParser::Cheat_WhereIs_Ack( void* pMsg ) 
	{
		MSG_WORD* pmsg = (MSG_WORD*)pMsg;
		CHATMGR->AddMsg( CTC_TOGM, CHATMGR->GetChatMsg( 430 ) );
#ifdef _GMTOOL_
		GMTOOLMGR->DisplayWhereInfo( "SELECT", 0, 0, 0 );
#endif
	}

	void CCheatMsgParser::Cheat_WhereIs_MapServer_Ack( void* pMsg ) 
	{
		MSG_WHERE_INFO* pmsg = (MSG_WHERE_INFO*)pMsg;

		VECTOR3 vPos;
		pmsg->cpos.Decompress( &vPos );

		char* strMapName = GetMapName( pmsg->wMapNum );
		if( strMapName[0] == 0 )
		{
			CHATMGR->AddMsg( CTC_TOGM, CHATMGR->GetChatMsg( 430 ) );
#ifdef _GMTOOL_
			GMTOOLMGR->DisplayWhereInfo( "SELECT", 0, 0, 0 );	
#endif
		}
		else
		{
			CHATMGR->AddMsg( CTC_TOGM, CHATMGR->GetChatMsg( 431 ),
				strMapName, pmsg->bChannel,
				(int)(vPos.x/100.0f), (int)(vPos.z/100.0f) );
#ifdef _GMTOOL_
			GMTOOLMGR->DisplayWhereInfo( strMapName, pmsg->bChannel,
				(int)(vPos.x/100.0f), (int)(vPos.z/100.0f) );
#endif
		}
	}

	void CCheatMsgParser::Cheat_WhereIs_Nack( void* pMsg ) 
	{
		MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;

		switch( pmsg->bData )
		{
//			case 0:
//				break;
		case 1:
			CHATMGR->AddMsg( CTC_TOGM, CHATMGR->GetChatMsg( 432 ) );
			break;
		case 2:
			CHATMGR->AddMsg( CTC_TOGM, CHATMGR->GetChatMsg( 433 ) );
			break;
		default:
			CHATMGR->AddMsg( CTC_TOGM, CHATMGR->GetChatMsg( 434 ) );
			break;
		}
	}

	void CCheatMsgParser::Cheat_BlockCharacter_Ack( void* pMsg ) 
	{
		CHATMGR->AddMsg( CTC_TOGM, CHATMGR->GetChatMsg( 435 ) );
	}

	void CCheatMsgParser::Cheat_BlockCharacter_Nack( void* pMsg ) 
	{
		CHATMGR->AddMsg( CTC_TOGM, CHATMGR->GetChatMsg( 432 ) );
	}

	void CCheatMsgParser::Cheat_BanCharacter_Ack( void* pMsg ) 
	{
		CHATMGR->AddMsg( CTC_TOGM, CHATMGR->GetChatMsg( 436 ) );
	}

	/*void CCheatMsgParser::Cheat_BanCharacter_Nack( void* pMsg ) 
	{
	}*/

	void CCheatMsgParser::Cheat_Item_Ack( void* pMsg ) 
	{
		ITEMOBTAINARRAY * pmsg = (ITEMOBTAINARRAY *)pMsg;
		
			CItem* pItem = NULL;

			ITEMBASE* pItemBase ;

			for(int i = 0 ; i < pmsg->ItemNum ; ++i)
			{
				pItemBase = NULL ;
				pItemBase = pmsg->GetItem(i) ;

				if( !pItemBase ) continue ;

				if( pItem = GAMEIN->GetInventoryDialog()->GetItemForPos( pmsg->GetItem(i)->Position ) )
				{
					pItem->SetItemBaseInfo(*pItemBase);

				}
				else
				{
					pItem = ITEMMGR->MakeNewItem(pItemBase,"MP_CHEAT_ITEM_ACK");	

					if( !pItem ) continue ;

					ITEM_INFO* pInfo = NULL ;
					pInfo = ITEMMGR->GetItemInfo( pItem->GetItemIdx() ) ;

					if( !pInfo ) continue ;

					if( pInfo->wSeal == eITEM_TYPE_SEAL )
					{
						pItemBase->nSealed = eITEM_TYPE_SEAL ;
						pItemBase->nRemainSecond = pInfo->dwUseTime ;

						pItem->SetItemBaseInfo( *pItemBase ) ;
					}

					BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(pItem);
					if(!rt)
					{
						ASSERT(0);
					}
				}
			}

			cDialog* pDlg = WINDOWMGR->GetWindowForID( DBOX_DIVIDE_INV );
			if( pDlg )
			{
				((cDivideBox*)pDlg)->ExcuteDBFunc( 0 );	//Ae¼O¹oÆ° ´ⓒ¸￡±a
			}
			
			CHATMGR->AddMsg( CTC_GETITEM, CHATMGR->GetChatMsg( 101 ), pItem->GetItemInfo()->ItemName );
			
			ITEMMGR->ItemDropEffect( pItem->GetItemIdx() );

			QUICKMGR->RefreshQickItem();		
	}

	void CCheatMsgParser::Cheat_AgentCheck_Ack( void* pMsg ) 
	{
		CHATMGR->AddMsg( CTC_SYSMSG, "CHEAT_AGENT_ACK!" );
	}

#ifdef _DEBUG
	void CCheatMsgParser::Cheat_HPMP_Determination_Ack( void* pMsg ) 
	{
		MSG_BYTE2* pmsg = (MSG_BYTE2*)pMsg ;

		WINDOWMGR->GetDebugString()->SetHpMpPoint(pmsg->bData1, pmsg->bData2) ;

		CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwObjectID );

		WINDOWMGR->GetDebugString()->SetPlayer(pPlayer) ;

		WINDOWMGR->GetDebugString()->set_debug_string(e_print_hpmp_determination, 1 ) ;
	}

	void CCheatMsgParser::Cheat_HPMP_Determination_Nack( void* pMsg ) 
	{
		MSG_BYTE* pmsg = (MSG_BYTE*)pMsg ;

		if(!pmsg->bData)
		{
			CHATMGR->AddMsg( CTC_SYSMSG, "There is no same hpmp determination." );
		}
		else
		{
			CHATMGR->AddMsg( CTC_SYSMSG, "It's not same class index." );
		}
	}

	void CCheatMsgParser::Cheat_Experience_Notice( void* pMsg ) 
	{
		WINDOWMGR->GetDebugString()->SetExpInfo((MSG_EXP_RESULT*)pMsg) ;
	}

	void CCheatMsgParser::Cheat_DropMoney_Info_Ack( void* pMsg ) 
	{
		MSG_DROPMONEY_RESULT* pmsg = (MSG_DROPMONEY_RESULT*)pMsg ;
		WINDOWMGR->GetDebugString()->Set_DropMoney(pmsg) ;
	}

	void CCheatMsgParser::Cheat_DropItem_Info_Ack( void* pMsg ) 
	{
		MSG_DROPITEM_RESULT* pmsg = (MSG_DROPITEM_RESULT*)pMsg ;
		WINDOWMGR->GetDebugString()->Set_DropItem(pmsg) ;
	}
#endif // _DEBUG

#endif	//_CHEATENABLE_
	void CCheatMsgParser::Cheat_Move_Ack( void* pMsg )
	{
		MOVE_POS* pmsg = (MOVE_POS*)pMsg;
		VECTOR3 pos;			
		pmsg->cpos.Decompress(&pos);

		BASEMOVE_INFO info;
		info.bMoving = FALSE;
		info.KyungGongIdx = 0;
		info.MoveMode = eMoveMode_Run;
		info.CurPosition = pos;
		MOVEMGR->InitMove(HERO,&info);

	}

	void CCheatMsgParser::Cheat_Hide_Ack( void* pMsg ) 
	{
		MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
		CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwData1 );
		if( !pPlayer ) return;
		
		ASSERT(pPlayer->GetObjectKind() == eObjectKind_Player);
		
		if(pmsg->dwData2 == 0 )
		{
#ifdef _GMTOOL_
			if( MAINGAME->GetUserLevel() <= eUSERLEVEL_GM  && CAMERA->GetCameraMode() != eCM_EyeView )
				pPlayer->GetEngineObject()->SetAlpha( 1.0f );
			else
#endif
				pPlayer->GetEngineObject()->Show();
			
			pPlayer->GetCharacterTotalInfo()->bVisible = TRUE;				
		}
		else
		{
#ifdef _GMTOOL_
			if( MAINGAME->GetUserLevel() <= eUSERLEVEL_GM  && CAMERA->GetCameraMode() != eCM_EyeView )
				pPlayer->GetEngineObject()->SetAlpha( 0.3f );
			else
#endif
				pPlayer->GetEngineObject()->HideWithScheduling();

			
			pPlayer->GetCharacterTotalInfo()->bVisible = FALSE;
			if( pPlayer->GetID() == OBJECTMGR->GetSelectedObjectID() )
			{
				OBJECTMGR->SetSelectedObject( NULL );	
			}
		}

		OBJECTMGR->ApplyShadowOption(pPlayer);		
		OBJECTMGR->ApplyOverInfoOption(pPlayer);	
	}
	
#ifdef _GMTOOL_
	void CCheatMsgParser::Cheat_GM_Login_Ack( void* pMsg ) 
	{
		MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

		if( MAINGAME->GetUserLevel() <= eUSERLEVEL_GM )
		{
			GMTOOLMGR->Login( TRUE, (int)pmsg->dwData );
		}
	}

	void CCheatMsgParser::Cheat_GM_Login_Nack( void* pMsg ) 
	{
		MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;

		if( MAINGAME->GetUserLevel() <= eUSERLEVEL_GM )
		{
			GMTOOLMGR->Login( FALSE, (int)pmsg->bData );
		}
	}

	void CCheatMsgParser::Cheat_PartyInfo_Ack( void* pMsg ) 
	{
		PARTY_INFO * pmsg = (PARTY_INFO*)pMsg;
		for(int i=0; i<MAX_PARTY_LISTNUM; ++i)
		{
			if(pmsg->Member[i].dwMemberID == 0)
				SafeStrCpy(pmsg->Member[i].Name, "!!!", MAX_NAME_LENGTH+1);
			CHATMGR->AddMsg( CTC_SYSMSG, "%s, %d, %d, %d", 
				pmsg->Member[i].Name, pmsg->Member[i].Level, pmsg->Member[i].bLogged,
				pmsg->Member[i].LifePercent);
		}
	}

	void CCheatMsgParser::Cheat_EventMap_Result( void* pMsg ) 
	{
		MSG_NAME_WORD* pmsg = (MSG_NAME_WORD*)pMsg;
		if( pmsg->dwObjectID == 0 )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 716 ), pmsg->wData, pmsg->Name );
		}
		else if( pmsg->dwObjectID == 1 )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 716 ), pmsg->wData, pmsg->Name );
		}
		else if( pmsg->dwObjectID == 2 )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 717 ), pmsg->wData );
		}

		EVENTMAP->Init();
	}

	void CCheatMsgParser::Cheat_EventNotify_On( void* pMsg ) 
	{
		MSG_EVENTNOTIFY_ON* pmsg = (MSG_EVENTNOTIFY_ON*)pMsg;

		NOTIFYMGR->SetEventNotifyStr( pmsg->strTitle, pmsg->strContext );
		NOTIFYMGR->SetEventNotify( TRUE );

		if( GAMEIN->GetEventNotifyDialog() )
		{
			GAMEIN->GetEventNotifyDialog()->SetTitle( pmsg->strTitle );
			GAMEIN->GetEventNotifyDialog()->SetContext( pmsg->strContext );
			GAMEIN->GetEventNotifyDialog()->SetActive( TRUE );
			NOTIFYMGR->SetEventNotifyChanged( FALSE );
			//사운드?
		}
		else
		{
			NOTIFYMGR->SetEventNotifyChanged( TRUE );
		}

		NOTIFYMGR->ResetEventApply();
		for(int i=0; i<eEvent_Max; ++i)
		{
			if( pmsg->EventList[i] )
				NOTIFYMGR->SetEventApply( i );
		}
	}

	void CCheatMsgParser::Cheat_EventNotify_Off( void* pMsg ) 
	{
		MSGBASE* pmsg = (MSGBASE*)pMsg;
		NOTIFYMGR->SetEventNotify( FALSE );
		NOTIFYMGR->SetEventNotifyChanged( FALSE );

		if( GAMEIN->GetEventNotifyDialog() )
		{
			GAMEIN->GetEventNotifyDialog()->SetActive( FALSE );	
		}
	}

	void CCheatMsgParser::Cheat_NpcHide_Ack( void* pMsg ) 
	{
		MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;

		OBJECTMGR->HideNpc(pmsg->wData1, (BOOL)pmsg->wData2);
	}

	void CCheatMsgParser::Cheat_Default_Stats_Ack( void* pMsg ) 
	{
		if( !pMsg ) return ;
			
		SEND_DEFAULT_STATS* pmsg = NULL ;
		pmsg = (SEND_DEFAULT_STATS*)pMsg ;

		if( !pmsg ) return ;

		if( !HERO ) return ;

		HERO->SetStrength((DWORD)pmsg->nStr) ;
		HERO->SetDexterity((DWORD)pmsg->nDex) ;
		HERO->SetVitality((DWORD)pmsg->nVit) ;
		HERO->SetIntelligence((DWORD)pmsg->nInt) ;
		HERO->SetWisdom((DWORD)pmsg->nWis) ;
		GAMEIN->GetCharacterDialog()->SetPointLeveling(TRUE, (WORD)pmsg->nPoint);
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 1282 ) );
		//HERO->SetHeroLevelupPoint((LEVELTYPE)pmsg->nPoint);
	}

	void CCheatMsgParser::Cheat_Default_Stats_Nack( void* pMsg ) 
	{
	}

	void CCheatMsgParser::Cheat_Default_Skill_Ack( void* pMsg )
	{
		if( !pMsg ) return ;

		SEND_SKILL_RESETINFO* pmsg = NULL ;
		pmsg = (SEND_SKILL_RESETINFO*)pMsg ;

		if( !pmsg ) return ;

		if( !HERO ) return ;

		SKILLTREEMGR->RemoveAllSkill() ;

		CAddableInfoIterator iter(&pmsg->AddableInfo) ;

		SKILL_BASE SkillTreeInfo[ 1024 ] ;
		iter.GetInfoData( SkillTreeInfo ) ;

		ASSERT( 1024 > pmsg->SkillNum ) ;

		SKILLTREEMGR->InitSkillInfo( SkillTreeInfo, pmsg->SkillNum ) ;

		HERO->GetHeroTotalInfo()->SkillPoint = 0;
		HERO->SetSkillPoint(pmsg->nSkillPoint) ;

		//CHATMGR->AddMsg(CTC_SYSMSG, "스킬 정보가 초기화되었습니다.") ;
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 1274 ) );

		cSkillTrainingDlg* dialog = ( cSkillTrainingDlg* )WINDOWMGR->GetWindowForID( SKILL_TRAINING );
		dialog->OpenDialog() ;
	}

	void CCheatMsgParser::Cheat_Default_Skill_Nack( void* pMsg )
	{
	}
	void CCheatMsgParser::Cheat_PvP_Damage_Rate( void* pMsg )
	{
		MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;

		char Text[256] = { 0, };
		//sprintf( Text, "PvP 데미지 비율이 [%d]%로 설정 되었습니다.", pmsg->bData );
		sprintf( Text, CHATMGR->GetChatMsg(1471), pmsg->bData );
		CHATMGR->AddMsg(CTC_SYSMSG, Text) ;
	}
#endif // _GMTOOL_


/*
void CCheatMsgParser::EventMonsterRegen(WORD type, WORD count)
{
	if( !m_bEvent ) return;

	MSG_EVENT_MONSTERREGEN msg;
	msg.Category	= MP_CHEAT;
	msg.Protocol	= MP_CHEAT_EVENT_MONSTER_REGEN;
	msg.dwObjectID	= HEROID;
//	msg.bBoss = 0;

	int i = 0;
	switch(type)
	{
	case 1:
		{
			for(i=0;i<count;++i)
			{
				msg.MonsterKind = 8;
				msg.ItemID = ExtractItem(0);
				msg.Pos = GetRegenPos();
				NETWORK->Send(&msg,sizeof(msg));
			}

			for(i=0;i<count;++i)
			{
				msg.MonsterKind = 12;
				msg.ItemID = ExtractItem(1);
				msg.Pos = GetRegenPos();
				NETWORK->Send(&msg,sizeof(msg));
			}

			for(i=0;i<count;++i)
			{
				msg.MonsterKind = 14;
				msg.ItemID = ExtractItem(2);
				msg.Pos = GetRegenPos();
				NETWORK->Send(&msg,sizeof(msg));
			}
		}
		break;
	case 2: // ¾×±I Au≫o≫c°￥·u °A´e≫e¼O
		{
			for(i=0;i<count;++i)
			{
				msg.MonsterKind = 19;
				msg.ItemID = ExtractItem(3);
				msg.Pos = GetRegenPos();
				NETWORK->Send(&msg,sizeof(msg));
			}

			for(i=0;i<count;++i)
			{
				msg.MonsterKind = 25;
				msg.ItemID = ExtractItem(4);
				msg.Pos = GetRegenPos();
				NETWORK->Send(&msg,sizeof(msg));
			}

			for(i=0;i<count;++i)
			{
				msg.MonsterKind = 24;
				msg.ItemID = ExtractItem(5);
				msg.Pos = GetRegenPos();
				NETWORK->Send(&msg,sizeof(msg));
			}
		}
		break;
	case 4: // 2±Þμ¿¿μAIAU 4±Þμ¿¿μAIAU ≫c¹≪¶oAI
		{
			for(i=0;i<count;++i)
			{
				msg.MonsterKind = 18;
				msg.ItemID = ExtractItem(6);
				msg.Pos = GetRegenPos();
				NETWORK->Send(&msg,sizeof(msg));
			}

			for(i=0;i<count;++i)
			{
				msg.MonsterKind = 16;
				msg.ItemID = ExtractItem(7);
				msg.Pos = GetRegenPos();
				NETWORK->Send(&msg,sizeof(msg));
			}

			for(i=0;i<count;++i)
			{
				msg.MonsterKind = 91;
				msg.ItemID = ExtractItem(8);
				msg.Pos = GetRegenPos();
				NETWORK->Send(&msg,sizeof(msg));
			}
		}
		break;
	case 3:
		{
			for(i=0;i<count;++i)
			{
				msg.MonsterKind = 84;
				msg.ItemID = ExtractItem(6);
				msg.Pos = GetRegenPos();
				NETWORK->Send(&msg,sizeof(msg));
			}

			for(i=0;i<count;++i)
			{
				msg.MonsterKind = 21;
				msg.ItemID = ExtractItem(7);
				msg.Pos = GetRegenPos();
				NETWORK->Send(&msg,sizeof(msg));
			}

			for(i=0;i<count;++i)
			{
				msg.MonsterKind = 99;
				msg.ItemID = ExtractItem(8);
				msg.Pos = GetRegenPos();
				NETWORK->Send(&msg,sizeof(msg));
			}
		}
		break;
	case 10: //E²¸ª
		{
			msg.MonsterKind = 1000;
			msg.ItemID = 0;
			msg.Pos = GetRegenPos();
//			msg.bBoss = 1;
			NETWORK->Send(&msg,sizeof(msg));
		}
		break;
	default:
		return;
	}
}

VECTOR3 CCheatMsgParser::GetRegenPos()
{
	VECTOR3 pos;
	int rd;
	VECTOR3 Curpos = HERO->GetCurPosition();

	rd = (rand() % 3000)-1500;
	pos.x = Curpos.x + rd;

	pos.y = Curpos.y;

	rd = (rand() % 3000)-1500;
	pos.z = Curpos.z + rd;

	return pos;
}

WORD CCheatMsgParser::ExtractItem(WORD idx)
{
	WORD rd = rand() % m_itemCount[idx];

	ITEM_INFO* pInfo;
	pInfo = ITEMMGR->FindItemInfoForName(m_eventitem[idx][rd].strName);
	if(pInfo)
		return pInfo->ItemIdx;

	return 0;
}
*/


//#endif	//_CHEATENABLE_	//KES CHEAT!!!

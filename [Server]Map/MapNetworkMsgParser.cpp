#include "stdafx.h"
#include "MapNetworkMsgParser.h"
#include "Network.h"
#include "MapDBMsgParser.h"
#include "CharMove.h"
#include "PackedData.h"
#include "SkillTreeManager.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "Player.h"
#include "Npc.h"
#include "PartyManager.h"
#include "UserTable.h"
#include "UngijosikManager.h"
#include "PeaceWarModManager.h"
#include "ItemManager.h"
#include "QuickManager.h"
#include "StreetStallManager.h"
#include "StorageManager.h"
#include "BattleSystem_Server.h"
#include "CharacterManager.h"
#include "BootManager.h"
#include "objectstatemanager.h"
#include "PKManager.h"
/*어빌리티 삭제 - 이영준 #include "AbilityManager.h" */
#include "FriendManager.h"
#include "ChannelSystem.h"
#include "RecallManager.h"
#include "WantedManager.h"
#include "QuestManager.h"
#include "LootingManager.h"
#include "JournalManager.h"
#include "Party.h"
#include "CharacterCalcManager.h"
#include "FameManager.h"
#include "ObjectFactory.h"
#include "Guild.h"
#include "GuildManager.h"
#include "EventMapMgr.h"
#include "GuildFieldWarMgr.h"
#include "PartyWarMgr.h"
#include "QuestMapMgr.h"
#include "GuildTournamentMgr.h"
#include "SiegeWarMgr.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "GuildUnionManager.h"
#include "cJackpotManager.h"
#include "SiegeWarProfitMgr.h"
#include "Monster.h"

// Weather System
#include "WeatherManager.h"

#include "../[cc]skill/server/manager/skillmanager.h"

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.04.11
#include "../hseos/Farm/SHFarmManager.h"
// E 농장시스템 추가 added by hseos 2007.04.11

// desc_hseos_패밀리01
// S 패밀리 추가 added by hseos 2007.07.15
#include "../hseos/Family/SHFamilyManager.h"
// E 패밀리 추가 added by hseos 2007.07.15

// desc_hseos_데이트 존_01
// S 데이트 존 추가 added by hseos 2007.11.19
#include "../hseos/Date/SHDateManager.h"
// E 데이트 존 추가 added by hseos 2007.11.19

// desc_hseos_결혼_01
// S 결혼 추가 added by hseos 2007.12.11
#include "../hseos/Marriage/SHMarriageManager.h"
// E 결혼 추가 added by hseos 2007.12.11

//---KES AUTONOTE
#include "AutoNoteManager.h"
//---------------

//080327 NYJ --- 낚시시스템
#include "FishingManager.h"
#include "AISystem.h"
#include "RegenManager.h"

//#include "FamilyManager.h"
#include "AttackManager.h"

#include "MHFile.h"

// 070429 LYW --- MapNetworkMsgParser : Add for test.
#ifdef _DEBUG
#include "CharacterCalcManager.h"
#include "ItemDrop.h"
#endif //_DEBUG

#include "GridSystem.h"

#include "PetManager.h"
#include "Pet.h"

extern HWND g_hWnd;
extern BOOL g_bCloseWindow;
extern int	g_nHackCheckNum;
extern int	g_nHackCheckWriteNum;
//extern BOOL g_bPlusTime;

void MP_MonitorMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_MORNITORMAPSERVER_NOTICESEND_SYN:			MornitorMapServer_NoticeSend_Syn() ;											break;
	case MP_MORNITORMAPSERVER_PING_SYN:					MornitorMapServer_Ping_Syn(pTempMsg, dwConnectionIndex, pMsg, dwLength) ;		break;
	case MP_MORNITORMAPSERVER_QUERYUSERCOUNT_SYN:		MornitorMapServer_QueryUserCount_Syn(dwConnectionIndex, pMsg) ;					break;
	case MP_MORNITORMAPSERVER_QUERY_CHANNELINFO_SYN:	MornitorMapServer_Query_ChannelInfo_Syn(dwConnectionIndex) ;					break;
	case MP_MORNITORMAPSERVER_ASSERTMSGBOX_SYN:			MornitorMapServer_AssertMsgBox_Syn(pMsg) ;										break;
	case MP_MORNITORMAPSERVER_SERVEROFF_SYN:			MornitorMapServer_ServerOff_Syn() ;												break;
	}
}


void MornitorMapServer_NoticeSend_Syn() 
{
	ASSERT(0);
}


void MornitorMapServer_Ping_Syn(MSGROOT* pTempMsg, DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	pTempMsg->Protocol = MP_MORNITORMAPSERVER_PING_ACK;
	g_Network.Send2Server(dwConnectionIndex, pMsg, dwLength);
}


void MornitorMapServer_QueryUserCount_Syn(DWORD dwConnectionIndex, char* pMsg) 
{
	MSGUSERCOUNT  * pmsg = (MSGUSERCOUNT  *)pMsg;
	pmsg->Protocol = MP_MORNITORMAPSERVER_QUERYUSERCOUNT_ACK;
	pmsg->dwUserCount = g_pUserTable->GetUserCount();
	pmsg->ServerPort = g_pServerTable->GetSelfServer()->wPortForServer;
	g_Network.Send2Server(dwConnectionIndex, pMsg, sizeof(MSGUSERCOUNT));
}


void MornitorMapServer_Query_ChannelInfo_Syn(DWORD dwConnectionIndex) 
{
	CHANNELSYSTEM->SendChannelInfoToMS( dwConnectionIndex );			
}


void MornitorMapServer_AssertMsgBox_Syn(char* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	g_bAssertMsgBox = (BOOL)pmsg->dwData;
	if(g_bAssertMsgBox)
		g_Console.LOG(4,"Assert MsgBox is On");
	else
		g_Console.LOG(4,"Assert MsgBox is Off");
}


void MornitorMapServer_ServerOff_Syn() 
{
	/*
	g_bCloseWindow = TRUE;	//콘솔창 닫는다.

	g_pServerSystem->HandlingBeforeServerEND();*/

	//SW060719 수정
	g_pServerSystem->SetStart(FALSE);
	g_pServerSystem->HandlingBeforeServerEND();

	g_Console.LOG(4, "Close Window" );
	g_bCloseWindow = TRUE;
}




void MP_POWERUPMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	BOOTMNGR->NetworkMsgParse(dwConnectionIndex, pMsg, dwLength);
}

void MP_MAPSERVERMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	ASSERT(0);
/*
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_SERVER_PWRUP:
		{
			MSG_WORD * msg = (MSG_WORD *)pMsg;
			SERVERINFO * info = g_pServerTable->FindServer(msg->wData);//port
			//PACKDATA에서 사용하는 변수
			if(g_pServerTable->GetMaxServerConnectionIndex() < dwConnectionIndex)
				g_pServerTable->SetMaxServerConnectionIndex(dwConnectionIndex);
			ASSERT(info);
			if(info)
			{
				info->dwConnectionIndex = dwConnectionIndex;
				if(info->wServerKind == AGENT_SERVER || info->wServerKind == MONITOR_SERVER || info->wServerKind == MONITOR_AGENT_SERVER)
				{
					REGIST_MAP msg;
					msg.Category = MP_SERVER;
					msg.Protocol = MP_SERVER_REGISTMAP_ACK;
					msg.mapServerPort = g_pServerTable->GetSelfServer()->wPortForServer;
					msg.mapnum = GAMERESRCMNGR->GetLoadMapNum();
					g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));
				}
			}
		}
		break;
	case MP_SERVER_REGISTMAP_SYN:
		{
			REGIST_MAP msg;
			msg.Category = MP_SERVER;
			msg.Protocol = MP_SERVER_REGISTMAP_ACK;
			msg.mapServerPort = g_pServerTable->GetSelfServer()->wPortForServer;
			msg.mapnum = GAMERESRCMNGR->GetLoadMapNum();
			g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));
		}
		break;
	}*/

}
void MP_CHARMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	//case MP_CHAR_LEVELDOWN_SYN:					Char_LevelDown_Syn(pMsg) ;									break;
	case MP_CHAR_POINTADD_SYN:						Char_PointAdd_Syn(pMsg) ;									break;
	case MP_CHAR_STATE_NOTIFY:						Char_State_Notify(pMsg) ;									break;
/*어빌리티 삭제 - 이영준 	
	case MP_CHAR_ABILITY_UPGRADE_SYN:				Char_Ability_Upgrade_Syn(pMsg) ;							break;
	case MP_CHAR_ABILITY_UPGRADE_SKPOINT_SYN:		Char_Ability_Upgrade_SkPoint_Syn(pMsg) ;					break;
	case MP_CHAR_ABILITY_DOWNGRADE_SKPOINT_SYN:		Char_Ability_Downgrade_SkPoint_Syn(pMsg) ;					break;
*/
	case MP_CHAR_EXITSTART_SYN:						Char_ExitStart_Syn(pMsg) ;									break;
	case MP_CHAR_EXIT_SYN:							Char_Exit_Syn(pMsg) ;										break;
	case MP_CHAR_BADFAME_SYN:						Char_BadFame_Syn(pMsg) ;									break;
	case MP_CHAR_PLAYTIME_SYN:						Char_PlayTime_Syn(pMsg) ;									break;
	case MP_CHAR_POINTMINUS_SYN:					Char_PointMinus_Syn(pMsg) ;									break;
	case MP_CHAR_MUSSANG_SYN:						Char_Mussang_Syn(pMsg, dwLength) ;							break;
	case MP_CHAR_EMERGENCY_SYN:						Char_Emergency_Syn(pMsg) ;									break;
		// 080616 LUJ, 플레이어의 HP 정보를 반환한다
		//				서버가 클라이언트의 값을 업데이트한 후, 클라이언트는 EffectUnit 단계에서 HP 값 변경을 지연 처리한다
		//				이에 따라 서버와 클라이언트 값의 비동기 현상이 발생한다. 이를 막기 위해 클라이언트에서
		//				HP 정보 업데이트를 요청하도록 한다
	case MP_CHAR_LIFE_GET_SYN:
		{
			const MSGBASE*	m		= ( MSGBASE* )pMsg;
			CObject*		object	= g_pUserTable->FindUser( m->dwObjectID );
			
            if( !	object			||
					object->GetObjectKind() != eObjectKind_Player )
			{
				break;
			}

			MSG_INT message;
			ZeroMemory( &message, sizeof( message ) );
			message.Category	= MP_CHAR;
			message.Protocol	= MP_CHAR_LIFE_GET_ACK;
			message.nData		= object->GetLife();

			object->SendMsg( &message, sizeof( message ) );
			break;
		}
	}
}


/*
void Char_LevelDown_Syn(char* pMsg)
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(!pPlayer) return;
	
	CHARMGR->SetCharLevelDown(pPlayer, pmsg->wData);
}


*/
void Char_PointAdd_Syn(char* pMsg)
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(!pPlayer || pPlayer->IsResetStat() ) return;

	// 지금 플레이어가 포인트를 올릴수 있는 상태인가?체크
	CHARMGR->ArrangeCharLevelPoint(pPlayer, pmsg->wData);
}


void Char_State_Notify(char* pMsg)
{
	MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(!pPlayer) return;

	if( pPlayer->GetState() == pmsg->bData && pPlayer->GetState() != eObjectState_Die )
		OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer, pmsg->bData);
}


/*
void Char_Ability_Upgrade_Syn(char* pMsg) 
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(!pPlayer) return;
	
	MSG_WORD msg;
	msg.Category = MP_CHAR;
	if(ABILITYMGR->UpgradeAbility(pmsg->wData,pPlayer->GetAbilityGroup()) == TRUE)
		msg.Protocol = MP_CHAR_ABILITY_UPGRADE_ACK;
	else
		msg.Protocol = MP_CHAR_ABILITY_UPGRADE_NACK;
	msg.wData = pmsg->wData;
	pPlayer->SendMsg(&msg,sizeof(msg));
}


void Char_Ability_Upgrade_SkPoint_Syn(char* pMsg) 
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(!pPlayer) return;
	
	MSG_WORD msg;
	msg.Category = MP_CHAR;
	if(ABILITYMGR->UpgradeAbilitySkPoint(pmsg->wData,pPlayer->GetAbilityGroup()) == TRUE)
		msg.Protocol = MP_CHAR_ABILITY_UPGRADE_SKPOINT_ACK;
	else
		msg.Protocol = MP_CHAR_ABILITY_UPGRADE_SKPOINT_NACK;
	msg.wData = pmsg->wData;
	pPlayer->SendMsg(&msg,sizeof(msg));
}


void Char_Ability_Downgrade_SkPoint_Syn(char* pMsg) 
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(!pPlayer) return;
	
	MSG_WORD msg;
	msg.Category = MP_CHAR;
	if(ABILITYMGR->DowngradeAbilitySkPoint(pmsg->wData,pPlayer->GetAbilityGroup()) == TRUE)
		msg.Protocol = MP_CHAR_ABILITY_DOWNGRADE_SKPOINT_ACK;
	else
		msg.Protocol = MP_CHAR_ABILITY_DOWNGRADE_SKPOINT_NACK;
	
	msg.wData = pmsg->wData;
	pPlayer->SendMsg(&msg,sizeof(msg));

	// 06. 03 국내무쌍 - 이영준
	if(pmsg->wData == ABILITYINDEX_TACTIC_MUSSANG)
		pPlayer->InitMussangPoint();
}


*/
void Char_ExitStart_Syn(char* pMsg)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if( !pPlayer ) return;
	if( pPlayer->IsExitStart() ) return;

	int errorCode;
	if( ( errorCode = pPlayer->CanExitStart() ) == eEXITCODE_OK )
	{
		pPlayer->SetExitStart( TRUE );
		MSGBASE msgAck;
		msgAck.Category		= MP_CHAR;
		msgAck.Protocol		= MP_CHAR_EXITSTART_ACK;
		pPlayer->SendMsg(&msgAck, sizeof(msgAck));
	}
	else
	{
		MSG_BYTE msgNack;
		msgNack.Category	= MP_CHAR;
		msgNack.Protocol	= MP_CHAR_EXITSTART_NACK;
		msgNack.bData		= errorCode;
		pPlayer->SendMsg(&msgNack, sizeof(msgNack));
	}
}

void Char_Emergency_Syn(char* pMsg)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if( !pPlayer ) return;

	pPlayer->SetEmergency();
}


void Char_Exit_Syn(char* pMsg)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if( !pPlayer ) return;
	if( !pPlayer->IsExitStart() ) return;
	
	int nErrorCode;
	if( ( nErrorCode = pPlayer->CanExit() ) == eEXITCODE_OK )
	{
		OBJECTSTATEMGR_OBJ->StartObjectState( pPlayer, eObjectState_Exit, 0 );
		pPlayer->SetNormalExit();
		pPlayer->SetExitStart( FALSE );

		MSGBASE msgAck;
		msgAck.Category = MP_CHAR;
		msgAck.Protocol = MP_CHAR_EXIT_ACK;				
		pPlayer->SendMsg(&msgAck, sizeof(msgAck));
	}
	else
	{
		pPlayer->SetExitStart( FALSE );
		MSG_BYTE msgNack;
		msgNack.Category	= MP_CHAR;
		msgNack.Protocol	= MP_CHAR_EXIT_NACK;
		msgNack.bData		= nErrorCode;
		pPlayer->SendMsg(&msgNack, sizeof(msgNack));				
	}	
}


void Char_BadFame_Syn(char* pMsg)
{
	MSG_FAME* pmsg = (MSG_FAME*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(!pPlayer)		return;

	if( FAMEMGR->BailBadFame( pPlayer, pmsg->Fame ) == TRUE)
	{				
		pPlayer->SetBadFame( pPlayer->GetBadFame() - pmsg->Fame );
		pPlayer->SetMoney( pmsg->Fame * BAIL_PRICE, MONEY_SUBTRACTION, 0, eItemTable_Inventory, eMoneyLog_LoseBadFrameBail, pPlayer->GetID() );
		CharacterBadFameUpdate( pPlayer->GetID(), pPlayer->GetBadFame(), pPlayer->GetMoney() );
		
		//전체에게 보내기
		FAMEMGR->SendBadFameMsg( pPlayer, pPlayer->GetBadFame() );

		//나에게 확인 보내기(메세지 보여주기)
		MSG_FAME msg;
		msg.Category	= MP_CHAR;
		msg.Protocol	= MP_CHAR_BADFAME_ACK;
		msg.dwObjectID	= pPlayer->GetID();
		msg.Fame		= pmsg->Fame;
		pPlayer->SendMsg(&msg, sizeof(msg));
	}
	else
	{
		MSGBASE msg;
		msg.Category	= MP_CHAR;
		msg.Protocol	= MP_CHAR_BADFAME_NACK;
		msg.dwObjectID	= pPlayer->GetID();
		pPlayer->SendMsg(&msg, sizeof(msg));
	}
}


void Char_PlayTime_Syn(char* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(!pPlayer)		return;

	int edst = abs((int)( pmsg->dwData - pPlayer->GetPlayTime() ));

	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.dwData = pPlayer->GetPlayTime();

	if(edst < MAX_ALLOWANCE)
	{
		msg.Protocol = MP_CHAR_PLAYTIME_ACK;
	}
	else
	{
		msg.Protocol = MP_CHAR_PLAYTIME_NACK;

		// Assert
		char buf[256] = { 0, };
		sprintf(buf, "플레이시간이 오차범위를 벗어났습니다. player:%d Server:%d ecost:%d allowance:%d",
			pmsg->dwData, pPlayer->GetPlayTime(), edst, MAX_ALLOWANCE);
		ASSERTMSG(0, buf);
	}

	pPlayer->SendMsg(&msg, sizeof(msg));
}


void Char_PointMinus_Syn(char* pMsg)
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(!pPlayer) return;

	// 지금 플레이어가 포인트를 올릴수 있는 상태인가?체크
	MSG_WORD msg;
	msg.Category = MP_CHAR;			

	if( CHARMGR->ArrangeCharMinusPoint(pPlayer, pmsg->wData) )
	{
		msg.Protocol = MP_CHAR_POINTMINUS_ACK;
	}
	else
	{
		msg.Protocol = MP_CHAR_POINTMINUS_NACK;
	}

//	msg.wData = pPlayer->GetShopItemStats()->StatePoint;
	pPlayer->SendMsg(&msg, sizeof(msg));
}


void Char_Mussang_Syn(char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(!pPlayer) return;

	int rt = pPlayer->StartMussangMode();

	MSGBASE msg;
	msg.Category = MP_CHAR;
	msg.dwObjectID = pPlayer->GetID();
	switch( rt )
	{
	case 1:
		{
			msg.Protocol = MP_CHAR_MUSSANG_ACK;
			PACKEDDATA_OBJ->QuickSend(pPlayer,&msg,dwLength);
		}
		break;
	case -99:
		{
			msg.Protocol = MP_CHAR_MUSSANG_NACK;
			pPlayer->SendMsg(&msg, sizeof(msg));
		}
		break;
	}
}

void MP_CHATMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
		// 061130 LYW --- Add New Msg Parser.
	case MP_CHAT_NORMAL :
	case MP_CHAT_ALL:
		{
			TESTMSG* pmsg = (TESTMSG*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer) return;
/*
			if( pPlayer->GetUserLevel() == eUSERLEVEL_GM )
				pmsg->Protocol = MP_CHAT_GM;
*/			
			PACKEDDATA_OBJ->QuickSend(pPlayer,pmsg,dwLength);
		}
		break;
	// 070105 LYW --- Add protocol.
	case MP_CHAT_WHOLE :
	case MP_CHAT_TRADE :
	case MP_CHAT_SMALLSHOUT:			//같은 채널의 모두에게 보내기!
		{
			MSG_CHAT_WITH_SENDERID* pmsg = (MSG_CHAT_WITH_SENDERID*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer) return;

			MSG_CHAT_WITH_SENDERID msg;
			msg.Category = pmsg->Category;
			msg.Protocol = pmsg->Protocol;
			msg.dwObjectID = pmsg->dwObjectID;
			msg.dwSenderID = pPlayer->GetID();
			SafeStrCpy( msg.Msg, pmsg->Msg, MAX_CHAT_LENGTH+1 );
			SafeStrCpy( msg.Name, pmsg->Name, MAX_NAME_LENGTH+1 );
 
			if( pPlayer->GetUserLevel() == eUSERLEVEL_GM )
 				msg.Protocol = MP_CHAT_GM_SMALLSHOUT;
			else
			{
				if( pPlayer->GetBattle()->GetBattleKind() != eBATTLE_KIND_NONE && pPlayer->GetBattle()->GetBattleKind() != eBATTLE_KIND_SHOWDOWN )
					return;
			}
			
			g_pUserTable->SetPositionUserHead();
			while( CObject* pObject = g_pUserTable->GetUserData() )
			{
				if( pObject->GetObjectKind() != eObjectKind_Player ) continue;
				
				CPlayer* pReceiver = (CPlayer*)pObject;
				if( pPlayer->GetChannelID() == pReceiver->GetChannelID() /*&&
					pPlayer->GetBattleID() < 5 && pReceiver->GetBattleID() < 5*/ )
				{
 					pReceiver->SendMsg( &msg, msg.GetMsgLength() );
				}
			}
		}
		break;
	case MP_CHAT_GUILD:
		{
			TESTMSGID * pmsg = (TESTMSGID*)pMsg;
			GUILDMGR->SendChatMsg(pmsg->dwObjectID, pmsg->dwSenderID, pmsg->Msg);
		}
		break;
	//case MP_CHAT_FAMILY :
	//	{
	//		SEND_FAMILY_CHAT * pmsg = (SEND_FAMILY_CHAT*)pMsg;
	//		FAMILYMGR->SendChatMsg(pmsg);
	//	}
	//	break ;
	case MP_CHAT_GUILDUNION:
		{
			MSG_CHAT_WITH_SENDERID * pmsg = (MSG_CHAT_WITH_SENDERID*)pMsg;
			GUILDUNIONMGR->SendChatMsg( pmsg );
		}
		break;

	default:
		//GAMESYSTEM_OBJ->m_ConnectionTable.SendToCharacter(pTempMsg,dwLength);
		break;
	}
}

void MP_USERCONNMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_USERCONN_CHANNELINFO_SYN:				UserConn_ChannelInfo_Syn(pTempMsg, dwConnectionIndex, pMsg) ;	break;
	case MP_USERCONN_GAMEIN_OTHERMAP_SYN:			UserConn_GameIn_OtherMap_Syn(pMsg) ;							break;
	case MP_USERCONN_GAMEIN_SYN:					UserConn_GameIn_Syn(dwConnectionIndex, pMsg) ;					break;
	case MP_USERCONN_GAMEINPOS_SYN:					UserConn_GameInPos_Syn(dwConnectionIndex, pMsg) ;				break;
	case MP_USERCONN_GAMEIN_NACK:					UserConn_GameIn_Nack(pTempMsg) ;								break;
	case MP_USERCONN_GAMEOUT_SYN:					UserConn_GameOut_Syn(pMsg) ;									break;
	case MP_USERCONN_NOWAITEXITPLAYER:				UserConn_NoWaitExitPlayer(pMsg) ;								break;
	case MP_USERCONN_DISCONNECTED:					UserConn_Disconnected(dwConnectionIndex, pMsg) ;				break;
	case MP_USERCONN_CHANGEMAP_SYN :				UserConn_ChangeMap_Syn(dwConnectionIndex, pMsg) ;				break;
	case MP_USERCONN_CHANGEMAP_NACK:				UserConn_ChangeMap_Nack() ;										break;
	// 070917 LYW --- MapNetworkMsgParser : Add process to change map through the npc.
	case MP_USERCONN_NPC_CHANGEMAP_SYN :			UserConn_Npc_ChangeMap_Syn(dwConnectionIndex, pMsg) ;			break;
	case MP_USERCONN_SAVEPOINT_SYN:					UserConn_SavePoint_Syn(pMsg) ;									break;
	case MP_USERCONN_BACKTOCHARSEL_SYN:				UserConn_BackToCharSel_Syn(pTempMsg, dwConnectionIndex, pMsg) ;	break;
	case MP_USERCONN_SETVISIBLE:					UserConn_SetVisible(pMsg) ;										break;		
	case MP_USERCONN_BACKTOBEFOREMAP_SYN:			UserConn_BackToBeforeMap_Syn(pMsg) ;							break;
	case MP_USERCONN_ENTER_EVENTMAP_SYN:			UserConn_Enter_EventMap_Syn(dwConnectionIndex, pMsg) ;			break;
	case MP_USERCONN_RETURN_SYN :					UserConn_Return_Syn(dwConnectionIndex, pMsg) ;					break;

	case MP_USERCONN_NPC_ADD_SYN:					UserConn_NpcAdd_Syn(pMsg);										break;
	case MP_USERCONN_NPC_DIE_SYN:					UserConn_NpcDie_Syn(pMsg);										break;
	}
}


void UserConn_ChannelInfo_Syn(MSGBASE* pTempMsg, DWORD dwConnectionIndex, char* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pTempMsg;
	CHANNELSYSTEM->SendChannelInfo(pmsg, dwConnectionIndex);
}


void UserConn_GameIn_OtherMap_Syn(char* pMsg) 
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	g_pServerSystem->RemovePlayer( pmsg->dwObjectID );
}


void UserConn_GameIn_Syn(DWORD dwConnectionIndex, char* pMsg)
{
	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.27
	// ..챌린지 존 섹션 번호 추가
	// MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
	MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;
	// E 데이트 존 추가 added by hseos 2007.11.27
	CObject* pObject = g_pUserTable->FindUser(pmsg->dwObjectID);

	if( pObject != NULL &&
		pObject->GetObjectKind() == eObjectKind_Player )
	{
//			g_pServerSystem->RemovePlayer(pmsg->dwObjectID);

		MSG_DWORD msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_GAMEIN_NACK;
		msg.dwData		= pmsg->dwObjectID;

		g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );					
		return;
	}

	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.28
	// ..챌린지 존 섹션 번호 추가
	// CPlayer* pPlayer = g_pServerSystem->AddPlayer(pmsg->dwObjectID,dwConnectionIndex,pmsg->dwData1, pmsg->dwData2);
	CPlayer* pPlayer = g_pServerSystem->AddPlayer(pmsg->dwObjectID,dwConnectionIndex,pmsg->dwData1, pmsg->dwData2, pmsg->dwData4);
	// E 데이트 존 추가 added by hseos 2007.11.28

	if(!pPlayer) return;

	// 080424 NYJ --- 낚시정보를 받아온다
	FishingData_Load(pmsg->dwObjectID);

	pPlayer->SetUserLevel( pmsg->dwData3 );
	CharacterNumSendAndCharacterInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
	CharacterSkillTreeInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
	CharacterItemInfo( pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN );
	
	/// 단축창 정보
	QuickInfo( pmsg->dwObjectID );


	// quest 정보 qurey 추가
	QUESTMGR->CreateQuestForPlayer( pPlayer );

	//퀘스트정보를 받아오도록 한다.
	QuestTotalInfo( pmsg->dwObjectID );				//QuestMainQuestLoad(), QuestSubQuestLoad(), QuestItemLoad(), TutorialLoad()이 연계된다.

	//농장 및 기타정보를 받아오도록 한다.
	Farm_LoadTimeDelay( pmsg->dwObjectID );			//MostserMeter_Load(), ResidentRegist_LoadInfo()가 연계된다.

/*
	QuestTotalInfo(pmsg->dwObjectID);		// 초보자 퀘스트
	
	QuestMainQuestLoad(pmsg->dwObjectID);
	QuestSubQuestLoad(pmsg->dwObjectID);

	QuestItemload(pmsg->dwObjectID);

	// 071023 LYW --- MapNetworkMsgParser : loading tutorial info.
	TutorialLoad(pmsg->dwObjectID) ;


	// desc_hseos_몬스터미터01
	// S 몬스터미터 추가 added by hseos 2007.05.28
	// ..몬스터미터 정보 로드
	MonsterMeter_Load(pmsg->dwObjectID);
	// E 몬스터미터 추가 added by hseos 2007.05.28

	// desc_hseos_주민등록01
	// S 주민등록 추가 added by hseos 2007.06.04
	// ..주민등록 정보 로드
	ResidentRegist_LoadInfo(pmsg->dwObjectID);
	// E 주민등록 추가 added by hseos 2007.06.04

	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.09.12
	// ..농작물 심기, 비료주기 딜레이 시간 로드
	Farm_LoadTimeDelay(pmsg->dwObjectID);
	// E 농장시스템 추가 added by hseos 2007.09.12


*/

	CYHHashTable<HIDE_NPC_INFO>* pHideNpcTable = GAMERESRCMNGR->GetHideNpcTable();
	
	// 06. 05 HIDE NPC - 이영준
	// 현재 맵의 숨김 가능 NPC들의 정보를 보내준다
	{
		pHideNpcTable->SetPositionHead();

		MSG_WORD2 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_NPCHIDE_ACK;

		for(	HIDE_NPC_INFO* pInfo = NULL;
				pInfo = pHideNpcTable->GetData();
				)
		{			
			msg.wData1 = pInfo->UniqueIdx;
			msg.wData2 = pInfo->ChannelInfo[pmsg->dwData2];

			pPlayer->SendMsg( &msg, sizeof(msg) );
		}
	}


	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.27	2007.11.28
	// ..함수 순서를 지키는 것이 좋음
	// ..챌린지 존 시작
	g_csDateManager.SRV_StartChallengeZone(pPlayer, pmsg->dwData2, pmsg->dwData4);
	// ..챌린지 존 입장 회수 로드
	ChallengeZone_EnterFreq_Load(pPlayer->GetID());
	// E 데이트 존 추가 added by hseos 2007.11.27	2007.11.28

	// desc_hseos_결혼_01
	// S 결혼 추가 added by hseos 2007.12.11
//	Marriage_Load(pPlayer->GetID());
	// E 결혼 추가 added by hseos 2007.12.11


	// 080328 NYJ --- 날씨정보를 보낸다. 낚시시스템 관련
	WEATHERMGR->SendWeather(pPlayer);
}


void UserConn_GameInPos_Syn(DWORD dwConnectionIndex, char* pMsg) 
{
	MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;

	CObject* pObject = g_pUserTable->FindUser(pmsg->dwObjectID);

	if( pObject != NULL )	//이미 있다.
	{
		if( pObject->GetObjectKind() == eObjectKind_Player )	//유저인가?
		{
//					g_pServerSystem->RemovePlayer(pmsg->dwObjectID);

			MSG_DWORD msg;
			msg.Category	= MP_USERCONN;
			msg.Protocol	= MP_USERCONN_GAMEIN_NACK;
			msg.dwData		= pmsg->dwObjectID;
			g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );					
			return;
		}
	}

	CPlayer* pPlayer = g_pServerSystem->AddPlayer(pmsg->dwObjectID,dwConnectionIndex,pmsg->dwData1, pmsg->dwData2);
	if(!pPlayer)
		return;
	
	pPlayer->SetUserLevel( pmsg->dwData3 );
	pPlayer->SetInitPoint( pmsg->dwData4 );

	CharacterNumSendAndCharacterInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
	CharacterSkillTreeInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
	CharacterItemInfo( pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN );
	
	/// 단축창 정보
	QuickInfo( pmsg->dwObjectID );

	// quest 정보 qurey 추가
	QUESTMGR->CreateQuestForPlayer( pPlayer );

	//퀘스트정보를 받아오도록 한다.
	QuestTotalInfo( pmsg->dwObjectID );				//QuestMainQuestLoad(), QuestSubQuestLoad(), QuestItemLoad(), TutorialLoad()이 연계된다.

	//농장 및 기타정보를 받아오도록 한다.
	Farm_LoadTimeDelay( pmsg->dwObjectID );			//MostserMeter_Load(), ResidentRegist_LoadInfo()가 연계된다.

/*
	QuestTotalInfo(pmsg->dwObjectID);		// 초보자 퀘스트
	QuestMainQuestLoad(pmsg->dwObjectID);
	QuestSubQuestLoad(pmsg->dwObjectID);
	QuestItemload(pmsg->dwObjectID);
*/

}


void UserConn_GameIn_Nack(MSGBASE* pTempMsg) 
{
	// GAMEIN_ACK를 보냈을때 에이젼트에서 유니크 아이디가 틀리면
	// 이 메세지를 날려준다. 그 케릭터를 그냥 지워버리면 된다.
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pTempMsg->dwObjectID);
	if(pPlayer == NULL)
		return;

	g_pServerSystem->RemovePlayer(pTempMsg->dwObjectID);
}


void UserConn_GameOut_Syn(char* pMsg) 
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( pPlayer == NULL ) return;

	g_pServerSystem->RemovePlayer( pmsg->dwObjectID );
	g_Console.LOG( 4, "Charackter Overlap!" );
}


void UserConn_NoWaitExitPlayer(char* pMsg) 
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if(pPlayer == NULL) return;

	pPlayer->SetWaitExitPlayer( FALSE );	//no wait for deleteing player.
}


void UserConn_Disconnected(DWORD dwConnectionIndex, char* pMsg) 
{
	//KES	//agent가 유저 정보 DB업데이트를 위해서 보내준다.
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if(pPlayer == NULL)
	{
		//KES 로그아웃 프로시져 호출하기 위해 에이젼트로 보내준다.
		MSGBASE msgLogout;
		msgLogout.Category		= MP_USERCONN;
		msgLogout.Protocol		= MP_USERCONN_LOGINCHECK_DELETE;
		msgLogout.dwObjectID	= pmsg->dwData;

		g_Network.Send2Server( dwConnectionIndex, (char*)&msgLogout, sizeof(msgLogout) );
		return;
	}

	if( !pPlayer->IsNormalExit() )
	{
		//강제 종료에 따른 제제
		if( pPlayer->IsPKMode() )
		{
			//PKMGR->PKModeExitPanelty( pPlayer );
			//---KES PK 071202
			PKMGR->PKModeDiePanelty( pPlayer ); //강제 종료일 경우는 PK모드 죽음 패널티를 적용한다.
		}

		LOOTINGMGR->DiePlayerExitPanelty( pPlayer );

//---KES AUTONOTE
		if( pPlayer->GetAutoNoteIdx() )	//---오토신고중인 캐릭터
		{
			AUTONOTEMGR->AutoPlayerLogOut( pPlayer );
		}
//---------------

		//강제 종료한 유저 잡아두기
		{
			// RaMa - 04.12.14
			pPlayer->UpdateLogoutToDB();

			PARTYMGR->UserLogOut(pPlayer);
			FRIENDMGR->UserLogOut(pmsg->dwObjectID);
			GUILDMGR->UserLogOut(pPlayer);

			g_pServerSystem->RemovePlayer(pmsg->dwObjectID);
			//
			ConfirmUserOut( dwConnectionIndex, pPlayer->GetUserID(), pPlayer->GetID(), 2 );
		}
	}
	else
	{
		// RaMa - 04.12.14
		pPlayer->UpdateLogoutToDB();

		PARTYMGR->UserLogOut(pPlayer);
		FRIENDMGR->UserLogOut(pmsg->dwObjectID);
		GUILDMGR->UserLogOut(pPlayer);

		g_pServerSystem->RemovePlayer(pmsg->dwObjectID);
		//
		ConfirmUserOut( dwConnectionIndex, pPlayer->GetUserID(), pPlayer->GetID(), 2 );
	}
}


	// 070121 LYW --- Modified this part.
	/*
void UserConn_ChangeMap_Syn(DWORD dwConnectionIndex, char* pMsg) 
{
	MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(pPlayer == NULL)
		return;
	
	if( !QUESTMAPMGR->IsQuestMap() )
	if( pPlayer->IsPKMode() || LOOTINGMGR->IsLootedPlayer( pPlayer->GetID() ) || pPlayer->GetState() == eObjectState_Die )
	{
		MSGBASE msgNack;
		msgNack.Category	= MP_USERCONN;
		msgNack.Protocol	= MP_USERCONN_CHANGEMAP_NACK;
		msgNack.dwObjectID	= pPlayer->GetID();
		pPlayer->SendMsg( &msgNack, sizeof(msgNack) );
		return;
	}

	MAPCHANGE_INFO* ChangeInfo = NULL;
	// Item사용추가
	if( pmsg->dwData1 > 2000 )
	{
		SHOPITEMUSEBASE BaseInfo;
		BaseInfo.ShopItemIdx = (WORD)pmsg->dwData2;
		BaseInfo.ShopItemPos = (POSTYPE)pmsg->dwData3;
		SHOPITEMBASE pShopItem;

		ChangeInfo = GAMERESRCMNGR->GetMapChangeInfoForDestMapNum( (WORD)(pmsg->dwData1-2000) );
		if( !ChangeInfo )
		{
			MSG_ITEM_ERROR msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_SHOPITEM_USE_NACK;
			pPlayer->SendMsg( &msg, sizeof(msg) );
			return;
		}

		if( ITEMMGR->IsUseAbleShopItem( pPlayer, BaseInfo.ShopItemIdx, BaseInfo.ShopItemPos ) )
		//BYTE rt = ITEMMGR->UseShopItem( pPlayer, BaseInfo, &pShopItem );
		//if(rt == eItemUseSuccess)
		{
			ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( BaseInfo.ShopItemIdx );
			if( pItemInfo && pItemInfo->ItemType == 10 )
				ITEMMGR->DiscardItem( pPlayer, BaseInfo.ShopItemPos, BaseInfo.ShopItemIdx, 1 );

			SEND_SHOPITEM_BASEINFO msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_SHOPITEM_USE_ACK;
			msg.ShopItemBase = pShopItem;
			msg.ShopItemIdx = BaseInfo.ShopItemIdx;
			msg.ShopItemPos = BaseInfo.ShopItemPos;					
			ITEMMGR->SendAckMsg(pPlayer, &msg, sizeof(msg));					
		}
		else
		{
			MSG_ITEM_ERROR msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_SHOPITEM_USE_NACK;
			msg.ECode = 0;
			ITEMMGR->SendErrorMsg(pPlayer, &msg, sizeof(msg), 0);
			return;
		}				
		
	}
	else
	{				
		ChangeInfo = GAMERESRCMNGR->GetMapChangeInfo(pmsg->dwData1);
		if( !ChangeInfo ) return;

		// checkhacknpc
		if( CheckHackNpc( pPlayer, (WORD)pmsg->dwData1 ) == FALSE )
		{
			MSGBASE msgNack;
			msgNack.Category	= MP_USERCONN;
			msgNack.Protocol	= MP_USERCONN_CHANGEMAP_NACK;
			msgNack.dwObjectID	= pPlayer->GetID();
			pPlayer->SendMsg( &msgNack, sizeof(msgNack) );
			return;
		}
	//
	}			
	//Pet 맵이동시 펫 소환상태 유지
	pPlayer->GetPetManager()->SetPetSummonning(ePSS_SaveSummon);
	// RaMa - 04.12.14
	pPlayer->UpdateLogoutToDB();

	LEVELTYPE lvl = pPlayer->GetLevel();
	EXPTYPE exp = pPlayer->GetPlayerExpPoint();
	g_pServerSystem->RemovePlayer(pPlayer->GetID());
	
	// 리턴 받으면 맵 아웃 메시지 보냄.
	SaveMapChangePointReturn(pmsg->dwObjectID, ChangeInfo->Kind, dwConnectionIndex, lvl, exp, pPlayer->GetMoney(eItemTable_Storage));
}


	*/
void UserConn_ChangeMap_Syn(DWORD dwConnectionIndex, char* pMsg) 
{
	MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(pPlayer == NULL)
		return;

	//---KES PK 071202
	//---KES AUTONOTE
	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.12.03
	// ..챌린지 존에서 죽었을 경우는 맵 이동을 시켜야 함.
	// if( pPlayer->IsPKMode() || LOOTINGMGR->IsLootedPlayer( pPlayer->GetID() ) || pPlayer->GetState() == eObjectState_Die )
	if( pPlayer->GetAutoNoteIdx() || pPlayer->IsPKMode() || LOOTINGMGR->IsLootedPlayer( pPlayer->GetID() ) || (pPlayer->GetState() == eObjectState_Die && g_csDateManager.IsChallengeZoneHere() == FALSE))
	// E 데이트 존 추가 added by hseos 2007.12.03
	{
		MSGBASE msgNack;
		msgNack.Category	= MP_USERCONN;
		msgNack.Protocol	= MP_USERCONN_CHANGEMAP_NACK;
		msgNack.dwObjectID	= pPlayer->GetID();
		pPlayer->SendMsg( &msgNack, sizeof(msgNack) );
		return;
	}

	MAPCHANGE_INFO* ChangeInfo = NULL;

	ChangeInfo = GAMERESRCMNGR->GetMapChangeInfo(pmsg->dwData1);
	if( !ChangeInfo )
	{
		MSG_ITEM_ERROR msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_USE_NACK;
		pPlayer->SendMsg( &msg, sizeof(msg) );
		return;
	}

	// RaMa - 04.12.14
	pPlayer->UpdateLogoutToDB(FALSE);

	VECTOR3 RandPos;

	int temp;
	temp = rand() % 500 - 250;
	RandPos.x = ChangeInfo->MovePoint.x + temp;
	temp = rand() % 500 - 250;
	RandPos.z = ChangeInfo->MovePoint.z + temp;
	RandPos.y = 0;

	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.26
	// ..챌린지 존으로 이동할 경우 현재 위치를 저장한다. 챌린지 존에서 로그아웃되었을 경우, 다음에 접속 시의 위치로 사용함.
	if (g_csDateManager.IsChallengeZone(g_pServerSystem->GetMapNum()) == FALSE &&
		g_csDateManager.IsChallengeZone(ChangeInfo->MoveMapNum))
	{
		VECTOR3 stPos;
		pPlayer->GetPosition(&stPos);

		ChallengeZone_SavePos_Save(pPlayer->GetID(), g_pServerSystem->GetMapNum(), (DWORD)stPos.x, (DWORD)stPos.z);
	}
	// E 데이트 존 추가 added by hseos 2007.11.26

	//CharacterLogoutPointUpdate( pPlayer->GetID(), ChangeInfo->MoveMapNum, (DWORD)RandPos.x, (DWORD)RandPos.z );

	pPlayer->SetMapMoveInfo( ChangeInfo->MoveMapNum, (DWORD)RandPos.x, (DWORD)RandPos.z );

	// 081218 LUJ, 해제하기 전에 값을 복사하자
	const DWORD		playerIndex		= pPlayer->GetID();
	const LEVELTYPE	level			= pPlayer->GetLevel();
	const EXPTYPE	experience		= pPlayer->GetPlayerExpPoint();
	const MONEYTYPE	inventoryMoney	= pPlayer->GetMoney( eItemTable_Inventory );
	const MONEYTYPE	storageMoney	= pPlayer->GetMoney( eItemTable_Storage );

	g_pServerSystem->RemovePlayer(pPlayer->GetID());
	
	// 리턴 받으면 맵 아웃 메시지 보냄.
	// 081218 LUJ, 플레이어 소지금 처리 추가
	SaveMapChangePointReturn(
		playerIndex,
		ChangeInfo->Kind,
		dwConnectionIndex,
		level,
		experience,
		inventoryMoney,
		storageMoney );
}

// 070917 LYW --- MapNetworkMsgParser : Add function to process change map through the npc.
void UserConn_Npc_ChangeMap_Syn(DWORD dwConnectionIndex, char* pMsg)
{
	MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(pPlayer == NULL) return;

	// RaMa - 04.12.14
	pPlayer->UpdateLogoutToDB(FALSE);

	//CharacterLogoutPointUpdate( pPlayer->GetID(), pPlayer->GetCurMapNum(), pmsg->dwData2, pmsg->dwData3 );
	
	pPlayer->SetMapMoveInfo( ( MAPTYPE )( pmsg->dwData1 ), pmsg->dwData2, pmsg->dwData3 );

	// 081218 LUJ, 해제하기 전에 값을 복사하자
	const DWORD		playerIndex		= pPlayer->GetID();
	const LEVELTYPE	level			= pPlayer->GetLevel();
	const EXPTYPE	experience		= pPlayer->GetPlayerExpPoint();
	const MONEYTYPE	inventoryMoney	= pPlayer->GetMoney( eItemTable_Inventory );
	const MONEYTYPE	storageMoney	= pPlayer->GetMoney( eItemTable_Storage );
	
	g_pServerSystem->RemovePlayer( playerIndex );

	// 리턴 받으면 맵 아웃 메시지 보냄.
	// 081218 LUJ, 플레이어 소지금 처리 추가
	SaveNpcMapChangePointReturn(
		playerIndex,
		WORD( pmsg->dwData1 ),
		dwConnectionIndex,
		level,
		experience,
		inventoryMoney,
		storageMoney );
}

void UserConn_Return_Syn(DWORD dwConnectionIndex, char* pMsg) 
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;

	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(pPlayer == NULL)
		return;

	LOGINPOINT_INFO* ReturnInfo = NULL;

	ReturnInfo = GAMERESRCMNGR->GetLoginPointInfo( pmsg->wData );

	if( !ReturnInfo )
	{
		return;
	}


	if( ReturnInfo->MapNum == GAMERESRCMNGR->GetLoadMapNum() )
	{
		VECTOR3 RandPos;

		int temp;
		temp = rand() % 500 - 250;
		RandPos.x = ReturnInfo->CurPoint[0].x + temp;
		temp = rand() % 500 - 250;
		RandPos.z = ReturnInfo->CurPoint[0].z + temp;
		RandPos.y = 0;

		CCharMove::Warp(pPlayer,&RandPos);

		return;
	}

	// RaMa - 04.12.14
	pPlayer->UpdateLogoutToDB(FALSE);
	
	VECTOR3 RandPos;

	int temp;
	temp = rand() % 500 - 250;
	RandPos.x = ReturnInfo->CurPoint[0].x + temp;
	temp = rand() % 500 - 250;
	RandPos.z = ReturnInfo->CurPoint[0].z + temp;
	RandPos.y = 0;

	//CharacterLogoutPointUpdate( pPlayer->GetID(), ReturnInfo->MapNum, (DWORD)RandPos.x, (DWORD)RandPos.z );
	
	pPlayer->SetMapMoveInfo( ReturnInfo->MapNum, (DWORD)RandPos.x, (DWORD)RandPos.z );

	// 081218 LUJ, 해제하기 전에 값을 복사하자
	const DWORD		playerIndex		= pPlayer->GetID();
	const LEVELTYPE	level			= pPlayer->GetLevel();
	const EXPTYPE	experience		= pPlayer->GetPlayerExpPoint();
	const MONEYTYPE	storageMoney	= pPlayer->GetMoney( eItemTable_Storage );
	const MONEYTYPE	inventoryMoney	= pPlayer->GetMoney( eItemTable_Inventory );

	g_pServerSystem->RemovePlayer(pPlayer->GetID());

	// 리턴 받으면 맵 아웃 메시지 보냄.
	// 081218 LUJ, 플레이어 소지금 처리 추가
	SaveMapChangePointReturn(
		playerIndex,
		ReturnInfo->MapNum + 2000,
		dwConnectionIndex,
		level,
		experience,
		inventoryMoney,
		storageMoney );
}


void UserConn_NpcAdd_Syn(char* pMsg)
{
	MSG_EVENT_NPCSUMMON* pmsg = (MSG_EVENT_NPCSUMMON*)pMsg;



	int i;
	for(i=0; i<pmsg->cbNpcCount; i++)
	{
		VECTOR3 vOutPos;
		vOutPos = pmsg->Pos;

		DWORD dwGridID = 0;
		MAPTYPE MapNum = pmsg->wMap;
		vOutPos.x = ((DWORD)(vOutPos.x/50.0f)*50.0f);
		vOutPos.z = ((DWORD)(vOutPos.z/50.0f)*50.0f);



		BASEOBJECT_INFO Baseinfo;
		NPC_TOTALINFO NpcTotalInfo;
		NPC_LIST* pNpcList = GAMERESRCMNGR->GetNpcInfo(pmsg->NpcKind);
		if(pNpcList == 0)
			return;

		DWORD dwID = 0;
		Baseinfo.dwObjectID =  g_pAISystem->GeneraterMonsterID();
		Baseinfo.BattleID = pmsg->cbChannel;
		
		SafeStrCpy(Baseinfo.ObjectName, pNpcList->Name, MAX_NAME_LENGTH+1);
		NpcTotalInfo.Group = 0;
		NpcTotalInfo.MapNum = MapNum;
		NpcTotalInfo.NpcJob = pNpcList->JobKind;
		NpcTotalInfo.NpcKind = pNpcList->NpcKind;
		NpcTotalInfo.NpcUniqueIdx = (WORD)dwID;

		CNpc* pNpc = g_pServerSystem->AddNpc(&Baseinfo, &NpcTotalInfo, &vOutPos);
		if(pNpc)
		{
			pNpc->SetDieTime(0);	// 자동소멸되지 않도록.
		}
	}
}

void UserConn_NpcDie_Syn(char* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	CNpc* pNpc = (CNpc*)g_pUserTable->FindUser(pmsg->dwData);
	if(pNpc == NULL)	return;

	MSG_DWORD2 msg;
	msg.Category	= MP_USERCONN;

	if(pNpc->GetObjectKind() & eObjectKind_Npc)
	{
		msg.Protocol	= MP_USERCONN_NPC_DIE;

		msg.dwObjectID	= 0;
		msg.dwData1		= pmsg->dwObjectID;
		msg.dwData2		= pNpc->GetID();
		
		PACKEDDATA_OBJ->QuickSend((CObject*)pNpc,&msg,sizeof(msg));
	}

	pNpc->Die(NULL);
}


void UserConn_ChangeMap_Nack() 
{
	g_Console.Log(eLogDisplay, 4, "Recv OK!!!");
}


void UserConn_SavePoint_Syn(char* pMsg) 
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if( pPlayer == NULL )	return;

	LOGINPOINT_INFO* LoginInfo = GAMERESRCMNGR->GetLoginPointInfo(pmsg->dwData);
	if( LoginInfo == NULL )	return;

	// checkhacknpc
	if( CheckHackNpc( pPlayer, (WORD)pmsg->dwData ) == FALSE )
		return;
	//
	
	WORD ServerMapNum = GAMERESRCMNGR->GetLoadMapNum();
	SavePointUpdate(pmsg->dwObjectID, LoginInfo->Kind, ServerMapNum);

	// 맵전환포인트를 항상 0으로 셋팅해준다.
/*			MAPCHANGE_INFO* ChangeInfo = GAMERESRCMNGR->GetMapChangeInfoForDestMapNum( (WORD)pmsg->dwData1 );
	if( !ChangeInfo ) return;
	SaveMapChangePointUpdate( pPlayer->GetID(), ChangeInfo->Kind );*/

	SaveMapChangePointUpdate(pmsg->dwObjectID, 0);
	
	pPlayer->InitMapChangePoint();
	pPlayer->SetLoginPoint(LoginInfo->Kind);
}


void UserConn_BackToCharSel_Syn(MSGBASE* pTempMsg, DWORD dwConnectionIndex, char* pMsg) 
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(pPlayer == NULL)
		return;
	// RaMa - 04.12.14
	pPlayer->UpdateLogoutToDB();
	PARTYMGR->UserLogOut(pPlayer);
	FRIENDMGR->UserLogOut(pTempMsg->dwObjectID);
	GUILDMGR->UserLogOut(pPlayer);
	//GUILDMGR->RemovePlayer( pPlayer->GetGuildIdx(), pPlayer->GetID() );

	SaveCharInfoBeforeLogout(pPlayer->GetID(), dwConnectionIndex, pPlayer->GetUserID(), pPlayer->GetMoney(eItemTable_Storage));

	// desc_hseos_몬스터미터01
	// S 몬스터미터 추가 added by hseos 2007.05.29
	// ..DB의 플레이타임과 몬스터킬 수를 초기화 해야 한다.
	// ..누적되는 것이 아니고 로그인해서 로그아웃할 떄까지의 데이터이기 때문.
	//MonsterMeter_Save(pPlayer->GetID(), 0, 0, pPlayer->GetMonstermeterInfo()->nPlayTimeTotal, pPlayer->GetMonstermeterInfo()->nKillMonsterNumTotal);
	// E 몬스터미터 추가 added by hseos 2007.05.29
}


void UserConn_SetVisible(char* pMsg) 
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(pPlayer == NULL)
		return;
	pPlayer->SetVisible(FALSE);
	PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pPlayer,pmsg,sizeof(MSG_DWORD2));
}




//=========================================================================
//	NAME : UserConn_BackToBeforeMap_Syn
//	DESC : The function to move player to specified destination. 080104 LYW
//=========================================================================
void UserConn_BackToBeforeMap_Syn(char* pMsg) 
{
	if( !pMsg ) return ;

	MSG_DWORD3* pmsg = NULL ;
	pmsg = (MSG_DWORD3*)pMsg ;

	if( !pmsg ) return ;

	DWORD dwObjectID = 0 ;
	dwObjectID = pmsg->dwObjectID ;

	CPlayer* pPlayer = NULL ;
	pPlayer = (CPlayer*)g_pUserTable->FindUser( dwObjectID ) ;

	if( !pPlayer ) return ;

	SaveMapChangePointUpdate( dwObjectID, 0 ) ;

	pPlayer->UpdateLogoutToDB() ;					
	g_pServerSystem->RemovePlayer( dwObjectID ) ;

	// 080603 LYW --- MapNetworkMsgParser : 컴파일 워닝 수정.
	//CharacterLogoutPointUpdate( pPlayer->GetID(), pmsg->dwData1, pmsg->dwData2, pmsg->dwData3 );
	CharacterLogoutPointUpdate( pPlayer->GetID(), (WORD)pmsg->dwData1, pmsg->dwData2, pmsg->dwData3 );

	MSG_WORD msg;
	msg.Category	= MP_USERCONN;
	msg.Protocol	= MP_USERCONN_BACKTOBEFOREMAP_ACK;
	msg.dwObjectID	= dwObjectID ;
	msg.wData		= (WORD)pmsg->dwData1;

	g_Network.Broadcast2AgentServer( (char*)&msg, sizeof(msg) ) ;	
}


//void UserConn_BackToBeforeMap_Syn(char* pMsg) 
//{
//	MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
//	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
//	if( pPlayer == NULL )	return;
//	
//	MAPCHANGE_INFO* ChangeInfo = GAMERESRCMNGR->GetMapChangeInfoFromToMapNum( pmsg->wData1, pmsg->wData2 );
//	if( !ChangeInfo )
//	{
//		ChangeInfo = GAMERESRCMNGR->GetMapChangeInfoForDestMapNum( pmsg->wData2 );
//		if( !ChangeInfo )	return;
//	}
//	SaveMapChangePointUpdate( pPlayer->GetID(), ChangeInfo->Kind );
//
//	// RaMa - 04.12.14
//	pPlayer->UpdateLogoutToDB();					
//	g_pServerSystem->RemovePlayer( pPlayer->GetID() );
//
//	MSG_WORD msg;
//	msg.Category	= MP_USERCONN;
//	msg.Protocol	= MP_USERCONN_BACKTOBEFOREMAP_ACK;
//	msg.dwObjectID	= pPlayer->GetID();
//	msg.wData		= pmsg->wData2;
//
//	g_Network.Broadcast2AgentServer( (char*)&msg, sizeof(msg) );	
//}


void UserConn_Enter_EventMap_Syn(DWORD dwConnectionIndex, char* pMsg) 
{
	MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;
	CPlayer* pPlayer = g_pServerSystem->AddPlayer( pmsg->dwObjectID, dwConnectionIndex, pmsg->dwData1, pmsg->dwData3-1 );
	if( !pPlayer )	return;

	pPlayer->SetUserLevel( pmsg->dwData2 );
	CharacterNumSendAndCharacterInfo2(pmsg->dwObjectID, pmsg->dwData4);
	CharacterSkillTreeInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
	CharacterItemInfo( pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN );
	
	EVENTMAPMGR->AddMember( pPlayer, pmsg->dwData4, pmsg->dwData3 );
}

void MP_MOVEMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_MOVE_TARGET:				Move_Target(pMsg, dwLength) ;			break;
	case MP_MOVE_ONETARGET:				Move_OneTarget(pMsg) ;					break;
	case MP_MOVE_STOP:					Move_Stop(pMsg, dwLength) ;				break;
	case MP_MOVE_WALKMODE:				Move_WalkMode(pMsg, dwLength) ;			break;	
	case MP_MOVE_RUNMODE:				Move_RunMode(pMsg, dwLength) ;			break;	
	case MP_MOVE_KYUNGGONG_SYN:			Move_Kyunggong_Syn(pMsg, dwLength) ;	break;		
	case MP_MOVE_EFFECTMOVE:			Move_EffectMove(pMsg) ;					break;	
	case MP_MOVE_PET_MOVE:				Move_Pet_Move(pMsg) ;					break;
	case MP_MOVE_PET_STOP:				Move_Pet_Stop(pMsg, dwLength) ;			break;
	default:																	break;
	}
}


void Move_Target(char* pMsg, DWORD dwLength)
{
	/*
	MOVE_TARGETPOS* pmsg = (MOVE_TARGETPOS*)pMsg;
	VECTOR3 Start;
	pmsg->spos.Decompress(&Start);
	VECTOR3 Target;
	pmsg->tpos.Decompress(&Target);
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwMoverID);
	if(pPlayer == NULL)
		return;

	if(pPlayer->GetState() == eObjectState_Die)
		return;

	//////////////////////////////////////////////////////////////////////////
	// 클라이언트들의 충돌 체크(클라이언트들에서 하기 때문에 필요없지만... 확인용)
	// 클라이언트의 해킹등을 방지하기 위해..
	VECTOR3 colpos;
	MAPTYPE MapNum = GAMERESRCMNGR->GetLoadMapNum();
	if(g_pServerSystem->GetMap()->CollisionCheck(&Start,&Target,&colpos,MapNum))
	{
		CCharMove::CorrectPlayerPosToServer(pPlayer);
		return;
	}
	//////////////////////////////////////////////////////////////////////////			

	CCharMove::StartMove(pPlayer,gCurTime,&Start,&Target);
	*/
	MOVE_TARGETPOS* pmsg = (MOVE_TARGETPOS*)pMsg;
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwMoverID);
	if(pPlayer == NULL)
		return;
	if(pPlayer->GetState() == eObjectState_Die)
	{
		MSG_DWORD msg;
		msg.Category = MP_CHAR;
		msg.Protocol = MP_CHAR_YOUAREDIED;
		pPlayer->SendMsg(&msg,sizeof(msg));
		return;
	}
	if(pPlayer->GetState() == eObjectState_Ungijosik)
	{				
		CCharMove::CorrectPlayerPosToServer(pPlayer);
		return;
	}
//KES			
	if(pPlayer->GetState() == eObjectState_Immortal )
	{
		OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer, eObjectState_Immortal, 0 );
	}
	if(pmsg->tpos.PosNum >= MAX_CHARTARGETPOSBUF_SIZE)
	{
		CCharMove::CorrectPlayerPosToServer(pPlayer);
		return;
	}
//
	VECTOR3 Start;
	pmsg->GetStartPos(&Start);
	
	VECTOR3* pCurPos = CCharMove::GetPosition(pPlayer);
	if(CalcDistanceXZ(pCurPos,&Start) >= 1000)
	{				
		CCharMove::CorrectPlayerPosToServer(pPlayer);
		return;
	}
	pmsg->GetTargetInfo((MOVE_INFO *)pPlayer->GetMoveInfo());
//			ASSERT(pPlayer->GetMoveInfo()->GetCurTargetPosIdx() < pPlayer->GetMoveInfo()->GetMaxTargetPosIdx());
	CCharMove::StartMoveEx(pPlayer,gCurTime,&Start,pPlayer->GetMoveInfo()->GetTargetPosition(pPlayer->GetMoveInfo()->GetCurTargetPosIdx()));
	pmsg->AddedMoveSpeed = pPlayer->GetMoveInfo()->AddedMoveSpeed;

	// 071008 웅주, 상태값 변경
	{
		OBJECTSTATEMGR_OBJ->StartObjectState( pPlayer, eObjectState_Move, 0 );
	}

	// RaMa
	CParty* pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx());
	if(pParty)
	{
		pParty->SendMemberPos(pPlayer->GetID(), pPlayer->GetMoveInfo());
	}

	PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pPlayer,pmsg,dwLength);
}


void Move_OneTarget(char* pMsg) 
{
	MOVE_ONETARGETPOS* pmsg = (MOVE_ONETARGETPOS*)pMsg;
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwMoverID);
	if(pPlayer == NULL)
		return;
	if(pPlayer->GetInited() == FALSE || pPlayer->GetInitedGrid() == FALSE)
		return;
	if(pPlayer->GetState() == eObjectState_Die)
	{
		MSG_DWORD msg;
		msg.Category = MP_CHAR;
		msg.Protocol = MP_CHAR_YOUAREDIED;
		pPlayer->SendMsg(&msg,sizeof(msg));
		return;
	}
	if(pPlayer->GetState() == eObjectState_Immortal )
	{
		OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer, eObjectState_Immortal, 0 );
	}

//---KES AUTONOTE
//	pPlayer->SetLastActionTime( gCurTime );
//---------------

	VECTOR3 Start;
	pmsg->GetStartPos(&Start);
	
	VECTOR3* pCurPos = CCharMove::GetPosition(pPlayer);
	if(CalcDistanceXZ(pCurPos,&Start) >= 1000)
	{				
		CCharMove::CorrectPlayerPosToServer(pPlayer);
		return;
	}

	pmsg->GetTargetInfo((MOVE_INFO *)pPlayer->GetMoveInfo());			
//			ASSERT(pPlayer->GetMoveInfo()->GetCurTargetPosIdx() < pPlayer->GetMoveInfo()->GetMaxTargetPosIdx());
	CCharMove::StartMoveEx(pPlayer,gCurTime,&Start,pPlayer->GetMoveInfo()->GetTargetPosition(pPlayer->GetMoveInfo()->GetCurTargetPosIdx()));
	// RaMa
	CParty* pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx());
	if(pParty)
	{
		pParty->SendMemberPos(pPlayer->GetID(), pPlayer->GetMoveInfo());
	}
	
	// 071008 웅주, 상태값 변경
	{
		OBJECTSTATEMGR_OBJ->StartObjectState( pPlayer, eObjectState_Move, 0 );
	}

	MOVE_ONETARGETPOS_FROMSERVER msg;
	SetProtocol(&msg,MP_MOVE,MP_MOVE_ONETARGET);
	msg.AddedMoveSpeed = pPlayer->GetMoveInfo()->AddedMoveSpeed;
	msg.dwObjectID = pPlayer->GetID();
	msg.SetStartPos(&Start);
	msg.SetTargetPos(pPlayer->GetMoveInfo()->GetTargetPosition(0));

	PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pPlayer,&msg,msg.GetSize());
}

void Move_Stop(char* pMsg, DWORD dwLength) 
{
	MOVE_POS* pmsg = (MOVE_POS*)pMsg;
	VECTOR3 pos;
	pmsg->cpos.Decompress(&pos);
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwMoverID);
	if(pPlayer == NULL)
		return;

	// 상태 변경
	//if( !OBJECTSTATEMGR_OBJ->StartObjectState(pPlayer, eObjectState_None, 0) )
	//	return;
	//if(pPlayer->GetState() == eObjectState_Move)
	OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer,eObjectState_Move);

	//////////////////////////////////////////////////////////////////////////
	// 클라이언트들의 충돌 체크(클라이언트들에서 하기 때문에 필요없지만... 확인용)
	// 클라이언트의 해킹등을 방지하기 위해..
	VECTOR3 colpos;
	VECTOR3* pCurPos = CCharMove::GetPosition(pPlayer);

	MAPTYPE MapNum = GAMERESRCMNGR->GetLoadMapNum();
	if(g_pServerSystem->GetMap()->CollisionCheck(pCurPos,&pos,&colpos,pPlayer))
	{
		CCharMove::CorrectPlayerPosToServer(pPlayer);
		return;
	}
	//////////////////////////////////////////////////////////////////////////	
	// 정상적인 이동인지 확인하고 틀리면 Correct를 해준다.
	if(CCharMove::CheckVaildStop(pPlayer,gCurTime,&pos) == TRUE)
		CCharMove::EndMove(pPlayer,gCurTime,&pos);	// 정상적이면 EndMove
	
	PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pPlayer,pmsg,dwLength);
}

void Move_Pet_Move(char* pMsg) 
{
	MOVE_ONETARGETPOS* pmsg = ( MOVE_ONETARGETPOS* )pMsg;
	CObject* pTarget = ( CObject* )g_pUserTable->FindUser( pmsg->dwObjectID );
	CPet* pPet = ( CPet* )g_pUserTable->FindUser( pmsg->dwMoverID );

	if( !pTarget || !pPet )
		return;

	if( pTarget->GetInited() == FALSE || pTarget->GetInitedGrid() == FALSE )
		return;

	if( pPet->GetInited() == FALSE || pPet->GetInitedGrid() == FALSE )
		return;

	VECTOR3 Start;
	pmsg->GetStartPos( &Start );
	
	VECTOR3* pCurPos = CCharMove::GetPosition( pPet );
	if( CalcDistanceXZ( pCurPos, &Start) >= 1000 )
	{				
		CCharMove::CorrectPlayerPosToServer( pPet );
		return;
	}

	pmsg->GetTargetInfo( (MOVE_INFO*)pPet->GetMoveInfo() );			

	CCharMove::StartMoveEx( pPet, gCurTime, &Start, pPet->GetMoveInfo()->GetTargetPosition( pPet->GetMoveInfo()->GetCurTargetPosIdx() ) );

	MOVE_ONETARGETPOS_FROMSERVER msg;
	SetProtocol( &msg, MP_MOVE,MP_MOVE_PET_MOVE );
	msg.AddedMoveSpeed = pPet->GetMoveInfo()->AddedMoveSpeed;
	msg.dwObjectID = pPet->GetID();
	msg.SetStartPos( &Start );
	msg.SetTargetPos( pPet->GetMoveInfo()->GetTargetPosition( 0 ) );

	PACKEDDATA_OBJ->QuickSendExceptObjectSelf( pPet, &msg,msg.GetSize() );
}

void Move_Pet_Stop(char* pMsg, DWORD dwLength) 
{
	MOVE_POS* pmsg = (MOVE_POS*)pMsg;
	VECTOR3 pos;
	pmsg->cpos.Decompress(&pos);

	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	CPet* pPet = ( CPet* )g_pUserTable->FindUser( pmsg->dwMoverID );

	if( !pPlayer || !pPet )
		return;

	VECTOR3 colpos;
	VECTOR3* pCurPos = CCharMove::GetPosition( pPet );

	MAPTYPE MapNum = GAMERESRCMNGR->GetLoadMapNum();
	if( g_pServerSystem->GetMap()->CollisionCheck( pCurPos, &pos, &colpos, pPet ) )
	{
		CCharMove::CorrectPlayerPosToServer( pPet );
		return;
	}
	if( CCharMove::CheckVaildStop( pPet, gCurTime, &pos ) == TRUE )
		CCharMove::EndMove( pPet, gCurTime, &pos );
	
	PACKEDDATA_OBJ->QuickSendExceptObjectSelf( pPet, pmsg, dwLength );
}

void Move_WalkMode(char* pMsg, DWORD dwLength) 
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(pPlayer == NULL)
		return;
	CCharMove::SetWalkMode(pPlayer);
	
	PACKEDDATA_OBJ->QuickSend(pPlayer,pmsg,dwLength);
}


void Move_RunMode(char* pMsg, DWORD dwLength) 
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(pPlayer == NULL)
		return;
	CCharMove::SetRunMode(pPlayer);
	
	PACKEDDATA_OBJ->QuickSend(pPlayer,pmsg,dwLength);
}


void Move_Kyunggong_Syn(char* pMsg, DWORD dwLength) 
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(pPlayer == NULL)
		return;
	if(CCharMove::SetKyungGong(pPlayer,(WORD)pmsg->dwData2))
	{
		pmsg->Protocol = MP_MOVE_KYUNGGONG_ACK;
		PACKEDDATA_OBJ->QuickSend(pPlayer,pmsg,dwLength);
	}
	else
	{
		pmsg->Protocol = MP_MOVE_KYUNGGONG_NACK;
		pPlayer->SendMsg(pmsg,dwLength);
	}
}


void Move_EffectMove(char* pMsg) 
{
	MOVE_POS* pmsg = (MOVE_POS*)pMsg;
	CObject* pObject = g_pUserTable->FindUser(pmsg->dwMoverID);
	if(pObject == NULL)
		return;

	if( pObject->IsEffectMove() == FALSE )
		return;
	
	VECTOR3 pos;
	pmsg->cpos.Decompress(&pos);

	if( pObject->GetObjectKind() == eObjectKind_Player )
	{
		VECTOR3* pCurPos = CCharMove::GetPosition( pObject );
		if( CalcDistanceXZ( &pos, pCurPos ) >= 1500 )
		{
			CCharMove::CorrectPlayerPosToServer( pObject );
			return;
		}
	}
	
	if(CheckValidPosition(pos) == FALSE)
	{
		return;
	}

	CCharMove::SetPosition(pObject,&pos);
//			PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pObject,pmsg,dwLength);
}



void MP_ITEMMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE *pmsg = (MSGBASE *)pMsg;
	ITEMMGR->NetworkMsgParse( dwConnectionIndex, pmsg->Protocol, pMsg );
}
void MP_SKILLTREEMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	SKILLTREEMGR->NetworkMsgParse(pTempMsg->Protocol, pMsg);
}
void MP_CHEATMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	CPlayer* pSender = (CPlayer*)g_pUserTable->FindUser( pTempMsg->dwObjectID );
	if(pSender)
	{
		if( pSender->GetUserLevel() > eUSERLEVEL_GM )
		{
			char buf[64];
			sprintf(buf, "User Lvl Chk! Character_idx : %d", pTempMsg->dwObjectID);
			ASSERTMSG(0, buf);
			return;
		}
	}
	switch(pTempMsg->Protocol)
	{

	case MP_CHEAT_WHEREIS_SYN:					Cheat_WhereIs_Syn(dwConnectionIndex, pMsg) ;			break;
	case MP_CHEAT_HIDE_SYN:						Cheat_Hide_Syn(pMsg) ;									break;
//	case MP_CHEAT_BANCHARACTER_SYN:				Cheat_BanCharacter_Syn(pMsg) ;							break;
	case MP_CHEAT_PKALLOW_SYN:					Cheat_PkAllow_Syn(pSender, pMsg) ;						break;
	case MP_CHEAT_MOVETOCHAR_SYN:				Cheat_MoveToChar_Syn(pMsg) ;							break;
	case MP_CHEAT_MOVE_SYN:						Cheat_Move_Syn(pMsg) ;									break;		
	case MP_CHEAT_MOVEUSER_SYN:					Cheat_MoveUser_Syn(pMsg) ;								break;		
	case MP_CHEAT_CHANGEMAP_SYN:				Cheat_ChangeMap_Syn(pMsg) ;								break;
	case MP_CHEAT_LIFE_SYN:						Cheat_Life_Syn() ;										break;
	case MP_CHEAT_MP_SYN:						Cheat_Mp_Syn() ;										break;
	case MP_CHEAT_ITEM_SYN:						Cheat_Item_Syn(pMsg) ;									break;
		//SW051011 강화/레어 치트
	//case MP_CHEAT_ITEM_OPTION_SYN:				Cheat_Item_Option_Syn(pMsg) ;							break;
	case MP_CHEAT_MANA_SYN:						Cheat_Mana_Syn(pMsg) ;									break;		
	case MP_CHEAT_ADDSKILL_SYN:					Cheat_AddSkill_Syn(pMsg) ;								break;
	case MP_CHEAT_SKILLLEVEL_SYN:				Cheat_SkillLevel_Syn(pMsg) ;							break;
	case MP_CHEAT_UPDATE_SKILL_SYN:				Cheat_Update_Skill_Syn(pMsg) ;							break;
	case MP_CHEAT_MONEY_SYN:					Cheat_Money_Syn(pMsg) ;									break;
	case MP_CHEAT_RELOADING:					Cheat_ReLoading() ;										break;
	case MP_CHEAT_LEVELUP_SYN:					Cheat_LevelUp_Syn(pMsg) ;								break;
	case MP_CHEAT_PET_LEVELUP_SYN:				Cheat_PetLevelUp_Syn(pMsg) ;								break;
	// 070411 LYW --- MapNetworkMsgParser : Add parser msg for level down.
	case MP_CHEAT_LEVELDOWN_SYN :				Cheat_LevelDown_Syn( pMsg ) ;							break ;
/*어빌리티 삭제 - 이영준 
	case MP_CHEAT_ABILITYEXP_SYN:				Cheat_AbilityExp_Syn(pMsg) ;							break;*/
	case MP_CHEAT_EVENT_MONSTER_REGEN:			Cheat_Event_Monster_Regen(pMsg) ;						break;
	case MP_CHEAT_EVENT_MONSTER_DELETE:			Cheat_Event_Monster_Delete(pMsg) ;						break;
	case MP_CHEAT_STR_SYN:						Cheat_Str_Syn(pMsg) ;									break;
	case MP_CHEAT_DEX_SYN:						Cheat_Dex_Syn(pMsg) ;									break;
	case MP_CHEAT_VIT_SYN:						Cheat_Vit_Syn(pMsg) ;									break;
	case MP_CHEAT_WIS_SYN:						Cheat_Wis_Syn(pMsg) ;									break;
	case MP_CHEAT_INT_SYN:						Cheat_Int_Syn(pMsg) ;									break;
	case MP_CHEAT_KILL_ALLMONSTER_SYN:			Cheat_Kill_AllMonster_Syn() ;							break;
	case MP_CHEAT_PARTYINFO_SYN:				Cheat_PartyInfo_Syn(pMsg) ;								break;
	case MP_CHEAT_EVENT_SYN:					Cheat_Event_Syn(pMsg) ;									break;
	case MP_CHEAT_PLUSTIME_ALLOFF:				Cheat_PlusTime_AllOff(pMsg) ;							break;
	case MP_CHEAT_PLUSTIME_ON:					Cheat_PlusTime_On(pMsg) ;								break;
	case MP_CHEAT_PLUSTIME_OFF:					Cheat_PlusTime_Off(pMsg) ;								break;
	case MP_CHEAT_MUNPADATECLR_SYN:				Cheat_MunpaDateClr_Syn(pMsg) ;							break;
	case MP_CHEAT_CHANGE_EVENTMAP_SYN:			Cheat_Change_EventMap_Syn(pMsg) ;						break;		
	case MP_CHEAT_EVENT_START_SYN:				Cheat_Event_Start_Syn(pMsg) ;							break;
	case MP_CHEAT_EVENT_READY_SYN:				Cheat_Event_Ready_Syn(pMsg) ;							break;		
	case MP_CHEAT_GETSHOPITEM:					Cheat_GetShopItem(pMsg) ;								break;
/*	case MP_CHEAT_STAGE_SYN:					Cheat_Stage_Syn(pMsg) ;									break;	*/
	case MP_CHEAT_BOSSMOB_SYN:					Cheat_BossMob_Syn(pMsg) ;								break;		
	case MP_CHEAT_JACKPOT_GETPRIZE:				Cheat_Jackpot_GetPrize(pMsg) ;							break;		
	case MP_CHEAT_JACKPOT_ONOFF:				Cheat_Jackpot_OnOff(pMsg) ;								break;		
	case MP_CHEAT_JACKPOT_MONEYPERMONSTER:		Cheat_Jackpot_MoneyPerMonster(pMsg) ;					break;
	case MP_CHEAT_JACKPOT_PROBABILITY:			Cheat_Jackpot_Probability(pMsg) ;						break;
	case MP_CHEAT_JACKPOT_CONTROL:				Cheat_Jackpot_Control(pMsg) ;							break;
	case MP_CHEAT_GUILDUNION_CLEAR:				Cheat_GuildUnion_Clear(pMsg) ;							break;
	case MP_CHEAT_MOB_CHAT:						Cheat_Mob_Chat(pMsg) ;									break;
	case MP_CHEAT_MUSSANG_READY:				Cheat_Mussang_Ready(pMsg)  ;							break;
	case MP_CHEAT_GUILDPOINT_SYN:				Cheat_GuildPoint_Syn(pMsg) ;							break;
	//case MP_CHEAT_GUILDHUNTED_MONSTERCOUNT_SYN:	Cheat_GuildHunted_MonsterCount_Syn(pMsg) ;				break;
	// 06. 05 HIDE NPC - 이영준
	case MP_CHEAT_NPCHIDE_SYN:					Cheat_NpcHide_Syn(pMsg) ;								break;
// 070429 LYW --- MapNetworkMsgParser : Add for test.
//#ifdef _DEBUG
	case MP_CHEAT_HPMP_DETERMINATION_SYN :		Cheat_HPMP_Determination_Syn(pMsg) ;					break;
	case MP_CHEAT_DROPITEM_INFO_SYN :			Cheat_DropItem_Syn(pMsg) ;								break;
	case MP_CHEAT_SKILLPOINT_SYN:				Cheat_SkillPoint_Syn(pMsg);								break;
	case MP_CHEAT_GOD_SYN:						Cheat_God_Syn(pMsg);									break;
//#endif //_DEBUG

	// 071106 LYW --- MapNetworkMsgParser : Default stats.
	case MP_CHEAT_DEFAULT_STATS_SYN :			Cheat_Default_Stats(pMsg) ;								break ;

	// 071106 LYW --- MapNetworkMsgParser : Default skill.
	case MP_CHEAT_DEFAULT_SKILL_SYN :			Cheat_Default_Skill(pMsg) ;								break ;
	case MP_CHEAT_RESET_INVENTORY_SYN :			Cheat_Reset_Inventory(pMsg);							break ;
	case MP_CHEAT_PVP_DAMAGE_RATE_SYN :			Cheat_PvP_Damage_Rate(pMsg);							break ;


//---KES CHEAT PKEVENT
	case MP_CHEAT_PKEVENT:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
			if( pmsg->bData == 0 )
				PKMGR->SetPKEvent( FALSE );
			else if( pmsg->bData == 1 )
				PKMGR->SetPKEvent( TRUE );
		}
		break;
//--------------------
//---KES CHEAT CHANGESIZE
	case MP_CHEAT_CHANGESIZE:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwData1 );
			if( !pPlayer ) return;

			float fSize = (float)pmsg->dwData2 / 100.f;
			if( fSize > 3.0f || fSize < .3f ) return;
			
			pPlayer->SetCharacterSize( fSize );

			MSG_DWORD2 msg;
			msg.Category	= MP_CHEAT;
			msg.Protocol	= MP_CHEAT_CHANGESIZE;
			msg.dwData1		= pmsg->dwData1;
			msg.dwData2		= pmsg->dwData2;

			PACKEDDATA_OBJ->QuickSend( pPlayer, &msg, sizeof( msg ) );
		}
		break;

	case MP_CHEAT_FISHING_FISHINGEXP:			Cheat_Fishing_Exp(pMsg);								break;
	case MP_CHEAT_FISHING_FISHPOINT:			Cheat_Fishing_FishPoint(pMsg);							break;
//-----------------------
	default:																							break;
	}
}


void Cheat_WhereIs_Syn(DWORD dwConnectionIndex, char* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	CPlayer* pSearcher	= (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	CPlayer* pTarget	= (CPlayer*)g_pUserTable->FindUser( pmsg->dwData );
	if( !pTarget )
	{
		MSG_BYTE msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_WHEREIS_NACK;
		msg.dwObjectID	= pmsg->dwObjectID;
		msg.bData		= CHATERR_ERROR;
		if( pSearcher )
			pSearcher->SendMsg( &msg, sizeof( msg ) );
		else
			g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );

		return;
	}

	MSG_WHERE_INFO msgWhere;
	msgWhere.Category	= MP_CHEAT;
	msgWhere.Protocol	= MP_CHEAT_WHEREIS_MAPSERVER_ACK;
	msgWhere.dwObjectID	= pmsg->dwObjectID;
	msgWhere.wMapNum	= g_pServerSystem->GetMapNum();
	msgWhere.bChannel	= (BYTE)pTarget->GetGridID();

	VECTOR3 TargetPos = *CCharMove::GetPosition(pTarget);
	msgWhere.cpos.Compress(&TargetPos);

	if( pSearcher )
		pSearcher->SendMsg( &msgWhere, sizeof( msgWhere ) );
	else
		g_Network.Send2Server( dwConnectionIndex, (char*)&msgWhere, sizeof( msgWhere ) );
}


void Cheat_Hide_Syn(char* pMsg) 
{
	MSG_NAME_DWORD* pmsg = (MSG_NAME_DWORD*)pMsg;
//			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
//			if(pPlayer == NULL)
//				return;
	CPlayer* pPlayer;

	if( pmsg->Name[0] == 0 )	//hero
		pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	else
		pPlayer = (CPlayer*)g_pUserTable->FindUserForName( pmsg->Name );
	if( !pPlayer )	return;
	
	if( pmsg->dwData == 1 )
	{
		if( pPlayer->IsVisible() == FALSE ) return;
		pPlayer->SetVisible(FALSE);
	}
	else
	{
		if( pPlayer->IsVisible() == TRUE )	return;
		pPlayer->SetVisible(TRUE);
	}

	if( pPlayer->GetUserLevel() <= eUSERLEVEL_GM )
	{
		char Msg[4096] = {0,};
		WORD MsgLen = 0;
		CGridTable* pGridTable = g_pServerSystem->GetGridSystem()->GetGridTable( pPlayer );
		if( !pGridTable )
			return;
		if( pPlayer->IsVisible() )
			pPlayer->SetAddMsg(Msg,&MsgLen,0,TRUE);
		else if( pPlayer->IsVisible() == FALSE )
			pPlayer->SetRemoveMsg( Msg, &MsgLen, 0 );
		if( MsgLen )
			PACKEDDATA_OBJ->QuickSendExceptObjectSelf( pPlayer, (MSGBASE*)Msg, MsgLen );
	}

	MSG_DWORD2 msg;
	msg.Category	= MP_CHEAT;
	msg.Protocol	= MP_CHEAT_HIDE_ACK;
	msg.dwObjectID	= pPlayer->GetID();
	msg.dwData1		= pPlayer->GetID();
	msg.dwData2		= pmsg->dwData;

	PACKEDDATA_OBJ->QuickSend( pPlayer, &msg, sizeof(MSG_DWORD2) );
}

/*
void Cheat_BanCharacter_Syn(char* pMsg) 
{
	MSG_NAME* pmsg = (MSG_NAME*)pMsg;
	CPlayer* pPlayer = g_pUserTable->FindUser(pmsg->dwObjectID);
	if(!pPlayer) break;
	CPlayer* pBanPlayer = 
}
*/

void Cheat_PkAllow_Syn(CPlayer* pSender, char* pMsg) 
{
	MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
	BOOL bAllow = (BOOL)pmsg->wData2;
	PKMGR->SetPKAllowWithMsg( bAllow );

	// 06.09.11 RaMa - GM툴 사용로그추가
	LogGMToolUse( pSender->GetID(), eGMLog_PKAllow, (DWORD)g_pServerSystem->GetMapNum(), (DWORD)bAllow, 0 );
}


void Cheat_MoveToChar_Syn(char* pMsg) 
{
	MSG_NAME_DWORD* pmsg = (MSG_NAME_DWORD*)pMsg;

	CObject* pPlayer = g_pUserTable->FindUser( pmsg->dwObjectID );
	if( !pPlayer ) return;
	CObject* pTarget = g_pUserTable->FindUserForName( pmsg->Name );			
	if( !pTarget ) return;
	
	VECTOR3 TargetPos;

	MOVE_POS msg;
	msg.Category	= MP_CHEAT;
	msg.Protocol	= MP_CHEAT_MOVE_ACK;

	if( pmsg->dwData == 0 )
	{
		TargetPos = *CCharMove::GetPosition(pTarget);
		CCharMove::SetPosition( pPlayer,&TargetPos);
		msg.cpos.Compress(&TargetPos);
		((CPlayer*)pPlayer)->SendMsg( &msg, sizeof(msg) );
	}
	else
	{
		TargetPos = *CCharMove::GetPosition(pPlayer);
		CCharMove::SetPosition( pTarget,&TargetPos);
		msg.cpos.Compress(&TargetPos);
		((CPlayer*)pTarget)->SendMsg( &msg, sizeof(msg) );
		//그리드에만 보내면 되는가?
	}

/*			
	MOVE_POS* pmsg = (MOVE_POS*)pMsg;
	VECTOR3 pos;
	CObject* pObject = g_pUserTable->FindUser(pmsg->dwObjectID);
	if(pObject == NULL)
		return;
	CObject* pTargetObject = g_pUserTable->FindUser(pmsg->dwMoverID);		// targetID
	if(pTargetObject == NULL)
		return;
	VECTOR3 TargetPos = *CCharMove::GetPosition(pTargetObject);
	CCharMove::SetPosition(pObject,&TargetPos);
	pmsg->cpos.Compress(&TargetPos);

	pmsg->Protocol = MP_CHEAT_MOVE_ACK;

	((CPlayer*)pObject)->SendMsg(pmsg,sizeof(MOVE_POS));
*/
}


void Cheat_Move_Syn(char* pMsg) 
{
	MOVE_POS* pmsg = (MOVE_POS*)pMsg;

	CObject* pObject = g_pUserTable->FindUser(pmsg->dwMoverID);
	if(pObject == NULL)
		return;

	VECTOR3 pos;
	pmsg->cpos.Decompress(&pos);
	
	//ASSERTVALID_POSITION(pos);
	if(CheckValidPosition(pos) == FALSE)
	{
		return;
	}

	CCharMove::SetPosition(pObject,&pos);

	pmsg->Protocol = MP_CHEAT_MOVE_ACK;

	((CPlayer*)pObject)->SendMsg(pmsg,sizeof(MOVE_POS));
}


void Cheat_MoveUser_Syn(char* pMsg) 
{
	MOVE_POS_USER* pmsg = (MOVE_POS_USER*)pMsg;

	CObject* pObject = g_pUserTable->FindUserForName(pmsg->Name);
	if(pObject == NULL)
		return;

	VECTOR3 pos;
	pmsg->cpos.Decompress(&pos);
	CCharMove::SetPosition(pObject,&pos);

	MOVE_POS posMsg;
	posMsg.Category = MP_CHEAT;
	posMsg.Protocol = MP_CHEAT_MOVE_ACK;
	posMsg.cpos.Compress(&pos);

	((CPlayer*)pObject)->SendMsg(&posMsg,sizeof(posMsg));
	//내 그리드에만 보낸다?
}


void Cheat_ChangeMap_Syn(char* pMsg) 
{
/*	MSG_WORD* pmsg = (MSG_WORD*)pMsg;
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	pmsg->Protocol = MP_CHEAT_CHANGEMAP_ACK;
	pPlayer->SendMsg(pmsg,sizeof(MSG_WORD));
*/
	MSG_NAME_DWORD2* pmsg = (MSG_NAME_DWORD2*)pMsg;
	CPlayer* pPlayer;

	if( pmsg->Name[0] == 0 )
		pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	else
		pPlayer = (CPlayer*)g_pUserTable->FindUserForName(pmsg->Name);

	if(pPlayer == NULL)
		return;

	//---KES PK 071202
	//---KES AUTONOTE
	if( pPlayer->IsPKMode() || LOOTINGMGR->IsLootedPlayer( pPlayer->GetID() ) || pPlayer->GetAutoNoteIdx() )
	{
		MSG_DWORD msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_CHANGEMAP_NACK;
		msg.dwData		= pPlayer->GetGridID() - 1;
		pPlayer->SendMsg( &msg, sizeof( msg ) );
		return;
	}
	//------------------

	/*
	if( CHANNELSYSTEM->GetChannelID( pmsg->dwData2 ) == 0 && !CHANNELSYSTEM->IsQuestMap() )
	{
		MSG_DWORD msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_CHANGEMAP_NACK;
		msg.dwData		= pPlayer->GetGridID() - 1;
		pPlayer->SendMsg( &msg, sizeof( msg ) );
		return;
	}
	*/
		
	MAPCHANGE_INFO* ChangeInfo = GAMERESRCMNGR->GetMapChangeInfoForDestMapNum( (WORD)pmsg->dwData1 );
	if( !ChangeInfo ) return;	//nackif( !ChangeInfo ) return;	//nack
	SaveMapChangePointUpdate( pPlayer->GetID(), ChangeInfo->Kind );

	// RaMa - 04.12.14
	pPlayer->UpdateLogoutToDB(FALSE);

	VECTOR3 RandPos;

	int temp;
	temp = rand() % 500 - 250;
	RandPos.x = ChangeInfo->MovePoint.x + temp;
	temp = rand() % 500 - 250;
	RandPos.z = ChangeInfo->MovePoint.z + temp;
	RandPos.y = 0;

	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.26
	// ..챌린지 존으로 이동할 경우 현재 위치를 저장한다. 챌린지 존에서 로그아웃되었을 경우, 다음에 접속 시의 위치로 사용함.
	if (g_csDateManager.IsChallengeZone(g_pServerSystem->GetMapNum()) == FALSE &&
		g_csDateManager.IsChallengeZone(ChangeInfo->MoveMapNum))
	{
		VECTOR3 stPos;
		pPlayer->GetPosition(&stPos);

		ChallengeZone_SavePos_Save(pPlayer->GetID(), g_pServerSystem->GetMapNum(), (DWORD)stPos.x, (DWORD)stPos.z);
	}
	// E 데이트 존 추가 added by hseos 2007.11.26
	//CharacterLogoutPointUpdate( pPlayer->GetID(), ChangeInfo->MoveMapNum, (DWORD)RandPos.x, (DWORD)RandPos.z );	

	pPlayer->SetMapMoveInfo( ChangeInfo->MoveMapNum, (DWORD)RandPos.x, (DWORD)RandPos.z);

	// 081218 LUJ, 플레이어를 해제하기 전에 DB에 업데이트할 주요 정보를 복사해놓는다
	const MAPTYPE	mapType			= MAPTYPE( pmsg->dwData1 );
	const WORD		channelIndex	= WORD( pmsg->dwData2 );
	const DWORD		playerIndex		= pPlayer->GetID();
	const DWORD		userIndex		= pPlayer->GetUserID();
	const MONEYTYPE	inventoryMoney	= pPlayer->GetMoney( eItemTable_Inventory );
	const MONEYTYPE	storageMoney	= pPlayer->GetMoney( eItemTable_Storage );

	g_pServerSystem->RemovePlayer( pPlayer->GetID() );

	// 081218 LUJ, 업데이트보다 맵 이동이 빨리 진행될 경우 업데이트되지 않은 정보가 오게된다.
	//			이를 막기 위해 프로시저 처리 완료 후 진행한다
	UpdatePlayerOnMapOut(
		playerIndex,
		userIndex,
		inventoryMoney,
		storageMoney,
		mapType,
		channelIndex );
}


void Cheat_Life_Syn()
{
}


void Cheat_Mp_Syn() 
{
}


void Cheat_Item_Syn(char* pMsg) 
{
	MSG_DWORD_WORD* pmsg = (MSG_DWORD_WORD*)pMsg;
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(!pPlayer) return;

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pmsg->dwData );
	if(!pItemInfo)		return;

/*	if( pItemInfo->ItemKind & eSHOP_ITEM )
	{
		ITEMMGR->CheatObtainShopItem( pPlayer, pmsg->dwData, pmsg->wData );
	}
	else
*/	{
		ITEMMGR->CheatObtainItem( pPlayer, pmsg->dwData, pmsg->wData );
	}

//			if(ITEMMGR->MonsterObtainItem(pPlayer, pmsg->wData1, 0) == 0)
//			{
		//if(pPlayer->GetPartyIdx())
		//{
		//	PARTYMGR->SendObtainItemMsg(pPlayer, pmsg->wData);	
		//}
//			}
}


//void Cheat_Item_Option_Syn(char* pMsg)
//{
//	SEND_CHEAT_ITEM_OPTION* pmsg = (SEND_CHEAT_ITEM_OPTION*)pMsg;
//	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
//	if(!pPlayer) return;
//
//	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pmsg->wItemIdx );
//	if(!pItemInfo)	return;
//
//	//CommonGameDefine.h 에 넣고 싶다.
//	enum{ eKindOption_None=0, eKindOption_Reinforce=1, eKindOption_Rare=2, };
//
//	//장착 아이템이 아니면 리턴.
////	if( !(pItemInfo->ItemKind & eEQUIP_ITEM) ) return;
//
//	//기존에 있는 아이템을 강화.
//	if( pmsg->wOptionKind & eKindOption_Reinforce )
//	{
//		//해당 아이템 정보를 가져와 기존 강화 옵션이 있으면 지운다.
//		const ITEMBASE * pTargetItemBase = ITEMMGR->GetItemInfoAbsIn(pPlayer, pmsg->bPosition);
//		if(pTargetItemBase->Durability != 0)
//			ItemOptionDelete(pTargetItemBase->Durability);
//
//		CItemSlot * pTargetSlot = pPlayer->GetSlot(pmsg->bPosition);
//		if( pTargetSlot->UpdateItemAbs(pPlayer, pmsg->bPosition, 0, 0, 0, 0, 0, UB_DURA, SS_PREINSERT ) != EI_TRUE )
//		{
//			return;
//		}
//		//치트 옵션을 등록한다.
//		//pmsg->OptionInfo.dwItemDBIdx = ;
//		//ItemReinfoceOptionInsert(pPlayer->GetID(), pTargetItemBase->dwDBIdx, pmsg->bPosition, pmsg->OptionInfo );				
//
//		// 06.09.12 RaMa
//		LogGMToolUse( pPlayer->GetID(), eGMLog_Item, eLog_ItemOptionObtainCheat, pmsg->wItemIdx, pTargetItemBase->dwDBIdx );
//	}
//	else if( pmsg->wOptionKind & eKindOption_Rare )
//	{
//		ITEMMGR->CheatObtainRareItem( pPlayer, pmsg->wItemIdx, 1, &pmsg->OptionInfo );
//	}
//}


void Cheat_Mana_Syn(char* pMsg)
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(!pPlayer) return;
	int tmp = pPlayer->GetMaxMana() - (pPlayer->GetMana()+pmsg->wData);
	if(tmp < 0)
		pPlayer->SetMana(pPlayer->GetMaxMana());
	else
		pPlayer->SetMana(pPlayer->GetMana()+pmsg->wData);
}


void Cheat_AddSkill_Syn(char* pMsg) 
{
/*	MSG_WORD3* pmsg = (MSG_WORD3*)pMsg;
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(!pPlayer) return;

	SKILLBASE* info = pPlayer->GetSkillBaseBySkillIdx(pmsg->wData1);
	if(info != NULL)
		return;

	MSGBASE msg;
	msg.Category = MP_CHEAT;
	if( SKILLTREEMGR->AddSkill(pPlayer, pmsg->wData1, pmsg->wData2) )
	{
		msg.Protocol = MP_CHEAT_ADDSKILL_ACK;
		LogCheat(pPlayer->GetID(),eCHT_AddSkill,pmsg->wData2,pmsg->wData1,0);
	}
	else
	{
		msg.Protocol = MP_CHEAT_ADDSKILL_NACK;
	}
	pPlayer->SendMsg( &msg, sizeof(msg) );
	*/
}


void Cheat_SkillLevel_Syn(char* pMsg) 
{
/*	MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(!pPlayer) return;

	SKILL_BASE* pSkill = pPlayer->GetSkillTree()->GetSkillBase( pmsg->wData1 );

	if( pSkill )
	{
		return;
	}
	else
	{
		SkillInsertToDB( pPlayer->GetID(), pmsg->wData1 );
	
		return;
	}

/*
	SKILLBASE* info = pPlayer->GetSkillBaseBySkillIdx(pmsg->wData1);
	if(info == NULL)
	{
		return;
	}

	pPlayer->SetPlayerSkillLevel(info->Position,(BYTE)pmsg->wData2,eLog_SkillCheatLevelup);
	LogCheat(pPlayer->GetID(),eCHT_SkillLevel,pmsg->wData2,pmsg->wData1,0);

	info->ExpPoint = 0;
	
	MSG_SKILLEXP msg;
	msg.Category	= MP_SKILLTREE;
	msg.Protocol	= MP_SKILLTREE_EXPPOINT_NOTIFY;
	msg.absPos		= info->Position;
	msg.ExpPoint	= info->ExpPoint;
	pPlayer->SendMsg(&msg,sizeof(msg));

	if(info->Level == 6 || info->Level == 11)
	{
		CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo(info->wIconIdx);
		if(pSkillInfo == NULL)
			return;
		pPlayer->AddExpPointToSkill(info->wIconIdx,pSkillInfo->GetNeedExp(info->Level),FALSE);
	}
*/
}


void Cheat_Update_Skill_Syn(char* pMsg) 
{
	MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(!pPlayer) return;

	cSkillTree* pSkillTree = pPlayer->GetSkillTree();
	if( !pSkillTree )
	{
		return;
	}

	SKILL_BASE* pSkillBase = pSkillTree->GetSkillBase( pmsg->wData1 );
	if( pSkillBase )
	{
		pSkillBase->Level = ( BYTE )( pmsg->wData2 );
		
		SKILLTREEMGR->UpdateSkill( pSkillBase, pPlayer );

		// 071109 웅주, 스킬 로그 추가
		InsertLogSkill( pPlayer, pSkillBase, eLog_SkillCheatLevelup );
	}
	else
	{
		SKILL_BASE skill;
		skill.dwDBIdx = 0;
		skill.wSkillIdx = pmsg->wData1;
		skill.Level = ( BYTE )( pmsg->wData2 );

		SKILLTREEMGR->AddSkill( pPlayer, &skill );
	}
}


void Cheat_Money_Syn(char* pMsg) 
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(!pPlayer) return;

	if( pPlayer->GetMoney() > pmsg->dwData )
	{
		pPlayer->SetMoney(pPlayer->GetMoney() - pmsg->dwData, MONEY_SUBTRACTION, 0, eItemTable_Inventory, eMoneyLog_GetCheatMoney, 0);
		// 06.09.12 RaMa
		LogGMToolUse( pPlayer->GetID(), eGMLog_Money, eMoneyLog_LoseGMTool, pPlayer->GetMoney() - pmsg->dwData, 0 );
	}
	else if( pPlayer->GetMoney() < pmsg->dwData )
	{
		pPlayer->SetMoney(pmsg->dwData - pPlayer->GetMoney(), MONEY_ADDITION, 0, eItemTable_Inventory, eMoneyLog_GetCheatMoney, 0);
		// 06.09.12 RaMa
		LogGMToolUse( pPlayer->GetID(), eGMLog_Money, eMoneyLog_GetGMTool, pmsg->dwData - pPlayer->GetMoney(), 0 );				
	}
	
	LogCheat(pPlayer->GetID(),eCHT_Money,pmsg->dwData,0,0);		
}


void Cheat_ReLoading() 
{
	g_pServerSystem->ReloadResourceData();
}


void Cheat_LevelUp_Syn(char* pMsg) 
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(!pPlayer) 
		return;
	EXPTYPE pointForLevel=0;
	LEVELTYPE curlvl = pPlayer->GetLevel();
	if(curlvl > pmsg->wData)
		return;
	else
	{
		LEVELTYPE diff = pmsg->wData - curlvl;
		for(LEVELTYPE i=0; i<diff; ++i)
		{
			pointForLevel = pointForLevel + GAMERESRCMNGR->GetMaxExpPoint(pPlayer->GetLevel()+i);
			
		}
		pPlayer->SetLevel(pmsg->wData-1);
		pPlayer->SetPlayerExpPoint(pointForLevel);
		pPlayer->SetPlayerExpPoint(1);					
		
		// YH
		CharacterHeroInfoUpdate(pPlayer);
		CharacterTotalInfoUpdate(pPlayer);		

		LogCheat(pPlayer->GetID(),eCHT_LevelUp,pmsg->wData,0,0);		
	}
}

void Cheat_PetLevelUp_Syn(char* pMsg) 
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(!pPlayer) 
		return;
	CPet* pPet = pPlayer->GetCurPet();
	if(!pPet)
		return;
	pPet->SetLevel( (BYTE)pmsg->wData );
}

// 070411 LYW --- MapNetworkMsgParser : Add function to level down.
void Cheat_LevelDown_Syn(char* pMsg) 
{
	/*
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(!pPlayer) 
		return;
	EXPTYPE pointForLevel=0;
	LEVELTYPE curlvl = pPlayer->GetLevel();
	if(curlvl < pmsg->wData)
		return;
	else
	{
		LEVELTYPE diff = curlvl - pmsg->wData ;
		for(LEVELTYPE i=0; i<diff; ++i)
		{
			pointForLevel = pointForLevel + GAMERESRCMNGR->GetMaxExpPoint(pPlayer->GetLevel()+i);
			
		}
		pPlayer->SetLevel(pmsg->wData);
		pPlayer->SetPlayerExpPointDown(pointForLevel);
		pPlayer->SetPlayerExpPointDown(1);					
		
		// YH
		CharacterHeroInfoUpdate(pPlayer);
		CharacterTotalInfoUpdate(pPlayer);		

		LogCheat(pPlayer->GetID(),eCHT_LevelUp,pmsg->wData,0,0);		
	}
	*/
}


/*어빌리티 삭제 - 이영준 
void Cheat_AbilityExp_Syn(char* pMsg)
{
	MSG_DWORD * pmsg = (MSG_DWORD*)pMsg;
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(!pPlayer) return;

	pPlayer->AddAbilityExp(pmsg->dwData);
	LogCheat(pPlayer->GetID(),eCHT_AbilityExp,pmsg->dwData,0,0);
}


*/
void Cheat_Event_Monster_Regen(char* pMsg) 
{
	MSG_EVENT_MONSTERREGEN* pmsg = (MSG_EVENT_MONSTERREGEN*)pMsg;
	CObject* pPlayer = g_pUserTable->FindUser(pmsg->dwObjectID);
	if(!pPlayer) return;
	RECALLMGR->EventRecall( pPlayer, pmsg->MonsterKind, pmsg->cbMobCount, pmsg->cbChannel, &pmsg->Pos, pmsg->wRadius, pmsg->ItemID, pmsg->dwDropRatio );

	// 06.09.12 RaMa
	LogGMToolUse( pPlayer->GetID(), eGMLog_Summon, pmsg->MonsterKind, pmsg->cbMobCount, pmsg->ItemID );
}


void Cheat_Event_Monster_Delete(char* pMsg) 
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	CObject* pPlayer = g_pUserTable->FindUser(pmsg->dwObjectID);
	if(!pPlayer) return;

	RECALLMGR->DeleteEventRecallMonster();

	// 06.09.12 RaMa
	LogGMToolUse( pPlayer->GetID(), eGMLog_SummonDelete, 0, 0, 0 );
}


void Cheat_Str_Syn(char* pMsg) 
{
	MSG_WORD *pmsg = (MSG_WORD*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(!pPlayer) return;
	if((pmsg->wData < 1) || (pmsg->wData > 9999))
		return;
	pPlayer->SetStrength(pmsg->wData);
	LogCheat(pPlayer->GetID(),eCHT_Str,pmsg->wData,0,0);
}


void Cheat_Dex_Syn(char* pMsg) 
{
	MSG_WORD *pmsg = (MSG_WORD*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(!pPlayer) return;
	if((pmsg->wData < 1) || (pmsg->wData > 9999))
		return;
	pPlayer->SetDexterity(pmsg->wData);
	LogCheat(pPlayer->GetID(),eCHT_Dex,pmsg->wData,0,0);
}


void Cheat_Vit_Syn(char* pMsg)
{
	MSG_WORD *pmsg = (MSG_WORD*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(!pPlayer) return;
	if((pmsg->wData < 1) || (pmsg->wData > 9999))
		return;
	pPlayer->SetVitality(pmsg->wData);
	LogCheat(pPlayer->GetID(),eCHT_Vit,pmsg->wData,0,0);
}


void Cheat_Wis_Syn(char* pMsg) 
{
	MSG_WORD *pmsg = (MSG_WORD*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(!pPlayer) return;
	if((pmsg->wData < 1) || (pmsg->wData > 9999))
		return;
	pPlayer->SetWisdom(pmsg->wData);
	LogCheat(pPlayer->GetID(),eCHT_Wis,pmsg->wData,0,0);
}
 
void Cheat_Int_Syn(char* pMsg)
{
	MSG_WORD *pmsg = (MSG_WORD*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(!pPlayer) return;
	if((pmsg->wData < 1) || (pmsg->wData > 9999))
		return;
	pPlayer->SetIntelligence(pmsg->wData);
	LogCheat(pPlayer->GetID(),eCHT_Wis,pmsg->wData,0,0);
}

void Cheat_Kill_AllMonster_Syn() 
{
	CObject* pInfo = NULL;
	g_pUserTable->SetPositionHead();
	while(pInfo = g_pUserTable->GetData())
	{
		if(pInfo->GetObjectKind() == eObjectKind_Monster)
		{
			if(pInfo->GetGridID() == 1)
			{
				//g_pServerSystem->RemoveMonster( pInfo->GetID() );
				pInfo->Die(NULL);

				// 06.09.12 RaMa
				LogGMToolUse( pInfo->GetID(), eGMLog_MonsterKill, MP_CHEAT_KILL_ALLMONSTER_SYN, pInfo->GetID(), 0 );
			}
		}
	}
}


void Cheat_PartyInfo_Syn(char* pMsg)
{
	MSG_DWORD * pmsg = (MSG_DWORD*)pMsg;
	CParty * pParty = PARTYMGR->GetParty(pmsg->dwData);
	PARTY_INFO SendParty;
	memset(&SendParty, 0, sizeof(PARTY_INFO));
	if(pParty)
	{
		SendParty.Category = MP_CHEAT;
		SendParty.Protocol = MP_CHEAT_PARTYINFO_ACK;
		SendParty.PartyDBIdx = pmsg->dwData;
		for(int i=0; i<MAX_PARTY_LISTNUM; ++i)
		{
			pParty->GetMemberInfo(i, &SendParty.Member[i]);
		}
		SendParty.Option = pParty->GetOption();
	}			
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(pPlayer)
	{
		pPlayer->SendMsg(&SendParty, sizeof(SendParty));
	}
}


// 080205 LYW --- MapNetworkMsgParser : 이밴트 적용, 전 맵에 처리 되도록 수정.
void Cheat_Event_Syn(char* pMsg) 
{
	if( !pMsg ) return ;														// 메시지 체크.

	MSG_WORD2* pmsg = NULL ;
	pmsg = (MSG_WORD2*)pMsg ;													// 메시지를 변환 받는다.

	if( !pmsg ) return ;														// 변환 된 메시지 체크.

	WORD wEventType = 0 ;
	wEventType = pmsg->wData1 ;													// 이벤트 종류를 받는다.

	WORD wEventRate = 0 ;
	wEventRate = pmsg->wData2 ;													// 이벤트율을 받는다.

	if( wEventRate > 3000 ) return ;											// 최대 30배 제한.

	if( wEventType == eEvent_ExpRate )											// 경험치를 적용한다.
	{
		gEventRate[eEvent_ExpRate] = wEventRate*0.01f;
		gEventRateFile[eEvent_ExpRate] = wEventRate*0.01f;
	}
	else if( wEventType == eEvent_ItemRate )									// 아이템 드랍률을 적용한다.
	{
		gEventRate[eEvent_ItemRate] = wEventRate*0.01f ;
		gEventRateFile[eEvent_ItemRate] = wEventRate*0.01f;
	}
	else if( wEventType == eEvent_MoneyRate )									// 머니 드랍률을 적용한다.
	{
		gEventRate[eEvent_MoneyRate] = wEventRate*0.01f;
		gEventRateFile[eEvent_MoneyRate] = wEventRate*0.01f;
	}

	// 080118 KTH -- 내용을 파일에 기록하기

	FILE* fp = NULL;
	fp = fopen("./System/Resource/droprate.txt", "wb");

	char lineBuf[128] ;															// 기록을 위한 임시 버퍼 선언 및 초기화.
	char dataBuf[2048] = {0, } ;

	if( !fp )																	// 열기에 실패 했다면, 에러 메시지 출력 후 return.
	{
		char errorBuf[64] = {0,} ;
		strcpy(errorBuf, "UpdateEventRate - Failed to create bin file.") ;		
		g_Console.Log(eLogDisplay, 4, errorBuf) ;
		return ;
	}

	for( int count = 0 ; count < eEvent_Max ; ++count )								
	{
		memset(lineBuf, 0, sizeof(lineBuf)) ;

		switch(count)
		{
		case eEvent_ExpRate :			sprintf(lineBuf, "#EXP\t%f\r\n",			gEventRate[eEvent_ExpRate]) ;			break ;
		case eEvent_AbilRate :			sprintf(lineBuf, "#ABIL\t%f\r\n",			gEventRate[eEvent_AbilRate]) ;			break ;
		case eEvent_ItemRate :			sprintf(lineBuf, "#ITEM\t%f\r\n",			gEventRate[eEvent_ItemRate]) ;			break ;
		case eEvent_MoneyRate :			sprintf(lineBuf, "#MONEY\t%f\r\n",			gEventRate[eEvent_MoneyRate]) ;			break ;
		case eEvent_GetMoney :			sprintf(lineBuf, "#GETMONEY\t%f\r\n",		gEventRate[eEvent_GetMoney]) ;			break ;
		case eEvent_DamageReciveRate :	sprintf(lineBuf, "#DAMAGERECIVE\t%f\r\n",	gEventRate[eEvent_DamageReciveRate]) ;	break ;
		case eEvent_DamageRate :		sprintf(lineBuf, "#DAMAGERATE\t%f\r\n",		gEventRate[eEvent_DamageRate]) ;		break ;
		case eEvent_ManaRate :			sprintf(lineBuf, "#MANASPEND\t%f\r\n",		gEventRate[eEvent_ManaRate]) ;			break ;
		case eEvent_UngiSpeed :			sprintf(lineBuf, "#UNGISPEED\t%f\r\n",		gEventRate[eEvent_UngiSpeed]) ;			break ;
		case eEvent_PartyExpRate :		sprintf(lineBuf, "#PARTYEXP\t%f\r\n",		gEventRate[eEvent_PartyExpRate]) ;		break ;
		case eEvent_SkillExp :			sprintf(lineBuf, "#SKILLEXPRATE\t%f\r\n",	gEventRate[eEvent_SkillExp]) ;			break ;
		}

		if( strlen(lineBuf) == 0 ) continue ;
		strcat(dataBuf, lineBuf) ;
	}

	fwrite(dataBuf, strlen(dataBuf), 1, fp);
	fclose(fp);
	/*
	else if( pmsg->wData1 == eEvent_ItemRate )
		gItemRate = pmsg->wData2*0.01f;
	else if( pmsg->wData1 == eEvent_MoneyRate )
		gMoneyRate = pmsg->wData2*0.01f;
	else if( pmsg->wData1 == eEvent_DamageReciveRate )
		gDamageReciveRate = pmsg->wData2*0.01f;
	else if( pmsg->wData1 == eEvent_DamageRate )
		gDamageRate = pmsg->wData2*0.01f;
	else if( pmsg->wData1 == eEvent_NaeRuykRate )
		gNaeRuykRate = pmsg->wData2*0.01f;
	else if( pmsg->wData1 == eEvent_UngiSpeed )
		gUngiSpeed = pmsg->wData2*0.01f;
	else if( pmsg->wData1 == eEvent_PartyExpRate )
		gPartyExpRate = pmsg->wData2*0.01f;
	else if( pmsg->wData1 == eEvent_AbilRate )
		gAbilRate = pmsg->wData2*0.01f;
	else if( pmsg->wData1 == eEvent_GetMoney )
		gGetMoney = pmsg->wData2*0.01f;
	*/
}


void Cheat_PlusTime_AllOff(char* pMsg) 
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;

//	g_bPlusTime = FALSE;

	// 모든플러스타임이 종료되어있는지 확인
	for(int i=0; i<eEvent_Max; ++i)
	{
		if( gEventRate[i] != gEventRateFile[i] )
			break;
	}
	
	if( i != eEvent_Max )
	{
		// 적용
		for(i=0; i<eEvent_Max; ++i)
			gEventRate[i] = gEventRateFile[i];
		
//		// 개인플러스타임 계산
////		CObject* pObject = NULL;
////		g_pUserTable->SetPositionHead();
////		while( pObject = g_pUserTable->GetData() )
////		{
////			if( pObject->GetObjectKind() != eObjectKind_Player )		continue;
//			
////			((CPlayer*)pObject)->GetShopItemManager()->CalcPlusTime( 0, MP_CHEAT_PLUSTIME_ALLOFF );
////		}
	}
}


void Cheat_PlusTime_On(char* pMsg) 
{
	// 080118 LYW --- MapNetworkMsgParser : GM툴로 처리 된 이벤트 종류의 범위를 체크.

	MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;

	if( pmsg->wData1 >= eEvent_Max ) return ;

//	g_bPlusTime = TRUE;

	// 다른에이전트에서 오는건 막는다.
	if( gEventRate[pmsg->wData1] != pmsg->wData2*0.01f )
		gEventRate[pmsg->wData1] = pmsg->wData2*0.01f;
	else
		return;

//	//MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
//
//	//g_bPlusTime = TRUE;
//
//	//// 다른에이전트에서 오는건 막는다.
//	//if( gEventRate[pmsg->wData1] != pmsg->wData2*0.01f )
//	//	gEventRate[pmsg->wData1] = pmsg->wData2*0.01f;
//	//else
//	//	return;
//
//	// 개인플러스타임 계산
////	CObject* pObject = NULL;
////	g_pUserTable->SetPositionHead();
////	while( pObject = g_pUserTable->GetData() )
////	{
////		if( pObject->GetObjectKind() != eObjectKind_Player )		continue;
//
////		((CPlayer*)pObject)->GetShopItemManager()->CalcPlusTime( pmsg->wData1, MP_CHEAT_PLUSTIME_ON );
////	}
}


void Cheat_PlusTime_Off(char* pMsg) 
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;

	// 다른에이전트 에서 오는건 막는다.
	if( gEventRate[pmsg->wData] != gEventRateFile[pmsg->wData] )
		gEventRate[pmsg->wData] = gEventRateFile[pmsg->wData];
	else
		return;

	// 모든플러스타임이 종료되었으면 플러스타임 종료
	for(int i=0; i<eEvent_Max; ++i)
	{
		if( gEventRate[i] != gEventRateFile[i] )
			break;
	}
//	if( i == eEvent_Max )			g_bPlusTime = FALSE;

//	// 개인플러스타임 계산
///*	CObject* pObject = NULL;
//	g_pUserTable->SetPositionHead();
//	while( pObject = g_pUserTable->GetData() )
//	{
//		if( pObject->GetObjectKind() != eObjectKind_Player )		continue;
//
//		((CPlayer*)pObject)->GetShopItemManager()->CalcPlusTime( pmsg->wData, MP_CHEAT_PLUSTIME_OFF );
//	}
//*/
}


void Cheat_MunpaDateClr_Syn(char* pMsg) 
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(pPlayer == 0)
		return;
	pPlayer->ClrGuildCanEntryDate();
}


void Cheat_Change_EventMap_Syn(char* pMsg) 
{
	MSG_NAME_DWORD3* pmsg = (MSG_NAME_DWORD3*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUserForName( pmsg->Name );
	if( !pPlayer )	return;

	DWORD dwParty = pPlayer->GetPartyIdx();
//			CPlayer* pData = NULL;
	CObject* pData = NULL;
	if( dwParty )
	{
		cPtrList list;
		g_pUserTable->SetPositionHead();
		while( pData = g_pUserTable->GetData() )
		{
			if( pData->GetObjectKind() != eObjectKind_Player )
				continue;

			if( dwParty == ((CPlayer*)pData)->GetPartyIdx() )
			{
				list.AddTail(pData);
				//g_pServerSystem->RemovePlayer( pData->GetID() );

				MSG_DWORD3 msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_MAP_OUT_TO_EVENTMAP;
				msg.dwObjectID = pData->GetID();
				msg.dwData1 = pmsg->dwData1;	// map
				msg.dwData2 = pmsg->dwData2;	// channel
				msg.dwData3 = pmsg->dwData3;	// team

				g_Network.Broadcast2AgentServer( (char*)&msg, sizeof(msg));
			}
		}
		PTRLISTPOS pos = list.GetHeadPosition();
		while( pos )
		{
			CObject* p = (CObject*)list.GetNext(pos);
			g_pServerSystem->RemovePlayer( p->GetID() );
		}
		list.RemoveAll();
	}
	else
	{
		g_pServerSystem->RemovePlayer( pPlayer->GetID() );

		MSG_DWORD3 msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_MAP_OUT_TO_EVENTMAP;
		msg.dwObjectID = pPlayer->GetID();
		msg.dwData1 = pmsg->dwData1;
		msg.dwData2 = pmsg->dwData2;
		msg.dwData3 = pmsg->dwData3;

		g_Network.Broadcast2AgentServer( (char*)&msg, sizeof(msg));
	}
}


void Cheat_Event_Start_Syn(char* pMsg) 
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	EVENTMAPMGR->StartEvent( pmsg->dwData2 );
}


void Cheat_Event_Ready_Syn(char* pMsg) 
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	EVENTMAPMGR->ReadyEvent( pmsg->dwData2 );
}


void Cheat_GetShopItem(char* pMsg) 
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;

	DWORD dur = 0;
	if( ITEMMGR->IsDupItem( pmsg->wData ) )
		dur = 1;

	ShopItemGetItem( pmsg->dwObjectID, pmsg->wData, dur );

	// 06.09.12 RaMa
	LogGMToolUse( pmsg->dwObjectID, eGMLog_Item, eLog_ShopItemGetCheat, pmsg->wData, dur );
}


/*	void Cheat_Stage_Syn(char* pMsg) 
{
	MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( !pPlayer )	return;
	/*어빌리티 삭제 - 			ChangeCharacterStageAbility( pPlayer->GetID(), pPlayer->GetStage(), pPlayer->GetAbilityGroup() );*/
	/*
	pPlayer->SetStage( pmsg->bData );	
}


*/
void Cheat_BossMob_Syn(char* pMsg) 
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( !pPlayer )	return;
	
//	QuestMainQuestInsert( pmsg->dwObjectID, 23, -33554432, 0 );
//	QuestSubQuestInsert( pmsg->dwObjectID, 23, 7, 0, 0 );
//	QuestSubQuestInsert( pmsg->dwObjectID, 23, 8, 0, 0 );
}


void Cheat_Jackpot_GetPrize(char* pMsg) 
{
//			MSG_NAME_DWORD2* pmsg = (MSG_NAME_DWORD2*)pMsg;
//			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUserForName( pmsg->Name );
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( !pPlayer ) return;

	if( 0 > pmsg->dwData && pmsg->dwData > 2 ) return;

	JACKPOTMGR->SendMsgDBPrizeInfo( pPlayer, pmsg->dwData );
//			JackpotPrizeInfo( m_dwMapJPMoney, pPlayer->GetID(), m_nPrizeKind, m_stPrize[m_nPrizeKind].m_dwPercent);
}


void Cheat_Jackpot_OnOff(char* pMsg) 
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			
	if( pmsg->dwData == 0 || pmsg->dwData == 1 )
	JACKPOTMGR->SetPrizeOnOff((BOOL)pmsg->dwData);
}


void Cheat_Jackpot_MoneyPerMonster(char* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	if( pmsg->dwData < 0 || pmsg->dwData > 1000 ) return;
	JACKPOTMGR->SetChipPerMon( pmsg->dwData );
}


void Cheat_Jackpot_Probability(char* pMsg)
{
	MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

	if( pmsg->dwData1 < 0 || pmsg->dwData1 > 2 ) return;
	if( pmsg->dwData2 < 0 || pmsg->dwData2 > 100000000 ) return;
	if( pmsg->dwData3 < 0 || pmsg->dwData3 > 100 ) return;

	JACKPOTMGR->SetPrizeProb( pmsg->dwData1, pmsg->dwData2, pmsg->dwData3 );
}


void Cheat_Jackpot_Control(char* pMsg) 
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			
	enum {TIMELENGTH, };	//CommonHeader 로~

	if( pmsg->dwData1 == TIMELENGTH )
	{
		if( pmsg->dwData2 < 0 || pmsg->dwData2 > 3600000 ) return;
		
		JACKPOTMGR->SetDBUpdateTimeLen( pmsg->dwData2 );
	}
}


void Cheat_GuildUnion_Clear(char* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( !pPlayer )	return;

	GUILDUNIONMGR->ClearEntryTime( pPlayer->GetGuildIdx() );
}


void Cheat_Mob_Chat(char* pMsg)
{
	TESTMSGID* pmsg = (TESTMSGID*)pMsg;
	CObject* pObject = g_pUserTable->FindUser( pmsg->dwSenderID );
	if( pObject == NULL ) return;

	char buf[MAX_CHAT_LENGTH+1];
	SafeStrCpy( buf, pmsg->Msg, MAX_CHAT_LENGTH+1 );

	if( pObject->GetObjectKind() & eObjectKind_Monster )
	{
		((CMonster*)pObject)->AddChat( buf );
	}
	else if( pObject->GetObjectKind() == eObjectKind_Npc )
	{
		((CNpc*)pObject)->AddChat( buf );
	}
}


void Cheat_Mussang_Ready(char* pMsg) 
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( !pPlayer ) return;

	pPlayer->CheatMussangReady();
}

void Cheat_GuildPoint_Syn(char* pMsg) 
{
	// TODO: 웅주: 치트 기능을 GM툴에 넣을 것...

	const MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

	CPlayer* player = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	
	if( player )
	{
		const DWORD guildIndex = player->GetGuildIdx();

		if( GUILDMGR->GetGuild( guildIndex ) )
		{
			const float score = float( pmsg->dwData3 );

			GuildAddScore( guildIndex, score );

			InsertLogGuildScore( guildIndex, score, eGuildLog_ScoreCheat );
		}
	}	

	//CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	//if( !pPlayer ) return;

	//MSG_DWORD rtMsg;
	//rtMsg.Category = MP_CHEAT;

	////
	//enum tempflag{Err_GP_NonePlayer = 0xfffffa, Err_GP_NoneGuild = 0xfffffb};
	//
	//CObject* pObj = g_pUserTable->FindUser( pmsg->dwData1 );
	//if( NULL == pObj || pObj->GetObjectKind() != eObjectKind_Player )
	//{
	//	rtMsg.Protocol = MP_CHEAT_GUILDPOINT_NACK;
	//	rtMsg.dwData = Err_GP_NonePlayer;
	//}
	//else
	//{
	//	CPlayer* pTargetPlayer = (CPlayer*)pObj;
	//	CGuild* pGuild = GUILDMGR->GetGuild(pmsg->dwData2);
	//	if( NULL == pGuild || pTargetPlayer->GetGuildIdx() != pmsg->dwData2 )
	//	{
	//		rtMsg.Protocol = MP_CHEAT_GUILDPOINT_NACK;
	//		rtMsg.dwData = Err_GP_NoneGuild;
	//	}
	//	//else
	//	//{
	//	//	//DB
	//	//	LogGuildPoint( pmsg->dwData2, pGuild->GetGuildPoint(), TRUE, eGM_CheatGuildPoint, pmsg->dwData3, pTargetPlayer->GetID(), pPlayer->GetID());

	//	//	GuildAddGuildPoint(pmsg->dwData2, pmsg->dwData3, eGM_CheatGuildPoint, pPlayer->GetID());

	//	//	rtMsg.Protocol = MP_CHEAT_GUILDPOINT_ACK;
	//	//}
	//}

	//pPlayer->SendMsg( &rtMsg, sizeof(rtMsg) );
}


//void Cheat_GuildHunted_MonsterCount_Syn(char* pMsg)
//{
//	MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
//
//	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
//	if( !pPlayer ) return;
//
//	MSG_DWORD rtMsg;
//	rtMsg.Category = MP_CHEAT;
//
//	//
//	enum tempflag{Err_GMC_NonePlayer = 0xfffffa, Err_GMC_NoneGuild = 0xfffffb};
//	
//	CObject* pObj = g_pUserTable->FindUser( pmsg->dwData1 );
//	if( NULL == pObj || pObj->GetObjectKind() != eObjectKind_Player )
//	{
//		rtMsg.Protocol = MP_CHEAT_GUILDHUNTED_MONSTERCOUNT_NACK;
//		rtMsg.dwData = Err_GMC_NonePlayer;
//	}
//	else
//	{
//		CPlayer* pTargetPlayer = (CPlayer*)pObj;
//		CGuild* pGuild = GUILDMGR->GetGuild(pmsg->dwData2);
//		if( NULL == pGuild || pTargetPlayer->GetGuildIdx() != pmsg->dwData2 )
//		{
//			rtMsg.Protocol = MP_CHEAT_GUILDHUNTED_MONSTERCOUNT_NACK;
//			rtMsg.dwData = Err_GMC_NoneGuild;
//		}
//		else
//		{
//			GuildAddHuntedMonsterCount(pmsg->dwData2, pmsg->dwData3);
//			//GUILDMGR->SendGuildHuntedMonsterTotalCountToMapServerExceptOneself(pmsg->dwData2, pmsg->dwData3);
//			rtMsg.Protocol = MP_CHEAT_GUILDHUNTED_MONSTERCOUNT_ACK;
//		}
//	}
//
//	pPlayer->SendMsg( &rtMsg, sizeof(rtMsg) );
//}


// 06. 05 HIDE NPC - 이영준
void Cheat_NpcHide_Syn(char* pMsg)
{
	MSG_WORD3* pmsg = (MSG_WORD3*)pMsg;

	// 숨김 가능 NPC 정보를 가져온다
	HIDE_NPC_INFO* pInfo = GAMERESRCMNGR->GetHideNpcInfo(pmsg->wData1);

	if(pInfo)
	{
		// 전체 채널
		if(pmsg->wData2 == 0)
		{
			for( DWORD i = 0 ; i < MAX_CHANNEL_NUM ; ++i )
				pInfo->ChannelInfo[i] = (BOOL)pmsg->wData3;

			CObject* pObject = NULL;
			g_pUserTable->SetPositionHead();

			MSG_WORD2 msg;
			msg.Category = MP_CHEAT;
			msg.Protocol = MP_CHEAT_NPCHIDE_ACK;
			msg.wData1 = pmsg->wData1;
			msg.wData2 = pmsg->wData3;

			while( pObject = g_pUserTable->GetData() )
			{
				if( pObject->GetObjectKind() == eObjectKind_Player )
					pObject->SendMsg(&msg, sizeof(msg));
			}
		}
		// 특정 채널
		else
		{
			pInfo->ChannelInfo[pmsg->wData2 - 1] = (BOOL)pmsg->wData3;
			
			DWORD dwChannelID = CHANNELSYSTEM->GetChannelID( pmsg->wData2 - 1 );

			if(dwChannelID)
			{
				CObject* pObject = NULL;
				g_pUserTable->SetPositionHead();

				MSG_WORD2 msg;
				msg.Category = MP_CHEAT;
				msg.Protocol = MP_CHEAT_NPCHIDE_ACK;
				msg.wData1 = pmsg->wData1;
				msg.wData2 = pmsg->wData3;

				while( pObject = g_pUserTable->GetData() )
				{
					if( pObject->GetObjectKind() == eObjectKind_Player )
						if( ((CPlayer*)pObject)->GetChannelID() == dwChannelID )
							pObject->SendMsg(&msg, sizeof(msg));
				}
			}
		}
	}
}

// 070429 LYW --- MapNetworkMsgParser : Add function to check hpmp determination for test.
//#ifdef _DEBUG
void Cheat_HPMP_Determination_Syn(char* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg ;

	// error msg 
	// 0 : There is no same hpmp determination.
	// 1 : It's not same class index.

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );

	if( !pPlayer )
	{
		return ;
	}
	else
	{
		DWORD dwClassIndex = pmsg->dwData ;

		CHARACTER_TOTALINFO totalInfo ;
		memset(&totalInfo, 0, sizeof(CHARACTER_TOTALINFO)) ;
		pPlayer->GetCharacterTotalInfo(&totalInfo) ;

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

		DWORD dwClassIdx = (base_class * 1000) + ( race * 100 ) + ( job_grade * 10 ) + cur_class ;

		if(dwClassIndex == dwClassIdx)
		{
			BYTE hpPoint = 0 ;
			BYTE mpPoint = 0 ;
			BOOL bResult = CHARCALCMGR->Get_HPMP_Point(pPlayer, &hpPoint, &mpPoint) ;

			if(bResult)
			{
				MSG_BYTE2 msg ;

				msg.Category	= MP_CHEAT ;
				msg.Protocol	= MP_CHEAT_HPMP_DETERMINATION_ACK ;
				msg.dwObjectID	= pmsg->dwObjectID ;
				msg.bData1 = hpPoint ;
				msg.bData2 = mpPoint ;

				pPlayer->SendMsg(&msg, sizeof(msg)) ;
			}
			else
			{
				MSG_BYTE msg ;
				msg.Category	= MP_CHEAT ;
				msg.Protocol	= MP_CHEAT_HPMP_DETERMINATION_NACK ;
				msg.dwObjectID	= pmsg->dwObjectID ;
				msg.bData		= 0 ;

				pPlayer->SendMsg(&msg, sizeof(msg)) ;
			}
		}
		else
		{
			MSG_BYTE msg ;
			msg.Category	= MP_CHEAT ;
			msg.Protocol	= MP_CHEAT_HPMP_DETERMINATION_NACK ;
			msg.dwObjectID	= pmsg->dwObjectID ;
			msg.bData		= 1 ;

			pPlayer->SendMsg(&msg, sizeof(msg)) ;
		}
	}
}

void Cheat_DropItem_Syn(char* pMsg)
{
#ifdef _DEBUG
	MSG_BYTE* pmsg = (MSG_BYTE*)pMsg ;
	ITEMDROP_OBJ->SetDebugStringOn(pmsg->bData) ;
	ITEMDROP_OBJ->SetDebugStringID(pmsg->dwObjectID) ;
#endif
}

void Cheat_SkillPoint_Syn(char* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	
	// 080611 LYW --- Player : 스킬포인트 업데이트 처리를 수정함.
	// (아이템으로 스킬 포인트를 추가하는 기능이 생겼기 때문.)
	//pPlayer->SetSkillPoint( pmsg->dwData );
	pPlayer->SetSkillPoint( pmsg->dwData, TRUE ) ;
}

void Cheat_God_Syn(char* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	
	pPlayer->SetGod( pmsg->dwData );
}




// 071106 LYW --- MapNetworkMsgParser : Add function to setting default hero stats.
void Cheat_Default_Stats(char* pMsg)										// 스탯 초기화 함수.
{
	ASSERT(!pMsg) ;															// 인자로 넘어오는 pMsg의 무결성 체크.

	MSGBASE* pmsg = NULL ;													// MSGBASE형 포인터를 선언하고 null.
	pmsg = (MSGBASE*)pMsg ;													// 포인터를 형 변환을 해서 받는다.

	if( !pmsg ) return ;		 											// 형 변환 받은 포인터가 유효하지 않으면, return.

	CPlayer* pPlayer = NULL ;												// CPlayer형 포인터를 선언하고 null.
	pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID) ;			// 오브젝트 아이디로 CPlayer 정보를 받는다.

	if( !pPlayer ) return ;													// CPlayer형 정보가 유효하지 않으면, return.

	ITEMMGR->UseItem_ResetStats(pPlayer, TRUE) ;							// 스탯 초기화 함수를 호출한다.
}





// 071106 LYW --- MapNetworkMsgParser : Add function to setting default hero skill.
void Cheat_Default_Skill(char* pMsg)										// 스킬 초기화 함수.
{
	ASSERT(!pMsg) ;															// 인자로 넘어오는 pMsg의 무결성 체크.

	MSGBASE* pmsg = NULL ;													// MSGBASE형 포인터를 선언하고 null.
	pmsg = (MSGBASE*)pMsg ;													// 포인터를 형 변환을 해서 받는다.

	if( !pmsg ) return ;		 											// 형 변환 받은 포인터가 유효하지 않으면, return.

	CPlayer* pPlayer = NULL ;												// CPlayer형 포인터를 선언하고 null.
	pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID) ;			// 오브젝트 아이디로 CPlayer 정보를 받는다.

	if( !pPlayer ) return ;	

	//---KES 스킬초기화돈	071207
	cSkillTree* pSkillTree = pPlayer->GetSkillTree();
	if( !pSkillTree ) return;

	DWORD dwTotalSkillMoney = 0;

	pSkillTree->SetPositionHead();
	while( SKILL_BASE* pSkill = pSkillTree->GetSkillBase() )
	{
		if( pSkill->dwDBIdx )	//배운 스킬
		{
			dwTotalSkillMoney += GAMERESRCMNGR->GetSkillMoney( pSkill->wSkillIdx + pSkill->Level - 1 );
		}
		else	//세트로 얻어진 스킬이므로 stored에서 찾아온다.
		{
			SKILL_BASE* pStoredSkill = pPlayer->GetStoredSkill( pSkill->wSkillIdx );
			if( pStoredSkill )
				dwTotalSkillMoney += GAMERESRCMNGR->GetSkillMoney( pStoredSkill->wSkillIdx + pStoredSkill->Level - 1 );
		}
	}

	pPlayer->SetRetrunSkillMoney( dwTotalSkillMoney );	
	//-----------------------------

	// 071129 LYW --- MapDBMsgParser : 스킬 초기화 쿼리 함수 추가.
	DB_ResetToDefaultSkill( pPlayer->GetID()) ;
}



//#endif //_DEBUG


void MP_QUICKMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	QUICKMNGR->NetworkMsgParse(pTempMsg->Protocol, pMsg);
/*
		
		switch(pTempMsg->Protocol)
		{
		case MP_QUICK_ADD_SYN:
			{
				MSG_QUICK_ADD_SYN * msg = (MSG_QUICK_ADD_SYN *)pMsg;
				CPlayer * player = (CPlayer *)g_pUserTable->FindUser(msg->dwObjectID);
				if(!player) return;
				QUICKMNGR->AddQuickItem(player, msg);
			}
			break;
		case MP_QUICK_USE_SYN:
			{
				MSG_QUICK_USE_SYN * msg = (MSG_QUICK_USE_SYN *)pMsg;
				CPlayer * player = (CPlayer *)g_pUserTable->FindUser(msg->dwObjectID);
				if(!player) return;
				player->UseQuickItem(msg->wQuickRelPosition);
			}
			break;
		case MP_QUICK_MOVE_SYN:
			{
				MSG_QUICK_MOVE_SYN * msg = (MSG_QUICK_MOVE_SYN *)pMsg;
				CPlayer * player = (CPlayer *)g_pUserTable->FindUser(msg->dwObjectID);
				if(!player) return;
				QUICKMNGR->MoveQuickItem(player, msg);
			}
			break;
		case MP_QUICK_REM_SYN:
			{
				MSG_QUICK_REM_SYN * msg = (MSG_QUICK_REM_SYN *)pMsg;
				CPlayer * player = (CPlayer *)g_pUserTable->FindUser(msg->dwObjectID);
				if(!player) return;
				
				QUICKMNGR->RemQuickItem(player, msg);
			}
			break;
		}*/
	
}

void MP_PARTYMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol) 
	{
	case MP_PARTY_ADD_SYN: 
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
//			PARTYMGR->AddParty(pmsg->dwData2,pmsg->dwObjectID,pmsg->dwData1); //include DB
			PARTYMGR->AddPartyInvite(pmsg->dwObjectID, pmsg->dwData1);
		}
		break;
	case MP_PARTY_DEL_SYN: 
		{
			MSG_DWORD * pmsg = (MSG_DWORD*)pMsg;
			PARTYMGR->DelMemberSyn(pmsg->dwObjectID, pmsg->dwData);
		}
		break;
	case MP_PARTY_CREATE_SYN: 
		{
			// 070314 LYW --- MapNetworkMsgParser : Modified MP_PARTY_CREATE_SYS part.
			/*
			MSG_DWORDBYTE * pmsg = (MSG_DWORDBYTE*)pMsg;
			
			CPlayer* pMaster = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pMaster == NULL)
				return;
			
			CPlayer* pMember = (CPlayer *)g_pUserTable->FindUser(pmsg->dwData);
			if(pMember == NULL)
				return;

//			PARTYMGR->CreatePartyQuery(pPlayer, pmsg->bData); //include DB
			PARTYMGR->CreateParty(pMaster, pMember, pmsg->bData); 
			*/

			MSG_DWORDBYTE* pmsg = (MSG_DWORDBYTE*) pMsg ;

			CPlayer* pMaster = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID ) ;

			if( pMaster == NULL ) return ;

			PARTYMGR->CreateParty( pMaster, (BYTE)pmsg->dwData ) ;
		}
		break;
		// 070110 LYW --- Add case for change pickup item from party part.
	case MP_PARTY_CHANGE_PICKUPITEM_SYN :
		{
			MSG_DWORD2 * pmsg = (MSG_DWORD2*)pMsg;
			CParty* pParty = PARTYMGR->GetParty(pmsg->dwData2) ;
			if( pParty == NULL )
			{
				PARTYMGR->SendErrMsg( pmsg->dwObjectID, eErr_Fail_Change_PickupItem, MP_PARTY_CHANGE_PICKUPITEM_NACK ) ;
			}
			else
			{
				//// 070315 LYW --- MapNetworkMsgParser : Add function to initialize party.
				//pParty->SetOption((BYTE)pmsg->dwData1 ) ;
				///*
				//CPlayer* pMaster = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID ) ;
				//pParty->Init(pParty->GetPartyIdx(), pMaster->GetID(), pMaster->GetObjectName(), pMaster, pmsg->dwData1 ) ;
				//*/

				pParty->SetOption((BYTE)pmsg->dwData1 ) ;

				MSG_BYTE msg ;
				msg.Category = MP_PARTY;
				msg.Protocol = MP_PARTY_CHANGE_PICKUPITEM_ACK;
				msg.bData	 = (BYTE)pmsg->dwData1 ;

				CPlayer* pMember;
				PARTY_MEMBER partyMember ;

				for(int i=0; i<MAX_PARTY_LISTNUM; ++i) 
				{
					memset(&partyMember, 0, sizeof(PARTY_MEMBER)) ;
					pParty->GetMemberInfo(i, &partyMember) ;

					if(partyMember.bLogged == TRUE ) 
					{
						pMember = (CPlayer*)g_pUserTable->FindUser(partyMember.dwMemberID);

						if(!pMember) continue;

						msg.dwObjectID = partyMember.dwMemberID;

						pMember->SendMsg(&msg, sizeof(MSG_BYTE));
					}
				}
				//PARTYMGR->SendPartyMsg( pmsg->dwData2, &msg, sizeof(msg) ) ;

				// 071002 LYW --- MapNetworkMsgParser : Modified process to change option of party.
				pParty->NotifyChangedOption((DWORD)pmsg->dwData1);					
			}
		}
		break ;

	case MP_PARTY_CHANGEMASTER_SYN: 
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			PARTYMGR->ChangeMasterParty(pmsg->dwData2,pmsg->dwObjectID,pmsg->dwData1); 
		}
		break;
	case MP_PARTY_BREAKUP_SYN: 
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			PARTYMGR->BreakupParty(pmsg->dwData,pmsg->dwObjectID); //include DB
		}
		break; 
	case MP_PARTY_BAN_SYN: 
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			PARTYMGR->BanParty(pmsg->dwData2,pmsg->dwObjectID,pmsg->dwData1); 
		}
		break;
	case MP_PARTY_INVITE_ACCEPT_SYN: 
		{
			MSG_DWORD * pmsg = (MSG_DWORD*)pMsg;
			PARTYMGR->AddMember(pmsg->dwData, pmsg->dwObjectID);
		}
		break;
	case MP_PARTY_INVITE_DENY_SYN: 
		{
			MSG_DWORD * pmsg = (MSG_DWORD*)pMsg;
			PARTYMGR->AddMemberNack(pmsg->dwData, pmsg->dwObjectID);
		}
		break;
	case MP_PARTY_NOTIFYMEMBER_LOGIN_TO_MAPSERVER:
		{
			SEND_CHANGE_PARTY_MEMBER_INFO* pmsg = (SEND_CHANGE_PARTY_MEMBER_INFO*)pMsg;
			PARTYMGR->NotifyUserLogIn(pmsg);
		}
		break;
	case MP_PARTY_NOTIFYMEMBER_LOGOUT_TO_MAPSERVER:
		{
			SEND_CHANGE_PARTY_MEMBER_INFO* pmsg = (SEND_CHANGE_PARTY_MEMBER_INFO*)pMsg;

			PARTYMGR->NotifyUserLogOut(pmsg);
		}
		break;
	case MP_PARTY_NOTIFYADD_TO_MAPSERVER:
		{
			SEND_CHANGE_PARTY_MEMBER_INFO* pmsg = (SEND_CHANGE_PARTY_MEMBER_INFO*)pMsg;
			PARTYMGR->NotifyAddParty(pmsg); 
		}
		break;
	case MP_PARTY_NOTIFYDELETE_TO_MAPSERVER: 
		{
			SEND_CHANGE_PARTY_MEMBER_INFO* pmsg = (SEND_CHANGE_PARTY_MEMBER_INFO*)pMsg;
			PARTYMGR->NotifyDelParty(pmsg);
		}
		break;
	case MP_PARTY_NOTIFYBAN_TO_MAPSERVER:
		{
			SEND_CHANGE_PARTY_MEMBER_INFO* pmsg = (SEND_CHANGE_PARTY_MEMBER_INFO*)pMsg;
			PARTYMGR->NotifyBanParty(pmsg); 
		}
		break;
	case MP_PARTY_NOTIFYCHANGEMASTER_TO_MAPSERVER:
		{
			SEND_CHANGE_PARTY_MEMBER_INFO* pmsg = (SEND_CHANGE_PARTY_MEMBER_INFO*)pMsg;
			PARTYMGR->NotifyChangeMasterParty(pmsg); 
		}
		break;
	case MP_PARTY_NOTIFYBREAKUP_TO_MAPSERVER:
		{
			SEND_CHANGE_PARTY_MEMBER_INFO* pmsg = (SEND_CHANGE_PARTY_MEMBER_INFO*)pMsg;
			PARTYMGR->NotifyBreakupParty(pmsg); 
		}
		break;
	case MP_PARTY_SYNDELETE_TO_MAPSERVER: 
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			PARTYMGR->NotifyDelSyn(pmsg);			
		}
		break;
	case MP_PARTY_NOTIFYCREATE_TO_MAPSERVER:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			PARTYMGR->NotifyCreateParty(pmsg->dwData);
		}
		break;
	case MP_PARTY_NOTIFYMEMBER_LOGINMSG:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			PARTYMGR->NotifyMemberLoginMsg(pmsg->dwData, pmsg->dwObjectID);
		}
		break;
	case MP_PARTY_NOTIFYMEMBER_LEVEL:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			PARTYMGR->DoMemberLevelUp(pmsg->dwData1, pmsg->dwData2, (LEVELTYPE)pmsg->dwData3);
		}
		break;

	// 071002 LYW --- MapNetworkMsgParser : Add process to change option of party.
	case MP_PARTY_NOTIFY_CHANGE_OPTION :
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg ;
			PARTYMGR->SendOptionMsg( pmsg ) ;
		}
		break ;

	//// 071003 LYW --- MapNetworkMsgParser : Add process to send msg to party about member log in.
	//case MP_PARTY_REAL_MEMBER_LOGIN :
	//	{
	//		MSGBASE* pmsg = (MSGBASE*)pMsg ;

	//		CPlayer* pMember = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID ) ;

	//		if( !pMember ) return ;

	//		CParty* pParty = PARTYMGR->GetParty(pMember->GetPartyIdx()) ;

	//		if( pParty == NULL )
	//		{
	//			PARTYMGR->SendErrMsg( pmsg->dwObjectID, eErr_Fail_Change_PickupItem, MP_PARTY_CHANGE_PICKUPITEM_NACK ) ;
	//		}
	//		else
	//		{
	//			for(int count = 0 ; count < MAX_PARTY_LISTNUM ; ++count )
	//			{
	//				//if( pmsg->dwObjectID == pParty->GetMemberInfo(
	//			}
	//		}
	//	}
	//	break ;
	} 
}

void MP_UNGIJOSIKMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;	
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pTempMsg->dwObjectID);
	
	if(pPlayer == NULL)
		return;
	
	switch(pTempMsg->Protocol)
	{
	case MP_UNGIJOSIK_START:
		UNGIMGR->UngijosikMode(pPlayer, TRUE);
		break;

	case MP_UNGIJOSIK_END:
		UNGIMGR->UngijosikMode(pPlayer, FALSE);
		break;
	}
}
// 070124 LYW --- PeaceWar : Delete peacewar part.
/*
void MP_PEACEWARMODEMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg	= (MSGBASE*)pMsg;
	CPlayer* pPlayer	= (CPlayer *)g_pUserTable->FindUser(pTempMsg->dwObjectID);
	
	if(pPlayer == NULL)
		return;

	switch(pTempMsg->Protocol) 
	{
	case MP_PEACEWARMODE_PEACE: 
		{
			PWMODMGR_OBJ->Peace_WarMode(pPlayer, TRUE);
		}
	break;

	case MP_PEACEWARMODE_WAR:
		{
			PWMODMGR_OBJ->Peace_WarMode(pPlayer, FALSE);
		}
		break;
	}
}
*/
void MP_STORAGEMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	STORAGEMGR->NetworkMsgParse(pmsg->Protocol,pMsg);
}

void MP_WANTEDMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	WANTEDMGR->NetworkMsgParse(pmsg->Protocol,pMsg);
}

void MP_JOURNALMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	JOURNALMGR->NetworkMsgParse(pmsg->Protocol,pMsg);
}

void MP_SkillMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	SKILLMGR->NetworkMsgParse(pmsg->Protocol,pMsg);
}

void MP_EXCHANGEMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	EXCHANGEMGR->NetworkMsgParse( pmsg->Protocol, pMsg );
}

void MP_STREETSTALLMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	STREETSTALLMGR->NetworkMsgParse( pmsg->Protocol, pMsg );
}

void MP_BattleMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	BATTLESYSTEM->NetworkMsgParse( pmsg->Protocol, pmsg );
}

void MP_REVIVEMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	if( EVENTMAPMGR->IsEventMap() )	return;

	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_CHAR_REVIVE_PRESENTSPOT_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if( pPlayer )
			{
				pPlayer->RevivePresentSpot();
			}
		}
		break;

	case MP_CHAR_REVIVE_VILLAGESPOT_SYN:
	case MP_CHAR_REVIVE_LOGINSPOT_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if( pPlayer )
			{
				pPlayer->ReviveLogIn();
			}
		}
		break;
	}
}


void MP_OPTIONMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	OPTIONMGR->NetworkMsgParse( pmsg->Protocol, pMsg );
}

void MP_NPCMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_NPC_SPEECH_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			CNpc* pNpc = (CNpc*)g_pUserTable->FindUser(pmsg->dwData1);

			MSG_DWORD2 msg = *pmsg;
			msg.Protocol = MP_NPC_SPEECH_ACK;
			//msg.dwObjectID = pmsg->dwObjectID;

			// error Check
			if(!pPlayer) return;

			BYTE BattleKind = pPlayer->GetBattle()->GetBattleKind();
			if( BattleKind != eBATTLE_KIND_NONE && BattleKind != eBATTLE_KIND_SIEGEWAR )
				goto SPEECH_NACK;

			// checkhacknpc
			if(pNpc == 0)
			{
				if( CheckHackNpc( pPlayer, (WORD)pmsg->dwData2 ) == FALSE )
				{
					goto SPEECH_NACK;
				}
			}
			else
			{
				VECTOR3 ObjectPos	= *CCharMove::GetPosition(pPlayer);
				VECTOR3 TObjectPos	= *CCharMove::GetPosition(pNpc);
				DWORD	Distance	= (DWORD)CalcDistanceXZ( &ObjectPos, &TObjectPos );
				if(Distance > 3000.0f)	//1000->3000
				{
					goto SPEECH_NACK;
				}
				if(pNpc->GetNpcJob() == BOMUL_ROLE)
				{
					DWORD PlayerID = pNpc->GetFirstTalkCharacterID();
					if(PlayerID)
					{
						CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(PlayerID);
						if(pPlayer == 0) // maybe?
						{
							ASSERT(0); 
							pNpc->Die(NULL);
						}

						goto SPEECH_NACK;
					}
				}
				if(pNpc->GetState() == eObjectState_Die)
					goto SPEECH_NACK;
				if(pNpc->GetNpcJob() == BOMUL_ROLE)
					pNpc->SetFirstTalkCharacterID(pmsg->dwObjectID);
			}

			// State Check
			if( !OBJECTSTATEMGR_OBJ->StartObjectState(pPlayer, eObjectState_Deal, 0) ) goto SPEECH_NACK;

			pPlayer->SendMsg(&msg, sizeof(msg));
					
			return;

SPEECH_NACK:
			MSG_DWORD NackMsg;
			NackMsg.Category = MP_NPC;
			NackMsg.Protocol = MP_NPC_SPEECH_NACK;
			NackMsg.dwObjectID = pmsg->dwObjectID;
			NackMsg.dwData = pmsg->dwData2;
			pPlayer->SendMsg(&NackMsg, sizeof(NackMsg));
		}
		break;
	case MP_NPC_CLOSEBOMUL_SYN:
		{
			MSG_DWORD * pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == 0)
				return;
			CNpc* pNpc = (CNpc*)g_pUserTable->FindUser(pmsg->dwData); //pmsg->dwData : NpcObjectIdx

			// 검사
			if((pNpc == 0) || (pNpc->GetFirstTalkCharacterID() != pPlayer->GetID()))
				return;
			else
			{				
				pNpc->SetFirstTalkCharacterID(0);
			}			
		}
		break;
	case MP_NPC_OPENBOMUL_SYN:
		{
			MSG_DWORD * pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == 0)
				return;
			CNpc* pNpc = (CNpc*)g_pUserTable->FindUser(pmsg->dwData); //pmsg->dwData : NpcObjectIdx
			if((pNpc == 0) || (pNpc->GetFirstTalkCharacterID() != pPlayer->GetID()))
			{
				ASSERT(0);
				return;
			}
			pNpc->DoDie(pPlayer);
		}
		break;
	case MP_NPC_DOJOB_SYN:
		{
			MSG_DWORD * pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == 0)
				return;
			CNpc* pNpc = (CNpc*)g_pUserTable->FindUser(pmsg->dwData); //pmsg->dwData : NpcObjectIdx
			if(pNpc == 0)
				return;
			pNpc->DoJob(pPlayer, dwConnectionIndex);			
		}
		break;

		// 080514 KTH -- npc action check hack
	case MP_NPC_CHECK_HACK_SYN:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			// pmsg->dwData1 = NPC Idx
			// pmsg->dwData2 = NPC UniqueIdx
			// pmsg->dwData3 = Action Value
			MSG_DWORD3 msg = *pmsg;
			msg.Protocol = MP_NPC_CHECK_HACK_ACK;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if( pPlayer == 0 )
				return;
			CNpc* pNpc = (CNpc*)g_pUserTable->FindUser(pmsg->dwData1);
			
			if(pNpc == 0)
			{
				if( CheckHackNpc( pPlayer, (WORD)pmsg->dwData2 ) == FALSE )
				{
					goto FIND_HACK;
				}
			}
			else
			{
				VECTOR3 ObjectPos	= *CCharMove::GetPosition(pPlayer);
				VECTOR3 TObjectPos	= *CCharMove::GetPosition(pNpc);
				DWORD	Distance	= (DWORD)CalcDistanceXZ( &ObjectPos, &TObjectPos );
				if(Distance > 3000.0f)	//1000->3000
				{
					goto FIND_HACK;
				}
			}

			pPlayer->SendMsg(&msg, sizeof(msg));
			return;
FIND_HACK:
			MSG_DWORD HackMsg;
			HackMsg.Category = MP_NPC;
			HackMsg.Protocol = MP_NPC_CHECK_HACK_NACK;
			HackMsg.dwObjectID = pmsg->dwObjectID;
			HackMsg.dwData = pmsg->dwData2;
			pPlayer->SendMsg(&HackMsg, sizeof(HackMsg));
		}
		break;
	}
}

void MP_MurimnetMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	//////////////////////////////////////////////////////////////////////////
	// 일반 맵서버에서 받는 부분
	case MP_MURIMNET_CHANGETOMURIMNET_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)
				return;			
			
			g_pServerSystem->RemovePlayer(pTempMsg->dwObjectID);

			MSGBASE msg;
			msg.Category = MP_MURIMNET;
			msg.Protocol = MP_MURIMNET_CHANGETOMURIMNET_ACK;
			msg.dwObjectID = pmsg->dwObjectID;
			
			g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));
		}
		break;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 베틀 서버에서 받는 부분
	case MP_MURIMNET_PLAYROOM_INFO_SYN:
		{
/*			MSG_MNPLAYROOM_INFO* pmsg = (MSG_MNPLAYROOM_INFO*)pMsg;
			
			MSG_BATTLESETTING_SYN_MUNPAFIELD binfo;
			memset(&binfo,0,sizeof(MSG_BATTLESETTING_SYN_MUNPAFIELD));
			binfo.BattleID = pmsg->nIndex;
			ASSERT(binfo.BattleID != 0);
			binfo.BattleKind = pmsg->nGameKind;
			for(int n=0;n<pmsg->nTotalPlayerNum;++n)
			{
				BYTE Team = pmsg->PlayerInfo[n].cbTeam;
				if(Team == 2)	// 옵저버
				{

				}
				else
				{
					binfo.TeamInfo[Team].AddTeamMember(pmsg->PlayerInfo[n].dwObjectID);
				}
			}
			if(BATTLESYSTEM->CreateBattle(&binfo) == FALSE)
			{	// 베틀 생성 실패
				ASSERT(0);
			}

			// 성공 메세지 무림넷에 보낸다.
			pmsg->Protocol = MP_MURIMNET_PLAYROOM_INFO_ACK;
			PACKEDDATA_OBJ->SendToMapServer(dwConnectionIndex,99,pmsg,dwLength);
*/	
			MSG_MNPLAYROOM_INFO* pmsg = (MSG_MNPLAYROOM_INFO*)pMsg;
			
			MSG_BATTLESETTING_SYN_MURIMFIELD bInfo;
			memset( &bInfo, 0, sizeof(MSG_BATTLESETTING_SYN_MURIMFIELD) );
			bInfo.BattleID = pmsg->nIndex;
			ASSERT(bInfo.BattleID != 0);
			bInfo.BattleKind = eBATTLE_KIND_MURIMFIELD;
			bInfo.MurimBattleInfo.wMurimBattleKind = (WORD)pmsg->nGameKind;
			for( int n = 0; n < pmsg->nTotalPlayerNum; ++n )
			{
				BYTE Team = pmsg->PlayerInfo[n].cbTeam;
				if(Team == 2)	// 옵저버
				{
				}
				else
				{
					bInfo.TeamInfo[Team].AddTeamMember( pmsg->PlayerInfo[n].dwObjectID );
				}
			}
			if( BATTLESYSTEM->CreateBattle( &bInfo, pmsg->wMapNum ) == FALSE )
			{	// 베틀 생성 실패
				ASSERT(0);
			}

			// 성공 메세지 무림넷에 보낸다.
			pmsg->Protocol = MP_MURIMNET_PLAYROOM_INFO_ACK;
			PACKEDDATA_OBJ->SendToMapServer( dwConnectionIndex, 99, pmsg, (WORD)dwLength );
		}
		break;
		
	case MP_MURIMNET_RETURNTOMURIMNET_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)
				return;

			MSGBASE msg;
			msg.Category = MP_MURIMNET;
			msg.Protocol = MP_MURIMNET_RETURNTOMURIMNET_ACK;
			pPlayer->SendMsg(&msg,sizeof(msg));

			g_pServerSystem->RemovePlayer(pmsg->dwObjectID);
		}
		break;
	//////////////////////////////////////////////////////////////////////////

		
	}
}

void MP_QUESTMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	QUESTMGR->NetworkMsgParse( pTempMsg->Protocol, pMsg );

	switch(pTempMsg->Protocol)
	{
	case MP_QUEST_CHANGESTATE:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)
			{
//				char buff[256] = {0,};
//				sprintf(buff, "여기뜨면 법석한테 연락부탁드려염 [Client ID : %d]", pmsg->dwObjectID);
//				ASSERTMSG(0, buff);
				return;
			}		

			pPlayer->SetQuestState(pmsg->dwData1, pmsg->dwData2);
			
		}
		break;
/*		
	case MP_QUEST_TEST_START_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if( pPlayer )
			{
				MSG_DWORD msg;
				msg.Category = MP_QUEST;
				msg.Protocol = MP_QUEST_TEST_START_ACK;
				msg.dwObjectID = pmsg->dwObjectID;
				msg.dwData = pmsg->dwData;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				
//				pPlayer->AddQuestScript( pmsg->dwData );
				return;
			}
			else
			{
				return;
			}
		}
		break;
*/
	}

}

void MP_PKMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	PKMGR->NetworkMsgParse( pmsg->Protocol, pMsg );	
}


void MP_HACKCHECKMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;

	switch( pTempMsg->Protocol )
	{
	case MP_HACKCHECK_BAN_USER:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if( pPlayer == NULL ) return;
			
			if( pmsg->dwData )
				InsertSpeedHackCheck( pPlayer->GetID(), pPlayer->GetObjectName(), pmsg->dwData, 0 );
		}
		break;
	}
}

void MP_SOCIETYACTMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;

	switch( pTempMsg->Protocol )
	{
	case MP_SOCIETYACT_ACT_SYN:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if( pPlayer == NULL ) return;
			
			if( pPlayer->CanSocietyAction( gCurTime ) == FALSE )
				return;

			MSG_DWORD_WORD msg;
			msg.Category	= MP_SOCIETYACT;
			msg.Protocol	= MP_SOCIETYACT_ACT;
			msg.dwObjectID	= pPlayer->GetID();
			msg.dwData		= pPlayer->GetID();
			msg.wData		= pmsg->wData;
			
			PACKEDDATA_OBJ->QuickSendExceptObjectSelf( pPlayer, &msg, sizeof(msg) );
			
			//동작을 했으므로 무적상태 풀어줌
			if( pPlayer->GetState() == eObjectState_Immortal )
				OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer, eObjectState_Immortal, 0 );
		}
		break;
	}
}


void MP_GUILDMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	GUILDMGR->NetworkMsgParse(pmsg->Protocol,pMsg);
}

void MP_GUILDFIELDWARMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	GUILDWARMGR->NetworkMsgParse(pmsg->Protocol,pMsg);
}

void MP_PARTYWARMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	PARTYWARMGR->NetworkMsgParse(pmsg->Protocol, pMsg);
}

void MP_GTOURNAMENTMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	GTMGR->NetworkMsgParse( dwConnectionIndex, pmsg->Protocol, pMsg );
}

void MP_GUILUNIONMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	GUILDUNIONMGR->NetworkMsgParse( pmsg->Protocol, pMsg );
}

void MP_SIEGEWARMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	SIEGEWARMGR->NetworkMsgParse( dwConnectionIndex, pmsg->Protocol, pMsg );
}

void MP_SIEGEWARPROFITMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	SWPROFITMGR->NetworkMsgParse( dwConnectionIndex, pmsg->Protocol, pMsg );
}

// desc_hseos_소스분석용테스트_01
// S 소스분석용테스트 추가 added by hseos 2007.04.05
void MP_HSEOS_TESTMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
//	MessageBox(NULL, "클라이언트로부터 메세지 수신됨. 확인을 누르면 클라이언트에게 메세지를 보냄", NULL, NULL);
	// 클라이언트에 메세지 보냄
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pTempMsg->dwObjectID);
	if(!pPlayer) return;

//	PACKEDDATA_OBJ->QuickSend(pPlayer,pTempMsg,dwLength);
	pPlayer->SendMsg(pTempMsg, dwLength);
}
// E 소스분석용테스트 추가 added by hseos 2007.04.05

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.04.11
void MP_FARM_MsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	g_csFarmManager.SRV_ParseRequestFromClient(dwConnectionIndex, pMsg, dwLength);
}
// E 농장시스템 추가 added by hseos 2007.04.11

// desc_hseos_주민등록01
// S 주민등록 추가 added by hseos 2007.06.04
void MP_RESIDENTREGIST_MsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	g_csResidentRegistManager.SRV_ParseRequestFromClient(dwConnectionIndex, pMsg, dwLength);
}
// E 주민등록 추가 added by hseos 2007.06.04

void MP_FAMILYMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	g_csFamilyManager.SRV_ParseRequestFromClient(dwConnectionIndex, pMsg, dwLength);
}

// desc_hseos_데이트 존_01
// S 데이트 존 추가 added by hseos 2007.11.19
void MP_DATE_MsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	g_csDateManager.SRV_ParseRequestFromClient(dwConnectionIndex, pMsg, dwLength);
}
// E 데이트 존 추가 added by hseos 2007.11.19

// desc_hseos_결혼_01
// S 결혼 추가 added by hseos 2007.12.11
void MP_MARRIAGE_MsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
//	g_csMarriageManager.SRV_ParseRequestFromClient(dwConnectionIndex, pMsg, dwLength);
}
// E 결혼 추가 added by hseos 2007.12.11

// Weather System
void MP_WEATHERMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;

//	WEATHERMGR->NetworkMsgParse( pmsg->Protocol, pMsg );
}

void MP_AUTONOTE_MsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	AUTONOTEMGR->NetworkMsgParse( pmsg->Protocol, pMsg, dwLength );
}

void MP_FISHING_ServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	FISHINGMGR->NetworkMsgParse( pmsg->Protocol, pMsg, dwLength );
}

void MP_PET_MsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	PETMGR->NetworkMsgParse( pmsg->Protocol, pMsg, dwLength );
}

// 070228 LYW --- MapNetworkMsgParser : Add message parser for facial part.
void MP_FACIALMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSG_BYTE* pTempMsg = ( MSG_BYTE* )pMsg ;

	switch( pTempMsg->Protocol )
	{
	case MP_FACIAL_NORMAL :
		{
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pTempMsg->dwObjectID);
			if(!pPlayer) return;
			
			PACKEDDATA_OBJ->QuickSend(pPlayer,pTempMsg,dwLength);
		}
		break ;
	}
}

// 070228 LYW --- MapNetworkMsgParser : Add message parser for emotion part.
void MP_EMOTIONMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSG_INT* pTempMsg = ( MSG_INT* )pMsg ;

	switch( pTempMsg->Protocol )
	{
	case MP_EMOTION_NORMAL :
		{
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pTempMsg->dwObjectID);
			if(!pPlayer) return;
			
			PACKEDDATA_OBJ->QuickSend(pPlayer,pTempMsg,dwLength);
		}
		break ;

	// 070502 LYW --- MapNetworkMsgParser : Add message to setting rest mode.
	case MP_EMOTION_SITSTAND_SYN :
		{
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pTempMsg->dwObjectID);
			if( pPlayer )
			{
				if( pPlayer->IsShowdown() || pPlayer->IsPKMode() )
				{
					return;
				}

				MSG_BYTE* message = ( MSG_BYTE* )pTempMsg;

				pTempMsg->Protocol = MP_EMOTION_SITSTAND_ACK ;
				PACKEDDATA_OBJ->QuickSend(pPlayer,pTempMsg,dwLength);

				pPlayer->SetState( message->bData ? eObjectState_Rest : eObjectState_None );
			}
		}
		break ;
	// 070502 LYW --- MapNetworkMsgParser : Add message to setting ing motion.
	case MP_EMOTION_ING_MOTION_SYN :
		{
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pTempMsg->dwObjectID);
			if(!pPlayer) return;
			
			pTempMsg->Protocol = MP_EMOTION_ING_MOTION_ACK ;
			PACKEDDATA_OBJ->QuickSend(pPlayer,pTempMsg,dwLength);
		}
		break ;
	// 070528 LYW --- MapNetworkMsgParser : Add message to play emoticon part.
	case MP_EMOTICON_SYN :
		{
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pTempMsg->dwObjectID);
			if(!pPlayer) return;
			
			pTempMsg->Protocol = MP_PLAY_EMOTICON_ACK ;
			PACKEDDATA_OBJ->QuickSend(pPlayer,pTempMsg,dwLength);
		}
		break ;
	}
}

BOOL CheckHackNpc( CPlayer* pPlayer, WORD wNpcIdx )
{
	STATIC_NPCINFO* pInfo = GAMERESRCMNGR->GetStaticNpcInfo( wNpcIdx );
	if( pInfo == NULL )
	{
		return FALSE;
	}
	
	VECTOR3 vPos;
	pPlayer->GetPosition( &vPos );
	float dist = CalcDistanceXZ( &vPos, &pInfo->vPos );
	if( dist > 3000.0f )
	{
		ASSERT(0); //TL임시
		return FALSE;
	}

	return TRUE;
}

// 071023 LYW --- MapNetworkMsgParser : Add message parser for tutorial part.
void MP_TUTORIALMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSG_INT* pmsg = ( MSG_INT* )pMsg ;

	switch( pmsg->Protocol )
	{
	case MP_TUTORIAL_UPDATE_SYN :
		{
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if( pPlayer == NULL )	return;

			TutorialUpdateToDB( pmsg->dwObjectID, pmsg->nData, TRUE ) ;
		}
		break ;
	}
}

// 071212 KTH -- 확장 인벤토리를 초기화 하여 주는 치트키
void Cheat_Reset_Inventory(char* pMsg) 
{
	MSGBASE* pmsg = (MSG_DWORD*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);

	ResetCharacterInventory(pPlayer->GetID());

	pPlayer->SetInventoryExpansion(0);

	pmsg->Protocol = MP_CHEAT_RESET_INVENTORY_ACK;

	pPlayer->SendMsg(pmsg, sizeof(MSGBASE) );
}

// 080115 KTH -- PVP Damage Ratio
void Cheat_PvP_Damage_Rate(char* pMsg)
{
	MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);

	ATTACKMGR->SetPvpDamageRatio(pmsg->bData);

	pmsg->Protocol = MP_CHEAT_PVP_DAMAGE_RATE_ACK;

	pPlayer->SendMsg(pmsg, sizeof(MSG_BYTE) );
}
/*
// desc_hseos_결혼_01
// S 결혼 추가 added by hseos 2008.02.01
void Cheat_Marriage_Clear_DivorceDate(char* pMsg)
{
	MSGBASE* pmsg = (MSG_DWORD*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);

	char szTxt[256] = {0,};
	pPlayer->SetMarriageDivorceDate(szTxt);
}
// E 결혼 추가 added by hseos 2008.02.01
*/
void Cheat_Fishing_Exp(char* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*) pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	pPlayer->SetFishingExp(pmsg->dwData);

	// 080602 LYW --- Player : 경험치 수치 (__int32) 에서 (__int64) 사이즈로 변경 처리.
	//MSG_DWORD_WORD msgFishingExp;
	//msgFishingExp.Category = MP_FISHING;
	//msgFishingExp.Protocol = MP_FISHING_EXP_ACK;
	//msgFishingExp.dwObjectID = pPlayer->GetID();
	//msgFishingExp.wData    = pPlayer->GetFishingLevel();
	//msgFishingExp.dwData   = pPlayer->GetFishingExp();
	//pPlayer->SendMsg( &msgFishingExp, sizeof( msgFishingExp ) );


	MSG_DWORDEX2 msgFishingExp;
	msgFishingExp.Category = MP_FISHING;
	msgFishingExp.Protocol = MP_FISHING_EXP_ACK;
	msgFishingExp.dwObjectID = pPlayer->GetID();
	msgFishingExp.dweData1 = (DWORDEX)pPlayer->GetFishingLevel();
	msgFishingExp.dweData2  = pPlayer->GetFishingExp();
	pPlayer->SendMsg( &msgFishingExp, sizeof( msgFishingExp ) );
}

void Cheat_Fishing_FishPoint(char* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*) pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	pPlayer->SetFishPoint(pmsg->dwData);

	MSG_DWORD msgFishPoint;
	msgFishPoint.Category = MP_FISHING;
	msgFishPoint.Protocol = MP_FISHING_POINT_ACK;
	msgFishPoint.dwData	= pPlayer->GetFishPoint();
	pPlayer->SendMsg(&msgFishPoint, sizeof(msgFishPoint));

	// 080808 LUJ, 낚시 경험치/레벨 추가
	Log_Fishing(
		pPlayer->GetID(),
		eFishingLog_SetPointFromCheat,
		pPlayer->GetFishPoint(),
		pPlayer->GetFishPoint(),
		0,
		0,
		pPlayer->GetFishingExp(),
		pPlayer->GetFishingLevel() );
}
/*
void Cheat_Clear_Inventory(char* pMsg)
{
	MSGBASE* pmsg = (MSG_DWORD*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);

	if( pPlayer->ClearInventory() )
	{
		pmsg->Protocol = MP_CHEAT_CELAR_INVENTORY_ACK;
		pPlayer->SendMsg(pmsg, sizeof(MSG_BYTE));
	}
}
*/
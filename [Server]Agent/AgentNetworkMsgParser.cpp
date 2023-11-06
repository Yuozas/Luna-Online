
#include "stdafx.h"
#include "AgentNetworkMsgParser.h"
#include "ServerTable.h"
#include "Network.h"
#include "AgentDBMsgParser.h"
#include "MsgTable.h"

#include "FilteringTable.h"
#include "BootManager.h"
#include "MHTimeManager.h"
#include "GMPowerList.h"
#include "ShoutManager.h"
#include "PlustimeMgr.h"

//#include "JackpotManager_Agent.h"
#include "SkillDalayManager.h"
#include "TrafficLog.h"

#include "DateMatchingManager.h"					// ����Ʈ ��Ī

// desc_hseos_�йи�01
// S �йи� �߰� added by hseos 2007.07.09
#include "../hseos/Family/SHFamilyManager.h"
// E �йи� �߰� added by hseos 2007.07.09

// desc_hseos_����Ʈ ��_01
// S ����Ʈ �� �߰� added by hseos 2007.11.16
#include "../hseos/Date/SHDateManager.h"
#include "../hseos/ResidentRegist/SHResidentRegistManager.h"
// E ����Ʈ �� �߰� added by hseos 2007.11.16

// desc_hseos_��ȥ_01
// S ��ȥ �߰� added by hseos 2007.12.12
#include "../hseos/Marriage/SHMarriageManager.h"
// E ��ȥ �߰� added by hseos 2007.12.12

// desc_hseos_����ý���_01
// S ����ý��� �߰� added by hseos 2008.01.15
#include "../hseos/Farm/SHFarmManager.h"
// E ����ý��� �߰� added by hseos 2008.01.15
//---KES PUNISH
#include "PunishManager.h"
//-------------
// 080407 LYW --- AgentNetworkMsgParser : Include chatroom manager.
#include "./ChatRoomMgr.h"

#ifdef _HACK_SHIELD_
#include "HackShieldManager.h"
#endif

#include "ggsrv25.h"
#include "NProtectManager.h"

#ifdef _HACK_SHIELD_
#include "HackShieldManager.h"
#endif

extern CMemoryPoolTempl<USERINFO> g_UserInfoPool;
extern int g_nServerSetNum;
extern HWND g_hWnd;
extern BOOL g_bCloseWindow;
DWORD g_dwAcceptUser2 = 0;

DWORD g_dwMaxUser = 4000;

#ifdef _NPROTECT_
#include "ggsrv25.h"
#include "NProtectManager.h"

extern CMemoryPoolTempl<CCSAuth2> g_CCSAuth2Pool;
#endif

#include ".\giftmanager.h"

void MP_MonitorMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_MORNITORMAPSERVER_NOTICESEND_SYN:						MornitorMapServer_NoticeSend_Syn(dwConnectionIndex, pMsg) ;						break;
	case MP_MORNITORMAPSERVER_PING_SYN:								MornitorMapServer_Ping_Syn(pTempMsg, dwConnectionIndex, pMsg, dwLength) ;		break;
	case MP_MORNITORMAPSERVER_QUERYUSERCOUNT_SYN:					MornitorMapServer_QueryUserCount_Syn(dwConnectionIndex, pMsg) ;					break;
	case MP_MORNITORMAPSERVER_ASSERTMSGBOX_SYN:						MornitorMapServer_AssertMsgBox_Syn(pMsg) ;										break;		
	case MP_MORNITORMAPSERVER_DISCONNECT_ALLUSER_SYN:				MornitorMapServer_Disconnect_AllUser_Syn(dwConnectionIndex)	;					break;
	case MP_MORNITORMAPSERVER_SERVEROFF_SYN:						MornitorMapServer_ServerOff_Syn() ;												break;
	case MP_MORNITORMAPSERVER_CONNECTED_SERVERLIST_SYN:				MornitorMapServer_Connected_ServerList_Syn(dwConnectionIndex) ;					break;		
	case MP_MORNITORMAPSERVER_EVENTNOTICE_ON:						MornitorMapServer_EventNotice_On(pMsg) ;										break;
	case MP_MORNITORMAPSERVER_EVENTNOTICE_OFF:						MornitorMapServer_EventNotice_Off(pMsg) ;										break;

	case MP_MORNITORMAPSERVER_QUERY_MAXUSER_SYN:
		{
			MSG_DWORD msg;
			msg.Category = MP_MORNITORMAPSERVER;
			msg.Protocol = MP_MORNITORMAPSERVER_QUERY_MAXUSER_ACK;
			msg.dwData = g_dwMaxUser;

			g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		break;
	case MP_MORNITORMAPSERVER_CHANGE_MAXUSER_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			g_dwMaxUser = pmsg->dwData;
			if( pmsg->dwData > 4000 )
				g_dwMaxUser = 4000;

			g_Console.LOG( 4,"Max User : %d", g_dwMaxUser );

			MSG_DWORD msg;
			msg.Category = MP_MORNITORMAPSERVER;
			msg.Protocol = MP_MORNITORMAPSERVER_CHANGE_MAXUSER_ACK;
			msg.dwData = g_dwMaxUser;

			g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		break;
		
	}
}


void MornitorMapServer_NoticeSend_Syn(DWORD dwConnectionIndex, char* pMsg)
{
	// �����Ϩ�Ao��icC���� 
	MSGNOTICE* pmsg = (MSGNOTICE*)pMsg;
	USERINFO * info = NULL;
	TESTMSG msg;
	msg.Category = MP_SIGNAL;
	msg.Protocol = MP_SIGNAL_COMMONUSER;
//			strcpy(msg.Msg, pmsg->Msg);
	SafeStrCpy( msg.Msg, pmsg->Msg, MAX_CHAT_LENGTH + 1 );

	g_pUserTable->SetPositionUserHead();
	while(info = (USERINFO *)g_pUserTable->GetUserData())
	{	
		//g_Network.Send2User(info->dwConnectionIndex, (char*)&msg, sizeof(TESTMSG));
		TESTMSG msgTemp = msg;
		g_Network.Send2User(info->dwConnectionIndex, (char*)&msgTemp, msgTemp.GetMsgLength());	//CHATMSG 040324
	}
	//g_Console.Log(eLogDisplay,4,pmsg->Msg);
}


void MornitorMapServer_Ping_Syn(MSGROOT* pTempMsg, DWORD dwConnectionIndex, char* pMsg, DWORD dwLength) 
{
	pTempMsg->Protocol = MP_MORNITORMAPSERVER_PING_ACK;
	g_Network.Send2Server(dwConnectionIndex, pMsg, dwLength);
//			g_Console.LOG(4, "ping msg recv...");
}


void MornitorMapServer_QueryUserCount_Syn(DWORD dwConnectionIndex, char* pMsg) 
{
	MSGUSERCOUNT  * pmsg = (MSGUSERCOUNT  *)pMsg;
	pmsg->Protocol = MP_MORNITORMAPSERVER_QUERYUSERCOUNT_ACK;
	pmsg->dwUserCount = g_pUserTable->GetUserCount();
	g_Network.Send2Server(dwConnectionIndex, pMsg, sizeof(MSGUSERCOUNT));
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


void MornitorMapServer_Disconnect_AllUser_Syn(DWORD dwConnectionIndex)
{
	// �ˡ��ϡ̡�ic A?Au��?�ˢ� �ˡ�eCN A��E��uO C����A|...
	g_pUserTableForUserID->SetPositionHead();
	USERINFO* pInfo;
	while( pInfo = g_pUserTableForUserID->GetData() )
	{
		if( pInfo->dwConnectionIndex != 0 )
		{
			DWORD dwConIdx = pInfo->dwConnectionIndex;
			OnDisconnectUser( dwConIdx );
			DisconnectUser( dwConIdx );
		}
		else
		{
			LoginCheckDelete(pInfo->dwUserID);
			g_pUserTableForUserID->RemoveUser(pInfo->dwUserID);
#ifdef _NPROTECT_
			if( pInfo->m_pCSA )
			{
				pInfo->m_pCSA->Close();
				g_CCSAuth2Pool.Free(pInfo->m_pCSA);
			}
#endif
			memset( pInfo, 0, sizeof(USERINFO) );	//�߰� 060414 KES
			g_UserInfoPool.Free(pInfo);
		}
	}

	// A|�ˡ�e����I Ao�ˡ���c�ˡ�| �ˡ��ϡ̡�iI A�ϩ��ˡ���cC�ϡ��ˡ�AAo��?�ˢ� �ˡ�eCN A��uA��I...
	while( g_DB.AllCleared() == FALSE )
	{
		g_DB.ProcessingDBMessage();
		Sleep(50);
	}

	MSGBASE Msg;
	Msg.Category = MP_MORNITORMAPSERVER;
	Msg.Protocol = MP_MORNITORMAPSERVER_DISCONNECT_ALLUSER_ACK;
	g_Network.Send2Server(dwConnectionIndex, (char*)&Msg, sizeof(Msg));
}


void MornitorMapServer_ServerOff_Syn() 
{
	g_bCloseWindow = TRUE;
}


void MornitorMapServer_Connected_ServerList_Syn(DWORD dwConnectionIndex) 
{
	MSG_PWRUP_BOOTLIST Msg;
	memset( &Msg, 0, sizeof(MSG_PWRUP_BOOTLIST) );

	Msg.Category = MP_MORNITORMAPSERVER;
	Msg.Protocol = MP_MORNITORMAPSERVER_CONNECTED_SERVERLIST_ACK;

	SERVERINFO* pInfo = NULL;
	g_pServerTable->SetPositionHead();
	while( pInfo = g_pServerTable->GetNextServer() )
		Msg.AddList( pInfo );

	g_Network.Send2Server( dwConnectionIndex, (char*)&Msg, Msg.GetSize() );
}


void MornitorMapServer_EventNotice_On(char* pMsg) 
{
	MSG_EVENTNOTIFY_ON* pmsg = (MSG_EVENTNOTIFY_ON*)pMsg;

	g_pServerSystem->SetEventNotifyStr( pmsg->strTitle, pmsg->strContext );
	g_pServerSystem->SetUseEventNotify( TRUE );
	
//			g_Network.Broadcast2AgentServerExceptSelf( (char*)pmsg, dwLength );
}


void MornitorMapServer_EventNotice_Off(char* pMsg) 
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	
	g_pServerSystem->SetUseEventNotify( FALSE );
//			g_Network.Broadcast2AgentServerExceptSelf( (char*)pmsg, dwLength );
}



void MP_POWERUPMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	BOOTMNGR->NetworkMsgParse(dwConnectionIndex, pMsg, dwLength);
}
void MP_AGENTSERVERMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{/*
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_SERVER_PWRUP:
		{
			MSG_WORD * msg = (MSG_WORD *)pMsg;
			SERVERINFO * info = g_pServerTable->FindServer(msg->wData);//port
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
					msg.mapnum = g_pListCollect->GetLoadAgentNum();
					g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));
				}

				// situation : ��?�ˢ�AIA?������c�����ˢ� ANA��c AO����i ��i�ϡ̡�i����i��������c�ˡ���c����a������c�����ˢ� A��E��uOC�ϡ�A��i��O���� usercountA�ˢ������ˡ� �ϩ�N����e
				if(info->wServerKind == DISTRIBUTE_SERVER)
				{
					SERVERINFO * myInfo = g_pServerTable->GetSelfServer();
					MSG_WORD2 msg2;
					msg2.Category = MP_SERVER;
					msg2.Protocol = MP_SERVER_USERCNT;
					msg2.wData1 = myInfo->wPortForServer;
					msg2.wData2 = myInfo->wAgentUserCnt;
					g_Network.Send2Server(dwConnectionIndex, (char *)&msg2, sizeof(msg2));
				}
				else if(info->wServerKind == MAP_SERVER)
				{
					MSGROOT msg;
					msg.Category = MP_SERVER;
					msg.Protocol = MP_SERVER_REGISTMAP_SYN;
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
			msg.mapnum = g_pListCollect->GetLoadAgentNum();
			g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));
		}
		break;


		case MP_SERVER_REGISTMAP_ACK:
			{
				REGIST_MAP* recv = (REGIST_MAP*)pMsg;
				if(recv->mapnum != 0)
				{
					g_pServerTable->SetMapRegist(recv->mapnum, recv->mapServerPort);
					SERVERINFO* pserver = g_pServerTable->FindServer(recv->mapServerPort);
					if(pserver)
						pserver->wHaveMapNum = recv->mapnum;
					
					SERVERINFO* serverinfo = g_pServerTable->FindServerForConnectionIndex(dwConnectionIndex);
					
					if(serverinfo)
						if(serverinfo->wPortForServer>MAPSERVER_PORT && serverinfo->wPortForServer < MAXSERVER_PORT)
						{
							MapUserUnRegistLoginMapInfo(serverinfo->wPortForServer);
						}
				}
			}
			break;
		}
	*/
}

//---KES ��ŷ ��� ����
void MP_TEMPMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	USERINFO* pUserInfo = g_pUserTable->FindUser( dwConnectionIndex );
	if( !pUserInfo ) return;

	if( pUserInfo->dwCharacterID )
	{
		MSGBASE* pTempMsg = (MSGBASE*)pMsg;

		pTempMsg->dwObjectID = pUserInfo->dwCharacterID;		//��⸦ ���� ����. ��� Ŭ���̾�Ʈ�� ���� ���� �޼����� �̷��� �ٲ���� �Ѵ�!

		TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
	}
}
//------------

void MP_USERCONNMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
//////////////////////////////////////////////////////////////////////////
// these Protocol Come From Distibute Server
	case MP_USERCONN_NOTIFY_USERLOGIN_SYN:							UserConn_Notify_UserLogin_Syn(dwConnectionIndex, pMsg) ;			break;
	case MP_USERCONN_NOTIFYTOAGENT_ALREADYOUT:						UserConn_NotifyToAgent_AlreadyOut(dwConnectionIndex, pMsg) ;		break;
	case MP_USERCONN_NOTIFY_OVERLAPPEDLOGIN:						UserConn_Notify_OverlappedLogin(dwConnectionIndex, pMsg) ;			break;
	case MP_USERCONN_FORCE_DISCONNECT_OVERLAPLOGIN:					UserConn_Force_Disconnect_OverlapLogin(dwConnectionIndex, pMsg) ;	break;
	case MP_USERCONN_DISCONNECTED_ON_LOGIN:							UserConn_Disconnected_On_Login(pMsg) ;								break;
	case MP_USERCONN_DISCONNECT_SYN:								UserConn_Disconnect_Syn(dwConnectionIndex) ;						break;
	case MP_USERCONN_CONNECTION_CHECK_OK:							UserConn_Connection_Check_Ok(dwConnectionIndex) ;					break;
	case MP_USERCONN_CHARACTERLIST_SYN:								UserConn_Characterlist_Syn(dwConnectionIndex, pMsg) ;				break;
	case MP_USERCONN_DIRECTCHARACTERLIST_SYN:						UserConn_DirectCharacterList_Syn(dwConnectionIndex) ;				break;	
	case MP_USERCONN_CHARACTERSELECT_SYN:							UserConn_CharacterSelect_Syn(dwConnectionIndex, pMsg) ;				break;
	case MP_USERCONN_CHANNELINFO_SYN:								UserConn_ChannelInfo_Syn(dwConnectionIndex, pMsg) ;					break;
	case MP_USERCONN_CHANNELINFO_ACK:								UserConn_ChannelInfo_Ack(pMsg, dwLength) ;							break;
	case MP_USERCONN_CHANNELINFO_NACK:								UserConn_ChannelInfo_Nack(pMsg, dwLength) ;							break;
	case MP_USERCONN_GAMEIN_SYN:									UserConn_Gamein_Syn(dwConnectionIndex, pMsg) ;						break;
	case MP_USERCONN_GAMEIN_NACK:									UserConn_Gamein_Nack(pMsg) ;										break;
	case MP_USERCONN_GAMEIN_ACK:									UserConn_Gamein_Ack(dwConnectionIndex, pMsg, dwLength) ;			break;
	case MP_USERCONN_CHARACTER_MAKE_SYN:							UserConn_Character_Make_Syn(dwConnectionIndex, pMsg) ;				break;
	case MP_USERCONN_CHARACTER_NAMECHECK_SYN:						UserConn_Character_NameCheck_Syn(dwConnectionIndex, pMsg) ;			break;
	case MP_USERCONN_CHARACTER_REMOVE_SYN:							UserConn_Character_Remove_Syn(dwConnectionIndex, pMsg) ;			break;
	case MP_USERCONN_CHANGEMAP_SYN:									UserConn_ChangeMap_Syn(dwConnectionIndex, pMsg, dwLength) ;			break;
	// 070917 LYW --- AgentNetworkMsgParser : Add function to process change map through the npc.
	case MP_USERCONN_NPC_CHANGEMAP_SYN :							UserConn_Npc_ChangeMap_Syn(dwConnectionIndex, pMsg, dwLength) ;		break;
	case MP_USERCONN_MAP_OUT:										UserConn_Map_Out(pMsg) ;											break;
	case MP_USERCONN_MAP_OUT_WITHMAPNUM:							UserConn_Map_Out_WithMapNum(pMsg) ;									break;
	// 070917 LYW --- AgentNetworkMsgParser : Add process to map out through the npc.
	case MP_USERCONN_NPC_MAP_OUT :									UserConn_Npc_Map_Out(pMsg) ;										break;
	case MP_USERCONN_CHANGEMAP_ACK:									UserConn_ChangeMap_Ack(pTempMsg, dwLength) ;						break;
	case MP_USERCONN_SAVEPOINT_SYN:									UserConn_SavePoint_Syn(dwConnectionIndex, pMsg, dwLength) ;			break;
	case MP_USERCONN_BACKTOCHARSEL_SYN:								UserConn_BackToCharSel_Syn(dwConnectionIndex, pMsg, dwLength) ;		break;
	case MP_USERCONN_BACKTOCHARSEL_ACK:								UserConn_BackToCharSel_Ack(pMsg) ;									break;
	case MP_USERCONN_CHEAT_USING:									UserConn_Cheat_Using(pMsg) ;										break;
	case MP_USERCONN_LOGINCHECK_DELETE:								UserConn_LoginCheck_Delete(pMsg) ;									break;
	case MP_USERCONN_GAMEINPOS_SYN:									UserConn_GameInPos_Syn(dwConnectionIndex, pMsg) ;					break;
	case MP_USERCONN_BACKTOBEFOREMAP_SYN:							UserConn_BackToBeforeMap_Syn(pMsg, dwLength) ;						break;
	case MP_USERCONN_BACKTOBEFOREMAP_ACK:							UserConn_BackToBeforeMap_Ack(pMsg) ;								break;
	case MP_USERCONN_MAP_OUT_TO_EVENTMAP:							UserConn_Map_Out_To_EventMap(pMsg) ;								break;
	case MP_USERCONN_ENTER_EVENTMAP_SYN:							UserConn_Enter_EventMap_Syn(dwConnectionIndex, pMsg) ;				break;
	case MP_USERCONN_EVENTITEM_USE:									UserConn_EventItem_Use(pMsg, dwLength) ;							break;
	case MP_USERCONN_EVENTITEM_USE2:								UserConn_EventItem_Use2(pMsg, dwLength) ;							break;
	case MP_USERCONN_RETURN_SYN:									UserConn_Return_Syn(dwConnectionIndex, pMsg, dwLength) ;			break;
	case MP_USERCONN_NPC_ADD_SYN:									TransToMapServerMsgParser(dwConnectionIndex,pMsg,dwLength);			break;
	case MP_USERCONN_NPC_DIE_SYN:									TransToMapServerMsgParser(dwConnectionIndex,pMsg,dwLength);			break;
	default:
		g_Network.Send2User((MSGBASE*)pTempMsg,dwLength);
		break;
	}
}

void MP_USERCONNUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
		case MP_USERCONN_FORCE_DISCONNECT_OVERLAPLOGIN:					UserConn_Force_Disconnect_OverlapLogin(dwConnectionIndex, pMsg) ;	break;
		case MP_USERCONN_DISCONNECT_SYN:								UserConn_Disconnect_Syn(dwConnectionIndex) ;						break;
		case MP_USERCONN_CONNECTION_CHECK_OK:							UserConn_Connection_Check_Ok(dwConnectionIndex) ;					break;
		case MP_USERCONN_CHARACTERLIST_SYN:								UserConn_Characterlist_Syn(dwConnectionIndex, pMsg) ;				break;
		case MP_USERCONN_DIRECTCHARACTERLIST_SYN:						UserConn_DirectCharacterList_Syn(dwConnectionIndex) ;				break;	
		case MP_USERCONN_CHARACTERSELECT_SYN:							UserConn_CharacterSelect_Syn(dwConnectionIndex, pMsg) ;				break;
		case MP_USERCONN_CHANNELINFO_SYN:								UserConn_ChannelInfo_Syn(dwConnectionIndex, pMsg) ;					break;
		case MP_USERCONN_GAMEIN_SYN:									UserConn_Gamein_Syn(dwConnectionIndex, pMsg) ;						break;
		case MP_USERCONN_CHARACTER_MAKE_SYN:							UserConn_Character_Make_Syn(dwConnectionIndex, pMsg) ;				break;
		case MP_USERCONN_CHARACTER_NAMECHECK_SYN:						UserConn_Character_NameCheck_Syn(dwConnectionIndex, pMsg) ;			break;
		case MP_USERCONN_CHARACTER_REMOVE_SYN:							UserConn_Character_Remove_Syn(dwConnectionIndex, pMsg) ;			break;
		case MP_USERCONN_CHANGEMAP_SYN:									UserConn_ChangeMap_Syn(dwConnectionIndex, pMsg, dwLength) ;			break;
		case MP_USERCONN_NPC_CHANGEMAP_SYN :							UserConn_Npc_ChangeMap_Syn(dwConnectionIndex, pMsg, dwLength) ;		break;
		case MP_USERCONN_MAP_OUT_WITHMAPNUM:							UserConn_Map_Out_WithMapNum(pMsg) ;									break;
		case MP_USERCONN_SAVEPOINT_SYN:									UserConn_SavePoint_Syn(dwConnectionIndex, pMsg, dwLength) ;			break;
		case MP_USERCONN_BACKTOCHARSEL_SYN:								UserConn_BackToCharSel_Syn(dwConnectionIndex, pMsg, dwLength) ;		break;
		case MP_USERCONN_GAMEINPOS_SYN:									UserConn_GameInPos_Syn(dwConnectionIndex, pMsg) ;					break;
		case MP_USERCONN_RETURN_SYN:									UserConn_Return_Syn(dwConnectionIndex, pMsg, dwLength) ;			break;
		case MP_USERCONN_NPC_ADD_SYN:									TransToMapServerMsgParser(dwConnectionIndex,pMsg,dwLength);			break;
		case MP_USERCONN_NPC_DIE_SYN:									TransToMapServerMsgParser(dwConnectionIndex,pMsg,dwLength);			break;
		case MP_USERCONN_GETSERVERTIME_SYN:								TransToMapServerMsgParser(dwConnectionIndex,pMsg,dwLength);			break;
		case MP_USERCONN_CHANGE_CHANNEL_IN_GAME_SYN:					TransToMapServerMsgParser(dwConnectionIndex,pMsg,dwLength);			break;
	}
}

void MP_USERCONNServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
		case MP_USERCONN_NOTIFY_USERLOGIN_SYN:							UserConn_Notify_UserLogin_Syn(dwConnectionIndex, pMsg) ;			break;
		case MP_USERCONN_NOTIFYTOAGENT_ALREADYOUT:						UserConn_NotifyToAgent_AlreadyOut(dwConnectionIndex, pMsg) ;		break;
		case MP_USERCONN_NOTIFY_OVERLAPPEDLOGIN:						UserConn_Notify_OverlappedLogin(dwConnectionIndex, pMsg) ;			break;
		case MP_USERCONN_FORCE_DISCONNECT_OVERLAPLOGIN:					UserConn_Force_Disconnect_OverlapLogin(dwConnectionIndex, pMsg) ;	break;
		case MP_USERCONN_DISCONNECTED_ON_LOGIN:							UserConn_Disconnected_On_Login(pMsg) ;								break;
		case MP_USERCONN_DISCONNECT_SYN:								UserConn_Disconnect_Syn(dwConnectionIndex) ;						break;
		case MP_USERCONN_CONNECTION_CHECK_OK:							UserConn_Connection_Check_Ok(dwConnectionIndex) ;					break;
		case MP_USERCONN_CHARACTERLIST_SYN:								UserConn_Characterlist_Syn(dwConnectionIndex, pMsg) ;				break;
		case MP_USERCONN_DIRECTCHARACTERLIST_SYN:						UserConn_DirectCharacterList_Syn(dwConnectionIndex) ;				break;	
		case MP_USERCONN_CHARACTERSELECT_SYN:							UserConn_CharacterSelect_Syn(dwConnectionIndex, pMsg) ;				break;
		case MP_USERCONN_CHANNELINFO_SYN:								UserConn_ChannelInfo_Syn(dwConnectionIndex, pMsg) ;					break;
		case MP_USERCONN_CHANNELINFO_ACK:								UserConn_ChannelInfo_Ack(pMsg, dwLength) ;							break;
		case MP_USERCONN_CHANNELINFO_NACK:								UserConn_ChannelInfo_Nack(pMsg, dwLength) ;							break;
		case MP_USERCONN_GAMEIN_SYN:									UserConn_Gamein_Syn(dwConnectionIndex, pMsg) ;						break;
		case MP_USERCONN_GAMEIN_NACK:									UserConn_Gamein_Nack(pMsg) ;										break;
		case MP_USERCONN_GAMEIN_ACK:									UserConn_Gamein_Ack(dwConnectionIndex, pMsg, dwLength) ;			break;
		case MP_USERCONN_CHARACTER_MAKE_SYN:							UserConn_Character_Make_Syn(dwConnectionIndex, pMsg) ;				break;
		case MP_USERCONN_CHARACTER_NAMECHECK_SYN:						UserConn_Character_NameCheck_Syn(dwConnectionIndex, pMsg) ;			break;
		case MP_USERCONN_CHARACTER_REMOVE_SYN:							UserConn_Character_Remove_Syn(dwConnectionIndex, pMsg) ;			break;
		case MP_USERCONN_CHANGEMAP_SYN:									UserConn_ChangeMap_Syn(dwConnectionIndex, pMsg, dwLength) ;			break;
		// 070917 LYW --- AgentNetworkMsgParser : Add function to process change map through the npc.
		case MP_USERCONN_NPC_CHANGEMAP_SYN :							UserConn_Npc_ChangeMap_Syn(dwConnectionIndex, pMsg, dwLength) ;		break;
		case MP_USERCONN_MAP_OUT:										UserConn_Map_Out(pMsg) ;											break;
		case MP_USERCONN_MAP_OUT_WITHMAPNUM:							UserConn_Map_Out_WithMapNum(pMsg) ;									break;
		// 070917 LYW --- AgentNetworkMsgParser : Add process to map out through the npc.
		case MP_USERCONN_NPC_MAP_OUT :									UserConn_Npc_Map_Out(pMsg) ;										break;
		case MP_USERCONN_CHANGEMAP_ACK:									UserConn_ChangeMap_Ack(pTempMsg, dwLength) ;						break;
		case MP_USERCONN_SAVEPOINT_SYN:									UserConn_SavePoint_Syn(dwConnectionIndex, pMsg, dwLength) ;			break;
		case MP_USERCONN_BACKTOCHARSEL_SYN:								UserConn_BackToCharSel_Syn(dwConnectionIndex, pMsg, dwLength) ;		break;
		case MP_USERCONN_BACKTOCHARSEL_ACK:								UserConn_BackToCharSel_Ack(pMsg) ;									break;
		case MP_USERCONN_CHEAT_USING:									UserConn_Cheat_Using(pMsg) ;										break;
		case MP_USERCONN_LOGINCHECK_DELETE:								UserConn_LoginCheck_Delete(pMsg) ;									break;
		case MP_USERCONN_GAMEINPOS_SYN:									UserConn_GameInPos_Syn(dwConnectionIndex, pMsg) ;					break;
		case MP_USERCONN_BACKTOBEFOREMAP_SYN:							UserConn_BackToBeforeMap_Syn(pMsg, dwLength) ;						break;
		case MP_USERCONN_BACKTOBEFOREMAP_ACK:							UserConn_BackToBeforeMap_Ack(pMsg) ;								break;
		case MP_USERCONN_MAP_OUT_TO_EVENTMAP:							UserConn_Map_Out_To_EventMap(pMsg) ;								break;
		case MP_USERCONN_ENTER_EVENTMAP_SYN:							UserConn_Enter_EventMap_Syn(dwConnectionIndex, pMsg) ;				break;
		case MP_USERCONN_EVENTITEM_USE:									UserConn_EventItem_Use(pMsg, dwLength) ;							break;
		case MP_USERCONN_EVENTITEM_USE2:								UserConn_EventItem_Use2(pMsg, dwLength) ;							break;
		case MP_USERCONN_RETURN_SYN:									UserConn_Return_Syn(dwConnectionIndex, pMsg, dwLength) ;			break;
		case MP_USERCONN_NPC_ADD_SYN:									TransToMapServerMsgParser(dwConnectionIndex,pMsg,dwLength);			break;
		case MP_USERCONN_NPC_DIE_SYN:									TransToMapServerMsgParser(dwConnectionIndex,pMsg,dwLength);			break;
		default:
			g_Network.Send2User((MSGBASE*)pTempMsg,dwLength);
			break;
	}
}

void UserConn_Notify_UserLogin_Syn(DWORD dwConnectionIndex, char* pMsg) 
{
	MSG_DWORD3BYTE2* pmsg = (MSG_DWORD3BYTE2*)pMsg;

	DWORD UserID				= pmsg->dwObjectID;
	DWORD DistAuthKey			= pmsg->dwData1;
	DWORD DistConnectionIndex	= pmsg->dwData2;
	int nTime					= (int)pmsg->dwData3;
	BYTE UserLevel				= pmsg->bData1;
	BYTE bType					= pmsg->bData2;
	
	if(g_bReady == FALSE)
	{
		MSG_DWORD3 msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_NOTIFY_USERLOGIN_NACK;
		msg.dwObjectID	= UserID;
		msg.dwData1		= DistAuthKey;
		msg.dwData2		= LOGIN_ERROR_NOAGENTSERVER;
		msg.dwData3		= DistConnectionIndex;
		g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );
		return;
	}

	//������ �̹� �� �α��� ���̵� �����ϴ��� ����. �̷����� �������� �ȵ�.
	//��¥�� ���� agent�� üũ�� �ȴ�. �ٸ� agent�� ���?
	//�׷��Ƿ�, ������ DB�� LoginCheck�ۿ� ����! �ݵ�� ��Ȯ�� �����ؾ��Ѵ�.
	//agent������ �׳� ���α׷� ������ �ʰ� ����� ���ư��� �ϴ°��� ����!
	USERINFO* pPreInfo = g_pUserTableForUserID->FindUser(UserID);
	if( pPreInfo )	//������ ������ ���������� ���.(�����ϱ�?)
	{
		//char temp[128];
		//sprintf( temp, "[ERROR]: NOTIFY_USERLOGIN - OConIdx:%d, OUsrIdx:%d, OCharIdx:%d, NewUsrIdx:%d", pPreInfo->dwConnectionIndex, pPreInfo->dwUserID, pPreInfo->dwCharacterID, UserID );
		//g_Console.LOG( 4, temp );
		//���ӵǾ� ������? �׷���찡 �߻��ϴ��� �ַܼα׸� ����. OldConIdx: 0 �̾�� �Ѵ�!

		if( pPreInfo->dwConnectionIndex )		
		{
			DWORD dwPreConIdx = pPreInfo->dwConnectionIndex;
			//OnDisconnectUser( dwPreConIdx ); //try use later
			DisconnectUser( dwPreConIdx );
			MSG_DWORD3 msg;
			msg.Category	= MP_USERCONN;
			msg.Protocol	= MP_USERCONN_NOTIFY_USERLOGIN_NACK;
			msg.dwObjectID	= UserID;
			msg.dwData1		= DistAuthKey;
			msg.dwData2		= LOGIN_ERROR_NOAGENTSERVER;	//�ӽ÷� ����Ѵ�. ���� �߰�����.
			msg.dwData3		= DistConnectionIndex;
			g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );
			return;
		}
		else
		{
			char temp[128];
			sprintf( temp, "[ERROR]: NOTIFY_USERLOGIN - OConIdx:%d, OUsrIdx:%d, OCharIdx:%d, NewUsrIdx:%d", pPreInfo->dwConnectionIndex, pPreInfo->dwUserID, pPreInfo->dwCharacterID, UserID );
			g_Console.LOG( 4, temp );
			g_pUserTableForUserID->RemoveUser( UserID );
#ifdef _NPROTECT_
			if( pPreInfo->m_pCSA )
			{
				pPreInfo->m_pCSA->Close();
				g_CCSAuth2Pool.Free(pPreInfo->m_pCSA);
			}
#endif

			memset( pPreInfo, 0, sizeof(USERINFO) );
			g_UserInfoPool.Free( pPreInfo );
		}
	}

	USERINFO* pInfo = g_UserInfoPool.Alloc();
	//���࿡ pInfo�� NULL�̶�� �׳� ���� ���°� ����. ���̻� ������Ʈ�� �Ҵɻ��°� �ǹǷ�.
	if( pInfo == NULL )
	{	//agent disable.
		MSGBASE msg;
		msg.Category	= MP_SERVER;
		msg.Protocol	= MP_SERVER_AGENTDISABLE;

		g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );

		g_Console.LOG( 4, "Agent Disable : Can't accept users!!" );
		return;
	}

	memset( pInfo, 0, sizeof(USERINFO) );
	
	pInfo->dwUserID						= UserID;
	pInfo->dwLastConnectionCheckTime	= gCurTime;
	pInfo->dwUniqueConnectIdx			= g_pServerSystem->MakeAuthKey();
	pInfo->DistAuthKey					= DistAuthKey;
	pInfo->UserLevel					= UserLevel;

#ifdef _NPROTECT_
	pInfo->m_pCSA = g_CCSAuth2Pool.Alloc();
	pInfo->m_pCSA->Init();
#endif			
	g_pUserTableForUserID->AddUser( pInfo, UserID );


	// 080314 NYJ --- �ӽ��׽�Ʈ�ڵ� :: �����α��ν� ID
	//g_Console.LOG( 4, "User Connected : [%ld]", pInfo->dwUserID );
	/////////////////////////////////////////////////////////


	MSG_DWORD3 msg;
	msg.Category	= MP_USERCONN;
	msg.Protocol	= MP_USERCONN_NOTIFY_USERLOGIN_ACK;
	msg.dwObjectID	= UserID;
	msg.dwData1		= DistAuthKey;
	msg.dwData2		= pInfo->dwUniqueConnectIdx;
	msg.dwData3		= DistConnectionIndex;
	
	g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );


//---KES PUNISH
	PunishListLoad( UserID );
//-------------
}


void UserConn_NotifyToAgent_AlreadyOut(DWORD dwConnectionIndex, char* pMsg) 
{
	//�α��� ���� ��û�ϰ� Ŭ���̾�Ʈ�� ������. �׷��� ������ ��������.
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	DWORD UserID		= pmsg->dwData1;
	DWORD AgentAuthKey	= pmsg->dwData2;

	USERINFO* pInfo = g_pUserTableForUserID->FindUser( UserID );

	if( pInfo )
	if( pInfo->dwUniqueConnectIdx == AgentAuthKey )
	{
		LoginCheckDelete(UserID);
		g_pUserTableForUserID->RemoveUser(UserID);
#ifdef _NPROTECT_
		if( pInfo->m_pCSA )
		{
			pInfo->m_pCSA->Close();
			g_CCSAuth2Pool.Free(pInfo->m_pCSA);
		}
#endif
		memset( pInfo, 0, sizeof(USERINFO) );
		g_UserInfoPool.Free( pInfo );
	}
}


void UserConn_Notify_OverlappedLogin(DWORD dwConnectionIndex, char* pMsg) 
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	DWORD UserID = pmsg->dwData;
	USERINFO* pInfo = g_pUserTableForUserID->FindUser(UserID);
	if(pInfo == NULL)
	{
		return;
	}

	MSGBASE msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_OTHERUSER_CONNECTTRY_NOTIFY;
	g_Network.Send2User(pInfo->dwConnectionIndex,(char*)&msg,sizeof(msg));
}


void UserConn_Force_Disconnect_OverlapLogin(DWORD dwConnectionIndex, char* pMsg) 
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;

	DWORD UserID = pmsg->dwObjectID;

	USERINFO* pUserInfo = g_pUserTableForUserID->FindUser(UserID);

	if(pUserInfo == NULL)
	{
		return;
	}
	
	DWORD dwUserConIndex = pUserInfo->dwConnectionIndex;
	
	if(dwUserConIndex)
	{
		MSGBASE msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_DISCONNECTED_BY_OVERLAPLOGIN;
		g_Network.Send2User(dwUserConIndex,(char*)&msg,sizeof(msg));

		//INFORM TO MAPSERVER : FOR NO DELAY DISCONNECTING. : no wait player exit
		if( pUserInfo->dwCharacterID )
		{
			SERVERINFO* pServerInfo = g_pServerTable->FindServerForConnectionIndex(pUserInfo->dwMapServerConnectionIndex);
			if( pServerInfo )
			{
				MSGBASE msgToMap;
				msgToMap.Category = MP_USERCONN;
				msgToMap.Protocol = MP_USERCONN_NOWAITEXITPLAYER;
				msgToMap.dwObjectID	= pUserInfo->dwCharacterID;
				g_Network.Send2Server( pServerInfo->dwConnectionIndex, (char*)&msgToMap, sizeof(msgToMap) );
			}
		}

		OnDisconnectUser( dwUserConIndex );
		DisconnectUser( dwUserConIndex );
	}
	else	//Ŀ�ؼ��� ������ CharcterID�� ������ ����. ���̺����� �����ش�.
	{
		LoginCheckDelete(UserID);
		g_pUserTableForUserID->RemoveUser(UserID);
#ifdef _NPROTECT_
		if( pUserInfo->m_pCSA )
		{
			pUserInfo->m_pCSA->Close();
			g_CCSAuth2Pool.Free(pUserInfo->m_pCSA);			
		}
#endif
		memset( pUserInfo, 0, sizeof(USERINFO) );
		g_UserInfoPool.Free( pUserInfo );                
	}	
}


void UserConn_Disconnected_On_Login(char* pMsg) 
{
	// 
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	DWORD UserID = pmsg->dwObjectID;
	DWORD AuthKey = pmsg->dwData;

	USERINFO* pInfo = g_pUserTableForUserID->FindUser(UserID);
	if(pInfo == NULL)
	{
		return;
	}

	if( AuthKey != pInfo->DistAuthKey )
	{
		return;
	}

	LoginCheckDelete(UserID);
	g_pUserTableForUserID->RemoveUser(UserID);
#ifdef _NPROTECT_
	if( pInfo->m_pCSA )
	{
		pInfo->m_pCSA->Close();
		g_CCSAuth2Pool.Free(pInfo->m_pCSA);
	}
#endif
	memset( pInfo, 0, sizeof(USERINFO) );	//�߰� 060414 KES
	g_UserInfoPool.Free( pInfo );
}


void UserConn_Disconnect_Syn(DWORD dwConnectionIndex) 
{
/*	USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if(!pInfo)
	{
		ASSERT(0);
	}*/
//			MSGBASE msg;
//			msg.Category = MP_USERCONN;
//			msg.Protocol = MP_USERCONN_DISCONNECT_ACK;
//			g_Network.Send2User(dwConnectionIndex,(char*)&msg,sizeof(msg));
//			DisconnectUser(dwConnectionIndex);

	OnDisconnectUser( dwConnectionIndex );
	DisconnectUser( dwConnectionIndex );
}


void UserConn_Connection_Check_Ok(DWORD dwConnectionIndex) 
{
	USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if(!pInfo)
		return;
	pInfo->bConnectionCheckFailed = FALSE;
}


void UserConn_Characterlist_Syn(DWORD dwConnectionIndex, char* pMsg) 
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			
	DWORD UserID		= pmsg->dwData1;
	DWORD DistAuthKey	= pmsg->dwData2;

	USERINFO* pInfo = g_pUserTableForUserID->FindUser(UserID);

	if(!pInfo)
	{
		MSGBASE msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_CHARACTERLIST_NACK;
		g_Network.Send2User(dwConnectionIndex,(char*)&msg,sizeof(msg));
		//���� ��������� ���� ����.
		OnDisconnectUser( dwConnectionIndex );
		DisconnectUser( dwConnectionIndex );
		return;
	}

	if(pInfo->DistAuthKey != DistAuthKey)	//������ �ٸ������� �Դ�.
	{

		MSGBASE msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_CHARACTERLIST_NACK;
		g_Network.Send2User(dwConnectionIndex,(char*)&msg,sizeof(msg));
		//���� ��������� ���� ����.
		OnDisconnectUser( dwConnectionIndex );
		DisconnectUser( dwConnectionIndex );
		return;
	}

	//---KES Network Process Fix 071114
	USERINFO* pPreInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if( pPreInfo == NULL )	//���������� ������ �����̴�. ��������� �����ΰ�?
	{
		char temp[128];
		sprintf( temp, "[ERROR]: NO PreInfo - UsrIdx:%d", UserID );
		g_Console.LOG( 4, temp );
		//���������� ���־��ϳ�?
		return;
	}

//---KES Crypt
#ifdef _CRYPTCHECK_ 
	pInfo->crypto.Create();	// key ����
#endif

	pInfo->dwConnectionIndex = dwConnectionIndex;

	//������ ��ü���ش�.
	g_pUserTable->RemoveUser(dwConnectionIndex);	//�ӽ����� ����
	g_UserInfoPool.Free( pPreInfo );				//�ӽ����� ����
	g_pUserTable->AddUser(pInfo,dwConnectionIndex);	//���ο� ������ ��ü

	/*
	//���࿡ �̹� ���� �Ǿ� �ִٸ�.... �������?
	//�̹� ������� �� ������. ���̺� �� ������?
	USERINFO* pPreInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if( pPreInfo )
	{
		char temp[128];
		sprintf( temp, "[ERROR]: CHARACTERLIST_SYN - OConIdx:%d, OUsrIdx:%d, OCharIdx:%d, NewUsrIdx:%d", pPreInfo->dwConnectionIndex, pPreInfo->dwUserID, pPreInfo->dwCharacterID, pInfo->dwUserID );
		g_Console.LOG( 4, temp );
		//���ӵǾ� ������? �׷���찡 �߻��ϴ��� �ַܼα׸� ����.

		//--�׳� ���̺����� ��������.
		g_pUserTable->RemoveUser(dwConnectionIndex);
	}
	//////////////////////////////////////////////////////////////////////////
	*/
	//----------------------------



	// billing
	UserIDXSendAndCharacterBaseInfo(UserID,pInfo->dwUniqueConnectIdx,dwConnectionIndex);

	++g_dwAcceptUser2;

	// desc_hseos_��������01
	// S �������� �߰� added by hseos 2007.06.15
	GetUserSexKind(pInfo->dwUserID);
	// E �������� �߰� added by hseos 2007.06.15
}


void UserConn_DirectCharacterList_Syn(DWORD dwConnectionIndex) 
{
	USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if(!pInfo)
		return;
	ASSERT(pInfo->dwUserID);
	if(pInfo->dwUserID == 0)
	{
		ASSERTMSG(0, "UserID�����ˢ� 0AI�ˡ�U.");
		return;
	}
	
	UserIDXSendAndCharacterBaseInfo(pInfo->dwUserID,pInfo->dwUniqueConnectIdx,dwConnectionIndex);
}


void UserConn_CharacterSelect_Syn(DWORD dwConnectionIndex, char* pMsg)
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;
	USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if( !pInfo )	return;
	
	CHARSELECTINFO* SelInfoArray = (CHARSELECTINFO*)pInfo->SelectInfoArray;
	DWORD playerID	= pmsg->dwObjectID;
	MAPTYPE mapnum;

	pInfo->wChannel = pmsg->wData;	//ä���� ��Ƶд�.

	for(int i = 0; i < MAX_CHARACTER_NUM; i++)
	{
		if(SelInfoArray[i].dwCharacterID == playerID)
		{
			mapnum = SelInfoArray[i].MapNum;
			break;
		}
		if(i == MAX_CHARACTER_NUM - 1)
		{
			MSGBASE msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_CHARACTERSELECT_NACK;
			g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
			return;
		}
	}
	
	SaveMapChangePointUpdate(playerID, 0);
	UnRegistLoginMapInfo(playerID);

	// desc_hseos_���͹���01
	// S ���͹��� �߰� added by hseos 2007.05.29
	// ..ó�� ���ӽÿ� �÷���Ÿ��, ���� ų ���� �ʱ�ȭ ���ش�.
	InitMonstermeterInfoOfDB(playerID);
	// E ���͹��� �߰� added by hseos 2007.05.29

	WORD mapport = g_pServerTable->GetServerPort( eSK_MAP, mapnum);

	if(mapport)
	{
		// 080424 LYW --- AgentNetworkMsgPasrser : ĳ���� �ƿ� ó��.
//===CHATROOM �ӽ� ��
//		USERINFO* pUserInfo = NULL ;
//		pUserInfo = g_pUserTableForObjectID->FindUser(playerID) ;
//		if(pUserInfo)
//		{
//			CHATROOMMGR->DestroyPlayer_From_Lobby(pUserInfo) ;
//		}

		SERVERINFO* pSInfo = g_pServerTable->FindServer(mapport);
		MSG_BYTE msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_CHARACTERSELECT_ACK;
		msg.bData = (BYTE)mapnum;
		msg.dwObjectID = playerID;	
		g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));

		USERINFO* pPreInfo = g_pUserTableForObjectID->FindUser(playerID);
		if( pPreInfo ) //�̹� ������Ʈ�� �ִٸ�..
		{
			char temp[128];
			sprintf( temp, "[ERROR]: CHARACTERSELECT - OConIdx:%d, OUsrIdx:%d, OCharIdx:%d, NewUsrIdx:%d", pPreInfo->dwConnectionIndex, pPreInfo->dwUserID, pPreInfo->dwCharacterID, pInfo->dwUserID );
			g_Console.LOG( 4, temp );

			g_pUserTableForObjectID->RemoveUser(playerID);

			//map���� ����ֳ�? ��������� ������.
			if( pPreInfo->dwMapServerConnectionIndex )
			{
				SERVERINFO* pServerInfo = g_pServerTable->FindServerForConnectionIndex(pPreInfo->dwMapServerConnectionIndex);
				if( pServerInfo )
				{
					MSG_DWORD msg;
					msg.Category	= MP_USERCONN;
					msg.Protocol	= MP_USERCONN_GAMEOUT_SYN;	//�ʿ��� ������ �׳� ���ִ� ���̴�.
					msg.dwObjectID	= pPreInfo->dwCharacterID;
					msg.dwData		= pPreInfo->dwUserID;
					g_Network.Send2Server(pPreInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg));
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////
		
		g_pUserTableForObjectID->AddUser(pInfo, playerID);

		pInfo->wUserMapNum = (WORD)mapnum;
		pInfo->dwCharacterID = playerID;
		pInfo->dwMapServerConnectionIndex = pSInfo->dwConnectionIndex;
	}
	else
	{
		MSG_BYTE msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_CHARACTERSELECT_NACK;
		msg.bData = (BYTE)mapnum;
		msg.dwObjectID = playerID;
		g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(MSG_BYTE));
	}
}


void UserConn_ChannelInfo_Syn(DWORD dwConnectionIndex, char* pMsg) 
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if(!pInfo)
		return;
	CHARSELECTINFO * SelectInfoArray = (CHARSELECTINFO*)pInfo->SelectInfoArray;
	CHARSELECTINFO * pSelectedInfo;
	MSG_DWORD msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHANNELINFO_SYN;
	msg.dwObjectID = dwConnectionIndex;
	msg.dwData = pInfo->dwUniqueConnectIdx;
	
	MAPTYPE mapnum;
	for(int i = 0; i < MAX_CHARACTER_NUM; i++)
	{
		if(SelectInfoArray[i].dwCharacterID == pmsg->dwObjectID)
		{
			pSelectedInfo = &SelectInfoArray[i];
			mapnum = pSelectedInfo->MapNum;
			break;
		}
		if(i == MAX_CHARACTER_NUM - 1)	// A?Au�����ˢ� �����ˢ�Ao����i AO�ˡ�A A�ϩ��ˡ�?����u �ˡ�U�ˡ���I A�ϩ��ˡ�?AI ��u����AA��iE
		{
			MSG_BYTE msg;
			msg.Category	= MP_USERCONN;
			msg.Protocol	= MP_USERCONN_CHANNELINFO_NACK;
			msg.bData		= 1;	//A?AuA�ϩ��ˡ�?A�ϡ��ˡ��Ϩ���u����AA
			g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
			return;
		}
	}
	WORD mapport = g_pServerTable->GetServerPort( eSK_MAP, mapnum);
	if(mapport)
	{
		SERVERINFO* pSInfo = g_pServerTable->FindServer(mapport);
		g_Network.Send2Server(pSInfo->dwConnectionIndex, (char*)&msg, sizeof(MSG_DWORD));
	}
	else
	{
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHANNELINFO_NACK;
		msg.bData		= 0;	//��u���Ϩ�oo�ϡ�����A�ϩ��ˡ�U.
		g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
	}
}


void UserConn_ChannelInfo_Ack(char* pMsg, DWORD dwLength)
{
	MSG_CHANNEL_INFO* pmsg = (MSG_CHANNEL_INFO*)pMsg;
	USERINFO* pInfo = g_pUserTable->FindUser(pmsg->dwObjectID);
	if(pInfo == NULL)
		return;
	if(pInfo->dwUniqueConnectIdx != pmsg->dwUniqueIDinAgent)
		return;
	g_Network.Send2User(pmsg->dwObjectID, (char*)pMsg, dwLength);
}


void UserConn_ChannelInfo_Nack(char* pMsg, DWORD dwLength) 
{
	MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
	USERINFO* pInfo = g_pUserTable->FindUser(pmsg->dwObjectID);
	if(pInfo == NULL)
		return;
	g_Network.Send2User(pmsg->dwObjectID, (char*)pMsg, dwLength);
}


void UserConn_Gamein_Syn(DWORD dwConnectionIndex, char* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if(!pInfo)
		return;

	// desc_hseos_����Ʈ ��_01
	// S ����Ʈ �� �߰� added by hseos 2007.11.27
	// ..ç���� �� ���� ��ȣ �߰�
	// MSG_DWORD3 msg;
	MSG_DWORD4 msg;
	// E ����Ʈ �� �߰� added by hseos 2007.11.27
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_GAMEIN_SYN;
	msg.dwObjectID = pInfo->dwCharacterID;
	msg.dwData1 = pInfo->dwUniqueConnectIdx;
/*
	if( pInfo->UserLevel < eUSERLEVEL_SUPERUSER )
	{
		msg.dwData2 = pmsg->dwData;	//Ŭ���̾�Ʈ���� ���� ä�ι�ȣ
		pInfo->wChannel = (WORD)pmsg->dwData;
	}
	else
	{
		msg.dwData2	= pInfo->wChannel;	//����� ä�ι�ȣ�� ��������.
	}
*/
	
	if(pInfo->bForceMove)	// GM������ �̵� or ġƮ�̵�
	{
		if(g_pServerSystem->GetChannelNum(pInfo->wUserMapNum) < pmsg->dwData)
		{
			// ��ȿ ä�η� �̵��ϰ��� �ϴ� ��� 1��ä�η� �̵�
			msg.dwData2 = 0;
		}
		else
		{
			//ä���� ����Ȱ�� ����
			if(pInfo->wChannel != pmsg->dwData)
			{
				msg.dwData2 = pmsg->dwData;
				pInfo->wChannel = pmsg->dwData;
			}
			//�䱸��� �̵�
			else
			{
				msg.dwData2 = pmsg->dwData;
			}
		}

		pInfo->bForceMove = FALSE;
	}
	else	// �α��� or ��Ż�� ���� �����̵�
	{
		if(pInfo->wChannel <= g_pServerSystem->GetChannelNum(pInfo->wUserMapNum))
		{
			// �����̵�
			msg.dwData2 = pInfo->wChannel;
		}
		else
		{
			// �̵��Ұ����� ��� 1��ä�η� �̵�
			msg.dwData2 = 0;
		}
	}

	/*
	// ���� ä�η� �̵��ϰ��� �ϴ� ��� 1��ä�η� �̵���Ų��.
	if( g_pServerSystem->GetChannelNum(pInfo->wUserMapNum) <  pInfo->wChannel)
	{
		msg.dwData2 = 0;
	}
	else
	{
		if(pInfo->wChannel <= g_pServerSystem->GetChannelNum(pInfo->wUserMapNum))
		{
			msg.dwData2 = pInfo->wChannel;
		}
		else
		{
			msg.dwData2 = pmsg->dwData;
		}
	}
	*/

	// desc_hseos_����Ʈ ��_01
	// S ����Ʈ �� �߰� added by hseos 2007.11.25
	// ..ç���� �������� �̵��̶�� ä�ι�ȣ�� ���ΰ� ��Ʈ������ �˸��� ç���� �� ���� ID�� �����Ѵ�.
	// ..�ʼ������� �� ID�� �����ؼ� ä���� ������.
	if (g_csDateManager.IsChallengeZone(pInfo->wUserMapNum))
	{
		msg.dwData2 = pInfo->nChallengeZoneEnterID;
		msg.dwData4 = pInfo->nChallengeZoneEnterSection;
		pInfo->nChallengeZoneEnterID = 0;
		pInfo->nChallengeZoneEnterSection = 0;
	}
	// E ����Ʈ �� �߰� added by hseos 2007.11.25
	msg.dwData3 = pInfo->UserLevel;

	g_Network.Send2Server(pInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg));

	// 080331 LYW --- AgentNetworkMsgParser : Add player to chatroom system.
//===CHATROOM �ӽ� ��
//	if(!CHATROOMMGR->IsAddedUser(pInfo->dwCharacterID))
//	{
//		CHATROOMMGR->RegistPlayer_To_Lobby(pInfo, MP_CHATROOM_ADD_USER_SYN) ;
//	}
}


void UserConn_Gamein_Nack(char* pMsg) 
{
	//������ �����ش�.
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pmsg->dwData);
	if( pUserInfo )
	{
		DWORD dwConIdx = pUserInfo->dwConnectionIndex;
		OnDisconnectUser( dwConIdx );
		DisconnectUser( dwConIdx );
	}
}


void UserConn_Gamein_Ack(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength) 
{
	SEND_HERO_TOTALINFO* pmsg = (SEND_HERO_TOTALINFO*)pMsg;
	DWORD uniqueID = pmsg->UniqueIDinAgent;
	DWORD CharacterID = pmsg->BaseObjectInfo.dwObjectID;
	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(CharacterID);
//			g_Console.Log(eLogDisplay,4,"GameinAck %d",CharacterID);
	if(pUserInfo == NULL)
	{				
		MSGBASE msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_GAMEIN_NACK;
		msg.dwObjectID = CharacterID;
		g_Network.Send2Server(dwConnectionIndex,(char*)&msg,sizeof(msg));
		return;
	}
	if(pUserInfo->dwUniqueConnectIdx != uniqueID)
	{
		MSGBASE msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_GAMEIN_NACK;
		msg.dwObjectID = CharacterID;
		g_Network.Send2Server(dwConnectionIndex,(char*)&msg,sizeof(msg));

		return;
	}
	// 100618 ������ ĳ���� ���̵��� �ʹ�ȣ ����
	CHARSELECTINFO* SelectInfoArray = (CHARSELECTINFO*)pUserInfo->SelectInfoArray;
	for(int i = 0; i < MAX_CHARACTER_NUM; i++)
	{
		if(SelectInfoArray[i].dwCharacterID == pmsg->dwObjectID)
		{
			SelectInfoArray[i].MapNum	=	pUserInfo->wUserMapNum;
			break;
		}
	}


	// desc_hseos_����Ʈ��Ī01
	// S ����Ʈ��Ī �߰� added by hseos 2007.11.20	2008.01.02
	// ..����Ʈ ��Ī ä�� ���̾��ٸ� ��뿡�� ������ ������� �˸���.
	g_csResidentRegistManager.ASRV_EndDateMatchingChat(pUserInfo);
	// E ����Ʈ��Ī �߰� added by hseos 2007.11.20	2008.01.02

	pUserInfo->DateMatchingInfo	=	pmsg->ChrTotalInfo.DateMatching;			// ���ӳ� '�˻�' ������ ������Ʈ�� ������ �÷��̾�(ĳ����)������ �ӽ� ����. �� �����ִ°� �߿��� ��.
//	pUserInfo->DateMatchingInfo
	strcpy( pUserInfo->DateMatchingInfo.szName, pmsg->BaseObjectInfo.ObjectName);		// ĳ���� �̸�.
	pUserInfo->DateMatchingInfo.nRequestChatState = NULL;
	pUserInfo->DateMatchingInfo.nChatPlayerID = NULL;


	g_Network.Send2User((MSGBASE*)pMsg,dwLength);

	//SW050729
	//jackpot
//COMMENT:JACKPOT	

	// 071115 ����, ���� ��� ����
	//if(pUserInfo->wUserMapNum)	//�� ������ �����̸� !=0
	//{
	//	JACKPOTMGR->SendMsgAddUserTotalMoney( CharacterID );
	//}

	//GameInAck�� ���� �Ŀ� ȣ������.
	SKILLDELAYMGR->SendSkillDelayMsgToClient( CharacterID );

	MSG_DWORD msgCheck;
	msgCheck.Category = MP_USERCONN;
	msgCheck.Protocol = MP_USERCONN_GAMEIN_OTHERMAP_SYN;
	msgCheck.dwObjectID = CharacterID;

	g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, (char*)&msgCheck, sizeof(msgCheck));

	// desc_hseos_�йи�01
	// S �йи� �߰� added by hseos 2007.07.11	2007.10.11
	// ..ó�� �ʿ� ������ �� �� ���� �йи� ������ DB���� �ε��Ѵ�.
	// ..�ϴ� �� �� �ε��ϰ� �Ǹ� ���̺� �߰��ǹǷ�, pcsFamily �� �����ȴ�.
	// ..�׷��� pcsFamily �� NULL �� ���� ó�� �ʿ� �����ϴ� �ɷ� ó��
	// ..(���̵� �ø��� GameIn ó���� �Ǳ� ������ �̷��� ó����....)
	if (pUserInfo->pcsFamily == NULL)
	{
		Family_LoadInfo(pUserInfo->dwCharacterID);
	}
	else
	{
		// ���� ���� ����
		for(UINT i=0; i<pUserInfo->pcsFamily->Get()->nMemberNum; i++)
		{
			if (pUserInfo->dwCharacterID == pUserInfo->pcsFamily->GetMember(i)->Get()->nID)
			{
				CSHFamilyMember::stINFO		stInfo		= *pUserInfo->pcsFamily->GetMember(i)->Get();
				
				stInfo.eConState = CSHFamilyMember::MEMBER_CONSTATE_LOGIN;
				pUserInfo->pcsFamily->GetMember(i)->Set(&stInfo);
				break;
			}
		}

		// Ŭ���̾�Ʈ�� ���� ������
		g_csFamilyManager.ASRV_SendInfoToClient(pUserInfo->pcsFamily);
		// �ʼ����� ȣĪ �˸���
		g_csFamilyManager.ASRV_SendNicknameToMap(pUserInfo, pUserInfo->pcsFamily->GetMember(i)->Get()->szNickname);
		// ���� �ε�
		Family_LoadEmblemInfo(pUserInfo->dwCharacterID, pUserInfo->pcsFamily->Get()->nID);
		// �ʼ����� ����UI ���� ��û
		g_csFarmManager.ASRV_RequestFarmUIInfoToMap(pUserInfo->dwCharacterID, pUserInfo->pcsFamily);
	}

	// ..Ż�� �г�Ƽ �ð� �ε�
	Family_Leave_LoadInfo(pUserInfo->dwCharacterID);
	// E �йи� �߰� added by hseos 2007.07.11	2007.10.11

	// 080418 LYW --- AgentNetworkMsgParser : ���� ���� ������Ʈ �Լ�.
//===CHATROOM �ӽ� ��
	//CHATROOMMGR->UpdatePlayerInfo(pUserInfo) ;
}


void UserConn_Character_Make_Syn(DWORD dwConnectionIndex, char* pMsg)
{
	CHARACTERMAKEINFO* pmsg = (CHARACTERMAKEINFO*)pMsg;
	USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if(!pInfo)
		return;
	
// ĳ���� ���� ���� üũ ---------------------------------------------------------
	if( !CheckCharacterMakeInfo( pmsg ) )
	{
		MSGBASE msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_CHARACTER_MAKE_NACK;
		g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
		return;
	}
//---------------------------------------------------------------------------------

	DWORD UserID = pInfo->dwUserID;
	pmsg->UserID = UserID;
#ifndef _SELECT_GENDER_
	// desc_hseos_��������01
	// S �������� �߰� added by hseos 2007.06.16
	// ..DB���� �о�� ������ �ξ��� ���������� ����
	pmsg->SexType = (BYTE)pInfo->nSexKind;	
	// E �������� �߰� added by hseos 2007.06.16
#endif
	//string overflow����
	// desc_hseos_��������01
	// S �������� �߰� added by hseos 2007.06.16
	// ..DB���� �о�� ������ �ξ��� ���������� ����
//	pmsg->SexType = pmsg->SexType;	
	// E �������� �߰� added by hseos 2007.06.16

	//string overflow����
	char buf[MAX_NAME_LENGTH+1];
	SafeStrCpy( buf, pmsg->Name, MAX_NAME_LENGTH+1 );

	if( FILTERTABLE->IsUsableName( buf ) &&
		!FILTERTABLE->IsInvalidCharInclude( (unsigned char*)buf ) )
	{			
		CreateCharacter(pmsg, g_nServerSetNum, dwConnectionIndex);
	}
	else
	{
		MSGBASE msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_CHARACTER_MAKE_NACK;
		g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
	}
}


void UserConn_Character_NameCheck_Syn(DWORD dwConnectionIndex, char* pMsg)
{
	MSG_NAME* pmsg = (MSG_NAME*)pMsg;
			
	//string overflow����
	char buf[MAX_NAME_LENGTH+1];
	SafeStrCpy( buf, pmsg->Name, MAX_NAME_LENGTH+1 );
	
	if( FILTERTABLE->IsUsableName( buf ) &&
		!FILTERTABLE->IsInvalidCharInclude( (unsigned char*)buf ) )
	{
		CharacterNameCheck( buf, dwConnectionIndex);
	}
	else
	{
		MSG_WORD msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_CHARACTER_NAMECHECK_NACK;
		msg.wData = 2;
		g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
	}	
}


void UserConn_Character_Remove_Syn(DWORD dwConnectionIndex, char* pMsg) 
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	DeleteCharacter(pmsg->dwData, g_nServerSetNum, dwConnectionIndex);
}


void UserConn_ChangeMap_Syn(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength) 
{
	MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

	WORD tomapserverport = 0;

	MAPCHANGE_INFO* pChangeInfo = g_pServerSystem->GetMapChangeInfo(pmsg->dwData1);
	if( pmsg->dwData1 < 2000 )
	{
		if(!pChangeInfo)
		{	
			// �ˡ�EAI ��u�Ϩ�A�ˡ��ˡ�e ����A����I
			MSGBASE msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_CHANGEMAP_NACK;
			msg.dwObjectID = pmsg->dwObjectID;
			g_Network.Send2User((MSGBASE*)&msg, sizeof(MSGBASE));
			return;
		}
		tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, pChangeInfo->MoveMapNum);
	}
	else
		tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, (WORD)(pmsg->dwData1-2000));

	if(tomapserverport)
	{
		USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
		if( !pInfo ) return;

		if( GMINFO->IsEventStarted() )
		if( pChangeInfo->MoveMapNum == 59 )
		{
			MSGBASE msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_CHANGEMAP_NACK;
			msg.dwObjectID = pmsg->dwObjectID;
			g_Network.Send2User((MSGBASE*)&msg, sizeof(MSGBASE));
			return;
		}				

		g_Network.Send2Server(pInfo->dwMapServerConnectionIndex,(char*)pMsg,dwLength);

		//SERVERINFO* pSInfo = g_pServerTable->FindServer((DWORD)tomapserverport);
		//���ϡ�a���� ��O CIAo?
		//pInfo->wUserMapNum = (WORD)pChangeInfo->MoveMapNum;		// ��oU�ϡ��ϡ� �ˡ�E ��o�Ϩ�E��I����I ��uA����A
		//pInfo->dwMapServerConnectionIndex = pSInfo->dwConnectionIndex;	// ��oU�ϡ��ϡ� �ˡ�E dwConnectionIndex����I ��uA����A
	}
	else
	{
		// �ˡ�EAI ��u�Ϩ�A�ˡ��ˡ�e ����A����I
		MSGBASE msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_CHANGEMAP_NACK;
		msg.dwObjectID = pmsg->dwObjectID;
		g_Network.Send2User((MSGBASE*)&msg, sizeof(MSGBASE));
		return;
	}
}

// 070917 LYW --- AgentNetworkMsgParser : Add function to process change map through the npc.
void UserConn_Npc_ChangeMap_Syn(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength) 
{
	USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if( !pInfo ) return;

	MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

	WORD tomapserverport = 0;
	tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, pmsg->dwData1);

	if(!tomapserverport)
	{
		MSGBASE msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_NPC_CHANGEMAP_NACK;
		msg.dwObjectID = pmsg->dwObjectID;
		g_Network.Send2User((MSGBASE*)&msg, sizeof(MSGBASE));
		return;
	}

	g_Network.Send2Server(pInfo->dwMapServerConnectionIndex,(char*)pMsg,dwLength);
}

void UserConn_Return_Syn(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength) 
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;

	USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);

	LOGINPOINT_INFO* pLoginInfo = g_pServerSystem->GetLoginInfo(pmsg->wData);

	if( !pLoginInfo )
		return;

	WORD tomapserverport = 0;

	tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, pLoginInfo->MapNum );

	if(tomapserverport)
	{
		USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
		if( !pInfo ) return;


		g_Network.Send2Server(pInfo->dwMapServerConnectionIndex,(char*)pMsg,dwLength);

	}
	else
	{
		MSGBASE msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_CHANGEMAP_NACK;
		msg.dwObjectID = pmsg->dwObjectID;
		g_Network.Send2User((MSGBASE*)&msg, sizeof(MSGBASE));
		return;
	}
}

void UserConn_Map_Out(char* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	MAPCHANGE_INFO* ChangeInfo = NULL;
	WORD mapport = 0;

	if( pmsg->dwData < 2000 )
	{
		ChangeInfo = g_pServerSystem->GetMapChangeInfo(pmsg->dwData);
		mapport = g_pServerTable->GetServerPort( eSK_MAP, ChangeInfo->MoveMapNum);
	}
	else
		mapport = g_pServerTable->GetServerPort( eSK_MAP, (WORD)(pmsg->dwData-2000));

	if(mapport)
	{
		MSG_DWORD msgserver;
		msgserver.Category = MP_USERCONN;
		msgserver.Protocol = MP_USERCONN_CHANGEMAP_ACK;
		if( pmsg->dwData < 2000 )
			msgserver.dwData = ChangeInfo->MoveMapNum;
		else
			msgserver.dwData = pmsg->dwData-2000;
		msgserver.dwObjectID = pmsg->dwObjectID;	
		g_Network.Send2User((MSG_DWORD*)&msgserver, sizeof(MSG_DWORD));

		SERVERINFO* pSInfo = g_pServerTable->FindServer(mapport);
		USERINFO* pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);

		if(pInfo == NULL)
			return;

		if( pmsg->dwData < 2000 )
			pInfo->wUserMapNum = (WORD)ChangeInfo->MoveMapNum;
		else
			pInfo->wUserMapNum = (WORD)pmsg->dwData-2000;				
		pInfo->dwMapServerConnectionIndex = pSInfo->dwConnectionIndex;
	}
}

// 070917 LYW --- AgentNetworkMsgParser : Add function to process map out through the npc.
void UserConn_Npc_Map_Out(char* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	WORD mapport = 0;

	mapport = g_pServerTable->GetServerPort( eSK_MAP, (WORD)pmsg->dwData);

	if(mapport)
	{
		MSG_DWORD msgserver;
		msgserver.Category = MP_USERCONN;
		msgserver.Protocol = MP_USERCONN_NPC_CHANGEMAP_ACK;
		msgserver.dwObjectID = pmsg->dwObjectID;	

		msgserver.dwData = pmsg->dwData ;

		g_Network.Send2User((MSG_DWORD*)&msgserver, sizeof(MSG_DWORD));

		SERVERINFO* pSInfo = g_pServerTable->FindServer(mapport);
		USERINFO* pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);

		if(pInfo == NULL)
			return;

		pInfo->wUserMapNum = (WORD)pmsg->dwData ;			
		pInfo->dwMapServerConnectionIndex = pSInfo->dwConnectionIndex;
	}
}


void UserConn_Map_Out_WithMapNum(char* pMsg)
{
	MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
	WORD mapport = g_pServerTable->GetServerPort( eSK_MAP, pmsg->wData1);
	if(mapport)
	{
		USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
		if( !pInfo ) return;

//---KES ä���̵� ����
		MSG_DWORD2 msgserver;
		msgserver.Category		= MP_USERCONN;
		msgserver.Protocol		= MP_USERCONN_CHEAT_CHANGEMAP_ACK;
		msgserver.dwData1		= pmsg->wData1;
		// 080319 KTH -- wData2�� ���� (WORD)(-1)�� ���� ���� UserInfo(pInfo)���� ä�� ������ �����´�.
		if( pmsg->wData2 != (WORD)(-1) )
			msgserver.dwData2	= pmsg->wData2;
		else
			msgserver.dwData2	= pInfo->wChannel;
		msgserver.dwObjectID	= pmsg->dwObjectID;

		g_Network.Send2User( &msgserver, sizeof(msgserver) );
//---------------------
		/////////////////
		SERVERINFO* pSInfo	= g_pServerTable->FindServer(mapport);
		pInfo->wUserMapNum	= pmsg->wData1;		// ��oU�ϡ��ϡ� �ˡ�E ��o�Ϩ�E��I����I ��uA����A
		pInfo->dwMapServerConnectionIndex = pSInfo->dwConnectionIndex;	// ��oU�ϡ��ϡ� �ˡ�E dwConnectionIndex����I ��uA����A
		/*if( pmsg->wData2 != (WORD)(-1) )
			pInfo->wChannel	= pmsg->wData2;*/
	}
}


void UserConn_ChangeMap_Ack(MSGROOT* pTempMsg, DWORD dwLength)
{
	g_Network.Send2User((MSGBASE*)pTempMsg,dwLength);
}


void UserConn_SavePoint_Syn(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if(!pInfo)
		return;
	g_Network.Send2Server(pInfo->dwMapServerConnectionIndex,pMsg,dwLength);
}


void UserConn_BackToCharSel_Syn(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if(!pInfo)
		return;

	// desc_hseos_�йи�01
	// S �йи� �߰� added by hseos 2007.07.11
	// ..�ٸ� ������Ʈ�� �˸���
	g_csFamilyManager.ASRV_SendMemberConToOtherAgent(pInfo->pcsFamily, pInfo->dwCharacterID, CSHFamilyMember::MEMBER_CONSTATE_LOGOFF);
	// ..�йи� ���̺��� ����
	g_csFamilyManager.DelFamilyToTbl(pInfo->dwCharacterID, pInfo->pcsFamily);
	// ..Ŭ���̾�Ʈ�� �˸���
	g_csFamilyManager.ASRV_SendInfoToClient(pInfo->pcsFamily, 2);
	pInfo->pcsFamily = NULL;
	pInfo->nFamilyLeaveDate = 0;
	pInfo->nFamilyLeaveKind = 0;
	// E �йи� �߰� added by hseos 2007.07.11

	// desc_hseos_����Ʈ��Ī01
	// S ����Ʈ��Ī �߰� added by hseos 2007.11.20	2008.01.02
	// ..����Ʈ ��Ī ä�� ���̾��ٸ� ��뿡�� ������ ������� �˸���.
	g_csResidentRegistManager.ASRV_EndDateMatchingChat(pInfo);
	// E ����Ʈ��Ī �߰� added by hseos 2007.11.20	2008.01.02

	g_Network.Send2Server(pInfo->dwMapServerConnectionIndex,pMsg,dwLength);
}


void UserConn_BackToCharSel_Ack(char* pMsg)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	USERINFO * pInfo = (USERINFO*)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if(!pInfo)
		return;

//KES 031111
	DWORD UserID = pInfo->dwUserID;
	UserIDXSendAndCharacterBaseInfo(UserID, pInfo->dwUniqueConnectIdx, pInfo->dwConnectionIndex);

	g_pUserTableForObjectID->RemoveUser(pInfo->dwCharacterID);
	pInfo->dwCharacterID = 0;
	pInfo->dwMapServerConnectionIndex = 0;
	pInfo->wUserMapNum = 0;

	// 080331 LYW --- AgentNetworkMsgParser : Add player to chatroom system.
	//CHATROOMMGR->DestroyPlayer_From_Lobby(pInfo) ;
}


void UserConn_Cheat_Using(char* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	CheatLog(pmsg->dwObjectID,pmsg->dwData);
}


void UserConn_LoginCheck_Delete(char* pMsg) 
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;

	if( pmsg->dwObjectID )
	{
		//no user in agent	//any other agent?
		if( g_pUserTableForUserID->FindUser(pmsg->dwObjectID) == NULL )
		{
			LoginCheckDelete( pmsg->dwObjectID );
		}
	}
}


void UserConn_GameInPos_Syn(DWORD dwConnectionIndex, char* pMsg) 
{
	//	MSGBASE* pmsg = (MSGBASE*)pMsg;
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if(!pInfo)
		return;

	// ��u����AACN AE�ˡ�?AI��?�ˢ� �ˡ�A�ˡ�A �ˡ�E��u���Ϩ�oo����I AE�ˡ�?AI ��u����AAA��i ��uE�ˡ��ϡ�
/*		MSG_DWORD msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_GAMEIN_SYN;
	msg.dwObjectID = pInfo->dwCharacterID;
	msg.dwData = pInfo->dwUniqueConnectIdx;
*/
	MSG_DWORD4 msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_GAMEINPOS_SYN;
	msg.dwObjectID = pInfo->dwCharacterID;
	msg.dwData1 = pInfo->dwUniqueConnectIdx;
	
	if( pInfo->UserLevel < eUSERLEVEL_SUPERUSER )
	{
		msg.dwData2 = pmsg->dwData1;				//ä�ι�ȣ
		pInfo->wChannel = (WORD)pmsg->dwData1;
	}
	else
	{
		msg.dwData2	= pInfo->wChannel;	//����� ä�ι�ȣ�� ��������.
	}
	msg.dwData3 = pInfo->UserLevel;
	msg.dwData4 = pmsg->dwData2;

	g_Network.Send2Server(pInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg));
}





//=========================================================================
//	NAME : UserConn_BackToBeforeMap_Syn
//	DESC : The function to move player to specified destination. 080104 LYW
//=========================================================================
void UserConn_BackToBeforeMap_Syn(char* pMsg, DWORD dwLength) 
{
	if( !pMsg ) return ;

	MSG_DWORD3* pmsg = NULL ;
	pmsg = (MSG_DWORD3*)pMsg ;

	if( !pmsg ) return ;

	WORD wMapNum = 0 ;
	wMapNum = (WORD)pmsg->dwData1 ;

	WORD mapport = 0 ;
	mapport = g_pServerTable->GetServerPort( eSK_MAP, wMapNum ) ;

	if(mapport)
	{
		USERINFO* pInfo = NULL ;
		pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID ) ;

		if( !pInfo ) return ;

		g_Network.Send2Server( pInfo->dwMapServerConnectionIndex, (char*)pMsg, dwLength ) ;
	}
}


//void UserConn_BackToBeforeMap_Syn(char* pMsg, DWORD dwLength) 
//{
//	MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
//	WORD mapport = g_pServerTable->GetServerPort( eSK_MAP, pmsg->wData2 );
//	if(mapport)
//	{
//		USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
//		if( !pInfo ) return;
//
//		g_Network.Send2Server( pInfo->dwMapServerConnectionIndex, (char*)pMsg, dwLength );
//	}
//}





//=========================================================================
//	NAME : UserConn_BackToBeforeMap_Ack
//	DESC : The function to move player to specified destination. 080104 LYW
//=========================================================================
void UserConn_BackToBeforeMap_Ack(char* pMsg) 
{
	if( !pMsg ) return ;

	MSG_DWORD3* pmsg = NULL ;
	pmsg = (MSG_DWORD3*)pMsg ;

	if( !pmsg ) return ;

	WORD mapport = 0 ;
	mapport = g_pServerTable->GetServerPort( eSK_MAP, (WORD)pmsg->dwData1 ) ;

	if(mapport)
	{
		USERINFO* pInfo = NULL ;
		pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID ) ;

		if( !pInfo ) return ;

		MSG_DWORD msgserver ;
		msgserver.Category		= MP_USERCONN ;
		msgserver.Protocol		= MP_USERCONN_CHANGEMAP_ACK ;
		msgserver.dwObjectID	= pmsg->dwObjectID ;
		msgserver.dwData		= (WORD)pmsg->dwData1 ;
		
		g_Network.Send2User((MSG_DWORD*)&msgserver, sizeof(MSG_DWORD)) ;

		SERVERINFO* pSInfo	= NULL ;
		pSInfo = g_pServerTable->FindServer(mapport) ;

		if( !pSInfo ) return ;

		pInfo->wUserMapNum	= (WORD)pmsg->dwData1 ;
		pInfo->dwMapServerConnectionIndex = pSInfo->dwConnectionIndex ;
	}
}


//void UserConn_BackToBeforeMap_Ack(char* pMsg) 
//{
//	MSG_WORD* pmsg = (MSG_WORD*)pMsg;
//	WORD mapport = g_pServerTable->GetServerPort( eSK_MAP, pmsg->wData );
//	if(mapport)
//	{
//		USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
//		if( !pInfo ) return;
//
//		MSG_DWORD msgserver;
//		msgserver.Category		= MP_USERCONN;
//		msgserver.Protocol		= MP_USERCONN_CHANGEMAP_ACK;
//		msgserver.dwObjectID	= pmsg->dwObjectID;
//		msgserver.dwData		= pmsg->wData;
//		
//		g_Network.Send2User((MSG_DWORD*)&msgserver, sizeof(MSG_DWORD));
//
//		SERVERINFO* pSInfo	= g_pServerTable->FindServer(mapport);
//		pInfo->wUserMapNum	= pmsg->wData;
//		pInfo->dwMapServerConnectionIndex = pSInfo->dwConnectionIndex;
//	}
//}


void UserConn_Map_Out_To_EventMap(char* pMsg) 
{
	MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
	WORD mapport = g_pServerTable->GetServerPort( eSK_MAP, (WORD)pmsg->dwData1 );
	if( mapport )
	{
		USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
		if( !pInfo ) return;

		SERVERINFO* pSInfo = g_pServerTable->FindServer( mapport );
		pInfo->wUserMapNum = (WORD)pmsg->dwData1;				
		pInfo->dwMapServerConnectionIndex = pSInfo->dwConnectionIndex;

		g_Network.Send2User( (MSGBASE*)pMsg, sizeof(MSG_DWORD3) );				
	}
}


void UserConn_Enter_EventMap_Syn(DWORD dwConnectionIndex, char* pMsg) 
{
	USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if( !pInfo )	return;

	MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
	
	MSG_DWORD4 msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_ENTER_EVENTMAP_SYN;
	msg.dwObjectID = pInfo->dwCharacterID;
	msg.dwData1 = pInfo->dwUniqueConnectIdx;
	msg.dwData2 = pInfo->UserLevel;
	msg.dwData3 = pmsg->dwData2;		// channel
	msg.dwData4 = pmsg->dwData3;		// team
	
	g_Network.Send2Server(pInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg));
}


void UserConn_EventItem_Use(char* pMsg, DWORD dwLength) 
{
	/*
	// event 050106
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pInfo )	return;
	EventItemUse050106( pInfo->dwUserID );
	g_Network.Send2User( (MSGBASE*)pMsg, dwLength );			

	// event 050203
	MSG_NAME* pmsg = (MSG_NAME*)pMsg;
	USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pInfo )	return;
	EventItemUse050203( pInfo->dwUserID, pmsg->Name, g_nServerSetNum );
	g_Network.Send2User( (MSGBASE*)pMsg, dwLength );

	// 050322 event
	MSG_NAME* pmsg = (MSG_NAME*)pMsg;
	USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pInfo )	return;
	EventItemUse050322( pInfo->dwUserID );
	g_Network.Send2User( (MSGBASE*)pMsg, dwLength );
*/
	// event 051108
	MSG_NAME* pmsg = (MSG_NAME*)pMsg;
	USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pInfo )	return;
	EventItemUse051108( pInfo->dwUserID, pmsg->Name, g_nServerSetNum );
	g_Network.Send2User( (MSGBASE*)pMsg, dwLength );
}


void UserConn_EventItem_Use2(char* pMsg, DWORD dwLength) 
{
	MSG_NAME* pmsg = (MSG_NAME*)pMsg;
	USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pInfo )	return;
	EventItemUse2( pInfo->dwUserID, pmsg->Name, g_nServerSetNum );
	g_Network.Send2User( (MSGBASE*)pMsg, dwLength );
}



void MP_CHATServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;

	switch(pTempMsg->Protocol)
	{
	case MP_CHAT_ALL:		
	case MP_CHAT_SMALLSHOUT:
	case MP_CHAT_WHOLE :
	case MP_CHAT_TRADE :
	case MP_CHAT_GM_SMALLSHOUT:
	case MP_CHAT_MONSTERSPEECH:
	case MP_CHAT_NORMAL :					Chat_Normal( dwConnectionIndex,  pMsg,  dwLength) ;			break;
	case MP_CHAT_WHISPER_SYN:				Chat_Whisper_Syn( dwConnectionIndex,  pMsg) ;				break;
	case MP_CHAT_WHISPER_GM_SYN:			Chat_Whisper_Gm_Syn( dwConnectionIndex,  pMsg) ;			break;
	case MP_CHAT_WHISPER_ACK:				Chat_Whisper_Ack( pMsg,  dwLength) ;						break;
	case MP_CHAT_WHISPER_NACK:				Chat_Whisper_Nack( pMsg,  dwLength) ;						break;
	case MP_CHAT_PARTY:						Chat_Party(pMsg) ;											break;
	case MP_CHAT_GUILD:						Chat_Guild( pMsg, dwLength) ;								break;
	case MP_CHAT_FAMILY :					Chat_Family( pMsg, dwLength ) ;								break ;
	case MP_CHAT_GUILDUNION:				Chat_GuildUnion( pMsg, dwLength) ;							break;
	//case MP_CHAT_SHOUT_SENDSERVER:			Chat_Shout_Sendserver(pMsg) ;								break;		
	case MP_CHAT_SHOUT_ACK:					Chat_Shout_Ack(dwConnectionIndex, pMsg, dwLength);			break;
	}
}

void Chat_Normal(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength) 
{
	TransToClientMsgParser(dwConnectionIndex,pMsg,dwLength);
}


void Chat_Whisper_Syn(DWORD dwConnectionIndex, char* pMsg)
{
	MSG_WHISPER* pmsg = (MSG_WHISPER*)pMsg;
	USERINFO* pReceiverInfo = g_pUserTableForObjectID->FindUser( pmsg->dwReceiverID );
	if( !pReceiverInfo ) return;

	if( pReceiverInfo->GameOption.bNoWhisper )
	{
		MSG_BYTE msg;
		msg.Category	= MP_CHAT;
		msg.Protocol	= MP_CHAT_WHISPER_NACK;
		msg.dwObjectID	= pmsg->dwObjectID;	//�����ˡ��ϩ���o��ic��O��A ��u����AI��i�ϡ�
		msg.bData		= CHATERR_OPTION_NOWHISPER;

		g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));
		return;
	}

	//---�����ˡ��ϩ���o��ic��O��AAI AO�ˡ�A ��?�ˢ�AIA?������c����I
	MSG_CHAT msgChat;
	msgChat.Category	= MP_CHAT;
	msgChat.Protocol	= MP_CHAT_WHISPER_ACK;
	msgChat.dwObjectID	= pmsg->dwObjectID;	//�����ˡ��ϩ���o��ic��O��A
	SafeStrCpy( msgChat.Name, pmsg->ReceiverName, MAX_NAME_LENGTH+1 );	//��i����i���ˡ�A��ic��O��AAC AI�ˡ�����
	SafeStrCpy( msgChat.Msg, pmsg->Msg, MAX_CHAT_LENGTH+1 );
	g_Network.Send2Server( dwConnectionIndex, (char*)&msgChat, msgChat.GetMsgLength() );

	//---��i����i���ˡ�A��ic��O��"?�ˢ碮��O �����ˡ��ϩ���i����a
	msgChat.Protocol	= MP_CHAT_WHISPER;
	SafeStrCpy( msgChat.Name, pmsg->SenderName, MAX_NAME_LENGTH+1 ); //�����ˡ��ϩ���o��ic��O��AAC AI�ˡ�����
	g_Network.Send2User( pReceiverInfo->dwConnectionIndex, (char*)&msgChat, msgChat.GetMsgLength() );	//CHATMSG 040324
}


void Chat_Whisper_Gm_Syn(DWORD dwConnectionIndex, char* pMsg) 
{
	MSG_WHISPER* pmsg = (MSG_WHISPER*)pMsg;
	USERINFO* pReceiverInfo = g_pUserTableForObjectID->FindUser( pmsg->dwReceiverID );
	if( !pReceiverInfo ) return;

//			GM�� �Ӹ��� ��� �Ӹ� �źδ� ������� �ʴ´�.
//			if( pReceiverInfo->GameOption.bNoWhisper )
//			{
//				MSG_BYTE msg;
//				msg.Category	= MP_CHAT;
//				msg.Protocol	= MP_CHAT_WHISPER_NACK;
//				msg.dwObjectID	= pmsg->dwObjectID;
//				msg.bData		= CHATERR_OPTION_NOWHISPER;

//				g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));
//				break;
//			}

	MSG_CHAT msgChat;
	msgChat.Category	= MP_CHAT;
	msgChat.Protocol	= MP_CHAT_WHISPER_ACK;
	msgChat.dwObjectID	= pmsg->dwObjectID;	
	SafeStrCpy( msgChat.Name, pmsg->ReceiverName, MAX_NAME_LENGTH+1 );
	SafeStrCpy( msgChat.Msg, pmsg->Msg, MAX_CHAT_LENGTH+1 );
	g_Network.Send2Server( dwConnectionIndex, (char*)&msgChat, msgChat.GetMsgLength() );

	msgChat.Protocol	= MP_CHAT_WHISPER_GM;
	SafeStrCpy( msgChat.Name, pmsg->SenderName, MAX_NAME_LENGTH+1 );
	g_Network.Send2User( pReceiverInfo->dwConnectionIndex, (char*)&msgChat, msgChat.GetMsgLength() );	//CHATMSG 040324
}


void Chat_Whisper_Ack(char* pMsg, DWORD dwLength) 
{
	MSG_CHAT* pmsg = (MSG_CHAT*)pMsg;
	USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );			
	if( !pInfo ) return;

	g_Network.Send2User( pInfo->dwConnectionIndex, (char*)pmsg, dwLength );
}


void Chat_Whisper_Nack(char* pMsg, DWORD dwLength) 
{
	MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pSenderInfo ) return;
	
	g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)pmsg, dwLength );
}


void Chat_Party(char* pMsg) 
{
	MSG_CHAT_WITH_SENDERID* pmsg = (MSG_CHAT_WITH_SENDERID*)pMsg;
	USERINFO* pReceiverInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pReceiverInfo ) return;
	
	g_Network.Send2User( pReceiverInfo->dwConnectionIndex, (char*)pmsg, pmsg->GetMsgLength() );	//CHATMSG 040324
}


void Chat_Guild(char* pMsg, DWORD dwLength) 
{
	TESTMSGID * pmsg = (TESTMSGID*)pMsg;
	USERINFO* pReceiverInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pReceiverInfo ) return;
	
	TransToClientMsgParser(pReceiverInfo->dwConnectionIndex, pMsg, dwLength);
}

// 070601 LYW --- Add function for famil chatting.
void Chat_Family( char* pMsg, DWORD dwLength )
{
	// desc_hseos_�йи�01
	// S �йи� �߰� added by hseos 2007.11.08
/*
	SEND_FAMILY_CHAT * pmsg = (SEND_FAMILY_CHAT*)pMsg;
	USERINFO* pReceiverInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pReceiverInfo ) return;
	
	TransToClientMsgParser(pReceiverInfo->dwConnectionIndex, pMsg, dwLength);
*/
	MSG_CHAT* pstPacket = (MSG_CHAT*)pMsg;
	CSHFamily* pcsFamily = g_csFamilyManager.GetFamily(pstPacket->dwObjectID);
	if (pcsFamily == NULL) return;

	MSG_CHAT stPacket;
	USERINFO* pMemberInfo;
	for(UINT i=0; i<pcsFamily->Get()->nMemberNum; i++)
	{
		pMemberInfo = g_pUserTableForObjectID->FindUser(pcsFamily->GetMember(i)->Get()->nID);
		if (pMemberInfo == NULL) continue;

		stPacket.Category		= MP_CHAT;
		stPacket.Protocol		= MP_CHAT_FAMILY;
		stPacket.dwObjectID		= pMemberInfo->dwCharacterID;
		SafeStrCpy(stPacket.Name, pstPacket->Name, MAX_NAME_LENGTH+1);
		SafeStrCpy(stPacket.Msg, pstPacket->Msg, MAX_CHAT_LENGTH+1);

		g_Network.Send2User(&stPacket, sizeof(stPacket));
	}
	// E �йи� �߰� added by hseos 2007.11.08
}


void Chat_GuildUnion(char* pMsg, DWORD dwLength)
{
	MSG_CHAT_WITH_SENDERID* pmsg = (MSG_CHAT_WITH_SENDERID*)pMsg;
	USERINFO* pReceiverInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pReceiverInfo ) return;
	
	TransToClientMsgParser(pReceiverInfo->dwConnectionIndex, pMsg, dwLength);
}

// 080312 NYJ --- ��ü��ġ�� ó�� �����Լ� ����
//void Chat_Shout_Sendserver(char* pMsg) 
//{
//	SEND_SHOUTBASE* pmsg = (SEND_SHOUTBASE*)pMsg;
//			
//	SHOUTMGR->AddShoutMsg( &pmsg->ShoutMsg[0] );
//}

void Chat_Shout_Ack(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	Send_Shout_Ack_AllUser(pMsg);
}

void Send_Shout_Ack_AllUser(char* pMsg)
{
	SEND_SHOUTBASE_ITEMINFO* pmsg = (SEND_SHOUTBASE_ITEMINFO*)pMsg;
	if(!pmsg)		return;

	// �� Agent������ ����� ����������� ����
	MSG_CHAT_WORD msgtoUser;
	msgtoUser.Category = MP_CHAT;
	msgtoUser.Protocol = MP_CHAT_SHOUT_ACK;
	msgtoUser.dwObjectID = pmsg->dwObjectID;
	sprintf(msgtoUser.Msg, "[%s]: %s", pmsg->Name, pmsg->Msg);

	USERINFO * info = NULL;
	g_pUserTable->SetPositionUserHead();
	while(info = (USERINFO *)g_pUserTable->GetUserData())
	{
		MSG_CHAT_WORD msgdata = msgtoUser;
		g_Network.Send2User( info->dwConnectionIndex, (char*)&msgdata, msgdata.GetMsgLength());
	}
}

void MP_CHATMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)	//from client
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	
	USERINFO* pInfoForCheck = g_pUserTable->FindUser( dwConnectionIndex );
	
	if( pInfoForCheck )
	{
		if( gCurTime - pInfoForCheck->dwLastChatTime < 1000 )	//1��
		{
			//����
			MSGBASE msg;
			msg.Category	= MP_CHAT;
			msg.Protocol	= MP_CHAT_FASTCHAT;
			msg.dwObjectID	= pTempMsg->dwObjectID;
			g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
			return;
		}
		
		pInfoForCheck->dwLastChatTime = gCurTime;

		// 081205 LUJ, GM�� ���� ��ȭ�� �α׿� �����Ѵ�
		switch( pInfoForCheck->UserLevel )
		{
		case eUSERLEVEL_GM:
		case eUSERLEVEL_PROGRAMMER:
		case eUSERLEVEL_DEVELOPER:
			{
				const MSG_CHAT_WORD* const message = ( MSG_CHAT_WORD* )pMsg;

				TCHAR chat[ MAX_CHAT_LENGTH + 1 ] = { 0 };
				SafeStrCpy( chat, message->Msg, sizeof( chat ) );

			/*	g_DB.LogMiddleQuery(
					0,
					0,
					_T( "EXEC dbo.TP_CHAT_LOG_INSERT %d, \'%s\'" ),
					message->dwObjectID,
					chat );*/
				break;
			}
		}
	}

	switch(pTempMsg->Protocol)
	{
	case MP_CHAT_ALL:
	case MP_CHAT_SMALLSHOUT:
	case MP_CHAT_WHOLE :
	case MP_CHAT_TRADE :
	case MP_CHAT_MONSTERSPEECH:
	case MP_CHAT_NORMAL :					chatmsg_normal(dwConnectionIndex, pMsg, dwLength) ;			break;
	case MP_CHAT_WHISPER_SYN:				chatmsg_whisper_syn(dwConnectionIndex, pMsg) ;				break;
	case MP_CHAT_PARTY:						chatmsg_party(pMsg) ;										break;
	case MP_CHAT_GUILD:						chatmsg_guild(pMsg, dwLength) ;								break;
	case MP_CHAT_FAMILY :					chatmsg_family(pMsg, dwLength) ;							break ;
	case MP_CHAT_GUILDUNION:				chatmsg_guildunion(pMsg, dwLength) ;						break;
	// 070321 LYW --- AgentNetworkMsgParser : Add protocol to fine user.
	case MP_CHAT_FINDUSER :					chatmsg_FindUser(dwConnectionIndex, pMsg, dwLength) ;		break ;

		/*
	case MP_CHAT_SHOUT_SYN:
		{
			SEND_SHOUTBASE* pmsg = (SEND_SHOUTBASE*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if( !pSenderInfo )	return;

			SHOUTRECEIVE rev;
			SHOUTMGR->AddShoutMsg( &pmsg->ShoutMsg[0], &rev );

			// �޼����� �� ��������
			if( rev.Count == 0 )
			{
				MSGBASE msg;				
				msg.Category = MP_CHAT;
				msg.Protocol = MP_CHAT_SHOUT_NACK;

				g_Network.Send2User( &msg, sizeof(msg) );
			}
			else
			{
				SEND_SHOUTRECEIVE msg;
				msg.Category = MP_CHAT;
				msg.Protocol = MP_CHAT_SHOUT_ACK;
				msg.Receive = rev;

				g_Network.Send2User( pSenderInfo->dwConnectionIndex,  (char*)&msg, sizeof(msg) );

				pmsg->Protocol = MP_CHAT_SHOUT_SENDSERVER;
				g_Network.Broadcast2AgentServerExceptSelf( (char*)pmsg, dwLength );
			}
		}
		break;

	case MP_CHAT_SHOUT_SENDSERVER:
		{
			SEND_SHOUTBASE* pmsg = (SEND_SHOUTBASE*)pMsg;
			
			SHOUTMGR->AddShoutMsg( &pmsg->ShoutMsg[0] );
		}
		break;		
		*/
		
	default:
		ASSERT(0);
		//GAMESYSTEM_OBJ->m_ConnectionTable.SendToCharacter(pTempMsg,dwLength);
		break;
	}
}


void chatmsg_normal(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	TransToMapServerMsgParser(dwConnectionIndex,pMsg,dwLength);
}

// 070321 LYW --- Add function to find user.
void chatmsg_FindUser(DWORD dwConnectionIndex, char*pMsg, DWORD dwLength)
{
	//TransToMapServerMsgParser(dwConnectionIndex,pMsg,dwLength);

	MSG_CHAT* pmsg = (MSG_CHAT*)pMsg ;

	char buf[ MAX_NAME_LENGTH+1 ] = {0, } ;

	SafeStrCpy( buf, pmsg->Name, MAX_NAME_LENGTH+1 ) ;

	DWORD dwKeyOut;
	if( g_MsgTable.AddMsg( pmsg, &dwKeyOut ) )
	{
		SearchWhisperUserAndSend( pmsg->dwObjectID, buf, dwKeyOut );
	}
}


void chatmsg_whisper_syn(DWORD dwConnectionIndex, char* pMsg)
{
	MSG_CHAT* pmsg = (MSG_CHAT*)pMsg;

	USERINFO* pInfo = g_pUserTable->FindUser( dwConnectionIndex );
	if( !pInfo )	return;

	//---Ŭ���̾�Ʈ���� �� ������ ���� �� ����. buf�� �ű��.
	char buf[MAX_NAME_LENGTH+1];
	SafeStrCpy( buf, pmsg->Name, MAX_NAME_LENGTH+1 );
	int nLen = strlen( buf );
	if( nLen < 4 ) return;		//ĳ���� �̸��� 4���� �����ϸ��� ����.

	// desc_hseos_�������͸�01
	// S �������͸� �߰� added by hseos 2007.06.25
	// ..���Ϳ��� ������ ���� ó���� ���� Ÿ���� �����Ѵ�.
	FILTERTABLE->SetExceptionInvalidCharIncludeType(FET_WHISPER_CHARNAME);
	// E �������͸� �߰� added by hseos 2007.06.25
	if( (FILTERTABLE->IsInvalidCharInclude((unsigned char*)buf)) == TRUE )
	{
		MSG_BYTE msg;
		msg.Category	= MP_CHAT;
		msg.Protocol	= MP_CHAT_WHISPER_NACK;
		msg.dwObjectID	= pmsg->dwObjectID;
		msg.bData		= CHATERR_NO_NAME;
		g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
		return;
	}

	DWORD dwKeyOut;
	if( g_MsgTable.AddMsg( pmsg, &dwKeyOut ) )
	{
		SearchWhisperUserAndSend( pmsg->dwObjectID, buf, dwKeyOut );
	}
	else
	{
		MSG_BYTE msg;
		msg.Category	= MP_CHAT;
		msg.Protocol	= MP_CHAT_WHISPER_NACK;
		msg.dwObjectID	= pmsg->dwObjectID;
		msg.bData		= CHATERR_ERROR;
		g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
		return ;
	}
}


void chatmsg_party(char* pMsg) 
{
	SEND_PARTY_CHAT* pmsg =	(SEND_PARTY_CHAT*)pMsg;
	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if(!pSenderInfo)
		return;
	
	MSG_CHAT_WITH_SENDERID msgChat;
	msgChat.Category	= pmsg->Category;
	msgChat.Protocol	= pmsg->Protocol;
	msgChat.dwSenderID	= pmsg->dwObjectID;

	SafeStrCpy( msgChat.Name, pmsg->Name, MAX_NAME_LENGTH+1 );
	SafeStrCpy( msgChat.Msg, pmsg->Msg, MAX_CHAT_LENGTH+1 );

	for(int i=0; i<pmsg->MemberNum; ++i)
	{
		if(pmsg->MemberID[i] != 0)
		{
			USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pmsg->MemberID[i]);
			msgChat.dwObjectID	= pmsg->MemberID[i];
			if(pRecverInfo)
			{
//						g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msgChat, msgChat.GetMsgLength() );	//CHATMSG 040324
				MSG_CHAT_WITH_SENDERID msgTemp = msgChat;
				g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msgTemp, msgTemp.GetMsgLength() );	//CHATMSG 040324
			}
			else
			{						
				g_Network.Broadcast2AgentServerExceptSelf( (char*)&msgChat, msgChat.GetMsgLength() );
			}
		}
	}
}


void chatmsg_guild(char* pMsg, DWORD dwLength) 
{
	TESTMSGID* pmsg = (TESTMSGID*)pMsg;
	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser(pmsg->dwSenderID);
	if(!pSenderInfo)
		return;
	g_Network.Broadcast2MapServer(pMsg, dwLength);
}

// 070601 LYW --- AgentNetMsgParser : Add function for family chatting.
void chatmsg_family(char* pMsg, DWORD dwLength)
{
	// desc_hseos_�йи�01
	// S �йи� �߰� added by hseos 2007.07.13	2007.11.08
	/*
	SEND_FAMILY_CHAT* pmsg = (SEND_FAMILY_CHAT*)pMsg;
	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if(!pSenderInfo)
		return;
	g_Network.Broadcast2MapServer(pMsg, dwLength);
	*/

	MSG_CHAT* pstPacket = (MSG_CHAT*)pMsg;
	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pstPacket->dwObjectID);
	if (pUserInfo == NULL) return;
	if (pUserInfo->pcsFamily == NULL) return;

	MSG_CHAT stPacket;
	USERINFO* pMemberInfo;
	for(UINT i=0; i<pUserInfo->pcsFamily->Get()->nMemberNum; i++)
	{
		pMemberInfo = g_pUserTableForObjectID->FindUser(pUserInfo->pcsFamily->GetMember(i)->Get()->nID);
		if (pMemberInfo == NULL) continue;

		stPacket.Category		= MP_CHAT;
		stPacket.Protocol		= MP_CHAT_FAMILY;
		stPacket.dwObjectID		= pMemberInfo->dwCharacterID;
		SafeStrCpy(stPacket.Name, pstPacket->Name, MAX_NAME_LENGTH+1);
		SafeStrCpy(stPacket.Msg, pstPacket->Msg, MAX_CHAT_LENGTH+1);

		g_Network.Send2User(&stPacket, sizeof(stPacket));
	}

	// �ٸ� ������Ʈ���� ������
	stPacket.Category		= MP_CHAT;
	stPacket.Protocol		= MP_CHAT_FAMILY;
	stPacket.dwObjectID		= pUserInfo->pcsFamily->Get()->nID;
	SafeStrCpy(stPacket.Name, pstPacket->Name, MAX_NAME_LENGTH+1);
	SafeStrCpy(stPacket.Msg, pstPacket->Msg, MAX_CHAT_LENGTH+1);
	g_Network.Broadcast2AgentServerExceptSelf( (char*)&stPacket, sizeof(stPacket));
	// E �йи� �߰� added by hseos 2007.07.13	2007.11.08
}


void chatmsg_guildunion(char* pMsg, DWORD dwLength) 
{
	MSG_CHAT_WITH_SENDERID* pmsg = (MSG_CHAT_WITH_SENDERID*)pMsg;
	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwSenderID );
	if( !pSenderInfo )	return;
	g_Network.Broadcast2MapServer( pMsg, dwLength );
}



void MP_PARTYServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_PARTY_NOTIFYADD_TO_MAPSERVER:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	case MP_PARTY_NOTIFYDELETE_TO_MAPSERVER:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	case MP_PARTY_NOTIFYCHANGEMASTER_TO_MAPSERVER:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	case MP_PARTY_NOTIFYBREAKUP_TO_MAPSERVER:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	case MP_PARTY_NOTIFYBAN_TO_MAPSERVER:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	case MP_PARTY_NOTIFYMEMBER_LOGIN_TO_MAPSERVER:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	case MP_PARTY_NOTIFYMEMBER_LOGOUT_TO_MAPSERVER:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	case MP_PARTY_NOTIFYMEMBER_LOGINMSG:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex,pMsg,dwLength);			
		}
		break;
	case MP_PARTY_NOTIFYCREATE_TO_MAPSERVER:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex,pMsg,dwLength);
		}
		break;
	case MP_PARTY_NOTIFYMEMBER_LEVEL:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex,pMsg,dwLength);
		}
		break;

	// 071002 LYW --- AgentNetworkMsgParser : Add process to change option of party.
	case MP_PARTY_NOTIFY_CHANGE_OPTION :
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex,pMsg,dwLength);
		}
		break ;


	default:
		TransToClientMsgParser(dwConnectionIndex,pMsg,dwLength);
		break;
	}
}

void TransToClientMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	g_Network.Send2User(pTempMsg,dwLength);
}

void TransToMapServerMsgParser(DWORD dwConnectionIndex/*A?Au��?�ˢ� �ˡ�eCNConnectionIndex*/, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;

	USERINFO * userinfo = (USERINFO *)g_pUserTable->FindUser(dwConnectionIndex);
	if(userinfo == NULL)
		return;

	g_Network.Send2Server(userinfo->dwMapServerConnectionIndex,pMsg,dwLength);
}
void MP_PACKEDMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_PACKEDDATA_NORMAL:
		{
			static char TempData[MAX_PACKEDDATA_SIZE];
			SEND_PACKED_DATA * pack =  (SEND_PACKED_DATA *)pMsg;
			DWORD* charterIDs = (DWORD*)&pack->Data[pack->wRealDataSize];
			for(int n=0;n<pack->wReceiverNum;++n)
			{
				USERINFO * userInfo = g_pUserTableForObjectID->FindUser(charterIDs[n]);
				// taiyo :
				// charterIDs[n] �����ˢ� Ca��?eCIAo ��uE�ˡ�A ID�����ˢ� �ϩ�N��ui��?I��u���� NULLAI ��iC��ui��u���� �ˡ���cAIA�ϩ��ˡ���c
				if(!userInfo)
				{
					continue;
				}

				memcpy(TempData,pack->Data,pack->wRealDataSize);
				TempData[pack->wRealDataSize] = 0;
				g_Network.Send2User(userInfo->dwConnectionIndex, TempData, pack->wRealDataSize);
			}
		}
		break;

	case MP_PACKEDDATA_TOMAPSERVER:
		{
			SEND_PACKED_TOMAPSERVER_DATA* pmsg = (SEND_PACKED_TOMAPSERVER_DATA*)pMsg;
			WORD ToMapPort = g_pServerTable->GetServerPort( eSK_MAP, pmsg->ToMapNum);
			if(ToMapPort == 0)
				return;

			SERVERINFO* pInfo = g_pServerTable->FindServer(ToMapPort);
			if(pInfo == NULL)
				return;

			g_Network.Send2Server(pInfo->dwConnectionIndex,pmsg->Data,pmsg->wRealDataSize);						
		}
		break;
	case MP_PACKEDDATA_TOBROADMAPSERVER:
		{
			SEND_PACKED_TOMAPSERVER_DATA* pmsg = (SEND_PACKED_TOMAPSERVER_DATA*)pMsg;

			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pmsg->Data,pmsg->wRealDataSize);
			//AI����?�ˡ�e AI ��?�ˢ�AIAu������c��?�ˢ� ��?��������aAI �ϡ���!��a �ˡ�E ��u���Ϩ�oo�ˡ�A? 
		}
		break;
	}
}

void MP_FRIENDMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_FRIEND_LOGIN:								Friend_Login(pTempMsg) ;						break;
	case MP_FRIEND_ADD_SYN:								Friend_Add_Syn(pMsg) ;							break;
	case MP_FRIEND_ADD_ACCEPT:							Friend_Add_Accept(pMsg) ;						break;
	case MP_FRIEND_ADD_DENY:							Friend_Add_Deny(pMsg) ;							break;
	case MP_FRIEND_DEL_SYN:								Friend_Del_Syn(pMsg) ;							break;
	case MP_FRIEND_DELID_SYN:							Friend_Delid_Syn(pMsg) ;						break;
	case MP_FRIEND_ADDID_SYN:							Friend_Addid_Syn(pMsg) ;						break;
	case MP_FRIEND_LOGOUT_NOTIFY_TO_AGENT:				Friend_Logout_Notify_To_Agent(pMsg)  ;			break;
	case MP_FRIEND_LOGOUT_NOTIFY_AGENT_TO_AGENT:		Friend_Logout_Notify_Agent_To_Agent(pMsg)  ;	break;			
	case MP_FRIEND_LIST_SYN:							Friend_List_Syn(pMsg) ;							break;
	case MP_FRIEND_ADD_ACK_TO_AGENT:					Friend_Add_Ack_To_Agent(pMsg) ;					break;
	case MP_FRIEND_ADD_NACK_TO_AGENT:					Friend_Add_Nack_To_Agent(pMsg) ;				break;
	case MP_FRIEND_ADD_ACCEPT_TO_AGENT:					Friend_Add_Accept_To_Agent(pMsg) ;				break;
	case MP_FRIEND_ADD_ACCEPT_NACK_TO_AGENT:			Friend_Add_Accept_Nack_To_Agent(pMsg) ;			break;
	case MP_FRIEND_LOGIN_NOTIFY_TO_AGENT:				Friend_Login_Notify_To_Agent(pMsg) ;			break;
	case MP_FRIEND_ADD_INVITE_TO_AGENT:					Friend_Add_Invite_To_Agent(pMsg) ;				break;
	case MP_FRIEND_ADD_NACK:							Friend_Add_Nack(pMsg) ;							break;		
	default:							TransToClientMsgParser(dwConnectionIndex,pMsg,dwLength);		break;
	}	
}

void Friend_Login(MSGBASE* pTempMsg)
{
	USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pTempMsg->dwObjectID);
	if(!userinfo)
		return;
	//�ϩ����̡ˡ�| ��ii����ICN A��I�����ˡ���ie��?�ˢ碮��O ����I��������AI ��uE�ˡ��ϡ�
	FriendNotifyLogintoClient(pTempMsg->dwObjectID);
	//�ϩ���i�����ˢ� ��ii����ICN A��I�����ˡ���ieAI ����I��������AIAIAo ��uE��u������?E
//	FriendGetLoginFriends(pTempMsg->dwObjectID); CE��a ������A ��a��EAI�Ƣ�... 
	NoteIsNewNote(pTempMsg->dwObjectID);
}


void Friend_Add_Syn(char* pMsg)
{
	MSG_NAME* pmsg = (MSG_NAME*)pMsg;
	USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if(!userinfo)
		return;
	
	char buf[MAX_NAME_LENGTH+1];
	SafeStrCpy( buf, pmsg->Name, MAX_NAME_LENGTH+1 );

	//�������� üũ "'"��...
	if( FILTERTABLE->IsInvalidCharInclude( (unsigned char*) buf ) )
		return;
	
	FriendGetUserIDXbyName(pmsg->dwObjectID, buf);
}


void Friend_Add_Accept(char* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if(!userinfo)
		return;
	FriendAddFriend(pmsg->dwData, pmsg->dwObjectID);//db insert
}


void Friend_Add_Deny(char* pMsg) 
{
	MSG_FRIEND_MEMBER_ADDDELETEID* pmsg = (MSG_FRIEND_MEMBER_ADDDELETEID*)pMsg;
			
	USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->PlayerID);
	if(!userinfo)
		return;

	MSG_FRIEND_MEMBER_ADDDELETEID msg;
	msg.Category = MP_FRIEND;
	msg.Protocol = MP_FRIEND_ADD_NACK;
	SafeStrCpy( msg.Name, pmsg->Name, MAX_NAME_LENGTH + 1 );
	msg.PlayerID = eFriend_AddDeny;
	g_Network.Send2User(userinfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
}


void Friend_Del_Syn(char* pMsg) 
{
	MSG_NAME * pmsg = (MSG_NAME*)pMsg;
	FriendDelFriend(pmsg->dwObjectID, pmsg->Name); //DB
	//msg
}


void Friend_Delid_Syn(char* pMsg) 
{
	MSG_DWORD2 * pmsg = (MSG_DWORD2*)pMsg;
			
	FriendDelFriendID(pmsg->dwObjectID, pmsg->dwData1, pmsg->dwData2);
}


void Friend_Addid_Syn(char* pMsg) 
{
	MSG_FRIEND_MEMBER_ADDDELETEID * pmsg = (MSG_FRIEND_MEMBER_ADDDELETEID*)pMsg;
			
	FriendIsValidTarget(pmsg->dwObjectID, pmsg->PlayerID, pmsg->Name);
}


void Friend_Logout_Notify_To_Agent(char* pMsg) 
{
	MSG_NAME_DWORD* pmsg = (MSG_NAME_DWORD*)pMsg;
	USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if(userinfo)
	{
		pmsg->Protocol = MP_FRIEND_LOGOUT_NOTIFY_TO_CLIENT;
		g_Network.Send2User(userinfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_NAME));
	}
	else
	{
		pmsg->Protocol = MP_FRIEND_LOGOUT_NOTIFY_AGENT_TO_AGENT;
		g_Network.Broadcast2AgentServerExceptSelf( (char*)pmsg, sizeof(MSG_NAME));
	}
}


void Friend_Logout_Notify_Agent_To_Agent(char* pMsg) 
{
	MSG_NAME_DWORD* pmsg = (MSG_NAME_DWORD*)pMsg;
	USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if(userinfo)
	{
		pmsg->Protocol = MP_FRIEND_LOGOUT_NOTIFY_TO_CLIENT;
		g_Network.Send2User(userinfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_NAME));
	}
}


void Friend_List_Syn(char* pMsg) 
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;
	USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if(!userinfo)
		return;
				
	FriendGetFriendList(pmsg->dwObjectID);
}


void Friend_Add_Ack_To_Agent(char* pMsg) 
{
	MSG_FRIEND_MEMBER_ADDDELETEID* pmsg = (MSG_FRIEND_MEMBER_ADDDELETEID*)pMsg;
	USERINFO * pRecverInfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if(!pRecverInfo) return;
	pmsg->Protocol = MP_FRIEND_ADD_ACK;
	g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_FRIEND_MEMBER_ADDDELETEID));
}


void Friend_Add_Nack_To_Agent(char* pMsg) 
{
	MSG_FRIEND_MEMBER_ADDDELETEID* pmsg = (MSG_FRIEND_MEMBER_ADDDELETEID*)pMsg;
	USERINFO * pRecverInfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if(!pRecverInfo) return;
	pmsg->Protocol = MP_FRIEND_ADD_NACK;
	g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_FRIEND_MEMBER_ADDDELETEID));
}


void Friend_Add_Accept_To_Agent(char* pMsg)
{
	MSG_FRIEND_MEMBER_ADDDELETEID * pmsg = (MSG_FRIEND_MEMBER_ADDDELETEID*)pMsg;
	USERINFO* pToRecverInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if(pToRecverInfo)
	{			
		pmsg->Protocol = MP_FRIEND_ADD_ACCEPT_ACK;
		g_Network.Send2User(pToRecverInfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_FRIEND_MEMBER_ADDDELETEID));
	}
}


void Friend_Add_Accept_Nack_To_Agent(char* pMsg)
{
	MSG_FRIEND_MEMBER_ADDDELETEID * pmsg = (MSG_FRIEND_MEMBER_ADDDELETEID*)pMsg;
	USERINFO* pToRecverInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if(!pToRecverInfo) return;
	pmsg->Protocol = MP_FRIEND_ADD_ACCEPT_NACK;
	g_Network.Send2User(pToRecverInfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_FRIEND_MEMBER_ADDDELETEID));
}


void Friend_Login_Notify_To_Agent(char* pMsg)
{
	MSG_NAME_DWORD* pmsg = (MSG_NAME_DWORD*)pMsg;
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if(pRecverInfo)
	{
		pmsg->Protocol = MP_FRIEND_LOGIN_NOTIFY;
		g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_NAME_DWORD));
	}
}


void Friend_Add_Invite_To_Agent(char* pMsg)
{
	MSG_FRIEND_MEMBER_ADDDELETEID* pmsg = (MSG_FRIEND_MEMBER_ADDDELETEID*)pMsg;
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if(pRecverInfo)
	{
		if(pRecverInfo->GameOption.bNoFriend == TRUE)
		{
			//nack
			MSG_FRIEND_MEMBER_ADDDELETEID msg;
			msg.Category = MP_FRIEND;
			msg.dwObjectID = pmsg->PlayerID;
			msg.Protocol = MP_FRIEND_ADD_NACK;
			strcpy( msg.Name,  "");
			msg.PlayerID = eFriend_OptionNoFriend;	//errcode insert

			g_Network.Broadcast2AgentServerExceptSelf((char*)&msg, sizeof(msg));
			return;
		}	
		else //ack
		{
			pmsg->Protocol = MP_FRIEND_ADD_INVITE;
			g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_FRIEND_MEMBER_ADDDELETEID));
		}
	}
}


void Friend_Add_Nack(char* pMsg) 
{
	MSG_FRIEND_MEMBER_ADDDELETEID * pmsg = (MSG_FRIEND_MEMBER_ADDDELETEID*)pMsg;
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if(pRecverInfo)
	{
		g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_FRIEND_MEMBER_ADDDELETEID));
	}
}



void MP_NOTEServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{//////////////////////////////////////////////////////////////////////////
	//AEAo
	case MP_NOTE_SENDNOTE_SYN:
		{
			MSG_FRIEND_SEND_NOTE * pmsg = (MSG_FRIEND_SEND_NOTE *) pMsg;
			USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->FromId);
			if(!userinfo)
			{
				return;
			}

			char bufFrom[MAX_NAME_LENGTH+1];
			char bufTo[MAX_NAME_LENGTH+1];
			char bufNote[MAX_NOTE_LENGTH+1];
			SafeStrCpy( bufFrom, pmsg->FromName, MAX_NAME_LENGTH+1 );
			SafeStrCpy( bufTo, pmsg->ToName, MAX_NAME_LENGTH+1 );
			SafeStrCpy( bufNote, pmsg->Note, MAX_NOTE_LENGTH+1 );
			
			if( FILTERTABLE->IsInvalidCharInclude( (unsigned char*)bufFrom ) ||
				FILTERTABLE->IsInvalidCharInclude( (unsigned char*)bufTo ) )
				return;
			//pmsg->Note[MAX_NOTE_LENGTH] = 0; // �� ���ڰ� �����°���? 

			NoteServerSendtoPlayer(pmsg->FromId, bufFrom, bufTo, bufNote);
		}
		break;
	}
}
void MP_NOTEMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{//////////////////////////////////////////////////////////////////////////
	//AEAo
	case MP_NOTE_SENDNOTE_SYN:			Note_SendNote_Syn(pMsg) ;			break;
	case MP_NOTE_SENDNOTEID_SYN:		Note_SendNoteId_Syn(pMsg) ;			break;
	case MP_NOTE_RECEIVENOTE:			Note_ReceiveNote(pMsg) ;			break;		
	case MP_NOTE_DELALLNOTE_SYN:		Note_DelAllNote_Syn(pMsg) ;			break;
	case MP_NOTE_NOTELIST_SYN:			Note_NoteList_Syn(pMsg) ;			break;
	case MP_NOTE_READNOTE_SYN:			Note_ReadNote_Syn(pMsg) ;			break;
	case MP_NOTE_DELNOTE_SYN:			Note_DelNote_Syn(pMsg) ;			break;
	}
}

void Note_SendNote_Syn(char* pMsg) 
{
	MSG_FRIEND_SEND_NOTE * pmsg = (MSG_FRIEND_SEND_NOTE *) pMsg;
	USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->FromId);
	if(!userinfo)
	{
		return;
	}

	char bufFrom[MAX_NAME_LENGTH+1];
	char bufTo[MAX_NAME_LENGTH+1];
	char bufNote[MAX_NOTE_LENGTH+1];
	SafeStrCpy( bufFrom, pmsg->FromName, MAX_NAME_LENGTH+1 );
	SafeStrCpy( bufTo, pmsg->ToName, MAX_NAME_LENGTH+1 );
	SafeStrCpy( bufNote, pmsg->Note, MAX_NOTE_LENGTH+1 );

	if( FILTERTABLE->IsInvalidCharInclude( (unsigned char*)bufFrom ) ||
		FILTERTABLE->IsInvalidCharInclude( (unsigned char*)bufTo ) ||
		FILTERTABLE->IsCharInString(bufNote, "'") )
		return;
	//pmsg->Note[MAX_NOTE_LENGTH] = 0; // �� ���ڰ� �����°���? 

	NoteSendtoPlayer(pmsg->FromId, bufFrom, bufTo, bufNote);
}


void Note_SendNoteId_Syn(char* pMsg) 
{
	MSG_FRIEND_SEND_NOTE_ID * pmsg = (MSG_FRIEND_SEND_NOTE_ID*)pMsg;
	NoteSendtoPlayerID(pmsg->dwObjectID, pmsg->FromName, pmsg->TargetID, pmsg->Note);
}


void Note_ReceiveNote(char* pMsg) 
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if(!userinfo)
	{
		return;
	}
	g_Network.Send2User(userinfo->dwConnectionIndex, (char*)pmsg, sizeof(MSGBASE));
}


void Note_DelAllNote_Syn(char* pMsg) 
{
	MSGBASE * pmsg = (MSGBASE*)pMsg;
	NoteDelAll(pmsg->dwObjectID);

	USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if(!userinfo)
		return;
	MSGBASE msg;
	msg.Category = MP_NOTE;
	msg.Protocol = MP_NOTE_DELALLNOTE_ACK;
	msg.dwObjectID = pmsg->dwObjectID;

	g_Network.Send2User(userinfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
}


void Note_NoteList_Syn(char* pMsg) 
{
	MSG_WORD2 * pmsg = (MSG_WORD2*)pMsg;
	NoteList(pmsg->dwObjectID, pmsg->wData1, pmsg->wData2);
}


void Note_ReadNote_Syn(char* pMsg)
{
	MSG_DWORD2 * pmsg = (MSG_DWORD2*)pMsg;
	USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if(!userinfo)
		return;

	NoteRead(pmsg->dwObjectID, pmsg->dwData1, pmsg->dwData2);
}


void Note_DelNote_Syn(char* pMsg) 
{
	MSG_FRIEND_DEL_NOTE* pmsg = (MSG_FRIEND_DEL_NOTE*)pMsg;

	if( pmsg->bLast == 1 || pmsg->bLast == 0 )
		NoteDelete(pmsg->dwObjectID, pmsg->NoteID, pmsg->bLast);
}



void MP_WANTEDServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_WANTED_NOTIFY_DELETE_TO_MAP:
	case MP_WANTED_NOTIFY_REGIST_TO_MAP:
	case MP_WANTED_NOTIFY_NOTCOMPLETE_TO_MAP:
	case MP_WANTED_DESTROYED_TO_MAP:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	case MP_WANTED_NOTCOMPLETE_TO_AGENT:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(!pUserInfo)
				return;
			pmsg->Protocol = MP_WANTED_NOTCOMPLETE_BY_DELCHR;
			g_Network.Send2Server(pUserInfo->dwMapServerConnectionIndex, (char*)pmsg, sizeof(MSG_DWORD));
		}
		break;
	default:
		TransToClientMsgParser(dwConnectionIndex,pMsg,dwLength);
		break;
	}
}

/*
void MP_MORNITORTOOLMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{

		MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	
		switch(pTempMsg->Protocol)
		{
		case MP_MORNITORTOOL_CHECK_ON_SYN:
			{
	
				MAPSERVER_TRANSDATA msg;
				
	
				msg.Category = MP_MORNITORTOOL; 
				msg.Protocol = MP_MORNITORTOOL_CHECK_ON_ACK;
	//			msg.MapServerGroupNum = (MAPSERVER_TRANSDATA*)pMsg->MapServerGroupNum;
	//			msg.MapServerNum = (MAPSERVER_TRANSDATA*)pMsg->MapServerNum;
				msg.MapServerNum = g_pListCollect->GetLoadAgentNum();
				g_Console.LOG(4, "Test Protocol Recv");
				g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(MAPSERVER_TRANSDATA));
	
			}
			break;
		case MP_MORNITORTOOL_MAPSERVER_TURNOFF_SYN:
			{
				MAPSERVER_TRANSDATA msg;
				memcpy(&msg,(MAPSERVER_TRANSDATA*)pMsg,sizeof(MAPSERVER_TRANSDATA));
	
				msg.Category = MP_MORNITORTOOL; 
				msg.Protocol = MP_MORNITORTOOL_MAPSERVER_TURNOFF_ACK;
	//			msg.MapServerGroupNum = (MAPSERVER_TRANSDATA*)pMsg->MapServerGroupNum;
	//			msg.MapServerNum = (MAPSERVER_TRANSDATA*)pMsg->MapServerNum;
			
				g_Console.LOG(4, "Off Request received");
				g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(MAPSERVER_TRANSDATA));
				ExitProcess(0);	
			}
			break;
		case MP_MORNITORTOOL_MESSAGE_SYN:
			{
				MSGBASEDATA msg;
				memcpy(&msg,(MSGBASEDATA*)pMsg,sizeof(MSGBASEDATA));
				msg.Messagebuff;
				g_Console.LOG(4, msg.Messagebuff);
			}
			break;
		case MP_MORNITORTOOL_USERLOGOUT_SYN:
			{
				//A?Au ����I�������� ��u������?oA�ϩ��ˡ���c 
				g_Console.LOG(4, "User Logout Request received");
			}
			break;
	
	
		}
	
}*/

void MP_MURIMNETServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_MURIMNET_CHANGETOMURIMNET_ACK:			MurimNet_Changetomurimnet_ack(pMsg) ;								break;
	case MP_MURIMNET_PR_START:						MurimNet_Pr_Start( dwConnectionIndex,  pMsg, dwLength )  ;			break;		
	case MP_MURIMNET_DISCONNECT_ACK:				MurimNet_Disconnect_Ack( dwConnectionIndex, pMsg, dwLength ) ;		break;
	case MP_MURIMNET_RETURNTOMURIMNET_ACK:			MurimNet_ReturnToMurimNet_Ack(pMsg) ;								break;
	default:										TransToClientMsgParser( dwConnectionIndex,pMsg,dwLength);			break;
	}
}


void MurimNet_Changetomurimnet_ack(char* pMsg) 
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	// YH2DO AO��oA CI��ia AU��iu
	WORD MurimnetServerNum = 99;
	WORD MurimnetPort = g_pServerTable->GetServerPort( eSK_MAP, MurimnetServerNum);
	if(MurimnetPort)
	{
		MSGBASE msgserver;
		msgserver.Category = MP_MURIMNET;
		msgserver.Protocol = MP_MURIMNET_CHANGETOMURIMNET_ACK;
		msgserver.dwObjectID = pmsg->dwObjectID;	
		g_Network.Send2User((MSGBASE*)&msgserver, sizeof(MSGBASE));
	}
	else
	{
		MSGBASE msgserver;
		msgserver.Category = MP_MURIMNET;
		msgserver.Protocol = MP_MURIMNET_CHANGETOMURIMNET_NACK;
		msgserver.dwObjectID = pmsg->dwObjectID;
		g_Network.Send2User((MSGBASE*)&msgserver, sizeof(MSGBASE));
	}
}


void MurimNet_Pr_Start( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength ) 
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			
	USERINFO* pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if(!pInfo)
		return;

	WORD MurimnetPort	= g_pServerTable->GetServerPort( eSK_MAP, (WORD)pmsg->dwData);
	SERVERINFO* pSInfo	= g_pServerTable->FindServer(MurimnetPort);
	
	pInfo->wUserMapNum					= (WORD)pmsg->dwData;
	pInfo->dwMapServerConnectionIndex	= pSInfo->dwConnectionIndex;

	// ��u����AACN AE�ˡ�?AI��?�ˢ� �ˡ�A�ˡ�A �ˡ�E��u���Ϩ�oo����I AE�ˡ�?AI ��u����AAA��i ��uE�ˡ��ϡ�
	TransToClientMsgParser(dwConnectionIndex,pMsg,dwLength);
}


void MurimNet_Disconnect_Ack( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	USERINFO* pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if(!pInfo)
		return;

	WORD wMapPort		= g_pServerTable->GetServerPort( eSK_MAP, (WORD)pmsg->dwData);
	SERVERINFO* pSInfo	= g_pServerTable->FindServer(wMapPort);
	
	pInfo->wUserMapNum					= (WORD)pmsg->dwData;
	pInfo->dwMapServerConnectionIndex	= pSInfo->dwConnectionIndex;

	// ��u����AACN AE�ˡ�?AI��?�ˢ� �ˡ�A�ˡ�A �ˡ�E��u���Ϩ�oo����I AE�ˡ�?AI ��u����AAA��i ��uE�ˡ��ϡ�
	TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );
}


void MurimNet_ReturnToMurimNet_Ack(char* pMsg) 
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	USERINFO* pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if(!pInfo)
		return;
	
	// YH2DO AO��oA CI��ia AU��iu
	WORD MurimnetServerNum = 99;
	WORD MurimnetPort = g_pServerTable->GetServerPort( eSK_MAP, MurimnetServerNum);
	if(MurimnetPort)
	{				
		SERVERINFO* pSInfo	= g_pServerTable->FindServer(MurimnetPort);
		
		pInfo->wUserMapNum					= (WORD)MurimnetServerNum;
		pInfo->dwMapServerConnectionIndex	= pSInfo->dwConnectionIndex;
		
		
		MSGBASE msgserver;
		msgserver.Category = MP_MURIMNET;
		msgserver.Protocol = MP_MURIMNET_RETURNTOMURIMNET_ACK;
		msgserver.dwObjectID = pmsg->dwObjectID;	
		g_Network.Send2User((MSGBASE*)&msgserver, sizeof(MSGBASE));
	}
	else
	{
		// ��o��i�ˡ��ϡ��ϩ�YA�ˡ�����I ��i��o��u���������ˢ碮��A�ˡ�A��i��I ��o��i�ˡ��ϡ��ϩ�YAI ��u�Ϩ�A��o.
		MSGBASE msgserver;
		msgserver.Category = MP_MURIMNET;
		msgserver.Protocol = MP_MURIMNET_RETURNTOMURIMNET_NACK;
		msgserver.dwObjectID = pmsg->dwObjectID;	
		g_Network.Send2User((MSGBASE*)&msgserver, sizeof(MSGBASE));
	}
}

void MP_OPTIONServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	default:
		TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );
		break;
	}
}


void MP_OPTIONUserMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_OPTION_SET_SYN:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;

			USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if(!pInfo)
				return;
			( pmsg->wData & eOPT_NOWHISPER )
				? pInfo->GameOption.bNoWhisper = TRUE
				: pInfo->GameOption.bNoWhisper = FALSE;
			( pmsg->wData & eOPT_NOFRIEND )
				? pInfo->GameOption.bNoFriend = TRUE
				: pInfo->GameOption.bNoFriend = FALSE;

			TransToMapServerMsgParser(dwConnectionIndex,pMsg,dwLength);
		}
		break;
	
	default:
		TransToMapServerMsgParser(dwConnectionIndex,pMsg,dwLength);
		break;
	}
}

void MP_MURIMNETUserMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_MURIMNET_CHANGETOMURIMNET_SYN:				MurimNet_ChangeToMurimNet_Syn( dwConnectionIndex, pMsg, dwLength ) ;		break;
	case MP_MURIMNET_CONNECT_SYN:						MurimNet_Connect_Syn( dwConnectionIndex, pMsg) ;							break;
	case MP_MURIMNET_RECONNECT_SYN:						MurimNet_Reconnect_Syn( dwConnectionIndex, pMsg) ;							break;
/*		
	case MP_MURIMNET_MURIMSERVERIN_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			
			USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if(!pInfo)
				return;

			WORD MurimnetPort = g_pServerTable->GetServerPort(pmsg->dwData);
			SERVERINFO* pSInfo = g_pServerTable->FindServer((DWORD)MurimnetPort);

			// ��u����AACN AE�ˡ�?AI��?�ˢ� �ˡ�A�ˡ�A �ˡ�E��u���Ϩ�oo����I AE�ˡ�?AI ��u����AAA��i ��uE�ˡ��ϡ�
			MSG_DWORD msg;
			msg.Category = MP_MURIMNET;
			msg.Protocol = MP_MURIMNET_MURIMSERVERIN_SYN;
			msg.dwObjectID = pInfo->dwCharacterID;
			msg.dwData = pInfo->dwUniqueConnectIdx;
		
			pInfo->wUserMapNum = pmsg->dwData;
			pInfo->dwMapServerConnectionIndex = pSInfo->dwConnectionIndex;
				
			g_Network.Send2Server(pInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg));
		
		}
		break;
*/

	default:											TransToMapServerMsgParser(dwConnectionIndex,pMsg,dwLength);				break;		
	}
}

void MurimNet_ChangeToMurimNet_Syn( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	// YH2DO AO��oA CI��ia AU��iu
	WORD MurimnetServerNum = 99;
	WORD MurimnetPort = g_pServerTable->GetServerPort( eSK_MAP, MurimnetServerNum);
	if(MurimnetPort)
	{
		USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
		g_Network.Send2Server(pInfo->dwMapServerConnectionIndex,(char*)pMsg,dwLength);
		SERVERINFO* pSInfo = g_pServerTable->FindServer(MurimnetPort);
		pInfo->wUserMapNum = (WORD)MurimnetServerNum;		// ��oU�ϡ��ϡ� �ˡ�E ��o�Ϩ�E��I����I ��uA����A
		pInfo->dwMapServerConnectionIndex = pSInfo->dwConnectionIndex;	// ��oU�ϡ��ϡ� �ˡ�E dwConnectionIndex����I ��uA����A
	}
	else
	{
		// �ˡ�EAI ��u�Ϩ�A�ˡ��ˡ�e ����A����I
		MSGBASE msg;
		msg.Category = MP_MURIMNET;
		msg.Protocol = MP_MURIMNET_CHANGETOMURIMNET_NACK;
		msg.dwObjectID = pmsg->dwObjectID;
		g_Network.Send2User((MSGBASE*)&msg, sizeof(MSGBASE));
		return;
	}
}


void MurimNet_Connect_Syn( DWORD dwConnectionIndex, char* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			
	USERINFO* pInfo = g_pUserTable->FindUser( dwConnectionIndex );
	if(!pInfo)
		return;

	//��o��i�ˡ��ϡ��ϩ�YA�ˡ�����I A��E��uO ��uE�ˡ��ϡ�
	MSG_DWORD2 msg;
	msg.Category	= MP_MURIMNET;
	msg.Protocol	= MP_MURIMNET_CONNECT_SYN;
	msg.dwObjectID	= pInfo->dwCharacterID;
	msg.dwData1		= pInfo->dwUniqueConnectIdx;
	msg.dwData2		= pmsg->dwData;

	g_Network.Send2Server( pInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg) );
}


void MurimNet_Reconnect_Syn( DWORD dwConnectionIndex, char* pMsg)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
			
	USERINFO* pInfo = g_pUserTable->FindUser( dwConnectionIndex );
	if(!pInfo)
		return;

	//��o��i�ˡ��ϡ��ϩ�YA�ˡ�����I A��E��uO ��uE�ˡ��ϡ�
	MSGBASE msg;
	msg.Category	= MP_MURIMNET;
	msg.Protocol	= MP_MURIMNET_RECONNECT_SYN;
	msg.dwObjectID	= pInfo->dwCharacterID;

	g_Network.Send2Server( pInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg) );
}




void MP_DebugMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_DEBUG_CLIENTASSERT:
		{
			MSG_ASSERT* pmsg = (MSG_ASSERT*)pMsg;
			char temp[256];
			sprintf(temp,"%d\t%d\t%d\t%s",
				dwConnectionIndex,pmsg->UserID,pmsg->CharacterID,pmsg->strAssertMsg);

			WriteAssertMsg("CLIENT",0,temp);
		}
	}
}


void MP_CHEATUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;

	USERINFO* pUserInfo = g_pUserTable->FindUser( dwConnectionIndex );
	if( !pUserInfo ) return;
	if( !( pUserInfo->UserLevel == eUSERLEVEL_GM || pUserInfo->UserLevel == eUSERLEVEL_PROGRAMMER
		|| pUserInfo->UserLevel == eUSERLEVEL_DEVELOPER ) )
		return;

	switch( pTempMsg->Protocol )
	{
	case MP_CHEAT_GM_LOGIN_SYN:				Cheat_Gm_Login_Syn(dwConnectionIndex, pMsg) ;		break;//return;
	}

//////
	GM_INFO* pGMInfo = GMINFO->GetGMInfo( dwConnectionIndex );

	if( pUserInfo->UserLevel == eUSERLEVEL_GM )		//AI��C ����A������ Ao��iAU.
	if( pGMInfo == NULL )
		return;	
/////
	//-- �� �� �Լ��� ����� ���� ��������? �Ф� ������...

	//--- 1. GM�̻��̸� ������ �׼� ��� ������ ġƮ��  
	switch( pTempMsg->Protocol )
	{
	case MP_CHEAT_CHANGEMAP_SYN:			Cheat_ChangeMap_Syn(dwConnectionIndex, pMsg, dwLength) ;						return;		
	case MP_CHEAT_BANCHARACTER_SYN:			Cheat_BanCharacter_Syn(pMsg) ;													return;		
	case MP_CHEAT_BANMAP_SYN:				Cheat_BanMap_Syn(pMsg) ;														return;		
	case MP_CHEAT_WHEREIS_SYN:				Cheat_WhereIs_Syn(pMsg) ;														return;
	case MP_CHEAT_AGENTCHECK_SYN:			Cheat_AgentCheck_Syn(pMsg, dwLength) ;											return;
	case MP_CHEAT_NOTICE_SYN:				Cheat_Notice_Syn(pMsg, dwLength) ;												return;
	case MP_CHEAT_MOVE_SYN:
	case MP_CHEAT_MOVEUSER_SYN:
	case MP_CHEAT_GETID_SYN:
	case MP_CHEAT_HIDE_SYN:
	case MP_CHEAT_CHECKIP_SYN:
	case MP_CHEAT_WHEREIS_MAPSERVER_ACK:
	case MP_CHEAT_PARTYINFO_SYN:
//---KES CHEAT PKEVENT
	case MP_CHEAT_PKEVENT:
//-------------
//---KES CHEAT CHANGESIZE
	case MP_CHEAT_CHANGESIZE:
//---------------
	case MP_CHEAT_MOB_CHAT:					TransToMapServerMsgParser(dwConnectionIndex,pMsg,dwLength);						return;
	
	case MP_CHEAT_MOVETOCHAR_SYN:
		{
			pUserInfo->bForceMove = 1;
			TransToMapServerMsgParser(dwConnectionIndex,pMsg,dwLength);
			return;
		}
	}

	//--- 2. GM �����͸� ������ ġƮ��
/*
	if( g_pServerSystem->IsTestServer() == FALSE )
	{
		if( pGMInfo != NULL )
		if( pGMInfo->nPower != eGM_POWER_MASTER ) return;
	}
*/
	switch( pTempMsg->Protocol )
	{
	case MP_CHEAT_EVENT_MONSTER_REGEN:		Cheat_Event_Monster_Regen(pUserInfo, dwConnectionIndex, pMsg, dwLength) ;		return;
	case MP_CHEAT_EVENT_MONSTER_DELETE:		Cheat_Event_Monster_Delete(pUserInfo, dwConnectionIndex, pMsg, dwLength) ;		return;
	case MP_CHEAT_EVENT_SYN:				Cheat_Event_Syn(pUserInfo, dwConnectionIndex, pMsg, dwLength) ;					return;
	case MP_CHEAT_EVENTNOTIFY_ON:			Cheat_EventNotify_On(pUserInfo, dwConnectionIndex, pMsg, dwLength) ;			return;
	case MP_CHEAT_EVENTNOTIFY_OFF:			Cheat_EventNotify_Off(pUserInfo, dwConnectionIndex, pMsg, dwLength) ;			return;		
	case MP_CHEAT_PLUSTIME_ON:				Cheat_PlusTime_On(pUserInfo, dwConnectionIndex, pMsg, dwLength) ;				return;
	case MP_CHEAT_PLUSTIME_ALLOFF:			Cheat_PlusTime_AllOff(pUserInfo, dwConnectionIndex, pMsg, dwLength) ;			return;
	case MP_CHEAT_EVENT_START_SYN:			
	case MP_CHEAT_EVENT_READY_SYN:			Cheat_Event_Syn2(pUserInfo, dwConnectionIndex, pMsg, dwLength) ;				return;
	case MP_CHEAT_CHANGE_EVENTMAP_SYN:		Cheat_Change_EventMap_Syn(pUserInfo, dwConnectionIndex, pMsg, dwLength) ;		return;
	case MP_CHEAT_EVENTMAP_RESULT:			TransToMapServerMsgParser(dwConnectionIndex,pMsg,dwLength);						return;
	}


	if( g_pServerSystem->IsTestServer() == FALSE )	return;

	//--- 3. �������� �Ұ����� ġƮ��
	switch( pTempMsg->Protocol )
	{
	case MP_CHEAT_BLOCKCHARACTER_SYN:		Cheat_BlockCharacter_Syn(pUserInfo, dwConnectionIndex, pMsg) ;					return;
	case MP_CHEAT_PKALLOW_SYN:				Cheat_PkAllow_Syn(pUserInfo, dwConnectionIndex, pMsg, dwLength) ;				return;		
	case MP_CHEAT_ADDSKILL_SYN:
	case MP_CHEAT_SKILLLEVEL_SYN:
	case MP_CHEAT_ITEM_SYN:
	case MP_CHEAT_ITEM_OPTION_SYN:
	case MP_CHEAT_MONEY_SYN:				Cheat_Money_Syn(pUserInfo, dwConnectionIndex, pMsg, dwLength) ;					return;
	case MP_CHEAT_MUSSANG_READY:
	case MP_CHEAT_JACKPOT_GETPRIZE:			Cheat_Jackpot_GetPrize(pUserInfo, dwConnectionIndex, pMsg, dwLength) ;			return;
	case MP_CHEAT_JACKPOT_MONEYPERMONSTER:
	case MP_CHEAT_JACKPOT_ONOFF:			Cheat_Jackpot_MoneyPerMonster(pUserInfo, dwConnectionIndex, pMsg, dwLength) ;	return;
	case MP_CHEAT_JACKPOT_PROBABILITY:		Cheat_Jackpot_Probability(pUserInfo, dwConnectionIndex, pMsg, dwLength) ;		return;
	case MP_CHEAT_JACKPOT_CONTROL:			Cheat_Jackpot_Control(pUserInfo, dwConnectionIndex, pMsg, dwLength) ;			return;

	default:								TransToMapServerMsgParser(dwConnectionIndex,pMsg,dwLength);						return;
	}
}


void Cheat_Gm_Login_Syn(DWORD dwConnectionIndex, char* pMsg)
{
	MSG_NAME2* pmsg = (MSG_NAME2*)pMsg;
	WORD port;
	char ip[16];
	g_Network.GetUserAddress( dwConnectionIndex, ip, &port );

	GM_Login( dwConnectionIndex, pmsg->str1, pmsg->str2, ip );
}


void Cheat_ChangeMap_Syn(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength) 
{
	MSG_NAME_DWORD2* pmsg = (MSG_NAME_DWORD2*)pMsg;

	if( *(pmsg->Name) == 0 )
	{
		WORD tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, (WORD)pmsg->dwData1);	//map��u���Ϩ�oo��o�Ϩ�E��I��?I A�ˡ� ��u���Ϩ�oo��o�Ϩ�E��I�����ˢ� A�Ϣ���i��oCIAo�ˡ�A ��uE�ˡ�A�����ˢ�?
		if(tomapserverport)
		{				
			USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
			pInfo->bForceMove = TRUE;
			
			if( pInfo->dwMapServerConnectionIndex )
				g_Network.Send2Server(pInfo->dwMapServerConnectionIndex,(char*)pMsg,dwLength);
			
		}
		else
		{	
			MSG_DWORD msg;
			msg.Category	= MP_CHEAT;
			msg.Protocol	= MP_CHEAT_CHANGEMAP_NACK;
			msg.dwObjectID	= pmsg->dwObjectID;
			msg.dwData		= 0;
			g_Network.Send2User((MSGBASE*)&msg, sizeof(msg));
			return;
		}
	}
	else
	{
		g_Network.Broadcast2MapServer( (char*)pMsg, dwLength );	//AO��oA. DB��?�ˢ硧u���� ��i����i����u������?A�ˡ�A����O �ϩ��ˡͨϩ�����?
	}
}


void Cheat_BanCharacter_Syn(char* pMsg) 
{
	MSG_NAME* pmsg = (MSG_NAME*)pMsg;
	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pSenderInfo ) return;

	if( (FILTERTABLE->IsInvalidCharInclude((unsigned char*)pmsg->Name)) == TRUE )
	{
		return;
	}
	else
		GM_BanCharacter( pmsg->dwObjectID, pmsg->Name, pmsg->dwObjectID );
}


void Cheat_BlockCharacter_Syn(USERINFO* pUserInfo, DWORD dwConnectionIndex, char* pMsg) 
{
	MSG_NAME_DWORD* pmsg = (MSG_NAME_DWORD*)pMsg;
	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pSenderInfo ) return;

	if( pUserInfo->UserLevel == eUSERLEVEL_GM )	//AI��C ����A������ Ao��o����Ao..
	if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
		GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
		return;
	if( (FILTERTABLE->IsInvalidCharInclude((unsigned char*)pmsg->Name)) == TRUE )
		return;
	
	if( pmsg->dwData == 1 )
	{
		//block
		GM_UpdateUserLevel( pmsg->dwObjectID, g_nServerSetNum, pmsg->Name, 6 );	//6 : ����eA�ˢ�����i��������
	}
	else if( pmsg->dwData == 0 )
	{
		//nonblock
		GM_UpdateUserLevel( pmsg->dwObjectID, g_nServerSetNum, pmsg->Name, 1 );	//1 : C����A|
	}
}


void Cheat_WhereIs_Syn(char* pMsg) 
{
	MSG_NAME* pmsg = (MSG_NAME*)pMsg;
	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pSenderInfo ) return;

	if( (FILTERTABLE->IsInvalidCharInclude((unsigned char*)pmsg->Name)) == TRUE )
	{
		return;
	}
	else
		GM_WhereIsCharacter( pmsg->dwObjectID, pmsg->Name, pmsg->dwObjectID );
}


void Cheat_Event_Monster_Regen(USERINFO* pUserInfo, DWORD dwConnectionIndex, char* pMsg, DWORD dwLength) 
{
	MSG_EVENT_MONSTERREGEN* pmsg = (MSG_EVENT_MONSTERREGEN*)pMsg;
	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pSenderInfo ) return;

	if( pUserInfo->UserLevel == eUSERLEVEL_GM )	//AI��C ����A������ Ao��o����Ao..
	if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MASTER &&
		GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
		return;

	//---KES ���������� ġƮ����
	if( g_pServerSystem->IsTestServer() == FALSE )
		pmsg->ItemID = 0;
	//--------------------------

	WORD wServerPort = g_pServerTable->GetServerPort( eSK_MAP, pmsg->wMap );
	SERVERINFO* pInfo = g_pServerTable->FindServer( wServerPort );

	if(pInfo)
		g_Network.Send2Server( pInfo->dwConnectionIndex, pMsg, dwLength );
}


void Cheat_Event_Monster_Delete(USERINFO* pUserInfo,DWORD dwConnectionIndex, char* pMsg, DWORD dwLength) 
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pSenderInfo ) return;

	if( pUserInfo->UserLevel == eUSERLEVEL_GM )	//AI��C ����A������ Ao��o����Ao..
	if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MASTER &&
		GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
		return;

	g_Network.Broadcast2MapServer( pMsg, dwLength );
}


void Cheat_BanMap_Syn(char* pMsg) 
{
	MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pSenderInfo ) return;

	USERINFO* pInfo;
	g_pUserTable->SetPositionUserHead();
	while( pInfo = g_pUserTable->GetUserData() )
	{
		if( pInfo->wUserMapNum == pmsg->wData1 )
		{
			if( pmsg->wData2 != 1 || pInfo->dwCharacterID != pSenderInfo->dwCharacterID )
			{
				DWORD dwConIdx = pInfo->dwConnectionIndex;
				OnDisconnectUser( dwConIdx );
				DisconnectUser( dwConIdx );	//3:GMTOOL_BAN
			}
		}
	}
	
	MSG_WORD msg;
	msg.Category	= MP_CHEAT;
	msg.Protocol	= MP_CHEAT_BANMAP_SYN;
	msg.wData		= pmsg->wData1;

	g_Network.Broadcast2AgentServerExceptSelf( (char*)&msg, sizeof(msg) );
}


void Cheat_AgentCheck_Syn(char* pMsg, DWORD dwLength) 
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
			
	g_Network.Broadcast2AgentServerExceptSelf( pMsg, dwLength );

	pmsg->Protocol = MP_CHEAT_AGENTCHECK_ACK;
	
	USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( pInfo )
		g_Network.Send2User( pInfo->dwConnectionIndex, (char*)pmsg, dwLength );	
}


void Cheat_PkAllow_Syn(USERINFO* pUserInfo,DWORD dwConnectionIndex, char* pMsg, DWORD dwLength) 
{
	MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pSenderInfo ) return;

	if( pUserInfo->UserLevel == eUSERLEVEL_GM )	//AI��C ����A������ Ao��o����Ao..
	if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
		GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
		return;

	WORD wServerPort = g_pServerTable->GetServerPort( eSK_MAP, pmsg->wData1 );
	SERVERINFO* pInfo = g_pServerTable->FindServer( wServerPort );
	if( pInfo )
		g_Network.Send2Server( pInfo->dwConnectionIndex, pMsg, dwLength );	
}


void Cheat_Notice_Syn(char* pMsg, DWORD dwLength) 
{
	MSG_CHAT_WORD* pmsg = (MSG_CHAT_WORD*)pMsg;
	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pSenderInfo ) return;

	TESTMSG msg;
	msg.Category = MP_SIGNAL;
	msg.Protocol = MP_SIGNAL_COMMONUSER;
	SafeStrCpy( msg.Msg, pmsg->Msg, MAX_CHAT_LENGTH + 1 );

	USERINFO* pInfo = NULL;
	g_pUserTable->SetPositionUserHead();
	while( pInfo = (USERINFO *)g_pUserTable->GetUserData() )
	{	
		if( pmsg->wData == 0 || pmsg->wData == pInfo->wUserMapNum )
		{
			TESTMSG msgTemp = msg;
			g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msgTemp, msgTemp.GetMsgLength() );	//CHATMSG 040324
		}
	}

	g_Network.Broadcast2AgentServerExceptSelf( (char*)pmsg, dwLength );
}


void Cheat_Money_Syn(USERINFO* pUserInfo,DWORD dwConnectionIndex, char* pMsg, DWORD dwLength) 
{
	if( pUserInfo->UserLevel == eUSERLEVEL_GM )	//AI��C ����A������ Ao��o����Ao..
	if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MASTER )
		return;

	TransToMapServerMsgParser(dwConnectionIndex,pMsg,dwLength);		
}


void Cheat_Event_Syn(USERINFO* pUserInfo,DWORD dwConnectionIndex, char* pMsg, DWORD dwLength) 
{
	MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pSenderInfo ) return;
	
	if( pUserInfo->UserLevel == eUSERLEVEL_GM )
	if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MASTER &&
		GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
		return;			
/*		
	if( pmsg->wData2 == 1 )
	{
		GMINFO->SetEventCheat( TRUE );
	}
	else
	{
		GMINFO->SetEventCheat( FALSE );
	}
	g_Network.Broadcast2AgentServerExceptSelf( (char*)pmsg, dwLength );
*/
	g_Network.Broadcast2MapServer( (char*)pmsg, dwLength );

	// 06.09.12 RaMa
	LogGMToolUse( pmsg->dwObjectID, eGMLog_Event, MP_CHEAT_EVENT_SYN, (DWORD)pmsg->wData1, (DWORD)pmsg->wData2 );
}


void Cheat_EventNotify_On(USERINFO* pUserInfo,DWORD dwConnectionIndex, char* pMsg, DWORD dwLength) 
{
	MSG_EVENTNOTIFY_ON* pmsg = (MSG_EVENTNOTIFY_ON*)pMsg;
	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pSenderInfo ) return;

	if( pUserInfo->UserLevel == eUSERLEVEL_GM )
	if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
		GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
		return;
	
	g_pServerSystem->ResetApplyEvent();
	for( int i=0; i<eEvent_Max; ++i )
	{
		if( pmsg->EventList[i] )
		{
			g_pServerSystem->SetApplyEvent( i ) ;
		}
	}

	g_pServerSystem->SetEventNotifyStr( pmsg->strTitle, pmsg->strContext );
	g_pServerSystem->SetUseEventNotify( TRUE );	//seteventnotifystr ���Ŀ� ȣ���ؾ��Ѵ�.
	
	g_Network.Broadcast2AgentServerExceptSelf( (char*)pmsg, dwLength );
}


void Cheat_PlusTime_On(USERINFO* pUserInfo,DWORD dwConnectionIndex, char* pMsg, DWORD dwLength) 
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pSenderInfo ) return;

	if( pUserInfo->UserLevel == eUSERLEVEL_GM )
	if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
		GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
		return;

	PLUSTIMEMGR->PlusTimeOn();
	g_Network.Broadcast2AgentServerExceptSelf( (char*)pmsg, dwLength );

	//// 06.09.12 RaMa
	//LogGMToolUse( pmsg->dwObjectID, eGMLog_PlusTime, MP_CHEAT_PLUSTIME_ON, 0, 0 );
}


void Cheat_EventNotify_Off(USERINFO* pUserInfo,DWORD dwConnectionIndex, char* pMsg, DWORD dwLength) 
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pSenderInfo ) return;

	if( pUserInfo->UserLevel == eUSERLEVEL_GM )
	if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
		GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
		return;
	
	g_pServerSystem->SetUseEventNotify( FALSE );
	g_Network.Broadcast2AgentServerExceptSelf( (char*)pmsg, dwLength );

	// 06.09.12 RaMa
	LogGMToolUse( pmsg->dwObjectID, eGMLog_PlusTime, MP_CHEAT_EVENTNOTIFY_OFF, 0, 0 );
}

	
void Cheat_PlusTime_AllOff(USERINFO* pUserInfo,DWORD dwConnectionIndex, char* pMsg, DWORD dwLength) 
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pSenderInfo ) return;

	if( pUserInfo->UserLevel == eUSERLEVEL_GM )
	if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
		GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
		return;
	
	PLUSTIMEMGR->PlusTimeOff();
	g_Network.Broadcast2AgentServerExceptSelf( (char*)pmsg, dwLength );

	//// 06.09.12 RaMa
	//LogGMToolUse( pmsg->dwObjectID, eGMLog_PlusTime, MP_CHEAT_PLUSTIME_ALLOFF, 0, 0 );
}


void Cheat_Change_EventMap_Syn(USERINFO* pUserInfo,DWORD dwConnectionIndex, char* pMsg, DWORD dwLength) 
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pSenderInfo ) return;

	if( pUserInfo->UserLevel == eUSERLEVEL_GM )
	if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
		GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
		return;

	g_Network.Broadcast2MapServer( pMsg, dwLength );
}


void Cheat_Event_Syn2(USERINFO* pUserInfo,DWORD dwConnectionIndex, char* pMsg, DWORD dwLength) 
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	if( pUserInfo->UserLevel == eUSERLEVEL_GM )
	if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
		GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
		return;

	WORD wServerPort = g_pServerTable->GetServerPort( eSK_MAP, (WORD)pmsg->dwData1 );
	SERVERINFO* pInfo = g_pServerTable->FindServer( wServerPort );
	if( pInfo )
		g_Network.Send2Server( pInfo->dwConnectionIndex, pMsg, dwLength );	
}


void Cheat_Jackpot_GetPrize(USERINFO* pUserInfo,DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	if( pUserInfo->UserLevel == eUSERLEVEL_GM )
		if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
			GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
			return;

	g_Network.Send2Server(pUserInfo->dwMapServerConnectionIndex,pMsg,dwLength);
}


void Cheat_Jackpot_MoneyPerMonster(USERINFO* pUserInfo,DWORD dwConnectionIndex, char* pMsg, DWORD dwLength) 
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pSenderInfo ) return;
	
	if( pUserInfo->UserLevel == eUSERLEVEL_GM )
	if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
		GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
		return;

	g_Network.Broadcast2MapServer( pMsg, dwLength );
}


void Cheat_Jackpot_Probability(USERINFO* pUserInfo,DWORD dwConnectionIndex, char* pMsg, DWORD dwLength) 
{
	MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pSenderInfo ) return;
	
	if( pUserInfo->UserLevel == eUSERLEVEL_GM )
	{
		if(GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
			GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
			return;
	}
		g_Network.Broadcast2MapServer( pMsg, dwLength );
}


void Cheat_Jackpot_Control(USERINFO* pUserInfo,DWORD dwConnectionIndex, char* pMsg, DWORD dwLength) 
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;	// �ʿ䰡 �ִ°�?
	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pSenderInfo ) return;

	if( pUserInfo->UserLevel == eUSERLEVEL_GM )
	if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
		GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
		return;

	// 071115 ����, ���� ��� ����
	//if( pmsg->dwData1 == 0 )	// Fr_Client 0 == TIMELENGTH
	//{
	//	if( pmsg->dwData2 < 0 || pmsg->dwData2 > 3600000 ) return;	// sec ����

	//	JACKPOTMGR->SetUpdateTimeLength( pmsg->dwData2 );

	//	g_Network.Broadcast2MapServer( pMsg, dwLength );
	//	
	//	return;
	//}

//			g_Network.Broadcast2MapServer( pMsg, dwLength );
}



void MP_CHEATServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch( pTempMsg->Protocol )
	{
	case MP_CHEAT_BANCHARACTER_SYN:						CheatServer_BanCharacter_Syn(dwConnectionIndex, pMsg) ;			break;
	case MP_CHEAT_BANCHARACTER_ACK:						CheatServer_BanCharacter_Ack(pMsg) ;							break;
/*
	case MP_CHEAT_WHEREIS_SYN:	//<--Agent
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			USERINFO* pTargetInfo = g_pUserTableForObjectID->FindUser( pmsg->dwData );
			if( pTargetInfo )
			{
				MSG_WORD msgAck;
				msgAck.Category	= MP_CHEAT;
				msgAck.Protocol	= MP_CHEAT_WHEREIS_ACK;
				msgAck.dwObjectID	= pmsg->dwObjectID;	//senderid
				msgAck.wData		= pTargetInfo->wUserMapNum;

				g_Network.Broadcast2AgentServerExceptSelf( (char*)&msgAck, sizeof(MSG_WORD) );
			}
			else
			{
				MSGBASE msgNack;
				msgNack.Category	= MP_CHEAT;
				msgNack.Protocol	= MP_CHEAT_WHEREIS_NACK;

				g_Network.Broadcast2AgentServerExceptSelf( (char*)&msgNack, sizeof(MSGBASE) );
			}
		}
		break;

	case MP_CHEAT_WHEREIS_ACK:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;

			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;
			
			g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_WORD) );
		}
		break;
*/
	case MP_CHEAT_WHEREIS_MAPSERVER_ACK:				CheatServer_WhereIs_MapServer_Ack(pMsg, dwLength) ;				break;
	case MP_CHEAT_WHEREIS_NACK:							CheatServer_WhereIs_Nack(pMsg, dwLength) ;						break;
	case MP_CHEAT_BANMAP_SYN:							CheatServer_BanMap_Syn(pMsg) ;									break;
	case MP_CHEAT_AGENTCHECK_SYN:						CheatServer_AgentCheck_Syn(dwConnectionIndex, pMsg, dwLength) ;	break;
	case MP_CHEAT_AGENTCHECK_ACK:						CheatServer_AgentCheck_Ack(pMsg, dwLength) ;					break;
	case MP_CHEAT_NOTICE_SYN:							CheatServer_Notice_Syn(pMsg) ;									break;
	case MP_CHEAT_EVENT_SYN:							CheatServer_Event_Syn(pMsg) ;									break;
	case MP_CHEAT_EVENTMAP_RESULT:						CheatServer_EventMap_Result(pMsg) ;								break;
	case MP_CHEAT_EVENTNOTIFY_ON:						CheatServer_EventNotify_On(pMsg) ;								break;
	case MP_CHEAT_EVENTNOTIFY_OFF:						CheatServer_EventNotify_Off(pMsg) ;								break;
	case MP_CHEAT_PLUSTIME_ON:							CheatServer_PlusTime_On(pMsg) ;									break;
	case MP_CHEAT_PLUSTIME_ALLOFF:						CheatServer_PlusTime_AllOff(pMsg) ;								break;
	default:											TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );	break;
	}
}


void CheatServer_BanCharacter_Syn(DWORD dwConnectionIndex, char* pMsg)
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	USERINFO* pTargetInfo = g_pUserTableForObjectID->FindUser( pmsg->dwData1 );
	if( !pTargetInfo ) return;

	MSG_DWORD msg;
	msg.Category	= MP_CHEAT;
	msg.dwData		= pmsg->dwData2;	//�����ˡ��ϩ���o��ic��O��A ��u����AI��i�ϡ�

	DWORD dwConIdx = pTargetInfo->dwConnectionIndex;
	OnDisconnectUser( dwConIdx );
	DisconnectUser( dwConIdx );
	msg.Protocol	= MP_CHEAT_BANCHARACTER_ACK;

	g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );
}


void CheatServer_BanCharacter_Ack(char* pMsg) 
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwData );
	if( !pUserInfo ) return ;

	MSGBASE msg;
	msg.Category	= MP_CHEAT;
	msg.Protocol	= MP_CHEAT_BANCHARACTER_ACK;
	msg.dwObjectID	= pmsg->dwData;
	g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof( msg ) );
}


void CheatServer_WhereIs_MapServer_Ack(char* pMsg, DWORD dwLength)
{
	MSG_WHERE_INFO* pmsg = (MSG_WHERE_INFO*)pMsg;
	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pSenderInfo ) return;

	g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)pmsg, dwLength );
}


void CheatServer_WhereIs_Nack(char* pMsg, DWORD dwLength)
{
	MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;

	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pSenderInfo ) return;

	g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)pmsg, dwLength );
}


void CheatServer_BanMap_Syn(char* pMsg) 
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;

	USERINFO* pInfo;
	g_pUserTable->SetPositionUserHead();
	while( pInfo = g_pUserTable->GetUserData() )
	{
		if( pInfo->wUserMapNum == pmsg->wData )
		{
			DWORD dwConIdx = pInfo->dwConnectionIndex;
			OnDisconnectUser( dwConIdx );
			DisconnectUser( dwConIdx );	//3:GMTOOL_BAN
		}
	}
}


void CheatServer_AgentCheck_Syn(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength) 
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	pmsg->Protocol = MP_CHEAT_AGENTCHECK_ACK;

	g_Network.Send2Server( dwConnectionIndex, (char*)pmsg, dwLength );
}


void CheatServer_AgentCheck_Ack(char* pMsg, DWORD dwLength) 
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( pInfo )
		g_Network.Send2User( pInfo->dwConnectionIndex, (char*)pmsg, dwLength );
}


void CheatServer_Notice_Syn(char* pMsg) 
{
	MSG_CHAT_WORD* pmsg = (MSG_CHAT_WORD*)pMsg;

	TESTMSG msg;
	msg.Category = MP_SIGNAL;
	msg.Protocol = MP_SIGNAL_COMMONUSER;
	SafeStrCpy( msg.Msg, pmsg->Msg, MAX_CHAT_LENGTH + 1 );

	USERINFO* pInfo = NULL;
	g_pUserTable->SetPositionUserHead();
	while( pInfo = (USERINFO *)g_pUserTable->GetUserData() )
	{	
		if( pmsg->wData == 0 || pmsg->wData == pInfo->wUserMapNum )
		{
			TESTMSG msgTemp = msg;
			g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msgTemp, msgTemp.GetMsgLength() );	//CHATMSG 040324
		}
	}
}


void CheatServer_Event_Syn(char* pMsg) 
{
	MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			
	if( pmsg->wData2 == 1 )
	{
		GMINFO->SetEventCheat( TRUE );
	}
	else
	{
		GMINFO->SetEventCheat( FALSE );
	}
}


void CheatServer_EventMap_Result(char* pMsg) 
{
	MSG_NAME_WORD* pmsg = (MSG_NAME_WORD*)pMsg;
	USERINFO* pInfo = NULL;
	g_pUserTable->SetPositionUserHead();
	while( pInfo = g_pUserTable->GetUserData() )
	{
		if( pInfo->UserLevel == eUSERLEVEL_GM )
		{
			MSG_NAME_WORD msgTemp = *pmsg;
			g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msgTemp, sizeof(msgTemp) );
		}
	}
}


void CheatServer_EventNotify_On(char* pMsg) 
{
	MSG_EVENTNOTIFY_ON* pmsg = (MSG_EVENTNOTIFY_ON*)pMsg;

	g_pServerSystem->SetEventNotifyStr( pmsg->strTitle, pmsg->strContext );
	g_pServerSystem->SetUseEventNotify( TRUE );	//seteventnotifystr���Ŀ� ȣ���Ͻÿ�
}


void CheatServer_EventNotify_Off(char* pMsg) 
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	g_pServerSystem->SetUseEventNotify( FALSE );
}


void CheatServer_PlusTime_On(char* pMsg) 
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;

	PLUSTIMEMGR->PlusTimeOn();
}


void CheatServer_PlusTime_AllOff(char* pMsg) 
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
			
	PLUSTIMEMGR->PlusTimeOff();
}



void MP_HACKCHECKMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;

	switch(pTempMsg->Protocol)
	{

	case MP_HACKCHECK_SPEEDHACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pInfo ) break;
			
			DWORD curTime = MHTIMEMGR_OBJ->GetNewCalcCurTime();
			if( curTime - pmsg->dwData < SPEEDHACK_CHECKTIME - 3000 )	//1000->2000
			{
				MSGBASE msg;
				msg.Category = MP_HACKCHECK;
				msg.Protocol = MP_HACKCHECK_BAN_USER;
				g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
				
				MSG_DWORD msgMap;
				msgMap.Category		= MP_HACKCHECK;
				msgMap.Protocol		= MP_HACKCHECK_BAN_USER;
				msgMap.dwObjectID	= pmsg->dwObjectID;
				msgMap.dwData		= curTime - pmsg->dwData;	//
				
				g_Network.Send2Server( pInfo->dwMapServerConnectionIndex, (char*)&msgMap, sizeof(msgMap) );

				DWORD dwConIdx = pInfo->dwConnectionIndex;
				OnDisconnectUser( dwConIdx );
				DisconnectUser( dwConIdx );
			}
		}
		break;
	case MP_HACKCHECK_BAN_USER_TOAGENT:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pInfo ) break;
			
			MSGBASE msg;
			msg.Category = MP_HACKCHECK;
			msg.Protocol = MP_HACKCHECK_BAN_USER;
			g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
			
			MSG_DWORD msgMap;
			msgMap.Category	= MP_HACKCHECK;
			msgMap.Protocol = MP_HACKCHECK_BAN_USER;
			msgMap.dwObjectID = pmsg->dwObjectID;
			msgMap.dwData = 0;
				
			g_Network.Send2Server( pInfo->dwMapServerConnectionIndex, (char*)&msgMap, sizeof(msgMap) );

			DWORD dwConIdx = pInfo->dwConnectionIndex;
			OnDisconnectUser( dwConIdx );
			DisconnectUser( dwConIdx );
		}
		break;	
	}
}

void MP_GUILDServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	/*
	��ε�ĳ���� ����

	- ������Ʈ�� ���� ��ε�ĳ����: Ư�� ȸ������ �޽����� ������ ������
	- �ʿ� ���� ��ε�ĳ����: ��ü �ʿ� �޽����� ������ ������
	*/
	MSGBASE*				pTempMsg = ( MSGBASE* )pMsg;
	const MP_PROTOCOL_GUILD protocol = ( MP_PROTOCOL_GUILD )pTempMsg->Protocol;

	switch( protocol )
	{
	case MP_GUILD_CREATE_NOTIFY_TO_MAP:
	case MP_GUILD_ADDMEMBER_NOTIFY_TO_MAP:
	case MP_GUILD_DELETEMEMBER_NOTIFY_TO_MAP:
	case MP_GUILD_BREAKUP_NOTIFY_TO_MAP:
	case MP_GUILD_SECEDE_NOTIFY_TO_MAP:
	case MP_GUILDMARK_NOTIFY_TO_MAP:
	case MP_GUILD_LEVELUP_NOTIFY_TO_MAP:
	case MP_GUILD_MEMBERLEVEL_NOTIFY_TO_MAP:
	case MP_GUILD_CHANGERANK_NOTIFY_TO_MAP:
	case MP_GUILD_LOGINFO_NOTIFY_TO_MAP:
	case MP_GUILD_CHANGE_LOCATION_NOTIFY_TO_MAP:
	case MP_GUILD_NOTICE_NOTIFY:
	case MP_GUILD_CHANGE_MASTER_TO_AGENT:
	case MP_GUILD_SET_MEMBER_TO_MAP:
	// 080417 LUJ, ��� ��ų ȹ�� ����
	case MP_GUILD_ADD_SKILL_ACK_TO_MAP:
	// 080602 LUJ, ��� ���� ���ΰ�ħ�� ��û�Ѵ�
	case MP_GUILD_SCORE_UPDATE_TO_MAP:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);

			break;
		}
	case MP_GUILD_GIVENICKNAME_NOTIFY_TO_MAP:
		{
			SEND_GUILD_NICKNAME * pmsg = (SEND_GUILD_NICKNAME*)pMsg;
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->TargetIdx );
			if(pInfo == 0)
				return;
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);

			break;
		}
	//case MP_GUILD_LOGIN_MEMBER_TO_AGENT:
	//	{
	//		pTempMsg->Protocol	= MP_GUILD_LOGIN_MEMBER_TO_USER;

	//		g_Network.Broadcast2AgentServer( ( char* )pTempMsg, dwLength );

	//		// ����: break�� ���� �� �ǵ����� ����. ��ε�ĳ��Ʈ�� �ڽ��� ������ �������� ������ ������
	//	}
	//case MP_GUILD_LOGIN_MEMBER_TO_USER:
	//	{
	//		const USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pTempMsg->dwObjectID );

	//		if( pInfo )
	//		{
	//			g_Network.Send2User( pTempMsg, dwLength );
	//		}

	//		break;
	//	}
	//case MP_GUILD_MEMBER_SEARCH_TO_AGENT:
	//	{
	//		pTempMsg->Protocol	= MP_GUILD_MEMBER_SEARCH_TO_MAP;

	//		g_Network.Broadcast2MapServer( pMsg, dwLength );
	//		break;
	//	}
	//case MP_GUILD_LOGOUT_MEMBER_TO_AGENT:
	//	{
	//		pTempMsg->Protocol	= MP_GUILD_LOGOUT_MEMBER_TO_MAP;

	//		g_Network.Broadcast2MapServer( pMsg, dwLength );
	//		break;
	//	}
	//case MP_GUILD_SCORE_UPDATE_TO_MAP:
	//	{
	//		g_Network.Broadcast2MapServer( ( char* )pTempMsg, dwLength );
	//		break;
	//	}
	default:
		{
			TransToClientMsgParser(dwConnectionIndex, pMsg, dwLength);
			break;
		}
	}
}

void MP_GUILDUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE*				pTempMsg = ( MSGBASE* )pMsg;
	const MP_PROTOCOL_GUILD protocol = ( MP_PROTOCOL_GUILD )pTempMsg->Protocol;

	switch( protocol )
	{
	case MP_GUILD_CREATE_SYN:
		{
			SEND_GUILD_CREATE_SYN * pmsg = (SEND_GUILD_CREATE_SYN*)pMsg;
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pInfo ) break;

			if( ( FILTERTABLE->IsInvalidCharInclude((unsigned char *)pmsg->GuildName) == TRUE ) || 
				( FILTERTABLE->IsUsableName(pmsg->GuildName) == FALSE ) )
			{
				MSG_BYTE msg;
				msg.Category = MP_GUILD;
				msg.Protocol = MP_GUILD_CREATE_NACK;
				msg.bData = eGuildErr_Create_Invalid_Name;
				g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
			}
			else
			{
				TransToMapServerMsgParser(dwConnectionIndex, pMsg, dwLength);
			}

			break;
		}
	case MP_GUILD_GIVENICKNAME_SYN:
		{
			SEND_GUILD_NICKNAME * pmsg = (SEND_GUILD_NICKNAME *)pMsg;
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pInfo ) break;

			if(( FILTERTABLE->IsUsableName(pmsg->NickName, FALSE ) == FALSE ) || (FILTERTABLE->IsCharInString(pmsg->NickName, " '") == TRUE) )
			{
				MSG_BYTE msg;
				msg.Category = MP_GUILD;
				msg.Protocol = MP_GUILD_GIVENICKNAME_NACK;
				msg.bData = eGuildErr_Nick_Filter;
				g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
			}
			else
			{
				TransToMapServerMsgParser(dwConnectionIndex, pMsg, dwLength);
			}

			break;
		}		
	case MP_GUILD_RETIRE_SYN:
		{
			MSG_DWORD*		message		= ( MSG_DWORD* )pTempMsg;
			const DWORD		playerIndex	= message->dwData;
			const USERINFO* pInfo		= g_pUserTableForObjectID->FindUser( playerIndex );

			message->dwObjectID	= playerIndex;

			if( pInfo )
			{
				g_Network.Send2User( message, dwLength );
			}
			else
			{
				g_Network.Broadcast2AgentServerExceptSelf( pMsg, dwLength );
			}

			break;
		}
	case MP_GUILD_RETIRE_NACK:
		{
			const USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pTempMsg->dwObjectID );

			if( pInfo )
			{
				g_Network.Send2User( pTempMsg, dwLength );
			}
			else
			{
				g_Network.Broadcast2AgentServerExceptSelf( pMsg, dwLength );
			}

			break;
		}
	default:
		{
			TransToMapServerMsgParser(dwConnectionIndex, pMsg, dwLength);
			break;
		}
	}
}

// 070801 ����, ���� �йи� �����ϸ� �ּ�ó��

//void MP_FAMILYServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
//{
//	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
//	switch(pTempMsg->Protocol)
//	{
//	case MP_FAMILY_CREATE_NOTIFY_TO_MAP:
//	case MP_FAMILY_ADDMEMBER_NOTIFY_TO_MAP:
//	case MP_FAMILY_DELETEMEMBER_NOTIFY_TO_MAP:
//	case MP_FAMILY_BREAKUP_NOTIFY_TO_MAP:
//	case MP_FAMILY_SECEDE_NOTIFY_TO_MAP:
//	case MP_FAMILYMARK_NOTIFY_TO_MAP:
//	case MP_FAMILY_LEVELUP_NOTIFY_TO_MAP:
//	case MP_FAMILY_MEMBERLEVEL_NOTIFY_TO_MAP:
//	case MP_FAMILY_CHANGERANK_NOTIFY_TO_MAP:
//	case MP_FAMILY_LOGINFO_NOTIFY_TO_MAP:
//	case MP_FAMILY_CHANGE_LOCATION_NOTIFY_TO_MAP:
//	case MP_FAMILY_NOTICE_NOTIFY:
//	case MP_FAMILY_HUNTEDMONSTER_TOTALCOUNT_TO_MAP:
//	case MP_FAMILY_PLUSTIME_SET_TO_MAP:
//	case MP_FAMILY_PLUSTIME_END_TO_MAP:
//	case MP_FAMILY_POINT_ADD_TO_MAP:
//	case MP_FAMILY_POINT_USEINFO_TO_MAP:
//	case MP_FAMILY_POINT_TO_MAP:
//		{
//			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
//		}		
//		break;
//	case MP_FAMILY_GIVENICKNAME_NOTIFY_TO_MAP:
//		{
//			SEND_FAMILY_NICKNAME * pmsg = (SEND_FAMILY_NICKNAME*)pMsg;
//			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->TargetIdx );
//			if(pInfo == 0)
//				return;
//			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
//		}
//		break;
//	default:
//		TransToClientMsgParser(dwConnectionIndex, pMsg, dwLength);
//	}
//}


// 070801 ����, ���� �йи� �����ϸ� �ּ�ó��

//void MP_FAMILYUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
//{
//	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
//	switch(pTempMsg->Protocol)
//	{
//	case MP_FAMILY_CREATE_SYN:
//		{
//			SEND_FAMILY_CREATE_SYN * pmsg = (SEND_FAMILY_CREATE_SYN*)pMsg;
//			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
//			if( !pInfo ) break;
//
//			if( ( FILTERTABLE->IsInvalidCharInclude((unsigned char *)pmsg->FamilyName) == TRUE ) || 
//				( FILTERTABLE->IsUsableName(pmsg->FamilyName) == FALSE ) )
//			{
//				MSG_BYTE msg;
//				msg.Category = MP_FAMILY;
//				msg.Protocol = MP_FAMILY_CREATE_NACK;
//				msg.bData = eFamilyErr_Create_Name;
//				g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
//			}
//			else
//				TransToMapServerMsgParser(dwConnectionIndex, pMsg, dwLength);
//		}
//		break;
//	case MP_FAMILY_GIVENICKNAME_SYN:
//		{
//			SEND_FAMILY_NICKNAME * pmsg = (SEND_FAMILY_NICKNAME *)pMsg;
//			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
//			if( !pInfo ) break;
//
//			if(( FILTERTABLE->IsUsableName(pmsg->NickName, FALSE ) == FALSE ) || (FILTERTABLE->IsCharInString(pmsg->NickName, " '") == TRUE) )
//			{
//				MSG_BYTE msg;
//				msg.Category = MP_FAMILY;
//				msg.Protocol = MP_FAMILY_GIVENICKNAME_NACK;
//				msg.bData = eFamilyErr_Nick_Filter;
//				g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
//			}
//			else
//				TransToMapServerMsgParser(dwConnectionIndex, pMsg, dwLength);
//		}
//		break;
//	default:
//		TransToMapServerMsgParser(dwConnectionIndex, pMsg, dwLength);
//	}
//}

void MP_GUILD_WARServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch( pTempMsg->Protocol )
	{
	case MP_GUILD_WAR_DECLARE_DENY_NOTIFY_TOMAP:
	case MP_GUILD_WAR_START_NOTIFY_TOMAP:
	case MP_GUILD_WAR_END_NOTIFY_TOMAP:
	case MP_GUILD_WAR_SUGGESTEND_NOTIFY_TOMAP:
	case MP_GUILD_WAR_SUGGESTEND_ACCEPT_NOTIFY_TOMAP:
	case MP_GUILD_WAR_SUGGESTEND_DENY_NOTIFY_TOMAP:
	case MP_GUILD_WAR_SURRENDER_NOTIFY_TOMAP:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
		}		
		break;		
	case MP_GUILD_WAR_DECLARE_NACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwData1 );
			if( pInfo )
			{
				g_Network.Send2User( pInfo->dwConnectionIndex, pMsg, sizeof(MSG_DWORD2) );
			}
		}
		break;
	case MP_GUILD_WAR_ADDMONEY_TOMAP:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			AddGuildFieldWarMoney( dwConnectionIndex, pmsg->dwData1, pmsg->dwData2 );
		}
		break;
	case MP_GUILD_WAR_RESULT_TOALLUSER:
		{
			MSG_NAME2* pmsg = (MSG_NAME2*)pMsg;
			MSG_NAME2 Msg;

			USERINFO* pInfo = NULL;
			g_pUserTable->SetPositionUserHead();
			while( pInfo = (USERINFO*)g_pUserTable->GetUserData() )
			{	
				Msg = *pmsg;
				g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&Msg, dwLength );
			}
		}
		break;
	default:
		TransToClientMsgParser(dwConnectionIndex, pMsg, dwLength);
	}
}

void MP_GUILD_WARUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch( pTempMsg->Protocol )
	{
	case MP_GUILD_WAR_DECLARE:
	case MP_GUILD_WAR_SUGGESTEND:
		{
			MSG_NAME_DWORD* pmsg = (MSG_NAME_DWORD*)pMsg;
			CheckGuildMasterLogin( dwConnectionIndex, pmsg->dwObjectID, pmsg->Name, pmsg->dwData, pTempMsg->Protocol );			
		}
		break;
	case MP_GUILD_WAR_DECLARE_ACCEPT:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CheckGuildFieldWarMoney( dwConnectionIndex, pmsg->dwObjectID, pmsg->dwData1, pmsg->dwData2 );
		}
		break;
	default:
		{
			TransToMapServerMsgParser(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	}
}

BOOL CheckCharacterMakeInfo( CHARACTERMAKEINFO* pmsg )
{	
// �ӽ÷� ����......
//------------------------------------------------------------------------------------------------------------------------------	
	int nHairType = 0 ;
	int nFaceType = 0 ;

	switch(pmsg->RaceType)
	{
	case 0 :
		{
			switch(pmsg->SexType)
			{
			case 0 :
				{
					nHairType = 9 ;
					nFaceType = 9 ;
				}
				break ;

			case 1 :
				{
					nHairType = 9 ;
					nFaceType = 9 ;
				}
				break ;
			}
		}
		break ;

	case 1 :
		{
			switch(pmsg->SexType)
			{
			case 0 :
				{
					nHairType = 9 ;
					nFaceType = 9 ;
				}
				break ;

			case 1 :
				{
					nHairType = 9 ;
					nFaceType = 9 ;
				}
				break ;
			}
		}
		break ;
	}

	if( pmsg->SexType > 1 )
		return FALSE;
	if( pmsg->HairType > nHairType )
		return FALSE;
	if( pmsg->FaceType > nFaceType )
		return FALSE;
//	if( pmsg->StartArea != 17 )
//		return FALSE;
	return TRUE;
}

void MP_ITEMUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_ITEM_SHOPITEM_CHANGEMAP_SYN:
		{

		}
		break;
	case MP_ITEM_SHOPITEM_NCHANGE_SYN:
		{
			SEND_CHANGENAMEBASE* pmsg = (SEND_CHANGENAMEBASE*)pMsg;
			USERINFO* pUser = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pUser )		return;

			MSG_DWORD msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_SHOPITEM_NCHANGE_NACK;

			DWORD len = strlen( pmsg->Name );
			if( len < 4 || len > MAX_NAME_LENGTH )
			{
				msg.dwData = 6;
				g_Network.Send2User( pUser->dwConnectionIndex, (char*)&msg, sizeof(msg) );
				return;
			}

			if( FILTERTABLE->IsUsableName( pmsg->Name ) &&
				!FILTERTABLE->IsInvalidCharInclude( (unsigned char*)pmsg->Name ) )
			{
				TransToMapServerMsgParser(dwConnectionIndex, pMsg, dwLength);
			}
			else
			{
				msg.dwData = 6;
				g_Network.Send2User( pUser->dwConnectionIndex, (char*)&msg, sizeof(msg) );
				return;
			}
		}
		break;
	case MP_ITEM_SHOPITEM_CHASE_SYN:
		{
			SEND_CHASEBASE* pmsg = (SEND_CHASEBASE*)pMsg;
			USERINFO* pUser = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pUser )		return;
			
			char Name[MAX_NAME_LENGTH+1];
			SafeStrCpy( Name, pmsg->WantedName, MAX_NAME_LENGTH+1 );
			
			ChaseFindUser( pmsg->dwObjectID, Name, pmsg->dwItemIdx );
		}
		break;
	case MP_ITEM_GIFT_EVENT_START:
		{
			MSGBASE* pmsg = ( MSGBASE* )pMsg;
			USERINFO* pUser = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pUser )		
			{
				FILE* pFile = fopen( "./log/GiftEventLog.txt", "a+" );
				fprintf( pFile, "%u\tInvalid User(MP_ITEMUserMsgParser) - character_idx : %u\n", gCurTime, pmsg->dwObjectID );
				fclose( pFile );
				return;
			}

//			if( pUser->UserLevel > eUSERLEVEL_GM )
//				return;

			DWORD count = GIFTMGR->GetStartEventCount();
			DWORD* index = GIFTMGR->GetStartEventIndex();

			for( int i = 0; i < count; i++ )
			{
				GIFTMGR->AddEvent( pmsg->dwObjectID, index[ i ] );
			}
			//GiftEvent( pUser->dwUserID );
		}
		break;
	default:
		TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
	}
}




void MP_ITEMServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_ITEM_SHOPITEM_CHANGEMAP_SYN:				Item_ShopItem_ChangeMap_Syn(pMsg) ;									break;
	case MP_ITEM_SHOPITEM_CHASE_ACK:					Item_ShopItem_Chase_Ack(pMsg) ;										break;
	case MP_ITEM_SHOPITEM_CHASE_NACK:					Item_ShopItem_Chase_Nack(pMsg) ;									break;
	//case MP_ITEM_SHOPITEM_SHOUT_ACK:					Item_ShopItem_Shout_Ack(dwConnectionIndex, pMsg, dwLength)  ;		break;
	//case MP_ITEM_SHOPITEM_SHOUT_SENDSERVER:				Item_ShopItem_Shout_SendServer(pMsg) ;								break;
	// 080121 KTH -- �ɸ����� �̸��� ����� ��� �ʼ������� �����ش�.
	case MP_ITEM_CHANGE_CHARACTER_NAME_SYN:				Item_Change_Character_Name(pMsg);									break;
	default:											TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );		
	}
}


void Item_ShopItem_ChangeMap_Syn(char* pMsg)
{
	SEND_MOVEDATA_SIMPLE* pmsg = (SEND_MOVEDATA_SIMPLE*)pMsg;
}


void Item_ShopItem_Chase_Ack(char* pMsg) 
{
	SEND_CHASEINFO* pmsg = (SEND_CHASEINFO*)pMsg;
	USERINFO* pUser = g_pUserTableForObjectID->FindUser( pmsg->CharacterIdx );
	if( !pUser )		return;

	g_Network.Send2User( pUser->dwConnectionIndex, (char*)pmsg, sizeof(*pmsg) );
}


void Item_ShopItem_Chase_Nack(char* pMsg) 
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	USERINFO* pUser = g_pUserTableForObjectID->FindUser( pmsg->dwData );
	if( !pUser )		return;

	pmsg->dwData = 2;
	g_Network.Send2User( pUser->dwConnectionIndex, (char*)pmsg, sizeof(*pmsg) );
}

// 080312 NYJ --- ��ü��ġ�� �����Լ� ����
/*
void Item_ShopItem_Shout_Ack(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength) 
{
	SEND_SHOUTBASE* pmsg = (SEND_SHOUTBASE*)pMsg;

	SHOUTRECEIVE rev;
	SHOUTMGR->AddShoutMsg( &pmsg->ShoutMsg[0], &rev );

	// �޼����� �� ��������
	if( rev.Count == 0 )
	{
		MSG_DWORD msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_SHOUT_NACK;
		msg.dwData = pmsg->ShoutMsg[0].CharacterIdx;
		g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );
	}
	else
	{
		SEND_SHOUTRECEIVE msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_SHOUT_ACK;
		msg.Receive = rev;
		msg.Receive.CharacterIdx = pmsg->ShoutMsg[0].CharacterIdx;
		g_Network.Send2Server( dwConnectionIndex,  (char*)&msg, sizeof(msg) );

		pmsg->Protocol = MP_ITEM_SHOPITEM_SHOUT_SENDSERVER;
		g_Network.Broadcast2AgentServerExceptSelf( (char*)pmsg, dwLength );
	}
}



void Item_ShopItem_Shout_SendServer(char* pMsg) 
{
	SEND_SHOUTBASE* pmsg = (SEND_SHOUTBASE*)pMsg;

	SHOUTMGR->AddShoutMsg( &pmsg->ShoutMsg[0] );
}
*/

// 080121 KTH -- �̸� ������ �Ͽ��� ���.
void Item_Change_Character_Name(char* pMsg)
{
	// MAP �������� �޾ƿ� ������ �ɹ� DB������ �����Ͽ��ش�. dbo.up_Character_Insert
	MSG_CHANGE_CHARACTER_NAME_AGENT* msg = (MSG_CHANGE_CHARACTER_NAME_AGENT*)pMsg;
	//USERINFO* pUser = g_pUserTableForObjectID->FindUser(msg->dwObjectID);
	char txt[128];
	sprintf(txt, "EXEC dbo.up_Character_Update_Name %d, %d, %s, %d", msg->CharacterIdx, msg->UserIdx, msg->changeName, g_nServerSetNum);
	g_DB.LoginQuery( eQueryType_FreeQuery, eChange_Character_Name, 0, txt );

}



#define GTMAPNUM		94
#define GTRETURNMAPNUM	52


void MP_GTOURNAMENTUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_GTOURNAMENT_MOVETOBATTLEMAP_SYN:			Gtournament_MoveToBattleMap_Syn(pMsg, dwLength) ;					break;
	case MP_GTOURNAMENT_STANDINGINFO_SYN:				Gtournament_StandingInfo_Syn(pMsg) ;								break;
	case MP_GTOURNAMENT_BATTLEJOIN_SYN:
	case MP_GTOURNAMENT_OBSERVERJOIN_SYN:				Gtournament_ObserverJoin_Syn(pMsg, dwLength) ;						break;
	case MP_GTOURNAMENT_LEAVE_SYN:						Gtournament_Leave_Syn(pMsg) ;										break;
	case MP_GTOURNAMENT_CHEAT:							Gtournament_Cheat(pMsg) ;											break;
	default:											TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );		break;
	}
}


void Gtournament_MoveToBattleMap_Syn(char* pMsg, DWORD dwLength)
{
	return;
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pUserInfo )			return;

	WORD tomapserverport = 0;
	//tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, Tournament );
	if(tomapserverport)
	{
		SERVERINFO* pInfo = g_pServerTable->FindServer( tomapserverport );
		if(pInfo)
		{
			//g_Network.Send2Server( pInfo->dwConnectionIndex, pMsg, dwLength );
			g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, pMsg, dwLength );
			return;
		}
		else
			goto MoveBattleMap_Error;
	}
	else
		goto MoveBattleMap_Error;

MoveBattleMap_Error:
	MSG_DWORD msg;
	msg.Category = MP_GTOURNAMENT;
	msg.Protocol = MP_GTOURNAMENT_MOVETOBATTLEMAP_NACK;
	msg.dwData = eGTError_ERROR;
	g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
}


void Gtournament_StandingInfo_Syn(char* pMsg) 
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;

	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pUserInfo )			return;

	WORD tomapserverport = 0;
	tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, GTMAPNUM );
	if(tomapserverport)
	{
		SERVERINFO* pInfo = g_pServerTable->FindServer( tomapserverport );
		if(pInfo)
		{
			MSG_DWORD2 msg;
			msg.Category = MP_GTOURNAMENT;
			msg.Protocol = MP_GTOURNAMENT_STANDINGINFO_SYN;
			msg.dwData1 = pmsg->wData;
			msg.dwData2 = pmsg->dwObjectID;
			g_Network.Send2Server( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
			return;
		}
		else
			goto StandingInfo_Error;
	}
	else
		goto StandingInfo_Error;

StandingInfo_Error:
	MSG_DWORD msg;
	msg.Category = MP_GTOURNAMENT;
	msg.Protocol = MP_GTOURNAMENT_STANDINGINFO_NACK;
	msg.dwData = eGTError_ERROR;
	g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
}


void Gtournament_ObserverJoin_Syn(char* pMsg, DWORD dwLength) 
{
	MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pUserInfo )			return;

	WORD tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, GTMAPNUM );
	if(tomapserverport)
	{
		SERVERINFO* pInfo = g_pServerTable->FindServer( tomapserverport );
		if(pInfo)
		{
			pUserInfo->wUserMapNum = GTMAPNUM;
			pUserInfo->dwMapServerConnectionIndex = pInfo->dwConnectionIndex;

			SEND_BATTLEJOIN_INFO msg;
			msg.Category = MP_GTOURNAMENT;
			msg.Protocol = pmsg->Protocol;
			msg.dwObjectID = pmsg->dwObjectID;
			msg.AgentIdx = pUserInfo->dwUniqueConnectIdx;					
			msg.UserLevel = pUserInfo->UserLevel;
			msg.GuildIdx = pmsg->dwData1;
			msg.BattleIdx = pmsg->dwData2;
			msg.ReturnMapNum = pmsg->dwData3;
			g_Network.Send2Server( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
			return;
		}
	}

//BattleJoin_Err:
	MSG_DWORD msg;
	msg.Category = MP_GTOURNAMENT;
	msg.Protocol = MP_GTOURNAMENT_BATTLEJOIN_NACK;
	msg.dwData = eGTError_ERROR;
	g_Network.Send2User( pUserInfo->dwConnectionIndex, pMsg, dwLength );
}


void Gtournament_Leave_Syn(char* pMsg) 
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;

	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pUserInfo )			return;

	WORD tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, GTRETURNMAPNUM );
	if(tomapserverport)
	{
		SERVERINFO* pInfo = g_pServerTable->FindServer( tomapserverport );
		if(pInfo)
		{
			pUserInfo->wUserMapNum = GTRETURNMAPNUM;
			pUserInfo->dwMapServerConnectionIndex = pInfo->dwConnectionIndex;

			MSG_DWORD3 msg;
			msg.Category = MP_GTOURNAMENT;
			msg.Protocol = MP_GTOURNAMENT_LEAVE_SYN;
			msg.dwObjectID = pmsg->dwObjectID;
			msg.dwData1 = pUserInfo->dwUniqueConnectIdx;
			msg.dwData2 = pUserInfo->UserLevel;
			msg.dwData3 = pUserInfo->wChannel;
			g_Network.Send2Server( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
			return;
		}
	}
	else
	{
		// �̵��Ϸ��� ���� ����..��.��
	}
}


void Gtournament_Cheat(char* pMsg) 
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			
	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pUserInfo )			return;
	
	MSG_DWORD2 msg;
	msg.Category = MP_GTOURNAMENT;
	msg.Protocol = MP_GTOURNAMENT_CHEAT;
	msg.dwData1 = pmsg->dwObjectID;
	msg.dwData2 = pmsg->dwData;
	if( pmsg->dwData == 1 )
	{
		g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg));
	}
	else if( pmsg->dwData == 2 )
	{
		WORD tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, GTMAPNUM );
		if(tomapserverport)
		{
			SERVERINFO* pInfo = g_pServerTable->FindServer( tomapserverport );
			if(pInfo)
			{
				g_Network.Send2Server( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
			}
		}
	}
}




void MP_GTOURNAMENTServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_GTOURNAMENT_CHEAT:						GtournamentServer_Cheat(pMsg, dwLength) ;								break;		
	case MP_GTOURNAMENT_STANDINGINFO_REGISTED:		GtournamentServer_StandingInfo_Registed(pMsg, dwLength) ;				break;
	case MP_GTOURNAMENT_RETURNTOMAP:				GtournamentServer_EeturntoMap(dwConnectionIndex, pMsg, dwLength) ;		break;
	case MP_GTOURNAMENT_NOTIFY_WINLOSE:				GtournamentServer_Notify_WinLose(pMsg, dwLength) ;						break;
	case MP_GTOURNAMENT_BATTLE_NOTIFY:				GtournamentServer_Battle_Notify(pMsg, dwLength) ;						break;
	case MP_GTOURNAMENT_STARTBATTLE:
	case MP_GTOURNAMENT_ENDBATTLE:					GtournamentServer_EndBattle(pMsg, dwLength)  ;							break;
	default:										TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );			break;
	}
}


void GtournamentServer_Cheat(char* pMsg, DWORD dwLength)
{
	MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwData1 );
	if( !pUserInfo )			return;

	g_Network.Send2User( pUserInfo->dwConnectionIndex, pMsg, dwLength );
}


void GtournamentServer_StandingInfo_Registed(char* pMsg, DWORD dwLength) 
{
	SEND_REGISTEDGUILDINFO* pmsg = (SEND_REGISTEDGUILDINFO*)pMsg;

	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->PlayerID );
	if( !pUserInfo )			return;

	g_Network.Send2User( pUserInfo->dwConnectionIndex, pMsg, dwLength );
}


void GtournamentServer_EeturntoMap(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pUserInfo )			return;

	WORD tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, (WORD)pmsg->dwData );
	if(tomapserverport)
	{
		SERVERINFO* pInfo = g_pServerTable->FindServer( tomapserverport );
		if( pInfo )
		{
			pUserInfo->wUserMapNum = (WORD)pmsg->dwData;
			pUserInfo->dwMapServerConnectionIndex = pInfo->dwConnectionIndex;
		}
	}

	TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );

	//--------------------------
}


void GtournamentServer_Notify_WinLose(char* pMsg, DWORD dwLength)
{
	SEND_GTWINLOSE* pmsg = (SEND_GTWINLOSE*)pMsg;

	USERINFO* info = NULL;
	g_pUserTable->SetPositionUserHead();
	while( info = g_pUserTable->GetUserData() )
	{
		SEND_GTWINLOSE msg = *pmsg;
		g_Network.Send2User( info->dwConnectionIndex, (char*)&msg, dwLength );
	}
}


void GtournamentServer_Battle_Notify(char* pMsg, DWORD dwLength)
{
	MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

	USERINFO* info = NULL;
	g_pUserTable->SetPositionUserHead();
	while( info = g_pUserTable->GetUserData() )
	{
		MSG_DWORD3 msg = *pmsg;
		g_Network.Send2User( info->dwConnectionIndex, (char*)&msg, dwLength );
	}
}


void GtournamentServer_EndBattle(char* pMsg, DWORD dwLength) 
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	USERINFO * info = NULL;
	g_pUserTable->SetPositionUserHead();
	while(info = g_pUserTable->GetUserData())
	{
		MSG_DWORD2 msg = *pmsg;
		g_Network.Send2User( info->dwConnectionIndex, (char*)&msg, dwLength );
	}
}


// 071115 ����, ���� ó�� ����
//void MP_JACKPOTUserMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
//{
//	
//}


// 071115 ����, ���� ó�� ����
//void MP_JACKPOTServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
//{
//	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
//	switch(pTempMsg->Protocol)
//	{
//	case MP_JACKPOT_PRIZE_NOTIFY:
//		{
//			MSG_JACKPOT_PRIZE_NOTIFY* pmsg = (MSG_JACKPOT_PRIZE_NOTIFY*)pMsg;
//
//			//SW050729	//DB Update ���� ���̸� ���ش�. & ���� �������� ��Ȯ�� �ѻ���� ���������� ����.
//			JACKPOTMGR->SetTotalMoney(pmsg->dwRestTotalMoney);
//
//			USERINFO* info = NULL;
//			g_pUserTable->SetPositionUserHead();
//			while( info = g_pUserTable->GetUserData() )
//			{
//				MSG_JACKPOT_PRIZE_NOTIFY msg = *pmsg;
//				g_Network.Send2User( info->dwConnectionIndex, (char*)&msg, dwLength );
//			}
//		}
//		break;
////	case MP_JACKPOT_TOTALMONEY_NOTIFY:
////		{
////			MSG_JACKPOT_TOTALMONEY_NOTIFY* pmsg = (MSG_JACKPOT_TOTALMONEY_NOTIFY*)pMsg;
////
////			//ObjectID�κ��� connectionidex �� Send2User
////		}
//	case MP_JACKPOT_TOTALMONEY_NOTIFY_TO_AGENT:
//		{
//			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
//
//			JACKPOTMGR->SetTotalMoney(pmsg->dwData); //���� AGENT�� ����
//
//			pmsg->Protocol = MP_JACKPOT_TOTALMONEY_NOTIFY;
//
//			USERINFO* info = NULL;
//			g_pUserTable->SetPositionUserHead();
//			while( info = g_pUserTable->GetUserData() )
//			{
//				if( info->wUserMapNum )
//				{
//					MSG_DWORD msgTemp = *pmsg;
//					g_Network.Send2User( info->dwConnectionIndex, (char*)&msgTemp, sizeof(msgTemp) );
//				}
//			}
//			
//		}
//		break;
//	default:
//		TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );
//		break;
//	}
//}

void MP_SkillServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_SKILL_START_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			
			SKILLDELAYMGR->AddSkillUse( pmsg->dwData1, pmsg->dwData2, TRUE );

		}
		break;
	// 080602 LUJ, ������ ��� ��ų ����� �ٸ� ������ �˸���
	case MP_SKILL_START_TO_MAP:
		{
			g_Network.Broadcast2MapServerExceptOne( dwConnectionIndex, pMsg, dwLength );
		}
		break;
	default:
		TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );		
		break;
	}
}


void MP_SkillUserMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_SKILL_START_SYN:
		{
			MSG_SKILL_START_SYN* pmsg = (MSG_SKILL_START_SYN*)pMsg;
			
			USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if(!pInfo)	return;

			if( SKILLDELAYMGR->AddSkillUse( pInfo->dwCharacterID, pmsg->SkillIdx ) )
			{
				TransToMapServerMsgParser(dwConnectionIndex,pMsg,dwLength);
			}
			else
			{
				//NACK
				
				MSG_BYTE msg;
				msg.Category = MP_SKILL;
				msg.Protocol = MP_SKILL_START_NACK;
				msg.dwObjectID = pInfo->dwCharacterID;
				msg.bData = 0;

				g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );

				ASSERT(0); //������ ��ŷ�� �õ����� ���ɼ��� �ִ�.
			}
		}
		break;
	default:
		TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
		break;
	}
}


// guildunion
void MP_GUILD_UNIONUserMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch( pTempMsg->Protocol )
	{
	case MP_GUILD_UNION_CREATE_SYN:
		{
			MSG_NAME_DWORD* pmsg = (MSG_NAME_DWORD*)pMsg;
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pInfo )	return;

			if( (FILTERTABLE->IsInvalidCharInclude( (unsigned char*)pmsg->Name ) == TRUE ) || 
				(FILTERTABLE->IsUsableName( pmsg->Name ) == FALSE ) )
			{
				MSG_DWORD2 Msg;
				Msg.Category = MP_GUILD_UNION;
				Msg.Protocol = MP_GUILD_UNION_CREATE_NACK;
				Msg.dwData1 = eGU_Not_ValidName;
				Msg.dwData2 = 0;
				g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&Msg, sizeof(Msg) );
			}
			else
				TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
		}
		break;
	default:
		TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
	}
}

void MP_GUILD_UNIONServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch( pTempMsg->Protocol )
	{
	case MP_GUILD_UNION_CREATE_NOTIFY_TO_MAP:
	case MP_GUILD_UNION_DESTROY_NOTIFY_TO_MAP:
	case MP_GUILD_UNION_INVITE_ACCEPT_NOTIFY_TO_MAP:
	case MP_GUILD_UNION_ADD_NOTIFY_TO_MAP:
	case MP_GUILD_UNION_REMOVE_NOTIFY_TO_MAP:
	case MP_GUILD_UNION_SECEDE_NOTIFY_TO_MAP:
	case MP_GUILD_UNION_MARK_REGIST_NOTIFY_TO_MAP:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	default:
		TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );
	}	
}



void MP_SIEGEWARUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_SIEGEWAR_CHEAT:					SiegeWar_Cheat(pMsg, dwLength) ;									break;
	case MP_SIEGEWAR_MOVEIN_SYN:			SiegeWar_MoveIn_Syn(pMsg, dwLength) ;								break;
	case MP_SIEGEWAR_BATTLEJOIN_SYN:
	case MP_SIEGEWAR_OBSERVERJOIN_SYN:		SiegeWar_ObserverJoin_Syn(pMsg, dwLength) ;							break;
	case MP_SIEGEWAR_LEAVE_SYN:				SiegeWar_Leave_Syn(pMsg) ;											break;
	default:								TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );		break;
	}
}


void SiegeWar_Cheat(char* pMsg, DWORD dwLength)
{
	MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;

	SERVERINFO* pInfo = NULL;

	WORD tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, (WORD)pmsg->dwData2 );
	if(tomapserverport)
	{
		pInfo = g_pServerTable->FindServer( tomapserverport );
		if(pInfo)
		{
			MSG_DWORD4 msg;
			memcpy( &msg, pmsg, sizeof(MSG_DWORD4) );
			g_Network.Send2Server( pInfo->dwConnectionIndex, pMsg, dwLength );
		}
	}

	tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, (WORD)pmsg->dwData3 );
	if(tomapserverport)
	{
		pInfo = g_pServerTable->FindServer( tomapserverport );
		if(pInfo)
		{
			MSG_DWORD4 msg;
			memcpy( &msg, pmsg, sizeof(MSG_DWORD4) );
			g_Network.Send2Server( pInfo->dwConnectionIndex, pMsg, dwLength );
		}
	}
}


void SiegeWar_MoveIn_Syn(char* pMsg, DWORD dwLength) 
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pUserInfo )			return;

	g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, pMsg, dwLength );

/*			WORD tomapserverport = 0;
	tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, pmsg->dwData3 );
	if(tomapserverport)
	{
		SERVERINFO* pInfo = g_pServerTable->FindServer( tomapserverport );
		if(pInfo)
		{
			//g_Network.Send2Server( pInfo->dwConnectionIndex, pMsg, dwLength );
			g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, pMsg, dwLength );
			return;
		}
		else
			goto MoveIn_Error;
	}
	else
		goto MoveIn_Error;

MoveIn_Error:
	MSG_DWORD msg;
	msg.Category = MP_SIEGEWAR;
	msg.Protocol = MP_SIEGEWAR_MOVEIN_NACK;
	msg.dwData = eGTError_ERROR;
	g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );*/
}


void SiegeWar_ObserverJoin_Syn(char* pMsg, DWORD dwLength) 
{
	MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;
	// dwData1 - GuildIdx
	// dwData2 - ReturnMapNum
	// dwData3 - bObserver
	// dwData4 - SeigeWarMapNum

	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pUserInfo )			return;

	WORD tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, (WORD)pmsg->dwData4 );
	if(tomapserverport)
	{
		SERVERINFO* pInfo = g_pServerTable->FindServer( tomapserverport );
		if(pInfo)
		{
			pUserInfo->wUserMapNum = (WORD)pmsg->dwData4;
			pUserInfo->dwMapServerConnectionIndex = pInfo->dwConnectionIndex;

			SEND_SIEGEWAR_JOININFO msg;
			msg.Category = MP_SIEGEWAR;
			msg.Protocol = pmsg->Protocol;
			msg.dwObjectID = pmsg->dwObjectID;
			msg.AgentIdx = pUserInfo->dwUniqueConnectIdx;					
			msg.UserLevel = pUserInfo->UserLevel;
			msg.GuildIdx = pmsg->dwData1;
			msg.ReturnMapNum = pmsg->dwData2;
			msg.bObserver = (BYTE)pmsg->dwData3;
			g_Network.Send2Server( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
			return;
		}
	}

	MSGBASE msg;
	msg.Category = MP_SIEGEWAR;
	msg.Protocol = MP_SIEGEWAR_BATTLEJOIN_NACK;			
	g_Network.Send2User( pUserInfo->dwConnectionIndex, pMsg, dwLength );
}


void SiegeWar_Leave_Syn(char* pMsg) 
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;

	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pUserInfo )			return;
	
	MSG_DWORD3 msg;
	msg.Category = MP_SIEGEWAR;
	msg.Protocol = MP_SIEGEWAR_LEAVE_SYN;
	msg.dwObjectID = pmsg->dwObjectID;
	msg.dwData1 = pUserInfo->dwUniqueConnectIdx;
	msg.dwData2 = pUserInfo->UserLevel;
	msg.dwData3 = pUserInfo->wChannel;
	g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg));
}




void MP_SIEGEWARServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
/*	case MP_SIEGEWAR_CHANGESTATE:
	case MP_SIEGEWAR_REGISTTIME:
	case MP_SIEGEWAR_DEFENCE_REGIST:
	case MP_SIEGEWAR_ATTACK_REGIST:
	case MP_SIEGEWAR_ACCEPTGUILD:	
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			WORD tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, pmsg->dwData1 );
			if(tomapserverport)
			{
				SERVERINFO* pInfo = g_pServerTable->FindServer( tomapserverport );
				if(pInfo)
					g_Network.Send2Server( pInfo->dwConnectionIndex, (char*)pmsg, dwLength );
			}
		}
		break;*/
	case MP_SIEGEWAR_TAXRATE:
		{
			SEND_AFFECTED_MAPLIST* pmsg = (SEND_AFFECTED_MAPLIST*)pMsg;

			//
			MSG_DWORD msg;
			SetProtocol( &msg, MP_SIEGEWAR, MP_SIEGEWAR_TAXRATE );
			msg.dwData = pmsg->Param;

			WORD tomapserverport = 0;
			SERVERINFO* pInfo = NULL;			
			for(int i=0; i<pmsg->Count; ++i)
			{
				tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, pmsg->MapList[i] );
				if( 0 == tomapserverport )			continue;

				pInfo = g_pServerTable->FindServer( tomapserverport );
				if( !pInfo )			continue;

				MSG_DWORD tmpMsg = msg;
				g_Network.Send2Server( pInfo->dwConnectionIndex, (char*)&tmpMsg, sizeof(tmpMsg) );								
			}
		}
		break;
	case MP_SIEGEWAR_RETURNTOMAP:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pUserInfo )			return;

			WORD tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, (WORD)pmsg->dwData );
			if(tomapserverport)
			{
				SERVERINFO* pInfo = g_pServerTable->FindServer( tomapserverport );
				if( pInfo )
				{
					pUserInfo->wUserMapNum = (WORD)pmsg->dwData;
					pUserInfo->dwMapServerConnectionIndex = pInfo->dwConnectionIndex;
				}
			}

			TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );

			//--------------------------
			
		}
		break;
	default:
		TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );		
		break;
	}
}

void MP_SIEGEWARPROFITUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
}

void MP_SIEGEWARPROFITServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch( pTempMsg->Protocol )
	{
	case MP_SIEGEWAR_PROFIT_CHANGE_TEXRATE_NOTIFY_TO_MAP:
	case MP_SIEGEWAR_PROFIT_CHANGE_GUILD_NOTIFY_TO_MAP:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	default:
		TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );
	}	
}

// 2005 ũ�������� �̺�Ʈ
// GM�� ���� ���� ������ �ʺ��� �ϱ� ����...
void MP_WEATHERUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;

	USERINFO* pUserInfo = g_pUserTable->FindUser( dwConnectionIndex );
	if( !pUserInfo ) return;
	if( !( pUserInfo->UserLevel == eUSERLEVEL_GM || pUserInfo->UserLevel == eUSERLEVEL_PROGRAMMER
		|| pUserInfo->UserLevel == eUSERLEVEL_DEVELOPER ) )
		return;

	if( pUserInfo->UserLevel == eUSERLEVEL_GM )	//AI��C ����A������ Ao��o����Ao..
	if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MASTER &&
		GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
		return;

	WORD	wServer = 0;

	switch( pTempMsg->Protocol )
	{
		case MP_WEATHER_SET:
		{
			MSG_WEATHER* msg = (MSG_WEATHER*)pMsg;

			wServer = msg->MapNum;
		}
		break;
	case MP_WEATHER_EXE:
		{
			MSG_WORD2* msg = (MSG_WORD2*)pMsg;

			wServer = msg->wData2;
		}
		break;
	case MP_WEATHER_RETURN:
		{
			MSG_WORD* msg = (MSG_WORD*)pMsg;

			wServer = msg->wData;
		}
		break;
	}
	
	WORD wServerPort = g_pServerTable->GetServerPort( eSK_MAP, wServer );
	SERVERINFO* pInfo = g_pServerTable->FindServer( wServerPort );
	if( pInfo )
		g_Network.Send2Server( pInfo->dwConnectionIndex, pMsg, dwLength );	
}

// DateMatching
void MP_DATEMATCHING_UserMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	USERINFO* pUserInfo = g_pUserTable->FindUser( dwConnectionIndex );

	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch( pTempMsg->Protocol )
	{
	case MP_DATEMATCHING_SEARCH_SYN:
		{
			MSG_DWORD2*		msg	=	(MSG_DWORD2*)pMsg;
			DWORD	dwAgeSelect		=	msg->dwData1;						// ���� ������ �ε���. ���� ��ü�� �ƴϴ�.
			DWORD	dwRegionSelect	=	msg->dwData2;						// ���� ������ �ε���.

			DATEMATCHINGMGR->AddUserInfo( pUserInfo);						// ����Ʈ ��Ī �޴����� ���.

			MSG_DATEMATCHING_SEARCH_RESULT	packet;							// ���� ��Ŷ.
			packet.Category	=	MP_DATEMATCHING;
			packet.Protocol	=	MP_DATEMATCHING_SEARCH_ACK;				// DATEMATCHING_SEARCH_RESULT

			DWORD	dwSearchCount	=	0;
			DATEMATCHINGMGR->SearchUser( &dwSearchCount, packet.pList, DATE_MATCHING_LIST_MAX, pUserInfo, dwAgeSelect, dwRegionSelect);
			
			packet.dwResultCount	=	dwSearchCount;
			g_Network.Send2User( dwConnectionIndex, (char*)&packet, packet.GetMsgLength());

			_asm nop;
		}
		break;
	case MP_DATEMATCHING_DEACTIVE_SYN:
		{
			// Ŭ���̾�Ʈ������ ����Ʈ ��Ī ���̾�α׸� ���ų� �ϸ� �� �޽����� ���ƿ´�.
			DATEMATCHINGMGR->RemoveUserInfo( pUserInfo);			// ����Ʈ �޴������� ����.
		}
		break;
	case MP_DATEMATCHING_REQUEST_TARGET_INFO_SYN:
		{
			MSG_DATEMATCHING_REQUEST_TARGET_INFO*	pPacket	=	(MSG_DATEMATCHING_REQUEST_TARGET_INFO*)pMsg;
			DATEMATCHINGMGR->OnRequestTargetInfo( pPacket->dwTargetUserID, pUserInfo);			// ObjID? ã�ƺ���.
		}
		break;
	case MP_DATEMATCHING_MODIFY_INTRODUCE_SYN:
		{
			MSG_DATEMATCHING_MODIFY_INTRODUCE*	pPacket	=	(MSG_DATEMATCHING_MODIFY_INTRODUCE*)pMsg;
			DATEMATCHINGMGR->OnModifyIntroduce( pPacket->szIntroduce, pUserInfo);
		}
		break;
	default:
		_asm nop;
	}



	_asm nop;

}

// desc_hseos_�ֹε��01
// S �ֹε�� �߰� added by hseos 2007.06.06
void MP_RESIDENTREGIST_UserMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	g_csResidentRegistManager.ASRV_ParseRequestFromClient(dwConnectionIndex, pMsg, dwLength);
}

void MP_RESIDENTREGIST_ServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	g_csResidentRegistManager.ASRV_ParseRequestFromServer(dwConnectionIndex, pMsg, dwLength);

}
// E �ֹε�� �߰� added by hseos 2007.06.06

// desc_hseos_�йи�01
// S �йи� �߰� added by hseos 2007.07.09	2007.07.11
void MP_FAMILY_UserMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	g_csFamilyManager.ASRV_ParseRequestFromClient(dwConnectionIndex, pMsg, dwLength);
}

void MP_FAMILY_ServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	g_csFamilyManager.ASRV_ParseRequestFromServer(dwConnectionIndex, pMsg, dwLength);
}
// E �йи� �߰� added by hseos 2007.07.09	2007.07.11

// desc_hseos_����ý���_01
// S ����ý��� �߰� added by hseos 2007.09.10
void MP_FARM_UserMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	g_csFarmManager.ASRV_ParseRequestFromClient(dwConnectionIndex, pMsg, dwLength);
}

void MP_FARM_ServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	g_csFarmManager.ASRV_ParseRequestFromServer(dwConnectionIndex, pMsg, dwLength);
}
// E ����ý��� �߰� added by hseos 2007.09.10

// desc_hseos_����Ʈ ��_01
// S ����Ʈ �� �߰� added by hseos 2007.11.16
void MP_DATE_UserMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	g_csDateManager.ASRV_ParseRequestFromClient(dwConnectionIndex, pMsg, dwLength);
}

void MP_DATE_ServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	g_csDateManager.ASRV_ParseRequestFromServer(dwConnectionIndex, pMsg, dwLength);
}
// E ����Ʈ �� �߰� added by hseos 2007.11.16

// desc_hseos_��ȥ_01
// S ��ȥ �߰� added by hseos 2007.12.12
void MP_MARRIAGE_UserMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
//	g_csMarriageManager.ASRV_ParseRequestFromClient(dwConnectionIndex, pMsg, dwLength);
}

void MP_MARRIAGE_ServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
//	g_csMarriageManager.ASRV_ParseRequestFromServer(dwConnectionIndex, pMsg, dwLength);
}
// E ��ȥ �߰� added by hseos 2007.12.12


//---KES AUTONOTE
void MP_AUTONOTE_UserMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	USERINFO* pUserInfo = g_pUserTable->FindUser( dwConnectionIndex );
	if( !pUserInfo ) return;

	MSGBASE* pTempMsg = (MSGBASE*)pMsg;

	if( pUserInfo->dwCharacterID )	pTempMsg->dwObjectID = pUserInfo->dwCharacterID;	//---KES ��� �޼����� ���ؼ� ����Ǿ�� �Ѵ�.. ���� ��ġ��.

	switch( pTempMsg->Protocol )
	{
	case MP_AUTONOTE_ASKTOAUTO_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			if( pUserInfo->UserLevel > eUSERLEVEL_GM )
			if( CPunishUnit* pPunishUnit = PUNISHMGR->GetPunishUnit( pUserInfo->dwUserID, ePunish_AutoNote ) )
			{
				DWORD dwRemainTime = pPunishUnit->GetRemainTime();
				MSG_DWORD msg;
				msg.Category	= MP_AUTONOTE;					//--- MP_PUNISH �� �ٲ���
				msg.Protocol	= MP_AUTONOTE_PUNISH;
				msg.dwData		= dwRemainTime;
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
				return;
			}

			TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
		}
		break;
	default:
		{
			TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
		}
		break;
	}
}

void MP_AUTONOTE_ServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;

	switch( pTempMsg->Protocol )
	{
	case MP_AUTONOTE_ASKTOAUTO_ACK:
		{
			MSGBASE*	pmsg = (MSGBASE*)pMsg;	//�����뿡�� �����ϴ� ���� �����ߴ�. 2�е��� �� ������ �Ұ����ϴ�.

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pTempMsg->dwObjectID);
			if( !pUserInfo ) return;

			//---�Ű��ڰ� 2�� �����Ʈ�� ��� ���ϵ���
			PunishListAdd( pUserInfo->dwUserID, ePunish_AutoNote, 2*60 );
			PUNISHMGR->AddPunishUnit( pUserInfo->dwUserID, ePunish_AutoNote, 2*60 );

			g_Network.Send2User( pUserInfo->dwConnectionIndex, pMsg, dwLength );
		}
		break;

	case MP_AUTONOTE_NOTAUTO:		//---���䰡 ���� ���߾��ٰ� �Ű��ڿ��� �����ش�.
		{
			MSG_DWORD*	pmsg = (MSG_DWORD*)pMsg;

			USERINFO* pUserInfo = g_pUserTableForUserID->FindUser( pmsg->dwData );	//---�ʼ������� ��� ������Ʈ�� ��ε�ĳ���� �Ȱ��̹Ƿ� ���� �ɸ���.
			if( !pUserInfo ) return;

			PUNISHMGR->AddPunishUnit( pUserInfo->dwUserID, ePunish_AutoNote, 60*60 );	//60��	// �Ű��ڰ� �����Ʈ�� �����ð� ��� ���ϵ��� ���縦 ����.

			if( pUserInfo->dwCharacterID )
			{
				MSGBASE msg;
				msg.Category	= MP_AUTONOTE;
				msg.Protocol	= MP_AUTONOTE_NOTAUTO;
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
			}
		}
		break;

	case MP_AUTONOTE_ANSWER_ACK:	//---�亯 ���� ���信�� ������ �޼���
		{
			MSG_DWORD*	pmsg = (MSG_DWORD*)pMsg;

			//---��밡 �亯�� ���߾�����, �Ű��ڰ� �����Ʈ�� �����ð� ��� ���ϵ���
			PunishListAdd( pmsg->dwData, ePunish_AutoNote, 60*60 );	//������Ʈ���� ���ϴ� ������ 1�ð� �����Ʈ ������ DB�� �߰�

			MSGBASE msg;
			msg.Category	= MP_AUTONOTE;
			msg.Protocol	= MP_AUTONOTE_ANSWER_ACK;
			msg.dwObjectID	= pmsg->dwObjectID;
			g_Network.Send2User( &msg, sizeof(msg) );
		}
		break;

	case MP_AUTONOTE_ANSWER_FAIL:
		{
			MSG_DWORD*	pmsg = (MSG_DWORD*)pMsg;

			PunishCountAdd( pmsg->dwData, ePunishCount_AutoUse, 1 );		//����������. ������ȸ�� 1ȸ����

			MSGBASE msg;
			msg.Category	= MP_AUTONOTE;
			msg.Protocol	= MP_AUTONOTE_ANSWER_FAIL;
			msg.dwObjectID	= pmsg->dwObjectID;
			g_Network.Send2User( &msg, sizeof(msg) );
		}
		break;

	case MP_AUTONOTE_ANSWER_LOGOUT:
		{
			MSG_DWORD*	pmsg = (MSG_DWORD*)pMsg;

            PunishCountAdd( pmsg->dwData, ePunishCount_AutoUse, 1 );		//����������. ������ȸ�� 1ȸ����
		}
		break;

	case MP_AUTONOTE_ANSWER_TIMEOUT:
		{
			MSG_DWORD*	pmsg = (MSG_DWORD*)pMsg;

            PunishCountAdd( pmsg->dwData, ePunishCount_AutoUse, 1 );		//����������. ������ȸ�� 1ȸ����

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pTempMsg->dwObjectID);
			if( pUserInfo )
				g_Network.Send2User( pUserInfo->dwConnectionIndex, pMsg, dwLength );
		}
		break;

	case MP_AUTONOTE_KILLAUTO:
		{
			MSG_DWORD*	pmsg = (MSG_DWORD*)pMsg;
			USERINFO* pUserInfo = g_pUserTableForUserID->FindUser( pmsg->dwData );
			if( !pUserInfo ) return;

			MSGBASE msg;
			msg.Category = MP_AUTONOTE;
			msg.Protocol = MP_AUTONOTE_KILLAUTO;

			if( pUserInfo->dwCharacterID )
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) ); 
		}
		break;

	case MP_AUTONOTE_DISCONNECT:
		{
			MSG_DWORD*	pmsg = (MSG_DWORD*)pMsg;
			USERINFO* pUserInfo = g_pUserTableForUserID->FindUser( pmsg->dwData );
			if( !pUserInfo ) return;

			DWORD dwConnectionIdx = pUserInfo->dwConnectionIndex;
			OnDisconnectUser( dwConnectionIdx );
			DisconnectUser( dwConnectionIdx );
		}
		break;

	default:
		{
			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pTempMsg->dwObjectID);
			if( pUserInfo )
				g_Network.Send2User( pUserInfo->dwConnectionIndex, pMsg, dwLength );
		}
		break;
	}
}
//----------------

//---KES PUNISH
void MP_PUNISH_ServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;

	switch( pTempMsg->Protocol )
	{
	case MP_PUNISH_PUNISH:
		{
//			MSG_DWORD3* pTempMsg = (MSG_DWORD3*)pMsg;
//			PUNISHMGR->AddPunishUnit( pANRoom->GetAskCharacterIdx(), ePunish_AutoNote, 60*60*1000 ); //60��
		}
		break;
	}
}
//-------------

void MP_FISHING_UserMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	default:											TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );		break;
	}
}

void MP_FISHING_ServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	default:											TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );		break;
	}
}


// 080407 LYW --- ServerSystem : Add a user message parser for chatroom system.
void MP_CHATROOM_UserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		MessageBox( NULL, "Invalid a message parameter!!", "MP_CHATROOM_UserMsgParser", MB_OK) ;
		return ;
	}

//===CHATROOM �ӽ� ��
	//CHATROOMMGR->UserMsgParser(dwConnectionIndex, pMsg, dwLength) ;
}

// 080407 LYW --- ServerSystem : Add a server message parser for chatroom system.
void MP_CHATROOM_ServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		MessageBox( NULL, "Invalid a message parameter!!", "MP_CHATROOM_ServerMsgParser", MB_OK) ;
		return ;
	}
//===CHATROOM �ӽ� ��
	//CHATROOMMGR->ServerMsgParser(dwConnectionIndex, pMsg, dwLength) ;
}

//void MP_NPCMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
//{
//	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
//	switch(pTempMsg->Protocol)
//	{
//	case MP_NPC_SPEECH_SYN:
//	case MP_NPC_CLOSEBOMUL_SYN:
//	case MP_NPC_OPENBOMUL_SYN:
//	case MP_NPC_DOJOB_SYN:
//		{
//			TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
//		}
//		break;
//	default:												
//		{
//			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pTempMsg->dwObjectID);
//			if( pUserInfo )
//			{
//				g_Network.Send2User( pUserInfo->dwConnectionIndex, pMsg, dwLength );	
//			}
//			break;
//		}
//	};
//}

#ifdef _HACK_SHIELD_
void MP_HACKSHIELDUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	HACKSHIELDMGR->NetworkMsgParse(dwConnectionIndex, pMsg, dwLength);
}
#endif

#ifdef _NPROTECT_
void MP_NPROTECTUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	NPROTECTMGR->NetworkMsgParse(dwConnectionIndex, pMsg, dwLength);
}
#endif

//=================================================================================================
//	FILE		: CChatRoomMgr.cpp
//	DESC		: Implementation part of chatroom manager class.
//	DATE		: MARCH 31, 2008 LYW
//=================================================================================================





//-------------------------------------------------------------------------------------------------
//		Include part.
//-------------------------------------------------------------------------------------------------
#include "stdafx.h"																						// An include file for standard system include files

#include "../[CC]ServerModule/Network.h"																// An include file for network.

#include "./ChatRoomMgr.h"																				// An include file for chatroom manager.

#include "./AgentDBMsgParser.h"
#include "./FilteringTable.h"





//-------------------------------------------------------------------------------------------------
//	NAME : CChatRoomMgr
//	DESC : The function constructor.
//  DATE : MARCH 31, 2008 LYW
//-------------------------------------------------------------------------------------------------
CChatRoomMgr::CChatRoomMgr(void)
{
}





//-------------------------------------------------------------------------------------------------
//	NAME : ~CChatRoomMgr
//	DESC : The function destructor.
//  DATE : MARCH 31, 2008 LYW
//-------------------------------------------------------------------------------------------------
CChatRoomMgr::~CChatRoomMgr(void)
{
}





//-------------------------------------------------------------------------------------------------
//	NAME : RegistPlayer_To_Lobby
//	DESC : ���� ������ ��� Dist ������ ������ �Լ�.
//  DATE : MARCH 31, 2008 LYW
//-------------------------------------------------------------------------------------------------
BYTE CChatRoomMgr::IsAddedUser(DWORD dwCharacterID)
{
	// ���� ���� �ޱ�.
	USERINFO* pInfo = NULL ;
	pInfo = g_pUserTableForObjectID->FindUser(dwCharacterID) ;

	ASSERT(pInfo) ;

	if(!pInfo)
	{
		char tempMsg[128] = {0, } ;
		sprintf(tempMsg, "%d : Failed to find user to user id", dwCharacterID) ;

		Throw_Error(tempMsg, __FUNCTION__) ;

		return FALSE ;
	}

	if(pInfo->byAddedChatSystem) return TRUE ;
	else return FALSE ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : RegistPlayer_To_Lobby
//	DESC : ���� ������ ��� Dist ������ ������ �Լ�.
//  DATE : MARCH 31, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::RegistPlayer_To_Lobby(USERINFO* pUserInfo, BYTE byProtocol) 
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pUserInfo) ;

	if(!pUserInfo)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// �ӽ� ���� ������.
	int nNameLength ;

	ST_CR_USER guest ;
	memset(&guest, 0, sizeof(ST_CR_USER)) ;

	for(BYTE count = 0 ; count < MAX_CHARACTER_NUM ; ++count)
	{
		if(pUserInfo->SelectInfoArray[count].dwCharacterID != pUserInfo->dwCharacterID) continue ;

		// ĳ���� ������ �޴´�.
		guest.dwUserID			= pUserInfo->dwUserID ;

		guest.dwPlayerID	= pUserInfo->SelectInfoArray[count].dwCharacterID ; 
		//guest.wClassIdx		= pUserInfo->SelectInfoArray[count].wClassIndex ;
		guest.byLevel		= (BYTE)pUserInfo->SelectInfoArray[count].Level ;
		guest.byMapNum		= (BYTE)pUserInfo->SelectInfoArray[count].MapNum ;

		// ĳ������ �̸��� üũ�Ѵ�.
		nNameLength = 0 ;
		nNameLength = strlen(pUserInfo->SelectInfoArray[count].name) ;
		ASSERT(nNameLength > 0) ;
		if(nNameLength <= 0)
		{
			Throw_Error("Failed to receive user name!!", __FUNCTION__) ;
		}

		strcpy(guest.name, pUserInfo->SelectInfoArray[count].name) ;

		// ĳ���� ������ ��´�.
		MSG_CR_USER msg ;
		memset(&msg, 0, sizeof(MSG_CR_USER)) ;

		msg.Category	= MP_CHATROOM ;
		msg.Protocol	= byProtocol ;
		msg.dwObjectID	= guest.dwPlayerID ;

		memcpy(&msg.user, &guest, sizeof(ST_CR_USER)) ;

		// Dist�� ��� �޽��� ����.
		g_Network.Send2DistributeServer((char*)&msg, sizeof(MSG_CR_USER)) ;

		// ĳ������ ���¸� Lobby�� �ִ� ���·� ����.
		pUserInfo->byAddedChatSystem = TRUE ;

		break ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : RegistPlayer_To_Lobby
//	DESC : ���� ������ ��� Dist ������ ������ �Լ�.
//  DATE : MAY 7, 2008 LYW
//-------------------------------------------------------------------------------------------------
BYTE CChatRoomMgr::RegistPlayer_To_Lobby(ST_CR_USER* pUserInfo)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pUserInfo) ;

	if(!pUserInfo)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return FALSE ;
	}



	// �̸� üũ.
	int nNameLength = 0 ;
	nNameLength = strlen(pUserInfo->name) ;

	ASSERT(nNameLength > 0) ;
	if(nNameLength <= 0 || !pUserInfo->name)
	{
		Throw_Error("Failed to receive name!!", __FUNCTION__) ;
		return FALSE ;
	}



	// ĳ���� ������ ��´�.
	MSG_CR_USER msg ;
	memset(&msg, 0, sizeof(MSG_CR_USER)) ;

	msg.Category	= MP_CHATROOM ;
	msg.Protocol	= MP_CHATROOM_ADD_USER_SYN ;
	msg.dwObjectID	= pUserInfo->dwPlayerID ;

	memcpy(&msg.user, pUserInfo, sizeof(ST_CR_USER)) ;

	// Dist�� ��� �޽��� ����.
	g_Network.Send2DistributeServer((char*)&msg, sizeof(MSG_CR_USER)) ;

	return TRUE ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : ForceRegistPlayer_To_Lobby
//	DESC : ������ ĳ���� ������ Dist�� ��� ��Ű�� �Լ�.
//  DATE : APRIL 14, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::ForceRegistPlayer_To_Lobby(DWORD dwIndex, char* pMsg, DWORD dwLength)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// �⺻ �޽��� ��ȯ.
	MSGBASE* pmsg = NULL ;
	pmsg = (MSGBASE*)pMsg ;

	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}

	// ���� ���� �ޱ�.
	USERINFO* pInfo = NULL ;
	pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID) ;

	if(!pInfo)
	{
		Throw_Error(err_FRUI, __FUNCTION__) ;

		MSGBASE msg ;
		memset(&msg, 0, sizeof(MSGBASE)) ;

		msg.Category	= MP_CHATROOM ;
		msg.Protocol	= MP_CHATROOM_FORCE_ADD_USER_NACK ;
		msg.dwObjectID	= pmsg->dwObjectID ;

		g_Network.Send2User(dwIndex, (char*)&msg, sizeof(MSGBASE)) ;

		return ;
	}

	// ���� ���� ���.
	RegistPlayer_To_Lobby(pInfo, MP_CHATROOM_FORCE_ADD_USER_SYN) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : ChatMsg_Normal_Syn
//	DESC : ä�ù� ������ �޽��� ó���� �ϴ� �Լ�.
//  DATE : APRIL 24, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::ChatMsg_Normal_Syn(DWORD dwIndex, char* pMsg, DWORD dwLength)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_CR_MSG_BROADCAST* pmsg = NULL ;
	pmsg = (MSG_CR_MSG_BROADCAST*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}

	// �޽��� ���� ó��.
	USERINFO* pInfo ;
	for(BYTE count = 0 ; count < pmsg->byCount ; ++count)
	{
		// ������ �˻�.
		pInfo = NULL ;
		pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwUser[count]) ;

		if(!pInfo) continue ;

		MSG_CR_MSG msg ;
		memset(&msg, 0, sizeof(MSG_CR_MSG)) ;

		msg.Category	= MP_CHATROOM ;
		msg.Protocol	= MP_CHATROOM_CHATMSG_NORMAL_NOTICE ;
		msg.dwObjectID	= pmsg->dwObjectID ;

		SafeStrCpy(msg.name, pmsg->name, MAX_NAME_LENGTH) ;
		SafeStrCpy(msg.Msg, pmsg->Msg, MAX_CHAT_LENGTH) ;

		// �޽����� �������� ����.
		g_Network.Send2User(pInfo->dwConnectionIndex, (char*)&msg, sizeof(MSG_CR_MSG)) ;
	}

	// �ٸ� ������Ʈ���� ����.
	if(pmsg->Protocol == MP_CHATROOM_CHATMSG_NORMAL_SYN)
	{
		MSG_CR_MSG_BROADCAST msg ;
		memset(&msg, 0, sizeof(MSG_CR_MSG_BROADCAST)) ;

		msg.Category	= MP_CHATROOM ;
		msg.Protocol	= MP_CHATROOM_CHATMSG_NORMAL_NOTICE_OTHER_AGENT ;
		msg.dwObjectID	= pmsg->dwObjectID ;

		SafeStrCpy(msg.name, pmsg->name, MAX_NAME_LENGTH) ;
		SafeStrCpy(msg.Msg, pmsg->Msg, MAX_CHAT_LENGTH+1) ;

		for(BYTE count = 0 ; count < pmsg->byCount ; ++count)
		{
			msg.dwUser[count] = pmsg->dwUser[count] ;

			++msg.byCount ;
		}

		// �ٸ� ������Ʈ�� �޽����� ����.
		g_Network.Send2AgentExceptThis((char*)&msg, sizeof(MSG_CR_MSG_BROADCAST)) ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : DestroyPlayer_From_Lobby
//	DESC : ���� ������ �����ϴ� �Լ�.
//  DATE : APRIL 2, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::DestroyPlayer_From_Lobby(USERINFO* pUserInfo) 
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pUserInfo) ;

	if(!pUserInfo)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	if(pUserInfo->dwCharacterID == 0 || pUserInfo->dwUserID == 0) return ;

	pUserInfo->byAddedChatSystem = FALSE ;
	
	// ���� ���� ��û�� ������.
	MSGBASE msg ;
	memset(&msg, 0, sizeof(MSGBASE)) ;

	msg.Category	= MP_CHATROOM ;
	msg.Protocol	= MP_CHATROOM_DEL_USER_SYN ;
	msg.dwObjectID	= pUserInfo->dwCharacterID ;

	g_Network.Send2DistributeServer((char*)&msg, sizeof(MSGBASE)) ;
}






//-------------------------------------------------------------------------------------------------
//	NAME : UpdatePlayerInfo
//	DESC : ������ ������ ������Ʈ ��Ű�� �Լ�.
//  DATE : MARCH 31, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::UpdatePlayerInfo(USERINFO* pUserInfo)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pUserInfo) ;

	if(!pUserInfo)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// �ӽ� ���� ������.
	ST_CR_USER guest ;
	memset(&guest, 0, sizeof(ST_CR_USER)) ;

	for(BYTE count = 0 ; count < MAX_CHARACTER_NUM ; ++count)
	{
		if(pUserInfo->SelectInfoArray[count].dwCharacterID != pUserInfo->dwCharacterID) continue ;

		// ĳ���� ������ �޴´�.
		//guest.wClassIdx		= pUserInfo->SelectInfoArray[count].wClassIndex ;
		guest.byLevel		= (BYTE)pUserInfo->SelectInfoArray[count].Level ;
		guest.byMapNum		= (BYTE)pUserInfo->SelectInfoArray[count].MapNum ;

		strcpy(guest.name, pUserInfo->SelectInfoArray[count].name) ;

		MSG_CR_USER msg ;
		memset(&msg, 0, sizeof(MSG_CR_USER)) ;

		msg.Category	= MP_CHATROOM ;
		msg.Protocol	= MP_CHATROOM_UPDATE_USERINFO_SYN ;
		msg.dwObjectID	= pUserInfo->dwCharacterID ;

		msg.user.byLevel		= guest.byLevel ;
		msg.user.byMapNum	= guest.byMapNum ;
		//msg.user.wClassIdx	= guest.wClassIdx ;

		SafeStrCpy(msg.user.name, guest.name, MAX_NAME_LENGTH) ;

		g_Network.Send2DistributeServer((char*)&msg, sizeof(MSG_CR_USER)) ;

		break ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : UserMsgParser
//	DESC : ������ ���� ���� ��Ʈ��ũ �޽��� ó���� �ϴ� �Լ�.
//  DATE : MARCH 31, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::UserMsgParser(DWORD dwIndex, char* pMsg, DWORD dwLength)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// �⺻ �޽��� ��ȯ.
	MSGBASE* pmsg = NULL ;
	pmsg = (MSGBASE*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTB, __FUNCTION__) ;
		return ;
	}

	// �������� üũ.
	switch(pmsg->Protocol)
	{
	case MP_CHATROOM_FORCE_ADD_USER_SYN :	ForceRegistPlayer_To_Lobby(dwIndex, pMsg, dwLength) ;	break ;
	case MP_CHATROOM_CHATMSG_NORMAL_SYN :	ChatMsg_Normal_Syn(dwIndex, pMsg, dwLength) ;			break ;
	default : g_Network.Send2DistributeServer(pMsg, dwLength) ;										break ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : ServerMsgParser
//	DESC : ������ ���� ���� ��Ʈ��ũ �޽��� ó���� �ϴ� �ռ�.
//  DATE : MARCH 31, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::ServerMsgParser(DWORD dwIndex, char* pMsg, DWORD dwLength)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// �⺻ �޽��� ��ȯ.
	MSGBASE* pmsg = NULL ;
	pmsg = (MSGBASE*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTB, __FUNCTION__) ;
		return ;
	}
	
	// �������� Ȯ��.
	switch(pmsg->Protocol)
	{
	case MP_CHATROOM_ADD_USER_ACK :

	case MP_CHATROOM_ROOMLIST_ACK :
	case MP_CHATROOM_ROOMLIST_NACK :

	case MP_CHATROOM_CREATE_ROOM_ACK :	
	case MP_CHATROOM_CREATE_ROOM_NACK :

	case MP_CHATROOM_JOIN_ROOM_ACK :
	case MP_CHATROOM_JOIN_ROOM_NACK :

	case MP_CHATROOM_OUT_ROOM_ACK :
	case MP_CHATROOM_OUT_ROOM_NACK :
	case MP_CHATROOM_OUT_ROOM_EMPTYROOM :
	case MP_CHATROOM_OUT_ROOM_LAST_MAN :

	case MP_CHATROOM_CHANGE_OPTION_NACK :

	case MP_CHATROOM_CHANGE_OWNER_NACK :

	case MP_CHATROOM_KICK_GUEST_NACK :

	case MP_CHATROOM_REQUEST_FRIEND_NACK :

	case MP_CHATROOM_CHATMSG_NORMAL_NACK :	

	case MP_CHATROOM_SEARCH_FOR_NAME_ACK :
	case MP_CHATROOM_SEARCH_FOR_NAME_NACK :

	case MP_CHATROOM_SEARCH_FOR_TITLE_ACK :
	case MP_CHATROOM_SEARCH_FOR_TITLE_NACK :

	case MP_CHATROOM_UPDATEINFO_CREATED_ROOM :
	case MP_CHATROOM_UPDATEINFO_DELETED_ROOM :
	case MP_CHATROOM_UPDATEINFO_SECRETMODE :
	case MP_CHATROOM_UPDATEINFO_ROOMTYPE :
	case MP_CHATROOM_UPDATEINFO_TITLE :
	case MP_CHATROOM_UPDATEINFO_CUR_GUESTCOUNT :
	case MP_CHATROOM_UPDATEINFO_TOTAL_GUESTCOUNT :				SendMsg2User(dwIndex, pMsg, dwLength) ; break ;

	case MP_CHATROOM_JOIN_ROOM_NOTICE :					
	case MP_CHATROOM_JOIN_ROOM_OTHER_AGENT :					Join_Room_Notice(dwIndex, pMsg, dwLength) ; break ;

	case MP_CHATROOM_OUT_ROOM_NOTICE :
	case MP_CHATROOM_OUT_ROOM_NOTICE_OTHER_AGENT :				Out_Room_Notice(dwIndex, pMsg, dwLength) ; break ;

	case MP_CHATROOM_OUT_ROOM_CHANGE_OWNER_NOTICE :
	case MP_CHATROOM_OUT_ROOM_CHANGE_OWNER_NOTICE_OTHER_AGENT : Out_Room_Change_Owner_Notcie(dwIndex, pMsg, dwLength) ; break ;		

	case MP_CHATROOM_CHANGE_OPTION_NOTICE :
	case MP_CHATROOM_CHANGE_OPTION_NOTICE_OTHER_AGENT :			Change_Option_Notice(dwIndex, pMsg, dwLength) ; break ;

	case MP_CHATROOM_CHANGE_OWNER_NOTICE :
	case MP_CHATROOM_CHANGE_OWNER_NOTICE_OTHER_AGENT :			Change_Owner_Notice(dwIndex, pMsg, dwLength) ; break ;

	case MP_CHATROOM_KICK_GUEST_ACK :
	case MP_CHATROOM_KICK_GUEST_ACK_OTHER_AGENT :				Kick_Guest_Ack(dwIndex, pMsg, dwLength) ; break ;

	case MP_CHATROOM_KICK_GUEST_NOTICE :
	case MP_CHATROOM_KICK_GUEST_NOTICE_OTHER_AGENT :			Kick_Guest_Notice(dwIndex, pMsg, dwLength) ; break ;

	case MP_CHATROOM_REQUEST_FRIEND_ACK :						Request_Friend_Ack(dwIndex, pMsg, dwLength) ; break ;

	case MP_CHATROOM_CHATMSG_NORMAL_NOTICE_OTHER_AGENT :		ChatMsg_Normal_Syn(dwIndex, pMsg, dwLength) ; break ;

	default : break ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : SendMsg2User
//	DESC : ����(Dist)�� ���� ���� �� �޽����� Ŭ���̾�Ʈ�� ����.
//  DATE : APRIL 2, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::SendMsg2User(DWORD dwIndex, char* pMsg, DWORD dwLength) 
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSGBASE* pmsg = NULL ;
	pmsg = (MSGBASE*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}


	// ���� ã��.
	USERINFO* pInfo = NULL ;
	pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID) ;

	if(!pInfo) return ;

	// �޽��� ����.
	g_Network.Send2User(pInfo->dwConnectionIndex, pMsg, dwLength) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Join_Room_Ack
//		 : 
//	DESC : ä�ù� ���� ���� ó���� �ϴ� �Լ�.
//		 : 
//		 : 1. ���ۿ� �޽��� ����.
//		 : 2. ���� �˻�.
//		 : 3. ������ �����ϰ��� ���� �޽��� ����.
//		 : 4. ������ ������ �ƴҰ�쿡�� ���� ���� ����.
//		 : 5. �ٸ� ������Ʈ���� ���� ������� ����.
//		 : 
//  DATE : APRIL 7, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Join_Room_Notice(DWORD dwIndex, char* pMsg, DWORD dwLength)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_CR_JOIN_NOTICE* pmsg = NULL ;
	pmsg = (MSG_CR_JOIN_NOTICE*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}

	// ���� ���Ǵ� ���� - ������ �ޱ�.
	int nSize = 0 ;
	nSize = sizeof(MSG_CR_JOIN_NOTICE) ;

	USERINFO* pInfo ;
	for(BYTE count = 0 ; count < pmsg->byCount ; ++count)
	{
		// ������ �˻�.
		pInfo = NULL ;
		pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwUser[count]) ;

		if(!pInfo) continue ;

		// �޽����� ����.
		MSG_CR_JOIN_NOTICE sendMsg ;
		memset(&sendMsg, 0, nSize) ;
		memcpy(&sendMsg, pmsg, nSize) ;

		sendMsg.Protocol = MP_CHATROOM_JOIN_ROOM_NOTICE ;

		// �޽����� �������� ����.
		g_Network.Send2User(pInfo->dwConnectionIndex, (char*)&sendMsg, nSize) ;
	}

	// �ٸ� ������Ʈ���� ����.
	if(pmsg->Protocol == MP_CHATROOM_JOIN_ROOM_NOTICE)
	{
		// �޽����� ����.
		MSG_CR_JOIN_NOTICE sendMsg ;
		memset(&sendMsg, 0, nSize) ;
		memcpy(&sendMsg, pmsg, nSize) ;

		// �ٸ� ������Ʈ���� ���� �� �������� ����.
		sendMsg.Protocol = MP_CHATROOM_JOIN_ROOM_OTHER_AGENT ;

		// �ٸ� ������Ʈ�� �޽����� ����.
		g_Network.Send2AgentExceptThis((char*)&sendMsg, nSize) ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : Out_Room_Notice
//		 : 
//	DESC : ä�ù� ������ ���� ó���� �ϴ� �Լ�.
//		 : 
//		 : 1. ���� �˻�.
//		 : 2. ������ �����ϰ��� ���� �޽��� ����.
//		 : 3. ������ ������ �ƴҰ�쿡�� ���� ���� ����.
//		 : 4. �ٸ� ������Ʈ���� ���� ������� ����.
//		 : 
//  DATE : APRIL 7, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Out_Room_Notice(DWORD dwIndex, char* pMsg, DWORD dwLength)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_CR_IDNAME* pmsg = NULL ;
	pmsg = (MSG_CR_IDNAME*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}

	// ���� ���Ǵ� ���� - ������ �ޱ�.
	int nSize = 0 ;
	nSize = sizeof(MSG_CR_IDNAME) ;

	USERINFO* pInfo ;
	for(BYTE count = 0 ; count < pmsg->byCount ; ++count)
	{
		// ������ �˻�.
		pInfo = NULL ;
		pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwUser[count]) ;

		if(!pInfo) continue ;

		// �޽����� ����.
		MSG_CR_IDNAME sendMsg ;
		memset(&sendMsg, 0, nSize) ;
		memcpy(&sendMsg, pmsg, nSize) ;

		sendMsg.Protocol = MP_CHATROOM_OUT_ROOM_NOTICE ;

		// �޽����� �������� ����.
		g_Network.Send2User(pInfo->dwConnectionIndex, (char*)&sendMsg, nSize) ;
	}

	// �ٸ� ������Ʈ���� ����.
	if(pmsg->Protocol == MP_CHATROOM_OUT_ROOM_NOTICE)
	{
		// �޽����� ����.
		MSG_CR_IDNAME sendMsg ;
		memset(&sendMsg, 0, nSize) ;
		memcpy(&sendMsg, pmsg, nSize) ;

		// �ٸ� ������Ʈ���� ���� �� �������� ����.
		sendMsg.Protocol = MP_CHATROOM_OUT_ROOM_NOTICE_OTHER_AGENT ;

		// �ٸ� ������Ʈ�� �޽����� ����.
		g_Network.Send2AgentExceptThis((char*)&sendMsg, nSize) ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : Out_Room_Change_Owner_Notcie
//		 : 
//	DESC : ä�ù��� ���� ��� ó���� �ϴµ�, ������ ���������� ���� ��η� ������ �� ó���ϴ� �Լ�.
//		 : 
//		 : 1. ���� �˻�.
//		 : 2. �޽��� ����.
//		 : 3. �ٸ� ������Ʈ���� ���� ������� ����.
//		 : 
//  DATE : APRIL 14, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Out_Room_Change_Owner_Notcie(DWORD dwIndex, char* pMsg, DWORD dwLength)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_CR_IDNAME* pmsg = NULL ;
	pmsg = (MSG_CR_IDNAME*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}

	// ���� ���Ǵ� ���� - ������ �ޱ�.
	int nSize = 0 ;
	nSize = sizeof(MSG_CR_IDNAME) ;

	USERINFO* pInfo ;
	for(BYTE count = 0 ; count < pmsg->byCount ; ++count)
	{
		// ������ �˻�.
		pInfo = NULL ;
		pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwUser[count]) ;

		if(!pInfo) continue ;

		// �޽����� ����.
		MSG_CR_IDNAME sendMsg ;
		memset(&sendMsg, 0, nSize) ;
		memcpy(&sendMsg, pmsg, nSize) ;

		sendMsg.Protocol = MP_CHATROOM_OUT_ROOM_CHANGE_OWNER_NOTICE ;

		// �޽����� �������� ����.
		g_Network.Send2User(pInfo->dwConnectionIndex, (char*)&sendMsg, nSize) ;
	}

	// �ٸ� ������Ʈ���� ����.
	if(pmsg->Protocol == MP_CHATROOM_OUT_ROOM_CHANGE_OWNER_NOTICE)
	{
		// �޽����� ����.
		MSG_CR_IDNAME sendMsg ;
		memset(&sendMsg, 0, nSize) ;
		memcpy(&sendMsg, pmsg, nSize) ;

		// �ٸ� ������Ʈ���� ���� �� �������� ����.
		sendMsg.Protocol = MP_CHATROOM_OUT_ROOM_CHANGE_OWNER_NOTICE_OTHER_AGENT ;

		// �ٸ� ������Ʈ�� �޽����� ����.
		g_Network.Send2AgentExceptThis((char*)&sendMsg, nSize) ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : Change_Option_Notice
//		 : 
//	DESC : ä�ù� �ɼ��� ����Ǿ��ٴ� ó���� �ϴ� �Լ�.
//		 : 
//		 : 1. ���� �˻�.
//		 : 2. �޽��� ����.
//		 : 3. �ٸ� ������Ʈ���� ���� ������� ����.
//		 : 
//  DATE : APRIL 14, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Change_Option_Notice(DWORD dwIndex, char* pMsg, DWORD dwLength)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_CR_ROOM_NOTICE* pmsg = NULL ;
	pmsg = (MSG_CR_ROOM_NOTICE*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}

	// ���� ���Ǵ� ���� - ������ �ޱ�.
	int nSize = 0 ;
	nSize = sizeof(MSG_CR_ROOM_NOTICE) ;

	USERINFO* pInfo ;
	for(BYTE count = 0 ; count < pmsg->byCount ; ++count)
	{
		// ������ �˻�.
		pInfo = NULL ;
		pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwUser[count]) ;

		if(!pInfo) continue ;

		// �޽����� ����.
		MSG_CR_ROOM_NOTICE sendMsg ;
		memset(&sendMsg, 0, nSize) ;
		memcpy(&sendMsg, pmsg, nSize) ;

		sendMsg.Protocol = MP_CHATROOM_CHANGE_OPTION_NOTICE ;

		// �޽����� �������� ����.
		g_Network.Send2User(pInfo->dwConnectionIndex, (char*)&sendMsg, nSize) ;
	}

	// �ٸ� ������Ʈ���� ����.
	if(pmsg->Protocol == MP_CHATROOM_CHANGE_OPTION_NOTICE)
	{
		// �޽����� ����.
		MSG_CR_ROOM_NOTICE sendMsg ;
		memset(&sendMsg, 0, nSize) ;
		memcpy(&sendMsg, pmsg, nSize) ;

		// �ٸ� ������Ʈ���� ���� �� �������� ����.
		sendMsg.Protocol = MP_CHATROOM_CHANGE_OPTION_NOTICE_OTHER_AGENT ;

		// �ٸ� ������Ʈ�� �޽����� ����.
		g_Network.Send2AgentExceptThis((char*)&sendMsg, nSize) ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : Change_Owner_Notice
//		 :
//	DESC : ���� ���� ���� ó���� �ϴ� �Լ�.
//		 : 
//		 : 1. ���� �˻�.
//		 : 2. �޽��� ����.
//		 : 3. �ٸ� ������Ʈ���� ���� ������� ����.
//		 : 
//  DATE : APRIL 14, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Change_Owner_Notice(DWORD dwIndex, char* pMsg, DWORD dwLength)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_CR_IDNAME* pmsg = NULL ;
	pmsg = (MSG_CR_IDNAME*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}

	// ���� ���Ǵ� ���� - ������ �ޱ�.
	int nSize = 0 ;
	nSize = sizeof(MSG_CR_IDNAME) ;

	// ���� �� ��ŭ ����.
	USERINFO* pInfo ;
	for(BYTE count = 0 ; count < pmsg->byCount ; ++count)
	{
		// ������ �˻�.
		pInfo = NULL ;
		pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwUser[count]) ;

		if(!pInfo) continue ;

		// �޽����� ����.
		MSG_CR_IDNAME sendMsg ;
		memset(&sendMsg, 0, nSize) ;
		memcpy(&sendMsg, pmsg, nSize) ;

		sendMsg.Protocol = MP_CHATROOM_CHANGE_OWNER_NOTICE ;

		// �޽����� �������� ����.
		g_Network.Send2User(pInfo->dwConnectionIndex, (char*)&sendMsg, nSize) ;
	}

	// �ٸ� ������Ʈ���� ����.
	if(pmsg->Protocol == MP_CHATROOM_CHANGE_OWNER_NOTICE)
	{
		// �޽����� ����.
		MSG_CR_IDNAME sendMsg ;
		memset(&sendMsg, 0, nSize) ;
		memcpy(&sendMsg, pmsg, nSize) ;

		// �ٸ� ������Ʈ���� ���� �� �������� ����.
		sendMsg.Protocol = MP_CHATROOM_CHANGE_OWNER_NOTICE_OTHER_AGENT ;

		// �ٸ� ������Ʈ�� �޽����� ����.
		g_Network.Send2AgentExceptThis((char*)&sendMsg, nSize) ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : Kick_Guest_Ack
//		 : 
//	DESC : ���� ���� ���� ������� ���ϴ� ó��.
//		 : 
//		 : 1. ���� �˻�.
//		 : 2. �޽��� ����.
//		 : 3. �ٸ� ������Ʈ���� ���� ������� ����.
//		 : 
//  DATE : APRIL 14, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Kick_Guest_Ack(DWORD dwIndex, char* pMsg, DWORD dwLength)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_CR_KICK_ACK* pmsg = NULL ;
	pmsg = (MSG_CR_KICK_ACK*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}

	// ���� ���Ǵ� ���� - ������ �ޱ�.
	int nSize = 0 ;
	nSize = sizeof(MSG_CR_KICK_ACK) ;

	// ������ �˻�.
	USERINFO* pInfo = NULL ;
	pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwKickPlayer) ;

	if(pInfo)
	{
		// �޽����� ����.
		MSG_CR_KICK_ACK sendMsg ;
		memset(&sendMsg, 0, nSize) ;
		memcpy(&sendMsg, pmsg, nSize) ;

		sendMsg.Protocol = MP_CHATROOM_KICK_GUEST_ACK ;

		// �޽����� �������� ����.
		g_Network.Send2User(pInfo->dwConnectionIndex, (char*)&sendMsg, nSize) ;
	}
	else
	{
		// �ٸ� ������Ʈ���� ����.
		if(pmsg->Protocol == MP_CHATROOM_KICK_GUEST_ACK)
		{
			// �޽����� ����.
			MSG_CR_KICK_ACK sendMsg ;
			memset(&sendMsg, 0, nSize) ;
			memcpy(&sendMsg, pmsg, nSize) ;

			// �ٸ� ������Ʈ���� ���� �� �������� ����.
			sendMsg.Protocol = MP_CHATROOM_KICK_GUEST_ACK_OTHER_AGENT ;

			// �ٸ� ������Ʈ�� �޽����� ����.
			g_Network.Send2AgentExceptThis((char*)&sendMsg, nSize) ;
		}
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : Kick_Guest_Notice
//		 : 
//	DESC : ���� ���� ���� ó���� �ϴ� �Լ�.
//		 : 
//		 : 1. ���� �˻�.
//		 : 2. �޽��� ����.
//		 : 3. �ٸ� ������Ʈ���� ���� ������� ����.
//		 : 
//  DATE : APRIL 14, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Kick_Guest_Notice(DWORD dwIndex, char* pMsg, DWORD dwLength)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_CR_IDNAME* pmsg = NULL ;
	pmsg = (MSG_CR_IDNAME*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}

	// ���� ���Ǵ� ���� - ������ �ޱ�.
	int nSize = 0 ;
	nSize = sizeof(MSG_CR_IDNAME) ;

	// ���� �� ��ŭ ����.
	USERINFO* pInfo ;
	for(BYTE count = 0 ; count < pmsg->byCount ; ++count)
	{
		// ������ �˻�.
		pInfo = NULL ;
		pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwUser[count]) ;

		if(!pInfo) continue ;

		// �޽����� ����.
		MSG_CR_IDNAME sendMsg ;
		memset(&sendMsg, 0, nSize) ;
		memcpy(&sendMsg, pmsg, nSize) ;

		sendMsg.Protocol = MP_CHATROOM_KICK_GUEST_NOTICE ;

		// �޽����� �������� ����.
		g_Network.Send2User(pInfo->dwConnectionIndex, (char*)&sendMsg, nSize) ;
	}

	// �ٸ� ������Ʈ���� ����.
	if(pmsg->Protocol == MP_CHATROOM_KICK_GUEST_NOTICE)
	{
		// �޽����� ����.
		MSG_CR_IDNAME sendMsg ;
		memset(&sendMsg, 0, nSize) ;
		memcpy(&sendMsg, pmsg, nSize) ;

		// �ٸ� ������Ʈ���� ���� �� �������� ����.
		sendMsg.Protocol = MP_CHATROOM_KICK_GUEST_NOTICE_OTHER_AGENT ;

		// �ٸ� ������Ʈ�� �޽����� ����.
		g_Network.Send2AgentExceptThis((char*)&sendMsg, nSize) ;
	}
}





////-------------------------------------------------------------------------------------------------
////	NAME : ChatMsg_Normal_Notice
////		 : 
////	DESC : ä�ù� �� �޽��� ���� ó���� �ϴ� �Լ�.
////		 : 
////	DESC : ���� ���� ���� ó���� �ϴ� �Լ�.
////		 : 
////		 : 1. ���� �˻�.
////		 : 2. �޽��� ����.
////		 : 3. �ٸ� ������Ʈ���� ���� ������� ����.
////		 : 
////  DATE : APRIL 14, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomMgr::ChatMsg_Normal_Notice(DWORD dwIndex, char* pMsg, DWORD dwLength)
//{
//	// �Լ� �Ķ���� üũ.
//	ASSERT(pMsg) ;
//
//	if(!pMsg)
//	{
//		Throw_Error(err_IMP, __FUNCTION__) ;
//		return ;
//	}
//
//	// ���� �޽��� ��ȯ.
//	MSG_CR_MSG_RESULT* pmsg = NULL ;
//	pmsg = (MSG_CR_MSG_RESULT*)pMsg ;
//
//	ASSERT(pmsg) ;
//
//	if(!pmsg)
//	{
//		Throw_Error(err_FCMTO, __FUNCTION__) ;
//		return ;
//	}
//
//	// ���� ���Ǵ� ���� - ������ �ޱ�.
//	int nSize = 0 ;
//	nSize = sizeof(MSG_CR_MSG_RESULT) ;
//
//	// ���� �� ��ŭ ����.
//	USERINFO* pInfo ;
//	for(BYTE count = 0 ; count < pmsg->byCount ; ++count)
//	{
//		// ������ �˻�.
//		pInfo = NULL ;
//		pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwUser[count]) ;
//
//		if(!pInfo) continue ;
//
//		// �޽����� ����.
//		MSG_CR_MSG_RESULT sendMsg ;
//		memset(&sendMsg, 0, nSize) ;
//		memcpy(&sendMsg, pmsg, nSize) ;
//
//		// �޽����� �������� ����.
//		g_Network.Send2User(pInfo->dwConnectionIndex, (char*)&sendMsg, nSize) ;
//	}
//
//	// �ٸ� ������Ʈ���� ����.
//	if(pmsg->Protocol == MP_CHATROOM_CHATMSG_NORMAL_NOTICE)
//	{
//		// �޽����� ����.
//		MSG_CR_MSG_RESULT sendMsg ;
//		memset(&sendMsg, 0, nSize) ;
//		memcpy(&sendMsg, pmsg, nSize) ;
//
//		// �ٸ� ������Ʈ���� ���� �� �������� ����.
//		sendMsg.Protocol = MP_CHATROOM_CHATMSG_NORMAL_NOTICE_OTHER_AGENT ;
//
//		// �ٸ� ������Ʈ�� �޽����� ����.
//		g_Network.Send2AgentExceptThis((char*)&sendMsg, nSize) ;
//	}
//}





//-------------------------------------------------------------------------------------------------
//	NAME : Request_Friend_Ack
//		 : 
//	DESC : ģ�� ��û üũ ���� ó���� �ϴ� �Լ�.
//		 : 
//		 : 1. ���� ã��.
//		 : 2. ģ�� �߰� ó��.
//		 : 
//  DATE : APRIL 14, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Request_Friend_Ack(DWORD dwIndex, char* pMsg, DWORD dwLength)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_NAME* pmsg = NULL ;
	pmsg = (MSG_NAME*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}

	// ���� ã��.
	USERINFO * userinfo = NULL ;
	userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID) ;

	if(!userinfo) return ;
	
	// ģ�� �߰� ó��.
	char buf[MAX_NAME_LENGTH+1] = {0, } ;
	SafeStrCpy( buf, pmsg->Name, MAX_NAME_LENGTH+1 ) ;

	//�������� üũ "'"��...
	if( FILTERTABLE->IsInvalidCharInclude( (unsigned char*) buf ) ) return ;
	
	FriendGetUserIDXbyName(pmsg->dwObjectID, buf) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Error
//	DESC : ä�ù� ��, ���� �޽��� ���� �α׸� ����� �Լ�.
//  DATE : APRIL 14, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Throw_Error(BYTE errType, char* szCaption)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(szCaption) ;

	if(!szCaption || strlen(szCaption) <= 1)
	{
#ifdef _USE_ERR_MSGBOX_
		MessageBox( NULL, "Invalid Caption!!", "Throw_Error", MB_OK) ;
#else
		char tempStr[128] = {0, } ;

		SafeStrCpy(tempStr, __FUNCTION__, 128) ;
		strcat(tempStr, " - ") ;
		strcat(tempStr, "Invalid Caption!!") ;
		WriteLog(tempStr) ;
#endif //_USE_ERR_MSGBOX_
		return ;
	}

	// ���� Ÿ�� Ȯ��.
	char tempErr[128] = {0, } ;
	switch(errType)
	{
	case err_IMP :		SafeStrCpy(tempErr, "Invalid a message parameter!!", 128) ;					break ;
	case err_FCMTB :	SafeStrCpy(tempErr, "Failed to convert a message to base!!", 128) ;			break ;
	case err_FCMTO :	SafeStrCpy(tempErr, "Failed to convert a message to original!!", 128) ;		break ;
	case err_FRUI : 	SafeStrCpy(tempErr, "Failed to receive a user info!!", 128) ;				break ;
	default : break ;
	}

	// ���� ���/�α�.
#ifdef _USE_ERR_MSGBOX_
	MessageBox( NULL, tempErr, szCaption, MB_OK) ;
#else
	char tempStr[128] = {0, } ;

	SafeStrCpy(tempStr, szCaption, 128) ;
	strcat(tempStr, " - ") ;
	strcat(tempStr, tempErr) ;
	WriteLog(tempStr) ;
#endif //_USE_ERR_MSGBOX_
}





//-------------------------------------------------------------------------------------------------
//	NAME : Throw_Error
//	DESC : ä�ù� ��, ���� �޽��� ���� �α׸� ����� �Լ�.
//  DATE : APRIL 14, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Throw_Error(char* szErr, char* szCaption)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(szCaption) ;

	if(!szErr || strlen(szErr) <= 1)
	{
#ifdef _USE_ERR_MSGBOX_
		MessageBox( NULL, "Invalid err message!!", "Throw_Error", MB_OK) ;
#else
		char tempStr[128] = {0, } ;

		SafeStrCpy(tempStr, __FUNCTION__, 128) ;
		strcat(tempStr, " - ") ;
		strcat(tempStr, "Invalid err message!!") ;
		WriteLog(tempStr) ;
#endif //_USE_ERR_MSGBOX_
		return ;
	}

	if(!szCaption || strlen(szCaption) <= 1)
	{
#ifdef _USE_ERR_MSGBOX_
		MessageBox( NULL, "Invalid Caption!!", "Throw_Error", MB_OK) ;
#else
		char tempStr[128] = {0, } ;

		SafeStrCpy(tempStr, __FUNCTION__, 128) ;
		strcat(tempStr, " - ") ;
		strcat(tempStr, "Invalid Caption!!") ;
		WriteLog(tempStr) ;
#endif //_USE_ERR_MSGBOX_
		return ;
	}

	// ���� ���/�α�.
#ifdef _USE_ERR_MSGBOX_
	MessageBox( NULL, szErr, szCaption, MB_OK) ;
#else
	char tempStr[128] = {0, } ;

	SafeStrCpy(tempStr, szCaption, 128) ;
	strcat(tempStr, " - ") ;
	strcat(tempStr, szErr) ;
	WriteLog(tempStr) ;
#endif //_USE_ERR_MSGBOX_
}





//-------------------------------------------------------------------------------------------------
//	NAME : WriteLog
//	DESC : ä�ù� ��, ���� �޽��� ���� �α׸� ����� �Լ�.
//  DATE : APRIL 14, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::WriteLog(char* pMsg)
{
	SYSTEMTIME time ;
	GetLocalTime(&time) ;

	TCHAR szTime[_MAX_PATH] = {0, } ;
	sprintf(szTime, "%04d-%02d-%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond) ;

	FILE *fp = fopen("Log/Agent-ChatRoomErr.log", "a+");
	if (fp)
	{
		fprintf(fp, "%s [%s]\n", pMsg,  szTime);
		fclose(fp);
	}
}





//=================================================================================================
//	FILE		: CChatRoomMgr.cpp
//	DESC		: ä�ù� �Ŵ����� ����cpp.
//	DATE		: APRIL 3, 2008 LYW
//=================================================================================================





//-------------------------------------------------------------------------------------------------
//		Include part.
//-------------------------------------------------------------------------------------------------
#include "stdafx.h"																						// An include file for standard system include files

#include "../WindowIDEnum.h"

#include "./Interface/cWindowManager.h"																	// Include header file of window manager.

#include "./Interface/cListDialog.h"
//#include "./Interface/cTextArea.h"
#include "./Interface/cEditBox.h"

#include "./cMsgBox.h"																					// Include header file of message box.
#include "./GameIn.h"

#include "./ObjectManager.h"
#include "./UserInfoManager.h"
#include "./FilteringTable.h"

#include "./ChatManager.h"
#include "./ChatRoomMgr.h"

#include "./ChatRoomCreateDlg.h"
#include "./ChatRoomDlg.h"
#include "./ChatRoomGuestListDlg.h"
#include "./ChatRoomJoinDlg.h"
#include "./ChatRoomMainDlg.h"
#include "./ChatRoomOptionDlg.h"





//-------------------------------------------------------------------------------------------------
//		Define some macroinstruction.
//-------------------------------------------------------------------------------------------------
GLOBALTON(CChatRoomMgr)





//-------------------------------------------------------------------------------------------------
//	NAME : CChatRoomMgr
//	DESC : ������ �Լ�.
//  DATE : APRIL 3, 2008 LYW
//-------------------------------------------------------------------------------------------------
CChatRoomMgr::CChatRoomMgr(void)
{
	// �ʱ�ȭ ó��.
	m_byChatting	= e_State_Away_From_Chatroom ;

	memset(&m_MyRoom, 0, sizeof(ST_CR_ROOM_CLT)) ;

	m_pCreateDlg	= NULL ;
	m_pChatRoomDlg	= NULL ;
	m_pGuestListDlg = NULL ;
	m_pJoinDlg		= NULL ;
	m_pMainDlg		= NULL ;
	m_pOptionDlg	= NULL ;

	m_BackupList.RemoveAll() ;

	m_dwLastMyChat = 0 ;

	m_dwLastMyComTime = 0 ;

	// ���� ���� �ʱ�ȭ.
	for(BYTE count = 0 ; count < MAX_USER_PER_ROOM ; ++count)
	{
		m_dwFreezedID[count] = 0 ;
	}

	m_byRequestFriend = FALSE ;

	// �ɼ� ������ ���� ������ �ʱ�ȭ.
	m_byOptSavedMode			= e_RM_Open ;										
	memset(m_szOptSavedCode, 0, sizeof(SECRET_CODE_SIZE+1)) ;

	m_byOptSavedGuestCount	= e_GC_20 ;
	m_byOptSavedRoomType		= e_RT_Party ;

	memset(m_szOptSavedTitleBuff, 0, 128) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : ~CChatRoomMgr
//	DESC : �Ҹ��� �Լ�.
//  DATE : APRIL 3, 2008 LYW
//-------------------------------------------------------------------------------------------------
CChatRoomMgr::~CChatRoomMgr(void)
{
	ReleaseGuestList() ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : ToggleChatRoomMainDlg
//		 : 
//	DESC : ä�ù� ���â ��/�ݱ� �Լ�.
//		 : 
//		 : 1. ä�ù� ��������� ��Ȳ�̸�, HERO�� ����üũ.
//		 : 2. �κ� ��� �� ���°� �ƴϸ�, �κ� ����� ��û.
//		 : 3. ��� �� ���¸�, ä�ù� ����Ʈ ��û.
//		 : 
//  DATE : APRIL 3, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::ToggleChatRoomMainDlg()
{
	// ���� ��Ʈ�� üũ.
	ASSERT(m_pMainDlg) ;

	if(!m_pMainDlg)
	{
		Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}

	// ä�ù� ��� ��/�ݱ�.
	if(m_pMainDlg->IsActive())
	{
		m_pMainDlg->SetActive(FALSE) ;
	}
	else
	{
		if(CHATROOMMGR->Get_ChatRoomState() == e_State_Away_From_Chatroom)
		{
			// ������ Ŭ���̾�Ʈ�� �ٿ� �Ǿ��� ���, 
			// ���� �ʿ� ĳ������ ����� ������ �ٽ� �ϴ� ��û�� �Ѵ�.
			MSGBASE msg ;
			memset(&msg, 0, sizeof(MSGBASE)) ;

			msg.Category	= MP_CHATROOM ;
			msg.Protocol	= MP_CHATROOM_FORCE_ADD_USER_SYN ;
			msg.dwObjectID	= HEROID ;

            NETWORK->Send( &msg, sizeof(MSGBASE) ) ;
		}
		else
		{
			// ���� �ð� üũ.
			if(!IsEnableExecuteCom())
			{
				WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1517)) ;
				return ;
			}
			// ������, ä�ù� ��� �޾ƿ���.
			//m_pMainDlg->SetActive(TRUE) ;
			
			MSG_BYTE2 msg ;
			memset(&msg, 0, sizeof(MSG_BYTE2)) ;

			msg.Category		= MP_CHATROOM ;
			msg.Protocol		= MP_CHATROOM_ROOM_SYN ;
			msg.dwObjectID		= HEROID ;

			msg.bData1			= e_RTM_AllLooK ;
			msg.bData2			= 0 ;

			NETWORK->Send( &msg, sizeof(MSG_BYTE2) ) ;

			// �ð� ������Ʈ.
			CHATROOMMGR->UpdateMyLastComTime() ;
		}

		m_pMainDlg->SetActive(TRUE) ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : ReleaseGuestList
//	DESC : Player�� ������ ���� ������ ������ ���� ����Ʈ�� ���� �Ѵ�.
//  DATE : APRIL 23, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::ReleaseGuestList()
{
	ST_CR_USER* pInfo ;
	PTRLISTPOS pos = NULL ;
	pos = m_GuestList.GetHeadPosition() ;

	while(pos)
	{
		pInfo = NULL ;
		pInfo = (ST_CR_USER*)m_GuestList.GetNext(pos) ;

		if(!pInfo) continue ;
		delete pInfo ;
	}

	m_GuestList.RemoveAll() ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : SetOption
//	DESC : ���� �� �ɼ��� �����ϴ� �Լ�.
//  DATE : APRIL 30, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::SetOption(ST_CR_ROOM_SRV* pInfo)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pInfo) ;

	if(!pInfo)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}



	// ���� ��Ʈ�� üũ.
	ASSERT(m_pOptionDlg) ;

	if(!m_pOptionDlg)
	{
		Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// �ɼ� ����.
	m_byOptSavedMode		= pInfo->bySecretMode ;

	SafeStrCpy(m_szOptSavedCode, pInfo->code, SECRET_CODE_SIZE) ;

	m_byOptSavedGuestCount	= pInfo->byTotalGuestCount ;
	m_byOptSavedRoomType	= pInfo->byRoomType ;

	SafeStrCpy(m_szOptSavedTitleBuff, pInfo->title, 128) ;

	// ����� �ɼ� ����.
	m_pOptionDlg->SetStoredDlg() ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : NetworkMsgParser
//	DESC : ��Ʈ��ũ �޽��� �з� �Լ�.
//  DATE : APRIL 3, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::NetworkMsgParser(void* pMsg)
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
	case MP_CHATROOM_ADD_USER_ACK :					Add_User_Ack(pMsg) ;					break ;

	case MP_CHATROOM_CHANGE_OPTION_NACK :			Change_Option_Nack(pMsg) ;				break ;
	case MP_CHATROOM_CHANGE_OPTION_NOTICE :			Change_Option_Notice(pMsg) ;			break ;

	case MP_CHATROOM_CHANGE_OWNER_NACK :			Change_Owner_Nack(pMsg) ;				break ;
	case MP_CHATROOM_CHANGE_OWNER_NOTICE :			Change_Owner_Notice(pMsg) ;				break ;

	case MP_CHATROOM_CHATMSG_NORMAL_NACK :			Chatmsg_Normal_Nack(pMsg) ;				break ;
	case MP_CHATROOM_CHATMSG_NORMAL_NOTICE :		Chatmsg_Normal_Notice(pMsg) ;			break ;

	case MP_CHATROOM_CREATE_ROOM_ACK :				Create_Room_Ack(pMsg) ;					break ;
	case MP_CHATROOM_CREATE_ROOM_NACK :				Create_Room_Nack(pMsg) ;				break ;

	case MP_CHATROOM_JOIN_ROOM_ACK :				Join_Room_Ack(pMsg) ;					break ;
	case MP_CHATROOM_JOIN_ROOM_NACK :				Join_Room_Nack(pMsg) ;					break ;
	case MP_CHATROOM_JOIN_ROOM_NOTICE :				Join_Room_Notice(pMsg) ;				break ;

	case MP_CHATROOM_KICK_GUEST_ACK :				Kick_Guest_Ack(pMsg) ;					break ;
	case MP_CHATROOM_KICK_GUEST_NACK :				Kick_Guest_Nack(pMsg) ;					break ;
	case MP_CHATROOM_KICK_GUEST_NOTICE :			Kick_Guest_Notice(pMsg) ;				break ;

	case MP_CHATROOM_OUT_ROOM_ACK :					Out_Room_Ack(pMsg) ;					break ;
	case MP_CHATROOM_OUT_ROOM_NACK :				Out_Room_Nack(pMsg) ;					break ;
	case MP_CHATROOM_OUT_ROOM_EMPTYROOM :			Out_Room_EmptyRoom(pMsg) ;				break ;
	case MP_CHATROOM_OUT_ROOM_LAST_MAN :			Out_Room_Last_Man(pMsg) ;				break ;
	case MP_CHATROOM_OUT_ROOM_NOTICE :				Out_Room_Notice(pMsg) ;					break ;
	case MP_CHATROOM_OUT_ROOM_CHANGE_OWNER_NOTICE :	Out_Room_Change_Owner_Notice(pMsg) ;	break ;

	case MP_CHATROOM_REQUEST_FRIEND_NACK :			Request_Friend_Nack(pMsg) ;				break ;

	case MP_CHATROOM_ROOMLIST_ACK :					RoomList_Ack(pMsg) ;					break ;
	case MP_CHATROOM_ROOMLIST_NACK :				RoomList_Nack(pMsg) ;					break ;

	case MP_CHATROOM_SEARCH_FOR_NAME_ACK :			SearchName_Ack(pMsg) ;					break ;
	case MP_CHATROOM_SEARCH_FOR_NAME_NACK :			SearchName_Nack(pMsg) ;					break ;

	case MP_CHATROOM_SEARCH_FOR_TITLE_ACK :			SearchTitle_Ack(pMsg) ;					break ;
	case MP_CHATROOM_SEARCH_FOR_TITLE_NACK :		SearchTitle_Nack(pMsg) ;				break ;

	case MP_CHATROOM_UPDATEINFO_CREATED_ROOM :		UpdateInfo_Created_Room(pMsg) ;			break ;
	case MP_CHATROOM_UPDATEINFO_DELETED_ROOM :		UpdateInfo_Deleted_Room(pMsg) ;			break ;
	case MP_CHATROOM_UPDATEINFO_SECRETMODE :		UpdateInfo_SecretMode(pMsg) ;			break ;
	case MP_CHATROOM_UPDATEINFO_ROOMTYPE :			UpdateInfo_RoomType(pMsg) ;				break ;
	case MP_CHATROOM_UPDATEINFO_TITLE :				UpdateInfo_Title(pMsg) ;				break ;
	case MP_CHATROOM_UPDATEINFO_CUR_GUESTCOUNT :	UpdateInfo_Cur_GuestCount(pMsg) ;		break ;
	case MP_CHATROOM_UPDATEINFO_TOTAL_GUESTCOUNT :	UpdateInfo_Total_GuestCount(pMsg) ;		break ;

	default : break ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : Add_User_Ack
//	DESC : HERO�� ���¸� �κ� �ִ� ���·� ����.
//  DATE : APRIL 16, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Add_User_Ack(void* pMsg)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}
	
	// HERO ���� ����.
	Set_ChatRoomState(e_State_In_Lobby) ;
}






//-------------------------------------------------------------------------------------------------
//	NAME : Change_Option_Nack
//	DESC : �ɼ� ���� ���� ó���� �ϴ� �Լ�.
//  DATE : APRIL 16, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Change_Option_Nack(void* pMsg) 
{
	// ���� �޽��� ����.
	// 0 = err_You_Are_Not_In_ChatRoom,
	// 1 = err_CanNot_Found_Room,
	// 2 = err_You_Are_Not_Owner,

	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_BYTE* pmsg = NULL ;
	pmsg = (MSG_BYTE*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}



	// ���� �޽��� Ȯ��.
	WORD wErrMsgNum = 0 ;
	switch(pmsg->bData)
	{
	case 0 : wErrMsgNum = 1498 ; break ;
	case 1 : wErrMsgNum = 1507 ; break ;
	case 2 : wErrMsgNum = 1487 ; break ;
	default : return ;
	}

	// ä�ù濡 ���� �޽��� ���.
	AddChatRoomMsg(CHATMGR->GetChatMsg(wErrMsgNum), e_ChatMsg_System) ;
}






//-------------------------------------------------------------------------------------------------
//	NAME : Change_Option_Notice
//		 : 
//	DESC : �ɼ� ������ �������� �� ó���ϴ� �Լ�.
//		 : 
//		 : 1. CHATMGR�� HERO ä�ù� ���� ������Ʈ.
//		 : 2. �ɼ�â�� �ٲ� �ɼ� ����.
//		 : 3. ä�ù� ����Ʈ ������Ʈ.
//		 : 4. ä�ù濡 �ɼ� ���� ���� ����.
//		 : 
//  DATE : APRIL 16, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Change_Option_Notice(void* pMsg)
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

	// ���� ��Ʈ�� üũ.
	ASSERT(m_pMainDlg) ;
	ASSERT(m_pChatRoomDlg) ;
	ASSERT(m_pOptionDlg) ;

	if(!m_pMainDlg || !m_pChatRoomDlg || !m_pOptionDlg)
	{
		Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// ä�ù� ���� �ޱ�.
	m_MyRoom = pmsg->room ;

	// ä�ù� ����Ʈ ������Ʈ.
	ST_CR_ROOM_CLT myRoom ;
	memset(&myRoom, 0, sizeof(ST_CR_ROOM_CLT)) ;

	myRoom.byIdx				= m_MyRoom.byIdx ;
	myRoom.bySecretMode			= m_MyRoom.bySecretMode ;
	myRoom.byRoomType			= m_MyRoom.byRoomType ;
	myRoom.byCurGuestCount		= m_MyRoom.byCurGuestCount ;
	myRoom.byTotalGuestCount	= m_MyRoom.byTotalGuestCount ;

	SafeStrCpy(myRoom.title, m_MyRoom.title, TITLE_SIZE) ;

	m_pMainDlg->Update_RoomInfo(&myRoom) ;

	// �ɼ�â�� �ٲ� �ɼ� ����.
	//if(!m_pOptionDlg->SetOption(&pmsg->room))
	//{
	//	Throw_Error("Failed to apply option!!", __FUNCTION__) ;
	//	return ;
	//}
	SetOption(&pmsg->room) ;

	// �ɼ� ���� ����.
	m_pChatRoomDlg->SetRoomTitle(&myRoom) ;

	char tempMsg[128] = {0, } ;
	strcat(tempMsg, "[") ;
	strcat(tempMsg, CHATMGR->GetChatMsg(1494)) ;
	strcat(tempMsg, "]") ;
	m_pChatRoomDlg->AddMsg(tempMsg, e_ChatMsg_System) ;
}






//-------------------------------------------------------------------------------------------------
//	NAME : Change_Owner_Nack
//	DESC : ���� ���� ���� ó���� �ϴ� �Լ�.
//  DATE : APRIL 16, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Change_Owner_Nack(void* pMsg)
{
	// ���� �޽��� ����.
	// 0 = err_Invalid_Current_Owner,
	// 1 = err_Invalid_Next_Owner,
	// 2 = err_CanNot_Found_RoomInfo,
	// 3 = err_You_Are_Not_Owner,
	// 4 = err_You_Alone,

	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_BYTE* pmsg = NULL ;
	pmsg = (MSG_BYTE*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}



	// ���� �޽��� Ȯ��.
	WORD wErrMsgNum = 0 ;
	switch(pmsg->bData)
	{
	case 0 : wErrMsgNum = 1501 ; break ;
	case 1 : wErrMsgNum = 1502 ; break ;
	case 2 : wErrMsgNum = 1503 ; break ;
	case 3 : wErrMsgNum = 1487 ; break ;
	case 4 : wErrMsgNum = 1489 ; break ;
	default : return ;
	}

	// ä�ù濡 ���� �޽��� ���.
	AddChatRoomMsg(CHATMGR->GetChatMsg(wErrMsgNum), e_ChatMsg_System) ;
}






//-------------------------------------------------------------------------------------------------
//	NAME : Change_Owner_Notice
//	DESC : ���� ���� ó���� �ϴ� �Լ�.
//  DATE : APRIL 16, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Change_Owner_Notice(void* pMsg) 
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

	// ���� ��Ʈ�� üũ.
	ASSERT(m_pChatRoomDlg) ;
	ASSERT(m_pGuestListDlg) ;

	if(!m_pChatRoomDlg || !m_pGuestListDlg)
	{
		Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// ���� �ε��� ������Ʈ.
	m_MyRoom.dwOwnerIdx = pmsg->dwID ;

	// ������ �ε��� ������Ʈ.
	for(BYTE count = 0 ; count < MAX_USER_PER_ROOM ; ++count)
	{
		m_MyRoom.dwUser[count] = pmsg->dwUser[count] ;
	}

	// ������ �� ������Ʈ.
	m_MyRoom.byCurGuestCount = pmsg->byCount ;



	// ���� ���� ���� ���.
	char NoticeMsg[128] = {0, } ;
	strcat(NoticeMsg, "[") ;
	strcat(NoticeMsg, "<") ;
	strcat(NoticeMsg, pmsg->name) ;
	strcat(NoticeMsg, ">") ;
	strcat(NoticeMsg, CHATMGR->GetChatMsg(1495)) ;
	strcat(NoticeMsg, "]") ;
	
	m_pChatRoomDlg->AddMsg(NoticeMsg, e_ChatMsg_System) ;

	m_pGuestListDlg->SetClassMark() ;
}






//-------------------------------------------------------------------------------------------------
//	NAME : Chatmsg_Normal_Nack
//	DESC : ä�ù� �� ä�� ���� ó���� �ϴ� �Լ�.
//  DATE : APRIL 16, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Chatmsg_Normal_Nack(void* pMsg) 
{
	// ���� �޽��� ����.
	// 0 = err_Invalid_Room_Info,
	// 1 = err_Invalid_Sender_Info,

	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_BYTE* pmsg = NULL ;
	pmsg = (MSG_BYTE*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� Ȯ��.
	WORD wErrMsgNum = 0 ;
	switch(pmsg->bData)
	{
	case 0 : wErrMsgNum = 1503 ; break ;
	case 1 : wErrMsgNum = 1514 ; break ;
	default : return ;
	}

	// ä�ù濡 ���� �޽��� ���.
	AddChatRoomMsg(CHATMGR->GetChatMsg(wErrMsgNum), e_ChatMsg_System) ;
}






//-------------------------------------------------------------------------------------------------
//	NAME : Chatmsg_Normal_Notice
//	DESC : ä�ù� ��, �޽��� ���� ���� ó���� �ϴ� �Լ�.
//  DATE : APRIL 16, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Chatmsg_Normal_Notice(void* pMsg) 
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_CR_MSG* pmsg = NULL ;
	pmsg = (MSG_CR_MSG*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}

	// ���� ��Ʈ�� üũ.
	ASSERT(m_pGuestListDlg) ;
	ASSERT(m_pChatRoomDlg) ;

	if(!m_pGuestListDlg || !m_pChatRoomDlg)
	{
		Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}

	// ���� �� ����̸� return ó��.
	if(IsFreezed(pmsg->dwObjectID)) return ;

	// �޽��� Ÿ���� ����.
	BYTE byMsgType = e_ChatMsg_OtherMsg ;

	if(pmsg->dwObjectID == HEROID)
	{
		byMsgType = e_ChatMsg_MyMsg ;
	}

	// �޽��� ���� ��, ä�ù濡 ���.
	char tempMsg[(MAX_CHAT_LENGTH+1) + (MAX_NAME_LENGTH+1)] = {0,} ;

	strcat(tempMsg, "[") ;
	strcat(tempMsg, pmsg->name) ;
	strcat(tempMsg, "]") ;
	strcat(tempMsg, ": ") ;
	strcat(tempMsg, pmsg->Msg) ;

	m_pChatRoomDlg->AddMsg(tempMsg, byMsgType) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Create_Room_Ack
//	DESC : ä�ù� ���� ������ ó���ϴ� �Լ�.
//  DATE : APRIL 16, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Create_Room_Ack(void* pMsg)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_CR_ROOMINFO* pmsg = NULL ;
	pmsg = (MSG_CR_ROOMINFO*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}

	// ���� ��Ʈ�� üũ.
	ASSERT(m_pChatRoomDlg) ;
	ASSERT(m_pGuestListDlg) ;
	ASSERT(m_pMainDlg) ;
	ASSERT(m_pOptionDlg) ;

	if(!m_pChatRoomDlg || !m_pGuestListDlg || !m_pMainDlg || !m_pOptionDlg)
	{
		Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// ������ ����.
	m_MyRoom = pmsg->room ;

	ST_CR_ROOM_CLT myRoom ;
	memset(&myRoom, 0, sizeof(ST_CR_ROOM_CLT)) ;

	myRoom.byIdx				= pmsg->room.byIdx ;
	myRoom.bySecretMode			= pmsg->room.bySecretMode ;
	myRoom.byRoomType			= pmsg->room.byRoomType ;
	myRoom.byCurGuestCount		= pmsg->room.byCurGuestCount ;
	myRoom.byTotalGuestCount	= pmsg->room.byTotalGuestCount ;

	SafeStrCpy(myRoom.title, pmsg->room.title, TITLE_SIZE) ;

	// ä�ù� ����Ʈ�� ������ �߰�.
	// ä�ù� ����Ʈ�� �����ִٸ�, �� �з��� ������ �߰�.
	/*if(m_pMainDlg->IsActive())
	{
		BYTE byType = 0 ;
		byType = m_pMainDlg->GetCurRoomType() ;

		if(byType == pmsg->room.byRoomType || byType == e_RTM_AllLooK)
		{
			m_pMainDlg->Add_Room(&myRoom) ;
		}
	}*/

	m_pMainDlg->Add_Room(&myRoom) ;

	// ä�ù� �ɼ� ����.
	//if(!m_pOptionDlg->SetOption(&m_MyRoom))
	//{
	//	Throw_Error("Failed to setting room option!!", __FUNCTION__) ;
	//	return ;
	//}
	SetOption(&m_MyRoom) ;

	// ������ ����Ʈ ����.
	m_pGuestListDlg->InitGuestList(pmsg->user, 1) ;

	// ä��â Ȱ��ȭ.
	m_pChatRoomDlg->SetDlgToDefault() ;
	m_pChatRoomDlg->SetRoomTitle(&myRoom) ;
	m_pChatRoomDlg->AddCommonMsg(e_ChatMsg_StepIn, HERO->GetObjectName()) ;
	m_pChatRoomDlg->SetActive(TRUE) ;

	// HERO ä�� ���·� ����.
	Set_ChatRoomState(e_State_In_ChatRoom) ;

	// ���� ��� �ʱ�ȭ.
	ClearFreeze() ;
}






//-------------------------------------------------------------------------------------------------
//	NAME : Create_Room_Nack
//	DESC : ä�ù� ���� ���� ó���� �ϴ� �Լ�.
//  DATE : APRIL 16, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Create_Room_Nack(void* pMsg) 
{
	// ���� �޽��� ����.
	// 0 = err_Already_Chatting,
	// 1 = err_RoomCount_Over,
	// 2 = err_User_Is_Not_In_Lobby,
	// 3 = err_Registed_But_Not_Found_Room,
	// 4 = err_Registed_But_Not_Found_User,

	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_BYTE* pmsg = NULL ;
	pmsg = (MSG_BYTE*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}



	// ���� �޽��� Ȯ��.
	WORD wErrMsgNum = 0 ;
	switch(pmsg->bData)
	{
	case 0 : wErrMsgNum = 1505 ; break ;
	case 1 : wErrMsgNum = 1506 ; break ;
	case 2 : wErrMsgNum = 1514 ; break ;
	case 3 : wErrMsgNum = 1507 ; break ;
	case 4 : wErrMsgNum = 1508 ; break ;
	default : return ;
	}

	// ä�ù濡 ���� �޽��� ���.
	WINDOWMGR->MsgBox( MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(wErrMsgNum)) ;
}






//-------------------------------------------------------------------------------------------------
//	NAME : Join_Room_Ack
//	DESC : ä�ù� ���� ���� ó���� �ϴ� �Լ�.
//  DATE : APRIL 16, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Join_Room_Ack(void* pMsg) 
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_CR_ROOMINFO* pmsg = NULL ;
	pmsg = (MSG_CR_ROOMINFO*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}

	// ���� ��Ʈ�� üũ.
	ASSERT(m_pChatRoomDlg) ;
	ASSERT(m_pGuestListDlg) ;
	ASSERT(m_pMainDlg) ;
	ASSERT(m_pOptionDlg) ;

	if(!m_pChatRoomDlg || !m_pGuestListDlg || !m_pMainDlg || !m_pOptionDlg)
	{
		Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// ������ ����.
	m_MyRoom = pmsg->room ;

	ST_CR_ROOM_CLT myRoom ;
	memset(&myRoom, 0, sizeof(ST_CR_ROOM_CLT)) ;

	myRoom.byIdx				= pmsg->room.byIdx ;
	myRoom.bySecretMode			= pmsg->room.bySecretMode ;
	myRoom.byRoomType			= pmsg->room.byRoomType ;
	myRoom.byCurGuestCount		= pmsg->room.byCurGuestCount ;
	myRoom.byTotalGuestCount	= pmsg->room.byTotalGuestCount ;

	SafeStrCpy(myRoom.title, pmsg->room.title, TITLE_SIZE) ;

	// ä�ù� ����Ʈ�� ������ �߰�.
	// ä�ù� ����Ʈ�� �����ִٸ�, �� �з��� ������ �߰�.
	/*if(m_pMainDlg->IsActive())
	{
		BYTE byType = 0 ;
		byType = m_pMainDlg->GetCurRoomType() ;

		if(byType == pmsg->room.byRoomType || byType == e_RTM_AllLooK)
		{
			m_pMainDlg->Update_RoomInfo(&myRoom) ;
		}
	}*/
	m_pMainDlg->Update_RoomInfo(&myRoom) ;

	// ä�ù� �ɼ� ����.
	//if(!m_pOptionDlg->SetOption(&m_MyRoom))
	//{
	//	Throw_Error("Failed to setting room option!!", __FUNCTION__) ;
	//	return ;
	//}
	SetOption(&m_MyRoom) ;

	// ������ ����Ʈ ����.
	m_pGuestListDlg->InitGuestList(pmsg->user, pmsg->room.byCurGuestCount) ;

	// ä��â Ȱ��ȭ.
	m_pChatRoomDlg->SetDlgToDefault() ;
	m_pChatRoomDlg->SetRoomTitle(&myRoom) ;
	m_pChatRoomDlg->AddCommonMsg(e_ChatMsg_StepIn, HERO->GetObjectName()) ;
	m_pChatRoomDlg->SetActive(TRUE) ;

	// HERO ä�� ���·� ����.
	Set_ChatRoomState(e_State_In_ChatRoom) ;

	// ���� ��� �ʱ�ȭ.
	ClearFreeze() ;
}






//-------------------------------------------------------------------------------------------------
//	NAME : Join_Room_Nack
//	DESC : ä�ù� ���� ���� ó���� �ϴ� �Լ�.
//  DATE : APRIL 16, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Join_Room_Nack(void* pMsg) 
{
	// ���� �޽��� ����
    // 0 = err_Already_Chatting,
	// 1 = err_Invalid_Room_Info,
	// 2 = err_Invalid_Secret_Code,
	// 3 = err_Guest_Count_Is_Over,
	// 4 = err_Invalid_User_Info,

	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_BYTE* pmsg = NULL ;
	pmsg = (MSG_BYTE*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}



	// ���� �޽��� Ȯ��.
	WORD wErrMsgNum = 0 ;
	switch(pmsg->bData)
	{
	case 0 : wErrMsgNum = 1505 ; break ;
	case 1 : wErrMsgNum = 1509 ; break ;
	case 2 : wErrMsgNum = 1510 ; break ;
	case 3 : wErrMsgNum = 1511 ; break ;
	case 4 : wErrMsgNum = 1514 ; break ;
	default : return ;
	}

	// ä�ù濡 ���� �޽��� ���.
	WINDOWMGR->MsgBox( MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(wErrMsgNum)) ;
}






//-------------------------------------------------------------------------------------------------
//	NAME : Join_Room_Notice
//	DESC : 
//  DATE : APRIL 16, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Join_Room_Notice(void* pMsg) 
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

	// ���� ��Ʈ�� üũ.
	ASSERT(m_pChatRoomDlg) ;
	ASSERT(m_pGuestListDlg) ;
	ASSERT(m_pMainDlg) ;

	if(!m_pChatRoomDlg || !m_pGuestListDlg || !m_pMainDlg)
	{
		Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}


	// ������ �ε��� ������Ʈ.
	for(BYTE count = 0 ; count < MAX_USER_PER_ROOM ; ++count)
	{
		m_MyRoom.dwUser[count] = pmsg->dwUser[count] ;
	}

	// ������ �� ������Ʈ.
	m_MyRoom.byCurGuestCount = pmsg->byCount ;

	// ���� ������ ���� ���̵� �߰�.
	m_MyRoom.dwUser[m_MyRoom.byCurGuestCount] = pmsg->user.dwPlayerID ;

	// ������ �� ���� ó��.
	++m_MyRoom.byCurGuestCount ;



	// ä�ù� ����Ʈ�� ������ ������Ʈ.
	ST_CR_ROOM_CLT myRoom ;
	memset(&myRoom, 0, sizeof(ST_CR_ROOM_CLT)) ;

	myRoom.byIdx				= m_MyRoom.byIdx ;
	myRoom.bySecretMode			= m_MyRoom.bySecretMode ;
	myRoom.byRoomType			= m_MyRoom.byRoomType ;
	myRoom.byCurGuestCount		= m_MyRoom.byCurGuestCount ;
	myRoom.byTotalGuestCount	= m_MyRoom.byTotalGuestCount ;

	SafeStrCpy(myRoom.title, m_MyRoom.title, TITLE_SIZE) ;

	m_pMainDlg->Update_RoomInfo(&myRoom) ;

	// ������ ����Ʈ ����.
	if(!m_pGuestListDlg->AddGuest(&pmsg->user))
	{
		Throw_Error("Failed to add guest!!", __FUNCTION__) ;
		return ;
	}

	// ���� ���� �߰�.
	m_pChatRoomDlg->AddCommonMsg(e_ChatMsg_StepIn, pmsg->user.name) ;
}






//-------------------------------------------------------------------------------------------------
//	NAME : Kick_Guest_Ack
//	DESC : ���� ���� ���� ó���� �ϴ� �Լ�.
//  DATE : APRIL 16, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Kick_Guest_Ack(void* pMsg)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_CR_ROOMLIST* pmsg = NULL ;
	pmsg = (MSG_CR_ROOMLIST*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}



	// ������ �ʱ�ȭ.
	memset(&m_MyRoom, 0, sizeof(ST_CR_ROOM_SRV)) ;

	// ä�ù� ����Ʈ�� ������ ������Ʈ.
	m_pMainDlg->Init_RoomList((MSG_CR_ROOMLIST*)pmsg) ;

	// ä�ù� �ɼ� �ʱ�ȭ.
	m_pOptionDlg->SetActive(FALSE) ;
	m_pOptionDlg->SetDlgToDefault() ;

	// ������ ����Ʈ �ʱ�ȭ.
	m_pGuestListDlg->SetActive(FALSE) ;
	m_pGuestListDlg->SetDlgToDefault() ;

	// ä��â �ʱ�ȭ.
	//m_pChatRoomDlg->SetActive(FALSE) ;
	//m_pChatRoomDlg->SetDlgToDefault() ;
	m_pChatRoomDlg->CloseChatRoom() ;

	// HERO �κ� ���·� ����.
	Set_ChatRoomState(e_State_In_Lobby) ;

	WINDOWMGR->MsgBox( MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1513)) ;




	//// �Լ� �Ķ���� üũ.
	//ASSERT(pMsg) ;

	//if(!pMsg)
	//{
	//	Throw_Error(err_IMP, __FUNCTION__) ;
	//	return ;
	//}

	//// ���� �޽��� ��ȯ.
	//MSGBASE* pmsg = NULL ;
	//pmsg = (MSGBASE*)pMsg ;

	//ASSERT(pmsg) ;

	//if(!pmsg)
	//{
	//	Throw_Error(err_FCMTO, __FUNCTION__) ;
	//	return ;
	//}




	//--m_MyRoom.byCurGuestCount ;

	//if(m_MyRoom.byCurGuestCount == 0)
	//{
	//	// ä�ù� ����Ʈ�� ������ ����.
	//	m_pMainDlg->DeleteRoom(m_MyRoom.byIdx) ;
	//}
	//else
	//{
	//	ST_CR_ROOM_CLT myRoom ;
	//	memset(&myRoom, 0, sizeof(ST_CR_ROOM_CLT)) ;

	//	myRoom.byIdx				= m_MyRoom.byIdx ;
	//	myRoom.bySecretMode			= m_MyRoom.bySecretMode ;
	//	myRoom.byRoomType			= m_MyRoom.byRoomType ;
	//	myRoom.byCurGuestCount		= m_MyRoom.byCurGuestCount ;
	//	myRoom.byTotalGuestCount	= m_MyRoom.byTotalGuestCount ;

	//	SafeStrCpy(myRoom.title, m_MyRoom.title, TITLE_SIZE) ;

	//	// ä�ù� ����Ʈ�� ������ ������Ʈ.
	//	m_pMainDlg->Update_RoomInfo(&myRoom) ;
	//}

	//// ������ �ʱ�ȭ.
	//memset(&m_MyRoom, 0, sizeof(ST_CR_ROOM_CLT)) ;

	//// ä�ù� �ɼ� �ʱ�ȭ.
	//m_pOptionDlg->SetDlgToDefault() ;
	//m_pOptionDlg->SetActive(FALSE) ;

	//// ������ ����Ʈ �ʱ�ȭ.
	//m_pGuestListDlg->SetDlgToDefault() ;
	//m_pGuestListDlg->SetActive(FALSE) ;

	//// ä��â �ʱ�ȭ.
	//m_pChatRoomDlg->SetActive(FALSE) ;
	//m_pChatRoomDlg->SetDlgToDefault() ;

	//// HERO �κ� ���·� ����.
	//Set_ChatRoomState(e_State_In_Lobby) ;

	//// ���� �޽��� ����.
	//WINDOWMGR->MsgBox( MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1513)) ;
}






//-------------------------------------------------------------------------------------------------
//	NAME : Kick_Guest_Nack
//	DESC : ���� ���� ���� ó���� �ϴ� �Լ�.
//  DATE : APRIL 16, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Kick_Guest_Nack(void* pMsg) 
{
	// ���� �޽��� ����.
	// 0 = err_CanNot_Found_RoomInfo,
	// 1 = err_Invalid_OwnerInfo,
	// 2 = err_You_Are_Not_Owner,
	// 3 = err_Invalid_Target,

	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_BYTE* pmsg = NULL ;
	pmsg = (MSG_BYTE*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}



	// ���� �޽��� Ȯ��.
	WORD wErrMsgNum = 0 ;
	switch(pmsg->bData)
	{
	case 0 : wErrMsgNum = 1509 ; break ;
	case 1 : wErrMsgNum = 1501 ; break ;
	case 2 : wErrMsgNum = 1487 ; break ;
	case 3 : wErrMsgNum = 1502 ; break ;
	default : return ;
	}

	// ä�ù濡 ���� �޽��� ���.
	AddChatRoomMsg(CHATMGR->GetChatMsg(wErrMsgNum), e_ChatMsg_System) ;
}






//-------------------------------------------------------------------------------------------------
//	NAME : Kick_Guest_Notice
//	DESC : ���� ���� ó���� �ϴ� �Լ�.
//  DATE : APRIL 16, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Kick_Guest_Notice(void* pMsg) 
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



	// ������ �ε��� ������Ʈ.
	for(BYTE count = 0 ; count < MAX_USER_PER_ROOM ; ++count)
	{
		m_MyRoom.dwUser[count] = pmsg->dwUser[count] ;
	}

	// ������ �� ������Ʈ.
	m_MyRoom.byCurGuestCount = pmsg->byCount ;



	// ä�ù� ����Ʈ�� ������ ������Ʈ.
	ST_CR_ROOM_CLT myRoom ;
	memset(&myRoom, 0, sizeof(ST_CR_ROOM_CLT)) ;

	myRoom.byIdx				= m_MyRoom.byIdx ;
	myRoom.bySecretMode			= m_MyRoom.bySecretMode ;
	myRoom.byRoomType			= m_MyRoom.byRoomType ;
	myRoom.byCurGuestCount		= m_MyRoom.byCurGuestCount ;
	myRoom.byTotalGuestCount	= m_MyRoom.byTotalGuestCount ;

	SafeStrCpy(myRoom.title, m_MyRoom.title, TITLE_SIZE) ;

	m_pMainDlg->Update_RoomInfo(&myRoom) ;

	// ������ ����Ʈ ����.
	m_pGuestListDlg->DeleteGuest(pmsg->dwID) ;

	// �޽��� ���� ��, ä�ù濡 ���.
	char tempMsg[(MAX_CHAT_LENGTH+1) + (MAX_NAME_LENGTH+1)] = {0,} ;

	strcat(tempMsg, "[") ;
	strcat(tempMsg, "<") ;
	strcat(tempMsg, pmsg->name) ;
	strcat(tempMsg, ">") ;
	strcat(tempMsg, CHATMGR->GetChatMsg(1496)) ;
	strcat(tempMsg, "]") ;

	m_pChatRoomDlg->AddMsg(tempMsg, e_ChatMsg_System) ;

	// ������� ���ܸ�ܿ��� ����.
	//DeleteFreezeUser(pmsg->dwID) ;
	UnFreezeUser(pmsg->dwID) ;



	//if(pmsg->dwID == HEROID)
	//{
	//	ST_CR_ROOM_CLT myRoom ;
	//	memset(&myRoom, 0, sizeof(ST_CR_ROOM_CLT)) ;

	//	myRoom.byIdx				= m_MyRoom.byIdx ;
	//	myRoom.bySecretMode			= m_MyRoom.bySecretMode ;
	//	myRoom.byRoomType			= m_MyRoom.byRoomType ;
	//	myRoom.byCurGuestCount		= m_MyRoom.byCurGuestCount ;
	//	myRoom.byTotalGuestCount	= m_MyRoom.byTotalGuestCount ;

	//	SafeStrCpy(myRoom.title, m_MyRoom.title, TITLE_SIZE) ;

	//	// ä�ù� ����Ʈ�� ������ ������Ʈ.
	//	m_pMainDlg->Update_RoomInfo(&myRoom) ;

	//	// ������ �ʱ�ȭ.
	//	memset(&m_MyRoom, 0, sizeof(ST_CR_ROOM_CLT)) ;

	//	// ä�ù� �ɼ� �ʱ�ȭ.
	//	m_pOptionDlg->SetDlgToDefault() ;
	//	m_pOptionDlg->SetActive(FALSE) ;

	//	// ������ ����Ʈ �ʱ�ȭ.
	//	m_pGuestListDlg->SetDlgToDefault() ;
	//	m_pGuestListDlg->SetActive(FALSE) ;

	//	// ä��â �ʱ�ȭ.
	//	m_pChatRoomDlg->SetActive(FALSE) ;
	//	m_pChatRoomDlg->SetDlgToDefault() ;

	//	// HERO �κ� ���·� ����.
	//	Set_ChatRoomState(e_State_In_Lobby) ;

	//	// ���� �޽��� ����.
	//	WINDOWMGR->MsgBox( MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1513)) ;
	//}
	//else
	//{
	//	// ���� �ε����� �ӽ� �����Ѵ�.
	//	BYTE byCheckCount = 0 ;
	//	DWORD dwPlayerID[ MAX_USER_PER_ROOM ] = {0, } ;
	//	for(BYTE count = 0 ; count < m_MyRoom.byCurGuestCount ; ++count)
	//	{
	//		if(m_MyRoom.dwUser[count] == pmsg->dwID) continue ;

	//		dwPlayerID[byCheckCount] = m_MyRoom.dwUser[count] ;
	//		++byCheckCount ;
	//	}

	//	// ä�ù��� ������ �� ����.
	//	--m_MyRoom.byCurGuestCount ;

	//	// ���� �ε��� ����
	//	for(BYTE count = 0 ; count  < MAX_USER_PER_ROOM ; ++count)
	//	{
	//		m_MyRoom.dwUser[count] = dwPlayerID[count] ;
	//	}

	//	// ä�ù� ����Ʈ�� ������ ������Ʈ.
	//	ST_CR_ROOM_CLT myRoom ;
	//	memset(&myRoom, 0, sizeof(ST_CR_ROOM_CLT)) ;

	//	myRoom.byIdx				= m_MyRoom.byIdx ;
	//	myRoom.bySecretMode			= m_MyRoom.bySecretMode ;
	//	myRoom.byRoomType			= m_MyRoom.byRoomType ;
	//	myRoom.byCurGuestCount		= m_MyRoom.byCurGuestCount ;
	//	myRoom.byTotalGuestCount	= m_MyRoom.byTotalGuestCount ;

	//	SafeStrCpy(myRoom.title, m_MyRoom.title, TITLE_SIZE) ;

	//	m_pMainDlg->Update_RoomInfo(&myRoom) ;

	//	// ������ ����Ʈ ����.
	//	m_pGuestListDlg->DeleteGuest(pmsg->dwID) ;

	//	// �޽��� ���� ��, ä�ù濡 ���.
	//	char tempMsg[(MAX_CHAT_LENGTH+1) + (MAX_NAME_LENGTH+1)] = {0,} ;

	//	strcat(tempMsg, "[") ;
	//	strcat(tempMsg, "<") ;
	//	strcat(tempMsg, pmsg->name) ;
	//	strcat(tempMsg, ">") ;
	//	strcat(tempMsg, CHATMGR->GetChatMsg(1496)) ;
	//	strcat(tempMsg, "]") ;

	//	m_pChatRoomDlg->AddMsg(tempMsg, e_ChatMsg_System) ;

	//	// ������� ���ܸ�ܿ��� ����.
	//	DeleteFreezeUser(pmsg->dwID) ;
	//}
}






//-------------------------------------------------------------------------------------------------
//	NAME : Out_Room_Ack
//		 : 
//	DESC : �� ������ ���� ó���� �ϴ� �Լ�.
//		 : 
//		 : Ack�� ������ ���� ����� ��� ó���� ���ش�.
//		 : ������ ����
//		 : 
//  DATE : APRIL 16, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Out_Room_Ack(void* pMsg)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_CR_ROOMLIST* pmsg = NULL ;
	pmsg = (MSG_CR_ROOMLIST*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}



	// ������ �ʱ�ȭ.
	memset(&m_MyRoom, 0, sizeof(ST_CR_ROOM_SRV)) ;

	// ä�ù� ����Ʈ�� ������ ������Ʈ.
	m_pMainDlg->Init_RoomList((MSG_CR_ROOMLIST*)pmsg) ;

	// ä�ù� �ɼ� �ʱ�ȭ.
	m_pOptionDlg->SetActive(FALSE) ;
	m_pOptionDlg->SetDlgToDefault() ;

	// ������ ����Ʈ �ʱ�ȭ.
	m_pGuestListDlg->SetActive(FALSE) ;
	m_pGuestListDlg->SetDlgToDefault() ;

	// ä��â �ʱ�ȭ.
	//m_pChatRoomDlg->SetActive(FALSE) ;
	//m_pChatRoomDlg->SetDlgToDefault() ;
	m_pChatRoomDlg->CloseChatRoom() ;

	// HERO �κ� ���·� ����.
	Set_ChatRoomState(e_State_In_Lobby) ;


	//// �Լ� �Ķ���� üũ.
	//ASSERT(pMsg) ;

	//if(!pMsg)
	//{
	//	Throw_Error(err_IMP, __FUNCTION__) ;
	//	return ;
	//}

	//// ���� �޽��� ��ȯ.
	//MSGBASE* pmsg = NULL ;
	//pmsg = (MSGBASE*)pMsg ;

	//ASSERT(pmsg) ;

	//if(!pmsg)
	//{
	//	Throw_Error(err_FCMTO, __FUNCTION__) ;
	//	return ;
	//}



	//// ������ �� ����.
	//--m_MyRoom.byCurGuestCount ;

	//if(m_MyRoom.byCurGuestCount == 0)
	//{
	//	// ä�ù� ����Ʈ�� ������ ����.
	//	m_pMainDlg->DeleteRoom(m_MyRoom.byIdx) ;
	//}
	//else
	//{
	//	ST_CR_ROOM_CLT myRoom ;
	//	memset(&myRoom, 0, sizeof(ST_CR_ROOM_CLT)) ;

	//	myRoom.byIdx				= m_MyRoom.byIdx ;
	//	myRoom.bySecretMode			= m_MyRoom.bySecretMode ;
	//	myRoom.byRoomType			= m_MyRoom.byRoomType ;
	//	myRoom.byCurGuestCount		= m_MyRoom.byCurGuestCount ;
	//	myRoom.byTotalGuestCount	= m_MyRoom.byTotalGuestCount ;

	//	SafeStrCpy(myRoom.title, m_MyRoom.title, TITLE_SIZE) ;

	//	m_pMainDlg->Update_RoomInfo(&myRoom) ;
	//}

	//// ������ �ʱ�ȭ.
	//memset(&m_MyRoom, 0, sizeof(ST_CR_ROOM_CLT)) ;

	//// ä�ù� �ɼ� �ʱ�ȭ.
	//m_pOptionDlg->SetDlgToDefault() ;
	//m_pOptionDlg->SetActive(FALSE) ;

	//// ������ ����Ʈ �ʱ�ȭ.
	//m_pGuestListDlg->SetDlgToDefault() ;
	//m_pGuestListDlg->SetActive(FALSE) ;

	//// ä��â �ʱ�ȭ.
	//m_pChatRoomDlg->SetActive(FALSE) ;
	//m_pChatRoomDlg->SetDlgToDefault() ;

	//// HERO �κ� ���·� ����.
	//Set_ChatRoomState(e_State_In_Lobby) ;
}






//-------------------------------------------------------------------------------------------------
//	NAME : Out_Room_Nack
//	DESC : ä�ù� ���� ���� ó���� �ϴ� �Լ�.
//  DATE : APRIL 16, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Out_Room_Nack(void* pMsg)
{
	// ���� �޽��� ����.
	// 0 = err_CanNot_Found_Room,
	// 1 = err_CanNot_Found_Outer,

	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_BYTE* pmsg = NULL ;
	pmsg = (MSG_BYTE*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}



	// ���� �޽��� Ȯ��.
	WORD wErrMsgNum = 0 ;
	switch(pmsg->bData)
	{
	case 0 : wErrMsgNum = 1503 ; break ;
	case 1 : wErrMsgNum = 1514 ; break ;
	default : return ;
	}

	// ä�ù濡 ���� �޽��� ���.
	AddChatRoomMsg(CHATMGR->GetChatMsg(wErrMsgNum), e_ChatMsg_System) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Out_Room_EmptyRoom
//	DESC : ä�ù��� ������ ��, ���̻� ���� ���� �� ó���� �ϴ� �Լ�.
//  DATE : APRIL 16, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Out_Room_EmptyRoom(void* pMsg)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� ��Ʈ�� üũ.
	ASSERT(m_pMainDlg) ;

	if(!m_pMainDlg)
	{
		Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// ������ �ʱ�ȭ.
	memset(&m_MyRoom, 0, sizeof(ST_CR_ROOM_SRV)) ;

	// ä�ù� ����Ʈ�� ������ ������Ʈ.
	m_pMainDlg->Clear_RoomList() ;
	m_pMainDlg->Refresh_RoomList() ;

	// ä�ù� �ɼ� �ʱ�ȭ.
	m_pOptionDlg->SetActive(FALSE) ;
	m_pOptionDlg->SetDlgToDefault() ;

	// ������ ����Ʈ �ʱ�ȭ.
	m_pGuestListDlg->SetActive(FALSE) ;
	m_pGuestListDlg->SetDlgToDefault() ;

	// ä��â �ʱ�ȭ.
	//m_pChatRoomDlg->SetActive(FALSE) ;
	//m_pChatRoomDlg->SetDlgToDefault() ;
	m_pChatRoomDlg->CloseChatRoom() ;

	// HERO �κ� ���·� ����.
	Set_ChatRoomState(e_State_In_Lobby) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Out_Room_Last_Man
//	DESC : ���������� ä�ù��� ������ �� ó���� �Ѵ�. Ack�ʹ� ó���ϴ� ������ �ٸ���.
//  DATE : APRIL 16, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Out_Room_Last_Man(void* pMsg) 
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_CR_ROOMLIST* pmsg = NULL ;
	pmsg = (MSG_CR_ROOMLIST*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}



	// ������ �ʱ�ȭ.
	memset(&m_MyRoom, 0, sizeof(ST_CR_ROOM_SRV)) ;

	// ä�ù� ����Ʈ�� ������ ������Ʈ.
	m_pMainDlg->Init_RoomList((MSG_CR_ROOMLIST*)pmsg) ;

	// ä�ù� �ɼ� �ʱ�ȭ.
	m_pOptionDlg->SetActive(FALSE) ;
	m_pOptionDlg->SetDlgToDefault() ;

	// ������ ����Ʈ �ʱ�ȭ.
	m_pGuestListDlg->SetActive(FALSE) ;
	m_pGuestListDlg->SetDlgToDefault() ;

	// ä��â �ʱ�ȭ.
	//m_pChatRoomDlg->SetActive(FALSE) ;
	//m_pChatRoomDlg->SetDlgToDefault() ;
	m_pChatRoomDlg->CloseChatRoom() ;

	// HERO �κ� ���·� ����.
	Set_ChatRoomState(e_State_In_Lobby) ;
}






//-------------------------------------------------------------------------------------------------
//	NAME : Out_Room_Notice
//	DESC : �ٸ� ������ ä�ù��� �����ٴ� ���� ó���� �ϴ� �Լ�.
//  DATE : APRIL 16, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Out_Room_Notice(void* pMsg) 
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



	// ���� �ε��� ������Ʈ.
	//m_MyRoom.dwOwnerIdx = pmsg->dwID ;

	// ������ �ε��� ������Ʈ.
	for(BYTE count = 0 ; count < MAX_USER_PER_ROOM ; ++count)
	{
		m_MyRoom.dwUser[count] = pmsg->dwUser[count] ;
	}

	// ������ �� ������Ʈ.
	m_MyRoom.byCurGuestCount = pmsg->byCount ;



	//// ���� �ε����� �ӽ� �����Ѵ�.
	//BYTE byCheckCount = 0 ;
	//DWORD dwPlayerID[ MAX_USER_PER_ROOM ] = {0, } ;
	//for(BYTE count = 0 ; count < m_MyRoom.byCurGuestCount ; ++count)
	//{
	//	if(m_MyRoom.dwUser[count] == pmsg->dwID) continue ;

	//	dwPlayerID[byCheckCount] = m_MyRoom.dwUser[count] ;
	//	++byCheckCount ;
	//}

	//// ä�ù��� ������ �� ����.
	//--m_MyRoom.byCurGuestCount ;

	//// ���� �ε��� ����
	//for(BYTE count = 0 ; count  < m_MyRoom.byCurGuestCount ; ++count)
	//{
	//	m_MyRoom.dwUser[count] = dwPlayerID[count] ;
	//}


	// ä�ù� ����Ʈ�� ������ ������Ʈ.
	ST_CR_ROOM_CLT myRoom ;
	memset(&myRoom, 0, sizeof(ST_CR_ROOM_CLT)) ;

	myRoom.byIdx				= m_MyRoom.byIdx ;
	myRoom.bySecretMode			= m_MyRoom.bySecretMode ;
	myRoom.byRoomType			= m_MyRoom.byRoomType ;
	myRoom.byCurGuestCount		= m_MyRoom.byCurGuestCount ;
	myRoom.byTotalGuestCount	= m_MyRoom.byTotalGuestCount ;

	SafeStrCpy(myRoom.title, m_MyRoom.title, TITLE_SIZE) ;

	m_pMainDlg->Update_RoomInfo(&myRoom) ;

	// ������ ����Ʈ ����.
	m_pGuestListDlg->DeleteGuest(pmsg->dwID) ;

	// �޽��� ���� ��, ä�ù濡 ���.
	char tempMsg[MAX_CHAT_LENGTH] = {0,} ;

	strcat(tempMsg, "[") ;
	strcat(tempMsg, "<") ;
	strcat(tempMsg, pmsg->name) ;
	strcat(tempMsg, ">") ;
	strcat(tempMsg, CHATMGR->GetChatMsg(1497)) ;
	strcat(tempMsg, "]") ;

	m_pChatRoomDlg->AddMsg(tempMsg, e_ChatMsg_System) ;

	// ������� ���ܸ�ܿ��� ����.
	//DeleteFreezeUser(pmsg->dwID) ;
	UnFreezeUser(pmsg->dwID) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Out_Room_Change_Owner_Notice
//	DESC : ������ ���������� ���� ��η� ä�ù��� ������ ��, ���� ������� ó���ϴ� �Լ�.
//  DATE : APRIL 16, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Out_Room_Change_Owner_Notice(void* pMsg)
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



	// ���� �ε��� ������Ʈ.
	m_MyRoom.dwOwnerIdx = pmsg->dwID ;

	// ������ �ε��� ������Ʈ.
	for(BYTE count = 0 ; count < MAX_USER_PER_ROOM ; ++count)
	{
		m_MyRoom.dwUser[count] = pmsg->dwUser[count] ;
	}

	// ������ �� ������Ʈ.
	m_MyRoom.byCurGuestCount = pmsg->byCount ;



	// ä�ù� ����Ʈ�� ������ ������Ʈ.
	ST_CR_ROOM_CLT myRoom ;
	memset(&myRoom, 0, sizeof(ST_CR_ROOM_CLT)) ;

	myRoom.byIdx				= m_MyRoom.byIdx ;
	myRoom.bySecretMode			= m_MyRoom.bySecretMode ;
	myRoom.byRoomType			= m_MyRoom.byRoomType ;
	myRoom.byCurGuestCount		= m_MyRoom.byCurGuestCount ;
	myRoom.byTotalGuestCount	= m_MyRoom.byTotalGuestCount ;

	SafeStrCpy(myRoom.title, m_MyRoom.title, TITLE_SIZE) ;

	m_pMainDlg->Update_RoomInfo(&myRoom) ;



	// ���� ó��.
	char NoticeMsg[128] = {0, } ;
	strcat(NoticeMsg, "[") ;
	strcat(NoticeMsg, "<") ;
	strcat(NoticeMsg, pmsg->name) ;
	strcat(NoticeMsg, ">") ;
	strcat(NoticeMsg, CHATMGR->GetChatMsg(1495)) ;
	strcat(NoticeMsg, "]") ;
	
	m_pChatRoomDlg->AddMsg(NoticeMsg, e_ChatMsg_System) ;

	m_pGuestListDlg->SetClassMark() ;
}






//-------------------------------------------------------------------------------------------------
//	NAME : Request_Friend_Nack
//	DESC : ģ����û ���� ó���� �ϴ� �Լ�.
//  DATE : APRIL 16, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Request_Friend_Nack(void* pMsg)
{
	// ���� �޽��� ����.
	// 0 = err_Invalid_Requester_Info,
	// 1 = err_Invalid_Target_Info.
	// 2 = err_You_Are_Not_In_SameRoom,

	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_BYTE* pmsg = NULL ;
	pmsg = (MSG_BYTE*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}



	// ���� �޽��� Ȯ��.
	WORD wErrMsgNum = 0 ;
	switch(pmsg->bData)
	{
	case 0 : wErrMsgNum = 1514 ; break ;
	case 1 : wErrMsgNum = 1502 ; break ;
	case 2 : wErrMsgNum = 1504 ; break ;
	default : return ;
	}

	// ä�ù濡 ���� �޽��� ���.
	AddChatRoomMsg(CHATMGR->GetChatMsg(wErrMsgNum), e_ChatMsg_System) ;
}






//-------------------------------------------------------------------------------------------------
//	NAME : RoomList_Ack
//	DESC : ä�ù� �ޱ� ���� ó���� �ϴ� �Լ�.
//  DATE : APRIL 16, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::RoomList_Ack(void* pMsg) 
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_CR_ROOMLIST* pmsg = NULL ;
	pmsg = (MSG_CR_ROOMLIST*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}

	// ���� ��Ʈ�� üũ.
	ASSERT(m_pMainDlg) ;

	if(!m_pMainDlg)
	{
		Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// ä�ù� ����Ʈ ���� üũ.
	if(pmsg->byCount == 0)
	{
		WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1490)) ;
	}

	// ä�ù� ����Ʈ ����.
	m_pMainDlg->Init_RoomList(pMsg) ;

	if(Get_ChatRoomState() == e_State_Away_From_Chatroom)
	{
		Set_ChatRoomState(e_State_In_Lobby) ;
	}
}






//-------------------------------------------------------------------------------------------------
//	NAME : RoomList_Nack
//	DESC : ä�ù� ��� �ޱ� ���� ó���� �ϴ� �Լ�.
//  DATE : APRIL 16, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::RoomList_Nack(void* pMsg) 
{
	// ���� �޽��� ����.
	// 0 = err_Empty_Room,
	// 1 = err_Delay_Time,

	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_BYTE* pmsg = NULL ;
	pmsg = (MSG_BYTE*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� Ȯ��.
	WORD wErrMsgNum = 0 ;
	switch(pmsg->bData)
	{
	case 0 : m_pMainDlg->Err_EmptyRoom() ; return ;
	case 1 : WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1517)) ; return ;
	default : return ;
	}

	// ä�ù濡 ���� �޽��� ���.
	//WINDOWMGR->MsgBox( MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(wErrMsgNum)) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : SearchName_Ack
//	DESC : �̸����� �˻� ���� ó���� �ϴ� �Լ�.
//  DATE : APRIL 24, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::SearchName_Ack(void* pMsg)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_CR_SEARCH_NAME* pmsg = NULL ;
	pmsg = (MSG_CR_SEARCH_NAME*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}
	


	// �˻� ��� ó��.
	char tempMsg[128] = {0, } ;
	sprintf(tempMsg, CHATMGR->GetChatMsg(1536), pmsg->name, pmsg->byRoomIdx) ;
	WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, tempMsg) ; return ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : SearchName_Nack
//	DESC : �̸����� �˻� ���� ó���� �ϴ� �Լ�.
//  DATE : APRIL 24, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::SearchName_Nack(void* pMsg)
{
	// ���� �޽��� ����.
	// 0 = err_CanNot_Found_User,
	// 1 = err_CanNot_Found_Name,
	// 2 = err_ChatRoom_Destroyed,

	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_BYTE* pmsg = NULL ;
	pmsg = (MSG_BYTE*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}

	//// ���� �޽��� Ȯ��.
	//WORD wErrMsgNum = 0 ;
	//switch(pmsg->bData)
	//{
	//case 0 : wErrMsgNum = 1503 ; break ;
	//case 1 : wErrMsgNum = 1514 ; break ;
	//case 2 : wErrMsgNum = 1514 ; break ;
	//default : return ;
	//}

	//// ä�ù濡 ���� �޽��� ���.
	//AddChatRoomMsg(CHATMGR->GetChatMsg(wErrMsgNum), e_ChatMsg_System) ;

	// ���� �޽��� Ȯ��.
	WORD wErrMsgNum = 0 ;
	switch(pmsg->bData)
	{
	case 0 : WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1514)) ; break ;
	case 1 : 
	case 2 : WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1537)) ; break ;
	default : return ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : SearchTitle_Ack
//	DESC : ä�ù� �������� ä�ù� �˻� ���� ó���� �ϴ� �Լ�.
//  DATE : APRIL 24, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::SearchTitle_Ack(void* pMsg)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_CR_ROOMLIST* pmsg = NULL ;
	pmsg = (MSG_CR_ROOMLIST*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}

	// ���� ��Ʈ�� üũ.
	ASSERT(m_pMainDlg) ;

	if(!m_pMainDlg)
	{
		Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// ����Ʈ ����.
	m_pMainDlg->Init_RoomList(pMsg) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : SearchTitle_Nack
//	DESC : ä�ù� �������� ä�ù� �˻� ���� ó���� �ϴ� �Լ�.
//  DATE : APRIL 24, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::SearchTitle_Nack(void* pMsg)
{
	// ���� �޽��� ����.
	// 0 = err_CanNot_Found_Room,

	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_BYTE* pmsg = NULL ;
	pmsg = (MSG_BYTE*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}

	//// ���� �޽��� Ȯ��.
	//WORD wErrMsgNum = 0 ;
	//switch(pmsg->bData)
	//{
	//case 0 : wErrMsgNum = 1503 ; break ;
	//case 1 : wErrMsgNum = 1514 ; break ;
	//case 2 : wErrMsgNum = 1514 ; break ;
	//default : return ;
	//}

	//// ä�ù濡 ���� �޽��� ���.
	//AddChatRoomMsg(CHATMGR->GetChatMsg(wErrMsgNum), e_ChatMsg_System) ;

	m_pMainDlg->Clear_RoomList() ;

	WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1537)) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : UpdateInfo_Created_Room
//	DESC : �������� ������ �������� ä�ù渮��Ʈ�� ���� �߰��ϴ� �Լ�.
//  DATE : APRIL 29, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::UpdateInfo_Created_Room(void* pMsg)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_CR_ROOMINFO_TO_NOTICE* pmsg = NULL ;
	pmsg = (MSG_CR_ROOMINFO_TO_NOTICE*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}

	// ���� ��Ʈ�� üũ.
	ASSERT(m_pMainDlg) ;

	if(!m_pMainDlg)
	{
		Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// �� ä�ù��� ������ ������ �Ѱܹ�����.
	if(m_MyRoom.byIdx == pmsg->room.byIdx) return ;



	// ä�ù� ����Ʈ�� ä�ù� �߰��ϱ�.
	ST_CR_ROOM_CLT myRoom ;
	memset(&myRoom, 0, sizeof(ST_CR_ROOM_CLT)) ;

	myRoom.byIdx				= pmsg->room.byIdx ;
	myRoom.bySecretMode			= pmsg->room.bySecretMode ;
	myRoom.byRoomType			= pmsg->room.byRoomType ;
	myRoom.byCurGuestCount		= pmsg->room.byCurGuestCount ;
	myRoom.byTotalGuestCount	= pmsg->room.byTotalGuestCount ;

	SafeStrCpy(myRoom.title, pmsg->room.title, TITLE_SIZE) ;

	m_pMainDlg->Add_Room(&myRoom) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : UpdateInfo_Deleted_Room
//	DESC : �������� ������ �������� ä�ù� ����Ʈ���� ���� �����ϴ� �Լ�.
//  DATE : APRIL 29, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::UpdateInfo_Deleted_Room(void* pMsg)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_BYTE* pmsg = NULL ;
	pmsg = (MSG_BYTE*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}

	// ���� ��Ʈ�� üũ.
	ASSERT(m_pMainDlg) ;

	if(!m_pMainDlg)
	{
		Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// �� ä�ù��� ������ ������ �Ѱܹ�����.
	if(m_MyRoom.byIdx == pmsg->bData) return ;



	// ä�ù� ����Ʈ���� ä�ù� ����.
	m_pMainDlg->DeleteRoom(pmsg->bData) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : UpdateInfo_SecretMode
//	DESC : �������� ������ ������ ä�ù渮��Ʈ����, ������ ���� ����/����� ������ �����Ѵ�.
//  DATE : APRIL 29, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::UpdateInfo_SecretMode(void* pMsg)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_BYTE2* pmsg = NULL ;
	pmsg = (MSG_BYTE2*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}

	// ���� ��Ʈ�� üũ.
	ASSERT(m_pMainDlg) ;

	if(!m_pMainDlg)
	{
		Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// �� ä�ù��� ������ ������ �Ѱܹ�����.
	//if(m_MyRoom.byIdx == pmsg->bData1) return ;



	// ä�ù� ����Ʈ���� ä�ù� ���� ������Ʈ.
	ST_CR_ROOM_CLT* pRoom = NULL ;
	pRoom = m_pMainDlg->GetRoomListItem(pmsg->bData1) ;

	if(!pRoom) return ;

	/*ST_CR_ROOM_CLT myRoom ;
	memset(&myRoom, 0, sizeof(ST_CR_ROOM_CLT)) ;

	myRoom.byIdx				= pmsg->bData1 ;
	myRoom.bySecretMode			= pmsg->bData2 ;
	myRoom.byRoomType			= pRoom->byRoomType ;
	myRoom.byCurGuestCount		= pRoom->byCurGuestCount ;
	myRoom.byTotalGuestCount	= pRoom->byTotalGuestCount ;

	SafeStrCpy(myRoom.title, pRoom->title, TITLE_SIZE) ;*/

	pRoom->bySecretMode = pmsg->bData2 ;

	m_pMainDlg->Update_RoomInfo(pRoom) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : UpdateInfo_RoomType
//	DESC : �������� ������ ������ ä�ù� ����Ʈ����, ������ ���� ä�ù� �з��� �����Ѵ�.
//  DATE : APRIL 29, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::UpdateInfo_RoomType(void* pMsg)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_BYTE2* pmsg = NULL ;
	pmsg = (MSG_BYTE2*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}

	// ���� ��Ʈ�� üũ.
	ASSERT(m_pMainDlg) ;

	if(!m_pMainDlg)
	{
		Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// �� ä�ù��� ������ ������ �Ѱܹ�����.
	//if(m_MyRoom.byIdx == pmsg->bData1) return ;



	// ä�ù� ����Ʈ���� ä�ù� ���� ������Ʈ.
	ST_CR_ROOM_CLT* pRoom = NULL ;
	pRoom = m_pMainDlg->GetRoomListItem(pmsg->bData1) ;

	if(!pRoom) return ;

	/*
	ST_CR_ROOM_CLT myRoom ;
	memset(&myRoom, 0, sizeof(ST_CR_ROOM_CLT)) ;

	myRoom.byIdx				= pmsg->bData1 ;
	myRoom.bySecretMode			= pRoom->bySecretMode ;
	myRoom.byRoomType			= pmsg->bData2 ;
	myRoom.byCurGuestCount		= pRoom->byCurGuestCount ;
	myRoom.byTotalGuestCount	= pRoom->byTotalGuestCount ;

	SafeStrCpy(myRoom.title, pRoom->title, TITLE_SIZE) ;*/

	pRoom->byRoomType	= pmsg->bData2 ;

	m_pMainDlg->Update_RoomInfo(pRoom) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : UpdateInfo_Title
//	DESC : �������� ������ ������ ä�ù� ����Ʈ����, ������ ���� ä�ù� ������ �����Ѵ�.
//  DATE : APRIL 29, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::UpdateInfo_Title(void* pMsg)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_CR_SEARCH_TITLE* pmsg = NULL ;
	pmsg = (MSG_CR_SEARCH_TITLE*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}

	// ���� ��Ʈ�� üũ.
	ASSERT(m_pMainDlg) ;

	if(!m_pMainDlg)
	{
		Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// �� ä�ù��� ������ ������ �Ѱܹ�����.
	//if(m_MyRoom.byIdx == pmsg->byRoomIdx) return ;



	// ä�ù� ����Ʈ���� ä�ù� ���� ������Ʈ.
	ST_CR_ROOM_CLT* pRoom = NULL ;
	pRoom = m_pMainDlg->GetRoomListItem(pmsg->byRoomIdx) ;

	if(!pRoom) return ;

	/*ST_CR_ROOM_CLT myRoom ;
	memset(&myRoom, 0, sizeof(ST_CR_ROOM_CLT)) ;

	myRoom.byIdx				= pmsg->byRoomIdx ;
	myRoom.bySecretMode			= pRoom->bySecretMode ;
	myRoom.byRoomType			= pRoom->byRoomType ;
	myRoom.byCurGuestCount		= pRoom->byCurGuestCount ;
	myRoom.byTotalGuestCount	= pRoom->byTotalGuestCount ;

	SafeStrCpy(myRoom.title, pmsg->title, TITLE_SIZE) ;*/

	SafeStrCpy(pRoom->title, pmsg->title, TITLE_SIZE) ;

	m_pMainDlg->Update_RoomInfo(pRoom) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : UpdateInfo_Cur_GuestCount
//	DESC : �������� ������ ������ ä�ù� ����Ʈ����, ������ ���� ���� ������ ���� �����Ѵ�.
//  DATE : APRIL 29, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::UpdateInfo_Cur_GuestCount(void* pMsg)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_BYTE2* pmsg = NULL ;
	pmsg = (MSG_BYTE2*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}

	// ���� ��Ʈ�� üũ.
	ASSERT(m_pMainDlg) ;

	if(!m_pMainDlg)
	{
		Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// �� ä�ù��� ������ ������ �Ѱܹ�����.
	//if(m_MyRoom.byIdx == pmsg->bData1) return ;



	// ä�ù� ����Ʈ���� ä�ù� ���� ������Ʈ.
	ST_CR_ROOM_CLT* pRoom = NULL ;
	pRoom = m_pMainDlg->GetRoomListItem(pmsg->bData1) ;

	if(!pRoom) return ;

	/*ST_CR_ROOM_CLT myRoom ;
	memset(&myRoom, 0, sizeof(ST_CR_ROOM_CLT)) ;

	myRoom.byIdx				= pmsg->bData1 ;
	myRoom.bySecretMode			= pRoom->bySecretMode ;
	myRoom.byRoomType			= pRoom->byRoomType ;
	myRoom.byCurGuestCount		= pmsg->bData2 ;
	myRoom.byTotalGuestCount	= pRoom->byTotalGuestCount ;

	SafeStrCpy(myRoom.title, pRoom->title, TITLE_SIZE) ;*/

	pRoom->byCurGuestCount = pmsg->bData2 ;

	m_pMainDlg->Update_RoomInfo(pRoom) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : UpdateInfo_Total_GuestCount
//	DESC : �������� ������ ������ ä�ù� ����Ʈ����, ������ ���� �ִ� ������ ���� �����Ѵ�.
//  DATE : APRIL 29, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::UpdateInfo_Total_GuestCount(void* pMsg)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ.
	MSG_BYTE2* pmsg = NULL ;
	pmsg = (MSG_BYTE2*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}

	// ���� ��Ʈ�� üũ.
	ASSERT(m_pMainDlg) ;

	if(!m_pMainDlg)
	{
		Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// �� ä�ù��� ������ ������ �Ѱܹ�����.
	//if(m_MyRoom.byIdx == pmsg->bData1) return ;



	// ä�ù� ����Ʈ���� ä�ù� ���� ������Ʈ.
	ST_CR_ROOM_CLT* pRoom = NULL ;
	pRoom = m_pMainDlg->GetRoomListItem(pmsg->bData1) ;

	if(!pRoom) return ;

	/*ST_CR_ROOM_CLT myRoom ;
	memset(&myRoom, 0, sizeof(ST_CR_ROOM_CLT)) ;

	myRoom.byIdx				= pmsg->bData1 ;
	myRoom.bySecretMode			= pRoom->bySecretMode ;
	myRoom.byRoomType			= pRoom->byRoomType ;
	myRoom.byCurGuestCount		= pRoom->byCurGuestCount ;
	myRoom.byTotalGuestCount	= pmsg->bData2 ;

	SafeStrCpy(myRoom.title, pRoom->title, TITLE_SIZE) ;*/

	pRoom->byTotalGuestCount = pmsg->bData2 ;

	m_pMainDlg->Update_RoomInfo(pRoom) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : UpdateMyLastComTime
//	DESC : �÷��̾ ������ ������ ��� �ð��� ������Ʈ �Ѵ�.
//  DATE : APRIL 14, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::UpdateMyLastComTime()
{
	m_dwLastMyComTime = gCurTime ; 
}





//-------------------------------------------------------------------------------------------------
//	NAME : IsEnableExecuteCom
//	DESC : ����� �ٽ� ������ �ð��� �������� Ȯ���ϴ� �Լ�.
//  DATE : APRIL 14, 2008 LYW
//-------------------------------------------------------------------------------------------------
BYTE CChatRoomMgr::IsEnableExecuteCom()
{
	DWORD dwDelay = 0 ;
	dwDelay = gCurTime - m_dwLastMyComTime ;

	if(dwDelay >= CHATROOM_DELAY_TIME) return TRUE ;
	else return FALSE ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : IsFreezed
//	DESC : ���� �� �������� üũ�ϴ� �Լ�.
//  DATE : APRIL 14, 2008 LYW
//-------------------------------------------------------------------------------------------------
BOOL CChatRoomMgr::IsFreezed(DWORD dwPlayerID)
{
	for(BYTE count = 0 ; count < MAX_USER_PER_ROOM ; ++count)
	{
		if(m_dwFreezedID[count] == dwPlayerID) return TRUE ;
	}

	return FALSE ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : FreezeUser
//	DESC : ���� �� ������ ��� ��Ű�� �Լ�.
//  DATE : APRIL 14, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::FreezeUser(DWORD dwPlayerID)
{
	for(BYTE count = 0 ; count < MAX_USER_PER_ROOM ; ++count)
	{
		if(m_dwFreezedID[count] != 0) continue ;

		m_dwFreezedID[count] = dwPlayerID ;
		return ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : UnFreezeUser
//	DESC : ���� ���� ��Ű�� �Լ�.
//  DATE : APRIL 14, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::UnFreezeUser(DWORD dwPlayerID)
{
	for(BYTE count = 0 ; count < MAX_USER_PER_ROOM ; ++count)
	{
		if(m_dwFreezedID[count] != dwPlayerID) continue ;

		m_dwFreezedID[count] = 0 ;
		return ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : ClearFreeze
//	DESC : ���� �� ������ ��� ���� �Լ�.
//  DATE : APRIL 14, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::ClearFreeze()
{
	for(BYTE count = 0 ; count < MAX_USER_PER_ROOM ; ++count)
	{
		m_dwFreezedID[count] = 0 ;
	}
}





////-------------------------------------------------------------------------------------------------
////	NAME : DeleteFreezeUser
////	DESC : ���õ� ���� �ε����� �����Ѵ�.
////  DATE : APRIL 14, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomMgr::DeleteFreezeUser(DWORD dwPlayerID)
//{
//	for(BYTE count = 0 ; count < MAX_USER_PER_ROOM ; ++count)
//	{
//		if(m_dwFreezedID[count] != dwPlayerID) continue ;
//
//		m_dwFreezedID[count] = 0 ;
//
//		return ;
//	}
//}





//-------------------------------------------------------------------------------------------------
//	NAME : Throw_Error
//	DESC : ���� �޽����� ó���ϴ� �Լ�.
//  DATE : APRIL 14, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Throw_Error(BYTE errType, char* szCaption)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(szCaption) ;

	if(!szCaption || strlen(szCaption) <= 1)
	{
#ifdef _MB_CRMGR_
		MessageBox( NULL, "Invalid err message!!", __FUNCTION__, MB_OK) ;
#endif //_MB_CRMGR_	
		return ;
	}

	// ���� Ÿ�� Ȯ��.
	char tempErr[128] = {0, } ;
	switch(errType)
	{
	case err_IMP :		SafeStrCpy(tempErr, "Invalid a message parameter!!", 128) ;					break ;
	case err_ICCTLS :	SafeStrCpy(tempErr, "Invalid child controls!!", 128) ;						break ;
	case err_FCMTB :	SafeStrCpy(tempErr, "Failed to convert a message to base!!", 128) ;			break ;
	case err_FCMTO :	SafeStrCpy(tempErr, "Failed to convert a message to original!!", 128) ;		break ;
	default : break ;
	}

	// ���� ���.
#ifdef _MB_CRMGR_
	MessageBox( NULL, tempErr, szCaption, MB_OK) ;
#endif //_MB_CRMGR_	
}





//-------------------------------------------------------------------------------------------------
//	NAME : Throw_Error
//	DESC : ���� �޽����� ó���ϴ� �Լ�.
//  DATE : APRIL 14, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Throw_Error(char* szErr, char* szCaption)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(szCaption) ;

	if(!szErr || strlen(szErr) <= 1)
	{
#ifdef _MB_CRMGR_
		MessageBox( NULL, "Invalid err message!!", __FUNCTION__, MB_OK) ;
#endif //_MB_CRMGR_	
		return ;
	}

	if(!szCaption || strlen(szCaption) <= 1)
	{
#ifdef _MB_CRMGR_
		MessageBox( NULL, "Invalid caption!!", __FUNCTION__, MB_OK) ;
#endif //_MB_CRMGR_	
	}

	// ���� ���.
#ifdef _MB_CRMGR_
	MessageBox( NULL, szErr, szCaption, MB_OK) ;
#endif //_MB_CRMGR_	
}





//-------------------------------------------------------------------------------------------------
//	NAME : SaveChat
//	DESC : �� �̵� ��, �ʿ��� ��, ä�ù��� ä�� ������ ����Ѵ�.
//  DATE : APRIL 11, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::SaveChat()
{
	// HERO�� ä�������� üũ.
	if(Get_ChatRoomState() == e_State_In_Lobby) return ;

	// ����Ʈ ���̾�α� �ޱ�.
	cListDialog* pListDlg = NULL ;
	pListDlg = m_pChatRoomDlg->GetListDlg() ;

	ASSERT(pListDlg) ;

	if(!pListDlg)
	{
#ifdef _MB_CRMGR_
		MessageBox( NULL, "ERR-FRLD", "CRMGR-SC", MB_OK) ;
#endif //_MB_CRMGR_
		return ;
	}

	// ������ ����Ʈ �ޱ�.
	cPtrList* pList = NULL ;
	pList = pListDlg->GetListItem() ;

	ASSERT(pList) ;

	if(!pList)
	{
#ifdef _MB_CRMGR_
		MessageBox( NULL, "ERR-FRPL", "CRMGR-SC", MB_OK) ;
#endif //_MB_CRMGR_
		return ;
	}

	// ä�ó��� ����.
	PTRLISTPOS pos = NULL ;
	pos = pList->GetHeadPosition() ;

	if(!pos) return ;

	while(pos)
	{
		ITEM* pItem = new ITEM ;
		*pItem = *(ITEM*)pList->GetNext( pos ) ;
		m_BackupList.AddTail( pItem ) ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : RestoreChat
//	DESC : ��� �� ä�� ������ �����Ѵ�.
//  DATE : APRIL 10, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::RestoreChat()
{
	if(!m_pChatRoomDlg || !m_pGuestListDlg || !m_pOptionDlg)
	{
		return ;
	}

	// HERO�� ä�������� üũ.
	if(Get_ChatRoomState() != e_State_In_ChatRoom) return ;

	// ä�ù� �ɼ� ����.
	//if(!m_pOptionDlg->SetOption(&m_MyRoom))
	//{
	//	Throw_Error("Faild to setting option!!", __FUNCTION__) ;
	//	return ;
	//}
	SetOption(&m_MyRoom) ;

	if(m_pOptionDlg->IsActive()) m_pOptionDlg->SetActive(FALSE) ;

	if(m_pGuestListDlg->IsActive()) m_pGuestListDlg->SetActive(FALSE) ;

	// ����Ʈ ���̾�α� �ޱ�.
	cListDialog* pListDlg = NULL ;
	pListDlg = m_pChatRoomDlg->GetListDlg() ;

	ASSERT(pListDlg) ;

	if(!pListDlg)
	{
		Throw_Error("Failtd to receive a list dialog!!", __FUNCTION__) ;
		return ;
	}

	// ��� ����Ʈ ������ �ޱ�.
	PTRLISTPOS pos = NULL ;
	pos = m_BackupList.GetHeadPosition() ;

	if(!pos) return ;

	// ä�� ���� ����.
	ITEM* pItem = NULL ;
	while( pos )
	{
		pItem = (ITEM*)m_BackupList.GetNext( pos ) ;

		if( pItem )
		{
			pListDlg->AddItem( pItem->string, pItem->rgb, pItem->line ) ;
		}
	}

	ST_CR_ROOM_CLT myRoom ;
	memset(&myRoom, 0, sizeof(ST_CR_ROOM_CLT)) ;

	myRoom.byIdx				= m_MyRoom.byIdx ;
	myRoom.bySecretMode			= m_MyRoom.bySecretMode ;
	myRoom.byRoomType			= m_MyRoom.byRoomType ;
	myRoom.byCurGuestCount		= m_MyRoom.byCurGuestCount ;
	myRoom.byTotalGuestCount	= m_MyRoom.byTotalGuestCount ;

	SafeStrCpy(myRoom.title, m_MyRoom.title, TITLE_SIZE) ;

	m_pChatRoomDlg->SetRoomTitle(&myRoom) ;
	m_pChatRoomDlg->SetActive(TRUE) ;

	m_pGuestListDlg->SetClassMark() ;

	// ��� ����Ʈ ����.
	ReleaseBackupList() ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : RestoreChat
//	DESC : ����� �� ����Ʈ�� �����Ѵ�.
//  DATE : APRIL 10, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::ReleaseBackupList()
{
	// ��� ����Ʈ �ޱ�.
	PTRLISTPOS pos = NULL ;
	pos = m_BackupList.GetHeadPosition() ;

	if(!pos) return ;

	// ��� ����Ʈ ����.
	ITEM* pItem = NULL ;
	while( pos )
	{
		pItem = (ITEM*)m_BackupList.GetNext( pos ) ;

		if( pItem ) delete pItem ;
	}

	m_BackupList.RemoveAll() ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : AddChatRoomMsg
//	DESC : ä�ù� â�� �޽����� �߰��Ѵ�.
//  DATE : APRIL 10, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::AddChatRoomMsg(char* pMsg, BYTE byType)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;
	ASSERT(strlen(pMsg) > 2) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� ��Ʈ�� üũ.
	ASSERT(m_pChatRoomDlg) ;

	if(!m_pChatRoomDlg)
	{
		Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}

	// ä�ù� â�� �޽����� �߰��Ѵ�.
	m_pChatRoomDlg->AddMsg(pMsg, byType) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : GetChatRoomTextArea
//	DESC : ä�ù��� �Է� ��Ʈ���� ��ȯ�ϴ� �Լ�.
//  DATE : APRIL 16, 2008 LYW
//-------------------------------------------------------------------------------------------------
//cTextArea* CChatRoomMgr::GetChatRoomTextArea()
cEditBox* CChatRoomMgr::GetChatRoomInputBox()
{
	// ���� ��Ʈ�� üũ.
	ASSERT(m_pChatRoomDlg) ;

	if(!m_pChatRoomDlg)
	{
		Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return NULL ;
	}

	// TextArea �ޱ�.
	//cTextArea* pCtrl = NULL ;
	cEditBox* pCtrl = NULL ;
	pCtrl = m_pChatRoomDlg->GetInputBox() ;

	ASSERT(pCtrl) ;

	if(!pCtrl)
	{
		Throw_Error("Failed to receive textarea!!", __FUNCTION__) ;
		return NULL ;
	}

	return pCtrl ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : MsgProc
//	DESC : ä�ù濡�� �߻��ϴ� �޽��� ���� ó���� �ϴ� �Լ�.
//  DATE : APRIL 16, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::MsgProc()
{
	// TextArea �ޱ�.
	//cTextArea* pCtrl = NULL ;
	//pCtrl = GetChatRoomTextArea() ;
	cEditBox* pCtrl = NULL ;
	pCtrl = GetChatRoomInputBox() ;

	ASSERT(pCtrl) ;

	if(!pCtrl)
	{
		Throw_Error("Failed to receive textarea!!", __FUNCTION__) ;
		return ;
	}

	char tempMsg[MAX_CHAT_LENGTH+1] = {0, } ;
	//pCtrl->GetScriptText(tempMsg) ;
	SafeStrCpy(tempMsg, pCtrl->GetEditText(), MAX_CHAT_LENGTH) ;

	if(strlen(tempMsg) == 0) return ;

	// ���͸� ó��.
	if( FILTERTABLE->FilterChat(tempMsg) )
	{
		m_pChatRoomDlg->AddMsg(CHATMGR->GetChatMsg(1225), e_ChatMsg_System) ;
		//pCtrl->SetScriptText("") ;
		pCtrl->SetEditText("") ;
		return;
	}

	if( gCurTime - m_dwLastMyChat < 1000 )	// 1��
	{
		AddChatRoomMsg(CHATMGR->GetChatMsg(248), e_ChatMsg_System) ;
		pCtrl->SetEditText("") ;
		return ;
	}
	else
	{
		// ������ ä�� �ð� ������Ʈ.
		UpdateChatLastTime(gCurTime) ;

		// �޽��� ���� ó��.
		MSG_CR_MSG_BROADCAST msg ;
		memset(&msg, 0, sizeof(MSG_CR_MSG_BROADCAST)) ;

		msg.Category	= MP_CHATROOM ;
		msg.Protocol	= MP_CHATROOM_CHATMSG_NORMAL_SYN ;
		msg.dwObjectID	= HEROID ;

		SafeStrCpy(msg.name, HERO->GetObjectName(), MAX_NAME_LENGTH) ;
		SafeStrCpy(msg.Msg, tempMsg, MAX_CHAT_LENGTH+1) ;

		BYTE byCount = 0 ;
		byCount = m_MyRoom.byCurGuestCount ;

		for(BYTE count = 0 ; count < byCount ; ++count)
		{
			msg.dwUser[count] = m_MyRoom.dwUser[count] ;

			++msg.byCount ;
		}

		NETWORK->Send( &msg, sizeof(MSG_CR_MSG_BROADCAST)) ;

		pCtrl->SetEditText("") ;
	}

	//pCtrl->SetReadOnly(FALSE) ; 
	//pCtrl->SetFocusEdit(TRUE) ;
	//pCtrl->SetScriptText("") ;

	//// �Ӹ��� �����ϴ� ������ üũ.
	//if(CHATROOMMGR->IsWhisperMsg())
	//{
	//	// �Ӹ� ����.
	//	SendWhisperMsg() ;
	//}
	//else
	//{
	//	// �Ϲ� �޽��� ����.
	//	SendNormalMsg() ;
	//}

	//pCtrl->SetScriptText("") ;
}





////-------------------------------------------------------------------------------------------------
////	NAME : IsWhisperMsg
////	DESC : �����Ϸ��� �޽����� �ӼӸ�����, �Ϲ� ä������ üũ�Ѵ�.
////  DATE : APRIL 16, 2008 LYW
////-------------------------------------------------------------------------------------------------
//BOOL CChatRoomMgr::IsWhisperMsg()
//{
//	// TextArea �ޱ�.
//	cTextArea* pCtrl = NULL ;
//	pCtrl = GetChatRoomTextArea() ;
//
//	ASSERT(pCtrl) ;
//
//	if(!pCtrl)
//	{
//		Throw_Error("Failed to receive textarea!!", __FUNCTION__) ;
//		return ;
//	}
//
//	char* pMsg = NULL ;
//	pMsg = pCtrl->GetScriptText() ;
//}
//
//
//
//
////-------------------------------------------------------------------------------------------------
////	NAME : SendNormalMsg
////	DESC : ä�ù� â�� �޽����� �����ϴ� �Լ�.
////  DATE : APRIL 10, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomMgr::SendNormalMsg()
//{
//	// TextArea �ޱ�.
//	cTextArea* pCtrl = NULL ;
//	pCtrl = GetChatRoomTextArea() ;
//
//	ASSERT(pCtrl) ;
//
//	if(!pCtrl)
//	{
//		Throw_Error("Failed to receive textarea!!", __FUNCTION__) ;
//		return ;
//	}
//
//	// �޽��� ���� ó��.
//	MSG_CHATROOM_MSG msg ;
//	memset(&msg, 0, sizeof(MSG_CHATROOM_MSG)) ;
//
//	msg.Category	= MP_CHATROOM ;
//	msg.Protocol	= MP_CHATROOM_CHATMSG_NORMAL_SYN ;
//	msg.dwObjectID	= HEROID ;
//
//	msg.dwOwnerIdx	= CHATROOMMGR->GetMyRoomOwnerIdx() ;
//
//	SafeStrCpy(msg.Msg, pCtrl->GetScriptText(), MAX_CHAT_LENGTH+1) ;
//
//	NETWORK->Send( &msg, sizeof(msg) ) ;
//}
//
//
//
//
//
////-------------------------------------------------------------------------------------------------
////	NAME : SendWhisperMsg
////	DESC : ä�ù� â�� �ӼӸ��� �����ϴ� �Լ�.
////  DATE : APRIL 10, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomMgr::SendWhisperMsg()
//{
//	// �Ӹ� ���� ó��.
//	MSG_CHATROOM_WHISPER msg ;
//	memset(&msg, 0, sizeof(MSG_CHATROOM_WHISPER)) ;
//
//	msg.Category		= MP_CHATROOM ;
//	msg.Protocol		= MP_CHATROOM_CHATMSG_WHISPER_SYN ;
//	msg.dwObjectID		= HEROID ;
//
//	msg.dwSenderID		= HEROID ;
//	msg.dwReceiverID	= ;
//
//	SafeStrCpy(msg.Msg, pCtrl->GetScriptText(), MAX_CHAT_LENGTH+1) ;
//
//	NETWORK->Send( &msg, sizeof(msg) ) ;
//}




















//////-------------------------------------------------------------------------------------------------
//////	NAME : SetMyRoomOption
//////	DESC : HERO�� ������ ä�ù��� �ɼ��� �����Ѵ�.
//////  DATE : APRIL 11, 2008 LYW
//////-------------------------------------------------------------------------------------------------
////void CChatRoomMgr::SetMyRoomOption(WORD wCode, ST_CHATROOM_ROOM_BASE* pInfo)
////{
////	ASSERT(pInfo) ;
////
////	if(!pInfo)
////	{
////#ifdef _MB_CRMGR_
////		MessageBox( NULL, "ERR-FRRB", "CRMGR-SRO", MB_OK) ;
////#endif //_MB_CRMGR_
////		return ;
////	}
////
////	m_wMyRoomCode		= wCode ;
////
////	m_ChatRoomOption	= *pInfo ;
////}












































////-------------------------------------------------------------------------------------------------
////	NAME : AddCharacterAck
////	DESC : ĳ������ ���� ��� �ϷḦ ó�� �ϴ� �Լ�.
////  DATE : APRIL 14, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomMgr::AddCharacterAck(void* pMsg)
//{
//	Set_ChatRoomState(e_State_In_Lobby) ;
//}
//
//
//
//
//
////-------------------------------------------------------------------------------------------------
////	NAME : Errors
////	DESC : ���� �޽��� ó���� �ϴ� �Լ�.
////  DATE : APRIL 7, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomMgr::Errors(void* pMsg) 
//{
////	// �Լ� �Ķ���� üũ.
////	ASSERT(pMsg) ;
////
////	if(!pMsg)
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-IMP", "CRMGR-ERR", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
////
////	// ���� �޽��� ��ȯ.
////	MSG_BYTE* pmsg = NULL ; 
////	pmsg = (MSG_BYTE*)pMsg ;
////
////	ASSERT(pmsg) ;
////
////	if(!pmsg)
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-FCMO", "CRMGR-ERR", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
////
////	// ���� �޽��� Ȯ��.
////	switch(pmsg->bData)
////	{
////	case err_Already_Chatting :					WINDOWMGR->MsgBox(NULL, MBT_OK, "ä�� �߿� ����� �� ���� ����Դϴ�.") ;	break ;
////
////	case err_CreateRoom_RoomCount_Over :		WINDOWMGR->MsgBox(NULL, MBT_OK, "���̻� ä�ù��� ������ �� �����ϴ�.") ;	break ;
////
////	case err_JoinRoom_SecretCode_Failed :		WINDOWMGR->MsgBox(NULL, MBT_OK, "��й�ȣ�� ��ġ���� �ʽ��ϴ�.") ; break ;
////	case err_JoinRoom_GuestCount_Over :			WINDOWMGR->MsgBox(NULL, MBT_OK, "�����Ͻ� ä�ù��� ���� �ο��� �Ѿ����ϴ�.") ; break ;
////	
////	case err_ChangeOwner_Invalid_OwnerInfo : 
////	case err_KickGuest_Invalid_OwnerInfo :		WINDOWMGR->MsgBox(NULL, MBT_OK, "������ ������ ��ȿ���� �ʽ��ϴ�.") ; break ;
////
////	case err_ChangeOwner_Invalid_TargetInfo : 
////	case err_ChatMsg_Whisper_Invalid_Receiver : 
////	case err_KickGuest_Invalid_TargetInfo : 
////	case err_OutRoom_Invalid_TargetInfo :	
////	case err_Request_Friend_Invalid_Receiver :	WINDOWMGR->MsgBox(NULL, MBT_OK, "����� ������ ��ȿ���� �ʽ��ϴ�.") ; break ;
////
////	case err_ChangeOption_Not_Chatting : 
////	case err_ChatMsg_Normal_Invalid_Sender : 
////	case err_ChatMsg_Whisper_Invalid_Sender : 
////	case err_Request_Friend_Invalid_Sender :	
////	case err_Request_Re_Login :					WINDOWMGR->MsgBox(NULL, MBT_OK, "ä�ù� ������ ��ȿ���� �ʽ��ϴ�. �ٽ� �α��� �� �ּ���.") ; break ;
////
////	case err_ChangeOption_No_RoomInfo :
////	case err_ChangeOwner_Invalid_RoomInfo : 
////	case err_ChatMsg_Normal_Invalid_RoomInfo : 
////	case err_ChatMsg_Whisper_Invalid_RoomInfo : 
////	case err_JoinRoom_No_RoomInfo :
////	case err_KickGuest_Invalid_RoomInfo :
////	case err_OutRoom_No_RoomInfo :				WINDOWMGR->MsgBox(NULL, MBT_OK, "�� ������ ��ȿ���� �ʽ��ϴ�.") ; break ;
////
////	case err_RoomList_Empty_Room :
////		{			
////			m_pMainDlg->Err_EmptyRoom() ;	
////
////			m_pMainDlg->SetPage(1, 1) ;
////		}
////		break ;
////
////	default : break ;
////	}
//}
//
//
//
//
//
////-------------------------------------------------------------------------------------------------
////	NAME : Create_Room_Ack
////	DESC : ä�ù� ���� ���� ó���� �ϴ� �Լ�.
////  DATE : APRIL 7, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomMgr::Create_Room_Ack(void* pMsg) 
//{
////	// �Լ� �Ķ���� üũ.
////	ASSERT(pMsg) ;
////
////	if(!pMsg)
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-IMP", "CRMGR-CRA", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
////
////	// ���� �޽��� ��ȯ
////	ST_CR_ROOM_CLT* pmsg = NULL ;
////	pmsg = (ST_CR_ROOM_CLT*)pMsg ;
////
////	ASSERT(pmsg) ;
////
////	if(!pmsg)
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-FCMO", "CRMGR-CRA", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
////
////	// ���� ��Ʈ�� üũ.
////	ASSERT(m_pChatRoomDlg) ;
////	ASSERT(m_pGuestListDlg) ;
////	ASSERT(m_pMainDlg) ;
////	ASSERT(m_pOptionDlg) ;
////
////	if(!m_pChatRoomDlg || !m_pGuestListDlg || !m_pMainDlg || !m_pOptionDlg)
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-IVDCS", "CRMGR-CRA", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
////
////	// ������ ����.
////	memset(&m_MyRoom, 0, sizeof(ST_CR_ROOM_CLT)) ;
////	memcpy(&m_MyRoom, pmsg, sizeof(ST_CR_ROOM_CLT)) ;
////
////	// ä�ù� ����Ʈ�� ������ �߰�.
////	if(!m_pMainDlg->Add_Room(&m_MyRoom))
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-FAR", "CRMGR-CRA", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
////
////	// ä�ù� �ɼ� ����.
////	if(!m_pOptionDlg->SetOption(&m_MyRoom.baseInfo, m_MyRoom.wCode))
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-FSO", "CRMGR-CRA", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
////
////	// ������ ����Ʈ ����.
////	if(!m_pGuestListDlg->AddGuest(&m_MyRoom))
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-FAG", "CRMGR-CRA", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
////
////	// ä��â Ȱ��ȭ.
////	m_pChatRoomDlg->SetDlgToDefault() ;
////	//m_pChatRoomDlg->SetRoomTitle(pmsg->baseInfo.title) ;
////	m_pChatRoomDlg->SetRoomTitle(&m_MyRoom.baseInfo, m_MyRoom.wIdx) ;
////	m_pChatRoomDlg->AddCommonMsg(e_ChatMsg_StepIn, m_MyRoom.user[0].name) ;
////	m_pChatRoomDlg->SetActive(TRUE) ;
////
////	// HERO ä�� ���·� ����.
////	Set_ChatRoomState(e_State_In_ChatRoom) ;
//}
//
//
//
//
//
////-------------------------------------------------------------------------------------------------
////	NAME : JoinRoom_Ack
////	DESC : ä�ù� ���� ���� ó�� �Լ�.
////  DATE : APRIL 7, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomMgr::JoinRoom_Ack(void* pMsg) 
//{
////	// �Լ� �Ķ���� üũ.
////	ASSERT(pMsg) ;
////
////	if(!pMsg)
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-IMP", "CRMGR-JRA", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
////
////	// ���� �޽��� ��ȯ.
////	MSG_CHATROOM_JOIN_RESULT* pmsg = NULL ;
////	pmsg = (MSG_CHATROOM_JOIN_RESULT*)pMsg ;
////
////	ASSERT(pmsg) ;
////
////	if(!pmsg)
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-FCMO", "CRMGR-JRA", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
////
////	// ���� ��Ʈ�� üũ.
////	ASSERT(m_pChatRoomDlg) ;
////	ASSERT(m_pGuestListDlg) ;
////	ASSERT(m_pMainDlg) ;
////	ASSERT(m_pOptionDlg) ;
////
////	if(!m_pChatRoomDlg || !m_pGuestListDlg || !m_pMainDlg || !m_pOptionDlg)
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-IMP", "CRMGR-JRA", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
////
////	// ������ ����.
////	memset(&m_MyRoom, 0, sizeof(ST_CR_ROOM_CLT)) ;
////	//memcpy(&m_MyRoom, pmsg, sizeof(ST_CR_ROOM_CLT)) ;
////	m_MyRoom.baseInfo	= pmsg->baseInfo ;
////	m_MyRoom.wIdx		= pmsg->wIdx ;
////	m_MyRoom.wCode		= pmsg->wCode ;
////
////	for( BYTE count = 0 ; count < MAX_USER_PER_ROOM ; ++count)
////	{
////		m_MyRoom.user[count] = pmsg->user[count] ;
////	}
////
////	// ä�ù� ����Ʈ�� ������ �߰�.
////	if(pmsg->joinID == HEROID)
////	{
////		if(!m_pMainDlg->Add_Room(&m_MyRoom))
////		{
////#ifdef _MB_CRMGR_	
////			MessageBox( NULL, "ERR-FAR", "CRMGR-JRA", MB_OK) ;
////#endif //_MB_CRMGR_	
////			return ;
////		}
////	}
////	else
////	{
////		m_pMainDlg->Update_RoomInfo(&m_MyRoom) ;
////	}
////
////	// ä�ù� �ɼ� ����.
////	if(!m_pOptionDlg->SetOption(&m_MyRoom.baseInfo, m_MyRoom.wCode))
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-FSO", "CRMGR-JRA", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
////
////	// ������ ����Ʈ ����.
////	if(!m_pGuestListDlg->AddGuest(&m_MyRoom))
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-FAG", "CRMGR-JRA", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
////
////	// ä��â Ȱ��ȭ.
////	if(pmsg->dwObjectID == HEROID)
////	{
////		// HERO�� ������ ��Ȳ�̸�, ���� �޽��� ��� �� ä��â Ȱ��ȭ.
////		m_pChatRoomDlg->SetDlgToDefault() ;
////		//m_pChatRoomDlg->SetRoomTitle(pmsg->baseInfo.title) ;
////		m_pChatRoomDlg->SetRoomTitle(&m_MyRoom.baseInfo, m_MyRoom.wIdx) ;
////		m_pChatRoomDlg->AddCommonMsg(e_ChatMsg_StepIn, HERO->GetObjectName()) ;
////		m_pChatRoomDlg->SetActive(TRUE) ;
////
////		// HERO ä�� ���·� ����.
////		Set_ChatRoomState(e_State_In_ChatRoom) ;
////	}
////	else
////	{
////		// �ٸ� ����� ���� �� ���̸�, ���� �޽��� ���.
////		if(pmsg->baseInfo.byCurGuestCount-1 == 0) return ;
////
////		BYTE byIdx = 0 ;
////		byIdx = pmsg->baseInfo.byCurGuestCount-1 ;
////
////		m_pChatRoomDlg->AddCommonMsg(e_ChatMsg_StepIn, pmsg->user[byIdx].name) ;
////	}
//}
//
//
//
//
//
////-------------------------------------------------------------------------------------------------
////	NAME : OutRoom_Ack
////	DESC : ������ ���� �̰ų�, �ڽ��� ä�ù��� ���� �� ó���� �ϴ� �Լ�.
////  DATE : APRIL 10, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomMgr::OutRoom_Ack(void* pMsg)
//{
////	// �Լ� �Ķ���� üũ.
////	ASSERT(pMsg) ;
////
////	if(!pMsg)
////	{
////#ifdef _MB_CRMGR_
////		MessageBox( NULL, "ERR-IMP", "CRMGR-ORA", MB_OK) ;
////#endif //_MB_CRMGR_
////		return ;
////	}
////
////    // ���� �޽��� ��ȯ.
////	MSG_CR_ROOMLIST* pmsg = NULL ;
////	pmsg = (MSG_CR_ROOMLIST*)pMsg ;
////
////	ASSERT(pmsg) ;
////
////	if(!pmsg)
////	{
////#ifdef _MB_CRMGR_
////		MessageBox( NULL, "ERR-FCMO", "CRMGR-ORA", MB_OK) ;
////#endif //_MB_CRMGR_
////		return ;
////	}
////
////	// ���� ��Ʈ�� üũ.
////	ASSERT(m_pChatRoomDlg) ;
////	ASSERT(m_pGuestListDlg) ;
////	ASSERT(m_pMainDlg) ;
////	ASSERT(m_pOptionDlg) ;
////
////	if(!m_pChatRoomDlg || !m_pGuestListDlg || !m_pMainDlg || !m_pOptionDlg)
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-IMP", "CRMGR-ORA", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
////
////	// �� ä�ù� ���� �ʱ�ȭ.
////	memset(&m_MyRoom, 0, sizeof(ST_CR_ROOM_CLT)) ;
////
////	// ä�ù� ����Ʈ ������Ʈ.
////	m_pMainDlg->Init_RoomList(pMsg) ;
////
////	// ä��â �ʱ�ȭ.
////	m_pChatRoomDlg->SetDlgToDefault() ;
////	m_pChatRoomDlg->SetActive(FALSE) ;
////
////	// �ɼ� �ʱ�ȭ.
////	m_pOptionDlg->SetDlgToDefault() ;
////	m_pOptionDlg->SetActive(FALSE) ;
////
////	// ��ȭ ��� ����Ʈ �ʱ�ȭ.
////	m_pGuestListDlg->SetDlgToDefault() ;
////	m_pGuestListDlg->SetActive(FALSE) ;
////
////	// HERO ���� ����.
////    Set_ChatRoomState(e_State_In_Lobby) ;
//}
//
//
//
//
//
////-------------------------------------------------------------------------------------------------
////	NAME : OutRoom_EmptyRoom
////	DESC : ä�ù濡�� ������ ���� ��, ä�ù� ����Ʈ�� ���� �� ó���ϴ� �Լ�.
////  DATE : APRIL 10, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomMgr::OutRoom_EmptyRoom(void* pMsg)
//{
////	// �Լ� �Ķ���� üũ.
////	ASSERT(m_pMainDlg) ;
////
////	if(!m_pMainDlg)
////	{
////#ifdef _MB_CRMGR_
////		MessageBox(	NULL, "ERR-FRD", "CRMGR-ORETR", MB_OK) ;
////#endif //_MB_CRMGR_
////		return ;
////	}
////
////	// ���� ��Ʈ�� üũ.
////	ASSERT(m_pChatRoomDlg) ;
////	ASSERT(m_pGuestListDlg) ;
////	ASSERT(m_pMainDlg) ;
////	ASSERT(m_pOptionDlg) ;
////
////	if(!m_pChatRoomDlg || !m_pGuestListDlg || !m_pMainDlg || !m_pOptionDlg)
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-IMP", "CRMGR-ORA", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
////
////	// �� ä�ù� ���� �ʱ�ȭ.
////	memset(&m_MyRoom, 0, sizeof(ST_CR_ROOM_CLT)) ;
////
////	// ä�ù� ����Ʈ Ŭ����.
////	m_pMainDlg->Clear_RoomList() ;
////
////	// ä��â �ʱ�ȭ.
////	m_pChatRoomDlg->SetDlgToDefault() ;
////	m_pChatRoomDlg->SetActive(FALSE) ;
////
////	// �ɼ� �ʱ�ȭ.
////	m_pOptionDlg->SetDlgToDefault() ;
////	m_pOptionDlg->SetActive(FALSE) ;
////
////	// ��ȭ ��� ����Ʈ �ʱ�ȭ.
////	m_pGuestListDlg->SetDlgToDefault() ;
////	m_pGuestListDlg->SetActive(FALSE) ;
////
////	// HERO ���� ����.
////    Set_ChatRoomState(e_State_In_Lobby) ;
//}
//
//
//
//
//
////-------------------------------------------------------------------------------------------------
////	NAME : OutRoom_Notice
////	DESC : ä�ù濡�� ������ �����ٴ� ���� ó���� �ϴ� �Լ�.
////  DATE : APRIL 10, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomMgr::OutRoom_Notice(void* pMsg)
//{
////	// �Լ� �Ķ���� üũ.
////	ASSERT(pMsg) ;
////
////	if(!pMsg)
////	{
////#ifdef _MB_CRMGR_
////		MessageBox( NULL, "ERR-IMP", "CRMGR-ORN", MB_OK) ;
////#endif //_MB_CRMGR_
////		return ;
////	}
////
////	// ���� �޽��� ��ȯ.
////	MSG_CHATROOM_OUT_ROOM* pmsg = NULL ;
////	pmsg = (MSG_CHATROOM_OUT_ROOM*)pMsg ;
////
////	ASSERT(pmsg) ;
////
////	if(!pmsg)
////	{
////#ifdef _MB_CRMGR_
////		MessageBox( NULL, "ERR-FCMO", "CRMGR-ORN", MB_OK) ;
////#endif //_MB_CRMGR_
////		return ;
////	}
////
////	// ���� ��Ʈ�� üũ.
////	ASSERT(m_pChatRoomDlg) ;
////	ASSERT(m_pGuestListDlg) ;
////	ASSERT(m_pMainDlg) ;
////	ASSERT(m_pOptionDlg) ;
////
////	if(!m_pChatRoomDlg || !m_pGuestListDlg || !m_pMainDlg || !m_pOptionDlg)
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-IMP", "CRMGR-JRA", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
////
////	// ������ ������Ʈ.
////	for(BYTE count = 0 ; count < MAX_USER_PER_ROOM ; ++count)
////	{
////		if(m_MyRoom.user[count].dwPlayerID != pmsg->dwOutIdx) continue ;
////
////		memset(&m_MyRoom.user[count], 0, sizeof(ST_CR_USER)) ;
////
////		--m_MyRoom.baseInfo.byCurGuestCount ;
////
////		break ;
////	}
////
////	// ä�ù� ����Ʈ ������Ʈ.
////	if(!m_pMainDlg->Add_Room(&m_MyRoom))
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-FAR", "CRMGR-JRA", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
////
////	// �ɼ� ������Ʈ.
////	if(!m_pOptionDlg->SetOption(&m_MyRoom.baseInfo, m_MyRoom.wCode))
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-FSO", "CRMGR-JRA", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
////
////	// ������ ����Ʈ ������Ʈ.
////	if(!m_pGuestListDlg->AddGuest(&m_MyRoom))
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-FAG", "CRMGR-JRA", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
////
////	// ������ ���� ������ ����Ѵ�.
////	m_pChatRoomDlg->AddCommonMsg(e_ChatMsg_StepOut, pmsg->OutName) ;
//}
//
//
//
//
//
////-------------------------------------------------------------------------------------------------
////	NAME : Kick_Guest_Ack
////	DESC : ������ ���� ���� ���� ó���� �ϴ� �Լ�.
////  DATE : APRIL 14, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomMgr::Kick_Guest_Ack(void* pMsg) 
//{
////	ASSERT(pMsg) ;																						// Check a message parameter.
////
////	if(!pMsg)
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-IMP", "CRMGR-KGA", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
////
////	MSG_CHATROOM_ROOM* pmsg = NULL ; 
////	pmsg = (MSG_CHATROOM_ROOM*)pMsg ;
////
////	ASSERT(pmsg) ;																						// Check converted a message.
////
////	if(!pmsg)
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-FCMO", "CRMGR-KGA", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
//}
//
//
//
//
//
////-------------------------------------------------------------------------------------------------
////	NAME : Lobby_Ack
////	DESC : 
////  DATE : APRIL 7, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomMgr::Lobby_Ack(void* pMsg) 
//{
////	ASSERT(pMsg) ;																						// Check a message parameter.
////
////	if(!pMsg)
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-IMP", "RMGR-LBA", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
////
////	MSG_CHATROOM_LOBBY* pmsg = NULL ; 
////	pmsg = (MSG_CHATROOM_LOBBY*)pMsg ;
////
////	ASSERT(pmsg) ;																						// Check converted a message.
////
////	if(!pmsg)
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-FCMO", "RMGR-LBA", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
//}
//
//
//
//
//
////-------------------------------------------------------------------------------------------------
////	NAME : Request_Friend_Ack
////	DESC : 
////  DATE : APRIL 7, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomMgr::Request_Friend_Ack(void* pMsg) 
//{
////	ASSERT(pMsg) ;																						// Check a message parameter.
////
////	if(!pMsg)
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-IMP", "CRMGR-RFA", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
////
////	MSGBASE* pmsg = NULL ; 
////	pmsg = (MSGBASE*)pMsg ;
////
////	ASSERT(pmsg) ;																						// Check converted a message.
////
////	if(!pmsg)
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-FCMO", "CRMGR-RFA", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
//}
//
//
//
//
//
////-------------------------------------------------------------------------------------------------
////	NAME : RoomList_Ack
////	DESC : The function to setting room info to main dialog.
////  DATE : APRIL 7, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomMgr::RoomList_Ack(void* pMsg)
//{
//	//m_pMainDlg->Init_RoomList(pMsg) ;
//
//	//if(Get_ChatRoomState() == e_State_Away_From_Chatroom)
//	//{
//	//	Set_ChatRoomState(e_State_In_Lobby) ;
//	//}
//}
//
//
//
//
//
////-------------------------------------------------------------------------------------------------
////	NAME : Search_Ack
////	DESC : 
////  DATE : APRIL 7, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomMgr::Search_Ack(void* pMsg) 
//{
////	ASSERT(pMsg) ;																						// Check a message parameter.
////
////	if(!pMsg)
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-IMP", "CRMGR-SCA", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
////
////	MSGBASE* pmsg = NULL ; 
////	pmsg = (MSGBASE*)pMsg ;
////
////	ASSERT(pmsg) ;																						// Check converted a message.
////
////	if(!pmsg)
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-FCMO", "CRMGR-SCA", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
//}
//
//
//
//
//
////-------------------------------------------------------------------------------------------------
////	NAME : ChatMsg_Normal
////	DESC : �Ϲ� ä�� �޽��� ó���� �ϴ� �Լ�.
////  DATE : APRIL 10, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomMgr::ChatMsg_Normal(void* pMsg)
//{
////	// �Լ� �Ķ���� üũ.
////	ASSERT(pMsg) ;
////
////	if(!pMsg)
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-IMP", "CRMGR-CMN", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
////
////	// ���� �޽��� ��ȯ.
////	MSG_CHATROOM_RETURN_MSG* pmsg = NULL ;
////	pmsg = (MSG_CHATROOM_RETURN_MSG*)pMsg ;
////
////	ASSERT(pmsg) ;
////
////	if(!pmsg)
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-FCMO", "CRMGR-CMN", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
////
////	// �޽��� Ÿ���� ����.
////	BYTE byMsgType = e_ChatMsg_OtherMsg ;
////
////	if(pmsg->dwObjectID == HEROID)
////	{
////		byMsgType = e_ChatMsg_MyMsg ;
////	}
////
////	// �޽��� ���� ��, ä�ù濡 ���.
////	char tempMsg[(MAX_CHAT_LENGTH+1) + (MAX_NAME_LENGTH+1)] = {0,} ;
////
////	strcat(tempMsg, "[") ;
////	strcat(tempMsg, pmsg->SenderName) ;
////	strcat(tempMsg, ": ") ;
////	strcat(tempMsg, pmsg->Msg) ;
////	strcat(tempMsg, "]") ;
////
////	m_pChatRoomDlg->AddMsg(tempMsg, byMsgType) ;
//}
//
//
//
//
//
////-------------------------------------------------------------------------------------------------
////	NAME : Change_Owner
////	DESC : ������ �ٲ���� �� ó���ϴ� �Լ�.
////  DATE : APRIL 14, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomMgr::Change_Owner(void* pMsg)
//{
////	// �Լ� �Ķ���� üũ.
////	ASSERT(pMsg) ;
////
////	if(!pMsg)
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-IMP", "CRMGR-CO", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
////
////	// ���� �޽��� ��ȯ.
////	MSG_CHATROOM_CHANGE_OWNER_RESULT* pmsg = NULL ;
////	pmsg = (MSG_CHATROOM_CHANGE_OWNER_RESULT*)pMsg ;
////
////	ASSERT(pmsg) ;
////
////	if(!pmsg)
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-FCMO", "CRMGR-CO", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
////
////	// ���� ��Ʈ�� üũ.
////	ASSERT(m_pChatRoomDlg) ;
////	ASSERT(m_pGuestListDlg) ;
////	ASSERT(m_pMainDlg) ;
////	ASSERT(m_pOptionDlg) ;
////
////	if(!m_pChatRoomDlg || !m_pGuestListDlg || !m_pMainDlg || !m_pOptionDlg)
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-IMP", "CRMGR-JRA", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
////
////	// �� ä�ù� ���� ������Ʈ.
////	m_MyRoom.baseInfo	= pmsg->room.baseInfo ;
////	m_MyRoom.wIdx		= pmsg->wIdx ;
////	m_MyRoom.wCode		= pmsg->wCode ;
////
////	// �ɼ� ����.
////	m_pOptionDlg->SetOption(&m_MyRoom.baseInfo, m_MyRoom.wCode) ;
////
////	// ��ȭ ��� ����Ʈ ������Ʈ.
////	m_pGuestListDlg->ChangeOwner(pmsg->dwBeforOwner, pmsg->dwCurOwner) ;
////
////	// ä�ù� ���� ������Ʈ.
////	m_pChatRoomDlg->SetRoomTitle(&m_MyRoom.baseInfo, m_MyRoom.wIdx) ;
////
////	// ���� ���� ���� ó��.
////	char tempMsg[18] = {0, } ;
////	strcat(tempMsg, "[") ;
////	strcat(tempMsg, pmsg->OwnerName) ;
////	strcat(tempMsg, "���� ������ �Ǽ̽��ϴ�.") ;
////	strcat(tempMsg, "]") ;
////	m_pChatRoomDlg->AddMsg(tempMsg, e_ChatMsg_System) ;	
////
////	// ä�ù� ����Ʈ ������Ʈ.
////	m_pMainDlg->Update_RoomInfo(&pmsg->room) ;
//}
//
//
//
//
//
////-------------------------------------------------------------------------------------------------
////	NAME : Option_Ack
////	DESC : �ɼ��� ���� �Ǿ��� �� ó���ϴ� �޽��� �Լ�.
////  DATE : APRIL 10, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomMgr::Option_Ack(void* pMsg)
//{
////	// �Լ� �Ķ���� üũ.
////	ASSERT(pMsg) ;
////
////	if(!pMsg)
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-IMP", "CRMGR-OPA", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
////
////	// ���� �޽��� ��ȯ.
////	MSG_CHATROOM_ROOM* pmsg = NULL ;
////	pmsg = (MSG_CHATROOM_ROOM*)pMsg ;
////
////	ASSERT(pmsg) ;
////
////	if(!pmsg)
////	{
////#ifdef _MB_CRMGR_	
////		MessageBox( NULL, "ERR-FCMO", "CRMGR-OPA", MB_OK) ;
////#endif //_MB_CRMGR_	
////		return ;
////	}
////
////	// �� ä�ù� ���� ������Ʈ.
////	m_MyRoom.baseInfo	= pmsg->room.baseInfo ;
////	m_MyRoom.wIdx		= pmsg->room.wIdx ;
////	m_MyRoom.wCode		= pmsg->room.wCode ;
////
////	// �ɼ� ����.
////	m_pOptionDlg->SetOption(&pmsg->room.baseInfo, pmsg->room.wCode) ;
////
////	// �ɼ� ���� ����.
////	//m_pChatRoomDlg->SetRoomTitle(pmsg->room.baseInfo.title) ;
////	m_pChatRoomDlg->SetRoomTitle(&m_MyRoom.baseInfo, m_MyRoom.wIdx) ;
////	m_pChatRoomDlg->AddMsg("[ä�ù� �ɼ��� ���� �Ǿ����ϴ�.]", e_ChatMsg_System) ;
////
////	// ä�ù� ����Ʈ ������Ʈ.
////	m_pMainDlg->Update_RoomInfo(pmsg) ;
//}
//
//
//
//
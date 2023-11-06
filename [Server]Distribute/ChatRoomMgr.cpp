//=================================================================================================
//	FILE		: CChatRoomMgr.cpp
//	DESC		: 채팅방 매니져 구현부.
//	DATE		: APRIL 2, 2008 LYW
//=================================================================================================





//-------------------------------------------------------------------------------------------------
//		헤더파일 포함.
//-------------------------------------------------------------------------------------------------
#include "stdafx.h"

#include "../[CC]ServerModule/Network.h"

#include <hash_map>
#include <algorithm>

#include "./ChatRoomMgr.h"





//-------------------------------------------------------------------------------------------------
//	NAME : CChatRoomMgr
//	DESC : 생성자 함수.
//  DATE : APRIL 2, 2008 LYW
//-------------------------------------------------------------------------------------------------
CChatRoomMgr::CChatRoomMgr(void)
{
	// 채팅방 인덱스 관리 배열 초기화.
	memset(m_byRoomIdx, 0, sizeof(BYTE)*MAX_ROOM_COUNT) ;

	m_byRoomIdx[0] = 1 ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : ~CChatRoomMgr
//	DESC : 소멸자 함수.
//  DATE : APRIL 2, 2008 LYW
//-------------------------------------------------------------------------------------------------
CChatRoomMgr::~CChatRoomMgr(void)
{
	// 채팅방/ 검색용 채팅방 해제.
	ReleaseRoom() ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : ReleaseRoom
//	DESC : 채팅방 리소스 해제 함수.
//  DATE : APRIL 22, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::ReleaseRoom()
{
	// 방 삭제.
	ST_CR_ROOM_SRV* pInfo ;
	PTRLISTPOS pos = NULL ;

	pos = m_RoomList.GetHeadPosition() ;

	while(pos)
	{
		pInfo = NULL ;
		pInfo = (ST_CR_ROOM_SRV*)m_RoomList.GetNext(pos) ;

		if(!pInfo) continue ;

		m_RoomList.Remove(pInfo) ;

		delete pInfo ;
	}

	m_RoomList.RemoveAll() ;



	// 검색용 방 삭제.
	ST_CR_ROOM_CLT* pTitle ;
	pos = m_RoomList.GetHeadPosition() ;

	while(pos)
	{
		pTitle = NULL ;
		pTitle = (ST_CR_ROOM_CLT*)m_Title.GetNext(pos) ;

		if(!pTitle) continue ;

		m_Title.Remove(pTitle) ;

		delete pTitle ;
	}

	m_Title.RemoveAll() ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : IsInRoomList
//	DESC : 방 리스트에, 방 정보가 있는지 체크하는 함수.
//  DATE : APRIL 22, 2008 LYW
//-------------------------------------------------------------------------------------------------
BYTE CChatRoomMgr::IsInRoomList(BYTE byRoomIdx) 
{
	ST_CR_ROOM_SRV* pItem ;

	PTRLISTPOS pos = NULL ;
	pos = m_RoomList.GetHeadPosition() ;

	while(pos)
	{
		pItem = NULL ;
		pItem = (ST_CR_ROOM_SRV*)m_RoomList.GetNext(pos) ;

		if(!pItem) continue ;

		if(pItem->byIdx == byRoomIdx) return TRUE ;
	}

	return FALSE ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : AddRoom
//	DESC : 채팅방 정보를 추가하는 함수.
//		 : 채팅방 정보 뿐 아니라, 검색용 채팅방 정보도 같이 추가를 한다.
//  DATE : APRIL 22, 2008 LYW
//-------------------------------------------------------------------------------------------------
BYTE CChatRoomMgr::AddRoom(ST_CR_ROOM_SRV* pInfo)
{
	// 함수 파라메터 체크.
	ASSERT(pInfo) ;

	if(!pInfo)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return FALSE ;
	}



	// 방추가.
	m_RoomList.AddTail(pInfo) ;

	// 방 인덱스 세팅.
	m_byRoomIdx[pInfo->byIdx] = pInfo->byIdx ;



	// 검색 용 방 추가.
	ST_CR_ROOM_CLT* pTitle = NULL ;
	pTitle = new ST_CR_ROOM_CLT ;

	ASSERT(pInfo) ;

	if(!pInfo)
	{
		Throw_Error("Failed to create title room!!", __FUNCTION__) ;
		return FALSE ;
	}

	pTitle->byIdx				= pInfo->byIdx ;
	pTitle->bySecretMode		= pInfo->bySecretMode ;
	pTitle->byRoomType			= pInfo->byRoomType ;
	pTitle->byCurGuestCount		= pInfo->byCurGuestCount ;
	pTitle->byTotalGuestCount	= pInfo->byTotalGuestCount ;

	SafeStrCpy(pTitle->title, pInfo->title, TITLE_SIZE) ;

	m_Title.AddTail(pTitle) ;

	return TRUE ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : DeleteRoom
//	DESC : 채팅방 정보를 삭제하는 함수.
//  DATE : APRIL 22, 2008 LYW
//-------------------------------------------------------------------------------------------------
BYTE CChatRoomMgr::DeleteRoom(BYTE byRoomIdx)
{
	// 방 삭제 처리.
	ST_CR_ROOM_SRV* pItem ;

	PTRLISTPOS pos = NULL ;
	pos = m_RoomList.GetHeadPosition() ;

	while(pos)
	{
		pItem = NULL ;
		pItem = (ST_CR_ROOM_SRV*)m_RoomList.GetNext(pos) ;

		if(!pItem) continue ;

		if(pItem->byIdx != byRoomIdx) continue ;

		m_RoomList.Remove(pItem) ;

		delete pItem ;

		m_byRoomIdx[byRoomIdx] = 0 ;

		break ;
	}



	// 검색용 방 삭제 처리.
	ST_CR_ROOM_CLT* pTitle ;
	pos = m_Title.GetHeadPosition() ;

	while(pos)
	{
		pTitle = NULL ;
		pTitle = (ST_CR_ROOM_CLT*)m_Title.GetNext(pos) ;

		if(!pTitle) continue ;

		if(pTitle->byIdx != byRoomIdx) continue ;

		m_Title.Remove(pTitle) ;

		delete pTitle ;

		break ;
	}

	return TRUE ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : GetRoomInfo
//	DESC : 채팅방 정보를 반환하는 함수.
//  DATE : APRIL 22, 2008 LYW
//-------------------------------------------------------------------------------------------------
ST_CR_ROOM_SRV* CChatRoomMgr::GetRoomInfo(BYTE byRoomIdx) 
{
	ST_CR_ROOM_SRV* pItem ;

	PTRLISTPOS pos = NULL ;
	pos = m_RoomList.GetHeadPosition() ;

	while(pos)
	{
		pItem = NULL ;
		pItem = (ST_CR_ROOM_SRV*)m_RoomList.GetNext(pos) ;

		if(!pItem) continue ;

		if(pItem->byIdx != byRoomIdx) continue ;

		return pItem ;
	}

	return NULL ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : GetRoomCountByType
//
//	DESC : 채팅방 분류에 따라서, 리스트에 몇개나 담겨 있는지 반환하는 함수.
//		 : 
//		 : * 방 개수는 최대 250개 이므로 BYTE형을 사용한다.
//		 : 
//  DATE : APRIL 22, 2008 LYW
//-------------------------------------------------------------------------------------------------
BYTE CChatRoomMgr::GetRoomCountByType(BYTE byType)
{
	BYTE byRoomCount = 0 ;

	ST_CR_ROOM_SRV* pItem ;

	PTRLISTPOS pos = NULL ;
	pos = m_RoomList.GetHeadPosition() ;

	while(pos)
	{
		pItem = NULL ;
		pItem = (ST_CR_ROOM_SRV*)m_RoomList.GetNext(pos) ;

		if(!pItem) continue ;

		if(pItem->byRoomType != byType) continue ;

		++byRoomCount ;
	}

	return byRoomCount ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : SendErrors
//	DESC : 에러 메시지를 전송하는 함수.
//  DATE : APRIL 7, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::SendErrors(DWORD dwIndex, DWORD dwPlayerID, BYTE byProtocol, BYTE byErr) 
{
	MSG_BYTE msg ;
	memset(&msg, 0, sizeof(MSG_BYTE)) ;

	msg.Category	= MP_CHATROOM ;
	msg.Protocol	= byProtocol ;
	msg.dwObjectID	= dwPlayerID ;

	msg.bData		= byErr ;

	g_Network.Send2Server(dwIndex, (char*)&msg, sizeof(MSG_BYTE)) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : NetworkMsgParser
//	DESC : 채팅방과 관련한, Dist 서버로 넘어오는 메시지를 처리하는 함수.
//  DATE : APRIL 2, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::NetworkMsgParser(DWORD dwIndex, char* pMsg, DWORD dwLength)
{
	// 함수 파라메터 체크.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// 기본 메시지 변환.
	MSGBASE* pmsg = NULL ;
	pmsg = (MSGBASE*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTB, __FUNCTION__) ;
		return ;
	}



	// 프로토콜 체크.
	switch(pmsg->Protocol)
	{
	case MP_CHATROOM_ADD_USER_SYN :			Add_User_Syn(dwIndex, pMsg, dwLength) ;			break ;
	case MP_CHATROOM_FORCE_ADD_USER_SYN :	Force_Add_User_Ysn(dwIndex, pMsg, dwLength) ;	break ;
	case MP_CHATROOM_DEL_USER_SYN :			Del_User_Syn(dwIndex, pMsg, dwLength) ;			break ;
	case MP_CHATROOM_ROOM_SYN :				Room_Syn(dwIndex, pMsg, dwLength) ;				break ;
	case MP_CHATROOM_CREATE_ROOM_SYN :		Create_Room_Syn(dwIndex, pMsg, dwLength) ;		break ;
	case MP_CHATROOM_JOIN_ROOM_SYN :		Join_Room_Syn(dwIndex, pMsg, dwLength) ;		break ;
	case MP_CHATROOM_OUT_ROOM_SYN :			Out_Room_Syn(dwIndex, pMsg, dwLength) ;			break ;
	case MP_CHATROOM_CHANGE_OPTION_SYN :	Option_Syn(dwIndex, pMsg, dwLength) ;			break ;	
	case MP_CHATROOM_CHANGE_OWNER_SYN :		Owner_Syn(dwIndex, pMsg, dwLength) ;			break ;	
	case MP_CHATROOM_KICK_GUEST_SYN :		Kick_Syn(dwIndex, pMsg, dwLength) ;				break ;	
	case MP_CHATROOM_REQUEST_FRIEND_SYN :	Friend_Syn(dwIndex, pMsg, dwLength) ;			break ;	
	//case MP_CHATROOM_CHATMSG_NORMAL_SYN :	ChatMsg_Normal_Syn(dwIndex, pMsg, dwLength) ;	break ;	
	case MP_CHATROOM_UPDATE_USERINFO_SYN :	UpdateUserInfo(dwIndex, pMsg, dwLength) ;		break ;	
	case MP_CHATROOM_SEARCH_FOR_NAME_SYN :	SearchName_Syn(dwIndex, pMsg, dwLength) ;		break ;	
	case MP_CHATROOM_SEARCH_FOR_TITLE_SYN :	SearchTitle_Syn(dwIndex, pMsg, dwLength) ;		break ;	

	//--------------------------------------------------------------------------//
	//-------------------------------[ ADD FOR TEST ]---------------------------//
	//--------------------------------------------------------------------------//
	case MP_CHATROOM_CHEAT_CREATE_ROOM_SYN :	 Cheat_Create_Room_Syn(pMsg) ;				break ;	
	case MP_CHATROOM_CHEAT_REMOVE_ALL_ROOM_SYN : Cheat_Remove_All_Room_Syn() ;				break ;
	case MP_CHATROOM_CHEAT_CREATE_USER_SYN :	 Cheat_Create_User_Syn(pMsg) ;				break ;
	case MP_CHATROOM_CHEAT_REMOVE_ALL_USER_SYN : Cheat_Remove_All_User_Syn() ;				break ;
	case MP_CHATROOM_CHEAT_ADD_USER_TO_MY_ROOM_SYN : Add_Guest_To_MyRoom_Syn(pMsg) ;		break ;

	default : break ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : Add_GuestInfo_Syn
//		 : 
//	DESC : 유저 정보를 등록하는 함수.
//		 :
//		 : 1. 채팅중인 유저인지 체크.
//		 : 2. 유저 등록.
//		 : 3. 검색용 이름 등록.
//		 : 4. 결과 전송.
//		 :
//  DATE : APRIL 4, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Add_User_Syn(DWORD dwIndex, char* pMsg, DWORD dwLength) 
{
	// 함수 파라메터 체크.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// 원본 메시지 변환.
	MSG_CR_USER* pmsg = NULL ;
	pmsg = (MSG_CR_USER*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}



	// 채팅중인지 체크.
	M_USER::iterator it ;
	it = m_hmUser.find(pmsg->dwObjectID) ;
	if(it != m_hmUser.end()) return ;



	// 유저 정보 등록.
	pmsg->user.dwConnectionIdx = dwIndex ;
	m_hmUser.insert(std::make_pair(pmsg->user.dwPlayerID, pmsg->user)) ;

	// 결과 전송.
	MSGBASE msg ;
	memset(&msg, 0, sizeof(MSGBASE)) ;

	msg.Category	= MP_CHATROOM ;
	msg.Protocol	= MP_CHATROOM_ADD_USER_ACK ;
	msg.dwObjectID	= pmsg->dwObjectID ;

	g_Network.Send2Server(dwIndex, (char*)&msg, sizeof(MSGBASE)) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Force_Add_User_Ysn
//		 : 
//	DESC : 이미 등록되어 있는 유저 정보가 있던말던, 강제로 유저를 등록하고, 채팅방 리스트를 전송한다.
//		 : 
//		 : 1. 등록 된 유저인지 체크.
//		 :    1-1. 채팅중인지 체크.
//		 :	  1-2. 채팅방 받기.
//		 :    1-3. 유저 아웃 처리.
//		 :    1-4. 검색용 이름 삭제.
//		 :    1-5. 유저 정보 삭제.
//		 : 2. 유저 등록.
//		 : 3. 검색용 이름 등록.
//		 : 
//  DATE : APRIL 22, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Force_Add_User_Ysn(DWORD dwIndex, char* pMsg, DWORD dwLength) 
{
	// 함수 파라메터 체크.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// 원본 메시지 변환.
	MSG_CR_USER* pmsg = NULL ;
	pmsg = (MSG_CR_USER*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}



	// 유저 받기.
	M_USER::iterator it ;
	it = m_hmUser.find(pmsg->dwObjectID) ;
	if(it != m_hmUser.end())
	{
		// 방 받기.
		ST_CR_ROOM_SRV* pRoom = NULL ;
		pRoom = GetRoomInfo(it->second.byIdx) ;

		// 방 있으면,
		if(pRoom)
		{
			// 아웃 처리.
			MSGBASE msg ;
			memset(&msg, 0, sizeof(MSGBASE)) ;

			msg.Category	= MP_CHATROOM ;
			msg.Protocol	= MP_CHATROOM_OUT_ROOM_SYN ;
			msg.dwObjectID	= pmsg->dwObjectID ;

			Out_Room_Syn(dwIndex, (char*)&msg, sizeof(MSGBASE)) ;
		}

		// 유저 정보 삭제.
		m_hmUser.erase(it) ;
	}



	// 유저 등록.
	m_hmUser.insert(std::make_pair(pmsg->dwObjectID, pmsg->user)) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Del_GuestInfo_SynDWORD
//		 : 
//	DESC : 캐릭터가 로그 아웃을 했을 때, 유저 정보를 완전히 삭제 시키는 함수.
//		 : 
//		 : 1. 등록 된 유저인지 체크.
//		 :    1-1. 채팅중인지 체크.
//		 :	  1-2. 채팅방 받기.
//		 :    1-3. 유저 아웃 처리.
//		 : 2. 유저 삭제.
//		 : 3. 검색용 이름 삭제.
//		 : 
//  DATE : APRIL 22, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Del_User_Syn(DWORD dwIndex, char* pMsg, DWORD dwLength) 
{
	// 함수 파라메터 체크.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// 원본 메시지 변환.
	MSGBASE* pmsg = NULL ;
	pmsg = (MSGBASE*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}



	// 유저 받기.
	M_USER::iterator it ;
	it = m_hmUser.find(pmsg->dwObjectID) ;
	if(it != m_hmUser.end())
	{
		// 방 받기.
		ST_CR_ROOM_SRV* pRoom = NULL ;
		pRoom = GetRoomInfo(it->second.byIdx) ;

		// 방 있으면,
		if(pRoom)
		{
			// 아웃 처리.
			MSGBASE msg ;
			memset(&msg, 0, sizeof(MSGBASE)) ;

			msg.Category	= MP_CHATROOM ;
			msg.Protocol	= MP_CHATROOM_OUT_ROOM_SYN ;
			msg.dwObjectID	= pmsg->dwObjectID ;

			Out_Room_Syn(dwIndex, (char*)&msg, sizeof(MSGBASE)) ;
		}

		// 검색용 유저 이름 삭제.
		EraseUserName(it->second.name) ;

		// 유저 정보 삭제.
		m_hmUser.erase(it) ;
	}	
}





//-------------------------------------------------------------------------------------------------
//	NAME : RoomList_Syn
//		 : 
//	DESC : 요청이 들어온 페이지에 해당하는 방 정보를 담아서 Agent로 전송한다.
//		 : 
//		 : 1. 리스트 헤더 세팅.
//		 : 2. 채팅방 분류에 따라 방정보를 담는다.
//		 : 3. 페이지 세팅.
//		 : 4. 전송하는 리스트 분류 세팅.
//		 : 5. 결과 전송.
//		 : 
//  DATE : APRIL 22, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Room_Syn(DWORD dwIndex, char* pMsg, DWORD dwLength) 
{
	// 에러 메시지 정의.
	// 0 = err_Empty_Room,
	// 1 = err_Delay_Time,

	// 함수 파라메터 체크.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// 원본 메시지 변환.
	MSG_BYTE2* pmsg = NULL ;
	pmsg = (MSG_BYTE2*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}



	// 전송 메시지 세팅.
	MSG_CR_ROOMLIST msg ;
	memset(&msg, 0, sizeof(MSG_CR_ROOMLIST)) ;

	msg.Category	= MP_CHATROOM ;
	msg.Protocol	= MP_CHATROOM_ROOMLIST_ACK ;
	msg.dwObjectID	= pmsg->dwObjectID ;



	// 리스트 헤더 세팅.
	ST_CR_ROOM_SRV* pInfo ;
	PTRLISTPOS pos = NULL ;

	pos = m_RoomList.GetHeadPosition() ;

	// 방정보를 담을 범위 인덱스 세팅.
	BYTE byStartIdx = 0 ;
	byStartIdx = pmsg->bData2 * ROOM_COUNT_PER_PAGE ;

	BYTE byIndex = 0 ;



	// 리스트 검색.
	while(pos)
	{
		// 방정보 받기.
		pInfo = NULL ;
		pInfo = (ST_CR_ROOM_SRV*)m_RoomList.GetNext(pos) ;

		if(!pInfo) continue ;

		// 인덱스 유효 체크.
		if(byIndex < byStartIdx)
		{
			++byIndex ;
			continue ;
		}

		// 전송 할 개수 체크.
		if(msg.byCount >= ROOM_COUNT_PER_PAGE) break ;

		if(pmsg->bData1 != e_RTM_AllLooK)
		{
			if(pInfo->byRoomType != pmsg->bData1-1) continue ;
		}

		// 방 정보 담기.
		msg.room[msg.byCount].byIdx				= pInfo->byIdx ;
		msg.room[msg.byCount].bySecretMode		= pInfo->bySecretMode ;
		msg.room[msg.byCount].byRoomType		= pInfo->byRoomType ;
		msg.room[msg.byCount].byCurGuestCount	= pInfo->byCurGuestCount ;
		msg.room[msg.byCount].byTotalGuestCount = pInfo->byTotalGuestCount ;

		SafeStrCpy(msg.room[msg.byCount].title, pInfo->title, TITLE_SIZE) ;

		++msg.byCount ;
	}



	// 페이지 세팅.
	msg.byCurPage	= pmsg->bData2 ;
	if(pmsg->bData1 != e_RTM_AllLooK)
	{
		msg.byTotalPage	= GetRoomCountByType(pmsg->bData1-1) / ROOM_COUNT_PER_PAGE ;
	}
	else
	{
		msg.byTotalPage	= m_RoomList.GetCount() / ROOM_COUNT_PER_PAGE ;
	}

	// 담은 채팅방 분류 세팅.
	msg.byRoomType	= pmsg->bData1 ;



	// 결과 전송.
	g_Network.Send2Server(dwIndex, (char*)&msg, sizeof(MSG_CR_ROOMLIST)) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Create_Room_Syn
//		 : 
//	DESC : 채팅방 생성 요청이 왔을 때 처리하는 함수.
//		 : 
//		 : 1. 채팅방 개수 체크.
//		 : 2. 등록 된 유저인지 체크.
//		 : 3. 채팅중인지 체크.
//		 : 4. 채팅방 생성.
//		 : 5. 관리 리스트에 방 추가.
//		 : 6. 유저의 채팅방 번호 세팅.
//		 : 7. 결과 전송.
//		 :
//  DATE : APRIL 4, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Create_Room_Syn(DWORD dwIndex, char* pMsg, DWORD dwLength) 
{
	// 에러 메시지 정의.
	// 0 = err_Already_Chatting,
	// 1 = err_RoomCount_Over,
	// 2 = err_User_Is_Not_In_Lobby,
	// 3 = err_Registed_But_Not_Found_Room,
	// 4 = err_Registed_But_Not_Found_User,

	// 함수 파라메터 체크.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// 원본 메시지 변환.
	MSG_CR_ELEMENT* pmsg = NULL ;
	pmsg = (MSG_CR_ELEMENT*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}



	// 채팅방 개수 체크.
	if(m_RoomList.GetCount() >= MAX_ROOM_COUNT)
	{
		SendErrors(dwIndex, pmsg->dwObjectID, MP_CHATROOM_CREATE_ROOM_NACK, 1) ;
		return ;
	}

	// 등록 된 유저인지 체크.
	M_USER::iterator it ;
	it = m_hmUser.find(pmsg->dwObjectID) ;
	if(it == m_hmUser.end())
	{
		SendErrors(dwIndex, pmsg->dwObjectID, MP_CHATROOM_CREATE_ROOM_NACK, 2) ;
		return ;
	}

	// 채팅중인지 체크.
	if(it->second.byIdx != 0 )
	{
		SendErrors(dwIndex, pmsg->dwObjectID, MP_CHATROOM_CREATE_ROOM_NACK, 0) ;
		return ;
	}



	// 채팅방 생성.
	ST_CR_ROOM_SRV* pRoom = NULL ;
	pRoom = new ST_CR_ROOM_SRV ;

	ASSERT(pRoom) ;

	if(!pRoom)
	{
		Throw_Error("Failed to create chatroom !!", __FUNCTION__) ;
		return ;
	}

	// 기본 방 정보 세팅.
	pRoom->byIdx			= GetEmptyRoomIdx() ;
	pRoom->dwOwnerIdx		= pmsg->dwObjectID ;

	pRoom->bySecretMode		= pmsg->bySecretMode ;
	pRoom->byRoomType		= pmsg->byRoomType ;
	pRoom->byCurGuestCount	= 1 ;

	SafeStrCpy(pRoom->code, pmsg->code, SECRET_CODE_SIZE) ;

	// 최대 참여 인원 세팅.
	BYTE byTotalGuest = e_GC_20 ;
	switch(pmsg->byTotalGuestCount)
	{
	case e_GC_20 :	byTotalGuest = e_GC_20 ;	 break ;
	case e_GC_15 :	byTotalGuest = e_GC_15 ;	 break ;
	case e_GC_10 :	byTotalGuest = e_GC_10 ;	 break ;
	case e_GC_5 :	byTotalGuest = e_GC_5 ;	 break ;
	default : break ;
	}
	pRoom->byTotalGuestCount	= byTotalGuest ;

	// 방 제목 세팅.
	SafeStrCpy(pRoom->title, pmsg->title, TITLE_SIZE) ;

	// 참여자 아이디 세팅.
	pRoom->dwUser[0] = it->second.dwPlayerID ;



	// 관리 리스트에 방 추가.
	AddRoom(pRoom) ;

	// 유저의 채팅방 번호 세팅.
	it->second.byIdx = pRoom->byIdx ;

	// 검색용 이름 등록.
	InsertUserName(it->second.name) ;

	// 검색용 참여 정보 세팅.
	UpdateNameRoomIdx(it->second.name, pRoom->byIdx) ;



	// 결과 전송.
	MSG_CR_ROOMINFO msg ;
	memset(&msg, 0, sizeof(MSG_CR_ROOMINFO)) ;

	msg.Category	= MP_CHATROOM ;
	msg.Protocol	= MP_CHATROOM_CREATE_ROOM_ACK ;
	msg.dwObjectID	= pmsg->dwObjectID ;

	msg.room		= *pRoom ;
	msg.user[0]		= it->second ;

	g_Network.Send2Server(dwIndex, (char*)&msg, sizeof(MSG_CR_ROOMINFO)) ;



	// 다른 유저들에게 방이 추가되었음을 공지한다.
	//Add_Room_To_Client(dwIndex, pRoom) ;

	UpdateInfo_Created_Room(pRoom) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Join_Room_Syn
//		 : 
//	DESC : 채팅방 참여 요청이 들어왔을 때 처리하는 함수.
//		 : 
//		 : 1. 등록 된 유저인지 체크.
//		 : 2. 채팅중인지 체크.
//		 : 3. 채팅방 받기.
//		 : 4. 비밀 번호 체크.
//		 : 5. 채팅방의 수용 인원 체크.
//		 : 6. 현재 참여자 수 체크.
//		 : 7. 채팅방의 참여자 수 증가.
//		 : 8. 유저의 채팅방 인덱스 세팅.
//		 : 9. 참여 성공 메시지 처리.
//		 :10. 참여 공지 메시지 처리.
//		 : 
//  DATE : APRIL 4, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Join_Room_Syn(DWORD dwIndex, char* pMsg, DWORD dwLength) 
{
	// 에러 메시지 정의
    // 0 = err_Already_Chatting,
	// 1 = err_Invalid_Room_Info,
	// 2 = err_Invalid_Secret_Code,
	// 3 = err_Guest_Count_Is_Over,
	// 4 = err_Invalid_User_Info,

	// 함수 파라메터 체크.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// 원본 메시지 변환.
	MSG_CR_JOIN_SYN* pmsg = NULL ;
	pmsg = (MSG_CR_JOIN_SYN*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}



	// 등록 된 유저인지 체크.
	M_USER::iterator it ;
	it = m_hmUser.find(pmsg->dwObjectID) ;
	if(it == m_hmUser.end())
	{
		SendErrors(dwIndex, pmsg->dwObjectID, MP_CHATROOM_JOIN_ROOM_NACK, 4) ;
		return ;
	}

	// 채팅중인지 체크.
	if(it->second.byIdx != 0)
	{
		SendErrors(dwIndex, pmsg->dwObjectID, MP_CHATROOM_JOIN_ROOM_NACK, 0) ;
		return ;
	}

	// 채팅방 받기.
	// 080522 LYW --- ChatRoomMgr : 채팅방 정보 체크.
	if(pmsg->byRoomIdx == 0)
	{
		SendErrors(dwIndex, pmsg->dwObjectID, MP_CHATROOM_JOIN_ROOM_NACK, 1) ;
		return ;
	}

	ST_CR_ROOM_SRV* pRoom = NULL ;
	pRoom = GetRoomInfo(pmsg->byRoomIdx) ;
	if(!pRoom)
	{
		SendErrors(dwIndex, pmsg->dwObjectID, MP_CHATROOM_JOIN_ROOM_NACK, 1) ;
		return ;
	}

	// 비밀 번호 체크.
	if(pRoom->bySecretMode == e_RM_Close)
	{
		//if(pRoom->wCode != pmsg->wData2)
		if( strcmp(pRoom->code, pmsg->code) != 0 )
		{
			SendErrors(dwIndex, pmsg->dwObjectID, MP_CHATROOM_JOIN_ROOM_NACK, 2) ;
			return ;
		}
	}

	// 채팅방의 수용 인원 체크.
	BYTE byTotalCount = 0 ;
	switch(pRoom->byTotalGuestCount)
	{
	case e_GC_20 :	byTotalCount = 20 ; break ;
	case e_GC_15 :	byTotalCount = 15 ; break ;
	case e_GC_10 :	byTotalCount = 10 ; break ;
	case e_GC_5 :	byTotalCount =  5 ; break ;
	default : break ;
	}

	// 현재 참여자 수 체크.
	if(pRoom->byCurGuestCount >= byTotalCount)
	{
		SendErrors(dwIndex, pmsg->dwObjectID, MP_CHATROOM_JOIN_ROOM_NACK, 3) ;
		return ;
	}



	// 참여자 인덱스 세팅.
	pRoom->dwUser[pRoom->byCurGuestCount] = it->second.dwPlayerID ;

	// 채팅방의 참여자 수 증가.
	++pRoom->byCurGuestCount ;

	// 유저의 채팅방 인덱스 세팅.
	it->second.byIdx = pRoom->byIdx ;

	// 검색용 이름 등록.
	InsertUserName(it->second.name) ;

	// 유저가 참여하는 방 인덱스 업데이트.
	UpdateNameRoomIdx(it->second.name, pRoom->byIdx) ;



	// 참여 성공 메시지 처리.
	MSG_CR_ROOMINFO msg ;
	memset(&msg, 0, sizeof(MSG_CR_ROOMINFO)) ;

	msg.Category	= MP_CHATROOM ;
	msg.Protocol	= MP_CHATROOM_JOIN_ROOM_ACK ;
	msg.dwObjectID	= pmsg->dwObjectID ;

	msg.room		= *pRoom ;

	// 참여자 정보 담기.
	M_USER::iterator it_member ;
	for(BYTE count = 0 ; count < MAX_USER_PER_ROOM ; ++count)
	{
		it_member = m_hmUser.find(pRoom->dwUser[count]) ;
		if(it_member == m_hmUser.end()) continue ;

		msg.user[msg.byCount] = it_member->second ;

		++msg.byCount ;
	}
	// 결과 전송.
	g_Network.Send2Server(dwIndex, (char*)&msg, sizeof(MSG_CR_ROOMINFO)) ;



	// 참여 공지 메시지 처리.
	MSG_CR_JOIN_NOTICE msg2 ;
	memset(&msg2, 0, sizeof(MSG_CR_JOIN_NOTICE)) ;

	msg2.Category	= MP_CHATROOM ;
	msg2.Protocol	= MP_CHATROOM_JOIN_ROOM_NOTICE ;
	msg2.dwObjectID	= pmsg->dwObjectID ;

	msg2.user		= it->second ;

	// 참여자 인덱스 담기.
	for(BYTE count = 0 ; count < pRoom->byCurGuestCount ; ++count)
	{
		it_member = m_hmUser.find(pRoom->dwUser[count]) ;
		if(it_member == m_hmUser.end()) continue ;

		if(it_member->second.dwPlayerID == it->second.dwPlayerID) continue ;

		msg2.dwUser[msg2.byCount] = it_member->second.dwPlayerID ;

		++msg2.byCount ;
	}

	// 결과 전송.
	g_Network.Send2Server(dwIndex, (char*)&msg2, sizeof(MSG_CR_JOIN_NOTICE)) ;



	// 다른 유저들에게 방이 변경되었음을 공지한다.
	//Update_Room_To_Client(dwIndex, pRoom) ;
	UpdateInfo_Changed_Current_GuestCount(pRoom) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Out_Room_Syn
//		 : 
//	DESC : 유저가 채팅방을 나가는 처리를 하는 함수.
//		 : 
//		 : 1. 등록 된 유저인지 체크.
//		 : 2. 방정보 받기.
//		 : 3. 방에 남아있는 유저 수 체크.
//		 :	  3-1. 방 삭제.
//		 :	  3-2. 유저의 방 인덱스 삭제.
//		 :	  3-3. 결과 전송.
//		 : 4. 마지막이 아니면,
//		 :    4-1. 채팅방의 참여중인 유저수를 감소시킨다.
//		 :	  4-2. 나가는 유저의 방 인덱스를 초기화.
//		 :	  4-3. 나가기 성공 메시지 처리.
//		 :    4-4. 결과를 공지한다.
//		 :	  4-5. 방장이 나갔다면, 방장 바뀜 공지.
//		 : 
//  DATE : APRIL 4, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Out_Room_Syn(DWORD dwIndex, char* pMsg, DWORD dwLength) 
{

	// 에러 메시지 정의.
	// 0 = err_CanNot_Found_Room,
	// 1 = err_CanNot_Found_Outer,

	// 파라메터 체크.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// 원본 메시지 변환.
	MSG_DWORD* pmsg = NULL ;
	pmsg = (MSG_DWORD*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}



	// 유저 정보 받기.
	M_USER::iterator it ;
	it = m_hmUser.find(pmsg->dwObjectID) ;
	if(it == m_hmUser.end())
	{
		SendErrors(dwIndex, pmsg->dwObjectID, MP_CHATROOM_OUT_ROOM_NACK, 1) ;
		return ;
	}

	// 방정보 받기.
	ST_CR_ROOM_SRV* pRoom = NULL ;

	// 080522 LYW --- ChatroomMgr : 유저의 채팅방 인덱스 체크.
	if(it->second.byIdx == 0)
	{
		SendErrors(dwIndex, pmsg->dwObjectID, MP_CHATROOM_OUT_ROOM_NACK, 0) ;
		return ;
	}

	pRoom = GetRoomInfo(it->second.byIdx) ;
	if(!pRoom)
	{
		SendErrors(dwIndex, pmsg->dwObjectID, MP_CHATROOM_OUT_ROOM_NACK, 0) ;
		return ;
	}



	// 유저의 방 인덱스 삭제.
	it->second.byIdx = 0 ;

	// 검색용 유저 이름 삭제.
	EraseUserName(it->second.name) ;

	// 방에 남아있는 유저 수 체크.
	if(pRoom->byCurGuestCount == 1)
	{
		// 다른 유저들에게 방이 삭제되었음을 공지한다.
		//Delete_Room_To_Client(dwIndex, pRoom) ;
		UpdateInfo_Deleted_Room(pRoom) ;

		// 방 삭제.
		DeleteRoom(pRoom->byIdx) ;

		// 방이 비었다고, 그냥 에러 메시지 보내지 않고, 클라이언트의 채팅방 리스트를 모두 비우도록 하자.
		if( m_RoomList.GetCount() == 0 )
		{
			MSGBASE msg ;
			memset(&msg, 0, sizeof(MSGBASE)) ;

			msg.Category	= MP_CHATROOM ;
			msg.Protocol	= MP_CHATROOM_OUT_ROOM_EMPTYROOM ;
			msg.dwObjectID	= pmsg->dwObjectID ;

			g_Network.Send2Server(dwIndex, (char*)&msg, sizeof(MSGBASE)) ;
			return ;
		}
		else
		{
			// 전송 메시지 세팅.
			MSG_CR_ROOMLIST msg ;
			memset(&msg, 0, sizeof(MSG_CR_ROOMLIST)) ;

			msg.Category	= MP_CHATROOM ;
			msg.Protocol	= MP_CHATROOM_OUT_ROOM_LAST_MAN ;
			msg.dwObjectID	= pmsg->dwObjectID ;

			// 리스트 헤더 세팅.
			ST_CR_ROOM_SRV* pInfo ;
			PTRLISTPOS pos = NULL ;

			pos = m_RoomList.GetHeadPosition() ;

			// 방정보를 담을 범위 인덱스 세팅.
			BYTE byStartIdx = 0 ;
			byStartIdx = byStartIdx * ROOM_COUNT_PER_PAGE ;

			BYTE byIndex = 0 ;

			// 리스트 검색.
			while(pos)
			{
				// 방정보 받기.
				pInfo = NULL ;
				pInfo = (ST_CR_ROOM_SRV*)m_RoomList.GetNext(pos) ;

				if(!pInfo) continue ;

				// 인덱스 유효 체크.
				if(byIndex < byStartIdx) continue ;

				// 전송 할 개수 체크.
				if(msg.byCount >= ROOM_COUNT_PER_PAGE) break ;

				// 방 정보 담기.
				msg.room[msg.byCount].byIdx				= pInfo->byIdx ;
				msg.room[msg.byCount].bySecretMode		= pInfo->bySecretMode ;
				msg.room[msg.byCount].byRoomType		= pInfo->byRoomType ;
				msg.room[msg.byCount].byCurGuestCount	= pInfo->byCurGuestCount ;
				msg.room[msg.byCount].byTotalGuestCount = pInfo->byTotalGuestCount ;

				SafeStrCpy(msg.room[msg.byCount].title, pInfo->title, TITLE_SIZE) ;

				++msg.byCount ;
			}

			// 페이지 세팅.
			msg.byCurPage	= 0 ;
			msg.byTotalPage	= m_RoomList.GetCount() / ROOM_COUNT_PER_PAGE ;

			// 담은 채팅방 분류 세팅.
			msg.byRoomType	= e_RTM_AllLooK ;

			// 결과 전송.
			g_Network.Send2Server(dwIndex, (char*)&msg, sizeof(MSG_CR_ROOMLIST)) ;
		}
	}
	else
	{
		// 방장이 후임 방장 위임없이 튕기거나, 채팅을 종료했는지 체크.
		BYTE byChangedOwner = FALSE ;
		if(pRoom->dwOwnerIdx == it->second.dwPlayerID) byChangedOwner = TRUE ;

		// 유저 인덱스를 임시 보관한다.
		BYTE byCheckCount = 0 ;
		DWORD dwPlayerID[ MAX_USER_PER_ROOM ] = {0, } ;
		for(BYTE count = 0 ; count < pRoom->byCurGuestCount ; ++count)
		{
			if(pRoom->dwUser[count] == it->second.dwPlayerID) continue ;

			dwPlayerID[byCheckCount] = pRoom->dwUser[count] ;
			++byCheckCount ;
		}

		// 채팅방의 참여자 수 감소.
		--pRoom->byCurGuestCount ;

		// 유저 인덱스 복원
		for(BYTE count = 0 ; count  < pRoom->byCurGuestCount ; ++count)
		{
			pRoom->dwUser[count] = dwPlayerID[count] ;
		}

		// 후임 방장 결정.
		if(byChangedOwner)
		{
			pRoom->dwOwnerIdx = dwPlayerID[0] ;
		}

		// 다른 유저들에게 방이 변경되었음을 공지한다.
		//Update_Room_To_Client(dwIndex, pRoom) ;
		UpdateInfo_Changed_Current_GuestCount(pRoom) ;


		//// 나간사람 아웃 처리.
		//MSGBASE msg ;
		//memset(&msg, 0, sizeof(MSGBASE)) ;

		//msg.Category	= MP_CHATROOM ;
		//msg.Protocol	= MP_CHATROOM_OUT_ROOM_ACK ;
		//msg.dwObjectID	= pmsg->dwObjectID ;

		//g_Network.Send2Server(dwIndex, (char*)&msg, sizeof(MSGBASE)) ;

		// 전송 메시지 세팅.
		MSG_CR_ROOMLIST msg ;
		memset(&msg, 0, sizeof(MSG_CR_ROOMLIST)) ;

		msg.Category	= MP_CHATROOM ;
		msg.Protocol	= MP_CHATROOM_OUT_ROOM_ACK ;
		msg.dwObjectID	= pmsg->dwObjectID ;

		// 리스트 헤더 세팅.
		ST_CR_ROOM_SRV* pInfo ;
		PTRLISTPOS pos = NULL ;

		pos = m_RoomList.GetHeadPosition() ;

		// 방정보를 담을 범위 인덱스 세팅.
		BYTE byStartIdx = 0 ;
		byStartIdx = byStartIdx * ROOM_COUNT_PER_PAGE ;

		BYTE byIndex = 0 ;

		// 리스트 검색.
		while(pos)
		{
			// 방정보 받기.
			pInfo = NULL ;
			pInfo = (ST_CR_ROOM_SRV*)m_RoomList.GetNext(pos) ;

			if(!pInfo) continue ;

			// 인덱스 유효 체크.
			if(byIndex < byStartIdx) continue ;

			// 전송 할 개수 체크.
			if(msg.byCount >= ROOM_COUNT_PER_PAGE) break ;

			// 방 정보 담기.
			msg.room[msg.byCount].byIdx				= pInfo->byIdx ;
			msg.room[msg.byCount].bySecretMode		= pInfo->bySecretMode ;
			msg.room[msg.byCount].byRoomType		= pInfo->byRoomType ;
			msg.room[msg.byCount].byCurGuestCount	= pInfo->byCurGuestCount ;
			msg.room[msg.byCount].byTotalGuestCount = pInfo->byTotalGuestCount ;

			SafeStrCpy(msg.room[msg.byCount].title, pInfo->title, TITLE_SIZE) ;

			++msg.byCount ;
		}

		// 페이지 세팅.
		msg.byCurPage	= 0 ;
		msg.byTotalPage	= m_RoomList.GetCount() / ROOM_COUNT_PER_PAGE ;

		// 담은 채팅방 분류 세팅.
		msg.byRoomType	= e_RTM_AllLooK ;

		// 결과 전송.
		g_Network.Send2Server(dwIndex, (char*)&msg, sizeof(MSG_CR_ROOMLIST)) ;



		// 참여자 나감 공지 처리.
		MSG_CR_IDNAME msg2 ;
		memset(&msg2, 0, sizeof(MSG_CR_IDNAME)) ;

		msg2.Category	= MP_CHATROOM ;
		msg2.Protocol	= MP_CHATROOM_OUT_ROOM_NOTICE ;
		msg2.dwObjectID	= pmsg->dwObjectID ;

		msg2.dwID		= it->second.dwPlayerID ;

		SafeStrCpy(msg2.name, it->second.name, MAX_NAME_LENGTH) ;

		M_USER::iterator it_member ;
		for(BYTE count = 0 ; count < pRoom->byCurGuestCount ; ++count)
		{
			it_member = m_hmUser.find(pRoom->dwUser[count]) ;
			if(it_member == m_hmUser.end()) continue ;

			msg2.dwUser[msg2.byCount] = it_member->second.dwPlayerID ;

			++msg2.byCount ;
		}

		g_Network.Send2Server(dwIndex, (char*)&msg2, sizeof(MSG_CR_IDNAME)) ;



		// 방장 바뀜 공지 처리.
		if(!byChangedOwner) return ;

		MSG_CR_IDNAME msg3 ;
		memset(&msg3, 0, sizeof(MSG_CR_IDNAME)) ;

		msg3.Category	= MP_CHATROOM ;
		msg3.Protocol	= MP_CHATROOM_OUT_ROOM_CHANGE_OWNER_NOTICE ;
		msg3.dwObjectID	= pmsg->dwObjectID ;

		for(BYTE count = 0 ; count < pRoom->byCurGuestCount ; ++count)
		{
			it_member = m_hmUser.find(pRoom->dwUser[count]) ;
			if(it_member == m_hmUser.end()) continue ;

			if(it_member->second.dwPlayerID == pRoom->dwOwnerIdx)
			{
				msg3.dwID = it_member->second.dwPlayerID ;
				SafeStrCpy(msg3.name, it_member->second.name, MAX_NAME_LENGTH) ;
			}

			msg3.dwUser[msg3.byCount] = it_member->second.dwPlayerID ;

			++msg3.byCount ;
		}

		g_Network.Send2Server(dwIndex, (char*)&msg3, sizeof(MSG_CR_IDNAME)) ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : Option_Syn
//		 : 
//	DESC : The function to change option of selected room.
//		 : 
//		 : 1. 등록 된 유저인지 체크.
//		 : 2. 방 정보 받기.
//		 : 3. 방장인지 검사.
//		 : 4. 방 옵션 변경.
//		 : 5. 결과 공지.
//		 : 
//  DATE : APRIL 4, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Option_Syn(DWORD dwIndex, char* pMsg, DWORD dwLength) 
{
	// 에러 메시지 정의.
	// 0 = err_You_Are_Not_In_ChatRoom,
	// 1 = err_CanNot_Found_Room,
	// 2 = err_You_Are_Not_Owner,

	// 함수 파라메터 체크.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// 원본 메시지 변환.
	MSG_CR_ELEMENT* pmsg = NULL ;
	pmsg = (MSG_CR_ELEMENT*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}



	// 등록 된 유저인지 체크.
	M_USER::iterator it ;
	it = m_hmUser.find(pmsg->dwObjectID) ;
	if(it == m_hmUser.end())
	{
		SendErrors(dwIndex, pmsg->dwObjectID, MP_CHATROOM_CHANGE_OPTION_NACK, 0) ;
		return ;
	}

	// 방정보 받기.
	ST_CR_ROOM_SRV* pRoom = NULL ;
	pRoom = GetRoomInfo(it->second.byIdx) ;
	if(!pRoom)
	{
		SendErrors(dwIndex, pmsg->dwObjectID, MP_CHATROOM_CHANGE_OPTION_NACK, 1) ;
		return ;
	}

    // 방장인지 검사.
	if(it->second.dwPlayerID != pRoom->dwOwnerIdx)
	{
		SendErrors(dwIndex, pmsg->dwObjectID, MP_CHATROOM_CHANGE_OPTION_NACK, 2) ;
		return ;
	}


	
	// 채팅방 변경 내용 공지 처리.
	// 다른 유저들에게 방이 변경되었음을 공지한다.
	BYTE byChangedSecretMode = FALSE ;
	if(pRoom->bySecretMode != pmsg->bySecretMode)
	{
		byChangedSecretMode = TRUE ;
	}

	BYTE byChangedRoomType = FALSE ;
	if(pRoom->byRoomType != pmsg->byRoomType)
	{
		byChangedRoomType = TRUE ;
	}

	BYTE byChangedGuestCount = FALSE ;
	if(pRoom->byTotalGuestCount != pmsg->byTotalGuestCount)
	{
		byChangedGuestCount = TRUE ;
	}

	BYTE byChangedTitle = FALSE ;
	if(strcmp(pRoom->title, pmsg->title) != 0)
	{
		byChangedTitle = TRUE ;
	}



	// 방 옵션 변경.
	pRoom->bySecretMode			= pmsg->bySecretMode ;
	pRoom->byRoomType			= pmsg->byRoomType ;
	pRoom->byTotalGuestCount	= pmsg->byTotalGuestCount ;

	//pRoom->wCode				= pmsg->wCode ;
	SafeStrCpy(pRoom->code, pmsg->code, SECRET_CODE_SIZE) ;

	SafeStrCpy(pRoom->title, pmsg->title, TITLE_SIZE) ;

	// 검색용 방 정보 업데이트.
	UpdateSearchRoomInfo(pRoom) ;



	// 결과 공지.
	MSG_CR_ROOM_NOTICE msg ;
	memset(&msg, 0, sizeof(MSG_CR_ROOM_NOTICE)) ;

	msg.Category	= MP_CHATROOM ;
	msg.Protocol	= MP_CHATROOM_CHANGE_OPTION_NOTICE ;
	msg.dwObjectID	= pmsg->dwObjectID ;

	msg.room		= *pRoom ;

	M_USER::iterator it_member ;
	for(BYTE count = 0 ; count < pRoom->byCurGuestCount ; ++count)
	{
		it_member = m_hmUser.find(pRoom->dwUser[count]) ;
		if(it_member == m_hmUser.end()) continue ;

		msg.dwUser[msg.byCount] = it_member->second.dwPlayerID ;

		++msg.byCount ;
	}

	g_Network.Send2Server(dwIndex, (char*)&msg, sizeof(MSG_CR_ROOM_NOTICE)) ;



	// 변경 된 정보를 공지 처리.
	if(byChangedSecretMode)
	{
		UpdateInfo_Changed_SecretMode(pRoom) ;
	}
	
	if(byChangedRoomType)
	{
		UpdateInfo_Changed_RoomType(pRoom) ;
	}
	
	if(byChangedGuestCount)
	{
		UpdateInfo_Changed_Total_GuestCount(pRoom) ;
	}
	
	if(byChangedTitle)
	{
		UpdateInfo_Changed_Title(pRoom) ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : Owner_Syn
//		 : 
//	DESC : The function to change owner of chatroom.
//		 : 
//		 : 1. 현재 방장 받기.
//		 : 2. 후임 방장 받기.
//		 : 3. 방정보 받기.
//		 : 4. 현재 방장이 방장인지 체크.
//		 : 5. 현재 참여중인 유저 수 체크.
//		 : 6. 채팅방의 방장인덱스 변경.
//		 : 7. 결과 공지.
//		 : 
//  DATE : APRIL 4, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Owner_Syn(DWORD dwIndex, char* pMsg, DWORD dwLength)
{
	// 에러 메시지 정의.
	// 0 = err_Invalid_Current_Owner,
	// 1 = err_Invalid_Next_Owner,
	// 2 = err_CanNot_Found_RoomInfo,
	// 3 = err_You_Are_Not_Owner,
	// 4 = err_You_Alone,

	// 함수 파라메터 체크.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// 원본 메시지 변환.
	MSG_DWORD* pmsg = NULL ;
	pmsg = (MSG_DWORD*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}



	// 현재 방장 받기.
	M_USER::iterator it_owner ;
	it_owner = m_hmUser.find(pmsg->dwObjectID) ;
	if(it_owner == m_hmUser.end())
	{
		SendErrors(dwIndex, pmsg->dwObjectID, MP_CHATROOM_CHANGE_OWNER_NACK, 0) ;
		return ;
	}

	// 후임 방장 받기.
	M_USER::iterator it_new ;
	it_new = m_hmUser.find(pmsg->dwData) ;
	if(it_new == m_hmUser.end())
	{
		SendErrors(dwIndex, pmsg->dwObjectID, MP_CHATROOM_CHANGE_OWNER_NACK, 1) ;
		return ;
	}

	// 방정보 받기.
	ST_CR_ROOM_SRV* pRoom = NULL ;
	pRoom = GetRoomInfo(it_owner->second.byIdx) ;
	if(!pRoom)
	{
		SendErrors(dwIndex, pmsg->dwObjectID, MP_CHATROOM_CHANGE_OWNER_NACK, 2) ;
		return ;
	}

	// 현재 방장이 방장인지 체크.
	if(pRoom->dwOwnerIdx != it_owner->second.dwPlayerID)
	{
		SendErrors(dwIndex, pmsg->dwObjectID, MP_CHATROOM_CHANGE_OWNER_NACK, 3) ;
		return ;
	}

	// 현재 참여중인 유저 수 체크.
	if(pRoom->byCurGuestCount <= 1)
	{
		SendErrors(dwIndex, pmsg->dwObjectID, MP_CHATROOM_CHANGE_OWNER_NACK, 4) ;
		return ;
	}



	// 채팅방의 방장 아이디 변경.
	pRoom->dwOwnerIdx = it_new->second.dwPlayerID ;



	// 결과 공지.
	MSG_CR_IDNAME msg ;
	memset(&msg, 0, sizeof(MSG_CR_IDNAME)) ;

	msg.Category	= MP_CHATROOM ;
	msg.Protocol	= MP_CHATROOM_CHANGE_OWNER_NOTICE ;
	msg.dwObjectID	= pmsg->dwObjectID ;

	msg.dwID		= it_new->second.dwPlayerID ;

	SafeStrCpy(msg.name, it_new->second.name, MAX_NAME_LENGTH) ;

	M_USER::iterator it_member ;
	for(BYTE count = 0 ; count < pRoom->byCurGuestCount ; ++count)
	{
		it_member = m_hmUser.find(pRoom->dwUser[count]) ;
		if(it_member == m_hmUser.end()) continue ;

		msg.dwUser[msg.byCount] = it_member->second.dwPlayerID ;

		++msg.byCount ;
	}

	// 결과 전송.
	g_Network.Send2Server(dwIndex, (char*)&msg, sizeof(MSG_CR_IDNAME)) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Kick_Syn
//		 : 
//	DESC : The function to kick selected user from chatroom.
//		 : 
//		 : 1. 방장 받기.
//		 : 2. 방정보 받기.
//		 : 3. 방장인지 체크.
//		 : 4. 강제 퇴장 대상 받기.
//		 : 5. 대상의 방 인덱스 초기화.
//		 : 6. 채팅방의 접속 유저수 감소.
//		 : 7. 강퇴 처리.
//		 : 8. 강퇴 공지 처리.
//		 : 
//  DATE : APRIL 4, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Kick_Syn(DWORD dwIndex, char* pMsg, DWORD dwLength)
{
	// 에러 메시지 정의.
	// 0 = err_CanNot_Found_RoomInfo,
	// 1 = err_Invalid_OwnerInfo,
	// 2 = err_You_Are_Not_Owner,
	// 3 = err_Invalid_Target,

	// 함수 파라메터 체크.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// 원본 메시지 변환.
	MSG_DWORD* pmsg = NULL ;
	pmsg = (MSG_DWORD*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}



	// 방장 받기.
	M_USER::iterator it_owner ;
	it_owner = m_hmUser.find(pmsg->dwObjectID) ;
	if(it_owner == m_hmUser.end())
	{
		SendErrors(dwIndex, pmsg->dwObjectID, MP_CHATROOM_KICK_GUEST_NACK, 1) ;
		return ;
	}

	// 방정보 받기.
	ST_CR_ROOM_SRV* pRoom = NULL ;
	pRoom = GetRoomInfo(it_owner->second.byIdx) ;
	if(!pRoom)
	{
		SendErrors(dwIndex, pmsg->dwObjectID, MP_CHATROOM_KICK_GUEST_NACK, 0) ;
		return ;
	}

	// 방장인지 체크.
	if(it_owner->second.dwPlayerID != pRoom->dwOwnerIdx)
	{
		SendErrors(dwIndex, pmsg->dwObjectID, MP_CHATROOM_KICK_GUEST_NACK, 2) ;
		return ;
	}

	// 강제 퇴장 대상 받기.
	M_USER::iterator it_kick ;
	it_kick = m_hmUser.find(pmsg->dwData) ;
	if(it_kick == m_hmUser.end())
	{
		SendErrors(dwIndex, pmsg->dwObjectID, MP_CHATROOM_KICK_GUEST_NACK, 3) ;
		return ;
	}



	// 대상의 방인덱스 초기화.
	it_kick->second.byIdx = 0 ;

	// 검색용 유저 이름 삭제.
	EraseUserName(it_kick->second.name) ;



	// 유저 인덱스를 임시 보관한다.
	BYTE byCheckCount = 0 ;
	DWORD dwPlayerID[ MAX_USER_PER_ROOM ] = {0, } ;
	for(BYTE count = 0 ; count < pRoom->byCurGuestCount ; ++count)
	{
		if(pRoom->dwUser[count] == it_kick->second.dwPlayerID) continue ;

		dwPlayerID[byCheckCount] = pRoom->dwUser[count] ;
		++byCheckCount ;
	}

	// 채팅방의 참여자 수 감소.
	--pRoom->byCurGuestCount ;

	// 유저 인덱스 복원
	for(BYTE count = 0 ; count  < pRoom->byCurGuestCount ; ++count)
	{
		pRoom->dwUser[count] = dwPlayerID[count] ;
	}



	//// 강퇴처리.
	//MSGBASE msg ;
	//memset(&msg, 0, sizeof(MSGBASE)) ;

	//msg.Category	= MP_CHATROOM ;
	//msg.Protocol	= MP_CHATROOM_KICK_GUEST_ACK ;
	//msg.dwObjectID	= it_kick->second.dwPlayerID ;

	//g_Network.Send2Server(dwIndex, (char*)&msg, sizeof(MSGBASE)) ;

	// 전송 메시지 세팅.
	MSG_CR_KICK_ACK msg ;
	memset(&msg, 0, sizeof(MSG_CR_KICK_ACK)) ;

	msg.Category	= MP_CHATROOM ;
	msg.Protocol	= MP_CHATROOM_KICK_GUEST_ACK ;
	msg.dwObjectID	= pmsg->dwObjectID ;

	msg.dwKickPlayer = pmsg->dwData ;

	// 리스트 헤더 세팅.
	ST_CR_ROOM_SRV* pInfo ;
	PTRLISTPOS pos = NULL ;

	pos = m_RoomList.GetHeadPosition() ;

	// 방정보를 담을 범위 인덱스 세팅.
	BYTE byStartIdx = 0 ;
	byStartIdx = byStartIdx * ROOM_COUNT_PER_PAGE ;

	BYTE byIndex = 0 ;

	// 리스트 검색.
	while(pos)
	{
		// 방정보 받기.
		pInfo = NULL ;
		pInfo = (ST_CR_ROOM_SRV*)m_RoomList.GetNext(pos) ;

		if(!pInfo) continue ;

		// 인덱스 유효 체크.
		if(byIndex < byStartIdx) continue ;

		// 전송 할 개수 체크.
		if(msg.byCount >= ROOM_COUNT_PER_PAGE) break ;

		// 방 정보 담기.
		msg.room[msg.byCount].byIdx				= pInfo->byIdx ;
		msg.room[msg.byCount].bySecretMode		= pInfo->bySecretMode ;
		msg.room[msg.byCount].byRoomType		= pInfo->byRoomType ;
		msg.room[msg.byCount].byCurGuestCount	= pInfo->byCurGuestCount ;
		msg.room[msg.byCount].byTotalGuestCount = pInfo->byTotalGuestCount ;

		SafeStrCpy(msg.room[msg.byCount].title, pInfo->title, TITLE_SIZE) ;

		++msg.byCount ;
	}

	// 페이지 세팅.
	msg.byCurPage	= 0 ;
	msg.byTotalPage	= m_RoomList.GetCount() / ROOM_COUNT_PER_PAGE ;

	// 담은 채팅방 분류 세팅.
	msg.byRoomType	= e_RTM_AllLooK ;

	// 결과 전송.
	g_Network.Send2Server(dwIndex, (char*)&msg, sizeof(MSG_CR_KICK_ACK)) ;



	// 강퇴 공지 처리.
	MSG_CR_IDNAME msg2 ;
	memset(&msg2, 0, sizeof(MSG_CR_IDNAME)) ;

	msg2.Category	= MP_CHATROOM ;
	msg2.Protocol	= MP_CHATROOM_KICK_GUEST_NOTICE ;
	msg2.dwObjectID	= pmsg->dwObjectID ;

	// 강퇴 대상자 정보 세팅.
	msg2.dwID		= it_kick->second.dwPlayerID ;
	SafeStrCpy(msg2.name, it_kick->second.name, MAX_NAME_LENGTH) ;

	// 공지 대상자 담기.
	M_USER::iterator it_member ;
	for(BYTE count = 0 ; count < MAX_USER_PER_ROOM ; ++count)				
	{
		it_member = m_hmUser.find(pRoom->dwUser[count]) ;
		if(it_member == m_hmUser.end()) continue ;

		// 공지 대상에서 강퇴자는 뺀다.
		if(it_member->second.dwPlayerID == pmsg->dwData) continue ;	

		msg2.dwUser[msg2.byCount] = it_member->second.dwPlayerID ;

		++msg2.byCount ;
	}

	// 결과 전송.
	g_Network.Send2Server(dwIndex, (char*)&msg2, sizeof(MSG_CR_IDNAME)) ;



	// 다른 유저들에게 방이 변경되었음을 공지한다.
	//Update_Room_To_Client(dwIndex, pRoom) ;
	UpdateInfo_Changed_Current_GuestCount(pRoom) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Friend_Syn
//		 : 
//	DESC : The function to check user info for add friend.
//		 : 
//		 : 1. 요청자 받기.
//		 : 2. 대상 받기.
//		 : 3. 같은 채팅방인지 체크.
//		 : 4. 결과 전송.
//		 : 
//  DATE : APRIL 4, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Friend_Syn(DWORD dwIndex, char* pMsg, DWORD dwLength)
{
	// 에러 메시지 정의.
	// 0 = err_Invalid_Requester_Info,
	// 1 = err_Invalid_Target_Info.
	// 2 = err_You_Are_Not_In_SameRoom,

	// 함수 파라메터 체크.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// 원본 메시지 변환.
	MSG_DWORD* pmsg = NULL ;
	pmsg = (MSG_DWORD*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}



	// 요청자 받기.
	M_USER::iterator it_first ;
	it_first = m_hmUser.find(pmsg->dwObjectID) ;
	if(it_first == m_hmUser.end())
	{
		SendErrors(dwIndex, pmsg->dwObjectID, MP_CHATROOM_REQUEST_FRIEND_NACK, 0) ;
		return ;
	}

	// 대상 받기.
	M_USER::iterator it_second ;
	it_second = m_hmUser.find(pmsg->dwData) ;
	if(it_second == m_hmUser.end())
	{
		SendErrors(dwIndex, pmsg->dwObjectID, MP_CHATROOM_REQUEST_FRIEND_NACK, 1) ;
		return ;
	}

	// 같은 채팅방인지 체크.
	if(it_first->second.byIdx != it_second->second.byIdx)
	{
		SendErrors(dwIndex, pmsg->dwObjectID, MP_CHATROOM_REQUEST_FRIEND_NACK, 2) ;
		return ;
	}



	// 결과 메시지 전송.
	MSG_NAME msg ;
	memset(&msg, 0, sizeof(MSG_NAME)) ;

	msg.Category	= MP_CHATROOM ;
	msg.Protocol	= MP_CHATROOM_REQUEST_FRIEND_ACK ;
	msg.dwObjectID	= pmsg->dwObjectID ;

	SafeStrCpy(msg.Name, it_second->second.name, MAX_NAME_LENGTH+1) ;

	g_Network.Send2Server(dwIndex, (char*)&msg, sizeof(MSG_NAME)) ;
}





////-------------------------------------------------------------------------------------------------
////	NAME : ChatMsg_Normal_Syn
////		 : 
////	DESC : The function to check sender info and room info.
////		 : 
////		 : 1. 보낸사람 받기.
////		 : 2. 방 받기.
////		 : 3. 메시지 담고 결과 공지.
////		 : 
////  DATE : APRIL 7, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomMgr::ChatMsg_Normal_Syn(DWORD dwIndex, char*pMsg, DWORD dwLength)
//{
//	// 에러 메시지 정의.
//	// 0 = err_Invalid_Room_Info,
//	// 1 = err_Invalid_Sender_Info,
//
//	// 함수 파라메터 체크.
//	ASSERT(pMsg) ;
//
//	if(!pMsg)
//	{
//		Throw_Error(err_IMP, __FUNCTION__) ;
//		return ;
//	}
//
//	// 원본 메시지 변환.
//	MSG_CR_MSG* pmsg = NULL ;
//	pmsg = (MSG_CR_MSG*)pMsg ;
//
//	ASSERT(pmsg) ;
//
//	if(!pmsg)
//	{
//		Throw_Error(err_FCMTO, __FUNCTION__) ;
//		return ;
//	}
//
//	// 보낸사람 받기.
//	M_USER::iterator it ;
//	it = m_hmUser.find(pmsg->dwObjectID) ;
//	if(it == m_hmUser.end())
//	{
//		SendErrors(dwIndex, pmsg->dwObjectID, MP_CHATROOM_CHATMSG_NORMAL_NACK, 1) ;
//		return ;
//	}
//
//	// 방 받기.
//	ST_CR_ROOM_SRV* pRoom = NULL ;
//	pRoom = GetRoomInfo(it->second.byIdx) ;
//	if(!pRoom)
//	{
//		SendErrors(dwIndex, pmsg->dwObjectID, MP_CHATROOM_CHATMSG_NORMAL_NACK, 0) ;
//		return ;
//	}
//
//	// 메시지 전송.
//	MSG_CR_MSG_RESULT msg ;
//	memset(&msg, 0, sizeof(MSG_CR_MSG_RESULT)) ;
//
//	msg.Category		= MP_CHATROOM ;
//	msg.Protocol		= MP_CHATROOM_CHATMSG_NORMAL_NOTICE ;
//	msg.dwObjectID		= pmsg->dwObjectID ;
//
//	SafeStrCpy(msg.name, it->second.name, MAX_NAME_LENGTH) ;	
//	SafeStrCpy(msg.Msg, pmsg->Msg, MAX_CHAT_LENGTH) ;
//
//	for(BYTE count = 0 ; count < pRoom->byCurGuestCount ; ++count)
//	{
//		it = m_hmUser.find(pRoom->dwUser[count]) ;
//		if(it == m_hmUser.end()) continue ;
//
//		msg.dwUser[msg.byCount] = it->second.dwPlayerID ;
//
//		++msg.byCount ;
//	}
//
//	g_Network.Send2Server(dwIndex, (char*)&msg, sizeof(MSG_CR_MSG_RESULT)) ;
//}





//-------------------------------------------------------------------------------------------------
//	NAME : UpdateUserInfo
//	DESC : 유저의 정보가 업데이트 되었을 때, 정보를 업데이트 하는 함수.
//  DATE : APRIL 2, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::UpdateUserInfo(DWORD dwIndex, char*pMsg, DWORD dwLength)
{
	// 함수 파라메터 체크.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// 원본 메시지 변환.
	MSG_CR_USER* pmsg = NULL ;
	pmsg = (MSG_CR_USER*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}


	// 유저 정보 업데이트.
	M_USER::iterator it ;
	it = m_hmUser.find(pmsg->dwObjectID) ;
	if(it != m_hmUser.end())
	{
		// 검색용 이름 체크.
		if(strcmp(it->second.name, pmsg->user.name) !=0)
		{
			EraseUserName(it->second.name) ;

			//it->second.wClassIdx = pmsg->user.wClassIdx ;

			it->second.byLevel	= pmsg->user.byLevel ;
			it->second.byMapNum	= pmsg->user.byMapNum ;

			SafeStrCpy(it->second.name, pmsg->user.name, MAX_NAME_LENGTH) ;

			InsertUserName(it->second.name) ;
			UpdateNameRoomIdx(it->second.name, it->second.byIdx) ;
		}
		else
		{
			//it->second.wClassIdx = pmsg->user.wClassIdx ;

			it->second.byLevel	= pmsg->user.byLevel ;
			it->second.byMapNum	= pmsg->user.byMapNum ;

			SafeStrCpy(it->second.name, pmsg->user.name, MAX_NAME_LENGTH) ;

			UpdateNameRoomIdx(it->second.name, it->second.byIdx) ;
		}
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : SearchName_Syn
//	DESC : 이름으로 채팅방 정보를 검색하는 함수.
//  DATE : APRIL 23, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::SearchName_Syn(DWORD dwIndex, char*pMsg, DWORD dwLength) 
{
	// 에러 메시지 정의.
	// 0 = err_CanNot_Found_User,
	// 1 = err_CanNot_Found_Name,
	// 2 = err_ChatRoom_Destroyed,

	// 함수 파라메터 체크.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// 원본 메시지 변환.
	MSG_CR_SEARCH_NAME* pmsg = NULL ;
	pmsg = (MSG_CR_SEARCH_NAME*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}



	// 유저 검색.
	M_USER::iterator it_user ;
	it_user = m_hmUser.find(pmsg->dwObjectID) ;
	if(it_user == m_hmUser.end())
	{
		SendErrors(dwIndex, pmsg->dwObjectID, MP_CHATROOM_SEARCH_FOR_NAME_NACK, 0) ;
		return ;
	}

	//// 이름 검색.
	const char* name = pmsg->name ;
	M_NAME::iterator it_name ;
	it_name = m_Name.find(GetHashCode(name)) ;
	if(it_name == m_Name.end())
	{
		SendErrors(dwIndex, pmsg->dwObjectID, MP_CHATROOM_SEARCH_FOR_NAME_NACK, 1) ;
		return ;
	}

	// 방 검색.
	ST_CR_ROOM_SRV* pRoom = NULL ;
	pRoom = GetRoomInfo(it_name->second) ;

	if(!pRoom)
	{
		SendErrors(dwIndex, pmsg->dwObjectID, MP_CHATROOM_SEARCH_FOR_NAME_NACK, 2) ;
		return ;
	}



	// 결과 전송.
	MSG_CR_SEARCH_NAME msg ;
	memset(&msg, 0, sizeof(MSG_CR_SEARCH_NAME)) ;

	msg.Category	= MP_CHATROOM ;
	msg.Protocol	= MP_CHATROOM_SEARCH_FOR_NAME_ACK ;
	msg.dwObjectID	= pmsg->dwObjectID ;

	msg.byRoomIdx	= pRoom->byIdx ;

	SafeStrCpy(msg.name, pmsg->name, MAX_NAME_LENGTH) ;

	g_Network.Send2Server(dwIndex, (char*)&msg, sizeof(MSG_CR_SEARCH_NAME)) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : SearchTitle_Syn
//	DESC : 제목으로 채팅방 정보를 검색하는 함수.
//  DATE : APRIL 23, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::SearchTitle_Syn(DWORD dwIndex, char*pMsg, DWORD dwLength)
{
	// 에러 메시지 정의.
	// 0 = err_CanNot_Found_Room,

	// 함수 파라메터 체크.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// 원본 메시지 변환.
	MSG_CR_SEARCH_TITLE* pmsg = NULL ;
	pmsg = (MSG_CR_SEARCH_TITLE*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}



	// 결과 메시지 세팅.
	MSG_CR_ROOMLIST msg ;
	memset(&msg, 0, sizeof(MSG_CR_ROOMLIST)) ;

	msg.Category	= MP_CHATROOM ;
	msg.Protocol	= MP_CHATROOM_SEARCH_FOR_TITLE_ACK ;
	msg.dwObjectID	= pmsg->dwObjectID ;



	// 제목으로 채팅방 검색.
	ST_CR_ROOM_CLT* pInfo ;

	PTRLISTPOS pos = NULL ;
	pos = m_Title.GetHeadPosition() ;

	BYTE byCount = 0 ;

	while(pos)
	{
		pInfo = NULL ;
		pInfo = (ST_CR_ROOM_CLT*)m_Title.GetNext(pos) ;

		if(!pInfo) continue ;

		if( strstr(pInfo->title, pmsg->title) == NULL ) continue ;

		++byCount ;

		if(msg.byCount >= MAX_USER_PER_ROOM) continue ;

		msg.room[msg.byCount] = *pInfo ;

		++msg.byCount ;
	}



	// 검색 된 개수 체크.
	if(msg.byCount == 0)
	{
		SendErrors(dwIndex, pmsg->dwObjectID, MP_CHATROOM_SEARCH_FOR_TITLE_NACK, 0) ;
		return ;
	}

	// 검색모드로 세팅.
	msg.byRoomType = e_RTM_Searched ;

	// 페이지 세팅.
	msg.byCurPage	= 0 ;
	msg.byTotalPage = byCount / ROOM_COUNT_PER_PAGE ;

	// 결과 전송.
	g_Network.Send2Server(dwIndex, (char*)&msg, sizeof(MSG_CR_ROOMLIST)) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : GetEmptyRoomIdx
//	DESC : 채팅방을 관리하는 맵에서, 앞에서 부터 비어있는 방 인덱스를 반환하는 함수. 0번은 제외.
//  DATE : APRIL 14, 2008 LYW
//-------------------------------------------------------------------------------------------------
BYTE CChatRoomMgr::GetEmptyRoomIdx()
{
	for(BYTE count = 1 ; count < MAX_ROOM_COUNT ; ++count)
	{
		if(m_byRoomIdx[count] == 0) return count ;
	}

	ASSERT(TRUE) ;

	Throw_Error("Cant found empty room index!!", __FUNCTION__) ;

	return 0 ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : GetHashCode
//	DESC : 인자로 주어진 스트링을 해쉬맵에 사용할 코드 값으로 변환하는 함수.
//  DATE : APRIL 24, 2008 LYW
//-------------------------------------------------------------------------------------------------
DWORD CChatRoomMgr::GetHashCode( const char* pName )
{
	// 함수 파라메터 체크.
	ASSERT(pName) ;

	if(!pName || strlen(pName) == 0)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return 5381 ;
	}

	DWORD dwCount	= 0 ;
	DWORD dwLength	= 0 ;
	DWORD dwCh		= 0 ;
	DWORD dwResult	= 0 ;

	dwLength = strlen( pName ) ;
	dwResult = 5381 ;

	for( dwCount = 0 ; dwCount < dwLength ; dwCount++ )
	{
		dwCh = (DWORD)pName[ dwCount ] ;
		dwResult = ((dwResult << 5) + dwResult) + dwCh ; // hash * 33 + ch
	}	  

	return dwResult ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : InsertUserName
//	DESC : 검색용 해쉬맵에 유저 이름을 추가하는 함수.
//  DATE : APRIL 24, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::InsertUserName(char* pName ) 
{
	// 함수 파라메터 체크.
	ASSERT(pName) ;

	if(!pName || strlen(pName) == 0)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	const char* name = pName ;
	m_Name.insert(std::make_pair(GetHashCode(name), 0)) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : EraseUserName
//	DESC : 검색용 해쉬맵의 유저 이름을 삭제하는 함수.
//  DATE : APRIL 24, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::EraseUserName(char* pName )
{
	// 함수 파라메터 체크.
	ASSERT(pName) ;

	if(!pName || strlen(pName) == 0)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	M_NAME::iterator it ;
	const char* name = pName ;

	it = m_Name.find(GetHashCode(name)) ;

	if(it != m_Name.end())
	{
		m_Name.erase(it) ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : UpdateNameRoomIdx
//	DESC : 검색용 해쉬맵의 유저가 참여한 방 번호를 업데이트 하는 함수.
//  DATE : APRIL 24, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::UpdateNameRoomIdx(char* pName, BYTE byRoomIdx)
{
	// 함수 파라메터 체크.
	ASSERT(pName) ;

	if(!pName || strlen(pName) == 0)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	M_NAME::iterator it ;
	const char* name = pName ;

	it = m_Name.find(GetHashCode(name)) ;
	if(it != m_Name.end())
	{
		it->second = byRoomIdx ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : UpdateSearchRoomInfo
//	DESC : 검색용 방의 정보를 업데이트 하는 함수.
//  DATE : APRIL 24, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::UpdateSearchRoomInfo(ST_CR_ROOM_SRV* pInfo)
{
	// 함수 파라메터 체크.
	ASSERT(pInfo) ;

	if(!pInfo)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// 검색 용 방 업데이트.
	ST_CR_ROOM_CLT* pTitle = NULL ;
	PTRLISTPOS pos = NULL ;

	pos = m_Title.GetHeadPosition() ;

	while(pos)
	{
		pTitle = NULL ;
		pTitle = (ST_CR_ROOM_CLT*)m_Title.GetNext(pos) ;

		if(!pInfo) continue ;

		if(pInfo->byIdx != pTitle->byIdx) continue ;

		pTitle->byIdx				= pInfo->byIdx ;
		pTitle->bySecretMode		= pInfo->bySecretMode ;
		pTitle->byRoomType			= pInfo->byRoomType ;
		pTitle->byCurGuestCount		= pInfo->byCurGuestCount ;
		pTitle->byTotalGuestCount	= pInfo->byTotalGuestCount ;

		SafeStrCpy(pTitle->title, pInfo->title, TITLE_SIZE) ;

		return ;
	}
}





////-------------------------------------------------------------------------------------------------
////	NAME : Add_Room_To_Client
////	DESC : 서버에 추가 된 방 정보를 클라이언트에 알려주는 함수.
////  DATE : APRIL 2, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomMgr::Add_Room_To_Client(DWORD dwIdx, ST_CR_ROOM_SRV* pInfo)
//{
//	// 함수 파라메터 체크.
//	ASSERT(pInfo) ;
//
//	if(!pInfo)
//	{
//		Throw_Error(err_IMP, __FUNCTION__) ;
//		return ;
//	}
//
//
//
//	// 유저 받기.
//	M_USER::iterator it ;
//	it = m_hmUser.begin() ;
//
//	ASSERT(it != m_hmUser.end()) ;
//
//	if(it == m_hmUser.end())
//	{
//		Throw_Error("Failed to notice add room!!", __FUNCTION__) ;
//		return ;
//	}
//
//
//
//	// 전송 메시지 세팅.
//	MSG_CR_ROOMINFO_TO_NOTICE msg ;
//	memset(&msg, 0, sizeof(MSG_CR_ROOMINFO_TO_NOTICE)) ;
//
//	msg.Category	= MP_CHATROOM ;
//	msg.Protocol	= MP_CHATROOM_NOTICE_ADDROOM_TO_CLIENT ;
//
//	msg.room		= *pInfo ;	
//
//	// 유저에게 메시지 전송.
//    for(it = m_hmUser.begin() ; it != m_hmUser.end() ; ++it)
//	{
//		if(it == m_hmUser.end()) break ;
//
//		msg.dwObjectID = it->second.dwPlayerID ;
//
//		g_Network.Send2Server(dwIdx, (char*)&msg, sizeof(MSG_CR_ROOMINFO_TO_NOTICE)) ;
//	}
//}
//
//
//
//
//
////-------------------------------------------------------------------------------------------------
////	NAME : Delete_Room_To_Client
////	DESC : 서버에서 삭제 된 방 정보를 클라이언트에 알려주는 함수.
////  DATE : APRIL 2, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomMgr::Delete_Room_To_Client(DWORD dwIdx, ST_CR_ROOM_SRV* pInfo)
//{
//	// 함수 파라메터 체크.
//	ASSERT(pInfo) ;
//
//	if(!pInfo)
//	{
//		Throw_Error(err_IMP, __FUNCTION__) ;
//		return ;
//	}
//
//
//
//	// 유저 받기.
//	M_USER::iterator it ;
//	it = m_hmUser.begin() ;
//
//	ASSERT(it != m_hmUser.end()) ;
//
//	if(it == m_hmUser.end())
//	{
//		Throw_Error("Failed to notice add room!!", __FUNCTION__) ;
//		return ;
//	}
//
//
//
//	// 전송 메시지 세팅.
//	MSG_BYTE msg ;
//	memset(&msg, 0, sizeof(MSG_BYTE)) ;
//
//	msg.Category	= MP_CHATROOM ;
//	msg.Protocol	= MP_CHATROOM_NOTICE_DELROOM_TO_CLIENT ;
//
//	msg.bData		= pInfo->byIdx ;
//
//	// 유저에게 메시지 전송.
//	for(it = m_hmUser.begin() ; it != m_hmUser.end() ; ++it)
//	{
//		if(it == m_hmUser.end()) break ;
//
//		msg.dwObjectID = it->second.dwPlayerID ;
//	}
//
//	g_Network.Send2Server(dwIdx, (char*)&msg, sizeof(MSG_CR_ROOMINFO_TO_NOTICE)) ;
//}
//
//
//
//
//
////-------------------------------------------------------------------------------------------------
////	NAME : Update_Room_To_Client
////	DESC : 서버에서 변경 된 방 정보를 클라이언트에 알려주는 함수.
////  DATE : APRIL 2, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomMgr::Update_Room_To_Client(DWORD dwIdx, ST_CR_ROOM_SRV* pInfo)
//{
//	// 함수 파라메터 체크.
//	ASSERT(pInfo) ;
//
//	if(!pInfo)
//	{
//		Throw_Error(err_IMP, __FUNCTION__) ;
//		return ;
//	}
//
//
//
//	// 유저 받기.
//	M_USER::iterator it ;
//	it = m_hmUser.begin() ;
//
//	ASSERT(it != m_hmUser.end()) ;
//
//	if(it == m_hmUser.end())
//	{
//		Throw_Error("Failed to notice add room!!", __FUNCTION__) ;
//		return ;
//	}
//
//
//
//	// 전송 메시지 세팅.
//	MSG_CR_ROOMINFO_TO_NOTICE msg ;
//	memset(&msg, 0, sizeof(MSG_CR_ROOMINFO_TO_NOTICE)) ;
//
//	msg.Category	= MP_CHATROOM ;
//	msg.Protocol	= MP_CHATROOM_NOTICE_UPDATEROOM_TO_CLIENT ;
//
//	msg.room		= *pInfo ;	
//
//	// 유저에게 메시지 전송.
//    for(it = m_hmUser.begin() ; it != m_hmUser.end() ; ++it)
//	{
//		if(it == m_hmUser.end()) break ;
//
//		msg.dwObjectID = it->second.dwPlayerID ;
//	}
//
//	g_Network.Send2Server(dwIdx, (char*)&msg, sizeof(MSG_CR_ROOMINFO_TO_NOTICE)) ;
//}





//-------------------------------------------------------------------------------------------------
//	NAME : UpdateInfo_Created_Room
//	DESC : 서버 내, 채팅방이 추가되었음을 클라이언트에 알리는 함수.
//  DATE : APRIL 2, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::UpdateInfo_Created_Room(ST_CR_ROOM_SRV* pInfo)
{
	// 함수 파라메터 체크.
	ASSERT(pInfo) ;

	if(!pInfo)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}



	// 유저 받기.
	M_USER::iterator it ;
	it = m_hmUser.begin() ;

	ASSERT(it != m_hmUser.end()) ;

	if(it == m_hmUser.end())
	{
		Throw_Error("Failed to notice add room!!", __FUNCTION__) ;
		return ;
	}



	// 전송 메시지 세팅.
	MSG_CR_ROOMINFO_TO_NOTICE msg ;
	memset(&msg, 0, sizeof(MSG_CR_ROOMINFO_TO_NOTICE)) ;

	msg.Category	= MP_CHATROOM ;
	msg.Protocol	= MP_CHATROOM_UPDATEINFO_CREATED_ROOM ;

	msg.room		= *pInfo ;	

	// 유저에게 메시지 전송.
    for(it = m_hmUser.begin() ; it != m_hmUser.end() ; ++it)
	{
		if(it == m_hmUser.end()) break ;

		msg.dwObjectID = it->second.dwPlayerID ;

		g_Network.Send2Server(it->second.dwConnectionIdx, (char*)&msg, sizeof(MSG_CR_ROOMINFO_TO_NOTICE)) ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : UpdateInfo_Deleted_Room
//	DESC : 서버 내, 채팅방이 삭제되었음을 클라이언트에 알리는 함수.
//  DATE : APRIL 2, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::UpdateInfo_Deleted_Room(ST_CR_ROOM_SRV* pInfo)
{
	// 함수 파라메터 체크.
	ASSERT(pInfo) ;

	if(!pInfo)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}



	// 유저 받기.
	M_USER::iterator it ;
	it = m_hmUser.begin() ;

	ASSERT(it != m_hmUser.end()) ;

	if(it == m_hmUser.end())
	{
		Throw_Error("Failed to notice add room!!", __FUNCTION__) ;
		return ;
	}



	// 전송 메시지 세팅.
	MSG_BYTE msg ;
	memset(&msg, 0, sizeof(MSG_BYTE)) ;

	msg.Category	= MP_CHATROOM ;
	msg.Protocol	= MP_CHATROOM_UPDATEINFO_DELETED_ROOM ;

	msg.bData		= pInfo->byIdx ;

	// 유저에게 메시지 전송.
    for(it = m_hmUser.begin() ; it != m_hmUser.end() ; ++it)
	{
		if(it == m_hmUser.end()) break ;

		msg.dwObjectID = it->second.dwPlayerID ;

		g_Network.Send2Server(it->second.dwConnectionIdx, (char*)&msg, sizeof(MSG_BYTE)) ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : UpdateInfo_Changed_SecretMode
//	DESC : 서버 내, 채팅방의 공개/비공개가 변경되었음을 공지하는 함수.
//  DATE : APRIL 2, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::UpdateInfo_Changed_SecretMode(ST_CR_ROOM_SRV* pInfo)
{
	// 함수 파라메터 체크.
	ASSERT(pInfo) ;

	if(!pInfo)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}



	// 유저 받기.
	M_USER::iterator it ;
	it = m_hmUser.begin() ;

	ASSERT(it != m_hmUser.end()) ;

	if(it == m_hmUser.end())
	{
		Throw_Error("Failed to notice add room!!", __FUNCTION__) ;
		return ;
	}



	// 전송 메시지 세팅.
	MSG_BYTE2 msg ;
	memset(&msg, 0, sizeof(MSG_BYTE2)) ;

	msg.Category	= MP_CHATROOM ;
	msg.Protocol	= MP_CHATROOM_UPDATEINFO_SECRETMODE ;

	msg.bData1		= pInfo->byIdx ;
	msg.bData2		= pInfo->bySecretMode ;	

	// 유저에게 메시지 전송.
    for(it = m_hmUser.begin() ; it != m_hmUser.end() ; ++it)
	{
		if(it == m_hmUser.end()) break ;

		msg.dwObjectID = it->second.dwPlayerID ;

		g_Network.Send2Server(it->second.dwConnectionIdx, (char*)&msg, sizeof(MSG_BYTE2)) ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : UpdateInfo_Changed_RoomType
//	DESC : 서버 내, 채팅방분류가 변경되었음을 공지하는 함수.
//  DATE : APRIL 2, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::UpdateInfo_Changed_RoomType(ST_CR_ROOM_SRV* pInfo)
{
	// 함수 파라메터 체크.
	ASSERT(pInfo) ;

	if(!pInfo)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}



	// 유저 받기.
	M_USER::iterator it ;
	it = m_hmUser.begin() ;

	ASSERT(it != m_hmUser.end()) ;

	if(it == m_hmUser.end())
	{
		Throw_Error("Failed to notice add room!!", __FUNCTION__) ;
		return ;
	}



	// 전송 메시지 세팅.
	MSG_BYTE2 msg ;
	memset(&msg, 0, sizeof(MSG_BYTE2)) ;

	msg.Category	= MP_CHATROOM ;
	msg.Protocol	= MP_CHATROOM_UPDATEINFO_ROOMTYPE ;

	msg.bData1		= pInfo->byIdx ;
	msg.bData2		= pInfo->byRoomType ;	

	// 유저에게 메시지 전송.
    for(it = m_hmUser.begin() ; it != m_hmUser.end() ; ++it)
	{
		if(it == m_hmUser.end()) break ;

		msg.dwObjectID = it->second.dwPlayerID ;

		g_Network.Send2Server(it->second.dwConnectionIdx, (char*)&msg, sizeof(MSG_BYTE2)) ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : UpdateInfo_Changed_Title
//	DESC : 서버 내, 채팅방의 제목이 변경되었음을 공지하는 함수.
//  DATE : APRIL 2, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::UpdateInfo_Changed_Title(ST_CR_ROOM_SRV* pInfo)
{
	// 함수 파라메터 체크.
	ASSERT(pInfo) ;

	if(!pInfo)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}



	// 유저 받기.
	M_USER::iterator it ;
	it = m_hmUser.begin() ;

	ASSERT(it != m_hmUser.end()) ;

	if(it == m_hmUser.end())
	{
		Throw_Error("Failed to notice add room!!", __FUNCTION__) ;
		return ;
	}



	// 전송 메시지 세팅.
	MSG_CR_SEARCH_TITLE msg ;
	memset(&msg, 0, sizeof(MSG_CR_SEARCH_TITLE)) ;

	msg.Category	= MP_CHATROOM ;
	msg.Protocol	= MP_CHATROOM_UPDATEINFO_TITLE ;

	msg.byRoomIdx	= pInfo->byIdx ;
	SafeStrCpy(msg.title, pInfo->title, TITLE_SIZE) ;	

	// 유저에게 메시지 전송.
    for(it = m_hmUser.begin() ; it != m_hmUser.end() ; ++it)
	{
		if(it == m_hmUser.end()) break ;

		msg.dwObjectID = it->second.dwPlayerID ;

		g_Network.Send2Server(it->second.dwConnectionIdx, (char*)&msg, sizeof(MSG_CR_SEARCH_TITLE)) ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : UpdateInfo_Changed_Current_GuestCount
//	DESC : 채팅방의 참여자 인원이 변경되었음을 공지하는 함수.
//  DATE : APRIL 2, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::UpdateInfo_Changed_Current_GuestCount(ST_CR_ROOM_SRV* pInfo)
{
	// 함수 파라메터 체크.
	ASSERT(pInfo) ;

	if(!pInfo)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}



	// 유저 받기.
	M_USER::iterator it ;
	it = m_hmUser.begin() ;

	ASSERT(it != m_hmUser.end()) ;

	if(it == m_hmUser.end())
	{
		Throw_Error("Failed to notice add room!!", __FUNCTION__) ;
		return ;
	}



	// 전송 메시지 세팅.
	MSG_BYTE2 msg ;
	memset(&msg, 0, sizeof(MSG_BYTE2)) ;

	msg.Category	= MP_CHATROOM ;
	msg.Protocol	= MP_CHATROOM_UPDATEINFO_CUR_GUESTCOUNT ;

	msg.bData1		= pInfo->byIdx ;
	msg.bData2		= pInfo->byCurGuestCount ;	

	// 유저에게 메시지 전송.
    for(it = m_hmUser.begin() ; it != m_hmUser.end() ; ++it)
	{
		if(it == m_hmUser.end()) break ;

		msg.dwObjectID = it->second.dwPlayerID ;

		g_Network.Send2Server(it->second.dwConnectionIdx, (char*)&msg, sizeof(MSG_BYTE2)) ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : UpdateInfo_Changed_Total_GuestCount
//	DESC : 채팅방의 최대 참여자 인원이 변경되었음을 공지하는 함수.
//  DATE : APRIL 2, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::UpdateInfo_Changed_Total_GuestCount(ST_CR_ROOM_SRV* pInfo)
{
	// 함수 파라메터 체크.
	ASSERT(pInfo) ;

	if(!pInfo)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}



	// 유저 받기.
	M_USER::iterator it ;
	it = m_hmUser.begin() ;

	ASSERT(it != m_hmUser.end()) ;

	if(it == m_hmUser.end())
	{
		Throw_Error("Failed to notice add room!!", __FUNCTION__) ;
		return ;
	}



	// 전송 메시지 세팅.
	MSG_BYTE2 msg ;
	memset(&msg, 0, sizeof(MSG_BYTE2)) ;

	msg.Category	= MP_CHATROOM ;
	msg.Protocol	= MP_CHATROOM_UPDATEINFO_TOTAL_GUESTCOUNT ;

	msg.bData1		= pInfo->byIdx ;
	msg.bData2		= pInfo->byTotalGuestCount ;	

	// 유저에게 메시지 전송.
    for(it = m_hmUser.begin() ; it != m_hmUser.end() ; ++it)
	{
		if(it == m_hmUser.end()) break ;

		msg.dwObjectID = it->second.dwPlayerID ;

		g_Network.Send2Server(it->second.dwConnectionIdx, (char*)&msg, sizeof(MSG_BYTE2)) ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : Cheat_Create_Room_500_Syn
//	DESC : 테스트용으로 추가한 함수. 랜덤하게 250개의 방을 생성.
//  DATE : APRIL 2, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Cheat_Create_Room_Syn(void* pMsg)
{
	// 함수 파라메터 체크.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// 원본 메시지 변환.
	MSG_WORD* pmsg = NULL ;
	pmsg = (MSG_WORD*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}

	for(WORD count = 0 ; count < pmsg->wData ; ++count)
	{
		ST_CR_ROOM_SRV* pRoom = NULL ;
		pRoom = new ST_CR_ROOM_SRV ;

		ASSERT(pRoom) ;

		if(!pRoom)
		{
			Throw_Error("Failed to create a room!!", __FUNCTION__) ;
			return ;
		}

		pRoom->byIdx				= GetEmptyRoomIdx() ;
		//pRoom->wCode				= 1111 ;
		SafeStrCpy(pRoom->code, "code1111", SECRET_CODE_SIZE) ;
		pRoom->bySecretMode		= 1 ;
		pRoom->byRoomType			= random(0, 4) ;
		pRoom->byCurGuestCount	= 1 ;
		pRoom->byTotalGuestCount	= 0 ;
		pRoom->dwOwnerIdx			= random(1111, 9999) ;

		sprintf(pRoom->title, "TEST ROOM - %d", pRoom->dwOwnerIdx) ;

		AddRoom(pRoom) ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : Cheat_Remove_All_Room_Syn
//	DESC : 방을 담고 있는 맵을 모두 비운다.
//  DATE : APRIL 2, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Cheat_Remove_All_Room_Syn()
{
	ReleaseRoom() ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Cheat_Create_User_Syn
//	DESC : 테스트용으로 추가한 함수. 랜덤하게 6000명 유저 추가.
//  DATE : APRIL 8, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Cheat_Create_User_Syn(void* pMsg)
{
	// 함수 파라메터 체크.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// 원본 메시지 변환.
	MSG_WORD* pmsg = NULL ;
	pmsg = (MSG_WORD*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}

	ST_CR_USER user ;
	for(WORD count = 0 ; count < pmsg->wData ; ++count)
	{
		memset(&user, 0, sizeof(ST_CR_USER)) ;

		user.byIdx	= random(1, 250) ;

		user.dwUserID	= random(1111, 9999) ;
		user.dwPlayerID	= m_hmUser.size()+1000 ;

		BYTE byRan1 = random(0, 5) ;
		BYTE byRan2 = 0 ;

		switch(byRan1)
		{
		case 0 : byRan2 = random(0, 5) ;   break ;
		case 1 : byRan2 = random(20, 24) ; break ;
		case 2 : byRan2 = random(28, 43) ; break ;
		case 3 : byRan2 = random(55, 60) ; break ;
		case 4 : byRan2 = random(72, 77) ; break ;
		case 5 : byRan2 = random(90, 95) ; break ;
		default : byRan2 = 0 ;
		}

		//user.wClassIdx	= byRan2 ;
		user.byLevel	= random(1, 150) ;
		user.byMapNum	= random(20, 25) ;
		sprintf(user.name, "TEST USER - %d", user.dwPlayerID) ;

		m_hmUser.insert(std::make_pair(user.dwPlayerID, user)) ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : Cheat_Remove_All_User_Syn
//	DESC : 유저를 담고 있는 맵을 모두 비운다.
//  DATE : APRIL 8, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Cheat_Remove_All_User_Syn()
{
	m_hmUser.clear() ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Add_Guest_To_MyRoom_Syn
//	DESC : 내가 있는 방에 가상의 유저를 추가한다.
//  DATE : APRIL 8, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Add_Guest_To_MyRoom_Syn(void* pMsg)
{
	//// 함수 파라메터 체크.
	//ASSERT(pMsg) ;

	//if(!pMsg)
	//{
	//	Throw_Error(err_IMP, __FUNCTION__) ;
	//	return ;
	//}

	//// 원본 메시지 변환.
	//MSGBASE* pmsg = NULL ;
	//pmsg = (MSGBASE*)pMsg ;

	//ASSERT(pmsg) ;

	//if(!pmsg)
	//{
	//	Throw_Error(err_FCMTO, __FUNCTION__) ;
	//	return ;
	//}

	//// 내 정보 받기.
	//M_USER::iterator it ;
	//it = m_hmUser.find(pmsg->dwObjectID) ;
	//if(it == m_hmUser.end()) return ;

	//// 방 정보 받기.
	//MAP_ROOM::iterator it_room ;
	//it_room = m_hmRoom.find(it->second.byIdx) ;
	//if(it_room == m_hmRoom.end()) return ;

	//BYTE byCurGuestCount = 0 ; 
	//byCurGuestCount = it_room->second.byCurGuestCount ;

	//for(BYTE count = 0 ; count < byCurGuestCount ; ++count)
	//{
	//	if(it_room->second.dwPlayerID[count] != pmsg->dwObjectID) continue ;

	//	for(BYTE count2 = count+1 ; count2 < MAX_USER_PER_ROOM ; ++count2)
	//	{
	//		ST_CR_USER user ;

	//		memset(&user, 0, sizeof(ST_CR_USER)) ;

	//		user.byIdx	= it->second.byIdx ;

	//		user.dwUserID	= random(1111, 9999) ;
	//		user.dwPlayerID	= m_hmUser.size()+1000 ;

	//		BYTE byRan1 = random(0, 5) ;
	//		BYTE byRan2 = 0 ;

	//		switch(byRan1)
	//		{
	//		case 0 : byRan2 = random(0, 5) ;   break ;
	//		case 1 : byRan2 = random(20, 24) ; break ;
	//		case 2 : byRan2 = random(28, 43) ; break ;
	//		case 3 : byRan2 = random(55, 60) ; break ;
	//		case 4 : byRan2 = random(72, 77) ; break ;
	//		case 5 : byRan2 = random(90, 95) ; break ;
	//		default : byRan2 = 0 ;
	//		}

	//		user.wClassIdx	= byRan2 ;
	//		user.byLevel	= random(1, 150) ;
	//		user.byMapNum	= random(20, 25) ;
	//		sprintf(user.name, "TEST USER - %d", user.dwPlayerID) ;

	//		m_hmUser.insert(std::make_pair(user.dwPlayerID, user)) ;

	//		it_room->second.dwPlayerID[count2] = user.dwPlayerID ;

	//		++it_room->second.byCurGuestCount ;
	//	}

	//	it_room->second.byTotalGuestCount = e_GC_20 ;

	//	return ;
	//}
}





//-------------------------------------------------------------------------------------------------
//	NAME : Error
//	DESC : 채팅방 내, 에러 메시지 관련 로그를 남기는 함수.
//  DATE : APRIL 14, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Throw_Error(BYTE errType, char* szCaption)
{
	// 함수 파라메터 체크.
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

	// 에러 타입 확인.
	char tempErr[128] = {0, } ;
	switch(errType)
	{
	case err_IMP :		SafeStrCpy(tempErr, "Invalid a message parameter!!", 128) ;					break ;
	case err_FCMTB :	SafeStrCpy(tempErr, "Failed to convert a message to base!!", 128) ;			break ;
	case err_FCMTO :	SafeStrCpy(tempErr, "Failed to convert a message to original!!", 128) ;		break ;
	default : break ;
	}

	// 에러 출력/로그.
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
//	DESC : 채팅방 내, 에러 메시지 관련 로그를 남기는 함수.
//  DATE : APRIL 14, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Throw_Error(char* szErr, char* szCaption)
{
	// 함수 파라메터 체크.
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

	// 에러 출력/로그.
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
//	NAME : Throw_Error
//	DESC : 채팅방 내, 에러 메시지 관련 로그를 남기는 함수.
//  DATE : APRIL 14, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::Throw_Error(char* szCommonErr, char* szErr, char* szCaption)
{
	// 함수 파라메터 체크.
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

	// 에러 출력/로그.
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
//	DESC : 채팅방 내, 에러 메시지 관련 로그를 남기는 함수.
//  DATE : APRIL 14, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMgr::WriteLog(char* pMsg)
{
	SYSTEMTIME time ;
	GetLocalTime(&time) ;

	TCHAR szTime[_MAX_PATH] = {0, } ;
	sprintf(szTime, "%04d-%02d-%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond) ;

	FILE *fp = fopen("Log/Dist-ChatRoomErr.log", "a+");
	if (fp)
	{
		fprintf(fp, "%s [%s]\n", pMsg,  szTime);
		fclose(fp);
	}
}













//=================================================================================================
//	FILE		: CChatRoomMainDlg.cpp
//	DESC		: Implementation part of CChatRoomMainDlg class.
//	PROGRAMMER	: Yongs Lee
//	DATE		: APRIL 3, 2008
//=================================================================================================





//-------------------------------------------------------------------------------------------------
//		Include header files.
//-------------------------------------------------------------------------------------------------
#include "stdafx.h"

#include "../WindowIDEnum.h"

#include "./Input/Mouse.h"
#include "./Input/Keyboard.h"

#include "./FilteringTable.h"

#include "./Interface/cResourceManager.h"
#include "./Interface/cWindowManager.h"

#include "./Interface/cButton.h"
#include "./Interface/cComboBox.h"
#include "./Interface/cPushupButton.h"
#include "./Interface/cEditBox.h"
#include "./Interface/cListDialog.h"
#include "./Interface/cStatic.h"

#include "./cMsgBox.h"
#include "./GameIn.h"

#include "./ObjectManager.h"
#include "./ChatManager.h"
#include "./ChatRoomMgr.h"

#include "./ChatRoomCreateDlg.h"
#include "./ChatRoomDlg.h"
#include "./ChatRoomGuestListDlg.h"
#include "./ChatRoomJoinDlg.h"
#include "./ChatRoomMainDlg.h"
#include "./ChatRoomOptionDlg.h"





//-------------------------------------------------------------------------------------------------
//	NAME : CChatRoomMainDlg
//	DESC : 생성자 함수.
//  DATE : APRIL 3, 2008 LYW
//-------------------------------------------------------------------------------------------------
CChatRoomMainDlg::CChatRoomMainDlg(void)
{
	// 초기화 처리.
	m_pRoomList			= NULL ;

	m_pSearchBtn		= NULL ;
	m_pSearchBox		= NULL ;
	m_pTitle			= NULL ;
	m_pName				= NULL ;

	m_pRoomType			= NULL ;

	m_pCreateBtn		= NULL ;
	m_pJoinBtn			= NULL ;
	m_pNumJoinBtn		= NULL ;
	m_pRefreshBtn		= NULL ;

	m_pPrevBtn			= NULL ;
	m_pNextBtn			= NULL ;

	m_pPage				= NULL ;

	//m_pLoad_Chatroom	= NULL ;

	//memset(m_SearchBuff, 0, sizeof(m_SearchBuff)) ;

	m_bySearchMode		= e_Search_Title ;

	//m_byRoomType		= e_RTM_Party ;

	m_byCurPage			= 0 ;
	m_byMaxPage			= 0 ;

	memset(&m_SelectRoom, 0, sizeof(ST_CR_ROOM_CLT)) ;

	m_byRoomType		= e_RTM_AllLooK ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : ~CChatRoomMainDlg
//	DESC : 소멸자 함수.
//  DATE : APRIL 3, 2008 LYW
//-------------------------------------------------------------------------------------------------
CChatRoomMainDlg::~CChatRoomMainDlg(void)
{
}





////-------------------------------------------------------------------------------------------------
////	NAME : ReleaseRoom
////	DESC : 채팅방 리소스 해제 함수.
////  DATE : APRIL 22, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomMgr::ReleaseRoom()
//{
//	ST_CR_ROOM_CLT* pInfo ;
//	PTRLISTPOS pos = NULL ;
//
//	pos = m_RoomList.GetHeadPosition() ;
//
//	while(pos)
//	{
//		pInfo = NULL ;
//		pInfo = (ST_CR_ROOM_CLT*)m_RoomList.GetNext(pos) ;
//
//		if(!pInfo) continue ;
//
//		delete pInfo ;
//	}
//
//	m_RoomList.RemoveAll() ;
//}
//
//
//
//
//
////-------------------------------------------------------------------------------------------------
////	NAME : IsInRoomList
////	DESC : 방 리스트에, 방 정보가 있는지 체크하는 함수.
////  DATE : APRIL 22, 2008 LYW
////-------------------------------------------------------------------------------------------------
//BYTE CChatRoomMgr::IsInRoomList(BYTE byRoomIdx) 
//{
//	ST_CR_ROOM_CLT* pItem ;
//
//	PTRLISTPOS pos = NULL ;
//	pos = m_RoomList.GetHeadPosition() ;
//
//	while(pos)
//	{
//		pItem = NULL ;
//		pItem = (ST_CR_ROOM_CLT*)m_RoomList.GetNext(pos) ;
//
//		if(!pItem) continue ;
//
//		if(pItem->byIdx == byRoomIdx) return TRUE ;
//	}
//
//	return FALSE ;
//}
//
//
//
//
//
////-------------------------------------------------------------------------------------------------
////	NAME : AddRoom
////	DESC : 채팅방 정보를 추가하는 함수.
////  DATE : APRIL 22, 2008 LYW
////-------------------------------------------------------------------------------------------------
//BYTE CChatRoomMgr::AddRoom(ST_CR_ROOM_CLT* pInfo)
//{
//	ASSERT(pInfo) ;
//
//	if(!pInfo)
//	{
//		Throw_Error(err_IMP, __FUNCTION__) ;
//		return FALSE ;
//	}
//
//	m_RoomList.AddTail(pInfo) ;
//
//	return TRUE ;
//}
//
//
//
//
//
////-------------------------------------------------------------------------------------------------
////	NAME : DeleteRoom
////	DESC : 채팅방 정보를 삭제하는 함수.
////  DATE : APRIL 22, 2008 LYW
////-------------------------------------------------------------------------------------------------
//BYTE CChatRoomMgr::DeleteRoom(BYTE byRoomIdx)
//{
//	ST_CR_ROOM_CLT* pItem ;
//
//	PTRLISTPOS pos = NULL ;
//	pos = m_RoomList.GetHeadPosition() ;
//
//	while(pos)
//	{
//		pItem = NULL ;
//		pItem = (ST_CR_ROOM_CLT*)m_RoomList.GetNext(pos) ;
//
//		if(!pItem) continue ;
//
//		if(pItem->byIdx != byRoomIdx) continue ;
//
//		delete pItem ;
//
//		return TRUE ;
//	}
//
//	return FALSE ;
//}
//
//
//
//
//
////-------------------------------------------------------------------------------------------------
////	NAME : GetRoomInfo
////	DESC : 채팅방 정보를 반환하는 함수.
////  DATE : APRIL 22, 2008 LYW
////-------------------------------------------------------------------------------------------------
//ST_CR_ROOM_CLT* CChatRoomMgr::GetRoomInfo(BYTE byRoomIdx) 
//{
//	ST_CR_ROOM_CLT* pItem ;
//
//	PTRLISTPOS pos = NULL ;
//	pos = m_RoomList.GetHeadPosition() ;
//
//	while(pos)
//	{
//		pItem = NULL ;
//		pItem = (ST_CR_ROOM_CLT*)m_RoomList.GetNext(pos) ;
//
//		if(!pItem) continue ;
//
//		if(pItem->byIdx != byRoomIdx) continue ;
//
//		return pItem ;
//	}
//
//	return NULL ;
//}
//
//
//
//
//
////-------------------------------------------------------------------------------------------------
////	NAME : UpdateRoom
////	DESC : 채팅방 정보를 업데이트하는 함수.
////  DATE : APRIL 22, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomMgr::UpdateRoom(ST_CR_ROOM_CLT* pInfo)
//{
//}





//-------------------------------------------------------------------------------------------------
//	NAME : Linking
//		 : 
//	DESC : 채팅방 리스트 다이얼로그의 하위 컨트롤들을 링크한다.
//		 : 
//		 : 1. 하위 컨트롤 포인터 링크.
//		 : 2. 링크 된 컨트롤 유효성 체크.
//		 : 3. 기본 분류 세팅.
//		 : 4. 기본 검색 모드 세팅.
//		 : 5. 로딩 스태틱 비활성화.
//		 : 6. 페이지 세팅.
//		 : 7. 채팅방 매니져에 등록.
//		 : 
//  DATE : APRIL 3, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::Linking()
{
	// 하위 컨트롤 포인터 링크.
	m_pRoomList			= (cListDialog*)GetWindowForID(CRMD_LST_ROOMLIST) ;

	m_pSearchBtn		= (cButton*)GetWindowForID(CRMD_BTN_SEARCH) ;
	m_pSearchBox		= (cEditBox*)GetWindowForID(CRMD_EDB_SEARCH) ;
	m_pTitle			= (cPushupButton*)GetWindowForID(CRMD_CHK_TITLE) ;
	m_pName				= (cPushupButton*)GetWindowForID(CRMD_CHK_NAME) ;

	m_pRoomType			= (cComboBox*)GetWindowForID(CRMD_CMB_ROOMTYPE) ;

	m_pCreateBtn		= (cButton*)GetWindowForID(CRMD_BTN_CREATE) ;
	m_pJoinBtn			= (cButton*)GetWindowForID(CRMD_BTN_JOIN) ;
	m_pNumJoinBtn		= (cButton*)GetWindowForID(CRMD_BTN_NUMJOIN) ;
	m_pRefreshBtn		= (cButton*)GetWindowForID(CRMD_BTN_REFRESH) ;

	m_pPrevBtn			= (cButton*)GetWindowForID(CRMD_BTN_PREV) ;
	m_pNextBtn			= (cButton*)GetWindowForID(CRMD_BTN_NEXT) ;

	m_pPage				= (cStatic*)GetWindowForID(CRMD_STC_PAGE) ;

	//m_pLoad_Chatroom	= (cStatic*)GetWindowForID(CRMD_STC_LOAD_CHATROOM) ;



	// 링크 된 컨트롤 유효성 체크.
	ASSERT(m_pRoomList) ;
	ASSERT(m_pSearchBtn) ;
	ASSERT(m_pSearchBox) ;
	ASSERT(m_pTitle) ;
	ASSERT(m_pName) ;
	ASSERT(m_pRoomType) ;
	ASSERT(m_pCreateBtn) ;
	ASSERT(m_pJoinBtn) ;
	ASSERT(m_pNumJoinBtn) ;
	ASSERT(m_pRefreshBtn) ;
	ASSERT(m_pPrevBtn) ;
	ASSERT(m_pNextBtn) ;
	ASSERT(m_pPage) ;
	//ASSERT(m_pLoad_Chatroom) ;

	if( !m_pRoomList	|| !m_pSearchBtn	|| !m_pSearchBox || !m_pTitle || !m_pName ||
		!m_pRoomType	|| !m_pCreateBtn	|| !m_pJoinBtn  ||
		!m_pNumJoinBtn	|| !m_pRefreshBtn	|| //!m_pLoad_Chatroom ||
		!m_pPrevBtn		|| !m_pNextBtn		|| !m_pPage )
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// 채팅방 분류 세팅.
	//m_pRoomType->SetCurSelectedIdx(e_RTM_AllLooK) ;
	m_pRoomType->SetMaxLine(e_RT_Max) ;



	// 기본 검색 모드 세팅.
	if(m_pTitle)		m_pTitle->SetPush(TRUE) ;
	if(m_pName)			m_pName->SetPush(FALSE) ;

	// 로딩 스태틱 비활성화.
	//m_pLoad_Chatroom->SetActive(FALSE) ;



	// 페이지 세팅.
	//SetPage(1, 1) ;
	m_pPage->SetStaticText("") ;



	// 채팅방 매니져에 등록.
	CHATROOMMGR->SetMainDlg(this) ;
}





////-------------------------------------------------------------------------------------------------
////	NAME : ActionKeyboardEvent
////	DESC : The function to handling keyboard event.
////  DATE : APRIL 24, 2008 LYW
////-------------------------------------------------------------------------------------------------
//DWORD CChatRoomMainDlg::ActionKeyboardEvent( CKeyboard * keyInfo )
//{
//	DWORD we = WE_NULL ;
//
//	if( !m_bActive ) return we ;
//
//	we |= cDialog::ActionKeyboardEvent(keyInfo) ;
//
//	ASSERT(m_pSearchBox) ;
//
//	if(!m_pSearchBox)
//	{
//		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
//		return we ;
//	}
//
//	if(m_pSearchBox->IsFocus())
//	{
//		if( keyInfo->GetKeyPressed(KEY_RETURN) || keyInfo->GetKeyPressed(KEY_PADENTER) )
//		{
//			Search_Syn() ;
//		}
//	}
//
//	return we ;
//}





//-------------------------------------------------------------------------------------------------
//	NAME : ActionEvent
//	DESC : The function to handling a mouse events.
//  DATE : APRIL 3, 2008 LYW
//-------------------------------------------------------------------------------------------------
DWORD CChatRoomMainDlg::ActionEvent( CMouse* mouseInfo ) 
{
	DWORD we = WE_NULL ;

	// 함수 파라메터 체크.
	ASSERT(mouseInfo) ;

	if( !mouseInfo )
	{
		CHATROOMMGR->Throw_Error("Invalid a mouse info!!", __FUNCTION__) ;
		return we ;
	}



	// 다이얼로그 이벤트를 받는다.
	we = cDialog::ActionEvent( mouseInfo ) ;



	// 유저 리스트 상에 마우스가 있으면,
	if(m_pRoomList->PtInWindow( (LONG)mouseInfo->GetMouseX(), (LONG)mouseInfo->GetMouseY() ))
	{
		// 클릭 이벤트가 발생했을 경우.
		if( we & WE_LBTNCLICK )
		{
			ITEM* pItem ;

			DWORD dwColor ;

			LONG lItemCount = 0 ;
			lItemCount = m_pRoomList->GetItemCount() ;

			int nSelectedIdx = 0 ;
			nSelectedIdx  = m_pRoomList->GetSelectRowIdx() ;
			
			for(LONG count = 0 ; count < lItemCount ; ++count)
			{
				pItem = NULL ;
				pItem = m_pRoomList->GetItem(count) ;

				if(!pItem) continue ;

				if(pItem->line == nSelectedIdx)
				{
					dwColor = RGBA_MAKE(255, 0, 0, 255) ;
				}
				else
				{
					dwColor = RGBA_MAKE(255, 255, 255, 255) ;
				}

				pItem->rgb = dwColor ;
			}

			LIST_ROOM::iterator it ;
			it = m_Room.begin() ;

			std::advance( it, nSelectedIdx ) ;

			if(it == m_Room.end())
			{
				memset(&m_SelectRoom, 0, sizeof(ST_CR_ROOM_CLT)) ;
			}
			else
			{
				m_SelectRoom = *it ;
			}

			/*if(m_byPrintMode == e_PrintMode_Normal)
			{
				LIST_ROOM::iterator it ;
				it = m_Room.begin() ;

				std::advance( it, nSelectedIdx ) ;

				if(it == m_Room.end())
				{
					memset(&m_SelectRoom, 0, sizeof(ST_CR_ROOM_CLT)) ;
				}
				else
				{
					m_SelectRoom = *it ;
				}
			}
			else
			{
				LIST_TEMPROOM::iterator it ;
				it = m_Room.begin() ;

				std::advance( it, nSelectedIdx ) ;

				if(it == m_Room.end())
				{
					memset(&m_SelectRoom, 0, sizeof(ST_CR_ROOM_CLT)) ;
				}
				else
				{
					m_SelectRoom = *it ;
				}
			}*/
		}
		else if( we & WE_LBTNDBLCLICK )
		{
			Join_Syn() ;

			//if(m_SelectRoom.byIdx != 0)
			//{
			//	MSG_CR_JOIN_SYN msg ;
			//	memset(&msg, 0, sizeof(MSG_CR_JOIN_SYN)) ;

			//	msg.Category	= MP_CHATROOM ;
			//	msg.Protocol	= MP_CHATROOM_JOIN_ROOM_SYN ;
			//	msg.dwObjectID	= HEROID ;

			//	msg.byRoomIdx	= m_SelectRoom.byIdx ;

			//	NETWORK->Send( &msg, sizeof(MSG_CR_JOIN_SYN) ) ;
			//}
		}
	}

	/*if(we & WE_COMBOBOXSELECT)
	{
		SortRoomByType() ;
	}*/

	//else if(m_pRoomType->PtInWindow((LONG)mouseInfo->GetMouseX(), (LONG)mouseInfo->GetMouseY()))
	//{
	//	if( we & WE_LBTNCLICK )
	//	{
	//		/*DWORD dwComboEvent = WE_NULL ;
	//		dwComboEvent = m_pRoomType->ActionEvent(mouseInfo) ;*/

	//		if(we & WE_COMBOBOXSELECT)
	//		{
	//			SortRoomByType() ;
	//		}
	//	}
	//}

	return we ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : OnActionEvent
//	DESC : 하위컨트롤에 이벤트가 발생했을 때 처리하는 함수.
//  DATE : APRIL 3, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::OnActionEvent(LONG id, void* p, DWORD event)
{
	// 함수 파라메터 체크.
	ASSERT(p) ;

	if(!p)
	{
		CHATROOMMGR->Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}



	// 컨트롤 아이디 확인.
	switch(id)
	{
	case CRMD_BTN_SEARCH :		Search_Syn() ;			break ;

	case CRMD_CHK_TITLE :		Check_Title() ;			break ;
	case CRMD_CHK_NAME :		Check_Name() ;			break ;

	case CRMD_BTN_CREATE :		Create_Syn() ;			break ;
	case CRMD_BTN_JOIN :		Join_Syn() ;			break ;
	case CRMD_BTN_NUMJOIN :		NumJoin_Syn() ;			break ;
	case CRMD_BTN_REFRESH :		Refresh_Syn() ;			break ;

	case CRMD_BTN_PREV :		Btn_Prev() ;			break ;
	case CRMD_BTN_NEXT :		Btn_Next() ;			break ;

	case CRMD_CMB_ROOMTYPE :	
	{
		if( event == WE_COMBOBOXSELECT )
		{
			//SortRoomByType() ;

			// 재사용 시간 체크.
			if(!CHATROOMMGR->IsEnableExecuteCom())
			{
				WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1517)) ;
				return ;
			}

			m_byRoomType = (BYTE)m_pRoomType->GetCurSelectedIdx() ;

			MSG_BYTE2 msg ;
			memset(&msg, 0, sizeof(MSG_BYTE2)) ;

			msg.Category		= MP_CHATROOM ;
			msg.Protocol		= MP_CHATROOM_ROOM_SYN ;
			msg.dwObjectID		= HEROID ;

			msg.bData1			= m_byRoomType ;
			msg.bData2			= m_byCurPage ;

			NETWORK->Send( &msg, sizeof(MSG_BYTE2) ) ;

			//m_byPrintMode = (BYTE)m_pRoomType->GetCurSelectedIdx() ;

			//MSG_BYTE2 msg ;
			//memset(&msg, 0, sizeof(MSG_BYTE2)) ;

			//msg.Category	= MP_CHATROOM ;
			//msg.Protocol	= MP_CHATROOM_ROOM_SYN ;
			//msg.dwObjectID	= HEROID ;

			//msg.bData1		= m_byPrintMode ;
			//msg.bData2		= 0 ;

			//NETWORK->Send( &msg, sizeof(MSG_BYTE2) ) ;

			// 시간 업데이트.
			CHATROOMMGR->UpdateMyLastComTime() ;
		}
	}
	break ;

	default : break ;
	}
}





////-------------------------------------------------------------------------------------------------
////	NAME : Search_Syn
////	DESC : 채팅방 리스트를 활성/비활성화 할때, 로딩중입니다라는 글자를 활성/비활성화 한다.
////  DATE : APRIL 3, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomMainDlg::SetActive(BOOL val)
//{
//	// 활성/비활성 체크.
//	if(val)
//	{
//		m_pLoad_Chatroom->SetActive(TRUE) ;
//	}
//	else
//	{
//		m_pLoad_Chatroom->SetActive(FALSE) ;
//	}
//
//	cDialog::SetActive(val) ;
//}





//-------------------------------------------------------------------------------------------------
//	NAME : Search_Syn
//		 : 
//	DESC : 세팅 된 모드에 따라 검색 요청을 하는 함수.
//		 : 
//		 : 1. 검색 내용 체크.
//		 : 2. 검색 모드 체크.
//		 : 3. 검색 요청.
//		 : 
//  DATE : APRIL 24, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::Search_Syn()
{
	// 하위 컨트롤 체크.
	ASSERT(m_pSearchBox) ;

	if(!m_pSearchBox)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// 검색 내용 체크.
	char* word ;

	word = NULL ;
	word = m_pSearchBox->GetEditText() ;

	m_pSearchBox->SetEditText("") ;

	if(strlen(word) <= 1)
	{
		WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1491)) ;
		return ;
	}

	// 필터링 체크.
	if( FILTERTABLE->FilterChat(word) )
	{
		WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1225)) ;
		return ;
	}

	// 검색 모드 체크.
	if(m_bySearchMode == e_Search_Title)
	{
		MSG_CR_SEARCH_TITLE msg ;
		memset(&msg, 0, sizeof(MSG_CR_SEARCH_TITLE)) ;

		msg.Category	= MP_CHATROOM ;
		msg.Protocol	= MP_CHATROOM_SEARCH_FOR_TITLE_SYN ;
		msg.dwObjectID	= HEROID ;

		msg.byRoomIdx	= 0 ;
		
		SafeStrCpy(msg.title, word, TITLE_SIZE) ;

		NETWORK->Send( &msg, sizeof(MSG_CR_SEARCH_TITLE) ) ;
	}
	else
	{
		if( strcmp(word, HERO->GetObjectName()) == 0)
		{
			WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1538)) ;
			return ;
		}

		MSG_CR_SEARCH_NAME msg ;
		memset(&msg, 0, sizeof(MSG_CR_SEARCH_NAME)) ;

		msg.Category	= MP_CHATROOM ;
		msg.Protocol	= MP_CHATROOM_SEARCH_FOR_NAME_SYN ;
		msg.dwObjectID	= HEROID ;

		msg.byRoomIdx	= 0 ;
		
		SafeStrCpy(msg.name, word, MAX_NAME_LENGTH) ;

		NETWORK->Send( &msg, sizeof(MSG_CR_SEARCH_NAME) ) ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : Check_Title
//	DESC : 검색 모드를 방제목 검색으로 세팅한다.
//  DATE : APRIL 8, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::Check_Title() 
{
	// 하위 컨트롤 체크.
	ASSERT(m_pTitle) ;
	ASSERT(m_pName) ;

 	if(!m_pTitle || !m_pName) 
 	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
 	}
 
	// 검색모드 세팅.
 	/*BOOL bChecked = FALSE ;
 
	bChecked = m_pTitle->IsPushed() ;
 
 	if(bChecked)
 	{
 		m_pTitle->SetPush(FALSE) ;
 		m_pName->SetPush(TRUE) ;
 
 		m_bySearchMode = e_Search_Name ;
 	}
 	else
 	{
 		m_pTitle->SetPush(TRUE) ;
 		m_pName->SetPush(FALSE) ;
 
 		m_bySearchMode = e_Search_Title ;
 	}*/

	m_pTitle->SetPush(TRUE) ;
 	m_pName->SetPush(FALSE) ;

 	m_bySearchMode = e_Search_Title ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Check_Name
//	DESC : 검색모드를 이름 검색으로 세팅한다.
//  DATE : APRIL 8, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::Check_Name()
{
	// 하위 컨트롤 체크.
	ASSERT(m_pTitle) ;
	ASSERT(m_pName) ;

 	if(!m_pTitle || !m_pName) 
 	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
 	}
 
 	/*BOOL bChecked = FALSE ;
 
 	bChecked = m_pName->IsPushed() ;
 
 	if(bChecked)
 	{
 		m_pName->SetPush(FALSE) ;
 		m_pTitle->SetPush(TRUE) ;
 
 		m_bySearchMode = e_Search_Title ;
 	}
 	else
 	{
 		m_pName->SetPush(TRUE) ;
 		m_pTitle->SetPush(FALSE) ;
 
 		m_bySearchMode = e_Search_Name ;
 	}*/

	m_pName->SetPush(TRUE) ;
 	m_pTitle->SetPush(FALSE) ;

 	m_bySearchMode = e_Search_Name ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Create_Syn
//	DESC : 채팅방 생성 다이얼로그를 연다.
//  DATE : APRIL 3, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::Create_Syn()
{
	if(CHATROOMMGR->Get_ChatRoomState() == e_State_In_ChatRoom) 
	{
		WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1505)) ;
		return ;
	}

	// 채팅방 생성 창 받기.
	CChatRoomCreateDlg* pDlg = NULL ;
	pDlg = GAMEIN->GetChatRoomCreateDlg() ;

	ASSERT(pDlg) ;

	if(!pDlg)
	{
		CHATROOMMGR->Throw_Error("Failed to receive create dialog!!", __FUNCTION__) ;
		return ;
	}

	pDlg->SetActive(TRUE) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Join_Syn
//	DESC : 선택한 방에 참여하기를 요청하는 함수.
//  DATE : APRIL 8, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::Join_Syn()
{
	if(CHATROOMMGR->Get_ChatRoomState() == e_State_In_ChatRoom) 
	{
		WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1505)) ;
		return ;
	}
	
	// 선택한 채팅방 체크.
	if(m_SelectRoom.byIdx == 0)
	{
		WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1516)) ;
		return ;
	}

	//MSG_CR_JOIN_SYN msg ;
	//memset(&msg, 0, sizeof(MSG_CR_JOIN_SYN)) ;

	//msg.Category	= MP_CHATROOM ;
	//msg.Protocol	= MP_CHATROOM_JOIN_ROOM_SYN ;
	//msg.dwObjectID	= HEROID ;

	//msg.byRoomIdx	= m_SelectRoom.byIdx ;

	//NETWORK->Send( &msg, sizeof(MSG_CR_JOIN_SYN) ) ;

	// 공개모드라면,
	if(m_SelectRoom.bySecretMode == e_RM_Open)
	{
		// 참여 요청을 한다.
		MSG_CR_JOIN_SYN msg ;
		memset(&msg, 0, sizeof(MSG_CR_JOIN_SYN)) ;

		msg.Category	= MP_CHATROOM ;
		msg.Protocol	= MP_CHATROOM_JOIN_ROOM_SYN ;
		msg.dwObjectID	= HEROID ;

		msg.byRoomIdx	= m_SelectRoom.byIdx ;

		NETWORK->Send( &msg, sizeof(MSG_CR_JOIN_SYN) ) ;
	}
	// 비공개 모드라면,
	else if(m_SelectRoom.bySecretMode == e_RM_Close)
	{
		// 참여 창을 받는다.
		CChatRoomJoinDlg* pDlg = NULL ;
		pDlg = GAMEIN->GetChatRoomJoinDlg() ;

		if(!pDlg)
		{
			CHATROOMMGR->Throw_Error("Failed to receive join dialog!!", __FUNCTION__) ;
			return ;
		}

		// 비밀번호 입력 모드로 활성화 시킨다.
		pDlg->SetInputMode(e_JoinDlg_Type_SecretCode) ;
		pDlg->SetActive(TRUE) ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : NumJoin_Syn
//	DESC : 번호 입력으로 참여하기를 눌렀을 때 처리를 하는 함수.
//  DATE : APRIL 8, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::NumJoin_Syn() 
{
	if(CHATROOMMGR->Get_ChatRoomState() == e_State_In_ChatRoom) 
	{
		WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1505)) ;
		return ;
	}

	// 참여 창 받기.
	CChatRoomJoinDlg* pDlg = NULL ;
	pDlg = GAMEIN->GetChatRoomJoinDlg() ;

	if(!pDlg)
	{
		CHATROOMMGR->Throw_Error("Failed to receive join dialog!!", __FUNCTION__) ;	
		return ;
	}

	// 방번호 입력 모드로 참여창 열기.
	pDlg->SetInputMode(e_JoinDlg_Type_RoonIndex) ;
	pDlg->SetActive(TRUE) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Refresh_Syn
//	DESC : 새로고침 요청을 처리하는 함수.
//  DATE : APRIL 8, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::Refresh_Syn()
{
	//// 하위 컨트롤 체크.
	//ASSERT(m_pLoad_Chatroom) ;

 //	if(!m_pLoad_Chatroom) 
 //	{
	//	CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
	//	return ;
 //	}

	//m_pLoad_Chatroom->SetActive(TRUE) ;

	// 재사용 시간 체크.
	if(!CHATROOMMGR->IsEnableExecuteCom())
	{
		WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1517)) ;
		return ;
	}

	MSG_BYTE2 msg ;
	memset(&msg, 0, sizeof(MSG_BYTE2)) ;

	msg.Category		= MP_CHATROOM ;
	msg.Protocol		= MP_CHATROOM_ROOM_SYN ;
	msg.dwObjectID		= HEROID ;

	if(m_byRoomType == e_RTM_Searched)
	{
		msg.bData1			= e_RTM_AllLooK ;
		msg.bData2			= 0 ;
	}
	else
	{
		msg.bData1			= m_byRoomType ;
		msg.bData2			= m_byCurPage ;
	}

	NETWORK->Send( &msg, sizeof(MSG_BYTE2) ) ;

	// 시간 업데이트.
	CHATROOMMGR->UpdateMyLastComTime() ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Btn_Prev
//	DESC : 이전 페이지 요청을 하는 함수.
//  DATE : APRIL 8, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::Btn_Prev() 
{
	// 페이지 확인.
	if(m_byCurPage == 0) return ;
	else
	{
		//// 하위 컨트롤 체크.
		//ASSERT(m_pLoad_Chatroom) ;

 	//	if(!m_pLoad_Chatroom) 
 	//	{
		//	CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		//	return ;
 	//	}

		//m_pLoad_Chatroom->SetActive(TRUE) ;

		// 재사용 시간 체크.
		if(!CHATROOMMGR->IsEnableExecuteCom())
		{
			WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1517)) ;
			return ;
		}

		if( m_byCurPage == 0 ) return ;

		MSG_BYTE2 msg ;
		memset(&msg, 0, sizeof(MSG_BYTE2)) ;

		msg.Category		= MP_CHATROOM ;
		msg.Protocol		= MP_CHATROOM_ROOM_SYN ;
		msg.dwObjectID		= HEROID ;

		msg.bData1			= m_byRoomType ;
		msg.bData2			= m_byCurPage-1 ;

		NETWORK->Send( &msg, sizeof(MSG_BYTE2) ) ;

		//if(m_byPrintMode == e_PrintMode_Normal || m_byPrintMode == e_PrintMode_Searced)
		//{
		//	// 페이지 요청.
		//	MSG_BYTE msg ;
		//	memset(&msg, 0, sizeof(MSG_BYTE)) ;

		//	msg.Category		= MP_CHATROOM ;
		//	msg.Protocol		= MP_CHATROOM_ROOM_SYN ;
		//	msg.dwObjectID		= HEROID ;

		//	msg.bData			= m_byCurPage-1 ;

		//	NETWORK->Send( &msg, sizeof(MSG_BYTE) ) ;
		//}
		//else
		//{
		//	// 페이지 요청.
		//	MSG_BYTE2 msg ;
		//	memset(&msg, 0, sizeof(MSG_BYTE2)) ;

		//	msg.Category		= MP_CHATROOM ;
		//	msg.Protocol		= MP_CHATROOM_ROOM_SYN ;
		//	msg.dwObjectID		= HEROID ;

		//	msg.bData1			= m_byPrintMode ;
		//	msg.bData2			= m_byCurPage-1 ;

		//	NETWORK->Send( &msg, sizeof(MSG_BYTE2) ) ;
		//}

		// 시간 업데이트.
		CHATROOMMGR->UpdateMyLastComTime() ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : Btn_Next
//	DESC : 다음 페이지 요청을 하는 함수.
//  DATE : APRIL 8, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::Btn_Next()
{
	// 페이지 확인.
	if(m_byCurPage == m_byMaxPage) return ;
	else
	{
		//// 하위 컨트롤 체크.
		//ASSERT(m_pLoad_Chatroom) ;

 	//	if(!m_pLoad_Chatroom) 
 	//	{
		//	CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		//	return ;
 	//	}

		//m_pLoad_Chatroom->SetActive(TRUE) ;

		// 재사용 시간 체크.
		if(!CHATROOMMGR->IsEnableExecuteCom())
		{
			WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1517)) ;
			return ;
		}

		if(m_byCurPage >= m_byMaxPage) return ;

		MSG_BYTE2 msg ;
		memset(&msg, 0, sizeof(MSG_BYTE2)) ;

		msg.Category		= MP_CHATROOM ;
		msg.Protocol		= MP_CHATROOM_ROOM_SYN ;
		msg.dwObjectID		= HEROID ;

		msg.bData1			= m_byRoomType ;
		msg.bData2			= m_byCurPage+1 ;

		NETWORK->Send( &msg, sizeof(MSG_BYTE2) ) ;

		//if(m_byPrintMode == e_PrintMode_Normal || m_byPrintMode == e_PrintMode_Searced)
		//{
		//	// 페이지 요청.
		//	MSG_BYTE msg ;
		//	memset(&msg, 0, sizeof(MSG_BYTE)) ;

		//	msg.Category		= MP_CHATROOM ;
		//	msg.Protocol		= MP_CHATROOM_ROOM_SYN ;
		//	msg.dwObjectID		= HEROID ;

		//	msg.bData			= m_byCurPage+1 ;

		//	NETWORK->Send( &msg, sizeof(MSG_BYTE) ) ;
		//}
		//else
		//{
		//	// 페이지 요청.
		//	MSG_BYTE2 msg ;
		//	memset(&msg, 0, sizeof(MSG_BYTE2)) ;

		//	msg.Category		= MP_CHATROOM ;
		//	msg.Protocol		= MP_CHATROOM_ROOM_SYN ;
		//	msg.dwObjectID		= HEROID ;

		//	msg.bData1			= m_byPrintMode ;
		//	msg.bData2			= m_byCurPage+1 ;

		//	NETWORK->Send( &msg, sizeof(MSG_BYTE2) ) ;
		//}

		// 시간 업데이트.
		CHATROOMMGR->UpdateMyLastComTime() ;
	}
}





////-------------------------------------------------------------------------------------------------
////	NAME : SortRoomByType
////	DESC : 선택한 분류로 채팅방을 담아서 출력한다.
////  DATE : APRIL 8, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomMainDlg::SortRoomByType()
//{
//	int nType = 0 ;
//	nType = m_pRoomType->GetCurSelectedIdx() ;
//
//	/*switch(nType)
//	{
//	case 0 : m_byRoomType = e_RTM_Party ;			break ;
//	case 1 : m_byRoomType = e_RTM_Deal ;			break ;
//	case 2 : m_byRoomType = e_RTM_Normal_Chat ;	break ;
//	case 3 : m_byRoomType = e_RTM_Local_Chat ;		break ;
//	case 4 : m_byRoomType = e_RTM_Make_Friend ;	break ;
//	default : break ;
//	}*/
//
//	// 채팅방을 담고 있는 Map 시작 받기.
//	LIST_ROOM::iterator it ;
//	it = m_Room.begin() ;
//
//	// 채팅방 개수를 받는다.
//	BYTE byCount = 0 ;
//	byCount = m_Room.size() ;
//
//	// 채팅방이 없으면 에러 출력.
//	if(byCount == 0 || it == m_Room.end())
//	{
//		WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1490)) ;
//		return ;
//	}
//
//	// 임시로 채팅방을 담고 있는 Map을 비운다.
//	m_TempRoom.clear() ;
//
//	for(BYTE count = 0 ; count < byCount ; ++count)
//	{
//		if(it == m_Room.end()) break ;
//
//		const ST_CR_ROOM_CLT& room = *it ;
//
//		if(room.byRoomType != nType)
//		{
//			++it ;
//			continue ;
//		}
//
//		m_TempRoom.push_back(room) ;
//		++it ;
//	}
//
//	// 검색 된 채팅방을 담고 있는 Map의 시작 받기.
//	LIST_ROOM::iterator it_search ;
//	it_search = m_TempRoom.begin() ;
//
//	// 검색 된 내용이 없으면 메시지 박스 출력.
//	if(it_search == m_TempRoom.end())
//	{
//		WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1492)) ;
//		return ;
//	}
//
//	// 채팅방 리스트 다이얼로그를 모두 비운다.
//	m_pRoomList->RemoveAll() ;
//
//	// 채팅방 리스트 다이얼로그의 아이템이 보이는 개수를 받는다.	
//	byCount = 0 ;
//	byCount = m_TempRoom.size() ;
//
//	char tempBuf[256] = {0,} ;
//	char tempNum[10] = {0,} ;
//
//	BYTE byCurGuestCount ;
//	for(BYTE count = 0 ; count < byCount ; ++count)
//	{
//		// 데이터 유효성 검사.
//		if(it_search == m_TempRoom.end()) break ;
//
//		// 임시 데이터 초기화.
//		memset(tempBuf, 0, sizeof(tempBuf)) ;
//		memset(tempNum, 0, sizeof(tempNum)) ;
//
//		if( it_search->byIdx / 100 <= 0 )
//		{
//			if( it_search->byIdx / 10 <= 0 )
//			{
//				strcat(tempBuf, RESRCMGR->GetMsg(10)) ;
//				strcat(tempBuf, RESRCMGR->GetMsg(10)) ;
//			}
//			else
//			{
//				strcat(tempBuf, RESRCMGR->GetMsg(10)) ;
//			}
//		}
//
//		strcat(tempBuf, itoa(it_search->byIdx, tempNum, 10)) ;
//		strcat(tempBuf, ".") ;
//
//		// 공개/비공개 세팅.
//		strcat(tempBuf, "[") ;
//		if( it_search->bySecretMode == e_RM_Open )
//		{
//			strcat(tempBuf, RESRCMGR->GetMsg(892)) ;
//		}
//		else
//		{
//			strcat(tempBuf, RESRCMGR->GetMsg(893)) ;
//		}
//		strcat(tempBuf, "]") ;
//
//		// 채팅방 분류 세팅.
//		strcat(tempBuf, "[") ;
//		switch(it_search->byRoomType)
//		{
//		case e_RTM_Party :			strcat(tempBuf, RESRCMGR->GetMsg(880)) ;	break ;
//		case e_RTM_Deal :			strcat(tempBuf, RESRCMGR->GetMsg(881)) ;	break ;
//		case e_RTM_Normal_Chat :	strcat(tempBuf, RESRCMGR->GetMsg(882)) ;	break ;
//		case e_RTM_Local_Chat :		strcat(tempBuf, RESRCMGR->GetMsg(883)) ;	break ;
//		case e_RTM_Make_Friend :	strcat(tempBuf, RESRCMGR->GetMsg(884)) ;	break ;
//		default : break ;
//		}
//		strcat(tempBuf, "]") ;
//
//		// 채팅방 제목 세팅.
//		strcat(tempBuf, it_search->title) ;
//
//		// 현재 참여자 인원 세팅.
//		strcat(tempBuf, "(") ;
//
//		byCurGuestCount = 0 ;
//		byCurGuestCount = it_search->byCurGuestCount ;
//
//		if(byCurGuestCount % 10 < 0)
//		{
//			strcat(tempBuf, RESRCMGR->GetMsg(10)) ;
//		}
//
//		memset(tempNum, 0, sizeof(tempNum)) ;
//		strcat(tempBuf, itoa(byCurGuestCount, tempNum, 10)) ;
//
//		strcat(tempBuf, "/") ;
//
//		// 최대 참여자 세팅.
//		memset(tempNum, 0, sizeof(tempNum)) ;
//
//		switch(it_search->byTotalGuestCount)
//		{
//		case e_GC_20 :	strcat(tempBuf, RESRCMGR->GetMsg(922)) ;	break ;
//		case e_GC_15 :	strcat(tempBuf, RESRCMGR->GetMsg(921)) ;	break ;
//		case e_GC_10 :	strcat(tempBuf, RESRCMGR->GetMsg(920)) ;	break ;
//		case e_GC_5 :	strcat(tempBuf, RESRCMGR->GetMsg(919)) ;	break ;
//		default : break ;
//		}
//		strcat(tempBuf, ")") ;
//
//		// 채팅방 리스트 다이얼로그에 추가.
//		m_pRoomList->AddItem( tempBuf, RGBA_MAKE(255, 255, 255, 255), count ) ;
//
//		++it_search ;
//	}
//
//	//// 로딩 텍스트 비활성화.
//	//if(m_pLoad_Chatroom->IsActive())
//	//{
//	//	m_pLoad_Chatroom->SetActive(FALSE) ;
//	//}
//
//	// 현재 출력되고 있는 내용이 검색된 내용이라고 세팅.
//	//m_byPrintMode = e_PrintMode_Searced ;
//}





//-------------------------------------------------------------------------------------------------
//	NAME : Check_Room_To_Join
//	DESC : 참여창/비밀번호창에서 OK버튼을 눌렀을 때 처리하는 함수.
//  DATE : APRIL 8, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::Check_Room_To_Join(BYTE inputMode, char* pCode)
{
	// 함수 파라메터 체크.
	ASSERT(pCode) ;

	if(!pCode || strlen(pCode) == 0)
	{
		//CHATROOMMGR->Throw_Error(err_IMP, __FUNCTION__) ;
		WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1510)) ;
		return ;
	}

	// 참여창 받기.
	CChatRoomJoinDlg* pDlg = NULL ;
	pDlg = GAMEIN->GetChatRoomJoinDlg() ;

	ASSERT(pDlg) ;

	if(!pDlg)
	{
		CHATROOMMGR->Throw_Error("Failed to receive join dialog!!", __FUNCTION__) ;
		return ;
	}

	// 채팅방이 담긴 Map의 시작 받기.
	LIST_ROOM::iterator it ;
	it = m_Room.begin() ;

	// 방이 없으면 메시지 박스 출력.
	if(it == m_Room.end())
	{
		WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1490)) ;

		m_pRoomList->RemoveAll() ;

		m_Room.clear() ;
		m_TempRoom.clear() ;

		return ;
	}

	// 방번호 입력 모드면, 
	if(inputMode == e_JoinDlg_Type_RoonIndex)
	{
		BYTE byCheckCount = 0 ; 
		byCheckCount = m_Room.size() ;

		for(BYTE count = 0 ; count < byCheckCount ; ++count)
		{
			if(it->byIdx != (BYTE)(atoi(pCode)))
			{
				++it ;
				continue ;
			}

			// 선택한 방 설정.
			m_SelectRoom = *it ;

			// 공개/비공개 확인.
			if(it->bySecretMode == e_RM_Close)
			{
				// 비밀번호 창을 활성화 한다.
				pDlg->SetInputMode(e_JoinDlg_Type_SecretCode) ;
				pDlg->SetActive(TRUE) ;
				return ;
			}
		}
		//	else
		//	{
		//		// 공개방이면, 참여 요청.
		//		MSG_CR_JOIN_SYN msg ;
		//		memset(&msg, 0, sizeof(MSG_CR_JOIN_SYN)) ;

		//		msg.Category	= MP_CHATROOM ;
		//		msg.Protocol	= MP_CHATROOM_JOIN_ROOM_SYN ;
		//		msg.dwObjectID	= HEROID ;

		//		msg.byRoomIdx	= it->byIdx ;

		//		NETWORK->Send( &msg, sizeof(MSG_CR_JOIN_SYN) ) ;
		//		return ;
		//	}
		//}

		//// 여기까지 오면 일치하는 방이 없는거.
		//pDlg->SetActive(FALSE) ;
		//WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1507)) ;


		// 여기까지 일치하는 방이 없다면, 단순 방번호 입력 로그인 요청이다.
		MSG_CR_JOIN_SYN msg ;
		memset(&msg, 0, sizeof(MSG_CR_JOIN_SYN)) ;

		msg.Category	= MP_CHATROOM ;
		msg.Protocol	= MP_CHATROOM_JOIN_ROOM_SYN ;
		msg.dwObjectID	= HEROID ;

		msg.byRoomIdx	= (BYTE)atoi(pCode) ;

		NETWORK->Send( &msg, sizeof(MSG_CR_JOIN_SYN) ) ;

		pDlg->SetActive(FALSE) ;
	}
	// 비밀번호 입력 모드면,
	else if(inputMode == e_JoinDlg_Type_SecretCode)
	{
		// 비밀번호를 담아 참여 요청을 한다.
		MSG_CR_JOIN_SYN msg ;
		memset(&msg, 0, sizeof(MSG_CR_JOIN_SYN)) ;

		msg.Category	= MP_CHATROOM ;
		msg.Protocol	= MP_CHATROOM_JOIN_ROOM_SYN ;
		msg.dwObjectID	= HEROID ;

		msg.byRoomIdx	= m_SelectRoom.byIdx ;
		SafeStrCpy(msg.code, pCode, SECRET_CODE_SIZE) ;

		NETWORK->Send( &msg, sizeof(MSG_CR_JOIN_SYN) ) ;

		pDlg->SetActive(FALSE) ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : Init_RoomList
//		 : 
//	DESC : 서버로 부터 넘어 온, 채팅방 정보를 세팅하는 함수.
//		 : 
//		 : 1. 채팅방을 담고 있는 Map을 비운다.
//		 : 2. 방정보를 받는다.
//		 : 3. 페이지 세팅.
//		 : 4. 로딩 텍스트 비활성화.
//		 : 5. 채팅방 리스트 다이얼로그 새로고침.
//		 : 
//  DATE : APRIL 9, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::Init_RoomList(void* pMsg)
{
	// 함수 파라메터 체크.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		CHATROOMMGR->Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// 원본 메시지 변환
	MSG_CR_ROOMLIST* pmsg = NULL ;
	pmsg = (MSG_CR_ROOMLIST*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		CHATROOMMGR->Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}

	// 하위 컨트롤 체크.
	ASSERT(m_pRoomList) ;
	//ASSERT(m_pLoad_Chatroom) ;

	if(!m_pRoomList /*|| !m_pLoad_Chatroom*/)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// 채팅방 타입 세팅.
	m_pRoomType->SetCurSelectedIdx(pmsg->byRoomType) ;
	m_pRoomType->SelectComboText(pmsg->byRoomType) ;
	m_byRoomType = pmsg->byRoomType ;

	// 채팅방 리스트 비움.
	m_Room.clear() ;

	// 채팅방 개수 만큼 루프.
	for(BYTE count = 0 ; count < pmsg->byCount ; ++count)
	{
		if(pmsg->room[count].byIdx == 0) continue ;

		const ST_CR_ROOM_CLT& room = pmsg->room[ count ] ;

		m_Room.push_back(room) ;
	}

	// 채팅방 리스트 페이지 세팅.
	SetPage(pmsg->byCurPage, pmsg->byTotalPage) ;

	// 채팅방 리스트 다이얼로그 새로고침.
	Refresh_RoomList() ;

	//// 로딩 이미지 비활성화.
	//if(m_pLoad_Chatroom->IsActive())
	//{
	//	m_pLoad_Chatroom->SetActive(FALSE) ;
	//}

	//m_byPrintMode = e_PrintMode_Normal ;
}





////-------------------------------------------------------------------------------------------------
////	NAME : Init_TypeRoomList
////	DESC : 채팅방 분류에 따라 채팅방을 초기화 하는 함수.
////  DATE : APRIL 9, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomMainDlg::Init_TypeRoomList(void* pMsg)
//{
//	// 함수 파라메터 체크.
//	ASSERT(pMsg) ;
//
//	if(!pMsg)
//	{
//		CHATROOMMGR->Throw_Error(err_IMP, __FUNCTION__) ;
//		return ;
//	}
//
//	// 원본 메시지 변환
//	MSG_CR_ROOMLIST* pmsg = NULL ;
//	pmsg = (MSG_CR_ROOMLIST*)pMsg ;
//
//	ASSERT(pmsg) ;
//
//	if(!pmsg)
//	{
//		CHATROOMMGR->Throw_Error(err_FCMTO, __FUNCTION__) ;
//		return ;
//	}
//
//	// 하위 컨트롤 체크.
//	ASSERT(m_pRoomList) ;
//	//ASSERT(m_pLoad_Chatroom) ;
//
//	if(!m_pRoomList /*|| !m_pLoad_Chatroom*/)
//	{
//		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
//		return ;
//	}
//
//	// 채팅방 리스트 비움.
//	m_Room.clear() ;
//
//	// 채팅방 개수 만큼 루프.
//	for(BYTE count = 0 ; count < ROOM_COUNT_PER_PAGE ; ++count)
//	{
//		if(pmsg->room[count].byIdx == 0) continue ;
//
//		const ST_CR_ROOM_CLT& room = pmsg->room[ count ] ;
//
//		m_Room.push_back(room) ;
//	}
//
//	// 채팅방 리스트 페이지 세팅.
//	SetPage(pmsg->byCurPage, pmsg->byTotalPage) ;
//
//	// 채팅방 리스트 다이얼로그 새로고침.
//	Refresh_RoomList() ;
//}





//-------------------------------------------------------------------------------------------------
//	NAME : Refresh_RoomList
//	DESC : 채팅방 리스트를 새로고침하는 함수.
//  DATE : APRIL 9, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::Refresh_RoomList()
{
	// 하위 컨트롤 체크.
	ASSERT(m_pRoomList) ;

	if(!m_pRoomList)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}

	// 활성화 여부 체크.
	if(!IsActive()) return ;



	// 리스트 다이얼로그 비우기.
	m_pRoomList->RemoveAll() ;

	// 방 위치 받기.
	LIST_ROOM::iterator it ;
	it = m_Room.begin() ;
	if(it == m_Room.end()) return ;

	// 임시 데이터 선언.
	char tempBuf[256] = {0,} ;
	char tempNum[10] = {0,} ;

	BYTE byCount = 0 ;
	byCount = m_Room.size() ;

	BYTE byStartCount = 0 ;
	byStartCount = m_byCurPage*ROOM_COUNT_PER_PAGE ;



	// 방 개수 만큼 루프.
	for(BYTE count = 0 ; count < byCount ; ++count)
	{
		if(it == m_Room.end()) return ;

		memset(tempBuf, 0, sizeof(tempBuf)) ;
		memset(tempNum, 0, sizeof(tempNum)) ;

		// 인덱스 세팅.
		if( it->byIdx / 100 <= 0 )
		{
			if( it->byIdx / 10 <= 0 )
			{
				strcat(tempBuf, RESRCMGR->GetMsg(10)) ;
				strcat(tempBuf, RESRCMGR->GetMsg(10)) ;
			}
			else
			{
				strcat(tempBuf, RESRCMGR->GetMsg(10)) ;
			}
		}

		strcat(tempBuf, itoa(it->byIdx, tempNum, 10)) ;
		strcat(tempBuf, ".") ;

		// 공개/비공개 세팅.
		strcat(tempBuf, "[") ;
		if( it->bySecretMode == e_RM_Open )
		{
			strcat(tempBuf, RESRCMGR->GetMsg(892)) ;
		}
		else
		{
			strcat(tempBuf, RESRCMGR->GetMsg(893)) ;
		}
		strcat(tempBuf, "]") ;

		// 채팅방 분류 세팅.
		strcat(tempBuf, "[") ;
		switch(it->byRoomType)
		{
		case e_RT_Party :			strcat(tempBuf, RESRCMGR->GetMsg(880)) ;	break ;
		case e_RT_Deal :			strcat(tempBuf, RESRCMGR->GetMsg(881)) ;	break ;
		case e_RT_Normal_Chat :		strcat(tempBuf, RESRCMGR->GetMsg(882)) ;	break ;
		case e_RT_Local_Chat :		strcat(tempBuf, RESRCMGR->GetMsg(883)) ;	break ;
		case e_RT_Make_Friend :		strcat(tempBuf, RESRCMGR->GetMsg(884)) ;	break ;
		default : break ;
		}
		strcat(tempBuf, "]") ;

		// 채팅방 제목 세팅.
		strcat(tempBuf, it->title) ;

		// 현재 참여자 세팅.
		strcat(tempBuf, "(") ;

		if(it->byCurGuestCount % 10 < 0)
		{
			strcat(tempBuf, RESRCMGR->GetMsg(10)) ;
		}

		memset(tempNum, 0, sizeof(tempNum)) ;
		strcat(tempBuf, itoa(it->byCurGuestCount, tempNum, 10)) ;

		strcat(tempBuf, "/") ;

		if(it->byCurGuestCount % 10 < 0)
		{
			strcat(tempBuf, RESRCMGR->GetMsg(10)) ;
		}

		// 최대 참여자 세팅.
		memset(tempNum, 0, sizeof(tempNum)) ;

		switch(it->byTotalGuestCount)
		{
		case e_GC_20 :	strcat(tempBuf, RESRCMGR->GetMsg(922)) ;	break ;
		case e_GC_15 :	strcat(tempBuf, RESRCMGR->GetMsg(921)) ;	break ;
		case e_GC_10 :	strcat(tempBuf, RESRCMGR->GetMsg(920)) ;	break ;
		case e_GC_5 :		strcat(tempBuf, RESRCMGR->GetMsg(919)) ;	break ;
		default : break ;
		}
		strcat(tempBuf, ")") ;

		// 리스트 다이얼로그에 추가.
		m_pRoomList->AddItem( tempBuf, RGBA_MAKE(255, 255, 255, 255), count ) ;

		++it ;
	}

	//// 선택 된 방을 세팅.
	//it = m_Room.begin() ;

	//ASSERT(it != m_Room.end()) ;

	//const ST_CR_ROOM_CLT& selectRoom = *it ;

	//m_SelectRoom = selectRoom ;

	//if(!IsActive()) SetActive(TRUE) ;

	//if(m_pLoad_Chatroom->IsActive()) m_pLoad_Chatroom->SetActive(FALSE) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Clear_RoomList
//	DESC : 채팅방 리스트를 모두 비우는 함수.
//  DATE : APRIL 11, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::Clear_RoomList()
{
	// 하위 컨트롤 체크.
	ASSERT(m_pRoomList) ;

	if(!m_pRoomList)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}

	// 채팅방 리스트 다이얼로그 비우기.
	m_pRoomList->RemoveAll() ;

	// 채팅방 리스트 비우기.
	m_Room.clear() ;

	// 선택 된 방 정보 지우기.
	memset(&m_SelectRoom, 0, sizeof(ST_CR_ROOM_CLT)) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Add_RoomInfo
//	DESC : 방 정보를 추가하는 함수.
//  DATE : APRIL 9, 2008 LYW
//-------------------------------------------------------------------------------------------------
//void CChatRoomMainDlg::Add_RoomInfo(void* pMsg)
//{
//	// 함수 파라메터 체크.
//	ASSERT(pMsg) ;
//
//	if(!pMsg)
//	{
//#ifdef _MB_CRMD_
//		MessageBox( NULL, "ERR-IMP", "CRMD-ARI", MB_OK) ;
//#endif //_MB_CRMD_
//		return ;
//	}
//
//	// 원본 메시지 변환.
//	ST_CR_ROOM_CLT* pmsg = NULL ; 
//	pmsg = (ST_CR_ROOM_CLT*)pMsg ;
//
//	ASSERT(pmsg) ;
//
//	if(!pmsg)
//	{
//#ifdef _MB_CRMD_	
//		MessageBox( NULL, "ERR-FCMO", "CRMD-ARI", MB_OK) ;
//#endif //_MB_CRMD_	
//		return ;
//	}
//
//	// 하위 컨트롤 체크.
//	ASSERT(m_pRoomList) ;
//
//	if(!m_pRoomList)
//	{
//#ifdef _MB_CRMD_
//		MessageBox( NULL, "ERR-IVDCS", "CRMD-ARI", MB_OK) ;
//#endif //_MB_CRMD_
//		return ;
//	}
//
//	// 방정보 받기.
//	ST_CHATROOM_ROOM room ;
//	memset(&room, 0, sizeof(ST_CHATROOM_ROOM)) ;
//
//	room.wIdx			= pmsg->wIdx ;
//	room.wCode			= pmsg->wCode ;
//	room.baseInfo		= pmsg->baseInfo ;
//
//	//room.guestIdx[0]	= pmsg->user[0].base.dwPlayerID ;
//	room.guestIdx[0]	= pmsg->user[0].dwPlayerID ;
//
//	// 방정보 추가.
//	const ST_CHATROOM_ROOM& addInfo = room ;
//
//	m_Room.push_back(addInfo) ;
//
//	// 채팅방 리스트 새로 고침.
//	Refresh_RoomList() ;
//	//SetPage(pmsg->byPage, pmsg->byTotalPage) ;
//
//	//// 채팅방 비밀번호 세팅.
//	//CHATROOMMGR->SetMyRoomCode(pmsg->wCode) ;
//
//	//// 채팅창 방장 인덱스 세팅.
//	//CHATROOMMGR->SetMyRoomOwnerIdx(pmsg->baseInfo.dwOwnerIdx) ;
//
//	//// 채팅방 옵션 세팅/반환.
//	//CHATROOMMGR->SetMyRoomOption(pmsg->wCode, &pmsg->baseInfo) ;
//
//	// 채팅창 활성화.
//	Active_ChatRoom(pmsg) ;
//}





//-------------------------------------------------------------------------------------------------
//	NAME : Add_MyRoom
//	DESC : 채팅방 리스트에 내 방 정보를 추가한다.
//  DATE : APRIL 12, 2008 LYW
//-------------------------------------------------------------------------------------------------
BOOL CChatRoomMainDlg::Add_Room(ST_CR_ROOM_CLT* pInfo)
{
	// 함수 파라메터 체크.
	ASSERT(pInfo) ;

	if(!pInfo)
	{
		CHATROOMMGR->Throw_Error(err_IMP, __FUNCTION__) ;
		return FALSE ;
	}

	const ST_CR_ROOM_CLT& addInfo = *pInfo ;
	m_Room.push_back(addInfo) ;

	Refresh_RoomList() ;

	return TRUE ;
    
	//BYTE bySize = 0 ;
	//bySize = m_Room.size() ;

	//LIST_ROOM::iterator it ;
	//it = m_Room.begin() ;

	//// 아직 추가 된 방이 없을 경우.
	//if(it == m_Room.end())
	//{
	//	const ST_CR_ROOM_CLT& addInfo = *pInfo ;

	//	m_Room.push_back(addInfo) ;
	//}
	//else
	//{
	//	// 방 개수 만큼 for문.
	//	for(BYTE count = 0 ; count < bySize ; ++count)
	//	{
	//		// 이미 있는 방이라면,
	//		if(it->dwOwnerIdx == pInfo->info.dwOwnerIdx)
	//		{
	//			// 방정보 받기.
	//			it->info = pInfo->info ;

	//			// 유저 정보 받기.
	//			for(BYTE count2 = 0 ; count2 < MAX_USER_PER_ROOM ; ++count2)
	//			{
	//				it->user[count2] = pInfo->user[count2] ;
	//			}
	//			break ;
	//		}
	//		else
	//		{
	//			const ST_CR_ROOM_CLT& addInfo = *pInfo ;

	//			m_Room.push_back(addInfo) ;

	//			break ;
	//		}

	//		++it ;
	//	}
	//}

	//// 채팅방 리스트 새로 고침.
	//Refresh_RoomList() ;

	//return TRUE ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Update_RoomInfo
//	DESC : 방 정보 하나를 업데이트 할 때 처리하는 함수.
//  DATE : APRIL 9, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::Update_RoomInfo(ST_CR_ROOM_CLT* pInfo)
{
	// 함수 파라메터 체크.
	ASSERT(pInfo) ;

	if(!pInfo)
	{
		CHATROOMMGR->Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// 방리스트를 참조할 이더레이터 받기.
	LIST_ROOM::iterator it ;
	it = m_Room.begin() ;

	// 리스트 사이즈 받기.
	if(it == m_Room.end()) return ;

	BYTE bySize = 0 ;
	bySize = m_Room.size() ;

	// 업데이트 할 방 찾기.
	for(BYTE count = 0 ; count < bySize ; ++count)
	{
		if(it->byIdx != pInfo->byIdx) continue ;

		// 방정보 업데이트.
		it->byIdx				= pInfo->byIdx ;
		it->bySecretMode		= pInfo->bySecretMode ;
		it->byRoomType			= pInfo->byRoomType ;
		it->byCurGuestCount		= pInfo->byCurGuestCount ;
		it->byTotalGuestCount	= pInfo->byTotalGuestCount ;

		SafeStrCpy(it->title, pInfo->title, TITLE_SIZE) ;

		break ;
	}

	// 채팅방 리스트 업데이트.
	Refresh_RoomList() ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : DeleteRoom
//	DESC : 인자로 넘어온 방을 찾아서, 리스트에서 삭제한다.
//  DATE : APRIL 9, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::DeleteRoom(BYTE byRoomIdx)
{
	// 방리스트를 참조할 이더레이터 받기.
	LIST_ROOM::iterator it ;
	it = m_Room.begin() ;

	// 리스트 사이즈 받기.
	if(it == m_Room.end()) return ;

	BYTE bySize = 0 ;
	bySize = m_Room.size() ;

	// 업데이트 할 방 찾기.
	for(BYTE count = 0 ; count < bySize ; ++count)
	{
		if(it->byIdx != byRoomIdx) continue ;

		m_Room.erase(it) ;
		break ;
	}

	// 채팅방 리스트 업데이트.
	Refresh_RoomList() ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : SetPage
//	DESC : 채팅방 리스트의 페이지를 세팅한다.
//  DATE : APRIL 9, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::SetPage(BYTE num1, BYTE num2)
{
	// 채팅방 리스트 페이지 세팅.
	char tempBuff[10] = {0,} ;
	char tempNum[10]  = {0,} ;

	sprintf(tempBuff, "%d", num1+1) ;
	strcat(tempBuff, "/") ;
	strcat(tempBuff, itoa(num2+1, tempNum, 10)) ;

	m_pPage->SetStaticText(tempBuff) ;

	m_byCurPage = num1 ;
	m_byMaxPage = num2 ;
}




//-------------------------------------------------------------------------------------------------
//	NAME : GetRoomListItem
//	DESC : 인자로 넘어온 번호의 방을 찾아서 반환한다.
//  DATE : APRIL 9, 2008 LYW
//-------------------------------------------------------------------------------------------------

ST_CR_ROOM_CLT* CChatRoomMainDlg::GetRoomListItem(BYTE byRoomIdx)
{
	// 방리스트를 참조할 이더레이터 받기.
	LIST_ROOM::iterator it ;
	it = m_Room.begin() ;

	// 리스트 사이즈 받기.
	if(it == m_Room.end()) return NULL ;

	BYTE bySize = 0 ;
	bySize = m_Room.size() ;

	// 업데이트 할 방 찾기.
	for(BYTE count = 0 ; count < bySize ; ++count)
	{
		if(it->byIdx != byRoomIdx)
		{
			++it ;
			continue ;
		}

		ST_CR_ROOM_CLT& room = *it ;

		return &room ;
	}

	return NULL ;
}





void CChatRoomMainDlg::Err_EmptyRoom()
{
	ASSERT(m_pRoomList) ;
	//ASSERT(m_pLoad_Chatroom) ;

	if(!m_pRoomList /*|| !m_pLoad_Chatroom*/)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}

	if(!IsActive())
	{
		SetActive(TRUE) ;
	}

	WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1490)) ;

	m_pRoomList->RemoveAll() ;

	m_Room.clear() ;
	m_TempRoom.clear() ;

	/*if(m_pLoad_Chatroom->IsActive())
	{
		m_pLoad_Chatroom->SetActive(FALSE) ;
	}*/
}
















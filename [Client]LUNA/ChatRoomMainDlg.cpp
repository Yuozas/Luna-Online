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
//	DESC : ������ �Լ�.
//  DATE : APRIL 3, 2008 LYW
//-------------------------------------------------------------------------------------------------
CChatRoomMainDlg::CChatRoomMainDlg(void)
{
	// �ʱ�ȭ ó��.
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
//	DESC : �Ҹ��� �Լ�.
//  DATE : APRIL 3, 2008 LYW
//-------------------------------------------------------------------------------------------------
CChatRoomMainDlg::~CChatRoomMainDlg(void)
{
}





////-------------------------------------------------------------------------------------------------
////	NAME : ReleaseRoom
////	DESC : ä�ù� ���ҽ� ���� �Լ�.
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
////	DESC : �� ����Ʈ��, �� ������ �ִ��� üũ�ϴ� �Լ�.
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
////	DESC : ä�ù� ������ �߰��ϴ� �Լ�.
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
////	DESC : ä�ù� ������ �����ϴ� �Լ�.
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
////	DESC : ä�ù� ������ ��ȯ�ϴ� �Լ�.
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
////	DESC : ä�ù� ������ ������Ʈ�ϴ� �Լ�.
////  DATE : APRIL 22, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomMgr::UpdateRoom(ST_CR_ROOM_CLT* pInfo)
//{
//}





//-------------------------------------------------------------------------------------------------
//	NAME : Linking
//		 : 
//	DESC : ä�ù� ����Ʈ ���̾�α��� ���� ��Ʈ�ѵ��� ��ũ�Ѵ�.
//		 : 
//		 : 1. ���� ��Ʈ�� ������ ��ũ.
//		 : 2. ��ũ �� ��Ʈ�� ��ȿ�� üũ.
//		 : 3. �⺻ �з� ����.
//		 : 4. �⺻ �˻� ��� ����.
//		 : 5. �ε� ����ƽ ��Ȱ��ȭ.
//		 : 6. ������ ����.
//		 : 7. ä�ù� �Ŵ����� ���.
//		 : 
//  DATE : APRIL 3, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::Linking()
{
	// ���� ��Ʈ�� ������ ��ũ.
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



	// ��ũ �� ��Ʈ�� ��ȿ�� üũ.
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



	// ä�ù� �з� ����.
	//m_pRoomType->SetCurSelectedIdx(e_RTM_AllLooK) ;
	m_pRoomType->SetMaxLine(e_RT_Max) ;



	// �⺻ �˻� ��� ����.
	if(m_pTitle)		m_pTitle->SetPush(TRUE) ;
	if(m_pName)			m_pName->SetPush(FALSE) ;

	// �ε� ����ƽ ��Ȱ��ȭ.
	//m_pLoad_Chatroom->SetActive(FALSE) ;



	// ������ ����.
	//SetPage(1, 1) ;
	m_pPage->SetStaticText("") ;



	// ä�ù� �Ŵ����� ���.
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

	// �Լ� �Ķ���� üũ.
	ASSERT(mouseInfo) ;

	if( !mouseInfo )
	{
		CHATROOMMGR->Throw_Error("Invalid a mouse info!!", __FUNCTION__) ;
		return we ;
	}



	// ���̾�α� �̺�Ʈ�� �޴´�.
	we = cDialog::ActionEvent( mouseInfo ) ;



	// ���� ����Ʈ �� ���콺�� ������,
	if(m_pRoomList->PtInWindow( (LONG)mouseInfo->GetMouseX(), (LONG)mouseInfo->GetMouseY() ))
	{
		// Ŭ�� �̺�Ʈ�� �߻����� ���.
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
//	DESC : ������Ʈ�ѿ� �̺�Ʈ�� �߻����� �� ó���ϴ� �Լ�.
//  DATE : APRIL 3, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::OnActionEvent(LONG id, void* p, DWORD event)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(p) ;

	if(!p)
	{
		CHATROOMMGR->Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}



	// ��Ʈ�� ���̵� Ȯ��.
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

			// ���� �ð� üũ.
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

			// �ð� ������Ʈ.
			CHATROOMMGR->UpdateMyLastComTime() ;
		}
	}
	break ;

	default : break ;
	}
}





////-------------------------------------------------------------------------------------------------
////	NAME : Search_Syn
////	DESC : ä�ù� ����Ʈ�� Ȱ��/��Ȱ��ȭ �Ҷ�, �ε����Դϴٶ�� ���ڸ� Ȱ��/��Ȱ��ȭ �Ѵ�.
////  DATE : APRIL 3, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomMainDlg::SetActive(BOOL val)
//{
//	// Ȱ��/��Ȱ�� üũ.
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
//	DESC : ���� �� ��忡 ���� �˻� ��û�� �ϴ� �Լ�.
//		 : 
//		 : 1. �˻� ���� üũ.
//		 : 2. �˻� ��� üũ.
//		 : 3. �˻� ��û.
//		 : 
//  DATE : APRIL 24, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::Search_Syn()
{
	// ���� ��Ʈ�� üũ.
	ASSERT(m_pSearchBox) ;

	if(!m_pSearchBox)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// �˻� ���� üũ.
	char* word ;

	word = NULL ;
	word = m_pSearchBox->GetEditText() ;

	m_pSearchBox->SetEditText("") ;

	if(strlen(word) <= 1)
	{
		WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1491)) ;
		return ;
	}

	// ���͸� üũ.
	if( FILTERTABLE->FilterChat(word) )
	{
		WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1225)) ;
		return ;
	}

	// �˻� ��� üũ.
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
//	DESC : �˻� ��带 ������ �˻����� �����Ѵ�.
//  DATE : APRIL 8, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::Check_Title() 
{
	// ���� ��Ʈ�� üũ.
	ASSERT(m_pTitle) ;
	ASSERT(m_pName) ;

 	if(!m_pTitle || !m_pName) 
 	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
 	}
 
	// �˻���� ����.
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
//	DESC : �˻���带 �̸� �˻����� �����Ѵ�.
//  DATE : APRIL 8, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::Check_Name()
{
	// ���� ��Ʈ�� üũ.
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
//	DESC : ä�ù� ���� ���̾�α׸� ����.
//  DATE : APRIL 3, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::Create_Syn()
{
	if(CHATROOMMGR->Get_ChatRoomState() == e_State_In_ChatRoom) 
	{
		WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1505)) ;
		return ;
	}

	// ä�ù� ���� â �ޱ�.
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
//	DESC : ������ �濡 �����ϱ⸦ ��û�ϴ� �Լ�.
//  DATE : APRIL 8, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::Join_Syn()
{
	if(CHATROOMMGR->Get_ChatRoomState() == e_State_In_ChatRoom) 
	{
		WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1505)) ;
		return ;
	}
	
	// ������ ä�ù� üũ.
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

	// ���������,
	if(m_SelectRoom.bySecretMode == e_RM_Open)
	{
		// ���� ��û�� �Ѵ�.
		MSG_CR_JOIN_SYN msg ;
		memset(&msg, 0, sizeof(MSG_CR_JOIN_SYN)) ;

		msg.Category	= MP_CHATROOM ;
		msg.Protocol	= MP_CHATROOM_JOIN_ROOM_SYN ;
		msg.dwObjectID	= HEROID ;

		msg.byRoomIdx	= m_SelectRoom.byIdx ;

		NETWORK->Send( &msg, sizeof(MSG_CR_JOIN_SYN) ) ;
	}
	// ����� �����,
	else if(m_SelectRoom.bySecretMode == e_RM_Close)
	{
		// ���� â�� �޴´�.
		CChatRoomJoinDlg* pDlg = NULL ;
		pDlg = GAMEIN->GetChatRoomJoinDlg() ;

		if(!pDlg)
		{
			CHATROOMMGR->Throw_Error("Failed to receive join dialog!!", __FUNCTION__) ;
			return ;
		}

		// ��й�ȣ �Է� ���� Ȱ��ȭ ��Ų��.
		pDlg->SetInputMode(e_JoinDlg_Type_SecretCode) ;
		pDlg->SetActive(TRUE) ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : NumJoin_Syn
//	DESC : ��ȣ �Է����� �����ϱ⸦ ������ �� ó���� �ϴ� �Լ�.
//  DATE : APRIL 8, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::NumJoin_Syn() 
{
	if(CHATROOMMGR->Get_ChatRoomState() == e_State_In_ChatRoom) 
	{
		WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1505)) ;
		return ;
	}

	// ���� â �ޱ�.
	CChatRoomJoinDlg* pDlg = NULL ;
	pDlg = GAMEIN->GetChatRoomJoinDlg() ;

	if(!pDlg)
	{
		CHATROOMMGR->Throw_Error("Failed to receive join dialog!!", __FUNCTION__) ;	
		return ;
	}

	// ���ȣ �Է� ���� ����â ����.
	pDlg->SetInputMode(e_JoinDlg_Type_RoonIndex) ;
	pDlg->SetActive(TRUE) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Refresh_Syn
//	DESC : ���ΰ�ħ ��û�� ó���ϴ� �Լ�.
//  DATE : APRIL 8, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::Refresh_Syn()
{
	//// ���� ��Ʈ�� üũ.
	//ASSERT(m_pLoad_Chatroom) ;

 //	if(!m_pLoad_Chatroom) 
 //	{
	//	CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
	//	return ;
 //	}

	//m_pLoad_Chatroom->SetActive(TRUE) ;

	// ���� �ð� üũ.
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

	// �ð� ������Ʈ.
	CHATROOMMGR->UpdateMyLastComTime() ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Btn_Prev
//	DESC : ���� ������ ��û�� �ϴ� �Լ�.
//  DATE : APRIL 8, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::Btn_Prev() 
{
	// ������ Ȯ��.
	if(m_byCurPage == 0) return ;
	else
	{
		//// ���� ��Ʈ�� üũ.
		//ASSERT(m_pLoad_Chatroom) ;

 	//	if(!m_pLoad_Chatroom) 
 	//	{
		//	CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		//	return ;
 	//	}

		//m_pLoad_Chatroom->SetActive(TRUE) ;

		// ���� �ð� üũ.
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
		//	// ������ ��û.
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
		//	// ������ ��û.
		//	MSG_BYTE2 msg ;
		//	memset(&msg, 0, sizeof(MSG_BYTE2)) ;

		//	msg.Category		= MP_CHATROOM ;
		//	msg.Protocol		= MP_CHATROOM_ROOM_SYN ;
		//	msg.dwObjectID		= HEROID ;

		//	msg.bData1			= m_byPrintMode ;
		//	msg.bData2			= m_byCurPage-1 ;

		//	NETWORK->Send( &msg, sizeof(MSG_BYTE2) ) ;
		//}

		// �ð� ������Ʈ.
		CHATROOMMGR->UpdateMyLastComTime() ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : Btn_Next
//	DESC : ���� ������ ��û�� �ϴ� �Լ�.
//  DATE : APRIL 8, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::Btn_Next()
{
	// ������ Ȯ��.
	if(m_byCurPage == m_byMaxPage) return ;
	else
	{
		//// ���� ��Ʈ�� üũ.
		//ASSERT(m_pLoad_Chatroom) ;

 	//	if(!m_pLoad_Chatroom) 
 	//	{
		//	CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		//	return ;
 	//	}

		//m_pLoad_Chatroom->SetActive(TRUE) ;

		// ���� �ð� üũ.
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
		//	// ������ ��û.
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
		//	// ������ ��û.
		//	MSG_BYTE2 msg ;
		//	memset(&msg, 0, sizeof(MSG_BYTE2)) ;

		//	msg.Category		= MP_CHATROOM ;
		//	msg.Protocol		= MP_CHATROOM_ROOM_SYN ;
		//	msg.dwObjectID		= HEROID ;

		//	msg.bData1			= m_byPrintMode ;
		//	msg.bData2			= m_byCurPage+1 ;

		//	NETWORK->Send( &msg, sizeof(MSG_BYTE2) ) ;
		//}

		// �ð� ������Ʈ.
		CHATROOMMGR->UpdateMyLastComTime() ;
	}
}





////-------------------------------------------------------------------------------------------------
////	NAME : SortRoomByType
////	DESC : ������ �з��� ä�ù��� ��Ƽ� ����Ѵ�.
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
//	// ä�ù��� ��� �ִ� Map ���� �ޱ�.
//	LIST_ROOM::iterator it ;
//	it = m_Room.begin() ;
//
//	// ä�ù� ������ �޴´�.
//	BYTE byCount = 0 ;
//	byCount = m_Room.size() ;
//
//	// ä�ù��� ������ ���� ���.
//	if(byCount == 0 || it == m_Room.end())
//	{
//		WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1490)) ;
//		return ;
//	}
//
//	// �ӽ÷� ä�ù��� ��� �ִ� Map�� ����.
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
//	// �˻� �� ä�ù��� ��� �ִ� Map�� ���� �ޱ�.
//	LIST_ROOM::iterator it_search ;
//	it_search = m_TempRoom.begin() ;
//
//	// �˻� �� ������ ������ �޽��� �ڽ� ���.
//	if(it_search == m_TempRoom.end())
//	{
//		WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1492)) ;
//		return ;
//	}
//
//	// ä�ù� ����Ʈ ���̾�α׸� ��� ����.
//	m_pRoomList->RemoveAll() ;
//
//	// ä�ù� ����Ʈ ���̾�α��� �������� ���̴� ������ �޴´�.	
//	byCount = 0 ;
//	byCount = m_TempRoom.size() ;
//
//	char tempBuf[256] = {0,} ;
//	char tempNum[10] = {0,} ;
//
//	BYTE byCurGuestCount ;
//	for(BYTE count = 0 ; count < byCount ; ++count)
//	{
//		// ������ ��ȿ�� �˻�.
//		if(it_search == m_TempRoom.end()) break ;
//
//		// �ӽ� ������ �ʱ�ȭ.
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
//		// ����/����� ����.
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
//		// ä�ù� �з� ����.
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
//		// ä�ù� ���� ����.
//		strcat(tempBuf, it_search->title) ;
//
//		// ���� ������ �ο� ����.
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
//		// �ִ� ������ ����.
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
//		// ä�ù� ����Ʈ ���̾�α׿� �߰�.
//		m_pRoomList->AddItem( tempBuf, RGBA_MAKE(255, 255, 255, 255), count ) ;
//
//		++it_search ;
//	}
//
//	//// �ε� �ؽ�Ʈ ��Ȱ��ȭ.
//	//if(m_pLoad_Chatroom->IsActive())
//	//{
//	//	m_pLoad_Chatroom->SetActive(FALSE) ;
//	//}
//
//	// ���� ��µǰ� �ִ� ������ �˻��� �����̶�� ����.
//	//m_byPrintMode = e_PrintMode_Searced ;
//}





//-------------------------------------------------------------------------------------------------
//	NAME : Check_Room_To_Join
//	DESC : ����â/��й�ȣâ���� OK��ư�� ������ �� ó���ϴ� �Լ�.
//  DATE : APRIL 8, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::Check_Room_To_Join(BYTE inputMode, char* pCode)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pCode) ;

	if(!pCode || strlen(pCode) == 0)
	{
		//CHATROOMMGR->Throw_Error(err_IMP, __FUNCTION__) ;
		WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1510)) ;
		return ;
	}

	// ����â �ޱ�.
	CChatRoomJoinDlg* pDlg = NULL ;
	pDlg = GAMEIN->GetChatRoomJoinDlg() ;

	ASSERT(pDlg) ;

	if(!pDlg)
	{
		CHATROOMMGR->Throw_Error("Failed to receive join dialog!!", __FUNCTION__) ;
		return ;
	}

	// ä�ù��� ��� Map�� ���� �ޱ�.
	LIST_ROOM::iterator it ;
	it = m_Room.begin() ;

	// ���� ������ �޽��� �ڽ� ���.
	if(it == m_Room.end())
	{
		WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1490)) ;

		m_pRoomList->RemoveAll() ;

		m_Room.clear() ;
		m_TempRoom.clear() ;

		return ;
	}

	// ���ȣ �Է� ����, 
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

			// ������ �� ����.
			m_SelectRoom = *it ;

			// ����/����� Ȯ��.
			if(it->bySecretMode == e_RM_Close)
			{
				// ��й�ȣ â�� Ȱ��ȭ �Ѵ�.
				pDlg->SetInputMode(e_JoinDlg_Type_SecretCode) ;
				pDlg->SetActive(TRUE) ;
				return ;
			}
		}
		//	else
		//	{
		//		// �������̸�, ���� ��û.
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

		//// ������� ���� ��ġ�ϴ� ���� ���°�.
		//pDlg->SetActive(FALSE) ;
		//WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1507)) ;


		// ������� ��ġ�ϴ� ���� ���ٸ�, �ܼ� ���ȣ �Է� �α��� ��û�̴�.
		MSG_CR_JOIN_SYN msg ;
		memset(&msg, 0, sizeof(MSG_CR_JOIN_SYN)) ;

		msg.Category	= MP_CHATROOM ;
		msg.Protocol	= MP_CHATROOM_JOIN_ROOM_SYN ;
		msg.dwObjectID	= HEROID ;

		msg.byRoomIdx	= (BYTE)atoi(pCode) ;

		NETWORK->Send( &msg, sizeof(MSG_CR_JOIN_SYN) ) ;

		pDlg->SetActive(FALSE) ;
	}
	// ��й�ȣ �Է� ����,
	else if(inputMode == e_JoinDlg_Type_SecretCode)
	{
		// ��й�ȣ�� ��� ���� ��û�� �Ѵ�.
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
//	DESC : ������ ���� �Ѿ� ��, ä�ù� ������ �����ϴ� �Լ�.
//		 : 
//		 : 1. ä�ù��� ��� �ִ� Map�� ����.
//		 : 2. �������� �޴´�.
//		 : 3. ������ ����.
//		 : 4. �ε� �ؽ�Ʈ ��Ȱ��ȭ.
//		 : 5. ä�ù� ����Ʈ ���̾�α� ���ΰ�ħ.
//		 : 
//  DATE : APRIL 9, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::Init_RoomList(void* pMsg)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		CHATROOMMGR->Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// ���� �޽��� ��ȯ
	MSG_CR_ROOMLIST* pmsg = NULL ;
	pmsg = (MSG_CR_ROOMLIST*)pMsg ;

	ASSERT(pmsg) ;

	if(!pmsg)
	{
		CHATROOMMGR->Throw_Error(err_FCMTO, __FUNCTION__) ;
		return ;
	}

	// ���� ��Ʈ�� üũ.
	ASSERT(m_pRoomList) ;
	//ASSERT(m_pLoad_Chatroom) ;

	if(!m_pRoomList /*|| !m_pLoad_Chatroom*/)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// ä�ù� Ÿ�� ����.
	m_pRoomType->SetCurSelectedIdx(pmsg->byRoomType) ;
	m_pRoomType->SelectComboText(pmsg->byRoomType) ;
	m_byRoomType = pmsg->byRoomType ;

	// ä�ù� ����Ʈ ���.
	m_Room.clear() ;

	// ä�ù� ���� ��ŭ ����.
	for(BYTE count = 0 ; count < pmsg->byCount ; ++count)
	{
		if(pmsg->room[count].byIdx == 0) continue ;

		const ST_CR_ROOM_CLT& room = pmsg->room[ count ] ;

		m_Room.push_back(room) ;
	}

	// ä�ù� ����Ʈ ������ ����.
	SetPage(pmsg->byCurPage, pmsg->byTotalPage) ;

	// ä�ù� ����Ʈ ���̾�α� ���ΰ�ħ.
	Refresh_RoomList() ;

	//// �ε� �̹��� ��Ȱ��ȭ.
	//if(m_pLoad_Chatroom->IsActive())
	//{
	//	m_pLoad_Chatroom->SetActive(FALSE) ;
	//}

	//m_byPrintMode = e_PrintMode_Normal ;
}





////-------------------------------------------------------------------------------------------------
////	NAME : Init_TypeRoomList
////	DESC : ä�ù� �з��� ���� ä�ù��� �ʱ�ȭ �ϴ� �Լ�.
////  DATE : APRIL 9, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomMainDlg::Init_TypeRoomList(void* pMsg)
//{
//	// �Լ� �Ķ���� üũ.
//	ASSERT(pMsg) ;
//
//	if(!pMsg)
//	{
//		CHATROOMMGR->Throw_Error(err_IMP, __FUNCTION__) ;
//		return ;
//	}
//
//	// ���� �޽��� ��ȯ
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
//	// ���� ��Ʈ�� üũ.
//	ASSERT(m_pRoomList) ;
//	//ASSERT(m_pLoad_Chatroom) ;
//
//	if(!m_pRoomList /*|| !m_pLoad_Chatroom*/)
//	{
//		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
//		return ;
//	}
//
//	// ä�ù� ����Ʈ ���.
//	m_Room.clear() ;
//
//	// ä�ù� ���� ��ŭ ����.
//	for(BYTE count = 0 ; count < ROOM_COUNT_PER_PAGE ; ++count)
//	{
//		if(pmsg->room[count].byIdx == 0) continue ;
//
//		const ST_CR_ROOM_CLT& room = pmsg->room[ count ] ;
//
//		m_Room.push_back(room) ;
//	}
//
//	// ä�ù� ����Ʈ ������ ����.
//	SetPage(pmsg->byCurPage, pmsg->byTotalPage) ;
//
//	// ä�ù� ����Ʈ ���̾�α� ���ΰ�ħ.
//	Refresh_RoomList() ;
//}





//-------------------------------------------------------------------------------------------------
//	NAME : Refresh_RoomList
//	DESC : ä�ù� ����Ʈ�� ���ΰ�ħ�ϴ� �Լ�.
//  DATE : APRIL 9, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::Refresh_RoomList()
{
	// ���� ��Ʈ�� üũ.
	ASSERT(m_pRoomList) ;

	if(!m_pRoomList)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}

	// Ȱ��ȭ ���� üũ.
	if(!IsActive()) return ;



	// ����Ʈ ���̾�α� ����.
	m_pRoomList->RemoveAll() ;

	// �� ��ġ �ޱ�.
	LIST_ROOM::iterator it ;
	it = m_Room.begin() ;
	if(it == m_Room.end()) return ;

	// �ӽ� ������ ����.
	char tempBuf[256] = {0,} ;
	char tempNum[10] = {0,} ;

	BYTE byCount = 0 ;
	byCount = m_Room.size() ;

	BYTE byStartCount = 0 ;
	byStartCount = m_byCurPage*ROOM_COUNT_PER_PAGE ;



	// �� ���� ��ŭ ����.
	for(BYTE count = 0 ; count < byCount ; ++count)
	{
		if(it == m_Room.end()) return ;

		memset(tempBuf, 0, sizeof(tempBuf)) ;
		memset(tempNum, 0, sizeof(tempNum)) ;

		// �ε��� ����.
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

		// ����/����� ����.
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

		// ä�ù� �з� ����.
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

		// ä�ù� ���� ����.
		strcat(tempBuf, it->title) ;

		// ���� ������ ����.
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

		// �ִ� ������ ����.
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

		// ����Ʈ ���̾�α׿� �߰�.
		m_pRoomList->AddItem( tempBuf, RGBA_MAKE(255, 255, 255, 255), count ) ;

		++it ;
	}

	//// ���� �� ���� ����.
	//it = m_Room.begin() ;

	//ASSERT(it != m_Room.end()) ;

	//const ST_CR_ROOM_CLT& selectRoom = *it ;

	//m_SelectRoom = selectRoom ;

	//if(!IsActive()) SetActive(TRUE) ;

	//if(m_pLoad_Chatroom->IsActive()) m_pLoad_Chatroom->SetActive(FALSE) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Clear_RoomList
//	DESC : ä�ù� ����Ʈ�� ��� ���� �Լ�.
//  DATE : APRIL 11, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::Clear_RoomList()
{
	// ���� ��Ʈ�� üũ.
	ASSERT(m_pRoomList) ;

	if(!m_pRoomList)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}

	// ä�ù� ����Ʈ ���̾�α� ����.
	m_pRoomList->RemoveAll() ;

	// ä�ù� ����Ʈ ����.
	m_Room.clear() ;

	// ���� �� �� ���� �����.
	memset(&m_SelectRoom, 0, sizeof(ST_CR_ROOM_CLT)) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Add_RoomInfo
//	DESC : �� ������ �߰��ϴ� �Լ�.
//  DATE : APRIL 9, 2008 LYW
//-------------------------------------------------------------------------------------------------
//void CChatRoomMainDlg::Add_RoomInfo(void* pMsg)
//{
//	// �Լ� �Ķ���� üũ.
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
//	// ���� �޽��� ��ȯ.
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
//	// ���� ��Ʈ�� üũ.
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
//	// ������ �ޱ�.
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
//	// ������ �߰�.
//	const ST_CHATROOM_ROOM& addInfo = room ;
//
//	m_Room.push_back(addInfo) ;
//
//	// ä�ù� ����Ʈ ���� ��ħ.
//	Refresh_RoomList() ;
//	//SetPage(pmsg->byPage, pmsg->byTotalPage) ;
//
//	//// ä�ù� ��й�ȣ ����.
//	//CHATROOMMGR->SetMyRoomCode(pmsg->wCode) ;
//
//	//// ä��â ���� �ε��� ����.
//	//CHATROOMMGR->SetMyRoomOwnerIdx(pmsg->baseInfo.dwOwnerIdx) ;
//
//	//// ä�ù� �ɼ� ����/��ȯ.
//	//CHATROOMMGR->SetMyRoomOption(pmsg->wCode, &pmsg->baseInfo) ;
//
//	// ä��â Ȱ��ȭ.
//	Active_ChatRoom(pmsg) ;
//}





//-------------------------------------------------------------------------------------------------
//	NAME : Add_MyRoom
//	DESC : ä�ù� ����Ʈ�� �� �� ������ �߰��Ѵ�.
//  DATE : APRIL 12, 2008 LYW
//-------------------------------------------------------------------------------------------------
BOOL CChatRoomMainDlg::Add_Room(ST_CR_ROOM_CLT* pInfo)
{
	// �Լ� �Ķ���� üũ.
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

	//// ���� �߰� �� ���� ���� ���.
	//if(it == m_Room.end())
	//{
	//	const ST_CR_ROOM_CLT& addInfo = *pInfo ;

	//	m_Room.push_back(addInfo) ;
	//}
	//else
	//{
	//	// �� ���� ��ŭ for��.
	//	for(BYTE count = 0 ; count < bySize ; ++count)
	//	{
	//		// �̹� �ִ� ���̶��,
	//		if(it->dwOwnerIdx == pInfo->info.dwOwnerIdx)
	//		{
	//			// ������ �ޱ�.
	//			it->info = pInfo->info ;

	//			// ���� ���� �ޱ�.
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

	//// ä�ù� ����Ʈ ���� ��ħ.
	//Refresh_RoomList() ;

	//return TRUE ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Update_RoomInfo
//	DESC : �� ���� �ϳ��� ������Ʈ �� �� ó���ϴ� �Լ�.
//  DATE : APRIL 9, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::Update_RoomInfo(ST_CR_ROOM_CLT* pInfo)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pInfo) ;

	if(!pInfo)
	{
		CHATROOMMGR->Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}

	// �渮��Ʈ�� ������ �̴������� �ޱ�.
	LIST_ROOM::iterator it ;
	it = m_Room.begin() ;

	// ����Ʈ ������ �ޱ�.
	if(it == m_Room.end()) return ;

	BYTE bySize = 0 ;
	bySize = m_Room.size() ;

	// ������Ʈ �� �� ã��.
	for(BYTE count = 0 ; count < bySize ; ++count)
	{
		if(it->byIdx != pInfo->byIdx) continue ;

		// ������ ������Ʈ.
		it->byIdx				= pInfo->byIdx ;
		it->bySecretMode		= pInfo->bySecretMode ;
		it->byRoomType			= pInfo->byRoomType ;
		it->byCurGuestCount		= pInfo->byCurGuestCount ;
		it->byTotalGuestCount	= pInfo->byTotalGuestCount ;

		SafeStrCpy(it->title, pInfo->title, TITLE_SIZE) ;

		break ;
	}

	// ä�ù� ����Ʈ ������Ʈ.
	Refresh_RoomList() ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : DeleteRoom
//	DESC : ���ڷ� �Ѿ�� ���� ã�Ƽ�, ����Ʈ���� �����Ѵ�.
//  DATE : APRIL 9, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::DeleteRoom(BYTE byRoomIdx)
{
	// �渮��Ʈ�� ������ �̴������� �ޱ�.
	LIST_ROOM::iterator it ;
	it = m_Room.begin() ;

	// ����Ʈ ������ �ޱ�.
	if(it == m_Room.end()) return ;

	BYTE bySize = 0 ;
	bySize = m_Room.size() ;

	// ������Ʈ �� �� ã��.
	for(BYTE count = 0 ; count < bySize ; ++count)
	{
		if(it->byIdx != byRoomIdx) continue ;

		m_Room.erase(it) ;
		break ;
	}

	// ä�ù� ����Ʈ ������Ʈ.
	Refresh_RoomList() ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : SetPage
//	DESC : ä�ù� ����Ʈ�� �������� �����Ѵ�.
//  DATE : APRIL 9, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomMainDlg::SetPage(BYTE num1, BYTE num2)
{
	// ä�ù� ����Ʈ ������ ����.
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
//	DESC : ���ڷ� �Ѿ�� ��ȣ�� ���� ã�Ƽ� ��ȯ�Ѵ�.
//  DATE : APRIL 9, 2008 LYW
//-------------------------------------------------------------------------------------------------

ST_CR_ROOM_CLT* CChatRoomMainDlg::GetRoomListItem(BYTE byRoomIdx)
{
	// �渮��Ʈ�� ������ �̴������� �ޱ�.
	LIST_ROOM::iterator it ;
	it = m_Room.begin() ;

	// ����Ʈ ������ �ޱ�.
	if(it == m_Room.end()) return NULL ;

	BYTE bySize = 0 ;
	bySize = m_Room.size() ;

	// ������Ʈ �� �� ã��.
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
















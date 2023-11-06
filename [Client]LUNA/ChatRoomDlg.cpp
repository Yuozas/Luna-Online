//=================================================================================================
//	FILE		: CChatRoomDlg.cpp
//	DESC		: Implementation part of CChatRoomDlg class.
//	PROGRAMMER	: Yongs Lee
//	DATE		: February 25, 2008
//=================================================================================================




//-------------------------------------------------------------------------------------------------
//		Include header files.
//-------------------------------------------------------------------------------------------------
#include "stdafx.h"

#include "../WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "../Interface/cResourceManager.h"

#include "./GameIn.h"

#include "./ChatManager.h"
#include "./ChatRoomMgr.h"

#include "./ChatRoomDlg.h"
#include "./ChatRoomGuestListDlg.h"
#include "./ChatRoomOptionDlg.h"

#include "./Input/Mouse.h"
#include "./Input/Keyboard.h"

#include "./Interface/cButton.h"
#include "./Interface/cEditBox.h"
#include "./Interface/cListDialog.h"
#include "./Interface/cStatic.h"
//#include "./Interface/cTextArea.h"

#include "./cMsgBox.h"
#include "Interface/cScriptManager.h"
#include "./FilteringTable.h"
#include "./ObjectManager.h"








//-------------------------------------------------------------------------------------------------
//	NAME : CChatRoomDlg
//	DESC : 생성자 함수.
//-------------------------------------------------------------------------------------------------
CChatRoomDlg::CChatRoomDlg(void)
{
	// 초기화 처리.
	m_pRoomTitle		= NULL ;

	m_pChatList			= NULL ;

	m_pInputBox			= NULL ;

	m_pBtn_GuestList	= NULL ;
	m_pBtn_Option		= NULL ;

	m_pBtn_Close		= NULL ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : ~CChatRoomDlg
//	DESC : 소멸자 함수.
//-------------------------------------------------------------------------------------------------
CChatRoomDlg::~CChatRoomDlg(void)
{
}





//-------------------------------------------------------------------------------------------------
//	NAME : Linking
//	DESC : 하위 컨트롤 링크 함수.
//-------------------------------------------------------------------------------------------------
void CChatRoomDlg::Linking()
{
	// 하위 컨트롤 링크.
	m_pRoomTitle		= (cStatic*)GetWindowForID(CRD_STC_ROOMTITLE) ;

	m_pChatList			= (cListDialog*)GetWindowForID(CRD_LST_CHATLIST) ;

	m_pInputBox			= (cEditBox*)GetWindowForID(CRD_EDB_INPUTBOX) ;
	//m_pInputBox			= (cTextArea*)GetWindowForID(CRD_EDB_INPUTBOX) ;

	m_pBtn_GuestList	= (cButton*)GetWindowForID(CRD_BTN_GUESTLIST) ;
	m_pBtn_Option		= (cButton*)GetWindowForID(CRD_BTN_OPTION) ;

	m_pBtn_Close		= (cButton*)GetWindowForID(CRD_BTN_CLOSE_DLG) ;



	// 하위 컨트롤 체크.
	ASSERT(m_pRoomTitle) ;

	ASSERT(m_pChatList) ;
	
	ASSERT(m_pInputBox) ;

	ASSERT(m_pBtn_GuestList) ;
	ASSERT(m_pBtn_Option) ;

	ASSERT(m_pBtn_Close) ;

	if(!m_pRoomTitle || !m_pChatList || !m_pInputBox || !m_pBtn_GuestList || !m_pBtn_Option || !m_pBtn_Close)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// 채팅방 매니져에 등록.
	CHATROOMMGR->SetChatRoomDlg(this) ;
}





////-------------------------------------------------------------------------------------------------
////	NAME : ActionEvent
////	DESC : The function to handling mouse event.
////-------------------------------------------------------------------------------------------------
//DWORD CChatRoomDlg::ActionEvent(CMouse* mouseInfo)
//{
//	DWORD we = WE_NULL ;
//
//	if( !m_bActive ) return we ;
//
//	we = cDialog::ActionEvent(mouseInfo) ;
//
//	//if(m_pChatList->PtInWindow((LONG)mouseInfo->GetMouseX(), (LONG)mouseInfo->GetMouseY()))
//	//{
//	//	// 휠 이벤트를 받는다.
//	//	int Wheel = mouseInfo->GetWheel() ;
//
//	//	if( Wheel > 0 )
//	//	{
//	//		m_pChatList->OnUpwardItem() ;
//	//	}
//	//	else
//	//	{
//	//		m_pChatList->OnDownwardItem() ;
//	//	}
//	//}
//
//	//if(m_pInputBox->PtInWindow((LONG)mouseInfo->GetMouseX(), (LONG)mouseInfo->GetMouseY()))
//	//{
//	//	if(we & WE_LBTNCLICK)
//	//	{
//	//		m_pInputBox->SetReadOnly(FALSE) ; 
//	//		m_pInputBox->SetFocusEdit(TRUE) ;
//	//	}
//	//}
//
//	return we ;
//}





//-------------------------------------------------------------------------------------------------
//	NAME : ActionKeyboardEvent
//	DESC : The function to handling keyboard event.
//  DATE : APRIL 8, 2008 LYW
//-------------------------------------------------------------------------------------------------
//DWORD CChatRoomDlg::ActionKeyboardEvent( CKeyboard * keyInfo )
//{
//	DWORD we = WE_NULL ;
//
//	if( !m_bActive ) return we ;
//
//	we |= cDialog::ActionKeyboardEvent(keyInfo) ;
//
//	ASSERT(m_pInputBox) ;
//
//	if(!m_pInputBox)
//	{
//		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
//		return we ;
//	}
//
//	if(m_pInputBox->IsFocus())
//	{
//		if( keyInfo->GetKeyPressed(KEY_RETURN) || keyInfo->GetKeyPressed(KEY_PADENTER) )
//		{
//			char tempMsg[MAX_CHAT_LENGTH+1] = {0, } ;
//			SafeStrCpy(tempMsg, m_pInputBox->GetEditText(), MAX_CHAT_LENGTH) ;
//
//			if(strlen(tempMsg) == 0) return we ;
//
//			// 필터링 처리.
//			if( FILTERTABLE->FilterChat(tempMsg) )
//			{
//				AddMsg(CHATMGR->GetChatMsg(1225), e_ChatMsg_System) ;
//				m_pInputBox->SetEditText("") ;
//				return we ;
//			}
//
//			if( gCurTime - CHATROOMMGR->GetChatListTime() < 1000 )	// 1초
//			{
//				AddMsg(CHATMGR->GetChatMsg(248), e_ChatMsg_System) ;
//				m_pInputBox->SetEditText("") ;
//				return we ;
//			}
//			else
//			{
//				// 마지막 채팅 시간 업데이트.
//				CHATROOMMGR->UpdateChatLastTime(gCurTime) ;
//
//				// 메시지 전송 처리.
//				MSG_CR_MSG_BROADCAST msg ;
//				memset(&msg, 0, sizeof(MSG_CR_MSG_BROADCAST)) ;
//
//				msg.Category	= MP_CHATROOM ;
//				msg.Protocol	= MP_CHATROOM_CHATMSG_NORMAL_SYN ;
//				msg.dwObjectID	= HEROID ;
//
//				SafeStrCpy(msg.name, HERO->GetObjectName(), MAX_NAME_LENGTH) ;
//				SafeStrCpy(msg.Msg, tempMsg, MAX_CHAT_LENGTH+1) ;
//
//				ST_CR_ROOM_SRV* pRoom = NULL ;
//				pRoom = CHATROOMMGR->GetMyRoom() ;
//
//				ASSERT(pRoom) ;
//
//				if(!m_pInputBox)
//				{
//					CHATROOMMGR->Throw_Error("Failed to receive a room info!!", __FUNCTION__) ;
//					return we ;
//				}
//
//				BYTE byCount = 0 ;
//				byCount = pRoom->byCurGuestCount ;
//
//				for(BYTE count = 0 ; count < byCount ; ++count)
//				{
//					msg.dwUser[count] = pRoom->dwUser[count] ;
//
//					++msg.byCount ;
//				}
//
//				NETWORK->Send( &msg, sizeof(MSG_CR_MSG_BROADCAST)) ;
//				m_pInputBox->SetEditText("") ;
//			}
//		}
//	}
//
//	return we ;
//}





//-------------------------------------------------------------------------------------------------
//	NAME : OnActionEvent
//	DESC : EventFunc로 부터 넘어온 Event를 처리하는 함수.
//-------------------------------------------------------------------------------------------------
void CChatRoomDlg::OnActionEvent(LONG id, void* p, DWORD event)
{
	// 함수 파라메터 체크.
	ASSERT(p) ;

	if(!p)
	{
		CHATROOMMGR->Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}



	// 하위 컨트롤 체크.
	ASSERT(m_pBtn_GuestList) ;
	ASSERT(m_pBtn_Option) ;

	if(!m_pBtn_GuestList || !m_pBtn_Option)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// 컨트롤 아이디 체크.
	switch(id)
	{
	// 참여자 버튼.
	case CRD_BTN_GUESTLIST : 
		{
			// 참여자 다이얼로그 받기.
			CChatRoomGuestListDlg* pDlg = NULL ;
			pDlg = GAMEIN->GetChatRoomGuestListDlg() ;

			ASSERT(pDlg) ;

			if(!pDlg)
			{
				CHATROOMMGR->Throw_Error("Failed to receive gld!!", __FUNCTION__) ;
				return ;
			}

			// 참여자 다이얼로그 열기/닫기 처리.
			pDlg->SetActive(!pDlg->IsActive()) ;
		}
		break ;

	// 채팅 옵션 버튼.
	case CRD_BTN_OPTION :
		{
			// 방장인지 체크.
			if(CHATROOMMGR->GetMyRoomOwnerIdx() != HEROID)
			{
				AddMsg(CHATMGR->GetChatMsg(1487), e_ChatMsg_System) ;
				return ;
			}

			// 옵션 다이얼로그 받기.
			CChatRoomOptionDlg* pDlg = NULL ;
			pDlg = GAMEIN->GetChatRoomOptionDlg() ;

			ASSERT(pDlg) ;

			if(!pDlg)
			{
				CHATROOMMGR->Throw_Error("Failed to receive od!!", __FUNCTION__) ;
				return ;
			}

			// 옵션창 열기/닫기 처리.
			pDlg->SetActive(!pDlg->IsActive()) ;
		}
		break ;

	// 채팅방 닫기 버튼.
	case CRD_BTN_CLOSE_DLG :
		{
			// 종료 확인 알림창 활성화.
			WINDOWMGR->MsgBox( MBI_CR_ENDCHAT, MBT_YESNO, CHATMGR->GetChatMsg(1480) ) ;
		}
		break ;

	default : break ;
	}
}




////-------------------------------------------------------------------------------------------------
////	NAME : SetActive
////	DESC : 채팅방을 비활성화 하게 될 경우, 대화상대, 옵션창도 같이 닫는다.
////  DATE : APRIL 16, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomDlg::SetActive(BOOL val)
//{
//	if(!val)
//	{
//		//// 대화상대 리스트 다이얼로그 받기.
//		//CChatRoomGuestListDlg* pGuestListDlg = NULL ;
//		//pGuestListDlg = GAMEIN->GetChatRoomGuestListDlg() ;
//
//		//ASSERT(pGuestListDlg) ;
//
//		//if(!pGuestListDlg)
//		//{
//		//	CHATROOMMGR->Throw_Error("Failed to receive guest list dlg!!", __FUNCTION__) ;
//		//	return ;
//		//}
//
//		// 초기화 하고 닫는다.
//		//pGuestListDlg->SetDlgToDefault() ;
//		//pGuestListDlg->SetActive(FALSE) ;
//
//		// 옵션 다이얼로그 받기.
//		//CChatRoomOptionDlg* pOptionDlg = NULL ;
//		//pOptionDlg = GAMEIN->GetChatRoomOptionDlg() ;
//
//		//ASSERT(pOptionDlg) ;
//
//		//if(!pOptionDlg)
//		//{
//		//	CHATROOMMGR->Throw_Error("Failed to receive option dlg!!", __FUNCTION__) ;
//		//	return ;
//		//}
//
//		//// 초기화 하고 닫는다.
//		//pOptionDlg->SetDlgToDefault() ;
//		//pOptionDlg->SetActive(FALSE) ;
//
//		// 채팅방을 초기화 한다.
//		SetDlgToDefault() ;
//	}
//
//	cDialog::SetActive(val) ;
//}





////-------------------------------------------------------------------------------------------------
////	NAME : Render
////	DESC : 채팅다이얼로그를 렌더하는 함수.
////  DATE : APRIL 8, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomDlg::Render()
//{
//	if( !IsActive() ) return;
//
//	cDialog::Render();
//}





//-------------------------------------------------------------------------------------------------
//	NAME : AddMsg
//	DESC : 채팅방 내에 메시지를 추가하는 함수.
//-------------------------------------------------------------------------------------------------
void CChatRoomDlg::AddMsg(char* pMsg, BYTE byType)
{
	// 함수 파라메터 체크.
	ASSERT(pMsg) ;

	if(!pMsg) 
	{
		CHATROOMMGR->Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}



	// 메시지 길이에 따라 적당히 잘라서 채팅방 리스트에 추가한다.
	char buf[ MAX_TEMPBUFF ] = {0,} ;

	int nLen	= strlen( pMsg ) ;
	int nCpyNum = 0 ;
	int count = 0 ;

	int ntextLen = 54 ;

	while( nLen > ntextLen )
	{
		if( (pMsg + ntextLen ) != CharNext( CharPrev( pMsg, pMsg + ntextLen ) ) )
		{
			nCpyNum = ntextLen - 1 ;
		}
		else
		{
			nCpyNum = ntextLen ;
		}

		strncpy( buf, pMsg, nCpyNum ) ;
		buf[ nCpyNum ] = 0 ;

		m_pChatList->AddItem( buf, GetMsgColor(byType) ) ;

		nLen -= nCpyNum ;
		pMsg  += nCpyNum ;

		if( *pMsg == ' ' ) ++pMsg ;
	}

	if( nLen > 0 )
	{
		m_pChatList->AddItem( pMsg, GetMsgColor(byType) ) ;
	}

	//m_pChatList->AddItem(pMsg, GetMsgColor(byType)) ;
}





////-------------------------------------------------------------------------------------------------
////	NAME : AddMsg
////	DESC : The function to handling event message.
////-------------------------------------------------------------------------------------------------
//void CChatRoomDlg::AddMsg(char* pMsg, DWORD dwColor)
//{
//	ASSERT(pMsg) ;
//
//	if(!pMsg) 
//	{
//#ifdef _MB_CRD_
//		MessageBox( NULL, "Invalid a message parameter!!", "CRD-AMC", MB_OK) ;
//#endif //_MB_CRD_
//
//		return ;
//	}
//
//	m_pChatList->AddItem(pMsg, dwColor) ;
//}





//-------------------------------------------------------------------------------------------------
//	NAME : AddCommonMsg
//	DESC : 메시지 타입과 함께 메시지를 추가하는 함수.
//-------------------------------------------------------------------------------------------------
void CChatRoomDlg::AddCommonMsg(BYTE byType, char* pMsg)
{
	// 함수 파라메터 체크.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		CHATROOMMGR->Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}



	// 메시지 타입 확인.
	char szStr[128] = {0,} ;

	strcat(szStr, "[") ;
	strcat(szStr, "<") ;
	switch(byType)
	{
	// 채팅방 입장.
	case e_ChatMsg_StepIn :
		{
			strcat(szStr, pMsg) ;
			strcat(szStr, ">") ;
			strcat(szStr, CHATMGR->GetChatMsg(1481)) ;
		}
		break ;

	// 채팅방 퇴장.
	case e_ChatMsg_StepOut :
		{
			strcat(szStr, pMsg) ;
			strcat(szStr, ">") ;
			strcat(szStr, CHATMGR->GetChatMsg(1482)) ;
		}
		break ;

	default : break ;
	}
	strcat(szStr, "]") ;

	// 메시지 출력.
	m_pChatList->AddItem(szStr, GetMsgColor(byType)) ;
}





////-------------------------------------------------------------------------------------------------
////	NAME : SetRoomTitle
////	DESC : The function to print room title.
////-------------------------------------------------------------------------------------------------
//void CChatRoomDlg::SetRoomTitle(char* pMsg)
//{
//	ASSERT(pMsg) ;
//
//	if(!pMsg)
//	{
//        CHATROOMMGR->Throw_Error(err_IMP, __FUNCTION__) ;
//		return ;
//	}
//
//	m_pRoomTitle->SetStaticText(pMsg) ;
//}





//-------------------------------------------------------------------------------------------------
//	NAME : SetRoomTitle
//	DESC : 채팅방 제목을 세팅하는 함수.
//	DATE : APRIL 3, 2008
//-------------------------------------------------------------------------------------------------
void CChatRoomDlg::SetRoomTitle(ST_CR_ROOM_CLT* pInfo)
{
	// 함수 파라메터 체크.
	ASSERT(pInfo) ;

	if(!pInfo)
	{
		CHATROOMMGR->Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}



	// 방 번호 세팅.
	char tempMsg[128] = {0, } ;
	char tempRoomNum[12] = {0, } ;
	strcat(tempMsg, "[") ;
	sprintf(tempRoomNum, CHATMGR->GetChatMsg(1483), pInfo->byIdx) ;
	strcat(tempMsg, tempRoomNum) ;
	strcat(tempMsg, "]") ;
	strcat(tempMsg, "-") ;



	// 공개/비공개 세팅.
	strcat(tempMsg, "[") ;
	switch(pInfo->bySecretMode)
	{
	case e_RM_Open :	strcat(tempMsg, RESRCMGR->GetMsg(892)) ; break ;
	case e_RM_Close :	strcat(tempMsg, RESRCMGR->GetMsg(893)) ; break ;
	default : break ;
	}
	strcat(tempMsg, "]") ;



	// 채팅방 분류 세팅.
	strcat(tempMsg, "[") ;
	switch(pInfo->byRoomType)
	{
	case e_RT_Party :			strcat(tempMsg, RESRCMGR->GetMsg(880)) ;	break ;
	case e_RT_Deal :			strcat(tempMsg, RESRCMGR->GetMsg(881)) ;	break ;
	case e_RT_Normal_Chat :		strcat(tempMsg, RESRCMGR->GetMsg(882)) ;	break ;
	case e_RT_Local_Chat :		strcat(tempMsg, RESRCMGR->GetMsg(883)) ;	break ;
	case e_RT_Make_Friend :		strcat(tempMsg, RESRCMGR->GetMsg(884)) ;	break ;
	default : break ;
	}
	strcat(tempMsg, "]") ;



	// 채팅방 제목 세팅.
	m_pRoomTitle->SetStaticText(tempMsg) ;



	// 툴팁 세팅.
	m_pRoomTitle->SetToolTip(pInfo->title, RGBA_MAKE(255, 255, 255, 255)) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : GetMsgColor
//	DESC : 메시지 타입에 따라 색상을 반환하는 함수.
//-------------------------------------------------------------------------------------------------
DWORD CChatRoomDlg::GetMsgColor(BYTE byColorType)
{
	// 색상 변수 선언.
	DWORD dwColor = RGBA_MAKE(255, 255, 255, 255) ;



	// 메시지 타입 확인.
	switch(byColorType)
	{
	case e_ChatMsg_StepIn :
	case e_ChatMsg_StepOut :
	case e_ChatMsg_System :				dwColor = RGBA_MAKE(255, 255, 0, 255) ; break ;

	case e_ChatMsg_MyMsg :				dwColor = RGBA_MAKE(0, 255, 0, 255) ; break ;
	case e_ChatMsg_OtherMsg :			dwColor = RGBA_MAKE(255, 255, 255, 255) ; break ;

	case e_ChatMsg_Whisper_Sener :		dwColor = RGBA_MAKE(255, 0, 255, 255) ; break ;
	case e_ChatMsg_Whisper_Receiver :	dwColor = RGBA_MAKE(0, 255, 255, 255) ; break ;
	default : break ;
	}



	// 세팅 된 색상 반환.
	return dwColor ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : CloseChatRoom
//	DESC : 채팅방을 닫는 처리를 하는 함수.
//  DATE : APRIL 29, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomDlg::CloseChatRoom()
{
	SetActive(FALSE) ;
	SetDlgToDefault() ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : SetDlgToDefault
//	DESC : 채팅창을 처음(기본) 상태로 초기화 하는 함수.
//  DATE : APRIL 10, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomDlg::SetDlgToDefault()
{
	// 채팅방 제목 초기화.
	m_pRoomTitle->SetStaticText("") ;

	// 채팅 입력 내용 초기화.
	m_pInputBox->SetEditText("") ;
	//m_pInputBox->SetScriptText("") ;

	// 채팅 내용 초기화.
	m_pChatList->RemoveAll() ;
}
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
//	DESC : ������ �Լ�.
//-------------------------------------------------------------------------------------------------
CChatRoomDlg::CChatRoomDlg(void)
{
	// �ʱ�ȭ ó��.
	m_pRoomTitle		= NULL ;

	m_pChatList			= NULL ;

	m_pInputBox			= NULL ;

	m_pBtn_GuestList	= NULL ;
	m_pBtn_Option		= NULL ;

	m_pBtn_Close		= NULL ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : ~CChatRoomDlg
//	DESC : �Ҹ��� �Լ�.
//-------------------------------------------------------------------------------------------------
CChatRoomDlg::~CChatRoomDlg(void)
{
}





//-------------------------------------------------------------------------------------------------
//	NAME : Linking
//	DESC : ���� ��Ʈ�� ��ũ �Լ�.
//-------------------------------------------------------------------------------------------------
void CChatRoomDlg::Linking()
{
	// ���� ��Ʈ�� ��ũ.
	m_pRoomTitle		= (cStatic*)GetWindowForID(CRD_STC_ROOMTITLE) ;

	m_pChatList			= (cListDialog*)GetWindowForID(CRD_LST_CHATLIST) ;

	m_pInputBox			= (cEditBox*)GetWindowForID(CRD_EDB_INPUTBOX) ;
	//m_pInputBox			= (cTextArea*)GetWindowForID(CRD_EDB_INPUTBOX) ;

	m_pBtn_GuestList	= (cButton*)GetWindowForID(CRD_BTN_GUESTLIST) ;
	m_pBtn_Option		= (cButton*)GetWindowForID(CRD_BTN_OPTION) ;

	m_pBtn_Close		= (cButton*)GetWindowForID(CRD_BTN_CLOSE_DLG) ;



	// ���� ��Ʈ�� üũ.
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



	// ä�ù� �Ŵ����� ���.
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
//	//	// �� �̺�Ʈ�� �޴´�.
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
//			// ���͸� ó��.
//			if( FILTERTABLE->FilterChat(tempMsg) )
//			{
//				AddMsg(CHATMGR->GetChatMsg(1225), e_ChatMsg_System) ;
//				m_pInputBox->SetEditText("") ;
//				return we ;
//			}
//
//			if( gCurTime - CHATROOMMGR->GetChatListTime() < 1000 )	// 1��
//			{
//				AddMsg(CHATMGR->GetChatMsg(248), e_ChatMsg_System) ;
//				m_pInputBox->SetEditText("") ;
//				return we ;
//			}
//			else
//			{
//				// ������ ä�� �ð� ������Ʈ.
//				CHATROOMMGR->UpdateChatLastTime(gCurTime) ;
//
//				// �޽��� ���� ó��.
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
//	DESC : EventFunc�� ���� �Ѿ�� Event�� ó���ϴ� �Լ�.
//-------------------------------------------------------------------------------------------------
void CChatRoomDlg::OnActionEvent(LONG id, void* p, DWORD event)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(p) ;

	if(!p)
	{
		CHATROOMMGR->Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}



	// ���� ��Ʈ�� üũ.
	ASSERT(m_pBtn_GuestList) ;
	ASSERT(m_pBtn_Option) ;

	if(!m_pBtn_GuestList || !m_pBtn_Option)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// ��Ʈ�� ���̵� üũ.
	switch(id)
	{
	// ������ ��ư.
	case CRD_BTN_GUESTLIST : 
		{
			// ������ ���̾�α� �ޱ�.
			CChatRoomGuestListDlg* pDlg = NULL ;
			pDlg = GAMEIN->GetChatRoomGuestListDlg() ;

			ASSERT(pDlg) ;

			if(!pDlg)
			{
				CHATROOMMGR->Throw_Error("Failed to receive gld!!", __FUNCTION__) ;
				return ;
			}

			// ������ ���̾�α� ����/�ݱ� ó��.
			pDlg->SetActive(!pDlg->IsActive()) ;
		}
		break ;

	// ä�� �ɼ� ��ư.
	case CRD_BTN_OPTION :
		{
			// �������� üũ.
			if(CHATROOMMGR->GetMyRoomOwnerIdx() != HEROID)
			{
				AddMsg(CHATMGR->GetChatMsg(1487), e_ChatMsg_System) ;
				return ;
			}

			// �ɼ� ���̾�α� �ޱ�.
			CChatRoomOptionDlg* pDlg = NULL ;
			pDlg = GAMEIN->GetChatRoomOptionDlg() ;

			ASSERT(pDlg) ;

			if(!pDlg)
			{
				CHATROOMMGR->Throw_Error("Failed to receive od!!", __FUNCTION__) ;
				return ;
			}

			// �ɼ�â ����/�ݱ� ó��.
			pDlg->SetActive(!pDlg->IsActive()) ;
		}
		break ;

	// ä�ù� �ݱ� ��ư.
	case CRD_BTN_CLOSE_DLG :
		{
			// ���� Ȯ�� �˸�â Ȱ��ȭ.
			WINDOWMGR->MsgBox( MBI_CR_ENDCHAT, MBT_YESNO, CHATMGR->GetChatMsg(1480) ) ;
		}
		break ;

	default : break ;
	}
}




////-------------------------------------------------------------------------------------------------
////	NAME : SetActive
////	DESC : ä�ù��� ��Ȱ��ȭ �ϰ� �� ���, ��ȭ���, �ɼ�â�� ���� �ݴ´�.
////  DATE : APRIL 16, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomDlg::SetActive(BOOL val)
//{
//	if(!val)
//	{
//		//// ��ȭ��� ����Ʈ ���̾�α� �ޱ�.
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
//		// �ʱ�ȭ �ϰ� �ݴ´�.
//		//pGuestListDlg->SetDlgToDefault() ;
//		//pGuestListDlg->SetActive(FALSE) ;
//
//		// �ɼ� ���̾�α� �ޱ�.
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
//		//// �ʱ�ȭ �ϰ� �ݴ´�.
//		//pOptionDlg->SetDlgToDefault() ;
//		//pOptionDlg->SetActive(FALSE) ;
//
//		// ä�ù��� �ʱ�ȭ �Ѵ�.
//		SetDlgToDefault() ;
//	}
//
//	cDialog::SetActive(val) ;
//}





////-------------------------------------------------------------------------------------------------
////	NAME : Render
////	DESC : ä�ô��̾�α׸� �����ϴ� �Լ�.
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
//	DESC : ä�ù� ���� �޽����� �߰��ϴ� �Լ�.
//-------------------------------------------------------------------------------------------------
void CChatRoomDlg::AddMsg(char* pMsg, BYTE byType)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg) 
	{
		CHATROOMMGR->Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}



	// �޽��� ���̿� ���� ������ �߶� ä�ù� ����Ʈ�� �߰��Ѵ�.
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
//	DESC : �޽��� Ÿ�԰� �Բ� �޽����� �߰��ϴ� �Լ�.
//-------------------------------------------------------------------------------------------------
void CChatRoomDlg::AddCommonMsg(BYTE byType, char* pMsg)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pMsg) ;

	if(!pMsg)
	{
		CHATROOMMGR->Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}



	// �޽��� Ÿ�� Ȯ��.
	char szStr[128] = {0,} ;

	strcat(szStr, "[") ;
	strcat(szStr, "<") ;
	switch(byType)
	{
	// ä�ù� ����.
	case e_ChatMsg_StepIn :
		{
			strcat(szStr, pMsg) ;
			strcat(szStr, ">") ;
			strcat(szStr, CHATMGR->GetChatMsg(1481)) ;
		}
		break ;

	// ä�ù� ����.
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

	// �޽��� ���.
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
//	DESC : ä�ù� ������ �����ϴ� �Լ�.
//	DATE : APRIL 3, 2008
//-------------------------------------------------------------------------------------------------
void CChatRoomDlg::SetRoomTitle(ST_CR_ROOM_CLT* pInfo)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pInfo) ;

	if(!pInfo)
	{
		CHATROOMMGR->Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}



	// �� ��ȣ ����.
	char tempMsg[128] = {0, } ;
	char tempRoomNum[12] = {0, } ;
	strcat(tempMsg, "[") ;
	sprintf(tempRoomNum, CHATMGR->GetChatMsg(1483), pInfo->byIdx) ;
	strcat(tempMsg, tempRoomNum) ;
	strcat(tempMsg, "]") ;
	strcat(tempMsg, "-") ;



	// ����/����� ����.
	strcat(tempMsg, "[") ;
	switch(pInfo->bySecretMode)
	{
	case e_RM_Open :	strcat(tempMsg, RESRCMGR->GetMsg(892)) ; break ;
	case e_RM_Close :	strcat(tempMsg, RESRCMGR->GetMsg(893)) ; break ;
	default : break ;
	}
	strcat(tempMsg, "]") ;



	// ä�ù� �з� ����.
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



	// ä�ù� ���� ����.
	m_pRoomTitle->SetStaticText(tempMsg) ;



	// ���� ����.
	m_pRoomTitle->SetToolTip(pInfo->title, RGBA_MAKE(255, 255, 255, 255)) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : GetMsgColor
//	DESC : �޽��� Ÿ�Կ� ���� ������ ��ȯ�ϴ� �Լ�.
//-------------------------------------------------------------------------------------------------
DWORD CChatRoomDlg::GetMsgColor(BYTE byColorType)
{
	// ���� ���� ����.
	DWORD dwColor = RGBA_MAKE(255, 255, 255, 255) ;



	// �޽��� Ÿ�� Ȯ��.
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



	// ���� �� ���� ��ȯ.
	return dwColor ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : CloseChatRoom
//	DESC : ä�ù��� �ݴ� ó���� �ϴ� �Լ�.
//  DATE : APRIL 29, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomDlg::CloseChatRoom()
{
	SetActive(FALSE) ;
	SetDlgToDefault() ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : SetDlgToDefault
//	DESC : ä��â�� ó��(�⺻) ���·� �ʱ�ȭ �ϴ� �Լ�.
//  DATE : APRIL 10, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomDlg::SetDlgToDefault()
{
	// ä�ù� ���� �ʱ�ȭ.
	m_pRoomTitle->SetStaticText("") ;

	// ä�� �Է� ���� �ʱ�ȭ.
	m_pInputBox->SetEditText("") ;
	//m_pInputBox->SetScriptText("") ;

	// ä�� ���� �ʱ�ȭ.
	m_pChatList->RemoveAll() ;
}
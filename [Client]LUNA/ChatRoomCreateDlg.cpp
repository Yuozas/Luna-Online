//=================================================================================================
//	FILE		: CChatRoomCreateDlg.cpp
//	DESC		: Implementation part of CChatRoomCreateDlg class.
//	PROGRAMMER	: Yongs Lee
//	DATE		: APRIL 3, 2008
//=================================================================================================




//-------------------------------------------------------------------------------------------------
//		Include header files.
//-------------------------------------------------------------------------------------------------
#include "stdafx.h"

#include "../WindowIDEnum.h"
#include "../cMsgBox.h"

#include "./Interface/cResourceManager.h"
#include "./Interface/cWindowManager.h"

#include "./Interface/cButton.h"
#include "./Interface/cComboBox.h"
#include "./Interface/cEditBox.h"
#include "./Interface/cPushupButton.h"

#include "./ChatManager.h"
#include "./FilteringTable.h"

#include "./ChatRoomMgr.h"

#include "./ChatRoomCreateDlg.h"

#include "cIMEex.h"





//-------------------------------------------------------------------------------------------------
//	NAME : CChatRoomCreateDlg
//	DESC : 생성자 함수.
//	DATE : APRIL 3, 2008
//-------------------------------------------------------------------------------------------------
CChatRoomCreateDlg::CChatRoomCreateDlg(void)
{
	// 초기화 처리.
	m_pOpen				= NULL ;																		// Initialize all member variables.
	m_pClose			= NULL ;

	m_pCodeBox			= NULL ;

	m_pGuestCount		= NULL ;
	m_pRoomType			= NULL ;

	m_pTitleBox			= NULL ;

	m_pOKBtn			= NULL ;
	m_pCancelBtn		= NULL ;

	m_bySecretMode		= e_RM_Open ;

	m_byGeustCount		= e_GC_20 ;
	m_byRoomType		= e_RT_Party ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : ~CChatRoomCreateDlg
//	DESC : 소멸자 함수.
//	DATE : APRIL 3, 2008
//-------------------------------------------------------------------------------------------------
CChatRoomCreateDlg::~CChatRoomCreateDlg(void)
{
}





//-------------------------------------------------------------------------------------------------
//	NAME : Linking
//	DESC : 하위 컨트롤들을 링크하는 함수.
//	DATE : APRIL 3, 2008
//-------------------------------------------------------------------------------------------------
void CChatRoomCreateDlg::Linking()
{
	// 하위 컨트롤들을 링크.
	m_pOpen				= (cPushupButton*)GetWindowForID(CRCD_CHK_ON) ;									// Link child controls.
	m_pClose			= (cPushupButton*)GetWindowForID(CRCD_CHK_OFF) ;

	m_pCodeBox			= (cEditBox*)GetWindowForID(CRCD_EDB_CODE) ;

	m_pGuestCount		= (cComboBox*)GetWindowForID(CRCD_CMB_GUEST) ;
	m_pRoomType			= (cComboBox*)GetWindowForID(CRCD_CMB_ROOMTYPE) ;

	m_pTitleBox			= (cEditBox*)GetWindowForID(CRCD_EDB_TITLE) ;

	m_pOKBtn			= (cButton*)GetWindowForID(CRCD_BTN_OK) ;
	m_pCancelBtn		= (cButton*)GetWindowForID(CRCD_BTN_CANCEL) ;



	// 하위 컨트롤 체크.
	ASSERT(m_pOpen) ;
	ASSERT(m_pClose) ;

	ASSERT(m_pCodeBox) ;

	ASSERT(m_pGuestCount) ;
	ASSERT(m_pRoomType) ;

	ASSERT(m_pTitleBox) ;

	ASSERT(m_pOKBtn) ;
	ASSERT(m_pCancelBtn) ;

	if( !m_pOpen	 || !m_pClose	 || !m_pCodeBox || !m_pGuestCount || 
		!m_pRoomType || !m_pTitleBox || !m_pOKBtn	|| !m_pCancelBtn )
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}

	//m_pCodeBox->SetValidCheck( VCM_NORMAL_NUMBER ) ;													// Setting valid to number.



	// 기본 상태의 창으로 세팅.
	SetDefaultDlg() ;																					// Setting dialog state to default.



	// 채팅방 매니져에 생성창 등록.
	CHATROOMMGR->SetCreateDlg(this) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : SetActive
//	DESC : 채팅방 생성 다이얼로그를 기본 상태로 세팅하는 함수.
//	DATE : APRIL 3, 2008
//-------------------------------------------------------------------------------------------------
void CChatRoomCreateDlg::SetActive(BOOL val)
{
	if(val) SetDefaultDlg() ;																			// Setting dialog state to default.

	cDialog::SetActive(val) ;																			// Active this dialog or not.
}





//-------------------------------------------------------------------------------------------------
//	NAME : SetDefaultDlg
//	DESC : 채팅방 생성 창을 기본상태로 세팅하는 함수.
//	DATE : APRIL 3, 2008
//-------------------------------------------------------------------------------------------------
void CChatRoomCreateDlg::SetDefaultDlg() 
{
	// 하위 컨트롤 체크.
	ASSERT(m_pOpen) ;
	ASSERT(m_pClose) ;
	ASSERT(m_pCodeBox) ;
	ASSERT(m_pGuestCount) ;
	ASSERT(m_pTitleBox) ;

	if( !m_pOpen || !m_pClose || !m_pCodeBox || !m_pGuestCount || !m_pTitleBox )
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// 채팅방 공개모드 세팅.
	m_pOpen->SetPush(TRUE) ;
	m_pClose->SetPush(FALSE) ;

	m_bySecretMode		= e_RM_Open ;

	// 채팅방 제목 세팅.
	m_pCodeBox->SetEditText("") ;
	m_pCodeBox->SetDisable(TRUE) ;
	m_pCodeBox->SetFocusEdit(FALSE) ;



	// 현재 유저수 초기화.
	// 임시 데이터 선언.
	/*int nCount ;
	char tempBuf[16] ;*/

	int nUserCount = 0 ;

	m_pGuestCount->SetMaxLine(e_GC_Max) ;

	/*for(nCount = 0 ; nCount < e_GuestCount_Max ; ++nCount )
	{
		memset(tempBuf, 0, sizeof(tempBuf)) ;

		switch(nCount)
		{
		case e_GC_20 :	SafeStrCpy(tempBuf, RESRCMGR->GetMsg(922), 16) ; break ;
		case e_GC_15 :	SafeStrCpy(tempBuf, RESRCMGR->GetMsg(921), 16) ; break ;
		case e_GC_10 :	SafeStrCpy(tempBuf, RESRCMGR->GetMsg(920), 16) ; break ;
		case e_GC_5 :		SafeStrCpy(tempBuf, RESRCMGR->GetMsg(919), 16) ; break ;
		default : break ;
		}

		ITEM* pItem = new ITEM ;
		memset(pItem, 0, sizeof(ITEM)) ;
		pItem->line = nUserCount ;
		pItem->rgb = RGB(255, 255, 255) ;
		memcpy(pItem->string, tempBuf, sizeof(tempBuf)) ;

		m_pGuestCount->AddItem(pItem) ;

		++nUserCount ;
	}*/
	m_pGuestCount->SetCurSelectedIdx(0) ;
	m_pGuestCount->SelectComboText(0) ;



	// 채팅방 분류 초기화.
	m_pRoomType->SetMaxLine(e_RT_Max) ;															// Setting combobox for roomtype.

	/*for(nCount = 0 ; nCount < e_RT_Max ; ++nCount )
	{
		memset(tempBuf, 0, sizeof(tempBuf)) ;
		sprintf(tempBuf, "%s", RESRCMGR->GetMsg(nCount+880)) ;

		ITEM* pItem = new ITEM ;
		memset(pItem, 0, sizeof(ITEM)) ;
		pItem->line = nCount ;
		pItem->rgb = RGB(255, 255, 255) ;
		memcpy(pItem->string, tempBuf, sizeof(tempBuf)) ;

		m_pRoomType->AddItem(pItem) ;
	}*/

	m_pRoomType->SetCurSelectedIdx(0) ;
	m_pRoomType->SelectComboText(0) ;

	m_byGeustCount		= e_GC_20 ;																// Setting variables for guest count & room type.
	m_byRoomType		= e_RT_Party ;



	// 채팅방 제목 초기화.
	// 리소스 메시지 번호 908
	m_pTitleBox->SetEditText(CHATMGR->GetChatMsg(1479)) ;												// Setting editbox for title of chatroom.
}





//-------------------------------------------------------------------------------------------------
//	NAME : OnActionEvent
//	DESC : EventFunc로 부터 넘어온 Event를 처리하는 함수.
//	DATE : APRIL 3, 2008
//-------------------------------------------------------------------------------------------------
void CChatRoomCreateDlg::OnActionEvent(LONG id, void* p, DWORD event)
{
	// 함수 파라메터 체크.
	ASSERT(p) ;

	if(!p)
	{
		CHATROOMMGR->Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}



	// Check id & Handling message.
	switch(id)
	{
	case CRCD_CHK_ON :			Chk_Open() ;		break ;
	case CRCD_CHK_OFF :			Chk_Close() ;		break ;

	case CRCD_CMB_GUEST :		Cmb_Guest() ;		break ;
	case CRCD_CMB_ROOMTYPE :	Cmb_Roomtype() ;	break ;

	case CRCD_BTN_OK :			Btn_Ok() ;			break ;
	case CRCD_BTN_CANCEL :		Btn_Cancel() ;		break ;

	default : break ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : Chk_Open
//	DESC : 공개모드로 세팅하는 함수.
//	DATE : APRIL 3, 2008
//-------------------------------------------------------------------------------------------------
void CChatRoomCreateDlg::Chk_Open() 
{
	// 하위 컨트롤 체크.
	ASSERT(m_pOpen) ;
	ASSERT(m_pClose) ;

	if(!m_pOpen || !m_pClose)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// 공개 모드로 세팅.
	BOOL bChecked = FALSE ;

	bChecked = m_pOpen->IsPushed() ;																	// Receive current state.
	if(!bChecked) m_pOpen->SetPush(TRUE) ;

	m_bySecretMode = e_RM_Open ;
	m_pClose->SetPush(FALSE) ;																			// Switching secret mode.

	m_pCodeBox->SetEditText("") ;
	m_pCodeBox->SetDisable(TRUE) ;
	m_pCodeBox->SetFocusEdit(FALSE) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Chk_Close
//	DESC : 비공개 모드로 세팅하는 함수.
//	DATE : APRIL 3, 2008
//-------------------------------------------------------------------------------------------------
void CChatRoomCreateDlg::Chk_Close()
{
	// 하위 컨트롤 체크.
	ASSERT(m_pOpen) ;
	ASSERT(m_pClose) ;

	if(!m_pOpen || !m_pClose)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// 비공개 모드로 세팅.
	BOOL bChecked = FALSE ;

	bChecked = m_pClose->IsPushed() ;																	// Receive current state.
	if(!bChecked) m_pClose->SetPush(TRUE) ;

	m_bySecretMode = e_RM_Close ;
	m_pOpen->SetPush(FALSE) ;																			// Switching secret mode.

	m_pCodeBox->SetEditText("") ;
	m_pCodeBox->SetDisable(FALSE) ;
	m_pCodeBox->SetFocusEdit(TRUE) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Cmb_Guest
//	DESC : 최대 참여자 수를 세팅하는 함수.
//	DATE : APRIL 3, 2008
//-------------------------------------------------------------------------------------------------
void CChatRoomCreateDlg::Cmb_Guest()
{
	// 하위 컨트롤 체크.
	ASSERT(m_pGuestCount) ;

	if(!m_pGuestCount)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}

	// 최대 참여자 타입 받기.
	int nIndex = 0 ;
	nIndex = m_pGuestCount->GetCurSelectedIdx() ;

	m_byGeustCount = nIndex ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Cmb_Roomtype
//	DESC : 채팅방 분류를 세팅하는 함수.
//	DATE : APRIL 3, 2008
//-------------------------------------------------------------------------------------------------
void CChatRoomCreateDlg::Cmb_Roomtype()
{
	// 하위 컨트롤 체크.
	ASSERT(m_pRoomType) ;

	if(!m_pRoomType)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// 채팅방 분류 받기.
	//m_byRoomType = m_pRoomType->GetCurSelectedIdx() ;													// Setting room type.

	// 채팅방 분류 세팅.
	int nIndex = 0 ;
	nIndex = m_pRoomType->GetCurSelectedIdx() ;

	switch(nIndex)
	{
	case 0 : m_byRoomType = e_RT_Party ;		break ;
	case 1 : m_byRoomType = e_RT_Deal ;			break ;
	case 2 : m_byRoomType = e_RT_Normal_Chat ;	break ;
	case 3 : m_byRoomType = e_RT_Local_Chat ;	break ;
	case 4 : m_byRoomType = e_RT_Make_Friend ;	break ;
	default : break ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : Btn_Ok
//	DESC : 확인 버튼 이벤트를 처리하는 함수.
//	DATE : APRIL 3, 2008
//-------------------------------------------------------------------------------------------------
void CChatRoomCreateDlg::Btn_Ok()
{
	// 하위 컨트롤 체크.
	ASSERT(m_pCodeBox) ;
	ASSERT(m_pTitleBox) ;

	if(!m_pCodeBox || !m_pTitleBox)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// 비밀번호 받기.
	char codeBuf[SECRET_CODE_SIZE] = {0,} ;
	strcpy(codeBuf, m_pCodeBox->GetEditText()) ;

	// 비밀번호 체크.
	if(m_bySecretMode == e_RM_Close)
	{
		if( strcmp("", codeBuf) == 0 )
		{
			WINDOWMGR->MsgBox(	MBI_CR_SECRETCODE_ERR, 
								MBT_OK, 
								CHATMGR->GetChatMsg(1478) ) ;
			return ;
		}
	}

	// 비밀번호 필터링.
	if( FILTERTABLE->FilterChat(codeBuf) )
	{
		WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1225)) ;
		m_pCodeBox->SetEditText("") ;
		SetActive(FALSE) ;
		return;
	}



	// 제목 받기.
	char titleBuf[TITLE_SIZE] = {0,} ;
	strcpy(titleBuf, m_pTitleBox->GetEditText()) ;

	// 제목 체크.
	if( strcmp("", titleBuf) == 0 )
	{
		if( strcmp("", codeBuf) == 0 )
		{
			WINDOWMGR->MsgBox(	MBI_CR_ROOMTITLE_ERR, 
								MBT_OK, 
								CHATMGR->GetChatMsg(1479) ) ;
			return ;
		}
	}

	// 제목 필터링.
	if( FILTERTABLE->FilterChat(titleBuf) )
	{
		WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1225)) ;
		m_pTitleBox->SetEditText("") ;
		SetActive(FALSE) ;
		return;
	}



	// 재사용 시간 체크.
	if(!CHATROOMMGR->IsEnableExecuteCom())
	{
		WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1517)) ;
		return ;
	}



	// 채팅방 생성 정보 세팅 후 메시지 전송.
	MSG_CR_ELEMENT msg ;
	memset(&msg, 0, sizeof(MSG_CR_ELEMENT)) ;

	msg.Category			= MP_CHATROOM ;
	msg.Protocol			= MP_CHATROOM_CREATE_ROOM_SYN ;

	msg.dwObjectID			= HEROID ;

	msg.bySecretMode		= m_bySecretMode ;
	SafeStrCpy(msg.code, m_pCodeBox->GetEditText(), SECRET_CODE_SIZE) ;

	msg.byTotalGuestCount	= m_byGeustCount ;
	msg.byRoomType			= m_byRoomType ;

	SafeStrCpy(msg.title, titleBuf, 64) ;

	NETWORK->Send( &msg, sizeof(msg) ) ;																// Send message to server.

	SetActive(FALSE) ;

	// 시간 업데이트.
	CHATROOMMGR->UpdateMyLastComTime() ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Btn_Cancel
//	DESC : 취소 버튼 이벤터를 처리하는 함수.
//	DATE : APRIL 3, 2008
//-------------------------------------------------------------------------------------------------
void CChatRoomCreateDlg::Btn_Cancel()
{
	this->SetActive(FALSE) ;																			// Deactive this dialog.
}
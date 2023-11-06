//=================================================================================================
//	FILE		: CChatRoomOptionDlg.cpp
//	DESC		: Implementation part of CChatRoomOptionDlg class.
//	PROGRAMMER	: Yongs Lee
//	DATE		: February 25, 2008
//=================================================================================================





//-------------------------------------------------------------------------------------------------
//		Define caption string.
//-------------------------------------------------------------------------------------------------
// CROD-LK	: CChatRoomOptionDlg::Linking// CROD-SSD	: CChatRoomOptionDlg::SetStoredDlg
// CROD-OAE : CChatRoomOptionDlg::OnActionEvent
// CROD-CO	: CChatRoomOptionDlg::Chk_Open
// CROD-CC	: CChatRoomOptionDlg::Chk_Close
// CROD-CG	: CChatRoomOptionDlg::Cmb_Guest
// CROD-CR  : CChatRoomOptionDlg::Cmb_Roomtype
// CROD-SO	: CChatRoomOptionDlg::SetOption





//-------------------------------------------------------------------------------------------------
//		Include header files.
//-------------------------------------------------------------------------------------------------
#include "stdafx.h"

#include "../WindowIDEnum.h"
#include "../cMsgBox.h"

#include "./Interface/cButton.h"
#include "./Interface/cComboBox.h"
#include "./Interface/cEditBox.h"
#include "./Interface/cPushupButton.h"
#include "./Interface/cResourceManager.h"
#include "./Interface/cWindowManager.h"

#include "./ChatManager.h"
#include "./ChatRoomMgr.h"

#include "../FilteringTable.h"

#include "./ChatRoomOptionDlg.h"

#include "cIMEex.h"





//-------------------------------------------------------------------------------------------------
//	NAME : CChatRoomOptionDlg
//	DESC : 생성자 함수.
//  DATE : APRIL 16, 2008 LYW
//-------------------------------------------------------------------------------------------------
CChatRoomOptionDlg::CChatRoomOptionDlg(void)
{
	// 초기화 처리.
	m_pOpen				= NULL ;
	m_pClose			= NULL ;

	m_pCodeBox			= NULL ;

	m_pGuestCount		= NULL ;
	m_pRoomType			= NULL ;

	m_pTitleBox			= NULL ;

	m_pOKBtn			= NULL ;
	m_pCancelBtn		= NULL ;

	m_bySecretMode		= e_RM_Open ;
	//m_wSecretCode		= 0 ;

	m_byGeustCount		= e_GC_20 ;
	m_byRoomType		= e_RT_Party ;

	//memset(m_TitleBuff, 0, 128) ;


	//m_byStoredMode			= e_RM_Open ;										
	////m_wStoredCode			= 0 ;
	//memset(m_code, 0, sizeof(SECRET_CODE_SIZE+1)) ;

	//m_byStoredGuestCount	= e_GC_20 ;
	//m_byStoredRoomType		= e_RT_Party ;

	//memset(m_StoredTitleBuff, 0, 128) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : ~CChatRoomOptionDlg
//	DESC : 소멸자 함수.
//  DATE : APRIL 16, 2008 LYW
//-------------------------------------------------------------------------------------------------
CChatRoomOptionDlg::~CChatRoomOptionDlg(void)
{
}





//-------------------------------------------------------------------------------------------------
//	NAME : Linking
//		 : 
//	DESC : 옵션창의 하위 컨트롤들을 링크한다.
//		 : 
//		 : 1. 하위 컨트롤 포인터 링크.
//		 : 2. 링크 된 컨트롤 유효성 체크.
//		 : 3. ,를 사용하지 않는 EditBox로 설정.
//		 : 4. 설정된 값으로 옵션 창 세팅.
//		 : 5. 채팅방 매니저에 옵션창 등록.
//		 : 
//  DATE : APRIL 16, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomOptionDlg::Linking()
{
	// 하위 컨트롤 포인터 링크.
	m_pOpen				= (cPushupButton*)GetWindowForID(CROD_CHK_ON) ;			
	m_pClose			= (cPushupButton*)GetWindowForID(CROD_CHK_OFF) ;

	m_pCodeBox			= (cEditBox*)GetWindowForID(CROD_EDB_CODE) ;

	m_pGuestCount		= (cComboBox*)GetWindowForID(CROD_CMB_GUEST) ;
	m_pRoomType			= (cComboBox*)GetWindowForID(CROD_CMB_ROOMTYPE) ;

	m_pTitleBox			= (cEditBox*)GetWindowForID(CROD_EDB_TITLE) ;

	m_pOKBtn			= (cButton*)GetWindowForID(CROD_BTN_OK) ;
	m_pCancelBtn		= (cButton*)GetWindowForID(CROD_BTN_CANCEL) ;



	// 링크 된 컨트롤 유효성 체크.
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

	// ,를 사용하지 않는 EditBox로 설정.
	//m_pCodeBox->SetValidCheck( VCM_NORMAL_NUMBER ) ;							

	// 설정된 값으로 옵션 창 세팅.
	SetStoredDlg() ;



	// 채팅방 매니저에 옵션창 등록.
	CHATROOMMGR->SetOptionDlg(this) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : SetStoredDlg
//		 : 
//	DESC : The function to setting this dialog to default.
//		 : 
//		 : 1. 공개/비공개 세팅.
//		 : 2. 암호 입력 세팅.
//		 : 3. 참여 인원 세팅.
//		 : 4. 채팅방 분류 세팅.
//		 : 5. 채팅방 제목 세팅.
//		 : 
//  DATE : APRIL 10, 2008 LYW
//-------------------------------------------------------------------------------------------------
BOOL CChatRoomOptionDlg::SetStoredDlg() 
{
	// 하위 컨트롤 체크.
	ASSERT(m_pOpen) ;
	ASSERT(m_pClose) ;

	ASSERT(m_pCodeBox) ;

	ASSERT(m_pGuestCount) ;
	ASSERT(m_pRoomType) ;

	ASSERT(m_pTitleBox) ;

	ASSERT(m_pOKBtn) ;
	ASSERT(m_pCancelBtn) ;

	if( !m_pOpen || !m_pClose || !m_pCodeBox || !m_pGuestCount || 
		!m_pRoomType || !m_pTitleBox || !m_pOKBtn	|| !m_pCancelBtn )
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return FALSE ;
	}														



	// 채팅방 모드 세팅.
	//if(m_byStoredMode == e_RM_Open)
	if(CHATROOMMGR->GetOptSavedMode() == e_RM_Open)
	{
		m_pOpen->SetPush(TRUE) ;												
		m_pClose->SetPush(FALSE) ;
	}
	else
	{
		m_pOpen->SetPush(FALSE) ;												
		m_pClose->SetPush(TRUE) ;
	}

	//if(strlen(m_code) == 0)
	if(CHATROOMMGR->GetOptSavedMode() == e_RM_Close)
	{
		if(strlen(CHATROOMMGR->GetOptSecretCode()) == 0)
		{
			m_pCodeBox->SetEditText("") ;
		}
		else
		{
			//m_pCodeBox->SetEditText(m_code) ;
			m_pCodeBox->SetEditText(CHATROOMMGR->GetOptSecretCode()) ;
		}

		m_pCodeBox->SetDisable(FALSE) ;
		m_pCodeBox->SetFocusEdit(FALSE) ;
	}
	else
	{
		m_pCodeBox->SetEditText("") ;
		m_pCodeBox->SetDisable(TRUE) ;
		m_pCodeBox->SetFocusEdit(FALSE) ;
	}

	//// 비밀번호 세팅.
	//if(m_wStoredCode == 0)
	//{
	//	m_pCodeBox->SetEditText("") ;
	//}
	//else
	//{
	//	char tempCode[12] = {0, } ;

	//	/*BYTE byNum[4] = {0,} ;

	//	byNum[0] = m_wStoredCode / 1000 ;
	//	byNum[1] = (m_wStoredCode - (1000*byNum[0])) / 100 ;
	//	byNum[2] = (m_wStoredCode - (1000*byNum[0]) - (100*byNum[1])) / 10 ;
	//	byNum[3] = (m_wStoredCode - (1000*byNum[0]) - (100*byNum[1]) - (10*byNum[2])) / 1 ;

	//	for(int count = 0 ; count < 4 ; ++count )
	//	{
	//		if( byNum[count] <= 0 )
	//		{
	//			strcat(tempCode, "0") ;
	//		}
	//		else
	//		{
	//			char num[10] = {0,} ;

	//			sprintf(num, "%d", byNum[count]) ;
	//			strcat(tempCode, num) ;
	//		}
	//	}*/

	//	//m_pCodeBox->SetEditText(tempCode) ;		

	//	m_pCodeBox->SetEditText(itoa(m_wStoredCode, tempCode, 10)) ;		
	//}

	int nUserCount = 0 ; 



	// 참여인원 세팅.
	m_pGuestCount->SetMaxLine(e_GC_Max) ;
	/*for(nCount = 0 ; nCount < e_GuestCount_Max ; ++nCount )
	{
		memset(tempBuf, 0, sizeof(tempBuf)) ;

		switch(nCount)
		{
		case e_GC_20 :	SafeStrCpy(tempBuf, "20-15명", 16) ; break ;
		case e_GC_15 :	SafeStrCpy(tempBuf, "15-10명", 16) ; break ;
		case e_GC_10 :	SafeStrCpy(tempBuf, "10-5명", 16) ; break ;
		case e_GC_5 :		SafeStrCpy(tempBuf, "5-2명", 16) ; break ;
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

	//m_pGuestCount->SetCurSelectedIdx(m_byStoredGuestCount) ;
	//m_pGuestCount->SelectComboText(m_byStoredGuestCount) ;
	m_pGuestCount->SetCurSelectedIdx(CHATROOMMGR->GetOptTotalGuest()) ;
	m_pGuestCount->SelectComboText(CHATROOMMGR->GetOptTotalGuest()) ;



	// 채팅방 분류 세팅.
	m_pRoomType->SetMaxLine(e_RTM_Max) ;									

	/*for(nCount = 0 ; nCount < e_RTM_Max ; ++nCount )
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

	//m_pRoomType->SetCurSelectedIdx(m_byStoredRoomType) ;
	//m_pRoomType->SelectComboText(m_byStoredRoomType) ;
	m_pRoomType->SetCurSelectedIdx(CHATROOMMGR->GetOptRoomType()) ;
	m_pRoomType->SelectComboText(CHATROOMMGR->GetOptRoomType()) ;



	// 채팅방 제목 세팅.
	//m_pTitleBox->SetEditText(m_StoredTitleBuff) ;	
	m_pTitleBox->SetEditText(CHATROOMMGR->GetOptRoomTitle()) ;	

	return TRUE ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : SetDlgToDefault
//		 : 
//	DESC : The function to setting this dialog to default.
//		 : 
//		 : 1. 공개/비공개 초기화.
//		 : 2. 암호 입력 초기화.
//		 : 3. 참여 인원 초기화.
//		 : 4. 채팅방 분류 초기화.
//		 : 5. 채팅방 제목 초기화.
//		 : 
//  DATE : APRIL 10, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomOptionDlg::SetDlgToDefault()
{
	// 하위 컨트롤 체크.
	ASSERT(m_pOpen) ;
	ASSERT(m_pClose) ;

	ASSERT(m_pCodeBox) ;

	ASSERT(m_pGuestCount) ;
	ASSERT(m_pRoomType) ;

	ASSERT(m_pTitleBox) ;

	ASSERT(m_pOKBtn) ;
	ASSERT(m_pCancelBtn) ;

	if( !m_pOpen || !m_pClose || !m_pCodeBox || !m_pGuestCount || 
		!m_pRoomType || !m_pTitleBox || !m_pOKBtn	|| !m_pCancelBtn )
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// 공개/비공개 초기화.
	m_pOpen->SetPush(TRUE) ;												
	m_pClose->SetPush(FALSE) ;

	m_bySecretMode		= e_RM_Open ;
	//m_wSecretCode		= 0 ;



	// 암호 입력 초기화.
	m_pCodeBox->SetEditText("") ;
	m_pCodeBox->SetDisable(TRUE) ;
	m_pCodeBox->SetFocusEdit(FALSE) ;



	// 참여 인원 초기화.
	m_pGuestCount->SetCurSelectedIdx(0) ;
	m_pGuestCount->SelectComboText(0) ;



	// 채팅방 분류 초기화.
	m_pRoomType->SetCurSelectedIdx(0) ;
	m_pRoomType->SelectComboText(0) ;

	m_byGeustCount		= e_GC_20 ;
	m_byRoomType		= e_RT_Party ;



	// 채팅방 제목 초기화.
	m_pTitleBox->SetEditText("") ;	
}





//-------------------------------------------------------------------------------------------------
//	NAME : OnActionEvent
//	DESC : 하위컨트롤에 이벤트가 발생했을 때 처리하는 함수.
//  DATE : APRIL 10, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomOptionDlg::OnActionEvent(LONG id, void* p, DWORD event)
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
	case CROD_CHK_ON :			Chk_Open() ;		break ;
	case CROD_CHK_OFF :			Chk_Close() ;		break ;

	case CROD_CMB_GUEST :		Cmb_Guest() ;		break ;
	case CROD_CMB_ROOMTYPE :	Cmb_Roomtype() ;	break ;

	case CROD_BTN_OK :			Btn_Ok() ;			break ;
	case CROD_BTN_CANCEL :		Btn_Cancel() ;		break ;

	default : break ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : Chk_Open
//	DESC : 채팅방을 공개모드로 세팅하는 함수.
//  DATE : APRIL 10, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomOptionDlg::Chk_Open() 
{
	// 하위 컨트롤 체크.
	ASSERT(m_pOpen) ;
	ASSERT(m_pClose) ;

	if(!m_pOpen || !m_pClose)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// 공개모드로 세팅.
	BOOL bChecked = FALSE ;

	bChecked = m_pOpen->IsPushed() ;
	if(!bChecked) m_pOpen->SetPush(TRUE) ;

	m_bySecretMode = e_RM_Open ;
	m_pClose->SetPush(FALSE) ;

	m_pCodeBox->SetEditText("") ;
	m_pCodeBox->SetDisable(TRUE) ;
	m_pCodeBox->SetFocusEdit(FALSE) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Chk_Close
//	DESC : 채팅방을 비공개모드로 세팅하는 함수.
//  DATE : APRIL 10, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomOptionDlg::Chk_Close()
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

	bChecked = m_pClose->IsPushed() ;											
	if(!bChecked) m_pClose->SetPush(TRUE) ;

	m_bySecretMode = e_RM_Close ;
	m_pOpen->SetPush(FALSE) ;

	m_pCodeBox->SetEditText("") ;
	m_pCodeBox->SetDisable(FALSE) ;
	m_pCodeBox->SetFocusEdit(TRUE) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Cmb_Guest
//	DESC : 채팅방이 수용할 수 있는 수용인원을 세팅하는 함수.
//  DATE : APRIL 10, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomOptionDlg::Cmb_Guest()
{
	// 하위 컨트롤 체크.
	ASSERT(m_pGuestCount) ;

	if(!m_pGuestCount)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// 허용인원을 세팅.
	int nIndex = 0 ;
	nIndex = m_pGuestCount->GetCurSelectedIdx() ;

	m_byGeustCount = nIndex ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Cmb_Roomtype
//	DESC : 채팅방 분류를 세팅하는 함수.
//  DATE : APRIL 10, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomOptionDlg::Cmb_Roomtype()
{
	// 하위 컨트롤 체크.
	ASSERT(m_pRoomType) ;

	if(!m_pRoomType)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	//m_byRoomType = m_pRoomType->GetCurSelectedIdx() ;							
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
//	DESC : 변경한 옵션적용을 서버로 요청하는 함수.
//  DATE : APRIL 10, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomOptionDlg::Btn_Ok()
{
	// 하위 컨트롤 체크.
	ASSERT(m_pCodeBox) ;
	ASSERT(m_pTitleBox) ;

	if(!m_pCodeBox || !m_pTitleBox)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// 비밀번호 체크.
	char codeBuf[SECRET_CODE_SIZE] = {0,} ;
	strcpy(codeBuf, m_pCodeBox->GetEditText()) ;								

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

	// 비밀번호 필터링 체크.
	if( FILTERTABLE->FilterChat(codeBuf) )
	{
		WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1225)) ;
		m_pCodeBox->SetEditText("") ;
		SetActive(FALSE) ;
		return;
	}



	// 채팅방 제목 체크.
	char titleBuf[TITLE_SIZE] = {0,} ;
	strcpy(titleBuf, m_pTitleBox->GetEditText()) ;

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

	// 제목 필터링 체크.
	if( FILTERTABLE->FilterChat(titleBuf) )
	{
		WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1225)) ;
		m_pTitleBox->SetEditText("") ;
		SetActive(FALSE) ;
		return;
	}

	// 방장인지 체크.
	if(HEROID != CHATROOMMGR->GetMyRoomOwnerIdx())
	{
		CHATROOMMGR->AddChatRoomMsg(CHATMGR->GetChatMsg(1487), e_ChatMsg_System) ;
		return ;
	}

	// 재사용 시간 체크.
	if(!CHATROOMMGR->IsEnableExecuteCom())
	{
		WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1517)) ;
		return ;
	}



	// 옵션 변경 요청.
	MSG_CR_ELEMENT msg ;
	memset(&msg, 0, sizeof(MSG_CR_ELEMENT)) ;

	msg.Category			= MP_CHATROOM ;
	msg.Protocol			= MP_CHATROOM_CHANGE_OPTION_SYN ;

	msg.dwObjectID			= HEROID ;

	msg.bySecretMode		= m_bySecretMode ;
	SafeStrCpy(msg.code, m_pCodeBox->GetEditText(), SECRET_CODE_SIZE) ;			

	msg.byTotalGuestCount	= m_byGeustCount ;
	msg.byRoomType			= m_byRoomType ;

	SafeStrCpy(msg.title, titleBuf, 64) ;

	NETWORK->Send( &msg, sizeof(msg) ) ;										

	SetActive(FALSE) ;

	// 시간 업데이트.
	CHATROOMMGR->UpdateMyLastComTime() ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Btn_Cancel
//	DESC : 변경 중이던 옵션을 취소하고, 옵션창을 닫는 함수.
//  DATE : APRIL 10, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomOptionDlg::Btn_Cancel()
{
	SetStoredDlg() ;

	this->SetActive(FALSE) ;
}





////-------------------------------------------------------------------------------------------------
////	NAME : SetOption
////	DESC : 채팅방 옵션을 적용하는 함수.
////  DATE : APRIL 12, 2008 LYW
////-------------------------------------------------------------------------------------------------
//BOOL CChatRoomOptionDlg::SetOption(ST_CR_ROOM_SRV* pInfo)
//{
//	// 함수 파라메터 체크.
//	ASSERT(pInfo) ;
//
//	if(!pInfo)
//	{
//		CHATROOMMGR->Throw_Error(err_IMP, __FUNCTION__) ;
//		return FALSE ;
//	}
//
//	// 옵션 적용.
//	m_byStoredMode			= pInfo->bySecretMode ;
//	//m_wStoredCode			= pInfo->wCode ;
//
//	SafeStrCpy(m_code, pInfo->code, SECRET_CODE_SIZE) ;
//
//	m_byStoredGuestCount	= pInfo->byTotalGuestCount ;
//	m_byStoredRoomType		= pInfo->byRoomType-1 ;
//
//	SafeStrCpy(m_StoredTitleBuff, pInfo->title, 128) ;
//
//	// 적용된 옵션 세팅.
//	if(!SetStoredDlg()) return FALSE ;
//
//	return TRUE ;
//}





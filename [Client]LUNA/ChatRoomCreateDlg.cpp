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
//	DESC : ������ �Լ�.
//	DATE : APRIL 3, 2008
//-------------------------------------------------------------------------------------------------
CChatRoomCreateDlg::CChatRoomCreateDlg(void)
{
	// �ʱ�ȭ ó��.
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
//	DESC : �Ҹ��� �Լ�.
//	DATE : APRIL 3, 2008
//-------------------------------------------------------------------------------------------------
CChatRoomCreateDlg::~CChatRoomCreateDlg(void)
{
}





//-------------------------------------------------------------------------------------------------
//	NAME : Linking
//	DESC : ���� ��Ʈ�ѵ��� ��ũ�ϴ� �Լ�.
//	DATE : APRIL 3, 2008
//-------------------------------------------------------------------------------------------------
void CChatRoomCreateDlg::Linking()
{
	// ���� ��Ʈ�ѵ��� ��ũ.
	m_pOpen				= (cPushupButton*)GetWindowForID(CRCD_CHK_ON) ;									// Link child controls.
	m_pClose			= (cPushupButton*)GetWindowForID(CRCD_CHK_OFF) ;

	m_pCodeBox			= (cEditBox*)GetWindowForID(CRCD_EDB_CODE) ;

	m_pGuestCount		= (cComboBox*)GetWindowForID(CRCD_CMB_GUEST) ;
	m_pRoomType			= (cComboBox*)GetWindowForID(CRCD_CMB_ROOMTYPE) ;

	m_pTitleBox			= (cEditBox*)GetWindowForID(CRCD_EDB_TITLE) ;

	m_pOKBtn			= (cButton*)GetWindowForID(CRCD_BTN_OK) ;
	m_pCancelBtn		= (cButton*)GetWindowForID(CRCD_BTN_CANCEL) ;



	// ���� ��Ʈ�� üũ.
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



	// �⺻ ������ â���� ����.
	SetDefaultDlg() ;																					// Setting dialog state to default.



	// ä�ù� �Ŵ����� ����â ���.
	CHATROOMMGR->SetCreateDlg(this) ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : SetActive
//	DESC : ä�ù� ���� ���̾�α׸� �⺻ ���·� �����ϴ� �Լ�.
//	DATE : APRIL 3, 2008
//-------------------------------------------------------------------------------------------------
void CChatRoomCreateDlg::SetActive(BOOL val)
{
	if(val) SetDefaultDlg() ;																			// Setting dialog state to default.

	cDialog::SetActive(val) ;																			// Active this dialog or not.
}





//-------------------------------------------------------------------------------------------------
//	NAME : SetDefaultDlg
//	DESC : ä�ù� ���� â�� �⺻���·� �����ϴ� �Լ�.
//	DATE : APRIL 3, 2008
//-------------------------------------------------------------------------------------------------
void CChatRoomCreateDlg::SetDefaultDlg() 
{
	// ���� ��Ʈ�� üũ.
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



	// ä�ù� ������� ����.
	m_pOpen->SetPush(TRUE) ;
	m_pClose->SetPush(FALSE) ;

	m_bySecretMode		= e_RM_Open ;

	// ä�ù� ���� ����.
	m_pCodeBox->SetEditText("") ;
	m_pCodeBox->SetDisable(TRUE) ;
	m_pCodeBox->SetFocusEdit(FALSE) ;



	// ���� ������ �ʱ�ȭ.
	// �ӽ� ������ ����.
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



	// ä�ù� �з� �ʱ�ȭ.
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



	// ä�ù� ���� �ʱ�ȭ.
	// ���ҽ� �޽��� ��ȣ 908
	m_pTitleBox->SetEditText(CHATMGR->GetChatMsg(1479)) ;												// Setting editbox for title of chatroom.
}





//-------------------------------------------------------------------------------------------------
//	NAME : OnActionEvent
//	DESC : EventFunc�� ���� �Ѿ�� Event�� ó���ϴ� �Լ�.
//	DATE : APRIL 3, 2008
//-------------------------------------------------------------------------------------------------
void CChatRoomCreateDlg::OnActionEvent(LONG id, void* p, DWORD event)
{
	// �Լ� �Ķ���� üũ.
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
//	DESC : �������� �����ϴ� �Լ�.
//	DATE : APRIL 3, 2008
//-------------------------------------------------------------------------------------------------
void CChatRoomCreateDlg::Chk_Open() 
{
	// ���� ��Ʈ�� üũ.
	ASSERT(m_pOpen) ;
	ASSERT(m_pClose) ;

	if(!m_pOpen || !m_pClose)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// ���� ���� ����.
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
//	DESC : ����� ���� �����ϴ� �Լ�.
//	DATE : APRIL 3, 2008
//-------------------------------------------------------------------------------------------------
void CChatRoomCreateDlg::Chk_Close()
{
	// ���� ��Ʈ�� üũ.
	ASSERT(m_pOpen) ;
	ASSERT(m_pClose) ;

	if(!m_pOpen || !m_pClose)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// ����� ���� ����.
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
//	DESC : �ִ� ������ ���� �����ϴ� �Լ�.
//	DATE : APRIL 3, 2008
//-------------------------------------------------------------------------------------------------
void CChatRoomCreateDlg::Cmb_Guest()
{
	// ���� ��Ʈ�� üũ.
	ASSERT(m_pGuestCount) ;

	if(!m_pGuestCount)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}

	// �ִ� ������ Ÿ�� �ޱ�.
	int nIndex = 0 ;
	nIndex = m_pGuestCount->GetCurSelectedIdx() ;

	m_byGeustCount = nIndex ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Cmb_Roomtype
//	DESC : ä�ù� �з��� �����ϴ� �Լ�.
//	DATE : APRIL 3, 2008
//-------------------------------------------------------------------------------------------------
void CChatRoomCreateDlg::Cmb_Roomtype()
{
	// ���� ��Ʈ�� üũ.
	ASSERT(m_pRoomType) ;

	if(!m_pRoomType)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// ä�ù� �з� �ޱ�.
	//m_byRoomType = m_pRoomType->GetCurSelectedIdx() ;													// Setting room type.

	// ä�ù� �з� ����.
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
//	DESC : Ȯ�� ��ư �̺�Ʈ�� ó���ϴ� �Լ�.
//	DATE : APRIL 3, 2008
//-------------------------------------------------------------------------------------------------
void CChatRoomCreateDlg::Btn_Ok()
{
	// ���� ��Ʈ�� üũ.
	ASSERT(m_pCodeBox) ;
	ASSERT(m_pTitleBox) ;

	if(!m_pCodeBox || !m_pTitleBox)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// ��й�ȣ �ޱ�.
	char codeBuf[SECRET_CODE_SIZE] = {0,} ;
	strcpy(codeBuf, m_pCodeBox->GetEditText()) ;

	// ��й�ȣ üũ.
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

	// ��й�ȣ ���͸�.
	if( FILTERTABLE->FilterChat(codeBuf) )
	{
		WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1225)) ;
		m_pCodeBox->SetEditText("") ;
		SetActive(FALSE) ;
		return;
	}



	// ���� �ޱ�.
	char titleBuf[TITLE_SIZE] = {0,} ;
	strcpy(titleBuf, m_pTitleBox->GetEditText()) ;

	// ���� üũ.
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

	// ���� ���͸�.
	if( FILTERTABLE->FilterChat(titleBuf) )
	{
		WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1225)) ;
		m_pTitleBox->SetEditText("") ;
		SetActive(FALSE) ;
		return;
	}



	// ���� �ð� üũ.
	if(!CHATROOMMGR->IsEnableExecuteCom())
	{
		WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1517)) ;
		return ;
	}



	// ä�ù� ���� ���� ���� �� �޽��� ����.
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

	// �ð� ������Ʈ.
	CHATROOMMGR->UpdateMyLastComTime() ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Btn_Cancel
//	DESC : ��� ��ư �̺��͸� ó���ϴ� �Լ�.
//	DATE : APRIL 3, 2008
//-------------------------------------------------------------------------------------------------
void CChatRoomCreateDlg::Btn_Cancel()
{
	this->SetActive(FALSE) ;																			// Deactive this dialog.
}
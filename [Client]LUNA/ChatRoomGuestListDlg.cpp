//-------------------------------------------------------------------------------------------------
//	FILE		: CChatRoomDlg.cpp
//	DESC		: Implementation part of CChatRoomGuestListDlg class.
//	PROGRAMMER	: Yongs Lee
//	DATE		: February 25, 2008
//-------------------------------------------------------------------------------------------------




//-------------------------------------------------------------------------------------------------
//		Include header files.
//-------------------------------------------------------------------------------------------------
#include "stdafx.h"

#include "../WindowIDEnum.h"
#include "../cMsgBox.h"

#include "./ChatRoomGuestListDlg.h"

#include "./Input/Mouse.h"

#include "../FilteringTable.h"

#include "./Interface/cResourceManager.h"
#include "./Interface/cScriptManager.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cButton.h"
//#include "./Interface/cListCtrl.h"
#include "./Interface/cListDialog.h"

//#include "./cRItemEx.h"

#include "./ChatRoomMgr.h"
#include "./ChatManager.h"





//-------------------------------------------------------------------------------------------------
//	NAME : CChatRoomGuestListDlg
//	DESC : ������ �Լ�.
//  DATE : APRIL 12, 2008 LYW
//-------------------------------------------------------------------------------------------------
CChatRoomGuestListDlg::CChatRoomGuestListDlg(void)
{
	m_pGuestList		= NULL ;

	m_pBtn_ChangeOwner	= NULL ;
	m_pBtn_FreezeGuest	= NULL ;
	m_pBtn_KickGuest	= NULL ;
	m_pBtn_AddFriend	= NULL ;

	//// Ŭ���� ��ũ �ʱ�ȭ.
	//for(BYTE count = 0 ; count < MAX_ROW_GUESTLIST ; ++count)
	//{
	//	ZeroMemory(&m_MarkImg[count], sizeof(cImage)) ;
	//}

	// ���� �� ���� ���� �ʱ�ȭ.
	memset(&m_SelectedUser, 0, sizeof(ST_CR_USER)) ;

	// �� ���� �ʱ�ȭ.
	//memset(&m_MyUserInfo, 0, sizeof(ST_CR_USER)) ;

	m_plGuest = NULL ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : ~CChatRoomGuestListDlg
//	DESC : �Ҹ��� �Լ�.
//  DATE : APRIL 12, 2008 LYW
//-------------------------------------------------------------------------------------------------
CChatRoomGuestListDlg::~CChatRoomGuestListDlg(void)
{
	//ST_CR_USER* pInfo ;
	//PTRLISTPOS pos = NULL ;
	//pos = m_plGuest->GetHeadPosition() ;

	//BYTE byCount = 0 ;
	//while(pos)
	//{
	//	pInfo = NULL ;
	//	pInfo = (ST_CR_USER*)m_plGuest->GetNext(pos) ;

	//	if(!pInfo) continue ;

	//	m_plGuest->Remove(pInfo) ;

	//	delete pInfo ;
	//}

	//m_plGuest->RemoveAll() ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : Linking
//	DESC : ���� ��Ʈ�ѵ��� ��ũ�ϴ� �Լ�.
//  DATE : APRIL 12, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomGuestListDlg::Linking()
{
	// ���� ��Ʈ�� ��ũ.
	m_pGuestList		= (cListDialog*)GetWindowForID(CRGLD_LST_GUESTLIST) ;

	m_pBtn_ChangeOwner	= (cButton*)GetWindowForID(CRGLD_BTN_CHANGEOWNER) ;
	m_pBtn_FreezeGuest	= (cButton*)GetWindowForID(CRGLD_BTN_FREEZEGUEST) ;
	m_pBtn_KickGuest	= (cButton*)GetWindowForID(CRGLD_BTN_KICKGUEST) ;
	m_pBtn_AddFriend	= (cButton*)GetWindowForID(CRGLD_BTN_ADDFRIEND) ;



	// ��ũ �� ��Ʈ�� ��ȿ�� üũ.
	ASSERT(m_pGuestList) ;

	ASSERT(m_pBtn_ChangeOwner) ;
	ASSERT(m_pBtn_FreezeGuest) ;
	ASSERT(m_pBtn_KickGuest) ;
	ASSERT(m_pBtn_AddFriend) ;

	if(!m_pGuestList || !m_pBtn_ChangeOwner || !m_pBtn_FreezeGuest || !m_pBtn_KickGuest || !m_pBtn_AddFriend)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// ä�ù� �Ŵ����� ���.
	CHATROOMMGR->SetGuestListDlg(this) ;



	// ä�ù� �Ŵ����� �����ϰ� �ִ� ������ ����Ʈ�� �޴´�.
	m_plGuest = CHATROOMMGR->GetGuestList() ;
}





////-------------------------------------------------------------------------------------------------
////	NAME : Render
////	DESC : ���� ����Ʈ ��Ʈ�ѿ� Ŭ���� ��ũ�� ����Ѵ�.
////  DATE : APRIL 12, 2008 LYW
////-------------------------------------------------------------------------------------------------
//void CChatRoomGuestListDlg::Render()
//{
//	// Ȱ��ȭ ���� üũ.
//	if( !IsActive() ) return ;
//
//	// ���� ��Ʈ�� üũ.
//	ASSERT(m_pGuestList) ;
//
//	if(!m_pGuestList)
//	{
//		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
//		SetActive(FALSE) ;
//		return ;
//	}
//
//	// ���̾�α�/���� ��Ʈ�� ����.
//	cDialog::RenderWindow() ;
//	cDialog::RenderComponent() ;
//
//	// �̹��� ��Ʈ ����.
//	cImageRect* pRect ;
//
//	// ������ ����.
//	VECTOR2 vScale ;
//	memset(&vScale, 0, sizeof(VECTOR2)) ;
//
//	vScale.x = 1.0f ;
//	vScale.y = 1.0f ;
//
//	// ��ġ ����.
//	VECTOR2 vPos ;
//	memset(&vPos, 0, sizeof(VECTOR2)) ;
//
//	vPos.x = m_pGuestList->GetAbsX() ;
//	vPos.y = m_pGuestList->GetAbsY() ;//+ m_pGuestList->GetHeadLineHeight() + 4 ;
//
//	// ��� ���� ���� �ޱ�.
//	int nRows = 0 ;
//	//nRows = m_pGuestList->GetRowsNum() ;
//	nRows = m_pGuestList->GetMinMiddleNum() ;
//
//	// ��� ������ ���� �� ��ŭ for��.
//	for(int count = 0 ; count < nRows ; ++count)
//	{
//		// �̹��� ��ȿ�� üũ.
//		if(m_MarkImg[count].IsNull()) continue ;
//
//		// �̹��� ����.
//		m_MarkImg[count].RenderSprite(&vScale, 0, 0, &vPos, 0xffffffff) ;
//
//		// �̹��� ��Ʈ ����.
//		pRect = NULL ;
//		pRect = m_MarkImg[count].GetImageRect() ;
//
//		ASSERT(pRect) ;
//
//		if(!pRect)
//		{
//			CHATROOMMGR->Throw_Error("Invalid rect!!", __FUNCTION__) ;
//			SetActive(FALSE) ;
//			return ;
//		}
//
//		vPos.y += ((pRect->bottom-pRect->top)+6) ;
//	}
//}





//-------------------------------------------------------------------------------------------------
//	NAME : ActionEvent
//	DESC : ���콺 �̺�Ʈ�� ó���ϴ� �Լ�.
//  DATE : APRIL 12, 2008 LYW
//-------------------------------------------------------------------------------------------------
DWORD CChatRoomGuestListDlg::ActionEvent( CMouse* mouseInfo ) 
{
	// �̺�Ʈ ���� ����.
	DWORD we = WE_NULL ;



	// ���� ��Ʈ�� üũ.
	ASSERT(m_pGuestList) ;

	if(!m_pGuestList)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		SetActive(FALSE) ;
		return we ;
	}



	// ���콺 ���� üũ.
	if( !mouseInfo ) return we ;



	// ���̾�α� �̺�Ʈ�� �޴´�.
	we = cDialog::ActionEvent( mouseInfo ) ;

	// ���� ����Ʈ �� ���콺�� ������,
	if(m_pGuestList->PtInWindow( (LONG)mouseInfo->GetMouseX(), (LONG)mouseInfo->GetMouseY() ))
	{
		// ������ ����Ʈ üũ.
		ASSERT(m_plGuest) ;

		if(!m_plGuest)
		{
			CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
			return we ;
		}

		//// �� �̺�Ʈ�� �޴´�.
		//int Wheel = mouseInfo->GetWheel();

		//// �� �̺�Ʈ�� �߻��ߴٸ�,
		//if(Wheel)
		//{
		//	// ��°����� ���� ���� �޴´�.
		//	int nRows = 0 ;
		//	//nRows = m_pGuestList->GetRowsNum() ;
		//	nRows = m_pGuestList->GetMinMiddleNum() ;
		//
		//	// ��������Ʈ ����� �޴´�.
		//	int nMaxRow = 0 ;
		//	nMaxRow = m_plGuest->GetCount() ;

		//	// �ֻ�� �ε����� �޴´�.
		//	WORD lTopIdx = 0 ;
		//	//lTopIdx = m_pGuestList->GetStartIdxOfPrintedItems() ;
		//	lTopIdx = m_pGuestList->GetTopListItemIdx() ;

		//	int nCheckRow = 0 ;
		//	nCheckRow = lTopIdx+nRows-1 ;

		//	// �ε��� ��ȿ�� üũ.
		//	ASSERT(lTopIdx >= 0) ;
		//
		//	if(lTopIdx < 0)
		//	{
		//		CHATROOMMGR->Throw_Error("Invalid Index!!", __FUNCTION__) ;
		//		SetActive(FALSE) ;
		//		return we ;
		//	}

		//	// ���� ���� ��������,
		//	if( Wheel > 0 )
		//	{
		//		// ����Ʈ ��Ʈ�ѿ� ������ �ٽ� �����ϰ� ���.
		//		if(lTopIdx > 0)
		//		{
		//			//m_pGuestList->SetStartIdxOfPrintedItems(lTopIdx-1) ;
		//			m_pGuestList->SetCurSelectedRowIdx(lTopIdx-1) ;
		//			SetClassMark() ;
		//		}
		//		else return we ;
		//	}
		//	// ���� �Ʒ��� ��������,
		//	else
		//	{
		//		// �ε��� ��ȿ üũ �� ������ �ٽ� �����ϰ� ���.
		//		if( nCheckRow < nMaxRow )
		//		{
		//			if(lTopIdx < nMaxRow-1)
		//			{
		//				//m_pGuestList->SetStartIdxOfPrintedItems(lTopIdx+1) ;
		//				m_pGuestList->SetCurSelectedRowIdx(lTopIdx+1) ;
		//			}
		//			else
		//			{
		//				//m_pGuestList->SetStartIdxOfPrintedItems(nMaxRow-1) ;
		//				m_pGuestList->SetCurSelectedRowIdx(lTopIdx-1) ;
		//			}

		//			SetClassMark() ;
		//		}
		//		else return we ;
		//	}

		//}

		// Ŭ�� �̺�Ʈ�� �߻����� ���.
		if( we & WE_LBTNCLICK )
		{
			//cRITEMEx* pItem ;
			ITEM* pItem ;

			// ������ �� �ޱ�.
			BYTE byCount = 0 ;
			byCount = (BYTE)m_pGuestList->GetItemCount() ;

			// ���� ���� �ޱ�.
			BYTE bySelectRow = 0 ;
			//bySelectRow = m_pGuestList->GetSelectedRowIdx() ;
			bySelectRow = m_pGuestList->GetSelectRowIdx() + m_pGuestList->GetTopListItemIdx() ;

			// �⺻ ����.
			DWORD dwColor = 0 ;
			dwColor = RGBA_MAKE(255, 255, 255, 255) ;

			for(BYTE count = 0 ; count < byCount ; ++count)
			{
				pItem = NULL ;
				//pItem = (cRITEMEx*)m_pGuestList->GetRItem(count) ;
				pItem = (ITEM*)m_pGuestList->GetItem((LONG)count) ;

				if(!pItem) continue ;

				// ������ �������̸�, 
				if(count == bySelectRow)
				{
					// ������ �������� ������Ʈ.
					dwColor = RGBA_MAKE(255, 0, 0, 255) ;
				}
				// �⺻ �������� ����.
				else
				{
					dwColor = RGBA_MAKE(255, 255, 255, 255) ;
				}
				
				//pItem->rgb[0] = dwColor ;
				//pItem->rgb[1] = dwColor ;
				//pItem->rgb[2] = dwColor ;
				//pItem->rgb[3] = dwColor ;
				pItem->rgb = dwColor ;
			}

			ST_CR_USER* pInfo ;
			PTRLISTPOS pos = NULL ;
			pos = m_plGuest->GetHeadPosition() ;

			BYTE byCheckCount = 0 ;
			while(pos)
			{
				pInfo = NULL ;
				pInfo = (ST_CR_USER*)m_plGuest->GetNext(pos) ;

				if(!pInfo) continue ;

				if(byCheckCount == bySelectRow)
				{
					memcpy(&m_SelectedUser, pInfo, sizeof(ST_CR_USER)) ;
					break ;
				}
				++byCheckCount ;
			}
		}
	}

	return we ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : OnActionEvent
//	DESC : EventFunc�� ���� �Ѿ�� Event�� ó���ϴ� �Լ�.
//  DATE : APRIL 12, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomGuestListDlg::OnActionEvent(LONG id, void* p, DWORD event)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(p) ;	

	if(!p)
	{
		CHATROOMMGR->Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}



	// ���� ��Ʈ�� üũ.
	ASSERT(m_pBtn_ChangeOwner) ;
	ASSERT(m_pBtn_FreezeGuest) ;
	ASSERT(m_pBtn_KickGuest) ;
	ASSERT(m_pBtn_AddFriend) ;

	if(!m_pGuestList || !m_pBtn_ChangeOwner || !m_pBtn_FreezeGuest || !m_pBtn_KickGuest || !m_pBtn_AddFriend)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// ��Ʈ�� ���̵� üũ.
	switch(id)
	{
	case CRGLD_BTN_CHANGEOWNER : ChangeOwner() ; break ;
	case CRGLD_BTN_FREEZEGUEST : FreezeGuest() ; break ;
	case CRGLD_BTN_KICKGUEST :	 KickGuest() ;	 break ;
	case CRGLD_BTN_ADDFRIEND :	 AddFriend() ;	 break ;
	default : break ;
	}



	//������ ������ ����Ʈ ���̾�α׿� ����.
	SetClassMark() ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : InitGuestList
//	DESC : ������ ����Ʈ�� �����ϴ� �Լ�.
//  DATE : APRIL 7, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomGuestListDlg::InitGuestList(ST_CR_USER* pUser, BYTE byCount)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pUser) ;

	if(!pUser)
	{
		CHATROOMMGR->Throw_Error(err_IMP, __FUNCTION__) ;
		return ;
	}



	// ������ ����Ʈ üũ.
	ASSERT(m_plGuest) ;

	if(!m_plGuest)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// ���� ���� ����Ʈ ���.
	ST_CR_USER* pInfo ;
	PTRLISTPOS pos = NULL ;
	pos = m_plGuest->GetHeadPosition() ;

	while(pos)
	{
		pInfo = NULL ;
		pInfo = (ST_CR_USER*)m_plGuest->GetNext(pos) ;

		if(!pInfo) continue ;

		m_plGuest->Remove(pInfo) ;

		delete pInfo ;
	}
	m_plGuest->RemoveAll() ;

	// ����Ʈ ���̾�α� ����.
	//m_pGuestList->DeleteAllItems() ;
	m_pGuestList->RemoveAll() ;



	// ������ ����Ʈ�� ���� ���� ���.
	for(BYTE count = 0 ; count < byCount ; ++count)
	{
		if(pUser[count].dwPlayerID == 0) continue ;

		//if(HEROID == pUser[count].dwPlayerID)
		//{
		//	m_MyUserInfo = pUser[count] ;
		//}

		ST_CR_USER* pGuest = NULL ;
		pGuest = new ST_CR_USER ;

		ASSERT(pGuest) ;

		if(!pGuest)
		{
			CHATROOMMGR->Throw_Error("Failed to create user!!", __FUNCTION__) ;
			return ;
		}

		memcpy(pGuest, &pUser[count], sizeof(ST_CR_USER)) ;
		m_plGuest->AddTail(pGuest) ;
	}



	// ������ ������ ����Ʈ ���̾�α׿� ����
	SetClassMark() ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : AddGuest
//	DESC : ������ �߰� ó���� �ϴ� �Լ�.
//  DATE : APRIL 7, 2008 LYW
//-------------------------------------------------------------------------------------------------
BOOL CChatRoomGuestListDlg::AddGuest(ST_CR_USER* pUser)
{
	// �Լ� �Ķ���� üũ.
	ASSERT(pUser) ;

	if(!pUser)
	{
		CHATROOMMGR->Throw_Error(err_IMP, __FUNCTION__) ;
		return FALSE ;
	}



	// ������ ����Ʈ üũ.
	ASSERT(m_plGuest) ;

	if(!m_plGuest)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return FALSE ;
	}



	// ���� ���� ����.
	ST_CR_USER* pGuest = NULL ;
	pGuest = new ST_CR_USER ;

	// ���� ���� üũ.
	ASSERT(pGuest) ;

	if(!pGuest)
	{
		CHATROOMMGR->Throw_Error("Failed to create user!!", __FUNCTION__) ;
		return FALSE ;
	}

	// ���� ���� ���.
	memcpy(pGuest, pUser, sizeof(ST_CR_USER)) ;
	m_plGuest->AddTail(pGuest) ;



	// ������ ������ ����Ʈ ���̾�α׿� ����.
	SetClassMark() ;

	return TRUE ;
}




//-------------------------------------------------------------------------------------------------
//	NAME : DeleteGuest
//	DESC : ������ ���� ó���� �ϴ� �Լ�.
//  DATE : APRIL 7, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomGuestListDlg::DeleteGuest(DWORD dwPlayerID)
{
	// ������ ����Ʈ üũ.
	ASSERT(m_plGuest) ;

	if(!m_plGuest)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// ���� �� ��� ���� ó��.
	ST_CR_USER* pInfo ;
	PTRLISTPOS pos = NULL ;
	pos = m_plGuest->GetHeadPosition() ;

	while(pos)
	{
		pInfo = NULL ;
		pInfo = (ST_CR_USER*)m_plGuest->GetNext(pos) ;

		if(!pInfo) continue ;

		if(pInfo->dwPlayerID != dwPlayerID) continue ;

		m_plGuest->Remove(pInfo) ;

		delete pInfo ;

		break ;
	}



	// ������ ������ ����Ʈ ���̾�α׿� ����
	SetClassMark() ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : SetClassMark
//	DESC : ������ ������ ����Ʈ ���̾�α׿� �����ϴ� �Լ�.
//  DATE : APRIL 7, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomGuestListDlg::SetClassMark()
{
	// ���� ��Ʈ�� üũ.
	ASSERT(m_pGuestList) ;
	
	if(!m_pGuestList)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		SetActive(FALSE) ;
		return ;
	}



	// ������ ����Ʈ üũ.
	ASSERT(m_plGuest) ;

	if(!m_plGuest)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// ���� ������ �� �ޱ�.
	int nMaxRow = 0 ;
	nMaxRow = m_plGuest->GetCount() ;

	// �ֻ��� �ε��� �ޱ�.
	LONG lTopIdx = 0 ;
	//lTopIdx = m_pGuestList->GetStartIdxOfPrintedItems() ;
	lTopIdx = m_pGuestList->GetTopListItemIdx() ;

	// �ε��� ��ȿ üũ.
	ASSERT(lTopIdx >= 0) ;

	if(lTopIdx < 0)
	{
		CHATROOMMGR->Throw_Error("Invalid Index!!", __FUNCTION__) ;
		SetActive(FALSE) ;
		return ;
	}

	//// Ŭ���� ��ũ �ʱ�ȭ.
	//for(BYTE count = 0 ; count < MAX_ROW_GUESTLIST ; ++count)
	//{
	//	ZeroMemory(&m_MarkImg[count], sizeof(cImage)) ;
	//}

	//m_pGuestList->DeleteAllItems() ;
	m_pGuestList->RemoveAll() ;

	//char Level[4] ;
	//char State[5] ;

	char tempBuff[128] = {0,} ;
	char szState[24] = {0, } ;

	DWORD dwColor = 0 ;
	dwColor = RGBA_MAKE(255, 255, 255, 255) ;

	ST_CR_USER* pInfo ;
	PTRLISTPOS pos = NULL ;
	pos = m_plGuest->GetHeadPosition() ;

	BYTE byCount = 0 ;
	while(pos)
	{
		pInfo = NULL ;
		pInfo = (ST_CR_USER*)m_plGuest->GetNext(pos) ;

		if(!pInfo) continue ;

		//int nIconNum = 0 ;
		//nIconNum = RESRCMGR->GetClassIconNum(pInfo->wClassIdx) ;
		//SCRIPTMGR->GetImage( nIconNum, &m_MarkImg[byCount], PFT_CLASS_ICON_PATH) ;

		ITEM* pItem = new ITEM ;

		ASSERT(pItem) ;

		if(!pItem)
		{
			CHATROOMMGR->Throw_Error("Failed to create item!!", __FUNCTION__) ;
			continue ;
		}

		memset(&tempBuff, 0, sizeof(tempBuff)) ;
		memset(&szState, 0, sizeof(szState)) ;

		// Ŭ���� ��ũ ���� ����.
		//SafeStrCpy(tempBuff, " ", 1) ;

		// ���� ����.
		if(CHATROOMMGR->GetMyRoomOwnerIdx() == pInfo->dwPlayerID)
		{
			if(CHATROOMMGR->IsFreezed(pInfo->dwPlayerID))
			{
				strcat(szState, CHATMGR->GetChatMsg(1484)) ;
				strcat(szState, "/") ;
				strcat(szState, CHATMGR->GetChatMsg(1485)) ;
			}
			else
			{
				SafeStrCpy(szState, CHATMGR->GetChatMsg(1484), 5) ;
			}
		}
		else if(CHATROOMMGR->IsFreezed(pInfo->dwPlayerID))
		{
			SafeStrCpy(szState, CHATMGR->GetChatMsg(1485), 5) ;
		}
		else
		{
			SafeStrCpy(szState, CHATMGR->GetChatMsg(1486), 5) ;
		}

		// �̸� ����.
		sprintf(tempBuff, "%-28s %-8s %5d", pInfo->name, szState, pInfo->byLevel) ;

		m_pGuestList->AddItem(tempBuff, dwColor, byCount) ;	

		++byCount ;

		//memset(Level, 0, sizeof(Level)) ;
		//memset(State, 0, sizeof(State)) ;

		//cRITEMEx *ritem = new cRITEMEx(4) ;

		//ASSERT(ritem) ;

		//if(!ritem)
		//{
		//	CHATROOMMGR->Throw_Error("Failed to create item!!", __FUNCTION__) ;
		//	continue ;
		//}

		//// Ŭ���� ��ũ ���� ����.
		//SafeStrCpy(ritem->pString[0], " ", 1) ;
		//ritem->rgb[0] = dwColor ;

		//// �̸� ����.
		//SafeStrCpy(ritem->pString[1], pInfo->name, MAX_NAME_LENGTH+1) ;
		//ritem->rgb[1] = dwColor ;

		//// ���� ����.
		//if(CHATROOMMGR->GetMyRoomOwnerIdx() == pInfo->dwPlayerID)
		//{
		//	if(CHATROOMMGR->IsFreezed(pInfo->dwPlayerID))
		//	{
		//		char tempState[32] = {0, } ;

		//		strcat(tempState, CHATMGR->GetChatMsg(1484)) ;
		//		strcat(tempState, "/") ;
		//		strcat(tempState, CHATMGR->GetChatMsg(1485)) ;

		//		SafeStrCpy(ritem->pString[2], tempState, 32+1) ;
		//	}
		//	else
		//	{
		//		SafeStrCpy(ritem->pString[2], CHATMGR->GetChatMsg(1484), 5) ;
		//	}
		//}
		//else if(CHATROOMMGR->IsFreezed(pInfo->dwPlayerID))
		//{
		//	SafeStrCpy(ritem->pString[2], CHATMGR->GetChatMsg(1485), 5) ;
		//}
		//else
		//{
		//	SafeStrCpy(ritem->pString[2], CHATMGR->GetChatMsg(1486), 5) ;
		//}
		//ritem->rgb[2] = dwColor ;

		//// ���� ����.
		//sprintf(Level, "%d", pInfo->byLevel) ;
		//SafeStrCpy(ritem->pString[3], Level, 4) ;
		//ritem->rgb[3] = dwColor ;

		// ������ �߰�.
		//m_pGuestList->InsertItem((WORD)count, ritem) ;	
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : SetDlgToDefault
//	DESC : ������ ����Ʈ�� �⺻ ���·� �����ϴ� �Լ�.
//  DATE : APRIL 10, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomGuestListDlg::SetDlgToDefault()
{
	// ������ ����Ʈ üũ.
	ASSERT(m_plGuest) ;

	if(!m_plGuest)
	{
		CHATROOMMGR->Throw_Error(err_ICCTLS, __FUNCTION__) ;
		return ;
	}



	// ����Ʈ ��Ʈ�� ����.
	//m_pGuestList->DeleteAllItems() ;
	m_pGuestList->RemoveAll() ;

	// ����Ʈ ����.
	ST_CR_USER* pInfo ;
	PTRLISTPOS pos = NULL ;
	pos = m_plGuest->GetHeadPosition() ;

	while(pos)
	{
		pInfo = NULL ;
		pInfo = (ST_CR_USER*)m_plGuest->GetNext(pos) ;

		if(!pInfo) continue ;

		m_plGuest->Remove(pInfo) ;

		delete pInfo ;
	}
	m_plGuest->RemoveAll() ;

	//// Ŭ���� ��ũ �ʱ�ȭ.
	//for(BYTE count = 0 ; count < MAX_ROW_GUESTLIST ; ++count)
	//{
	//	ZeroMemory(&m_MarkImg[count], sizeof(cImage)) ;
	//}



	// ���� �� ���� ���� �ʱ�ȭ.
	memset(&m_SelectedUser, 0, sizeof(ST_CR_USER)) ;

	// �� ���� �ʱ�ȭ.
	//memset(&m_MyUserInfo, 0, sizeof(ST_CR_USER)) ;



	// ������ ������ ����Ʈ ���̾�α׿� ����
	SetClassMark() ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : ChangeOwner
//	DESC : �������� ��ư�� ������ �� ó���� �ϴ� �Լ�.
//  DATE : APRIL 10, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomGuestListDlg::ChangeOwner()
{
	// �������� üũ.
	if(HEROID != CHATROOMMGR->GetMyRoomOwnerIdx())
	{
		CHATROOMMGR->AddChatRoomMsg(CHATMGR->GetChatMsg(1487), e_ChatMsg_System) ;
		return ;
	}

	// ����� ���̵� üũ.
	if(m_SelectedUser.dwPlayerID == HEROID)
	{
		CHATROOMMGR->AddChatRoomMsg(CHATMGR->GetChatMsg(1488), e_ChatMsg_System) ;
		return ;
	}

	// ����� ��ȿ�� üũ.
	if(m_SelectedUser.dwPlayerID == 0)
	{
		CHATROOMMGR->AddChatRoomMsg(CHATMGR->GetChatMsg(1489), e_ChatMsg_System) ;
		return ;
	}

	// ���� �ð� üũ.
	if(!CHATROOMMGR->IsEnableExecuteCom())
	{
		WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1517)) ;
		return ;
	}



	// ���� ���� ��û.
	MSG_DWORD msg ;
	memset(&msg, 0, sizeof(MSG_DWORD)) ;

	msg.Category	= MP_CHATROOM ;
	msg.Protocol	= MP_CHATROOM_CHANGE_OWNER_SYN ;
	msg.dwObjectID	= HEROID ;
	
	msg.dwData		= m_SelectedUser.dwPlayerID ;

	NETWORK->Send( &msg, sizeof(MSG_DWORD) ) ;



	// �ð� ������Ʈ.
	CHATROOMMGR->UpdateMyLastComTime() ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : FreezeGuest
//	DESC : ���� ��ư�� ������ �� ó���� �ϴ� �Լ�.
//  DATE : APRIL 10, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomGuestListDlg::FreezeGuest()
{
	// ����� ���̵� üũ.
	if(m_SelectedUser.dwPlayerID == HEROID)
	{
		CHATROOMMGR->AddChatRoomMsg(CHATMGR->GetChatMsg(1488), e_ChatMsg_System) ;
		return ;
	}

	// ����� ��ȿ�� üũ.
	if(m_SelectedUser.dwPlayerID == 0)
	{
		CHATROOMMGR->AddChatRoomMsg(CHATMGR->GetChatMsg(1489), e_ChatMsg_System) ;
		return ;
	}

	// ����� ���¸� �����Ѵ�.
	if(CHATROOMMGR->IsFreezed(m_SelectedUser.dwPlayerID))
	{
		CHATROOMMGR->UnFreezeUser(m_SelectedUser.dwPlayerID) ;
	}
	else if(!CHATROOMMGR->IsFreezed(m_SelectedUser.dwPlayerID))
	{
		CHATROOMMGR->FreezeUser(m_SelectedUser.dwPlayerID) ;
	}
	else return ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : KickGuest
//	DESC : �������� ��ư�� ������ �� ó���� �ϴ� �Լ�.
//  DATE : APRIL 10, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomGuestListDlg::KickGuest()
{
	// �������� üũ.
	if(HEROID != CHATROOMMGR->GetMyRoomOwnerIdx())
	{
		CHATROOMMGR->AddChatRoomMsg(CHATMGR->GetChatMsg(1487), e_ChatMsg_System) ;
		return ;
	}

	// ����� ���̵� üũ.
	if(m_SelectedUser.dwPlayerID == HEROID)
	{
		CHATROOMMGR->AddChatRoomMsg(CHATMGR->GetChatMsg(1488), e_ChatMsg_System) ;
		return ;
	}

	// ����� ��ȿ�� üũ.
	if(m_SelectedUser.dwPlayerID == 0)
	{
		CHATROOMMGR->AddChatRoomMsg(CHATMGR->GetChatMsg(1489), e_ChatMsg_System) ;
		return ;
	}

	// ���� �ð� üũ.
	if(!CHATROOMMGR->IsEnableExecuteCom())
	{
		WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1517)) ;
		return ;
	}



	// ���� ���� ��û.
	MSG_DWORD msg ;
	memset(&msg, 0, sizeof(MSG_DWORD)) ;

	msg.Category	= MP_CHATROOM ;
	msg.Protocol	= MP_CHATROOM_KICK_GUEST_SYN ;
	msg.dwObjectID	= HEROID ;
	
	msg.dwData		= m_SelectedUser.dwPlayerID ;

	NETWORK->Send( &msg, sizeof(MSG_DWORD) ) ;



	// �ð� ������Ʈ.
	CHATROOMMGR->UpdateMyLastComTime() ;
}





//-------------------------------------------------------------------------------------------------
//	NAME : AddFriend
//	DESC : ģ���߰� ��ư�� ������ �� ó���� �ϴ� �Լ�.
//  DATE : APRIL 10, 2008 LYW
//-------------------------------------------------------------------------------------------------
void CChatRoomGuestListDlg::AddFriend()
{
	// ����� ���̵� üũ.
	if(m_SelectedUser.dwPlayerID == HEROID)
	{
		CHATROOMMGR->AddChatRoomMsg(CHATMGR->GetChatMsg(1488), e_ChatMsg_System) ;
		return ;
	}

	// ����� ��ȿ�� üũ.
	if(m_SelectedUser.dwPlayerID == 0)
	{
		CHATROOMMGR->AddChatRoomMsg(CHATMGR->GetChatMsg(1489), e_ChatMsg_System) ;
		return ;
	}

	// ���� �ð� üũ.
	if(!CHATROOMMGR->IsEnableExecuteCom())
	{
		WINDOWMGR->MsgBox(MBI_CHATROOM_NOTICE, MBT_OK, CHATMGR->GetChatMsg(1517)) ;
		return ;
	}



	// ���� ���� ��û.
	MSG_DWORD msg ;
	memset(&msg, 0, sizeof(MSG_DWORD)) ;

	msg.Category	= MP_CHATROOM ;
	msg.Protocol	= MP_CHATROOM_REQUEST_FRIEND_SYN ;
	msg.dwObjectID	= HEROID ;
	
	msg.dwData		= m_SelectedUser.dwPlayerID ;

	NETWORK->Send( &msg, sizeof(MSG_DWORD) ) ;

	CHATROOMMGR->AddChatRoomMsg(CHATMGR->GetChatMsg(928), e_ChatMsg_System) ;
	CHATROOMMGR->SetRequestFriendState(TRUE) ;



	// �ð� ������Ʈ.
	CHATROOMMGR->UpdateMyLastComTime() ;
}
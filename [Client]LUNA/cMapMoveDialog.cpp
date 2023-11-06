//=================================================================================================
//	FILE		: cMapMoveDialog.cpp
//	PURPOSE		: Implemantacion part of map move dialog interface.
//	PROGRAMMER	: Yongs Lee
//	DATE		: November 30, 2007
//	ADDITION	:
//	ATTENTION	:
//=================================================================================================





//=========================================================================
//		�ʿ��� ��������� �ҷ��´�.
//=========================================================================
#include "stdafx.h"															// ǥ�� ��������� �����Ѵ�.
#include "../WindowIDEnum.h"
#include "../Interface/cWindowManager.h"
#include "../ChatManager.h"
#include "../cMsgBox.h"
#include "..\MHFile.h"

#include "./Input/Mouse.h"

#include "./Interface/cPushupButton.h"
#include "./Interface/cListDialog.h"

#include ".\cmapmovedialog.h"

#include "Item.h"

#include "ObjectManager.h"
#include "ObjectStateManager.h"





cMapMoveDialog::cMapMoveDialog(void)
{
	// ��� �������� �ʱ�ȭ �Ѵ�.
	m_pMapListDlg = NULL ;													

	m_pMoveBtn = NULL ;														
	m_pCloseBtn = NULL ;		
	m_pXCloseBtn = NULL ;

	m_nSelectedMapNum = -1 ;	

	m_pItem = NULL ;
}





cMapMoveDialog::~cMapMoveDialog(void)
{
	RelaseMapLise() ;
}





void cMapMoveDialog::Linking()
{
	m_pMapListDlg	= (cListDialog*)GetWindowForID(MMD_MAPLIST) ;				// �� ����Ʈ�� ��ũ�Ѵ�.

	m_pMoveBtn		= (cButton*)GetWindowForID(MMD_MOVEBTN) ;					// �̵� ��ư�� ��ũ�Ѵ�.
	m_pCloseBtn		= (cButton*)GetWindowForID(MMD_CLOSEBTN) ;					// �ݱ� ��ư�� ��ũ�Ѵ�.
	m_pXCloseBtn	= (cButton*)GetWindowForID(MMD_XCLOSEBTN) ;					// X�ݱ� ��ư�� ��ũ�Ѵ�.

	InitializeMapList() ;														// �� ����Ʈ ������ �ʱ�ȭ �Ѵ�.
}





DWORD cMapMoveDialog::ActionEvent(CMouse * mouseInfo)							// �׼� �̺�Ʈ �Լ�.
{
	DWORD we = WE_NULL ;														// ������ �̺�Ʈ�� ���� ������ �����ϰ� �ʱ�ȭ �Ѵ�.

	if( IsDisable() ) return we ;

	we |= cDialog::ActionEvent( mouseInfo ) ;									// ���̾�α� ������ �̺�Ʈ�� �޴´�.

	if( m_pMapListDlg->PtInWindow(mouseInfo->GetMouseX(), mouseInfo->GetMouseY()) )	// �� ����Ʈ ���̶��,					
	{
		int nSelectedLine = -1 ;
		nSelectedLine = m_pMapListDlg->GetCurSelectedRowIdx() ;					// ���� �� ���� ��ȣ�� �����Ѵ�.

		ITEM* pItem = NULL ;													// ������ �����͸� �����ϰ� null ����.
		pItem = m_pMapListDlg->GetItem(nSelectedLine) ;							// ���� �� ������ �������� �޴´�.

		if( !pItem ) return we ;												// ������ ������ �������� ������, return we .

		if( we & WE_LBTNCLICK )
		{
			m_nSelectedMapNum = pItem->line ;									// �� ��ȣ�� �����Ѵ�.			

			int BackupLine = m_pMapListDlg->GetTopListItemIdx();				// ��� ������ �����Ѵ�.

			m_pMapListDlg->RemoveAll() ;

			stMAPMOVEITEM* pNormalItem ;
			PTRLISTPOS pos = m_MapMoveList.GetHeadPosition() ;

			DWORD dwColor = RGB_HALF(10, 10, 10) ;

			while(pos)
			{
				pNormalItem = NULL ;
				pNormalItem = (stMAPMOVEITEM*)m_MapMoveList.GetNext(pos) ;

				if( !pNormalItem ) continue ;

				if( m_nSelectedMapNum == pNormalItem->mapnum )
				{
					char tempBuf[128] = {0, } ;
					strcpy(tempBuf, "[ ") ;
					strcat(tempBuf, GetMapName(m_nSelectedMapNum)) ;
					strcat(tempBuf, " ]") ;

					m_pMapListDlg->AddItem(tempBuf, RGB_HALF(255, 255, 0), pNormalItem->mapnum) ;
					continue ;
				}

				m_pMapListDlg->AddItem(pNormalItem->mapName, dwColor, pNormalItem->mapnum) ;
			}

			m_pMapListDlg->ResetGuageBarPos() ;
			m_pMapListDlg->SetTopListItemIdx(BackupLine);						// �������� ���� �ε����� �����Ѵ�.
		}

		if( we & WE_LBTNDBLCLICK )												// ���� Ŭ���� �ߴٸ�,
		{
			MoveNotice() ;														// �� �̵� ��ũ�� ��û�Ѵ�.
		}
	}

	return we ;
}





void cMapMoveDialog::OnActionEvent( LONG id, void* p, DWORD event ) 
{
	if( event != WE_BTNCLICK ) return ;

	switch( id )
	{
	case MMD_MOVEBTN :
		{
			MoveNotice() ;
		}
		break ;

	case MMD_XCLOSEBTN :
	case MMD_CLOSEBTN :
		{
			m_pItem->SetLock(FALSE) ;
			m_pItem = NULL ;
			SetActive( FALSE ) ;
		}
		break ;
	}
}





void cMapMoveDialog::InitializeMapList()
{
	CMHFile file ;

	char* pMapName ;

	WORD	mapnum = 0;

	float	posx = 0;
	float	posz = 0;

	//if(!file.Init("./System/Resource/SafetyLoc.bin", "rb"))	return ;
	if(!file.Init("./System/Resource/MapMoveScroll.bin", "rb"))	return ;

	while(!file.IsEOF())
	{
		mapnum = file.GetWord();
		/*posx = file.GetFloat();
		posz = file.GetFloat();*/

		stMAPMOVEITEM* pItem = new stMAPMOVEITEM ;

		ASSERT(pItem) ;
		ASSERT(mapnum > 0 ) ;

		pMapName = NULL ;
		pMapName = GetMapName(mapnum) ;

		ASSERT(pMapName) ;
		ASSERT(strlen(pMapName) > 4) ;

		memset(pItem, 0, sizeof(stMAPMOVEITEM)) ;
		pItem->mapnum = mapnum ;
		strcpy(pItem->mapName, pMapName) ;

		m_MapMoveList.AddTail(pItem) ;
		m_pMapListDlg->AddItem(pMapName, RGB_HALF(10, 10, 10), mapnum) ;
	}

	file.Release() ;
}





void cMapMoveDialog::RelaseMapLise()
{
	stMAPMOVEITEM* pItem ;
	PTRLISTPOS pDeletePos = m_MapMoveList.GetHeadPosition() ;

	while( pDeletePos )
	{
		pItem = NULL ;
		pItem = (stMAPMOVEITEM*)m_MapMoveList.GetNext(pDeletePos) ;

		if( !pItem ) continue ;

		delete pItem ;
	}

	m_MapMoveList.RemoveAll() ;
}





void cMapMoveDialog::MoveMapSyn()
{
	MSG_DWORD3 msg ;
	memset(&msg, 0, sizeof(MSG_DWORD3)) ;

	if( m_nSelectedMapNum <= 0 || m_nSelectedMapNum >= 100 )
	{
		WINDOWMGR->MsgBox( MBI_MAPMOVE_NOTICE, MBT_OK, CHATMGR->GetChatMsg( 56 ) ) ;	
		SetDisableState(FALSE) ;
		return ;
	}

	//---KES PK Mode 080104
	if( HERO->IsPKMode() )
	{
		WINDOWMGR->MsgBox( MBI_MAPMOVE_NOTICE, MBT_OK, CHATMGR->GetChatMsg( 358 ) ) ;	
		SetDisableState(FALSE) ;
		return ;
	}
	//--------------------

	msg.Category	= MP_ITEM ;
	msg.Protocol	= MP_ITEM_MAPMOVE_SCROLL_SYN ;
	msg.dwObjectID	= HEROID ;
	msg.dwData1		= m_nSelectedMapNum ;
	msg.dwData2		= m_pItem->GetPosition() ;
	msg.dwData3		= m_pItem->GetItemIdx() ;

	NETWORK->Send( &msg, sizeof(MSG_DWORD3) ) ;
}


void cMapMoveDialog::MoveNotice() 
{
	if( !m_pItem )
	{
		WINDOWMGR->MsgBox( MBI_MAPMOVE_NOTICE, MBT_OK, CHATMGR->GetChatMsg( 1207 ) );	
		SetActive(FALSE) ;
		return ;
	}

	BOOL bValidMapNum = FALSE ;

	stMAPMOVEITEM* pItem ;
	PTRLISTPOS pDeletePos = m_MapMoveList.GetHeadPosition() ;

	while( pDeletePos )
	{
		pItem = NULL ;
		pItem = (stMAPMOVEITEM*)m_MapMoveList.GetNext(pDeletePos) ;

		if( !pItem ) continue ;

		if( pItem->mapnum == m_nSelectedMapNum )
		{
			WINDOWMGR->MsgBox( MBI_MAPMOVE_AREYOUSURE, MBT_YESNO, CHATMGR->GetChatMsg(480), pItem->mapName ) ;	
			SetDisableState(TRUE) ;
			return ;
		}
	}

	if( !bValidMapNum )
	{
		WINDOWMGR->MsgBox( MBI_MAPMOVE_NOTICE, MBT_OK, CHATMGR->GetChatMsg( 1208 ) );	
		return ;
	}
}





void cMapMoveDialog::SetDisableState(BOOL val)
{
	if( val )
	{
		OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal) ;
	}
	else
	{
		OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal) ;
	}

	m_pMapListDlg->SetDisable(val) ;
	SetDisable(val) ;
}

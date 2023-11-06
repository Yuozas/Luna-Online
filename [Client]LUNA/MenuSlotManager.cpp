// MenuSlotManager.cpp: implementation of the CMenuSlotManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MenuSlotManager.h"
#include "./Interface/cWindowManager.h"
#include "ObjectManager.h"
#include "MenuIcon.h"
#include "MenuSlotDialog.h"
#include "GameIn.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CMenuSlotManager)

CMenuSlotManager::CMenuSlotManager()
{
	m_IconIndexCreator.Init( MAX_ITEMICON_NUM, IG_MENUSLOTITEM_STARTINDEX );
}

CMenuSlotManager::~CMenuSlotManager()
{
	m_IconIndexCreator.Release();
}

void CMenuSlotManager::Init()
{

}

void CMenuSlotManager::Release()
{

}

BOOL CMenuSlotManager::AddMenuIcon( WORD wPage, WORD wPos, cIcon* pSrcIcon )
{

	return FALSE;
}

BOOL CMenuSlotManager::DeleteMenuIcon( CMenuIcon* pMenuIcon )
{
	if( GAMEIN->GetMenuSlotDialog()->DeleteMenuIcon( pMenuIcon ) == TRUE )
	{
		//ablility update		

		pMenuIcon->SetDisable( TRUE );
		pMenuIcon->SetActive( FALSE );
		WINDOWMGR->AddListDestroyWindow( (cWindow*)pMenuIcon );

		return TRUE;
	}
	else
	{
		return FALSE;		
	}
}

BOOL CMenuSlotManager::ExchangeMenuIcon( CMenuIcon* pMenuIcon1, CMenuIcon* pMenuIcon2 )
{
	if( GAMEIN->GetMenuSlotDialog()->ExchangeMenuIcon( pMenuIcon1, pMenuIcon2 ) )
	{
		//abliltiyIcon의 SlotPos바꾸기
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CMenuSlotManager::MoveMenuIcon( WORD wPage, WORD wPos, CMenuIcon* pMenuIcon )
{
	if( GAMEIN->GetMenuSlotDialog()->MoveMenuIcon( wPage, wPos, pMenuIcon ) )
	{
		//abliltiyIcon의 SlotPos바꾸기
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CMenuSlotManager::UseMenuIcon( CMenuIcon* pMenuIcon )
{
	return TRUE;
}
// SkillPointRedist.cpp: implementation of the CSkillPointRedist class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillPointRedist.h"
#include "./Interface/cIconGridDialog.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cScriptManager.h"
#include "./Interface/cButton.h"
#include "./Interface/cStatic.h"
#include "WindowIDEnum.h"

#include "GameIn.h"

#include "ObjectManager.h"

#include "ChatManager.h"
#include "ItemManager.h"

#include "ObjectStateManager.h"
#include "./Interface/cPushupButton.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillPointRedist::CSkillPointRedist()
{
	m_type = WT_SKPOINTDIALOG;
	m_dwCurTabNum = eTab_War;
}

CSkillPointRedist::~CSkillPointRedist()
{

}


void CSkillPointRedist::SetActive( BOOL val )
{
	cDialog::SetActive( val );

	if( !val )
	{
		SetDisable( FALSE );
		OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
	}
}


void CSkillPointRedist::Linking()
{
	m_UpBtn = (cButton*)GetWindowForID( SK_UPBTN );
	m_DownBtn = (cButton*)GetWindowForID( SK_DOWNBTN );
	m_OkBtn = (cButton*)GetWindowForID( SK_OKBTN );
	m_RePointst = (cStatic*)GetWindowForID( SK_POINTSTATIC );
	m_UsePointst = (cStatic*)GetWindowForID( SK_USESTATIC );
	m_OgPointst = (cStatic*)GetWindowForID( SK_ORIGINALSTATIC );

	for(int i=0; i<eTab_Max; ++i)
	{
		m_IconGrid[i] = (cIconGridDialog*)GetWindowForID( SK_ICONGRID1+i );
		m_GridButton[i] = (cPushupButton*)GetWindowForID( SK_POINTAGAIN1BTN+i );

		if(m_IconGrid[i])
			m_IconGrid[i]->SetActive( FALSE );
		if(m_GridButton[i])
			m_GridButton[i]->SetPush( FALSE );
	}
	if( m_IconGrid[eTab_War] )
		m_IconGrid[eTab_War]->SetActive( TRUE );
	if( m_GridButton[eTab_War] )
		m_GridButton[eTab_War]->SetPush( TRUE );
}
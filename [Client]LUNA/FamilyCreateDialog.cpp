// FamilyCreateDialog.cpp: implementation of the CFamilyCreateDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FamilyCreateDialog.h"
#include "./Interface/cWindowManager.h"
#include "WindowIDEnum.h"
#include "./interface/cStatic.h"
#include "./interface/cEditBox.h"
#include "./interface/cTextArea.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "GameIn.h"
#include "NpcScriptDialog.h"
#include "ChatManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFamilyCreateDialog::CFamilyCreateDialog()
{
	m_type = WT_FAMILYCREATEDLG;
}

CFamilyCreateDialog::~CFamilyCreateDialog()
{

}

void CFamilyCreateDialog::Linking()
{
	m_pLocation = (cStatic*)GetWindowForID(FD_CLOCATION);
	m_pFamilyName = (cEditBox*)GetWindowForID(FD_CNAME);
	m_pIntro = (cTextArea*)GetWindowForID(FD_CINTROTEXT);
}

void CFamilyCreateDialog::SetActive(BOOL val)
{
	if(val == TRUE)
	{
		m_pLocation->SetStaticText(GetMapName(HERO->GetCharacterTotalInfo()->CurMapNum));
		m_pFamilyName->SetEditText("");
		m_pIntro->SetScriptText("");
	}
	else
	{
		m_pFamilyName->SetFocusEdit(FALSE);
		if(HERO == 0)
			return;
		if( ( HERO->GetState() == eObjectState_Deal ) && (GAMEIN->GetNpcScriptDialog()->IsActive() == FALSE))
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
	}
	cDialog::SetActive(val);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFamilyUnionCreateDialog::CFamilyUnionCreateDialog()
{
	m_type = WT_FAMILYUNIONCREATEDLG;
}

CFamilyUnionCreateDialog::~CFamilyUnionCreateDialog()
{
}

void CFamilyUnionCreateDialog::SetActive( BOOL val )
{
	if( val == TRUE )
	{
		m_pNameEdit->SetEditText("");
	}	
	else
	{
		if( HERO == 0 )	return;
		if( (HERO->GetState() == eObjectState_Deal) && (GAMEIN->GetNpcScriptDialog()->IsActive() == FALSE) )
			OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
		m_pNameEdit->SetFocusEdit( FALSE );
	}
	cDialog::SetActive(val);
}

void CFamilyUnionCreateDialog::Linking()
{
	m_pNameEdit = (cEditBox*)GetWindowForID(GDU_NAME);
	m_pOkBtn = (cButton*)GetWindowForID(GDU_OKBTN);
	m_pText = (cTextArea*)GetWindowForID(GDU_TEXT);

	m_pText->SetScriptText( CHATMGR->GetChatMsg( 1125 ) );
}
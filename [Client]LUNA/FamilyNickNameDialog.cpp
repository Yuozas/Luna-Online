// FamilyNickNameDialog.cpp: implementation of the CFamilyNickNameDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FamilyNickNameDialog.h"
#include "./interface/cTextArea.h"
#include "./interface/cEditBox.h"
#include "cImeEx.h"
#include "WindowIDEnum.h"
#include "ChatManager.h"
//#include "FamilyManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFamilyNickNameDialog::CFamilyNickNameDialog()
{
	m_type = WT_FAMILYNICKNAMEDLG;
}

CFamilyNickNameDialog::~CFamilyNickNameDialog()
{

}

void CFamilyNickNameDialog::Linking()
{
	m_pNickMsg = (cTextArea*)GetWindowForID(FD_NICKTEXTAREA);
	m_pNickName = (cEditBox*)GetWindowForID(FD_NICKNAMEEDIT);
	m_pNickName->SetValidCheck( VCM_SPACE );
}

//void CFamilyNickNameDialog::SetActive(BOOL val)
//{
//	if(val == TRUE)
//	{
//		if(FAMILYMGR->GetSelectedMemberID() == 0)
//		{			
//			cDialog::SetActive(FALSE);
//			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(714));
//			return;
//		}
//		m_pNickName->SetEditText("");
//		SetNickMsg(FAMILYMGR->GetSelectedMemberName());
//	}
//	else
//	{
//		m_pNickName->SetFocusEdit(FALSE);
//	}
//	cDialog::SetActive(val);
//}

void CFamilyNickNameDialog::SetActive(BOOL val)
{
	if( val == TRUE )
	{
		m_pNickName->SetEditText("");
	}

	cDialog::SetActive(val);
}

void CFamilyNickNameDialog::SetNickMsg(char* Name)
{
	char text[128];
	sprintf(text, CHATMGR->GetChatMsg(704), Name);
	m_pNickMsg->SetScriptText(text);
}


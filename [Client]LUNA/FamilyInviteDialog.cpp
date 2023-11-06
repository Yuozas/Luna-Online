// FamilyInviteDialog.cpp: implementation of the CFamilyInviteDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FamilyInviteDialog.h"
#include "./interface/cTextArea.h"
//#include "FamilyManager.h"
#include "ChatManager.h"
#include "WindowIDEnum.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFamilyInviteDialog::CFamilyInviteDialog()
{
	m_type = WT_FAMILYINVITEDLG;
}

CFamilyInviteDialog::~CFamilyInviteDialog()
{

}

void CFamilyInviteDialog::Linking()
{
	m_pInviteMsg = (cTextArea*)GetWindowForID(FD_IINVITE);                                                                                                                                                                                                                                                             
	
}

void CFamilyInviteDialog::SetInfo(char* FamilyName, char* MasterName, int FlgKind)
{
	// 070801 웅주, 기존 패밀리 제거하며 주석처리

	//char text[128];

	//if(FlgKind == AsFamilyMember)
	//{
	//	sprintf(text, CHATMGR->GetChatMsg(45), FamilyName, MasterName);
	//}
	//else	//AsStudent
	//{
	//	sprintf(text, CHATMGR->GetChatMsg(1370), MasterName, FamilyName);
	//}
	//m_pInviteMsg->SetScriptText(text);
}
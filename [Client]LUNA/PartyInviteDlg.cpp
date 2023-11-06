// PartyInviteDlg.cpp: implementation of the CPartyInviteDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartyInviteDlg.h"
#include "./Interface/cWindowManager.h"
#include "WindowIDEnum.h"
#include "./Interface/cButton.h"
#include "./Interface/cTextArea.h"
#include "./Interface/cStatic.h"
#include "ChatManager.h"
#include "cResourceManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPartyInviteDlg::CPartyInviteDlg()
{
	m_type = WT_PARTYINVITEDLG;
}

CPartyInviteDlg::~CPartyInviteDlg()
{

}

void CPartyInviteDlg::Linking()
{
	m_pDistribute = (cStatic*)GetWindowForID(PA_INVITEDISTRIBUTE);
	m_pInviter = (cTextArea*)GetWindowForID(PA_INVITER);
	m_pOK = (cButton*)GetWindowForID(PA_INVITEOK);
	m_pCancel = (cButton*)GetWindowForID(PA_INVITECANCEL);
}

void CPartyInviteDlg::SetMsg(char* pInviter, BYTE Option)
{
	char buf[256] = {0,};
	char Opt[256] = {0,};
	// 070508 LYW --- PartyInviteDlg : Modified set message.
	/*
	if(Option == ePartyOpt_Random)
		SafeStrCpy(Opt, CHATMGR->GetChatMsg(640), 64);
	else if(Option == ePartyOpt_Damage)
		SafeStrCpy(Opt, CHATMGR->GetChatMsg(641), 32);
	sprintf(buf, CHATMGR->GetChatMsg(305), pInviter);
	m_pDistribute->SetStaticText(Opt);
	m_pInviter->SetScriptText(buf);
	*/

	switch(Option)
	{
	case ePartyOpt_Random :
		{
			SafeStrCpy(Opt, RESRCMGR->GetMsg( 162 ), 64);
		}
		break ;

	case ePartyOpt_Damage :
		{
			SafeStrCpy(Opt, RESRCMGR->GetMsg( 163 ), 64);
		}
		break ;

	case ePartyOpt_Sequence :
		{
			SafeStrCpy(Opt, RESRCMGR->GetMsg( 161 ), 64);
		}
		break ;
	}

	sprintf(buf, CHATMGR->GetChatMsg(765), pInviter);

	m_pDistribute->SetStaticText(Opt);
	m_pInviter->SetScriptText(buf);
}
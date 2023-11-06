// ShoutDialog.cpp: implementation of the CShoutDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShoutDialog.h"
#include "WindowIDEnum.h"
#include ".\Interface\cWindowManager.h"
#include ".\interface\cTextArea.h"
#include ".\interface\cEditBox.h"
#include ".\interface\cStatic.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "FilteringTable.h"
#include "Item.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShoutDialog::CShoutDialog()
{
	m_type = WT_SHOUT_DLG;
	m_dwItemIdx = m_dwItemPos = 0;
	m_pItem = NULL;
}

CShoutDialog::~CShoutDialog()
{
	
}


void CShoutDialog::Linking()
{
	//m_pMsgBox = (cEditBox*)GetWindowForID( CHA_MSG );

	m_pMsgText = (cTextArea*)GetWindowForID(SHOUT_TEXTAREA_MSG);
	m_pMsgText->SetEnterAllow(FALSE);
	m_pMsgText->SetScriptText("");

	m_pOkBtn = (cButton*)GetWindowForID(SHOUT_BTN_OK);
	m_pCancelBtn = (cButton*)GetWindowForID(SHOUT_BTN_CANCEL);
}


BOOL CShoutDialog::SendShoutMsgSyn()
{
	if(m_pMsgText)
	{
		WCHAR pWText[MAX_SHOUT_LENGTH + 1] = {0,};
		char szFullText[200];
		m_pMsgText->GetScriptText(szFullText);
		int nLen = strlen(szFullText);

		if( nLen == 0 )
			return FALSE;

		if(nLen > MAX_SHOUT_LENGTH)
			nLen = MAX_SHOUT_LENGTH;

		char szShout[MAX_SHOUT_LENGTH + 1] = {0,};
		// 문자열 컷팅은 유니코드로 변환후 작업한다.
		MultiByteToWideChar(CP_ACP, 0, szFullText, nLen, pWText, MAX_SHOUT_LENGTH/2);
		WideCharToMultiByte(CP_ACP, 0, pWText, MAX_SHOUT_LENGTH/2, szShout, nLen, 0, 0);


		if( FILTERTABLE->FilterChat( szShout ) )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1225) );
			return FALSE;
		}

		// 서버로 전송
		SEND_SHOUTBASE_ITEMINFO msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_SHOUT_SYN;
		msg.dwObjectID = HERO->GetID();
		msg.dwItemIdx = m_pItem->GetItemIdx();
		msg.dwItemPos = (WORD)m_pItem->GetPosition();
		strcpy(msg.Name, HERO->GetObjectName());
		strcpy(msg.Msg, szShout);

		NETWORK->Send(&msg, msg.GetMsgLength());

		return TRUE;
	}
	
	return FALSE;
}

void CShoutDialog::SetActive(BOOL val)
{
	if( m_bDisable ) return;

	if(val == TRUE)
	{
		m_pMsgText->SetScriptText("");
		m_pMsgText->SetFocusEdit(TRUE);
	}
	else
	{
		m_pItem->SetLock(FALSE);
		m_pItem = NULL;
		m_pMsgText->SetScriptText("");
		m_pMsgText->SetFocusEdit(FALSE);
	}
	cDialog::SetActive(val);
}

#include "stdafx.h"
#include "UnionNoteDlg.h"
#include "WindowIDEnum.h"
#include "ObjectManager.h"
#include "Hero.h"
#include "Item.h"
#include "./Interface\cTextArea.h"
#include "ChatManager.h"

CUnionNoteDlg::CUnionNoteDlg(void)
{
	m_bUse = FALSE;
}

CUnionNoteDlg::~CUnionNoteDlg(void)
{
}

void CUnionNoteDlg::Linking()
{
	m_pNoteText = (cTextArea*)GetWindowForID(AN_TEXTREA);

	m_pNoteText->SetEnterAllow(FALSE);
	m_pNoteText->SetScriptText("");
}

void CUnionNoteDlg::Show(CItem* pItem)
{
	if( !HERO->GetGuildIdx() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(968) );
		return;
	}
	if( HERO->GetGuildMemberRank() != GUILD_MASTER && HERO->GetGuildMemberRank() != GUILD_VICEMASTER )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(969) );
		return;
	}
	if( !HERO->GetGuildUnionIdx() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(970) );
		return;
	}

	if(pItem == NULL)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(971) );
		//786 "������ ��뿡 �����Ͽ����ϴ�."
		return;
	}
	if(m_bUse)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(972) );
		//752 "�̹� ������� ������ �Դϴ�."
		return;
	}

	m_pItem = pItem;

	SetActive(TRUE);
}

void CUnionNoteDlg::OnActionEvnet(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		switch(lId)
		{
		case AN_SENDOKBTN:
			{
				MSG_GUILD_SEND_NOTE msg;
				msg.Category = MP_GUILD_UNION;
				msg.Protocol = MP_GUILD_UNION_NOTE_SYN;
				msg.FromId = HEROID;
				msg.dwObjectID = HERO->GetGuildUnionIdx();

				SafeStrCpy(msg.FromName, HERO->GetObjectName(), MAX_NAME_LENGTH+1);
				m_pNoteText->GetScriptText(msg.Note);

				NETWORK->Send(&msg, msg.GetMsgLength());

				SetActive(FALSE);
			}
		case AN_CANCELBTN:
			{
				SetActive(FALSE);
			}
		}
	}
}

void CUnionNoteDlg::Use()
{
	m_bUse = FALSE;
	m_pNoteText->SetScriptText("");

	MSG_ITEM_USE_SYN msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_USE_SYN;
	msg.dwObjectID = HEROID;
	msg.wItemIdx = m_pItem->GetItemIdx();
	msg.TargetPos = m_pItem->GetPosition();

	NETWORK->Send(&msg,sizeof(msg));
}

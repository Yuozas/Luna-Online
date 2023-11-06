#include "stdafx.h"
#include "FamilyNoteDlg.h"
#include "WindowIDEnum.h"
#include "ObjectManager.h"
#include "Hero.h"
#include "Item.h"
#include "./Interface\cTextArea.h"
#include "ChatManager.h"

CFamilyNoteDlg::CFamilyNoteDlg(void)
{
	m_bUse = FALSE;
}

CFamilyNoteDlg::~CFamilyNoteDlg(void)
{
}

void CFamilyNoteDlg::Linking()
{
	m_pNoteText = (cTextArea*)GetWindowForID(FN_TEXTREA);

	m_pNoteText->SetEnterAllow(FALSE);
	m_pNoteText->SetScriptText("");
}

void CFamilyNoteDlg::Show(CItem* pItem)
{
	if( !HERO->GetFamilyIdx() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(35) );
		return;
	}
    if(pItem == NULL)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(786) );
		//786 "������ ��뿡 �����Ͽ����ϴ�."
		return;
	}
	if(m_bUse)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(752) );
		//752 "�̹� ������� ������ �Դϴ�."
		return;
	}

	m_pItem = pItem;

	SetActive(TRUE);
}

void CFamilyNoteDlg::OnActionEvnet(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		switch(lId)
		{
		case GN_SENDOKBTN:
			{
				// 070801 ����, ���� �йи� �����ϸ� �ּ�ó��

				//MSG_FAMILY_SEND_NOTE msg;
				//msg.Category = MP_FAMILY;
				//msg.Protocol = MP_FAMILY_NOTE_SYN;
				//msg.FromId = HEROID;
				//msg.dwObjectID = HERO->GetFamilyIdx();

				//SafeStrCpy(msg.FromName, HERO->GetObjectName(), MAX_NAME_LENGTH+1);
				//m_pNoteText->GetScriptText(msg.Note);

				//NETWORK->Send(&msg, msg.GetMsgLength());

				//SetActive(FALSE);
			}
		case GN_CANCELBTN:
			{
				SetActive(FALSE);
			}
		}
	}
}

void CFamilyNoteDlg::Use()
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


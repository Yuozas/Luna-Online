#include "stdafx.h"
#include "GuildRevivalDialog.h"
#include "WindowIDEnum.h"
#include "cWindowManager.h"
#include "cMsgBox.h"
#include "ChatManager.h"

#include "ChatManager.h"


CGuildRevivalDialog::CGuildRevivalDialog()
{}


CGuildRevivalDialog::~CGuildRevivalDialog()
{}


void CGuildRevivalDialog::Linking()
{}


void CGuildRevivalDialog::OnActionEvent( LONG id, void* p, DWORD event )
{
	switch( id )
	{
	// �������� ��Ȱ�Ѵ�
	case GD_REVIVAL_AT_TOWN:
		{
			//WINDOWMGR->MsgBox( MBI_GUILDFIELDWAR_REVIVE, MBT_YESNO, "���� �������� ��Ȱ�Ͻðڽ��ϱ�" ) ;
			WINDOWMGR->MsgBox( MBI_GUILDFIELDWAR_REVIVE, MBT_YESNO, CHATMGR->GetChatMsg( 1305 ) );
			break;
		}
	}
}
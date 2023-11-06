// ExitManager.cpp: implementation of the CExitManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExitManager.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "MoveManager.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "cMsgBox.h"
#include "MainGame.h"
#include "PKManager.h"
#include "MHTimeManager.h"
extern HWND _g_hWnd;

// 070122 LYW --- Include header file.
#include "cResourceManager.h"
#include "StreetStallManager.h"

GLOBALTON(CExitManager);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExitManager::CExitManager()
{
	m_bExitProcessing	= FALSE;
	m_dwStartTime		= 0;
	m_nExitCount		= EXIT_COUNT;
	m_nCount			= 0;
	m_bExitSync			= FALSE;
}

CExitManager::~CExitManager()
{

}


int CExitManager::CanExit()
{
	if( HERO->IsPKMode() )	//PK����߿� ������ �� �����ϴ�.
		return eEXITCODE_PKMODE;

	if( PKMGR->IsPKLooted() )	//PK������ ���ϴ� �߿��� ������ �� ����.
		return eEXITCODE_LOOTING;

	if( HERO->GetState() == eObjectState_Exchange )	//��ȯ�߿� ������ �� ����.
		return eEXITCODE_NOT_ALLOW_STATE;

	if( HERO->GetState() == eObjectState_StreetStall_Owner ||
		HERO->GetState() == eObjectState_StreetStall_Guest )	//�����߿� ������ �� ����.
		return eEXITCODE_NOT_ALLOW_STATE;

	if( HERO->GetState() == eObjectState_Deal )	//���� �̿��߿� ������ �� ����.
		return eEXITCODE_NOT_ALLOW_STATE;

	return eEXITCODE_OK;
}


BOOL CExitManager::SendExitMsg( int nExitKind )
{
	if( m_bExitSync || m_bExitProcessing ) return FALSE;

	m_nExitKind		= nExitKind;
	m_bExitSync		= TRUE;

////
	if( HERO->GetState() == eObjectState_StreetStall_Owner )
	{
		STREETSTALLMGR->CloseStreetStall();
	}
	HERO->DisableAutoAttack();					//�ڵ� ���� ���
	HERO->SetNextAction(NULL);					//��ų ���
	MOVEMGR->HeroMoveStop();					//�̵�����
	OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Exit );
////
	
	MSGBASE msg;
	msg.Category	= MP_CHAR;
	msg.Protocol	= MP_CHAR_EXITSTART_SYN;
	msg.dwObjectID	= HEROID;	
	NETWORK->Send( &msg, sizeof(msg) );
	
	return TRUE;
}

void CExitManager::StartExit()	//Start_Ack
{
	//m_bExitSync		= FALSE;	//���⼱ false�� ������ �ʴ´�. ī��Ʈ������
	m_bExitProcessing	= TRUE;
	DWORD dwCurTime = MHTIMEMGR->GetNewCalcCurTime();
	m_dwStartTime		= dwCurTime - 1000;	//�������ڸ��� ī��Ʈ�� �ϱ����ؼ� -1000
	m_nCount			= m_nExitCount;
}

void CExitManager::RejectExit( int nExitCode )	//Start_Nack
{
	m_bExitSync			= FALSE;
	OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Exit );

	switch( nExitCode )
	{
	case eEXITCODE_PKMODE:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(552) );	// "��⸦ �巯�� ���¿����� ������ �� �����ϴ�."
		break;
	case eEXITCODE_LOOTING:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(553) );	//"������ ���ϴ� �߿��� ������ �� �����ϴ�."
		break;
	default:
		// 070622 LYW --- GlobalEventFunc : �޽��� ��ȣ ����.
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(219) );	//"������ �� ���� �����Դϴ�."
		break;
	}
	
}

void CExitManager::Exit()
{
	if( m_nExitKind == eEK_GAMEEXIT )
	{
//		MAINGAME->SetGameState( eGAMESTATE_END );
		WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, MBT_NOBTN, CHATMGR->GetChatMsg( 307 ) );
		PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
	}
	else if( m_nExitKind == eEK_CHARSEL )
	{
		MSGBASE data;
		data.Category	= MP_USERCONN;
		data.Protocol	= MP_USERCONN_BACKTOCHARSEL_SYN;
		data.dwObjectID	= HEROID;
		NETWORK->Send(&data,sizeof(data));

		// 070122 LYW --- Modified this line.
		//WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, MBT_NOBTN, CHATMGR->GetChatMsg( 290 ) );
		WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, MBT_NOBTN, RESRCMGR->GetMsg( 262 ) );
		//MP_USERCONN_CHARACTERLIST_ACK�� ���� ĳ������â���� ����...
	}
	
	m_bExitSync = FALSE;	//Ȯ���ϰ� �Ϸ���....?
}

void CExitManager::CancelExit( int nErrorCode )	//�װų� ��Ÿ���ο� ���ؼ� ��ҵ�
{
	m_bExitProcessing	= FALSE;
	m_bExitSync			= FALSE;
	OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Exit );

	switch( nErrorCode )
	{
	case eEXITCODE_PKMODE:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(552) );
		break;
	case eEXITCODE_LOOTING:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(553) );
		break;
	case eEXITCODE_SPEEDHACK:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(556) );
		break;
	case eEXITCODE_DIEONEXIT:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(555) );
		break;
	default:
		// 070622 LYW --- GlobalEventFunc : �޽��� ��ȣ ����.
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(219) );
		break;
	}	
}

void CExitManager::Process()
{
	if( !m_bExitProcessing || !m_bExitSync ) return;

	if( gCurTime - m_dwStartTime >= 1000 )	//1�ʸ��� üũ
	{
		m_dwStartTime += 1000;
		
		if( m_nCount <= 0  )
		{
			m_bExitProcessing = FALSE;
			
			MSGBASE msg;
			msg.Category	= MP_CHAR;
			msg.Protocol	= MP_CHAR_EXIT_SYN;
			msg.dwObjectID	= HEROID;
			NETWORK->Send( &msg, sizeof(msg) );
		}
		else
		{
			// 070125 LYW --- Modified this part.
			/*
			if( m_nExitKind == eEK_GAMEEXIT )
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 540 ), m_nCount );
			else if( m_nExitKind == eEK_CHARSEL )
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 541 ), m_nCount );
				*/
			if( m_nExitKind == eEK_GAMEEXIT )
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 300 ), m_nCount );
			else if( m_nExitKind == eEK_CHARSEL )
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 218 ), m_nCount );

		}

		--m_nCount;		//ī��Ʈ�ٿ�
	}
}

// HackCheck.cpp: implementation of the CHackCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HackCheck.h"
#include "ExitManager.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "cMsgBox.h"
#include "ChatManager.h"
#include "MHTimeManager.h"
// 090109 LUJ, Ŭ���̾�Ʈ�� ���� �ð� �Ŀ� �����Ű�� ���� ��
#include "GlobalEventFunc.h"

#ifdef _NPROTECT_
#include "NProtectManager.h"
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHackCheck::CHackCheck()
{
	m_bIsHackUser	= FALSE;
	m_dwLastTime	= gCurTime;
	// 090109 LUJ, ��ũ��Ʈ üũ ���� �ʱ�ȭ
	ZeroMemory( &mScriptHackCheck, sizeof( mScriptHackCheck ) );
}

CHackCheck::~CHackCheck()
{
	PTRLISTPOS pos = m_ListSpeedHackCheck.GetHeadPosition();
	while( pos )
	{
		sCHECKTIME* pTime = (sCHECKTIME*)m_ListSpeedHackCheck.GetNext( pos );
		delete pTime;
	}
	m_ListSpeedHackCheck.RemoveAll();
}

void CHackCheck::Release()
{
	PTRLISTPOS pos = m_ListSpeedHackCheck.GetHeadPosition();
	while( pos )
	{
		sCHECKTIME* pTime = (sCHECKTIME*)m_ListSpeedHackCheck.GetNext( pos );
		delete pTime;
	}
	m_ListSpeedHackCheck.RemoveAll();
	
	m_bIsHackUser	= FALSE;
	m_dwLastTime	= gCurTime;
	// 090109 LUJ, ������ �� �ʱ�ȭ���Ѿ� ��� �������� �ʴ´�
	ZeroMemory( &mScriptHackCheck, sizeof( mScriptHackCheck ) );
}

void CHackCheck::Process()
{
	// 090109 LUJ, ���� �ֱ⸶�� ��ũ��Ʈ���� ������ �������� ������ �����Ѵ�
	if( mScriptHackCheck.mCheckStepTick )
	{
		const BOOL isCheckTime = ( mScriptHackCheck.mNextCheckTick < gCurTime );

		if( isCheckTime )
		{
			MSG_SCRIPT_CHECK message;
			ZeroMemory( &message, sizeof( message ) );
			message.Category	= MP_HACKCHECK;
			message.Protocol	= MP_HACKCHECK_SCRIPTHACK_SYN;
			message.dwObjectID	= HEROID;
			message.mCheckValue	= mScriptHackCheck.mCheckValue;

			NETWORK->Send(
				&message,
				sizeof( message ) );

			mScriptHackCheck.mNextCheckTick = gCurTime + mScriptHackCheck.mCheckStepTick;
		}
	}

	if( m_ListSpeedHackCheck.IsEmpty() )
		return;

	if( gCurTime - m_dwLastTime >= SPEEDHACK_CHECKTIME )	//gCurTime�� �����ð�(�����ð�)�̶� �������.
	{
#ifdef _NPROTECT_
		NPROTECTMGR->Check();
#endif

		sCHECKTIME* pTime = (sCHECKTIME*)m_ListSpeedHackCheck.GetHead();
		if( pTime )
		{
			MSG_DWORD msg;
			msg.Category	= MP_HACKCHECK;
			msg.Protocol	= MP_HACKCHECK_SPEEDHACK;
			msg.dwObjectID	= HEROID;
			msg.dwData		= pTime->dwServerSendTime;
			NETWORK->Send( &msg, sizeof( msg ) );

			m_ListSpeedHackCheck.RemoveHead();
			delete pTime;

			if( !m_ListSpeedHackCheck.IsEmpty() )
			{
				m_dwLastTime = ((sCHECKTIME*)m_ListSpeedHackCheck.GetHead())->dwArrivedTime;
			}
		}
	}
}

void CHackCheck::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch( Protocol )
	{
	case MP_HACKCHECK_SPEEDHACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			sCHECKTIME* pTime = new sCHECKTIME;
			//pTime->dwArrivedTime = gCurTime;	//���⿡ ���� �ð���?
			pTime->dwArrivedTime	= MHTIMEMGR->GetNewCalcCurTime();
			pTime->dwServerSendTime = pmsg->dwData;
			m_ListSpeedHackCheck.AddTail( pTime );
			m_dwLastTime = ((sCHECKTIME*)m_ListSpeedHackCheck.GetHead())->dwArrivedTime;
		}
		break;
	case MP_HACKCHECK_BAN_USER:
		{
			m_bIsHackUser = TRUE;
			//Ÿ�̸� 10�� ���Ŀ� �����ؾ� �ϴ°ǰ�?
		}
		break;
		// 090109 LUJ, ��ũ��Ʈ üũ ����
	case MP_HACKCHECK_SCRIPTHACK_ACK:
		{
			const MSG_DWORD* const message = ( MSG_DWORD* )pMsg;

			mScriptHackCheck.mCheckStepTick = message->dwData;
			SetScriptCheckValue( mScriptHackCheck.mCheckValue );
		}
		break;
		// 090109 LUJ, ��ũ��Ʈ�� �߸��Ǿ� ���� �����ؾ��ϴ� ���
	case MP_HACKCHECK_SCRIPTHACK_NACK:
		{
			CHATMGR->AddMsg(
				CTC_SYSMSG,
				CHATMGR->GetChatMsg( 1275 ) );
			// 090115 LUJ, GM���� Ŭ���̾�Ʈ������ �ڵ� �������� �ʵ��� ��ũ�� ó���Ѵ�
#ifndef _GMTOOL_
			// 090120 LUJ, ��ġ ������ ���ʷ� ������
			{
				const char* const	versionFile = "LunaVerInfo.ver";
				FILE* const			file		= fopen( versionFile, "r+" );
	
				if( file )
				{
					// 090109 LUJ, 4���ڷ� ������ ���� ����� ���Ͽ��� �о�� ���ڿ� ������ ��´�
					const size_t headerSize = 4;
					char header[ headerSize + 1 ] = { 0 };
					fread(
						header,
						headerSize,
						sizeof( *header ),
						file );

					// 090109 LUJ, ��ü ��ġ�� ������ ���� ��ȣ�� �����
					char versionText[ MAX_PATH ] = { 0 };
					sprintf(
						versionText,
						"%s00000000",
						header );
					// 090109 LUJ, ��ġ ���� ���ڿ��� ���Ͽ� ����
					fseek(
						file,
						0,
						SEEK_SET );
					fwrite(
						versionText,
						sizeof( *versionText ),
						strlen( versionText ),
						file );
					fclose( file );
				}
			}

			// 090120 LUJ, ī���� �� Ŭ���̾�Ʈ�� ����ǵ��� �Ѵ�
			MessageBox_Func(
				MBI_EXIT,
				0,
				MBI_YES );
#endif
		}
		break;
	}
}

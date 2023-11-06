#include "stdafx.h"
#include ".\autonotemanager.h"
#include "ObjectManager.h"
#include "Player.h"
#include "ChatManager.h"
#include "GameIn.h"
#include "AutoAnswerDlg.h"
#include "AutoNoteDlg.h"

GLOBALTON(CAutoNoteManager);

CAutoNoteManager::CAutoNoteManager(void)
{
}

CAutoNoteManager::~CAutoNoteManager(void)
{
}

void CAutoNoteManager::ToggleAutoNote()
{
	if( GAMEIN->GetAutoNoteDlg()->IsActive() )
		GAMEIN->GetAutoNoteDlg()->SetActive(FALSE);
	else
		GAMEIN->GetAutoNoteDlg()->SetActive(TRUE);
}

//---���� ������ ����
void CAutoNoteManager::AskToAutoUser( DWORD dwAutoPlayerIdx, DWORD dwQuestion )
{
	MSG_DWORD2 msg;
	msg.Category	= MP_AUTONOTE;
	msg.Protocol	= MP_AUTONOTE_ASKTOAUTO_SYN;
	msg.dwObjectID	= HEROID;
	msg.dwData1		= dwAutoPlayerIdx;
	msg.dwData2		= dwQuestion;	//---���

	NETWORK->Send(&msg, sizeof(msg));
}

//---���� �亯�� ����
void CAutoNoteManager::AnswerToQuestion( DWORD dwData1, DWORD dwData2, DWORD dwData3, DWORD dwData4 )
{
	MSG_DWORD4 msg;
	msg.Category	= MP_AUTONOTE;
	msg.Protocol	= MP_AUTONOTE_ANSWER_SYN;
	msg.dwObjectID	= HEROID;
/*
	msg.dwData1	 = AUTONOTEMGR->GetAnswerKey() + dwData1;
	msg.dwData2	 = AUTONOTEMGR->GetAnswerKey() + dwData2 - dwData1;
	msg.dwData3	 = AUTONOTEMGR->GetAnswerKey() + dwData3 - dwData2;
	msg.dwData4	 = AUTONOTEMGR->GetAnswerKey() + dwData4 - dwData3;
*/
	msg.dwData1	 = dwData1;
	msg.dwData2	 = dwData2;
	msg.dwData3	 = dwData3;
	msg.dwData4	 = dwData4;

	NETWORK->Send(&msg, sizeof(msg));
}

void CAutoNoteManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{
	case MP_AUTONOTE_ASKTOAUTO_ACK:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1398 ) ); //"[�����Ʈ]������ ��󿡰� ������ �����Ͽ����ϴ�." );
		}
		break;

	case MP_AUTONOTE_ASKTOAUTO_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			switch( pmsg->dwData )
			{
			case eAutoNoteError_CantFind:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1399 ) ); //"[�����Ʈ]������ ����� ã�� �� �����ϴ�." );
				break;
			case eAutoNoteError_NotProperState:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1400 ) ); //"[�����Ʈ]������ ����� �����Ʈ�� ����� �� �ִ� ���°� �ƴմϴ�." );
				break;
			case eAutoNoteError_AlreadyAsked:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1401 ) ); //"[�����Ʈ]������ ����� �����Ʈ ������ Ǯ�� �ִ� ���Դϴ�." );
				break;
			case eAutoNoteError_AlreadyAsking:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1402 ) ); //"[�����Ʈ]���� �̹� �ٸ� ��󿡰� �����Ʈ�� ��� ���Դϴ�." );
				break;
			case eAutoNoteError_CantUseMap:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1418 ) ); //"[�����Ʈ]�����Ʈ�� ����� �� ���� �����Դϴ�." );
				break;
			default:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1403 ) ); //"[�����Ʈ]�����Ʈ ��뿡 �����Ͽ����ϴ�." );
				break;
			};
		}
		break;
/*	
	case MP_AUTONOTE_ASKTOAUTO:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

			DWORD dwKey = ( pmsg->dwData1 + pmsg->dwData2 + pmsg->dwData3 - HEROID ) / 4;	//---���Ƽ� �� �޼������� key���� ���Ѵ�.

			DWORD dwUnitKey[8];							//---key������ �� �ڸ��� ���ڸ� �����Ѵ�.
			int i;
			for( i = 0 ; i < 8 ; ++i )
			{
				dwUnitKey[i] = dwKey % 10;
				dwKey /= 10;
			}

			int n = 0;
			for( i = 0 ; i < 4 ; ++i )
			{
				n += dwUnitKey[i]+1;
				m_dwAskColor[i] = dwUnitKey[ n % 8 ];	//--key���� ���� ������ ���Ѵ�.
			}

			for( i = 0 ; i < 4 ; ++i )					//--key���� ���� Į�����̺��� ���´�.
				m_dwColorTable[i] = i;

			int swap = m_dwColorTable[dwUnitKey[3]];	m_dwColorTable[dwUnitKey[3]] = m_dwColorTable[dwUnitKey[2]];	m_dwColorTable[dwUnitKey[2]] = swap;
			swap = m_dwColorTable[dwUnitKey[2]];		m_dwColorTable[dwUnitKey[2]] = m_dwColorTable[dwUnitKey[1]];	m_dwColorTable[dwUnitKey[1]] = swap;
			swap = m_dwColorTable[dwUnitKey[1]];		m_dwColorTable[dwUnitKey[1]] = m_dwColorTable[dwUnitKey[0]];	m_dwColorTable[dwUnitKey[0]] = swap;

			GAMEIN->GetAutoAnswerDlg()->Shuffle( m_dwColorTable );

			//---�����ۼ�
			char buf[128];
			wsprintf(  buf, "%s %s %s %s", 
				CHATMGR->GetChatMsg( 1394 + m_dwAskColor[0] ),			//1394������ ����,���,�Ķ�,����
				CHATMGR->GetChatMsg( 1394 + m_dwAskColor[1] ),
				CHATMGR->GetChatMsg( 1394 + m_dwAskColor[2] ),
				CHATMGR->GetChatMsg( 1394 + m_dwAskColor[3] ) );

			GAMEIN->GetAutoAnswerDlg()->SetQuestion( buf );
			GAMEIN->GetAutoAnswerDlg()->SetActive( TRUE );
			
			m_dwAnswerKey = dwUnitKey[7]*100 + dwUnitKey[6]*10 + dwUnitKey[5] * 2;
			m_dwAnswerKey += dwUnitKey[6]*100 + dwUnitKey[5]*10 + dwUnitKey[4] * 3;	//�⺻Ű
		}
		break;
*/
	case MP_AUTONOTE_ANSWER_NACK:	//��������-�亯�� Ʋ�ȴ�
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1404 ), pmsg->dwData ); //"[�����Ʈ]���� Ʋ�Ƚ��ϴ�. %d���� ��ȸ�� ���ҽ��ϴ�.", pmsg->dwData );

			if( GAMEIN->GetAutoAnswerDlg() )
				GAMEIN->GetAutoAnswerDlg()->Retry();
		}
		break;

	case MP_AUTONOTE_NOTAUTO:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1405 ) ); //"[�����Ʈ]������ ����� �亯�� ���߾����ϴ�." );
		}
		break;

	case MP_AUTONOTE_ANSWER_ACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1406 ) ); //"[�����Ʈ]�亯�� ���߼̽��ϴ�." );

			if( GAMEIN->GetAutoAnswerDlg() )
				GAMEIN->GetAutoAnswerDlg()->SetActive( FALSE );
		}
		break;

	case MP_AUTONOTE_ANSWER_FAIL:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1407 ) ); //"[�����Ʈ]�亯�� Ʋ�� ������ �����ϴ�." );

			if( GAMEIN->GetAutoAnswerDlg() )
				GAMEIN->GetAutoAnswerDlg()->SetActive( FALSE );
		}
		break;

	case MP_AUTONOTE_ANSWER_TIMEOUT:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1408 ) ); //"[�����Ʈ]�亯 ���ѽð��� ���� ������ �����ϴ�." );

			if( GAMEIN->GetAutoAnswerDlg() )
				GAMEIN->GetAutoAnswerDlg()->SetActive( FALSE );
		}
		break;

	case MP_AUTONOTE_KILLAUTO:		//---��� �����
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1409 ) ); //"[�����Ʈ]������ ����� �亯�� ������ ���Ͽ� ������ ����Ǿ����ϴ�." );
		}
		break;

	case MP_AUTONOTE_PUNISH:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			
			char buf[256] = {0};
			char temp[64];
			DWORD milisec	= pmsg->dwData;
			DWORD hour		= milisec / 60 / 60 / 1000;		milisec -= hour * 60 * 60 * 1000;
			DWORD min		= milisec / 60 / 1000;			milisec -= min * 60 * 1000;			
			DWORD sec		= milisec / 1000;

			if( hour ) { sprintf( temp, CHATMGR->GetChatMsg( 1410 )/*"%d�ð� "*/, hour ); strcat( buf, temp ); }
			if( min )  { sprintf( temp, CHATMGR->GetChatMsg( 1411 )/*"%d�� "*/, min ); strcat( buf, temp ); }
			sprintf( temp, CHATMGR->GetChatMsg( 1412 )/*"%d��"*/, sec ); strcat( buf, temp );

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1413 ), buf ); //"[�����Ʈ]%s �Ŀ� ����� �� �ֽ��ϴ�.", buf );
		}
		break;

	case MP_AUTONOTE_LIST_ADD:
		{
			MSG_AUTOLIST_ADD* pmsg = (MSG_AUTOLIST_ADD*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1414 ) ); //"[�����Ʈ]�����Ʈ�� �̸��� �߰��Ǿ����ϴ�." );
			
			if( GAMEIN->GetAutoNoteDlg() )
				GAMEIN->GetAutoNoteDlg()->AddAutoList( pmsg->Name, pmsg->Date );
		}
		break;
		
	case MP_AUTONOTE_LIST_ALL:
		{
			MSG_AUTOLIST_ALL* pmsg = (MSG_AUTOLIST_ALL*)pMsg;

//			CHATMGR->AddMsg( CTC_SYSMSG, "[�����Ʈ]�����Ʈ ����� �޾ƿԽ��ϴ�." );

			int nCount = pmsg->nCount;

			if( GAMEIN->GetAutoNoteDlg() )
			{
				for( int i = 0 ; i < nCount ; ++i )
				{
					GAMEIN->GetAutoNoteDlg()->AddAutoList( pmsg->row[i].Name, pmsg->row[i].Date );
				}
			}
		}
		break;

	case MP_AUTONOTE_ASKTOAUTO_IMAGE:
		{
			MSG_AUTONOTE_IMAGE* pmsg = (MSG_AUTONOTE_IMAGE*)pMsg;

			if( GAMEIN->GetAutoAnswerDlg() )
			{
				GAMEIN->GetAutoAnswerDlg()->Shuffle( m_dwColorTable );	//��ġ�� �������� ����ش�. (��ư��ġ ���°� �ּ�ó���Ǿ���)
				GAMEIN->GetAutoAnswerDlg()->SaveImage( pmsg->Image );
				GAMEIN->GetAutoAnswerDlg()->SetQuestion( "" );
				GAMEIN->GetAutoAnswerDlg()->SetActive( TRUE );
			}
		}
		break;
	}
}

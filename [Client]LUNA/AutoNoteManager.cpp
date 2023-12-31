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

//---오토 적발을 위해
void CAutoNoteManager::AskToAutoUser( DWORD dwAutoPlayerIdx, DWORD dwQuestion )
{
	MSG_DWORD2 msg;
	msg.Category	= MP_AUTONOTE;
	msg.Protocol	= MP_AUTONOTE_ASKTOAUTO_SYN;
	msg.dwObjectID	= HEROID;
	msg.dwData1		= dwAutoPlayerIdx;
	msg.dwData2		= dwQuestion;	//---허당

	NETWORK->Send(&msg, sizeof(msg));
}

//---오토 답변을 위해
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

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1398 ) ); //"[오토노트]선택한 대상에게 문제를 전송하였습니다." );
		}
		break;

	case MP_AUTONOTE_ASKTOAUTO_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			switch( pmsg->dwData )
			{
			case eAutoNoteError_CantFind:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1399 ) ); //"[오토노트]선택한 대상을 찾을 수 없습니다." );
				break;
			case eAutoNoteError_NotProperState:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1400 ) ); //"[오토노트]선택한 대상은 오토노트를 사용할 수 있는 상태가 아닙니다." );
				break;
			case eAutoNoteError_AlreadyAsked:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1401 ) ); //"[오토노트]선택한 대상은 오토노트 문제를 풀고 있는 중입니다." );
				break;
			case eAutoNoteError_AlreadyAsking:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1402 ) ); //"[오토노트]현재 이미 다른 대상에게 오토노트를 사용 중입니다." );
				break;
			case eAutoNoteError_CantUseMap:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1418 ) ); //"[오토노트]오토노트를 사용할 수 없는 지역입니다." );
				break;
			default:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1403 ) ); //"[오토노트]오토노트 사용에 실패하였습니다." );
				break;
			};
		}
		break;
/*	
	case MP_AUTONOTE_ASKTOAUTO:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

			DWORD dwKey = ( pmsg->dwData1 + pmsg->dwData2 + pmsg->dwData3 - HEROID ) / 4;	//---꼬아서 온 메세지에서 key값을 구한다.

			DWORD dwUnitKey[8];							//---key값에서 각 자리의 숫자를 추출한다.
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
				m_dwAskColor[i] = dwUnitKey[ n % 8 ];	//--key값을 통해 문제를 구한다.
			}

			for( i = 0 ; i < 4 ; ++i )					//--key값을 통해 칼라테이블을 섞는다.
				m_dwColorTable[i] = i;

			int swap = m_dwColorTable[dwUnitKey[3]];	m_dwColorTable[dwUnitKey[3]] = m_dwColorTable[dwUnitKey[2]];	m_dwColorTable[dwUnitKey[2]] = swap;
			swap = m_dwColorTable[dwUnitKey[2]];		m_dwColorTable[dwUnitKey[2]] = m_dwColorTable[dwUnitKey[1]];	m_dwColorTable[dwUnitKey[1]] = swap;
			swap = m_dwColorTable[dwUnitKey[1]];		m_dwColorTable[dwUnitKey[1]] = m_dwColorTable[dwUnitKey[0]];	m_dwColorTable[dwUnitKey[0]] = swap;

			GAMEIN->GetAutoAnswerDlg()->Shuffle( m_dwColorTable );

			//---문제작성
			char buf[128];
			wsprintf(  buf, "%s %s %s %s", 
				CHATMGR->GetChatMsg( 1394 + m_dwAskColor[0] ),			//1394번부터 빨강,노랑,파랑,검정
				CHATMGR->GetChatMsg( 1394 + m_dwAskColor[1] ),
				CHATMGR->GetChatMsg( 1394 + m_dwAskColor[2] ),
				CHATMGR->GetChatMsg( 1394 + m_dwAskColor[3] ) );

			GAMEIN->GetAutoAnswerDlg()->SetQuestion( buf );
			GAMEIN->GetAutoAnswerDlg()->SetActive( TRUE );
			
			m_dwAnswerKey = dwUnitKey[7]*100 + dwUnitKey[6]*10 + dwUnitKey[5] * 2;
			m_dwAnswerKey += dwUnitKey[6]*100 + dwUnitKey[5]*10 + dwUnitKey[4] * 3;	//기본키
		}
		break;
*/
	case MP_AUTONOTE_ANSWER_NACK:	//오토유저-답변이 틀렸다
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1404 ), pmsg->dwData ); //"[오토노트]답이 틀렸습니다. %d번의 기회가 남았습니다.", pmsg->dwData );

			if( GAMEIN->GetAutoAnswerDlg() )
				GAMEIN->GetAutoAnswerDlg()->Retry();
		}
		break;

	case MP_AUTONOTE_NOTAUTO:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1405 ) ); //"[오토노트]선택한 대상이 답변을 맞추었습니다." );
		}
		break;

	case MP_AUTONOTE_ANSWER_ACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1406 ) ); //"[오토노트]답변을 맞추셨습니다." );

			if( GAMEIN->GetAutoAnswerDlg() )
				GAMEIN->GetAutoAnswerDlg()->SetActive( FALSE );
		}
		break;

	case MP_AUTONOTE_ANSWER_FAIL:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1407 ) ); //"[오토노트]답변이 틀려 접속을 끊습니다." );

			if( GAMEIN->GetAutoAnswerDlg() )
				GAMEIN->GetAutoAnswerDlg()->SetActive( FALSE );
		}
		break;

	case MP_AUTONOTE_ANSWER_TIMEOUT:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1408 ) ); //"[오토노트]답변 제한시간이 지나 접속을 끊습니다." );

			if( GAMEIN->GetAutoAnswerDlg() )
				GAMEIN->GetAutoAnswerDlg()->SetActive( FALSE );
		}
		break;

	case MP_AUTONOTE_KILLAUTO:		//---상대 제재됨
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1409 ) ); //"[오토노트]선택한 대상이 답변을 맞추지 못하여 접속이 종료되었습니다." );
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

			if( hour ) { sprintf( temp, CHATMGR->GetChatMsg( 1410 )/*"%d시간 "*/, hour ); strcat( buf, temp ); }
			if( min )  { sprintf( temp, CHATMGR->GetChatMsg( 1411 )/*"%d분 "*/, min ); strcat( buf, temp ); }
			sprintf( temp, CHATMGR->GetChatMsg( 1412 )/*"%d초"*/, sec ); strcat( buf, temp );

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1413 ), buf ); //"[오토노트]%s 후에 사용할 수 있습니다.", buf );
		}
		break;

	case MP_AUTONOTE_LIST_ADD:
		{
			MSG_AUTOLIST_ADD* pmsg = (MSG_AUTOLIST_ADD*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1414 ) ); //"[오토노트]오토노트에 이름이 추가되었습니다." );
			
			if( GAMEIN->GetAutoNoteDlg() )
				GAMEIN->GetAutoNoteDlg()->AddAutoList( pmsg->Name, pmsg->Date );
		}
		break;
		
	case MP_AUTONOTE_LIST_ALL:
		{
			MSG_AUTOLIST_ALL* pmsg = (MSG_AUTOLIST_ALL*)pMsg;

//			CHATMGR->AddMsg( CTC_SYSMSG, "[오토노트]오토노트 목록을 받아왔습니다." );

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
				GAMEIN->GetAutoAnswerDlg()->Shuffle( m_dwColorTable );	//위치만 랜덤으로 잡아준다. (버튼위치 섞는것 주석처리되었음)
				GAMEIN->GetAutoAnswerDlg()->SaveImage( pmsg->Image );
				GAMEIN->GetAutoAnswerDlg()->SetQuestion( "" );
				GAMEIN->GetAutoAnswerDlg()->SetActive( TRUE );
			}
		}
		break;
	}
}

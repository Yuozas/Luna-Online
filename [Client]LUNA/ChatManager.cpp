#include "stdafx.h"
#include ".\ChatManager.h"
#include "MHFile.h"
#include "Interface/cResourceDef.h"
#include "ChattingDlg.h"

#ifdef _CHEATENABLE_			
#include "CheatMsgParser.h"
#endif	//_CHEATENABLE_		

#include "ChatManager.h"
#include "MacroManager.h"
#include "ObjectManager.h"
#include "PartyManager.h"
#include "GuildManager.h"
#include "GameIn.h"
#include "GameResourceManager.h"
#include "cIMEex.h"
#include "cMsgBox.h"
#include "Input/cIMEWnd.h"

#include "Interface/cWindowManager.h"
#include "Interface/cEditBox.h"
#include "Interface/cListDialog.h"
#include "Interface/cTextArea.h"

// 070106 LYW --- Include option manager.
#include "OptionManager.h"
#include "FilteringTable.h"


#ifdef _GMTOOL_
#include "GMToolManager.h"
#include "MainGame.h"
#endif

#include "DateMatchingDlg.h"
#include "cMonsterSpeechManager.h"
#include "GMNotifyManager.h"

// 080410 LYW --- ChatManager : Include header file for chatroom system.
#include "./ChatRoomMgr.h"
#include "./ChatRoomDlg.h"


GLOBALTON(CChatManager)


CChatManager::CChatManager(void)
{
	ZeroMemory( m_pChatMsg, sizeof(m_pChatMsg) ) ;											// 채팅 메시지를 초기화 한다.

	ZeroMemory( &m_ChatOption, sizeof( m_ChatOption ) ) ;									// 채팅 옵션을 초기화 한다.

	m_dwLastChatTime = 0;																	// 마지막 채팅 타임을 초기화 한다.

	// 070928 LYW --- ChatManager : Initialize whisper name.
	InitializeWhisperName() ;
}


CChatManager::~CChatManager(void)
{
	// 070420 LYW --- ChatManager : Release m_pChatMsg.
	Release() ;																				// 해제 함수를 호출한다.

	//// 070608 LYW --- ChatManager : Delete emotion filter.
	//// 070420 LYW --- ChatManager : Release m_EnotionList.
	//ReleaseEmotionList() ;																// 이모션 파트를 해제 한다.
}


void	CChatManager::Init()
{
	LoadChatMsg();																			// 채팅 메시지를 로딩한다.

	LoadFaceCommandList() ;																	// 얼굴 교체 명령을 로딩한다.

	// 070910 LYW --- ChatManager : Add function to load action command.
	LoadActionCommandList() ;																// 액션 명령을 로딩한다.

	//// 070608 LYW --- ChatManager : Delete emotion filter.
	//LoadEmotionFilter() ;																	// 이모션 필터를 로딩한다.
}


void	CChatManager::Release()
{
	for( int i = 0 ; i < MAX_CONVERSATION_MSGNUM ; ++i )									// 최대 대화 메시지 수 만큼, 포문을 돌면서,
	{
		SAFE_DELETE_ARRAY( m_pChatMsg[ i ] );												// 채팅 메시지를 안전하게 삭제한다.
	}

	PTRLISTPOS pos = m_FaceCommandList.GetHeadPosition() ;									// 얼굴 교체 명령어 리스트에서 위치 포지션을 받아온다.

	sFACEINFO* pFaceInfo = NULL ;															// 얼굴 정보 포인터를 선언한다.

	while( pFaceInfo = (sFACEINFO*)m_FaceCommandList.GetNext( pos ) )						// 리스트에서 위치에 따른 얼굴 정보를 받아온다.
	{
		if( pFaceInfo )																		// 얼굴 정보가 유효하다면,
		{
			SAFE_DELETE( pFaceInfo );														// 정보를 안전하게 삭제한다.
		}
	}

	m_FaceCommandList.RemoveAll() ;															// 얼굴 변경 명령 리스트를 모두 비운다.

	sACTION_COMMAND* pDeleteCommand ;

	PTRLISTPOS deletePos = NULL ;
	deletePos = m_ActionCommandList.GetHeadPosition() ;

	while( deletePos )																	// 파일의 끝까지 while을 돌린다.
	{
		pDeleteCommand = NULL ;
		pDeleteCommand = (sACTION_COMMAND*)m_ActionCommandList.GetNext(deletePos) ;

		if( !pDeleteCommand ) continue ;

		m_ActionCommandList.Remove(pDeleteCommand) ;

		delete pDeleteCommand ;
	}

	m_ActionCommandList.RemoveAll() ;
}


void	CChatManager::LoadChatMsg()
{
	CMHFile fp;																				// 묵향파일을 선언한다.

	if( !fp.Init( FILE_CHAT_MSG, "rb" ) )													// 채팅 메시지 파일을 읽는것이 실패하면,
	{
		return;																				// 리턴 처리를 한다.
	}

	char msg[1024] = {0,};																	// 임시 메시지 버퍼틀 선언한다.

	while( TRUE )																			// whiel문을 돌린다.
	{
		const int nIndex = fp.GetInt();														// 인덱스를 읽어들인다.

		if( fp.IsEOF() ||																	// 파일의 끝이거나,
			nIndex < 0 ||																	// 인덱스타 0보가 크거나,
			nIndex >= MAX_CONVERSATION_MSGNUM )												// 인덱스가 최대 메시지 수를 넘으면,
		{
			break;																			// while문을 벗어난다.
		}

		fp.GetStringInQuotation(msg);														// msg로 스트링을 읽어 들인다.

		const int nLen = strlen( msg );														// 메시지의 길이를 구한다.

		if( nLen > 0 )																		// 메시지가 유효하다면,
		{
			m_pChatMsg[nIndex] = new char[nLen+1];											// 메시지 길이만큼 메모리를 할당한다.
			memcpy( m_pChatMsg[nIndex], msg, nLen+1 );										// 메시지 인덱스 포인터에, 메시지를 복사한다.
			fp.GetLine(msg, 256);															// 라인을 읽어들인다.
		}
	}
}


char*	CChatManager::GetChatMsg( int nMsgNum )
{
	ASSERT( -1 < nMsgNum && MAX_CONVERSATION_MSGNUM > nMsgNum );							// 메시지 개수 체크를 한다.

	char* message = m_pChatMsg[ nMsgNum ];													// 들어온 메시지 번호로 메시지 포인터를 넘겨 받는다.

	if( message )																			// 메시지가 유효한지 체크한다.
	{
		return message;																		// 메시지를 리턴한다.
	}
	else																					// 메시지가 유효하지 않다면,
	{
		return "";																			// 빈 메시지를 리턴한다.
	}
}

void	CChatManager::AddMsg( int nClass, char* str, ... )
{
	if( ! m_pChattingDlg || str == NULL )													// 채팅다이얼 로그가 유효하지 않거나, 메시지가 유효하지 않으면,
	{
		return;																				// 리턴처리를 한다.
	}

	char msg[256];																			// 임시 메시지 버퍼를 선언한다.

	va_list argList;																		// 가변인수를 읽기 위한 포인터 변수를 선언한다.

	va_start(argList, str);																	// 포인터 변수가 첫 번째 가변 인수를 가리키도록 초기화한다.

	vsprintf(msg,str,argList);																// 포인터를 이용해, 포멧된 아웃풋으로 msg에 쓴다.

	va_end(argList);																		// 가변인수를 다 읽은 후 뒷정리를 한다.

	DWORD dwColor = dwChatTextColor[ nClass ];												// 클래스에 따른 색상을 지정한다.

	switch( nClass )																		// 클래스를 체크한다.
	{
	// 080313 NYJ --- 전체외치기 추가
	case CTC_SHOUT:
		{
			m_pChattingDlg->AddShoutMsg(msg, dwColor);										// 외치기 메세지를 출력한다.
		}
		break;

	case CTC_OPERATOR:																		// 운영자 메세지라면,
		{
			m_pChattingDlg->AddMsg( e_CONVERSATION_SYSTEM, msg, dwColor ) ;					// 시스템 창에 메시지를 출력한다.
			NOTIFYMGR->AddMsg( msg );														// 공지로 메시지를 출력한다.
		}
		break;

	case CTC_OPERATOR2:																		// 운영자 메세지라면,
		{
			NOTIFYMGR->AddMsg( msg );														// 공지로 메시지를 출력한다.
		}
		break;

	case CTC_BILLING :																		// 빌링 메시지라면,
		{
			m_pChattingDlg->AddMsg( e_CONVERSATION_SYSTEM, msg, dwColor ) ;					// 시스템 창에 메시지를 출력한다.
			NOTIFYMGR->AddMsg( msg, eNTC_REMAINTIME );										// 공지로 메시지를 출력한다.
		}
		break ;

	case CTC_GMCHAT :																		// GM 채팅이라면,
		{
			m_pChattingDlg->AddMsg( e_CONVERSATION_SYSTEM, msg, dwColor ) ;					// 메시지를 출력한다.
		}
		break ;

	case CTC_GENERALCHAT :																	// 일반 채팅이면,
		{
			if( !m_ChatOption.bOption[CTO_NOCHATMSG] )										// 채팅 옵션의 일반채팅 끄기를 체크한다.
			{
				m_pChattingDlg->AddMsg( e_CONVERSATION_ALL, msg, dwColor ) ;				// 일반 채팅을 출력한다.
			}
		}
		break ;

	case CTC_PARTYCHAT :																	// 파티 채팅이면,
		{
			if( !m_ChatOption.bOption[CTO_NOPARTYMSG] )										// 채팅 옵션의 파티채팅 끄기를 체크한다.
			{
				m_pChattingDlg->AddMsg( e_CONVERSATION_PARTY, msg, dwColor ) ;				// 파티 채팅을 출력한다.
			}
		}
		break ;

	case CTC_FAMILY :																		// 패밀리 채팅이면,
		{
			if( !m_ChatOption.bOption[CTO_NOFAMILYMSG] )										// 채팅 옵션의 패밀리채팅 끄기를 체크한다.
			{
				m_pChattingDlg->AddMsg( e_CONVERSATION_FAMILY, msg, dwColor ) ;				// 패밀리 채팅을 출력한다.
			}
		}
		break ;

	case CTC_GUILDCHAT :																	// 길드 채팅이면,
		{
			if( !m_ChatOption.bOption[CTO_NOGUILDMSG] )										// 채팅 옵션의 길드채팅 끄기를 체크한다.
			{
				m_pChattingDlg->AddMsg( e_CONVERSATION_GUILD, msg, dwColor ) ;				// 길드 채팅을 출력한다.
			}
		}
		break ;

	case CTC_ALLIANCE :																		// 동맹 채팅이면,
		{
			if( !m_ChatOption.bOption[CTO_NOALLIANCEMSG] )									// 채팅 옵션의 동맹 채팅 끄기를 체크한다.
			{
				m_pChattingDlg->AddMsg( e_CONVERSATION_ALLIANCE, msg, dwColor ) ;			// 동맹 체팅을 끈다.
			}
		}
		break ;

	case CTC_TRADE :																		// 거래 채팅이면,
		{
			if( !m_ChatOption.bOption[CTO_NOSHOUTMSG] )										// 채팅 옵션의 외치기 끄기를 체크한다.
			{
				m_pChattingDlg->AddMsg( e_CONVERSATION_TRADE, msg, dwColor ) ;				// 외치기 채팅을 출력한다.
			}
		}
		break ;

	case CTC_SYSMSG :																		// 시스템 메시지라면,
		{
			if( !m_ChatOption.bOption[CTO_NOSYSMSG] )										// 채팅 옵션의 시스템 메시지 끄기를 확인한다.
			{
				m_pChattingDlg->AddMsg( e_CONVERSATION_SYSTEM, msg, dwColor ) ;				// 시스템 메시지를 출력한다.
			}
		}
		break ;

	case CTC_WHISPER :																		// 귓속말 메시지라면,
		{
			m_pChattingDlg->AddMsg( e_CONVERSATION_SYSTEM, msg, dwColor ) ;					// 귓속말을 출력한다.
		}
		break ;

	case CTC_ATTACK :																		// 공격 메시지라면,
		{
			if( !m_ChatOption.bOption[CTO_NOSYSMSG] )										// 채팅 옵션에서 시스템 메시지 끄기를 체크한다.
			{
				m_pChattingDlg->AddMsg( e_CONVERSATION_ALL, msg, dwColor ) ;				// 공격 메시지를 출력한다.
			}
		}
		break ;

	case CTC_ATTACKED :																		// 상대가 공격한 메시지라면,
		{
			if( !m_ChatOption.bOption[CTO_NOSYSMSG] )										// 채팅 옵션에서 시스템 메시지 끄기를 체크한다.
			{
				m_pChattingDlg->AddMsg( e_CONVERSATION_ALL, msg, dwColor ) ;				// 상대가 공격한 메시지를 출력한다.
			}
		}
		break ;

	case CTC_DEFENCE :																		// 방어 메시지라면,
		{
			if( !m_ChatOption.bOption[CTO_NOSYSMSG] )										// 채팅 옵션에서 시스템 메시지 끄기를 체크한다.
			{
				m_pChattingDlg->AddMsg( e_CONVERSATION_ALL, msg, dwColor ) ;				// 방어 메시지를 출력한다.
			}
		}
		break ;

	case CTC_DEFENCED :																		// 상대가 방어한 메시지라면,
		{
			if( !m_ChatOption.bOption[CTO_NOSYSMSG] )										// 채팅 옵션에서 시스템 메시지 끄기를 체크한다.
			{
				m_pChattingDlg->AddMsg( e_CONVERSATION_ALL, msg, dwColor ) ;				// 상대가 방어한 메시지를 출력한다.
			}
		}
		break ;

	case CTC_KILLED :																		// 죽었을 때 메시지 라면,
		{
			if( !m_ChatOption.bOption[CTO_NOSYSMSG] )										// 채팅 옵션에서 시스템 메시지 끄기를 체크한다.
			{
				m_pChattingDlg->AddMsg( e_CONVERSATION_ALL, msg, dwColor ) ;				// 죽었을 때 메시지를 출력한다.
			}
		}
		break ;

	case CTC_MPWARNING :																	// MP 경고,
		{
			if( !m_ChatOption.bOption[CTO_NOSYSMSG] )										// 채팅 옵션에서 시스템 메시지 끄기를 체크한다.
			{
				m_pChattingDlg->AddMsg( e_CONVERSATION_ALL, msg, dwColor ) ;				// 마나 부족 경고를 출력한다.
			}
		}
		break ;

	case CTC_HPWARNING :																	// HP 경고,
		{
			if( !m_ChatOption.bOption[CTO_NOSYSMSG] )										// 채팅 옵션에서 시스템 메시지 끄기를 체크한다.
			{
				m_pChattingDlg->AddMsg( e_CONVERSATION_ALL, msg, dwColor ) ;				// 생명력 부족 경고를 출력한다.
			}
		}

	case CTC_GETITEM :																		// 아이템 파트 메시지라면,
		{
			if( !m_ChatOption.bOption[CTO_NOITEMMSG] )										// 채팅 옵션의 돈/아이템 획득메시지 끄기를 확인한다.
			{
				m_pChattingDlg->AddMsg( e_CONVERSATION_ALL, msg, dwColor ) ;				// 아이템 메시지를 출력한다.
			}
		}
		break ;

	case CTC_GETEXP :																		// 경험치 파트 메시지라면,
		{
			if( !m_ChatOption.bOption[CTO_NOEXPMSG] )										// 채팅 옶션의 경험치/수련치 획득 메시지 끄기를 확인한다.
			{
				m_pChattingDlg->AddMsg( e_CONVERSATION_ALL, msg, dwColor ) ;				// 경험치 메시지를 출력한다.
			}
		}
		break ;

	case CTC_TOWHOLE :																		// 외치기를 했다면,
		{
			m_pChattingDlg->AddMsg( e_CONVERSATION_ALL, msg, dwColor ) ;					// 메시지를 출력한다.
		}
		break ;

	case CTC_TOPARTY :																		// 파티에게 메시지를 보낸다면,
		{
			m_pChattingDlg->AddMsg( e_CONVERSATION_ALL, msg, dwColor ) ;					// 메시지를 출력한다.
		}
		break ;

	case CTC_TOFAMILY :																		// 패밀리에게 메시지를 보낸다면,
		{
			m_pChattingDlg->AddMsg( e_CONVERSATION_ALL, msg, dwColor ) ;					// 메시지를 출력한다.
		}
		break ;

	case CTC_TOGUILD :																		// 길드에게 메시지를 보낸가면,
		{
			m_pChattingDlg->AddMsg( e_CONVERSATION_ALL, msg, dwColor ) ;					// 메시지를 출력한다.
		}
		break ;

	case CTC_MONSTERSPEECH :																// 몬스터 말하기 메시지라면,
		{
			m_pChattingDlg->AddMsg( e_CONVERSATION_ALL, msg, dwColor ) ;					// 메시지를 출력한다.
		}
		break ;

	case CTC_MONSTERSHOUT :																	// 몬스터 외치기라면,
		{
			m_pChattingDlg->AddMsg( e_CONVERSATION_ALL, msg, dwColor ) ;					// 메시지를 출력한다.
		}
		break ;

	default :																				// 지금까지의 경우 외에,
		{
			m_pChattingDlg->AddMsg( e_CONVERSATION_SYSTEM, msg, dwColor ) ;					// 메시지를 출력한다.
		}
		break ;
	}
}

//// 070608 LYW --- ChatManager : Delete emotion filter.
//void	CChatManager::LoadEmotionFilter()
//{
//	CMHFile	fp ;
//
//	BOOL bResult = FALSE ;
//
//	bResult = fp.Init( FILE_CHATFILTER_MSG, "rb" ) ;
//
//	if( !bResult )
//	{
//		char buff[256];
//		sprintf(buff, "Loading failed!! ./data/interface/windows/Chat_Filter.bin");
//		LOGEX(buff, PT_MESSAGEBOX);
//	}
//
//	int nEmotionNum ;
//
//	while( TRUE )
//	{
//		nEmotionNum = fp.GetInt() ;
//
//		if( fp.IsEOF() ) break ;
//
//		sEMOTION* pEmotion = new sEMOTION ;
//
//		SafeStrCpy( pEmotion->buf, fp.GetString(), MAX_NAME_LENGTH+1 ) ;
//
//		pEmotion->nEmotionNum	= nEmotionNum ;
//		pEmotion->nLen			= strlen( pEmotion->buf ) ;
//
//		m_EmotionList.AddTail( pEmotion ) ;
//	}
//
//	fp.Release() ;
//}

//// 070608 LYW --- ChatManager : Delete emotion filter.
//// 070420 LYW --- ChatManager : Release m_EmotionList.
//void	CChatManager::ReleaseEmotionList()
//{
//	PTRLISTPOS pos = m_EmotionList.GetHeadPosition();
//	while( sEMOTION* pEmotion = (sEMOTION*)m_EmotionList.GetNext( pos ) )
//	{
//		delete pEmotion;
//	}
//	m_EmotionList.RemoveAll();
//}

//// 070608 LYW --- ChatManager : Delete emotion filter.
//int		CChatManager::GetEmotionNum( char* str )
//{
//	int returnNum = -1 ;
//	int count = 0 ;
//	int nStrLen = strlen(str) ;
//
//	char* pStr = str ;
//
//	char ch1 = 0 ;
//	char ch2 = 0 ;
//
//	char tempStr[ 256 ] = {0, } ;
//
//	BOOL bReturn = FALSE ;
//
//	for( count = 0 ; count < nStrLen ; ++count )
//	{
//		PTRLISTPOS pos = m_EmotionList.GetHeadPosition() ;
//		
//		while( pos )
//		{
//			sEMOTION* pEmotion = ( sEMOTION* )m_EmotionList.GetNext( pos ) ;
//
//			if( pEmotion )
//			{
//				if( pEmotion->nLen <= nStrLen )
//				{
//					memcpy( &ch1, pStr, sizeof(char) ) ;
//					memcpy( &ch2, pEmotion->buf, sizeof(char) ) ;
//
//					if( ch1 == ch2 )
//					{
//						strncpy( tempStr, pStr, pEmotion->nLen ) ;
//
//						if( strcmp( tempStr, pEmotion->buf ) == 0 )
//						{
//							bReturn		= TRUE ;
//							returnNum	= pEmotion->nEmotionNum ;
//							break ;
//						}
//						else
//						{
//							memset( tempStr, 0, sizeof(tempStr) ) ;
//						}
//					}
//				}
//			}
//			else
//			{
//				bReturn = TRUE ;
//				break ;
//			}
//		}
//
//		if( bReturn ) break ;
//		++pStr ;
//	}
//
//	return returnNum ;
//}

// 070910 LYW --- ChatManager : Add function to load action command.
void CChatManager::LoadActionCommandList()
{
	CMHFile	fp ;																			// 묵향 파일을 선언한다.

	if( ! fp.Init( FILE_ACTION_COMMAND, "rb" ) )												// 얼굴 교체 파일을 읽는데 실패하면,
	{
		char buff[256];																		// 임시 버퍼를 선언한다.
		sprintf(buff, "Loading failed!! ./data/interface/windows/Action_Command.bin");		// 파일 경로를 확인하여 버퍼에 세팅한다.
		ASSERTMSG(0, buff) ;
	}

	int nIndex = 0 ;
	char tempBuf[32] = {0, } ;

	while( ! fp.IsEOF() )																	// 파일의 끝까지 while을 돌린다.
	{
		memset(tempBuf, 0, sizeof(tempBuf)) ;

		nIndex = fp.GetInt() ;	
		strcpy(tempBuf, fp.GetString()) ;

		if( strlen(tempBuf) <= 2 ) continue ;

		sACTION_COMMAND* pCommand = new sACTION_COMMAND ;									// 얼굴 정보 구조체 만큼 메모리를 할당하여 포인터로 넘긴다.

		pCommand->index = nIndex ;															// 인덱스를 읽는다.
		strcpy(pCommand->string, tempBuf) ;													// 스트링을 읽어 Descript에 복사한다.

		m_ActionCommandList.AddTail(pCommand) ;												// 정보를 리스트에 추가한다.
	}
}

void CChatManager::LoadFaceCommandList()
{
	CMHFile	fp ;																			// 묵향 파일을 선언한다.

	if( ! fp.Init( FILE_CHAT_FACELIST, "rb" ) )												// 얼굴 교체 파일을 읽는데 실패하면,
	{
		char buff[256];																		// 임시 버퍼를 선언한다.
		sprintf(buff, "Loading failed!! ./data/interface/windows/Chat_FaceList.bin");		// 파일 경로를 확인하여 버퍼에 세팅한다.
		LOGEX(buff, PT_MESSAGEBOX);															// 로그를 남기고, 메시지 박스를 띄운다.
	}

	while( ! fp.IsEOF() )																	// 파일의 끝까지 while을 돌린다.
	{
		sFACEINFO* pFaceInfo = new sFACEINFO ;												// 얼굴 정보 구조체 만큼 메모리를 할당하여 포인터로 넘긴다.

		pFaceInfo->nIndex = fp.GetInt() ;													// 인덱스를 읽는다.
		strcpy(pFaceInfo->szDescript, fp.GetString()) ;										// 스트링을 읽어 Descript에 복사한다.
		strcpy(pFaceInfo->szCommand, fp.GetStringInQuotation()) ;							// 스트링을 읽어 Command에 복사한다.
		pFaceInfo->nFaceNum = fp.GetInt() ;													// 얼굴 번호을 읽는다.
		pFaceInfo->nLen = strlen(pFaceInfo->szCommand) ;									// 명령어의 길이를 구한다.

		m_FaceCommandList.AddTail(pFaceInfo) ;												// 정보를 리스트에 추가한다.
	}
}

int CChatManager::GetFaceNum( const TCHAR* str ) const
{
	// 문자를 하나씩 증가시켜서 해당 문자가 포함되었는지 찾는다.
	// 문자열에 멀티바이트 문자가 포함되어 있으므로, 증가시킬 때는 멀티바이트 단위로 증가시켜야 한다
	for(	const TCHAR* subString = str;
		*subString; 
		subString = _tcsinc( subString ) )
	{
		for(	PTRLISTPOS pos = m_FaceCommandList.GetHeadPosition();
			pos;
			)
		{
			const sFACEINFO* pFaceInfo = ( sFACEINFO* )m_FaceCommandList.GetNext( pos );
			ASSERT( pFaceInfo );

			if( ! _tcsncmp( subString, pFaceInfo->szCommand, _tcslen( pFaceInfo->szCommand ) ) )
			{
				return pFaceInfo->nFaceNum;
			}
		}
	}

	return -1;
}


void CChatManager::MsgProc( UINT msg, WPARAM wParam )
{
	if(		msg		== WM_KEYDOWN	&& 
		wParam	== VK_RETURN	&&
		!	CIMEWND->IsDocking() )
	{
		cMsgBox* pMsgBox = WINDOWMGR->GetFirstMsgBox();

		if( pMsgBox &&
			pMsgBox->PressDefaultBtn() )
		{
			return;
		}
	}

	if( ! m_pChattingDlg )
	{
		return;
	}

	cEditBox* pEdit = m_pChattingDlg->GetInputBox() ;
	if( pEdit == NULL ) return;

	// 채팅방의 텍스트 입력을 받는다.
	//cTextArea* pChatRoomCtrl = NULL ;
	//pChatRoomCtrl = CHATROOMMGR->GetChatRoomTextArea() ;
	cEditBox* pChatRoomCtrl = NULL ;
	pChatRoomCtrl = CHATROOMMGR->GetChatRoomInputBox() ;

	ASSERT(pChatRoomCtrl) ;

	if(!pChatRoomCtrl)
	{
		CHATROOMMGR->Throw_Error("Failed to receive chatroom textarea!!", __FUNCTION__) ;
		return ;
	}

	switch( msg )
	{
	case WM_CHAR:
	case WM_IME_CHAR:
	case WM_IME_COMPOSITION:
		{
			if( MACROMGR->IsChatMode()	&& !pEdit->IsFocus() && !CIMEWND->IsDocking() && pEdit->GetIME()->IsValidChar((unsigned char*)&wParam) && !pChatRoomCtrl->IsFocus())		
			{
				pEdit->SetFocusEdit( TRUE );
			}
		}
		break;

	case WM_KEYDOWN :
		{
			if( wParam == VK_RETURN )
			{
				// 080410 LYW --- ChatManager : Modified message proc.
				cEditBox*  pInputBox = GAMEIN->GetDateMatchingDlg()->GetChatingDlg()->GetEditBox() ;
				cTextArea* pTextArea = GAMEIN->GetDateMatchingDlg()->GetMyInfoDlg()->GetIntroduce() ;

				// 일반 채팅창 처리 체크.
				if (pInputBox->IsFocus())
				{
					char szText[256] = {0, } ;
					DWORD dwColor = RGB(0, 255, 255) ;

					sprintf(szText, "[%s]:%s", HERO->GetObjectName(), pInputBox->GetEditText()) ;		

					GAMEIN->GetDateMatchingDlg()->GetChatingDlg()->AddMsg(szText, dwColor);
					g_csResidentRegistManager.CLI_SendDateMatchingChatText(pInputBox->GetEditText()) ;
					pInputBox->SetEditText("");
				}
				// 채팅방 쪽 처리 체크.
				else if(pChatRoomCtrl->IsFocus())
				{
					CHATROOMMGR->MsgProc() ;
				}
				// 매칭 쪽 처리 체크.
				else if( ! pTextArea->IsFocus() )
				{
					if( !CIMEWND->IsDocking() )		//다른에디터에 포커스가 없다면.
					{	
						pEdit->SetFocusEdit( TRUE );
					}
					else
					{
						char* strChat = pEdit->GetEditText();

						if( *strChat )
						{
							OnInputChatMsg( strChat );
							pEdit->SetEditText("");
						}

						m_pChattingDlg->SetCommandToInputBox();

						pEdit->SetFocusEdit( FALSE );
					}
				}
			}		
		}
		break ;
	}
}

void CChatManager::OnInputChatMsg( char* str )
{
#ifdef _CHEATENABLE_
	if( !CHEATMGR->IsCheatEnable() )
#endif
		if( FILTERTABLE->FilterChat( str ) )
		{
			AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1225) );
			return;
		}

		char nToken = *str;

		if( CanChatTime( nToken ) )
		{
			m_pChattingDlg->InspectMsg(str);
		}
}

BOOL CChatManager::CanChatTime( char cToken )
{
	if( gCurTime - m_dwLastChatTime < 1000 )	// 1초
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(248) );
		return FALSE;
	}

	//#ifdef _HK_LOCAL_
	//
	//	switch( cToken )
	//	{
	//		case '/':
	//			if( gCurTime - m_dwLastChatDelayTime[eCHATDELAY_WHISPER] < 3000 ) //3초
	//				return FALSE;
	//			m_dwLastChatDelayTime[eCHATDELAY_WHISPER] = gCurTime;
	//			break;
	//		case '@':
	//			if( gCurTime - m_dwLastChatDelayTime[eCHATDELAY_PARTY] < 1000 ) //1초
	//				return FALSE;
	//			m_dwLastChatDelayTime[eCHATDELAY_PARTY] = gCurTime;
	//			break;
	//		case '#':
	//			if( gCurTime - m_dwLastChatDelayTime[eCHATDELAY_GUILD] < 3000 ) //3초
	//				return FALSE;
	//			m_dwLastChatDelayTime[eCHATDELAY_GUILD] = gCurTime;
	//			break;
	//		case '%':
	//			if( gCurTime - m_dwLastChatDelayTime[eCHATDELAY_SHOUT] < 5000 ) //5초
	//				return FALSE;
	//			m_dwLastChatDelayTime[eCHATDELAY_SHOUT] = gCurTime;
	//			break;
	//		case '$':
	//			if( gCurTime - m_dwLastChatDelayTime[eCHATDELAY_UNION] < 5000 ) //5초
	//				return FALSE;
	//			m_dwLastChatDelayTime[eCHATDELAY_UNION] = gCurTime;
	//			break;
	//	}
	//
	//#endif

	m_dwLastChatTime = gCurTime;
	return TRUE;
}

void	CChatManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{
	case MP_CHAT_ALL:
	case MP_CHAT_NORMAL :						Chat_Normal( pMsg ) ;										break ;
	case MP_CHAT_PARTY :						Chat_Party( pMsg ) ;										break ;
	case MP_CHAT_FAMILY :						Chat_Family( pMsg ) ;										break ;
	case MP_CHAT_GUILD :						Chat_Guild( pMsg ) ;										break ;
	case MP_CHAT_GUILDUNION :					Chat_Alliance( pMsg ) ;										break ;
	case MP_CHAT_TRADE :						Chat_Trade( pMsg ) ;										break ;
	case MP_CHAT_SYSTEM :						Chat_System( pMsg ) ;										break ;
	case MP_CHAT_WHOLE :						Chat_Whole( pMsg ) ;										break ;
	case MP_CHAT_WHISPER_GM:
	case MP_CHAT_WHISPER:						Chat_Whisper( pMsg ) ;										break ;
	case MP_CHAT_WHISPER_ACK:					Chat_Whisper_Ack( pMsg ) ;									break ;
	case MP_CHAT_WHISPER_NACK:					Chat_Whisper_Nack( pMsg ) ;									break ;
	case MP_CHAT_MONSTERSPEECH:					Chat_Monster_Speech( pMsg ) ;								break ;
	case MP_CHAT_SHOUT_ACK:						Chat_Shout_Ack(pMsg);										break;
	case MP_CHAT_SHOUT_NACK:					Chat_Shout_Nack(pMsg);										break;
//---KES CHEAT
	case MP_CHAT_FROMMONSTER_ALL:
		{
			TESTMSG* data = (TESTMSG*)pMsg;

			CObject* pObject = OBJECTMGR->GetObject(data->dwObjectID);
			if( !pObject ) return;

			OBJECTMGR->ShowChatBalloon( pObject, data->Msg );

		}
		break;
//------------
	}
}

void	CChatManager::Chat_Normal( void* pMsg )
{
	TESTMSG* data = (TESTMSG*)pMsg;

	CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( data->dwObjectID );
	if( !pPlayer ) return;

	AddMsg( CTC_GENERALCHAT, "[%s]: %s", pPlayer->GetObjectName(), data->Msg );

	if( !OPTIONMGR->GetGameOption()->bNoBalloon )
	{
		OBJECTMGR->ShowChatBalloon( pPlayer, data->Msg );
	}
}

void	CChatManager::Chat_Party( void* pMsg )
{
	MSG_CHAT_WITH_SENDERID* data = (MSG_CHAT_WITH_SENDERID*)pMsg;

	AddMsg( CTC_PARTYCHAT, "[%s]: %s", data->Name, data->Msg );
}
void	CChatManager::Chat_Family( void* pMsg )
{
	// desc_hseos_패밀리01
	// S 패밀리 추가 added by hseos 2007.07.13
	// SEND_FAMILY_CHAT* data = (SEND_FAMILY_CHAT*)pMsg;
	MSG_CHAT* pstPacket = (MSG_CHAT*)pMsg;
	// E 패밀리 추가 added by hseos 2007.07.13

	AddMsg( CTC_FAMILY, "[%s]: %s", pstPacket->Name, pstPacket->Msg );
}
void	CChatManager::Chat_Guild( void* pMsg )
{
	TESTMSGID* pData = (TESTMSGID*)pMsg;
	char Sendername[MAX_NAME_LENGTH+1];
	SafeStrCpy(Sendername, GUILDMGR->GetMemberName(pData->dwSenderID), MAX_NAME_LENGTH+1);
	if(strcmp(Sendername, "")== 0)
		return;
	AddMsg( CTC_GUILDCHAT, "[%s]: %s", Sendername, pData->Msg );
}
void	CChatManager::Chat_Alliance( void* pMsg )
{
	MSG_CHAT_WITH_SENDERID* data = (MSG_CHAT_WITH_SENDERID*)pMsg;

	AddMsg( CTC_ALLIANCE, "[%s]: %s", data->Name, data->Msg );
}
void	CChatManager::Chat_Trade( void* pMsg )
{
	MSG_CHAT_WITH_SENDERID* data = (MSG_CHAT_WITH_SENDERID*)pMsg;

	AddMsg( CTC_TRADE, "[%s]: %s", data->Name, data->Msg );
}

void	CChatManager::Chat_System( void* pMsg )
{
	MSG_CHAT_WITH_SENDERID* data = (MSG_CHAT_WITH_SENDERID*)pMsg;

	AddMsg( CTC_SYSMSG, "[%s]: %s", data->Name, data->Msg );
}
void	CChatManager::Chat_Whole( void* pMsg )
{
	MSG_CHAT_WITH_SENDERID* data = (MSG_CHAT_WITH_SENDERID*)pMsg;

	AddMsg( CTC_WHOLE, "[%s]: %s", data->Name, data->Msg );
}
void	CChatManager::Chat_Whisper( void* pMsg )
{
	MSG_CHAT* data = (MSG_CHAT*)pMsg;

	AddMsg( CTC_WHISPER, "From %s: %s", data->Name, data->Msg );
#ifdef _GMTOOL_
	GMTOOLMGR->AddChatMsg( data->Name, data->Msg, 2 );	//2:받았다.
#endif
	//m_pChattingDlg->AddReceiverName( data->Name ) ;
	AddWhisperName(data->Name) ;
}
void	CChatManager::Chat_Whisper_Ack( void* pMsg )
{
	MSG_CHAT* data = (MSG_CHAT*)pMsg;

#ifdef _GMTOOL_
	if( MAINGAME->GetUserLevel() == eUSERLEVEL_GM )
		AddMsg( CTC_GMCHAT, "To %s: %s", data->Name, data->Msg );
	else
		AddMsg( CTC_WHISPER, "To %s: %s", data->Name, data->Msg );
#else
	AddMsg( CTC_WHISPER, "To %s: %s", data->Name, data->Msg );
#endif

#ifdef _GMTOOL_
	GMTOOLMGR->AddChatMsg( data->Name, data->Msg, 1 );	//1:했다.
#endif
	//m_pChattingDlg->AddSenderName( data->Name ) ;

	AddWhisperName(data->Name) ;

	// 071223 LYW --- ChatManager : 귓말이 성공하면, 채팅 창에 귓말 입력 대기로 세팅한다.
	cEditBox* pInputBox = NULL ;
	pInputBox = m_pChattingDlg->GetInputBox() ;

	if( pInputBox )
	{
		char tempName[32] = {0, } ;
		sprintf(tempName, "/%s ", data->Name) ;
		pInputBox->SetEditText(tempName) ;
	}
}

// 070928 LYW --- ChatManager : Add function to setting whisper name to chatting manager.
void	CChatManager::AddWhisperName(char* name)
{
	if( strlen(name) < 4 ) return ;											// 최소 글자보다 이름이 작으면 return 처리를 한다.

	int count ;																// for문을 돌릴 count 변수를 선언한다.								

	BOOL bSameName = FALSE ;												// 이름의 중복 여부를 저장하는 변수를 선언하고 false로 세팅한다.

	for( count = 0 ; count < MAX_WNAME_COUNT ; ++count )					// 최대 귓말 대상 수 만큼 for문을 돌린다.
	{
		if( strcmp(whisperName[count], name) == 0 )							// 같은 이름이 있으면, 
		{
			bSameName = TRUE ;												// 결과 값을 true로 세팅한다.
			break ;															// for문을 탈출한다.
		}
	}

	if( bSameName ) return ;												// 같은 이름이 있다면, return 처리를 한다.

	int nSize = sizeof(char)*MAX_WNAME_SIZE ;								// 이름 복사를 위한 사이즈를 담을 변수를 선언하고 초기화 한다.

	char tempName[MAX_WNAME_COUNT-1][MAX_WNAME_SIZE] ;						// 이름을 복사 할 임시 버퍼를 선언한다.

	for(count = 0 ; count < MAX_WNAME_COUNT-1 ; ++count)					// 최대 귓말 대상 수 -1 만큼 for문을 돌린다.
	{
		memset(tempName[count], 0, nSize) ;									// 임시 버퍼를 초기화 한다.
	}

	for(count = 0 ; count < MAX_WNAME_COUNT-1 ; ++count)					// 최대 귓말 대상 수 -1 만큼 for문을 돌린다.
	{
		memset(whisperName[count], 0, nSize) ;								// 현재 귓말 대상의 이름을 초기화 한다.
		memcpy(whisperName[count], whisperName[count+1], nSize) ;			// 다음 귓말 대상의 이름을 현재 귓말 대상에 복사한다.
	}

	memset(whisperName[MAX_WNAME_COUNT-1], 0, nSize) ;						// 마지막 귓말 대상의 이름을 초기화 한다.
	strcpy(whisperName[MAX_WNAME_COUNT-1], name) ;							// 마지막 귓말 대상의 이름에 인자로 넘어온 이름을 세팅한다.
}

// 070928 LYW --- Add function to return whisper name by selected index.
char*	CChatManager::GetWhisperName(int nIdx)
{
	if( (nIdx < 0) ||  nIdx > MAX_WNAME_COUNT-1 ) return NULL ;				// 인자로 넘어온 인덱스의 범위를 체크한다.

	return whisperName[nIdx] ;												// 인덱스에 해당하는 whisper name을 return 처리를 한다.
}

// 071001 LYW --- Add function to initialize whisper buffer.
void CChatManager::InitializeWhisperName()
{
	for( int count = 0 ; count < MAX_WNAME_COUNT ; ++count )								// 배열 수 만큼 for문을 돌린다.
	{
		memset(whisperName[count], 0, sizeof(char)*MAX_WNAME_SIZE) ;						// 귓말 배열을 초기화 한다.
	}
}


void	CChatManager::Chat_Whisper_Nack( void* pMsg )
{
	const MSG_BYTE* data = (MSG_BYTE*)pMsg;

	switch( data->bData )
	{
	case CHATERR_NO_NAME:
		{
#ifdef _GMTOOL_
			//GMTOOLMGR->AddChatMsg( "", GetChatMsg(210), 3 );
			AddMsg( CTC_WHISPER, GetChatMsg(210) );
#else
			AddMsg( CTC_WHISPER, GetChatMsg(210) );
#endif
			break;
		}
	case CHATERR_NOT_CONNECTED:
		{
#ifdef _GMTOOL_
			//GMTOOLMGR->AddChatMsg( "", GetChatMsg(374), 3 );
			AddMsg( CTC_WHISPER, GetChatMsg(374) );//confirm
#else
			AddMsg( CTC_WHISPER, GetChatMsg(374) );//confirm
#endif
			break;
		}
	case CHATERR_OPTION_NOWHISPER:
		{
#ifdef _GMTOOL_
			//GMTOOLMGR->AddChatMsg( "", GetChatMsg(201), 3 );
			AddMsg( CTC_WHISPER, GetChatMsg(201) );//confirm
#else
			AddMsg( CTC_WHISPER, GetChatMsg(201) );//confirm
#endif
			break;
		}
	default:
		{
			AddMsg( CTC_WHISPER, GetChatMsg(202) );//confirm
			break;
		}
	}
}


void	CChatManager::Chat_Monster_Speech( void* pMsg )
{
	MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;
	char* temp = MON_SPEECHMGR->GetMonsterSpeech(pmsg->dwData2);
	if( temp )
	{
		switch(pmsg->dwData1)
		{
		case 0:	//말풍선
			{
				CMonster* pMonster = (CMonster*)OBJECTMGR->GetObject(pmsg->dwObjectID);

				if( pMonster )
				{
					OBJECTMGR->ShowChatBalloon( pMonster, temp);
				}
			}
			break;
		case 1:	//말풍선 + 채팅창
			{
				CMonster* pMonster = (CMonster*)OBJECTMGR->GetObject(pmsg->dwObjectID);
				if( pMonster )
				{
					OBJECTMGR->ShowChatBalloon( pMonster, temp);
				}


				AddMsg( CTC_MONSTERSPEECH, "%s: %s", pMonster->GetSInfo()->Name, temp );
			}
			break;
		case 2:	//채널 외치기
			{
				BASE_MONSTER_LIST* pMonInfo = GAMERESRCMNGR->GetMonsterListInfo(pmsg->dwData4);
				if(pMonInfo) 
					AddMsg( CTC_MONSTERSHOUT, "%s: %s", pMonInfo->Name, temp );

				CMonster* pMonster = (CMonster*)OBJECTMGR->GetObject(pmsg->dwData3);
				if( pMonster )
				{
					OBJECTMGR->ShowChatBalloon( pMonster, temp);
				}
			}
			break;
		}
	}
}

void CChatManager::Chat_Shout_Ack(void*pMsg)
{
	MSG_CHAT_WORD* data = (MSG_CHAT_WORD*)pMsg;
	AddMsg(CTC_SHOUT, "%s", data->Msg);
}

void CChatManager::Chat_Shout_Nack(void*pMsg)
{
	MSG_DWORD* data = (MSG_DWORD*)pMsg;

	AddMsg( CTC_WHISPER, GetChatMsg(1461));
}

void CChatManager::SaveChatList()
{/*
	for( int i = 0 ; i < e_CONVERSATION_MAX-1 ; ++i )
	{
		cPtrList* pPtr = m_pChattingDlg->GetChatting(i)->GetListDlg()->GetListItem();
		PTRLISTPOS pos = pPtr->GetHeadPosition();

		while( pos )
		{
			ITEM* pItem = new ITEM;
			*pItem = *(ITEM*)pPtr->GetNext( pos );
			m_ChatList[i].AddTail( pItem );
		}
	}

	cPtrList* pPtr = m_pChattingDlg->GetAssist()->GetListDlg()->GetListItem();
	PTRLISTPOS pos = pPtr->GetHeadPosition();

	while( pos )
	{
		ITEM* pItem = new ITEM;
		*pItem = *(ITEM*)pPtr->GetNext( pos );
		m_AssistList.AddTail( pItem );
	}*/

		cPtrList* pPtr = m_pChattingDlg->GetChatListDlg()->GetListItem() ;
	PTRLISTPOS pos = pPtr->GetHeadPosition() ;

	while(pos)
	{
		ITEM* pItem = new ITEM ;
		*pItem = *(ITEM*)pPtr->GetNext( pos ) ;
		m_ChatList.AddTail( pItem ) ;
	}

	cPtrList* pPtr2 = m_pChattingDlg->GetAssistListDlg()->GetListItem() ;
	PTRLISTPOS pos2 = pPtr2->GetHeadPosition();

	while( pos2 )
	{
		ITEM* pItem = new ITEM;
		*pItem = *(ITEM*)pPtr2->GetNext( pos2 );
		m_AssistList.AddTail( pItem );
	}

	cPtrList* pPtr3 = m_pChattingDlg->GetShoutListDlg()->GetListItem() ;
	PTRLISTPOS pos3 = pPtr3->GetHeadPosition();

	while( pos3 )
	{
		ITEM* pItem = new ITEM;
		*pItem = *(ITEM*)pPtr3->GetNext( pos3 );
		m_ShoutList.AddTail( pItem );
	}
}

void CChatManager::RestoreChatList()
{
	/*for( int i = 0 ; i < e_CONVERSATION_MAX-1 ; ++i )
	{
		PTRLISTPOS pos = m_ChatList[i].GetHeadPosition();

		while( pos )
		{
			ITEM* pItem = (ITEM*)m_ChatList[i].GetNext( pos );
			if( pItem )
			{
				m_pChattingDlg->GetChatting(i)->GetListDlg()->AddItem( pItem->string, pItem->rgb, pItem->line );
			}
		}
	}

	PTRLISTPOS pos = m_ChatList[i].GetHeadPosition();

	while( pos )
	{
		ITEM* pItem = (ITEM*)m_AssistList.GetNext( pos );
		if( pItem )
		{
			m_pChattingDlg->GetAssist()->GetListDlg()->AddItem( pItem->string, pItem->rgb, pItem->line );
		}
	}

	ReleaseChatList();*/

		PTRLISTPOS pos = m_ChatList.GetHeadPosition() ;

	ITEM* pItem = NULL ;

	while( pos )
	{
		pItem = (ITEM*)m_ChatList.GetNext( pos ) ;

		if( pItem )
		{
			m_pChattingDlg->GetChatListDlg()->AddItem( pItem->string, pItem->rgb, pItem->line ) ;
		}
	}

	PTRLISTPOS pos2 = m_AssistList.GetHeadPosition() ;

	ITEM* pItem2 = NULL ;
	while( pos2 )
	{
		pItem2 = (ITEM*)m_AssistList.GetNext( pos2 ) ;

		if( pItem2 )
		{
			m_pChattingDlg->GetAssistListDlg()->AddItem( pItem2->string, pItem2->rgb, pItem2->line ) ;
		}
	}

	PTRLISTPOS pos3 = m_ShoutList.GetHeadPosition() ;

	ITEM* pItem3 = NULL ;
	while( pos3 )
	{
		pItem3 = (ITEM*)m_ShoutList.GetNext( pos3 ) ;

		if( pItem3 )
		{
			m_pChattingDlg->GetShoutListDlg()->AddItem( pItem3->string, pItem3->rgb, pItem3->line ) ;
		}
	}

	ReleaseChatList() ;
}

void CChatManager::ReleaseChatList()
{
	/*for( int i = 0 ; i < e_CONVERSATION_MAX-1 ; ++i )
	{
		PTRLISTPOS pos = m_ChatList[i].GetHeadPosition();

		while( pos )
		{
			ITEM* pItem = (ITEM*)m_ChatList[i].GetNext( pos );
			if( pItem )
				delete pItem;				
		}

		m_ChatList[i].RemoveAll();
	}

	PTRLISTPOS pos = m_AssistList.GetHeadPosition();

	while( pos )
	{
		ITEM* pItem = (ITEM*)m_AssistList.GetNext( pos );
		if( pItem )
			delete pItem;				
	}

	m_AssistList.RemoveAll();*/

		PTRLISTPOS pos = m_ChatList.GetHeadPosition() ;

	ITEM* pItem = NULL ;

	while( pos )
	{
		pItem = (ITEM*)m_ChatList.GetNext( pos ) ;

		if( pItem ) delete pItem ;
	}

	m_ChatList.RemoveAll() ;

	PTRLISTPOS pos2 = m_AssistList.GetHeadPosition() ;

	ITEM* pItem2 = NULL ;

	while( pos2 )
	{
		pItem2 = (ITEM*)m_AssistList.GetNext( pos2 ) ;

		if( pItem2 ) delete pItem2 ;
	}

	m_AssistList.RemoveAll() ;

	PTRLISTPOS pos3 = m_ShoutList.GetHeadPosition() ;

	ITEM* pItem3 = NULL ;

	while( pos3 )
	{
		pItem3 = (ITEM*)m_ShoutList.GetNext( pos3 ) ;

		if( pItem3 ) delete pItem3 ;
	}

	m_ShoutList.RemoveAll() ;
}

void CChatManager::SetOption( sChatOption* pChatOption )
{
	memcpy( &m_ChatOption, pChatOption, sizeof(sChatOption) ) ;								// 채팅 매니져의 멤버 옵션에 들어온 옵션을 받는다.
}

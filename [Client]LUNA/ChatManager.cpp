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
	ZeroMemory( m_pChatMsg, sizeof(m_pChatMsg) ) ;											// ä�� �޽����� �ʱ�ȭ �Ѵ�.

	ZeroMemory( &m_ChatOption, sizeof( m_ChatOption ) ) ;									// ä�� �ɼ��� �ʱ�ȭ �Ѵ�.

	m_dwLastChatTime = 0;																	// ������ ä�� Ÿ���� �ʱ�ȭ �Ѵ�.

	// 070928 LYW --- ChatManager : Initialize whisper name.
	InitializeWhisperName() ;
}


CChatManager::~CChatManager(void)
{
	// 070420 LYW --- ChatManager : Release m_pChatMsg.
	Release() ;																				// ���� �Լ��� ȣ���Ѵ�.

	//// 070608 LYW --- ChatManager : Delete emotion filter.
	//// 070420 LYW --- ChatManager : Release m_EnotionList.
	//ReleaseEmotionList() ;																// �̸�� ��Ʈ�� ���� �Ѵ�.
}


void	CChatManager::Init()
{
	LoadChatMsg();																			// ä�� �޽����� �ε��Ѵ�.

	LoadFaceCommandList() ;																	// �� ��ü ����� �ε��Ѵ�.

	// 070910 LYW --- ChatManager : Add function to load action command.
	LoadActionCommandList() ;																// �׼� ����� �ε��Ѵ�.

	//// 070608 LYW --- ChatManager : Delete emotion filter.
	//LoadEmotionFilter() ;																	// �̸�� ���͸� �ε��Ѵ�.
}


void	CChatManager::Release()
{
	for( int i = 0 ; i < MAX_CONVERSATION_MSGNUM ; ++i )									// �ִ� ��ȭ �޽��� �� ��ŭ, ������ ���鼭,
	{
		SAFE_DELETE_ARRAY( m_pChatMsg[ i ] );												// ä�� �޽����� �����ϰ� �����Ѵ�.
	}

	PTRLISTPOS pos = m_FaceCommandList.GetHeadPosition() ;									// �� ��ü ��ɾ� ����Ʈ���� ��ġ �������� �޾ƿ´�.

	sFACEINFO* pFaceInfo = NULL ;															// �� ���� �����͸� �����Ѵ�.

	while( pFaceInfo = (sFACEINFO*)m_FaceCommandList.GetNext( pos ) )						// ����Ʈ���� ��ġ�� ���� �� ������ �޾ƿ´�.
	{
		if( pFaceInfo )																		// �� ������ ��ȿ�ϴٸ�,
		{
			SAFE_DELETE( pFaceInfo );														// ������ �����ϰ� �����Ѵ�.
		}
	}

	m_FaceCommandList.RemoveAll() ;															// �� ���� ��� ����Ʈ�� ��� ����.

	sACTION_COMMAND* pDeleteCommand ;

	PTRLISTPOS deletePos = NULL ;
	deletePos = m_ActionCommandList.GetHeadPosition() ;

	while( deletePos )																	// ������ ������ while�� ������.
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
	CMHFile fp;																				// ���������� �����Ѵ�.

	if( !fp.Init( FILE_CHAT_MSG, "rb" ) )													// ä�� �޽��� ������ �д°��� �����ϸ�,
	{
		return;																				// ���� ó���� �Ѵ�.
	}

	char msg[1024] = {0,};																	// �ӽ� �޽��� ����Ʋ �����Ѵ�.

	while( TRUE )																			// whiel���� ������.
	{
		const int nIndex = fp.GetInt();														// �ε����� �о���δ�.

		if( fp.IsEOF() ||																	// ������ ���̰ų�,
			nIndex < 0 ||																	// �ε���Ÿ 0���� ũ�ų�,
			nIndex >= MAX_CONVERSATION_MSGNUM )												// �ε����� �ִ� �޽��� ���� ������,
		{
			break;																			// while���� �����.
		}

		fp.GetStringInQuotation(msg);														// msg�� ��Ʈ���� �о� ���δ�.

		const int nLen = strlen( msg );														// �޽����� ���̸� ���Ѵ�.

		if( nLen > 0 )																		// �޽����� ��ȿ�ϴٸ�,
		{
			m_pChatMsg[nIndex] = new char[nLen+1];											// �޽��� ���̸�ŭ �޸𸮸� �Ҵ��Ѵ�.
			memcpy( m_pChatMsg[nIndex], msg, nLen+1 );										// �޽��� �ε��� �����Ϳ�, �޽����� �����Ѵ�.
			fp.GetLine(msg, 256);															// ������ �о���δ�.
		}
	}
}


char*	CChatManager::GetChatMsg( int nMsgNum )
{
	ASSERT( -1 < nMsgNum && MAX_CONVERSATION_MSGNUM > nMsgNum );							// �޽��� ���� üũ�� �Ѵ�.

	char* message = m_pChatMsg[ nMsgNum ];													// ���� �޽��� ��ȣ�� �޽��� �����͸� �Ѱ� �޴´�.

	if( message )																			// �޽����� ��ȿ���� üũ�Ѵ�.
	{
		return message;																		// �޽����� �����Ѵ�.
	}
	else																					// �޽����� ��ȿ���� �ʴٸ�,
	{
		return "";																			// �� �޽����� �����Ѵ�.
	}
}

void	CChatManager::AddMsg( int nClass, char* str, ... )
{
	if( ! m_pChattingDlg || str == NULL )													// ä�ô��̾� �αװ� ��ȿ���� �ʰų�, �޽����� ��ȿ���� ������,
	{
		return;																				// ����ó���� �Ѵ�.
	}

	char msg[256];																			// �ӽ� �޽��� ���۸� �����Ѵ�.

	va_list argList;																		// �����μ��� �б� ���� ������ ������ �����Ѵ�.

	va_start(argList, str);																	// ������ ������ ù ��° ���� �μ��� ����Ű���� �ʱ�ȭ�Ѵ�.

	vsprintf(msg,str,argList);																// �����͸� �̿���, ����� �ƿ�ǲ���� msg�� ����.

	va_end(argList);																		// �����μ��� �� ���� �� �������� �Ѵ�.

	DWORD dwColor = dwChatTextColor[ nClass ];												// Ŭ������ ���� ������ �����Ѵ�.

	switch( nClass )																		// Ŭ������ üũ�Ѵ�.
	{
	// 080313 NYJ --- ��ü��ġ�� �߰�
	case CTC_SHOUT:
		{
			m_pChattingDlg->AddShoutMsg(msg, dwColor);										// ��ġ�� �޼����� ����Ѵ�.
		}
		break;

	case CTC_OPERATOR:																		// ��� �޼������,
		{
			m_pChattingDlg->AddMsg( e_CONVERSATION_SYSTEM, msg, dwColor ) ;					// �ý��� â�� �޽����� ����Ѵ�.
			NOTIFYMGR->AddMsg( msg );														// ������ �޽����� ����Ѵ�.
		}
		break;

	case CTC_OPERATOR2:																		// ��� �޼������,
		{
			NOTIFYMGR->AddMsg( msg );														// ������ �޽����� ����Ѵ�.
		}
		break;

	case CTC_BILLING :																		// ���� �޽������,
		{
			m_pChattingDlg->AddMsg( e_CONVERSATION_SYSTEM, msg, dwColor ) ;					// �ý��� â�� �޽����� ����Ѵ�.
			NOTIFYMGR->AddMsg( msg, eNTC_REMAINTIME );										// ������ �޽����� ����Ѵ�.
		}
		break ;

	case CTC_GMCHAT :																		// GM ä���̶��,
		{
			m_pChattingDlg->AddMsg( e_CONVERSATION_SYSTEM, msg, dwColor ) ;					// �޽����� ����Ѵ�.
		}
		break ;

	case CTC_GENERALCHAT :																	// �Ϲ� ä���̸�,
		{
			if( !m_ChatOption.bOption[CTO_NOCHATMSG] )										// ä�� �ɼ��� �Ϲ�ä�� ���⸦ üũ�Ѵ�.
			{
				m_pChattingDlg->AddMsg( e_CONVERSATION_ALL, msg, dwColor ) ;				// �Ϲ� ä���� ����Ѵ�.
			}
		}
		break ;

	case CTC_PARTYCHAT :																	// ��Ƽ ä���̸�,
		{
			if( !m_ChatOption.bOption[CTO_NOPARTYMSG] )										// ä�� �ɼ��� ��Ƽä�� ���⸦ üũ�Ѵ�.
			{
				m_pChattingDlg->AddMsg( e_CONVERSATION_PARTY, msg, dwColor ) ;				// ��Ƽ ä���� ����Ѵ�.
			}
		}
		break ;

	case CTC_FAMILY :																		// �йи� ä���̸�,
		{
			if( !m_ChatOption.bOption[CTO_NOFAMILYMSG] )										// ä�� �ɼ��� �йи�ä�� ���⸦ üũ�Ѵ�.
			{
				m_pChattingDlg->AddMsg( e_CONVERSATION_FAMILY, msg, dwColor ) ;				// �йи� ä���� ����Ѵ�.
			}
		}
		break ;

	case CTC_GUILDCHAT :																	// ��� ä���̸�,
		{
			if( !m_ChatOption.bOption[CTO_NOGUILDMSG] )										// ä�� �ɼ��� ���ä�� ���⸦ üũ�Ѵ�.
			{
				m_pChattingDlg->AddMsg( e_CONVERSATION_GUILD, msg, dwColor ) ;				// ��� ä���� ����Ѵ�.
			}
		}
		break ;

	case CTC_ALLIANCE :																		// ���� ä���̸�,
		{
			if( !m_ChatOption.bOption[CTO_NOALLIANCEMSG] )									// ä�� �ɼ��� ���� ä�� ���⸦ üũ�Ѵ�.
			{
				m_pChattingDlg->AddMsg( e_CONVERSATION_ALLIANCE, msg, dwColor ) ;			// ���� ü���� ����.
			}
		}
		break ;

	case CTC_TRADE :																		// �ŷ� ä���̸�,
		{
			if( !m_ChatOption.bOption[CTO_NOSHOUTMSG] )										// ä�� �ɼ��� ��ġ�� ���⸦ üũ�Ѵ�.
			{
				m_pChattingDlg->AddMsg( e_CONVERSATION_TRADE, msg, dwColor ) ;				// ��ġ�� ä���� ����Ѵ�.
			}
		}
		break ;

	case CTC_SYSMSG :																		// �ý��� �޽������,
		{
			if( !m_ChatOption.bOption[CTO_NOSYSMSG] )										// ä�� �ɼ��� �ý��� �޽��� ���⸦ Ȯ���Ѵ�.
			{
				m_pChattingDlg->AddMsg( e_CONVERSATION_SYSTEM, msg, dwColor ) ;				// �ý��� �޽����� ����Ѵ�.
			}
		}
		break ;

	case CTC_WHISPER :																		// �ӼӸ� �޽������,
		{
			m_pChattingDlg->AddMsg( e_CONVERSATION_SYSTEM, msg, dwColor ) ;					// �ӼӸ��� ����Ѵ�.
		}
		break ;

	case CTC_ATTACK :																		// ���� �޽������,
		{
			if( !m_ChatOption.bOption[CTO_NOSYSMSG] )										// ä�� �ɼǿ��� �ý��� �޽��� ���⸦ üũ�Ѵ�.
			{
				m_pChattingDlg->AddMsg( e_CONVERSATION_ALL, msg, dwColor ) ;				// ���� �޽����� ����Ѵ�.
			}
		}
		break ;

	case CTC_ATTACKED :																		// ��밡 ������ �޽������,
		{
			if( !m_ChatOption.bOption[CTO_NOSYSMSG] )										// ä�� �ɼǿ��� �ý��� �޽��� ���⸦ üũ�Ѵ�.
			{
				m_pChattingDlg->AddMsg( e_CONVERSATION_ALL, msg, dwColor ) ;				// ��밡 ������ �޽����� ����Ѵ�.
			}
		}
		break ;

	case CTC_DEFENCE :																		// ��� �޽������,
		{
			if( !m_ChatOption.bOption[CTO_NOSYSMSG] )										// ä�� �ɼǿ��� �ý��� �޽��� ���⸦ üũ�Ѵ�.
			{
				m_pChattingDlg->AddMsg( e_CONVERSATION_ALL, msg, dwColor ) ;				// ��� �޽����� ����Ѵ�.
			}
		}
		break ;

	case CTC_DEFENCED :																		// ��밡 ����� �޽������,
		{
			if( !m_ChatOption.bOption[CTO_NOSYSMSG] )										// ä�� �ɼǿ��� �ý��� �޽��� ���⸦ üũ�Ѵ�.
			{
				m_pChattingDlg->AddMsg( e_CONVERSATION_ALL, msg, dwColor ) ;				// ��밡 ����� �޽����� ����Ѵ�.
			}
		}
		break ;

	case CTC_KILLED :																		// �׾��� �� �޽��� ���,
		{
			if( !m_ChatOption.bOption[CTO_NOSYSMSG] )										// ä�� �ɼǿ��� �ý��� �޽��� ���⸦ üũ�Ѵ�.
			{
				m_pChattingDlg->AddMsg( e_CONVERSATION_ALL, msg, dwColor ) ;				// �׾��� �� �޽����� ����Ѵ�.
			}
		}
		break ;

	case CTC_MPWARNING :																	// MP ���,
		{
			if( !m_ChatOption.bOption[CTO_NOSYSMSG] )										// ä�� �ɼǿ��� �ý��� �޽��� ���⸦ üũ�Ѵ�.
			{
				m_pChattingDlg->AddMsg( e_CONVERSATION_ALL, msg, dwColor ) ;				// ���� ���� ��� ����Ѵ�.
			}
		}
		break ;

	case CTC_HPWARNING :																	// HP ���,
		{
			if( !m_ChatOption.bOption[CTO_NOSYSMSG] )										// ä�� �ɼǿ��� �ý��� �޽��� ���⸦ üũ�Ѵ�.
			{
				m_pChattingDlg->AddMsg( e_CONVERSATION_ALL, msg, dwColor ) ;				// ����� ���� ��� ����Ѵ�.
			}
		}

	case CTC_GETITEM :																		// ������ ��Ʈ �޽������,
		{
			if( !m_ChatOption.bOption[CTO_NOITEMMSG] )										// ä�� �ɼ��� ��/������ ȹ��޽��� ���⸦ Ȯ���Ѵ�.
			{
				m_pChattingDlg->AddMsg( e_CONVERSATION_ALL, msg, dwColor ) ;				// ������ �޽����� ����Ѵ�.
			}
		}
		break ;

	case CTC_GETEXP :																		// ����ġ ��Ʈ �޽������,
		{
			if( !m_ChatOption.bOption[CTO_NOEXPMSG] )										// ä�� ������ ����ġ/����ġ ȹ�� �޽��� ���⸦ Ȯ���Ѵ�.
			{
				m_pChattingDlg->AddMsg( e_CONVERSATION_ALL, msg, dwColor ) ;				// ����ġ �޽����� ����Ѵ�.
			}
		}
		break ;

	case CTC_TOWHOLE :																		// ��ġ�⸦ �ߴٸ�,
		{
			m_pChattingDlg->AddMsg( e_CONVERSATION_ALL, msg, dwColor ) ;					// �޽����� ����Ѵ�.
		}
		break ;

	case CTC_TOPARTY :																		// ��Ƽ���� �޽����� �����ٸ�,
		{
			m_pChattingDlg->AddMsg( e_CONVERSATION_ALL, msg, dwColor ) ;					// �޽����� ����Ѵ�.
		}
		break ;

	case CTC_TOFAMILY :																		// �йи����� �޽����� �����ٸ�,
		{
			m_pChattingDlg->AddMsg( e_CONVERSATION_ALL, msg, dwColor ) ;					// �޽����� ����Ѵ�.
		}
		break ;

	case CTC_TOGUILD :																		// ��忡�� �޽����� ��������,
		{
			m_pChattingDlg->AddMsg( e_CONVERSATION_ALL, msg, dwColor ) ;					// �޽����� ����Ѵ�.
		}
		break ;

	case CTC_MONSTERSPEECH :																// ���� ���ϱ� �޽������,
		{
			m_pChattingDlg->AddMsg( e_CONVERSATION_ALL, msg, dwColor ) ;					// �޽����� ����Ѵ�.
		}
		break ;

	case CTC_MONSTERSHOUT :																	// ���� ��ġ����,
		{
			m_pChattingDlg->AddMsg( e_CONVERSATION_ALL, msg, dwColor ) ;					// �޽����� ����Ѵ�.
		}
		break ;

	default :																				// ���ݱ����� ��� �ܿ�,
		{
			m_pChattingDlg->AddMsg( e_CONVERSATION_SYSTEM, msg, dwColor ) ;					// �޽����� ����Ѵ�.
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
	CMHFile	fp ;																			// ���� ������ �����Ѵ�.

	if( ! fp.Init( FILE_ACTION_COMMAND, "rb" ) )												// �� ��ü ������ �дµ� �����ϸ�,
	{
		char buff[256];																		// �ӽ� ���۸� �����Ѵ�.
		sprintf(buff, "Loading failed!! ./data/interface/windows/Action_Command.bin");		// ���� ��θ� Ȯ���Ͽ� ���ۿ� �����Ѵ�.
		ASSERTMSG(0, buff) ;
	}

	int nIndex = 0 ;
	char tempBuf[32] = {0, } ;

	while( ! fp.IsEOF() )																	// ������ ������ while�� ������.
	{
		memset(tempBuf, 0, sizeof(tempBuf)) ;

		nIndex = fp.GetInt() ;	
		strcpy(tempBuf, fp.GetString()) ;

		if( strlen(tempBuf) <= 2 ) continue ;

		sACTION_COMMAND* pCommand = new sACTION_COMMAND ;									// �� ���� ����ü ��ŭ �޸𸮸� �Ҵ��Ͽ� �����ͷ� �ѱ��.

		pCommand->index = nIndex ;															// �ε����� �д´�.
		strcpy(pCommand->string, tempBuf) ;													// ��Ʈ���� �о� Descript�� �����Ѵ�.

		m_ActionCommandList.AddTail(pCommand) ;												// ������ ����Ʈ�� �߰��Ѵ�.
	}
}

void CChatManager::LoadFaceCommandList()
{
	CMHFile	fp ;																			// ���� ������ �����Ѵ�.

	if( ! fp.Init( FILE_CHAT_FACELIST, "rb" ) )												// �� ��ü ������ �дµ� �����ϸ�,
	{
		char buff[256];																		// �ӽ� ���۸� �����Ѵ�.
		sprintf(buff, "Loading failed!! ./data/interface/windows/Chat_FaceList.bin");		// ���� ��θ� Ȯ���Ͽ� ���ۿ� �����Ѵ�.
		LOGEX(buff, PT_MESSAGEBOX);															// �α׸� �����, �޽��� �ڽ��� ����.
	}

	while( ! fp.IsEOF() )																	// ������ ������ while�� ������.
	{
		sFACEINFO* pFaceInfo = new sFACEINFO ;												// �� ���� ����ü ��ŭ �޸𸮸� �Ҵ��Ͽ� �����ͷ� �ѱ��.

		pFaceInfo->nIndex = fp.GetInt() ;													// �ε����� �д´�.
		strcpy(pFaceInfo->szDescript, fp.GetString()) ;										// ��Ʈ���� �о� Descript�� �����Ѵ�.
		strcpy(pFaceInfo->szCommand, fp.GetStringInQuotation()) ;							// ��Ʈ���� �о� Command�� �����Ѵ�.
		pFaceInfo->nFaceNum = fp.GetInt() ;													// �� ��ȣ�� �д´�.
		pFaceInfo->nLen = strlen(pFaceInfo->szCommand) ;									// ��ɾ��� ���̸� ���Ѵ�.

		m_FaceCommandList.AddTail(pFaceInfo) ;												// ������ ����Ʈ�� �߰��Ѵ�.
	}
}

int CChatManager::GetFaceNum( const TCHAR* str ) const
{
	// ���ڸ� �ϳ��� �������Ѽ� �ش� ���ڰ� ���ԵǾ����� ã�´�.
	// ���ڿ��� ��Ƽ����Ʈ ���ڰ� ���ԵǾ� �����Ƿ�, ������ų ���� ��Ƽ����Ʈ ������ �������Ѿ� �Ѵ�
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

	// ä�ù��� �ؽ�Ʈ �Է��� �޴´�.
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

				// �Ϲ� ä��â ó�� üũ.
				if (pInputBox->IsFocus())
				{
					char szText[256] = {0, } ;
					DWORD dwColor = RGB(0, 255, 255) ;

					sprintf(szText, "[%s]:%s", HERO->GetObjectName(), pInputBox->GetEditText()) ;		

					GAMEIN->GetDateMatchingDlg()->GetChatingDlg()->AddMsg(szText, dwColor);
					g_csResidentRegistManager.CLI_SendDateMatchingChatText(pInputBox->GetEditText()) ;
					pInputBox->SetEditText("");
				}
				// ä�ù� �� ó�� üũ.
				else if(pChatRoomCtrl->IsFocus())
				{
					CHATROOMMGR->MsgProc() ;
				}
				// ��Ī �� ó�� üũ.
				else if( ! pTextArea->IsFocus() )
				{
					if( !CIMEWND->IsDocking() )		//�ٸ������Ϳ� ��Ŀ���� ���ٸ�.
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
	if( gCurTime - m_dwLastChatTime < 1000 )	// 1��
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(248) );
		return FALSE;
	}

	//#ifdef _HK_LOCAL_
	//
	//	switch( cToken )
	//	{
	//		case '/':
	//			if( gCurTime - m_dwLastChatDelayTime[eCHATDELAY_WHISPER] < 3000 ) //3��
	//				return FALSE;
	//			m_dwLastChatDelayTime[eCHATDELAY_WHISPER] = gCurTime;
	//			break;
	//		case '@':
	//			if( gCurTime - m_dwLastChatDelayTime[eCHATDELAY_PARTY] < 1000 ) //1��
	//				return FALSE;
	//			m_dwLastChatDelayTime[eCHATDELAY_PARTY] = gCurTime;
	//			break;
	//		case '#':
	//			if( gCurTime - m_dwLastChatDelayTime[eCHATDELAY_GUILD] < 3000 ) //3��
	//				return FALSE;
	//			m_dwLastChatDelayTime[eCHATDELAY_GUILD] = gCurTime;
	//			break;
	//		case '%':
	//			if( gCurTime - m_dwLastChatDelayTime[eCHATDELAY_SHOUT] < 5000 ) //5��
	//				return FALSE;
	//			m_dwLastChatDelayTime[eCHATDELAY_SHOUT] = gCurTime;
	//			break;
	//		case '$':
	//			if( gCurTime - m_dwLastChatDelayTime[eCHATDELAY_UNION] < 5000 ) //5��
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
	// desc_hseos_�йи�01
	// S �йи� �߰� added by hseos 2007.07.13
	// SEND_FAMILY_CHAT* data = (SEND_FAMILY_CHAT*)pMsg;
	MSG_CHAT* pstPacket = (MSG_CHAT*)pMsg;
	// E �йи� �߰� added by hseos 2007.07.13

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
	GMTOOLMGR->AddChatMsg( data->Name, data->Msg, 2 );	//2:�޾Ҵ�.
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
	GMTOOLMGR->AddChatMsg( data->Name, data->Msg, 1 );	//1:�ߴ�.
#endif
	//m_pChattingDlg->AddSenderName( data->Name ) ;

	AddWhisperName(data->Name) ;

	// 071223 LYW --- ChatManager : �Ӹ��� �����ϸ�, ä�� â�� �Ӹ� �Է� ���� �����Ѵ�.
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
	if( strlen(name) < 4 ) return ;											// �ּ� ���ں��� �̸��� ������ return ó���� �Ѵ�.

	int count ;																// for���� ���� count ������ �����Ѵ�.								

	BOOL bSameName = FALSE ;												// �̸��� �ߺ� ���θ� �����ϴ� ������ �����ϰ� false�� �����Ѵ�.

	for( count = 0 ; count < MAX_WNAME_COUNT ; ++count )					// �ִ� �Ӹ� ��� �� ��ŭ for���� ������.
	{
		if( strcmp(whisperName[count], name) == 0 )							// ���� �̸��� ������, 
		{
			bSameName = TRUE ;												// ��� ���� true�� �����Ѵ�.
			break ;															// for���� Ż���Ѵ�.
		}
	}

	if( bSameName ) return ;												// ���� �̸��� �ִٸ�, return ó���� �Ѵ�.

	int nSize = sizeof(char)*MAX_WNAME_SIZE ;								// �̸� ���縦 ���� ����� ���� ������ �����ϰ� �ʱ�ȭ �Ѵ�.

	char tempName[MAX_WNAME_COUNT-1][MAX_WNAME_SIZE] ;						// �̸��� ���� �� �ӽ� ���۸� �����Ѵ�.

	for(count = 0 ; count < MAX_WNAME_COUNT-1 ; ++count)					// �ִ� �Ӹ� ��� �� -1 ��ŭ for���� ������.
	{
		memset(tempName[count], 0, nSize) ;									// �ӽ� ���۸� �ʱ�ȭ �Ѵ�.
	}

	for(count = 0 ; count < MAX_WNAME_COUNT-1 ; ++count)					// �ִ� �Ӹ� ��� �� -1 ��ŭ for���� ������.
	{
		memset(whisperName[count], 0, nSize) ;								// ���� �Ӹ� ����� �̸��� �ʱ�ȭ �Ѵ�.
		memcpy(whisperName[count], whisperName[count+1], nSize) ;			// ���� �Ӹ� ����� �̸��� ���� �Ӹ� ��� �����Ѵ�.
	}

	memset(whisperName[MAX_WNAME_COUNT-1], 0, nSize) ;						// ������ �Ӹ� ����� �̸��� �ʱ�ȭ �Ѵ�.
	strcpy(whisperName[MAX_WNAME_COUNT-1], name) ;							// ������ �Ӹ� ����� �̸��� ���ڷ� �Ѿ�� �̸��� �����Ѵ�.
}

// 070928 LYW --- Add function to return whisper name by selected index.
char*	CChatManager::GetWhisperName(int nIdx)
{
	if( (nIdx < 0) ||  nIdx > MAX_WNAME_COUNT-1 ) return NULL ;				// ���ڷ� �Ѿ�� �ε����� ������ üũ�Ѵ�.

	return whisperName[nIdx] ;												// �ε����� �ش��ϴ� whisper name�� return ó���� �Ѵ�.
}

// 071001 LYW --- Add function to initialize whisper buffer.
void CChatManager::InitializeWhisperName()
{
	for( int count = 0 ; count < MAX_WNAME_COUNT ; ++count )								// �迭 �� ��ŭ for���� ������.
	{
		memset(whisperName[count], 0, sizeof(char)*MAX_WNAME_SIZE) ;						// �Ӹ� �迭�� �ʱ�ȭ �Ѵ�.
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
		case 0:	//��ǳ��
			{
				CMonster* pMonster = (CMonster*)OBJECTMGR->GetObject(pmsg->dwObjectID);

				if( pMonster )
				{
					OBJECTMGR->ShowChatBalloon( pMonster, temp);
				}
			}
			break;
		case 1:	//��ǳ�� + ä��â
			{
				CMonster* pMonster = (CMonster*)OBJECTMGR->GetObject(pmsg->dwObjectID);
				if( pMonster )
				{
					OBJECTMGR->ShowChatBalloon( pMonster, temp);
				}


				AddMsg( CTC_MONSTERSPEECH, "%s: %s", pMonster->GetSInfo()->Name, temp );
			}
			break;
		case 2:	//ä�� ��ġ��
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
	memcpy( &m_ChatOption, pChatOption, sizeof(sChatOption) ) ;								// ä�� �Ŵ����� ��� �ɼǿ� ���� �ɼ��� �޴´�.
}

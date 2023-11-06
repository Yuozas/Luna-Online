#pragma once

#include "ChattingDlg.h"

#define CHATMGR USINGTON(CChatManager)
//#define CHATMGR CChatManager::GetInstance()
#define MAX_CONVERSATION_MSGNUM	2000

// 070928 LYW --- ChatManager : Define whisper name count and size.
#define MAX_WNAME_COUNT	3
#define MAX_WNAME_SIZE	16

enum eTextClass
{
	CTC_OPERATOR,		
	CTC_GENERALCHAT,	
	CTC_PARTYCHAT,		
	CTC_GUILDCHAT,		
	CTC_ALLIANCE,		
	CTC_SHOUT,			
	CTC_TIPS,			
	CTC_WHISPER,				
	CTC_MONSTERSPEECH,
	CTC_MONSTERSHOUT,
	CTC_ATTACK,			
	CTC_ATTACKED,		
	CTC_DEFENCE,		
	CTC_DEFENCED,		
	CTC_KILLED,			
	CTC_MPWARNING,		
	CTC_HPWARNING,		
	CTC_GETITEM,		
	CTC_GETMONEY,		
	CTC_GETEXP,
	CTC_SYSMSG,			
	CTC_TOGM,
	CTC_TOWHOLE,
	CTC_TOPARTY,
	CTC_TOGUILD,
	CTC_CHEAT_1,
	CTC_CHEAT_2,
	CTC_CHEAT_3,
	CTC_CHEAT_4,
	CTC_CHEAT_5,
	CTC_QUEST,
	CTC_GMCHAT,
	CTC_BILLING,	
	CTC_TRACKING,
	CTC_GTOURNAMENT,
	CTC_JACKPOT,
	CTC_GUILD_NOTICE,
	CTC_WHOLE,
	CTC_FAMILY, 
	CTC_ARMY, 
	CTC_TRADE, 
	CTC_NOTICE, 
	CTC_EMOTICON,
	CTC_TOFAMILY,
	CTC_BUYITEM,		
	CTC_SELLITEM,
	CTC_OPERATEITEM,
	CTC_EXPENDEDITEM,
	CTC_OPERATOR2,
	CTC_COUNT,	
};

const DWORD dwChatTextColor[CTC_COUNT] = {

	RGBA_MAKE(0, 255, 60, 255),			//CTC_OPERATOR
		RGBA_MAKE(255, 255, 255, 255),		//CTC_GENERAL
		RGBA_MAKE(170, 245, 103, 255),		//CTC_PARTYCHAT
		RGBA_MAKE(255, 207, 59, 255),		//CTC_GUILDCHAT
		RGBA_MAKE(240, 247, 55, 255),		//CTC_ALLIANCE
		RGBA_MAKE(255, 255, 10, 255),		//CTC_SHOUT
		RGBA_MAKE(2, 255, 163, 255),		//CTC_TIPS
		RGBA_MAKE(255, 165, 165, 255),		//CTC_WHISPER
		RGBA_MAKE(255, 255, 255, 255),		//CTC_MONSTERSPEECH
		RGBA_MAKE(247, 182, 24, 255),		//CTC_MONSTERSHOUT
		RGBA_MAKE(255, 97, 19, 255),		//CTC_ATTACK
		RGBA_MAKE(255, 97, 19, 255),		//CTC_ATTACKED
		RGBA_MAKE(255, 97, 19, 255),		//CTC_DEFENCE
		RGBA_MAKE(255, 97, 19, 255),		//CTC_DEFENCED
		RGBA_MAKE(255, 97, 19, 255),		//CTC_KILLED
		RGBA_MAKE(255, 52, 19, 255),		//CTC_MPWARNING
		RGBA_MAKE(255, 52, 19, 255),		//CTC_HPWARNING
		RGBA_MAKE(2, 255, 163, 255),		//CTC_GETITEM
		RGBA_MAKE(255, 255, 10, 255),		//CTC_GETMONEY
		//RGBA_MAKE(10, 10, 10, 255),			//CTC_GETEXP
		//RGBA_MAKE(94, 94, 94, 255),			//CTC_SYSMSG
		RGBA_MAKE(255, 255, 255, 255),			//CTC_GETEXP
		RGBA_MAKE(255, 255, 255, 255),			//CTC_SYSMSG
		RGBA_MAKE(82, 255, 204, 255),		//CTC_TOGM
		RGBA_MAKE(0, 0, 100, 255),			//CTC_TOWHOLE
		RGBA_MAKE(38, 143, 124, 255),		//CTC_TOPARTY
		RGBA_MAKE(0, 114, 19, 255),			//CTC_TOGUILD
		RGBA_MAKE(255, 0, 0, 255),			//CTC_CHEAT_1
		RGBA_MAKE(255, 255, 0, 255),		//CTC_CHEAT_2
		RGBA_MAKE(255, 0, 255, 255),		//CTC_CHEAT_3
		RGBA_MAKE(0, 255, 255, 255),		//CTC_CHEAT_4
		RGBA_MAKE(100, 50, 100, 255),		//CTC_CHEAT_5
		RGBA_MAKE( 0, 255, 255, 255 ),		//CTC_QUEST
		RGBA_MAKE( 28, 233, 151, 255 ),		//CTC_GMCHAT 
		RGBA_MAKE(0, 255, 60, 255),			//CTC_BILLING
		RGBA_MAKE(255, 20, 20, 255),		//CTC_TRACKING
		RGBA_MAKE(20, 255, 60, 255),		//CTC_GTOURNAMENT
		RGBA_MAKE(144, 252, 222, 255),		//CTC_JACKPOT
		RGBA_MAKE(255, 214, 0, 255),		//CTC_GUILD_NOTICE
		RGBA_MAKE(100, 100, 100, 255),		// WHOLE
		RGBA_MAKE(255, 241, 143, 255),		// FAMILY
		RGBA_MAKE(255, 114, 19, 255),		// ARMY
		RGBA_MAKE(183, 239, 255, 255),		// TRADE
		RGBA_MAKE(236, 236, 0, 255),		// NOTICE
		RGBA_MAKE(255, 255, 255, 255),		// EMOTICON
		RGBA_MAKE(183, 239, 255, 255),		// CTC_TOFAMILY
		RGBA_MAKE(255, 204, 0, 255),		// CTC_BUYITEM
		RGBA_MAKE(255, 204, 0, 255),		// CTC_SELLITEM
		RGBA_MAKE(200, 215, 170, 255),		// CTC_OPERATEITEM
		RGBA_MAKE(215, 215, 215, 255),		// CTC_EXPENDEDITEM
};

enum eChatOption
{
	/*CTO_NOSYSMSG,
	CTO_NOITEMMSG,
	CTO_NOEXPMSG,
	CTO_NOCHATMSG,
	CTO_NOSHOUTMSG,
	CTO_NOGUILDMSG,
	CTO_NOALLIANCEMSG,	
	CTO_NOPARTYMSG,
	CTO_NOFAMILYMSG,
	CTO_COUNT,*/

	CTO_NOWHISPERMSG,
	CTO_NOCHATMSG, 
	CTO_NOSHOUTMSG, 
	CTO_NOPARTYMSG,
	CTO_NOFAMILYMSG,
	CTO_NOGUILDMSG,
	CTO_NOALLIANCEMSG,
	CTO_NOSYSMSG,
	CTO_NOEXPMSG,
	CTO_NOITEMMSG,
	CTO_COUNT,
};

struct sChatOption
{
	BOOL bOption[CTO_COUNT];
};
#define MAX_WHISPERLIST_PER_SIDE	5

enum eCHAT_DELAY
{
	eCHATDELAY_PARTY,
	eCHATDELAY_GUILD,
	eCHATDELAY_UNION,
	eCHATDELAY_SHOUT,
	eCHATDELAY_WHISPER,
	eCHATDELAY_MAX,
};

struct sEMOTION
{
	int nEmotionNum;
	int nLen;
	char buf[MAX_NAME_LENGTH+1];

};

// 070910 LYW --- ChatManager : Add struct for command.
struct sACTION_COMMAND
{
	int index ;

	char string[32] ;

	sACTION_COMMAND()
	{
		index = 0 ;
		memset(string, 0, sizeof(string)) ;
	}
} ;

struct sFACEINFO
{
	int nIndex ;

	char szDescript[32] ;
	char szCommand[32] ;

	int nFaceNum ;

	int nLen ;
} ;

class CUserCommendParser;

class CChattingDlg ;

class CChatManager
{
protected : 
	CChattingDlg*	m_pChattingDlg ;

	char*			m_pChatMsg[MAX_CONVERSATION_MSGNUM];
	//char			m_NULL;	//채팅 메세지가 없는 번호일경우 리턴하기위한 템프멤버

	//// 070608 LYW --- ChatManager : Delete emotion filter.
	//cPtrList		m_EmotionList ;
	cPtrList		m_FaceCommandList ;

	// 070910 LYW --- ChatManager : Add action command list.
	cPtrList		m_ActionCommandList ;

	DWORD			m_dwLastChatTime;	//모든 채팅(채팅간전환포함)

	cPtrList		m_ChatList;
	cPtrList		m_AssistList ;
	cPtrList		m_ShoutList;

	sChatOption		m_ChatOption ;

	// 070928 LYW --- ChatManager : Add whisper name array.
	char whisperName[MAX_WNAME_COUNT][MAX_WNAME_SIZE] ;

public:
	CChatManager(void);
	virtual ~CChatManager(void);

	void Init() ;
	void Release() ;

	// CONVERSATINO DIALOG PART.
	void SetChattingDlg( CChattingDlg* dlg ) { m_pChattingDlg = dlg ; }
	CChattingDlg* GetChattingDlg() { return m_pChattingDlg ; }

	// MESSAGE PART.
	void LoadChatMsg();
//	static CChatManager* GetInstance();
	char* GetChatMsg( int nMsgNum );

	void AddMsg( int nClass, char* str, ... ) ;

	//// 070608 LYW --- Chatmanager : Delete emotion filter.
	//// EMOTION PART.
	//void LoadEmotionFilter() ;
	//int GetEmotionNum( char* str ) ;
	//void ReleaseEmotionList() ;

	// FACE PART.
	void LoadFaceCommandList() ;

	// 070910 LYW --- ChatManager : Add function to load action command.
	void LoadActionCommandList() ;

	int GetFaceNum( const TCHAR* str ) const;

	// MESSAGE PROC.
	void MsgProc( UINT msg, WPARAM wParam ) ;

	// NETWORK PART.
	void NetworkMsgParse( BYTE Protocol, void* pMsg );

	void Chat_Normal( void* pMsg ) ;
	void Chat_Party( void* pMsg ) ;
	void Chat_Family( void* pMsg ) ;
	void Chat_Guild( void* pMsg ) ;
	void Chat_Alliance( void* pMsg ) ;
	void Chat_Trade( void* pMsg ) ;
	void Chat_System( void* pMsg ) ;
	void Chat_Whole( void* pMsg ) ;
	void Chat_Whisper( void* pMsg ) ;
	void Chat_Whisper_Ack( void* pMsg ) ;
	void Chat_Whisper_Nack( void* pMsg ) ;
	void Chat_Monster_Speech( void* pMsg ) ;
	//080312 NYJ --- 전체외치기 추가
	void Chat_Shout_Ack(void*pMsg);
	void Chat_Shout_Nack(void*pMsg);

	void SaveChatList() ;
	void RestoreChatList() ;
	void ReleaseChatList() ;

	void OnInputChatMsg( char* str ) ;
	BOOL CanChatTime( char cToken ) ;

	DWORD GetMsgColor(BYTE msgIdx) { return dwChatTextColor[msgIdx] ; }

	void SetOption( sChatOption* pChatOption ) ;

	cPtrList* GetCommandList() { return &m_ActionCommandList ; }

	// 070928 LYW --- Add function to setting whisper name to chatting manager.
	void AddWhisperName(char* name) ;
	
	// 070928 LYW --- Add function to return whisper name by selected index.
	char* GetWhisperName(int nIdx) ;

	// 071001 LYW --- Add function to initialize whisper buffer.
	void InitializeWhisperName() ;
};

EXTERNGLOBALTON(CChatManager)

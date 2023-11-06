#pragma once
#include ".\interface\cdialog.h"

#define MAX_TEXTLEN			58
#define MAX_TEMPBUFF		256
#define MAX_LIFE_ACTION		10

#define WHOLE_COMMAND		264
#define WHISPER_COMMAND		281
#define ACTION_COMMAND		487
#define EM_COMMAND			285


enum CONVERSATION
{
	e_CONVERSATION_ALL, 
	e_CONVERSATION_PARTY, 
	e_CONVERSATION_FAMILY, 
	e_CONVERSATION_GUILD, 
	e_CONVERSATION_ALLIANCE, 
	e_CONVERSATION_TRADE, 
	e_CONVERSATION_SYSTEM, 
	e_CONVERSATION_ASSISTANCE,
	e_CONVERSATION_MAX,
} ;

enum SEND_MSG_TYPE
{
	e_SMT_ALL, 
	e_SMT_PARTY, 
	e_SMT_FAMILY, 
	e_SMT_GUILD, 
	e_SMT_ALLIANCE, 
	e_SMT_TRADE, 
	e_SMT_WHOLE, 
	e_SMT_WHISPER, 
	e_SMT_WHISPER2, // /id MSG.
	e_SMT_MAX,
} ;

struct sWHISPER_NAME
{
	char name1[MAX_NAME_LENGTH] ;
	char name2[MAX_NAME_LENGTH] ;
	char name3[MAX_NAME_LENGTH] ;

	BYTE byIndex ;
	BYTE byCount ;
} ;

class cButton ;
class cEditBox ;
class cListDialog ;
class cPushupButton ;
class cStatic ;

//class CChatting : public cDialog
//{
//protected : 
//	BYTE			m_byClass ;					// 분류
//
//	BOOL			m_bMoved ;
//
//	cPushupButton*	m_pButton ;					// 버튼
//	cListDialog*	m_pListDlg ;				// 리스트
//
//	// COMMAND NUMBER
//	int				m_nCommandNum[3] ;
//
//public :
//	CChatting(void) ;
//	virtual ~CChatting(void) ;
//
//	void SetActiveDlg( BOOL bActive, BOOL bExtend ) ;
//
//	virtual DWORD ActionEvent( CMouse* mouseInfo ) ;
//
//	cListDialog* GetListDlg() { return m_pListDlg ; }
//	cPushupButton* GetTitleButton() { return m_pButton ; }
//	void SetTitleButton( cPushupButton* pButton ) { m_pButton = pButton ; }
//
//	void SetClass( BYTE byClass ) { m_byClass = byClass ; }
//	BYTE GetClass() { return m_byClass ; }
//
//	BOOL IsSameCommand( char* command ) ;
//	void SetCommandNum( int nNum1, int nNum2, int nNum3 ) { m_nCommandNum[0] = nNum1 ; m_nCommandNum[1] = nNum2 ; m_nCommandNum[2] = nNum3 ; }
//	int GetCommandNum( BYTE idx ) { return m_nCommandNum[idx] ; }
//
//	void AddMsg( char* msg, DWORD dwColor ) ;
//
//	void SetExtend( BOOL bExtend ) ;
//
//	void Add( cWindow* controler ) ;
//
//	BOOL IsMoved() { return m_bMoved ; }
//} ;
//
//class CChattingDlg : public cDialog
//{
//protected : 
//	CChatting*	m_pConversationDlg[e_CONVERSATION_MAX] ;		// 대화창
//	CChatting*	m_pAssistanceDlg ;				// 보조 대화창
//
//	cEditBox*		m_pInputBox ;					// 대화 입력창
//	BOOL			m_bFocused ;
//	
//	cButton*		m_pExtendBtn ;					// 확대 축소 버튼
//
//	cStatic*		m_pBGStatic ;
//	cStatic*		m_pBGExtendUp ;
//	cStatic*		m_pBGExtendDown ;
//
//	BYTE			m_byCurConversation ;
//	BYTE			m_byCurAssistance ;
//
//	BYTE			m_byCurCommand ;
//
//	BOOL			m_bExtened ;
//
//	// WHISPER NAME PART.
//	sWHISPER_NAME	m_SenderName ;
//	sWHISPER_NAME	m_ReceiverName ;
//
//	int				m_nNameIndex ;
//
//	char			m_szWhisperName[128] ;
//	char			m_szSendMsg[128] ;
//
//public:
//	CChattingDlg(void);
//	virtual ~CChattingDlg(void);
//
//	virtual DWORD ActionEvent( CMouse* mouseInfo ) ;
//	virtual DWORD ActionKeyboardEvent( CKeyboard* keyInfo ) ;
//	void OnActionEvent( LONG lId, void* p, DWORD we ) ;
//
//	CChatting* GetChatting(int idx) { return m_pConversationDlg[idx] ; }
//	CChatting* GetAssist() { return m_pAssistanceDlg ; }
//
//	cEditBox* GetInputBox() {  return m_pInputBox ; }
//
//	void Linking() ;
//
//	void SetCurConversation( BYTE byClass ) ;
//
//	void ExtendDlg() ;
//	void ResetAssist() ;
//
//	void AddMsg( BYTE byClass, char* msg, DWORD dwColor ) ;
//
//	void CallAssistanceDlg( BYTE byClass ) ;
//
//	void SetCommandToInputBox() ;
//
//	// CONVERSATION STRING PART.
//	void InspectMsg() ;
//	void FilteringMsg(char* str) ;
//	void InspectCommand( char* str ) ;
//	void FindUserName( char* name, char* msg ) ;
//
//	BOOL Check_LifeAction( char* str ) ;
//	BOOL Check_EmotionCommand( char* str ) ;
//	BOOL Check_EmotionWord( char* str ) ;
//
//	// SEND MESSAGE PART.
//	void SendMsg() ;
//
//	void Msg_All() ;
//	void Msg_Party() ;
//	void Msg_Family() ;
//	void Msg_Guild() ;
//	void Msg_Alliance() ;
//	void Msg_Trade() ;
//	void Msg_Whole() ;	
//	void Msg_Whisper() ;
//
//	// WHISPER NAME PART.
//	char* GetWhisperName( int nameIdx ) ;
//
//	void AddSenderName( char* name ) ;
//	void AddReceiverName( char* name ) ;
//
//	void UpdateSenderName( char* name ) ;
//	void UpdateReceiverName( char* name ) ;
//
//	// 070528 LYW --- ChattingDlg : Add function to return emoticon number.
//	int GetEmoticonNum(char* str) ;
//};

class CChattingDlg : public cDialog
{
	cListDialog*	m_pChatList ;
	cListDialog*	m_pAssistList ;
	cListDialog*	m_pShoutList;

	cEditBox*		m_pInputBox ;

	cButton*		m_pExtendBtn ;
	cButton*		m_pReductionBtn ;
	cButton*		m_pCloseBtn ;
	cPushupButton*	m_pTabButton[e_CONVERSATION_MAX] ;

	// 080421 LYW --- ChattingDlg : 채팅방 메인창 호출 컨트롤 추가.
	cButton*		m_pToggleChatRoom ;

	BYTE			m_byCurConversation ;
	BYTE			m_byCurAssistance ;

	BYTE			m_byCurCommand ;

	BOOL			m_bExtened ;

	// WHISPER NAME PART.
	sWHISPER_NAME	m_SenderName ;
	sWHISPER_NAME	m_ReceiverName ;

	int				m_nNameIndex ;

	char			m_szWhisperName[128] ;
	char			m_szSendMsg[128] ;

public:
	CChattingDlg(void);
	virtual ~CChattingDlg(void);

	void Linking() ;

	virtual DWORD ActionEvent( CMouse* mouseInfo ) ;
	virtual DWORD ActionKeyboardEvent( CKeyboard* keyInfo ) ;

	BOOL Event_LButtonClick(CMouse* mouseInfo) ;
	void Event_RButtonClick(CMouse* mouseInfo) ;
	void Event_PushDown(CMouse* mouseInfo) ;

	cEditBox* GetInputBox() { return m_pInputBox ; }

	void SetCommandToInputBox() ;

	// CONVERSATION STRING PART.
	void InspectMsg(char* str) ;
	void FilteringMsg(char* str) ;
	void InspectCommand( char* str ) ;
	void FindUserName( char* name, char* msg ) ;

	BOOL Check_FaceCommand( char* str ) ;
	BOOL Check_LifeAction( char* str ) ;
	BOOL Check_EmotionCommand( char* str ) ;
	BOOL Check_EmotionWord( char* str ) ;

	// SEND MESSAGE PART.
	void SendMsg() ;
	
	void Msg_All() ;
	void Msg_Party() ;
	void Msg_Family() ;
	void Msg_Guild() ;
	void Msg_Alliance() ;
	void Msg_Trade() ;
	void Msg_Whole() ;	
	void Msg_Whisper() ;

	// 070928 LYW --- ChattingDlg : Delete this function.
	// WHISPER NAME PART.
	//char* GetWhisperName( int nameIdx ) ;

	// 070928 LYW --- ChattingDlg : Delete this function.
	//void AddSenderName( char* name ) ;
	//void AddReceiverName( char* name ) ;

	void UpdateSenderName( char* name ) ;
	void UpdateReceiverName( char* name ) ;

	int GetEmoticonNum(char* str) ;

	int GetCommand() ;

	void AddMsg( BYTE byClass, char* msg, DWORD dwColor ) ;
	void AddShoutMsg(char* msg, DWORD dwColor);

	void ExtendDlg() ;
	void ReductionDlg() ;
	void ResetAssist() ;
	void CallAssistanceDlg( BYTE byClass ) ;
	void CallShoutDlg();

	DWORD GetCurMsgColor(BYTE msgType) ;

	cListDialog* GetChatListDlg() { return  m_pChatList ; }
	cListDialog* GetAssistListDlg() { return m_pAssistList ; }
	cListDialog* GetShoutListDlg() {return m_pShoutList;}

	// 080122 LYW --- ChattingDlg : 채팅 창의 컨트롤들 상에 마우스가 있는지 체크하는 함수 추가.
	BOOL IsPtInControls(CMouse* mouseInfo) ;
} ;

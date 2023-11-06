#pragma once
//=================================================================================================
//	FILE		: CChatRoomMgr.h
//	DESC		: ä�ù� �ý����� �����ϴ� �Ŵ��� Ŭ����.
//	PROGRAMMER	: Yongs Lee
//	DATE		: APRIL 3, 2008
//=================================================================================================





//-------------------------------------------------------------------------------------------------
//		Define some macroinstruction.
//-------------------------------------------------------------------------------------------------
#define CHATROOMMGR USINGTON(CChatRoomMgr) // Ŭ���� �̸��� �����̸����� �����, ������ ��ȯ.

// �� ��ũ�θ� ����ϸ�, �޽��� �ڽ��� ������ ����Ѵ�.
#define _MB_CRMGR_

//#define SECRET_CODE_SIZE	4+1												// ��й�ȣ ������ ����.

enum ENUM_PRINT_ERRORS														// �������� ���Ǵ� ���� �޽��� ����.
{
	err_IMP = 0,															// �Լ��� �Ķ���Ͱ� �߸��Ǿ���.
	err_ICCTLS,																// ���� ��Ʈ���� ������ �ٸ��� �ʴ�.
	err_FCMTB,																// �⺻ �޽����� ��ȯ�� �����Ͽ���.
	err_FCMTO,																// ���� �޽����� ��ȯ�� �����Ͽ���.
} ;

enum ENUM_CHATROOM_MSGTYPE													// ä�ù濡 ��� �� �޽��� Ÿ�� ����.
{
	e_ChatMsg_StepIn = 0,													// ä�ù� ����.
	e_ChatMsg_StepOut,														// ä�ù� ����.
	e_ChatMsg_MyMsg,														// ���� �޽���.
	e_ChatMsg_OtherMsg,														// �ٸ� ����� �޽���.
	e_ChatMsg_System,														// ä�ù� �� �ý��� �޽���.
	e_ChatMsg_Whisper_Sener,												// �Ӹ� �������.
	e_ChatMsg_Whisper_Receiver,												// �Ӹ� �޴»��.
} ;

enum ENUM_CHATROOM_HERO_STATE												// HERO�� ä�� ���� ���� ����.
{
	e_State_Away_From_Chatroom = 0,											// ä���� �� �غ� ���� ���� ����.
	e_State_In_Lobby ,														// ������ HERO ���� ��� �� ����.
	e_State_In_ChatRoom,													// ä�ù濡 �������� ����.
} ;

enum ENUM_CR_GUEST_COUNT													// ä�ù� �ִ� ������ ����.
{
	e_GC_20,																// 20��.
	e_GC_15,																// 15��.
	e_GC_10,																// 10��.
	e_GC_5,																	// 5��.

	e_GC_Max,
} ;

enum ENUM_CR_MAIN_TYPE
{
	e_RTM_AllLooK = 0,														// ��ü����.
	e_RTM_Party ,															// ��Ƽ����.
	e_RTM_Deal,																// ��ǰ�ŷ�.
	e_RTM_Normal_Chat,														// �Ϲ�ä��.
	e_RTM_Local_Chat,														// ����ä��.
	e_RTM_Make_Friend,														// ģ��ã��.
	e_RTM_Searched,															// �˻����

	e_RTM_Max,
} ;

enum ENUM_CR_ROOM_MODE														// ä�ù� ����/�����.
{	
	e_RM_Open = 0 ,															// ����.
	e_RM_Close,																// �����.

	e_RM_Max,
} ;

enum ENUM_CR_GUEST_STATE													// ä�ù� ������ ����.
{
	e_GS_Owner,																// ����.
	e_GS_Guest,																// �մ�.
	e_GS_Freezed,															// ����.
} ;

enum ENUM_CR_ROOM_TYPE														// ä�ù� �з� ����.
{
	//e_RT_AllLooK = 0,														// ��ä����.
	e_RT_Party,																// ��Ƽ����.
	e_RT_Deal,																// ��ǰ�ŷ�.
	e_RT_Normal_Chat,														// �Ϲ�ä��.
	e_RT_Local_Chat,														// ����ä��.
	e_RT_Make_Friend,														// ģ��ã��.
	//e_RT_Make_Searched,														// �˻� �� ������.

	e_RT_Max,
} ;





//-------------------------------------------------------------------------------------------------
//		Include part.
//-------------------------------------------------------------------------------------------------
class CChatRoomCreateDlg ;
class CChatRoomDlg ;
class CChatRoomGuestListDlg ;
class CChatRoomJoinDlg ;
class CChatRoomMainDlg ;
class CChatRoomOptionDlg ;
//class cTextArea ;
class cEditBox ;






//-------------------------------------------------------------------------------------------------
//		The class CChatRoomMgr.
//-------------------------------------------------------------------------------------------------
class CChatRoomMgr
{
	BYTE						m_byChatting ;									// HERO�� ä�������� ���θ� ���� ����.

	CChatRoomCreateDlg*			m_pCreateDlg ;									// ä�ù� ����â.
	CChatRoomDlg*				m_pChatRoomDlg ;								// ä��â.
	CChatRoomGuestListDlg*		m_pGuestListDlg ;								// ä�ù� ��ȭ ��� ���â.
	CChatRoomJoinDlg*			m_pJoinDlg ;									// ä�ù� ����â.
	CChatRoomMainDlg*			m_pMainDlg ;									// ä�ù� ���â.
	CChatRoomOptionDlg*			m_pOptionDlg ;									// ä�ù� �ɼ�â.

	ST_CR_ROOM_SRV				m_MyRoom ;										// HERO�� ä�ù� ����.
	cPtrList					m_BackupList ;									// �� �̵��� ä�ó����� ������ ����Ʈ.
	cPtrList					m_GuestList ;									// HERO�� ä�ù� �����ڸ� ���� ����Ʈ.

	DWORD						m_dwLastMyChat ;								// ä�� �޽����� ������ �ð��� ���� ����.
	DWORD						m_dwLastMyComTime ;								// ä�ù� ����� ����� �ð��� ���� ����.

	DWORD						m_dwFreezedID[MAX_USER_PER_ROOM] ;				// ä�ù� �޽��� ���� ���̵� ���� �迭.

	BYTE						m_byRequestFriend ;								// ģ����û�� �� �������� ��� ����.

	// �ɼ� ������ ���� ������.
	BYTE					m_byOptSavedMode ;									// ����/����� ��� ����.
	char					m_szOptSavedCode[SECRET_CODE_SIZE+1] ;				// ��й�ȣ�� ���� ���ڿ�.
	BYTE					m_byOptSavedGuestCount ;							// �ִ� ������ �� Ÿ�� ����.
	BYTE					m_byOptSavedRoomType ;								// ä�ù� �з� ����.
	char					m_szOptSavedTitleBuff[128+1] ;						// ������ ���� ���ڿ�.

public:
	CChatRoomMgr(void) ;														// ������ �Լ�.
	virtual ~CChatRoomMgr(void)	;												// �Ҹ��� �Լ�.

	// ä�ù� ���� �������̽� ������ ���� �Լ�.
	void SetCreateDlg(CChatRoomCreateDlg* pDlg)			{ m_pCreateDlg = pDlg ; }
	void SetChatRoomDlg(CChatRoomDlg* pDlg)				{ m_pChatRoomDlg = pDlg ; }
	void SetGuestListDlg(CChatRoomGuestListDlg* pDlg)	{ m_pGuestListDlg = pDlg ; }
	void SetJoinDlg(CChatRoomJoinDlg* pDlg)				{ m_pJoinDlg = pDlg ; }
	void SetMainDlg(CChatRoomMainDlg* pDlg)				{ m_pMainDlg = pDlg ; }
	void SetOptionDlg(CChatRoomOptionDlg* pDlg)			{ m_pOptionDlg = pDlg ; }

	void ToggleChatRoomMainDlg() ;						// ä�ù� ����Ʈ ��/�ݱ� �Լ�.

	void ReleaseGuestList() ;							// HERO�� ä�ù� ��������Ʈ�� ���� �Լ�.

	// �ɼ� ���� ������ ����/��ȯ �Լ���.
	//void SetOptSavedMode(BYTE byMode) ;
	BYTE GetOptSavedMode() { return m_byOptSavedMode ; }
	//void SetOptSecretCode(char* pCode) ;
	char* GetOptSecretCode() { return m_szOptSavedCode ; }
	//void SetOptTotalGuest(BYTE byGuestCount) ;
	BYTE GetOptTotalGuest() { return m_byOptSavedGuestCount ; }
	//void SetOptRoomType(BYTE byRoomType) ;
	BYTE GetOptRoomType() { return m_byOptSavedRoomType ; }
	//void SetOptRoomTitle(char* pTitle) ;
	char* GetOptRoomTitle() { return m_szOptSavedTitleBuff ; }

	void SetOption(ST_CR_ROOM_SRV* pInfo) ;

public :
	void NetworkMsgParser(void* pMsg) ;					// ��Ʈ��ũ �޽��� �Ľ� �Լ�.

	void Add_User_Ack(void* pMsg) ;						// �������� ��� ���� ó�� �Լ�.

	void Change_Option_Nack(void* pMsg) ;				// �ɼǺ��� ���� ó�� �Լ�.
	void Change_Option_Notice(void* pMsg) ;				// �ɼǺ��� ���� �Լ�.

	void Change_Owner_Nack(void* pMsg) ;				// �������� ���� ó�� �Լ�.
	void Change_Owner_Notice(void* pMsg) ;				// �������� ���� �Լ�.

	void Chatmsg_Normal_Nack(void* pMsg) ;				// ä�� �޽��� ���� ���� �Լ�.
	void Chatmsg_Normal_Notice(void* pMsg) ;			// ä�� �޽��� ��� �Լ�.

	void Create_Room_Ack(void* pMsg) ;					// ä�ù� ���� ���� �Լ�.
	void Create_Room_Nack(void* pMsg) ;					// ä�ù� ���� ���� �Լ�.

	void Join_Room_Ack(void* pMsg) ;					// ä�ù� ���� ���� �Լ�.
	void Join_Room_Nack(void* pMsg) ;					// ä�ù� ���� ���� �Լ�.
	void Join_Room_Notice(void* pMsg) ;					// ä�ù� ���� ���� �Լ�.

	void Kick_Guest_Ack(void* pMsg) ;					// �������� ���� ó�� �Լ�.
	void Kick_Guest_Nack(void* pMsg) ;					// �������� ���� �Լ�.
	void Kick_Guest_Notice(void* pMsg) ;				// �������� ���� �Լ�.

	void Out_Room_Ack(void* pMsg) ;						// ä�ù� ������ ���� �Լ�.
	void Out_Room_Nack(void* pMsg) ;					// ä�ù� ������ ���� �Լ�.
	void Out_Room_EmptyRoom(void* pMsg) ;				// ä�ù��� ������ ��, ä�ù��� �ϳ��� ���� ��, ó���ϴ� �Լ�.
	void Out_Room_Last_Man(void* pMSg) ;				// ä�ù��� ���ö�, ������ ����̾��� ��� ó���Լ�.
	void Out_Room_Notice(void* pMsg) ;					// ä�ù� ������ ���� �Լ�.
	void Out_Room_Change_Owner_Notice(void* pMsg) ;		// ������ ���������� ��� ������ ä�ù��� ������ ��, ���� ������ �����ϴ� �Լ�.

	void Request_Friend_Nack(void* pMsg) ;				// ģ����û ���� �Լ�.

	void RoomList_Ack(void* pMsg) ;						// ä�ù� ����Ʈ �ޱ� ���� �Լ�.
	void RoomList_Nack(void* pMsg) ;					// ä�ù� ����Ʈ �ޱ� ���� �Լ�.
	
	void SearchName_Ack(void* pMsg) ;					// �̸� �˻� ���� �Լ�.
	void SearchName_Nack(void* pMsg) ;					// �̸� �˻� ���� �Լ�.

	void SearchTitle_Ack(void* pMsg) ;					// ä�ù� ���� �˻� ���� �Լ�.
	void SearchTitle_Nack(void* pMsg) ;					// ä�ù� ���� �˻� ���� �Լ�.

	// ä�ù� ����Ʈ ���� ������Ʈ �Լ���.
	void UpdateInfo_Created_Room(void* pMsg) ;			// ä�ù� �߰� ó��.
	void UpdateInfo_Deleted_Room(void* pMsg) ;			// ä�ù� ���� ó��.
	void UpdateInfo_SecretMode(void* pMsg) ;			// ����/����� ������Ʈ ó��.
	void UpdateInfo_RoomType(void* pMsg) ;				// ä�ù� �з� ������Ʈ ó��.
	void UpdateInfo_Title(void* pMsg) ;					// ä�ù� ���� ������Ʈ ó��.
	void UpdateInfo_Cur_GuestCount(void* pMsg) ;		// ���� ������ �� ������Ʈ ó��.
	void UpdateInfo_Total_GuestCount(void* pMsg) ;		// �ִ� ������ �� ������Ʈ ó��.

public :
	// HERO ä�� ���� ����/��ȯ.
	void Set_ChatRoomState(BYTE byEnum) {m_byChatting = byEnum ; }
	BYTE Get_ChatRoomState() { return m_byChatting ; }

	// ä�ó��� ����/���� �Լ���.
	void SaveChat() ;				// ä�� ����.
	void RestoreChat() ;			// ä�� ����.
	void ReleaseBackupList() ;		// ���� ����Ʈ ����.

	// HERO�� ä�ù� ����/���� ���� ��ȯ �Լ�.
	DWORD GetMyRoomOwnerIdx() { return m_MyRoom.dwOwnerIdx ; }
	ST_CR_ROOM_SRV* GetMyRoom() { return &m_MyRoom ; }

	// ä�ù� ��, �޽����� �߰��ϴ� �Լ�.
	void AddChatRoomMsg(char* pMsg, BYTE byType) ;

	// ä�ù� �� ä�� �޽����� �Է��ϴ� ����Ʈ �ڽ� ��ȯ �Լ�.
	//cTextArea* GetChatRoomTextArea() ;
	cEditBox* GetChatRoomInputBox() ;

	// �޽��� ���ν��� �Լ� - Ű���� �Է� ó���� ��.
	void MsgProc() ;

	//BOOL IsWhisperMsg() ;
	//void SendNormalMsg() ;
	//void SendWhisperMsg() ;

	// HERO�� ������ ä�ñ�� ��� �ð��� ������Ʈ/��밡���� �������� ��ȯ�ϴ� �Լ�.
	void UpdateMyLastComTime() ;
	BYTE IsEnableExecuteCom() ;

	// HERO�� ������ ä�ýð��� ������Ʈ/��ȯ�ϴ� �Լ�.
	void UpdateChatLastTime(DWORD dwTime) { m_dwLastMyChat = dwTime ; }
	DWORD GetChatListTime() { return m_dwLastMyChat ; }

	BOOL IsFreezed(DWORD dwPlayerID) ;					// ������ ����� ���� �� ������� üũ�ϴ� �Լ�.
	void FreezeUser(DWORD dwPlayerID) ;					// ������ ����� �����ϴ� �Լ�.
	void UnFreezeUser(DWORD dwPlayerID) ;				// ������ ����� ������ �����ϴ� �Լ�.
	void ClearFreeze() ;								// ���� ����� ��� �����ϴ� �Լ�.
	//void DeleteFreezeUser(DWORD dwPlayerID) ;

	// HERO�� ģ����û�� �� ���·� �����ϴ� �Լ�.
	void SetRequestFriendState(BYTE byState) { m_byRequestFriend = byState ; }
	// HERO�� ģ����û�� �� �������� ��ȯ�ϴ� �Լ�.
	BYTE IsRequestedFriend() { return m_byRequestFriend ; }
	// HERO�� ä�ù��� �����ڸ� �����ϴ� ����Ʈ�� ��ȯ�ϴ� �Լ�.
	cPtrList* GetGuestList() { return &m_GuestList ; }

public :
	// ���� ó���� �ϴ� �Լ���.
	void Throw_Error(BYTE errType, char* szCaption) ;	// ����Ÿ������ ���� ó���� �Ѵ�.
	void Throw_Error(char* szErr, char* szCaption) ;	// ���� �޽����� ���� ó���� �Ѵ�.
};

EXTERNGLOBALTON(CChatRoomMgr)							// Extern class name to global class.

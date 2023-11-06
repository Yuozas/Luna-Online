#pragma once
//=================================================================================================
//	FILE		: CChatRoomMgr.h
//	DESC		: ä�ù� �ý��۰� ���� �� �޽������� ������ �����ϴ� �Ŵ��� Ŭ����.
//	DATE		: MARCH 31, 2008 LYW
//=================================================================================================





//-------------------------------------------------------------------------------------------------
//		Definition part.
//-------------------------------------------------------------------------------------------------
#define CHATROOMMGR	CChatRoomMgr::GetInstance()

//#define _USE_ERR_MSGBOX_		// �� ��ũ�θ� ����ϸ�, �޽��� �ڽ��� ������ ����ϰ�, ������� ������ �α׸� ����.

enum ENUM_PRINT_ERRORS
{
	err_IMP = 0,				// �Լ��� �Ķ���Ͱ� �߸��Ǿ���.
	err_FCMTB,					// �⺻ �޽����� ��ȯ�� �����Ͽ���.
	err_FCMTO,					// ���� �޽����� ��ȯ�� �����Ͽ���.
	err_FRUI,					// ���� ������ �޴µ� �����Ͽ���.
} ;





//-------------------------------------------------------------------------------------------------
//	The ChatRoom Manager.
//-------------------------------------------------------------------------------------------------
class CChatRoomMgr
{
public:
	GETINSTANCE(CChatRoomMgr) ;			// ä�� �Ŵ��� ������ ��ȯ.

	CChatRoomMgr(void) ;				// ������ �Լ�.
	virtual ~CChatRoomMgr(void) ;		// �Ҹ��� �Լ�.

	// ä�ù� �ý��ۿ� �߰� �� �������� �˻��ϴ� �Լ�.
	BYTE IsAddedUser(DWORD dwCharacterID) ;

	// ����(Dist)�� ���� ����� ��Ű�� �Լ�.
	void RegistPlayer_To_Lobby(USERINFO* pUserInfo, BYTE byProtocol) ;
	BYTE RegistPlayer_To_Lobby(ST_CR_USER* pUserInfo) ;
	// ������ ����(Dist)�� ������ ��Ͻ�Ű�� �Լ�.
	void ForceRegistPlayer_To_Lobby(DWORD dwIndex, char* pMsg, DWORD dwLength) ;
	// ������ ������Ű�� �Լ�.
	void DestroyPlayer_From_Lobby(USERINFO* pUserInfo) ;

	// ������ ������ ������Ʈ ��Ű�� �Լ�.
	void UpdatePlayerInfo(USERINFO* pUserInfo) ;

	// ä�ù� ������ �޽��� ó���� �ϴ� �Լ�.
	void ChatMsg_Normal_Syn(DWORD dwIndex, char* pMsg, DWORD dwLength) ;

public :
	// Ŭ���̾�Ʈ�� ���� ���� �޽����� ó���ϴ� �Լ�.
	void UserMsgParser(DWORD dwIndex, char* pMsg, DWORD dwLength) ;

	// ����(Dist/Agent)�� ���� ���� �޽����� ó���ϴ� �Լ�.
	void ServerMsgParser(DWORD dwIndex, char* pMsg, DWORD dwLength) ;

	// �������� �޽��� �ٷ� �����ϴ� �Լ�.
	void SendMsg2User(DWORD dwIndex, char* pMsg, DWORD dwLength) ;
	void BroadCastToUser(DWORD dwIndex, char* pMsg, DWORD dwLength) ;
	// ��Ȳ�� ���� �ٸ� Agent���� �޽����� �����ϴ� �Լ�.
	void Join_Room_Notice(DWORD dwIndex, char* pMsg, DWORD dwLength) ;
	void Out_Room_Notice(DWORD dwIndex, char* pMsg, DWORD dwLength) ;
	void Out_Room_Change_Owner_Notcie(DWORD dwIndex, char* pMsg, DWORD dwLength) ;
	void Change_Option_Notice(DWORD dwIndex, char* pMsg, DWORD dwLength) ;
	void Change_Owner_Notice(DWORD dwIndex, char* pMsg, DWORD dwLength) ;
	void Kick_Guest_Ack(DWORD dwIndex, char* pMsg, DWORD dwLength) ;
	void Kick_Guest_Notice(DWORD dwIndex, char* pMsg, DWORD dwLength) ;
	//void ChatMsg_Normal_Notice(DWORD dwIndex, char* pMsg, DWORD dwLength) ;

	void Request_Friend_Ack(DWORD dwIndex, char* pMsg, DWORD dwLength) ;

public :
	// ���� ó���� �ϴ� �Լ���.
	void Throw_Error(BYTE errType, char* szCaption) ;	// ����Ÿ������ ���� ó���� �Ѵ�.
	void Throw_Error(char* szErr, char* szCaption) ;	// ���� �޽����� ���� ó���� �Ѵ�.
	void WriteLog(char* pMsg) ;							// �α׸� ����� �Լ�.
};

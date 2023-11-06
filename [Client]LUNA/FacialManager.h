#pragma once

#define FACIALMGR USINGTON(CFacialManager)

class CPlayer ;

class CFacialManager
{
public:
	// desc_hseos_�󱼱�ü01
	// S �󱼱�ü �߰� added by hseos 2007.06.08
	static DWORD			PLAYER_FACE_EYE_BLINK_OPEN_TIME;						// �� �������� �� �߰� �ִ� �ð�
	static DWORD			PLAYER_FACE_EYE_BLINK_OPEN_ADD_RND_TIME;				// �� �������� �� �߰� �ִ� �߰� ���� �ð�
	static DWORD			PLAYER_FACE_EYE_BLINK_CLOSE_TIME;						// �� �������� �� ���� �ִ� �ð�
	static DWORD			PLAYER_FACE_EYE_BLINK_CLOSE_ADD_RND_TIME;				// �� �������� �� ���� �ִ� �߰� ���� �ð�
	static DWORD			PLAYER_FACE_SHAPE_TIME;									// �� ����� ǥ�õǴ� �ð� ( �ִϸ��̼��� ��� �������� ó�� �߰��ʿ�)
	// E �󱼱�ü �߰� added by hseos 2007.06.08

public:
	CFacialManager(void);
	~CFacialManager(void); 


	// desc_hseos_�󱼱�ü01
	// S �󱼱�ü �߰� added by hseos 2007.06.08
	//						��ũ��Ʈ ���� �ε�
	BOOL					LoadScriptFileData();														
	// E �󱼱�ü �߰� added by hseos 2007.06.08

	void ChangeHeroFace(BYTE faceNum) ;
	void ChangePlayerFace(CPlayer* pPlayer, BYTE faceNum) ;
	void NetworkMsgParse( BYTE Protocol, void* pMsg );

	// 070312 LYW --- FacialManager : Add function for eye part.
	void CloseHeroEye() ;
	void ClosePlayerEye() ;
	void SendToServerFace(BYTE faceNum) ;
};
EXTERNGLOBALTON(CFacialManager)
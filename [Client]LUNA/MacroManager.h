#pragma once


#define MACROMGR USINGTON(CMacroManager)

class CKeyboard;
class CObject;

//---------------------------------------------------------------------------------------
// taiyo 
// ����� �ӽ÷� ����Ű�� ����������,
// ����Ű ����� ���� �߰��� ����!
//---------------------------------------------------------------------------------------
//KES_030806
// ����Ű ���
//---------------------------------------------------------------------------------------


////////////
//����Ű���
//#define START_CONFLICT_CHAT		ME_USE_QUICKITEM01	//ä�ð� �浹�Ǵ� ����Ű.

enum eMacroEvent {	//���̾�α׿� ������ ���߾�� �Ѵ�.

	ME_USE_QUICKITEM_1_01,										// ���� ������ 1��Ű ���
	ME_USE_QUICKITEM_1_02, 										// ���� ������ 2��Ű ���
	ME_USE_QUICKITEM_1_03, 										// ���� ������ 3��Ű ���
	ME_USE_QUICKITEM_1_04, 										// ���� ������ 4��Ű ���
	ME_USE_QUICKITEM_1_05, 										// ���� ������ 5��Ű ���
	ME_USE_QUICKITEM_1_06, 										// ���� ������ 6��Ű ���
	ME_USE_QUICKITEM_1_07, 										// ���� ������ 7��Ű ���
	ME_USE_QUICKITEM_1_08, 										// ���� ������ 8��Ű ���
	ME_USE_QUICKITEM_1_09, 										// ���� ������ 9��Ű ���
	ME_USE_QUICKITEM_1_10, 										// ���� ������10��Ű ���

	ME_USE_QUICKITEM_2_01,										// ���� ������ 1��Ű ��� 
	ME_USE_QUICKITEM_2_02, 										// ���� ������ 2��Ű ��� 
	ME_USE_QUICKITEM_2_03, 										// ���� ������ 3��Ű ��� 
	ME_USE_QUICKITEM_2_04, 										// ���� ������ 4��Ű ��� 
	ME_USE_QUICKITEM_2_05, 										// ���� ������ 5��Ű ��� 
	ME_USE_QUICKITEM_2_06, 										// ���� ������ 6��Ű ��� 
	ME_USE_QUICKITEM_2_07, 										// ���� ������ 7��Ű ��� 
	ME_USE_QUICKITEM_2_08, 										// ���� ������ 8��Ű ��� 
	ME_USE_QUICKITEM_2_09, 										// ���� ������ 9��Ű ��� 
	ME_USE_QUICKITEM_2_10, 										// ���� ������10��Ű ��� 

	ME_CHANGE_QUICKSLOT_01,										// ���� �������� 1������ ������.
	ME_CHANGE_QUICKSLOT_02,										// ���� �������� 2������ ������.
	ME_CHANGE_QUICKSLOT_03,										// ���� �������� 3������ ������.
	ME_CHANGE_QUICKSLOT_04,										// ���� �������� 4������ ������.
	ME_CHANGE_QUICKSLOT_05,										// ���� �������� 5������ ������.
	ME_CHANGE_QUICKSLOT_06,										// ���� �������� 6������ ������.
	ME_CHANGE_QUICKSLOT_07,										// ���� �������� 7������ ������.
	ME_CHANGE_QUICKSLOT_08,										// ���� �������� 8������ ������.

	ME_INCREASE_MAIN_QUICKSLOT_NUM,								// ���� ������ ��ȣ�� ���� ��Ų��.
	ME_DECREASE_MAIN_QUICKSLOT_NUM,								// ���� ������ ��ȣ�� ���� ��Ų��.

	ME_INCREASE_SUB_QUICKSLOT_NUM,								// ���� ������ ��ȣ�� ���� ��Ų��.
	ME_DECREASE_SUB_QUICKSLOT_NUM,								// ���� ������ ��ȣ�� ���� ��Ų��.

	ME_SELECT_MYSELF,											// �� �ڽ��� �����Ѵ�.

	ME_SELECT_PARTYMEMBER_1,									// ù ��° ��Ƽ ����� �����Ѵ�.
	ME_SELECT_PARTYMEMBER_2,									// �� ��° ��Ƽ ����� �����Ѵ�.
	ME_SELECT_PARTYMEMBER_3,									// �� ��° ��Ƽ ����� �����Ѵ�.
	ME_SELECT_PARTYMEMBER_4,									// �� ��° ��Ƽ ����� �����Ѵ�.
	ME_SELECT_PARTYMEMBER_5,									// �ټ� ��° ��Ƽ ����� �����Ѵ�.
	ME_SELECT_PARTYMEMBER_6,									// ���� ��° ��Ƽ ����� �����Ѵ�.

	ME_SELECT_NEAR_TARGET,										// ���� �����̿� �ִ� Ÿ���� �����Ѵ�.

	ME_SCREENCAPTURE,											// ��ũ���� ĸ���Ѵ�.

	ME_TOGGLE_EXITDLG,											// ���� �޽��� â�� ����.

	ME_TOGGLE_ALLINTERFACE,										// ��� �������̽��� ����Ѵ�.

	ME_TOGGLE_SKILLDLG,											// ��ųâ�� ����Ѵ�.

	ME_TOGGLE_FAMILYDLG,										// �йи� â�� ����Ѵ�.

	ME_TOGGLE_INVENTORYDLG,										// �κ��丮�� ����Ѵ�.

	ME_TOGGLE_HELP_SHOW,										// ������ ����Ѵ�.

	ME_TOGGLE_MINIMAP,											// �̴ϸ��� ����Ѵ�.

	ME_TOGGLE_BIGMAP,											// ����� ����Ѵ�.

	ME_TOGGLE_OPTIONDLG,										// �ɼ� â�� ����Ѵ�.

	ME_TOGGLE_GUILDDLG,											// ��� â�� ����Ѵ�.

	ME_TOGGLE_QUESTDLG,											// ����Ʈ â�� ����Ѵ�.

	ME_TOGGLE_CHARINFODLG,										// ĳ���� ����â�� ����Ѵ�.

	ME_WALK_RUN,												// �ȱ�/�ٱ� ��带 ��ȯ�Ѵ�.

	ME_TOGGLE_CAMERAVIEWMODE,									// ī�޶� �� ��带 ��ȯ�Ѵ�.

	ME_TOGGLE_MATCHINGDLG,										// ��Ī â�� ����Ѵ�.

	ME_TOGGLE_FRIENDDLG,										// ģ��â
	ME_TOGGLE_NOTEDLG,											// ����â

	// 071025 LYW --- MacroManager : Add macro for help dialog.
	ME_TOGGLE_HELPDLG,											// ���� â.
	
	ME_FISHING_PULLINGBTN,										// ���� �����ư

	// 080429 LYW --- MacroManager : Add macro for chatroom system.
	ME_TOGGLE_CHATROOM_LISTDLG,									// ä�ù� ����Ʈ ���̾�α� ���.
	ME_TOGGLE_CHATROOM_ROOMDLG,									// ä�ù� ���.
	ME_SETFOCUS_CHATROOM_CHATTING,								// ä��â�� ä�ù� ����Ʈ �ڽ� ���̿� ��Ŀ���� �̵������ִ� ����Ű.

	ME_PET_INFO,
	ME_COUNT,					//��ũ�� �� ����
};

enum eMacroPressEvent
{
	MPE_CAMERAMOVE_ZOOMIN,
	MPE_CAMERAMOVE_ZOOMOUT,
	
	MPE_CAMERAMOVE_UP,
	MPE_CAMERAMOVE_DOWN,
	MPE_CAMERAMOVE_RIGHT,
	MPE_CAMERAMOVE_LEFT,

	// 070123 LYW --- Add macro event.
	MPE_VIEW_FRONTYARD, 
	MPE_VIEW_BACKYARD,
	
	MPE_COUNT,
};

////////
//����Ű
enum eSysKey {

	MSK_NONE	= 1,	//
	MSK_CTRL	= 2,
	MSK_ALT		= 4,
	MSK_SHIFT	= 8,
	MSK_ALL		= MSK_NONE | MSK_CTRL | MSK_ALT | MSK_SHIFT,
};

//////////////
//��ũ�α���ü
struct sMACRO
{
	int		nSysKey;	//����Ű
	WORD	wKey;		//����Ű
	BOOL	bAllMode;	//ä�ø�忡 ������� �Ǿ���ϴ� ��üũ
	BOOL	bUp;		//up�� �̺�Ʈ�� �߻��ؾ� �Ұ�
};


enum eMacroMode 
{
	MM_CHAT,					//ä�� �켱���
	MM_MACRO,					//����Ű �켱���

	MM_COUNT,
};




class CMacroManager
{

protected:

	sMACRO		m_DefaultKey[MM_COUNT][ME_COUNT];	//����Ʈ��ũ��Ű
	sMACRO		m_MacroKey[MM_COUNT][ME_COUNT];		//������ũ��Ű

	sMACRO		m_DefaultPressKey[MM_COUNT][MPE_COUNT];		//����Ʈ��ũ��������Ű
	sMACRO		m_MacroPressKey[MM_COUNT][MPE_COUNT];		//������ũ��������Ű

	int			m_nMacroMode;					//���õ� ��ũ�� ���
//	int			m_nUserSetMode;					//�������ÿ��� ���õ� ���
	BOOL		m_bPause;
	BOOL		m_bChatMode;
	BOOL		m_bInVisibleCursor;

	int			m_nCurTargetIdx ;
	DWORD		m_dwTargetID ;
	BOOL		mIsToggleInterface;

protected:
	BOOL LoadMacro( LPCTSTR strPath );
	BOOL SaveMacro( LPCTSTR strPath );
	void SetDefaultMacro( int nMacroMode );

public:
	CMacroManager();
	virtual ~CMacroManager();

//	//MAKESINGLETON(CMacroManager);
	BOOL IsVisibleCursor() { return !m_bInVisibleCursor; }

	void Init();
	void KeyboardInput( CKeyboard* keyInfo );

	void KeyboardInput_Normal( BOOL bDock, CKeyboard* keyInfo ) ;
	void KeyboardInput_GMTool( BOOL bDock, CKeyboard* keyInfo ) ;
	void KeyboardInput_CheatEnable( BOOL bDock, CKeyboard* keyInfo ) ;
	void KeyboardInput_Cheat_TestClient( BOOL bDock, CKeyboard* keyInfo ) ;
	void KeyboardInput_Cheat_NotTestClient( BOOL bDock, CKeyboard* keyInfo ) ;

	void KeyboardPressInput( CKeyboard* keyInfo );
	void LoadUserMacro();
	void SaveUserMacro();

	void GetMacro( int nMacroMode, sMACRO* pMacro );		//��ü ��ũ�� ����
	void SetMacro( int nMacroMode, sMACRO* pMacro );		//���� ��ũ�� ����

	sMACRO* GetCurMacroKey( int nMacroEvent )	//Ư�� ��ũ��Ű �ϳ� ����
	{ 
		if( nMacroEvent >= ME_COUNT )
			return NULL;
		
		return &m_DefaultKey[m_nMacroMode][nMacroEvent];	//���Ŀ� �ٲ���.userset�������ÿ�.
	}
	
	void GetDefaultMacro( int nMacroMode, sMACRO* pMacro );

	int GetMacroMode() { return m_nMacroMode; }
	void SetMacroMode( int nMacroMode );

//	int GetUserSetMode() { return m_nUserSetMode; }
//	void SetUserSetMode( int nUserSetMode ) { m_nUserSetMode = nUserSetMode; }

	void SetPause( BOOL bPause ) { m_bPause = bPause; }
	int IsUsableKey( CKeyboard* keyInfo, WORD* wKey, BOOL bUserMode );
	BOOL IsChatMode()	{ return m_bChatMode; }

	void CheckToggleState( CObject* );

	// 061203 LYW --- Add Function To Setting Value That Will Be TRUE.
	void SetChatMode( BOOL val ) { m_bChatMode = val ; }

	void PlayMacro( int nMacroEvent );
	void PM_Toggle_ExitDlg() ;
	void PM_ScreenCapture() ;
	void PM_UseMainQuickSlotItem( int nMacroEvent ) ;
	void PM_UseSubQuickSlotItem( int nMacroEvent ) ;
	void PM_Change_MainSlotNum( int nMacroEvent ) ;
	void PM_SetSlotPage( DWORD dwSlotID, int nMacroEvent ) ;
	void PM_SelectPartyMember( int nMacroEvent ) ;
	void PM_Toggle_AllInterface() ;
	void PM_Toggle_SkillDlg() ;
	void PM_Toggle_FamilyDlg() ;
	void PM_Toggle_InventoryDlg() ;
	void PM_Toggle_ShowKeyTip() ;
	void PM_Toggle_MiniMap() ;
	void PM_Toggle_BigMap() ;
	void PM_Toggle_OptionDlg() ;
	void PM_Toggle_GuildDlg() ;
	void PM_Toggle_QuestDlg() ;
	void PM_Toggle_CharInfoDlg() ;
	void PM_Toggle_MatchingDlg() ;
	void PM_SetAutoTarget() ;

	void PM_Toggle_FriendDlg() ;
	void PM_Toggle_NoteDlg() ;
	void PM_Toggle_HelpDlg() ;
	// 080429 LYW --- MacroManager : Add functions to toggle chatroom part.
	void PM_Toggle_ChatRoomListDlg() ;
	void PM_Toggle_ChatRoomDlg() ;
	void PM_SetFocus_ChatRoom_Chat() ;

	void PM_Toggle_Pet_Info();

	int GetCurTargetIdx() { return m_nCurTargetIdx ; }

	void PlayMacroPress( int nMacroEvent );
/////
};

EXTERNGLOBALTON(CMacroManager)


//// MacroManager.h: interface for the CMacroManager class.
////
////////////////////////////////////////////////////////////////////////
//
//#if !defined(AFX_MACROMANAGER_H__A85EAD4A_B009_45AE_AC28_B00E31856F59__INCLUDED_)
//#define AFX_MACROMANAGER_H__A85EAD4A_B009_45AE_AC28_B00E31856F59__INCLUDED_
//
//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
//
//#define MACROMGR USINGTON(CMacroManager)
//
//class CKeyboard;
////---------------------------------------------------------------------------------------
//// taiyo 
//// ����� �ӽ÷� ����Ű�� ����������,
//// ����Ű ����� ���� �߰��� ����!
////---------------------------------------------------------------------------------------
////KES_030806
//// ����Ű ���
////---------------------------------------------------------------------------------------
//
//
//////////////
////����Ű���
////#define START_CONFLICT_CHAT		ME_USE_QUICKITEM01	//ä�ð� �浹�Ǵ� ����Ű.
//
//enum eMacroEvent {	//���̾�α׿� ������ ���߾�� �Ѵ�.
//
//	// 070123 LYW --- Modified this part.
//	/*
//	ME_TOGGLE_EXITDLG,
//
//	ME_USE_QUICKITEM_1_01,
//	ME_USE_QUICKITEM_1_02,
//	ME_USE_QUICKITEM_1_03,
//	ME_USE_QUICKITEM_1_04,
//	ME_USE_QUICKITEM_1_05,
//	ME_USE_QUICKITEM_1_06,
//	ME_USE_QUICKITEM_1_07,
//	ME_USE_QUICKITEM_1_08,
//	
//	ME_SELECT_QUICKSLOT_1_NEXT,
//	ME_SELECT_QUICKSLOT_1_PREV,
//	
//	ME_USE_QUICKITEM_2_01,
//	ME_USE_QUICKITEM_2_02,
//	ME_USE_QUICKITEM_2_03,
//	ME_USE_QUICKITEM_2_04,
//	ME_USE_QUICKITEM_2_05,
//	ME_USE_QUICKITEM_2_06,
//	ME_USE_QUICKITEM_2_07,
//	ME_USE_QUICKITEM_2_08,
//	
//	ME_SELECT_QUICKSLOT_2_NEXT,
//	ME_SELECT_QUICKSLOT_2_PREV,
//
//	ME_TOGGLE_SKILLDLG,
//	ME_TOGGLE_INVENTORYDLG,
//	ME_TOGGLE_CHARACTERDLG,
//	ME_TOGGLE_MUNPADLG,
//
//	ME_TOGGLE_MINIMAP,
//
//	ME_TOGGLE_QUESTDLG,
//	ME_TOGGLE_OPTIONDLG,
//	ME_TOGGLE_FRIENDLIST,
//	ME_SEND_MEMO,
//
//	ME_TOGGLE_MOVEMODE,
//
//	ME_TOGGLE_PEACEWARMODE,
//
//	ME_TOGGLE_AUTOATTACK,
//	ME_TOGGLE_AUTOATTACK2,
//
//	ME_TOGGLE_HELP,
//	
//	ME_TOGGLE_CAMERAVIEWMODE,
//
//	ME_SCREENCAPTURE,
//
//	ME_SHOWALLNAME_ON,
//	ME_SHOWALLNAME_OFF,
//
//	ME_CHANGE_WHISPERLIST_UP,
//	ME_CHANGE_WHISPERLIST_DOWN,
//	
//	ME_TOGGLE_BIGMAP,
//	ME_MUSSANG_ON,
//	ME_TOGGLE_KEY_SETTING_TIP_SHOW,
//	ME_TOGGLE_KEY_SETTING_TIP_HIDE,
//	*/
//
//	ME_TOGGLE_EXITDLG, 
//
//	ME_SELECT_PARTYMEMBER1, 
//	ME_SELECT_PARTYMEMBER2, 
//	ME_SELECT_PARTYMEMBER3, 
//	ME_SELECT_PARTYMEMBER4, 
//	ME_SELECT_PARTYMEMBER5, 
//	ME_SELECT_PARTYMEMBER6, 
//
//	ME_SCREENCAPTURE, 
//
//	ME_USE_QUICKITEM_1_01,
//	ME_USE_QUICKITEM_1_02,
//	ME_USE_QUICKITEM_1_03,
//	ME_USE_QUICKITEM_1_04,
//	ME_USE_QUICKITEM_1_05,
//	ME_USE_QUICKITEM_1_06,
//	ME_USE_QUICKITEM_1_07,
//	ME_USE_QUICKITEM_1_08,
//	ME_USE_QUICKITEM_1_09,
//	ME_USE_QUICKITEM_1_10,
//
//	ME_TOGGLE_ALLINTERFACE, 
//
//	ME_TOGGLE_CHATTINGDLG, 
//
//	ME_TOGGLE_AUTOATTACK, 
//
//	ME_TOGGLE_SKILLDLG, 
//
//	//ME_TOGGLE_KEY_SETTING_TIP, 
//
//	ME_TOGGLE_FAMILYDLG, 
//
//	ME_SEND_MEMO, 
//
//	ME_TOGGLE_INVENTORYDLG,
//
//	// 070614 LYW --- MacroManager : Modified help key.
//	//ME_TOGGLE_HELP,
//	ME_TOGGLE_HELP_SHOW,
//	//ME_TOGGLE_HELP_HIDE,
//
//
//	ME_TOGGLE_FRIENDLIST,
//
//	ME_TOGGLE_MINIMAP,
//
//	ME_TOGGLE_BIGMAP,
//
//	ME_TOGGLE_OPTIONDLG,
//
//	ME_TOGGLE_GUILDDLG, 
//
//	ME_TOGGLE_QUESTDLG, 
//
//	ME_SIT_STANCE, 
//
//	ME_TOGGLE_CHARINFODLG, 
//
//	ME_TOGGLE_ALLIANCEDLG, 
//
//	ME_WALK_RUN,
//
//	// 070124 LYW --- Add macro event for camera view mode.
//	ME_TOGGLE_CAMERAVIEWMODE,
//
//	// 070607 LYW --- Add macro event to open matching dialog.
//	ME_TOGGLE_MATCHINGDLG,
//
//
//	ME_COUNT,					//��ũ�� �� ����
//};
//
//enum eMacroPressEvent
//{
//	MPE_CAMERAMOVE_ZOOMIN,
//	MPE_CAMERAMOVE_ZOOMOUT,
//	
//	MPE_CAMERAMOVE_UP,
//	MPE_CAMERAMOVE_DOWN,
//	MPE_CAMERAMOVE_RIGHT,
//	MPE_CAMERAMOVE_LEFT,
//
//	// 070123 LYW --- Add macro event.
//	MPE_VIEW_FRONTYARD, 
//	MPE_VIEW_BACKYARD,
//	
//	MPE_COUNT,
//};
//
//////////
////����Ű
//enum eSysKey {
//
//	MSK_NONE	= 1,	//
//	MSK_CTRL	= 2,
//	MSK_ALT		= 4,
//	MSK_SHIFT	= 8,
//	MSK_ALL		= MSK_NONE | MSK_CTRL | MSK_ALT | MSK_SHIFT,
//};
//
////////////////
////��ũ�α���ü
//struct sMACRO
//{
//	int		nSysKey;	//����Ű
//	WORD	wKey;		//����Ű
//	BOOL	bAllMode;	//ä�ø�忡 ������� �Ǿ���ϴ� ��üũ
//	BOOL	bUp;		//up�� �̺�Ʈ�� �߻��ؾ� �Ұ�
//};
//
//
//enum eMacroMode 
//{
//	MM_CHAT,					//ä�� �켱���
//	MM_MACRO,					//����Ű �켱���
//
//	MM_COUNT,
//};
//
//
//
//
//class CMacroManager
//{
//
//protected:
//
//	sMACRO		m_DefaultKey[MM_COUNT][ME_COUNT];	//����Ʈ��ũ��Ű
//	sMACRO		m_MacroKey[MM_COUNT][ME_COUNT];		//������ũ��Ű
//
//	sMACRO		m_DefaultPressKey[MM_COUNT][MPE_COUNT];		//����Ʈ��ũ��������Ű
//	sMACRO		m_MacroPressKey[MM_COUNT][MPE_COUNT];		//������ũ��������Ű
//
//	int			m_nMacroMode;					//���õ� ��ũ�� ���
////	int			m_nUserSetMode;					//�������ÿ��� ���õ� ���
//	BOOL		m_bPause;
//	BOOL		m_bChatMode;
//	BOOL		m_bInVisibleCursor;
//
//protected:
//	BOOL LoadMacro( LPCTSTR strPath );
//	BOOL SaveMacro( LPCTSTR strPath );
//	void SetDefaultMacro( int nMacroMode );
//
//public:
//	CMacroManager();
//	virtual ~CMacroManager();
//
////	//MAKESINGLETON(CMacroManager);
//	BOOL IsVisibleCursor() { return !m_bInVisibleCursor; }
//
//	void Init();
//	void KeyboardInput( CKeyboard* keyInfo );
//	void KeyboardPressInput( CKeyboard* keyInfo );
//	void LoadUserMacro();
//	void SaveUserMacro();
//
//	void GetMacro( int nMacroMode, sMACRO* pMacro );		//��ü ��ũ�� ����
//	void SetMacro( int nMacroMode, sMACRO* pMacro );		//���� ��ũ�� ����
//
//	sMACRO* GetCurMacroKey( int nMacroEvent )	//Ư�� ��ũ��Ű �ϳ� ����
//	{ 
//		if( nMacroEvent >= ME_COUNT )
//			return NULL;
//		
//		return &m_DefaultKey[m_nMacroMode][nMacroEvent];	//���Ŀ� �ٲ���.userset�������ÿ�.
//	}
//	
//	void GetDefaultMacro( int nMacroMode, sMACRO* pMacro );
//
//	int GetMacroMode() { return m_nMacroMode; }
//	void SetMacroMode( int nMacroMode );
//
////	int GetUserSetMode() { return m_nUserSetMode; }
////	void SetUserSetMode( int nUserSetMode ) { m_nUserSetMode = nUserSetMode; }
//
//	void SetPause( BOOL bPause ) { m_bPause = bPause; }
//	int IsUsableKey( CKeyboard* keyInfo, WORD* wKey, BOOL bUserMode );
//	BOOL IsChatMode()	{ return m_bChatMode; }
//	// 061203 LYW --- Add Function To Setting Value That Will Be TRUE.
//	void SetChatMode( BOOL val ) { m_bChatMode = val ; }
//
//	void PlayMacro( int nMacroEvent );
//	void PlayMacroPress( int nMacroEvent );
///////
//};
//
//EXTERNGLOBALTON(CMacroManager)
//
//#endif // !defined(AFX_MACROMANAGER_H__A85EAD4A_B009_45AE_AC28_B00E31856F59__INCLUDED_)

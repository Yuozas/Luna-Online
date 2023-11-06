// OptionManager.h: interface for the COptionManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTIONMANAGER_H__16984CD0_B39F_40C4_AA38_3BCF9894A020__INCLUDED_)
#define AFX_OPTIONMANAGER_H__16984CD0_B39F_40C4_AA38_3BCF9894A020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define OPTIONMGR USINGTON(COptionManager)

#define OPTIONVERSION	0x00070716

class COptionDialog;
class cGuageBar;

struct sGAMEOPTION
{
	BOOL bNoDeal;															// �ŷ� �ź�.
	BOOL bNoParty;															// ��Ƽ �ź�.
	BOOL bNoFriend;															// ģ����� �ź�.
	BOOL bNoShowdown;														// ���� �ź�.

	BOOL bNameMunpa;														// ���� �̸� ǥ��.
	BOOL bNameParty;														// ��Ƽ�� �̸� ǥ��.
	BOOL bNameOthers;														// �ٸ� ĳ���� �̸� ǥ��.
	BOOL bNoMemberDamage;													// ��Ƽ�� ������ ����.
	BOOL bNoGameTip;														// ���� �� ����.

	int nMacroMode;															// ä�ÿ켱���/����Ű�켱���.

	BOOL bNoWhisper;														// �ӼӸ� �ź�.
	BOOL bNoChatting;														// �Ϲ�ä�� ����.
	BOOL bNoBalloon;														// ��ǳ�� ����.
	//BOOL bAutoHide;															// �޽��� ǥ��â �ڵ� ����.
	BOOL bNoPartyChat ;														// ��Ƽ ä�� ����.
	BOOL bNoFamilyChat ;													// �йи� ä�� ����.
	BOOL bNoShoutChat;														// ��ġ�� ����.
	BOOL bNoGuildChat;														// ��� ä�� ����.
	BOOL bNoAllianceChat;													// ����ä�� ����.
	BOOL bNoSystemMsg;														// �ý��� �޽��� ����.
	BOOL bNoExpMsg;															// ����ġ/����ġ ȹ��޽��� ����.
	BOOL bNoItemMsg;														// ��/������ ȹ��޽��� ����.

	int nSightDistance;														// �þ� �Ÿ� ��.

	BOOL bGraphicCursor;													// �׷��� Ŀ�� ����?

	BOOL bShadowHero;														// �׸��� ǥ�� - �ڽ�.
	BOOL bShadowMonster;													// �׸��� ǥ�� - ����/NPC.
	BOOL bShadowOthers;														// �׸��� ǥ�� - �ٸ�ĳ����.

	BOOL bAutoCtrl;															// ���� ȭ�� �ڵ�����.
	int nLODMode;															// �׷��� ���� ��.
	int nEffectMode;														// ����Ʈ ǥ�ð�.
	int nEffectSnow;														// �� ����Ʈ ����.

	BOOL bSoundBGM;															// ������� ON/OFF.
	BOOL bSoundEnvironment;													// ȿ���� ON/OFF.

	int	nVolumnBGM;															// ������� ����.
	int nVolumnEnvironment;													// ȿ���� ����.

	BOOL bAmbientMax;														//SW060904 ĳ��ȯ��ݻ簪 ���� �ɼ� �߰�

	// desc_hseos_�ɼ�01
	// S �ɼ� �߰� added by hseos 2007.06.19
	int	nChatLineNum;
	int nCurChatMode;
	// E �ɼ� �߰� added by hseos 2007.06.19

	// 071025 LYW === OptionManager : Add tutorial option.
	BOOL bShowTutorial ;													// Ʃ�丮�� ���̱� ���θ� ��� ����.
};

class COptionManager  
{
protected:

	sGAMEOPTION		m_GameOption;											// ���� �ɼ��� ��� �ִ� ����ü.

	BOOL			m_bBGMSoundChanged;										// ��� ������ �ٲ������ ���θ� ��� ����.
	WORD			m_wSendOption;											// �ɼ� ���¸� ���� �� ����ϴ� �÷��� ����.

	BOOL			m_bShowMemberDamage ;

	COptionDialog*	m_pOptionDlg;											// �ɼ� ���̾�α� ������.

	cGuageBar*		m_pCammaGB;		//���� ������ �ȵ�						// ���� ��������.
	cGuageBar*		m_pSightGB;												// �þ߰Ÿ� ��������.

	LONG			m_SigntGBMinValue;										// �þ߰Ÿ� �ּ� ��.
	LONG			m_SigntGBMaxValue;										// �þ߰Ÿ� �ִ� ��.

	DWORD			m_dwShdowOption;										// �׸��� �ɼǰ��� ���� ����.

	enum {LOWEST=1,LOW,MID,HIGH,STANDARD_FRAME=15,NICE_FRAME=20};			// �ɼ� ���� ������ ���� �̳�.

	DWORD			m_dwLastCheckTime;										// ������ üũ �ð�.
	DWORD			m_dwCheck;												// üũ ��.
	DWORD			m_dwFrameAverage;										// ��� ������ ��.
	DWORD			m_dwCountFrame;											// ������ ī��Ʈ.
	int				m_nCurOptionState;										// ���� �ɼ� ����.
	
protected:

	BOOL LoadGameOption();													// �ɼ��� �ε��ϴ� �Լ�.
	WORD MakeOptionFlag();													// ���� �ź� �ɼ��� �߰��ϴ� �Լ�.

public:
	
	COptionManager();														// ������ �Լ�.
	virtual ~COptionManager();												// �Ҹ��� �Լ�.

	void Init();															// �ʱ�ȭ �Լ�.

	void SaveGameOption();													// �ɼ��� �����ϴ� �Լ�.

	void SetDefaultOption();												// �⺻ �ɼ��� �����ϴ� �Լ�.

	void ApplySettings();													// ���� ���� ���� �����ϴ� �Լ�.

	void ApplySound() ;														// ���� ��Ʈ�� �����ϴ� �Լ�.
	void ApplyGraphics() ;													// �׷��� ��Ʈ�� �����ϴ� �Լ�.
	void ApplyChatting() ;													// ä�� ��Ʈ�� �����ϴ� �Լ�.
	void ApplyEnvironmemt() ;												// ȯ��ɼ� ��Ʈ�� �����ϴ� �Լ�.

	void CancelSettings();													// ������׵��� ����ϴ� �Լ�.

	void SendOptionMsg();													// �ɼǻ����� ������ �����ϴ� �Լ�.

	sGAMEOPTION* GetGameOption() { return &m_GameOption; }					// ���� �ɼ��� ��ȯ�ϴ� �Լ�.
	void SetGameOption( sGAMEOPTION* pOption );								// ���� �ɼ��� �޴� �Լ�.
	
	void NetworkMsgParse( BYTE Protocol, void* pMsg );						// ��Ʈ��ũ �޽��� �Ľ� ó�� �Լ�.

	void SetMemberDemageActive(BOOL bDo) { m_bShowMemberDamage = bDo ; }
	BOOL IsShowMemberDamage() { return m_bShowMemberDamage ; }

	void GraphicAutoCtrlProcess();											// �׷��� �ڵ� ���� ���μ��� �Լ�.
	void ApplyGraphicSetting();												// �׷��� ������ �����ϴ� �Լ�.
	void InitForGameIn();													// �����ο� �鶧 ó���ϴ� �ʱ�ȭ �Լ�.

	// 071025 LYW --- OptionManager : Add function to setting and return tutorial option.
	void Set_ShowTutorial(BOOL val) { m_GameOption.bShowTutorial = val ; }	// Ʃ�丮�� ���̱� ���θ� �����Ѵ�.
	BOOL Is_ShowTutorial() { return m_GameOption.bShowTutorial ;}			// Ʃ�丮�� ���̱� ���θ� ��ȯ�Ѵ�.
};

EXTERNGLOBALTON(COptionManager);

#endif // !defined(AFX_OPTIONMANAGER_H__16984CD0_B39F_40C4_AA38_3BCF9894A020__INCLUDED_)

//struct sGAMEOPTION
//{
//	// 061204 LYW --- Change member variables name.
//	// befor modified.
//	/*
//	BOOL bNoDeal;	//...
//	BOOL bNoParty;	//...
//	BOOL bNoFriend;	//...
//	BOOL bNoShowdown;	//...
//
//	BOOL bNameMunpa;
//	BOOL bNameParty;
//	BOOL bNameOthers;
//	BOOL bNoMemberDamage;
//	BOOL bNoGameTip;
//
//	int nMacroMode;
//	*/
//	// after modified.
//	BOOL bWhisper ;
//	BOOL bTalk ;
//	BOOL bBalloon ;
//	BOOL bSystem ;
//	//BOOL bNotify ;
//	BOOL bExperience1 ;
//	//BOOL bExperience2 ;
//	BOOL bItem1 ;
//	//BOOL bItem2 ;
//	BOOL bMoney1 ;
//	//BOOL bMoney2 ;
//	BOOL bCombat1 ;
//	//BOOL bCombat2 ;
//	BOOL bEmoticon ;
//	// end modify.
//
//	BOOL bNoWhisper;	//...
//	BOOL bNoChatting;
//	BOOL bNoBalloon;
//	BOOL bAutoHide;
//	BOOL bNoShoutChat;
//	BOOL bNoGuildChat;
//	BOOL bNoAllianceChat;
//	BOOL bNoSystemMsg;
//	BOOL bNoExpMsg;
//	BOOL bNoItemMsg;
//
//	int nGamma;
//	int nSightDistance;
//
//	BOOL bGraphicCursor;		//
//
//	BOOL bShadowHero;
//	BOOL bShadowMonster;
//	BOOL bShadowOthers;
//	//SW050822
//
//	BOOL bAutoCtrl;
//	int nLODMode;
//	int nEffectMode;
//	int nEffectSnow; // 2005.12.28�� ��On/Off �߰� 
//
//
//	BOOL bSoundBGM;				//
//	BOOL bSoundEnvironment;		//
//	int	nVolumnBGM;				//
//	int nVolumnEnvironment;		//
//
//	//SW060904 ĳ��ȯ��ݻ簪 ���� �ɼ� �߰�
//	BOOL bAmbientMax;
//
//	// desc_hseos_�ɼ�01
//	// S �ɼ� �߰� added by hseos 2007.06.19
//	int	nChatLineNum;
//	int nCurChatMode;
//	// E �ɼ� �߰� added by hseos 2007.06.19
//
////	WORD wKyungGongIdx;
////	BOOL bIsKyungGongMode;
//	
////	BOOL bShadowMap;
////	int nShadowDetail;
////	BOOL bHideGXObject;
////	int nInterfaceAlpha;
////	BOOL bNameHero;
////	BOOL bNameMonster;
////	BOOL bCameraAction;
////	BOOL bSoundInterface;
////	BOOL bSoundAttack;
////	BOOL bSoundCharater;
////	BOOL bSoundMonster;
//};


//class COptionManager  
//{
//protected:
//
//	sGAMEOPTION		m_GameOption;
//
//	BOOL			m_bBGMSoundChanged;
//	WORD			m_wSendOption;
//
//	//SW050804 �ɼ�â: ���� ������ ���� �߰�
//	BOOL			m_bShowMenberDamage;
//
//	//SW050825 �׷��ȿɼ�Tab: GraphicAutoCtrlProcess()
//	COptionDialog*	m_pOptionDlg;
//	cGuageBar*		m_pCammaGB;		//���� ������ �ȵ�
//	cGuageBar*		m_pSightGB;
//	LONG			m_SigntGBMinValue;
//	LONG			m_SigntGBMaxValue;
//	DWORD			m_dwShdowOption;
//	enum {LOWEST=1,LOW,MID,HIGH,STANDARD_FRAME=15,NICE_FRAME=20};
//	DWORD			m_dwLastCheckTime;
//	DWORD			m_dwCheck;
//	DWORD			m_dwFrameAverage;
//	DWORD			m_dwCountFrame;
//	int				m_nCurOptionState;
//	
//protected:
//
//	BOOL LoadGameOption();
//	WORD MakeOptionFlag();
//
//public:
//
//	//MAKESINGLETON(COptionManager);
//	
//	COptionManager();
//	virtual ~COptionManager();
//
//	void Init();
//
//	void SaveGameOption();
//	void SetDefaultOption();
//	void ApplySettings();
//	void ApplySound() ;
//	void ApplyGraphics() ;
//	void ApplyChatting() ;
//	void ApplyEnvironmemt() ;
//	void CancelSettings();
//
//	void SendOptionMsg();
//
//	sGAMEOPTION* GetGameOption() { return &m_GameOption; }
//	void SetGameOption( sGAMEOPTION* pOption );
//	
////	void SetKyungGongIdx(WORD KyungGondIdx);
////	WORD GetKyungGongIdx()	{ return m_GameOption.wKyungGongIdx;	}
////	BOOL GetKyungGongMode();
////	void SetKyungGongMode(BOOL bMode);
////	void ClrKyungGong();
//
////parsing
//	void NetworkMsgParse( BYTE Protocol, void* pMsg );
//
//	void SetMemberDemageActive(BOOL bDo) {m_bShowMenberDamage = bDo;}		// 
//	BOOL IsShowMemberDamage() {return m_bShowMenberDamage;}	
//
//	void GraphicAutoCtrlProcess();	
//	void ApplyGraphicSetting();
//	void InitForGameIn();
//
//};
//
//
//EXTERNGLOBALTON(COptionManager);
//
//#endif // !defined(AFX_OPTIONMANAGER_H__16984CD0_B39F_40C4_AA38_3BCF9894A020__INCLUDED_)


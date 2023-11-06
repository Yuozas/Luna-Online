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
	BOOL bNoDeal;															// 거래 거부.
	BOOL bNoParty;															// 파티 거부.
	BOOL bNoFriend;															// 친구등록 거부.
	BOOL bNoShowdown;														// 결투 거부.

	BOOL bNameMunpa;														// 길드원 이름 표시.
	BOOL bNameParty;														// 파티원 이름 표시.
	BOOL bNameOthers;														// 다른 캐릭터 이름 표시.
	BOOL bNoMemberDamage;													// 파티원 데미지 보기.
	BOOL bNoGameTip;														// 게임 팁 보기.

	int nMacroMode;															// 채팅우선모드/단축키우선모드.

	BOOL bNoWhisper;														// 귓속말 거부.
	BOOL bNoChatting;														// 일반채팅 끄기.
	BOOL bNoBalloon;														// 말풍선 끄기.
	//BOOL bAutoHide;															// 메시지 표시창 자동 숨김.
	BOOL bNoPartyChat ;														// 파티 채팅 끄기.
	BOOL bNoFamilyChat ;													// 패밀리 채팅 끄기.
	BOOL bNoShoutChat;														// 외치기 끄기.
	BOOL bNoGuildChat;														// 길드 채팅 끄기.
	BOOL bNoAllianceChat;													// 동맹채팅 끄기.
	BOOL bNoSystemMsg;														// 시스템 메시지 끄기.
	BOOL bNoExpMsg;															// 경험치/수련치 획득메시지 끄기.
	BOOL bNoItemMsg;														// 돈/아이템 획득메시지 끄기.

	int nSightDistance;														// 시야 거리 값.

	BOOL bGraphicCursor;													// 그래픽 커서 여부?

	BOOL bShadowHero;														// 그림자 표시 - 자신.
	BOOL bShadowMonster;													// 그림자 표시 - 몬스터/NPC.
	BOOL bShadowOthers;														// 그림자 표시 - 다른캐릭터.

	BOOL bAutoCtrl;															// 게임 화면 자동조절.
	int nLODMode;															// 그래픽 상태 값.
	int nEffectMode;														// 이펙트 표시값.
	int nEffectSnow;														// 눈 이펙트 끄기.

	BOOL bSoundBGM;															// 배경음악 ON/OFF.
	BOOL bSoundEnvironment;													// 효과음 ON/OFF.

	int	nVolumnBGM;															// 배경음악 볼륨.
	int nVolumnEnvironment;													// 효과음 볼륨.

	BOOL bAmbientMax;														//SW060904 캐릭환경반사값 설정 옵션 추가

	// desc_hseos_옵션01
	// S 옵션 추가 added by hseos 2007.06.19
	int	nChatLineNum;
	int nCurChatMode;
	// E 옵션 추가 added by hseos 2007.06.19

	// 071025 LYW === OptionManager : Add tutorial option.
	BOOL bShowTutorial ;													// 튜토리얼 보이기 여부를 담는 변수.
};

class COptionManager  
{
protected:

	sGAMEOPTION		m_GameOption;											// 게임 옵션을 담고 있는 구조체.

	BOOL			m_bBGMSoundChanged;										// 배경 음악이 바뀌었는지 여부를 담는 변수.
	WORD			m_wSendOption;											// 옵션 상태를 보낼 때 사용하는 플래그 변수.

	BOOL			m_bShowMemberDamage ;

	COptionDialog*	m_pOptionDlg;											// 옵션 다이얼로그 포인터.

	cGuageBar*		m_pCammaGB;		//현재 적용은 안됨						// 감마 게이지바.
	cGuageBar*		m_pSightGB;												// 시야거리 게이지바.

	LONG			m_SigntGBMinValue;										// 시야거리 최소 값.
	LONG			m_SigntGBMaxValue;										// 시야거리 최대 값.

	DWORD			m_dwShdowOption;										// 그림자 옵션값을 담을 변수.

	enum {LOWEST=1,LOW,MID,HIGH,STANDARD_FRAME=15,NICE_FRAME=20};			// 옵션 상태 구분을 위한 이넘.

	DWORD			m_dwLastCheckTime;										// 마지막 체크 시간.
	DWORD			m_dwCheck;												// 체크 값.
	DWORD			m_dwFrameAverage;										// 평균 프레임 값.
	DWORD			m_dwCountFrame;											// 프레임 카운트.
	int				m_nCurOptionState;										// 현재 옵션 상태.
	
protected:

	BOOL LoadGameOption();													// 옵션을 로딩하는 함수.
	WORD MakeOptionFlag();													// 각종 거부 옵션을 추가하는 함수.

public:
	
	COptionManager();														// 생성자 함수.
	virtual ~COptionManager();												// 소멸자 함수.

	void Init();															// 초기화 함수.

	void SaveGameOption();													// 옵션을 저장하는 함수.

	void SetDefaultOption();												// 기본 옵션을 적용하는 함수.

	void ApplySettings();													// 각종 설정 값을 적용하는 함수.

	void ApplySound() ;														// 사운드 파트를 적용하는 함수.
	void ApplyGraphics() ;													// 그래픽 파트를 적용하는 함수.
	void ApplyChatting() ;													// 채팅 파트를 적용하는 함수.
	void ApplyEnvironmemt() ;												// 환경옵션 파트를 적용하는 함수.

	void CancelSettings();													// 적용사항들을 취소하는 함수.

	void SendOptionMsg();													// 옵션사항을 서버로 전송하는 함수.

	sGAMEOPTION* GetGameOption() { return &m_GameOption; }					// 게임 옵션을 반환하는 함수.
	void SetGameOption( sGAMEOPTION* pOption );								// 게임 옵션을 받는 함수.
	
	void NetworkMsgParse( BYTE Protocol, void* pMsg );						// 네트워크 메시지 파싱 처리 함수.

	void SetMemberDemageActive(BOOL bDo) { m_bShowMemberDamage = bDo ; }
	BOOL IsShowMemberDamage() { return m_bShowMemberDamage ; }

	void GraphicAutoCtrlProcess();											// 그래픽 자동 설정 프로세싱 함수.
	void ApplyGraphicSetting();												// 그래픽 설정을 적용하는 함수.
	void InitForGameIn();													// 게임인에 들때 처리하는 초기화 함수.

	// 071025 LYW --- OptionManager : Add function to setting and return tutorial option.
	void Set_ShowTutorial(BOOL val) { m_GameOption.bShowTutorial = val ; }	// 튜토리얼 보이기 여부를 세팅한다.
	BOOL Is_ShowTutorial() { return m_GameOption.bShowTutorial ;}			// 튜토리얼 보이기 여부를 반환한다.
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
//	int nEffectSnow; // 2005.12.28일 눈On/Off 추가 
//
//
//	BOOL bSoundBGM;				//
//	BOOL bSoundEnvironment;		//
//	int	nVolumnBGM;				//
//	int nVolumnEnvironment;		//
//
//	//SW060904 캐릭환경반사값 설정 옵션 추가
//	BOOL bAmbientMax;
//
//	// desc_hseos_옵션01
//	// S 옵션 추가 added by hseos 2007.06.19
//	int	nChatLineNum;
//	int nCurChatMode;
//	// E 옵션 추가 added by hseos 2007.06.19
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
//	//SW050804 옵션창: 방파 데미지 보기 추가
//	BOOL			m_bShowMenberDamage;
//
//	//SW050825 그래픽옵션Tab: GraphicAutoCtrlProcess()
//	COptionDialog*	m_pOptionDlg;
//	cGuageBar*		m_pCammaGB;		//현재 적용은 안됨
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


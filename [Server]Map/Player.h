#pragma once


#include "Object.h"
#include "ExchangeManager.h"
#include "OptionManager.h"
#include "ItemContainer.h"
#include "InventorySlot.h"
#include "StorageSlot.h"
#include "WearSlot.h"
#include "ShopItemSlot.h"
#include "ShopInvenSlot.h"
#include "Purse.h"
/*어빌리티 삭제 - #include "AbilityGroup.h"*/
#include "ShopItemManager.h"

// jsd quest
#include "..\[CC]Quest\QuestDefines.h"
#include "QuestGroup.h"
#include "../[cc]skill/server/tree/skilltree.h"
#include "QuickSlot.h"
// desc_hseos_주민등록01
// S 주민등록 추가 added by hseos 2007.06.04
#include "../hseos/ResidentRegist/SHResidentRegistManager.h"
// E 주민등록 추가 added by hseos 2007.06.04

// 071115 LYW --- Player : 유료 아이템 관련 추가.
//#include "ChargeItemManager.h"


// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2008.01.22
struct stFARMPLAYERINFO																		// 플레이어 정보
{
	UINT				nCropPlantRetryTime;												// ..다시 농작물을 심는데 필요한 시간
	DWORD				nCropPlantRetryTimeTick;											// ..다시 농작물을 심는데 필요한 시간 틱
	UINT				nCropManureRetryTime;												// ..다시 농작물에 비료를 주는데 필요한 시간
	DWORD				nCropManureRetryTimeTick;											// ..다시 농작물에 비료를 주는데 필요한 시간 틱
	UINT				nAnimalFeedRetryTime;												// ..다시 가축에 사료를 주는데 필요한 시간
	DWORD				nAnimalFeedRetryTimeTick;											// ..다시 가축에 사료를 주는데 필요한 시간 틱
	UINT				nAnimalCleanRetryTime;												// ..다시 가축을 씻기는대 필요한 시간
	DWORD				nAnimalCleanRetryTimeTick;											// ..다시 가축을 씻기는대 필요한 시간 틱
};
// E 농장시스템 추가 added by hseos 2008.01.22

class cStreetStall;
class CMonster;
class CQuestBase;
class CQuestScript;
class cSkillObject;

struct SetScript;

enum
{
	PLAYERINITSTATE_ONLY_ADDED			= 1 << 0,
	PLAYERINITSTATE_HERO_INFO			= 1 << 1,
	PLAYERINITSTATE_SKILL_INFO			= 1 << 2,
	PLAYERINITSTATE_ITEM_INFO			= 1 << 3,
	PLAYERINITSTATE_QUICK_INFO			= 1 << 4,
	PLAYERINITSTATE_STORAGE_INFO		= 1 << 5,
	PLAYERINITSTATE_SHOPITEM_USEDINFO	= 1 << 6,	
	PLAYERINITSTATE_ACKMSG_SEND			= 1 << 7,
	PLAYERINITSTATE_INITED				= 1 << 8,
	PLAYERINITSTATE_GMCONTROL_INFO		= 1 << 9,
	PLAYERINITSTATE_QEUST_INFO			= 1 << 11,
	PLAYERINITSTATE_FARM_INFO			= 1 << 12,
	// desc_hseos_챌린지 존_01
	// S 챌린지 존 추가 added by hseos 2007.12.12
	PLAYERINITSTATE_CHALLENGEZONE_INFO	= 1 << 13,		// 챌린지 존
	// E 챌린지 존 추가 added by hseos 2007.12.12

	// desc_hseos_결혼_01
	// S 결혼 추가 added by hseos 2007.12.12
	PLAYERINITSTATE_MARRIAGE_INFO		= 1 << 14,		// 결혼
	// E 결혼 추가 added by hseos 2007.12.12

	// 080917 NYJ - 옵션단계추가
	PLAYERINITSTATE_OPTION_INFO			= 1 << 15,
};

enum
{
	MONEY_ADDITION = 0,
	MONEY_SUBTRACTION,
};

//SW051112 무쌍모드
#define MUSSANG_VALID_TIME		1000*60*10
enum eMussangStates{ eMSS_None=1, eMSS_Ready=2, eMSS_InMode=4, eMSS_LevelUp=8};

#define GETITEM_FLAG_INVENTORY	0x00000001
#define GETITEM_FLAG_WEAR		0x00000002
#define GETITEM_FLAG_PETWEAR	0x00000004
#define WANTED_DESTROYED_TIME 48*3600*1000
class CPlayer : public CObject  
{
//////////////////////////////////////////////////////////////////////////////////////////
/// 변수 선언부

	///--------------------------------------------------------------------/
	/// 케릭터 기본&관리 정보 관련 변수------------------------------------/
private:
	/// 유저 관리 정보
	/// Agent서버 정보
	DWORD				m_UniqueIDinAgent;
	/// 채널 정보
	DWORD				m_dwCurChannel;		// 현재 유저가 존재하는 채널
	DWORD				m_ChannelID;

	/// 유저레벨
	DWORD				m_dwUserLevel;

	/// 접속시간
	DWORD				m_LoginTime;
	DWORD				m_ContinuePlayTime;	
	
	/// 초기화 상태
	int					m_initState;

	/// 케릭터 데이터
	CHARACTER_TOTALINFO m_HeroCharacterInfo;
	HERO_TOTALINFO		m_HeroInfo;

	/// 게임 옵션 정보
	sGAMEOPTION			m_GameOption;

	/// 맵정보
	DWORD				m_dwOldMapNum;

	/// 단축창
	cQuickSlot			m_QuickSlot[8];

	BOOL				m_God;
	/// 케릭터 기본&관리 정보 관련 변수------------------------------------/
	///--------------------------------------------------------------------/

	
	///--------------------------------------------------------------------/
	/// 스테이터스&각종 수치 정보 관련 변수--------------------------------/
private:
	/// 기본 케릭터 스텟과 아이템에 의한 스텟
	PlayerStat	m_charStats;
	PlayerStat	m_itemStats;

	// 080313 LUJ, 세트 아이템 스탯 분리
	PlayerStat	m_SetItemStats;

	Status				mPassiveStatus;
	Status				mRatePassiveStatus;

	friend class		CCharacterCalcManager;

	/// 스테이터스&각종 수치 정보 관련 변수--------------------------------/
	///--------------------------------------------------------------------/


	///--------------------------------------------------------------------/
	/// 전투/행동 정보 관련 변수-------------------------------------------/
private:
	/// 이모티콘 사용 시간
	DWORD		m_dwLastSocietyAction;
	
	/// 대전 관련 정보
	BOOL		m_bShowdown;
	DWORD		m_dwOpPlayerID;

	/// PK모드 시간 정보
	DWORD		m_dwPKModeStartTime;	//with gCurTime
	DWORD		m_dwPKContinueTime;

	//---KES PK 071202
	BOOL	m_bNoExpPenaltyByPK;

	// 080318 LUJ, PK에 의한 경험치 비손실 상태인지 반환
public:
	BOOL	IsNoExpPenaltyByPK() const	{ return m_bNoExpPenaltyByPK; }

private:
	/// 악명치 정보
	DWORD		m_dwLastReduceHour;		//with PlayTime
	void		AddBadFameReduceTime();

	/// 현상범 정보
	WANTEDTYPE	m_WantedIDX;
	cPtrList	m_WantedList;

	/// 자신을 죽인 대상 정보
	WORD		m_MurdererKind;
	DWORD		m_MurdererIDX;
	WORD		m_MurderObjKind;

	/// 무쌍정보
	DWORD		m_dwMussangPoint;
	DWORD		m_dwMussangStartTime;
	BOOL		m_bIsMussangReady;
	BYTE		m_bMussangFlag;
	DWORD		m_dwMussangTime;
	WORD		m_wMussangStat;
	
	/// 죽음/부활 관련 정보
	BOOL		m_bPenaltyByDie;
	BOOL		m_bReadyToRevive; //죽은 상태에서만 씀
	REVIVEDATA	m_ReviveData;
	int			m_ReviveTime;
	DWORD		m_InitPosition;
	BOOL		m_bSavePointInited;

	/// 자신에게 붙은 몬스터
	CYHHashTable<CMonster> m_FollowMonsterList;
	
	/// 전투 능력 수치
	float		mAccuracy;
	float		mAvoid;
	float		mBlock;
	float		mPhysicAttackMax;
	float		mPhysicAttackMin;
	float		mMagicAttack;
	float		mPhysicDefense;
	float		mMagicDefense;
	float		mCriticalRate;
	float		mCriticalDamage;
	float		mLifeRecover;
	float		mManaRecover;
	float		mMaxLife;
	float		mMaxMana;
	// 080910 LUJ, 방패 방어력 수치. 블록 계산에 사용
	DWORD		mShieldDefense;

	/// 문파전 정보
protected:
	BOOL		m_bDieForGFW;		// 문파전상대한테 죽었을때...
	BOOL		m_bDieInSpecialMap;
	WORD		m_wReturnMapNum;
	WORD		m_wGTReturnMapNum;
	int			m_ObserverBattleIdx;

	/// 퀘스트 정보
	CQuestGroup	m_QuestGroup;
public:
	BOOL		m_bNeedRevive;

	cDelay*		mpBattleStateDelay;
	/// 전투/행동 정보 관련 변수-------------------------------------------/
	///--------------------------------------------------------------------/


	///--------------------------------------------------------------------/
	/// 프로세스 관련 변수-------------------------------------------------/
private:
	/// 맵이동 관련
	WORD			m_ChangeBeforeMapNum;
	WORD			m_ChangeWantMapNum;

	/// 로그 프로세스 시간
	DWORD			m_dwProgressTime;

	/// 스피드핵 체크
	int				m_nHackCount;			//skill + move
	DWORD			m_dwHackStartTime;

	/// 회복프로세스 관련 정보
	RECOVER_TIME	m_ManaRecoverTime;
	YYRECOVER_TIME	m_YYManaRecoverTime;

	/// 종료 정보
	BOOL			m_bExit;	//종료플래그
	DWORD			m_dwExitStartTime;
	BOOL			m_bNormalExit;
	BOOL			m_bWaitExitPlayer;

	BOOL			mIsEmergency;
	MAPTYPE			mTargetMap;
	DWORD			mTargetPosX;
	DWORD			mTargetPosZ;

	/// 프로세스 관련 변수-------------------------------------------------/
	///--------------------------------------------------------------------/

	///--------------------------------------------------------------------/
	/// 아이템 정보 관련 변수----------------------------------------------/
	/// 아이템으로 인한 케릭터 수치 정보는 수치 정보 관련 변수에 분류
private:
	/// 각종 인벤토리
	CItemContainer						m_ItemContainer;
	CInventorySlot						m_InventorySlot;
	CWearSlot							m_WearSlot;
	CStorageSlot						m_StorageSlot;
	CShopItemSlot						m_ShopItemSlot;			// ItemMall용	
	CShopInvenSlot						m_ShopInvenSlot;
	CInventoryPurse						m_InventoryPurse;
	CStoragePurse						m_StoragePurse;
	/// 인벤토리 관련 클래스
	friend class						CInventoryPurse;
	
	/// 초기화 관련 변수
	BOOL								m_bGotWarehouseItems;
	BOOL								m_bStorageInit;
	BOOL								m_bShopInit;
	BOOL								m_bPetInvenInit;

	/// 교환 관련
	sEXCHANGECONTAINER					m_ExchangeContainer;

	/// 노점상 관련
	cStreetStall*						m_pGuetStall;
	char								m_StreetStallTitle[MAX_STREETSTALL_TITLELEN+1];

	/// 캐시 아이템
//	CShopItemManager					m_ShopItemManager;

	// 071115 LYW --- Player : 유료 아이템 매니져 관련 추가.
	//CChargeItemManager					m_ChargeItemManager ;
		
	/// 아이템 정보 관련 변수----------------------------------------------/
	///--------------------------------------------------------------------/


	///--------------------------------------------------------------------/
	/// 스킬 정보 관련 변수------------------------------------------------/
	/// 스킬로 인한 케릭터 수치 정보는 수치 정보 관련 변수에 분류
	/// 보유스킬 정보
	cSkillTree					m_SkillTree;

	/// 일반 공격 콤보
	DWORD						m_CurComboNum;

	/// 스킬 실패 횟수
	WORD						m_SkillFailCount;

	CYHHashTable<EXCEPTION>		mExceptionTable;

	BOOL						mbBuffSkillLoad;
	BOOL						mbPetLoad;

	/// 스킬 정보 관련 변수------------------------------------------------/
	///--------------------------------------------------------------------/

	DATE_MATCHING_INFO			m_DateMatching;						// date matching	2007/03/29

	// desc_hseos_몬스터미터01
	// S 몬스터미터 추가 added by hseos 2007.05.23
	stMONSTERMETER							m_stMonstermeterInfo;
	// E 몬스터미터 추가 added by hseos 2007.05.23

	// desc_hseos_주민등록01
	// S 주민등록 추가 added by hseos 2007.06.04
	CSHResidentRegistManager::stRegistInfo	m_stRegistInfo;
	// E 주민등록 추가 added by hseos 2007.06.04

	// desc_hseos_패밀리01
	// S 패밀리 추가 added by hseos 2007.07.06	2007.07.17
	char*									m_pcFamilyEmblem;
	// E 패밀리 추가 added by hseos 2007.07.06	2007.07.17

	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.08.23	2008.01.22
	stFARMPLAYERINFO						m_stFarmInfo;
	// E 농장시스템 추가 added by hseos 2007.08.23	2008.01.22

//---KES Aggro 070918
	CYHHashTable<CMonster>	m_htAggroedTable;	//나를 AggroList에 가지고 있는 몬스터 목록

	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.27	2007.11.28	2007.11.30	2007.12.05
	UINT									m_nChallengeZoneEnterFreq;
	UINT									m_nChallengeZoneEnterBonusFreq;
	UINT									m_nChallengeZonePartnerID;
	UINT									m_nChallengeZoneSection;
	UINT									m_nChallengeZoneStartState;
	DWORD									m_nChallengeZoneStartTimeTick;
	UINT									m_nChallengeZoneMonsterNum;
	UINT									m_nChallengeZoneKillMonsterNum;
	BOOL									m_bChallengeZoneNeedSaveEnterFreq;
	BOOL									m_bChallengeZoneCreateMonRightNow;
	UINT									m_nChallengeZoneMonsterNumTillNow;
	DWORD									m_nChallengeZoneClearTime;
	// E 데이트 존 추가 added by hseos 2007.11.27	2007.11.28	2007.11.30	2007.12.05
	
//-------------------
	// 071128 LYW --- Player : HPMP 적용.
	BYTE m_byHP_Point ;
	BYTE m_byMP_Point ;

	// desc_hseos_결혼_01
	// S 결혼 추가 added by hseos 2007.12.10	2008.01.11
	DWORD									m_nMarriageRequestPartnerID;
	WORD									m_nMarriageTeleportFreq;
	char									m_szMarriageDivorceDate[16+1];
	DWORD									m_nMarriagePartnerID;
	BYTE									m_nMarriagePartnerRace;
	BYTE									m_nMarriagePartnerJobFirst;
	BYTE									m_nMarriagePartnerJobCur;
	BYTE									m_nMarriagePartnerJobGrade;
	WORD									m_nMarriagePartnerLv;
	// E 결혼 추가 added by hseos 2007.12.10	2008.01.11

/// 변수 선언부
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
/// 함수 선언부
public:
	/// 생성자/소멸자
	CPlayer();
	virtual ~CPlayer();

	///--------------------------------------------------------------------/
	/// 케릭터 기본 정보 관련 함수-----------------------------------------/
private:
public:
	/// 초기화
	virtual BOOL	Init( EObjectKind kind, DWORD AgentNum, BASEOBJECT_INFO* pBaseObjectInfo );
	virtual void	Release();
	void			InitBaseObjectInfo( BASEOBJECT_INFO* pBaseInfo );
	void			InitClearData();
	void			InitCharacterTotalInfo( CHARACTER_TOTALINFO* pTotalInfo );
	void			InitHeroTotalInfo( HERO_TOTALINFO* pHeroInfo );
	void			DoSetInited();
	virtual void	SetInitedGrid();

	/// 유저 관리용
	/// Agent서버 정보
	void			SetUniqueIDinAgent( DWORD uniqueid ) { m_UniqueIDinAgent = uniqueid; }
	DWORD			GetUniqueIDinAgent() { return m_UniqueIDinAgent; }

	/// 채널 정보
	void			SetChannelID( DWORD id ) { m_ChannelID = id; }
	DWORD			GetChannelID() { return m_ChannelID; }

	void			SetCurChannel(DWORD dwChannel) {m_dwCurChannel = dwChannel;}
	DWORD			GetCurChannel() {return m_dwCurChannel;}

	/// 유저 레벨
	void			SetUserLevel( DWORD dwUserLevel ) { m_dwUserLevel = dwUserLevel; }
	DWORD			GetUserLevel() { return m_dwUserLevel; }

	/// 플레이시간
	DWORD			GetPlayTime() {	return m_HeroInfo.Playtime;	}
	void			SetLoginTime( int time ) { m_LoginTime = time; }

	/// Grid 시스템 관련
	void			SetAddMsg( char* pAddMsg, WORD* pMsgLen, DWORD dwReceiverID, BOOL bLogin );
	virtual void	SendMsg( MSGBASE* pMsg, int MsgLen );

	/// 기본 정보 획득
	void			GetCharacterTotalInfo( CHARACTER_TOTALINFO* pRtInfo );

	// 080225 LUJ, 기본 정보 반환
	const CHARACTER_TOTALINFO&	GetCharacterTotalInfo() const;

	void			GetHeroTotalInfo( HERO_TOTALINFO* pRtInfo );
	HERO_TOTALINFO*	GetHeroTotalInfo();
	int				GetGender() { return m_HeroCharacterInfo.Gender; }

	/// 게임 옵션
	sGAMEOPTION*	GetGameOption() { return &m_GameOption; }
	void			SetGameOption( sGAMEOPTION* pGameOption ) { m_GameOption = *pGameOption; }

	/// 상태
	int				GetInitState(){ return m_initState; }
	void			SetInitState( int initstate, DWORD protocol );

	void			OnStartObjectState( BYTE State, DWORD dwParam );
	void			OnEndObjectState( BYTE State );

	void			SetPeaceMode( BOOL bPeace ) { m_HeroCharacterInfo.bPeace = bPeace?true:false; }
	BOOL			GetPeaceMode() { return m_HeroCharacterInfo.bPeace; }
	BOOL			IsVisible()	{ return (m_HeroCharacterInfo.bVisible ? TRUE : FALSE);	}
	void			SetVisible( BOOL val ) { m_HeroCharacterInfo.bVisible = val ? true : false; }

	/// 직업 
	// 070415 LYW --- Player : Active and modified functions to setting and return stage.
	//void			SetStage( BYTE stage );
	// 070416 LYW --- Player ; Modified function SetStage.
	void			SetStage( BYTE grade, BYTE index ) ;
	BYTE			GetStage() { return m_HeroCharacterInfo.JobGrade ; }

	// 070410 LYW --- Player : Add functions to return hero info.
	BYTE			GetJop(BYTE idx){ return m_HeroCharacterInfo.Job[idx] ; }
	BYTE			GetRacial()		{ return m_HeroCharacterInfo.Race ; }
	BYTE			GetJopGrade()	{ return m_HeroCharacterInfo.JobGrade ; }

	// 070415 LYW --- Plsyer : Add function to setting job.
	void			SetJob( BYTE jobGrade, BYTE jobIdx ) ;

	/// 맵 정보
	WORD			GetPlayerLoginPoint() { return m_HeroCharacterInfo.LoginPoint_Index; }
	void			SetLoginPoint(WORD LoginPoint) { m_HeroCharacterInfo.LoginPoint_Index = LoginPoint; }
	void			InitMapChangePoint() { m_HeroCharacterInfo.MapChangePoint_Index = 0; }
	void			SetMapChangePoint(WORD point) { m_HeroCharacterInfo.MapChangePoint_Index = point; }	// YH
	WORD			GetMapChangePoint() { return m_HeroCharacterInfo.MapChangePoint_Index;	} //hs for party
	// 080104 LYW --- Player : 퀘스트의 실행으로 맵이동하는 함수 수정.
	void			SendPlayerToMap(MAPTYPE mapNum, float xpos, float zpos) ;
	//void			SendPlayerToMap();	
	
	void			SetInitPoint( DWORD Pos ) {	m_InitPosition = Pos; }
	DWORD			GetInitPoint() { return m_InitPosition; }
	BOOL			GetSavePointInit() { return m_bSavePointInited; }
	void			SetSavePointInit( BOOL val ) { m_bSavePointInited = val; }
	// 070627 LYW --- Player : Add function to return map number.
	MAPTYPE			GetCurMapNum() { return m_HeroCharacterInfo.CurMapNum ; }
	

	/// 케릭터 변환
	void			SetCharChangeInfo( CHARACTERCHANGE_INFO* pInfo );	

	BOOL			GetKyungGongIdx() { return m_MoveInfo.KyungGongIdx; }

	BOOL			AddQuick( BYTE sheet, WORD pos, SLOT_INFO* pSlot );
	SLOT_INFO*		GetQuick( BYTE sheet, WORD pos );

	void			SetGod( BOOL val ) { m_God = val; }
	BOOL			IsGod() { return m_God; }
	/// 케릭터 기본 정보 관련 함수-----------------------------------------/
	///--------------------------------------------------------------------/

	
	///--------------------------------------------------------------------/
	/// 스테이터스&각종 수치 정보 관련 함수--------------------------------/
private:
public:
	/// 레벨업시 스텟포인트
	LEVELTYPE			GetPlayerLevelUpPoint() { return m_HeroInfo.LevelUpPoint;	}
	void				SetPlayerLevelUpPoint( LEVELTYPE val );

	// 080611 LYW --- Player : 스킬포인트 업데이트 처리를 수정함.
	// (아이템으로 스킬 포인트를 추가하는 기능이 생겼기 때문.)
	//void SetSkillPoint( DWORD point ) ;
	void SetSkillPoint( DWORD point, BYTE byForced ) ;

	DWORD				GetSkillPoint();
	/// 레벨 관련
	LEVELTYPE			GetMaxLevel() { return m_HeroInfo.MaxLevel; }
	virtual				LEVELTYPE GetLevel();
	virtual void		SetLevel( LEVELTYPE level );

    /// 경험치
	EXPTYPE				GetPlayerExpPoint() { return m_HeroInfo.ExpPoint; }
	DOUBLE				GetPlayerTotalExpPoint();
	void				SetPlayerExpPoint( EXPTYPE point, BYTE bDie=0 );
	// 070411 LYW --- Player : Add function to set experience.
	//void				SetPlayerExpPointDown(EXPTYPE point,BYTE bDie=0) ;
	void				AddPlayerExpPoint( EXPTYPE Exp );
	void				ReduceExpPoint( EXPTYPE minusExp, BYTE bLogType );

	/// 명성/악명 관련
	FAMETYPE			GetFame() { return m_HeroInfo.Fame; }
	void				SetFame(FAMETYPE val) { m_HeroInfo.Fame = val; }
	FAMETYPE			GetBadFame() { return m_HeroCharacterInfo.BadFame; }
	void				SetBadFame(FAMETYPE val) { m_HeroCharacterInfo.BadFame = val; }

	/// 기본 스텟 정보
	PlayerStat&	GetCharStats() { return m_charStats; }
	PlayerStat&	GetItemStats() { return m_itemStats; }

	// 080313 LUJ, 세트 스탯 분리
	PlayerStat& GetSetItemStats() { return m_SetItemStats; };

	// 070410 LYW --- Player : Add functions to return hero info.
	DWORD				GetRealVitality()		{ return m_HeroInfo.Vit ; }
	DWORD				GetRealWisdom()			{ return m_HeroInfo.Wis ; }
	DWORD				GetRealStrength()		{ return m_HeroInfo.Str ; }
	DWORD				GetRealDexterity()		{ return m_HeroInfo.Dex ; }
	DWORD				GetRealIntelligence()	{ return m_HeroInfo.Int ; }

	DWORD				GetVitality();
	DWORD				GetWisdom();
	DWORD				GetStrength();
	DWORD				GetDexterity();
	DWORD				GetIntelligence();
	
	void				SetVitality( DWORD val );
	void				SetWisdom( DWORD val );
	void				SetStrength( DWORD val );
	void				SetDexterity( DWORD val );
	void				SetIntelligence( DWORD val );

	void				AddStrength( int val );
	void				AddWisdom( int val );
	void				AddDexterity( int val );
	void				AddVitality( int val );
	void				AddIntelligence( int val );

	/// 생명력/마나 정보
	BYTE				GetLifePercent();
	BYTE				GetManaPercent();
	void				SetMaxLife( DWORD val );
	void				SetMaxMana( DWORD val );

	// 080625 LYW --- Player : 생명력 세팅 함수 수정.
	// 캐릭터가 죽은 상태에서 강종이나 튕김 현상이 발생 할 때, 
	// 캐릭터의 생명력/마나 30%를 복구해 주어야 한다. 그런데 상태가 죽은 상태면, 
	// 기존 함수는 return 처리를 하므로, 강제로 복구 여부를 세팅할 수 있는 함수를 추가한다.
	void SetLifeForce( DWORD Life, BYTE byForce, BOOL bSendMsg = TRUE) ;
	void SetManaForce(DWORD Mana, BYTE byForce, BOOL bSendMsg = TRUE) ;

	virtual DWORD		GetLife();
	virtual void		SetLife( DWORD Life, BOOL bSendMsg = TRUE );

	virtual DWORD		GetMana();
	virtual void		SetMana( DWORD valb, BOOL SendMsg = TRUE );	

	virtual DWORD		DoGetMaxLife();
	virtual DWORD		DoGetMaxMana();

	float				GetMaxLifeWithItem();
	float				GetMaxManaWithItem();

	/// 방어력/공격력
	virtual DWORD		DoGetPhyDefense();
	virtual float		DoGetAttDefense( WORD Attrib );
	virtual float		GetAttribPlusPercent( WORD Attrib );
	virtual DWORD		DoGetPhyAttackPowerMin();
	virtual DWORD		DoGetPhyAttackPowerMax();

	/// 사정거리
	virtual float		DoGetAddAttackRange() { return GetDexterity() / 3.f; }
	/// 크리티컬 확률
	virtual DWORD		DoGetCritical();
	virtual DWORD		DoGetDecisive();

	/// 이동속도 계산
	virtual float		DoGetMoveSpeed();
	WORD				GetKyungGongGrade()	{ return m_HeroInfo.KyungGongGrade; }
	void				SetKyungGongGrade(WORD grade) { m_HeroInfo.KyungGongGrade = grade; }

	/// 명중/회피

	/// 스텟 재계산
	void				CalcState();

	/// 스테이터스&각종 수치 정보 관련 함수--------------------------------/
	///--------------------------------------------------------------------/
	

	///--------------------------------------------------------------------/
	/// 전투/행동 관련 함수------------------------------------------------/
	
	/// 자신을 죽인 대상 정보
private:
	void						SetMurderIDX( DWORD CharacterIDX ) { m_MurdererIDX = CharacterIDX; }
	void						SetMurderKind( WORD Kind ) { m_MurdererKind = Kind; }
public:
	void						SetMurderObjKind( WORD ObjKind ) {	m_MurderObjKind = ObjKind;	}
	WORD						GetMurderObjKind() {	return m_MurderObjKind;	}
	void						ClearMurderIdx();
	DWORD						GetMurderIDX() { return m_MurdererIDX;	}
	WORD						GetMurderKind() { return m_MurdererKind;}

	/// 부활
	void						SetPenaltyByDie( BOOL bVal ) { m_bPenaltyByDie = bVal; }	//방파 비무 시 상대에 의해 죽었을 시 TRUE //강제종료 시 패널티 체크를 위한 변수
	BOOL						IsPenaltyByDie() { return m_bPenaltyByDie; }
	BOOL						IsReadyToRevive() { return m_bReadyToRevive;	}
	void						SetReadyToRevive( BOOL val ) { m_bReadyToRevive = val;	}
	// 080602 LYW --- Player : 경험치 수치 (__int32) 에서 (__int64) 사이즈로 변경 처리.
	//DWORD						RevivePenalty(BOOL bAdditionPenalty) ;
	EXPTYPE						RevivePenalty(BOOL bAdditionPenalty) ;
	void						RevivePresentSpot();
	void						ReviveLogIn();
	void						ReviveAfterShowdown( BOOL bSendMsg = TRUE );
	void						ReviveLogInPenelty();
	// 080602 LYW --- Player : 경험치 수치 (__int32) 에서 (__int64) 사이즈로 변경 처리.
	//DWORD						ReviveBySkill();
	EXPTYPE						ReviveBySkill();

	// 070831 웅주, 길드전 중 사망 시 마을에서 부활 처리
	void						ReviveTownByGuildWar();
	BOOL						IsAbleReviveOther();
	BOOL						ReviveShopItem();
	void						SetReviveData( DWORD Id, WORD ItemIdx, POSTYPE ItemPos )
	{	
		m_ReviveData.TargetID=Id;	
		m_ReviveData.ItemIdx=ItemIdx;	
		m_ReviveData.ItemPos=ItemPos;	
	}
	REVIVEDATA*					GetReviveData() { return &m_ReviveData; }
	void						SetReviveTime( DWORD time )	{ m_ReviveTime = time; }

	/// 행동/이모티콘
	BOOL						CanSocietyAction( DWORD curTime );

	/// 파티 정보
	DWORD						GetPartyIdx() { return m_HeroInfo.PartyID; }
	void						SetPartyIdx( DWORD PartyIDx ) 
								{ 
									m_HeroInfo.PartyID = PartyIDx; 
								}
	void						SendPlayerLifeToParty( DWORD life, DWORD PartyIdx );
	void						SendPlayerManaToParty( DWORD mana, DWORD PartyIdx );
	void						SendMoveInfotoParty();

	/// 길드 정보
	DWORD						GetGuildIdx() const { return m_HeroCharacterInfo.MunpaID;	}
	void						SetGuildIdx( DWORD GuildIdx ) { m_HeroCharacterInfo.MunpaID = GuildIdx; }
	void						SetGuildInfo( DWORD GuildIdx, BYTE Grade, const char* GuildName, MARKNAMETYPE MarkName );
	BYTE						GetGuildMemberRank() { return m_HeroCharacterInfo.PositionInMunpa; 	}
	void						SetGuildMemberRank( BYTE rank ) { m_HeroCharacterInfo.PositionInMunpa = rank; 	} //문파에서의 자신의 지위 설정
	void						SetGuildName( char* GuildName ) { SafeStrCpy( m_HeroCharacterInfo.GuildName, GuildName, MAX_MUNPA_NAME+1 ); }
	void						SetGuildMarkName( MARKNAMETYPE MarkName );
	char*						GetGuildCanEntryDate();
	void						SetGuildCanEntryDate( char* date );
	void						ClrGuildCanEntryDate();
	void						SetNickName( char* NickName );

	/// 동맹 길드
	DWORD						GetGuildUnionIdx() { return m_HeroCharacterInfo.dwGuildUnionIdx; }
	void						InitGuildUnionInfo( DWORD dwGuildUnionIdx, char* pGuildUnionName, DWORD dwMarkIdx );
	void						SetGuildUnionInfo( DWORD dwGuildUnionIdx, char* pGuildUnionName, DWORD dwMarkIdx );
	void						SetGuildUnionMarkIdx( DWORD dwMarkIdx )	{ m_HeroCharacterInfo.dwGuildUnionMarkIdx = dwMarkIdx; }

	/// 길드 정보
	DWORD						GetFamilyIdx() { return m_HeroCharacterInfo.FamilyID;	}
	void						SetFamilyIdx( DWORD FamilyIdx ) { m_HeroCharacterInfo.FamilyID = FamilyIdx; }
	void						SetFamilyInfo( DWORD FamilyIdx, BYTE Grade, char* FamilyName, MARKNAMETYPE MarkName );
	BYTE						GetFamilyMemberRank() { return m_HeroCharacterInfo.PositionInFamily; 	}
	void						SetFamilyMemberRank( BYTE rank ) { m_HeroCharacterInfo.PositionInFamily = rank; 	} //문파에서의 자신의 지위 설정
	void						SetFamilyName( char* FamilyName ) { SafeStrCpy( m_HeroCharacterInfo.FamilyName, FamilyName, sizeof( m_HeroCharacterInfo.FamilyName ) ); }
	void						SetFamilyMarkName( MARKNAMETYPE MarkName );
	char*						GetFamilyCanEntryDate();
	void						SetFamilyCanEntryDate( char* date );
	void						ClrFamilyCanEntryDate();
	void						SetFamilyNickName( char* NickName );

	/// 동맹 길드
	//DWORD						GetFamilyUnionIdx() { return m_HeroCharacterInfo.dwFamilyUnionIdx; }
	//void						InitFamilyUnionInfo( DWORD dwFamilyUnionIdx, char* pFamilyUnionName, DWORD dwMarkIdx );
	//void						SetFamilyUnionInfo( DWORD dwFamilyUnionIdx, char* pFamilyUnionName, DWORD dwMarkIdx );
	//void						SetFamilyUnionMarkIdx( DWORD dwMarkIdx )	{ m_HeroCharacterInfo.dwFamilyUnionMarkIdx = dwMarkIdx; }

	/// 현상범
	void						ClearWantedList();
	WANTEDTYPE					GetWantedIdx() { return m_WantedIDX; }
	void						SetWantedIdx( WANTEDTYPE val ) { m_WantedIDX = val; }
	DWORD						GetWantedNum() { return m_WantedList.GetCount(); }
	void						AddWanted( WANTEDTYPE WantedIDX );
	void						DeleteWanted( WANTEDTYPE WantedIDX );
	BOOL						IsWantedOwner( WANTEDTYPE WantedIDX );

	/// 퀘스트
	void						AddQuest( CQuestBase* pQuest );
	void						RemoveQuest( DWORD QuestIdx );
	BOOL						SetQuestState( DWORD QuestIdx, QSTATETYPE value );
	CYHHashTable<CQuestBase>	m_QuestList;
	CQuestGroup*				GetQuestGroup()	{ return &m_QuestGroup; }
	
	/// 몬스터
	BOOL						AddFollowList( CMonster * pMob );
	void						RemoveFollowList( DWORD ID );
	BOOL						RemoveFollowAsFarAs( DWORD GAmount, CObject* pObject );
	WORD						m_wKillMonsterLevel;
	void						SetKillMonsterLevel( WORD wMonLevel ) { m_wKillMonsterLevel = wMonLevel; }
	WORD						GetKillMonsterLevel() { return m_wKillMonsterLevel; }
	virtual	DWORD				GetGravity();
	virtual void				SetGravity(DWORD gravity) {};

//---KES Aggro 070918
	void						AddToAggroed( CMonster* pMonster );		//void CMonster::CalcAggro() 함수에서만 호출한다. 다른곳에서 호출 금지!
	void						RemoveFromAggroed( DWORD dwObjectID );	//void CMonster::RemoveFromAggro(), RemoveAllAggro() 함수에서만 호출한다. 다른곳에서 호출 금지!
	void						RemoveAllAggroed();

	void						AddAggroToMyMonsters( int nAggroAdd, CObject* pTarget );
//-------------------
	/// 대전
	void 						SetShowdown( BOOL bShowdown ) { m_bShowdown = bShowdown; }
	BOOL 						IsShowdown() { return m_bShowdown; }
	void 						SetShowdownOpPlayerID( DWORD dwOpPlayerID ) { m_dwOpPlayerID = dwOpPlayerID; }
	DWORD 						GetShowdownOpPlayerID()	{ return m_dwOpPlayerID; }

	/// PK
	int  						PKModeOn();
	BOOL  						PKModeOff();
	void  						PKModeOffForce();
	BOOL  						IsPKMode()							{ return m_HeroCharacterInfo.bPKMode; }
	void  						SetPKStartTimeReset()				{ m_dwPKModeStartTime = gCurTime; }
	//---KES PK 071202
	DWORD						GetPKContinueTime()					{ return m_dwPKContinueTime; }
	void  						SetPKContinueTime( DWORD dwTime )	{ m_dwPKContinueTime = dwTime; }
	//----------------
	void  						SetPKModeEndtime();

	/// 길드 토너먼트
	void  						SetReturnMapNum( WORD num )	{ m_wGTReturnMapNum = num; }
	WORD  						GetReturnMapNum() { return m_wGTReturnMapNum; }
	void  						SetObserverBattleIdx( int Idx ) { m_ObserverBattleIdx = Idx; }
	int	 						GetObserverBattleIdx() { return m_ObserverBattleIdx; }
	
	/// 길드 필드전
	void						SetDieForGFW( BOOL bDie ) { m_bDieForGFW = bDie; }
	BOOL						IsDieFromGFW() { return m_bDieForGFW; }
	void						SetDieInSpecialMap( BOOL bDie, WORD wReturnMap ) { m_bDieInSpecialMap = bDie; m_wReturnMapNum = wReturnMap; }	

	/// 공성전
	void  						SetRestraintMode( bool val ) { m_HeroCharacterInfo.bRestraint = val; }
	BOOL  						IsRestraintMode() { return m_HeroCharacterInfo.bRestraint; }
	void  						ReviveVillage();

	/// 무쌍모드
	void						InitMussangPoint() { m_dwMussangPoint = 0;	}
	void						SetMussangMode(BOOL bMode) { m_HeroCharacterInfo.bMussangMode = bMode; }
	BOOL						IsMussangMode() { return m_HeroCharacterInfo.bMussangMode; }
	BOOL						IsMussangReady() { return m_bIsMussangReady; }
	int							StartMussangMode();
	void						CheckMussangTime();
	void						SetMussangMaxPoint();
	DWORD						GetMussangMaxPoint();
	void						AddMussangPointFromLevel( DWORD dwMonsterLevel );
	void						SendMussangInfo();
	void						SendMussangEnd();	//캐릭 죽었을 시 또는 지속 시간 종료시
	void						CheatMussangReady();
	WORD						GetMussangStat() { return m_wMussangStat; }

	/// 전투 능력 수치
	float						GetAccuracy();
	float						GetAvoid();
	float						GetBlock();
	float						GetPhysicAttackMax();
	float						GetPhysicAttackMin();
	float						GetMagicAttack();
	float						GetPhysicDefense();
	float						GetMagicDefense();
	float						GetCriticalRate();
	float						GetCriticalDamage();
	float						GetLifeRecover();
	float						GetManaRecover();
	// 080910 LUJ, 방패의 방어력을 반환한다
	DWORD						GetShieldDefense();
	// 080910 LUJ, 방패의 방어력을 설정한다
	void						SetShieldDefence( DWORD );

	
	///--------------------------------------------------------------------/
	/// 프로세스 관련 함수-------------------------------------------------/
private:
public:
	/// 스피드 핵 체크
	void			SpeedHackCheck();

	/// 기본 상태 프로세스
	virtual void	DoDie( CObject* pAttacker );
	virtual void	DoDamage( CObject* pAttacker, RESULTINFO* pDamageInfo, DWORD beforeLife );
	virtual DWORD	Damage( CObject* pAttacker, RESULTINFO* pDamageInfo );

	virtual void	StateProcess();
	
	/// 퀘스트 프로세스
	void			QuestProcess();

	/// 종료
	void			SetWaitExitPlayer( BOOL bWait ) { m_bWaitExitPlayer = bWait; }
	BOOL			IsWaitExitPlayer() { return m_bWaitExitPlayer; }
	BOOL			IsExitStart() { return m_bExit; }
	int				CanExitStart();
	void			SetExitStart( BOOL bExit );
	int				CanExit();
	void			ExitCancel();
	void			SetNormalExit() { m_bNormalExit = TRUE; }
	BOOL			IsNormalExit() { return m_bNormalExit; }
	void			UpdateLogoutToDB(BOOL val = TRUE);
	
	void			SetEmergency() { mIsEmergency = TRUE; }
	BOOL			IsEmergency() { return mIsEmergency; }
	
	void			SetMapMoveInfo( MAPTYPE map, DWORD pos_x, DWORD pos_z )
	{
		mTargetMap = map;
		mTargetPosX = pos_x;
		mTargetPosZ = pos_z;
	}

	void			GetMapMoveInfo( MAPTYPE &map, DWORD &pos_x, DWORD &pos_z )
	{
		map = mTargetMap;
		pos_x =	mTargetPosX;
        pos_z =	mTargetPosZ;
	}

	/// 프로세스 관련 함수-------------------------------------------------/
	///--------------------------------------------------------------------/
	
	
	///--------------------------------------------------------------------/
	/// 아이템 정보 관련 함수----------------------------------------------/
public:
	/// 아이템 획득
	ITEMOBTAINARRAYINFO*				GetArray( WORD id );
	void								AddArray( ITEMOBTAINARRAYINFO * pInfo );
	void								RemoveArray( DWORD id );
	CYHHashTable<ITEMOBTAINARRAYINFO>	m_ItemArrayList;

	/// 각종 인벤토리
	CItemSlot *							GetSlot( POSTYPE absPos );
	CItemSlot *							GetSlot( eITEMTABLE tableIdx );

	void								AddStorageItem( ITEMBASE * pStorageItem );
	void								SetStorageNum( BYTE n );
	BYTE								GetStorageNum();

	/// 초기화 관련
	void								InitItemTotalInfo( ITEM_TOTALINFO* pItemInfo );
	
	void								InitStorageItemInfo( STORAGE_ITEM* pItemInfo );
	void								InitStorageInfo( BYTE storagenum,MONEYTYPE money );
	BOOL								IsStorageInit() { return m_bStorageInit; }
	
	BOOL								IsGotWarehouseItems() { return m_bGotWarehouseItems; }
	void								SetGotWarehouseItems( BOOL bGot ) { m_bGotWarehouseItems = bGot; }
	
	// 071125 KTH
	void								SetShopItemInit( BOOL bInit ) { m_bShopInit = bInit; }
	BOOL								GetShopItemInit() { return m_bShopInit; };
	void								InitShopItemInfo( SHOP_ITEM* pItemInfo );
	void								SendShopItemInfo();
//	void								InitShopInvenInfo( ITEMBASE* pShopWareHouse );

	/// 아이템 정보
	void								GetItemtotalInfo( ITEM_TOTALINFO* pRtInfo, DWORD dwFlag );
				
	void								GetStorageItemInfo( STORAGE_ITEM * pRtInfo );
	
	DWORD								GetWearedItemIdx( DWORD WearedPosition ) { return m_HeroCharacterInfo.WearedItemIdx[WearedPosition]; }
	void								SetWearedItemIdx( DWORD WearedPosition, DWORD ItemIdx );
	DWORD								GetWearedWeapon() { return m_HeroCharacterInfo.WearedItemIdx[eWearedItem_Weapon]; }
	WORD								GetWeaponEquipType();
	// 080703 LUJ, 반환 타입을 enum으로 변경
	eWeaponAnimationType				GetWeaponAniType();
	
	/// 아이템 사용
	BOOL								UseItem( WORD abs_pos );
	BOOL								UseConsumptionItem( WORD abs_pos );
	BOOL								UseEquipItem( WORD abs_pos );
	BOOL								UseSkillItem( WORD abs_pos );

    /// 돈 관련
	MONEYTYPE							SetMoney( MONEYTYPE ChangeValue, BYTE bOper, BYTE MsgFlag = 0, eITEMTABLE tableIdx = eItemTable_Inventory, BYTE LogType = 0, DWORD TargetIdx = 0 );
	MONEYTYPE							GetMoney( eITEMTABLE tableIdx = eItemTable_Inventory );
	MONEYTYPE							GetMaxPurseMoney( eITEMTABLE TableIdx );
	void								SetMaxPurseMoney( eITEMTABLE TableIdx, DWORD MaxMoney );
	BOOL								IsEnoughAdditionMoney( MONEYTYPE money, eITEMTABLE tableIdx = eItemTable_Inventory );
	void								RSetMoney( MONEYTYPE money, BYTE flag );	
private:
	void								MoneyUpdate( MONEYTYPE money ); //(Money Class에서 호출 ), DB에 Set
public:

	/// 캐시 아이템
//	SHOPITEMOPTION*						GetShopItemStats() { return &m_ShopItemOption; }
//	AVATARITEMOPTION*					GetAvatarOption() { return &m_AvatarOption; }
//	CShopItemManager*					GetShopItemManager() { return &m_ShopItemManager; }

	// 071115 LYW --- Player : 유료 아이템 매니져 관련 추가.
	//CChargeItemManager*					GetChargeItemManager() { return &m_ChargeItemManager ; }
	
	/// 교환창
	void								SetExchangeContainer( sEXCHANGECONTAINER* ExchangeContainer ) { m_ExchangeContainer = *ExchangeContainer; }
	sEXCHANGECONTAINER*					GetExchangeContainer() { return &m_ExchangeContainer; }
	
	/// 노점상
	void								SetGuestStall( cStreetStall* pStall ) { m_pGuetStall = pStall; }
	cStreetStall*						GetGuestStall() { return m_pGuetStall; }
	void								SetStreetStallTitle( char* title );
	void								GetStreetStallTitle( char* title );
	
	/// 아이템 정보 관련 함수----------------------------------------------/
	///--------------------------------------------------------------------/

	
	///--------------------------------------------------------------------/
	/// 스킬 정보 관련 함수------------------------------------------------/

	// 080509 LUJ, 스킬 쿨타임 체크
public:	
	// 080509 LUJ, 스킬 쿨타임이 지나지 않았으면 참을 반환한다
	// 080516 LUJ, 실패 카운트 체크를 저장하기 위해 상수 선언 제거
	BOOL IsCoolTime( const ACTIVE_SKILL_INFO& );
	// 080511 LUJ, 스킬 쿨타임을 설정해놓는다
	void SetCoolTime( const ACTIVE_SKILL_INFO& );

private:
	// 080509 LUJ, 키: 스킬 인덱스, 값: 만료될 시점의 틱카운트
	typedef stdext::hash_map< DWORD, DWORD >	SkillCoolTimeMap;
	// 080617 LUJ, CPlayer가 풀에 할당되고 해제되는 과정에서 메모리 침범이 발생하는 것으로 추측. 이에 따라 포인터로 변경
	SkillCoolTimeMap							*mSkillCoolTimeMap;

	// 080514 LUJ, 스킬 애니메이션 타임 체크. 애니메이션 시간 동안 다른 스킬이 발동할 수 없도록 채쿠
	DWORD mSkillAnimationTime;    
	// 080515 LUJ, 쿨타임 체크 실패 관리용 구조체
	struct CheckCoolTime
	{
		// 080515 LUJ, 쿨타임 체크가 실패한 최초 틱카운트
		DWORD mCheckedTick;
		// 080515 LUJ, 쿨타임 체크 실패 회수
		DWORD mFailureCount;
	}
	mCheckCoolTime;

public:
	/// 보유 스킬 정보 관련
	cSkillTree*					GetSkillTree() { return &m_SkillTree; }
	int							GetSkillLevel( DWORD SkillIdx );

	/// 스킬 경험치&레벨
	void						SetPlayerSkillLevel( DWORD SkillIdx, BYTE level, WORD Reason );
	void						SkillLevelDown (DWORD SkillIdx, WORD Reason );

	/// 기본공격
	void						SetCurComboNum(DWORD combonum) { m_CurComboNum = combonum; }
	DWORD						GetCurComboNum() { return m_CurComboNum; }

	/// 스킬 실패 체크
	BOOL						CanSkillState();

	Status*						GetPassiveStatus() { return &mPassiveStatus; }
	Status*						GetRatePassiveStatus() { return &mRatePassiveStatus; }


	void						SetHide( WORD level );
	void						SetDetect( WORD level );
	WORD						IsHide() { return m_HeroCharacterInfo.HideLevel; }
	WORD						IsDetect() { return m_HeroCharacterInfo.DetectLevel; }

	void						AddSkillException( EXCEPTION* pException );
	void						RemoveSkillException( EXCEPTION* pException );
	EXCEPTION*					GetException( WORD TargetKind, DWORD TargetIndex );

	/// 스킬 정보 관련 함수------------------------------------------------/
	///--------------------------------------------------------------------/

	// desc_hseos_몬스터미터01
	// S 몬스터미터 추가		 added by hseos 2007.05.23
	void						SetMonstermeterPlaytime(DWORD nTime, DWORD nTimeTotal)		{ m_stMonstermeterInfo.nPlayTime = nTime; m_stMonstermeterInfo.nPlayTimeTotal = nTimeTotal; }
	void						SetMonstermeterKillMonNum(DWORD nNum, DWORD nNumTotal)		{ m_stMonstermeterInfo.nKillMonsterNum = nNum; m_stMonstermeterInfo.nKillMonsterNumTotal = nNumTotal; }
	void						SetMonstermeterPlaytimeTick(DWORD nTick)					{ m_stMonstermeterInfo.nPlayTimeTick = nTick; }

	stMONSTERMETER*				GetMonstermeterInfo()										{ return &m_stMonstermeterInfo; }

	void						ProcMonstermeterPlayTime();
	void						ProcMonstermeterKillMon();
	// E 몬스터미터 추가 added by hseos 2007.05.23

	// desc_hseos_주민등록01
	// S 주민등록 추가 added by hseos 2007.06.04	2007.06.09
	DATE_MATCHING_INFO*								GetResidentRegistInfo()			{ return &m_DateMatching; }
	CSHResidentRegistManager::stRegistInfo*			GetResidentRegistInfo2()		{ return &m_stRegistInfo; }
	void											SetResidentRegistInfo(DATE_MATCHING_INFO* pInfo);

	void		SetDateMatchingSerchTimeTick(DWORD nTick)			{ m_DateMatching.nSerchTimeTick = nTick; }
	void		SetDateMatchingChatPlayerID(DWORD nChatPlayerID)	{ m_DateMatching.nChatPlayerID = nChatPlayerID; }
	void		SetDateMatchingChatState(DWORD nState)				{ m_DateMatching.nRequestChatState = nState; }
	void		SetDateMatchingChatTimeTick(DWORD nTick)			{ m_DateMatching.nRequestChatTimeTick = nTick; }
	// E 주민등록 추가 added by hseos 2007.06.04	2007.06.09

	// desc_hseos_패밀리01
	// S 패밀리 추가 added by hseos 2007.07.06	2007.07.17	2007.11.22
	void						SetFamilyEmblem(char* pEmblem)			{ m_pcFamilyEmblem = pEmblem; }
	char*						GetFamilyEmblem()						{ return m_pcFamilyEmblem; }

	void						IncreaseFamilyEmblemChangedFreq()		{ m_HeroCharacterInfo.nFamilyEmblemChangedFreq++; }
	void						SetFamilyEmblemChangedFreq(DWORD nFreq)	{ m_HeroCharacterInfo.nFamilyEmblemChangedFreq = nFreq; }
	DWORD						GetFamilyEmblemChangedFreq()			{ return m_HeroCharacterInfo.nFamilyEmblemChangedFreq; }
	// E 패밀리 추가 added by hseos 2007.07.06	2007.07.17	2007.11.22

	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.08.23
	stFARMPLAYERINFO*				GetFarmInfo()							{ return &m_stFarmInfo; }
	void							SetFarmCropPlantRetryTime(UINT nTime)	{ m_stFarmInfo.nCropPlantRetryTime = nTime; m_stFarmInfo.nCropPlantRetryTimeTick = gCurTime; }
	void							SetFarmCropManureRetryTime(UINT nTime)	{ m_stFarmInfo.nCropManureRetryTime = nTime; m_stFarmInfo.nCropManureRetryTimeTick = gCurTime; }
	void							ProcFarmTime();
	// E 농장시스템 추가 added by hseos 2007.08.23

	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.27	2007.12.05	2008.01.22
	UINT							GetChallengeZoneEnterFreq()						{ return m_nChallengeZoneEnterFreq; }
	UINT							GetChallengeZoneEnterBonusFreq()				{ return m_nChallengeZoneEnterBonusFreq; }
	void							IncreaseChallengeZoneEnterFreq()				{ m_nChallengeZoneEnterFreq++; }
	void							SetChallengeZoneEnterFreq(UINT nFreq)			{ m_nChallengeZoneEnterFreq = nFreq; }
	void							SetChallengeZoneEnterBonusFreq(UINT nFreq)		{ m_nChallengeZoneEnterBonusFreq = nFreq; }

	UINT							GetChallengeZonePartnerID()						{ return m_nChallengeZonePartnerID; }
	void							SetChallengeZonePartnerID(DWORD nPlayerID)		{ m_nChallengeZonePartnerID = nPlayerID; }

	UINT							GetChallengeZoneSection()						{ return m_nChallengeZoneSection; }
	void							SetChallengeZoneSection(UINT nSection)			{ m_nChallengeZoneSection = nSection; }
	UINT							GetChallengeZoneStartState()					{ return m_nChallengeZoneStartState; }
	void							SetChallengeZoneStartState(UINT nState)			{ m_nChallengeZoneStartState = nState; }
	UINT							GetChallengeZoneStartTimeTick()					{ return m_nChallengeZoneStartTimeTick; }
	void							SetChallengeZoneStartTimeTick(UINT nTimeTick)	{ m_nChallengeZoneStartTimeTick = nTimeTick; }

	UINT							GetChallengeZoneMonsterNum()					{ return m_nChallengeZoneMonsterNum; }
	void							SetChallengeZoneMonsterNum(UINT nNum)			{ m_nChallengeZoneMonsterNum = nNum; }
	UINT							GetChallengeZoneKillMonsterNum()				{ return m_nChallengeZoneKillMonsterNum; }
	void							SetChallengeZoneKillMonsterNum(UINT nNum)		{ m_nChallengeZoneKillMonsterNum = nNum; }
	void							IncreaseChallengeZoneKillMonsterNum()			{ m_nChallengeZoneKillMonsterNum++; }

	UINT							GetChallengeZoneNeedSaveEnterFreq()				{ return m_bChallengeZoneNeedSaveEnterFreq; }
	void							SetChallengeZoneNeedSaveEnterFreq(BOOL bNeed)	{ m_bChallengeZoneNeedSaveEnterFreq = bNeed; }

	UINT							IsChallengeZoneCreateMonRightNow()				{ return m_bChallengeZoneCreateMonRightNow; }
	void							SetChallengeZoneCreateMonRightNow(BOOL bSet)	{ m_bChallengeZoneCreateMonRightNow = bSet; }

	UINT							GetChallengeZoneMonsterNumTillNow()				{ return m_nChallengeZoneMonsterNumTillNow; }
	void							SetChallengeZoneMonsterNumTillNow(UINT nNum)	{ m_nChallengeZoneMonsterNumTillNow = nNum; }

	DWORD							GetChallengeZoneClearTime()						{ return m_nChallengeZoneClearTime; }
	void							SetChallengeZoneClearTime(DWORD nClearTime)		{ m_nChallengeZoneClearTime = nClearTime; }
	// E 데이트 존 추가 added by hseos 2007.11.27	2007.12.05	2008.01.22

	// desc_hseos_결혼_01
	// S 결혼 추가 added by hseos 2007.12.10	2007.12.12
	BOOL							IsMarried()										{ return m_nMarriagePartnerID; }
	DWORD							GetMarriageRequestPartnerID()					{ return m_nMarriageRequestPartnerID; }
	void							SetMarriageRequestPartnerID(DWORD nPartnerID)	{ m_nMarriageRequestPartnerID = nPartnerID; }
	DWORD							GetMarriagePartnerID()							{ return m_nMarriagePartnerID; }
	void							SetMarriagePartnerID(DWORD nPartnerID)			{ m_nMarriagePartnerID = nPartnerID; }
	WORD							GetMarriageTeleportFreq()						{ return m_nMarriageTeleportFreq; }
	void							SetMarriageTeleportFreq(WORD nFreq)				{ m_nMarriageTeleportFreq = nFreq; }
	char*							GetMarriageDivorceDate()						{ return m_szMarriageDivorceDate; }
	void							SetMarriageDivorceDate(char* pszDate)			{ SafeStrCpy(m_szMarriageDivorceDate, pszDate, sizeof(m_szMarriageDivorceDate)); }

	BYTE							GetMarriagePartnerRace()							{ return m_nMarriagePartnerRace; }
	void							SetMarriagePartnerRace(BYTE nPartnerRace)			{ m_nMarriagePartnerRace = nPartnerRace; }
	BYTE							GetMarriagePartnerJobFirst()						{ return m_nMarriagePartnerJobFirst; }
	void							SetMarriagePartnerJobFirst(BYTE nPartnerJobFirst)	{ m_nMarriagePartnerJobFirst = nPartnerJobFirst; }
	BYTE							GetMarriagePartnerJobCur()							{ return m_nMarriagePartnerJobCur; }
	void							SetMarriagePartnerJobCur(BYTE nPartnerJobCur)		{ m_nMarriagePartnerJobCur = nPartnerJobCur; }
	BYTE							GetMarriagePartnerJobGrade()						{ return m_nMarriagePartnerJobGrade; }
	void							SetMarriagePartnerJobGrade(BYTE nPartnerJobGrade)	{ m_nMarriagePartnerJobGrade = nPartnerJobGrade; }
	WORD							GetMarriagePartnerLv()								{ return m_nMarriagePartnerLv; }
	void							SetMarriagePartnerLv(BYTE nPartnerLv)				{ m_nMarriagePartnerLv = nPartnerLv; }

	char*							GetMarriagePartnerName()						{ return m_HeroCharacterInfo.szMarriagePartnerName; }
	void							SetMarriagePartnerName(char* pszName)			{ SafeStrCpy(m_HeroCharacterInfo.szMarriagePartnerName,		pszName,	 MAX_NAME_LENGTH+1); }
	char*							GetMarriagePartnerNickName()					{ return m_HeroCharacterInfo.szMarriagePartnerNickName; }
	void							SetMarriagePartnerNickName(char* pszNickName)	{ SafeStrCpy(m_HeroCharacterInfo.szMarriagePartnerNickName, pszNickName, MAX_NAME_LENGTH+1); }
	// E 결혼 추가 added by hseos 2007.12.10	2007.12.12

	// 070813 웅주, 인벤토리 위치 체크
	BOOL IsInventoryPosition( POSTYPE );

	// 080507 KTH --
	void							SetFarmAnimalFeedRetryTime(UINT nTime)	{ m_stFarmInfo.nAnimalFeedRetryTime = nTime; m_stFarmInfo.nAnimalFeedRetryTimeTick = gCurTime; }
	void							SetFarmAnimalCleanRetryTime(UINT nTime)	{ m_stFarmInfo.nAnimalCleanRetryTime = nTime; m_stFarmInfo.nAnimalCleanRetryTimeTick = gCurTime; }
	
	// 070705 세트 아이템 능력 관리
public:
	// 071207 LUJ 세트 정보 초기화
	void	ResetSetItemStatus();

	typedef std::map< const SetScript*, int >	SetItemLevel;

	const SetItemLevel&	GetSetItemLevel() const;
	SetItemLevel&		GetSetItemLevel();
	
private:
	SetItemLevel			mSetItemLevel;


	// 070719 세트 스킬 관리
	// 웅주, 070719 세트 패시브 스킬 적용
public:
	void				AddSetSkill		( DWORD skillIndex, BYTE level );
	void				RemoveSetSkill	( DWORD skillIndex );
	SKILL_BASE*			GetStoredSkill	( DWORD index );
	void				SetStoredSkill	( const SKILL_BASE& );

	BOOL				IsSetSkill		( DWORD index );

private:
	// 세트로 인해 부여된 능력이 제거될 때는, 플레이어가 정적으로 부여한 능력이 복구하기 위해 필요한 정보
	// 키: 스킬 인덱스
	typedef std::map< DWORD, SKILL_BASE >	StoredSkill;
	StoredSkill								mStoredSkill;

	// 세트 아이템은 스킬을 부여하거나 제거한다. 이때 레벨이 다른 동일 스킬(예: 파이어볼1, 파이어볼2)이 있을 경우 복구할 방법이 없다.
	// 이에 스택에 저장하여 세트 스킬을 제거할 때마다 체크하여 역순으로 복구한다.
	typedef std::stack< BYTE >	SetSkill;
	typedef std::map< DWORD, SetSkill >	StoredSetSkill;
	StoredSetSkill						mStoredSetSkill;

public:
	
	// 071005 웅주, 쿨타임
public:
	// 쿨타임 갱신에 성공한다는 것은 현재 쿨타임에 영향받는 아이템이 없다는 뜻
	BOOL AddCoolTime( DWORD coolTimeGroupIndex, DWORD coolTime );
	
private:
	// 유효 시간이 경과한 아이템을 제거한다.
	void ProcCoolTime();

	struct CoolTime
	{
		DWORD mBeginTick;
		DWORD mEndTick;
	};

	// 키워드: 쿨타임 그룹 인덱스
	typedef std::map< DWORD, CoolTime > CoolTimeMap;
	// 080617 LUJ, CPlayer가 풀에 할당되고 해제되는 과정에서 메모리 침범이 발생하는 것으로 추측. 이에 따라 포인터로 변경
	CoolTimeMap							*mCoolTimeMap;

	virtual	void SetObjectBattleState( BYTE state );


	// 071122 KTH --- delete
	//CYHHashTable<ITEMBASE> m_htTimeCheckItem ;
	DWORD m_dwLastTimeCheckItemDBUpdate;
	DWORD m_dwLastTimeCheckItem;
public:
	// 071220 KTH -- 케릭터가 기억하고 있는 장소
	//std::map<BYTE, st_MemoryStoneInfo>	m_MemoryStoneInfoMap;
	
	//void AddTimeCheckItem( ITEMBASE* pItemBase );

	void ProcessTimeCheckItem( BOOL bForceDBUpdate );

	// 071128 LYW --- Player : HPMP 적용.
	void SetHP_Point( BYTE hpPoint ) { m_byHP_Point = hpPoint ; }
	BYTE GetHP_Point() { return m_byHP_Point ; }
	void SetMP_Point( BYTE mpPoint ) { m_byMP_Point = mpPoint ; }
	BYTE GetMP_Point() { return m_byMP_Point ; }

	
	// 071206 KTH -- 스킬과 스텟이 초기화 중인지 검사.
	BOOL	IsResetSkill()	{	return m_bResetSkill;	}
	void	SetResetSkill( BOOL bResetSkill )	{	m_bResetSkill = bResetSkill;	}
	BOOL	IsResetStat()	{	return m_bResetStat;	}
	void	SetResetStat( BOOL bResetStat )	{	m_bResetStat = bResetStat;	}

private:
	BOOL	m_bResetSkill;
	BOOL	m_bResetStat;

protected:
	DWORD	m_dwReturnSkillMoney;	//돌려줄 돈 임시 보관 장소
public:
	void	SetRetrunSkillMoney( DWORD ReturnSkillMoney )	{	m_dwReturnSkillMoney = ReturnSkillMoney;	}
	DWORD	GetReturnSkillMoney()							{	return m_dwReturnSkillMoney;	}

	// 071210 KTH -- 확장인벤토리 기능
	WORD	GetInventoryExpansion()							{ return m_HeroCharacterInfo.wInventoryExpansion; }
	// 071225 KTH -- (Fix) 확장 인벤토리의 갯수가 변경될 경우 확장된 수만큼 슬롯의 갯수를 다시 셋팅하여 준다.
	void	SetInventoryExpansion(WORD InventoryExpansion)	{ 
																m_HeroCharacterInfo.wInventoryExpansion = InventoryExpansion; 
																m_InventorySlot.SetSlotNum(SLOT_INVENTORY_NUM + GetInventoryExpansionSize());
															}
	BOOL	IncreaseInventoryExpansion();
	// 071213 KTH -- 확장인벤토리의 크기 가져오기
	WORD	GetInventoryExpansionSize()						{	return (WORD)m_HeroCharacterInfo.wInventoryExpansion * TABCELL_INVENTORY_NUM;	}

	// 071226 KTH -- 이름 변경.
	VOID	SetCharacterName(char* name)					{	strcpy(m_BaseObjectInfo.ObjectName, name);	}

	// 071208 LYW --- Player : 캐릭터 스탯 초기화시, 스탯 변경 후 다른 업데이트 구문 없이 사용하기 위해 함수 추가.
	HERO_TOTALINFO* Get_HeroTotalInfo() { return &m_HeroInfo ; }

	void	DecreaseCount( BYTE kind );
	
	void	PassiveSkillCheckForWeareItem();


//---KES AUTONOTE
protected:
	DWORD m_dwAutoNoteIdx;
	DWORD m_dwLastActionTime;
public:
	DWORD GetAutoNoteIdx() { return m_dwAutoNoteIdx; }
	void SetAutoNoteIdx( DWORD AutoNoteIdx ) { m_dwAutoNoteIdx = AutoNoteIdx; }
//---------------
//---KES CHEAT CHANGESIZE
	void SetCharacterSize( float fSize ) { m_HeroCharacterInfo.Width = m_HeroCharacterInfo.Height = fSize; }
//--------------------------
	DWORD GetLastActionTime() { return m_dwLastActionTime; }
	void SetLastActionTime( DWORD dwLastActionTime ) { m_dwLastActionTime = dwLastActionTime; }

	void SetWeaponEnchant( BYTE level )
	{
		m_HeroCharacterInfo.WeaponEnchant = level;
	}

// 080411 NYJ - 낚시
protected:
	DWORD m_dwFishingPlaceIdx;				// 낚시터NPC Instance Index
	DWORD m_dwFishingStartTime;				// 시작시간
	DWORD m_dwFishingProcessTime;			// 진행시간
	DWORD m_dwFishingBaitIdx;				// 사용한 미끼 Item index

	float m_fFishItemRate[eFishItem_Max];	// 장비에 따른 낚시확률 추가변수
	float m_fFishBaitRate[eFishItem_Max];	// 미끼에 따른 낚시확률 추가변수


	DWORD m_dwFM_MissionCode;				// 진행중인 미션
	DWORD m_dwFM_StartTime;					// 미션 시작 시간

	// 낚시관련데이타
	WORD  m_wFishingLevel;					// 낚시숙련등급
	// 080602 LYW --- Player : 경험치 수치 (__int32) 에서 (__int64) 사이즈로 변경 처리.
	//DWORD m_dwFishingExp;					// 낚시숙련도
	EXPTYPE m_dwFishingExp;					// 낚시숙련도

	DWORD m_dwFishPoint;					// 물고기포인트
	
public:
	std::list<DWORD> m_lstGetFishList;		// 미션중 최근 낚은 3마리 고기, FM(FishingMission)

	void FishingInfoClear();				// 낚시관련정보 한방에 클리어.

	void SetFishingPlace(DWORD dwIndex)						{m_dwFishingPlaceIdx = dwIndex;}
	DWORD GetFishingPlace()									{return m_dwFishingPlaceIdx;}

	void SetFishingStartTime(DWORD dwTime)					{m_dwFishingStartTime = dwTime;}
	DWORD GetFishingStartTime()								{return m_dwFishingStartTime;}

	void SetFishingProcessTime(DWORD dwTime)				{m_dwFishingProcessTime = dwTime;}
	DWORD GetFishingProcessTime()							{return m_dwFishingProcessTime;}

	void SetFishingBait(DWORD dwIndex)						{m_dwFishingBaitIdx = dwIndex;}
	DWORD GetFishingBait()									{return m_dwFishingBaitIdx;}

	float* GetFishItemRate()								{return m_fFishItemRate;}
	float GetFishItemRate_Grade(int nGrade)					{if(0<=nGrade && nGrade<eFishItem_Max)	return m_fFishItemRate[nGrade];	return 0.0f;}

	float* GetFishBaitRate()								{return m_fFishBaitRate;}
	float GetFishBaitRate_Grade(int nGrade)					{if(0<=nGrade && nGrade<eFishItem_Max)	return m_fFishBaitRate[nGrade];	return 0.0f;}


	void SetFM_StartTime(DWORD dwStartTime)					{m_dwFM_StartTime = dwStartTime;}
	DWORD GetFM_StartTime()									{return m_dwFM_StartTime;}

	// 낚시미션관련
	void SetFM_Mission(DWORD dwCode)						{m_dwFM_MissionCode = dwCode;}
	DWORD GetFM_Mission()									{return m_dwFM_MissionCode;}

	// 낚시관련데이타
	void SetFishingLevel(WORD wLevel)						{m_wFishingLevel = wLevel;}
	WORD GetFishingLevel()									{return m_wFishingLevel;}
	// 080602 LYW --- Player : 경험치 수치 (__int32) 에서 (__int64) 사이즈로 변경 처리.
	//void SetFishingExp(DWORD dwExp);
	void SetFishingExp(EXPTYPE dwExp);
	// 080602 LYW --- Player : 경험치 수치 (__int32) 에서 (__int64) 사이즈로 변경 처리.
	//DWORD GetFishingExp()									{return m_dwFishingExp;}
	EXPTYPE GetFishingExp()									{return m_dwFishingExp;}
	void SetFishPoint(DWORD dwPoint)						{m_dwFishPoint = dwPoint; if(MAX_FISHPOINT<m_dwFishPoint) m_dwFishPoint=MAX_FISHPOINT;}
	DWORD GetFishPoint()									{return m_dwFishPoint;}

	bool	tempHideParts[eWearedItem_Max];					// 파츠숨기기 여부.  m_HeroCharacterInfo의 HideParts에 넣어서 유저에게 보내야 한다.
	const bool*	GetHideParts() const						{return m_HeroCharacterInfo.HideParts;}
	void	SetHideParts(bool* pHideParts);

	// 080616 LUJ, 생성한 함정 관리
public:
	inline BOOL IsHaveTrap( DWORD monsterIndex ) const	{ return mTrapSet->end() != mTrapSet->find( monsterIndex ); }
	inline void AddTrap( DWORD monsterIndex )			{ mTrapSet->insert( monsterIndex ); }
	inline void RemoveTrap( DWORD monsterIndex )		{ mTrapSet->erase( monsterIndex ); }

private:
	// 080616 LUJ, 한 종류의 함정은 하나만 생성 가능하도록, 보유한 함정의 MonsterKind를 저장한다
	typedef std::set< DWORD >	TrapSet;
	TrapSet*					mTrapSet;

	
protected:	
	CYHHashTable<PET_OBJECT_INFO>	mPetInfoTable;
	CPet*							mpCurPet;
	DWORD							mSummonWaitPet;

public:
	void							AddPetInfo( PET_OBJECT_INFO* pInfo )	{ mPetInfoTable.Add( pInfo, pInfo->ItemDBIdx ); }
	PET_OBJECT_INFO*				GetPetInfo( DWORD ItemDBIdx )			{ return mPetInfoTable.GetData( ItemDBIdx ); }
	void							SetCurPet( CPet* pPet )					{ mpCurPet = pPet; }
	CPet*							GetCurPet()								{ return mpCurPet; }
	void							SetSummonWaitPet( DWORD ItemDBIdx )		{ mSummonWaitPet = ItemDBIdx; }
	DWORD							GetSummonWaitPet()						{ return mSummonWaitPet; }
};

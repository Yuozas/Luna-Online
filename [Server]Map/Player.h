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
/*�����Ƽ ���� - #include "AbilityGroup.h"*/
#include "ShopItemManager.h"

// jsd quest
#include "..\[CC]Quest\QuestDefines.h"
#include "QuestGroup.h"
#include "../[cc]skill/server/tree/skilltree.h"
#include "QuickSlot.h"
// desc_hseos_�ֹε��01
// S �ֹε�� �߰� added by hseos 2007.06.04
#include "../hseos/ResidentRegist/SHResidentRegistManager.h"
// E �ֹε�� �߰� added by hseos 2007.06.04

// 071115 LYW --- Player : ���� ������ ���� �߰�.
//#include "ChargeItemManager.h"


// desc_hseos_����ý���_01
// S ����ý��� �߰� added by hseos 2008.01.22
struct stFARMPLAYERINFO																		// �÷��̾� ����
{
	UINT				nCropPlantRetryTime;												// ..�ٽ� ���۹��� �ɴµ� �ʿ��� �ð�
	DWORD				nCropPlantRetryTimeTick;											// ..�ٽ� ���۹��� �ɴµ� �ʿ��� �ð� ƽ
	UINT				nCropManureRetryTime;												// ..�ٽ� ���۹��� ��Ḧ �ִµ� �ʿ��� �ð�
	DWORD				nCropManureRetryTimeTick;											// ..�ٽ� ���۹��� ��Ḧ �ִµ� �ʿ��� �ð� ƽ
	UINT				nAnimalFeedRetryTime;												// ..�ٽ� ���࿡ ��Ḧ �ִµ� �ʿ��� �ð�
	DWORD				nAnimalFeedRetryTimeTick;											// ..�ٽ� ���࿡ ��Ḧ �ִµ� �ʿ��� �ð� ƽ
	UINT				nAnimalCleanRetryTime;												// ..�ٽ� ������ �ı�´� �ʿ��� �ð�
	DWORD				nAnimalCleanRetryTimeTick;											// ..�ٽ� ������ �ı�´� �ʿ��� �ð� ƽ
};
// E ����ý��� �߰� added by hseos 2008.01.22

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
	// desc_hseos_ç���� ��_01
	// S ç���� �� �߰� added by hseos 2007.12.12
	PLAYERINITSTATE_CHALLENGEZONE_INFO	= 1 << 13,		// ç���� ��
	// E ç���� �� �߰� added by hseos 2007.12.12

	// desc_hseos_��ȥ_01
	// S ��ȥ �߰� added by hseos 2007.12.12
	PLAYERINITSTATE_MARRIAGE_INFO		= 1 << 14,		// ��ȥ
	// E ��ȥ �߰� added by hseos 2007.12.12

	// 080917 NYJ - �ɼǴܰ��߰�
	PLAYERINITSTATE_OPTION_INFO			= 1 << 15,
};

enum
{
	MONEY_ADDITION = 0,
	MONEY_SUBTRACTION,
};

//SW051112 ���ָ��
#define MUSSANG_VALID_TIME		1000*60*10
enum eMussangStates{ eMSS_None=1, eMSS_Ready=2, eMSS_InMode=4, eMSS_LevelUp=8};

#define GETITEM_FLAG_INVENTORY	0x00000001
#define GETITEM_FLAG_WEAR		0x00000002
#define GETITEM_FLAG_PETWEAR	0x00000004
#define WANTED_DESTROYED_TIME 48*3600*1000
class CPlayer : public CObject  
{
//////////////////////////////////////////////////////////////////////////////////////////
/// ���� �����

	///--------------------------------------------------------------------/
	/// �ɸ��� �⺻&���� ���� ���� ����------------------------------------/
private:
	/// ���� ���� ����
	/// Agent���� ����
	DWORD				m_UniqueIDinAgent;
	/// ä�� ����
	DWORD				m_dwCurChannel;		// ���� ������ �����ϴ� ä��
	DWORD				m_ChannelID;

	/// ��������
	DWORD				m_dwUserLevel;

	/// ���ӽð�
	DWORD				m_LoginTime;
	DWORD				m_ContinuePlayTime;	
	
	/// �ʱ�ȭ ����
	int					m_initState;

	/// �ɸ��� ������
	CHARACTER_TOTALINFO m_HeroCharacterInfo;
	HERO_TOTALINFO		m_HeroInfo;

	/// ���� �ɼ� ����
	sGAMEOPTION			m_GameOption;

	/// ������
	DWORD				m_dwOldMapNum;

	/// ����â
	cQuickSlot			m_QuickSlot[8];

	BOOL				m_God;
	/// �ɸ��� �⺻&���� ���� ���� ����------------------------------------/
	///--------------------------------------------------------------------/

	
	///--------------------------------------------------------------------/
	/// �������ͽ�&���� ��ġ ���� ���� ����--------------------------------/
private:
	/// �⺻ �ɸ��� ���ݰ� �����ۿ� ���� ����
	PlayerStat	m_charStats;
	PlayerStat	m_itemStats;

	// 080313 LUJ, ��Ʈ ������ ���� �и�
	PlayerStat	m_SetItemStats;

	Status				mPassiveStatus;
	Status				mRatePassiveStatus;

	friend class		CCharacterCalcManager;

	/// �������ͽ�&���� ��ġ ���� ���� ����--------------------------------/
	///--------------------------------------------------------------------/


	///--------------------------------------------------------------------/
	/// ����/�ൿ ���� ���� ����-------------------------------------------/
private:
	/// �̸�Ƽ�� ��� �ð�
	DWORD		m_dwLastSocietyAction;
	
	/// ���� ���� ����
	BOOL		m_bShowdown;
	DWORD		m_dwOpPlayerID;

	/// PK��� �ð� ����
	DWORD		m_dwPKModeStartTime;	//with gCurTime
	DWORD		m_dwPKContinueTime;

	//---KES PK 071202
	BOOL	m_bNoExpPenaltyByPK;

	// 080318 LUJ, PK�� ���� ����ġ ��ս� �������� ��ȯ
public:
	BOOL	IsNoExpPenaltyByPK() const	{ return m_bNoExpPenaltyByPK; }

private:
	/// �Ǹ�ġ ����
	DWORD		m_dwLastReduceHour;		//with PlayTime
	void		AddBadFameReduceTime();

	/// ����� ����
	WANTEDTYPE	m_WantedIDX;
	cPtrList	m_WantedList;

	/// �ڽ��� ���� ��� ����
	WORD		m_MurdererKind;
	DWORD		m_MurdererIDX;
	WORD		m_MurderObjKind;

	/// ��������
	DWORD		m_dwMussangPoint;
	DWORD		m_dwMussangStartTime;
	BOOL		m_bIsMussangReady;
	BYTE		m_bMussangFlag;
	DWORD		m_dwMussangTime;
	WORD		m_wMussangStat;
	
	/// ����/��Ȱ ���� ����
	BOOL		m_bPenaltyByDie;
	BOOL		m_bReadyToRevive; //���� ���¿����� ��
	REVIVEDATA	m_ReviveData;
	int			m_ReviveTime;
	DWORD		m_InitPosition;
	BOOL		m_bSavePointInited;

	/// �ڽſ��� ���� ����
	CYHHashTable<CMonster> m_FollowMonsterList;
	
	/// ���� �ɷ� ��ġ
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
	// 080910 LUJ, ���� ���� ��ġ. ��� ��꿡 ���
	DWORD		mShieldDefense;

	/// ������ ����
protected:
	BOOL		m_bDieForGFW;		// ������������� �׾�����...
	BOOL		m_bDieInSpecialMap;
	WORD		m_wReturnMapNum;
	WORD		m_wGTReturnMapNum;
	int			m_ObserverBattleIdx;

	/// ����Ʈ ����
	CQuestGroup	m_QuestGroup;
public:
	BOOL		m_bNeedRevive;

	cDelay*		mpBattleStateDelay;
	/// ����/�ൿ ���� ���� ����-------------------------------------------/
	///--------------------------------------------------------------------/


	///--------------------------------------------------------------------/
	/// ���μ��� ���� ����-------------------------------------------------/
private:
	/// ���̵� ����
	WORD			m_ChangeBeforeMapNum;
	WORD			m_ChangeWantMapNum;

	/// �α� ���μ��� �ð�
	DWORD			m_dwProgressTime;

	/// ���ǵ��� üũ
	int				m_nHackCount;			//skill + move
	DWORD			m_dwHackStartTime;

	/// ȸ�����μ��� ���� ����
	RECOVER_TIME	m_ManaRecoverTime;
	YYRECOVER_TIME	m_YYManaRecoverTime;

	/// ���� ����
	BOOL			m_bExit;	//�����÷���
	DWORD			m_dwExitStartTime;
	BOOL			m_bNormalExit;
	BOOL			m_bWaitExitPlayer;

	BOOL			mIsEmergency;
	MAPTYPE			mTargetMap;
	DWORD			mTargetPosX;
	DWORD			mTargetPosZ;

	/// ���μ��� ���� ����-------------------------------------------------/
	///--------------------------------------------------------------------/

	///--------------------------------------------------------------------/
	/// ������ ���� ���� ����----------------------------------------------/
	/// ���������� ���� �ɸ��� ��ġ ������ ��ġ ���� ���� ������ �з�
private:
	/// ���� �κ��丮
	CItemContainer						m_ItemContainer;
	CInventorySlot						m_InventorySlot;
	CWearSlot							m_WearSlot;
	CStorageSlot						m_StorageSlot;
	CShopItemSlot						m_ShopItemSlot;			// ItemMall��	
	CShopInvenSlot						m_ShopInvenSlot;
	CInventoryPurse						m_InventoryPurse;
	CStoragePurse						m_StoragePurse;
	/// �κ��丮 ���� Ŭ����
	friend class						CInventoryPurse;
	
	/// �ʱ�ȭ ���� ����
	BOOL								m_bGotWarehouseItems;
	BOOL								m_bStorageInit;
	BOOL								m_bShopInit;
	BOOL								m_bPetInvenInit;

	/// ��ȯ ����
	sEXCHANGECONTAINER					m_ExchangeContainer;

	/// ������ ����
	cStreetStall*						m_pGuetStall;
	char								m_StreetStallTitle[MAX_STREETSTALL_TITLELEN+1];

	/// ĳ�� ������
//	CShopItemManager					m_ShopItemManager;

	// 071115 LYW --- Player : ���� ������ �Ŵ��� ���� �߰�.
	//CChargeItemManager					m_ChargeItemManager ;
		
	/// ������ ���� ���� ����----------------------------------------------/
	///--------------------------------------------------------------------/


	///--------------------------------------------------------------------/
	/// ��ų ���� ���� ����------------------------------------------------/
	/// ��ų�� ���� �ɸ��� ��ġ ������ ��ġ ���� ���� ������ �з�
	/// ������ų ����
	cSkillTree					m_SkillTree;

	/// �Ϲ� ���� �޺�
	DWORD						m_CurComboNum;

	/// ��ų ���� Ƚ��
	WORD						m_SkillFailCount;

	CYHHashTable<EXCEPTION>		mExceptionTable;

	BOOL						mbBuffSkillLoad;
	BOOL						mbPetLoad;

	/// ��ų ���� ���� ����------------------------------------------------/
	///--------------------------------------------------------------------/

	DATE_MATCHING_INFO			m_DateMatching;						// date matching	2007/03/29

	// desc_hseos_���͹���01
	// S ���͹��� �߰� added by hseos 2007.05.23
	stMONSTERMETER							m_stMonstermeterInfo;
	// E ���͹��� �߰� added by hseos 2007.05.23

	// desc_hseos_�ֹε��01
	// S �ֹε�� �߰� added by hseos 2007.06.04
	CSHResidentRegistManager::stRegistInfo	m_stRegistInfo;
	// E �ֹε�� �߰� added by hseos 2007.06.04

	// desc_hseos_�йи�01
	// S �йи� �߰� added by hseos 2007.07.06	2007.07.17
	char*									m_pcFamilyEmblem;
	// E �йи� �߰� added by hseos 2007.07.06	2007.07.17

	// desc_hseos_����ý���_01
	// S ����ý��� �߰� added by hseos 2007.08.23	2008.01.22
	stFARMPLAYERINFO						m_stFarmInfo;
	// E ����ý��� �߰� added by hseos 2007.08.23	2008.01.22

//---KES Aggro 070918
	CYHHashTable<CMonster>	m_htAggroedTable;	//���� AggroList�� ������ �ִ� ���� ���

	// desc_hseos_����Ʈ ��_01
	// S ����Ʈ �� �߰� added by hseos 2007.11.27	2007.11.28	2007.11.30	2007.12.05
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
	// E ����Ʈ �� �߰� added by hseos 2007.11.27	2007.11.28	2007.11.30	2007.12.05
	
//-------------------
	// 071128 LYW --- Player : HPMP ����.
	BYTE m_byHP_Point ;
	BYTE m_byMP_Point ;

	// desc_hseos_��ȥ_01
	// S ��ȥ �߰� added by hseos 2007.12.10	2008.01.11
	DWORD									m_nMarriageRequestPartnerID;
	WORD									m_nMarriageTeleportFreq;
	char									m_szMarriageDivorceDate[16+1];
	DWORD									m_nMarriagePartnerID;
	BYTE									m_nMarriagePartnerRace;
	BYTE									m_nMarriagePartnerJobFirst;
	BYTE									m_nMarriagePartnerJobCur;
	BYTE									m_nMarriagePartnerJobGrade;
	WORD									m_nMarriagePartnerLv;
	// E ��ȥ �߰� added by hseos 2007.12.10	2008.01.11

/// ���� �����
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� �����
public:
	/// ������/�Ҹ���
	CPlayer();
	virtual ~CPlayer();

	///--------------------------------------------------------------------/
	/// �ɸ��� �⺻ ���� ���� �Լ�-----------------------------------------/
private:
public:
	/// �ʱ�ȭ
	virtual BOOL	Init( EObjectKind kind, DWORD AgentNum, BASEOBJECT_INFO* pBaseObjectInfo );
	virtual void	Release();
	void			InitBaseObjectInfo( BASEOBJECT_INFO* pBaseInfo );
	void			InitClearData();
	void			InitCharacterTotalInfo( CHARACTER_TOTALINFO* pTotalInfo );
	void			InitHeroTotalInfo( HERO_TOTALINFO* pHeroInfo );
	void			DoSetInited();
	virtual void	SetInitedGrid();

	/// ���� ������
	/// Agent���� ����
	void			SetUniqueIDinAgent( DWORD uniqueid ) { m_UniqueIDinAgent = uniqueid; }
	DWORD			GetUniqueIDinAgent() { return m_UniqueIDinAgent; }

	/// ä�� ����
	void			SetChannelID( DWORD id ) { m_ChannelID = id; }
	DWORD			GetChannelID() { return m_ChannelID; }

	void			SetCurChannel(DWORD dwChannel) {m_dwCurChannel = dwChannel;}
	DWORD			GetCurChannel() {return m_dwCurChannel;}

	/// ���� ����
	void			SetUserLevel( DWORD dwUserLevel ) { m_dwUserLevel = dwUserLevel; }
	DWORD			GetUserLevel() { return m_dwUserLevel; }

	/// �÷��̽ð�
	DWORD			GetPlayTime() {	return m_HeroInfo.Playtime;	}
	void			SetLoginTime( int time ) { m_LoginTime = time; }

	/// Grid �ý��� ����
	void			SetAddMsg( char* pAddMsg, WORD* pMsgLen, DWORD dwReceiverID, BOOL bLogin );
	virtual void	SendMsg( MSGBASE* pMsg, int MsgLen );

	/// �⺻ ���� ȹ��
	void			GetCharacterTotalInfo( CHARACTER_TOTALINFO* pRtInfo );

	// 080225 LUJ, �⺻ ���� ��ȯ
	const CHARACTER_TOTALINFO&	GetCharacterTotalInfo() const;

	void			GetHeroTotalInfo( HERO_TOTALINFO* pRtInfo );
	HERO_TOTALINFO*	GetHeroTotalInfo();
	int				GetGender() { return m_HeroCharacterInfo.Gender; }

	/// ���� �ɼ�
	sGAMEOPTION*	GetGameOption() { return &m_GameOption; }
	void			SetGameOption( sGAMEOPTION* pGameOption ) { m_GameOption = *pGameOption; }

	/// ����
	int				GetInitState(){ return m_initState; }
	void			SetInitState( int initstate, DWORD protocol );

	void			OnStartObjectState( BYTE State, DWORD dwParam );
	void			OnEndObjectState( BYTE State );

	void			SetPeaceMode( BOOL bPeace ) { m_HeroCharacterInfo.bPeace = bPeace?true:false; }
	BOOL			GetPeaceMode() { return m_HeroCharacterInfo.bPeace; }
	BOOL			IsVisible()	{ return (m_HeroCharacterInfo.bVisible ? TRUE : FALSE);	}
	void			SetVisible( BOOL val ) { m_HeroCharacterInfo.bVisible = val ? true : false; }

	/// ���� 
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

	/// �� ����
	WORD			GetPlayerLoginPoint() { return m_HeroCharacterInfo.LoginPoint_Index; }
	void			SetLoginPoint(WORD LoginPoint) { m_HeroCharacterInfo.LoginPoint_Index = LoginPoint; }
	void			InitMapChangePoint() { m_HeroCharacterInfo.MapChangePoint_Index = 0; }
	void			SetMapChangePoint(WORD point) { m_HeroCharacterInfo.MapChangePoint_Index = point; }	// YH
	WORD			GetMapChangePoint() { return m_HeroCharacterInfo.MapChangePoint_Index;	} //hs for party
	// 080104 LYW --- Player : ����Ʈ�� �������� ���̵��ϴ� �Լ� ����.
	void			SendPlayerToMap(MAPTYPE mapNum, float xpos, float zpos) ;
	//void			SendPlayerToMap();	
	
	void			SetInitPoint( DWORD Pos ) {	m_InitPosition = Pos; }
	DWORD			GetInitPoint() { return m_InitPosition; }
	BOOL			GetSavePointInit() { return m_bSavePointInited; }
	void			SetSavePointInit( BOOL val ) { m_bSavePointInited = val; }
	// 070627 LYW --- Player : Add function to return map number.
	MAPTYPE			GetCurMapNum() { return m_HeroCharacterInfo.CurMapNum ; }
	

	/// �ɸ��� ��ȯ
	void			SetCharChangeInfo( CHARACTERCHANGE_INFO* pInfo );	

	BOOL			GetKyungGongIdx() { return m_MoveInfo.KyungGongIdx; }

	BOOL			AddQuick( BYTE sheet, WORD pos, SLOT_INFO* pSlot );
	SLOT_INFO*		GetQuick( BYTE sheet, WORD pos );

	void			SetGod( BOOL val ) { m_God = val; }
	BOOL			IsGod() { return m_God; }
	/// �ɸ��� �⺻ ���� ���� �Լ�-----------------------------------------/
	///--------------------------------------------------------------------/

	
	///--------------------------------------------------------------------/
	/// �������ͽ�&���� ��ġ ���� ���� �Լ�--------------------------------/
private:
public:
	/// �������� ��������Ʈ
	LEVELTYPE			GetPlayerLevelUpPoint() { return m_HeroInfo.LevelUpPoint;	}
	void				SetPlayerLevelUpPoint( LEVELTYPE val );

	// 080611 LYW --- Player : ��ų����Ʈ ������Ʈ ó���� ������.
	// (���������� ��ų ����Ʈ�� �߰��ϴ� ����� ����� ����.)
	//void SetSkillPoint( DWORD point ) ;
	void SetSkillPoint( DWORD point, BYTE byForced ) ;

	DWORD				GetSkillPoint();
	/// ���� ����
	LEVELTYPE			GetMaxLevel() { return m_HeroInfo.MaxLevel; }
	virtual				LEVELTYPE GetLevel();
	virtual void		SetLevel( LEVELTYPE level );

    /// ����ġ
	EXPTYPE				GetPlayerExpPoint() { return m_HeroInfo.ExpPoint; }
	DOUBLE				GetPlayerTotalExpPoint();
	void				SetPlayerExpPoint( EXPTYPE point, BYTE bDie=0 );
	// 070411 LYW --- Player : Add function to set experience.
	//void				SetPlayerExpPointDown(EXPTYPE point,BYTE bDie=0) ;
	void				AddPlayerExpPoint( EXPTYPE Exp );
	void				ReduceExpPoint( EXPTYPE minusExp, BYTE bLogType );

	/// ��/�Ǹ� ����
	FAMETYPE			GetFame() { return m_HeroInfo.Fame; }
	void				SetFame(FAMETYPE val) { m_HeroInfo.Fame = val; }
	FAMETYPE			GetBadFame() { return m_HeroCharacterInfo.BadFame; }
	void				SetBadFame(FAMETYPE val) { m_HeroCharacterInfo.BadFame = val; }

	/// �⺻ ���� ����
	PlayerStat&	GetCharStats() { return m_charStats; }
	PlayerStat&	GetItemStats() { return m_itemStats; }

	// 080313 LUJ, ��Ʈ ���� �и�
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

	/// �����/���� ����
	BYTE				GetLifePercent();
	BYTE				GetManaPercent();
	void				SetMaxLife( DWORD val );
	void				SetMaxMana( DWORD val );

	// 080625 LYW --- Player : ����� ���� �Լ� ����.
	// ĳ���Ͱ� ���� ���¿��� �����̳� ƨ�� ������ �߻� �� ��, 
	// ĳ������ �����/���� 30%�� ������ �־�� �Ѵ�. �׷��� ���°� ���� ���¸�, 
	// ���� �Լ��� return ó���� �ϹǷ�, ������ ���� ���θ� ������ �� �ִ� �Լ��� �߰��Ѵ�.
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

	/// ����/���ݷ�
	virtual DWORD		DoGetPhyDefense();
	virtual float		DoGetAttDefense( WORD Attrib );
	virtual float		GetAttribPlusPercent( WORD Attrib );
	virtual DWORD		DoGetPhyAttackPowerMin();
	virtual DWORD		DoGetPhyAttackPowerMax();

	/// �����Ÿ�
	virtual float		DoGetAddAttackRange() { return GetDexterity() / 3.f; }
	/// ũ��Ƽ�� Ȯ��
	virtual DWORD		DoGetCritical();
	virtual DWORD		DoGetDecisive();

	/// �̵��ӵ� ���
	virtual float		DoGetMoveSpeed();
	WORD				GetKyungGongGrade()	{ return m_HeroInfo.KyungGongGrade; }
	void				SetKyungGongGrade(WORD grade) { m_HeroInfo.KyungGongGrade = grade; }

	/// ����/ȸ��

	/// ���� ����
	void				CalcState();

	/// �������ͽ�&���� ��ġ ���� ���� �Լ�--------------------------------/
	///--------------------------------------------------------------------/
	

	///--------------------------------------------------------------------/
	/// ����/�ൿ ���� �Լ�------------------------------------------------/
	
	/// �ڽ��� ���� ��� ����
private:
	void						SetMurderIDX( DWORD CharacterIDX ) { m_MurdererIDX = CharacterIDX; }
	void						SetMurderKind( WORD Kind ) { m_MurdererKind = Kind; }
public:
	void						SetMurderObjKind( WORD ObjKind ) {	m_MurderObjKind = ObjKind;	}
	WORD						GetMurderObjKind() {	return m_MurderObjKind;	}
	void						ClearMurderIdx();
	DWORD						GetMurderIDX() { return m_MurdererIDX;	}
	WORD						GetMurderKind() { return m_MurdererKind;}

	/// ��Ȱ
	void						SetPenaltyByDie( BOOL bVal ) { m_bPenaltyByDie = bVal; }	//���� �� �� ��뿡 ���� �׾��� �� TRUE //�������� �� �г�Ƽ üũ�� ���� ����
	BOOL						IsPenaltyByDie() { return m_bPenaltyByDie; }
	BOOL						IsReadyToRevive() { return m_bReadyToRevive;	}
	void						SetReadyToRevive( BOOL val ) { m_bReadyToRevive = val;	}
	// 080602 LYW --- Player : ����ġ ��ġ (__int32) ���� (__int64) ������� ���� ó��.
	//DWORD						RevivePenalty(BOOL bAdditionPenalty) ;
	EXPTYPE						RevivePenalty(BOOL bAdditionPenalty) ;
	void						RevivePresentSpot();
	void						ReviveLogIn();
	void						ReviveAfterShowdown( BOOL bSendMsg = TRUE );
	void						ReviveLogInPenelty();
	// 080602 LYW --- Player : ����ġ ��ġ (__int32) ���� (__int64) ������� ���� ó��.
	//DWORD						ReviveBySkill();
	EXPTYPE						ReviveBySkill();

	// 070831 ����, ����� �� ��� �� �������� ��Ȱ ó��
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

	/// �ൿ/�̸�Ƽ��
	BOOL						CanSocietyAction( DWORD curTime );

	/// ��Ƽ ����
	DWORD						GetPartyIdx() { return m_HeroInfo.PartyID; }
	void						SetPartyIdx( DWORD PartyIDx ) 
								{ 
									m_HeroInfo.PartyID = PartyIDx; 
								}
	void						SendPlayerLifeToParty( DWORD life, DWORD PartyIdx );
	void						SendPlayerManaToParty( DWORD mana, DWORD PartyIdx );
	void						SendMoveInfotoParty();

	/// ��� ����
	DWORD						GetGuildIdx() const { return m_HeroCharacterInfo.MunpaID;	}
	void						SetGuildIdx( DWORD GuildIdx ) { m_HeroCharacterInfo.MunpaID = GuildIdx; }
	void						SetGuildInfo( DWORD GuildIdx, BYTE Grade, const char* GuildName, MARKNAMETYPE MarkName );
	BYTE						GetGuildMemberRank() { return m_HeroCharacterInfo.PositionInMunpa; 	}
	void						SetGuildMemberRank( BYTE rank ) { m_HeroCharacterInfo.PositionInMunpa = rank; 	} //���Ŀ����� �ڽ��� ���� ����
	void						SetGuildName( char* GuildName ) { SafeStrCpy( m_HeroCharacterInfo.GuildName, GuildName, MAX_MUNPA_NAME+1 ); }
	void						SetGuildMarkName( MARKNAMETYPE MarkName );
	char*						GetGuildCanEntryDate();
	void						SetGuildCanEntryDate( char* date );
	void						ClrGuildCanEntryDate();
	void						SetNickName( char* NickName );

	/// ���� ���
	DWORD						GetGuildUnionIdx() { return m_HeroCharacterInfo.dwGuildUnionIdx; }
	void						InitGuildUnionInfo( DWORD dwGuildUnionIdx, char* pGuildUnionName, DWORD dwMarkIdx );
	void						SetGuildUnionInfo( DWORD dwGuildUnionIdx, char* pGuildUnionName, DWORD dwMarkIdx );
	void						SetGuildUnionMarkIdx( DWORD dwMarkIdx )	{ m_HeroCharacterInfo.dwGuildUnionMarkIdx = dwMarkIdx; }

	/// ��� ����
	DWORD						GetFamilyIdx() { return m_HeroCharacterInfo.FamilyID;	}
	void						SetFamilyIdx( DWORD FamilyIdx ) { m_HeroCharacterInfo.FamilyID = FamilyIdx; }
	void						SetFamilyInfo( DWORD FamilyIdx, BYTE Grade, char* FamilyName, MARKNAMETYPE MarkName );
	BYTE						GetFamilyMemberRank() { return m_HeroCharacterInfo.PositionInFamily; 	}
	void						SetFamilyMemberRank( BYTE rank ) { m_HeroCharacterInfo.PositionInFamily = rank; 	} //���Ŀ����� �ڽ��� ���� ����
	void						SetFamilyName( char* FamilyName ) { SafeStrCpy( m_HeroCharacterInfo.FamilyName, FamilyName, sizeof( m_HeroCharacterInfo.FamilyName ) ); }
	void						SetFamilyMarkName( MARKNAMETYPE MarkName );
	char*						GetFamilyCanEntryDate();
	void						SetFamilyCanEntryDate( char* date );
	void						ClrFamilyCanEntryDate();
	void						SetFamilyNickName( char* NickName );

	/// ���� ���
	//DWORD						GetFamilyUnionIdx() { return m_HeroCharacterInfo.dwFamilyUnionIdx; }
	//void						InitFamilyUnionInfo( DWORD dwFamilyUnionIdx, char* pFamilyUnionName, DWORD dwMarkIdx );
	//void						SetFamilyUnionInfo( DWORD dwFamilyUnionIdx, char* pFamilyUnionName, DWORD dwMarkIdx );
	//void						SetFamilyUnionMarkIdx( DWORD dwMarkIdx )	{ m_HeroCharacterInfo.dwFamilyUnionMarkIdx = dwMarkIdx; }

	/// �����
	void						ClearWantedList();
	WANTEDTYPE					GetWantedIdx() { return m_WantedIDX; }
	void						SetWantedIdx( WANTEDTYPE val ) { m_WantedIDX = val; }
	DWORD						GetWantedNum() { return m_WantedList.GetCount(); }
	void						AddWanted( WANTEDTYPE WantedIDX );
	void						DeleteWanted( WANTEDTYPE WantedIDX );
	BOOL						IsWantedOwner( WANTEDTYPE WantedIDX );

	/// ����Ʈ
	void						AddQuest( CQuestBase* pQuest );
	void						RemoveQuest( DWORD QuestIdx );
	BOOL						SetQuestState( DWORD QuestIdx, QSTATETYPE value );
	CYHHashTable<CQuestBase>	m_QuestList;
	CQuestGroup*				GetQuestGroup()	{ return &m_QuestGroup; }
	
	/// ����
	BOOL						AddFollowList( CMonster * pMob );
	void						RemoveFollowList( DWORD ID );
	BOOL						RemoveFollowAsFarAs( DWORD GAmount, CObject* pObject );
	WORD						m_wKillMonsterLevel;
	void						SetKillMonsterLevel( WORD wMonLevel ) { m_wKillMonsterLevel = wMonLevel; }
	WORD						GetKillMonsterLevel() { return m_wKillMonsterLevel; }
	virtual	DWORD				GetGravity();
	virtual void				SetGravity(DWORD gravity) {};

//---KES Aggro 070918
	void						AddToAggroed( CMonster* pMonster );		//void CMonster::CalcAggro() �Լ������� ȣ���Ѵ�. �ٸ������� ȣ�� ����!
	void						RemoveFromAggroed( DWORD dwObjectID );	//void CMonster::RemoveFromAggro(), RemoveAllAggro() �Լ������� ȣ���Ѵ�. �ٸ������� ȣ�� ����!
	void						RemoveAllAggroed();

	void						AddAggroToMyMonsters( int nAggroAdd, CObject* pTarget );
//-------------------
	/// ����
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

	/// ��� ��ʸ�Ʈ
	void  						SetReturnMapNum( WORD num )	{ m_wGTReturnMapNum = num; }
	WORD  						GetReturnMapNum() { return m_wGTReturnMapNum; }
	void  						SetObserverBattleIdx( int Idx ) { m_ObserverBattleIdx = Idx; }
	int	 						GetObserverBattleIdx() { return m_ObserverBattleIdx; }
	
	/// ��� �ʵ���
	void						SetDieForGFW( BOOL bDie ) { m_bDieForGFW = bDie; }
	BOOL						IsDieFromGFW() { return m_bDieForGFW; }
	void						SetDieInSpecialMap( BOOL bDie, WORD wReturnMap ) { m_bDieInSpecialMap = bDie; m_wReturnMapNum = wReturnMap; }	

	/// ������
	void  						SetRestraintMode( bool val ) { m_HeroCharacterInfo.bRestraint = val; }
	BOOL  						IsRestraintMode() { return m_HeroCharacterInfo.bRestraint; }
	void  						ReviveVillage();

	/// ���ָ��
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
	void						SendMussangEnd();	//ĳ�� �׾��� �� �Ǵ� ���� �ð� �����
	void						CheatMussangReady();
	WORD						GetMussangStat() { return m_wMussangStat; }

	/// ���� �ɷ� ��ġ
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
	// 080910 LUJ, ������ ������ ��ȯ�Ѵ�
	DWORD						GetShieldDefense();
	// 080910 LUJ, ������ ������ �����Ѵ�
	void						SetShieldDefence( DWORD );

	
	///--------------------------------------------------------------------/
	/// ���μ��� ���� �Լ�-------------------------------------------------/
private:
public:
	/// ���ǵ� �� üũ
	void			SpeedHackCheck();

	/// �⺻ ���� ���μ���
	virtual void	DoDie( CObject* pAttacker );
	virtual void	DoDamage( CObject* pAttacker, RESULTINFO* pDamageInfo, DWORD beforeLife );
	virtual DWORD	Damage( CObject* pAttacker, RESULTINFO* pDamageInfo );

	virtual void	StateProcess();
	
	/// ����Ʈ ���μ���
	void			QuestProcess();

	/// ����
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

	/// ���μ��� ���� �Լ�-------------------------------------------------/
	///--------------------------------------------------------------------/
	
	
	///--------------------------------------------------------------------/
	/// ������ ���� ���� �Լ�----------------------------------------------/
public:
	/// ������ ȹ��
	ITEMOBTAINARRAYINFO*				GetArray( WORD id );
	void								AddArray( ITEMOBTAINARRAYINFO * pInfo );
	void								RemoveArray( DWORD id );
	CYHHashTable<ITEMOBTAINARRAYINFO>	m_ItemArrayList;

	/// ���� �κ��丮
	CItemSlot *							GetSlot( POSTYPE absPos );
	CItemSlot *							GetSlot( eITEMTABLE tableIdx );

	void								AddStorageItem( ITEMBASE * pStorageItem );
	void								SetStorageNum( BYTE n );
	BYTE								GetStorageNum();

	/// �ʱ�ȭ ����
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

	/// ������ ����
	void								GetItemtotalInfo( ITEM_TOTALINFO* pRtInfo, DWORD dwFlag );
				
	void								GetStorageItemInfo( STORAGE_ITEM * pRtInfo );
	
	DWORD								GetWearedItemIdx( DWORD WearedPosition ) { return m_HeroCharacterInfo.WearedItemIdx[WearedPosition]; }
	void								SetWearedItemIdx( DWORD WearedPosition, DWORD ItemIdx );
	DWORD								GetWearedWeapon() { return m_HeroCharacterInfo.WearedItemIdx[eWearedItem_Weapon]; }
	WORD								GetWeaponEquipType();
	// 080703 LUJ, ��ȯ Ÿ���� enum���� ����
	eWeaponAnimationType				GetWeaponAniType();
	
	/// ������ ���
	BOOL								UseItem( WORD abs_pos );
	BOOL								UseConsumptionItem( WORD abs_pos );
	BOOL								UseEquipItem( WORD abs_pos );
	BOOL								UseSkillItem( WORD abs_pos );

    /// �� ����
	MONEYTYPE							SetMoney( MONEYTYPE ChangeValue, BYTE bOper, BYTE MsgFlag = 0, eITEMTABLE tableIdx = eItemTable_Inventory, BYTE LogType = 0, DWORD TargetIdx = 0 );
	MONEYTYPE							GetMoney( eITEMTABLE tableIdx = eItemTable_Inventory );
	MONEYTYPE							GetMaxPurseMoney( eITEMTABLE TableIdx );
	void								SetMaxPurseMoney( eITEMTABLE TableIdx, DWORD MaxMoney );
	BOOL								IsEnoughAdditionMoney( MONEYTYPE money, eITEMTABLE tableIdx = eItemTable_Inventory );
	void								RSetMoney( MONEYTYPE money, BYTE flag );	
private:
	void								MoneyUpdate( MONEYTYPE money ); //(Money Class���� ȣ�� ), DB�� Set
public:

	/// ĳ�� ������
//	SHOPITEMOPTION*						GetShopItemStats() { return &m_ShopItemOption; }
//	AVATARITEMOPTION*					GetAvatarOption() { return &m_AvatarOption; }
//	CShopItemManager*					GetShopItemManager() { return &m_ShopItemManager; }

	// 071115 LYW --- Player : ���� ������ �Ŵ��� ���� �߰�.
	//CChargeItemManager*					GetChargeItemManager() { return &m_ChargeItemManager ; }
	
	/// ��ȯâ
	void								SetExchangeContainer( sEXCHANGECONTAINER* ExchangeContainer ) { m_ExchangeContainer = *ExchangeContainer; }
	sEXCHANGECONTAINER*					GetExchangeContainer() { return &m_ExchangeContainer; }
	
	/// ������
	void								SetGuestStall( cStreetStall* pStall ) { m_pGuetStall = pStall; }
	cStreetStall*						GetGuestStall() { return m_pGuetStall; }
	void								SetStreetStallTitle( char* title );
	void								GetStreetStallTitle( char* title );
	
	/// ������ ���� ���� �Լ�----------------------------------------------/
	///--------------------------------------------------------------------/

	
	///--------------------------------------------------------------------/
	/// ��ų ���� ���� �Լ�------------------------------------------------/

	// 080509 LUJ, ��ų ��Ÿ�� üũ
public:	
	// 080509 LUJ, ��ų ��Ÿ���� ������ �ʾ����� ���� ��ȯ�Ѵ�
	// 080516 LUJ, ���� ī��Ʈ üũ�� �����ϱ� ���� ��� ���� ����
	BOOL IsCoolTime( const ACTIVE_SKILL_INFO& );
	// 080511 LUJ, ��ų ��Ÿ���� �����س��´�
	void SetCoolTime( const ACTIVE_SKILL_INFO& );

private:
	// 080509 LUJ, Ű: ��ų �ε���, ��: ����� ������ ƽī��Ʈ
	typedef stdext::hash_map< DWORD, DWORD >	SkillCoolTimeMap;
	// 080617 LUJ, CPlayer�� Ǯ�� �Ҵ�ǰ� �����Ǵ� �������� �޸� ħ���� �߻��ϴ� ������ ����. �̿� ���� �����ͷ� ����
	SkillCoolTimeMap							*mSkillCoolTimeMap;

	// 080514 LUJ, ��ų �ִϸ��̼� Ÿ�� üũ. �ִϸ��̼� �ð� ���� �ٸ� ��ų�� �ߵ��� �� ������ ä��
	DWORD mSkillAnimationTime;    
	// 080515 LUJ, ��Ÿ�� üũ ���� ������ ����ü
	struct CheckCoolTime
	{
		// 080515 LUJ, ��Ÿ�� üũ�� ������ ���� ƽī��Ʈ
		DWORD mCheckedTick;
		// 080515 LUJ, ��Ÿ�� üũ ���� ȸ��
		DWORD mFailureCount;
	}
	mCheckCoolTime;

public:
	/// ���� ��ų ���� ����
	cSkillTree*					GetSkillTree() { return &m_SkillTree; }
	int							GetSkillLevel( DWORD SkillIdx );

	/// ��ų ����ġ&����
	void						SetPlayerSkillLevel( DWORD SkillIdx, BYTE level, WORD Reason );
	void						SkillLevelDown (DWORD SkillIdx, WORD Reason );

	/// �⺻����
	void						SetCurComboNum(DWORD combonum) { m_CurComboNum = combonum; }
	DWORD						GetCurComboNum() { return m_CurComboNum; }

	/// ��ų ���� üũ
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

	/// ��ų ���� ���� �Լ�------------------------------------------------/
	///--------------------------------------------------------------------/

	// desc_hseos_���͹���01
	// S ���͹��� �߰�		 added by hseos 2007.05.23
	void						SetMonstermeterPlaytime(DWORD nTime, DWORD nTimeTotal)		{ m_stMonstermeterInfo.nPlayTime = nTime; m_stMonstermeterInfo.nPlayTimeTotal = nTimeTotal; }
	void						SetMonstermeterKillMonNum(DWORD nNum, DWORD nNumTotal)		{ m_stMonstermeterInfo.nKillMonsterNum = nNum; m_stMonstermeterInfo.nKillMonsterNumTotal = nNumTotal; }
	void						SetMonstermeterPlaytimeTick(DWORD nTick)					{ m_stMonstermeterInfo.nPlayTimeTick = nTick; }

	stMONSTERMETER*				GetMonstermeterInfo()										{ return &m_stMonstermeterInfo; }

	void						ProcMonstermeterPlayTime();
	void						ProcMonstermeterKillMon();
	// E ���͹��� �߰� added by hseos 2007.05.23

	// desc_hseos_�ֹε��01
	// S �ֹε�� �߰� added by hseos 2007.06.04	2007.06.09
	DATE_MATCHING_INFO*								GetResidentRegistInfo()			{ return &m_DateMatching; }
	CSHResidentRegistManager::stRegistInfo*			GetResidentRegistInfo2()		{ return &m_stRegistInfo; }
	void											SetResidentRegistInfo(DATE_MATCHING_INFO* pInfo);

	void		SetDateMatchingSerchTimeTick(DWORD nTick)			{ m_DateMatching.nSerchTimeTick = nTick; }
	void		SetDateMatchingChatPlayerID(DWORD nChatPlayerID)	{ m_DateMatching.nChatPlayerID = nChatPlayerID; }
	void		SetDateMatchingChatState(DWORD nState)				{ m_DateMatching.nRequestChatState = nState; }
	void		SetDateMatchingChatTimeTick(DWORD nTick)			{ m_DateMatching.nRequestChatTimeTick = nTick; }
	// E �ֹε�� �߰� added by hseos 2007.06.04	2007.06.09

	// desc_hseos_�йи�01
	// S �йи� �߰� added by hseos 2007.07.06	2007.07.17	2007.11.22
	void						SetFamilyEmblem(char* pEmblem)			{ m_pcFamilyEmblem = pEmblem; }
	char*						GetFamilyEmblem()						{ return m_pcFamilyEmblem; }

	void						IncreaseFamilyEmblemChangedFreq()		{ m_HeroCharacterInfo.nFamilyEmblemChangedFreq++; }
	void						SetFamilyEmblemChangedFreq(DWORD nFreq)	{ m_HeroCharacterInfo.nFamilyEmblemChangedFreq = nFreq; }
	DWORD						GetFamilyEmblemChangedFreq()			{ return m_HeroCharacterInfo.nFamilyEmblemChangedFreq; }
	// E �йи� �߰� added by hseos 2007.07.06	2007.07.17	2007.11.22

	// desc_hseos_����ý���_01
	// S ����ý��� �߰� added by hseos 2007.08.23
	stFARMPLAYERINFO*				GetFarmInfo()							{ return &m_stFarmInfo; }
	void							SetFarmCropPlantRetryTime(UINT nTime)	{ m_stFarmInfo.nCropPlantRetryTime = nTime; m_stFarmInfo.nCropPlantRetryTimeTick = gCurTime; }
	void							SetFarmCropManureRetryTime(UINT nTime)	{ m_stFarmInfo.nCropManureRetryTime = nTime; m_stFarmInfo.nCropManureRetryTimeTick = gCurTime; }
	void							ProcFarmTime();
	// E ����ý��� �߰� added by hseos 2007.08.23

	// desc_hseos_����Ʈ ��_01
	// S ����Ʈ �� �߰� added by hseos 2007.11.27	2007.12.05	2008.01.22
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
	// E ����Ʈ �� �߰� added by hseos 2007.11.27	2007.12.05	2008.01.22

	// desc_hseos_��ȥ_01
	// S ��ȥ �߰� added by hseos 2007.12.10	2007.12.12
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
	// E ��ȥ �߰� added by hseos 2007.12.10	2007.12.12

	// 070813 ����, �κ��丮 ��ġ üũ
	BOOL IsInventoryPosition( POSTYPE );

	// 080507 KTH --
	void							SetFarmAnimalFeedRetryTime(UINT nTime)	{ m_stFarmInfo.nAnimalFeedRetryTime = nTime; m_stFarmInfo.nAnimalFeedRetryTimeTick = gCurTime; }
	void							SetFarmAnimalCleanRetryTime(UINT nTime)	{ m_stFarmInfo.nAnimalCleanRetryTime = nTime; m_stFarmInfo.nAnimalCleanRetryTimeTick = gCurTime; }
	
	// 070705 ��Ʈ ������ �ɷ� ����
public:
	// 071207 LUJ ��Ʈ ���� �ʱ�ȭ
	void	ResetSetItemStatus();

	typedef std::map< const SetScript*, int >	SetItemLevel;

	const SetItemLevel&	GetSetItemLevel() const;
	SetItemLevel&		GetSetItemLevel();
	
private:
	SetItemLevel			mSetItemLevel;


	// 070719 ��Ʈ ��ų ����
	// ����, 070719 ��Ʈ �нú� ��ų ����
public:
	void				AddSetSkill		( DWORD skillIndex, BYTE level );
	void				RemoveSetSkill	( DWORD skillIndex );
	SKILL_BASE*			GetStoredSkill	( DWORD index );
	void				SetStoredSkill	( const SKILL_BASE& );

	BOOL				IsSetSkill		( DWORD index );

private:
	// ��Ʈ�� ���� �ο��� �ɷ��� ���ŵ� ����, �÷��̾ �������� �ο��� �ɷ��� �����ϱ� ���� �ʿ��� ����
	// Ű: ��ų �ε���
	typedef std::map< DWORD, SKILL_BASE >	StoredSkill;
	StoredSkill								mStoredSkill;

	// ��Ʈ �������� ��ų�� �ο��ϰų� �����Ѵ�. �̶� ������ �ٸ� ���� ��ų(��: ���̾1, ���̾2)�� ���� ��� ������ ����� ����.
	// �̿� ���ÿ� �����Ͽ� ��Ʈ ��ų�� ������ ������ üũ�Ͽ� �������� �����Ѵ�.
	typedef std::stack< BYTE >	SetSkill;
	typedef std::map< DWORD, SetSkill >	StoredSetSkill;
	StoredSetSkill						mStoredSetSkill;

public:
	
	// 071005 ����, ��Ÿ��
public:
	// ��Ÿ�� ���ſ� �����Ѵٴ� ���� ���� ��Ÿ�ӿ� ����޴� �������� ���ٴ� ��
	BOOL AddCoolTime( DWORD coolTimeGroupIndex, DWORD coolTime );
	
private:
	// ��ȿ �ð��� ����� �������� �����Ѵ�.
	void ProcCoolTime();

	struct CoolTime
	{
		DWORD mBeginTick;
		DWORD mEndTick;
	};

	// Ű����: ��Ÿ�� �׷� �ε���
	typedef std::map< DWORD, CoolTime > CoolTimeMap;
	// 080617 LUJ, CPlayer�� Ǯ�� �Ҵ�ǰ� �����Ǵ� �������� �޸� ħ���� �߻��ϴ� ������ ����. �̿� ���� �����ͷ� ����
	CoolTimeMap							*mCoolTimeMap;

	virtual	void SetObjectBattleState( BYTE state );


	// 071122 KTH --- delete
	//CYHHashTable<ITEMBASE> m_htTimeCheckItem ;
	DWORD m_dwLastTimeCheckItemDBUpdate;
	DWORD m_dwLastTimeCheckItem;
public:
	// 071220 KTH -- �ɸ��Ͱ� ����ϰ� �ִ� ���
	//std::map<BYTE, st_MemoryStoneInfo>	m_MemoryStoneInfoMap;
	
	//void AddTimeCheckItem( ITEMBASE* pItemBase );

	void ProcessTimeCheckItem( BOOL bForceDBUpdate );

	// 071128 LYW --- Player : HPMP ����.
	void SetHP_Point( BYTE hpPoint ) { m_byHP_Point = hpPoint ; }
	BYTE GetHP_Point() { return m_byHP_Point ; }
	void SetMP_Point( BYTE mpPoint ) { m_byMP_Point = mpPoint ; }
	BYTE GetMP_Point() { return m_byMP_Point ; }

	
	// 071206 KTH -- ��ų�� ������ �ʱ�ȭ ������ �˻�.
	BOOL	IsResetSkill()	{	return m_bResetSkill;	}
	void	SetResetSkill( BOOL bResetSkill )	{	m_bResetSkill = bResetSkill;	}
	BOOL	IsResetStat()	{	return m_bResetStat;	}
	void	SetResetStat( BOOL bResetStat )	{	m_bResetStat = bResetStat;	}

private:
	BOOL	m_bResetSkill;
	BOOL	m_bResetStat;

protected:
	DWORD	m_dwReturnSkillMoney;	//������ �� �ӽ� ���� ���
public:
	void	SetRetrunSkillMoney( DWORD ReturnSkillMoney )	{	m_dwReturnSkillMoney = ReturnSkillMoney;	}
	DWORD	GetReturnSkillMoney()							{	return m_dwReturnSkillMoney;	}

	// 071210 KTH -- Ȯ���κ��丮 ���
	WORD	GetInventoryExpansion()							{ return m_HeroCharacterInfo.wInventoryExpansion; }
	// 071225 KTH -- (Fix) Ȯ�� �κ��丮�� ������ ����� ��� Ȯ��� ����ŭ ������ ������ �ٽ� �����Ͽ� �ش�.
	void	SetInventoryExpansion(WORD InventoryExpansion)	{ 
																m_HeroCharacterInfo.wInventoryExpansion = InventoryExpansion; 
																m_InventorySlot.SetSlotNum(SLOT_INVENTORY_NUM + GetInventoryExpansionSize());
															}
	BOOL	IncreaseInventoryExpansion();
	// 071213 KTH -- Ȯ���κ��丮�� ũ�� ��������
	WORD	GetInventoryExpansionSize()						{	return (WORD)m_HeroCharacterInfo.wInventoryExpansion * TABCELL_INVENTORY_NUM;	}

	// 071226 KTH -- �̸� ����.
	VOID	SetCharacterName(char* name)					{	strcpy(m_BaseObjectInfo.ObjectName, name);	}

	// 071208 LYW --- Player : ĳ���� ���� �ʱ�ȭ��, ���� ���� �� �ٸ� ������Ʈ ���� ���� ����ϱ� ���� �Լ� �߰�.
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

// 080411 NYJ - ����
protected:
	DWORD m_dwFishingPlaceIdx;				// ������NPC Instance Index
	DWORD m_dwFishingStartTime;				// ���۽ð�
	DWORD m_dwFishingProcessTime;			// ����ð�
	DWORD m_dwFishingBaitIdx;				// ����� �̳� Item index

	float m_fFishItemRate[eFishItem_Max];	// ��� ���� ����Ȯ�� �߰�����
	float m_fFishBaitRate[eFishItem_Max];	// �̳��� ���� ����Ȯ�� �߰�����


	DWORD m_dwFM_MissionCode;				// �������� �̼�
	DWORD m_dwFM_StartTime;					// �̼� ���� �ð�

	// ���ð��õ���Ÿ
	WORD  m_wFishingLevel;					// ���ü��õ��
	// 080602 LYW --- Player : ����ġ ��ġ (__int32) ���� (__int64) ������� ���� ó��.
	//DWORD m_dwFishingExp;					// ���ü��õ�
	EXPTYPE m_dwFishingExp;					// ���ü��õ�

	DWORD m_dwFishPoint;					// ���������Ʈ
	
public:
	std::list<DWORD> m_lstGetFishList;		// �̼��� �ֱ� ���� 3���� ���, FM(FishingMission)

	void FishingInfoClear();				// ���ð������� �ѹ濡 Ŭ����.

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

	// ���ù̼ǰ���
	void SetFM_Mission(DWORD dwCode)						{m_dwFM_MissionCode = dwCode;}
	DWORD GetFM_Mission()									{return m_dwFM_MissionCode;}

	// ���ð��õ���Ÿ
	void SetFishingLevel(WORD wLevel)						{m_wFishingLevel = wLevel;}
	WORD GetFishingLevel()									{return m_wFishingLevel;}
	// 080602 LYW --- Player : ����ġ ��ġ (__int32) ���� (__int64) ������� ���� ó��.
	//void SetFishingExp(DWORD dwExp);
	void SetFishingExp(EXPTYPE dwExp);
	// 080602 LYW --- Player : ����ġ ��ġ (__int32) ���� (__int64) ������� ���� ó��.
	//DWORD GetFishingExp()									{return m_dwFishingExp;}
	EXPTYPE GetFishingExp()									{return m_dwFishingExp;}
	void SetFishPoint(DWORD dwPoint)						{m_dwFishPoint = dwPoint; if(MAX_FISHPOINT<m_dwFishPoint) m_dwFishPoint=MAX_FISHPOINT;}
	DWORD GetFishPoint()									{return m_dwFishPoint;}

	bool	tempHideParts[eWearedItem_Max];					// ��������� ����.  m_HeroCharacterInfo�� HideParts�� �־ �������� ������ �Ѵ�.
	const bool*	GetHideParts() const						{return m_HeroCharacterInfo.HideParts;}
	void	SetHideParts(bool* pHideParts);

	// 080616 LUJ, ������ ���� ����
public:
	inline BOOL IsHaveTrap( DWORD monsterIndex ) const	{ return mTrapSet->end() != mTrapSet->find( monsterIndex ); }
	inline void AddTrap( DWORD monsterIndex )			{ mTrapSet->insert( monsterIndex ); }
	inline void RemoveTrap( DWORD monsterIndex )		{ mTrapSet->erase( monsterIndex ); }

private:
	// 080616 LUJ, �� ������ ������ �ϳ��� ���� �����ϵ���, ������ ������ MonsterKind�� �����Ѵ�
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

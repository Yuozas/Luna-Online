#pragma once

#include "Player.h"
#include "CAction.h"
#include "../[cc]skill/client/delay/timedelay.h"
#include "../[cc]skill/client/object/skillobject.h"

// desc_hseos_패밀리01
// S 패밀리 추가 added by hseos 2007.07.12
#include "../hseos/Family/SHFamily.h"
// E 패밀리 추가 added by hseos 2007.07.12

class cStatus;

class CHero : public CPlayer  
{
	//////////////////////////////////////////////////////////////////////////////////////////
	/// 변수 선언부

	///--------------------------------------------------------------------/
	/// 케릭터 기본&관리 정보 관련 변수------------------------------------/

	/// 기본 정보
	HERO_TOTALINFO		m_HeroTotalInfo;
	EXPTYPE				m_maxExpPoint;

	/// 접속 시간
	stPlayTime			m_stPlayTime;

	/// 케릭터 기본&관리 정보 관련 변수------------------------------------/
	///--------------------------------------------------------------------/

	///--------------------------------------------------------------------/
	/// 스테이터스&각종 수치 정보 관련 변수--------------------------------/

	PlayerStat	mCharStat;
	PlayerStat	mItemStat;

	// 080313 LUJ, 세트 아이템 스탯 분리
	PlayerStat	mSetItemStat;

	CYHHashTable< cStatus >	mStatusTable;


	/// 스테이터스&각종 수치 정보 관련 변수--------------------------------/
	///--------------------------------------------------------------------/

	///--------------------------------------------------------------------/
	/// 전투/행동 정보 관련 변수-------------------------------------------/

	/// 액션
	CAction				m_MovingAction;
	CAction				m_NextAction;
	CAction				m_SkillStartAction;

	/// 사회행동
	DWORD				m_dwLastSocietyAction;

	BOOL				m_bIsKyungGongMode;
	BOOL				m_bUngiFlag;

	DWORD		mAccuracy;
	DWORD		mAvoid;
	DWORD		mBlock;
	DWORD		mPhysicAttackMax;
	DWORD		mPhysicAttackMin;
	DWORD		mMagicAttack;
	DWORD		mPhysicDefense;
	DWORD		mMagicDefense;
	DWORD		mCriticalRate;
	DWORD		mCriticalDamage;
	DWORD		mLifeRecover;
	DWORD		mManaRecover;
	DWORD		mMaxLife;
	DWORD		mMaxMana;

	/// 전투/행동 정보 관련 변수-------------------------------------------/
	///--------------------------------------------------------------------/


	///--------------------------------------------------------------------/
	/// 아이템 정보 관련 변수----------------------------------------------/

	BYTE				m_StorageNum;

	/// 아이템 정보 관련 변수----------------------------------------------/
	///--------------------------------------------------------------------/

	///--------------------------------------------------------------------/
	/// 스킬 정보 관련 변수------------------------------------------------/

	/// 딜레이
	BOOL				m_bSkillCooling;
	DWORD				m_SkillCoolTimeDuration;
	DWORD				m_SkillCoolTimeStart;

	/// 자동공격	
	BOOL				m_bIsAutoAttackMode;
	BOOL				m_bIsAutoAttacking;
	CActionTarget		m_AutoAttackTarget;

	/// 기본공격
	DWORD				m_CurComboNum;

	cTimeDelay			mSkillDelay;
	cTimeDelay			mSkillCancelDelay;
	cSkillObject*		mpCurrentSkill;

	AbnormalStatus mAbnormalStatus;
	Status				mBuffStatus;
	Status				mRateBuffStatus;
	Status				mPassiveStatus;
	Status				mRatePassiveStatus;
	cPtrList			mActiveStatusList;
	cPtrList			mInertStatusList;
	
	CYHHashTable<cSkillObject>	mBuffSkillList;

	/// 스킬 정보 관련 변수------------------------------------------------/
	///--------------------------------------------------------------------/


	// desc_hseos_패밀리01
	// S 패밀리 추가 added by hseos 2007.07.12
	CSHFamily*			m_pcsFamily;
	// E 패밀리 추가 added by hseos 2007.07.12

	// desc_hseos_몬스터미터01
	// S 몬스터미터 추가 added by hseos 2007.05.23 2008.01.11:CPlayer 에서 옮김
	stMONSTERMETER		m_stMonstermeterInfo;
	// E 몬스터미터 추가 added by hseos 2007.05.23 2008.01.11:CPlayer 에서 옮김

	// desc_hseos_결혼_01
	// S 결혼 추가 added by hseos 2008.01.11
	BYTE 				m_nMarriagePartnerRace;
	BYTE 				m_nMarriagePartnerJobFirst;
	BYTE 				m_nMarriagePartnerJobCur;
	BYTE 				m_nMarriagePartnerJobGrade;
	WORD 				m_nMarriagePartnerLv;
	WORD 				m_nMarriageTeleportFreq;
	// E 결혼 추가 added by hseos 2008.01.11

//---KES 장비 방어력 적용 여부 071128
	BOOL	m_bNoDeffenceFromArmor;
//----------------------------------

	// 071210 LYW --- Hero : HERO의 확장 인벤토리 수를 담는 변수 추가.
	BYTE	m_byExtendedInvenCount ;

	/// 변수 선언부
	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
	/// 함수 선언부

public:

	/// 생성자/소멸자
	CHero();
	virtual ~CHero();

	///--------------------------------------------------------------------/
	/// 케릭터 기본 정보 관련 함수-----------------------------------------/

	/// 초기화
	void				InitHero( HERO_TOTALINFO * pHeroTotalInfo );

	/// 케릭터 정보
	void				GetHeroTotalInfo( HERO_TOTALINFO * info );
	HERO_TOTALINFO*		GetHeroTotalInfo() { return &m_HeroTotalInfo; }

	/// 경험치
	EXPTYPE				GetExpPoint() { return m_HeroTotalInfo.ExpPoint; }
	EXPTYPE				GetMaxExpPoint() { return m_maxExpPoint; }
	void				SetExpPoint( EXPTYPE dwPoint, BYTE type=1 );
	void				SetMaxExpPoint( EXPTYPE dwPoint );

	/// 레벨
	LEVELTYPE			GetMaxLevel() { return m_HeroTotalInfo.MaxLevel; }
	void				SetMaxLevel( LEVELTYPE Level ) { m_HeroTotalInfo.MaxLevel = Level; }
	virtual void		SetLevel( LEVELTYPE level );

	void				SetSkillPoint( DWORD point );

	/// 직업
	//	virtual void		SetStage( BYTE Stage );

	/// 스텟 포인트
	void				RefreshLevelupPoint();
	void				SetHeroLevelupPoint( LEVELTYPE dwPoint ) { m_HeroTotalInfo.LevelUpPoint = dwPoint; }
	DWORD				GetHeroLevelUpPoint() { return m_HeroTotalInfo.LevelUpPoint; }

	/// 이름 표시
	void				SetGuageName( char * szName );

	/// 플레이 시간
	void				SetPlayTime( int time ) { m_stPlayTime.value = time; }
	int					GetPlayTime() { m_stPlayTime.value; }

	/// 상태 알림
	void				HeroStateNotify( BYTE State );

	/// 케릭터 기본 정보 관련 함수-----------------------------------------/
	///--------------------------------------------------------------------/

	///--------------------------------------------------------------------/
	/// 스테이터스&각종 수치 정보 관련 함수--------------------------------/

	/// 스텟 계산 구조체
	PlayerStat&	GetCharStats() { return mCharStat; }
	PlayerStat&	GetItemStats() { return mItemStat; }

	// 080313 LUJ, 세트 아이템 스탯 반환
	PlayerStat& GetSetItemStats()	{ return mSetItemStat; }

	/// 기본 스텟
	void				SetStrength( DWORD val );
	void				SetDexterity( DWORD val );
	void				SetVitality( DWORD val );
	void				SetWisdom( DWORD val );
	void				SetIntelligence( DWORD val );

	DWORD				GetVitality()				const;
	DWORD				GetWisdom()					const;
	DWORD				GetStrength()				const;
	DWORD				GetDexterity()				const;
	DWORD				GetIntelligence()			const;

	/// 생명력 / 마나
	virtual void		SetMaxLife( DWORD maxlife );
	virtual void		SetLife( DWORD life, BYTE type = 1 );
	void				ChangeLife( int changeval );

	virtual void		SetMaxMana( DWORD val );
	virtual void		SetMana( DWORD val, BYTE type = 1 );
	virtual DWORD		GetMana() { return m_HeroTotalInfo.Mana; }
	virtual DWORD		DoGetMaxMana();
	BYTE				GetManaPercent();

	DWORD				GetMaxLifeWithItem();
	DWORD				GetMaxManaWithItem();
	/// 전투 관련 스텟
	/*
	virtual DWORD		DoGetPhyDefense() { return mCharStat.PhysicalDefense+mItemStat.PhysicalDefense; }
	virtual float		DoGetAttDefense( WORD Attrib ) { return mItemStat.AttributeResist.GetElement_Val( Attrib ); }	
	virtual DWORD		DoGetPhyAttackPowerMin();
	virtual DWORD		DoGetPhyAttackPowerMax();
	virtual float		DoGetAddAttackRange() { return GetDexterity() / 3.f; }
	virtual DWORD		DoGetCritical() { return mCharStat.Critical + mItemStat.Critical;	}
	virtual DWORD		DoGetDecisive() { return mCharStat.Decisive + mItemStat.Critical;	}
	*/
		// 070411 LYW --- Hero : Add functions to setting physical part.
		virtual DWORD		DoGetAttackRate();
	virtual DWORD		DoGetDefenseRate();
	virtual DWORD		DoGetMagAttackRate();
	virtual DWORD		DoGetMagDefenseRate();
	virtual DWORD		DoGetAccuracyRate();
	virtual DWORD		DoGetEvasionRate();
	virtual DWORD		DoGetCriticalRate();		



	/// 명성/악명
	void				SetFame( FAMETYPE val );
	FAMETYPE			GetFame() { return m_HeroTotalInfo.Fame; }
	virtual void		SetBadFame( FAMETYPE val );

	WORD				GetKyungGongGrade()	{ return m_HeroTotalInfo.KyungGongGrade; }
	void				SetKyungGongGrade( WORD grade );

	/// 스테이터스
	virtual void		AddStatus( cStatus* pStatus );
	virtual void		RemoveStatus( cStatus* pStatus );
	virtual cStatus*	GetStatus( WORD kind );

	DWORD				GetAccuracy();
	DWORD				GetAvoid();
	DWORD				GetBlock();
	DWORD				GetPhysicAttackMax();
	DWORD				GetPhysicAttackMin();
	DWORD				GetMagicAttack();
	DWORD				GetPhysicDefense();
	DWORD				GetMagicDefense();
	DWORD				GetCriticalRate();
	DWORD				GetCriticalDamage();
	DWORD				GetLifeRecover();
	DWORD				GetManaRecover();

	/// 스테이터스&각종 수치 정보 관련 함수--------------------------------/
	///--------------------------------------------------------------------/

	///--------------------------------------------------------------------/
	/// 전투/행동 관련 함수------------------------------------------------/

	/// 파티
	DWORD				GetPartyIdx() { return m_HeroTotalInfo.PartyID; }
	void				SetPartyIdx( DWORD PartyId ) { m_HeroTotalInfo.PartyID = PartyId;	}

	/// 길드
	void				SetGuild();
	virtual void		ClearGuildInfo();
	void				SetGuildName( char* Name );
	char*				GetGuildName();
	void				SetGuildEntryDate( char* day );
	char*				GetGuildEntryDate();

	void				SetFamily();
	void				SetFamilyName( char* Name );
	char*				GetFamilyName();
	void				SetFamilyEntryDate( char* day );
	char*				GetFamilyEntryDate();

	/// 사회 행동
	virtual BOOL		StartSocietyAct( int nStartMotion, int nIngMotion = -1, int nEndMotion = -1, BOOL bHideWeapon = FALSE );
	virtual BOOL		EndSocietyAct();
	void				StartSocietyActForQuest( int nStartMotion, int nIngMotion = -1, int nEndMotion = -1, BOOL bHideWeapon = FALSE );
	BOOL				CanSocietyAction( DWORD curTime );

	/// 기본 행동
	void				Die( CObject* pAttacker, BOOL bFatalDamage, BOOL bCritical, BOOL bDecisive );
	void				Damage( CObject* pAttacker, BYTE DamageKind, DWORD Damage, BOOL bCritical, BOOL bDecisive );
	void				Revive( VECTOR3* pRevivePos );
	void				Heal( CObject* pHealer,BYTE HealKind,DWORD HealVal );

	BOOL				GetKyungGongMode();
	void				SetKyungGongMode( BOOL bMode );

	/// 액션
	void				SetMovingAction( CAction* pAction ) { m_MovingAction.CopyFrom( pAction ); }
	void				SetNextAction( CAction* pAction ) { m_NextAction.CopyFrom( pAction );	}
	CAction*			GetNextAction()	{ return &m_NextAction; }
	void				SetSkillStartAction( CAction* pAction ) { m_SkillStartAction.CopyFrom( pAction ); }

	/// 전투/행동 관련 함수------------------------------------------------/
	///--------------------------------------------------------------------/

	///--------------------------------------------------------------------/
	/// 프로세스 관련 함수-------------------------------------------------/

	/// 기본 프로세스
	void				Process();

	/// 상태 프로세스
	void				OnStartObjectState( BYTE State );
	void				OnEndObjectState( BYTE State );

	/// 인터페이스
	void				ApplyInterface();

	/// 프로세스 관련 함수-------------------------------------------------/
	///--------------------------------------------------------------------/

	///--------------------------------------------------------------------/
	/// 아이템 정보 관련 함수----------------------------------------------/

	/// 돈
	void				SetMoney( MONEYTYPE Money );
	MONEYTYPE			GetMoney() { return m_HeroTotalInfo.Money; }

	/// 창고
	BYTE				GetStorageNum()	{ return m_StorageNum; }
	void				SetStorageNum( BYTE num ) { m_StorageNum = num; }

	/// 캐시 아이템
	void				CalcShopItemOption( WORD wIdx, BOOL bAdd, DWORD Param=0 );
	void				ActiveOptionInfoToInterface();
	void				CheckShopItemUseInfoToInventory( WORD ItemIdx, WORD ItemPos );

	/// 아이템 정보 관련 함수----------------------------------------------/
	///--------------------------------------------------------------------/

	///--------------------------------------------------------------------/
	/// 스킬 정보 관련 함수------------------------------------------------/

	/// 관련 클래스
	friend class		CAttackManager;

	/// 딜레이
	BOOL				IsSkillCooling() {	return m_bSkillCooling; }
	void				SetSkillCoolTime( DWORD CoolTime )	
	{
		m_SkillCoolTimeDuration = CoolTime;
		m_SkillCoolTimeStart = gCurTime;
		m_bSkillCooling = TRUE;
	}

	/// 자동공격
	BOOL				IsAutoAttacking() { return m_bIsAutoAttacking; }
	CActionTarget*		GetAutoAttackTarget() { return &m_AutoAttackTarget; }
	void				EnableAutoAttack( CActionTarget* pTarget )
	{	
		if( m_bIsAutoAttackMode && pTarget->GetTargetKind() == eActionTargetKind_Object )
		{
			m_bIsAutoAttacking = TRUE;	
			m_AutoAttackTarget.CopyFrom( pTarget ); 
		}
	}
	void				DisableAutoAttack() 
	{ 
		m_bIsAutoAttacking = FALSE; 
		m_AutoAttackTarget.CopyFrom( NULL ); 
	}
	void				ToggleAutoAttackMode() { m_bIsAutoAttackMode = !m_bIsAutoAttackMode; }

	DWORD				GetCurComboNum() { return m_CurComboNum < MAX_COMBO_NUM ? m_CurComboNum : 0; }
	void				SetCurComboNum( DWORD num )	{ m_CurComboNum = num; }


	/// 타겟 정보
	void				ClearTarget( CObject* pObject );
	void				OnHitTarget( CObject* pMainTarget );

	/// 스킬 정보
	int					GetSkillLevel( DWORD SkillIdx );

	cTimeDelay*			GetSkillDelay() { return &mSkillDelay; }
	void				StartSkillProgressBar( DWORD time );

	Status*				GetBuffStatus() { return &mBuffStatus; }
	Status*				GetRateBuffStatus() { return &mRateBuffStatus; }
	Status*				GetPassiveStatus() { return &mPassiveStatus; }
	Status*				GetRatePassiveStatus() { return &mRatePassiveStatus; }

	AbnormalStatus*	GetAbnormalStatus() { return &mAbnormalStatus; }


	void	AddSpecialSkill( cBuffSkillInfo* pInfo ) { mInertStatusList.AddTail( (void*)pInfo ); }
	void	RemoveSpecialSkill( cBuffSkillInfo* pInfo ) { mInertStatusList.Remove( (void*)pInfo ); }
	void	ProcessSpecialSkill();

	void	AddBuffSkillObject( cSkillObject* pObj )
	{
		mBuffSkillList.Add( pObj, pObj->GetSkillIdx() / 100 );
	}

	cSkillObject* GetBuffSkillObject( DWORD idx )
	{
		return mBuffSkillList.GetData( idx );
	}

	void	RemoveBuffSkillObject( DWORD idx )
	{
		mBuffSkillList.Remove( idx );
	}
	/// 스킬 정보 관련 함수------------------------------------------------/
	///--------------------------------------------------------------------/

	/// 플레이어의 매칭 정보를 체크
public:
	void AddMatch();
	void RemoveMatch();

	// 그리드 내의 전체 플레이어 상대로 하트 재계산. 히어로의 매칭 정보가 바뀌었을 때 호출되어야 한다.
	void CheckAllHeart();

private:
	typedef std::set< DWORD > PlayerSet;

	// 하트 매칭된 플레이어 수
	int m_matchedQuantity;


	// desc_hseos_패밀리01
	// S 패밀리 추가 added by hseos 2007.07.12
public:
	void		SetFamily(CSHFamily* pcsFamily)			{ m_pcsFamily->Set(pcsFamily); }
	CSHFamily*	GetFamily()								{ return m_pcsFamily; }
	// E 패밀리 추가 added by hseos 2007.07.12

	// desc_hseos_몬스터미터01
	// S 몬스터미터 추가 added by hseos 2007.05.23 2008.01.11:CPlayer 에서 옮김
	void				SetMonstermeterPlaytime(DWORD nTime, DWORD nTimeTotal)		{ m_stMonstermeterInfo.nPlayTime = nTime; m_stMonstermeterInfo.nPlayTimeTotal = nTimeTotal; }
	void				SetMonstermeterKillMonNum(DWORD nNum, DWORD nNumTotal)		{ m_stMonstermeterInfo.nKillMonsterNum = nNum; m_stMonstermeterInfo.nKillMonsterNumTotal = nNumTotal; }
	void				SetMonstermeterPlaytimeTick(DWORD nTick)					{ m_stMonstermeterInfo.nPlayTimeTick = nTick; }
	stMONSTERMETER*		GetMonstermeterInfo()										{ return &m_stMonstermeterInfo; }

	void				ProcMonstermeterPlayTime();
	// E 몬스터미터 추가 added by hseos 2007.05.23 2008.01.11:CPlayer 에서 옮김

	// desc_hseos_결혼_01
	// S 결혼 추가 added by hseos 2008.01.11
	BYTE 				GetMarriagePartnerRace()			{ return m_nMarriagePartnerRace; }
	BYTE 				GetMarriagePartnerJobFirst()		{ return m_nMarriagePartnerJobFirst; }
	BYTE 				GetMarriagePartnerJobCur()			{ return m_nMarriagePartnerJobCur; }
	BYTE 				GetMarriagePartnerJobGrade()		{ return m_nMarriagePartnerJobGrade; }
	WORD 				GetMarriagePartnerLv()				{ return m_nMarriagePartnerLv; }
	WORD 				GetMarriageTeleportFreq()			{ return m_nMarriageTeleportFreq; }

	void				SetMarriagePartnerRace(BYTE nRace)					{ m_nMarriagePartnerRace		= nRace; }
	void				SetMarriagePartnerJobFirst(BYTE nJobFirst)			{ m_nMarriagePartnerJobFirst	= nJobFirst; }
	void				SetMarriagePartnerJobCur(BYTE nJobCur)				{ m_nMarriagePartnerJobCur		= nJobCur; }
	void				SetMarriagePartnerJobGrade(BYTE nJobGrade)			{ m_nMarriagePartnerJobGrade	= nJobGrade; }
	void				SetMarriagePartnerLv(WORD nLv)						{ m_nMarriagePartnerLv			= nLv; }
	void				SetMarriagePartnerTeleportFreq(WORD nTeleportFreq)	{ m_nMarriageTeleportFreq		= nTeleportFreq; }
	// E 결혼 추가 added by hseos 2008.01.11

	// 이동속도 정보 반환
public:
	virtual float DoGetMoveSpeed();

	cTimeDelay* GetSkillCancelDelay() { return &mSkillCancelDelay; }
	void SetCurrentSkill( cSkillObject* pSkill ) { mpCurrentSkill = pSkill; }
	cSkillObject* GetCurrentSkill() { return mpCurrentSkill; }

//---KES 장비 방어력 적용 여부 071128
	BOOL	IsNoDeffenceFromArmor()		{ return m_bNoDeffenceFromArmor; }
//----------------------------------

	// 071210 LYW --- Hero : HERO의 확장 인벤토리 수를 설정 및 반환하는 함수 추가.
	void Set_HeroExtendedInvenCount(BYTE count) ;
	BYTE Get_HeroExtendedInvenCount() { return m_byExtendedInvenCount ; }

	void PassiveSkillCheckForWeareItem();

//---KES 따라가기
protected:
	DWORD	m_dwFollowPlayerIdx;
public:
	void SetFollowPlayer( DWORD PlayerIdx )		{ m_dwFollowPlayerIdx = PlayerIdx; }
	DWORD GetFollowPlayer()						{ return m_dwFollowPlayerIdx; }
//---------------


	// 080310 NYJ --- 상점검색 테스트용 (현재사용하지 않음)
	//void SetStallSearch(char* szSearch);
	//char* GetStallSearch() {return m_szStallSearch;}
};

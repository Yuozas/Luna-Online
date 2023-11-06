#pragma once

#include "Player.h"
#include "CAction.h"
#include "../[cc]skill/client/delay/timedelay.h"
#include "../[cc]skill/client/object/skillobject.h"

// desc_hseos_�йи�01
// S �йи� �߰� added by hseos 2007.07.12
#include "../hseos/Family/SHFamily.h"
// E �йи� �߰� added by hseos 2007.07.12

class cStatus;

class CHero : public CPlayer  
{
	//////////////////////////////////////////////////////////////////////////////////////////
	/// ���� �����

	///--------------------------------------------------------------------/
	/// �ɸ��� �⺻&���� ���� ���� ����------------------------------------/

	/// �⺻ ����
	HERO_TOTALINFO		m_HeroTotalInfo;
	EXPTYPE				m_maxExpPoint;

	/// ���� �ð�
	stPlayTime			m_stPlayTime;

	/// �ɸ��� �⺻&���� ���� ���� ����------------------------------------/
	///--------------------------------------------------------------------/

	///--------------------------------------------------------------------/
	/// �������ͽ�&���� ��ġ ���� ���� ����--------------------------------/

	PlayerStat	mCharStat;
	PlayerStat	mItemStat;

	// 080313 LUJ, ��Ʈ ������ ���� �и�
	PlayerStat	mSetItemStat;

	CYHHashTable< cStatus >	mStatusTable;


	/// �������ͽ�&���� ��ġ ���� ���� ����--------------------------------/
	///--------------------------------------------------------------------/

	///--------------------------------------------------------------------/
	/// ����/�ൿ ���� ���� ����-------------------------------------------/

	/// �׼�
	CAction				m_MovingAction;
	CAction				m_NextAction;
	CAction				m_SkillStartAction;

	/// ��ȸ�ൿ
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

	/// ����/�ൿ ���� ���� ����-------------------------------------------/
	///--------------------------------------------------------------------/


	///--------------------------------------------------------------------/
	/// ������ ���� ���� ����----------------------------------------------/

	BYTE				m_StorageNum;

	/// ������ ���� ���� ����----------------------------------------------/
	///--------------------------------------------------------------------/

	///--------------------------------------------------------------------/
	/// ��ų ���� ���� ����------------------------------------------------/

	/// ������
	BOOL				m_bSkillCooling;
	DWORD				m_SkillCoolTimeDuration;
	DWORD				m_SkillCoolTimeStart;

	/// �ڵ�����	
	BOOL				m_bIsAutoAttackMode;
	BOOL				m_bIsAutoAttacking;
	CActionTarget		m_AutoAttackTarget;

	/// �⺻����
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

	/// ��ų ���� ���� ����------------------------------------------------/
	///--------------------------------------------------------------------/


	// desc_hseos_�йи�01
	// S �йи� �߰� added by hseos 2007.07.12
	CSHFamily*			m_pcsFamily;
	// E �йи� �߰� added by hseos 2007.07.12

	// desc_hseos_���͹���01
	// S ���͹��� �߰� added by hseos 2007.05.23 2008.01.11:CPlayer ���� �ű�
	stMONSTERMETER		m_stMonstermeterInfo;
	// E ���͹��� �߰� added by hseos 2007.05.23 2008.01.11:CPlayer ���� �ű�

	// desc_hseos_��ȥ_01
	// S ��ȥ �߰� added by hseos 2008.01.11
	BYTE 				m_nMarriagePartnerRace;
	BYTE 				m_nMarriagePartnerJobFirst;
	BYTE 				m_nMarriagePartnerJobCur;
	BYTE 				m_nMarriagePartnerJobGrade;
	WORD 				m_nMarriagePartnerLv;
	WORD 				m_nMarriageTeleportFreq;
	// E ��ȥ �߰� added by hseos 2008.01.11

//---KES ��� ���� ���� ���� 071128
	BOOL	m_bNoDeffenceFromArmor;
//----------------------------------

	// 071210 LYW --- Hero : HERO�� Ȯ�� �κ��丮 ���� ��� ���� �߰�.
	BYTE	m_byExtendedInvenCount ;

	/// ���� �����
	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
	/// �Լ� �����

public:

	/// ������/�Ҹ���
	CHero();
	virtual ~CHero();

	///--------------------------------------------------------------------/
	/// �ɸ��� �⺻ ���� ���� �Լ�-----------------------------------------/

	/// �ʱ�ȭ
	void				InitHero( HERO_TOTALINFO * pHeroTotalInfo );

	/// �ɸ��� ����
	void				GetHeroTotalInfo( HERO_TOTALINFO * info );
	HERO_TOTALINFO*		GetHeroTotalInfo() { return &m_HeroTotalInfo; }

	/// ����ġ
	EXPTYPE				GetExpPoint() { return m_HeroTotalInfo.ExpPoint; }
	EXPTYPE				GetMaxExpPoint() { return m_maxExpPoint; }
	void				SetExpPoint( EXPTYPE dwPoint, BYTE type=1 );
	void				SetMaxExpPoint( EXPTYPE dwPoint );

	/// ����
	LEVELTYPE			GetMaxLevel() { return m_HeroTotalInfo.MaxLevel; }
	void				SetMaxLevel( LEVELTYPE Level ) { m_HeroTotalInfo.MaxLevel = Level; }
	virtual void		SetLevel( LEVELTYPE level );

	void				SetSkillPoint( DWORD point );

	/// ����
	//	virtual void		SetStage( BYTE Stage );

	/// ���� ����Ʈ
	void				RefreshLevelupPoint();
	void				SetHeroLevelupPoint( LEVELTYPE dwPoint ) { m_HeroTotalInfo.LevelUpPoint = dwPoint; }
	DWORD				GetHeroLevelUpPoint() { return m_HeroTotalInfo.LevelUpPoint; }

	/// �̸� ǥ��
	void				SetGuageName( char * szName );

	/// �÷��� �ð�
	void				SetPlayTime( int time ) { m_stPlayTime.value = time; }
	int					GetPlayTime() { m_stPlayTime.value; }

	/// ���� �˸�
	void				HeroStateNotify( BYTE State );

	/// �ɸ��� �⺻ ���� ���� �Լ�-----------------------------------------/
	///--------------------------------------------------------------------/

	///--------------------------------------------------------------------/
	/// �������ͽ�&���� ��ġ ���� ���� �Լ�--------------------------------/

	/// ���� ��� ����ü
	PlayerStat&	GetCharStats() { return mCharStat; }
	PlayerStat&	GetItemStats() { return mItemStat; }

	// 080313 LUJ, ��Ʈ ������ ���� ��ȯ
	PlayerStat& GetSetItemStats()	{ return mSetItemStat; }

	/// �⺻ ����
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

	/// ����� / ����
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
	/// ���� ���� ����
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



	/// ��/�Ǹ�
	void				SetFame( FAMETYPE val );
	FAMETYPE			GetFame() { return m_HeroTotalInfo.Fame; }
	virtual void		SetBadFame( FAMETYPE val );

	WORD				GetKyungGongGrade()	{ return m_HeroTotalInfo.KyungGongGrade; }
	void				SetKyungGongGrade( WORD grade );

	/// �������ͽ�
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

	/// �������ͽ�&���� ��ġ ���� ���� �Լ�--------------------------------/
	///--------------------------------------------------------------------/

	///--------------------------------------------------------------------/
	/// ����/�ൿ ���� �Լ�------------------------------------------------/

	/// ��Ƽ
	DWORD				GetPartyIdx() { return m_HeroTotalInfo.PartyID; }
	void				SetPartyIdx( DWORD PartyId ) { m_HeroTotalInfo.PartyID = PartyId;	}

	/// ���
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

	/// ��ȸ �ൿ
	virtual BOOL		StartSocietyAct( int nStartMotion, int nIngMotion = -1, int nEndMotion = -1, BOOL bHideWeapon = FALSE );
	virtual BOOL		EndSocietyAct();
	void				StartSocietyActForQuest( int nStartMotion, int nIngMotion = -1, int nEndMotion = -1, BOOL bHideWeapon = FALSE );
	BOOL				CanSocietyAction( DWORD curTime );

	/// �⺻ �ൿ
	void				Die( CObject* pAttacker, BOOL bFatalDamage, BOOL bCritical, BOOL bDecisive );
	void				Damage( CObject* pAttacker, BYTE DamageKind, DWORD Damage, BOOL bCritical, BOOL bDecisive );
	void				Revive( VECTOR3* pRevivePos );
	void				Heal( CObject* pHealer,BYTE HealKind,DWORD HealVal );

	BOOL				GetKyungGongMode();
	void				SetKyungGongMode( BOOL bMode );

	/// �׼�
	void				SetMovingAction( CAction* pAction ) { m_MovingAction.CopyFrom( pAction ); }
	void				SetNextAction( CAction* pAction ) { m_NextAction.CopyFrom( pAction );	}
	CAction*			GetNextAction()	{ return &m_NextAction; }
	void				SetSkillStartAction( CAction* pAction ) { m_SkillStartAction.CopyFrom( pAction ); }

	/// ����/�ൿ ���� �Լ�------------------------------------------------/
	///--------------------------------------------------------------------/

	///--------------------------------------------------------------------/
	/// ���μ��� ���� �Լ�-------------------------------------------------/

	/// �⺻ ���μ���
	void				Process();

	/// ���� ���μ���
	void				OnStartObjectState( BYTE State );
	void				OnEndObjectState( BYTE State );

	/// �������̽�
	void				ApplyInterface();

	/// ���μ��� ���� �Լ�-------------------------------------------------/
	///--------------------------------------------------------------------/

	///--------------------------------------------------------------------/
	/// ������ ���� ���� �Լ�----------------------------------------------/

	/// ��
	void				SetMoney( MONEYTYPE Money );
	MONEYTYPE			GetMoney() { return m_HeroTotalInfo.Money; }

	/// â��
	BYTE				GetStorageNum()	{ return m_StorageNum; }
	void				SetStorageNum( BYTE num ) { m_StorageNum = num; }

	/// ĳ�� ������
	void				CalcShopItemOption( WORD wIdx, BOOL bAdd, DWORD Param=0 );
	void				ActiveOptionInfoToInterface();
	void				CheckShopItemUseInfoToInventory( WORD ItemIdx, WORD ItemPos );

	/// ������ ���� ���� �Լ�----------------------------------------------/
	///--------------------------------------------------------------------/

	///--------------------------------------------------------------------/
	/// ��ų ���� ���� �Լ�------------------------------------------------/

	/// ���� Ŭ����
	friend class		CAttackManager;

	/// ������
	BOOL				IsSkillCooling() {	return m_bSkillCooling; }
	void				SetSkillCoolTime( DWORD CoolTime )	
	{
		m_SkillCoolTimeDuration = CoolTime;
		m_SkillCoolTimeStart = gCurTime;
		m_bSkillCooling = TRUE;
	}

	/// �ڵ�����
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


	/// Ÿ�� ����
	void				ClearTarget( CObject* pObject );
	void				OnHitTarget( CObject* pMainTarget );

	/// ��ų ����
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
	/// ��ų ���� ���� �Լ�------------------------------------------------/
	///--------------------------------------------------------------------/

	/// �÷��̾��� ��Ī ������ üũ
public:
	void AddMatch();
	void RemoveMatch();

	// �׸��� ���� ��ü �÷��̾� ���� ��Ʈ ����. ������� ��Ī ������ �ٲ���� �� ȣ��Ǿ�� �Ѵ�.
	void CheckAllHeart();

private:
	typedef std::set< DWORD > PlayerSet;

	// ��Ʈ ��Ī�� �÷��̾� ��
	int m_matchedQuantity;


	// desc_hseos_�йи�01
	// S �йи� �߰� added by hseos 2007.07.12
public:
	void		SetFamily(CSHFamily* pcsFamily)			{ m_pcsFamily->Set(pcsFamily); }
	CSHFamily*	GetFamily()								{ return m_pcsFamily; }
	// E �йи� �߰� added by hseos 2007.07.12

	// desc_hseos_���͹���01
	// S ���͹��� �߰� added by hseos 2007.05.23 2008.01.11:CPlayer ���� �ű�
	void				SetMonstermeterPlaytime(DWORD nTime, DWORD nTimeTotal)		{ m_stMonstermeterInfo.nPlayTime = nTime; m_stMonstermeterInfo.nPlayTimeTotal = nTimeTotal; }
	void				SetMonstermeterKillMonNum(DWORD nNum, DWORD nNumTotal)		{ m_stMonstermeterInfo.nKillMonsterNum = nNum; m_stMonstermeterInfo.nKillMonsterNumTotal = nNumTotal; }
	void				SetMonstermeterPlaytimeTick(DWORD nTick)					{ m_stMonstermeterInfo.nPlayTimeTick = nTick; }
	stMONSTERMETER*		GetMonstermeterInfo()										{ return &m_stMonstermeterInfo; }

	void				ProcMonstermeterPlayTime();
	// E ���͹��� �߰� added by hseos 2007.05.23 2008.01.11:CPlayer ���� �ű�

	// desc_hseos_��ȥ_01
	// S ��ȥ �߰� added by hseos 2008.01.11
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
	// E ��ȥ �߰� added by hseos 2008.01.11

	// �̵��ӵ� ���� ��ȯ
public:
	virtual float DoGetMoveSpeed();

	cTimeDelay* GetSkillCancelDelay() { return &mSkillCancelDelay; }
	void SetCurrentSkill( cSkillObject* pSkill ) { mpCurrentSkill = pSkill; }
	cSkillObject* GetCurrentSkill() { return mpCurrentSkill; }

//---KES ��� ���� ���� ���� 071128
	BOOL	IsNoDeffenceFromArmor()		{ return m_bNoDeffenceFromArmor; }
//----------------------------------

	// 071210 LYW --- Hero : HERO�� Ȯ�� �κ��丮 ���� ���� �� ��ȯ�ϴ� �Լ� �߰�.
	void Set_HeroExtendedInvenCount(BYTE count) ;
	BYTE Get_HeroExtendedInvenCount() { return m_byExtendedInvenCount ; }

	void PassiveSkillCheckForWeareItem();

//---KES ���󰡱�
protected:
	DWORD	m_dwFollowPlayerIdx;
public:
	void SetFollowPlayer( DWORD PlayerIdx )		{ m_dwFollowPlayerIdx = PlayerIdx; }
	DWORD GetFollowPlayer()						{ return m_dwFollowPlayerIdx; }
//---------------


	// 080310 NYJ --- �����˻� �׽�Ʈ�� (���������� ����)
	//void SetStallSearch(char* szSearch);
	//char* GetStallSearch() {return m_szStallSearch;}
};

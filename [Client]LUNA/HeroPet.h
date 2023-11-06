#pragma once
#include "pet.h"
#include "CAction.h"
#include "../[cc]skill/client/delay/timedelay.h"
#include "../[cc]skill/client/object/skillobject.h"

enum ePetAI
{
	ePetAI_Aggressive,
	ePetAI_Support,
	ePetAI_Stand,
};

class CHeroPet :
	public CPet
{
	DWORD		mItemDBIdx;
	BYTE		mSkillSlot;
	WORD		mExp;
	BYTE		mFriendly;
	BYTE		mAI;
	BYTE		mType;
	DWORD		mHP;
	DWORD		mMP;
	DWORD		mMaxHP;
	DWORD		mMaxMP;
	DWORD		mDistanceCheckTime;

	DWORD		mTargetID;

	PlayerStat	mPetStat;
	PlayerStat	mItemStat;

	Status		mBuffStatus;
	Status		mRateBuffStatus;
	Status		mPassiveStatus;
	Status		mRatePassiveStatus;

	BOOL				m_bSkillCooling;
	DWORD				m_SkillCoolTimeDuration;
	DWORD				m_SkillCoolTimeStart;

	/// 자동공격	
	BOOL				m_bIsAutoAttackMode;
	BOOL				m_bIsAutoAttacking;
	CActionTarget		m_AutoAttackTarget;

	/// 기본공격
	DWORD				m_CurComboNum;

	cTimeDelay			m_SkillDelay;
	cTimeDelay			mSkillCancelDelay;
	cSkillObject*		mpCurrentSkill;

	AbnormalStatus mAbnormalStatus;

	CAction				m_MovingAction;
	CAction				m_NextAction;

	BOOL		mIsSkillUse;
	DWORD		mSkillDelay;
	DWORD		mSkillDelayCheckTime;

	DWORD		mSkillIndex;

	friend class CPetStateDialog;

public:					
	CHeroPet(void);
	virtual ~CHeroPet(void);

	virtual void InitPet( PET_OBJECT_INFO* pPetObjectInfo );
	virtual void Release();

	void	Process();

	void	CalcStats();

	void	Recall();
	DWORD	SetTarget();
	void	SetTarget( DWORD id );
	void	Attack();
	void	SetUseSkill( DWORD idx ) { mSkillIndex = idx; }
	void	UseSkill();

	void	AddExp();
	virtual void SetWearedItem( ITEMBASE* pItemInfo );
	virtual void SetWearedItem( WORD pos, DWORD idx ) { mWearedItem[ pos ] = idx; }
	DWORD	GetWearedItem( WORD pos ) { if( pos >= ePetEquipSlot_Max ) return 0; return mWearedItem[ pos ]; }
	Status*	GetBuffStatus() { return &mBuffStatus; }
	Status*	GetRateBuffStatus() { return &mRateBuffStatus; }
	Status*	GetPassiveStatus() { return &mPassiveStatus; }
	Status*	GetRatePassiveStatus() { return &mRatePassiveStatus; }

	void	SetCanSkillUse() { mIsSkillUse = FALSE; }
	void	SetSkillDelay( DWORD delay ) { mSkillDelay = delay; mSkillDelayCheckTime = gCurTime; }

	BYTE	GetType()	{ return mType; }
	void	SetType( BYTE type ) { mType = type; }
	void	SetFriendly( BYTE byte );
	void	SetAI( BYTE ai ) { mAI = ai; }
	virtual void	SetLevel( BYTE level );
	virtual void	LevelUp();

	virtual void	SetMaxLife( DWORD maxlife );
	virtual void	SetLife( DWORD life, BYTE type = 1 );
	virtual DWORD	GetLife() { return mHP; }
	virtual void	SetMaxMana( DWORD val );
	virtual void	SetMana( DWORD val, BYTE type = 1 );
	virtual DWORD	GetMana() { return mMP; }
	void	SetExp( WORD exp );
	void	SetSkillSlot( BYTE slot ) { mSkillSlot = slot; }
	BYTE	GetSkillSlot() { return mSkillSlot; }
	DWORD	GetItemDBIdx() { return mItemDBIdx; }
	PlayerStat&	GetItemStats() { return mItemStat; }


	void				SetMovingAction( CAction* pAction ) { m_MovingAction.CopyFrom( pAction ); }
	void				SetNextAction( CAction* pAction ) { m_NextAction.CopyFrom( pAction );	}
	CAction*			GetNextAction()	{ return &m_NextAction; }
	
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

	AbnormalStatus*	GetAbnormalStatus() { return &mAbnormalStatus; }

	cTimeDelay*			GetSkillDelay() { return &m_SkillDelay; }

	DWORD	GetRealSkillIndex( DWORD index );
	void	ClearTarget(CObject* pObject);

	void OnEndObjectState(BYTE State);

};

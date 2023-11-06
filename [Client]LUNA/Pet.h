#pragma once
#include "object.h"

class CPet :
	public CObject
{
protected:
	PET_INFO* mpPetInfo;

	DWORD	mID;
	DWORD	mMasterIdx;
	char	mMasterName[MAX_NAME_LENGTH+1];
	DWORD	mKind;
	BYTE	mGrade;
	BYTE	mState;
	BYTE	mLevel;

	CPlayer* mpMaster;

	DWORD	mWearedItem[ ePetEquipSlot_Max ];
	
	friend class CAppearanceManager;

public:
	CPet(void);
	virtual ~CPet(void);

	virtual void InitPet( PET_OBJECT_INFO* pPetObjectInfo );
	virtual void Release();

	DWORD	GetKind()	{ return mKind; }
	BYTE	GetGrade()	{ return mGrade; }
	virtual void	LevelUp()	{ mLevel++; }
	void	GradeUp()	{ mGrade++; }

	void		SetMaster( DWORD master )	{ mMasterIdx = master; }
	CPlayer*	GetMaster();
	char*		GetMasterName()					{ return mMasterName; }

	virtual float DoGetMoveSpeed();
	virtual void SetMotionInState(BYTE State);

	virtual void SetLevel( BYTE level ) { mLevel = level; }
	BYTE	GetLevel() { return mLevel; }
	virtual void SetWearedItem( ITEMBASE* pItemInfo );
	virtual void SetWearedItem( WORD pos, DWORD idx ) { mWearedItem[ pos ] = idx; }

	virtual void	Die( CObject* pAttacker, BOOL bFatalDamage, BOOL bCritical, BOOL bDecisive );
	virtual void	Damage( CObject* pAttacker, BYTE DamageKind, DWORD Damage, BOOL bCritical, BOOL bDecisive );
	virtual void	Heal( CObject* pHealer, BYTE HealKind, DWORD HealVal );
	virtual void	Recharge( CObject* pHealer, BYTE RechargeKind, DWORD RechargeVal );
};

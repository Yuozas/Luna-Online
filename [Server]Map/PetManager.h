#pragma once

class CPlayer;
class CPet;

#define PETMGR USINGTON(CPetManager)

class CPetManager
{
	BYTE	mMaxGrade;
	BYTE	mMaxLevel;
	BYTE*	mGradeUpLevel;
	BYTE*	mSummonPlayerLevel;
	WORD*	mExp;
	BYTE	mExpPenaltyLevel;
	BYTE	mShopBaseSkillSlot;
	BYTE	mMinBaseSkillSlot;
	BYTE	mMaxBaseSkillSlot;
	BYTE*	mExtendedSkillSlot;
	WORD	mMaxDistance;
	WORD	mAreaDistance;
	DWORD	mDistanceCheckTime;
	DWORD	mHatchDelay;
	DWORD	mSummonDelay;
	DWORD	mSealDelay;
	DWORD	mSummonPenaltyDelay;
	DWORD	mResummonPenaltyDelay;
	DWORD	mClosedSkillIndex;
	DWORD	mRangeSkillIndex;
	DWORD	mFriendlyReduceTime;
	BYTE	mFriendlyReducePoint;

	CYHHashTable<PET_INFO>				mPetInfoTable;
	CYHHashTable<PET_OBJECT_INFO>		mPetObjectInfoTable;
	CYHHashTable<CPet>					mPetObjectTable;

	CYHHashTable<PET_STATUS_INFO>		mPetStatusInfoTable[ ePetType_Max ];
	CYHHashTable<PET_HP_MP_INFO>		mPetHPMPInfoTable;

	CYHHashTable<PET_FRIENDLY_PENALTY>	mPetFriendlyPenaltyTable;

	CMemoryPoolTempl<PET_OBJECT_INFO>	mPetObjectInfoPool;


public:
	CPetManager(void);
	virtual ~CPetManager(void);

	void LoadPetInfo();
	void UsePetItem( CPlayer* pPlayer, DWORD ItemDBIdx );

	PET_OBJECT_INFO*	NewPetObjectInfo();

	void NetworkMsgParse( BYTE Protocol, void* pMsg, DWORD dwLength );

	PET_INFO*			GetPetInfo( DWORD kind )					{ return mPetInfoTable.GetData( kind ); }
	PET_STATUS_INFO*	GetPetStatusInfo( BYTE type, BYTE level )	{ return mPetStatusInfoTable[ type ].GetData( level ); }

	void SealPet( CPet* pPet );
	void SummonPet( CPlayer* pPlayer, DWORD ItemDBIdx );

	void AddPetObject( CPet* pPet );		
	void RemovePetObject( CPet* pPet );

	BOOL SetTarget( CPet* pPet, DWORD id );
	BOOL PetAttack( CPet* pPet );
	BOOL PetSkillUse( CPet* pPet, DWORD idx );
	BOOL PetRecall( CPet* pPet );

	BOOL HatchPet( CPlayer* pPlayer, const ITEMBASE* pItem );

	WORD GetMaxEXP( BYTE grade );
	BYTE GetGradeUpLevel( BYTE grade );
	BYTE GetMaxGrade() { return mMaxGrade; }
	BYTE GetMaxLevel() { return mMaxLevel; }
	WORD GetAreaDistance() { return mAreaDistance; }

	PET_HP_MP_INFO*	GetHPMPInfo( BYTE idx ) { return mPetHPMPInfoTable.GetData( idx ); }

	DWORD GetFriendlyReduceTime() { return mFriendlyReduceTime; }
	BYTE GetFriendlyReducePoint() { return mFriendlyReducePoint; }

	float GetFriendlyPenalty( CPet* pPet );
	BYTE GetExtendedSkillSlot( BYTE grade ) { return mExtendedSkillSlot[ grade ]; }
	BYTE GetExpPenaltyLevel() { return mExpPenaltyLevel; }
};

EXTERNGLOBALTON(CPetManager);

#pragma once

#include "GameResourceStruct.h"

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
	WORD	mHatchEffect;
	WORD	mSummonEffect;
	WORD	mSealEffect;
	WORD	mAppearEffect;
	WORD	mDisappearEffect;
	WORD	mLevelUpEffect;
	WORD	mGradeUpEffect;
	DWORD	mHatchDelay;
	DWORD	mSummonDelay;
	DWORD	mSealDelay;
	DWORD	mSummonPenaltyDelay;
	DWORD	mResummonPenaltyDelay;
	DWORD	mClosedSkillIndex;
	DWORD	mRangeSkillIndex;
	WORD	mNameHight;
	
	BYTE	mRunAnimation;
	BYTE	mWalkAnimation;
	BYTE	mIdleAnimation;
	BYTE	mDamageAnimation;

	CYHHashTable<PET_INFO>				mPetInfoTable;
	CYHHashTable<PET_OBJECT_INFO>		mPetObjectInfoTable;

	CYHHashTable<PET_STATUS_INFO>		mPetStatusInfoTable[ ePetType_Max ];
	CYHHashTable<PET_HP_MP_INFO>		mPetHPMPInfoTable;
	CYHHashTable<PET_FRIENDLY_PENALTY>	mPetFriendlyPenaltyTable;
	CYHHashTable<cPtrList>				mPetSpeechInfoTable;
	CYHHashTable<cPtrList>				mPetEmoticonInfoTable;
	CYHHashTable<PET_FRIENDLY_STATE>	mFriendlyStateTable;

	DWORD	mLastSummonTime;

public:
	CPetManager(void);
	virtual ~CPetManager(void);
	
	void LoadPetInfo();

	void NetworkMsgParse(BYTE Protocol,void* pMsg);

	PET_INFO*			GetPetInfo( DWORD kind )					{ return mPetInfoTable.GetData( kind ); }
	PET_STATUS_INFO*	GetPetStatusInfo( BYTE type, BYTE level )	{ return mPetStatusInfoTable[ type ].GetData( level ); }

	DWORD				GetDistanceCheckTime()						{ return mDistanceCheckTime; }
	WORD				GetMaxDistance()							{ return mMaxDistance; }
	WORD				GetAreaDistance()							{ return mAreaDistance; }
	WORD				GetExp( BYTE grade )						{ return mExp[ grade ]; }
	BYTE				GetSummonPlayerLevel( BYTE grade )			{ return mSummonPlayerLevel[ grade ]; }


	BYTE	GetRunAnimation()		{ return mRunAnimation; }
	BYTE	GetWalkAnimation()		{ return mWalkAnimation; }
	BYTE	GetIdleAnimation()		{ return mIdleAnimation; }
	BYTE	GetDamageAnimation()	{ return mDamageAnimation; }

	DWORD	GetClosedSkillIndex()	{ return mClosedSkillIndex; }
	DWORD	GetRangeSkillIndex()	{ return mRangeSkillIndex; }
	
	PET_HP_MP_INFO*	GetHPMPInfo( BYTE idx ) { return mPetHPMPInfoTable.GetData( idx ); }

	BYTE	GetGradeUpLevel( BYTE grade ) { if( grade >= mMaxGrade ) return 0; return mGradeUpLevel[ grade ]; }
	float	GetFriendlyPenalty( BYTE friendy );
	PET_OBJECT_INFO*	GetPetObjectInfo( DWORD ItemDBIdx )			{ return mPetObjectInfoTable.GetData( ItemDBIdx ); }

	DWORD	GetLastSummonTime() { return mLastSummonTime; }
	void	SetLastSummonTime() { mLastSummonTime = gCurTime; }
	DWORD	GetSummonPenaltyDelay() { return mSummonPenaltyDelay; }

	WORD	GetSummonEffect() { return mSummonEffect; }
	WORD	GetSealEffect() { return mSealEffect; }
	WORD	GetLevelUpEffect() { return mLevelUpEffect; }
	WORD	GetGradeUpEffect() { return mGradeUpEffect; }
	WORD	GetNameHeight() { return mNameHight; }

	cPtrList*				GetPetSpeechInfoTable( DWORD index )	{ return mPetSpeechInfoTable.GetData( index ); }
	cPtrList*				GetPetEmoticonInfoTable( DWORD index )	{ return mPetEmoticonInfoTable.GetData( index ); }
	PET_FRIENDLY_STATE*		GetFriendlyStateTable( BYTE friendly )	{ return mFriendlyStateTable.GetData( friendly ); }
};

EXTERNGLOBALTON(CPetManager);

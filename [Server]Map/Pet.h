#pragma once
#include "object.h"

class CPlayer;
class Citem;
class CMonster;

class CPet :
	public CObject
{
	PET_OBJECT_INFO*	mpPetObjectInfo;
	CPlayer*			mpMaster;

	DWORD				mTargetID;

	PlayerStat			mPetStat;
	PlayerStat			mItemStat;

	Status				mPassiveStatus;
	Status				mRatePassiveStatus;

	friend class		CCharacterCalcManager;

	ITEMBASE*			mItemSlot;

	DWORD				mCheckTime;

	BOOL				mbBuffSkillLoad;

	RECOVER_TIME		m_ManaRecoverTime;
	YYRECOVER_TIME		m_YYManaRecoverTime;

	//---KES Aggro 070918
	CYHHashTable<CMonster>	m_htAggroedTable;	//나를 AggroList에 가지고 있는 몬스터 목록
	/// 자신에게 붙은 몬스터
	CYHHashTable<CMonster> m_FollowMonsterList;

protected:	
	virtual void DoSetInited()	{}
	virtual void DoDamage(CObject* pAttacker,RESULTINFO* pDamageInfo,DWORD beforeLife)	{}

public:
	CPet(void);
	virtual ~CPet(void);
	
	//virtual void SetState(BYTE state);

	void SetMaster( CPlayer* pPlayer ) { mpMaster = pPlayer; }
	void SetObjectInfo( PET_OBJECT_INFO* pInfo ) { mpPetObjectInfo = pInfo; }
	PET_OBJECT_INFO* GetObjectInfo() { return mpPetObjectInfo; }
	CPlayer* GetMaster() { return mpMaster; }
	
	virtual BOOL Init(EObjectKind kind,DWORD AgentNum, BASEOBJECT_INFO* pBaseObjectInfo);
	virtual void Release();

	virtual void SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin);
	virtual void SetRemoveMsg(char* pRemoveMsg,WORD* pMsgLen,DWORD dwReceiverID);
	virtual float GetRadius()	const {	return 0;	}
	virtual DWORD GetGravity();
	
	virtual void OnStartObjectState(BYTE State,DWORD dwParam)	{}
	virtual void OnEndObjectState(BYTE State) {}
	virtual void StateProcess();	

	virtual void DoDie(CObject* pAttacker);
	virtual DWORD Damage( CObject* pAttacker, RESULTINFO* pDamageInfo );
	//virtual DWORD Damage(CObject* pAttacker,RESULTINFO* pDamageInfo);	
	
	virtual LEVELTYPE GetLevel()	{	return mpPetObjectInfo->Level;	};
	virtual DWORD GetLife() {	return mpPetObjectInfo->HP;	}
	virtual void SetLife(DWORD Life,BOOL bSendMsg = TRUE);	
	virtual DWORD GetMana(){ return mpPetObjectInfo->MP; }
	virtual void SetMana(DWORD val,BOOL bSendMsg = TRUE);	

//---KES Aggro 070918
	void	AddToAggroed( CMonster* pMonster );		//void CMonster::CalcAggro() 함수에서만 호출한다. 다른곳에서 호출 금지!
	void	RemoveFromAggroed( DWORD dwObjectID );	//void CMonster::RemoveFromAggro(), RemoveAllAggro() 함수에서만 호출한다. 다른곳에서 호출 금지!
	void	RemoveAllAggroed();

	void	AddAggroToMyMonsters( int nAggroAdd, CObject* pTarget );
//-------------------
	BOOL	AddFollowList( CMonster * pMob );
	void	RemoveFollowList( DWORD ID );
	BOOL	RemoveFollowAsFarAs( DWORD GAmount, CObject* pObject );

	virtual DWORD DoGetMaxLife()	{return mpPetObjectInfo->MAXHP;}
	virtual DWORD DoGetMaxMana()	{return mpPetObjectInfo->MAXMP;}
	virtual DWORD DoGetPhyDefense()	{return (DWORD)mPetStat.mPhysicDefense.mPlus;}
	virtual DWORD DoGetPhyAttackPowerMin()	{return (DWORD)mPetStat.mPhysicAttack.mPlus;}
	virtual DWORD DoGetPhyAttackPowerMax()	{return (DWORD)mPetStat.mPhysicAttack.mPlus;}
	virtual DWORD DoGetMagicAttackPower()	{return (DWORD)mPetStat.mMagicAttack.mPlus;}
	virtual DWORD DoGetMagicDefense()	{return (DWORD)mPetStat.mMagicAttack.mPlus;}
	virtual DWORD DoGetShieldDefense()	{return 0;}
	virtual DWORD DoGetCritical()	{return (DWORD)mPetStat.mCriticalRate.mPlus;}
	virtual DWORD DoGetAccuracy()	{return (DWORD)mPetStat.mAccuracy.mPlus;}
	virtual DWORD DoGetAvoid()	{return (DWORD)mPetStat.mEvade.mPlus;}

	virtual float DoGetMoveSpeed();

	void	SetMaxLife( DWORD val );
	void	SetMaxMana( DWORD val );

	Status*	GetPassiveStatus() { return &mPassiveStatus; }
	Status*	GetRatePassiveStatus() { return &mRatePassiveStatus; }

	DWORD GetItemDBIdx()	{ return mpPetObjectInfo->ItemDBIdx; }

	void SetTarget( DWORD id ) { mTargetID = id; }
	DWORD GetTarget() { return mTargetID; }

	void AddExp();
	void LevelUp();
	void GradeUp();
	void SetType( BYTE type );
	void FriendlyUp( BYTE val );
	void SetLevel( BYTE level );

	ITEMBASE* GetWearedItem( POSTYPE pos );
	void SetWearedItem( POSTYPE pos, ITEMBASE* pitem );
	void SetAllWearedItem( ITEMBASE* info );

	void CalcStats();

	PlayerStat*	GetStat() { return &mPetStat; }

	void SetPetObjectState( BYTE state ) { mpPetObjectInfo->State = state; }
	void FriendlyProcess();

	void DBInfoUpdate();
	void MasterInfoUpdate();
};

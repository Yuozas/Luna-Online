// Object.h: interface for the CObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECT_H__A8AFB488_5BB5_45E5_8482_EE4EE2A55DDD__INCLUDED_)
#define AFX_OBJECT_H__A8AFB488_5BB5_45E5_8482_EE4EE2A55DDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GridUnit.h"
#include "../[cc]skill/server/Status/Status.h"

class CBattle;
class cBuffSkillInfo;
// 080616 LUJ, 이벤트 발생 시 버프 중인 스킬이 액션을 취하기 위해 선언
class CEvent;
class cSkillObject;

enum EObjectKind
{
	// 080616 LUJ, 초기화용 값 추가
	eObjectKind_None,
	eObjectKind_Player = 1,
	eObjectKind_Npc		=	2,
	eObjectKind_Item	=	4,
	eObjectKind_SkillObject=16,
	eObjectKind_Monster	=	32,
	eObjectKind_BossMonster=33,
	eObjectKind_SpecialMonster=34,
	
	// 필드보스 - 05.12 이영준
	eObjectKind_FieldBossMonster=35,
	eObjectKind_FieldSubMonster=36,
	eObjectKind_ToghterPlayMonster=37,
	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.23
	eObjectKind_ChallengeZoneMonster = 38,
	// E 데이트 존 추가 added by hseos 2007.11.23
	// 080616 LUJ, 함정 추가
	eObjectKind_Trap = 39,

	eObjectKind_MapObject	= 64,
	eObjectKind_CastleGate	= 65,
	eObjectKind_Pet			= 128,
};

class CAction;
class CSpecialState;
class cDelay;

class CObject : public CGridUnit
{
	TILEINDEX m_dwPreoccupiedTileIndex;		// 선점하고 있는 타일의 인덱스

	BOOL m_bInited;
	friend class CObjectFactory;
	friend class CCharMove;
	friend class CObjectStateManager;

	
protected:
	CObject();
	virtual ~CObject();

	DWORD m_AgentNum;
	BYTE m_ObjectKind;

	BASEOBJECT_INFO m_BaseObjectInfo;
	MOVE_INFO m_MoveInfo;
	STATE_INFO	m_ObjectState;

	RECOVER_TIME m_LifeRecoverTime;
	YYRECOVER_TIME m_YYLifeRecoverTime;

	AbnormalStatus mAbnormalStatus;
	
	DWORD m_KyungGongMana_LastCheckTime;
	
	CBattle* m_pBattle;

	virtual void DoSetInited()	{}
	virtual void DoDamage(CObject* pAttacker,RESULTINFO* pDamageInfo,DWORD beforeLife)	{}

	friend class CCharacterCalcManager;

	/// 스킬 오브젝트 리스트
//	CYHHashTable<cSkillObject>	m_OnOffSkillList;
	CYHHashTable<cSkillObject>	m_BuffSkillList;

	Status				mBuffStatus;
	Status				mRateBuffStatus;
	cPtrList			mActiveStatusList;
	cPtrList			mInertStatusList;

	DWORD				mCurrentSkillID;

	WORD				mEffectMoveCount;


public:
	virtual void SetState(BYTE state);

	BYTE GetBattleTeam()						{	return m_BaseObjectInfo.BattleTeam;	}
	void SetBattleTeam( DWORD BattleTeam )		{	m_BaseObjectInfo.BattleTeam = (BYTE)BattleTeam;	}
	DWORD GetBattleID()				{	return m_BaseObjectInfo.BattleID;	}
	void SetBattleID(DWORD BattleID){ m_BaseObjectInfo.BattleID = BattleID;	}
	CBattle* GetBattle()			{	return m_pBattle;	}
	void SetBattle( DWORD BattleID, BYTE Team, DWORD Param=0 );
	
	virtual void SendMsg(MSGBASE* pMsg,int MsgLen);

	inline DWORD GetAgentNum()	
	{
		return m_AgentNum;	
	}

	virtual BOOL Init(EObjectKind kind,DWORD AgentNum, BASEOBJECT_INFO* pBaseObjectInfo);
	virtual void Release();

	inline BYTE GetObjectKind()						{	return m_ObjectKind;	}
	inline void SetObjectKind(EObjectKind kind)		{	m_ObjectKind = kind;	}

	inline char* GetObjectName()					{return m_BaseObjectInfo.ObjectName;	}
	
	inline BYTE GetState()					{return m_BaseObjectInfo.ObjectState;	}

	void GetPosition(VECTOR3* pos);
	BASEMOVE_INFO * GetMoveInfo() { return &m_MoveInfo; }
	//VECTOR3 * GetCurTargetPosition();

	void SetInited();
	void SetNotInited();
	BOOL GetInited()	const {	return m_bInited;	}


	inline DWORD GetID()		const	{	return m_BaseObjectInfo.dwObjectID;	}
	inline DWORD GetUserID()	const	{	return m_BaseObjectInfo.dwUserID;	}

	void GetBaseObjectInfo(BASEOBJECT_INFO* pRtInfo);
	virtual void GetSendMoveInfo(SEND_MOVEINFO* pRtInfo,CAddableInfoList* pAddInfoList);

	virtual void SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin) = 0;
	virtual void SetRemoveMsg(char* pRemoveMsg,WORD* pMsgLen,DWORD dwReceiverID);

	virtual float GetRadius()	const {	return 0;	}
	
	//////////////////////////////////////////////////////////////////////////
	// 상태(State) 관련
	virtual void OnStartObjectState(BYTE State,DWORD dwParam)	{}
	virtual void OnEndObjectState(BYTE State) {}
	virtual void StateProcess()		{};	

	//////////////////////////////////////////////////////////////////////////
	// 타일 관련
	TILEINDEX GetPreoccupiedTileIndex()		{	return m_dwPreoccupiedTileIndex;	}
	void SetPreoccupiedTileIndex(TILEINDEX tileindex)		{	m_dwPreoccupiedTileIndex = tileindex;	}
	virtual int GetObjectTileSize();
	
	//////////////////////////////////////////////////////////////////////////
	// 데미지 죽음 관련
	void Die(CObject* pAttacker);
	virtual void DoDie(CObject* pAttacker)	{}
	virtual void CalcRealDamage(CObject* pOperator,WORD PhyDamage,WORD AttrDamage,RESULTINFO* pDamageInfo);
	virtual void CalcRealAttack(CObject* pTarget, WORD PhyDamage, WORD AttrDamage, RESULTINFO* pDamageInfo, BOOL bIsContinueAttack = FALSE );

	// 06. 03 국내무쌍 - 이영준
	// 국내무쌍관련 호신 데미지 함수 변경
	virtual void CalcCounterDamage(WORD Attrib,WORD AttrDamage,WORD PhyDamage,RESULTINFO* pDamageInfo);	
	virtual void CalcVampiric(RESULTINFO* pDamageInfo);
	virtual void CalcReverseVampiric(RESULTINFO* pDamageInfo);
	virtual DWORD Damage(CObject* pAttacker,RESULTINFO* pDamageInfo);	
	
	//////////////////////////////////////////////////////////////////////////
	// Status
//	virtual void AddStatus( eStatusKind kind, StatusData* pData, Status_Kind flag );
//	virtual void RemoveStatus( eStatusKind kind, StatusData* pData, Status_Kind flag );
//	virtual cStatus* GetStatus( eStatusKind kind );

	//////////////////////////////////////////////////////////////////////////
	// 정보 얻어오는 함수들
	virtual LEVELTYPE GetLevel()	{	return 0;	};
	virtual void SetLevel(LEVELTYPE level)	{};	
	virtual DWORD GetLife() {	return 0;	}
	virtual void SetLife(DWORD Life,BOOL bSendMsg = TRUE)	{};	
	virtual DWORD GetMana(){ return 0; }
	virtual void SetMana(DWORD val,BOOL bSendMsg = TRUE)	{};	

	virtual DWORD GetRecoverLife();
	virtual DWORD GetRecoverMana();

//////////////////////////////////////////////////////////////////////////
// 
	DWORD GetMaxLife() { return DoGetMaxLife(); }
	DWORD GetMaxMana() { return DoGetMaxMana(); }
	DWORD GetPhyDefense() { return DoGetPhyDefense(); }
	DWORD GetPhyAttackPowerMin() { return DoGetPhyAttackPowerMin(); }
	DWORD GetPhyAttackPowerMax() { return DoGetPhyAttackPowerMax(); }
	DWORD GetMagicAttackPower() { return DoGetMagicAttackPower(); }
	DWORD GetMagicDefense() { return DoGetMagicDefense(); }
	DWORD GetShieldDefense() { return DoGetShieldDefense(); }
	DWORD GetCritical() { return DoGetCritical(); }
	DWORD GetAccuracy() { return DoGetAccuracy(); }
	DWORD GetAvoid() { return DoGetAvoid(); }

	virtual DWORD DoGetMaxLife()	{return 0;}
	virtual DWORD DoGetMaxMana()	{return 0;}
	virtual DWORD DoGetPhyDefense()	{return 0;}
	virtual DWORD DoGetPhyAttackPowerMin()	{return 0;}
	virtual DWORD DoGetPhyAttackPowerMax()	{return 0;}
	virtual DWORD DoGetMagicAttackPower()	{return 0;}
	virtual DWORD DoGetMagicDefense()	{return 0;}
	virtual DWORD DoGetShieldDefense()	{return 0;}
	virtual DWORD DoGetCritical()	{return 0;}
	virtual DWORD DoGetAccuracy()	{return 0;}
	virtual DWORD DoGetAvoid()	{return 0;}


	float GetMoveSpeed() { return DoGetMoveSpeed(); }
	float GetUngiPlusRate() { return DoGetUngiPlusRate(); }
	float GetAddAttackRange() { return DoGetAddAttackRange(); }
	float GetDodgeRate() { return DoGetDodgeRate(); }
	
	virtual float DoGetMoveSpeed()	{return 0;}
	virtual float DoGetUngiPlusRate()	{return 0;}
	virtual float DoGetAddAttackRange()	{return 0;}
	virtual float DoGetDodgeRate()	{return 0;}

	float GetAttDefense(WORD Attrib);
	virtual float DoGetAttDefense(WORD Attrib)	{return 0;}
//////////////////////////////////////////////////////////////////////////

	virtual void SetMaxLife(WORD life)	{};	
	virtual void SetMaxMana(WORD val)	{};	
	virtual float GetAttribPlusPercent(WORD Attrib) { return 0;	}	
	
	void AddLife(DWORD val,DWORD* realAddLife,BOOL bSendMsg = TRUE);
	// 080710 LUJ, 스킬 사용 시 HP를 소모할 수 있도록 추가
	void ReduceLife(DWORD val);
	void AddMana(DWORD val,DWORD* realAddMana);
	void ReduceMana(DWORD val);
	
	void CheckLifeValid();
	void CheckManaValid();

	virtual DWORD GetGravity(){ return 0;	}
	virtual void SetGravity(DWORD gravity){ }
	//////////////////////////////////////////////////////////////////////////	

	virtual int GetSkillLevel( DWORD SkillIdx ) { return 0; }

//	CYHHashTable<cSkillObject>* GetOnOffSkillList() { return &m_OnOffSkillList; }
	CYHHashTable<cSkillObject>* GetBuffSkillList() { return &m_BuffSkillList; }

	AbnormalStatus*	GetAbnormalStatus() { return &mAbnormalStatus; }

	Status*	GetBuffStatus() { return &mBuffStatus; }
	Status*	GetRateBuffStatus() { return &mRateBuffStatus; }

	void	AddSpecialSkill( cBuffSkillInfo* pInfo ) { mInertStatusList.AddTail( (void*)pInfo ); }
	void	RemoveSpecialSkill( cBuffSkillInfo* pInfo ) { mInertStatusList.Remove( (void*)pInfo ); }
	void	ProcessSpecialSkill();
	void	ProcessAbnormalStatus();

	void	EndSkill();
	void	EndAllSkill();


	void	SetCurrentSkill( DWORD idx ) { mCurrentSkillID = idx; }
	DWORD	GetCurrentSkill() { return mCurrentSkillID; }

	void	EndBuffSkillByStatus( WORD Status );

	virtual	void SetObjectBattleState( BYTE state ) { m_BaseObjectInfo.ObjectBattleState = state; }
	BYTE	GetObjectBattleState() { return m_BaseObjectInfo.ObjectBattleState; }

	void	SetEffectMove()
	{
		mEffectMoveCount++;
	}

	BOOL	IsEffectMove()
	{
		if( mEffectMoveCount == 0 )
			return FALSE;

		mEffectMoveCount--;

		return TRUE;
	}

	DWORD	CurCastingSkillID;

	// 080616 LUJ, 오브젝트에게 이벤트가 발생 시마다, 버프 중인 스킬로 하여금 액션을 취하게 한다
public:
	// 080616 LUJ, 이벤트 처리
	virtual void Execute( const CEvent& );
	// 080616 LUJ, 이벤트 시 처리할 버프 스킬을 등록한다.
	void AddEventSkill( cSkillObject& );

private:
	typedef std::list< DWORD >	SkillObjectIndexList;
	SkillObjectIndexList		mSkillObjectIndexList;
};

#endif // !defined(AFX_OBJECT_H__A8AFB488_5BB5_45E5_8482_EE4EE2A55DDD__INCLUDED_)


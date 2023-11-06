// Object.h: interface for the CObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECT_H__A8AFB488_5BB5_45E5_8482_EE4EE2A55DDD__INCLUDED_)
#define AFX_OBJECT_H__A8AFB488_5BB5_45E5_8482_EE4EE2A55DDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Engine/EngineObject.h"
#include "./Engine/EngineEffect.h"
#include "Angle.h"
#include "EFFECT/EffectManager.h"
#include "../[cc]skill/client/delay/timedelay.h"

class CMOTIONDESC;
class cSkillObject;
class CObjectBalloon;
class cBuffSkillInfo;

enum EObjectKind
{
	// 080616 LUJ, 초기화용 값 추가
	eObjectKind_None,
	eObjectKind_Player = 1,
	eObjectKind_Npc		=	2,
	eObjectKind_Item	=	4,
	eObjectKind_Tactic	=	8,
	eObjectKind_SkillObject=16,
	eObjectKind_Monster	=	32,
	eObjectKind_BossMonster=33,
	eObjectKind_SpecialMonster=34,
	// 필드보스 - 05.12 이영준
	eObjectKind_FieldBossMonster=35,
	eObjectKind_FieldSubMonster=36,
	eObjectKind_ToghterPlayMonster=37,
	// 080616 LUJ, 함정 추가
	eObjectKind_Trap = 39,
	eObjectKind_Pet = 64,
	eObjectKind_MapObject = 128,
	eObjectKind_CastleGate = 129,
	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.05.07
	eObjectKind_FarmObj= 130,
	// E 농장시스템 추가 added by hseos 2007.05.07
};

struct ROTATEINFO
{
	BOOL bRotating;
	CAngle Angle;
	DWORD Rotate_StartTime;
	float EstimatedRotateTime;
};

#define NAMECOLOR_DEFAULT	RGB_HALF( 240, 240, 240 ) //KES 색수정 조금 밝게  //RGB_HALF( 230, 230, 230 )
#define NAMECOLOR_SELECTED	RGB_HALF( 255, 255, 0 )
#define NAMECOLOR_MONSTER	NAMECOLOR_DEFAULT
#define NAMECOLOR_PLAYER	NAMECOLOR_DEFAULT
#define NAMECOLOR_NPC		RGB_HALF( 255, 214, 0 )

#define NAMECOLOR_PARTY		RGB_HALF( 157, 204, 58 )
#define NAMECOLOR_MUNPA		RGB_HALF( 126, 156, 180 )

#define NAMECOLOR_PKMODE	RGB_HALF( 255, 0, 0 )
#define NAMECOLOR_WANTED	RGB_HALF( 234, 0, 255 )

#define NAMECOLOR_GM		RGB_HALF( 28, 233, 151 )


struct OBJECTEFFECTDESC
{
	OBJECTEFFECTDESC()		{	SetDesc(0);	}
	OBJECTEFFECTDESC(WORD EffectNum,DWORD dwFlag = EFFECT_FLAG_NORMAL,VECTOR3* pPos = 0)
	{	SetDesc(EffectNum,dwFlag,pPos);	}
	WORD Effect;
	DWORD Flag;
	VECTOR3 Position;
	void SetDesc(WORD EffectNum,DWORD dwFlag = EFFECT_FLAG_NORMAL,VECTOR3* pPos = 0)
	{
		Effect = EffectNum;
		Flag = dwFlag;
		if(pPos)
			Position = *pPos;
		else
			Position.x = Position.y = Position.z = 0;
	}
};


class CObject : public CObjectBase
{
	int m_bMoveSkipCount;
//	HEFFPROC m_ShadowEff;	//protected로 옮김 KES
	CYHHashTable<void> m_StateEffectList;
	BOOL m_bIsYMoving;

	float	mAddedMoveSpeed;

protected:
	CObject();
	virtual ~CObject();

	DWORD m_DiedTime;
	CEngineEffect m_ShadowObj;
	BOOL m_bInited;
	BOOL m_bSelected;

	BYTE m_ObjectKind;

	BASEOBJECT_INFO m_BaseObjectInfo;
	MOVE_INFO m_MoveInfo;
	ROTATEINFO m_RotateInfo;

	cTimeDelay	mBattleStateCheckDelay;
	
	STATE_INFO	m_ObjectState;

	CMOTIONDESC	* m_pMotionDESC;			// 모션 정보  : taiyo
	CEngineObject m_EngineObject;
	
	BOOL m_bDieFlag;
	
	// overInfo 관련 --------------------------------------------------------------
	CObjectBalloon * m_pObjectBalloon;
	BYTE m_bObjectBalloonBits;

//	void OnMouseLeave();
//	void OnMouseOver();
	// overInfo 관련 --------------------------------------------------------------

	cPtrList m_StatusList;

	friend class CAppearanceManager;
	friend class CMoveManager;
	friend class CMHCamera;
	friend class CObjectStateManager;
	friend class CObjectActionManager;
	friend class CMotionManager;


	virtual BOOL Init(EObjectKind kind,BASEOBJECT_INFO* pBaseObjectInfo);
	void InitMove(BASEMOVE_INFO* pMoveInfo);
	virtual void Release();
	int	GetMotionIDX(int mainMotion, int subMotion = 0);
public:
	// overInfo 관련 --------------------------------------------------------------
	void InitObjectBalloon(BYTE bitFlag);
//	void AddChatBalloon(char * chatMsg);

	// 070121 LYW --- Add function to return balloon.
	CObjectBalloon* GetObjectBalloon(){ return m_pObjectBalloon ; }
	void SetOverInfoOption( DWORD dwOption );
	void SetGuildMark();
	void SetNickName();
	void SetFamilyMark();
	void SetFamilyNickName();


	void ShowObjectName( BOOL bShow, DWORD dwColor = NAMECOLOR_DEFAULT );
	void ShowChatBalloon( BOOL bShow, char* chatMsg, DWORD dwColor = RGB_HALF(70,70,70), DWORD dwAliveTime = 5000 );
	// overInfo 관련 --------------------------------------------------------------

	//SW050913 수정
	void AddObjectEffect(DWORD KeyValue,OBJECTEFFECTDESC* EffectNumArray,WORD NumOfEffect,CObject* pSkillOperator=NULL);
	void RemoveObjectEffect(DWORD KeyValue);
	void RemoveAllObjectEffect();
	
//	virtual void InitObjectOverInfoEx();
	BYTE GetBattleTeam()						{	return m_BaseObjectInfo.BattleTeam;	}
	void SetBattleTeam( DWORD BattleTeam )		{	m_BaseObjectInfo.BattleTeam = (BYTE)BattleTeam;	}
	DWORD GetBattleID()				{	return m_BaseObjectInfo.BattleID;	}
	void SetBattle(DWORD BattleID,BYTE Team);


	inline BYTE GetObjectKind()						{	return m_ObjectKind;	}
	inline void SetObjectKind(EObjectKind kind)		{	m_ObjectKind = kind;	}
	
	inline char* GetObjectName()	{	return m_BaseObjectInfo.ObjectName;		}

	inline DWORD GetID()	{	return m_BaseObjectInfo.dwObjectID;	}


	void GetBaseObjectInfo(BASEOBJECT_INFO* pRtInfo);
	void GetBaseMoveInfo(BASEMOVE_INFO* pRtInfo);
	MOVE_INFO* GetBaseMoveInfo() {	return &m_MoveInfo;	}

	virtual void Process();
	

	inline CEngineObject* GetEngineObject()	{	return &m_EngineObject;		}


	friend class CObjectManager;

	inline BOOL IsInited()		{	return m_bInited;	}
	VECTOR3& GetCurPosition() { return m_MoveInfo.CurPosition; }
	float GetAngleDeg()		 { return m_RotateInfo.Angle.ToDeg();	}	

	inline BOOL IsDied()			{	return m_bDieFlag;	}
	inline DWORD GetDiedTime()			{	return m_DiedTime;	}
	void SetDieFlag();
	void SetFlag(BOOL val);

//protected:
	// 상태 함수들
	virtual void SetMotionInState(BYTE State)	{};

	virtual BOOL StartSocietyAct( int nStartMotion, int nIngMotion = -1, int nEndMotion = -1, BOOL bHideWeapon = FALSE )	{ return FALSE; }
	virtual BOOL EndSocietyAct()	{ return TRUE; }

protected:	
	void SetState(BYTE State);
public:

	BYTE GetState();
	DWORD GetStateStartTime()	{	return m_ObjectState.State_Start_Time;		}

	// 행동 함수들...
	virtual void Die(CObject* pAttacker,BOOL bFatalDamage,BOOL bCritical, BOOL bDecisive);
	virtual void Damage(CObject* pAttacker,BYTE DamageKind,DWORD Damage,BOOL bCritical, BOOL bDecisive) {}
	virtual void Heal(CObject* pHealer,BYTE HealKind,DWORD HealVal) {}
	virtual void Recharge(CObject* pHealer,BYTE RechargeKind,DWORD RechargeVal) {}	
	virtual void Revive(VECTOR3* pRevivePos);

	//////////////////////////////////////////////////////////////////////////
	// ObjectStateManager에서 State의 시작과 끝에서 호출해주는 함수들
	virtual void OnStartObjectState(BYTE State)	{}
	virtual void OnEndObjectState(BYTE State)	{}
	//////////////////////////////////////////////////////////////////////////

	// 수치 
	virtual void SetLife(DWORD life, BYTE type = 1){}
	virtual DWORD GetLife(){ return 0; }
	virtual float GetWeight()		{	return 1.f;	}
	virtual float GetRadius()		{	return 0.f;	}
	virtual DWORD GetMana()		{	return 0; }
	virtual void SetMana(DWORD val, BYTE type = 1)		{}

	
//////////////////////////////////////////////////////////////////////////
// 
#define GET_STATUS(var_type,func)						\
var_type	func ## ()									\
{														\
	var_type Ori = Do ## func();						\
	return Ori;											\
};														\
virtual var_type	Do ## func ## ()	{	return 0; }		
//	
	GET_STATUS(DWORD,GetMaxLife);
	GET_STATUS(DWORD,GetMaxMana);
	GET_STATUS(DWORD,GetPhyDefense);
	GET_STATUS(DWORD,GetPhyAttackPowerMin);
	GET_STATUS(DWORD,GetPhyAttackPowerMax);
	GET_STATUS(DWORD,GetCritical);
	GET_STATUS(DWORD,GetDecisive);
	GET_STATUS(float,GetMoveSpeed);
	GET_STATUS(float,GetAddAttackRange);

	// 070411 LYW --- Object : Add physical part.
	GET_STATUS(DWORD,GetAttackRate);
	GET_STATUS(DWORD,GetDefenseRate);
	GET_STATUS(DWORD,GetMagAttackRate);
	GET_STATUS(DWORD,GetMagDefenseRate); 
	GET_STATUS(DWORD,GetAccuracyRate);
	GET_STATUS(DWORD,GetEvasionRate);
	GET_STATUS(DWORD,GetCriticalRate);		
	

	float GetAttDefense(WORD Attrib);	
	virtual float DoGetAttDefense(WORD Attrib)	{return 0;}
//////////////////////////////////////////////////////////////////////////
	
	virtual void SetPosition(VECTOR3* pPos);
	virtual void GetPosition(VECTOR3* pPos);
	virtual void SetAngle(float AngleRad);
	virtual float GetAngle();
	virtual DIRINDEX GetDirectionIndex();	


	virtual void ChangeMotion(int motion,BOOL bRoop, float rate = 1.0f);
	virtual void ChangeBaseMotion(int motion);

	// 070222 LYW --- Object : Add function to change emotion of hero.
	void ChangeEmotion( int emotion ) ;

	//////////////////////////////////////////////////////////////////////////
	// skillManager에서 쓰는 함수들
//	virtual void AddStatus(CStatus* pStatus);
//	virtual void RemoveStatus(CStatus* pStatus);


	//////////////////////////////////////////////////////////////////////////
	// ObjectManager에서 쓰는 함수들
	void OnSelected();
	void OnDeselected();

	
	// Quest Npc Mark
	BOOL	IsNpcMark( DWORD dwValue );
	
	void	SetKyungGongLevel( WORD wLevel );
	void	SetAddedMoveSpeed( float speed ) { mAddedMoveSpeed = speed; }

	void	SetObjectBattleState( BYTE state );
	BYTE	GetObjectBattleState() { return m_BaseObjectInfo.ObjectBattleState; }

};

#endif // !defined(AFX_OBJECT_H__A8AFB488_5BB5_45E5_8482_EE4EE2A55DDD__INCLUDED_)

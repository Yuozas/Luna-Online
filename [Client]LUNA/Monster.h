#pragma once


#include "Object.h"


class BASE_MONSTER_LIST;


class CMonster : public CObject  
{
	MONSTER_TOTALINFO m_DummyMonsterInfo;
	MONSTER_TOTALINFO m_MonsterInfo;

	BASE_MONSTER_LIST * m_pSInfo;

	void InitMonster(MONSTER_TOTALINFO* pTotalInfo);
public:
	void CheckTmpFunc();
	CMonster();
	virtual ~CMonster();
	
	BASE_MONSTER_LIST * GetSInfo() { /*CheckTmpFunc();*/ return m_pSInfo; }
	virtual void SetLife(DWORD val, BYTE type = 1);
	virtual DWORD GetLife(){ /*CheckTmpFunc();*/ return m_MonsterInfo.Life;	}
	virtual float DoGetMoveSpeed();

	virtual void Process();

	friend class CObjectManager;

	WORD GetMonsterKind()		{ /*CheckTmpFunc();*/ return m_MonsterInfo.MonsterKind;	}
	
	virtual void SetMotionInState(BYTE State);
	
	void Attack(int AttackNum,CObject* pTarget);
	
	// 행동 함수들...
	virtual void Die(CObject* pAttacker,BOOL bFatalDamage,BOOL bCritical, BOOL bDecisive);
	virtual void Damage(CObject* pAttacker,BYTE DamageKind,DWORD Damage,BOOL bCritical, BOOL bDecisive);
	virtual void Heal(CObject* pHealer,BYTE HealKind,DWORD HealVal);

	float GetWeight();
	float GetRadius();

	// 080616 LUJ, 일정 시간 후 함정이 자동으로 제거되도록 한다
	DWORD mLastCheckedTick;
};
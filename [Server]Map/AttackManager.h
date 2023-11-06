// AttackManager.h: interface for the CAttackManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATTACKMANAGER_H__90EBFECC_751A_4C43_AF61_AA0C5F5BAD34__INCLUDED_)
#define AFX_ATTACKMANAGER_H__90EBFECC_751A_4C43_AF61_AA0C5F5BAD34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AttackCalc.h"
#include "Object.h"


enum eATTACKABS_KIND
{
	eAAK_LIFE = 1,
	eAAK_MANA = 4,
};



#define  ATTACKMGR CAttackManager::GetInstance()
class CAttackManager  
{
	CAttackCalc m_ATTACKCALC;

	WORD GetComboPhyDamage(CObject* pAttacker,CObject* pTargetObject,float PhyAttackRate,float fCriticalRate,
		RESULTINFO* pDamageInfo,DWORD AmplifiedPower,float fDecreaseDamageRate );
	WORD GetSkillPhyDamage(CObject* pAttacker,CObject* pTargetObject,float PhyAttackRate,float fCriticalRate,
		RESULTINFO* pDamageInfo,DWORD AmplifiedPower,float fDecreaseDamageRate );
	WORD GetSkillAttrDamage(CObject* pAttacker,CObject* pTargetObject,
		WORD Attrib,DWORD AttAttackMin,DWORD AttAttackMax,float AttAttackRate,float fCriticalRate,RESULTINFO* pDamageInfo,float fDecreaseDamageRate );

public:
	GETINSTANCE(CAttackManager);
	CAttackManager();
	virtual ~CAttackManager();

	// 최종 데미지를 넘겨준다. 다만 죽었을 경우엔 DIE 메세지를 뿌려준다.
	void Attack(BOOL bSkill, CObject* pAttacker,CObject* pTarget,DWORD AmplifiedPower,
				float PhyAttackRate,
				WORD Attrib,DWORD AttAttackMin,DWORD AttAttackMax,float AttAttackRate,
				float fCriticalRate,
				RESULTINFO* pDamageInfo,BOOL bCounter, float fDecreaseDamageRate,
				WORD AmplifiedPowerAttrib, BOOL bContinueAttack = FALSE );
	
	
	void RecoverLife(CObject* pOperator,CObject* pObject,WORD m_Attrib,float RecoverLifeVal,RESULTINFO* pResultInfo);
	void RecoverMana(CObject* pOperator,CObject* pObject,WORD m_Attrib,float RecoverManaVal,RESULTINFO* pResultInfo);
	
	void sendDieMsg(CObject * pAttacker, CObject* pTarget);
	
	inline CAttackCalc& GetAttackCalc()		{	return m_ATTACKCALC;	}

	void AttackAbs( CObject* pAttacker, CObject* pTarget, int nAbsKind, float AttackRate, RESULTINFO* pDamageInfo );
	
	void AttackJinbub( CObject* pAttacker, CObject* pTarget, DWORD AttackPower, 
					   DWORD AttackMin,DWORD AttackMax, RESULTINFO* pDamageInfo,
					   float fDecreaseDamageRate );
	
	DWORD GetJinbubDamage(CObject* pAttacker,CObject* pTargetObject,DWORD AttackPower,
			RESULTINFO* pDamageInfo,float fDecreaseDamageRate);

	// 070418 LYW --- AttackManager : Add funtion to setting exp and level when the character died.
	//void DownExpLevel( CObject* pPlayer, CObject* pMonster ) ;

	void PhysicAttack( CObject* pAttacker, CObject* pTarget, RESULTINFO* pDamageInfo, float Accuracy = 0, float AddDamage = 0, WORD AddType = 0 );
	void MagicAttack( CObject* pAttacker, CObject* pTarget, RESULTINFO* pDamageInfo, float Accuracy = 0, float AddDamage = 0, WORD AddType = 0 );
	void SpecialAttack( CObject* pAttacker, CObject* pTarget, RESULTINFO* pDamageInfo, float Damage = 0, WORD Type = 0 );

	void SetPvpDamageRatio(BYTE ratio)	{	m_PvpDamageRatio = ratio; }
	float GetPvpDamageRatio()	{ return m_PvpDamageRatio; }
private:
	// 080115 KTH -- PVP Damage Ratio
	float	m_PvpDamageRatio;
};

#endif // !defined(AFX_ATTACKMANAGER_H__90EBFECC_751A_4C43_AF61_AA0C5F5BAD34__INCLUDED_)

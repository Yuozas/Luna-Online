// AttackCalc.h: interface for the CAttackCalc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATTACKCALC_H__EB8D1663_38B7_43EA_A9AA_07FB8ACC1B01__INCLUDED_)
#define AFX_ATTACKCALC_H__EB8D1663_38B7_43EA_A9AA_07FB8ACC1B01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object.h"
#include "Player.h"
#include "Monster.h"
#include "..\[CC]Header\GameResourceManager.h"

class CAttackCalc  
{
	
	// New Calc Function 
	// 2003-05-12 taiyo
	double	getPlayerPhysicalAttackPower(CPlayer * pPlayer, float PhyAttackRate, BOOL bCritical );
	double	getPlayerAttributeAttackPower(CPlayer * pPlayer, WORD Attrib, DWORD AttAttackMin,DWORD AttAttackMax,float AttAttackRatefloat );

	double	getMonsterPhysicalAttackPower(CMonster * pMonster, float PhyAttackRate, BOOL bCritical);
	double	getMonsterAttributeAttackPower(CMonster * pMonster, WORD Attrib, DWORD AttAttackMin,DWORD AttAttackMax);

	
public:
	CAttackCalc();
	virtual ~CAttackCalc();

	// 080602 LYW --- Player : 경험치 수치 (__int32) 에서 (__int64) 사이즈로 변경 처리.
	//DWORD GetPlayerPoint(LEVELTYPE level1, int leve_gab);
	EXPTYPE GetPlayerPoint(LEVELTYPE level1, int leve_gab);
	//SW050806 기존 데이타 테이블 파싱에서 공식화로 변경
	DWORD GetPlayerExpPoint(int level_gap,DWORD MonsterExp);

	double	getPhysicalAttackPower(CObject * pObject, float PhyAttackRate, BOOL bCritical );
	double	getAttributeAttackPower(CObject * pObject, WORD Attrib, DWORD AttAttackMin,DWORD AttAttackMax,float AttAttackRate );
	
	double	getPhyDefenceLevel(CObject* pObject, CObject* pAttacker);

	BOOL	getCritical(CObject* pAttacker,CObject* pTarget,float fCriticalRate);
	
private:

};

#endif // !defined(AFX_ATTACKCALC_H__EB8D1663_38B7_43EA_A9AA_07FB8ACC1B01__INCLUDED_)

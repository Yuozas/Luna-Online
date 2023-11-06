/**************本资源来自互联网 开发者中心提示禁商业 仅学习*******************/




/*************站长 星期八 感谢广大用户对开发者中心游戏资源社区的大力支持*****************/
// AbilityGroup.h: interface for the CAbilityGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ABILITYGROUP_H__0143CFF4_ADB3_4F77_B3F8_D66F06568673__INCLUDED_)
#define AFX_ABILITYGROUP_H__0143CFF4_ADB3_4F77_B3F8_D66F06568673__INCLUDED_

#if _MSC_VER > 1000
#pragma once

#endif // _MSC_VER > 1000

#include "Ability.h"

#include "AbilityCommonHeader.h"




class CPlayer;


class CAbilityGroup  
{
	CPlayer* m_pObject;
	CYHHashTable<CAbility> m_AbilityTable;
	DWORD m_AbilityExp;
	
public:
	CAbilityGroup();
	~CAbilityGroup();



	void Init(ABILITY_TOTALINFO* pTotalInfo);
	void Release();

	void AddAbility(CAbilityInfo* pAbilityInfo,BYTE Level,POSTYPE QuickPos);
	void SetAbilityExp(DWORD Exp);
	DWORD GetAbilityExp()			{	return m_AbilityExp;	}
	
	void SetOwenerObject(CPlayer* pObject)	{	m_pObject = pObject;	}
	CPlayer* GetOwenerObject()	{	return m_pObject;	}

	void SetPositionHead();
	CAbility* GetNextAbility(int GroupNum);

	CAbility* GetAbility(WORD Idx);
	void DeleteAbility(CAbility* pAbility,POSTYPE QuickPos);
};

#endif // !defined(AFX_ABILITYGROUP_H__0143CFF4_ADB3_4F77_B3F8_D66F06568673__INCLUDED_)



/**************本资源来自互联网 开发者中心提示禁商业 仅学习*******************/





/*************站长 星期八 感谢广大用户对开发者中心游戏资源社区的大力支持*****************/
// AbilityUse_Society.h: interface for the CAbilityUse_Society class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_ABILITYUSE_SOCIETY_H__20F585C7_81FB_441C_AA01_1B27DBAF7B59__INCLUDED_)
#define AFX_ABILITYUSE_SOCIETY_H__20F585C7_81FB_441C_AA01_1B27DBAF7B59__INCLUDED_

#if _MSC_VER > 1000
#pragma once

#endif // _MSC_VER > 1000





#include "AbilityUse.h"

class CAbilityUse_Society : public CAbilityUse  

{
public:
	CAbilityUse_Society();
	virtual ~CAbilityUse_Society();

//	virtual void Use(BYTE Level,CAbilityInfo* pAbilityInfo);
	void Act(CObject* pObject, CAbilityInfo* pAbilityInfo);
};

#endif // !defined(AFX_ABILITYUSE_SOCIETY_H__20F585C7_81FB_441C_AA01_1B27DBAF7B59__INCLUDED_)



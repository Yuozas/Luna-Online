/**************本资源来自互联网 开发者中心提示禁商业 仅学习*******************/


/*************站长 星期八 感谢广大用户对开发者中心游戏资源社区的大力支持*****************/
// AbilityUse.h: interface for the CAbilityUse class.
//

//////////////////////////////////////////////////////////////////////


#if !defined(AFX_ABILITYUSE_H__FA1F572F_34EB_42C5_97F9_13D3965D539F__INCLUDED_)
#define AFX_ABILITYUSE_H__FA1F572F_34EB_42C5_97F9_13D3965D539F__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "AbilityInfo.h"

class CAbilityUse  
{
public:

	CAbilityUse();

	virtual ~CAbilityUse();



	virtual void Use(BYTE Level,CAbilityInfo* pAbilityInfo)			{}
	virtual BOOL Activate(BYTE Level,CAbilityInfo* pAbilityInfo)	{	return TRUE;	}
	virtual BOOL UnActivate(BYTE Level,CAbilityInfo* pAbilityInfo)	{	return FALSE;	}

};

#endif // !defined(AFX_ABILITYUSE_H__FA1F572F_34EB_42C5_97F9_13D3965D539F__INCLUDED_)




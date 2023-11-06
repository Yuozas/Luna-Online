/**************本资源来自互联网 开发者中心提示禁商业 仅学习*******************/



/*************站长 星期八 感谢广大用户对开发者中心游戏资源社区的大力支持*****************/
// AbilityUse_KyungGong.h: interface for the CAbilityUse_KyungGong class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ABILITYUSE_KYUNGGONG_H__713DE1F1_CA58_4A2F_AB64_0CB3933532C0__INCLUDED_)
#define AFX_ABILITYUSE_KYUNGGONG_H__713DE1F1_CA58_4A2F_AB64_0CB3933532C0__INCLUDED_

#if _MSC_VER > 1000


#pragma once
#endif // _MSC_VER > 1000

#include "AbilityUse.h"





class CAbilityUse_KyungGong : public CAbilityUse  

{
public:

	CAbilityUse_KyungGong();
	virtual ~CAbilityUse_KyungGong();

	virtual BOOL Activate(BYTE Level,CAbilityInfo* pAbilityInfo);
	virtual BOOL UnActivate(BYTE Level,CAbilityInfo* pAbilityInfo);
};


#endif // !defined(AFX_ABILITYUSE_KYUNGGONG_H__713DE1F1_CA58_4A2F_AB64_0CB3933532C0__INCLUDED_)



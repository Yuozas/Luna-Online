/**************本资源来自互联网 开发者中心提示禁商业 仅学习*******************/


/*************站长 星期八 感谢广大用户对开发者中心游戏资源社区的大力支持*****************/
// AbilityUse_Job.h: interface for the CAbilityUse_Job class.

//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ABILITYUSE_JOB_H__0C785C6A_D0B8_48DD_9A26_10F354F85997__INCLUDED_)
#define AFX_ABILITYUSE_JOB_H__0C785C6A_D0B8_48DD_9A26_10F354F85997__INCLUDED_

#if _MSC_VER > 1000


#pragma once
#endif // _MSC_VER > 1000


#include "AbilityUse.h"



class CAbilityUse_Job : public CAbilityUse  
{
public:

	CAbilityUse_Job();
	virtual ~CAbilityUse_Job();


	virtual void Use(BYTE Level,CAbilityInfo* pAbilityInfo);
};



#endif // !defined(AFX_ABILITYUSE_JOB_H__0C785C6A_D0B8_48DD_9A26_10F354F85997__INCLUDED_)



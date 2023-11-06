/**************本资源来自互联网 开发者中心提示禁商业 仅学习*******************/


/*************站长 星期八 感谢广大用户对开发者中心游戏资源社区的大力支持*****************/
#pragma once

#include "AbilityUse.h"


class CAbilityUse_Pet :	public CAbilityUse
{
public:
	CAbilityUse_Pet(void);
	virtual ~CAbilityUse_Pet(void);

	virtual void Use(BYTE Level,CAbilityInfo* pAbilityInfo);
};



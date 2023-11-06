/**************本资源来自互联网 开发者中心提示禁商业 仅学习*******************/

/*****************如需帮助请到开发者中心交流总群咨询 QQ群号:63081844*********************/



/*************站长 星期八 感谢广大用户对开发者中心游戏资源社区的大力支持*****************/

// AbilityUpdater_CheckLevelUp.h: interface for the CAbilityUpdater_CheckLevelUp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ABILITYUPDATER_CHECKLEVELUP_H__CBF9E0F9_5F61_43AE_9F92_A8A250171AC8__INCLUDED_)
#define AFX_ABILITYUPDATER_CHECKLEVELUP_H__CBF9E0F9_5F61_43AE_9F92_A8A250171AC8__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AbilityUpdater.h"


class CAbilityUpdater_CheckLevelUp : public CAbilityUpdater  
{

	BOOL GetAbility(CAbilityGroup* pGroup,CAbilityInfo* pAbilityInfo);
public:
	MAKESINGLETON(CAbilityUpdater_CheckLevelUp);



	CAbilityUpdater_CheckLevelUp();
	virtual ~CAbilityUpdater_CheckLevelUp();


	virtual void Update(DWORD& UpdateCmd,DWORD Param,CAbilityGroup* pGroup,CYHHashTable<CAbilityInfo>* pInfoTable);
};

#endif // !defined(AFX_ABILITYUPDATER_CHECKLEVELUP_H__CBF9E0F9_5F61_43AE_9F92_A8A250171AC8__INCLUDED_)



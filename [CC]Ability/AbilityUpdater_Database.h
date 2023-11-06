/**************本资源来自互联网 开发者中心提示禁商业 仅学习*******************/



/*****************如需帮助请到开发者中心交流总群咨询 QQ群号:63081844*********************/
/*************站长 星期八 感谢广大用户对开发者中心游戏资源社区的大力支持*****************/

// AbilityUpdater_Database.h: interface for the CAbilityUpdater_Database class.

//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_ABILITYUPDATER_DATABASE_H__B88EB21D_BC38_4AC0_AA56_0F5260453195__INCLUDED_)
#define AFX_ABILITYUPDATER_DATABASE_H__B88EB21D_BC38_4AC0_AA56_0F5260453195__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "AbilityUpdater.h"

class CAbilityUpdater_Database : public CAbilityUpdater  
{
public:
	MAKESINGLETON(CAbilityUpdater_Database);




	CAbilityUpdater_Database();
	virtual ~CAbilityUpdater_Database();

	virtual void Update(DWORD& UpdateCmd,DWORD Param,CAbilityGroup* pGroup,CYHHashTable<CAbilityInfo>* pInfoTable);
};


#endif // !defined(AFX_ABILITYUPDATER_DATABASE_H__B88EB21D_BC38_4AC0_AA56_0F5260453195__INCLUDED_)



/**************本资源来自互联网 开发者中心提示禁商业 仅学习*******************/



/*************站长 星期八 感谢广大用户对开发者中心游戏资源社区的大力支持*****************/
// AbilityUpdater.h: interface for the CAbilityUpdater class.
//

//////////////////////////////////////////////////////////////////////


#if !defined(AFX_ABILITYUPDATER_H__75A44D14_B475_4C30_863B_9E2DC9D9C902__INCLUDED_)

#define AFX_ABILITYUPDATER_H__75A44D14_B475_4C30_863B_9E2DC9D9C902__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HashTable.h"
#include "AbilityInfo.h"

class CAbilityGroup;

class CAbilityUpdater  
{


public:
	CAbilityUpdater();
	virtual ~CAbilityUpdater();

	virtual void Update(DWORD& UpdateCmd,DWORD Param,CAbilityGroup* pGroup,CYHHashTable<CAbilityInfo>* pInfoTable) = 0;
};


#endif // !defined(AFX_ABILITYUPDATER_H__75A44D14_B475_4C30_863B_9E2DC9D9C902__INCLUDED_)




/**************本资源来自互联网 开发者中心提示禁商业 仅学习*******************/


/*************站长 星期八 感谢广大用户对开发者中心游戏资源社区的大力支持*****************/
// AbilityUpdater_ApplyData.h: interface for the CAbilityUpdater_ApplyData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ABILITYUPDATER_APPLYDATA_H__11E317D5_4EE7_469C_A9F7_46D170BDFE1C__INCLUDED_)
#define AFX_ABILITYUPDATER_APPLYDATA_H__11E317D5_4EE7_469C_A9F7_46D170BDFE1C__INCLUDED_


#if _MSC_VER > 1000

#pragma once
#endif // _MSC_VER > 1000

#include "AbilityUpdater.h"



class CAbilityUpdater_ApplyData : public CAbilityUpdater  
{

public:
	MAKESINGLETON(CAbilityUpdater_ApplyData);


	CAbilityUpdater_ApplyData();
	virtual ~CAbilityUpdater_ApplyData();



	virtual void Update(DWORD& UpdateCmd,DWORD Param,CAbilityGroup* pGroup,CYHHashTable<CAbilityInfo>* pInfoTable);

};


#endif // !defined(AFX_ABILITYUPDATER_APPLYDATA_H__11E317D5_4EE7_469C_A9F7_46D170BDFE1C__INCLUDED_)



/**************本资源来自互联网 开发者中心提示禁商业 仅学习*******************/


/*************站长 星期八 感谢广大用户对开发者中心游戏资源社区的大力支持*****************/
// AbilityUpdater_Interface.h: interface for the CAbilityUpdater_Interface class.



//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_ABILITYUPDATER_INTERFACE_H__AB1B8E34_67A2_4A8A_B1A6_EEBDEB6278AE__INCLUDED_)
#define AFX_ABILITYUPDATER_INTERFACE_H__AB1B8E34_67A2_4A8A_B1A6_EEBDEB6278AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include "AbilityUpdater.h"



class CAbilityUpdater_Interface : public CAbilityUpdater  
{
public:
	MAKESINGLETON(CAbilityUpdater_Interface);


	CAbilityUpdater_Interface();
	virtual ~CAbilityUpdater_Interface();


	virtual void Update(DWORD& Flag,DWORD Param,CAbilityGroup* pGroup,CYHHashTable<CAbilityInfo>* pInfoTable);
};

#endif // !defined(AFX_ABILITYUPDATER_INTERFACE_H__AB1B8E34_67A2_4A8A_B1A6_EEBDEB6278AE__INCLUDED_)



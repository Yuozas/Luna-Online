/**************本资源来自互联网 开发者中心提示禁商业 仅学习*******************/

/*****************如需帮助请到开发者中心交流总群咨询 QQ群号:63081844*********************/
/*************站长 星期八 感谢广大用户对开发者中心游戏资源社区的大力支持*****************/
/**************本资源来自互联网 开发者中心提示禁商业 仅学习*******************/


/*****************如需帮助请到开发者中心交流总群咨询 QQ群号:63081844*********************/
/*************站长 星期八 感谢广大用户对开发者中心游戏资源社区的大力支持*****************/
// AbilityUpdater_CheckLevelUp.cpp: implementation of the CAbilityUpdater_CheckLevelUp class.
//
//////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "AbilityUpdater_CheckLevelUp.h"
#include "AbilityGroup.h"

#include "AbilityCommonHeader.h"
#include "Player.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAbilityUpdater_CheckLevelUp::CAbilityUpdater_CheckLevelUp()
{

}

CAbilityUpdater_CheckLevelUp::~CAbilityUpdater_CheckLevelUp()

{


}

BOOL CAbilityUpdater_CheckLevelUp::GetAbility(CAbilityGroup* pGroup,CAbilityInfo* pAbilityInfo)
{

	BYTE InitLevel = pAbilityInfo->GetInfo()->Ability_AcquireInitLevel;
	if(InitLevel == 0)
		return FALSE;

	pGroup->AddAbility(pAbilityInfo,InitLevel,0);
	return TRUE;
}

void CAbilityUpdater_CheckLevelUp::Update(DWORD& UpdateCmd,DWORD Param,CAbilityGroup* pGroup,CYHHashTable<CAbilityInfo>* pInfoTable)
{
	BOOL bAbilityLevelChanged = FALSE;
	CPlayer* pObj = pGroup->GetOwenerObject();
	CAbilityInfo* pAbilityInfo;
	ABILITY_INFO* pInfo;
	pInfoTable->SetPositionHead();

	while(pAbilityInfo = pInfoTable->GetData())
	{

		// 饭骇捞 0牢 漂扁俊 措秦辑父 八荤 茄促.


		if(pGroup->GetAbility(pAbilityInfo->GetIdx()))
			continue;

		pInfo = pAbilityInfo->GetInfo();


		// 纳腐狼 饭骇捞 函拳沁阑锭
		if( UpdateCmd == ABILITYUPDATE_CHARACTERLEVEL_CHANGED &&
			pInfo->Ability_AcquireKind == eAAK_CharacterLevel )
		{

			LEVELTYPE Level = (LEVELTYPE)Param;

			if(Level >= pInfo->Ability_AcquireParam1)
				bAbilityLevelChanged |= GetAbility(pGroup,pAbilityInfo);
		}


		// 促弗 漂扁狼 饭骇捞 函拳沁阑锭
		if( UpdateCmd == ABILITYUPDATE_ABILITYLEVEL_CHANGED &&
			pInfo->Ability_AcquireKind == eAAK_OtherAbility )
		{
			WORD NeedAbilityIdx = pInfo->Ability_AcquireParam1;

			BYTE NeedAbilityLevel = (BYTE)pInfo->Ability_AcquireParam2;
			CAbility* pNeedAbility = pGroup->GetAbility(NeedAbilityIdx);
			if(pNeedAbility == NULL)
				continue;
			if(pNeedAbility->GetLevel() >= NeedAbilityLevel)
				bAbilityLevelChanged |= GetAbility(pGroup,pAbilityInfo);
		}
		
		// 酒捞牌 借阑版快
		if( UpdateCmd == ABILITYUPDATE_ABILITYUPITEM_USE &&
			pInfo->Ability_AcquireKind == eAAK_Item )
		{
			WORD UsedItemIdx = (WORD)Param;
			WORD NeedItemIdx = pInfo->Ability_AcquireParam1;
			if(UsedItemIdx == NeedItemIdx)
				bAbilityLevelChanged |= GetAbility(pGroup,pAbilityInfo);
		}

		// 涅胶飘... -_-a


	}

	if(bAbilityLevelChanged)
	{
		UpdateCmd = ABILITYUPDATE_ABILITYLEVEL_CHANGED;

		Update(UpdateCmd,0,pGroup,pInfoTable);
	}

}





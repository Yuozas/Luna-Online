/**************本资源来自互联网 开发者中心提示禁商业 仅学习*******************/


/*************站长 星期八 感谢广大用户对开发者中心游戏资源社区的大力支持*****************/

/**************本资源来自互联网 开发者中心提示禁商业 仅学习*******************/




/*************站长 星期八 感谢广大用户对开发者中心游戏资源社区的大力支持*****************/
#include "stdafx.h"
#include ".\abilityuse_pet.h"
#include "AbilityManager.h"


#ifdef _MHCLIENT_
#include "PetManager.h"

#endif



CAbilityUse_Pet::CAbilityUse_Pet(void)
{
}

CAbilityUse_Pet::~CAbilityUse_Pet(void)
{
}

void CAbilityUse_Pet::Use(BYTE Level,CAbilityInfo* pAbilityInfo)
{
#ifdef _MHCLIENT_


	ySWITCH(pAbilityInfo->GetInfo()->Ability_effect_Param1)

		yCASE(eAUKPET_State)
			//!!!浦惑怕芒 凯扁

			PETMGR->OpenPetStateDlg();
		yCASE(eAUKPET_Inven)
			//!!!浦牢亥芒 凯扁


			PETMGR->OpenPetInvenDlg();
		yCASE(eAUKPET_Skill)
			//!!!浦胶懦荤侩

			PETMGR->CheckRestNSkillUse();
		yCASE(eAUKPET_Rest)
			//!!!浦绒侥惑怕汲沥

			PETMGR->SendPetRestMsg(TRUE);
		yCASE(eAUKPET_Seal)
			//!!!浦豪牢
			PETMGR->SendPetSealMsg();
			yDEFAULT
		yENDSWITCH
#endif


}








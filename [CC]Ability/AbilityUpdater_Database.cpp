/**************本资源来自互联网 开发者中心提示禁商业 仅学习*******************/

/*****************如需帮助请到开发者中心交流总群咨询 QQ群号:63081844*********************/
/*************站长 星期八 感谢广大用户对开发者中心游戏资源社区的大力支持*****************/

/**************本资源来自互联网 开发者中心提示禁商业 仅学习*******************/


/*****************如需帮助请到开发者中心交流总群咨询 QQ群号:63081844*********************/
/*************站长 星期八 感谢广大用户对开发者中心游戏资源社区的大力支持*****************/
// AbilityUpdater_Database.cpp: implementation of the CAbilityUpdater_Database class.

//

//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "AbilityUpdater_Database.h"
#include "AbilityGroup.h"

#include "AbilityCommonHeader.h"
#include "Player.h"

#include "AbilityManager.h"


//////////////////////////////////////////////////////////////////////

// Construction/Destruction

//////////////////////////////////////////////////////////////////////

CAbilityUpdater_Database::CAbilityUpdater_Database()

{



}


CAbilityUpdater_Database::~CAbilityUpdater_Database()
{

}


void CAbilityUpdater_Database::Update(DWORD& UpdateCmd,DWORD Param,CAbilityGroup* pGroup,CYHHashTable<CAbilityInfo>* pInfoTable)
{
	if( UpdateCmd != ABILITYUPDATE_ABILITYLEVEL_CHANGED &&
		UpdateCmd != ABILITYUPDATE_ABILITYQUICK_CHANGED)

		return;

	ABILITY_TOTALINFO TInfo;
	ABILITYMGR->GetAbilityTotalInfo(pGroup,&TInfo);

	g_DB.FreeQuery(0,0,


		"MP_CHARACTER_UpdateAbility %d, \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\'",


			pGroup->GetOwenerObject()->GetID(),
			TInfo.AbilityDataArray[eAGK_Battle],
			TInfo.AbilityDataArray[eAGK_KyungGong],
			TInfo.AbilityDataArray[eAGK_Society],
			TInfo.AbilityDataArray[eAGK_Job],
			TInfo.AbilityQuickPositionArray[eAGK_Battle],
			TInfo.AbilityQuickPositionArray[eAGK_KyungGong],
			TInfo.AbilityQuickPositionArray[eAGK_Society],
			TInfo.AbilityQuickPositionArray[eAGK_Job]);
}





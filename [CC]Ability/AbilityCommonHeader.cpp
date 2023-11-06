/**************本资源来自互联网 开发者中心提示禁商业 仅学习*******************/



/*************站长 星期八 感谢广大用户对开发者中心游戏资源社区的大力支持*****************/
/**************本资源来自互联网 开发者中心提示禁商业 仅学习*******************/


/*************站长 星期八 感谢广大用户对开发者中心游戏资源社区的大力支持*****************/
#include "stdafx.h"
#include "AbilityCommonHeader.h"



WORD GetAbilityIndex_WithKindPosition(BYTE Kind,BYTE Position)

{
	ASSERT(Kind < eAGK_Max);
	ASSERT(Position < MAX_ABILITY_NUM_PER_GROUP);
	return ABILITY_STARTINDEX_BATTLE + ABILITY_STARTINDEX_INTERVAL*Kind + Position;


}

void GetKindPosition_WithAbilityIndex(WORD AbilityIndex,BYTE* pOutKind,BYTE* pOutPosition)
{

	*pOutKind = (AbilityIndex-ABILITY_STARTINDEX_BATTLE)/ABILITY_STARTINDEX_INTERVAL;
	*pOutPosition = AbilityIndex%ABILITY_STARTINDEX_INTERVAL;
}


BYTE GetAbilityLevel_WithDBValue(BYTE DBValue)


{
	ASSERT(DBValue >= ABILITYLEVEL_DB_KEY);
	ASSERT(DBValue-ABILITYLEVEL_DB_KEY <= ABILITY_MAX_LEVEL);
	
	return DBValue - ABILITYLEVEL_DB_KEY;
}




// 06. 01 捞康霖 - 窜绵芒 函版

// 捞 窃荐甫 荤侩窍瘤 臼绰促
/*
WORD GetAbilityQuickPosition(BYTE DBValue)

{
	if(DBValue == ABILITYQUICKPOSITION_NULL)
		return 0;


	
	return (DBValue - ABILITYLEVEL_DB_KEY) + TP_QUICK_START;


}

*/





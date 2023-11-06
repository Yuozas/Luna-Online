#include "StdAfx.h"
#include "TrapSkillUnit.h"
#include "../Object/SkillObject.h"
#include "../Info/ActiveSkillInfo.h"
#include "RegenManager.h"

cTrapSkillUnit::cTrapSkillUnit( cSkillObject* object, cTarget* target ) :
cSkillUnit( object, target )
{}

cTrapSkillUnit::~cTrapSkillUnit(void)
{}

BOOL cTrapSkillUnit::Excute()
{
	// 080616 LUJ, 설정에 정의된 몬스터를 소환하자

	if( !	mpParentSkill ||
		!	mpParentSkill->GetOperator() )
	{
		return FALSE;
	}

	const ACTIVE_SKILL_INFO* skillInfo = 0;
	{
		cActiveSkillInfo* info = ( cActiveSkillInfo* )mpParentSkill->GetSkillInfo();

		if( ! info )
		{
			return FALSE;
		}

		skillInfo = info->GetSkillInfo();

		if( ! skillInfo )
		{
			return FALSE;
		}
	}

	const DWORD monsterKind = skillInfo->UnitData;

	// 080616 LUJ, 몬스터를 소환한다
	return REGENMGR->RegenTrap(
		mpParentSkill->GetOperator(),
		monsterKind );
}
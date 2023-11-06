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
	// 080616 LUJ, ������ ���ǵ� ���͸� ��ȯ����

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

	// 080616 LUJ, ���͸� ��ȯ�Ѵ�
	return REGENMGR->RegenTrap(
		mpParentSkill->GetOperator(),
		monsterKind );
}
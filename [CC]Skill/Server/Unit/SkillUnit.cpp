#include "stdafx.h"
#include ".\skillunit.h"
#include "../Object/SkillObject.h"
#include "../Info/ActiveSkillInfo.h"
#include "../Target/Target.h"

cSkillUnit::cSkillUnit( cSkillObject* pParentSkill, cTarget* pTarget )
: mpParentSkill( pParentSkill ), mpTarget( pTarget )
{
}

cSkillUnit::~cSkillUnit(void)
{
}

void cSkillUnit::TargetUpdate()
{
	mpTarget->Update();
}

void cSkillUnit::Init()
{
	mUnitType = ( ( cActiveSkillInfo* )mpParentSkill->GetSkillInfo() )->GetSkillInfo()->Unit;
	mAccuracy = ( ( cActiveSkillInfo* )mpParentSkill->GetSkillInfo() )->GetSkillInfo()->Success;
	mAddDamage = ( ( cActiveSkillInfo* )mpParentSkill->GetSkillInfo() )->GetSkillInfo()->UnitData;
	mAddType = ( ( cActiveSkillInfo* )mpParentSkill->GetSkillInfo() )->GetSkillInfo()->UnitDataType;
}

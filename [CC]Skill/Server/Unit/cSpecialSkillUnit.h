#pragma once
#include "skillunit.h"

class cSpecialSkillUnit :
	public cSkillUnit
{
public:
	cSpecialSkillUnit( cSkillObject* pParentSkill, cTarget* pTarget );
	virtual ~cSpecialSkillUnit(void);

	virtual BOOL Excute();
};

#pragma once
#include "skillunit.h"

class cDeBuffSkillUnit :
	public cSkillUnit
{
public:
	cDeBuffSkillUnit( cSkillObject* pParentSkill, cTarget* pTarget );
	virtual ~cDeBuffSkillUnit(void);

	virtual BOOL Excute();
};

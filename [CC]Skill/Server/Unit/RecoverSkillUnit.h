#pragma once
#include "skillunit.h"

enum RECOVER_TYPE
{
	RECOVER_LIFE,
	RECOVER_MANA,
};

class cRecoverSkillUnit :
	public cSkillUnit
{
public:
	cRecoverSkillUnit( cSkillObject* pParentSkill, cTarget* pTarget );
	virtual ~cRecoverSkillUnit( void );

	virtual BOOL Excute();
};

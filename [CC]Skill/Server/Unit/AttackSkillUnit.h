#pragma once
#include "skillunit.h"

enum ATTACK_TYPE
{
	ATTACK_NORMAL,
};

class cAttackSkillUnit :
	public cSkillUnit
{
public:
	cAttackSkillUnit( cSkillObject* pParentSkill, cTarget* pTarget );
	virtual ~cAttackSkillUnit( void );

	virtual BOOL Excute();
};

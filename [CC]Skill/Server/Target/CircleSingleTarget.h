#pragma once
#include "singletarget.h"

class cCircleSingleTarget :
	public cSingleTarget
{
public:
	cCircleSingleTarget(void);
	virtual ~cCircleSingleTarget(void);

	virtual void Init( cSkillObject* pParentSkill, MAINTARGET* pMainTarget );
	virtual void Update();
};

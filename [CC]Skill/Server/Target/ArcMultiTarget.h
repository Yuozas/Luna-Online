#pragma once
#include "multitarget.h"

class cArcMultiTarget :
	public cMultiTarget
{
public:
	cArcMultiTarget(void);
	virtual ~cArcMultiTarget(void);

	virtual void Init( cSkillObject* pParentSkill, MAINTARGET* pMainTarget );
	virtual void Update();
};

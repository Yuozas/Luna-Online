#pragma once
#include "multitarget.h"

class cCircleMultiTarget :
	public cMultiTarget
{
public:
	cCircleMultiTarget(void);
	virtual ~cCircleMultiTarget(void);
	
	virtual void Init( cSkillObject* pParentSkill, MAINTARGET* pMainTarget );
	virtual void Update();
};

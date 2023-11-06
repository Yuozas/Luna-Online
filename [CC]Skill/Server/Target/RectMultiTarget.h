#pragma once
#include "multitarget.h"

class cRectMultiTarget :
	public cMultiTarget
{
public:
	cRectMultiTarget(void);
	virtual ~cRectMultiTarget(void);

	virtual void Init( cSkillObject* pParentSkill, MAINTARGET* pMainTarget );
	virtual void Update();
};

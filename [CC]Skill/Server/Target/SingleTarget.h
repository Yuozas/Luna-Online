#pragma once
#include "target.h"

class CObject;

class cSingleTarget :
	public cTarget
{
	DWORD			mTarget;
public:
	cSingleTarget(void);
	virtual ~cSingleTarget(void);

	virtual void		Init( cSkillObject* pParentSkill, MAINTARGET* pMainTarget );
	virtual void		Update();

	virtual CObject*	GetData();
};

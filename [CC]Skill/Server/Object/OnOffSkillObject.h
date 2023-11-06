#pragma once
#include "activeskillobject.h"

class cOnOffSkillObject :
	public cActiveSkillObject
{
	cDelay*		mpInterval;
	void		Consumption();

public:
	cOnOffSkillObject( cSkillInfo* pSkillInfo, cTarget* pTarget );
	virtual ~cOnOffSkillObject(void);

	virtual void	Init( sSKILL_CREATE_INFO* pInfo );
	virtual void	Release();
	void Destroy();
	virtual SKILL_STATE	Update();
	virtual void	Excute();
};

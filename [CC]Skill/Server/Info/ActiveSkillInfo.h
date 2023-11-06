#pragma once
#include "./skillinfo.h"

class cActiveSkillInfo :
	public cSkillInfo
{
protected:
	ACTIVE_SKILL_INFO mInfo;
public:
	cActiveSkillInfo(void);
	virtual ~cActiveSkillInfo(void);

	BOOL InitSkillInfo(CMHFile* pFile);

	BOOL IsExcutableSkillState(CObject* pObject,int SkillLevel);
	BOOL IsInSkillRange(CObject* pObject,VECTOR3* pTargetPos,float TargetRadius);

	ACTIVE_SKILL_INFO*	GetSkillInfo();
	const ACTIVE_SKILL_INFO& GetInfo() const { return mInfo; }
};

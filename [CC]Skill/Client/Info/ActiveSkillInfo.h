#pragma once
#include "skillinfo.h"
#include "../delay/timedelay.h"

class CHero;
class CActionTarget;
class CHeroPet;

class cActiveSkillInfo :
	public cSkillInfo
{
	cTimeDelay	mCoolTime;
	cTimeDelay	mPetCoolTime;

public:
	cActiveSkillInfo(void);
	virtual ~cActiveSkillInfo(void);

	BOOL InitSkillInfo(CMHFile* pFile);

	BOOL IsExcutableSkillState(CHero* pHero,int SkillLevel,CActionTarget* pTarget);
	BOOL IsInSkillRange(CHero* pHero,CActionTarget* pTarget);
	BOOL IsExcutablePetSkillState(CHeroPet* pHeroPet,int SkillLevel,CActionTarget* pTarget);
	BOOL IsInPetSkillRange(CHeroPet* pHeroPet,CActionTarget* pTarget);

	ACTIVE_SKILL_INFO*	GetSkillInfo();

	BOOL IsValidTarget(CHero* pHero,CActionTarget* pTarget);
	// 071228 LYW --- ActiveSkillInfo : 버프 인덱스를 반환하는 함수 추가.
	DWORD GetBuffSkillIdx(int index) ;

	void SetCooling();
	cTimeDelay* GetCoolTime() { return &mCoolTime; }
	void SetPetCooling();
	cTimeDelay* GetPetCoolTime() { return &mPetCoolTime; }
};

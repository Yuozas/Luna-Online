#pragma once

enum SKILLUNIT_KIND
{
	SKILLUNIT_NONE,
	SKILLUNIT_ATTACK,
	SKILLUNIT_RECOVER,
};

class cSkillInfo;
class cSkillObject;
class cTarget;

class cSkillUnit
{
protected:
	cSkillObject*	mpParentSkill;
	cTarget*		mpTarget;
	WORD			mUnitType;
	float			mAccuracy;
	float			mAddDamage;
	WORD			mAddType;


public:
	cSkillUnit( cSkillObject* pParentSkill, cTarget* pTarget );
	virtual ~cSkillUnit( void );
	
	void TargetUpdate();

	virtual BOOL Excute() = 0;
	virtual void Init();
};

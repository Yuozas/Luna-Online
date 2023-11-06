#pragma once

class cSkillObject;
class cSkillInfo;
class cTarget;

class cSkillObjectFactory
{
public:
	cSkillObjectFactory(void);
	virtual ~cSkillObjectFactory(void);

	static cSkillObject* CreateSkillObject( cSkillInfo* pSkillInfo );

	static cSkillObject* CreateNonBuffSkillObject( cSkillInfo* pSkillInfo );
	static cSkillObject* CreateBuffSkillObject( cSkillInfo* pSkillInfo );
};

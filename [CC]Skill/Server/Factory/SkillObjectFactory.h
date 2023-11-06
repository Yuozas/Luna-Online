#pragma once

class cSkillObject;
class cSkillInfo;
class cTarget;

class cSkillObjectFactory
{
public:
	cSkillObjectFactory( void );
	virtual ~cSkillObjectFactory( void );

	static cSkillObject* CreateSkillObject( cSkillInfo* pSkillInfo );
	
	static cSkillObject* CreateActiveSkillObject( cSkillInfo* pSkillInfo, cTarget* pTarget );
	static cSkillObject* CreateOnOffSkillObject( cSkillInfo* pSkillInfo, cTarget* pTarget );
	static cSkillObject* CreateBuffSkillObject( cSkillInfo* pSkillInfo, cTarget* pTarget );
};

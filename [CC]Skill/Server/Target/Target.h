#pragma once

class cSkillObject;
class CObject;

class cTarget
{
protected:
	TARGET_KIND		mKind;
	cSkillObject*	mpParentSkill;
	MAINTARGET*		mpMainTarget;

public:
	cTarget(void);
	virtual ~cTarget(void);
	virtual void		Init( cSkillObject* pParentSkill, MAINTARGET* pMainTarget );
	virtual void		Update() = 0;
	
	

	TARGET_KIND	GetKind();

	virtual	void		SetPositionHead() {};
	virtual CObject*	GetData();
};

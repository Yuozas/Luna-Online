#include "stdafx.h"
#include ".\target.h"

#include "../Object/SkillObject.h"

cTarget::cTarget(void)
{
}

cTarget::~cTarget(void)
{
}

void cTarget::Init( cSkillObject* pParentSkill, MAINTARGET* pMainTarget )
{
	mpParentSkill = pParentSkill;
	mpMainTarget = pMainTarget;
}

TARGET_KIND cTarget::GetKind()
{
	return mKind;
}

CObject* cTarget::GetData()
{
	return NULL;
}
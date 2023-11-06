#include "stdafx.h"
#include ".\singletarget.h"

#include "../Object/SkillObject.h"

#include "UserTable.h"

cSingleTarget::cSingleTarget(void)
{
	mKind = TARGET_KIND_SINGLE;
}

cSingleTarget::~cSingleTarget(void)
{
}

void cSingleTarget::Init( cSkillObject* pParentSkill, MAINTARGET* pMainTarget )
{
	cTarget::Init( pParentSkill, pMainTarget );

	mTarget = pMainTarget->dwMainTargetID;
}

void cSingleTarget::Update()
{
}

CObject* cSingleTarget::GetData()
{
	return g_pUserTable->FindUser( mTarget );
}
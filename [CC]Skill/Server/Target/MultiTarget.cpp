#include "stdafx.h"
#include ".\multitarget.h"

#include "../Object/SkillObject.h"

#include "UserTable.h"

cMultiTarget::cMultiTarget(void)
{
	mKind = TARGET_KIND_MULTI;
	mTagetTable.Initialize( 10 );
}

cMultiTarget::~cMultiTarget(void)
{
}

void cMultiTarget::Init( cSkillObject* pParentSkill, MAINTARGET* pMainTarget )
{
}

void cMultiTarget::Update()
{
}

void cMultiTarget::SetPositionHead()
{
	mTagetTable.SetPositionHead();
}

CObject* cMultiTarget::GetData()
{
	CObject* pTarget = NULL;

	while( pTarget = mTagetTable.GetData() )
	{
		if( pTarget->GetInited() )
		{
			return pTarget;
		}
	}

	return NULL;
}

CObject* cMultiTarget::GetData( DWORD idx )
{
	CObject* pTarget = g_pUserTable->FindUser( idx );

	if( pTarget->GetInited() )
	{
		return pTarget;
	}
	return NULL; 
}

void cMultiTarget::AddData( CObject* pObject )
{
	mTagetTable.Add( pObject, pObject->GetID() );
}

void cMultiTarget::Remove( DWORD idx )
{
	mTagetTable.Remove( idx );
}

void cMultiTarget::RemoveAll()
{
	mTagetTable.RemoveAll();
}
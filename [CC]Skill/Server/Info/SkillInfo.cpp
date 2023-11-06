#include "stdafx.h"
#include ".\skillinfo.h"
#include "../Object/SkillObject.h"
#include "../Factory/SkillObjectFactory.h"

cSkillInfo::cSkillInfo(void)
{
}

cSkillInfo::~cSkillInfo(void)
{
	PTRLISTSEARCHSTART(m_SkillObjectPool,cSkillObject*,pSObj)
		delete pSObj;
	PTRLISTSEARCHEND;
	m_SkillObjectPool.RemoveAll();
}


cSkillObject* cSkillInfo::GetSkillObject()
{
	cSkillObject* pSObj;
	if(m_SkillObjectPool.GetCount() == 0)
		pSObj = cSkillObjectFactory::CreateSkillObject(this);
	else
		pSObj = (cSkillObject*)m_SkillObjectPool.RemoveTail();
	return pSObj;
}

void cSkillInfo::ReleaseSkillObject(cSkillObject* pSObj)
{
	PTRLISTSEARCHSTART(m_SkillObjectPool,cSkillObject*,pPreObj)
		if(pSObj == pPreObj)
		{			
			return;
		}
	PTRLISTSEARCHEND

	m_SkillObjectPool.AddHead(pSObj);
}

WORD cSkillInfo::GetKind()
{
	return mpSkillInfo->Kind;
}

DWORD cSkillInfo::GetIndex()
{
	return mpSkillInfo->Index;
}

WORD cSkillInfo::GetLevel()
{
	return mpSkillInfo->Level;
}
#pragma once

#include "PtrList.h"
#include "MHFile.h"

class cSkillObject;
class CObject;

class cSkillInfo
{
protected:
	cPtrList		m_SkillObjectPool;
	SKILL_INFO*		mpSkillInfo;

public:
	cSkillInfo(void);
	virtual ~cSkillInfo(void);

	virtual BOOL	InitSkillInfo(CMHFile* pFile) = 0;

	cSkillObject*	GetSkillObject();
	void			ReleaseSkillObject(cSkillObject* pSObj);

	WORD		GetKind();
	DWORD		GetIndex();
	WORD		GetLevel();
};

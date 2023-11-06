/*
080616 LUJ, 함정 소환 스킬 유닛 추가
*/
#pragma once
#include "..\[cc]skill\server\unit\skillunit.h"

class cTrapSkillUnit :
	public cSkillUnit
{
public:
	cTrapSkillUnit( cSkillObject*, cTarget* );
	virtual ~cTrapSkillUnit(void);

	virtual BOOL Excute();
};

/*
080616 LUJ, ���� ��ȯ ��ų ���� �߰�
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

#pragma once
#include "skillinfo.h"

class CPlayer;

class cBuffSkillInfo :
	public cSkillInfo
{
	CYHHashTable<EXCEPTION> mExceptionTable;

public:
	cBuffSkillInfo(void);
	virtual ~cBuffSkillInfo(void);
	
	BOOL InitSkillInfo(CMHFile* pFile);
	BUFF_SKILL_INFO*	GetSkillInfo();

	void AddPassiveStatus();
	void RemovePassiveStatus();

	void AddPetBuffStatus();
	void RemovePetBuffStatus();
	void AddBuffStatus();
	void RemoveBuffStatus();


	void SetObjectBattleState( CObject* pOperator, CObject* pTarget );

	void AddException( EXCEPTION* pExeption );
	void AddExceptionToTarget( CPlayer* pPlayer );
	void RemoveExceptionToTarget( CPlayer* pPlayer );
};

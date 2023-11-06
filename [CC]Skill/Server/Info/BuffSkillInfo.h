#pragma once
#include "./skillinfo.h"

class cBuffSkillInfo :
	public cSkillInfo
{
	CYHHashTable<EXCEPTION> mExceptionTable;

	void AddPlayerBuffStatus( CPlayer* pPlayer );
	void RemovePlayerBuffStatus( CPlayer* pPlayer );
	
	void AddPetBuffStatus( CPet* pPet );
	void RemovePetBuffStatus( CPet* pPet );

	void AddMonsterBuffStatus( CMonster* pMonster );
	void RemoveMonsterBuffStatus( CMonster* pMonster );

public:
	cBuffSkillInfo(void);
	virtual ~cBuffSkillInfo(void);
	
	BOOL InitSkillInfo(CMHFile* pFile);

	BUFF_SKILL_INFO*	GetSkillInfo();

	void AddPassiveStatus( CPlayer* pPlayer );
	void RemovePassiveStatus( CPlayer* pPlayer );

	void AddBuffStatus( CObject* pTarget );
	void RemoveBuffStatus( CObject* pTarget );

	void AddException( EXCEPTION* pExeption );
	void AddExceptionToTarget( CPlayer* pPlayer );
	void RemoveExceptionToTarget( CPlayer* pPlayer );
};

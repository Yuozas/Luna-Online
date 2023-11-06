#pragma once

class cSkillTree
{
	CPlayer*							mpPlayer;
	CYHHashTable< SKILL_BASE >			mSkillBaseTable;
	CMemoryPoolTempl< SKILL_BASE >*		mSkillBasePool;

public:
	cSkillTree(void);
	virtual ~cSkillTree(void);

	void Init( CPlayer* pPlayer );
	void Release();

	SKILL_BASE* NewSkillBase();
	void AddSkillBase( SKILL_BASE* pSkillBase );
	//BOOL UpdateSkillBase( DWORD idx, BYTE level );
	SKILL_BASE* GetSkillBase( DWORD SkillIdx = 0 );
	void RemoveSkillBase( SKILL_BASE* );

	void SetPositionHead();

	// 071129 LYW --- SkillTree : 모든 스킬 삭제 함수 추가.
	void RemoveAllSkill() ;
};

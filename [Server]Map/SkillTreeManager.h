#pragma once


#define SKILLTREEMGR CSkillTreeManager::GetInstance()


class CPlayer;


class CSkillTreeManager  
{
	CSkillTreeManager();
public:
	virtual ~CSkillTreeManager();

	static CSkillTreeManager* GetInstance();

	/*
	BOOL RemSkill(CPlayer * pPlayer, WORD wSkillIdx, POSTYPE TargetPos, BYTE bType);
	BOOL MoveSkill(CPlayer * pPlayer, POSTYPE FromPos, POSTYPE ToPos);
	BOOL AddSkill( CPlayer * pPlayer, WORD wMuongIdx , WORD ItemKind, POSTYPE QuickPos = 0, BYTE bLevel = 0, WORD Option = 0 );
	
	void AddSkillDBResult(CPlayer * pPlayer, SKILLBASE * pSkillBase);
	void ChangeSkill(CPlayer* pPlayer, WORD wRemSkillIdx, POSTYPE TargetPos, WORD wAddSkillIdx, WORD LogType);
	
	BOOL SetOption(CPlayer * pPlayer, WORD wSkillIdx, POSTYPE TargetPos, WORD ItemIdx, WORD ItemPos);
	BOOL ClearOption(CPlayer * pPlayer, WORD wSkillIdx, POSTYPE TargetPos, WORD ItemIdx, WORD ItemPos);
	*/
	void NetworkMsgParse( BYTE Protocol, void* pMsg );

	void UpdateSkill( SKILL_BASE*, CPlayer* pPlayer );
	void UpdateAck( CPlayer * pPlayer, SKILL_BASE* pSkillBase );
	void UpdateNack( CPlayer * pPlayer, BYTE result );

	void AddSkill( CPlayer * pPlayer, SKILL_BASE* pSkillBase );
	void AddSkillDBResult( CPlayer * pPlayer, SKILL_BASE* pSkillBase );	
};
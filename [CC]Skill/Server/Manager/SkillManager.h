#pragma once


#include "../Object/SkillObject.h"


#define SKILLMGR	cSkillManager::GetInstance()


class cSkillInfo;

enum SKILL_RELEASE
{
	SKILL_RELEASE_DELETE,
	SKILL_RELEASE_RECYCLE,
};


class cSkillManager
{
	CYHHashTable<cSkillInfo>	m_SkillInfoTable;
	CYHHashTable<cSkillObject>	m_SkillObjectTable;
	CYHHashTable<SKILL_TREE_INFO>	m_SkillTreeTable;

	cSkillManager(void);
public:
	virtual ~cSkillManager(void);
	static cSkillManager* GetInstance();

	/// 초기화/해제
	void Init();
	void Release();

	/// 스킬리스트 로딩
	void LoadSkillInfoList();

	/// 접근자
	cSkillInfo* GetSkillInfo(DWORD SkillInfoIdx);
	cSkillObject* GetSkillObject(DWORD SkillObjID);

	/// 스킬 생성
	void MonsterAttack(DWORD SkillIdx,CObject* pMonster,CObject* pTarget);
	BOOL PetAttack(DWORD SkillIdx,CObject* pPet,CObject* pTarget);
	SKILL_RESULT OnSkillStartSyn(MSG_SKILL_START_SYN* pmsg, ITEMBASE* pItemBase = NULL);
	SKILL_RESULT OnPetSkillStartSyn(MSG_SKILL_START_SYN* pmsg, ITEMBASE* pItemBase = NULL);
	cSkillObject* OnBuffSkillStart( cSkillInfo* pSkillInfo, sSKILL_CREATE_INFO* pCreateInfo );
	void BuffSkillStart( DWORD characteridx, DWORD skillidx, DWORD remaintime, WORD count );
	void AddPassive( CPlayer* pTarget, DWORD SkillIdx );
	void RemovePassive( CPlayer* pTarget, DWORD SkillIdx );
	
	void OnSkillTargetUpdate( MSG_SKILL_UPDATE_TARGET* pmsg );
	void OnSkillCancel( MSG_DWORD* pmsg );

	/// 스킬해제
	void ReleaseSkillObject(cSkillObject* pSkillObject, int ReleaseKind = SKILL_RELEASE_RECYCLE);

	/// 프로세스
	void Process();
	
	/// 네트워크 메세지 수신
	void NetworkMsgParse(BYTE Protocol,void* pMsg);

	/// 배틀 내 스킬 모두 해제
	void ReleaseAllSkillInBattle(DWORD BattleID);

	DWORD GetNewSkillObjectID();

	DWORD GetSkillSize( DWORD skillIndex ) const;

	// 080602 LUJ, 버프 스킬을 적용한다
	// 080616 LUJ, CPlayer -> CObject로 변경
	void AddBuffSkill( CObject&, const ACTIVE_SKILL_INFO& );
	// 080616 LUJ, 액티브 스킬을 적용한다
	void AddActiveSkill( CObject&, DWORD skillIndex );
	BOOL IsLearnableSkill(const CHARACTER_TOTALINFO&, const SKILL_BASE&);
private:
	typedef std::map< DWORD, DWORD >	SkillSize;
	SkillSize							mSkillSize;
};

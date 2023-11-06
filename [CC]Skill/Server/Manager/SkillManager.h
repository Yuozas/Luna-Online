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

	/// �ʱ�ȭ/����
	void Init();
	void Release();

	/// ��ų����Ʈ �ε�
	void LoadSkillInfoList();

	/// ������
	cSkillInfo* GetSkillInfo(DWORD SkillInfoIdx);
	cSkillObject* GetSkillObject(DWORD SkillObjID);

	/// ��ų ����
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

	/// ��ų����
	void ReleaseSkillObject(cSkillObject* pSkillObject, int ReleaseKind = SKILL_RELEASE_RECYCLE);

	/// ���μ���
	void Process();
	
	/// ��Ʈ��ũ �޼��� ����
	void NetworkMsgParse(BYTE Protocol,void* pMsg);

	/// ��Ʋ �� ��ų ��� ����
	void ReleaseAllSkillInBattle(DWORD BattleID);

	DWORD GetNewSkillObjectID();

	DWORD GetSkillSize( DWORD skillIndex ) const;

	// 080602 LUJ, ���� ��ų�� �����Ѵ�
	// 080616 LUJ, CPlayer -> CObject�� ����
	void AddBuffSkill( CObject&, const ACTIVE_SKILL_INFO& );
	// 080616 LUJ, ��Ƽ�� ��ų�� �����Ѵ�
	void AddActiveSkill( CObject&, DWORD skillIndex );
	BOOL IsLearnableSkill(const CHARACTER_TOTALINFO&, const SKILL_BASE&);
private:
	typedef std::map< DWORD, DWORD >	SkillSize;
	SkillSize							mSkillSize;
};

#pragma once

class CHero;
class CHeroPet;
class CActionTarget;
class cSkillInfo;
class cSkillObject;
class cActiveSkillInfo;

#define SKILLMGR	cSkillManager::GetInstance()

class cSkillManager
{
	CYHHashTable<cSkillInfo>	m_SkillInfoTable;
	CYHHashTable<cSkillObject>	m_SkillObjectTable;
	CYHHashTable<SKILL_TREE_INFO>	m_SkillTreeTable;

	void DoCreateSkillObject(cSkillObject* pSObj, SKILLOBJECT_INFO* pSOInfo);
	void DoCreateTempSkillObject(cActiveSkillInfo* pSkillInfo, CActionTarget* pTarget);
	BOOL DoChangeTempSkillObject(SKILLOBJECT_INFO* pSOInfo);
	
	void ReleaseSkillObject(cSkillObject* pObject);

	cSkillInfo*	mpSkillInfo;

public:
	cSkillManager(void);
	virtual ~cSkillManager(void);

	//GETINSTANCE(cSkillManager);
	static cSkillManager* GetInstance();

	void Init();
	void Release();

	void ReleaseAllSkillObject();

	void LoadSkillInfoList();

	cSkillObject* GetSkillObject(DWORD SkillObjectID);
	cSkillInfo*	GetSkillInfo(DWORD SkillInfoIdx);
	SKILL_TREE_INFO* GetSkillTreeInfo( WORD ClassIndex );

	DWORD GetSkillTooltipInfo(DWORD SkillInfoIdx);
	DWORD GetComboSkillIdx(CHero* pHero);
	void SetSkill(cSkillInfo* pSkillInfo) { mpSkillInfo = pSkillInfo; }

	// 스킬을 사용
	BOOL ExcuteSkillSYN(CHero* pHero,CActionTarget* pTarget,BOOL bSkill);
	BOOL ExcuteSkillSYN(CHero* pHero,CActionTarget* pTarget,cSkillInfo* pInfo);
	BOOL RealExcuteSkillSYN(CHero* pHero,CActionTarget* pTarget,cSkillInfo* pInfo);

	BOOL ExcutePetSkillSYN(CHeroPet* pHeroPet,CActionTarget* pTarget,BOOL bSkill);
	BOOL ExcutePetSkillSYN(CHeroPet* pHeroPet,CActionTarget* pTarget,cSkillInfo* pInfo);
	BOOL RealExcutePetSkillSYN(CHeroPet* pHeroPet,CActionTarget* pTarget,cSkillInfo* pInfo);

	BOOL OnSkillCommand(CHero* pHero,CActionTarget* pTarget,BOOL bSkill);
	BOOL OnSkillCommand( DWORD idx );
	void OnComboTurningPoint(CHero* pHero);
	void OnPetComboTurningPoint(CHeroPet* pHeroPet);
	void SetNextComboSkill(CHero* pHero,CActionTarget* pTarget,BOOL bSkill);

	BOOL OnPetSkillCommand(CHeroPet* pHeroPet,CActionTarget* pTarget,BOOL bSkill);
	BOOL OnPetSkillCommand( DWORD idx );
	DWORD GetPetComboSkillIdx(CHeroPet* pHeroPet);
	void SetNextPetComboSkill(CHeroPet* pHeroOet,CActionTarget* pTarget,BOOL bSkill);

	void OnReceiveSkillObjectAdd(MSG_SKILLOBJECT_ADD2* pmsg);
	void OnReceiveSkillObjectRemove(MSG_DWORD* pmsg);
	void OnReceiveSkillResult(MSG_SKILL_RESULT* pmsg);
	void OnReceiveSkillCancel(MSG_DWORD* pmsg);
	void OnReceiveSkillDelayReset(MSG_DWORD2* pmsg);
	void OnReceiveSkillCount(MSG_DWORD2* pmsg);

	void NetworkMsgParse(BYTE Protocol,void* pMsg);

	void AddPassive( DWORD SkillIdx );
	void RemovePassive( DWORD SkillIdx );

	DWORD GetSkillSize( DWORD skillIndex ) const;

	void Process();

	void SkillCancel( cSkillObject* pSkill );

	// 080318 LUJ, 스킬 오브젝트가 담긴 컨테이너를 반환한다
	inline CYHHashTable< cSkillObject >& GetSkillObjectHashTable() { return m_SkillObjectTable; }

private:
	typedef std::map< DWORD, DWORD >	SkillSize;
	SkillSize							mSkillSize;
};

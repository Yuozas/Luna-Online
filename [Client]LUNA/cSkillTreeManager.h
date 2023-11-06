#pragma once


#define SKILLTREEMGR cSkillTreeManager::GetInstance()


class cSkillBase;
class cImage;



class cSkillTreeManager
{
	CIndexGenerator mIconIndexCreator;
	CYHHashTable< cSkillBase > mSkillBaseTable;
	cSkillTreeManager(void);
	
public:
	virtual ~cSkillTreeManager(void);

	static cSkillTreeManager* GetInstance();

	void InitSkillInfo( SKILL_BASE* pSkill, WORD num );

	void Init();
	void Release();
	void NetworkMsgParse(BYTE Protocol,void* pMsg);
	// 071130 LYW --- cSkillTreeManager : 스킬 초기화 함수 추가.
	void ResetSkillTree( void* pMsg ) ;
	void SetToolTipIcon(cSkillBase *pSkill);
	
	int GetSkillNum();

	cSkillBase * NewSkillIcon(SKILL_BASE * skillBase);
	//void DeleteSkillIcon(cSkillBase* pSkill);ski
	void GetIconImage(DWORD Skilldx, cImage * pLowImage , cImage * pHighImage);
	cSkillBase* GetSkill(DWORD SkillIdx);
	const SKILL_BASE* GetSkillBase( DWORD skillIndex );
	void RefreshSkill();
	void AddSkill(cSkillBase* pSkill);
	void RemoveSkill( cSkillBase* );


	// 세트 아이템으로 발생하는 스킬 관리
public:
	void AddSetSkill( DWORD index, BYTE level );
	void RemoveSetSkill( DWORD index );
	SKILL_BASE* GetStoredSkill( DWORD index );
	void		SetStoredSkill( const SKILL_BASE* );
	BOOL IsSetSkill( DWORD index );

	// 071207 LUJ 세트 스킬 초기화
	void ResetSetSkill();

private:
	// 세트로 인해 부여된 능력이 제거될 때는, 플레이어가 정적으로 부여한 능력이 복구하기 위해 필요한 정보
	typedef std::map< DWORD, SKILL_BASE >	StoredSkill;
	StoredSkill								mStoredSkill;

	// 세트 아이템은 스킬을 부여하거나 제거한다. 이때 레벨이 다른 동일 스킬(예: 파이어볼1, 파이어볼2)이 있을 경우 복구할 방법이 없다.
	// 이에 스택에 저장하여 세트 스킬을 제거할 때마다 체크하여 역순으로 복구한다.

	typedef std::stack< BYTE >			SetSkill;		// 상대적으로 적용되는 스킬 레벨값
	typedef std::map< DWORD, SetSkill >	StoredSetSkill;
	StoredSetSkill						mStoredSetSkill;
public :
	// 071129 LYW --- cSkillTreeManager : 모든 스킬을 지우는 함수 추가.
	void RemoveAllSkill() ;

	void SetPositionHead();
	SKILL_BASE* GetSkillBase();
};
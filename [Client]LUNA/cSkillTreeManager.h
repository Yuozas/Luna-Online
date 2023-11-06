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
	// 071130 LYW --- cSkillTreeManager : ��ų �ʱ�ȭ �Լ� �߰�.
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


	// ��Ʈ ���������� �߻��ϴ� ��ų ����
public:
	void AddSetSkill( DWORD index, BYTE level );
	void RemoveSetSkill( DWORD index );
	SKILL_BASE* GetStoredSkill( DWORD index );
	void		SetStoredSkill( const SKILL_BASE* );
	BOOL IsSetSkill( DWORD index );

	// 071207 LUJ ��Ʈ ��ų �ʱ�ȭ
	void ResetSetSkill();

private:
	// ��Ʈ�� ���� �ο��� �ɷ��� ���ŵ� ����, �÷��̾ �������� �ο��� �ɷ��� �����ϱ� ���� �ʿ��� ����
	typedef std::map< DWORD, SKILL_BASE >	StoredSkill;
	StoredSkill								mStoredSkill;

	// ��Ʈ �������� ��ų�� �ο��ϰų� �����Ѵ�. �̶� ������ �ٸ� ���� ��ų(��: ���̾1, ���̾2)�� ���� ��� ������ ����� ����.
	// �̿� ���ÿ� �����Ͽ� ��Ʈ ��ų�� ������ ������ üũ�Ͽ� �������� �����Ѵ�.

	typedef std::stack< BYTE >			SetSkill;		// ��������� ����Ǵ� ��ų ������
	typedef std::map< DWORD, SetSkill >	StoredSetSkill;
	StoredSetSkill						mStoredSetSkill;
public :
	// 071129 LYW --- cSkillTreeManager : ��� ��ų�� ����� �Լ� �߰�.
	void RemoveAllSkill() ;

	void SetPositionHead();
	SKILL_BASE* GetSkillBase();
};
#pragma once


#define GUILDMGR CGuildManager::GetInstance()


class CGuildManager  
{
	void MarkChange(MSG_DWORD3 * pInfo);

	void SetPlayerNickName(CObject* pObject, char* NickName);
	void SetPlayerGuildName(CObject* pObject, DWORD dwGuildIdx, char* GuildName);
public:	
	static CGuildManager* GetInstance();

	CGuildManager();
	virtual ~CGuildManager();
	
	void Init();
	void Release();
	void NetworkMsgParse(BYTE Protocol,void* pMsg);	
	
	const char* GetMasterName() const;
	DWORD GetMemberSize() const;
	DWORD GetStudentSize() const;
	const char* GetMemberName(DWORD MemberID) const;

	void SetScore( DWORD );
	DWORD GetScore() const;
	DWORD GetLevel() const;
	const char* GetGuildName() const;

	BOOL CanEntryGuild(char* date);

	const GUILDMEMBERINFO* GetSelectedMember() const;
	
	const char* GetRankName(BYTE rank);


	// 070801 ����, ��� ������ �о���δ�.
private:
	void LoadScript();
	void ParseScoreScript( const std::list< std::string >& );
	void ParseLevelScript( const std::list< std::string >& );
	void ParseSkillScript( const std::list< std::string >& );


public:
	struct ScoreSetting
	{
		DWORD	mUnitTime;		// ���� ���� �ð�
		float	mUnitTimeScore;	// ���� ���� �ð� �� ��� ����
		float	mHuntScore;		// ���� ��ɽø��� ��� ����
		DWORD	mUpdateGap;		// DB�� �����ϰ� �������� ��ε�ĳ�����ϴ� �ֱ�. �и��� ����. �ʹ� ������ ���Ϲ߻�...
		float	mKillerScore;	// �ٸ� ������ ���ؾ��� �� ��� ����
		float	mCorpseScore;	// �ٸ� �������� ���صǾ��� �� �Ҵ� ����

		// ����� �¸��� ��� ����
		// ����: mPercent�� �й� ��尡 �Ҵ� ������ ���տ��� �¸� ��尡 ȹ���ϴ� ������ ���Ѵ�.
		struct Unit
		{
			float mValue;
			float mPercent;
		}
		mWinScore;;

		float	mWinMoneyRate;	// �����ݿ��� ȹ���ϴ� ����

		// ����� �й�� �Ҵ� ����
		typedef std::map< DWORD, Unit >		LostScoreMap;
		LostScoreMap						mLostScoreMap;
	};

private:
	ScoreSetting mScoreSetting;


public:
	struct LevelSetting
	{
		DWORD	mRequiredMasterLevel;
		DWORD	mRequiredScore;
		DWORD	mRequiredMoney;
		DWORD	mRequiredQuest;
		DWORD	mMemberSize;
		DWORD	mUnionSize;
		DWORD	mApprenticeSize;
		BOOL	mGuildMark;
		DWORD	mWarehouseSize;
		// 080417 LUJ, ȹ�� ������ ��� ��ų ����
		DWORD	mSkillSize;
	};

	const LevelSetting* GetLevelSetting( DWORD level ) const;

private:
	// Ű: ��� ����
	typedef stdext::hash_map< DWORD, LevelSetting >	LevelSettingMap;
	LevelSettingMap									mLevelSettingMap;


	// 080417 LUJ, ��� ��ų ������ �д´�
public:
	struct SkillSetting
	{
		// 080417 LUJ, ��� ������ �ִ� ��ų ����
		BYTE				mLevel;

		// 080417 LUJ, ��밡���� ����
		std::set< DWORD >	mRank;
	};

	// 080417 LUJ, ��ų ���� ������ ��� ��, Ű: ��ų �ε���
	typedef stdext::hash_map< DWORD, SkillSetting > SkillSettingMap;

	// 080417 LUJ, ���� �� ��ų ���� ������ ��� ��. Ű: ��� ����
	typedef stdext::hash_map< DWORD, SkillSettingMap >	SkillSettingLevelMap;

	// 080417 LUJ, ������ ��� ��ų ���� ��ȯ
	const SkillSettingMap* GetSkillSetting( DWORD level )	const;

	// 080417 LUJ, �ִ� ��� ������ ��ų ������ ��´�
	const SkillSettingMap* GetMaxLevelSkillSetting()		const;

private:
	SkillSettingLevelMap mSkillSettingLevelMap;


	// 080417 LUJ, ��� ��ų ����
private:
	// 080417 LUJ, ������ ��ų�� ��´�. Ű: ��ų �ε���, ��: ��ų ����
	typedef stdext::hash_map< DWORD, BYTE >	SkillMap;
	SkillMap								mSkillMap;

public:
	// ������ ��� ��ų�� �÷��̾� ���¿� ���Ͽ� �����Ѵ�
	void UpdateSkill();

	// 080417 LUJ, ��ų�� ���ٸ� 0�� ��ȯ�ȴ�
	BYTE			GetSkillLevel( DWORD index )	const;
	inline DWORD	GetSkillSize()					const	{ return mSkillMap.size(); }
};

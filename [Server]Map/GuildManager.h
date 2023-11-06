#pragma once


#define GUILDMGR CGuildManager::GetInstance()
//#define OnErr(pPlayer, Protocol, errstate) \
//if(errstate) { SendNackMsg(pPlayer, Protocol, errstate); return;}

class CGuild;
class CGuildMark;
class CItemSlot;
class CGuildScore;



enum eGuildPointPerGuildTournamentResult
{
	eGDT_32,
	eGDT_16,
	eGDT_8,
	eGDT_4,
	eGDT_3,
	eGDT_2,
	eGDT_1,
	eGDT_MAX,
};

//struct GUILDPOINT_RULE
//{
//	int NOMAL_MONSTER_HUNTCOUNT;
//	int FIELDBOSS_MONSTER_HUNTCOUNT;
//	int BOSS_MONSTER_HUNTCOUNT;
//	int QUEST_MONSTER_HUNTCOUNT;
//
//	int GUILDPOINT_SECTION[eSectionForGuildHuntedMonsterCount_MAX];
//	int GUILDPOINT_GT_RESULT[eGDT_MAX];
//};


//=============GuildPlusTime=================
//enum GuildPlusTimeKind
//{
//	eSuRyun_OneHalf,
//	eMuGong_OneHalf,
//	eExp_OneHalf,
//	eDamageUp_20Pct,
//	eGuildPlusTime_Max,
//};

//struct GUILDPLUSTIME_INFO
//{
//	DWORD PlusTimeIdx;
//	DWORD PlusTimeKind;
//	char PlustimeName[MAX_ITEMNAME_LENGTH+1];
//	DWORD UsableTimeLen;
//	DWORD NeedPoint;
//	DWORD AddData;
//};

//===============================================


class CGuildManager  
{
	//friend class cGuildScore;

	//enum eRank
	//{
	//	eRankNone,
	//	eRankMaster,
	//	eRankVice,
	//	eRankFirst,
	//	eRankSecond,
	//	eRankThird,
	//	eRankApprentice,		
	//};

	//SW060719 ��������Ʈ
	//GUILDPOINT_RULE	m_GuildPointRule;
	//DWORD	m_CHECKTIMELEN_GuildHuntedMonsterCounter;	//�� ���� ���ͻ��ī��Ʈ DB���� �ֱ�
	//DWORD	m_dwLastDBUpdateTime;	//���� ���� �ð�
	//WORD	m_wCurDateCheckForConverCount;	//���� ��� ����Ʈ ������ ���� ��¥ üũ.

	//DWORD	m_CheckTimeForGuildPlustimeEnd;

	//GUILDPLUSTIME_INFO m_pGuildPlustimeInfo[eGuildPlusTime_Max];
	//GUILDPLUSTIME_INFO* m_pGuildPlustimeInfo;

	//cPtrList	m_GuildPlustimeUsingGuildList;


	CYHHashTable<CGuild> m_GuildRoomHashTable;
	CYHHashTable<CGuildMark> m_GuildMarkTable;
	
	//MONEYTYPE m_LevelUpInfo[MAX_GUILD_STEP];
	CGuildManager();
	BOOL IsVaildGuildName( const char* );

	BYTE DoBreakUp(DWORD GuildIdx);
	void RegistMarkSyn(DWORD GuildIdx, char* ImgData, DWORD PlayerIdx);
	void RequestMark(CPlayer* pPlayer, MARKNAMETYPE MarkName);
	int convertCharToInt(char c);
	BYTE HexToByte(char* pStr);
	//void LoadLevelUpInfo();
	//MONEYTYPE GetLevelUpMoney(BYTE GuildLevel);
	BOOL CanEntryGuild(char* date);
public:
	//GETINSTANCE(CGuildManager);
	static CGuildManager* GetInstance();
	virtual ~CGuildManager();

	void NetworkMsgParse( BYTE Protocol, void* pMsg );

	CGuild* RegistGuild( const GUILDINFO& pGuildInfo, MONEYTYPE GuildMoney );
	//void RegistGuildItem(DWORD GuildIdx, ITEMBASE* Guilditem);
	void LoadMembers(GUILDMEMBERINFO_ID* pInfo);
	void LoadMark(MARKNAMETYPE MarkName, char* pImgData);

	void UserLogIn(CPlayer* pPlayer);
	void UserLogOut(CPlayer* pPlayer);

	void CreateGuildSyn( CPlayer*,  const char* GuildName, const char* Intro);
	void CreateGuildResult(LEVELTYPE MasterLvl, const GUILDINFO& pGuildInfo);

	void BreakUpGuildSyn(CPlayer* pMaster);
	void BreakUpGuildResult(CPlayer* pMaster, DWORD GuildIdx, char* EntryDate);

	//void DeleteMemberSyn(CPlayer* pMaster, DWORD MemberIDX);
	BYTE DoDeleteMember(DWORD GuildIDX, DWORD MemberIDX, BYTE bType, char* EntryDay);
	void DeleteMemberResult(DWORD GuildIDX, DWORD MemberIDX, char* EntryDay);

	void AddMemberSyn(CPlayer* pMaster, DWORD TargetIdx);
	void AddMemberResult(CPlayer* pTarget, DWORD MasterIdx);

	CGuild* GetGuild(DWORD GuildIdx);
	CGuild* GetGuildFromMasterIdx( DWORD dwMasterIdx );
	CGuildMark* GetMark(MARKNAMETYPE MarkName);

	void SendNackMsg(CPlayer* pPlayer, BYTE Protocol, BYTE errstate);

	void SecedeSyn(CPlayer* pPlayer);
	void SecedeResult(DWORD GuildIdx, DWORD MemberIDX, char* EntryDay);

	void SendChatMsg(DWORD GuildIdx, DWORD PlayerIdx, char* pMsg);

	void RegistMarkResult(CPlayer* pPlayer, DWORD GuildIdx, MARKNAMETYPE MarkName, char * pImgData);
	void DoRegistMark(CGuild* pGuild, MARKNAMETYPE MarkName, char* imgData);

	void DeleteMark(MARKNAMETYPE MarkName);
	void MarkChange(CPlayer* pPlayer, DWORD GuildIdx, MARKNAMETYPE MarkName);

	//void LevelUpSyn(CPlayer* pPlayer);

	void SendGuildEntryDate(CPlayer* pPlayer, char* day);

	void ChangeMemberRank(CPlayer* pMaster, DWORD MemberIdx, BYTE Rank);

	void GiveNickNameSyn(CPlayer* pMaster, DWORD TargetId, char* NickName);

	CItemSlot* GetSlot(DWORD GuildIdx);
	//void GuildWarehouseInfo(CPlayer* pPlayer, BYTE TabNum);
	//void LeaveWareHouse(CPlayer* pPlayer, BYTE TabNum);

	void MemberLevelUp(DWORD GuildIdx, DWORD PlayerIdx, LEVELTYPE PlayerLvl);

	void SendGuildName(CPlayer* pPlayer, DWORD dwGuildIdx, const char * GuildName); // ban, secede, breakup �ô� �Ⱥ���. ��� �̸��� �����Ƿ� Ŭ���̾�Ʈ���� markchange 0 ���� ó��

	void SetLogInfo(CGuild* pGuild, DWORD PlayerIdx, BOOL vals);

	BOOL	CheckHack( CPlayer* pPlayer ); 

	void	MsgChangeLocationSyn( void* pMsg );
	void	MsgChangeLocationNotifyToMap( void* pMsg );
	void	MsgGuildNoteSyn( void* pMsg );
	
	void	AddStudentResult(CPlayer* pTarget, DWORD MasterID);

	//SW060719 ��������Ʈ
	//BOOL	LoadGuildPointRule();
	//GUILDPLUSTIME_INFO* GetGuildPointPlustimeList()	{	return m_pGuildPlustimeInfo;	}

	void	AddHuntedMonster(CPlayer* pPlayer, CObject* pHuntedMonster);
	
	void	Process();
	//void	GUILDMGR_5MinProcess();

	void	GuildHuntedMonsterCountDBUpdate();
	void	CheckDateForGuildHuntedMonsterCountConvertToGuildPoint();
	//void	CheckGuildPlustimeEnd();

	DWORD	GetMasterIndex( const char* masterName );


	// 070731 ����, ���� ���
	// ���� ��� �� �� ���� ����� �����͸� ���� ���� ������, ���� �������� �÷��̾� ������ ������ ���� ������ �����Ѵ�.
	// �׸��� ���ν����� ȣ���Ͽ� DB�� ���� �����ش�. �ڼ��� ���� CGuildScore ����
public:	
	void AddPlayer( CPlayer* );
	void RemovePlayer( const CPlayer* );

	CGuildScore* GetGuildScore( DWORD guildIndex );

private:
	// ��� ������ ����
	typedef stdext::hash_map< DWORD, CGuildScore* >	GuildMap;
	GuildMap										mGuildScoreMap;

	// �ð� ������ ó���� ��� ��ȣ�� ����
	typedef std::list< DWORD >						TimeList;
	TimeList										mTimeList;


	// 070801 ����, ��� ������ �о���δ�.
private:
	void LoadScript();
	void ParseScoreScript( const std::list< std::string >& );
	void ParseLevelScript( const std::list< std::string >& );
	// 080417 LUJ, ��� ��ų ��ũ��Ʈ �ε�
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
	const ScoreSetting& GetScoreSetting() const;

	// 071012 ����, ���� ������ �� ���� ������ ��ȯ�Ѵ�.
	float GetLosingScore( float score ) const;


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
	const SkillSettingMap* GetSkillSetting( DWORD level ) const;

	// 080417 LUJ, �ִ� ��� ������ ��ų ������ ��´�
	const SkillSettingMap* GetMaxLevelSkillSetting() const;

private:
	SkillSettingLevelMap mSkillSettingLevelMap;
};

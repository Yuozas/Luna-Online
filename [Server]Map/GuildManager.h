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

	//SW060719 문파포인트
	//GUILDPOINT_RULE	m_GuildPointRule;
	//DWORD	m_CHECKTIMELEN_GuildHuntedMonsterCounter;	//맵 누적 몬스터사냥카운트 DB갱신 주기
	//DWORD	m_dwLastDBUpdateTime;	//이전 갱신 시각
	//WORD	m_wCurDateCheckForConverCount;	//몬스터 사냥 포인트 정산을 위한 날짜 체크.

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

	void SendGuildName(CPlayer* pPlayer, DWORD dwGuildIdx, const char * GuildName); // ban, secede, breakup 시는 안보냄. 길드 이름이 없으므로 클라이언트에서 markchange 0 에서 처리

	void SetLogInfo(CGuild* pGuild, DWORD PlayerIdx, BOOL vals);

	BOOL	CheckHack( CPlayer* pPlayer ); 

	void	MsgChangeLocationSyn( void* pMsg );
	void	MsgChangeLocationNotifyToMap( void* pMsg );
	void	MsgGuildNoteSyn( void* pMsg );
	
	void	AddStudentResult(CPlayer* pTarget, DWORD MasterID);

	//SW060719 문파포인트
	//BOOL	LoadGuildPointRule();
	//GUILDPLUSTIME_INFO* GetGuildPointPlustimeList()	{	return m_pGuildPlustimeInfo;	}

	void	AddHuntedMonster(CPlayer* pPlayer, CObject* pHuntedMonster);
	
	void	Process();
	//void	GUILDMGR_5MinProcess();

	void	GuildHuntedMonsterCountDBUpdate();
	void	CheckDateForGuildHuntedMonsterCountConvertToGuildPoint();
	//void	CheckGuildPlustimeEnd();

	DWORD	GetMasterIndex( const char* masterName );


	// 070731 웅주, 점수 계산
	// 점수 계산 시 각 맵은 동기된 데이터를 갖고 있지 않으며, 각각 개별적인 플레이어 정보를 가지고 더할 점수를 산출한다.
	// 그리고 프로시저를 호출하여 DB에 값을 더해준다. 자세한 것은 CGuildScore 참조
public:	
	void AddPlayer( CPlayer* );
	void RemovePlayer( const CPlayer* );

	CGuildScore* GetGuildScore( DWORD guildIndex );

private:
	// 길드 정보가 보관
	typedef stdext::hash_map< DWORD, CGuildScore* >	GuildMap;
	GuildMap										mGuildScoreMap;

	// 시간 순으로 처리할 길드 번호가 보관
	typedef std::list< DWORD >						TimeList;
	TimeList										mTimeList;


	// 070801 웅주, 길드 정보를 읽어들인다.
private:
	void LoadScript();
	void ParseScoreScript( const std::list< std::string >& );
	void ParseLevelScript( const std::list< std::string >& );
	// 080417 LUJ, 길드 스킬 스크립트 로딩
	void ParseSkillScript( const std::list< std::string >& );

public:
	struct ScoreSetting
	{
		DWORD	mUnitTime;		// 접속 단위 시간
		float	mUnitTimeScore;	// 접속 단위 시간 당 얻는 점수
		float	mHuntScore;		// 몬스터 사냥시마다 얻는 점수
		DWORD	mUpdateGap;		// DB에 갱신하고 길드원에게 브로드캐스팅하는 주기. 밀리초 단위. 너무 잦으면 부하발생...
		float	mKillerScore;	// 다른 길드원을 살해앴을 때 얻는 점수
		float	mCorpseScore;	// 다른 길드원에게 살해되었을 때 잃는 점수

		// 길드전 승리시 얻는 점수
		// 주의: mPercent는 패배 길드가 잃는 점수의 총합에서 승리 길드가 획득하는 비율을 뜻한다.
		struct Unit
		{
			float mValue;
			float mPercent;
		}
		mWinScore;;

		float	mWinMoneyRate;	// 선포금에서 획득하는 비율

		// 길드전 패배시 잃는 점수
		typedef std::map< DWORD, Unit >		LostScoreMap;
		LostScoreMap						mLostScoreMap;
	};

private:
	ScoreSetting mScoreSetting;


public:
	const ScoreSetting& GetScoreSetting() const;

	// 071012 웅주, 현재 점수로 몇 점을 잃을지 반환한다.
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
		// 080417 LUJ, 획득 가능한 길드 스킬 개수
		DWORD	mSkillSize;
	};

	const LevelSetting* GetLevelSetting( DWORD level ) const;


private:
	// 키: 길드 레벨
	typedef stdext::hash_map< DWORD, LevelSetting >	LevelSettingMap;
	LevelSettingMap									mLevelSettingMap;


	// 080417 LUJ, 길드 스킬 정보를 읽는다
public:
	struct SkillSetting
	{
		// 080417 LUJ, 사용 가능한 최대 스킬 레벨
		BYTE				mLevel;

		// 080417 LUJ, 사용가능한 직위
		std::set< DWORD >	mRank;
	};

	// 080417 LUJ, 스킬 설정 정보가 담긴 맵, 키: 스킬 인덱스
	typedef stdext::hash_map< DWORD, SkillSetting > SkillSettingMap;

	// 080417 LUJ, 레벨 별 스킬 설정 정보가 담긴 맵. 키: 길드 레벨
	typedef stdext::hash_map< DWORD, SkillSettingMap >	SkillSettingLevelMap;

	// 080417 LUJ, 레벨별 길드 스킬 설정 반환
	const SkillSettingMap* GetSkillSetting( DWORD level ) const;

	// 080417 LUJ, 최대 길드 레벨의 스킬 정보를 얻는다
	const SkillSettingMap* GetMaxLevelSkillSetting() const;

private:
	SkillSettingLevelMap mSkillSettingLevelMap;
};

#pragma once


#include "GuildWarehouse.h"

//#define GUILD_STUDENT_NUM_MAX	25

enum RTGChangeRank
{
	eGRankPos_1,			//@�ι���
	eGRankPos_2,			//@���1
	eGRankPos_3,			//@���2
	eGRankPos_Max,
	eGRankPos_Err,
};

//enum GuildPlusTimeKind
//{
//	eSuRyun_OneHalf,
//	eSkill_OneHalf,
//	eExp_OneHalf,
//	eDamageUp_20Pct,
//	eGuildPlusTime_Max,
//};
enum eGuildPointUseKind{ForGuildPlusTime, ForGuildSkill,};

//struct GUILDUSING_PLUSTIME_INFO
//{
//	//int PlusTimeKind;
//	DWORD PlusTimeEndTime;
//};

class CGuild
{
	GUILDINFO m_GuildInfo;
	cPtrList m_MemberList;
	MARKNAMETYPE m_MarkName;
	
	DWORD m_RankMemberIdx[eGRankPos_Max]; // m_RankMemberIdx[0] : vice master, m_RankMemberIdx[1],m_RankMemberIdx[2] : seniors  

	//SW060719 ��������Ʈ
	//GUILDPOINT_INFO m_GuildPoint;
	DWORD	m_nMemberOnConnectingThisMap;

	//GUILDPLUSTIME_INFO m_GuildUsingPlusTime[eGuildPlusTime_Max];
		
	CGuildWarehouse m_GuildWare;

	// GuilTournament Info
	DWORD		m_GTBattleID;

	//BOOL	m_bItemInfoInited;
	//BOOL	m_bWaitingItemInfoFromDB;	//SW �� �ѹ��� Query..(DB������ ���� ��� ���)

	//SW060713 ���ϻ�
	DWORD	m_nStudentCount;

public:
	CGuild( const GUILDINFO& pInfo, MONEYTYPE GuildMoney);
	virtual ~CGuild();

	DWORD GetIdx() const;

	void InitGuildItem( const ITEMBASE& );
	
	BOOL IsMaster(DWORD PlayerIDX) const;
	BOOL IsMember(DWORD MemberIDX);
	BOOL IsViceMaster(DWORD PlayerIDX);
	
	BOOL AddMember( GUILDMEMBERINFO* );
	BOOL DeleteMember(DWORD PlayerIDX, BYTE bType);
	char* GetGuildName() { return m_GuildInfo.GuildName;	}
	
	void BreakUp();

	GUILDINFO& GetInfo();
	const GUILDINFO& GetInfo() const;

	void SendMsgToAll(MSGBASE* msg, int size);
	void SendMsgToAllExceptOne(MSGBASE* msg, int size, DWORD CharacterIdx);
	void SendDeleteMsg(DWORD PlayerIDX, BYTE bType);
	
	void SendNote(CPlayer *pPlayer, char* toName, char* note);
	void SendAllNote(CPlayer *pPlayer, char* note);
	void SendUnionNote(CPlayer *pPlayer, char* note);
	
	MARKNAMETYPE GetMarkName();
	void SetMarkName(MARKNAMETYPE name);
	
	DWORD GetMasterIdx();
	int GetMemberNum() { return m_MemberList.GetCount();	}
	void GetTotalMember(GUILDMEMBERINFO* rtInfo);
	BYTE GetLevel();
	DWORD GetMaxMemberNum() const;

	GUILDMEMBERINFO* GetMemberInfo(DWORD MemberIdx);
	int IsVacancy(BYTE ToRank);
	
	BOOL CanAddMember() const;
	
	BOOL ChangeMemberRank(DWORD MemberIdx, BYTE ToRank);
	
	void ResetRankMemberInfo(DWORD MemberIdx, BYTE Rank);
	CItemSlot* GetSlot();
	
	MAPTYPE GetLocation();
	void	SetLocation( MAPTYPE MapNum )	{ m_GuildInfo.MapNum = MapNum; }
	
	MONEYTYPE GetMaxPurseMoney();
	
	void SetMemberLevel(DWORD PlayerIdx, LEVELTYPE PlayerLvl);
	void SetLogInfo(DWORD PlayerIdx, BOOL vals);  // send msg to all member, broadcasting to other mapserver

	// 080225 LUJ, ȸ�� ������ �����Ѵ�
	BOOL SetMember( const GUILDMEMBERINFO& );

	void LevelUp();
	void LevelDown();
	
	// GuilTournament Info
	void SetGTBattleID( DWORD BattleID )			{	m_GTBattleID = BattleID;	}
	DWORD GetGTBattleID()							{	return m_GTBattleID;		}

	void	SetGuildUnionInfo( DWORD dwGuildUnionIdx, char* pGuildUnionName, DWORD dwGuildUnionMarkIdx, BOOL isMaster );
	void	SetGuildUnionIdx( DWORD dwGuildUnionIdx )	{ m_GuildInfo.UnionIdx = dwGuildUnionIdx; }
	void	SetUnionMarkIndex( DWORD dwMarkIdx );
	DWORD	GetUnionIndex()							{ return m_GuildInfo.UnionIdx; }
	
	BOOL	IsEmptyGuildWare();

	CGuildWarehouse* GetWarehouse()	{	return &m_GuildWare;	}

	//SW060713 ���ϻ�
	DWORD	GetStudentNum() const { return m_nStudentCount; }
    BOOL	CanAddStudent() const;

	void	AddConnectingMapMemberCount(int Val);
	int		GetConnectingMapMemberCount()	{	return m_nMemberOnConnectingThisMap;	}

	void	SetZeroMoney();		//KES 070604 : from DarkStory
	void	SetMasterIndex( DWORD masterIndex );

	// 080108 LUJ, ������ ������ �ʱ�ȭ�Ѵ�
	void	ClearItem();


	// 080417 LUJ, ��� ��ų ����
private:
	// 080417 LUJ, ������ ��ų�� ��´�. Ű: ��ų �ε���, ��: ��ų ����
	typedef stdext::hash_map< DWORD, BYTE >	SkillMap;
	SkillMap								mSkillMap;

public:
	void			AddSkill		( DWORD index, BYTE level );
	void			RemoveSkill		( DWORD index );
	inline	void	RemoveAllSkill	()						{ mSkillMap.clear(); }
	// 080417 LUJ, ��尡 ������ ��ų�� �ش� �÷��̾�� �����Ѵ�
	void			SendSkill		( CObject& )	const;
	// 080417 LUJ, ��ų�� ���ٸ� 0�� ��ȯ�ȴ�
	BYTE			GetSkillLevel	( DWORD index )	const;
	inline DWORD	GetSkillSize	()				const	{ return mSkillMap.size(); }

	// 080417 LUJ, �ش� �÷��̾��� ��� ��ų�� �����Ѵ�
	void UpdateSkill( DWORD playerIndex );	
};

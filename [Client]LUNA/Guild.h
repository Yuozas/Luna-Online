#pragma once


using namespace std;

#define GUILD_STUDENT_NUM_MAX	25

enum RTChangeRank
{
	eRankPos_1,
	eRankPos_2,
	eRankPos_3,
	eRankPos_Max,
	eRankPos_Err,
};

enum sortype
{
	eRank,
	eName,
	eLevel,
	eMax
};

typedef BYTE RANK;

class CGuild  
{
	DWORD m_GuildIdx;
	BYTE m_GuildLvl;
	BYTE m_GuildLocation;
	char m_MasterName[MAX_NAME_LENGTH+1];
	char m_GuildName[MAX_GUILD_NAME+1];
	//char m_GuildNotice[MAX_GUILD_NOTICE+1];
	DWORD m_UnionIdx;

	DWORD m_RankMemberIdx[eRankPos_Max];
	
	cPtrList m_MemberList;

	BYTE m_SortType;
	multimap< RANK, GUILDMEMBERINFO* > m_MemberMultiMap;
		
	//SW060713 ���ϻ�
	DWORD	m_nStudentCount;	//���ϻ� ���� ī����

	//SW060719 ��������Ʈ
	//GUILDPOINT_INFO m_GuildPoint;
public:
	CGuild();
	virtual ~CGuild();
	
	void Init(GUILDINFO* pInfo);
	void Release();

	BOOL DeleteMember(DWORD PlayerIDX);
	BOOL AddMember(GUILDMEMBERINFO* pMemberInfo);
	BOOL IsMember(DWORD PlayerIDX);
	void SetTotalInfo(GUILDINFO* pInfo, GUILDMEMBERINFO* pMemberInfo, int MemberNum);
	
	DWORD GetGuildIdx()	const	{ return m_GuildIdx; }
	char* GetGuildName() 		{ return m_GuildName; }
	BYTE GetLevel()	const		{ return m_GuildLvl; }
	char* GetMasterName()		{ return m_MasterName; }
	BYTE GetMemberSize()			{ return m_MemberList.GetCount(); }
	cPtrList& GetMemberList()	{ return m_MemberList; }
	MAPTYPE GetLocation()		{ return m_GuildLocation;	}
	void RefreshMember();
	GUILDMEMBERINFO* GetMemberInfo(DWORD PlayerIDX);
	char* GetMemberName(DWORD MemberID);
	DWORD GetMemberIdx(int num);
	void SetLevel(BYTE Level);
	//BYTE GetMaxMemberNum();
	//BOOL CanAddMember();

	int IsVacancy(BYTE ToRank);
	BOOL ChangeRank(DWORD MemberID, BYTE Rank);
	void ResetRankMemberInfo(DWORD MemberIdx, BYTE Rank);
	void SetMemberLevel(DWORD MemberIdx, DWORD MemberLvl);	
	void SetLogInfo(DWORD MemberIdx, BOOL vals);

	//void SetSortType(BYTE type);
	//BYTE GetSortType();
	//void SortbyType();
	
	void	SetGuildPosition( DWORD dwMapnum );

	// 06. 03. ���İ��� - �̿���
	//void SetGuildNotice(char* str) { strcpy(m_GuildNotice, str); }
	//char* GetGuildNotice()
	//{ 
	//	if(m_GuildNotice[0])
	//		return m_GuildNotice; 
	//	else
	//		return NULL;
	//}

	//SW060713 ���ϻ�
	DWORD GetStudentNum() { return m_nStudentCount; }
	BOOL CanAddStudent();

	//SW060719 ��������Ʈ
	//void AddGuildPlustime(DWORD plustimeIdx, int plustimeKind, DWORD endTime);
	//void AddGuildPlustime(DWORD plustimeKind, DWORD plustimeIdx, DWORD endTime);
	//void RemoveGuildPlustime(DWORD plustimeKind);
	//BOOL IsUsingPlustime(DWORD plustimeIdx);
	//GUILDPOINT_INFO* GetGuildPointInfo()	{	return &m_GuildPoint;	}
	
	//void	SetGuildPointInfo(GUILDPOINT_INFO* pGuildPointInfo);
	//void	SetGuildPoint(int TotalPoint);
	//int		GetGuildPoint()	{	return m_GuildPoint.GuildPoint;	}
};
// GuildTournamentMgr.h: interface for the CGuildTournamentMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDTOURNAMENTMGR_H__21A71231_1238_4D1F_99B6_D53A80C23436__INCLUDED_)
#define AFX_GUILDTOURNAMENTMGR_H__21A71231_1238_4D1F_99B6_D53A80C23436__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\[CC]BattleSystem\GTournament\Battle_GTournament.h"

#define GTMGR CGuildTournamentMgr::GetInstance()
#define GTMAPNUM		94
#define GTRETURNMAPNUM	52

#define GT_NOTIFY_STARTTIME			(60000 * 5)
#define GT_NOTIFY_INTERVAL			60000

#define GT_CONVERT_DAY2MS(day)		(60000 * 60 * 24 * day)
#define GT_CONVERT_HOUR2MS(hour)	(60000 * 60 * hour)
#define GT_CONVERT_MIN2MS(min)		(60000 * min)
#define GT_CONVERT_SEC2MS(sec)		(1000 * sec)

#define GT_WAITING_NONE			0x00000000
#define GT_WAITING_INFOLOAD		0x00000001
#define GT_WAITING_INFOLOADALL	0x00000010
#define GT_WAITING_PLAYERLOAD	0x00000100

enum {
	eGTTime_OP_Add = 0,
	eGTTime_OP_Sub,
};

class CGuild;
class CPlayer;


struct GTInfo
{
	CGuild* pGuild;
	WORD	Ranking;
	WORD	Position;
	WORD	ProcessTournament;		// 어디까지 경기를 진행했는지
};


class CGuildTournamentMgr
{
	CYHHashTable<CGuild>		m_TournamentCancelList;

	GTInfo						m_GuildInfoList[MAXGUILD_INTOURNAMENT];
	int							m_PositionTable[2][MAXGUILD_INTOURNAMENT/2];
	BATTLE_INFO_GTOURNAMENT		m_BattleInfo[MAXGUILD_INTOURNAMENT/2];
	DWORD						m_BattleCount;
	stTournamentTime			m_RegistTime[2];
	stTournamentTime			m_TournamentTime[30][eGTT_MAX];
	DWORD						m_RegistedGuildCount;
	DWORD						BattleCount[eGTT_MAX];

	DWORD		m_TournamentCount;			// 몇차 토너먼트인가

	DWORD		m_MaxGroup;
	DWORD		m_CurTournament;			// 몇강경기인가
	DWORD		m_TournamentState;			// 토너먼트 진행상황

	DWORD		m_EntranceTime;
	DWORD		m_NotifyEntranceTime;
	DWORD		m_FightTime;
	DWORD		m_LeaveTime;
	DWORD		m_RemainTime;
	
	stTIME		m_WaitTime;
	
	BOOL		m_Inited;

	//
	int GetEmptyGuildPosition();

public:
	MAKESINGLETON( CGuildTournamentMgr );

	CGuildTournamentMgr();
	virtual ~CGuildTournamentMgr();

	void Init();
	void Reset();
	void Process();
	
	DWORD AddGuild( CGuild* pGuild, DWORD& Pos );
	DWORD AddedGuild( CGuild* pGuild, DWORD Position );
	void AddPlayer( CPlayer* pPlayer );
	DWORD DeleteAddedGuild(	DWORD GuildIdx );

	DWORD IsRegistedGuid( DWORD GuildIdx );
	int GetCurTournamentGuild( DWORD Group, BOOL bFront );

	void CreateBattle();
	void StartTournament();
	void NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol,void* pMsg );

	DWORD GetRanking( DWORD Ranking, BOOL bWin );
	int GetBattleID( CGuild* pGuild );
	void SetResult( DWORD GuildIdx, BOOL bWin, BOOL bUnearnedWin=FALSE );

	// DB
	void SetTournamentInfo( DWORD Tournament, DWORD CurTournament, DWORD WaitTime );
	void SetGuildInfo( GTDBLoadInfo* pGuildInfo, DWORD Count );
	void AddCancelGuild( DWORD GuildIdx );
};

#endif // !defined(AFX_GUILDTOURNAMENTMGR_H__21A71231_1238_4D1F_99B6_D53A80C23436__INCLUDED_)

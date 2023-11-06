#include "stdafx.h"
#include "Guild.h"
#include "Player.h"
#include "UserTable.h"
#include "GuildManager.h"
#include "GuildScore.h"
#include "MapDBMsgParser.h"
#include "NetWork.h"
// 080417 LUJ, 길드 스킬위해 참조
#include "../[cc]skill/server/manager/skillmanager.h"
#include "../[cc]skill/Server/Info/SkillInfo.h"


CGuild::CGuild( const GUILDINFO& info, MONEYTYPE GuildMoney) :
m_GuildInfo( info ),
m_GTBattleID( 0 ),
m_nStudentCount( 0 ),
m_nMemberOnConnectingThisMap( 0 ),
m_MarkName( info.MarkName )
{
	ZeroMemory( &m_RankMemberIdx, sizeof( m_RankMemberIdx ) );
	
	m_GuildWare.Init(0);
}


CGuild::~CGuild()
{
//KES delete추가 MEM
	PTRLISTPOS pos = m_MemberList.GetHeadPosition();
	GUILDMEMBERINFO* pInfo = NULL;
	while(pos)
	{
		pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);
		SAFE_DELETE(pInfo);
	}
//-------
	//m_MemberList.RemoveAll();
}


BOOL CGuild::IsMaster(DWORD PlayerIDX) const
{
	return m_GuildInfo.MasterIdx == PlayerIDX;
}

BOOL CGuild::IsViceMaster(DWORD PlayerIDX)
{
	return m_RankMemberIdx[0] == PlayerIDX;
}

void CGuild::BreakUp()
{
// set member info clear
	CPlayer* pPlayer = NULL;
	MSGBASE msg;
	msg.Category	= MP_GUILD;
	msg.Protocol	= MP_GUILD_BREAKUP_ACK;
	
	PTRLISTPOS pos = m_MemberList.GetHeadPosition();
	
	while(pos)
	{
		GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);
		if( pInfo )
		{
			pPlayer = (CPlayer*)g_pUserTable->FindUser(pInfo->MemberIdx);
			if(pPlayer)
			{
				pPlayer->SetGuildInfo(0, GUILD_NOTMEMBER, "", 0);
				pPlayer->SetNickName("");
				pPlayer->SendMsg(&msg, sizeof(msg));
				if(GetMarkName())
					GUILDMGR->MarkChange(pPlayer, 0, 0);
				else
					GUILDMGR->SendGuildName(pPlayer, 0, "");

				// 080417 LUJ, 보유한 길드 스킬을 갱신한다
				UpdateSkill( pInfo->MemberIdx );
			}
			
		}

		SAFE_DELETE(pInfo);
	}
	m_MemberList.RemoveAll();

	// 080417 LUJ, 보유 스킬을 초기화한다
	mSkillMap.clear();
}


BOOL CGuild::AddMember(GUILDMEMBERINFO* pMemberInfo)
{
	PTRLISTPOS pos = m_MemberList.GetHeadPosition();
	GUILDMEMBERINFO* pInfo = NULL;
	while(pos)
	{
		GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);

		if(pInfo == NULL) return FALSE;
 		if(pInfo->MemberIdx == pMemberInfo->MemberIdx)
 		{
 			return FALSE;
		}
	}
	
	pInfo = new GUILDMEMBERINFO;
	*pInfo = *pMemberInfo;
	
	m_MemberList.AddTail(pInfo);

	switch( pInfo->Rank )
	{
	case GUILD_VICEMASTER:
		{
			m_RankMemberIdx[0] = pInfo->MemberIdx;
			break;
		}
	case GUILD_SENIOR:
		{
			const int pos = IsVacancy(GUILD_SENIOR);

			if((pos <0) || (pos > eGRankPos_Max))
			{
				ASSERTMSG(0, "Load GuildMemberInfo - Set Ranked Member");
				return TRUE;
			}
			m_RankMemberIdx[pos] = pInfo->MemberIdx;

			break;
		}
	case GUILD_STUDENT:
		{
			++m_nStudentCount;
			break;
		}
	}

	// 080417 LUJ, 보유한 길드 스킬을 갱신한다
	UpdateSkill( pMemberInfo->MemberIdx );

	{
		SEND_GUILD_MEMBER_INFO message;
		message.Category	= MP_GUILD;
		message.Protocol	= MP_GUILD_ADDMEMBER_ACK;
		message.GuildIdx	= GetIdx();
		message.MemberInfo	= *pMemberInfo;
	
		SendMsgToAllExceptOne(&message, sizeof(message), pMemberInfo->MemberIdx );
	}
	return TRUE;
}

BOOL CGuild::DeleteMember(DWORD PlayerIDX, BYTE bType)
{
	CPlayer* pPlayer = NULL;
	PTRLISTPOS pos = m_MemberList.GetHeadPosition();
	GUILDMEMBERINFO* pInfo = NULL;

	while(pos)
	{
		pInfo = (GUILDMEMBERINFO*)m_MemberList.GetAt(pos);
		if(pInfo)
		{
			if(pInfo->MemberIdx == PlayerIDX)
			{
				if(pInfo->Rank == GUILD_STUDENT)
				{
					--m_nStudentCount;
				}

				ResetRankMemberInfo(PlayerIDX, pInfo->Rank);
				SAFE_DELETE(pInfo);
				m_MemberList.RemoveAt(pos);

				// 080417 LUJ, 보유한 길드 스킬을 갱신한다
				UpdateSkill( PlayerIDX );
				return TRUE;
			}
		}
		m_MemberList.GetNext(pos);
	}

	return FALSE;
}

void CGuild::SendDeleteMsg(DWORD PlayerIDX, BYTE bType)
{
	MSG_DWORDBYTE msg;
	msg.Category	= MP_GUILD;
	msg.Protocol	= MP_GUILD_DELETEMEMBER_ACK;
	msg.dwData		= PlayerIDX;
	msg.bData		= bType;
	SendMsgToAll(&msg, sizeof(msg));
}


void CGuild::SendMsgToAll(MSGBASE* msg, int size)
{
	CPlayer* pPlayer = NULL;
	
	PTRLISTPOS pos = m_MemberList.GetHeadPosition();

	while(pos)
	{
		GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);

		if( pInfo )
		{
			pPlayer = (CPlayer*)g_pUserTable->FindUser(pInfo->MemberIdx);
			if(pPlayer)
			{
				pPlayer->SendMsg(msg, size);
			}
		}
	}
}

void CGuild::SendMsgToAllExceptOne(MSGBASE* msg, int size, DWORD CharacterIdx)
{
	CPlayer* pPlayer = NULL;

	PTRLISTPOS pos = m_MemberList.GetHeadPosition();
	while(pos)
	{
		GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);
		
		if( pInfo )
		{
			pPlayer = (CPlayer*)g_pUserTable->FindUser(pInfo->MemberIdx);
			if(pPlayer)
			{
				if(pPlayer->GetID() == CharacterIdx)
					continue;
				pPlayer->SendMsg(msg, size);
			}
		}
	}
}

BOOL CGuild::IsMember(DWORD MemberIDX)
{
	CPlayer* pMember = (CPlayer*)g_pUserTable->FindUser(MemberIDX);
	if(pMember)
	{
		if(pMember->GetGuildIdx() == GetIdx())
			return TRUE;
	}
	else
	{
		PTRLISTPOS pos = m_MemberList.GetHeadPosition();
		while(pos)
		{
			GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);

			if(pInfo)
			if(pInfo->MemberIdx == MemberIDX)
				return TRUE;
		}
	}
	return FALSE;
}

DWORD CGuild::GetIdx() const
{
	return m_GuildInfo.GuildIdx;
}

void CGuild::GetTotalMember(GUILDMEMBERINFO* pRtInfo)
{
	int i=0;

	for(
		PTRLISTPOS pos = m_MemberList.GetHeadPosition();
		pos;
		)
	{
		const GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);

		if( ! pInfo )
		{
			continue;
		}

		if( i == MAX_GUILD_MEMBER )
		{
			break;
		}

		pRtInfo[ i++ ] = *pInfo;
	}
}


MARKNAMETYPE CGuild::GetMarkName()
{
	return m_MarkName;
}

void CGuild::SetMarkName(MARKNAMETYPE name)
{
	m_MarkName = name;
}

BYTE CGuild::GetLevel()
{
	return m_GuildInfo.GuildLevel;
}

DWORD CGuild::GetMasterIdx()
{
	return m_GuildInfo.MasterIdx;
}

DWORD CGuild::GetMaxMemberNum() const
{
	const CGuildScore* guild = GUILDMGR->GetGuildScore( m_GuildInfo.GuildIdx );

	if( guild )
	{
		const CGuildManager::LevelSetting* setting = GUILDMGR->GetLevelSetting( guild->GetLevel() );

		return setting ? setting->mMemberSize : 0;
	}

	return 0;

	//if(( m_GuildInfo.GuildLevel <= 0) || ( m_GuildInfo.GuildLevel > GUILD_5LEVEL))
	//{
	//	char buf[64];
	//	sprintf(buf, "Wrong Member Num, GuildIdx : %u", m_GuildInfo.GuildIdx);
	//	ASSERTMSG(0, buf);
	//	return 0;
	//}

	//return 10 + ( (m_GuildInfo.GuildLevel-1) * 5 ); 
}

BOOL CGuild::CanAddMember() const
{	
	const CGuildScore* guild = GUILDMGR->GetGuildScore( m_GuildInfo.GuildIdx );

	if( ! guild )
	{
		return FALSE;
	}

	const CGuildManager::LevelSetting*	setting = GUILDMGR->GetLevelSetting( guild->GetLevel() );
	const DWORD							size	= ( setting ? setting->mMemberSize : 0 );

	return size > ( DWORD( m_MemberList.GetCount() ) - m_nStudentCount );

	//if( GetMemberNum() - GetStudentNum() < GetMaxMemberNum() )
	//	return TRUE;
	//else
	//	return FALSE;
}

BOOL CGuild::ChangeMemberRank(DWORD MemberIdx, BYTE ToRank)
{
	{
		GUILDMEMBERINFO* pMemberInfo = GetMemberInfo(MemberIdx);
		if( ! pMemberInfo )
		{
			ASSERTMSG(0, "Change Rank - No Member Data");
			return FALSE;
		}
		
		ResetRankMemberInfo(MemberIdx, pMemberInfo->Rank);

		if( pMemberInfo->Rank	== GUILD_STUDENT &&
			ToRank				!= GUILD_STUDENT)
		{
			--m_nStudentCount;
		}
		
		pMemberInfo->Rank = ToRank;

		// 080417 LUJ, 보유한 길드 스킬을 갱신한다
		UpdateSkill( pMemberInfo->MemberIdx );
	}

	MSG_DWORDBYTE msg;
	msg.Category	= MP_GUILD;
	msg.Protocol	= MP_GUILD_CHANGERANK_ACK;
	msg.dwData		= MemberIdx;
	msg.bData		= ToRank;
	SendMsgToAll( &msg, sizeof( msg ) );

	return TRUE;
}

int CGuild::IsVacancy(BYTE ToRank)
{
	int end = 0;
	int start = 0;
	switch(ToRank)
	{
	case GUILD_VICEMASTER:
		{
			start = 0; end = 1;
		}
		break;
	case GUILD_SENIOR:
		{
			start = 1; end = 3;
		}
		break;
	case GUILD_MEMBER:
	case GUILD_STUDENT:
		return eGRankPos_Max;
		
	default:
		ASSERTMSG(0, "IsVancancy Err.");
		return eGRankPos_Err;
	}

	BOOL rt = eGRankPos_Err;
	for(int i=start; i<end; ++i)
	{
		if( m_RankMemberIdx[i] == 0 )
		{
			return i;
		}
	}
	return rt;
}

GUILDMEMBERINFO* CGuild::GetMemberInfo(DWORD MemberIdx)
{
	PTRLISTPOS pos = m_MemberList.GetHeadPosition();
	GUILDMEMBERINFO* pInfo = NULL;

	while(pos)
	{
		pInfo = (GUILDMEMBERINFO*)m_MemberList.GetAt(pos);
		if(pInfo)
		{
			if(pInfo->MemberIdx == MemberIdx)
			{
				return pInfo;
			}
		}
		m_MemberList.GetNext(pos);
	}
	char buf[64];
	sprintf(buf, "ERR, NO MemberInfo. GuildIdx: %d, MemberIdx: %d", GetIdx(), MemberIdx);
	ASSERTMSG(0, buf);
	return NULL;
}

//BOOL CGuild::DoChangeMemberRank(DWORD MemberIdx, int Pos, BYTE Rank)
//{
//	GUILDMEMBERINFO* pMemberInfo = GetMemberInfo(MemberIdx);
//	if(pMemberInfo == 0)
//	{
//		ASSERTMSG(0, "Change Rank - No Member Data");
//		return FALSE;
//	}
//	if((Pos <0) || (Pos >eGRankPos_Max))
//	{
//		ASSERTMSG(0, "change rank pos boundary error");
//		return FALSE;
//	}
//	ResetRankMemberInfo(MemberIdx, pMemberInfo->Rank);
//
//	if(pMemberInfo->Rank == GUILD_STUDENT && Rank != GUILD_STUDENT)
//	{
//		--m_nStudentCount;
//	}
//	//없는경우이다.
//	//else if(pMemberInfo->Rank != GUILD_STUDENT && Rank == GUILD_STUDENT)
//	//{
//	//	++m_nStudentCount;
//	//}
//	pMemberInfo->Rank = Rank;
//	//if(Pos == eGRankPos_Max)
//	//	return TRUE;
//	//m_RankMemberIdx[Pos] = MemberIdx;
//	return TRUE;
//}

void CGuild::ResetRankMemberInfo(DWORD MemberIdx, BYTE Rank)
{
	switch(Rank)
	{
	case GUILD_VICEMASTER:
		m_RankMemberIdx[0] = 0; break;
	case GUILD_SENIOR:
		{
			if(m_RankMemberIdx[1] == MemberIdx)
				m_RankMemberIdx[1] = 0;
			else if(m_RankMemberIdx[2] == MemberIdx)
				m_RankMemberIdx[2] = 0;
			else 
				ASSERTMSG(0, "ResetRankMemberInfo() - No Rank");
		}
		break;
	}
}

void CGuild::InitGuildItem( const ITEMBASE& item )
{
	m_GuildWare.InitGuildItem( const_cast< ITEMBASE* >( &item ) );
}

CItemSlot* CGuild::GetSlot()
{
	return &m_GuildWare;
}

//void CGuild::EnterWareHouse(CPlayer* pPlayer,BYTE TabNum)
//{
//	m_GuildWare.EnterWareHouse(pPlayer, TabNum);
//}
//
//void CGuild::LeaveWareHouse(DWORD PlayerID,BYTE TabNum)
//{
//	m_GuildWare.LeaveWareHouse(PlayerID, TabNum);
//}

MAPTYPE CGuild::GetLocation()
{
	return m_GuildInfo.MapNum;
}

MONEYTYPE CGuild::GetMaxPurseMoney()
{
	return m_GuildWare.GetMaxMoney();
}

//MONEYTYPE CGuild::GetMoney()
//{
//	return m_GuildWare.GetMoney();
//}
//
//MONEYTYPE CGuild::SetMoney( MONEYTYPE ChangeValue, BYTE bOper, BYTE MsgFlag )
//{
//	return m_GuildWare.SetMoney(ChangeValue, bOper, MsgFlag);
//}

//KES 070604 : from DarkStory
void CGuild::SetZeroMoney()
{
	CPurse* purse = m_GuildWare.GetPurse();
	if( purse )
		purse->SetZeroMoney();
}

//void CGuild::SendtoWarehousePlayerAll(MSGBASE* msg, int size)
//{
//	m_GuildWare.SendtoWarehousePlayerAll(msg, size);
//}

void CGuild::SetMemberLevel(DWORD PlayerIdx, LEVELTYPE PlayerLvl)
{
	GUILDMEMBERINFO* pMemberInfo = GetMemberInfo(PlayerIdx);
	if( pMemberInfo == NULL )
	{
		return;
	}

	pMemberInfo->Memberlvl = PlayerLvl;
	MSG_DWORD2 msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_MEMBERLEVEL_NOTIFY;
	msg.dwData1 = PlayerIdx;
	msg.dwData2 = PlayerLvl;
	SendMsgToAll(&msg, sizeof(msg));
}

void CGuild::SetLogInfo(DWORD PlayerIdx, BOOL vals)
{
	GUILDMEMBERINFO* pMemberInfo = GetMemberInfo(PlayerIdx);
	if( pMemberInfo == NULL )
	{
		return;
	}
	pMemberInfo->bLogged = vals;

	SEND_GUILDMEMBER_LOGININFO msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_LOGINFO;
	msg.MemberIdx = PlayerIdx;
	msg.bLogIn = vals;
	SendMsgToAllExceptOne(&msg, sizeof(msg), PlayerIdx);
}

void CGuild::LevelUp()
{
	++m_GuildInfo.GuildLevel;

	//if(m_GuildInfo.GuildLevel >= MAX_GUILD_LEVEL)
	//	return;
	//m_GuildInfo.GuildLevel += 1;

	//MSG_BYTE msg;
	//msg.Category = MP_GUILD;
	///msg.Protocol = MP_GUILD_LEVELUP_ACK;
	//msg.bData = m_GuildInfo.GuildLevel;
	//SendMsgToAll(&msg, sizeof(msg));

	//LogGuild(0, GetIdx(), eGuildLog_GuildLevelUp, GetLevel());
}

void CGuild::LevelDown()
{
	//if(m_GuildInfo.GuildLevel <= 1 )
	//	return;

	--m_GuildInfo.GuildLevel;
	
	//LogGuild(0, GetIdx(), eGuildLog_GuildLevelDown, GetLevel());
}

void CGuild::SetGuildUnionInfo( DWORD index, char* name, DWORD markIndex, BOOL isMaster )
{
	m_GuildInfo.UnionIdx		= index;
	m_GuildInfo.mIsUnionMaster	= isMaster;

	PTRLISTPOS pos = m_MemberList.GetHeadPosition();

	for( const GUILDMEMBERINFO* pInfo; pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos); )
	{
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pInfo->MemberIdx );

		if( pPlayer )
		{
			pPlayer->SetGuildUnionInfo( index, name, markIndex );
		}
	}
}

void CGuild::SetUnionMarkIndex( DWORD dwMarkIdx )
{
	CPlayer* pPlayer = NULL;
	PTRLISTPOS pos = m_MemberList.GetHeadPosition();

	while(pos)
	{
		GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);

		if( pInfo )
		{
			pPlayer = (CPlayer*)g_pUserTable->FindUser( pInfo->MemberIdx );
			if( pPlayer )
				pPlayer->SetGuildUnionMarkIdx( dwMarkIdx );
		}
	}
}

BOOL CGuild::IsEmptyGuildWare()
{
	return m_GuildWare.IsEmptyAndMoney();
}

void CGuild::SendAllNote(CPlayer *pPlayer, char* note)
{
	CPlayer* pReceiver = NULL;

	PTRLISTPOS pos = m_MemberList.GetHeadPosition();
	while(pos)
	{
		GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);
		
		if( pInfo )
		{
			SendNote(pPlayer, pInfo->MemberName, note);
		}
	}
}

void CGuild::SendUnionNote(CPlayer *pPlayer, char* note)
{
	SendNote(pPlayer, m_GuildInfo.MasterName, note);

	if(m_RankMemberIdx[0])
	{
		GUILDMEMBERINFO* pInfo = GetMemberInfo( m_RankMemberIdx[0] );
		SendNote(pPlayer, pInfo->MemberName, note);
	}
}

void CGuild::SendNote(CPlayer *pPlayer, char* toName, char* note)
{
	MSG_FRIEND_SEND_NOTE msg;
	msg.Category = MP_NOTE;
	msg.Protocol = MP_NOTE_SENDNOTE_SYN;
	msg.FromId = pPlayer->GetID();
	
	SafeStrCpy(msg.FromName, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);
	SafeStrCpy(msg.ToName, toName, MAX_NAME_LENGTH+1);
	SafeStrCpy(msg.Note, note, MAX_NOTE_LENGTH+1);	

	g_Network.Broadcast2AgentServer((char*)&msg, msg.GetMsgLength());
}

BOOL CGuild::CanAddStudent() const
{
	const CGuildScore* guild = GUILDMGR->GetGuildScore( m_GuildInfo.GuildIdx );

	if( ! guild )
	{
		return 0;
	}

	const CGuildManager::LevelSetting*	setting = GUILDMGR->GetLevelSetting( guild->GetLevel() );
	const DWORD							size	= ( setting ? setting->mApprenticeSize : 0 );

	return size > m_nStudentCount;

//#define GUILD_STUDENT_NUM_MAX	25
	//return ( GetStudentNum() <= GUILD_STUDENT_NUM_MAX );
}

//void CGuild::AddHuntedMonsterCount( int AddCount )
//{
//	m_GuildPoint.GuildHuntedMonsterCount += AddCount;
//}

//void CGuild::SetHuntedMonsterTotalCountInfo( int SetTotal, DWORD DBProcessTime )
//{
//	m_GuildPoint.GuildHuntedMonsterTotalCount = SetTotal;
//	m_GuildPoint.DBProcessTime = DBProcessTime;
//}
//
//void CGuild::InitGuildPointInfo( GUILDPOINT_INFO* pGuildPointInfo )
//{
//	//memcpy(&m_GuildPoint, pGuildPointInfo, sizeof(GUILDPOINT_INFO));
//
//	m_GuildPoint.GuildPoint = pGuildPointInfo->GuildPoint;
//	m_GuildPoint.GuildHuntedMonsterCount = 0;
//	m_GuildPoint.GuildHuntedMonsterTotalCount = pGuildPointInfo->GuildHuntedMonsterTotalCount;
//	m_GuildPoint.DBProcessTime = GetCurTime();
//	
//	//for( int plustimeKind = 0; plustimeKind < eGPT_Kind_Max; ++plustimeKind )
//	//{
//	//	AddGuildPlustime(plustimeKind, pGuildPointInfo->GuildUsingPlusTimeInfo[plustimeKind].PlusTimeEndTime);
//	//}
//
//}
//
//void CGuild::SetGuildPoint( int TotalPoint )
//{
//	m_GuildPoint.GuildPoint = TotalPoint;
//
//	MSG_DWORD msg;
//	msg.Category = MP_GUILD;
//	msg.Protocol = MP_GUILD_SCORE_UPDATE;
//	msg.dwData = TotalPoint;
//
//	SendMsgToAll(&msg, sizeof(MSG_DWORD));
//}

void CGuild::AddConnectingMapMemberCount( int Val )
{
	m_nMemberOnConnectingThisMap += Val;

	if(m_nMemberOnConnectingThisMap < 0)
	{
		ASSERT(0);
	}
}

//void CGuild::SendGuildPointAddInfoToAll( DWORD AddPoint, DWORD TotalPoint, int eAddKind, DWORD AddData )
//{
//	//맵 전파
//	MSG_DWORD4 msg;
//	msg.Category = MP_GUILD;
//	msg.Protocol = MP_GUILD_POINT_ADD;
//	msg.dwData1 = AddPoint;
//	msg.dwData2 = TotalPoint;
//	msg.dwData3 = eAddKind;
//	msg.dwData4 = AddData;
//	//pAdditionalData
//	//문원렙업 -> 레벨
//	//토너먼트 -> 순위
//	//몬스터카운트 -> 총카운트
//
//	SendMsgToAll(&msg, sizeof(MSG_DWORD4));
//}


void CGuild::SetMasterIndex( DWORD masterIndex )
{
	m_GuildInfo.MasterIdx = masterIndex;
}


GUILDINFO& CGuild::GetInfo()
{
	return m_GuildInfo;
}


const GUILDINFO& CGuild::GetInfo() const
{
	return m_GuildInfo;
}


void CGuild::ClearItem()
{
	m_GuildWare.Init( 0 );
}


// 080225 LUJ, 회원 정보를 갱신한다
BOOL CGuild::SetMember( const GUILDMEMBERINFO& member )
{
	GUILDMEMBERINFO* storedMember = GetMemberInfo( member.MemberIdx );

	if( ! storedMember )
	{
		return FALSE;
	}

	*storedMember = member;

	return TRUE;
}

// 080417 LUJ
void CGuild::AddSkill( DWORD index, BYTE level )
{	
	mSkillMap[ index ] = level;

	// 080417 LUJ,	길드원에게 세트 스킬로 세팅한다. 세트 스킬은 플레이어가 임시로 보유하는 스킬을 관리하는데 최적화되어 있기 때문
	//
	//				주의: 길드 스킬과 세트 스킬이 겹치면 안된다
	for(
		PTRLISTPOS position = m_MemberList.GetHeadPosition();
		position; )
	{
		const GUILDMEMBERINFO* info	= ( GUILDMEMBERINFO* )	m_MemberList.GetNext( position );
		
		UpdateSkill( info ? info->MemberIdx : 0 );
	}
}

// 080417 LUJ
void CGuild::RemoveSkill( DWORD index )
{
	mSkillMap.erase( index );	

	// 080417 LUJ, 길드원에게 스킬을 뺏는다
	for(
		PTRLISTPOS position = m_MemberList.GetHeadPosition();
		position; 
		)
	{
		const GUILDMEMBERINFO* info	= ( GUILDMEMBERINFO* )	m_MemberList.GetNext( position );

		UpdateSkill( info ? info->MemberIdx : 0 );
	}
}

// 080417 LUJ, 해당 플레이어의 길드 스킬을 갱신한다
void CGuild::UpdateSkill( DWORD playerIndex )
{
	CPlayer* player = ( CPlayer* )g_pUserTable->FindUser( playerIndex );

	if( ! player )
	{
		return;
	}

	typedef CGuildManager::SkillSettingMap SkillSettingMap;
	const SkillSettingMap* settingMap = GUILDMGR->GetSkillSetting( GetLevel() );

	if( ! settingMap )
	{
		settingMap = GUILDMGR->GetMaxLevelSkillSetting();

		if( ! settingMap )
		{
			return;
		}
	}

	// 080417 LUJ, 보유한 길드 스킬을 초기화한다
	{
		cSkillTree* skillTree = player->GetSkillTree();

		if( ! skillTree )
		{
			return;
		}

		// 080417 LUJ, 플레이어가 보유한 스킬이 아닌 경우 제거한다. 주의: 세트 스킬도 제거된다. 따라서 세트 스킬과 길드 스킬이 같아서는 안된다
		for(
			SkillMap::const_iterator it = mSkillMap.begin();
			mSkillMap.end() != it;
			++it )
		{
			player->RemoveSetSkill( it->first );
		}
	}

	const GUILDMEMBERINFO* member = GetMemberInfo( playerIndex );
	
	if( ! member )
	{
		return;
	}

	// 080417 LUJ, 해당 길드 스킬에 대할 설정 정보를 꺼내서 부여 조건에 해당하는지 체크한다
	for(
		SkillMap::const_iterator it = mSkillMap.begin();
		mSkillMap.end() != it;
		++it )
	{
		const DWORD index	= it->first;
		const BYTE	level	= it->second;

		SkillSettingMap::const_iterator setting_it = settingMap->find( index );

		if( settingMap->end() == setting_it )
		{
			continue;
		}

		const CGuildManager::SkillSetting& setting = setting_it->second;

		if(	setting.mRank.end() != setting.mRank.find( member->Rank ) )
		{
			player->AddSetSkill( index, level );
		}
	}
}

// 080417 LUJ, 길드가 보유한 스킬을 해당 플레이어에게 전송한다
void CGuild::SendSkill( CObject& object ) const
{
	MSG_SKILL_LIST message;
	{
		ZeroMemory( &message, sizeof( message ) );
		message.Category	= MP_GUILD;
		message.Protocol	= MP_GUILD_GET_SKILL_ACK;
		message.mSize		= 0;
	}

	for(
		SkillMap::const_iterator it = mSkillMap.begin();
		mSkillMap.end() != it;
		++it )
	{
		SKILL_BASE& skill = message.mData[ message.mSize++ ];

		skill.wSkillIdx = it->first;
		skill.Level		= it->second;
	}
	
	object.SendMsg( &message, message.GetSize() );
}

// 080417 LUJ, 스킬이 없다면 0이 반환된다
BYTE CGuild::GetSkillLevel( DWORD index ) const
{
	SkillMap::const_iterator it = mSkillMap.find( index );

	return mSkillMap.end() == it ? 0 : it->second;
}

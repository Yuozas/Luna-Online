// Guild.cpp: implementation of the CGuild class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Guild.h"
#include "GameIn.h"
#include "GuildDialog.h"
#include "GuildPlusTimeDialog.h"
#include "ObjectManager.h"
#include "GuildManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuild::CGuild()
{
	m_GuildIdx = 0;
	m_SortType = eRank;

	// 06. 03. ���İ��� - �̿���
	//memset(m_GuildNotice, 0, MAX_GUILD_NOTICE+1);

	m_nStudentCount = 0;
}

CGuild::~CGuild()
{
	Release();
}

void CGuild::Init(GUILDINFO* pInfo)
{
	Release();
	if(pInfo)
	{
		m_GuildIdx = pInfo->GuildIdx;
		m_GuildLvl = pInfo->GuildLevel;
		m_GuildLocation = (BYTE)pInfo->MapNum;
		SafeStrCpy(m_MasterName, pInfo->MasterName, MAX_NAME_LENGTH+1);
		SafeStrCpy(m_GuildName, pInfo->GuildName, MAX_GUILD_NAME+1);
		// 06. 03. ���İ��� - �̿���
		//strcpy(m_GuildNotice, pInfo->GuildNotice);
		m_UnionIdx = pInfo->UnionIdx;
		for(int i=0; i<eRankPos_Max; ++i)
			m_RankMemberIdx[i] = 0;
		
		m_SortType = eRank;

		m_nStudentCount = 0;
	}
}

void CGuild::Release()
{
	PTRLISTPOS pos = m_MemberList.GetHeadPosition();

	while( pos )
	{
		GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);
		SAFE_DELETE(pInfo);
	}
	m_MemberList.RemoveAll();

	m_UnionIdx = 0;
	for(int i=0; i<eRankPos_Max; ++i)
		m_RankMemberIdx[i] = 0;
	
	m_MemberMultiMap.clear();
}

BOOL CGuild::DeleteMember(DWORD PlayerIDX)
{
	PTRLISTPOS pos = m_MemberList.GetHeadPosition();
	GUILDMEMBERINFO* pInfo = NULL;
	while(pos)
	{
		pInfo = (GUILDMEMBERINFO*)m_MemberList.GetAt(pos);
		if(pInfo)
		{
			//SW060713 ���ϻ�
			if(pInfo->Rank == GUILD_STUDENT)
			{
				--m_nStudentCount;
			}

			if(pInfo->MemberIdx == PlayerIDX)
			{
				ResetRankMemberInfo(PlayerIDX, pInfo->Rank);
				SAFE_DELETE(pInfo);
				m_MemberList.RemoveAt(pos);
				RefreshMember();
				return TRUE;
			}
		}
		m_MemberList.GetNext(pos);
	}

	return FALSE;
}

BOOL CGuild::AddMember(GUILDMEMBERINFO* pMemberInfo)
{
	PTRLISTPOS pos = m_MemberList.GetHeadPosition();
	
	GUILDMEMBERINFO* pInfo = NULL;
	while(pos)
	{
		pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);

		if(pInfo)
		if(pInfo->MemberIdx == pMemberInfo->MemberIdx)
		{
			return FALSE;
		}
	}
	
	pInfo = new GUILDMEMBERINFO;
	*pInfo = *pMemberInfo;
	
	m_MemberList.AddTail(pInfo);
	if(pInfo->Rank == GUILD_VICEMASTER)
		m_RankMemberIdx[0] = pInfo->MemberIdx;
	else if(pInfo->Rank == GUILD_SENIOR)
	{
		int pos = IsVacancy(GUILD_SENIOR);
		if((pos <0) || (pos > eRankPos_Max))
		{
			ASSERTMSG(0, "Load GuildMemberInfo - Set Ranked Member");
			return TRUE;
		}
		m_RankMemberIdx[pos] = pInfo->MemberIdx;
	}

	//SW00713 ���ϻ�
	if(pInfo->Rank == GUILD_STUDENT)
	{
		++m_nStudentCount;
	}

	return TRUE;
}

BOOL CGuild::IsMember(DWORD PlayerIDX)
{
	PTRLISTPOS pos = m_MemberList.GetHeadPosition();

	while(pos)
	{
		GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);
	
		if(pInfo)
		if(pInfo->MemberIdx == PlayerIDX)
		{
			return TRUE;
		}
	}
	return FALSE;
}

void CGuild::SetTotalInfo(GUILDINFO* pInfo, GUILDMEMBERINFO* pMemberInfo, int MemberNum)
{
	Release();

	m_GuildIdx = pInfo->GuildIdx;
	m_GuildLvl = pInfo->GuildLevel;
	for(int i=0; i<MemberNum; ++i)
	{
		if(HEROID == pMemberInfo[i].MemberIdx)
			pMemberInfo[i].bLogged = TRUE;
		AddMember(&pMemberInfo[i]);
	}
	m_GuildLocation = (BYTE)pInfo->MapNum;
	SafeStrCpy(m_MasterName, pInfo->MasterName, MAX_NAME_LENGTH+1);
    SafeStrCpy(m_GuildName, pInfo->GuildName , MAX_GUILD_NAME+1);
	// 06. 03. ���İ��� - �̿���
	//strcpy(m_GuildNotice, pInfo->GuildNotice);
	RefreshMember();	
}

void CGuild::RefreshMember()
{
	// sort and update
	/*
	GAMEIN->GetGuildDlg()->DeleteMemberAll();
	PTRLISTPOS pos = m_MemberList.GetHeadPosition();
	GUILDMEMBERINFO* pInfo = NULL;
	while(pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos))
	{
	GAMEIN->GetGuildDlg()->ResetMemberInfo(pInfo);	
	}
	GAMEIN->GetGuildDlg()->SetDisableFuncBtn(HERO->GetGuildMemberRank());
	*/

	//SortbyType();
	//GAMEIN->GetGuildDlg()->DeleteMemberAll();
	
	//{
	//	CGuildDialog* dialog = GAMEIN->GetGuildDlg();

	//	for(	multimap< RANK, GUILDMEMBERINFO* >::reverse_iterator it= m_MemberMultiMap.rbegin();
	//			it != m_MemberMultiMap.rend();
	//			++it )
	//	{
	//		dialog->ResetMemberInfo( it->second );
	//	}
	//}	

	//���̸�� �߰� - �׽�Ʈ��
/*
	GUILDMEMBERINFO dummymember;
	sprintf(dummymember.MemberName,"�δ���0");
	dummymember.bLogged = false;
	dummymember.MemberIdx = 1;
	dummymember.Memberlvl = 30;
	dummymember.Rank = 40;
	
	GAMEIN->GetGuildDlg()->ResetMemberInfo(&dummymember);

	sprintf(dummymember.MemberName,"�ܿ�1");
	dummymember.bLogged = false;
	dummymember.MemberIdx = 2;
	dummymember.Memberlvl = 31;
	dummymember.Rank = 20;

	GAMEIN->GetGuildDlg()->ResetMemberInfo(&dummymember);

	sprintf(dummymember.MemberName,"�δ���2");
	dummymember.bLogged = false;
	dummymember.MemberIdx = 3;
	dummymember.Memberlvl = 34;
	dummymember.Rank = 40;

	GAMEIN->GetGuildDlg()->ResetMemberInfo(&dummymember);

	sprintf(dummymember.MemberName,"�ܿ�3");
	dummymember.bLogged = false;
	dummymember.MemberIdx = 4;
	dummymember.Memberlvl = 5;
	dummymember.Rank = 20;

	GAMEIN->GetGuildDlg()->ResetMemberInfo(&dummymember);

	sprintf(dummymember.MemberName,"�ܿ�4");
	dummymember.bLogged = false;
	dummymember.MemberIdx = 5;
	dummymember.Memberlvl = 7;
	dummymember.Rank = 20;

	GAMEIN->GetGuildDlg()->ResetMemberInfo(&dummymember);

	sprintf(dummymember.MemberName,"�ܿ�5");
	dummymember.bLogged = false;
	dummymember.MemberIdx = 6;
	dummymember.Memberlvl = 9;
	dummymember.Rank = 20;

	GAMEIN->GetGuildDlg()->ResetMemberInfo(&dummymember);

	sprintf(dummymember.MemberName,"�ܿ�6");
	dummymember.bLogged = false;
	dummymember.MemberIdx = 7;
	dummymember.Memberlvl = 42;
	dummymember.Rank = 20;

	GAMEIN->GetGuildDlg()->ResetMemberInfo(&dummymember);

	sprintf(dummymember.MemberName,"�δ���7");
	dummymember.bLogged = false;
	dummymember.MemberIdx = 8;
	dummymember.Memberlvl = 21;
	dummymember.Rank = 40;

	GAMEIN->GetGuildDlg()->ResetMemberInfo(&dummymember);

	sprintf(dummymember.MemberName,"�δ���8");
	dummymember.bLogged = false;
	dummymember.MemberIdx = 9;
	dummymember.Memberlvl = 43;
	dummymember.Rank = 40;

	GAMEIN->GetGuildDlg()->ResetMemberInfo(&dummymember);

	sprintf(dummymember.MemberName,"�ܿ�9");
	dummymember.bLogged = false;
	dummymember.MemberIdx = 10;
	dummymember.Memberlvl = 24;
	dummymember.Rank = 20;

	GAMEIN->GetGuildDlg()->ResetMemberInfo(&dummymember);

	sprintf(dummymember.MemberName,"�ܿ�10");
	dummymember.bLogged = false;
	dummymember.MemberIdx = 11;
	dummymember.Memberlvl = 23;
	dummymember.Rank = 20;

	GAMEIN->GetGuildDlg()->ResetMemberInfo(&dummymember);
*/
}

GUILDMEMBERINFO* CGuild::GetMemberInfo(DWORD PlayerIDX)
{
	PTRLISTPOS pos = m_MemberList.GetHeadPosition();
	GUILDMEMBERINFO* pInfo = NULL;
	while(pos)
	{
		GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);
		
		if(pInfo)
		if(pInfo->MemberIdx == PlayerIDX)
		{
			return pInfo;
		}
	}
	return NULL;
}

char* CGuild::GetMemberName(DWORD MemberID)
{
	PTRLISTPOS pos = m_MemberList.GetHeadPosition();
	
	while(pos)
	{
		GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);
		
		if(pInfo)
		if(pInfo->MemberIdx == MemberID)
		{
			return pInfo->MemberName;
		}
	}
	return "";
}

DWORD CGuild::GetMemberIdx(int num)
{
	int i=0;

	multimap< RANK, GUILDMEMBERINFO* >::reverse_iterator rit;
	
	for(rit = m_MemberMultiMap.rbegin(); rit != m_MemberMultiMap.rend(); rit++)
	{
		if(i == num)
		{	
			GUILDMEMBERINFO* pInfo = (*rit).second;
			return pInfo->MemberIdx;
		}
		i++;
	}

	return 0;
}

void CGuild::SetLevel(BYTE Level)
{
	m_GuildLvl = Level;
}

//BYTE CGuild::GetMaxMemberNum()
//{
//	if(( m_GuildLvl <= 0) || ( m_GuildLvl > GUILD_5LEVEL))
//	{
//		char buf[64];
//		sprintf(buf, "Wrong Member Num, GuildIdx : %u", m_GuildIdx);
//		ASSERTMSG(0, buf);
//		return 0;
//	}
//
//	return 10 + ( (m_GuildLvl-1) * 5 ); 
//}

//BOOL CGuild::CanAddMember()
//{
//	if( GetMemberSize() < GetMaxMemberNum() )
//		return TRUE;
//	else
//		return FALSE;
//}

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
		return eRankPos_Max;
		
	default:
		ASSERTMSG(0, "IsVancancy Err.");
		return eRankPos_Err;
	}

	int rt = eRankPos_Err;
	for(int i=start; i<end; ++i)
	{
		if( m_RankMemberIdx[i] == 0 )
		{
			return i;
		}
	}
	return rt;
}

BOOL CGuild::ChangeRank(DWORD MemberID, BYTE Rank)
{
	//int Pos = IsVacancy(Rank);
	//if(Pos == eRankPos_Err)
	//{
	//	ASSERTMSG(0, "Client Change Rank IsVacancy Error");
	//	return FALSE;
	//}
	GUILDMEMBERINFO* pMemberInfo = GetMemberInfo(MemberID);
	if(pMemberInfo == 0)
	{
		//ASSERTMSG(0, "Change Rank - No Member Data");
		return FALSE;
	}
	//if((Pos <0) || (Pos >eRankPos_Max))
	//{
	//	ASSERTMSG(0, "change rank pos boundary error");
	//	return FALSE;
	//}
	ResetRankMemberInfo(MemberID, pMemberInfo->Rank);

	//SW060713 ���ϻ�
	if(pMemberInfo->Rank == GUILD_STUDENT && Rank != GUILD_STUDENT)
	{
		--m_nStudentCount;
	}

	pMemberInfo->Rank = Rank;
	//if(Pos == eRankPos_Max)
	//	return TRUE;
	//m_RankMemberIdx[Pos] = MemberID;
	return TRUE;
}

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

void CGuild::SetMemberLevel(DWORD MemberIdx, DWORD MemberLvl)
{
	{
		CGuildDialog* dialog = GAMEIN->GetGuildDlg();

		dialog->SetMemberLevel( MemberIdx, MemberLvl );
	}

	//GAMEIN->GetGuildDlg()->DeleteMemberAll();
	//PTRLISTPOS pos = m_MemberList.GetHeadPosition();

	//while(pos)
	//{
	//	GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);
	//	
	//	if(pInfo)
	//		GAMEIN->GetGuildDlg()->ResetMemberInfo(pInfo);	
	//}
}

void CGuild::SetLogInfo(DWORD MemberIdx, BOOL vals)
{
	GUILDMEMBERINFO* pMemberInfo = GetMemberInfo(MemberIdx);
	if(pMemberInfo == 0)
		return;
	pMemberInfo->bLogged = vals;

	//dlg refresh
}

//void CGuild::SetSortType(BYTE type)
//{
//	m_SortType = type;	
//}
//
//BYTE CGuild::GetSortType()
//{
//	return m_SortType;
//}
//
//void CGuild::SortbyType()
//{
//	if(m_SortType == eRank)
//	{
//		m_MemberMultiMap.clear();
//		PTRLISTPOS pos = m_MemberList.GetHeadPosition();
//
//		while(pos)
//		{
//			GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);
//
//			if(pInfo)
//			{
//				if( pInfo->bLogged )
//					m_MemberMultiMap.insert(multimap< RANK, GUILDMEMBERINFO* >::value_type(10+pInfo->Rank, pInfo));
//				else
//					m_MemberMultiMap.insert(multimap< RANK, GUILDMEMBERINFO* >::value_type(pInfo->Rank, pInfo));
//			}
//
////			m_MemberMultiMap.insert(multimap< RANK, GUILDMEMBERINFO* >::value_type(pInfo->Rank, pInfo));
//		}		
//	}
//}

void CGuild::SetGuildPosition( DWORD dwMapnum )
{
	m_GuildLocation = (BYTE)dwMapnum;
	GAMEIN->GetGuildDlg()->SetPosition( GetMapName( m_GuildLocation ) );	
}

//BOOL CGuild::CanAddStudent()
//{
////#define GUILD_STUDENT_NUM_MAX	25
//	return ( GetStudentNum() <= GUILD_STUDENT_NUM_MAX );
//}

//void CGuild::AddGuildPlustime( DWORD plustimeKind, DWORD plustimeIdx, DWORD endTime )
//{
//	m_GuildPoint.GuildPlusTimeflg |= (1<<plustimeKind);
//
//	m_GuildPoint.GuildUsingPlusTimeInfo[plustimeKind].PlusTimeIdx = plustimeIdx;
//	m_GuildPoint.GuildUsingPlusTimeInfo[plustimeKind].PlusTimeEndTime = endTime;
//}
//
//void CGuild::RemoveGuildPlustime( DWORD plustimeKind )
//{
//	m_GuildPoint.GuildPlusTimeflg ^= (1<<plustimeKind);
//
//	m_GuildPoint.GuildUsingPlusTimeInfo[plustimeKind].PlusTimeEndTime = 0;
//}
//
//BOOL CGuild::IsUsingPlustime( DWORD plustimeKind )
//{
//	return ( m_GuildPoint.GuildPlusTimeflg & (1<<plustimeKind) );
//}

//void CGuild::SetGuildPointInfo( GUILDPOINT_INFO* pGuildPointInfo )
//{
//	//memcpy(&m_GuildPoint, pGuildPointInfo, sizeof(GUILDPOINT_INFO));
//
//	SetGuildPoint(pGuildPointInfo->GuildPoint);
//	//m_GuildPoint.GuildHuntedMonsterCount = pGuildPointInfo->GuildHuntedMonsterCount;
//	m_GuildPoint.GuildHuntedMonsterTotalCount = pGuildPointInfo->GuildHuntedMonsterTotalCount;
//	m_GuildPoint.DBProcessTime = pGuildPointInfo->DBProcessTime;
//
//	m_GuildPoint.GuildPlusTimeflg = 0;
//	for( int n = 0; n < eGPT_Kind_Max; ++n )
//	{
//		int PlustimeIdx = pGuildPointInfo->GuildUsingPlusTimeInfo[n].PlusTimeIdx;
//		//AddGuildPlustime(m_GuildPoint.GuildUsingPlusTimeInfo[n].PlusTimeKind, m_GuildPoint.GuildUsingPlusTimeInfo[n].PlusTimeEndTime);
//		if(PlustimeIdx)
//		{
//			AddGuildPlustime(n, PlustimeIdx, pGuildPointInfo->GuildUsingPlusTimeInfo[n].PlusTimeEndTime);
//			GUILDMGR->AddGuildPlustimeStatusIcon(PlustimeIdx);
//		}
//	}
//}

//void CGuild::SetGuildPoint( int TotalPoint )
//{
//	m_GuildPoint.GuildPoint = TotalPoint;
//
//	CGuildPlusTimeDlg* pDlg = GAMEIN->GetGuildPlusTimeDlg();
//	if(pDlg)
//	{
//		pDlg->SetGuildPointText(TotalPoint);
//	}
//}


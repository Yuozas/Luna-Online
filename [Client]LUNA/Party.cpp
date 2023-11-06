// Party.cpp: implementation of the CParty class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Party.h"
#include "PartyManager.h"
#include "GameIn.h"
#include "ObjectManager.h"

// 071002 LYW --- Party.cpp : Include header files.
#include "interface/cWindowManager.h"
#include "windowidenum.h"
#include "PartyBtnDlg.h"

// 071004 LYW --- PartyManager : Include header file of CGuageDialog.
#include "GuageDialog.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CParty::CParty()
{
//	m_PartyDBIdx = 0;
	m_Option = 0;
}

CParty::~CParty()
{
	
}

void CParty::InitParty(PARTY_INFO* pInfo)
{
	m_Option = 0;
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
		m_Member[n].Clear();

	HERO->SetPartyIdx(pInfo->PartyDBIdx);

	SetMaster(&pInfo->Member[0]);

	// 071004 LYW --- Party : Setting master mark on of hero guage.
	CGuageDialog* pGuageDlg = NULL ;							// HERO 게이지를 받을 포인터를 선언하고 null 처리를 한다.
	pGuageDlg = GAMEIN->GetGuageDlg() ;							// HERO 게이지를 받는다.

	if( pGuageDlg )												// HERO 게이지 정보가 유효하다면,
	{
		if( HEROID == GetMasterIdx() )							// HERO 가 파티 마스터라면,
		{
			pGuageDlg->SetRenderMasterMark(TRUE) ;				// 파티 마스터 마크 표시를 활성화 한다.
		}
		else
		{
			pGuageDlg->SetRenderMasterMark(FALSE) ;				// 파티 마스터 마크 표시를 비활성화 한다.
		}
	}

	SetOption(pInfo->Option);

	for(n=1;n<MAX_PARTY_LISTNUM;++n)
	{
		if(pInfo->Member[n].dwMemberID != 0)
		{
			AddMember(&pInfo->Member[n]);
		}
	}

	CPartyBtnDlg* pPartySet = NULL ;
	pPartySet = (CPartyBtnDlg*)WINDOWMGR->GetWindowForID( PA_BTNDLG );

	if( pPartySet )
	{
		pPartySet->SetCheckBox(pInfo->Option) ;
	}
}
/*
void CParty::SetPartyIdx(DWORD Partyid)
{
	m_PartyDBIdx = Partyid;
	if(HERO)
		HERO->SetPartyIdx(Partyid);
}
*/
void CParty::SetMaster(PARTY_MEMBER* pMasterInfo)
{
	m_Member[0].SetInfo(pMasterInfo);
}
void CParty::SetMaster(DWORD MasterIdx,char* strName,BOOL bLog,BYTE LifePercent, BYTE ManaPercent)
{
	m_Member[0].SetInfo(MasterIdx,strName,bLog,LifePercent,ManaPercent);
}
/*
DWORD CParty::GetPartyIdx()
{
	return m_PartyDBIdx;
}
*/
DWORD CParty::GetMasterIdx()
{
//	ASSERT(m_Member[0].GetMemberID());
	return m_Member[0].GetMemberID();
}

char* CParty::GetMasterName()
{
//	ASSERT(m_Member[0].GetMemberID());
	return m_Member[0].GetMemberName();
}


void CParty::AddMember(PARTY_MEMBER* pMemberInfo) 
{
	for(int n=1;n<MAX_PARTY_LISTNUM;++n)
	{
		// 070319 LYW --- Party : Check duplicate member.
		for( int m=0; m<n; ++m )
		{
			if( m_Member[m].GetMemberID() == pMemberInfo->dwMemberID ) return ;
		}
		if(m_Member[n].GetMemberID() == 0)
		{
			m_Member[n].SetInfo(pMemberInfo);
			return;
		}
	}
//	ASSERT(0);
}

// 070315 LYW --- Party : Add function to remove member.
void CParty::RemovieMember( DWORD id )
{
	BOOL bSameID = FALSE ;

	for(int n=1;n<MAX_PARTY_LISTNUM;++n) 
	{
		if(m_Member[n].GetMemberID() == id) 
		{			
			bSameID = TRUE ;
		}

		if( bSameID )
		{
			if( n+1 < MAX_PARTY_LISTNUM )
			{
				m_Member[n].SetInfo( m_Member[n+1].GetMemberInfo() ) ;
			}
			else
			{
				m_Member[n].SetInfo(0, "", FALSE, 0, 0) ;
				m_Member[n].SetLevel(0) ;
			}
		}
	}
}

void CParty::DelMember(DWORD id)
{
	for(int n=1;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].GetMemberID() == id)
		{
			m_Member[n].Clear();
			return;
		}
	}
//	ASSERT(0);
}

void CParty::LogIn(PARTY_MEMBER* pMemberInfo)
{
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].GetMemberID() == pMemberInfo->dwMemberID)
		{
			m_Member[n].SetInfo(pMemberInfo);
			return;
		}
	}
//	ASSERT(0);
}

void CParty::LogOut(DWORD PlayerID)
{
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].GetMemberID() == PlayerID)
		{
			m_Member[n].SetLog(FALSE);
			m_Member[n].SetLifePercent(0);
			m_Member[n].SetManaPercent(0);
			return;
		}
	}
//	ASSERT(0);
}

char* CParty::GetMemberName(DWORD MemberID)
{	
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].GetMemberID() == MemberID)
		{
			return m_Member[n].GetMemberName();
		}
	}

	// 없는 파티원을 찾으려 했다.
//	ASSERT(0);
	return "";
}

DWORD CParty::GetMemberID(int num)
{
//	ASSERT(num < MAX_PARTY_LISTNUM);
	return m_Member[num].GetMemberID();
}

void CParty::SetMemberLifePercent(DWORD PlayerID, DWORD lifePercent)
{
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].GetMemberID() == PlayerID)
		{
			m_Member[n].SetLifePercent((BYTE)lifePercent);
			return;
		}
	}
//	ASSERT(0);
}

BYTE CParty::GetMemberLifePercent(DWORD PlayerID)
{
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].GetMemberID() == PlayerID)
		{
			return (m_Member[n].GetLifePercent());
		}
	}
//	ASSERT(0);
	return 0;
}

void CParty::SetMemberManaPercent(DWORD PlayerID, DWORD manaPercent)
{
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].GetMemberID() == PlayerID)
		{
			m_Member[n].SetManaPercent((BYTE)manaPercent);
			return;
		}
	}
//	ASSERT(0);
}

void CParty::SetMemberLevel(DWORD PlayerID, LEVELTYPE lvl)
{
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].GetMemberID() == PlayerID)
		{
			m_Member[n].SetLevel(lvl);
			return;
		}
	}
//	ASSERT(0);
}

BYTE CParty::GetMemberManaPercent(DWORD PlayerID)
{
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].GetMemberID() == PlayerID)
		{
			return (m_Member[n].GetManaPercent());
		}
	}
//	ASSERT(0);
	return 0;
}

LEVELTYPE CParty::GetMemberLevel(DWORD PlayerID)
{
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].GetMemberID() == PlayerID)
		{
			return (m_Member[n].GetLevel());
		}
	}
//	ASSERT(0);
	return 0;
}


void CParty::BreakUpResult()
{
	for(int n=0; n<MAX_PARTY_LISTNUM; ++n) 
	{			
		if(m_Member[n].GetMemberID() != 0)	
		{
			m_Member[n].SetInfo(0,"",0,0,0);
		} //if
	} //for	
}

void CParty::Clear()
{
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		m_Member[n].Clear();
	}
}

void CParty::ChangeMaster(DWORD dwPlayerID)
{
	for(int n=1;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].GetMemberID() == dwPlayerID)
		{
			//swap
			CPartyMember temp;
			memcpy(&temp,&m_Member[0],sizeof(CPartyMember));
			memcpy(&m_Member[0],&m_Member[n],sizeof(CPartyMember));
			memcpy(&m_Member[n],&temp,sizeof(CPartyMember));
			break;
		}		
	}
}

BOOL CParty::IsPartyMember(DWORD dwPlayerID)
{
//	ASSERT(dwPlayerID);
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].GetMemberID() == dwPlayerID)
			return TRUE;
	}
	return FALSE;
}





// 071006 LYW --- Party : Add function to check member for name.
BOOL CParty::IsPartyMember(char* pName)
{
	if( !pName ) return FALSE ;

	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		if( strcmp(m_Member[n].GetMemberName(), pName) == 0 )
		{
			return TRUE ;
		}
	}

	return FALSE ;
}





// 071006 LYW --- Party : Add function to return member id to use name.
DWORD CParty::GetMemberIdForName(char* pName)
{
	if( !pName ) return 0 ;

	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		if( strcmp(m_Member[n].GetMemberName(), pName) == 0 )
		{
			return m_Member[n].GetMemberID() ;
		}
	}

	return 0 ;
}





PARTY_MEMBER* CParty::GetPartyMemberInfo(int i)
{
//	ASSERT(i < MAX_PARTY_LISTNUM);
	return m_Member[i].GetMemberInfo();
}

int CParty::GetMemberNum()
{
	int num=0;
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].GetMemberID() != 0)
			++num;
	}
	return num;
}

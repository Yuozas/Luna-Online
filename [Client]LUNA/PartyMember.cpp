// PartyMember.cpp: implementation of the CPartyMember class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartyMember.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPartyMember::CPartyMember()
{
	m_MemberInfo.dwMemberID = 0;
	m_MemberInfo.Name[0] = 0;
	m_MemberInfo.bLogged = FALSE;
	m_MemberInfo.LifePercent = 0;
}

CPartyMember::~CPartyMember()
{

}

void CPartyMember::SetInfo(DWORD PlayerIDx,char* Name,BOOL bLog,BYTE LifePercent,BYTE ManaPercent)
{
	m_MemberInfo.dwMemberID = PlayerIDx;
	strcpy(m_MemberInfo.Name,Name);
	m_MemberInfo.bLogged = bLog ? true : false;
	m_MemberInfo.LifePercent = LifePercent;
	m_MemberInfo.ManaPercent = ManaPercent;
}

void CPartyMember::SetInfo(PARTY_MEMBER* pInfo)
{
	m_MemberInfo.dwMemberID = pInfo->dwMemberID;
	strcpy(m_MemberInfo.Name, pInfo->Name);
	m_MemberInfo.bLogged = pInfo->bLogged ? true : false;
	m_MemberInfo.LifePercent = pInfo->LifePercent;
	m_MemberInfo.ManaPercent = pInfo->ManaPercent;
	m_MemberInfo.Level = pInfo->Level;

	// 070420 LYW --- PartyMember : Setting racial and gender.
	m_MemberInfo.Race	= pInfo->Race ;
	m_MemberInfo.Gender	= pInfo->Gender ;
}

void CPartyMember::SetLog(BOOL bLog)
{
	m_MemberInfo.bLogged = bLog ? true : false;
}

void CPartyMember::SetLifePercent(BYTE LifePercent)
{
	if(m_MemberInfo.LifePercent != LifePercent)
	{
		m_MemberInfo.LifePercent = LifePercent;
	}
	
}

void CPartyMember::SetManaPercent(BYTE ManaPercent)
{
	if(m_MemberInfo.ManaPercent != ManaPercent)
	{
		m_MemberInfo.ManaPercent = ManaPercent;
	}
}

void CPartyMember::SetLevel(LEVELTYPE lvl)
{
	m_MemberInfo.Level = lvl;
}


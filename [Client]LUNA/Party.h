// Party.h: interface for the CParty class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARTY_H__F6D978D7_2592_4CB4_B54E_2AF0835E1A20__INCLUDED_)
#define AFX_PARTY_H__F6D978D7_2592_4CB4_B54E_2AF0835E1A20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PartyMember.h"

class CParty  
{	
//	DWORD m_PartyDBIdx;
	CPartyMember m_Member[MAX_PARTY_LISTNUM];	// �����ʹ� [0]!!
	
	BYTE m_Option;			//a distribute way that dropped item and dropped money
public:
	CParty();
	virtual ~CParty();
	
	void InitParty(PARTY_INFO* pInfo);
	
//	void SetPartyIdx(DWORD Partyid);
	void SetMaster(PARTY_MEMBER* pMasterInfo);
	void SetMaster(DWORD MasterIdx,char* strName,BOOL bLog,BYTE LifePercent,BYTE ManaPercent);
	DWORD GetMemberID(int num); //��Ƽ������ ��� ����
	int GetMemberNum();
//	DWORD GetPartyIdx();
	DWORD GetMasterIdx();

	char* GetMasterName();

	void AddMember(PARTY_MEMBER* pMemberInfo);
	// 070315 LYW --- Party : Add function to remove member.
	void RemovieMember( DWORD id ) ;
	void DelMember(DWORD id);
	void BreakUpResult();

	void LogIn(PARTY_MEMBER* pMemberInfo);
	void LogOut(DWORD PlayerID);
	
	char* GetMemberName(DWORD MemberID);
	
	void SetMemberLifePercent(DWORD PlayerID, DWORD lifePercent);
	BYTE GetMemberLifePercent(DWORD PlayerID);
	
	void SetMemberManaPercent(DWORD PlayerID, DWORD manaPercent);
	BYTE GetMemberManaPercent(DWORD PlayerID);
		
	void SetMemberLevel(DWORD PlayerID, LEVELTYPE lvl);
	LEVELTYPE GetMemberLevel(DWORD PlayerID);

	PARTY_MEMBER* GetPartyMemberInfo(int i);
	
	void Clear();
	
	BOOL IsLogIn(int n)		{	return m_Member[n].IsMemberLog();	}

	void ChangeMaster(DWORD dwPlayerID);
	
	BOOL IsPartyMember(DWORD dwPlayerID);

	// 071006 LYW --- Party : Add function to check member for name.
	BOOL IsPartyMember(char* pName) ;
	// 071006 LYW --- Party : Add function to return member id to use name.
	DWORD GetMemberIdForName(char* pName) ;

	void SetOption(BYTE Option) { m_Option = Option;	}
};

#endif // !defined(AFX_PARTY_H__F6D978D7_2592_4CB4_B54E_2AF0835E1A20__INCLUDED_)


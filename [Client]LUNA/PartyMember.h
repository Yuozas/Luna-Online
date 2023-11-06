// PartyMember.h: interface for the CPartyMember class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARTYMEMBER_H__52175D0F_A4F2_41D9_BF75_58E721259163__INCLUDED_)
#define AFX_PARTYMEMBER_H__52175D0F_A4F2_41D9_BF75_58E721259163__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPartyMember  
{
	PARTY_MEMBER m_MemberInfo;
		
public:
	CPartyMember();
	virtual ~CPartyMember();

	void SetInfo(DWORD PlayerIDx,char* Name,BOOL bLog,BYTE LifePercent,BYTE ManaPercent);
	void SetInfo(PARTY_MEMBER* pInfo);
	void SetLog(BOOL bLog);
	void SetLifePercent(BYTE LifePercent);
	void SetShieldPercent(BYTE ShieldPercent);
	void SetManaPercent(BYTE ManaPercent);
	void SetLevel(LEVELTYPE lvl);

	DWORD GetMemberID()			{
		return DWORD(m_MemberInfo.dwMemberID);
	}
	char* GetMemberName()		{	return m_MemberInfo.Name;			}
	BOOL IsMemberLog()			{	return m_MemberInfo.bLogged;		}
	BYTE GetLifePercent()		{	return m_MemberInfo.LifePercent;	}
	BYTE GetManaPercent()	{	return m_MemberInfo.ManaPercent;	}
	LEVELTYPE GetLevel()		{	return m_MemberInfo.Level;			}

	PARTY_MEMBER* GetMemberInfo()	{	return &m_MemberInfo;	}

	void Clear()				{	m_MemberInfo.dwMemberID = 0;		}
};

#endif // !defined(AFX_PARTYMEMBER_H__52175D0F_A4F2_41D9_BF75_58E721259163__INCLUDED_)

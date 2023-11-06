// Party.h: interface for the CParty class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARTY_H__87E3B123_4418_45C7_B287_81730C428C79__INCLUDED_)
#define AFX_PARTY_H__87E3B123_4418_45C7_B287_81730C428C79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Player.h"
class CDistributeWay;
class MONSTEREX_LIST;
template <class T> void swap(T& x, T& y) 
{ 
	T temp = x;
	x = y;
	y = temp;
}
struct PARTYMEMBER
{    
	PARTYMEMBER()	
	{
		MemberID = 0;
		Name[0] = 0;
		bLogged = FALSE;
		LifePercent = 0;
		ManaPercent = 0;
		Level = 0;

		// 070420 LYW --- Party : Add racial and gender.
		racial = 0 ;
		gender = 0 ;
	}
	// 070420 LYW --- Party : Modified function SetMember.
	//void SetMember(DWORD AddMemberID,char* strName,CPlayer* pPlayer, LEVELTYPE lvl)
	void SetMember(DWORD AddMemberID,char* strName,CPlayer* pPlayer, LEVELTYPE lvl, BYTE byRacial, BYTE byGender)
	{
		MemberID = AddMemberID;
		//strcpy(this->Name,strName);
		SafeStrCpy(this->Name,strName, MAX_NAME_LENGTH+1);
		
		Level = lvl;
		if(pPlayer)
		{
			bLogged = TRUE;
			ManaPercent = pPlayer->GetManaPercent();
			LifePercent = pPlayer->GetLifePercent();
			
			racial = byRacial ;
			gender = byGender ;
		}
		else
		{
			bLogged = FALSE;
			ManaPercent = 0;
			LifePercent = 0;

			racial = 0 ;
			/**/gender = 0 ;/**/
		}
	}
	void SetLevel(LEVELTYPE lvl)
	{
		Level = lvl;
	}

	char Name[MAX_NAME_LENGTH+1];
	DWORD MemberID;
	BOOL bLogged;
	BYTE LifePercent;
	BYTE ManaPercent;
	LEVELTYPE Level;
// 070420 LYW --- Party : Add racial and gender.
	BYTE racial ;
	BYTE gender ;
	
};

class CParty
{	
	//DB에 저장될 인덱스 값

	DWORD m_PartyIDx;
	PARTYMEMBER m_Member[MAX_PARTY_LISTNUM];
	BYTE m_Option;
	DWORD m_OldSendtime;

	//경험치 분산 방법
	CDistributeWay* m_pDistributeWay; 

	void Init(DWORD id,DWORD MasterID,char* strMasterName,CPlayer* pMaster,BYTE Option);
	void SendPartyInfo(CPlayer* pToPlayer);

	// 070316 LYW --- Party : 순차 분배시 사용.
	// 당첨자 아이디
	DWORD					m_dwWinnerID ;
	// 이전 당첨자 다음 멤버 아이디
	DWORD m_dwMemberID[MAX_PARTY_LISTNUM];

	BYTE					m_ItemWinner ;
	PARTY_RECEIVE_MEMBER*	m_pCandidate ;
	CPlayer*				m_pItemWinner ;

	// 070507 LYW --- Party : Add check count.
	BYTE m_byWinnerIdx ;
	BOOL m_bDistributed ;
public:
	CParty();
	~CParty();

	// 070420 LYW --- Party : Modified function Add partyMember.
	//BOOL AddPartyMember(DWORD AddMemberID,char* Name,CPlayer* pPlayer, LEVELTYPE lvl) ;
	BOOL AddPartyMember(DWORD AddMemberID,char* Name,CPlayer* pPlayer, LEVELTYPE lvl, BYTE racial, BYTE gender) ;
	BOOL RemovePartyMember(DWORD MemberID);
	// 070315 LYW --- Party : Add function to delete member.
	void DeletePartyMember(DWORD MemberID) ;
	BOOL ChangeMaster(DWORD FromID,DWORD ToID);
	BOOL IsPartyMember(DWORD PlayerID);
	BOOL IsMemberLogIn(int n);

	// 070507 LYW --- Party : Add function to setting winner index.
	void SetWinnerIdx(BYTE idx) { m_byWinnerIdx = idx ; }
	BYTE GetWinnerIdx() { return m_byWinnerIdx ; }

	void SetDistributed(BOOL val) { m_bDistributed = val ; }
	BOOL IsDistributed() { return m_bDistributed ; }

	void SetMaster(CPlayer* pMaster);
	void SetOption(BYTE Option);
	BYTE GetOption();
	

	void UserLogIn(CPlayer* pPlayer,BOOL bNotifyUserLogin);
	void UserLogOut(DWORD PlayerID);
	void SendMsgUserLogOut(DWORD PlayerID);
	
	void SendMsgLoginMemberInfo(SEND_PARTY_MEMBER_INFO* msg, DWORD GridID);

	void SendMsgToAll(MSGBASE* msg, int size);
	void SendMsgExceptOne(MSGBASE* msg, int size, DWORD PlayerID);

	// 071002 LYW --- Party : Add function to send message to member.
	void SendMsgToMember(MSG_DWORD2* pMsg) ;

	DWORD GetPartyIdx()	{	return m_PartyIDx;	}
	DWORD GetMemberID(int n)	{	return m_Member[n].MemberID; }
	DWORD GetMasterID()			{ return m_Member[0].MemberID;	}
	char* GetMemberName(DWORD MemberID);

	DWORD GetFirstOnlineMemberID();

	void BreakUp();

	void GetMemberInfo(int n,PARTY_MEMBER* pRtInfo);
	PARTYMEMBER* GetPartyMember()	{ return m_Member; }
	void NotifyUserLogIn(SEND_CHANGE_PARTY_MEMBER_INFO* pmsg);
	void SendMemberLogInMsg(DWORD PlayerID);
	void NotifyMemberLogin(DWORD PlayerID);
	void SendPlayerInfoToOtherMembers(int n, DWORD GridID);
	int SetMemberInfo(DWORD MemberID, char* strName, CPlayer* pPlayer, LEVELTYPE lvl, BOOL bLog);
	
	void SetMasterChanging(BOOL val);
	BOOL IsMasterChanging(CPlayer* pPlayer, BYTE Protocol);
	void SendMsgExceptOneinChannel(MSGBASE* msg, int size, DWORD PlayerID, DWORD GridID);

	friend class CPartyManager;
	
	//level
	void SetMemberLevel(DWORD PlayerID, LEVELTYPE lvl);
	void SendMemberPos(DWORD MoverID, BASEMOVE_INFO* pMoveInfo);

	/*어빌리티 삭제 - void SendAbil(LEVELTYPE MonsterLevel, PARTY_RECEIVE_MEMBER* pMemberInfo, LEVELTYPE MaxLevel );*/
	void SendItem(PARTY_RECEIVE_MEMBER* pRealMember, DWORD DropItemId, DWORD DropItemRatio, MONSTEREX_LIST * pMonInfo, WORD MonsterKind, LEVELTYPE MaxLevel);

	// 070316 LYW --- Party : 순차 분배시 사용.
	CPlayer* GetItemWinner() ;

	// 멤버 아이디 셋팅 함수
	void InitMemberID() ;
	// 당첨자 결정 함수
	void SetWinner() ;
	// 당첨자 찾기
	BOOL FindWinner(DWORD id) ;

	// 071002 LYW --- Party : Add function to notice to members that changed option of party.
	void NotifyChangedOption(DWORD dwOption) ;
	
private:
	BOOL m_MasterChanging;
};

#endif // !defined(AFX_PARTY_H__87E3B123_4418_45C7_B287_81730C428C79__INCLUDED_)

// PartyManager.h: interface for the CPartyManager class.
//
//////////////////////////////////////////////////////////////////////
#ifndef _CPARTYMANAGER_H_
#define _CPARTYMANAGER_H_
#pragma once

#include "Party.h"

#define PARTYMGR USINGTON(CPartyManager)

// 071002 LYW --- PartyManager.h : Add enum for party option.
enum PARTY_ITEM_OPTION
{
	e_OPTION_RANDOM = 0,
	e_OPTION_DAMAGE, 
	e_OPTION_SEQUENCE, 
} ;

class CPartyManager 
{
	CParty m_Party;
	void SetPartyInfo(PARTY_INFO* pmsg);
	DWORD m_RequestPartyID;
	BOOL m_bIsProcessing;

	int m_PartyOption ;

public:
	CPartyManager();
	virtual ~CPartyManager();

	void Init();
	void NetworkMsgParse(BYTE Protocol,void* pMsg);
	void CreatePartySyn(int Option);	

	// 070110 LYW --- Add function to setting option for pickup item.
	void SetPickupItem( int option ) ;
	// 071002 LYW --- Add function to return option of party.
	int GetPickupItem() { return m_PartyOption ; }

	void AddPartyMemberSyn(DWORD AddPartymemeberID); 
	void DelPartyMemberSyn();
	void BanPartyMemberSyn(DWORD DelPartymemberID);
	void ChangeMasterPartySyn(DWORD FromMemberID, DWORD ToMemberID);
	void BreakUpSyn();
	void PartyInviteAccept();
	void PartyInviteDeny();

	void SetRequestPartyID(DWORD id)		{ m_RequestPartyID = id;	}
	DWORD GetRequestPartyID()				{ return m_RequestPartyID;	}
		
	void CreatePartyResult(MSG_DWORDBYTE* pMsg);
	void AddPartyMemberResult(SEND_PARTY_MEMBER_INFO* pMsg); 
	void DelPartyMemberResult(MSG_DWORD* pMsg);
	void BanPartyMemberResult(MSG_DWORD* pMsg);
	void ChangeMasterPartyResult(MSG_DWORD* pMsg);
	void BreakUpPartyResult();

	void UserLogIn(SEND_PARTY_MEMBER_INFO* pMsg);
	void UserLogOut(MSG_DWORD* pMsg);
	
	void SetPartyMemberLifePercent(DWORD PlayerID, DWORD LifePercent);
	BYTE GetPartyMemberLifePercent(DWORD PlayerID);

	void SetPartyMemberManaPercent(DWORD PlayerID, DWORD ManaPercent);
	BYTE GetPartyMemberManaPercent(DWORD PlayerID);
	
	void SetPartyMemberLevel(DWORD PlayerID, LEVELTYPE lvl);
	LEVELTYPE GetPartyMemberLevel(DWORD PlayerID);

	PARTY_MEMBER* GetPartyMemberInfo(int i);

//	DWORD GetPartyIdx()		{	return m_Party.GetPartyIdx();	}
	DWORD GetMasterID()		{	return m_Party.GetMasterIdx();	}
	
	char* GetPartyMemberName(DWORD PlayerID) { return m_Party.GetMemberName(PlayerID); }
	DWORD GetPartyMemberID(int n) { return m_Party.GetMemberID(n); }

	BOOL IsMemberLogIn(int n)	{	return m_Party.IsLogIn(n);	}
		
	BOOL IsPartyMember(DWORD PlayerID);
	BOOL PartyChat(char* ChatMsg, char* SenderName);
	BOOL CanActivate();
	
	void SetIsProcessing(BOOL val) { m_bIsProcessing = val;	}
	BOOL IsProcessing() { return m_bIsProcessing;	}

	// 070109 LYW --- Add function to return party pointer.
	CParty* GetParty() { return &m_Party ; }

	// 070110 LYW --- Add function to setting value m_MasterChanging.
	//void SetChangingMaster( BOOL val ) { m_MasterChanging = val ; }

	// desc_hseos_데이트존_01
	// S 데이트존 추가 added by hseos 2007.11.13 2007.11.15
	BOOL IsPossibleEnterDateZone();
	BOOL IsPossibleEnterChallengeZone();
	// E 데이트존 추가 added by hseos 2007.11.13 2007.11.15

private:
	BOOL m_MasterChanging;

private:
	void SetMasterChanging(BOOL val) { m_MasterChanging = val;	}
	BOOL IsMasterChanging();

	// 070205 LYW --- Add functions to process network msg.
public :
	void Party_Info( void* pMsg ) ;
	void Party_Add_Invite( void* pMsg ) ;
	void Party_Add_Ack( void* pMsg ) ;
	void Party_Add_Nack( void* pMsg ) ;
	void Party_Del_Ack( void* pMsg ) ;
	void Party_Del_Nack( void* pMsg ) ;
	void Party_Create_Ack( void* pMsg ) ;
	void Party_Create_Nack( void* pMsg ) ;
	void Party_Ban_Ack( void* pMsg ) ;
	void Party_Member_Login( void* pMsg ) ;
	void Party_Member_Logout( void* pMsg ) ;
	void Party_ChangeMaster_Ack( void* pMsg ) ;
	void Party_Breakup_Ack( void* pMsg ) ;
	void Party_Breaup_Nack( void* pMsg ) ;
	void Party_MemverLife( void* pMsg ) ;
	void Party_MemberMana( void* pMsg ) ;
	void Party_MemberLevel( void* pMsg ) ;
	void Party_SendPos( void* pMsg ) ;
	void Party_RevivePos( void* pMsg ) ;
	void Party_Invite_Accept_Nack( void* pMsg ) ;
	void Party_Member_LoginMsg( void* pMsg ) ;
	void Party_Monster_Obtain_Notify( void* pMsg ) ;
	void Party_Change_PickupItem_Ack( void* pMsg ) ;
	// 071002 LYW --- PartyManager : Add function to parsing message to nack change option.
	void Party_Change_PickupItem_Nack( void* pMsg ) ;
};
EXTERNGLOBALTON(CPartyManager);

#endif

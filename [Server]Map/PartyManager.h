#pragma once 
#define PARTYMGR CPartyManager::GetInstance()

class CPlayer;
class CParty;
struct PARTYMEMBER;


class CPartyManager 
{
	CMemoryPoolTempl< CParty >* m_partyRoomPool;
	CYHHashTable<CParty> m_partyRoomHashTable;	
	CYHHashTable<CParty> m_tempPartyRoomHashTable;	

	CPartyManager();

public:
	GETINSTANCE(CPartyManager);

	virtual ~CPartyManager();

	CParty* GetParty(DWORD partyIDx); 
	
	CParty* RegistParty(DWORD PartyIDX);
	void CreatePartyQuery(CPlayer * pMaster, BYTE bOption);
	void CreatePartyResult(CPlayer * pMaster, DWORD PartyIDX, BYTE Option);

	// 070314 LYW --- PartyManager : Modified function CreateParty(...).
	//void CreateParty(CPlayer* pMaster, CPlayer* pMember, BYTE bOption);
	void CreateParty(CPlayer* pMaster, BYTE bOption) ;

	void AddMember(DWORD PartyID, DWORD TargetPlayerID);
	void AddMemberResult(CParty* pParty, CPlayer* pNewMember);
	void AddMemberNack(DWORD PartyID, DWORD TargetPlayerID);
	void DelMember(DWORD PlayerID,DWORD PartyID);
	void DelMemberSyn(DWORD PlayerID, DWORD PartyID);
	void RemoveParty(DWORD DeletedPlayerID, DWORD PartyID);
	void BanParty(DWORD PartyID,DWORD PlayerID,DWORD TargetPlayerID);
	void ChangeMasterParty(DWORD PartyID,DWORD PlayerID,DWORD TargetPlayerID);
	void DoChangeMasterParty(CParty* pParty, DWORD TargetID);
	void BreakupParty(DWORD PartyID,DWORD PlayerID);

	void UserLogIn(CPlayer* pPlayer,BOOL bNotifyUserLogin);
	void UserLogOut(CPlayer* pPlayer);

	void AddPartyInvite(DWORD MasterID, DWORD TargetID);

	// 유저가 로그인 했을때 파티가 없으면 DB에서 파티정보를 얻어왔을때 호출하는 함수
	void UserLogInAfterDBQuery(DWORD CharacterID,DWORD PartyIdx,PARTYMEMBER* PartyInfo, BYTE Option);

	void NotifyChangesOtherMapServer(DWORD TargetPlayerID,CParty* pParty, BYTE Protocol, DWORD PartyIDX, LEVELTYPE Level=0);
	void NotifyCreateParty(DWORD PartyIDX);
	void NotifyAddParty(SEND_CHANGE_PARTY_MEMBER_INFO* pmsg);
	void NotifyBanParty(SEND_CHANGE_PARTY_MEMBER_INFO* pmsg);
	void NotifyChangeMasterParty(SEND_CHANGE_PARTY_MEMBER_INFO* pmsg);
	void NotifyBreakupParty(SEND_CHANGE_PARTY_MEMBER_INFO* pmsg);
	void NotifyDelParty(SEND_CHANGE_PARTY_MEMBER_INFO* pmsg);
	void NotifyDelSyn(MSG_DWORD2* pmsg);
	void NotifyUserLogIn(SEND_CHANGE_PARTY_MEMBER_INFO* pmsg);
	void NotifyUserLogOut(SEND_CHANGE_PARTY_MEMBER_INFO* pmsg);
	void NotifyMemberLoginMsg(DWORD PartyID, DWORD PlayerID);
	
	void SendErrMsg(DWORD PlayerID, int ErrKind, BYTE Protocol);
	BOOL SendPartyMsg(DWORD PartyID, MSGBASE* pmsg, int msglen);
	// 071002 LYW --- PartyManager : Add function to send message to party member that changed option of party.
	void SendOptionMsg(MSG_DWORD2* pMsg) ;
	
	void MemberLevelUp(DWORD PartyIDX, DWORD PlayerIDX, LEVELTYPE lvl);
	void DoMemberLevelUp(DWORD PartyIDX, DWORD PlayerIDX, LEVELTYPE lvl);
	
	void SendObtainItemMsg(CPlayer* pPlayer, DWORD ObtainIdx);
};


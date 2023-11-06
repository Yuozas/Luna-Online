#include "stdafx.h"
#include "PartyManager.h"
#include "Party.h"
#include "MapDBMsgParser.h"
#include "UserTable.h"
#include "Network.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "Battle.h"
// desc_hseos_데이트 존_01
// S 데이트 존 추가 added by hseos 2007.11.30
#include "../hseos/Date/SHDateManager.h"
// E 데이트 존 추가 added by hseos 2007.11.30

CPartyManager::CPartyManager() 
{
	m_partyRoomPool = new CMemoryPoolTempl< CParty >;
	m_partyRoomPool->Init( 300, 100, "PartyRoomPool" );
    m_partyRoomHashTable.Initialize(256);	
	m_tempPartyRoomHashTable.Initialize(32);	
}

CPartyManager::~CPartyManager() 
{
	CParty* pInfo;
	m_partyRoomHashTable.SetPositionHead();
	while(pInfo = m_partyRoomHashTable.GetData())
	{
		m_partyRoomPool->Free( pInfo );
	}
	m_partyRoomHashTable.RemoveAll();

	m_tempPartyRoomHashTable.SetPositionHead();
	while(pInfo = m_tempPartyRoomHashTable.GetData())
	{
		m_partyRoomPool->Free( pInfo );
	}

	SAFE_DELETE( m_partyRoomPool );
}


CParty* CPartyManager::RegistParty(DWORD PartyIDX)
{
	CParty* pParty = m_partyRoomPool->Alloc();
	
	pParty->Init(PartyIDX, 0, "", NULL,0);

	if(m_partyRoomHashTable.GetData(PartyIDX))
	{
		char buf[128];
		sprintf(buf, "파티인덱스가 이미 있습니다: %d", PartyIDX);
		ASSERTMSG(0, buf);
		//to do
	}

	ASSERT(pParty);
	m_partyRoomHashTable.Add(pParty,PartyIDX);
	return pParty;
}

void CPartyManager::CreatePartyQuery(CPlayer * pMaster, BYTE bOption)
{
	/*
	if(pMaster->GetPartyIdx() != 0)
	{
		SendErrMsg(pMaster->GetID(), eErr_Create_MapHasParty, MP_PARTY_CREATE_NACK);
		return;
	}		

	PartyCreate(pMaster->GetID(), bOption); //db
	*/

	//SW060712 인덱스의 파티를
	//파티장일 경우 파티를 깨고
	//멤버일 경우 탈퇴 처리를 하자.

	DWORD PartyIdx = pMaster->GetPartyIdx();
	if(PartyIdx != 0)
	{
		int errorFlag = 0;
		CParty* pParty = GetParty( PartyIdx );

		if( pParty )
		{
			if( pParty->GetMasterID() == pMaster->GetID() )
			{
				errorFlag = 1;
				//breakup
				BreakupParty( PartyIdx, pMaster->GetID() );
			}
			else if( pParty->IsPartyMember(pMaster->GetID()) )
			{
				errorFlag = 2;
				//DelMember
				DelMember( pMaster->GetID(), PartyIdx );
			}
		}
		else
		{
			errorFlag = 3;
		}

		//우선 이곳에 들어온다면 로직 에러이다. 확인!
		char temp[128] = {0,};
		sprintf(temp, "PartyLogicError(%d)!! :: CharaterID: %d\tPartyIdx: %d\n", errorFlag, pMaster->GetID(), PartyIdx );
		ASSERTMSG(0, temp);

		//anyway .. party index set zero.
		pMaster->SetPartyIdx(0);
	}		

	PartyCreate(pMaster->GetID(), bOption); //db
}

void CPartyManager::AddPartyInvite(DWORD MasterID, DWORD TargetID)
{
	CPlayer* pMaster = (CPlayer*)g_pUserTable->FindUser(MasterID);
	if(!pMaster)
		return;
	CParty* pParty = GetParty(pMaster->GetPartyIdx());
	if(!pParty)
	{
		ASSERTMSG(0,"[파티]-멤버추가-파티가 없습니다");
		return;
	}

	if(pParty->IsPartyMember(MasterID) == FALSE)
		return;
	if(pParty->IsPartyMember(TargetID) == TRUE)
		return;

	
	MSG_DWORD msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_ADD_NACK;
	msg.dwData = 0;

	CPlayer* pTargetPlayer = (CPlayer*)g_pUserTable->FindUser(TargetID);	
	if(!pTargetPlayer)
	{
		msg.dwData = eErr_Add_NoPlayer;
	}
	else if(pTargetPlayer->GetPartyIdx() != 0)
		msg.dwData		= eErr_Add_AlreadyinParty;
	else if(pTargetPlayer->GetGameOption()->bNoParty)
		msg.dwData		= eErr_Add_OptionNoParty;
	else if(pTargetPlayer->GetState() == eObjectState_Die)
		msg.dwData		= eErr_Add_DiedPlayer;	
	else if( pMaster->GetBattle()->GetBattleKind() == eBATTLE_KIND_GTOURNAMENT )
	{
		// RaMa - 05.05.20   문파 토너먼트시 같은 문파만 파티를 맺을 수 있다.
		if(	pTargetPlayer->GetPartyIdx() != pMaster->GetPartyIdx() )
			msg.dwData = eErr_Add_NotOurGuild;
	}	
	if(msg.dwData > 0) //nack
		pMaster->SendMsg(&msg, sizeof(msg));
	else //ack
	{
		MSG_NAME_DWORD2 invitemsg;
		invitemsg.Category = MP_PARTY;
		invitemsg.Protocol = MP_PARTY_ADD_INVITE;
		invitemsg.dwData1	= pParty->GetPartyIdx();
		invitemsg.dwData2	= pParty->GetOption();
		SafeStrCpy(invitemsg.Name,pMaster->GetObjectName(), MAX_NAME_LENGTH+1);
		pTargetPlayer->SendMsg(&invitemsg,sizeof(invitemsg));

		MSGBASE rtmsg;
		rtmsg.Category = MP_PARTY;
		rtmsg.Protocol = MP_PARTY_INVITE_ACCEPT_ACK;
		pMaster->SendMsg(&rtmsg, sizeof(rtmsg));
	}
}

void CPartyManager::AddMember(DWORD PartyID, DWORD TargetPlayerID)
{
	CPlayer* pNewMember = (CPlayer*)g_pUserTable->FindUser(TargetPlayerID);
	if(!pNewMember)
		return;
	if(pNewMember->GetPartyIdx() != 0) 
	{	//	nack 처리 필요 
		ASSERT(0);
		return;
	}

	CParty* pParty = NULL;

	/// 임시파티가 있는 경우 - 최초 파티 생성
	pParty = m_tempPartyRoomHashTable.GetData( PartyID );

	if( pParty )
	{
		if( pParty->m_Member[1].MemberID == TargetPlayerID )
		{
			PartyCreate( pParty->GetMasterID(), pParty->GetOption() );
		}
	}
	else
	{
		pParty = GetParty(PartyID);
		if(pParty == NULL)
		{
			//	nack 처리 필요 - 파티가 해산되었을꺼다. 
			MSG_BYTE msg;
			msg.Category = MP_PARTY;
			msg.Protocol = MP_PARTY_INVITE_ACCEPT_NACK;
			msg.bData = eErr_BrokenParty;
			pNewMember->SendMsg(&msg, sizeof(msg));
			return;
		}
		if(pParty->IsPartyMember(pNewMember->GetID()) == TRUE)
			return;

		PartyAddMember(PartyID,TargetPlayerID); //DB update

		AddMemberResult(pParty, pNewMember);
	}
}

void CPartyManager::AddMemberResult(CParty* pParty, CPlayer* pNewMember)
{
	pNewMember->SetPartyIdx(pParty->GetPartyIdx());

	// 070420 LYW --- PartyMamager : Setting racial and gender.
	CHARACTER_TOTALINFO totalInfo ;
	memset(&totalInfo, 0, sizeof(CHARACTER_TOTALINFO)) ;

	pNewMember->GetCharacterTotalInfo(&totalInfo) ;

	SEND_PARTY_MEMBER_INFO smsg;
	smsg.Category = MP_PARTY;
	smsg.Protocol = MP_PARTY_ADD_ACK;
	smsg.MemberInfo.dwMemberID = pNewMember->GetID();	
	SafeStrCpy(smsg.MemberInfo.Name,pNewMember->GetObjectName(), MAX_NAME_LENGTH+1);
	smsg.MemberInfo.bLogged = TRUE;
	smsg.MemberInfo.LifePercent = pNewMember->GetLifePercent();
	smsg.MemberInfo.ManaPercent = pNewMember->GetManaPercent();
	smsg.MemberInfo.Level = pNewMember->GetLevel();	
	// 070420 LYW --- PartyMamager : Setting racial and gender.
	smsg.MemberInfo.Race	= totalInfo.Race ;
	smsg.MemberInfo.Gender	= totalInfo.Gender ;
	VECTOR3 vPos;
	pNewMember->GetPosition(&vPos);
	smsg.MemberInfo.posX = (WORD)vPos.x;
	smsg.MemberInfo.posZ = (WORD)vPos.z;
	smsg.PartyID = pParty->GetPartyIdx();
	pParty->SendMsgToAll(&smsg,sizeof(smsg));

	// 070420 LYW --- PartyManager : Modified call function AddPartyMember.
	//pParty->AddPartyMember(pNewMember->GetID(),pNewMember->GetObjectName(),pNewMember,smsg.MemberInfo.Level);
	pParty->AddPartyMember(pNewMember->GetID(),pNewMember->GetObjectName(),pNewMember,smsg.MemberInfo.Level, smsg.MemberInfo.Race, smsg.MemberInfo.Gender);
	
	NotifyChangesOtherMapServer(pNewMember->GetID(),pParty,MP_PARTY_NOTIFYADD_TO_MAPSERVER, pParty->GetPartyIdx(), smsg.MemberInfo.Level);
}

void CPartyManager::AddMemberNack(DWORD PartyID, DWORD TargetPlayerID)
{
	CParty* pParty = NULL;

	/// 임시파티가 있는 경우
	pParty = m_tempPartyRoomHashTable.GetData( PartyID );

	if( pParty )
	{
		if( pParty->m_Member[1].MemberID == TargetPlayerID )
		{
			m_partyRoomPool->Free( pParty );

			MSGBASE msg;
			msg.Category = MP_PARTY;
			msg.Protocol = MP_PARTY_INVITE_DENY_ACK;
			pParty->SendMsgToAll(&msg, sizeof(msg));
		}
	}
}

void CPartyManager::NotifyAddParty(SEND_CHANGE_PARTY_MEMBER_INFO* pmsg)
{
	CParty* pParty = GetParty(pmsg->PartyID);
	if(pParty ==NULL)
		return;
	
	if(pParty->GetMasterID() == 0) //파티 정보가 없으면
		return;

	// 070420 LYW --- PartyManager : Modified call function AddPartyMember.
	//pParty->AddPartyMember(pmsg->dwObjectID,pmsg->Name,NULL, pmsg->Level);
	pParty->AddPartyMember(pmsg->dwObjectID,pmsg->Name,NULL, pmsg->Level, pmsg->racial, pmsg->gender);

	SEND_PARTY_MEMBER_INFO smsg;
	smsg.Category = MP_PARTY;
	smsg.Protocol = MP_PARTY_ADD_ACK;
	smsg.MemberInfo.dwMemberID = pmsg->dwObjectID;
	SafeStrCpy(smsg.MemberInfo.Name,pmsg->Name, MAX_NAME_LENGTH+1);
	smsg.MemberInfo.bLogged = TRUE;
	smsg.MemberInfo.LifePercent = 0;
	smsg.MemberInfo.ManaPercent = 0;
	smsg.MemberInfo.Level = pmsg->Level;
	smsg.PartyID = pmsg->PartyID;

	// 070420 LYW --- PartyMamager : Setting racial and gender.
	smsg.MemberInfo.Race	= pmsg->racial ;
	smsg.MemberInfo.Gender	= pmsg->gender ;

	pParty->SendMsgToAll(&smsg,sizeof(smsg));
}

void CPartyManager::DelMember(DWORD TargetPlayerID,DWORD PartyID)
{
	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.30
	// ..챌린지 존에서는 탈퇴 불가
	if (g_csDateManager.IsChallengeZoneHere())
		return;
	// E 데이트 존 추가 added by hseos 2007.11.30

	CParty* pParty = GetParty(PartyID);
	if(pParty == NULL)
	{
		ASSERTMSG(0,"There is no Party.");
		return;
	}
	
	if(pParty->IsPartyMember(TargetPlayerID) == FALSE)
		return;
	if(pParty->GetMasterID() == 0) //파티 정보가 없으면
		return;

	MSG_DWORD smsg;
	smsg.Category = MP_PARTY;
	smsg.Protocol = MP_PARTY_DEL_ACK;
	smsg.dwData = TargetPlayerID;
	pParty->SendMsgToAll(&smsg,sizeof(smsg));

	NotifyChangesOtherMapServer(TargetPlayerID,pParty,MP_PARTY_NOTIFYDELETE_TO_MAPSERVER, PartyID);
	
	pParty->RemovePartyMember(TargetPlayerID);
	
	CPlayer * pPlayer = (CPlayer*)g_pUserTable->FindUser(TargetPlayerID);
	if(pPlayer)
		pPlayer->SetPartyIdx(0);

//	CharacterTablePartyIdxUpdate(TargetPlayerID); //로그 아웃 한 사람 ban할 때 캐릭터 테이블.partyidx 업데이트
}

void CPartyManager::NotifyDelParty(SEND_CHANGE_PARTY_MEMBER_INFO* pmsg)
{
	CParty* pParty = GetParty(pmsg->PartyID);
	if(pParty ==NULL)
		return;
	
	MSG_DWORD smsg;
	smsg.Category = MP_PARTY;
	smsg.Protocol = MP_PARTY_DEL_ACK;
	smsg.dwData = pmsg->dwObjectID;
	pParty->SendMsgToAll(&smsg,sizeof(smsg));
	
	pParty->RemovePartyMember(pmsg->dwObjectID);
	
	CPlayer * pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(pPlayer)
		pPlayer->SetPartyIdx(0);
}

void CPartyManager::BanParty(DWORD PartyID,DWORD PlayerID,DWORD TargetPlayerID) 
{
	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.30
	// ..챌린지 존에서는 추방 불가
	if (g_csDateManager.IsChallengeZoneHere())
		return;
	// E 데이트 존 추가 added by hseos 2007.11.30

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(PlayerID);
	if(pPlayer == NULL)
		return;

	DWORD PartyIdx = pPlayer->GetPartyIdx();
	CParty* pParty = GetParty(PartyIdx);
	if(pParty == NULL)
	{
		ASSERTMSG(0,"파티가 존재하지 않습니다.");//pjslocal
		return;
	}
	if(pParty->IsMasterChanging(pPlayer, MP_PARTY_BAN_NACK) == TRUE)
		return;
	
	if(pParty->IsPartyMember(pPlayer->GetID()) && pParty->IsPartyMember(TargetPlayerID))
	{		
		// 070315 LYW --- PartyManager : Modified function BanParty.
		/*
		MSG_DWORD smsg;
		smsg.Category = MP_PARTY;
		smsg.Protocol = MP_PARTY_BAN_ACK;
		smsg.dwData = TargetPlayerID;
		pParty->SendMsgToAll(&smsg,sizeof(smsg));
		
		CharacterTablePartyIdxUpdate(TargetPlayerID); //로그 아웃 한 사람 ban할 때 캐릭터 테이블.partyidx 업데이트
		
		CPlayer* pBanedPlayer = (CPlayer *)g_pUserTable->FindUser(TargetPlayerID);
		if(pBanedPlayer)
			pBanedPlayer->SetPartyIdx(0);

		NotifyChangesOtherMapServer(TargetPlayerID,pParty,MP_PARTY_NOTIFYBAN_TO_MAPSERVER, PartyID);

		pParty->RemovePartyMember(TargetPlayerID);	
		PartyDelMember(PartyID, TargetPlayerID);
		*/

		MSG_DWORD smsg;
		smsg.Category = MP_PARTY;
		smsg.Protocol = MP_PARTY_BAN_ACK;
		smsg.dwData = TargetPlayerID;
		pParty->SendMsgToAll(&smsg,sizeof(smsg));
		
		CharacterTablePartyIdxUpdate(TargetPlayerID); //로그 아웃 한 사람 ban할 때 캐릭터 테이블.partyidx 업데이트
		
		CPlayer* pBanedPlayer = (CPlayer *)g_pUserTable->FindUser(TargetPlayerID);
		if(pBanedPlayer)
			pBanedPlayer->SetPartyIdx(0);

		NotifyChangesOtherMapServer(TargetPlayerID,pParty,MP_PARTY_NOTIFYBAN_TO_MAPSERVER, PartyID);

		pParty->RemovePartyMember(TargetPlayerID);	
		PartyDelMember(PartyID, TargetPlayerID);

//		PartyMemberUpdate(pParty, PartyID);	
	}
	else
	{
		ASSERTMSG(0,"파티원이 아닌 사람이 Ban을시도했습니다");
	}
}
void CPartyManager::NotifyBanParty(SEND_CHANGE_PARTY_MEMBER_INFO* pmsg)
{
	CParty* pParty = GetParty(pmsg->PartyID);
	if(pParty == NULL)
		return;
	if(pParty->GetMasterID() == 0) //파티 정보가 없으면
		return;
	MSG_DWORD smsg;
	smsg.Category = MP_PARTY;
	smsg.Protocol = MP_PARTY_BAN_ACK;
	smsg.dwData = pmsg->dwObjectID;
	pParty->SendMsgToAll(&smsg,sizeof(smsg));
	
	pParty->RemovePartyMember(pmsg->dwObjectID);

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(pPlayer)
		pPlayer->SetPartyIdx(0);
}
void CPartyManager::ChangeMasterParty(DWORD PartyID,DWORD PlayerID,DWORD TargetPlayerID) 
{
	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.30
	// ..챌린지 존에서는 불가
	if (g_csDateManager.IsChallengeZoneHere())
		return;
	// E 데이트 존 추가 added by hseos 2007.11.30

	if(PlayerID == TargetPlayerID)
		return;
	
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(PlayerID);
	if(pPlayer == NULL)
		return;

	CParty* pParty = GetParty(pPlayer->GetPartyIdx());
	if(pParty == NULL)
	{
		ASSERTMSG(0,"파티가 존재하지 않습니다.");//pjslocal
		return;
	}

	if(pParty->IsPartyMember(pPlayer->GetID()) == FALSE)
		return;
	if(pParty->IsPartyMember(TargetPlayerID) == FALSE)
		return;
	if(pParty->GetMemberID(0) != pPlayer->GetID())		// 마스터가 아님
		return;

	PartyChangeMaster(pPlayer->GetPartyIdx(), PlayerID, TargetPlayerID);
	pParty->SetMasterChanging(TRUE);
/*	pParty->ChangeMaster(PlayerID,TargetPlayerID);

	MSG_DWORD smsg;
	smsg.Category = MP_PARTY;
	smsg.Protocol = MP_PARTY_CHANGEMASTER_ACK;
	smsg.dwData = TargetPlayerID;
	pParty->SendMsgToAll(&smsg,sizeof(smsg));

	PartyChangeMaster(PartyID, PlayerID, TargetPlayerID);
	
	NotifyChangesOtherMapServer(TargetPlayerID,pParty,MP_PARTY_NOTIFYCHANGEMASTER_TO_MAPSERVER, PartyID);
	*/
}
void CPartyManager::NotifyChangeMasterParty(SEND_CHANGE_PARTY_MEMBER_INFO* pmsg)
{	
	CParty* pParty = GetParty(pmsg->PartyID);
	if(pParty == NULL)
		return;
	if(pParty->GetMasterID() == 0) //파티 정보가 없으면
		return;
	DoChangeMasterParty(pParty, pmsg->dwObjectID);
/*	pParty->ChangeMaster(pParty->GetMasterID(),pmsg->dwObjectID);

	MSG_DWORD smsg;
	smsg.Category = MP_PARTY;
	smsg.Protocol = MP_PARTY_CHANGEMASTER_ACK;
	smsg.dwData = pmsg->dwObjectID;
	
	pParty->SendMsgToAll(&smsg,sizeof(smsg)); */
}
void CPartyManager::DoChangeMasterParty(CParty* pParty, DWORD TargetID)
{
	pParty->ChangeMaster(pParty->GetMasterID(),TargetID);

	MSG_DWORD smsg;
	smsg.Category = MP_PARTY;
	smsg.Protocol = MP_PARTY_CHANGEMASTER_ACK;
	smsg.dwData = TargetID;
	
	pParty->SendMsgToAll(&smsg,sizeof(smsg));
}
void CPartyManager::BreakupParty(DWORD PartyID,DWORD PlayerID) 
{
	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.30
	// ..챌린지 존에서는 해체 불가
	if (g_csDateManager.IsChallengeZoneHere())
		return;
	// E 데이트 존 추가 added by hseos 2007.11.30

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(PlayerID);
	if(pPlayer == NULL)
		return;

	if(pPlayer->GetPartyIdx() == 0)
	{
		ASSERTMSG(pPlayer->GetPartyIdx(), "파티가 없는 사람이 해산하려 했습니다");
		return;
	}
		

	CParty* pParty = GetParty(pPlayer->GetPartyIdx());
	if(pParty == NULL)
	{
		ASSERTMSG(0,"파티가 존재하지 않습니다.");//pjslocal
		return;
	}

	if(pParty->IsMasterChanging(pPlayer, MP_PARTY_BREAKUP_NACK) == TRUE)
		return;
		
	MSGBASE send_msg;
	send_msg.Category = MP_PARTY;
	send_msg.Protocol = MP_PARTY_BREAKUP_ACK;
	send_msg.dwObjectID = pPlayer->GetID();

	pParty->SendMsgToAll(&send_msg,sizeof(send_msg));
	
	PartyBreakup(pPlayer->GetPartyIdx(), pPlayer->GetID()); // DB
	
	NotifyChangesOtherMapServer(PlayerID,pParty,MP_PARTY_NOTIFYBREAKUP_TO_MAPSERVER, PartyID);
	
    pParty->BreakUp();

	m_partyRoomHashTable.Remove(PartyID);
	m_partyRoomPool->Free( pParty );
}
void CPartyManager::NotifyBreakupParty(SEND_CHANGE_PARTY_MEMBER_INFO* pmsg)
{	
	CParty* pParty = GetParty(pmsg->PartyID);
	if(pParty == NULL)
		return;

	MSGBASE send_msg;
	send_msg.Category = MP_PARTY;
	send_msg.Protocol = MP_PARTY_BREAKUP_ACK;
		
	pParty->SendMsgToAll(&send_msg,sizeof(send_msg));

	pParty->BreakUp();

	m_partyRoomHashTable.Remove(pmsg->PartyID);
	m_partyRoomPool->Free( pParty );
}

void CPartyManager::CreatePartyResult(CPlayer * pMaster, DWORD PartyIDX, BYTE Option)
{
	CParty* pParty = NULL;
	pParty = m_tempPartyRoomHashTable.GetData( pMaster->GetID() );

	/// 임시 파티가 있다면
	if( !pParty )
	{
		return;
	}

	/// 임시파티를 다시 초기화 한뒤 정식 파티로 등록한다
	pParty->Init( PartyIDX, pMaster->GetID(), pMaster->GetObjectName(), pMaster, Option );
	m_tempPartyRoomHashTable.Remove( pMaster->GetID() );
	m_partyRoomHashTable.Add(pParty,PartyIDX);

	pMaster->SetPartyIdx(PartyIDX);

	MSG_DWORDBYTE send_msg;
	send_msg.Category = MP_PARTY;
	send_msg.Protocol = MP_PARTY_CREATE_ACK;
	send_msg.dwObjectID = pMaster->GetID();
	send_msg.dwData = PartyIDX;
	send_msg.bData = Option;
	pMaster->SendMsg(&send_msg,sizeof(send_msg));

	pParty->SetMaster(pMaster);
	//	UserLogIn(pMaster);

	//Broadcasting
	MSG_DWORD msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_NOTIFYCREATE_TO_MAPSERVER;
	msg.dwObjectID = pMaster->GetID();
	msg.dwData = PartyIDX;

	g_Network.Send2AgentServer((char*)&msg, sizeof(msg));
//	g_Network.Broadcast2AgentServer((char*)&msg,sizeof(msg));

	/// 임시 맴버를 정식으로 등록한다.
	DWORD member = pParty->m_Member[1].MemberID;
	pParty->m_Member[1].MemberID = 0;
	AddMember( PartyIDX, member );
}

void CPartyManager::NotifyCreateParty(DWORD PartyIDX)
{
	CParty* pParty = m_partyRoomPool->Alloc();
	
	pParty->Init(PartyIDX, 0, "", NULL,0);

	if(m_partyRoomHashTable.GetData(PartyIDX))
	{
		ASSERTMSG(0, "notify : 파티인덱스가 이미 있습니다.");
		return;
	}		

	m_partyRoomHashTable.Add(pParty,PartyIDX);	
}

CParty* CPartyManager::GetParty(DWORD partyIDx)
{
	CParty* pParty = (CParty*)m_partyRoomHashTable.GetData(partyIDx);
	return pParty;
}

void CPartyManager::UserLogIn(CPlayer* pPlayer,BOOL bNotifyUserLogin)
{
	DWORD PartyID = pPlayer->GetPartyIdx();
	if(PartyID == 0)
		return;
	
	CParty* pParty = GetParty(PartyID);
	if(pParty == NULL)
	{
		char buf[128];
		sprintf(buf,"파티 인덱스가 맵서버에 없음. PartyIDX: %u, CharacterIdx: %u, MapNum : %d 어디가 꺼졌었나?", PartyID, pPlayer->GetID(), GAMERESRCMNGR->GetLoadMapNum());
		ASSERTMSG(0,buf);

		PartyInfoByUserLogin(PartyID,pPlayer->GetID());
		return;
	}
	if(pParty->GetMasterID() != 0) //파티 정보가 있으면 
		//파티원에게 자기 정보를 보내고 파티 정보를 받는다.
		pParty->UserLogIn(pPlayer,bNotifyUserLogin);
	
	else 
	{	// 파티 정보가 없으면, DB에 해당하는 파티의 세부 정보 요청
		PartyInfoByUserLogin(PartyID,pPlayer->GetID());
	}	
	
	if(!pPlayer->GetMapChangePoint()) //로그인이면 파티원들에게 메시지
	{
		pParty->SendMemberLogInMsg(pPlayer->GetID());
		pParty->NotifyMemberLogin(pPlayer->GetID());
	}

//	NotifyChangesOtherMapServer(pPlayer->GetID(),pParty,MP_PARTY_NOTIFYMEMBER_LOGIN_TO_MAPSERVER, PartyID);
}
void CPartyManager::NotifyUserLogIn(SEND_CHANGE_PARTY_MEMBER_INFO* pmsg)
{
	CParty* pParty = GetParty(pmsg->PartyID);
	if(pParty ==NULL)
		return;
	if(pParty->m_Member[0].MemberID == 0) //이 맵서버에 파티 세부 정보가 없으면
		return;
	pParty->NotifyUserLogIn(pmsg);
}
void CPartyManager::UserLogOut(CPlayer* pPlayer)
{
	DWORD PartyID = pPlayer->GetPartyIdx();
	if(PartyID == 0)
		return;

	CParty* pParty = GetParty(PartyID);
	if(pParty == NULL)
	{
		char buf[128];
		sprintf(buf, "파티가 존재하지 않습니다. PartyIdx: %d", PartyID);
		ASSERTMSG(0,buf);
		return;
	}
	pParty->UserLogOut(pPlayer->GetID()); 
	pParty->SendMsgUserLogOut(pPlayer->GetID());

	NotifyChangesOtherMapServer(pPlayer->GetID(),pParty,MP_PARTY_NOTIFYMEMBER_LOGOUT_TO_MAPSERVER, PartyID);
}

void CPartyManager::NotifyUserLogOut(SEND_CHANGE_PARTY_MEMBER_INFO* pmsg)
{
	CParty* pParty = GetParty(pmsg->PartyID);
	if(pParty == NULL)
		return;
	if(pParty->GetMasterID() == 0) //파티 정보가 없으면
		return;
	pParty->UserLogOut(pmsg->dwObjectID);
	pParty->SendMsgUserLogOut(pmsg->dwObjectID);
}

void CPartyManager::UserLogInAfterDBQuery(DWORD CharacterID,DWORD PartyIdx,PARTYMEMBER* PartyInfo, BYTE Option)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(CharacterID);
	if(pPlayer == NULL)
		return;

	CParty* pParty;
	pParty = GetParty(PartyIdx);
	if(!pParty)
	{
		pParty = RegistParty(PartyIdx);
	}

	pParty->SetOption(Option);
	for(int i=0; i<MAX_PARTY_LISTNUM; ++i)
	{
		pParty->m_Member[i].bLogged = PartyInfo[i].bLogged;
		pParty->m_Member[i].MemberID = PartyInfo[i].MemberID;
		SafeStrCpy(pParty->m_Member[i].Name, PartyInfo[i].Name, MAX_NAME_LENGTH+1);

		pParty->m_Member[i].Level = PartyInfo[i].Level;
	}

	pParty->UserLogIn(pPlayer,TRUE);
}

void CPartyManager::DelMemberSyn(DWORD PlayerID, DWORD PartyID)
{
	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.30
	// ..챌린지 존에서는 탈퇴 불가
	if (g_csDateManager.IsChallengeZoneHere())
		return;
	// E 데이트 존 추가 added by hseos 2007.11.30

	CParty* pParty = GetParty(PartyID);
	if(pParty == NULL)
	{
		ASSERT(0);
		return;
	}

	PartyDelMember(PartyID, PlayerID);
		

/*	DWORD FirstOnlineMemberID = pParty->GetFirstOnlineMemberID();

	if(FirstOnlineMemberID == NULL)
		ASSERTMSG(0,"온라인 멤버가(자신포함) 한명도 없습니다.");

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(FirstOnlineMemberID);
	if(pPlayer)
		DelMember(PlayerID,PartyID);
	else
	{
		MSG_DWORD2 msg;
		msg.Category = MP_PARTY;
		msg.Protocol = MP_PARTY_SYNDELETE_TO_MAPSERVER;
		msg.dwObjectID = FirstOnlineMemberID;
		msg.dwData1 = PlayerID;
		msg.dwData2 = PartyID;

		g_Network.Broadcast2AgentServer((char*)&msg,sizeof(msg));
	}
	*/
}

void CPartyManager::NotifyDelSyn(MSG_DWORD2* pmsg)
{	
	DelMember(pmsg->dwData1,pmsg->dwData2);
}
void CPartyManager::NotifyChangesOtherMapServer(DWORD TargetPlayerID,CParty* pParty, BYTE Protocol, DWORD PartyIDX, LEVELTYPE Level)
{
	SEND_CHANGE_PARTY_MEMBER_INFO msg;
	msg.Category = MP_PARTY;
	msg.Protocol = Protocol;
	msg.dwObjectID = TargetPlayerID;
	msg.PartyID = PartyIDX;
	SafeStrCpy(msg.Name,pParty->GetMemberName(TargetPlayerID), MAX_NAME_LENGTH+1);
	msg.Level = Level;

	//YH2DO no -_-+++++++
	g_Network.Send2AgentServer((char*)&msg,sizeof(msg));
}

void CPartyManager::RemoveParty(DWORD DeletedPlayerID, DWORD PartyID)
{
	CParty* pParty = GetParty(PartyID);
	if(pParty == NULL)
		return;
	pParty->RemovePartyMember(DeletedPlayerID);
}


void CPartyManager::SendErrMsg(DWORD PlayerID, int ErrKind, BYTE Protocol)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(PlayerID);
	if(pPlayer == NULL)
		return;

	char buf[128];
	sprintf(buf, "PartyErrNum: %d, CharacterIdx: %d", ErrKind, PlayerID);
	ASSERTMSG(0, buf);

	//플레이어에게 에러 메시지 보냄. 
	MSG_BYTE msg;
	msg.Category = MP_PARTY;
	msg.Protocol = Protocol;
	msg.bData = ErrKind;
	pPlayer->SendMsg(&msg, sizeof(msg));
}

BOOL CPartyManager::SendPartyMsg(DWORD PartyID, MSGBASE* pmsg, int msglen)
{
	CParty* pParty = GetParty(PartyID);
	if(pParty == NULL)
		return FALSE;

	pParty->SendMsgToAll(pmsg,msglen);
	return TRUE;
}

// 071002 LYW --- PartyManager : Add function to send message to party member that changed option of party.
void CPartyManager::SendOptionMsg(MSG_DWORD2* pMsg)
{
	if( !pMsg ) return ;

	/*CPlayer* pMaster = (CPlayer*)g_pUserTable->FindUser( pMsg->dwObjectID ) ;
	CParty* pParty = GetParty(pMaster->GetPartyIdx());*/

	CParty* pParty = GetParty(pMsg->dwData1) ;
	if( !pParty ) return ;

	pParty->SendMsgToMember(pMsg);
}

void CPartyManager::NotifyMemberLoginMsg(DWORD PartyID, DWORD PlayerID)
{
	CParty* pParty = GetParty(PartyID);
	if(pParty == NULL)
		return;

	pParty->SendMemberLogInMsg(PlayerID);
}

void CPartyManager::MemberLevelUp(DWORD PartyIDX, DWORD PlayerIDX, LEVELTYPE lvl)
{
	DoMemberLevelUp(PartyIDX, PlayerIDX, lvl);
	
	MSG_DWORD3 msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_NOTIFYMEMBER_LEVEL;
	msg.dwData1 = PartyIDX;
	msg.dwData2 = PlayerIDX;
	msg.dwData3 = lvl;	
	g_Network.Send2AgentServer((char*)&msg,sizeof(msg));	
}

void CPartyManager::DoMemberLevelUp(DWORD PartyIDX, DWORD PlayerIDX, LEVELTYPE lvl)
{
	CParty* pParty = PARTYMGR->GetParty(PartyIDX);
	if(pParty == NULL)
		return;
	
	if(pParty->GetMasterID() == 0) //파티 멤버 정보가 없으면
		return;
	pParty->SetMemberLevel(PlayerIDX, lvl);

	MSG_DWORD2 msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_MEMBERLEVEL;
	msg.dwData1 = PlayerIDX;
	msg.dwData2 = lvl;
	pParty->SendMsgExceptOne(&msg, sizeof(msg), PlayerIDX);
}

void CPartyManager::SendObtainItemMsg(CPlayer* pPlayer, DWORD ObtainIdx)
{
	CParty* pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx());
	if(pParty == NULL)
		return;
	MSG_DWORD2 msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_MONSTER_OBTAIN_NOTIFY;
	msg.dwData1 = pPlayer->GetID();
	msg.dwData2 = ObtainIdx;
	pParty->SendMsgExceptOneinChannel(&msg, sizeof(msg), pPlayer->GetID(), pPlayer->GetGridID());
}

// 070314 LYW --- PartyManager : Modified function CreateParty(...).
/*
void CPartyManager::CreateParty(CPlayer* pMaster, CPlayer* pMember, BYTE bOption)
{
	/// 임시 파티 생성
	CParty* pParty = m_partyRoomPool->Alloc();
	pParty->Init(0, pMaster->GetID(), "", NULL,bOption);
	m_tempPartyRoomHashTable.Add( pParty, pMaster->GetID() );

	/// 파티에 일단 추가
	pParty->m_Member[1].MemberID = pMember->GetID();

	/// 상대에게 초대 메세지를 보낸다.
	MSG_NAME_DWORD2 invitemsg;
	invitemsg.Category = MP_PARTY;
	invitemsg.Protocol = MP_PARTY_ADD_INVITE;
	invitemsg.dwData1	= pMaster->GetID();
	invitemsg.dwData2	= pParty->GetOption();
	SafeStrCpy(invitemsg.Name,pMaster->GetObjectName(), MAX_NAME_LENGTH+1);
	pMember->SendMsg(&invitemsg,sizeof(invitemsg));

	/// 자신에게 초대 성공 메세지를 보낸다.
	MSGBASE rtmsg;
	rtmsg.Category = MP_PARTY;
	rtmsg.Protocol = MP_PARTY_INVITE_ACCEPT_ACK;
	pMaster->SendMsg(&rtmsg, sizeof(rtmsg));
}
*/
void CPartyManager::CreateParty(CPlayer* pMaster, BYTE bOption)
{
	/// 임시 파티 생성
	CParty* pParty = m_partyRoomPool->Alloc();
	pParty->Init(0, pMaster->GetID(), "", NULL,bOption);
	m_tempPartyRoomHashTable.Add( pParty, pMaster->GetID() );

	PartyCreate( pParty->GetMasterID(), pParty->GetOption() );
}

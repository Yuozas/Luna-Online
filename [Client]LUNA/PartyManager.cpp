#include "stdafx.h"
#include "GameIn.h"
#include "PartyManager.h"

#include "WindowIdEnum.h"
#include "./interface/cWindowManager.h"
#include "objectmanager.h"
#include "ChatManager.h"
#include "cMsgBox.h"
#include "ItemManager.h"
#include "GameEventManager.h"
#include "MiniMapDlg.h"
#include "PartyIconManager.h"
#include "PartyCreateDlg.h"
#include "PartyInviteDlg.h"
#include "StatusIconDlg.h"
#include "CharacterDialog.h"

// 071002 LYW --- PartyManager : Include header file of checkbox class.
#include "./Interface/cCheckBox.h"

// 071002 LYW --- PartyManager : Include header file of checkbox class.
#include "PartyBtnDlg.h"

// 070420 LYW --- PartyManager : Include header file of CPartyMemberDlg.
#include "PartyMemberDlg.h"

// 071004 LYW --- PartyManager : Include header file of CGuageDialog.
#include "GuageDialog.h"

// desc_hseos_데이트존_01
// S 데이트존 추가 added by hseos 2007.11.13
#include ".\datematchingdlg.h"
// E 데이트존 추가 added by hseos 2007.11.13

// desc_hseos_데이트 존_01
// S 데이트 존 추가 added by hseos 2008.01.10
#include "../hseos/Date/SHDateManager.h"
// E 데이트 존 추가 added by hseos 2008.01.10

GLOBALTON(CPartyManager);

// 070226 LYW --- PartyManager : Modified message number.

CPartyManager::CPartyManager()
{
}

CPartyManager::~CPartyManager()
{
}

void CPartyManager::Init()
{
	m_MasterChanging = FALSE;
	m_Party.Clear();
	m_bIsProcessing = FALSE;
	m_RequestPartyID = 0;
//	GAMEIN->GetPartyDialog()->Init();

	m_PartyOption = e_OPTION_RANDOM ;
}

void CPartyManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	if( GAMEIN->GetPartyDialog() == NULL )
		return;
	if(HERO == NULL)
		return;

	switch(Protocol) 
	{
	case MP_PARTY_INFO: 						Party_Info( pMsg ) ;										break;		
	case MP_PARTY_ADD_INVITE:					Party_Add_Invite( pMsg ) ;									break;	
	case MP_PARTY_ADD_ACK:						Party_Add_Ack( pMsg ) ;										break; 
	case MP_PARTY_ADD_NACK:						Party_Add_Nack( pMsg ) ;									break;		
	case MP_PARTY_DEL_ACK:						Party_Del_Ack( pMsg ) ;										break;		
	case MP_PARTY_DEL_NACK:						Party_Del_Nack( pMsg ) ;									break;		
	case MP_PARTY_CREATE_ACK:					Party_Create_Ack( pMsg ) ;									break;		
	case MP_PARTY_CREATE_NACK:					Party_Create_Nack( pMsg ) ;									break;		
	case MP_PARTY_BAN_ACK:						Party_Ban_Ack( pMsg ) ;										break;		
	case MP_PARTY_BAN_NACK:																					break;		
	case MP_PARTY_MEMBER_LOGIN:					Party_Member_Login( pMsg ) ;								break;		
	case MP_PARTY_MEMBER_LOGOUT:				Party_Member_Logout( pMsg ) ;								break;		
	case MP_PARTY_CHANGEMASTER_ACK:				Party_ChangeMaster_Ack( pMsg ) ;							break;		
	case MP_PARTY_CHANGEMASTER_NACK:			ASSERT(0); SetMasterChanging(FALSE);						break;		
	case MP_PARTY_BREAKUP_ACK:					Party_Breakup_Ack( pMsg ) ;									break;		
	case MP_PARTY_BREAKUP_NACK:					Party_Breaup_Nack( pMsg ) ;									break;
	case MP_PARTY_MEMBERLIFE:					Party_MemverLife( pMsg ) ;									break;
	case MP_PARTY_MEMBERMANA:					Party_MemberMana( pMsg ) ;									break;
	case MP_PARTY_MEMBERLEVEL:					Party_MemberLevel( pMsg ) ;									break;
	case MP_PARTY_SENDPOS:						Party_SendPos( pMsg ) ;										break;
	case MP_PARTY_REVIVEPOS:					Party_RevivePos( pMsg ) ;									break;
	case MP_PARTY_INVITE_DENY_ACK:				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(407) );	break;
	case MP_PARTY_INVITE_ACCEPT_ACK:			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(529) );		break;
	case MP_PARTY_INVITE_ACCEPT_NACK:			Party_Invite_Accept_Nack( pMsg ) ;							break;
	case MP_PARTY_MEMBER_LOGINMSG:				Party_Member_LoginMsg( pMsg ) ;								break;
	case MP_PARTY_MONSTER_OBTAIN_NOTIFY:		Party_Monster_Obtain_Notify( pMsg ) ;						break;
	case MP_PARTY_CHANGE_PICKUPITEM_ACK :		Party_Change_PickupItem_Ack( pMsg ) ;						break;		
	case MP_PARTY_CHANGE_PICKUPITEM_NACK :		Party_Change_PickupItem_Nack( pMsg ) ;						break;
	} 
}

void CPartyManager::CreatePartySyn(int Option) 
{
	if(HERO->GetPartyIdx() != 0)
	{
		ASSERT(0);
		return;
	}
	MSG_BYTE msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_CREATE_SYN;
	msg.dwObjectID = HEROID;
	msg.bData = Option;
	NETWORK->Send(&msg,sizeof(msg));

	SetIsProcessing(TRUE);
}

// 070110 LYW --- Add function to setting option for pickup item.
void CPartyManager::SetPickupItem( int option )
{
	MSG_DWORD2 msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_CHANGE_PICKUPITEM_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData1 = option;
	msg.dwData2 = HERO->GetPartyIdx();
	NETWORK->Send(&msg, sizeof(msg));
}

void CPartyManager::AddPartyMemberSyn(DWORD AddPartymemeberID) 
{
	if(AddPartymemeberID == 0)
	{
		ASSERT(0);
		return;
	}
	if(m_Party.IsPartyMember(AddPartymemeberID) == TRUE)
		return;

	if(m_Party.GetMemberNum() != MAX_PARTY_LISTNUM)
	{
		MSG_DWORD2 msg;
		msg.Category	= MP_PARTY;
		msg.Protocol	= MP_PARTY_ADD_SYN;
		msg.dwObjectID	= HEROID;
		msg.dwData1		= AddPartymemeberID;			
		msg.dwData2		= HERO->GetPartyIdx();
		NETWORK->Send(&msg,sizeof(msg));
	}
	else 
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(110) );
}

void CPartyManager::DelPartyMemberSyn() 
{
	if(HEROID == GetMasterID())
		ASSERTMSG(0,"방장은 탈퇴를 할 수 없다.");//pjslocal
	
	MSG_DWORD msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_DEL_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData = HERO->GetPartyIdx();
	NETWORK->Send(&msg,sizeof(msg));
}

void CPartyManager::ChangeMasterPartySyn(DWORD FromMemberID, DWORD ToMemberID) 
{
	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2008.01.10
	if (g_csDateManager.IsChallengeZoneHere())
	{
		return;
	}
	// E 데이트 존 추가 added by hseos 2008.01.10

	if(ToMemberID == 0)
		return;
	if(ToMemberID == FromMemberID)
		return;
	if(m_Party.IsPartyMember(ToMemberID) == FALSE)
		return;
	
	MSG_DWORD2 msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_CHANGEMASTER_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData1 = ToMemberID;
	msg.dwData2 = HERO->GetPartyIdx();
	NETWORK->Send(&msg, sizeof(msg));
	
	SetMasterChanging(TRUE);	
}

void CPartyManager::BreakUpSyn() 
{
	if(IsMasterChanging() == TRUE)
		return;
	if(IsProcessing() == TRUE)
		return;

	MSG_DWORD msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_BREAKUP_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData = HERO->GetPartyIdx();
	NETWORK->Send(&msg,sizeof(msg));

	SetIsProcessing(TRUE);
}

void CPartyManager::BanPartyMemberSyn(DWORD DelPartymemberID) 
{
	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2008.01.10
	if (g_csDateManager.IsChallengeZoneHere())
	{
		return;
	}
	// E 데이트 존 추가 added by hseos 2008.01.10

	if(DelPartymemberID == 0)
		return;
	if(DelPartymemberID == HEROID)
		return;

	if(IsMasterChanging() == TRUE)
		return;

	if(m_Party.IsPartyMember(DelPartymemberID) == FALSE)
		return;
	MSG_DWORD2 msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_BAN_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData1 = DelPartymemberID;
	msg.dwData2 = HERO->GetPartyIdx();
	NETWORK->Send(&msg,sizeof(msg));

	// 070315 LYW --- PartyManager : Call fucntion to remove member.
	m_Party.RemovieMember(DelPartymemberID) ;
}

void CPartyManager::CreatePartyResult(MSG_DWORDBYTE* pMsg)
{
	m_Party.Clear();
//	m_Party.SetPartyIdx(pMsg->dwData);
	HERO->SetPartyIdx(pMsg->dwData);
	m_Party.SetOption(pMsg->bData);
	m_Party.SetMaster(HEROID,HERO->GetObjectName(),TRUE,HERO->GetLifePercent(),HERO->GetManaPercent());

	// 071004 LYW --- Party : Setting master mark on of hero guage.
	CGuageDialog* pGuageDlg = NULL ;							// HERO 게이지를 받을 포인터를 선언하고 null 처리를 한다.
	pGuageDlg = GAMEIN->GetGuageDlg() ;							// HERO 게이지를 받는다.

	if( pGuageDlg )												// HERO 게이지 정보가 유효하다면,
	{
		if( HEROID == GetMasterID() )							// HERO 가 파티 마스터라면,
		{
			pGuageDlg->SetRenderMasterMark(TRUE) ;				// 파티 마스터 마크 표시를 활성화 한다.
		}
		else
		{
			pGuageDlg->SetRenderMasterMark(FALSE) ;				// 파티 마스터 마크 표시를 비활성화 한다.
		}
	}

	SetIsProcessing(FALSE);
	GAMEIN->GetPartyDialog()->RefreshDlg();
	
	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(96) );
	GAMEIN->GetPartyDialog()->SetActive(TRUE);
	
	OBJECTMGR->ApplyOverInfoOption( HERO );//

	CPartyBtnDlg* pPartySet = NULL ;
	pPartySet = (CPartyBtnDlg*)WINDOWMGR->GetWindowForID( PA_BTNDLG );

	if( pPartySet )
	{
		pPartySet->SetCheckBox(pMsg->bData) ;
	}
}

void CPartyManager::AddPartyMemberResult(SEND_PARTY_MEMBER_INFO* pMsg) 
{
	if(pMsg->MemberInfo.dwMemberID == HEROID) //추가된 사람
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(98) );
		HERO->SetPartyIdx(pMsg->PartyID);

		OBJECTMGR->ApplyOverInfoOptionToAll();//--setpartyinfo와 두번겹치는가? confirm
	}
	else //방파원들
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(97), pMsg->MemberInfo.Name);
		m_Party.AddMember(&pMsg->MemberInfo);

		CObject* pMember = OBJECTMGR->GetObject( pMsg->MemberInfo.dwMemberID );
		OBJECTMGR->ApplyOverInfoOption( pMember );//

		// 파티원 아이콘 추가.
		if(pMsg->MemberInfo.bLogged && pMsg->MemberInfo.LifePercent)
		{
			PICONMGR->CreatePartyIcon(pMsg->MemberInfo.dwMemberID, &pMsg->MemberInfo);
		}

		// desc_hseos_데이트존_01
		// S 데이트존 추가 added by hseos 2007.11.13 2007.11.15
		// ..데이트 매칭 채팅 중이라면 데이트 존 이동 버튼을 활성화시킨다.
		if (IsPossibleEnterDateZone())
		{
			CDateMatchingDlg* pDlg = GAMEIN->GetDateMatchingDlg() ;
			pDlg->ActivateBtnEnterDateZone(TRUE);
		}
		else
		{
			CDateMatchingDlg* pDlg = GAMEIN->GetDateMatchingDlg() ;
			pDlg->ActivateBtnEnterDateZone(FALSE);
		}

		// ..데이트 매칭 채팅 중이라면 챌린지 존 이동 버튼을 활성화시킨다.
		if (IsPossibleEnterChallengeZone())
		{
			CDateMatchingDlg* pDlg = GAMEIN->GetDateMatchingDlg() ;
			pDlg->ActivateBtnEnterChallengeZone(TRUE);
		}
		else
		{
			CDateMatchingDlg* pDlg = GAMEIN->GetDateMatchingDlg() ;
			pDlg->ActivateBtnEnterChallengeZone(FALSE);
		}
		// E 데이트존 추가 added by hseos 2007.11.13 2007.11.15
	}
	
	GAMEIN->GetPartyDialog()->RefreshDlg();

	// 080318 LUJ,	파티에 추가된 플레이어의 스킬을 검색해서 추가해준다. 스킬 정보가 따로 서버에서 전송되지 않으므로 클라이언트에서 찾는다
	GAMEIN->GetPartyDialog()->InitializeSkill( pMsg->MemberInfo );
}

void CPartyManager::DelPartyMemberResult(MSG_DWORD* pMsg) 
{
	if(pMsg->dwData == HEROID) 
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(103) );
		HERO->SetPartyIdx(0);
		m_Party.Clear();

		GAMEIN->GetPartyDialog()->SetActive(FALSE);
		OBJECTMGR->ApplyOverInfoOptionToAll();//
	}
	else 
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(102), m_Party.GetMemberName(pMsg->dwData));
		m_Party.DelMember(pMsg->dwData);		
		CObject* pMember = OBJECTMGR->GetObject( pMsg->dwData );
		OBJECTMGR->ApplyOverInfoOption( pMember );//

		// desc_hseos_데이트존_01
		// S 데이트존 추가 added by hseos 2007.11.13 2007.11.15
		// ..데이트 매칭 채팅 중이라면 데이트 존 이동 버튼을 활성화시킨다.
		if (IsPossibleEnterDateZone())
		{
			CDateMatchingDlg* pDlg = GAMEIN->GetDateMatchingDlg() ;
			pDlg->ActivateBtnEnterDateZone(TRUE);
		}
		else
		{
			CDateMatchingDlg* pDlg = GAMEIN->GetDateMatchingDlg() ;
			pDlg->ActivateBtnEnterDateZone(FALSE);
		}

		// ..데이트 매칭 채팅 중이라면 챌린지 존 이동 버튼을 활성화시킨다.
		if (IsPossibleEnterChallengeZone())
		{
			CDateMatchingDlg* pDlg = GAMEIN->GetDateMatchingDlg() ;
			pDlg->ActivateBtnEnterChallengeZone(TRUE);
		}
		else
		{
			CDateMatchingDlg* pDlg = GAMEIN->GetDateMatchingDlg() ;
			pDlg->ActivateBtnEnterChallengeZone(FALSE);
		}
		// E 데이트존 추가 added by hseos 2007.11.13 2007.11.15
	}
	GAMEIN->GetPartyDialog()->RefreshDlg();
}

void CPartyManager::BanPartyMemberResult(MSG_DWORD* pMsg) 
{
	if(pMsg->dwData == HEROID)
	{
//		"[파티] 넌 쫒겨났어.");		
		GAMEIN->GetPartyDialog()->SetActive(FALSE);
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(106) );
		HERO->SetPartyIdx(0);
		m_Party.Clear();
		
		OBJECTMGR->ApplyOverInfoOptionToAll();//

		PICONMGR->DeleteAllPartyIcon();
	}
	else 
	{
//		"[파티] %s를 추방했다.",m_Party.GetMemberName(pMsg->dwData));//pjslocal
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(105), m_Party.GetMemberName(pMsg->dwData));
		
		m_Party.DelMember(pMsg->dwData);

		CObject* pMember = OBJECTMGR->GetObject( pMsg->dwData );
		OBJECTMGR->ApplyOverInfoOption( pMember );//

		PICONMGR->DeletePartyIcon(pMsg->dwData);

		// desc_hseos_데이트존_01
		// S 데이트존 추가 added by hseos 2007.11.13 2007.11.15
		// ..데이트 매칭 채팅 중이라면 데이트 존 이동 버튼을 활성화시킨다.
		if (IsPossibleEnterDateZone())
		{
			CDateMatchingDlg* pDlg = GAMEIN->GetDateMatchingDlg() ;
			pDlg->ActivateBtnEnterDateZone(TRUE);
		}
		else
		{
			CDateMatchingDlg* pDlg = GAMEIN->GetDateMatchingDlg() ;
			pDlg->ActivateBtnEnterDateZone(FALSE);
		}
		// ..데이트 매칭 채팅 중이라면 챌린지 존 이동 버튼을 활성화시킨다.
		if (IsPossibleEnterChallengeZone())
		{
			CDateMatchingDlg* pDlg = GAMEIN->GetDateMatchingDlg() ;
			pDlg->ActivateBtnEnterChallengeZone(TRUE);
		}
		else
		{
			CDateMatchingDlg* pDlg = GAMEIN->GetDateMatchingDlg() ;
			pDlg->ActivateBtnEnterChallengeZone(FALSE);
		}
		// E 데이트존 추가 added by hseos 2007.11.13 2007.11.15
	}
	GAMEIN->GetPartyDialog()->SetClickedMemberID(0);
	GAMEIN->GetPartyDialog()->RefreshDlg();
}

void CPartyManager::UserLogIn(SEND_PARTY_MEMBER_INFO* pMsg) 
{
	m_Party.LogIn(&pMsg->MemberInfo);
	GAMEIN->GetPartyDialog()->RefreshDlg();
}

void CPartyManager::UserLogOut(MSG_DWORD* pMsg) 
{
	m_Party.LogOut(pMsg->dwData);
	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(404), m_Party.GetMemberName(pMsg->dwData));

	GAMEIN->GetPartyDialog()->RefreshDlg();
}

void CPartyManager::ChangeMasterPartyResult(MSG_DWORD* pMsg) 
{
	m_Party.ChangeMaster(pMsg->dwData);

	// 071004 LYW --- Party : Setting master mark on of hero guage.
	CGuageDialog* pGuageDlg = NULL ;							// HERO 게이지를 받을 포인터를 선언하고 null 처리를 한다.
	pGuageDlg = GAMEIN->GetGuageDlg() ;							// HERO 게이지를 받는다.

	if( pGuageDlg )												// HERO 게이지 정보가 유효하다면,
	{
		if( HEROID == GetMasterID() )							// HERO 가 파티 마스터라면,
		{
			pGuageDlg->SetRenderMasterMark(TRUE) ;				// 파티 마스터 마크 표시를 활성화 한다.

			CPartyMemberDlg* pMemberDlg ;

			for(int count = 0 ; count < MAX_PARTY_LISTNUM ; count++)
			{
				pMemberDlg = NULL ;
				pMemberDlg = GAMEIN->GetPartyDialog()->GetMemberDlg(count) ;

				if( !pMemberDlg ) continue ;

				pMemberDlg->SetRenderMasterMark(FALSE) ;
			}
		}
		else
		{
			pGuageDlg->SetRenderMasterMark(FALSE) ;				// 파티 마스터 마크 표시를 비활성화 한다.
		}
	}

	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(406), m_Party.GetMemberName(pMsg->dwData) );
	GAMEIN->GetPartyDialog()->RefreshDlg();
}

void CPartyManager::BreakUpPartyResult()
{
	HERO->SetPartyIdx(0);
	m_Party.Clear();
	m_Party.BreakUpResult();
	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 104 ) );
	GAMEIN->GetPartyDialog()->RefreshDlg();
	GAMEIN->GetPartyDialog()->SetActive(FALSE);
	SetIsProcessing(FALSE);

	OBJECTMGR->ApplyOverInfoOptionToAll();//

	// 071004 LYW --- Party : Setting master mark on of hero guage.
	CGuageDialog* pGuageDlg = NULL ;							// HERO 게이지를 받을 포인터를 선언하고 null 처리를 한다.
	pGuageDlg = GAMEIN->GetGuageDlg() ;							// HERO 게이지를 받는다.

	if( pGuageDlg )												// HERO 게이지 정보가 유효하다면,
	{
		pGuageDlg->SetRenderMasterMark(FALSE) ;					// 파티 마스터 마크 표시를 비활성화 한다.
	}

	// desc_hseos_데이트존_01
	// S 데이트존 추가 added by hseos 2007.11.13 2007.11.15
	CDateMatchingDlg* pDlg = GAMEIN->GetDateMatchingDlg() ;
	if( pDlg )
	{
		pDlg->ActivateBtnEnterDateZone(FALSE);
		pDlg->ActivateBtnEnterChallengeZone(FALSE);
	}
	// E 데이트존 추가 added by hseos 2007.11.13 2007.11.15
}

void CPartyManager::SetPartyInfo(PARTY_INFO* pmsg) 
{
	m_Party.InitParty(pmsg);
	
	OBJECTMGR->ApplyOverInfoOptionToAll();//로그인이나 파티에 들어왔을 때
}

void CPartyManager::SetPartyMemberLifePercent(DWORD PalyerID, DWORD LifePercent)
{
	m_Party.SetMemberLifePercent(PalyerID, LifePercent);
}

void CPartyManager::SetPartyMemberManaPercent(DWORD PlayerID, DWORD ManaPercent)
{
	m_Party.SetMemberManaPercent(PlayerID, ManaPercent);
}

void CPartyManager::SetPartyMemberLevel(DWORD PlayerID, LEVELTYPE lvl)
{
	m_Party.SetMemberLevel(PlayerID, lvl);
}

BYTE CPartyManager::GetPartyMemberLifePercent(DWORD PlayerID)
{
	return m_Party.GetMemberLifePercent(PlayerID);
}

BYTE CPartyManager::GetPartyMemberManaPercent(DWORD PlayerID)
{
	return m_Party.GetMemberManaPercent(PlayerID);
}

LEVELTYPE CPartyManager::GetPartyMemberLevel(DWORD PlayerID)
{
	return m_Party.GetMemberLevel(PlayerID);
}

void CPartyManager::PartyInviteAccept()
{
	MSG_DWORD msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_INVITE_ACCEPT_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData = GetRequestPartyID();
	
	NETWORK->Send(&msg,sizeof(msg));
}

void CPartyManager::PartyInviteDeny()
{
	MSG_DWORD msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_INVITE_DENY_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData = GetRequestPartyID();
	NETWORK->Send(&msg,sizeof(msg));

	SetRequestPartyID(0);
}

BOOL CPartyManager::IsPartyMember(DWORD PlayerID)
{
	return m_Party.IsPartyMember(PlayerID);
}

PARTY_MEMBER* CPartyManager::GetPartyMemberInfo(int i)
{
	return m_Party.GetPartyMemberInfo(i);
}

BOOL CPartyManager::PartyChat(char* ChatMsg, char* SenderName)
{
	if( HERO->GetPartyIdx() == 0 )
		return FALSE;
	
	SEND_PARTY_CHAT msg;
	memset(&msg, 0, sizeof(msg));
	msg.Category = MP_CHAT;
	msg.Protocol = MP_CHAT_PARTY;
	msg.dwObjectID = HEROID;
	strcpy(msg.Name, SenderName);
	strcpy(msg.Msg, ChatMsg);
	for(int n=0; n<MAX_PARTY_LISTNUM; ++n)
	{
		if(m_Party.IsLogIn(n))
		{
			msg.MemberID[msg.MemberNum] = m_Party.GetMemberID(n);
			msg.MemberNum++;
		}
	}
	//NETWORK->Send(&msg, sizeof(msg));
	NETWORK->Send(&msg, msg.GetMsgLength()); //CHATMSG 040324

	return TRUE;
}
/*
void CPartyManager::SetClickedMemberID(DWORD id)
{
	if(m_ClickedMemberID == id)
		return;

	m_ClickedMemberID = id;
	GAMEIN->GetPartyDialog()->MemberBtnPushUp(id);
}

*/

BOOL CPartyManager::IsMasterChanging()
{
	if(m_MasterChanging == TRUE)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(412) );
		return TRUE;
	}
	return FALSE;
}

BOOL CPartyManager::CanActivate()
{
	if(HERO->GetPartyIdx() == 0)
	{
		if(m_bIsProcessing == FALSE)
		{
			GAMEIN->GetPartyCreateDialog()->SetActive(TRUE);
			return FALSE;
		}
		else
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(408) );
			return FALSE;
		}
	}
	return TRUE;
}

// desc_hseos_데이트존_01
// S 데이트존 추가 added by hseos 2007.11.13 2007.11.15
BOOL CPartyManager::IsPossibleEnterDateZone()
{
	CDateMatchingDlg* pDlg = GAMEIN->GetDateMatchingDlg() ;
	if( pDlg )
	{
		// 파티원이 데이트 매칭 대상자 2명인가?
		CParty* pParty = GetParty();
		
		int nNum = 0;
		for(int i=0; i<MAX_PARTY_LISTNUM; i++)
		{
			if (pParty->GetMemberID(i))
			{
				nNum++;
			}
		}

		if(pParty->GetMasterIdx() == HERO->GetID() && 
		   pParty->IsPartyMember((char*)pDlg->GetPartnerInfoDlg()->GetNickName()->GetStaticText()) && nNum == 2)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CPartyManager::IsPossibleEnterChallengeZone()
{
	CDateMatchingDlg* pDlg = GAMEIN->GetDateMatchingDlg() ;
	if( pDlg )
	{
		// 파티원이 데이트 매칭 대상자 2명인가?
		CParty* pParty = GetParty();
		
		int nNum = 0;
		for(int i=0; i<MAX_PARTY_LISTNUM; i++)
		{
			if (pParty->GetMemberID(i))
			{
				nNum++;
			}
		}

		if(pParty->GetMasterIdx() == HERO->GetID() && 
		   pParty->IsPartyMember((char*)pDlg->GetPartnerInfoDlg()->GetNickName()->GetStaticText()) && nNum == 2)
		{
			return TRUE;
		}
	}

	return FALSE;
}
// E 데이트존 추가 added by hseos 2007.11.13 2007.11.15

void CPartyManager::Party_Info( void* pMsg )
{
	PARTY_INFO* pmsg = (PARTY_INFO*)pMsg;
	SetPartyInfo(pmsg);
	GAMEIN->GetPartyDialog()->SetActive(TRUE);
	GAMEIN->GetPartyDialog()->RefreshDlg();

	// 070420 LYW --- PartyManager : Add pointer of member dialog to setting racial and gender.
	CPartyMemberDlg* pMemberDlg = NULL ;

	BYTE byMemberCount = 0 ;

	for(int i=0; i<MAX_PARTY_LISTNUM; i++)
	{
		if(pmsg->Member[i].bLogged && pmsg->Member[i].dwMemberID != HERO->GetID() &&
			pmsg->Member[i].LifePercent)
		{
			/*
			// 070319 LYW --- PartyManager : Check duplication member.
			for( int j=0; j<i; ++j )
			{
				if( pmsg->Member[j].dwMemberID == pmsg->Member[i].dwMemberID ) return ;
			}
			*/
			PICONMGR->CreatePartyIcon(pmsg->Member[i].dwMemberID, &pmsg->Member[i]);
		}

		// 070420 LYW --- PartyManager : Setting member face to party member dialog.
		if( pmsg->Member[i].dwMemberID != HERO->GetID() )
		{
			pMemberDlg = GAMEIN->GetPartyDialog()->GetMemberDlg(byMemberCount) ;

			if( pmsg->Member[i].dwMemberID != HERO->GetID() && pMemberDlg != NULL )
			{
				pMemberDlg->SetFace( pmsg->Member[i].Race, pmsg->Member[i].Gender ) ;
			}

			++byMemberCount ;
		}

		// 080318 LUJ, 파티원 각각의 보유 스킬도 갱신해주자
		GAMEIN->GetPartyDialog()->InitializeSkill(  pmsg->Member[ i ] );
	}
}


void CPartyManager::Party_Add_Invite( void* pMsg )
{
	MSG_NAME_DWORD2* pmsg = (MSG_NAME_DWORD2*)pMsg;
	SetRequestPartyID(pmsg->dwData1);
	GAMEIN->GetPartyInviteDialog()->SetMsg(pmsg->Name, (BYTE)pmsg->dwData2);
	GAMEIN->GetPartyInviteDialog()->SetActive(TRUE);
}


void CPartyManager::Party_Add_Ack( void* pMsg )
{
	SEND_PARTY_MEMBER_INFO* pmsg = (SEND_PARTY_MEMBER_INFO*)pMsg;
	AddPartyMemberResult(pmsg);	

	// 080613 LUJ, 파티 멤버 변경이 발생하면, 스킬 아이콘 위치를 재배치해야 한다
	{
		CPartyDialog* dialog = GAMEIN->GetPartyDialog();

		if( dialog )
		{
			dialog->RelocateSkill();
		}
	}
}


void CPartyManager::Party_Add_Nack( void* pMsg )
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	
	switch(pmsg->dwData)
	{
	case eErr_Add_NoPlayer:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(405)); break;
	case eErr_Add_AlreadyinParty:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(107)); break;
	case eErr_Add_OptionNoParty:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(108)); break;
	case eErr_Add_DiedPlayer:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(411)); break;
	}			
}


void CPartyManager::Party_Del_Ack( void* pMsg )
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	DelPartyMemberResult(pmsg);
	
	if(pmsg->dwData == HERO->GetID())			
		PICONMGR->DeleteAllPartyIcon();			
	else			
		PICONMGR->DeletePartyIcon(pmsg->dwData);

	// 080613 LUJ, 파티 멤버 변경이 발생하면, 스킬 아이콘 위치를 재배치해야 한다
	{
		CPartyDialog* dialog = GAMEIN->GetPartyDialog();

		if( dialog )
		{
			dialog->RelocateSkill();
		}
	}
}


void CPartyManager::Party_Del_Nack( void* pMsg )
{
	MSG_INT* pmsg = (MSG_INT*)pMsg;
	
	switch(pmsg->nData)
	{
	case eErr_ChangingMaster:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(412) ); break;
	}
}


void CPartyManager::Party_Create_Ack( void* pMsg )
{
	MSG_DWORDBYTE* pmsg = (MSG_DWORDBYTE*)pMsg;
	CreatePartyResult(pmsg);
}


void CPartyManager::Party_Create_Nack( void* pMsg )
{
	ASSERT(0);
	MSG_BYTE* pmsg = (MSG_BYTE* )pMsg;

	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(409), pmsg->bData);
	
	//SW060712 release Lock
	SetIsProcessing(FALSE);
}


void CPartyManager::Party_Ban_Ack( void* pMsg )
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	BanPartyMemberResult(pmsg);		
}


// 080318 LUJ, 파티원 스킬 아이콘 초기화
void CPartyManager::Party_Member_Login( void* pMsg )
{
	SEND_PARTY_MEMBER_INFO* pmsg = (SEND_PARTY_MEMBER_INFO*)pMsg;
	UserLogIn(pmsg);
	
	if(pmsg->MemberInfo.bLogged && pmsg->MemberInfo.dwMemberID != HERO->GetID())
	{
		if(pmsg->MemberInfo.LifePercent)			// 새로 접속한 파티원
			PICONMGR->CreatePartyIcon(pmsg->MemberInfo.dwMemberID, &pmsg->MemberInfo);
		else			// 기존에 접속해 있던 파티원
		{
			PICONMGR->DeletePartyIcon(pmsg->MemberInfo.dwMemberID);

			// 080318 LUJ, 파티원의 스킬 아이콘 표시를 제거한다
			{
				CPartyDialog* dialog = GAMEIN->GetPartyDialog();

				if( dialog )
				{
					dialog->RemoveAllSkill( pmsg->MemberInfo.dwMemberID );
				}
			}			
		}
	}
}


// 080318 LUJ, 파티원 스킬 아이콘 초기화
void CPartyManager::Party_Member_Logout( void* pMsg )
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	UserLogOut(pmsg);
	
	PICONMGR->DeletePartyIcon(pmsg->dwData);

	// 080318 LUJ, 해당 파티원이 가진 스킬 아이콘을 초기화한다
	{
		CPartyDialog* dialog = GAMEIN->GetPartyDialog();

		if( dialog )
		{
			dialog->RemoveAllSkill( pmsg->dwData );
		}
	}
}


void CPartyManager::Party_ChangeMaster_Ack( void* pMsg )
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	ChangeMasterPartyResult(pmsg);
	SetMasterChanging(FALSE);
}


void CPartyManager::Party_Breakup_Ack( void* pMsg )
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	BreakUpPartyResult();

	PICONMGR->DeleteAllPartyIcon();
}


void CPartyManager::Party_Breaup_Nack( void* pMsg )
{
	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(412) );
						
	//SW060712 release Lock
	SetIsProcessing(FALSE);
}


void CPartyManager::Party_MemverLife( void* pMsg )
{
	SEND_PARTY_GAUGE* pmsg = (SEND_PARTY_GAUGE*)pMsg;
	SetPartyMemberLifePercent(pmsg->PlayerID, (DWORD)pmsg->GaugePercent); //data1:playerid, data2:val
	GAMEIN->GetPartyDialog()->RefreshDlg();
}


void CPartyManager::Party_MemberMana( void* pMsg )
{
	SEND_PARTY_GAUGE* pmsg = (SEND_PARTY_GAUGE*)pMsg;
	SetPartyMemberManaPercent(pmsg->PlayerID, (DWORD)pmsg->GaugePercent);			
	GAMEIN->GetPartyDialog()->RefreshDlg();
}


void CPartyManager::Party_MemberLevel( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	SetPartyMemberLevel(pmsg->dwData1, (LEVELTYPE)pmsg->dwData2);			
	GAMEIN->GetPartyDialog()->RefreshDlg();
}


void CPartyManager::Party_SendPos( void* pMsg )
{
	SEND_PARTYICON_MOVEINFO* pmsg = (SEND_PARTYICON_MOVEINFO*)pMsg;
	if(pmsg->MoveInfo.dwMoverID != HERO->GetID())
		PICONMGR->SetMoveInfo(&pmsg->MoveInfo);
}


void CPartyManager::Party_RevivePos( void* pMsg )
{
	SEND_PARTYICON_REVIVE* pmsg = (SEND_PARTYICON_REVIVE*)pMsg;
	PICONMGR->SetCurPosition(pmsg->dwMoverID, pmsg->Pos);
}


void CPartyManager::Party_Invite_Accept_Nack( void* pMsg )
{
	MSG_BYTE * pmsg = (MSG_BYTE*)pMsg;
	
	if(pmsg->bData == eErr_BrokenParty)
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(104) );
	else if(pmsg->bData == eErr_Add_Full)
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(110) );
}


void CPartyManager::Party_Member_LoginMsg( void* pMsg ) 
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	
	if(pmsg->dwData != HEROID)
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(403), m_Party.GetMemberName(pmsg->dwData));
}


void CPartyManager::Party_Monster_Obtain_Notify( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pmsg->dwData2);
	if(pItemInfo == 0)
		return;
	CHATMGR->AddMsg( CTC_GETITEM, CHATMGR->GetChatMsg( 100 ), GetPartyMemberName(pmsg->dwData1), pItemInfo->ItemName );
}


void CPartyManager::Party_Change_PickupItem_Ack( void* pMsg )
{
	if( !pMsg ) return ;

	char tempStr[128] = {0,} ;

	MSG_BYTE* pmsg = ( MSG_BYTE* )pMsg ;

	CPartyBtnDlg* pPartySet = NULL ;
	pPartySet = (CPartyBtnDlg*)WINDOWMGR->GetWindowForID( PA_BTNDLG );

	if( !pPartySet ) return ;


	cCheckBox* pRandomBox = NULL ;
	pRandomBox = pPartySet->GetRandomCheckBox() ;
	if( pRandomBox ) pRandomBox->SetChecked(FALSE) ;

	cCheckBox* pDamageBox = NULL ;
	pDamageBox = pPartySet->GetDamageCheckBox() ;
	if( pDamageBox ) pDamageBox->SetChecked(FALSE) ;

	cCheckBox* pSequenceBox = NULL ;
	pSequenceBox = pPartySet->GetSequenceCheckBox() ;
	if( pSequenceBox ) pSequenceBox->SetChecked(FALSE) ;

	switch( pmsg->bData )
	{
	case e_OPTION_RANDOM :
		{
			sprintf( tempStr, CHATMGR->GetChatMsg(1113) ) ;

			if( pRandomBox ) pRandomBox->SetChecked(TRUE) ;

			m_PartyOption = e_OPTION_RANDOM ;
		}
		break ;

	case e_OPTION_DAMAGE :
		{
			sprintf( tempStr, CHATMGR->GetChatMsg(1115) ) ;

			if( pDamageBox ) pRandomBox->SetChecked(TRUE) ;

			m_PartyOption = e_OPTION_DAMAGE ;
		}
		break ;

	case e_OPTION_SEQUENCE :
		{
			sprintf( tempStr, CHATMGR->GetChatMsg(1114) ) ;

			if( pSequenceBox ) pRandomBox->SetChecked(TRUE) ;

			m_PartyOption = e_OPTION_SEQUENCE ;
		}
		break ;
	}

	pPartySet->SetCheckBox(pmsg->bData) ;

	CHATMGR->AddMsg( CTC_PARTYCHAT, tempStr );
}

// 071002 LYW --- PartyManager : Add function to parsing message to nack change option.
void CPartyManager::Party_Change_PickupItem_Nack( void* pMsg )
{
	//CHATMGR->AddMsg( CTC_PARTYCHAT, "파티 아이템 분배 변경에 실패하였습니다." );
	CHATMGR->AddMsg( CTC_PARTYCHAT, CHATMGR->GetChatMsg( 1297 ) );

	CPartyBtnDlg* pPartySet = NULL ;
	pPartySet = (CPartyBtnDlg*)WINDOWMGR->GetWindowForID( PA_BTNDLG );

	if( !pPartySet ) return ;

	cCheckBox* pRandomBox = NULL ;
	pRandomBox = pPartySet->GetRandomCheckBox() ;
	if( pRandomBox ) pRandomBox->SetChecked(FALSE) ;

	cCheckBox* pDamageBox = NULL ;
	pDamageBox = pPartySet->GetDamageCheckBox() ;
	if( pDamageBox ) pDamageBox->SetChecked(FALSE) ;

	cCheckBox* pSequenceBox = NULL ;
	pSequenceBox = pPartySet->GetSequenceCheckBox() ;
	if( pSequenceBox ) pSequenceBox->SetChecked(FALSE) ;

	switch( m_PartyOption )
	{
	case e_OPTION_RANDOM :
		{
			if( pRandomBox ) pRandomBox->SetChecked(TRUE) ;
		}
		break ;

	case e_OPTION_DAMAGE :
		{
			if( pDamageBox ) pRandomBox->SetChecked(TRUE) ;
		}
		break ;

	case e_OPTION_SEQUENCE :
		{
			if( pSequenceBox ) pRandomBox->SetChecked(TRUE) ;
		}
		break ;
	}
}

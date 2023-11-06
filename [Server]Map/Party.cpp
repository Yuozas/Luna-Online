// Party.cpp: implementation of the CParty class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Party.h"
#include "UserTable.h"
#include "Network.h"
#include "Distribute_Random.h"
#include "Distribute_Damage.h"
// 070314 LYW --- Party : Include header file of distribute rotation.
#include "Distribute_Rotation.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CParty::CParty()
{
	// 070316 LYW --- Party : ���� �й�� ���.
	m_ItemWinner = 0 ;

	m_pCandidate = NULL ;
	m_pItemWinner = NULL ;

	// ��÷�� ���̵� �ʱ�ȭ
	m_dwWinnerID = 0 ;

	// ��� ���̵� �ʱ�ȭ
	for( BYTE count = 0 ; count < MAX_PARTY_LISTNUM ; ++count )
	{
		m_dwMemberID[ count ] = 0 ;
	}

	// 070507 LYW --- Party : Add check count.
	m_byWinnerIdx = 0 ;
	m_bDistributed = FALSE ;
}

CParty::~CParty()
{

}

void CParty::Init(DWORD id,DWORD MasterID,char* strMasterName,CPlayer* pMaster,BYTE Option)
{
	m_PartyIDx = id;
	// 070420 LYW --- Modified call function SetMember.
	//m_Member[0].SetMember(MasterID,strMasterName,NULL, 0);
	CHARACTER_TOTALINFO totalInfo ;
	memset(&totalInfo, 0, sizeof(CHARACTER_TOTALINFO)) ;
	if(pMaster)
	{
		pMaster->GetCharacterTotalInfo(&totalInfo) ;
	}
	m_Member[0].SetMember(MasterID,strMasterName,NULL, 0, totalInfo.Race, totalInfo.Gender);
	m_MasterChanging = FALSE;
	m_Option = Option;
	m_OldSendtime = 0;
	
	//������ �й� �ɼ�
	switch(m_Option)
	{
	case ePartyOpt_Random: //����
		m_pDistributeWay = DISTRIBUTERAND; break;
	case ePartyOpt_Damage: //Ÿ��ġ
		m_pDistributeWay = DISTRIBUTEDAM; break;
	case ePartyOpt_Sequence: //�������
		{
			// 070314 LYW --- Party : Modified sequence part.
			/*
			m_pDistributeWay = DISTRIBUTERAND;
			ASSERT(0);
			*/

			m_pDistributeWay = DISTRIBUTEROTATION ;

			// 070507 LYW --- Party : Setting winner index.
			m_byWinnerIdx = MAX_PARTY_LISTNUM - 1 ;
		}
		break;
	default:
		{
			m_pDistributeWay = DISTRIBUTERAND;
			ASSERT(0);
		}		
		break;
	}
}



void CParty::SetMaster(CPlayer* pMaster)
{
	// 070420 LYW --- Modified function SetMaster.
	//m_Member[0].SetMember(pMaster->GetID(), pMaster->GetObjectName(), pMaster, pMaster->GetLevel());
	CHARACTER_TOTALINFO totalInfo ;
	memset(&totalInfo, 0, sizeof(CHARACTER_TOTALINFO)) ;
	if(pMaster)
	{
		pMaster->GetCharacterTotalInfo(&totalInfo) ;
	}
	m_Member[0].SetMember(pMaster->GetID(), pMaster->GetObjectName(), pMaster, pMaster->GetLevel(), totalInfo.Race, totalInfo.Gender);
}

void CParty::SendPartyInfo(CPlayer* pToPlayer)
{
	PARTY_INFO msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_INFO;
	msg.dwObjectID = pToPlayer->GetID();
	msg.PartyDBIdx = m_PartyIDx;
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		GetMemberInfo(n,&msg.Member[n]);

		CPlayer* pMember = (CPlayer*)g_pUserTable->FindUser(msg.Member[n].dwMemberID);
		if(pMember)
		if(pToPlayer->GetGridID() != pMember->GetGridID())
		{
			msg.Member[n].LifePercent = 0;
			msg.Member[n].ManaPercent = 0;
		}
	}
	msg.Option = GetOption();
	pToPlayer->SendMsg(&msg,sizeof(msg));
}

// 070420 LYW --- Party : Modified function Add partyMember.
//BOOL CParty::AddPartyMember(DWORD AddMemberID,char* Name,CPlayer* pPlayer, LEVELTYPE lvl)
BOOL CParty::AddPartyMember(DWORD AddMemberID,char* Name,CPlayer* pPlayer, LEVELTYPE lvl, BYTE racial, BYTE gender)
{
	//��Ƽ�� ������ �ȵǾ� �ִٸ�
	if(m_Member[0].MemberID == 0)
		ASSERTMSG(0, "��Ƽ ������ ���µ� AddPartyMember��");

	// 070927 LYW --- Party : ���� �ι��� �߰� �ߺ��� ���� ���� ��� ���̵� ���� üũ�Ѵ�.

	BOOL bSameID = FALSE ;
	for(int count = 0 ; count < MAX_PARTY_LISTNUM ; ++count )
	{
		if( m_Member[count].MemberID == 0 ) continue ;

		if( m_Member[count].MemberID == AddMemberID ) 
		{
			bSameID = TRUE ;
			break ;
		}
	}

    if( bSameID ) return FALSE ;

	for(int n=1;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].MemberID == 0)
		{
			// 070420 LYW --- Party : Modified setting member.
			//m_Member[n].SetMember(AddMemberID,Name,pPlayer,lvl);
			m_Member[n].SetMember(AddMemberID,Name,pPlayer,lvl, racial, gender);
			if(pPlayer)
			{				
				SendPartyInfo(pPlayer);
			}
			return TRUE;
		}
	}
	ASSERTMSG(0,"��Ƽ�� �� ã�µ� Add�Ϸ� �߽��ϴ�");

	return FALSE;
}

BOOL CParty::RemovePartyMember(DWORD MemberID) 
{
	// 070314 LYW --- Party : Modified function RemovePartyMember(...).
	/*
	for(int n=1;n<MAX_PARTY_LISTNUM;++n) 
	{
		if(m_Member[n].MemberID == MemberID) 
		{			
			m_Member[n].MemberID = 0;
			m_Member[n].Name[0] = 0;
			m_Member[n].bLogged = FALSE;
			m_Member[n].LifePercent = 0;
			m_Member[n].ManaPercent = 0;
			m_Member[n].Level = 0;

			return TRUE;
		}
	}
	*/

	BOOL bSameID = FALSE ;

	for(int n=1;n<MAX_PARTY_LISTNUM;++n) 
	{
		if(m_Member[n].MemberID == MemberID) 
		{			
			bSameID = TRUE ;
		}

		if( bSameID )
		{
			if( n+1 < MAX_PARTY_LISTNUM )
			{
				m_Member[n].MemberID	= m_Member[n+1].MemberID ;	
				memset(m_Member[n].Name, 0, strlen(m_Member[n].Name) ) ;
				memcpy(m_Member[n].Name, m_Member[n+1].Name, strlen(m_Member[n+1].Name) ) ;
				m_Member[n].bLogged		= m_Member[n+1].bLogged ;		
				m_Member[n].LifePercent = m_Member[n+1].LifePercent ;
				m_Member[n].ManaPercent = m_Member[n+1].ManaPercent ;
				m_Member[n].Level		= m_Member[n+1].Level ;	
			}
			else
			{
				m_Member[n].MemberID	= 0;
				m_Member[n].Name[0]		= 0;
				m_Member[n].bLogged		= FALSE;
				m_Member[n].LifePercent = 0;
				m_Member[n].ManaPercent = 0;
				m_Member[n].Level		= 0;
			}
		}
	}

	return FALSE;
} 

BOOL CParty::ChangeMaster(DWORD FromID,DWORD ToID)
{
	ASSERTMSG(m_Member[0].MemberID != ToID,"�ڱ��ڽſ��� ������ �̾��Ϸ� �߽��ϴ�.");
	ASSERTMSG(m_Member[0].MemberID == FromID,"������ �ƴ� ����� �̾��Ϸ� �߽��ϴ�.");


	for(int n=1; n<MAX_PARTY_LISTNUM; ++n) 
	{
		if(m_Member[n].MemberID == ToID) 
		{
			swap(m_Member[0], m_Member[n]);
			return TRUE;
		} //if
	} // for
	
	return FALSE;
} 

BOOL CParty::IsPartyMember(DWORD PlayerID) {
	for(int n=0; n<MAX_PARTY_LISTNUM; ++n) 
		if(m_Member[n].MemberID == PlayerID) return TRUE;
	return FALSE;	
	
}

BOOL CParty::IsMemberLogIn(int n)
{
	return m_Member[n].bLogged;
}

void CParty::UserLogIn(CPlayer* pPlayer,BOOL bNotifyUserLogin) 
{	
	int n = SetMemberInfo(pPlayer->GetID(), pPlayer->GetObjectName(), pPlayer, pPlayer->GetLevel(), TRUE);
	if(n == -1)
	{
		ASSERT(0); 
		return;
	}
	// ���� ��Ƽ�� ������ �α����� �������� �����ش�.
	SendPartyInfo(pPlayer);

	if(bNotifyUserLogin)
	{
		// �α����� ������ ������ ���� ��Ƽ������ �����ش�.
		SendPlayerInfoToOtherMembers(n, pPlayer->GetGridID());
		
		SEND_CHANGE_PARTY_MEMBER_INFO cmsg;
		cmsg.Category = MP_PARTY;
		cmsg.Protocol = MP_PARTY_NOTIFYMEMBER_LOGIN_TO_MAPSERVER;
		cmsg.dwObjectID = pPlayer->GetID();
		cmsg.PartyID = pPlayer->GetPartyIdx();
		cmsg.Level = pPlayer->GetLevel();
		SafeStrCpy(cmsg.Name,this->GetMemberName(pPlayer->GetID()), MAX_NAME_LENGTH+1);
		
		g_Network.Send2AgentServer((char*)&cmsg,sizeof(cmsg));
	}
}

void CParty::NotifyUserLogIn(SEND_CHANGE_PARTY_MEMBER_INFO* pmsg)
{
	int n = SetMemberInfo(pmsg->dwObjectID, pmsg->Name, NULL, pmsg->Level, TRUE);
	SendPlayerInfoToOtherMembers(n, 0);
}

void CParty::UserLogOut(DWORD PlayerID) 
{
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].MemberID == PlayerID)
		{
			m_Member[n].bLogged = FALSE;
			m_Member[n].LifePercent = 0;
			m_Member[n].ManaPercent = 0;
		}
	}
}

void CParty::SendMsgUserLogOut(DWORD PlayerID)
{
	// �α׾ƿ��� ������ ������ �ٸ� ��Ƽ������ �����ش�.
	MSG_DWORD msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_MEMBER_LOGOUT;
	msg.dwData = PlayerID;
	SendMsgToAll(&msg,sizeof(msg));
}

void CParty::SendMsgToAll(MSGBASE* msg, int size) 
{
	CPlayer* pPlayer;
	for(int i=0; i<MAX_PARTY_LISTNUM; ++i) 
	{
		if(m_Member[i].bLogged == TRUE ) 
		{
			msg->dwObjectID = m_Member[i].MemberID;
			pPlayer = (CPlayer*)g_pUserTable->FindUser(m_Member[i].MemberID);
			if(pPlayer == NULL)
				continue;

			pPlayer->SendMsg(msg, size);
		}
	}
} 

// 071002 LYW --- Party : Add function to send message to member.
void CParty::SendMsgToMember(MSG_DWORD2* pMsg)
{
	if( !pMsg ) return ;

	MSG_BYTE msg ;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_CHANGE_PICKUPITEM_ACK;
	msg.bData	 = (BYTE)pMsg->dwData2 ;

	CPlayer* pPlayer ;
	for(int i=0; i<MAX_PARTY_LISTNUM; ++i) 
	{
		if(m_Member[i].bLogged == TRUE ) 
		{
			pPlayer = (CPlayer*)g_pUserTable->FindUser(m_Member[i].MemberID);
			if(pPlayer == NULL)
				continue;

			pPlayer->SendMsg(&msg, sizeof(MSG_BYTE));
		}
	}
}

void CParty::SendMsgExceptOne(MSGBASE* msg, int size, DWORD PlayerID) 
{
	CPlayer* pPlayer;
	for(int i=0; i<MAX_PARTY_LISTNUM; ++i) 
	{
		if(m_Member[i].MemberID == PlayerID )
			continue;
		else if(m_Member[i].bLogged == TRUE ) 
		{
			msg->dwObjectID = m_Member[i].MemberID;
			pPlayer = (CPlayer*)g_pUserTable->FindUser(m_Member[i].MemberID);
			if(pPlayer == NULL)
				continue;
			
			pPlayer->SendMsg(msg, size);
		}
	}
}

void CParty::SendMsgExceptOneinChannel(MSGBASE* msg, int size, DWORD PlayerID, DWORD GridID)
{
	CPlayer* pPlayer;
	for(int i=0; i<MAX_PARTY_LISTNUM; ++i) 
	{
		if(m_Member[i].MemberID == PlayerID )
			continue;
		else if(m_Member[i].bLogged == TRUE ) 
		{
			msg->dwObjectID = m_Member[i].MemberID;
			pPlayer = (CPlayer*)g_pUserTable->FindUser(m_Member[i].MemberID);
			if(pPlayer == NULL)
				continue;
			if(pPlayer->GetGridID() != GridID)
				continue;
			pPlayer->SendMsg(msg, size);
		}
	}
}

void CParty::BreakUp()
{
	CPlayer* pPlayer;
	for(int i=0; i<MAX_PARTY_LISTNUM; ++i) 
	{
		if(m_Member[i].MemberID != NULL ) 
		{
			pPlayer = (CPlayer*)g_pUserTable->FindUser(m_Member[i].MemberID);
			if(pPlayer == NULL)
				continue;
			
			pPlayer->SetPartyIdx(0);
		}
	}

	// 070315 LYW --- Party : Setting to 0 member array.
	for( int j=0; j<MAX_PARTY_LISTNUM; ++j )
	{
		m_Member[j].MemberID	= 0;
		m_Member[j].Name[0]		= 0;
		m_Member[j].bLogged		= FALSE;
		m_Member[j].LifePercent = 0;
		m_Member[j].ManaPercent = 0;
		m_Member[j].Level		= 0;
	}
}

void CParty::GetMemberInfo(int n, PARTY_MEMBER* pRtInfo)
{	
	pRtInfo->dwMemberID = m_Member[n].MemberID;
	SafeStrCpy(pRtInfo->Name,m_Member[n].Name, MAX_NAME_LENGTH+1);
	pRtInfo->bLogged = m_Member[n].bLogged;
	pRtInfo->Level = m_Member[n].Level;

	// 070423 LYW --- Party : Setting racial and gender.
	pRtInfo->Gender = m_Member[n].gender ;
	pRtInfo->Race = m_Member[n].racial ;

	if(m_Member[n].MemberID != NULL && m_Member[n].bLogged == TRUE)
	{
		pRtInfo->LifePercent = m_Member[n].LifePercent;
		pRtInfo->ManaPercent = m_Member[n].ManaPercent;
		
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->GetData(m_Member[n].MemberID);
		if(pPlayer)
		{
			VECTOR3 vPos;
			pPlayer->GetPosition(&vPos);
			pRtInfo->posX = (WORD)vPos.x;
			pRtInfo->posZ = (WORD)vPos.z;
		}
	}
	else
	{
		pRtInfo->LifePercent = 0;
		pRtInfo->ManaPercent = 0;
		pRtInfo->posX = 0;
		pRtInfo->posZ = 0;
	}
}

DWORD CParty::GetFirstOnlineMemberID()
{
	for(int i=0; i<MAX_PARTY_LISTNUM; ++i) 
	{
		if( m_Member[i].bLogged == TRUE ) 
		{
			return m_Member[i].MemberID;
		}
	}
	return 0;
}

char* CParty::GetMemberName(DWORD MemberID)
{
	for(int i=0; i<MAX_PARTY_LISTNUM; ++i) 
	{
		if(m_Member[i].MemberID == MemberID ) 
		{
			return m_Member[i].Name;
		} //if
	} //for
//	ASSERTMSG(0,"�ɹ��� �ƴ� ����� ���̵� ã���� �߽��ϴ�.");
	return "";
}

void CParty::SendMemberLogInMsg(DWORD PlayerID)
{
	MSG_DWORD msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_MEMBER_LOGINMSG;
	msg.dwData = PlayerID;
	SendMsgToAll(&msg, sizeof(msg)); 
}

void CParty::NotifyMemberLogin(DWORD PlayerID)
{
	MSG_DWORD bmsg;
	bmsg.Category = MP_PARTY;
	bmsg.Protocol = MP_PARTY_NOTIFYMEMBER_LOGINMSG;
	bmsg.dwObjectID = PlayerID;
	bmsg.dwData = GetPartyIdx();
	g_Network.Send2AgentServer((char*)&bmsg, sizeof(bmsg));
}

// 071002 LYW --- Party : Add function to notice to members that changed option of party.
void CParty::NotifyChangedOption(DWORD dwOption)
{
	MSG_DWORD2 msg ;
	msg.Category = MP_PARTY ;
	msg.Protocol = MP_PARTY_NOTIFY_CHANGE_OPTION ;
	msg.dwObjectID = GetMasterID() ;
	msg.dwData1 = GetPartyIdx() ;
	msg.dwData2 = dwOption ;
	g_Network.Send2AgentServer((char*)&msg, sizeof(msg));
}

int CParty::SetMemberInfo(DWORD MemberID, char* strName, CPlayer* pPlayer, LEVELTYPE lvl, BOOL bLog)
{
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].MemberID == MemberID)
		{
			// 070420 LYW --- Modified function SetMemberInfo.
			//m_Member[n].SetMember(MemberID, strName, pPlayer, lvl);
			CHARACTER_TOTALINFO totalInfo ;
			memset(&totalInfo, 0, sizeof(CHARACTER_TOTALINFO)) ;
			if(pPlayer)
			{
				pPlayer->GetCharacterTotalInfo(&totalInfo) ;
			}

			m_Member[n].SetMember(MemberID, strName, pPlayer, lvl, totalInfo.Race, totalInfo.Gender);
			m_Member[n].bLogged = bLog;
			return n;
		}
	}
	ASSERT( n < MAX_PARTY_LISTNUM );
	return -1;
}

void CParty::SendPlayerInfoToOtherMembers(int n, DWORD GridID)
{
	SEND_PARTY_MEMBER_INFO msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_MEMBER_LOGIN;
	GetMemberInfo(n,&msg.MemberInfo);
	msg.PartyID = GetPartyIdx();
	SendMsgLoginMemberInfo(&msg, GridID);
}

void CParty::SendMsgLoginMemberInfo(SEND_PARTY_MEMBER_INFO* msg, DWORD GridID)
{
	CPlayer* pPlayer;
	for(int i=0; i<MAX_PARTY_LISTNUM; ++i) 
	{
		if(m_Member[i].bLogged == TRUE ) 
		{
			msg->dwObjectID = m_Member[i].MemberID;
			pPlayer = (CPlayer*)g_pUserTable->FindUser(m_Member[i].MemberID);
			if(pPlayer == NULL)
				continue;
			if(pPlayer->GetGridID() != GridID)
			{
				msg->MemberInfo.LifePercent = 0;
				msg->MemberInfo.ManaPercent = 0;
			}
			pPlayer->SendMsg(msg, sizeof(SEND_PARTY_MEMBER_INFO));
		}
	}
}

void CParty::SetMasterChanging(BOOL val)
{
	m_MasterChanging = val;
}

BOOL CParty::IsMasterChanging(CPlayer* pPlayer, BYTE Protocol)
{
	if(m_MasterChanging == TRUE)
	{
		MSG_INT msg;
		msg.Category = MP_PARTY;
		msg.Protocol = Protocol;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return TRUE;
	}
	return FALSE;
}

void CParty::SetMemberLevel(DWORD PlayerID, LEVELTYPE lvl)
{
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		if(m_Member[n].MemberID == PlayerID)
		{
			m_Member[n].SetLevel(lvl);
			return;
		}
	}
	char buff[256] = {0, };
	sprintf(buff, "PartyID : %d, PlayerID : %d, level : %d", m_PartyIDx, PlayerID, lvl);
	ASSERTMSG(0, buff);
}

void CParty::SendMemberPos(DWORD MoverID, BASEMOVE_INFO* pMoveInfo)
{
	CPlayer* pPlayer = NULL;

	SEND_PARTYICON_MOVEINFO msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_SENDPOS;
	msg.MoveInfo.dwMoverID = MoverID;
	msg.MoveInfo.tgCount = pMoveInfo->GetMaxTargetPosIdx();
	msg.MoveInfo.cPos.posX = (WORD)pMoveInfo->CurPosition.x;
	msg.MoveInfo.cPos.posZ = (WORD)pMoveInfo->CurPosition.z;
	msg.MoveInfo.KyungGongIdx = pMoveInfo->KyungGongIdx;
	msg.MoveInfo.MoveState = pMoveInfo->MoveMode;
	
	for(int i=0; i<msg.MoveInfo.tgCount; i++)
	{
		VECTOR3* pTgPos = pMoveInfo->GetTargetPosition(i);
		msg.MoveInfo.tgPos[i].posX = (WORD)pTgPos->x;
		msg.MoveInfo.tgPos[i].posZ = (WORD)pTgPos->z;
	}
	
	for(i=0; i<MAX_PARTY_LISTNUM; i++)
	{
		if(m_Member[i].bLogged && m_Member[i].MemberID != MoverID)
//		if(m_Member[i].bLogged)
		{
			pPlayer = (CPlayer*)g_pUserTable->FindUser(m_Member[i].MemberID);
			if(!pPlayer)			continue;

			pPlayer->SendMsg(&msg, msg.GetSize());
		}
	}
}

void CParty::SetOption(BYTE Option)
{
	m_Option = Option;

	// 070315 LYW --- Party : Setting option to party.
	switch(m_Option)
	{
	case ePartyOpt_Random: //����
		m_pDistributeWay = DISTRIBUTERAND; break;
	case ePartyOpt_Damage: //Ÿ��ġ
		m_pDistributeWay = DISTRIBUTEDAM; break;
	case ePartyOpt_Sequence: //�������
		{
			// 070314 LYW --- Party : Modified sequence part.
			/*
			m_pDistributeWay = DISTRIBUTERAND;
			ASSERT(0);
			*/

			m_pDistributeWay = DISTRIBUTEROTATION ;
		}
		break;
	default:
		{
			m_pDistributeWay = DISTRIBUTERAND;
			ASSERT(0);
		}		
		break;
	}
}

BYTE CParty::GetOption()
{
	return m_Option;
}
/*�����Ƽ ���� - 
void CParty::SendAbil( LEVELTYPE MonsterLevel, PARTY_RECEIVE_MEMBER* pMemberInfo, LEVELTYPE MaxLevel )
{
	m_pDistributeWay->CalcAbilandSend(MonsterLevel, pMemberInfo, MaxLevel);
}
*/
void CParty::SendItem( PARTY_RECEIVE_MEMBER* pRealMember, DWORD DropItemId, DWORD DropItemRatio, MONSTEREX_LIST * pMonInfo, WORD MonsterKind, LEVELTYPE MaxLevel )
{
	//// 070316 LYW --- Party : 
	//if( GetOption() == ePartyOpt_Sequence )
	//{
	//	m_pCandidate = pRealMember ;
	//}
	//m_pDistributeWay->SendItem(pRealMember, DropItemId, DropItemRatio, pMonInfo, MonsterKind, MaxLevel);
}


// 070316 LYW --- Party : ���� �й�� ���.
CPlayer* CParty::GetItemWinner()
{
	ASSERT( (m_ItemWinner < m_pCandidate->count) ) ;
	m_pItemWinner = (CPlayer*)m_pCandidate->pPlayer[m_ItemWinner] ;

	m_dwWinnerID = m_pItemWinner->GetID() ;

	return m_pItemWinner ;
}

// ��� ���̵� ���� �Լ�
void CParty::InitMemberID()
{
	BOOL bStart = FALSE ;

	BYTE byCount = 0 ;

	// ���� ������ ���� �� ������ �ʱ�ȭ.
	for( BYTE initCount = 0 ; initCount < MAX_PARTY_LISTNUM ; ++initCount )
	{
		m_dwMemberID[ initCount ] = 0 ;
	}

	// ��Ƽ���� id�� �˻��Ѵ�.
	for( BYTE count = 0 ; count < MAX_PARTY_LISTNUM ; ++count )
	{
		if( m_Member[ count ].MemberID == m_dwWinnerID )
		{
			bStart = TRUE ;
		}

		if(bStart)
		{
			m_dwMemberID[ byCount ] = m_Member[ count ].MemberID ;

			++byCount ;
		}
	}
}

// ��÷�� ���� �Լ�
void CParty::SetWinner()
{
	CPlayer* pPlayer = NULL ;

	BOOL bWinner = FALSE ;

	pPlayer = (CPlayer*)m_pCandidate->pPlayer[0] ;

	// ���� ��÷�ڰ� ���ٸ�.
	if(m_dwWinnerID == 0)
	{
		m_dwWinnerID = pPlayer->GetID() ;

		m_ItemWinner = 0 ;
	}
	// ���� ��÷�ڰ� �ִٸ�.
	else	
	{
		for( BYTE count = 0 ; count < m_pCandidate->count ; ++count )
		{
			pPlayer = (CPlayer*)m_pCandidate->pPlayer[ count ] ;

			// �ĺ�����Ʈ�� ���� ��÷�ڰ� �ִ��� �˻�.
			if( pPlayer->GetID() == m_dwWinnerID )
			{
				bWinner = TRUE ;

				break ;
			}
		}

		// �ĺ�����Ʈ�� ���� ��÷�ڰ� �ִٸ�.
		if(bWinner)
		{
			// ���� ��÷�ڰ� ������ �ĺ���.
			if( count+1 >= m_pCandidate->count )
			{
				m_ItemWinner = 0 ;
			}
			// ���� �ĺ��� ��÷�ڷ� ����.
			else m_ItemWinner = count+1 ;
		}
		// �ĺ�����Ʈ�� ���� ��÷�ڰ� ���ٸ�.
		else
		{
			// ���� ��÷�� ������ ��� ����Ʈ Ȯ��.
			InitMemberID() ;

			// �ĺ��� ���� ��� ����Ʈ ��.
			for( BYTE count = 0 ; count < m_pCandidate->count ; ++count )
			{
				pPlayer = (CPlayer*)m_pCandidate->pPlayer[ count ] ;

				if( FindWinner(pPlayer->GetID()) ) m_ItemWinner = count ;
			}
		}
	}

	
}

// ��÷�� ã��
BOOL CParty::FindWinner(DWORD id) 
{
	for( BYTE count = 0 ; count < MAX_PARTY_LISTNUM ; ++count )
	{
		if( m_Member[count].MemberID == id ) return TRUE ;
	}

	return FALSE ;
}

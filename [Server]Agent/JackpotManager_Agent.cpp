// JackpotManager_Agent.cpp: implementation of the JackpotManager_Agent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JackpotManager_Agent.h"
#include "ServerSystem.h"
#include "Network.h"
#include "MHTimeManager.h"
#include "AgentDBMsgParser.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLOBALTON(JackpotManager_Agent)

JackpotManager_Agent::JackpotManager_Agent()
{
	Init();
}

JackpotManager_Agent::~JackpotManager_Agent()
{

}

void	JackpotManager_Agent::CheckManager()	//���̰����ϴ� Agent����
{
	//g_pServerSystem->GetServerNum()
	if( g_pServerSystem->GetServerNum() == 0 )
		m_bManager = TRUE;
	else m_bManager = FALSE;

}
void	JackpotManager_Agent::GetTotalMoneyFrDB()	//DB�� ����...�����ɶ�..�׸��� ����������
{
	if( gCurTime - m_dwLastDBUpdateTime < m_dwUpdateTimeLength ) return;
	
	//DB.Query
	JackpotLoadTotalMoney();

	m_dwLastDBUpdateTime = gCurTime;
}

void	JackpotManager_Agent::SendMsgUserTotalMoney()	//�ѱݾ� ������ �������� ������. //!�� ���� �������� Ȯ��
{
	MSG_DWORD msg;
	//SetProtocol((MSGBASE*)&msg, MP_JACKPOT, MP_JACKPOT_TOTALMONEY_NOTIFY_TO_AGENT);	//�ٸ� AGENT�� ������
	msg.Category = MP_JACKPOT;
	msg.Protocol = MP_JACKPOT_TOTALMONEY_NOTIFY_TO_AGENT;
	msg.dwData = m_dwTotalMoney;

	g_Network.Broadcast2AgentServerExceptSelf((char*)&msg, sizeof(msg));

	//SetProtocol((MSGBASE*)&msg, MP_JACKPOT, MP_JACKPOT_TOTALMONEY_NOTIFY);	//this AGENT�� USERS ���� ������.
	msg.Category = MP_JACKPOT;
	msg.Protocol = MP_JACKPOT_TOTALMONEY_NOTIFY;

	USERINFO* info = NULL;	
	g_pUserTable->SetPositionUserHead();
	while( info = g_pUserTable->GetUserData() )	//!g_pUserTable ���� �� CPlayer object ���ΰ�?
	{
		if( info->wUserMapNum )
		{
			MSG_DWORD msgTemp = msg;
			g_Network.Send2User( info->dwConnectionIndex, (char*)&msgTemp, sizeof(msgTemp) );
		}
	}

	//MP_JACKPOT_TOTALMONEY_NOTIFY_TO_AGENT
}

void	JackpotManager_Agent::SendMsgAddUserTotalMoney( DWORD CharacterID )
{
	MSG_DWORD msg;

	msg.Category = MP_JACKPOT;
	msg.Protocol = MP_JACKPOT_TOTALMONEY_NOTIFY;
	msg.dwObjectID = CharacterID;
	msg.dwData = m_dwTotalMoney;
	
	g_Network.Send2User((MSGBASE*)&msg, sizeof(msg) );	
}

void	JackpotManager_Agent::Start()
{
	CheckManager();
}

void	JackpotManager_Agent::Process()
{
	if(m_bManager)
	GetTotalMoneyFrDB();
}

void	JackpotManager_Agent::Init()
{
	// 071115 ����, ���� ��� ����

	//m_dwTotalMoney			= 0;
	//m_dwUpdateTimeLength	= DB_UPDATE_TIMELENGTH;
	//m_dwLastDBUpdateTime	= 0;
	//m_bManager				= FALSE;

	////!DB.Query �ѱݾ� �޾ƿ���
	//JackpotLoadTotalMoney();

//	//������ �������� Ȯ���ϱ�
//	CheckManager();
}

void	JackpotManager_Agent::Release()
{

}

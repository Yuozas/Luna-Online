// ServerTable.cpp: implementation of the CServerTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerTable.h"
#include "ServerSystem.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CServerTable * g_pServerTable = NULL;
/*

char * SERVER_KIND_STR[] =
{
		"DISTRIBUTE_SERVER",
		"AGENT_SERVER",
		"MAP_SERVER",
		"CHAT_SERVER",
		"MONITOR_AGENT_SERVER",
		"MONITOR_SERVER"
};
*/

CServerTable::CServerTable()
{
	//m_dwMaxBucketNum = 0;
	m_MaxServerConnectionIndex = 0;
	m_pSelfServerInfo = NULL;
	m_pMSServerInfo = NULL;

	ZeroMemory( m_nDisableAgent, sizeof(m_nDisableAgent) );
	
/*
#ifdef __AGENTSERVER__
	memset(m_MapIDMapNum, 0, sizeof(WORD)*MAX_MAP_NUM);
#endif //__AGENTSERVER__*/

}

CServerTable::~CServerTable()
{
	if( m_pSelfServerInfo)
	{
		// CBootManager::GetServerInfoByNum() ���� ������ ����INFO�� �����ش�. 2007/05/10 - ������.
		// �̰� ��~��� �ӽö����� ��, �Ҵ��� ���� �����ϴ� ��Ģ�� �����.
		// ���� �ڵ带 DistributeServer�� AgentServer�� ���纻���� �����Ѵ�.
		delete	m_pSelfServerInfo;
		m_pSelfServerInfo	=	0;
	}
	Release();
}

void CServerTable::Release()
{
	RemoveAllServer();
}

void CServerTable::Init(DWORD dwBucketNum)
{
	CYHHashTable<SERVERINFO>::Initialize(dwBucketNum);
}

SERVERINFO * CServerTable::FindServer(WORD Port)
{
	return CYHHashTable<SERVERINFO>::GetData(Port);
}
SERVERINFO* CServerTable::GetNextServer()
{
	SERVERINFO* pRt = CYHHashTable<SERVERINFO>::GetData();
	if(pRt == NULL)
		return NULL;

	if(pRt->dwConnectionIndex == 0)
		return GetNextServer();
	else
		return pRt;
}
SERVERINFO* CServerTable::GetNextDistServer()
{
	SERVERINFO* pRt = GetNextServer();
	if(pRt == NULL)
		return NULL;

	if(pRt->wServerKind != DISTRIBUTE_SERVER)
		return GetNextDistServer();
	else
		return pRt;
}

SERVERINFO* CServerTable::GetNextMapServer()
{
	SERVERINFO* pRt = GetNextServer();
	if(pRt == NULL)
		return NULL;

	if(pRt->wServerKind != MAP_SERVER)
		return GetNextMapServer();
	else
		return pRt;
}
SERVERINFO* CServerTable::GetNextAgentServer()
{
	SERVERINFO* pRt = GetNextServer();
	if(pRt == NULL)
		return NULL;

	if(pRt->wServerKind != AGENT_SERVER)
		return GetNextAgentServer();
	else
		return pRt;
}


void CServerTable::AddServer(SERVERINFO * info, WORD Port)
{
	if( info == (SERVERINFO *)GetData(Port) )
	{
		return;
	}

	CYHHashTable<SERVERINFO>::Add(info, Port);
}
void CServerTable::AddSelfServer(SERVERINFO * info)
{
	//CYHHashTable<SERVERINFO>::Add(info, key);
	ASSERT(!m_pSelfServerInfo);
	m_pSelfServerInfo = info;
}
void CServerTable::AddMSServer(SERVERINFO * info)
{
	ASSERT(!m_pMSServerInfo);
	m_pMSServerInfo = info;
}

SERVERINFO* CServerTable::FindServerForConnectionIndex(DWORD dwConnectionIndex)
{
	SetPositionHead();
	SERVERINFO* info = NULL;
	while(info = (SERVERINFO*)GetData())
	{
		if(info->dwConnectionIndex == dwConnectionIndex)
		{
			return info;
		}
	}
	return NULL;
}

SERVERINFO * CServerTable::RemoveServer(DWORD dwConnectionIndex)
{
	SetPositionHead();
	SERVERINFO * info = NULL;
	while(info = (SERVERINFO *)GetData())
	{
		if(info->dwConnectionIndex == dwConnectionIndex)
		{
			Remove(info->wPortForServer);
			return info;
		}
	}
	return NULL;
}

SERVERINFO * CServerTable::RemoveServer(WORD wKey)
{
	SERVERINFO * info = (SERVERINFO *)GetData(wKey);
	if( info->wServerKind == AGENT_SERVER &&
		info->wServerNum >= 100 )
		return NULL;
	Remove(wKey);
	return info;
}
// HASH�� �����ִ� SERVERINFO(���޸�)���� �� ����
void CServerTable::RemoveAllServer()
{
	SetPositionHead();
	SERVERINFO * info = NULL;
	while(info = (SERVERINFO *)GetData())
	{
//		Remove(info->wPortForServer);
		delete info;
		info = NULL;
	}
	RemoveAll();
}

WORD CServerTable::GetServerPort(WORD ServerKind, WORD ServerNum)
{
	SERVERINFO* pInfo = GetServer(ServerKind,ServerNum);
	return pInfo ? pInfo->wPortForServer : 0;
}

SERVERINFO* CServerTable::GetServer(WORD ServerKind, WORD ServerNum)
{
	SetPositionHead();
	SERVERINFO * info = NULL;
	while(info = (SERVERINFO *)GetData())
	{
		if(info->wServerKind == ServerKind && info->wServerNum == ServerNum)
		{
			return info;
		}
	}
	return NULL;
}

WORD CServerTable::GetServerNum(WORD ServerPort)
{
	SetPositionHead();
	SERVERINFO * info = NULL;
	while(info = (SERVERINFO *)GetData())
	{
		if(info->wPortForServer == ServerPort)
		{
			return info->wServerNum;
		}
	}
	return 0;
}
SERVERINFO* CServerTable::GetFastServer(WORD ServerKind)
{
	SetPositionHead();
	SERVERINFO * info = NULL;
	WORD min_cnt = 65535;
	SERVERINFO * min_info = 0;
	while(info = (SERVERINFO *)g_pServerTable->GetData())
	{
		// ���ӵ� ������Ʈ �߿��� 
		if(info->dwConnectionIndex != 0 && info->wServerKind == ServerKind )
		{
			if( info->wServerKind == AGENT_SERVER )
			if( info->wServerNum >=0 && info->wServerNum < 8 )
			{
				if(m_nDisableAgent[info->wServerNum]==TRUE)//AAA
					continue;
			}

			if(info->wAgentUserCnt < min_cnt)
			{
				min_cnt = info->wAgentUserCnt;
				min_info = info;
			}
		}
	}
	
	if(min_info == NULL)
		return 0;
	
	return min_info;
}
BOOL CServerTable::GetFastServer(WORD ServerKind, char* pOutIP,WORD* pOutPort)
{	
	SetPositionHead();
	SERVERINFO * info = NULL;
	WORD min_cnt = 65535;
	SERVERINFO * min_info = 0;
	while(info = (SERVERINFO *)g_pServerTable->GetData())
	{
		// ���ӵ� ������Ʈ �߿��� 
		if(info->dwConnectionIndex != 0 && info->wServerKind == ServerKind )
		{
			if( info->wServerKind == AGENT_SERVER )
			if( info->wServerNum >=0 && info->wServerNum < 8 )
			{
				if(m_nDisableAgent[info->wServerNum]==TRUE)//AAA
					continue;
			}

			if(info->wAgentUserCnt < min_cnt)
			{
				min_cnt = info->wAgentUserCnt;
				min_info = info;
			}
		}
	}
	
	if(min_info == NULL)
		return FALSE;
	sprintf(pOutIP, "%s", min_info->szIPForServer);
	*pOutPort = min_info->wPortForUser;
	return TRUE;
}

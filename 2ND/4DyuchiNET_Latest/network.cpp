//#define _WIN32_WINNT=0x0500

#include "network.h"
#include "cpio.h"
#include "log.h"
#include "accept.h"
#include "connect.h"

CNetwork* g_pNetworkForUser = NULL;
CNetwork* g_pNetworkForServer = NULL;


//
// ����Ʈ���� 
//
CNetwork::CNetwork(NETWORK_ID id,DWORD num,DWORD dwAcceptInterval,DWORD dwFlag)
{
	// ���� �ʱ�ȭ
	m_pDMWrite = NULL;
	m_pDMRead = NULL;
	m_pRecycledConnectionHead = NULL;
	m_pRecycledConnectionTail = NULL;
	m_dwRecycledConnectionNum = NULL;
	m_bEncryptionSend = FALSE;
	m_iMaxTransferRecvSizePerConnection = DEFAULT_TRANSFER_RECV_SIZE;
	m_iMaxTransferSendSizePerConnection = DEFAULT_TRANSFER_SEND_SIZE;
	m_dwBufferSizePerConnection = 0;
	m_socketBase = INVALID_SOCKET;
	m_dwConnectionNum = 0;
	m_dwMaxConnectionNum = num; 
	m_dwNetworkID = id;
	m_pConnectionList = NULL;
	m_dwConnectionNum = 0;
	m_wPort = 0;
	m_dwAcceptInterval = dwAcceptInterval;

//	Modified by chan78 at 2002/07/22
	m_hAcceptThread;

	if ( (dwFlag & NETDDSC_ENCRYPTION_MASK) == NETDDSC_ENCRYPTION)
		EnableEncryptionSend();
	else
		DisableEncryptionSend();

	memset(&m_sckadrAdress,0,sizeof(sockaddr_in));
	
	// ���ؼ� ������ �迭 ����.�ʱ�ȭ
	m_pConnectionList = new CONNECTION_DESC[m_dwMaxConnectionNum+1];
	memset(m_pConnectionList,0,sizeof(CONNECTION_DESC)*(m_dwMaxConnectionNum+1));

	m_IC.Initialize(m_dwMaxConnectionNum);
}
//
// listen socket ������
//
void CNetwork::ReleaseListenSocket()
{
	EndAcceptThread( m_socketBase, m_hAcceptThread );
	SetAcceptTHreadHandle( NULL );
}

void CNetwork::CompulsiveDisconnect(DWORD dwIndex)
{
	if (!dwIndex)
		return;

	CConnection* pCon;
	GetConnection(&pCon,dwIndex);

	if (pCon)
	{
		PostDisconnectEvent(pCon->GetHandleID());
	}
	
}
CConnection* CNetwork::AllocConnection()
{
	CConnection* pCon = NULL;

	if (!m_dwRecycledConnectionNum)
	{
		pCon = new CConnection;
		goto lb_return;
		
	}
	pCon = m_pRecycledConnectionHead;
	m_pRecycledConnectionHead = m_pRecycledConnectionHead->GetNextConnection();
	
	m_dwRecycledConnectionNum--;

lb_return:
	return pCon;

}
void CNetwork::FreeConnection(CConnection* pCon)
{
	pCon->AddNextConnection(NULL);
	
	if (!m_dwRecycledConnectionNum)
		m_pRecycledConnectionTail = m_pRecycledConnectionHead = pCon;
	else 
		m_pRecycledConnectionTail->AddNextConnection(pCon);
	
	m_pRecycledConnectionTail = pCon;
	m_dwRecycledConnectionNum++;
}

//
// send�Լ�.
// ���� �޽����� send�ϴ� �Լ�.
BOOL CNetwork::Send(DWORD index,char* msg,DWORD length,DWORD flag)
{
	//
	// Send �Լ�.CConnectoin::Send()�� ���� �Լ��̴�.�������� index�� ���ڷ� �޾� index�� connnection��
	// ���� �����ϴ��� Ȯ�� �� send�Ѵ�.
	//
	BOOL	bResult = FALSE;
	if (index > m_dwMaxConnectionNum)
		goto lb_return;

	if (!index)
		goto lb_return;

	if (m_pConnectionList[index].status != CONNECTION_STATUS_CONNECTED)
		goto lb_return;
	
	
	bResult = m_pConnectionList[index].pConnection->Send(msg,length);

lb_return:
	return bResult;
}
BOOL CNetwork::Send(DWORD index,WSABUF* pBuf,DWORD dwNum,DWORD flag)
{
	//
	// Send �Լ�.CConnectoin::Send()�� ���� �Լ��̴�.�������� index�� ���ڷ� �޾� index�� connnection��
	// ���� �����ϴ��� Ȯ�� �� send�Ѵ�.
	//
	BOOL	bResult = FALSE;

	if (index > m_dwMaxConnectionNum)
		goto lb_return;

	if (!index)
		goto lb_return;

	if (m_pConnectionList[index].status != CONNECTION_STATUS_CONNECTED)
		goto lb_return;

	bResult = m_pConnectionList[index].pConnection->Send(pBuf,dwNum);
	

lb_return:
	return bResult;
}
BOOL CNetwork::Send(DWORD index,PACKET_LIST* pList,DWORD flag)
{	
	//
	// Send �Լ�.CConnectoin::Send()�� ���� �Լ��̴�.�������� index�� ���ڷ� �޾� index�� connnection��
	// ���� �����ϴ��� Ȯ�� �� send�Ѵ�.
	//
	BOOL	bResult = FALSE;

	if (index > m_dwMaxConnectionNum)
		goto lb_return;

	if (!index)
		goto lb_return;

	if (m_pConnectionList[index].status != CONNECTION_STATUS_CONNECTED)
		goto lb_return;

	bResult = m_pConnectionList[index].pConnection->Send(pList);
	
lb_return:
	return bResult;
}
//
// ���� ��ŸƮ
//
BOOL CNetwork::StartServer(char* ip,WORD port)
{
	//
	// ���ڷ� ���� ip�� port�� ������ �����Ѵ�.
	//
	char txt[128];
	memset(txt,0,128);

	// ���� ������ �����.�� ������ ��Ʈ�� ����̽��� ���ε� �� ���̴�.
	m_socketBase = socket(AF_INET, SOCK_STREAM, 0);

	if(m_socketBase == INVALID_SOCKET )
	{
		MessageBox(GetActiveWindow(),"Fail to create socket","Error",MB_OK);
		return FALSE;
	}

	// �ּ� ����.
	m_sckadrAdress.sin_family = AF_INET;
	m_sckadrAdress.sin_port = htons(port);
	m_sckadrAdress.sin_addr.s_addr = inet_addr(ip);
	//m_sckadrAdress.sin_addr.s_addr = htons(INADDR_ANY);

	// ���� ��Ʈ�� ����̽��� ���ε��Ѵ�.
	if(bind(m_socketBase, (struct sockaddr *)&m_sckadrAdress, sizeof(m_sckadrAdress)) < 0) 
	{
		closesocket(m_socketBase);
		m_socketBase = INVALID_SOCKET;
		
		return FALSE;
	}
	
	// ������ ��û�� ��ٸ���.
	if(listen(m_socketBase,SOMAXCONN) < 0) 
	{
		MessageBox(GetActiveWindow(),"Socket Listen Fail","Error",MB_OK);
		closesocket(m_socketBase);
		m_socketBase = INVALID_SOCKET;
		
		return FALSE;
	}

	StartAcceptThread(this);

	return TRUE;
}

void CNetwork::GetConnection(CConnection** ppCon,DWORD dwConnectionIndex)
{
	 // index�� �̿��ؼ� Connection Ŭ���� �����͸� ����
	if (dwConnectionIndex > m_dwMaxConnectionNum)
		*ppCon = NULL;
	else	
		*ppCon = m_pConnectionList[dwConnectionIndex].pConnection; 

	return;
}

void CNetwork::CloseAllConnection()
{
	//
	// �� ��Ʈ�� Ŭ������ ��� ������ ���������.
	//
	if (!m_pConnectionList)
		goto lb_return;

	DWORD	i;
	for (i=1; i<=m_dwMaxConnectionNum; i++)
	{
		if (m_pConnectionList[i].pConnection)
		{
			// Ȥ�� send ���۰� �� ���� ���� �༮�� ������ �𸣹Ƿ�...
			// CConnectionŬ���� �ν��Ͻ� ����.
			m_pConnectionList[i].pConnection->CloseSocket();
			FreeConnection(m_pConnectionList[i].pConnection);
			m_pConnectionList[i].pConnection = NULL;
			m_pConnectionList[i].status = CONNECTION_STATUS_DISCONNECTED;
		}

	}
	m_dwConnectionNum = 0;
lb_return:
	return;
}

void CNetwork::CloseConnection(DWORD dwConnectionIndex)
{
	// 
	// Ư�� connection�� �����Ѵ�.��Ȯ���� �̹� ������ ���� connection�� ���� ���ҽ��� �����ϴ°��̴�.
	// �� �Լ��� ȣ��Ǵ� ������ worker�����忡�� ������ ������ �����ϰ� �� ����� ���� �����忡 �˷��� �����̴�.
	// �и��� �� �Լ��� ���� �����忡�� ȣ��Ǿ�� �Ѵ�. ���Ƿ� �� �Լ��� �ٸ� �����忡�� ȣ���� �ÿ��� �ſ� ������
	// ����� �ʷ��Ѵ�.
	//


	if (m_pConnectionList[dwConnectionIndex].status != CONNECTION_STATUS_CONNECTED)
		goto lb_return;

	

	// connectionŬ���� ������ �迭�� ���� �ش� connection�� �ε����� ã�� null�� �ʱ�ȭ�Ѵ�.
	// ���� CNetwork::Send()�Լ��� ������ ��� �� connection�� ���ؼ� false�� ������ ���̴�.
	CConnection* pConnection;
	pConnection = m_pConnectionList[dwConnectionIndex].pConnection;	
	m_pConnectionList[dwConnectionIndex].pConnection = NULL;
	m_pConnectionList[dwConnectionIndex].status = CONNECTION_STATUS_DISCONNECTED;

	
	
	if (pConnection)
	{
		// ������ ����� ���ҽ��� ���������� ���÷��� �Ѵ�.
		if (pConnection->GetConnectionIndex() != dwConnectionIndex)
			__asm int 3

		ICFreeIndex(&m_IC,pConnection->GetConnectionIndex());

		pConnection->SetConnectionStatus(CONNECTION_STATUS_DISCONNECTED);
		pConnection->CloseSocket();
		FreeConnection(pConnection);
		m_dwConnectionNum--;
	}

	if (g_pLogManager)
	{
		char txt[32];
		wsprintf(txt,"disconnectd %d",dwConnectionIndex);
		g_pLogManager->WriteLog(txt,lstrlen(txt));
	}
lb_return:
	return;
}

CConnection* CNetwork::AddConnection(SOCKET socket)
{
	char	txt[32];

	CConnection* pCon = NULL;
	
	DWORD	dwConnectionIndex = ICAllocIndex(&m_IC);
	if (!dwConnectionIndex)
		goto lb_return;


	// ��Ʈ��ũ Ŭ������ connectionŬ���� ������ �迭�� ���� ������ connectionŬ������ �����͸� �߰��Ѵ�.
	// ���� index�� �����ϰ� �ڵ��� �ο��Ѵ�.�� �ڵ��� ���ø��� ��Ʈ�� ����Ͽ� ioó���� ���� ��������� �Ѵ�.


	// CConnectionŬ������ �����Ѵ�.���ڷ� ���� ������� �ִ� ���� ����� �Ѱ��ش�.
	// ���� ����� ���Ƿ� �������� ���� �������� �����ŭ send,recv���۸� ��´�.�ƴ϶�� MAX_BUFFER_SIZE�� ���ǵ� ��ŭ
	// ���� ����� ��´�.
	pCon = AllocConnection();
	pCon->SetNetwork(this);
	pCon->Initialize(socket,m_dwBufferSizePerConnection,GetMaxTransferRecvSize(),GetMaxTransferSendSize());
	
	pCon->SetConnectionIndex(dwConnectionIndex);
	m_pConnectionList[dwConnectionIndex].pConnection = pCon;
	m_pConnectionList[dwConnectionIndex].status = CONNECTION_STATUS_CONNECTED;
	pCon->SetConnectionStatus(CONNECTION_STATUS_CONNECTED);

	pCon->SetHandleID(m_dwNetworkID,pCon->GetConnectionIndex(),ACTION_TYPE_TCP);
	
	
	sockaddr_in addr;
	memset(&addr,0,sizeof(sockaddr));
	
	int len;
	len = sizeof(sockaddr);
	
	int result;
	result = getpeername (socket,(sockaddr*)&addr,&len);

	pCon->SetAddress(&addr);
	


	// keep alive segment ����
	BOOL	alive;
	alive = TRUE;
	setsockopt(socket,SOL_SOCKET,SO_KEEPALIVE,(char*)&alive,4);

	tcp_keepalive keep_alive;
	keep_alive.onoff = 1;
	keep_alive.keepalivetime = 3000;
	keep_alive.keepaliveinterval = 1000;

	DWORD dwBytes;
	dwBytes = 0;
	WSAIoctl(socket,SIO_KEEPALIVE_VALS,(void*)&keep_alive,sizeof(keep_alive),(void*)&keep_alive,sizeof(keep_alive),
		&dwBytes,NULL,NULL);

	// send���۰� �� á�� ��쿡 ��� ó������ �����Ѵ�.����Ʈ�� send���۰� á�� ���� �޽����� ����ϴ� 
	// �ɼ��� ���ִ�.
	SetConnectionType(pCon,m_dwNetworkID);

	// ���ø��� ��Ʈ�� ���ε��Ѵ�.�������� ��� ������ �༮�� ���Ͽ� ���õ� �̺�Ʈ�� ���ø��� ��Ʈ���� �˷��ش�.
	if (CreateIoCompletionPort((HANDLE)socket, g_hCompletionPort, (DWORD)(pCon->GetHandleID()),0) == NULL)
	{
		MessageBox(GetActiveWindow(),"CreateIoCompletionPort failed with error","Error",MB_OK);
		return FALSE;
	}
	
	if (g_pLogManager)
	{

		wsprintf(txt,"AddConnection %d",pCon->GetConnectionIndex());
		g_pLogManager->WriteLog(txt,lstrlen(txt));
	}
	m_dwConnectionNum++;	// ���� �����ϰ� �ִ� �� connection���� �ϳ� ����.
lb_return:

	return pCon;
}
BOOL CNetwork::ConnectToServer(char* szIP,WORD wPort,CONNECTSUCCESSFUNC SuccessFunc,CONNECTFAILFUNC FailFunc,void* pExt)
{
	//
	// �ٸ� ������ ������ �õ��ϴ� �Լ��̴�.
	// �� ��Ʈ��ũ ����� Ŭ���̾�Ʈ�� �۵��ϴ� ���, �л� ���� �ý��ۿ��� ������ �������� �ٸ� ������ �����ϴ� 
	// ��� �� �Լ��� ����Ѵ�.
	return Connect(szIP,wPort,SuccessFunc,FailFunc,this,pExt);

}
void CNetwork::SetConnectionType(CConnection* con,NETWORK_ID net_id)
{
	// 
	// connection Ÿ�Կ� ���� send���۰� �� ����� �ൿ�� �������ִ� �Լ��̴�.
	// ���� ������ �ʿ�� ����.����� ���۰� �� ��� ������ ���Ŀ� send�ϴ� �޽�
	// ���� ����Ѵ�.
	//

	if (net_id == ID_NETWORK_FOR_USER)
	{
		con->GetOverlappedSend()->SetBlockMethod(BLOCK_METHOD_IGNORE);
		con->GetOverlappedRecv()->SetBlockMethod(BLOCK_METHOD_IGNORE);
	}
	else if (net_id == ID_NETWORK_FOR_SERVER)
	{
		con->GetOverlappedSend()->SetBlockMethod(BLOCK_METHOD_IGNORE);
		con->GetOverlappedRecv()->SetBlockMethod(BLOCK_METHOD_IGNORE);

//		con->m_SendOverlapped.SetBlockMethod(BLOCK_METHOD_WAIT);
//		con->m_RecvOverlapped.SetBlockMethod(BLOCK_METHOD_WAIT);
	}
	else
		MessageBox(GetActiveWindow(),"Network ID illegal","Error",MB_OK);

}

void CNetwork::Broadcast(char* pMsg,DWORD len,DWORD flag)
{
	for (DWORD i=1; i<=m_dwMaxConnectionNum; i++)
	{
		if (m_pConnectionList[i].pConnection)
		{
			if (m_bEncryptionSend)
				m_pConnectionList[i].pConnection->Send(pMsg,len,flag);
			else
				m_pConnectionList[i].pConnection->Send(pMsg,len);
		}
	}
}
void CNetwork::SetBufferSizePerConnection(DWORD size)
{
	if (m_pDMWrite || m_pDMRead)
		return;

	
	m_pDMRead = new CDynamicMemoryContainer;
	m_pDMWrite = new CDynamicMemoryContainer;
	m_pDMRead->Initialize(size,3,m_dwMaxConnectionNum,MEMORY_TYPE_FIXED);
	m_pDMWrite->Initialize(size,3,m_dwMaxConnectionNum,MEMORY_TYPE_CHAIN);

	m_dwBufferSizePerConnection = size;	// ���ؼǴ� ���� ����� ����
}


CNetwork::~CNetwork()
{

	// �����ִ� ���ؼ��� �����ϰ� �޸𸮸� �����Ѵ�.
	
	CloseAllConnection();	// ��� ���ؼ� ������ 

	// ���ؼ� ������ ���̺� ������
	if (m_pConnectionList)
	{
		delete [] m_pConnectionList;
		m_pConnectionList = NULL;

	}

	CConnection* cur = m_pRecycledConnectionHead;
	CConnection* next = NULL;

	while (cur)
	{
		next = cur->GetNextConnection();
		delete cur;
		cur = next;
		
	}
	if (m_pDMRead)
	{
		delete m_pDMRead;
		m_pDMRead = NULL;
	}

	if (m_pDMWrite)
	{
		delete m_pDMWrite;
		m_pDMWrite = NULL;
	}
	if( m_hAcceptThread && ((DWORD)m_hAcceptThread != 0xFFFFFFFF) )
		CloseHandle(m_hAcceptThread);

}


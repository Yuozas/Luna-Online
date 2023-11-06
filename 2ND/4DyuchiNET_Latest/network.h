#pragma once

//
// ������ �޾� ������ CONNECTION�� ������ �����ϰų� Ȥ�� ������ ������ �õ��Ͽ� ������ �� �ִ� Ŭ�����̴�.
// �����μ��� �ּ����� ����� �� �� �ְ� ���������� ���� ������ �����ϴ�.
//

#include "../4DyuchiNET_Common/stdafx.h"
#include "../4DyuchiNET_Common/typedef.h"
#include "connection.h"
#include "create_index.h"

//#include "que.h"
//#include "dynamic_memory.h"
class CPrfCheckContainer;

struct CONNECTION_DESC
{
	CConnection*		pConnection;
	CONNECTION_STATUS	status;
};
/*
GLOBAL_FUNC_DLL STMPOOL_HANDLE	__stdcall	;
GLOBAL_FUNC_DLL void			__stdcall	;
GLOBAL_FUNC_DLL BOOL			__stdcall	InitializeStaticMemoryPool(STMPOOL_HANDLE pool,DWORD dwUnitSize,DWORD dwDefaultCommitNum,DWORD dwMaxNum);

GLOBAL_FUNC_DLL void*			__stdcall	LALAlloc(STMPOOL_HANDLE pool);
GLOBAL_FUNC_DLL void			__stdcall	LALFree(STMPOOL_HANDLE pool,void* pMemory);
*/

class CNetwork
{
	NETWORK_ID				m_dwNetworkID;		// �� ��Ʈ�� Ŭ������ ������ ���� ������ ������ ���� ������ �ĺ� ���̵�.

	CONNECTION_DESC*		m_pConnectionList;
	CIndexCreator			m_IC;

//	CConnection**			m_ppConnectionList;	// ���ؼ� ������ ���̺��� ������.

	CConnection*			m_pRecycledConnectionHead;
	CConnection*			m_pRecycledConnectionTail;
	DWORD					m_dwRecycledConnectionNum;


	WORD					m_wPort;			// �� ��Ʈ��ũ Ŭ������ ���ε� �ϰ� �ִ� �ּ��� ��Ʈ��ȣ.
	DWORD					m_dwConnectionNum;	// ���� �����ϰ��ִ� ���ؼ� ��.
	DWORD					m_dwMaxConnectionNum;	// �ִ� ���ؼ� ��.
	SOCKET					m_socketBase;			// listen socket
	sockaddr_in				m_sckadrAdress;			// ���ε��� �ּ� ����ü.
	DWORD					m_dwBufferSizePerConnection;	// ���ؼ� �� ���� ������.�� ���� �ϴ��� �� ��Ʈ�� Ŭ������ ���� ��� ���ؼǿ� ���� �ϰ� ����ȴ�.
	int						m_iMaxTransferRecvSizePerConnection;	// �ִ� recv������.�� �� ���� �� ��Ʈ�� Ŭ������ ���� ��� ���ؼǿ� ���� �Ұ� ����ȴ�. 
	int						m_iMaxTransferSendSizePerConnection;	// �ִ� send������.�� �� ���� �� ��Ʈ�� Ŭ������ ���� ��� ���ؼǿ� ���� �Ұ� ����ȴ�. 
	ACCEPTFUNC				m_pOnAccept;							// ���ӵǾ����� ������ �Լ�
//	RECVFUNC				(*m_pOnRecvUDP)(DWORD dwAddr,char* pMsg,DWORD dwLength);	// UDP��Ŷ�� ���������� ������ �Լ�.
	DISCONNECTFUNC			m_pOnDisconnect;
	RECVFUNC				m_pOnRecvTCP;
	DWORD					m_dwAcceptInterval;
	
	CDynamicMemoryContainer*	m_pDMWrite;
	CDynamicMemoryContainer*	m_pDMRead;
	BOOL					m_bEncryptionSend;
	HANDLE					m_hAcceptThread;

	CConnection*			AllocConnection();
	void					FreeConnection(CConnection* pCon);

public:
	DWORD					GetAcceptInterval()		{return m_dwAcceptInterval;}
	DWORD					GetActorType()			{return m_dwNetworkID;}
	HANDLE					GetAcceptThreadHandle()		{return m_hAcceptThread;}
	void					SetAcceptTHreadHandle( HANDLE hAcceptThread ) { m_hAcceptThread = hAcceptThread; }
	void					ReleaseListenSocket();	// listen socket ������.
	CDynamicMemoryContainer*	GetDMWrite()			{return m_pDMWrite;}
	CDynamicMemoryContainer*	GetDMRead()				{return m_pDMRead;}
	void					EnableEncryptionSend() {m_bEncryptionSend = TRUE;}
	void					DisableEncryptionSend() {m_bEncryptionSend = FALSE;}
	CConnection*			AddConnection(SOCKET socket);
	void					CompulsiveDisconnect(DWORD dwIndex);			
	int						GetMaxTransferRecvSize() {return m_iMaxTransferRecvSizePerConnection;} // ���ؼǴ� �ִ� recv������ ����
	int						GetMaxTransferSendSize() {return m_iMaxTransferSendSizePerConnection;} // ���ؼǴ� �ִ� send������ ����
	void					SetMaxTransferRecvSize(int size) {m_iMaxTransferRecvSizePerConnection = size;} // ���ؼŴ� �ִ� recv������ ����
	void					SetMaxTransferSendSize(int size) {m_iMaxTransferSendSizePerConnection = size;} // ���ؼǴ� �ִ� send������ ����
	void					Broadcast(char* pMsg,DWORD len,DWORD flag);
	void					OnDisconnect(DWORD dwConnectionIndex) { m_pOnDisconnect(dwConnectionIndex);}	// ������ ���������� ȣ��Ǵ� �Լ�.
	void					OnRecvTCP(DWORD dwConnectionIndex,char* msg,DWORD len) { m_pOnRecvTCP(dwConnectionIndex,msg,len);} // TCP��Ŷ�� ���������� ȣ��Ǵ� �Լ�.
//	void					OnRecvUDP(DWORD dwAddr,char* msg,DWORD len) { m_pOnRecvUDP(dwAddr,msg,len);}	// UDP��Ŷ�� ���������� ȣ��Ǵ� �Լ�.
	void					OnAccept(DWORD dwConnectionIndex) { m_pOnAccept(dwConnectionIndex);}	// ������ �Ǿ����� ȣ��Ǵ� �Լ�.
	ACCEPTFUNC				GetOnAcceptProc() {return m_pOnAccept;}
//	void					SetOnRecvUDP(void (*pFunc)(DWORD dwAddr,char* msg,DWORD len)) {m_pOnRecvUDP = pFunc;} // UDP��Ŷ�� �޾����� �ڵ�ȣ��Ǵ� �Լ� ����.
	void					SetOnRecvTCP(RECVFUNC pFunc) { m_pOnRecvTCP = pFunc;} // TCP��Ŷ�� �޾����� �ڵ�ȣ��Ǵ� �Լ� ����.
	void					SetOnDisconnect(DISCONNECTFUNC pFunc) {m_pOnDisconnect = pFunc;} // ������ ���������� �ڵ�ȣ��Ǵ� �Լ� ����
	void					SetOnAccept(ACCEPTFUNC pFunc) { m_pOnAccept = pFunc;} // ������ �Ǿ����� �ڵ�ȣ��Ǵ� �Լ� ����.

	void					SetConnectionType(CConnection* con,NETWORK_ID net_id);	// ���ؼǴ� �� �޼ҵ带 �����ϴ� �Լ�.
	
	BOOL					Send(DWORD index,char* msg,DWORD length,DWORD flag);	
	BOOL					Send(DWORD index,WSABUF* pBuf,DWORD dwNum,DWORD flag);	
	BOOL					Send(DWORD index,PACKET_LIST* pList,DWORD flag);
	SOCKET					GetListenSocket() {return m_socketBase;}	// listen socket ��ũ���͸� ����.
	void		 			GetConnection(CConnection** ppCon,DWORD index);
	
	void					SetBufferSizePerConnection(DWORD size);
	DWORD					GetConnectionNum() {return m_dwConnectionNum;}	// ���� ���ؼ� ����.
	WORD					GetPort() {return m_wPort;}	// ���� ���ε� �ϰ� �ִ� �ּ��� ��Ʈ�� ����.
	BOOL					ConnectToServer(char* szIP,WORD wPort,CONNECTSUCCESSFUNC SuccessFunc,CONNECTFAILFUNC FailFunc,void* pExt);
	BOOL					StartServer(char* ip,WORD port);	// Ư�� ip�� ��Ʈ�� ������ ��ŸƮ�Ѵ�.
	void					CloseAllConnection();	// �� ��Ʈ�� Ŭ������ ��� ���ؼ��� release�Ѵ�.
	void					CloseConnection(DWORD index); // Ư�� �ε����� ���ؼ��� release�Ѵ�.

	CNetwork(NETWORK_ID id,DWORD num,DWORD dwAcceptInterval,DWORD dwFlag);
	~CNetwork();
	
};

extern CNetwork* g_pNetworkForUser;
extern CNetwork* g_pNetworkForServer;

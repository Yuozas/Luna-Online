#pragma once

//
// ����(CONNECTION)�� �̷������ �ش� ���ӿ� ���� RECV,SEND,DISCONNECT���� ó���ؾ��Ѵ�.
// �̷��� ���� ó���ϱ⿡ SOCKET��ũ���ʹ� ���뼺�� �ʹ� �����ϹǷ� �ϳ��� ������ �Ѱ��Ҽ� 
// �ִ� Ŭ������ �����Ѵ�.�� Ŭ������ CConnection Ŭ���� �̴�.
//

#include "../4DyuchiNET_Common/net_define.h"
#include "../4DyuchiNET_Common/typedef.h"

//#include "protocol.h"
#include "overlapped_recv.h"
#include "overlapped_send.h"



class CConnection;


class CConnection 
{
	void*				m_pInfo;			// ���� ������ ���� �� �ֵ��� ����� �����͸� �ϳ� �����صξ���.USERINFO�� ������� �������ϱ� �ٶ���.
	CONNECTION_STATUS	m_dwStatus;				// ���ؼ� ����.
	DWORD				m_dwConnectionIndex;	// ���ؼ� ������ �迭�� �ε����̴�.���� �߿��ϰ� ���δ�.
	sockaddr_in			m_address;			// ��� �����ؿԴ��� �ּ�.
	COverlappedSend*	m_pSendOverlapped;
	COverlappedRecv*	m_pRecvOverlapped;
	CConnection*		m_pNext;
	CNetwork*			m_pNetwork;

	
	
public:
	COverlappedSend*	GetOverlappedSend()	{return m_pSendOverlapped;}
	COverlappedRecv*	GetOverlappedRecv() {return m_pRecvOverlapped;}
	void				AddNextConnection(CConnection* pCon) {m_pNext = pCon;}
	CConnection*		GetNextConnection() {return m_pNext;}
	void				CloseSocket();
	void				Release();	
	
	void 				SetHandleID(NETWORK_ID apType,DWORD dwIndex,ACTION_TYPE acType)
	{
		m_pSendOverlapped->SetHandleID(apType,dwIndex,acType);
		m_pRecvOverlapped->SetHandleID(apType,dwIndex,acType);
	}
	DWORD				GetHandleID() {return m_pRecvOverlapped->GetHandleID();}	// �ڵ� ����

	DWORD				GetConnectionIndex() {return m_dwConnectionIndex;}	// index����
	void				SetConnectionIndex(DWORD index) {m_dwConnectionIndex = index;}	// index����
	BOOL				PrepareRead() {return m_pRecvOverlapped->PrepareRead();}// recv��û.

	void				SetInfo(void* info) {if (m_dwStatus == CONNECTION_STATUS_DISCONNECTED)return; m_pInfo = info;} // �������� ������ ����
	void*				GetInfo() {if (m_dwStatus == CONNECTION_STATUS_DISCONNECTED)return NULL;return m_pInfo;}	// �������� ������ ���� 
	void				SetConnectionStatus(CONNECTION_STATUS st) { m_dwStatus = st;}	// ���� ����.
	DWORD				GetConnectionStatus() {return m_dwStatus;}	// ���� ����

	sockaddr_in*		GetAddress() {return &m_address;}	// �����ؿ� �ּ� ����.
	void				SetAddress(sockaddr_in* addr) {memcpy(&m_address,addr,sizeof(sockaddr_in));}	// �����ؿ� �ּ� ����
	BOOL				GetAddress(char* pIP,WORD* pwport);

	BOOL				Send(char* msg,DWORD length,DWORD flag);	// ���� �߿��� �Լ�.�޽����� ������.
	BOOL				Send(char* msg,DWORD length);
	BOOL				Send(WSABUF* pBuf,DWORD dwNum);
	BOOL				Send(PACKET_LIST* pList);
	BOOL				Initialize(SOCKET s,DWORD size,int max_recv_size,int max_send_size);
	void				Clear();
	void				SetNetwork(CNetwork* pNet) {m_pNetwork = pNet;}
	CConnection();		
	~CConnection();
};

typedef CConnection* LPCConnection;


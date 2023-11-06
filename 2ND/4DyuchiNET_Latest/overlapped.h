//
// �������� ����� �ۼ��ſ��� �ݵ�� �ʿ��� OVERLAPPED ����ü�� Ȯ���ϰ� ��� ���� �޼ҵ带 �߰��ߴ�.
// TCP�ۼ��ſ��� ����� �ȵ� SEND,RECVť�� �� OVERLAPPED_IO ����ü�� �����Ǹ� ������ thread safe �� �����Ǿ��ִ�.
// connectionŬ������ work������ �ܿ��� �＼�� �� ���� ���� ������ �޼ҵ带 ������ ���� �＼���� ���� ���⸦ ���Ѵ�.
//
#pragma once 

#include "../4DyuchiNET_Common/stdafx.h"
#include "../4DyuchiNET_Common/net_define.h"


#include "define.h"
#include "write_memory_handle.h"
#include "read_memory_handle.h"


// io type bit flag

// READ or WRITE [0-7] 8 bit 

enum IO_TYPE
{
	IO_TYPE_WRITE =	0x00000001,
	IO_TYPE_READ  =	0x00000002,
	IO_TYPE_WAIT  = 0x00000003
};

/*---------------------------------------------------------------------------------------------------------------*/
// �� �޼ҵ尡 ���� �ϸ�...
// sendť�� ���� ���� �� �̻� send�� �� ���� ��쿡 ��� ó�� �� �������� ó���ϴ� �ɼ��̴�.
// OVERLAPPED_IO::SetBlockMethod()�� ������ �� ������ ���� io�����߿��� �ɼ��� �߲� �� �ִ�.
// 
// ex)  CConnection* con = send�� ���ؼ� ������;
//		con->OverlappedSend.SetBlockMethod(BLOCK_METHOD_IGNORE);	sendť�� �� á�� ���� send�Լ� ����
//		con->OverlappedSEnd.SetBlockMethod(BLOCK_METHOD_WAIT);		sendť�� �������� ���.���� ��ü�� �����ս� ����.		
// BLOCK_METHOD_WAIT�� ������ ���� �����尡 sendť�� �� ������ ����ϴ� ���°� �ǹǷ� �����ϸ� ���� �ʴ� ���� ����.
// ����Ʈ�� ������ �̷������ ��� ���ؼ��� BLOCK_METHOD_IGNORE�� �����ȴ�.���� ũ��Ƽ���� ��Ŷ�� �������ϴ� ��쿡�� �������Ѵ�.

// when it can't send because que is overflow 	[8-15] 8 bit	
enum BLOCK_METHOD
{
	BLOCK_METHOD_IGNORE =	0x0000100
//	BLOCK_METHOD_WAIT  =	0x0000200		������ 
};
#define		IO_TYPE_MASK				0x000000ff
#define		BLOCK_METHOD_MASK			0x0000ff00

#define		IO_TYPE_MASK_INVERSE		0xffffff00
#define		BLOCK_METHOD_MASK_INVERSE	0xffff00ff

#define		IO_TYPE_MASK_SENDABLE		0xff000000

/*---------------------------------------------------------------------------------------------------------------*/
class CNetwork;
class COverlapped
{
protected:
	OVERLAPPED				m_Overlapped;				// �̰��� �⺻������ OVERLAPPED���¿��� �Ѵ�.���� �� ���� OVERLAPPED����ü	
	DWORD					m_dwIoType;					// SEND�� �������̳� RECV�� �������̳İ� ǥ�õȴ�.
	DWORD					m_dwTransferredBytes;		// WSASend,WSARecv�� ���� ���ܵ� 4����Ʈ.�����Ѵ�.
	SOCKET					m_socket;					// �� �������� ����� ���� ��ũ����
	int						m_iMaxTransferSize;			// �ѹ��� ���� �� �ִ� �ִ� ��Ŷ ũ��
	CNetwork*				m_pNetwork;
	DWORD					m_dwFlag;
	DWORD					m_dwHandle;

public:
	void 				SetHandleID(NETWORK_ID apType,DWORD dwIndex,ACTION_TYPE acType) {m_dwHandle = dwIndex | acType | apType;} // �ڵ� ����
	DWORD				GetHandleID() {return m_dwHandle;}
	void				SetNetwork(CNetwork* pNet) {m_pNetwork = pNet;}
	void				SetMaxTransferSize(int size)		{m_iMaxTransferSize = size;}	// �ִ� ���� ����� ����.
	int					GetMaxTransferSize()				{return m_iMaxTransferSize;}	// �ִ� ���� ����� ����.
	void				SetBlockMethod(BLOCK_METHOD bm)		{m_dwIoType = m_dwIoType & BLOCK_METHOD_MASK_INVERSE | bm;} // sendť�� �� á�� ����� ó�� ����� ����.
	BLOCK_METHOD		GetBlockMethod()					{return (BLOCK_METHOD)(m_dwIoType & BLOCK_METHOD_MASK);}	// sendť�� �� á�� ����� ��� ó���ϴ��� ���¸� ����.
	void inline			SetIOType(IO_TYPE type)				{m_dwIoType = m_dwIoType & IO_TYPE_MASK_INVERSE | type;}	// send�� ���������� recv������ ����.
	IO_TYPE				GetIOType() {return					(IO_TYPE)(m_dwIoType & IO_TYPE_MASK);}					// send�� ���������� recv������ ����
	SOCKET				GetSocket()							{return m_socket;}	// socket ��ũ���� ����
	void				SetSocket(SOCKET s)					{m_socket = s;}		// ���� ��ũ���� ���� ����.���������� ����.

	COverlapped();
	~COverlapped();
};





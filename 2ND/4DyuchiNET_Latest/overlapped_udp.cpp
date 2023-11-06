#include "overlapped_udp.h"

COverlappedUDP::COverlappedUDP()
{
	Initialize();
}
void COverlappedUDP::Initialize()
{
	// ����Ʈ����.���� �ʱ�ȭ.
	// ������ �ʱ�ȭ�Ѵ�.
	memset(&m_Overlapped,0,sizeof(m_Overlapped));
	m_socket = INVALID_SOCKET;
	m_dwIoType = IO_TYPE_READ;
	m_dwFlag = MSG_PARTIAL;
	m_iAddrLength = sizeof(SOCKADDR_IN);
}

BOOL COverlappedUDP::PrepareRead()
{	
	int result;
	WSABUF wsabuf;
	

	wsabuf.buf = m_pBuffer;
	wsabuf.len = MAX_UDP_BUFFER_SIZE;

	result = WSARecvFrom(
		m_socket,&wsabuf,
		1,
		&m_dwTransferredBytes,
		&m_dwFlag,
		(SOCKADDR *)&m_sckAddr,
		&m_iAddrLength,
		&m_Overlapped,
		NULL);
	
	if (result != SOCKET_ERROR)
		return TRUE;
	
	result = WSAGetLastError();

	if (result == ERROR_IO_PENDING)
		return TRUE;

	// RECV�� �����ϰ� �ɰ��� ������ �ϴ��� ������ ������ �ʴ´�.���⼭ ���Ǵ� ������ ���ؼǺ� ������ �ƴϰ�
	// �� �ڽ��� UDP��Ʈ�� ���ε��� �����̱� �����̴�.������ ������ � UDP��Ŷ�� ���� ���Ѵ�.
	MessageBox(GetActiveWindow(),"WSARecvFrom() Error","Error",MB_OK);
	return FALSE;
}

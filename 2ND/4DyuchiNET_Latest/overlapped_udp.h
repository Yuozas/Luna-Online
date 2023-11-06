#pragma once

#include "overlapped.h"

// �� ����ü�� UDP��Ŷ�� OVERLAPPED������� ó���ϱ� ���� OVERLAPPED����ü�� Ȯ�����̴�.
// UDP�� connection oriented�� �ƴϹǷ� �ּ� �ʵ�� �ʼ����̴�.���� 4����Ʈ ip������ ��Ʈ�� �����ϴ� SOCKADDR_IN����ü
// �� �����Ѵ�.
class COverlappedUDP : public COverlapped
{
	SOCKADDR_IN			m_sckAddr;	// �ּ��ʵ� ��𼭺��� �Դ����� write�� ���̴�.
	int					m_iAddrLength;	// �ּ��� ����.
	char				m_pBuffer[MAX_UDP_BUFFER_SIZE];	// ��Ŷ�� ������ ����.
public:
	DWORD				GetAddress() {return m_sckAddr.sin_addr.S_un.S_addr;}
	void				Initialize();
	void				SetAddrLength(int len) {m_iAddrLength = len;}
	char*				GetBufferPtr() {return m_pBuffer;}// ���� ������ ����
	BOOL				PrepareRead();
	COverlappedUDP();

		
};

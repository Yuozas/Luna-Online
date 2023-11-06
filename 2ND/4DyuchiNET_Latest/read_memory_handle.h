#pragma once

#include "../4DyuchiNET_Common/stdafx.h"

// �޸� ī�Ǹ� ���� �� ����.�޸� ���� ü���� �ϴ� ��� ���� ��Ŷ�� ī���Ҷ�...�� ���� �޽��� ť��
// ������ �޸� ī�Ǹ� �Ѵ�.
// 1.�⺻������ ū ������ ���� �ΰ� �Ҵ� �޴´�.
// 2.OnRead�Ҷ����� ���� ������ �����͸� �ڷ� �о��ش�.������ �����ͺ��� recv�Ѵ�.
// 3.recv�� ������ 100����Ʈ ���� ���Ϸ� �������� ���� ����Ʈ���� �� ������ ī���ϰ� �����µ� ������ ����.
// 4.���ο� ���������κ��� recv�Ѵ�.
// 5.�޽����� ���������� Ȯ���� ��� �޽����� ���� �����Ϳ� ���ο� ���� �Ҵ�ް� ���� �޸𸮴� �ݳ��Ѵ�.

// �޽��� ť�� ��������....�� �����帶�� �޽��� �����Ϳ� ���̸� ���� ��Ʈ���ĸ� �˳��ϰ� �غ��Ͽ� �޽����� Ȯ��
// �ϸ� �ϴ� �� �迭�� �����͸� �ִ´�.ť�κ��� write�� �� �ִ� �ε����� �Ҵ�ް� ���� �ٷ� ����.ī���Ѵ�.

#define		MIN_READ_BYTES	128
#define		MSG_LENGTH_SIZE 0x02
class CReadMemoryHandle
{
	char*					m_pMemory;
	DWORD					m_dwMemoryIndex;
	char*					m_pWritePtr;
	char*					m_pReadPtr;
	DWORD					m_dwReservedBytes;
	DWORD					m_dwUsageBytes;
	DWORD					m_dwMaxMemorySize;
	DWORD					m_dwReserved;
	
	
public:
	
	void					SetMemory(char* pBuffer,DWORD dwSize)
	{
		m_pMemory = pBuffer;
		m_dwMaxMemorySize = dwSize;
		InitializeMemory();
	}
	void					SetMemory(char* pBuffer);
	char*					GetMemory() {return m_pMemory;}
	DWORD					GetMaxMemorySize() {return m_dwMaxMemorySize;}
	DWORD					GetReservedBytes() {return m_dwReservedBytes;}
	
	char*					GetEntryPtr() {return m_pWritePtr;}

	void					OnRead(DWORD dwSize,WSABUF* pBuf,DWORD dwBufNum,DWORD* pdwTotalMsgBytes,DWORD* pdwMsgNum);
	void					OnReadC(DWORD dwSize,WSABUF* pBuf,DWORD dwBufNum,DWORD* pdwTotalMsgBytes,DWORD* pdwTotalMsgNum);
	void					InitializeMemory();
	void					ResetMemory();
	CReadMemoryHandle();
	~CReadMemoryHandle();
	

	
};

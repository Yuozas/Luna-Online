#pragma once

#define	MEMORY_BLOCK_SIZE 64

#include "../4DyuchiNET_Common/stdafx.h"


// �޸� ������Ʈ.64����Ʈ ������ ������.

#define MASK_INIT_NULL		0x00000040	// ��Ʈ������ ��ġ���� �ʱ�ȭ�ϱ� ���� ����ũ.
#define MASK_INIT_FULL		0x40004000	// ��Ʈ������ ��ġ���� �ʱ�ȭ�ϱ� ���� ����ũ.

#define	MASK_RESERVED_BYTES	0x000000ff
#define	MASK_USAGED_BYTES	0x0000ff00
#define MASK_READ_POSITION	0x00ff0000
#define MASK_WRITE_POSITION	0xff000000

struct MEMORY_OBJECT
{
	MEMORY_OBJECT*		m_pNext;
	MEMORY_OBJECT*		m_pPrv;

	BYTE				m_bReservedBytes;
	BYTE				m_bUsageBytes;
	BYTE				m_bReadPosition;		// WSABUF�� ������ ����� ��Ʈ�� ������ �ε���.�޸� ������Ʈ ��常 �ش�.
	BYTE				m_bWritePosition;
	
	char				m_pBuffer[MEMORY_BLOCK_SIZE];
};
#ifdef _NEED_CONFIRM
	extern void (*InitializeMemoryObjectChain)(MEMORY_OBJECT* pMemory,DWORD dwNum);
#else
	void InitializeMemoryObjectChain(MEMORY_OBJECT* pMemory,DWORD dwNum);
#endif






#define SIZE_MEMORY_OBJECT	76

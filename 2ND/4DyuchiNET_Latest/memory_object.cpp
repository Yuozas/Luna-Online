#include "memory_object.h"

#ifdef _NEED_CONFIRM
	void (*InitializeMemoryObjectChain)(MEMORY_OBJECT* pMemory,DWORD dwNum);
#else
void InitializeMemoryObjectChain(MEMORY_OBJECT* pMemory,DWORD dwNum)
{
//	MEMORY_OBJECT*		m_pNext;
//	MEMORY_OBJECT*		m_pPrv;
//	BYTE				m_bReservedBytes;
//	BYTE				m_bUsageBytes;
//	BYTE				m_bReadPosition;		// WSABUF�� ������ ����� ��Ʈ�� ������ �ε���.�޸� ������Ʈ ��常 �ش�.
//	BYTE				m_bWritePosition;
//	char				m_pBuffer[MEMORY_BLOCK_SIZE];

	__asm
	{
		xor			esi,esi
				
		mov			ebx,dword ptr[pMemory]
		mov			ecx,dword ptr[dwNum]
		mov			eax,MASK_INIT_NULL
		mov			edx,ebx
			
		
lb_loop:
		mov			dword ptr[ebx+4],esi		; write prv ptr
		add			edx,SIZE_MEMORY_OBJECT

		mov			dword ptr[ebx],edx			; write next ptr
		mov			esi,ebx						; bakcup cur ptr
		mov			dword ptr[ebx+8],eax		; clear value
		add			ebx,SIZE_MEMORY_OBJECT

		loop		lb_loop

		sub			ebx,SIZE_MEMORY_OBJECT
		mov			dword ptr[ebx],0
		
	}
	
}
#endif

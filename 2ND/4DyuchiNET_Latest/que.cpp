//#define _WIN32_WINNT 0x0500

#include "../4DyuchiNET_Common/stdafx.h"
#include "que.h"
#include "define.h"

CRITICAL_SECTION g_csWrite;


CMsgQue::CMsgQue(DWORD dwMaxBufferSize)
{
	// ����Ʈ����
	// ��� ���� �ʱ�ȭ �� ���۷� �� �޸� �Ҵ�

	m_bWaitFlag = TRUE;
	m_dwOffset = 0;
	m_dwBufferPosition = 0;
	m_dwMsgNum = 0;
	m_dwMaxBufferSize = dwMaxBufferSize;
	m_pBuffer = new char[dwMaxBufferSize];
	memset(m_pBuffer,0,dwMaxBufferSize);
}



MSG_CONTAINER* GetMsgContainer(CMsgQue* pMsgQue)
{
	//
	// �޽��� �����̳��� �����͸� �����ִ� �Լ�.���ڷ� ���� ���������͸� ���� �ѱ��.�ι�° ���ڴ� �޽��� �����̳��� �ε���.
	//
//	DWORD					m_dwMsgNum;				// �׿��ִ� �޽��� ����
//	char*					m_pBuffer;
//	DWORD					m_dwOffset;				// ���� �޽����� ���� �������� ��ġ
//	DWORD					m_dwBufferPosition;		// ���� ���� ����.
//	DWORD					m_dwMaxBufferSize;		// �ִ� ���� ������

	MSG_CONTAINER*	pContainer;
	__asm
	{
		mov			ebx,dword ptr[pMsgQue]
		mov			edx,dword ptr[ebx+8]			; load m_dwOffset
		mov			eax,dword ptr[ebx+4]			; load m_pBuffer
		add			eax,edx
		
		mov			ecx,dword ptr[eax+8]			; load msg's length
		add			ecx,SIZE_MSG_CONTAINER
		add			edx,ecx
		mov			dword ptr[ebx+8],edx			; write m_dwOffset
		

		mov			dword ptr[pContainer],eax
	}
	return pContainer;

//	*ppmc = (MSG_CONTAINER*)(m_pBuffer+m_dwOffset);
//	m_dwOffset += (*ppmc)->GetContainerSize();

	
}

CMsgQue::~CMsgQue()
{
	// ���ͷ���
	
	if (m_pBuffer)
	{
		delete [] m_pBuffer;
		m_pBuffer = NULL;
	}
}


BOOL PushMsg(CMsgQue* pMsgQue,DWORD dwHeader,WSABUF* pBuf,DWORD dwBufNum,DWORD dwTotalMsgSize,DWORD dwFlag,DWORD* pdwMsgNum)
{
//	class CMsgQue

//	DWORD					m_dwMsgNum;				// �׿��ִ� �޽��� ����
//	char*					m_pBuffer;
//	DWORD					m_dwOffset;				// ���� �޽����� ���� �������� ��ġ
//	DWORD					m_dwBufferPosition;		// ���� ���� ����.
//	DWORD					m_dwMaxBufferSize;		// �ִ� ���� ������
//	CRITICAL_SECTION		m_csAlloc;

	char*	pEntry = NULL;
	char*	pDest = NULL;
	DWORD	dwSize = 0;
	BOOL	bResult;
	CRITICAL_SECTION* pcs;

	__asm
	{
		xor			ecx,ecx
		mov			dword ptr[bResult],ecx				; return value 

		mov			ebx,dword ptr[pMsgQue]
		mov			edx,SIZE_MSG_CONTAINER
		mov			eax,dword ptr[dwBufNum]		
		mul			edx

		add			eax,dword ptr[dwTotalMsgSize]		
		mov			dword ptr[dwSize],eax				; must alloc memory as this size


		;
		; enter critical section
		;
		mov			eax,offset g_csWrite				; critical section ptr
		mov			dword ptr[pcs],eax					; backup critical section ptr

		push		eax
		call		dword ptr[EnterCriticalSection]

		mov			eax,dword ptr[ebx+12]				; load m_dwBufferPosition;
		mov			esi,dword ptr[dwSize]				; need memory bytes
		mov			ecx,eax								; backup m_dwBufferPosition
		add			eax,esi

		cmp			eax,dword ptr[ebx+16]				; m_dwBufferPosition + dwSize >= m_dwMaxBufferSize
		jl			lb_push

		mov			eax,dword ptr[pcs]					; restore critical section ptr
		push		eax
		call		dword ptr[LeaveCriticalSection]
		jmp			lb_return


lb_push:
		mov			dword ptr[ebx+12],eax				; write m_dwBufferPosition

		mov			edx,dword ptr[ebx+4]				; m_pBuffer
		
		add			edx,ecx								; pEntry = m_pBuffer + m_dwBufferPosition
		mov			dword ptr[pEntry],edx				;

		mov			edi,dword ptr[pdwMsgNum]			; load will be writed dwMsgNum
		mov			edx,dword ptr[dwBufNum]				; m_dwMsgNum += dwBufNum;
		add			edx,dword ptr[ebx]
		mov			dword ptr[edi],edx
		mov			dword ptr[ebx],edx					;
		

		;
		; leave critical section
		;
		mov			eax,dword ptr[pcs]					; restore critical section ptr
		push		eax
		call		dword ptr[LeaveCriticalSection]

		;
		; write msg que
		;
//		DWORD						dwHeader;			// 8��Ʈ�� ���� �������,24��Ʈ�� �ε��� .handle�� ����.
//		DWORD						dwSize;				// �޽��� �����̳� ������
//		DWORD						dwMsgLength;		// �޽��� ����
//		DWORD						dwReserved;			// ���������� ������ ���ܵ� 4����Ʈ.���⼱ udp���� ����Ѵ�.

		mov			edi,dword ptr[pEntry]				; will be copied msg to this ptr
		mov			eax,dword ptr[pBuf]					; src WSABUF ptr
		movd		mm1,dword ptr[dwHeader]				; load header
		mov			edx,dword ptr[dwBufNum]				; loa dwBufNum
	
lb_loop:

//		struct WSABUF
//	    u_long      len;
//	    char FAR *  buf;
		mov			ecx,dword ptr[dwFlag]
		mov			dword ptr[edi+12],ecx
		
		movd		dword ptr[edi],mm1					; write header
		mov			dword ptr[edi+4],SIZE_MSG_CONTAINER	; write msg container's size
		mov			dword ptr[edi+8],0					; wirte msg length

		or			eax,eax
		jz			lb_return_true						; in this case DISCONNECT event

		mov			esi,dword ptr[eax+4]				; msg src ptr
		mov			ecx,dword ptr[eax]					; msg length
		
		mov			dword ptr[edi+8],ecx				; wirte msg length
		

		add			edi,SIZE_MSG_CONTAINER
		;
		; wirte msg
		;
		; esi = src , edi = dest, ecx = length
			
		or			ecx,ecx
		jz			lb_cpy_end

		test		ecx,1
		jz			lb_cpy_2
		movsb
		dec			ecx
		jz			lb_cpy_end

lb_cpy_2:
		test		ecx,2
		jz			lb_cpy_4
		movsw
		sub			ecx,2
		jz			lb_cpy_end
		
lb_cpy_4:
		shr			ecx,2
		rep			movsd
		
lb_cpy_end:
		add			eax,8								; src WSABUF ptr offset move
		dec			edx									; dwBufNum--
		jnz			lb_loop

		emms

lb_return_true:
		inc			dword ptr[bResult]

	}
lb_return:
	return bResult;

}

BOOL CMsgQue::PushMsg(DWORD dwHeader,WSABUF* pBuf,DWORD dwBufNum,DWORD dwTotalMsgSize,DWORD dwFlag,DWORD* pdwMsgNum)
{
	char*	pEntry = NULL;
	char*	pDest = NULL;
	DWORD	dwSize = 0;
		
	// �� �����ø� ��� ���� �ִ¸�ŭ�� �޸𸮸� �Ҵ� �޴´�.���� �޸� ���
	DWORD size = SIZE_MSG_CONTAINER*dwBufNum + dwTotalMsgSize;
	

	EnterCriticalSection(&g_csWrite);

	pEntry = m_pBuffer + m_dwBufferPosition;

	if (m_dwBufferPosition + size > m_dwMaxBufferSize)
	{
		LeaveCriticalSection(&g_csWrite);
		return FALSE;
	}

	m_dwBufferPosition += size;
	m_dwMsgNum += dwBufNum;
	*pdwMsgNum = m_dwMsgNum;

	LeaveCriticalSection(&g_csWrite);

	pDest = pEntry;
	for (DWORD i=0; i<dwBufNum; i++)
	{
		((MSG_CONTAINER*)pDest)->m_dwSize = 16;
		((MSG_CONTAINER*)pDest)->m_dwHeader.m_dwHeader = dwHeader;
		((MSG_CONTAINER*)pDest)->m_dwMsgLength = (pBuf+i)->len;
		memcpy(((MSG_CONTAINER*)pDest)->m_pMsg,(pBuf+i)->buf,(pBuf+i)->len);
		pDest += (SIZE_MSG_CONTAINER + (pBuf+i)->len);
	}
	return TRUE;
}

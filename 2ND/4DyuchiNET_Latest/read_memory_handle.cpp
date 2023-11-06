#include "read_memory_handle.h"

CReadMemoryHandle::CReadMemoryHandle()
{
	m_dwMaxMemorySize = 0;
	m_pMemory = NULL;
	m_dwMemoryIndex = -1;
	m_pWritePtr = NULL;
	m_pReadPtr = NULL;
	m_dwReservedBytes = 0;
	m_dwUsageBytes = 0;
}

void CReadMemoryHandle::InitializeMemory()
{
	m_dwReserved = 0;
	m_pWritePtr = m_pReadPtr = m_pMemory;
	m_dwReservedBytes = m_dwMaxMemorySize;
	m_dwUsageBytes = 0;
}

void CReadMemoryHandle::OnRead(DWORD dwSize,WSABUF* pBuf,DWORD dwBufNum,DWORD* pdwTotalMsgBytes,DWORD* pdwTotalMsgNum)
{
//	WSABUF 
//	char*	buf;
//	int		len
		
		
//	void*					m_pMemory;
//	DWORD					m_dwMemoryIndex;
//	char*					m_pWritePtr;
//	char*					m_pReadPtr;
//	DWORD					m_dwReservedBytes;
//  DWORD					m_dwUsageBytes	
	DWORD const64[2] = {0x00000001,00000000};

	__asm
	{
		pxor			mm0,mm0								; ���� �޽��� ī��Ʈ�� ��������
		movq			mm1,dword ptr[const64]				; ��� �ε�
		movd			mm2,dword ptr[pBuf]					; WSABUF ptr

		mov				ebx,dword ptr[this]				
		mov				ecx,dword ptr[dwSize]
		add				dword ptr[ebx+8],ecx				; write�� �����͸� �ڷ� �̵�.
		xor				edx,edx
		sub				dword ptr[ebx+16],ecx				; ���� ����Ʈ.
		xor				eax,eax
		add				dword ptr[ebx+20],ecx				; ���� ����Ʈ.
		mov				edx,MSG_LENGTH_SIZE					; ��� edx = 2 


lb_dispatch:
		; dispatch msg
		mov				ecx,dword ptr[dwBufNum]				; WSABUF�迭�� ����
		or				ecx,ecx
		jz				lb_end

		mov				ecx,dword ptr[ebx+20]				; ���� ����Ʈ �ε�
		cmp				ecx,edx								; ���� 2����Ʈ��ŭ�� ���۰� ���� �ʾ����� 

		jbe				lb_end

lb_read_length:
		sub				ecx,edx								; m_dwUsageBytes -= 2
		mov				esi,dword ptr[ebx+12]				; �о���� ������
		movzx			edi,word ptr[esi]					; ���̸� �д´�
		
		cmp				ecx,edi								; m_dwUsageBytes - 2 < length : return 
		jb				lb_end
		
		add				esi,edx								; ���� 2����Ʈ�� �о����Ƿ� ������+=2
		shl				edx,2								; ��� edx = 8
lb_read_msg:
		
		mov				ecx,edi								; �о���� ����Ʈ ��
		movd			edi,mm2								; ���̿� �����͸� ���� WSABUF ptr

		mov				dword ptr[edi],ecx					; dest ���ۿ� ���̸� �ִ´�.
		mov				dword ptr[edi+4],esi				; dest���ۿ� �޽����� �����͸� �ִ´�.

		add				esi,ecx								; read ptr += len
		add				edi,edx								; dest ����+=8
		movd			mm2,edi

		
		shr				edx,2								; ��� edx = 2
		add				eax,ecx								; ���⼭ �о���� �޽����� �� ����.������Ų��.
		add				ecx,edx								; (�޽��� ���� +-2)	
		sub				dword ptr[ebx+20],ecx				; ���� ����Ʈ -= (�޽��� ���� +-2)
		mov				dword ptr[ebx+12],esi				; read ptr �缳��

		paddd			mm0,mm1								; ���� �޽��� ���� ī��Ʈ
		dec				dword ptr[dwBufNum]					; ��� ������ WSABUF�迭 ��ī��Ʈ
		jmp				lb_dispatch



lb_end:
		mov				edi,dword ptr[pdwTotalMsgBytes]		; dispatch�� �޽��� �� ����Ʈ��(���� 2����Ʈ�� ����)
		mov				dword ptr[edi],eax
		mov				esi,dword ptr[pdwTotalMsgNum]		; dispatch�� �޽��� ���� 
		movd			dword ptr[esi],mm0
		emms

	}
}

void CReadMemoryHandle::OnReadC(DWORD dwSize,WSABUF* pBuf,DWORD dwBufNum,DWORD* pdwTotalMsgBytes,DWORD* pdwTotalMsgNum)
{	
//	void*					m_pMemory;
//	DWORD					m_dwMemoryIndex;
//	char*					m_pWritePtr;
//	char*					m_pReadPtr;
//	DWORD					m_dwReservedBytes;
//  DWORD					m_dwUsageBytes	

	m_dwReserved += dwSize;
	m_pWritePtr += dwSize;
	m_dwUsageBytes += dwSize;
	m_dwReservedBytes -= dwSize;
	DWORD dwLength = 0;
	DWORD count = 0;
	DWORD size = 0;

	*pdwTotalMsgBytes = 0;
	*pdwTotalMsgNum = 0;


lb_loop:

	if (m_dwUsageBytes < 3)
		return;
	
	
	dwLength = (DWORD)(*(WORD*)m_pReadPtr);

	if (m_dwUsageBytes - 2 < dwLength )
		return;
	
	m_pReadPtr += 2;
	
	pBuf[count].buf = m_pReadPtr;
	pBuf[count].len = dwLength;

	m_pReadPtr += dwLength;

	(*pdwTotalMsgBytes) += dwLength;
	(*pdwTotalMsgNum)++;
	m_dwUsageBytes -= (2 + dwLength);



	count++;

	goto lb_loop;

}
	

void CReadMemoryHandle::ResetMemory()
{
	if (m_dwReservedBytes < 100)
	{
		m_dwReservedBytes = m_dwMaxMemorySize-m_dwUsageBytes;
		memcpy(m_pMemory,m_pReadPtr,m_dwUsageBytes);
		m_pReadPtr = m_pMemory;
		m_pWritePtr = m_pReadPtr+m_dwUsageBytes;
	}
}

CReadMemoryHandle::~CReadMemoryHandle()
{

}
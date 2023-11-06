#pragma once

#include "overlapped.h"

class COverlappedSend : public COverlapped
{

public:
	CWriteMemoryHandle	m_mhWrite;		// OFFSET 48
	CRITICAL_SECTION	m_csSend;		

	void				Initialize();
	DWORD				GetReservedBytes() {return m_mhWrite.GetReservedBytes();}
	DWORD				GetUsageBytes()	{return m_mhWrite.GetUsageBytes();}	
	DWORD				GetMaxBufferSize()	{return m_mhWrite.GetMaxMemorySize();}
	void				LockSend() {EnterCriticalSection(&m_csSend);}		// send�Ҷ� lock
	void				UnLockSend() {LeaveCriticalSection(&m_csSend);}	// send�ϰ� ���� unlock
	BOOL				AllocBuffer(DWORD size);						// send,recv���� ����� ����.	
	BOOL				PushMsg(char* msg,DWORD dwLength)	{return m_mhWrite.Push(msg,dwLength);}	// �޽����� �����ϱ� ���� ���ۿ� ����.
	void				OnWrite(DWORD dwLength)	{m_mhWrite.OnWrite(dwLength);}		// �ѹ���Ʈ�� ���۵Ǹ� �ڵ� ȣ��Ǵ� �Լ�
	BOOL				SendBuffer();
	void				Release();		
	COverlappedSend();
	~COverlappedSend();
};


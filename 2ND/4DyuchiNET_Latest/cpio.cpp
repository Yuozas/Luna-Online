//
//	�޽����� �ۼ����ϴ� Completion I/O�� �ڵ尡 ��� ����ִ� �ҽ� �ڵ��̴�.
//	�ּ����� ������� ���� ������ �ڵ�� ��� ���� ����ִ�.
//  accept,send,recv,disconnect �̺�Ʈ�� üũ�� ��� ���⼭ �̷������.
//

//#define _WIN32_WINNT 0x0500

#include "cpio.h"
#include "quemanager.h"
#include "network.h"
#include "../4DyuchiNET_Common/net_define.h"
#include "overlapped_udp.h"
#include "net_global.h"
#include "switch_que.h"
#include "log.h"
#include <process.h>

HANDLE g_hCompletionPort = NULL;	// ���ø��� ��Ʈ�� �ڵ�

DWORD dwWorkThreadNum = 0;			// recv,sendó���� ����� ������ ������ ����.worker������� �θ���.

//HANDLE	g_hEndEvent[MAX_WORKER_THREAD_NUM];// worker�����尡 ����Ǿ����� �˸��� �̺�Ʈ �ڵ�.
HANDLE	g_hWorkerThread[MAX_WORKER_THREAD_NUM];
WSABUF*	g_pWsaBuf[MAX_WORKER_THREAD_NUM];

UINT WINAPI ServerWorkerThread(LPVOID lpVoid);	// worker������
UINT WINAPI ServerWorkerThreadLog(LPVOID lpVoid);


DWORD	GetWorkerThreadNum()
{
	return dwWorkThreadNum;
}

//
// ���� api�� �ʱ�ȭ�ϴ� �Լ�.
//
void InitSocket()
{	//
	// ���� ���� 2.2�� �����Ѵ�.������� �ֽ� �����̴�.
	//
	WORD Version;
	WSADATA wsaData;
	
	WSACleanup();
	Version = MAKEWORD(2, 2);
	WSAStartup(Version, &wsaData);

}
//
// ���ø��� ��Ʈ�� �����ϰ� �ʱ�ȭ�Ѵ�.
//
BOOL InitCompletionIO(DWORD size,HANDLE hMsgHandle)
{
	memset(g_pWsaBuf,0,sizeof(WSABUF*)*MAX_WORKER_THREAD_NUM);

	//
	// ���ø��� ��Ʈ�� �ʱ�ȭ �ϰ�, ���ø��� ��Ʈ�� �̺�Ʈ�� ���� ����� �����带 �����Ѵ�.
	// ù��° ���ڴ� ���� ������� worker������ ��� �＼���ϰ� �� ���� �޽��� ť�� �������̴�.
	// �ι�° ���ڴ� �޽����� ���������� ���� �����忡�� �˷��� �̺�Ʈ �ڵ��̴�.
	//

	// ���� �޽��� ť�� �ΰ��� ���������.�ּ��� �ΰ� �̻��� �����尡 ���� �۵��ϰ� �ϱ� ���� �޽����� �����ؼ� 
	// ť�� push�ϴ� ���� ���� �����尡 �� �ٸ� ť�� ���� �޽����� ó���Ѵ�.
	// ���� ť�� writable queue�� readable queue�� ������.�� ť���� �����ϰ� switch�� �� �ִ� ����� 
	// �����ϴ� ���� CQueManager�̴�.
	g_pQueManager = new CQueManager;
	g_pQueManager->Initialize(size,hMsgHandle);


	// ���� ���� �ʱ�ȭ
	SYSTEM_INFO SystemInfo;
	UINT ThreadID = 0;
	
	// ���ø��� ��Ʈ ����.
	if ((g_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)) == NULL)
	{
		MessageBox(GetActiveWindow(),"CreateIoCompletionPort failed with error","Error",MB_OK);
		return FALSE;
	}
	
	// cpu������ ��´�.
	GetSystemInfo(&SystemInfo);
	for (DWORD i = 0; i < SystemInfo.dwNumberOfProcessors*2; i++)
	{
		g_pWsaBuf[i] = new WSABUF[MAX_DETECTABLE_MSG_NUM];
		memset(g_pWsaBuf[i],0,sizeof(WSABUF)*MAX_DETECTABLE_MSG_NUM);

		if (g_pLogManager)
			g_hWorkerThread[i] = (HANDLE)_beginthreadex(NULL,NULL,ServerWorkerThreadLog,(LPVOID)i,NULL,&ThreadID);
		else 
			g_hWorkerThread[i] = (HANDLE)_beginthreadex(NULL,NULL,ServerWorkerThread,(LPVOID)i,NULL,&ThreadID);


		if (0xffffffff == (DWORD)g_hWorkerThread[i])
		{
			MessageBox(NULL,"Fail to Create WorkerThread","Error",MB_OK);
			return FALSE;
		}
		dwWorkThreadNum++;
	}
	StartSwitchQueThread(hMsgHandle,dwWorkThreadNum);
	
	return TRUE;
}

//
// �� ������ worker������.�޽����� �����ϰ� ���� send���� ���ϰ� ���ۿ� �����ִ� �޽����� ���� ������.
//
DWORD endcount = 0;

UINT WINAPI ServerWorkerThread(LPVOID lpVoid)
{
	DWORD			dwBufNum = MAX_DETECTABLE_MSG_NUM;
	
	// ���� ����� �������� �� �������� ���ÿ� ����� �������̹Ƿ� thread safe�ϴ�.
	DWORD			dwTotalMsgNum = 0;
	DWORD			dwTotalMsgBytes = 0;
	DWORD			dwWorkIndex = (DWORD)lpVoid;	// �� ������ ������ �ε���.���ڷ� �Ѱ��ش�.
	WSABUF*			pBuf = g_pWsaBuf[dwWorkIndex];
	BOOL			bResult = FALSE;
	DWORD			BytesTransferred = 0;			// ���۵� ����Ʈ ����� ���� ����
	COverlapped*	pIO;							// Overlapped ������� ��û�� io�� ���� overlapped�����͸� �޾ƿ� ������
	CMsgQue*		pMsgQue = g_pQueManager->GetWritableQue();	// ���� �޽����� push�� �޽��� ť ������.������ ����Ʈ ������ ť�� �＼���Ѵ�.
	HEADER			dwHeader;
	DWORD			dwQueMsgNum = 0;

//	SOCKET			socket;							// Commented by chan78 at 2001/10/17


	while(TRUE)
  	{

		
		if (!GetQueuedCompletionStatus(g_hCompletionPort, &BytesTransferred,(LPDWORD)&dwHeader, (LPOVERLAPPED *)&pIO, INFINITE))
		{
			if (GetLastError() != ERROR_NETNAME_DELETED)
				goto seg_lp;
		}


		// GetQueuedCompletionStatus()�Լ��� �����ϴ� ���� ���ø��� ��Ʈ�� io��û�� �Ϸ�� ���� üũ�߱� ������
		// �����带 ������̴�.���� �� ������� suspend���·� ���ư������� ��� �߻��� �̺�Ʈ�� ó���Ѵ�.
		// ���⼭ ó���� �̺�Ʈ�� send,recv,disconnect�� ���� ���̴�.disconnect�� �� ��찡 �ٸ����� �� ��쵵 
		// Ư�� �޽����� �����Ѱ����� �����Ͽ� �޽��� ť�� �������̴�.�׷��� �ϹǷμ� disconnect���ǵ���ȭ ������ ���� 
		// �� �ִ�.
		
		if (dwHeader.GetActionType() == ACTION_TYPE_DESTROY)	// �� ���� ���α׷��� �����϶�� �޽����̴�.
			goto seg_destroy;									// ���� �ڵ�� ����.

		if (!BytesTransferred || dwHeader.GetActionType() == ACTION_TYPE_DISCONNECT )
		{
			dwHeader.SetActionType(ACTION_TYPE_DISCONNECT);
			 

			EnterCriticalSection(g_csSwitchQue+dwWorkIndex);
	
			bResult = PushMsg(
				g_pQueManager->GetWritableQue(),
				dwHeader.m_dwHeader,
				NULL,
				1,
				0,
				0,
				&dwQueMsgNum
				);
			
			LeaveCriticalSection(g_csSwitchQue+dwWorkIndex);
			if (dwQueMsgNum)
				SwitchQue();
			
			goto seg_lp;
		}

		if (!pIO)
			goto seg_lp;

		if (dwHeader.GetActionType() == ACTION_TYPE_TCP)
		{

			// tcp��Ŷ�� �����߰ų� ������ ������ WSASend()�� ���� ����� ���� ����̴�.
			// recv�ߴٸ� �ϼ��� �޽����� �����ϴ��� üũ�ؼ� �ϼ��� �޽��� ������ŭ �޽��� ť�� �ְ�
			// send�� ���� ������� �޾Ҵٸ� sendť���� ������ŭ�� �����Ѵ�.

			if (pIO->GetIOType() == IO_TYPE_READ)
			{			

				((COverlappedRecv*)pIO)->OnRead(BytesTransferred,pBuf,dwBufNum,&dwTotalMsgBytes,&dwTotalMsgNum);
					
				if (dwTotalMsgNum)
				{
					// ������� �޽��� ť�� ü���� �� �� ����.
					EnterCriticalSection(g_csSwitchQue+dwWorkIndex);
							
					bResult = PushMsg(g_pQueManager->GetWritableQue(),
						dwHeader.m_dwHeader,
						pBuf,
						dwTotalMsgNum,
						dwTotalMsgBytes,
						0,
						&dwQueMsgNum
						);

					LeaveCriticalSection(g_csSwitchQue+dwWorkIndex);
					
				}
				if (dwQueMsgNum)
					SwitchQue();

				((COverlappedRecv*)pIO)->ResetBuffer();
				((COverlappedRecv*)pIO)->PrepareRead();
				goto seg_lp;

			}
			else if (pIO->GetIOType() == IO_TYPE_WRITE)
			{
				// ������ ������ WSASend()�� ���� ����� ���� ���.
				// ���⼭ ��� ��û�� WSASend()�� ����� �����Ʈ�� �۽��ߴ��� �� �� �ִ�.
				
				((COverlappedSend*)pIO)->LockSend();	// �� ������ �ٸ� �����忡�� send�۾��� �ϰ� ������ �𸣹Ƿ� lock�� �Ǵ�.
				((COverlappedSend*)pIO)->OnWrite(BytesTransferred);	// ���� send�� ������ ����Ʈ ����ŭ ť���� ����.

				if (((COverlappedSend*)pIO)->GetUsageBytes())
					((COverlappedSend*)pIO)->SendBuffer();
					//SendBufferA((COverlappedSend*)pIO);
				else 
					pIO->SetIOType(IO_TYPE_WAIT);	// ������ ���� �����忡�� WSASend()�Լ��� �����ص� �ȴ�.
					
				((COverlappedSend*)pIO)->UnLockSend();
				goto seg_lp;
			}
		}
seg_lp:
		dwHeader.SetHeader(0);
	}
seg_destroy:
	_endthreadex(0);

	return 0;
}
UINT WINAPI ServerWorkerThreadLog(LPVOID lpVoid)
{
	DWORD			dwBufNum = MAX_DETECTABLE_MSG_NUM;
	
	// ���� ����� �������� �� �������� ���ÿ� ����� �������̹Ƿ� thread safe�ϴ�.
	DWORD			dwTotalMsgNum = 0;
	DWORD			dwTotalMsgBytes = 0;
	DWORD			dwWorkIndex = (DWORD)lpVoid;	// �� ������ ������ �ε���.���ڷ� �Ѱ��ش�.
	WSABUF*			pBuf = g_pWsaBuf[dwWorkIndex];
	BOOL			bResult = FALSE;
	DWORD			BytesTransferred = 0;			// ���۵� ����Ʈ ����� ���� ����
	COverlapped*	pIO;							// Overlapped ������� ��û�� io�� ���� overlapped�����͸� �޾ƿ� ������
	CMsgQue*		pMsgQue = g_pQueManager->GetWritableQue();	// ���� �޽����� push�� �޽��� ť ������.������ ����Ʈ ������ ť�� �＼���Ѵ�.
	HEADER			dwHeader;
	DWORD			dwQueMsgNum = 0;

//	BOOL			bCompletionResult;				// Commented by chan78 at 2001/10/17
//	SOCKET			socket;							// Commented by chan78 at 2001/10/17


	while(TRUE)
  	{

		
		if (!GetQueuedCompletionStatus(g_hCompletionPort, &BytesTransferred,(LPDWORD)&dwHeader, (LPOVERLAPPED *)&pIO, INFINITE))
		{
			if (GetLastError() != ERROR_NETNAME_DELETED)
				goto seg_lp;
		}

		// GetQueuedCompletionStatus()�Լ��� �����ϴ� ���� ���ø��� ��Ʈ�� io��û�� �Ϸ�� ���� üũ�߱� ������
		// �����带 ������̴�.���� �� ������� suspend���·� ���ư������� ��� �߻��� �̺�Ʈ�� ó���Ѵ�.
		// ���⼭ ó���� �̺�Ʈ�� send,recv,disconnect�� ���� ���̴�.disconnect�� �� ��찡 �ٸ����� �� ��쵵 
		// Ư�� �޽����� �����Ѱ����� �����Ͽ� �޽��� ť�� �������̴�.�׷��� �ϹǷμ� disconnect���ǵ���ȭ ������ ���� 
		// �� �ִ�.
		
		if (dwHeader.GetActionType() == ACTION_TYPE_DESTROY)	// �� ���� ���α׷��� �����϶�� �޽����̴�.
			goto seg_destroy;									// ���� �ڵ�� ����.

		if (!BytesTransferred || dwHeader.GetActionType() == ACTION_TYPE_DISCONNECT )
		{
			dwHeader.SetActionType(ACTION_TYPE_DISCONNECT);
			 

			EnterCriticalSection(g_csSwitchQue+dwWorkIndex);
	
			bResult = PushMsg(
				g_pQueManager->GetWritableQue(),
				dwHeader.m_dwHeader,
				NULL,
				1,
				0,
				0,
				&dwQueMsgNum
				);
			
			LeaveCriticalSection(g_csSwitchQue+dwWorkIndex);
			if (dwQueMsgNum)
				SwitchQue();
			
			goto seg_lp;
		}

		if (!pIO)
			goto seg_lp;

		if (dwHeader.GetActionType() == ACTION_TYPE_TCP)
		{

			// tcp��Ŷ�� �����߰ų� ������ ������ WSASend()�� ���� ����� ���� ����̴�.
			// recv�ߴٸ� �ϼ��� �޽����� �����ϴ��� üũ�ؼ� �ϼ��� �޽��� ������ŭ �޽��� ť�� �ְ�
			// send�� ���� ������� �޾Ҵٸ� sendť���� ������ŭ�� �����Ѵ�.

			if (pIO->GetIOType() == IO_TYPE_READ)
			{			
				((COverlappedRecv*)pIO)->OnRead(BytesTransferred,pBuf,dwBufNum,&dwTotalMsgBytes,&dwTotalMsgNum);
					
				if (dwTotalMsgNum)
				{
					// ������� �޽��� ť�� ü���� �� �� ����.
					EnterCriticalSection(g_csSwitchQue+dwWorkIndex);
							
					bResult = PushMsg(g_pQueManager->GetWritableQue(),
						dwHeader.m_dwHeader,
						pBuf,
						dwTotalMsgNum,
						dwTotalMsgBytes,
						0,
						&dwQueMsgNum
						);

					LeaveCriticalSection(g_csSwitchQue+dwWorkIndex);
					
					if (!bResult)
					{
						g_pLogManager->WriteLog("MainMsgQue Overflow",lstrlen("MainMsgQue Overflow"));
					}

					
				}
				if (dwQueMsgNum)
					SwitchQue();

				((COverlappedRecv*)pIO)->ResetBuffer();
				((COverlappedRecv*)pIO)->PrepareRead();
				goto seg_lp;

			}
			else if (pIO->GetIOType() == IO_TYPE_WRITE)
			{
				// ������ ������ WSASend()�� ���� ����� ���� ���.
				// ���⼭ ��� ��û�� WSASend()�� ����� �����Ʈ�� �۽��ߴ��� �� �� �ִ�.
				
				((COverlappedSend*)pIO)->LockSend();	// �� ������ �ٸ� �����忡�� send�۾��� �ϰ� ������ �𸣹Ƿ� lock�� �Ǵ�.
				((COverlappedSend*)pIO)->OnWrite(BytesTransferred);	// ���� send�� ������ ����Ʈ ����ŭ ť���� ����.

				if (((COverlappedSend*)pIO)->GetUsageBytes())
					((COverlappedSend*)pIO)->SendBuffer();
					//SendBufferA((COverlappedSend*)pIO);
				else 
					pIO->SetIOType(IO_TYPE_WAIT);	// ������ ���� �����忡�� WSASend()�Լ��� �����ص� �ȴ�.
					
				((COverlappedSend*)pIO)->UnLockSend();
				goto seg_lp;
			}
		}
seg_lp:
		dwHeader.SetHeader(0);
	}
seg_destroy:
	_endthreadex(0);

	return 0;
}

//
// ���ø��� ��Ʈ�� �����ϰ� ��Ÿ �۷ι� ���ҽ��� ����.
//
// Modified by chan78 at 2002/07/22
void CloseComplitionIO()
{

	// ���ø��� ��Ʈ�� �����Ѵ�.���� ���� Ŀ�� ������Ʈ �ڵ��� �ݴ´�.
	// �����嵵 ���⼭ �����Ų��.��� �����尡 ���� ����ɶ����� ����Ѵ�.
	if (g_hCompletionPort)
	{
		// worker�����带 �����Ų��.
		for (DWORD i=0; i<dwWorkThreadNum; i++)
			PostQueuedCompletionStatus(g_hCompletionPort, 0,ACTION_TYPE_DESTROY, NULL);

		WaitForMultipleObjects( dwWorkThreadNum, g_hWorkerThread, TRUE, INFINITE );

		
		for (DWORD i=0; i<dwWorkThreadNum; i++)
		{
			delete [] g_pWsaBuf[i];
		}

		for (DWORD i=0; i<dwWorkThreadNum; i++)
		{
			CloseHandle( g_hWorkerThread[i] );
			g_hWorkerThread[i] = NULL;
		}

		// ���ø��� ��Ʈ�� �ڵ��� �ݴ´�.
		CloseHandle(g_hCompletionPort);
		g_hCompletionPort = NULL;
	}


//	MessageBox(NULL,"Work Thread Deleted","Report",MB_OK);
}

void CleanSocket()
{
	// ���� api Ŭ����
	WSACleanup();

}

void PostDisconnectEvent(DWORD dwHandle)
{
	HEADER	dwHeader;
	dwHeader.SetHeader(dwHandle);
	dwHeader.SetActionType(ACTION_TYPE_DISCONNECT);

	PostQueuedCompletionStatus(g_hCompletionPort,0,dwHeader.m_dwHeader, NULL);
}


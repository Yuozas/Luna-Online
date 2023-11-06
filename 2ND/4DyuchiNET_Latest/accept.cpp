#include "accept.h"
#include "network.h"
#include <process.h>
#include "cpio.h"
#include "pre_connect.h"
#include "mainthread.h"

UINT WINAPI AcceptThread(LPVOID lpVoid);

//
// accept�����带 �����ϴ� �Լ�.
//
// Modified by chan78 at 2002/07/22
BOOL StartAcceptThread( CNetwork* pNetwork )
{
	UINT ThreadID = 0;

	HANDLE hAcceptHandle = (HANDLE)_beginthreadex(NULL,NULL,AcceptThread,(LPVOID)pNetwork,NULL,&ThreadID);

	if (0xffffffff == (DWORD)hAcceptHandle)
	{
		MessageBox(GetActiveWindow(),"Fail to Create Accept Thread","Error",MB_OK);
		pNetwork->SetAcceptTHreadHandle( NULL );
		return FALSE;
	}
	SetThreadPriority( hAcceptHandle,THREAD_PRIORITY_HIGHEST );
	pNetwork->SetAcceptTHreadHandle( hAcceptHandle );
	
	return TRUE;
}

// Modified by chan78 at 2002/07/22
BOOL EndAcceptThread( SOCKET socketListen, HANDLE hAcceptThread )
{
	if (socketListen == INVALID_SOCKET)
		return FALSE;
	
	closesocket( socketListen );
	WaitForSingleObject( hAcceptThread, INFINITE );
	CloseHandle( hAcceptThread );

	return TRUE;
}

UINT WINAPI AcceptThread(LPVOID lpVoid)
{
	// ������ �޴� �������̴�.
	// ������ ������ ���ڷ� ���� ��Ʈ��ũ Ŭ������ OnConnect()�Լ��� �ڵ� �����Ѵ�.
	SOCKET accept = INVALID_SOCKET;

	CNetwork* pNetwork = (CNetwork*)lpVoid;	// ���ڷ� ��Ʈ�� Ŭ������ �����͸� �޴´�.
	DWORD		dwAcceptInterval = pNetwork->GetAcceptInterval();
	
	CConnection* con = NULL;
	DWORD		index = 0;
	sockaddr_in	addr;
	int			len = sizeof(addr);
	memset(&addr,0,len);
	CONNECT_ITEM item;
	
	if (dwAcceptInterval > 1000)
		dwAcceptInterval = 1000;
	while (TRUE)
	{
		// accept�� �ɾ���´�.���⼭ accept��û�� ���ö����� �� ������� ���ȴ�.
		accept = WSAAccept(pNetwork->GetListenSocket(),
			(sockaddr*)&addr,
			&len,
			NULL,
			0);

		Sleep(dwAcceptInterval);
		
		if (accept == INVALID_SOCKET)
		{
			goto seg_destroy;
		}

		
		// ������ �ް� �־��� ��Ʈ�� Ŭ������ ����Ѵ�.
		// ���ν����忡 ó���� ���ӿ䱸�� ������ �뺸.
		item.socket = accept;
		item.pNetwork = pNetwork;
		item.SuccessConnect = NULL;
		item.FailConnect = NULL;
		g_pPreConnectPool->InsertPreConnect(&item);
		ProcessPreConnectedItem();
	}
seg_destroy:
	_endthreadex(0);

	return 0;
}

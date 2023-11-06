// FtpClientSocket.h: interface for the CFtpClientSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FTPCLIENTSOCKET_H__DCEA296E_4D74_4068_94CB_36B92167398B__INCLUDED_)
#define AFX_FTPCLIENTSOCKET_H__DCEA296E_4D74_4068_94CB_36B92167398B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CMD_BUF_SIZE 2047
#define MAX_HOSTNAME_SIZE 256
#define TRANSFER_BUFFER_SIZE 2048

#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif

#ifdef __UNIX__
typedef int SOCKET;
//�ᱹ INVALID_SOCKET ==> -1�� ���� ��Ʈ ������ ������.
#define INVALID_SOCKET (SOCKET)(~0)
#endif

const static char LOCAL_NAME[] = {"Local"};
const static char LINE_END_CHAR[] = {"\r\n"};
const static int LINE_END_CR = '\r';
const static int LINE_END_LF = '\n';

//return�� ����
const static int RETURN_FAIL = -1;
const static int RETURN_GOOD = 0;

//recv�� send �� �󸶳� �ٽ� ��õ� �� ���ΰ��� �����Ѵ�.
const static int MAX_SOCKET_BUFFER = 1024;
const static int SOCKET_TIME_OUT = 5;
//���Ͽ� �ΰ� �̻��� code���� ���� ��츦 ����ؼ�
//���� �����͸� �� ��Ƴ��Ҵٰ� ��� code�� ���Դ��� �˻��Ѵ�.
const static int MAX_CODE_CHECK_BUFFER = MAX_SOCKET_BUFFER*3;

//Code �˻翡 �ʿ��� ����.
const static int CODE_LENGTH = 4;
const static int MAX_LINE_LENGTH = 2048;
const static int MAX_COMMAND_BUFFER = 256;

const static int PORT_INFO_LENGTH = 6;
const static int PORT_MAKE_KEY = 256;
const static int PORT_NUMBER_START_INDEX = 4;
const static int PORT_NUMBER_ADD_INDEX = 5;

#include "StdAfx.h"	// Added by ClassView

typedef vector<char*> VEC_COMMAND_RESULT;
typedef VEC_COMMAND_RESULT::iterator ITR_COMMAND_RESULT_VEC;

class CFtpClientSocket
{
public:
	CFtpClientSocket();
	virtual ~CFtpClientSocket();

protected:
	int ConnectFtp(const char* pszServerName, unsigned int uiPort/*, BOOL bReadGreeting=TRUE*/);
	int SendRawData(char* pszCommand, int iLength, SOCKET socketId=-1); //iLength�� ���Ṯ�� ������ ������.
	int RecvRawData(char* pszBuffer, int iLength, int iWaitData=0, SOCKET socketId=-1); //iLength�� ���Ṯ�� ������ ������.

	int InitFtpSocket();
	void InitMember();

	VEC_COMMAND_RESULT m_vectorCommand;

	char m_szPassword[64];
	char m_szUserId[64];
	unsigned int m_uiServerPort;
	char m_szServerName[64];
	
	struct hostent* m_pHostInfo; //host ������ ã�Ƽ� �����Ѵ�.
	char m_szTransferBuffer[TRANSFER_BUFFER_SIZE]; //Data ���ۿ� ���Ǵ� ����..
	char m_szHostName[MAX_HOSTNAME_SIZE]; //ȣ��Ʈ�̸��� �����ϴ� ����..
	char m_szCommandBuffer[CMD_BUF_SIZE+1]; //Command�� �����ϴ� ����..
	SOCKET m_socketId; //�ش� ���� ID
private:
	void CloseFtp();
	BOOL m_bStartSocket;
};

#endif // !defined(AFX_FTPCLIENTSOCKET_H__DCEA296E_4D74_4068_94CB_36B92167398B__INCLUDED_)

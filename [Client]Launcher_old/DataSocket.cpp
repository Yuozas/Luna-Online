// DataSocket.cpp: implementation of the CDataSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "DataSocket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataSocket::CDataSocket()
{

}

CDataSocket::~CDataSocket()
{
	printf("!!!!!!!!!!!! Destroy DataSocket!!!!\n");
}

int CDataSocket::ConnectFtp(char *pszServerName, unsigned int uiPort)
{
	return CFtpClientSocket::ConnectFtp(pszServerName, uiPort);
}

int CDataSocket::RecvInformationData(char *pszBuffer, int iLength)
{
	//�ڿ� 1�ʸ� �ִ� ���� ������ �� ����...... ===> ���. FILE SIZE�� CHECK�� �� ����.
	return RecvRawData(pszBuffer, iLength/*, 1*/);
}

int CDataSocket::SendInformationData(char *pszBuffer, int iLength)
{
	return SendRawData(pszBuffer, iLength);
}


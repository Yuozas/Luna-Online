// CommandSocket.h: interface for the CCommandSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMANDSOCKET_H__AF25C9EB_C199_4708_8887_0684A67CDD6B__INCLUDED_)
#define AFX_COMMANDSOCKET_H__AF25C9EB_C199_4708_8887_0684A67CDD6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FtpClientSocket.h"
#include "DataSocket.h"

/*
#ifdef __UNIX__
const static char FTPLOG_PATH_NAME[] = {"./ctmp4log/"};
#else
const static char FTPLOG_PATH_NAME[] = {".\\ctmp4log\\"};
#endif
*/

class CCommandSocket : public CFtpClientSocket  
{
public:
	void SetResponseTimeOut(int iWaitLimit);
	int RemoteRenameFile(char* pszSourceFileName, char* pszDestFileName);
	int RemoteDeleteFile(char* pszFileName);
	int ConnectFtp(const char* pszServerName, unsigned int uiPort); //����..
	int LogIn(const char* pszUserId, const char* pszPassword); //�α���..
	int GetRemotePwd(char* pszRemotePwd); //���� ���� ��θ� �о��..
	int ProcedureConnectDataSocket(char* pszCommand, CDataSocket* pDataSocket); //DataSocket�� ������.
	int ConfirmEndTransfer(); //DataSocket���� �����͸� �� ���´ٴ� ���� Ȯ����.
	int GetRemoteFileSize(char* pszFileName); //������ ���� ����� �о��.
	int SetTransferType(BOOL bBin=FALSE); //���� Type�� ������.
	int RemoteChangeDirectory(char* pszPath); //������ �۾������� ������..

	CCommandSocket();
	virtual ~CCommandSocket();

private:
	int m_iResponseTimeOut;
	void WriteFtpCommandLog(const char *pszFormat, ...);
	void CloseConnection();
	int GetCommandCodeFromServer(int* pIntCode, char* pszCodeString=NULL);
	int SendCommandData(char* pszCommand); //0�̻��� ���, ������.
	int GetPortFotDataConnection(BOOL bPassiveMode=TRUE); //data Socket�� Port�� ������.
	int InsertEachCommand(char* pszCommandsBuffer);
	int RecvCommandResultData(); //0�̻��� ���, ������.
	BOOL GetLastLineInRecv(char* pszSource, char* pszLast); //���������� �߿� ������ ������ ��ȯ�Ѵ�.
	BOOL IsRecvLastLine(char* pszLine);
	BOOL IsRecvLineComplete(char *pszLine, int iSize); //iSize�� ���Ṯ�ڸ� ������ ���̸� �Է��Ѵ�.
};

#endif // !defined(AFX_COMMANDSOCKET_H__AF25C9EB_C199_4708_8887_0684A67CDD6B__INCLUDED_)

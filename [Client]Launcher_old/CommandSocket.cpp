// CommandSocket.cpp: implementation of the CCommandSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "CommandSocket.h"
#include "FtpFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommandSocket::CCommandSocket()
{
	/*
#ifdef __UNIX__
   mkdir(FTPLOG_PATH_NAME, S_IRWXU|S_IRWXG|S_IRWXO);
#else
   mkdir(FTPLOG_PATH_NAME);
#endif
   */
   m_iResponseTimeOut = 1;
}

CCommandSocket::~CCommandSocket()
{
	printf("!!!!!!!!!!!! Destroy CommandSocket!!!!\n");
	CloseConnection();
}

void CCommandSocket::CloseConnection()
{
	int iCode = 0;
	if(SendCommandData("QUIT")>=0)
		GetCommandCodeFromServer(&iCode);

//	//�糪�׽�Ʈ
//	FILE* pFile = fopen( "launcherdebug.txt", "at" );
//	if( pFile )
//	{
//		fprintf( pFile, "****After CloseConnection()!****\n" );
//		fclose(pFile);
//	}
//	//---------
}

int CCommandSocket::ConnectFtp(const char *pszServerName, unsigned int uiPort)
{
	int iConnect = CFtpClientSocket::ConnectFtp(pszServerName, uiPort);
	if(iConnect==RETURN_FAIL)
		return RETURN_FAIL;

	//�λ縻�� �о���δ�.
	int iReturnCode = -1;
	if(GetCommandCodeFromServer(&iReturnCode)<0)
		return RETURN_FAIL;

	if( iReturnCode != WELCOME_MESSAGE )
		printf("It is not welcom message => %d\n", iReturnCode);

	return iConnect;
}

int CCommandSocket::LogIn(const char *pszUserId, const char *pszPassword)
{
	char szCommand[MAX_COMMAND_BUFFER];
	//memset(szCommand, 0x00, sizeof(szCommand));

	sprintf(szCommand, "USER %s", pszUserId);
	if(SendCommandData(szCommand)<0)
	{
		printf("!!! Fail to Log In On Send..\n");
		return RETURN_FAIL;
	}

	int iCode = 0;
	if(GetCommandCodeFromServer(&iCode)<0)
	{
		printf("!!! Fail to Log In On Recv..\n");
		return RETURN_FAIL;		
	}

	//�н����� ���� �׳� �н���...
	if(iCode==LOGIN_SUCCESS)
		return RETURN_GOOD;

	if(iCode!=PASSWORD_REQUIRED)
		return RETURN_FAIL;

	//memset(szCommand, 0x00, sizeof(szCommand));
	sprintf(szCommand, "PASS %s", pszPassword);
	if(SendCommandData(szCommand)<0)
	{
		printf("!!! Fail to Log In PASS On Send..\n");
		return RETURN_FAIL;		
	}

	iCode = 0;
	if(GetCommandCodeFromServer(&iCode)<0)
	{
		printf("!!! Fail to Log In PASS On Recv..\n");
		return RETURN_FAIL;		
	}

	if(iCode!=LOGIN_SUCCESS)
	{
		printf("!!! Fail to Log In Error from SERVER..%d\n", iCode);
		return RETURN_FAIL;		
	}

	//���������س���.....
	memset(m_szUserId, 0x00, sizeof(m_szUserId));
	sprintf(m_szUserId, "%s", pszUserId);
	memset(m_szPassword, 0x00, sizeof(m_szPassword));
	sprintf(m_szPassword, "%s", pszPassword);

	return RETURN_GOOD;
}

int CCommandSocket::GetPortFotDataConnection(BOOL bPassiveMode/*=TRUE*/)
{
	char szCommand[MAX_COMMAND_BUFFER];
	memset(szCommand, 0x00, sizeof(szCommand));

	if(bPassiveMode==TRUE)
	{
		sprintf(szCommand, "PASV");
	}
	else
	{
		printf("!!! Support Only the 'PassiveMode'. Change the setting.\n");
		return RETURN_FAIL;
	}

	if(SendCommandData(szCommand)<0)
	{
		printf("!!! Fail to PASV On Send..\n");
		return RETURN_FAIL;
	}

	int iCode = 0;
	char szResultPort[MAX_COMMAND_BUFFER];
	memset(szResultPort, 0x00, sizeof(szResultPort));

	if(GetCommandCodeFromServer(&iCode, szResultPort)<0)
	{
		printf("!!! Fail to PASV On Recv..\n");
		return RETURN_FAIL;
	}

	if(ENTER_PASSIVE_MODE!=iCode)
	{
		printf("!!! Fail to Enter Passive Mode => error code %d\n", iCode);
		return RETURN_FAIL;
	}

	//�տ� return�� code�� ����..
	int iLen = strlen(szResultPort+CODE_LENGTH);
	for(int iIndex= 0; iIndex<iLen; iIndex++)
	{
		if(!isdigit(szResultPort[CODE_LENGTH+iIndex]))
			szResultPort[CODE_LENGTH+iIndex] = ' ';
	}
	
	unsigned int arrPortInfo[6];
	memset(arrPortInfo, 0x00, sizeof(arrPortInfo));
	if(PORT_INFO_LENGTH!=sscanf(szResultPort+CODE_LENGTH,
											"%u %u %u %u %u %u",
											arrPortInfo,
											arrPortInfo+1,
											arrPortInfo+2,
											arrPortInfo+3,
											arrPortInfo+4,
											arrPortInfo+5))
	{
		return RETURN_FAIL;
	}

	return (arrPortInfo[PORT_NUMBER_START_INDEX]*PORT_MAKE_KEY) + arrPortInfo[PORT_NUMBER_ADD_INDEX];
}

int CCommandSocket::SendCommandData(char* pszCommand)
{
	//strlne�� null���Ṯ�ڸ� �� ���̸� Return�Ѵ�.
	//����, �������+���Ṯ��(2����Ʈ)+���Ṯ��(1����Ʈ)
	char* pszLine = new char[strlen(pszCommand)+sizeof(LINE_END_CHAR)+1];
	memset(pszLine, 0x00, strlen(pszCommand)+sizeof(LINE_END_CHAR)+1);

	sprintf(pszLine, "%s%s", pszCommand, LINE_END_CHAR);

	//SendRawData�� ���Ṯ�ڸ� ����, �����͸� ������.
	//LINE_END_CHAR�� ������� ���Ṯ�ڰ� ���ԵǾ� �־ �����Ѵ�.
	//pszCommand�� ������� strlen�� ����ϸ�, ���Ṯ�� ������ ���̰� ���ϵȴ�.
	int iResult = SendRawData(pszLine, strlen(pszCommand)+(sizeof(LINE_END_CHAR)-1));

/*DEBUG*/
	if(iResult>0)
	{
		printf("S => %s", pszLine);
		WriteFtpCommandLog("S => %s", pszLine);
	}
/**/

	delete [] pszLine;
	pszLine = NULL;

	return iResult;
}

int CCommandSocket::RecvCommandResultData()
{
	//Socket���� ���� �о���� ������....
	char szDataBuffer[MAX_SOCKET_BUFFER];
	//������ ©�ȴ� Data�� �����ϰ� �ִ� �����..
	char szRemainDataBuffer[MAX_LINE_LENGTH];
	memset(szRemainDataBuffer, 0x00, sizeof(szRemainDataBuffer));
	//���Ͽ��� ���� ���� ��, �ϼ��� ���������� �����ϴ� �����..
	char szLastLine[MAX_LINE_LENGTH];

	int iTotRecvLen = 0;
	int iRecvLen = 0;

	char szCodeCheckBuffer[MAX_CODE_CHECK_BUFFER];
	memset(szCodeCheckBuffer, 0x00, sizeof(szCodeCheckBuffer));

	//����.. �����͸� �о�´�.
	while(1)
	{
		memset(szDataBuffer, 0x00, sizeof(szDataBuffer));
		iRecvLen = RecvRawData(szDataBuffer, sizeof(szDataBuffer)-1, m_iResponseTimeOut);
		if(iRecvLen<0)
			return iRecvLen; //Error Code

		//��ü ���� �����͸� ���Ѵ�....
		iTotRecvLen += iRecvLen;

/*DEBUG*/
//		if(iRecvLen>0)
//			printf("R => %s", szDataBuffer);
/**/

		//Data�� �޾�����, ������ �޾Ҵ� �ҿ����� �������� �ڿ����δ�.
		if(strlen(szRemainDataBuffer)>0)
			strcat(szRemainDataBuffer, szDataBuffer);
		else
			sprintf(szRemainDataBuffer, "%s", szDataBuffer);

		//���� szRemainDataBuffer�� ������ �۾��Ѵ�.

		//������ ������ Return�Ѵ�.
		memset(szLastLine, 0x00, sizeof(szLastLine));
		if(GetLastLineInRecv(szRemainDataBuffer, szLastLine)==FALSE)
			return 0; //szRemainDataBuffer �����Ͱ� ���� ��쿡��, FALSE��..
					  //=> ���� ���� �����Ͱ� ����.

		//�����ϱ� ���� �˻��Ѵ�.
		if((unsigned int)iTotRecvLen>sizeof(szCodeCheckBuffer))
		{
			printf("!!! too many data from server. Length=>%d\n", iTotRecvLen);
			printf("%s", szCodeCheckBuffer);
			printf("%s", szRemainDataBuffer);
			return RETURN_FAIL;
		}
		//���⼭ �޾Ƴ��� �����͸� ��ü ���ۿ� �����Ѵ�.
		if(strlen(szCodeCheckBuffer)>0)
			strcat(szCodeCheckBuffer, szRemainDataBuffer);
		else
			sprintf(szCodeCheckBuffer, "%s", szRemainDataBuffer);

		//���� ������ ���������� ���Դ��� �˻��Ѵ�.
		//������ ©������, ���� ������ �ִ� ������(szLastLine)�� �ڿ� ������ �����͸� �ٿ��� �Ѵ�.
		memset(szRemainDataBuffer, 0x00, sizeof(szRemainDataBuffer));
		if(IsRecvLineComplete(szLastLine, strlen(szLastLine))==FALSE)
		{
			sprintf(szRemainDataBuffer, "%s", szLastLine);
			continue;
		}

		//������ ������ ���� �˻��Ѵ�.
		if(IsRecvLastLine(szLastLine)==TRUE)
			break;
	}

	InsertEachCommand(szCodeCheckBuffer);

	return iTotRecvLen;
}

//1Line�� ©���� ������ ���,
//1Line�� �� ���߾ ������ ���,
//2Line�̻��� �ɷ��� ������ ���..
BOOL CCommandSocket::IsRecvLineComplete(char *pszLine, int iSize/*���Ṯ������.*/)
{
	if(iSize<=0)
		return FALSE;

	//������ �ι���Ʈ�� ���Ṯ������.......
	if((pszLine[iSize-2]==LINE_END_CR)&&(pszLine[iSize-1]==LINE_END_LF))
		return TRUE;

	return FALSE;
}

//1Line�� �Է¹ް�, ������ Line������ �˻��Ѵ�.
BOOL CCommandSocket::IsRecvLastLine(char *pszLine)
{
	if(strlen(pszLine)<CODE_LENGTH)
		return FALSE;

	if(pszLine[CODE_LENGTH-1]=='-')
		return FALSE;

	return TRUE;
}

//���������� �߿� ������ ������ ��ȯ�Ѵ�.
BOOL CCommandSocket::GetLastLineInRecv(char *pszSource, char *pszLast)
{
	if(strlen(pszSource)<=0)
		return FALSE;

	char* pToken = NULL;
	int iNextPointer = 0;
	int iLastPointer = 0;

	while((pToken = strstr(pszSource+iNextPointer, LINE_END_CHAR))!=NULL)
	{
		iLastPointer = iNextPointer;
		iNextPointer += (pToken-(pszSource+iNextPointer))+(sizeof(LINE_END_CHAR)-1);
	}
	
	sprintf(pszLast, "%s", pszSource+iNextPointer);
	if(strlen(pszLast)==0) //������ ���α��� �����ϰ� �����Ͱ� ���°��̴�.
		sprintf(pszLast, "%s", pszSource+iLastPointer);

	return TRUE;
}

int CCommandSocket::InsertEachCommand(char *pszCommandsBuffer)
{
	if(strlen(pszCommandsBuffer)<=0)
		return RETURN_FAIL;

	char szNowCommand[MAX_LINE_LENGTH]; //1���� �о �����ϴ� ����..
	memset(szNowCommand, 0x00, sizeof(szNowCommand));
	char szOneCommand[MAX_CODE_CHECK_BUFFER]; //���� Code�� ���� ����� ��Ƴ��� ����..
	memset(szOneCommand, 0x00, sizeof(szOneCommand));

	int iPreCode = 0;
	int iNowCode = 0;

	char* pToken = NULL;
	int iNextPointer = 0;
	int iLastPointer = 0;

	while((pToken = strstr(pszCommandsBuffer+iNextPointer, LINE_END_CHAR))!=NULL)
	{
		iLastPointer = iNextPointer;
		iNextPointer += 
			(pToken-(pszCommandsBuffer+iNextPointer))+(sizeof(LINE_END_CHAR)-1);

		//���� �о��ش�...
		memset(szNowCommand, 0x00, sizeof(szNowCommand));
		memcpy(szNowCommand, pszCommandsBuffer+iLastPointer, iNextPointer-iLastPointer);
		//�о� �� �Ϳ��� �ڵ带 �о��ش�..
		if(strlen(szNowCommand)>0)
			sscanf(szNowCommand, "%d", &iNowCode);

		//���� �ڵ�� ���� ������, ���� �ڵ尪���� Vector�� �����Ѵ�.
		if(iNowCode!=iPreCode)
		{
			//�����ڵ�� ������ �����Ͱ� �־�� �Ѵ�.
			if(strlen(szOneCommand)>0)
			{
				//�޸𸮸� �����ϰ�, �����Ѵ�.
				char* pszCommand = new char[strlen(szOneCommand)+1];
				sprintf(pszCommand, "%s", szOneCommand);
				//pszCommand�� Pointer�� vector�� �����Ѵ�.
				m_vectorCommand.push_back(pszCommand);
			}
			memset(szOneCommand, 0x00, sizeof(szOneCommand));
			sprintf(szOneCommand, "%s", szNowCommand);
		}
		else
		{
			if(strlen(szOneCommand)>0)
				strcat(szOneCommand, szNowCommand);
			else
				sprintf(szOneCommand, "%s", szNowCommand);
		}
		iPreCode = iNowCode;
	}

	if(strlen(szOneCommand)>0)
	{
		//�޸𸮸� �����ϰ�, �����Ѵ�.
		char* pszCommand = new char[strlen(szOneCommand)+1];
		sprintf(pszCommand, "%s", szOneCommand);
		//pszCommand�� Pointer�� vector�� �����Ѵ�.
		m_vectorCommand.push_back(pszCommand);
		//������ �Ŀ�, �޸𸮸� Ŭ���� �ϰ� ��� �ʱ�ȭ �Ѵ�.
		memset(szOneCommand, 0x00, sizeof(szOneCommand));
	}

	return RETURN_GOOD;
}

int CCommandSocket::GetCommandCodeFromServer(int* pIntCode, char *pszCodeString/*=NULL*/)
{
	//Command Vector�� �������, Data�� �о�;� �ϴµ�..
	if(m_vectorCommand.empty()==true)
	{
		//���Ͽ��� �����Ͱ� ���ٸ�.. �����̴�....
		if(RecvCommandResultData()<=0)
			return RETURN_FAIL;
	}

	ITR_COMMAND_RESULT_VEC itrCommand = m_vectorCommand.begin();

	if(itrCommand==m_vectorCommand.end())
		return RETURN_FAIL;

	if((*itrCommand)==NULL)
		return RETURN_FAIL;

	sscanf((*itrCommand), "%d", pIntCode);
/*Debug*/
	WriteFtpCommandLog("R => %s", (*itrCommand));
	printf("R => %s", (*itrCommand));
/**/

	if(pszCodeString!=NULL)
		sprintf(pszCodeString, "%s", (*itrCommand));

	delete [] (*itrCommand);
	(*itrCommand) = NULL;
	m_vectorCommand.erase(itrCommand);

	return *pIntCode;
}

int CCommandSocket::RemoteChangeDirectory(char *pszPath)
{
	char szCommand[MAX_COMMAND_BUFFER];
	memset(szCommand, 0x00, sizeof(szCommand));

	sprintf(szCommand, "CWD %s", pszPath);
	if(SendCommandData(szCommand)<0)
	{
		printf("!!! Fail to Send 'CWD'..\n");
		return RETURN_FAIL;		
	}

	int iCode = 0;
	if(GetCommandCodeFromServer(&iCode)<0)
	{
		printf("!!! Fail to Get CODE for 'CWD'..\n");
		return RETURN_FAIL;		
	}

	//�н����� ���� �׳� �н���...
	if(iCode!=SUCCESS_COMMAND)
		return RETURN_FAIL;

	return RETURN_GOOD;
}

int CCommandSocket::SetTransferType(BOOL bBin/*=FLASE*/)
{
	char szCommand[MAX_COMMAND_BUFFER];
	memset(szCommand, 0x00, sizeof(szCommand));

	if(bBin==FALSE)
		sprintf(szCommand, "TYPE A");
	else
		sprintf(szCommand, "TYPE I");

	if(SendCommandData(szCommand)<0)
	{
		printf("!!! Error Send Data...[TYPE A]\n");
		return RETURN_FAIL;
	}
	int iCode = 0;
	if(GetCommandCodeFromServer(&iCode)<0)
	{
		printf("!!! Error Recv Open Connection For Dir-List Message\n");
		return RETURN_FAIL;
	}
	if(iCode!=CHANGE_TYPE_SUCCESS)
	{
		printf("!!! Error it's not code=>%d\n", iCode);
		return RETURN_FAIL;
	}

	return RETURN_GOOD;
}

int CCommandSocket::GetRemoteFileSize(char *pszFileName)
{
	char szCommand[MAX_COMMAND_BUFFER];
	memset(szCommand, 0x00, sizeof(szCommand));
	sprintf(szCommand, "SIZE %s", pszFileName);

	if(SendCommandData(szCommand)<0)
	{
		printf("!!! Error Send Data...[%s]\n", szCommand);
		return RETURN_FAIL;
	}

	char szReturnString[MAX_LINE_LENGTH];
	memset(szReturnString, 0x00, sizeof(szReturnString));

	int iCode = 0;
	if(GetCommandCodeFromServer(&iCode, szReturnString)<0)
	{
		printf("!!! Error Recv %s\n", szCommand);
		return RETURN_FAIL;
	}
	if(iCode!=TARGET_FILE_SIZE)
	{
		printf("!!! Error it's not code=>%s\n", szReturnString);
		return RETURN_FAIL;
	}

	int iFileSize = 0;
	sscanf(szReturnString+CODE_LENGTH, "%d", &iFileSize);
	if(iFileSize<=0)
	{
		printf("!!! FileSize is Error => %d\n", iFileSize);
		return RETURN_FAIL;
	}

	return iFileSize;
}

////////////////////////////////////////////////////////////
// �����͸� �� ���´ٴ� ������ command connection���� �޴´�.
int CCommandSocket::ConfirmEndTransfer()
{
	int iCode = 0;

	//time-out, socket error ��� ����..
	//Code�� ������ ���� while������ Ż��...
	//===> time-out�� ��� ���ؼ��� 0�� Check�� �ؾ��Ѵ�.
	if(SOCKET_ERROR>=GetCommandCodeFromServer(&iCode))
	{
		printf("!!! Error On Result from command socket=>%d\n", iCode);
		return RETURN_FAIL;
	}

	//���� ���� �ϱ�?
	if(iCode!=TRANSFER_COMPLETE)
		return RETURN_FAIL;

	return RETURN_GOOD;
}

int CCommandSocket::ProcedureConnectDataSocket(char *pszCommand,
											   CDataSocket* pDataSocket)
{
// Get Data Connection Port
	int iPort = GetPortFotDataConnection();
	
	//Port��ȣ�� 0���ٴ� Ŀ���Ѵ�. ����.. 1024���� Ŀ����..
	if(iPort<=0)
	{
		printf("!!! Port is less then zero => %d\n", iPort);
		return RETURN_FAIL;
	}

// Send Command
	if(SendCommandData(pszCommand)<0)
	{
		printf("!!! Error Send Data...[%s]\n", pszCommand);
		return RETURN_FAIL;
	}

// Data Connection ����....
	if(pDataSocket->ConnectFtp(m_szServerName, iPort)<0)
	{
		printf("!!! Data Socket Connect Fail\n");
		return RETURN_FAIL;
	}

//Connection�� Open�϶�� �޽����� �޾ƾ� �Ѵ�.
	int iCode = 0;
	if(GetCommandCodeFromServer(&iCode)<0)
	{
		printf("!!! Error Recv Open Connection For download\n");
		return RETURN_FAIL;
	}

	if(iCode == CONNECT_ERROR )
	{
		return iCode;
	}

	if(iCode!=OPEN_CONNECT_FOR_DIRLIST)
	{
		//printf("!!! Recv Error On Open Connection For download => %d\n", iCode);
		//return RETURN_FAIL;
	}

	return RETURN_GOOD;
}

int CCommandSocket::GetRemotePwd(char *pszRemotePwd)
{
	char szCommand[MAX_COMMAND_BUFFER];
	memset(szCommand, 0x00, sizeof(szCommand));

	sprintf(szCommand, "PWD");
	if(SendCommandData(szCommand)<0)
	{
		printf("!!! Error Send Data...[%s]\n", szCommand);
//		MessageBox( NULL, "101", "getremotepwd", MB_OK );
		return RETURN_FAIL;
	}

	char szReturnString[MAX_LINE_LENGTH];
	memset(szReturnString, 0x00, sizeof(szReturnString));
	int iCode = 0;
	if(GetCommandCodeFromServer(&iCode, szReturnString)<0)
	{
		printf("!!! Error Recv %s\n", szCommand);
//		MessageBox( NULL, "102", "getremotepwd", MB_OK );
		return RETURN_FAIL;
	}
	if(iCode!=PRESENT_WORKING_DIR)	//KES ���� ����!! iCode �� 200 (257�϶� ����) ���н� connect���� �ٽ��ϸ� ���.
	{
		printf("!!! Error it's not code=>%s\n", szReturnString);
//		char buff[64];
//		wsprintf( buff, "iCode:%d, %d, %s", iCode, PRESENT_WORKING_DIR, szReturnString );
//		MessageBox( NULL, buff, "getremotepwd", MB_OK );
		return RETURN_FAIL;
	}

	CFtpFile ftpFile;
	if(ftpFile.GetRemoteDirectoryString(szReturnString, pszRemotePwd)==FALSE)
	{
		printf("!!! Error Directory Name =>%s \n", szReturnString);
//		MessageBox( NULL, "104", "getremotepwd", MB_OK );
		return RETURN_FAIL;
	}

	return RETURN_GOOD;
}

int CCommandSocket::RemoteDeleteFile(char *pszFileName)
{
	char szCommand[MAX_COMMAND_BUFFER];
	memset(szCommand, 0x00, sizeof(szCommand));

	sprintf(szCommand, "DELE %s", pszFileName);
	if(SendCommandData(szCommand)<0)
	{
		printf("!!! Error Send Data...[%s]\n", szCommand);
		return RETURN_FAIL;
	}

	int iCode = 0;
	if(GetCommandCodeFromServer(&iCode)<0)
	{
		printf("!!! Error Recv %d\n", iCode);
		return RETURN_FAIL;
	}
	if(iCode!=SUCCESS_COMMAND)
	{
		printf("!!! Error it's not code=>%d\n", iCode);
		return RETURN_FAIL;
	}

	return RETURN_GOOD;
}

int CCommandSocket::RemoteRenameFile(char *pszSourceFileName, char *pszDestFileName)
{
	char szCommand[MAX_COMMAND_BUFFER];
	memset(szCommand, 0x00, sizeof(szCommand));
	sprintf(szCommand, "RNFR %s", pszSourceFileName);
	if(SendCommandData(szCommand)<0)
	{
		printf("!!! Error Send Data...[%s]\n", szCommand);
		return RETURN_FAIL;
	}

	int iCode = 0;
	if(GetCommandCodeFromServer(&iCode)<0)
	{
		printf("!!! Error Recv %d\n", iCode);
		return RETURN_FAIL;
	}
	if(iCode!=WAIT_NEXT_COMMAND)
	{
		printf("!!! Error it's not code=>%d\n", iCode);
		return RETURN_FAIL;
	}

	memset(szCommand, 0x00, sizeof(szCommand));
	sprintf(szCommand, "RNTO %s", pszDestFileName);
	if(SendCommandData(szCommand)<0)
	{
		printf("!!! Error Send Data...[%s]\n", szCommand);
		return RETURN_FAIL;
	}

	if(GetCommandCodeFromServer(&iCode)<0)
	{
		printf("!!! Error Recv %d\n", iCode);
		return RETURN_FAIL;
	}
	if(iCode!=SUCCESS_COMMAND)
	{
		printf("!!! Error it's not code=>%d\n", iCode);
		return RETURN_FAIL;
	}

	return RETURN_GOOD;
}

void CCommandSocket::WriteFtpCommandLog(const char *pszFormat, ...)
{
	/*
#ifdef	__UNIX__
	struct timeb tbCurrent;
	ftime(&tbCurrent);
#else
	struct _timeb tbCurrent;
	_ftime(&tbCurrent);
#endif

	struct tm* pTimeCurrent = localtime(&(tbCurrent.time));

	char szTimeLog[256];
	memset(szTimeLog, 0x00, sizeof(szTimeLog));
	sprintf(szTimeLog,"[%02d:%02d:%02d.%03d] ",
		pTimeCurrent->tm_hour, pTimeCurrent->tm_min, 
		pTimeCurrent->tm_sec, tbCurrent.millitm);

	char szLogFileName[MAX_PATH];
	memset(szLogFileName, 0x00, sizeof(szLogFileName));
	sprintf(szLogFileName, "%sCtmpFtpProtocol-%04d%02d%02d.log",
							FTPLOG_PATH_NAME,
							pTimeCurrent->tm_year+1900,
							pTimeCurrent->tm_mon+1,
							pTimeCurrent->tm_mday);

	FILE* pLogFile;
	va_list vArgs;	
	va_start(vArgs, pszFormat);

	pLogFile = fopen(szLogFileName, "a+b");
	if(pLogFile!=NULL)
	{
		fprintf(pLogFile, "%s", (char*)szTimeLog);
		vfprintf(pLogFile, pszFormat, vArgs);
		fclose(pLogFile);
	}
	va_end(vArgs);
	*/
}

void CCommandSocket::SetResponseTimeOut(int iWaitLimit)
{
	m_iResponseTimeOut = iWaitLimit;
}

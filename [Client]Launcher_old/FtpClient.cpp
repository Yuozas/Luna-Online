// FtpClient.cpp: implementation of the CFtpClient class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "FtpClient.h"
#include "FtpFile.h"
#include "common.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFtpClient::CFtpClient()
{
	InitVariables();
}

CFtpClient::~CFtpClient()
{
	SafeCloseDataSocket();
	SafeCloseCommandSocket();

	printf("!!!!!!!!!!!! Destroy FtpClient\n");
}

BOOL CFtpClient::InitalizeCheckCommandSocket(BOOL bCreate/*=FALSE*/)
{
// ������ ������ ���� ���, �����ϰ� �ٽ� �����. ������ Return True�̴�.
	if(bCreate==TRUE)
	{
		if(m_pCommandSocket!=NULL)
		{
			delete m_pCommandSocket;
			m_pCommandSocket = NULL;
		}
		m_pCommandSocket = new CCommandSocket;

		/*���⼭ �������� �������־�� �Ѵ�. 1����..

		*/

		return TRUE;
	}
//bCreate = FALSE �� ���...NULL�� Check�Ѵ�.
	else
	{
		if(m_pCommandSocket==NULL)
			return FALSE;
		else
			return TRUE;
	}

	return FALSE;
}

void CFtpClient::SafeCloseCommandSocket()
{
	if(m_pCommandSocket!=NULL)
	{
//		m_pCommandSocket->CloseFtp();
//		m_pCommandSocket->CloseConnection();
//		m_pCommandSocket->ConfirmEndTransfer();
//		m_pCommandSocket->
		delete m_pCommandSocket;
		m_pCommandSocket = NULL;
	}
}

BOOL CFtpClient::InitalizeCheckDataSocket(BOOL bCreate)
{
// ������ ������ ���� ���, �����ϰ� �ٽ� �����. ������ Return True�̴�.
	if(bCreate==TRUE)
	{
		if(m_pDataSocket!=NULL)
		{
			delete m_pDataSocket;
			m_pDataSocket = NULL;
		}
		m_pDataSocket = new CDataSocket;
		return TRUE;
	}
//bCreate = FALSE �� ���...NULL�� Check�Ѵ�.
	else
	{
		if(m_pDataSocket==NULL)
			return FALSE;
		else
			return TRUE;
	}

	return FALSE;
}

void CFtpClient::SafeCloseDataSocket()
{
	if(m_pDataSocket!=NULL)
	{
		delete m_pDataSocket;
		m_pDataSocket = NULL;
	}
}

int CFtpClient::ConnectFtp(const char *pszServerName, unsigned int uiPort, int iTimeOut/*=0*/)
{
	if(InitalizeCheckCommandSocket(TRUE)==FALSE)
		return RETURN_FAIL;

	//memset(m_szServerName, 0x00, sizeof(m_szServerName));
	//m_uiServerPort = 0;

	sprintf(m_szServerName, "%s", pszServerName);
	m_uiServerPort = uiPort;

	if(iTimeOut!=0)
		m_pCommandSocket->SetResponseTimeOut(iTimeOut);

	return m_pCommandSocket->ConnectFtp(pszServerName, uiPort);
}

int CFtpClient::LogIn( const char *pszUserId, const char *pszPassword)
{
	if(InitalizeCheckCommandSocket()==FALSE)
		return RETURN_FAIL;

	//memset(m_szUserId, 0x00, sizeof(m_szUserId));
	//memset(m_szPassword, 0x00, sizeof(m_szPassword));

	//sprintf(m_szUserId, "%s", pszUserId);
	//sprintf(m_szPassword, "%s", pszPassword);

	strcpy( m_szUserId, pszUserId );
	strcpy( m_szPassword, pszPassword );

	return m_pCommandSocket->LogIn(pszUserId, pszPassword);
}

int CFtpClient::GetRemoteFileList(char* pszFileList/*=NULL*/, unsigned int uiBufferSize/*=MAX_LINE_LENGTH*/)
{
	if(InitalizeCheckCommandSocket()==FALSE)
		return RETURN_FAIL;

////////////////////////////////////////////////////////////
// Type ����..
	if(m_pCommandSocket->SetTransferType(FALSE)<0)
		return RETURN_FAIL;

////////////////////////////////////////////////////////////
// Data Connection ����..
	InitalizeCheckDataSocket(TRUE);

	char szCommand[MAX_COMMAND_BUFFER];
	memset(szCommand, 0x00, sizeof(szCommand));

	sprintf(szCommand, "NLST");
	if(m_pCommandSocket->ProcedureConnectDataSocket(szCommand, m_pDataSocket)<0)
	{
		SafeCloseDataSocket();
		return RETURN_FAIL;
	}

////////////////////////////////////////////////////////////
// Fetch the data, list of files..
	char szRecvBuffer[MAX_SOCKET_BUFFER];
	memset(szRecvBuffer, 0x00, sizeof(szRecvBuffer));

	int iTotRecvLength = 0;
	int iRecvLength = 0;
	while(TRUE)
	{
		//�����Ͱ� ��� ������ �����ڰ� ����.
		//�׳� socket�� �ִµ��� �� �����´�. ������, ������..
		//�� ���� �ϳ� �־�� �Ѵ�
		if((iRecvLength==0)&&(iTotRecvLength>0))
			break;

		iRecvLength = m_pDataSocket->RecvInformationData(szRecvBuffer, sizeof(szRecvBuffer)-1);
		//������Ȳ�� Ȯ���ϴ�......
		if(iRecvLength<0)
		{
			SafeCloseDataSocket();
			m_pCommandSocket->ConfirmEndTransfer();
			return RETURN_FAIL;
		}

		iTotRecvLength += iRecvLength;
/*DEBUG*/
		if(strlen(szRecvBuffer)>0)
			printf("%s", szRecvBuffer);

		if(pszFileList!=NULL)
		{
			//���� ������ ��ŭ�� ��´�...�Է¹��� ������� ���Ṯ�� ������ ũ����.
			if((strlen(pszFileList)+(unsigned int)iRecvLength)<=uiBufferSize)
			{
				if(strlen(pszFileList)>0)
					strcat(pszFileList, szRecvBuffer);
				else
					sprintf(pszFileList, "%s", szRecvBuffer);
			}
		}
/**/
		memset(szRecvBuffer, 0x00, sizeof(szRecvBuffer));
	}

/*DEBUG*/
	printf("TOTAL LENGTH from DATA CONNECTION => %d bytes\n", iTotRecvLength);
/**/
// End Fetch the data, list of files..
////////////////////////////////////////////////////////////

	SafeCloseDataSocket();

	return m_pCommandSocket->ConfirmEndTransfer();
}

int CFtpClient::RemoteChangeDirectory(char *pszPath)
{
	if(InitalizeCheckCommandSocket()==FALSE)
		return RETURN_FAIL;

	return m_pCommandSocket->RemoteChangeDirectory(pszPath);
}

int CFtpClient::DownloadFile(char *pszFileName, char *pszDownloadPath, BOOL bBinMode/*=FALSE*/)
{
	if(InitalizeCheckCommandSocket()==FALSE)
		//return 1;
		return RETURN_FAIL;

/////////////////////////////////////////////////////////////////
// Local���� ����� �̸��� �����......
	char szDownFullName[MAX_PATH];
	memset(szDownFullName, 0x00, sizeof(szDownFullName));

	if(pszDownloadPath!=NULL)
	{
		CFtpFile ftpFile;
		if(ftpFile.CheckDownloadPath(pszDownloadPath))
			sprintf(szDownFullName, "%s%s", pszDownloadPath, pszFileName);
		else
#ifdef __UNIX__
			sprintf(szDownFullName, "%s/%s", pszDownloadPath, pszFileName);
#else
			sprintf(szDownFullName, "%s\\%s", pszDownloadPath, pszFileName);
#endif
	}
	else
		sprintf(szDownFullName, "%s", pszFileName);
/////////////////////////////////////////////////////////////////

	FILE* pTestFile = fopen(szDownFullName, "r");
	if(pTestFile!=NULL)
	{
		printf("!!! File is already exsit => %s, errno=>%d, errstr=>%s\n",
					szDownFullName, errno, strerror(errno));
		fclose(pTestFile);
		DeleteFile( szDownFullName );
		//return RETURN_FAIL;
	}

////////////////////////////////////////////////////////////
// ���� �̸� �����..
	char szRemoteDirectory[MAX_PATH];
	memset(szRemoteDirectory, 0x00, sizeof(szRemoteDirectory));
	if(m_pCommandSocket->GetRemotePwd(szRemoteDirectory)<0)
		return RETURN_FAIL;
//		return 2;

	char szTargetName[MAX_PATH];
	memset(szTargetName, 0x00, sizeof(szTargetName));
	sprintf(szTargetName, "%s%s", szRemoteDirectory, pszFileName);
// End ���� �̸� �����..
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Type ����..
	if(m_pCommandSocket->SetTransferType(bBinMode)<0)
		return RETURN_FAIL;
//		return 3;

	int iFileSize = m_pCommandSocket->GetRemoteFileSize(szTargetName);
	if(iFileSize<=0)
		return RETURN_FAIL;
//		return 4;

	InitalizeCheckDataSocket(TRUE);

////////////////////////////////////////////////////////////
// Data Connection ����..
	char szCommand[MAX_COMMAND_BUFFER];
	memset(szCommand, 0x00, sizeof(szCommand));
	sprintf(szCommand, "RETR %s", szTargetName);
	
	if(m_pCommandSocket->ProcedureConnectDataSocket(szCommand, m_pDataSocket)<0)
	{
		SafeCloseDataSocket();
		m_pCommandSocket->ConfirmEndTransfer();
		return RETURN_FAIL;
		//return 5;
	}

////////////////////////////////////////////////////////////
// TargetFile ����..
	//�� ���̳ʸ� ���Ϸ� ����� �Ѵ�.
	FILE* pTargetFile = fopen(szDownFullName, "wb");
	if(pTargetFile==NULL)
	{
		printf("!!! File is exsit => %s, errno=>%d, errstr=>%s\n",
					szDownFullName, errno, strerror(errno));
		SafeCloseDataSocket();
		m_pCommandSocket->ConfirmEndTransfer();
		return RETURN_FAIL;
		//return 6;
	}
// End TargetFile ����..
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Fetch the data, list of files..
	char szRecvBuffer[MAX_SOCKET_BUFFER];
	memset(szRecvBuffer, 0x00, sizeof(szRecvBuffer));

	int iTotRecvLength = 0;
	int iRecvLength = 0;
	while(TRUE)
	{
		//������ ���̸�ŭ �� �о�´�.
		if(iTotRecvLength>=iFileSize)
			break;

		iRecvLength = m_pDataSocket->RecvInformationData(szRecvBuffer, sizeof(szRecvBuffer)-1);
		
		if(iRecvLength<0)
		{
			printf("!!! Recv Error On download Files => %d\n", iRecvLength);
			fclose(pTargetFile);
			remove(szDownFullName);
			SafeCloseDataSocket();
			m_pCommandSocket->ConfirmEndTransfer();
			return RETURN_FAIL;
			//return 7;
		}
		
		if(iRecvLength>0)
			fwrite(szRecvBuffer, sizeof(char), iRecvLength, pTargetFile);

		iTotRecvLength += iRecvLength;
		memset(szRecvBuffer, 0x00, sizeof(szRecvBuffer));
		
		if(m_bHashMark)
			printf("#");

		if(m_iUpDownSleepTime>0)
#ifdef __UNIX__
			UnixSleep(m_iUpDownSleepTime);
#else
			Sleep(m_iUpDownSleepTime);
#endif
	}
	printf("\n");

	fclose(pTargetFile);

/*DEBUG*/
	printf("TOTAL LENGTH from DATA CONNECTION => %d bytes\n", iTotRecvLength);
/**/
// End Fetch the data, list of files..
////////////////////////////////////////////////////////////

	SafeCloseDataSocket();


	return m_pCommandSocket->ConfirmEndTransfer();
}


int CFtpClient::UploadFile(char *pszFileName, BOOL bBinMode/*=FALSE*/)
{
	if(InitalizeCheckCommandSocket()==FALSE)
		return RETURN_FAIL;

	FILE* pTestFile = fopen(pszFileName, "r");
	if(pTestFile==NULL)
	{
		printf("!!! File is not exsit => %s, errno=>%d, errstr=>%s\n",
					pszFileName, errno, strerror(errno));
		return RETURN_FAIL;
	}

////////////////////////////////////////////////////////////
// ���� �̸� �����..
	CFtpFile ftpFile;
	char* pszOnlyFileName = ftpFile.GetPureFileName(pszFileName);

	char szRemoteDirectory[MAX_PATH];
	memset(szRemoteDirectory, 0x00, sizeof(szRemoteDirectory));
	if(m_pCommandSocket->GetRemotePwd(szRemoteDirectory)<0)
		return RETURN_FAIL;

	char szTargetName[MAX_PATH];
	memset(szTargetName, 0x00, sizeof(szTargetName));
	sprintf(szTargetName, "%s%s", szRemoteDirectory, pszOnlyFileName);

////////////////////////////////////////////////////////////
// Type ����..
	if(m_pCommandSocket->SetTransferType(bBinMode)<0)
		return RETURN_FAIL;

	InitalizeCheckDataSocket(TRUE);

////////////////////////////////////////////////////////////
// Data Connection ����..
	char szCommand[MAX_COMMAND_BUFFER];
	memset(szCommand, 0x00, sizeof(szCommand));
	sprintf(szCommand, "STOR %s", szTargetName);
	
	if(m_pCommandSocket->ProcedureConnectDataSocket(szCommand, m_pDataSocket)<0)
	{
		SafeCloseDataSocket();
		m_pCommandSocket->ConfirmEndTransfer();
		return RETURN_FAIL;
	}

////////////////////////////////////////////////////////////
// ���� ����....
	//���߿� Rename�� �����ϵ��� �ٲ�� �ϳ�?? �ʿ���� ��..
	FILE* pTargetFile = fopen(pszFileName, "rb");
	if(pTargetFile==NULL)
	{
		printf("!!! File is not exsit => %s\n", pszFileName);
		SafeCloseDataSocket();
		m_pCommandSocket->ConfirmEndTransfer();
		return RETURN_FAIL;
	}

////////////////////////////////////////////////////////////
// Fetch the data, list of files..
	char szSendBuffer[MAX_SOCKET_BUFFER];
	memset(szSendBuffer, 0x00, sizeof(szSendBuffer));

	int iTotSendLength = 0;
	int iSendLength = 0;
	int iReadLength = 0;

	while(!feof(pTargetFile))
	{
		iReadLength = 
				fread(szSendBuffer, sizeof(char), sizeof(szSendBuffer)-1, pTargetFile);
		
		if(ferror(pTargetFile))
		{
			printf("!!! Send Error On Read File => %d, %s\n", errno, strerror(errno));
			fclose(pTargetFile);
/*�ö� Remote������ �����*/  //=== �߰��ؾ� ��...
			SafeCloseDataSocket();
			m_pCommandSocket->ConfirmEndTransfer();
			return RETURN_FAIL;
		}

		iSendLength = 
				m_pDataSocket->SendInformationData(szSendBuffer, iReadLength);
		
		if(iSendLength<0)
		{
			printf("!!! Send Error On upload Files => %d\n", iSendLength);
			fclose(pTargetFile);
/*�ö� Remote������ �����*/  //=== �߰��ؾ� ��...
			SafeCloseDataSocket();
			m_pCommandSocket->ConfirmEndTransfer();
			return RETURN_FAIL;
		}
		
		iTotSendLength += iSendLength;
		memset(szSendBuffer, 0x00, sizeof(szSendBuffer));

		if(m_bHashMark)
			printf("#");

		if(m_iUpDownSleepTime>0)
#ifdef __UNIX__
			UnixSleep(m_iUpDownSleepTime);
#else
			Sleep(m_iUpDownSleepTime);
#endif
	}
	printf("\n");

	fclose(pTargetFile);

/*DEBUG*/
	printf("TOTAL LENGTH through out DATA CONNECTION => %d bytes\n", iTotSendLength);
/**/
// End Fetch the data, list of files..
////////////////////////////////////////////////////////////

	SafeCloseDataSocket();

	return m_pCommandSocket->ConfirmEndTransfer();
}

int CFtpClient::GetRemoteCurrentDirectory(char *pszDirectoryName)
{
	if(InitalizeCheckCommandSocket()==FALSE)
		return RETURN_FAIL;

	char szRemoteDirectory[MAX_PATH];
	memset(szRemoteDirectory, 0x00, sizeof(szRemoteDirectory));
	if(m_pCommandSocket->GetRemotePwd(szRemoteDirectory)<0)
		return RETURN_FAIL;
	
	sprintf(pszDirectoryName, "%s", szRemoteDirectory);
	return RETURN_GOOD;
}


int CFtpClient::RemoteDeleteFile(char *pszFileName)
{
	if(InitalizeCheckCommandSocket()==FALSE)
		return RETURN_FAIL;

	return m_pCommandSocket->RemoteDeleteFile(pszFileName);
}

int CFtpClient::RemoteRenameFile(char *pszSourceFileName, char *pszDestFileName)
{
	if(InitalizeCheckCommandSocket()==FALSE)
		return RETURN_FAIL;

	return m_pCommandSocket->RemoteRenameFile(pszSourceFileName, pszDestFileName);
}

//Download���� �غ������ �̸� �� ���´�.
//DATASOCKET ����.. 
//TYPE ����..
//������ ������ local�� �����..
int CFtpClient::SetupExtendDownload(char *pszFileName, char* pszDownloadPath, BOOL bBinMode/*=FALSE*/)
{
///////////////////////////////////////////////////////////////////////////
//���� ���� �����带 �������� �ʴ´�. 1���� DataSocket�� ����Ѵ�.
//����, �ܺο��� �ΰ��� Thread�� 1���� DLL�̳� Class�� ������ ���ÿ� ������ 
//��� �߻��ϴ� ���׸� ���ش�.
//���߿� �������� ������ ������ �����ϵ��� ������ �� �ִ�.
//DataSocket ID�� �����ϸ�, SetupExtendDownload ȣ��ø��� 1���� �����Ѵ�.
	if((strlen(m_szUploadFileName)>0)
		||(m_pUploadSaveFile!=NULL)
		||(m_iDownloadFileSize>0)
		||(strlen(m_szDownloadFileName)>0)
		||(m_pDownloadSaveFile!=NULL)
		||(m_iDownloadFileSize>0))
	{
		return RETURN_FAIL;
	}
//End ���� ���� �����带 �������� �ʴ´�. 1���� DataSocket�� ����Ѵ�.
//////////////////////////////////////////////////////////////////////////

//Command Socket�� Ȯ���Ѵ�.
	if(InitalizeCheckCommandSocket()==FALSE)
		return RETURN_FAIL;

/////////////////////////////////////////////////////////////////
// Local���� ����� �̸��� �����......
	char szDownFullName[MAX_PATH];
	memset(szDownFullName, 0x00, sizeof(szDownFullName));

	if(pszDownloadPath!=NULL)
	{
		CFtpFile ftpFile;
		if(ftpFile.CheckDownloadPath(pszDownloadPath))
			sprintf(szDownFullName, "%s%s", pszDownloadPath, pszFileName);
		else
#ifdef __UNIX__
			sprintf(szDownFullName, "%s/%s", pszDownloadPath, pszFileName);
#else
			sprintf(szDownFullName, "%s\\%s", pszDownloadPath, pszFileName);
#endif
	}
	else
		sprintf(szDownFullName, "%s", pszFileName);
/////////////////////////////////////////////////////////////////

//������ �ִ��� Ȯ���ϰ� �����.
	FILE* pTestFile = fopen( PATCH_FILE, "r" );
	if( pTestFile != NULL )
	{
		fclose(pTestFile);
		DeleteFile( PATCH_FILE );
	}

////////////////////////////////////////////////////////////
// ���� �̸� �����..
	char szRemoteDirectory[MAX_PATH];
	memset(szRemoteDirectory, 0x00, sizeof(szRemoteDirectory));
	if(m_pCommandSocket->GetRemotePwd(szRemoteDirectory)<0)
		return RETURN_FAIL;

	char szTargetName[MAX_PATH];
	memset(szTargetName, 0x00, sizeof(szTargetName));
	sprintf(szTargetName, "%s%s%s", szRemoteDirectory, pszFileName, ".zip");
// End ���� �̸� �����..
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Type ����..
	if(m_pCommandSocket->SetTransferType(bBinMode)<0)
		return RETURN_FAIL;

//������ ��������.....
	int iFileSize = m_pCommandSocket->GetRemoteFileSize(szTargetName);
	if(iFileSize<=0)
		return RETURN_FAIL;

//������ ������ �����ϰ�....
	InitalizeCheckDataSocket(TRUE);

////////////////////////////////////////////////////////////
// Data Connection ����..
	char szCommand[MAX_COMMAND_BUFFER];
	memset(szCommand, 0x00, sizeof(szCommand));
	sprintf(szCommand, "RETR %s", szTargetName);
	
	if(m_pCommandSocket->ProcedureConnectDataSocket(szCommand, m_pDataSocket)<0)
	{
		SafeCloseDataSocket();
		SetExtendDownload(szDownFullName, NULL, iFileSize);
		return RETURN_FAIL;
	}

////////////////////////////////////////////////////////////
// TargetFile ����..
	//�� ���̳ʸ� ���Ϸ� ����� �Ѵ�.
	FILE* pTargetFile = fopen( PATCH_FILE, "wb" );
	if( pTargetFile == NULL )
	{
		printf("!!! File is exsit => %s, errno=>%d, errstr=>%s\n",
					szDownFullName, errno, strerror(errno));
		SafeCloseDataSocket();
		SetExtendDownload(szDownFullName, pTargetFile, iFileSize);
		return RETURN_FAIL;
	}
//	fclose(pTargetFile); ==> ���� �ʴ´�.. Release���� �ݾ��ش�..
// End TargetFile ����..
////////////////////////////////////////////////////////////

	SetExtendDownload(szDownFullName, pTargetFile, iFileSize);

//������ ����� ���� ���� ���ϻ�����..
	return iFileSize;
}

void CFtpClient::SetExtendDownload(char *pszFileName, FILE *pFile, int iFileSize)
{
	sprintf(m_szDownloadFileName, "%s", pszFileName);
	m_pDownloadSaveFile = pFile;
	m_iDownloadFileSize = iFileSize;
}

void CFtpClient::ReleaseExtendDownload()
{
	//���ϻ���� �ִٴ� ���� �ϴ� �����ͼ����� �ٿ����ٴ� �����..
	if(m_iDownloadFileSize>0)
	{
		SafeCloseDataSocket();
		m_pCommandSocket->ConfirmEndTransfer();
	}

	//���� �߻��ÿ� ���ÿ� �ִ� ������ ����....
	//�����ϱ� ���� �ڵ��� �ݾ� ��..
	if(m_pDownloadSaveFile!=NULL)
	{
		fclose(m_pDownloadSaveFile);
		m_pDownloadSaveFile = NULL;
	}

	//���� �߻��ÿ� ���ÿ� �ִ� ������ ����....
	if(m_iLastDownloadError<RETURN_GOOD)
		remove(m_szDownloadFileName);

	//������ �ʱ�ȭ..
	m_iDownloadFileSize = 0;
	memset(m_szDownloadFileName, 0x00, sizeof(m_szDownloadFileName));
	m_iLastDownloadError = 0;
	m_iTotRecvSize = 0;

}

//���� ������� ���� ������ ������� 1����Ʈ ���� �Է¹޾ƾ� �Ѵ�.
//��, iBufferSize�� ���� pszDownloadBuffer�� ������� 1����Ʈ ���� ���� �Է¹޾ƾ� ��.
int CFtpClient::WriteExtendDownloadFile()
{
	if((strlen(m_szDownloadFileName)<=0)
		||(m_pDownloadSaveFile==NULL)
		||(m_iDownloadFileSize<=0))
	{
		m_iLastDownloadError = RETURN_FAIL;
		return RETURN_FAIL;
	}

	char szRecvBuffer[MAX_SOCKET_BUFFER];
	memset(szRecvBuffer, 0x00, sizeof(szRecvBuffer));

	int iRecvLength = m_pDataSocket->RecvInformationData(szRecvBuffer, sizeof(szRecvBuffer)-1);
	if(iRecvLength<0)
	{
		m_iLastDownloadError = RETURN_FAIL;
		printf("!!! Recv Error On download Files => %d\n", iRecvLength);
		return RETURN_FAIL;
	}

	if((iRecvLength>0)&&(m_pDownloadSaveFile!=NULL))
		fwrite(szRecvBuffer, sizeof(char), iRecvLength, m_pDownloadSaveFile);

	//������ ���̸�ŭ �� �о�´�.
	m_iLastDownloadError = RETURN_GOOD;
	m_iTotRecvSize += iRecvLength;

	return m_iTotRecvSize;
}


void CFtpClient::SetExtendUpload(char *pszFileName, FILE *pFile, int iFileSize)
{
	sprintf(m_szUploadFileName, "%s", pszFileName);
	m_pUploadSaveFile = pFile;
	m_iUploadFileSize = iFileSize;
}

void CFtpClient::ReleaseExtendUpload()
{
	//���ϻ���� �ִٴ� ���� �ϴ� �����ͼ����� �ٿ����ٴ� �����..
	//Upload�� ��쿡�� �����ϰ� �۵��ϰ� �ϱ� ���ؼ� �ҽ��� ���߾���...
	if(m_iUploadFileSize>0)
	{
		SafeCloseDataSocket();
		m_pCommandSocket->ConfirmEndTransfer();
	}

	//���� �߻��ÿ� ���ÿ� �ִ� ������ ����....
	//�����ϱ� ���� �ڵ��� �ݾ� ��..
	if(m_pUploadSaveFile!=NULL)
	{
		fclose(m_pUploadSaveFile);
		m_pUploadSaveFile = NULL;
	}

//���� �߻��ÿ� ����Ʈ�� �ִ� ������ ���� ==> ���߿�.. Ȥ�� Client���� ����...
/*
	if(m_iLastUploadError<RETURN_GOOD)
		remove(m_szDownloadFileName);
*/

	//������ �ʱ�ȭ..
	m_iUploadFileSize = 0;
	memset(m_szUploadFileName, 0x00, sizeof(m_szUploadFileName));
	m_iLastUploadError = 0;
	m_iTotSendSize = 0;
}

//Upload����  �غ������ �̸� �� ���´�.
//DATASOCKET ����.. 
//TYPE ����..
//�ø� ���� Ȯ���ϱ�..
int CFtpClient::SetupExtendUpload(char *pszFileName, BOOL bBinMode/*=FALSE*/)
{
///////////////////////////////////////////////////////////////////////////
//���� ���� �����带 �������� �ʴ´�. 1���� DataSocket�� ����Ѵ�.
//����, �ܺο��� �ΰ��� Thread�� 1���� DLL�̳� Class�� ������ ���ÿ� ������ 
//��� �߻��ϴ� ���׸� ���ش�.
//���߿� �������� ������ ������ �����ϵ��� ������ �� �ִ�.
//DataSocket ID�� �����ϸ�, SetupExtendUpload ȣ��ø��� 1���� �����Ѵ�.
	if((strlen(m_szUploadFileName)>0)
		||(m_pUploadSaveFile!=NULL)
		||(m_iDownloadFileSize>0)
		||(strlen(m_szDownloadFileName)>0)
		||(m_pDownloadSaveFile!=NULL)
		||(m_iDownloadFileSize>0))
	{
		return RETURN_FAIL;
	}
//End ���� ���� �����带 �������� �ʴ´�. 1���� DataSocket�� ����Ѵ�.
//////////////////////////////////////////////////////////////////////////

//Command Socket�� Ȯ���Ѵ�.
	if(InitalizeCheckCommandSocket()==FALSE)
		return RETURN_FAIL;

//������ �ִ��� Ȯ���Ѵ�.
	FILE* pTestFile = fopen(pszFileName, "r");
	if(pTestFile==NULL)
	{
		printf("!!! File is not exsit => %s, errno=>%d, errstr=>%s\n",
					pszFileName, errno, strerror(errno));
		return RETURN_FAIL;
	}

////////////////////////////////////////////////////////////
// ���� �̸� �����..
	CFtpFile ftpFile;
	char* pszOnlyFileName = ftpFile.GetPureFileName(pszFileName);

	char szRemoteDirectory[MAX_PATH];
	memset(szRemoteDirectory, 0x00, sizeof(szRemoteDirectory));
	if(m_pCommandSocket->GetRemotePwd(szRemoteDirectory)<0)
		return RETURN_FAIL;

	char szTargetName[MAX_PATH];
	memset(szTargetName, 0x00, sizeof(szTargetName));
	sprintf(szTargetName, "%s%s", szRemoteDirectory, pszOnlyFileName);
// End ���� �̸� �����..
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Type ����..
	if(m_pCommandSocket->SetTransferType(bBinMode)<0)
		return RETURN_FAIL;

//������ ��������.....
	int iFileSize = 0;
#ifdef __UNIX__
	struct stat statFileSize;
	if(stat(pszFileName, &statFileSize)==0)
#else
	struct _stat statFileSize;
	if(_stat(pszFileName, &statFileSize)==0)
#endif
		iFileSize = statFileSize.st_size;

	if(iFileSize<=0)
		return RETURN_FAIL;
//End ������ ��������.....

//������ ������ �����ϰ�....
	InitalizeCheckDataSocket(TRUE);

////////////////////////////////////////////////////////////
// Data Connection ����..
	char szCommand[MAX_COMMAND_BUFFER];
	memset(szCommand, 0x00, sizeof(szCommand));
	sprintf(szCommand, "STOR %s", szTargetName);
	
	if(m_pCommandSocket->ProcedureConnectDataSocket(szCommand, m_pDataSocket)<0)
	{
		SafeCloseDataSocket();
		SetExtendUpload(pszFileName, NULL, iFileSize);
		return RETURN_FAIL;
	}

////////////////////////////////////////////////////////////
// SourceFile ����...
	//�� ���̳ʸ� ���Ϸ� ����� �Ѵ�.
	FILE* pSourceFile = fopen(pszFileName, "rb");
	if(pSourceFile==NULL)
	{
		printf("!!! File is exsit => %s, errno=>%d, errstr=>%s\n",
					pszFileName, errno, strerror(errno));
		SafeCloseDataSocket();
		SetExtendUpload(pszFileName, pSourceFile, iFileSize);
		return RETURN_FAIL;
	}
// End TargetFile ����..
////////////////////////////////////////////////////////////

	SetExtendUpload(pszFileName, pSourceFile, iFileSize);

//������ ����� ���� ���� ���ϻ�����..
	return iFileSize;
}

//���� ������� ���� ������ ������� 1����Ʈ ���� �Է¹޾ƾ� �Ѵ�.
//��, iBufferSize�� ���� pszDownloadBuffer�� ������� 1����Ʈ ���� ���� �Է¹޾ƾ� ��.
int CFtpClient::SendExtendUploadFile()
{
	if((strlen(m_szUploadFileName)<=0)
		||(m_pUploadSaveFile==NULL)
		||(m_iUploadFileSize<=0))
	{
		m_iLastUploadError = RETURN_FAIL;
		return RETURN_FAIL;
	}

	//������ ���� ������ ��쿡��, ���и� ������.
	//Ŭ���̾�Ʈ���� ����� üũ�ؼ� ���� ������ �� �������� Ȯ���ؾ� �Ѵ�.
	if(feof(m_pUploadSaveFile))
	{
		m_iLastUploadError = RETURN_FAIL;
		return RETURN_FAIL;
	}
	
	char szSendBuffer[MAX_SOCKET_BUFFER];
	memset(szSendBuffer, 0x00, sizeof(szSendBuffer));
	int iReadLength = 
			fread(szSendBuffer, sizeof(char), sizeof(szSendBuffer)-1, m_pUploadSaveFile);
		
	if(ferror(m_pUploadSaveFile))
	{
		printf("!!! Send Error On Read File => %d, %s\n", errno, strerror(errno));
		m_iLastUploadError = RETURN_FAIL;
		return RETURN_FAIL;
	}
	// End ���Ͽ��� ������ �о����......

	int iSendLength = 
		m_pDataSocket->SendInformationData(szSendBuffer, sizeof(szSendBuffer)-1);
	if(iSendLength<0)
	{
		m_iLastDownloadError = RETURN_FAIL;
		printf("!!! Recv Error On download Files => %d\n", iSendLength);
		return RETURN_FAIL;
	}

	//������ ���̸�ŭ �� �о�´�.
	m_iLastDownloadError = RETURN_GOOD;
	m_iTotSendSize += iSendLength;

	return m_iTotSendSize;
}

void CFtpClient::SetHashMark(BOOL bHashMark/*=TRUE*/)
{
	m_bHashMark = bHashMark;
}

void CFtpClient::SetCommandResponseTimeOut(int iWaitLimit)
{
	if(InitalizeCheckCommandSocket()==FALSE)
		return;

	m_pCommandSocket->SetResponseTimeOut(iWaitLimit);
}

void CFtpClient::LogOut()
{
	SafeCloseDataSocket();
			//�糪�׽�Ʈ
//			FILE* pFile = fopen( "launcherdebug.txt", "at" );
//			if( pFile )
//			{
//				fprintf( pFile, "****After SafeCloseDataSocket() ***\n" );
//				fclose(pFile);
//			}
			//---------
	SafeCloseCommandSocket(); //���ӱ��� �����Ŵ.....
			//�糪�׽�Ʈ
//			pFile = fopen( "launcherdebug.txt", "at" );
//			if( pFile )
//			{
//				fprintf( pFile, "****After SafeCloseCommandSocket() ***\n" );
//				fclose(pFile);
//			}
			//---------
	//�� �������� ȣ���ؾ� ��..
	InitVariables();
}

void CFtpClient::InitVariables()
{
	m_iTotRecvSize = 0;
	m_iLastDownloadError = 0;
	m_iDownloadFileSize = 0;
	m_pDownloadSaveFile = NULL;
	memset(m_szDownloadFileName, 0x00, sizeof(m_szDownloadFileName));

	m_iTotSendSize = 0;
	m_iLastUploadError = 0;
	m_iUploadFileSize = 0;
	m_pUploadSaveFile = NULL;
	memset(m_szUploadFileName, 0x00, sizeof(m_szUploadFileName));

	m_iUpDownSleepTime = 0;
	m_bHashMark = FALSE;
	memset(m_szServerName, 0x00, sizeof(m_szServerName));
	m_uiServerPort = 0;
	memset(m_szUserId, 0x00, sizeof(m_szUserId));
	memset(m_szPassword, 0x00, sizeof(m_szPassword));

	m_pCommandSocket = NULL;
	m_pDataSocket = NULL;
}

void CFtpClient::SetTransferDataSpeed(int iSleepTime)
{
	m_iUpDownSleepTime = iSleepTime;
}

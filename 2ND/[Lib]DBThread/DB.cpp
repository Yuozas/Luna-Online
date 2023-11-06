
//////////////////////////////////////////////////////////////////////
// Coolwater.Fire version 2001.1.15
// Ultimate Database Gasoline Engine
// If it does not work correctly, plz enter www.coolwater.fire.com 
// 
// programed by Park Sung Jun 
//
//////////////////////////////////////////////////////////////////////
#define _WIN32_WINNT 0x0500

#include "stdafx.h"
#include "DB.h"
#include "dbclass.h"
#include <process.h>
#include "memorypool.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDB::CDB(DESC_DBTHREAD* pDesc)
{
	memset(this,0,sizeof(CDB));
	
	m_pDBAdmin	= new DBCON;
	if (m_pDBAdmin == NULL)
		goto FAIL;
	m_pDBClient = new DBCON;
	if (m_pDBClient == NULL)
		goto FAIL;
	
	memset(m_pDBAdmin,0,sizeof(DBCON));
	memset(m_pDBClient,0,sizeof(DBCON));

	m_pDBProcess1 = new CDBProcess;
	if (m_pDBProcess1 == NULL)
		goto FAIL;
	m_pDBProcess2 = new CDBProcess;
	if (m_pDBProcess2 == NULL)
		goto FAIL;
	m_pDBResultMessage1 = new CDBMessage;
	if (m_pDBResultMessage1 == NULL)
		goto FAIL;
	m_pDBResultMessage2 = new CDBMessage;
	if (m_pDBResultMessage2 == NULL)
		goto FAIL;
	
	m_pResultMemory				= NULL;
	m_pDBMessageMemory			= NULL;
	m_pDBMessageResultMemory	= NULL;
	m_pDBStackMemory			= NULL;

	m_dwMessageMaxSize		=	pDesc->dwMessageMaxSize;		// DBMESSAGE ����ü�� �ִ� ũ��
	m_dwNumofMessage		=	pDesc->dwNumofMessage;			// �ִ� ���� ���� ���� ���� 
	m_dwResultMaxSize		=	pDesc->dwResultMaxSize;		// ���� ����� �ִ� ������(select��)
	m_dwNumofResult			=	pDesc->dwNumofResult;			// ���� ����� �ִ� ����  (select��)
	m_dwSizeofStack			=	pDesc->dwSizeofStack;			// ������ �Է��� ���ڸ� ���� �޸� ũ�� 

	// DB������ ���� �޸� Ǯ ����
	if (m_dwMessageMaxSize < 1 || m_dwMessageMaxSize > 1024)
	{
		MessageBox(NULL,"dwMessageMaxSize is Invalid value: make sure 0~1024","ERROR",NULL);
		goto FAIL;
	}
	if (m_dwNumofMessage < 1 || m_dwNumofMessage > 4096)
	{
		MessageBox(NULL,"dwNumofMessage is Invalid value: make sure 0~4096","ERROR",NULL);
		goto FAIL;
	}
	if (m_dwResultMaxSize < MAX_ROW_NUM*sizeof(QUERYST) || m_dwResultMaxSize > 256000)
	{
		MessageBox(NULL,"dwResultMaxSize is Invalid value: make sure MAX_ROW_NUM*sizeof(QUERYST)~256000","ERROR",NULL);
		goto FAIL;
	}
	if (m_dwNumofResult < 1 || m_dwNumofResult > 1024)
	{
		MessageBox(NULL,"dwNumofResult is Invalid value: make sure 0~1024","ERROR",NULL);
		goto FAIL;
	}
	if (m_dwSizeofStack < 1 || m_dwSizeofStack > 4096)
	{
		MessageBox(NULL,"m_dwSizeofStack is Invalid value: make sure 0~1024","ERROR",NULL);
		goto FAIL;
	}

	m_pDBMessageMemory = new CMemoryPool(m_dwNumofMessage,m_dwMessageMaxSize);
	if(!m_pDBMessageMemory) goto FAIL;

	m_pResultMemory = new CMemoryPool(m_dwNumofResult,m_dwResultMaxSize);
	if(!m_pResultMemory) 	goto FAIL;
	
	m_pDBMessageResultMemory = new CMemoryPool(m_dwNumofMessage,sizeof(DBRESULTMESSAGE));
	if(!m_pDBMessageResultMemory) 	goto FAIL;

	m_pDBStackMemory = new CMemoryPool(m_dwNumofMessage,m_dwSizeofStack);
	if(!m_pDBStackMemory) 	goto FAIL;
	
//	InitializeCriticalSectionAndSpinCount(&m_csDBMessageMemory,200);
//	InitializeCriticalSectionAndSpinCount(&m_csDBStackMemory,200);
//	InitializeCriticalSectionAndSpinCount(&m_csDBResultMemory,200);
//	InitializeCriticalSectionAndSpinCount(&m_csDBResultStackMemory,200);

	return;

FAIL:
	return;
//	m_pWriteMessage("Initial Error : Close App");
}


CDB::~CDB()
{
	CloseDBConnection();

	delete m_pDBClient;
	delete m_pDBAdmin;
	delete m_pDBProcess1;
	delete m_pDBProcess2;
	delete m_pDBResultMessage1;
	delete m_pDBResultMessage2;

//	DeleteCriticalSection(&m_csDBMessageMemory);
//	DeleteCriticalSection(&m_csDBStackMemory);
//	DeleteCriticalSection(&m_csDBResultMemory);
//	DeleteCriticalSection(&m_csDBResultStackMemory);

	if (m_pResultMemory) 
		delete m_pResultMemory;

	if (m_pDBMessageMemory) 
		delete m_pDBMessageMemory;

	if (m_pDBMessageResultMemory) 
		delete m_pDBMessageResultMemory;

	if (m_pDBStackMemory)
		delete 	m_pDBStackMemory;



}

char*	CDB::InitDBConnection(LPSTR szID, LPSTR szPassword, LPSTR Dbname, LPSTR szIDC, LPSTR szPasswordC, LPSTR DbnameC)
{
	strcpy(m_pDBAdmin->szDBName,Dbname);
	strcpy(m_pDBAdmin->szID,szID);
	strcpy(m_pDBAdmin->szPW,szPassword);
	strcpy(m_pDBClient->szDBName,DbnameC);
	strcpy(m_pDBClient->szID,szIDC);
	strcpy(m_pDBClient->szPW,szPasswordC);

	return InitDBConnection();
}
////////////////////////////////////////////////////////////////////////////////////////////
// ������ �����ϰ� �ʱ�ȭ�Ѵ�.
// ���� ���� ������ �Լ� 
////////////////////////////////////////////////////////////////////////////////////////////
char* CDB::InitDBConnection()
{
	///////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	int a=0;	


	RETCODE		retCode;
	int			preceedCode = 0;

	// ȯ�� �ڵ� ���� 
	retCode = SQLAllocHandle(SQL_HANDLE_ENV,SQL_NULL_HANDLE,&m_hEnv);
	if (SQLOK(retCode))
		preceedCode = 1;
	else 
	{
		m_pWriteMessage("Can't Init SQL Env" );
		goto FAIL;
	}

	///////////////////////////////////////////////////////////////////////////
	a = 100;
	//////////////////////////////////////////////////////////////////////////

	// ȯ�� ���� ���� 
	retCode = SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0); 
	if (SQLOK(retCode))
		preceedCode = 2;
	else 
	{
		m_pWriteMessage("Can't Set Attribute SQL Env" );
		goto FAIL;
	}
	
	// ������ ���� Admin ������ Client ���� �ΰ��� Ŀ�ؼ� �ڵ��� ����
	retCode = SQLAllocHandle(SQL_HANDLE_DBC,m_hEnv,&(m_pDBAdmin->hDBC));
	if (SQLOK(retCode))
		preceedCode = 3;
	else 
	{
		m_pWriteMessage("Can't Init Connect Handle for Admin" );
		goto FAIL;
	}
	
	// ������ ���� Admin ������ Client ���� �ΰ��� Ŀ�ؼ� �ڵ��� ����
	retCode = SQLAllocHandle(SQL_HANDLE_DBC,m_hEnv,&(m_pDBClient->hDBC));
	if (SQLOK(retCode))
		preceedCode = 4;
	else 
	{
		m_pWriteMessage("Can't Init Connect Handle for Client" );
		goto FAIL;
	}
	// ���� Ÿ�Ӿƿ� ����( 5�� ���� ����)
	retCode = SQLSetConnectAttr(m_pDBAdmin->hDBC, 3,(void*)SQL_ATTR_LOGIN_TIMEOUT, 0);
	retCode = SQLSetConnectAttr(m_pDBClient->hDBC, 3,(void*)SQL_ATTR_LOGIN_TIMEOUT, 0);
	
	// ���� SQL ������ ������ �õ� 
	retCode = SQLConnect(m_pDBAdmin->hDBC, (UCHAR *)m_pDBAdmin->szDBName, SQL_NTS, (UCHAR *)m_pDBAdmin->szID, SQL_NTS, (UCHAR *)m_pDBAdmin->szPW, SQL_NTS);

	if (SQLOK(retCode))
		preceedCode = 5;
	else 
	{
		m_pWriteMessage("Can't Connect to SQL server for Admin ID" );
		goto FAIL;
	}
	retCode = SQLConnect(m_pDBClient->hDBC, (UCHAR *)m_pDBClient->szDBName, SQL_NTS, (UCHAR *)m_pDBClient->szID, SQL_NTS, (UCHAR *)m_pDBClient->szPW, SQL_NTS);

	if (SQLOK(retCode))
		preceedCode = 6;
	else 
	{
		m_pWriteMessage("Can't Connect to SQL server for Clinet ID" );
		goto FAIL;
	}

	retCode = SQLAllocHandle(SQL_HANDLE_STMT, m_pDBAdmin->hDBC, &(m_pDBAdmin->hStmt));
	if (SQLOK(retCode))
		preceedCode = 7;
	else 
	{
		m_pWriteMessage("Can't Set STMT for Admin ID" );
		goto FAIL;
	}

	retCode = SQLAllocHandle(SQL_HANDLE_STMT, m_pDBClient->hDBC, &(m_pDBClient->hStmt));
	if (SQLOK(retCode))
		preceedCode = 8;
	else 
	{
		m_pWriteMessage("Can't Set STMT for Client ID" );
		goto FAIL;
	}

	retCode = SQLAllocHandle(SQL_HANDLE_STMT, m_pDBAdmin->hDBC, &(m_pDBAdmin->hScrollStmt));
	if (SQLOK(retCode))
		preceedCode = 9;
	else 
	{
		m_pWriteMessage("Can't Set STMT for Admin ID" );
		goto FAIL;
	}

	retCode = SQLAllocHandle(SQL_HANDLE_STMT, m_pDBClient->hDBC, &(m_pDBClient->hScrollStmt));
	if (SQLOK(retCode))
		preceedCode = 10;
	else 
	{
		m_pWriteMessage("Can't Set STMT for Client ID" );
		goto FAIL;
	}
	if (m_bUsingThread)
		InitDBThread();
	
	m_SucceedInit = TRUE;
	return (char*) 1;

FAIL:
	if (retCode == SQL_INVALID_HANDLE)
		m_pWriteMessage("SQL_INVALID_HANDLE");
	// ���� �޼��� �ذ� 
	switch(preceedCode)
	{
	case 0:
	case 1:
		DisplaySQLError(m_pDBAdmin);
		break;
	case 2:
	case 4:
	case 6:
	case 8:
		DisplaySQLError(m_pDBAdmin);
		break;
	case 3:
	case 5:
	case 7:
	case 9:
		DisplaySQLError(m_pDBClient);
		break;
	}

	return NULL;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
// ���ο� Ŀ�ؼ��� �߰��Ѵ�.
// ������ Ŀ�ؼ� �̿��� Ŀ�ؼ��� �ʿ��� ��� �� �Լ��� �����Ѵ�.
// ���� : �Ҵ�� Ŀ�ؼ��� ������.
// ���� : NULL 
//////////////////////////////////////////////////////////////////////////////////////////////////////
LPDBCON	CDB::MakeNewConnection(LPSTR lpDbName, LPSTR lpID, LPSTR lpPW)
{
	if (m_NumConnection == MAX_NUM_SQL_CONNECTION) return NULL;

	LPDBCON lpTemp = NULL;
	lpTemp = new DBCON;
	if (lpTemp == NULL) return NULL;
	m_NumConnection++;
	memset(lpTemp,0,sizeof(DBCON));

	for (int k=0;k<MAX_NUM_SQL_CONNECTION;k++)
		if (m_ArrayCon[k] == NULL) 
		{
			m_ArrayCon[k] = lpTemp;
			break;
		}

	strcpy(lpTemp->szDBName,lpDbName);
	strcpy(lpTemp->szID,lpID);
	strcpy(lpTemp->szPW,lpPW);

	RETCODE		retCode;
	
	retCode = SQLAllocHandle(SQL_HANDLE_DBC,m_hEnv,&(lpTemp->hDBC));
	if (!SQLOK(retCode))
	{
		m_pWriteMessage("Can't Init Connect Handle" );
		goto FAIL;
	}
	// ���� Ÿ�Ӿƿ� ����( 5�� ���� ����)
	retCode = SQLSetConnectAttr(lpTemp->hDBC, 3,(void*)SQL_ATTR_LOGIN_TIMEOUT, 0);
	
	// ���� SQL ������ ������ �õ� 
	retCode = SQLConnect(lpTemp->hDBC, (UCHAR *)lpTemp->szDBName, SQL_NTS, (UCHAR *)lpTemp->szID, SQL_NTS, (UCHAR *)lpTemp->szPW, SQL_NTS);
	if (!SQLOK(retCode))
	{
		m_pWriteMessage("Can't Connect to SQL server" );
		goto FAIL;
	}
	retCode = SQLAllocHandle(SQL_HANDLE_STMT, lpTemp->hDBC, &(lpTemp->hStmt));
	if (!SQLOK(retCode))
	{
		m_pWriteMessage("Can't Set STMT" );
		goto FAIL;
	}
	retCode = SQLAllocHandle(SQL_HANDLE_STMT, lpTemp->hDBC, &(lpTemp->hScrollStmt));
	if (!SQLOK(retCode))
	{
		m_pWriteMessage("Can't Set STMT" );
		goto FAIL;
	}

	return lpTemp;

FAIL:
	if (retCode == SQL_INVALID_HANDLE)
		m_pWriteMessage("SQL_INVALID_HANDLE");
	DisplaySQLError(lpTemp);
	CloseThisConnection(lpTemp);
	return NULL;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
// MakeNewConnection�� ���� �߰��� � Ŀ�ؼ��� �����Ѵ�.
//////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDB::CloseThisConnection(LPDBCON pCon)
{
	if (m_NumConnection == 0) return false;
	if (pCon == NULL) return false;
	
	SQLCloseCursor(pCon->hStmt);
	SQLFreeHandle(SQL_HANDLE_STMT,pCon->hStmt);
	SQLDisconnect(pCon->hDBC);
	SQLFreeHandle(SQL_HANDLE_DBC, pCon->hDBC);
	delete pCon;
	pCon = NULL;
	m_NumConnection--;
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
// MakeNewConnection�� ���� �߰��� ��� Ŀ�ؼ��� �����Ѵ�.
//////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDB::CloseAllCustomConnection()
{
	if (m_NumConnection == 0) return true;
	BOOL ret;
	for (int k=0;k<MAX_NUM_SQL_CONNECTION;k++)
		if (m_ArrayCon[k] != NULL) 
		{
			ret = CloseThisConnection(m_ArrayCon[k]);
			if(!ret) m_pWriteMessage("CloseAllCustomConnection : Close Error"); 
		}
	if(m_NumConnection) return false;
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// SQL ���� ��� ��ƾ 
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CDB::DisplaySQLError(LPDBCON pCon)
{

	UCHAR 	szSqlState[MSG_BUF_SIZE];
	UCHAR	szErrorMsg[MSG_BUF_SIZE];
	
	SQLINTEGER	fNativeError	= 0;
	SWORD		cbErrorMsg		= MSG_BUF_SIZE;
	RETCODE		rc;

	rc = SQLError(m_hEnv,
				  pCon ? pCon->hDBC : 0,
				 //0,
				  pCon ? pCon->hStmt : 0,
				  szSqlState,
				  &fNativeError,
				  szErrorMsg,
				  MSG_BUF_SIZE,
				  &cbErrorMsg
				  );

	if (rc != SQL_NO_DATA_FOUND || rc != SQL_ERROR)
	{
		if (fNativeError != 0x1645)	// ignore change database to master context message
		{
			char Buffer[256];
			wsprintf(Buffer,"Error Message: %s SqlState: %s, fNativeError: %d",szErrorMsg, szSqlState,fNativeError);
			m_pWriteMessage(Buffer);
		}
	}
	else
	{	
		m_pWriteMessage("SQLError() failed: , NO_DATA_FOUND OR SQL_ERROR");
	}

}
///////////////////////////////////////////////////////////////////////////////////////////////////////
// SQL ���� ��� ��ƾ 
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CDB::DisplaySQLError(LPDBCON pCon, HSTMT hStmt)
{

	UCHAR 	szSqlState[MSG_BUF_SIZE];
	UCHAR	szErrorMsg[MSG_BUF_SIZE];
	
	SQLINTEGER	fNativeError	= 0;
	SWORD		cbErrorMsg		= MSG_BUF_SIZE;
	RETCODE		rc;

	rc = SQLError(m_hEnv,
				  pCon ? pCon->hDBC : 0,
				  pCon ? hStmt : 0,
				  szSqlState,
				  &fNativeError,
				  szErrorMsg,
				  MSG_BUF_SIZE,
				  &cbErrorMsg
				  );

	if (rc != SQL_NO_DATA_FOUND || rc != SQL_ERROR)
	{
		if (fNativeError != 0x1645)	// ignore change database to master context message
		{
			char Buffer[256];
			wsprintf(Buffer,"Error Message: %s SqlState: %s, fNativeError: %d",szErrorMsg, szSqlState,fNativeError);
			m_pWriteMessage(Buffer);
		}
	}
	else
	{	
		m_pWriteMessage("SQLError() failed: , NO_DATA_FOUND OR SQL_ERROR");
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// SQL ������ ������ �޾ƿ´�.
// ���� : Ŀ�ؼ� ������, ������ ����Ÿ��, ������ ���� ������
// �������θ� ���� 
/////////////////////////////////////////////////////////////////////////////////////////////////////// 
SQLRETURN CDB::GetSQLInfo(LPDBCON pCon, SQLUSMALLINT InfoType, SQLUINTEGER fFuncs)
{
	SQLRETURN ret;

	ret = SQLGetInfo(pCon->hDBC,
			InfoType,
			(SQLPOINTER)&fFuncs,
			sizeof(fFuncs),
			NULL);
	return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////
// ������ �ݰ� �ڵ��� ��ȯ
// ���ø����̼� ���� ���� ����  
////////////////////////////////////////////////////////////////////////////////////////////
void	CDB::CloseDBConnection()
{
	if(m_bUsingThread)
		CloseDBThread();
	
	if (CloseAllCustomConnection() == false)
		m_pWriteMessage("Some Error at Counting Custom Connection");

	if (m_SucceedInit)
	{
		if (m_pDBClient->hStmt) SQLCloseCursor(m_pDBClient->hStmt);
		if (m_pDBAdmin->hStmt) SQLCloseCursor(m_pDBAdmin->hStmt);
		
		// ���� �ڵ� ��ȯ
		if (m_pDBClient->hStmt) SQLFreeHandle(SQL_HANDLE_STMT,m_pDBClient->hStmt);
		if (m_pDBAdmin->hStmt) SQLFreeHandle(SQL_HANDLE_STMT,m_pDBAdmin->hStmt);

		// ���� ���� ����
		SQLDisconnect(m_pDBAdmin->hDBC);
		SQLDisconnect(m_pDBClient->hDBC);

		// �ڵ� ��ȯ 
		SQLFreeHandle(SQL_HANDLE_DBC, m_pDBClient->hDBC);
		SQLFreeHandle(SQL_HANDLE_DBC, m_pDBAdmin->hDBC);
		SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////
// ���ο� ���ڵ带 �߰��Ѵ�.
// ����
// Ŀ�ؼ�, ���̺� �̸�, ������Į���� �̸�, ���� ���� ����ü�� ������, Į������ 
//	sprintf(Buffer, "INSERT INTO %s (FirstName, LastName) VALUES ('Cool','Fire')", szTableName);
////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDB::InsertRecord(LPDBCON pCon, LPSTR szTableName, LPVOID lpCol, LPVOID lpValue, BYTE bNumData)
{
	RETCODE		retCode;
	
	if (pCon == NULL) return 0;
	if (lpCol == NULL || lpValue == NULL || bNumData == 0) return 0;
	
	char Buffer[MAX_LENGTH_QUERY];
	switch(bNumData)
	{
	case 1:
	{
		LPDATA1 pCol = (LPDATA1) lpCol;
		LPDATA1 pValue = (LPDATA1) lpValue;
		sprintf(Buffer, "INSERT INTO %s (%s) VALUES ('%s')", szTableName,pCol->Data1, pValue->Data1);
		break;
	}
	case 2:
	{
		LPDATA2 pCol = (LPDATA2) lpCol;
		LPDATA2 pValue = (LPDATA2) lpValue;
		sprintf(Buffer, "INSERT INTO %s (%s,%s) VALUES ('%s', '%s')", szTableName,pCol->Data1,pCol->Data2, pValue->Data1,pValue->Data2);
		break;
	}
	case 3:
	{
		LPDATA3 pCol = (LPDATA3) lpCol;
		LPDATA3 pValue = (LPDATA3) lpValue;
		sprintf(Buffer, "INSERT INTO %s (%s, %s, %s ) VALUES ('%s','%s','%s')", szTableName,pCol->Data1,pCol->Data2,pCol->Data3,pValue->Data1,pValue->Data2,pValue->Data3);
		break;	
	}
	case 4:
	{
		LPDATA4 pCol = (LPDATA4) lpCol;
		LPDATA4 pValue = (LPDATA4) lpValue;
		sprintf(Buffer, "INSERT INTO %s (%s, %s, %s, %s ) VALUES ('%s','%s','%s','%s')", szTableName,pCol->Data1,pCol->Data2,pCol->Data3,pCol->Data4, pValue->Data1,pValue->Data2,pValue->Data3,pValue->Data4);
		break;	
	}
	case 5:
	{
		LPDATA5 pCol = (LPDATA5) lpCol;
		LPDATA5 pValue = (LPDATA5) lpValue;
		sprintf(Buffer, "INSERT INTO %s (%s, %s, %s, %s, %s ) VALUES ('%s','%s','%s','%s','%s')", szTableName,pCol->Data1,pCol->Data2,pCol->Data3,pCol->Data4,pCol->Data5, pValue->Data1,pValue->Data2,pValue->Data3,pValue->Data4,pValue->Data5);
		break;	
	}
	case 6:
	{
		LPDATA6 pCol = (LPDATA6) lpCol;
		LPDATA6 pValue = (LPDATA6) lpValue;
		sprintf(Buffer, "INSERT INTO %s (%s, %s, %s, %s, %s, %s ) VALUES ('%s','%s','%s','%s','%s','%s')", szTableName,pCol->Data1,pCol->Data2,pCol->Data3,pCol->Data4,pCol->Data5,pCol->Data6, pValue->Data1,pValue->Data2,pValue->Data3,pValue->Data4,pValue->Data5,pValue->Data6);
		break;	
	}
	case 7:
	{
		LPDATA7 pCol = (LPDATA7) lpCol;
		LPDATA7 pValue = (LPDATA7) lpValue;
		sprintf(Buffer, "INSERT INTO %s (%s, %s, %s, %s, %s, %s, %s) VALUES ('%s','%s','%s','%s','%s','%s','%s')", szTableName,pCol->Data1,pCol->Data2,pCol->Data3,pCol->Data4,pCol->Data5,pCol->Data6, pCol->Data7, pValue->Data1,pValue->Data2,pValue->Data3,pValue->Data4,pValue->Data5,pValue->Data6, pValue->Data7);
		break;	
	}
	case 8:
	{
		LPDATA8 pCol = (LPDATA8) lpCol;
		LPDATA8 pValue = (LPDATA8) lpValue;
		sprintf(Buffer, "INSERT INTO %s (%s, %s, %s, %s, %s, %s, %s, %s) VALUES ('%s','%s','%s','%s','%s','%s','%s','%s')", szTableName,pCol->Data1,pCol->Data2,pCol->Data3,pCol->Data4,pCol->Data5,pCol->Data6, pCol->Data7,pCol->Data8, pValue->Data1,pValue->Data2,pValue->Data3,pValue->Data4,pValue->Data5,pValue->Data6, pValue->Data7,pValue->Data8);
		break;	
	}
	case 9:
	{
		LPDATA9 pCol = (LPDATA9) lpCol;
		LPDATA9 pValue = (LPDATA9) lpValue;
		sprintf(Buffer, "INSERT INTO %s (%s, %s, %s, %s, %s, %s, %s, %s,%s) VALUES ('%s','%s','%s','%s','%s','%s','%s','%s','%s')", szTableName,pCol->Data1,pCol->Data2,pCol->Data3,pCol->Data4,pCol->Data5,pCol->Data6, pCol->Data7,pCol->Data8,pCol->Data9, pValue->Data1,pValue->Data2,pValue->Data3,pValue->Data4,pValue->Data5,pValue->Data6, pValue->Data7,pValue->Data8,pValue->Data9);
		break;	
	}
	
	case 10:
	{
		LPDATA10 pCol = (LPDATA10) lpCol;
		LPDATA10 pValue = (LPDATA10) lpValue;
		sprintf(Buffer, "INSERT INTO %s (%s, %s, %s, %s, %s, %s, %s, %s,%s,%s) VALUES ('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')", szTableName,pCol->Data1,pCol->Data2,pCol->Data3,pCol->Data4,pCol->Data5,pCol->Data6, pCol->Data7,pCol->Data8,pCol->Data9,pCol->Data10,  pValue->Data1,pValue->Data2,pValue->Data3,pValue->Data4,pValue->Data5,pValue->Data6, pValue->Data7,pValue->Data8,pValue->Data9, pValue->Data10);
		break;	
	}
	
	}
	
	if (pCon->hStmt == NULL)
	{
		retCode = SQLAllocHandle(SQL_HANDLE_STMT, pCon->hDBC, &(pCon->hStmt));
		if(retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO)
			DisplaySQLError(pCon);
	}

	retCode = SQLExecDirect(pCon->hStmt, (UCHAR *)Buffer, SQL_NTS);
	if (SQLOK(retCode))
	{
		return 1;
	}
	else
		DisplaySQLError(pCon);
	
//	pCon->hStmt = NULL;
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////
// ���ڵ带 �����Ѵ�
// ����
// Ŀ�ؼ�, ���̺� �̸�, Į���̸� , ��
// lpCol == lpValue �� ���ڵ带 �����Ѵ�.
////////////////////////////////////////////////////////////////////////////////////////////
BOOL	CDB::DeleteRecord(LPDBCON pCon, LPSTR szTableName, LPSTR lpCol, LPSTR lpValue)
{
	RETCODE		retCode;
	
	if (pCon == NULL) return 0;
	if (lpCol == NULL || lpValue == NULL) return 0;
	char Buffer[MAX_LENGTH_QUERY];

	sprintf(Buffer, "DELETE FROM %s where %s = '%s'",szTableName,lpCol,lpValue);

	if (pCon->hStmt == NULL)
	{
		retCode = SQLAllocHandle(SQL_HANDLE_STMT, pCon->hDBC, &(pCon->hStmt));
		if(retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO)
			DisplaySQLError(pCon);
	}

	retCode = SQLExecDirect(pCon->hStmt, (UCHAR *)Buffer, SQL_NTS);
	if (SQLOK(retCode))
	{
		return 1;
	}
	else
		DisplaySQLError(pCon);

//	pCon->hStmt = NULL;
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////
// ���ڵ带 �����Ѵ�
// ����
// Ŀ�ؼ�, ���̺� �̸�, Į���̸� , ��
// lpCol == lpValue �� ���ڵ带 ã�Ƽ� DestCol Į���� ���� DestValue�� �����Ѵ�.
// ���� lpCol�� NULL �� ��� ��� ���ڵ��� DestCol Į�� ���� DestValue�� �����Ѵ�.
////////////////////////////////////////////////////////////////////////////////////////////
BOOL	CDB::UpdateRecord(LPDBCON pCon, LPSTR szTableName, LPSTR lpCol, LPSTR lpValue, LPSTR DestCol, LPSTR	DestValue)
{
	RETCODE		retCode;
	
	if (pCon == NULL) return 0;

	char Buffer[MAX_LENGTH_QUERY];
	if (lpCol == NULL)
		sprintf(Buffer, "Update %s set %s = '%s'",szTableName,DestCol, DestValue);
	else
		sprintf(Buffer, "Update %s set %s = '%s' where %s = '%s'",szTableName,DestCol, DestValue, lpCol,lpValue);

	if (pCon->hStmt == NULL)
	{
		retCode = SQLAllocHandle(SQL_HANDLE_STMT, pCon->hDBC, &(pCon->hStmt));
		if(retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO)
			DisplaySQLError(pCon);
	}

	retCode = SQLExecDirect(pCon->hStmt, (UCHAR *)Buffer, SQL_NTS);
	if (SQLOK(retCode))
	{
		return 1;
	}
	else
	{
		switch(retCode)
		{
			case 	SQL_NO_DATA:
				m_pWriteMessage("Updat Fail : No Data");
				break;
			case 	SQL_SUCCESS_WITH_INFO:	
				break;
			case 	SQL_SUCCESS:	
				break;
			case 	SQL_STILL_EXECUTING:	
				break;
			case 	SQL_NEED_DATA:	
				break;
			case 	SQL_ERROR:
			case 	SQL_INVALID_HANDLE:	
			default:
				DisplaySQLError(pCon);
		}
	}

//	pCon->hStmt = NULL;
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////
// Update, Delete ���ν��� ���� �Լ�.
// ���ϴ� ������ ���� ��� ���� �������� �ۼ�
// ������ ���ڰ� �� �پ��� ������ �����ټ��� ����..^^;;;
// �� select ���� �Ʒ� �ٸ� �����Լ��� �̿��� ��
////////////////////////////////////////////////////////////////////////////////////////////
BOOL	CDB::ExcuteIUDForProcedure(LPDBCON pCon, LPSTR szQuery)
{
	RETCODE		retCode;
	
	if (pCon == NULL) return 0;

	char Buffer[MAX_LENGTH_QUERY];
	strcpy(Buffer,szQuery);


	if (pCon->hStmt == NULL)
	{
		retCode = SQLAllocHandle(SQL_HANDLE_STMT, pCon->hDBC, &(pCon->hStmt));
		if(retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO)
			DisplaySQLError(pCon);
	}
	HSTMT	hStmt;		
	retCode = SQLAllocHandle(SQL_HANDLE_STMT, pCon->hDBC, &hStmt);
	if(retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO)
		DisplaySQLError(pCon);

	retCode = SQLExecDirect(hStmt, (UCHAR *)Buffer, SQL_NTS);
//	SQLFreeHandle(SQL_HANDLE_STMT,hStmt);
//	return retCode;

	if (SQLOK(retCode))
	{
		SQLFreeHandle(SQL_HANDLE_STMT,hStmt);
		return 1;
	}
	else
	{
		switch(retCode)
		{
			case 	SQL_NO_DATA:
				m_pWriteMessage("ExcuteIUD Fail : No Data");
				break;
			case 	SQL_SUCCESS_WITH_INFO:	
				break;
			case 	SQL_SUCCESS:	
				break;
			case 	SQL_STILL_EXECUTING:	
				break;
			case 	SQL_NEED_DATA:	
				break;
			case 	SQL_ERROR:
			case 	SQL_INVALID_HANDLE:	
			default:
				DisplaySQLError(pCon);
		}
	}
	SQLFreeHandle(SQL_HANDLE_STMT,hStmt);

	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////
// Insert, Update, Delete �� ���� �Լ�
// ���ϴ� ������ ���� ��� ���� �������� �ۼ�
// ������ ���ڰ� �� �پ��� ������ �����ټ��� ����..^^;;;
// �� select ���� �Ʒ� �ٸ� �����Լ��� �̿��� ��
////////////////////////////////////////////////////////////////////////////////////////////
BOOL	CDB::ExcuteIUD(LPDBCON pCon, LPSTR szQuery)
{
	RETCODE		retCode;
	
	if (pCon == NULL) return 0;

	char Buffer[MAX_LENGTH_QUERY];
	strcpy(Buffer,szQuery);

	if (pCon->hStmt == NULL)
	{
		retCode = SQLAllocHandle(SQL_HANDLE_STMT, pCon->hDBC, &(pCon->hStmt));
		if(retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO)
			DisplaySQLError(pCon);
	}


	retCode = SQLExecDirect(pCon->hStmt, (UCHAR *)Buffer, SQL_NTS);
	if (SQLOK(retCode))
	{
		return 1;
	}
	else
	{
		switch(retCode)
		{
			case 	SQL_NO_DATA:
				m_pWriteMessage("ExcuteIUD Fail : No Data");
				break;
			case 	SQL_SUCCESS_WITH_INFO:	
				break;
			case 	SQL_SUCCESS:	
				break;
			case 	SQL_STILL_EXECUTING:	
				break;
			case 	SQL_NEED_DATA:	
				break;
			case 	SQL_ERROR:
			case 	SQL_INVALID_HANDLE:	
			default:
				DisplaySQLError(pCon);
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////
// ���� �α� �Լ� 
// ���ڰ� ���Ƽ� ������ ������ �����̱� �����̴�.
// ������ SimpleLogin �Լ� ���� 
// ���ڼ��� 
// Ŀ�ؼ�, ���̺� �̸�, IDĮ���̸� , PWĮ���̸�, ����ڰ� �Է��� ���̵�, �н�����, ���ϰ��� ã�Ƴ� Į���̸�, ���ϰ��� ���� ������, ���ϰ��� ����, ���ϰ��� ����
// ���̵�� �н����尡 ��ġ�ϴ� ���ڵ带 �˻��� �ش� ���ڵ尡 �ִ� ���
// �ش� ���ڵ��� pRetCol Į���� ������ sqlType������ �о iLength���̸�ŭ �о pRetValue�� ��ƺ�����. 
// ���� ��� FASLE ����.
// ���ϰ� ���� Dword: SQL_C_UBIGINT, ��Ʈ�� SQL_C_CHAR �������� ���۷��� ���� 
// ������ ���ÿ��� ó�� Ȯ�θ޼����� �ݵ�� �޾ƾ� �ϴ� �Լ�!
////////////////////////////////////////////////////////////////////////////////////////////
BOOL	CDB::Login(LPDBCON pCon, LPSTR szTableName, LPSTR pColID, LPSTR pColPW, LPSTR pID, LPSTR pPW, LPSTR pRetCol, LPVOID pRetValue, int iLength, SQLSMALLINT sqlType)
{
	RETCODE		retCode;
	SDWORD		cbValue;
	
	if (pCon == NULL) return 0;
	if (pPW == NULL || pID == NULL) return 0;
	if (pColPW == NULL || pColID == NULL) return 0;

	char Buffer[MAX_LENGTH_QUERY];

	sprintf(Buffer, "SELECT  %s FROM %s where %s = '%s' AND %s = '%s'",pRetCol, szTableName, pColID, pID, pColPW, pPW);

	if (pCon->hStmt == NULL)
	{
		retCode = SQLAllocHandle(SQL_HANDLE_STMT, pCon->hDBC, &(pCon->hStmt));
		if(retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO)
			DisplaySQLError(pCon);
	}

	retCode = SQLExecDirect(pCon->hStmt, (UCHAR *)Buffer, SQL_NTS);
	if (SQLOK(retCode))
	{
		retCode = SQLFetch(pCon->hStmt);

		if (retCode == SQL_SUCCESS)
		{
			retCode = SQLGetData(pCon->hStmt, 1, sqlType, pRetValue, iLength, &cbValue);
			if (retCode == SQL_SUCCESS && pRetValue> 0)
			{
				SQLFreeStmt(pCon->hStmt,SQL_CLOSE);
				return 1;
			}
			else 
			{
				SQLFreeStmt(pCon->hStmt,SQL_CLOSE);
				return 0;
			}
		}
	}
	else
		DisplaySQLError(pCon);

	SQLFreeStmt(pCon->hStmt,SQL_CLOSE);
//	pCon->hStmt = NULL;
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////
// ���� �α� �Լ� 
// �����ϰ� ���� ����ڰ� �Է��� �����̸��� ����� �˻��� ������ �� ������ ����dword���̵� �����Ϳ� �־��ش�.
// �� ��Ģ�� ���� �ʰ� DB�� ����� ��� ���� ���� �α��Լ��� �� ��.
// ���ڼ��� 
// Ŀ�ؼ�, ����ڰ� �Է��� ���̵�, �н�����, ���ϰ��� ���� ������
// ���̵�� �н����尡 ��ġ�ϴ� ���ڵ带 �˻��� �ش� ���ڵ尡 �ִ� ���
// �ش� ���ڵ��� DEFAULT_USER_GUID_COLĮ���� dwUserId���� pRetValue�� ��� �Ѱ��ش�.
// ���� ��� FASLE ����.
// ���ϰ� ���� Dword: SQL_C_UBIGINT, ��Ʈ�� SQL_C_CHAR �������� ���۷��� ���� 
////////////////////////////////////////////////////////////////////////////////////////////
BOOL	CDB::SimpleLogin(LPDBCON pCon,LPSTR pID, LPSTR pPW, LPVOID pRetValue)
{
	RETCODE		retCode;
	SDWORD		cbValue;
	
	if (pCon == NULL) return 0;
	if (pPW == NULL || pID == NULL) return 0;

	char Buffer[MAX_LENGTH_QUERY];

	sprintf(Buffer, "SELECT  %s FROM %s where %s = '%s' AND %s = '%s'",m_szDefault_User_guid, m_szDefault_User_table, m_szDefault_User_account, pID, m_szDefault_User_pw, pPW);

	if (pCon->hStmt == NULL)
	{
		retCode = SQLAllocHandle(SQL_HANDLE_STMT, pCon->hDBC, &(pCon->hStmt));
		if(retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO)
			DisplaySQLError(pCon);
	}

	retCode = SQLExecDirect(pCon->hStmt, (UCHAR *)Buffer, SQL_NTS);
	if (SQLOK(retCode))
	{
		retCode = SQLFetch(pCon->hStmt);

		if (retCode == SQL_SUCCESS)
		{
			retCode = SQLGetData(pCon->hStmt, 1, SQL_C_UBIGINT, pRetValue, 4, &cbValue);
			if (retCode == SQL_SUCCESS && pRetValue> 0)
			{
				SQLFreeStmt(pCon->hStmt,SQL_CLOSE);
				return 1;
			}
			else 
			{
				SQLFreeStmt(pCon->hStmt,SQL_CLOSE);
				return 0;
			}
		}
	}
	else
		DisplaySQLError(pCon);

	SQLFreeStmt(pCon->hStmt,SQL_CLOSE);
//	pCon->hStmt = NULL;
	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////
// �������� �Լ�(SELECT�� ����)
// ���ڼ��� 
// Ŀ�ؼ�, ������, ��ȯ�Ǵ� ���ڵ��� ���ڸ� ���� DWORD�� ������
// "select City, FirstName from employees" ���� ������ �������� ����� �־��ش�.
// �� �Լ��� define���� ������ �޸𸮸� ��� ������ ����� ���� ����ü�� �����͸� �����Ѵ�.
// ����Į�� ���� ����
// ����!!!!
// ���ο��� �޸𸮸� ��Ƽ� �����͸� �ѱ�Ƿ� �޾Ƽ� ����� �� �� �޸𸮸� Ǯ����� �Ѵ�.!!!!!!!!!!!!!!
// ���������� VK_F4 ���� ����..(��뿹)
// �̰��� �̸��� Column -Wise Binding ����̴�.
////////////////////////////////////////////////////////////////////////////////////////////

LPQUERY FAR PASCAL CDB::FreeQuery(LPDBCON pCon,LPSTR szBuffer, LPDWORD pNumRow)
{
	SWORD     nCols = 0;                  // # of result columns
	SWORD     nCount;                     // index
	SQLRETURN nReturn;                    // return code
	
	if (pCon->hStmt == NULL)
	{
		nReturn = SQLAllocHandle(SQL_HANDLE_STMT, pCon->hDBC, &(pCon->hStmt));
		if(nReturn != SQL_SUCCESS && nReturn != SQL_SUCCESS_WITH_INFO)
		{
			m_pWriteMessage(szBuffer);
			DisplaySQLError(pCon);
		}
	}

  // execute SQL and process errors if any
   // call SQLExecDirect to execute the SQL statement
   nReturn = SQLExecDirect(pCon->hStmt, (UCHAR *)szBuffer, SQL_NTS);
   if (nReturn != SQL_SUCCESS) 
   {
	    m_pWriteMessage(szBuffer);
		DisplaySQLError(pCon);
      // if the return value was just information, carry on
      if (nReturn != SQL_SUCCESS_WITH_INFO) 
	  {
         // Close the open result set.
         SQLCloseCursor(pCon->hStmt);
         return (LPQUERY) NULL;
      }
   }

   // call SQLNumResultCols to calculate the number of columns in
   // the result set, if more than the MAX_COL (the array boundary)
   // limit the number to MAX_COL and display truncation warning.
   // if it is 0, the statement probably was a non-SELECT simply return

   nReturn = SQLNumResultCols(pCon->hStmt, &nCols);
   if (nCols >= MAX_COL_NUM) 
   {
      nCols = MAX_COL_NUM;
   }
   else if (nCols == 0) 
   {
      // Close the open result set.
      SQLCloseCursor(pCon->hStmt);
      return (LPQUERY) NULL;
   }
	// ����Ÿ�� ��� ���� �޸𸮸� �Ҵ��Ѵ�.
	QUERYST stData;
	memset(&stData,0,sizeof(QUERYST));
//	for (int k=0;k<MAX_COL_NUM;k++)
//		stData.Length[k] = 0;
	stData.nNum = nCols;
	
//	LPQUERY RealData = new QUERYST[MAX_ROW_NUM];
	LPQUERY RealData = (LPQUERY) m_pResultMemory->Alloc();
	if(!RealData) 
	{
		m_pWriteMessage("FreeQuery : Fail to alloc memory");
		return (LPQUERY) NULL;
	}
	memset(RealData,0,MAX_ROW_NUM*sizeof(QUERYST));
   // call SQLBindCol to bind column data array and column data length
   // array to the result set
   for(nCount=0; nCount<nCols; nCount++)
      SQLBindCol(pCon->hStmt, (UWORD)(nCount+1), SQL_C_CHAR, stData.Data[nCount], MAX_COL_LENGTH, &(stData.Length[nCount]));


/* // call SQLDescribeCol to get description of each column in the
   // result set. Store the column name in the display buffer and
   // make it the first entry in the results list box of the MDI
   // child window.
   for(nCount=1; nCount <= nCols; nCount++)
   {
      SQLDescribeCol(pCon->hStmt, nCount, szBuffer, MAX_DATA_LEN, &swColLength, &swColType, &udwColDef,
                     &swColScale, &swColNull);
   }
*/
   // call SQLFetch to fetch each row of the result set. Extract data
   // value and store it in the display buffer, separated by tabs.
   // continue until SQLFetch fails.
   DWORD nCountRow = 0;
   for(; (nReturn = SQLFetch(pCon->hStmt))==SQL_SUCCESS || nReturn==SQL_SUCCESS_WITH_INFO;)
   {
      
	   if (nCountRow == MAX_ROW_NUM) break;
	   memcpy(&(RealData[nCountRow]),&stData,sizeof(QUERYST));
	   nCountRow++;
	   if (nReturn != SQL_SUCCESS)
	   {
			m_pWriteMessage(szBuffer);
			DisplaySQLError(pCon);
	   }
   }

   // if there was any error returned by SQLFetch, display it
   if (nReturn == SQL_ERROR)
   {
		m_pWriteMessage(szBuffer);
		DisplaySQLError(pCon);
   }

   // Close the open result set.
   // Unbind all bound columns.
	SQLCloseCursor(pCon->hStmt);
	SQLFreeStmt(pCon->hStmt, SQL_UNBIND);
	SQLFreeStmt(pCon->hStmt, SQL_RESET_PARAMS);
//	pCon->hStmt = NULL;

   memcpy(pNumRow,&nCountRow,4);
   return RealData;
}

////////////////////////////////////////////////////////////////////////////////////////////
// ����Middle���� �Լ�(SELECT�� ����)
// ���ڼ��� 
// Ŀ�ؼ�, ������, ��ȯ�Ǵ� ���ڵ��� ���ڸ� ���� DWORD�� ������
// ���������� ���� ������ �߰�. �����������ٴ� ��� ���� �������ٴ� ª�� ���� �޾ƿ´�. 
////////////////////////////////////////////////////////////////////////////////////////////
LPMIDDLEQUERY FAR PASCAL CDB::FreeMiddleQuery(LPDBCON pCon,LPSTR szBuffer, LPDWORD pNumRow)
{
	SWORD     nCols = 0;                  // # of result columns
	SWORD     nCount;                     // index
	SQLRETURN nReturn;                    // return code
	
	if (pCon->hStmt == NULL)
	{
		nReturn = SQLAllocHandle(SQL_HANDLE_STMT, pCon->hDBC, &(pCon->hStmt));
		if(nReturn != SQL_SUCCESS && nReturn != SQL_SUCCESS_WITH_INFO)
		{
			m_pWriteMessage(szBuffer);
			DisplaySQLError(pCon);
		}
	}

  // execute SQL and process errors if any
   // call SQLExecDirect to execute the SQL statement
   nReturn = SQLExecDirect(pCon->hStmt, (UCHAR *)szBuffer, SQL_NTS);
   if (nReturn != SQL_SUCCESS) 
   {
	    m_pWriteMessage(szBuffer);
		DisplaySQLError(pCon);
      // if the return value was just information, carry on
      if (nReturn != SQL_SUCCESS_WITH_INFO) 
	  {
         // Close the open result set.
         SQLCloseCursor(pCon->hStmt);
         return (LPMIDDLEQUERY) NULL;
      }
   }

   // call SQLNumResultCols to calculate the number of columns in
   // the result set, if more than the MAX_COL (the array boundary)
   // limit the number to MAX_COL and display truncation warning.
   // if it is 0, the statement probably was a non-SELECT simply return

   nReturn = SQLNumResultCols(pCon->hStmt, &nCols);
   if (nCols >= MAX_MIDDLECOL_NUM) 
   {
      nCols = MAX_MIDDLECOL_NUM;
   }
   else if (nCols == 0) 
   {
      // Close the open result set.
      SQLCloseCursor(pCon->hStmt);
      return (LPMIDDLEQUERY) NULL;
   }
	// ����Ÿ�� ��� ���� �޸𸮸� �Ҵ��Ѵ�.
	MIDDLEQUERYST stData;
	memset(&stData,0,sizeof(MIDDLEQUERYST));
//	for (int k=0;k<MAX_COL_NUM;k++)
//		stData.Length[k] = 0;
	stData.nNum = nCols;
	
//	LPQUERY RealData = new QUERYST[MAX_ROW_NUM];
	LPMIDDLEQUERY RealData = (LPMIDDLEQUERY) m_pResultMemory->Alloc();
	if(!RealData) 
	{
		m_pWriteMessage("FreeMiddleQuery : Fail to alloc memory");
		return (LPMIDDLEQUERY) NULL;
	}
	memset(RealData,0,MAX_MIDDLECOL_NUM*sizeof(MIDDLEQUERYST));
   // call SQLBindCol to bind column data array and column data length
   // array to the result set
   for(nCount=0; nCount<nCols; nCount++)
      SQLBindCol(pCon->hStmt, (UWORD)(nCount+1), SQL_C_CHAR, stData.Data[nCount], MAX_MIDDLECOL_LENGTH, &(stData.Length[nCount]));


/* // call SQLDescribeCol to get description of each column in the
   // result set. Store the column name in the display buffer and
   // make it the first entry in the results list box of the MDI
   // child window.
   for(nCount=1; nCount <= nCols; nCount++)
   {
      SQLDescribeCol(pCon->hStmt, nCount, szBuffer, MAX_DATA_LEN, &swColLength, &swColType, &udwColDef,
                     &swColScale, &swColNull);
   }
*/
   // call SQLFetch to fetch each row of the result set. Extract data
   // value and store it in the display buffer, separated by tabs.
   // continue until SQLFetch fails.
   DWORD nCountRow = 0;
   for(; (nReturn = SQLFetch(pCon->hStmt))==SQL_SUCCESS || nReturn==SQL_SUCCESS_WITH_INFO;)
   {
      
	   if (nCountRow == MAX_MIDDLEROW_NUM) break;
	   memcpy(&(RealData[nCountRow]),&stData,sizeof(MIDDLEQUERYST));
	   nCountRow++;
	   if (nReturn != SQL_SUCCESS)
	   {
			m_pWriteMessage(szBuffer);
			DisplaySQLError(pCon);
	   }
   }

   // if there was any error returned by SQLFetch, display it
   if (nReturn == SQL_ERROR)
   {
		m_pWriteMessage(szBuffer);
		DisplaySQLError(pCon);
   }

   // Close the open result set.
   // Unbind all bound columns.
	SQLCloseCursor(pCon->hStmt);
	SQLFreeStmt(pCon->hStmt, SQL_UNBIND);
	SQLFreeStmt(pCon->hStmt, SQL_RESET_PARAMS);
//	pCon->hStmt = NULL;

   memcpy(pNumRow,&nCountRow,4);
   return RealData;
}

////////////////////////////////////////////////////////////////////////////////////////////
// ����Large���� �Լ�(SELECT�� ����)
// ���ڼ��� 
// Ŀ�ؼ�, ������, ��ȯ�Ǵ� ���ڵ��� ���ڸ� ���� DWORD�� ������
// For Large Data(Ex: Board)
////////////////////////////////////////////////////////////////////////////////////////////
LPLARGEQUERY FAR PASCAL CDB::FreeLargeQuery(LPDBCON pCon,LPSTR szBuffer, LPDWORD pNumRow)
{
	SWORD     nCols = 0;                  // # of result columns
	SWORD     nCount;                     // index
	SQLRETURN nReturn;                    // return code
	
	if (pCon->hStmt == NULL)
	{
		nReturn = SQLAllocHandle(SQL_HANDLE_STMT, pCon->hDBC, &(pCon->hStmt));
		if(nReturn != SQL_SUCCESS && nReturn != SQL_SUCCESS_WITH_INFO)
		{			
			m_pWriteMessage(szBuffer);
			DisplaySQLError(pCon);
		}
	}

  // execute SQL and process errors if any
   // call SQLExecDirect to execute the SQL statement
   nReturn = SQLExecDirect(pCon->hStmt, (UCHAR *)szBuffer, SQL_NTS);
   if (nReturn != SQL_SUCCESS) 
   {
		m_pWriteMessage(szBuffer);
		DisplaySQLError(pCon);
      // if the return value was just information, carry on
      if (nReturn != SQL_SUCCESS_WITH_INFO) 
	  {
         // Close the open result set.
         SQLCloseCursor(pCon->hStmt);
         return (LPLARGEQUERY) NULL;
      }
   }

   // call SQLNumResultCols to calculate the number of columns in
   // the result set, if more than the MAX_COL (the array boundary)
   // limit the number to MAX_COL and display truncation warning.
   // if it is 0, the statement probably was a non-SELECT simply return

   nReturn = SQLNumResultCols(pCon->hStmt, &nCols);
   if (nCols >= MAX_LARGECOL_NUM) 
   {
      nCols = MAX_LARGECOL_NUM;
   }
   else if (nCols == 0) 
   {
      // Close the open result set.
      SQLCloseCursor(pCon->hStmt);
      return (LPLARGEQUERY) NULL;
   }
	// ����Ÿ�� ��� ���� �޸𸮸� �Ҵ��Ѵ�.
	LARGEQUERYST stData;
	memset(&stData,0,sizeof(LARGEQUERYST));
	stData.nNum = nCols;
	
	LPLARGEQUERY RealData = (LPLARGEQUERY) m_pResultMemory->Alloc();
	if(!RealData) 
	{
		m_pWriteMessage("FreeLargeQuery : Fail to alloc memory");
		return (LPLARGEQUERY) NULL;
	}
	memset(RealData,0,sizeof(LARGEQUERYST));
   // call SQLBindCol to bind column data array and column data length
   // array to the result set
   for(nCount=0; nCount<nCols; nCount++)
      SQLBindCol(pCon->hStmt, (UWORD)(nCount+1), SQL_C_CHAR, stData.Data[nCount], MAX_LARGECOL_LENGTH, &(stData.Length[nCount]));

/* // call SQLDescribeCol to get description of each column in the
   // result set. Store the column name in the display buffer and
   // make it the first entry in the results list box of the MDI
   // child window.
   for(nCount=1; nCount <= nCols; nCount++)
   {
      SQLDescribeCol(pCon->hStmt, nCount, szBuffer, MAX_DATA_LEN, &swColLength, &swColType, &udwColDef,
                     &swColScale, &swColNull);
   }
*/
   
   // call SQLFetch to fetch each row of the result set. Extract data
   // value and store it in the display buffer, separated by tabs.
   // continue until SQLFetch fails.
   DWORD nCountRow = 0;
   for(; (nReturn = SQLFetch(pCon->hStmt))==SQL_SUCCESS || nReturn==SQL_SUCCESS_WITH_INFO;)
   {
       if (nCountRow == 1) break;		// Large Query can Get Only One Row.  Written By HaeSun
	   memcpy(&(RealData[nCountRow]),&stData,sizeof(LARGEQUERYST));
	   nCountRow++;
	   if (nReturn != SQL_SUCCESS)
		{			
			m_pWriteMessage(szBuffer);
			DisplaySQLError(pCon);
		}
   }

   // if there was any error returned by SQLFetch, display it
   if (nReturn == SQL_ERROR)
	{			
		m_pWriteMessage(szBuffer);
		DisplaySQLError(pCon);
	}

   // Close the open result set.
   // Unbind all bound columns.
	SQLCloseCursor(pCon->hStmt);
	SQLFreeStmt(pCon->hStmt, SQL_UNBIND);
	SQLFreeStmt(pCon->hStmt, SQL_RESET_PARAMS);
//	pCon->hStmt = NULL;

   memcpy(pNumRow,&nCountRow,4);
   return RealData;
}


////////////////////////////////////////////////////////////////////////////////////////////
// ���ε����� �Լ�(SELECT�� ����)
// ���ڼ��� 
// Ŀ�ؼ�, ������, LPBIND�� ������, ������ϴ� ���ڵ尹��(�ִ�), ������ϴ� ���ڵ� ��Ʈ������ ũ��, Ÿ��(��Ʈ��? ����?)
// "select City, FirstName from employees" ���� ������ �������� ����� �־��ش�.
// �� �Լ��� ���� ���������� �޸� ���� ������ϴ� �������� �޸𸮸� ���� �Ŀ�
// �Լ��� ������ �ű⿡ ���� ä���ִ´�.
// �̰��� �̸��� Row -Wise Binding ����̴�.
// ��뿹�� VK_F5 ����
// ���� Į������ �ڷ����� �ٸ� ��쿡�� �̸� LPBIND�� QueryType���ٰ� �� ���� �־�ΰ� 
// sqlType���� NULL�� �־��ָ� �˾Ƽ� �ȴ�.
////////////////////////////////////////////////////////////////////////////////////////////
BOOL	CDB::BindQuery(LPDBCON pCon,LPSTR szBuffer, LPBIND lpBind, int Rowset, int StSize, SQLSMALLINT sqlType)
{
	SQLRETURN retCode;                    // return code
	SQLUINTEGER  NumRowsFetched;
	SQLUSMALLINT RowStatusArray[MAX_ROW_NUM];

	if (pCon->hStmt == NULL)
	{
		retCode = SQLAllocHandle(SQL_HANDLE_STMT, pCon->hDBC, &(pCon->hStmt));
		if(retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO)
			DisplaySQLError(pCon);
	}

	SQLSetStmtAttr(pCon->hStmt, SQL_ATTR_ROW_BIND_TYPE, (SQLPOINTER) StSize, 0);
	SQLSetStmtAttr(pCon->hStmt, SQL_ATTR_ROW_ARRAY_SIZE,(SQLPOINTER) Rowset, 0);
	SQLSetStmtAttr(pCon->hStmt, SQL_ATTR_ROW_STATUS_PTR, RowStatusArray, 0);
	SQLSetStmtAttr(pCon->hStmt, SQL_ATTR_ROWS_FETCHED_PTR, &NumRowsFetched, 0);

	// �޸� ���ε�
	for (int k = 0;k < lpBind->nColNum;k++)
	{
		if (sqlType == NULL)
			SQLBindCol(pCon->hStmt,k+1,lpBind->QueryType[k], ((LPROWWISEBIND) lpBind->pData[0])->Data[k],((LPROWWISEBIND) lpBind->pData[0])->Length[k],&(((LPROWWISEBIND) lpBind->pData[0])->LengthRead[k]));
		else SQLBindCol(pCon->hStmt,k+1,sqlType, ((LPROWWISEBIND) lpBind->pData[0])->Data[k],((LPROWWISEBIND) lpBind->pData[0])->Length[k],&(((LPROWWISEBIND) lpBind->pData[0])->LengthRead[k]));
	}
	// ���� 
	retCode = SQLExecDirect(pCon->hStmt, (UCHAR *)szBuffer, SQL_NTS);
   // ������ ��Ī
	while ((retCode = SQLFetchScroll(pCon->hStmt,SQL_FETCH_NEXT,0)) != SQL_NO_DATA) 
	{
		// �ϰ͵� �Ұ� ����..-_-;
	}
   if (retCode == SQL_ERROR)
		DisplaySQLError(pCon);

	// Close the open result set.
	// Unbind all bound columns.
	retCode  = SQLCloseCursor(pCon->hStmt);
	SQLFreeHandle(SQL_HANDLE_STMT,pCon->hStmt);
//	retCode  = SQLFreeStmt(pCon->hStmt, SQL_RESET_PARAMS);
//	retCode  = SQLFreeStmt(pCon->hStmt, SQL_UNBIND);
	pCon->hStmt = NULL;

	return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////
// �̱� ���� �Լ�
// ���ڼ��� 
// Ŀ�ؼ�, ���̺� �̸�, IDENTITY Į���� ��, ������ Į���� �̸�, ������ Ÿ��
// �� �Լ��� ���������� ���� ���� ���� �� ����.
// ���̺��� IDENTITYĮ���� �����ϴµ� �̰��� ������ ���̴�.(���� ��� �������̵�)
// ���� �� ���� ��ȸ�ؼ� �� �ϳ��� ���ڵ忡 ���� ������ �޾ƿ��� ���̴�.
// lpCol�� NULL�� ������ �ش� ���ڵ��� ��� ������ �����ϰ�
// Ư�� Į���� �����ϸ� �� Į���� ���븸�� �Ѱ��ش�.
// ����Į�� ���� ����
// ����!!!!
// ���ο��� �޸𸮸� ��Ƽ� �����͸� �ѱ�Ƿ� �޾Ƽ� ����� �� �� �޸𸮸� Ǯ����� �Ѵ�.!!!!!!!!!!!!!!
// �̰��� �̸��� Column -Wise Binding ����̴�.
////////////////////////////////////////////////////////////////////////////////////////////
LPQUERY FAR PASCAL CDB::SingleQuery(LPDBCON pCon, LPSTR szTable, DWORD dwValue, LPSTR lpCol, SQLSMALLINT sqltype)
{
	SWORD     nCols = 0;                  // # of result columns
	SWORD     nCount;                     // index
	SQLRETURN retCode;                    // return code
	
 	if (pCon->hStmt == NULL)
	{
		retCode = SQLAllocHandle(SQL_HANDLE_STMT, pCon->hDBC, &(pCon->hStmt));
		if(retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO)
			DisplaySQLError(pCon);
	}
	// ���� ����
	char szBuffer[MAX_LENGTH_QUERY];
	if (lpCol == NULL || *lpCol == 0)
		sprintf(szBuffer,"select * from %s where IDENTITYCOL = '%d' " ,szTable,dwValue);
	else sprintf(szBuffer,"select %s from %s where IDENTITYCOL = '%d' " ,lpCol, szTable, dwValue);

  // execute SQL and process errors if any
   // call SQLExecDirect to execute the SQL statement
   retCode = SQLExecDirect(pCon->hStmt, (UCHAR *)szBuffer, SQL_NTS);
   if (retCode != SQL_SUCCESS) 
   {
		DisplaySQLError(pCon);
      // if the return value was just information, carry on
      if (retCode != SQL_SUCCESS_WITH_INFO) 
	  {
         // Close the open result set.
         SQLCloseCursor(pCon->hStmt);
         return (LPQUERY) NULL;
      }
   }

   retCode = SQLNumResultCols(pCon->hStmt, &nCols);
   if (nCols >= MAX_COL_NUM) 
   {
	  nCols = MAX_COL_NUM;
   }
   else if (nCols == 0) 
   {
	  // Close the open result set.
	  SQLCloseCursor(pCon->hStmt);
	  return (LPQUERY) NULL;
   }
	
   // ����Ÿ�� ��� ���� �޸𸮸� �Ҵ��Ѵ�.
//	LPQUERY RealData = new QUERYST;
	LPQUERY RealData = (LPQUERY) m_pResultMemory->Alloc();
	if(!RealData) m_pWriteMessage("SingleQuery : Fail to alloc memory");

	memset(RealData,0,sizeof(QUERYST));
	RealData->nNum = nCols;

	for(nCount=0; nCount<nCols; nCount++)
      SQLBindCol(pCon->hStmt, (UWORD)(nCount+1), sqltype, RealData->Data[nCount], MAX_COL_LENGTH, &(RealData->Length[nCount]));


   for(; (retCode = SQLFetch(pCon->hStmt))==SQL_SUCCESS || retCode==SQL_SUCCESS_WITH_INFO;)
   {
	  if (retCode != SQL_SUCCESS)
			DisplaySQLError(pCon);
   }

   // if there was any error returned by SQLFetch, display it
   if (retCode == SQL_ERROR)
		DisplaySQLError(pCon);

   // Close the open result set.
   // Unbind all bound columns.
	SQLCloseCursor(pCon->hStmt);
	SQLFreeStmt(pCon->hStmt, SQL_UNBIND);
	SQLFreeStmt(pCon->hStmt, SQL_RESET_PARAMS);

   return RealData;
}


////////////////////////////////////////////////////////////////////////////////////////////
// �̱� ����DA �Լ�
// �� �̱����� �Լ��� �ٸ� ���� �޸𸮰� �������� �Ҵ�ȴٴ� ���̴�...-_-
// ���� ������ ���� ������ ��� �޸𸮸� �Ƴ� �� �ִ�.
// ���ڼ��� 
// Ŀ�ؼ�, ���̺� �̸�, IDENTITY Į���� ��, ������ Į���� �̸�, ������ Ÿ��
// �ٸ� ������ ���� SingleQuery�Լ� ���� ����
// �ٸ� ���� ���ϰ��� LPQUERYDA �������ε� �̰��� ���� �������� �����Ͱ� ����ִ� ����ü�̴�.
// �޸� �Ҵ絵 ���� �����ϹǷ� 
// Free �� ������ �ݵ�� ���ÿ� �ִ� ��ó�� ���� Ǯ����� �Ѵ�.
// ������ SamlpeSingleQueryDA ����
////////////////////////////////////////////////////////////////////////////////////////////

LPQUERYDA FAR PASCAL CDB::SingleQueryDA(LPDBCON pCon, LPSTR szTable, DWORD dwValue, LPSTR lpCol, SQLSMALLINT sqltype)
{
	SWORD     nCols = 0;                  // # of result columns
	SWORD     nCount;                     // index
	SQLRETURN retCode;                    // return code
	
 	if (pCon->hStmt == NULL)
	{
		retCode = SQLAllocHandle(SQL_HANDLE_STMT, pCon->hDBC, &(pCon->hStmt));
		if(retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO)
			DisplaySQLError(pCon);
	}
	// ���� ����
	char szBuffer[MAX_LENGTH_QUERY];
	if (lpCol == NULL || *lpCol == 0)
		sprintf(szBuffer,"select * from %s where IDENTITYCOL = '%d' " ,szTable,dwValue);
	else sprintf(szBuffer,"select %s from %s where IDENTITYCOL = '%d' " ,lpCol, szTable, dwValue);

   retCode = SQLExecDirect(pCon->hStmt, (UCHAR *)szBuffer, SQL_NTS);
   if (retCode != SQL_SUCCESS) 
   {
		DisplaySQLError(pCon);
      if (retCode != SQL_SUCCESS_WITH_INFO) 
	  {
         SQLCloseCursor(pCon->hStmt);
         return (LPQUERYDA) NULL;
      }
   }

   retCode = SQLNumResultCols(pCon->hStmt, &nCols);
   if (nCols >= MAX_COL_NUM) 
   {
	  nCols = MAX_COL_NUM;
   }
   else if (nCols == 0) 
   {
	  SQLCloseCursor(pCon->hStmt);
	  return (LPQUERYDA) NULL;
   }
	
 	LPQUERYDA RealData = new QUERYDA;
	LPDBSDWORD pLength = new DBSDWORD[nCols];
	DBSTRING*   pData = new DBSTRING[nCols]; 
	memset(RealData,0,sizeof(QUERYDA));
	memset(pLength,0,sizeof(DBSDWORD)*nCols);
	memset(pData,0,sizeof(DBSTRING)*nCols);
	
	RealData->nNum = nCols;
	RealData->pData = pData;
	RealData->pLength = pLength;

	for(nCount=0; nCount<nCols; nCount++)
	    SQLBindCol(pCon->hStmt, (UWORD)(nCount+1), sqltype, (&pData[nCount])->szData, MAX_COL_LENGTH, &( (&(pLength[nCount]))->pLeng));

   for(; (retCode = SQLFetch(pCon->hStmt))==SQL_SUCCESS || retCode==SQL_SUCCESS_WITH_INFO;)
   {
	  if (retCode != SQL_SUCCESS)
			DisplaySQLError(pCon);
   }

   if (retCode == SQL_ERROR)
		DisplaySQLError(pCon);

	SQLCloseCursor(pCon->hStmt);
	SQLFreeStmt(pCon->hStmt, SQL_UNBIND);
	SQLFreeStmt(pCon->hStmt, SQL_RESET_PARAMS);

   return RealData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �� ǥ�÷� ���� �Ϸ��� �Լ����� ��ũ�� �޽�带 ���� �Լ����̴�.
// ���� ��� �Խ����� ǥ���� ��츦 ���� ��� ����Ÿ�� �Ͻÿ� �޾ƿ��� �� �ƴ϶�
// �������� ���ڵ带 �޾ƿ��� ������ ������ �޾ƿ��� ���� �˼� �ִ�.
// �� �Լ����� �׷� ����� ������ �ش�.
// �� ���ڵ��� ���� �ѹ��� �޾ƿ��⿡ ����ġ�� ���ų� �Ͻÿ� �� ���� �ʿ䰡 ���� ���
// ������ ����ŭ ���ڵ带 �޾ƿ��� ���� �Լ� ��뿡 ���� ���� �Ǵ� ������ ���ڵ带 �޾ƿ� �� �ְ� �ϴ� ���̴�.
// �� �Ϸ��� �Լ��� ����ϴ� ����� SampleGenerateResultSet() �Լ��� ������ ��.
// �뷫 ������ �����ϸ� ���� GenerateResultSet �� ������ ������� ������ �� 
// GetNext, GetLast ����� �Լ��� �̿��� ���ϴ� �κ��� ������ 
// CloseRecordSet �Լ��� ������� �ݴ´�.


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GenerateResultSet�Լ� 
// �Լ����ڴ� BindQuery�� ����.
BOOL	CDB::GenerateResultSet(LPDBCON pCon, LPSTR szBuffer, LPBIND lpBind, int StSize, SQLSMALLINT sqlType, int Rowset)
{
	SQLRETURN retCode;                    // return code
	SQLUSMALLINT RowStatusArray[MAX_ROW_NUM];

	if (pCon->hScrollStmt == NULL)
	{
		retCode = SQLAllocHandle(SQL_HANDLE_STMT, pCon->hDBC, &(pCon->hScrollStmt));
		if(retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO)
			DisplaySQLError(pCon,pCon->hScrollStmt);
	}

	SQLSetCursorName(pCon->hScrollStmt, (SQLCHAR*) &(pCon->szCursorName[0]), SQL_NTS);

	SQLSetStmtAttr(pCon->hScrollStmt, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER) SQL_CURSOR_KEYSET_DRIVEN, 0);
	SQLSetStmtAttr(pCon->hScrollStmt, SQL_ATTR_ROW_BIND_TYPE, (SQLPOINTER) StSize, 0);
	SQLSetStmtAttr(pCon->hScrollStmt, SQL_ATTR_ROW_ARRAY_SIZE,(SQLPOINTER) Rowset, 0);
	SQLSetStmtAttr(pCon->hScrollStmt, SQL_ATTR_ROW_STATUS_PTR, RowStatusArray, 0);
	SQLSetStmtAttr(pCon->hScrollStmt, SQL_ATTR_ROWS_FETCHED_PTR, &m_NumRowsFetched, 0);

	// �޸� ���ε�
	for (int k = 0;k < lpBind->nColNum;k++)
	{
		if (sqlType == NULL)
			SQLBindCol(pCon->hScrollStmt,k+1,lpBind->QueryType[k], ((LPROWWISEBIND) lpBind->pData[0])->Data[k],((LPROWWISEBIND) lpBind->pData[0])->Length[k],&(((LPROWWISEBIND) lpBind->pData[0])->LengthRead[k]));
		else SQLBindCol(pCon->hScrollStmt,k+1,sqlType, ((LPROWWISEBIND) lpBind->pData[0])->Data[k],((LPROWWISEBIND) lpBind->pData[0])->Length[k],&(((LPROWWISEBIND) lpBind->pData[0])->LengthRead[k]));
	}
	// ���� 
	retCode = SQLExecDirect(pCon->hScrollStmt, (UCHAR *)szBuffer, SQL_NTS);
   // ������ ��Ī

	retCode = SQLFetchScroll(pCon->hScrollStmt,SQL_FETCH_NEXT,0);
	if (retCode == SQL_ERROR)
		DisplaySQLError(pCon,pCon->hScrollStmt);

   // Ŀ���� ��ġ�� ������� �� ó�� 
   SQLSetPos(pCon->hScrollStmt, 1, SQL_POSITION, SQL_LOCK_NO_CHANGE);

	// ��Ȱ���ϴ� ������̹Ƿ� �������� �ʴ´�.	
   return 1;
}
// ���� ����Ÿ���� ��´�.
// �Խ����� ���� ��� ������ �����ϸ� �ȴ�.
BOOL	CDB::GetNextRecordSet(LPDBCON pCon)
{
	SQLRETURN retCode;                    // return code
	retCode = SQLFetchScroll(pCon->hScrollStmt, SQL_FETCH_NEXT, 0);

	if (SQLOK(retCode))
		return 1;
	else if (retCode == SQL_NO_DATA) return 0;
	else DisplaySQLError(pCon,pCon->hScrollStmt);
	return 0;
}
// ������ ����Ÿ���� ��´�.
// ������ ���ڵ���� �ѹ��� ����� ������� ���´�.
// �Խ����� �� ������ ��� ���� ���
BOOL	CDB::GetLastRecordSet(LPDBCON pCon)
{
	SQLRETURN retCode;                    // return code
	retCode = SQLFetchScroll(pCon->hScrollStmt, SQL_FETCH_LAST, 0);

	if (SQLOK(retCode))
		return 1;
	else if (retCode == SQL_NO_DATA) return 0;
	else DisplaySQLError(pCon,pCon->hScrollStmt);
	return 0;
}
// ���� ����Ÿ���� ��´�.
// ������ ��ġ �� ������
// �Խ��� ��� �� �� ��� ����
BOOL	CDB::GetPriorRecordSet(LPDBCON pCon)
{
	SQLRETURN retCode;                    // return code
	retCode = SQLFetchScroll(pCon->hScrollStmt, SQL_FETCH_PRIOR, 0);

	if (SQLOK(retCode))
		return 1;
	else if (retCode == SQL_NO_DATA) return 0;
	else DisplaySQLError(pCon,pCon->hScrollStmt);
	return 0;
}
// ù �������� ��´�.
// �Խ����� �� ÷ ��� ����
BOOL	CDB::GetFirstRecordSet(LPDBCON pCon)
{
	SQLRETURN retCode;                    // return code
	retCode = SQLFetchScroll(pCon->hScrollStmt, SQL_FETCH_FIRST, 0);

	if (SQLOK(retCode))
		return 1;
	else if (retCode == SQL_NO_DATA) return 0;
	else DisplaySQLError(pCon,pCon->hScrollStmt);
	return 0;
}
// ������ �ڵ��� ���ڵ� ���� �ݰ� Ŀ���� �����Ѵ�.
BOOL	CDB::CloseRecordSet(LPDBCON pCon)
{
	SQLCloseCursor(pCon->hScrollStmt);
	SQLFreeStmt(pCon->hScrollStmt, SQL_UNBIND);
	SQLFreeStmt(pCon->hScrollStmt, SQL_RESET_PARAMS);
	return 1;
}


// ���� Ŀ���� ��ġ�� �ִ� �����͸� ���� 
BOOL	CDB::DeleteRecordInCursor(LPDBCON pCon)
{
	if (pCon->nCursorNum == 0 ) return 0;

	SQLRETURN retCode;                    // return code
	retCode = SQLSetPos(pCon->hScrollStmt, pCon->nCursorNum, SQL_DELETE, SQL_LOCK_NO_CHANGE);
	if (SQLOK(retCode))
		return 1;
	else DisplaySQLError(pCon,pCon->hScrollStmt);
	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���̳ʸ��� �����ϱ� 
// ���ڴ� Ŀ�ؼ� ������, ������, ������ �������� ������, ���忡 �ʿ��� �Ķ���͸� ���� ������,�� ������ ������ ����, ���� ������ ���� ���� ������
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDB::SaveDataByPtr( LPDBCON pCon, char* szQuery, int nSize, LPVOID pPara, long nBuffersize, LPVOID pSettingData)
{
	if (pCon == NULL)
		return FALSE;

	RETCODE		retCode;
	RETCODE		putCode;
	SQLPOINTER	pParamData;

//	long		lqitem;
//	long		leitem;
//	long		liitem;

	long		exec;


	if (pCon->hStmt == NULL)
	{
		retCode = SQLAllocHandle(SQL_HANDLE_STMT, pCon->hDBC, &(pCon->hStmt));
		if( retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO)
			DisplaySQLError(pCon);
	}

	retCode = SQLPrepare(pCon->hStmt, (UCHAR *)szQuery, SQL_NTS);
	if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) 
	{
		retCode = SQLBindParameter(pCon->hStmt, 1, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_BINARY, nSize, 0, pPara, 0, &nBuffersize );
		if( !SQLOK( retCode))
		{
			DisplaySQLError(pCon);
			return FALSE;
		}

		exec = SQL_LEN_DATA_AT_EXEC(0);

		retCode = SQLExecute(pCon->hStmt);
		if( retCode != SQL_NEED_DATA) 
		{
			DisplaySQLError(pCon);
			return  FALSE;
		}

		while( retCode == SQL_NEED_DATA)
		{      
			retCode = SQLParamData(pCon->hStmt, &pParamData);
			if( retCode == SQL_NEED_DATA)
			{
				putCode = SQLPutData(pCon->hStmt, pSettingData, nSize );
				if( !SQLOK( putCode))
				{
					DisplaySQLError(pCon);
					return  FALSE;
				}
			}
			else break;
		}
	}

	retCode = SQLFreeStmt( pCon->hStmt, SQL_CLOSE );
	if( !SQLOK( retCode))
	{
		return FALSE;
		DisplaySQLError(pCon);
	}
//	pCon->hStmt = NULL;
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���̳ʸ��� �� �޾ƿ��� 
// ���ڴ� Ŀ�ؼ� ������, ������, �޾ƿ� ����, ���� �����͸� ���� ������ 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDB::LoadDataByPtr( LPDBCON pCon, char* szQuery, int nSize, LPVOID pData)
{
	if (pCon == NULL)
		return FALSE;

	RETCODE		retCode;
	if (pCon->hStmt == NULL)
	{
		retCode = SQLAllocHandle(SQL_HANDLE_STMT, pCon->hDBC, &(pCon->hStmt));
		if(retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO)
		{
			DisplaySQLError(pCon);
			return FALSE;
		}
	}

	long	IndPtr;

	retCode = SQLExecDirect(pCon->hStmt, (UCHAR *)szQuery, SQL_NTS);

	if (0 != SQLOK(retCode))
	{
		retCode = SQLFetch(pCon->hStmt);

		if (retCode == SQL_SUCCESS)
		{
			memset(pData, 0x00, nSize);

			retCode = SQLGetData(pCon->hStmt, 1, SQL_C_BINARY, pData, nSize, &IndPtr);

			SQLFreeStmt(pCon->hStmt,SQL_CLOSE);

			if (retCode == SQL_SUCCESS)
				return TRUE;
			else
				return FALSE;
		}
	}
	
	DisplaySQLError(pCon);

	SQLFreeStmt( pCon->hStmt, SQL_CLOSE);
//	pCon->hStmt = NULL;

	return FALSE;
}


void CDB::DeleteThisPointer(DWORD dwFnNo, LPVOID pTarget)
{
	switch(dwFnNo)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
		break;
	case 101: //Free Query
		{
			LPQUERY pData	= (LPQUERY) pTarget;
			m_pResultMemory->Free((char*)pData);
			break;
		}
	case 104: //FreeLarge Query
		{
			LPLARGEQUERY pData	= (LPLARGEQUERY) pTarget;
			m_pResultMemory->Free((char*)pData);
			break;
		}
	case 105: //FreeMiddle Query
		{
			LPMIDDLEQUERY pData	= (LPMIDDLEQUERY) pTarget;
			m_pResultMemory->Free((char*)pData);
			break;
		}		
	case 102: // SingleQuery
		{
			LPQUERY pData	= (LPQUERY) pTarget;
			m_pResultMemory->Free((char*)pData);
			break;
		}
	case 103: // SingleQueryDA
		{
			LPQUERYDA pData	= (LPQUERYDA) pTarget;
			LPDBSDWORD pLength	= (LPDBSDWORD) pData->pLength;	
			LPDBSTRING pDatast	= (LPDBSTRING) pData->pData;
			delete [] pLength;
			delete [] pDatast;
			delete  pData;
			break;
		}
	}
}

void	CDB::FreeDBMessageMemory(char* pTarget)
{
//	EnterCriticalSection(&m_csDBMessageMemory);
	m_pDBMessageMemory->Free(pTarget);
//	LeaveCriticalSection(&m_csDBMessageMemory);
}

DWORD	CDB::GetUnusedDBMemoryNum()
{
	return m_pDBMessageMemory->GetFreeIndexNum();

//	EnterCriticalSection(&m_csDBMessageMemory);
//	DWORD temp = m_pDBMessageMemory->GetFreeIndexNum();
//	LeaveCriticalSection(&m_csDBMessageMemory);
//	return temp;
}
char*	CDB::GetDBMessageMemory()
{
	return m_pDBMessageMemory->Alloc();
//	EnterCriticalSection(&m_csDBMessageMemory);
//	char* temp = m_pDBMessageMemory->Alloc();
//	LeaveCriticalSection(&m_csDBMessageMemory);
//	return temp;
}




void	CDB::FreeDBStackMemory(char* pTarget)
{
//	EnterCriticalSection(&m_csDBStackMemory);
	m_pDBStackMemory->Free(pTarget);
//	LeaveCriticalSection(&m_csDBStackMemory);
}
DWORD	CDB::GetUnusedStackMemoryNum()
{
	return m_pDBStackMemory->GetFreeIndexNum();
//	EnterCriticalSection(&m_csDBStackMemory);
//	DWORD temp = m_pDBStackMemory->GetFreeIndexNum();
//	LeaveCriticalSection(&m_csDBStackMemory);
//	return temp;
}

char*	CDB::GetDBMessageStackMemory()
{
	return m_pDBStackMemory->Alloc();
//	EnterCriticalSection(&m_csDBStackMemory);
//	char* temp = m_pDBStackMemory->Alloc();
//	LeaveCriticalSection(&m_csDBStackMemory);
//	return temp;
}



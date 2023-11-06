// TrafficLog.cpp: implementation of the CTrafficLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TrafficLog.h"
#include "UserTable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CTrafficLog);
CTrafficLog::CTrafficLog()
{
	m_pUserTrafficPool = new CMemoryPoolTempl<USERTRAFFIC>;
	m_pUserTrafficPool->Init( MAX_USER_IN_A_AGENT, 500, "UserTrafficPool");
	m_UserTrafficTable.Initialize( MAX_USER_IN_A_AGENT );

	m_dwCheckTime = 0;
	m_dwLastPacketCheckTime = 0;
	m_dwLastPacketWriteTime = 0;
	m_dwUnValuedCount = 0;
	m_dwValuedCount = 0;

	//------------
	memset( m_dwReceive, 0, sizeof( m_dwReceive ) );
	memset( m_dwSend, 0, sizeof( m_dwSend ) );
	//------------
}

CTrafficLog::~CTrafficLog()
{
	m_UserTrafficTable.SetPositionHead();

	USERTRAFFIC* pData = NULL;

	while( pData = m_UserTrafficTable.GetData() )
	{
		m_pUserTrafficPool->Free(pData);
	}

	m_UserTrafficTable.RemoveAll();

	SAFE_DELETE(m_pUserTrafficPool);
}


void CTrafficLog::StartRecord()
{/*
	m_dwStartTime		= gCurTime;

	SYSTEMTIME ti;
	GetLocalTime( &ti );

	char fname[256];
	sprintf(fname,"./Log/RTrafficLog_Agent%d_%02d%02d%02d.txt", g_pServerSystem->GetServerNum(),
														ti.wYear, ti.wMonth, ti.wDay );

	FILE* fp = NULL;
	fp = fopen( fname, "a+" );
	if(fp)
	{
		fprintf( fp, "��Ͻð�\t�ѹ���ũ��\t�ѹ�����\t����ð�\t���絿��\n" );
		fclose( fp );
	}	

	sprintf(fname,"./Log/STrafficLog_Agent%d_%02d%02d%02d.txt", g_pServerSystem->GetServerNum(),
														ti.wYear, ti.wMonth, ti.wDay );

	fp = fopen( fname, "a+" );
	if(fp)
	{
		fprintf( fp, "��Ͻð�\t�Ѻ���ũ��\t�Ѻ�����\t����ð�\t���絿��\n" );
		fclose( fp );
	}

	sprintf(fname,"./Log/RMove_Agent%d_%02d%02d%02d.txt", g_pServerSystem->GetServerNum(),
														ti.wYear, ti.wMonth, ti.wDay );

	fp = fopen( fname, "a+" );
	if(fp)
	{
		fprintf( fp, "0: player, 1:monster ; 0: MP_TARGET, 1:MP_ONETARGET, 2:MP_COLLECTION\n" );
		fclose( fp );
	}	

	sprintf(fname,"./Log/SRMove_Agent%d_%02d%02d%02d.txt", g_pServerSystem->GetServerNum(),
														ti.wYear, ti.wMonth, ti.wDay );

	fp = fopen( fname, "a+" );
	if(fp)
	{
		fprintf( fp, "0: player, 1:monster ; 0: MP_TARGET, 1:MP_ONETARGET, 2:MP_COLLECTION\n" );
		fclose( fp );
	}

	Clear();
	*/
}


void CTrafficLog::EndRecord()
{
	WriteTrafficLogFile();
//	Clear();
}

//------------
void CTrafficLog::AddReceivePacketAll( WORD category, WORD Protocol, DWORD dwLength )
{
	++m_dwReceive[0][category][Protocol];					//count
	m_dwReceive[1][category][Protocol] += (dwLength + 40);	//size	
}

void CTrafficLog::AddSendPacketAll( WORD category, WORD Protocol, DWORD dwLength )
{
	++m_dwSend[0][category][Protocol];						//count
	m_dwSend[1][category][Protocol] += (dwLength + 40);	//size	
}
//-----------


#define TRAFFIC_LOG_WRITE_TIME	600000

void CTrafficLog::Process()
{
/*
	if(gCurTime - m_dwLastPacketCheckTime >= m_dwCheckTime)
	{
		ResetUserPacket(gCurTime - m_dwLastPacketCheckTime);
		m_dwLastPacketCheckTime = gCurTime;
	}

	if( gCurTime - m_dwLastPacketWriteTime >= TRAFFIC_LOG_WRITE_TIME )
	{
		WriteUserTrafficLogFile();
		m_dwLastPacketWriteTime = gCurTime;
	}

	if( gCurTime - m_dwStartTime >= TRAFFIC_LOG_WRITE_TIME )
	{
		WriteTrafficLogFile();
		Clear();
		m_dwStartTime = gCurTime;
	}
*/
}

void CTrafficLog::Clear()
{
//	ZeroMemory( m_dwTotalReceiveSize, sizeof(m_dwTotalReceiveSize) );
//	ZeroMemory( m_dwTotalReceiveNum, sizeof(m_dwTotalReceiveNum) );
//	ZeroMemory( m_dwTotalSendSize, sizeof(m_dwTotalSendSize) );
//	ZeroMemory( m_dwTotalSendNum, sizeof(m_dwTotalSendNum) );

//	ZeroMemory( m_dwMoveSizeReceive, sizeof(m_dwMoveSizeReceive) );
//	ZeroMemory( m_dwMoveNumReceive, sizeof(m_dwMoveNumReceive) );
//	ZeroMemory( m_dwMoveSizeSend, sizeof(m_dwMoveSizeSend) );
//	ZeroMemory( m_dwMoveNumSend, sizeof(m_dwMoveNumSend) );
}

void CTrafficLog::WriteTrafficLogFile()	
{
	SYSTEMTIME ti;
	GetLocalTime( &ti );

	char fname[256];
	sprintf(fname,"./Log/RTrafficLog_Agent%d_%02d%02d%02d.txt", g_pServerSystem->GetServerNum(),
														ti.wYear, ti.wMonth, ti.wDay );

	FILE* fp = fopen( fname, "a+" );

	if( fp )
	{
		//count
		for( int i = 0 ; i < MP_MAX ; ++i )
		{
			for( int j = 0 ; j < 256 ; ++j )
			{
				fprintf( fp, "%d ", m_dwReceive[0][i][j] );
			}

			fprintf( fp, "\n" );
		}

		fprintf( fp, "\n\n" );

		//size
		for( int i = 0 ; i < MP_MAX ; ++i )
		{
			for( int j = 0 ; j < 256 ; ++j )
			{
				fprintf( fp, "%d ", m_dwReceive[1][i][j] );
			}
			fprintf( fp, "\n" );
		}

		fprintf( fp, "\n\n" );

		fclose( fp );
	}

	sprintf(fname,"./Log/STrafficLog_Agent%d_%02d%02d%02d.txt", g_pServerSystem->GetServerNum(),
														ti.wYear, ti.wMonth, ti.wDay );

	fp = fopen( fname, "a+" );

	if( fp )
	{
		//count
		for( int i = 0 ; i < MP_MAX ; ++i )
		{
			for( int j = 0 ; j < 256 ; ++j )
			{
				fprintf( fp, "%d ", m_dwSend[0][i][j] );
			}

			fprintf( fp, "\n" );
		}

		fprintf( fp, "\n\n" );

		//size
		for( int i = 0 ; i < MP_MAX ; ++i )
		{
			for( int j = 0 ; j < 256 ; ++j )
			{
				fprintf( fp, "%d ", m_dwSend[1][i][j] );
			}
			fprintf( fp, "\n" );
		}

		fclose( fp );
	}

/*
//
	DWORD dwReceiveTotalSize	= 0;
	DWORD dwSendTotalSize		= 0;
	DWORD dwReceiveTotalNum		= 0;
	DWORD dwSendTotalNum		= 0;

	for( int i = 0 ; i < MP_MAX ; ++i )
	{
		dwReceiveTotalSize += m_dwTotalReceiveSize[i];
		dwReceiveTotalNum += m_dwTotalReceiveNum[i];
		dwSendTotalSize += m_dwTotalSendSize[i];
		dwSendTotalNum += m_dwTotalSendNum[i];
	}

	SYSTEMTIME ti;
	GetLocalTime( &ti );

	char fname[256];

	sprintf(fname,"./Log/RTrafficLog_Agent%d_%02d%02d%02d.txt", g_pServerSystem->GetServerNum(),
														ti.wYear, ti.wMonth, ti.wDay );

	FILE* fp;
	
	fp = fopen( fname, "a+" );

	if(fp)
	{
		fprintf( fp, "%02d:%02d:%02d\t", ti.wHour, ti.wMinute, ti.wSecond );
		fprintf( fp, "%u\t%u", dwReceiveTotalSize, dwReceiveTotalNum );
		fprintf( fp, "\t%u", g_pUserTable->GetDataNum() );
		fprintf( fp, "\t%u", gCurTime - m_dwStartTime );

		for( int i = 0 ; i < MP_MAX ; ++i )
		{
			fprintf( fp, "\t%u\t%u", m_dwTotalReceiveSize[i], m_dwTotalReceiveNum[i] );
		}
		fprintf( fp, "\n");

		fclose(fp);
	}	

	sprintf(fname,"./Log/STrafficLog_Agent%d_%02d%02d%02d.txt", g_pServerSystem->GetServerNum(),
														ti.wYear, ti.wMonth, ti.wDay );

	fp = fopen( fname, "a+" );

	if(fp)
	{
		fprintf( fp, "%02d:%02d:%02d\t", ti.wHour, ti.wMinute, ti.wSecond );
		fprintf( fp, "%u\t%u", dwSendTotalSize, dwSendTotalNum );
		fprintf( fp, "\t%u", g_pUserTable->GetDataNum() );
		fprintf( fp, "\t%u", gCurTime - m_dwStartTime );

		for( int i = 0 ; i < MP_MAX ; ++i )
		{
			fprintf( fp, "\t%u\t%u", m_dwTotalSendSize[i], m_dwTotalSendNum[i] );
		}
		fprintf( fp, "\n");


		fclose(fp);
	}


	sprintf(fname,"./Log/RMove_Agent%d_%02d%02d%02d.txt", g_pServerSystem->GetServerNum(),
														ti.wYear, ti.wMonth, ti.wDay );
	fp = fopen( fname, "a+" );
	if(fp)
	{
		fprintf( fp, "%02d:%02d:%02d\t", ti.wHour, ti.wMinute, ti.wSecond );
		fprintf( fp, "\t%u", g_pUserTable->GetDataNum() );
		fprintf( fp, "\t%u", gCurTime - m_dwStartTime );		

		int i, j;
		for( i = 0 ; i < 2 ; ++i )
		{
			for( j = 0 ; j < 4 ; ++j )
			{
				fprintf( fp, "\t%u", m_dwMoveSizeReceive[i][j] );
			}
			for( j = 0 ; j < 4 ; ++j )
			{
				fprintf( fp, "\t%u", m_dwMoveNumReceive[i][j] );
			}
			fprintf( fp, "\n" );
		}

		fclose( fp );
	}	

	sprintf(fname,"./Log/SRMove_Agent%d_%02d%02d%02d.txt", g_pServerSystem->GetServerNum(),
														ti.wYear, ti.wMonth, ti.wDay );

	fp = fopen( fname, "a+" );
	if(fp)
	{
		fprintf( fp, "%02d:%02d:%02d\t", ti.wHour, ti.wMinute, ti.wSecond );
		fprintf( fp, "\t%u", g_pUserTable->GetDataNum() );
		fprintf( fp, "\t%u", gCurTime - m_dwStartTime );

		int i, j;
		for( i = 0 ; i < 2 ; ++i )
		{
			for( j = 0 ; j < 4 ; ++j )
			{
				fprintf( fp, "\t%u", m_dwMoveSizeSend[i][j] );
			}
			for( j = 0 ; j < 4 ; ++j )
			{
				fprintf( fp, "\t%u", m_dwMoveNumSend[i][j] );
			}
		}

		fclose( fp );
	}
*/



}

// 06. 01. ������ Ʈ���� �α� ��� - �̿���
void CTrafficLog::AddUser(DWORD dwUserID, DWORD dwConnectionIndex)
{
	USERTRAFFIC* pData = NULL;

	pData = m_UserTrafficTable.GetData(dwUserID);

	// ���� ���� ������ �ִ°�?
	if( pData )
	{
		// �α��� �Ǿ��ִ°�?
		if( pData->bLogin )
		{
			// ����!
			ASSERT(0);
		}
		else
		{
			// �α��� ���·� ��ȯ
			pData->bLogin = true;
		}
	}
	// ���ο� ���� ������ �ۼ��Ѵ�
	else
	{
		pData = m_pUserTrafficPool->Alloc();

		pData->dwConnectionIndex = dwConnectionIndex;
		pData->dwUserID = dwUserID;
		pData->dwLoginTime = 0;
		pData->dwTotalPacketCount = 0;
		pData->dwValuedCount = 0;
		pData->dwUnValuedCount = 0;
		pData->bLogin = true;

		m_UserTrafficTable.Add( pData, dwUserID );
	}
}

void CTrafficLog::RemoveUser(DWORD dwUserID)
{
    USERTRAFFIC* pData = NULL;

	// ���������� �����´�
	pData = m_UserTrafficTable.GetData(dwUserID);

	// �α��� �����϶�
	if( pData && pData->bLogin )
	{
		// ������ üũ�� ������� �ð� ���
		DWORD spend = gCurTime - m_dwLastPacketCheckTime;
		
		pData->dwLoginTime += spend;
		
		pData->dwUnValuedCount = 0;
		pData->dwValuedCount = 0;

		pData->bLogin = false;

		return;
	}

	// �α��� ���� �ƴ϶�� �߸��Ǿ���.
	ASSERT(0);
}

void CTrafficLog::AddUserPacket(DWORD dwUserID, BYTE Category)
{
	USERTRAFFIC* pData = NULL;

	// ���� ������ �����´�
	pData = m_UserTrafficTable.GetData(dwUserID);

	// ������ ��ȿ�ϰ� �α��� ���϶�
	if( pData && pData->bLogin )
	{
		// ī�װ��� �߸��Ǿ��ٸ�
		if(Category >= MP_MAX || Category <= 0)
			// �߸��� ��Ŷ
			pData->dwUnValuedCount++;
		else
			// ���� ��Ŷ
			pData->dwValuedCount++;

		// ��ü ��Ŷ
		pData->dwTotalPacketCount++;
		
		// �߸��� ��Ŷ�� ����ġ �̻� ������ �߶�
		if(pData->dwUnValuedCount >= m_dwUnValuedCount)
		{
			DWORD dwConIdx = pData->dwConnectionIndex;
			OnDisconnectUser( dwConIdx );
			DisconnectUser( dwConIdx );
		}

		// ���� ��Ŷ�� ����ġ �̻� ������ �߶�
		if(pData->dwValuedCount >= m_dwValuedCount)
		{
			DWORD dwConIdx = pData->dwConnectionIndex;
			OnDisconnectUser( dwConIdx );
			DisconnectUser( dwConIdx );
		}
		
		return;
	}

	// �α��� ���� �ƴ϶�� �߸��Ǿ���.
	// ASSERT(0); // �Ѳ����� ���� �����Ͱ� ���ý� ���� �����Ŀ� �޼����� ó���ؼ� ���ü� �ִ�.
}

void CTrafficLog::ResetUserPacket(DWORD dwTime)
{
	USERTRAFFIC* pData = NULL;

	m_UserTrafficTable.SetPositionHead();
	while( pData = m_UserTrafficTable.GetData() )
	{
		if( pData->bLogin )
		{
			// �������� ���ӽð� ����
			pData->dwLoginTime += dwTime;

			pData->dwValuedCount = 0;
			/*
			// ��Ŷ ���
			double time = dwTime / 1000;
			double average = pData->dwPacketCount / time;
			pData->dwPacketCount = 0;

			if( average > m_fAveragePacketCount)
			{		
				DisconnectUser( pData->dwConnectionIndex );
				RemoveUser(pData->dwUserID);
			}
			*/
		}
	}
}

void CTrafficLog::WriteUserTrafficLogFile()
{
	SYSTEMTIME ti;
	GetLocalTime( &ti );
	char fname[256];

	sprintf(fname,"./Log/UserTrafficLog_Agent%d_%02d%02d%02d.txt", g_pServerSystem->GetServerNum(),
														ti.wYear, ti.wMonth, ti.wDay );

	FILE* fp;
	
	fp = fopen( fname, "a+" );

	if(fp)
	{
		fprintf( fp, "%02d : %02d : %02d ���� ��Ȳ\n��ϵ� ������ : %d\n", ti.wHour, ti.wMinute, ti.wSecond, m_UserTrafficTable.GetDataNum() );

		USERTRAFFIC* pData = NULL;
		WORD count = 0;
		
		m_UserTrafficTable.SetPositionHead();
		while( pData = m_UserTrafficTable.GetData() )
		{
			double time = pData->dwLoginTime / 1000;
			double average = pData->dwTotalPacketCount / time;
			
			fprintf( fp, "%04d USERID : %d , ���ӽð�: %f, ����Ŷ�� : %d, ��� : %f\n", ++count, pData->dwUserID, time, pData->dwTotalPacketCount, average );
			if( !pData->bLogin )
			{
				m_UserTrafficTable.Remove( pData->dwUserID );
				m_pUserTrafficPool->Free(pData);
			}
		}

		fclose( fp );
	}
}

void CTrafficLog::Init()
{
	// �������� �б�
	char fname[] = "./Traffic.txt";

	FILE* fp;
	
	fp = fopen( fname, "r" );

	if(fp)
	{
		// �߸��� ��Ŷ��	üũŸ��	���� ��Ŷ��
		fscanf(fp, "%d %d %d", &m_dwUnValuedCount, &m_dwCheckTime, &m_dwValuedCount);

		fclose( fp );
	}
	else
	{
		// �⺻��
		m_dwUnValuedCount = 10;
		m_dwCheckTime = 1;
		m_dwValuedCount = 1000;
	}

	m_dwCheckTime *= 1000;
}

void CTrafficLog::End()
{
	// �����Ҷ� �ѹ��� ���������
	WriteUserTrafficLogFile();
}

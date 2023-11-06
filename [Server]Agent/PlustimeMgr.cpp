// PlustimeMgr.cpp: implementation of the CPlustimeMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PlustimeMgr.h"
#include "MHFile.h"
#include "Network.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlustimeMgr::CPlustimeMgr()
{
	m_pPlusTimePool = new CMemoryPoolTempl<stPlusTime>;
	m_pPlusTimePool->Init( 50, 10, "stPlusTimePool" );
	m_PlusTimeTable.Initialize( 50 );
	m_AppliedTable.Initialize( 50 );
	
	m_bToggleOn = TRUE;
}


CPlustimeMgr::~CPlustimeMgr()
{
	m_PlusTimeTable.SetPositionHead();
	stPlusTime* pData = NULL;
	while( pData = m_PlusTimeTable.GetData() )
	{
		m_pPlusTimePool->Free( pData );
	}
	m_PlusTimeTable.RemoveAll();
	SAFE_DELETE( m_pPlusTimePool );
}


void CPlustimeMgr::ResetPlusTimeInfo()
{
	m_PlusTimeTable.SetPositionHead();
	stPlusTime* pData = NULL;
	while( pData = m_PlusTimeTable.GetData() )
	{
//		m_PlusTimeTable.Remove( pData->Index );
		m_pPlusTimePool->Free( pData );		
	}
	m_PlusTimeTable.RemoveAll();
	m_AppliedTable.RemoveAll();

	//
	m_PlustimeCount = 0;
	memset( m_Context, 0, sizeof(char)*1024 );
}


void CPlustimeMgr::Init()
{
	ResetPlusTimeInfo();

	CMHFile file;
	if(!file.Init("./system/Resource/PlustimeInfo.bin", "rb"))
	{
		ASSERT(0);
		return;
	}
	
	m_PlustimeCount = file.GetWord();
	stPlusTime* pData = NULL;

	for( DWORD i=0; i<m_PlustimeCount; ++i )
	{
		pData = m_pPlusTimePool->Alloc();

		//
		pData->Index = file.GetWord();
		pData->EventIndex = file.GetWord();
		pData->StartDay = file.GetWord();
		pData->StartHour = file.GetWord();
		pData->StartMinute = file.GetWord();
		pData->EndDay = file.GetWord();
		pData->EndHour = file.GetWord();
		pData->EndMinute = file.GetWord();
		pData->Rate = file.GetDword();
//		strcpy( pData->strTitle, file.GetString() );
//		strcpy( pData->strContext, file.GetString() );
		SafeStrCpy( pData->strTitle, file.GetString(), 32 );
		SafeStrCpy( pData->strContext, file.GetString(), 128 );


		//
		m_PlusTimeTable.Add( pData, pData->Index );
	}
	
	file.Release();
}


void CPlustimeMgr::Process()
{
	if( !m_bToggleOn )		return;

	stPlusTime* pData = NULL;
	SYSTEMTIME cTime;
	GetLocalTime( &cTime );
	BOOL bChange = FALSE;

	// ����üũ
	m_PlusTimeTable.SetPositionHead();
	while( pData = m_PlusTimeTable.GetData() )
	{
		if( m_AppliedTable.GetData( pData->Index ) != NULL )	continue;		
		if( pData->EndDay < cTime.wDayOfWeek )					continue;			// �̹� ����� (����)
		else if( pData->EndDay == cTime.wDayOfWeek )
		{
			if( pData->EndHour < cTime.wHour )					continue;			// �̹� ����� (�ð�)
			else if( pData->EndHour == cTime.wHour )
				if( pData->EndMinute <= cTime.wMinute )			continue;			// �̹� ����� (��)
		}
		if( pData->StartDay != cTime.wDayOfWeek )				continue;			// ���� ���ڰ� �ƴ�
		if( pData->StartHour > cTime.wHour )					continue;			// ���� ������ (�ð�)
		else if( pData->StartHour == cTime.wHour )
			if( pData->StartMinute > cTime.wMinute )				continue;			// ���� ������ (��)

		// ����Ÿ ����
		if( m_AppliedTable.GetDataNum() )		strcat( m_Context, ", " );
		strcat( m_Context, pData->strContext );		
		g_pServerSystem->SetEventNotifyStr( pData->strTitle, m_Context );
		g_pServerSystem->SetUseEventNotify( TRUE );
		
		PlusTimeOn( pData->EventIndex, (WORD)pData->Rate );

		m_AppliedTable.Add( pData, pData->Index );

		bChange = TRUE;
	}

	// ����üũ
	cPtrList list;	
	m_AppliedTable.SetPositionHead();
	while( pData = m_AppliedTable.GetData() )
	{
		if( pData->EndDay != cTime.wDayOfWeek )				continue;			// ���ᳯ��
		if( pData->EndHour > cTime.wHour )					continue;			// ����ð� üũ
		if( pData->EndHour == cTime.wHour )
		if( pData->EndMinute > cTime.wMinute )				continue;			// ���� ��

		list.AddTail( pData );

		bChange = TRUE;
	}

	// ���ְ� �˸���.
	PTRLISTPOS pos = list.GetHeadPosition();
	while( pos )
	{
		stPlusTime* pAppliedData = (stPlusTime*)list.GetNext( pos );
		if( pAppliedData )
		{
			PlusTimeOff( pAppliedData->EventIndex );
			m_AppliedTable.Remove( pAppliedData->Index );
		}
	}
	list.RemoveAll();

	// �����ִ°� ������ Context�� �ٽ� ���ش�.
	if( !bChange )			return;
	if( m_AppliedTable.GetDataNum() )
	{
		char title[128] = {0, };
		memset( m_Context, 0, sizeof(char)*1024 );

		g_pServerSystem->ResetApplyEvent();

		m_AppliedTable.SetPositionHead();
		while( pData = m_AppliedTable.GetData() )
		{
			if( strlen( m_Context ) )		strcat( m_Context, ", " );

			strcat( m_Context, pData->strContext );

//			strcpy( title, pData->strTitle );
			SafeStrCpy( title, pData->strTitle, 32 );

			g_pServerSystem->SetApplyEvent( pData->EventIndex );
		}
		g_pServerSystem->SetEventNotifyStr( title, m_Context );
		g_pServerSystem->SetUseEventNotify( TRUE );
	}
	// ������ ��
	else
	{
		memset( m_Context, 0, sizeof(char)*1024 );
		g_pServerSystem->ResetApplyEvent();
		g_pServerSystem->SetUseEventNotify( FALSE );
	}
}


void CPlustimeMgr::PlusTimeOn()
{	
	Init();

	m_bToggleOn = TRUE;
}


void CPlustimeMgr::PlusTimeOff()
{
	m_AppliedTable.RemoveAll();

	g_pServerSystem->SetUseEventNotify( FALSE );
	PlustimeAllOff();

	m_bToggleOn = FALSE;
}


void CPlustimeMgr::PlustimeAllOff()
{
	MSGBASE msg;
	msg.Category = MP_CHEAT;
	msg.Protocol = 	MP_CHEAT_PLUSTIME_ALLOFF;
	g_Network.Broadcast2MapServer( (char*)&msg, sizeof(msg) );
}


void CPlustimeMgr::PlusTimeOn( WORD Index, WORD Rate )
{
	MSG_WORD2 msg;
	msg.Category = MP_CHEAT;
	msg.Protocol = 	MP_CHEAT_PLUSTIME_ON;
	msg.wData1 = Index;
	msg.wData2 = Rate;
	g_Network.Broadcast2MapServer( (char*)&msg, sizeof(msg) );
}


void CPlustimeMgr::PlusTimeOff( WORD Index )
{
	MSG_WORD msg;
	msg.Category = MP_CHEAT;
	msg.Protocol = 	MP_CHEAT_PLUSTIME_OFF;
	msg.wData = Index;	
	g_Network.Broadcast2MapServer( (char*)&msg, sizeof(msg) );
}

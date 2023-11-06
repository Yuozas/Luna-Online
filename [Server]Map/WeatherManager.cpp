// WeatherManager.cpp: implementation of the CWeatherManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WeatherManager.h"
#include "Player.h"
#include "MhFile.h"
#include "..\[cc]header\GameResourceManager.h"
#include "UserTable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLOBALTON(CWeatherManager);

CWeatherManager::CWeatherManager()
{
	m_dwLastCheckTime = 0;

	m_wState = eWS_Clear;

	ZeroMemory(m_nWeatherDecisionRate, sizeof(m_nWeatherDecisionRate));
	m_bAcceptWeather = FALSE;

	//m_wEventCount = 0;
	//m_bExecution = FALSE;
	
	//m_pWeatherSchedulePool = new CMemoryPoolTempl<stWeatherTime>;
	//m_pWeatherSchedulePool->Init( 50, 10, "stWeatherTimePool" );
	//m_WeatherSchedule.Initialize( 50 );

	//m_pEventSchedulePool = new CMemoryPoolTempl<stWeatherTime>;
	//m_pEventSchedulePool->Init( 50, 10, "stWeatherTimePool" );
	//m_EventSchedule.Initialize( 50 );
	
}

CWeatherManager::~CWeatherManager()
{
	//m_WeatherSchedule.SetPositionHead();
	//m_EventSchedule.SetPositionHead();
	//
	//stWeatherTime* pData = NULL;
	//
	//while( pData = m_WeatherSchedule.GetData() )
	//{
	//	m_pWeatherSchedulePool->Free( pData );
	//}
	//m_WeatherSchedule.RemoveAll();
	//SAFE_DELETE( m_pWeatherSchedulePool );

	//while( pData = m_EventSchedule.GetData() )
	//{
	//	m_pEventSchedulePool->Free( pData );
	//}
	//m_EventSchedule.RemoveAll();
	//SAFE_DELETE( m_pEventSchedulePool );
}

void CWeatherManager::Init()
{
	int nTemp[eWS_Max] = {51, 31, 10, 2, 5, 1};
	int nAccumulate = 0;

	int i;
	for(i=0; i<eWS_Max; i++)
	{
		if(100 <= nAccumulate)
		{
			m_nWeatherDecisionRate[i] = 0;
			continue;
		}

		m_nWeatherDecisionRate[i] = nAccumulate + nTemp[i];
		nAccumulate += nTemp[i];
	}

	if(nAccumulate == 100)
		m_bAcceptWeather = TRUE;
	else
		m_bAcceptWeather = FALSE;

	/*
	// 080328 NYJ --- �����ڵ����
	CMHFile file;
	char filename[256];
	sprintf(filename,"System/Resource/Weather_%02d.bin",GAMERESRCMNGR->GetLoadMapNum());
	if(!file.Init(filename,"rb"))
		return ;
	
	m_wStateCount = file.GetWord();
	stWeatherTime* pData = NULL;
	
	for( DWORD i=0; i<m_wStateCount; ++i )
	{
		pData = m_pWeatherSchedulePool->Alloc();
		
		pData->Index = file.GetWord();
		pData->StartDay = file.GetWord();
		pData->StartHour = file.GetWord();
		pData->StartMinute = file.GetWord();
		pData->EndDay = file.GetWord();
		pData->EndHour = file.GetWord();
		pData->EndMinute = file.GetWord();
		pData->State = file.GetWord();
		
		m_WeatherSchedule.Add( pData, pData->Index );
	}
	
	file.Release();
	*/
}

void CWeatherManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	/*
	// 080328 NYJ --- �����ڵ����
	MSGBASE* pmsg = (MSGBASE*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	
	if( !pPlayer )	// ���� ����� ���´�... ���� �ǳ�;;;
		return;
	
	if( pPlayer->GetUserLevel() > eUSERLEVEL_GM ) // GM�� �ƴ� ����� ���´�... Ȥ�ó�...
		return;

	switch( Protocol )
	{
	case MP_WEATHER_SET:
		{
			MSG_WEATHER* msg = (MSG_WEATHER*)pMsg;

            if( msg->MapNum != g_pServerSystem->GetMapNum() )
				return;

			stWeatherTime* pData = NULL;
			SYSTEMTIME cTime;
			GetLocalTime( &cTime );

			pData = m_pEventSchedulePool->Alloc();
			
			memcpy(pData, &msg->WeatherTime, sizeof(stWeatherTime));

			if( pData->State >= eWS_Max)
			{
				m_pEventSchedulePool->Free(pData);
				return;
			}

			pData->Index = m_wEventCount++;
			
			m_EventSchedule.Add( pData, pData->Index );
		}
		break;
	case MP_WEATHER_EXE:
		{
			MSG_WORD2* msg = (MSG_WORD2*)pMsg;

			if( msg->wData2 != g_pServerSystem->GetMapNum() )
				return;

			m_bExecution = TRUE;

			if( msg->wData1 >= eWS_Max)
				return;

			m_wState = msg->wData1;

			SendAllWeather();
		}
		break;
	case MP_WEATHER_RETURN:
		{
			MSG_WORD* msg = (MSG_WORD*)pMsg;

			if( msg->wData != g_pServerSystem->GetMapNum() )
				return;

			m_bExecution = FALSE;
		}
	}
	*/
}

void CWeatherManager::Process()
{
	if(!m_bAcceptWeather)
		return;

	int nChangeTime = 300000;	// default 5 minute

	switch(m_wState)
	{
	case eWS_Clear:			nChangeTime=300000;			break;
	case eWS_Cloudy:		nChangeTime=300000;			break;
	case eWS_Rainy:			nChangeTime=300000;			break;
	case eWS_Windy:			nChangeTime=120000;			break;
	case eWS_Foggy:			nChangeTime=60000;			break;
	case eWS_Typoon:		nChangeTime=60000;			break;
	}

	if( 0==m_dwLastCheckTime || m_dwLastCheckTime+nChangeTime < gCurTime)
	{
		int nNextWeather = rand()%100;
		int nNextWeatherState = eWS_Clear;

		if(0<=nNextWeather && nNextWeather<m_nWeatherDecisionRate[eWS_Clear])
		{
			nNextWeatherState = eWS_Clear;
		}
		else if(m_nWeatherDecisionRate[eWS_Clear]<=nNextWeather && nNextWeather<m_nWeatherDecisionRate[eWS_Cloudy])
		{
			nNextWeatherState = eWS_Cloudy;
		}
		else if(m_nWeatherDecisionRate[eWS_Cloudy]<=nNextWeather && nNextWeather<m_nWeatherDecisionRate[eWS_Rainy])
		{
			nNextWeatherState = eWS_Rainy;
		}
		else if(m_nWeatherDecisionRate[eWS_Rainy]<=nNextWeather && nNextWeather<m_nWeatherDecisionRate[eWS_Windy])
		{
			nNextWeatherState = eWS_Windy;
		}
		else if(m_nWeatherDecisionRate[eWS_Windy]<=nNextWeather && nNextWeather<m_nWeatherDecisionRate[eWS_Foggy])
		{
			nNextWeatherState = eWS_Foggy;
		}
		else if(m_nWeatherDecisionRate[eWS_Foggy]<=nNextWeather && nNextWeather<m_nWeatherDecisionRate[eWS_Typoon])
		{
			nNextWeatherState = eWS_Typoon;
		}

		m_wState = nNextWeatherState;
		m_dwLastCheckTime = gCurTime+1;

		SendAllWeather();
	}

	/*
	// 080328 NYJ --- �����ڵ����
	// 1�п� �ѹ��� ����
	if(gCurTime - m_dwLastCheckTime < 60000)
		return;

	m_dwLastCheckTime = gCurTime;

	if( m_bExecution ) // GM�� ���� ���� ����� ������ ����
		return;

	stWeatherTime* pData = NULL;
	
	SYSTEMTIME cTime;
	GetLocalTime( &cTime );
	WORD wState = m_wState;
	BOOL bState = FALSE;
	
	m_WeatherSchedule.SetPositionHead();
	
	// ������ �⺻ ���õ� ������
	while( pData = m_WeatherSchedule.GetData() )
	{
		// ���� �� ��Ȳ...
		// ���� ���� ��
		if( pData->StartDay > cTime.wDayOfWeek )
			continue;
		
		// ���� �ð� ��
		if( pData->StartDay == cTime.wDayOfWeek && pData->StartHour > cTime.wHour )
			continue;
		
		// ���� �� ��
		if( pData->StartHour == cTime.wHour && pData->StartMinute > cTime.wMinute )
			continue;
		
		// �̹� ���� ��Ȳ...
		// ������ ������
		if( pData->EndDay < cTime.wDayOfWeek )
			continue;
		
		// �ð��� ������
		if( pData->EndDay == cTime.wDayOfWeek && pData->EndHour < cTime.wHour )
			continue;
		
		// ���� ������
		if( pData->EndHour == cTime.wHour && pData->EndMinute < cTime.wMinute )
			continue;		
		
		wState = pData->State;
		bState = TRUE;
	}
	
	m_EventSchedule.SetPositionHead();
	
	// GM�� ���� ���õ� ������
	while( pData = m_EventSchedule.GetData() )
	{
		BOOL bPassed = FALSE;
		
		// ���� �� ��Ȳ...
		// ���� ���� ��
		if( pData->StartDay > cTime.wDayOfWeek )
			continue;
		
		// ���� �ð� ��
		if( pData->StartDay == cTime.wDayOfWeek && pData->StartHour > cTime.wHour )
			continue;
		
		// ���� �� ��
		if( pData->StartHour == cTime.wHour && pData->StartMinute > cTime.wMinute )
			continue;
		
		// �̹� ���� ��Ȳ...
		// ������ ������
		if( pData->EndDay < cTime.wDayOfWeek )
			bPassed = TRUE;
		
		// �ð��� ������
		if( pData->EndDay == cTime.wDayOfWeek && pData->EndHour < cTime.wHour )
			bPassed = TRUE;
		
		// ���� ������
		if( pData->EndHour == cTime.wHour && pData->EndMinute < cTime.wMinute )
			bPassed = TRUE;
		
		if( bPassed )
		{
			// ���� �������̸� ���� �Ѵ�
			m_EventSchedule.Remove( pData->Index );
			
			continue;
		}
		
		wState = pData->State;
		bState = TRUE;
	}
	
	// �����쿡 ���� ����� �����Ͱ� ������ �������� ����
	if( !bState )
		wState = eWS_None;

	// ��� ���°� �ٲ������ ��ü���� �����ش�
	if(wState != m_wState)
	{
		m_wState = wState;

		SendAllWeather();
	}
	*/
}

void CWeatherManager::UserLogOut()
{
}

void CWeatherManager::SendAllWeather()
{
	g_pUserTable->SetPositionUserHead();
	
	while( CObject* pObject = g_pUserTable->GetUserData() )
	{
		if( pObject->GetObjectKind() != eObjectKind_Player ) continue;
		
		CPlayer* pReceiver = (CPlayer*)pObject;
		
		SendWeather( pReceiver );	
	}
}

void CWeatherManager::SendWeather(CPlayer* pPlayer)
{
	MSG_WORD msg;

	msg.Category = MP_WEATHER;
	msg.dwObjectID = pPlayer->GetID();
	msg.Protocol = MP_WEATHER_CHANGE;	
	msg.wData = m_wState;
	
	pPlayer->SendMsg( &msg , sizeof( msg ) );
/*	
	// �̺�Ʈ ������ �����ÿ� �ƹ�Ÿ �ɼ��� �������־�� �Ѵ�
	WORD* pAvatar = pPlayer->GetShopItemStats()->Avatar;

	if(	(pAvatar[eAvatar_Dress] == EVENT_SHOPITEM_SNOWMAN_DRESS && pAvatar[eAvatar_Hat] == EVENT_SHOPITEM_SNOWMAN_HAT) ||
		(pAvatar[eAvatar_Dress] ==  EVENT_SHOPITEM_RUDOLP_DRESS && pAvatar[eAvatar_Hat] ==  EVENT_SHOPITEM_RUDOLP_HAT) ||
		(pAvatar[eAvatar_Dress] == EVENT_SHOPITEM_SNOWMAN_DRESS2 && pAvatar[eAvatar_Hat] == EVENT_SHOPITEM_SNOWMAN_HAT2) ||
		(pAvatar[eAvatar_Dress] ==  EVENT_SHOPITEM_RUDOLP_DRESS2 && pAvatar[eAvatar_Hat] ==  EVENT_SHOPITEM_RUDOLP_HAT2) ||
		(pAvatar[eAvatar_Dress] == EVENT_SHOPITEM_SNOWMAN_DRESS && pAvatar[eAvatar_Hat] == EVENT_SHOPITEM_SNOWMAN_HAT2) ||
		(pAvatar[eAvatar_Dress] ==  EVENT_SHOPITEM_RUDOLP_DRESS && pAvatar[eAvatar_Hat] ==  EVENT_SHOPITEM_RUDOLP_HAT2) ||
		(pAvatar[eAvatar_Dress] == EVENT_SHOPITEM_SNOWMAN_DRESS2 && pAvatar[eAvatar_Hat] == EVENT_SHOPITEM_SNOWMAN_HAT) ||
		(pAvatar[eAvatar_Dress] ==  EVENT_SHOPITEM_RUDOLP_DRESS2 && pAvatar[eAvatar_Hat] ==  EVENT_SHOPITEM_RUDOLP_HAT) )
		pPlayer->GetShopItemManager()->CalcAvatarOption();
*/
}
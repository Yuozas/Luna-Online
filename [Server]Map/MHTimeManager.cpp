// MHTimeManager.cpp: implementation of the CMHTimeManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MHTimeManager.h"
#include "mmsystem.h"


DWORD gCurTime = 0;		//�ʱ�ȭ �߰�
DWORD gTickTime;

#define TICK_PER_DAY 86400000		// 24 * 60 * 60 * 1000
#define TICK_PER_HOUR 3600000		// 60 * 60 * 1000
#define TICK_PER_MINUTE 60000		// 60 * 1000

#define DAY_PER_YEAR 360
#define DAY_PER_MONTH 30

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMHTimeManager::CMHTimeManager()
{
	m_MHDate = 0;
	m_MHTime = 0;
	m_lasttime	= 0;
	// desc_hseos_����Ʈ ��_01
	// S ����Ʈ �� �߰� added by hseos 2007.11.27
	GetLocalTime(&m_stLocalTime);
	GetLocalTime(&m_stOldLocalTime);
	// E ����Ʈ �� �߰� added by hseos 2007.11.27
}

CMHTimeManager::~CMHTimeManager()
{

}

void CMHTimeManager::Init(DWORD mhDate,DWORD mhTime)
{
	m_MHDate = mhDate;
	m_MHTime = mhTime;
}

void CMHTimeManager::Process()
{
	static bool bFirst = true;
//	static DWORD lasttime = 0;
	static DWORD curtime = 0;
	static int tempDay = 0;

	if( !bFirst )
	{
		curtime = timeGetTime();
		if( curtime < m_lasttime )		// DWORD ���� �Ѱ踦 �Ѿ��
			gTickTime = curtime - m_lasttime + 4294967295; //( 2^32 - 1 )
		else
			gTickTime = curtime - m_lasttime;

		if( gTickTime == 0 ) return;
		
		m_lasttime = curtime;
		
		gCurTime += gTickTime;
//		if( gTickTime > 2000 )
//			int a = 0;

		
		//////////////////////////////////////////////////////////////////////////
		// �����-_-a;
		m_MHTime += gTickTime;
		if(m_MHTime >= TICK_PER_DAY)
		{
			++m_MHDate;
			m_MHTime -= TICK_PER_DAY;
		}
	}	
	else
	{
		curtime = m_lasttime = timeGetTime();
		bFirst = false;
	}

	// desc_hseos_����Ʈ ��_01
	// S ����Ʈ �� �߰� added by hseos 2007.11.27
	ProcCurLocalTime();
	// E ����Ʈ �� �߰� added by hseos 2007.11.27
}


DWORD CMHTimeManager::GetNewCalcCurTime()	//curŸ�� ���� �޾ƿ��� ���� gCurTime, gTickTime�� �������.
{
	DWORD lcurtime = timeGetTime();
	DWORD lTickTime;
		
	if( lcurtime < m_lasttime )		// DWORD ���� �Ѱ踦 �Ѿ��
		lTickTime = lcurtime - m_lasttime + 4294967295; //( 2^32 - 1 )
	else
		lTickTime = lcurtime - m_lasttime;
	
	return gCurTime + lTickTime;
}


DWORD CMHTimeManager::GetMHDate()
{
	return m_MHDate;
}
DWORD CMHTimeManager::GetMHTime()
{
	return m_MHTime;
}

void CMHTimeManager::GetMHDate(BYTE& year,BYTE& month,BYTE& day)
{
	year = (BYTE)(m_MHDate / DAY_PER_YEAR) + 1;
	month = (BYTE)((m_MHDate - year) / DAY_PER_MONTH) + 1;
	day = (BYTE)m_MHDate % DAY_PER_MONTH + 1;  // -_-a; ���⿡�� ��� ���� 30�ϱ����� -_-	
}

void CMHTimeManager::GetMHTime(BYTE& hour,BYTE& minute)
{
	hour = (BYTE)m_MHTime / TICK_PER_HOUR;
	minute = (BYTE)(m_MHTime - hour) / TICK_PER_MINUTE;
}

// desc_hseos_����Ʈ ��_01
// S ����Ʈ �� �߰� added by hseos 2007.11.27
void CMHTimeManager::ProcCurLocalTime()
{
	static DWORD nBeforeTimeTick = gCurTime;

	// 60�� ���� üũ
	if (gCurTime-nBeforeTimeTick > 60000)
	{
		nBeforeTimeTick = gCurTime;
		GetLocalTime(&m_stLocalTime);
	}
}
// E ����Ʈ �� �߰� added by hseos 2007.11.27

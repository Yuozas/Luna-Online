// MHTimeManager.cpp: implementation of the CMHTimeManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MHTimeManager.h"
#include "mmsystem.h"


DWORD gCurTime = 0;		//0 �ʱ�ȭ �߰�...
DWORD gTickTime;
float gAntiGravity = 1.0f;

#define TICK_PER_DAY 86400000		// 24 * 60 * 60 * 1000

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CMHTimeManager)
CMHTimeManager::CMHTimeManager()
{
	m_MHDate = 0;
	m_MHTime = 0;

	m_lasttime = 0;

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
//	static DWORD aaa = 0;
//	aaa++;


	if( bFirst )
	{
		curtime = m_lasttime = GetTickCount();
		bFirst = false;
	}
	else
	{
		curtime = GetTickCount();		

		if( curtime < m_lasttime )		// DWORD ���� �Ѱ踦 �Ѿ��
			gTickTime = curtime - m_lasttime + 4294967295; //( 2^32 - 1 )
		else
			gTickTime = curtime - m_lasttime;
		m_lasttime = curtime;
		
		gCurTime += gTickTime;

		
		//////////////////////////////////////////////////////////////////////////
		// �����-_-a; 
		// ����� ������� �ʾ� ���ǰ� ���� �ð�� ����
		m_MHTime += gTickTime;
		if(m_MHTime >= TICK_PER_DAY)
		{
			++m_MHDate;
			m_MHTime -= TICK_PER_DAY;
		}
	}	
}

DWORD CMHTimeManager::GetNewCalcCurTime()	//curŸ�� ���� �޾ƿ��� ���� gCurTime, gTickTime�� �������.
{
	DWORD lcurtime = GetTickCount();
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
	year = (BYTE)(m_MHDate / 360) + 1;
	month = (BYTE)((m_MHDate - year) / 30) + 1;
	day = (BYTE)m_MHDate % 30 + 1;  // -_-a; ���⿡�� ��� ���� 30�ϱ����� -_-	
}

void CMHTimeManager::GetMHTime(BYTE& hour,BYTE& minute)
{
	hour = (BYTE)m_MHTime / 3600000;
	minute = (BYTE)(m_MHTime - hour) / 60000;
}
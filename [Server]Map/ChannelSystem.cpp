// ChannelSystem.cpp: implementation of the CChannelSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChannelSystem.h"
#include "MHFile.h"
#include "BattleSystem_Server.h"
#include "Object.h"
#include "NetWork.h"
#include "EventMapMgr.h"
#include "QuestMapMgr.h"
// desc_hseos_데이트 존_01
// S 데이트 존 추가 added by hseos 2007.11.25
#include "../hseos/Date/SHDateManager.h"
// E 데이트 존 추가 added by hseos 2007.11.25

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern int g_nServerSetNum;

CChannelSystem::CChannelSystem()
{
	m_dwCount = 0;
	m_bQuestMap = FALSE;
	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2008.01.10
	// ..클래스의 메모리값을 직접 설정하는 건 문제가 발생할 소지가 있음.(virtual 포인터 관련해서 등등...)
	// ..m_QMChannel 의 생성자가 맴버의 값들을 초기화 할 테니 안 해도 됨)
	// memset( m_QMChannel, 0, sizeof(CChannel)*1000 );
	// E 데이트 존 추가 added by hseos 2008.01.10

	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.25
	m_nChallengeZoneLastChannelID = 0;
	ZeroMemory(m_nChallengeZoneAllocatedCH,		sizeof(m_nChallengeZoneAllocatedCH));
	ZeroMemory(m_nChallengeZoneAgentChannelID, sizeof(m_nChallengeZoneAgentChannelID));
	m_nChallengeZoneAllocatedCHNum = 0;
	m_nChallengeZonePlayerAgentChannelID= 0;
	// E 데이트 존 추가 added by hseos 2007.11.25
}

CChannelSystem::~CChannelSystem()
{
	for(DWORD i=0; i<m_dwCount; ++i)
	{
		if(m_Channel[i])
		{		
			delete m_Channel[i];
			m_Channel[i] = NULL;
		}
	}
}

void CChannelSystem::LoadChannelInfo()
{
	CMHFile file;
	BYTE Kind, num = 0;
	char value[64] = {0,};
	
	char filename[256];
	sprintf(filename,"serverset/%d/ChannelInfo.bin",g_nServerSetNum);

	file.Init(filename, "rb");

	if(file.IsInited() == FALSE)
	{
		MessageBox(NULL,"Can't Open ChannelInfo File", NULL, NULL);
		return;
	}

	while(1)
	{
		if(file.IsEOF())
			break;
		
		strcpy( value, strupr(file.GetString()) );

		if( strcmp( value,"*CHANNEL" ) == 0 )
		{
			Kind = file.GetByte();
			num = file.GetByte();
			file.GetStringInQuotation( m_ChannelName );
		}
	}
	file.Release();
}

void CChannelSystem::CreateChannel(BYTE Kind, BYTE num)
{
	for(BYTE i=0; i<num; ++i)
	{			
		 DWORD ChannelIDX = BATTLESYSTEM->CreateChannel(); // 그리드 아이디 == ChannelIDX
		 m_Channel[i] = new CChannel;
		 m_Channel[i]->SetChannelID(ChannelIDX);
	} 
	
	m_dwCount = num;
	m_bQuestMap = FALSE;
}

void CChannelSystem::IncreasePlayerNum(DWORD ChannelID)
{
	if( m_bQuestMap )	return;

	ASSERT(ChannelID);
	if(ChannelID > m_dwCount)
	{
		ASSERT(0); return;
	}
	m_Channel[ChannelID-1]->IncreaseNum();
}

void CChannelSystem::DecreasePlayerNum(DWORD ChannelID)
{
	if( m_bQuestMap )	return;

	ASSERT(ChannelID);
	if(ChannelID > m_dwCount)
	{
		return;
	}
	m_Channel[ChannelID-1]->DecreaseNum();
}


void CChannelSystem::SendChannelInfo(MSG_DWORD* pInfo, DWORD dwConnectionIndex)
{
	if(g_pServerSystem->GetStart() == FALSE || m_bQuestMap )
	{
		MSG_BYTE nmsg;
		nmsg.Category = MP_USERCONN;
		nmsg.Protocol = MP_USERCONN_CHANNELINFO_NACK;
		nmsg.bData = 0;
		nmsg.dwObjectID = pInfo->dwObjectID;
		g_Network.Send2Server(dwConnectionIndex,(char*)&nmsg,sizeof(nmsg));
		return;
	}

	MSG_CHANNEL_INFO msg;
	memset(&msg, 0, sizeof(MSG_CHANNEL_INFO));
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHANNELINFO_ACK;
	SafeStrCpy(msg.ChannelName, m_ChannelName, MAX_CHANNEL_NAME+1);

	for(DWORD i=0; i<m_dwCount; ++i)
	{
		msg.PlayerNum[i] = m_Channel[i]->GetPlayerNum();
	}
	msg.dwObjectID = pInfo->dwObjectID;
	msg.dwUniqueIDinAgent = pInfo->dwData;
	msg.Count = (BYTE)m_dwCount;
	g_Network.Send2Server(dwConnectionIndex,(char*)&msg,sizeof(msg));
}

DWORD CChannelSystem::GetChannelID(int num)
{
	if( m_bQuestMap )	return 0;

	if(((DWORD)num >= m_dwCount) || (num < 0))
	{
		ASSERT(0);
		return 0;
	}
		
	return m_Channel[num]->GetChannelID();
}

void CChannelSystem::SendChannelInfoToMS( DWORD dwConnectionIndex )
{
	if( m_bQuestMap )	return;
	
	MSG_CHANNEL_INFO msg;
	memset(&msg, 0, sizeof(MSG_CHANNEL_INFO));
	msg.Category = MP_MORNITORMAPSERVER;
	msg.Protocol = MP_MORNITORMAPSERVER_QUERY_CHANNELINFO_ACK;
	SafeStrCpy(msg.ChannelName, m_ChannelName, MAX_CHANNEL_NAME+1);
	for(DWORD i=0; i<m_dwCount; ++i)
	{
		msg.PlayerNum[i] = m_Channel[i]->GetPlayerNum();
	}
	msg.Count = (BYTE)m_dwCount;
	g_Network.Send2Server(dwConnectionIndex,(char*)&msg,sizeof(msg));
}

void CChannelSystem::InitQuestMapChannel()
{
	m_bQuestMap = TRUE;
}

// desc_hseos_데이트 존_01
// S 데이트 존 추가 added by hseos 2007.11.27
//DWORD CChannelSystem::CreateQuestMapChannel() 
DWORD CChannelSystem::CreateQuestMapChannel(DWORD nFirstEnterPlayerID) 
// E 데이트 존 추가 added by hseos 2007.11.27
{
	if( !m_bQuestMap )	return 0;

	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.25
	// ..파트너에 의해 이미 채널이 할당되어 있으면 그 채널을 사용한다.
	if (g_csDateManager.IsChallengeZoneHere())
	{
		// 에이전트에서 1000 이상으로 보낸다. 1000 이라는 값은 기본으로 설정한 채널과 구분하기 위한 수치임.
		if (m_nChallengeZonePlayerAgentChannelID >= 1000)
		{
			// 여기에서의 카운트 1000에 대해서 설명.
			// 동시에 2000명이 챌린지 존에 입장할 경우, 1000명이 먼저 챌린지 존에 도착할 수 있다.
			// 그러면 나중에 들어오는 플레이어는 먼저 들어온 1000명의 식별코드 중 자신의 식별코드와 일치하는 경우에
			// 먼저 들어온 플레이어의 채널ID를 공유한다.
			// 일반(정상)적이라면 2000명이 챌린지 존에 입장할 경우, 2000명의 일부가 먼저 들어오고, 그 일부의 파트너가 들어오고,
			// 또 나머지의 일부가 들어오고, 그 일부의 파트너가 들어오고 할 텐데, 최악의 경우는
			// 혼자들어온 경우만 1000건이 될 수가 있다. 이와 같이 혼자들어온 최악의 경우의 수가 1000 임.
			DWORD nCnt = 1000;
			if (nCnt > m_nChallengeZoneAllocatedCHNum) nCnt = m_nChallengeZoneAllocatedCHNum;

			for(DWORD i=0; i<nCnt; i++)
			{
				if (m_nChallengeZoneAgentChannelID[i] == m_nChallengeZonePlayerAgentChannelID)
				{
					DWORD nChannel = m_nChallengeZoneAllocatedCH[i][0];

					// 챌린지 시작 설정
					g_csDateManager.SetChallengeZoneStart(TRUE);
					g_csDateManager.SetChallengeZoneFirstEnterPlayerID(m_nChallengeZoneAllocatedCH[i][1]);
/*					if (g_pServerSystem->IsTestServer())
					{
						g_Console.LOG(4, "Share ChallengeZone Channel:%d FirstEnterPlayerID:%d Agent CHID:%d)", nChannel, m_nChallengeZoneAllocatedCH[i][1], m_nChallengeZonePlayerAgentChannelID);
					}
*/					// 잊지 말고 초기화
					m_nChallengeZoneAllocatedCH[i][0]	= 0;
					m_nChallengeZoneAllocatedCH[i][1]	= 0;
					m_nChallengeZoneAgentChannelID[i]	= 0;
					return nChannel;
				}
			}
		}
	}
	// E 데이트 존 추가 added by hseos 2007.11.25

	DWORD ChannelIDX = BATTLESYSTEM->CreateChannel();
	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2008.01.10
	// for( int i = 0; i < 1000; ++i )
	for( int i = 0; i < MAX_QUEST_OR_CHALLENGE_CHANNEL_NUM; ++i )
	// E 데이트 존 추가 added by hseos 2008.01.10
	{
		if( m_QMChannel[i].GetChannelID() == 0 )
			break;
	}
	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2008.01.10
	// if( i > 999 )	return 0;
	if( i > MAX_QUEST_OR_CHALLENGE_CHANNEL_NUM-1 )	return 0;
	// E 데이트 존 추가 added by hseos 2008.01.10

	m_QMChannel[i].SetChannelID( ChannelIDX );
	
	++m_dwCount;

	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.25
	if (g_csDateManager.IsChallengeZoneHere())
	{
		for(DWORD i=0; i<1000; i++)
		{
			if (m_nChallengeZoneAllocatedCH[i][0] == 0)
			{
				m_nChallengeZoneAllocatedCH[i][0]	= ChannelIDX;
				m_nChallengeZoneAllocatedCH[i][1]	= nFirstEnterPlayerID;
				m_nChallengeZoneAgentChannelID[i]	= m_nChallengeZonePlayerAgentChannelID;
				m_nChallengeZoneLastChannelID		= ChannelIDX;
				m_nChallengeZoneAllocatedCHNum++;
				// ..파트너없이 혼자 접속되었을 경우에 대비해서 일정시간 파트너의 접속이 없어서 초기화가 되지 않은 정보는 초기화한다.
				if (i >= 500)
				{
					m_nChallengeZoneAllocatedCH[i-500][0]	= 0;
					m_nChallengeZoneAllocatedCH[i-500][1]	= 0;
					m_nChallengeZoneAgentChannelID[i-500]	= 0;
				}
/*				if (g_pServerSystem->IsTestServer())
				{
					g_Console.LOG(4, "Create ChallengeZone Channel:%d ChannelQty(or EnterFreq):%d Agent CHID:%d", ChannelIDX, m_nChallengeZoneAllocatedCHNum, m_nChallengeZonePlayerAgentChannelID);
				}
*/				break;
			}
		}
	}
	// E 데이트 존 추가 added by hseos 2007.11.25

	return ChannelIDX;
}

// desc_hseos_데이트 존_01
// S 데이트 존 추가 added by hseos 2007.11.25
BOOL CChannelSystem::GetChallengeZonePartnerCH(DWORD nChallengeZoneAgentChannelID)
{
	DWORD nCnt = 1000;
	if (nCnt > m_nChallengeZoneAllocatedCHNum) nCnt = m_nChallengeZoneAllocatedCHNum;

	for(DWORD i=0; i<nCnt; i++)
	{
		if (m_nChallengeZoneAgentChannelID[i] == nChallengeZoneAgentChannelID)
		{
			return m_nChallengeZoneAllocatedCH[i][0];
		}
	}

	return 0;
}
// E 데이트 존 추가 added by hseos 2007.11.25

void CChannelSystem::DestroyQuestMapChannel( DWORD dwChannel )
{
	if( !m_bQuestMap )	return;

	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2008.01.10
	// for( int i = 0; i < 1000; ++i )
	for( int i = 0; i < MAX_QUEST_OR_CHALLENGE_CHANNEL_NUM; ++i )
	// E 데이트 존 추가 added by hseos 2008.01.10
	{
		if( m_QMChannel[i].GetChannelID() == dwChannel )
		{
			m_QMChannel[i].SetChannelID( 0 );
			break;
		}
	}
	--m_dwCount;

	BATTLESYSTEM->DestroyChannel( dwChannel );

	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.28
	// ..2명의 플레이어 입장에 사용하는 채널과 파트너ID 값을 초기화한다.
	// ..초기화하지 않을 경우, 먼저 들어온 플레이어가 접속을 끊고, 채널을 지웠는데
	// ..나중에 들어온 플레이어가 지운 채널을 할당받아서 거기로 들어간다.
	// ..이럴 경우 아마 다른 팀과 같은 채널을 사용하게 된다거나 뭔가 꼬이는 일이 발생할 듯..
	if (g_csDateManager.IsChallengeZoneHere())
	{
/*		if (g_pServerSystem->IsTestServer())
		{
			g_Console.LOG(4, "Delete ChallengeZone Channel:%d", dwChannel);
		}
*/
		DWORD nCnt = 1000;
		if (nCnt > m_nChallengeZoneAllocatedCHNum) nCnt = m_nChallengeZoneAllocatedCHNum;

		for(DWORD i=0; i<nCnt; i++)
		{
			if (m_nChallengeZoneAllocatedCH[i][0] == dwChannel)
			{
				m_nChallengeZoneAllocatedCH[i][0]	= 0;
				m_nChallengeZoneAllocatedCH[i][1]	= 0;
				m_nChallengeZoneAgentChannelID[i]	= 0;
/*				if (g_pServerSystem->IsTestServer())
				{
					g_Console.LOG(4, "Delete ChallengeZone ChannelInfo(Alone):%d", dwChannel);
				}
*/				break;
			}
		}
	}
	// E 데이트 존 추가 added by hseos 2007.11.28
}

void CChannelSystem::Init( WORD wMapNum, WORD wChannelNum )
{
	if( QUESTMAPMGR->Init( wMapNum ) )	return;
	if( wMapNum == EVENTMAPNUM )
	{
		EVENTMAPMGR->Init( wMapNum, 10 );
	}
	else
	{
		LoadChannelInfo();

		int kind = 0;
		CreateChannel(kind, wChannelNum);
	}
}

DWORD CChannelSystem::GetRandomChannelID()
{
	if( m_bQuestMap )	return 0;

	int num = 0;
//	if( m_dwCount > 1 )
//		num = random(0, GetChannelCount()-2);
	if( m_dwCount > 1 )
		num = random(0, GetChannelCount()-1);
	return m_Channel[num]->GetChannelID();
}

DWORD CChannelSystem::GetLastChannelID()
{
	if( m_bQuestMap )	return 0;

	return m_Channel[m_dwCount-1]->GetChannelID();
}

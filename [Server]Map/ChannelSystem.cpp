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
// desc_hseos_����Ʈ ��_01
// S ����Ʈ �� �߰� added by hseos 2007.11.25
#include "../hseos/Date/SHDateManager.h"
// E ����Ʈ �� �߰� added by hseos 2007.11.25

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern int g_nServerSetNum;

CChannelSystem::CChannelSystem()
{
	m_dwCount = 0;
	m_bQuestMap = FALSE;
	// desc_hseos_����Ʈ ��_01
	// S ����Ʈ �� �߰� added by hseos 2008.01.10
	// ..Ŭ������ �޸𸮰��� ���� �����ϴ� �� ������ �߻��� ������ ����.(virtual ������ �����ؼ� ���...)
	// ..m_QMChannel �� �����ڰ� �ɹ��� ������ �ʱ�ȭ �� �״� �� �ص� ��)
	// memset( m_QMChannel, 0, sizeof(CChannel)*1000 );
	// E ����Ʈ �� �߰� added by hseos 2008.01.10

	// desc_hseos_����Ʈ ��_01
	// S ����Ʈ �� �߰� added by hseos 2007.11.25
	m_nChallengeZoneLastChannelID = 0;
	ZeroMemory(m_nChallengeZoneAllocatedCH,		sizeof(m_nChallengeZoneAllocatedCH));
	ZeroMemory(m_nChallengeZoneAgentChannelID, sizeof(m_nChallengeZoneAgentChannelID));
	m_nChallengeZoneAllocatedCHNum = 0;
	m_nChallengeZonePlayerAgentChannelID= 0;
	// E ����Ʈ �� �߰� added by hseos 2007.11.25
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
		 DWORD ChannelIDX = BATTLESYSTEM->CreateChannel(); // �׸��� ���̵� == ChannelIDX
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

// desc_hseos_����Ʈ ��_01
// S ����Ʈ �� �߰� added by hseos 2007.11.27
//DWORD CChannelSystem::CreateQuestMapChannel() 
DWORD CChannelSystem::CreateQuestMapChannel(DWORD nFirstEnterPlayerID) 
// E ����Ʈ �� �߰� added by hseos 2007.11.27
{
	if( !m_bQuestMap )	return 0;

	// desc_hseos_����Ʈ ��_01
	// S ����Ʈ �� �߰� added by hseos 2007.11.25
	// ..��Ʈ�ʿ� ���� �̹� ä���� �Ҵ�Ǿ� ������ �� ä���� ����Ѵ�.
	if (g_csDateManager.IsChallengeZoneHere())
	{
		// ������Ʈ���� 1000 �̻����� ������. 1000 �̶�� ���� �⺻���� ������ ä�ΰ� �����ϱ� ���� ��ġ��.
		if (m_nChallengeZonePlayerAgentChannelID >= 1000)
		{
			// ���⿡���� ī��Ʈ 1000�� ���ؼ� ����.
			// ���ÿ� 2000���� ç���� ���� ������ ���, 1000���� ���� ç���� ���� ������ �� �ִ�.
			// �׷��� ���߿� ������ �÷��̾�� ���� ���� 1000���� �ĺ��ڵ� �� �ڽ��� �ĺ��ڵ�� ��ġ�ϴ� ��쿡
			// ���� ���� �÷��̾��� ä��ID�� �����Ѵ�.
			// �Ϲ�(����)���̶�� 2000���� ç���� ���� ������ ���, 2000���� �Ϻΰ� ���� ������, �� �Ϻ��� ��Ʈ�ʰ� ������,
			// �� �������� �Ϻΰ� ������, �� �Ϻ��� ��Ʈ�ʰ� ������ �� �ٵ�, �־��� ����
			// ȥ�ڵ��� ��츸 1000���� �� ���� �ִ�. �̿� ���� ȥ�ڵ��� �־��� ����� ���� 1000 ��.
			DWORD nCnt = 1000;
			if (nCnt > m_nChallengeZoneAllocatedCHNum) nCnt = m_nChallengeZoneAllocatedCHNum;

			for(DWORD i=0; i<nCnt; i++)
			{
				if (m_nChallengeZoneAgentChannelID[i] == m_nChallengeZonePlayerAgentChannelID)
				{
					DWORD nChannel = m_nChallengeZoneAllocatedCH[i][0];

					// ç���� ���� ����
					g_csDateManager.SetChallengeZoneStart(TRUE);
					g_csDateManager.SetChallengeZoneFirstEnterPlayerID(m_nChallengeZoneAllocatedCH[i][1]);
/*					if (g_pServerSystem->IsTestServer())
					{
						g_Console.LOG(4, "Share ChallengeZone Channel:%d FirstEnterPlayerID:%d Agent CHID:%d)", nChannel, m_nChallengeZoneAllocatedCH[i][1], m_nChallengeZonePlayerAgentChannelID);
					}
*/					// ���� ���� �ʱ�ȭ
					m_nChallengeZoneAllocatedCH[i][0]	= 0;
					m_nChallengeZoneAllocatedCH[i][1]	= 0;
					m_nChallengeZoneAgentChannelID[i]	= 0;
					return nChannel;
				}
			}
		}
	}
	// E ����Ʈ �� �߰� added by hseos 2007.11.25

	DWORD ChannelIDX = BATTLESYSTEM->CreateChannel();
	// desc_hseos_����Ʈ ��_01
	// S ����Ʈ �� �߰� added by hseos 2008.01.10
	// for( int i = 0; i < 1000; ++i )
	for( int i = 0; i < MAX_QUEST_OR_CHALLENGE_CHANNEL_NUM; ++i )
	// E ����Ʈ �� �߰� added by hseos 2008.01.10
	{
		if( m_QMChannel[i].GetChannelID() == 0 )
			break;
	}
	// desc_hseos_����Ʈ ��_01
	// S ����Ʈ �� �߰� added by hseos 2008.01.10
	// if( i > 999 )	return 0;
	if( i > MAX_QUEST_OR_CHALLENGE_CHANNEL_NUM-1 )	return 0;
	// E ����Ʈ �� �߰� added by hseos 2008.01.10

	m_QMChannel[i].SetChannelID( ChannelIDX );
	
	++m_dwCount;

	// desc_hseos_����Ʈ ��_01
	// S ����Ʈ �� �߰� added by hseos 2007.11.25
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
				// ..��Ʈ�ʾ��� ȥ�� ���ӵǾ��� ��쿡 ����ؼ� �����ð� ��Ʈ���� ������ ��� �ʱ�ȭ�� ���� ���� ������ �ʱ�ȭ�Ѵ�.
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
	// E ����Ʈ �� �߰� added by hseos 2007.11.25

	return ChannelIDX;
}

// desc_hseos_����Ʈ ��_01
// S ����Ʈ �� �߰� added by hseos 2007.11.25
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
// E ����Ʈ �� �߰� added by hseos 2007.11.25

void CChannelSystem::DestroyQuestMapChannel( DWORD dwChannel )
{
	if( !m_bQuestMap )	return;

	// desc_hseos_����Ʈ ��_01
	// S ����Ʈ �� �߰� added by hseos 2008.01.10
	// for( int i = 0; i < 1000; ++i )
	for( int i = 0; i < MAX_QUEST_OR_CHALLENGE_CHANNEL_NUM; ++i )
	// E ����Ʈ �� �߰� added by hseos 2008.01.10
	{
		if( m_QMChannel[i].GetChannelID() == dwChannel )
		{
			m_QMChannel[i].SetChannelID( 0 );
			break;
		}
	}
	--m_dwCount;

	BATTLESYSTEM->DestroyChannel( dwChannel );

	// desc_hseos_����Ʈ ��_01
	// S ����Ʈ �� �߰� added by hseos 2007.11.28
	// ..2���� �÷��̾� ���忡 ����ϴ� ä�ΰ� ��Ʈ��ID ���� �ʱ�ȭ�Ѵ�.
	// ..�ʱ�ȭ���� ���� ���, ���� ���� �÷��̾ ������ ����, ä���� �����µ�
	// ..���߿� ���� �÷��̾ ���� ä���� �Ҵ�޾Ƽ� �ű�� ����.
	// ..�̷� ��� �Ƹ� �ٸ� ���� ���� ä���� ����ϰ� �ȴٰų� ���� ���̴� ���� �߻��� ��..
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
	// E ����Ʈ �� �߰� added by hseos 2007.11.28
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

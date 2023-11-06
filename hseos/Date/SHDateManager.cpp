/*********************************************************************

	 ����		: SHDateManager.cpp
	 �ۼ���		: hseos
	 �ۼ���		: 2007/11/16

	 ���ϼ���	: CSHDateManager Ŭ������ �ҽ�

 *********************************************************************/

#include "stdafx.h"
#include "MHFile.h"
#include "SHDateManager.h"

#if defined(_AGENTSERVER)
	#include "Network.h"
	#include "AgentDBMsgParser.h"
	#include "AgentNetworkMsgParser.h"
#elif defined(_MAPSERVER_)
	#include "UserTable.h"
	#include "Player.h"
	#include "PackedData.h"
	#include "Network.h"
	#include "../[CC]Header/GameResourceManager.h"
	#include "MapDBMsgParser.h"
	#include "Party.h"
	#include "PartyManager.h"
	#include "MHTimeManager.h"
	#include "RecallManager.h"
#else
	#include "../Player.h"
	#include "../ChatManager.h"
	#include "../GameIn.h"
	#include "../ObjectManager.h"
	#include "../WindowIDEnum.h"
	#include "../cMsgBox.h"
	#include "../interface/cWindowManager.h"
	#include "../ResidentRegist/SHResidentRegistManager.h"
	#include "PartyManager.h"
	#include "../DateMatchingDlg.h"
	#include "mhMap.h"
	#include "cStatic.h"
	#include "SHChallengeZoneClearNo1Dlg.h"
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CSHDateManager
//
CSHDateManager g_csDateManager;


UINT	CSHDateManager::NOTICE_HEART_MATCHING_POING = 0;						// ��Ʈ �˸��� ��Ī ����Ʈ
UINT	CSHDateManager::ENTER_CHALLENGE_ZONE_MATCHING_POINT = 0;				// ç���� �� ���� ���� ��Ī ����Ʈ
UINT	CSHDateManager::ENTER_CHALLENGE_ZONE_FREQ_PER_DAY = 0;					// ç���� �� ���� ���� ȸ��(�Ϸ�)
UINT	CSHDateManager::ENTER_CHALLENGE_ZONE_SECTION_NUM = 0;					// ç���� �� ���� ����
DWORD	CSHDateManager::CHALLENGE_ZONE_START_DELAY_TIME = 0;					// ç���� �� ���� ��� �ð�
DWORD	CSHDateManager::CHALLENGE_ZONE_END_DELAY_TIME = 0;						// ç���� �� ���� ��� �ð�

UINT	CSHDateManager::CHALLENGE_ZONE_MOTION_NUM_START = 0;					// ç���� �� ���� ��� ��ȣ
UINT	CSHDateManager::CHALLENGE_ZONE_MOTION_NUM_SUCCESS = 0;					// ç���� �� ���� ��� ��ȣ
UINT	CSHDateManager::CHALLENGE_ZONE_MOTION_NUM_SUCCESS_LEAST_CLEAR_TIME = 0;	// ç���� �� �ִ� �ð� Ŭ���� ���� ��� ��ȣ
UINT	CSHDateManager::CHALLENGE_ZONE_MOTION_NUM_FAIL = 0;						// ç���� �� ���� ��� ��ȣ
// -------------------------------------------------------------------------------------------------------------------------------------
// CSHFamilyManager Method																										  ������
//
CSHDateManager::CSHDateManager()
{
	m_bIsChallengeZone = FALSE;

	m_pnDateZoneMoveIndexList = NULL;
	m_nDateZoneMoveIndexNum = 0;

	m_pstChallengeZoneMoveIndexList = NULL;
	m_nChallengeZoneMoveIndexNum = 0;

	// �Ϲ� �⺻ ä�ΰ��� �ĺ��� ���� �⺻������ 1000 �� ����
	m_nChallengeZoneEnterNum = 1000;

	m_pstChallengeZoneMonsterGroupSection = NULL;
	m_nChallengeZoneMonsterGroupSectionNum = 0;

	m_pstChallengeZoneSectionMonLevel = NULL;
	m_nChallengeZoneSectionNum = 0;

	m_nChallengeZoneState = 0;
	m_nChallengeZoneTime = 0;
	m_nChallengeZoneTimeTick = 0;

	m_bChallengeZoneStart = FALSE;
	m_nChallengeZoneFirstEnterPlayerID = 0;

	m_nChallengeZoneLeastClearTime = 0;
	LoadDateInfo();
}

// -------------------------------------------------------------------------------------------------------------------------------------
// ~CSHFamilyManager Method																										  �ı���
//
CSHDateManager::~CSHDateManager()
{
	SAFE_DELETE_ARRAY(m_pnDateZoneMoveIndexList);
	SAFE_DELETE_ARRAY(m_pstChallengeZoneMoveIndexList);

	if (m_pstChallengeZoneMonsterGroupSection)
	{
		for(UINT i=0; i<m_nChallengeZoneMonsterGroupSectionNum; i++)
		{
			for(UINT j=0; j<m_pstChallengeZoneMonsterGroupSection[i].nGroupNum; j++)
			{
				SAFE_DELETE_ARRAY(m_pstChallengeZoneMonsterGroupSection[i].pstGroup[j].pstMonster);
			}
			SAFE_DELETE_ARRAY(m_pstChallengeZoneMonsterGroupSection[i].pstGroup);
		}
		SAFE_DELETE_ARRAY(m_pstChallengeZoneMonsterGroupSection);
	}
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  Init Method																													  �ʱ�ȭ
//
VOID CSHDateManager::Init()
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	if (IsChallengeZone(MAP->GetMapNum()))
	{
		m_bIsChallengeZone = TRUE;
		return;
	}

	m_bIsChallengeZone = FALSE;
	m_nChallengeZoneState = 0;
	m_nChallengeZoneTime = 0;
	m_nChallengeZoneTimeTick = 0;
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  LoadDateInfo Method																										 ������ �б�
//
BOOL CSHDateManager::LoadDateInfo()
{
	char szLine[MAX_PATH], szFile[MAX_PATH];
	int	 nKind = 0, nCnt = 0;
	CMHFile fp;

	sprintf(szFile, "./System/Resource/DateMatching.bin");
	fp.Init(szFile, "rb");
	if(!fp.IsInited()) return FALSE;

	// ���� ���� �б�
	while(1)
	{
		if(fp.IsEOF()) break;
		fp.GetLine(szLine, sizeof(szLine));
		if (strstr(szLine, "//") ||
			IsEmptyLine(szLine))
		{
			continue;			
		}
		else if (strstr(szLine, "END_KIND")) 
		{
			nCnt = 0;
			nKind++;
			continue;
		}

		switch(nKind)
		{
		case 1: m_nDateZoneMoveIndexNum++;			break;
		case 2: m_nChallengeZoneMoveIndexNum++;		break;
		case 4: ENTER_CHALLENGE_ZONE_SECTION_NUM++;	break;
		}
		
		nCnt++;
	}
	fp.Release();

	// ����Ʈ ��
	SAFE_DELETE_ARRAY(m_pnDateZoneMoveIndexList);
	m_pnDateZoneMoveIndexList = new UINT[m_nDateZoneMoveIndexNum];
	ZeroMemory(m_pnDateZoneMoveIndexList, sizeof(*m_pnDateZoneMoveIndexList)*m_nDateZoneMoveIndexNum);
	// ç���� ��
	SAFE_DELETE_ARRAY(m_pstChallengeZoneMoveIndexList);
	m_pstChallengeZoneMoveIndexList = new stCHALLENGEZONE_MAPINFO[m_nChallengeZoneMoveIndexNum];
	ZeroMemory(m_pstChallengeZoneMoveIndexList, sizeof(*m_pstChallengeZoneMoveIndexList)*m_nChallengeZoneMoveIndexNum);
	// ç���� �� ����
	SAFE_DELETE_ARRAY(m_pstChallengeZoneSectionMonLevel);
	m_pstChallengeZoneSectionMonLevel = new stCHALLENGEZONE_SECTION_MONLEVEL[ENTER_CHALLENGE_ZONE_SECTION_NUM];
	ZeroMemory(m_pstChallengeZoneSectionMonLevel, sizeof(*m_pstChallengeZoneSectionMonLevel)*ENTER_CHALLENGE_ZONE_SECTION_NUM);
	
	// ������ �б�
	nKind = 0;
	nCnt = 0;
	fp.Init(szFile, "rb");
	if(!fp.IsInited()) return FALSE;

	while(1)
	{
		if(fp.IsEOF()) break;
		fp.GetLine(szLine, sizeof(szLine));
		if (strstr(szLine, "//") ||
			IsEmptyLine(szLine))
		{
			continue;			
		}
		else if (strstr(szLine, "END_KIND")) 
		{
			nCnt = 0;
			nKind++;
			continue;
		}

		switch(nKind)
		{
		case 0:		sscanf(szLine, "%d %d",			&NOTICE_HEART_MATCHING_POING,
													&ENTER_CHALLENGE_ZONE_MATCHING_POINT);				break;

		case 1:		sscanf(szLine, "%d",			&m_pnDateZoneMoveIndexList[nCnt]);					break;
		case 2:		sscanf(szLine, "%d %d",			&m_pstChallengeZoneMoveIndexList[nCnt].nMapNum,
													&m_pstChallengeZoneMoveIndexList[nCnt].nMoveIndex);	break;

		case 3:		sscanf(szLine, "%d %d %d",		&ENTER_CHALLENGE_ZONE_FREQ_PER_DAY,
													&CHALLENGE_ZONE_START_DELAY_TIME,
													&CHALLENGE_ZONE_END_DELAY_TIME);					break;

		case 4:		sscanf(szLine, "%d %d %d",		&m_pstChallengeZoneSectionMonLevel[nCnt].nStart,
													&m_pstChallengeZoneSectionMonLevel[nCnt].nEnd,
													&m_pstChallengeZoneSectionMonLevel[nCnt].nLimitTime);	
			
					m_pstChallengeZoneSectionMonLevel[nCnt].nLimitTime += CHALLENGE_ZONE_START_DELAY_TIME;
																										break;
		case 5:		sscanf(szLine, "%d %d %d %d",	&CHALLENGE_ZONE_MOTION_NUM_START,
													&CHALLENGE_ZONE_MOTION_NUM_SUCCESS,
													&CHALLENGE_ZONE_MOTION_NUM_SUCCESS_LEAST_CLEAR_TIME,
													&CHALLENGE_ZONE_MOTION_NUM_FAIL);					break;
		}
		
		nCnt++;
	}
	fp.Release();

	return TRUE;
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  LoadChallengeMonsterInfo Method																			ç���� �� ���� ������ �б�
//
BOOL CSHDateManager::LoadChallengeMonsterInfo(int nMapNum)
{
#if defined(_MAPSERVER_)
	if (IsChallengeZone(nMapNum) == FALSE) return TRUE;
	m_bIsChallengeZone = TRUE;

	if (m_pstChallengeZoneMonsterGroupSection)
	{
		for(UINT i=0; i<m_nChallengeZoneMonsterGroupSectionNum; i++)
		{
			for(UINT j=0; j<m_pstChallengeZoneMonsterGroupSection[i].nGroupNum; j++)
			{
				SAFE_DELETE_ARRAY(m_pstChallengeZoneMonsterGroupSection[i].pstGroup[j].pstMonster);
			}
			SAFE_DELETE_ARRAY(m_pstChallengeZoneMonsterGroupSection[i].pstGroup);
		}
		SAFE_DELETE_ARRAY(m_pstChallengeZoneMonsterGroupSection);
	}

	// ���� ������ŭ ����
	m_pstChallengeZoneMonsterGroupSection = new stCHALLENGEZONE_MONSTER_GROUP_SECTION[ENTER_CHALLENGE_ZONE_SECTION_NUM];
	ZeroMemory(m_pstChallengeZoneMonsterGroupSection, sizeof(*m_pstChallengeZoneMonsterGroupSection)*ENTER_CHALLENGE_ZONE_SECTION_NUM);
	for(UINT nSection=0; nSection<ENTER_CHALLENGE_ZONE_SECTION_NUM; nSection++)
	{
		char szLine[MAX_PATH], szFile[MAX_PATH];
		int	 nKind = 0, nCnt = 0;
		CMHFile fp;

		sprintf(szFile, "./System/Resource/ChallengeZoneMonster%d%02d.bin", nMapNum, nSection+1);
		fp.Init(szFile, "rb");
		if(!fp.IsInited())
		{
			char szTmp[256];
			sprintf(szTmp, "%s ������ �������� �ʽ��ϴ�.", szFile);
			ASSERTMSG(0, szTmp);
			return FALSE;
		}

		// �׷� ���� �б�
		while(1)
		{
			if(fp.IsEOF()) break;
			fp.GetLine(szLine, sizeof(szLine));
			if (strstr(szLine, "//") ||
				IsEmptyLine(szLine))
			{
				continue;			
			}
			else if (strstr(szLine, "END_KIND")) 
			{
				nCnt = 0;
				nKind++;
				m_pstChallengeZoneMonsterGroupSection[nSection].nGroupNum++;
				continue;
			}

			nCnt++;
		}
		fp.Release();

		m_pstChallengeZoneMonsterGroupSection[nSection].pstGroup = new stCHALLENGEZONE_MONSTER_GROUP[m_pstChallengeZoneMonsterGroupSection[nSection].nGroupNum];
		ZeroMemory(m_pstChallengeZoneMonsterGroupSection[nSection].pstGroup, sizeof(*m_pstChallengeZoneMonsterGroupSection[nSection].pstGroup)*m_pstChallengeZoneMonsterGroupSection[nSection].nGroupNum);

		// ���� ���� �б�
		nKind = 0;
		nCnt = 0;
		fp.Init(szFile, "rb");
		if(!fp.IsInited()) return FALSE;

		while(1)
		{
			if(fp.IsEOF()) break;
			fp.GetLine(szLine, sizeof(szLine));
			if (strstr(szLine, "//") ||
				IsEmptyLine(szLine))
			{
				continue;			
			}
			else if (strstr(szLine, "END_KIND")) 
			{
				m_pstChallengeZoneMonsterGroupSection[nSection].pstGroup[nKind].nMonsterNum = nCnt;
				nCnt = 0;
				nKind++;
				continue;
			}

			nCnt++;
		}
		fp.Release();

		for(UINT i=0; i<m_pstChallengeZoneMonsterGroupSection[nSection].nGroupNum; i++)
		{
			m_pstChallengeZoneMonsterGroupSection[nSection].pstGroup[i].pstMonster = new stCHALLENGEZONE_MONSTER[m_pstChallengeZoneMonsterGroupSection[nSection].pstGroup[i].nMonsterNum];
			ZeroMemory(m_pstChallengeZoneMonsterGroupSection[nSection].pstGroup[i].pstMonster, sizeof(*m_pstChallengeZoneMonsterGroupSection[nSection].pstGroup[i].pstMonster)*m_pstChallengeZoneMonsterGroupSection[nSection].pstGroup[i].nMonsterNum);
		}

		//  ���� ������ ����
		nKind = 0;
		nCnt = 0;
		fp.Init(szFile, "rb");
		if(!fp.IsInited()) return FALSE;

		while(1)
		{
			if(fp.IsEOF()) break;
			fp.GetLine(szLine, sizeof(szLine));
			if (strstr(szLine, "//") ||
				IsEmptyLine(szLine))
			{
				continue;			
			}
			else if (strstr(szLine, "END_KIND")) 
			{
				nCnt = 0;
				nKind++;
				continue;
			}

			sscanf(szLine, "%d %d %f %f %d",	&m_pstChallengeZoneMonsterGroupSection[nSection].pstGroup[nKind].pstMonster[nCnt].nMonsterKind,
												&m_pstChallengeZoneMonsterGroupSection[nSection].pstGroup[nKind].pstMonster[nCnt].nMonsterNum,
												&m_pstChallengeZoneMonsterGroupSection[nSection].pstGroup[nKind].pstMonster[nCnt].nPosX,
												&m_pstChallengeZoneMonsterGroupSection[nSection].pstGroup[nKind].pstMonster[nCnt].nPosZ,
												&m_pstChallengeZoneMonsterGroupSection[nSection].pstGroup[nKind].pstMonster[nCnt].nDelayTime
												);
			// �� ���� �� ����
			m_pstChallengeZoneMonsterGroupSection[nSection].nTotalMonsterNum += m_pstChallengeZoneMonsterGroupSection[nSection].pstGroup[nKind].pstMonster[nCnt].nMonsterNum;
			// ó�� �����ϴ� ������ ������ Ÿ���� ç���� ���� ������ Ÿ������ ����
			if (nKind == 0)
			{
				m_pstChallengeZoneMonsterGroupSection[nSection].pstGroup[nKind].pstMonster[nCnt].nDelayTime += CHALLENGE_ZONE_START_DELAY_TIME;
			}
			// ������ Ÿ���� �����ؼ� ����Ѵ�.
			if (nKind > 0)
			{
				m_pstChallengeZoneMonsterGroupSection[nSection].pstGroup[nKind].pstMonster[nCnt].nDelayTime += 
				m_pstChallengeZoneMonsterGroupSection[nSection].pstGroup[nKind-1].pstMonster[0].nDelayTime;
			}
			nCnt++;
		}
		fp.Release();
	}
#endif

	return TRUE;
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  GetDateZoneMoveIndex Method																				  ����Ʈ �� �̵� �ε��� ���
//
BOOL CSHDateManager::GetDateZoneMoveIndex(UINT nSelIndex, int* pnIndex)
{
	if (nSelIndex >= m_nDateZoneMoveIndexNum)
	{
		return FALSE;
	}

	*pnIndex = m_pnDateZoneMoveIndexList[nSelIndex];

	return TRUE;
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  GetChallengeZoneMoveIndex Method																		  ç���� �� �̵� �ε��� ���
//
BOOL CSHDateManager::GetChallengeZoneMoveIndex(UINT nSelIndex, int* pnIndex)
{
	if (nSelIndex >= m_nChallengeZoneMoveIndexNum)
	{
		return FALSE;
	}

	*pnIndex = m_pstChallengeZoneMoveIndexList[nSelIndex].nMoveIndex;

	return TRUE;
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  IsChallengeZone Method																								ç���� �� �ΰ�?
//
BOOL CSHDateManager::IsChallengeZone(int nMapNum)
{
	for(UINT i=0; i<m_nChallengeZoneMoveIndexNum; i++)
	{
		if (m_pstChallengeZoneMoveIndexList[i].nMapNum == nMapNum)
		{
			return TRUE;
		}
	}

	return FALSE;
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  Process Method																									 
//
VOID CSHDateManager::MainLoop()
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
  	switch(m_nChallengeZoneState)
	{
	case CHALLENGEZONE_START:
		if (gCurTime - m_nChallengeZoneTimeTick > 1000)
		{
			// ó�� �� ��
			if (m_nChallengeZoneTime == 0)
			{
				// ����Ʈ ����
				if (CHALLENGE_ZONE_MOTION_NUM_START)
					HEFFPROC nEff = EFFECTMGR->StartEffectProcess(CHALLENGE_ZONE_MOTION_NUM_START, HERO, NULL, 0, 0);
			}

			m_nChallengeZoneTime += (gCurTime - m_nChallengeZoneTimeTick)/1000;
			if (m_nChallengeZoneTime >= CHALLENGE_ZONE_START_DELAY_TIME/1000)
			{
				m_nChallengeZoneState = NULL;
				m_nChallengeZoneTime = 0;
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1218));
				GAMEIN->GetChallengeZoneClearNo1Dlg()->SetChallengeZoneStartTimeTick(gCurTime);
			}
			else
			{
				m_nChallengeZoneTimeTick = gCurTime;
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1219), CHALLENGE_ZONE_START_DELAY_TIME/1000 - m_nChallengeZoneTime);
			}
		}
		break;
	case CHALLENGEZONE_END:
		if (gCurTime - m_nChallengeZoneTimeTick > 1000)
		{
			m_nChallengeZoneTime += (gCurTime - m_nChallengeZoneTimeTick)/1000;
			if (m_nChallengeZoneTime >= CHALLENGE_ZONE_END_DELAY_TIME/1000)
			{
				m_nChallengeZoneState = NULL;
				m_nChallengeZoneTime = 0;
			}
			else
			{
				m_nChallengeZoneTimeTick = gCurTime;
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1220), CHALLENGE_ZONE_END_DELAY_TIME/1000 - m_nChallengeZoneTime);
			}
		}
		break;
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_Process Method																							 ç���� �� ���� ���μ���					 
//
VOID CSHDateManager::SRV_Process(CPlayer* pPlayer)
{
#if defined(_MAPSERVER_)
	SRV_ProcChallengeZoneEnterFreq(pPlayer);
	SRV_ProcRegenMonster(pPlayer);
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_StartChallengeZone Method																						  ç���� �� ����					 
//
VOID CSHDateManager::SRV_StartChallengeZone(CPlayer* pPlayer, UINT nChannelNumFromAgent, UINT nSection)
{
#if defined(_MAPSERVER_)
	// ç���� ���� �����̶�� ȸ�� ����
	// ..�������� ç���� �������� �̵��� ��� �ĺ��� ���� ������Ʈ���� 1000 �̻����� ����
	if (nChannelNumFromAgent >= 1000)
	{
		pPlayer->IncreaseChallengeZoneEnterFreq();
		g_csDateManager.SRV_SendChallengeZoneEnterFreq(pPlayer);
		// ���� ���⼭ DB�� �����ؾ� ������ �� ��ƾ ���� �Ŀ� DB Load ��ƾ�� ����ȴ�.
		// �׷��� Load �κп��� ���� ȸ���� �����ƴ����� üũ�ؼ� �����Ǿ��ٸ� �� ���� DB�� �����Ѵ�.
	}

	if (m_bChallengeZoneStart == FALSE) return;

	// ���� ���� �ִ� ��Ʈ�� üũ
	// ..��Ʈ�ʰ� ���ų� ä���� �ٸ��ٸ� ���� �̻�..
	CPlayer* pFirstEnterPlayer = (CPlayer*)g_pUserTable->FindUser(m_nChallengeZoneFirstEnterPlayerID);
	if(pFirstEnterPlayer && pPlayer->GetChannelID() == pFirstEnterPlayer->GetChannelID())
	{
		// ��Ʈ�� ID ����
		pPlayer->SetChallengeZonePartnerID(pFirstEnterPlayer->GetID());
		pFirstEnterPlayer->SetChallengeZonePartnerID(pPlayer->GetID());
		// ���� �� ����
		pPlayer->SetChallengeZoneMonsterNum(m_pstChallengeZoneMonsterGroupSection[nSection].nTotalMonsterNum);
		pPlayer->SetChallengeZoneMonsterNumTillNow(0);
		pPlayer->SetChallengeZoneKillMonsterNum(0);
		pPlayer->SetChallengeZoneCreateMonRightNow(FALSE);
		pFirstEnterPlayer->SetChallengeZoneMonsterNum(m_pstChallengeZoneMonsterGroupSection[nSection].nTotalMonsterNum);
		pFirstEnterPlayer->SetChallengeZoneMonsterNumTillNow(0);
		pFirstEnterPlayer->SetChallengeZoneKillMonsterNum(0);
		pFirstEnterPlayer->SetChallengeZoneCreateMonRightNow(FALSE);
		// ..���� ���� üũ. ���� ������ ������ ����ٸ� �׳� �� ����..��
		if (nSection < ENTER_CHALLENGE_ZONE_SECTION_NUM)
		{
			// ç���� ���� ���� ����
			// ���� �� ���� ������ 1���� ����Ѵ�.
			pPlayer->SetChallengeZoneStartState(1);
			// ���� ����
			pPlayer->SetChallengeZoneSection(nSection);
			pFirstEnterPlayer->SetChallengeZoneSection(nSection);
			// ƽ ī��Ʈ ����
			pPlayer->SetChallengeZoneStartTimeTick(gCurTime);
			pFirstEnterPlayer->SetChallengeZoneStartTimeTick(gCurTime);

			// ç���� ���� �� ������ ç���� ���� �ִ� Ŭ���� ������ �ε�/�����Ѵ�.
			ChallengeZone_ClearNo1_Load(pPlayer->GetID(), nSection);
			ChallengeZone_ClearNo1_Load(pFirstEnterPlayer->GetID(), nSection);
		}
	}
	else
	{
/*		if (g_pServerSystem->IsTestServer())
		{
			g_Console.LOG(4, "CHALLENGEZONE_ENTER_ERROR:%d", pPlayer->GetID());
		}
*/	}

	m_bChallengeZoneStart = FALSE;
	m_nChallengeZoneFirstEnterPlayerID = 0;
#endif
}
// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_NotifyStartChallengeZone Method																				 ç���� �� ���� �˸�					 
//
VOID CSHDateManager::SRV_NotifyStartChallengeZone(CPlayer* pPlayer, char* pszName1, char* pszName2, DWORD nLeastClearTime, UINT nSection)
{
#if defined(_MAPSERVER_)
	MSG_NAME2_DWORD2 stPacket;

	stPacket.Category	= MP_DATE;
	stPacket.Protocol	= MP_DATE_CHALLENGEZONE_START;

	if (nLeastClearTime)
	{
		SafeStrCpy(stPacket.Name1, pszName1, MAX_NAME_LENGTH+1);
		SafeStrCpy(stPacket.Name2, pszName2, MAX_NAME_LENGTH+1);
		stPacket.dwData1 = nSection;
		stPacket.dwData2 = nLeastClearTime;
	}
	else
	{
		ZeroMemory(stPacket.Name1, sizeof(stPacket.Name1));
		ZeroMemory(stPacket.Name2, sizeof(stPacket.Name2));
		stPacket.dwData1 = nSection;
		stPacket.dwData2 = 0;
	}

	pPlayer->SendMsg(&stPacket, sizeof(stPacket));

	// �ִ� �ð� Ŭ���� Ÿ���� �����Ѵ�. �ִ� �ð� ���� �� ���
	m_pstChallengeZoneSectionMonLevel[nSection].nLeastClearTime = nLeastClearTime;
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_ProcChallengeEnterFreq Method																			ç���� �� ���� ȸ�� ó��					 
//
VOID CSHDateManager::SRV_ProcChallengeZoneEnterFreq(CPlayer* pPlayer)
{
#if defined(_MAPSERVER_)
	// ��(day)�� ����Ǿ�����(������ ������ ���� ȸ�� �ʱ�ȭ)
	if (MHTIMEMGR_OBJ->GetOldLocalTime()->wDay != MHTIMEMGR_OBJ->GetCurLocalTime()->wDay)
	{
		if (pPlayer->GetChallengeZoneEnterFreq())
		{
			pPlayer->SetChallengeZoneEnterFreq(0);
			SRV_SendChallengeZoneEnterFreq(pPlayer);
			// ���⼭ DB������ �ʿ�������, �����ϸ� �� ������ ������ �� ��ŭ�� DB������ �߻��Ѵ�. ���� ���� �߻�������....
			// �����ϴ� �� Ȯ���ϱ� ������ ������ �� �ص� �ȴ�. �����󿡼��� ���� 0 ���� �� �ָ� �ȴ�.
			// ç���� ������ �̵��ϰų�, �α׾ƿ�, �� �̵��� �� �� DB Load �� �ϸ鼭 ��¥ üũ�� �ؼ� 
			// ��¥�� �����ٸ� DB�� ����Ǿ� �ִ� ���� ������� 0 ���� �����ϱ� ����.
			// �׸���, ç���� ������ �̵��� �� 0 ���� ���ϵ� ���� ç���� ������ ���鼭 +1 �Ǿ�
			// DB�� ������ �ϹǷ� DB ������ ���⼭ �Ű� �� �ᵵ ��. (��, ç���� ������ �̵��ϴ� �������� DB ������ ��)
			// ..��¥ üũ�� �̷��� ���� �ʰ�, �йи� Ż��ó�� ç���� �� �̵��� �õ��� ��, �� �� ��¥��
			// ..üũ�ؼ� ó���ص� �Ǵµ�, �׷� ��� Ŭ���̾�Ʈ�� ���ŵ� ������ �����ִ� �� �Ұ���������, ����
			// ..�������� ç���� ���� �� ������ ��¥�� ���� �־�� �ϴµ�, �������� ���� ���� �ʰ� ó���� �� �ִ�
			// ..���(DB ���ν������� ó���� �� �ִ� ���)�� �켱���ؼ� �̷��� ó������.
			//
			// �׷��� ������ ���� �� �ִ�. ���Ӽ����� �ý���Ÿ���� DB������ �ý���Ÿ�Ӻ��� ���� ���, ���Ӽ���������
			// ȸ���� 0���� �ʱ�ȭ �� ��, �� �̵��� �ϰ� �Ǿ��� ��, �� �� DB������ �ý���Ÿ���� ������ ������ �ʾҴٸ�,
			// DB�� ����Ǿ� �ִ� ���� �о����, DB������ �ý���Ÿ���� ������ ���� �� ���̵��� �ϱ� �������� 
			// ç���� ���� ������ �� ���� �ȴ�. ���Ӽ����� �ý���Ÿ�Ӱ� DB������ �ý���Ÿ���� ������ ����~
			// �� ������ �����ϱ� ���� ���̺� �÷��׸� �ϳ� �ּ� �÷��װ� ���õǾ� ������ �� ��Ż�ÿ� DB������ �ϵ��� �Ѵ�.
			pPlayer->SetChallengeZoneNeedSaveEnterFreq(TRUE);
			// �׷��� ���� �� ��Ż�� ���� ���� DB�������� ���� ���� �ε� DB������ ���� ����ȴٸ�..???  ��� ����..
			// DB������ �ý���Ÿ���� ������� �ʰ� ���Ӽ����� �ý���Ÿ���� ����Ѵٰ� �ص�, ��� �ʼ����� �� �ϵ���
			// ž��Ǿ� ���� ���� �̻� �ð������� ���� ���� ����. ���� Ȯ���� �� �پ�������..
		}
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_SendChallengeZoneEnterFreq Method																		ç���� �� ���� ȸ�� ����					 
//
VOID CSHDateManager::SRV_SendChallengeZoneEnterFreq(CPlayer* pPlayer)
{
#if defined(_MAPSERVER_)
	MSG_DWORD2 msg;

	msg.Category	= MP_DATE;
	msg.Protocol	= MP_DATE_CHALLENGEZONE_ENTER_FREQ;
	msg.dwData1		= pPlayer->GetChallengeZoneEnterFreq();
	msg.dwData2		= pPlayer->GetChallengeZoneEnterBonusFreq();
	pPlayer->SendMsg(&msg, sizeof(msg));
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_ProcRegenMonster Method																				  ç���� �� ���� ���� ó��					 
//
VOID CSHDateManager::SRV_ProcRegenMonster(CPlayer* pPlayer)
{
#if defined(_MAPSERVER_)
	// ç���� ���°� �ƴϸ�..
	if (pPlayer->GetChallengeZoneStartState() == 0)
	{
		return;
	}

	// ç������ �������� ��� �ð� �� ��ȯ�ϱ� ���� ��� ������.. Ŭ���̾�Ʈ���� ������ ���͸� ��� ������ ������ �ð��� ����..
	// ���⿡���� �����̰� ������ ���� ī��Ʈ �ٿ� ��.
	if (pPlayer->GetChallengeZoneStartState() == CHALLENGEZONE_END_SUCCESS)
	{
		if (pPlayer->GetChallengeZoneStartTimeTick())
		{
			const DWORD END_DELAY = 5000;
			if (gCurTime - pPlayer->GetChallengeZoneStartTimeTick() > END_DELAY)
			{
				pPlayer->SetChallengeZoneStartState(CHALLENGEZONE_END_START_COUNTDOWN);
				pPlayer->SetChallengeZoneStartTimeTick(gCurTime);

				MSG_DWORD stPacket;
				stPacket.Category			= MP_DATE;
				stPacket.Protocol			= MP_DATE_CHALLENGEZONE_END_SUCCESS;
				stPacket.dwObjectID			= pPlayer->GetID();
				stPacket.dwData				= 0;
				// �ִ� �ð� Ŭ���� Ÿ���� �����ߴٸ� Ŭ���̾�Ʈ ������ ���� �� ����
				if (pPlayer->GetChallengeZoneClearTime() &&
					pPlayer->GetChallengeZoneClearTime() < m_pstChallengeZoneSectionMonLevel[pPlayer->GetChallengeZoneSection()].nLeastClearTime)
				{
					stPacket.dwData			= 1;
				}

				pPlayer->SendMsg(&stPacket, sizeof(stPacket));	
			}
		}
		return;
	}

	// ç������ �������� ��� �ð� �� ��ȯ
	if (pPlayer->GetChallengeZoneStartState() > CHALLENGEZONE_END)
	{
		if (pPlayer->GetChallengeZoneStartTimeTick())
		{
			if (gCurTime - pPlayer->GetChallengeZoneStartTimeTick() > CHALLENGE_ZONE_END_DELAY_TIME)
			{
				pPlayer->SetChallengeZoneStartTimeTick(0);

				MSG_DWORD stPacket;

				stPacket.Category 		= MP_DATE;
				stPacket.Protocol 		= MP_DATE_CHALLENGEZONE_RETURN;
				stPacket.dwObjectID		= pPlayer->GetID();;
				pPlayer->SendMsg(&stPacket, sizeof(stPacket) );
			}
		}
		return;
	}

	UINT nSection	= pPlayer->GetChallengeZoneSection();
	UINT i			= pPlayer->GetChallengeZoneStartState()-1;
	for(; i<m_pstChallengeZoneMonsterGroupSection[nSection].nGroupNum; i++)
	{
		// ���� �ð��� �Ƴ�? �Ǵ� ���ݱ��� ���� ���͸� ��� ��ġ������ �ٷ� ���� ���� ����
		if (gCurTime - pPlayer->GetChallengeZoneStartTimeTick() > m_pstChallengeZoneMonsterGroupSection[nSection].pstGroup[i].pstMonster[0].nDelayTime ||
			pPlayer->IsChallengeZoneCreateMonRightNow())
		{
			// ���� ��ȯ
			for(UINT j=0; j<m_pstChallengeZoneMonsterGroupSection[nSection].pstGroup[i].nMonsterNum; j++)
			{
				VECTOR3 stPos;
				stPos.x = GetChallengeZoneMonsterGroup(nSection, i)->pstMonster[j].nPosX;
				stPos.z = GetChallengeZoneMonsterGroup(nSection, i)->pstMonster[j].nPosZ;
				RECALLMGR->ChallengeZoneRecall(g_csDateManager.GetChallengeZoneMonsterGroup(nSection, i)->pstMonster[j].nMonsterKind, 
											   g_csDateManager.GetChallengeZoneMonsterGroup(nSection, i)->pstMonster[j].nMonsterNum, 
											   pPlayer->GetChannelID(), 
											   &stPos,
											   0);
				/*
				if (g_pServerSystem->IsTestServer())
				{
					g_Console.LOG(4, "CHALLENGEZONE Monster: Channel:%d Group:%d Mon:%d,%d)", 
								  pPlayer->GetChannelID(),
								  i, 
								  g_csDateManager.GetChallengeZoneMonsterGroup(nSection, i)->pstMonster[j].nMonsterKind, 
								  g_csDateManager.GetChallengeZoneMonsterGroup(nSection, i)->pstMonster[j].nMonsterNum);
				}
				*/

				// ���ݱ��� ���� ���� �� ����
				UINT nNum = pPlayer->GetChallengeZoneMonsterNumTillNow() + g_csDateManager.GetChallengeZoneMonsterGroup(nSection, i)->pstMonster[j].nMonsterNum;
				pPlayer->SetChallengeZoneMonsterNumTillNow(nNum);
			}
			pPlayer->SetChallengeZoneStartState(pPlayer->GetChallengeZoneStartState()+1);
			pPlayer->SetChallengeZoneCreateMonRightNow(FALSE);
			break;
		}
	}

	// Ÿ�Ӿƿ�
	if (gCurTime - pPlayer->GetChallengeZoneStartTimeTick() > m_pstChallengeZoneSectionMonLevel[pPlayer->GetChallengeZoneSection()].nLimitTime)
	{
		SRV_EndChallengeZone(pPlayer, CHALLENGEZONE_END_TIMEOUT);
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_ProcMonsterDie Method																				  ç���� �� ���� ���� ó��					 
//
VOID CSHDateManager::SRV_ProcMonsterDie(CPlayer* pPlayer)
{
#if defined(_MAPSERVER_)
	pPlayer->IncreaseChallengeZoneKillMonsterNum();
	CPlayer* pPartner = (CPlayer*)g_pUserTable->FindUser(pPlayer->GetChallengeZonePartnerID());
	if (pPartner)
	{
		UINT nKillMonNum = pPlayer->GetChallengeZoneKillMonsterNum() + pPartner->GetChallengeZoneKillMonsterNum();
		// ���͸� ��� ��ġ������ ����!!
		if (nKillMonNum >= pPlayer->GetChallengeZoneMonsterNum())
		{
			SRV_EndChallengeZone(pPlayer,  CHALLENGEZONE_END_SUCCESS);
		}
		else
		{
			// ���ݱ��� ���� ���͸� ��� ��ġ������ �ٷ� ���� ���� ���� ����
			// ..���� ������ ����ϴ� �÷��̾ üũ�ؼ� ó��
			if (pPlayer->GetChallengeZoneStartState())
			{
				if (nKillMonNum  >= pPlayer->GetChallengeZoneMonsterNumTillNow())
				{
					pPlayer->SetChallengeZoneCreateMonRightNow(TRUE);
				}
			}
			else
			{
				if (nKillMonNum  >= pPartner->GetChallengeZoneMonsterNumTillNow())
				{
					pPartner->SetChallengeZoneCreateMonRightNow(TRUE);
				}
			}
		}
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_SetChallengeZoneEnterBonusFreq Method															 ç���� �� ���� ���ʽ� ȸ�� ����				 
//
VOID CSHDateManager::SRV_SetChallengeZoneEnterBonusFreq(CPlayer* pPlayer, UINT nFreq)
{
#if defined(_MAPSERVER_)
	pPlayer->SetChallengeZoneEnterBonusFreq(pPlayer->GetChallengeZoneEnterBonusFreq()+nFreq);
	// DB�� ����
	ChallengeZone_EnterFreq_Save(pPlayer->GetID(), pPlayer->GetChallengeZoneEnterFreq(), pPlayer->GetChallengeZoneEnterBonusFreq());

	SRV_SendChallengeZoneEnterFreq(pPlayer);
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_EndChallengeZone Method																						    ç���� ���� ����					 
//
VOID CSHDateManager::SRV_EndChallengeZone(CPlayer* pPlayer, CHALLENGEZONE_STATE eEndState)
{
#if defined(_MAPSERVER_)
	if (IsChallengeZoneHere() == FALSE) return;
	
	switch(eEndState)
	{
	// Ÿ�Ӿƿ�
	case CHALLENGEZONE_END_TIMEOUT:
		if (pPlayer->GetChallengeZoneStartState() < CHALLENGEZONE_END &&
			pPlayer->GetChallengeZonePartnerID())
		{
			CPlayer* pPartner = (CPlayer*)g_pUserTable->FindUser(pPlayer->GetChallengeZonePartnerID());
			if (pPartner)
			{
				// Ÿ�Ӿƿ��̸� ç���� �� ��.
				pPlayer->SetChallengeZoneStartState(eEndState);
				pPlayer->SetChallengeZoneStartTimeTick(gCurTime);

				pPartner->SetChallengeZoneStartState(eEndState);
				pPartner->SetChallengeZoneStartTimeTick(gCurTime);

				MSG_DWORD stPacket;
				stPacket.Category			= MP_DATE;
				stPacket.Protocol			= MP_DATE_CHALLENGEZONE_END_TIMEOUT;
				stPacket.dwObjectID			= pPlayer->GetID();
				stPacket.dwData				= CHALLENGE_ZONE_END_DELAY_TIME;
				pPlayer->SendMsg(&stPacket, sizeof(stPacket));	

				stPacket.Category			= MP_DATE;
				stPacket.Protocol			= MP_DATE_CHALLENGEZONE_END_TIMEOUT;
				stPacket.dwObjectID			= pPartner->GetID();
				stPacket.dwData				= CHALLENGE_ZONE_END_DELAY_TIME;
				pPartner->SendMsg(&stPacket, sizeof(stPacket));	
			}
			else
			{
				// Ÿ�Ӿƿ��ε�, ��Ʈ�ʰ� ����? 
				// ��Ʈ�ʰ� �����鼭 ���ÿ� Ÿ�Ӿƿ��� ��� ��Ʈ�ʰ� �����鼭 ���� ����.
				// ȥ�� �����ִ� �� Ÿ�Ӿƿ�? ������������ ��Ȳ. ��·�ų� �ѾƳ���
				pPlayer->SetChallengeZoneStartState(eEndState);
				pPlayer->SetChallengeZoneStartTimeTick(gCurTime);

				MSG_DWORD stPacket;
				stPacket.Category			= MP_DATE;
				stPacket.Protocol			= MP_DATE_CHALLENGEZONE_END_TIMEOUT;
				stPacket.dwObjectID			= pPlayer->GetID();
				stPacket.dwData				= CHALLENGE_ZONE_END_DELAY_TIME;
				pPlayer->SendMsg(&stPacket, sizeof(stPacket));	
			}
		}
		break;
	// pPlayer �� ç���� ���� ��Ż�ϸ� pPartner ���� ���и� �˸���.
	case CHALLENGEZONE_END_PARTNER_OUT:
		if (pPlayer->GetChallengeZoneStartState() < CHALLENGEZONE_END &&
			pPlayer->GetChallengeZonePartnerID())
		{
			CPlayer* pPartner = (CPlayer*)g_pUserTable->FindUser(pPlayer->GetChallengeZonePartnerID());
			if (pPartner)
			{
				pPartner->SetChallengeZoneStartState(eEndState);
				pPartner->SetChallengeZoneStartTimeTick(gCurTime);

				MSG_DWORD stPacket;
				stPacket.Category			= MP_DATE;
				stPacket.Protocol			= MP_DATE_CHALLENGEZONE_END_PARTNER_OUT;
				stPacket.dwObjectID			= pPlayer->GetID();
				stPacket.dwData				= CHALLENGE_ZONE_END_DELAY_TIME;
				pPartner->SendMsg(&stPacket, sizeof(stPacket));	
			}
		}
		break;
	// pPlayer �� �׾��� ��, pPartner �� �׾��ִٸ� ��ο��� ���и��˸���.
	case CHALLENGEZONE_END_ALL_DIE:
		if (pPlayer->GetChallengeZoneStartState() < CHALLENGEZONE_END &&
			pPlayer->GetChallengeZonePartnerID())
		{
			CPlayer* pPartner = (CPlayer*)g_pUserTable->FindUser(pPlayer->GetChallengeZonePartnerID());
			if (pPartner)
			{
				// �� �� �׾����� ç���� �� ��.
				if (pPartner->GetState() == eObjectState_Die)
				{
					pPlayer->SetChallengeZoneStartState(eEndState);
					pPlayer->SetChallengeZoneStartTimeTick(gCurTime);

					pPartner->SetChallengeZoneStartState(eEndState);
					pPartner->SetChallengeZoneStartTimeTick(gCurTime);

					MSG_DWORD stPacket;
					stPacket.Category			= MP_DATE;
					stPacket.Protocol			= MP_DATE_CHALLENGEZONE_END_ALL_DIE;
					stPacket.dwObjectID			= pPlayer->GetID();
					stPacket.dwData				= CHALLENGE_ZONE_END_DELAY_TIME;
					pPlayer->SendMsg(&stPacket, sizeof(stPacket));	

					stPacket.Category			= MP_DATE;
					stPacket.Protocol			= MP_DATE_CHALLENGEZONE_END_ALL_DIE;
					stPacket.dwObjectID			= pPartner->GetID();
					stPacket.dwData				= CHALLENGE_ZONE_END_DELAY_TIME;
					pPartner->SendMsg(&stPacket, sizeof(stPacket));	
				}
			}
			else
			{
				// �׾��� ��, ��Ʈ�ʰ� ����? 
				// ��Ʈ�ʰ� �����鼭 ���ÿ� �׾��� ��� ��Ʈ�ʰ� �����鼭 ���� ����.
				// ȥ�� �����ִ� �� �׾���? ������������ ��Ȳ. ��·�ų� �ѾƳ���
				pPlayer->SetChallengeZoneStartState(eEndState);
				pPlayer->SetChallengeZoneStartTimeTick(gCurTime);

				MSG_DWORD stPacket;
				stPacket.Category			= MP_DATE;
				stPacket.Protocol			= MP_DATE_CHALLENGEZONE_END_ALL_DIE;
				stPacket.dwObjectID			= pPlayer->GetID();
				stPacket.dwData				= CHALLENGE_ZONE_END_DELAY_TIME;
				pPlayer->SendMsg(&stPacket, sizeof(stPacket));	
			}
		}
		break;
	// ���͸� ��� ��Ƽ� ����!
	case CHALLENGEZONE_END_SUCCESS:
		{
			if (pPlayer->GetChallengeZoneStartState() < CHALLENGEZONE_END)
			{
				CPlayer* pPartner = (CPlayer*)g_pUserTable->FindUser(pPlayer->GetChallengeZonePartnerID());
				if (pPartner)
				{
					UINT nClearTime = gCurTime - pPlayer->GetChallengeZoneStartTimeTick() - CHALLENGE_ZONE_START_DELAY_TIME;

					pPlayer->SetChallengeZoneStartState(eEndState);
					pPlayer->SetChallengeZoneStartTimeTick(gCurTime);

					pPartner->SetChallengeZoneStartState(eEndState);
					pPartner->SetChallengeZoneStartTimeTick(gCurTime);

					// Ŭ���� Ÿ�� ����
					pPlayer->SetChallengeZoneClearTime(nClearTime);
					pPartner->SetChallengeZoneClearTime(nClearTime);
					// DB ����
					ChallengeZone_Success_Save(pPlayer->GetID(), pPartner->GetID(), pPlayer->GetChallengeZoneSection(), nClearTime);
					ChallengeZone_Success_Save(pPartner->GetID(), pPlayer->GetID(), pPartner->GetChallengeZoneSection(), nClearTime);
				}
			}
		}
		break;
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_EnterDateZone Method																							  ����Ʈ �� ����
//
VOID CSHDateManager::SRV_EnterDateZone(CPlayer* pPlayer, DWORD nTargetPlayerID, int nZoneIndex)
{
#if defined(_MAPSERVER_)
	DWORD PartyIdx = pPlayer->GetPartyIdx();
	CParty* pParty = PARTYMGR->GetParty(PartyIdx);
	MSG_DWORD stPacket;

	// ��Ƽ�� �ֳ�?
	if(pParty == NULL)
	{
		return;
	}

	// �������ΰ�?
	if(pPlayer->GetID() != pParty->GetMasterID())
	{
		return;
	}
	
	// ��Ƽ���� ����Ʈ ��Ī ����� 2���ΰ�?
	int nNum = 0;
	for(int i=0; i<MAX_PARTY_LISTNUM; i++)
	{
		if (pParty->GetMemberID(i))
		{
			nNum++;
		}
	}
	if(pParty->IsPartyMember(nTargetPlayerID) == FALSE || nNum != 2)
	{
		return;
	}

	stPacket.Category			= MP_DATE;
	stPacket.Protocol			= MP_DATE_ENTER_DATEZONE_2;
	stPacket.dwObjectID			= pPlayer->GetID();
	stPacket.dwData				= nZoneIndex;

	pPlayer->SendMsg(&stPacket, sizeof(stPacket));	
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_EnterDateZone2 Method																							  ����Ʈ �� ����
//
VOID CSHDateManager::SRV_EnterDateZone2(CPlayer* pPlayer, MSG_DWORD4* pPacket)
{
#if defined(_MAPSERVER_)
	DWORD PartyIdx = pPlayer->GetPartyIdx();
	CParty* pParty = PARTYMGR->GetParty(PartyIdx);
	MSG_DWORD4 stPacket;

	// ��Ƽ�� �ֳ�?
	if(pParty == NULL)
	{
		return;
	}

	// �������ΰ�?
	if(pPlayer->GetID() != pParty->GetMasterID())
	{
		return;
	}
	
	// ��Ƽ���� ����Ʈ ��Ī ����� 2���ΰ�?
	int nNum = 0;
	for(int i=0; i<MAX_PARTY_LISTNUM; i++)
	{
		if (pParty->GetMemberID(i))
		{
			nNum++;
		}
	}
	if(pParty->IsPartyMember(pPacket->dwData2) == FALSE || nNum != 2)
	{
		return;
	}

	stPacket.Category			= MP_DATE;
	stPacket.Protocol			= MP_DATE_ENTER_DATEZONE_2_DIFF_AGENT_CHECK_PARTY_FOR_OTHER_AGENT_2;
	stPacket.dwObjectID			= pPlayer->GetID();
	stPacket.dwData1			= pPacket->dwData1;
	stPacket.dwData2			= pPacket->dwData2;
	stPacket.dwData3			= pPacket->dwData3;
	stPacket.dwData4			= pPacket->dwData4;

	pPlayer->SendMsg(&stPacket, sizeof(stPacket));	
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_EnterChallengeZone Method																						  ç���� �� ����
//
VOID CSHDateManager::SRV_EnterChallengeZone(CPlayer* pPlayer, DWORD nTargetPlayerID, int nZoneIndex, int nKind)
{
#if defined(_MAPSERVER_)
	RESULT eResult = RESULT_OK;
	DWORD PartyIdx = pPlayer->GetPartyIdx();
	CParty* pParty = PARTYMGR->GetParty(PartyIdx);
	MSG_DWORD2 stPacket;
	
	// ��Ƽ���� ��� ��Ƽ ���� üũ
	if (nKind == 0)
	{
		// ��Ƽ�� �ֳ�?
		if(pParty == NULL)
		{
			return;
		}

		// �������ΰ�?
		if(pPlayer->GetID() != pParty->GetMasterID())
		{
			return;
		}
		
		// ��Ƽ���� ����Ʈ ��Ī ����� 2���ΰ�?
		int nNum = 0;
		for(int i=0; i<MAX_PARTY_LISTNUM; i++)
		{
			if (pParty->GetMemberID(i))
			{
				nNum++;
			}
		}
		if (pParty->IsPartyMember(nTargetPlayerID) == FALSE || nNum != 2)
		{
			return;
		}
	}

	// �Ϸ� ���� ���� ȸ���� ���� �ʾҳ�?
	if (pPlayer->GetChallengeZoneEnterFreq() >= ENTER_CHALLENGE_ZONE_FREQ_PER_DAY+pPlayer->GetChallengeZoneEnterBonusFreq())
	{
		eResult = RESULT_FAIL_02;
	}

	// ç���� �������� �Ұ���
	if (IsChallengeZoneHere())
	{
		eResult = RESULT_FAIL_03;
	}

	// ���� ���� ���� üũ
	if (pPlayer->GetLevel() < m_pstChallengeZoneSectionMonLevel[nZoneIndex].nStart)
	{
		eResult = RESULT_FAIL_04;
	}

	// PK ��� ���� �Ұ���
	if (pPlayer->IsPKMode())
	{
		eResult = RESULT_FAIL_05;
	}

//---KES AUTONOTE
	if( pPlayer->GetAutoNoteIdx() )
	{
		eResult = RESULT_FAIL_06;
	}
//--------------

	stPacket.Category			= MP_DATE;
	stPacket.Protocol			= MP_DATE_ENTER_CHALLENGEZONE_2;
	if (nKind != 0)
	{
		stPacket.Protocol		= MP_DATE_ENTER_CHALLENGEZONE_3;
	}
	stPacket.dwObjectID			= pPlayer->GetID();
	stPacket.dwData1			= nZoneIndex;
	stPacket.dwData2			= eResult;

	pPlayer->SendMsg(&stPacket, sizeof(stPacket));	
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_EnterChallengeZone Method																						  ç���� �� ����
//
VOID CSHDateManager::SRV_EnterChallengeZone2(CPlayer* pPlayer, MSG_DWORD4* pPacket, int nKind)
{
#if defined(_MAPSERVER_)
	RESULT eResult = RESULT_OK;
	DWORD PartyIdx = pPlayer->GetPartyIdx();
	CParty* pParty = PARTYMGR->GetParty(PartyIdx);
	MSG_DWORD5 stPacket;

	// ��Ƽ���� ��� ��Ƽ ���� üũ
	if (nKind == 0)
	{
		// ��Ƽ�� �ֳ�?
		if(pParty == NULL)
		{
			return;
		}

		// �������ΰ�?
		if(pPlayer->GetID() != pParty->GetMasterID())
		{
			return;
		}
		
		// ��Ƽ���� ����Ʈ ��Ī ����� 2���ΰ�?
		int nNum = 0;
		for(int i=0; i<MAX_PARTY_LISTNUM; i++)
		{
			if (pParty->GetMemberID(i))
			{
				nNum++;
			}
		}
		if(pParty->IsPartyMember(pPacket->dwData2) == FALSE || nNum != 2)
		{
			return;
		}
	}

	// �Ϸ� ���� ���� ȸ���� ���� �ʾҳ�?
	if (pPlayer->GetChallengeZoneEnterFreq() >= ENTER_CHALLENGE_ZONE_FREQ_PER_DAY+pPlayer->GetChallengeZoneEnterBonusFreq())
	{
		eResult = RESULT_FAIL_02;
	}

	// ç���� �������� �Ұ���
	if (IsChallengeZoneHere())
	{
		eResult = RESULT_FAIL_03;
	}

	// ���� ���� ���� üũ
	if (pPlayer->GetLevel() < m_pstChallengeZoneSectionMonLevel[pPacket->dwData4].nStart)
	{
		eResult = RESULT_FAIL_04;
	}

	// PK ��� ���� �Ұ���
	if (pPlayer->IsPKMode())
	{
		eResult = RESULT_FAIL_05;
	}

//---KES AUTONOTE
	if( pPlayer->GetAutoNoteIdx() )
	{
		eResult = RESULT_FAIL_06;	//=_=6���� �ֱ�.. �ϴ� �������
	}
//--------------

	stPacket.Category			= MP_DATE;
	stPacket.Protocol			= MP_DATE_ENTER_CHALLENGEZONE_2_DIFF_AGENT_CHECK_PARTY_FOR_OTHER_AGENT_2;
	if (nKind != 0)
	{
		stPacket.Protocol		= MP_DATE_ENTER_CHALLENGEZONE_2_DIFF_AGENT_CHECK_PARTY_FOR_OTHER_AGENT_4;
	}
	stPacket.dwObjectID			= pPlayer->GetID();
	stPacket.dwData1			= pPacket->dwData1;
	stPacket.dwData2			= pPacket->dwData2;
	stPacket.dwData3			= pPacket->dwData3;
	stPacket.dwData4			= pPacket->dwData4;
	stPacket.dwData5			= eResult;

	pPlayer->SendMsg(&stPacket, sizeof(stPacket));	
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ASRV_ParseRequestFromClient Method																 Ŭ���̾�Ʈ ��û �м�/ó��(������Ʈ)
//
VOID CSHDateManager::ASRV_ParseRequestFromClient(DWORD dwConnectionID, char* pMsg, DWORD dwLength)
{
#if defined(_AGENTSERVER)
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;

	// Ŭ���̾�Ʈ���� ��Ŷ �������� ���� ĳ���� ID �� ���� �� �ֱ� ������
	// g_pUserTableForObjectID->FindUser �� UserInfo �� ���ϸ� �� ��!!
	USERINFO* pUserInfo = g_pUserTable->FindUser( dwConnectionID );
	if( pUserInfo == NULL ) return;
	pTempMsg->dwObjectID = pUserInfo->dwCharacterID;

	switch( pTempMsg->Protocol )
	{
	// ����Ʈ �� ����
	// ç���� �� ����
	case MP_DATE_ENTER_DATEZONE:
	case MP_DATE_ENTER_CHALLENGEZONE:
		{
			MSG_DWORD2* pPacket = (MSG_DWORD2*)pMsg;
			// ��û�ڰ� ä������ �÷��̾��ΰ�?
			if (pUserInfo->DateMatchingInfo.nRequestChatState != DATE_MATCHING_CHAT_REQUEST_STATE_CHATTING)
			{
				return;
			}

			// ��� �÷��̾� ã��
			USERINFO* pTargetUserInfo = g_pUserTableForObjectID->FindUser(pUserInfo->DateMatchingInfo.nChatPlayerID);
			if (pTargetUserInfo == NULL)
			{
				// ä���� ������ ������.
				// ..���̵�, �α׾ƿ� ����� ���..
				pUserInfo->DateMatchingInfo.nRequestChatState			= DATE_MATCHING_CHAT_REQUEST_STATE_NONE;
				pUserInfo->DateMatchingInfo.nChatPlayerID				= NULL;
				// ..ä���� ����Ǿ����� �˸���.
				MSG_DWORD2 stPacket;
				stPacket.Category			= MP_RESIDENTREGIST;
				stPacket.Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
				stPacket.dwObjectID			= pUserInfo->dwCharacterID;
				stPacket.dwData1			= DATE_MATCHING_CHAT_RESULT_END_CHAT;
				stPacket.dwData2			= NULL;										// ���ID
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
				return;
			}

			// ��� �÷��̾��� ��ȭ ��밡 ��û���ΰ�?
			if (pTargetUserInfo->DateMatchingInfo.nChatPlayerID != pUserInfo->dwCharacterID)
			{
				// ä���� ������ ������.
				// ..���̵�, �α׾ƿ� ����� ���..
				pUserInfo->DateMatchingInfo.nRequestChatState			= DATE_MATCHING_CHAT_REQUEST_STATE_NONE;
				pUserInfo->DateMatchingInfo.nChatPlayerID				= NULL;
				// ..ä���� ����Ǿ����� �˸���.
				MSG_DWORD2 stPacket;
				stPacket.Category			= MP_RESIDENTREGIST;
				stPacket.Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
				stPacket.dwObjectID			= pUserInfo->dwCharacterID;
				stPacket.dwData1			= DATE_MATCHING_CHAT_RESULT_END_CHAT;
				stPacket.dwData2			= NULL;										// ���ID
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
				return;
			}

			// ä�� ��� ID �Է�
			pPacket->dwData2 = pUserInfo->DateMatchingInfo.nChatPlayerID;

			// ������ �������� üũ�� ���� ���� Ŭ���̾�Ʈ�� ���� - �������� üũ�� Ŭ���̾�Ʈ������ ��
			g_Network.Send2User( pTargetUserInfo->dwConnectionIndex, pMsg, dwLength );
		}
		break;
	case MP_DATE_ENTER_DATEZONE_DIFF_AGENT:
	case MP_DATE_ENTER_CHALLENGEZONE_DIFF_AGENT:
		{
			MSG_DWORD2* pPacket = (MSG_DWORD2*)pMsg;
			// ��û�ڰ� ä������ �÷��̾��ΰ�?
			if (pUserInfo->DateMatchingInfo.nRequestChatState != DATE_MATCHING_CHAT_REQUEST_STATE_CHATTING)
			{
				return;
			}

			// ��� �÷��̾� ã��
			// ..��� �÷��̾� ������Ʈ�� �˸���
			MSG_DWORD4 stPacket;
			stPacket.Category			= MP_DATE;
			stPacket.Protocol			= pPacket->Protocol;;
			stPacket.dwObjectID			= pPacket->dwData2;
			stPacket.dwData1			= pUserInfo->DateMatchingInfo.nChatPlayerID;
			stPacket.dwData2			= pUserInfo->dwCharacterID;
			stPacket.dwData3			= g_pServerSystem->GetServerNum();
			stPacket.dwData4			= pPacket->dwData1;
			g_Network.Send2SpecificAgentServer((char*)&stPacket, sizeof(stPacket) );
		}
		break;
	// ����Ʈ �� ���� - ���� ���� ���� üũ ��
	// ç���� �� ���� - ���� ���� ���� üũ ��
	case MP_DATE_ENTER_DATEZONE_2:
	case MP_DATE_ENTER_CHALLENGEZONE_2:
		{
			MSG_DWORD2* pPacket = (MSG_DWORD2*)pMsg;

			// ��� �÷��̾� ã��. ��� �÷��̾ ��Ƽ����. �򰥸��� �� �ǿ�~
			USERINFO* pTargetUserInfo = g_pUserTableForObjectID->FindUser(pUserInfo->DateMatchingInfo.nChatPlayerID);
			if (pTargetUserInfo == NULL)
			{
				return;
			}

			// ��û�ڰ� ä������ �÷��̾��ΰ�?
			if (pTargetUserInfo->DateMatchingInfo.nRequestChatState != DATE_MATCHING_CHAT_REQUEST_STATE_CHATTING)
			{
				return;
			}

			// ���� �����ΰ�?
			if (pPacket->dwData2 != RESULT_OK)
			{
				goto FAILED;
			}

			// ä�� ��� ID �Է�
			pPacket->dwObjectID = pTargetUserInfo->dwCharacterID;
			pPacket->dwData2 = pTargetUserInfo->DateMatchingInfo.nChatPlayerID;


			// �ʼ����� ����
			g_Network.Send2Server( pTargetUserInfo->dwMapServerConnectionIndex, pMsg, dwLength );
			return;

FAILED:
			MSG_DWORD stPacket;
			stPacket.Category			= MP_DATE;
			stPacket.Protocol			= pTempMsg->Protocol;
			stPacket.dwObjectID			= pUserInfo->dwCharacterID;
			stPacket.dwData				= pPacket->dwData2;
			g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket));

			stPacket.Category			= MP_DATE;
			stPacket.Protocol			= pTempMsg->Protocol;
			stPacket.dwObjectID			= pTargetUserInfo->dwCharacterID;
			stPacket.dwData				= pPacket->dwData2;
			g_Network.Send2User( pTargetUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket));
		}
		break;
	case MP_DATE_ENTER_DATEZONE_2_DIFF_AGENT:
	case MP_DATE_ENTER_CHALLENGEZONE_2_DIFF_AGENT:
		{
			MSG_DWORD5* pPacket = (MSG_DWORD5*)pMsg;

			// ���� �����ΰ�?
			if (pPacket->dwData5 != RESULT_OK)
			{
				MSG_DWORD stPacket;
				stPacket.Category			= MP_DATE;
				stPacket.Protocol			= pTempMsg->Protocol;
				stPacket.dwObjectID			= pUserInfo->dwCharacterID;
				stPacket.dwData				= pPacket->dwData5;
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket));

				// ��� �÷��̾� ������Ʈ�� �˸���
				{
					MSG_DWORD2 stPacket;
					stPacket.Category			= MP_DATE;
					stPacket.Protocol			= MP_DATE_ENTER_DATEZONE_2_DIFF_AGENT_CHECK_BATTLE_FOR_OTHER_AGENT;
					if (pPacket->Protocol == MP_DATE_ENTER_CHALLENGEZONE_2_DIFF_AGENT)
					{
						stPacket.Protocol = MP_DATE_ENTER_CHALLENGEZONE_2_DIFF_AGENT_CHECK_BATTLE_FOR_OTHER_AGENT;
					}
					stPacket.dwObjectID			= pPacket->dwData3;
					stPacket.dwData1			= pPacket->dwData2;
					stPacket.dwData2			= pPacket->dwData5;
					g_Network.Send2SpecificAgentServer((char*)&stPacket, sizeof(stPacket) );
				}
			}
			else
			{
				// ��� �÷��̾ ��Ƽ���̱⿡ ��Ƽ ���¸� üũ�ϱ� ���� ��� �÷��̾� ������Ʈ�� �˸���
				{
					MSG_DWORD4 stPacket;
					stPacket.Category			= MP_DATE;
					stPacket.Protocol			= MP_DATE_ENTER_DATEZONE_2_DIFF_AGENT_CHECK_PARTY_FOR_OTHER_AGENT;
					if (pPacket->Protocol == MP_DATE_ENTER_CHALLENGEZONE_2_DIFF_AGENT)
					{
						stPacket.Protocol = MP_DATE_ENTER_CHALLENGEZONE_2_DIFF_AGENT_CHECK_PARTY_FOR_OTHER_AGENT;
					}
					stPacket.dwObjectID			= pPacket->dwData3;
					stPacket.dwData1			= pPacket->dwData2;
					stPacket.dwData2			= pUserInfo->dwCharacterID;
					stPacket.dwData3			= g_pServerSystem->GetServerNum();
					stPacket.dwData4			= pPacket->dwData4;
					g_Network.Send2SpecificAgentServer((char*)&stPacket, sizeof(stPacket) );
				}
			}
		}
		break;
	default:
		g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, pMsg, dwLength );
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ASRV_ParseRequestFromServer Method																	   ���� ��û �м�/ó��(������Ʈ)
//
VOID CSHDateManager::ASRV_ParseRequestFromServer(DWORD dwConnectionID, char* pMsg, DWORD dwLength)
{
#if defined(_AGENTSERVER)
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	
	// USERINFO �� �������� �ʴ� ���
	//
	switch( pTempMsg->Protocol )
	{
	case MP_DATE_ENTER_DATEZONE_DIFF_AGENT:
	case MP_DATE_ENTER_CHALLENGEZONE_DIFF_AGENT:
		{
			MSG_DWORD4* pPacket = (MSG_DWORD4*)pMsg;

			// ����ڰ� �ֳ�?
			USERINFO* pTargetUserInfo = g_pUserTableForObjectID->FindUser(pPacket->dwData1);
			if (pTargetUserInfo == NULL)
			{
				return;
			}

			// ��� �÷��̾��� ��ȭ ��밡 ��û���ΰ�?
			if (pTargetUserInfo->DateMatchingInfo.nChatPlayerID != pPacket->dwData2)
			{
				return;
			}

			// ������ �������� üũ�� ���� ���� Ŭ���̾�Ʈ�� ���� - �������� üũ�� Ŭ���̾�Ʈ������ ��
			g_Network.Send2User( pTargetUserInfo->dwConnectionIndex, pMsg, dwLength );
		}
		return;
	case MP_DATE_ENTER_DATEZONE_2_DIFF_AGENT_CHECK_BATTLE_FOR_OTHER_AGENT:
	case MP_DATE_ENTER_CHALLENGEZONE_2_DIFF_AGENT_CHECK_BATTLE_FOR_OTHER_AGENT:
		{
			MSG_DWORD2* pPacket = (MSG_DWORD2*)pMsg;

			// ��û�ڰ� �ֳ�?
			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pPacket->dwData1);
			if (pUserInfo == NULL)
			{
				return;
			}

			// ����ڰ� ���������̴�.
			MSG_DWORD stPacket;
			stPacket.Category			= MP_DATE;
			stPacket.Protocol			= pTempMsg->Protocol;
			stPacket.dwObjectID			= pUserInfo->dwCharacterID;
			stPacket.dwData				= pPacket->dwData2;
			g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket));
		}
		return;
	case MP_DATE_ENTER_DATEZONE_2_DIFF_AGENT_CHECK_PARTY_FOR_OTHER_AGENT:
	case MP_DATE_ENTER_CHALLENGEZONE_2_DIFF_AGENT_CHECK_PARTY_FOR_OTHER_AGENT:
		{
			MSG_DWORD4* pPacket = (MSG_DWORD4*)pMsg;

			// ��û�ڰ� �ֳ�?
			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pPacket->dwData1);
			if (pUserInfo == NULL)
			{
				return;
			}

			// ��Ƽ ���¸� üũ�Ѵ�.
			// ..�ʼ����� �˸���
			MSG_DWORD4 stPacket;
			stPacket.Category			= MP_DATE;
			stPacket.Protocol			= pTempMsg->Protocol;
			stPacket.dwObjectID			= pUserInfo->dwCharacterID;
			stPacket.dwData1			= pPacket->dwData1;
			stPacket.dwData2			= pPacket->dwData2;
			stPacket.dwData3			= pPacket->dwData3;
			stPacket.dwData4			= pPacket->dwData4;
			g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)&stPacket, sizeof(stPacket));
		}
		return;
	case MP_DATE_ENTER_DATEZONE_2_DIFF_AGENT_CHECK_PARTY_FOR_OTHER_AGENT_3:
		{
			MSG_DWORD4* pPacket = (MSG_DWORD4*)pMsg;

			// ��û�ڰ� �ֳ�?
			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pPacket->dwData1);
			if (pUserInfo == NULL)
			{
				return;
			}

			// ����Ʈ ������ �̵�
			// ..�̵� �ε��� ���
			int nZone = 0;
			if (GetDateZoneMoveIndex(pPacket->dwData4, &nZone) == FALSE)
			{
				return;
			}

			MSG_DWORD stPacket;
			stPacket.Category	= MP_USERCONN;
			stPacket.Protocol	= MP_USERCONN_CHANGEMAP_SYN;
			stPacket.dwObjectID	= pUserInfo->dwCharacterID;
			stPacket.dwData		= nZone ;
			UserConn_ChangeMap_Syn(pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
		}
		return;
	case MP_DATE_ENTER_CHALLENGEZONE_2_DIFF_AGENT_CHECK_PARTY_FOR_OTHER_AGENT_3:
		{
			MSG_DWORD4* pPacket = (MSG_DWORD4*)pMsg;

			// ��û�ڰ� �ֳ�?
			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pPacket->dwData1);
			if (pUserInfo == NULL)
			{
				return;
			}

			// ������� ��Ƽ ���¸� üũ�Ѵ�.
			// ..�ʼ����� �˸���
			MSG_DWORD4 stPacket;
			stPacket.Category			= MP_DATE;
			stPacket.Protocol			= MP_DATE_ENTER_CHALLENGEZONE_2_DIFF_AGENT_CHECK_PARTY_FOR_OTHER_AGENT_3;
			stPacket.dwObjectID			= pUserInfo->dwCharacterID;
			stPacket.dwData1			= pPacket->dwData1;
			stPacket.dwData2			= pPacket->dwData2;
			stPacket.dwData3			= pPacket->dwData3;
			stPacket.dwData4			= pPacket->dwData4;
			g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)&stPacket, sizeof(stPacket));
		}
		return;
	case MP_DATE_ENTER_CHALLENGEZONE_2_DIFF_AGENT_CHECK_PARTY_FOR_OTHER_AGENT_5:
		{
			MSG_DWORD5* pPacket = (MSG_DWORD5*)pMsg;

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pPacket->dwData1);
			if (pUserInfo == NULL)
			{
				return;
			}

			// ��Ʈ�ʰ� �̹� ç���� ������ ������ �� �̰��� ����Ǳ� ������ ���� ������ üũ�ϴ� �� ���� �ǹ̰� ����. �׳� ���� ��Ŵ.
			// ç���� ������ �̵�
			// ..���� ��������..
			if (m_nChallengeZoneMoveIndexNum == 0) return;
			int nZone = pPacket->dwData5;

			// ..���� ���� ����
			if (ENTER_CHALLENGE_ZONE_SECTION_NUM == 0 || 
				pPacket->dwData4 >= ENTER_CHALLENGE_ZONE_SECTION_NUM) return;

			pUserInfo->nChallengeZoneEnterSection		= pPacket->dwData4;
			pUserInfo->nChallengeZoneEnterID			= pPacket->dwData3;

			// ..��ȯ �� ����
			// ....ç���� ������ ç���� ������ �̵��ϴ� ��찡 �ƴ� ��쿡��.
			if (IsChallengeZone(pUserInfo->wUserMapNum) == FALSE)
			{
				pUserInfo->nChallengeZoneReturnMapNum		= pUserInfo->wUserMapNum;
			}

			// ..�̵�
			MSG_DWORD stPacket;

			stPacket.Category	= MP_USERCONN;
			stPacket.Protocol	= MP_USERCONN_CHANGEMAP_SYN;
			stPacket.dwObjectID	= pUserInfo->dwCharacterID;
			stPacket.dwData		= nZone ;
			UserConn_ChangeMap_Syn(pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
		}
		return;
	case MP_DATE_ENTER_CHALLENGEZONE_2_DIFF_AGENT_CHECK_PARTY_FOR_OTHER_AGENT_6:
		{
			MSG_DWORD2* pPacket = (MSG_DWORD2*)pMsg;

			// ��û�ڰ� �ֳ�?
			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pPacket->dwData1);
			if (pUserInfo == NULL)
			{
				return;
			}

			// Ŭ���̾�Ʈ�� �˸���
			MSG_DWORD stPacket;
			stPacket.Category			= MP_DATE;
			stPacket.Protocol			= pTempMsg->Protocol;
			stPacket.dwObjectID			= pUserInfo->dwCharacterID;
			stPacket.dwData				= pPacket->dwData2;
			g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket));
		}
		return;
	}

	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pTempMsg->dwObjectID);
	if (pUserInfo == NULL) return;


	switch( pTempMsg->Protocol )
	{
	// ����Ʈ �� ���� - �ʼ������� ��Ƽ Ȯ�� ��
	case MP_DATE_ENTER_DATEZONE_2:
		{
			MSG_DWORD* pPacket = (MSG_DWORD*)pMsg;
			// ��û�ڰ� ä������ �÷��̾��ΰ�?
			if (pUserInfo->DateMatchingInfo.nRequestChatState != DATE_MATCHING_CHAT_REQUEST_STATE_CHATTING)
			{
				return;
			}

			// ��� �÷��̾� ã��
			USERINFO* pTargetUserInfo = g_pUserTableForObjectID->FindUser(pUserInfo->DateMatchingInfo.nChatPlayerID);
			if (pTargetUserInfo == NULL)
			{
				return;
			}

			// ����Ʈ ������ �̵�
			// ..�̵� �ε��� ���
			int nZone = 0;
			if (GetDateZoneMoveIndex(pPacket->dwData, &nZone) == FALSE)
			{
				return;
			}

			MSG_DWORD stPacket;

			stPacket.Category	= MP_USERCONN;
			stPacket.Protocol	= MP_USERCONN_CHANGEMAP_SYN;
			stPacket.dwObjectID	= pUserInfo->dwCharacterID;
			stPacket.dwData		= nZone ;
			UserConn_ChangeMap_Syn(pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );

			stPacket.Category	= MP_USERCONN;
			stPacket.Protocol	= MP_USERCONN_CHANGEMAP_SYN;
			stPacket.dwObjectID	= pTargetUserInfo->dwCharacterID;
			stPacket.dwData		= nZone ;
			UserConn_ChangeMap_Syn( pTargetUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket));
		}
		break;
	case MP_DATE_ENTER_DATEZONE_2_DIFF_AGENT_CHECK_PARTY_FOR_OTHER_AGENT_2:
		{
			MSG_DWORD4* pPacket = (MSG_DWORD4*)pMsg;
			// ��û�ڰ� ä������ �÷��̾��ΰ�?
			if (pUserInfo->DateMatchingInfo.nRequestChatState != DATE_MATCHING_CHAT_REQUEST_STATE_CHATTING)
			{
				return;
			}

			// ����Ʈ ������ �̵�
			// ..�̵� �ε��� ���
			int nZone = 0;
			if (GetDateZoneMoveIndex(pPacket->dwData4, &nZone) == FALSE)
			{
				return;
			}

			MSG_DWORD stPacket;

			stPacket.Category	= MP_USERCONN;
			stPacket.Protocol	= MP_USERCONN_CHANGEMAP_SYN;
			stPacket.dwObjectID	= pUserInfo->dwCharacterID;
			stPacket.dwData		= nZone ;
			UserConn_ChangeMap_Syn(pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );

			// ����� ������Ʈ�� �˸���
			{
				MSG_DWORD4 stPacket;
				stPacket.Category			= MP_DATE;
				stPacket.Protocol			= MP_DATE_ENTER_DATEZONE_2_DIFF_AGENT_CHECK_PARTY_FOR_OTHER_AGENT_3;
				stPacket.dwObjectID			= pPacket->dwData3;
				stPacket.dwData1			= pPacket->dwData2;
				stPacket.dwData2			= pUserInfo->dwCharacterID;
				stPacket.dwData3			= g_pServerSystem->GetServerNum();
				stPacket.dwData4			= pPacket->dwData4;
				g_Network.Send2SpecificAgentServer((char*)&stPacket, sizeof(stPacket) );
			}
		}
		break;
	// ç���� �� ���� - �ʼ������� ��Ƽ Ȯ�� ��, ��Ʈ���� ���� ���� ȸ���� üũ..
	case MP_DATE_ENTER_CHALLENGEZONE_2:
		{
			MSG_DWORD2* pPacket = (MSG_DWORD2*)pMsg;
			// ��û�ڰ� ä������ �÷��̾��ΰ�?
			if (pUserInfo->DateMatchingInfo.nRequestChatState != DATE_MATCHING_CHAT_REQUEST_STATE_CHATTING)
			{
				return;
			}

			// ��� �÷��̾� ã��
			USERINFO* pTargetUserInfo = g_pUserTableForObjectID->FindUser(pUserInfo->DateMatchingInfo.nChatPlayerID);
			if (pTargetUserInfo == NULL)
			{
				return;
			}

			if (pPacket->dwData2 == RESULT_OK)
			{
				// ��Ƽ���� ��Ƽ ���¿� �ڽ��� ���� ȸ���� üũ�ϰ� ������
				// �̹��� ��Ʈ���� ���� ȸ���� üũ�ϱ� ���� �� �ʼ�����.. ����..
				MSG_DWORD2 stPacket;
				stPacket.Category			= MP_DATE;
				stPacket.Protocol			= MP_DATE_ENTER_CHALLENGEZONE_3;
				stPacket.dwObjectID			= pTargetUserInfo->dwCharacterID;
				stPacket.dwData1			= pPacket->dwData1;
				g_Network.Send2Server( pTargetUserInfo->dwMapServerConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
			}
			else
			{
				MSG_DWORD stPacket;
				stPacket.Category			= MP_DATE;
				stPacket.Protocol			= pTempMsg->Protocol;
				stPacket.dwObjectID			= pUserInfo->dwCharacterID;
				stPacket.dwData				= pPacket->dwData2;
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket));
			}
		}
		break;
	case MP_DATE_ENTER_CHALLENGEZONE_2_DIFF_AGENT_CHECK_PARTY_FOR_OTHER_AGENT_2:
		{
			MSG_DWORD5* pPacket = (MSG_DWORD5*)pMsg;
			// ��û�ڰ� ä������ �÷��̾��ΰ�?
			if (pUserInfo->DateMatchingInfo.nRequestChatState != DATE_MATCHING_CHAT_REQUEST_STATE_CHATTING)
			{
				return;
			}

			if (pPacket->dwData5 == RESULT_OK)
			{
				// ��Ƽ���� ��Ƽ ���¿� �ڽ��� ���� ȸ���� üũ�ϰ� ������
				// �̹��� ��Ʈ���� ���� ȸ���� üũ�ϱ� ���� �� �ʼ�����.. ����..
				// ������Ʈ�� �ٸ��Ƿ� ����� ������Ʈ��..
				MSG_DWORD4 stPacket;
				stPacket.Category			= MP_DATE;
				stPacket.Protocol			= MP_DATE_ENTER_CHALLENGEZONE_2_DIFF_AGENT_CHECK_PARTY_FOR_OTHER_AGENT_3;
				stPacket.dwObjectID			= pPacket->dwData3;
				stPacket.dwData1			= pPacket->dwData2;
				stPacket.dwData2			= pUserInfo->dwCharacterID;
				stPacket.dwData3			= g_pServerSystem->GetServerNum();
				stPacket.dwData4			= pPacket->dwData4;
				g_Network.Send2SpecificAgentServer((char*)&stPacket, sizeof(stPacket) );
			}
			else
			{
				MSG_DWORD stPacket;
				stPacket.Category			= MP_DATE;
				stPacket.Protocol			= pTempMsg->Protocol;
				stPacket.dwObjectID			= pUserInfo->dwCharacterID;
				stPacket.dwData				= pPacket->dwData5;
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket));
			}
		}
		break;
	// ç���� �� ���� - ��Ʈ���� ���� ���� ȸ�� Ȯ�� ��
	case MP_DATE_ENTER_CHALLENGEZONE_3:
		{
			MSG_DWORD2* pPacket = (MSG_DWORD2*)pMsg;
			// ��û�ڰ� ä������ �÷��̾��ΰ�?
			if (pUserInfo->DateMatchingInfo.nRequestChatState != DATE_MATCHING_CHAT_REQUEST_STATE_CHATTING)
			{
				return;
			}

			// ��� �÷��̾� ã��
			USERINFO* pTargetUserInfo = g_pUserTableForObjectID->FindUser(pUserInfo->DateMatchingInfo.nChatPlayerID);
			if (pTargetUserInfo == NULL)
			{
				return;
			}

			if (pPacket->dwData2 == RESULT_OK)
			{
				// ç���� ������ �̵�
				// ..���� ��������..
				if (m_nChallengeZoneMoveIndexNum == 0) return;
				int nZone = m_pstChallengeZoneMoveIndexList[rand()%m_nChallengeZoneMoveIndexNum].nMoveIndex;

				// ..���� ���� ����
				if (ENTER_CHALLENGE_ZONE_SECTION_NUM == 0 || 
					pPacket->dwData1 >= ENTER_CHALLENGE_ZONE_SECTION_NUM) return;
				pUserInfo->nChallengeZoneEnterSection		= pPacket->dwData1;
				pTargetUserInfo->nChallengeZoneEnterSection = pPacket->dwData1;

				// ..���� ���� ����
				// ....������Ʈ�� �������� ���� ������Ʈ ���� ������ ����� �ϹǷ� ������Ʈ���� 100000000 �� ���� ����
				// ....0 �� ������Ʈ 0 ~ 100000000, 1 �� ������Ʈ 100000000 ~ 200000000, 2 �� ������Ʈ 200000000 ~ 300000000 ....
				// ....�׷��⿡ ������Ʈ ��ȣ�� ���� ������ 1�� �����ؾ��ϸ� 42��(������Ʈ �ϳ��� 1���� �Ҵ��� ��)�� ���� �ʴ� ���� ������
				// ....�߻��� Ȯ���� ����.
				pUserInfo->nChallengeZoneEnterID			= m_nChallengeZoneEnterNum + g_pServerSystem->GetServerNum()*100000000;
				pTargetUserInfo->nChallengeZoneEnterID		= m_nChallengeZoneEnterNum + g_pServerSystem->GetServerNum()*100000000;
				m_nChallengeZoneEnterNum++;
				// ..Ȥ�� �ִ������ ���� ��� �⺻������ ����. ������ ���� ���ķ� 42xxxxxxxx ���� ��Ī ç���� ����...���� �̷�����..?
				if (m_nChallengeZoneEnterNum < 1000) m_nChallengeZoneEnterNum = 1000;

				// ..��ȯ �� ����
				// ....ç���� ������ ç���� ������ �̵��ϴ� ��찡 �ƴ� ��쿡��.
				if (IsChallengeZone(pUserInfo->wUserMapNum) == FALSE)
				{
					pUserInfo->nChallengeZoneReturnMapNum		= pUserInfo->wUserMapNum;
					pTargetUserInfo->nChallengeZoneReturnMapNum	= pTargetUserInfo->wUserMapNum;
				}

				// ..�̵�
				MSG_DWORD stPacket;

				stPacket.Category	= MP_USERCONN;
				stPacket.Protocol	= MP_USERCONN_CHANGEMAP_SYN;
				stPacket.dwObjectID	= pUserInfo->dwCharacterID;
				stPacket.dwData		= nZone ;
				UserConn_ChangeMap_Syn(pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );

				stPacket.Category	= MP_USERCONN;
				stPacket.Protocol	= MP_USERCONN_CHANGEMAP_SYN;
				stPacket.dwObjectID	= pTargetUserInfo->dwCharacterID;
				stPacket.dwData		= nZone ;
				UserConn_ChangeMap_Syn( pTargetUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket));
			}
			else
			{
				MSG_DWORD stPacket;
				stPacket.Category			= MP_DATE;
				stPacket.Protocol			= pTempMsg->Protocol;
				stPacket.dwObjectID			= pTargetUserInfo->dwCharacterID;
				stPacket.dwData				= pPacket->dwData2;
				g_Network.Send2User( pTargetUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket));
			}
		}
		break;
	case MP_DATE_ENTER_CHALLENGEZONE_2_DIFF_AGENT_CHECK_PARTY_FOR_OTHER_AGENT_4:
		{
			MSG_DWORD5* pPacket = (MSG_DWORD5*)pMsg;
			// ��û�ڰ� ä������ �÷��̾��ΰ�?
			if (pUserInfo->DateMatchingInfo.nRequestChatState != DATE_MATCHING_CHAT_REQUEST_STATE_CHATTING)
			{
				return;
			}

			if (pPacket->dwData5 == RESULT_OK)
			{
				// ç���� ������ �̵�
				// ..���� ��������..
				if (m_nChallengeZoneMoveIndexNum == 0) return;
				int nZone = m_pstChallengeZoneMoveIndexList[rand()%m_nChallengeZoneMoveIndexNum].nMoveIndex;

				// ..���� ���� ����
				if (ENTER_CHALLENGE_ZONE_SECTION_NUM == 0 || 
					pPacket->dwData4 >= ENTER_CHALLENGE_ZONE_SECTION_NUM) return;
				pUserInfo->nChallengeZoneEnterSection		= pPacket->dwData4;

				// ..���� ���� ����
				// ....������Ʈ�� �������� ���� ������Ʈ ���� ������ ����� �ϹǷ� ������Ʈ���� 100000000 �� ���� ����
				// ....0 �� ������Ʈ 0 ~ 100000000, 1 �� ������Ʈ 100000000 ~ 200000000, 2 �� ������Ʈ 200000000 ~ 300000000 ....
				// ....�׷��⿡ ������Ʈ ��ȣ�� ���� ������ 1�� �����ؾ��ϸ� 42��(������Ʈ �ϳ��� 1���� �Ҵ��� ��)�� ���� �ʴ� ���� ������
				// ....�߻��� Ȯ���� ����.
				pUserInfo->nChallengeZoneEnterID			= m_nChallengeZoneEnterNum + g_pServerSystem->GetServerNum()*100000000;
				m_nChallengeZoneEnterNum++;
				// ..Ȥ�� �ִ������ ���� ��� �⺻������ ����. ������ ���� ���ķ� 42xxxxxxxx ���� ��Ī ç���� ����...���� �̷�����..?
				if (m_nChallengeZoneEnterNum < 1000) m_nChallengeZoneEnterNum = 1000;

				// ..��ȯ �� ����
				// ....ç���� ������ ç���� ������ �̵��ϴ� ��찡 �ƴ� ��쿡��.
				if (IsChallengeZone(pUserInfo->wUserMapNum) == FALSE)
				{
					pUserInfo->nChallengeZoneReturnMapNum		= pUserInfo->wUserMapNum;
				}

				// ..�̵�
				MSG_DWORD stPacket;

				stPacket.Category	= MP_USERCONN;
				stPacket.Protocol	= MP_USERCONN_CHANGEMAP_SYN;
				stPacket.dwObjectID	= pUserInfo->dwCharacterID;
				stPacket.dwData		= nZone ;
				UserConn_ChangeMap_Syn(pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );

				// ..����� ������Ʈ�� �˸���
				{
					MSG_DWORD5 stPacket;
					stPacket.Category			= MP_DATE;
					stPacket.Protocol			= MP_DATE_ENTER_CHALLENGEZONE_2_DIFF_AGENT_CHECK_PARTY_FOR_OTHER_AGENT_5;
					stPacket.dwObjectID			= pPacket->dwData3;
					stPacket.dwData1			= pPacket->dwData2;
					stPacket.dwData2			= pPacket->dwData1;
					stPacket.dwData3			= pUserInfo->nChallengeZoneEnterID;
					stPacket.dwData4			= pPacket->dwData4;
					stPacket.dwData5			= nZone;
					g_Network.Send2SpecificAgentServer((char*)&stPacket, sizeof(stPacket) );
				}
			}
			else
			{
				// Ŭ���̾�Ʈ�� �˸���
				MSG_DWORD stPacket;
				stPacket.Category			= MP_DATE;
				stPacket.Protocol			= pTempMsg->Protocol;
				stPacket.dwObjectID			= pUserInfo->dwCharacterID;
				stPacket.dwData				= pPacket->dwData5;
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket));

				// ����� ������Ʈ�� �˸���
				{
					MSG_DWORD2 stPacket;
					stPacket.Category			= MP_DATE;
					stPacket.Protocol			= MP_DATE_ENTER_CHALLENGEZONE_2_DIFF_AGENT_CHECK_PARTY_FOR_OTHER_AGENT_6;
					stPacket.dwObjectID			= pPacket->dwData3;
					stPacket.dwData1			= pPacket->dwData2;
					stPacket.dwData2			= pPacket->dwData5;
					g_Network.Send2SpecificAgentServer((char*)&stPacket, sizeof(stPacket) );
				}
			}
		}
		break;
	// ç���� ������ ���� ������ ��ȯ
	case MP_DATE_CHALLENGEZONE_RETURN:
		{
			MSG_DWORD stPacket;

			stPacket.Category	= MP_USERCONN;
			stPacket.Protocol	= MP_USERCONN_CHANGEMAP_SYN;
			stPacket.dwObjectID	= pUserInfo->dwCharacterID;
			stPacket.dwData		= g_pServerSystem->GetMapChangeIndex(pUserInfo->nChallengeZoneReturnMapNum);
			UserConn_ChangeMap_Syn(pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
		}
		break;
	default:
		g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)pMsg, dwLength );
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_ParseRequestFromClient Method																		   Ŭ���̾�Ʈ ��û �м�/ó��
//
VOID CSHDateManager::SRV_ParseRequestFromClient(DWORD dwConnectionID, char* pMsg, DWORD dwLength)
{
#if defined(_MAPSERVER_)
	MSGBASE* pPacket = (MSGBASE*)pMsg;

	// �߸��� �÷��̾� ó��
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pPacket->dwObjectID );
	if( !pPlayer )	
	{
		return;
	}

	switch(pPacket->Protocol)
	{
	// ����Ʈ �� ���� 
	case MP_DATE_ENTER_DATEZONE_2:
		{
			MSG_DWORD2* pPacket = (MSG_DWORD2*)pMsg;
			SRV_EnterDateZone(pPlayer, pPacket->dwData2, pPacket->dwData1);
		}
		break;
	case MP_DATE_ENTER_DATEZONE_2_DIFF_AGENT_CHECK_PARTY_FOR_OTHER_AGENT:
		{
			MSG_DWORD4* pPacket = (MSG_DWORD4*)pMsg;
			SRV_EnterDateZone2(pPlayer, pPacket);
		}
		break;
	// ç���� �� ���� 
	case MP_DATE_ENTER_CHALLENGEZONE_2:
		{
			MSG_DWORD2* pPacket = (MSG_DWORD2*)pMsg;
			SRV_EnterChallengeZone(pPlayer, pPacket->dwData2, pPacket->dwData1);
		}
		break;
	case MP_DATE_ENTER_CHALLENGEZONE_3:
		{
			MSG_DWORD2* pPacket = (MSG_DWORD2*)pMsg;
			SRV_EnterChallengeZone(pPlayer, pPacket->dwData2, pPacket->dwData1, 1);
		}
		break;
	case MP_DATE_ENTER_CHALLENGEZONE_2_DIFF_AGENT_CHECK_PARTY_FOR_OTHER_AGENT:
		{
			MSG_DWORD4* pPacket = (MSG_DWORD4*)pMsg;
			SRV_EnterChallengeZone2(pPlayer, pPacket);
		}
		break;
	case MP_DATE_ENTER_CHALLENGEZONE_2_DIFF_AGENT_CHECK_PARTY_FOR_OTHER_AGENT_3:
		{
			MSG_DWORD4* pPacket = (MSG_DWORD4*)pMsg;
			SRV_EnterChallengeZone2(pPlayer, pPacket, 1);
		}
		break;
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_IsPossibleEnterDateZone Method																		  ����Ʈ �� ������ �����Ѱ�?
//
BOOL CSHDateManager::CLI_IsPossibleEnterDateZone()
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	CDateMatchingDlg* pDlg = GAMEIN->GetDateMatchingDlg() ;

	if( pDlg &&
		pDlg->GetChatingDlg()->IsOnChatMode() &&
		HERO->GetGender() != g_csResidentRegistManager.CLI_GetChatPartner().stRegistInfo.nSex)
	{
		return TRUE;
	}

#endif
	return FALSE;
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_IsPossibleEnterChallengeZone Method																	  ç���� �� ������ �����Ѱ�?
//
BOOL CSHDateManager::CLI_IsPossibleEnterChallengeZone()
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	CDateMatchingDlg* pDlg = GAMEIN->GetDateMatchingDlg() ;

	if( pDlg &&
		pDlg->GetChatingDlg()->IsOnChatMode() &&
		HERO->GetGender() != g_csResidentRegistManager.CLI_GetChatPartner().stRegistInfo.nSex &&
		g_csResidentRegistManager.CLI_GetChatPartner().nMatchingPoint >= ENTER_CHALLENGE_ZONE_MATCHING_POINT &&
		IsChallengeZoneHere() == FALSE)
	{
		return TRUE;
	}

#endif
	return FALSE;
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_RequestDateMatchingEnterDateZone Method															  ����Ʈ ��Ī ����Ʈ �� ���� ��û
//
VOID CSHDateManager::CLI_RequestDateMatchingEnterDateZone(int nZoneIndex)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	if (HERO->GetObjectBattleState() == eObjectBattleState_Battle)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1209) );
		return;
	}

	MSG_DWORD2 stPacket;
	stPacket.Category 		= MP_DATE;
	stPacket.Protocol 		= MP_DATE_ENTER_DATEZONE;
	stPacket.dwObjectID		= HEROID;
	stPacket.dwData1		= nZoneIndex;

	if (g_csResidentRegistManager.CLI_GetChatPartner().nAgentID >= 1000)
	{
		stPacket.Protocol 		= MP_DATE_ENTER_DATEZONE_DIFF_AGENT;
		stPacket.dwData2		= g_csResidentRegistManager.CLI_GetChatPartner().nAgentID - 1000;
	}

	NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_RequestDateMatchingEnterChallengeZone Method													  ����Ʈ ��Ī ç���� �� ���� ��û
//
VOID CSHDateManager::CLI_RequestDateMatchingEnterChallengeZone(int nZoneIndex)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	if (HERO->GetObjectBattleState() == eObjectBattleState_Battle)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1209) );
		return;
	}

	MSG_DWORD2 stPacket;
	stPacket.Category 		= MP_DATE;
	stPacket.Protocol 		= MP_DATE_ENTER_CHALLENGEZONE;
	stPacket.dwObjectID		= HEROID;
	stPacket.dwData1		= nZoneIndex;

	if (g_csResidentRegistManager.CLI_GetChatPartner().nAgentID >= 1000)
	{
		stPacket.Protocol 		= MP_DATE_ENTER_CHALLENGEZONE_DIFF_AGENT;
		stPacket.dwData2		= g_csResidentRegistManager.CLI_GetChatPartner().nAgentID - 1000;
	}

	NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_ParseAnswerFromSrv Method																			  �����κ����� ���� �м�/ó��
//
VOID CSHDateManager::CLI_ParseAnswerFromSrv(void* pMsg)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	MSGBASE* pTmp = (MSGBASE*)pMsg;

	switch(pTmp->Protocol)
	{
	// ����Ʈ �� ������ ���� ���� ���� üũ
	case MP_DATE_ENTER_DATEZONE:
		{
			MSG_DWORD2* pPacket = (MSG_DWORD2*)pMsg;
			MSG_DWORD2 stPacket;

			stPacket.Category 		= MP_DATE;
			stPacket.Protocol 		= MP_DATE_ENTER_DATEZONE_2;
			stPacket.dwObjectID		= HEROID;
			stPacket.dwData1		= pPacket->dwData1;
			stPacket.dwData2		= RESULT_OK;
			if (HERO->GetObjectBattleState() == eObjectBattleState_Battle)
			{
				stPacket.dwData2 = RESULT_FAIL_01;
			}

			NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
		}
		break;
	// ����Ʈ �� ������ ���� ���� ���� üũ
	case MP_DATE_ENTER_DATEZONE_DIFF_AGENT:
		{
			MSG_DWORD4* pPacket = (MSG_DWORD4*)pMsg;
			MSG_DWORD5 stPacket;

			stPacket.Category 		= MP_DATE;
			stPacket.Protocol 		= MP_DATE_ENTER_DATEZONE_2_DIFF_AGENT;
			stPacket.dwObjectID		= HEROID;
			stPacket.dwData1		= pPacket->dwData1;
			stPacket.dwData2		= pPacket->dwData2;
			stPacket.dwData3		= pPacket->dwData3;
			stPacket.dwData4		= pPacket->dwData4;
			stPacket.dwData5		= RESULT_OK;
			if (HERO->GetObjectBattleState() == eObjectBattleState_Battle)
			{
				stPacket.dwData5 = RESULT_FAIL_01;
			}

			NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
		}
		break;
	// ç���� �� ������ ���� ���� ���� üũ
	case MP_DATE_ENTER_CHALLENGEZONE:
		{
			MSG_DWORD2* pPacket = (MSG_DWORD2*)pMsg;
			MSG_DWORD2 stPacket;

			stPacket.Category 		= MP_DATE;
			stPacket.Protocol 		= MP_DATE_ENTER_CHALLENGEZONE_2;
			stPacket.dwObjectID		= HEROID;
			stPacket.dwData1		= pPacket->dwData1;
			stPacket.dwData2		= RESULT_OK;
			if (HERO->GetObjectBattleState() == eObjectBattleState_Battle)
			{
				stPacket.dwData2 = RESULT_FAIL_01;
			}

			NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
		}
		break;
	// ç���� �� ������ ���� ���� ���� üũ
	case MP_DATE_ENTER_CHALLENGEZONE_DIFF_AGENT:
		{
			MSG_DWORD4* pPacket = (MSG_DWORD4*)pMsg;
			MSG_DWORD5 stPacket;

			stPacket.Category 		= MP_DATE;
			stPacket.Protocol 		= MP_DATE_ENTER_CHALLENGEZONE_2_DIFF_AGENT;
			stPacket.dwObjectID		= HEROID;
			stPacket.dwData1		= pPacket->dwData1;
			stPacket.dwData2		= pPacket->dwData2;
			stPacket.dwData3		= pPacket->dwData3;
			stPacket.dwData4		= pPacket->dwData4;
			stPacket.dwData5		= RESULT_OK;
			if (HERO->GetObjectBattleState() == eObjectBattleState_Battle)
			{
				stPacket.dwData5 = RESULT_FAIL_01;
			}

			NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
		}
		break;
	// ����Ʈ �� ���� �Ұ�
	// ç���� �� ���� �Ұ�
	case MP_DATE_ENTER_DATEZONE_2:
	case MP_DATE_ENTER_CHALLENGEZONE_2:
	case MP_DATE_ENTER_DATEZONE_2_DIFF_AGENT:
	case MP_DATE_ENTER_DATEZONE_2_DIFF_AGENT_CHECK_BATTLE_FOR_OTHER_AGENT:
	case MP_DATE_ENTER_CHALLENGEZONE_2_DIFF_AGENT:
	case MP_DATE_ENTER_CHALLENGEZONE_2_DIFF_AGENT_CHECK_BATTLE_FOR_OTHER_AGENT:
	case MP_DATE_ENTER_CHALLENGEZONE_2_DIFF_AGENT_CHECK_PARTY_FOR_OTHER_AGENT_2:
		{
			MSG_DWORD* pPacket = (MSG_DWORD*)pMsg;
			switch(pPacket->dwData)
			{
			case RESULT_FAIL_01:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1209) );	break;
			case RESULT_FAIL_02:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1210) );	break;
//			case RESULT_FAIL_03:		CHATMGR->AddMsg( CTC_SYSMSG, "�̹� ç���� ���Դϴ�." );		break;
			case RESULT_FAIL_04:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1211) );	break;
			case RESULT_FAIL_05:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1263) );	break;
			}
		}
		break;
	case MP_DATE_ENTER_CHALLENGEZONE_3:
	case MP_DATE_ENTER_CHALLENGEZONE_2_DIFF_AGENT_CHECK_PARTY_FOR_OTHER_AGENT_6:
		{
			MSG_DWORD* pPacket = (MSG_DWORD*)pMsg;
			switch(pPacket->dwData)
			{
			case RESULT_FAIL_02:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1212) );			break;
//			case RESULT_FAIL_03:		CHATMGR->AddMsg( CTC_SYSMSG, "��Ʈ�ʰ� �̹� ç���� ���Դϴ�." );	break;
			case RESULT_FAIL_04:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1213) );			break;
			case RESULT_FAIL_05:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1264) );	break;
			}
		}
		break;
	// ç���� �� ����
	case MP_DATE_CHALLENGEZONE_START:
		{
 			char szTxt[256] = "";
			MSG_NAME2_DWORD2* pPacket = (MSG_NAME2_DWORD2*)pMsg;

			m_nChallengeZoneState = CHALLENGEZONE_START;
			m_nChallengeZoneTimeTick = gCurTime;
			m_nChallengeZoneTime = 0;

			GAMEIN->GetChallengeZoneClearNo1Dlg()->SetActive(TRUE);
			// ����
			sprintf(szTxt, "LV: %d - %d", m_pstChallengeZoneSectionMonLevel[pPacket->dwData1].nStart, m_pstChallengeZoneSectionMonLevel[pPacket->dwData1].nEnd);
			GAMEIN->GetChallengeZoneClearNo1Dlg()->SetChallengeZoneLeastClearSection(szTxt);
			GAMEIN->GetChallengeZoneClearNo1Dlg()->SetChallengeZoneStartTime(m_pstChallengeZoneSectionMonLevel[pPacket->dwData1].nLimitTime - CHALLENGE_ZONE_START_DELAY_TIME);
			GAMEIN->GetChallengeZoneClearNo1Dlg()->SetChallengeZoneLeastClearName1(pPacket->Name1);
			GAMEIN->GetChallengeZoneClearNo1Dlg()->SetChallengeZoneLeastClearName2(pPacket->Name2);

			ZeroMemory(szTxt, sizeof(szTxt));
			if (pPacket->dwData2)
			{
				DWORD nSec = pPacket->dwData2/1000;
				sprintf(szTxt, "%02d:%02d:%03d", nSec/60, nSec%60, pPacket->dwData2%1000);
			}
			GAMEIN->GetChallengeZoneClearNo1Dlg()->SetChallengeZoneLeastClearTime(szTxt);

		}
		break;
	// ç���� �� ���� ȸ�� ����
	case MP_DATE_CHALLENGEZONE_ENTER_FREQ:
		{
			MSG_DWORD2* pPacket = (MSG_DWORD2*)pMsg;

 			if (HERO == NULL) return;

			HERO->SetChallengeZoneEnterFreq(pPacket->dwData1);
			HERO->SetChallengeZoneEnterBonusFreq(pPacket->dwData2);

			int nPossibleFreq = ENTER_CHALLENGE_ZONE_FREQ_PER_DAY - pPacket->dwData1;
			if (nPossibleFreq < 0) nPossibleFreq = 0;
			nPossibleFreq += pPacket->dwData2;
			GAMEIN->GetDateMatchingDlg()->SetEnterChallengeZoneFreq(nPossibleFreq);
		}
		break;
	// ç���� �� ���� - Ÿ�Ӿƿ�
	case MP_DATE_CHALLENGEZONE_END_TIMEOUT:
		{
			MSG_DWORD* pPacket = (MSG_DWORD*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1214));
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1220), CHALLENGE_ZONE_END_DELAY_TIME/1000);
			m_nChallengeZoneState = CHALLENGEZONE_END;
			m_nChallengeZoneTimeTick = gCurTime;
			m_nChallengeZoneTime = 0;

			GAMEIN->GetChallengeZoneClearNo1Dlg()->SetChallengeZoneStartTime(0);
			GAMEIN->GetChallengeZoneClearNo1Dlg()->SetChallengeZoneStartTimeTick(0);
			// ����Ʈ ����
			if (CHALLENGE_ZONE_MOTION_NUM_FAIL)
				HEFFPROC nEff = EFFECTMGR->StartEffectProcess(CHALLENGE_ZONE_MOTION_NUM_FAIL, HERO, NULL, 0, 0);
		}
		break;
	// ç���� �� ���� - ��Ʈ���� ��Ż
	case MP_DATE_CHALLENGEZONE_END_PARTNER_OUT:
		{
			MSG_DWORD* pPacket = (MSG_DWORD*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1215));
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1220), CHALLENGE_ZONE_END_DELAY_TIME/1000);
			m_nChallengeZoneState = CHALLENGEZONE_END;
			m_nChallengeZoneTimeTick = gCurTime;
			m_nChallengeZoneTime = 0;

			GAMEIN->GetChallengeZoneClearNo1Dlg()->SetChallengeZoneStartTime(0);
			GAMEIN->GetChallengeZoneClearNo1Dlg()->SetChallengeZoneStartTimeTick(0);
			// ����Ʈ ����
			if (CHALLENGE_ZONE_MOTION_NUM_FAIL)
				HEFFPROC nEff = EFFECTMGR->StartEffectProcess(CHALLENGE_ZONE_MOTION_NUM_FAIL, HERO, NULL, 0, 0);
		}
		break;
	// ç���� �� ���� - ��� ���
	case MP_DATE_CHALLENGEZONE_END_ALL_DIE:
		{
			MSG_DWORD* pPacket = (MSG_DWORD*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1216));
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1220), CHALLENGE_ZONE_END_DELAY_TIME/1000);
			m_nChallengeZoneState = CHALLENGEZONE_END;
			m_nChallengeZoneTimeTick = gCurTime;
			m_nChallengeZoneTime = 0;

			GAMEIN->GetChallengeZoneClearNo1Dlg()->SetChallengeZoneStartTime(0);
			GAMEIN->GetChallengeZoneClearNo1Dlg()->SetChallengeZoneStartTimeTick(0);
			// ����Ʈ ����
			if (CHALLENGE_ZONE_MOTION_NUM_FAIL)
				HEFFPROC nEff = EFFECTMGR->StartEffectProcess(CHALLENGE_ZONE_MOTION_NUM_FAIL, HERO, NULL, 0, 0);
		}
		break;
	// ç���� �� ���� 
	case MP_DATE_CHALLENGEZONE_END_SUCCESS:
		{
			MSG_DWORD* pPacket = (MSG_DWORD*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1217));
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1220), CHALLENGE_ZONE_END_DELAY_TIME/1000);
			m_nChallengeZoneState = CHALLENGEZONE_END;
			m_nChallengeZoneTimeTick = gCurTime;
			m_nChallengeZoneTime = 0;

			GAMEIN->GetChallengeZoneClearNo1Dlg()->SetChallengeZoneStartTime(0);
			GAMEIN->GetChallengeZoneClearNo1Dlg()->SetChallengeZoneStartTimeTick(0);
			// �ִ� �ð� Ŭ���� Ÿ���� �����߳�?
			// ..����Ʈ ����
			if (pPacket->dwData)
			{
				if (CHALLENGE_ZONE_MOTION_NUM_SUCCESS)
					HEFFPROC nEff = EFFECTMGR->StartEffectProcess(CHALLENGE_ZONE_MOTION_NUM_SUCCESS, HERO, NULL, 0, 0);
			}
			else
			{
				if (CHALLENGE_ZONE_MOTION_NUM_SUCCESS_LEAST_CLEAR_TIME)
					HEFFPROC nEff = EFFECTMGR->StartEffectProcess(CHALLENGE_ZONE_MOTION_NUM_SUCCESS_LEAST_CLEAR_TIME, HERO, NULL, 0, 0);
			}
		}
		break;
	}
#endif
}

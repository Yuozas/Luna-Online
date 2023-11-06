/*********************************************************************

	 ����		: SHMonstermeterManager.cpp
	 �ۼ���		: hseos
	 �ۼ���		: 2007/07/08

	 ���ϼ���	: ���͹��� Ŭ������ �ҽ�

 *********************************************************************/

#include "stdafx.h"
#include "SHMonstermeterManager.h"
#include "MHFile.h"
#include "ItemManager.h"
#include "Player.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CSHMonstermeterManager
//

CSHMonstermeterManager g_csMonstermeterManager;

// -------------------------------------------------------------------------------------------------------------------------------------
// CSHMonstermeterManager Method																								  ������
//
CSHMonstermeterManager::CSHMonstermeterManager()
{
	ZeroMemory(m_stReward, sizeof(m_stReward));
	LoadScriptFileData();
}

// -------------------------------------------------------------------------------------------------------------------------------------
// ~CSHMonstermeterManager Method																								  �ı���
//
CSHMonstermeterManager::~CSHMonstermeterManager()
{
	for(int i=0; i<MAX_REWARD_BEHAVE_TYPE; i++)
	{
		SAFE_DELETE_ARRAY(m_stReward[i].stRewardBase);
	}
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  LoadScriptData Method																							  ��ũ��Ʈ ���� �ε�
//
BOOL CSHMonstermeterManager::LoadScriptFileData()
{
 	char szFile[MAX_PATH];
	char szLine[MAX_PATH], szTxt[MAX_PATH];
	int	 nKind = 0;
	CMHFile fp;

	// �ִ밳���� ��� ���� ó�� �д� �κ�
	sprintf(szFile, "./System/Resource/MonstermeterReward.bin");
	fp.Init(szFile, "rb");
	if(!fp.IsInited())
	{
		char szTmp[256];
		sprintf(szTmp, "%s ������ �������� �ʽ��ϴ�.", szFile);
		ASSERTMSG(0, szTmp);
		return FALSE;
	}

	while(TRUE)
	{
		if (fp.IsEOF()) break;
		fp.GetLine(szLine, sizeof(szLine));
		if (strstr(szLine, "//")) continue;			
		else if (strstr(szLine, "END_KIND")) 
		{
			nKind++;
			continue;
		}

		m_stReward[nKind].nNum++;
	}

	fp.Release();

	for(int i=0; i<MAX_REWARD_BEHAVE_TYPE; i++)
	{
		m_stReward[i].stRewardBase	= new stREWARD_BASE[m_stReward[i].nNum];
	}

	// �����͸� �б� ���� �� ��°..
	int nBehaveNum = 0, nValue[MAX_VALUE_NUM] = {0}, nRewardNum = 0;
	nKind = 0;
	fp.Init(szFile, "rb");
	if(!fp.IsInited())
	{
		return FALSE;
	}
	while(TRUE)
	{
		if (fp.IsEOF()) break;
		fp.GetLine(szLine, sizeof(szLine));
		if (strstr(szLine, "//")) continue;			
		else if (strstr(szLine, "END_KIND"))
		{
			nKind++;
			nRewardNum = 0;
			continue;
		}

		// ������ �б�
		sscanf(szLine, "%d %s %u %u", &nBehaveNum, szTxt, &nValue[0], &nValue[1]);
		if (strcmp(szTxt, "ITEM") == 0)	m_stReward[nKind].stRewardBase[nRewardNum].eRewardType = REWARD_TYPE_ITEM;
		else 
		{
			char szTmp[256];
			sprintf(szTmp, "�������� �ʴ� Ű�����Դϴ�.(Ű����:%s, ����:%s)", szTxt, szFile);
			ASSERTMSG(0, szTmp);
		}

		m_stReward[nKind].stRewardBase[nRewardNum].nBehaveNum = nBehaveNum;
		m_stReward[nKind].stRewardBase[nRewardNum].nValue[0] = nValue[0];
		m_stReward[nKind].stRewardBase[nRewardNum].nValue[1] = nValue[1];
		nRewardNum++;
	}
	
	fp.Release();

	return TRUE;
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ProcessReward Method																									   ���� ó��
//
VOID CSHMonstermeterManager::ProcessReward(CPlayer* pPlayer, REWARD_BEHAVE_TYPE eType, int nBehaveNum)
{
	// stRewardBase[].nBehaveNum �� ������� ������ ��� �������� ó�� �͸� ���޵ǰų�, �ߺ� ���޵ȴ�.(return ���Ͽ� ����)
	// �׷��� ������������ ó���Ͽ� ��ũ��Ʈ���� ���� �Ʒ� �׸� 1���� ���޵ǰ� �Ѵ�.
	// ����, ��ũ��Ʈ���� nBehaveNum �� ����(��������)��� �ۼ��ؾ� �Ѵ�.
	for(int i=m_stReward[eType].nNum-1; i>= 0; i--)
	{
		if ((nBehaveNum%m_stReward[eType].stRewardBase[i].nBehaveNum) == 0)
		{
			switch(m_stReward[eType].stRewardBase[i].eRewardType)
			{
				case REWARD_TYPE_ITEM:
					{
						CItemSlot* pSlot = pPlayer->GetSlot(eItemTable_Inventory);
						if(NULL == pSlot) ASSERTMSG(0,"�ùٸ� ������ ������ ���� �� �����ϴ�.");

						// 071211 KTH -- pPlayer �߰�
						DWORD nNum = ITEMMGR->GetTotalEmptySlotNum(pSlot, pPlayer);
						if(nNum == 0)
						{
							return;
						}

						int nItemID = m_stReward[eType].stRewardBase[i].nValue[0];
						int nItemNum = m_stReward[eType].stRewardBase[i].nValue[1];

						// ��ȿ �������ΰ�?
						// ..��ũ��Ʈ ������ �����̳� ��Ÿ����� ������ ��ȿ���� ���� �������̶�� ó������ �ʴ´�.
						ITEM_INFO* pstItem = ITEMMGR->GetItemInfo(nItemID);
						if (pstItem == NULL)
						{
							continue;
						}

						ITEMMGR->ObtainMonstermeterItem(pPlayer, nItemID, nItemNum);
					}
					break;
			}
			return;
		}
	}
}


/*********************************************************************

	 ����		: SHMarriageManager.cpp
	 �ۼ���		: hseos
	 �ۼ���		: 2007/12/10

	 ���ϼ���	: ��ȥ Ŭ������ �ҽ�

 *********************************************************************/

#include "stdafx.h"
#include "MHFile.h"
#include "SHMarriageManager.h"

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
	#include "ItemManager.h"
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
	#include "OBalloonName.h"
	#include "ObjectBalloon.h"
	#include "ItemManager.h"
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CSHMarriageManager
//
CSHMarriageManager g_csMarriageManager;

UINT CSHMarriageManager::DIVORCE_PENALTY_REMARRIAGE_TIME = 0;		// ��ȥ �� ��ȥ�� �ʿ��� �ð�
UINT CSHMarriageManager::TELEPORT_TO_PARTNER_FREQ = 0;				// ����� ��ġ�� �ڷ���Ʈ ���� ȸ��
UINT CSHMarriageManager::NICKNAME_CHARACTER_NUM = 0;				// ȣĪ ����(����Ʈ) ��

// -------------------------------------------------------------------------------------------------------------------------------------
// 																																  ������
//
CSHMarriageManager::CSHMarriageManager()
{
	ZeroMemory(&m_stMarriageNeedItem, sizeof(m_stMarriageNeedItem));
	ZeroMemory(&m_stMarriageTakeItem, sizeof(m_stMarriageTakeItem));

	LoadMarriageInfo();
}

// -------------------------------------------------------------------------------------------------------------------------------------
// 																																  �ı���
//
CSHMarriageManager::~CSHMarriageManager()
{
	SAFE_DELETE_ARRAY(m_stMarriageNeedItem.pstWearItemID);
	SAFE_DELETE_ARRAY(m_stMarriageTakeItem.pstItem);
}

// -------------------------------------------------------------------------------------------------------------------------------------
// 																															 ������ �б�
//
BOOL CSHMarriageManager::LoadMarriageInfo()
{
	char szLine[MAX_PATH], szFile[MAX_PATH];
	int	 nKind = 0, nCnt = 0;
	CMHFile fp;

	sprintf(szFile, "./System/Resource/Marriage.bin");
	fp.Init(szFile, "rb");
	if(!fp.IsInited()) return FALSE;
	// ���� üũ
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
		case 1:		m_stMarriageNeedItem.nWearItemNum++;		break;
		case 2:		m_stMarriageTakeItem.nItemNum++;			break;
		}
		
		nCnt++;
	}
	fp.Release();

	SAFE_DELETE_ARRAY(m_stMarriageNeedItem.pstWearItemID);
	m_stMarriageNeedItem.pstWearItemID = new DWORD[m_stMarriageNeedItem.nWearItemNum];
	ZeroMemory(m_stMarriageNeedItem.pstWearItemID, sizeof(m_stMarriageNeedItem.pstWearItemID[0])*m_stMarriageNeedItem.nWearItemNum);

	SAFE_DELETE_ARRAY(m_stMarriageTakeItem.pstItem);
	m_stMarriageTakeItem.pstItem = new MARRIAGE_ITEM[m_stMarriageTakeItem.nItemNum];
	ZeroMemory(m_stMarriageTakeItem.pstItem, sizeof(m_stMarriageTakeItem.pstItem[0])*m_stMarriageTakeItem.nItemNum);

	nKind = 0;
	nCnt = 0;
	fp.Init(szFile, "rb");
	if(!fp.IsInited()) return FALSE;
	// ������ �б�
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
		case 0:		sscanf(szLine, "%d %d %d",			&DIVORCE_PENALTY_REMARRIAGE_TIME,
														&TELEPORT_TO_PARTNER_FREQ,
														&NICKNAME_CHARACTER_NUM);						break;
		case 1:		sscanf(szLine, "%u",				&m_stMarriageNeedItem.pstWearItemID[nCnt]);		break;
		case 2:		sscanf(szLine, "%u %u",				&m_stMarriageTakeItem.pstItem[nCnt].nItemID,
														&m_stMarriageTakeItem.pstItem[nCnt].nItemNum);	break;
		}
		
		nCnt++;
	}
	fp.Release();

	// ������ ����ؼ� ���� �߸��Ǿ��� ��� �⺻���� 4 �� �Ѵ�.
	if (NICKNAME_CHARACTER_NUM == 0 || NICKNAME_CHARACTER_NUM > MAX_NAME_LENGTH) NICKNAME_CHARACTER_NUM = 4;

	return TRUE;
}

// -------------------------------------------------------------------------------------------------------------------------------------
// 																									 Ŭ���̾�Ʈ ��û �м�/ó��(������Ʈ)
//
VOID CSHMarriageManager::ASRV_ParseRequestFromClient(DWORD dwConnectionID, char* pMsg, DWORD dwLength)
{
#if defined(_AGENTSERVER)
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	
	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pTempMsg->dwObjectID);
	if (pUserInfo == NULL)
	{
		return;
	}
return;
//	switch( pTempMsg->Protocol )
//	{
//	default:
		g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, pMsg, dwLength );
//	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
// 																										   ���� ��û �м�/ó��(������Ʈ)
//
VOID CSHMarriageManager::ASRV_ParseRequestFromServer(DWORD dwConnectionID, char* pMsg, DWORD dwLength)
{
#if defined(_AGENTSERVER)
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	
	switch( pTempMsg->Protocol )
	{
	case MP_MARRIAGE_FORCE_DIVORCE_RESULT_TO_PARTNER_TO_OTHER_AGENT:
		{
			MSG_NAME_DWORD2* pPacket = (MSG_NAME_DWORD2*)pMsg;

			// ����ڰ� �ִ� ������Ʈ �ΰ�?
			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pPacket->dwData1);
			if (pUserInfo == NULL) return;

			// ���� ��ȥ�Ǿ����� �˸���.
			// ..������ ������� ID ����
			pUserInfo->nMarriageConnPartnerID = 0;
			// �ʼ����� ���� ���� ��ȥ�Ǿ����� �����Ѵ�.
			MSG_DWORD stPacket;

			stPacket.Category	= MP_MARRIAGE;
			stPacket.Protocol	= MP_MARRIAGE_FORCE_DIVORCE_RESULT_TO_PARTNER_TO_OTHER_AGENT;
			stPacket.dwObjectID	= pUserInfo->dwCharacterID;
			g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)&stPacket, sizeof(stPacket));
		}
		return;
	case MP_MARRIAGE_TELEPORT_TO_PARTNER_TO_OTHER_AGENT:
		{
			MSG_DWORD* pPacket = (MSG_DWORD*)pMsg;

			// ����ڰ� �ִ� ������Ʈ �ΰ�?
			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pPacket->dwObjectID);
			if (pUserInfo == NULL) return;
		}
		return;
	// ���������� ����ڿ��� �˷ȴ� ��, ����ڵ� ���� ������ �˷��Դ�.
	case MP_MARRIAGE_CONN_TO_PARTNER_TO_SENDER:
		{
			MSG_DWORD2* pPacket = (MSG_DWORD2*)pMsg;

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pPacket->dwData1);
			if (pUserInfo == NULL) return;

			// ������ ������� ID ����
			pUserInfo->nMarriageConnPartnerID = pPacket->dwData2;

			// Ŭ���̾�Ʈ�� �˸���
			MSG_DWORD2 stPacket;
			stPacket.Category	= MP_MARRIAGE;
			stPacket.Protocol	= MP_MARRIAGE_CONN_TO_PARTNER_TO_SENDER;
			stPacket.dwObjectID	= pUserInfo->dwCharacterID;

			g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
		}
		return;
	}

	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pTempMsg->dwObjectID);
	if (pUserInfo == NULL) return;


	switch( pTempMsg->Protocol )
	{
	// ���� �� ��ȥ ���� �ε�
	case MP_MARRIAGE_LOADINFO:
		{
			PACKET_MARRIAGE_INFO* pPacket = (PACKET_MARRIAGE_INFO*)pMsg;

			// ����ڿ��� �ڽ��� ������ �˸���. ���� ������Ʈ�� ���ٸ� �ٸ� ������Ʈ�� Broadcast
			USERINFO* pPartnerInfo = g_pUserTableForObjectID->FindUser(pPacket->nPartnerID);
			if (pPartnerInfo)
			{
				// ������ ������� ID ����
				pUserInfo->nMarriageConnPartnerID = pPartnerInfo->dwCharacterID;
				pPartnerInfo->nMarriageConnPartnerID = pUserInfo->dwCharacterID;

				// Ŭ���̾�Ʈ�� �˸���
				MSG_DWORD2 stPacket;
				stPacket.Category	= MP_MARRIAGE;
				stPacket.Protocol	= MP_MARRIAGE_CONN_TO_PARTNER_TO_SENDER;
				stPacket.dwObjectID	= pUserInfo->dwCharacterID;
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );

				stPacket.Category	= MP_MARRIAGE;
				stPacket.Protocol	= MP_MARRIAGE_CONN_TO_PARTNER;
				stPacket.dwObjectID	= pPartnerInfo->dwCharacterID;
				g_Network.Send2User( pPartnerInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
			}
			else
			{
				MSG_DWORD2 stPacket;
				stPacket.Category	= MP_MARRIAGE;
				stPacket.Protocol	= MP_MARRIAGE_CONN_TO_PARTNER;
				stPacket.dwObjectID	= pPacket->nPartnerID;
				stPacket.dwData1	= pUserInfo->dwCharacterID;
				stPacket.dwData2	= g_pServerSystem->GetServerNum();

				g_Network.Broadcast2AgentServerExceptSelf((char*)&stPacket, sizeof(stPacket));
			}

			// Ŭ���̾�Ʈ�� ���� ����. ���� �ٲٸ� �� ��.
			g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)pMsg, dwLength );
		}
		break;
	// ����ڰ� ����
	case MP_MARRIAGE_CONN_TO_PARTNER:
		{
			MSG_DWORD2* pPacket = (MSG_DWORD2*)pMsg;

			// ������ ������� ID ����
			pUserInfo->nMarriageConnPartnerID = pPacket->dwData1;

			// Ŭ���̾�Ʈ�� �˸���
			MSG_DWORD2 stPacket;
			stPacket.Category	= MP_MARRIAGE;
			stPacket.Protocol	= MP_MARRIAGE_CONN_TO_PARTNER;
			stPacket.dwObjectID	= pUserInfo->dwCharacterID;

			g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );

			// �ڽŵ� ���� ������ �˸���
			{
				MSG_DWORD2 stPacket;
				stPacket.Category	= MP_MARRIAGE;
				stPacket.Protocol	= MP_MARRIAGE_CONN_TO_PARTNER_TO_SENDER;
				stPacket.dwObjectID	= pPacket->dwData2;
				stPacket.dwData1	= pPacket->dwData1;
				stPacket.dwData2	= pUserInfo->dwCharacterID;

				g_Network.Send2SpecificAgentServer((char*)&stPacket, sizeof(stPacket));
			}
		}
		break;
	// ���� ��ȥ ��Ʈ�ʿ��� �˸���
	case MP_MARRIAGE_FORCE_DIVORCE_RESULT_TO_PARTNER:
		{
			// ����ڰ� ���� ��ȥ�� �߱⿡ ��Ʈ�ʿ��� �̸� �˸���.
			// ..����ڰ� ���� ������Ʈ�� ���ٸ� BroadCast
			if (pUserInfo->nMarriageConnPartnerID == 0) return;

			USERINFO* pPartnerInfo = g_pUserTableForObjectID->FindUser(pUserInfo->nMarriageConnPartnerID);
			if (pPartnerInfo)
			{
				// ������ ������� ID ����
				pPartnerInfo->nMarriageConnPartnerID = 0;

				// �ʼ����� ���� ���� ��ȥ�Ǿ����� �����Ѵ�.
				MSG_DWORD stPacket;

				stPacket.Category	= MP_MARRIAGE;
				stPacket.Protocol	= MP_MARRIAGE_FORCE_DIVORCE_RESULT_TO_PARTNER;
				stPacket.dwObjectID	= pPartnerInfo->dwCharacterID;
				g_Network.Send2Server( pPartnerInfo->dwMapServerConnectionIndex, (char*)&stPacket, sizeof(stPacket));
			}
			else
			{
				pTempMsg->Protocol = MP_MARRIAGE_FORCE_DIVORCE_RESULT_TO_PARTNER_TO_OTHER_AGENT;
				g_Network.Broadcast2AgentServerExceptSelf((char*)pMsg, dwLength);
			}
			// ������ ������� ID ����
			pUserInfo->nMarriageConnPartnerID = 0;
		}
		break;
	// ����� ��ġ�� �ڷ���Ʈ
	case MP_MARRIAGE_TELEPORT_TO_PARTNER:
		{
			MSG_DWORD* pPacket = (MSG_DWORD*)pMsg;

			if (pPacket->dwData == RESULT_OK)
			{
				// �ڷ���Ʈ ������ �����ϸ� ����ڸ� ã�� ���� ������Ʈ�� BroadCast
				pPacket->Protocol = MP_MARRIAGE_TELEPORT_TO_PARTNER_TO_OTHER_AGENT;
				g_Network.Broadcast2AgentServer((char*)pMsg, dwLength);
			}
			else
			{
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)pMsg, dwLength );
			}
		}
		break;
	default:
		g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)pMsg, dwLength );
	}
#endif
}


// -------------------------------------------------------------------------------------------------------------------------------------
// 																												  ����� ��ġ�� �ڷ���Ʈ
//
VOID CSHMarriageManager::ASRV_TeleportToPartner(USERINFO* pUserInfo)
{
}


// -------------------------------------------------------------------------------------------------------------------------------------
// 																											   Ŭ���̾�Ʈ ��û �м�/ó��
//
VOID CSHMarriageManager::SRV_ParseRequestFromClient(DWORD dwConnectionID, char* pMsg, DWORD dwLength)
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
	// ��ȥ
	case MP_MARRIAGE_MARRIAGE_REQUEST:
		{
			MSG_DWORD* pPacket = (MSG_DWORD*)pMsg;
			SRV_Marriage(pPlayer, pPacket->dwData);
		}
		break;
	// ��ȥ ���
	case MP_MARRIAGE_MARRIAGE_REQUESTED:
		{
			MSG_DWORD* pPacket = (MSG_DWORD*)pMsg;
			SRV_MarriageResult(pPlayer, pPacket->dwData);
		}
		break;
	// ���� ��ȥ
	case MP_MARRIAGE_AGREE_DIVORCE_REQUEST:
		{
			MSG_DWORD* pPacket = (MSG_DWORD*)pMsg;
			SRV_AgreeDivorce(pPlayer, pPacket->dwData);
		}
		break;
	// ��ȥ ���
	case MP_MARRIAGE_AGREE_DIVORCE_REQUESTED:
		{
			MSG_DWORD* pPacket = (MSG_DWORD*)pMsg;
			SRV_AgreeDivorceResult(pPlayer, pPacket->dwData);
		}
		break;
	// ���� ��ȥ
	case MP_MARRIAGE_FORCE_DIVORCE_REQUEST:
		{
			MSG_DWORD* pPacket = (MSG_DWORD*)pMsg;
			SRV_ForceDivorce(pPlayer);
		}
		break;
	// ���� ��ȥ ����
	case MP_MARRIAGE_FORCE_DIVORCE_RESULT_TO_PARTNER:
	case MP_MARRIAGE_FORCE_DIVORCE_RESULT_TO_PARTNER_TO_OTHER_AGENT:
		{
			MSG_DWORD* pPacket = (MSG_DWORD*)pMsg;
			SRV_ForceDivorced(pPlayer);
		}
		break;
	// ȣĪ
	case MP_MARRIAGE_NICKNAME:
		{
			MSG_NAME* pPacket = (MSG_NAME*)pMsg;
			SRV_ChangeNickname(pPlayer, pPacket->Name);
		}
		break;
	// ����� ��ġ�� �ڷ���Ʈ
	case MP_MARRIAGE_TELEPORT_TO_PARTNER:
		{
			MSG_DWORD* pPacket = (MSG_DWORD*)pMsg;
			SRV_TeleportToPartner(pPlayer);
		}
		break;
	}
#endif
}


// -------------------------------------------------------------------------------------------------------------------------------------
// 																																	��ȥ
//
VOID CSHMarriageManager::SRV_Marriage(CPlayer* pPlayer, DWORD nPartnerID)
{
#if defined(_MAPSERVER_)
	RESULT	eResult		= RESULT_OK;
	DWORD	nValue		= 0;

	CPlayer* pPartner = (CPlayer*)g_pUserTable->FindUser( nPartnerID );
	if( !pPartner ) return;

	// �̹� ��ȥ�� �߳�?
	if (pPlayer->IsMarried())
	{
		eResult = RESULT_FAIL_01;
		goto FAILED;
	}

	// ������ �̹� ��ȥ�� �߳�?
	if (pPartner->IsMarried())
	{
		eResult = RESULT_FAIL_02;
		goto FAILED;
	}

	// ���� üũ
	if (pPlayer->GetGender() == pPartner->GetGender())
	{
		eResult = RESULT_FAIL_03;
		goto FAILED;
	}

	// ��ȥ ��û ���̰ų� ��û�� ���� �����ΰ�?
	if (pPlayer->GetMarriageRequestPartnerID())
	{
		eResult = RESULT_FAIL_04;
		goto FAILED;
	}

	// ������ ��ȥ ��û ���̰ų� ��û�� ���� �����ΰ�?
	if (pPartner->GetMarriageRequestPartnerID())
	{
		eResult = RESULT_FAIL_05;
		goto FAILED;
	}

	// ��ȥ �� �г�Ƽ �ð��� ������?
	if (strcmp(pPlayer->GetMarriageDivorceDate(), "") != 0)
	{
		nValue = GetPastTime(pPlayer->GetMarriageDivorceDate());
		if (nValue < DIVORCE_PENALTY_REMARRIAGE_TIME)
		{
			eResult = RESULT_FAIL_06;
			goto FAILED;
		}
	}
	
	// ������ ��ȥ �� �г�Ƽ �ð��� ������?
	if (strcmp(pPartner->GetMarriageDivorceDate(), "") != 0)
	{
		nValue = GetPastTime(pPartner->GetMarriageDivorceDate());
		if (nValue < DIVORCE_PENALTY_REMARRIAGE_TIME)
		{
			nValue = GetPastTime(pPartner->GetMarriageDivorceDate());
			eResult = RESULT_FAIL_07;
			goto FAILED;
		}
	}
	
	// ��ȥ �������� �ֳ�?
	for(UINT i=0; i<m_stMarriageNeedItem.nWearItemNum; i++)
	{
		BOOL bHave = FALSE;
		for(int j=0; j<eWearedItem_Max; j++)
		{
			if (pPlayer->GetWearedItemIdx(j) == m_stMarriageNeedItem.pstWearItemID[i])
			{
				bHave = TRUE;
				break;
			}
		}

		if (bHave == FALSE)
		{
			eResult = RESULT_FAIL_08;
			nValue = m_stMarriageNeedItem.pstWearItemID[i];
			goto FAILED;
		}
	}	

	// ���濡�� ��ȥ �������� �ֳ�?
	for(UINT i=0; i<m_stMarriageNeedItem.nWearItemNum; i++)
	{
		BOOL bHave = FALSE;
		for(int j=0; j<eWearedItem_Max; j++)
		{
			if (pPartner->GetWearedItemIdx(j) == m_stMarriageNeedItem.pstWearItemID[i])
			{
				bHave = TRUE;
				break;
			}
		}

		if (bHave == FALSE)
		{
			eResult = RESULT_FAIL_09;
			nValue = m_stMarriageNeedItem.pstWearItemID[i];
			goto FAILED;
		}
	}	

	// ������ ���� üũ
	// ..�������� ���������� üũ�ؼ� �ʿ� ���� ������ ���Ѵ�.
	DWORD nNeedNum = 0;
	for(UINT i=0; i<	m_stMarriageTakeItem.nItemNum; i++)
	{
		if (ITEMMGR->IsDupItem(m_stMarriageTakeItem.pstItem[i].nItemID))
		{
			nNeedNum++;
		}
		else
		{
			nNeedNum += m_stMarriageTakeItem.pstItem[i].nItemNum;
		}
	}

	// ..�ʿ� ���� �������� ������ ����
	CItemSlot* pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	if(NULL == pSlot)
	{
		return;
	}

	DWORD nNum = ITEMMGR->GetTotalEmptySlotNum(pSlot, pPlayer);
	if(nNum < nNeedNum)
	{
		eResult = RESULT_FAIL_10;
		nValue = nNeedNum;
		goto FAILED;
	}

	// ..�ʿ� ���� �������� ������ ����. ���� üũ
	pSlot = pPartner->GetSlot(eItemTable_Inventory);
	if(NULL == pSlot)
	{
		return;
	}

	nNum = ITEMMGR->GetTotalEmptySlotNum(pSlot, pPartner);
	if(nNum < nNeedNum)
	{
		eResult = RESULT_FAIL_11;
		nValue = nNeedNum;
		goto FAILED;
	}

FAILED:
	MSG_NAME_DWORD2 stPacket;

	switch(eResult)
	{
	case RESULT_OK:
		{
			// ��ȥ ��û ���� ����
			pPlayer->SetMarriageRequestPartnerID(pPartner->GetID());
			pPartner->SetMarriageRequestPartnerID(pPlayer->GetID());

			// ��û�Ǿ����� �˸���.
			stPacket.Category	= MP_MARRIAGE;
			stPacket.Protocol	= MP_MARRIAGE_MARRIAGE_REQUEST;
			stPacket.dwObjectID	= pPlayer->GetID();
			stPacket.dwData1	= eResult;
			SafeStrCpy(stPacket.Name, pPartner->GetObjectName(), MAX_NAME_LENGTH+1);
			pPlayer->SendMsg(&stPacket, sizeof(stPacket));

			// ���濡�� ��ȥ ��û�� �˸���.
			stPacket.Category	= MP_MARRIAGE;
			stPacket.Protocol	= MP_MARRIAGE_MARRIAGE_REQUESTED;
			stPacket.dwObjectID	= pPartner->GetID();
			stPacket.dwData1	= eResult;
			SafeStrCpy(stPacket.Name, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);
			pPartner->SendMsg(&stPacket, sizeof(stPacket));
		}
		break;
	default:
		{
			// Ŭ���̾�Ʈ�� ���� �˸���
			stPacket.Category	= MP_MARRIAGE;
			stPacket.Protocol	= MP_MARRIAGE_MARRIAGE_REQUEST;
			stPacket.dwObjectID	= pPlayer->GetID();
			stPacket.dwData1	= eResult;
			stPacket.dwData2	= nValue;
			pPlayer->SendMsg(&stPacket, sizeof(stPacket));
		}
		break;
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
// 																															   ��ȥ ���
//
VOID CSHMarriageManager::SRV_MarriageResult(CPlayer* pPlayer, BOOL bAccept)
{
#if defined(_MAPSERVER_)
	UINT nValue = 0;

	// ��ȥ ��û�� ����� �ֳ�?
	if (pPlayer->GetMarriageRequestPartnerID() == FALSE)
	{
		return;
	}
	CPlayer* pPartner = (CPlayer*)g_pUserTable->FindUser( pPlayer->GetMarriageRequestPartnerID() );
	if( !pPartner ) return;

	// �̹� ��ȥ�� �߳�?
	if (pPlayer->IsMarried())
	{
		return;
	}

	// ������ �̹� ��ȥ�� �߳�?
	if (pPartner->IsMarried())
	{
		return;
	}

	// ��ȥ �� �г�Ƽ �ð��� ������?
	if (strcmp(pPlayer->GetMarriageDivorceDate(), "") != 0)
	{
		nValue = GetPastTime(pPlayer->GetMarriageDivorceDate());
		if (nValue < DIVORCE_PENALTY_REMARRIAGE_TIME)
		{
			return;
		}
	}
	
	// ������ ��ȥ �� �г�Ƽ �ð��� ������?
	if (strcmp(pPartner->GetMarriageDivorceDate(), "") != 0)
	{
		nValue = GetPastTime(pPartner->GetMarriageDivorceDate());
		if (nValue < DIVORCE_PENALTY_REMARRIAGE_TIME)
		{
			return;
		}
	}

	// ��ȥ �������� �ֳ�?
	for(UINT i=0; i<m_stMarriageNeedItem.nWearItemNum; i++)
	{
		BOOL bHave = FALSE;
		for(int j=0; j<eWearedItem_Max; j++)
		{
			if (pPlayer->GetWearedItemIdx(j) == m_stMarriageNeedItem.pstWearItemID[i])
			{
				bHave = TRUE;
				break;
			}
		}

		if (bHave == FALSE)
		{
			return;
		}
	}	

	// ���濡�� ��ȥ �������� �ֳ�?
	for(UINT i=0; i<m_stMarriageNeedItem.nWearItemNum; i++)
	{
		BOOL bHave = FALSE;
		for(int j=0; j<eWearedItem_Max; j++)
		{
			if (pPartner->GetWearedItemIdx(j) == m_stMarriageNeedItem.pstWearItemID[i])
			{
				bHave = TRUE;
				break;
			}
		}

		if (bHave == FALSE)
		{
			return;
		}
	}	

	// ������ ���� üũ
	// ..�������� ���������� üũ�ؼ� �ʿ� ���� ������ ���Ѵ�.
	DWORD nNeedNum = 0;
	for(UINT i=0; i<	m_stMarriageTakeItem.nItemNum; i++)
	{
		if (ITEMMGR->IsDupItem(m_stMarriageTakeItem.pstItem[i].nItemID))
		{
			nNeedNum++;
		}
		else
		{
			nNeedNum += m_stMarriageTakeItem.pstItem[i].nItemNum;
		}
	}

	// ..�ʿ� ���� �������� ������ ����
	CItemSlot* pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	if(NULL == pSlot)
	{
		return;
	}

	DWORD nNum = ITEMMGR->GetTotalEmptySlotNum(pSlot, pPlayer);
	if(nNum < nNeedNum)
	{
		return;
	}

	// ..�ʿ� ���� �������� ������ ����. ���� üũ
	pSlot = pPartner->GetSlot(eItemTable_Inventory);
	if(NULL == pSlot)
	{
		return;
	}

	nNum = ITEMMGR->GetTotalEmptySlotNum(pSlot, pPartner);
	if(nNum < nNeedNum)
	{
		return;
	}

	MSG_DWORD_NAME stPacket;

	// ��ȥ ó��
	if (bAccept)
	{
		// ��ȥ ����
		// ..���� ����
		pPlayer->SetMarriageRequestPartnerID(0);
		pPlayer->SetMarriagePartnerID(pPartner->GetID());
		pPlayer->SetMarriagePartnerName(pPartner->GetObjectName());
		pPlayer->SetMarriageTeleportFreq(TELEPORT_TO_PARTNER_FREQ);

		pPartner->SetMarriageRequestPartnerID(0);
		pPartner->SetMarriagePartnerID(pPlayer->GetID());
		pPartner->SetMarriagePartnerName(pPlayer->GetObjectName());
		pPartner->SetMarriageRequestPartnerID(TELEPORT_TO_PARTNER_FREQ);
		// ..DB�� ����
//		Marriage_Save(pPlayer->GetID(), pPartner->GetID(), TELEPORT_TO_PARTNER_FREQ);
//		Marriage_Save(pPartner->GetID(), pPlayer->GetID(), TELEPORT_TO_PARTNER_FREQ);

		// ..Ŭ���̾�Ʈ�� �˸���
		stPacket.Category	= MP_MARRIAGE;
		stPacket.Protocol	= MP_MARRIAGE_MARRIAGE_RESULT;
		stPacket.dwObjectID	= pPlayer->GetID();
		stPacket.dwData		= RESULT_OK;
		SafeStrCpy(stPacket.Name, pPartner->GetObjectName(), MAX_NAME_LENGTH+1);
		/*
		pPlayer->SendMsg(&stPacket, sizeof(stPacket));
		PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pPlayer, &stPacket, sizeof(stPacket));
		*/
		PACKEDDATA_OBJ->QuickSend(pPlayer, &stPacket, sizeof(stPacket));

		stPacket.Category	= MP_MARRIAGE;
		stPacket.Protocol	= MP_MARRIAGE_MARRIAGE_RESULT;
		stPacket.dwObjectID	= pPartner->GetID();
		stPacket.dwData		= RESULT_OK;
		SafeStrCpy(stPacket.Name, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);
		/*
		pPartner->SendMsg(&stPacket, sizeof(stPacket));
		PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pPartner, &stPacket, sizeof(stPacket));
		*/
		PACKEDDATA_OBJ->QuickSend(pPartner, &stPacket, sizeof(stPacket));

		// ..������ ����
		for(UINT i=0; i<m_stMarriageTakeItem.nItemNum; i++)
		{
			ITEMMGR->ObtainGeneralItem(pPlayer, m_stMarriageTakeItem.pstItem[i].nItemID, m_stMarriageTakeItem.pstItem[i].nItemNum, eLog_ItemObtainMarriage, MP_ITEM_MARRIAGE_ACK);
			ITEMMGR->ObtainGeneralItem(pPartner, m_stMarriageTakeItem.pstItem[i].nItemID, m_stMarriageTakeItem.pstItem[i].nItemNum, eLog_ItemObtainMarriage, MP_ITEM_MARRIAGE_ACK);
		}
	}
	else
	{
		// ��ȥ ����
		// ..���� ����
		pPlayer->SetMarriageRequestPartnerID(0);
		pPartner->SetMarriageRequestPartnerID(0);

		stPacket.Category	= MP_MARRIAGE;
		stPacket.Protocol	= MP_MARRIAGE_MARRIAGE_RESULT;
		stPacket.dwObjectID	= pPlayer->GetID();
		stPacket.dwData		= RESULT_FAIL_01;
		SafeStrCpy(stPacket.Name, pPartner->GetObjectName(), MAX_NAME_LENGTH+1);
		pPlayer->SendMsg(&stPacket, sizeof(stPacket));

		// ��ȥ ��������
		stPacket.Category	= MP_MARRIAGE;
		stPacket.Protocol	= MP_MARRIAGE_MARRIAGE_RESULT;
		stPacket.dwObjectID	= pPartner->GetID();
		stPacket.dwData		= RESULT_FAIL_02;
		SafeStrCpy(stPacket.Name, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);
		pPartner->SendMsg(&stPacket, sizeof(stPacket));
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
// 																															   ���� ��ȥ
//
VOID CSHMarriageManager::SRV_AgreeDivorce(CPlayer* pPlayer, DWORD nPartnerID)
{
#if defined(_MAPSERVER_)
	RESULT	eResult		= RESULT_OK;
	DWORD	nValue		= 0;

	// ��ȥ�� �߳�?
	if (pPlayer->IsMarried() == FALSE)
	{
		eResult = RESULT_FAIL_01;
		goto FAILED;
	}

	// �̹� ��ȥ ��û ���ΰ�?
	if (pPlayer->GetMarriageRequestPartnerID())
	{
		eResult = RESULT_FAIL_02;
		goto FAILED;
	}

	// ����ڿ� ������ �ֳ�?
	CPlayer* pPartner = (CPlayer*)g_pUserTable->FindUser( pPlayer->GetMarriagePartnerID() );
	if( !pPartner )
	{
		eResult = RESULT_FAIL_03;
		goto FAILED;
	}

	// �Ÿ�üũ
	VECTOR3 stPlayerPos, stPartnerPos;
	pPlayer->GetPosition(&stPlayerPos);
	pPartner->GetPosition(&stPartnerPos);
	float nDistance = CalcDistanceXZ(&stPlayerPos, &stPartnerPos); 
	if (nDistance > 500)
	{
		eResult = RESULT_FAIL_03;
		goto FAILED;
	}

	// ����ڰ� �̹� ��ȥ ��û ���ΰ�?
	if (pPartner->GetMarriageRequestPartnerID())
	{
		eResult = RESULT_FAIL_04;
		goto FAILED;
	}


FAILED:
	MSG_NAME_DWORD2 stPacket;

	switch(eResult)
	{
	case RESULT_OK:
		{
			// ��ȥ ��û ���� ����
			pPlayer->SetMarriageRequestPartnerID(pPartner->GetID());
			pPartner->SetMarriageRequestPartnerID(pPlayer->GetID());

			// ��û�Ǿ����� �˸���.
			stPacket.Category	= MP_MARRIAGE;
			stPacket.Protocol	= MP_MARRIAGE_AGREE_DIVORCE_REQUEST;
			stPacket.dwObjectID	= pPlayer->GetID();
			stPacket.dwData1	= eResult;
			SafeStrCpy(stPacket.Name, pPartner->GetObjectName(), MAX_NAME_LENGTH+1);
			pPlayer->SendMsg(&stPacket, sizeof(stPacket));

			// ���濡�� ��ȥ ��û�� �˸���.
			stPacket.Category	= MP_MARRIAGE;
			stPacket.Protocol	= MP_MARRIAGE_AGREE_DIVORCE_REQUESTED;
			stPacket.dwObjectID	= pPartner->GetID();
			stPacket.dwData1	= eResult;
			SafeStrCpy(stPacket.Name, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);
			pPartner->SendMsg(&stPacket, sizeof(stPacket));
		}
		break;
	default:
		{
			// Ŭ���̾�Ʈ�� ���� �˸���
			stPacket.Category	= MP_MARRIAGE;
			stPacket.Protocol	= MP_MARRIAGE_AGREE_DIVORCE_REQUEST;
			stPacket.dwObjectID	= pPlayer->GetID();
			stPacket.dwData1	= eResult;
			stPacket.dwData2	= nValue;
			pPlayer->SendMsg(&stPacket, sizeof(stPacket));
		}
		break;
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
// 																														  ���� ��ȥ ���
//
VOID CSHMarriageManager::SRV_AgreeDivorceResult(CPlayer* pPlayer, BOOL bAccept)
{
#if defined(_MAPSERVER_)
	RESULT	eResult		= RESULT_OK;
	DWORD	nValue		= 0;

	// ��ȥ�� �߳�?
	if (pPlayer->IsMarried() == FALSE)
	{
		eResult = RESULT_FAIL_01;
		goto FAILED;
	}

	// ��ȥ ��û�� �޾ҳ�?
	if (pPlayer->GetMarriageRequestPartnerID() == FALSE)
	{
		eResult = RESULT_FAIL_02;
		goto FAILED;
	}

	// ����ڿ� ������ �ֳ�?
	CPlayer* pPartner = (CPlayer*)g_pUserTable->FindUser( pPlayer->GetMarriagePartnerID() );
	if( !pPartner )
	{
		eResult = RESULT_FAIL_03;
		goto FAILED;
	}

	// �Ÿ�üũ
	// ..���� �Ÿ��� üũ. �������� Ŭ���̾�Ʈ�� ��쿡 ��ȥ ��û�ڴ� NPC�� ������ ���� �״�..�ϴ� ���� �Ÿ��� üũ(�ð��Ǹ� NPCüũ�� ����..^^)
	VECTOR3 stPlayerPos, stPartnerPos;
	pPlayer->GetPosition(&stPlayerPos);
	pPartner->GetPosition(&stPartnerPos);
	float nDistance = CalcDistanceXZ(&stPlayerPos, &stPartnerPos); 
	if (nDistance > 500)
	{
		eResult = RESULT_FAIL_03;
		goto FAILED;
	}

	// ����ڰ� ��ȥ ��û ���ΰ�?
	if (pPartner->GetMarriageRequestPartnerID() == FALSE)
	{
		eResult = RESULT_FAIL_04;
		goto FAILED;
	}

FAILED:
	MSG_DWORD_NAME stPacket;

	switch(eResult)
	{
	case RESULT_OK:
		{
			if (bAccept)
			{
				char szTxt[256] = {0,};
				// ��ȥ ����
				// ..���� ����
				pPlayer->SetMarriagePartnerID(0);
				pPlayer->SetMarriageRequestPartnerID(0);
				pPlayer->SetMarriageTeleportFreq(0);
				pPlayer->SetMarriagePartnerName(szTxt);
				pPlayer->SetMarriagePartnerNickName(szTxt);

				pPartner->SetMarriagePartnerID(0);
				pPartner->SetMarriageRequestPartnerID(0);
				pPartner->SetMarriageTeleportFreq(0);
				pPartner->SetMarriagePartnerName(szTxt);
				pPartner->SetMarriagePartnerNickName(szTxt);
				// ��ȥ ����
				sprintf(szTxt, "%04d-%02d-%02d %02d:%02d", MHTIMEMGR_OBJ->GetCurLocalTime()->wYear, 
														  MHTIMEMGR_OBJ->GetCurLocalTime()->wMonth,
														  MHTIMEMGR_OBJ->GetCurLocalTime()->wDay,
														  MHTIMEMGR_OBJ->GetCurLocalTime()->wHour,
														  MHTIMEMGR_OBJ->GetCurLocalTime()->wMinute);
				pPlayer->SetMarriageDivorceDate(szTxt);
				pPartner->SetMarriageDivorceDate(szTxt);

				// DB�� ����
//				Marriage_Divorce_Save(pPlayer->GetID());
//				Marriage_Divorce_Save(pPartner->GetID());

				// ..Ŭ���̾�Ʈ�� �˸���
				stPacket.Category	= MP_MARRIAGE;
				stPacket.Protocol	= MP_MARRIAGE_AGREE_DIVORCE_RESULT;
				stPacket.dwObjectID	= pPlayer->GetID();
				stPacket.dwData		= RESULT_OK;
				SafeStrCpy(stPacket.Name, pPartner->GetObjectName(), MAX_NAME_LENGTH+1);
				/*
				pPlayer->SendMsg(&stPacket, sizeof(stPacket));
				PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pPlayer, &stPacket, sizeof(stPacket));
				*/
				PACKEDDATA_OBJ->QuickSend(pPlayer, &stPacket, sizeof(stPacket));

				stPacket.Category	= MP_MARRIAGE;
				stPacket.Protocol	= MP_MARRIAGE_AGREE_DIVORCE_RESULT;
				stPacket.dwObjectID	= pPartner->GetID();
				stPacket.dwData		= RESULT_OK;
				SafeStrCpy(stPacket.Name, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);
				/*
				pPartner->SendMsg(&stPacket, sizeof(stPacket));
				PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pPartner, &stPacket, sizeof(stPacket));
				*/
				PACKEDDATA_OBJ->QuickSend(pPartner, &stPacket, sizeof(stPacket));
			}
			else
			{
				// ��ȥ ����
				// ..���� ����
				pPlayer->SetMarriageRequestPartnerID(0);
				pPartner->SetMarriageRequestPartnerID(0);

				stPacket.Category	= MP_MARRIAGE;
				stPacket.Protocol	= MP_MARRIAGE_AGREE_DIVORCE_RESULT;
				stPacket.dwObjectID	= pPlayer->GetID();
				stPacket.dwData		= RESULT_FAIL_01;
				SafeStrCpy(stPacket.Name, pPartner->GetObjectName(), MAX_NAME_LENGTH+1);
				pPlayer->SendMsg(&stPacket, sizeof(stPacket));

				// ��ȥ ��������
				stPacket.Category	= MP_MARRIAGE;
				stPacket.Protocol	= MP_MARRIAGE_AGREE_DIVORCE_RESULT;
				stPacket.dwObjectID	= pPartner->GetID();
				stPacket.dwData		= RESULT_FAIL_02;
				SafeStrCpy(stPacket.Name, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);
				pPartner->SendMsg(&stPacket, sizeof(stPacket));
			}
		}
		break;
	default:
		{
			// Ŭ���̾�Ʈ�� ���� �˸���
			stPacket.Category	= MP_MARRIAGE;
			stPacket.Protocol	= MP_MARRIAGE_AGREE_DIVORCE_REQUEST;
			stPacket.dwObjectID	= pPlayer->GetID();
			stPacket.dwData		= eResult;
			pPlayer->SendMsg(&stPacket, sizeof(stPacket));
		}
		break;
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
// 																															   ���� ��ȥ
//
VOID CSHMarriageManager::SRV_ForceDivorce(CPlayer* pPlayer)
{
#if defined(_MAPSERVER_)
	RESULT	eResult		= RESULT_OK;
	DWORD	nValue		= 0;

	// ��ȥ�� �߳�?
	if (pPlayer->IsMarried() == FALSE)
	{
		eResult = RESULT_FAIL_01;
		goto FAILED;
	}

FAILED:
	MSG_NAME_DWORD2 stPacket;

	switch(eResult)
	{
	case RESULT_OK:
		{
			DWORD nPartnerID = pPlayer->GetMarriagePartnerID();
			char szTxt[256] = {0,};
			// ��ȥ ó��
			// ..���� ����
			pPlayer->SetMarriagePartnerID(0);
			pPlayer->SetMarriageRequestPartnerID(0);
			pPlayer->SetMarriageTeleportFreq(0);
			pPlayer->SetMarriagePartnerName(szTxt);
			pPlayer->SetMarriagePartnerNickName(szTxt);
			sprintf(szTxt, "%04d-%02d-%02d %02d:%02d", MHTIMEMGR_OBJ->GetCurLocalTime()->wYear, 
														MHTIMEMGR_OBJ->GetCurLocalTime()->wMonth,
														MHTIMEMGR_OBJ->GetCurLocalTime()->wDay,
														MHTIMEMGR_OBJ->GetCurLocalTime()->wHour,
														MHTIMEMGR_OBJ->GetCurLocalTime()->wMinute);
			pPlayer->SetMarriageDivorceDate(szTxt);

			// DB�� ����
			// ..����ڵ� ���⼭ ������ ������. ����ڰ� ���� ���� ������ ���� ��� ������
			// ..���⼭ �����ϰ�, ����ڿ��� ���� ��ȥ�� �˸��� �κп����� �� �����Ѵ�.
			// ..�̷��� �� �ϸ� ���� �� ������� ��ȥ ������ �ִ� �� üũ�ؼ� ������ ���� ��ȥ���� �����ϰ�
			// ..��ȥ ó���� �� ��� �Ѵ�. ����...
//			Marriage_Divorce_Save(pPlayer->GetID());
//			Marriage_Divorce_Save(nPartnerID);

			// ..Ŭ���̾�Ʈ�� �˸���
			stPacket.Category	= MP_MARRIAGE;
			stPacket.Protocol	= MP_MARRIAGE_FORCE_DIVORCE_RESULT;
			stPacket.dwObjectID	= pPlayer->GetID();
			stPacket.dwData1	= RESULT_OK;
			SafeStrCpy(stPacket.Name, pPlayer->GetMarriagePartnerName(), MAX_NAME_LENGTH+1);
			/*
			pPlayer->SendMsg(&stPacket, sizeof(stPacket));
			PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pPlayer, &stPacket, sizeof(stPacket));
			*/
			PACKEDDATA_OBJ->QuickSend(pPlayer, &stPacket, sizeof(stPacket));
			// ..����ڿ��Ե� �˷��� �ϴµ� ����ִ��� �𸣹Ƿ� ������Ʈ���� BroadCasting �Ѵ�
			stPacket.Category	= MP_MARRIAGE;
			stPacket.Protocol	= MP_MARRIAGE_FORCE_DIVORCE_RESULT_TO_PARTNER;
			stPacket.dwObjectID	= pPlayer->GetID();
			stPacket.dwData1	= nPartnerID;
			SafeStrCpy(stPacket.Name, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);
			pPlayer->SendMsg(&stPacket, sizeof(stPacket));
		}
		break;
	default:
		{
			// Ŭ���̾�Ʈ�� ���� �˸���
			stPacket.Category	= MP_MARRIAGE;
			stPacket.Protocol	= MP_MARRIAGE_FORCE_DIVORCE_REQUEST;
			stPacket.dwObjectID	= pPlayer->GetID();
			stPacket.dwData1	= eResult;
			pPlayer->SendMsg(&stPacket, sizeof(stPacket));
		}
		break;
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
// 																														   ���� ��ȥ ����
//
VOID CSHMarriageManager::SRV_ForceDivorced(CPlayer* pPlayer)
{
#if defined(_MAPSERVER_)
	RESULT	eResult		= RESULT_OK;
	DWORD	nValue		= 0;

	// ��ȥ�� �߳�?
	if (pPlayer->IsMarried() == FALSE)
	{
		eResult = RESULT_FAIL_01;
		goto FAILED;
	}

FAILED:
	MSG_NAME_DWORD2 stPacket;

	switch(eResult)
	{
	case RESULT_OK:
		{
			DWORD nPartnerID = pPlayer->GetMarriagePartnerID();
			char szTxt[256] = {0,};
			// ��ȥ ó��
			// ..���� ����
			pPlayer->SetMarriagePartnerID(0);
			pPlayer->SetMarriageRequestPartnerID(0);
			pPlayer->SetMarriageTeleportFreq(0);
			pPlayer->SetMarriagePartnerName(szTxt);
			pPlayer->SetMarriagePartnerNickName(szTxt);
			sprintf(szTxt, "%04d-%02d-%02d %02d:%02d", MHTIMEMGR_OBJ->GetCurLocalTime()->wYear, 
														MHTIMEMGR_OBJ->GetCurLocalTime()->wMonth,
														MHTIMEMGR_OBJ->GetCurLocalTime()->wDay,
														MHTIMEMGR_OBJ->GetCurLocalTime()->wHour,
														MHTIMEMGR_OBJ->GetCurLocalTime()->wMinute);
			pPlayer->SetMarriageDivorceDate(szTxt);

			// DB�� ����
//			Marriage_Divorce_Save(pPlayer->GetID());

			// ..Ŭ���̾�Ʈ�� �˸���
			stPacket.Category	= MP_MARRIAGE;
			stPacket.Protocol	= MP_MARRIAGE_FORCE_DIVORCE_RESULT_TO_PARTNER_TO_CLIENT;
			stPacket.dwObjectID	= pPlayer->GetID();
			stPacket.dwData1	= RESULT_OK;
			SafeStrCpy(stPacket.Name, pPlayer->GetMarriagePartnerName(), MAX_NAME_LENGTH+1);
			/*
			pPlayer->SendMsg(&stPacket, sizeof(stPacket));
			PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pPlayer, &stPacket, sizeof(stPacket));
			*/
			PACKEDDATA_OBJ->QuickSend(pPlayer, &stPacket, sizeof(stPacket));
		}
		break;
	default:
		{
		}
		break;
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
// 																												  ����� ��ġ�� �ڷ���Ʈ
//
VOID CSHMarriageManager::SRV_TeleportToPartner(CPlayer* pPlayer)
{
#if defined(_MAPSERVER_)
	RESULT eResult = RESULT_OK;

	// ��ȥ�� �߳�?
	if (pPlayer->IsMarried() == FALSE)
	{
		eResult = RESULT_FAIL_01;
		goto FAILED;
	}

	//  �ڷ���Ʈ Ƚ���� �ֳ�?
	if (pPlayer->GetMarriageTeleportFreq() == 0)
	{
		eResult = RESULT_FAIL_02;
		goto FAILED;
	}

FAILED:
	MSG_DWORD stPacket;

	// ��û�Ǿ����� �˸���.
	stPacket.Category	= MP_MARRIAGE;
	stPacket.Protocol	= MP_MARRIAGE_TELEPORT_TO_PARTNER;
	stPacket.dwObjectID	= pPlayer->GetID();
	stPacket.dwData		= eResult;

	pPlayer->SendMsg(&stPacket, sizeof(stPacket));
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
// 																															   ȣĪ ����
//
VOID CSHMarriageManager::SRV_ChangeNickname(CPlayer* pPlayer, char* pszNickname)
{
#if defined(_MAPSERVER_)
	RESULT eResult = RESULT_OK;
	char szNickname[MAX_NAME_LENGTH+1] = {0,};

	// ��ȥ�� �߳�?
	if (pPlayer->IsMarried() == FALSE)
	{
		eResult = RESULT_FAIL_01;
		goto FAILED;
	}

	// ȣĪ ����
	strncpy(szNickname, pszNickname, NICKNAME_CHARACTER_NUM);
	pPlayer->SetMarriagePartnerNickName(szNickname);

	// DB�� ����
//	Marriage_Nickname_Save(pPlayer->GetID(), szNickname);

FAILED:
	MSG_DWORD_NAME stPacket;

	// ��û�Ǿ����� �˸���.
	stPacket.Category	= MP_MARRIAGE;
	stPacket.Protocol	= MP_MARRIAGE_NICKNAME;
	stPacket.dwObjectID	= pPlayer->GetID();
	stPacket.dwData		= eResult;
	SafeStrCpy(stPacket.Name, szNickname, MAX_NAME_LENGTH+1);
	/*
	pPlayer->SendMsg(&stPacket, sizeof(stPacket));
	PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pPlayer, &stPacket, sizeof(stPacket));
	*/
	PACKEDDATA_OBJ->QuickSend(pPlayer, &stPacket, sizeof(stPacket));
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
// 																														��ȥ ���� ������
//
VOID CSHMarriageManager::SRV_SendMarriageInfo(CPlayer* pPlayer)
{
#if defined(_MAPSERVER_)
	if (pPlayer->GetMarriagePartnerID())
	{
		// ����� �̸�, ȣĪ�� �⺻ ĳ���� ����ü�� ���ԵǾ� ����. (�׸��� ���� �ٸ� �÷��̾� ������)
		// �߰��� ������Ʈ���� ������� ������ �˸���.
		PACKET_MARRIAGE_INFO stPacket;

		stPacket.Category		= MP_MARRIAGE;
		stPacket.Protocol		= MP_MARRIAGE_LOADINFO;
		stPacket.nPartnerID		= pPlayer->GetMarriagePartnerID();
		stPacket.nRace			= pPlayer->GetMarriagePartnerRace();		
		stPacket.nJobFirst		= pPlayer->GetMarriagePartnerJobFirst();	
		stPacket.nJobCur		= pPlayer->GetMarriagePartnerJobCur();		
		stPacket.nJobGrade		= pPlayer->GetMarriagePartnerJobGrade();	
		stPacket.nLv			= pPlayer->GetMarriagePartnerLv();			
		stPacket.nTeleportFreq	= pPlayer->GetMarriageTeleportFreq();

		pPlayer->SendMsg(&stPacket, sizeof(stPacket));
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
// 																															   ��ȥ ��û
//
VOID CSHMarriageManager::CLI_RequestMarriage(BOOL bWin)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	if (bWin)
	{
		if (OBJECTMGR->GetSelectedObject() &&
			OBJECTMGR->GetSelectedObject()->GetObjectKind() == eObjectKind_Player)
		{
			char szTxt[256];
			sprintf(szTxt, "%s�Կ��� ��ȥ��û�� �Ͻðڽ��ϱ�?", OBJECTMGR->GetSelectedObject()->GetObjectName());
			WINDOWMGR->MsgBox( MBI_MARRIAGE_MARRIAGE, MBT_YESNO, szTxt );
		}
		else
		{
			CHATMGR->AddMsg( CTC_SYSMSG, "��ȥ�� ��븦 ������ �ּ���.");
		}
	}
	else
	{
		if (OBJECTMGR->GetSelectedObject() &&
			OBJECTMGR->GetSelectedObject()->GetObjectKind() == eObjectKind_Player)
		{
			MSG_DWORD stPacket;
			stPacket.Category 		= MP_MARRIAGE;
			stPacket.Protocol 		= MP_MARRIAGE_MARRIAGE_REQUEST;
			stPacket.dwObjectID		= HEROID;
			stPacket.dwData			= OBJECTMGR->GetSelectedObject()->GetID();

			NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
		}
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
// 																															   ��ȥ ����
//
VOID CSHMarriageManager::CLI_RequestAcceptMarriage(BOOL bAccept)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	MSG_DWORD stPacket;
	stPacket.Category 		= MP_MARRIAGE;
	stPacket.Protocol 		= MP_MARRIAGE_MARRIAGE_REQUESTED;
	stPacket.dwObjectID		= HEROID;
	stPacket.dwData			= bAccept;

	NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
// 																														  ���� ��ȥ ��û
//
VOID CSHMarriageManager::CLI_RequestAgreeDivorce()
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	MSG_DWORD stPacket;
	stPacket.Category 		= MP_MARRIAGE;
	stPacket.Protocol 		= MP_MARRIAGE_AGREE_DIVORCE_REQUEST;
	stPacket.dwObjectID		= HEROID;

	NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
// 																														  ���� ��ȥ ����
//
VOID CSHMarriageManager::CLI_RequestAcceptAgreeDivorce(BOOL bAccept)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	MSG_DWORD stPacket;
	stPacket.Category 		= MP_MARRIAGE;
	stPacket.Protocol 		= MP_MARRIAGE_AGREE_DIVORCE_RESULT;
	stPacket.dwObjectID		= HEROID;
	stPacket.dwData			= bAccept;

	NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
// 																														  ���� ��ȥ ��û
//
VOID CSHMarriageManager::CLI_RequestForceDivorce()
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	MSG_DWORD stPacket;
	stPacket.Category 		= MP_MARRIAGE;
	stPacket.Protocol 		= MP_MARRIAGE_FORCE_DIVORCE_REQUEST;
	stPacket.dwObjectID		= HEROID;

	NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//																											 ����� ��ġ�� �ڷ���Ʈ ��û
//
VOID CSHMarriageManager::CLI_RequestTeleportToPartner()
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	MSG_DWORD stPacket;
	stPacket.Category 		= MP_MARRIAGE;
	stPacket.Protocol 		= MP_MARRIAGE_TELEPORT_TO_PARTNER;
	stPacket.dwObjectID		= HEROID;

	NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//																														  ȣĪ ���� ��û
//
VOID CSHMarriageManager::CLI_RequestNickname(char* pszNickname)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	if (pszNickname == NULL || strlen(pszNickname) > NICKNAME_CHARACTER_NUM)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, "�ѱ�(����) %d����, ���� %d���� ���ϸ� �����մϴ�.", NICKNAME_CHARACTER_NUM/2, NICKNAME_CHARACTER_NUM);
		return;
	}

	MSG_NAME stPacket;
	ZeroMemory(&stPacket, sizeof(stPacket));

	stPacket.Category 		= MP_MARRIAGE;
	stPacket.Protocol 		= MP_MARRIAGE_NICKNAME;
	stPacket.dwObjectID		= HEROID;
	SafeStrCpy(stPacket.Name, pszNickname, NICKNAME_CHARACTER_NUM+1);
	stPacket.Name[NICKNAME_CHARACTER_NUM+1] = 0;

	NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
// 																												   �̸� �տ� ȣĪ ���̱�
//
VOID CSHMarriageManager::CLI_SetNickname(CPlayer* pPlayer, char* pszPartnerName, char* pszPartnerNickname)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	if (pszPartnerName[0] &&
		pPlayer->GetObjectBalloon() &&
		pPlayer->GetObjectBalloon()->GetOBalloonName())
	{
		char szName[MAX_MONSTER_NAME_LENGTH+1] = "";
		char szNickName[MAX_NAME_LENGTH+1];

		if (pszPartnerNickname && pszPartnerNickname[0])
		{
			sprintf(szNickName, "�� %s", pszPartnerNickname);
		}
		else
		{
			if (pPlayer->GetGender() == GENDER_MALE)
			{
				sprintf(szNickName, "�� ����");
			}
			else
			{
				sprintf(szNickName, "�� �Ƴ�");
			}
		}

		sprintf(szName, "%s%s %s", pszPartnerName, szNickName, pPlayer->GetObjectName());
		pPlayer->GetObjectBalloon()->GetOBalloonName()->SetName(szName);
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
// 																											 �����κ����� ���� �м�/ó��
//
VOID CSHMarriageManager::CLI_ParseAnswerFromSrv(void* pMsg)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	MSGBASE* pTmp = (MSGBASE*)pMsg;

 	switch(pTmp->Protocol)
	{
	// ��ȥ ���� �ε�
	case MP_MARRIAGE_LOADINFO:
		{
			PACKET_MARRIAGE_INFO* pPacket = (PACKET_MARRIAGE_INFO*)pMsg;
			
			HERO->SetMarriagePartnerRace(pPacket->nRace);
			HERO->SetMarriagePartnerJobFirst(pPacket->nJobFirst);
			HERO->SetMarriagePartnerJobCur(pPacket->nJobCur);
			HERO->SetMarriagePartnerJobGrade(pPacket->nJobGrade);
			HERO->SetMarriagePartnerLv(pPacket->nLv);
			HERO->SetMarriagePartnerTeleportFreq(pPacket->nTeleportFreq);
		}
		break;
	// ����ڰ� ����
	case MP_MARRIAGE_CONN_TO_PARTNER:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, "����� %s���� �����̽��ϴ�.", HERO->GetMarriagePartnerName());
		}
		break;
	// �����ߴ� �� ����ڰ� ���� ���̴�.
	case MP_MARRIAGE_CONN_TO_PARTNER_TO_SENDER:
		{
		}
		break;
	// ��ȥ ��û
	case MP_MARRIAGE_MARRIAGE_REQUEST:
		{
			MSG_NAME_DWORD2* pPacket = (MSG_NAME_DWORD2*)pMsg;

			switch(pPacket->dwData1)
			{
			case RESULT_OK:			CHATMGR->AddMsg( CTC_SYSMSG, "%s�Կ��� ��ȥ��û�� �߽��ϴ�.", pPacket->Name);								break;
			case RESULT_FAIL_01:	CHATMGR->AddMsg( CTC_SYSMSG, "�̹� ��ȥ�� �� �����Դϴ�.", pPacket->Name);									break;
			case RESULT_FAIL_02:	CHATMGR->AddMsg( CTC_SYSMSG, "������ �̹� ��ȥ�� �� �����Դϴ�.");										break;
			case RESULT_FAIL_03:	CHATMGR->AddMsg( CTC_SYSMSG, "���� ������ ��ȥ�� �� �����ϴ�.");											break;
			case RESULT_FAIL_04:	
				CHATMGR->AddMsg( CTC_SYSMSG, " �̹� �������� ��ȥ �غ� ���Դϴ�.", DIVORCE_PENALTY_REMARRIAGE_TIME, pPacket->dwData2);
				break;
			case RESULT_FAIL_05:	
				CHATMGR->AddMsg( CTC_SYSMSG, "������ �̹� �������� ��ȥ �غ� ���Դϴ�.");
				break;
			case RESULT_FAIL_06:	
				CHATMGR->AddMsg( CTC_SYSMSG, "��ȥ �� %d�ð� �Ŀ� �����մϴ�.(���� %d�ð� ���)", DIVORCE_PENALTY_REMARRIAGE_TIME, pPacket->dwData2);
				break;
			case RESULT_FAIL_07:	
				CHATMGR->AddMsg( CTC_SYSMSG, "��ȥ �� %d�ð� �Ŀ� �����մϴ�.(���� %d�ð� ���)", DIVORCE_PENALTY_REMARRIAGE_TIME, pPacket->dwData2);
				break;
			case RESULT_FAIL_08:	
				CHATMGR->AddMsg( CTC_SYSMSG, "������ ��ȥ �� %d�ð� �Ŀ� �����մϴ�.(���� %d�ð� ���)", DIVORCE_PENALTY_REMARRIAGE_TIME, pPacket->dwData2);		
				break;
			case RESULT_FAIL_09:	
				CHATMGR->AddMsg( CTC_SYSMSG, "��ȥ�� �ʿ��� ������(%s)�� �����ϰ� �־�� �մϴ�.", ITEMMGR->GetItemInfo(pPacket->dwData2)->ItemName);		
				break;
			case RESULT_FAIL_10:	
				CHATMGR->AddMsg( CTC_SYSMSG, "������ ��ȥ�� �ʿ��� ������(%s)�� �����ϰ� �־�� �մϴ�.", ITEMMGR->GetItemInfo(pPacket->dwData2)->ItemName);		
				break;
			case RESULT_FAIL_11:	
				CHATMGR->AddMsg( CTC_SYSMSG, "��ȥ������ ȹ���� ���� %d���� �κ��� ����־�� �մϴ�.", pPacket->dwData2);;		
				break;
			case RESULT_FAIL_12:	
				CHATMGR->AddMsg( CTC_SYSMSG, "������ ��ȥ������ ȹ���� ���� %d���� �κ��� ����־�� �մϴ�.", pPacket->dwData2);;		
				break;
			}
		}
		break;
	// ��ȥ ��û ����
	case MP_MARRIAGE_MARRIAGE_REQUESTED:
		{
			MSG_NAME_DWORD2* pPacket = (MSG_NAME_DWORD2*)pMsg;

			switch(pPacket->dwData1)
			{
			case RESULT_OK:
				{
					char szTxt[256];
					sprintf(szTxt, "%s���� ��ȥ��û�� �߽��ϴ�. �����Ͻðڽ��ϱ�?", pPacket->Name);
					WINDOWMGR->MsgBox( MBI_MARRIAGE_MARRIAGE_REQUESTED, MBT_YESNO, szTxt );
				}
				break;
			}
		}
		break;
	// ��ȥ ���
	case MP_MARRIAGE_MARRIAGE_RESULT:
		{
			MSG_DWORD_NAME* pPacket = (MSG_DWORD_NAME*)pMsg;

			if (HERO == NULL) return;

			switch(pPacket->dwData)
			{
			case RESULT_OK:
				{
					if (pPacket->dwObjectID == HERO->GetID())
					{
						// ��ȥ ����
						CHATMGR->AddMsg( CTC_SYSMSG, "%s�԰� ��ȥ�Ͽ����ϴ�!!", pPacket->Name);
						HERO->SetMarriagePartnerName(pPacket->Name);
						// ..�⺻ ȣĪ ����
						CLI_SetNickname((CPlayer*)HERO, pPacket->Name);
					}
					else
					{
						// ������ ��ȥ�� ĳ������ ȣĪ ����
						CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pPacket->dwObjectID);
						if( !pPlayer )	return;

						CLI_SetNickname(pPlayer, pPacket->Name);
					}
				}
				break;
			case RESULT_FAIL_01: CHATMGR->AddMsg( CTC_SYSMSG, "%s���� ��ȥ��û�� �����߽��ϴ�.", pPacket->Name);		break;
			case RESULT_FAIL_02: CHATMGR->AddMsg( CTC_SYSMSG, "%s���� ��ȥ��û�� �����߽��ϴ�.", pPacket->Name);		break;
			}
		}
		break;
	// ���� ��ȥ ��û
	case MP_MARRIAGE_AGREE_DIVORCE_REQUEST:
		{
			MSG_NAME_DWORD2* pPacket = (MSG_NAME_DWORD2*)pMsg;

			switch(pPacket->dwData1)
			{
			case RESULT_OK:			CHATMGR->AddMsg( CTC_SYSMSG, "%s�Կ��� ��ȥ��û�� �߽��ϴ�.", pPacket->Name);						break;
			case RESULT_FAIL_01:	CHATMGR->AddMsg( CTC_SYSMSG, "���� ��ȥ�� �� �� �����Դϴ�.", pPacket->Name);						break;
			case RESULT_FAIL_02:	CHATMGR->AddMsg( CTC_SYSMSG, "�̹� ����ڿ��� ��ȥ��û�� �� �����Դϴ�.");							break;
			case RESULT_FAIL_03:	CHATMGR->AddMsg( CTC_SYSMSG, "����ڰ� ���� ���� �ƴϰų� ������ ���� �ʽ��ϴ�.");					break;
			case RESULT_FAIL_04:	CHATMGR->AddMsg( CTC_SYSMSG, "�̹� ����ڷκ��� ��ȥ��û�� ���� �����Դϴ�.");						break;
			}
		}
		break;
	// ���� ��ȥ ��û ����
	case MP_MARRIAGE_AGREE_DIVORCE_REQUESTED:
		{
			MSG_NAME_DWORD2* pPacket = (MSG_NAME_DWORD2*)pMsg;

			switch(pPacket->dwData1)
			{
			case RESULT_OK:
				{
					char szTxt[256];
					sprintf(szTxt, "%s���� ��ȥ��û�� �߽��ϴ�. �����Ͻðڽ��ϱ�?", pPacket->Name);
					WINDOWMGR->MsgBox( MBI_MARRIAGE_AGREE_DIVORCE_REQUESTED, MBT_YESNO, szTxt );
				}
				break;
			}
		}
		break;
	// ���� ��ȥ ���
	case MP_MARRIAGE_AGREE_DIVORCE_RESULT:
		{
			MSG_DWORD_NAME* pPacket = (MSG_DWORD_NAME*)pMsg;

			if (HERO == NULL) return;

			switch(pPacket->dwData)
			{
			case RESULT_OK:
				{
					if (pPacket->dwObjectID == HERO->GetID())
					{
						// ��ȥ ����
						CHATMGR->AddMsg( CTC_SYSMSG, "%s�԰� ��ȥ�Ͽ����ϴ�!", pPacket->Name);
						HERO->SetMarriagePartnerName("");
						// ..ȣĪ�� �����ϰ� ĳ���� �̸����� ����
						HERO->GetObjectBalloon()->GetOBalloonName()->SetName(HERO->GetObjectName());
					}
					else
					{
						// ������ ��ȥ�� ĳ������ ȣĪ ����
						CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pPacket->dwObjectID);
						if( !pPlayer )	return;

						// ..ȣĪ�� �����ϰ� ĳ���� �̸����� ����
						pPlayer->GetObjectBalloon()->GetOBalloonName()->SetName(pPlayer->GetObjectName());
					}
				}
				break;
			case RESULT_FAIL_01: CHATMGR->AddMsg( CTC_SYSMSG, "%s���� ��ȥ��û�� �����߽��ϴ�.", pPacket->Name);		break;
			case RESULT_FAIL_02: CHATMGR->AddMsg( CTC_SYSMSG, "%s���� ��ȥ��û�� �����߽��ϴ�.", pPacket->Name);		break;
			}
		}
		break;
	// ���� ��ȥ ��û
	case MP_MARRIAGE_FORCE_DIVORCE_REQUEST:
		{
			MSG_NAME_DWORD2* pPacket = (MSG_NAME_DWORD2*)pMsg;

			switch(pPacket->dwData1)
			{
			case RESULT_OK:			CHATMGR->AddMsg( CTC_SYSMSG, "%s�Կ��� ��ȥ��û�� �߽��ϴ�.", pPacket->Name);		break;
			case RESULT_FAIL_01:	CHATMGR->AddMsg( CTC_SYSMSG, "���� ��ȥ�� �� �� �����Դϴ�.", pPacket->Name);		break;
			}
		}
		break;
	// ���� ��ȥ ���
	case MP_MARRIAGE_FORCE_DIVORCE_RESULT:
		{
			MSG_NAME_DWORD2* pPacket = (MSG_NAME_DWORD2*)pMsg;

			switch(pPacket->dwData1)
			{
			case RESULT_OK:			CHATMGR->AddMsg( CTC_SYSMSG, "%s�԰� ��ȥ�Ͽ����ϴ�!!", pPacket->Name);				break;
			}
		}
		break;
	// ���� ��ȥ ��� - ��Ʈ��
	case MP_MARRIAGE_FORCE_DIVORCE_RESULT_TO_PARTNER_TO_CLIENT:
		{
			MSG_NAME_DWORD2* pPacket = (MSG_NAME_DWORD2*)pMsg;

			if (HERO == NULL) return;

			switch(pPacket->dwData1)
			{
			case RESULT_OK:
				{
					if (pPacket->dwObjectID == HERO->GetID())
					{
						CHATMGR->AddMsg( CTC_SYSMSG, "%s���� ������ȥ�� �߽��ϴ�!!", pPacket->Name);		break;
						HERO->SetMarriagePartnerName("");
						// ..ȣĪ�� �����ϰ� ĳ���� �̸����� ����
						HERO->GetObjectBalloon()->GetOBalloonName()->SetName(HERO->GetObjectName());
					}
					else
					{
						// ������ ��ȥ�� ĳ������ ȣĪ ����
						CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pPacket->dwObjectID);
						if( !pPlayer )	return;

						// ..ȣĪ�� �����ϰ� ĳ���� �̸����� ����
						pPlayer->GetObjectBalloon()->GetOBalloonName()->SetName(pPlayer->GetObjectName());
					}
				}
				break;
			}
		}
		break;
	// ȣĪ ����
	case MP_MARRIAGE_NICKNAME:
		{
			MSG_DWORD_NAME* pPacket = (MSG_DWORD_NAME*)pMsg;

 			switch(pPacket->dwData)
			{
			case RESULT_OK:
				{
					if (HERO == NULL) return;

					// ..ȣĪ ����
					if (HERO->GetID() == pPacket->dwObjectID)
					{
						HERO->SetMarriagePartnerNickName(pPacket->Name);
						CLI_SetNickname((CPlayer*)HERO, HERO->GetMarriagePartnerName(), pPacket->Name);
					}
					else
					{
						// ������ ĳ������ ȣĪ ����
						CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pPacket->dwObjectID);
						if( !pPlayer )	return;

						pPlayer->SetMarriagePartnerNickName(pPacket->Name);
						CLI_SetNickname(pPlayer, pPlayer->GetMarriagePartnerName(), pPacket->Name);
					}
				}
				break;
			case RESULT_FAIL_01: CHATMGR->AddMsg( CTC_SYSMSG, "���� ��ȥ�� �� �� �����Դϴ�.");		break;
			}
		}
		break;
	// ����� ��ġ�� �ڸ���Ʈ
	case MP_MARRIAGE_TELEPORT_TO_PARTNER:
		{
			MSG_DWORD* pPacket = (MSG_DWORD*)pMsg;

			switch(pPacket->dwData)
			{
			case RESULT_FAIL_01: CHATMGR->AddMsg( CTC_SYSMSG, "���� ��ȥ�� �� �� �����Դϴ�.");		break;
			case RESULT_FAIL_02: CHATMGR->AddMsg( CTC_SYSMSG, "��ȥ ������ ���� Ƚ���� �����մϴ�.");		break;
			}
			break;
		}
		break;
	}
#endif
}


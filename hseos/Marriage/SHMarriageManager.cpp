/*********************************************************************

	 파일		: SHMarriageManager.cpp
	 작성자		: hseos
	 작성일		: 2007/12/10

	 파일설명	: 결혼 클래스의 소스

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

UINT CSHMarriageManager::DIVORCE_PENALTY_REMARRIAGE_TIME = 0;		// 이혼 후 결혼에 필요한 시간
UINT CSHMarriageManager::TELEPORT_TO_PARTNER_FREQ = 0;				// 배우자 위치로 텔레포트 가능 회수
UINT CSHMarriageManager::NICKNAME_CHARACTER_NUM = 0;				// 호칭 글자(바이트) 수

// -------------------------------------------------------------------------------------------------------------------------------------
// 																																  생성자
//
CSHMarriageManager::CSHMarriageManager()
{
	ZeroMemory(&m_stMarriageNeedItem, sizeof(m_stMarriageNeedItem));
	ZeroMemory(&m_stMarriageTakeItem, sizeof(m_stMarriageTakeItem));

	LoadMarriageInfo();
}

// -------------------------------------------------------------------------------------------------------------------------------------
// 																																  파괴자
//
CSHMarriageManager::~CSHMarriageManager()
{
	SAFE_DELETE_ARRAY(m_stMarriageNeedItem.pstWearItemID);
	SAFE_DELETE_ARRAY(m_stMarriageTakeItem.pstItem);
}

// -------------------------------------------------------------------------------------------------------------------------------------
// 																															 데이터 읽기
//
BOOL CSHMarriageManager::LoadMarriageInfo()
{
	char szLine[MAX_PATH], szFile[MAX_PATH];
	int	 nKind = 0, nCnt = 0;
	CMHFile fp;

	sprintf(szFile, "./System/Resource/Marriage.bin");
	fp.Init(szFile, "rb");
	if(!fp.IsInited()) return FALSE;
	// 개수 체크
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
	// 데이터 읽기
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

	// 만약을 대비해서 값이 잘못되었을 경우 기본값을 4 로 한다.
	if (NICKNAME_CHARACTER_NUM == 0 || NICKNAME_CHARACTER_NUM > MAX_NAME_LENGTH) NICKNAME_CHARACTER_NUM = 4;

	return TRUE;
}

// -------------------------------------------------------------------------------------------------------------------------------------
// 																									 클라이언트 요청 분석/처리(에이전트)
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
// 																										   서버 요청 분석/처리(에이전트)
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

			// 배우자가 있는 에이전트 인가?
			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pPacket->dwData1);
			if (pUserInfo == NULL) return;

			// 강제 이혼되었음을 알린다.
			// ..접속한 배우자의 ID 설정
			pUserInfo->nMarriageConnPartnerID = 0;
			// 맵서버로 가서 강제 이혼되었음을 설정한다.
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

			// 배우자가 있는 에이전트 인가?
			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pPacket->dwObjectID);
			if (pUserInfo == NULL) return;
		}
		return;
	// 접속했음을 배우자에게 알렸는 데, 배우자도 접속 중임을 알려왔다.
	case MP_MARRIAGE_CONN_TO_PARTNER_TO_SENDER:
		{
			MSG_DWORD2* pPacket = (MSG_DWORD2*)pMsg;

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pPacket->dwData1);
			if (pUserInfo == NULL) return;

			// 접속한 배우자의 ID 설정
			pUserInfo->nMarriageConnPartnerID = pPacket->dwData2;

			// 클라이언트에 알리기
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
	// 접속 시 결혼 정보 로드
	case MP_MARRIAGE_LOADINFO:
		{
			PACKET_MARRIAGE_INFO* pPacket = (PACKET_MARRIAGE_INFO*)pMsg;

			// 배우자에게 자신의 접속을 알린다. 같은 에이전트에 없다면 다른 에이전트로 Broadcast
			USERINFO* pPartnerInfo = g_pUserTableForObjectID->FindUser(pPacket->nPartnerID);
			if (pPartnerInfo)
			{
				// 접속한 배우자의 ID 설정
				pUserInfo->nMarriageConnPartnerID = pPartnerInfo->dwCharacterID;
				pPartnerInfo->nMarriageConnPartnerID = pUserInfo->dwCharacterID;

				// 클라이언트에 알리기
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

			// 클라이언트로 정보 전송. 순서 바꾸면 안 됨.
			g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)pMsg, dwLength );
		}
		break;
	// 배우자가 접속
	case MP_MARRIAGE_CONN_TO_PARTNER:
		{
			MSG_DWORD2* pPacket = (MSG_DWORD2*)pMsg;

			// 접속한 배우자의 ID 설정
			pUserInfo->nMarriageConnPartnerID = pPacket->dwData1;

			// 클라이언트에 알리기
			MSG_DWORD2 stPacket;
			stPacket.Category	= MP_MARRIAGE;
			stPacket.Protocol	= MP_MARRIAGE_CONN_TO_PARTNER;
			stPacket.dwObjectID	= pUserInfo->dwCharacterID;

			g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );

			// 자신도 접속 중임을 알리자
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
	// 강제 이혼 파트너에게 알리기
	case MP_MARRIAGE_FORCE_DIVORCE_RESULT_TO_PARTNER:
		{
			// 배우자가 강제 이혼을 했기에 파트너에게 이를 알린다.
			// ..배우자가 같은 에이전트에 없다면 BroadCast
			if (pUserInfo->nMarriageConnPartnerID == 0) return;

			USERINFO* pPartnerInfo = g_pUserTableForObjectID->FindUser(pUserInfo->nMarriageConnPartnerID);
			if (pPartnerInfo)
			{
				// 접속한 배우자의 ID 설정
				pPartnerInfo->nMarriageConnPartnerID = 0;

				// 맵서버로 가서 강제 이혼되었음을 설정한다.
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
			// 접속한 배우자의 ID 설정
			pUserInfo->nMarriageConnPartnerID = 0;
		}
		break;
	// 배우자 위치로 텔레포트
	case MP_MARRIAGE_TELEPORT_TO_PARTNER:
		{
			MSG_DWORD* pPacket = (MSG_DWORD*)pMsg;

			if (pPacket->dwData == RESULT_OK)
			{
				// 텔레포트 조건을 충족하면 배우자를 찾기 위해 에이전트로 BroadCast
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
// 																												  배우자 위치로 텔레포트
//
VOID CSHMarriageManager::ASRV_TeleportToPartner(USERINFO* pUserInfo)
{
}


// -------------------------------------------------------------------------------------------------------------------------------------
// 																											   클라이언트 요청 분석/처리
//
VOID CSHMarriageManager::SRV_ParseRequestFromClient(DWORD dwConnectionID, char* pMsg, DWORD dwLength)
{
#if defined(_MAPSERVER_)
	MSGBASE* pPacket = (MSGBASE*)pMsg;

	// 잘못된 플레이어 처리
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pPacket->dwObjectID );
	if( !pPlayer )	
	{
		return;
	}

	switch(pPacket->Protocol)
	{
	// 결혼
	case MP_MARRIAGE_MARRIAGE_REQUEST:
		{
			MSG_DWORD* pPacket = (MSG_DWORD*)pMsg;
			SRV_Marriage(pPlayer, pPacket->dwData);
		}
		break;
	// 결혼 결과
	case MP_MARRIAGE_MARRIAGE_REQUESTED:
		{
			MSG_DWORD* pPacket = (MSG_DWORD*)pMsg;
			SRV_MarriageResult(pPlayer, pPacket->dwData);
		}
		break;
	// 합의 이혼
	case MP_MARRIAGE_AGREE_DIVORCE_REQUEST:
		{
			MSG_DWORD* pPacket = (MSG_DWORD*)pMsg;
			SRV_AgreeDivorce(pPlayer, pPacket->dwData);
		}
		break;
	// 이혼 결과
	case MP_MARRIAGE_AGREE_DIVORCE_REQUESTED:
		{
			MSG_DWORD* pPacket = (MSG_DWORD*)pMsg;
			SRV_AgreeDivorceResult(pPlayer, pPacket->dwData);
		}
		break;
	// 강제 이혼
	case MP_MARRIAGE_FORCE_DIVORCE_REQUEST:
		{
			MSG_DWORD* pPacket = (MSG_DWORD*)pMsg;
			SRV_ForceDivorce(pPlayer);
		}
		break;
	// 강제 이혼 당함
	case MP_MARRIAGE_FORCE_DIVORCE_RESULT_TO_PARTNER:
	case MP_MARRIAGE_FORCE_DIVORCE_RESULT_TO_PARTNER_TO_OTHER_AGENT:
		{
			MSG_DWORD* pPacket = (MSG_DWORD*)pMsg;
			SRV_ForceDivorced(pPlayer);
		}
		break;
	// 호칭
	case MP_MARRIAGE_NICKNAME:
		{
			MSG_NAME* pPacket = (MSG_NAME*)pMsg;
			SRV_ChangeNickname(pPlayer, pPacket->Name);
		}
		break;
	// 배우자 위치로 텔레포트
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
// 																																	결혼
//
VOID CSHMarriageManager::SRV_Marriage(CPlayer* pPlayer, DWORD nPartnerID)
{
#if defined(_MAPSERVER_)
	RESULT	eResult		= RESULT_OK;
	DWORD	nValue		= 0;

	CPlayer* pPartner = (CPlayer*)g_pUserTable->FindUser( nPartnerID );
	if( !pPartner ) return;

	// 이미 결혼을 했나?
	if (pPlayer->IsMarried())
	{
		eResult = RESULT_FAIL_01;
		goto FAILED;
	}

	// 상대방이 이미 결혼을 했나?
	if (pPartner->IsMarried())
	{
		eResult = RESULT_FAIL_02;
		goto FAILED;
	}

	// 성별 체크
	if (pPlayer->GetGender() == pPartner->GetGender())
	{
		eResult = RESULT_FAIL_03;
		goto FAILED;
	}

	// 결혼 신청 중이거나 신청을 받은 상태인가?
	if (pPlayer->GetMarriageRequestPartnerID())
	{
		eResult = RESULT_FAIL_04;
		goto FAILED;
	}

	// 상대방이 결혼 신청 중이거나 신청을 받은 상태인가?
	if (pPartner->GetMarriageRequestPartnerID())
	{
		eResult = RESULT_FAIL_05;
		goto FAILED;
	}

	// 이혼 후 패널티 시간이 지났나?
	if (strcmp(pPlayer->GetMarriageDivorceDate(), "") != 0)
	{
		nValue = GetPastTime(pPlayer->GetMarriageDivorceDate());
		if (nValue < DIVORCE_PENALTY_REMARRIAGE_TIME)
		{
			eResult = RESULT_FAIL_06;
			goto FAILED;
		}
	}
	
	// 상대방의 이혼 후 패널티 시간이 지났나?
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
	
	// 결혼 아이템이 있나?
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

	// 상대방에게 결혼 아이템이 있나?
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

	// 아이템 공간 체크
	// ..겹쳐지는 아이템인지 체크해서 필요 슬롯 개수를 구한다.
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

	// ..필요 슬롯 개수보다 작으면 실패
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

	// ..필요 슬롯 개수보다 작으면 실패. 상대방 체크
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
			// 결혼 신청 상태 설정
			pPlayer->SetMarriageRequestPartnerID(pPartner->GetID());
			pPartner->SetMarriageRequestPartnerID(pPlayer->GetID());

			// 신청되었음을 알린다.
			stPacket.Category	= MP_MARRIAGE;
			stPacket.Protocol	= MP_MARRIAGE_MARRIAGE_REQUEST;
			stPacket.dwObjectID	= pPlayer->GetID();
			stPacket.dwData1	= eResult;
			SafeStrCpy(stPacket.Name, pPartner->GetObjectName(), MAX_NAME_LENGTH+1);
			pPlayer->SendMsg(&stPacket, sizeof(stPacket));

			// 상대방에게 결혼 신청을 알린다.
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
			// 클라이언트에 에러 알리기
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
// 																															   결혼 결과
//
VOID CSHMarriageManager::SRV_MarriageResult(CPlayer* pPlayer, BOOL bAccept)
{
#if defined(_MAPSERVER_)
	UINT nValue = 0;

	// 결혼 신청한 사람이 있나?
	if (pPlayer->GetMarriageRequestPartnerID() == FALSE)
	{
		return;
	}
	CPlayer* pPartner = (CPlayer*)g_pUserTable->FindUser( pPlayer->GetMarriageRequestPartnerID() );
	if( !pPartner ) return;

	// 이미 결혼을 했나?
	if (pPlayer->IsMarried())
	{
		return;
	}

	// 상대방이 이미 결혼을 했나?
	if (pPartner->IsMarried())
	{
		return;
	}

	// 이혼 후 패널티 시간이 지났나?
	if (strcmp(pPlayer->GetMarriageDivorceDate(), "") != 0)
	{
		nValue = GetPastTime(pPlayer->GetMarriageDivorceDate());
		if (nValue < DIVORCE_PENALTY_REMARRIAGE_TIME)
		{
			return;
		}
	}
	
	// 상대방의 이혼 후 패널티 시간이 지났나?
	if (strcmp(pPartner->GetMarriageDivorceDate(), "") != 0)
	{
		nValue = GetPastTime(pPartner->GetMarriageDivorceDate());
		if (nValue < DIVORCE_PENALTY_REMARRIAGE_TIME)
		{
			return;
		}
	}

	// 결혼 아이템이 있나?
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

	// 상대방에게 결혼 아이템이 있나?
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

	// 아이템 공간 체크
	// ..겹쳐지는 아이템인지 체크해서 필요 슬롯 개수를 구한다.
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

	// ..필요 슬롯 개수보다 작으면 실패
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

	// ..필요 슬롯 개수보다 작으면 실패. 상대방 체크
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

	// 결혼 처리
	if (bAccept)
	{
		// 결혼 수락
		// ..정보 설정
		pPlayer->SetMarriageRequestPartnerID(0);
		pPlayer->SetMarriagePartnerID(pPartner->GetID());
		pPlayer->SetMarriagePartnerName(pPartner->GetObjectName());
		pPlayer->SetMarriageTeleportFreq(TELEPORT_TO_PARTNER_FREQ);

		pPartner->SetMarriageRequestPartnerID(0);
		pPartner->SetMarriagePartnerID(pPlayer->GetID());
		pPartner->SetMarriagePartnerName(pPlayer->GetObjectName());
		pPartner->SetMarriageRequestPartnerID(TELEPORT_TO_PARTNER_FREQ);
		// ..DB에 저장
//		Marriage_Save(pPlayer->GetID(), pPartner->GetID(), TELEPORT_TO_PARTNER_FREQ);
//		Marriage_Save(pPartner->GetID(), pPlayer->GetID(), TELEPORT_TO_PARTNER_FREQ);

		// ..클라이언트에 알리기
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

		// ..아이템 생성
		for(UINT i=0; i<m_stMarriageTakeItem.nItemNum; i++)
		{
			ITEMMGR->ObtainGeneralItem(pPlayer, m_stMarriageTakeItem.pstItem[i].nItemID, m_stMarriageTakeItem.pstItem[i].nItemNum, eLog_ItemObtainMarriage, MP_ITEM_MARRIAGE_ACK);
			ITEMMGR->ObtainGeneralItem(pPartner, m_stMarriageTakeItem.pstItem[i].nItemID, m_stMarriageTakeItem.pstItem[i].nItemNum, eLog_ItemObtainMarriage, MP_ITEM_MARRIAGE_ACK);
		}
	}
	else
	{
		// 결혼 거절
		// ..정보 설정
		pPlayer->SetMarriageRequestPartnerID(0);
		pPartner->SetMarriageRequestPartnerID(0);

		stPacket.Category	= MP_MARRIAGE;
		stPacket.Protocol	= MP_MARRIAGE_MARRIAGE_RESULT;
		stPacket.dwObjectID	= pPlayer->GetID();
		stPacket.dwData		= RESULT_FAIL_01;
		SafeStrCpy(stPacket.Name, pPartner->GetObjectName(), MAX_NAME_LENGTH+1);
		pPlayer->SendMsg(&stPacket, sizeof(stPacket));

		// 결혼 거절당함
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
// 																															   합의 이혼
//
VOID CSHMarriageManager::SRV_AgreeDivorce(CPlayer* pPlayer, DWORD nPartnerID)
{
#if defined(_MAPSERVER_)
	RESULT	eResult		= RESULT_OK;
	DWORD	nValue		= 0;

	// 결혼을 했나?
	if (pPlayer->IsMarried() == FALSE)
	{
		eResult = RESULT_FAIL_01;
		goto FAILED;
	}

	// 이미 이혼 신청 중인가?
	if (pPlayer->GetMarriageRequestPartnerID())
	{
		eResult = RESULT_FAIL_02;
		goto FAILED;
	}

	// 배우자와 근접해 있나?
	CPlayer* pPartner = (CPlayer*)g_pUserTable->FindUser( pPlayer->GetMarriagePartnerID() );
	if( !pPartner )
	{
		eResult = RESULT_FAIL_03;
		goto FAILED;
	}

	// 거리체크
	VECTOR3 stPlayerPos, stPartnerPos;
	pPlayer->GetPosition(&stPlayerPos);
	pPartner->GetPosition(&stPartnerPos);
	float nDistance = CalcDistanceXZ(&stPlayerPos, &stPartnerPos); 
	if (nDistance > 500)
	{
		eResult = RESULT_FAIL_03;
		goto FAILED;
	}

	// 배우자가 이미 이혼 신청 중인가?
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
			// 이혼 신청 상태 설정
			pPlayer->SetMarriageRequestPartnerID(pPartner->GetID());
			pPartner->SetMarriageRequestPartnerID(pPlayer->GetID());

			// 신청되었음을 알린다.
			stPacket.Category	= MP_MARRIAGE;
			stPacket.Protocol	= MP_MARRIAGE_AGREE_DIVORCE_REQUEST;
			stPacket.dwObjectID	= pPlayer->GetID();
			stPacket.dwData1	= eResult;
			SafeStrCpy(stPacket.Name, pPartner->GetObjectName(), MAX_NAME_LENGTH+1);
			pPlayer->SendMsg(&stPacket, sizeof(stPacket));

			// 상대방에게 결혼 신청을 알린다.
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
			// 클라이언트에 에러 알리기
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
// 																														  합의 이혼 결과
//
VOID CSHMarriageManager::SRV_AgreeDivorceResult(CPlayer* pPlayer, BOOL bAccept)
{
#if defined(_MAPSERVER_)
	RESULT	eResult		= RESULT_OK;
	DWORD	nValue		= 0;

	// 결혼을 했나?
	if (pPlayer->IsMarried() == FALSE)
	{
		eResult = RESULT_FAIL_01;
		goto FAILED;
	}

	// 이혼 신청을 받았나?
	if (pPlayer->GetMarriageRequestPartnerID() == FALSE)
	{
		eResult = RESULT_FAIL_02;
		goto FAILED;
	}

	// 배우자와 근접해 있나?
	CPlayer* pPartner = (CPlayer*)g_pUserTable->FindUser( pPlayer->GetMarriagePartnerID() );
	if( !pPartner )
	{
		eResult = RESULT_FAIL_03;
		goto FAILED;
	}

	// 거리체크
	// ..둘의 거리로 체크. 정상적인 클라이언트의 경우에 이혼 신청자는 NPC에 근접해 있을 테니..일단 둘의 거리만 체크(시간되면 NPC체크로 수정..^^)
	VECTOR3 stPlayerPos, stPartnerPos;
	pPlayer->GetPosition(&stPlayerPos);
	pPartner->GetPosition(&stPartnerPos);
	float nDistance = CalcDistanceXZ(&stPlayerPos, &stPartnerPos); 
	if (nDistance > 500)
	{
		eResult = RESULT_FAIL_03;
		goto FAILED;
	}

	// 배우자가 이혼 신청 중인가?
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
				// 이혼 수락
				// ..정보 설정
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
				// 이혼 일자
				sprintf(szTxt, "%04d-%02d-%02d %02d:%02d", MHTIMEMGR_OBJ->GetCurLocalTime()->wYear, 
														  MHTIMEMGR_OBJ->GetCurLocalTime()->wMonth,
														  MHTIMEMGR_OBJ->GetCurLocalTime()->wDay,
														  MHTIMEMGR_OBJ->GetCurLocalTime()->wHour,
														  MHTIMEMGR_OBJ->GetCurLocalTime()->wMinute);
				pPlayer->SetMarriageDivorceDate(szTxt);
				pPartner->SetMarriageDivorceDate(szTxt);

				// DB에 저장
//				Marriage_Divorce_Save(pPlayer->GetID());
//				Marriage_Divorce_Save(pPartner->GetID());

				// ..클라이언트에 알리기
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
				// 이혼 거절
				// ..정보 설정
				pPlayer->SetMarriageRequestPartnerID(0);
				pPartner->SetMarriageRequestPartnerID(0);

				stPacket.Category	= MP_MARRIAGE;
				stPacket.Protocol	= MP_MARRIAGE_AGREE_DIVORCE_RESULT;
				stPacket.dwObjectID	= pPlayer->GetID();
				stPacket.dwData		= RESULT_FAIL_01;
				SafeStrCpy(stPacket.Name, pPartner->GetObjectName(), MAX_NAME_LENGTH+1);
				pPlayer->SendMsg(&stPacket, sizeof(stPacket));

				// 이혼 거절당함
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
			// 클라이언트에 에러 알리기
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
// 																															   강제 이혼
//
VOID CSHMarriageManager::SRV_ForceDivorce(CPlayer* pPlayer)
{
#if defined(_MAPSERVER_)
	RESULT	eResult		= RESULT_OK;
	DWORD	nValue		= 0;

	// 결혼을 했나?
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
			// 이혼 처리
			// ..정보 설정
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

			// DB에 저장
			// ..배우자도 여기서 저장해 버린다. 배우자가 현재 접속 중이지 않을 경우 때문에
			// ..여기서 저장하고, 배우자에게 강제 이혼을 알리는 부분에서도 또 저장한다.
			// ..이렇게 안 하면 접속 시 배우자의 결혼 정보가 있는 지 체크해서 없으면 강제 이혼으로 간주하고
			// ..이혼 처리를 해 줘야 한다. 귀찮...
//			Marriage_Divorce_Save(pPlayer->GetID());
//			Marriage_Divorce_Save(nPartnerID);

			// ..클라이언트에 알리기
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
			// ..배우자에게도 알려야 하는데 어디있는지 모르므로 에이전트에서 BroadCasting 한다
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
			// 클라이언트에 에러 알리기
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
// 																														   강제 이혼 당함
//
VOID CSHMarriageManager::SRV_ForceDivorced(CPlayer* pPlayer)
{
#if defined(_MAPSERVER_)
	RESULT	eResult		= RESULT_OK;
	DWORD	nValue		= 0;

	// 결혼을 했나?
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
			// 이혼 처리
			// ..정보 설정
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

			// DB에 저장
//			Marriage_Divorce_Save(pPlayer->GetID());

			// ..클라이언트에 알리기
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
// 																												  배우자 위치로 텔레포트
//
VOID CSHMarriageManager::SRV_TeleportToPartner(CPlayer* pPlayer)
{
#if defined(_MAPSERVER_)
	RESULT eResult = RESULT_OK;

	// 결혼을 했나?
	if (pPlayer->IsMarried() == FALSE)
	{
		eResult = RESULT_FAIL_01;
		goto FAILED;
	}

	//  텔레포트 횟수가 있나?
	if (pPlayer->GetMarriageTeleportFreq() == 0)
	{
		eResult = RESULT_FAIL_02;
		goto FAILED;
	}

FAILED:
	MSG_DWORD stPacket;

	// 신청되었음을 알린다.
	stPacket.Category	= MP_MARRIAGE;
	stPacket.Protocol	= MP_MARRIAGE_TELEPORT_TO_PARTNER;
	stPacket.dwObjectID	= pPlayer->GetID();
	stPacket.dwData		= eResult;

	pPlayer->SendMsg(&stPacket, sizeof(stPacket));
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
// 																															   호칭 변경
//
VOID CSHMarriageManager::SRV_ChangeNickname(CPlayer* pPlayer, char* pszNickname)
{
#if defined(_MAPSERVER_)
	RESULT eResult = RESULT_OK;
	char szNickname[MAX_NAME_LENGTH+1] = {0,};

	// 결혼을 했나?
	if (pPlayer->IsMarried() == FALSE)
	{
		eResult = RESULT_FAIL_01;
		goto FAILED;
	}

	// 호칭 설정
	strncpy(szNickname, pszNickname, NICKNAME_CHARACTER_NUM);
	pPlayer->SetMarriagePartnerNickName(szNickname);

	// DB에 저장
//	Marriage_Nickname_Save(pPlayer->GetID(), szNickname);

FAILED:
	MSG_DWORD_NAME stPacket;

	// 신청되었음을 알린다.
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
// 																														결혼 정보 보내기
//
VOID CSHMarriageManager::SRV_SendMarriageInfo(CPlayer* pPlayer)
{
#if defined(_MAPSERVER_)
	if (pPlayer->GetMarriagePartnerID())
	{
		// 배우자 이름, 호칭은 기본 캐릭터 구조체에 포함되어 있음. (그리드 안의 다른 플레이어 때문에)
		// 중간에 에이전트에서 배우자의 접속을 알린다.
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
// 																															   결혼 요청
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
			sprintf(szTxt, "%s님에게 결혼신청을 하시겠습니까?", OBJECTMGR->GetSelectedObject()->GetObjectName());
			WINDOWMGR->MsgBox( MBI_MARRIAGE_MARRIAGE, MBT_YESNO, szTxt );
		}
		else
		{
			CHATMGR->AddMsg( CTC_SYSMSG, "결혼할 상대를 지정해 주세요.");
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
// 																															   결혼 수락
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
// 																														  합의 이혼 요청
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
// 																														  합의 이혼 수락
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
// 																														  강제 이혼 요청
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
//																											 배우자 위치로 텔레포트 요청
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
//																														  호칭 변경 요청
//
VOID CSHMarriageManager::CLI_RequestNickname(char* pszNickname)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	if (pszNickname == NULL || strlen(pszNickname) > NICKNAME_CHARACTER_NUM)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, "한글(한자) %d글자, 영문 %d글자 이하만 가능합니다.", NICKNAME_CHARACTER_NUM/2, NICKNAME_CHARACTER_NUM);
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
// 																												   이름 앞에 호칭 붙이기
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
			sprintf(szNickName, "의 %s", pszPartnerNickname);
		}
		else
		{
			if (pPlayer->GetGender() == GENDER_MALE)
			{
				sprintf(szNickName, "의 남편");
			}
			else
			{
				sprintf(szNickName, "의 아내");
			}
		}

		sprintf(szName, "%s%s %s", pszPartnerName, szNickName, pPlayer->GetObjectName());
		pPlayer->GetObjectBalloon()->GetOBalloonName()->SetName(szName);
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
// 																											 서버로부터의 응답 분석/처리
//
VOID CSHMarriageManager::CLI_ParseAnswerFromSrv(void* pMsg)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	MSGBASE* pTmp = (MSGBASE*)pMsg;

 	switch(pTmp->Protocol)
	{
	// 결혼 정보 로드
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
	// 배우자가 접속
	case MP_MARRIAGE_CONN_TO_PARTNER:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, "배우자 %s님이 들어오셨습니다.", HERO->GetMarriagePartnerName());
		}
		break;
	// 접속했는 데 배우자가 접속 중이다.
	case MP_MARRIAGE_CONN_TO_PARTNER_TO_SENDER:
		{
		}
		break;
	// 결혼 신청
	case MP_MARRIAGE_MARRIAGE_REQUEST:
		{
			MSG_NAME_DWORD2* pPacket = (MSG_NAME_DWORD2*)pMsg;

			switch(pPacket->dwData1)
			{
			case RESULT_OK:			CHATMGR->AddMsg( CTC_SYSMSG, "%s님에게 결혼신청을 했습니다.", pPacket->Name);								break;
			case RESULT_FAIL_01:	CHATMGR->AddMsg( CTC_SYSMSG, "이미 결혼을 한 상태입니다.", pPacket->Name);									break;
			case RESULT_FAIL_02:	CHATMGR->AddMsg( CTC_SYSMSG, "상대방이 이미 결혼을 한 상태입니다.");										break;
			case RESULT_FAIL_03:	CHATMGR->AddMsg( CTC_SYSMSG, "같은 성별은 결혼할 수 없습니다.");											break;
			case RESULT_FAIL_04:	
				CHATMGR->AddMsg( CTC_SYSMSG, " 이미 누군가와 결혼 준비 중입니다.", DIVORCE_PENALTY_REMARRIAGE_TIME, pPacket->dwData2);
				break;
			case RESULT_FAIL_05:	
				CHATMGR->AddMsg( CTC_SYSMSG, "상대방이 이미 누군가와 결혼 준비 중입니다.");
				break;
			case RESULT_FAIL_06:	
				CHATMGR->AddMsg( CTC_SYSMSG, "이혼 후 %d시간 후에 가능합니다.(현재 %d시간 경과)", DIVORCE_PENALTY_REMARRIAGE_TIME, pPacket->dwData2);
				break;
			case RESULT_FAIL_07:	
				CHATMGR->AddMsg( CTC_SYSMSG, "이혼 후 %d시간 후에 가능합니다.(현재 %d시간 경과)", DIVORCE_PENALTY_REMARRIAGE_TIME, pPacket->dwData2);
				break;
			case RESULT_FAIL_08:	
				CHATMGR->AddMsg( CTC_SYSMSG, "상대방의 이혼 후 %d시간 후에 가능합니다.(현재 %d시간 경과)", DIVORCE_PENALTY_REMARRIAGE_TIME, pPacket->dwData2);		
				break;
			case RESULT_FAIL_09:	
				CHATMGR->AddMsg( CTC_SYSMSG, "결혼에 필요한 아이템(%s)을 장착하고 있어야 합니다.", ITEMMGR->GetItemInfo(pPacket->dwData2)->ItemName);		
				break;
			case RESULT_FAIL_10:	
				CHATMGR->AddMsg( CTC_SYSMSG, "상대방이 결혼에 필요한 아이템(%s)을 장착하고 있어야 합니다.", ITEMMGR->GetItemInfo(pPacket->dwData2)->ItemName);		
				break;
			case RESULT_FAIL_11:	
				CHATMGR->AddMsg( CTC_SYSMSG, "결혼아이템 획득을 위해 %d개의 인벤이 비어있어야 합니다.", pPacket->dwData2);;		
				break;
			case RESULT_FAIL_12:	
				CHATMGR->AddMsg( CTC_SYSMSG, "상대방이 결혼아이템 획득을 위해 %d개의 인벤이 비어있어야 합니다.", pPacket->dwData2);;		
				break;
			}
		}
		break;
	// 결혼 신청 받음
	case MP_MARRIAGE_MARRIAGE_REQUESTED:
		{
			MSG_NAME_DWORD2* pPacket = (MSG_NAME_DWORD2*)pMsg;

			switch(pPacket->dwData1)
			{
			case RESULT_OK:
				{
					char szTxt[256];
					sprintf(szTxt, "%s님이 결혼신청을 했습니다. 수락하시겠습니까?", pPacket->Name);
					WINDOWMGR->MsgBox( MBI_MARRIAGE_MARRIAGE_REQUESTED, MBT_YESNO, szTxt );
				}
				break;
			}
		}
		break;
	// 결혼 결과
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
						// 결혼 성립
						CHATMGR->AddMsg( CTC_SYSMSG, "%s님과 결혼하였습니다!!", pPacket->Name);
						HERO->SetMarriagePartnerName(pPacket->Name);
						// ..기본 호칭 설정
						CLI_SetNickname((CPlayer*)HERO, pPacket->Name);
					}
					else
					{
						// 주위의 결혼한 캐릭터의 호칭 설정
						CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pPacket->dwObjectID);
						if( !pPlayer )	return;

						CLI_SetNickname(pPlayer, pPacket->Name);
					}
				}
				break;
			case RESULT_FAIL_01: CHATMGR->AddMsg( CTC_SYSMSG, "%s님의 결혼신청을 거절했습니다.", pPacket->Name);		break;
			case RESULT_FAIL_02: CHATMGR->AddMsg( CTC_SYSMSG, "%s님이 결혼신청을 거절했습니다.", pPacket->Name);		break;
			}
		}
		break;
	// 합의 이혼 신청
	case MP_MARRIAGE_AGREE_DIVORCE_REQUEST:
		{
			MSG_NAME_DWORD2* pPacket = (MSG_NAME_DWORD2*)pMsg;

			switch(pPacket->dwData1)
			{
			case RESULT_OK:			CHATMGR->AddMsg( CTC_SYSMSG, "%s님에게 이혼신청을 했습니다.", pPacket->Name);						break;
			case RESULT_FAIL_01:	CHATMGR->AddMsg( CTC_SYSMSG, "아직 결혼을 안 한 상태입니다.", pPacket->Name);						break;
			case RESULT_FAIL_02:	CHATMGR->AddMsg( CTC_SYSMSG, "이미 배우자에게 이혼신청을 한 상태입니다.");							break;
			case RESULT_FAIL_03:	CHATMGR->AddMsg( CTC_SYSMSG, "배우자가 접속 중이 아니거나 근접해 있지 않습니다.");					break;
			case RESULT_FAIL_04:	CHATMGR->AddMsg( CTC_SYSMSG, "이미 배우자로부터 이혼신청을 받은 상태입니다.");						break;
			}
		}
		break;
	// 합의 이혼 신청 받음
	case MP_MARRIAGE_AGREE_DIVORCE_REQUESTED:
		{
			MSG_NAME_DWORD2* pPacket = (MSG_NAME_DWORD2*)pMsg;

			switch(pPacket->dwData1)
			{
			case RESULT_OK:
				{
					char szTxt[256];
					sprintf(szTxt, "%s님이 이혼신청을 했습니다. 수락하시겠습니까?", pPacket->Name);
					WINDOWMGR->MsgBox( MBI_MARRIAGE_AGREE_DIVORCE_REQUESTED, MBT_YESNO, szTxt );
				}
				break;
			}
		}
		break;
	// 합의 이혼 결과
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
						// 결혼 성립
						CHATMGR->AddMsg( CTC_SYSMSG, "%s님과 이혼하였습니다!", pPacket->Name);
						HERO->SetMarriagePartnerName("");
						// ..호칭을 삭제하고 캐릭터 이름으로 변경
						HERO->GetObjectBalloon()->GetOBalloonName()->SetName(HERO->GetObjectName());
					}
					else
					{
						// 주위의 이혼한 캐릭터의 호칭 삭제
						CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pPacket->dwObjectID);
						if( !pPlayer )	return;

						// ..호칭을 삭제하고 캐릭터 이름으로 변경
						pPlayer->GetObjectBalloon()->GetOBalloonName()->SetName(pPlayer->GetObjectName());
					}
				}
				break;
			case RESULT_FAIL_01: CHATMGR->AddMsg( CTC_SYSMSG, "%s님의 이혼신청을 거절했습니다.", pPacket->Name);		break;
			case RESULT_FAIL_02: CHATMGR->AddMsg( CTC_SYSMSG, "%s님이 이혼신청을 거절했습니다.", pPacket->Name);		break;
			}
		}
		break;
	// 강제 이혼 신청
	case MP_MARRIAGE_FORCE_DIVORCE_REQUEST:
		{
			MSG_NAME_DWORD2* pPacket = (MSG_NAME_DWORD2*)pMsg;

			switch(pPacket->dwData1)
			{
			case RESULT_OK:			CHATMGR->AddMsg( CTC_SYSMSG, "%s님에게 이혼신청을 했습니다.", pPacket->Name);		break;
			case RESULT_FAIL_01:	CHATMGR->AddMsg( CTC_SYSMSG, "아직 결혼을 안 한 상태입니다.", pPacket->Name);		break;
			}
		}
		break;
	// 강제 이혼 결과
	case MP_MARRIAGE_FORCE_DIVORCE_RESULT:
		{
			MSG_NAME_DWORD2* pPacket = (MSG_NAME_DWORD2*)pMsg;

			switch(pPacket->dwData1)
			{
			case RESULT_OK:			CHATMGR->AddMsg( CTC_SYSMSG, "%s님과 이혼하였습니다!!", pPacket->Name);				break;
			}
		}
		break;
	// 강제 이혼 결과 - 파트너
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
						CHATMGR->AddMsg( CTC_SYSMSG, "%s님이 강제이혼을 했습니다!!", pPacket->Name);		break;
						HERO->SetMarriagePartnerName("");
						// ..호칭을 삭제하고 캐릭터 이름으로 변경
						HERO->GetObjectBalloon()->GetOBalloonName()->SetName(HERO->GetObjectName());
					}
					else
					{
						// 주위의 이혼한 캐릭터의 호칭 삭제
						CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pPacket->dwObjectID);
						if( !pPlayer )	return;

						// ..호칭을 삭제하고 캐릭터 이름으로 변경
						pPlayer->GetObjectBalloon()->GetOBalloonName()->SetName(pPlayer->GetObjectName());
					}
				}
				break;
			}
		}
		break;
	// 호칭 변경
	case MP_MARRIAGE_NICKNAME:
		{
			MSG_DWORD_NAME* pPacket = (MSG_DWORD_NAME*)pMsg;

 			switch(pPacket->dwData)
			{
			case RESULT_OK:
				{
					if (HERO == NULL) return;

					// ..호칭 변경
					if (HERO->GetID() == pPacket->dwObjectID)
					{
						HERO->SetMarriagePartnerNickName(pPacket->Name);
						CLI_SetNickname((CPlayer*)HERO, HERO->GetMarriagePartnerName(), pPacket->Name);
					}
					else
					{
						// 주위의 캐릭터의 호칭 변경
						CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pPacket->dwObjectID);
						if( !pPlayer )	return;

						pPlayer->SetMarriagePartnerNickName(pPacket->Name);
						CLI_SetNickname(pPlayer, pPlayer->GetMarriagePartnerName(), pPacket->Name);
					}
				}
				break;
			case RESULT_FAIL_01: CHATMGR->AddMsg( CTC_SYSMSG, "아직 결혼을 안 한 상태입니다.");		break;
			}
		}
		break;
	// 배우자 위치로 텔리포트
	case MP_MARRIAGE_TELEPORT_TO_PARTNER:
		{
			MSG_DWORD* pPacket = (MSG_DWORD*)pMsg;

			switch(pPacket->dwData)
			{
			case RESULT_FAIL_01: CHATMGR->AddMsg( CTC_SYSMSG, "아직 결혼을 안 한 상태입니다.");		break;
			case RESULT_FAIL_02: CHATMGR->AddMsg( CTC_SYSMSG, "결혼 반지의 충전 횟수가 부족합니다.");		break;
			}
			break;
		}
		break;
	}
#endif
}


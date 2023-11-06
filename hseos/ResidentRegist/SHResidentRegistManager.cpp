/*********************************************************************

	 ����		: SHResidentRegist.cpp
	 �ۼ���		: hseos
	 �ۼ���		: 2007/06/04

	 ���ϼ���	: �ֹε�� Ŭ������ �ҽ�

 *********************************************************************/

#include "stdafx.h"
#include "../../hseos/SHMain.h"
#include "../../hseos/Common/SHMath.h"
#include "SHResidentRegistManager.h"
#include "MHFile.h"

#if defined(_AGENTSERVER)
	#include "Network.h"
	#include "AgentDBMsgParser.h"
	#include "AgentNetworkMsgParser.h"
#elif defined(_MAPSERVER_)
	#include "MapDBMsgParser.h"
	#include "NetWork.h"
	#include "UserTable.h"
	#include "Player.h"
	#include "PackedData.h"
	#include "Party.h"
	#include "PartyManager.h"
	#include "Itemmanager.h"
#else
	#include "ObjectManager.h"
	#include "ObjectStateManager.h"

	#include "../GameIn.h"
	#include "../DateMatchingDlg.h"
	#include "../ChatManager.h"
	#include "../WindowIDEnum.h"	
	#include "../cMsgBox.h"

	#include "../Interface/cTextArea.h"
	#include "../Interface/cWindowManager.h"
	#include "../Interface/cListDialog.h"
	#include "PartyManager.h"

	#include "NpcScriptDialog.h"
#endif

CSHResidentRegistManager g_csResidentRegistManager;

const int CSHResidentRegistManager::MAX_SERCH_NUM_PER_AGENT			= 30;
const int CSHResidentRegistManager::MAX_SERCH_NUM_CLIENT			= CSHResidentRegistManager::MAX_SERCH_NUM_PER_AGENT	*4;
const int CSHResidentRegistManager::DATE_MATCHING_RESERCH_TIME		= 4000;

// -------------------------------------------------------------------------------------------------------------------------------------
// CSHResidentRegistManager Method																								  ������
//
CSHResidentRegistManager::CSHResidentRegistManager()
{
	m_pstSerchResult = NULL;
	m_stAddInfo.m_pTblPropensity = NULL;
	m_stAddInfo.m_pTblLocation = NULL;
	m_nSerchResultNum = 0;
	LoadScriptFileData();

	#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
		m_pstSerchResult = new stSerchResult[MAX_SERCH_NUM_CLIENT];
	#endif

	ZeroMemory(&m_stChatPartner, sizeof(m_stChatPartner));
}

// -------------------------------------------------------------------------------------------------------------------------------------
// ~CSHResidentRegist Method																									  �ı���
//
CSHResidentRegistManager::~CSHResidentRegistManager()
{
	SAFE_DELETE_ARRAY(m_stAddInfo.m_pTblPropensity);
	SAFE_DELETE_ARRAY(m_stAddInfo.m_pTblLocation);
	SAFE_DELETE_ARRAY(m_stAddInfo.m_pTblSerchAgeRange);
	SAFE_DELETE_ARRAY(m_pstSerchResult);
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  LoadScriptData Method																							  ��ũ��Ʈ ���� �ε�
//
BOOL CSHResidentRegistManager::LoadScriptFileData()
{
	char szLine[MAX_PATH], szTxt[MAX_PATH];
	int	 nID = 0, nKind = 0, nIndex = 0, nMin = 0, nMax = 0;
	CMHFile fp;

	// �ִ밳���� ��� ���� ó�� �д� �κ�
	fp.Init("./System/Resource/ResidentRegist.bin", "rb");
	if(!fp.IsInited())
	{
		return FALSE;
	}

	while(TRUE)
	{
		fp.GetLine(szLine, sizeof(szLine));
		if (strstr(szLine, "//")) continue;			
		else if (strstr(szLine, "END_KIND")) 
		{
			nKind++;
			continue;
		}

		switch(nKind)
		{
		// ����
		case 0: m_stAddInfo.m_nMaxPropensity++;			break;
		// ���� ����
		case 1:	m_stAddInfo.m_nMaxLocation++;			break;
		// �˻� ���̹���
		case 2:	m_stAddInfo.m_nMaxSerchAgeRange++;		break;
		}

		if (fp.IsEOF()) break;
	}

	fp.Release();

	m_stAddInfo.m_pTblPropensity	= new WORD[m_stAddInfo.m_nMaxPropensity];
	m_stAddInfo.m_pTblLocation		= new stLocation[m_stAddInfo.m_nMaxLocation];
	m_stAddInfo.m_pTblSerchAgeRange	= new stSerchAgeRange[m_stAddInfo.m_nMaxSerchAgeRange];

	// �����͸� �б� ���� �� ��°..
	nKind = 0;
	fp.Init("./System/Resource/ResidentRegist.bin", "rb");
	if(!fp.IsInited())
	{
		return FALSE;
	}
	while(TRUE)
	{
		fp.GetLine(szLine, sizeof(szLine));
		if (strstr(szLine, "//")) continue;			
		else if (strstr(szLine, "END_KIND"))
		{
			nKind++;
			continue;
		}

		switch(nKind)
		{
		// ����
		case 0:
			sscanf(szLine, "%d %s", &nID, szTxt);
			if		(strcmp(szTxt, "HAIR_BLACK") == 0)		nIndex = PROPENSITY_KIND_HAIR_BLACK;
			else if (strcmp(szTxt, "HAIR_DYEING") == 0)		nIndex = PROPENSITY_KIND_HAIR_DYEING;
			else if (strcmp(szTxt, "CLOTHES_SUIT") == 0)	nIndex = PROPENSITY_KIND_CLOTHES_SUIT;
			else if (strcmp(szTxt, "CLOTHES_CASUAL") == 0)	nIndex = PROPENSITY_KIND_CLOTHES_CASUAL;
			else if (strcmp(szTxt, "MAKEUP") == 0)			nIndex = PROPENSITY_KIND_MAKEUP;
			else if (strcmp(szTxt, "PERFUME") == 0)			nIndex = PROPENSITY_KIND_PERFUME;
			else if (strcmp(szTxt, "BODY_THIN") == 0)		nIndex = PROPENSITY_KIND_BODY_THIN;
			else if (strcmp(szTxt, "BODY_FAT") == 0)		nIndex = PROPENSITY_KIND_BODY_FAT;
			else if (strcmp(szTxt, "BEARD") == 0)			nIndex = PROPENSITY_KIND_BEARD;
			else if (strcmp(szTxt, "GLASSES") == 0)			nIndex = PROPENSITY_KIND_GLASSES;
			else if (strcmp(szTxt, "HAT") == 0)				nIndex = PROPENSITY_KIND_HAT;
			else if (strcmp(szTxt, "ACCESSORY") == 0)		nIndex = PROPENSITY_KIND_ACCESSORY;
			else if (strcmp(szTxt, "SUMMER") == 0)			nIndex = PROPENSITY_KIND_SUMMER;
			else if (strcmp(szTxt, "WINTER") == 0)			nIndex = PROPENSITY_KIND_WINTER;
			else if (strcmp(szTxt, "BLOOD_A") == 0)			nIndex = PROPENSITY_KIND_BLOOD_A;
			else if (strcmp(szTxt, "BLOOD_B") == 0)			nIndex = PROPENSITY_KIND_BLOOD_B;
			else if (strcmp(szTxt, "BLOOD_O") == 0)			nIndex = PROPENSITY_KIND_BLOOD_O;
			else if (strcmp(szTxt, "BLOOD_AB") == 0)		nIndex = PROPENSITY_KIND_BLOOD_AB;
			else if (strcmp(szTxt, "BOTTLE_STAR") == 0)		nIndex = PROPENSITY_KIND_BOTTLE_STAR;
			else if (strcmp(szTxt, "FISH_STAR") == 0)		nIndex = PROPENSITY_KIND_FISH_STAR;
			else if (strcmp(szTxt, "SHEEP_STAR") == 0)		nIndex = PROPENSITY_KIND_SHEEP_STAR;
			else if (strcmp(szTxt, "OX_STAR") == 0)			nIndex = PROPENSITY_KIND_OX_STAR;
			else if (strcmp(szTxt, "TWINS_STAR") == 0)		nIndex = PROPENSITY_KIND_TWINS_STAR;
			else if (strcmp(szTxt, "CRAB_STAR") == 0)		nIndex = PROPENSITY_KIND_CRAB_STAR;
			else if (strcmp(szTxt, "LION_STAR") == 0)		nIndex = PROPENSITY_KIND_LION_STAR;
			else if (strcmp(szTxt, "VIRGIN_STAR") == 0)		nIndex = PROPENSITY_KIND_VIRGIN_STAR;
			else if (strcmp(szTxt, "WEIGHT_STAR") == 0)		nIndex = PROPENSITY_KIND_WEIGHT_STAR;
			else if (strcmp(szTxt, "SCORPION_STAR") == 0)	nIndex = PROPENSITY_KIND_SCORPION_STAR;
			else if (strcmp(szTxt, "ARCHER_STAR") == 0)		nIndex = PROPENSITY_KIND_ARCHER_STAR;
			else if (strcmp(szTxt, "GOAT_STAR") == 0)		nIndex = PROPENSITY_KIND_GOAT_STAR;
			else if (strcmp(szTxt, "CLEAN") == 0)			nIndex = PROPENSITY_KIND_CLEAN;
			else if (strcmp(szTxt, "COOKING") == 0)			nIndex = PROPENSITY_KIND_COOKING;
			else if (strcmp(szTxt, "PARTY") == 0)			nIndex = PROPENSITY_KIND_PARTY;
			else if (strcmp(szTxt, "LONELY") == 0)			nIndex = PROPENSITY_KIND_LONELY;
			else if (strcmp(szTxt, "SPORTS") == 0)			nIndex = PROPENSITY_KIND_SPORTS;
			else if (strcmp(szTxt, "MUSIC") == 0)			nIndex = PROPENSITY_KIND_MUSIC;
		#if defined(_DEBUG)
			else 
			{
				char szErr[2048];

				sprintf(szErr, "%s %s() %d\n�������� �ʴ� Ű�����Դϴ�.(Ű����:%s, ����:ResidentRegist.bin)", __FILE__, __FUNCTION__, __LINE__, szTxt);
				MessageBox(NULL, szErr, NULL, NULL);
			}
		#endif

			m_stAddInfo.m_pTblPropensity[nIndex] = nID;
			break;
		// ���� ����
		case 1:
			sscanf(szLine, "%d %s", &nID, szTxt);

			m_stAddInfo.m_pTblLocation[nID].nID = nID;
			SafeStrCpy(m_stAddInfo.m_pTblLocation[nID].szName, szTxt, MAX_PATH);
		#if defined(_DEBUG)
			// �ٷ� ���� �����Ϳ� ���ؼ� ID���� +1 �� ���� �ƴ϶�� ���� �޼��� ���
			if (nID > 0)
			{
				if (m_stAddInfo.m_pTblLocation[nID-1].nID != m_stAddInfo.m_pTblLocation[nID].nID-1)
				{
					char szErr[2048];

					sprintf(szErr, "%s %s() %d\n�������� ID�� �ߺ��Ǿ��ų� ���������� �ʽ��ϴ�.(����:ResidentRegist.bin)", __FILE__, __FUNCTION__, __LINE__);
					MessageBox(NULL, szErr, NULL, NULL);
				}
			}
		#endif
			break;
		// �˻� ���� ����
		case 2:
			sscanf(szLine, "%d %d %d", &nID, &nMin, &nMax);

			m_stAddInfo.m_pTblSerchAgeRange[nID].nMin = nMin;
			m_stAddInfo.m_pTblSerchAgeRange[nID].nMax = nMax;
			break;
		}

		if (fp.IsEOF()) break;
	}
	
	fp.Release();

	return TRUE;
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ASRV_CalcMatchingPoint Method																		
//
/*
UINT CSHResidentRegistManager::ASRV_CalcMatchingPoint(USERINFO* pUserInfo, USERINFO* pSerchInfo)
{
	int nMatchingPoint = 100;
	int nMinusPoint = 0;

	// ���̳��� ���̸�ŭ -, �ִ� -���� 30
	nMinusPoint += abs((int)pUserInfo->DateMatchingInfo.dwAge - (int)pSerchInfo->DateMatchingInfo.dwAge);
	if (nMinusPoint > 30) nMinusPoint = 30;

	// ���� ������ �ٸ��� -10
	if (pUserInfo->DateMatchingInfo.dwRegion != pSerchInfo->DateMatchingInfo.dwRegion)
	{
		nMinusPoint += 10;
	}

	// ���̳��� �����ϴ� ���� ������ŭ -10
	int nMaxPro = sizeof(pUserInfo->DateMatchingInfo.pdwGoodFeeling)/sizeof(pUserInfo->DateMatchingInfo.pdwGoodFeeling[0]);
	for(int i=0; i<nMaxPro; i++)
	{
		for(int j=0; j<nMaxPro; j++)
		{
			if (pUserInfo->DateMatchingInfo.pdwGoodFeeling[i] == pSerchInfo->DateMatchingInfo.pdwGoodFeeling[j])
			{
				goto NEXT1;
			}
		}
		nMinusPoint += 10;
	NEXT1:;
	}

	// ���̳��� �Ⱦ��ϴ� ���� ������ŭ -10
	nMaxPro = sizeof(pUserInfo->DateMatchingInfo.pdwBadFeeling)/sizeof(pSerchInfo->DateMatchingInfo.pdwBadFeeling[0]);
	for(int i=0; i<nMaxPro; i++)
	{
		for(int j=0; j<nMaxPro; j++)
		{
			if (pUserInfo->DateMatchingInfo.pdwBadFeeling[i] == pSerchInfo->DateMatchingInfo.pdwBadFeeling[j])
			{
				goto NEXT2;
			}
		}
		nMinusPoint += 10;
	NEXT2:;
	}

	nMatchingPoint -= nMinusPoint;
	if (nMatchingPoint < 0)	nMatchingPoint = 0;

	return nMatchingPoint;
}
*/
// -------------------------------------------------------------------------------------------------------------------------------------
//  ASRV_ParseRequestFromClient Method																 Ŭ���̾�Ʈ ��û �м�/ó��(������Ʈ)
//
VOID CSHResidentRegistManager::ASRV_ParseRequestFromClient(DWORD dwConnectionID, char* pMsg, DWORD dwLength)
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
	case MP_RESIDENTREGIST_DATEMATCHING_REQUEST_CHAT:
		{
			MSG_DWORD2* pPacket = (MSG_DWORD2*)pMsg;

			// �÷��̾ �ֹε���� �߳�?
			if (pUserInfo->DateMatchingInfo.bIsValid == FALSE)
			{
				return;
			}
			// �÷��̾ ��ȭ �����Ѱ�?
			// ..��ŷ �Ǵ� ������ ���
			if (pUserInfo->DateMatchingInfo.nRequestChatState != DATE_MATCHING_CHAT_REQUEST_STATE_NONE)
			{
				return;
			}

			// ��� �÷��̾� ã��
			USERINFO* pTargetUserInfo = g_pUserTableForObjectID->FindUser(pPacket->dwData1);
			if (pTargetUserInfo == NULL)
			{
				// ���� ���ӵǾ� ���� �ʴ�.
				pPacket->Category			= MP_RESIDENTREGIST;
				pPacket->Protocol			= MP_RESIDENTREGIST_DATEMATCHING_REQUEST_CHAT;
				pPacket->dwObjectID			= pPacket->dwObjectID;
				pPacket->dwData1			= DATE_MATCHING_CHAT_RESULT_LOGOFF;
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)pPacket, sizeof(*pPacket) );
				return;
			}

			// ��� �÷��̾ �ֹε���� �߳�?
			if (pTargetUserInfo->DateMatchingInfo.bIsValid == FALSE)
			{
				return;
			}
			// ��� �÷��̾ ��ȭ �����Ѱ�?
			if (pTargetUserInfo->DateMatchingInfo.nRequestChatState != DATE_MATCHING_CHAT_REQUEST_STATE_NONE)
			{
				// ä�� ���̰ų� ä�� ��û(����) ���̴�.
				pPacket->Category			= MP_RESIDENTREGIST;
				pPacket->Protocol			= MP_RESIDENTREGIST_DATEMATCHING_REQUEST_CHAT;
				pPacket->dwObjectID			= pPacket->dwObjectID;
				pPacket->dwData1			= DATE_MATCHING_CHAT_RESULT_BUSY;
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)pPacket, sizeof(*pPacket) );
				return;
			}

			pUserInfo->DateMatchingInfo.nRequestChatState			= DATE_MATCHING_CHAT_REQUEST_STATE_REQUEST;
			pUserInfo->DateMatchingInfo.nChatPlayerID				= pTargetUserInfo->dwCharacterID;
			pUserInfo->DateMatchingInfo.nRequestChatTimeTick		= gCurTime;
			pTargetUserInfo->DateMatchingInfo.nRequestChatState		= DATE_MATCHING_CHAT_REQUEST_STATE_RECV_REQUEST;
			pTargetUserInfo->DateMatchingInfo.nChatPlayerID			= pUserInfo->dwCharacterID;

			// Ŭ���̾�Ʈ�� �˸���
			// ..��û�ڿ��� ä�� ��û�� ������ �˸���
			pPacket->Category			= MP_RESIDENTREGIST;
			pPacket->Protocol			= MP_RESIDENTREGIST_DATEMATCHING_REQUEST_CHAT;
			pPacket->dwObjectID			= pUserInfo->dwCharacterID;
			pPacket->dwData1			= DATE_MATCHING_CHAT_RESULT_REQUEST_AND_WAIT;
			g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)pPacket, sizeof(*pPacket) );
			// ..��û���� �ʼ����� ��û���� �˸���
			pPacket->Category			= MP_RESIDENTREGIST;
			pPacket->Protocol			= MP_RESIDENTREGIST_DATEMATCHING_REQUEST_CHAT;
			pPacket->dwObjectID			= pUserInfo->dwCharacterID;
 			pPacket->dwData1			= pUserInfo->DateMatchingInfo.nRequestChatState;
			pPacket->dwData2			= gCurTime;
			g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)pPacket, sizeof(*pPacket) );
			// ..����ڿ��� �������� ä�� ��û�� ������ �˸���
			MSG_DWORD_NAME stPacket;

 			stPacket.Category			= MP_RESIDENTREGIST;
			stPacket.Protocol			= MP_RESIDENTREGIST_DATEMATCHING_REQUEST_CHAT;
			stPacket.dwObjectID			= pTargetUserInfo->dwCharacterID;
			stPacket.dwData				= DATE_MATCHING_CHAT_RESULT_REQUESTED;
			SafeStrCpy(stPacket.Name, pUserInfo->DateMatchingInfo.szName, MAX_NAME_LENGTH+1);
			g_Network.Send2User( pTargetUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );

			// ..����ڿ��� ��û���� ���� ������
			{
				PACKET_SERCH_DATE_MATCHING stPacket;
				ZeroMemory(&stPacket, sizeof(stPacket));
				stPacket.Category											= MP_RESIDENTREGIST;
				stPacket.Protocol											= MP_RESIDENTREGIST_DATEMATCHING_SOMEBODY_INFO;
				stPacket.dwObjectID											= pTargetUserInfo->dwCharacterID;		
				SafeStrCpy(stPacket.stSerchResult.stRegistInfo.szNickName, pUserInfo->DateMatchingInfo.szName, MAX_NAME_LEN+1);
				stPacket.stSerchResult.stRegistInfo.nRace					= (WORD)pUserInfo->DateMatchingInfo.nRace;
				stPacket.stSerchResult.stRegistInfo.nSex					= (WORD)pUserInfo->DateMatchingInfo.byGender;			
				stPacket.stSerchResult.stRegistInfo.nAge					= (WORD)pUserInfo->DateMatchingInfo.dwAge;				
				stPacket.stSerchResult.stRegistInfo.nLocation				= (WORD)pUserInfo->DateMatchingInfo.dwRegion;			
				stPacket.stSerchResult.stRegistInfo.nFavor					= (WORD)pUserInfo->DateMatchingInfo.dwGoodFeelingDegree;	
				stPacket.stSerchResult.stRegistInfo.nPropensityLike01		= (WORD)pUserInfo->DateMatchingInfo.pdwGoodFeeling[0];	
				stPacket.stSerchResult.stRegistInfo.nPropensityLike02		= (WORD)pUserInfo->DateMatchingInfo.pdwGoodFeeling[1];	
				stPacket.stSerchResult.stRegistInfo.nPropensityLike03		= (WORD)pUserInfo->DateMatchingInfo.pdwGoodFeeling[2];	
				stPacket.stSerchResult.stRegistInfo.nPropensityDisLike01	= (WORD)pUserInfo->DateMatchingInfo.pdwBadFeeling[0];	
				stPacket.stSerchResult.stRegistInfo.nPropensityDisLike02	= (WORD)pUserInfo->DateMatchingInfo.pdwBadFeeling[1];	
				stPacket.stSerchResult.nID									= pUserInfo->dwCharacterID;
				stPacket.stSerchResult.nMatchingPoint						= CalcMatchingPoint(&pTargetUserInfo->DateMatchingInfo, &pUserInfo->DateMatchingInfo);
				SafeStrCpy(stPacket.stSerchResult.szIntroduction, pUserInfo->DateMatchingInfo.szIntroduce, MAX_INTRODUCE_LEN+1);
				g_Network.Send2User( pTargetUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
			}
		}
		break;
	// �ٸ� ������Ʈ�� �ִ� �÷��̾�� ��ȭ ��û
	case MP_RESIDENTREGIST_DATEMATCHING_REQUEST_CHAT_DIFF_AGENT:
		{
			MSG_DWORD2* pPacket = (MSG_DWORD2*)pMsg;

			// �÷��̾ �ֹε���� �߳�?
			if (pUserInfo->DateMatchingInfo.bIsValid == FALSE)
			{
				return;
			}
			// �÷��̾ ��ȭ �����Ѱ�?
			// ..��ŷ �Ǵ� ������ ���
			if (pUserInfo->DateMatchingInfo.nRequestChatState != DATE_MATCHING_CHAT_REQUEST_STATE_NONE)
			{
				return;
			}

			// ��� �÷��̾� ������Ʈ�� Ȯ���Ϸ� ����.
			MSG_DWORD4 stPacket;

			stPacket.Category	= pPacket->Category;
			stPacket.Protocol	= MP_RESIDENTREGIST_DATEMATCHING_REQUEST_CHAT_DIFF_AGENT_TARGET_CHECK;
			stPacket.dwObjectID = pPacket->dwData2;					// ��� �÷��̾��� ������Ʈ ID
			stPacket.dwData1	= pPacket->dwData1;					// ��� �÷��̾� ID
			stPacket.dwData2	= g_pServerSystem->GetServerNum();	// ��û ������Ʈ ID - ����� ���� �� �ʿ�
			stPacket.dwData3	= pPacket->dwObjectID;				// ��û �÷��̾� ID
			g_Network.Send2SpecificAgentServer((char*)&stPacket, sizeof(stPacket) );
		}
		break;
	case MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT:
		{
			MSG_DWORD2* pPacket = (MSG_DWORD2*)pMsg;
			// ��� �÷��̾� ã��
			USERINFO* pTargetUserInfo = g_pUserTableForObjectID->FindUser(pUserInfo->DateMatchingInfo.nChatPlayerID);
			if (pTargetUserInfo == NULL)
			{
				// ��� �÷��̾ ���� ������Ʈ�� �־�� �ϴ� �� ����.
				// ..���� �ʱ�ȭ
				ASRV_InitDateMatchingChatState(pUserInfo);
				// ..��û���� �ʼ����� ��û���� �˸���
				pPacket->Category			= MP_RESIDENTREGIST;
				pPacket->Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
				pPacket->dwObjectID			= pUserInfo->dwCharacterID;
 				pPacket->dwData1			= pUserInfo->DateMatchingInfo.nRequestChatState;
				pPacket->dwData2			= gCurTime;
				g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)pPacket, sizeof(*pPacket) );
				return;
			}

			// �÷��̾ �ֹε���� �߳�?
			if (pUserInfo->DateMatchingInfo.bIsValid == FALSE)
			{
				return;
			}
			// ��� �÷��̾ �ֹε���� �߳�?
			if (pTargetUserInfo->DateMatchingInfo.bIsValid == FALSE)
			{
				return;
			}
			
			int nResult = pPacket->dwData1;
			switch(nResult)
			{
			// ��û�ڰ� ���
			case DATE_MATCHING_CHAT_RESULT_CANCEL:
				{
					// ��û���ΰ�?
					if (pUserInfo->DateMatchingInfo.nRequestChatState != DATE_MATCHING_CHAT_REQUEST_STATE_REQUEST)
					{
						return;
					}

					ASRV_InitDateMatchingChatState(pUserInfo);
					ASRV_InitDateMatchingChatState(pTargetUserInfo);
					// Ŭ���̾�Ʈ�� �˸���
					// ..����ڿ��� ��ҵǾ����� �˸���
					pPacket->Category			= MP_RESIDENTREGIST;
					pPacket->Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
					pPacket->dwObjectID			= pTargetUserInfo->dwCharacterID;
					pPacket->dwData1			= DATE_MATCHING_CHAT_RESULT_CANCEL;
					g_Network.Send2User( pTargetUserInfo->dwConnectionIndex, (char*)pPacket, sizeof(*pPacket) );
					// ..��û���� �ʼ����� ��û���� �˸���
					pPacket->Category			= MP_RESIDENTREGIST;
					pPacket->Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
					pPacket->dwObjectID			= pUserInfo->dwCharacterID;
 					pPacket->dwData1			= pUserInfo->DateMatchingInfo.nRequestChatState;
					pPacket->dwData2			= gCurTime;
					g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)pPacket, sizeof(*pPacket) );
				}
				break;
			// ������
			case DATE_MATCHING_CHAT_RESULT_REFUSE:
				{
					// ������ΰ�?
					if (pUserInfo->DateMatchingInfo.nRequestChatState != DATE_MATCHING_CHAT_REQUEST_STATE_RECV_REQUEST)
					{
						return;
					}

					ASRV_InitDateMatchingChatState(pUserInfo);
					ASRV_InitDateMatchingChatState(pTargetUserInfo);
					// Ŭ���̾�Ʈ�� �˸���
					// ..��û�ڿ��� ������������ �˸���
					pPacket->Category			= MP_RESIDENTREGIST;
					pPacket->Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
					pPacket->dwObjectID			= pTargetUserInfo->dwCharacterID;
					pPacket->dwData1			= DATE_MATCHING_CHAT_RESULT_REFUSE;
					g_Network.Send2User( pTargetUserInfo->dwConnectionIndex, (char*)pPacket, sizeof(*pPacket) );
					// ..��û���� �ʼ����� ��û���� �˸���
					pPacket->Category			= MP_RESIDENTREGIST;
					pPacket->Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
					pPacket->dwObjectID			= pTargetUserInfo->dwCharacterID;
 					pPacket->dwData1			= pTargetUserInfo->DateMatchingInfo.nRequestChatState;
					pPacket->dwData2			= gCurTime;
					g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)pPacket, sizeof(*pPacket) );
				}
				break;
			// ä�ý���
			case DATE_MATCHING_CHAT_RESULT_START_CHAT:
				{
					// ������ΰ�?
					if (pUserInfo->DateMatchingInfo.nRequestChatState != DATE_MATCHING_CHAT_REQUEST_STATE_RECV_REQUEST)
					{
						return;
					}

					pUserInfo->DateMatchingInfo.nRequestChatState		= DATE_MATCHING_CHAT_REQUEST_STATE_CHATTING;
					pTargetUserInfo->DateMatchingInfo.nRequestChatState	= DATE_MATCHING_CHAT_REQUEST_STATE_CHATTING;

					PACKET_SERCH_DATE_MATCHING stPacket;
					ZeroMemory(&stPacket, sizeof(stPacket));
					// Ŭ���̾�Ʈ�� �˸���
					// ..��û�ڿ��� ä�ý��� �˸���
					stPacket.Category											= MP_RESIDENTREGIST;
					stPacket.Protocol											= MP_RESIDENTREGIST_DATEMATCHING_RESULT_START_CHAT;
					stPacket.dwObjectID											= pTargetUserInfo->dwCharacterID;
					SafeStrCpy(stPacket.stSerchResult.stRegistInfo.szNickName, pUserInfo->DateMatchingInfo.szName, MAX_NAME_LEN+1);
					stPacket.stSerchResult.stRegistInfo.nRace					= (WORD)pUserInfo->DateMatchingInfo.nRace;
					stPacket.stSerchResult.stRegistInfo.nSex					= (WORD)pUserInfo->DateMatchingInfo.byGender;			
					stPacket.stSerchResult.stRegistInfo.nAge					= (WORD)pUserInfo->DateMatchingInfo.dwAge;				
					stPacket.stSerchResult.stRegistInfo.nLocation				= (WORD)pUserInfo->DateMatchingInfo.dwRegion;			
					stPacket.stSerchResult.stRegistInfo.nFavor					= (WORD)pUserInfo->DateMatchingInfo.dwGoodFeelingDegree;	
					stPacket.stSerchResult.stRegistInfo.nPropensityLike01		= (WORD)pUserInfo->DateMatchingInfo.pdwGoodFeeling[0];	
					stPacket.stSerchResult.stRegistInfo.nPropensityLike02		= (WORD)pUserInfo->DateMatchingInfo.pdwGoodFeeling[1];	
					stPacket.stSerchResult.stRegistInfo.nPropensityLike03		= (WORD)pUserInfo->DateMatchingInfo.pdwGoodFeeling[2];	
					stPacket.stSerchResult.stRegistInfo.nPropensityDisLike01	= (WORD)pUserInfo->DateMatchingInfo.pdwBadFeeling[0];	
					stPacket.stSerchResult.stRegistInfo.nPropensityDisLike02	= (WORD)pUserInfo->DateMatchingInfo.pdwBadFeeling[1];	
					stPacket.stSerchResult.nID									= pUserInfo->dwCharacterID;
					stPacket.stSerchResult.nMatchingPoint						= CalcMatchingPoint(&pTargetUserInfo->DateMatchingInfo, &pUserInfo->DateMatchingInfo);
					SafeStrCpy(stPacket.stSerchResult.szIntroduction, pUserInfo->DateMatchingInfo.szIntroduce, MAX_INTRODUCE_LEN+1);
					g_Network.Send2User( pTargetUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
					// ..��û���� �ʼ����� ��û���� �˸���
					pPacket->Category											= MP_RESIDENTREGIST;
					pPacket->Protocol											= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
					pPacket->dwObjectID											= pTargetUserInfo->dwCharacterID;
 					pPacket->dwData1											= pTargetUserInfo->DateMatchingInfo.nRequestChatState;
					pPacket->dwData2											= gCurTime;
					g_Network.Send2Server( pTargetUserInfo->dwMapServerConnectionIndex, (char*)pPacket, sizeof(*pPacket) );
					// ..����ڿ��� ä�ý��� �˸���
					ZeroMemory(&stPacket, sizeof(stPacket));
					stPacket.Category											= MP_RESIDENTREGIST;
					stPacket.Protocol											= MP_RESIDENTREGIST_DATEMATCHING_RESULT_START_CHAT;
					stPacket.dwObjectID											= pUserInfo->dwCharacterID;		
					SafeStrCpy(stPacket.stSerchResult.stRegistInfo.szNickName, pTargetUserInfo->DateMatchingInfo.szName, MAX_NAME_LEN+1);
					stPacket.stSerchResult.stRegistInfo.nRace					= (WORD)pTargetUserInfo->DateMatchingInfo.nRace;
					stPacket.stSerchResult.stRegistInfo.nSex					= (WORD)pTargetUserInfo->DateMatchingInfo.byGender;			
					stPacket.stSerchResult.stRegistInfo.nAge					= (WORD)pTargetUserInfo->DateMatchingInfo.dwAge;				
					stPacket.stSerchResult.stRegistInfo.nLocation				= (WORD)pTargetUserInfo->DateMatchingInfo.dwRegion;			
					stPacket.stSerchResult.stRegistInfo.nFavor					= (WORD)pTargetUserInfo->DateMatchingInfo.dwGoodFeelingDegree;	
					stPacket.stSerchResult.stRegistInfo.nPropensityLike01		= (WORD)pTargetUserInfo->DateMatchingInfo.pdwGoodFeeling[0];	
					stPacket.stSerchResult.stRegistInfo.nPropensityLike02		= (WORD)pTargetUserInfo->DateMatchingInfo.pdwGoodFeeling[1];	
					stPacket.stSerchResult.stRegistInfo.nPropensityLike03		= (WORD)pTargetUserInfo->DateMatchingInfo.pdwGoodFeeling[2];	
					stPacket.stSerchResult.stRegistInfo.nPropensityDisLike01	= (WORD)pTargetUserInfo->DateMatchingInfo.pdwBadFeeling[0];	
					stPacket.stSerchResult.stRegistInfo.nPropensityDisLike02	= (WORD)pTargetUserInfo->DateMatchingInfo.pdwBadFeeling[1];	
					stPacket.stSerchResult.nID									= pTargetUserInfo->dwCharacterID;
					stPacket.stSerchResult.nMatchingPoint						= CalcMatchingPoint(&pTargetUserInfo->DateMatchingInfo, &pUserInfo->DateMatchingInfo);
					SafeStrCpy(stPacket.stSerchResult.szIntroduction, pTargetUserInfo->DateMatchingInfo.szIntroduce, MAX_INTRODUCE_LEN+1);
					g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
				}
				break;
			// ä�ó�
			case DATE_MATCHING_CHAT_RESULT_END_CHAT:
				{
					ASRV_InitDateMatchingChatState(pUserInfo);
					// Ŭ���̾�Ʈ�� �˸���
					// ..��û��?���� ä�ó� �˸���
					pPacket->Category			= MP_RESIDENTREGIST;
					pPacket->Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
					pPacket->dwObjectID			= pUserInfo->dwCharacterID;
					pPacket->dwData1			= DATE_MATCHING_CHAT_RESULT_END_CHAT;
					pPacket->dwData2			= pTargetUserInfo->dwCharacterID;			// ���ID
					g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)pPacket, sizeof(*pPacket) );
					// ..��û���� �ʼ����� ��û���� �˸���
					pPacket->Category			= MP_RESIDENTREGIST;
					pPacket->Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
					pPacket->dwObjectID			= pUserInfo->dwCharacterID;
 					pPacket->dwData1			= pUserInfo->DateMatchingInfo.nRequestChatState;
					pPacket->dwData2			= gCurTime;
					g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)pPacket, sizeof(*pPacket) );
					if (pTargetUserInfo->DateMatchingInfo.nRequestChatState != DATE_MATCHING_CHAT_REQUEST_STATE_NONE)
					{
						ASRV_InitDateMatchingChatState(pTargetUserInfo);
						// ..�����?���� ä�ó� �˸���
						pPacket->Category			= MP_RESIDENTREGIST;
						pPacket->Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
						pPacket->dwObjectID			= pTargetUserInfo->dwCharacterID;
						pPacket->dwData1			= DATE_MATCHING_CHAT_RESULT_END_CHAT;
						pPacket->dwData2			= pUserInfo->dwCharacterID;					// ���ID
						g_Network.Send2User( pTargetUserInfo->dwConnectionIndex, (char*)pPacket, sizeof(*pPacket) );
						// ..������� �ʼ����� ��û���� �˸���
						pPacket->Category			= MP_RESIDENTREGIST;
						pPacket->Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
						pPacket->dwObjectID			= pTargetUserInfo->dwCharacterID;
 						pPacket->dwData1			= pTargetUserInfo->DateMatchingInfo.nRequestChatState;
						pPacket->dwData2			= gCurTime;
						g_Network.Send2Server( pTargetUserInfo->dwMapServerConnectionIndex, (char*)pPacket, sizeof(*pPacket) );
					}
				}
				break;
			default:
				g_Network.Send2Server( dwConnectionID, pMsg, dwLength );
			}	
		}
		break;
	// �ٸ� ������Ʈ�� �÷��̾���� ä�� ���
	case MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT_DIFF_AGENT:
		{
			MSG_DWORD2* pPacket = (MSG_DWORD2*)pMsg;

			// �÷��̾ �ֹε���� �߳�?
			if (pUserInfo->DateMatchingInfo.bIsValid == FALSE)
			{
				return;
			}
			
			int nResult = pPacket->dwData1;
			switch(nResult)
			{
			// ��û�ڰ� ���
			case DATE_MATCHING_CHAT_RESULT_CANCEL:
				{
					// ��û���ΰ�?
					if (pUserInfo->DateMatchingInfo.nRequestChatState != DATE_MATCHING_CHAT_REQUEST_STATE_REQUEST)
					{
						return;
					}

					// ����ڿ��� ��� �˸���
					// ..����� ������Ʈ�� �˸���
					MSG_DWORD2 stPacket;
					stPacket.Category			= MP_RESIDENTREGIST;
					stPacket.Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT_DIFF_AGENT;
					stPacket.dwObjectID			= pPacket->dwData2;
					stPacket.dwData1			= DATE_MATCHING_CHAT_RESULT_CANCEL;
					stPacket.dwData2			= pUserInfo->DateMatchingInfo.nChatPlayerID;
					g_Network.Send2SpecificAgentServer((char*)&stPacket, sizeof(stPacket) );
					// ..��û���� �ʼ����� ��û���� �˸���
					stPacket.Category			= MP_RESIDENTREGIST;
					stPacket.Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
					stPacket.dwObjectID			= pUserInfo->dwCharacterID;
 					stPacket.dwData1			= DATE_MATCHING_CHAT_REQUEST_STATE_NONE;
					g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)&stPacket, sizeof(stPacket) );

					ASRV_InitDateMatchingChatState(pUserInfo);
				}
				break;
			// ������
			case DATE_MATCHING_CHAT_RESULT_REFUSE:
				{
					// ������ΰ�?
					if (pUserInfo->DateMatchingInfo.nRequestChatState != DATE_MATCHING_CHAT_REQUEST_STATE_RECV_REQUEST)
					{
						return;
					}

					// ��û�ڿ��� ���� �˸���
					// ..��û�� ������Ʈ�� �˸���
					MSG_DWORD2 stPacket;
					stPacket.Category			= MP_RESIDENTREGIST;
					stPacket.Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT_DIFF_AGENT;
					stPacket.dwObjectID			= pPacket->dwData2;
					stPacket.dwData1			= DATE_MATCHING_CHAT_RESULT_REFUSE;
					stPacket.dwData2			= pUserInfo->DateMatchingInfo.nChatPlayerID;
					g_Network.Send2SpecificAgentServer((char*)&stPacket, sizeof(stPacket) );

					ASRV_InitDateMatchingChatState(pUserInfo);
				}
				break;
			// ä�ý���
			case DATE_MATCHING_CHAT_RESULT_START_CHAT:
				{
					// ������ΰ�?
					if (pUserInfo->DateMatchingInfo.nRequestChatState != DATE_MATCHING_CHAT_REQUEST_STATE_RECV_REQUEST)
					{
						return;
					}

					// ��û�ڿ��� ä�ý��� �˸���
					// ..��û�� ������Ʈ�� �˸���
					PACKET_SERCH_DATE_MATCHING stPacket;
					stPacket.Category											= MP_RESIDENTREGIST;
					stPacket.Protocol											= MP_RESIDENTREGIST_DATEMATCHING_RESULT_START_CHAT_DIFF_AGENT;
					stPacket.dwObjectID											= pPacket->dwData2;		
					stPacket.nRequestPlayerID									= pUserInfo->DateMatchingInfo.nChatPlayerID;
					SafeStrCpy(stPacket.stSerchResult.stRegistInfo.szNickName, pUserInfo->DateMatchingInfo.szName, MAX_NAME_LEN+1);
					stPacket.stSerchResult.stRegistInfo.nRace					= (WORD)pUserInfo->DateMatchingInfo.nRace;
					stPacket.stSerchResult.stRegistInfo.nSex					= (WORD)pUserInfo->DateMatchingInfo.byGender;			
					stPacket.stSerchResult.stRegistInfo.nAge					= (WORD)pUserInfo->DateMatchingInfo.dwAge;				
					stPacket.stSerchResult.stRegistInfo.nLocation				= (WORD)pUserInfo->DateMatchingInfo.dwRegion;			
					stPacket.stSerchResult.stRegistInfo.nFavor					= (WORD)pUserInfo->DateMatchingInfo.dwGoodFeelingDegree;	
					stPacket.stSerchResult.stRegistInfo.nPropensityLike01		= (WORD)pUserInfo->DateMatchingInfo.pdwGoodFeeling[0];	
					stPacket.stSerchResult.stRegistInfo.nPropensityLike02		= (WORD)pUserInfo->DateMatchingInfo.pdwGoodFeeling[1];	
					stPacket.stSerchResult.stRegistInfo.nPropensityLike03		= (WORD)pUserInfo->DateMatchingInfo.pdwGoodFeeling[2];	
					stPacket.stSerchResult.stRegistInfo.nPropensityDisLike01	= (WORD)pUserInfo->DateMatchingInfo.pdwBadFeeling[0];	
					stPacket.stSerchResult.stRegistInfo.nPropensityDisLike02	= (WORD)pUserInfo->DateMatchingInfo.pdwBadFeeling[1];	
					stPacket.stSerchResult.nID									= pUserInfo->dwCharacterID;
					stPacket.stSerchResult.nAgentID								= g_pServerSystem->GetServerNum();
					SafeStrCpy(stPacket.stSerchResult.szIntroduction, pUserInfo->DateMatchingInfo.szIntroduce, MAX_INTRODUCE_LEN+1);
					g_Network.Send2SpecificAgentServer( (char*)&stPacket, sizeof(stPacket) );

					pUserInfo->DateMatchingInfo.nRequestChatState		= DATE_MATCHING_CHAT_REQUEST_STATE_CHATTING;
				}
				break;
			// ä�ó�
			case DATE_MATCHING_CHAT_RESULT_END_CHAT:
				{
					MSG_DWORD2 stPacket;
					// ��â�ڿ��� �˸���
					// ..��û��?���� ä�ó� �˸���
					stPacket.Category			= MP_RESIDENTREGIST;
					stPacket.Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
					stPacket.dwObjectID			= pUserInfo->dwCharacterID;
					stPacket.dwData1			= DATE_MATCHING_CHAT_RESULT_END_CHAT;
					stPacket.dwData2			= pUserInfo->DateMatchingInfo.nChatPlayerID;			// ���ID
					g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
					// ..��û���� �ʼ����� ��û���� �˸���
					stPacket.Category			= MP_RESIDENTREGIST;
					stPacket.Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
					stPacket.dwObjectID			= pUserInfo->dwCharacterID;
 					stPacket.dwData1			= DATE_MATCHING_CHAT_REQUEST_STATE_NONE;
					g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)&stPacket, sizeof(stPacket) );

					// ����ڿ��� �˸���
					// ..������� ������Ʈ�� �˸���
					stPacket.Category			= MP_RESIDENTREGIST;
					stPacket.Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT_DIFF_AGENT;
					stPacket.dwObjectID			= pPacket->dwData2;
					stPacket.dwData1			= DATE_MATCHING_CHAT_RESULT_END_CHAT;
					stPacket.dwData2			= pUserInfo->DateMatchingInfo.nChatPlayerID;
					g_Network.Send2SpecificAgentServer((char*)&stPacket, sizeof(stPacket) );

					ASRV_InitDateMatchingChatState(pUserInfo);
				}
				break;
			default:
				g_Network.Send2Server( dwConnectionID, pMsg, dwLength );
			}	
		}
		break;

	// ä��
	case MP_RESIDENTREGIST_DATEMATCHING_CHAT:
		{
			MSG_DWORD2 stPacket;
			// ��û�ڰ� ä������ �÷��̾��ΰ�?
			if (pUserInfo->DateMatchingInfo.nRequestChatState != DATE_MATCHING_CHAT_REQUEST_STATE_CHATTING)
			{
				return;
			}

			// ��� �÷��̾� ã��
			USERINFO* pTargetUserInfo = g_pUserTableForObjectID->FindUser(pUserInfo->DateMatchingInfo.nChatPlayerID);
			if (pTargetUserInfo == NULL)
			{
				// ���̵� �Ǵ� ���Ӳ����� ���(�Ǵ� �ý��� ����)
				// ..ä���� ������ ������.
				ASRV_InitDateMatchingChatState(pUserInfo);
				// Ŭ���̾�Ʈ�� �˸���
				// ..��û��?���� ä�ó� �˸���
				stPacket.Category			= MP_RESIDENTREGIST;
				stPacket.Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
				stPacket.dwObjectID			= pUserInfo->dwCharacterID;
				stPacket.dwData1			= DATE_MATCHING_CHAT_RESULT_END_CHAT;
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
				// ..��û���� �ʼ����� ��û���� �˸���
				stPacket.Category			= MP_RESIDENTREGIST;
				stPacket.Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
				stPacket.dwObjectID			= pUserInfo->dwCharacterID;
 				stPacket.dwData1			= pUserInfo->DateMatchingInfo.nRequestChatState;
				g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
				return;
			}

			// ��� �÷��̾��� ä�� ��밡 ���ΰ�?
			if (pTargetUserInfo->DateMatchingInfo.nChatPlayerID != pUserInfo->dwCharacterID)
			{
				// ..ä���� ������ ������.
				ASRV_InitDateMatchingChatState(pUserInfo);
				// Ŭ���̾�Ʈ�� �˸���
				// ..��û��?���� ä�ó� �˸���
				stPacket.Category			= MP_RESIDENTREGIST;
				stPacket.Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
				stPacket.dwObjectID			= pUserInfo->dwCharacterID;
				stPacket.dwData1			= DATE_MATCHING_CHAT_RESULT_END_CHAT;
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
				// ..��û���� �ʼ����� ��û���� �˸���
				stPacket.Category			= MP_RESIDENTREGIST;
				stPacket.Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
				stPacket.dwObjectID			= pUserInfo->dwCharacterID;
 				stPacket.dwData1			= pUserInfo->DateMatchingInfo.nRequestChatState;
				g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
				return;
			}

			g_Network.Send2User( pTargetUserInfo->dwConnectionIndex, pMsg, dwLength );
		}
		break;
	// �ٸ� ������Ʈ �÷��̾�� ä��
	case MP_RESIDENTREGIST_DATEMATCHING_CHAT_DIFF_AGENT:
		{
			MSG_CHAT_WITH_SENDERID* pPacket = (MSG_CHAT_WITH_SENDERID*)pMsg;
			// ��û�ڰ� ä������ �÷��̾��ΰ�?
			if (pUserInfo->DateMatchingInfo.nRequestChatState != DATE_MATCHING_CHAT_REQUEST_STATE_CHATTING)
			{
				return;
			}

			// ����ڿ��� �޼��� ������
			// ..����� ������Ʈ�� �˸���
			pPacket->Category			= MP_RESIDENTREGIST;
			pPacket->Protocol			= MP_RESIDENTREGIST_DATEMATCHING_CHAT_DIFF_AGENT;
			pPacket->dwObjectID			= pPacket->dwSenderID;
			pPacket->dwSenderID			= pUserInfo->DateMatchingInfo.nChatPlayerID;
			g_Network.Send2SpecificAgentServer((char*)pPacket, sizeof(*pPacket) );
		}
		break;
	default:
		{
			g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, pMsg, dwLength );
		}
		break;
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ASRV_ParseRequestFromServer Method																	   ���� ��û �м�/ó��(������Ʈ)
//
VOID CSHResidentRegistManager::ASRV_ParseRequestFromServer(DWORD dwConnectionID, char* pMsg, DWORD dwLength)
{
#if defined(_AGENTSERVER)
	static const int	MAX_EACH_FIND_NUM = 5000;
	static USERINFO*	pUser[11][MAX_EACH_FIND_NUM];
	static int			nUserCnt[11];

	MSGBASE* pTempMsg = (MSGBASE*)pMsg;

	switch(pTempMsg->Protocol)
	{
	case MP_RESIDENTREGIST_REGIST_NOTIFY:
		{
			char buf[256] = {0,};

			USERINFO* pInfo = NULL;
			g_pUserTable->SetPositionUserHead();
			while( pInfo = (USERINFO *)g_pUserTable->GetUserData() )
			{	
				memcpy( buf, pMsg, dwLength );

				g_Network.Send2User( pInfo->dwConnectionIndex, buf, dwLength );	//CHATMSG 040324
			}
		}
		break;
	// �ٸ� ������Ʈ�κ����� �˻� ���
	case MP_RESIDENTREGIST_DATEMATCHING_SERCH_TO_OTHER_AGENT_RESULT:
		{
			PACKET_SERCH_DATE_MATCHING* pPacket = (PACKET_SERCH_DATE_MATCHING*)pMsg;

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pPacket->nRequestPlayerID);
			if (pUserInfo == NULL) return;

			pPacket->Category						= MP_RESIDENTREGIST;
			pPacket->Protocol						= MP_RESIDENTREGIST_DATEMATCHING_SERCH;
			pPacket->dwObjectID						= pPacket->nRequestPlayerID;
			// ���� ID�� 1�̻��� �� �ֱ⿡, +1000�� �� �༭ Ŭ���̾�Ʈ���� 1000 �̻��� �� �ٸ� ������Ʈ���� ä������ �����Ѵ�.
			pPacket->stSerchResult.nAgentID			+= 1000;
			g_Network.Send2User( pUserInfo->dwConnectionIndex, pMsg, dwLength );
		}
		break;
	// �ٸ� ������Ʈ�κ����� �˻�
	case MP_RESIDENTREGIST_DATEMATCHING_SERCH_TO_OTHER_AGENT:
		{
			PACKET_SERCH_DATE_MATCHING_TO_OTHER_AGENT* pPacket = (PACKET_SERCH_DATE_MATCHING_TO_OTHER_AGENT*)pMsg;
			PACKET_SERCH_DATE_MATCHING stPacket;

			DWORD nAgeMin	= pPacket->nAgeMin;
			DWORD nAgeMax	= pPacket->nAgeMax;
			DWORD nLocation = pPacket->nLocation;
			DWORD nSex		= pPacket->nSex;
			DWORD nGroupIndex = pPacket->nGroupIndex;

			ZeroMemory(pUser,		sizeof(pUser));
			ZeroMemory(nUserCnt,	sizeof(nUserCnt));

			// �˻�!
			USERINFO* pSerchInfo;
			g_pUserTable->SetPositionUserHead();

			int nCnt = 0;
			int nFindUserIndex = 0;
			UINT nSerchCnt = 0;
			while( pSerchInfo = g_pUserTable->GetUserData() )
			{
				// �ֹε���� ���� ���� ĳ���ʹ� �ǳʶڴ�.
				if (pSerchInfo->DateMatchingInfo.bIsValid == FALSE) continue;
				// ���� üũ
				if (pSerchInfo->DateMatchingInfo.byGender != nSex) continue;

				// ���� üũ
				if (pSerchInfo->DateMatchingInfo.dwAge < nAgeMin || pSerchInfo->DateMatchingInfo.dwAge > nAgeMax) continue;
				// ���� üũ. 0�� ��� ����
				if (nLocation != 0 && pSerchInfo->DateMatchingInfo.dwRegion != nLocation) continue;

				pSerchInfo->DateMatchingInfo.nMatchingPoint = CalcMatchingPoint(&pSerchInfo->DateMatchingInfo, &pPacket->stRegistInfo);
				nFindUserIndex = pSerchInfo->DateMatchingInfo.nMatchingPoint/10;
				if (nFindUserIndex > 10) continue;
				if (nUserCnt[nFindUserIndex] >= MAX_EACH_FIND_NUM) continue;

				nSerchCnt++;

				pUser[nFindUserIndex][nUserCnt[nFindUserIndex]] = pSerchInfo; 
				nUserCnt[nFindUserIndex]++;
			}

			// �˻� �׷� �ε����� �ִ� �׷� ���� �Ѿ�� ����
			if (nGroupIndex >= (nSerchCnt+MAX_SERCH_NUM_PER_AGENT-1) / MAX_SERCH_NUM_PER_AGENT)
			{
				return;
			}

			UINT nGroupCnt = 0;
			for(int i=10; i>=0; i--)
			{
				for(int j=0; j<nUserCnt[i]; j++)
				{
					nGroupCnt++;
					if (nGroupCnt < nGroupIndex*MAX_SERCH_NUM_PER_AGENT+1)
					{
						continue;
					}

					pSerchInfo = pUser[i][j];
					// Ŭ���̾�Ʈ�� �˸���
					stPacket.Category						= MP_RESIDENTREGIST;
					stPacket.Protocol						= MP_RESIDENTREGIST_DATEMATCHING_SERCH_TO_OTHER_AGENT_RESULT;
					// ..��û�� ������Ʈ���� dwObjectID�� �ڽ��� ID�� �ְ� BroadCast �ߴ�.
					// ..��û�� ������Ʈ�� ����� �����ֱ� ���� �� ID �� Send2SpecificAgentServer �Ѵ�.
					stPacket.dwObjectID						= pPacket->dwObjectID;

					SafeStrCpy(stPacket.stSerchResult.stRegistInfo.szNickName, pSerchInfo->DateMatchingInfo.szName, MAX_NAME_LENGTH+1);
					stPacket.stSerchResult.stRegistInfo.nRace					= (WORD)pSerchInfo->DateMatchingInfo.nRace;
					stPacket.stSerchResult.stRegistInfo.nSex					= (WORD)pSerchInfo->DateMatchingInfo.byGender;			
					stPacket.stSerchResult.stRegistInfo.nAge					= (WORD)pSerchInfo->DateMatchingInfo.dwAge;
					stPacket.stSerchResult.stRegistInfo.nLocation				= (WORD)pSerchInfo->DateMatchingInfo.dwRegion;			
					stPacket.stSerchResult.stRegistInfo.nFavor					= (WORD)pSerchInfo->DateMatchingInfo.dwGoodFeelingDegree;	
					stPacket.stSerchResult.stRegistInfo.nPropensityLike01		= (WORD)pSerchInfo->DateMatchingInfo.pdwGoodFeeling[0];	
					stPacket.stSerchResult.stRegistInfo.nPropensityLike02		= (WORD)pSerchInfo->DateMatchingInfo.pdwGoodFeeling[1];	
					stPacket.stSerchResult.stRegistInfo.nPropensityLike03		= (WORD)pSerchInfo->DateMatchingInfo.pdwGoodFeeling[2];	
					stPacket.stSerchResult.stRegistInfo.nPropensityDisLike01	= (WORD)pSerchInfo->DateMatchingInfo.pdwBadFeeling[0];	
					stPacket.stSerchResult.stRegistInfo.nPropensityDisLike02	= (WORD)pSerchInfo->DateMatchingInfo.pdwBadFeeling[1];	
					stPacket.stSerchResult.nMatchingPoint						= pSerchInfo->DateMatchingInfo.nMatchingPoint;
					stPacket.stSerchResult.nID									= pSerchInfo->dwCharacterID;
					// ..�÷��̾ ���� ������Ʈ ID ����
					stPacket.stSerchResult.nAgentID								= g_pServerSystem->GetServerNum();
					SafeStrCpy(stPacket.stSerchResult.szIntroduction, pSerchInfo->DateMatchingInfo.szIntroduce, MAX_INTRODUCE_LEN+1);
					stPacket.nGroupIndex										= nGroupIndex;
					stPacket.nGroupMax											= (nSerchCnt+MAX_SERCH_NUM_PER_AGENT-1) / MAX_SERCH_NUM_PER_AGENT;
					stPacket.nRequestPlayerID									= pPacket->nRequestPlayerID;
					g_Network.Send2SpecificAgentServer((char*)&stPacket, sizeof(stPacket) );
					// �ִ� MAX_SERCH_NUM_PER_AGENT�� ������ ó���ϵ���
					nCnt++;
					if (nCnt >= MAX_SERCH_NUM_PER_AGENT)
					{
						return;
					}
				}
			}
		}
		return;
	// �ٸ� ������Ʈ�κ����� ä�� ��û
	case MP_RESIDENTREGIST_DATEMATCHING_REQUEST_CHAT_DIFF_AGENT_TARGET_CHECK:
		{
			MSG_DWORD4* pPacket = (MSG_DWORD4*)pMsg;

			// ��� �÷��̾� ã��
			USERINFO* pTargetUserInfo = g_pUserTableForObjectID->FindUser(pPacket->dwData1);
			if (pTargetUserInfo == NULL)
			{
				// ���� ���ӵǾ� ���� �ʴ�.
				// ..��û�� ������Ʈ�� �˸���
				pPacket->Category			= MP_RESIDENTREGIST;
				pPacket->Protocol			= MP_RESIDENTREGIST_DATEMATCHING_REQUEST_CHAT_DIFF_AGENT_TARGET_CHECK_RESULT;
				pPacket->dwObjectID			= pPacket->dwData2;		// ��û ������Ʈ�� ID
				pPacket->dwData1			= pPacket->dwData3;		// ��û �÷��̾��� ID
				pPacket->dwData2			= DATE_MATCHING_CHAT_RESULT_LOGOFF;
				g_Network.Send2SpecificAgentServer( (char*)pPacket, sizeof(*pPacket) );
				return;
			}

			// ��� �÷��̾ �ֹε���� �߳�?
			if (pTargetUserInfo->DateMatchingInfo.bIsValid == FALSE)
			{
				return;
			}
			// ��� �÷��̾ ��ȭ �����Ѱ�?
			if (pTargetUserInfo->DateMatchingInfo.nRequestChatState != DATE_MATCHING_CHAT_REQUEST_STATE_NONE)
			{
				// ä�� ���̰ų� ä�� ��û(����) ���̴�.
				// ..��û�� ������Ʈ�� �˸���
				pPacket->Category			= MP_RESIDENTREGIST;
				pPacket->Protocol			= MP_RESIDENTREGIST_DATEMATCHING_REQUEST_CHAT_DIFF_AGENT_TARGET_CHECK_RESULT;
				pPacket->dwObjectID			= pPacket->dwData2;		// ��û ������Ʈ�� ID
				pPacket->dwData1			= pPacket->dwData3;		// ��û �÷��̾��� ID
				pPacket->dwData2			= DATE_MATCHING_CHAT_RESULT_BUSY;
				g_Network.Send2SpecificAgentServer( (char*)pPacket, sizeof(*pPacket) );
				return;
			}

			// ��û�ڿ��� ��û ������ �˸���
			// ..��û�� ������Ʈ�� �˸���
			MSG_DWORD4 stPacket;
			stPacket.Category			= MP_RESIDENTREGIST;
			stPacket.Protocol			= MP_RESIDENTREGIST_DATEMATCHING_REQUEST_CHAT_DIFF_AGENT_TARGET_CHECK_RESULT;
			stPacket.dwObjectID			= pPacket->dwData2;			// ��û ������Ʈ�� ID
			stPacket.dwData1			= pPacket->dwData3;			// ��û �÷��̾��� ID
			stPacket.dwData2			= DATE_MATCHING_CHAT_RESULT_REQUEST_AND_WAIT;
			stPacket.dwData3			= g_pServerSystem->GetServerNum();
			stPacket.dwData4			= pTargetUserInfo->dwCharacterID;
			g_Network.Send2SpecificAgentServer( (char*)&stPacket, sizeof(stPacket) );

			// ����� ���� ����
			pTargetUserInfo->DateMatchingInfo.nRequestChatState		= DATE_MATCHING_CHAT_REQUEST_STATE_RECV_REQUEST;
			pTargetUserInfo->DateMatchingInfo.nChatPlayerID			= pPacket->dwData3;
			pTargetUserInfo->DateMatchingInfo.nChatPlayerAgentID	= pPacket->dwData2+1000;
		}
		return;
	// �ٸ� ������Ʈ�� �ִ� �÷��̾�� ��ȭ ��û �� ��� �ޱ�
	case MP_RESIDENTREGIST_DATEMATCHING_REQUEST_CHAT_DIFF_AGENT_TARGET_CHECK_RESULT:
		{
			MSG_DWORD4* pPacket = (MSG_DWORD4*)pMsg;

			// ��û�� ã��
			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pPacket->dwData1);
			if (pUserInfo == NULL) return;

			// �÷��̾ �ֹε���� �߳�?
			if (pUserInfo->DateMatchingInfo.bIsValid == FALSE)
			{
				return;
			}
			// �÷��̾ ��ȭ �����Ѱ�?
			// ..��ŷ �Ǵ� ������ ���
			if (pUserInfo->DateMatchingInfo.nRequestChatState != DATE_MATCHING_CHAT_REQUEST_STATE_NONE)
			{
				return;
			}

			// Ŭ���̾�Ʈ�� ��� �˸���
			MSG_DWORD2 stPacket;
			stPacket.Category			= MP_RESIDENTREGIST;
			stPacket.Protocol			= MP_RESIDENTREGIST_DATEMATCHING_REQUEST_CHAT;
			stPacket.dwObjectID			= pUserInfo->dwCharacterID;
			stPacket.dwData1			= pPacket->dwData2;
			g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );

			// ��û�Ǿ��ٸ� ���� ���� �� ����ڿ��� ��û���� ���� ������
			// ..����� ������Ʈ�� �˸���
			if (pPacket->dwData2 == DATE_MATCHING_CHAT_RESULT_REQUEST_AND_WAIT)
			{
				pUserInfo->DateMatchingInfo.nRequestChatState		= DATE_MATCHING_CHAT_REQUEST_STATE_REQUEST;
				pUserInfo->DateMatchingInfo.nChatPlayerID			= pPacket->dwData4;
				pUserInfo->DateMatchingInfo.nChatPlayerAgentID		= pPacket->dwData3+1000;

				PACKET_SERCH_DATE_MATCHING stPacket;

				stPacket.Category											= MP_RESIDENTREGIST;
				stPacket.Protocol											= MP_RESIDENTREGIST_DATEMATCHING_REQUEST_CHAT_DIFF_AGENT_SEND_REQUESTER_INFO;
				stPacket.dwObjectID											= pPacket->dwData3;				// ����� ������Ʈ�� ID
				stPacket.nRequestPlayerID									= pPacket->dwData4;				// ����� �÷��̾� ID
				SafeStrCpy(stPacket.stSerchResult.stRegistInfo.szNickName, pUserInfo->DateMatchingInfo.szName, MAX_NAME_LEN+1);
				stPacket.stSerchResult.stRegistInfo.nRace					= (WORD)pUserInfo->DateMatchingInfo.nRace;
				stPacket.stSerchResult.stRegistInfo.nSex					= (WORD)pUserInfo->DateMatchingInfo.byGender;			
				stPacket.stSerchResult.stRegistInfo.nAge					= (WORD)pUserInfo->DateMatchingInfo.dwAge;				
				stPacket.stSerchResult.stRegistInfo.nLocation				= (WORD)pUserInfo->DateMatchingInfo.dwRegion;			
				stPacket.stSerchResult.stRegistInfo.nFavor					= (WORD)pUserInfo->DateMatchingInfo.dwGoodFeelingDegree;	
				stPacket.stSerchResult.stRegistInfo.nPropensityLike01		= (WORD)pUserInfo->DateMatchingInfo.pdwGoodFeeling[0];	
				stPacket.stSerchResult.stRegistInfo.nPropensityLike02		= (WORD)pUserInfo->DateMatchingInfo.pdwGoodFeeling[1];	
				stPacket.stSerchResult.stRegistInfo.nPropensityLike03		= (WORD)pUserInfo->DateMatchingInfo.pdwGoodFeeling[2];	
				stPacket.stSerchResult.stRegistInfo.nPropensityDisLike01	= (WORD)pUserInfo->DateMatchingInfo.pdwBadFeeling[0];	
				stPacket.stSerchResult.stRegistInfo.nPropensityDisLike02	= (WORD)pUserInfo->DateMatchingInfo.pdwBadFeeling[1];	
				stPacket.stSerchResult.nID									= pUserInfo->dwCharacterID;
				stPacket.stSerchResult.nAgentID								= g_pServerSystem->GetServerNum();

				SafeStrCpy(stPacket.stSerchResult.szIntroduction, pUserInfo->DateMatchingInfo.szIntroduce, MAX_INTRODUCE_LEN+1);
				g_Network.Send2SpecificAgentServer( (char*)&stPacket, sizeof(stPacket) );
			}

			// �ʼ����� �˸���
			// ..��û���� �ʼ����� ��û���� �˸���
			stPacket.Category			= MP_RESIDENTREGIST;
			stPacket.Protocol			= MP_RESIDENTREGIST_DATEMATCHING_REQUEST_CHAT;
			stPacket.dwObjectID			= pUserInfo->dwCharacterID;
 			stPacket.dwData1			= pUserInfo->DateMatchingInfo.nRequestChatState;
			g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
		}
		return;
	// �ٸ� ������Ʈ�� �ִ� �÷��̾�� ��ȭ ��û ���� �� ��û�� ���� �ޱ�
	case MP_RESIDENTREGIST_DATEMATCHING_REQUEST_CHAT_DIFF_AGENT_SEND_REQUESTER_INFO:
		{
			PACKET_SERCH_DATE_MATCHING* pPacket = (PACKET_SERCH_DATE_MATCHING*)pMsg;

			// ����� ã��
			USERINFO* pTargetUserInfo = g_pUserTableForObjectID->FindUser(pPacket->nRequestPlayerID);
			if (pTargetUserInfo == NULL) return;

			// �÷��̾ �ֹε���� �߳�?
			if (pTargetUserInfo->DateMatchingInfo.bIsValid == FALSE)
			{
				return;
			}

			// ��û�� ���� �����ΰ�?
			if (pTargetUserInfo->DateMatchingInfo.nRequestChatState != DATE_MATCHING_CHAT_REQUEST_STATE_RECV_REQUEST)
			{
				return;
			}

			// ����ڿ��� �������� ä�� ��û�� ������ �˸���
			MSG_DWORD_NAME stPacket;
 			stPacket.Category			= MP_RESIDENTREGIST;
			stPacket.Protocol			= MP_RESIDENTREGIST_DATEMATCHING_REQUEST_CHAT;
			stPacket.dwObjectID			= pTargetUserInfo->dwCharacterID;
			stPacket.dwData				= DATE_MATCHING_CHAT_RESULT_REQUESTED;
			SafeStrCpy(stPacket.Name, pPacket->stSerchResult.stRegistInfo.szNickName, MAX_NAME_LENGTH+1);
			g_Network.Send2User( pTargetUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );

			// ��û�� ������Ʈ���� ���� ������ �÷��̾�ID�� ��Ī����Ʈ�� �����ϰ� Ŭ���̾�Ʈ�� ����
			pPacket->Protocol						= MP_RESIDENTREGIST_DATEMATCHING_SOMEBODY_INFO;
			pPacket->dwObjectID						= pTargetUserInfo->dwCharacterID;
			pPacket->stSerchResult.nMatchingPoint	= CalcMatchingPoint(&pTargetUserInfo->DateMatchingInfo, &pPacket->stSerchResult.stRegistInfo);
			pPacket->stSerchResult.nAgentID			+= 1000;
			g_Network.Send2User( pTargetUserInfo->dwConnectionIndex, (char*)pPacket, sizeof(*pPacket) );
		}
		return;
	// �ٸ� ������Ʈ�� �ִ� �÷��̾�� ��ȭ ��û �� ���� ���
	case MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT_DIFF_AGENT:
		{
			MSG_DWORD2* pPacket = (MSG_DWORD2*)pMsg;

			// ����� ã��
			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pPacket->dwData2);
			if (pUserInfo == NULL) return;

			// �÷��̾ �ֹε���� �߳�?
			if (pUserInfo->DateMatchingInfo.bIsValid == FALSE)
			{
				return;
			}
			
			int nResult = pPacket->dwData1;
			switch(nResult)
			{
			// ��û�ڰ� ���
			case DATE_MATCHING_CHAT_RESULT_CANCEL:
				{
					// ������ΰ�?
					if (pUserInfo->DateMatchingInfo.nRequestChatState != DATE_MATCHING_CHAT_REQUEST_STATE_RECV_REQUEST)
					{
						return;
					}

					// ����ڿ��� ��� �˸���
					MSG_DWORD2 stPacket;
					stPacket.Category			= MP_RESIDENTREGIST;
					stPacket.Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
					stPacket.dwObjectID			= pPacket->dwData2;
					stPacket.dwData1			= DATE_MATCHING_CHAT_RESULT_CANCEL;
					stPacket.dwData2			= pUserInfo->DateMatchingInfo.nChatPlayerID;
					g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );

					ASRV_InitDateMatchingChatState(pUserInfo);
				}
				break;
			// ������
			case DATE_MATCHING_CHAT_RESULT_REFUSE:
				{
					// ��û���ΰ�?
					if (pUserInfo->DateMatchingInfo.nRequestChatState != DATE_MATCHING_CHAT_REQUEST_STATE_REQUEST)
					{
						return;
					}

					// ��û�ڿ��� ���� �˸���
					MSG_DWORD2 stPacket;
					stPacket.Category			= MP_RESIDENTREGIST;
					stPacket.Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
					stPacket.dwObjectID			= pPacket->dwData2;
					stPacket.dwData1			= DATE_MATCHING_CHAT_RESULT_REFUSE;
					stPacket.dwData2			= pUserInfo->DateMatchingInfo.nChatPlayerID;
					g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );

					ASRV_InitDateMatchingChatState(pUserInfo);
				}
				break;
			// ä�� ��
			case DATE_MATCHING_CHAT_RESULT_END_CHAT:
				{
					MSG_DWORD2 stPacket;
					// ����ڿ��� �˸���
					// ..����ڿ��� ä�ó� �˸���
					stPacket.Category			= MP_RESIDENTREGIST;
					stPacket.Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
					stPacket.dwObjectID			= pUserInfo->dwCharacterID;
					stPacket.dwData1			= DATE_MATCHING_CHAT_RESULT_END_CHAT;
					stPacket.dwData2			= pUserInfo->DateMatchingInfo.nChatPlayerID;			// ���ID
					g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
					// ..������� �ʼ����� ��û���� �˸���
					stPacket.Category			= MP_RESIDENTREGIST;
					stPacket.Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
					stPacket.dwObjectID			= pUserInfo->dwCharacterID;
 					stPacket.dwData1			= DATE_MATCHING_CHAT_REQUEST_STATE_NONE;
					g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)&stPacket, sizeof(stPacket) );

					ASRV_InitDateMatchingChatState(pUserInfo);
				}
				break;
			// �ð��ʰ�
			case DATE_MATCHING_CHAT_RESULT_TIMEOUT:
				{
					ASRV_InitDateMatchingChatState(pUserInfo);
					// ����ڿ��� �˸���
					// ..����ڿ��� �ð��ʰ� �Ǿ����� �˸���
					pPacket->Category			= MP_RESIDENTREGIST;
					pPacket->Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
					pPacket->dwObjectID			= pUserInfo->dwCharacterID;
					pPacket->dwData1			= DATE_MATCHING_CHAT_RESULT_TIMEOUT;
					g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)pPacket, sizeof(*pPacket) );
				}
				break;
			}
		}
		return;
	// ä�ý���
	case MP_RESIDENTREGIST_DATEMATCHING_RESULT_START_CHAT_DIFF_AGENT:
		{
			PACKET_SERCH_DATE_MATCHING* pPacket = (PACKET_SERCH_DATE_MATCHING*)pMsg;

			// ��û�� ã��
			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pPacket->nRequestPlayerID);
			if (pUserInfo == NULL) return;

			// �÷��̾ �ֹε���� �߳�?
			if (pUserInfo->DateMatchingInfo.bIsValid == FALSE)
			{
				return;
			}

			if (pUserInfo->DateMatchingInfo.nRequestChatState != DATE_MATCHING_CHAT_REQUEST_STATE_CHATTING)
			{
				// ����ڿ��� ä�ý��� �˸���
				// ..����� ������Ʈ�� �˸���
				PACKET_SERCH_DATE_MATCHING stPacket;
				stPacket = *pPacket;
				SafeStrCpy(stPacket.stSerchResult.stRegistInfo.szNickName, pUserInfo->DateMatchingInfo.szName, MAX_NAME_LEN+1);
				stPacket.stSerchResult.stRegistInfo.nRace					= (WORD)pUserInfo->DateMatchingInfo.nRace;
				stPacket.stSerchResult.stRegistInfo.nSex					= (WORD)pUserInfo->DateMatchingInfo.byGender;			
				stPacket.stSerchResult.stRegistInfo.nAge					= (WORD)pUserInfo->DateMatchingInfo.dwAge;				
				stPacket.stSerchResult.stRegistInfo.nLocation				= (WORD)pUserInfo->DateMatchingInfo.dwRegion;			
				stPacket.stSerchResult.stRegistInfo.nFavor					= (WORD)pUserInfo->DateMatchingInfo.dwGoodFeelingDegree;	
				stPacket.stSerchResult.stRegistInfo.nPropensityLike01		= (WORD)pUserInfo->DateMatchingInfo.pdwGoodFeeling[0];	
				stPacket.stSerchResult.stRegistInfo.nPropensityLike02		= (WORD)pUserInfo->DateMatchingInfo.pdwGoodFeeling[1];	
				stPacket.stSerchResult.stRegistInfo.nPropensityLike03		= (WORD)pUserInfo->DateMatchingInfo.pdwGoodFeeling[2];	
				stPacket.stSerchResult.stRegistInfo.nPropensityDisLike01	= (WORD)pUserInfo->DateMatchingInfo.pdwBadFeeling[0];	
				stPacket.stSerchResult.stRegistInfo.nPropensityDisLike02	= (WORD)pUserInfo->DateMatchingInfo.pdwBadFeeling[1];	
				SafeStrCpy(stPacket.stSerchResult.szIntroduction, pUserInfo->DateMatchingInfo.szIntroduce, MAX_INTRODUCE_LEN+1);

				stPacket.dwObjectID											= pPacket->stSerchResult.nAgentID;		
				stPacket.nRequestPlayerID									= pPacket->stSerchResult.nID;
				stPacket.stSerchResult.nID									= pUserInfo->dwCharacterID;
				stPacket.stSerchResult.nAgentID								= g_pServerSystem->GetServerNum();
				g_Network.Send2SpecificAgentServer( (char*)&stPacket, sizeof(stPacket) );
			}

			// ��û�ڿ��� ä�ý��� �˸���
			pPacket->Category						= MP_RESIDENTREGIST;
			pPacket->Protocol						= MP_RESIDENTREGIST_DATEMATCHING_RESULT_START_CHAT;
			pPacket->dwObjectID						= pUserInfo->dwCharacterID;
			pPacket->stSerchResult.nMatchingPoint	= CalcMatchingPoint(&pUserInfo->DateMatchingInfo, &pPacket->stSerchResult.stRegistInfo);
			pPacket->stSerchResult.nAgentID			+= 1000;
			g_Network.Send2User( pUserInfo->dwConnectionIndex, pMsg, dwLength );
			// ��û���� �ʼ����� ��û���� �˸���
			MSG_DWORD2 stPacket;
			stPacket.Category						= MP_RESIDENTREGIST;
			stPacket.Protocol						= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
			stPacket.dwObjectID						= pUserInfo->dwCharacterID;
 			stPacket.dwData1						= DATE_MATCHING_CHAT_REQUEST_STATE_CHATTING;
			g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)&stPacket, sizeof(stPacket) );

			pUserInfo->DateMatchingInfo.nRequestChatState		= DATE_MATCHING_CHAT_REQUEST_STATE_CHATTING;
		}
		return;
	// �ٸ� ������Ʈ �÷��̾�� ä��
	case MP_RESIDENTREGIST_DATEMATCHING_CHAT_DIFF_AGENT:
		{
			MSG_CHAT_WITH_SENDERID* pPacket = (MSG_CHAT_WITH_SENDERID*)pMsg;

			// ����� ã��
			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pPacket->dwSenderID);
			if (pUserInfo == NULL) return;

			// ����ڰ� ä������ �÷��̾��ΰ�?
			if (pUserInfo->DateMatchingInfo.nRequestChatState != DATE_MATCHING_CHAT_REQUEST_STATE_CHATTING)
			{
				return;
			}

			// ����ڿ��� �޼��� ������
			pPacket->Protocol = MP_RESIDENTREGIST_DATEMATCHING_CHAT;
			g_Network.Send2User( pUserInfo->dwConnectionIndex, pMsg, dwLength );
		}
		return;
	}

	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pTempMsg->dwObjectID);
	if (pUserInfo == NULL) return;

	switch( pTempMsg->Protocol )
	{
	case MP_RESIDENTREGIST_REGIST:
		{
			PACKET_RESIDENTREGIST_REGIST* pPacket = (PACKET_RESIDENTREGIST_REGIST*)pMsg;

			SafeStrCpy(pUserInfo->DateMatchingInfo.szName, pPacket->stInfo.szNickName, MAX_NAME_LENGTH+1);
			pUserInfo->DateMatchingInfo.bIsValid			= TRUE;
			pUserInfo->DateMatchingInfo.byGender			= (BYTE)pPacket->stInfo.nSex;
			pUserInfo->DateMatchingInfo.dwAge				= pPacket->stInfo.nAge;
			pUserInfo->DateMatchingInfo.dwRegion			= pPacket->stInfo.nLocation;
			pUserInfo->DateMatchingInfo.dwGoodFeelingDegree	= pPacket->stInfo.nFavor;
			pUserInfo->DateMatchingInfo.pdwGoodFeeling[0]	= pPacket->stInfo.nPropensityLike01;
			pUserInfo->DateMatchingInfo.pdwGoodFeeling[1]	= pPacket->stInfo.nPropensityLike02;
			pUserInfo->DateMatchingInfo.pdwGoodFeeling[2]	= pPacket->stInfo.nPropensityLike03;
			pUserInfo->DateMatchingInfo.pdwBadFeeling[0]	= pPacket->stInfo.nPropensityDisLike01;
			pUserInfo->DateMatchingInfo.pdwBadFeeling[1]	= pPacket->stInfo.nPropensityDisLike02;

			g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)pPacket, sizeof(*pPacket) );
		}
		break;
	case MP_RESIDENTREGIST_UPDATE_INTRODUCTION:
		{
			PACKET_RESIDENTREGIST_UPDATE_INTRODUCTION* pPacket = (PACKET_RESIDENTREGIST_UPDATE_INTRODUCTION*)pMsg;

			SafeStrCpy(pUserInfo->DateMatchingInfo.szIntroduce, pPacket->szIntroduction, MAX_INTRODUCE_LEN+1);

			g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)pPacket, sizeof(*pPacket) );
		}
		break;
	case MP_RESIDENTREGIST_DATEMATCHING_SERCH:
		{
			MSG_DWORD5* pPacket = (MSG_DWORD5*)pMsg;   
			PACKET_SERCH_DATE_MATCHING stPacket;

			DWORD nAgeMin	= pPacket->dwData1;
			DWORD nAgeMax	= pPacket->dwData2;
			DWORD nLocation = pPacket->dwData3;
			DWORD nSex		= pPacket->dwData4;
			DWORD nGroupIndex = pPacket->dwData5;

			ZeroMemory(pUser,		sizeof(pUser));
			ZeroMemory(nUserCnt,	sizeof(nUserCnt));

			// �˻�!
			USERINFO* pSerchInfo;
			g_pUserTable->SetPositionUserHead();

			int nCnt = 0;
			int nFindUserIndex = 0;
			UINT nSerchCnt = 0;
			while( pSerchInfo = g_pUserTable->GetUserData() )
			{
				// �ֹε���� ���� ���� ĳ���ʹ� �ǳʶڴ�.
				if (pSerchInfo->DateMatchingInfo.bIsValid == FALSE) continue;
				// �ڽŵ� �ǳʶڴ�.
				if (pSerchInfo->dwCharacterID == pUserInfo->dwCharacterID) continue;
				// ���� üũ
				if (pSerchInfo->DateMatchingInfo.byGender != nSex) continue;

				// ���� üũ
				if (pSerchInfo->DateMatchingInfo.dwAge < nAgeMin || pSerchInfo->DateMatchingInfo.dwAge > nAgeMax) continue;
				// ���� üũ. 0�� ��� ����
				if (nLocation != 0 && pSerchInfo->DateMatchingInfo.dwRegion != nLocation) continue;

				pSerchInfo->DateMatchingInfo.nMatchingPoint = CalcMatchingPoint(&pSerchInfo->DateMatchingInfo, &pUserInfo->DateMatchingInfo);
				nFindUserIndex = pSerchInfo->DateMatchingInfo.nMatchingPoint/10;
				if (nFindUserIndex > 10) continue;
				if (nUserCnt[nFindUserIndex] >= MAX_EACH_FIND_NUM) continue;

				nSerchCnt++;

				pUser[nFindUserIndex][nUserCnt[nFindUserIndex]] = pSerchInfo; 
				nUserCnt[nFindUserIndex]++;
			}

			// �˻� �׷� �ε����� �ִ� �׷� ���� �Ѿ�� ����
			if (nGroupIndex >= (nSerchCnt+MAX_SERCH_NUM_PER_AGENT-1) / MAX_SERCH_NUM_PER_AGENT)
			{
				goto STOP_SERCH;
			}

			UINT nGroupCnt = 0;
			for(int i=10; i>=0; i--)
			{
				for(int j=0; j<nUserCnt[i]; j++)
				{
					nGroupCnt++;
					if (nGroupCnt < nGroupIndex*MAX_SERCH_NUM_PER_AGENT+1)
					{
						continue;
					}

					pSerchInfo = pUser[i][j];
					// Ŭ���̾�Ʈ�� �˸���
					stPacket.Category						= MP_RESIDENTREGIST;
					stPacket.Protocol						= MP_RESIDENTREGIST_DATEMATCHING_SERCH;
					stPacket.dwObjectID						= pTempMsg->dwObjectID;

					SafeStrCpy(stPacket.stSerchResult.stRegistInfo.szNickName, pSerchInfo->DateMatchingInfo.szName, MAX_NAME_LENGTH+1);
					stPacket.stSerchResult.stRegistInfo.nRace					= (WORD)pSerchInfo->DateMatchingInfo.nRace;
					stPacket.stSerchResult.stRegistInfo.nSex					= (WORD)pSerchInfo->DateMatchingInfo.byGender;			
					stPacket.stSerchResult.stRegistInfo.nAge					= (WORD)pSerchInfo->DateMatchingInfo.dwAge;
					stPacket.stSerchResult.stRegistInfo.nLocation				= (WORD)pSerchInfo->DateMatchingInfo.dwRegion;			
					stPacket.stSerchResult.stRegistInfo.nFavor					= (WORD)pSerchInfo->DateMatchingInfo.dwGoodFeelingDegree;	
					stPacket.stSerchResult.stRegistInfo.nPropensityLike01		= (WORD)pSerchInfo->DateMatchingInfo.pdwGoodFeeling[0];	
					stPacket.stSerchResult.stRegistInfo.nPropensityLike02		= (WORD)pSerchInfo->DateMatchingInfo.pdwGoodFeeling[1];	
					stPacket.stSerchResult.stRegistInfo.nPropensityLike03		= (WORD)pSerchInfo->DateMatchingInfo.pdwGoodFeeling[2];	
					stPacket.stSerchResult.stRegistInfo.nPropensityDisLike01	= (WORD)pSerchInfo->DateMatchingInfo.pdwBadFeeling[0];	
					stPacket.stSerchResult.stRegistInfo.nPropensityDisLike02	= (WORD)pSerchInfo->DateMatchingInfo.pdwBadFeeling[1];	
					stPacket.stSerchResult.nMatchingPoint						= pSerchInfo->DateMatchingInfo.nMatchingPoint;
					stPacket.stSerchResult.nID									= pSerchInfo->dwCharacterID;
					stPacket.stSerchResult.nAgentID								= 0;
					SafeStrCpy(stPacket.stSerchResult.szIntroduction, pSerchInfo->DateMatchingInfo.szIntroduce, MAX_INTRODUCE_LEN+1);
					stPacket.nGroupIndex										= nGroupIndex;
					stPacket.nGroupMax											= (nSerchCnt+MAX_SERCH_NUM_PER_AGENT-1) / MAX_SERCH_NUM_PER_AGENT;
					stPacket.nRequestPlayerID									= 0;
					g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
					// �ִ� MAX_SERCH_NUM_PER_AGENT�� ������ ó���ϵ���
					nCnt++;
					if (nCnt >= MAX_SERCH_NUM_PER_AGENT) goto STOP_SERCH;
				}
			}
STOP_SERCH:
			// �ٸ� ������Ʈ�� �˸���
			{
				PACKET_SERCH_DATE_MATCHING_TO_OTHER_AGENT stPacket;

				stPacket.Category						= MP_RESIDENTREGIST;
				stPacket.Protocol						= MP_RESIDENTREGIST_DATEMATCHING_SERCH_TO_OTHER_AGENT;
				stPacket.dwObjectID						= g_pServerSystem->GetServerNum();
				stPacket.nAgeMin						= nAgeMin;
				stPacket.nAgeMax						= nAgeMax;
				stPacket.nLocation						= nLocation;
				stPacket.nSex							= nSex;
				stPacket.nGroupIndex					= nGroupIndex;
				stPacket.nRequestPlayerID				= pUserInfo->dwCharacterID;
				SafeStrCpy(stPacket.stRegistInfo.szNickName, pUserInfo->DateMatchingInfo.szName, MAX_NAME_LENGTH+1);
				stPacket.stRegistInfo.nRace					= (WORD)pUserInfo->DateMatchingInfo.nRace;
				stPacket.stRegistInfo.nSex					= (WORD)pUserInfo->DateMatchingInfo.byGender;			
				stPacket.stRegistInfo.nAge					= (WORD)pUserInfo->DateMatchingInfo.dwAge;				
				stPacket.stRegistInfo.nLocation				= (WORD)pUserInfo->DateMatchingInfo.dwRegion;			
				stPacket.stRegistInfo.nFavor				= (WORD)pUserInfo->DateMatchingInfo.dwGoodFeelingDegree;	
				stPacket.stRegistInfo.nPropensityLike01		= (WORD)pUserInfo->DateMatchingInfo.pdwGoodFeeling[0];	
				stPacket.stRegistInfo.nPropensityLike02		= (WORD)pUserInfo->DateMatchingInfo.pdwGoodFeeling[1];	
				stPacket.stRegistInfo.nPropensityLike03		= (WORD)pUserInfo->DateMatchingInfo.pdwGoodFeeling[2];	
				stPacket.stRegistInfo.nPropensityDisLike01	= (WORD)pUserInfo->DateMatchingInfo.pdwBadFeeling[0];	
				stPacket.stRegistInfo.nPropensityDisLike02	= (WORD)pUserInfo->DateMatchingInfo.pdwBadFeeling[1];	
				g_Network.Broadcast2AgentServerExceptSelf((char*)&stPacket, sizeof(stPacket));
			}
		}
		break;
	case MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT:
		{
 			MSG_DWORD2* pPacket = (MSG_DWORD2*)pMsg;

			// �÷��̾ �ֹε���� �߳�?
			if (pUserInfo->DateMatchingInfo.bIsValid == FALSE)
			{
				return;
			}

			// ��� �÷��̾ �ٸ� ������Ʈ�� �ֳ�?
			if (pUserInfo->DateMatchingInfo.nChatPlayerAgentID)
			{
				int nResult = pPacket->dwData1;
				switch(nResult)
				{
				// �ð��ʰ�
				case DATE_MATCHING_CHAT_RESULT_TIMEOUT:
					{
						// Ŭ���̾�Ʈ�� �˸���
						// ..��û�ڿ��� �ð��ʰ� �Ǿ����� �˸���
						pPacket->Category			= MP_RESIDENTREGIST;
						pPacket->Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
						pPacket->dwObjectID			= pUserInfo->dwCharacterID;
						pPacket->dwData1			= DATE_MATCHING_CHAT_RESULT_TIMEOUT;
						g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)pPacket, sizeof(*pPacket) );
						// ����ڿ��� �ð��ʰ� �Ǿ����� �˸���
						// ..����� ������Ʈ�� �˸���
						MSG_DWORD2 stPacket;
						stPacket.Category			= MP_RESIDENTREGIST;
						stPacket.Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT_DIFF_AGENT;
						stPacket.dwObjectID			= pUserInfo->DateMatchingInfo.nChatPlayerAgentID-1000;
						stPacket.dwData1			= DATE_MATCHING_CHAT_RESULT_TIMEOUT;
						stPacket.dwData2			= pUserInfo->DateMatchingInfo.nChatPlayerID;			// ���ID
						g_Network.Send2SpecificAgentServer((char*)&stPacket, sizeof(stPacket) );

						ASRV_InitDateMatchingChatState(pUserInfo);
					}
					break;
				}

				return;
			}

			// ��� �÷��̾� ã��
			USERINFO* pTargetUserInfo = g_pUserTableForObjectID->FindUser(pUserInfo->DateMatchingInfo.nChatPlayerID);
			if (pTargetUserInfo == NULL)
			{
				ASRV_InitDateMatchingChatState(pUserInfo);
				// ..��û���� �ʼ����� ��û���� �˸���
				pPacket->Category			= MP_RESIDENTREGIST;
				pPacket->Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
				pPacket->dwObjectID			= pUserInfo->dwCharacterID;
 				pPacket->dwData1			= pUserInfo->DateMatchingInfo.nRequestChatState;
				pPacket->dwData2			= gCurTime;
				g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)pPacket, sizeof(*pPacket) );
				return;
			}

			// ��� �÷��̾ �ֹε���� �߳�?
			if (pTargetUserInfo->DateMatchingInfo.bIsValid == FALSE)
			{
				return;
			}
			
			int nResult = pPacket->dwData1;
			switch(nResult)
			{
			// �ð��ʰ�
			case DATE_MATCHING_CHAT_RESULT_TIMEOUT:
				{
					ASRV_InitDateMatchingChatState(pUserInfo);
					ASRV_InitDateMatchingChatState(pTargetUserInfo);
					// Ŭ���̾�Ʈ�� �˸���
					// ..��û�ڿ��� �ð��ʰ� �Ǿ����� �˸���
					pPacket->Category			= MP_RESIDENTREGIST;
					pPacket->Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
					pPacket->dwObjectID			= pUserInfo->dwCharacterID;
					pPacket->dwData1			= DATE_MATCHING_CHAT_RESULT_TIMEOUT;
					g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)pPacket, sizeof(*pPacket) );
					// ..����ڿ��� �ð��ʰ� �Ǿ����� �˸���
					pPacket->Category			= MP_RESIDENTREGIST;
					pPacket->Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
					pPacket->dwObjectID			= pTargetUserInfo->dwCharacterID;
					pPacket->dwData1			= DATE_MATCHING_CHAT_RESULT_TIMEOUT;
					g_Network.Send2User( pTargetUserInfo->dwConnectionIndex, (char*)pPacket, sizeof(*pPacket) );
				}
				break;
			}
		}
		break;
	default:
		g_Network.Send2User( dwConnectionID, (char*)pMsg, dwLength );
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ASRV_EndDateMatchingChat Method																   �α׾ƿ� ����� ��Ȳ�� ���� ä�� ����
//
VOID CSHResidentRegistManager::ASRV_EndDateMatchingChat(USERINFO* pUserInfo)
{
#if defined(_AGENTSERVER)
	if (pUserInfo->DateMatchingInfo.nChatPlayerID)
	{
		// �ٸ� ������Ʈ�� �ֳ�?
		if (pUserInfo->DateMatchingInfo.nChatPlayerAgentID)
		{
			MSG_DWORD2 stPacket;
			stPacket.Category			= MP_RESIDENTREGIST;
			stPacket.Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT_DIFF_AGENT;
			stPacket.dwObjectID			= pUserInfo->DateMatchingInfo.nChatPlayerAgentID-1000;
			stPacket.dwData1			= DATE_MATCHING_CHAT_RESULT_END_CHAT;
			stPacket.dwData2			= pUserInfo->DateMatchingInfo.nChatPlayerID;			// ���ID
			g_Network.Send2SpecificAgentServer((char*)&stPacket, sizeof(stPacket) );
		}
		else
		{
			USERINFO* pTargetUserInfo = g_pUserTableForObjectID->FindUser(pUserInfo->DateMatchingInfo.nChatPlayerID);
			if (pTargetUserInfo)
			{
				ASRV_InitDateMatchingChatState(pTargetUserInfo);

				MSG_DWORD2 stPacket;
				stPacket.Category			= MP_RESIDENTREGIST;
				stPacket.Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
				stPacket.dwObjectID			= pTargetUserInfo->dwCharacterID;
				stPacket.dwData1			= DATE_MATCHING_CHAT_RESULT_END_CHAT;
				stPacket.dwData2			= NULL;										// ���ID
				g_Network.Send2User( pTargetUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
			}
		}
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ASRV_InitDateMatchingChatState Method																	����Ʈ ��Ī ä�� ���� �ʱ�ȭ
//
VOID CSHResidentRegistManager::ASRV_InitDateMatchingChatState(USERINFO* pUserInfo)
{
#if defined(_AGENTSERVER)
	pUserInfo->DateMatchingInfo.nRequestChatState			= DATE_MATCHING_CHAT_REQUEST_STATE_NONE;
	pUserInfo->DateMatchingInfo.nChatPlayerID				= NULL;
	pUserInfo->DateMatchingInfo.nChatPlayerAgentID			= NULL;
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_ParseRequestFromClient Method																			Ŭ���̾�Ʈ ��û �м�/ó��
//
VOID CSHResidentRegistManager::SRV_ParseRequestFromClient(DWORD dwConnectionID, char* pMsg, DWORD dwLength)
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
	// ���
	case MP_RESIDENTREGIST_REGIST:
		{
			PACKET_RESIDENTREGIST_REGIST* pPacket = (PACKET_RESIDENTREGIST_REGIST*)pMsg;
			SRV_RegistResident(pPlayer, &pPacket->stInfo);

			// TODO : �ֹε�Ͻ� ��ü����
		}
		break;
	// �ڱ�Ұ� ������Ʈ
	case MP_RESIDENTREGIST_UPDATE_INTRODUCTION:
		{
			PACKET_RESIDENTREGIST_UPDATE_INTRODUCTION* pPacket = (PACKET_RESIDENTREGIST_UPDATE_INTRODUCTION*)pMsg;
			SRV_UpdateIntroduction(pPlayer, pPacket->szIntroduction);
		}
		break;
	// ����Ʈ��Ī �˻�
	case MP_RESIDENTREGIST_DATEMATCHING_SERCH:
		{
			MSG_WORD4* pPacket = (MSG_WORD4*)pMsg;
			SRV_SerchDateMatching(pPlayer, pPacket->wData1, pPacket->wData2, pPacket->wData3, pPacket->wData4);
		}
		break;
	// ����Ʈ��Ī ä�� ��û
	case MP_RESIDENTREGIST_DATEMATCHING_REQUEST_CHAT:
		{
			MSG_DWORD2* pPacket = (MSG_DWORD2*)pMsg;
			SRV_RequestDateMatchingChat(pPlayer, pPacket);
		}
		break;
	// ����Ʈ��Ī ä�� ��û ���
	case MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT:
		{
			MSG_DWORD2* pPacket = (MSG_DWORD2*)pMsg;
			SRV_ResultRequestDateMatchingChat(pPlayer, pPacket);
		}
		break;
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_RegistResident Method																							   �ֹε�� ó��
//
VOID CSHResidentRegistManager::SRV_RegistResident(CPlayer* pPlayer, stRegistInfo* pInfo)
{
#if defined(_MAPSERVER_)
	CSHMain::RESULT eResult = CSHMain::RESULT_OK;
	PACKET_RESIDENTREGIST_REGIST stPacket;

	// �̹� �߱� �޾ҳ�?
	if (pPlayer->GetResidentRegistInfo()->bIsValid)
	{
		eResult = CSHMain::RESULT_FAIL_INVALID;
		goto FAILED;
	}

	// ���� ���� üũ
	if (pInfo->nAge == 0 || pInfo->nAge > 100)
	{
		eResult = CSHMain::RESULT_FAIL_INVALID;
		goto FAILED;
	}

	// ���� ���� üũ
	if (pInfo->nPropensityLike01 == 0 ||
		pInfo->nPropensityLike02 == 0 ||
		pInfo->nPropensityLike03 == 0 ||
		pInfo->nPropensityDisLike01 == 0 ||
		pInfo->nPropensityDisLike02 == 0 ||
		pInfo->nPropensityLike01 > m_stAddInfo.m_nMaxPropensity ||
		pInfo->nPropensityLike02 > m_stAddInfo.m_nMaxPropensity ||
		pInfo->nPropensityLike03 > m_stAddInfo.m_nMaxPropensity ||
		pInfo->nPropensityDisLike01 > m_stAddInfo.m_nMaxPropensity ||
		pInfo->nPropensityDisLike02 > m_stAddInfo.m_nMaxPropensity)
	{
		eResult = CSHMain::RESULT_FAIL_INVALID;
		goto FAILED;
	}

	// �������� ���� üũ
	if (pInfo->nLocation == 0 || pInfo->nLocation >= m_stAddInfo.m_nMaxLocation)
	{
		eResult = CSHMain::RESULT_FAIL_INVALID;
		goto FAILED;
	}

	// �ʱ�ȭ�� �ʿ��� �͵�..
	pInfo->nFavor = 0;
	pInfo->nSex = pPlayer->GetGender();
	SafeStrCpy(pInfo->szNickName, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);

	// �ʼ����� �÷��̾� ������ ����
	DATE_MATCHING_INFO stInfo;
	ZeroMemory(&stInfo, sizeof(stInfo));
	stInfo.bIsValid				= TRUE;
	SafeStrCpy(stInfo.szName, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);
	stInfo.nRace				= pInfo->nRace;
	stInfo.byGender				= (BYTE)pInfo->nSex;
	stInfo.dwAge				= pInfo->nAge;
	stInfo.dwRegion				= pInfo->nLocation;
	stInfo.dwGoodFeelingDegree	= pInfo->nFavor;
	stInfo.pdwGoodFeeling[0]	= pInfo->nPropensityLike01;
	stInfo.pdwGoodFeeling[1]	= pInfo->nPropensityLike02;
	stInfo.pdwGoodFeeling[2]	= pInfo->nPropensityLike03;
	stInfo.pdwBadFeeling[0]		= pInfo->nPropensityDisLike01;
	stInfo.pdwBadFeeling[1]		= pInfo->nPropensityDisLike02;
	pPlayer->SetResidentRegistInfo(&stInfo);

	ResidentRegist_SaveInfo(pPlayer->GetID(), pInfo);

	// Ŭ���̾�Ʈ�� �˸���
	// ..Ŭ���̾�Ʈ�� �ٷ� ���� �ʰ� Agent���� ó���� ���� ó���ϰ� ���� (�ڽ��� ���)
	// ..��Ʈ�˸��� ������ �׸��峻�� �ٸ� ĳ���Ϳ��Ե� �˷��ֱ� ���� PACKEDDATA_OBJ->QuickSendExceptObjectSelf �Ѵ�.
	stPacket.Category			= MP_RESIDENTREGIST;
	stPacket.Protocol			= MP_RESIDENTREGIST_REGIST;
	stPacket.dwObjectID			= pPlayer->GetID();
	stPacket.stInfo				= *pInfo;

	pPlayer->SendMsg(&stPacket, sizeof(stPacket));
	PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pPlayer, &stPacket, sizeof(stPacket));

	MSG_DWORD_NAME msg;
	msg.Category = MP_RESIDENTREGIST;
	msg.Protocol = MP_RESIDENTREGIST_REGIST_NOTIFY;
	strcpy( msg.Name, pPlayer->GetObjectName() );

	//GiftItemInsert( pPlayer->GetID(), pPlayer->GetUserID(), 0, 21000282, 5, 1 );
	//GiftItemInsert( pPlayer->GetID(), pPlayer->GetUserID(), 0, 21000285, 10, 1 );
	
	ITEMMGR->ObtainMonstermeterItem( pPlayer, 21000526, 5 );
	ITEMMGR->ObtainMonstermeterItem( pPlayer, 21000285, 3 );

	g_Network.Broadcast2AgentServer( (char*)&msg, sizeof(msg) );
FAILED:
	;
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_ResetRegistResident Method																						 �ֹε�� �ʱ�ȭ
//	080215 KTH
BOOL CSHResidentRegistManager::SRV_ResetRegistResident(CPlayer* pPlayer)
{
#if defined(_MAPSERVER_)

	//PACKET_RESIDENTREGIST_REGIST stPacket;

	// �߱� ���� ������...
	if( !pPlayer->GetResidentRegistInfo()->bIsValid || pPlayer->IsPKMode() )
	{
		// ���� ����
		return FALSE;
	}
	
	DATE_MATCHING_INFO* pDateMatchingInfo = pPlayer->GetResidentRegistInfo();
	CSHResidentRegistManager::stRegistInfo* pRegistInfo = pPlayer->GetResidentRegistInfo2();

	// �ʼ����� ����Ʈ ��Ī ������ �ֹε�� ������ �ʱ�ȭ�Ѵ�.
	ZeroMemory(pDateMatchingInfo, sizeof(DATE_MATCHING_INFO));
	ZeroMemory(pRegistInfo, sizeof(stRegistInfo));

	ResidentRegist_Reset(pPlayer->GetID());

	// ������Ʈ�� ������ �����־�� �ϴ´� ���� �ϸ� ��� ���� ;;
#endif
	return TRUE;
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_UpdateIntroduction Method																				  �ڱ�Ұ� ������Ʈ ó��
//
VOID CSHResidentRegistManager::SRV_UpdateIntroduction(CPlayer* pPlayer, char* szIntroduction)
{
#if defined(_MAPSERVER_)
	ResidentRegist_SaveInfo(pPlayer->GetID(), szIntroduction, 1);

	// Ŭ���̾�Ʈ�� �˸���
	// ..Ŭ���̾�Ʈ�� �ٷ� ���� �ʰ� Agent���� ó���� ���� ó���ϰ� ����
	PACKET_RESIDENTREGIST_UPDATE_INTRODUCTION stPacket;
	stPacket.Category			= MP_RESIDENTREGIST;
	stPacket.Protocol			= MP_RESIDENTREGIST_UPDATE_INTRODUCTION;
	stPacket.dwObjectID			= pPlayer->GetID();
	
	SafeStrCpy(stPacket.szIntroduction, szIntroduction, sizeof(stPacket.szIntroduction));

	pPlayer->SendMsg(&stPacket, sizeof(stPacket));
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_SerchDateMatching Method																						����Ʈ ��Ī �˻�
//
VOID CSHResidentRegistManager::SRV_SerchDateMatching(CPlayer* pPlayer, WORD nAgeRange, WORD nLocation, WORD nSex, WORD nGroupIndex)
{
#if defined(_MAPSERVER_)
	CSHMain::RESULT eResult = CSHMain::RESULT_OK;
	MSG_DWORD5 stPacket;

	// ���� ���� üũ
	// ..0�� ��� ����
	if (nAgeRange >= m_stAddInfo.m_nMaxSerchAgeRange)
	{
		eResult = CSHMain::RESULT_FAIL_INVALID;
		goto FAILED;
	}

	// ���� ���� üũ
	if (nLocation >= m_stAddInfo.m_nMaxLocation)
	{
		eResult = CSHMain::RESULT_FAIL_INVALID;
		goto FAILED;
	}

	// �˻� ��� �ð� üũ
	if (gCurTime - pPlayer->GetResidentRegistInfo()->nSerchTimeTick < DATE_MATCHING_RESERCH_TIME)
	{
		eResult = CSHMain::RESULT_FAIL_01;
		goto FAILED;
	}
	pPlayer->SetDateMatchingSerchTimeTick(gCurTime);

	// Ŭ���̾�Ʈ�� �˸���
	// ..Ŭ���̾�Ʈ�� �ٷ� ���� �ʰ� Agent���� �˻� ����� ó���ϰ� ����.
	stPacket.Category			= MP_RESIDENTREGIST;
	stPacket.Protocol			= MP_RESIDENTREGIST_DATEMATCHING_SERCH;
	stPacket.dwObjectID			= pPlayer->GetID();
	stPacket.dwData1			= m_stAddInfo.m_pTblSerchAgeRange[nAgeRange].nMin;
	stPacket.dwData2			= m_stAddInfo.m_pTblSerchAgeRange[nAgeRange].nMax;
	stPacket.dwData3			= m_stAddInfo.m_pTblLocation[nLocation].nID;
	stPacket.dwData4			= nSex;
	stPacket.dwData5			= nGroupIndex;

	pPlayer->SendMsg(&stPacket, sizeof(stPacket));
FAILED:
	;
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_RequestDateMatchingChat Method																		  ����Ʈ ��Ī ä�� ��û ó��
//
VOID CSHResidentRegistManager::SRV_RequestDateMatchingChat(CPlayer* pPlayer, MSG_DWORD2* pPacket)
{
#if defined(_MAPSERVER_)
	pPlayer->SetDateMatchingChatState(pPacket->dwData1);
	pPlayer->SetDateMatchingChatTimeTick(gCurTime);
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_ResultRequestDateMatchingChat Method																  ����Ʈ ��Ī ä�� ��� ó��
//
VOID CSHResidentRegistManager::SRV_ResultRequestDateMatchingChat(CPlayer* pPlayer, MSG_DWORD2* pPacket)
{
#if defined(_MAPSERVER_)
	pPlayer->SetDateMatchingChatState(pPacket->dwData1);
	pPlayer->SetDateMatchingChatTimeTick(gCurTime);
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_ProcDateMatchingChatTimeOut Method																  ����Ʈ ��Ī ä�� �ð� �ʰ� ó��
//
VOID CSHResidentRegistManager::SRV_ProcDateMatchingChatTimeOut(CPlayer* pPlayer)
{
#if defined(_MAPSERVER_)
	// ä�� ��û ���̸� �ð� �ʰ� ó���� �Ѵ�.
	if (pPlayer->GetResidentRegistInfo()->nRequestChatState == DATE_MATCHING_CHAT_REQUEST_STATE_REQUEST)
	{
		if (gCurTime - pPlayer->GetResidentRegistInfo()->nRequestChatTimeTick > SHMath_SECOND(10))
		{
			pPlayer->SetDateMatchingChatState(DATE_MATCHING_CHAT_REQUEST_STATE_NONE);
			pPlayer->SetDateMatchingChatTimeTick(gCurTime);

			MSG_DWORD2 stPacket;

			stPacket.Category			= MP_RESIDENTREGIST;
			stPacket.Protocol			= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
			stPacket.dwObjectID			= pPlayer->GetID();
			stPacket.dwData1			= DATE_MATCHING_CHAT_RESULT_TIMEOUT;
			pPlayer->SendMsg(&stPacket, sizeof(stPacket));
		}
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CalcMatchingPoint Method																							��Ī ����Ʈ ��� 
//
UINT CSHResidentRegistManager::CalcMatchingPoint(DATE_MATCHING_INFO* pstInfo, stRegistInfo* pstTargetInfo)
{
	int nMatchingPoint = 100;
	int nMinusPoint = 0;

	// ���̳��� ���̸�ŭ -, �ִ� -���� 30
	nMinusPoint += abs((int)pstInfo->dwAge - (int)pstTargetInfo->nAge);
	if (nMinusPoint > 30) nMinusPoint = 30;

	// ���� ������ �ٸ��� -10
	if (pstInfo->dwRegion != pstTargetInfo->nLocation)
	{
		nMinusPoint += 10;
	}

	// ���̳��� �����ϴ� ���� ������ŭ -10
	int nMaxPro = sizeof(pstInfo->pdwGoodFeeling)/sizeof(pstInfo->pdwGoodFeeling[0]);
	int nTmp[10];
	nTmp[0] = pstTargetInfo->nPropensityLike01;
	nTmp[1] = pstTargetInfo->nPropensityLike02;
	nTmp[2] = pstTargetInfo->nPropensityLike03;
	for(int i=0; i<nMaxPro; i++)
	{
		for(int j=0; j<nMaxPro; j++)
		{
			if (pstInfo->pdwGoodFeeling[i] == nTmp[j])
			{
				goto NEXT1;
			}
		}
		nMinusPoint += 10;
	NEXT1:;
	}

	// ���̳��� �Ⱦ��ϴ� ���� ������ŭ -10
	nMaxPro = sizeof(pstInfo->pdwBadFeeling)/sizeof(pstInfo->pdwBadFeeling[0]);
	nTmp[0] = pstTargetInfo->nPropensityDisLike01;
	nTmp[1] = pstTargetInfo->nPropensityDisLike02;
	for(int i=0; i<nMaxPro; i++)
	{
		for(int j=0; j<nMaxPro; j++)
		{
			if (pstInfo->pdwBadFeeling[i] == nTmp[j])
			{
				goto NEXT2;
			}
		}
		nMinusPoint += 10;
	NEXT2:;
	}

	nMatchingPoint -= nMinusPoint;
	if (nMatchingPoint < 0)	nMatchingPoint = 0;

	return nMatchingPoint;
}
UINT CSHResidentRegistManager::CalcMatchingPoint(DATE_MATCHING_INFO* pstInfo, DATE_MATCHING_INFO* pstTargetInfo)
{
	int nMatchingPoint = 100;
	int nMinusPoint = 0;

	// ���̳��� ���̸�ŭ -, �ִ� -���� 30
	nMinusPoint += abs((int)pstInfo->dwAge - (int)pstTargetInfo->dwAge);
	if (nMinusPoint > 30) nMinusPoint = 30;

	// ���� ������ �ٸ��� -10
	if (pstInfo->dwRegion != pstTargetInfo->dwRegion)
	{
		nMinusPoint += 10;
	}

	// ���̳��� �����ϴ� ���� ������ŭ -10
	int nMaxPro = sizeof(pstInfo->pdwGoodFeeling)/sizeof(pstInfo->pdwGoodFeeling[0]);
	for(int i=0; i<nMaxPro; i++)
	{
		for(int j=0; j<nMaxPro; j++)
		{
			if (pstInfo->pdwGoodFeeling[i] == pstTargetInfo->pdwGoodFeeling[j])
			{
				goto NEXT1;
			}
		}
		nMinusPoint += 10;
	NEXT1:;
	}

	// ���̳��� �Ⱦ��ϴ� ���� ������ŭ -10
	nMaxPro = sizeof(pstInfo->pdwBadFeeling)/sizeof(pstInfo->pdwBadFeeling[0]);
	for(int i=0; i<nMaxPro; i++)
	{
		for(int j=0; j<nMaxPro; j++)
		{
			if (pstInfo->pdwBadFeeling[i] == pstTargetInfo->pdwBadFeeling[j])
			{
				goto NEXT2;
			}
		}
		nMinusPoint += 10;
	NEXT2:;
	}

	nMatchingPoint -= nMinusPoint;
	if (nMatchingPoint < 0)	nMatchingPoint = 0;

	return nMatchingPoint;
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_RequestRegist Method																						������ �ֹε�� ��û
//
VOID CSHResidentRegistManager::CLI_RequestRegist(stRegistInfo* pInfo)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	PACKET_RESIDENTREGIST_REGIST stPacket;
	stPacket.Category 		= MP_RESIDENTREGIST;
	stPacket.Protocol 		= MP_RESIDENTREGIST_REGIST;
	stPacket.dwObjectID		= HEROID;
	stPacket.stInfo			= *pInfo;

	NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_RequestRegist Method																			   ������ �ڱ�Ұ� ������Ʈ ��û
//
VOID CSHResidentRegistManager::CLI_RequestUpdateIntroduction(char* szIntroduction)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	PACKET_RESIDENTREGIST_UPDATE_INTRODUCTION stPacket;
	stPacket.Category 		= MP_RESIDENTREGIST;
	stPacket.Protocol 		= MP_RESIDENTREGIST_UPDATE_INTRODUCTION;
	stPacket.dwObjectID		= HEROID;

	SafeStrCpy(stPacket.szIntroduction, szIntroduction, MAX_INTRODUCE_LEN+1);
	// ' �� ���ڿ��� ��Ÿ���� ǥ���̹Ƿ� ���ڿ��� ����� �� ����. " �� ��ü�ϵ��� ��.
	for(int i=0; i<MAX_INTRODUCE_LEN+1; i++)
	{
		if (stPacket.szIntroduction[i] == '\'')
		{
			stPacket.szIntroduction[i] = '"';
		}
	}

	NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_RequestSerchDateMatching Method																		������ ����Ʈ ��Ī �˻� ��û
//
VOID CSHResidentRegistManager::CLI_RequestSerchDateMatching(WORD nAgeRange, WORD nLocation, WORD nSex, int nGroupIndexDelta)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
  	if (GAMEIN->GetDateMatchingDlg()->GetPartnerListDlg()->GetSerchGroupIndex() + nGroupIndexDelta < 0) return;
	// 0�� �ε����� �׷��� �˻��Ѵٸ� �ʱ�ȭ..
	if (nGroupIndexDelta == 0)
	{
		GAMEIN->GetDateMatchingDlg()->GetPartnerListDlg()->SetSerchGroupIndex(0);
		GAMEIN->GetDateMatchingDlg()->GetPartnerListDlg()->SetSerchGroupNum(0);
		GAMEIN->GetDateMatchingDlg()->GetPartnerListDlg()->UpdateGroupInfo();
	}

	m_nSerchResultNum		= 0;

	MSG_WORD4 stPacket;
	stPacket.Category 		= MP_RESIDENTREGIST;
	stPacket.Protocol 		= MP_RESIDENTREGIST_DATEMATCHING_SERCH;
	stPacket.dwObjectID		= HEROID;

	stPacket.wData1			= nAgeRange;
	stPacket.wData2			= nLocation;
	stPacket.wData3			= nSex;
	stPacket.wData4			= GAMEIN->GetDateMatchingDlg()->GetPartnerListDlg()->GetSerchGroupIndex() + nGroupIndexDelta;

	NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_RequestRecommendOtherChar Method																������ �ٸ� ĳ���� ��õ�ϱ� ��û
//
VOID CSHResidentRegistManager::CLI_RequestRecommendOtherChar(DWORD nTargetPlayerID)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	MSG_DWORD stPacket;
	stPacket.Category 		= MP_RESIDENTREGIST;
	stPacket.Protocol 		= MP_RESIDENTREGIST_DATEMATCHING_RECOMMEND_OTHER_CHAR;
	stPacket.dwObjectID		= HEROID;
	stPacket.dwData			= nTargetPlayerID;

	NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
#endif
}


// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_RequestDateMatchingChat Method																   ������ ����Ʈ ��Ī ä�� ��û ��û
//
VOID CSHResidentRegistManager::CLI_RequestDateMatchingChat(stSerchResult* pSerchTarget)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	m_stChatPartner.nAgentID = pSerchTarget->nAgentID;

	MSG_DWORD2 stPacket;
	stPacket.Category 		= MP_RESIDENTREGIST;
	stPacket.Protocol 		= MP_RESIDENTREGIST_DATEMATCHING_REQUEST_CHAT;
	stPacket.dwObjectID		= HEROID;
	stPacket.dwData1		= pSerchTarget->nID;
	stPacket.dwData2		= m_stChatPartner.nAgentID;

	// �ٸ� ������Ʈ�� �÷��̾��� �������� ����
	// ..�ٸ� ������Ʈ �÷��̾�� ä���� ��� �˻� ������� nAgentID�� +1000�� �Ѵ�. 
	// ..�������� ������ ���� +1000�� �ϰ� ���������δ� nAgentID���� �״�� ����ϱ� ������ -1000 �ؼ� ������ �Ѵ�.
	if (m_stChatPartner.nAgentID >= 1000)
	{
		stPacket.Protocol 	= MP_RESIDENTREGIST_DATEMATCHING_REQUEST_CHAT_DIFF_AGENT;
		stPacket.dwData2	= m_stChatPartner.nAgentID - 1000;
	}

	NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_CancelDateMatchingChat Method																   ������ ����Ʈ ��Ī ä�� ��û ���
//
VOID CSHResidentRegistManager::CLI_CancelDateMatchingChat()
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	MSG_DWORD2 stPacket;
	stPacket.Category 		= MP_RESIDENTREGIST;
	stPacket.Protocol 		= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
	stPacket.dwObjectID		= HEROID;
	stPacket.dwData1		= DATE_MATCHING_CHAT_RESULT_CANCEL;
	stPacket.dwData2		= m_stChatPartner.nAgentID;
	// �ٸ� ������Ʈ�� �÷��̾��� �������� ����
	// ..�ٸ� ������Ʈ �÷��̾�� ä���� ��� �˻� ������� nAgentID�� +1000�� �Ѵ�. 
	// ..�������� ������ ���� +1000�� �ϰ� ���������δ� nAgentID���� �״�� ����ϱ� ������ -1000 �ؼ� ������ �Ѵ�.
	if (m_stChatPartner.nAgentID >= 1000)
	{
		stPacket.Protocol 	= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT_DIFF_AGENT;
		stPacket.dwData2	= m_stChatPartner.nAgentID - 1000;
	}

	NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_RefuseDateMatchingChat Method																   ������ ����Ʈ ��Ī ä�� ��û ����
//
VOID CSHResidentRegistManager::CLI_RefuseDateMatchingChat()
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	MSG_DWORD2 stPacket;
	stPacket.Category 		= MP_RESIDENTREGIST;
	stPacket.Protocol 		= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
	stPacket.dwObjectID		= HEROID;
	stPacket.dwData1		= DATE_MATCHING_CHAT_RESULT_REFUSE;
	stPacket.dwData2		= m_stChatPartner.nAgentID;
	// �ٸ� ������Ʈ�� �÷��̾��� �������� ����
	// ..�ٸ� ������Ʈ �÷��̾�� ä���� ��� �˻� ������� nAgentID�� +1000�� �Ѵ�. 
	// ..�������� ������ ���� +1000�� �ϰ� ���������δ� nAgentID���� �״�� ����ϱ� ������ -1000 �ؼ� ������ �Ѵ�.
	if (m_stChatPartner.nAgentID >= 1000)
	{
		stPacket.Protocol 	= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT_DIFF_AGENT;
		stPacket.dwData2	= m_stChatPartner.nAgentID - 1000;
	}

	NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_StartDateMatchingChat Method																   ������ ����Ʈ ��Ī ä�� ��û �¶�
//
VOID CSHResidentRegistManager::CLI_StartDateMatchingChat()
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	MSG_DWORD2 stPacket;
	stPacket.Category 		= MP_RESIDENTREGIST;
	stPacket.Protocol 		= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
	stPacket.dwObjectID		= HEROID;
	stPacket.dwData1		= DATE_MATCHING_CHAT_RESULT_START_CHAT;
	stPacket.dwData2		= m_stChatPartner.nAgentID;
	// �ٸ� ������Ʈ�� �÷��̾��� �������� ����
	// ..�ٸ� ������Ʈ �÷��̾�� ä���� ��� �˻� ������� nAgentID�� +1000�� �Ѵ�. 
	// ..�������� ������ ���� +1000�� �ϰ� ���������δ� nAgentID���� �״�� ����ϱ� ������ -1000 �ؼ� ������ �Ѵ�.
	if (m_stChatPartner.nAgentID >= 1000)
	{
		stPacket.Protocol 	= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT_DIFF_AGENT;
		stPacket.dwData2	= m_stChatPartner.nAgentID - 1000;
	}

	NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_StartDateMatchingChat Method																     ������ ����Ʈ ��Ī ä�� �� ��û
//
VOID CSHResidentRegistManager::CLI_EndDateMatchingChat()
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	MSG_DWORD2 stPacket;
	stPacket.Category 		= MP_RESIDENTREGIST;
	stPacket.Protocol 		= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT;
	stPacket.dwObjectID		= HEROID;
	stPacket.dwData1		= DATE_MATCHING_CHAT_RESULT_END_CHAT;
	stPacket.dwData2		= m_stChatPartner.nAgentID;
	// �ٸ� ������Ʈ�� �÷��̾��� �������� ����
	// ..�ٸ� ������Ʈ �÷��̾�� ä���� ��� �˻� ������� nAgentID�� +1000�� �Ѵ�. 
	// ..�������� ������ ���� +1000�� �ϰ� ���������δ� nAgentID���� �״�� ����ϱ� ������ -1000 �ؼ� ������ �Ѵ�.
	if (m_stChatPartner.nAgentID >= 1000)
	{
		stPacket.Protocol 	= MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT_DIFF_AGENT;
		stPacket.dwData2	= m_stChatPartner.nAgentID - 1000;
	}

	NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_SendDateMatchingChatText Method																 ������ ����Ʈ ��Ī ä�� ���� ������
//
VOID CSHResidentRegistManager::CLI_SendDateMatchingChatText(char* szText)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	MSG_CHAT_WITH_SENDERID stPacket;
	stPacket.Category 		= MP_RESIDENTREGIST;
	stPacket.Protocol 		= MP_RESIDENTREGIST_DATEMATCHING_CHAT;
	stPacket.dwObjectID		= HEROID;
	SafeStrCpy(stPacket.Name, HERO->GetObjectName(), MAX_NAME_LENGTH+1);
	SafeStrCpy(stPacket.Msg, szText, MAX_CHAT_LENGTH+1);
	stPacket.dwSenderID		= m_stChatPartner.nAgentID;
	// �ٸ� ������Ʈ�� �÷��̾��� �������� ����
	// ..�ٸ� ������Ʈ �÷��̾�� ä���� ��� �˻� ������� nAgentID�� +1000�� �Ѵ�. 
	// ..�������� ������ ���� +1000�� �ϰ� ���������δ� nAgentID���� �״�� ����ϱ� ������ -1000 �ؼ� ������ �Ѵ�.
	if (m_stChatPartner.nAgentID >= 1000)
	{
		stPacket.Protocol 		= MP_RESIDENTREGIST_DATEMATCHING_CHAT_DIFF_AGENT;
		stPacket.dwSenderID		= m_stChatPartner.nAgentID - 1000;
	}

	NETWORK->Send( (MSGROOT*)&stPacket, stPacket.GetMsgLength() );
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_ParseAnswerFromSrv Method																			  �����κ����� ���� �м�/ó��
//
VOID CSHResidentRegistManager::CLI_ParseAnswerFromSrv(void* pMsg)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)
	MSGBASE* pTmp = (MSGBASE*)pMsg;

	switch(pTmp->Protocol)
	{
	// ��� ó��
	case MP_RESIDENTREGIST_REGIST:
		{
			PACKET_RESIDENTREGIST_REGIST* pPacket = (PACKET_RESIDENTREGIST_REGIST*)pMsg;
			DATE_MATCHING_INFO stInfo;

			ZeroMemory(&stInfo, sizeof(DATE_MATCHING_INFO));
			SafeStrCpy(stInfo.szName, pPacket->stInfo.szNickName, MAX_NAME_LENGTH+1);
			stInfo.bIsValid				= TRUE;
			stInfo.nRace				= pPacket->stInfo.nRace;
			stInfo.byGender				= (BYTE)pPacket->stInfo.nSex;
			stInfo.dwAge				= pPacket->stInfo.nAge;
			stInfo.dwRegion				= pPacket->stInfo.nLocation;
			stInfo.dwGoodFeelingDegree	= pPacket->stInfo.nFavor;
			stInfo.pdwGoodFeeling[0]	= pPacket->stInfo.nPropensityLike01;
			stInfo.pdwGoodFeeling[1]	= pPacket->stInfo.nPropensityLike02;
			stInfo.pdwGoodFeeling[2]	= pPacket->stInfo.nPropensityLike03;
			stInfo.pdwBadFeeling[0]		= pPacket->stInfo.nPropensityDisLike01;
			stInfo.pdwBadFeeling[1]		= pPacket->stInfo.nPropensityDisLike02;

			if (HERO->GetID() == pPacket->dwObjectID)
			{
				HERO->SetResidentRegistInfo(&stInfo);
				HERO->CheckAllHeart();

				// update matching dialog.
				GAMEIN->GetDateMatchingDlg()->UpdateMyInfo(&stInfo) ;

				// active matching dialog.
				CDateMatchingDlg* pDlg = GAMEIN->GetDateMatchingDlg() ;
				pDlg->GetPartnerListDlg()->SettingControls() ;
				//pDlg->GetPartnerListDlg()->UpdateInfo() ;
				pDlg->SetActive(TRUE) ;

				// Deactive identification dialog.
				GAMEIN->GetIdentificationDlg()->SetActive(FALSE) ;

				// close favor icon dialog.
				GAMEIN->GetFavorIconDlg()->SetActive(FALSE) ;

				// Deactive npc script dialog.
				if( HERO->GetState() != eObjectState_Die )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

				// notice to chatting dialog.
				//CHATMGR->AddMsg( CTC_SYSMSG, "�ֹε������ �߱� �޾ҽ��ϴ�." );
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1298 ) );

				// 080122 LYW --- SHResidentRegistManager : npc ���̾�α� �ݱ�.
				cNpcScriptDialog* pNpcDlg = NULL ;
				pNpcDlg = GAMEIN->GetNpcScriptDialog() ;
				
				if( pNpcDlg )
				{
					pNpcDlg->SetActive(FALSE) ;
				}
			}
			else
 			{
				CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pPacket->dwObjectID);
				if( !pPlayer )	return;

				pPlayer->SetResidentRegistInfo(&stInfo);

				// ����ο� ��Ī��Ų��.
				if( pPlayer->CheckHeart( HERO ) )
				{
					HERO->AddMatch();

					pPlayer->SetHeart( TRUE );
				}
			}
		}
		break;
	// �ڱ�Ұ� ó��
	case MP_RESIDENTREGIST_UPDATE_INTRODUCTION:
		{
			PACKET_RESIDENTREGIST_UPDATE_INTRODUCTION* pPacket = (PACKET_RESIDENTREGIST_UPDATE_INTRODUCTION*)pMsg;

			if (HERO->GetID() == pPacket->dwObjectID)
			{
				DATE_MATCHING_INFO stInfo;

				stInfo = HERO->GetDateMatchingInfo();
				SafeStrCpy(stInfo.szIntroduce, pPacket->szIntroduction, MAX_INTRODUCE_LEN+1);
				HERO->SetResidentRegistInfo(&stInfo);
			}

			// setting introduce to read only.
			cTextArea* pTextArea = GAMEIN->GetMatchMyInfoDlg()->GetIntroduce() ;

			if( pTextArea )
			{
				pTextArea->SetReadOnly(TRUE) ;
				pTextArea->SetFocusEdit(FALSE) ;
			}

			// notice to chatting dialog.
			//CHATMGR->AddMsg( CTC_SYSMSG, "�Ұ��� ����Ǿ����ϴ�." );
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1299 ) );
		}
		break;
	//  �˻� ��� ó��
	case MP_RESIDENTREGIST_DATEMATCHING_SERCH:
		{
   			PACKET_SERCH_DATE_MATCHING* pPacket = (PACKET_SERCH_DATE_MATCHING*)pMsg;

			// �ִ� ������ �ʰ��ϸ� ó���Ұ�
			if (m_nSerchResultNum >= MAX_SERCH_NUM_CLIENT-1) break;
			m_pstSerchResult[m_nSerchResultNum] = pPacket->stSerchResult;
			m_nSerchResultNum++;

			// �˻��� �׷� �ε���
			GAMEIN->GetDateMatchingDlg()->GetPartnerListDlg()->SetSerchGroupIndex(pPacket->nGroupIndex);
			// ������Ʈ���� ������ ���� ���̰� �� �� �ִµ� �̷� ���
			// ���� ���� �׷� ���� ���� ������Ʈ���� ���� �׷� ���� �����ϱ� ���� ���� �׸� ���� ���� �׷� ������
			// Ŭ �� �׷� ���� �����Ѵ�. ���� �˻� ���۽ÿ��� �׿� ������� ������ �׷� ���� �����ؾ� �Ѵ�.
			if (pPacket->nGroupMax > GAMEIN->GetDateMatchingDlg()->GetPartnerListDlg()->GetSerchGroupNum())
			{
				GAMEIN->GetDateMatchingDlg()->GetPartnerListDlg()->SetSerchGroupNum(pPacket->nGroupMax);
			}
			if (m_nSerchResultNum == 1)
			{
				// �������̽� ����
				GAMEIN->GetDateMatchingDlg()->GetPartnerListDlg()->ClearPartnerList();
				GAMEIN->GetDateMatchingDlg()->GetPartnerListDlg()->SetSerchGroupNum(pPacket->nGroupMax);
				GAMEIN->GetDateMatchingDlg()->GetPartnerListDlg()->UpdateGroupInfo();
			}

// 			for(int i=0; i<74; i++)
			{
//				pPacket->stSerchResult.nID = i+10;
//				pPacket->stSerchResult.stRegistInfo.nAge = rand()/100;
				// setting to partner list.
				GAMEIN->GetDateMatchingDlg()->GetPartnerListDlg()->UpdateInfo(&pPacket->stSerchResult) ;
			}
		}
		break;
	// ����Ʈ��Ī ä�� ��û
	case MP_RESIDENTREGIST_DATEMATCHING_REQUEST_CHAT:
		{
			MSG_DWORD2* stPacket = (MSG_DWORD2*)pMsg;
			DWORD nRequestPlayerID = stPacket->dwData2;

			switch(stPacket->dwData1)
			{
			// ���� ���� �ƴ�
			case DATE_MATCHING_CHAT_RESULT_LOGOFF:
				{
					//WINDOWMGR->MsgBox( MBI_MATCHAT_CANCEL, MBT_CANCEL, "������ ���� ���� ���� �ƴմϴ�." );
					WINDOWMGR->MsgBox( MBI_MATCHAT_CANCEL, MBT_CANCEL, CHATMGR->GetChatMsg( 211 ) );
				}
				break;
			// ����ڰ� �̹� ä�� �� or ä�� ��û�� or ä�� ��û�޴� �� 
			case DATE_MATCHING_CHAT_RESULT_BUSY:
				{
					//MessageBox(NULL, "������ ä�� ���̰ų� �� ���� ��..", NULL, NULL);
/*
					cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID(MBI_MATCHAT_CANCEL) ;
					if( pMsgBox )
					{
						pMsgBox->ForceClose() ;

						WINDOWMGR->MsgBox( MBI_MATCHAT_CANTCHAT, MBT_OK, "������ ���� ��ȭ �غ� ���Դϴ�. ��� �� �ٽ� ��û�� �ּ���." );
					}
*/
					//WINDOWMGR->MsgBox( MBI_MATCHAT_CANCEL, MBT_CANCEL, "������ ���� ��ȭ �غ� ���Դϴ�. ��� �� �ٽ� ��û�� �ּ���." );
					WINDOWMGR->MsgBox( MBI_MATCHAT_CANCEL, MBT_CANCEL, CHATMGR->GetChatMsg( 1320 ) );
				}
				break;
			// ��û�ڿ��� ��ûó���� ��⸦ �˸�
			case DATE_MATCHING_CHAT_RESULT_REQUEST_AND_WAIT:
				{
					//MessageBox(NULL, "ä���� ��û����. ���� ���", NULL, NULL);
					//WINDOWMGR->MsgBox( MBI_MATCHAT_CANCEL, MBT_CANCEL, "��뿡�� ��ȭ�� ��û�߽��ϴ�. ������ ������ ��ø� ��ٷ� �ּ���." );
					WINDOWMGR->MsgBox( MBI_MATCHAT_CANCEL, MBT_CANCEL, CHATMGR->GetChatMsg( 1321 ) );
				}
				break;
			// ����ڿ��� �������� ä�ý�û �˸�
			case DATE_MATCHING_CHAT_RESULT_REQUESTED:
				{
					MSG_DWORD_NAME* pPacket = (MSG_DWORD_NAME*)pMsg;
					char szTxt[256];
					//sprintf(szTxt, "%s(��)�κ����� ä�ÿ�û~", pPacket->Name);
					//MessageBox(NULL, szTxt, NULL, NULL);
					sprintf(szTxt, CHATMGR->GetChatMsg( 1323 ), pPacket->Name);
					WINDOWMGR->MsgBox( MBI_MATCHAT_INVITE, MBT_YESNO, szTxt );
				}
				break;
			}
		}
		break;
	// ����Ʈ��Ī ä�� ��û ���
	case MP_RESIDENTREGIST_DATEMATCHING_RESULT_REQUEST_CHAT:
		{
			MSG_DWORD2* stPacket = (MSG_DWORD2*)pMsg;
			DWORD nPlayerID = stPacket->dwData2;

			switch(stPacket->dwData1)
			{
			// ���
			case DATE_MATCHING_CHAT_RESULT_CANCEL:
				{
					//MessageBox(NULL, "���", NULL, NULL);
					WINDOWMGR->MsgBox( MBI_MATCHAT_CANCEL, MBT_OK, CHATMGR->GetChatMsg( 1322 ) );
				}
				break;
			// �ð� �ʰ�
			case DATE_MATCHING_CHAT_RESULT_TIMEOUT:
				{
					//MessageBox(NULL, "�ð� �ʰ�..", NULL, NULL);
					//cDialog* pMsgBox = WINDOWMGR->GetWindowForID(MBI_MATCHAT_NOTICE) ;
					cMsgBox* pMsgBox = NULL ;
					
					pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID(MBI_MATCHAT_CANCEL) ;
					if( pMsgBox )
					{
						pMsgBox->ForceClose() ;

						//WINDOWMGR->MsgBox( MBI_MATCHAT_TIMEOVER, MBT_OK, "�ʴ뿡 ������ ���� �ʴ� ��û�� ����մϴ�." );
						WINDOWMGR->MsgBox( MBI_MATCHAT_TIMEOVER, MBT_OK, CHATMGR->GetChatMsg( 1324 ) );
					}

					pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID(MBI_MATCHAT_INVITE) ;
					if( pMsgBox )
					{
						pMsgBox->ForceClose() ;

						//WINDOWMGR->MsgBox( MBI_MATCHAT_TIMEOVER, MBT_OK, "�ʴ뿡 ������ �ʾҽ��ϴ�." );
						WINDOWMGR->MsgBox( MBI_MATCHAT_TIMEOVER, MBT_OK, CHATMGR->GetChatMsg( 1325 ) );
					}
				}
				break;
			// ��������
			case DATE_MATCHING_CHAT_RESULT_REFUSE:
				{
					//MessageBox(NULL, "��������..!!!", NULL, NULL);
					// deactive wait msg.
					//ForcePressButton(m_pButton[i]->GetID());
					cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID(MBI_MATCHAT_CANCEL) ;
					if( pMsgBox )
					{
						pMsgBox->ForceClose() ;
					}
					//WINDOWMGR->MsgBox( MBI_MATCHAT_REJECT, MBT_OK, "������ ��ȭ ��û�� �����߽��ϴ�." );
					WINDOWMGR->MsgBox( MBI_MATCHAT_REJECT, MBT_OK, CHATMGR->GetChatMsg( 1326 ) );
				}
				break;
				break;
			// ä�� ��
			case DATE_MATCHING_CHAT_RESULT_END_CHAT:
				{
					CDateMatchingDlg* pDlg = GAMEIN->GetDateMatchingDlg() ;

					if( pDlg )
					{
						if (pDlg->GetChatingDlg()->IsOnChatMode())
						{
							//WINDOWMGR->MsgBox( MBI_MATCHAT_STOP, MBT_OK, "��ȭ�� ����Ǿ����ϴ�." );
							WINDOWMGR->MsgBox( MBI_MATCHAT_STOP, MBT_OK, CHATMGR->GetChatMsg( 1327 ) );
						}
						// return to partner list.
						pDlg->SetUpperDlg(e_PartnerListDlg) ;

						CDateMatchingChatDlg* pChatDlg = pDlg->GetChatingDlg() ;

						// setting chat mode to off.
						if(pChatDlg)
						{
							pChatDlg->SetOnOffChatMode(FALSE) ;

							// clear chat list.
							pChatDlg->GetChatList()->RemoveAll() ;
						}

						// ����Ʈ �� ���� �Ұ�
						pDlg->ActivateBtnEnterDateZone(FALSE);
						// ç���� �� ���� �Ұ�
						pDlg->ActivateBtnEnterChallengeZone(FALSE);
					}
				}
				break;
			}
		}
		break;

	// ä�ý���
	case MP_RESIDENTREGIST_DATEMATCHING_RESULT_START_CHAT:
		{
			//MessageBox(NULL, "ä�ý���~", NULL, NULL);
			PACKET_SERCH_DATE_MATCHING* pPacket = (PACKET_SERCH_DATE_MATCHING*)pMsg;

			m_stChatPartner = pPacket->stSerchResult;

			// deactive wait msg.
			cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID(MBI_MATCHAT_CANCEL) ;
			if( pMsgBox )
			{
				pMsgBox->ForceClose() ;
			}

			// open chatting tab.
			CDateMatchingDlg* pDlg = GAMEIN->GetDateMatchingDlg() ;

			if( pDlg )
			{
				// open matching dialog.
				if( !pDlg->IsActive() )
				{
					pDlg->SetActive(TRUE) ;
				}

				// active chatting tab.
				pDlg->SetUpperDlg(e_ChatDlg) ;
				pDlg->GetChatingDlg()->SetOnOffChatMode(TRUE) ;

				// active partner info tab.
				pDlg->GetPartnerInfoDlg()->UpdateInfo(&pPacket->stSerchResult) ;
				pDlg->SetLowerDlg(e_PartnerInfoDlg) ;

				// ����Ʈ �� ���� ���� üũ
				if (PARTYMGR->IsPossibleEnterDateZone())
				{
					pDlg->ActivateBtnEnterDateZone(TRUE);
				}
				//  ç���� �� ���� ���� üũ
				if (PARTYMGR->IsPossibleEnterChallengeZone())
				{
					pDlg->ActivateBtnEnterChallengeZone(TRUE);
				}
			}
		}
		break;

	// ä��
	case MP_RESIDENTREGIST_DATEMATCHING_CHAT:
		{
			MSG_CHAT_WITH_SENDERID* pPacket = (MSG_CHAT_WITH_SENDERID*)pMsg;
			
			static char szText[256];

			sprintf(szText, "[%s]:%s", pPacket->Name, pPacket->Msg);
			//MessageBox(NULL, szText, "����Ʈ ��Ī ä��", NULL);

			CDateMatchingChatDlg* pDlg = GAMEIN->GetDateMatchingDlg()->GetChatingDlg() ;

			if( pDlg )
			{
				cListDialog* pListDlg = pDlg->GetChatList() ;

				if( pListDlg )
				{
					DWORD dwColor = RGB(255, 255, 255) ;

					int nLineNum = pListDlg->GetItemCount() ;

					pDlg->AddMsg(szText, dwColor) ;
					//pListDlg->AddItem(szText, dwColor, nLineNum) ;
				}
			}
		}
		break;
	// �ٸ� �������� ����
	case MP_RESIDENTREGIST_DATEMATCHING_SOMEBODY_INFO:
		{
			PACKET_SERCH_DATE_MATCHING* pPacket = (PACKET_SERCH_DATE_MATCHING*)pMsg;

			if (HERO == NULL) return;

			m_stChatPartner = pPacket->stSerchResult;

			// deactive wait msg.
			cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID(MBI_MATCHAT_CANCEL) ;
			if( pMsgBox )
			{
				pMsgBox->ForceClose() ;
			}

			// open chatting tab.
			CDateMatchingDlg* pDlg = GAMEIN->GetDateMatchingDlg() ;

			if( pDlg )
			{
				// open matching dialog.
				if( !pDlg->IsActive() )
				{
					pDlg->GetPartnerListDlg()->SettingControls() ;
					pDlg->SetActive(TRUE) ;
				}

				// active partner info tab.
				pDlg->GetPartnerInfoDlg()->UpdateInfo(&pPacket->stSerchResult) ;
				pDlg->SetLowerDlg(e_PartnerInfoDlg) ;
			}
		}
		break;
	case MP_RESIDENTREGIST_REGIST_NOTIFY:
		{
			MSG_DWORD_NAME* pmsg = ( MSG_DWORD_NAME* )pMsg;

			CHATMGR->AddMsg( CTC_OPERATOR2, CHATMGR->GetChatMsg( 1535 ), pmsg->Name );
		}
		break;
	}
#endif
}

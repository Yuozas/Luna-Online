/*********************************************************************

	 파일		: SHFamilyManager.cpp
	 작성자		: hseos
	 작성일		: 2007/07/03

	 파일설명	: CSHFamilyManager 클래스의 소스

 *********************************************************************/

#include "stdafx.h"
#include "MHFile.h"
#include "SHFamilyManager.h"
#include <io.h>

#if defined(_AGENTSERVER)
	#include "Network.h"
	#include "AgentDBMsgParser.h"
	#include "FilteringTable.h"
	#include "../Farm/SHFarmManager.h"
#elif defined(_MAPSERVER_)
	#include "UserTable.h"
	#include "Player.h"
	#include "PackedData.h"
	#include "Network.h"
	#include "../[CC]Header/GameResourceManager.h"
	#include "MapDBMsgParser.h"
	#include "../Farm/SHFarmManager.h"
#else
	#include "../FilteringTable.h"
	#include "../Player.h"
	#include "../ChatManager.h"
	#include "../GameIn.h"
	#include "../ObjectManager.h"
	#include "../WindowIDEnum.h"
	#include "../cMsgBox.h"
	#include "../interface/cWindowManager.h"
	#include "../FamilyCreateDialog.h"
	#include "../FamilyDialog.h"
	#include "ObjectBalloon.h"
	#include "OBalloonName.h"
	#include "../FreeImage/FreeImage.h"
	#include "cImageSelf.h"
	#include "../Monstermeter/SHMonstermeterDlg.h"
	#include "../Farm/SHFarmManageDlg.h"
	#include "MiniMapDlg.h"
	#include "BigMapDlg.h"
#endif

extern void ClearDirectory( LPCTSTR pStrName );
extern int g_nServerSetNum;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CSHFamilyManager
//
CSHFamilyManager g_csFamilyManager;

// 기본값(파일에서 읽어서 다시 설정함)
DWORD	CSHFamilyManager::HONOR_POINT_CHECK_TIME		= 60*60*1000;  	
int		CSHFamilyManager::HONOR_POINT_CHECK_MEMBER_NUM	= 2;
int		CSHFamilyManager::HONOR_POINT_CHECK_POINT		= 10;
float	CSHFamilyManager::MONKILL_HONOR_POINT_EXP_RATE	= 0.01f;
float	CSHFamilyManager::LEAVE_PENALTY_EXP_DOWN_RATE[FLK_MAX]	= {0.03f,};
int		CSHFamilyManager::LEAVE_PENALTY_REJOIN_TIME[FLK_MAX] = {24,};

// -------------------------------------------------------------------------------------------------------------------------------------
// CSHFamilyManager Method																										  생성자
//
CSHFamilyManager::CSHFamilyManager()
{
	ZeroMemory(m_pcsFamily, sizeof(m_pcsFamily));
	m_nFamilyNum = 0;
	m_nInviteMasterID = NULL;

	LoadFamilyInfo();
#if !defined(_AGENTSERVER) && !defined(_MAPSERVER_)
	ClearDirectory("Data/Interface/2DImage/FamilyEmblem");
	CreateDirectory("Data/Interface/2DImage/FamilyEmblem", NULL);
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
// ~CSHFamilyManager Method																										  파괴자
//
CSHFamilyManager::~CSHFamilyManager()
{
	for(int i=0; i<MAX_FAMILY_UNIT_NUM; i++)
	{
		SAFE_DELETE_ARRAY(m_pcsFamily[i]);
	}
}

// -------------------------------------------------------------------------------------------------------------------------------------
// LoadFamilyInfo Method																								패밀리 정보 읽기
//
BOOL CSHFamilyManager::LoadFamilyInfo()
{
	char szLine[MAX_PATH], szFile[MAX_PATH];
	int	 nKind = 0, nCnt = 0;
	CMHFile fp;

	sprintf(szFile, "./System/Resource/Family.bin");
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
		case 0:
			sscanf(szLine, "%d %d %d %f",		&HONOR_POINT_CHECK_TIME,
												&HONOR_POINT_CHECK_MEMBER_NUM,
												&HONOR_POINT_CHECK_POINT,
												&MONKILL_HONOR_POINT_EXP_RATE);
			HONOR_POINT_CHECK_TIME = HONOR_POINT_CHECK_TIME*60*1000;
			break;
		case 1:
			sscanf(szLine, "%f %f %f %d %d %d",			&LEAVE_PENALTY_EXP_DOWN_RATE[FLK_LEAVE],
														&LEAVE_PENALTY_EXP_DOWN_RATE[FLK_EXPEL],
														&LEAVE_PENALTY_EXP_DOWN_RATE[FLK_BREAKUP],
														&LEAVE_PENALTY_REJOIN_TIME[FLK_LEAVE],
														&LEAVE_PENALTY_REJOIN_TIME[FLK_EXPEL],
														&LEAVE_PENALTY_REJOIN_TIME[FLK_BREAKUP]);
			break;
		}
		
		nCnt++;
	}
	fp.Release();

	return TRUE;
}

// -------------------------------------------------------------------------------------------------------------------------------------
// AddFamilyToTbl Method																							테이블에 패밀리 추가
//
CSHFamily* CSHFamilyManager::AddFamilyToTbl(CSHFamily* pcsFamily)
{
	// 테이블에 이미 존재하는지 체크
	CSHFamily* pstAleadyFamily = GetFamily(pcsFamily->Get()->nID);
	if (pstAleadyFamily)
	{
		return pstAleadyFamily;
	}

	// 패밀리 추가
	//
	// ..패밀리 개수가 최대값을 넘는다면...
	if (m_nFamilyNum >= MAX_FAMILY_UNIT_NUM*MAX_FAMILY_1U_NUM)
	{
		return NULL;
	}

	// 삭제되어 빈 공간이 있으면 그 곳에 추가
	for(int i=0; i<m_nFamilyNum; i++)
	{
		int nUnitIndex = i/MAX_FAMILY_1U_NUM;
		int n1UIndex = i%MAX_FAMILY_1U_NUM;

		if (m_pcsFamily[nUnitIndex][n1UIndex].Get()->nID == NULL)
		{
			m_pcsFamily[nUnitIndex][n1UIndex].Set(pcsFamily);
			m_pcsFamily[nUnitIndex][n1UIndex].SetIndexAtTbl(i);
			return &m_pcsFamily[nUnitIndex][n1UIndex];
		}
	}

	int nUnitIndex = m_nFamilyNum/MAX_FAMILY_1U_NUM;
	int n1UIndex = m_nFamilyNum%MAX_FAMILY_1U_NUM;
	// ..유닛을 넘어가면(혹은 처음이면) 메모리 할당
	if (n1UIndex == 0)
	{
		m_pcsFamily[nUnitIndex] = new CSHFamily[MAX_FAMILY_1U_NUM];
	}

	m_pcsFamily[nUnitIndex][n1UIndex].Set(pcsFamily);
	m_pcsFamily[nUnitIndex][n1UIndex].SetIndexAtTbl(m_nFamilyNum);
	m_nFamilyNum++;

	return &m_pcsFamily[nUnitIndex][n1UIndex];
}

// -------------------------------------------------------------------------------------------------------------------------------------
// DelFamilyToTbl Method																						  테이블에서 패밀리 삭제
//
BOOL CSHFamilyManager::DelFamilyToTbl(DWORD nPlayerID, CSHFamily* pcsFamily)
{
	// 테이블에 이미 존재하는지 체크
	if (pcsFamily)
	{
		CSHFamily* pstAleadyFamily = GetFamily(pcsFamily->Get()->nID);
		if (pstAleadyFamily)
		{
			// 로그아웃 처리
			int	nLogoutCnt = 0;
			// ..로그아웃 하는 멤버의 접속 상태 설정
			for(UINT i=0; i<pcsFamily->Get()->nMemberNum; i++)
			{
				if (nPlayerID == pcsFamily->GetMember(i)->Get()->nID)
				{
					CSHFamilyMember::stINFO stMemberInfo = *pcsFamily->GetMember(i)->Get();

					// 접속 상태 설정
					stMemberInfo.eConState = CSHFamilyMember::MEMBER_CONSTATE_LOGOFF;
					pcsFamily->GetMember(i)->Set(&stMemberInfo);
				}

				// 로그아웃한 멤버 수 체크
				if (pcsFamily->GetMember(i)->Get()->eConState == CSHFamilyMember::MEMBER_CONSTATE_LOGOFF)
				{
					nLogoutCnt++;
				}
			}

			// ..현재 접속해 있는 멤버가 HONOR_POINT_CHECK_MEMBER_NUM명 미만이 되면 명예 포인트 시간 초기화
			if (pcsFamily->Get()->nMemberNum-nLogoutCnt < (UINT)CSHFamilyManager::HONOR_POINT_CHECK_MEMBER_NUM)
			{
				CSHFamily::stINFO_EX stInfoEx = *pcsFamily->GetEx();
				stInfoEx.nHonorPointTimeTick = 0;
				pcsFamily->SetEx(&stInfoEx);
			}

			// ..모든 멤버가 로그아웃 했으면 테이블에서 패밀리 제거
			if (nLogoutCnt == pcsFamily->Get()->nMemberNum)
			{
				int nUnitIndex = pcsFamily->GetIndexAtTbl()/MAX_FAMILY_1U_NUM;
				int n1UIndex = pcsFamily->GetIndexAtTbl()%MAX_FAMILY_1U_NUM;

				CSHFamily csFamily;
				m_pcsFamily[nUnitIndex][n1UIndex].Set(&csFamily);
				m_pcsFamily[nUnitIndex][n1UIndex].SetIndexAtTbl(NULL);
				return TRUE;
			}
		}
	}

	return FALSE;
}

// -------------------------------------------------------------------------------------------------------------------------------------
// DelFamilyToTbl Method																						  테이블에서 패밀리 삭제
//
BOOL CSHFamilyManager::DelFamilyToTbl(CSHFamily* pcsFamily)
{
	// 테이블에 이미 존재하는지 체크
	if (pcsFamily)
	{
		CSHFamily* pstAleadyFamily = GetFamily(pcsFamily->Get()->nID);
		if (pstAleadyFamily)
		{
			int nUnitIndex = pcsFamily->GetIndexAtTbl()/MAX_FAMILY_1U_NUM;
			int n1UIndex = pcsFamily->GetIndexAtTbl()%MAX_FAMILY_1U_NUM;

			CSHFamily csFamily;
			m_pcsFamily[nUnitIndex][n1UIndex].Set(&csFamily);
			m_pcsFamily[nUnitIndex][n1UIndex].SetIndexAtTbl(NULL);
			return TRUE;
		}
	}
	return FALSE;
}

// -------------------------------------------------------------------------------------------------------------------------------------
// SetFamily Method																											 패밀리 설정
//
VOID CSHFamilyManager::SetFamily(CSHFamily* pcsFamily)
{
	int nUnitIndex = pcsFamily->GetIndexAtTbl()/MAX_FAMILY_1U_NUM;
	int n1UIndex = pcsFamily->GetIndexAtTbl()%MAX_FAMILY_1U_NUM;

	m_pcsFamily[nUnitIndex][n1UIndex].Set(pcsFamily);
}

// -------------------------------------------------------------------------------------------------------------------------------------
// GetFamily Method																											 패밀리 얻기
//
CSHFamily* CSHFamilyManager::GetFamily(DWORD nFamilyID)
{
	int nUnitIndex = 0, n1UIndex = 0;

	for(int i=0; i<m_nFamilyNum; i++)
	{
		nUnitIndex = i/MAX_FAMILY_1U_NUM;
		n1UIndex = i%MAX_FAMILY_1U_NUM;

		if (m_pcsFamily[nUnitIndex][n1UIndex].Get()->nID == nFamilyID)
		{
			return &m_pcsFamily[nUnitIndex][n1UIndex];
		}
	}

	return NULL;
}

// -------------------------------------------------------------------------------------------------------------------------------------
// GetFamily Method																											 패밀리 얻기
//
CSHFamily* CSHFamilyManager::GetFamily(char* pszName)
{
	int nUnitIndex = 0, n1UIndex = 0;
	for(int i=0; i<m_nFamilyNum; i++)
	{
		nUnitIndex = i/MAX_FAMILY_1U_NUM;
		n1UIndex = i%MAX_FAMILY_1U_NUM;

		if (m_pcsFamily[nUnitIndex][n1UIndex].Get()->nID)
		{
			if (stricmp(m_pcsFamily[nUnitIndex][n1UIndex].Get()->szName, pszName) == 0)
			{
				return &m_pcsFamily[nUnitIndex][n1UIndex];
			}
		}
	}

	return NULL;
}

// -------------------------------------------------------------------------------------------------------------------------------------
// GetFamilyByMasterID Method																								 패밀리 얻기
//
CSHFamily* CSHFamilyManager::GetFamilyByMasterID(DWORD nMasterID)
{
	int nUnitIndex = 0, n1UIndex = 0;
	for(int i=0; i<m_nFamilyNum; i++)
	{
		nUnitIndex = i/MAX_FAMILY_1U_NUM;
		n1UIndex = i%MAX_FAMILY_1U_NUM;

		if (m_pcsFamily[nUnitIndex][n1UIndex].Get()->nID)
		{
			if (m_pcsFamily[nUnitIndex][n1UIndex].Get()->nMasterID == nMasterID)
			{
				return &m_pcsFamily[nUnitIndex][n1UIndex];
			}
		}
	}

	return NULL;
}

// -------------------------------------------------------------------------------------------------------------------------------------
// Process Method																											
//
VOID CSHFamilyManager::Process()
{
#if defined(_AGENTSERVER)
	static DWORD nBeforeTimeTick = gCurTime;

	// 60초 마다 체크
	// 071219 LUJ, 1시간마다 체크
	const DWORD space = 60 * 60 * 1000;

	if (gCurTime-nBeforeTimeTick > space )
	{
		nBeforeTimeTick = gCurTime;
		int nUnitIndex = 0, n1UIndex = 0;

		for(int i=0; i<m_nFamilyNum; i++)
		{
			nUnitIndex = i/MAX_FAMILY_1U_NUM;
			n1UIndex = i%MAX_FAMILY_1U_NUM;

			if (m_pcsFamily[nUnitIndex][n1UIndex].Get()->nID)
			{
				if (m_pcsFamily[nUnitIndex][n1UIndex].GetEx()->nHonorPointTimeTick)
				{
					if (gCurTime - m_pcsFamily[nUnitIndex][n1UIndex].GetEx()->nHonorPointTimeTick > HONOR_POINT_CHECK_TIME)
					{
						// 명예 포인트 지급
						CSHFamily::stINFO_EX	stInfoEx = *m_pcsFamily[nUnitIndex][n1UIndex].GetEx();
						stInfoEx.nHonorPoint += HONOR_POINT_CHECK_POINT;
						stInfoEx.nHonorPointTimeTick = gCurTime;

						if (stInfoEx.nHonorPoint > 4200000000) stInfoEx.nHonorPoint = 4200000000;
						m_pcsFamily[nUnitIndex][n1UIndex].SetEx(&stInfoEx);

						// DB에 저장
						Family_SaveInfo(m_pcsFamily[nUnitIndex][n1UIndex].Get()->nMasterID, 
										m_pcsFamily[nUnitIndex][n1UIndex].Get()->szName, 
										m_pcsFamily[nUnitIndex][n1UIndex].GetEx()->nHonorPoint, 
										m_pcsFamily[nUnitIndex][n1UIndex].GetEx()->nNicknameON,
										1);

						// 071114 웅주, 로그
						{
							CSHFamily*	family	= &( m_pcsFamily[nUnitIndex][n1UIndex] );
							ASSERT( family );

							InsertLogFamilyPoint( family, eLog_FamilyPointAdd );
						}

						// 클라이언트에 알리기
						{
							MSG_DWORD stPacket;
							for(UINT i=0; i<m_pcsFamily[nUnitIndex][n1UIndex].Get()->nMemberNum; i++)
							{
								USERINFO* pMemberUserInfo = g_pUserTableForObjectID->FindUser(m_pcsFamily[nUnitIndex][n1UIndex].GetMember(i)->Get()->nID);
								if (pMemberUserInfo)
								{
									stPacket.Category			= MP_FAMILY;
									stPacket.Protocol			= MP_FAMILY_HONOR_POINT;
									stPacket.dwObjectID			= pMemberUserInfo->dwCharacterID;
									stPacket.dwData				= HONOR_POINT_CHECK_POINT;

									g_Network.Send2User( pMemberUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
								}
							}
						}

						// 다른 에이전트에 알리기
						MSG_NAME_DWORD3 stPacket;
						ZeroMemory(&stPacket, sizeof(stPacket));

						stPacket.Category			= MP_FAMILY;
						stPacket.Protocol			= MP_FAMILY_HONOR_POINT_TO_OTHER_AGENT;
						stPacket.dwData1			= m_pcsFamily[nUnitIndex][n1UIndex].Get()->nID;
						stPacket.dwData2			= HONOR_POINT_CHECK_POINT;
						stPacket.dwData3			= 1;
						g_Network.Broadcast2AgentServerExceptSelf((char*)&stPacket, sizeof(stPacket));	
					}
				}
			}
		}
	}
#endif
}
// -------------------------------------------------------------------------------------------------------------------------------------
// ASRV_CreateFamily Method																									 패밀리 생성
//
VOID CSHFamilyManager::ASRV_CreateFamily(USERINFO* pUserInfo, char* szFamilyName, int nCheckState, CSHFamily* pcsFamily)
{
#if defined(_AGENTSERVER)
	RESULT eResult = RESULT_OK;
	int nPastTime = 0;

	// 이미 패밀리에 속해 있나?
	if (pUserInfo->pcsFamily)
	{
		eResult = RESULT_FAIL_01;
		goto FAILED;
	}

	if (szFamilyName == NULL)
	{
		eResult = RESULT_FAIL_02;
		goto FAILED;
	}

	// 이름이 최대 길이를 초과했나?
	if (strlen(szFamilyName) > MAX_NAME_LENGTH)
	{
		eResult = RESULT_FAIL_02;
		goto FAILED;
	}

	// 사용할 수 없는 이름인가?
	if((FILTERTABLE->IsInvalidCharInclude((unsigned char*)szFamilyName)))
	{
		eResult = RESULT_FAIL_02;
		goto FAILED;
	}

	// 탈퇴 페널티 기한이 지났나?
	// ..DB에 int 로 0710111524(년월일시간분)와 같이 저장됨. 설마 int 로 인한 날짜문제가 생길만큼 오래 서비스 되진...ㅋ
	if (pUserInfo->nFamilyLeaveDate)
	{
		nPastTime = GetPastTime(pUserInfo->nFamilyLeaveDate);
		// 최종 시간 체크
		if (nPastTime < LEAVE_PENALTY_REJOIN_TIME[pUserInfo->nFamilyLeaveKind])
		{
			eResult = RESULT_FAIL_03;
			switch(pUserInfo->nFamilyLeaveKind)
			{
			case FLK_LEAVE:		eResult = RESULT_FAIL_03;	break;
			case FLK_EXPEL:		eResult = RESULT_FAIL_04;	break;
			case FLK_BREAKUP:	eResult = RESULT_FAIL_05;	break;
			}
			goto FAILED;
		}
		// 기한 초기화
		Family_Leave_SaveInfo(pUserInfo->dwCharacterID, pUserInfo->nFamilyLeaveKind, TRUE);
	}

	switch(nCheckState)
	{
	// 이름 체크1
	case FCS_NAME_CHECK1:
		{
			// 이미 DB상에 존재하는 패밀리 이름인지 체크 쿼리를 DB에 보낸다.
			Family_CheckName(pUserInfo->dwCharacterID, szFamilyName);
			return;
		}
		break;

	// 이름 체크1 에러
	case FCS_NAME_ERR:
		{
			eResult = RESULT_FAIL_02;
			goto FAILED;
		}
		break;

	// 이름 체크2
	case FCS_NAME_CHECK2:
		{
			// DB에는 없지만, 혹시 서버의 테이블에 존재하는 이름인가?
			// 이름 체크 쿼리를 DB에 보냈을 때 응답이 늦어지는 일이 발생할 경우에는 DB 체크만으로는 정확성을 보장할 수 없다. 특히나 멀티스레드의 경우.
			// 그래서 서버의 테이블도 체크한다.
			if (GetFamily(szFamilyName))
			{
				eResult = RESULT_FAIL_02;
				goto FAILED;
			}

			// 맵서버의 정보를 이용한 조건 체크를 위해 맵서버에 패밀리 생성 패킷을 보낸다.
			MSG_NAME stPacket;

			stPacket.Category			= MP_FAMILY;
			stPacket.Protocol			= MP_FAMILY_CREATE;
			stPacket.dwObjectID			= pUserInfo->dwCharacterID;
			SafeStrCpy(stPacket.Name, szFamilyName, MAX_NAME_LENGTH+1);
			g_Network.Send2Server(pUserInfo->dwMapServerConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
			return;
		}
		break;

	// 생성
	case FCS_CREATE:
		{
			// 바로 생성하는 건 아니고, 생성할 정보를 이용해서 DB에 저장을 시도한다.
			CSHFamily csFamily(pUserInfo->dwCharacterID, szFamilyName);
			Family_SaveInfo(pUserInfo->dwCharacterID, szFamilyName, csFamily.GetEx()->nHonorPoint, csFamily.GetEx()->nNicknameON);
			return;
		}
		break;

	// 생성완료
	case FCS_COMPLETE:
		{
			// 패밀리 정보 설정
			// ..테이블에 추가
			pUserInfo->pcsFamily = AddFamilyToTbl(pcsFamily);
			// ..클라이언트에 패밀리 정보 보내기
			ASRV_SendInfoToClient(pUserInfo->pcsFamily);
		}
		break;
	}

FAILED:
	// 클라이언트에 알리기
	MSG_DWORD2 stPacket;

	stPacket.Category			= MP_FAMILY;
	stPacket.Protocol			= MP_FAMILY_CREATE;
	stPacket.dwObjectID			= pUserInfo->dwCharacterID;
	stPacket.dwData1			= eResult;
	stPacket.dwData2			= nPastTime;
	g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ASRV_ParseRequestFromClient Method																 클라이언트 요청 분석/처리(에이전트)
//
VOID CSHFamilyManager::ASRV_ParseRequestFromClient(DWORD dwConnectionID, char* pMsg, DWORD dwLength)
{
#if defined(_AGENTSERVER)
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	
	// 클라이언트에서 패킷 조작으로 남의 캐릭터 ID 를 보낼 수 있기 때문에
	// g_pUserTableForObjectID->FindUser 로 UserInfo 를 구하면 안 됨!!
	USERINFO* pUserInfo = g_pUserTable->FindUser( dwConnectionID );
	if( pUserInfo == NULL ) return;
	pTempMsg->dwObjectID = pUserInfo->dwCharacterID;

	switch( pTempMsg->Protocol )
	{
	// 패밀리 생성
	case MP_FAMILY_CREATE:
		{
			MSG_DWORD_NAME* pstPacket = (MSG_DWORD_NAME*)pMsg;

			ASRV_CreateFamily(pUserInfo, pstPacket->Name, CSHFamilyManager::FCS_NAME_CHECK1);
		}
		break;
	// 멤버로 초대
	case MP_FAMILY_INVITE:
		{
			MSG_DWORD* pstPacket = (MSG_DWORD*)pMsg;

			ASRV_Invite(pUserInfo, pstPacket->dwData);
		}
		break;
	// 호칭 변경
	case MP_FAMILY_CHANGE_NICKNAME:
		{
			MSG_DWORD_NAME* pstPacket = (MSG_DWORD_NAME*)pMsg;

			ASRV_ChangeNickname(pUserInfo, pstPacket->dwData, pstPacket->Name);
		}
		break;
	// 문장 등록
	case MP_FAMILY_REGIST_EMBLEM:
		{
			PACKET_FAMILY_EMBLEM* pstPacket = (PACKET_FAMILY_EMBLEM*)pMsg;

			ASRV_RegistEmblem(pUserInfo, pstPacket->nFamilyID, pstPacket->cImgData);
		}
		break;
	// 탈퇴
	case MP_FAMILY_LEAVE:
		{
			MSGBASE* pstPacket = (MSGBASE*)pMsg;

			ASRV_Leave(pUserInfo);
		}
		break;
	// 추방
	case MP_FAMILY_EXPEL:
		{
			MSG_DWORD* pstPacket = (MSG_DWORD*)pMsg;

			ASRV_Expel(pUserInfo, pstPacket->dwData);
		}
		break;
	// 해체
	case MP_FAMILY_BREAKUP:
		{
			MSGBASE* pstPacket = (MSGBASE*)pMsg;

			ASRV_BreakUp(pUserInfo);
		}
		break;
	default:
		g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, pMsg, dwLength );
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ASRV_ParseRequestFromServer Method																	   서버 요청 분석/처리(에이전트)
//
VOID CSHFamilyManager::ASRV_ParseRequestFromServer(DWORD dwConnectionID, char* pMsg, DWORD dwLength)
{
#if defined(_AGENTSERVER)
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	
	// USERINFO 를 참조하지 않는 경우
	//
	switch( pTempMsg->Protocol )
	{
	// 다른 에이전트로부터의 멤버 접속 처리
	case MP_FAMILY_MEMBER_CONNECTION_TO_OTHER_AGENT:
		{
			MSG_DWORD4* pstPacket = (MSG_DWORD4*)pMsg;

			ASRV_MemberConFromOtherAgent(pstPacket->dwData1, pstPacket->dwData2, pstPacket->dwData3, pstPacket->dwData4);
		}
		return;
	// 다른 에이전트로부터의 호칭 변경 알림
	case MP_FAMILY_CHANGE_NICKNAME:
		{
			MSG_NAME_DWORD3* pstPacket = (MSG_NAME_DWORD3*)pMsg;

			ASRV_ChangeNicknameFromOtherAgent(pstPacket->dwData1, pstPacket->dwData2, pstPacket->dwData3, pstPacket->Name);
		}
		return;
	// 다른 에이전트로부터의 맵의 플레이어 정보 업데이트
	case MP_FAMILY_UPDATE_MAP_PLAYER_INFO_TO_OTHER_AGENT:
		{
			MSG_NAME_DWORD3* pstPacket = (MSG_NAME_DWORD3*)pMsg;

			ASRV_UpdateMapPlayerInfoFromOtherAgent(pstPacket->dwData1, pstPacket->dwData2, pstPacket->dwData3, pstPacket->Name);
		}
		return;
	// 다른 에이전트로부터의 명예 포인트
	case MP_FAMILY_HONOR_POINT_TO_OTHER_AGENT:
		{
			MSG_DWORD3* pstPacket = (MSG_DWORD3*)pMsg;

			ASRV_UpdateHonorPointFromOtherAgent(pstPacket->dwData1, pstPacket->dwData2, pstPacket->dwData3);
		}
		return;
	// 다른 에이전트로부터의 초대 결과
	case MP_FAMILY_INVITE_RESULT_TO_OTHER_AGENT:
		{
			PACKET_ADD_MEMBER* pstPacket = (PACKET_ADD_MEMBER*)pMsg;

			ASRV_InviteResultFromOtherAgent(pstPacket);
		}
		return;
	// 다른 에이전트로부터의 초대 결과
	case MP_FAMILY_INVITE_RESULT_TO_OTHER_AGENT_3:
		{
			PACKET_FAMILY_TOTAL* pstPacket = (PACKET_FAMILY_TOTAL*)pMsg;

			ASRV_InviteResultFromOtherAgent2(pstPacket);
		}
		return;
	// 다른 에이전트로부터의 문장 등록
	case MP_FAMILY_REGIST_EMBLEM_TO_OTHER_AGENT:
		{
			PACKET_FAMILY_EMBLEM* pstPacket = (PACKET_FAMILY_EMBLEM*)pMsg;

			ASRV_RegistEmblemFromOtherAgent(pstPacket->nFamilyID, pstPacket->cImgData);
		}
		return;
	// 다른 에이전트로부터의 탈퇴
	case MP_FAMILY_LEAVE_TO_OTHER_AGENT:
		{
			MSG_DWORD* pstPacket = (MSG_DWORD*)pMsg;

			ASRV_LeaveFromOtherAgent(pstPacket->dwData, pstPacket->dwObjectID);
		}
		return;
	// 다른 에이전트로부터의 추방
	case MP_FAMILY_EXPEL_TO_OTHER_AGENT:
		{
			MSG_DWORD2* pstPacket = (MSG_DWORD2*)pMsg;

			ASRV_ExpelFromOtherAgent(pstPacket->dwData1, pstPacket->dwObjectID);
		}
		return;
	// 다른 에이전트로부터의 해체
	case MP_FAMILY_BREAKUP_TO_OTHER_AGENT:
		{
			MSG_DWORD2* pstPacket = (MSG_DWORD2*)pMsg;

			ASRV_BreakUpFromOtherAgent(pstPacket->dwData1);
		}
		return;
	}


	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pTempMsg->dwObjectID);
	if (pUserInfo == NULL) return;


	switch( pTempMsg->Protocol )
	{
	// 패밀리 생성
	case MP_FAMILY_CREATE:
		{
			MSG_DWORD_NAME* pstPacket = (MSG_DWORD_NAME*)pMsg;

			if (pstPacket->dwData == RESULT_OK)
			{
				// 모든 조건을 만족해서 패밀리 생성!
				ASRV_CreateFamily(pUserInfo, pstPacket->Name, CSHFamilyManager::FCS_CREATE);
			}
		}
		break;
	// 초대 결과
	case MP_FAMILY_INVITE_RESULT:
		{
			PACKET_ADD_MEMBER* pstPacket = (PACKET_ADD_MEMBER*)pMsg;

			ASRV_InviteResult(pUserInfo, pstPacket);
		}
		break;
	// 명예 포인트
	case MP_FAMILY_HONOR_POINT:
		{
			MSG_DWORD* pstPacket = (MSG_DWORD*)pMsg;

			ASRV_UpdateHonorPoint(pUserInfo, pstPacket->dwData);
		}
		break;
	// 맵의 플레이어 정보 업데이트
	case MP_FAMILY_UPDATE_MAP_PLAYER_INFO:
		{
			MSG_DWORD_NAME* pstPacket = (MSG_DWORD_NAME*)pMsg;

			ASRV_UpdateMapPlayerInfo(pUserInfo, pstPacket->dwData, pstPacket->Name);

		}
		break;
	// 다른 에이전트로부터의 초대
	case MP_FAMILY_INVITED:
		{
			MSG_DWORD_NAME* pstPacket = (MSG_DWORD_NAME*)pMsg;

			ASRV_InviteFromOtherAgent(pUserInfo, pstPacket->dwData, pstPacket->Name);
		}
		break;

	default:
		g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)pMsg, dwLength );
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
// ASRV_Invite Method																							  패밀리로 초대(에이전트)
//
VOID CSHFamilyManager::ASRV_Invite(USERINFO* pUserInfo, DWORD nTargetPlayerID)
{
#if defined(_AGENTSERVER)
	RESULT eResult = RESULT_OK;
	int nPastTime = 0;

	// 패밀리에 속해 있나?
	if (pUserInfo->pcsFamily == NULL)
	{
		eResult = RESULT_FAIL_01;
		goto FAILED;
	}

	// 마스터인가?
	if (pUserInfo->dwCharacterID != pUserInfo->pcsFamily->Get()->nMasterID)
	{
		eResult = RESULT_FAIL_02;
		goto FAILED;
	}

	// 최대 멤버 수를 넘지 않나?
	if (pUserInfo->pcsFamily->Get()->nMemberNum == pUserInfo->pcsFamily->GetMemberNumMax())
	{
		eResult = RESULT_FAIL_03;
		goto FAILED;
	}

	// 추방 페널티 기한이 지났나?
	// ..DB에 int 로 0710111524(년월일시간분)와 같이 저장됨. 설마 int 로 인한 날짜문제가 생길만큼 오래 서비스 되진...ㅋ
	if (pUserInfo->nFamilyLeaveDate)
	{
		nPastTime = GetPastTime(pUserInfo->nFamilyLeaveDate);
		// 최종 시간 체크
		if (nPastTime < LEAVE_PENALTY_REJOIN_TIME[pUserInfo->nFamilyLeaveKind])
		{
			eResult = RESULT_FAIL_05;
			goto FAILED;
		}
		// 기한 초기화
		Family_Leave_SaveInfo(pUserInfo->dwCharacterID, pUserInfo->nFamilyLeaveKind, TRUE);
	}

	USERINFO* pTargetUserInfo = g_pUserTableForObjectID->FindUser(nTargetPlayerID);
	if (pTargetUserInfo)
	{
		// 타겟 플레이어가 다른 패밀리의 멤버가 아닌가?
		if (pTargetUserInfo->pcsFamily)
		{
			eResult = RESULT_FAIL_04;
			goto FAILED;
		}

		// 탈퇴 페널티 기한이 지났나?
		// ..DB에 int 로 2007101115(년월일시간)와 같이 저장됨. 설마 int 로 인한 날짜문제가 생길만큼 오래 서비스 되진...ㅋ
		if (pTargetUserInfo->nFamilyLeaveDate)
		{
			nPastTime = GetPastTime(pTargetUserInfo->nFamilyLeaveDate);
			// 최종 시간 체크
			if (nPastTime < LEAVE_PENALTY_REJOIN_TIME[pTargetUserInfo->nFamilyLeaveKind])
			{
				eResult = RESULT_FAIL_06;
				goto FAILED;
			}
			// 기한 초기화
			Family_Leave_SaveInfo(pTargetUserInfo->dwCharacterID, pTargetUserInfo->nFamilyLeaveKind, TRUE);
		}
	}
	else
	{
		// 다른 에이전트가 관리하는 플레이어다. 다른 에이전트에 알리기
		MSG_DWORD_NAME stPacket;
		stPacket.Category			= MP_FAMILY;
		stPacket.Protocol			= MP_FAMILY_INVITED;
		stPacket.dwObjectID			= nTargetPlayerID;
		stPacket.dwData				= pUserInfo->dwCharacterID;
		SafeStrCpy(stPacket.Name, pUserInfo->pcsFamily->Get()->szName, MAX_NAME_LENGTH+1);
		g_Network.Broadcast2AgentServerExceptSelf((char*)&stPacket, sizeof(stPacket));
		return;
	}

FAILED:
	if (eResult == RESULT_OK)
	{
		MSG_DWORD_NAME stPacket;
		stPacket.Category			= MP_FAMILY;
		stPacket.Protocol			= MP_FAMILY_INVITED;
		stPacket.dwObjectID			= nTargetPlayerID;
		stPacket.dwData				= pUserInfo->dwCharacterID;
		SafeStrCpy(stPacket.Name, pUserInfo->pcsFamily->Get()->szName, MAX_NAME_LENGTH+1);
		g_Network.Send2User( pTargetUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
	}

	MSG_DWORD2 stPacket;

	stPacket.Category			= MP_FAMILY;
	stPacket.Protocol			= MP_FAMILY_INVITE;
	stPacket.dwObjectID			= pUserInfo->dwCharacterID;
	stPacket.dwData1			= eResult;
	stPacket.dwData2			= nPastTime;
	g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
// ASRV_Invite Method																						패밀리로 초대 결과(에이전트)
//
VOID CSHFamilyManager::ASRV_InviteResult(USERINFO* pUserInfo, PACKET_ADD_MEMBER* pstInfo)
{
#if defined(_AGENTSERVER)
	RESULT eResult = RESULT_OK;

	// 플레이어가 이미 패밀리에 속해 있나?
	if (pUserInfo->pcsFamily)
	{
		return;
	}

	// 마스터가 유효한가?
	USERINFO* pMasterUserInfo = g_pUserTableForObjectID->FindUser(pstInfo->nMasterID);
	if (pMasterUserInfo == NULL)
	{
		// 다른 에이전트에서 관리하는 플레이어일 수 있다.
		pstInfo->Protocol = MP_FAMILY_INVITE_RESULT_TO_OTHER_AGENT;
		g_Network.Broadcast2AgentServerExceptSelf((char*)pstInfo, sizeof(*pstInfo));
		return;
	}

	// 마스터가 맞나?
	if (pMasterUserInfo->pcsFamily == NULL || pstInfo->nMasterID != pMasterUserInfo->pcsFamily->Get()->nMasterID)
	{
		return;
	}

	// 결과 보내기
	MSG_NAME_DWORD2 stPacket;
	// ..마스터에게 결과 보내기
	stPacket.Category			= MP_FAMILY;
	stPacket.Protocol			= MP_FAMILY_INVITE_RESULT;
	stPacket.dwObjectID			= pMasterUserInfo->dwCharacterID;
	stPacket.dwData1			= pUserInfo->dwCharacterID;
	if (pstInfo->nResult == CSHFamilyManager::FIR_ACCEPT_PLAYER)	stPacket.dwData2 = FIR_ACCEPT_MASTER;
	else															stPacket.dwData2 = FIR_DENY_MASTER;
	g_Network.Send2User( pMasterUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );

	// ..타겟 플레이어에게 결과 보내기
	stPacket.Category			= MP_FAMILY;
	stPacket.Protocol			= MP_FAMILY_INVITE_RESULT;
	stPacket.dwObjectID			= pUserInfo->dwCharacterID;
	stPacket.dwData1			= pMasterUserInfo->dwCharacterID;
	stPacket.dwData2			= pstInfo->nResult;
	SafeStrCpy(stPacket.Name, pMasterUserInfo->pcsFamily->Get()->szName, MAX_NAME_LENGTH+1);
	g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );

	// 승락
	if (pstInfo->nResult == FIR_ACCEPT_PLAYER)
	{
		// 멤버에 추가
		// ..혹시 모르니까 멤버 수 체크
		if (pMasterUserInfo->pcsFamily->Get()->nMemberNum == pMasterUserInfo->pcsFamily->GetMemberNumMax())
		{
			return;
		}

		CSHFamilyMember csMember;
		CSHFamilyMember::stINFO stInfo;

		ZeroMemory(&stInfo, sizeof(stInfo));
		stInfo.nID			= pUserInfo->dwCharacterID;
		stInfo.nRace		= pstInfo->nRace;
		stInfo.nSex			= pstInfo->nSex;
		stInfo.nJobFirst	= pstInfo->nJobFirst;
		stInfo.nJobCur		= pstInfo->nJobCur;
		stInfo.nJobGrade	= pstInfo->nJobGrade;
		stInfo.nLV			= pstInfo->nLV;
		stInfo.eConState	= CSHFamilyMember::MEMBER_CONSTATE_LOGIN;
		SafeStrCpy(stInfo.szName, pstInfo->szName, MAX_NAME_LENGTH+1);

		csMember.Set(&stInfo);
		pMasterUserInfo->pcsFamily->AddMember(&csMember);
		pUserInfo->pcsFamily = pMasterUserInfo->pcsFamily;

		// DB에 저장
		Family_Member_SaveInfo(pMasterUserInfo->pcsFamily->Get()->nID, pUserInfo->dwCharacterID, NULL);
		// 081205 LUJ, 로그
		InsertLogFamily(
			eLog_FamilyAddMember,
			pMasterUserInfo->pcsFamily->Get()->nID,
			pUserInfo->dwCharacterID,
			pMasterUserInfo->pcsFamily->Get()->szName );
		// 멤버가 된 플레이어에게 문장 전송
		Family_LoadEmblemInfo(pUserInfo->dwCharacterID, pUserInfo->pcsFamily->Get()->nID);

		// 클라이언트에 패밀리 정보 보내기
		ASRV_SendInfoToClient(pMasterUserInfo->pcsFamily);

		// 다른 에이전트의 멤버들에게도 보내기..
		{
			PACKET_FAMILY_TOTAL stPacket;
			int nLoginCnt = 0;

			ZeroMemory(&stPacket, sizeof(stPacket));
			stPacket.Category			= MP_FAMILY;
			stPacket.Protocol			= MP_FAMILY_INVITE_RESULT_TO_OTHER_AGENT_3;
			stPacket.dwObjectID			= pstInfo->dwObjectID;
			stPacket.stFamily.stInfo	= *pMasterUserInfo->pcsFamily->Get();
			stPacket.stFamily.stInfoEx	= *pMasterUserInfo->pcsFamily->GetEx();
			for(UINT i=0; i<pMasterUserInfo->pcsFamily->Get()->nMemberNum; i++)
			{
				stPacket.stMember[i].stInfo		= *pMasterUserInfo->pcsFamily->GetMember(i)->Get();
				stPacket.stMember[i].stInfoEx	= *pMasterUserInfo->pcsFamily->GetMember(i)->GetEx();
				stPacket.stMember[i].nIndex		= i;
				if (stPacket.stMember[i].stInfo.eConState == CSHFamilyMember::MEMBER_CONSTATE_LOGIN)
				{
					nLoginCnt++;
				}
			}

			// 현재 접속해 있는 멤버가 HONOR_POINT_CHECK_MEMBER_NUM명이 되면 명예 포인트 시간을 설정한다.
			if (nLoginCnt == CSHFamilyManager::HONOR_POINT_CHECK_MEMBER_NUM)
			{
				CSHFamily::stINFO_EX stInfoEx = *pMasterUserInfo->pcsFamily->GetEx();
				stInfoEx.nHonorPointTimeTick = gCurTime;
				pMasterUserInfo->pcsFamily->SetEx(&stInfoEx);
			}

			g_Network.Broadcast2AgentServerExceptSelf((char*)&stPacket, sizeof(stPacket));
		}

		// 맵서버에 농장UI 정보 요청
		g_csFarmManager.ASRV_RequestFarmUIInfoToMap(pUserInfo->dwCharacterID, pMasterUserInfo->pcsFamily);
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ASRV_ChangeNickname Method																								   호칭 변경
//
VOID CSHFamilyManager::ASRV_ChangeNickname(USERINFO* pUserInfo, DWORD nTargetPlayerID, char* pszNickname)
{
#if defined(_AGENTSERVER)
	RESULT eResult = RESULT_OK;
	USERINFO* pTargetUserInfo = g_pUserTableForObjectID->FindUser(nTargetPlayerID);

	// 패밀리에 속해 있나?
	if (pUserInfo->pcsFamily == NULL)
	{
		return;
	}

	// 이름이 최대 길이를 초과했나?
	if (strlen(pszNickname) > MAX_NAME_LENGTH)
	{
		eResult = RESULT_FAIL_01;
		goto FAILED;
	}
/*
	// 사용할 수 없는 이름인가?
	if((FILTERTABLE->IsInvalidCharInclude((unsigned char*)pszNickname)))
	{
		eResult = RESULT_FAIL_01;
		goto FAILED;
	}
*/
	BOOL bMember = FALSE;
	for(UINT i=0; i<pUserInfo->pcsFamily->Get()->nMemberNum; i++)
	{
		if (nTargetPlayerID == pUserInfo->pcsFamily->GetMember(i)->Get()->nID)
		{
			CSHFamilyMember::stINFO		stInfo		= *pUserInfo->pcsFamily->GetMember(i)->Get();
			
			SafeStrCpy(stInfo.szNickname, pszNickname, MAX_NAME_LENGTH+1);
			pUserInfo->pcsFamily->GetMember(i)->Set(&stInfo);
			bMember = TRUE;
			break;
		}
	}

	// 같은 패밀리 멤버인가?
	if (bMember == FALSE)
	{
		return;
	}

	// 클라이언트에 알리기
	ASRV_SendInfoToClient(pUserInfo->pcsFamily, 2);
	// DB에 저장
	Family_Member_SaveInfo(pUserInfo->pcsFamily->Get()->nID, nTargetPlayerID, pszNickname);
	// 081205 LUJ, 로그
	InsertLogFamily(
		eLog_FamilyChangeNick,
		pUserInfo->pcsFamily->Get()->nID,
		nTargetPlayerID,
		pszNickname );

	if (pTargetUserInfo)
	{
		// 맵서버에 알리기
		ASRV_SendNicknameToMap(pTargetUserInfo, pszNickname);

		// 다른 에이전트에 알리기
		MSG_NAME_DWORD3 stPacket;
		ZeroMemory(&stPacket, sizeof(stPacket));

		stPacket.Category			= MP_FAMILY;
		stPacket.Protocol			= MP_FAMILY_CHANGE_NICKNAME;
		stPacket.dwData1			= pUserInfo->pcsFamily->Get()->nID;
		stPacket.dwData2			= NULL;
		stPacket.dwData3			= nTargetPlayerID;
		SafeStrCpy(stPacket.Name, pszNickname, MAX_NAME_LENGTH+1);

		g_Network.Broadcast2AgentServerExceptSelf((char*)&stPacket, sizeof(stPacket));	
	}
	
FAILED:
	// 클라이언트에 알리기
	MSG_DWORD_NAME stPacket;
	// ..신청자에게 알리기
	ZeroMemory(&stPacket, sizeof(stPacket));
	stPacket.Category			= MP_FAMILY;
	stPacket.Protocol			= MP_FAMILY_CHANGE_NICKNAME;
	stPacket.dwObjectID			= pUserInfo->dwCharacterID;
	stPacket.dwData				= eResult;
	for(UINT i=0; i<pUserInfo->pcsFamily->Get()->nMemberNum; i++)
	{
		if (nTargetPlayerID == pUserInfo->pcsFamily->GetMember(i)->Get()->nID)
		{
			SafeStrCpy(stPacket.Name, pUserInfo->pcsFamily->GetMember(i)->Get()->szName, MAX_NAME_LENGTH+1);
			break;
		}
	}
	g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );

	// ..대상자에게 알리기
	if (eResult == RESULT_OK)
	{
		if (pTargetUserInfo)
		{
			if (pUserInfo != pTargetUserInfo)
			{
				ZeroMemory(&stPacket, sizeof(stPacket));
				stPacket.Category			= MP_FAMILY;
				stPacket.Protocol			= MP_FAMILY_CHANGE_NICKNAME;
				stPacket.dwObjectID			= pTargetUserInfo->dwCharacterID;
				stPacket.dwData				= RESULT_OK_02;

				for(UINT i=0; i<pUserInfo->pcsFamily->Get()->nMemberNum; i++)
				{
					if (pUserInfo->dwCharacterID == pUserInfo->pcsFamily->GetMember(i)->Get()->nID)
					{
						SafeStrCpy(stPacket.Name, pUserInfo->pcsFamily->GetMember(i)->Get()->szName, MAX_NAME_LENGTH+1);
						break;
					}
				}
				g_Network.Send2User( pTargetUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
			}
		}
		else
		{
			// 다른 에이전트가 관리하는 플레이어라면 거기서 처리해야 하니까..
			MSG_NAME_DWORD3 stPacket;
			ZeroMemory(&stPacket, sizeof(stPacket));

			stPacket.Category			= MP_FAMILY;
			stPacket.Protocol			= MP_FAMILY_CHANGE_NICKNAME;
			stPacket.dwData1			= pUserInfo->pcsFamily->Get()->nID;
			stPacket.dwData2			= pUserInfo->dwCharacterID;
			stPacket.dwData3			= nTargetPlayerID;
			SafeStrCpy(stPacket.Name, pszNickname, MAX_NAME_LENGTH+1);

			g_Network.Broadcast2AgentServerExceptSelf((char*)&stPacket, sizeof(stPacket));
		}
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ASRV_UpdateHonorPoint Method														  명예 포인트 처리 (몬스터 사냥시 경험치에 의한)
//
VOID CSHFamilyManager::ASRV_UpdateHonorPoint(USERINFO* pUserInfo, DWORD nHonorPoint)
{
#if defined(_AGENTSERVER)
	if (pUserInfo->pcsFamily == NULL)
	{
		return;
	}

	CSHFamily::stINFO_EX stInfoEx = *pUserInfo->pcsFamily->GetEx();

	// 농장 구입이나 업그레이드의 경우는 절감 포인트값을 - int 로 넘긴다. 그래서 int 로 캐스팅해서 - 값 인지 체크해서
	// 그럴 경우에는 -= 하도록 한다. 절감 포인트값이 int 범위라는 가정...
	if ((int)nHonorPoint < 0)
	{
		if (stInfoEx.nHonorPoint >= (DWORD)(-1*(int)nHonorPoint))
		{
			stInfoEx.nHonorPoint -= (DWORD)(-1*(int)nHonorPoint);
		}
		else
		{
			return;
		}
	}
	else
	{
		stInfoEx.nHonorPoint += nHonorPoint;
		if (stInfoEx.nHonorPoint > 4200000000) stInfoEx.nHonorPoint = 4200000000;
	}

	pUserInfo->pcsFamily->SetEx(&stInfoEx);

	MSG_DWORD stPacket;
	// 클라이언트에 알리기
	for(UINT i=0; i<pUserInfo->pcsFamily->Get()->nMemberNum; i++)
	{
		USERINFO* pMemberUserInfo = g_pUserTableForObjectID->FindUser(pUserInfo->pcsFamily->GetMember(i)->Get()->nID);
		if (pMemberUserInfo)
		{
			stPacket.Category			= MP_FAMILY;
			stPacket.Protocol			= MP_FAMILY_HONOR_POINT;
			stPacket.dwObjectID			= pMemberUserInfo->dwCharacterID;
			stPacket.dwData				= nHonorPoint;

			g_Network.Send2User( pMemberUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
		}
	}

	// DB에 저장
	Family_SaveInfo(pUserInfo->pcsFamily->Get()->nMasterID, 
					pUserInfo->pcsFamily->Get()->szName, 
					pUserInfo->pcsFamily->GetEx()->nHonorPoint, 
					pUserInfo->pcsFamily->GetEx()->nNicknameON,
					1);

	// 071114 웅주, 로그
	// 080610 LUJ, 로그 타입 변경
	InsertLogFamilyPoint( pUserInfo->pcsFamily, eLog_FamilyPointUpdate );

	// 다른 에이전트에 알리기
	{
		MSG_DWORD3 stPacket;
		ZeroMemory(&stPacket, sizeof(stPacket));

		stPacket.Category			= MP_FAMILY;
		stPacket.Protocol			= MP_FAMILY_HONOR_POINT_TO_OTHER_AGENT;
		stPacket.dwData1			= pUserInfo->pcsFamily->Get()->nID;
		stPacket.dwData2			= nHonorPoint;
		stPacket.dwData3			= 0;
		g_Network.Broadcast2AgentServerExceptSelf((char*)&stPacket, sizeof(stPacket));
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ASRV_UpdateMapPlayerInfo Method																			 맵의 플레이어 정보 업데이트
//
VOID CSHFamilyManager::ASRV_UpdateMapPlayerInfo(USERINFO* pUserInfo, DWORD nLevel, char* pszGuildName)
{
#if defined(_AGENTSERVER)
	if (pUserInfo->pcsFamily == NULL)
	{
		return;
	}

	// 업데이트 된 정보 불러오기
	for(UINT i=0; i<pUserInfo->pcsFamily->Get()->nMemberNum; i++)
	{
		if (pUserInfo->dwCharacterID == pUserInfo->pcsFamily->GetMember(i)->Get()->nID)
		{
			CSHFamilyMember::stINFO		stInfo		= *pUserInfo->pcsFamily->GetMember(i)->Get();
			CSHFamilyMember::stINFO_EX	stInfoEx	= *pUserInfo->pcsFamily->GetMember(i)->GetEx();

			stInfo.nLV = nLevel;
			if (pszGuildName)
			{
				SafeStrCpy(stInfoEx.szGuild, pszGuildName, MAX_NAME_LENGTH+1);
			}
			pUserInfo->pcsFamily->GetMember(i)->Set(&stInfo, &stInfoEx);
			break;
		}
	}

	// 클라이언트에 알리기
	ASRV_SendInfoToClient(pUserInfo->pcsFamily, 2);

	// 다른 에이전트에 알리기
	MSG_NAME_DWORD3 stPacket;

	stPacket.Category	= MP_FAMILY;
	stPacket.Protocol	= MP_FAMILY_UPDATE_MAP_PLAYER_INFO_TO_OTHER_AGENT;
	stPacket.dwData1	= pUserInfo->pcsFamily->Get()->nID;
	stPacket.dwData2	= pUserInfo->dwCharacterID;
	stPacket.dwData3	= nLevel;
	if (pszGuildName)
	{
		SafeStrCpy(stPacket.Name, pszGuildName, MAX_NAME_LENGTH+1);
	}

	g_Network.Broadcast2AgentServerExceptSelf((char*)&stPacket, sizeof(stPacket));
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ASRV_SendNicknameToMap Method																				  호칭을 맵서버에 보내기
//
VOID CSHFamilyManager::ASRV_SendNicknameToMap(USERINFO* pUserInfo, char* pszNickname)
{
#if defined(_AGENTSERVER)
	MSG_NAME stPacket;

	stPacket.Category			= MP_FAMILY;
	stPacket.Protocol			= MP_FAMILY_SHOW_NICKNAME;
	stPacket.dwObjectID			= pUserInfo->dwCharacterID;
	SafeStrCpy(stPacket.Name, pszNickname, MAX_NAME_LENGTH+1);

	g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ASRV_RegistEmblem Method																								   문장 등록
//
VOID CSHFamilyManager::ASRV_RegistEmblem(USERINFO* pUserInfo, DWORD nFamilyID, char* pImgData, int nState)
{
#if defined(_AGENTSERVER)
	if (pUserInfo->pcsFamily == NULL)
	{
		return;
	}

	if (pUserInfo->pcsFamily->Get()->nMasterID != pUserInfo->dwCharacterID && nState == 0)
	{
		return;
	}

	PACKET_FAMILY_EMBLEM stPacket;

	// 등록
	if (nState == 0)
	{
		pUserInfo->pcsFamily->IncreaseEmblemChangedNum();
		// 멤버들에게 알리기
		for(UINT i=0; i<pUserInfo->pcsFamily->Get()->nMemberNum; i++)
		{
			USERINFO* pMemberUserInfo = g_pUserTableForObjectID->FindUser(pUserInfo->pcsFamily->GetMember(i)->Get()->nID);
			if (pMemberUserInfo)
			{
				stPacket.Category		= MP_FAMILY;
				stPacket.Protocol		= MP_FAMILY_REGIST_EMBLEM;
				stPacket.dwObjectID		= pMemberUserInfo->dwCharacterID;
				stPacket.nFamilyID		= nFamilyID;
				stPacket.nChangedFreq	= pUserInfo->pcsFamily->GetEmblemChangedNum();
				memcpy(stPacket.cImgData, pImgData, EMBLEM_BUFSIZE);
				g_Network.Send2Server( pMemberUserInfo->dwMapServerConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
			}
		}
		// DB에 저장
		Family_SaveEmblemInfo(pUserInfo->dwCharacterID, nFamilyID, pImgData);
		// 081205 LUJ, 로그
		InsertLogFamily(
			eLog_FamilyRegistEmblem,
			nFamilyID,
			pUserInfo->dwCharacterID );
		// 다른 에이전트에 알리기
		stPacket.Category		= MP_FAMILY;
		stPacket.Protocol		= MP_FAMILY_REGIST_EMBLEM_TO_OTHER_AGENT;
		stPacket.nFamilyID		= nFamilyID;
		memcpy(stPacket.cImgData, pImgData, EMBLEM_BUFSIZE);
		g_Network.Broadcast2AgentServerExceptSelf((char*)&stPacket, sizeof(stPacket));
	}
	// 로드
	else
	{
		stPacket.Category		= MP_FAMILY;
		stPacket.Protocol		= MP_FAMILY_SHOW_EMBLEM;
		stPacket.dwObjectID		= pUserInfo->dwCharacterID;
		stPacket.nFamilyID		= nFamilyID;
		stPacket.nChangedFreq	= pUserInfo->pcsFamily->GetEmblemChangedNum();
		memcpy(stPacket.cImgData, pImgData, EMBLEM_BUFSIZE);
		g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ASRV_RegistEmblemFromOtherAgent Method																 다른 에이전트로부터의 문장 등록
//
VOID CSHFamilyManager::ASRV_RegistEmblemFromOtherAgent(DWORD nFamilyID, char* pImgData)
{
#if defined(_AGENTSERVER)
	// 테이블에 이미 존재하는지 체크
	CSHFamily* pcsFamily = GetFamily(nFamilyID);
	if (pcsFamily == NULL)
	{
		return;
	}

	pcsFamily->IncreaseEmblemChangedNum();

	PACKET_FAMILY_EMBLEM stPacket;

	// 멤버들에게 알리기
	for(UINT i=0; i<pcsFamily->Get()->nMemberNum; i++)
	{
		USERINFO* pMemberUserInfo = g_pUserTableForObjectID->FindUser(pcsFamily->GetMember(i)->Get()->nID);
		if (pMemberUserInfo)
		{
			stPacket.Category		= MP_FAMILY;
			stPacket.Protocol		= MP_FAMILY_REGIST_EMBLEM;
			stPacket.dwObjectID		= pMemberUserInfo->dwCharacterID;
			stPacket.nFamilyID		= nFamilyID;
			stPacket.nChangedFreq	= pcsFamily->GetEmblemChangedNum();
			memcpy(stPacket.cImgData, pImgData, EMBLEM_BUFSIZE);
			g_Network.Send2Server( pMemberUserInfo->dwMapServerConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
		}
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ASRV_Leave Method																												탈퇴
//
VOID CSHFamilyManager::ASRV_Leave(USERINFO* pUserInfo)
{
#if defined(_AGENTSERVER)
	MSG_DWORD stPacket;

	// 패밀리가 있나?
	if (pUserInfo->pcsFamily == NULL)
	{
		return;
	}

	// 마스터는 탈퇴 안 됨
	if (pUserInfo->dwCharacterID == pUserInfo->pcsFamily->Get()->nMasterID)
	{
		return;
	}

	// 멤버들에게 알리기
	int nLoginCnt = 0;
	for(UINT i=0; i<pUserInfo->pcsFamily->Get()->nMemberNum; i++)
	{
		USERINFO* pMemberUserInfo = g_pUserTableForObjectID->FindUser(pUserInfo->pcsFamily->GetMember(i)->Get()->nID);
		if (pMemberUserInfo)
		{
			stPacket.Category	= MP_FAMILY;
			stPacket.Protocol	= MP_FAMILY_LEAVE;
			stPacket.dwObjectID	= pMemberUserInfo->dwCharacterID;
			stPacket.dwData		= pUserInfo->dwCharacterID;
			g_Network.Send2User( pMemberUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
		}

		if (pUserInfo->pcsFamily->GetMember(i)->Get()->eConState == CSHFamilyMember::MEMBER_CONSTATE_LOGIN)
		{
			nLoginCnt++;
		}
	}

	// 현재 접속해 있는 멤버가 HONOR_POINT_CHECK_MEMBER_NUM명 미만이 되면 명예 포인트 시간 초기화
	if (nLoginCnt-1 < CSHFamilyManager::HONOR_POINT_CHECK_MEMBER_NUM)
	{
		CSHFamily::stINFO_EX stInfoEx = *pUserInfo->pcsFamily->GetEx();
		stInfoEx.nHonorPointTimeTick = 0;
		pUserInfo->pcsFamily->SetEx(&stInfoEx);
	}

	// 맵서버에 알리기
	// ..문장 삭제, 해당 캐릭터가 심어놓은 농작물 제거
	stPacket.Category	= MP_FAMILY;
	stPacket.Protocol	= MP_FAMILY_LEAVE;
	stPacket.dwObjectID = pUserInfo->dwCharacterID;
	g_Network.Broadcast2MapServer((char*)&stPacket, sizeof(stPacket));

	// 기한 설정
	SYSTEMTIME t;
	GetLocalTime(&t);
	pUserInfo->nFamilyLeaveDate = (t.wYear-2000)*100000000 + t.wMonth*1000000 + t.wDay*10000 + t.wHour*100 + t.wMinute;
	pUserInfo->nFamilyLeaveKind = FLK_LEAVE;

	// 다른 에이전트에 알리기
	stPacket.Category	= MP_FAMILY;
	stPacket.Protocol	= MP_FAMILY_LEAVE_TO_OTHER_AGENT;
	stPacket.dwObjectID	= pUserInfo->dwCharacterID;
	stPacket.dwData		= pUserInfo->pcsFamily->Get()->nID;
	g_Network.Broadcast2AgentServerExceptSelf((char*)&stPacket, sizeof(stPacket));

	// 멤버 삭제
	pUserInfo->pcsFamily->DelMember(pUserInfo->dwCharacterID);
	// DB에 저장
	Family_Leave_SaveInfo(pUserInfo->dwCharacterID, FLK_LEAVE, FALSE);
	// 081205 LUJ, 로그
	InsertLogFamily(
		eLog_FamilyLeaveMember,
		pUserInfo->pcsFamily->Get()->nID,
		pUserInfo->dwCharacterID );

	// 로그인 수가 탈퇴자 1명이면 패밀리 삭제
	if (nLoginCnt == 1)
	{
		// 081205 LUJ, 로그. 값을 초기화시키므로 먼저 로그를 남기자
		InsertLogFamily(
			eLog_FamilyBreakup,
			pUserInfo->pcsFamily->Get()->nID,
			pUserInfo->dwCharacterID,
			"auto" );
		DelFamilyToTbl(pUserInfo->pcsFamily);
	}
	// 탈퇴한 멤버의 유저정보 설정
	pUserInfo->pcsFamily = NULL;
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ASRV_Expel Method																												추방
//
VOID CSHFamilyManager::ASRV_Expel(USERINFO* pUserInfo, DWORD nMemberID)
{
#if defined(_AGENTSERVER)
	RESULT eResult = RESULT_OK;

	MSG_DWORD2 stPacket;

	// 패밀리가 있나?
	if (pUserInfo->pcsFamily == NULL)
	{
		return;
	}

	// 마스터인가?
	if (pUserInfo->dwCharacterID != pUserInfo->pcsFamily->Get()->nMasterID)
	{
		return;
	}

	// 추방 대상이 마스터인가?
	if (pUserInfo->dwCharacterID == nMemberID)
	{
		eResult = RESULT_FAIL_01;
		goto FAILED;
		return;
	}


	// 멤버들에게 알리기
	int nLoginCnt = 0;
	for(UINT i=0; i<pUserInfo->pcsFamily->Get()->nMemberNum; i++)
	{
		USERINFO* pMemberUserInfo = g_pUserTableForObjectID->FindUser(pUserInfo->pcsFamily->GetMember(i)->Get()->nID);
		if (pMemberUserInfo)
		{
			stPacket.Category	= MP_FAMILY;
			stPacket.Protocol	= MP_FAMILY_EXPEL;
			stPacket.dwObjectID	= pMemberUserInfo->dwCharacterID;
			stPacket.dwData1	= nMemberID;
			stPacket.dwData2	= eResult;
			g_Network.Send2User( pMemberUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
		}

		if (pUserInfo->pcsFamily->GetMember(i)->Get()->eConState == CSHFamilyMember::MEMBER_CONSTATE_LOGIN)
		{
			nLoginCnt++;
		}
	}

	// 현재 접속해 있는 멤버가 HONOR_POINT_CHECK_MEMBER_NUM명 미만이 되면 명예 포인트 시간 초기화
	if (nLoginCnt-1 < CSHFamilyManager::HONOR_POINT_CHECK_MEMBER_NUM)
	{
		CSHFamily::stINFO_EX stInfoEx = *pUserInfo->pcsFamily->GetEx();
		stInfoEx.nHonorPointTimeTick = 0;
		pUserInfo->pcsFamily->SetEx(&stInfoEx);
	}

	// 맵서버에 알리기
	// ..문장 삭제, 해당 캐릭터가 심어놓은 농작물 제거
	stPacket.Category	= MP_FAMILY;
	stPacket.Protocol	= MP_FAMILY_EXPEL;
	stPacket.dwObjectID = nMemberID;
	stPacket.dwData1	= nMemberID;
	stPacket.dwData2	= eResult;
	g_Network.Broadcast2MapServer((char*)&stPacket, sizeof(stPacket));

	// ..마스터의 추방 페널티를 위해..
	stPacket.Category	= MP_FAMILY;
	stPacket.Protocol	= MP_FAMILY_EXPEL;
	stPacket.dwObjectID = pUserInfo->dwCharacterID;
	stPacket.dwData1	= nMemberID;
	stPacket.dwData2	= eResult;
	g_Network.Send2Server(pUserInfo->dwMapServerConnectionIndex, (char*)&stPacket, sizeof(stPacket));

	// 기한 설정
	SYSTEMTIME t;
	GetLocalTime(&t);
	int nDate = (t.wYear-2000)*100000000 + t.wMonth*1000000 + t.wDay*10000 + t.wHour*100 + t.wMinute;
	pUserInfo->nFamilyLeaveDate = nDate;
	pUserInfo->nFamilyLeaveKind = FLK_EXPEL;

	// 다른 에이전트에 알리기
	stPacket.Category	= MP_FAMILY;
	stPacket.Protocol	= MP_FAMILY_EXPEL_TO_OTHER_AGENT;
	stPacket.dwObjectID	= nMemberID;
	stPacket.dwData1	= pUserInfo->pcsFamily->Get()->nID;
	stPacket.dwData2	= eResult;
	g_Network.Broadcast2AgentServerExceptSelf((char*)&stPacket, sizeof(stPacket));

	// 멤버 삭제
	pUserInfo->pcsFamily->DelMember(nMemberID);
	// DB에 저장
	// ..추방된 멤버, 마스터(마스터는 시간 카운트만 저장한다 -프로시저에서 알아서 처리함)
	Family_Leave_SaveInfo(nMemberID, FLK_EXPEL, FALSE);
	Family_Leave_SaveInfo(pUserInfo->dwCharacterID, FLK_EXPEL, FALSE);
	// 081205 LUJ, 로그
	InsertLogFamily(
		eLog_FamilyExpelMember,
		pUserInfo->pcsFamily->Get()->nID,
		pUserInfo->dwCharacterID );

	// 추방된 멤버의 유저정보 설정
	USERINFO* pMemberUserInfo = g_pUserTableForObjectID->FindUser(nMemberID);
	if (pMemberUserInfo)
	{
		pMemberUserInfo->pcsFamily = NULL;
		pMemberUserInfo->nFamilyLeaveDate = nDate;
		pMemberUserInfo->nFamilyLeaveKind = FLK_EXPEL;
	}

FAILED:
	if (eResult == RESULT_OK)
	{
	}
	else
	{
		stPacket.Category	= MP_FAMILY;
		stPacket.Protocol	= MP_FAMILY_EXPEL;
		stPacket.dwObjectID	= pUserInfo->dwCharacterID;
		stPacket.dwData2	= eResult;
		g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ASRV_BreakUp Method																												해체
//
VOID CSHFamilyManager::ASRV_BreakUp(USERINFO* pUserInfo)
{
#if defined(_AGENTSERVER)
	RESULT eResult = RESULT_OK;

	MSG_DWORD2 stPacket;

	// 패밀리가 있나?
	if (pUserInfo->pcsFamily == NULL)
	{
		eResult = RESULT_FAIL_01;
		goto FAILED;
		return;
	}

	// 마스터만 가능
	if (pUserInfo->dwCharacterID != pUserInfo->pcsFamily->Get()->nMasterID)
	{
		eResult = RESULT_FAIL_02;
		goto FAILED;
		return;
	}

	// 멤버가 없어야 함.
	if (pUserInfo->pcsFamily->Get()->nMemberNum > 1)
	{
		eResult = RESULT_FAIL_03;
		goto FAILED;
		return;
	}

	// 멤버들에게 알리기
	int nLoginCnt = 0;
	for(UINT i=0; i<pUserInfo->pcsFamily->Get()->nMemberNum; i++)
	{
		USERINFO* pMemberUserInfo = g_pUserTableForObjectID->FindUser(pUserInfo->pcsFamily->GetMember(i)->Get()->nID);
		if (pMemberUserInfo)
		{
			// 클라이언트에 알리기
			stPacket.Category	= MP_FAMILY;
			stPacket.Protocol	= MP_FAMILY_BREAKUP;
			stPacket.dwObjectID	= pMemberUserInfo->dwCharacterID;
			stPacket.dwData1	= pUserInfo->dwCharacterID;
			stPacket.dwData2	= eResult;
			g_Network.Send2User( pMemberUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
			// 맵서버에 알리기
			// ..문장 삭제, 해당 캐릭터가 심어놓은 농작물 제거
			stPacket.Category	= MP_FAMILY;
			stPacket.Protocol	= MP_FAMILY_BREAKUP;
			stPacket.dwObjectID = pMemberUserInfo->dwCharacterID;
			stPacket.dwData1	= pUserInfo->dwCharacterID;
			stPacket.dwData2	= eResult;
			g_Network.Broadcast2MapServer((char*)&stPacket, sizeof(stPacket));
			// 멤버의 패밀리 초기화
			if (pMemberUserInfo->dwCharacterID != pUserInfo->dwCharacterID) 
			{
				pMemberUserInfo->pcsFamily = NULL;
			}
		}
	}

	// 기한 설정
	SYSTEMTIME t;
	GetLocalTime(&t);
	pUserInfo->nFamilyLeaveDate = (t.wYear-2000)*100000000 + t.wMonth*1000000 + t.wDay*10000 + t.wHour*100 + t.wMinute;
	pUserInfo->nFamilyLeaveKind = FLK_BREAKUP;

	// 다른 에이전트에 알리기
	stPacket.Category	= MP_FAMILY;
	stPacket.Protocol	= MP_FAMILY_BREAKUP_TO_OTHER_AGENT;
	stPacket.dwObjectID	= pUserInfo->dwCharacterID;
	stPacket.dwData1	= pUserInfo->pcsFamily->Get()->nID;
	stPacket.dwData2	= eResult;
	g_Network.Broadcast2AgentServerExceptSelf((char*)&stPacket, sizeof(stPacket));

	// 081205 LUJ, 로그. 삭제 시 메모리를 초기화하므로 먼저 로그를 남기자
	InsertLogFamily(
		eLog_FamilyBreakup,
		pUserInfo->pcsFamily->Get()->nID,
		pUserInfo->dwCharacterID );
	// 패밀리 삭제
	DelFamilyToTbl(pUserInfo->pcsFamily);
	// DB에 저장
	Family_Leave_SaveInfo(pUserInfo->dwCharacterID, FLK_BREAKUP, FALSE, TRUE);
	// 유저 정보 설정
	pUserInfo->pcsFamily = NULL;

FAILED:
	if (eResult == RESULT_OK)
	{
	}
	else
	{
		stPacket.Category	= MP_FAMILY;
		stPacket.Protocol	= MP_FAMILY_BREAKUP;
		stPacket.dwObjectID	= pUserInfo->dwCharacterID;
		stPacket.dwData2	= eResult;
		g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ASRV_LeaveFromOtherAgent Method																		  	  다른 에이전트로부터의 탈퇴
//
VOID CSHFamilyManager::ASRV_LeaveFromOtherAgent(DWORD nFamilyID, DWORD nMemberID)
{
#if defined(_AGENTSERVER)
	// 테이블에 이미 존재하는지 체크
	CSHFamily* pcsFamily = GetFamily(nFamilyID);
	if (pcsFamily == NULL)
	{
		return;
	}

	MSG_DWORD stPacket;

	// 멤버들에게 알리기
	int nLoginCnt = 0;
	for(UINT i=0; i<pcsFamily->Get()->nMemberNum; i++)
	{
		USERINFO* pMemberUserInfo = g_pUserTableForObjectID->FindUser(pcsFamily->GetMember(i)->Get()->nID);
		if (pMemberUserInfo)
		{
			stPacket.Category		= MP_FAMILY;
			stPacket.Protocol		= MP_FAMILY_LEAVE;
			stPacket.dwObjectID		= pMemberUserInfo->dwCharacterID;
			stPacket.dwData			= nMemberID;
			g_Network.Send2User( pMemberUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
		}

		if (pcsFamily->GetMember(i)->Get()->eConState == CSHFamilyMember::MEMBER_CONSTATE_LOGIN)
		{
			nLoginCnt++;
		}
	}

	// 현재 접속해 있는 멤버가 HONOR_POINT_CHECK_MEMBER_NUM명 미만이 되면 명예 포인트 시간 초기화
	if (nLoginCnt-1 < CSHFamilyManager::HONOR_POINT_CHECK_MEMBER_NUM)
	{
		CSHFamily::stINFO_EX stInfoEx = *pcsFamily->GetEx();
		stInfoEx.nHonorPointTimeTick = 0;
		pcsFamily->SetEx(&stInfoEx);
	}

	// 멤버 삭제
	pcsFamily->DelMember(nMemberID);

	// 로그인 수가 탈퇴자 1명이면 패밀리 삭제
	if (nLoginCnt == 1)
	{
		DelFamilyToTbl(pcsFamily);
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ASRV_ExpelFromOtherAgent Method																		  	  다른 에이전트로부터의 추방
//
VOID CSHFamilyManager::ASRV_ExpelFromOtherAgent(DWORD nFamilyID, DWORD nMemberID)
{
#if defined(_AGENTSERVER)
	// 테이블에 이미 존재하는지 체크
	CSHFamily* pcsFamily = GetFamily(nFamilyID);
	if (pcsFamily == NULL)
	{
		return;
	}

	MSG_DWORD2 stPacket;

	// 멤버들에게 알리기
	int nLoginCnt = 0;
	for(UINT i=0; i<pcsFamily->Get()->nMemberNum; i++)
	{
		USERINFO* pMemberUserInfo = g_pUserTableForObjectID->FindUser(pcsFamily->GetMember(i)->Get()->nID);
		if (pMemberUserInfo)
		{
			stPacket.Category		= MP_FAMILY;
			stPacket.Protocol		= MP_FAMILY_EXPEL;
			stPacket.dwObjectID		= pMemberUserInfo->dwCharacterID;
			stPacket.dwData1		= nMemberID;
			stPacket.dwData2		= RESULT_OK;
			g_Network.Send2User( pMemberUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
		}

		if (pcsFamily->GetMember(i)->Get()->eConState == CSHFamilyMember::MEMBER_CONSTATE_LOGIN)
		{
			nLoginCnt++;
		}
	}

	// 현재 접속해 있는 멤버가 HONOR_POINT_CHECK_MEMBER_NUM명 미만이 되면 명예 포인트 시간 초기화
	if (nLoginCnt-1 < CSHFamilyManager::HONOR_POINT_CHECK_MEMBER_NUM)
	{
		CSHFamily::stINFO_EX stInfoEx = *pcsFamily->GetEx();
		stInfoEx.nHonorPointTimeTick = 0;
		pcsFamily->SetEx(&stInfoEx);
	}

	// 멤버 삭제
	pcsFamily->DelMember(nMemberID);

	// ..추방된 멤버의 유저 정보 설정
	USERINFO* pMemberUserInfo = g_pUserTableForObjectID->FindUser(nMemberID);
	if (pMemberUserInfo)
	{
		pMemberUserInfo->pcsFamily = NULL;

		SYSTEMTIME t;
		GetLocalTime(&t);
		int nDate = (t.wYear-2000)*100000000 + t.wMonth*1000000 + t.wDay*10000 + t.wHour*100 + t.wMinute;
		pMemberUserInfo->nFamilyLeaveDate = nDate;
		pMemberUserInfo->nFamilyLeaveKind = FLK_EXPEL;
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ASRV_BreakUpFromOtherAgent Method																	  	  다른 에이전트로부터의 해체
//
VOID CSHFamilyManager::ASRV_BreakUpFromOtherAgent(DWORD nFamilyID)
{
#if defined(_AGENTSERVER)
	// 테이블에 이미 존재하는지 체크
	CSHFamily* pcsFamily = GetFamily(nFamilyID);
	if (pcsFamily == NULL)
	{
		return;
	}

	// 패밀리 삭제
	DelFamilyToTbl(pcsFamily);
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ASRV_SendInfoToClient Method																		 클라이언트에 패밀리 정보 보내기
//
VOID CSHFamilyManager::ASRV_SendInfoToClient(CSHFamily* pcsFamily, int nType)
{
#if defined(_AGENTSERVER)
	PACKET_FAMILY stPacket;
	PACKET_MEMBER_INFO stMemberPacket;

	if (pcsFamily)
	{
		for(UINT i=0; i<pcsFamily->Get()->nMemberNum; i++)
		{
			USERINFO* pMemberUserInfo = g_pUserTableForObjectID->FindUser(pcsFamily->GetMember(i)->Get()->nID);
			if (pMemberUserInfo)
			{
				// 클라이언트에 알리기
				// ..패밀리 데이터
				if (nType == 0 || nType == 1)
				{
					stPacket.Category			= MP_FAMILY;
					stPacket.Protocol			= MP_FAMILY_DATA_FAMILY;
					stPacket.dwObjectID			= pMemberUserInfo->dwCharacterID;
					stPacket.stInfo				= *pcsFamily->Get();
					stPacket.stInfoEx			= *pcsFamily->GetEx();
					g_Network.Send2User(pMemberUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
				}

				if (nType == 0 || nType == 2)
				{
					// ..멤버 데이터
					for(UINT j=0; j<pcsFamily->Get()->nMemberNum; j++)
					{
						stMemberPacket.Category		= MP_FAMILY;
						stMemberPacket.Protocol		= MP_FAMILY_DATA_MEMBER;
						stMemberPacket.dwObjectID	= pMemberUserInfo->dwCharacterID;
						stMemberPacket.nIndex		= j;
						stMemberPacket.stInfo		= *pcsFamily->GetMember(j)->Get();
						stMemberPacket.stInfoEx		= *pcsFamily->GetMember(j)->GetEx();
						g_Network.Send2User( pMemberUserInfo->dwConnectionIndex, (char*)&stMemberPacket, sizeof(stMemberPacket) );
					}
				}
			}
		}
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ASRV_SendMemberConToOtherAgent Method																다른 에이전트에 멤버 접속 알리기
//
VOID CSHFamilyManager::ASRV_SendMemberConToOtherAgent(CSHFamily* pcsFamily, DWORD nMemberID, int nConState, int nType)
{
#if defined(_AGENTSERVER)
	if (pcsFamily == NULL)
	{
		return;
	}

	MSG_DWORD4 stPacket;

	stPacket.Category	= MP_FAMILY;
	stPacket.Protocol	= MP_FAMILY_MEMBER_CONNECTION_TO_OTHER_AGENT;
	stPacket.dwData1	= pcsFamily->Get()->nID;
	stPacket.dwData2	= nMemberID;
	stPacket.dwData3	= nConState;
	stPacket.dwData4	= nType;
	g_Network.Broadcast2AgentServerExceptSelf((char*)&stPacket, sizeof(stPacket));
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ASRV_MemberConFromOtherAgent Method																다른 에이전트로부터의 멤버 접속 처리
//
VOID CSHFamilyManager::ASRV_MemberConFromOtherAgent(DWORD nFamilyID, DWORD nMemberID, int nConState, int nType)
{
#if defined(_AGENTSERVER)
	// 테이블에 이미 존재하는지 체크
	CSHFamily* pcsFamily = GetFamily(nFamilyID);
	if (pcsFamily == NULL)
	{
		return;
	}

	// 해당 멤버의 접속 상태 설정
	int nLoginCnt = 0;
	for(UINT i=0; i<pcsFamily->Get()->nMemberNum; i++)
	{
		if (pcsFamily->GetMember(i)->Get()->nID == nMemberID)
		{
			CSHFamilyMember::stINFO stInfo = *pcsFamily->GetMember(i)->Get();
			stInfo.eConState = (CSHFamilyMember::MEMBER_CONSTATE)nConState;
			pcsFamily->GetMember(i)->Set(&stInfo);
		}

		if (pcsFamily->GetMember(i)->Get()->eConState == CSHFamilyMember::MEMBER_CONSTATE_LOGIN)
		{
			nLoginCnt++;
		}
	}

	// 현재 접속해 있는 멤버가 HONOR_POINT_CHECK_MEMBER_NUM명이 되면 명예 포인트 시간을 설정한다.
	if (nLoginCnt == CSHFamilyManager::HONOR_POINT_CHECK_MEMBER_NUM && nConState == CSHFamilyMember::MEMBER_CONSTATE_LOGIN)
	{
		CSHFamily::stINFO_EX stInfoEx = *pcsFamily->GetEx();
		stInfoEx.nHonorPointTimeTick = gCurTime;
		pcsFamily->SetEx(&stInfoEx);
	}

	// 현재 접속해 있는 멤버가 HONOR_POINT_CHECK_MEMBER_NUM명 미만이 되면 명예 포인트 시간 초기화
	if (nLoginCnt < CSHFamilyManager::HONOR_POINT_CHECK_MEMBER_NUM)
	{
		CSHFamily::stINFO_EX stInfoEx = *pcsFamily->GetEx();
		stInfoEx.nHonorPointTimeTick = 0;
		pcsFamily->SetEx(&stInfoEx);
	}

	// 다른 에이전트에 플레이어가 접속해서 멤버 접속 정보를 받았을 경우에만..
	// 아래에서 보낸 에이전트에게 여기 에이전트의 정보를 보내기 때문에.. 체크 안하면 무한정 주고 받고 한다.
	if (nType == 0)
	{
		// ..모든 멤버가 로그아웃 했으면 테이블에서 패밀리 제거
		if (nConState == CSHFamilyMember::MEMBER_CONSTATE_LOGOFF)
		{
			if (nLoginCnt == 0)
			{
				int nUnitIndex = pcsFamily->GetIndexAtTbl()/MAX_FAMILY_1U_NUM;
				int n1UIndex = pcsFamily->GetIndexAtTbl()%MAX_FAMILY_1U_NUM;

				CSHFamily csFamily;
				m_pcsFamily[nUnitIndex][n1UIndex].Set(&csFamily);
				m_pcsFamily[nUnitIndex][n1UIndex].SetIndexAtTbl(NULL);
			}
		}

		// 정보를 보낸 에이전트에게 여기 에이전트가 관리하는 멤버의 정보를 보낸다.
		if (nConState == CSHFamilyMember::MEMBER_CONSTATE_LOGIN)
		{
			for(UINT i=0; i<pcsFamily->Get()->nMemberNum; i++)
			{
				USERINFO* pMemberUserInfo = g_pUserTableForObjectID->FindUser(pcsFamily->GetMember(i)->Get()->nID);
				if (pMemberUserInfo && pcsFamily->GetMember(i)->Get()->eConState == CSHFamilyMember::MEMBER_CONSTATE_LOGIN)
				{
					ASRV_SendMemberConToOtherAgent(pcsFamily, pcsFamily->GetMember(i)->Get()->nID, pcsFamily->GetMember(i)->Get()->eConState, 1);
				}
			}
		}
	}

	// 클라이언트에 알리기
	ASRV_SendInfoToClient(pcsFamily, 2);
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ASRV_ChangeNicknameFromOtherAgent Method														다른 에이전트로부터의 호칭 변경 처리
//
VOID CSHFamilyManager::ASRV_ChangeNicknameFromOtherAgent(DWORD nFamilyID, DWORD nBehavePlayerID, DWORD nTargetPlayerID, char* pszNickname)
{
#if defined(_AGENTSERVER)
	// 테이블에 이미 존재하는지 체크
	CSHFamily* pcsFamily = GetFamily(nFamilyID);
	if (pcsFamily == NULL)
	{
		return;
	}

	// 변경된 호칭을 이 에이전트의 테이블에도 적용
	for(UINT i=0; i<pcsFamily->Get()->nMemberNum; i++)
	{
		if (nTargetPlayerID == pcsFamily->GetMember(i)->Get()->nID)
		{
			CSHFamilyMember::stINFO	stInfo = *pcsFamily->GetMember(i)->Get();
			
			SafeStrCpy(stInfo.szNickname, pszNickname, MAX_NAME_LENGTH+1);
			pcsFamily->GetMember(i)->Set(&stInfo);
			break;
		}
	}

	// 클라이언트에 알리기
	ASRV_SendInfoToClient(pcsFamily, 2);

	USERINFO* pTargetUserInfo = g_pUserTableForObjectID->FindUser(nTargetPlayerID);
	if (pTargetUserInfo)
	{
		// 맵서버에 알리기
		ASRV_SendNicknameToMap(pTargetUserInfo, pszNickname);

		// 클라이언트에 변경 알리기
		MSG_DWORD_NAME stPacket;
		ZeroMemory(&stPacket, sizeof(stPacket));

		stPacket.Category			= MP_FAMILY;
		stPacket.Protocol			= MP_FAMILY_CHANGE_NICKNAME;
		stPacket.dwObjectID			= pTargetUserInfo->dwCharacterID;
		stPacket.dwData				= RESULT_OK_02;
		for(UINT i=0; i<pcsFamily->Get()->nMemberNum; i++)
		{
			if (nBehavePlayerID == pcsFamily->GetMember(i)->Get()->nID)
			{
				SafeStrCpy(stPacket.Name, pcsFamily->GetMember(i)->Get()->szName, MAX_NAME_LENGTH+1);
				break;
			}
		}
		g_Network.Send2User( pTargetUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ASRV_UpdateMapPlayerInfoFromOtherAgent Method									   다른 에이전트로부터의 맵의 플레이어 정보 업데이트
//
VOID CSHFamilyManager::ASRV_UpdateMapPlayerInfoFromOtherAgent(DWORD nFamilyID, DWORD nPlayerID, DWORD nLevel, char* pszGuildName)
{
#if defined(_AGENTSERVER)
	// 테이블에 이미 존재하는지 체크
	CSHFamily* pcsFamily = GetFamily(nFamilyID);
	if (pcsFamily == NULL)
	{
		return;
	}

	// 업데이트 된 정보 불러오기
	for(UINT i=0; i<pcsFamily->Get()->nMemberNum; i++)
	{
		if (nPlayerID == pcsFamily->GetMember(i)->Get()->nID)
		{
			CSHFamilyMember::stINFO		stInfo		= *pcsFamily->GetMember(i)->Get();
			CSHFamilyMember::stINFO_EX	stInfoEx	= *pcsFamily->GetMember(i)->GetEx();

			stInfo.nLV = nLevel;
			if (pszGuildName)
			{
				SafeStrCpy(stInfoEx.szGuild, pszGuildName, MAX_NAME_LENGTH+1);
			}
			pcsFamily->GetMember(i)->Set(&stInfo, &stInfoEx);
			break;
		}
	}

	// 클라이언트에 알리기
	ASRV_SendInfoToClient(pcsFamily, 2);
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ASRV_UpdateHonorPointFromOtherAgent Method													  다른 에이전트로부터의 명예 포인트 처리
//
VOID CSHFamilyManager::ASRV_UpdateHonorPointFromOtherAgent(DWORD nFamilyID, DWORD nHonorPoint, int nType)
{
#if defined(_AGENTSERVER)
	// 테이블에 이미 존재하는지 체크
	CSHFamily* pcsFamily = GetFamily(nFamilyID);
	if (pcsFamily == NULL)
	{
		return;
	}

	// 농장 구입이나 업그레이드의 경우는 절감 포인트값을 - int 로 넘긴다. 그래서 int 로 캐스팅해서 - 값 인지 체크해서
	// 그럴 경우에는 -= 하도록 한다. 절감 포인트값이 int 범위라는 가정...
	CSHFamily::stINFO_EX stInfoEx = *pcsFamily->GetEx();
	if ((int)nHonorPoint < 0)
	{
		if (stInfoEx.nHonorPoint >= (DWORD)(-1*(int)nHonorPoint))
		{
			stInfoEx.nHonorPoint -= (DWORD)(-1*(int)nHonorPoint);
		}
		else
		{
			return;
		}
	}
	else
	{
		stInfoEx.nHonorPoint += nHonorPoint;
		if (stInfoEx.nHonorPoint > 4200000000) stInfoEx.nHonorPoint = 4200000000;
	}

	// 명예 포인트 지급 시간 초기화
	if (nType == 1)
	{
		stInfoEx.nHonorPointTimeTick = gCurTime;
	}
	pcsFamily->SetEx(&stInfoEx);

	MSG_DWORD stPacket;
	// 클라이언트에 알리기
	for(UINT i=0; i<pcsFamily->Get()->nMemberNum; i++)
	{
		USERINFO* pMemberUserInfo = g_pUserTableForObjectID->FindUser(pcsFamily->GetMember(i)->Get()->nID);
		if (pMemberUserInfo)
		{
			stPacket.Category			= MP_FAMILY;
			stPacket.Protocol			= MP_FAMILY_HONOR_POINT;
			stPacket.dwObjectID			= pMemberUserInfo->dwCharacterID;
			stPacket.dwData				= nHonorPoint;

			g_Network.Send2User( pMemberUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
		}
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ASRV_InviteFromOtherAgent Method																 다른 에이전트로부터의 패밀리로 초대
//
VOID CSHFamilyManager::ASRV_InviteFromOtherAgent(USERINFO* pUserInfo, DWORD nMasterID, char* pszFamilyName)
{
#if defined(_AGENTSERVER)
	RESULT eResult = RESULT_OK;
	int nPastTime = 0;

	// 다른 패밀리의 멤버가 아닌가?
	if (pUserInfo->pcsFamily)
	{
		eResult = RESULT_FAIL_04;
		goto FAILED;
	}

	// 탈퇴 페널티 기한이 지났나?
	// ..DB에 int 로 0710111524(년월일시간분)와 같이 저장됨. 설마 int 로 인한 날짜문제가 생길만큼 오래 서비스 되진...ㅋ
	if (pUserInfo->nFamilyLeaveDate)
	{
		nPastTime = GetPastTime(pUserInfo->nFamilyLeaveDate);
		// 최종 시간 체크
		if (nPastTime < LEAVE_PENALTY_REJOIN_TIME[pUserInfo->nFamilyLeaveKind])
		{
			eResult = RESULT_FAIL_06;
			goto FAILED;
		}
		// 기한 초기화
		Family_Leave_SaveInfo(pUserInfo->dwCharacterID, pUserInfo->nFamilyLeaveKind, TRUE);
	}

FAILED:
	if (eResult == RESULT_OK)
	{
		MSG_DWORD_NAME stPacket;
		stPacket.Category			= MP_FAMILY;
		stPacket.Protocol			= MP_FAMILY_INVITED;
		stPacket.dwObjectID			= pUserInfo->dwCharacterID;
		stPacket.dwData				= nMasterID;
		SafeStrCpy(stPacket.Name, pszFamilyName, MAX_NAME_LENGTH+1);
		g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
	}

	MSG_DWORD2 stPacket;
	stPacket.Category			= MP_FAMILY;
	stPacket.Protocol			= MP_FAMILY_INVITE;
	stPacket.dwObjectID			= nMasterID;
	stPacket.dwData1			= eResult;
	stPacket.dwData2			= nPastTime;
	g_Network.Broadcast2AgentServerExceptSelf((char*)&stPacket, sizeof(stPacket));
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ASRV_InviteResultFromOtherAgent Method														다른 에이전트로부터의 패밀리로 초대 결과
//
VOID CSHFamilyManager::ASRV_InviteResultFromOtherAgent(PACKET_ADD_MEMBER* pstInfo)
{
#if defined(_AGENTSERVER)
	USERINFO* pMasterUserInfo = g_pUserTableForObjectID->FindUser(pstInfo->nMasterID);
	
	// 마스터가 유효한가?
	if (pMasterUserInfo == NULL)
	{
		return;
	}

	// 마스터가 맞나?
	if (pMasterUserInfo->pcsFamily == NULL || pstInfo->nMasterID != pMasterUserInfo->pcsFamily->Get()->nMasterID)
	{
		return;
	}

	// 결과 보내기
	MSG_NAME_DWORD2 stPacket;
	// ..마스터에게 결과 보내기
	stPacket.Category			= MP_FAMILY;
	stPacket.Protocol			= MP_FAMILY_INVITE_RESULT;
	stPacket.dwObjectID			= pMasterUserInfo->dwCharacterID;
	stPacket.dwData1			= pstInfo->dwObjectID;
	if (pstInfo->nResult == CSHFamilyManager::FIR_ACCEPT_PLAYER)	stPacket.dwData2 = FIR_ACCEPT_MASTER;
	else															stPacket.dwData2 = FIR_DENY_MASTER;
	g_Network.Send2User( pMasterUserInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );

	// ..타겟 플레이어에게 결과 보내기
	// ....다른 에이전트를 통해야 한다..
	stPacket.Category			= MP_FAMILY;
	stPacket.Protocol			= MP_FAMILY_INVITE_RESULT_TO_OTHER_AGENT_2;
	stPacket.dwObjectID			= pstInfo->dwObjectID;
	stPacket.dwData1			= pMasterUserInfo->dwCharacterID;
	stPacket.dwData2			= pstInfo->nResult;
	SafeStrCpy(stPacket.Name, pMasterUserInfo->pcsFamily->Get()->szName, MAX_NAME_LENGTH+1);
	g_Network.Broadcast2AgentServerExceptSelf((char*)&stPacket, sizeof(stPacket));

	// 승락
	if (pstInfo->nResult == FIR_ACCEPT_PLAYER)
	{
		// 멤버에 추가
		// ..혹시 모르니까 멤버 수 체크
		if (pMasterUserInfo->pcsFamily->Get()->nMemberNum == pMasterUserInfo->pcsFamily->GetMemberNumMax())
		{
			return;
		}

		CSHFamilyMember csMember;
		CSHFamilyMember::stINFO stInfo;

		ZeroMemory(&stInfo, sizeof(stInfo));
		stInfo.nID			= pstInfo->dwObjectID;
		stInfo.nRace		= pstInfo->nRace;
		stInfo.nSex			= pstInfo->nSex;
		stInfo.nJobFirst	= pstInfo->nJobFirst;
		stInfo.nJobCur		= pstInfo->nJobCur;
		stInfo.nJobGrade	= pstInfo->nJobGrade;
		stInfo.nLV			= pstInfo->nLV;
		stInfo.eConState	= CSHFamilyMember::MEMBER_CONSTATE_LOGIN;
		SafeStrCpy(stInfo.szName, pstInfo->szName, MAX_NAME_LENGTH+1);

		csMember.Set(&stInfo);
		pMasterUserInfo->pcsFamily->AddMember(&csMember);

		// DB에 저장
		Family_Member_SaveInfo(pMasterUserInfo->pcsFamily->Get()->nID, pstInfo->dwObjectID, NULL);

		// 다른 에이전트에 알리기
		PACKET_FAMILY_TOTAL stPacket;
		int nLoginCnt = 0;

		ZeroMemory(&stPacket, sizeof(stPacket));
		stPacket.Category			= MP_FAMILY;
		stPacket.Protocol			= MP_FAMILY_INVITE_RESULT_TO_OTHER_AGENT_3;
		stPacket.dwObjectID			= pstInfo->dwObjectID;
		stPacket.stFamily.stInfo	= *pMasterUserInfo->pcsFamily->Get();
		stPacket.stFamily.stInfoEx	= *pMasterUserInfo->pcsFamily->GetEx();
		for(UINT i=0; i<pMasterUserInfo->pcsFamily->Get()->nMemberNum; i++)
		{
			stPacket.stMember[i].stInfo		= *pMasterUserInfo->pcsFamily->GetMember(i)->Get();
			stPacket.stMember[i].stInfoEx	= *pMasterUserInfo->pcsFamily->GetMember(i)->GetEx();
			stPacket.stMember[i].nIndex		= i;
			if (stPacket.stMember[i].stInfo.eConState == CSHFamilyMember::MEMBER_CONSTATE_LOGIN)
			{
				nLoginCnt++;
			}
		}

		// 현재 접속해 있는 멤버가 HONOR_POINT_CHECK_MEMBER_NUM명이 되면 명예 포인트 시간을 설정한다.
		if (nLoginCnt == CSHFamilyManager::HONOR_POINT_CHECK_MEMBER_NUM)
		{
			CSHFamily::stINFO_EX stInfoEx = *pMasterUserInfo->pcsFamily->GetEx();
			stInfoEx.nHonorPointTimeTick = gCurTime;
			pMasterUserInfo->pcsFamily->SetEx(&stInfoEx);
		}

		g_Network.Broadcast2AgentServerExceptSelf((char*)&stPacket, sizeof(stPacket));

		// 클라이언트에 패밀리 정보 보내기
		ASRV_SendInfoToClient(pMasterUserInfo->pcsFamily);
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  ASRV_InviteResultFromOtherAgent2 Method														다른 에이전트로부터의 패밀리로 초대 결과
//
VOID CSHFamilyManager::ASRV_InviteResultFromOtherAgent2(PACKET_FAMILY_TOTAL* pstInfo)
{
#if defined(_AGENTSERVER)
	CSHFamily* pcsFamily = GetFamily(pstInfo->stFamily.stInfo.nID);
	// 이미 테이블에 패밀리가 있으면 멤버 추가만 한다.
	if (pcsFamily)
	{
		// 추가된 유저의 패밀리 포인터 설정
		USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pstInfo->dwObjectID);
		// 유저가 접속 중이면 패밀리 포인터 설정. 초대 수락하고 곧바로 접속을 끊으면 유저가 없을 수도 있다. 
		// 유저가 없더라도, 테이블이 패밀리 데이터 갱신
		if (pUserInfo)
		{
			pUserInfo->pcsFamily = pcsFamily;
		}

		// 혹시 모르니까 멤버 수 체크
		if (pcsFamily->Get()->nMemberNum >= pcsFamily->GetMemberNumMax()) return;

		// 제일 마지막에 추가가 됐으니까..
		int nAddIndex = pstInfo->stFamily.stInfo.nMemberNum-1;

		// 멤버 추가
		CSHFamilyMember csFamilyMember;
		CSHFamilyMember::stINFO		stInfo		= pstInfo->stMember[nAddIndex].stInfo;
		CSHFamilyMember::stINFO_EX	stInfoEx	= pstInfo->stMember[nAddIndex].stInfoEx;

		csFamilyMember.Set(&stInfo, &stInfoEx);
		pcsFamily->AddMember(&csFamilyMember);

		int nLoginCnt = 0;
		for(UINT i=0; i<pcsFamily->Get()->nMemberNum; i++)
		{
			if (pcsFamily->GetMember(i)->Get()->eConState == CSHFamilyMember::MEMBER_CONSTATE_LOGIN)
			{
				nLoginCnt++;
			}
		}

		// 현재 접속해 있는 멤버가 HONOR_POINT_CHECK_MEMBER_NUM명이 되면 명예 포인트 시간을 설정한다.
		if (nLoginCnt == CSHFamilyManager::HONOR_POINT_CHECK_MEMBER_NUM)
		{
			CSHFamily::stINFO_EX stInfoEx = *pcsFamily->GetEx();
			stInfoEx.nHonorPointTimeTick = gCurTime;
			pcsFamily->SetEx(&stInfoEx);
		}

		// 클라이언트에 패밀리 정보 보내기
		ASRV_SendInfoToClient(pcsFamily);

		// 멤버가 된 플레이어에게 문장 전송
		if (pUserInfo)
		{
			Family_LoadEmblemInfo(pUserInfo->dwCharacterID, pUserInfo->pcsFamily->Get()->nID);
			// 맵서버에 농장UI 정보 요청
			g_csFarmManager.ASRV_RequestFarmUIInfoToMap(pUserInfo->dwCharacterID, pUserInfo->pcsFamily);
		}
	}
	// 테이블에 없고 추가된 플레이어를 관리하는 에이전트라면 패밀리를 테이블에 추가한다.
	else
	{
		USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pstInfo->dwObjectID);
		if (pUserInfo == NULL)
		{
			return;
		}

		// 패밀리 정보 설정
		CSHFamily::stINFO			stInfo				= pstInfo->stFamily.stInfo;
		CSHFamily::stINFO_EX		stInfoEx			= pstInfo->stFamily.stInfoEx;
		CSHFamily csNewFamily;
		stInfoEx.nHonorPointTimeTick = 0;
		csNewFamily.Set(&stInfo, &stInfoEx);

		// 멤버 정보 설정
		CSHFamilyMember::stINFO		stMemberInfo;
		CSHFamilyMember::stINFO_EX	stMemberInfoEx;
		CSHFamilyMember csMember;
		int nLoginCnt = 0;
		for(UINT i=0; i<pstInfo->stFamily.stInfo.nMemberNum; i++)
		{
			stMemberInfo	= pstInfo->stMember[i].stInfo;
			stMemberInfoEx	= pstInfo->stMember[i].stInfoEx;
			csMember.Set(&stMemberInfo, &stMemberInfoEx);
			csNewFamily.SetMember(&csMember, i);
			if (pstInfo->stMember[i].stInfo.eConState == CSHFamilyMember::MEMBER_CONSTATE_LOGIN)
			{
				nLoginCnt++;
			}
		}

		pUserInfo->pcsFamily = AddFamilyToTbl(&csNewFamily);

		// 현재 접속해 있는 멤버가 HONOR_POINT_CHECK_MEMBER_NUM명이 되면 명예 포인트 시간을 설정한다.
		// ..테이블에 추가가 되는 상황이므로 == 이 아니라 >= 여야 함. 기존에 연결되어 있던 멤버가 HONOR_POINT_CHECK_MEMBER_NUM 이상일 수도 있으므로..
		// ..더 자세하게 설명하자면, 에이전트1 에 이미 2명이 접속하고 있던 패밀리가 있었다. 에이전트2에는 그 패밀리 멤버가 접속해 있지 않아서
		// ..에이전트2 의 테이블에는 그 패밀리가 없었다. 이 상황에서 에이전트2 의 어떤 플레이어가 그 패밀리의 멤버가 될 때 이 루틴이 수행된다.
		// ..그러면 3명이 로그인 된 상태인데, == 로 처리하면, 아래의 코드가 수행이 되지 않는 일이 발생한다. 복잡...
		// ..이 루틴에 의해 패밀리가 테이블에 추가된 후에는 저어~기 위쪽 루틴에서 == 로 체크한다.
		if (nLoginCnt >= CSHFamilyManager::HONOR_POINT_CHECK_MEMBER_NUM)
		{
			CSHFamily::stINFO_EX stInfoEx = *pUserInfo->pcsFamily->GetEx();
			stInfoEx.nHonorPointTimeTick = gCurTime;
			pUserInfo->pcsFamily->SetEx(&stInfoEx);
		}

		// 클라이언트에 패밀리 정보 보내기
		ASRV_SendInfoToClient(pUserInfo->pcsFamily);
		// 멤버가 된 플레이어에게 문장 전송
		Family_LoadEmblemInfo(pUserInfo->dwCharacterID, pUserInfo->pcsFamily->Get()->nID);
		// 맵서버에 농장UI 정보 요청
		g_csFarmManager.ASRV_RequestFarmUIInfoToMap(pUserInfo->dwCharacterID, pUserInfo->pcsFamily);
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_ParseRequestFromClient Method																		   클라이언트 요청 분석/처리
//
VOID CSHFamilyManager::SRV_ParseRequestFromClient(DWORD dwConnectionID, char* pMsg, DWORD dwLength)
{
#if defined(_MAPSERVER_)
	MSGBASE* pPacket = (MSGBASE*)pMsg;

	// 잘못된 플레이어 처리
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pPacket->dwObjectID );
	if( !pPlayer )	
	{
		switch(pPacket->Protocol)
		{
		// 탈퇴 (농장 관련)
		case MP_FAMILY_LEAVE:
			{
				MSG_DWORD* pstPacket = (MSG_DWORD*)pMsg;

				SRV_Leave(NULL, pPacket->dwObjectID, 1);
			}
			break;
		// 추방 (농장 관련)
		case MP_FAMILY_EXPEL:
			{
				MSG_DWORD* pstPacket = (MSG_DWORD*)pMsg;

				SRV_Expel(NULL, pPacket->dwObjectID, 1);
			}
			break;
		// 해체 (농장 관련)
		case MP_FAMILY_BREAKUP:
			{
				MSG_DWORD* pstPacket = (MSG_DWORD*)pMsg;

				SRV_BreakUp(NULL, pPacket->dwObjectID, 1);
			}
			break;
		}
		return;
	}

	switch(pPacket->Protocol)
	{
	// 생성
	case MP_FAMILY_CREATE:
		{
			MSG_NAME* pPacket = (MSG_NAME*)pMsg;
			SRV_CreateFamily(pPlayer, pPacket->Name);
		}
		break;
	// 초대 결과
	case MP_FAMILY_INVITE_RESULT:
		{
			MSG_DWORD2* pPacket = (MSG_DWORD2*)pMsg;
			SRV_InviteResult(pPlayer, pPacket->dwData1, pPacket->dwData2);
		}
		break;
	// 호칭
	case MP_FAMILY_SHOW_NICKNAME:
		{
			MSG_NAME* pPacket = (MSG_NAME*)pMsg;
			SRV_SendNicknameToClient(pPlayer, pPacket->Name);
		}
		break;
	// 문장 등록
	case MP_FAMILY_REGIST_EMBLEM:
		{
			PACKET_FAMILY_EMBLEM* pstPacket = (PACKET_FAMILY_EMBLEM*)pMsg;

			SRV_RegistEmblem(pPlayer, pstPacket->nFamilyID, pstPacket->cImgData, pstPacket->nChangedFreq);
		}
		break;
	// 문장 표시
	case MP_FAMILY_SHOW_EMBLEM:
		{
			PACKET_FAMILY_EMBLEM* pstPacket = (PACKET_FAMILY_EMBLEM*)pMsg;

			SRV_ShowEmblem(pPlayer, pstPacket->nFamilyID, pstPacket->cImgData, pstPacket->nChangedFreq);
		}
		break;
	// 문장 표시2
	case MP_FAMILY_SHOW_EMBLEM_2:
		{
			MSG_DWORD* pstPacket = (MSG_DWORD*)pMsg;

			SRV_ShowEmblem2(pPlayer, pstPacket->dwData);
		}
		break;
	// 탈퇴
	case MP_FAMILY_LEAVE:
		{
			MSG_DWORD* pstPacket = (MSG_DWORD*)pMsg;

			SRV_Leave(pPlayer, pPlayer->GetID());
		}
		break;
	// 추방
	case MP_FAMILY_EXPEL:
		{
			MSG_DWORD* pstPacket = (MSG_DWORD*)pMsg;

			SRV_Expel(pPlayer, pstPacket->dwData);
		}
		break;
	// 해채
	case MP_FAMILY_BREAKUP:
		{
			MSG_DWORD* pstPacket = (MSG_DWORD*)pMsg;

			SRV_BreakUp(pPlayer, pstPacket->dwData);
		}
		break;
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_CreateFamily Method																									 패밀리 생성
//
VOID CSHFamilyManager::SRV_CreateFamily(CPlayer* pPlayer, char* szFamilyName)
{
#if defined(_MAPSERVER_)
	RESULT eResult = RESULT_OK;

	// 패밀리 생성 조건 체크
	// ..필요 돈 체크
/*
	if (pPlayer->GetMoney >= 5000)
	{
		pPlayer->GetMoney -= 5000;
		eResult = RESULT_FAIL_01;
		goto FAILED;
	}
*/
//FAILED:
	MSG_DWORD_NAME stPacket;

	// 클라이언트에 알리기
	// ..클라이언트에 바로 가지 않고 Agent에서 처리할 것을 처리하고 간다
	stPacket.Category			= MP_FAMILY;
	stPacket.Protocol			= MP_FAMILY_CREATE;
	stPacket.dwObjectID			= pPlayer->GetID();
	stPacket.dwData				= RESULT_OK;
	SafeStrCpy(stPacket.Name, szFamilyName, MAX_NAME_LENGTH+1);
	pPlayer->SendMsg(&stPacket, sizeof(stPacket));
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_InviteResult Method																							  패밀리로 초대 결과
//
VOID CSHFamilyManager::SRV_InviteResult(CPlayer* pPlayer, DWORD nMasterID, DWORD nResult)
{
#if defined(_MAPSERVER_)
	RESULT eResult = RESULT_OK;

	// 잘못된 플레이어 처리
	CPlayer* pMaster = (CPlayer*)g_pUserTable->FindUser( nMasterID );
	if( !pMaster ) return;

	PACKET_ADD_MEMBER stPacket;
	// 클라이언트에 알리기
	// ..클라이언트에 바로 가지 않고 Agent에서 처리할 것을 처리하고 간다
	stPacket.Category			= MP_FAMILY;
	stPacket.Protocol			= MP_FAMILY_INVITE_RESULT;
	stPacket.dwObjectID			= pPlayer->GetID();
	stPacket.nMasterID			= nMasterID;
	stPacket.nRace				= pPlayer->GetRacial();
	stPacket.nSex				= pPlayer->GetGender();
	stPacket.nJobFirst			= pPlayer->GetJop(0);
	stPacket.nJobCur			= pPlayer->GetJop(pPlayer->GetJopGrade()-1);
	stPacket.nJobGrade			= pPlayer->GetJopGrade();
	stPacket.nLV				= pPlayer->GetLevel();
	SafeStrCpy(stPacket.szName, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);
	stPacket.nResult			= nResult;

	pPlayer->SendMsg(&stPacket, sizeof(stPacket));
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_ProcessHonorPoint Method														  명예 포인트 처리 (몬스터 사냥시 경험치에 의한)
//
VOID CSHFamilyManager::SRV_ProcessHonorPoint(CPlayer* pPlayer, DWORD nExp)
{
#if defined(_MAPSERVER_)
	 nExp = (DWORD)(nExp*MONKILL_HONOR_POINT_EXP_RATE);

	if (nExp)
	{
		MSG_DWORD stPacket;
		// 클라이언트에 알리기
		// ..클라이언트에 바로 가지 않고 Agent에서 처리할 것을 처리하고 간다
		stPacket.Category			= MP_FAMILY;
		stPacket.Protocol			= MP_FAMILY_HONOR_POINT;
		stPacket.dwObjectID			= pPlayer->GetID();
		stPacket.dwData				= nExp;

		pPlayer->SendMsg(&stPacket, sizeof(stPacket));
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_UpdateMapPlayerInfo Method																			 맵의 플레이어 정보 업데이트
//
VOID CSHFamilyManager::SRV_UpdateMapPlayerInfo(CPlayer* pPlayer, char* pszGuildName)
{
#if defined(_MAPSERVER_)
	MSG_DWORD_NAME stPacket;
	ZeroMemory(&stPacket, sizeof(stPacket));

	// 클라이언트에 알리기
	// ..클라이언트에 바로 가지 않고 Agent에서 처리할 것을 처리하고 간다
	stPacket.Category			= MP_FAMILY;
	stPacket.Protocol			= MP_FAMILY_UPDATE_MAP_PLAYER_INFO;
	stPacket.dwObjectID			= pPlayer->GetID();
	stPacket.dwData				= pPlayer->GetLevel();
	if (pszGuildName)
	{
		SafeStrCpy(stPacket.Name, pszGuildName, MAX_NAME_LENGTH+1);
	}

	pPlayer->SendMsg(&stPacket, sizeof(stPacket));
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_SendNicknameToClient Method																			  호칭을 클라이언트에 보내기
//
VOID CSHFamilyManager::SRV_SendNicknameToClient(CPlayer* pPlayer, char* pszNickname)
{
#if defined(_MAPSERVER_)
	pPlayer->SetFamilyNickName(pszNickname);

	MSG_NAME stPacket;

	stPacket.Category			= MP_FAMILY;
	stPacket.Protocol			= MP_FAMILY_SHOW_NICKNAME;
	stPacket.dwObjectID			= pPlayer->GetID();
	SafeStrCpy(stPacket.Name, pszNickname, MAX_NAME_LENGTH+1);

	PACKEDDATA_OBJ->QuickSend(pPlayer, &stPacket, sizeof(stPacket));
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_RegistEmblem Method																									   문장 등록
//
VOID CSHFamilyManager::SRV_RegistEmblem(CPlayer* pPlayer, DWORD nFamilyID, char* pImgData, DWORD nChangedNum)
{
#if defined(_MAPSERVER_)
	if (pPlayer->GetFamilyEmblem() == NULL)
	{
		char* pcEmblem = new char[EMBLEM_BUFSIZE];
		pPlayer->SetFamilyEmblem(pcEmblem);
	}
	memcpy(pPlayer->GetFamilyEmblem(), pImgData, EMBLEM_BUFSIZE);
	pPlayer->SetFamilyEmblemChangedFreq(nChangedNum);

	PACKET_FAMILY_EMBLEM stPacket;

	stPacket.Category			= MP_FAMILY;
	stPacket.Protocol			= MP_FAMILY_REGIST_EMBLEM;
	stPacket.dwObjectID			= pPlayer->GetID();
	stPacket.nFamilyID			= nFamilyID;
	stPacket.nChangedFreq		= pPlayer->GetFamilyEmblemChangedFreq();
	stPacket.nTargetPlayerID	= pPlayer->GetID();
	memcpy(stPacket.cImgData, pImgData, EMBLEM_BUFSIZE);

	PACKEDDATA_OBJ->QuickSend(pPlayer, &stPacket, sizeof(stPacket));
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_ShowEmblem Method																									   문장 표시
//
VOID CSHFamilyManager::SRV_ShowEmblem(CPlayer* pPlayer, DWORD nFamilyID, char* pImgData, DWORD nChangedNum)
{
#if defined(_MAPSERVER_)
	// 맵서버에 패밀리ID, 문장 정보 설정
	pPlayer->SetFamilyIdx(nFamilyID);
	if (pPlayer->GetFamilyEmblem() == NULL)
	{
		char* pcEmblem = new char[EMBLEM_BUFSIZE];
		pPlayer->SetFamilyEmblem(pcEmblem);
	}
	memcpy(pPlayer->GetFamilyEmblem(), pImgData, EMBLEM_BUFSIZE);
	pPlayer->SetFamilyEmblemChangedFreq(nChangedNum);

	MSG_DWORD3 stPacket;

	stPacket.Category			= MP_FAMILY;
	stPacket.Protocol			= MP_FAMILY_SHOW_EMBLEM;
	stPacket.dwObjectID			= pPlayer->GetID();
	stPacket.dwData1			= nFamilyID;
	stPacket.dwData2			= pPlayer->GetID();
	stPacket.dwData3			= pPlayer->GetFamilyEmblemChangedFreq();
	// QuickSend 로는 pPlayer 에게 전송이 되지 않는다.. 에이전트에서 문장을 로드해서 보낸 데이터를
	// 여기(맵)에서 받은 건데, 이 시점에서는 pPlayer 는 그리드에 추가되어 있지 않은 듯...??
	// 그래서 아래처럼 2개로 나눠서 보냄
	PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pPlayer, &stPacket, sizeof(stPacket));
	pPlayer->SendMsg(&stPacket, sizeof(stPacket));
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_ShowEmblem2 Method																									  문장 표시2
//
VOID CSHFamilyManager::SRV_ShowEmblem2(CPlayer* pPlayer, DWORD nTargetPlayerID)
{
#if defined(_MAPSERVER_)
	CPlayer* pTargetPlayer = (CPlayer*)g_pUserTable->FindUser( nTargetPlayerID );
	if( !pTargetPlayer ) return;
	
	if (pTargetPlayer->GetFamilyEmblem())
	{
		PACKET_FAMILY_EMBLEM stPacket;

		stPacket.Category			= MP_FAMILY;
		stPacket.Protocol			= MP_FAMILY_REGIST_EMBLEM;
		stPacket.dwObjectID			= pPlayer->GetID();
		stPacket.nFamilyID			= pTargetPlayer->GetFamilyIdx();
		stPacket.nTargetPlayerID	= nTargetPlayerID;
		stPacket.nChangedFreq		= pTargetPlayer->GetFamilyEmblemChangedFreq();
		memcpy(stPacket.cImgData, pTargetPlayer->GetFamilyEmblem(), EMBLEM_BUFSIZE);

		pPlayer->SendMsg(&stPacket, sizeof(stPacket));
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_RegistEmblem Method																											탈퇴
//
VOID CSHFamilyManager::SRV_Leave(CPlayer* pPlayer, DWORD nPlayerID, int nKind)
{
#if defined(_MAPSERVER_)
	switch(nKind)
	{
	// 문장 제거, 경험치 다운
	case 0:
		{
			if (pPlayer == NULL) return;
			// 문장 제거
			if (pPlayer->GetFamilyEmblem())
			{
				char* pcEmblem = pPlayer->GetFamilyEmblem();
				delete[] pcEmblem;
				pPlayer->SetFamilyEmblem(NULL);
			}

			char szName[MAX_NAME_LENGTH+1] = "";
			pPlayer->SetFamilyNickName(szName);

			MSG_DWORD stPacket;

			stPacket.Category			= MP_FAMILY;
			stPacket.Protocol			= MP_FAMILY_LEAVE_EMBLEM;
			stPacket.dwData				= pPlayer->GetID();

			PACKEDDATA_OBJ->QuickSend(pPlayer, &stPacket, sizeof(stPacket));

			// 경험치 다운
			if (LEAVE_PENALTY_EXP_DOWN_RATE[FLK_LEAVE])
			{
				LEVELTYPE nOldLevel = pPlayer->GetLevel();
				EXPTYPE pointForLevel = GAMERESRCMNGR->GetMaxExpPoint(pPlayer->GetLevel());
				EXPTYPE minusExp = (EXPTYPE)(pointForLevel * LEAVE_PENALTY_EXP_DOWN_RATE[FLK_LEAVE]);
				pPlayer->ReduceExpPoint(minusExp, eExpLog_LosebyLeaveFamily);
				// ..메세지
				MSG_DWORD3 msg;													
				msg.Category	= MP_FAMILY ;										
				msg.Protocol	= MP_FAMILY_PENALTY_MSG;			
				msg.dwObjectID	= pPlayer->GetID();
				msg.dwData1		= 0;
				msg.dwData2		= (DWORD)(LEAVE_PENALTY_EXP_DOWN_RATE[FLK_LEAVE]*100+0.001f);
				msg.dwData3		= 0;
				if (nOldLevel > pPlayer->GetLevel()) msg.dwData3 = 1;
				pPlayer->SendMsg((MSGBASE*)&msg, sizeof(msg));
			}

			// 탈퇴하는 멤버가 심은 농작물이 있는지 확인하고 제거..
			g_csFarmManager.SRV_DelCrop(nPlayerID);
			// 080423 KTH -- 가축이 있으면 삭제
			g_csFarmManager.SRV_DelAnimal(nPlayerID);
			pPlayer->SetFarmCropPlantRetryTime(0);
			pPlayer->SetFarmCropManureRetryTime(0);
			// DB에 저장
			Farm_SetTimeDelay(pPlayer->GetID(), CSHFarmManager::FARM_TIMEDELAY_KIND_PLANT, 0);
			Farm_SetTimeDelay(pPlayer->GetID(), CSHFarmManager::FARM_TIMEDELAY_KIND_MANURE, 0);
			// 080430 KTH -- Animal Delay Add
		}
		break;
	// 농작물 제거
	case 1:
		{
			// 탈퇴하는 멤버가 심은 농작물이 있는지 확인하고 제거..
			g_csFarmManager.SRV_DelCrop(nPlayerID);
			// 080423 KTH
			g_csFarmManager.SRV_DelAnimal(nPlayerID);
		}
		break;
	}
	
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_Expel Method																												추방
//
VOID CSHFamilyManager::SRV_Expel(CPlayer* pPlayer, DWORD nMemberID, int nKind)
{
#if defined(_MAPSERVER_)
	switch(nKind)
	{
	// 문장 제거
	case 0:
		{
			if (pPlayer == NULL) return;
			// 멤버일 경우
			if (pPlayer->GetID() == nMemberID)
			{
				// 문장 제거
				if (pPlayer->GetFamilyEmblem())
				{
					char* pcEmblem = pPlayer->GetFamilyEmblem();
					delete[] pcEmblem;
					pPlayer->SetFamilyEmblem(NULL);
				}

				char szName[MAX_NAME_LENGTH+1] = "";
				pPlayer->SetFamilyNickName(szName);

				MSG_DWORD stPacket;

				stPacket.Category			= MP_FAMILY;
				stPacket.Protocol			= MP_FAMILY_LEAVE_EMBLEM;
				stPacket.dwData				= pPlayer->GetID();

				PACKEDDATA_OBJ->QuickSend(pPlayer, &stPacket, sizeof(stPacket));

				// 탈퇴하는 멤버가 심은 농작물이 있는지 확인하고 제거..
				g_csFarmManager.SRV_DelCrop(nMemberID);
				// 080423 KTH
				g_csFarmManager.SRV_DelAnimal(nMemberID);
				pPlayer->SetFarmCropPlantRetryTime(0);
				pPlayer->SetFarmCropManureRetryTime(0);
				// DB에 저장
				Farm_SetTimeDelay(pPlayer->GetID(), CSHFarmManager::FARM_TIMEDELAY_KIND_PLANT, 0);
				Farm_SetTimeDelay(pPlayer->GetID(), CSHFarmManager::FARM_TIMEDELAY_KIND_MANURE, 0);
			}
			// 마스터일 경우
			else
			{
				// 경험치 다운
				if (LEAVE_PENALTY_EXP_DOWN_RATE[FLK_EXPEL])
				{
					LEVELTYPE nOldLevel = pPlayer->GetLevel();
					EXPTYPE pointForLevel = GAMERESRCMNGR->GetMaxExpPoint(pPlayer->GetLevel());
					EXPTYPE minusExp = (EXPTYPE)(pointForLevel * LEAVE_PENALTY_EXP_DOWN_RATE[FLK_EXPEL]);
					pPlayer->ReduceExpPoint(minusExp, eExpLog_LosebyExpleFamilyMember);
					// ..메세지
					MSG_DWORD3 msg ;													
					msg.Category	= MP_FAMILY ;										
					msg.Protocol	= MP_FAMILY_PENALTY_MSG;			
					msg.dwObjectID	= pPlayer->GetID();
					msg.dwData1		= 1;
					msg.dwData2		= (DWORD)(LEAVE_PENALTY_EXP_DOWN_RATE[FLK_EXPEL]*100+0.001f);
					msg.dwData3		= 0;
					if (nOldLevel > pPlayer->GetLevel()) msg.dwData3 = 1;
					pPlayer->SendMsg((MSGBASE*)&msg, sizeof(msg));
				}
			}
		}
		break;
	// 농작물 제거
	case 1:
		{
			// 탈퇴하는 멤버가 심은 농작물이 있는지 확인하고 제거..
			g_csFarmManager.SRV_DelCrop(nMemberID);
			// 080423 KTH
			g_csFarmManager.SRV_DelAnimal(nMemberID);
		}
		break;
	}
	
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  SRV_BreakUp Method																												해체
//
VOID CSHFamilyManager::SRV_BreakUp(CPlayer* pPlayer, DWORD nPlayerID, int nKind)
{
#if defined(_MAPSERVER_)
	switch(nKind)
	{
	// 문장 제거
	case 0:
		{
			if (pPlayer == NULL) return;
			
			// 문장 제거
			if (pPlayer->GetFamilyEmblem())
			{
				char* pcEmblem = pPlayer->GetFamilyEmblem();
				delete[] pcEmblem;
				pPlayer->SetFamilyEmblem(NULL);
			}

			char szName[MAX_NAME_LENGTH+1] = "";
			pPlayer->SetFamilyNickName(szName);

			MSG_DWORD stPacket;

			stPacket.Category			= MP_FAMILY;
			stPacket.Protocol			= MP_FAMILY_LEAVE_EMBLEM;
			stPacket.dwData				= pPlayer->GetID();

			PACKEDDATA_OBJ->QuickSend(pPlayer, &stPacket, sizeof(stPacket));

			pPlayer->SetFarmCropPlantRetryTime(0);
			pPlayer->SetFarmCropManureRetryTime(0);
			// DB에 저장
			Farm_SetTimeDelay(pPlayer->GetID(), CSHFarmManager::FARM_TIMEDELAY_KIND_PLANT, 0);
			Farm_SetTimeDelay(pPlayer->GetID(), CSHFarmManager::FARM_TIMEDELAY_KIND_MANURE, 0);

			// 080430 KTH -- Animal Delay Add

			// 마스터일 경우
			if (pPlayer->GetID() == nPlayerID)
			{
				// 경험치 다운
				if (LEAVE_PENALTY_EXP_DOWN_RATE[FLK_BREAKUP])
				{
					LEVELTYPE nOldLevel = pPlayer->GetLevel();
					EXPTYPE pointForLevel = GAMERESRCMNGR->GetMaxExpPoint(pPlayer->GetLevel());
					EXPTYPE minusExp = (EXPTYPE)(pointForLevel * LEAVE_PENALTY_EXP_DOWN_RATE[FLK_BREAKUP]);
					pPlayer->ReduceExpPoint(minusExp, eExpLog_LosebyBreakupFamily);
					// ..메세지
					MSG_DWORD3 msg ;													
					msg.Category	= MP_FAMILY ;										
					msg.Protocol	= MP_FAMILY_PENALTY_MSG;			
					msg.dwObjectID	= pPlayer->GetID();
					msg.dwData1		= 2;
					msg.dwData2		= (DWORD)(LEAVE_PENALTY_EXP_DOWN_RATE[FLK_BREAKUP]*100+0.001f);
					msg.dwData3		= 0;
					if (nOldLevel > pPlayer->GetLevel()) msg.dwData3 = 1;
					pPlayer->SendMsg((MSGBASE*)&msg, sizeof(msg));
				}
				// 농장 삭제
				g_csFarmManager.SRV_DelFarm(pPlayer->GetID(), CSHFarmManager::FARM_DEL_KIND_BREAKUP_FAMILY);
			}
		}
		break;
	// 농작물 제거
	case 1:
		{
			// 농장 삭제
			g_csFarmManager.SRV_DelFarm(nPlayerID, CSHFarmManager::FARM_DEL_KIND_BREAKUP_FAMILY);
		}
		break;
	}
	
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
// CLI_RequestCreateFamily Method																					    패밀리 생성 요청
//
VOID CSHFamilyManager::CLI_RequestCreateFamily(char* szFamilyName)
{
#if !defined(_AGENTSERVER) && !defined(_MAPSERVER_)
	/*
	if( ( FILTERTABLE->IsInvalidCharInclude((unsigned char *)FamilyName) == TRUE ) || 
		( FILTERTABLE->IsUsableName(FamilyName) == FALSE ) )
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(698));
		return;
	}
	*/

	MSG_DWORD_NAME stPacket;
	stPacket.Category = MP_FAMILY;
	stPacket.Protocol = MP_FAMILY_CREATE;
	SafeStrCpy(stPacket.Name, szFamilyName, MAX_NAME_LENGTH+1);
	stPacket.dwObjectID = HEROID;

	NETWORK->Send(&stPacket, sizeof(stPacket));
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
// CLI_RequestInvite Method																						  패밀리원으로 초대 요청
//
VOID CSHFamilyManager::CLI_RequestInvite(CPlayer* pPlayer)
{
#if !defined(_AGENTSERVER) && !defined(_MAPSERVER_)
	MSG_DWORD stPacket;
	stPacket.Category 		= MP_FAMILY;
	stPacket.Protocol 		= MP_FAMILY_INVITE;
	stPacket.dwObjectID		= HEROID;
	stPacket.dwData			= pPlayer->GetID();

	NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
// CLI_RequestAnswerToInvite Method																				패밀리원 초대에 응답 요청
//
VOID CSHFamilyManager::CLI_RequestAnswerToInvite(FAMILY_INVITE_RESULT eResult)
{
#if !defined(_AGENTSERVER) && !defined(_MAPSERVER_)
	if (m_nInviteMasterID)
	{
		MSG_DWORD2 stPacket;
		stPacket.Category 		= MP_FAMILY;
		stPacket.Protocol 		= MP_FAMILY_INVITE_RESULT;
		stPacket.dwObjectID		= HEROID;
		stPacket.dwData1		= m_nInviteMasterID;
		stPacket.dwData2		= eResult;

		NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
// CLI_RequestRegistEmblem Method																						   문장 등록 요청
//
BOOL CSHFamilyManager::CLI_RequestRegistEmblem(char* pszBmpName)
{
#if !defined(_AGENTSERVER) && !defined(_MAPSERVER_)
	char cImgData[EMBLEM_BUFSIZE];

 	FIBITMAP* pBitMap = FreeImage_Load(FIF_BMP, pszBmpName, BMP_DEFAULT);
	BITMAPINFO* pBitInfo = FreeImage_GetInfo(pBitMap);

	if( (pBitMap == 0) || (pBitInfo == 0) )
	{
		char szTmp[256];
		sprintf(szTmp, "%s.bmp", pszBmpName);
		pBitMap = FreeImage_Load(FIF_BMP, szTmp, BMP_DEFAULT);
		pBitInfo = FreeImage_GetInfo(pBitMap);
		if( (pBitMap == 0) || (pBitInfo == 0) )
			return FALSE;
	}
	//사이즈가 틀림
	if( pBitInfo->bmiHeader.biHeight != EMBLEM_HEIGHT ||
		pBitInfo->bmiHeader.biWidth != EMBLEM_WIDTH )
		return FALSE;

	FIBITMAP* pConvertedBitmap = FreeImage_ConvertTo16Bits565(pBitMap);

	int bytesPerLine = EMBLEM_WIDTH*EMBLEM_BPP;
	BYTE* pData = FreeImage_GetBits(pConvertedBitmap);
	for(int y=0;y<EMBLEM_HEIGHT;++y)
	{
		memcpy(
			&cImgData[(EMBLEM_HEIGHT-y-1)*bytesPerLine],
			&pData[y*bytesPerLine],
			bytesPerLine);
	}
	
	FreeImage_Unload(pBitMap);
	FreeImage_Unload(pConvertedBitmap);

	PACKET_FAMILY_EMBLEM msg;

	SetProtocol(&msg,MP_FAMILY,MP_FAMILY_REGIST_EMBLEM);
	msg.dwObjectID = HEROID;
	msg.nFamilyID = HERO->GetFamily()->Get()->nID;
	msg.nTargetPlayerID = 0;
	memcpy(msg.cImgData,cImgData,EMBLEM_BUFSIZE);
	NETWORK->Send(&msg,sizeof(msg));

#endif
	return TRUE;
}

// -------------------------------------------------------------------------------------------------------------------------------------
// CLI_RequestShowEmblem Method															 문장 표시 요청 (그리드안의 다른 플레이어들에게)
//
VOID CSHFamilyManager::CLI_RequestShowEmblem(DWORD nPlayerID, DWORD nTargetPlayerID)
{
#if !defined(_AGENTSERVER) && !defined(_MAPSERVER_)
	MSG_DWORD stPacket;

	stPacket.Category	= MP_FAMILY;
	stPacket.Protocol	= MP_FAMILY_SHOW_EMBLEM_2;
	stPacket.dwObjectID = nPlayerID;
	stPacket.dwData		= nTargetPlayerID;
	NETWORK->Send(&stPacket, sizeof(stPacket));
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
// CLI_ShowEmblem Method																									   문장 표시
//
VOID CSHFamilyManager::CLI_ShowEmblem(CPlayer* pPlayer, DWORD nFamilyID, DWORD nChangedFreq)
{
#if !defined(_AGENTSERVER) && !defined(_MAPSERVER_)
	// 이미 전송받아서 저장하고 있는 문장이라면, 해당 파일의 문장을 사용한다.
	char szEmblem[256];
	DWORD nOldChangedFreq = 0;
	sprintf(szEmblem, "Data/Interface/2DImage/FamilyEmblem/%d_%d.txt", g_nServerSetNum, nFamilyID);
	FILE* fp = fopen(szEmblem, "rt");
	if (fp)
	{
		// 다른 그리드(패밀리 멤버가 아닐 경우), 캐릭터 선택화면, 맵이동 중 등등 경우에는 변경되는 문장을 전송받지 못한다.
		// 그래서, 문장의 변경 회수를 비교해서 클라이언트 상의 변경 회수가 실제 변경 회수 보다 작으면 
		// 변경된 문장을 요청한다. 
		// 하지만 같은 그리드 내에서 변경된 다른 패밀리의 문장을 받은 후 그리드 밖으로 나간 상황에서 그 패밀리의 문장이 또
		// 변경되었을 경우, 그 패밀리 멤버가 로그아웃을 하지 않고, 다시 그리드 안으로 들어올 경우는 변경된 문장을 받을 수 있지만
		// 로그아웃 후 다시 접속했을 경우에는 (모든 패밀리 멤버가.. 그래서 문장 변경 회수가 초기화가 되면) 변경 회수가 초기화되어서
		// 변경된 문장을 받을 수 없고, 마지막으로 저장한 문장으로 표시하게 된다. 변경 회수를 DB에 저장하지 않는 이상 어쩔 수 없음.
		// 클라이언트 실행 후 처음 접속 시에 문장 파일을 초기화하기 때문에 클라이언트를 다시 실행하면 해결되는 문제임.
		fscanf(fp, "%d", &nOldChangedFreq);
		if (nOldChangedFreq < nChangedFreq)
		{
			CLI_RequestShowEmblem(HERO->GetID(), pPlayer->GetID());
		}
		else
		{
			sprintf(szEmblem, "Data/Interface/2DImage/FamilyEmblem/%d_%d_%d.tga", g_nServerSetNum, nFamilyID, nOldChangedFreq);
			if (_access(szEmblem, 0) != -1)
			{
				cImageSelf* pEmblem = pPlayer->GetFamilyEmblem();
				if (pEmblem)
				{
					pEmblem->Release();
					delete pEmblem;
				}

				pEmblem = new cImageSelf;
				pEmblem->LoadSprite(szEmblem, EMBLEM_WIDTH, EMBLEM_HEIGHT);
				pPlayer->SetFamilyEmblem(pEmblem);
			}
		}
		fclose(fp);
	}
	// 없으면 다시 서버에 요청
	else
	{
		CLI_RequestShowEmblem(HERO->GetID(), pPlayer->GetID());
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
// CLI_RequestChangeNickname Method																						  호칭 변경 요청
//
VOID CSHFamilyManager::CLI_RequestChangeNickname(DWORD nTargetPlayerID, char* szNickname)
{
#if !defined(_AGENTSERVER) && !defined(_MAPSERVER_)
	MSG_DWORD_NAME stPacket;
	stPacket.Category 		= MP_FAMILY;
	stPacket.Protocol 		= MP_FAMILY_CHANGE_NICKNAME;
	stPacket.dwObjectID		= HEROID;								// 호칭을 변경하는 캐릭터ID
	stPacket.dwData			= nTargetPlayerID;						// 호칭이 변경될 캐릭터ID
	SafeStrCpy(stPacket.Name, szNickname, MAX_NAME_LENGTH+1);

	// ' 는 문자열을 나타내는 표시이므로 문자열로 사용할 수 없다. " 로 대체하도록 함.
	for(int i=0; i<MAX_NAME_LENGTH+1; i++)
	{
		if (stPacket.Name[i] == '\'')
		{
			stPacket.Name[i] = '"';
		}
	}

	NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
// CLI_RequestRegistEmblem Method																							  호칭ON 요청
//
VOID CSHFamilyManager::CLI_RequestChangeNicknameON()
{
#if !defined(_AGENTSERVER) && !defined(_MAPSERVER_)
	MSGBASE stPacket;
	stPacket.Category 		= MP_FAMILY;
	stPacket.Protocol 		= MP_FAMILY_CHANGE_NICKNAMEON;
	stPacket.dwObjectID		= HEROID;

	NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
// CLI_RequestLeave Method																									   탈퇴 요청
//
VOID CSHFamilyManager::CLI_RequestLeave()
{
#if !defined(_AGENTSERVER) && !defined(_MAPSERVER_)
	MSGBASE stPacket;
	stPacket.Category 		= MP_FAMILY;
	stPacket.Protocol 		= MP_FAMILY_LEAVE;
	stPacket.dwObjectID		= HEROID;

	NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
// CLI_RequestExpel Method																									   추방 요청
//
VOID CSHFamilyManager::CLI_RequestExpel(int nMemberID)
{
#if !defined(_AGENTSERVER) && !defined(_MAPSERVER_)
	MSG_DWORD stPacket;
	stPacket.Category 		= MP_FAMILY;
	stPacket.Protocol 		= MP_FAMILY_EXPEL;
	stPacket.dwObjectID		= HEROID;
	stPacket.dwData			= nMemberID;

	NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
// CLI_RequestBreakUp Method																								   해체 요청
//
VOID CSHFamilyManager::CLI_RequestBreakUp()
{
#if !defined(_AGENTSERVER) && !defined(_MAPSERVER_)
	MSG_DWORD stPacket;
	stPacket.Category 		= MP_FAMILY;
	stPacket.Protocol 		= MP_FAMILY_BREAKUP;
	stPacket.dwObjectID		= HEROID;

	NETWORK->Send( (MSGROOT*)&stPacket, sizeof(stPacket) );
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  CLI_ParseAnswerFromSrv Method																			  서버로부터의 응답 분석/처리
//
VOID CSHFamilyManager::CLI_ParseAnswerFromSrv(void* pMsg)
{
#if !defined(_MAPSERVER_) && !defined(_AGENTSERVER)														// 맵서버나, 에이전트 서버가 아니라면,

	char szTxt[256];																					// 임시 버퍼를 선언한다.

	MSGBASE* pTmp = (MSGBASE*)pMsg;																		// 패킷 메시지를 임시 패킷에 받는다.

	switch(pTmp->Protocol)																				// 패킷의 프로토콜을 확인한다.
	{
	// 생성 처리
	case MP_FAMILY_CREATE:																				// 패밀리 생성 결과 프로토콜이면,
		{
			MSG_DWORD2* pstPacket = (MSG_DWORD2*)pMsg;													// 패킷 정보를 받는다.

			switch(pstPacket->dwData1)																	// 패킷 데이타를 확인한다.
			{
			case RESULT_OK:																				// 생성 성공 메시지라면,
				{
					WINDOWMGR->MsgBox( MBI_FAMILY_CREATE_SUCESS, MBT_OK, CHATMGR->GetChatMsg( 729 ) );	// 패밀리 생성 성공 메시지를 출력한다.

					CFamilyCreateDialog* pDlg = GAMEIN->GetFamilyCreateDlg() ;							// 패밀리 생성창 정보를 받아온다.

					if( pDlg )																			// 패밀리 생성창 정보가 유효한지 체크한다.
					{
						pDlg->SetActive( FALSE ) ;														// 패밀리 생성창을 닫는다.
					}
				}
				break;

			case RESULT_FAIL_01:																		// 이미 패밀리에 속해있다는 메시지 라면,
				{
					WINDOWMGR->MsgBox( MBI_FAMILY_ALREADY_HAVE_FAMILY, MBT_OK, CHATMGR->GetChatMsg( 697 ) );// 이미 패밀리에 속해있다는 알림창을 출력한다.

					CFamilyCreateDialog* pDlg = GAMEIN->GetFamilyCreateDlg() ;							// 패밀리 생성창 정보를 받아온다.

					if( pDlg )																			// 패밀리 생성창 정보가 유효한지 체크한다.
					{
						pDlg->SetActive( FALSE ) ;														// 패밀리 생성창을 닫는다.
					}
				}
				break;

			case RESULT_FAIL_02:																		// 같은 이름이 이미 존재하거나 사용할 수 없는 메시지 라면,
				{				
					WINDOWMGR->MsgBox( MBI_FAMILY_SAME_FAMILYNAME, MBT_OK, CHATMGR->GetChatMsg( 698 ) );	// 예외 처리를 한다.
				}
				break;
			// 탈퇴
			case RESULT_FAIL_03:
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 1145 ), LEAVE_PENALTY_REJOIN_TIME[FLK_LEAVE], pstPacket->dwData2);
				}
				break;
			// 추방
			case RESULT_FAIL_04:
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 1146 ), LEAVE_PENALTY_REJOIN_TIME[FLK_EXPEL], pstPacket->dwData2);
				}
				break;
			// 해체
			case RESULT_FAIL_05:
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 1147 ), LEAVE_PENALTY_REJOIN_TIME[FLK_BREAKUP], pstPacket->dwData2);
				}
				break;
			}
		}
		break;
	// 초대
	case MP_FAMILY_INVITE:																				// 패밀리 초대 프로토콜이면,
		{
			MSG_DWORD2* pstPacket = (MSG_DWORD2*)pMsg;													// 패킷 정보를 받는다.

			switch(pstPacket->dwData1)																	// 데이터를 확인한다.
			{
			case RESULT_OK:																				// 초대 성공 메시지라면,
				{
					char inviteBuf[128] = {0, } ;																// 초대 메시지를 만들 임시 버퍼를 선언한다.

					CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetSelectedObject() ;								// 선택한 플레이어 정보를 받는다.
					sprintf( inviteBuf, CHATMGR->GetChatMsg( 1153 ), pPlayer->GetObjectName() ) ;		// 초대 메시지를 만든다.
					WINDOWMGR->MsgBox( MBI_FAMILY_INVITE, MBT_OK, inviteBuf );									// 초대 메시지를 보냈다는 알림창을 띄운다.
				}
				break;
			case RESULT_FAIL_01:																		// 패밀리에 속해있지 않다는 메시지라면,
				//MessageBox(NULL, "패밀리에 속해 있지 않음!", NULL, NULL);
				//WINDOWMGR->MsgBox( MBI_FAMILY_NO_FAMILY, MBT_OK, CHATMGR->GetChatMsg( 698 ) );		// 예외 처리를 한다.
				break;
			case RESULT_FAIL_02:																		// 마스터만 초대 가능하다는 메시지라면,
				WINDOWMGR->MsgBox( MBI_FAMILY_ABLE_MASTER, MBT_OK, CHATMGR->GetChatMsg( 732 ) );		// 예외 처리를 한다.
				break;
			case RESULT_FAIL_03:																		// 정원을 초과 했다는 메시지라면,
				WINDOWMGR->MsgBox( MBI_FAMILY_FULL_MEMBER, MBT_OK, CHATMGR->GetChatMsg( 738 ) );		// 예외 처리를 한다.
				break;
			case RESULT_FAIL_04:																		// 다른 패밀리에 속해있는 플레이어라는 메시지라면,
				WINDOWMGR->MsgBox( MBI_FAMILY_OTHER_FAMILY, MBT_OK, CHATMGR->GetChatMsg( 735 ) );		// 예외 처리를 한다.
				break;
			// 추방
			case RESULT_FAIL_05:
 				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 1148 ), LEAVE_PENALTY_REJOIN_TIME[FLK_EXPEL], pstPacket->dwData2);
				break;
			// 탈퇴
			case RESULT_FAIL_06:
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 1149 ), LEAVE_PENALTY_REJOIN_TIME[FLK_LEAVE], pstPacket->dwData2) ;
				break;
			}
		}
		break;
	// 초대됨
	case MP_FAMILY_INVITED:																				// 초대를 받는 프로토콜이면,
		{
			MSG_DWORD_NAME* pstPacket = (MSG_DWORD_NAME*)pMsg;											// 패킷을 받는다.

			CPlayer* pMaster = (CPlayer*)OBJECTMGR->GetObject( pstPacket->dwData );						// 마스터 정보를 받는다.

			if (pMaster == NULL)																		// 마스터 정보가 유효하지 않다면,
			{
				//WINDOWMGR->MsgBox( MBI_FAMILY_NO_MASTER_POSITION, MBT_OK, "마스터 또는 마스터의 위치가 불분명!!" );	// 예외 처리를 한다.
				return;																					// 리턴 처리를 한다.
			}

			sprintf(szTxt, CHATMGR->GetChatMsg( 1154 ),													// 초대 받는 메시지를 설정한다.
					pstPacket->Name, pMaster->GetObjectName(), HERO->GetObjectName());
			WINDOWMGR->MsgBox( MBI_FAMILY_INVITE_YOU, MBT_YESNO, szTxt );								// 알림 창을 띄운다.

			m_nInviteMasterID = pstPacket->dwData;														// 초대를 신청한 마스터의 아이디를 받는다.
		}
		break;
	// 초대 결과
	case MP_FAMILY_INVITE_RESULT:																		// 초대 결과를 받는 프로토콜이면,
	case MP_FAMILY_INVITE_RESULT_TO_OTHER_AGENT_2:
		{
			MSG_NAME_DWORD2* pstPacket = (MSG_NAME_DWORD2*)pMsg;										// 패킷을 받는다.

 			CPlayer* pTargetPlayer = (CPlayer*)OBJECTMGR->GetObject( pstPacket->dwData1 );				// 초대를 한 타겟의 정보를 받는다.

  			if (pTargetPlayer == NULL)																	// 타겟 정보가 유효한지 체크를 한다.
			{
				return;																					// 리턴 처리를 한다.
			}

			switch(pstPacket->dwData2)																	// 패킷의 데이터를 확인한다.
			{
			case FIR_ACCEPT_MASTER:																		// 초대를 수락했을 때 패밀리 마스터에게 오는 메시지라면,
				{
					if (WINDOWMGR->GetWindowForID(MBI_FAMILY_INVITE) &&
						WINDOWMGR->GetWindowForID(MBI_FAMILY_INVITE)->IsActive())
					{
						WINDOWMGR->CloseWindowForID(MBI_FAMILY_INVITE);
					}
					sprintf(szTxt, CHATMGR->GetChatMsg( 700 ), pTargetPlayer->GetObjectName());			// 상대가 패밀리 멤버가 되었다는 메시지를 설정한다.
					WINDOWMGR->MsgBox( MBI_FAMILY_ACCEPT_MASTER, MBT_OK, szTxt );						// 알림 메시지를 띄운다.
					CHATMGR->AddMsg(CTC_SYSMSG, szTxt) ;												// 채팅창에 메시지를 출력한다.

					CFamilyDialog* pDlg = GAMEIN->GetFamilyDlg() ;										// 패밀리 다이얼로그 정보를 받아온다.

					if( pDlg )																			// 패밀리 생성창 정보가 유효한지 체크한다.
					{
						pDlg->UpdateAllInfo() ;															// 패밀리 정보를 업데이트 한다.
						pDlg->SetActive( TRUE ) ;														// 패밀리 생성창을 닫는다.
					}
				}
				break;
			case FIR_DENY_MASTER:																		// 초대를 거절했을 때 마스터에게 오는 메시지라면,
				{
					if (WINDOWMGR->GetWindowForID(MBI_FAMILY_INVITE) &&
						WINDOWMGR->GetWindowForID(MBI_FAMILY_INVITE)->IsActive())
					{
						WINDOWMGR->CloseWindowForID(MBI_FAMILY_INVITE);
					}
					sprintf(szTxt, CHATMGR->GetChatMsg( 1155 ),											// 초대를 거부했다는 메시지를 설정한다.
							pTargetPlayer->GetObjectName(), HERO->GetObjectName());
					WINDOWMGR->MsgBox( MBI_FAMILY_DENY_MASTER, MBT_OK, szTxt );							// 알림 메시지를 띄운다.
					CHATMGR->AddMsg(CTC_SYSMSG, szTxt) ;												// 채팅창에 메시지를 출력한다.
				}
				break;
			case FIR_ACCEPT_PLAYER:																		// 초대를 수락했을 때 수락한 사람에게 오는 메시지라면,
				{
					sprintf(szTxt,CHATMGR->GetChatMsg( 1156 ), pstPacket->Name);						// 패밀리 멤버가 되었다는 메시지를 설정한다.
					WINDOWMGR->MsgBox( MBI_FAMILY_ACCEPT_PLAYER, MBT_OK, szTxt );						// 알림 메시지를 띄운다.
					CHATMGR->AddMsg(CTC_SYSMSG, szTxt) ;												// 채팅창에 메시지를 출력한다.

					CFamilyDialog* pDlg = GAMEIN->GetFamilyDlg() ;										// 패밀리 다이얼로그 정보를 받아온다.

					if( pDlg )																			// 패밀리 생성창 정보가 유효한지 체크한다.
					{
						pDlg->UpdateAllInfo() ;															// 패밀리 정보를 업데이트 한다.
						pDlg->SetActive( TRUE ) ;														// 패밀리 생성창을 닫는다.
					}
				}
				break;
			case FIR_DENY_PLAYER:																		// 초대를 거절했을 때 거절한 사람에게 오는 메시지라면,
				{
					sprintf(szTxt,CHATMGR->GetChatMsg( 1157 ),pTargetPlayer->GetObjectName());			// 초대를 거절했다는 메시지를 설정한다.
					WINDOWMGR->MsgBox( MBI_FAMILY_DENY_PLAYER, MBT_OK, szTxt );							// 알림 메시지를 띄운다.
					CHATMGR->AddMsg(CTC_SYSMSG, szTxt) ;												// 채팅창에 메시지를 출력한다.
				}
				break;
			}
		}
		break;
	// 패밀리 데이터 받기
	case MP_FAMILY_DATA_FAMILY:																			// 패밀리 정보를 받는 프로토콜이면,
		{
			if (HERO == NULL)
			{
				return;
			}

			PACKET_FAMILY* pstPacket = (PACKET_FAMILY*)pMsg;											// 패킷을 받는다.

			CSHFamily csFamily;																			// 패밀리 정보를 담을 클래스를 선언한다.
			csFamily.Set(&pstPacket->stInfo, &pstPacket->stInfoEx);										// 패밀리의 기본, 추가 정보를 세팅한다.

			HERO->SetFamily(&csFamily);																	// 세팅 된 패밀리를 나의 패밀리로 세팅한다.
		}
		break;
	// 패밀리 멤버 데이터 받기
	case MP_FAMILY_DATA_MEMBER:																			// 패밀리 멤버 정보를 받는 프로토콜이면,
		{
			if (HERO == NULL)
			{
				return;
			}

			PACKET_MEMBER_INFO* pstMemberPacket = (PACKET_MEMBER_INFO*)pMsg;							// 패킷을 받는다.

			CSHFamilyMember csMember;																	// 패밀리 멤버 정보를 담을 클래스를 선언한다.
			csMember.Set(&pstMemberPacket->stInfo, &pstMemberPacket->stInfoEx);							// 패밀리 멤버의 기본, 추가 정보를 세팅한다.

			HERO->GetFamily()->SetMember(&csMember, pstMemberPacket->nIndex);							// 세팅 된 멤버를 나의 패밀리 멤버로 세팅한다.

			// 자기 호칭 설정
			// 원래 MP_FAMILY_SHOW_NICKNAME 를 받아야 하는데 그리드안의 다른 플레이어는 받는데 자신은 못 받는다.
			// 클라이언트가 접속되기 전에 서버에서 보내는 듯.. 멀티스레드 때문?
			if (csMember.Get()->nID == HERO->GetID())
			{
				HERO->SetFamilyNickName(csMember.Get()->szNickname);
				HERO->GetObjectBalloon()->SetFamilyNickName(csMember.Get()->szNickname);
			}

			CFamilyDialog* pDlg = GAMEIN->GetFamilyDlg() ;												// 패밀리 다이얼로그 정보를 받는다.

			if( pDlg )																					// 다이얼로그 정보가 유효한지 체크한다.
			{
				pDlg->UpdateAllInfo() ;																	// 패밀리 창을 업데이트 한다.
			}
		}
		break;
	// 호칭 변경
	case MP_FAMILY_CHANGE_NICKNAME:																		// 호칭 변경 프로토콜 이라면,
		{
			if( HERO )
			{
				MSG_DWORD_NAME* pstPacket = (MSG_DWORD_NAME*)pMsg;											// 패킷을 받는다.

				switch(pstPacket->dwData)																	// 패킷 데이터를 확인한다.
				{
				// 신청자
				// ..변경
				case RESULT_OK:																				// 변경 성공 메시지 라면,
					{																						// 신청자에게 메시지를 보낸다.
						sprintf(szTxt, CHATMGR->GetChatMsg( 1158 ), pstPacket->Name);						// 메시지를 설정한다.
						WINDOWMGR->MsgBox( MBI_FAMILY_CHANGENAME_COMPLETE, MBT_OK, szTxt );					// 알림창을 띄운다.
						CHATMGR->AddMsg(CTC_SYSMSG, szTxt) ;												// 채팅창에 메시지를 출력한다.

						CFamilyDialog* pDlg = GAMEIN->GetFamilyDlg() ;										// 패밀리 다이얼로그 정보를 받아온다.

						if( pDlg )																			// 패밀리 생성창 정보가 유효한지 체크한다.
						{
							pDlg->UpdateAllInfo() ;															// 패밀리 정보를 업데이트 한다.
							pDlg->SetActive( TRUE ) ;														// 패밀리 생성창을 닫는다.
						}
					}
					break;
				// ..실패
				case RESULT_FAIL_01:																		// 사용할 수 없다는 메시지 라면,
					{
						WINDOWMGR->MsgBox( MBI_FAMILY_CHANGENAME_FAIL, MBT_OK, CHATMGR->GetChatMsg( 551 ) );// 알림창을 띄운다.
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 551 )) ;							// 채팅창에 메시지를 출력한다.
					}
					break;
				// 대상자
				// ..변경
				case RESULT_OK_02:																			// 변경 성공 메시지라면,
					{																						// 대상자에게 메시지를 보낸다.
						sprintf(szTxt, CHATMGR->GetChatMsg( 1159 ),											// 메시지를 설정한다.
								pstPacket->Name, HERO->GetObjectName());
						WINDOWMGR->MsgBox( MBI_FAMILY_CHANGENAME_COMPLETE2, MBT_OK, szTxt );				// 알림창을 띄운다.
						CHATMGR->AddMsg(CTC_SYSMSG, szTxt) ;												// 채팅창에 메시지를 출력한다.

						CFamilyDialog* pDlg = GAMEIN->GetFamilyDlg() ;										// 패밀리 다이얼로그 정보를 받아온다.

						if( pDlg )																			// 패밀리 생성창 정보가 유효한지 체크한다.
						{
							pDlg->UpdateAllInfo() ;															// 패밀리 정보를 업데이트 한다.
							pDlg->SetActive( TRUE ) ;														// 패밀리 생성창을 닫는다.
						}
					}
					break;
				}
			}
		}
		break;
	// 명예 포인트
	case MP_FAMILY_HONOR_POINT:																			// 패밀리 명예 포인트를 받는 메시지라면,
		{
			if( HERO )																					// 히어로의 존재 여부 확인.
			{
				CSHFamily* pFamily = HERO->GetFamily() ;												// 히어로가 패밀리가 있는지 확인한다.

				if( pFamily )																			// 패밀리가 있다면,
				{
					MSG_DWORD* pstPacket = (MSG_DWORD*)pMsg;											// 패킷 정보를 받는다.

					CSHFamily::stINFO_EX stInfoEx = *pFamily->GetEx();									// 히어로의 패밀리 추가 정보를 세팅한다.

					// 농장 구입이나 업그레이드의 경우는 절감 포인트값을 - int 로 넘긴다. 
					// 그래서 int 로 캐스팅해서 - 값 인지 체크해서
					// 그럴 경우에는 -= 하도록 한다. 절감 포인트값이 int 범위라는 가정...
					if ((int)pstPacket->dwData < 0)
					{
						stInfoEx.nHonorPoint -= (DWORD)(-1*(int)pstPacket->dwData);
					}
					else
					{
						stInfoEx.nHonorPoint += pstPacket->dwData;										// 추가 정보의 명예 포인트에, 패킷으로 넘어온 포인트를 설정한다.
					}

					pFamily->SetEx(&stInfoEx);															// 히어로의 패밀리에 명예 포인트를 적용한다.

					CFamilyDialog* pDlg = GAMEIN->GetFamilyDlg() ;										// 패밀리 다이얼로그 정보를 받는다.

					if( pDlg )																			// 다이얼로그 정보가 유효한지 체크한다.
					{
						pDlg->SetHonorPoint( pstPacket->dwData ) ;										// 명예 포인트를 설정한다.
					}
				}
			}
		}
		break;
	// 호칭 표시
	case MP_FAMILY_SHOW_NICKNAME:
		{
			if (HERO == NULL)
			{
				return;
			}

			MSG_NAME* pstPacket = (MSG_NAME*)pMsg;
	
			if (HERO->GetID() == pstPacket->dwObjectID)
			{
				HERO->SetFamilyNickName(pstPacket->Name);
				HERO->GetObjectBalloon()->SetFamilyNickName(pstPacket->Name);
			}
			else
			{
				CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pstPacket->dwObjectID);
				if( !pPlayer )	return;

				pPlayer->SetFamilyNickName(pstPacket->Name);
				pPlayer->GetObjectBalloon()->SetFamilyNickName(pstPacket->Name);
			}
		}
		break;
	// 문장 등록/변경
	case MP_FAMILY_REGIST_EMBLEM:
		{
 			if (HERO == NULL)
			{
				return;
			}

   			PACKET_FAMILY_EMBLEM* pstPacket = (PACKET_FAMILY_EMBLEM*)pMsg;

			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pstPacket->nTargetPlayerID);
			if( !pPlayer )	return;


			// tga 라이브러리에서 한 번 로드한 파일은 로드된 모든 파일을 해제하지 않는 이상 같은 이름으로 tga를 저장하다고 해도
			// tga 로드 시 그 전에 저장한 tga 파일을 리턴하기 때문에 문장이 변경되었을 경우 그 문장을 참조하는 모든 자원을 해제하고
			// 같은 이름으로 저장하던가, 아니면 새로운 파일이름으로 저장해서 그것을 로드하던가 하는 식으로 해야 한다. 
			// 전자의 경우는 복잡해지기 때문에 따로 관리가 필요없는 후자의 방법을 사용한다. 
			// 그래서 등록/변경될 때마다 파일에 등록/변경 회수와 패밀리ID를 저장해서 다음에 등록/변경될 때
			// 새로운 파일이름으로 tga를 생성 로드하도록 한다. 
			char szEmblemInfo[256];
			char szEmblemImg[256];
			DWORD nOldChangedFreq = 0;
			sprintf(szEmblemInfo, "Data/Interface/2DImage/FamilyEmblem/%d_%d.txt", g_nServerSetNum, pstPacket->nFamilyID);

			FILE* fp = fopen(szEmblemInfo, "rt");
			if (fp)
			{
				fscanf(fp, "%d", &nOldChangedFreq);
				fclose(fp);
			}

			// 마스터가 아닌 일반멤버 문장을 받을 경우..
			if (nOldChangedFreq > pstPacket->nChangedFreq) pstPacket->nChangedFreq = nOldChangedFreq;

			// 새로운 문장 저장
			sprintf( szEmblemImg, "Data/Interface/2DImage/FamilyEmblem/%d_%d_%d.tga", g_nServerSetNum, pstPacket->nFamilyID, pstPacket->nChangedFreq);
			// 같은 파일이 없을 때만 파일을 생성하고 있으면 있는 것 로드. 다른 패밀리 멤버로 인해서 문장 정보가 왔었을 경우에 해당된다.
			if (access(szEmblemImg, 0) == -1)
			{
				char tempBuf[EMBLEM_BPP*EMBLEM_WIDTH*EMBLEM_WIDTH];
				memset( tempBuf, 0, EMBLEM_BPP*EMBLEM_WIDTH*EMBLEM_WIDTH );
				memcpy( tempBuf, pstPacket->cImgData, EMBLEM_BPP*EMBLEM_WIDTH*EMBLEM_WIDTH );
				WriteTGA( szEmblemImg, tempBuf, EMBLEM_WIDTH, EMBLEM_WIDTH, EMBLEM_BPP*EMBLEM_WIDTH, EMBLEM_BPP*8 );
			}
			// 변경 회수 저장
			fp = fopen(szEmblemInfo, "wt");
			fprintf(fp, "%d", pstPacket->nChangedFreq);
			fclose(fp);

			// 저장된 문장을 메모리에 로드
			cImageSelf* pEmblem = pPlayer->GetFamilyEmblem();
			if (pEmblem)
			{
				pEmblem->Release();
				delete pEmblem;
			}

			pEmblem = new cImageSelf;
			pEmblem->LoadSprite(szEmblemImg, EMBLEM_WIDTH, EMBLEM_HEIGHT);
			pPlayer->SetFamilyEmblem(pEmblem);
		}
		break;
	// 문장 표시
	case MP_FAMILY_SHOW_EMBLEM:
		{
 			if (HERO == NULL)
			{
				return;
			}

   			MSG_DWORD3* pstPacket = (MSG_DWORD3*)pMsg;

			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pstPacket->dwData2);
			if( !pPlayer )	return;

			CLI_ShowEmblem(pPlayer, pstPacket->dwData1, pstPacket->dwData3);
		}
		break;
	// 탈퇴
	case MP_FAMILY_LEAVE:
		{
 			if (HERO == NULL) return;

			CSHFamily* pFamily = HERO->GetFamily() ;
			if( pFamily )							
			{
   				MSG_DWORD* pstPacket = (MSG_DWORD*)pMsg;
	
				char szName[256];
				for(UINT i=0; i<HERO->GetFamily()->Get()->nMemberNum; i++)
				{
					if (HERO->GetFamily()->GetMember(i)->Get()->nID == pstPacket->dwData)
					{
						SafeStrCpy(szName, HERO->GetFamily()->GetMember(i)->Get()->szName, MAX_NAME_LENGTH+1);
						break;
					}
				}

				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 1150 ), szName);

				// 멤버 삭제
				pFamily->DelMember(pstPacket->dwData);
				CFamilyDialog* pDlg = GAMEIN->GetFamilyDlg() ;	
				if( pDlg ) pDlg->UpdateAllInfo() ;						

				// 탈퇴한 당사자라면 패밀리 정보를 지운다.
				if (HERO->GetID() == pstPacket->dwData)
				{
					CSHFamily csFamily;
					pFamily->Set(&csFamily);
					if( pDlg )										
					{
						pDlg->SetActive(FALSE);
					}

					// 농장 딜레이 타임 초기화
					CSHMonstermeterDlg* pDlg = GAMEIN->GetMonstermeterDlg() ;
					if (pDlg == NULL) return;
					pDlg->GetFarmRePlantTimeBar()->CLI_End();
					pDlg->GetFarmReManureTimeBar()->CLI_End();

					// 농장관리 창 닫기
					CSHFarmManageDlg* pFarmDlg = GAMEIN->GetFarmManageDlg();
					if (pFarmDlg)
					{
						pFarmDlg->SetActive(FALSE);
						// 미니맵의 농장 아이콘 제거
						CMiniMapDlg* pMiniMapDlg = GAMEIN->GetMiniMapDialog();
						if (pMiniMapDlg)
						{
							pMiniMapDlg->RemoveIcon(pFarmDlg->GetFarm()->GetID()+FARM_RENDER_OBJ_ID_START);
						}
						CBigMapDlg* pBigMapDlg = GAMEIN->GetBigMapDialog();
						if (pBigMapDlg)
						{
							pBigMapDlg->RemoveIcon(pFarmDlg->GetFarm()->GetID()+FARM_RENDER_OBJ_ID_START);
						}
					}
				}
			}
		}
		break;
	// 추방
	case MP_FAMILY_EXPEL:
		{
 			if (HERO == NULL) return;

			CSHFamily* pFamily = HERO->GetFamily() ;
			if( pFamily )							
			{
   				MSG_DWORD2* pstPacket = (MSG_DWORD2*)pMsg;
	
				switch(pstPacket->dwData2)
				{
				case RESULT_OK:
					{
						char szName[256];
						for(UINT i=0; i<HERO->GetFamily()->Get()->nMemberNum; i++)
						{
							if (HERO->GetFamily()->GetMember(i)->Get()->nID == pstPacket->dwData1)
							{
								SafeStrCpy(szName, HERO->GetFamily()->GetMember(i)->Get()->szName, MAX_NAME_LENGTH+1);
								break;
							}
						}

						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 1151 ), szName) ;

						// 멤버 삭제
						pFamily->DelMember(pstPacket->dwData1);
						CFamilyDialog* pDlg = GAMEIN->GetFamilyDlg() ;	
						if( pDlg ) pDlg->UpdateAllInfo() ;						

						// 추방된 당사자라면 패밀리 정보를 지운다.
						if (HERO->GetID() == pstPacket->dwData1)
						{
							CSHFamily csFamily;
							pFamily->Set(&csFamily);
							if( pDlg ) pDlg->SetActive(FALSE);

							// 농장 딜레이 타임 초기화
							CSHMonstermeterDlg* pDlg = GAMEIN->GetMonstermeterDlg() ;
							if (pDlg == NULL) return;
							pDlg->GetFarmRePlantTimeBar()->CLI_End();
							pDlg->GetFarmReManureTimeBar()->CLI_End();

							// 농장관리 창 닫기
							CSHFarmManageDlg* pFarmDlg = GAMEIN->GetFarmManageDlg();
							if (pFarmDlg)
							{
								pFarmDlg->SetActive(FALSE);
								// 미니맵의 농장 아이콘 제거
								CMiniMapDlg* pMiniMapDlg = GAMEIN->GetMiniMapDialog();
								if (pMiniMapDlg)
								{
									pMiniMapDlg->RemoveIcon(pFarmDlg->GetFarm()->GetID()+FARM_RENDER_OBJ_ID_START);
								}
								CBigMapDlg* pBigMapDlg = GAMEIN->GetBigMapDialog();
								if (pBigMapDlg)
								{
									pBigMapDlg->RemoveIcon(pFarmDlg->GetFarm()->GetID()+FARM_RENDER_OBJ_ID_START);
								}
							}
						}
					}
					break;
				case RESULT_FAIL_01:
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 1163 ));
					break;
				}
			}
		}
		break;
	// 해체
	case MP_FAMILY_BREAKUP:
		{
 			if (HERO == NULL) return;

			CSHFamily* pFamily = HERO->GetFamily() ;
			if( pFamily )							
			{
   				MSG_DWORD2* pstPacket = (MSG_DWORD2*)pMsg;
	
				switch(pstPacket->dwData2)
				{
				case RESULT_OK:
					{
						CSHFamily csFamily;
						pFamily->Set(&csFamily);

						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 1152 )) ;

						CFamilyDialog* pFamilyDlg = GAMEIN->GetFamilyDlg() ;	
						if( pFamilyDlg ) pFamilyDlg->SetActive(FALSE);

						// 농장 딜레이 타임 초기화
						CSHMonstermeterDlg* pDlg = GAMEIN->GetMonstermeterDlg() ;
						if (pDlg == NULL) return;
						pDlg->GetFarmRePlantTimeBar()->CLI_End();
						pDlg->GetFarmReManureTimeBar()->CLI_End();

						// 농장관리 창 닫기
						CSHFarmManageDlg* pFarmDlg = GAMEIN->GetFarmManageDlg();
						if (pFarmDlg)
						{
							pFarmDlg->SetActive(FALSE);
							// 미니맵의 농장 아이콘 제거
							CMiniMapDlg* pMiniMapDlg = GAMEIN->GetMiniMapDialog();
							if (pMiniMapDlg)
							{
								pMiniMapDlg->RemoveIcon(pFarmDlg->GetFarm()->GetID()+FARM_RENDER_OBJ_ID_START);
							}
							CBigMapDlg* pBigMapDlg = GAMEIN->GetBigMapDialog();
							if (pBigMapDlg)
							{
								pBigMapDlg->RemoveIcon(pFarmDlg->GetFarm()->GetID()+FARM_RENDER_OBJ_ID_START);
							}
						}
					}
					break;
				case RESULT_FAIL_01:
				case RESULT_FAIL_02:
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 1162 )) ;
					break;
				case RESULT_FAIL_03:
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 1161 )) ;
					break;
				}
			}
		}
		break;
	// 탈퇴 문장 삭제
	case MP_FAMILY_LEAVE_EMBLEM:
		{
 			if (HERO == NULL) return;

			MSG_DWORD* pstPacket = (MSG_DWORD*)pMsg;

			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pstPacket->dwData );
			if( !pPlayer ) return;

			cImageSelf* pEmblem = pPlayer->GetFamilyEmblem();
			if (pEmblem)
			{
				pEmblem->Release();
				delete pEmblem;
				pPlayer->SetFamilyEmblem(NULL);
			}

			char szName[MAX_NAME_LENGTH+1] = "";
			pPlayer->SetFamilyNickName(szName);
			pPlayer->GetObjectBalloon()->SetFamilyNickName(szName);
		}
		break;
	// 탈퇴,추방,해체 페널티 메세지
	case MP_FAMILY_PENALTY_MSG:
		{
 			if (HERO == NULL) return;

			MSG_DWORD3* pstPacket = (MSG_DWORD3*)pMsg;

			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pstPacket->dwObjectID );
			if( !pPlayer ) return;

			char szText[256];

			// 경험치 감소
   			switch(pstPacket->dwData1)
			{
			case 0:	strcpy(szText, CHATMGR->GetChatMsg( 1165 )); break;
			case 1: strcpy(szText, CHATMGR->GetChatMsg( 1166 )); break;
			case 2: strcpy(szText, CHATMGR->GetChatMsg( 1167 )); break;
			}
			// 레벨 감소
			if (pstPacket->dwData3)
			{
				strcat(szText, " ");
				strcat(szText, CHATMGR->GetChatMsg( 657 ));
			}

			if (pstPacket->dwData2)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, szText, pstPacket->dwData2);
			}
		}
	}
#endif
}


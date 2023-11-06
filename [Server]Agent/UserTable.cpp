// UserTable.cpp: implementation of the CUserTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UserTable.h"

#include "Network.h"
#ifdef _AGENTSERVER
#include "TrafficLog.h"
#include "GMPowerList.h"
#include "DateMatchingManager.h"
// desc_hseos_패밀리01
// S 패밀리 추가 added by hseos 2007.07.10
#include "../hseos/Family/SHFamilyManager.h"
// E 패밀리 추가 added by hseos 2007.07.10
#endif

// desc_hseos_주민등록_01
// S 주민등록 추가 added by hseos 2008.01.02
#include "../hseos/ResidentRegist/SHResidentRegistManager.h"
// E 주민등록 추가 added by hseos 2008.01.02

//---KES PUNISH
#include "PunishManager.h"
//-------------

// 080418 LYW --- UserTable : Include chatroom manager.
#include "./ChatRoomMgr.h"

#ifdef _NPROTECT_
#include "ggsrv25.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CUserTable * g_pUserTable = NULL;
CUserTable * g_pUserTableForUserID = NULL;
#include "AgentDBMsgParser.h"
CUserTable * g_pUserTableForObjectID = NULL;

extern int g_nServerSetNum;

CUserTable::CUserTable()
{
}

CUserTable::~CUserTable()
{
}

void CUserTable::Init(DWORD dwBucket,USERDisconnectProc pProc)
{
	CYHHashTable<USERINFO>::Initialize(dwBucket);
	m_MaxUserCount = 0;
	m_dwUserCount = 0;
	
	m_addCount = 0;
	m_removeCount = 0;

	m_pOnDisconnectProc	=	pProc;
}
USERINFO* CUserTable::FindUser(DWORD dwKey)
{
	return CYHHashTable<USERINFO>::GetData(dwKey);
}

BOOL CUserTable::AddUser(USERINFO* pObject,DWORD dwKey)
{
	//ASSERT(!FindUser(dwKey));
	++m_dwUserCount;

	++m_addCount;

	return CYHHashTable<USERINFO>::Add(pObject,dwKey);	
}

USERINFO * CUserTable::RemoveUser(DWORD dwKey)
{

	USERINFO * info = NULL;
	info = (USERINFO *)GetData(dwKey);

	if( !info )
		return NULL;

	// desc_hseos_패밀리01
	// S 패밀리 추가 added by hseos 2007.07.10
	// ..다른 에이전트에 알리기
	g_csFamilyManager.ASRV_SendMemberConToOtherAgent(info->pcsFamily, info->dwCharacterID, CSHFamilyMember::MEMBER_CONSTATE_LOGOFF);
	// ..패밀리 테이블에서 삭제
	g_csFamilyManager.DelFamilyToTbl(info->dwCharacterID, info->pcsFamily);
	// ..클라이언트에 알리기
	g_csFamilyManager.ASRV_SendInfoToClient(info->pcsFamily, 2);
	info->pcsFamily = NULL;
	// E 패밀리 추가 added by hseos 2007.07.10

	// desc_hseos_데이트매칭01
	// S 데이트매칭 추가 added by hseos 2007.11.20
	// ..데이트 매칭 채팅 중이었다면 상대에게 접속이 종료됨을 알린다.
	g_csResidentRegistManager.ASRV_EndDateMatchingChat(info);
	// E 데이트매칭 추가 added by hseos 2007.11.20

	// 080331 LYW --- UserTable : Add player to chatroom system.
//===CHATROOM 임시 블럭
	//CHATROOMMGR->DestroyPlayer_From_Lobby(info) ;

	if( m_pOnDisconnectProc)
			m_pOnDisconnectProc( info);				// 지워질때 콜백을 호출.

	Remove(dwKey);
	--m_dwUserCount;

	++m_removeCount;

	return info;
}

void CUserTable::RemoveAllUser()
{
//	SetPositionHead();
//	USERINFO * info = NULL;
//	while(info = (USERINFO *)GetData())
//	{
//		Remove(info->dwConnectionIndex);
//		info = NULL;
//	}
	m_dwUserCount = 0;
	CYHHashTable<USERINFO>::RemoveAll();
}

void CUserTable::SetCalcMaxCount(DWORD CurCount)
{
	if(m_MaxUserCount < CurCount)
	{
		m_MaxUserCount = CurCount;
	}
}

BOOL CUserTable::SendToUser(DWORD dwKey,DWORD dwUniqueConnectIdx,MSGBASE* pmsg,DWORD size)
{
	USERINFO* pUserInfo = FindUser(dwKey);
	if(pUserInfo == NULL)
		return FALSE;
	if(pUserInfo->dwUniqueConnectIdx != dwUniqueConnectIdx)
		return FALSE;

	g_Network.Send2User(pUserInfo->dwConnectionIndex,(char*)pmsg,size);
	return TRUE;
}

extern CMemoryPoolTempl<USERINFO> g_UserInfoPool;

#ifdef _NPROTECT_
extern CMemoryPoolTempl<CCSAuth2> g_CCSAuth2Pool;
#endif

BOOL CUserTable::OnDisconnectUser(DWORD dwKey)
{
	USERINFO* pInfo = g_pUserTable->FindUser( dwKey );
	if(pInfo == NULL) return FALSE;

	//---KES Network Process Fix 071114
	g_pUserTable->RemoveUser( dwKey );
	//----------------------------

	if( pInfo->UserLevel == eUSERLEVEL_GM )
	{
		 GMINFO->RemoveGMList( dwKey );
	}


//update Billing
	if(pInfo->dwCharacterID)
	{
		g_pUserTableForObjectID->RemoveUser(pInfo->dwCharacterID);

		SERVERINFO* pServerInfo = g_pServerTable->FindServerForConnectionIndex(pInfo->dwMapServerConnectionIndex);
		if( pServerInfo )
		{
			// 로그인 Character_ID정보 초기화
			MSG_DWORD msg;
			msg.Category	= MP_USERCONN;
			msg.Protocol	= MP_USERCONN_DISCONNECTED;
			msg.dwObjectID	= pInfo->dwCharacterID;
			msg.dwData		= pInfo->dwUserID;
			g_Network.Send2Server(pInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg));
		}
		else
		{
			if(pInfo->dwUserID)
				LoginCheckDelete(pInfo->dwUserID);		// LOGOUT시간을 업데이트 합니다.
		}

		// ChangeMapPoint 초기화
		SaveMapChangePointUpdate(pInfo->dwCharacterID, 0);
	}
	else
	{
		if(pInfo->dwUserID)
			LoginCheckDelete(pInfo->dwUserID);		// LOGOUT시간을 업데이트 합니다.
	}

	if(pInfo->dwUserID)
	{
		g_pUserTableForUserID->RemoveUser(pInfo->dwUserID);
#ifdef _NPROTECT_
		if( pInfo->m_pCSA )
		{
			pInfo->m_pCSA->Close();
			g_CCSAuth2Pool.Free(pInfo->m_pCSA);
		}
#endif
		//---KES PUNISH
		PUNISHMGR->RemovePunishUnitAll( pInfo->dwUserID );
		//---------------
	}

	//---KES Network Process Fix 071114
	memset( pInfo, 0, sizeof(USERINFO) );
	g_UserInfoPool.Free(pInfo);	
	//----------------------------

	//user count 계산==============================================================================
	SERVERINFO * myInfo = g_pServerTable->GetSelfServer();
	myInfo->wAgentUserCnt = (WORD)GetUserCount();

	MSG_WORD2 msg2;
	msg2.Category = MP_SERVER;
	msg2.Protocol = MP_SERVER_USERCNT;
	msg2.wData1 = myInfo->wPortForServer;
	msg2.wData2 = myInfo->wAgentUserCnt;

	
	g_pServerTable->SetPositionHead();
	SERVERINFO* pOtherAgentInfo = NULL;
	while(pOtherAgentInfo = g_pServerTable->GetNextDistServer())
	{
		if(myInfo == pOtherAgentInfo)
			continue;

		g_Network.Send2Server(pOtherAgentInfo->dwConnectionIndex, (char *)&msg2, sizeof(msg2));
	}
	//user count 계산==============================================================================

	return TRUE;
}

BOOL DisconnectUser(DWORD dwConnectionIndex)
{
	g_Network.DisconnectUser(dwConnectionIndex);

	return TRUE;
}


// CUserTable중 ObjID쪽에 쓰일 콜백.
void	__stdcall OnRemoveObjProc( USERINFO* pInfo)
{
	if( pInfo)
		DATEMATCHINGMGR->RemoveUserInfo( pInfo);
	else
		_asm int 3;			// pInfo가 null
}

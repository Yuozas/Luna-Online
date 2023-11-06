// UserTable.cpp: implementation of the CUserTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UserTable.h"


#ifdef __MAPSERVER__
#ifndef _MURIMNET_
#include "CharMove.h"
#include "Player.h"
#endif
#endif

#include "Network.h"
#ifdef _AGENTSERVER
#include "TrafficLog.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CUserTable * g_pUserTable = NULL;
CUserTable * g_pUserTableForUserID = NULL;
#ifdef __AGENTSERVER__
#include "AgentDBMsgParser.h"
CUserTable * g_pUserTableForObjectID = NULL;
#endif


CUserTable::CUserTable()
{
}

CUserTable::~CUserTable()
{
}

#ifndef __MAPSERVER__
void CUserTable::Init(DWORD dwBucket)
{
	CYHHashTable<USERINFO>::Initialize(dwBucket);
	m_MaxUserCount = 0;
	m_dwUserCount = 0;
	
	m_addCount = 0;
	m_removeCount = 0;
}
USERINFO* CUserTable::FindUser(DWORD dwKey)
{
	return CYHHashTable<USERINFO>::GetData(dwKey);
}

BOOL CUserTable::AddUser(USERINFO* pObject,DWORD dwKey)
{
	ASSERT(!FindUser(dwKey));
	++m_dwUserCount;

	++ m_addCount;

	return CYHHashTable<USERINFO>::Add(pObject,dwKey);	
}

USERINFO * CUserTable::RemoveUser(DWORD dwKey)
{
	USERINFO * info = NULL;
	info = (USERINFO *)GetData(dwKey);
	Remove(dwKey);

	if( info == NULL ) return NULL;

	--m_dwUserCount;

	++m_removeCount;

	return info;
}

void CUserTable::RemoveAllUser()
{
/*
//	SetPositionHead();
//	USERINFO * info = NULL;
//	while(info = (USERINFO *)GetData())
//	{
//		Remove(info->dwConnectionIndex);
//		info = NULL;
//	}
*///�ּ�ó��.
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

#else // __MAPSERVER__ �϶� ����
#ifndef _MURIMNET_
#include "ObjectFactory.h"
#endif
void CUserTable::Init(DWORD dwBucket)
{
	CYHHashTable<CObject>::Initialize(dwBucket);
	m_dwUserCount = 0;
	m_dwMonsterCount = 0;
	m_dwNpcCount = 0;
	m_dwExtraCount = 0;
}
CObject* CUserTable::FindUser(DWORD dwKey)
{
	return CYHHashTable<CObject>::GetData(dwKey);
}

//ĳ���� �̸����� ã��. ������.
CObject* CUserTable::FindUserForName( char* strCharacterName )
{
	SetPositionHead();
	CObject* pObject = NULL;

	char buf1[MAX_NAME_LENGTH+1];
	char buf2[MAX_NAME_LENGTH+1];

	SafeStrCpy( buf1, strCharacterName, MAX_NAME_LENGTH+1 );
	strupr( buf1 );

	while( pObject = (CObject*)GetData() )
	{
		SafeStrCpy( buf2, pObject->GetObjectName(), MAX_NAME_LENGTH+1 );
		strupr( buf2 );

		if( strncmp( buf2, buf1, MAX_NAME_LENGTH ) == 0 )
			return pObject;
	}

	return NULL;
}

BOOL CUserTable::AddUser(CObject* pObject,DWORD dwKey)
{
	switch(pObject->GetObjectKind())
	{
	case eObjectKind_Player:
		{
			++m_dwUserCount;
		}
		break;
	case eObjectKind_Monster:
		{
			++m_dwMonsterCount;
		}
		break;
	case eObjectKind_BossMonster:
		{
			++m_dwMonsterCount;
		}
		break;
	case eObjectKind_SpecialMonster:
		{
			++m_dwMonsterCount;
		}
		break;
	case eObjectKind_ToghterPlayMonster:
		{
			++m_dwMonsterCount;
		}
		break;
	case eObjectKind_Npc:
		{
			++m_dwNpcCount;
		}
		break;
	default:
		{
			++m_dwExtraCount;
		}
		break;
	}
	return CYHHashTable<CObject>::Add(pObject,dwKey);	
}

CObject * CUserTable::RemoveUser(DWORD dwKey)
{
	CObject * info = NULL;
	info = (CObject *)GetData(dwKey);
	Remove(dwKey);
	
	if( info == NULL ) return NULL;

	switch(info->GetObjectKind())
	{
	case eObjectKind_Player:
		{
			--m_dwUserCount;
		}
		break;
	case eObjectKind_Monster:
		{
			--m_dwMonsterCount;
		}
		break;
	case eObjectKind_Npc:
		{
			--m_dwNpcCount;
		}
		break;
	default:
		{
			--m_dwExtraCount;
		}
		break;
	}
	return info;
}

void CUserTable::RemoveAllUser()
{
	SetPositionHead();
	CObject * info = NULL;
	while(info = (CObject *)GetData())
	{
//		Remove(info->dwConnectionIndex);
#ifndef _MURIMNET_
		g_pObjectFactory->ReleaseObject(info);
#endif
	}

	m_dwUserCount = 0;
	m_dwMonsterCount = 0;
	m_dwNpcCount = 0;
	m_dwExtraCount = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// 06. 08. 2�� ���� - �̿���
/// ���� GetTargetInRange �Լ��� ������ ���ڷ� �����Ÿ� �߰�
void CUserTable::GetTargetInRange(VECTOR3* pPivotPos,float Radius,CTargetList* pTList,DWORD GridID, float SafeRange)
{
#ifndef _MURIMNET_
	pTList->Clear();
	CTargetListIterator iter(pTList);

	CObject* pObject;
	VECTOR3* pObjectPos;
	float dist;
	float dx,dz;

	SetPositionHead();
	while(pObject = GetData())
	{
		if(	pObject->GetObjectKind() == eObjectKind_Player )
		{
			// 080604 KTH -- �׸��� üũ �κ� �߰�
			if(pObject->GetState() == eObjectState_Die || pObject->GetState() == eObjectState_Immortal || pObject->GetGridID() != GridID)
				continue;
			
			pObjectPos = CCharMove::GetPosition(pObject); 
			dx = (pPivotPos->x - pObjectPos->x);
			dz = (pPivotPos->z - pObjectPos->z);
			dist = sqrtf(dx*dx + dz*dz) - pObject->GetRadius();
			/// �����Ÿ� �����̰� �����Ÿ� �ٱ��ʸ� Ÿ���̴�
			if(dist <= Radius && dist >= SafeRange)
			{
				iter.AddTargetWithNoData(pObject->GetID(),SKILLRESULTKIND_NONE);
			}
		}
	}
	iter.Release();
#endif
}

// 080708 LUJ, Ư�� ������Ʈ ������ Ÿ������ ��´�
void CUserTable::GetTargetInRange(VECTOR3* pPivotPos,float Radius,CTargetList* pTList,DWORD GridID, float SafeRange, const CUserTable::ObjectKindSet& objectKindSet )
{
	CTargetListIterator iter( pTList );
	CObject* pObject = 0;

	SetPositionHead();
	while( pObject = GetData() )
	{
		if(	objectKindSet.end()		== objectKindSet.find( EObjectKind( pObject->GetObjectKind() ) )	||
			pObject->GetState()		== eObjectState_Die													||
			pObject->GetState()		== eObjectState_Immortal											||
			pObject->GetGridID()	!= GridID )
		{
			continue;
		}

		const VECTOR3*	pObjectPos	= CCharMove::GetPosition( pObject ); 
		const float		dx			= ( pPivotPos->x - pObjectPos->x );
		const float		dz			= ( pPivotPos->z - pObjectPos->z );
		const float		dist		= sqrtf( dx * dx + dz * dz );

		if( dist <= Radius && dist >= SafeRange )
		{
			iter.AddTargetWithNoData( pObject->GetID(), SKILLRESULTKIND_NONE );
		}
	}

	iter.Release();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void CUserTable::GetTargetInArea(CSkillArea* pSkillArea,CTargetList* pTList)
{
#ifndef _MURIMNET_
	pTList->Clear();
	CTargetListIterator iter(pTList);

	CObject* pObject;
	VECTOR3* pObjectPos;
	
	SetPositionHead();
	while(pObject = GetData())
	{
		if(	pObject->GetObjectKind() == eObjectKind_Player )
		{
			if(pObject->GetState() == eObjectState_Die || pObject->GetState() == eObjectState_Immortal)
				continue;
			
			pObjectPos = CCharMove::GetPosition(pObject); 
//			if(pSkillArea->IsInDamageTile(pObjectPos) == TRUE)
			{
//				iter.AddTargetWithNoData(pObject->GetID(),SKILLRESULTKIND_NONE);
			}
		}
	}
	iter.Release();
#endif
}

void CUserTable::GetTargetInDonutRangeInChannel(DWORD MonGridID, VECTOR3* pPivotPos,float MaxRadius, float MinRadius, CObject* rtpObject)
{
#ifndef _MURIMNET_
	
	CObject* pObject;
	VECTOR3* pObjectPos;
	float dist;
	float dx,dz;

	SetPositionHead();
	while(pObject = GetData())
	{
		if(	pObject->GetObjectKind() == eObjectKind_Player )
		{
			if(pObject->GetState() == eObjectState_Die || pObject->GetState() == eObjectState_Immortal)
				continue;
			if(pObject->GetGridID() != MonGridID)
				continue;
			if( !((CPlayer*)pObject)->IsVisible() )
				continue;

			pObjectPos = CCharMove::GetPosition(pObject); 
			dx = (pPivotPos->x - pObjectPos->x);
			dz = (pPivotPos->z - pObjectPos->z);
			dist = sqrtf(dx*dx + dz*dz) - pObject->GetRadius();
			if((dist <= MaxRadius) && (dist >= MinRadius))
			{
				rtpObject = pObject;
				break;
			}
		}
	}
#endif
}

#endif

#ifndef _MAPSERVER_
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

#ifdef _AGENTSERVER
BOOL CUserTable::OnDisconnectUser(DWORD dwKey)
{
	USERINFO * pInfo = g_pUserTable->FindUser(dwKey);

	if(pInfo == NULL)
		return FALSE;

#ifdef _NETWORKTEST
	TRAFFIC->LogUser(pInfo);
#endif

	LoginCheckDelete(pInfo->dwUserID);		// LOGOUT�ð��� ������Ʈ �մϴ�.
	
	if(pInfo->dwCharacterID)
		g_pUserTableForObjectID->RemoveUser(pInfo->dwCharacterID);
	
	// �α��� UserID�Ǿ����ִ� �� DB�ʵ忡�� ����
	ASSERT(pInfo->dwUserID);
	g_pUserTableForUserID->RemoveUser(pInfo->dwUserID);

	if(pInfo->dwCharacterID)
	{
		// �α��� Character_ID���� �ʱ�ȭ
		MSGBASE msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_DISCONNECTED;
		msg.dwObjectID = pInfo->dwCharacterID;
		g_Network.Send2Server(pInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg));

		// ChangeMapPoint �ʱ�ȭ
		SaveMapChangePointUpdate(pInfo->dwCharacterID, 0);
	}

	// ���� ����
	g_pUserTable->RemoveUser(dwKey);
	
	//user count ���==============================================================================
	SERVERINFO * myInfo = g_pServerTable->GetSelfServer();
	myInfo->wAgentUserCnt = GetUserCount();

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
	//user count ���==============================================================================

	g_Console.LOG(4, "  User Disconnected UserIDx:%d  CharID:%d  (%d)", 
			pInfo->dwUserID, pInfo->dwCharacterID, GetUserCount());

	// AuthKey Release
	g_pServerSystem->ReleaseAuthKey(pInfo->dwUniqueConnectIdx);
	// pool ����
	g_UserInfoPool.Free(pInfo);

	return TRUE;
}
#else
BOOL CUserTable::OnDisconnectUser(DWORD dwKey)
{
	USERINFO* pUserInfo = FindUser(dwKey);
	ASSERT(pUserInfo);

	if( pUserInfo->dwUserID != 0 &&
		pUserInfo->bRecvDistOut == FALSE)
	{
		// Send Msg To Agent 
		MSG_DWORD msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_DISCONNECTED_ON_LOGIN;
		msg.dwObjectID = pUserInfo->dwUserID;
		msg.dwData = pUserInfo->dwUniqueConnectIdx;
		g_Network.Broadcast2AgentServer((char*)&msg,sizeof(msg));
	}
	g_pServerSystem->ReleaseAuthKey(pUserInfo->dwUniqueConnectIdx);
	RemoveUser(dwKey);
	g_UserInfoPool.Free(pUserInfo);
	
	g_Console.LOG(4, "  User Disconnected UserIDx:%d  CharID:%d  (%d)", 
			pUserInfo->dwUserID, pUserInfo->dwCharacterID, GetUserCount());

	return TRUE;
}
#endif

BOOL DisconnectUser(DWORD dwConnectionIndex)
{
	g_Console.LOG(4, "CompulsiveDisconnect   ConnectionIndex : %d",dwConnectionIndex);
	USERINFO* pUserInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if(pUserInfo)
	{
		g_Console.LOG(4, "  User CompulsiveDisconnected UserIDx:%d  CharID:%d  (%d)", 
			pUserInfo->dwUserID, pUserInfo->dwCharacterID, g_pUserTable->GetUserCount());	//
	}

	g_Network.DisconnectUser(dwConnectionIndex);

	return TRUE;
}

BOOL DisconnectUser(DWORD dwConnectionIndex, int flag )
{
	g_Console.LOG(4, "CompulsiveDisconnect   ConnectionIndex : %d",dwConnectionIndex);
	USERINFO* pUserInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if(pUserInfo)
	{
		g_Console.LOG(4, "  User CompulsiveDisconnected UserIDx:%d  CharID:%d  (%d)", 
			pUserInfo->dwUserID, pUserInfo->dwCharacterID, g_pUserTable->GetUserCount());	//
	}

	g_Network.DisconnectUser(dwConnectionIndex);

	switch( flag )
	{
	case 1:		// connection check
		g_Console.LOG(4, "DisconnectUser : Connection Check", dwConnectionIndex );
		break;
	case 2:		// mapserver down
		g_Console.LOG(4, "DisconnectUser : Map Down", dwConnectionIndex );
		break;
	case 3:		// GMTool Ban
		g_Console.LOG(4, "DisconnectUser : GMTool Ban", dwConnectionIndex );
		break;
	case 4:		// Overlapped Login
		g_Console.LOG(4, "DisconnectUser : Overlapped Login", dwConnectionIndex );
		break;
	case 5:		// MP_USERCONN_DISCONNECT_SYN
		g_Console.LOG(4, "DisconnectUser : MP_USERCONN_DISCONNECT_SYN", dwConnectionIndex );
		break;
	case 6:		// MP_CHEAT_BANCHARACTER_SYN
		g_Console.LOG(4, "DisconnectUser : MP_CHEAT_BANCHARACTER_SYN", dwConnectionIndex );
		break;
	case 7:		// MP_HACKCHECK_SPEEDHACK
		g_Console.LOG(4, "DisconnectUser : MP_HACKCHECK_SPEEDHACK", dwConnectionIndex );
		break;
	}

	return TRUE;
}

#endif
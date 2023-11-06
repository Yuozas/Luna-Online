// ServerSystem.h: interface for the CServerSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERSYSTEM_H__FD3EBFC3_EE3D_4505_A5A1_24DA471D20AB__INCLUDED_)
#define AFX_SERVERSYSTEM_H__FD3EBFC3_EE3D_4505_A5A1_24DA471D20AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "MHMap.h"
//#include "PtrList.h"
class CObject;
class CGridSystem;

class CPlayer;
class CMonster;
class CNpc;
class cSkillObject;
class CBossMonster;
class CMapObject;
class CPet;

// 필드보스 - 05.12 이영준
class CFieldBossMonster;
class CFieldSubMonster;

enum eNATION
{
	eNATION_KOREA,
	eNATION_CHINA,
};

enum{ePET_FROM_DB, ePET_FROM_ITEM};		//DB로부터 펫생성, 소환아이템 첫사용으로 팻생성

struct WAIT_EXIT_PLAYER_INFO
{
	DWORD dwAgentConnectionIndex;
	DWORD dwPlayerID;
	DWORD dwStartTime;
};


class CServerSystem  
{
//	CYHHashTable<CItemObject> m_ItemObjectTable;
	CGridSystem* m_pGridSystem;
	CMHMap m_Map;
	
	friend class CCharMove;
	// 080813 LUJ, 수정된 inetwork 호출 형식에 따라 변경
	friend void __stdcall GameProcess(DWORD dwEventIndex);

	BOOL m_start;
//	CIndexGenerator m_objectIdxGen;
	
	WORD	m_wMapNum;
	
//국가
	int		m_Nation;

	BOOL			m_bTestServer;

	cPtrList									m_listWaitExitPlayer;
	CMemoryPoolTempl<WAIT_EXIT_PLAYER_INFO>*	m_mpWaitExitPlayer;
	
	DWORD	m_dwQuestTime;
	BOOL	m_bQuestTime;

	BOOL	m_bCompletionChrUpdate;
//--check process time
	DWORD	m_dwMainProcessTime;


public:
	DWORD GetMainProcessTime() { return m_dwMainProcessTime; }

public:

	CServerSystem();
	virtual ~CServerSystem();

	void AddWaitExitPlayer( DWORD dwAgentConnetionIndex, DWORD dwPlayerID );
	void ProcessWaitExitPlayer();
//국가
	void SetNation();
	int GetNation() { return m_Nation; }

	BOOL IsTestServer() { return m_bTestServer; }

	void _5minProcess();
	void Process();
	void Start(WORD ServerNum);
	void End();		

	CMHMap* GetMap()	{ return &m_Map;	}
	WORD	GetMapNum()	{ return m_wMapNum;	}
	
	BOOL GetStart()		{ return m_start; 	}
	void SetStart(BOOL state);
	/*
	CObject* GetObject(DWORD dwObjectID);
	CPlayer* GetPlayer(DWORD dwPlayerID);
	CMonster* GetMonster(DWORD dwMonsterID);
	CNpc* GetNpc(DWORD dwNpcID);
	*/
//	CItemObject* GetItemObject(DWORD dwItemID);
	
	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.28
	// CPlayer* AddPlayer(DWORD dwPlayerID, DWORD dwAgentNum,DWORD UniqueIDinAgent,int ChannelNum);
	CPlayer* AddPlayer(DWORD dwPlayerID, DWORD dwAgentNum,DWORD UniqueIDinAgent,int ChannelNum, UINT nChallengeZoneSection = 0);
	// E 데이트 존 추가 added by hseos 2007.11.28

	CPlayer* InitPlayerInfo(BASEOBJECT_INFO* pBaseObjectInfo,CHARACTER_TOTALINFO* pTotalInfo,HERO_TOTALINFO* pHeroInfo);

	void CreateNewPet(CPlayer* pMaster, DWORD dwItemIdx, DWORD SummonItemDBIdx, WORD wPetKind, WORD PetGrade = 1);
	CMonster* AddMonster(DWORD dwSubID, BASEOBJECT_INFO* pBaseObjectInfo,MONSTER_TOTALINFO* pTotalInfo,VECTOR3* pPos,WORD wObjectKind = 32);
	CBossMonster* AddBossMonster(DWORD dwSubID, BASEOBJECT_INFO* pBaseObjectInfo,MONSTER_TOTALINFO* pTotalInfo,VECTOR3* pPos);
	
	// 필드보스 - 05.12 이영준
	CFieldBossMonster* AddFieldBossMonster(DWORD dwSubID, BASEOBJECT_INFO* pBaseObjectInfo,MONSTER_TOTALINFO* pTotalInfo,VECTOR3* pPos);
	CFieldSubMonster* AddFieldSubMonster(DWORD dwSubID, BASEOBJECT_INFO* pBaseObjectInfo,MONSTER_TOTALINFO* pTotalInfo,VECTOR3* pPos);
	
	CNpc* AddNpc(BASEOBJECT_INFO* pBaseObjectInfo,NPC_TOTALINFO* pTotalInfo,VECTOR3* pPos);
	cSkillObject* AddSkillObject(cSkillObject* pSkillObj,VECTOR3* pPos);
	CMapObject* AddMapObject(DWORD Kind, BASEOBJECT_INFO* pBaseObjectInfo, MAPOBJECT_INFO* pMOInfo, VECTOR3* pPos);
	CPet* AddPet(BASEOBJECT_INFO* pBaseObjectInfo, PET_OBJECT_INFO* pPetObjectInfo, VECTOR3* pPos, CPlayer* pPlayer);

	void RemovePlayer(DWORD dwPlayerID, BOOL bRemoveFromUserTable = TRUE);
	void RemovePet(DWORD dwPetObjectID);
	void RemoveMonster(DWORD dwPlayerID);
	void RemoveBossMonster(DWORD dwMonster);
	void RemoveNpc(DWORD dwPlayerID);
	void RemoveItemObject(DWORD ItemID);
	void RemoveSkillObject(DWORD SkillObjectID);
	void RemoveMapObject( DWORD MapObjID );

	
	inline CGridSystem* GetGridSystem()	{	return m_pGridSystem;	}

	void SendToOne(CObject* pObject,void* pMsg,int MsgLen);
	
	void ReloadResourceData();
	
	//////////////////////////////////////////////////////////////////////////
	// Npc AI관련 함수들
	//void MonsterStateProc();	

	void	SetCharUpdateCompletion(BOOL bVal)	{ m_bCompletionChrUpdate = bVal;	}
	BOOL	GetCharUpdateCompletion()	{	return m_bCompletionChrUpdate;	}
	void	RemoveServerForKind( WORD wSrvKind );
	void	HandlingBeforeServerEND();

	void	LoadHackCheck();
};

// 080813 LUJ, 수정된 inetwork 호출 형식에 따라 변경
void __stdcall GameProcess(DWORD dwEventIndex);
void __stdcall OnConnectServerSuccess(DWORD dwIndex, void* pVoid);
void __stdcall OnConnectServerFail(void* pVoid);

void _5minGameProcess();
extern CServerSystem * g_pServerSystem;
BOOL LoadEventRate(char* strFileName);


#endif // !defined(AFX_SERVERSYSTEM_H__FD3EBFC3_EE3D_4505_A5A1_24DA471D20AB__INCLUDED_)

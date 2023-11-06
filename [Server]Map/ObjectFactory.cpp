// ObjectFactory.cpp: implementation of the CObjectFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "Stdafx.h"
#include "ObjectFactory.h"
#include "Player.h"
#include "BossMonster.h"
#include "Npc.h"
#include "MapObject.h"
#include "MemoryChecker.h"
// �ʵ庸�� - 05.12 �̿���
#include "FieldBossMonster.h"
#include "FieldSubMonster.h"
// 080616 LUJ, ���� ������� ����
#include "Trap.h"
#include "Pet.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CObjectFactory * g_pObjectFactory=NULL;

int count = 0;

CObjectFactory::CObjectFactory()
{
	PlayerPool = NULL;
	MonsterPool = NULL;
	NpcPool = NULL;
	BossMonsterPool = NULL;
	MapObjectPool = NULL;
	PetPool = NULL;

	// �ʵ庸�� - 05.12 �̿���
	FieldBossMonsterPool = NULL;
	FieldSubMonsterPool = NULL;
	// 080616 LUJ, ���� �߰�
	TrapPool = NULL;
}

CObjectFactory::~CObjectFactory()
{
	Release();
}

void CObjectFactory::Init()
{	
	PlayerPool = new	CMemoryPoolTempl<CPlayer>;
	MonsterPool = new	CMemoryPoolTempl<CMonster>;
	NpcPool = new		CMemoryPoolTempl<CNpc>;
	BossMonsterPool = new 	CMemoryPoolTempl<CBossMonster>;
	MapObjectPool = new CMemoryPoolTempl<CMapObject>;
	// 080616 LUJ, ���� �߰�
	TrapPool	= new CMemoryPoolTempl<CTrap>;
	PetPool = new CMemoryPoolTempl<CPet>;


	// �ʵ庸�� - 05.12 �̿���
	FieldBossMonsterPool = new CMemoryPoolTempl<CFieldBossMonster>;
	FieldSubMonsterPool = new CMemoryPoolTempl<CFieldSubMonster>;

	PlayerPool->Init(MAX_TOTAL_PLAYER_NUM,100,"PlayerPool");
	MonsterPool->Init(MAX_TOTAL_MONSTER_NUM,1000,"MonsterPool");
	BossMonsterPool->Init(MAX_TOTAL_BOSSMONSTER_NUM, 5, "BossMonsterPool");

	// �ʵ庸�� - 05.12 �̿���
	FieldBossMonsterPool->Init(MAX_TOTAL_BOSSMONSTER_NUM, 5, "FieldBossMonsterPool");
	FieldSubMonsterPool->Init(MAX_TOTAL_BOSSMONSTER_NUM * 10, 5, "FieldSubMonsterPool");
	
	NpcPool->Init(MAX_TOTAL_NPC_NUM,10,"NpcPool");
	MapObjectPool->Init( MAX_MAPOBJECT_NUM, 10, "MapObjectPool" );
	// 080616 LUJ, ���� �߰�
	TrapPool->Init( MAX_MAPOBJECT_NUM, 10, "TrapPool" );
	PetPool->Init(MAX_TOTAL_PET_NUM,50,"PetPool");

}

void CObjectFactory::Release()
{
	SAFE_DELETE(PlayerPool);
	SAFE_DELETE(MonsterPool);
	SAFE_DELETE(NpcPool);
	SAFE_DELETE(BossMonsterPool);
	
	// �ʵ庸�� - 05.12 �̿���
	SAFE_DELETE(FieldBossMonsterPool);
	SAFE_DELETE(FieldSubMonsterPool);

	SAFE_DELETE(MapObjectPool);
	// 080616 LUJ, ���� �߰�
	SAFE_DELETE(TrapPool);
	SAFE_DELETE(PetPool);//PetPool = new CMemoryPoolTempl<CPet>;

}

CObject* CObjectFactory::MakeNewObject(EObjectKind Kind,DWORD AgentNum, BASEOBJECT_INFO* pBaseObjectInfo)
{
	CObject* pObject;
	switch(Kind)
	{
	case eObjectKind_Player:
		// ��ü�� �����ؼ� ���Ͻ����ش�
		pObject = (CObject*)PlayerPool->Alloc();
		break;

    case eObjectKind_ToghterPlayMonster:
	case eObjectKind_SpecialMonster:
	case eObjectKind_Monster:
		{
			// ��ü�� �����ؼ� ���Ͻ����ش�
			pObject = (CObject*)MonsterPool->Alloc();
/*			
			// Memory Check
			stMemoryInfo* pMemoryInfo = MEMORYCHK->GetMemoryInfo( eObjectFactory_Monster );
			if( pMemoryInfo )
				pMemoryInfo->Increase( sizeof(CMonster) );
			//
*/
		}
		break;

	case eObjectKind_Npc:
		// ��ü�� �����ؼ� ���Ͻ����ش�
		pObject = (CObject*)NpcPool->Alloc();
		break;

	case eObjectKind_BossMonster:
		// ��ü�� �����ؼ� ���Ͻ����ش�
		pObject = (CObject*)BossMonsterPool->Alloc();
		break;	
	
	// �ʵ庸�� - 05.12 �̿���
	case eObjectKind_FieldBossMonster:
		pObject = (CObject*)FieldBossMonsterPool->Alloc();
		break;	
	case eObjectKind_FieldSubMonster:
		pObject = (CObject*)FieldSubMonsterPool->Alloc();
		break;

	case eObjectKind_MapObject:
	case eObjectKind_CastleGate:
		pObject = (CObject*)MapObjectPool->Alloc();
		break;

		// 080616 LUJ, ���� �߰�
	case eObjectKind_Trap:
		pObject = TrapPool->Alloc();
		break;
	case eObjectKind_Pet:
		{
			pObject = (CObject*)PetPool->Alloc();
			
			char log[256];
			sprintf( log, "./log/pet%02d.txt", g_pServerSystem->GetMapNum() );
			FILE* fp = fopen( log, "a+" );
			
			if( fp )
			{
				fprintf( fp, "ALLOC:\t%d\t%d\n", pObject, ++count );
				fclose( fp );
			}
		}
		break;
	}
	
	ASSERT(pObject);
	
	if(pObject->Init(Kind,AgentNum,pBaseObjectInfo) == FALSE)
	{
		ReleaseObject(pObject);
		return FALSE;
	}
	
	return pObject;
}

void CObjectFactory::ReleaseObject(CObject* pObject)
{
	BYTE Kind = pObject->GetObjectKind();
	pObject->Release();
	switch(Kind)
	{
	case eObjectKind_Player:
		PlayerPool->Free((CPlayer*)pObject);
		break;

	case eObjectKind_ToghterPlayMonster:
	case eObjectKind_SpecialMonster:
	case eObjectKind_Monster:
		{
			MonsterPool->Free((CMonster*)pObject);
/*
			// Memory Check
			stMemoryInfo* pMemoryInfo = MEMORYCHK->GetMemoryInfo( eObjectFactory_Monster );
			if( pMemoryInfo )
				pMemoryInfo->Decrease( sizeof(CMonster) );
			//
*/
		}
		
		break;

	case eObjectKind_Npc:
		NpcPool->Free((CNpc*)pObject);
		break;

	case eObjectKind_BossMonster:
		BossMonsterPool->Free((CBossMonster*)pObject);
		break;
	
	// �ʵ庸�� - 05.12 �̿���
	case eObjectKind_FieldBossMonster:
		FieldBossMonsterPool->Free((CFieldBossMonster*)pObject);
		break;
	case eObjectKind_FieldSubMonster:
		FieldSubMonsterPool->Free((CFieldSubMonster*)pObject);
		break;
		
	case eObjectKind_MapObject:
	case eObjectKind_CastleGate:
		MapObjectPool->Free((CMapObject*)pObject);
		break;

		// 080616 LUJ, ���� �߰�
	case eObjectKind_Trap:
		TrapPool->Free( ( CTrap* )pObject );
	case eObjectKind_Pet:
		{
			char log[256];
			sprintf( log, "./log/pet%02d.txt", g_pServerSystem->GetMapNum() );
			FILE* fp = fopen( log, "a+" );
			
			if( fp )
			{
				fprintf( fp, "FREE:\t%d\t%d\n", pObject, --count );
				fclose( fp );
			}
			
			PetPool->Free((CPet*)pObject);
		}
		break;
	}
}

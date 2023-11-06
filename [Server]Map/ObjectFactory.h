// ObjectFactory.h: interface for the CObjectFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTFACTORY_H__324EFDDF_059A_428C_94F2_76BC8E75E765__INCLUDED_)
#define AFX_OBJECTFACTORY_H__324EFDDF_059A_428C_94F2_76BC8E75E765__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object.h"
//class CObject;
class CPlayer;
class Pet;
class CMonster;
class CNpc;
class CBossMonster;
class CMapObject;
// 080616 LUJ, 함정 추가
class CTrap;

// 필드보스 - 05.12 이영준
class CFieldBossMonster;
class CFieldSubMonster;
class CPet;

class CObjectFactory
{
private:
	CMemoryPoolTempl<CPlayer>* PlayerPool;
	CMemoryPoolTempl<CMonster>* MonsterPool;	
	CMemoryPoolTempl<CNpc>* NpcPool;	
	CMemoryPoolTempl<CBossMonster>* BossMonsterPool;
	CMemoryPoolTempl<CPet>* PetPool;
	
	// 필드보스 - 05.12 이영준
	CMemoryPoolTempl<CFieldBossMonster>* FieldBossMonsterPool;
	CMemoryPoolTempl<CFieldSubMonster>* FieldSubMonsterPool;
	
	CMemoryPoolTempl<CMapObject>*	MapObjectPool;
	// 080616 LUJ, 함정 추가
	CMemoryPoolTempl<CTrap>* TrapPool;

public:
	CObjectFactory();
	virtual ~CObjectFactory();
	
	void Init();
	void Release();

	CObject* MakeNewObject(EObjectKind Kind,DWORD AgentNum, BASEOBJECT_INFO* pBaseObjectInfo);
	void ReleaseObject(CObject* pObject);
};
extern CObjectFactory * g_pObjectFactory; 
#endif // !defined(AFX_OBJECTFACTORY_H__324EFDDF_059A_428C_94F2_76BC8E75E765__INCLUDED_)

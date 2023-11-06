#pragma once
#include "distributeway.h"

#define DISTRIBUTEROTATION	CDistribute_Rotation::GetInstance()

class CParty ;

class CDistribute_Rotation :
	public CDistributeWay
{
protected : 
	CDistribute_Rotation(void);
public:
	GETINSTANCE(CDistribute_Rotation) ;
	virtual ~CDistribute_Rotation(void);

	virtual void SendItem(PARTY_RECEIVE_MEMBER* pRealMember, DWORD DropItemId, DWORD DropItemRatio, MONSTEREX_LIST * pMonInfo, WORD MonsterKind, LEVELTYPE MaxLevel);
};

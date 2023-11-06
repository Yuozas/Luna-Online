// Distribute_Random.cpp: implementation of the CDistribute_Random class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Distribute_Random.h"
#include "ItemDrop.h"
#include "ItemManager.h"
#include "Player.h"
#include "Monster.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDistribute_Random::CDistribute_Random()
{

}

CDistribute_Random::~CDistribute_Random()
{

}
/*
void CDistribute_Random::SendAbil(LEVELTYPE MonsterLevel, PARTY_RECEIVE_MEMBER* pRealMember)
{
	if(pRealMember->count == 0)
		return;

	int rate = rand() % pRealMember->count;
	SendToPersonalAbil((CPlayer*)pRealMember->pPlayer[rate], MonsterLevel);
}
*/
void CDistribute_Random::SendItem( PARTY_RECEIVE_MEMBER* pRealMember, DWORD DropItemId, DWORD DropItemRatio, MONSTEREX_LIST * pMonInfo, WORD MonsterKind, LEVELTYPE MaxLevel )
{
	//if( pRealMember->count == 0 )	return;
	//if(ChkGetItemLevel(MaxLevel, pMonInfo->Level) == FALSE)	return;

	//int rate = rand() % pRealMember->count;
	//CPlayer* pReceivePlayer = (CPlayer*)pRealMember->pPlayer[rate];
	//if( !pReceivePlayer )	return;	

	//if(DropItemId == 0)
	//{
	//	MONEYTYPE money;
	//	MONEYTYPE partymoney;

	//	money = ITEMDROP_OBJ->MoneyItemNoItemPercentCalculator(pMonInfo, pReceivePlayer, MonsterKind); //to do : 여기서 아이템도 드랍 하는지 확인
	//	if(money)
	//	{
	//		partymoney = money/pRealMember->count;
	//		for(int i=0; i< pRealMember->count; ++i)
	//		{
	//			SendToPersonalMoney((CPlayer*)pRealMember->pPlayer[i], partymoney, MonsterKind);
	//		}
	//		return;
	//	}
	//}
	//else
	//{			
	//	if( DropItemRatio )
	//	if( ( rand()%100 ) % ( 100 / DropItemRatio ) == 0 )
	//		ITEMMGR->MonsterObtainItem(pReceivePlayer,DropItemId,MonsterKind);
	//}
}
/*
void CDistribute_Random::SendItem(PARTY_RECEIVE_MEMBER* pRealMember, WORD DropItemId, DWORD DropItemRatio, MONSTEREX_LIST * pMonInfo, WORD MonsterKind)
{
	if(pRealMember->count == 0)
		return;

	MONEYTYPE money;
	MONEYTYPE partymoney;
	int rate = rand() % pRealMember->count;

	CPlayer* pReceivePlayer = (CPlayer*)pRealMember->pPlayer[rate];
	if(ChkGetItemLevel(pReceivePlayer->GetLevel(), pMonInfo->Level) == FALSE)
		return;

	if(DropItemId == 0)
	{
		money = ITEMDROP_OBJ->MoneyItemNoItemPercentCalculator(pMonInfo, pReceivePlayer, MonsterKind); //to do : 여기서 아이템도 드랍 하는지 확인
		if(money)
		{
			partymoney = money/pRealMember->count;
			for(int i=0; i< pRealMember->count; ++i)
			{
				SendToPersonalMoney((CPlayer*)pRealMember->pPlayer[i], partymoney, MonsterKind);
			}
			return;
		}
	}
	else
	{
		if( DropItemRatio )
		if( ( rand()%100 ) % ( 100 / DropItemRatio ) == 0 )
			ITEMMGR->MonsterObtainItem((CPlayer*)pRealMember->pPlayer[rate],DropItemId,MonsterKind);
	}
}
*/



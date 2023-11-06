#include "stdafx.h"
#include ".\distribute_rotation.h"

#include "ItemDrop.h"
#include "ItemManager.h"
#include "Player.h"
#include "Monster.h"

#include "Party.h"
#include "PartyManager.h"

CDistribute_Rotation::CDistribute_Rotation(void)
{
}

CDistribute_Rotation::~CDistribute_Rotation(void)
{
}

void CDistribute_Rotation::SendItem(PARTY_RECEIVE_MEMBER* pRealMember, DWORD DropItemId, DWORD DropItemRatio, MONSTEREX_LIST * pMonInfo, WORD MonsterKind, LEVELTYPE MaxLevel)
{
	//if( pRealMember->count == 0 )	return;
	//if(ChkGetItemLevel(MaxLevel, pMonInfo->Level) == FALSE)	return;
 //  
	//CPlayer* pReceivePlayer = (CPlayer*)pRealMember->pPlayer[0] ;
	//if( !pReceivePlayer ) return ;

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
	///*
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
	//*/
}
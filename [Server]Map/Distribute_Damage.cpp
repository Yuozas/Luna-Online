// Distribute_Damage.cpp: implementation of the CDistribute_Damage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Distribute_Damage.h"
#include "ItemDrop.h"
#include "ItemManager.h"
#include "Player.h"
#include "Monster.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDistribute_Damage::CDistribute_Damage()
{

}

CDistribute_Damage::~CDistribute_Damage()
{

}
/*
void CDistribute_Damage::SendAbil(LEVELTYPE MonsterLevel, PARTY_RECEIVE_MEMBER* pRealMember)
{
	if(pRealMember->count == 0)
		return;

	//������ ��Ƽ ��� ���̵�(�α����� �ְ� ���� ���� �ڸ� ��ó�� �ִ�), ���� ����, ���ڰ� �� ������
	ASSERT(pRealMember->count <= MAX_PARTY_LISTNUM);
	DWORD BigDamage;
	BigDamage = 0;
	int num=0;
	int rate=0;
	for(int i=0; i<pRealMember->count; ++i)
	{
		if(BigDamage < pRealMember->Damage[i])
		{
			num = i;
			BigDamage = pRealMember->Damage[i];
		}
		else if(BigDamage == pRealMember->Damage[i])
		{
			rate = rand() % 2;
			if(rate == 1)
				num = i;
		}
	}
	SendToPersonalAbil((CPlayer*)pRealMember->pPlayer[num],MonsterLevel);
}
*/
void CDistribute_Damage::SendItem( PARTY_RECEIVE_MEMBER* pRealMember, DWORD DropItemId, DWORD DropItemRatio, MONSTEREX_LIST * pMonInfo, WORD MonsterKind, LEVELTYPE MaxLevel )
{
	//if(pRealMember->count == 0)	return;
	//if(ChkGetItemLevel(MaxLevel, pMonInfo->Level) == FALSE)	return;

	//if(DropItemId == 0)
	//{
	//	MONEYTYPE money;
	//	MONEYTYPE partymoney;
	//	CPlayer* pReceivePlayer = (CPlayer*)pRealMember->pPlayer[0];
	//	if( !pReceivePlayer )	return;

	//	money = ITEMDROP_OBJ->MoneyItemNoItemPercentCalculator(pMonInfo, pReceivePlayer, MonsterKind);
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
	//	DWORD BigDamage = 0;
	//	int num=0;
	//	int rate=0;
	//	for(int i=0; i<pRealMember->count; ++i)
	//	{
	//		if(BigDamage < pRealMember->Damage[i])
	//		{
	//			num = i;
	//		}
	//		else if(BigDamage == pRealMember->Damage[i])
	//		{
	//			rate = rand() % 2;
	//			if(rate == 1)
	//				num = i;
	//		}
	//	}
	//	CPlayer* pReceivePlayer = (CPlayer*)pRealMember->pPlayer[num];
	//	if( !pReceivePlayer )	return;

	//	//�����Լ� --; ��� �������
	//	if( DropItemRatio )
	//	if( ( rand()%100 ) % ( 100 / DropItemRatio ) == 0 )
	//		ITEMMGR->MonsterObtainItem(pReceivePlayer,DropItemId,MonsterKind);
	//}
}

/*
void CDistribute_Damage::SendItem( PARTY_RECEIVE_MEMBER* pRealMember, WORD DropItemId, DWORD DropItemRatio, MONSTEREX_LIST * pMonInfo, WORD MonsterKind, LEVELTYPE MaxLevel )
{
	if(pRealMember->count == 0)
		return;

//	ASSERT(MonsterKind & eObjectKind_Monster );
	MONEYTYPE money;
	MONEYTYPE partymoney;
	DWORD BigDamage;
	BigDamage = 0;
	int num=0;
	int rate=0;
	for(int i=0; i<pRealMember->count; ++i)
	{
		if(BigDamage < pRealMember->Damage[i])
		{
			num = i;
		}
		else if(BigDamage == pRealMember->Damage[i])
		{
			rate = rand() % 2;
			if(rate == 1)
				num = i;
		}
	}

	CPlayer* pReceivePlayer = (CPlayer*)pRealMember->pPlayer[num];
	if(ChkGetItemLevel(pReceivePlayer->GetLevel(), pMonInfo->Level) == FALSE)
		return;
	if(DropItemId == 0)
	{
		money = ITEMDROP_OBJ->MoneyItemNoItemPercentCalculator(pMonInfo, pReceivePlayer, MonsterKind);
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
		//�����Լ� --; ��� �������
		if( DropItemRatio )
		if( ( rand()%100 ) % ( 100 / DropItemRatio ) == 0 )
			ITEMMGR->MonsterObtainItem(pReceivePlayer,DropItemId,MonsterKind);
	}
}
*/
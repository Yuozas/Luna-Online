// ItemDrop.cpp: implementation of the CItemDrop class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemDrop.h"
#include "Player.h"
#include "time.h"
#include "ItemManager.h"
#include "Monster.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "MHFile.h"
#include "PartyManager.h"
#include "Distribute_Random.h"

// 070314 LYW --- ItemDrop : Include header files.
#include "Party.h"
#include "Distribute_Rotation.h"

// 070506 LYW --- ItemDrop : Include user table.
#include "UserTable.h"

// 080212 LYW --- ItemDrop : Include Boss rewards manager.
#include "BossRewardsManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemDrop::CItemDrop()
{
	m_MonsterItemDropTable.Initialize(500);
}

CItemDrop::~CItemDrop()
{
	ClearMonsterDropItemList();
}

void CItemDrop::Init()
{
	srand((unsigned)time(NULL));
	LoadMonsterDropItemList();
}

/*//KES �̰� ����..? �ּ�ó����!
void CItemDrop::ReloadDropItem(MONSTEREX_LIST * pMonsterExList)
{
	for(int n=0;n<eDROPITEMKIND_MAX;++n)
	{
		ASSERT(pMonsterExList->CurDropRate[n] == 0);
		pMonsterExList->CurDropRate[n] = pMonsterExList->DropRate[n];
	}
}
*/

// 070423 LYW --- ItemDrop : Delete function DropItemKind.
/*
int CItemDrop::DropItemKind(MONSTEREX_LIST * pMonsterExList, CPlayer* pPlayer)
{
	DWORD CurTotalRate = 0;
	DWORD DropRate[eDROPITEMKIND_MAX];

	for(int i=0; i<eDROPITEMKIND_MAX; i++)
	{
		DropRate[i] = pMonsterExList->CurDropRate[i];

		if(i == eMONEY)
		{
			//DropRate[eMONEY] *= gMoneyRate;
			DropRate[eMONEY] = (DWORD)(DropRate[eMONEY]*gEventRate[eEvent_MoneyRate]);
		}
		else if(i >= eITEM1 && i <= eITEM3)
		{
			// DropRate[i] *= gItemRate;
			DropRate[i] = (DWORD)(DropRate[i]*gEventRate[eEvent_ItemRate]);
		}

		CurTotalRate += DropRate[i];	
	}


//KES �� ���ϴ°���? �ּ�ó�� �ع�����.
//
//	if(CurTotalRate == 0)
//	{
//		ReloadDropItem(pMonsterExList);
//
//		for( i=0; i<eDROPITEMKIND_MAX; i++)
//		{
//			DropRate[i] = pMonsterExList->CurDropRate[i];
//
//			if(i == eMONEY)
//				DropRate[eMONEY] *= gMoneyRate;
//			else if(i >= eITEM1 && i <= eITEM3)
//				DropRate[i] *= gItemRate;
//
//			CurTotalRate += DropRate[i];	
//		}
//	}
//

	// RaMa - 06.02.08 ->��ɲ��Ǻ���
	if( pPlayer->GetShopItemStats()->AddItemDrop )
	{
		for(i=eITEM1; i<=eITEM3; ++i)
		{
			CurTotalRate -= DropRate[i];
			DropRate[i] *= (1+(WORD)(pPlayer->GetShopItemStats()->AddItemDrop*0.01f));
			CurTotalRate += DropRate[i];
		}
	}

	// RaMa -04.11.24  ->���������
	if( CurTotalRate > MAX_DROP_ITEM_PERCENT )
	{
		int dwTemp = DropRate[eMONEY]+DropRate[eITEM1]+DropRate[eITEM2]+DropRate[eITEM3];

		if( dwTemp < MAX_DROP_ITEM_PERCENT )
		{
			DropRate[eNOITEM] = MAX_DROP_ITEM_PERCENT - dwTemp;
			CurTotalRate = MAX_DROP_ITEM_PERCENT;
		}
		else
		{
			DropRate[eNOITEM] = 0;
			CurTotalRate = dwTemp;
		}
	}

	//CurTotalRate == 0�̸� �ȵȴ�.
	if( CurTotalRate == 0 )
	{
//		char buf[128];
//		wsprintf( buf, "MonsterIdx:%d", pMonsterExList->MonsterKind );
//		ASSERTMSG(0, buf);
		ASSERT(0);
		return -1;
	}

	DWORD RandRate = random(1, CurTotalRate);
	DWORD CurMinRate=0,CurMaxRate=0;

	for( i=0; i<eDROPITEMKIND_MAX; i++)
	{
		CurMinRate = CurMaxRate;
		CurMaxRate += DropRate[i];

//		if(RandRate >= CurMinRate && RandRate < CurMaxRate)	//KES �̷��� �ȵ���!!! 050331
		if(RandRate > CurMinRate && RandRate <= CurMaxRate)
		{
//			ASSERT(DropRate[i] != 0);
			return i;
		}
	}
	ASSERT(0);

	char buf[128];
	wsprintf( buf, "MonsterIdx:%d, CurTotalRate:%d, RandRate:%d, CurMinRate:%d, CurMaxRate:%d,",
		pMonsterExList->MonsterKind, CurTotalRate, RandRate, CurMinRate, CurMaxRate );
	ASSERTMSG(0, buf);

	return -1;


//
//	WORD* CurRate = pMonsterExList->CurDropRate;
//
//	WORD CurTotalRate=0;
//	for(int n=0;n<eDROPITEMKIND_MAX;++n)
//		CurTotalRate += CurRate[n];
//
//	if(CurTotalRate == 0)
//	{
//		CurTotalRate = 0;
//		ReloadDropItem(pMonsterExList);
//		for(int n=0;n<eDROPITEMKIND_MAX;++n)
//			CurTotalRate += CurRate[n];
//	}
//	
//	--CurTotalRate;	
//
//	WORD RandRate = random( 0, CurTotalRate );
//
//	WORD CurMinRate=0,CurMaxRate=0;
//	for(n=0;n<eDROPITEMKIND_MAX;++n)
//	{
//		CurMinRate = CurMaxRate;
//		CurMaxRate += CurRate[n];
//		if(CurMinRate <= RandRate && RandRate < CurMaxRate)
//		{
//			ASSERT(CurRate[n] != 0);
//			--CurRate[n];
//			return n;
//		}
//	}
//	ASSERT(0);
//	return -1;
//
}
*/
// 070418 LYW --- ItemDrop : Modified function to claculate drop rate.
/*
MONEYTYPE CItemDrop::MoneyItemNoItemPercentCalculator(MONSTEREX_LIST * pMonInfo, CPlayer* pPlayer, WORD MonsterKind)
{

//	ASSERT(pMob->GetObjectKind() & eObjectKind_Monster );
	
	// RaMa - 04.11.24   ->ShopItemOption�� ItemDrop���� �߰�

	MONEYTYPE TotalMoney = 0; 

	for( int i = 0 ; i < pMonInfo->MaxDropItemNum ; ++i )
	{
		int eDropItemKind = DropItemKind(pMonInfo, pPlayer);

		switch(eDropItemKind)
		{
		case eNOITEM:
			{

			}
			break;
		case eMONEY:
			{
				//return MonsterDropMoney(pMonInfo);
				TotalMoney += MonsterDropMoney(pMonInfo);
		//	if( pPlayer->SetMoney(Money, MONEY_ADDITION, 1, eItemTable_Inventory, eMoneyLog_GetMonster, pMob->GetID()) != Money )
		//	{
		//		// error msg ������. ���ѷ� �ʰ�
		//		MSGBASE msg;
		//		msg.Category = MP_ITEM;
		//		msg.Protocol = MP_ITEM_MONEY_ERROR;
		//		msg.dwObjectID = pPlayer->GetID();
//
//				pPlayer->SendMsg(&msg, sizeof(msg));
//			}
			}
			break;
		case eITEM1:
			{
				MonsterDropItem(MonsterKind, pMonInfo->ItemListNum1, pPlayer);
			}
			break;
		case eITEM2:
			{
				MonsterDropItem(MonsterKind, pMonInfo->ItemListNum2, pPlayer);
			}
			break;
		case eITEM3:
			{
				MonsterDropItem(MonsterKind, pMonInfo->ItemListNum3, pPlayer);
			}
			break;
		default:
			{
				ASSERT(0);
			}
		}
	}

//	return 0;
	return TotalMoney;
}
*/
MONEYTYPE CItemDrop::MoneyItemNoItemPercentCalculator(MONSTEREX_LIST * pMonInfo, WORD MonsterKind, int nPenaltyType)
{
	if( !pMonInfo) return 0 ;															// ���Ϳ� �÷��̾� ������ üũ�Ѵ�.

	MONEYTYPE TotalMoney = 0;															// ��� �Ӵϸ� ��� ����.

	TotalMoney += MonsterDropMoney(pMonInfo, nPenaltyType);								// ������ ��� �Ӵϸ� �޴´�.

	return TotalMoney;																	// ȹ���� �Ӵϸ� ��ȯ�Ѵ�.
}

//// 070506 LYW --- ItemDrop : Add for test.
////#ifdef _DEBUG
//	
//	if( m_bOnDebugString )
//	{
//		MSG_DROPITEM_RESULT msg ;
//		msg.byCount = 0 ;
//		
//		for( BYTE count = 0 ;  count < 5 ; ++count )
//		{
//			NewMonsterDropItem(MonsterKind, &pMonInfo->dropItem[count], pPlayer, &msg);
//		}
//		
//		msg.Category = MP_CHEAT ;
//		msg.Protocol = MP_CHEAT_DROPITEM_INFO_ACK ;
//		msg.dwObjectID = pPlayer->GetID() ;
//
//		//CPlayer* pDebugStringPlayer = (CPlayer*)g_pUserTable->FindUser(ITEMDROP_OBJ->m_dwDebugStringID);
//
//		//pDebugStringPlayer->SendMsg(&msg, sizeof(msg)) ;
//		pPlayer->SendMsg(&msg, sizeof(msg)) ;
//	}
//	else
//	{
//		for( BYTE count = 0 ;  count < 5 ; ++count )
//		{
//			NewMonsterDropItem(MonsterKind, &pMonInfo->dropItem[count], pPlayer);
//		}
//	}
//	/*
//#else
//	for( BYTE count = 0 ;  count < 5 ; ++count )
//	{
//		NewMonsterDropItem(MonsterKind, &pMonInfo->dropItem[count], pPlayer);
//	}
//#endif // _DEBUG
//	*/
//
//	CParty* pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx() ) ;
//	if(pParty && pParty->IsDistributed()) pParty->SetDistributed(FALSE) ;

//	return TotalMoney;
//}

// 070418 LYW --- ItemDrop : Modified function to calculate drop money.
/*
MONEYTYPE CItemDrop::MonsterDropMoney(MONSTEREX_LIST * pMonsterExList)
{
	MONEYTYPE MinMoney = pMonsterExList->MinDropMoney;
	MONEYTYPE MaxMoney = pMonsterExList->MaxDropMoney;
//	ASSERT(MaxMoney >= MinMoney);

	MONEYTYPE ResultMoney = 0;

//	if(MinMoney == MaxMoney)
//	{
//		ResultMoney = MinMoney;
//	}
//	else
//	{
//		ResultMoney = (rand() % (MaxMoney - MinMoney)) + MinMoney;
//	}
	if( MaxMoney > MinMoney )
	{
		ResultMoney = random( MinMoney, MaxMoney );
	}
	else
	{
		ResultMoney = MinMoney;
	}

	// RaMa -04.11.24   ->�� ��� �׼�
	// ResultMoney *= gGetMoney;
	ResultMoney = (MONEYTYPE)(ResultMoney*gEventRate[eEvent_GetMoney]);

	return ResultMoney;
}
*/

// 080212 LYW --- ItemDrop : ����, �Ϲ� ���Ϳ� ���� �޾ƿ��� �� ó���� ������ �ش�.
MONEYTYPE CItemDrop::MonsterDropMoney(MONSTEREX_LIST * pMonsterExList, int nPenaltyType)
{
	ASSERT( pMonsterExList ) ;
	if( !pMonsterExList ) return 0 ;										// �ʿ� ���� üũ.

	MONEYTYPE ResultMoney	= 0;											// ȹ�� �Ӵ�.
	MONEYTYPE MaxMoney		= 0 ; 

	if( pMonsterExList->ObjectKind == eObjectKind_BossMonster )				// ���� �����̸�, 
	{
		WORD wMonsterKind = 0 ;
		wMonsterKind = pMonsterExList->MonsterKind ;

		MaxMoney = BOSSREWARDSMGR->GetMoneyOfBossMonster(wMonsterKind) ;	// ���� ���� �Ӵϸ� �޴´�.
	}
	else																	// �Ϲ� �����̸�,
	{
		MaxMoney = pMonsterExList->dropMoney ;								// ���Ͱ� ����� �� �ִ� �ִ� �Ӵϸ� �޴´�.
	}

	DWORD dwDropRateValue = pMonsterExList->dwMoneyDropRate ;				// ������ �Ӵ� ������� �޴´�.

	WORD wDropRate1 = random(0, 10000) ;									// �̱� Ȯ��1�� ���Ѵ�.
	WORD wDropRate2 = random(0, 10000) ;									// �̱� Ȯ��2�� ���Ѵ�.

	DWORD dwDropRate = (wDropRate1 * 10000) + wDropRate2 ;					// ����� �̱� Ȯ���� ���Ѵ�.

	float pickupPercent = random(50.0f, 100.0f) ;							// ȹ���� �� �ִ� Ȯ���� ���Ѵ�.
	float fObtainMoney = 0.0f ;												// ȹ���� �Ӵϸ� �ӽ÷� ���� ����.

	if( dwDropRateValue >= dwDropRate)										// ���� ��� Ȯ���� �̱� Ȯ�� ��.
	{
		fObtainMoney = MaxMoney * (pickupPercent/100) ;						// ȹ�� �Ӵϸ� �����Ѵ�.
	}

	if( nPenaltyType == 0 )
	{
		ResultMoney = (DWORD)fObtainMoney ;
	}
	else if( nPenaltyType == 1 )
	{
		ResultMoney = (DWORD)(fObtainMoney * 0.5f) ;
	}
	else if( nPenaltyType == 2 )
	{
		ResultMoney = (DWORD)(fObtainMoney * 0.25f) ;
	}
	else if( nPenaltyType == 3 )
	{
		ResultMoney = 1 ;
	}

	ResultMoney *= (DWORD)(gEventRate[eEvent_MoneyRate]);//gMoneyRate;  //090113 modified by XB ��MoneyDrop rate

	return ResultMoney ;													// ȹ�� �Ӵϸ� ��ȯ�Ѵ�.
}

//	// 070506 LYW --- ItemDrop : Add for test.
//#ifdef _DEBUG
//	if( m_bOnDebugString )
//	{
//		MSG_DROPMONEY_RESULT msg ;
//
//		msg.Category		= MP_CHEAT ;
//		msg.Protocol		= MP_CHEAT_DROPMONEY_INFO_ACK ;
//		msg.dwMonstersMoney = pMonsterExList->dropMoney ;
//		msg.dwMoneyDropRate = pMonsterExList->dwMoneyDropRate ;
//		msg.wRandomRate		= wDropRate ;
//		msg.fPickupPercent	= pickupPercent ;
//		msg.dwResultMoney	= ResultMoney ;
//
//		//CPlayer* pDebugStringPlayer = (CPlayer*)g_pUserTable->FindUser(ITEMDROP_OBJ->m_dwDebugStringID);
//
//		//pDebugStringPlayer->SendMsg(&msg, sizeof(msg)) ;
//		pPlayer->SendMsg(&msg, sizeof(msg)) ;
//	}
//#endif //_DEBUG

//	double dDropRateValue = pMonsterExList->dwMoneyDropRate ;	// pMonsterExList->droprate.
//	double dMonsterDropRate = dDropRateValue / 10000 ;
//
//	double dDropRate = random(0, 10000) ;
//	double dRandomDropRate = dDropRate / 100 ;
//
//	MONEYTYPE ResultMoney = 0;
//
//	float pickupPercent = random(50.0f, 100.0f) ;
//
//	if( dMonsterDropRate >= dRandomDropRate)
//	{
//		ResultMoney = (MONEYTYPE)(MaxMoney * (pickupPercent/100)) ;
//
//		
////		CParty* pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx() ) ;
////
////		if( pParty && !pParty->IsDistributed() ) 
////		{
////			BYTE winnerIdx = pParty->GetWinnerIdx() ;
////			
////			if(winnerIdx == MAX_PARTY_LISTNUM-1 )
////			{
////				pParty->SetWinnerIdx(0) ;
////			}
////			else
////			{
////				pParty->SetWinnerIdx(++winnerIdx) ;
////			}
////
////			pParty->SetDistributed(TRUE) ;
////		}
//		
//	}
//
//	// 070506 LYW --- ItemDrop : Add for test.
//#ifdef _DEBUG
//	if( m_bOnDebugString )
//	{
//		MSG_DROPMONEY_RESULT msg ;
//
//		msg.Category		= MP_CHEAT ;
//		msg.Protocol		= MP_CHEAT_DROPMONEY_INFO_ACK ;
//		msg.dwMonstersMoney = pMonsterExList->dropMoney ;
//		msg.dwMoneyDropRate = pMonsterExList->dwMoneyDropRate ;
//		msg.wRandomRate		= (WORD)dDropRate ;
//		msg.fPickupPercent	= pickupPercent/100 ;
//		msg.dwResultMoney	= ResultMoney ;
//
//		//CPlayer* pDebugStringPlayer = (CPlayer*)g_pUserTable->FindUser(ITEMDROP_OBJ->m_dwDebugStringID);
//
//		//pDebugStringPlayer->SendMsg(&msg, sizeof(msg)) ;
//		pPlayer->SendMsg(&msg, sizeof(msg)) ;
//	}
//#endif //_DEBUG

	//return ResultMoney ;
//}

void CItemDrop::MonsterDropItem(WORD MonsterKind, WORD wDropItemIdx, CPlayer* pPlayer)
{
	MONSTER_DROP_ITEM * pDropItem = GetMonsterItemDrop(wDropItemIdx);
	ASSERT(pDropItem);
	if(pDropItem == NULL)
		return;
	
	if( pDropItem->dwCurTotalRate == 0 )
	{
		// yh �������� ������� �ʴ� ���(TotalRate �� 0)�� FALSE ����
		if(ReloadMonsterDropItem(pDropItem) == FALSE)
			return;
	}

	DWORD RandRate = random( 1, pDropItem->dwCurTotalRate );
	
	DWORD FromPercent	= 0;
	DWORD ToPercent		= 0;

	for(WORD i = 0 ; i < pDropItem->wTotalNum ; ++i)
	{
		FromPercent = ToPercent;
		ToPercent += pDropItem->dwCurMonsterDropItemPercent[i];
//		if(FromPercent <= RandRate && RandRate < ToPercent)
		if(FromPercent < RandRate && RandRate <= ToPercent)
		{
//			ASSERT(pDropItem->CurMonsterDropItemPercent[i]!=0);
//			ASSERT(pDropItem->wCurTotalRate!=0);
			--pDropItem->dwCurMonsterDropItemPercent[i];
			--pDropItem->dwCurTotalRate;


			if(pDropItem->MonsterDropItemPercent[i].wItemIdx)
			{
				DWORD DropItemId = pDropItem->MonsterDropItemPercent[i].wItemIdx;

					DISTRIBUTERAND->ItemChangeAtLv(DropItemId);
				
				// 070314 LYW --- ItemDrop : Check party option.
				CParty* pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx() ) ;		// pParty�� NULL�� ��� ���� üũ ��. 2007/03/26 - ������
				// 070329 LYW --- ItemDrop : Check party.
				if( pParty == NULL ) 
				{
					ITEMMGR->MonsterObtainItem(pPlayer, DropItemId, MonsterKind);
				}
				else
				{
					if(pParty->GetOption() == ePartyOpt_Sequence)
					{
						pParty->SetWinner() ;

						ITEMMGR->MonsterObtainItem(pParty->GetItemWinner(), DropItemId, MonsterKind);
					}
					else
					{
						ITEMMGR->MonsterObtainItem(pPlayer, DropItemId, MonsterKind);
					}
				}
			}
			break;
		}
	}

}
// 070419 LYW --- ItemDrop : Add new function to drop item.
//#ifdef _DEBUG
BOOL CItemDrop::NewMonsterDropItem(WORD MonsterKind, MONSTER_DROPITEM* pDropItem, CPlayer* pPlayer, int nPenaltyType, MSG_DROPITEM_RESULT* pResult)
{
	if( !pDropItem || !pPlayer ) return FALSE ;									// ������ ������ �÷��̾� ������ üũ�Ѵ�.

	if( pDropItem->wItemIdx == 0 ) return FALSE ;								// ��� �������� �ε����� üũ�Ѵ�.

	// 080115 LYW --- ItemDrop : GM������ ������ ����� ���� ó��.
	//DWORD dwDropRateValue = pDropItem->dwDropPercent ;						// ������� �޴´�.

	if( gEventRate[eEvent_ItemRate] < 0 )										// Ȥ�ö� ������ ������, 
	{
		gEventRate[eEvent_ItemRate] = 1 ;										// �Ʒ� Ȯ���� ū�ϳ���.
	}

	DWORD dwDropRateValue = 0 ;
	dwDropRateValue = (DWORD)(gEventRate[eEvent_ItemRate] * pDropItem->dwDropPercent) ;	// ����� ����.
	//dwDropRateValue = (DWORD)(pDropItem->dwDropPercent) ;	// ����� ����.
	// 071204 -- KTH ����� ��� Buff ����
	dwDropRateValue += DWORD( dwDropRateValue * (pPlayer->GetRateBuffStatus()->IncreaseDropRate/100) );


	//dwDropRateValue *= gItemRate; //090113 modified by XB add droprate


	// stdlib�� rand �Լ��� �ִ� ���� 32767���� ������ ������, random �Լ��� ����� �Ѵ�.

	DWORD dwDropRate1 = random(0, 99) ;										// ���ڸ� �̱� Ȯ���� ���Ѵ�.
	DWORD dwDropRate2 = random(0, 99) ;										// ���ڸ� �̱� Ȯ���� ���Ѵ�.
	DWORD dwDropRate3 = random(0, 99) ;										// ���ڸ� �̱� Ȯ���� ���Ѵ�.
	DWORD dwDropRate4 = random(0, 99) ;										// ���ڸ� �̱� Ȯ���� ���Ѵ�.

	DWORD dwDropRate = (dwDropRate1 * 1000000) + (dwDropRate2 * 10000) + (dwDropRate3 * 100) + dwDropRate4 ;					// ����� �̱� Ȯ���� ���Ѵ�.

	if( nPenaltyType == 2 )														// �г�Ƽ Ÿ���� 1�̶��, �������̰� 11~15�̴�.
	{
		//float fDropRate = dwDropRate*0.75f ;
		//dwDropRate += (WORD)GetFloatInt(fDropRate) ;							// ��� Ȯ���� 75% ���ҽ�Ų��.

		float fDropRate = dwDropRateValue * 0.75f ;

		// warning ��� ������ ����ȯ�� ���ش�.
		dwDropRateValue = (DWORD)fDropRate ;
	}
	else if( nPenaltyType == 1 )												// �г�Ƽ Ÿ���� 0�̶��, �������̰� 6~10�̴�.
	{
		//float fDropRate = dwDropRate*0.5f ;
		//dwDropRate += (WORD)GetFloatInt(fDropRate) ;							// ��� Ȯ���� 50% ���ҽ�Ų��.

		float fDropRate = dwDropRateValue * 0.5f ;
	
		// warning ��� ������ ����ȯ�� ���ش�.
		dwDropRateValue = (DWORD)fDropRate ;
	}
	// nPenaltyType == 0 �̸� �г�Ƽ�� ������� �ʴ´�.

	if( dwDropRateValue > dwDropRate)											// ��� Ȯ���� �̱� Ȯ���� ���Ѵ�.
	{
		ITEMMGR->MonsterObtainItem(pPlayer, pDropItem->wItemIdx, MonsterKind, (WORD)pDropItem->byCount);

		return TRUE ;
	}

	return FALSE ;
}
/*
#else
void CItemDrop::NewMonsterDropItem(WORD MonsterKind, MONSTER_DROPITEM* pDropItem, CPlayer* pPlayer)
{
	if( pDropItem->wItemIdx == 0 ) return ;

	double dDropRateValue = pDropItem->dwDropPercent ;
	double dMonsterDropRate = dDropRateValue / 10000 ;

	double dwDropRate = random(0, 10000) ; 
	double dDropRate = dwDropRate / 100 ;
	
	if( dMonsterDropRate >= dDropRate) 
	{
		CParty* pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx() ) ;

		if( pParty == NULL ) 
		{
			ITEMMGR->MonsterObtainItem(pPlayer, pDropItem->wItemIdx, MonsterKind, (WORD)pDropItem->byCount);
		}
		else
		{
			if(pParty->GetOption() == ePartyOpt_Sequence)
			{
				pParty->SetWinner() ;

				ITEMMGR->MonsterObtainItem(pParty->GetItemWinner(), pDropItem->wItemIdx, MonsterKind, (WORD)pDropItem->byCount);
			}
			else
			{
				ITEMMGR->MonsterObtainItem(pPlayer, pDropItem->wItemIdx, MonsterKind, (WORD)pDropItem->byCount);
			}
		}
	}
}
#endif //_DEBUG
*/



BOOL CItemDrop::ReloadMonsterDropItem(MONSTER_DROP_ITEM* pDropItem)
{
	WORD HaveItem = pDropItem->wTotalNum;

	pDropItem->dwCurTotalRate = 0;
	for(WORD i = 0; i < HaveItem; ++i)
	{
		pDropItem->dwCurMonsterDropItemPercent[i] = pDropItem->MonsterDropItemPercent[i].dwDropPercent;
		pDropItem->dwCurTotalRate += pDropItem->dwCurMonsterDropItemPercent[i];
	}
	if(pDropItem->dwCurTotalRate == 0)
		return FALSE;

	return TRUE;
}


void CItemDrop::ClearMonsterDropItemList()
{
	// reset data
	m_MonsterItemDropTable.SetPositionHead();
	MONSTER_DROP_ITEM* pDropItem;
	while(pDropItem = m_MonsterItemDropTable.GetData())
	{
		delete pDropItem;
	}
	m_MonsterItemDropTable.RemoveAll();
}

BOOL CItemDrop::LoadMonsterDropItemList()
{
	ClearMonsterDropItemList();

	char filename[64];
	CMHFile file;
	sprintf(filename,"./System/Resource/MonsterDropItemList.bin");	
	if(!file.Init(filename, "rb"))
		return FALSE;

	while(1)
	{
		if(file.IsEOF())
			break;
		
		MONSTER_DROP_ITEM* pDropItem = new MONSTER_DROP_ITEM;
		memset(pDropItem, 0, sizeof(MONSTER_DROP_ITEM));
		pDropItem->wItemDropIndex = file.GetWord();// �б⸸ �Ѵ�.

		WORD totalnum = 0;
		for(int idx=0; idx<MAX_DROPITEM_NUM; ++idx)
		{
			// 070419 LYW --- ItemDrop : Modified function LoadMonsterDropItemList.
			/*
#ifdef _DEBUG
			//file.GetString(pDropItem->MonsterDropItemPercent[idx].Name);
			SafeStrCpy( pDropItem->MonsterDropItemPercent[idx].Name, file.GetString(), MAX_ITEMNAME_LENGTH+1 );
			
			pDropItem->MonsterDropItemPercent[idx].wItemIdx = file.GetWord();		
			pDropItem->MonsterDropItemPercent[idx].dwDropPercent = file.GetDword();
			if(pDropItem->MonsterDropItemPercent[idx].dwDropPercent)
//			if(pDropItem->MonsterDropItemPercent[idx].wItemIdx)
				++totalnum;
#else
			char szTempName[256];
			file.GetString(szTempName);
			pDropItem->MonsterDropItemPercent[idx].wItemIdx = file.GetWord();		
			pDropItem->MonsterDropItemPercent[idx].dwDropPercent = file.GetDword();
			if(pDropItem->MonsterDropItemPercent[idx].dwDropPercent)
//			if(pDropItem->MonsterDropItemPercent[idx].wItemIdx)
				++totalnum;
#endif
				*/

#ifdef _DEBUG
			SafeStrCpy( pDropItem->MonsterDropItemPercent[idx].Name, file.GetString(), MAX_ITEMNAME_LENGTH+1 );
#else
			char szTempName[256];
			file.GetString(szTempName);
#endif

			pDropItem->MonsterDropItemPercent[idx].wItemIdx		 = file.GetDword();		
			pDropItem->MonsterDropItemPercent[idx].dwDropPercent = file.GetDword();
			pDropItem->MonsterDropItemPercent[idx].byCount		 = file.GetByte() ;

			if(pDropItem->MonsterDropItemPercent[idx].dwDropPercent)
				++totalnum;
		}
		
		pDropItem->wTotalNum = totalnum;
		pDropItem->dwCurTotalRate = 0;

		ASSERT(!m_MonsterItemDropTable.GetData(pDropItem->wItemDropIndex));
		m_MonsterItemDropTable.Add(pDropItem,pDropItem->wItemDropIndex);

		ReloadMonsterDropItem(pDropItem);
	}	
	
	file.Release();
	
	return TRUE;
}

MONSTER_DROP_ITEM* CItemDrop::GetMonsterItemDrop(WORD wItemDropIdx)
{
	return m_MonsterItemDropTable.GetData(wItemDropIdx);
}

//////////////////////////////////////////////////////////////////////////
// test
// old code 
/*


void CItemDrop::TestMonsterDropItem(WORD wDropItemListIdx)
{
	int Kind = wDropItemListIdx;
	
	MONSTER_ITEM_DROP* pDropItem = GetMonsterItemDrop(Kind);
	int HaveItem = pDropItem->m_MonsterDropTotalNum;	
	int MaxDropNum = 0;
	int DropNum = 0;
	int DropPercent = 0;
	int CurPercent = 0;
	int TempPercent = 0;
	
	int VarMaxPercent = pDropItem->m_MonsterMaxVarPercent; // �и�
	if(!VarMaxPercent)
		return;

	DropNum = 1;
	for(int i = 0; i < DropNum; i++)
	{
		CurPercent = 0;
		TempPercent = 0;
		DropPercent = rand() % VarMaxPercent; // ����߸� Ȯ��
		if(!DpPercent)
			DropPercent = 1;
		for(int j=0; j<HaveItem; j++)
		{
			CurPercent = pDropItem->m_MonsterDropItemVarPercent[j].DropPercent;
			if(!CurPercent)	// �ۼ�Ʈ�� 0�̸� Zeroüũ�� Reload
			{
				BOOL flag = ZeroCheckPercent(Kind);
				if(flag)
				{
					ReloadDropItemPercent(Kind); // ��� 0�̸� ������
					
					FILE* fp = fopen("itemdroptest.txt","a+");
					fprintf(fp,"ReloadDropItemPercent\n",pDropItem->m_MonsterDropItemVarPercent[j].Name,
						pDropItem->m_MonsterDropItemVarPercent[j].ItemIdx);
					
					
					fclose(fp);
				}
			}
			else if(TempPercent<DropPercent && TempPercent+CurPercent>=DropPercent)
			{
				// Ȯ���� �������Ƿ� ���
				MONSTER_ITEM tmp;
				WORD HuntItemNum = pDropItem->m_MonsterDropItemVarPercent[j].ItemIdx;
				tmp = pDropItem->m_MonsterDropItemVarPercent[j];
				if(HuntItemNum == 0) 
					return;
				ASSERT(HuntItemNum != 52685);
				FILE* fp = fopen("itemdroptest.txt","a+");
				fprintf(fp,"item : %s (%d)\n",pDropItem->m_MonsterDropItemVarPercent[j].Name,
											pDropItem->m_MonsterDropItemVarPercent[j].ItemIdx);
				

				fclose(fp);
				
				CalculItemPercent(Kind, HaveItem, j);
			}
			TempPercent = TempPercent+CurPercent;
		}
		
	}
}
*/
//////////////////////////////////////////////////////////////////////////


/*
void CItemDrop::CalculItemPercent(WORD Kind, int HaveItem, int idx)
{
	MONSTER_ITEM_DROP* pDropItem = GetMonsterItemDrop(Kind);

	int VarPercent = 0;
	int tmpPercent = pDropItem->m_MonsterDropItemVarPercent[idx].DropPercent;
	int VarMaxPercent = pDropItem->m_MonsterMaxVarPercent; // �и�
	if(tmpPercent)
	{
		for(int i = 0; i < HaveItem; i++)
		{
			VarPercent = pDropItem->m_MonsterDropItemVarPercent[i].DropPercent;
			if(!VarPercent)
			{
				pDropItem->m_MonsterDropItemVarPercent[i].DropPercent = VarPercent;
				continue;
			}
			if(!VarMaxPercent)
				ReloadDropItemPercent(Kind); // 0�̸� ������
			if(i == idx)
			{
				pDropItem->m_MonsterDropItemVarPercent[i].DropPercent = VarPercent-1;
				pDropItem->m_MonsterMaxVarPercent = VarMaxPercent-1;
			}
		}
		// ��� �ۼ�Ʈ�� 0���� �˻�
		BOOL flag = ZeroCheckPercent(Kind);
		if(flag)
			ReloadDropItemPercent(Kind); // ��� 0�̸� ������
	}
	else
	{
		// ���⿡ �ɸ��� �������
		BOOL flag = ZeroCheckPercent(Kind);
		if(flag)
			ReloadDropItemPercent(Kind); // ��� 0�̸� ������
	}
}

BOOL CItemDrop::ZeroCheckPercent(WORD Kind)
{
	MONSTER_ITEM_DROP* pDropItem = GetMonsterItemDrop(Kind);

	int tmpPercent[MAX_DROPITEM_NUM];
	int AddPercent = 0;
	for(int i = 0; i < MAX_DROPITEM_NUM; i++)
	{
		tmpPercent[i] = pDropItem->m_MonsterDropItemVarPercent[i].DropPercent;
		AddPercent = AddPercent + tmpPercent[i];
	}

	if(AddPercent) // �ۼ�Ʈ ������ ���� 0�̸� ���� 0��
		return FALSE;  // Reload �϶�� ����
	else
		return TRUE;
}
*/

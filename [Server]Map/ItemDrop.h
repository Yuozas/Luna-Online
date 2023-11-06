// ItemDrop.h: interface for the CItemDrop class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMDROP_H__59770A47_500D_485D_89F1_9835D4500264__INCLUDED_)
#define AFX_ITEMDROP_H__59770A47_500D_485D_89F1_9835D4500264__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "Player.h"
//class CPlayer;

// 070423 LYW --- ItemDrop : Include GameResourceManager's header file.
#include "..\[CC]Header\GameResourceManager.h"

#define ITEMDROP_OBJ CItemDrop::GetInstance()
class MONSTEREX_LIST;

struct MONSTER_ITEM
{
#ifdef _DEBUG
	char Name[MAX_ITEMNAME_LENGTH+1];			// 아이템 이름
#endif
	DWORD wItemIdx;							// 해당 몬스터가 사망했을 때 드랍하는 아이템 - "0"값 입력 시 드랍 아이템 없음
	DWORD dwDropPercent;					// 해당몬스터가 사망했을 때 드랍하는 아이템의 드랍 확률 - 박만분율 수치로 입력 예) 75000 = 7.5%

	// 070419 LYW --- ItemDrop : Add count value to MONSTER_ITEM.
	BYTE byCount ;							// 해당 몬스터가 사망했을 때 드랍하는 아이템의 최대 개수 1~최대개수 랜덤하게 드랍.
};
/*
struct MONSTER_ITEM_DROP
{
	DWORD ItemDropIndex;
	int m_MonsterMaxPercent;
	int m_MonsterMaxVarPercent;	
	int m_MonsterDropTotalNum;
	MONSTER_ITEM m_MonsterDropItem[MAX_DROPITEM_NUM];
	MONSTER_ITEM m_MonsterDropItemVarPercent[MAX_DROPITEM_NUM];
};
*/

#define MAX_DROP_ITEM_PERCENT			10000
//#define MAX_MONSTER_DROP_ITEM_PERCENT	1000000

struct MONSTER_DROP_ITEM
{
	WORD			wItemDropIndex;
	WORD			wTotalNum;
	DWORD			dwCurTotalRate;
	DWORD			dwCurMonsterDropItemPercent[MAX_DROPITEM_NUM];
	MONSTER_ITEM	MonsterDropItemPercent[MAX_DROPITEM_NUM];
};

class CItemDrop  
{
	CYHHashTable<MONSTER_DROP_ITEM>	m_MonsterItemDropTable;

	BOOL LoadMonsterDropItemList();
	void ClearMonsterDropItemList();

	MONSTER_DROP_ITEM* GetMonsterItemDrop(WORD wItemDropIdx);

public:
	GETINSTANCE(CItemDrop);
 	
	CItemDrop();
	virtual ~CItemDrop();

//	void ReloadDropItem(MONSTEREX_LIST * pMonsterExList);
	// 070423 LYW --- ItemDrop : Delete function DropItemKind.
	//int DropItemKind(MONSTEREX_LIST * pMonsterExList, CPlayer* pPlayer);

	void Init();
	MONEYTYPE MoneyItemNoItemPercentCalculator(MONSTEREX_LIST * pMonInfo, WORD MonsterKind, int nPenaltyType);
	MONEYTYPE MonsterDropMoney(MONSTEREX_LIST * pMonsterExList, int nPenaltyType);
	
	void MonsterDropItem(WORD MonsterKind, WORD wDropItemListIdx, CPlayer* pPlayer);
	BOOL ReloadMonsterDropItem(MONSTER_DROP_ITEM* pDropItem);
	//void ReloadDropItemPercent(WORD Kind);
	//void CalculItemPercent(WORD Kind, int HaveItem, int idx);
	//BOOL ZeroCheckPercent(WORD Kind);

	// 070506 LYW --- ItemDrop : Add for test.
//#ifdef _DEBUG 
	BOOL m_bOnDebugString ;
	DWORD m_dwDebugStringID ;
	void SetDebugStringOn(BOOL val) { m_bOnDebugString = val ; }
	void SetDebugStringID(DWORD id) { m_dwDebugStringID = id ; }

	// 070419 LYW --- ItemDrop : Add new function to drop item.
	BOOL NewMonsterDropItem(WORD MonsterKind, MONSTER_DROPITEM* pDropItem, CPlayer* pPlayer, int nPenaltyType, MSG_DROPITEM_RESULT* pResult = NULL) ;
	/*
#else
	// 070419 LYW --- ItemDrop : Add new function to drop item.
	void NewMonsterDropItem(WORD MonsterKind, MONSTER_DROPITEM* pDropItem, CPlayer* pPlayer) ;
#endif //_DEBUG
	*/

	//void TestMonsterDropItem(WORD wDropItemListIdx);
};

#endif // !defined(AFX_ITEMDROP_H__59770A47_500D_485D_89F1_9835D4500264__INCLUDED_)

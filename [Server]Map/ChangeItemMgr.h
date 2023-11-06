// ChangeItemMgr.h: interface for the CChangeItemMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHANGEITEMMGR_H__94A4A82B_1B66_44EE_9965_C0BAFC2B306C__INCLUDED_)
#define AFX_CHANGEITEMMGR_H__94A4A82B_1B66_44EE_9965_C0BAFC2B306C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CHANGEITEMMGR CChangeItemMgr::GetInstance()


#define MAX_CHANGE_RATE	30001

struct sCHANGEITEMUNIT
{
	DWORD	wToItemIdx;
	DWORD	dwToItemDur;
	DWORD	dwPercent;
};

struct sCHANGEITEM
{
	DWORD	wItemIdx;
	WORD	wMaxToItem;
	sCHANGEITEMUNIT*	pItemUnit;

	sCHANGEITEM()	{ wItemIdx = wMaxToItem = 0; }
	~sCHANGEITEM()	{ if( pItemUnit ) { delete [] pItemUnit; } }
};

struct sMULTICHANGEITEM
{
	DWORD	wItemIdx;
	WORD	wLimitLevel;
	WORD	wMaxSet;
	unsigned int    nMaxItemSpace;
	sCHANGEITEM*	pChangeItem;
};

/*
struct sMULTICHANGEITEM
{
	WORD	wLimitLevel;
	WORD	wItemIdx;
	WORD	wMaxSingle;
	WORD	wMaxMulti;
	sCHANGEITEMUNIT*	pSingleItemUnit;	
	sCHANGEITEMUNIT*	pMultiItemUnit;
};
*/

class CChangeItemMgr  
{
	CYHHashTable<sCHANGEITEM>		m_ChangeItemList;
	CYHHashTable<sMULTICHANGEITEM>	m_MultiChangeItemList;	
	
public:
	MAKESINGLETON( CChangeItemMgr );
	CChangeItemMgr();
	virtual ~CChangeItemMgr();

	void	Init();
	void	Release();
	BOOL	LoadItemChange();

	int			 UseChangeItem( CPlayer* pPlayer, WORD TargetPos, DWORD wItemIdx );	
	int			 UseNormalChangeItem( CPlayer* pPlayer, WORD TargetPos, DWORD wItemIdx, ITEMBASE* pItem );
	int			 UseMultiChangeItem( CPlayer* pPlayer, WORD TargetPos, DWORD wItemIdx, ITEMBASE* pItem );
	unsigned int ChangedTotalItemNum(sMULTICHANGEITEM* pMultiChangeItem,WORD wMaxSet);//060613 Add by wonju

	// ShopItem
	int		UseChangeItemFromShopItem( CPlayer* pPlayer, DWORD wItemIdx );
	int		UseNormalChangeItemFromShopItem( CPlayer* pPlayer, DWORD wItemIdx );
	int		UseMultiChangeItemFromShopItem( CPlayer* pPlayer, DWORD wItemIdx );
	//

	sCHANGEITEMUNIT*	GetItemUnitFromCalPercent( DWORD wItemIdx );
	sCHANGEITEMUNIT*	GetMultiItemUnitFromCalPercent( sCHANGEITEM* pSet );
	sCHANGEITEMUNIT*    GetMaxSpaceItemRef(sCHANGEITEM* pSet , DWORD& nMin );
};

#endif // !defined(AFX_CHANGEITEMMGR_H__94A4A82B_1B66_44EE_9965_C0BAFC2B306C__INCLUDED_)

// DealItem.cpp: implementation of the CDealItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DealItem.h"
#include "ItemManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealItem::CDealItem()
{
	m_type = WT_DEALITEM;
}

CDealItem::~CDealItem()
{

}
DWORD CDealItem::GetSellPrice()
{
	ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(m_wItemIdx);
	if(pItemInfo)
		return pItemInfo->SellPrice;
	return 0;
}

DWORD CDealItem::GetBuyPrice()
{
	ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(m_wItemIdx);
	if(pItemInfo)
		return pItemInfo->BuyPrice;
	return 0;
}

DWORD CDealItem::GetBuyFishPoint()
{
	ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(m_wItemIdx);
	if(pItemInfo)
		return pItemInfo->dwBuyFishPoint;
	return 0;
}

char* CDealItem::GetItemName()
{
	ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(m_wItemIdx);
	if(pItemInfo)
		return pItemInfo->ItemName;
	return NULL;
}

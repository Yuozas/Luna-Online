// ItemContainer.cpp: implementation of the CItemContainer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemContainer.h"
#include "ItemSlot.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemContainer::CItemContainer()
{	
}

CItemContainer::~CItemContainer()
{
	Release();
}

void CItemContainer::Init()
{
	memset(m_TotalItemBase, 0, sizeof(ITEMBASE)*TP_MAX);
	memset(m_TotalSlotInfo, 0, sizeof(SLOTINFO)*TP_MAX);
	for( int i = 0 ; i < eItemTable_TableMax ; ++i )
		m_pItemSlot[i] = NULL;
}

void CItemContainer::SetInit(WORD tableIdx, POSTYPE StartPosition, POSTYPE slotNum, CItemSlot * pNewSlot)
{
	ASSERT(tableIdx < eItemTable_TableMax);
	m_pItemSlot[tableIdx] = pNewSlot;
	pNewSlot->Init(StartPosition, slotNum, m_TotalItemBase, m_TotalSlotInfo);
}

void CItemContainer::Release()
{
//	for( int i = 0 ; i < eItemTable_TableMax ; ++i )
//		SAFE_DELETE(m_pItemSlot[i]);
}

ITEMBASE const * CItemContainer::GetItemInfoAbs(POSTYPE absPos)
{
	if(absPos < TP_MAX)
		return &m_TotalItemBase[absPos];
	else
		return NULL;
}
/*
BOOL CItemContainer::InsertItemAbs(POSTYPE absPos, ITEMBASE * pItem)
{
	m_TotalItemBase[absPos] = *pItem;
}
BOOL CItemContainer::UpdateItemAbs(POSTYPE whatAbsPos, DWORD dwDBIdx, WORD wItemIdx, POSTYPE position, POSTYPE quickPosition, DURTYPE Dur, WORD flag=IUB_ALL)
{
	m_TotalItemBase[absPos] = *pItem;
}
BOOL CItemContainer::DeleteItemAbs(POSTYPE absPos, ITEMBASE * pItemOut, DWORD checkDBIdx)
{

}
*/
CItemSlot * CItemContainer::GetSlot(eITEMTABLE tableIdx)
{
	if(tableIdx < eItemTable_TableMax)
		return m_pItemSlot[tableIdx];
	else
		return NULL;
}
CItemSlot * CItemContainer::GetSlot(POSTYPE absPos)
{
	//if( TP_INVENTORY_START <= absPos && absPos < TP_INVENTORY_END )
	// 071210 KTH -- 확장인벤토리 까지 검사한다.
	if( TP_INVENTORY_START <= absPos && absPos < TP_EXTENDED_INVENTORY2_END )
	{	
		return m_pItemSlot[eItemTable_Inventory];
	}
	else if( TP_WEAR_START <= absPos && absPos < TP_WEAR_END )
	{
		return m_pItemSlot[eItemTable_Weared];
	}/*
	else if( TP_SKILL_START <= absPos && absPos < TP_SIMBUB_END )
	{
		
	}
	else if( TP_MUNPAWAREHOUSE_START <= absPos && absPos < TP_MUNPAWAREHOUSE_END )
	{
		return m_pItemSlot[eItemTable_MunpaWarehouse];
	}*/
	else if( TP_STORAGE_START <= absPos && absPos < TP_STORAGE_END )
	{
		return m_pItemSlot[eItemTable_Storage];
	}
	else if( TP_SHOPITEM_START <= absPos && absPos < TP_SHOPITEM_END )
	{
		return m_pItemSlot[eItemTable_Shop];
	}
	else if( TP_SHOPINVEN_START <= absPos && absPos < TP_SHOPINVEN_END )
	{
		return m_pItemSlot[eItemTable_ShopInven];
	}
	
	return NULL;
}

CPurse* CItemContainer::GetPurse(eITEMTABLE tableIdx)
{
	if(tableIdx < eItemTable_TableMax)
		return m_pItemSlot[tableIdx]->GetPurse();
	else
		return NULL;
}


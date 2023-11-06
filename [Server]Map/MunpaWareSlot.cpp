#include "stdafx.h"
#include "MunpaWareSlot.h"
#include "ItemManager.h"
#include "GuildManager.h"
#include "Player.h"
//#include "MapDBMsgParser.h"


CMunpaWareSlot::CMunpaWareSlot()
{}


CMunpaWareSlot::~CMunpaWareSlot()
{}


void CMunpaWareSlot::Init(POSTYPE startAbsPos, POSTYPE slotNum, ITEMBASE * pInfo, SLOTINFO * pSlotInfo)
{
	CItemSlot::Init(startAbsPos, slotNum, pInfo, pSlotInfo);
}


ERROR_ITEM CMunpaWareSlot::InsertItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItem, WORD state)
{
	// 로그를 위해 이전 상태를 저장해놓는다
	const POSTYPE previousPosition = pItem->Position;

	if( !IsPosIn( absPos ) )
	{
		return EI_OUTOFPOS;
	}

	// 071212 LUJ CItemSlot::InsertItemAbs() 참조
	//if( !IsEmptyInner(absPos) )
	if( ! IsEmpty( absPos ) )
	{
		return EI_EXISTED;
	}
	else if( !( SS_LOCKOMIT & state ) && IsLock(absPos) )
	{
		return EI_LOCKED;
	}
	else if( IsPassword(absPos) )
	{
		return EI_PASSWD;
	}

	//////////////////////////////////////////////////////////////////////////
	//itemmaxcheck
	if((state & SS_PREINSERT) == FALSE)	
		if( ITEMMGR->IsDupItem(m_ItemBaseArray[absPos - m_StartAbsPos].wIconIdx) && m_ItemBaseArray[absPos - m_StartAbsPos].Durability > MAX_YOUNGYAKITEM_DUPNUM )
			return EI_NOTEQUALDATA;
	//////////////////////////////////////////////////////////////////////////

	pItem->Position = absPos;

	m_ItemBaseArray[absPos - m_StartAbsPos] = *pItem;

	//---KES ItemDivide Fix 071020
	//---DBIDX체크 추가
	m_SlotInfoArray[absPos - m_StartAbsPos].wState = ( state & ~(SS_LOCKOMIT|SS_CHKDBIDX) );
	//----------------------------

	m_SlotInfoArray[absPos - m_StartAbsPos].bLock = FALSE;

	return EI_TRUE;
}


ERROR_ITEM CMunpaWareSlot::UpdateItemAbs(CPlayer * pPlayer, POSTYPE whatAbsPos, DWORD dwDBIdx, WORD wItemIdx, POSTYPE position, POSTYPE quickPosition, DURTYPE Dur, WORD flag, WORD state, DWORD RareDBIdx)
{
	if( !IsPosIn(whatAbsPos) )	return EI_OUTOFPOS;
	if( (flag & UB_ABSPOS) && (position != whatAbsPos) )
	{
		//YH
		//return EI_NOTEQUALDATA;
		position = whatAbsPos;
	}
	if( !( SS_LOCKOMIT & state ) && IsLock(whatAbsPos) )  return EI_LOCKED;
	if( IsPassword(whatAbsPos) ) return EI_PASSWD;

	//if(flag & UB_DBIDX)
	//m_ItemBaseArray[whatAbsPos - m_StartAbsPos].dwDBIdx = dwDBIdx;

	ITEMBASE& item = m_ItemBaseArray[whatAbsPos - m_StartAbsPos];

	if(flag & UB_ICONIDX)
		item.wIconIdx = wItemIdx;
	if(flag & UB_QABSPOS)
		item.QuickPosition = quickPosition;
	if(flag & UB_ABSPOS)
		item.Position = position;

	//////////////////////////////////////////////////////////////////////////
	//itemmaxcheck
	if( ITEMMGR->IsDupItem(item.wIconIdx) && item.Durability > MAX_YOUNGYAKITEM_DUPNUM )
		return EI_NOTEQUALDATA;
	//////////////////////////////////////////////////////////////////////////

	if(flag & UB_DURA)
		item.Durability = Dur;

	//---KES ItemDivide Fix 071020
	//---DBIDX체크 추가
	m_SlotInfoArray[whatAbsPos - m_StartAbsPos].wState = ( state & ~(SS_LOCKOMIT|SS_CHKDBIDX) );
	//----------------------------

	m_SlotInfoArray[whatAbsPos - m_StartAbsPos].bLock = FALSE;

	return EI_TRUE;
}


ERROR_ITEM CMunpaWareSlot::DeleteItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItemOut, WORD state)
{
	if( !IsPosIn(absPos) )
	{
		return EI_OUTOFPOS;
	}
	//else if( IsEmptyInner(absPos) )
	else if( IsEmpty( absPos ) )
	{
		return EI_NOTEXIST;
	}
	else if(	SS_LOCKOMIT != state &&
		IsLock(absPos) )
	{
		return EI_LOCKED;
	}
	else if( IsPassword(absPos) )
	{
		return EI_PASSWD;
	}

	ITEMBASE& item = m_ItemBaseArray[ absPos - m_StartAbsPos ];

	if( pItemOut )
	{
		*pItemOut = item;
	}

	SLOTINFO& slot = m_SlotInfoArray[ absPos - m_StartAbsPos ];

	ZeroMemory( &item, sizeof( item ) );
	ZeroMemory( &slot, sizeof( slot ) );

	//ClearItemBase(m_ItemBaseArray[absPos - m_StartAbsPos]);
	//ClearSlotInfo(m_SlotInfoArray[absPos - m_StartAbsPos]);

	return EI_TRUE;
}


BYTE CMunpaWareSlot::GetTabNum(POSTYPE absPos)
{
	return (absPos - m_StartAbsPos) / TABCELL_GUILDWAREHOUSE_NUM;
}


void CMunpaWareSlot::GetItemInfoTab(BYTE TabNum, ITEMBASE* pRtItemArray)
{
	int StartPos = TABCELL_GUILDWAREHOUSE_NUM * TabNum;
	for(int n=0;n<TABCELL_GUILDWAREHOUSE_NUM;++n)
	{
		pRtItemArray[n] = m_ItemBaseArray[StartPos+n];
	}
}


/*
071213 LUJ, 메소드를 사용하지 못하도록 주석 처리. 이 메소드를 사용하면, 해당 슬롯이 DB에서의 응답을 대기하는
SS_PREINSERT 플래그를 무시해버리기 때문에 동일 위치에 아이템이 겹쳐지게 할 수 있다(특히 DB랙이 있을 경우)
안전한 사용을 위해 IsEmpty()로 검사하고, InsertItemAbs( CPlayer*, const ITEMBASE& )로 갱신해야 한다
*/
//BOOL CMunpaWareSlot::IsEmptyInner(POSTYPE absPos)
//{
//	ASSERT( m_StartAbsPos <= absPos && absPos < m_StartAbsPos + m_SlotNum );
//
//	const int position = absPos - m_StartAbsPos;
//
//	if( 0 > position ||
//		m_SlotNum <= position )
//	{
//		ASSERT( 0 );
//		return FALSE;
//	}
//
//	const SLOTINFO& slot = m_SlotInfoArray[ position ];
//
//	if( SS_NONE != slot.wState )
//	{
//		return TRUE;
//	}
//	
//	const ITEMBASE& item = m_ItemBaseArray[ position ];
//
//	return ! item.dwDBIdx;
//}


BOOL CMunpaWareSlot::IsEmpty( POSTYPE absPos )
{
	return CItemSlot::IsEmpty( absPos - m_StartAbsPos );
}


BOOL CMunpaWareSlot::IsLock(POSTYPE absPos)
{
	return CItemSlot::IsLock( absPos - m_StartAbsPos );
}


BOOL CMunpaWareSlot::IsPassword(POSTYPE absPos)
{
	return CItemSlot::IsPassword( absPos - m_StartAbsPos );
}


void CMunpaWareSlot::SetLock(POSTYPE absPos, BOOL val)
{
	CItemSlot::SetLock( absPos - m_StartAbsPos, val );
}
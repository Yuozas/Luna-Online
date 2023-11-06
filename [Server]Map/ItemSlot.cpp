// ItemSlot.cpp: implementation of the CItemSlot class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemSlot.h"
#include "Purse.h"
#include "ItemManager.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define ClearItemBase(a)		a.dwDBIdx = 0;	\
								a.wIconIdx = 0;	\
								a.Position = 0;	\
								a.QuickPosition = 0;	\
								a.Durability = 0;	\
								a.nSealed = 0;	\
								a.nRemainSecond = 0;

#define ClearSlotInfo(b)		b.bLock = 0;	\
								b.wPassword = 0;	\
								b.wState = 0;
								
CItemSlot::CItemSlot()
{
	m_ItemBaseArray = NULL;
	m_SlotInfoArray = NULL;
	m_pPurse		= NULL;
}

CItemSlot::~CItemSlot()
{
	Release();
}

char * CItemSlot::_ToStringSlot()
{
	static char slotBuf[2512];
	slotBuf[0] = 0;
	char tmp[64];
	for( POSTYPE i = 0 ; i < m_SlotNum ; ++i )
	{
		sprintf(tmp, "DBIDX:%d ITEMIDX:%d POS:%d QPOS:%d DURA:%d LOCK:%d PWD:%d\n", 
							m_ItemBaseArray[i].dwDBIdx,
							m_ItemBaseArray[i].wIconIdx, 
							m_ItemBaseArray[i].Position, 
							m_ItemBaseArray[i].QuickPosition, 
							m_ItemBaseArray[i].Durability, 
							m_SlotInfoArray[i].bLock, 
							m_SlotInfoArray[i].wPassword);
		strcat(slotBuf, tmp);
		if( strlen(slotBuf) > 512 ) break;
	}

	return slotBuf;
}

void CItemSlot::Init(POSTYPE startAbsPos, POSTYPE slotNum, ITEMBASE * pInfo, SLOTINFO * pSlotInfo)
{
	m_StartAbsPos		= startAbsPos;
	m_SlotNum			= slotNum;
	m_ItemBaseArray		= pInfo;
	m_SlotInfoArray		= pSlotInfo;
}

BOOL CItemSlot::CreatePurse( CPurse* pNewPurse, void* pOwner, MONEYTYPE money, MONEYTYPE max)
{
	if(!m_pPurse)
	m_pPurse = pNewPurse;
	return m_pPurse->InitPurse( pOwner, money, max );
}

void CItemSlot::Release()
{
	m_pPurse		= NULL;
}

ITEMBASE const * CItemSlot::GetItemInfoAbs(POSTYPE absPos)
{ 
	if( !IsPosIn(absPos) )	return NULL;

	return  &m_ItemBaseArray[absPos];	
}


void CItemSlot::GetItemInfoAll(ITEMBASE * pItemBaseAll) const
{
	memcpy(pItemBaseAll, &m_ItemBaseArray[m_StartAbsPos], sizeof(ITEMBASE)*m_SlotNum);
}


void CItemSlot::SetItemInfoAll(ITEMBASE * pItemBaseAll)
{
	memcpy(&m_ItemBaseArray[m_StartAbsPos], pItemBaseAll, sizeof(ITEMBASE)*m_SlotNum);
}


ERROR_ITEM CItemSlot::InsertItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItem, WORD state)
{
	if( ! IsPosIn( absPos ) )
	{
		return EI_OUTOFPOS;
	}

	/*
	071211 LUJ

	if( !IsEmptyInner(absPos) )	return EI_EXISTED;
	
	문제점: 값이 삽입되는 슬롯은 상태를 SS_PREINSERT로 세팅시키고 DB에서 처리 결과가 올때까지 대기한다. 
			DB처리가 지연될 경우에도 예약되어 있으므로 해당 슬롯에 삽입을 못해야 정상이다. 그러나
			IsEmptyInner() 함수는 SS_NONE이 아니면 참을 반환하므로 SS_PREINSERT로 세팅한 것이 무효화된다.
			
	해결:	IsEmpty()를 쓰면 올바로 처리할 수 있다. 한편 DB 응답 후 처리는 InsertItemAbs( CPlayer*, ITEMBASE& )에서 한다
	의문:	왜 IsEmptyInner()를 썼는지는 알 수 없음. 아마도 DB에서 응답이 온 후에도 InsertItemAbs을 쓰고
			있기 때문이라고 추측됨. 
	*/
	if( ! IsEmpty( absPos ) )
	{
		return EI_EXISTED;
	}
	else if( !( SS_LOCKOMIT & state ) && IsLock( absPos ) )
	{
		return EI_LOCKED;
	}
	// 071227 LUJ, DB 응답 대기중인 슬롯은 쓰기 금지
	else if( SS_PREINSERT & m_SlotInfoArray[absPos].wState )
	{
		return EI_LOCKED;
	}
	else if( IsPassword(absPos) )
	{
		return EI_PASSWD;
	}
	
	//////////////////////////////////////////////////////////////////////////
	//itemmaxcheck
//	if((state & SS_PREINSERT) == FALSE)	
//	if( ITEMMGR->IsDupItem(m_ItemBaseArray[absPos].wIconIdx) && m_ItemBaseArray[absPos].Durability > MAX_YOUNGYAKITEM_DUPNUM )
//		return EI_NOTEQUALDATA;
	//////////////////////////////////////////////////////////////////////////

	m_ItemBaseArray[absPos] = *pItem;

	//---KES ItemDivide Fix 071020
	//---DBIDX체크 추가
	m_SlotInfoArray[absPos].wState = ( state & ~(SS_LOCKOMIT|SS_CHKDBIDX) );
	//----------------------------
	
	m_SlotInfoArray[absPos].bLock = FALSE;

	return EI_TRUE;
}

ERROR_ITEM CItemSlot::UpdateItemAbs(CPlayer * pPlayer, POSTYPE whatAbsPos, DWORD dwDBIdx, DWORD wItemIdx, POSTYPE position, POSTYPE quickPosition, DURTYPE Dur, WORD flag, WORD state, DWORD RareDBIdx)
{
	if( !IsPosIn(whatAbsPos) )	return EI_OUTOFPOS;
	if( (flag & UB_ABSPOS) && (position != whatAbsPos) )
	{
		//YH
		//return EI_NOTEQUALDATA;
		position = whatAbsPos;
	}
	if( !( SS_LOCKOMIT & state ) && IsLock(whatAbsPos) )  return EI_LOCKED;

	//---KES ItemDivide Fix 071020
	//---DBIDX가 같은지 체크
	if( SS_CHKDBIDX & state )
	{
		if( dwDBIdx != m_ItemBaseArray[whatAbsPos].dwDBIdx )
			return EI_NOTEQUALDATA;
	}
	 // 071227 LUJ, DB 응답 대기중인 슬롯은 쓰기 금지
	else if( SS_PREINSERT & m_SlotInfoArray[ whatAbsPos ].wState )
	{
		return EI_LOCKED;
	}
	
	if( IsPassword(whatAbsPos) ) return EI_PASSWD;

	//---KES ItemDivide Fix 071020
	//---주석. DBIDX를 업데이트할 일이 없다.
	//if(flag & UB_DBIDX)
	//m_ItemBaseArray[whatAbsPos].dwDBIdx = dwDBIdx;
	//----------------------------
	if(flag & UB_ICONIDX)
	m_ItemBaseArray[whatAbsPos].wIconIdx = wItemIdx;
	if(flag & UB_QABSPOS)
	m_ItemBaseArray[whatAbsPos].QuickPosition = quickPosition;
	if(flag & UB_ABSPOS)
	m_ItemBaseArray[whatAbsPos].Position = position;
	
	//////////////////////////////////////////////////////////////////////////
	//itemmaxcheck
	if( ITEMMGR->IsDupItem(m_ItemBaseArray[whatAbsPos].wIconIdx) && m_ItemBaseArray[whatAbsPos].Durability > MAX_YOUNGYAKITEM_DUPNUM )
		return EI_NOTEQUALDATA;
	//////////////////////////////////////////////////////////////////////////
	
	if(flag & UB_DURA)
	m_ItemBaseArray[whatAbsPos].Durability = Dur;
	
	//---KES ItemDivide Fix 071020
	//---DBIDX체크 추가
	m_SlotInfoArray[whatAbsPos].wState = ( state & ~(SS_LOCKOMIT|SS_CHKDBIDX) );
	//----------------------------

	m_SlotInfoArray[whatAbsPos].bLock = FALSE;

	return EI_TRUE;
}

ERROR_ITEM CItemSlot::DeleteItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItemOut, WORD state)
{
	if( !IsPosIn(absPos) )	return EI_OUTOFPOS;
	//if( IsEmptyInner(absPos) )	return EI_NOTEXIST;
	if( IsEmpty( absPos ) )
	{
		return EI_NOTEXIST;
	}

	if( !(absPos >= TP_SHOPITEM_START && absPos < TP_SHOPITEM_END) &&
		!(absPos >= TP_SHOPINVEN_START && absPos < TP_SHOPINVEN_END) )
		if( !( SS_LOCKOMIT & state ) && IsLock(absPos) )  return EI_LOCKED;
	if( IsPassword(absPos) )
	{
		return EI_PASSWD;
	}
	// 071227 LUJ, DB 응답 대기중인 슬롯은 쓰기 금지
	else if( SS_PREINSERT & m_SlotInfoArray[ absPos ].wState )
	{
		return EI_LOCKED;
	}

	if(pItemOut)
		*pItemOut = m_ItemBaseArray[absPos];

	ClearItemBase(m_ItemBaseArray[absPos]);
	ClearSlotInfo(m_SlotInfoArray[absPos]);
	return EI_TRUE;
}

/* Position이 현재 Slot에 포함되는 Position인지 Check*/
BOOL CItemSlot::IsPosIn(POSTYPE absPos)
{
	if( m_StartAbsPos > absPos || absPos >= m_StartAbsPos + m_SlotNum )
		return FALSE;
	else
		return TRUE;
}


/*
071213 LUJ, 메소드를 사용하지 못하도록 주석 처리. 이 메소드를 사용하면, 해당 슬롯이 DB에서의 응답을 대기하는
			SS_PREINSERT 플래그를 무시해버리기 때문에 동일 위치에 아이템이 겹쳐지게 할 수 있다(특히 DB랙이 있을 경우)
			안전한 사용을 위해 IsEmpty()로 검사하고, InsertItemAbs( CPlayer*, const ITEMBASE& )로 갱신해야 한다
*/
//BOOL CItemSlot::IsEmptyInner(POSTYPE absPos)
//{
////	char temp[1024];
////	sprintf(temp, "Start:%d, End:%d, Cur:%d", m_StartAbsPos, m_StartAbsPos+m_SlotNum, absPos);
////	ASSERTMSG( m_StartAbsPos <= absPos && absPos < m_StartAbsPos + m_SlotNum, temp ); 
//
//	if( m_SlotInfoArray[absPos].wState != SS_NONE )
//		return TRUE;
//	else if( m_ItemBaseArray[absPos].dwDBIdx == 0 ) 
//		return TRUE;
//	else
//		return FALSE;
//}


BOOL CItemSlot::IsEmpty(POSTYPE absPos)
{
//	char temp[1024];
//	sprintf(temp, "Start:%d, End:%d, Cur:%d", m_StartAbsPos, m_StartAbsPos+m_SlotNum, absPos);
//	ASSERTMSG( m_StartAbsPos <= absPos && absPos < m_StartAbsPos + m_SlotNum, temp ); 

	if( !m_SlotInfoArray[absPos].bLock && m_SlotInfoArray[absPos].wState == SS_NONE && m_ItemBaseArray[absPos].dwDBIdx == 0)
		return TRUE;
	else
		return FALSE;
}
BOOL CItemSlot::IsLock(POSTYPE absPos)
{
//	char temp[1024];
//	sprintf(temp, "Start:%d, End:%d, Cur:%d", m_StartAbsPos, m_StartAbsPos+m_SlotNum, absPos);
//	ASSERTMSG( m_StartAbsPos <= absPos && absPos < m_StartAbsPos + m_SlotNum, temp); 

	if( m_SlotInfoArray[absPos].bLock )
		return TRUE;
	else
		return FALSE;
}
BOOL CItemSlot::IsPassword(POSTYPE absPos)
{
//	char temp[1024];
//	sprintf(temp, "Start:%d, End:%d, Cur:%d", m_StartAbsPos, m_StartAbsPos+m_SlotNum, absPos);
//	ASSERTMSG( m_StartAbsPos <= absPos && absPos < m_StartAbsPos + m_SlotNum, temp ); 

	if( m_SlotInfoArray[absPos].wPassword ) 
		return TRUE;
	else
		return FALSE;
}
void CItemSlot::SetLock(POSTYPE absPos, BOOL val)
{
//	char temp[1024];
//	sprintf(temp, "Start:%d, End:%d, Cur:%d", m_StartAbsPos, m_StartAbsPos+m_SlotNum, absPos);
//	ASSERTMSG( m_StartAbsPos <= absPos && absPos < m_StartAbsPos + m_SlotNum, temp); 

	m_SlotInfoArray[absPos].bLock = val;
}
/*
MONEYTYPE CItemSlot::Addition(MONEYTYPE money)
{
	if( !m_pPurse ) return 0;
	return m_pPurse->Addition(money);
}

MONEYTYPE CItemSlot::Subtraction(MONEYTYPE money)
{
	if( !m_pPurse ) return 0;
	return m_pPurse->Subtraction(money);
}
*/
MONEYTYPE CItemSlot::GetMoney()
{
	if( !m_pPurse ) return 0;
	return m_pPurse->GetPurseCurMoney();
}

WORD CItemSlot::GetItemCount()
{
	POSTYPE EndPos = m_StartAbsPos + m_SlotNum;
	WORD ItemNum = 0;

	for( WORD i = m_StartAbsPos ; i < EndPos ; ++i )
	{
		if( IsEmpty(i) ) continue;

		++ItemNum;
	}

	return ItemNum;	
}


ERROR_ITEM CItemSlot::InsertItemAbsFromDb( CPlayer* player, const ITEMBASE& item )
{
	if( ! IsPosIn( item.Position ) )
	{
		return EI_OUTOFPOS;
	}

	SLOTINFO& slot = m_SlotInfoArray[ item.Position ];

	if( ! ( slot.wState & SS_PREINSERT ) )
	{
		ASSERT( 0 );
		return EI_EXISTED;
	}	
	else if( IsPassword(item.Position) )
	{
		return EI_PASSWD;
	}

	slot.wState &=	~( SS_PREINSERT | SS_LOCKOMIT | SS_CHKDBIDX );
	slot.bLock	=	FALSE;

	m_ItemBaseArray[ item.Position ] = item;
	
	return EI_TRUE;
}

// 080621 LYW --- ItemSlot : 아이템과 슬롯 정보를 초기화 하는 함수 추가.
void CItemSlot::ClearItemBaseAndSlotInfo(POSTYPE absPos)
{
	if( !IsPosIn(absPos) )	return ;

	ClearItemBase(m_ItemBaseArray[absPos]) ;
	ClearSlotInfo(m_SlotInfoArray[absPos]) ;
}

void CItemSlot::ForcedUnseal(POSTYPE absPos)
{
	m_ItemBaseArray[absPos].nSealed = 0;
}

#pragma once


//enum UPDATE_BIT{ UB_DBIDX = 1, UB_ICONIDX=2, UB_ABSPOS=4, UB_QABSPOS=8, UB_DURA=16, UB_ALL=31, };
//SW050920 Rare
enum UPDATE_BIT{ UB_DBIDX = 1, UB_ICONIDX=2, UB_ABSPOS=4, UB_QABSPOS=8, UB_DURA=16, UB_RARE=32, UB_ALL=63, };
enum ERROR_ITEM { EI_TRUE = 0, EI_OUTOFPOS = 1, EI_NOTEQUALDATA, EI_EXISTED, EI_NOTEXIST, EI_LOCKED, EI_PASSWD, EI_NOTENOUGHMONEY, EI_NOSPACE, EI_MAXMONEY };

// SS_LOCKOMIT �� ������ �Լ� �Ŀ� ���� Ǯ����. 
// ���� : Updateitemabs�� ���� Ǯ����!
//---KES StreetStall Fix 071020
enum SLOT_STATE { SS_NONE = 0, SS_PREINSERT=1, SS_LOCKOMIT = 2, SS_CHKDBIDX = 4,};
//----------------------------

class CPlayer;
class CPurse;
// 071226 LUJ
class CBackupSlot;

class CItemSlot  
{
	friend class CBackupSlot;

public:
	CItemSlot();
	virtual ~CItemSlot();

	virtual void Init(POSTYPE startAbsPos, POSTYPE slotNum, ITEMBASE * pInfo, SLOTINFO * pSlotInfo);
	void Release();
	
	virtual ITEMBASE const * GetItemInfoAbs(POSTYPE absPos);
	void GetItemInfoAll(ITEMBASE * pItemBaseAll) const;
	void SetItemInfoAll(ITEMBASE * pItemBaseAll);

	char * _ToStringSlot();

	//virtual ERROR_ITEM InsertItemAbs(POSTYPE absPos, ITEMBASE * pItem, WORD state=SS_NONE);
	//virtual ERROR_ITEM UpdateItemAbs(POSTYPE whatAbsPos, DWORD dwDBIdx, WORD wItemIdx, POSTYPE position, POSTYPE quickPosition, DURTYPE Dur, WORD flag=UB_ALL);
	//virtual ERROR_ITEM DeleteItemAbs(POSTYPE absPos, ITEMBASE * pItemOut);

	//virtual ERROR_ITEM UpdateItemAbs(CPlayer*, POSTYPE whatAbsPos, DWORD dwDBIdx, WORD wItemIdx, POSTYPE position, POSTYPE quickPosition, DURTYPE Dur, WORD flag=UB_ALL, WORD state=SS_NONE);
	//SW050920 Rare
	virtual ERROR_ITEM UpdateItemAbs(CPlayer*, POSTYPE whatAbsPos, DWORD dwDBIdx, DWORD wItemIdx, POSTYPE position, POSTYPE quickPosition, DURTYPE Dur, WORD flag=UB_ALL, WORD state=SS_NONE, DWORD RareDBIdx = 0);
	virtual ERROR_ITEM InsertItemAbs(CPlayer*, POSTYPE absPos, ITEMBASE * pItem, WORD state = SS_NONE );
	virtual ERROR_ITEM DeleteItemAbs(CPlayer*, POSTYPE absPos, ITEMBASE * pItemOut, WORD state = SS_NONE);

	// 071212 LUJ, DB���� ó���� ����� ������Ʈ�ϴ� �Լ�
	virtual ERROR_ITEM InsertItemAbsFromDb( CPlayer*, const ITEMBASE& );
	
	POSTYPE GetStartPos()	const { return m_StartAbsPos; }
	POSTYPE GetSlotNum()	const { return m_SlotNum; }
	inline virtual BOOL IsEmpty(POSTYPE absPos);

	CPurse*	GetPurse() { return m_pPurse; }
	MONEYTYPE GetMoney();

//	virtual MONEYTYPE Addition(MONEYTYPE money);
//	virtual MONEYTYPE Subtraction(MONEYTYPE money);

	/* purse func */
	BOOL CreatePurse( CPurse* pNewPurse, void* pOwner, MONEYTYPE money, MONEYTYPE max = MAXMONEY );

	virtual BOOL IsAddable(POSTYPE absPos, ITEMBASE * pItemBase){	return TRUE;	}
	virtual WORD GetEmptyCell(WORD * EmptyCellPos, WORD NeedCount = 1) { return 0; }
	virtual void SetLock(POSTYPE absPos, BOOL val);
	virtual BOOL IsLock(POSTYPE absPos);

	virtual WORD GetItemCount();

	// RaMa	
	virtual BOOL CheckQPosForItemIdx( DWORD ItemIdx )		{	return TRUE;	}
	virtual BOOL CheckItemLockForItemIdx( DWORD ItemIdx )	{	return TRUE;	}


	// 071225 KTH -- �ִ� ���� ������ ���� �Ҽ� �ֵ��� ����.
	void SetSlotNum(POSTYPE slotNum)	{	m_SlotNum = slotNum;	}

	// 080621 LYW --- ItemSlot : �����۰� ���� ������ �ʱ�ȭ �ϴ� �Լ� �߰�.
	void ClearItemBaseAndSlotInfo(POSTYPE absPos) ;

	void ForcedUnseal(POSTYPE absPos);
	
protected:
	//virtual inline BOOL IsEmptyInner(POSTYPE absPos);
	virtual inline BOOL IsPosIn(POSTYPE absPos);
	virtual inline BOOL IsPassword(POSTYPE absPos);

	ITEMBASE *		m_ItemBaseArray;
	SLOTINFO *		m_SlotInfoArray;
	POSTYPE			m_StartAbsPos;
	POSTYPE			m_SlotNum;						// Slot�� ����

	/* purse value */
	CPurse*			m_pPurse;
};


// 071226 LUJ, ���� ������ �����صΰ� �����ϰų� ���ϱ� ���� ���
class CBackupSlot
{
public:
	CBackupSlot( CItemSlot& slot ) :
		mSlot( slot ),
		mItemArray		( new ITEMBASE[ slot.GetSlotNum() ] ),
		mSlotInfoArray	( new SLOTINFO[ slot.GetSlotNum() ] )
	{
		memcpy( mItemArray,		slot.m_ItemBaseArray, sizeof( *slot.m_ItemBaseArray ) * slot.m_SlotNum );
		memcpy( mSlotInfoArray,	slot.m_SlotInfoArray, sizeof( *slot.m_SlotInfoArray ) * slot.m_SlotNum );
	}

	~CBackupSlot()
	{
		SAFE_DELETE_ARRAY( mItemArray );
		SAFE_DELETE_ARRAY( mSlotInfoArray );
	}

	void Restore() const
	{
		memcpy( mSlot.m_ItemBaseArray, mItemArray,		sizeof( *mSlot.m_ItemBaseArray ) * mSlot.m_SlotNum );
		memcpy( mSlot.m_SlotInfoArray, mSlotInfoArray,	sizeof( *mSlot.m_SlotInfoArray ) * mSlot.m_SlotNum );
	}

	const ITEMBASE* GetItem( POSTYPE position ) const
	{
		if( ! mSlot.IsPosIn( position ) )
		{
			return 0;
		}

		// �� �����Ϳ� ���� �� �˻�� �ǹ̰� ����
		const ITEMBASE* item = mItemArray + position;

		return item->dwDBIdx ? item : 0;
	}


private:
	CItemSlot&	mSlot;
	ITEMBASE*	mItemArray;
	SLOTINFO*	mSlotInfoArray;
};

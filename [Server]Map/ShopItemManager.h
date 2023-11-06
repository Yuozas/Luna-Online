// ShopItemManager.h: interface for the CShopItemManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOPITEMMANAGER_H__28EE3E98_5701_4E5B_B6E8_242524221154__INCLUDED_)
#define AFX_SHOPITEMMANAGER_H__28EE3E98_5701_4E5B_B6E8_242524221154__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CPlayer;
struct ITEM_INFO;


class CShopItemManager  
{
	CYHHashTable<SHOPITEMWITHTIME>			m_UsingItemTable;
	CMemoryPoolTempl<SHOPITEMWITHTIME>*		m_UsingItemPool;
	CYHHashTable<MOVEDATA>					m_MovePointTable;
	CMemoryPoolTempl<MOVEDATA>*				m_MovePointPool;

	CPlayer*	m_pPlayer;
	DWORD		m_Checktime;
	DWORD		m_Updatetime;

	//
	DWORD		m_DupIncantation;
	DWORD		m_DupCharm;
	DWORD		m_DupHerb;
	DWORD		m_DupSundries;

	//
	DWORD		m_ProtectItemIdx;

public:
	CShopItemManager();
	virtual ~CShopItemManager();

	void Init(CPlayer* pPlayer);
	void Release();
	void CheckEndTime();
	int UseShopItem( ITEMBASE* pItemBase, SHOPITEMUSEBASE UseBaseInfo, SHOPITEMBASE* pShopItem );
	BOOL UsedShopItem( ITEMBASE* pItemBase, DWORD Param, stTIME BeginTime, DWORD RemainTime );
	BOOL CalcShopItemOption( DWORD wIdx, BOOL bAdd, DWORD Param=0 );

	//
	void DeleteUsingShopItem( DWORD ItemIdx, BOOL bDelete=FALSE );
	void DeleteUsingShopItemInfo( DWORD ItemIdx );
	void SendMsgDwordToPlayer( DWORD Msg, DWORD Param );
	void SendShopItemUseInfo();
	void UpdateLogoutToDB();

	//
	BOOL AddMovePoint( MOVEDATA* pData );
	BOOL DeleteMovePoint( DWORD DBIdx );
	BOOL ReNameMovePoint( DWORD DBIdx, char* newName );
	void ReleseMovePoint();

	//
	SHOPITEMWITHTIME* GetUsingItemInfo( DWORD ItemIdx );

	DWORD GetSavedMPNum()		{	return m_MovePointTable.GetDataNum();	}

	//
	BOOL PutOnAvatarItem( DWORD ItemIdx, WORD ItemPos, BOOL bCalcStats=TRUE );
	void TakeOffAvatarItem( DWORD ItemIdx, WORD ItemPos, BOOL bCalcStats=TRUE );
	void AddShopItemUse( SHOPITEMBASE* pShopItem );
	void DiscardAvatarItem( DWORD ItemIdx, WORD ItemPos );
	void CheckAvatarEndtime();

	//
	void CalcAvatarOption( BOOL bCalcStats=TRUE );
	//void CalcPlusTime( DWORD dwEventIdx, DWORD dwType );

	//
	void AddDupParam( ITEM_INFO* pItemInfo );
	void DeleteDupParam( ITEM_INFO* pItemInfo );
	BOOL IsDupAble( ITEM_INFO* pItemInfo );

	// Protect Item
	DWORD GetProtectItemIdx()					{	return m_ProtectItemIdx;	}
	void SetProtectItemIdx( DWORD ItemIdx )		{	m_ProtectItemIdx = ItemIdx;		}
};



#endif // !defined(AFX_SHOPITEMMANAGER_H__28EE3E98_5701_4E5B_B6E8_242524221154__INCLUDED_)

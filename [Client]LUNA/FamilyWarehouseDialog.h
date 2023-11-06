// FamilyWarehouseDialog.h: interface for the CFamilyWarehouseDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FAMILYWAREHOUSEDIALOG_H__BEBB6A0F_CFBE_4047_A432_98F3EB5B1271__INCLUDED_)
#define AFX_FAMILYWAREHOUSEDIALOG_H__BEBB6A0F_CFBE_4047_A432_98F3EB5B1271__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INTERFACE\cTabDialog.h"

class CItem;
class cStatic;

class CFamilyWarehouseDialog : public cTabDialog  
{
	BOOL m_Lock;
	cStatic * m_pFamilyMoney;
	BOOL m_InitInfo;
public:
	CFamilyWarehouseDialog();
	virtual ~CFamilyWarehouseDialog();
	virtual void SetActive(BOOL val);
	virtual void Render();
	virtual void Add(cWindow * window);
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);
	void OnActionEvent(LONG lId, void * p, DWORD we);

	void Linking();
	BOOL FakeMoveItem( LONG mouseX, LONG mouseY, CItem * pFromItem );
	BOOL FakeGeneralItemMove( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem );
	BOOL CanBeMoved(CItem* pItem,POSTYPE toPos);
	BOOL GetPositionForXYRef(BYTE param, LONG x, LONG y, WORD& pos);
	CItem * GetItemForPos(POSTYPE absPos);
	
	BOOL	AddItem(ITEMBASE * itemBase);
	BOOL	AddItem(CItem* pItem);
	BOOL	DeleteItem(POSTYPE Pos,CItem** ppItem);
	POSTYPE GetRelativePosition( POSTYPE absPos);
	WORD GetTabIndex( POSTYPE absPos );

	void SetLock(BOOL val);
	BOOL GetLock();

	void SetWarehouseInfo(SEND_MUNPA_WAREHOUSE_ITEM_INFO* pInfo);
	
	void SetMoney(MONEYTYPE money);

	void PutInOutMoney(int vals);
	void PutInOutMoneyResult(MSG_DWORD3 * msg);

	static void OnPutInMoneyFamilyOk( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 );
	static void OnPutOutMoneyFamilyOk( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 );	
};

#endif // !defined(AFX_FAMILYWAREHOUSEDIALOG_H__BEBB6A0F_CFBE_4047_A432_98F3EB5B1271__INCLUDED_)

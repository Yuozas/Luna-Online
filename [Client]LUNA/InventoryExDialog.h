#pragma once


#include "./Interface/cTabDialog.h"
//#include "WindowIDEnum.h"
//#include ".\interface\cWindowHeader.h"

#include "WearedExDialog.h"
#include "ItemShopInven.h"

//#define CELLNUMPERPAGE				20

// 070423 LYW --- InventoryExDialog : Define max count of background.
#define MAX_BACKGROUND	4
#define EXTENDED_INVENTORY_END		(TP_INVENTORY_END + ( TABCELL_INVENTORY_NUM * HERO->Get_HeroExtendedInvenCount() ))

class CWearedExDialog;
class CItem;
class cSkillBase;
class CDealItem;
class cStatic;
class cMsgBox;
class cPushupButton;


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//	�κ��丮 : ���� ������ ���� ������ġ�� �����ġ�� ����� ������
//	����â	 : WEARED_STARTPOSITION ~ WEARED_ENDPOSITION�� ��ġ�� ����
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
enum { INV=0, WEAR=1, SHOPIVN=2, };
enum eINVENTORY_STATE { eITEMDEFAULT_STATE, eITEMUPGRADE_STATE, eITEMDEAL_STATE, };
class CInventoryExDialog : public cTabDialog  
{
	// ��Ȱ�ֹ��� ����
	CItem*		m_pSelectedShopItem;
	REVIVEDATA	m_ReviveData;

public:
	CInventoryExDialog();
	virtual ~CInventoryExDialog();
	virtual void Render();
	virtual void Add(cWindow * window);
	virtual DWORD ActionEvent(CMouse * mouseInfo);

	BOOL	AddItem(ITEMBASE * itemBase);
	BOOL	AddItem(CItem* pItem);
	BOOL	DeleteItem(POSTYPE Pos,CItem** ppItem);
	void	MoveItem(POSTYPE FromPos, POSTYPE ToPos);
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);

	BOOL	GetBlankPositionRestrictRef(WORD & absPos);
	BOOL	CheckItemLockForItemIdx(DWORD wItemIdx);

	void	Linking();

	BOOL	GetPositionForXYRef(BYTE param, LONG x, LONG y, WORD& pos);
	
	void	SetMoney(DWORD Money);
	BOOL	IsExist(POSTYPE abs_pos);
	BOOL	CanBeMoved(CItem* pItem,POSTYPE pos);
	
	void	OnActionEvnet(LONG lId, void * p, DWORD we);
	

	void	UseItem(CItem* pItem);
	// data ope
	CItem * GetItemForPos(POSTYPE absPos);
	CItem * GetCurSelectedItem(BYTE type);								// inv(0), wear(1), SHOPIVN(2)
	BOOL	UpdateItemDurabilityAdd(POSTYPE absPos, int dur);				// ���� ������ �ߺ�ó���� ����(-)�� �ü� �� ����: ������ ������� ��
	void	SetState(eINVENTORY_STATE state){ m_iInventoryState = state; }
	WORD	GetTabIndex(POSTYPE absPos);


	void	RefreshInvenItem();	//080424 NYJ --- �κ��丮 �������� ����
	
//KES EXCHNAGE 030929
	int		GetBlankNum();
//
//KES 040316 ����ȯ �ε����� ����	
	void	ReleaseInventory();

	virtual void SetActive(BOOL val);


	DURTYPE	GetTotalItemDurability(DWORD wItemIdx);	
	CItem * GetItemForIdx(DWORD wItemIdx);

	// RaMa - 04.12.09
	void OnShopItemBtn();
	void OffShopItemBtn(LONG lId);
	void ItemUnSealingSync();
	void ItemUnSealing(DWORD absPos);
	void SetReviveData( DWORD ID, DWORD ItemIdx, DWORD ItemPos )
	{	m_ReviveData.TargetID = ID;		m_ReviveData.ItemIdx = (WORD)ItemIdx;		m_ReviveData.ItemPos = (POSTYPE)ItemPos;		}
	REVIVEDATA GetReviveData()			{	return m_ReviveData;	}
	void SetShopItemInvenBtn( BOOL val );
	CItem*	GetSelectedShopItem()		{	return m_pSelectedShopItem;	}
	void ResetSelectedShopItem()		{	m_pSelectedShopItem = NULL;	}
	CItemShopInven* GetShopInven()		{	return m_pItemShopInven;	}


private:
	void	FakeBuyItem(LONG mouseX, LONG mouseY, CDealItem * icon);
	BOOL	IsDivideFunctionKey();
	BOOL	FakeMoveItem(LONG mouseX, LONG mouseY, CItem * icon);				// network syn msg send
	BOOL	FakeGeneralItemMove( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem );
	BOOL	FakeItemCombine( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem );
	BOOL	FakeItemDivide( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem );
	static void OnFakeItemDivideOk( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 );
	static void OnFakeItemDivideCancel( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 );

	CWearedExDialog		* m_pWearedDlg;
	cStatic				* m_pMoneyEdit;
	// RaMa -04.12.09
	CItemShopInven*		m_pItemShopInven;
	cPushupButton*		m_pItemShopBtn;
	BOOL				m_bShopInven;

	BOOL				m_fSpread;
	eINVENTORY_STATE	m_iInventoryState;

	MSG_ITEM_DIVIDE_SYN	m_divideMsg;

//��ưǪ�� ó��
	DWORD	m_BtnPushstartTime;	
	DWORD	m_BtnPushDelayTime;
	BOOL	m_bPushTabWithMouseOver;
	
// quest
public:
	cMsgBox*			m_pQuestMsgBox;
	CItem*				m_pTempQuestItem;
	void	ItemUseForQuestStart();

// 070326 LYW --- InventoryExDialog : npc�� �ŷ��� ���� ��ġ ����.
public :
	VECTOR2				m_PrevPos ;

	void SetPrevPos( VECTOR2* prevPos ) ;
	VECTOR2* GetPrevPos() { return &m_PrevPos ; }


private :
	cStatic*			m_pBackground[MAX_BACKGROUND] ;
public :
	void SetActiveBackground(BYTE idx) ;
	void CheckBackground() ;

	// 070917 ����, ��Ÿ�� ����
	void SetCoolTime( DWORD itemIndex, DWORD miliSecond );
	// 080326 NYJ --- ���������� ��Ÿ�Ӱ���.
	void SetCoolTimeAbs( DWORD itemIndex, DWORD dwBegin, DWORD dwEnd, DWORD miliSecond );

	
	// 070927 ��ȯ ������ ó��
public:
	void Send();
	void Restore();

//---KES ��� ���� ���� ���� 071128
	void RefreshArmorItem()	{ if( m_pWearedDlg ) m_pWearedDlg->RefreshArmorItem(); }
//---KES ��� ���� ���� ���� 071128

private:
	MSG_ITEM_USE_SYN mMessage;
	// 071204 LYW --- InventoryExDialog : �� ���� ����ϰ�, �κ��� ���� �� ���� �� ��� �� �ӽ� ���� ����.
	CItem* m_pQuickSelectedItem ;
public : 
	void Set_QuickSelectedItem(CItem* pItem) { m_pQuickSelectedItem = pItem ; }
	CItem* Get_QuickSelectedItem() { return m_pQuickSelectedItem ; }

	// 071210 LYW --- CInventoryExDialog : Ȯ�� �κ��丮�� Ȱ��ȭ ���θ� �����ϴ� �Լ� �߰�.
	void InitExtended_Inventory( CHARACTER_TOTALINFO* pInfo ) ;

	// 080213 KTH -- ClearInventory
	//BOOL ClearInventory();

	void SetMultipleItemLock(DWORD dwItemIdx, BOOL bVal);	// 080429 NYJ --- ������ ������ Lockó��

	void PetGradeUp();
};

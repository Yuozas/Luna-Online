#pragma once
#include ".\interface\cdialog.h"

class cQuickSlotDlg;
class CQuickItem;

class cQuickDlg :
	public cDialog
{
	cQuickSlotDlg* mpSlot;

	// 080702 LYW --- QuickDlg : Ȯ�� ���� �߰�.
	cQuickSlotDlg* mpExSlot ;
	cButton*	   mpExtendBtn ;
	

public:
	cQuickDlg(void);
	virtual ~cQuickDlg(void);

	void Linking();

	void SetActiveSheet( WORD page1, WORD page2 );

	void OnActionEvnet(LONG lId, void * p, DWORD we);

	void AddItem( WORD tab, WORD pos, CQuickItem* pItem );
	void RemoveItem( WORD tab, WORD pos );
	void ChangeItem( WORD tab1, WORD pos1, WORD tab2, WORD pos2 );

	cQuickSlotDlg* GetSlotDlg( WORD num );
	// 080702 LYW --- QuickDlg : Ȯ�� ���� �߰� ó��.
	cQuickSlotDlg* GetExSlotDlg( WORD num ) ;

	// 080703 LYW --- QuickDlg : Ȯ�� ���� ��� �Լ� �߰�.
	void Toggle_ExSlot() ;
};

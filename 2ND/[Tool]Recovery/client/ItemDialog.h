#pragma once
#include "afxwin.h"
#include "afxcmn.h"


class CclientApp;


// CItemDialog ��ȭ �����Դϴ�.

class CItemDialog : public CDialog
{
	DECLARE_DYNAMIC(CItemDialog)

public:
	CItemDialog( CclientApp&, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CItemDialog();

	virtual void OnFinalRelease();

	void Parse( DWORD serverIndex, const MSGROOT* );
	void Request( DWORD serverIndex, const CString& playerName, DWORD playerIndex, DWORD userIndex );

	//DWORD GetEmptyPosition() const;
	
	void AddItem( const Item& );
	

protected:
	void AddItem( const Item&, CListCtrl& );

	void RefreshInventory();
	void ResetGrid();
	

protected:
	CclientApp&	mApplication;
	//CListCtrl	mInventoryListCtrl;
	//CListCtrl	mStorageListCtrl;

	// ���� �� �������� �Ϲ����� ������ ǥ��;
	CGridCtrl	mItemGrid;

	// ������ �ɼ� ������ ǥ��
	CGridCtrl	mOptionGrid;

	DWORD		mServerIndex;
	DWORD		mSelectedItemDbIndex;

	CString		mPlayerName;
	DWORD		mPlayerIndex;
	DWORD		mUserIndex;
	
	DWORD		mStorageSize;
	DWORD		mExtendedInventorySize;

	// ���õ� �����͸� ǥ���Ѵ�
	void PutData( DWORD itemDbIndex );
	ITEMBASE*	GetItem( DWORD itemDbIndex );
	CString		GetDateString( DWORD second ) const;
	
	//DWORD		mSelectedPosition;

	// Ű: ������ DB�ε���
	typedef stdext::hash_map< DWORD, Item >	ItemMap;
	ItemMap									mItemMap;
	
	// Ű: ������ DB�ε���
	typedef stdext::hash_map< DWORD, ITEM_OPTION >	OptionMap;
	OptionMap										mOptionMap;

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ITEMDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnHdnItemclickItemDialogInventory(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedItemDialogInventory(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnBnClickedItemDialogDataUpdate();
	afx_msg void OnBnClickedItemDialogDataUpdateButton();
	//afx_msg void OnLvnItemchangedItemStorage(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedItemDataDeleteButton();
	//CStatic mPlayerIndexEdit;
	CStatic mDataFrameStatic;
	afx_msg void OnBnClickedItemInventoryLogButton();
	//afx_msg void OnBnClickedItemWarehouseLogButton2();
	CListCtrl mInventoryListCtrl;
	afx_msg void OnBnClickedItemAddButton();
	CStatic mDataFrame;
	CEdit mEndTimeStatic;
	CEdit mRemainTimeStatic;
	//afx_msg void OnEnSetfocusItemEndTimeStatic();
	//afx_msg void OnEnSetfocusItemRemainTimeStatic();
	afx_msg void OnBnClickedItemEndTimeButton();
	afx_msg void OnBnClickedItemRemainTimeButton();
	CComboBox mStorageSizeCombo;
	CEdit mStorageMoneyEdit;
	CTabCtrl mTabCtrl;
	afx_msg void OnTcnSelchangeItemTab(NMHDR *pNMHDR, LRESULT *pResult);
	CButton mItemAddButton;
	afx_msg void OnBnClickedItemStorageUpdateButton();
	CButton mStorageUpdateButton;
};

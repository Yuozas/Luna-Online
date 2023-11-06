#pragma once
#include "afxwin.h"
#include "afxcmn.h"


class CclientApp;


// CItemDialog 대화 상자입니다.

class CItemDialog : public CDialog
{
	DECLARE_DYNAMIC(CItemDialog)

public:
	CItemDialog( CclientApp&, CWnd* pParent = NULL);   // 표준 생성자입니다.
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

	// 개수 등 아이템의 일반적인 정보를 표시;
	CGridCtrl	mItemGrid;

	// 아이템 옵션 정보를 표시
	CGridCtrl	mOptionGrid;

	DWORD		mServerIndex;
	DWORD		mSelectedItemDbIndex;

	CString		mPlayerName;
	DWORD		mPlayerIndex;
	DWORD		mUserIndex;
	
	DWORD		mStorageSize;
	DWORD		mExtendedInventorySize;

	// 선택된 데이터를 표시한다
	void PutData( DWORD itemDbIndex );
	ITEMBASE*	GetItem( DWORD itemDbIndex );
	CString		GetDateString( DWORD second ) const;
	
	//DWORD		mSelectedPosition;

	// 키: 아이템 DB인덱스
	typedef stdext::hash_map< DWORD, Item >	ItemMap;
	ItemMap									mItemMap;
	
	// 키: 아이템 DB인덱스
	typedef stdext::hash_map< DWORD, ITEM_OPTION >	OptionMap;
	OptionMap										mOptionMap;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ITEMDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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

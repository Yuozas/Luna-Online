#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CItemAddDialog ��ȭ �����Դϴ�.

class CItemAddDialog : public CDialog
{
	DECLARE_DYNAMIC(CItemAddDialog)

public:
	CItemAddDialog( CclientApp&, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CItemAddDialog();

	//void Parse( DWORD serverIndex, const MSGROOT* );

	virtual void OnFinalRelease();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ITEMADDDIALOG };

	// ���õ� ������ ��ȣ�� ��ȯ�Ѵ�. ������ 0�� ��ȯ�ȴ�
	DWORD	GetSelectedItemIndex() const;

protected:
	//CString mServerName;
	//CString	mPlayerName;
	//DWORD	mServerIndex;
	//DWORD	mPlayerIndex;
	CclientApp&	mApplication;

	DWORD	mSelectedItemIndex;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	virtual BOOL OnInitDialog();
	//virtual INT_PTR DoModal( const CString& serverName, const CString& playerName, DWORD playerIndex );
	//CTabCtrl mTabCtrl;
	afx_msg void OnTcnSelchangeItemAddTab(NMHDR *pNMHDR, LRESULT *pResult);
	CTabCtrl mTabCtrl;
	//CListCtrl mItemListCtrl1;
	CListCtrl mWeaponListCtrl;
	CListCtrl mPotionListCtrl;
	CListCtrl mMaterialListCtrl;
	//CListCtrl mQuestListCtrl;
	CListCtrl mEtcListCtrl;
	//afx_msg void OnBnClickedItemAddSubmitButton();
protected:
	virtual void OnOK();
public:
	CEdit mItemIndexEdit;
	CStatic mItemNameStatic;
	afx_msg void OnEnChangeItemAddIndexEdit();
	afx_msg void OnLvnItemchangedItemAddListWeapon(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedItemAddListPotion(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedItemAddListMaterial(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox mItemNameCombo;
	afx_msg void OnCbnSelchangeItemAddCombo();
	
	afx_msg void OnLvnItemchangedItemAddListEtc(NMHDR *pNMHDR, LRESULT *pResult);
};
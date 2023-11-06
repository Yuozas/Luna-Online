#pragma once
#include "afxcmn.h"
//#include "afxwin.h"


class CclientApp;


// CItemFindDialog ��ȭ �����Դϴ�.

class CItemFindDialog : public CDialog
{
	DECLARE_DYNAMIC(CItemFindDialog)

public:
	CItemFindDialog( CclientApp&, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CItemFindDialog();

	virtual void OnFinalRelease();
	void Parse( DWORD serverIndex, const MSGROOT* );


protected:
	void PutPage( DWORD index );
	int GetCurrentPage() const;

protected:
	CclientApp&	mApplication;
	CListCtrl	mResultListCtrl;
	CListCtrl	mItemListctrl;
	CComboBox	mServerComboBox;
    CEdit		mKeywordEdit;
	CStatic		mPageStatic;
	CStatic		mResultStatic;
	CString		mSelectedServerName;
	CButton		mPagePrevButton;
	CButton		mPageNextButton;
	CButton		mViewPlayerButton;
	CButton		mFindButton;
	CButton		mMoreFindButton;

	struct Player
	{
		DWORD	mItemIndex;
		DWORD	mItemSize;	// ������ ���� ����
		CString mName;		// �÷��̾� �̸�
	};

	// Ű: �÷��̾� �ε���
	typedef std::map< DWORD, Player >	Page;

	// Ű: �� ��ȣ
	typedef std::map< DWORD, Page >		PageMap;
	PageMap								mPageMap;


// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ITEMFINDDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLvnItemchangedItemFindDataList(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnBnClickedItemFindSearch();
	//afx_msg void OnLvnItemchangedItemFindResultList(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnBnClickedItemFindPrevPageButton();
	afx_msg void OnBnClickedItemFindNextPageButton();
	
	
	afx_msg void OnBnClickedItemFindMoreSearchButton();
	
	
	afx_msg void OnBnClickedItemFindViewPlayer();
	afx_msg void OnBnClickedItemFindSearchButton();
};

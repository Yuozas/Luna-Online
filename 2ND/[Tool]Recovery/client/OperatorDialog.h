#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// COperatorDialog ��ȭ �����Դϴ�.

class COperatorDialog : public CDialog
{
	DECLARE_DYNAMIC(COperatorDialog)

public:
	COperatorDialog( CclientApp&, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~COperatorDialog();

	void Parse( DWORD serverIndex, const MSGROOT* );
	void Request( DWORD serverIndex );
	virtual void OnFinalRelease();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_OPERATORDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	CclientApp&	mApplication;
	DWORD		mServerIndex;

	CListCtrl mIpListCtrl;
	CListCtrl mOperatorListCtrl;
	CEdit mIdEdit;
	CEdit mNameEdit;
	CComboBox mRankCombo;
	CButton mAddButton;
	CIPAddressCtrl mIpAddressCtrl;
	virtual BOOL OnInitDialog();	
	//afx_msg void OnBnClickedOperatorRemoveButton();
	afx_msg void OnBnClickedOperatorAddButton();
	CEdit mAddIdEdit;
	CEdit mAddNameEdit;
	afx_msg void OnBnClickedOperatorIpAddButton();
	afx_msg void OnBnClickedOperatorIpRemoveButton();
	afx_msg void OnLvnItemchangedOperatorListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedIpListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOperatorUpdateButton();
	CStatic mRankStatic;
	afx_msg void OnCbnSelchangeOperatorAuthCombo();
	afx_msg void OnBnClickedOperatorRemoveButton();
};

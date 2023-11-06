#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxdtctl.h"


// CItemOptionLogDialog ��ȭ �����Դϴ�.

class CItemOptionLogDialog : public CDialog
{
	DECLARE_DYNAMIC(CItemOptionLogDialog)

public:
	CItemOptionLogDialog( CclientApp&, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CItemOptionLogDialog();

	virtual void OnFinalRelease();

	void Parse( DWORD serverIndex, const MSGROOT* );
	//void Request( const CString& serverName, const CString& playerName, DWORD playerIndex );

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ITEMOPTIONLOGDIALOG };


protected:
	// ������ ����� �� �� �����Ƿ� Ŭ���̾�Ʈ���� �ɷ��� �Ѵ�
	std::set< int > mLogIndexSet;

	CString	mItemName;
	DWORD	mServerIndex;
	CclientApp&	mApplication;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	CButton mFindButton;
	CButton mStopButton;
	CProgressCtrl mResultProgressCtrl;
	CStatic mResultStatic;
	CButton mExcelButton;
	CListCtrl mResultListCtrl;
	afx_msg void OnBnClickedItemOptionLogFindButton();
	afx_msg void OnBnClickedItemOptionLogStopButton();
	afx_msg void OnBnClickedItemOptionLogExcelButton();
	virtual BOOL OnInitDialog();
	//virtual INT_PTR DoModal( DWORD serverIndex, const CString& itemName, DWORD itemDbIndex );

	void Request( const CString& serverName, const CString& itemName, DWORD itemDbIndex );

protected:
	virtual void OnCancel();
public:
	CDateTimeCtrl mBeginDateCtrl;
	CDateTimeCtrl mEndDateCtrl;
	CDateTimeCtrl mBeginTimeCtrl;
	CDateTimeCtrl mEndTimeCtrl;
	CEdit mItemDbIndexEdit;
	CComboBox mServerCombo;
};

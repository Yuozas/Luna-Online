#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxdtctl.h"


// CItemOptionLogDialog 대화 상자입니다.

class CItemOptionLogDialog : public CDialog
{
	DECLARE_DYNAMIC(CItemOptionLogDialog)

public:
	CItemOptionLogDialog( CclientApp&, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CItemOptionLogDialog();

	virtual void OnFinalRelease();

	void Parse( DWORD serverIndex, const MSGROOT* );
	//void Request( const CString& serverName, const CString& playerName, DWORD playerIndex );

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ITEMOPTIONLOGDIALOG };


protected:
	// 동일한 결과가 올 수 있으므로 클라이언트에서 걸러야 한다
	std::set< int > mLogIndexSet;

	CString	mItemName;
	DWORD	mServerIndex;
	CclientApp&	mApplication;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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

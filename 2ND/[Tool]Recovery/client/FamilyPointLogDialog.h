#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxdtctl.h"


// CFamilyPointLogDialog ��ȭ �����Դϴ�.

class CFamilyPointLogDialog : public CDialog
{
	DECLARE_DYNAMIC(CFamilyPointLogDialog)

public:
	CFamilyPointLogDialog( CclientApp&, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CFamilyPointLogDialog();

	void Parse( DWORD serverIndex, const MSGROOT* );
	
	virtual void OnFinalRelease();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_FAMILYPOINTLOGDIALOG };

protected:
	// ������ ����� �� �� �����Ƿ� Ŭ���̾�Ʈ���� �ɷ��� �Ѵ�
	std::set< int > mLogIndexSet;

	DWORD	mServerIndex;
	CString mServerName;
	CString	mFamilyName;
	DWORD	mFamilyIndex;
	//DWORD	mPlayerIndex;
	CclientApp&	mApplication;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	CButton mFindButton;
	CButton mStopButton;
	CStatic mResultStatic;
	CProgressCtrl mResultProgressCtrl;
	CButton mExcelButton;
	CListCtrl mResultListCtrl;
	afx_msg void OnBnClickedFamilyPointLogFindButton();
	afx_msg void OnBnClickedFamilyPointLogStopButton();
	afx_msg void OnBnClickedFamilyPointLogExcelButton();
	virtual BOOL OnInitDialog();
	virtual INT_PTR DoModal( const CString& serverName, const CString& familyName, DWORD familyIndex );
protected:
	virtual void OnCancel();
public:
	CDateTimeCtrl mBeginDateCtrl;
	CDateTimeCtrl mBeginTimeCtrl;
	CDateTimeCtrl mEndDateCtrl;
	CDateTimeCtrl mEndTimeCtrl;
};

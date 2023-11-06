#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxdtctl.h"


// CFamilyPointLogDialog 대화 상자입니다.

class CFamilyPointLogDialog : public CDialog
{
	DECLARE_DYNAMIC(CFamilyPointLogDialog)

public:
	CFamilyPointLogDialog( CclientApp&, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CFamilyPointLogDialog();

	void Parse( DWORD serverIndex, const MSGROOT* );
	
	virtual void OnFinalRelease();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_FAMILYPOINTLOGDIALOG };

protected:
	// 동일한 결과가 올 수 있으므로 클라이언트에서 걸러야 한다
	std::set< int > mLogIndexSet;

	DWORD	mServerIndex;
	CString mServerName;
	CString	mFamilyName;
	DWORD	mFamilyIndex;
	//DWORD	mPlayerIndex;
	CclientApp&	mApplication;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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

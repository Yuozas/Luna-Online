#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxdtctl.h"


// CFamilyCropLogDialog 대화 상자입니다.

class CFamilyCropLogDialog : public CDialog
{
	DECLARE_DYNAMIC(CFamilyCropLogDialog)

public:
	CFamilyCropLogDialog( CclientApp&, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CFamilyCropLogDialog();

	void Parse( DWORD serverIndex, const MSGROOT* );

	virtual void OnFinalRelease();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_FAMILYCROPLOGDIALOG };

protected:
	// 동일한 결과가 올 수 있으므로 클라이언트에서 걸러야 한다
	std::set< int > mLogIndexSet;

	DWORD	mServerIndex;
	CString	mFarmName;
	DWORD	mFarmIndex;
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
	afx_msg void OnBnClickedFamilyCropLogFindButton();
	afx_msg void OnBnClickedFamilyCropLogStopButton();
	afx_msg void OnBnClickedFamilyCropLogExcelButton();
	virtual BOOL OnInitDialog();
	virtual INT_PTR DoModal( DWORD serverIndex, const CString& familyName, DWORD familyIndex );;
protected:
	virtual void OnCancel();
public:
	CDateTimeCtrl mBeginDateCtrl;
	CDateTimeCtrl mBeginTimeCtrl;
	CDateTimeCtrl mEndDateCtrl;
	CDateTimeCtrl mEndTimeCtrl;
};

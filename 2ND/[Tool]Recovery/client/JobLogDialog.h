#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxdtctl.h"


// CJobLogDialog 대화 상자입니다.

class CJobLogDialog : public CDialog
{
	DECLARE_DYNAMIC(CJobLogDialog)

public:
	CJobLogDialog( CclientApp&, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CJobLogDialog();

	virtual void OnFinalRelease();

	void Parse( DWORD serverIndex, const MSGROOT* );

// 대화 상자 데이터입니다.
	enum { IDD = IDD_JOBLOGDIALOG };


protected:
	// 동일한 결과가 올 수 있으므로 클라이언트에서 걸러야 한다
	std::set< int > mLogIndexSet;

	CString mServerName;
	CString	mPlayerName;
	DWORD	mServerIndex;
	DWORD	mPlayerIndex;
	CclientApp&	mApplication;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	CComboBox mBeginYearCombo;
	CComboBox mBeginMonthCombo;
	CComboBox mBeginDayCombo;
	CComboBox mEndYearCombo;
	CComboBox mEndMonthCombo;
	CComboBox mEndDayCombo;
	CButton mFindButton;
	CButton mStopButton;
	CProgressCtrl mResultProgressCtrl;
	CButton mExcelButton;
	CListCtrl mResultListCtrl;
	afx_msg void OnBnClickedJobLogFindButton();
	afx_msg void OnBnClickedJobLogStopButton();
	afx_msg void OnBnClickedJobLogExcelButton();
	virtual BOOL OnInitDialog();
	virtual INT_PTR DoModal( const CString& serverName, const CString& playerName, DWORD playerIndex );
	CStatic mResultStatic;
protected:
	virtual void OnCancel();
public:
	CDateTimeCtrl mBeginDateCtrl;
	CDateTimeCtrl mBeginTimeCtrl;
	CDateTimeCtrl mEndDateCtrl;
	CDateTimeCtrl mEndTimeCtrl;
};

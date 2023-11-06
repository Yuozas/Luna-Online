#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxdtctl.h"


// CStatLogDialog 대화 상자입니다.

class CStatLogDialog : public CDialog
{
	DECLARE_DYNAMIC(CStatLogDialog)

public:
	CStatLogDialog( CclientApp&, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CStatLogDialog();

	virtual void OnFinalRelease();

	void Parse( DWORD serverIndex, const MSGROOT* );
	//void Request( const CString& serverName, const CString& playerName, DWORD playerIndex );

// 대화 상자 데이터입니다.
	enum { IDD = IDD_STATLOGDIALOG };


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
	CButton mFindButton;
	CButton mStopButton;
	CStatic mResultStatic;
	CProgressCtrl mResultProgress;
	CButton mExcelButton;
	CListCtrl mResultListCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedStatLogFindButton();
	afx_msg void OnBnClickedStatLogStopButton();
	afx_msg void OnBnClickedStatLogExcelButton();
	virtual INT_PTR DoModal( const CString& serverName, const CString& playerName, DWORD playerIndex );
protected:
	virtual void OnCancel();
public:
	CDateTimeCtrl mBeginDateCtrl;
	CDateTimeCtrl mBeginTimeCtrl;
	CDateTimeCtrl mEndTimeCtrl;
	CDateTimeCtrl mEndDateCtrl;
};

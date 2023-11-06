#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxdtctl.h"



// CGuildLogDialog 대화 상자입니다.

class CGuildLogDialog : public CDialog
{
	DECLARE_DYNAMIC(CGuildLogDialog)

public:
	CGuildLogDialog( CclientApp&, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CGuildLogDialog();

	virtual void OnFinalRelease();

	void Parse( DWORD serverIndex, const MSGROOT* );
	//void Request( const CString& serverName, const CString& playerName, DWORD playerIndex );

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_GUILDLOGDIALOG };

protected:
	// 동일한 결과가 올 수 있으므로 클라이언트에서 걸러야 한다
	std::set< int > mLogIndexSet;

	DWORD	mServerIndex;
	CString mServerName;
	CString	mGuildName;
	DWORD	mGuildIndex;
	//DWORD	mPlayerIndex;
	CclientApp&	mApplication;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	afx_msg void OnBnClickedGuildLogFindButton();
	afx_msg void OnBnClickedGuildLogStopButton();
	afx_msg void OnBnClickedGuildLogExcelButton();
	virtual BOOL OnInitDialog();

	CProgressCtrl mResultProgressCtrl;
	CStatic mResultStatic;
	CListCtrl mResultListCtrl;
	CButton mFindButton;
	CButton mStopButton;
	virtual INT_PTR DoModal( DWORD serverIndex, const CString& guildName, DWORD guildIndex );
protected:
	virtual void OnCancel();
public:
	CDateTimeCtrl mBeginDateCtrl;
	CDateTimeCtrl mBeginTimeCtrl;
	CDateTimeCtrl mEndDateCtrl;
	CDateTimeCtrl mEndTimeCtrl;
	afx_msg void OnBnClickedGuildLogClipboardButton();
};
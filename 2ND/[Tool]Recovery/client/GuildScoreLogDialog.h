#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxdtctl.h"


// CGuildScoreLogDialog 대화 상자입니다.

class CGuildScoreLogDialog : public CDialog
{
	DECLARE_DYNAMIC(CGuildScoreLogDialog)

public:
	CGuildScoreLogDialog( CclientApp&, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CGuildScoreLogDialog();

	virtual void OnFinalRelease();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_GUILDSCORELOGDIALOG };

	void Parse( DWORD serverIndex, const MSGROOT* );
	//void Request( const CString& serverName, const CString& playerName, DWORD playerIndex );


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
	CButton mFindButton;
	CButton mStopButton;
	CStatic mResultStatic;
	CProgressCtrl mResultProgressCtrl;
	CButton mExcelButton;
	CListCtrl mResultListCtrl;
	afx_msg void OnBnClickedGuildScoreLogFindButton();
	afx_msg void OnBnClickedGuildScoreLogStopButton();
	afx_msg void OnBnClickedGuildScoreLogExcelButton();
	virtual BOOL OnInitDialog();
	virtual INT_PTR DoModal( const CString& serverName, const CString& guildName, DWORD guildIndex );
protected:
	virtual void OnCancel();
public:
	CDateTimeCtrl mBeginDateCtrl;
	CDateTimeCtrl mBeginTimeCtrl;
	CDateTimeCtrl mEndDateCtrl;
	CDateTimeCtrl mEndTimeCtrl;
};

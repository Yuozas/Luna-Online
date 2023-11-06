#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxdtctl.h"


// CQuestLogDialog 대화 상자입니다.

class CQuestLogDialog : public CDialog
{
	DECLARE_DYNAMIC(CQuestLogDialog)

public:
	CQuestLogDialog( CclientApp&, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CQuestLogDialog();

	void Parse( DWORD serverIndex, const MSGROOT* );
	virtual void OnFinalRelease();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_QUESTLOGDIALOG };


protected:
	//CString mServerName;
	CString	mPlayerName;
	DWORD	mServerIndex;
	DWORD	mPlayerIndex;
	CclientApp&	mApplication;

	// 동일한 결과가 올 수 있으므로 클라이언트에서 걸러야 한다
	std::set< int > mLogIndexSet;


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
	virtual BOOL OnInitDialog();
	virtual INT_PTR DoModal( DWORD serverIndex, const CString& playerName, DWORD playerIndex );
	afx_msg void OnBnClickedQuestLogFindButton();
	afx_msg void OnBnClickedQuestLogStopButton();
	afx_msg void OnBnClickedQuestLogExcelButton();
	CDateTimeCtrl mBeginDateCtrl;
	CDateTimeCtrl mBeginTimeCtrl;
	CDateTimeCtrl mEndDateCtrl;
	CDateTimeCtrl mEndTimeCtrl;
	afx_msg void OnBnClickedQuestLogClipboardButton();
};

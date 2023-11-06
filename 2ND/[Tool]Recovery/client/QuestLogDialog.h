#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxdtctl.h"


// CQuestLogDialog ��ȭ �����Դϴ�.

class CQuestLogDialog : public CDialog
{
	DECLARE_DYNAMIC(CQuestLogDialog)

public:
	CQuestLogDialog( CclientApp&, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CQuestLogDialog();

	void Parse( DWORD serverIndex, const MSGROOT* );
	virtual void OnFinalRelease();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_QUESTLOGDIALOG };


protected:
	//CString mServerName;
	CString	mPlayerName;
	DWORD	mServerIndex;
	DWORD	mPlayerIndex;
	CclientApp&	mApplication;

	// ������ ����� �� �� �����Ƿ� Ŭ���̾�Ʈ���� �ɷ��� �Ѵ�
	std::set< int > mLogIndexSet;


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

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxdtctl.h"


// CStatLogDialog ��ȭ �����Դϴ�.

class CStatLogDialog : public CDialog
{
	DECLARE_DYNAMIC(CStatLogDialog)

public:
	CStatLogDialog( CclientApp&, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CStatLogDialog();

	virtual void OnFinalRelease();

	void Parse( DWORD serverIndex, const MSGROOT* );
	//void Request( const CString& serverName, const CString& playerName, DWORD playerIndex );

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_STATLOGDIALOG };


protected:
	// ������ ����� �� �� �����Ƿ� Ŭ���̾�Ʈ���� �ɷ��� �Ѵ�
	std::set< int > mLogIndexSet;

	CString mServerName;
	CString	mPlayerName;
	DWORD	mServerIndex;
	DWORD	mPlayerIndex;
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

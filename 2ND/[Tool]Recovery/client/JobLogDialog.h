#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxdtctl.h"


// CJobLogDialog ��ȭ �����Դϴ�.

class CJobLogDialog : public CDialog
{
	DECLARE_DYNAMIC(CJobLogDialog)

public:
	CJobLogDialog( CclientApp&, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CJobLogDialog();

	virtual void OnFinalRelease();

	void Parse( DWORD serverIndex, const MSGROOT* );

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_JOBLOGDIALOG };


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

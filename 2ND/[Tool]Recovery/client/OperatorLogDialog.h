#pragma once
#include "afxdtctl.h"
#include "afxcmn.h"
#include "afxwin.h"


// COperatorLogDialog ��ȭ �����Դϴ�.

class COperatorLogDialog : public CDialog
{
	DECLARE_DYNAMIC(COperatorLogDialog)

public:
	COperatorLogDialog( CclientApp&, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~COperatorLogDialog();

	virtual void OnFinalRelease();

	void Parse( DWORD serverIndex, const MSGROOT* );

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_OPERATORLOGDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	afx_msg void OnBnClickedRecoveryLogFindButton();
	afx_msg void OnBnClickedRecoveryLogStopButton();
	afx_msg void OnBnClickedRecoveryLogExcelButton();
	afx_msg void OnBnClickedRecoveryLogClipboardButton();
	CDateTimeCtrl mBeginDateCtrl;
	CDateTimeCtrl mEndDateCtrl;
	CDateTimeCtrl mBeginTimeCtrl;
	CDateTimeCtrl mEndTimeCtrl;
	CProgressCtrl mProgressCtrl;
	CListCtrl mResultListCtrl;
	virtual BOOL OnInitDialog();
	CButton mFindButton;
	CButton mStopButton;
	virtual INT_PTR DoModal();


protected:
	// ���� �ٸ� ��û�� ������ �ʵ���, �� ���� ��ġ�� �α׸� ǥ���Ѵ�
	DWORD	mRequestTick;

	CclientApp& mApplication;
	

protected:
	typedef std::map< int, MSG_RM_OPERATOR_LOG::Log >	LogMap;
	LogMap												mLogMap;

	//void PutLog();


public:
	CComboBox mServerCombo;
	CStatic mResultStatic;
	//CEdit mIdEdit;
	afx_msg void OnCbnSelchangeOperatorLogCombo();
	CComboBox mOperatorCombo;
};

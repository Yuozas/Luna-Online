#pragma once
#include "afxdtctl.h"
#include "afxcmn.h"
#include "afxwin.h"


// COperatorLogDialog 대화 상자입니다.

class COperatorLogDialog : public CDialog
{
	DECLARE_DYNAMIC(COperatorLogDialog)

public:
	COperatorLogDialog( CclientApp&, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~COperatorLogDialog();

	virtual void OnFinalRelease();

	void Parse( DWORD serverIndex, const MSGROOT* );

// 대화 상자 데이터입니다.
	enum { IDD = IDD_OPERATORLOGDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
	// 서로 다른 요청이 섞이지 않도록, 이 값이 일치한 로그만 표시한다
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

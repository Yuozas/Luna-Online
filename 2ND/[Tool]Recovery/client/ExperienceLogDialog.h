#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxdtctl.h"


// CExperienceLogDialog ��ȭ �����Դϴ�.

class CExperienceLogDialog : public CDialog
{
	DECLARE_DYNAMIC(CExperienceLogDialog)

public:
	CExperienceLogDialog( CclientApp&, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CExperienceLogDialog();

	virtual void OnFinalRelease();

	void Parse( DWORD serverIndex, const MSGROOT* );
	//void Request( const CString& serverName, const CString& playerName, DWORD playerIndex );

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_EXPERIENCELOGDIALOG };


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
	afx_msg void OnBnClickedExpFindButton();
	afx_msg void OnBnClickedExpStopButton();
	
	CStatic mResultStatic;
	CListCtrl mResultListCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedExpExcelButton();
	CButton mFindButton;
	CButton mStopButton;
	virtual INT_PTR DoModal( const CString& serverName, const CString& playerName, DWORD playerIndex );

protected:
	virtual void OnCancel();
public:
	CProgressCtrl mResultProgressCtrl;
	CDateTimeCtrl mBeginDateCtrl;
	CDateTimeCtrl mBeginTimeCtrl;
	CDateTimeCtrl mEndDateCtrl;
	CDateTimeCtrl mEndTimeCtrl;
};

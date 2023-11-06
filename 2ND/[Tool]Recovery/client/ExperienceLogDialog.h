#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxdtctl.h"


// CExperienceLogDialog 대화 상자입니다.

class CExperienceLogDialog : public CDialog
{
	DECLARE_DYNAMIC(CExperienceLogDialog)

public:
	CExperienceLogDialog( CclientApp&, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CExperienceLogDialog();

	virtual void OnFinalRelease();

	void Parse( DWORD serverIndex, const MSGROOT* );
	//void Request( const CString& serverName, const CString& playerName, DWORD playerIndex );

// 대화 상자 데이터입니다.
	enum { IDD = IDD_EXPERIENCELOGDIALOG };


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

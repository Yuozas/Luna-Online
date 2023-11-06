#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxdtctl.h"


// CSkillLogDialog 대화 상자입니다.

class CSkillLogDialog : public CDialog
{
	DECLARE_DYNAMIC(CSkillLogDialog)

public:
	CSkillLogDialog( CclientApp&, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSkillLogDialog();

	virtual void OnFinalRelease();

	void Parse( DWORD serverIndex, const MSGROOT* );
	//void Request( const CString& serverName, const CString& playerName, DWORD playerIndex );

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SKILLLOGDIALOG };


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
	afx_msg void OnBnClickedSkillLogFindButton();
	CStatic mResultStatic;
	CProgressCtrl mResultProgressCtrl;
	CButton mExcelButton;
	CButton mFindButton;
	CButton mStopButton;
	CListCtrl mResultListCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSkillLogStopButton();
	afx_msg void OnBnClickedSkillLogExcelButton();
	virtual INT_PTR DoModal( const CString& serverName, const CString& playerName, DWORD playerIndex );
protected:
//	virtual void OnCancel();
public:
	CDateTimeCtrl mBeginDateCtrl;
	CDateTimeCtrl mBeginTimeCtrl;
	CDateTimeCtrl mEndDateCtrl;
	CDateTimeCtrl mEndTimeCtrl;
	afx_msg void OnBnClickedSkillLogClipboardButton();
};

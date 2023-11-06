#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxdtctl.h"



// CGuildLogDialog ��ȭ �����Դϴ�.

class CGuildLogDialog : public CDialog
{
	DECLARE_DYNAMIC(CGuildLogDialog)

public:
	CGuildLogDialog( CclientApp&, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CGuildLogDialog();

	virtual void OnFinalRelease();

	void Parse( DWORD serverIndex, const MSGROOT* );
	//void Request( const CString& serverName, const CString& playerName, DWORD playerIndex );

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_GUILDLOGDIALOG };

protected:
	// ������ ����� �� �� �����Ƿ� Ŭ���̾�Ʈ���� �ɷ��� �Ѵ�
	std::set< int > mLogIndexSet;

	DWORD	mServerIndex;
	CString mServerName;
	CString	mGuildName;
	DWORD	mGuildIndex;
	//DWORD	mPlayerIndex;
	CclientApp&	mApplication;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	afx_msg void OnBnClickedGuildLogFindButton();
	afx_msg void OnBnClickedGuildLogStopButton();
	afx_msg void OnBnClickedGuildLogExcelButton();
	virtual BOOL OnInitDialog();

	CProgressCtrl mResultProgressCtrl;
	CStatic mResultStatic;
	CListCtrl mResultListCtrl;
	CButton mFindButton;
	CButton mStopButton;
	virtual INT_PTR DoModal( DWORD serverIndex, const CString& guildName, DWORD guildIndex );
protected:
	virtual void OnCancel();
public:
	CDateTimeCtrl mBeginDateCtrl;
	CDateTimeCtrl mBeginTimeCtrl;
	CDateTimeCtrl mEndDateCtrl;
	CDateTimeCtrl mEndTimeCtrl;
	afx_msg void OnBnClickedGuildLogClipboardButton();
};
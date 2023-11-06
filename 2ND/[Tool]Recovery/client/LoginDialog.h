#pragma once
#include "afxwin.h"
#include "afxcmn.h"


class CclientApp;


// CLoginDialog 대화 상자입니다.

class CLoginDialog : public CDialog
{
	DECLARE_DYNAMIC(CLoginDialog)

public:
	CLoginDialog( CclientApp&, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLoginDialog();

	virtual void OnFinalRelease();
	void Parse( DWORD serverIndex, const MSGROOT* );

// 대화 상자 데이터입니다.
	enum { IDD = IDD_LOGINDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
//	afx_msg void OnOK();
	

private:
	CclientApp&	mApplication;

	CEdit	mIdEdit;
	CEdit	mPasswordEdit;
	

public:
	virtual BOOL	OnInitDialog();
	CListCtrl mServerListCtrl;
	CStatic mVersionStatic;
	afx_msg void OnBnClickedLoginConnectButton();
	CButton mSubmitButton;
	CButton mCancelButton;
};

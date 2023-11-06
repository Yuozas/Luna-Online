#pragma once
#include "afxwin.h"
#include "afxcmn.h"


class CclientApp;


// CLoginDialog ��ȭ �����Դϴ�.

class CLoginDialog : public CDialog
{
	DECLARE_DYNAMIC(CLoginDialog)

public:
	CLoginDialog( CclientApp&, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CLoginDialog();

	virtual void OnFinalRelease();
	void Parse( DWORD serverIndex, const MSGROOT* );

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_LOGINDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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

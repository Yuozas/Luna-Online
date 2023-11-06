#pragma once
#include "afxcmn.h"


// CPermissionDialog ��ȭ �����Դϴ�.

class CPermissionDialog : public CDialog
{
	DECLARE_DYNAMIC(CPermissionDialog)

public:
	CPermissionDialog( CclientApp&, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPermissionDialog();

	virtual void OnFinalRelease();

	void Parse( DWORD serverIndex, const MSGROOT* );
	void Request( DWORD serverIndex );

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PERMISSIONDIALOG };

protected:
	CString GetIpAddress();

protected:
	CclientApp& mApplication;
	DWORD		mServerIndex;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	CIPAddressCtrl mIpAddressCtrl;
	afx_msg void OnBnClickedPermissionAddButton();
	afx_msg void OnBnClickedPermissionRemoveButton();
};

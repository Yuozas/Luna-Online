#pragma once
#include "afxwin.h"


// CPasswordDialog ��ȭ �����Դϴ�.

class CPasswordDialog : public CDialog
{
	DECLARE_DYNAMIC(CPasswordDialog)

public:
	CPasswordDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPasswordDialog();

	virtual void OnFinalRelease();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PASSWORDDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	CEdit mPreviousPasswordEdit;
	CEdit mNextPasswordEdit;
	CEdit mConfirmPasswordEdit;
	//afx_msg void OnBnClickedButton1();

public:
	const char* GetPreviousPassword() const;
	const char* GetNextPassword() const;

protected:
	CString mPreviousPassword;
	CString mNextPassword;
	virtual void OnOK();
};

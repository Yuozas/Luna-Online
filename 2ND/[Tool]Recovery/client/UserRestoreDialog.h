#pragma once
#include "afxwin.h"


// CUserRestoreDialog ��ȭ �����Դϴ�.

class CUserRestoreDialog : public CDialog
{
	DECLARE_DYNAMIC(CUserRestoreDialog)

public:
	CUserRestoreDialog( CclientApp&, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CUserRestoreDialog();

	virtual void OnFinalRelease();
	//void Parse( DWORD serverIndex, const MSGROOT* );

	const char* GetName() const;

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_USERRESTOREDIALOG };


protected:
	CclientApp&	mApplication;

	//DWORD	mServerIndex;
	//DWORD	mPlayerIndex;
	CString	mPlayerName;
	//CString	mDuplicatedName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	CEdit mNameEdit;
	virtual INT_PTR DoModal( const CString& playerName );
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	//afx_msg void OnBnClickedUserRestoreSubmitButton();
	afx_msg void OnBnClickedOk();
};

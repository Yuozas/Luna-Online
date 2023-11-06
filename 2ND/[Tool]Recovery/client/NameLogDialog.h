#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CNameLogDialog ��ȭ �����Դϴ�.

class CNameLogDialog : public CDialog
{
	DECLARE_DYNAMIC(CNameLogDialog)

public:
	CNameLogDialog( CclientApp&, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CNameLogDialog();

	virtual void OnFinalRelease();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_NAMELOGDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	CListCtrl mResultListCtrl;

	virtual INT_PTR DoModal( DWORD serverIndex, DWORD playerIndex );

	void Parse( DWORD serverIndex, const MSGROOT* );


protected:
	DWORD		mPlayerIndex;
	DWORD		mServerIndex;
	int			mPage;

	CclientApp&	mApplication;

	// Ű: �α� �ε���
	typedef	std::map< DWORD, MSG_RM_NAME_LOG::Log >	LogMap;
	LogMap											mLogMap;	


public:
	virtual BOOL OnInitDialog();	
	afx_msg void OnBnClickedNameLogClipboardButton();
};

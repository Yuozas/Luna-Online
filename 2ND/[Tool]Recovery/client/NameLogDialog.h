#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CNameLogDialog 대화 상자입니다.

class CNameLogDialog : public CDialog
{
	DECLARE_DYNAMIC(CNameLogDialog)

public:
	CNameLogDialog( CclientApp&, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CNameLogDialog();

	virtual void OnFinalRelease();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_NAMELOGDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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

	// 키: 로그 인덱스
	typedef	std::map< DWORD, MSG_RM_NAME_LOG::Log >	LogMap;
	LogMap											mLogMap;	


public:
	virtual BOOL OnInitDialog();	
	afx_msg void OnBnClickedNameLogClipboardButton();
};

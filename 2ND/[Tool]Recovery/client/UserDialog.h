#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CUserDialog 대화 상자입니다.

class CUserDialog : public CDialog
{
	DECLARE_DYNAMIC(CUserDialog)

public:
	CUserDialog( CclientApp&, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUserDialog();

	virtual void OnFinalRelease();

	void Request( DWORD serverIndex, const CString& userName, DWORD userIndex );
	void Parse( DWORD serverIndex, const MSGROOT* );

	//void AddLivePlayer( DWORD playerIndex, const CString& name, DWORD level );
	void RestorePlayer( DWORD playerIndex, const CString& name );

// 대화 상자 데이터입니다.
	enum { IDD = IDD_USERDIALOG };

protected:
	CclientApp& mApplication;

	DWORD		mUserIndex;
	DWORD		mServerIndex;
	CString		mUserName;
	
	MSG_RM_USER_DATA	mReceivedData;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	CEdit mJoinedDateEdit;
	CEdit mLogoutDateEdit;
	CListCtrl mLivePlayerListCtrl;
	virtual BOOL OnInitDialog();
	CStatic mAgentStatic;
	CListCtrl mDeletedPlayerListCtrl;
	CButton mLivePlayerButton;
	CButton mDeletedPlayerButton;
	CEdit midEdit;
	CStatic mConnectStatic;
	CStatic mNameStatic;
	CComboBox mLevelCombo;

	afx_msg void OnBnClickedUserLivePlayerButton();
	afx_msg void OnBnClickedUserDeletedPlayerButton();
	afx_msg void OnBnClickedUserUpdateButton();
	afx_msg void OnBnClickedUserCreateButton();
	afx_msg void OnBnClickedUserRestoreButton();
	afx_msg void OnBnClickedUserDeleteButton();
};

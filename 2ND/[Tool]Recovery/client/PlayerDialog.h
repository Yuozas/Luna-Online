#pragma once
#include "afxwin.h"
#include "afxcmn.h"
//#include "afxcmn.h"
//#include "afxwin.h"


class CclientApp;


// CPlayerDialog 대화 상자입니다.

class CPlayerDialog : public CDialog
{
	DECLARE_DYNAMIC(CPlayerDialog)

public:
	CPlayerDialog( CclientApp&, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPlayerDialog();

	virtual void OnFinalRelease();
	virtual BOOL	OnInitDialog();

	void Request( DWORD serverIndex, const CString& playerName, DWORD playerIndex );
	void Request( const CString& serverName, const CString& playerName, DWORD playerIndex );
	void Parse( DWORD serverIndex, const MSGROOT* );


protected:
	CclientApp& mApplication;

	DWORD	mPlayerIndex;
	CString	mPlayerName;
	CString	mServerName;
	DWORD	mUserIndex;

	// for numeric data
	CGridCtrl	mDataGrid;

	// for string data
	CGridCtrl	mExtendedDataGrid;
	CGridCtrl	mRemovedDataGrid;

	// stored received data from server
	MSG_RM_PLAYER_DATA				mReceivedDataMessage;
	MSG_RM_PLAYER_EXTENDED_DATA		mReceivedExtendedMessage;
	MSG_RM_PLAYER_LOVE				mReceivedLicenseMessage;	
	DWORD	mServerIndex;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PLAYERDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	afx_msg void OnViewStatusBar();
//	afx_msg void OnBnClickedPlayerDialogUpdate();
	afx_msg void OnBnClickedPlayerItem();
	//CListCtrl mJobList;
	//CStatic mJobCategoryStatic;

	CEdit mLoveScoreEdit;
	CTreeCtrl mGoodThingsTree;
	CTreeCtrl mBadThingsTree;
	CButton mLicenseUpdateButton;
	afx_msg void OnBnClickedPlayerLicenseUpdateButtno();
	CEdit mAgeEdit;
	afx_msg void OnBnClickedPlayerExpLogButton();
	afx_msg void OnBnClickedPlayerStatLogButton();
	//CButton mGuildButton;
	afx_msg void OnBnClickedPlayerGuildButton();
	CEdit mUserNameEdit;
	afx_msg void OnBnClickedPlayerUserViewButton();
	//afx_msg void OnBnClickedPlayerJobLogButton();
	afx_msg void OnBnClickedPlayerJobButton();
	afx_msg void OnBnClickedPlayerItemLogButton();
	//afx_msg void OnBnClickedPlayerItemAddButton();
	CComboBox mAreaCombo;
	CButton mGuildButton;
	afx_msg void OnBnClickedPlayerNameUpdateButton();
	CComboBox mJob1Combo;
	CComboBox mJob2Combo;
	CComboBox mJob3Combo;
	CComboBox mJob4Combo;
	CComboBox mJob5Combo;
	CComboBox mJob6Combo;
	afx_msg void OnBnClickedPlayerJobSubmitButton();
	afx_msg void OnCbnSelchangePlayerJob1Combo();
	afx_msg void OnCbnSelchangePlayerJob2Combo();
	afx_msg void OnCbnSelchangePlayerJob3Combo();
	afx_msg void OnCbnSelchangePlayerJob4Combo();
	afx_msg void OnCbnSelchangePlayerJob5Combo();
	afx_msg void OnCbnSelchangePlayerJob6Combo();
	CTabCtrl mTabCtrl;
	afx_msg void OnTcnSelchangePlayerDataTab(NMHDR *pNMHDR, LRESULT *pResult);
	CButton mDataSubmitButton;
	CButton mExtendedDataSubmitButton;
	afx_msg void OnBnClickedPlayerNameLogButton();
};

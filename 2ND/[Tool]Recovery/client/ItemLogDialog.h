#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxdtctl.h"


// CItemLogDialog 대화 상자입니다.

class CItemLogDialog : public CDialog
{
	DECLARE_DYNAMIC(CItemLogDialog)

public:
	CItemLogDialog( CclientApp&, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CItemLogDialog();

	virtual void OnFinalRelease();

	void Request( DWORD serverIndex, const CString& itemName, DWORD itemDbIndex );
	void Parse( DWORD serverIndex, const MSGROOT* );
	void SetItemDbIndex( DWORD serverIndex, DWORD itemDbIndex );
	void SetPlayerIndex( DWORD serverIndex, DWORD playerIndex );
	
// 대화 상자 데이터입니다.
	enum { IDD = IDD_ITEMLOGDIALOG };


protected:
	CclientApp&	mApplication;
	DWORD		mServerIndex;
	DWORD		mItemIndex;

	// 동일한 결과가 올 수 있으므로 클라이언트에서 걸러야 한다
	//std::set< int > mLogIndexSet;

	typedef std::map< int, MSG_RM_ITEM_LOG::Log >	LogMap;
	LogMap											mLogMap;

	// 로그를 요청한 시간
	DWORD mRequestTick;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	//CComboBox mServerCombo;
	CEdit mKeywordEdit;
	CButton mFindButton;
	CButton mStopButton;
	CButton mExcelButton;
	CStatic mResultStatic;
	CProgressCtrl mResultProgress;
	CListCtrl mResultListCtrl;
	afx_msg void OnBnClickedItemLogFindButton();
	afx_msg void OnBnClickedItemLogStopButton();
	afx_msg void OnBnClickedItemLogExcelButton();
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedItemLogOptionLogButton();
protected:
	virtual void OnCancel();
	void PutLog();
public:
	CComboBox mServerCombo;

	CButton mItemIndexRadio;
	CButton	mDbIndexRadio;
	afx_msg void OnBnClickedItemLogDbIndexRadio();
	CDateTimeCtrl mBeginDateCtrl;
	CDateTimeCtrl mBeginTimeCtrl;
	CDateTimeCtrl mEndDateCtrl;
	CDateTimeCtrl mEndTimeCtrl;
	afx_msg void OnBnClickedItemLogCopyButton();
	CButton mPlayerIndexRadio;
};

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxdtctl.h"


// CItemLogDialog ��ȭ �����Դϴ�.

class CItemLogDialog : public CDialog
{
	DECLARE_DYNAMIC(CItemLogDialog)

public:
	CItemLogDialog( CclientApp&, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CItemLogDialog();

	virtual void OnFinalRelease();

	void Request( DWORD serverIndex, const CString& itemName, DWORD itemDbIndex );
	void Parse( DWORD serverIndex, const MSGROOT* );
	void SetItemDbIndex( DWORD serverIndex, DWORD itemDbIndex );
	void SetPlayerIndex( DWORD serverIndex, DWORD playerIndex );
	
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ITEMLOGDIALOG };


protected:
	CclientApp&	mApplication;
	DWORD		mServerIndex;
	DWORD		mItemIndex;

	// ������ ����� �� �� �����Ƿ� Ŭ���̾�Ʈ���� �ɷ��� �Ѵ�
	//std::set< int > mLogIndexSet;

	typedef std::map< int, MSG_RM_ITEM_LOG::Log >	LogMap;
	LogMap											mLogMap;

	// �α׸� ��û�� �ð�
	DWORD mRequestTick;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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

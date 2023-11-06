#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxdtctl.h"


enum eLogitemmoney;


// CGuildWarehouseLogDialog ��ȭ �����Դϴ�.

class CGuildWarehouseLogDialog : public CDialog
{
	DECLARE_DYNAMIC(CGuildWarehouseLogDialog)

public:
	CGuildWarehouseLogDialog( CclientApp&, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CGuildWarehouseLogDialog();

	virtual void OnFinalRelease();

	void Parse( DWORD serverIndex, const MSGROOT* );

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_GUILDWAREHOUSELOGDIALOG };


protected:
	//const char* GetLogType( eLogitemmoney, const ITEMBASE&, POSTYPE fromPosition ) const;


protected:
	// ������ ����� �� �� �����Ƿ� Ŭ���̾�Ʈ���� �ɷ��� �Ѵ�
	std::set< int > mLogIndexSet;

	DWORD	mServerIndex;
	CString	mGuildName;
	DWORD	mGuildIndex;
	//DWORD	mPlayerIndex;
	CclientApp&	mApplication;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual INT_PTR DoModal( DWORD serverIndex, const CString& guildName, DWORD guildIndex );
	afx_msg void OnBnClickedGuildItemLogFindButton();
	afx_msg void OnBnClickedGuildItemLogStopButton();
	afx_msg void OnBnClickedGuildItemLogExcelButton();
	CStatic mResultStatic;
	CProgressCtrl mResultProgressCtrl;
	CListCtrl mResultListCtrl;
	CButton mFindButton;
	CButton mStopButton;
	afx_msg void OnBnClickedGuildItemLogPlayerButton();
	afx_msg void OnBnClickedGuildItemLogPlayerButton2();
protected:
	virtual void OnCancel();
public:
	CDateTimeCtrl mBeginDateCtrl;
	CDateTimeCtrl mEndDateCtrl;
	CDateTimeCtrl mBeginTimeCtrl;
	CDateTimeCtrl mEndTimeCtrl;
};

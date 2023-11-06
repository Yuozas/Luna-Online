#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxdtctl.h"


enum eLogitemmoney;


// CGuildWarehouseLogDialog 대화 상자입니다.

class CGuildWarehouseLogDialog : public CDialog
{
	DECLARE_DYNAMIC(CGuildWarehouseLogDialog)

public:
	CGuildWarehouseLogDialog( CclientApp&, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CGuildWarehouseLogDialog();

	virtual void OnFinalRelease();

	void Parse( DWORD serverIndex, const MSGROOT* );

// 대화 상자 데이터입니다.
	enum { IDD = IDD_GUILDWAREHOUSELOGDIALOG };


protected:
	//const char* GetLogType( eLogitemmoney, const ITEMBASE&, POSTYPE fromPosition ) const;


protected:
	// 동일한 결과가 올 수 있으므로 클라이언트에서 걸러야 한다
	std::set< int > mLogIndexSet;

	DWORD	mServerIndex;
	CString	mGuildName;
	DWORD	mGuildIndex;
	//DWORD	mPlayerIndex;
	CclientApp&	mApplication;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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

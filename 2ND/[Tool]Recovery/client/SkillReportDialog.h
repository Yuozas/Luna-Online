#pragma once
#include "afxcmn.h"


// CSkillReportDialog ��ȭ �����Դϴ�.

class CSkillReportDialog : public CDialog
{
	DECLARE_DYNAMIC(CSkillReportDialog)

public:
	CSkillReportDialog( CclientApp&, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSkillReportDialog();

	virtual void OnFinalRelease();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SKILLREPORTDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
//	virtual INT_PTR DoModal();

protected:
	CclientApp& mApplication;

public:
	virtual BOOL OnInitDialog();
	CListCtrl mReportListCtrl;
};
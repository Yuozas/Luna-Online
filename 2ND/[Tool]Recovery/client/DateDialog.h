#pragma once
#include "afxwin.h"


// CDateDialog ��ȭ �����Դϴ�.

class CDateDialog : public CDialog
{
	DECLARE_DYNAMIC(CDateDialog)

public:
	CDateDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDateDialog();

	DWORD GetSecond() const;

	virtual void OnFinalRelease();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DATEDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

protected:
	DWORD		mSecond;
	CString		mDateString;


public:
	CStatic mPreviousDateStatic;
	CComboBox mYearCombo;
	CComboBox mMonthCombo;
	CComboBox mDayCombo;
	CComboBox mHourCombo;
	CComboBox mMinuteCombo;
	CComboBox mSecondCombo;
	virtual BOOL OnInitDialog();
	virtual INT_PTR DoModal( DWORD second, const CString& dateString );
protected:
	virtual void OnOK();
};

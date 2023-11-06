#pragma once
#include "afxwin.h"


// CQuantityDialog ��ȭ �����Դϴ�.

class CQuantityDialog : public CDialog
{
	DECLARE_DYNAMIC(CQuantityDialog)

public:
	CQuantityDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CQuantityDialog();

	virtual void OnFinalRelease();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_QUANTITYDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	CComboBox mQuantityCombo;
	afx_msg void OnBnClickedOk();


private:
	int	mQuantity;
public:
	int GetQuantity() const;

	virtual BOOL OnInitDialog();
};

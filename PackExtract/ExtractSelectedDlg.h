#pragma once
#include "afxwin.h"


// ExtractSelectedDlg ��ȭ �����Դϴ�.

class ExtractSelectedDlg : public CDialog
{
	DECLARE_DYNAMIC(ExtractSelectedDlg)

public:
	ExtractSelectedDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~ExtractSelectedDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_EXTRACT_SELECTED_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CListBox m_SeletList;
	virtual BOOL OnInitDialog();
	BOOL m_bClearDir;
	afx_msg void OnBnClickedDelteButton();
};

#if !defined(AFX_USINGSHOPDLG_H__82DCBE3D_1A87_43DB_BC26_319B085183BF__INCLUDED_)
#define AFX_USINGSHOPDLG_H__82DCBE3D_1A87_43DB_BC26_319B085183BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UsingShopDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUsingShopDlg dialog

class CUsingShopDlg : public CDialog
{
// Construction
public:
	CUsingShopDlg(CWnd* pParent = NULL);   // standard constructor

	CFont	m_Font;
	DWORD	m_dwDocumentIdx;
	DWORD	m_dwCharIdx;
	BOOL	m_bEdit;

	void	SetInitData( DWORD dwDocumentIdx, DWORD dwCharIdx, BOOL bEdit );
	void	SendUsingShopItemInfoSyn();
	void	UpdateUsingShopItemInfo( TMSG_USINGSHOPITEMINFO* pMsg );

	void	InitUsingShopItemListCtrl();

// Dialog Data
	//{{AFX_DATA(CUsingShopDlg)
	enum { IDD = IDD_DLG_USING_SHOPITEM };
	CButton	m_btnDelete;
	CButton	m_btnSave;
	CEdit	m_etItemRemainTime;
	CEdit	m_etItemBeginTime;
	CEdit	m_etItemParam;
	CEdit	m_etItemDBIdx;
	CEdit	m_etItemName;
	CListCtrl	m_lcUsingShop;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUsingShopDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUsingShopDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnClickListUsingShop(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUsiBtnSave();
	afx_msg void OnUsiBtnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USINGSHOPDLG_H__82DCBE3D_1A87_43DB_BC26_319B085183BF__INCLUDED_)

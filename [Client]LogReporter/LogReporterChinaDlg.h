#if !defined(AFX_LOGREPORTERCHINADLG_H__82316A4E_10F4_4425_9B5C_30234BCA47CC__INCLUDED_)
#define AFX_LOGREPORTERCHINADLG_H__82316A4E_10F4_4425_9B5C_30234BCA47CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogReporterChinaDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogReporterChinaDlg dialog

class CLogReporterChinaDlg : public CDialog
{
// Construction
public:
	CLogReporterChinaDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLogReporterChinaDlg)
	enum { IDD = IDD_LOGREPORTER_DIALOG_CHINA };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogReporterChinaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


public:

	//void	CheckFirstRadioButton();
	void	SetCheckButton(BOOL bSouthSelect);

	int IngValue;
	UINT m_nTimer;
	DWORD m_dwStartTime;
	BOOL m_bEnd;


// Button images. Custom Rect values /////////////////////////////////////////

	CRect		m_rcCustomCaption;
	
	CSkinStatic m_txtTitle;
	CSkinStatic m_txtSouthFtp;
	CSkinStatic m_txtNorthFtp;
	CSkinStatic	m_txtEnter;
	CSkinStatic	m_txtQuit;	

	CStatic		m_picBackScreen;
	
	CButton		m_btnSelect;
	CButton		m_btnQuit;
	
	CButton		m_btnNorthCheck;
	CButton		m_btnSouthCheck;

	HBITMAP		m_hBmpBackScreen;

	HBITMAP		m_hBmpEnterBtn;
	HBITMAP		m_hBmpEnterBtnPress;

	HBITMAP		m_hBmpQuitBtn;
	HBITMAP		m_hBmpQuitBtnPress;

	HBITMAP		m_hBmpSouthCheck;
	HBITMAP		m_hBmpSouthCheckPress;

	HBITMAP		m_hBmpNorthCheck;
	HBITMAP		m_hBmpNorthCheckPress;

	BOOL		m_bSouthSelect;	
	
//////////////////////////////////////////////////////////////////////////////

	void LoadRequireImages(void);
	void UnloadRequireImages(void);


public:
	
	void		QuitLogReporterDlg(int iResult);


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLogReporterChinaDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSelect();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonQuit();	
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);	
	afx_msg void OnCheckFtp01();
	afx_msg void OnCheckFtp02();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGREPORTERCHINADLG_H__82316A4E_10F4_4425_9B5C_30234BCA47CC__INCLUDED_)

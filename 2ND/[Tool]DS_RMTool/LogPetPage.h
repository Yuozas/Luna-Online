#if !defined(AFX_LOGPetPAGE_H__1A5D9DC6_8564_4132_9B14_8F9DABF28FC8__INCLUDED_)
#define AFX_LOGPetPAGE_H__1A5D9DC6_8564_4132_9B14_8F9DABF28FC8__INCLUDED_

#include "ViewerOptionDlg.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogPetPage.h : header file
//
#include "LDPropSheet.h"

/////////////////////////////////////////////////////////////////////////////
// CLogPetPage dialog

class CLogDoc;

class CLogPetPage : public CLDPropPage
{
	DECLARE_DYNCREATE(CLogPetPage)

// Construction
public:
	CLogPetPage();
	~CLogPetPage();

	CYHHashTable<LOGPET>	m_PetLogTable;
	CLogDoc*			m_pDocument;
	BOOL				m_bStart;
	CViewerOptionDlg	m_ViewerOptionDlg;
	BOOL				m_bVOption[14];
	DWORD				m_dwCharIdx;

	void	ReleaseLogTable();
	void	SetDocument( CLogDoc* pDoc )	{ m_pDocument = pDoc; }
	void	SetLogPet( TMSG_LOGPET* pMsg );
	void	SetCharacterIdx( DWORD dwCharIdx );
	void	InitComboItemName();
	void	InitLCPetLog();
	void	UpdateLCPetLog( DWORD dwHow );
	char*	GetItemName( DWORD dwItemIdx, char* temp );
	char*	GetLCData( LOGPET* pData, DWORD dwIndex, char* temp );

	virtual void CommandMessage(UINT nID, int nCode);

	CFont	m_Font;


	void	ChangeFont();

// Dialog Data
	//{{AFX_DATA(CLogPetPage)
	enum { IDD = IDD_LOG_PAGE_PET};
	CEdit	m_etEMin;
	CEdit	m_etEHour;
	CEdit	m_etSMin;
	CEdit	m_etSHour;
	CDateTimeCtrl	m_StartTime;
	CDateTimeCtrl	m_EndTime;
	CEdit	m_etCharIdx;
	CComboBox	m_cbItemName;
	CEdit		m_etDBIdx;
	CEdit		m_etCharName;
	CListCtrl	m_lcPetLog;
	BOOL		m_bCharName;
	BOOL		m_bItemName;
	BOOL		m_bDBIdx;
	BOOL	m_bOneDay;
	BOOL	m_bTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CLogPetPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CLogPetPage)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckLogCharname();
	afx_msg void OnCheckLogItemname();
	afx_msg void OnCheckLogDbidx();
	afx_msg void OnButtonLogSearch();
	afx_msg void OnCheckLogOneday();
	afx_msg void OnSelchangeComboViewerHow();
	afx_msg void OnBtnSavetofile();
	afx_msg void OnCheckTime();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnCbnSelchangeComboLogItemname();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGPetPAGE_H__1A5D9DC6_8564_4132_9B14_8F9DABF28FC8__INCLUDED_)

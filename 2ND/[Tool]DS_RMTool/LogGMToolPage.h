#pragma once

#include "LDPropSheet.h"
#include "ViewerOptionDlg.h"

class CLogDoc;

//enum eLOGTYPE {eGMLog_LogIn, eGMLog_Move, eGMLog_Item, eGMLog_Money, eGMLog_Summon,
//eGMLog_SummonDelete, eGMLog_MonsterKill, eGMLog_PKAllow, eGMLog_Disconnect_Map,
//eGMLog_Disconnect_User, eGMLog_Block, eGMLog_Event, eGMLog_Jackpot,	eGMLog_PlusTime};


class cLogGMToolPage :	public CLDPropPage
{
	DECLARE_DYNCREATE(cLogGMToolPage)

	// Construction
public:
	cLogGMToolPage(void);
	~cLogGMToolPage(void);

	CYHHashTable<sGMLogType>	m_LogTable;
	CLogDoc*			m_pDocument;
	BOOL				m_bStart;
	CViewerOptionDlg	m_ViewerOptionDlg;
	BOOL				m_bVOption[14];
	DWORD				m_dwCharIdx;

	void	ReleaseLogTable();
	void	SetDocument( CLogDoc* pDoc )	{ m_pDocument = pDoc; }
	void	SetLogGMTool( MSG_LOGGMTOOL* pMsg );
	void	SetCharacterIdx( DWORD dwCharIdx );
	void	InitComboItemName();
	void	InitLCGMToolLog();
	void	UpdateLCGMToolLog( DWORD dwHow );
	char*	GetLCData( sGMLogType* pData, DWORD dwIndex, char* temp );
	char*	GetKindName( unsigned long logtype, unsigned long logkind, char* temp );

	CFont	m_Font;

	void	ChangeFont();

	// Dialog Data
	//{{AFX_DATA(cLogGMToolPage)
	enum { IDD = IDD_LOG_PAGE_GMTOOL };
	CEdit	m_etEMin;
	CEdit	m_etEHour;
	CEdit	m_etSMin;
	CEdit	m_etSHour;
	CDateTimeCtrl	m_StartTime;
	CDateTimeCtrl	m_EndTime;
	CEdit	m_etCharIdx;
	CComboBox	m_cbLogType;
	CEdit		m_etCharName;
	CListCtrl	m_lcGMToolLog;
	BOOL		m_bCharName;
	BOOL		m_bLogType;
	BOOL	m_bOneDay;
	BOOL	m_bTime;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(cLogGMToolPage)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(cLogGMToolPage)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckLogCharname();
	afx_msg void OnCheckLogItemname();
	afx_msg void OnCheckLogDbidx();
	afx_msg void OnButtonLogSearch();
	afx_msg void OnCheckLogOneday();
	afx_msg void OnBtnSavetofile();
	afx_msg void OnCheckTime();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

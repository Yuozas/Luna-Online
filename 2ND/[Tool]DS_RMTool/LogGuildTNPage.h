/* ==========================================================================
* ��    �� : LogGuildTNPage.h
* ��    �� : ������ʸ�Ʈ �α� ��
* �� �� �� : �ڰ���
* �� �� �� : 2006-08-30
* 
* �߰����� :
*			�߰���¥	�۾���	�߰�����
*
* ���ǻ��� : 
*===========================================================================*/

#if !defined(AFX_LOGGUILDTNPAGE_H__1A5D9DC6_8564_4132_9B14_8F9DABF28FC8__INCLUDED_)
#define AFX_LOGGUILDTNPAGE_H__1A5D9DC6_8564_4132_9B14_8F9DABF28FC8__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogGuildtnPage.h : header file
//
#include "LDPropSheet.h"

/////////////////////////////////////////////////////////////////////////////
// CLogGuildTNPage dialog

class CLogDoc;

class cLogGuildtnPage :	public CLDPropPage
{
	DECLARE_DYNCREATE(cLogGuildtnPage)

// Construction
public:
	cLogGuildtnPage();
	~cLogGuildtnPage();

	CYHHashTable<sGuildtn>	mGuildtnLogTable;
	CLogDoc*				mpDocument;
	BOOL					mStart;
	BOOL					mOption[14];
	DWORD					mGuildIdx;

	void	releaseLogTable();
	void	setDocument( CLogDoc* pDoc )	{ mpDocument = pDoc; }
	void	setLogGuildTN( MSG_LOGGUILDTN* pMsg );
	void	setGuildIdx( unsigned long guildidx );
	void	initLCGuildTNLog();
	void	updateLCGuildTNLog( DWORD dwHow );
	char*	getLCData( sGuildtn* pData, DWORD dwIndex, char* temp );

	CFont	mFont;


	void	ChangeFont();

// Dialog Data
	//{{AFX_DATA(cLogGuildtnPage)
	enum { IDD = IDD_LOG_PAGE_GUILDTN};
	CEdit			mEndMin;
	CEdit			mEndHour;
	CEdit			mStartMin;
	CEdit			mStartHour;
	CDateTimeCtrl	mStartTime;
	CDateTimeCtrl	mEndTime;
	CEdit			mGuildIdxEdit;
	CEdit			mGuildName;
	CListCtrl		mGuildLog;
	BOOL			mChkGuildName;
	BOOL			mOneDay;
	BOOL			mTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(cLogGuildtnPage)
	protected:
	virtual	void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(cLogGuildtnPage)
	afx_msg	int		OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual	BOOL	OnInitDialog();
	afx_msg	void	OnCheckLogGuildname();
	afx_msg	void	OnButtonLogSearch();
	afx_msg	void	OnCheckLogOneday();
	afx_msg	void	OnSelchangeComboViewerHow();
	afx_msg	void	OnBtnSavetofile();
	afx_msg	void	OnCheckTime();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg	void	OnCbnSelchangeComboLogItemname();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGGUILDTNPAGE_H__1A5D9DC6_8564_4132_9B14_8F9DABF28FC8__INCLUDED_)

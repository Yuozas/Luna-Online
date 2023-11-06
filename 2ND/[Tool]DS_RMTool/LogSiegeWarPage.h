/* ==========================================================================
* 파    일 : LogSiegeWarPage.h
* 목    적 : 공성전 로그 뷰
* 작 성 자 : 박경희
* 작 성 일 : 2006-08-31
* 
* 추가내용 :
*			추가날짜	작업자	추가내용
*
* 주의사항 : 
*===========================================================================*/

#if !defined(AFX_LOGSIEGEWARPAGE_H__1A5D9DC6_8564_4132_9B14_8F9DABF28FC8__INCLUDED_)
#define AFX_LOGSIEGEWARPAGE_H__1A5D9DC6_8564_4132_9B14_8F9DABF28FC8__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogGuildtnPage.h : header file
//
#include "LDPropSheet.h"

/////////////////////////////////////////////////////////////////////////////
// cLogSiegeWarPage dialog

class CLogDoc;

class cLogSiegeWarPage :	public CLDPropPage
{
	DECLARE_DYNCREATE(cLogSiegeWarPage)

// Construction
public:
	cLogSiegeWarPage();
	~cLogSiegeWarPage();

	CYHHashTable<sSiegeWar>	mSiegeWarLogTable;
	CLogDoc*				mpDocument;
	BOOL					mStart;
	BOOL					mOption[14];
	DWORD					mGuildIdx;

	void	releaseLogTable();
	void	setDocument( CLogDoc* pDoc )	{ mpDocument = pDoc; }
	void	setLogSiegeWar( MSG_LOGSIEGEWAR* pMsg );
	void	setGuildIdx( unsigned long guildidx );
	void	initLCSiegeWarLog();
	void	updateLCSiegeWarLog( DWORD dwHow );
	char*	getLCData( sSiegeWar* pData, DWORD dwIndex, char* temp );

	CFont	mFont;


	void	ChangeFont();

// Dialog Data
	//{{AFX_DATA(cLogSiegeWarPage)
	enum { IDD = IDD_LOG_PAGE_SIEGEWAR};
	CEdit			mEndMin;
	CEdit			mEndHour;
	CEdit			mStartMin;
	CEdit			mStartHour;
	CDateTimeCtrl	mStartTime;
	CDateTimeCtrl	mEndTime;
	CEdit			mGuildIdxEdit;
	CEdit			mGuildName;
	CListCtrl		mSiegeWarLog;
	BOOL			mChkGuildName;
	BOOL			mOneDay;
	BOOL			mTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(cLogSiegeWarPage)
	protected:
	virtual	void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(cLogSiegeWarPage)
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

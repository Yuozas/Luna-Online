// LogReporter.h : main header file for the LOGREPORTER application
//

#if !defined(AFX_LOGREPORTER_H__EADDF8FA_2404_4574_BCB5_32E503B13003__INCLUDED_)
#define AFX_LOGREPORTER_H__EADDF8FA_2404_4574_BCB5_32E503B13003__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLogReporterApp:
// See LogReporter.cpp for the implementation of this class
//

class CLogReporterApp : public CWinApp
{
public:
	CLogReporterApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogReporterApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLogReporterApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGREPORTER_H__EADDF8FA_2404_4574_BCB5_32E503B13003__INCLUDED_)

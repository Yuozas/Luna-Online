/////////////////////////////////////////////////////////////////////////////
// CDelCharView 뷰입니다.
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DELCHARVIEW_H__E6C7C107_F059_4B94_945B_4B70A8A9349B__INCLUDED_)
#define AFX_DELCHARVIEW_H__E6C7C107_F059_4B94_945B_4B70A8A9349B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LDPropSheet.h"
#include "CommonRMControl.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CLDPropPage;
class CLDPropSheet;
class CDelChar;
class CDelCharDoc;
//class CItemDlg;

/////////////////////////////////////////////////////////////////////////////

class CDelCharView : public CFormView
{
protected:
	CDelCharView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	DECLARE_DYNCREATE(CDelCharView)

// Form Data
public:
	//{{AFX_DATA(CDelCharView)
	enum { IDD = IDD_DEL_CHAR_SHEET };
	//}}AFX_DATA

// Attributes
public:
	// Sheet and Page
	CLDPropSheet*		m_pSheet;
	CLDPropPage*		m_pPageList;
	//

// Operations
public:

	CDelCharDoc*		GetDocument();

	void				SetDelCharList( DWORD Count, TMSG_DELCHARLISRESULT* pMsg );

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMunpaView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	virtual ~CDelCharView();

public:

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif



// Generated message map functions
protected:
	//{{AFX_MSG(CDS_RMToolView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


#ifndef _DEBUG  // debug version in DS_RMToolView.cpp
inline CDelCharDoc* CDelCharView::GetDocument()
   { return (CDelCharDoc*)m_pDocument; }
#endif


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DELCHARVIEW_H__E6C7C107_F059_4B94_945B_4B70A8A9349B__INCLUDED_)

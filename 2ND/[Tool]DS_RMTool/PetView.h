#if !defined(AFX_PETVIEW_H__E6C7C107_F059_4B94_945B_4B70A8A9349B__INCLUDED_)
#define AFX_PETVIEW_H__E6C7C107_F059_4B94_945B_4B70A8A9349B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MunpaView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMunpaView form view
#include "LDPropSheet.h"
#include "CommonRMControl.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CLDPropPage;
class CLDPropSheet;
class CPet;
class CPetDoc;
class CItemDlg;

// CPetView 뷰입니다.

enum ePetPage
{
	ePetPage_PetInfo = 0, ePetPage_PetItem, ePetPage_Max, 
};

class CPetView : public CFormView
{
protected:
	CPetView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	DECLARE_DYNCREATE(CPetView)

// Form Data
public:
	//{{AFX_DATA(CPetView)
	enum { IDD = IDD_PET_SHEET };
	//}}AFX_DATA

// Attributes
public:
	// Sheet and Page
	CLDPropSheet*		m_pSheet;
	CLDPropPage*		m_pPageList[ePetPage_Max];
	//





// Operations
public:

	CPetDoc* GetDocument();

	CPet*			GetPetPage() { return (CPet*)m_pPageList[ePetPage_PetInfo]; }

	void			SetPetInfo( TMSG_PETINFO* pMsg );
	void			SetPetWareItemInfo( TMSG_CHARACTER_ITEMBASE* pMsg );
	void			SetPetAccItemInfo( TMSG_CHARACTER_ITEMBASE* pMsg );

public:
	void	EnableRecoverButton();
	void	RecoverError( DWORD dwKind );
	void	ChangeUserLevel( DWORD dwUserLevel );
	void	SetPetList( DWORD Count, TMSG_PETLISTRESULT* pMsg );
	void	SetUserLoginChk( TMSG_USERLOGINCHK* pMsg );
	void	SearchbtnEnable( int BOOL );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMunpaView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	virtual ~CPetView();

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
inline CPetDoc* CPetView::GetDocument()
   { return (CPetDoc*)m_pDocument; }
#endif


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MUNPAVIEW_H__E6C7C107_F059_4B94_945B_4B70A8A9349B__INCLUDED_)

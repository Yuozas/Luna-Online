#if !defined(AFX_DELCHAR_H__F78D1D8A_942E_4D24_A57D_0B47A6F7573F__INCLUDED_)
#define AFX_DELCHAR_H__F78D1D8A_942E_4D24_A57D_0B47A6F7573F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DelChar.h : header file
//

#include "LDPropSheet.h"
#include "CommonRMControl.h"

#include "MASInfoManager.h"

/////////////////////////////////////////////////////////////////////////////
// CDelChar 대화 상자입니다.
	
class CDelCharDoc;


class CDelChar : public CLDPropPage
{
	DECLARE_DYNAMIC(CDelChar)

// Attributes
public:
	CDelCharDoc* GetDocument();

public:
	CDelChar();   	// 표준 생성자입니다.
	~CDelChar();

	CDelCharDoc*					m_pDocument;
	DWORD							m_dwDocumentIdx;

	CYHHashTable<DELCHARLIST>		m_DelCharTable;

public:

	void							InitDelCharListControl();
	void							SetDelCharList( DWORD Count, TMSG_DELCHARLISRESULT* pMsg );
	virtual void					InitPage(void* pData) { m_pDocument = (CDelCharDoc*)pData; }
	void							ReleaseDelCharTable();

// 대화 상자 데이터입니다.
		//{{AFX_DATA(CDelCharInfo)
	enum { IDD = IDC_DEL_CHAR };
	CButton		m_btnSearch;
	CEdit		m_etSearch;
	CListCtrl	m_lCDelChar;
	//}}AFX_DATA

protected:
// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CGuildInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(CGuildInfo)
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSearch();
	afx_msg void OnDblclkDelCharListctrl(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DELCHAR_H__F78D1D8A_942E_4D24_A57D_0B47A6F7573F__INCLUDED_)

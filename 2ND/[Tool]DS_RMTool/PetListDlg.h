#if !defined(AFX_PETLISTDLG_H__6F10E07A_FAD9_4C13_A6FD_6701B270DFA3__INCLUDED_)
#define AFX_PETLISTDLG_H__6F10E07A_FAD9_4C13_A6FD_6701B270DFA3__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Pet.h"
#include "CommonRMControl.h"
#include "DS_RMToolDoc.h"

// CPetListDlg 대화 상자입니다.

class CPet;

class CPetListDlg : public CDialog
{
	DECLARE_DYNAMIC(CPetListDlg)

public:
	CPetListDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPetListDlg();

	CPet		m_Pet;

	CFont		m_Font;
	DWORD		m_dwDocumentIdx;
	DWORD		m_dwCharIdx;
	char		m_sCharName[32];
	BOOL		m_bInit;
	POSITION	m_Pos;

	void		SetInitData( DWORD dwDocumentIdx, DWORD dwCharIdx, char* sCharName );
	void		InitPetListCtrl();
	void		SendMainPetList();

	void		UpdatePetListCtrl( TMSG_MAINPETLIST* pMsg );

//	CDS_RMToolDoc* GetDocument();
//	CDS_RMToolDoc* m_pDocument;

// 대화 상자 데이터입니다.
	//{{AFX_DATA(CQuestInfoDlg)
	enum { IDD = IDD_PET_DLG };
	CEdit	m_etCharName;
	CListCtrl	m_lcPet;
	//}}AFX_DATA

protected:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQuestInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUA

protected:

	// Generated message map functions
	//{{AFX_MSG(CQuestInfoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnDblclkPetListctrl(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#endif // !defined(AFX_QUESTINFODLG_H__6F10E07A_FAD9_4C13_A6FD_6701B270DFA3__INCLUDED_)

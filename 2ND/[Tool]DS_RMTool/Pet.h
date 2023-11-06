#if !defined(AFX_PET_H__F78D1D8A_942E_4D24_A57D_0B47A6F7573F__INCLUDED_)
#define AFX_PET_H__F78D1D8A_942E_4D24_A57D_0B47A6F7573F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Pet.h : header file
//

#include "LDPropSheet.h"
#include "CommonRMControl.h"

#include "MASInfoManager.h"

#define		ERRORSTRUCT		5
/////////////////////////////////////////////////////////////////////////////
// CPet 대화 상자입니다.
	
class CPetDoc;
class CItemDlg;
class CItemRecoverDlg;


class CPet : public CLDPropPage
{
	DECLARE_DYNAMIC(CPet)

// Attributes
public:
	CPetDoc* GetDocument();

	// 표준 생성자입니다.
public:
	CPet();   
	~CPet();

	CPetDoc*						m_pDocument;
	CItemRecoverDlg*				m_pItemRecoverDlg;
	TMSG_CHARACTER_ITEMBASE			m_InvenInfoError;
	DWORD							m_dwPetDBIdx;
	DWORD							m_dwDocumentIdx;
	CItemDlg*						m_pItemDlg;
	WORD							m_wPower;

	CYHHashTable<PETBASEINFO>		m_PetTable;

	PETINVEN_ITEM					m_WareItemInfo_org;
	ITEM_OPTION_INFO				m_WareItemOptionInfo_org[TABCELL_PETINVEN_NUM];
	PETINVEN_ITEM					m_WareItemInfo;
	ITEM_OPTION_INFO				m_WareItemOptionInfo[TABCELL_PETINVEN_NUM];
	PETACC_ITEM						m_AccItemInfo_org;
	ITEM_OPTION_INFO				m_AccItemOptionInfo_org[SLOT_PETWEAR_NUM];
	PETACC_ITEM						m_AccItemInfo;
	ITEM_OPTION_INFO				m_AccItemOptionInfo[SLOT_PETWEAR_NUM];

public:

	CMRButton						m_cmrb_petacc[SLOT_PETWEAR_NUM];
	CMRButton						m_cmrb_petinven[TABCELL_PETINVEN_NUM];
	CMRButton						m_InvenItemError[5];
	CMREdit							m_StatusEdit[ePetTemp_MAX];

	TABLE_KIND						GetTableKindByControlID(UINT nId);

	void							ButtonControlCommand( UINT nId );
	void							EditControlCommand(UINT nId);

	int								ItemEdit( INFOFIELD field, WORD wControlId, const ITEMBASE* base_before, ITEMBASE& base_after,
											  const ITEM_OPTION_INFO* option_before, ITEM_OPTION_INFO& option_after );
	BOOL							ResultItemEdit( INFOFIELD field, int result, WORD wPos, int nControlIdx, ITEMBASE* Origin, ITEM_OPTION_INFO* OriginOption,
													ITEMBASE* pBItem, ITEMBASE* pAItem, ITEM_OPTION_INFO* pBOption, ITEM_OPTION_INFO* pAOption );
	void							ReleasePetTable();
	void							InitPetInfo();
	void							SetPetInfo( TMSG_PETINFO* pMsg );
	char*							GetStateRest( DWORD dwStateRes, char* temp );
	char*							GetStateAlive( DWORD dwStateAlive, char* temp );
	void							SetPetWareItemInfo( TMSG_CHARACTER_ITEMBASE* pMsg );
	void							InitPetListControl();
	void							SetPetList( DWORD Count, TMSG_PETLISTRESULT* pMsg );
	void							SetPetAccItemInfo( TMSG_CHARACTER_ITEMBASE* pMsg );
	void							SetUserLoginChk( TMSG_USERLOGINCHK* pMsg );
	int								GetEmptyPositionCount( WORD wType );
	void							RecoverInven( DWORD dwItemIdx, int nCount, int nR );
	int								GetEmptyPosition( DWORD dwType );
	void							GetChangeBaseInfo(TMSG_PET_UPDATEBASEINFO* msg);
	void							InitControl();
	void							SetInvenInfoError( TMSG_CHARACTER_ITEMBASE* inven );
	void							ResultPositionEdit( DWORD dwType, ICONBASE* pItem );
	void							SearchbtnEnable(int BOOL);

	virtual void					InitPage(void* pData) { m_pDocument = (CPetDoc*)pData; }

// 대화 상자 데이터입니다.
		//{{AFX_DATA(CGuildInfo)
	enum { IDD = IDD_PET };
	CButton		m_btnSearch;
	CComboBox	m_cbPetWare;
	CMRButton	m_cmrbPetAction;
	CMRButton	m_cmrbPetAlive;
	CEdit		m_etindex;
	CEdit		m_ettype;
	CEdit		m_etowner;
	CButton		m_btnUserSearch;
	CEdit		m_etSearch;
	CListCtrl	m_lcPet;
	CButton		m_btnpetsave;
	CStatic		m_stStatus;
	CEdit		m_etactionview;
	CEdit		m_etaliveview;
	//}}AFX_DATA

protected:
// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CGuildInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(CGuildInfo)
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSearch();
	afx_msg void OnBnClickedStateActive();
	afx_msg void OnBnClickedStateAlive();
	afx_msg void OnSelchangeComboPetware();
	afx_msg void OnBnClickedRecover();
	afx_msg void OnUserbtnSearch();
	afx_msg void OnRadioUserID();
	afx_msg void OnRadioCharName();
	afx_msg void OnClickPetList(NMHDR* pNMHDR, LRESULT* pResult); 
	afx_msg void OnDblclkPetListctrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnCheckname();
	afx_msg void OnBtnRecover();
	afx_msg void OnButtonChangeUserlevel();
	afx_msg void OnBnClickedPetSave();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PET_H__F78D1D8A_942E_4D24_A57D_0B47A6F7573F__INCLUDED_)

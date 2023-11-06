#if !defined(AFX_GUILDINFO_H__71710008_6A97_4712_A065_163D8EDF5CF5__INCLUDED_)
#define AFX_GUILDINFO_H__71710008_6A97_4712_A065_163D8EDF5CF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GuildInfo.h : header file
//
#include "LDPropSheet.h"
#include "CommonRMControl.h"

class CItemDlg;

/////////////////////////////////////////////////////////////////////////////
// CGuildInfo dialog
class CMunpaDoc;

class CGuildInfo : public CLDPropPage
{
	DECLARE_DYNCREATE(CGuildInfo)

public:
	CMunpaDoc*						m_pDocument;
	CYHHashTable<GUILDBASEINFO>		m_GuildTable;
	CYHHashTable<TGUILDMEMBERINFO>	m_GuildMemberTable;

	CMRButton	m_GuildWareItem[TABCELL_GUILDWAREHOUSE_NUM];
	MUNPA_WAREHOUSE_ITEM_INFO		m_WareItemInfo_org;
	ITEM_OPTION_INFO				m_WareItemOptionInfo_org[TABCELL_GUILDWAREHOUSE_NUM];

	MUNPA_WAREHOUSE_ITEM_INFO		m_WareItemInfo;
	ITEM_OPTION_INFO				m_WareItemOptionInfo[TABCELL_GUILDWAREHOUSE_NUM];

	CItemDlg*						m_pItemDlg;
		
// Construction
public:
	CGuildInfo();
	~CGuildInfo();

	virtual void	InitPage( void* pData )		{ m_pDocument = (CMunpaDoc*)pData; }

	void	ReleaseGuildTable();
	void	ReleaseGuildMemberTable();
	void	SetGuildList( TMSG_GUILD* pMsg );
	void	SetGuildInfo( TMSG_GUILD* pMsg );
	void	SetGuildMemberList( TMSG_GUILDMEMBERINFO* pMsg );
	void	SetGuildWareItemInfo( TMSG_CHARACTER_ITEMBASE* pMsg );
	void	SetGuildWareItemOptionInfo( TMSG_ITEMOPTIONINFO2* pMsg );

	void	InitGuildListControl();
	void	UpdateGuildListControl();
	void	InitGuildMemberListControl();
	void	UpdateGuildMemberListControl();

	int		GetItemPositionFromOptionDBIdx( DWORD dwOptionDBIdx );

	void	ButtonControlCommand( UINT nId );

	int		ItemEdit( INFOFIELD field, WORD wControlId, const ITEMBASE* base_before, ITEMBASE& base_after,
					  const ITEM_OPTION_INFO* option_before, ITEM_OPTION_INFO& option_after );
	BOOL	ResultItemEdit( INFOFIELD field, int result, WORD wPos, int nControlIdx, ITEMBASE* Origin, ITEM_OPTION_INFO* OriginOption,
							ITEMBASE* pBItem, ITEMBASE* pAItem, ITEM_OPTION_INFO* pBOption, ITEM_OPTION_INFO* pAOption );

// Dialog Data
	//{{AFX_DATA(CGuildInfo)
	enum { IDD = IDD_GUILD_PAGE_GUILD };
	CEdit	m_etGuildMoney;
	CComboBox	m_cbGuildWare;
	CEdit	m_etMemberNum;
	CEdit	m_etGuildMasterName;
	CEdit	m_etGuildName;
	CEdit	m_etGuildMap;
	CEdit	m_etGuildLevel;
	CEdit	m_etSearchGuildName;
	CListCtrl	m_lcGuildMember;
	CListCtrl	m_lcGuild;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CGuildInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CGuildInfo)
	afx_msg void OnSearchGuildTotal();
	afx_msg void OnSearchGuild();
	afx_msg void OnSelchangeComboGuildware();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUILDINFO_H__71710008_6A97_4712_A065_163D8EDF5CF5__INCLUDED_)

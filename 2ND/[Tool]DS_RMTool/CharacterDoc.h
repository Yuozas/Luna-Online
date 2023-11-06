#if !defined(AFX_CHARACTERDOC_H__AFB4C71C_76F3_4C34_B5DC_0C22FA5009EE__INCLUDED_)
#define AFX_CHARACTERDOC_H__AFB4C71C_76F3_4C34_B5DC_0C22FA5009EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CharacterDoc.h : header file
//
#include "basedoc.h"
#include "IndexGenerator.h"

/////////////////////////////////////////////////////////////////////////////
// CCharacterDoc document

class CCharacterDoc : public CBaseDoc
{
protected:
	CCharacterDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCharacterDoc)

// Attributes
protected:

public:
	DWORD							m_dwUserIdx;
	DWORD							m_dwCharIdx;
 
	BASEOBJECT_INFO					m_sBaseInfo;
	CHARACTER_TOTALINFO				m_sTotalInfo;
	HERO_TOTALINFO					m_sHTotalInfo;
	// orginal
	ITEM_TOTALINFO					m_sInven_org;
	MUGONG_TOTALINFO				m_sMugong_org;
	PHYOKUK_TOTALINFO				m_sPyoguk_org;
	ABILITY_TOTALINFO				m_sAbility_org;
	SHOP_ITEM						m_sShopWare_org;
	CYHHashTable<ITEM_OPTION_INFO>	m_ItemOptionInfo_org;
	CYHHashTable<ITEM_RARE_OPTION_INFO>	m_ItemRareOptionInfo_org;
	// view
	ITEM_TOTALINFO					m_sInven;
	MUGONG_TOTALINFO				m_sMugong;
	PHYOKUK_TOTALINFO				m_sPyoguk;
	ABILITY_TOTALINFO				m_sAbility;
	SHOP_ITEM						m_sShopWare;
	CYHHashTable<ITEM_OPTION_INFO>	m_ItemOptionInfo;
	CYHHashTable<ITEM_RARE_OPTION_INFO>	m_ItemRareOptionInfo;
	// error
	TMSG_CHARACTER_ITEMBASE			m_InvenInfoError;
	PHYOKUK_TOTALINFO				m_WareError;
	TMSG_CHARACTER_MUGONGINFO2		m_MugongError;
	// key
	CIndexGenerator m_OptionKeyGenerator;

public:
	virtual void		InitData();
	virtual void		OpenData();
	virtual void		UpdateData(DWORD dwParam1=0, DWORD dwParam2=0);
	virtual void		SaveData();

	void	CharacterInfoSaveDataSend();
	void	PyogukInfoSaveDataSend();
	void	AbilitySaveDataSend();

	BOOL	IsLoadingAll()	{ return (m_dwLoadingFlag==eCL_LoadingAll)? TRUE:FALSE; }

	void	SetCharacterBaseInfo( BASEOBJECT_INFO* objInfo, CHARACTER_TOTALINFO* totalInfo, HERO_TOTALINFO* heroInfo);
	void	SetCharacterInvenInfo( TMSG_CHARACTER_ITEMBASE* info );
	void	SetCharacterInvenInfo2( TMSG_CHARACTER_ITEMBASE* info );
	void	SetCharacterPyogukInfo( WORD wNum, MONEYTYPE money );
	void	SetCharacterPyogukItemInfo( WORD wCount, ITEMBASE* item, BOOL bEnd );
	void	SetCharacterMugongInfo( MUGONG_TOTALINFO* info);
	void	SetCharacterMugongInfoError( TMSG_CHARACTER_MUGONGINFO2* pinfo );
	void	SetCharacterAbillityInfo( ABILITY_TOTALINFO* info);
	void	SetCharacterItemOptionInfo( WORD wCount, ITEM_OPTION_INFO* info, BOOL bEnd );
	void	SetCharacterShopWareItemInfo( TMSG_CHARACTER_ITEMBASE* info );
	void	SetCharacterRareItemOptionInfo( WORD wCount, ITEM_RARE_OPTION_INFO* info, BOOL bEnd );

	BYTE	GetCharacterAbillityLevel(BYTE kind, WORD Index);
	ITEM_OPTION_INFO*	GetItemOptionInfo(DWORD dwOptionIdx);
	ITEM_OPTION_INFO*	GetItemOptionInfoOrigin(DWORD dwOptionIdx);

	ITEM_RARE_OPTION_INFO*	GetRareItemOptionInfo( DWORD dwRareIdx );
	
	void	SetAbilityLevel(BYTE kind, WORD Index, char level);

	DWORD	AddLocalOptionInfo( ITEM_OPTION_INFO* pLocalOption, BOOL bNew = TRUE );
	void	DelLocalOptionInfo( DWORD Index );
	DWORD	CreateLocalOptionIndex();
	
	void	ChangeLoginPoint( DWORD dwLoginPoint );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCharacterDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCharacterDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void	OpenCharacterInfo(DWORD dwServer, DWORD dwUserIdx, DWORD dwCharIdx, BOOL bReadOnly = FALSE);


	// Generated message map functions
protected:
	//{{AFX_MSG(CCharacterDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void	NetworkMsgParse(BYTE Protocol, void* pMsg);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARACTERDOC_H__AFB4C71C_76F3_4C34_B5DC_0C22FA5009EE__INCLUDED_)

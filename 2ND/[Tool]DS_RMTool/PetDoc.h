#if !defined(AFX_PETDOC_H__B85A4AAC_B586_4CB3_BA59_064EC6811B9E__INCLUDED_)
#define AFX_PETDOC_H__B85A4AAC_B586_4CB3_BA59_064EC6811B9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MunpaDoc.h : header file
//

#include "basedoc.h"
#include "IndexGenerator.h"

/////////////////////////////////////////////////////////////////////////////
// CPetDoc 문서입니다.

class CPetDoc : public CBaseDoc
{
protected:
	CPetDoc(); 
	DECLARE_DYNCREATE(CPetDoc)
	

// Attributes
public:
	OPERATOR_TOTALINFO	m_sOperator;
	CYHHashTable<ITEM_OPTION_INFO>	m_ItemOptionInfo;
	CYHHashTable<ITEM_OPTION_INFO>	m_ItemOptionInfo_org;
	CYHHashTable<ITEM_RARE_OPTION_INFO>	m_ItemRareOptionInfo;
	CIndexGenerator m_OptionKeyGenerator;

	virtual void		SaveData();

	void	SetOperatorTotalInfo(WORD count, OPERATOR* pOper, BOOL bEnd);
	void	SetIpTotalInfo(WORD count, IPINFO* pIP, BOOL bEnd);
	BOOL	IsLoadingAll() { return (m_dwLoadingFlag==eAL_LoadingAll)? TRUE:FALSE; }

	void	SetPetBaseInfo(TMSG_PETBASEINFO* info);

	void	SetUserInfo(USERINFO* user);

	void	DelLocalOptionInfo(DWORD dwIndex);
	DWORD	AddLocalOptionInfo( ITEM_OPTION_INFO* pLocalOption, BOOL bNew );
	DWORD	CreateLocalOptionIndex();

	//TMSG_PETBASEINFO m_PetBaseInfo_org;
	//TMSG_PETINFO		m_PetBaseInfo_org;
	DWORD				m_dwPetDBIdx;
	char				m_UserId[255];

	USERINFO			m_sUserInfoOrg;	
	USERINFO*			m_sUserInfo;

	void				PetInfoSaveDataSend();
	BASEOBJECT_INFO		m_sBaseInfo;

	virtual void		UpdateData(DWORD dwParam1=0, DWORD dwParam2=0);

	ITEM_OPTION_INFO*	GetItemOptionInfo(DWORD dwOptionIdx);
	ITEM_OPTION_INFO*	GetItemOptionInfoOrigin(DWORD dwOptionIdx);
	ITEM_RARE_OPTION_INFO*	GetRareItemOptionInfo( DWORD dwRareIdx );

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdminDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual void OnCloseDocument();
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Operations
public:
	virtual void		OpenData();
	void				SetInitData( DWORD dwPetDBIdx );


public:
	virtual ~CPetDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()

	void NetworkMsgParse(BYTE Protocol, void* pMsg);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MUNPADOC_H__B85A4AAC_B586_4CB3_BA59_064EC6811B9E__INCLUDED_)
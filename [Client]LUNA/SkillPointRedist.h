// SkillPointRedist.h: interface for the CSkillPointRedist class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLPOINTREDIST_H__28C269C4_1AF1_429A_B3CE_7A75D9D4A5A5__INCLUDED_)
#define AFX_SKILLPOINTREDIST_H__28C269C4_1AF1_429A_B3CE_7A75D9D4A5A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "./Interface/cDialog.h"


class cButton;
class cPushupButton;
class cStatic;
class cIconGridDialog;


enum
{
	eTab_War=0,
	eTab_KyungGong,
	eTab_Character,
	eTab_Max,
};



class CSkillPointRedist : public cDialog
{
	cButton*			m_UpBtn;
	cButton*			m_DownBtn;
	cButton*			m_OkBtn;
	cStatic*			m_RePointst;
	cStatic*			m_UsePointst;
	cStatic*			m_OgPointst;
	cPushupButton*		m_GridButton[eTab_Max];
	cIconGridDialog*	m_IconGrid[eTab_Max];

	DWORD				m_ItemIdx;
	DWORD				m_ItemPos;
	DWORD				m_dwCurTabNum;

public:
	CSkillPointRedist();
	virtual ~CSkillPointRedist();

	virtual void SetActive( BOOL val );
	void Linking();
};

#endif // !defined(AFX_SKILLPOINTREDIST_H__28C269C4_1AF1_429A_B3CE_7A75D9D4A5A5__INCLUDED_)

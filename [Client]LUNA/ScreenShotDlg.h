// CScreenShotDlg.h: interface for the CScreenShotDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CScreenShotDlg_H__73CE85B3_4B5D_4484_A158_A3067F4458BF__INCLUDED_)
#define AFX_CScreenShotDlg_H__73CE85B3_4B5D_4484_A158_A3067F4458BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "./Interface/cDialog.h"

class cStatic;

class CScreenShotDlg :public cDialog
{
protected:	
	cStatic*		m_sttime;		//�����α��� ��¿� static
	cStatic*		m_playtime;		//�÷���Ÿ�� ��¿� static
	BOOL			m_bShow;

	cImage			m_Image;
	SYSTEMTIME		serverti;		//���� �α��� �ð�

public:
	CScreenShotDlg();
	virtual ~CScreenShotDlg();

	virtual void	SetActive( BOOL val, DWORD HeroID );
	virtual void	Render();
	void			Linking();

	void			Setserverti(SYSTEMTIME servertime);
	void			ViewDatetime(DWORD HeroID);

};

#endif // !defined(AFX_CScreenShotDlg_H__73CE85B3_4B5D_4484_A158_A3067F4458BF__INCLUDED_)
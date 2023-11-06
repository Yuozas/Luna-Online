//////////////////////////////////////////////////////////////////////
// class name : cButton 
// Lastest update : 2002.10.29. by taiyo
//////////////////////////////////////////////////////////////////////

#ifndef _cBUTTON_H_
#define _cBUTTON_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cWindow.h"
#include "cWindowDef.h"

// 071004 LYW --- cButton : Define sound index to use button.
#define		CLICK_SOUND_IDX		115
#define		OVER_SOUND_IDX		116





class cButton : public cWindow  
{
public:
	cButton();
	virtual ~cButton();

	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, cImage * overImage, cImage * pressImage, cbFUNC Func, LONG ID=0);
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual void Render();
	void SetText(char * text, DWORD basicColor, DWORD overColor=NULL, DWORD PressColor=NULL);
	void SetTextValue(DWORD value);
	void SetTextXY(LONG x, LONG y) { m_textXY.x = x; m_textXY.y = y; }

	BOOL IsClickInside() { return m_bClickInside; }
	void SetTextAni( BYTE x, BYTE y ) { m_BtnTextAniX = x; m_BtnTextAniY = y; }

	void SetClickSound( SNDIDX idx ) { m_sndIdx = idx; m_bSound = TRUE; }

	// 071004 LYW --- cButton : Add function to setting sound index when the mouse is over to button.
	void SetOverSound( SNDIDX idx ) { m_osndIdx = idx ; m_bOSound = TRUE ; }
	
	void SetShadowTextXY(LONG x, LONG y)	{	m_shadowTextXY.x = x; m_shadowTextXY.y = y;		}
	void SetShadowColor(DWORD color)		{	m_shadowColor = color;							}
	void SetShadow(BOOL val)				{	m_bShadow = val;								}
	
//KES INTERFACE 031028	
	void SetAlign( int nAlign )				{ m_nAlign = nAlign; }
//
	// 070109 LYW --- Add function to return color.
	DWORD GetBasicColor() { return m_fgBasicColor ; }
	DWORD GetOverColor() { return m_fgOverColor ; }
	DWORD GetPressColor() { return m_fgPressColor ; }

	// 070209 LYW --- cButton : Add function to setting area of render part.
	void SetRenderArea() ;

	// 070409 LYW --- cButton : Add function to return text.
	char* GetButtonText() { return m_szText ; }

	virtual void Add( cWindow* );

protected:	
	cImage	m_OverImage;
	cImage	m_PressImage;
	
	cCoord m_textXY;
	
	// Text label of button
	int m_nCurTextLen;
	char m_szText[MAX_TEXT_SIZE];
	DWORD m_fgBasicColor;
	DWORD m_fgOverColor;
	DWORD m_fgPressColor;
	DWORD m_fgCurColor;
	
//KES INTERFACE 031006
	BOOL m_bClickInside;
	BYTE m_BtnTextAniX;
	BYTE m_BtnTextAniY;
	int	 m_nAlign;

	SNDIDX	m_sndIdx;
	BOOL	m_bSound;

	// 071004 LYW --- cButton : Add sound for mouse over part.
	SNDIDX	m_osndIdx ;											// ���콺 ������ �÷��� ��ų ���� �ε����� ��� ����.
	BOOL	m_bOSound ;											// ���� ���带 �÷��� ��ų �� ���θ� ��� ����.

	BOOL	m_bPlayOverSound ;									// ���� ���尡 �÷��� �Ǿ����� üũ�ϱ� ���� ����.
	
	BOOL			m_bShadow;
	cCoord			m_shadowTextXY;
	DWORD			m_shadowColor;
};

#endif // _cBUTTON_H_

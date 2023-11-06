// cFont.h: interface for the cFont class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _cFONT_H_
#define _cFONT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "cWindowDef.h"
#include "../Engine/GraphicEngine.h"
#include "../4DyuchiGRX_common/IGeometry.h"


#define CFONT_OBJ	USINGTON(cFont)

struct sGAMENOTIFY_MSG ;																// 게임 공지 메시지 구조체를 선언한다.

typedef struct _FONT_DESC
{
	WORD wWidth;
	WORD wHeight;
	//WORD wHalfWidth;
	//WORD wHalfHeight;
}FONT_DESC;

class cFont  
{
	HDC m_hdc ;
public:
	// 070329 LYW --- cFont : Add font type.
	//enum FONT_KIND{FONT0=0, FONT1, FONT2, FONT3, FONT4, FONTMAX};
	enum FONT_KIND{FONT0=0, FONT1, FONT2, FONT3, FONT4, FONT5, FONT6, FONT7, FONTMAX};
	cFont();
	virtual ~cFont();
	void Release();
	// GetInstance()
//	MAKESINGLETON(cFont);
	BOOL CreateFontObject(LOGFONT *pFont, WORD idx);
	void RenderFont(WORD wFontIdx, char * text, int size, RECT * pRect, DWORD color);

//임시
	void RenderFontAlpha(WORD wFontIdx, char * text, int size, RECT * pRect, DWORD color);

	IDIFontObject * GetFont(WORD idx)
	{
		#ifdef _DEBUG
		if(idx>FONTMAX||idx<0) return NULL;
		#endif //_DEBUG
		return m_pFont[idx];
	}
	BOOL IsCreateFont(WORD idx)
	{
		if(idx>FONTMAX||idx<0) return FALSE;
		if(m_pFont[idx]) return TRUE;
		else return FALSE;
	}
	LONG GetTextExtent(WORD hFIdx, char *str, LONG strSize);
	LONG GetTextHeight(WORD hFIdx) { return m_FontDesc[hFIdx].wHeight; }
	LONG GetTextWidth(WORD hFIdx) { return m_FontDesc[hFIdx].wWidth; }
	IDIFontObject * m_pFont[FONTMAX];
	FONT_DESC m_FontDesc[FONTMAX];
	
//KES 030915
	HFONT m_hFont[FONTMAX];
	LOGFONT m_LogFont[FONTMAX];
	LONG GetTextExtentEx( WORD hFIdx, char* str, int strLen );
	// 070624 LYW --- cFont : Add function to return text extent.
	// 현재 자간사이를 계산하지 않고 리턴하는 함수를 계산하는 함수로 처리.
	LONG GetTextExtentWidth( WORD hFIdx, char* str, int strLen ) ;

// 070511 LYW --- cFont : Add function to render font to shadow.
/////////////////////////////////////////////////////////
// RENDER FONT TO SHADOW PART.
/////////////////////////////////////////////////////////
private :
	/*
	DWORD	m_dwFrontColor ;
	DWORD	m_dwShadowColor ;

	LONG	m_lXGap ;
	LONG	m_lYGap ;
	*/
public :
	/*
	void	SetFrontColor( DWORD dwFrontColor )   { m_dwFrontColor = dwFrontColor ; }
	void	SetShadowColor( DWORD dwShadowColor ) { m_dwShadowColor = dwShadowColor ; }
	void	SetShadowGap( LONG xGap, LONG yGap )  { m_lXGap = xGap ; m_lYGap = yGap ; }
	*/

	// 071018 LYW --- cFont : Modified function RenderFontShadow.
	//void RenderFontShadow(WORD wFontIdx, char * text, int size, RECT * pRect, DWORD color);
	void RenderFontShadow(WORD wFontIdx, int nShadowDistance, char* text, int size, RECT* pRect, DWORD dwColor) ;
	// 071025 LYW --- Add function to render shadowfont to alpha.
	void RenderFontShadowAlpha(WORD wFontIdx, int nDistance, char* text, int size, RECT* pRect, DWORD dwTopColor, DWORD dwShadowColor ,DWORD dwAlpha) ;
	void RenderOutLineFont(WORD wFontIdx, char * text, int size, RECT * pRect, DWORD color) ;
	//void RenderNoticeMsg(WORD wFontIdx, char * text, int size, RECT * pRect, DWORD color, int nAlpha) ;
	void RenderNoticeMsg(WORD wFontIdx, char * text, int size, RECT * pRect, DWORD frontColor, DWORD backColor) ;

};
EXTERNGLOBALTON(cFont);
#endif // _cFONT_H_

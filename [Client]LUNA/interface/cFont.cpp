// cFont.cpp: implementation of the cFont class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cFont.h"

// 070202 LYW --- Include header file.
#include "../[CC]Header/GameResourceManager.h"

#include "interface/cWindowDef.h"
#include "./Interface/GameNotifyManager.h"											// 게임 공지 매니져 클래스 헤더를 불러온다.



extern HWND _g_hWnd;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(cFont);
cFont::cFont()
{
	for(int i=0;i<FONTMAX;i++)
	{
		m_pFont[i]=NULL;
//KES 030915
		m_hFont[i]=NULL;
	}

	ZeroMemory( m_LogFont, sizeof( m_LogFont ) );

	// 070511 LYW --- cFont : Add function to render font to shadow.
	/////////////////////////////////////////////////////////
	// RENDER FONT TO SHADOW PART.
	/////////////////////////////////////////////////////////
	/*
	m_dwFrontColor	= RGB(255, 255, 255) ;
	m_dwShadowColor = RGB(10, 10, 10) ;

	m_lXGap			= 1 ;
	m_lYGap			= 1 ;
	*/

	/*m_hdc = GetDC(_g_hWnd) ;

	SetBkMode(m_hdc, TRANSPARENT) ;*/
}

cFont::~cFont()
{
	Release();

	ReleaseDC(_g_hWnd, m_hdc) ;
}
void cFont::Release()
{
	for(int i=0;i<FONTMAX;i++)
	{
		SAFE_RELEASE(m_pFont[i]);
//KES 030915
		if( m_hFont[i] )
			DeleteObject( m_hFont[i] );
	}
}
BOOL cFont::CreateFontObject(LOGFONT * pFont, WORD idx)
{
	if(idx > FONTMAX || idx < 0) return FALSE;
	
	m_pFont[idx] = g_pExecutive->GetRenderer()->CreateFontObject(pFont, D3DX_FONT);//SS3D_FONT);//D3DX_FONT);

//KES 030915
	m_LogFont[idx] = *pFont;
	m_hFont[idx] = CreateFontIndirect( pFont );

//	Getfont
//tmp
//	m_FontDesc[idx].wWidth = 6;
	m_FontDesc[idx].wWidth = (unsigned short)pFont->lfWidth;
	
	m_FontDesc[idx].wHeight = (unsigned short)(-pFont->lfHeight);

	return TRUE;
}

LONG cFont::GetTextExtent(WORD hFIdx, char * str, LONG strSize)
{
	if(hFIdx > FONTMAX || hFIdx < 0) return 0;
	WORD wid = m_FontDesc[hFIdx].wWidth;

	if( hFIdx == 6 )
	{
		wid += 2 ;
	}

	WORD DBCNT = 0;
	WORD SBCNT = 0;
	for(int i = 0 ; i < strSize ; i++)
	{
//		if(str[i] & 0x80)
		if( IsDBCSLeadByte(str[i]) )
		{
			DBCNT++;
			i++;
		}
		else
		{
			SBCNT++;
		}
	}

	LONG TextWidth = SBCNT*wid+DBCNT*2*wid; 
	return TextWidth;
}


LONG cFont::GetTextExtentEx( WORD hFIdx, char* str, int strLen )
{
// 굴림체폰트를 사용함..

	if(hFIdx > FONTMAX || hFIdx < 0) return 0;
	WORD wid = m_FontDesc[hFIdx].wWidth;
	WORD DBCNT = 0;
	WORD SBCNT = 0;
	for(int i = 0 ; i < strLen ; i++)
	{
//		if(str[i] & 0x80)
		if( IsDBCSLeadByte(str[i]) )
		{
			DBCNT++;
			i++;
		}
		else
		{
			SBCNT++;
		}
	}

	LONG TextWidth = SBCNT*wid+DBCNT*2*wid; 
	return TextWidth;
}

// 070624 LYW --- cFont : Add function to return text extent.
// 현재 자간사이를 계산하지 않고 리턴하는 함수를 계산하는 함수로 처리.
LONG cFont::GetTextExtentWidth( WORD hFIdx, char* str, int strLen )
{
	HDC hdc = GetDC(_g_hWnd) ;

	HFONT OldFont = (HFONT)SelectObject( hdc, m_hFont[hFIdx] ) ;

	SIZE size ;
	GetTextExtentPoint32( hdc, str, strLen, &size ) ;

	SelectObject( hdc, OldFont ) ;

	ReleaseDC( _g_hWnd, hdc ) ;

	return size.cx ;
}


void cFont::RenderFont(WORD wFontIdx, char * text, int size, RECT * pRect, DWORD color)
{
	if(wFontIdx>FONTMAX||wFontIdx<0) return ;
	if(!m_pFont[wFontIdx])
		return ;

	//KES가 수정했음 040722
	if( wFontIdx == 5 )
	{
		pRect->right = pRect->left + GetTextExtentWidth( wFontIdx, text, size ) + GetTextWidth( wFontIdx ) ;
	}
	else
	{
		pRect->right = pRect->left + GetTextExtent( wFontIdx, text, size ) + GetTextWidth( wFontIdx );
	}

	// 061226 LYW --- Change this code because of it's not implement to font index2
	//pRect->bottom = pRect->top + GetTextHeight( wFontIdx ) + 1;	//+1안하면 밑줄이 안나온다.
	pRect->bottom = pRect->top + GetTextHeight( wFontIdx ) + 5;	//+1안하면 밑줄이 안나온다.

	//KES 040722 추가

	// 070202 LYW --- Render Font : Modified limited area.
	//if( pRect->left > 1280 || pRect->right < 0 || pRect->bottom < 0 || pRect->top > 1024 )
	DISPLAY_INFO	dispInfo ;

	GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;

	if( pRect->left > (LONG)(dispInfo.dwWidth) || pRect->right < 0 || pRect->bottom < 0 || pRect->top > (LONG)(dispInfo.dwHeight) )
		return;
	// 070202 LYW --- End.

	if(*text) 
	{
		DWORD dwColor = 0xff000000 | color;
		g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx],text,size,pRect,dwColor,CHAR_CODE_TYPE_ASCII,0,0);
//		g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx],text,size,pRect,dwColor,CHAR_CODE_TYPE_UNICODE,0,0);

		//m_pFont[wFontIdx]->DrawText(text, size, pRect,color, CHAR_CODE_TYPE_ASCII,0);
	}

}

void cFont::RenderOutLineFont(WORD wFontIdx, char * text, int size, RECT * pRect, DWORD color)
{
	wFontIdx = 6 ;

	if(!m_pFont[6])
		return ;

	if( wFontIdx == 5 )
	{
		pRect->right = pRect->left + GetTextExtentWidth( wFontIdx, text, size ) + GetTextWidth( wFontIdx ) ;
	}
	else
	{
		pRect->right = pRect->left + GetTextExtent( wFontIdx, text, size ) + GetTextWidth( wFontIdx );
	}

	// 061226 LYW --- Change this code because of it's not implement to font index2
	pRect->bottom = pRect->top + GetTextHeight( wFontIdx ) + 5;	//+1안하면 밑줄이 안나온다.
	// 070202 LYW --- Render Font : Modified limited area.
	DISPLAY_INFO	dispInfo ;

	GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;

	if( pRect->left > (LONG)(dispInfo.dwWidth) || pRect->right < 0 || pRect->bottom < 0 || pRect->top > (LONG)(dispInfo.dwHeight) )
		return;
	// 070202 LYW --- End.

	if(*text) 
	{
		DWORD dwColor2 = color; //color | color ;

		if( color == 0xff643200 )
		{
			color = RGBA_MAKE(255, 255, 255, 255)  ;
		}

		DWORD dwColor = RGBA_MAKE(10, 10, 10, 255) ;

		pRect->left -= 1 ;
		g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx],text,size,pRect,dwColor,CHAR_CODE_TYPE_ASCII,0,0);

		pRect->left += 2 ;
		g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx],text,size,pRect,dwColor,CHAR_CODE_TYPE_ASCII,0,0);

		pRect->left -= 1 ;

		pRect->top -= 1 ;
		g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx],text,size,pRect,dwColor,CHAR_CODE_TYPE_ASCII,0,0);

		pRect->top += 2 ;
		g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx],text,size,pRect,dwColor,CHAR_CODE_TYPE_ASCII,0,0);

		pRect->top -= 1 ;
		g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx],text,size,pRect,dwColor2,CHAR_CODE_TYPE_ASCII,0,0);
	}
}

// 070511 LYW --- cFont : Add function to render font to shadow.
//void cFont::RenderFontShadow(WORD wFontIdx, char * text, int size, RECT * pRect, DWORD color)
// 071018 LYW --- cFont : Modified function RenderFontShadow.
void cFont::RenderFontShadow(WORD wFontIdx, int nShadowDistance, char* text, int size, RECT* pRect, DWORD dwColor)
{
	if(wFontIdx>FONTMAX||wFontIdx<0) return ;
	if(!m_pFont[wFontIdx])
		return ;

	if( wFontIdx == 5 )
	{
		pRect->right = pRect->left + GetTextExtentWidth( wFontIdx, text, size ) + GetTextWidth( wFontIdx ) ;
	}
	else
	{
		pRect->right = pRect->left + GetTextExtent( wFontIdx, text, size ) + GetTextWidth( wFontIdx );
	}

	pRect->bottom = pRect->top + GetTextHeight( wFontIdx ) + 5;	

	DISPLAY_INFO	dispInfo ;

	GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;

	if( pRect->left > (LONG)(dispInfo.dwWidth) || pRect->right < 0 || pRect->bottom < 0 || pRect->top > (LONG)(dispInfo.dwHeight) )
		return;

	if(*text) 
	{
		// render shadow text.
		DWORD dwShadowColor = 0xff000000 | RGB(10, 10, 10) ;
		RECT shadowRect = { pRect->left + nShadowDistance, pRect->top + nShadowDistance, pRect->right, pRect->bottom } ;
		g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx],text,size,&shadowRect,dwShadowColor,CHAR_CODE_TYPE_ASCII,0,0);

		// render front text.
		DWORD dwFrontColor = 0xff000000 | dwColor ;
		g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx],text,size,pRect,dwFrontColor,CHAR_CODE_TYPE_ASCII,0,0);
	}
}

// 071025 LYW --- Add function to render shadowfont to alpha.
void cFont::RenderFontShadowAlpha(WORD wFontIdx, int nDistance, char* text, int size, RECT* pRect, DWORD dwTopColor, DWORD dwShadowColor ,DWORD dwAlpha)
{
	if(wFontIdx>FONTMAX||wFontIdx<0) return ;
	if(!m_pFont[wFontIdx])
		return ;

	if( wFontIdx == 5 )
	{
		pRect->right = pRect->left + GetTextExtentWidth( wFontIdx, text, size ) + GetTextWidth( wFontIdx ) ;
	}
	else
	{
		pRect->right = pRect->left + GetTextExtent( wFontIdx, text, size ) + GetTextWidth( wFontIdx );
	}

	pRect->bottom = pRect->top + GetTextHeight( wFontIdx ) + 5;	

	DISPLAY_INFO	dispInfo ;

	GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;

	if( pRect->left > (LONG)(dispInfo.dwWidth) || pRect->right < 0 || pRect->bottom < 0 || pRect->top > (LONG)(dispInfo.dwHeight) )
		return;

	if(*text) 
	{
		// render shadow text.
		DWORD dwShadowColor = dwAlpha | dwShadowColor ;
		RECT shadowRect = { pRect->left + nDistance, pRect->top + nDistance, pRect->right, pRect->bottom } ;
		g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx],text,size,&shadowRect,dwShadowColor,CHAR_CODE_TYPE_ASCII,0,0);

		// render front text.
		DWORD dwFrontColor = dwAlpha | dwTopColor ;
		g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx],text,size,pRect,dwFrontColor,CHAR_CODE_TYPE_ASCII,0,0);
	}
}

//임시
void cFont::RenderFontAlpha(WORD wFontIdx, char * text, int size, RECT * pRect, DWORD color)
{
	if(wFontIdx>FONTMAX||wFontIdx<0) return ;
	if(!m_pFont[wFontIdx])
		return ;

	//KES가 수정했음 040722
	pRect->right = pRect->left + GetTextExtent( wFontIdx, text, size ) + GetTextWidth( wFontIdx ) / 4;
	pRect->bottom = pRect->top + GetTextHeight( wFontIdx ) + 1;	//+1안하면 밑줄이 안나온다.

	//KES 040722 추가
	// 070202 LYW --- Render Font : Modified limited area.
	//if( pRect->left > 1280 || pRect->right < 0 || pRect->bottom < 0 || pRect->top > 1024 )
	//	return;
	DISPLAY_INFO	dispInfo ;
	GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;
	if( pRect->left > (LONG)(dispInfo.dwWidth) || pRect->right < 0 || pRect->bottom < 0 || pRect->top > (LONG)(dispInfo.dwHeight) )
		return;
	// 070202 LYW --- End.

	if(*text)
	{
		DWORD dwColor = color;
		g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx],text,size,pRect,dwColor,CHAR_CODE_TYPE_ASCII,0,0);
//		g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx],text,size,pRect,dwColor,CHAR_CODE_TYPE_UNICODE,0,0);

		//m_pFont[wFontIdx]->DrawText(text, size, pRect,color, CHAR_CODE_TYPE_ASCII,0);
	}

}

//void cFont::RenderNoticeMsg(WORD wFontIdx, char * text, int size, RECT * pRect, DWORD color, int nAlpha)
void cFont::RenderNoticeMsg(WORD wFontIdx, char * text, int size, RECT * pRect, DWORD frontColor, DWORD backColor)
{
	wFontIdx = 6 ;

	if(!m_pFont[6])
		return ;

	if( wFontIdx == 5 )
	{
		pRect->right = pRect->left + GetTextExtentWidth( wFontIdx, text, size ) + GetTextWidth( wFontIdx ) ;
	}
	else
	{
		pRect->right = pRect->left + GetTextExtent( wFontIdx, text, size ) + GetTextWidth( wFontIdx );
	}

	// 061226 LYW --- Change this code because of it's not implement to font index2
	pRect->bottom = pRect->top + GetTextHeight( wFontIdx ) + 5;	//+1안하면 밑줄이 안나온다.
	// 070202 LYW --- Render Font : Modified limited area.
	DISPLAY_INFO	dispInfo ;

	GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;

	if( pRect->left > (LONG)(dispInfo.dwWidth) || pRect->right < 0 || pRect->bottom < 0 || pRect->top > (LONG)(dispInfo.dwHeight) )
		return;
	// 070202 LYW --- End.

	if(*text) 
	{
		//DWORD dwColor2 = color; //color | color ;

		//if( color == 0xff643200 )
		//{
		//	color = RGBA_MAKE(255, 255, 255, nAlpha)  ;
		//}

		//DWORD dwColor = RGBA_MAKE(10, 10, 10, nAlpha) ;

		pRect->left -= 1 ;
		g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx],text,size,pRect,backColor,CHAR_CODE_TYPE_ASCII,0,0);

		pRect->left += 2 ;
		g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx],text,size,pRect,backColor,CHAR_CODE_TYPE_ASCII,0,0);

		pRect->left -= 1 ;

		pRect->top -= 1 ;
		g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx],text,size,pRect,backColor,CHAR_CODE_TYPE_ASCII,0,0);

		pRect->top += 2 ;
		g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx],text,size,pRect,backColor,CHAR_CODE_TYPE_ASCII,0,0);

		pRect->top -= 1 ;
		g_pExecutive->GetGeometry()->RenderFont(m_pFont[wFontIdx],text,size,pRect,frontColor,CHAR_CODE_TYPE_ASCII,0,0);
	}
}

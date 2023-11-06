// cButton.cpp: implementation of the cButton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cButton.h"
#include "..\Audio\MHAudioManager.h"
#include "../Input/Mouse.h"
#include "cFont.h"

// 071004 LYW --- cButton : Include object manager.
#include "ObjectManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cButton::cButton()
{
	*m_szText=0;
	m_fgBasicColor=RGB_HALF(255,255,255);
	m_fgOverColor=RGB_HALF(255,255,255);
	m_fgPressColor=RGB_HALF(255,255,255);
	m_fgCurColor=RGB_HALF(255,255,255);
	m_nCurTextLen = 0;
	m_type = WT_BUTTON;
	m_textXY.x = 5;
	m_textXY.y = 4;
	
	m_bClickInside	= FALSE;
	m_BtnTextAniX	= 1;
	m_BtnTextAniY	= 1;
	
	m_bSound = FALSE;
	m_bOSound = FALSE ;

	m_shadowTextXY.x = 1;
	m_shadowTextXY.y = 0;
	m_shadowColor = RGB_HALF(10,10,10);
	m_bShadow = FALSE;

	// 0701004 LYW --- cButton : Setting sound index of button.
	m_sndIdx  = CLICK_SOUND_IDX ;
	m_osndIdx = OVER_SOUND_IDX ;

	m_bPlayOverSound = FALSE ;
}

cButton::~cButton()
{
	// 리소스 릴리즈는 리소스 메니저에서
	//SAFE_DELETE(m_pPressImage);
	//SAFE_DELETE(m_pOverImage);
}

void cButton::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, cImage * overImage, cImage * pressImage, cbFUNC Func, LONG ID)
{
	cWindow::Init(x,y,wid,hei,basicImage,ID);
	m_type = WT_BUTTON;

	if( overImage )		m_OverImage = *overImage;
	if( pressImage )	m_PressImage = *pressImage;

	if(Func != NULL)
		cbWindowFunc = Func;
}

DWORD cButton::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we |= cWindow::ActionEvent(mouseInfo);
	
	if( m_bDisable )
	{
		m_bClickInside	= FALSE;
		m_pCurImage		= &m_BasicImage;
		m_fgCurColor	= m_fgBasicColor;
		return we;
	}
	

	LONG x			= mouseInfo->GetMouseX();
	LONG y			= mouseInfo->GetMouseY();

	BOOL LeftUp		= mouseInfo->LButtonUp();
	BOOL LeftPress	= mouseInfo->LButtonPressed();

	BOOL RightUp		= mouseInfo->RButtonUp();
	BOOL RightPress	= mouseInfo->RButtonPressed();

	if( we & ( WE_LBTNCLICK | WE_LBTNDBLCLICK | WE_RBTNCLICK | WE_RBTNDBLCLICK) )
	{
		m_bClickInside	= TRUE;
	}

	// 071004 LYW --- cButton : Check mouse over.
	BOOL bMouseOver = FALSE ;
	bMouseOver = PtInWindow( x, y ) ;

	if( bMouseOver )
	{
		if( !m_bPlayOverSound )
		{
			AUDIOMGR->Play( m_osndIdx, HERO);

			m_bPlayOverSound = TRUE ;
		}
	}
	else
	{
		if( m_bPlayOverSound )
		{
			m_bPlayOverSound = FALSE ;
		}
	}

	if( LeftUp | RightUp )
	{
		if( m_bClickInside && bMouseOver )
		{
			AUDIOMGR->Play( m_sndIdx, HERO);

			if( LeftUp )
			{
				(*cbWindowFunc)(m_ID, m_pParent, WE_BTNCLICK );
				we |= WE_BTNCLICK;
			}
			else
				(*cbWindowFunc)(m_ID, m_pParent, WE_RBTNCLICK );
		}

		m_bClickInside = FALSE;
	}

//	if( PtInWindow(x, y) )
	if( we & WE_MOUSEOVER )
	{
		if( LeftPress && m_bClickInside )
		{
			m_pCurImage		= &m_PressImage;
			m_fgCurColor	= m_fgPressColor;
		}
		else
		{
			m_pCurImage		= &m_OverImage;
			m_fgCurColor	= m_fgOverColor;
		}
	}
	else
	{
		if( LeftPress && m_bClickInside ) //안에서 클릭하여 밖으로 나간경우
		{
			m_pCurImage		= &m_OverImage;
			m_fgCurColor	= m_fgOverColor;
		}
		else
		{
			m_pCurImage		= &m_BasicImage;
			m_fgCurColor	= m_fgBasicColor;
		}
	}

	return we;
}

//DWORD cButton::ActionEvent(CMouse * mouseInfo)
//{
//	DWORD we = WE_NULL;
//	if( !m_bActive ) return we;
//	we |= cWindow::ActionEvent(mouseInfo);
//	
//	if( m_bDisable )
//	{
//		m_bClickInside	= FALSE;
//		m_pCurImage		= &m_BasicImage;
//		m_fgCurColor	= m_fgBasicColor;
//		return we;
//	}
//	
//
//	LONG x			= mouseInfo->GetMouseX();
//	LONG y			= mouseInfo->GetMouseY();
//
//	BOOL LeftUp		= mouseInfo->LButtonUp();
//	BOOL LeftPress	= mouseInfo->LButtonPressed();
//
//	BOOL RightUp		= mouseInfo->RButtonUp();
//	BOOL RightPress	= mouseInfo->RButtonPressed();
//
//	if( we & ( WE_LBTNCLICK | WE_LBTNDBLCLICK | WE_RBTNCLICK | WE_RBTNDBLCLICK) )
//	{
//		m_bClickInside	= TRUE;
//	}
//
//	if( LeftUp | RightUp )
//	{
//		if( m_bClickInside && PtInWindow( mouseInfo->GetMouseEventX(), mouseInfo->GetMouseEventY() ) )
//		{
//			/*if( m_bSound )
//			{
//				AUDIOMGR->Play( m_sndIdx, HERO);
//			}*/
//
//			if( LeftUp )
//			{
//				(*cbWindowFunc)(m_ID, m_pParent, WE_BTNCLICK );
//				we |= WE_BTNCLICK;
//			}
//			else
//				(*cbWindowFunc)(m_ID, m_pParent, WE_RBTNCLICK );
//		}
//
//		m_bClickInside = FALSE;
//	}
//
////	if( PtInWindow(x, y) )
//	if( we & WE_MOUSEOVER )
//	{
//		if( LeftPress && m_bClickInside )
//		{
//			m_pCurImage		= &m_PressImage;
//			m_fgCurColor	= m_fgPressColor;
//		}
//		else
//		{
//			m_pCurImage		= &m_OverImage;
//			m_fgCurColor	= m_fgOverColor;
//		}
//	}
//	else
//	{
//		if( LeftPress && m_bClickInside ) //안에서 클릭하여 밖으로 나간경우
//		{
//			m_pCurImage		= &m_OverImage;
//			m_fgCurColor	= m_fgOverColor;
//		}
//		else
//		{
//			m_pCurImage		= &m_BasicImage;
//			m_fgCurColor	= m_fgBasicColor;
//		}
//	}
//
//	return we;
//}

void cButton::Render()
{
	if( m_bActive == FALSE ) return;
	
	cWindow::Render();

	if( m_nCurTextLen != 0 )
	{
		// 070209 LYW --- cButton : Modified render part.
		if( IsMovedWnd() )
		{
			SetRenderArea() ;
			SetMovedWnd( FALSE ) ;
		}

		if( m_bShadow )
		{
			CFONT_OBJ->RenderFont(m_wFontIdx, m_szText, m_nCurTextLen,&m_renderShadowRect,RGBA_MERGE(m_shadowColor, m_alpha * m_dwOptionAlpha / 100));
		}

		CFONT_OBJ->RenderFont(m_wFontIdx, m_szText,m_nCurTextLen,&m_renderRect,RGBA_MERGE(m_fgCurColor, m_alpha * m_dwOptionAlpha / 100));

	}
}


// 070209 LYW --- cButton : Add function to setting area of render part.
void cButton::SetRenderArea()
{
	LONG TxtPosX;

	if( m_nAlign & TXT_LEFT )
	{
		TxtPosX = (LONG)m_absPos.x + m_textXY.x;
	}
	else if( m_nAlign & TXT_RIGHT )
	{
		TxtPosX = (LONG)m_absPos.x + m_width - m_textXY.x 
				    - CFONT_OBJ->GetTextExtentEx( m_wFontIdx, m_szText, strlen(m_szText) );
	}
	else //TXT_CENTER
	{
		TxtPosX = (LONG)m_absPos.x + ( m_width
				    - CFONT_OBJ->GetTextExtentEx( m_wFontIdx, m_szText, strlen(m_szText) ) ) / 2;
	}

	if( m_pCurImage == (&m_PressImage) )
	{
		OffsetRect( &m_renderRect, m_BtnTextAniX, m_BtnTextAniY );
	}

	m_renderRect.left	= TxtPosX ;
	m_renderRect.top	= (LONG)m_absPos.y + m_textXY.y ;
	m_renderRect.right	= 1 ;
	m_renderRect.bottom	= 1 ;

	
	if( m_bShadow )
	{
		m_renderShadowRect = m_renderRect;
		OffsetRect( &m_renderShadowRect, m_shadowTextXY.x, m_shadowTextXY.y );
	}
}

void cButton::SetText(char * text, DWORD basicColor, DWORD overColor, DWORD pressColor)
{
	ASSERT(MAX_TEXT_SIZE>strlen(text));
	m_nCurTextLen = strlen(text);
	strncpy(m_szText, text, m_nCurTextLen + 1);
	m_fgCurColor = m_fgBasicColor = m_fgOverColor = m_fgPressColor = basicColor;
	if(overColor)
		m_fgOverColor = overColor;
	if(pressColor)
		m_fgPressColor = pressColor;
}

void cButton::SetTextValue(DWORD value)
{
	wsprintf(m_szText, "%d", value);
	m_nCurTextLen = strlen(m_szText);
}


void cButton::Add( cWindow* )
{
	ASSERT( 0 && "It never contain any child control" );
}

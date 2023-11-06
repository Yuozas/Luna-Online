#include "stdafx.h"
#include "StreetStallTitleTip.h"
#include "./interface/cFont.h"
#include "BalloonOutline.h"

#define BALLOON_BORDER		10

CStreetStallTitleTip::CStreetStallTitleTip()
{
	m_fgColor	= RGB_HALF(0,200,0);
	m_TitleLen	= 0;
	m_lTall		= 0;
	m_wFontIdx	= 0;
	m_bActive	= TRUE;
	m_bShow		= FALSE;

	m_nLineNum	= 0;
	m_nWidth	= 0;
}

CStreetStallTitleTip::~CStreetStallTitleTip()
{

}
/*
void CStreetStallTitleTip::SetTitle(char * szTitle)
{
	if( szTitle == NULL )	return;
	if( *szTitle == 0 )		return;

//	m_TitleLen = strlen( szTitle );
//	ASSERTMSG(m_TitleLen < 39, "object name is very long");
//	strncpy( m_sztitle, szTitle, m_TitleLen+1 );
	SafeStrCpy( m_sztitle, szTitle, MAX_STREETSTALL_TITLELEN );
	m_TitleLen = strlen( m_sztitle );
	int strPixelLen = CFONT_OBJ->GetTextExtentEx(m_wFontIdx, m_sztitle, m_TitleLen);
	SetPosX(-strPixelLen/2);

	cBalloonOutline::SetBalloonSize(strPixelLen+8, 30);
}
*/
void CStreetStallTitleTip::SetTitle(char * szTitle)
{
	if( szTitle == NULL )	return;
	if( *szTitle == 0 )		return;

	int nLen = strlen( szTitle );
	if( nLen <= 0 || nLen > MAX_STREETSTALL_TITLELEN ) return;

	SafeStrCpy( m_sztitle, szTitle, MAX_STREETSTALL_TITLELEN+1 );

//	char buf[128];
	int nCpyNum = 0;

	m_nLineNum = 0;

	while( nLen > SS_TITLE_LENGTH )
	{
		if( *szTitle == 0 ) break;
		if( ( szTitle + SS_TITLE_LENGTH ) != CharNext( CharPrev( szTitle, szTitle + SS_TITLE_LENGTH ) ) )
			nCpyNum = SS_TITLE_LENGTH - 1;
		else
			nCpyNum = SS_TITLE_LENGTH;

		strncpy( m_szChatMsg[m_nLineNum], szTitle, nCpyNum );
		m_szChatMsg[m_nLineNum][nCpyNum] = 0;

		m_ChatMsgLen[m_nLineNum] = nCpyNum;

		nLen -= nCpyNum;
		szTitle += nCpyNum;
		if( *szTitle == ' ' ) ++szTitle;	//���� ù���ڰ� �����̽��� �ƴϵ���...
		if( ++m_nLineNum >= SS_TITLE_LINE-1 ) break;
	}

	if( nLen > 0 && m_nLineNum < SS_TITLE_LINE )
	{
		strncpy( m_szChatMsg[m_nLineNum], szTitle, SS_TITLE_LENGTH );
		m_szChatMsg[m_nLineNum][SS_TITLE_LENGTH] = 0;
		if( nLen > SS_TITLE_LENGTH )
			m_ChatMsgLen[m_nLineNum] = nCpyNum;
		else
			m_ChatMsgLen[m_nLineNum] = nLen;
		++m_nLineNum;
	}

	int strPixelLen = CFONT_OBJ->GetTextExtentEx(m_wFontIdx, m_szChatMsg[0], m_ChatMsgLen[0]);
	SetPosX(-strPixelLen/2);

	m_nWidth = strPixelLen + 8; //KES 070805   --8??

//	cBalloonOutline::SetBalloonSize(strPixelLen+8, 30, m_nLineNum);
}
/*
BOOL CStreetStallTitleTip::Render(LONG absX, LONG absY)
{
	if(!m_bActive)	return FALSE;
	if(!m_bShow)	return FALSE;
	
	cBalloonOutline::RenderOutline( float(absX+m_lPosX), float(absY+m_lTall-17), float(absX), 17);

	// �ؽ�Ʈ
	RECT rect = {(long)(absX + m_lPosX+BALLOON_BORDER), (long)(absY + m_lTall+BALLOON_BORDER-17), 1, 1};
	if( *m_sztitle != 0 )
	{
		CFONT_OBJ->RenderFont(m_wFontIdx, m_sztitle, m_TitleLen, &rect, RGBA_MERGE( RGB_HALF(70,70,70), 255 ));
	}

	return TRUE;
}
*/

BOOL CStreetStallTitleTip::Render(LONG absX, LONG absY, int type)
{
	if(!m_bActive) return FALSE;

/*	
	if( m_dwStartTime + m_dwDisappearTime > gCurTime )
	{
		m_bShow = TRUE;
	}
	else
	{
		m_dwStartTime = m_dwDisappearTime = 0;
		InitChatMsg();
		m_bShow = FALSE;
	}
*/
	if(!m_bShow) return FALSE;

	if( m_nLineNum <= 0 || m_nLineNum > 2 ) return FALSE;

	LONG lLineHeight = 15;
	LONG lMiddleTotalHeight = 2 + lLineHeight * m_nLineNum;
	LONG lPosX = absX + m_lPosX;
	LONG lPosY = absY + m_lTall - lMiddleTotalHeight - 10;	//2 = linespace , 10 = tail height

	// RaMa - 04.11.29  ->ä�ð� �������� �� �Լ��� ����Ѵ�.
	//cBalloonOutline::RenderOutlineSimple( (float)lPosX, (float)lPosY, (float)absX, lMiddleTotalHeight );
	if( type == 0 )
		BALLOONOUTLINE->RenderPlayerSellTitleBalloon( lPosX, lPosY, 0, m_nWidth, m_nLineNum, m_bgColor );
	else
		BALLOONOUTLINE->RenderPlayerBuyTitleBalloon( lPosX, lPosY, 0, m_nWidth, m_nLineNum, m_bgColor );

	int nExtraHeight = 0 ;
	int	nExtraWidth = 0 ;

	switch( m_nLineNum )
	{
	case 1 :
		{
			nExtraHeight = 4 ;	
			nExtraWidth = 35 ;
		}
		break ;
	case 2 :	
	case 3 :	
		{
			nExtraHeight = 6 ;
			nExtraWidth = 43 ;
		}
		break ;
	}

	// �ؽ�Ʈ
	RECT rect;
	for( int i = 0 ; i < m_nLineNum ; ++i )
	{
		if( m_szChatMsg[i][0] != 0 )
		{
			SetRect( &rect, (long)(lPosX + BALLOON_BORDER + nExtraWidth),
							(long)(lPosY + BALLOON_BORDER + lLineHeight * i + nExtraHeight), 1, 1 );
			//CFONT_OBJ->RenderFont(m_wFontIdx, m_szChatMsg[i], m_ChatMsgLen[i], &rect, RGBA_MERGE( /*m_fgColor,*/RGB_HALF(70,70,70), 255 ) );
			CFONT_OBJ->RenderFont(m_wFontIdx, m_szChatMsg[i], m_ChatMsgLen[i], &rect, RGBA_MERGE( m_fgColor, 255 ) );
		}	
	}

//	RECT rect = {(long)(absX + m_lPosX + BALLOON_BORDER), (long)(absY + m_lTall + BALLOON_BORDER), 1, 1};
//	if( *m_szChatMsg != 0 )
//	{
//		CFONT_OBJ->RenderFont(m_wFontIdx, m_szChatMsg, m_ChatMsgLen, &rect, RGBA_MERGE( RGB_HALF(70,70,70), 255 ));
//		//CFONT_OBJ->RenderFont(m_wFontIdx, m_szChatMsg, m_ChatMsgLen, &rect, RGBA_MERGE( m_fgColor, 255 ));
//	}

	return TRUE;
}

// 080310 NYJ --- �����˻� �׽�Ʈ�� ���� �߰� (���� ������ ����)
/*
bool CStreetStallTitleTip::SearchTitle(char* szSearch)
{
	int i;
	for(i=0; i<m_nLineNum; i++)
	{
		if(strstr(m_szChatMsg[i], szSearch))
			return true;
	}

	return false;
}
*/

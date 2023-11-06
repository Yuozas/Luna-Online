#include "stdafx.h"
#include "OBalloonName.h"
#include "./interface/cFont.h"
#include "./Interface/cWindowManager.h"
#include "./interface/cMultiLineText.h"
#include "./interface/cScriptManager.h"


COBalloonName::COBalloonName()
{
	m_wFontIdx	= 0;
	
	m_lTall		= 0;
	m_lPosX		= 0;

	m_lMasterTall	= 0;
	m_lMasterPosX	= 0;

	m_lNickTall = 0;
	mFamilyNickPosX	= 0;
	
	m_fgColor	= RGB_HALF(255,255,255);

	m_bActive	= FALSE;
	m_bShow		= FALSE;

	ZeroMemory( mObjectName, sizeof(mObjectName) );
	ZeroMemory( mGuildNickName, sizeof(mGuildNickName) );
	ZeroMemory( mFamilyNickName, sizeof(mFamilyNickName) );
	
	m_pSiegeName = new cMultiLineText;
	m_bSiegeMap = FALSE;
}

COBalloonName::~COBalloonName()
{
	delete m_pSiegeName;
}

void COBalloonName::InitName( const char * name )
{
	if( !name ) return;
	if( *name == 0 ) return;
//	ASSERTMSG(strlen(name) < 17, "object name is very long");
	SafeStrCpy( mObjectName, name, sizeof( mObjectName ) );
	
	SetPosX(-CFONT_OBJ->GetTextExtentEx(m_wFontIdx, mObjectName, strlen(mObjectName))/2);
}

void COBalloonName::SetName( const char* name)
{
	if( name && *name )
	{
		SafeStrCpy( mObjectName, name, sizeof( mObjectName ) );

		SetPosX(-CFONT_OBJ->GetTextExtentEx(m_wFontIdx, mObjectName, strlen(mObjectName))/2);
	}	
}


void COBalloonName::SetNickName( const char* nickname)
{
	if( nickname )
	{
		SafeStrCpy( mGuildNickName, nickname, sizeof( mGuildNickName ) );

		const LONG nameSize = CFONT_OBJ->GetTextExtentEx( m_wFontIdx, mObjectName, strlen(mObjectName) );
		const LONG nickSize = CFONT_OBJ->GetTextExtentEx( m_wFontIdx, mGuildNickName, strlen(mGuildNickName) ) ;

		mNickData.mX		= m_lPosX + ( nameSize - nickSize ) / 2;
		mNickData.mLength	= nickSize;
	}
}


void COBalloonName::SetFamilyNickName( const char* nickname)
{
	if( nickname ) 
	{
		SafeStrCpy( mFamilyNickName, nickname, sizeof( mFamilyNickName ) );

		const LONG nameSize = CFONT_OBJ->GetTextExtentEx( m_wFontIdx, mObjectName, strlen(mObjectName) );
		const LONG nickSize = CFONT_OBJ->GetTextExtentEx( m_wFontIdx, mFamilyNickName, strlen(mFamilyNickName) ) ;

		mFamilyNickPosX = m_lPosX + ( nameSize - nickSize ) / 2;
	}
}

BOOL COBalloonName::Render(LONG absX, LONG absY)
{
#ifdef _GMTOOL_
	if( WINDOWMGR->IsInterfaceHided() )	return FALSE;
#endif

	if(	! m_bActive ||
		! m_bShow)
	{
		return FALSE;
	}
	
	RECT rect = {(long)(absX + m_lPosX), (long)(absY + m_lTall), 1, 1};

	if( m_bSiegeMap )
	{
		m_pSiegeName->SetXY( rect.left,  rect.top );
		m_pSiegeName->Render();
		CFONT_OBJ->RenderFont(m_wFontIdx, mObjectName, strlen(mObjectName), &rect, RGBA_MERGE( RGB_HALF(70,70,70), 180 ));
		rect.left -= 1;
		rect.top -= 1;
		CFONT_OBJ->RenderFont(m_wFontIdx, mObjectName, strlen(mObjectName), &rect, RGBA_MERGE( m_fgColor, 255 ));
	}
	else if( *mObjectName != 0 )
	{
		CFONT_OBJ->RenderFont(m_wFontIdx, mObjectName, strlen(mObjectName), &rect, RGBA_MERGE( RGB_HALF(70,70,70), 180 ));
		rect.left -= 1;
		rect.top -= 1;
		CFONT_OBJ->RenderFont(m_wFontIdx, mObjectName, strlen(mObjectName), &rect, RGBA_MERGE( m_fgColor, 255 ));
	}

	int line = 0;

	// 패밀리가 없는 경우에는 이름이 ""으로, 처음 생성된 경우에는 " " 상태로 세팅된다. 둘다 호칭이 없는 것이며, 따라서 두 상태를 체크해줘야 한다.
	if(	*mFamilyNickName	&&
		strcmp( " ", mFamilyNickName ) )
	{
		RECT nickrect2 = {(long)(absX + mFamilyNickPosX), (long)(absY + m_lNickTall - ++line * 15.0f), 1, 1};

		CFONT_OBJ->RenderFont(m_wFontIdx, mFamilyNickName, strlen(mFamilyNickName), &nickrect2, RGBA_MERGE( RGB_HALF(70,70,70), 180));
		nickrect2.left -= 1;
		nickrect2.top -= 1;
		CFONT_OBJ->RenderFont(m_wFontIdx, mFamilyNickName, strlen(mFamilyNickName), &nickrect2, RGBA_MERGE( RGB_HALF(234, 0, 255), 255 ));
	}

	if( *mGuildNickName != 0 )
	{
		RECT nickrect = {(long)(absX + mNickData.mX ), (long)(absY + m_lNickTall - ++line * 15.0f), 1, 1};

		CFONT_OBJ->RenderFont(m_wFontIdx, mGuildNickName, strlen(mGuildNickName), &nickrect, RGBA_MERGE( RGB_HALF(70,70,70), 180 ));
		nickrect.left -= 1;
		nickrect.top -= 1;
		CFONT_OBJ->RenderFont(m_wFontIdx, mGuildNickName, strlen(mGuildNickName), &nickrect, RGBA_MERGE( RGB_HALF(255,255,0), 180 ));
	}

	if( *mGuildNickName )	ASSERT(0);
	
	return TRUE;
}


void COBalloonName::SetPosX(LONG	pos)
{ 
	m_lPosX = pos;	
}


void COBalloonName::SetSiegeName( DWORD NameType, DWORD dwLength )
{
	cImage Image;
	DWORD Color = TTCLR_CASTLEGUILD;

	switch( NameType )
	{
	case eSiegeWarNameBox_CastleGuild:
		Color = TTCLR_CASTLEGUILD;
		break;
	case eSiegeWarNameBox_SiegeGuild:
		Color = TTCLR_SIEGEGUILD;
		break;
	case eSiegeWarNameBox_Enermy:
		Color = TTCLR_ENERMY;
		break;
	}

	char buf[MAX_NAME_LENGTH] = { 0, };
	SCRIPTMGR->GetImage(63, &Image, PFT_HARDPATH);	
	m_pSiegeName->Init( 0, RGB_HALF(255,255,255), &Image, Color );
	m_pSiegeName->SetImageAlpha( TOOLTIP_ALPHA );
	m_pSiegeName->AddNamePannel( dwLength );

	m_bSiegeMap = TRUE;
}


const COBalloonName::Data& COBalloonName::GetNickData() const
{
	return mNickData;
}


DWORD COBalloonName::GetNameLength()
{
	return CFONT_OBJ->GetTextExtentEx( m_wFontIdx, mObjectName, strlen( mObjectName ) );
}

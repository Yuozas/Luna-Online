// ChaseDialog.cpp: implementation of the CChaseDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChaseDialog.h"
#include "WindowIDEnum.h"
#include "MHMap.h"
#include "MHFile.h"
//#include "GameResourceManager.h"
#include "Interface/cStatic.h"
#include "Interface/cTextArea.h"
#include "Interface/cScriptManager.h"
#include "ChatManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const char PosMsg[3][3][10] = 
{
	{ "북서", "북쪽", "북동", },
	{ "서쪽", "중심", "동쪽", },
	{ "남서", "남쪽", "남동", },
};


CChaseDialog::CChaseDialog()
{
	m_type = WT_CHASE_DLG;

}

CChaseDialog::~CChaseDialog()
{}


void CChaseDialog::Linking()
{
	m_pMap = (cStatic*)GetWindowForID( SEE_CHASE_MAP );
	m_TextArea = (cTextArea*)GetWindowForID( SEE_CHASE_MSG );

	m_bActive = FALSE;	
	SCRIPTMGR->GetImage( 126, &m_pIconImage );
	m_MapNum = 0;
}


void CChaseDialog::SetActive( BOOL isActive )
{
	cDialog::SetActive( m_bActive = isActive );
}


BOOL CChaseDialog::InitMiniMap( MAPTYPE MapNum, WORD PosX, WORD PosY, char* strName, MAPTYPE EventMapNum )
{
	const MAPTYPE LoadMap = ( EventMapNum == 44 ) ? EventMapNum : MapNum;

	m_EventMapNum = EventMapNum;

	if( !LoadMinimapImageInfo( LoadMap ) )
	{
		return FALSE;
	}
	
	m_TargetPos.x = PosX;
	m_TargetPos.y = PosY;
	SafeStrCpy( m_WantedName, strName, MAX_NAME_LENGTH+1 );

	return TRUE;
}


BOOL CChaseDialog::LoadMinimapImageInfo(MAPTYPE MapNum)
{
	DIRECTORYMGR->SetLoadMode(eLM_Minimap);

	char	name[ 80 ];
	CMHFile file;

	sprintf(name,"Minimap%d.bin",MapNum);	
	file.Init(name,"rb",MHFILE_FLAG_DONOTCHANGE_DIRECTORY_TOROOT);

	if( ! file.IsInited() )
	{
		DIRECTORYMGR->SetLoadMode(eLM_Root);
		return FALSE;
	}
	
	for( int i = 0; i < 2; ++i )
	{
		file.GetString(name);

		if( file.IsEOF() )
		{
			break;
		}

		m_MapImage.size.cx = file.GetDword();
		m_MapImage.size.cy = file.GetDword();			

		if( i == 1 )
		{
			m_MapImage.image.LoadSprite(name);
		}
	}
	
	DIRECTORYMGR->SetLoadMode(eLM_Root);
	m_MapNum = MapNum;

	return TRUE;
}


void CChaseDialog::Render()
{
	if( !m_bActive )
	{
		return;
	}

	cDialog::RenderWindow();
	
	if(m_MapImage.image.IsNull())
	{
		return;
	}

	if( IsMovedWnd() )
	{
		m_scale.x = (float)(m_pMap->GetWidth() / m_MapImage.size.cx);
		m_scale.y = (float)(m_pMap->GetHeight() / m_MapImage.size.cy);

		m_trans.x = m_pMap->GetAbsX();
		m_trans.y = m_pMap->GetAbsY();

		m_dwX = (DWORD)(m_TargetPos.x / (51200/3));
		m_dwY = (DWORD)(m_TargetPos.y / (51200/3));

		m_trans.x = m_pMap->GetAbsX() + m_dwX*(m_pMap->GetWidth()/3);
		m_trans.y = m_pMap->GetAbsY() + (2-m_dwY)*(m_pMap->GetHeight()/3);

		m_rect.left		= 0;
		m_rect.top		= 0;
		m_rect.right	= m_MapImage.size.cx;
		m_rect.bottom	= m_MapImage.size.cy;
	}

	m_MapImage.image.SetImageSrcRect(&m_rect);

	m_MapImage.image.RenderSprite(&m_scale,NULL,0,&m_trans,RGBA_MAKE(255,255,255,255));
	m_pIconImage.RenderSprite( &m_scale, NULL, 0, &m_trans, RGBA_MAKE(255,255,255,200) );

	char text[256];

	if( m_EventMapNum == 44 )
	{
		sprintf( text, CHATMGR->GetChatMsg(923), m_WantedName, GetMapName( m_MapNum ), GetMapName( m_EventMapNum ) );
	}
	else
	{
		sprintf( text, CHATMGR->GetChatMsg(906), m_WantedName, GetMapName( m_MapNum ), m_EventMapNum, PosMsg[(2-m_dwY)][m_dwX] );
	}
	m_TextArea->SetScriptText( text );
	
	cDialog::RenderComponent();

	if( IsMovedWnd() )
	{
		SetMovedWnd( FALSE );
	}
}

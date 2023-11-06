// GuildUnionMarkMgr.cpp: implementation of the CGuildUnionMarkMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildUnionMarkMgr.h"
#include "ObjectManager.h"
#include "FreeImage/FreeImage.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CGuildUnionMark::CGuildUnionMark()
{
	m_bLoaded = FALSE;
}

CGuildUnionMark::~CGuildUnionMark()
{
}

BOOL CGuildUnionMark::LoadNewGuildUnionMark( char* pName )
{
	if( m_Img.LoadSprite( pName ) )
	{
		m_bLoaded = TRUE;
		return TRUE;
	}
	m_bLoaded = FALSE;
	return FALSE;
}

void CGuildUnionMark::UpdateGuildUnionMark( char* pName )
{
	m_Img.Release();
	if( m_Img.LoadSprite( pName ) )
		m_bLoaded = TRUE;
	else
		m_bLoaded = FALSE;
}

void CGuildUnionMark::Render( VECTOR2* pos, DWORD dwColor /* = 0xffffffff  */ )
{
	if( m_bLoaded )
	{
		VECTOR2 scale;
		scale.x = 1;
		scale.y = 1;
		
		m_Img.RenderSprite( &scale, NULL, 0, pos, dwColor );
	}
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CGuildUnionMarkMgr)
CGuildUnionMarkMgr::CGuildUnionMarkMgr()
{
	m_MarkTable.Initialize( 64 );
}

CGuildUnionMarkMgr::~CGuildUnionMarkMgr()
{
	CGuildUnionMark* pMark = NULL;
	m_MarkTable.SetPositionHead();
	while( pMark = m_MarkTable.GetData() )
		delete pMark;
	m_MarkTable.RemoveAll();
}

CGuildUnionMark* CGuildUnionMarkMgr::LoadNewGuildUnionMark( int nServerSetNum, DWORD dwGuildUnionIdx, DWORD dwMarkIdx )
{
	char filename[256];
 	sprintf( filename, "Data/Interface/2DImage/UnionMark/%02d_%d.tga", nServerSetNum, dwMarkIdx );

	CGuildUnionMark* pMark = new CGuildUnionMark;
	if( pMark->LoadNewGuildUnionMark( filename ) )
	{
		m_MarkTable.Add( pMark, dwMarkIdx );
		return pMark;
	}
	else
	{
		m_MarkTable.Add( pMark, dwMarkIdx );
		return NULL;
	}
	return NULL;
}

CGuildUnionMark* CGuildUnionMarkMgr::GetGuildUnionMark( int nServerSetNum, DWORD dwGuildUnionIdx, DWORD dwMarkIdx )
{
	if( !dwGuildUnionIdx )	return NULL;
	if( !dwMarkIdx )		return NULL;

	CGuildUnionMark* pMark = m_MarkTable.GetData( dwMarkIdx );
	if( pMark )		return pMark;
	
	pMark = LoadNewGuildUnionMark( nServerSetNum, dwGuildUnionIdx, dwMarkIdx );
	if( pMark )		return pMark;

	// send request
	MSG_DWORD2 Msg;
	Msg.Category = MP_GUILD_UNION;
	Msg.Protocol = MP_GUILD_UNION_MARK_REQUEST_SYN;
	Msg.dwObjectID = HEROID;
	Msg.dwData1 = dwGuildUnionIdx;
	Msg.dwData2 = dwMarkIdx;
	NETWORK->Send( &Msg, sizeof(Msg) );
	
	return NULL;
}

BOOL CGuildUnionMarkMgr::RegistGuildUnionMark( int nServerSetNum, DWORD dwGuildUnionIdx, DWORD dwMarkIdx, const char* pBmpName )
{
	if( !dwGuildUnionIdx )	return FALSE;

	// bmp -> char
	char filename[ MAX_PATH ];
	sprintf( filename, "%s", pBmpName );

	const char* extension = ".bmp";

	if( strlen( filename ) < strlen( extension ) )
	{
		strcat( filename, extension );
	}
	else if( strcmpi( pBmpName + strlen( pBmpName ) - strlen( extension ), extension ) )
	{
		strcat( filename, extension );
	}

	char imgData[GUILDUNIONMARK_BUFSIZE];

	FIBITMAP* pBitMap = FreeImage_Load( FIF_BMP, filename, BMP_DEFAULT );
	BITMAPINFO* pBitInfo = FreeImage_GetInfo( pBitMap );
	if( (pBitMap == 0) || (pBitInfo == 0) )	return FALSE;
	if( pBitInfo->bmiHeader.biHeight != GUILDUNIONMARKHEIGHT ||	pBitInfo->bmiHeader.biWidth != GUILDMARKWIDTH )
		return FALSE;
	FIBITMAP* pConvertedBitmap = FreeImage_ConvertTo16Bits565( pBitMap );
	int bytesPerLine = GUILDMARKWIDTH*GUILDMARKBPP;
	BYTE* pData = FreeImage_GetBits( pConvertedBitmap );
	for( int y = 0; y < GUILDUNIONMARKHEIGHT; ++y )
	{
		memcpy( &imgData[(GUILDUNIONMARKHEIGHT-y-1)*bytesPerLine], &pData[y*bytesPerLine], bytesPerLine );
	}	
	FreeImage_Unload(pBitMap);
	FreeImage_Unload(pConvertedBitmap);

	// send msg
	MSG_GUILDUNIONMARK_IMG Msg;
	Msg.Category = MP_GUILD_UNION;
	Msg.Protocol = MP_GUILD_UNION_MARK_REGIST_SYN;
	Msg.dwObjectID = HEROID;
	Msg.dwGuildUnionIdx = dwGuildUnionIdx;
	Msg.dwMarkIdx = dwMarkIdx;
	memcpy( Msg.imgData, imgData, GUILDUNIONMARK_BUFSIZE );
	NETWORK->Send( &Msg, sizeof(Msg) );

	// markidx = 0
	HERO->SetGuildUnionMarkIdx( 0 );

	// ���� ���� ��ũ�� ������ �����
	CGuildUnionMark* pMark = m_MarkTable.GetData( dwMarkIdx );
	if( pMark )
	{
		delete pMark;
		m_MarkTable.Remove( dwMarkIdx );
	}	
	
	return TRUE;
}

void CGuildUnionMarkMgr::SaveGuildUnionMark( int nServerSetNum, DWORD dwGuildUnionIdx, DWORD dwMarkIdx, char* pImgData )
{
	if( !dwGuildUnionIdx )	return;
	if( !dwMarkIdx )		return;
	
	const char* directory = "Data/Interface/2DImage/UnionMark";

	if( ERROR_PATH_NOT_FOUND == CreateDirectory( directory, 0 ) )
	{
		ASSERT( 0 );
		return;
	}

	char filename[256];
	sprintf( filename, "%s/%02d_%d.tga", directory, nServerSetNum, dwMarkIdx );
	
	char tempBuf[GUILDMARKBPP*GUILDMARKWIDTH*GUILDMARKWIDTH];
	memset( tempBuf, 0, GUILDMARKBPP*GUILDMARKWIDTH*GUILDMARKWIDTH );
	memcpy( tempBuf, pImgData, GUILDUNIONMARK_BUFSIZE );
	WriteTGA( filename, tempBuf, GUILDMARKWIDTH, GUILDMARKWIDTH, GUILDMARKBPP*GUILDMARKWIDTH, GUILDMARKBPP*8 );

	CGuildUnionMark* pMark = m_MarkTable.GetData( dwMarkIdx );
	if( pMark )
	{
		pMark->UpdateGuildUnionMark( filename );
	}
	else
	{
		pMark = new CGuildUnionMark;
		pMark->LoadNewGuildUnionMark( filename ); 
		m_MarkTable.Add( pMark, dwMarkIdx );
	}	
}

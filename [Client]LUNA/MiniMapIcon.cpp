#include "stdafx.h"
#include "MiniMapIcon.h"
#include "../[cc]header/GameResourceManager.h"
#include "Interface/cImage.h"
#include "Interface/cFont.h"
#include "Object.h"
#include "Npc.h"
// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.10.31
#include "cResourceManager.h"
#include "../hseos/Farm/SHFarm.h"
// E 농장시스템 추가 added by hseos 2007.10.31


CMiniMapIconImage::~CMiniMapIconImage()
{
	SAFE_DELETE( m_pImage );
}


CMiniMapIcon::CMiniMapIcon()
{
	m_pIconImage = NULL;
	m_Kind = 0;
	m_Interpolation = 10;
	m_dwColor = 0xffffffff;
	m_bAlwaysTooltip = FALSE;

	//ZeroMemory( m_pMarkExclamation, sizeof(m_pMarkExclamation) );
	//ZeroMemory( m_pMarkQuestion, sizeof(m_pMarkQuestion) );

	m_ShowQuestMark = eQM_NONE;
}


CMiniMapIcon::~CMiniMapIcon()
{}


void CMiniMapIcon::SetMarkImage(CMiniMapIconImage* pMarkExclamation, CMiniMapIconImage* pMarkQuestion, SIZE_T size )
{
	if( pMarkQuestion )
	{
		m_pMarkQuestion.resize( size );
	}
	
	if( pMarkExclamation )
	{
		m_pMarkExclamation.resize( size );
	}

	for( SIZE_T i = 0; i < size; ++i )
	{
		if( pMarkExclamation )
		{
			m_pMarkExclamation[ i ] = pMarkExclamation	+ i;
		}

		if( pMarkQuestion )
		{
			m_pMarkQuestion[ i ]	= pMarkQuestion		+ i;
		}
	}

	/*const SIZE_T size = sizeof( m_pMarkExclamation ) / sizeof( CMiniMapIconImage* );

	for( int i = 0 ; i < size ; ++i )
	{
		if( pMarkExclamation )
		{
			m_pMarkExclamation[ i ] = pMarkExclamation + i;
		}

		if( pMarkQuestion )
		{
			m_pMarkQuestion[ i ]	= pMarkQuestion + i;
		}
	}*/
}

void CMiniMapIcon::Init(CMiniMapIconImage* pIconImage, int Kind )
{
	m_pIconImage = pIconImage;
	m_Kind = Kind;
}

void CMiniMapIcon::Init(CMiniMapIconImage* pIconImage,CObject* pObject, int Kind)
{
	m_pObject = pObject;
	m_pIconImage = pIconImage;
	m_Kind = Kind;
}

void CMiniMapIcon::Init(CMiniMapIconImage* pIconImage,VECTOR3* pObjectPos, int Kind)
{
	m_pObject = NULL;
	m_ObjectPos = *pObjectPos;
	m_pIconImage = pIconImage;
	m_Kind = Kind;
}

void CMiniMapIcon::Init(CMiniMapIconImage* pIconImage,PARTY_MEMBER* pMember, int Kind)
{
	m_pObject = NULL;
	strcpy(m_MemberName, pMember->Name);
	m_pIconImage = pIconImage;
	m_Kind = Kind;
}

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.10.31
void CMiniMapIcon::Init(CMiniMapIconImage* pIconImage, CSHFarm* pFarm, int Kind)
{
	m_pObject = NULL;
	strcpy(m_MemberName, RESRCMGR->GetMsg(799));
	m_ObjectPos = *pFarm->GetPos();
	m_pIconImage = pIconImage;
	m_Kind = Kind;
}
// E 농장시스템 추가 added by hseos 2007.10.31


VECTOR3 CMiniMapIcon::GetPosition()
{
	if(m_pObject)
	{
		m_pObject->GetPosition(&m_ObjectPos);
	}
	return m_ObjectPos;
}

void CMiniMapIcon::Update(DWORD MinimapImageWidth,DWORD MinimapImageHeight)
{
	if(m_Kind == eMNMICON_PARTYMEMBER)
	{
		m_ObjectPos.x += (m_TargetPos.x - m_ObjectPos.x)/m_Interpolation;
		m_ObjectPos.z += (m_TargetPos.z - m_ObjectPos.z)/m_Interpolation;		
	}
	else if(m_pObject)
	{
		if(m_pObject->IsInited() == FALSE)
			return;

		m_pObject->GetPosition(&m_ObjectPos);
	}

	m_ScreenPos.x = (m_ObjectPos.x / 51200)*MinimapImageWidth;
	m_ScreenPos.y = MinimapImageHeight - (m_ObjectPos.z / 51200)*MinimapImageHeight;
}

void CMiniMapIcon::Render(VECTOR2* pMapStartPixel,RECT* pCullRect)
{
	if( pCullRect )
	{
//		if(pCullRect->left > m_ScreenPos.x || m_ScreenPos.x > pCullRect->right)
//			return;
//		if(pCullRect->top > m_ScreenPos.y || m_ScreenPos.y > pCullRect->bottom)
//			return;

		VECTOR3 v1;
		VECTOR3 v2;

		v1.x = ( float )( ( pCullRect->left + pCullRect->right ) / 2 + 3);
		v1.z = ( float )( ( pCullRect->top + pCullRect->bottom ) / 2 - 3);

		v2.x = m_ScreenPos.x;
		v2.z = m_ScreenPos.y;

		float range = CalcDistanceXZ( &v1, &v2 );

		float dis = ( float )( ( pCullRect->right - pCullRect->left ) / 2 );

		if( range > dis )
			return;
	}

	VECTOR2 trans;
	trans.x = m_ScreenPos.x - m_pIconImage->m_ImageWH.x*0.5f + pMapStartPixel->x;
	trans.y = m_ScreenPos.y - m_pIconImage->m_ImageWH.y*0.5f + pMapStartPixel->y;
	
	m_pIconImage->m_pImage->RenderSprite(NULL,NULL,0,&trans,m_dwColor);


/*
	if( m_bAlwaysTooltip )
	{
		char name[64];
		GetToolTip( 0, 0, NULL, name );
		name[63] = 0;
		RECT rect;
		SetRect(&rect,trans.x,trans.y,30,10);
		CFONT_OBJ->RenderFontAlpha( 0, name, strlen(name), &rect, RGBA_MAKE( 255, 255, 255, 100 ) );
	}
*/
}

void CMiniMapIcon::RenderQuestMark( VECTOR2* pMapStartPixel, RECT* pCullRect )
{
	if( m_ShowQuestMark == eQM_NONE )
	{
		return;
	}

	if( pCullRect )
	{
//		if(pCullRect->left > m_ScreenPos.x || m_ScreenPos.x > pCullRect->right)
//			return;
//		if(pCullRect->top > m_ScreenPos.y || m_ScreenPos.y > pCullRect->bottom)
//			return;

		VECTOR3 v1;
		v1.x = ( float )( ( pCullRect->left + pCullRect->right ) / 2 + 3);
		v1.z = ( float )( ( pCullRect->top + pCullRect->bottom ) / 2 - 3);

		VECTOR3 v2;
		v2.x = m_ScreenPos.x;
		v2.z = m_ScreenPos.y;

		const float range	= CalcDistanceXZ( &v1, &v2 );
		const float dis		= ( float )( ( pCullRect->right - pCullRect->left ) / 2 );

		if( range > dis )
		{
			return;
		}
	}

	static SIZE_T index;

	// 특정 시간이 될때마다 인덱스를 0~2로 순차 변경한다.
	{
		static DWORD dwTime = gCurTime;

		if( gCurTime - dwTime > 150 )
		{
			static const SIZE_T size = max( m_pMarkExclamation.size(), m_pMarkQuestion.size() );
			ASSERT( size );

			index	= ++index % size;
			dwTime	= gCurTime;
		}
	}
	
	switch( m_ShowQuestMark )
	{
	case eQM_EXCLAMATION:
		{
			if( index < m_pMarkExclamation.size() )
			{
				CMiniMapIconImage* icon = m_pMarkExclamation[ index ];

				VECTOR2 trans;
				trans.x = m_ScreenPos.x - icon->m_ImageWH.x * 0.5f + pMapStartPixel->x - 1;
				trans.y = m_ScreenPos.y - icon->m_ImageWH.y * 0.5f + pMapStartPixel->y - 1;

				icon->m_pImage->RenderSprite( NULL, NULL, 0.0f, &trans, m_dwColor );		
			}

			break;
		}
	case eQM_QUESTION:
		{
			if( index < m_pMarkQuestion.size() )
			{
				CMiniMapIconImage* icon = m_pMarkQuestion[ index ];

				VECTOR2 trans;
				trans.x = m_ScreenPos.x - icon->m_ImageWH.x * 0.5f + pMapStartPixel->x - 1;
				trans.y = m_ScreenPos.y - icon->m_ImageWH.y * 0.5f + pMapStartPixel->y - 1;

				icon->m_pImage->RenderSprite( NULL, NULL, 0.0f, &trans, m_dwColor );				
			}

			break;
		}
	}
}

int CMiniMapIcon::GetToolTip(int x,int y,VECTOR2* pMapStartPixel,char* pOutText, float* distOut )
{
	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.10.31
	/*
	if(m_pObject == FALSE && m_Kind != eMNMICON_PARTYMEMBER)
		return 0;
	*/
	if(m_pObject == FALSE && m_Kind != eMNMICON_PARTYMEMBER && m_Kind != eMNMICON_FARM)
		return 0;
	// E 농장시스템 추가 added by hseos 2007.10.31

	if( pMapStartPixel == NULL ) return 0;

	int gapx = x - int(m_ScreenPos.x+pMapStartPixel->x);
	int gapy = y - int(m_ScreenPos.y+pMapStartPixel->y);
	int halfx = int(m_pIconImage->m_ImageWH.x*0.5f);
	int halfy = int(m_pIconImage->m_ImageWH.y*0.5f);
		
	if(gapx > halfx || gapx < -halfx)
		return 0;
	if(gapy > halfy || gapy < -halfy)
		return 0;

	if( distOut )
		*distOut = sqrtf( (float)(gapx*gapx + gapy*gapy) );

	if(m_Kind == eMNMICON_PARTYMEMBER)
	{
		strcpy(pOutText, m_MemberName);
		return 2;
	}
	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.10.31
	else if (m_Kind == eMNMICON_FARM)
	{
		strcpy(pOutText, m_MemberName);
		return 3;
	}
	// E 농장시스템 추가 added by hseos 2007.10.31
	else if(m_pObject->GetObjectKind() == eObjectKind_Npc )
	{
		NPC_LIST* pInfo = GAMERESRCMNGR->GetNpcInfo(((CNpc*)m_pObject)->GetNpcKind());
		// 070613 LYW --- MiniMapIcon : Modified.
		//wsprintf( pOutText, "%s(%s)", m_pObject->GetObjectName(), pInfo->Name );
		if( pInfo->ShowJob )
		{
			wsprintf( pOutText, "%s(%s)", m_pObject->GetObjectName(), pInfo->Name );
		}
		else
		{
			wsprintf( pOutText, "%s", m_pObject->GetObjectName());
		}
	}
	else
	{
		strcpy(pOutText,m_pObject->GetObjectName());
	}

	return 1;
}




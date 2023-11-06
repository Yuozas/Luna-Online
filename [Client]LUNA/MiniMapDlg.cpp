#include "stdafx.h"
#include "MiniMapDlg.h"
#include "BigMapDlg.h"
#include "Interface/cWindowManager.h"
#include "Interface/cStatic.h"
#include "Interface/cPushupButton.h"
#include "Interface/cScriptManager.h"
#include "Object.h"
#include "Npc.h"
#include "GameResourceManager.h"
#include "WindowIDEnum.h"
#include "Input/UserInput.h"
#include "mhFile.h"

#include "GameIn.h"
//#include "MainBarDialog.h"
#include "ChatManager.h"
#include "PartyIconManager.h"
#include "EventMapInfo.h"
#include "mhMap.h"

#include "cJackpotManager.h"

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.10.04
#include "../hseos/Farm/SHFarmManager.h"
// E 농장시스템 추가 added by hseos 2007.10.04

// desc_hseos_데이트 존_01
// S 데이트 존 추가 added by hseos 2007.12.03
#include "../hseos/Date/SHDateManager.h"
// E 데이트 존 추가 added by hseos 2007.12.03
#include "cResourceManager.h"

const DWORD IconDescription[] = 
{
	// index, relative x, relative y
	// 이미지의 
	 //0,	9,	9,			//eMNMICON_SELF,
	0, 14, 18,
	13,	8,	8,			//eMNMICON_PARTYMEMBER,
	14,	8,	8,			//eMNMICON_FAMILYMEMBER,
	15,	8,	8,			//eMNMICON_NORMALNPC,
	16,	8,	8,			//eMNMICON_LOADINGPOINT,
	//17, 16, 44,			//eMNMICON_CAMERA,
	9, 20, 48,
	18, 11, 11,			//eMNMICON_QUEST1,
	19, 11, 11,			//eMNMICON_QUEST2,
	20,	11, 11,			//eMNMICON_QUEST3,
	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.10.31
	0,	0, 0,			//eMNMICON_NULL01,
	0,	0, 0,			//eMNMICON_NULL02,
	0,	0, 0,			//eMNMICON_NULL03,
	0,	0, 0,			//eMNMICON_NULL04,
	6,	8, 8,			//eMNMICON_FARM,
	// E 농장시스템 추가 added by hseos 2007.10.31
};


extern int g_MapChannelNum[MAX_MAP_NUM];


CMiniMapDlg::CMiniMapDlg()
{
	m_pMapName		= NULL;
	// 080121 LYW --- MiniMapDlg : 채널용 스태틱 추가.
	m_pChannel		= NULL;
	m_pPosXY		= NULL;
	m_pZoom			= NULL;
	m_CurMode		= eMINIMAPMODE_SMALL;
	m_dwImageRGB	= RGBA_MAKE( 255, 255, 255, 190 );

	// 070430 LYW --- MiniMapDlg : Add button for toggle minimap.
	m_pToggleBtn = NULL ;
	
	m_IconTable.Initialize(64);
	
	//m_DiplayOriginPosOfImage.x = 0;	// ?횑쨔횑횁철?횉 쩐챤쨈? 쨘횓쨘횖?횑 쨔횑쨈횕쨍횎 횄짖쩔징쩌짯?횉 0,0쩔징 횂챦횊첨쨈횂째징
	//m_DiplayOriginPosOfImage.y = 0;
	
	m_IconPool.Init(100, 10, "MinimapIconPool");

	// 미니맵 상수
	{
		/// 표시할 크기
		m_MiniMap.mViewport.x = 159;
		m_MiniMap.mViewport.y = 146;

		/// 윈도우 틀만큼 이동한 후 미니맵을 표시해야한다.
		m_MiniMap.mTranslation.x = 6;
		m_MiniMap.mTranslation.y = 46;
	}
}


CMiniMapDlg::~CMiniMapDlg()
{	
	/*for(int n=0;n<eMINIMAPMODE_MAX;++n)
	{
		m_MinimapImage[n].image.Release();
		m_MinimapImage[n].ImageHeight = 0;
		m_MinimapImage[n].ImageWidth = 0;
	}*/

	CMiniMapIcon* pIcon;
	m_IconTable.SetPositionHead();
	while(pIcon = m_IconTable.GetData())
		ReleaseMinniMapIcon(pIcon);
	m_IconTable.RemoveAll();

	ReleaseNumImage();
}
/*
void CMiniMapDlg::SetZoomRate(float zoomrate)
{
	m_ZoomRate = zoomrate;

	m_DisplayMinX = m_DiplayOriginPosOfImage.x*m_ZoomRate;
	m_DisplayMinZ = m_DiplayOriginPosOfImage.y*m_ZoomRate;
	m_DisplayMaxX = (m_DiplayOriginPosOfImage.x+m_MiniMap.mViewport.x)*m_ZoomRate;
	m_DisplayMaxZ = (m_DiplayOriginPosOfImage.y+m_MiniMap.mViewport.y)*m_ZoomRate;
}
*/

void CMiniMapDlg::InitMiniMap(MAPTYPE MapNum)
{
	const eGameInInitKind kind	= ( eGameInInitKind )GAMEIN->GetGameInInitKind();
/*
	if(	eGameInInitKind_SuryunEnter		== kind		||
		eGameInInitKind_EventMapEnter	== kind		||
		BOSSMONSTER_MAP					== MapNum	||
		BOSSMONSTER_2NDMAP				== MapNum )
	{
		SetActive( FALSE );
	}
*/
//	TempID = SKILLOBJECT_ID_START;		// 쩍쨘횇쨀?횑 쨔횑쨈횕쨍횎쩔징 쨉챤쨌횕 쨉횋쨍짰 쩐첩?쨍쨈횕짹챤.
	
	LoadMinimapImageInfo( m_MapNum = MapNum );

	if( m_pMapName )
	{
		//char text[128] = {0, } ;
// 070430 LYW --- MiniMapdlg : Modified chat msg.
		/*
#ifdef TAIWAN_LOCAL	//chat_msg order
		wsprintf( buf, CHATMGR->GetChatMsg(442),gChannelNum+1,GetMapName(MapNum));
#elif defined _HK_LOCAL_	//chat_msg order
		wsprintf( buf, CHATMGR->GetChatMsg(442),gChannelNum+1,GetMapName(MapNum));
#else
		wsprintf( buf, CHATMGR->GetChatMsg(442),GetMapName(MapNum),gChannelNum+1);
#endif
//		m_pMapName->SetStaticText(GetMapName(MapNum));
		*/
		//wsprintf( text, CHATMGR->GetChatMsg(167),GetMapName(MapNum), gChannelNum+1);
		// desc_hseos_농장시스템_01
		// S 농장시스템 추가 added by hseos 2007.10.04
		/*if (g_csFarmManager.GetFarmZoneID(MapNum, NULL))
		{
			wsprintf(text, GetMapName(MapNum));
		}*/
		// E 농장시스템 추가 added by hseos 2007.10.04

		//m_pMapName->SetStaticText(text);

		m_pMapName->SetStaticText(GetMapName(MapNum)) ;
	}

	//// 080121 LYW --- MiniMapDlg : 채널용 스태틱 추가.
	//if( m_pChannel )
	//{
	//	char tempBuf[128] = {0, } ;
	//	sprintf(tempBuf, "%d", gChannelNum+1) ;
	//	strcat(tempBuf, RESRCMGR->GetMsg(102)) ;
	//	m_pChannel->SetStaticText(tempBuf) ;
	//}

	SetChannel(gChannelNum);
}

void CMiniMapDlg::SetChannel(int nChannel)
{

	BOOL bDraw = FALSE;
	if(1 < g_MapChannelNum[m_MapNum])
		bDraw = TRUE;

	if( m_pChannel && bDraw)
	{
		char tempBuf[128] = {0, } ;
		sprintf(tempBuf, "%d", nChannel+1) ;
		strcat(tempBuf, RESRCMGR->GetMsg(102)) ;
		m_pChannel->SetStaticText(tempBuf) ;
	}
}


void CMiniMapDlg::InitIconImage()
{
	const SIZE_T size = sizeof( IconDescription ) / ( sizeof( DWORD ) * 3 );

	for( SIZE_T i = 0; i < size; ++i )
	{
		cImage* image = new cImage;

		const DWORD index	= IconDescription[ 3 * i ];
		const DWORD x		= IconDescription[ 3 * i + 1 ];
		const DWORD y		= IconDescription[ 3 * i + 2 ];
		
		SCRIPTMGR->GetImage( index, image, PFT_MINIMAPPATH );

		m_pIconImage[ i ].InitIconImage( image, x, y );
	}

//KES
	//SCRIPTMGR->GetImage( 63, &m_TooltipImage, PFT_HARDPATH );
}

void CMiniMapDlg::Linking()
{
	memset(m_pIconImage,0,sizeof(cImage*)*eMNMICON_MAX);
	InitIconImage();
	m_pMapName	= (cStatic*)GetWindowForID(MNM_MAPNAME);
	// 080121 LYW --- MiniMapDlg : 채널용 스태틱 추가.
	m_pChannel = (cStatic*)GetWindowForID(MNM_CHANNELNAME) ;
	m_pPosXY	= (cStatic*)GetWindowForID(MNM_POSXY);
	// 070430 LYW --- MiniMapDlg : Add button for toggle minimap.
	m_pToggleBtn = (cButton*)GetWindowForID(MNM_ONOFF) ;
	m_pToggleBtn->SetActive( TRUE ) ;

	m_pZoom		= (cPushupButton*)GetWindowForID( MNM_TOGGLEFULL );

	JPInit();
	InitNumImage();	
}

void CMiniMapDlg::AddMonsterIcon(CObject* pObject)
{
}
void CMiniMapDlg::AddBossMonsterIcon(CObject* pObject)
{
}
void CMiniMapDlg::AddStaticNpcIcon(CObject* pObject)
{
	CNpc* pNpc = (CNpc*)pObject;
	if(pNpc->GetNpcKind() == 30 || pNpc->GetNpcKind() == 31)
	{
		AddIcon(eMNMICON_LOADINGPOINT,pObject);
	}
	else if(pNpc->GetNpcKind() != 30 && pNpc->GetNpcKind() != 31)
	{
		AddIcon(eMNMICON_NORMALNPC,pObject);
	}

	CBigMapDlg* dialog = GAMEIN->GetBigMapDialog();

	if( dialog )
	{
		dialog->AddStaticNpcIcon( pObject );
	}
}

void CMiniMapDlg::AddPartyMemberIcon(PARTY_MEMBER* pMember)
{
	AddIcon(eMNMICON_PARTYMEMBER,pMember);

	CBigMapDlg* dialog = GAMEIN->GetBigMapDialog();

	if( dialog )
	{
		dialog->AddPartyMemberIcon( pMember );
	}
}
/*
void CMiniMapDlg::AddQuestIcon( CObject* pTarget, int nKind )
{
	if( nKind == 0 )
		AddIcon(eMNMICON_QUEST_EXCLAMATION, pTarget);
	else
		AddIcon(eMNMICON_QUEST_QUESTION, pTarget);
//---
	if( GAMEIN->GetBigMapDialog() )
		GAMEIN->GetBigMapDialog()->AddQuestIcon( pTarget, nKind );
}
*/
void CMiniMapDlg::AddHeroIcon(CObject* pObject)
{
	m_HeroIcon.Init(&m_pIconImage[eMNMICON_SELF],pObject, eMNMICON_SELF);
	m_HeroIcon.SetCameraImage( &m_pIconImage[eMNMICON_CAMERA] );

	CBigMapDlg* dialog = GAMEIN->GetBigMapDialog();

	if( dialog )
	{
		dialog->AddHeroIcon( pObject );
	}
}

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.10.31
void CMiniMapDlg::AddFarmIcon(CSHFarm* pFarm)
{
	AddIcon(eMNMICON_FARM, pFarm);

	CBigMapDlg* dialog = GAMEIN->GetBigMapDialog();

	if( dialog )
	{
		dialog->AddFarmIcon(pFarm);
	}
}
// E 농장시스템 추가 added by hseos 2007.10.31

CMiniMapIcon* CMiniMapDlg::GetEmptyMiniMapIcon(WORD Kind,CObject* pObject)
{
	CMiniMapIcon* pIcon = m_IconPool.Alloc();
	pIcon->Init(&m_pIconImage[Kind],pObject, Kind);
	pIcon->SetMarkImage(&m_pIconImage[eMNMICON_QUEST1],0, 3 );
	//pIcon->SetMarkImage(0, &m_pIconImage[eMNMICON_QUEST1], 3 );
	return pIcon;
}

CMiniMapIcon* CMiniMapDlg::GetEmptyMiniMapIcon(WORD Kind,PARTY_MEMBER* pMember)
{
	CMiniMapIcon* pIcon = m_IconPool.Alloc();
	pIcon->Init(&m_pIconImage[Kind], pMember, Kind);
	return pIcon;
}

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.10.31
CMiniMapIcon* CMiniMapDlg::GetEmptyMiniMapIcon(WORD Kind, CSHFarm* pFarm)
{
	CMiniMapIcon* pIcon = m_IconPool.Alloc();
	pIcon->Init(&m_pIconImage[Kind], pFarm, Kind);
	return pIcon;
}
// E 농장시스템 추가 added by hseos 2007.10.31

void CMiniMapDlg::ReleaseMinniMapIcon(CMiniMapIcon* pIcon)
{
	m_IconPool.Free(pIcon);
}


void CMiniMapDlg::AddIcon(WORD Kind,CObject* pObject)
{
	ASSERT(m_IconTable.GetData(pObject->GetID()) == NULL);
	RemoveIcon(pObject->GetID());
	CMiniMapIcon* pIcon = GetEmptyMiniMapIcon(Kind,pObject);
	m_IconTable.Add(pIcon,pObject->GetID());
}

void CMiniMapDlg::AddIcon(WORD Kind,PARTY_MEMBER* pMember)
{
	ASSERT(m_IconTable.GetData(pMember->dwMemberID) == NULL);
	RemoveIcon(pMember->dwMemberID);
	CMiniMapIcon* pIcon = GetEmptyMiniMapIcon(Kind,pMember);
	m_IconTable.Add(pIcon,pMember->dwMemberID);
}

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.10.31
void CMiniMapDlg::AddIcon(WORD Kind, CSHFarm* pFarm)
{
	ASSERT(m_IconTable.GetData(pFarm->GetID()+FARM_RENDER_OBJ_ID_START) == NULL);
	RemoveIcon(pFarm->GetID()+FARM_RENDER_OBJ_ID_START);
	CMiniMapIcon* pIcon = GetEmptyMiniMapIcon(Kind, pFarm);
	m_IconTable.Add(pIcon, pFarm->GetID()+FARM_RENDER_OBJ_ID_START);
}
// E 농장시스템 추가 added by hseos 2007.10.31

void CMiniMapDlg::RemoveIcon(DWORD ObjectID)
{
	CMiniMapIcon* pIcon = m_IconTable.GetData( ObjectID );

	if( pIcon )
	{
		ReleaseMinniMapIcon(pIcon);
		m_IconTable.Remove(ObjectID);
	}
}

void CMiniMapDlg::Process()
{
	const MINIMAPIMAGE& pMinimap = m_MinimapImage[m_CurMode];
	DWORD iWidth,iHeight;

	if(m_CurMode == eMINIMAPMODE_FULL)
	{
		iWidth = (DWORD)m_MiniMap.mViewport.x;
		iHeight = (DWORD)m_MiniMap.mViewport.y;
	}
	else
	{
		iWidth = pMinimap.size.cx;
		iHeight = pMinimap.size.cy;
	}

	m_HeroIcon.Update(iWidth,iHeight);
	// PartyIcon쨉쨉 Update횄횩째징
	PICONMGR->Process();
	
	if(m_pPosXY)
	{
		char text[128];
		VECTOR3 heropos = m_HeroIcon.GetPosition();

		sprintf( text, "%4.0f,%4.0f", heropos.x * 0.01f,heropos.z * 0.01f );
		//sprintf(temp,"%s-횄짚쨀횓%d %4.0f,%4.0f",GetMapName(m_MapNum),gChannelNum+1,heropos.x*.01f,heropos.z*.01f);
		//쨍횎?횑쨍짠째첬 횁횂횉짜쨈횂 쨉청쨌횓횄쨀쨍짰.
		m_pPosXY->SetStaticText(text);
	}

	/*if(m_CurMode == eMINIMAPMODE_FULL)
	FitToCenterIcon(NULL);
	else
	FitToCenterIcon(&m_HeroIcon);*/

	CMiniMapIcon* pIcon;
	m_IconTable.SetPositionHead();
	while(pIcon = m_IconTable.GetData())
	{
		pIcon->Update(iWidth,iHeight);
	}

	CBigMapDlg* dialog = GAMEIN->GetBigMapDialog();

	if( dialog )
	{
		dialog->Process();
	}

	//Jackpot
	//JPProcess();

	// change map image
	{
		cImageRect& rect = m_MiniMap.mRect;

		if( eMINIMAPMODE_FULL == m_CurMode )
		{
			rect.left = 0;
			rect.top = 0;
		}
		else
		{
			const VECTOR2* heroIconPosition = m_HeroIcon.GetScreenPos();

			rect.left = ( LONG )( heroIconPosition->x - m_MiniMap.mViewport.x / 2.0f );
			rect.top = ( LONG )( heroIconPosition->y - m_MiniMap.mViewport.y / 2.0f );
		}

		rect.right = rect.left + ( LONG )m_MiniMap.mViewport.x;
		rect.bottom = rect.top + ( LONG )m_MiniMap.mViewport.y;

		m_MiniMap.mImage.SetImageSrcRect( &rect );
	}
}

//void CMiniMapDlg::FitToCenterIcon(CMiniMapIcon* pIcon)
//{
//	if(pIcon == NULL)
//	{
//		m_DiplayOriginPosOfImage.x = 0;
//		m_DiplayOriginPosOfImage.y = 0;
//		return;
//	}
//	
//	m_DiplayOriginPosOfImage.x = m_HeroIcon.GetScreenPos()->x - (m_MiniMap.mViewport.x*.5f);
//	m_DiplayOriginPosOfImage.y = m_HeroIcon.GetScreenPos()->y - (m_MiniMap.mViewport.y*.5f);
//
//	m_DisplayMinX = m_DiplayOriginPosOfImage.x;
//	m_DisplayMinZ = m_DiplayOriginPosOfImage.y;
//	m_DisplayMaxX = (m_DiplayOriginPosOfImage.x+m_MiniMap.mViewport.x);
//	m_DisplayMaxZ = (m_DiplayOriginPosOfImage.y+m_MiniMap.mViewport.y);
//}

void CMiniMapDlg::LoadMinimapImageInfo(MAPTYPE MapNum)
{
	DIRECTORYMGR->SetLoadMode(eLM_Minimap);

	char buffer[256];
	CMHFile file;

	sprintf( buffer, "Minimap%d.bin", MapNum );
	file.Init( buffer, "rb", MHFILE_FLAG_DONOTCHANGE_DIRECTORY_TOROOT );

	if( ! file.IsInited() )
	{
		DIRECTORYMGR->SetLoadMode(eLM_Root);
		return;
	}
	
	char name[ 256 ];

	{
		file.GetString( buffer );
		sprintf( name, "../image/%s", buffer );

		MINIMAPIMAGE& data = m_MinimapImage[ eMINIMAPMODE_SMALL ];

		data.size.cx = file.GetDword();
		data.size.cy = file.GetDword();
		data.image.LoadSprite( name );
	}
	
	{
		file.GetString( buffer );
		sprintf( name, "../image/%s", buffer );

		MINIMAPIMAGE& data = m_MinimapImage[ eMINIMAPMODE_FULL ];

		data.size.cx = file.GetDword();
		data.size.cy = file.GetDword();
		data.image.LoadSprite( name );
	}
	
	//file.Release();	
	DIRECTORYMGR->SetLoadMode(eLM_Root);

	CBigMapDlg* dialog = GAMEIN->GetBigMapDialog();
	if( dialog )
	{
		dialog->LoadMinimapImageInfo( MapNum );
	}

	/// 미니맵 초기화
	{
		if( eMINIMAPMODE_SMALL == m_CurMode )
		{
			const SIZE& size = m_MinimapImage[ eMINIMAPMODE_SMALL ].size;

			m_MiniMap.mScale.x = m_MiniMap.mViewport.x / size.cx;
			m_MiniMap.mScale.y = m_MiniMap.mViewport.y / size.cy;
		}
		else
		{
			m_MiniMap.mScale.x = 1.0f;
			m_MiniMap.mScale.y = 1.0f;
		}

		m_MiniMap.mImage.SetSpriteObject( m_MinimapImage[ m_CurMode ].image.GetSpriteObject() );
		m_MiniMap.mImage.SetImageSrcSize( &m_MiniMap.mViewport );
	}
}

DWORD CMiniMapDlg::ActionEvent( CMouse* mouseInfo )
{
	DWORD we = cDialog::ActionEvent( mouseInfo );

	if( ! m_bActive )
	{
		return we;
	}

	if( we & WE_MOUSEOVER )
	{
		cImageRect& rect = m_MiniMap.mRect;;
		/*rect.left = (LONG)m_DiplayOriginPosOfImage.x;
		rect.top = (LONG)m_DiplayOriginPosOfImage.y;
		rect.right = (LONG)(m_DiplayOriginPosOfImage.x+m_MiniMap.mViewport.x);
		rect.bottom = (LONG)(m_DiplayOriginPosOfImage.y+m_MiniMap.mViewport.y);*/
		
		char text[128] = {0,};
		
		VECTOR2 trans;
		trans.x = GetAbsX() + m_MiniMap.mTranslation.x - rect.left;
		trans.y = GetAbsY() + m_MiniMap.mTranslation.y - rect.top;
		
		CMiniMapIcon* pIcon;
		m_IconTable.SetPositionHead();
		while(pIcon = m_IconTable.GetData())
		{
			int sort = pIcon->GetToolTip(mouseInfo->GetMouseX(),mouseInfo->GetMouseY(),&trans,text);

			if(sort == 1)				// Npc
			{
				SetToolTip( text, RGB_HALF( 255, 255, 0 ) );
			}
			else if(sort == 2)			// Party
			{
				SetToolTip( text, RGB_HALF(185, 241, 69) );
			}
			// desc_hseos_농장시스템_01
			// S 농장시스템 추가 added by hseos 2007.10.31
			else if(sort == 3)			// Party
			{
				SetToolTip( text, RGB_HALF( 255, 255, 0 ) );
			}
			// E 농장시스템 추가 added by hseos 2007.10.31
		}
 
		if( ! strlen( text ) )
		{
			SetToolTip("");
		}
	}

	return we;
}

void CMiniMapDlg::Render()
{
	if( !	m_bActive ||
			m_MiniMap.mImage.IsNull() )
	{
		return;
	}

	cDialog::RenderWindow();
	
	VECTOR2 trans;
	trans.x = GetAbsX() + m_MiniMap.mTranslation.x;
	trans.y = GetAbsY() + m_MiniMap.mTranslation.y;
	
	m_MiniMap.mImage.RenderSprite( &m_MiniMap.mScale, 0, 0, &trans, m_dwImageRGB );

	cImageRect* rect = &m_MiniMap.mRect;

	trans.x -= rect->left;
	trans.y -= rect->top;
	
	m_IconTable.SetPositionHead();
	for( CMiniMapIcon* pIcon; pIcon = m_IconTable.GetData(); )
	{
		pIcon->Render(&trans,rect);
	}

	m_IconTable.SetPositionHead();
	for( CMiniMapIcon* pIcon; pIcon = m_IconTable.GetData(); )
	{
		pIcon->RenderQuestMark(&trans,rect);
	}

	//////////////////////////////////////////////////////////////////////////
	// 횁횜?횓째첩 쨋챌쩔챙짹창
	m_HeroIcon.Render(&trans);

	cDialog::RenderComponent();

	//jackpot

	JPRender();

}

void CMiniMapDlg::RefreshMode()
{
	if( m_pZoom )
		m_pZoom->SetPush( GAMEIN->GetBigMapDialog()->IsActive() );
}

void CMiniMapDlg::ToggleMinimapMode()
{
	if( GAMEIN->GetBigMapDialog() )
	{
		GAMEIN->GetBigMapDialog()->SetActive( !GAMEIN->GetBigMapDialog()->IsActive() );
	}

	RefreshMode();	

/*
	if(m_CurMode == eMINIMAPMODE_FULL)
		m_CurMode = eMINIMAPMODE_SMALL;
	else
		m_CurMode = eMINIMAPMODE_FULL;
*/
}

void CMiniMapDlg::SetActive( BOOL val )
{
	if( CanActive() == FALSE )
		val = FALSE;
	
	cDialog::SetActive( val );

//--- main bar icon
//	CMainBarDialog* pDlg = GAMEIN->GetMainInterfaceDialog();
//	if( pDlg )
//		pDlg->SetPushBarIcon( OPT_MINIMAPDLGICON, m_bActive );
}


void CMiniMapDlg::SetPartyIconTargetPos(DWORD PlayerID, int posX, int posZ)
{
	CMiniMapIcon* pIcon = m_IconTable.GetData(PlayerID);
	if(pIcon)
		pIcon->SetTargetPos(posX, posZ);

//---
	if( GAMEIN->GetBigMapDialog() )
		GAMEIN->GetBigMapDialog()->SetPartyIconTargetPos( PlayerID, posX, posZ );
}

void CMiniMapDlg::SetPartyIconObjectPos(DWORD PlayerID, int posX, int posZ)
{
	CMiniMapIcon* pIcon = m_IconTable.GetData(PlayerID);
	if(pIcon)
	{
		pIcon->SetObjectPos(posX, posZ);
		pIcon->SetTargetPos(posX, posZ);
	}

//---
	if( GAMEIN->GetBigMapDialog() )
		GAMEIN->GetBigMapDialog()->SetPartyIconObjectPos( PlayerID, posX, posZ );
}

BOOL CMiniMapDlg::CanActive()
{
	/*
	switch( MAP->GetMapNum() )
	{
	case BOSSMONSTER_MAP:		// bossmap
	case BOSSMONSTER_2NDMAP:	// 2nd bossmap
	case Tournament:			// 토너먼트
	case QuestRoom:				// 기연의방
	case dongkyung:				// 이벤트맵
	case jangbeak:				// 이벤트맵
		return FALSE;
		break;
	}
	*/

	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.12.03
	// ..챌린지 존에는 미니맵이 없다.
	if (g_csDateManager.IsChallengeZone(MAP->GetMapNum()))
	{
		return FALSE;
	}
	// E 데이트 존 추가 added by hseos 2007.12.03

	return TRUE;
}

void CMiniMapDlg::ShowQuestMarkIcon( CObject* pTarget, int nKind )
{
	CMiniMapIcon* pIcon = m_IconTable.GetData( pTarget->GetID() );
	if( pIcon == NULL ) return;

	pIcon->ShowQuestMark( nKind );

//---
	if( GAMEIN->GetBigMapDialog() )
		GAMEIN->GetBigMapDialog()->ShowQuestMarkIcon( pTarget, nKind );
}


//SW050722 jackpot_number
LONG NumImgRelpos[] =
{
	135, 191,
	119, 191,
	103, 191,
	87, 191,
	71, 191,
	55, 191,
	39, 191,
	23, 191,
	7, 191,
};

void CMiniMapDlg::InitNumImage()
{
	cImage* pImg;
	for( int n = 0; n < IMAGE_COUNT; ++n )
	{
		pImg = new cImage;
		SCRIPTMGR->GetImage(n, pImg, PFT_JACKPOTPATH);
		m_stNumImage[n].pImage = pImg;
//		m_stNumImage[n].dwW = NUMIMAGE_W;
//		m_stNumImage[n].dwH = NUMIMAGE_H;
	}
}

void CMiniMapDlg::ReleaseNumImage()
{
	for( int n = 0; n < IMAGE_COUNT; ++n )
	{
		if(m_stNumImage[n].pImage)
		{
			delete m_stNumImage[n].pImage;
			m_stNumImage[n].pImage = NULL;
		}
	}
}

void CMiniMapDlg::SetNumImagePos()
{
	float fDialPosX = this->GetAbsX();
	float fDialPosY = this->GetAbsY();

	for( int i = 0; i < CIPHER_NUM; ++i )
	{
		m_vPos[i].x = fDialPosX + (float)NumImgRelpos[i*2];
		m_vPos[i].y = fDialPosY + (float)NumImgRelpos[i*2+1];
	}

	//SW050805 자리수 구분점 추가
	m_vDotPos[0].x = fDialPosX + 98;
	m_vDotPos[0].y = fDialPosY + 205;
	m_vDotPos[1].x = fDialPosX + 50;
	m_vDotPos[1].y = fDialPosY + 205;
}

void CMiniMapDlg::ConvertCipherNum()
{// 각자리 숫자를 구한다. Get Number per Each Cipher
	// 숫자 변화시 갱신 애니를 한다. Do Animations When Number Changers
/*
	int n = 0;	// n자리수
	DWORD d = 1;	// 단위

	DWORD money = m_dwTotalMoney;

	if( !money ) return;
	while( money / d > 0 )
	{//	자리수 구하기 n
		d *= 10;
		n++;
	}

	ASSERT(n<CIPHER_NUM);
	d /= 10;

	m_dwMaxCipher = n;	// Need in ChangingAni

	for( int i = 1; i <= n; ++i )
	{// 큰자리 부터
		m_stCipherNum[n-i].dwNumber = money / d;
		m_stCipherNum[n-i].bIsAni = TRUE;
		money = m_dwTotalMoney % d;
		d /= 10;
	}
*/
	int n = 1;	// n자리수 0 도 1자리로 본다. 그리기위해!
	DWORD d = 10;
	while( m_dwTotalMoney / d > 0 )
	{
		d *= 10;
		n++;
	}
	
	m_dwMaxCipher = n;

	DWORD dec = 10;
	DWORD tmp = m_dwTotalMoney;

	for( int i = 0; i < n; ++i )
	{
		m_stCipherNum[i].dwNumber = tmp % dec;
		m_stCipherNum[i].bIsAni = TRUE;
		tmp /= dec;
	}

	while( n < CIPHER_NUM )
	{
		m_stCipherNum[n].dwNumber = DEFAULT_IMAGE;
		m_stCipherNum[n].bIsAni = FALSE;
		++n;
	}
}

bool CMiniMapDlg::IsNumChanged()					// 숫자 변화 체크	// Check Num charnges
{
	if( m_dwOldTotalMoney != m_dwTotalMoney )
	{
		if( m_dwTotalMoney < m_dwOldTotalMoney || m_dwOldTotalMoney == 0 )	// 돈 줄거나 맵처음 접속이면..
		{
			m_bDoSequenceAni = FALSE;
		}
		else
		{
			m_bDoSequenceAni = TRUE;
		}
		
		m_dwTempMoney		= m_dwOldTotalMoney;	// For SequenceAni
		m_dwOldTotalMoney	= m_dwTotalMoney;	// For CompareChanges
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CMiniMapDlg::DoAni()				// 숫자 변환 애니	// Ticktime control
{
	if( ! m_bIsAnimationing )
	{
		return;
	}

	if( gCurTime - m_dwNumChangeTime > NUM_CHANGE_TIMELENGTH )
	{
		for( int i = 0; i < CIPHER_NUM; ++i )
		{
			if( m_stCipherNum[i].bIsAni )
			{
				++m_stCipherNum[i].dwNumber;

				if(m_stCipherNum[i].dwNumber > 9)
				{
					m_stCipherNum[i].dwNumber = 0;
				}
			}
		}

//		m_dwNumChangeTime = curtime;
	}

	if( gCurTime - m_dwAniStartTime >= BASIC_ANI_TIMELENGTH	&&
		gCurTime - m_dwIntervalAniTime > BETWEEN_ANI_TIMELENGTH )
	{
		if(m_stCipherNum[m_dwCipherCount].bIsAni)
		{
			m_stCipherNum[m_dwCipherCount].dwNumber = m_stCipherNum[m_dwCipherCount].dwRealCipherNum;
			m_stCipherNum[m_dwCipherCount].bIsAni = FALSE;
		}

		if(m_dwCipherCount == m_dwMaxCipher)
		{
			m_bIsAnimationing = FALSE;
			return;
		}

		m_dwIntervalAniTime = gCurTime;
		++m_dwCipherCount;
	}
}

void CMiniMapDlg::DoSequenceAni()
{
	if( m_bIsAnimationing )
	{
		DWORD EllipsedTime	=  gCurTime - m_dwAniStartTime;
		DWORD durMoney		=  EllipsedTime * 33;
		m_dwTotalMoney		= m_dwTempMoney + durMoney;

		if( m_dwTotalMoney >= m_dwOldTotalMoney )
		{
			m_dwTotalMoney = m_dwOldTotalMoney;
			m_bIsAnimationing = FALSE;
		}
	}
}

void CMiniMapDlg::JPRender()
{
	//if(!m_bActive) return;
	// 숫자 이미지 출력 // 포즈값, 이미지번호
	for( int i = 0; i < CIPHER_NUM; ++i )
	{
		if(m_stCipherNum[i].dwNumber != DEFAULT_IMAGE)
		{
			m_stNumImage[m_stCipherNum[i].dwNumber].pImage->RenderSprite( NULL, NULL, 0.0f, &m_vPos[i], 0xffffffff );
		}	
	}

	//SW050805 자리수 구분점 추가. 4~9자리만 처리
	if( 3	< m_dwMaxCipher && 
		10	> m_dwMaxCipher )
	{
		for( int i = ( m_dwMaxCipher - 1 ) / 3; i--; )
		{
			m_stNumImage[IMG_DOT].pImage->RenderSprite( NULL, NULL, 0.0f, &m_vDotPos[ i ], 0xffffffff );
		}
	}
}

void CMiniMapDlg::JPProcess()
{// JACKPOTMGR로 부터 TotalMoney 갱신
	m_dwTotalMoney = JACKPOTMGR->GetJPTotalMoney();
	if( m_dwTotalMoney > 999999999 )
	{
		m_dwTotalMoney = 999999999;
	}
	
	SetNumImagePos();
/*
	// route for DoAni	//하나씩 변하는 애니
//	if(IsNumChanged())
//	{
//		ConvertCipherNum();
//		InitForAni();
//	}
//
//	DoAni();
*/

	// route for DoSequenceAni	//쌓이는 듯한 애니
	if(IsNumChanged())
	{
		InitForSequenceAni();
	}

	DoSequenceAni();
	
	ConvertCipherNum();
}

void CMiniMapDlg::InitForAni()
{
	//실제 숫자를 저장해두고 모든 숫자 애니를 불가 상태로 둔다.
	for( int i = 0; i < CIPHER_NUM; ++i )
	{
		m_stCipherNum[i].dwRealCipherNum = m_stCipherNum[i].dwNumber;
	}
	// 전체 애니 시작
	m_bIsAnimationing = TRUE;
	// 애니 카운터 초기화
	m_dwCipherCount = 0;
	// 애니 시작 시각
	m_dwAniStartTime = gCurTime;
}

void CMiniMapDlg::InitForSequenceAni()
{
	if(!m_bDoSequenceAni) return;

	m_bIsAnimationing = TRUE;

	m_dwAniStartTime = gCurTime;

	m_dwTotalMoney = m_dwOldTotalMoney;// When TotalMoney Decrease // 돈줄어들면 그냥 찍고만다.(KES PM님 지시)
}

void CMiniMapDlg::JPInit()
{
	memset(m_vPos, 0, sizeof(VECTOR2)*CIPHER_NUM);		// 숫자창 자리수 위치

	m_dwTotalMoney	= 0;			// 총금액
	m_dwOldTotalMoney	= 0;
	m_dwTempMoney	= 0;

	m_dwAniStartTime	= 0;
	m_dwNumChangeTime	= 0;
	m_dwIntervalAniTime	= 0;	
	m_dwMaxCipher	= 0;
	m_dwCipherCount	= 0;
	m_bIsAnimationing	= FALSE;
	m_bDoSequenceAni	= FALSE;

	ConvertCipherNum();
	//SW050802 snake_legs
//	m_BeforeTime = 0;
}

/*	//SW050802 snake_legs
void CMiniMapDlg::SetParticlePos()
{
	for( int i = 0; i < CIPHER_NUM; ++i )
	{
		for( int j = 0; j < PARTICLE_PER_NUM; ++j )
		{
			int sign_x = rand()%2;
			sign_x > 0 ? sign_x = 1 : sign_x = -1;
			m_stPtc[i][j].vDir.x = rand()%RND_RANGE * sign_x;
			m_stPtc[i][j].vPos.x = m_vPos[i].x;
			int sign_y = rand()%2;
			sign_y > 0 ? sign_y = 1 : sign_y = -1;
			m_stPtc[i][j].vDir.y = rand()%RND_RANGE * sign_y;
			m_stPtc[i][j].vPos.y = m_vPos[i].y;
		}
	}
}
	
void CMiniMapDlg::CalcMotion()
{
	if( gCurTime - m_BeforeTime < 250 ) return;
	for( int i = 0; i < CIPHER_NUM; ++i )
	{
		for( int j = 0; j < PARTICLE_PER_NUM; ++j )
		{
			m_stPtc[i][j].vPos.x += m_stPtc[i][j].vDir.x;
			m_stPtc[i][j].vDir.y -= GRAVITY;
			m_stPtc[i][j].vPos.y += m_stPtc[i][j].vDir.y;
		}
	}

	m_BeforeTime = gCurTime;
}
*/

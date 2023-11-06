// BigMapDlg.cpp: implementation of the CBigMapDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BigMapDlg.h"
#include "MHFile.h"
#include "GameIn.h"
#include "Npc.h"
#include "Interface/cScriptManager.h"
#include "Interface/cWindowManager.h"
#include "Input/Mouse.h"
#include "MiniMapDlg.h"

// 071022 LYW --- BigMapDlg : Include header file of tutorial manager.
#include "TutorialManager.h"

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.10.04
#include "../hseos/Farm/SHFarmManager.h"
// E 농장시스템 추가 added by hseos 2007.10.04

// desc_hseos_데이트 존_01
// S 데이트 존 추가 added by hseos 2007.12.03
#include "../hseos/Date/SHDateManager.h"
// E 데이트 존 추가 added by hseos 2007.12.03

const DWORD IconDescription[] = 
{
	// index, width, height
	0,	14,	18,			//eBMICON_SELF,
	1,	16,	16,			//eBMICON_PARTYMEMBER,
	2,	16,	16,			//eBMICON_FAMILYMEMBER,
	3,	16,	16,			//eBMICON_STORE,
	4,	16,	16,			//eBMICON_TRAINER,
	5,	16,	16,			//eBMICON_BANK,
	6,	16,	16,			//eBMICON_NOMALNPC,
	7,	16,	16,			//eBMICON_LOADINGPOINT,
	8,	16,	16,			//eBMICON_LOGIN,
	9,	20, 48,			//eBMICON_CAMERA,
	10,	18, 18,			//eBMICON_QUEST1,
	11,	18, 18,			//eBMICON_QUEST2,
	12,	18, 18,			//eBMICON_QUEST3,
	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.10.31
	6,	20, 48,			//eBMICON_FARM,
	// E 농장시스템 추가 added by hseos 2007.10.31
};


//#define BIGMAP_WIDTH	(512)
//#define BIGMAP_HEIGHT	(512)

#define MAP_WIDTH	512
#define MAP_HEIGHT	512

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBigMapDlg::CBigMapDlg()
{
	m_dwImageRGB = RGBA_MAKE( 255, 255, 255, 190 );
	m_IconTable.Initialize(64);
	m_IconPool.Init(100, 10, "MinimapIconPool");
}

CBigMapDlg::~CBigMapDlg()
{
	CMiniMapIcon* pIcon;
	m_IconTable.SetPositionHead();

	while(pIcon = m_IconTable.GetData())
	{
		ReleaseMinniMapIcon(pIcon);
	}
}

void CBigMapDlg::Linking()
{
	//memset(m_pIconImage,0,sizeof(cImage*)*eBMICON_MAX);
	ZeroMemory( m_pIconImage, sizeof( m_pIconImage ) );
	InitIconImage();
}

void CBigMapDlg::LoadMinimapImageInfo( MAPTYPE MapNum )
{
	DIRECTORYMGR->SetLoadMode(eLM_Minimap);

	char	name[ 80 ];
	CMHFile file;

	sprintf( name, "Minimap%d.bin", MapNum );
	file.Init( name, "rb", MHFILE_FLAG_DONOTCHANGE_DIRECTORY_TOROOT );

	if( ! file.IsInited())
	{
		DIRECTORYMGR->SetLoadMode(eLM_Root);
		return;
	}
	
	for( int i = 0 ; i <eMINIMAPMODE_MAX ; ++i )
	{
		file.GetString( name );

		if( file.IsEOF() )
			break;
		
		file.GetDword();
		file.GetDword();

		if( eMINIMAPMODE_SMALL == i )
		{
			m_MapImage.LoadSprite( name );
			
			const cImageRect r = { 0, 0, MAP_HEIGHT, MAP_HEIGHT };

			m_MapImage.SetImageSrcRect( &r );
		}
	}
	
	DIRECTORYMGR->SetLoadMode(eLM_Root);
}

void CBigMapDlg::InitBigMap( MAPTYPE MapNum )
{
	const eGameInInitKind kind	= ( eGameInInitKind )GAMEIN->GetGameInInitKind();

/*	if( eGameInInitKind_SuryunEnter		== kind		||
		eGameInInitKind_EventMapEnter	== kind		||
		BOSSMONSTER_MAP					== MapNum	||
		BOSSMONSTER_2NDMAP				== MapNum	)
	{
		SetActive(FALSE);
	}

*/	LoadMinimapImageInfo( m_MapNum = MapNum );
}

void CBigMapDlg::AddStaticNpcIcon(CObject* pObject)
{
	CNpc* pNpc = (CNpc*)pObject;
	NPC_TOTALINFO* pInfo = pNpc->GetNpcTotalInfo() ;

	if(pInfo->NpcJob == 1)
	{
		AddIcon(eBMICON_STORE,pObject);
	}
	else if(pInfo->NpcJob == 10)
	{
		AddIcon(eBMICON_TRAINER,pObject);
	}
	else if(pInfo->NpcJob == 4)
	{
		AddIcon(eBMICON_BANK,pObject);
	}
	else if(pInfo->NpcJob == 7)
	{
		AddIcon(eBMICON_LOGIN,pObject);
	}
	else if(pInfo->NpcJob == 8)
	{
		AddIcon(eBMICON_LOADINGPOINT,pObject);
	}
	else
	{
		AddIcon(eBMICON_NOMALNPC,pObject);
	}
}

void CBigMapDlg::AddPartyMemberIcon(PARTY_MEMBER* pMember)
{
	AddIcon(eBMICON_PARTYMEMBER,pMember);
}

void CBigMapDlg::AddHeroIcon(CObject* pObject)
{
	m_HeroIcon.Init(&m_pIconImage[eBMICON_SELF],pObject, eBMICON_SELF);
	m_HeroIcon.SetRGBColor( RGBA_MAKE(255,255,255,220) );
	m_HeroIcon.SetAlwaysTooltip( TRUE );
	m_HeroIcon.SetCameraImage( &m_pIconImage[eBMICON_CAMERA] );
}

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.10.31
void CBigMapDlg::AddFarmIcon(CSHFarm* pFarm)
{
	AddIcon(eBMICON_FARM, pFarm);
}
// E 농장시스템 추가 added by hseos 2007.10.31


CMiniMapIcon* CBigMapDlg::GetEmptyMiniMapIcon(WORD Kind,CObject* pObject)
{
	CMiniMapIcon* pIcon = m_IconPool.Alloc();
	pIcon->Init(&m_pIconImage[Kind],pObject, Kind);
	pIcon->SetMarkImage(&m_pIconImage[eBMICON_QUEST1],0, 3 );//&m_pIconImage[eBMICON_QUEST_QUESTION]);
	//pIcon->SetMarkImage(0, &m_pIconImage[eBMICON_QUEST1], 3 );//&m_pIconImage[eBMICON_QUEST_QUESTION]);
	return pIcon;
}

CMiniMapIcon* CBigMapDlg::GetEmptyMiniMapIcon(WORD Kind,PARTY_MEMBER* pMember)
{
	CMiniMapIcon* pIcon = m_IconPool.Alloc();
	pIcon->Init(&m_pIconImage[Kind], pMember, Kind);
	return pIcon;
}

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.10.31
CMiniMapIcon* CBigMapDlg::GetEmptyMiniMapIcon(WORD Kind, CSHFarm* pFarm)
{
	CMiniMapIcon* pIcon = m_IconPool.Alloc();
	pIcon->Init(&m_pIconImage[Kind], pFarm, Kind);
	return pIcon;
}
// E 농장시스템 추가 added by hseos 2007.10.31

void CBigMapDlg::ReleaseMinniMapIcon(CMiniMapIcon* pIcon)
{
	m_IconPool.Free(pIcon);
}


void CBigMapDlg::AddIcon(WORD Kind,CObject* pObject)
{
	ASSERT(m_IconTable.GetData(pObject->GetID()) == NULL);
	RemoveIcon(pObject->GetID());
	CMiniMapIcon* pIcon = GetEmptyMiniMapIcon(Kind,pObject);
	m_IconTable.Add(pIcon,pObject->GetID());
	pIcon->SetRGBColor( RGBA_MAKE(255,255,255,220) );
	pIcon->SetAlwaysTooltip( TRUE );
}

void CBigMapDlg::AddIcon(WORD Kind,PARTY_MEMBER* pMember)
{
	ASSERT(m_IconTable.GetData(pMember->dwMemberID) == NULL);
	RemoveIcon(pMember->dwMemberID);
	CMiniMapIcon* pIcon = GetEmptyMiniMapIcon(Kind,pMember);
	m_IconTable.Add(pIcon,pMember->dwMemberID);
	pIcon->SetRGBColor( RGBA_MAKE(255,255,255,220) );
	pIcon->SetAlwaysTooltip( TRUE );
}

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.10.31
void CBigMapDlg::AddIcon(WORD Kind, CSHFarm* pFarm)
{
	ASSERT(m_IconTable.GetData(pFarm->GetID()+FARM_RENDER_OBJ_ID_START) == NULL);
	RemoveIcon(pFarm->GetID()+FARM_RENDER_OBJ_ID_START);
	CMiniMapIcon* pIcon = GetEmptyMiniMapIcon(Kind, pFarm);
	m_IconTable.Add(pIcon, pFarm->GetID()+FARM_RENDER_OBJ_ID_START);
	pIcon->SetRGBColor( RGBA_MAKE(255,255,255,220) );
	pIcon->SetAlwaysTooltip( TRUE );
}
// E 농장시스템 추가 added by hseos 2007.10.31

void CBigMapDlg::RemoveIcon(DWORD ObjectID)
{
	CMiniMapIcon* pIcon = (CMiniMapIcon*)m_IconTable.GetData(ObjectID);

	if( pIcon )
	{
		ReleaseMinniMapIcon(pIcon);
		m_IconTable.Remove(ObjectID);
	}
}


void CBigMapDlg::Render()
{
	if( !	m_bActive	||
			m_MapImage.IsNull() )
	{
		return;
	}

	cDialog::RenderWindow();

//	m_MapImage.SetImageSrcRect(&rect);

//	VECTOR2 trans;
//	trans.x = GetAbsX();
//	trans.y = GetAbsY();

	if( this->IsMovedWnd() )
	{
		m_vScale.x =  MAP_WIDTH / 512.f ;
		m_vScale.y =  MAP_HEIGHT / 512.f ;

		SetMovedWnd( FALSE ) ;
	}

//	m_MapImage.RenderSprite( NULL, NULL, 0, &m_absPos, color );
	m_MapImage.RenderSprite( &m_vScale, NULL, 0, &m_absPos, m_dwImageRGB );
	
	m_IconTable.SetPositionHead();
	for( CMiniMapIcon* pIcon; pIcon = m_IconTable.GetData(); )
	{
		pIcon->Render( &m_absPos, NULL );	
		pIcon->RenderQuestMark(&m_absPos,NULL);
	}

	m_HeroIcon.Render(&m_absPos);

//가장위에 한번더
	if( m_pIconForToolTip )
	{
		m_pIconForToolTip->Render( &m_absPos, NULL );
		m_pIconForToolTip->RenderQuestMark( &m_absPos,NULL );
	}

	cDialog::RenderComponent();
}

void CBigMapDlg::SetPartyIconObjectPos(DWORD PlayerID, int posX, int posZ)
{
	CMiniMapIcon* pIcon = m_IconTable.GetData(PlayerID);

	if(pIcon)
	{
		pIcon->SetObjectPos(posX, posZ);
		pIcon->SetTargetPos(posX, posZ);
	}
}

void CBigMapDlg::SetPartyIconTargetPos(DWORD PlayerID, int posX, int posZ)
{
	CMiniMapIcon* pIcon = m_IconTable.GetData(PlayerID);

	if(pIcon)
	{
		pIcon->SetTargetPos(posX, posZ);
	}
}

BOOL CBigMapDlg::CanActive()
{
/*	switch( m_MapNum )
	{
	case BOSSMONSTER_MAP:		// bossmap
	case BOSSMONSTER_2NDMAP:	// 2nd bossmap
	case Tournament:			// 토너먼트
	case QuestRoom:				// 기연의방
	case dongkyung:				// 이벤트맵
	case jangbeak:				// 이벤트맵
		return FALSE;
	}
*/

	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.12.03
	// ..챌린지 존에는 미니맵이 없다.
	if (g_csDateManager.IsChallengeZone(m_MapNum))
	{
		return FALSE;
	}
	// E 데이트 존 추가 added by hseos 2007.12.03

	return TRUE;
}

void CBigMapDlg::InitIconImage()
{
	const SIZE_T size = sizeof( IconDescription ) / ( sizeof( DWORD ) * 3 );

	for( SIZE_T i = 0; i < size; ++i )
	{
		cImage*	image = new cImage;

		const DWORD index	= IconDescription[ 3 * i ];
		const DWORD width	= IconDescription[ 3 * i + 1 ];
		const DWORD height	= IconDescription[ 3 * i + 2 ];
		
		SCRIPTMGR->GetImage( index, image, PFT_MINIMAPPATH );

		m_pIconImage[ i ].InitIconImage( image, width, height );
	}

	SCRIPTMGR->GetImage( 0, &m_TooltipImage, PFT_HARDPATH );

	m_pIconForToolTip = NULL;
}

void CBigMapDlg::Process()
{
//	MINIMAPIMAGE* pMinimap = &m_MinimapImage[m_CurMode];
//	DWORD iWidth,iHeight;
//	if(m_CurMode == eMINIMAPMODE_FULL)
//	{
//		iWidth = m_ImageDisplayWH.x;
//		iHeight = m_ImageDisplayWH.y;
//	}
//	else
//	{
//		iWidth = pMinimap->ImageWidth;
//		iHeight = pMinimap->ImageHeight;
//	}	

	m_HeroIcon.Update( MAP_WIDTH, MAP_HEIGHT );

	CMiniMapIcon* pIcon;
	m_IconTable.SetPositionHead();
	while(pIcon = m_IconTable.GetData())
	{
		pIcon->Update( MAP_WIDTH, MAP_HEIGHT );
	}

//	VECTOR2* pHeroPos = m_HeroIcon.GetScreenPos();

//	SetAbsXY( (1024-BIGMAP_WIDTH)/2 - pHeroPos->x, (768-BIGMAP_HEIGHT)/2 - pHeroPos->y + 70 );
//	SetAbsXY( 1024/2 - pHeroPos->x, 768/2 - pHeroPos->y - 70 );
//	m_absPos.x = (1024-768)/2;
//	m_absPos.y = 0;


//	PICONMGR->Process();	
}

DWORD CBigMapDlg::ActionEvent( CMouse* mouseInfo )
{
	const DWORD we = cDialog::ActionEvent( mouseInfo );

	// 활성화 상태가 아니거나 파괴된 상태면 처리 이유 없음
	if( !	m_bActive	||
			m_bDestroy )
	{
		return we;
	}
	
	m_dwViewCurTime = GetTickCount();

	if( !	WINDOWMGR->IsMouseOverUsed()									&&	// 마우스 커서가 윈도우에 위치한 상태이고
			PtInWindow( mouseInfo->GetMouseX(), mouseInfo->GetMouseY() )	&&	// 맵 창 영역안에 존재하며
			WINDOWMGR->GetDragDlg() != this									&& 	// 드래그 중인 창이 이 윈도우가 아니면
			TOOLTIP_DELAY_TIME < m_dwViewCurTime - m_dwViewLastTime	)			// 표시 가능한 시간이면
	{
		m_ttX = mouseInfo->GetMouseX();
		m_ttY = mouseInfo->GetMouseY();
	}
	else
	{
		m_dwViewLastTime = m_dwViewCurTime = 0;
	}

	// 툴팁을 표시한다
	{
		char text[ 128 ] = { 0, };
		VECTOR2 trans;
		trans.x = GetAbsX();
		trans.y = GetAbsY();
		
		float olddist = 10000.0f;	//충분히 큰수
		
		m_IconTable.SetPositionHead();
		
		for(	CMiniMapIcon* pIcon;
				pIcon = m_IconTable.GetData()
				; )
		{
			float dist;
					
			switch( pIcon->GetToolTip(	mouseInfo->GetMouseX(),
										mouseInfo->GetMouseY(),
										&trans,
										text,
										&dist ) )
			{
			case 1:	// NPC
				{
					if( dist < olddist )
					{
						m_pIconForToolTip	= pIcon;
						olddist				= dist;

						SetToolTip( text, RGB_HALF( 255, 255, 0 ), &m_TooltipImage, RGB_HALF( 0, 0, 0 ) );
						SetWH( MAP_WIDTH, MAP_HEIGHT );
					}
					
					break;
				}
			case 2:	// Party
				{
					if( dist < olddist )
					{
						m_pIconForToolTip	= pIcon;
						olddist				= dist;

						SetToolTip( text, RGB_HALF(185, 241, 69), &m_TooltipImage, RGB_HALF( 0, 0, 0 ) );
						SetWH( MAP_WIDTH, MAP_HEIGHT );
					}

					break;
				}
			// desc_hseos_농장시스템_01
			// S 농장시스템 추가 added by hseos 2007.10.31
			case 3:	// Farm
				{
					if( dist < olddist )
					{
						m_pIconForToolTip	= pIcon;
						olddist				= dist;

						SetToolTip( text, RGB_HALF( 255, 255, 0 ), &m_TooltipImage, RGB_HALF( 0, 0, 0 ) );
						SetWH( MAP_WIDTH, MAP_HEIGHT );
					}

					break;
				}
			// E 농장시스템 추가 added by hseos 2007.10.31
			}
		}

		if( ! strlen( text ) )
		{
			SetToolTip( "" );
			SetWH( 0, 0 );

			m_pIconForToolTip = NULL;
		}
	}

	if( m_bDisable )
	{
		return we | ActionEventComponent( mouseInfo );
	}

	return we | ActionEventWindow( mouseInfo) | ActionEventComponent( mouseInfo );
}


void CBigMapDlg::ShowQuestMarkIcon( CObject* pTarget, int nKind )
{
	CMiniMapIcon* pIcon = m_IconTable.GetData( pTarget->GetID() );

	if( pIcon )
	{
		pIcon->ShowQuestMark( nKind );
	}
}


void CBigMapDlg::SetActive( BOOL isActive )
{
	cDialog::SetActive( CanActive() && isActive );

	CMiniMapDlg* dialog = GAMEIN->GetMiniMapDialog();

	if( dialog )
	{
		dialog->RefreshMode();
	}

	// 071022 LYW --- BigMapDlg : Check open dialog.
	TUTORIALMGR->Check_OpenDialog(this->GetID(), isActive) ;
}

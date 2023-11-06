#include "stdafx.h"
#include "ExchangeItem.h"
#include "./interface/cWindowManager.h"
#include "./interface/cFont.h"
#include "ItemManager.h"
#include "ChatManager.h"



CExchangeItem::CExchangeItem()
{
	m_pLinkItem		= NULL;
	m_pLinkImage	= NULL;
	m_LinkPos		= -1;

	m_Durability	= 1;
	
	m_type			= WT_EXCHANGEITEM;

	m_dwParam		= 0;
}

CExchangeItem::~CExchangeItem()
{

}


void CExchangeItem::InitItem( CItem* pItem, POSTYPE Pos, LONG id )
{
	cImage tmpImage;
	Init( 0, 0, DEFAULT_ICONSIZE, DEFAULT_ICONSIZE,
		  ITEMMGR->GetIconImage( pItem->GetItemIdx(), &tmpImage ), id );

	m_pLinkItem = pItem;
	m_Position	= Pos;
	m_LinkPos	= pItem->GetPosition();

	m_pLinkImage = WINDOWMGR->GetImageOfFile("./Data/Interface/2DImage/GameIn/link.tif", 14);

	m_dwParam = pItem->GetItemParam();

	SetIconType( eIconType_LinkedItem );
	SetData( pItem->GetItemIdx() );
	SetDurability( pItem->GetDurability() );

	ITEMMGR->AddToolTip( this );
}


void CExchangeItem::SetLock( BOOL bLock )
{
	cIcon::SetLock( bLock );
	SetMovable( !bLock );
}

/*
void CExchangeItem::InitItem( const ITEMBASE* pItemBase, POSTYPE Pos, LONG id )
{
	Init( 0, 0, DEFAULT_ICONSIZE, DEFAULT_ICONSIZE,
		  ITEMMGR->GetIconImage( pItemBase->wIconIdx ), id );

//	m_pLinkItem = pItem;
	m_Position	= Pos;

	m_LinkPos	= pItemBase->Position;

	m_pLockImage = WINDOWMGR->GetImageOfFile( "./Data/Interface/2DImage/GameIn/98/9810110.tif", 98 );		

	SetIconType( eIconType_LinkedItem );
	SetData( pItemBase->wIconIdx );

	ITEMMGR->AddToolTip( this );
}
*/

void CExchangeItem::Render()
{

	/*
if( m_bLock )
		m_pLockImage->RenderSprite( NULL, NULL, 0, &m_absPos, 
									RGBA_MERGE(0xffffff, m_alpha * m_dwOptionAlpha / 100),
									 );
*/

//	cIcon::Render();

	if( m_bActive == FALSE ) return;
	//추가 : 링크표시 해주기

	// 071119 LYW --- ExchangeItem : 아이템 Parameter 값 수정.
	//if( m_dwParam & ITEM_PARAM_SEAL )
	if( m_dwParam == eITEM_PARAM_SEAL )
	{
		cDialog::Render();
		
		if( IsMovedWnd() )
		{
			m_rect.left		= (LONG)m_absPos.x+9 ;
			m_rect.top		= (LONG)m_absPos.y+15 ;
			m_rect.right	= 1 ;
			m_rect.bottom	= 1 ;
		}

		CFONT_OBJ->RenderFont(0,CHATMGR->GetChatMsg(773),strlen(CHATMGR->GetChatMsg(773)),&m_rect,RGBA_MAKE(250,250,128,255));
	}
	else
		cIcon::Render();


/*
		if( m_pLinkImage )
		{
			VECTOR2 pos = { m_absPos.x+1, m_absPos.y+29 };
			m_pLinkImage->RenderSprite(NULL, NULL, NULL, &pos, RGBA_MERGE(0xffffff, m_alpha * m_dwOptionAlpha / 100),0,RENDER_TYPE_DISABLE_TEX_FILTERING);
		}*/
	

	// 갯수가 있는 아이템인 경우...
	// 070424 LYW --- ExchangeItem : Modified check IsDupItem.
	//if( ITEMMGR->IsDupItem( (WORD)m_dwData ) )
	if( ITEMMGR->IsDupItem( m_dwData ) )
	{
		static char nums[3];
		wsprintf(nums,"%2d", m_Durability);
		//if( IsMovedWnd() )
		//{
		//	m_rect.left		= (LONG)m_absPos.x+27 ;
		//	m_rect.top		= (LONG)m_absPos.y+29 ;
		//	m_rect.right	= 1 ;
		//	m_rect.bottom	= 1 ;
		//}
		//CFONT_OBJ->RenderFont(0,nums,strlen(nums),&m_rect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));		// color hard coding : taiyo 

		if( IsMovedWnd() )
		{
			// 080205 LYW --- ExchangeItem : 3자리 수의 아이템일 때, 개수 좌표를 수정.
			if( m_Durability > 99 )
			{
				m_rect.left		= (LONG)m_absPos.x+16 ;
			}
			else
			{
				m_rect.left		= (LONG)m_absPos.x+22 ;
			}
			m_rect.top		= (LONG)m_absPos.y+23 ;
			m_rect.right	= 1 ;
			m_rect.bottom	= 1 ;
		}

		CFONT_OBJ->RenderFontShadow(0, 1, nums,strlen(nums),&m_rect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
	}

	if( IsMovedWnd() ) SetMovedWnd( FALSE ) ;
}

char* CExchangeItem::GetItemName()
{
	ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(GetData());
	if(pItemInfo)
		return pItemInfo->ItemName;
	return NULL;
}

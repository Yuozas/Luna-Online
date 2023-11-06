#include "stdafx.h"
#include "Item.h"
#include "ItemManager.h"
#include "ObjectManager.h"
#include "cQuickItem.h"
#include "GameIn.h"
#include "Interface/cFont.h"

#include "ChatManager.h"


CItem::CItem( const ITEMBASE* pInfo )
{
	m_type					= WT_ITEM;
	m_bAdded				= FALSE;
	m_ItemBaseInfo			= *pInfo;
	m_pLinkItem				= NULL;
	m_LinkPos				= 0;
	m_Param					= 0;
	
	m_bNoDeffenceTooltip = FALSE;
}


CItem::~CItem()
{}


/*
eITEM_KINDBIT CItem::GetItemKind()
{
	ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(m_ItemBaseInfo.wIconIdx);
	ASSERT(pInfo);	//KES 040512
	if(pInfo)
		return (eITEM_KINDBIT)pInfo->ItemKind;

	return (eITEM_KINDBIT)0;
}
*/

const ITEM_INFO* CItem::GetItemInfo()
{
	ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(m_ItemBaseInfo.wIconIdx);
	ASSERT(pInfo);
	if(pInfo)
	ASSERT(pInfo->ItemIdx == m_ItemBaseInfo.wIconIdx);
	return pInfo;
}


void CItem::SetItemBaseInfo( const ITEMBASE& itemInfo )
{
	m_ItemBaseInfo = itemInfo;
}


void CItem::Render()
{
	if( !m_bActive )
	{
		return;
	}
	if( m_ItemBaseInfo.nSealed == eITEM_TYPE_SEAL )
	{
		cDialog::Render();

		m_SealImage.RenderSprite( NULL, NULL, 0, &m_absPos, 
									RGBA_MERGE(0xffffff, 255));
		
		/*RECT rect={(LONG)m_absPos.x, (LONG)m_absPos.y+12, 1,1};
		CFONT_OBJ->RenderFont(0,"[봉인]", strlen("[봉인]"),&rect,RGBA_MAKE(250,250,128,255));*/
	}
	// 071124 LYW --- Item : 봉인 해제 아이템 시간 툴팁 처리.
	else if( m_ItemBaseInfo.nSealed == eITEM_TYPE_UNSEAL )
	{
		cDialog::Render();

		//ITEM_INFO* pItemInfo = NULL ;
		//pItemInfo = ITEMMGR->GetItemInfo( m_ItemBaseInfo.wIconIdx ) ;

		//if( pItemInfo )
		//{
		//	/*if( pItemInfo->nTimeKind == eKIND_PLAYTIME )
		//	{*/
		//		DWORD dwElapsedMill	= gCurTime - m_ItemBaseInfo.LastCheckTime ;
		//		DWORD dwElapsedSecond = dwElapsedMill / 1000 ;

		//		m_ItemBaseInfo.LastCheckTime = gCurTime - ( dwElapsedMill - dwElapsedSecond * 1000 ) ;

		//		if( m_ItemBaseInfo.nRemainSecond > (int)dwElapsedSecond )
		//		{
		//			m_ItemBaseInfo.nRemainSecond -= (int)dwElapsedSecond ;
		//		}
		//		else
		//		{
		//			m_ItemBaseInfo.nRemainSecond = 0 ;
		//		}
		//	//}
		//}

		DWORD dwElapsedMill	= gCurTime -m_ItemBaseInfo.LastCheckTime ;
		DWORD dwElapsedSecond = dwElapsedMill / 1000 ;

		SetLastCheckTime(gCurTime - ( dwElapsedMill - dwElapsedSecond * 1000 )) ;

		if( GetItemBaseInfo().nRemainSecond > (int)dwElapsedSecond )
		{
			int nRemainTime = GetItemBaseInfo().nRemainSecond - (int)dwElapsedSecond ;
			SetRemainTime(nRemainTime) ;
		}
		else
		{
			SetRemainTime(0) ;
		}

		// 080317 NYJ --- 전체외치기 아이템은 봉인해제후 사용시 쿨타임이 있으므로 Render()함수 호출한다.
		//                아래코드를 제거하면 Lock상태이미지와 쿨타임블라인드가 그리지 않는다.
		if(this->GetItemInfo()->SupplyType == ITEM_KIND_WORLDSHOUT)
			cIcon::Render();
	}
	else
	{
		cIcon::Render();
	}

	if( m_Param == 1 )
	{
		m_LockImage.RenderSprite( NULL, NULL, 0, &m_absPos, 
			RGBA_MERGE(0xffffff, m_alpha * m_dwOptionAlpha / 100));
	}

	if( m_bZeroCount )
	{
		m_ZeroImage.RenderSprite( NULL, NULL, 0, &m_absPos, 
									RGBA_MERGE(0xffffff, 200 * m_dwOptionAlpha / 100));
	}

	if( mIsVisibleDurability &&
		ITEMMGR->IsDupItem(m_ItemBaseInfo.wIconIdx) )
	{
		static char nums[3];
		wsprintf(nums,"%2d", m_ItemBaseInfo.Durability);
		//RECT rect={(LONG)m_absPos.x+23, (LONG)m_absPos.y+25, 1,1};
		//CFONT_OBJ->RenderFont(0,nums,strlen(nums),&rect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100 ));		// color hard coding : taiyo 
		RECT rect ;

		rect.top = (LONG)m_absPos.y+24 ;
		rect.right = 1 ;
		rect.bottom = 1 ;

		if( m_ItemBaseInfo.Durability > 99 )
		{
			rect.left = (LONG)m_absPos.x+16 ;
			
		}
		else
		{
			rect.left = (LONG)m_absPos.x+22 ;
		}
		CFONT_OBJ->RenderFontShadow(0, 1, nums,strlen(nums),&rect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100 ));		// color hard coding : taiyo 
	}
}

void CItem::SetLock( BOOL bLock )
{
	cIcon::SetLock( bLock );
	
	SetMovable( !bLock );
	//아이템 사용도 금지
}

BOOL CItem::CanMoveToDialog( cDialog* ptargetdlg )
{
/*	if( GetItemKind() == eQUEST_ITEM_EQUIP )
	{
		int type = ptargetdlg->GetType();
		switch( type )
		{
		case WT_STREETSTALLDIALOG:
		case WT_STREETBUYSTALLDIALOG:
		case WT_DEALDIALOG:
		case WT_GUILDWAREHOUSEDLG:
		case WT_QUESTTOTALDIALOG:
		case WT_ITEMSHOPDIALOG:
			return FALSE;
		}
	}
*/	return TRUE;
}

BOOL CItem::CanDelete()
{
//	if( GetItemKind() == eQUEST_ITEM_EQUIP )
//		return FALSE;
	return TRUE;
}




// 071124 LYW --- Item : 이벤트 함수 추가.
DWORD CItem::ActionEvent(CMouse* mouseInfo)
{
	DWORD we = WE_NULL;
	we |= cWindow::ActionEvent(mouseInfo);

	if( !m_bDisable )
		we |= ActionEventWindow(mouseInfo);

	we |= ActionEventComponent(mouseInfo);

	if( we == WE_MOUSEOVER )
	{
		if( m_ItemBaseInfo.nSealed == eITEM_TYPE_UNSEAL ) 
		{
			//const ITEM_OPTION* pOption = NULL ;
			//pOption = ITEMMGR->GetOption( GetItemIdx(), GetDurability() ) ;

			//const ITEM_INFO* pItemInfo = NULL ;
			//pItemInfo = ITEMMGR->GetItemInfo( m_ItemBaseInfo.wIconIdx ) ;

			//if( pItemInfo )
			//{
			//	/*if( pItemInfo->nTimeKind != eKIND_REALTIME )
			//	{*/
			//		ITEMMGR->AddTimeToolTip(((CItem*)this), pOption, FALSE) ;
			//	//}
			//}

			const ITEM_OPTION& pOption = ITEMMGR->GetOption( m_ItemBaseInfo );

			ITEMMGR->AddTimeToolTip( this, pOption, FALSE);
		}
	}

	return we ;
}

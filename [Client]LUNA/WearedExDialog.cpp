// WearedExDialog.cpp: implementation of the CWearedExDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WearedExDialog.h"

#include "ObjectManager.h"
#include "AppearanceManager.h"
#include "ItemManager.h"
#include "CharacterCalcManager.h"
#include "GameIn.h"

#include "CharacterDialog.h"

#include "ChatManager.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWearedExDialog::CWearedExDialog()
{
	m_type = WT_WEAREDDIALOG;
	m_nIconType = WT_ITEM;
}

CWearedExDialog::~CWearedExDialog()
{}


BOOL CWearedExDialog::AddItem(WORD relPos, cIcon * InIcon)
{	
	CHero* pHero = OBJECTMGR->GetHero();

	if( ! AddIcon(relPos, InIcon) )
	{
		return FALSE;
	}

	CItem * item = (CItem * )InIcon;

	pHero->SetWearedItemIdx(relPos, item->GetItemIdx());
	pHero->PassiveSkillCheckForWeareItem();
	pHero->SetCurComboNum(SKILL_COMBO_NUM);	// ¹«±â¸¦ ¹Ù²Ù¸é Ã¹¹øÂ° ÄÞº¸ºÎÅÍ ½ÃÀÛÇÏµµ·Ï

	APPEARANCEMGR->AddCharacterPartChange(pHero->GetID());
	
	CHARCALCMGR->AddItem( pHero, item->GetItemBaseInfo() );
	//CHARCALCMGR->Initialize( pHero );

	RefreshSetItem( item->GetItemBaseInfo() );

	RefreshCharacterInfo() ;

	//---KES 장비 방어력 적용 여부 071128
	RefreshArmorItem();
	//----------------------------------

	return TRUE;
}

BOOL CWearedExDialog::DeleteItem(WORD relPos, cIcon ** outIcon)
{
	CHero* pHero = OBJECTMGR->GetHero();
	//KES 031128 ¹«°ø¾ÆÀÌÅÛ ÀåÂø°¡´É ÅøÆÁ º¯°æ
	if( ! DeleteIcon(relPos, outIcon) )
	{
		return FALSE;
	}
	
	pHero->SetWearedItemIdx(relPos, 0);
	pHero->SetCurComboNum(SKILL_COMBO_NUM);	// ¹«±â¸¦ ¹Ù²Ù¸é Ã¹¹øÂ° ÄÞº¸ºÎÅÍ ½ÃÀÛÇÏµµ·Ï
	pHero->PassiveSkillCheckForWeareItem();

	APPEARANCEMGR->AddCharacterPartChange(pHero->GetID());
	
	CItem* item	= ( CItem* )( *outIcon );

	if( ! item )
	{
		return FALSE;
	}
	
	CHARCALCMGR->RemoveItem( pHero, item->GetItemBaseInfo() );
	//CHARCALCMGR->Initialize( pHero );

	RefreshSetItem( item->GetItemBaseInfo() );
	RefreshCharacterInfo() ;

//	GAMEIN->GetQuickDialog()->RefreshIcon();

	//---KES 장비 방어력 적용 여부 071128
	if( item )
	{
		item->SetImageRGB( ICONCLR_USABLE ); //빠져나가는 아이템은 색을 바꾼다. (다른 곳과 충돌될 땐 코드를 좀 수정해야겠지..)
		item->SetDeffenceTooltip( FALSE );
	}
	RefreshArmorItem();
	//----------------------------------

	return TRUE;
}

// 070411 LYW --- WearedExDialog : Add function to refresh character info dialog.
void CWearedExDialog::RefreshCharacterInfo()
{
	// 070411 LYW --- WearedExDialog : Delete function to setting attack rate & defense rate.
		/*
		GAMEIN->GetCharacterDialog()->SetAttackRate();
		GAMEIN->GetCharacterDialog()->SetDefenseRate();
		GAMEIN->GetCharacterDialog()->SetStrength();
		GAMEIN->GetCharacterDialog()->SetDexterity();
		GAMEIN->GetCharacterDialog()->SetVitality();
		GAMEIN->GetCharacterDialog()->SetWisdom();
        GAMEIN->GetCharacterDialog()->SetCritical();
		GAMEIN->GetCharacterDialog()->UpdateData();
		*/
		GAMEIN->GetCharacterDialog()->SetStrength();
		GAMEIN->GetCharacterDialog()->SetDexterity();
		GAMEIN->GetCharacterDialog()->SetVitality();
		GAMEIN->GetCharacterDialog()->SetIntelligence() ;
		GAMEIN->GetCharacterDialog()->SetWisdom();

		// 070411 LYW --- CharacterDialog : Add function to print values of physical part.
		GAMEIN->GetCharacterDialog()->PrintAttackRate() ;
		GAMEIN->GetCharacterDialog()->PrintDefenseRate() ;
		GAMEIN->GetCharacterDialog()->PrintMagAttackRate() ;
		GAMEIN->GetCharacterDialog()->PrintMagDefenseRate() ;
		GAMEIN->GetCharacterDialog()->PrintAccuracyRate() ;
		GAMEIN->GetCharacterDialog()->PrintEvasionRate() ;
		GAMEIN->GetCharacterDialog()->PrintCriticalRate() ;
}


void CWearedExDialog::RefreshSetItem( const ITEMBASE& item )
{
	const SetScript* script = ITEMMGR->GetSetScript( item.wIconIdx );

	if( ! script )
	{
		return;
	}

	const SetScript::Item&	itemSet = script->mItem;
	CHero*					hero	= OBJECTMGR->GetHero();

	for(POSTYPE part = TP_WEAR_START ; part < TP_WEAR_END ; ++part )
	{
		const ITEMBASE* itemBase = ITEMMGR->GetItemInfoAbsIn( hero, part );

		if( itemBase )
		{
			CItem* item = ITEMMGR->GetItem( itemBase->dwDBIdx );
			ASSERT( item );

			if( itemSet.end() != itemSet.find( item->GetItemIdx() ) )
			{
				ITEMMGR->RefreshItem( item );
			}
		}
	}
}

//---KES 방어력 적용 여부 071128
void CWearedExDialog::RefreshArmorItem()
{
	if( !HERO ) return;

	for( WORD i = 0 ; i < 16 ; ++i )	//아바타 빼고 16개.... 디파인 어디 되어 있나?
	{
		cIcon* pIcon = GetIconForIdx( i );
		if( !pIcon ) continue;

		if( pIcon->GetType() != WT_ITEM ) return;

		CItem* pItem = (CItem*)pIcon;

		if( HERO->IsNoDeffenceFromArmor() )
		{
			pItem->SetImageRGB( ICONCLR_DISABLE );
			pItem->SetDeffenceTooltip( TRUE );
		}
		else
		{
			pIcon->SetImageRGB( ICONCLR_USABLE );
			pItem->SetDeffenceTooltip( FALSE );
		}

        ITEMMGR->AddToolTip( pItem );
	}
}
//--------------------------------

// CharacterManager.cpp: implementation of the CCharacterManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CharacterManager.h"
#include "Player.h"
#include "mapdbmsgparser.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CCharacterManager);
CCharacterManager::CCharacterManager()
{

}

CCharacterManager::~CCharacterManager()
{

}

void CCharacterManager::ArrangeCharLevelPoint(CPlayer * pPlayer, WORD type)
{
	if(pPlayer->GetPlayerLevelUpPoint() <= 0)
	{
		pPlayer->SetPlayerLevelUpPoint(0);
		return;
	}

	pPlayer->SetPlayerLevelUpPoint(pPlayer->GetPlayerLevelUpPoint()-1);
	switch(type)
	{
	case STR_POINT:
		{
			pPlayer->AddStrength(1);
		}
		break;
	case WIS_POINT:
		{
			pPlayer->AddWisdom(1);
		}
		break;
	case DEX_POINT:
		{
			pPlayer->AddDexterity(1);
		}
		break;
	case VIT_POINT:
		{
			pPlayer->AddVitality(1);
		}
		break;
	// 070411 LYW --- CharacterManager : Add INT_POINT.
	case INT_POINT :
		{
			pPlayer->AddIntelligence(1) ;
		}
		break ;
	}

	InsertLogCharacter( pPlayer->GetID(), pPlayer->GetLevel(),
						pPlayer->GetHeroTotalInfo() );
}



BOOL CCharacterManager::ArrangeCharMinusPoint(CPlayer * pPlayer, WORD type)
{
//	if(pPlayer->GetShopItemStats()->StatePoint <= 0)
//	{
//		pPlayer->GetShopItemStats()->StatePoint = 0;
//		return FALSE;
//	}

	int point = 0;
	switch(type)
	{
	case STR_POINT:
		{
			point = pPlayer->GetRealStrength();// - /*어빌리티 삭제 - pPlayer->GetAbilityStats()->Str -*/ pPlayer->GetItemStats()->Str - pPlayer->GetShopItemStats()->Str - pPlayer->GetAvatarOption()->Str;
			if( point <= 12 )
				return FALSE;

			pPlayer->AddStrength(-1);
		}
		break;
	case WIS_POINT:
		{
			point = pPlayer->GetRealWisdom();// - /*어빌리티 삭제 - pPlayer->GetAbilityStats()->Wis -*/ pPlayer->GetItemStats()->Wis - pPlayer->GetShopItemStats()->Wis - pPlayer->GetAvatarOption()->Wis;
			if( point <= 12 )
				return FALSE;

			pPlayer->AddWisdom(-1);
		}
		break;
	case DEX_POINT:
		{
			point = pPlayer->GetRealDexterity();// - /*어빌리티 삭제 - pPlayer->GetAbilityStats()->Dex -*/ pPlayer->GetItemStats()->Dex - pPlayer->GetShopItemStats()->Dex - pPlayer->GetAvatarOption()->Dex;
			if( point <= 12 )
				return FALSE;

			pPlayer->AddDexterity(-1);
		}
		break;
	case VIT_POINT:
		{
			point = pPlayer->GetRealVitality();// - /*어빌리티 삭제 - pPlayer->GetAbilityStats()->Vit -*/ pPlayer->GetItemStats()->Vit - pPlayer->GetShopItemStats()->Vit - pPlayer->GetAvatarOption()->Vit;
			if( point <= 12 )
				return FALSE;

			pPlayer->AddVitality(-1);
		}
		break;
	case INT_POINT:
		{
			point = pPlayer->GetRealIntelligence();// - /*어빌리티 삭제 - pPlayer->GetAbilityStats()->Vit -*/ pPlayer->GetItemStats()->Vit - pPlayer->GetShopItemStats()->Vit - pPlayer->GetAvatarOption()->Vit;
			if( point <= 12 )
				return FALSE;

			pPlayer->AddIntelligence(-1);
		}
	}


//	--pPlayer->GetShopItemStats()->StatePoint;

//	if( pPlayer->GetShopItemStats()->StatePoint == 0 )
//	{		
//		pPlayer->GetShopItemManager()->CalcShopItemOption( eIncantation_StatePoint, FALSE );
//	}
//	else
//	{
//		ShopItemUpdatetimeToDB( pPlayer->GetID(), eIncantation_StatePoint, pPlayer->GetShopItemStats()->StatePoint );
//	}

	// 
	InsertLogCharacter( pPlayer->GetID(), pPlayer->GetLevel(),
						pPlayer->GetHeroTotalInfo() );

	return TRUE;
}

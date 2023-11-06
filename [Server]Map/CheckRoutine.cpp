// CheckRoutine.cpp: implementation of the CCheckRoutine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CheckRoutine.h"
#include "CharMove.h"
#include "Player.h"
#include "../[cc]skill/server/tree/SkillTree.h"
/*어빌리티 삭제 - #include "AbilityManager.h"*/
#include "ItemManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCheckRoutine::CCheckRoutine()
{

}

CCheckRoutine::~CCheckRoutine()
{

}
BOOL CCheckRoutine::StateOf(CPlayer * pPlayer, DWORD state)
{
	if(pPlayer->GetState() == state)
		return TRUE;
	else
		return FALSE;
}
BOOL CCheckRoutine::MoneyOf(CPlayer * pPlayer, eITEMTABLE tabIdx, MONEYTYPE moneyAmount)
{
	if(pPlayer->GetMoney(tabIdx) >= moneyAmount)
		return TRUE;
	else
		return FALSE;
}
BOOL CCheckRoutine::LevelOf(CPlayer * pPlayer, LEVELTYPE level)
{
	if(pPlayer->GetLevel() >= level)
		return TRUE;
	else
		return FALSE;
}
BOOL CCheckRoutine::DistanceOf(CPlayer * pPlayer, VECTOR3 *vTPos, DWORD dist)
{
	VECTOR3 * ObjectPos	= CCharMove::GetPosition((CObject *)pPlayer);
	if((DWORD)CalcDistanceXZ( ObjectPos, vTPos ) <= dist)
		return TRUE;
	else
		return FALSE;
}
/*어빌리티 삭제 - 
BOOL CCheckRoutine::AbilityOf(CPlayer * pPlayer, eABILITY_USE_JOB eKind)
{
	WORD abilIDX = 0;
	switch(eKind)
	{
	case eAUKJOB_Ungijosik:
		{
			abilIDX = ABILITYINDEX_UNGI;
		}
		break;
	case eAUKJOB_Showdown:
		{
			abilIDX = ABILITYINDEX_SHOWDOWN;
		}
		break;
	case eAUKJOB_Party:
		{
			abilIDX = ABILITYINDEX_PARTY;
		}
		break;
	case eAUKJOB_Guild:
		{
			abilIDX = ABILITYINDEX_GUILD;
		}
		break;
	case eAUKJOB_Deal:
		{
			abilIDX = ABILITYINDEX_EXCHANGE;
		}
		break;
	case eAUKJOB_StreetStall:
		{
			abilIDX = ABILITYINDEX_STALL;
		}
		break;
	case eAUKJOB_Upgrade:
		{
			abilIDX = ABILITYINDEX_ITEMUPGRADE;
		}
		break;
	case eAUKJOB_Mix:
		{
			abilIDX = ABILITYINDEX_ITEMMIX;
		}
		break;
	case eAUKJOB_Reinforce:
		{
			abilIDX = ABILITYINDEX_ITEMREINFORCE;
		}
		break;
	case eAUKJOB_Dissolve:
		{
			abilIDX = ABILITYINDEX_ITEMDISSOLVE;
		}
		break;
	case eAUKJOB_Restraint:
		{
			abilIDX = ABILITYINDEX_RESTRAINT;
		}
		break;
	default:
		{
			return FALSE;
		}
	}

	if(ABILITYMGR->CheckAbilityLevel(abilIDX, pPlayer->GetAbilityGroup(), 1))
		return TRUE;
	else
		return FALSE;
}
*/
// IsEmpty, IsPassword, IsLock락체크와는 다르다.
// 아이템 동일성 체크(위치,존재,아이템정보)
BOOL CCheckRoutine::ItemOf(CPlayer * pPlayer, POSTYPE AtPos, DWORD wItemIdx, DURTYPE Dura, POSTYPE QuickPos, WORD flag)
{
	const ITEMBASE * pItemBase = ITEMMGR->GetItemInfoAbsIn(pPlayer, AtPos);

	if(!pItemBase)
		return FALSE;
	if(flag & CB_EXIST && pItemBase->dwDBIdx == 0)
		return FALSE;
	if(flag & CB_ICONIDX && pItemBase->wIconIdx != wItemIdx)
		return FALSE;
	if(flag & CB_QABSPOS && pItemBase->QuickPosition != QuickPos)
		return FALSE;
	if(flag & CB_DURA && pItemBase->Durability != Dura)
		return FALSE;
	if(flag & CB_ENOUGHDURA && Dura > pItemBase->Durability)
		return FALSE;
	
	return TRUE;
}
BOOL CCheckRoutine::DearlerItemOf(WORD wDealerIdx, DWORD wBuyItemIdx)
{
	DealerData * pDealer = ITEMMGR->GetDealer(wDealerIdx);
	if(!pDealer)
		return FALSE;
	return pDealer->FindItem(wBuyItemIdx);
}
BOOL CCheckRoutine::SkillOf(CPlayer * pPlayer, DWORD wSkillIdx)
{
	if(pPlayer->GetSkillTree()->GetSkillBase(wSkillIdx))
		return TRUE;
	else
		return FALSE;
}
/*
BOOL CCheckRoutine::ItemStageOf(CPlayer* pPlayer, BYTE bItemStage )
{
	// stage
	if( bItemStage == eItemStage_Normal )		return TRUE;
	
	BYTE stage = pPlayer->GetStage();
	if( stage == eStage_Normal )				return FALSE;
	if( bItemStage == eItemStage_ChangeStage )	return TRUE;

	switch( stage )
	{
	case eStage_Hwa:
		{
			if( bItemStage == eItemStage_OverHwa || bItemStage == eItemStage_Hwa )
				return TRUE;
		}
		break;
	case eStage_Hyun:
		{
			if( bItemStage == eItemStage_ChangeStage2 || bItemStage == eItemStage_OverHwa || bItemStage == eItemStage_Hyun )
				return TRUE;
		}
		break;
	case eStage_Geuk:
		{
			if( bItemStage == eItemStage_OverGeuk || bItemStage == eItemStage_Geuk )
				return TRUE;
		}
		break;
	case eStage_Tal:
		{
			if( bItemStage == eItemStage_ChangeStage2 || bItemStage == eItemStage_OverGeuk || bItemStage == eItemStage_Tal )
				return TRUE;
		}
		break;
	}

	return FALSE;
}
*/

#include "stdafx.h"
#include ".\skilltree.h"
#include "../[cc]skill/server/manager/skillmanager.h"
#include "../[cc]skill/server/info/skillinfo.h"
#include "Player.h"
#include "CharacterCalcManager.h"

cSkillTree::cSkillTree(void)
{
	mSkillBasePool = new CMemoryPoolTempl< SKILL_BASE >;
	mSkillBasePool->Init( 20, 20, "SkillBasePool" );

	mSkillBaseTable.Initialize( 20 );
}

cSkillTree::~cSkillTree(void)
{
	Release();

	delete mSkillBasePool;
}

void cSkillTree::Init( CPlayer* pPlayer )
{
	mpPlayer = pPlayer;
}

void cSkillTree::Release()
{
	mSkillBaseTable.SetPositionHead();
	SKILL_BASE* pSkillBase = NULL;
	while( pSkillBase = mSkillBaseTable.GetData() )
	{
		mSkillBasePool->Free( pSkillBase );
	}
	mSkillBaseTable.RemoveAll();
}

SKILL_BASE* cSkillTree::NewSkillBase()
{
	return mSkillBasePool->Alloc();
}

void cSkillTree::AddSkillBase( SKILL_BASE* pSkillBase )
{
	SKILL_BASE* pOld = mSkillBaseTable.GetData( pSkillBase->wSkillIdx );

	if( pOld )
	{
		if( pOld->Level >= pSkillBase->Level )
		{
			return;
		}
	}

	{
		DWORD skillSize = SKILLMGR->GetSkillSize( pSkillBase->wSkillIdx );

		if( pSkillBase->Level > skillSize )
		{
			ASSERT( 0 );
			return;
		}
	}	

	mSkillBaseTable.Add( pSkillBase, pSkillBase->wSkillIdx );

	const DWORD index = pSkillBase->wSkillIdx - 1 + pSkillBase->Level;

	cSkillInfo* pInfo = SKILLMGR->GetSkillInfo( index );

	if( pInfo	&&
		pInfo->GetKind() == SKILLKIND_PASSIVE )
	{
		SKILLMGR->AddPassive( mpPlayer, index );
		CHARCALCMGR->CalcCharStats( mpPlayer );
	}
}


//BOOL cSkillTree::UpdateSkillBase( DWORD idx, BYTE level )
//{
//	SKILL_BASE* pSkillBase = mSkillBaseTable.GetData( idx );
//	const BYTE oldlevel = pSkillBase->Level;
//
//	if( oldlevel == level )
//	{
//		return FALSE;
//	}
//
//	pSkillBase->Level = level;
//
//	cSkillInfo* pInfo = SKILLMGR->GetSkillInfo( pSkillBase->wSkillIdx );
//
//	if( pInfo->GetKind() == SKILLKIND_PASSIVE )
//	{
//		DWORD index = ( ( pSkillBase->wSkillIdx / 100 ) * 100 ) + oldlevel;
//		SKILLMGR->RemovePassive( mpPlayer, index );
//		index = index - oldlevel + level;
//		SKILLMGR->AddPassive( mpPlayer, index );
//	}
//
//	CHARCALCMGR->CalcCharStats( mpPlayer );
//	return TRUE;
//}


SKILL_BASE* cSkillTree::GetSkillBase( DWORD SkillIdx )
{
	if( SkillIdx )
	{
		return mSkillBaseTable.GetData( SkillIdx );
	}
	else
	{
		return mSkillBaseTable.GetData();
	}
}


//void cSkillTree::RemoveSkillBase( DWORD skillIndex, BYTE level )
void cSkillTree::RemoveSkillBase( SKILL_BASE* skill )
{
	if( ! skill )
	{
		return;
	}

	const DWORD baseSkillIndex	= skill->wSkillIdx;
	const DWORD skillIndex		= baseSkillIndex - 1 + skill->Level;

	cSkillInfo* info = SKILLMGR->GetSkillInfo( skillIndex );

	if( info	&&
		info->GetKind() == SKILLKIND_PASSIVE )
	{
		SKILLMGR->RemovePassive( mpPlayer, skillIndex );
		CHARCALCMGR->CalcCharStats( mpPlayer );
	}

	mSkillBaseTable.Remove( baseSkillIndex );
	mSkillBasePool->Free( skill );
}


void cSkillTree::SetPositionHead()
{
	mSkillBaseTable.SetPositionHead();
}

// 071129 LYW --- SkillTree : 모든 스킬 삭제 함수 추가.
void cSkillTree::RemoveAllSkill()
{
	mSkillBaseTable.SetPositionHead();
	SKILL_BASE* pSkillBase = NULL;
	while( pSkillBase = mSkillBaseTable.GetData() )
	{
		mSkillBasePool->Free( pSkillBase );
	}
	mSkillBaseTable.RemoveAll();
}
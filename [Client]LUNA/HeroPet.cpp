#include "stdafx.h"
#include ".\heropet.h"
#include "ObjectManager.h"
#include "PetManager.h"
#include "MoveManager.h"
#include "GameIn.h"
#include "PetWearedDialog.h"
#include "PetInfoDialog.h"
#include "ItemManager.h"
#include "Item.h"
#include "../[cc]skill/client/info/ActiveSkillInfo.h"
#include "../[cc]skill/client/manager/skillmanager.h"
#include "ObjectGuagen.h"
#include "WindowIdEnum.h"
#include "cStatic.h"
#include "chatmanager.h"
#include "conductmanager.h"
#include "petstatedialog.h"
#include "petstatusicondialog.h"

CHeroPet::CHeroPet(void)
{
	mTargetID = 0;
	mSkillIndex = 0;
}

CHeroPet::~CHeroPet(void)
{
	if( GAMEIN->GetPetInfoDlg()->IsActive() )
	{	
		GAMEIN->GetPetInfoDlg()->SetActive( FALSE );
	}

	for( int i = 0; i < ePetEquipSlot_Max; i++ )
	{
		cIcon* pIcon = NULL;
		
		GAMEIN->GetPetWearedDlg()->DeleteItem( i, &pIcon );

		if( !pIcon )
		{
			continue;
		}

		ITEMMGR->ItemDelete( ( CItem* )pIcon );
	}

	Release();
}

void CHeroPet::InitPet( PET_OBJECT_INFO* pPetObjectInfo ) 
{
	CPet::InitPet( pPetObjectInfo );

	mItemDBIdx = pPetObjectInfo->ItemDBIdx;
	mSkillSlot = pPetObjectInfo->SkillSlot;
	mExp = pPetObjectInfo->Exp;
	mFriendly = pPetObjectInfo->Friendly;
	mAI = pPetObjectInfo->AI;
	mType = pPetObjectInfo->Type;
	mHP = pPetObjectInfo->HP;
	mMP = pPetObjectInfo->MP;
	mMaxHP = pPetObjectInfo->MAXHP;
	mMaxMP = pPetObjectInfo->MAXMP;

	memset( &mPetStat, 0, sizeof( PlayerStat ) );
	memset( &mItemStat, 0, sizeof( PlayerStat ) );
	
	memset( &mBuffStatus, 0, sizeof( Status ) );
	memset( &mRateBuffStatus, 0, sizeof( Status ) );
	memset( &mPassiveStatus, 0, sizeof( Status ) );
	memset( &mRatePassiveStatus, 0, sizeof( Status ) );
	memset( &mAbnormalStatus, 0, sizeof( AbnormalStatus ) );

	mIsSkillUse = 0;
	mSkillDelay = 0;
	mSkillDelayCheckTime = 0;
	mSkillIndex = 0;
	
	m_MovingAction.Clear();
	m_NextAction.Clear();

	mDistanceCheckTime = 0;

	PETMGR->SetLastSummonTime();
	GAMEIN->GetPetUIDlg()->SetActive( TRUE );
	SetExp( mExp );

	CItem* pItem = ITEMMGR->GetItem( mItemDBIdx );
	pItem->SetLock( TRUE );

	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1574) );

	WORD maxexp = PETMGR->GetExp( mGrade );
	
	cStatic* name = (cStatic *)GAMEIN->GetPetUIDlg()->GetWindowForID(PET_UI_NAME);
	if( name )	name->SetStaticText( GetObjectName() );	
	cStatic* level = (cStatic *)GAMEIN->GetPetUIDlg()->GetWindowForID(PET_UI_LEVEL);
	if( level )	level->SetStaticValue( mLevel );	

	SetMaxLife( mMaxHP );
	SetMaxMana( mMaxMP );
	SetExp( mExp );

	VECTOR2 sidpos;
	sidpos.x = 0;
	sidpos.y = 0;
	PETSTATUSICONDLG->Init(HERO,&sidpos,8);
}

void CHeroPet::Release()
{
	CItem* pItem = ITEMMGR->GetItem( mItemDBIdx );
	if( pItem ) pItem->SetLock( FALSE );

	CPet::Release();
	
	memset( &mPetStat, 0, sizeof( PlayerStat ) );
	memset( &mItemStat, 0, sizeof( PlayerStat ) );
	
	memset( &mBuffStatus, 0, sizeof( Status ) );
	memset( &mRateBuffStatus, 0, sizeof( Status ) );
	memset( &mPassiveStatus, 0, sizeof( Status ) );
	memset( &mRatePassiveStatus, 0, sizeof( Status ) );

	mIsSkillUse = 0;
	mSkillDelay = 0;
	mSkillDelayCheckTime = 0;

	mDistanceCheckTime = 0;
	
	mHP = 0;
	mMP = 0;

	GAMEIN->GetPetUIDlg()->SetActive( FALSE );
	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1575) );
	
	PETMGR->SetLastSummonTime();
	PETSTATUSICONDLG->Release();
}

void CHeroPet::CalcStats()
{
	memset( &mPetStat, 0, sizeof( PlayerStat ) );
	memset( &mItemStat, 0, sizeof( PlayerStat ) );	

	PET_STATUS_INFO* pInfo = PETMGR->GetPetStatusInfo( mType, mLevel );

	if( !pInfo )
		return;

	for( int i = 0; i < ePetEquipSlot_Max; i++ )
	{
		if( mWearedItem[ i ] == 0 )
			continue;

		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( mWearedItem[ i ] );

		mItemStat.mPhysicAttack.mPlus += pInfo->PhysicAttack;
		mItemStat.mPhysicDefense.mPlus += pInfo->PhysicDefense;
		mItemStat.mMagicAttack.mPlus += pInfo->MagicAttack;
		mItemStat.mMagicDefense.mPlus += pInfo->MagicDefense;
		mItemStat.mStrength.mPlus += pInfo->ImprovementStr;
		mItemStat.mDexterity.mPlus += pInfo->ImprovementDex;
		mItemStat.mVitality.mPlus += pInfo->ImprovementVit;
		mItemStat.mIntelligence.mPlus += pInfo->ImprovementInt;
		mItemStat.mWisdom.mPlus += pInfo->ImprovementWis;
		mItemStat.mLife.mPlus += pInfo->ImprovementLife;
		mItemStat.mMana.mPlus += pInfo->ImprovementMana;
	}
	
	mPetStat.mStrength.mPlus		= ( pInfo->STR + mItemStat.mStrength.mPlus + GetBuffStatus()->Str) * ( 1.0f + mItemStat.mStrength.mPercent );
	mPetStat.mDexterity.mPlus 		= ( pInfo->DEX + mItemStat.mDexterity.mPlus + GetBuffStatus()->Dex) * ( 1.0f + mItemStat.mDexterity.mPercent );
	mPetStat.mVitality.mPlus		= ( pInfo->VIT + mItemStat.mVitality.mPlus + GetBuffStatus()->Vit) * ( 1.0f + mItemStat.mVitality.mPercent );
	mPetStat.mIntelligence.mPlus	= ( pInfo->INT + mItemStat.mIntelligence.mPlus + GetBuffStatus()->Int) * ( 1.0f + mItemStat.mIntelligence.mPercent );
	mPetStat.mWisdom.mPlus			= ( pInfo->WIS + mItemStat.mWisdom.mPlus + GetBuffStatus()->Wis) * ( 1.0f + mItemStat.mWisdom.mPercent );

	{// 공격력
		DWORD wWeaponAttack		= DWORD( mItemStat.mPhysicAttack.mPlus * ( 1.0f + mItemStat.mPhysicAttack.mPercent ) );

		DWORD wEnchantAttack	= 0;
		DWORD wLevel			= GetLevel() ;
		float wPassiveSkill		= GetPassiveStatus()->PhysicAttack ;

		float fAttackA			= (float)( wWeaponAttack + wEnchantAttack + wLevel + wPassiveSkill + 3) ;
		float wStrengthAll		= mPetStat.mStrength.mPlus;

		wStrengthAll *= 0.2f;

		float wMasteryLevel		= 0 ;

		float wAttackBuffVal	= GetBuffStatus()->PhysicAttack ;

		float fAttackB			= 1000 + (((wStrengthAll -10) * 20) + (wMasteryLevel * 25) + (wAttackBuffVal * 10)) ;

		mPetStat.mPhysicAttack.mPlus = fAttackA * fAttackB / 400 ;
	}

	{// 방어력
		DWORD wDefenseAll		= DWORD( mItemStat.mPhysicDefense.mPlus * ( 1.0f + mItemStat.mPhysicDefense.mPercent ) );

		DWORD wEnchantAll		= 0 ;
		DWORD wLevel			= GetLevel() ;
		float wPassiveSkill		= GetPassiveStatus()->PhysicDefense; ;

		float fDefense			= (float)wDefenseAll / 2 ;
		float fDefenseA			= fDefense + wEnchantAll + wLevel + wPassiveSkill ;

		float wVitalityAll		= mPetStat.mVitality.mPlus;

		float wMasteryLevel		= 0 ;
		wVitalityAll *= 0.2f;

		float wDefenseBuffVal	= GetBuffStatus()->PhysicDefense ;

		float fValA				= (((float)wVitalityAll - 20) * 10) ;
		float fValB				= (float)(wMasteryLevel * 30) ;
		float fValC				= (float)(wDefenseBuffVal * 10) ;

		float fDefenseB			= 500 + (fValA + fValB + fValC) ;

		float fResult			= (fDefenseA * fDefenseB) / 200 ;

		if( fResult <= 0 )
		{
			mPetStat.mPhysicDefense.mPlus = 0 ;
		}
		else
		{
			mPetStat.mPhysicDefense.mPlus	= fResult ;
		}
	}

	{// 마법 공격력
		DWORD wMagicAttack		= DWORD( mItemStat.mMagicAttack.mPlus * ( 1.0f + mItemStat.mMagicAttack.mPercent ) );

		DWORD wEnchantAttack	= 0 ;
		DWORD wLevel			= GetLevel() ;
		float wPassiveSkill		= GetPassiveStatus()->MagicAttack;

		float fAttackA			= (float)(wMagicAttack + wEnchantAttack + wLevel + wPassiveSkill) ;

		float wIntelligenceAll	= mPetStat.mIntelligence.mPlus;

		wIntelligenceAll *= 0.2f;

		float wAttackBuffVal	= GetBuffStatus()->MagicAttack;

		float fValA				= ((float)wIntelligenceAll -10) * 35 ;
		float fValB				= (float)(wAttackBuffVal * 10) ;

		float fAttackB			= 1000 + (fValA + fValB) ;

		float fResult			= (fAttackA * fAttackB) / 250 ;

		mPetStat.mMagicAttack.mPlus	= fResult ;
	}

	{// 마법 방어력
		DWORD wMagDefenseAll		= DWORD( mItemStat.mMagicDefense.mPlus * ( 1.0f + mItemStat.mMagicDefense.mPercent ) );

		DWORD wEnchantAll		= 0 ;
		DWORD wLevel			= GetLevel() ;
		float wPassiveSkill		= GetPassiveStatus()->MagicDefense;

		float fDefense			= (float)wMagDefenseAll / 5 ;
		float fDefenseA			= fDefense + wEnchantAll + wLevel + wPassiveSkill ;

		float wWisdomAll		= mPetStat.mWisdom.mPlus;

		wWisdomAll *= 0.2f;

		float wDefenseBuffVal	= GetBuffStatus()->MagicDefense;

		float fValA				= ((float)wWisdomAll - 20) * 15 ;
		float fValB				= (float)wDefenseBuffVal * 10 ;

		float fDefenseB			= 500 + (fValA + fValB) ;

		float fResult			= (fDefenseA * fDefenseB) / 150 ;

		mPetStat.mMagicDefense.mPlus = fResult;
	}

	{// 명중
		DWORD wLevel			= GetLevel() ;

		float wDexterityAll	= mPetStat.mDexterity.mPlus;
		float wStrengthAll	= mPetStat.mStrength.mPlus;

		wDexterityAll *= 0.2f;
		wStrengthAll *= 0.2f;

		DWORD wClassOrderVal	= 0 ;
		float wPassiveSkill		= GetPassiveStatus()->Accuracy;
		float wBuffSkillVal		= GetBuffStatus()->Accuracy;

		float fValA				= ((float)wDexterityAll / 1.4f) ;
		float fValC				= ((float)wStrengthAll / 4) ;
		float fValB				= (float)(fValA + wLevel + wClassOrderVal + fValC);

		float wAccuracy			= (fValB) ;

		float dwTotalAccuracy	= wAccuracy + wPassiveSkill + wBuffSkillVal;

		{
			const PlayerStat::Value& value = mItemStat.mAccuracy;

			mPetStat.mAccuracy.mPlus = ( dwTotalAccuracy + value.mPlus ) * ( 1.0f + value.mPercent );
		}
	}

	{// 회피
		DWORD wLevel			= GetLevel() ;

		float wDexterityAll	= mPetStat.mDexterity.mPlus;

		wDexterityAll *= 0.2f;

		DWORD wClassOrderVal	= 0 ;
		float wPassiveSkill		= GetPassiveStatus()->Avoid ;
		float wBuffSkillVal		= GetBuffStatus()->Avoid ;

		float fValA				= ((float)wDexterityAll / 2) ;
		float fValB				= (float)(fValA + wLevel + wClassOrderVal);

		float wEvasion			= (fValB) ;

		float dwTotalEvasion	= wEvasion + wPassiveSkill + wBuffSkillVal;

		// 080313 LUJ, 아이템 스탯을 일반과 세트로 분리
		{
			const PlayerStat::Value& value = mItemStat.mEvade;

			mPetStat.mEvade.mPlus = ( dwTotalEvasion + value.mPlus ) * ( 1.0f + value.mPercent );
		}
	}

	GAMEIN->GetPetStateDlg()->SetInfo();
}

#define SKILL_PERSUIT_TICK	1000

void CHeroPet::Process()
{
	CObject::Process();
	
	static DWORD	dwSkillPersuitTime	= 0;

	float distance = 0.f;

	if( gCurTime - mDistanceCheckTime > PETMGR->GetDistanceCheckTime() )
	{
		VECTOR3 HeroPos;
		VECTOR3 PetPos;

		HERO->GetPosition(&HeroPos);
		GetPosition(&PetPos);
		distance = CalcDistanceXZ(&HeroPos, &PetPos); 

		mDistanceCheckTime = gCurTime;
	}

	if( distance > PETMGR->GetMaxDistance() )
	{
		SetMovingAction(NULL);
		SetNextAction(NULL);
		
		Recall();
		return;
	}

	if( mAI == ePetAI_Stand )
	{
		SetMovingAction(NULL);
		SetNextAction(NULL);

		return;
	}


	if(m_MovingAction.HasAction())
	{
		if(m_MovingAction.CheckTargetDistance(&GetCurPosition()) == TRUE)
		{
			MOVEMGR->HeroPetMoveStop();

			m_MovingAction.ExcutePetAction(this);
			m_MovingAction.Clear();

			return;
		}
		else
		{
			if( m_MovingAction.GetActionKind() == eActionKind_Skill )
			{
				if( gCurTime - dwSkillPersuitTime > SKILL_PERSUIT_TICK )
				{
					m_MovingAction.ExcutePetAction(this);
					dwSkillPersuitTime = gCurTime;
				
					return;
				}
			}
			else if( MOVEMGR->IsMoving(this) == FALSE )
			{
				m_MovingAction.Clear();
			}
		}
	}
	else
	{
		if(m_bIsAutoAttacking)
		{
			if(m_NextAction.HasAction() == FALSE)
			{
				if(SKILLMGR->OnPetSkillCommand(this,&m_AutoAttackTarget,FALSE) == FALSE)
				{
					DisableAutoAttack();
				}			
			}
			else
			{
				m_NextAction.ExcutePetAction(this);
				return;
			}
		}
	}


	switch( mAI )
	{
	case ePetAI_Aggressive:
		{
			if( !SetTarget() )
			{
				if( distance > PETMGR->GetAreaDistance() )
				{
					if( MOVEMGR->IsMoving( this ) )
					{
						MOVEMGR->HeroPetMoveStop();
					}

					MOVEMGR->SetHeroPetTarget();
				}		
			}
		}
		break;
	case ePetAI_Support:
		{
			if( distance > PETMGR->GetAreaDistance() )
			{
				if( MOVEMGR->IsMoving( this ) )
				{
					MOVEMGR->HeroPetMoveStop();
				}

				MOVEMGR->SetHeroPetTarget();
			}		
		}
		break;
	case ePetAI_Stand:
		{
		}
		break;
	}
/*
	CObject* pTarget = OBJECTMGR->GetObject( mTargetID );

	if( pTarget )
	{
		if( pTarget->GetState() == eObjectState_Die )
		{
			SetTarget( 0 );
			mIsSkillUse = FALSE;
			return;
		}
		
		if( mSkillDelay )
		{
			if( gCurTime - mSkillDelayCheckTime < mSkillDelay )
			{
				return;
			}

			mSkillDelay = 0;
			mSkillDelayCheckTime = 0;
			mIsSkillUse = 0;
		}
		
		VECTOR3 TargetPos;
		VECTOR3 PetPos;

		pTarget->GetPosition(&TargetPos);
		GetPosition(&PetPos);
		float distance = CalcDistanceXZ(&TargetPos, &PetPos); 

		DWORD skillidx;

		if( mSkillIndex )
		{
			skillidx = mSkillIndex;
		}
		else
		{
			switch( mType )
			{
			case ePetType_Basic:
			case ePetType_Physic:	skillidx = PETMGR->GetClosedSkillIndex();	break;
			case ePetType_Magic:
			case ePetType_Support:	skillidx = PETMGR->GetRangeSkillIndex();	break;
			}
		}

		cActiveSkillInfo* pSkillInfo = ( cActiveSkillInfo* )SKILLMGR->GetSkillInfo( skillidx );

		float range = pSkillInfo->GetSkillInfo()->Range;
		if( distance > range )
		{
			if( MOVEMGR->IsMoving( this ) )
			{
				return;
			}

			MOVEMGR->SetHeroPetTarget( &TargetPos );
			return;
		}

		if( MOVEMGR->IsMoving( this ) )
		{
			MOVEMGR->HeroPetMoveStop();
		}


//		if( mIsSkillUse )
//			return;

		if( mSkillIndex )
		{
			UseSkill();
		}
		else
		{
			Attack();
		}
		return;
	}

	if( mAI == ePetAI_Aggressive )
	{
		if( SetTarget() )
		{
			return;
		}
	}

	if( distance > PETMGR->GetAreaDistance() )
	{
		if( mAI == ePetAI_Stand )
		{
			return;
		}
		
		if( MOVEMGR->IsMoving( this ) )
		{
			MOVEMGR->HeroPetMoveStop();
		}

		MOVEMGR->SetHeroPetTarget();
	}
*/
}

void CHeroPet::Recall()
{
	MSGBASE msg;

	msg.Category = MP_PET;
	msg.Protocol = MP_PET_RECALL_SYN;
	msg.dwObjectID = HEROID;

	NETWORK->Send( &msg, sizeof( msg ) );
}

DWORD CHeroPet::SetTarget()
{
	VECTOR3 HeroPos;
	VECTOR3 PetPos;

	HERO->GetPosition(&HeroPos);
	GetPosition(&PetPos);

	DWORD id = OBJECTMGR->GetSingleTargetInRange( &PetPos, mpPetInfo->Sight, 0, 1 );

	if( id )
	{	
		CObject* pTargetObj = OBJECTMGR->GetObject( id );
		CActionTarget Target;
		Target.InitActionTarget(pTargetObj,NULL);

		EnableAutoAttack(&Target);		
		SKILLMGR->OnPetSkillCommand(this, &Target, FALSE);	

		
/*		MSG_DWORD msg;

		msg.Category = MP_PET;
		msg.Protocol = MP_PET_TARGET_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData = id;

		NETWORK->Send( &msg, sizeof( msg ) );
*/
		mTargetID = id;
	}
	return id;
}


void CHeroPet::SetTarget( DWORD id )
{
	CObject* pTargetObj = OBJECTMGR->GetObject( id );
	CActionTarget Target;
	Target.InitActionTarget(pTargetObj,NULL);

	EnableAutoAttack(&Target);		

	if( mAI != ePetAI_Stand )
		SKILLMGR->OnPetSkillCommand(this, &Target, FALSE);	

	
/*	MSG_DWORD msg;

	msg.Category = MP_PET;
	msg.Protocol = MP_PET_TARGET_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData = id;

	NETWORK->Send( &msg, sizeof( msg ) );
*/
	mTargetID = id;
}

void CHeroPet::Attack()
{
	mIsSkillUse = TRUE;

	MSGBASE msg;

	msg.Category = MP_PET;
	msg.Protocol = MP_PET_ATTACK_SYN;
	msg.dwObjectID = HEROID;

	NETWORK->Send( &msg, sizeof( msg ) );
}

void CHeroPet::UseSkill()
{
	mIsSkillUse = TRUE;

	MSG_DWORD msg;

	msg.Category = MP_PET;
	msg.Protocol = MP_PET_SKILL_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData = mSkillIndex;

	NETWORK->Send( &msg, sizeof( msg ) );

	mSkillIndex = 0;
}


void CHeroPet::AddExp()
{
	SetExp( mExp + 1 ); 
}
	
void CHeroPet::SetExp( WORD exp )
{
	WORD maxexp = PETMGR->GetExp( mGrade );

	((CObjectGuagen*)GAMEIN->GetPetUIDlg()->GetWindowForID(PET_UI_EXP_GUAGE))->SetValue(
		(float)exp/(float)maxexp, 0 );
	char szValue[50];
	sprintf( szValue, "%d/%d", exp, maxexp );
	cStatic* ps = (cStatic *)GAMEIN->GetPetUIDlg()->GetWindowForID(PET_UI_EXP_STATIC);
	if( ps )	ps->SetStaticText( szValue );

	mExp = exp;
}

void CHeroPet::SetWearedItem( ITEMBASE* pItemInfo )
{
	CPet::SetWearedItem( pItemInfo );

	for( int i = 0; i < ePetEquipSlot_Max; i++ )
	{
		if( pItemInfo[ i ].dwDBIdx == 0 )
		{
			continue;
		}

		CItem * newItem = ITEMMGR->MakeNewItem( &pItemInfo[ i ], "AddItem" );
		if(newItem == NULL)
		{
			continue;
		}

		GAMEIN->GetPetWearedDlg()->AddItem( i, ( cIcon* )newItem );
	}

	CalcStats();
}

void CHeroPet::SetMaxLife(DWORD maxlife)
{
	mMaxHP = maxlife;

	((CObjectGuagen*)GAMEIN->GetPetUIDlg()->GetWindowForID(PET_UI_LIFE_GUAGE))->SetValue(
		(float)mHP/(float)mMaxHP, 0 );

	char szValue[50];
	sprintf( szValue, "%d/%d", mHP, mMaxHP );

	cStatic* ps = (cStatic *)GAMEIN->GetPetUIDlg()->GetWindowForID(PET_UI_LIFE_STATIC);
	if( ps )	ps->SetStaticText( szValue );	
}

void CHeroPet::SetLife(DWORD life, BYTE type)
{
	if(GetState() == eObjectState_Die)
		life = 0;
	
	mHP = life;

	((CObjectGuagen*)GAMEIN->GetPetUIDlg()->GetWindowForID(PET_UI_LIFE_GUAGE))->SetValue(
		(float)mHP/(float)mMaxHP, 
		(type == 0 ? 0 : (2000/mMaxHP)*mHP) );

	char szValue[50];
	sprintf( szValue, "%d/%d", mHP, mMaxHP );
	cStatic* ps = (cStatic *)GAMEIN->GetPetUIDlg()->GetWindowForID(PET_UI_LIFE_STATIC);
	if( ps )	ps->SetStaticText( szValue );
}


void CHeroPet::SetMaxMana(DWORD val)
{
	mMaxMP = val;

	((CObjectGuagen*)GAMEIN->GetPetUIDlg()->GetWindowForID(PET_UI_MANA_GUAGE))->SetValue(
		(float)mMP/(float)mMaxMP, 0 );

	char szValue[50];
	sprintf( szValue, "%d/%d", mMP, mMaxMP );

	cStatic* ps = (cStatic *)GAMEIN->GetPetUIDlg()->GetWindowForID(PET_UI_MANA_STATIC);
	if( ps )	ps->SetStaticText( szValue );
}

void CHeroPet::SetMana(DWORD val, BYTE type)
{
	mMP = val;

	((CObjectGuagen*)GAMEIN->GetPetUIDlg()->GetWindowForID(PET_UI_MANA_GUAGE))->SetValue(
		(float)mMP/(float)mMaxMP, 
		( type == 0 ? 0 : (1000/mMaxMP)*mMP) );
	char szValue[50];
	sprintf( szValue, "%d/%d", mMP, mMaxMP );
	cStatic* ps = (cStatic *)GAMEIN->GetPetUIDlg()->GetWindowForID(PET_UI_MANA_STATIC);
	if( ps )	ps->SetStaticText( szValue );
}

void CHeroPet::SetLevel( BYTE level )	
{ 
	mLevel = level; 
	cStatic* plevel = (cStatic *)GAMEIN->GetPetUIDlg()->GetWindowForID(PET_UI_LEVEL);
	if( plevel )	plevel->SetStaticValue( mLevel );	
	SetExp( 0 ); 
}

void CHeroPet::LevelUp()	
{ 
	SetLevel( ++mLevel ); 
}

void CHeroPet::SetFriendly( BYTE byte ) 
{ 
	PET_FRIENDLY_STATE* pFriendlyState = PETMGR->GetFriendlyStateTable( byte );

	if( pFriendlyState )
	{
		BYTE rate = ( BYTE )random( 1, 100 );

		if( rate > pFriendlyState->SpeechRate )
			rate -= pFriendlyState->SpeechRate;
		else
		{
			cPtrList* pList = PETMGR->GetPetSpeechInfoTable( pFriendlyState->SpeechIndex );

			if( pList )
			{
				int count = pList->GetCount();
				int select = rand() % count;
				char* speech = NULL;

				PTRLISTPOS pos = pList->GetHeadPosition();
				while(pos)
				{
					speech = (char *)pList->GetNext(pos);
				}

				OBJECTMGR->ShowChatBalloon( this, speech );
			}
		}

		if( rate > pFriendlyState->EmoticonRate )
			rate -= pFriendlyState->EmoticonRate;
		else
		{
			cPtrList* pList = PETMGR->GetPetEmoticonInfoTable( pFriendlyState->EmoticonIndex );

			if( pList )
			{
				int count = pList->GetCount();
				int select = rand() % count;
				DWORD* emoticon = NULL;

				PTRLISTPOS pos = pList->GetHeadPosition();
				while(pos)
				{
					emoticon = (DWORD *)pList->GetNext(pos);
				}

				if( emoticon )
					CONDUCTMGR->PetEmoticonCommand( *emoticon );
			}

		}
	}

	mFriendly = byte; 
}

DWORD CHeroPet::GetRealSkillIndex( DWORD index )
{
	DWORD level = 0;

	for( BYTE slot = ePetEquipSlot_Skill1; slot <= ePetEquipSlot_Skill6; slot++ )
	{
		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( mWearedItem[ slot ] );

		if( pInfo )
		{
			DWORD skillindex = pInfo->SupplyValue;

			if( skillindex / 100 == index / 100 )
			{
				level += ( skillindex % 100 );
			}
		}
	}

	return ( index / 100 ) * 100 + level;
}

void CHeroPet::ClearTarget(CObject* pObject)
{
	if(m_AutoAttackTarget.GetTargetID() == pObject->GetID())
		DisableAutoAttack();
	if(m_NextAction.GetTargetID() == pObject->GetID())
	{
		m_NextAction.Clear();
	}
}

void CHeroPet::OnEndObjectState(BYTE State)
{
	switch(State)
	{
	case eObjectState_Enter:
		{
		}
		break;
	case eObjectState_Deal:
		{
		}
		break;

	case eObjectState_SkillUsing:
	case eObjectState_SkillBinding:
		SKILLMGR->OnPetComboTurningPoint(this);
		break;

	case eObjectState_SkillSyn:
		{
			SKILLMGR->OnPetComboTurningPoint(this);
		}
		break;

	case eObjectState_Move:
		{
		}
		break;
	case eObjectState_SkillStart:
		{
		}
		break;
	}
}


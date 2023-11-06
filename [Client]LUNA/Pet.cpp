#include "stdafx.h"
#include ".\pet.h"
#include "player.h"
#include "MOTIONDESC.h"
#include "PetManager.h"
#include "ObjectManager.h"
#include "MoveManager.h"

CPet::CPet(void)
{
}

CPet::~CPet(void)
{
}

void CPet::InitPet( PET_OBJECT_INFO* pPetObjectInfo ) 
{
	mID = pPetObjectInfo->ID;
	SafeStrCpy( mMasterName, pPetObjectInfo->MasterName, MAX_NAME_LENGTH + 1 );
	mKind = pPetObjectInfo->Kind;
	mGrade = pPetObjectInfo->Grade;
	mState = pPetObjectInfo->State;
	mLevel = pPetObjectInfo->Level;

	mpPetInfo = PETMGR->GetPetInfo( mKind );
}

void CPet::Release()
{
	CObject::Release();

	mpPetInfo = NULL;

	mID = 0;
	mMasterIdx = 0;
	mMasterName[0] = 0;
	mKind = 0;
	mGrade = 0;
	mState = 0;
	mLevel = 0;

	mpMaster = NULL;

	memset( mWearedItem, 0, sizeof( DWORD ) * ePetEquipSlot_Max );
}

float CPet::DoGetMoveSpeed()
{
	CHeroPet* hero = HEROPET;

	float Speed = RUNSPEED;
		
	if( GetMaster() )
		Speed = GetMaster()->DoGetMoveSpeed();

	if( this == HEROPET )
	{		
		// 080630 LUJ, 세트 아이템으로 획득된 이동 속도 보너스가 적용되도록 함
		float addrateval	= ( hero->GetRateBuffStatus()->MoveSpeed + hero->GetRatePassiveStatus()->MoveSpeed ) / 100.f + hero->GetItemStats().mMoveSpeed.mPercent;
		float addval		= hero->GetBuffStatus()->MoveSpeed + hero->GetPassiveStatus()->MoveSpeed + hero->GetItemStats().mMoveSpeed.mPlus;

		m_MoveInfo.AddedMoveSpeed = Speed * addrateval + addval;
	}

	Speed += m_MoveInfo.AddedMoveSpeed;

	return max( 1, Speed );
}


void CPet::SetMotionInState(BYTE State)
{
//	CheckTmpFunc();
	switch(State)
	{
	case eObjectState_Move:
		{			
			switch(m_MoveInfo.MoveMode)
			{
			case eMoveMode_Walk:
				{
					m_ObjectState.State_Start_Motion = PETMGR->GetWalkAnimation();
					m_ObjectState.State_Ing_Motion = PETMGR->GetWalkAnimation();
					m_ObjectState.State_End_Motion = PETMGR->GetWalkAnimation();
					m_ObjectState.MotionSpeedRate = 1.0f;
				}
				break;
			case eMoveMode_Run:
				{
					m_ObjectState.State_Start_Motion = PETMGR->GetRunAnimation();
					m_ObjectState.State_Ing_Motion = PETMGR->GetRunAnimation();
					m_ObjectState.State_End_Motion = PETMGR->GetRunAnimation();
					m_ObjectState.MotionSpeedRate = 1.0f;
				}
				break;
			}
			m_ObjectState.State_End_MotionTime = 0;
			
			ChangeMotion(m_ObjectState.State_Start_Motion,FALSE);
			if(m_ObjectState.State_Ing_Motion != -1)
				ChangeBaseMotion(m_ObjectState.State_Ing_Motion);
		}
		break;
	case eObjectState_None:
		{
			if(	GetObjectBattleState() )
			{
				m_ObjectState.State_Start_Motion = PETMGR->GetIdleAnimation();
				m_ObjectState.State_Ing_Motion = PETMGR->GetIdleAnimation();
				m_ObjectState.State_End_Motion = PETMGR->GetIdleAnimation();
				m_ObjectState.State_End_MotionTime = 0;
				m_ObjectState.MotionSpeedRate = 1.0f;
			}
			else
			{
				int nSubMotion = rand()%100;
				if(nSubMotion < m_pMotionDESC->GetStandRate())
					nSubMotion = eMonsterMotionIdle_Stand2;
				else
					nSubMotion = eMonsterMotionIdle_Stand;

				m_ObjectState.State_Start_Motion = PETMGR->GetIdleAnimation();;
				m_ObjectState.State_Ing_Motion = PETMGR->GetIdleAnimation();
				m_ObjectState.State_End_Motion = PETMGR->GetIdleAnimation();
				m_ObjectState.State_End_MotionTime = 0;
				m_ObjectState.MotionSpeedRate = 1.0f;
			}
		}
		break;
	default:
		{
			m_ObjectState.State_Start_Motion = -1;
			m_ObjectState.State_Ing_Motion = -1;
			m_ObjectState.State_End_Motion = -1;
			m_ObjectState.State_End_MotionTime = 0;
			m_ObjectState.MotionSpeedRate = 1.0f;
		}
		break;
	}
}

void CPet::SetWearedItem( ITEMBASE* pItemInfo )
{
	for( int i = 0; i < ePetEquipSlot_Max; i++ )
	{
		mWearedItem[ i ] = pItemInfo [ i ].wIconIdx;
	}
}

void CPet::Die(CObject* pAttacker,BOOL bFatalDamage,BOOL bCritical, BOOL bDecisive) 
{	
	if(pAttacker)
	{
		MOVEMGR->SetLookatPos(this,&pAttacker->GetCurPosition(),0,gCurTime);
	}

/*
	WORD weapon = GetWeaponAniType();

	WORD DieMotion = CHARACTER_MOTION[ eCharacterMotion_Die1 ][ weapon ];
	WORD DiedMotion = CHARACTER_MOTION[ eCharacterMotion_Died1 ][ weapon ];


	int DieEffect;
	if(bFatalDamage)
	{
		DieEffect = eEffect_Die2;	// 날라가서 죽기
	
		SetWingObjectBaseMotion( eWingMotion_Died1 );
		SetWingObjectMotion( eWingMotion_Die1 );
	
	}
	else
	{
		DieEffect = eEffect_Die1;	// 그냥 죽기

		SetWingObjectBaseMotion( eWingMotion_Died2 );
		SetWingObjectMotion( eWingMotion_Die2 );
	}

	TARGETSET set;
	set.pTarget = this;
	set.ResultInfo.bCritical = bCritical;
	set.ResultInfo.bDecisive = bDecisive;
	
	EFFECTMGR->StartEffectProcess(DieEffect,pAttacker,&set,1,GetID(),
							EFFECT_FLAG_DIE|EFFECT_FLAG_GENDERBASISTARGET);


	ChangeMotion( DieMotion, 0 );
	ChangeBaseMotion( DiedMotion );

	CObject::Die(pAttacker,bFatalDamage,bCritical,bDecisive);
*/
}

void CPet::Damage(CObject* pAttacker,BYTE DamageKind,DWORD Damage,BOOL bCritical, BOOL bDecisive)
{
	// desc_hseos_전투평화상태01
	// S 전투평화상태 주석처리 added by hseos 2007.08.02
	SetObjectBattleState( eObjectBattleState_Battle );
	if (pAttacker)
		pAttacker->SetObjectBattleState( eObjectBattleState_Battle );
	// E 전투평화상태 주석처리 added by hseos 2007.08.02

	if(Damage != 0)
	{
		if(GetState() == eObjectState_None)
		{
			if(pAttacker)
				MOVEMGR->SetLookatPos(this,&pAttacker->GetCurPosition(),0,gCurTime);
		}
		
		if(GetState() == eObjectState_None)
		{
			ChangeMotion(PETMGR->GetDamageAnimation(),FALSE);
		}
		
		int DamageEffect = -1;

		if( bCritical )
		{
			DamageEffect = eEffect_CriticalDamage;
		}
		else
		{
			DamageEffect = eEffect_NormalDamage;
		}

		if(DamageEffect != -1)
		{
			TARGETSET set;
			set.pTarget = this;
			set.ResultInfo.bCritical = bCritical;
			set.ResultInfo.bDecisive = bDecisive;
			
			EFFECTMGR->StartEffectProcess(DamageEffect,pAttacker,&set,1,GetID(),
											EFFECT_FLAG_GENDERBASISTARGET);

			if( bCritical )
			{
				TARGETSET set;
				set.pTarget = this;
				EFFECTMGR->StartEffectProcess(19,this,&set,0,GetID());
			}
		}
	}

	if( pAttacker == HERO || pAttacker == HEROPET || this == HEROPET )
	{
		EFFECTMGR->AddDamageNumber(Damage,pAttacker,this,DamageKind,bCritical,bDecisive);
	}

	// Life 셋팅
	DWORD life = GetLife();
	if(life < Damage)
		life = 0;
	else
		life = life - Damage;
	SetLife(life);
}

void CPet::Heal(CObject* pHealer,BYTE HealKind,DWORD HealVal)
{

	// desc_hseos_전투평화상태01
	// S 전투평화상태 주석처리 added by hseos 2007.08.02
	if( GetObjectBattleState() == eObjectBattleState_Battle )
	{
		if( pHealer )	
			pHealer->SetObjectBattleState( eObjectBattleState_Battle );
	}
	// E 전투평화상태 주석처리 added by hseos 2007.08.02

	DWORD life = GetLife() + HealVal;
	SetLife(life);
}

void CPet::Recharge(CObject* pHealer,BYTE RechargeKind,DWORD RechargeVal)
{
	// desc_hseos_전투평화상태01
	// S 전투평화상태 주석처리 added by hseos 2007.08.02
	if( GetObjectBattleState() == eObjectBattleState_Battle )
	{
		if( pHealer )
			pHealer->SetObjectBattleState( eObjectBattleState_Battle );
	}
	// E 전투평화상태 주석처리 added by hseos 2007.08.02

	DWORD mana = GetMana() + RechargeVal;
	SetMana(mana);
}

CPlayer* CPet::GetMaster()
{
	return ( CPlayer* )OBJECTMGR->GetObject( mMasterIdx );
}

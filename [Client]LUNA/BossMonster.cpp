// BossMonster.cpp: implementation of the CBossMonster class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BossMonster.h"
#include "GameResourceManager.h"
#include "ObjectManager.h"
#include "MoveManager.h"
#include "PartyManager.h"
#include "OptionManager.h"

#ifdef _TESTCLIENT_
#include "MHFile.h"
//#include "BossMonsterInfo.h"
#include "MoveManager.h"
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBossMonster::CBossMonster()
{
	m_bOpenning = FALSE;
}

CBossMonster::~CBossMonster()
{

}

void CBossMonster::Damage(CObject* pAttacker,BYTE DamageKind,DWORD Damage,BOOL bCritical, BOOL bDecisive)
{	
//	CheckTmpFunc();
	
	if(Damage != 0)
	{
	/*
		if(GetState() == eObjectState_None)
			{
				//if(pAttacker)
				//	MOVEMGR->SetLookatPos(this,&pAttacker->GetCurPosition(),0,gCurTime);
			}
			*/
/*	
		if(GetState() == eObjectState_None)
		{
			ChangeMotion(1,FALSE);
			switch(DamageKind) {
			case eDamageKind_Front:
				ChangeMotion(eMonsterMotion_DamageFront,FALSE);
				break;
			case eDamageKind_Left:
				ChangeMotion(eMonsterMotion_DamageLeft,FALSE);
				break;
			case eDamageKind_Right:
				ChangeMotion(eMonsterMotion_DamageRight,FALSE);
				break;
			}	
		}
*/		
		int DamageEffect = -1;
		//no damage ani..
/*
		switch(DamageKind) {
		case eDamageKind_Front:
		case eDamageKind_Counter:
		case eDamageKind_ContinueDamage:
			DamageEffect = GAMERESRCMNGR->GetMonsterListInfo(GetMonsterKind())->Damage1DramaNum;
			break;
		case eDamageKind_Left:
			DamageEffect = GAMERESRCMNGR->GetMonsterListInfo(GetMonsterKind())->Damage1DramaNum;
			break;
		case eDamageKind_Right:
			DamageEffect = GAMERESRCMNGR->GetMonsterListInfo(GetMonsterKind())->Damage1DramaNum;
			break;
		}*/

		// 080131 KTH -- Hit ���������� �����´�.
		DamageEffect = GAMERESRCMNGR->GetMonsterListInfo(GetMonsterKind())->Damage1DramaNum;

		if(DamageEffect != -1)
		{
			TARGETSET set;
			set.pTarget = this;
			set.ResultInfo.bCritical = bCritical;
			set.ResultInfo.bDecisive = bDecisive;
			
			EFFECTMGR->StartEffectProcess(DamageEffect,pAttacker,&set,1,GetID());
		}
	}

//	if( pAttacker )
//	if( pAttacker == HERO || PARTYMGR->IsPartyMember( pAttacker->GetID() ) )	//�ڽ��� ������ ��ȣ�� ǥ�� //KES 040801
//		EFFECTMGR->AddDamageNumber(Damage,pAttacker,this,DamageKind,bCritical);

	//SW050804 �ɼ�â ��Ƽ������ ���� �߰�
	if( pAttacker )
	{
		if( OPTIONMGR->IsShowMemberDamage() )
		{
			if( pAttacker == HERO || pAttacker == HEROPET || PARTYMGR->IsPartyMember( pAttacker->GetID() ) )
				EFFECTMGR->AddDamageNumber(Damage,pAttacker,this,DamageKind,bCritical,bDecisive);
		}
		else
		{
			if( pAttacker == HERO || pAttacker == HEROPET )
				EFFECTMGR->AddDamageNumber(Damage,pAttacker,this,DamageKind,bCritical,bDecisive);
		}

	}
	
	// Life ����
	DWORD life = GetLife();
	if(life < Damage)
		life = 0;
	else
		life = life - Damage;
	SetLife(life);
}

void CBossMonster::SetMotionInState(BYTE State)
{
	switch(State)
	{
	case eObjectState_Move:
		{
			switch(m_MoveInfo.KyungGongIdx)
			{
			case eMA_WALKAROUND:
				{
					m_ObjectState.State_Start_Motion = eBossMonsterMotion_Walk;
					m_ObjectState.State_Ing_Motion = eBossMonsterMotion_Walk;
					m_ObjectState.State_End_Motion = eBossMonsterMotion_Walk;
					m_ObjectState.State_End_MotionTime = 0;
					m_ObjectState.MotionSpeedRate = 1.0f;
				}
				break;
			case eMA_RUNAWAY:
				{
					m_ObjectState.State_Start_Motion = eBossMonsterMotion_Run1;
					m_ObjectState.State_Ing_Motion = eBossMonsterMotion_Run2;
					m_ObjectState.State_End_Motion = eBossMonsterMotion_Run3;
					m_ObjectState.State_End_MotionTime = 0;
					m_ObjectState.MotionSpeedRate = 1.0f;
				}
				break;
			case eMA_PERSUIT:
				{
					m_ObjectState.State_Start_Motion = eBossMonsterMotion_Run1;
					m_ObjectState.State_Ing_Motion = eBossMonsterMotion_Run2;
					m_ObjectState.State_End_Motion = eBossMonsterMotion_Run3;
					m_ObjectState.State_End_MotionTime = 0;
					m_ObjectState.MotionSpeedRate = 1.0f;
				}
				break;
			}
		}
		break;
	case eObjectState_None:
		{
			m_ObjectState.State_Start_Motion = eBossMonsterMotion_Stand;
			m_ObjectState.State_Ing_Motion = eBossMonsterMotion_Stand;
			m_ObjectState.State_End_Motion = eBossMonsterMotion_Stand;
			m_ObjectState.State_End_MotionTime = 0;
			m_ObjectState.MotionSpeedRate = 1.0f;
		}
		break;
	case eObjectState_Rest:
		{
			m_ObjectState.State_Start_Motion = -1;
			m_ObjectState.State_Ing_Motion = -1;
			m_ObjectState.State_End_Motion = -1;
			m_ObjectState.State_End_MotionTime = 0;
			m_ObjectState.MotionSpeedRate = 1.0f;
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
/*
void CBossMonster::AddStatus(CStatus* pStatus)
{
	//boss block status
}
*/

// DamageEffectUnit.cpp: implementation of the CDamageEffectUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EffectManager.h"
#include "DamageEffectUnit.h"
#include "DamageEffectUnitDesc.h"
#include "..\MoveManager.h"
#include "..\ObjectActionManager.h"

#include "..\Engine\EngineSound.h"
#include "..\ObjectManager.h"


//
#include "..\interface\cListDialog.h"
#include "..\GameIn.h"
//


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDamageEffectUnit::CDamageEffectUnit()
{
}

CDamageEffectUnit::~CDamageEffectUnit()
{
}

void CDamageEffectUnit::Initialize(CEffectUnitDesc* pEffectUnitDesc,EFFECTPARAM* pEffectParam)
{
	m_DamageOrder = NULL;

	CEffectUnit::Initialize(pEffectUnitDesc,pEffectParam);
}
void CDamageEffectUnit::Release()
{
	if(m_DamageOrder)
	{
		delete [] m_DamageOrder;
		m_DamageOrder = NULL;
	}
	
	CDamageEffectUnitDesc::ReleaseUnit(this);
}

void CDamageEffectUnit::SortDamageOrder(DWORD nTargetNum,TARGETSET* pTargetArray)
{

}

void CDamageEffectUnit::GetDamageNumberVelocity(VECTOR3* pRtVelocity,CObject* pObject,DWORD DamageKind)
{	
	pRtVelocity->y = 0;
	if(DamageKind == eDamageKind_Left)
	{
		pRtVelocity->x = -.1f;
		pRtVelocity->y = 0.8f;
		pRtVelocity->z = .3f;
	}
	else if(DamageKind == eDamageKind_Right)
	{
		pRtVelocity->x = 0.1f;
		pRtVelocity->y = 0.8f;
		pRtVelocity->z = 0.3f;
	}
	else if(DamageKind == eDamageKind_Front)
	{
		pRtVelocity->x = 0;
		pRtVelocity->y = 1.f;
		pRtVelocity->z = .3f;
	}
	else if(DamageKind == eDamageKind_Counter)
	{
		pRtVelocity->x = 0;
		pRtVelocity->y = 7.f;
		pRtVelocity->z = .2f;
	}
	else if(DamageKind == eDamageKind_ContinueDamage)
	{
		pRtVelocity->x = 0;
		pRtVelocity->y = .5f;
		pRtVelocity->z = .15f;
	}
	
	float angle = pObject->GetAngle();
	TransToRelatedCoordinate(pRtVelocity,pRtVelocity,angle);
}

void CDamageEffectUnit::InitEffect(DWORD nTargetNum,TARGETSET* pTargetArray)
{
	if(nTargetNum == 0)
		return;
	ASSERT(nTargetNum);
	
	m_nObjectNum = nTargetNum;

	SortDamageOrder(nTargetNum,pTargetArray);
	
	m_nCurTargetNum = 0;

	m_bInited = TRUE;
}

BOOL CDamageEffectUnit::On(EFFECTPARAM* pParam)
{
	if(m_bInited == FALSE)
		return FALSE;

	int objectnum;
	TARGETSET* pObject;
	GetObjectSet(pParam,&pObject,&objectnum);

	CDamageEffectUnitDesc* pDesc = (CDamageEffectUnitDesc*)m_pEffectUnitDesc;
	pParam->m_DamageRate += pDesc->m_DamageRate;

	for(int n=0;n<objectnum;++n)
	{
		if(pObject[n].pTarget->IsInited() == FALSE)	// 케릭터가 존재하지 않으면 return
			continue;
		if( pObject[n].pTarget == HEROPETBACKUP )
			continue;
		m_nCurTargetNum = n;
		DoOn(pParam,&pObject[n]);
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 주인공과 데미지 입는 메인타겟과 가까우면 조금씩 떨어트림
	if(pParam->m_dwFlag & EFFECT_FLAG_HEROATTACK)
	{
		DWORD MainTargetID = GetMainTargetID(pParam->GetMainTarget());
		if(MainTargetID && MainTargetID != HEROID)
		{
			CObject* pMainTarget = OBJECTMGR->GetObject(MainTargetID);
			if(pMainTarget)
			{
				HERO->OnHitTarget(pMainTarget);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	

	return TRUE;
}

DWORD CalcTotalDamage(DWORD CurTotal,float CurRate,float SumRate)
{
	float lateSum = SumRate - CurRate;
	float inverse = 1 / (1 - lateSum);
	return (DWORD)(CurTotal * inverse);
}

void CDamageEffectUnit::Damage(CObject* pVictim,CObject* pAttacker,RESULTINFO* pResultInfo,
							   float CurRate,float SumRate,BYTE DamageKind,BOOL bFatalDamage)
{
	DWORD LifeOriginalTotalDamage = CalcTotalDamage(pResultInfo->RealDamage,CurRate,SumRate);
	DWORD VampiricLifeOriginalTotal = CalcTotalDamage(pResultInfo->Vampiric_Life,CurRate,SumRate);
	DWORD VampiricManaOriginalTotal = CalcTotalDamage(pResultInfo->Vampiric_Mana,CurRate,SumRate);
	DWORD HealLifeOriginalTotal = CalcTotalDamage( pResultInfo->HealLife, CurRate, SumRate );
	DWORD LifeDamage;
	DWORD VampiricLife;
	DWORD VampiricMana;
	DWORD HealLife;
	
	if(SumRate >= 0.98f)
	{	// 마지막 공격이면 나머지 전부를 데미지로
#ifdef _DEBUG
		if(SumRate > 1)
		{
			// SumRate가 1이 넘는 것은 이펙트 스크립트에 실수가 있기 때문이다.
			// 데미지 이펙트를 바인딩하고 있는 트리거를 살펴보자. 이들 트리거가
			// 바인딩한 데미지 이펙트들의 Percent 수치를 모두 더한 값이 1을 
			// 넘으면 예외가 발생된다.
			char temp[256];
			sprintf(temp,"%s Damage Error",m_pEffectUnitDesc->m_EffFilename);
			ASSERTMSG(SumRate <= 1,temp);
		}
#endif

		LifeDamage = pResultInfo->RealDamage;
		VampiricLife = pResultInfo->Vampiric_Life;
		VampiricMana = pResultInfo->Vampiric_Mana;
		HealLife = pResultInfo->HealLife;

		// 마지막 공격이면 Stun 상태로 만든다.
		if(pResultInfo->StunTime)
		{
		}
	}
	else
	{
		LifeDamage = (DWORD)(LifeOriginalTotalDamage*CurRate);		// 자신의 비율대로..
		VampiricLife = (DWORD)(VampiricLifeOriginalTotal*CurRate);		// 자신의 비율대로..
		VampiricMana = (DWORD)(VampiricManaOriginalTotal*CurRate);		// 자신의 비율대로..
		HealLife = (DWORD)(HealLifeOriginalTotal*CurRate);
	}
	pResultInfo->RealDamage -= (TARGETDATA)LifeDamage;
	pResultInfo->Vampiric_Life -= (TARGETDATA)VampiricLifeOriginalTotal;
	pResultInfo->Vampiric_Mana -= (TARGETDATA)VampiricManaOriginalTotal;
	pResultInfo->HealLife -= (TARGETDATA)HealLife;

	if(pResultInfo->bDodge)
		OBJECTACTIONMGR->Dodge(pVictim,pAttacker);
	else if( HealLife )
	{
		OBJECTACTIONMGR->Heal(pVictim,pAttacker,DamageKind,HealLife);
	}
	else
	{
		OBJECTACTIONMGR->Damage(pVictim,pAttacker,DamageKind,LifeDamage,pResultInfo->bCritical,pResultInfo->bDecisive);
	}

	if(VampiricLife)
	{
		if(pAttacker->GetID() == HERO->GetID())
			HERO->ChangeLife(VampiricLife);
		else
			pAttacker->SetLife(pAttacker->GetLife()+VampiricLife);
	}
	if(VampiricMana)
	{
		pAttacker->SetMana(pAttacker->GetMana()+VampiricMana);
	}
	
	if(SumRate >= 0.98f)		// 데미지를 다 때렸다
	{
		if(pVictim->IsDied())		// 죽었다
		{
			OBJECTACTIONMGR->Die(pVictim,pAttacker,bFatalDamage,pResultInfo->bCritical,pResultInfo->bDecisive);	// 날라가서 죽기
		}
	}
}

void CDamageEffectUnit::DoOn(EFFECTPARAM* pParam,TARGETSET* pObject)
{
	CDamageEffectUnitDesc* pDesc = (CDamageEffectUnitDesc*)m_pEffectUnitDesc;
	Damage(pObject->pTarget, pParam->m_pOperator, &pObject->ResultInfo,
		pDesc->m_DamageRate, pParam->m_DamageRate, pDesc->m_DamageKind,
		pParam->m_dwFlag & EFFECT_FLAG_FATALDAMAGE);

	//////////////////////////////////////////////////////////////////////////
	// 반격 처리
	if(pObject->ResultInfo.CounterDamage)
	{
		RESULTINFO tempinfo;
		tempinfo.Clear();
		tempinfo.RealDamage = pObject->ResultInfo.CounterDamage;
		Damage(pParam->m_pOperator, pObject->pTarget, &tempinfo,
		pDesc->m_DamageRate, pParam->m_DamageRate, eDamageKind_Counter,
		FALSE);
	}
	//////////////////////////////////////////////////////////////////////////

}

void CDamageEffectUnit::DoProcess(DWORD tickTime,EFFECTPARAM* pParam)
{	
	if(m_bInited == FALSE)
		return;
	/*
	float FadeRate = 1;

	if(m_bFading)		// Fade시작..
	{
		if(tickTime >= m_FadeTime)
		{
			m_FadeTime = 0;
			FadeRate = 0;
			for(int n=0;n<m_nObjectNum;++n)
				m_DamageNumberArray[n].Clear();
		}
		else
		{
			m_FadeTime -= tickTime;
		}
		FadeRate = m_FadeTime / (float)pDesc->m_FadeDuration;
	}
	for(int n=0;n<m_nObjectNum;++n)
	{
		m_DamageNumberArray[n].SetAlpha(FadeRate);
	}
	*/
}

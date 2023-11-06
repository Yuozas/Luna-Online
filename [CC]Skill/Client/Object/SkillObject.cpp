#include "stdafx.h"
#include ".\skillobject.h"
#include "../Info/SkillInfo.h"
#include "../Info/ActiveSkillInfo.h"
#include "../Delay/Delay.h"
#include "../Delay/TimeDelay.h"
#include "../Manager/SkillManager.h"

#include "ActionTarget.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"

#include "GameIn.h"
#include "BattleSystem_Client.h"
#include "PartyManager.h"

#include "OptionManager.h"
#include "CharacterDialog.h"
#include "ObjectActionManager.h"
#include "ObjectManager.h"


cSkillObject::cSkillObject( cSkillInfo* pSkillInfo )
: mpSkillInfo( pSkillInfo )
{
}

cSkillObject::~cSkillObject(void)
{
}

void cSkillObject::Init( SKILLOBJECT_INFO* pInfo )
{
	m_bInited = TRUE;
	m_bDieFlag = FALSE;

	m_BaseObjectInfo.dwObjectID = pInfo->SkillObjectIdx;
	SafeStrCpy(m_BaseObjectInfo.ObjectName,mpSkillInfo->GetName(), MAX_NAME_LENGTH+1);
	m_BaseObjectInfo.ObjectState = eObjectState_None;

	mSkillObjectInfo.skillObjectId = pInfo->SkillObjectIdx;
	mSkillObjectInfo.level = pInfo->SkillLevel;

	mSkillObjectInfo.pSkillInfo = mpSkillInfo;
	
	mSkillObjectInfo.operatorId = pInfo->Operator;
	mSkillObjectInfo.pOperator = OBJECTMGR->GetObject( pInfo->Operator );
	memcpy( &( mSkillObjectInfo.mainTarget ), &( pInfo->MainTarget ), sizeof( MAINTARGET ) );
	
	mSkillObjectInfo.pos = pInfo->Pos;
	mSkillObjectInfo.skillDir = pInfo->Direction;

	mSkillObjectInfo.pBattle = BATTLESYSTEM->GetBattle( pInfo->BattleID );

	cActiveSkillInfo* pSkillInfo = ( cActiveSkillInfo* )mpSkillInfo;

	mOperatorEffect.Init( pSkillInfo->GetSkillInfo()->OperatorEffect, mSkillObjectInfo.pOperator, &( mSkillObjectInfo.mainTarget ) );
	mSkillEffect.Init( pSkillInfo->GetSkillInfo()->SkillEffect, mSkillObjectInfo.pOperator, &( mSkillObjectInfo.mainTarget ) );
	mTargetEffect.Init( pSkillInfo->GetSkillInfo()->TargetEffect, mSkillObjectInfo.pOperator, &( mSkillObjectInfo.mainTarget ) );

	DWORD index = pSkillInfo->GetIndex();
	BOOL bFatal = ( index / 100000 ) % 10;
	mOperatorEffect.StartEffect( bFatal, pInfo->Rate );

	if( mSkillObjectInfo.pOperator == HERO && pSkillInfo->GetSkillInfo()->DelayType != 0 )
	{
		DWORD time = pSkillInfo->GetSkillInfo()->CastingTime + pSkillInfo->GetSkillInfo()->DelayTime;

		if( time > 200 )
		{
			time -= 200;
			mTargetDelay.Init( time );
			mTargetDelay.Start();
		}
	}

	if( mSkillObjectInfo.pOperator == HEROPET )
	{
		HEROPET->SetCanSkillUse();
		HEROPET->SetSkillDelay( pSkillInfo->GetSkillInfo()->AnimationTime );
	}

	SetOperatorState();
}

void cSkillObject::ReleaseSkillObject()
{
}

void cSkillObject::Release()
{
}

void cSkillObject::OnReceiveSkillResult( MSG_SKILL_RESULT* pmsg )
{
	if( pmsg->SkillDamageKind )
	{
		CTargetListIterator iter( &pmsg->TargetList );
		CObject* pObject;
		RESULTINFO rinfo;

		while(iter.GetNextTarget())
		{
			pObject = OBJECTMGR->GetObject(iter.GetTargetID());
			iter.GetTargetData(&rinfo);
			if(pObject == NULL)
				continue;

			OBJECTACTIONMGR->ApplyResult(pObject,mSkillObjectInfo.pOperator,&rinfo,eDamageKind_Front);
		}

		iter.Release();
	}
	else
	{
		mOperatorEffect.SetTargetArray( mSkillObjectInfo.pOperator, &( pmsg->TargetList ), &( mSkillObjectInfo.mainTarget ) );
	}
}

DWORD cSkillObject::GetSkillIdx() 
{
	return mpSkillInfo->GetIndex();
}

void cSkillObject::SetPosition(VECTOR3* pPos)
{
}

void cSkillObject::GetPosition(VECTOR3* pPos)
{
}

void cSkillObject::SetAngle(float AngleRad)
{
}

float cSkillObject::GetAngle()
{
	return 0;
}

CObject* cSkillObject::GetOperator() 
{ 
	return mSkillObjectInfo.pOperator; 
}

cSkillInfo* cSkillObject::GetSkillInfo() 
{ 
	return mpSkillInfo;	
}

sSKILL_CREATE_INFO* cSkillObject::GetSkillObjectInfo() 
{
	return &mSkillObjectInfo;
}

void cSkillObject::SetOperatorState()
{
	CObject* pObject;
	DWORD StateEndTime = 0;

	pObject = OBJECTMGR->GetObject(mSkillObjectInfo.operatorId);
	if(pObject == NULL)
		return;
	
	if(pObject->GetState() == eObjectState_Die)
		return;

	float AttackSpeedRate = 1.0f;

	if( mpSkillInfo->GetKind() == SKILLKIND_PHYSIC ||
		mpSkillInfo->GetKind() == SKILLKIND_MAGIC )
	{
		DWORD idx = mpSkillInfo->GetIndex();

		cActiveSkillInfo* pSkillInfo = ( cActiveSkillInfo* )mpSkillInfo;

		if( (idx / 100000) % 10 )
		{
			if( pSkillInfo->GetSkillInfo()->Unit == UNITKIND_PHYSIC_ATTCK )
			{
				AttackSpeedRate += ( ( HERO->GetRatePassiveStatus()->PhysicSkillSpeedRate + HERO->GetRateBuffStatus()->PhysicSkillSpeedRate ) * 0.01f );
			}
			else if( pSkillInfo->GetSkillInfo()->Unit == UNITKIND_MAGIC_ATTCK )
			{
				AttackSpeedRate += ( ( HERO->GetRatePassiveStatus()->MagicSkillSpeedRate + HERO->GetRateBuffStatus()->MagicSkillSpeedRate ) * 0.01f );
			}
		}
		else
		{
			AttackSpeedRate += ( ( HERO->GetRatePassiveStatus()->NormalSpeedRate + HERO->GetRateBuffStatus()->NormalSpeedRate ) * 0.01f );
		}

		if( AttackSpeedRate < 0 )
		{
			AttackSpeedRate = 1.0f;
		}
	}
	StateEndTime = ( ( cActiveSkillInfo* )mpSkillInfo )->GetSkillInfo()->AnimationTime / AttackSpeedRate;
			
	if(pObject->GetState() != eObjectState_SkillSyn)
	{
		OBJECTSTATEMGR->StartObjectState(pObject,eObjectState_SkillSyn);
		OBJECTSTATEMGR->EndObjectState(pObject,eObjectState_SkillSyn,StateEndTime);
	}
}

void cSkillObject::EndOperatorState()
{
	CObject* pObject;
	BYTE State;

	pObject = OBJECTMGR->GetObject(mSkillObjectInfo.operatorId);
	if(pObject == NULL)
		return;

	OBJECTSTATEMGR->StartObjectState(pObject,eObjectState_None);
}

void cSkillObject::ResetSkillObjectId( DWORD id )
{
	mSkillObjectInfo.skillObjectId = id;
	m_BaseObjectInfo.dwObjectID = id;
}

void cSkillObject::Update()
{
	if( mTargetDelay.IsStart() )
	{
		if( !mTargetDelay.Check() )
		{
			UpdateTarget();
		}
	}
}

void cSkillObject::UpdateTarget()
{
	CActionTarget Target;
	cActiveSkillInfo* pSkillInfo = ( cActiveSkillInfo* )mpSkillInfo;

	// 080804 LUJ, ��Ƽ�� ��� Ÿ�� ������Ʈ�� ���������� ���� ���Ѵ�. Ŭ���̾�Ʈ������
	//				���� ��ų�� ��� �ֱ������� Ÿ�� ���縦 �ؼ� ������ ������Ʈ�Ѵ�.
	//				�׷��� ��Ƽ ��ų�� �Ҽ� ���θ� �˻��ϰ�, ���۷����Ϳ� Ÿ���� ����
	//				��Ƽ�� �ִ��� �˻����� �ʴ´�. �׷��� Ÿ�� ������Ʈ�� �ϸ� Ÿ����
	//				�߸��Ǵ� ��찡 �߻��Ѵ�. ���� ��Ƽ�� ��� Ÿ�� �˻縦 �ƿ�
	//				���� �ʵ��� �Ѵ�. �׷��ٰ� Ÿ���� ���� ������ ������, ���� ������
	//				Ÿ���� �ʱ�ȭ�ȴ�. �ƿ� ó������ �ʾƾ� �Ѵ�
	if( !	pSkillInfo	||
			pSkillInfo->GetSkillInfo()->AreaTarget == eSkillAreaTargetParty )
	{
		return;
	}
	else if(TARGET_KIND_MULTI != pSkillInfo->GetSkillInfo()->Area)
	{
		return;
	}
	// 100111 LUJ, �ڽ��� ������ ��ų�� ó���ؾ��Ѵ�. �׷��� ������, ���Ͱ� �߻���Ų ���� ��ų ������� �ٲ��
	else if(mSkillObjectInfo.operatorId != gHeroID)
	{
		return;
	}

	Target.InitActionTarget( &mSkillObjectInfo.pos, NULL );
/*
	VECTOR3* pTargetPos = Target.GetTargetPosition();
	if(pTargetPos == NULL)
	{
		pTargetPos = &mSkillObjectInfo.pos;
	}

	switch( pSkillInfo->GetSkillInfo()->Area )
	{
	case 1:
		{
			Target.SetTargetObjectsInRange( pTargetPos, pSkillInfo->GetSkillInfo()->AreaData, pSkillInfo->GetSkillInfo()->AreaTarget );
		}
		break;
	}
*/
	VECTOR3* const pTargetPos = Target.GetTargetPosition();
	Target.SetTargetObjectsInRange(
		pTargetPos == NULL ? &mSkillObjectInfo.pos : pTargetPos,
		pSkillInfo->GetSkillInfo()->AreaData,
		WORD(pSkillInfo->GetSkillInfo()->AreaTarget));

	MSG_SKILL_UPDATE_TARGET msg;
	ZeroMemory(
		&msg,
		sizeof(msg));

	//MSG_SKILL_UPDATE_TARGET msg;

	msg.Category = MP_SKILL;
	msg.Protocol = MP_SKILL_UPDATE_TARGET_SYN;
	msg.dwObjectID = HEROID;
	msg.SkillObjectID = mSkillObjectInfo.skillObjectId;
	msg.Range = pSkillInfo->GetSkillInfo()->AreaData;
	msg.IDx = pSkillInfo->GetSkillInfo()->Index;
	Target.GetTargetArray(&msg.TargetList);

	NETWORK->Send(&msg,msg.GetMsgLength());
}

void cSkillObject::Cancel()
{
	mOperatorEffect.EndEffect();
	EndOperatorState();
}

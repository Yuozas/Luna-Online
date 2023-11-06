#include "stdafx.h"
#include ".\buffskillobject.h"

#include "../info/buffskillinfo.h"

#include "Object.h"
#include "ObjectManager.h"
#include "BattleSystem_Client.h"
#include "ObjectActionManager.h"
#include "StatusIconDlg.h"
#include "PetStatusIconDialog.h"


cBuffSkillObject::cBuffSkillObject( cSkillInfo* pSkillInfo )
: cSkillObject( pSkillInfo )
// 080318 LUJ, 설정 시각 초기화
, mBeginTime( 0 )
{
}

cBuffSkillObject::~cBuffSkillObject(void)
{
}

void cBuffSkillObject::Init( SKILLOBJECT_INFO* pInfo )
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
	mSkillObjectInfo.remainTime = pInfo->RemainTime;

	CObject* pObject = OBJECTMGR->GetObject( mSkillObjectInfo.mainTarget.dwMainTargetID );
/*
	if( HERO == pObject )
	{
		WORD kind = ( ( cBuffSkillInfo* )mpSkillInfo )->GetSkillInfo()->Status;
		DWORD data1 = ( ( cBuffSkillInfo* )mpSkillInfo )->GetSkillInfo()->Data[ 0 ];
		DWORD data2 = ( ( cBuffSkillInfo* )mpSkillInfo )->GetSkillInfo()->Data[ 1 ];
		DWORD data3 = ( ( cBuffSkillInfo* )mpSkillInfo )->GetSkillInfo()->Data[ 2 ];

		mStatus.Init( kind, data1, data2, data3 );

		HERO->AddStatus( &mStatus );
	}
*/
	if( HERO == pObject )
	{
		( ( cBuffSkillInfo* )mpSkillInfo )->AddBuffStatus();

		if( mpSkillInfo->GetImage() )
		{
			DWORD time = 0;

			if( mSkillObjectInfo.remainTime )
			{
				time = mSkillObjectInfo.remainTime + gCurTime;
			}
			else if( ( ( cBuffSkillInfo* )mpSkillInfo )->GetSkillInfo()->DelayTime > 0 )
			{
				time = ( ( cBuffSkillInfo* )mpSkillInfo )->GetSkillInfo()->DelayTime + gCurTime;
			}

			HERO->AddBuffSkillObject( this );
			STATUSICONDLG->AddSkillIcon( GetID(), mpSkillInfo->GetIndex(), time, pInfo->Count );
		}
	}
	if( HEROPET == pObject )
	{
		( ( cBuffSkillInfo* )mpSkillInfo )->AddPetBuffStatus();

		if( mpSkillInfo->GetImage() )
		{
			DWORD time = 0;

			if( mSkillObjectInfo.remainTime )
			{
				time = mSkillObjectInfo.remainTime + gCurTime;
			}
			else if( ( ( cBuffSkillInfo* )mpSkillInfo )->GetSkillInfo()->DelayTime > 0 )
			{
				time = ( ( cBuffSkillInfo* )mpSkillInfo )->GetSkillInfo()->DelayTime + gCurTime;
			}

			//HEROPET->AddBuffSkillObject( this );
			PETSTATUSICONDLG->AddSkillIcon( GetID(), mpSkillInfo->GetIndex(), time, pInfo->Count );
		}
	}

	cBuffSkillInfo* pSkillInfo = ( cBuffSkillInfo* )mpSkillInfo;
	mSkillEffect.Init( pSkillInfo->GetSkillInfo()->SkillEffect, mSkillObjectInfo.pOperator, &( mSkillObjectInfo.mainTarget ) );

	mSkillEffect.StartTargetEffect( mSkillObjectInfo.skillObjectId );

	// 080318 LUJ, 설정된 시각 저장
	mBeginTime = gCurTime;
}

void cBuffSkillObject::ReleaseSkillObject()
{
	CObject* pObject = OBJECTMGR->GetObject( mSkillObjectInfo.mainTarget.dwMainTargetID );

	// 080715 LUJ, 오브젝트가 있을 경우에만 처리하도록 한다
	if( pObject )
	{
		if( HERO == pObject )
		{
			( ( cBuffSkillInfo* )mpSkillInfo )->RemoveBuffStatus();
			HERO->RemoveBuffSkillObject( GetSkillIdx() / 100 );
			STATUSICONDLG->RemoveSkillIcon( GetID(), mpSkillInfo->GetIndex() );
		}
		else if( HEROPET == pObject )
		{
			( ( cBuffSkillInfo* )mpSkillInfo )->RemovePetBuffStatus();
			//HEROPET->RemoveBuffSkillObject( GetSkillIdx() / 100 );
			PETSTATUSICONDLG->RemoveSkillIcon( GetID(), mpSkillInfo->GetIndex() );
		}
	}

	mSkillEffect.EndTargetEffect();
}

void cBuffSkillObject::Release()
{
}

void cBuffSkillObject::OnReceiveSkillResult( MSG_SKILL_RESULT* pmsg )
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
		mSkillEffect.SetTargetArray( mSkillObjectInfo.pOperator, &( pmsg->TargetList ), &( mSkillObjectInfo.mainTarget ) );
	}
}

void cBuffSkillObject::AddStatusUnit( cStatus* pStatus )
{
}

void cBuffSkillObject::ResetDelay()
{
	CObject* pObject = OBJECTMGR->GetObject( mSkillObjectInfo.mainTarget.dwMainTargetID );

	if( HERO == pObject )
	{
		DWORD time = ( ( cBuffSkillInfo* )mpSkillInfo )->GetSkillInfo()->DelayTime + gCurTime;
		STATUSICONDLG->UpdateSkillTime( GetID(), mpSkillInfo->GetIndex(), time );
	}

	mSkillEffect.EndTargetEffect();
	mSkillEffect.StartTargetEffect( mSkillObjectInfo.skillObjectId );
}

void cBuffSkillObject::SetCount( WORD count )
{
	CObject* pObject = OBJECTMGR->GetObject( mSkillObjectInfo.mainTarget.dwMainTargetID );

	if( HERO == pObject )
	{
		STATUSICONDLG->UpdateSkillCount( GetID(), mpSkillInfo->GetIndex(), count );
	}
}

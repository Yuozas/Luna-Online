#include "stdafx.h"
#include ".\skillobject.h"
#include "../Info/SkillInfo.h"
#include "../Delay/Delay.h"
#include "../Unit/SkillUnit.h"
#include "../Target/Target.h"
#include "../Target/MultiTarget.h"
#include "Battle.h"
#include "..\Info\ActiveSkillInfo.h"
#include "UserTable.h"

#include "Object.h"
#include "Player.h"
#include "Monster.h"


cSkillObject::cSkillObject( cSkillInfo* pSkillInfo, cTarget* pTarget )
: mpSkillInfo( pSkillInfo ), mpTarget( pTarget )
{
	// 080602 LUJ, 오브젝트 타입을 초기화한다
	mType = cSkillObject::TypeNone;
}

cSkillObject::~cSkillObject(void)
{
	if( mpTarget )
	{
		delete mpTarget;
	}
}

cSkillInfo* cSkillObject::GetSkillInfo()
{
	return mpSkillInfo;
}

DWORD cSkillObject::GetSkillIdx()
{
	return mpSkillInfo->GetIndex();
}

WORD cSkillObject::GetSkillKind()
{
	return mpSkillInfo->GetKind();
}

WORD cSkillObject::GetSkillLevel()
{
	return mpSkillInfo->GetLevel();
}

CObject* cSkillObject::GetOperator()
{
	return g_pUserTable->FindUser( mSkillObjectInfo.operatorId );
//	return mSkillObjectInfo.pOperator;
}

inline void cSkillObject::GetPosition( VECTOR3* pPos )
{
	*pPos = mSkillObjectInfo.pos;
}

inline DIRINDEX cSkillObject::GetDirectionIndex()
{
	return mSkillObjectInfo.skillDir;
}

void cSkillObject::Init( sSKILL_CREATE_INFO* pInfo )
{
	/// 스킬 생성 정보 설정
	memcpy(	&mSkillObjectInfo, pInfo, sizeof( sSKILL_CREATE_INFO ) );

	/// 기본 정보 셋팅
	m_BaseObjectInfo.dwObjectID = pInfo->skillObjectId;
	strcpy(m_BaseObjectInfo.ObjectName,"SkillObject");
	m_BaseObjectInfo.ObjectState = eObjectState_None;

	/// 사용자 정보가 잘못되었을 경우 실패
	if( g_pUserTable->FindUser( pInfo->operatorId ) == NULL )
	{
		return;
	}
	/// 타겟 설정
	mpTarget->Init( this, &( pInfo->mainTarget ) );
}

void cSkillObject::Release()
{
}

SKILL_STATE cSkillObject::Update()
{
	return mState;
}

void cSkillObject::EndState()
{
	mState = SKILL_STATE_DESTROY;
	GetOperator()->CurCastingSkillID = 0;

	if( mpTarget->GetKind() < TARGET_KIND_MULTI )
	{
		return;
	}

	( ( cMultiTarget* )mpTarget )->RemoveAll();
}

void cSkillObject::UpdateTargetList( MAINTARGET* pTarget )
{
}

void cSkillObject::SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin)
{
	if( bLogin )
	{
		*pMsgLen = 0;
		return;
	}

	MSG_SKILLOBJECT_ADD2* pMsg = ( MSG_SKILLOBJECT_ADD2* )pAddMsg;
	
	pMsg->Category = MP_SKILL;
	pMsg->Protocol = MP_SKILL_SKILLOBJECT_ADD;
	pMsg->bCreate = FALSE;

	pMsg->SkillObjectInfo.SkillObjectIdx = mSkillObjectInfo.skillObjectId;
	pMsg->SkillObjectInfo.SkillIdx = mpSkillInfo->GetIndex();

	pMsg->SkillObjectInfo.Pos = mSkillObjectInfo.pos;
	pMsg->SkillObjectInfo.StartTime = 0;
	pMsg->SkillObjectInfo.Direction = mSkillObjectInfo.skillDir;
	pMsg->SkillObjectInfo.SkillLevel = ( BYTE )mSkillObjectInfo.level;
	pMsg->SkillObjectInfo.Operator = mSkillObjectInfo.operatorId;
	pMsg->SkillObjectInfo.Rate = 1.0f;
	pMsg->SkillObjectInfo.RemainTime = 0;

	memcpy( &( pMsg->SkillObjectInfo.MainTarget ), &( mSkillObjectInfo.mainTarget ), sizeof( MAINTARGET ) );

	pMsg->SkillObjectInfo.BattleID = GetBattleID();
   
	*pMsgLen = sizeof( MSG_SKILLOBJECT_ADD2 );
}

void cSkillObject::SetRemoveMsg(char* pRemoveMsg,WORD* pMsgLen,DWORD dwReceiverID)
{
	MSG_DWORD* pMsg = ( MSG_DWORD* )pRemoveMsg;

	pMsg->Category = MP_SKILL;
	pMsg->Protocol = MP_SKILL_SKILLOBJECT_REMOVE;
	pMsg->dwObjectID = dwReceiverID;
	pMsg->dwData = GetID();

	*pMsgLen = sizeof( MSG_DWORD );
}

void cSkillObject::SetMultiTarget( CTargetList* pTList , WORD AreaData)
{
	if( mpTarget->GetKind() < TARGET_KIND_MULTI )
	{
		return;
	}
	WORD skillInfo = ( ( cActiveSkillInfo* )mpSkillInfo )->GetSkillInfo()->AreaData;
	const float validArea = skillInfo;
	if (AreaData != skillInfo)
	{
		return;
	}
	( ( cMultiTarget* )mpTarget )->RemoveAll();

	CObject* pObject;
	CTargetListIterator iter(pTList);

	VECTOR3 mainTargetPosition = { 0 };
	{
		CObject* mainTargetObject = 0;
		GetMainTargetPos(
			&mSkillObjectInfo.mainTarget,
			&mainTargetPosition,
			&mainTargetObject );

		if( mainTargetObject )
		{
			mainTargetObject->GetPosition( &mainTargetPosition );
		}
	}

	while(iter.GetNextTarget())
	{
		pObject = g_pUserTable->FindUser(iter.GetTargetID());

		if(pObject == NULL)
			continue;
		// 090108 LUJ, 타겟의 위치를 알아낸다
		VECTOR3 targetPosition = { 0 };
		pObject->GetPosition( &targetPosition );

		// 090108 LUJ, 주 타겟을 중심으로 범위가 유효한 경우에만 타겟으로 등록시킨다
		//if(validArea < CalcDistanceXZ(&mainTargetPosition, &targetPosition))
		//{
		//	//( ( cMultiTarget* )mpTarget )->AddData( pObject );
		//	continue;
		//}

		( ( cMultiTarget* )mpTarget )->AddData( pObject );
	}

	iter.Release();
}

void cSkillObject::SetEndState()
{
	mState = SKILL_STATE_END;
}

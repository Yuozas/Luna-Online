#include "stdafx.h"
#include ".\onoffskillobject.h"
#include "..\Delay\Delay.h"
#include "..\Info\ActiveSkillInfo.h"
#include "..\Info\BuffSkillInfo.h"
#include "..\Unit\SkillUnit.h"
#include "..\Object\BuffSkillObject.h"
#include "..\Manager\SkillManager.h"
#include "..\Target\Target.h"
#include "Player.h"
#include "../[CC]BattleSystem/Battle.h"
#include "CharMove.h"
#include "PackedData.h"
#include "Battle.h"


cOnOffSkillObject::cOnOffSkillObject( cSkillInfo* pSkillInfo, cTarget* pTarget )
: cActiveSkillObject( pSkillInfo, pTarget )
{
	// 080602 LUJ, 오브젝트 타입을 초기화한다
	mType = cSkillObject::TypeOnOff;

	mpInterval = new cDelay;

	mpInterval->Init( 3000 );

}

cOnOffSkillObject::~cOnOffSkillObject(void)
{
	delete mpInterval;
}

void cOnOffSkillObject::Init( sSKILL_CREATE_INFO* pInfo )
{
/// cSkillObject의 Init함수 호출
	cSkillObject::Init( pInfo );

	MSG_SKILLOBJECT_ADD2 msg;
	
	msg.Category = MP_SKILL;
	msg.Protocol = MP_SKILL_SKILLOBJECT_ADD;
	msg.bCreate = TRUE;

	msg.SkillObjectInfo.SkillObjectIdx = pInfo->skillObjectId;
	msg.SkillObjectInfo.SkillIdx = mpSkillInfo->GetIndex();

	msg.SkillObjectInfo.Pos = pInfo->pos;
	msg.SkillObjectInfo.StartTime = 0;
	msg.SkillObjectInfo.Direction = pInfo->skillDir;
	msg.SkillObjectInfo.SkillLevel = ( BYTE )pInfo->level;
	msg.SkillObjectInfo.Operator = pInfo->operatorId;
	msg.SkillObjectInfo.Rate = 1.0f;
	msg.SkillObjectInfo.RemainTime = pInfo->remainTime;

	memcpy( &( msg.SkillObjectInfo.MainTarget ), &( pInfo->mainTarget ), sizeof( MAINTARGET ) );

	msg.SkillObjectInfo.BattleID = GetBattleID();

	PACKEDDATA_OBJ->QuickSend( GetOperator(), &msg, sizeof( MSG_SKILLOBJECT_ADD2 ) );

	DWORD idx = 0;

	/// 버프 스킬 정보
	for( WORD i = 0; i < MAX_BUFF_COUNT; i++ )
	{
		idx = ( ( cActiveSkillInfo* )mpSkillInfo )->GetSkillInfo()->Buff[ i ];

		if( idx )
		{
			mpBuffSkillInfo[i] = ( cBuffSkillInfo* )( SKILLMGR->GetSkillInfo( idx ) );
		}
		else
		{
			mpBuffSkillInfo[i] = NULL;
		}
	}

	/// 플레이어의 경우
	if( GetOperator()->GetObjectKind() == eObjectKind_Player )
	{
		/// 쿨링 딜레이 설정
		DWORD delay = ( ( cActiveSkillInfo* )mpSkillInfo )->GetSkillInfo()->CoolTime;

		if( delay )
		{
			mpDelay[ DELAY_KIND_COOLING ]->Init( delay );
			mpDelay[ DELAY_KIND_COOLING ]->Start();
		}
	}

	CObject* pTarget = NULL;

	if( mpTarget->GetKind() < TARGET_KIND_MULTI )
	{
		pTarget = mpTarget->GetData();

//		pTarget->GetOnOffSkillList()->Add( this, mpSkillInfo->GetIndex() );
	}
	/// Multi Target인 경우
	else
	{
		mpTarget->SetPositionHead();
		while( pTarget = mpTarget->GetData() )
		{
//			pTarget->GetOnOffSkillList()->Add( this, mpSkillInfo->GetIndex() );
		}
	}

	Excute();
	mState = SKILL_STATE_PROCESS;
}

void cOnOffSkillObject::Release()
{
	cActiveSkillObject::Release();

}

void cOnOffSkillObject::Destroy()
{
	m_BuffSkillTable.SetPositionHead();

	while( cBuffSkillObject* pBuff = m_BuffSkillTable.GetData() )
	{
		pBuff->SetEndState();
		pBuff->EndState();
	}

	m_BuffSkillTable.RemoveAll();


	CObject* pTarget = NULL;

	if( mpTarget->GetKind() < TARGET_KIND_MULTI )
	{
		pTarget = mpTarget->GetData();
//		pTarget->GetOnOffSkillList()->Remove( mpSkillInfo->GetIndex() );
	}
	/// Multi Target인 경우
	else
	{
		mpTarget->SetPositionHead();
		while( pTarget = mpTarget->GetData() )
		{
//			pTarget->GetOnOffSkillList()->Remove( mpSkillInfo->GetIndex() );
		}
	}

	mState = SKILL_STATE_DESTROY;
}

SKILL_STATE	cOnOffSkillObject::Update()
{
	if( mpInterval->IsStart() )
	{
		if( mpInterval->Check() )
		{
		}
		else
		{
			Consumption();
			mpInterval->Start();
		}
	}
	else
	{
		Consumption();
		mpInterval->Start();
	}

	CCharMove::SetPosition( this, CCharMove::GetPosition( mpTarget->GetData() ) );

	return cSkillObject::Update();
}

void cOnOffSkillObject::Excute()
{
	/// 버프 스킬 생성
	CObject* pTarget = NULL;
	cBuffSkillObject* pBuff = NULL;

	for( WORD i = 0; i < MAX_BUFF_COUNT; i++ )
	{
		/// 버프 스킬 정보가 없으면 패스
		if( !mpBuffSkillInfo[ i ] )
		{
			continue;
		}

		/// 타겟 종류
		/// Single Target인 경우
		if( mpTarget->GetKind() < TARGET_KIND_MULTI )
		{
			pTarget = mpTarget->GetData();

			/// 기본 설정 복사
			memcpy( &( mBuffSkillObjectInfo[ i ] ), &mSkillObjectInfo, sizeof( sSKILL_CREATE_INFO ) );

			pBuff = ( cBuffSkillObject* )( SKILLMGR->OnBuffSkillStart( mpBuffSkillInfo[i], &( mBuffSkillObjectInfo[ i ] ) ) );
			pBuff->SetDelayCheck( FALSE );

			if( pBuff )
			{
				AddBuffSkillObject( pBuff );
			}
		}
		/// Multi Target인 경우
		else
		{
			mpTarget->SetPositionHead();
			while( pTarget = mpTarget->GetData() )
			{
				/// 기본 설정 복사
				memcpy( &( mBuffSkillObjectInfo[ i ] ), &mSkillObjectInfo, sizeof( sSKILL_CREATE_INFO ) );

				mBuffSkillObjectInfo[ i ].mainTarget.SetMainTarget( pTarget->GetID() );

				pBuff = ( cBuffSkillObject* )( SKILLMGR->OnBuffSkillStart( mpBuffSkillInfo[i], &( mBuffSkillObjectInfo[ i ] ) ) );
				pBuff->SetDelayCheck( FALSE );

				if( pBuff )
				{
					AddBuffSkillObject( pBuff );
				}
			}
		}
	}

	/// 스킬 효과 발동
	if( mpSkillUnit )
	{
		WORD result = mpSkillUnit->Excute();
	}

	Consumption();
}

void cOnOffSkillObject::Consumption()
{
	if( GetOperator()->GetObjectKind() == eObjectKind_Player )
	{
		CPlayer* pOperator = (CPlayer*)GetOperator();

		const ACTIVE_SKILL_INFO* info = ( ( cActiveSkillInfo* )mpSkillInfo )->GetSkillInfo();

		if( ! info )
		{
			return;
		}

		// 080616 LUJ, 퍼센트 정보도 함께 처리한다
		const DWORD mana	= DWORD( float( pOperator->GetMana() ) * info->mMana.mPercent + info->mMana.mPlus );
		const DWORD life	= DWORD( float( pOperator->GetLife() ) * info->mLife.mPercent + info->mLife.mPlus );
		const DWORD money	= info->Money;

		if( pOperator->GetMana() < mana	||
			pOperator->GetLife() < life ||
			pOperator->GetMoney() < money )
		{
			Destroy();
			return;
		}

		if( mana ) pOperator->ReduceMana( mana );
		if( life ) pOperator->AddLife( life * -1, NULL );
		if( money ) pOperator->SetMoney( money, MONEY_SUBTRACTION, MF_OBTAIN );
	}
}

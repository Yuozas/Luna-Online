#include "stdafx.h"
#include ".\activeskillobject.h"
#include "..\Delay\Delay.h"
#include "..\Info\ActiveSkillInfo.h"
#include "..\Info\BuffSkillInfo.h"
#include "..\Unit\SkillUnit.h"
#include "..\Object\BuffSkillObject.h"
#include "..\Manager\SkillManager.h"
#include "..\Target\Target.h"
#include "Player.h"
#include "../[CC]BattleSystem/Battle.h"
#include "PackedData.h"
#include "Battle.h"
#include "MapDBMsgParser.h"
#include "pet.h"

cActiveSkillObject::cActiveSkillObject( cSkillInfo* pSkillInfo, cTarget* pTarget )
: cSkillObject( pSkillInfo, pTarget )
{
	// 080602 LUJ, 오브젝트 타입을 초기화한다
	mType = cSkillObject::TypeActive;

	mpDelay[ DELAY_KIND_CASTING ] = new cDelay;
	mpDelay[ DELAY_KIND_PROCESS ] = new cDelay;
	mpDelay[ DELAY_KIND_COOLING ] = new cDelay;

	m_BuffSkillTable.Initialize( 3 );
}

cActiveSkillObject::~cActiveSkillObject(void)
{
	delete mpDelay[ DELAY_KIND_CASTING ];
	delete mpDelay[ DELAY_KIND_PROCESS ];
	delete mpDelay[ DELAY_KIND_COOLING ];

	if( mpSkillUnit )
	{
		delete mpSkillUnit;
	}
}

void cActiveSkillObject::Init( sSKILL_CREATE_INFO* pInfo )
{
	/// cSkillObject의 Init함수 호출
	cSkillObject::Init( pInfo );

	DWORD idx = 0;

	/// 버프 스킬 정보
	for( WORD i = 0; i < MAX_BUFF_COUNT; i++ )
	{
		idx = ( ( cActiveSkillInfo* )mpSkillInfo )->GetSkillInfo()->Buff[ i ];

		if( idx )
		{
			mpBuffSkillInfo[i] = ( cBuffSkillInfo* )( SKILLMGR->GetSkillInfo( idx ) );
			mBuffRate[i] = ( ( cActiveSkillInfo* )mpSkillInfo )->GetSkillInfo()->BuffRate[ i ];

			// 080219 LUJ, 기간제 스킬인 경우 사용 즉시 기간을 연장해주어야 한다
			{
				const BUFF_SKILL_INFO* skillInfo = mpBuffSkillInfo[ i ]->GetSkillInfo();

				if( skillInfo &&
					skillInfo->IsEndTime )
				{
					CharacterBuffAdd(
						GetOperator()->GetID(),
						skillInfo->Index,
						skillInfo->DelayTime,
						skillInfo->Count,
						TRUE );
				}
			}			
		}
		else
		{
			mpBuffSkillInfo[i] = NULL;
		}
	}

	mAttackSpeedRate = 1.0f;

	if( GetOperator()->GetObjectKind() == eObjectKind_Player )
	{
		CPlayer* pPlayer = ( CPlayer* )GetOperator();

		if( mpSkillInfo->GetKind() == SKILLKIND_PHYSIC ||
			mpSkillInfo->GetKind() == SKILLKIND_MAGIC )
		{
			DWORD idx = mpSkillInfo->GetIndex();

			cActiveSkillInfo* pSkillInfo = ( cActiveSkillInfo* )mpSkillInfo;

			if( (idx / 100000) % 10 )
			{
				if( pSkillInfo->GetSkillInfo()->Unit == UNITKIND_PHYSIC_ATTCK )
				{
					mAttackSpeedRate += ( ( pPlayer->GetRateBuffStatus()->PhysicSkillSpeedRate + pPlayer->GetRatePassiveStatus()->PhysicSkillSpeedRate ) * 0.01f );
				}
				else if( pSkillInfo->GetSkillInfo()->Unit == UNITKIND_MAGIC_ATTCK )
				{
					mAttackSpeedRate += ( ( pPlayer->GetRateBuffStatus()->MagicSkillSpeedRate + pPlayer->GetRatePassiveStatus()->MagicSkillSpeedRate ) *0.01f );
				}
			}
			else
			{
				mAttackSpeedRate += ( ( pPlayer->GetRateBuffStatus()->NormalSpeedRate + pPlayer->GetRatePassiveStatus()->NormalSpeedRate ) * 0.01f );
			}
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

	if( mAttackSpeedRate < 0 )
	{
		mAttackSpeedRate = 1.0f;
	}

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
	msg.SkillObjectInfo.Rate = mAttackSpeedRate;
	msg.SkillObjectInfo.RemainTime = 0;
	msg.SkillObjectInfo.Count = 0;

	memcpy( &( msg.SkillObjectInfo.MainTarget ), &( pInfo->mainTarget ), sizeof( MAINTARGET ) );

	msg.SkillObjectInfo.BattleID = GetBattleID();

	PACKEDDATA_OBJ->QuickSend( GetOperator(), &msg, sizeof( MSG_SKILLOBJECT_ADD2 ) );

	/// 시전 딜레이 체크
	DWORD casting = ( ( cActiveSkillInfo* )mpSkillInfo )->GetSkillInfo()->CastingTime;
	
	GetOperator()->CurCastingSkillID = GetID();

	if( casting )
	/// 있다면
	{
		/// 시전 시작
		CastingStart();
	}
	/// 없다면
	{
		/// 발동 시작
		ProcessStart();
	}
}

void cActiveSkillObject::Release()
{
}

void cActiveSkillObject::AddSkillUnit( cSkillUnit* pSkillUnit )
{
	mpSkillUnit = pSkillUnit;
}

SKILL_STATE cActiveSkillObject::Update()
{
	if( !GetOperator() )
	{
		mState = SKILL_STATE_DESTROY;
	}

	if( mState == SKILL_STATE_DESTROY )
	{
		return cSkillObject::Update();
	}

	switch( mState )
	{
	/// 시전중
	case SKILL_STATE_CASTING:
		{
			CastingState();
		}
		break;
	/// 발동중
	case SKILL_STATE_PROCESS:
		{
			ProcessState();
		}
		break;
	case SKILL_STATE_END:
		{
			EndState();
		}
		break;
	case SKILL_STATE_DESTROY:
		break;
	}

	return cSkillObject::Update();
}

void cActiveSkillObject::Excute()
{
	if( !GetOperator() )
	{
		mState = SKILL_STATE_DESTROY;
		return;
	}

	GetOperator()->CurCastingSkillID = 0;

	/// 버프 스킬 생성
	CObject* pTarget = NULL;
	cBuffSkillObject* pBuff = NULL;

	/// 스킬 효과 발동
	if( mpSkillUnit )
	{
		WORD result = mpSkillUnit->Excute();
	}

	if( GetOperator()->GetObjectKind() == eObjectKind_Player )
	{
		const ACTIVE_SKILL_INFO* info = ( ( cActiveSkillInfo* )mpSkillInfo )->GetSkillInfo();

		if( ! info )
		{
			return;
		}

		CPlayer* pOperator = (CPlayer*)GetOperator();
		// 071203 KTH --- BuffStatus에서 DecreaseManaRate 만큼의 마나 소모량을 줄여준다.
		DWORD NeedMana = DWORD( float( pOperator->GetMana() ) * info->mMana.mPercent + info->mMana.mPlus );
		NeedMana = (DWORD) (NeedMana + ( NeedMana * (pOperator->GetRateBuffStatus()->DecreaseManaRate / 100 ) ) );
		pOperator->ReduceMana( NeedMana );
		//---
		
		// 080710 LUJ, 스킬 사용 시 HP를 소모하도록 함
		pOperator->ReduceLife( DWORD( float( pOperator->GetLife() ) * info->mLife.mPercent + info->mLife.mPlus ) );
	}
	else if( GetOperator()->GetObjectKind() == eObjectKind_Pet )
	{
		const ACTIVE_SKILL_INFO* info = ( ( cActiveSkillInfo* )mpSkillInfo )->GetSkillInfo();

		if( ! info )
		{
			return;
		}

		CPet* pOperator = (CPet*)GetOperator();
		// 071203 KTH --- BuffStatus에서 DecreaseManaRate 만큼의 마나 소모량을 줄여준다.
		DWORD NeedMana = DWORD( float( pOperator->GetMana() ) * info->mMana.mPercent + info->mMana.mPlus );
		NeedMana = (DWORD) (NeedMana + ( NeedMana * (pOperator->GetRateBuffStatus()->DecreaseManaRate / 100 ) ) );
		pOperator->ReduceMana( NeedMana );
		//---
		
		// 080710 LUJ, 스킬 사용 시 HP를 소모하도록 함
		pOperator->ReduceLife( DWORD( float( pOperator->GetLife() ) * info->mLife.mPercent + info->mLife.mPlus ) );
	}

	if( ( ( cActiveSkillInfo* )mpSkillInfo )->GetSkillInfo()->IsMove )
	{
		if( mpTarget->GetKind() < TARGET_KIND_MULTI )
		{
			pTarget = mpTarget->GetData();

			if( pTarget )
			{
				if( pTarget->GetState()  == eObjectState_Die )
				{
					return;
				}

				pTarget->SetEffectMove();
			}
		}
		else
		{
			mpTarget->SetPositionHead();
			while( pTarget = mpTarget->GetData() )
			{
				if( pTarget->GetState()  == eObjectState_Die )
				{
					continue;
				}

				pTarget->SetEffectMove();
			}
		}
	}

	for( WORD i = 0; i < MAX_BUFF_COUNT; i++ )
	{
		/// 버프 스킬 정보가 없으면 패스
		if( !mpBuffSkillInfo[ i ] )
		{
			continue;
		}

		WORD rate = (WORD)(rand() % 100);
		if( mBuffRate[i] <= rate )
		{
			continue;
		}

		/// 타겟 종류
		/// Single Target인 경우
		if( mpTarget->GetKind() < TARGET_KIND_MULTI )
		{
			pTarget = mpTarget->GetData();

			if( pTarget )
			{
				if( pTarget->GetState()  == eObjectState_Die )
				{
					return;
				}

				/// 기본 설정 복사
				memcpy( &( mBuffSkillObjectInfo[ i ] ), &mSkillObjectInfo, sizeof( sSKILL_CREATE_INFO ) );

				pBuff = ( cBuffSkillObject* )( SKILLMGR->OnBuffSkillStart( mpBuffSkillInfo[i], &( mBuffSkillObjectInfo[ i ] ) ) );

				if( pBuff )
				{
					AddBuffSkillObject( pBuff );

					// 080708 LUJ, 이벤트가 필요한 스킬은 플레이어에게 등록해놓는다
					pTarget->AddEventSkill( *pBuff );
				}
			}
		}
		/// Multi Target인 경우
		else
		{
			mpTarget->SetPositionHead();
			while( pTarget = mpTarget->GetData() )
			{
				if( pTarget->GetState()  == eObjectState_Die )
				{
					continue;
				}

				/// 기본 설정 복사
				memcpy( &( mBuffSkillObjectInfo[ i ] ), &mSkillObjectInfo, sizeof( sSKILL_CREATE_INFO ) );

				mBuffSkillObjectInfo[ i ].mainTarget.SetMainTarget( pTarget->GetID() );

				pBuff = ( cBuffSkillObject* )( SKILLMGR->OnBuffSkillStart( mpBuffSkillInfo[i], &( mBuffSkillObjectInfo[ i ] ) ) );

				if( pBuff )
				{
					AddBuffSkillObject( pBuff );

					// 080708 LUJ, 이벤트가 필요한 스킬은 플레이어에게 등록해놓는다
					pTarget->AddEventSkill( *pBuff );
				}
			}
		}

	}
}

void cActiveSkillObject::CastingStart()
{
	DWORD casting = ( ( cActiveSkillInfo* )mpSkillInfo )->GetSkillInfo()->CastingTime;

	casting = ( DWORD )( casting / mAttackSpeedRate );

	mpDelay[ DELAY_KIND_CASTING ]->Init( casting );
	mpDelay[ DELAY_KIND_CASTING ]->Start();
	/// 스킬 상태 초기화
	mState = SKILL_STATE_CASTING;
}

void cActiveSkillObject::CastingState()
{
	/// 시전 딜레이 체크
	if( !mpDelay[ DELAY_KIND_CASTING ]->Check() )
	{
		/// 발동 시작
		ProcessStart();
	}
}

void cActiveSkillObject::ProcessStart()
{
	/// 발동 딜레이 체크
	WORD type = ( ( cActiveSkillInfo* )mpSkillInfo )->GetSkillInfo()->DelayType;

	switch( type )
	{
		/// 발동 딜레이 없음
	case DELAY_NONE:
		{
			/// 스킬 종료 상태로 변경
			mState = SKILL_STATE_END;

			/// 스킬 실행
			Excute();
		}
		break;
		/// 시간형
	case DELAY_TIME:
		{
			/// 딜레이 설정
			DWORD delay = ( ( cActiveSkillInfo* )mpSkillInfo )->GetSkillInfo()->DelayTime;

			delay = ( DWORD )( delay / mAttackSpeedRate );
			mpDelay[ DELAY_KIND_PROCESS ]->Init( delay );
			mpDelay[ DELAY_KIND_PROCESS ]->Start();

			/// 발동중 상태로 변경
			mState = SKILL_STATE_PROCESS;
		}
		break;
		/// 이동형
	case DELAY_SCOPE:
		{
			/// 이동속도
			DWORD speed = ( ( cActiveSkillInfo* )mpSkillInfo )->GetSkillInfo()->DelayTime;

			/// 시전자 위치 타겟 지점의 거리 계산
			VECTOR3 oper;
			VECTOR3 tar;

			/// 시전자 위치
			GetOperator()->GetPosition( &oper );

			/// 타겟 위치
			tar.x = mSkillObjectInfo.mainTarget.cpTargetPos.wx;
			tar.z = mSkillObjectInfo.mainTarget.cpTargetPos.wz;

			/// 거리 계산
			DWORD distance = ( DWORD )CalcDistanceXZ( &oper, &tar );

			/// 시간 계산
			DWORD delay = ( distance / speed ) * 1000;

			mpDelay[ DELAY_KIND_PROCESS ]->Init( delay );
			mpDelay[ DELAY_KIND_PROCESS ]->Start();

			/// 발동중 상태로 변경
			mState = SKILL_STATE_PROCESS;
		}
		break;
	}
}

void cActiveSkillObject::ProcessState()
{
	/// 발동 딜레이 체크
	if( !mpDelay[ DELAY_KIND_PROCESS ]->Check() )
	{
		/// 실행
		Excute();

		mState = SKILL_STATE_END;
	}
}

void cActiveSkillObject::AddBuffSkillObject( cBuffSkillObject* pBuff )
{
	m_BuffSkillTable.Add( pBuff, pBuff->GetID() );
}

void cActiveSkillObject::RemoveBuffSkillObject ( cBuffSkillObject* pBuff )
{
	if( m_BuffSkillTable.GetData( pBuff->GetID() ) )
	{
		m_BuffSkillTable.Remove( pBuff->GetID() );
	}
}

// 080602 LUJ, 해당 오브젝트로 발동된 버프 스킬을 모두 제거한다
void cActiveSkillObject::RemoveAllBuffSkillObject()
{
	m_BuffSkillTable.SetPositionHead();

	for( ;; )
	{
		cBuffSkillObject* object = m_BuffSkillTable.GetData();

		if( ! object )
		{
			break;
		}

		object->SetEndState();
		object->EndState();
	}
}

void cActiveSkillObject::SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin)
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
	pMsg->SkillObjectInfo.Rate = mAttackSpeedRate;
	pMsg->SkillObjectInfo.RemainTime = 0;
	pMsg->SkillObjectInfo.Count = 0;

	memcpy( &( pMsg->SkillObjectInfo.MainTarget ), &( mSkillObjectInfo.mainTarget ), sizeof( MAINTARGET ) );

	pMsg->SkillObjectInfo.BattleID = GetBattleID();

    *pMsgLen = sizeof( MSG_SKILLOBJECT_ADD2 );
}

void cActiveSkillObject::SetRemoveMsg(char* pRemoveMsg,WORD* pMsgLen,DWORD dwReceiverID)
{
	MSG_DWORD* pMsg = ( MSG_DWORD* )pRemoveMsg;

	pMsg->Category = MP_SKILL;
	pMsg->Protocol = MP_SKILL_SKILLOBJECT_REMOVE;
	pMsg->dwObjectID = dwReceiverID;
	pMsg->dwData = GetID();

	*pMsgLen = sizeof( MSG_DWORD );
}

#include "stdafx.h"
#include ".\buffskillobject.h"
#include "../Info/BuffSkillInfo.h"
#include "../Delay/Delay.h"
#include "../Target/Target.h"
#include "../Status/Status.h"
#include "CharMove.h"
#include "AttackManager.h"
#include "PackedData.h"
#include "Battle.h"
#include "MapDBMsgParser.h"
#include "UserTable.h"
// 080616 LUJ, 버프 중 이벤트 처리하기위해 참조
#include "Event.h"
#include "CharacterCalcManager.h"
#include "..\[cc]skill\Server\\Manager\\SkillManager.h"
#include "pet.h"

cBuffSkillObject::cBuffSkillObject( cSkillInfo* pSkillInfo, cTarget* pTarget )
: cSkillObject( pSkillInfo, pTarget )
{
	// 080602 LUJ, 오브젝트 타입을 초기화한다
	mType = cSkillObject::TypeBuff;

	mpDelay = new cDelay;
	mpInterval = new cDelay;
	mpUpdate = new cDelay;
	mpConsumption = new cDelay;

	mpInterval->Init( 1000 );
	mpUpdate->Init( 1800000 );
	mpConsumption->Init( 3000 );

	mStatusKind =  ( ( cBuffSkillInfo* )mpSkillInfo )->GetSkillInfo()->Status;
	mStatusDataType = ( ( cBuffSkillInfo* )mpSkillInfo )->GetSkillInfo()->StatusDataType;

	mDelayCheck = TRUE;
	mDestroy = FALSE;
	mIsUpdate = TRUE;

	mCount = 0;
	mCountType = eBuffSkillCountType_None;
}

cBuffSkillObject::~cBuffSkillObject(void)
{
	delete mpDelay;
	delete mpInterval;
	delete mpUpdate;
	delete mpConsumption;
}

void cBuffSkillObject::Init( sSKILL_CREATE_INFO* pInfo )
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
	msg.SkillObjectInfo.Count = pInfo->count;

	memcpy( &( msg.SkillObjectInfo.MainTarget ), &( pInfo->mainTarget ), sizeof( MAINTARGET ) );

	msg.SkillObjectInfo.BattleID = GetBattleID();

	mDelayCheck = TRUE;
	mDestroy = FALSE;

	if( !mpTarget->GetData() )
	{
		mState = SKILL_STATE_DESTROY;
		return;
	}

	if( mpTarget->GetData()->GetObjectKind() != eObjectKind_Player && 
		mpTarget->GetData()->GetObjectKind() != eObjectKind_Pet )
	{
		mIsUpdate = FALSE;
	}

	mStatusData = ( ( cBuffSkillInfo* )mpSkillInfo )->GetSkillInfo()->StatusData;

	mpTargetStatus = mpTarget->GetData()->GetBuffStatus();

	DWORD idx = GetSkillIdx() / 100;

	cBuffSkillObject* pObject = ( cBuffSkillObject* )( mpTarget->GetData()->GetBuffSkillList()->GetData( idx ) );

	if( pObject )
	{
		if( pObject->GetSkillLevel() > GetSkillLevel() )
		{
			mState = SKILL_STATE_DESTROY;
			return;
		}
		else if( pObject->GetSkillLevel() == GetSkillLevel() )
		{
			mState = SKILL_STATE_DESTROY;
			pObject->ResetDelay();
			return;
		}
		else
		{
			pObject->SetEndState();
			pObject->EndState();
			ProcessStart();
		}
	}
	else
	{
		ProcessStart();
	}

	msg.SkillObjectInfo.Count = mCount;

	mpTarget->GetData()->GetBuffSkillList()->Add( ( cSkillObject* )this, idx );

	PACKEDDATA_OBJ->QuickSend( mpTarget->GetData(), &msg, sizeof( MSG_SKILLOBJECT_ADD2 ) );
}

void cBuffSkillObject::Release()
{
}

SKILL_STATE cBuffSkillObject::Update()
{
	if( mState == SKILL_STATE_DESTROY )
	{
		return cSkillObject::Update();
	}

	if( !mpTarget->GetData() )
	{
		mState = SKILL_STATE_DESTROY;
	}
	else
	{
		CPlayer* pPlayer =  (CPlayer*)GetOperator();
		if( mpTarget->GetData()->GetState() == eObjectState_Die )
		{
			if( ( ( cBuffSkillInfo* )mpSkillInfo )->GetSkillInfo()->Die == 0 )
			{
				SetEndState();
				EndState();
			}
		}


		switch( mState )
		{
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
		}

		CCharMove::SetPosition( this, CCharMove::GetPosition( mpTarget->GetData() ) );
	}

	return cSkillObject::Update();
}

void cBuffSkillObject::Excute()
{
}

void cBuffSkillObject::ProcessStart()
{
	const BUFF_SKILL_INFO* info = ( ( cBuffSkillInfo* )mpSkillInfo )->GetSkillInfo();

	/// 지속 시간 설정
	/// 지속 시간이 없다면 체크를 비활성화
	if( info->DelayTime )
	{
		mDelayCheck = TRUE;
		
		if( info->DelayTime < 10000 )
		{
			mIsUpdate = FALSE;
		}
	}
	else
	{
		mDelayCheck = FALSE;
	}

	/// 카운트 설정
	if( info->CountType )
	{
		mCountType = info->CountType;

		if( mSkillObjectInfo.count )
		{
			mCount = mSkillObjectInfo.count;
		}
		else
		{
			mCount = info->Count;
		}
	}

	if( mDelayCheck )
	{
		if( mSkillObjectInfo.remainTime )
		{
			mpDelay->Init( mSkillObjectInfo.remainTime );
			mpDelay->Start();
		}
		else
		{
			mpDelay->Init( info->DelayTime );
			mpDelay->Start();

			if( mIsUpdate )
			{
				CharacterBuffAdd( mpTarget->GetData()->GetID(), mpSkillInfo->GetIndex(), mpDelay->GetRemainTime(), mCount, info->IsEndTime );
			}
		}
	}
	else
	{
		if( !mSkillObjectInfo.remainTime )
		{
			if( mIsUpdate )
			{
				CharacterBuffAdd( mpTarget->GetData()->GetID(), mpSkillInfo->GetIndex(), 1, mCount, info->IsEndTime );
			}
		}
	}

	mpConsumption->Start();
	mpUpdate->Start();

	mActive = TRUE;
	( ( cBuffSkillInfo* )mpSkillInfo )->AddBuffStatus( mpTarget->GetData() );

	/// 발동중 상태로 변경
	mState = SKILL_STATE_PROCESS;
}

void cBuffSkillObject::ProcessState()
{
	/// 지속 시간 체크
	if( mDelayCheck )
	{
		if( !mpDelay->Check() )
		{
			mState = SKILL_STATE_END;
			return;
		}
	}

	/// 예외처리
	CObject* pTarget = NULL;
	pTarget = mpTarget->GetData();

	/// 1. 전투 상태
	if( pTarget->GetObjectBattleState() )
	{
		switch( ( ( cBuffSkillInfo* )mpSkillInfo )->GetSkillInfo()->Battle )
		{
		case eBuffSkillActiveRule_Active:
			{
				if( !mActive )
				{
					SetActive( TRUE );
				}
			}
			break;
		case eBuffSkillActiveRule_Delete:
			{
				SetEndState();
			}
			break;
		case eBuffSkillActiveRule_Stop:
			{
				if( mActive )
				{
					SetActive( FALSE );
				}
			}
			break;
		}
	}
	else
	{
		switch( ( ( cBuffSkillInfo* )mpSkillInfo )->GetSkillInfo()->Peace )
		{
		case eBuffSkillActiveRule_Active:
			{
				if( !mActive )
				{
					SetActive( TRUE );
				}
			}
			break;
		case eBuffSkillActiveRule_Delete:
			{
				SetEndState();
			}
			break;
		case eBuffSkillActiveRule_Stop:
			{
				if( mActive )
				{
					SetActive( FALSE );
				}
			}
			break;
		}
	}

	/// 2. 이동 상태
	if( CCharMove::IsMoving( pTarget ) )
	{
		switch( ( ( cBuffSkillInfo* )mpSkillInfo )->GetSkillInfo()->Move )
		{
		case eBuffSkillActiveRule_Active:
			{
				if( !mActive )
				{
					SetActive( TRUE );
				}
			}
			break;
		case eBuffSkillActiveRule_Delete:
			{
				SetEndState();
			}
			break;
		case eBuffSkillActiveRule_Stop:
			{
				if( mActive )
				{
					SetActive( FALSE );
				}
			}
			break;
		}
	}
	else
	{
		switch( ( ( cBuffSkillInfo* )mpSkillInfo )->GetSkillInfo()->Stop )
		{
		case eBuffSkillActiveRule_Active:
			{
				if( !mActive )
				{
					SetActive( TRUE );
				}
			}
			break;
		case eBuffSkillActiveRule_Delete:
			{
				SetEndState();
			}
			break;
		case eBuffSkillActiveRule_Stop:
			{
				if( mActive )
				{
					SetActive( FALSE );
				}
			}
			break;
		}
	}

	switch( mStatusKind )
	{
	case eStatusKind_Poison:
	case eStatusKind_Bleeding:
	case eStatusKind_Burning:
	case eStatusKind_HolyDamage:
		{
			CObject*	pTarget = NULL;
			RESULTINFO	damage;
			MSG_SKILL_RESULT msg;

			msg.InitMsg( GetID() );
			msg.SkillDamageKind = false;

			CTargetListIterator iter(&msg.TargetList);

			if( mpInterval->IsStart() )
			{
				if( mpInterval->Check() )
				{
				}
				else
				{
					pTarget = mpTarget->GetData();

					ATTACKMGR->SpecialAttack( GetOperator(), pTarget, &damage, mStatusData );

					iter.AddTargetWithResultInfo( pTarget->GetID(), 1, &damage);
					iter.Release();

					PACKEDDATA_OBJ->QuickSend( pTarget, &msg, msg.GetMsgLength() );

					mpInterval->Start();
				}
			}
			else
			{
				pTarget = mpTarget->GetData();

				ATTACKMGR->SpecialAttack( GetOperator(), pTarget, &damage, mStatusData );

				iter.AddTargetWithResultInfo( pTarget->GetID(), 1, &damage);
				iter.Release();

				PACKEDDATA_OBJ->QuickSend( pTarget, &msg, msg.GetMsgLength() );

				mpInterval->Start();
			}
		}
		break;
	}

	if( !mpConsumption->Check() )
	{
		Consumption();
		mpConsumption->Start();
	}

	if( !mpUpdate->Check() )
	{
		if( mDelayCheck )
		{
			if( mIsUpdate )
			{
				CharacterBuffUpdate( mpTarget->GetData()->GetID(), mpSkillInfo->GetIndex(), mpDelay->GetRemainTime(), mCount );
			}
		}
		else
		{
			if( mIsUpdate )
			{
				CharacterBuffUpdate( mpTarget->GetData()->GetID(), mpSkillInfo->GetIndex(), 1, mCount );
			}
		}
		mpUpdate->Start();
	}
}

void cBuffSkillObject::SetEndState()
{
	mState = SKILL_STATE_END;
	mDestroy = TRUE;
	mpDelay->End();
}

void cBuffSkillObject::EndState()
{
	if( mDelayCheck )
	{
		if( mpDelay->Check() )
		{
			if( mIsUpdate )
			{
				CharacterBuffUpdate( mpTarget->GetData()->GetID(), mpSkillInfo->GetIndex(), mpDelay->GetRemainTime(), mCount );
			}
		}
		else
		{
			if( mIsUpdate )
			{
				CharacterBuffRemove( mpTarget->GetData()->GetID(), mpSkillInfo->GetIndex() );
			}
		}
	}
	else
	{
		if( mDestroy )
		{
			if( mIsUpdate )
			{
				CharacterBuffRemove( mpTarget->GetData()->GetID(), mpSkillInfo->GetIndex() );
			}
		}
		else
		{
			if( mIsUpdate )
			{
				CharacterBuffUpdate( mpTarget->GetData()->GetID(), mpSkillInfo->GetIndex(), 1, mCount );
			}
		}
	}

	mDestroy = FALSE;
	mActive = FALSE;
	( ( cBuffSkillInfo* )mpSkillInfo )->RemoveBuffStatus( mpTarget->GetData() );
	mDelayCheck = TRUE;
	mState = SKILL_STATE_DESTROY;

	DWORD idx = GetSkillIdx() / 100;

	mpTarget->GetData()->GetBuffSkillList()->Remove( idx );
}

void cBuffSkillObject::SetActive( BOOL val )
{
	if( mState != SKILL_STATE_PROCESS )
	{
		return;
	}

	mActive = val;

	if( val )
	{
		( ( cBuffSkillInfo* )mpSkillInfo )->AddBuffStatus( mpTarget->GetData() );
	}
	else
	{
		( ( cBuffSkillInfo* )mpSkillInfo )->RemoveBuffStatus( mpTarget->GetData() );
	}
}


void cBuffSkillObject::Consumption()
{
	const BUFF_SKILL_INFO* info = ( ( cBuffSkillInfo* )mpSkillInfo )->GetSkillInfo();

	if( ! info )
	{
		return;
	}
	// 080728 LUJ, 소모할 값이 있는데 오퍼레이터가 존재하지 않으면 버프를 종료시켜야 한다
	else if(	info->mMana.mPlus		||
				info->mMana.mPercent	||
				info->mLife.mPlus		||
				info->mLife.mPercent	||
				info->Money )
	{
		if( ! GetOperator() )
		{
			SetEndState();
			return;
		}
	}
	// 080728 LUJ, 오퍼레이터가 없으면 소모할 주체가 없으므로 통과한다
	else if( ! GetOperator() )
	{
		return;
	}

	if( GetOperator()->GetObjectKind() == eObjectKind_Player )
	{
		CPlayer* pOperator = (CPlayer*)GetOperator();

		// 080616 LUJ, 퍼센트 정보도 함께 처리한다
		DWORD mana	= DWORD( float( pOperator->GetMana() ) * info->mMana.mPercent + info->mMana.mPlus );
		DWORD life	= DWORD( float( pOperator->GetLife() ) * info->mLife.mPercent + info->mLife.mPlus );
		DWORD money	= info->Money;

		if( pOperator->GetMana() < mana )
		{
			SetEndState();
			return;
		}

		if( pOperator->GetLife() < life )
		{
			SetEndState();
			return;
		}

		if( pOperator->GetMoney() < money )
		{
			SetEndState();
			return;
		}

		// 071203 KTH --- BuffStatus에서 DecreaseManaRate 만큼의 마나 소모량을 줄여준다.
		mana = (DWORD) (mana + ( mana * (pOperator->GetRateBuffStatus()->DecreaseManaRate / 100 ) ) );
		//---
		if( mana )	pOperator->ReduceMana( mana );
		if( life )	pOperator->AddLife( life * -1, NULL );
		if( money )	pOperator->SetMoney( money, MONEY_SUBTRACTION, MF_OBTAIN );
	}
	else if( GetOperator()->GetObjectKind() == eObjectKind_Pet )
	{
		CPet* pOperator = (CPet*)GetOperator();

		// 080616 LUJ, 퍼센트 정보도 함께 처리한다
		DWORD mana	= DWORD( float( pOperator->GetMana() ) * info->mMana.mPercent + info->mMana.mPlus );
		DWORD life	= DWORD( float( pOperator->GetLife() ) * info->mLife.mPercent + info->mLife.mPlus );

		if( pOperator->GetMana() < mana )
		{
			SetEndState();
			return;
		}

		if( pOperator->GetLife() < life )
		{
			SetEndState();
			return;
		}

		// 071203 KTH --- BuffStatus에서 DecreaseManaRate 만큼의 마나 소모량을 줄여준다.
		mana = (DWORD) (mana + ( mana * (pOperator->GetRateBuffStatus()->DecreaseManaRate / 100 ) ) );
		//---
		if( mana )	pOperator->ReduceMana( mana );
		if( life )	pOperator->AddLife( life * -1, NULL );
	}
}

void cBuffSkillObject::SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin)
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
	pMsg->SkillObjectInfo.Count = mCount;
	
	if( mDelayCheck )
	{
		pMsg->SkillObjectInfo.RemainTime = mpDelay->Check();
	}
	else
	{
		pMsg->SkillObjectInfo.RemainTime = 0;
	}
	memcpy( &( pMsg->SkillObjectInfo.MainTarget ), &( mSkillObjectInfo.mainTarget ), sizeof( MAINTARGET ) );

	pMsg->SkillObjectInfo.BattleID = GetBattleID();
   
	*pMsgLen = sizeof( MSG_SKILLOBJECT_ADD2 );
}

void cBuffSkillObject::ResetDelay()
{
	mpDelay->Init( ( ( cBuffSkillInfo* )mpSkillInfo )->GetSkillInfo()->DelayTime );
	if( mDelayCheck )
	{
		const BUFF_SKILL_INFO* info = ( ( cBuffSkillInfo* )mpSkillInfo )->GetSkillInfo();
		mpDelay->Init( info->DelayTime );
		mpDelay->Start();
	}

	MSG_DWORD2 msg;

	msg.Category = MP_SKILL;
	msg.Protocol = MP_SKILL_DELAY_RESET_NOTIFY;
	msg.dwData1 = mSkillObjectInfo.skillObjectId;

	if( mCountType )
	{
		mCount = ( ( cBuffSkillInfo* )mpSkillInfo )->GetSkillInfo()->Count;
	}

	msg.dwData2 = mCount;

	PACKEDDATA_OBJ->QuickSend( mpTarget->GetData(), &msg, sizeof( MSG_DWORD2 ) );

	if( mDelayCheck )
	{
		if( mIsUpdate )
		{
			CharacterBuffUpdate( mpTarget->GetData()->GetID(), mpSkillInfo->GetIndex(), mpDelay->GetRemainTime(), mCount );
		}
	}
	else
	{
		if( mIsUpdate )
		{
			CharacterBuffUpdate( mpTarget->GetData()->GetID(), mpSkillInfo->GetIndex(), 1, mCount );
		}
	}
}

BOOL cBuffSkillObject::DecreaseCount( BYTE kind )
{
	if( mCountType == eBuffSkillCountType_None )
		return FALSE;

	if( mCountType != kind )
		return FALSE;

	if( mCount == 0 )
		return FALSE;

	mCount--;

	MSG_DWORD2 msg;

	msg.Category = MP_SKILL;
	msg.Protocol = MP_SKILL_COUNT_NOTIFY;
	msg.dwData1 = mSkillObjectInfo.skillObjectId;
	msg.dwData2 = mCount;
	
	mpTarget->GetData()->SendMsg( &msg, sizeof( msg ) );
	
	if( mCount == 0 )
	{
		SetEndState();
		EndState();
	}

	// 080716 LUJ, 상태에 따른 로그
	switch( mStatusKind )
	{
		// 080716 LUJ, 경험치 보호 적용 시 로그 저장. 사용하지 않는 Ability Point를 회수를 저장하는데 쓴다
	case eStatusKind_ProtectExp:
		{
			CPlayer* player = ( CPlayer* )mpTarget->GetData();

			if( !	player ||
					player->GetObjectKind() != eObjectKind_Player )
			{
				break;
			}

			InsertLogExp(
				eExpLog_ProtectExp,
				player->GetID(),
				player->GetLevel(),
				0,
				player->GetPlayerExpPoint(),
				0,
				0,
				mCount );
			break;
		}
	}

	return TRUE;
}

// 080616 LUJ, 버프 중에 이벤트를 처리한다
void cBuffSkillObject::Execute( const CEvent& event )
{
	const BUFF_SKILL_INFO* skillInfo = ( ( cBuffSkillInfo* )GetSkillInfo() )->GetSkillInfo();

	if( ! skillInfo )
	{
		return;
	}

	CObject* object	= GetOperator();

	if( ! object )
	{
		return;
	}

	struct
	{
		float operator() ( const BUFF_SKILL_INFO& info, float value ) const
		{
			switch( info.StatusDataType )
			{
			case BUFF_SKILL_INFO::StatusTypeAdd:
				{
					return max( 0, value + info.StatusData );
				}
			case BUFF_SKILL_INFO::StatusTypePercent:
				{
					// 080708 LUJ, 퍼센트 적용 시 항상 100%를 추가로 더하던 문제 수정
					return max( 0, value * info.StatusData / 100.0f );
				}
			}

			return 0;
		}
	}
	GetValue;

	switch( event.GetType() )
	{
		// 080616 LUJ, 다른 오브젝트에게 피해를 줬을 때 발동되는 이벤트
	case CEvent::TypeGiveDamage:
		{
			CGiveDamageEvent&	giveDamageEvent	= *( ( CGiveDamageEvent* )&event );
			const RESULTINFO&	result			= giveDamageEvent.GetResult();
			
			switch( skillInfo->Status )
			{
				// 080616 LUJ, 준 데미지를 HP로 바꾸자
			case eStatusKind_ConvertHealthWithGivedDamage:
				{
					CCharacterCalcManager::StartUpdateLife(
						object,
						DWORD( GetValue( *skillInfo, float( result.RealDamage ) ) ),
						1,
						0 );

					// 080616 LUJ, 몬스터 HP 변경 정보가 전송되지 않으므로 직접 전송한다
					if( object->GetObjectKind() == eObjectKind_Monster )
					{
						MSG_DWORD3 message;
						ZeroMemory( &message, sizeof( message ) );
						message.Category	= MP_BOSSMONSTER;
						message.Protocol	= MP_FIELD_LIFE_NOTIFY;
						message.dwData1		= object->GetLife();
						message.dwData2		= ( ( CMonster* )object )->GetMonsterKind();
						message.dwData3		= GetID();

						PACKEDDATA_OBJ->QuickSend( this, &message, sizeof( message ) );
					}
					break;
				}
				// 080616 LUJ, 준 데미지를 MP로 바꾸자
			case eStatusKind_ConvertManaWithGivedDamage:
				{
					if( eObjectKind_Player != object->GetObjectKind() )
					{
						break;
					}

					// 080616 LUJ, 몬스터 HP 변경 정보가 전송되지 않으므로 직접 전송한다
					CCharacterCalcManager::StartUpdateMana(
						( CPlayer* )object,
						DWORD( GetValue( *skillInfo, float( result.RealDamage ) ) ),
						1,
						0 );
					break;
				}
			}
			
			break;
		}
		// 080616 LUJ, 다른 오브젝트가 피해를 받았을 때 발동되는 이벤트
	case CEvent::TypeTakeDamage:
		{
			CTakeDamageEvent&	takeDamageEvent	= *( ( CTakeDamageEvent* )&event );
			const RESULTINFO&	result			= takeDamageEvent.GetResult();

			switch( skillInfo->Status )
			{
				// 080616 LUJ, 입은 데미지를 반사하자
			case eStatusKind_Reflect:
				{
					CObject* attacker = takeDamageEvent.GetAttacker();

					if( !	attacker ||
						!	result.RealDamage )
					{
						break;
					}

					const DWORD previousAttackerLife = attacker->GetLife();

					// 080616 LUJ, 피해는 RESULTINFO.RealDamage에 따라 결정된다
					RESULTINFO	damageResult( result );
					damageResult.RealDamage	= DWORD( GetValue( *skillInfo, float( result.RealDamage ) ) );
					attacker->Damage( object, &damageResult );

					switch( attacker->GetObjectKind() )
					{
					case eObjectKind_Monster:
					case eObjectKind_BossMonster:
					case eObjectKind_FieldBossMonster:
					case eObjectKind_FieldSubMonster:
						{
							if( attacker->GetLife() )
							{
								// 080616 LUJ, 몬스터 HP 변경 정보가 전송되지 않으므로 직접 전송한다
								MSG_DWORD3 message;
								ZeroMemory( &message, sizeof( message ) );
								message.Category	= MP_BOSSMONSTER;
								message.Protocol	= MP_FIELD_LIFE_NOTIFY;
								message.dwData1		= attacker->GetLife();
								message.dwData2		= ( ( CMonster* )attacker )->GetMonsterKind();
								message.dwData3		= attacker->GetID();

								PACKEDDATA_OBJ->QuickSend( this, &message, sizeof( message ) );
							}
							else
							{
								// 080616 LUJ, 반사 데미지로 사망한 경우, 자연스럽게 처리하기 위해서 아래 메시지를 전송해야 한다
								//				그렇지 않으면 갑자기 푹 사라진다
								ATTACKMGR->sendDieMsg( object, attacker );

								attacker->Die( object );
							}

							break;
						}
					}

					break;
				}
				// 080616 LUJ, 받은 데이지를 바꾸자
			case eStatusKind_Absorb:
				{
					// 080616 LUJ, 기획 미비. 미구현
					break;
				}
				// 080616 LUJ, 받은 데미지를 HP로 바꾸자
			case eStatusKind_DamageToLife:
				{
					CObject* attacker = takeDamageEvent.GetAttacker();

					if( ! attacker )
					{
						break;
					}

					CCharacterCalcManager::StartUpdateLife(
						( CPlayer* )object,
						DWORD( GetValue( *skillInfo, float( result.RealDamage ) ) ),
						1,
						0 );
					
					// 080616 LUJ, 몬스터 HP 변경 정보가 전송되지 않으므로 직접 전송한다
					if( object->GetObjectKind() == eObjectKind_Monster )
					{
						MSG_DWORD3 message;
						ZeroMemory( &message, sizeof( message ) );
						message.Category	= MP_BOSSMONSTER;
						message.Protocol	= MP_FIELD_LIFE_NOTIFY;
						message.dwData1		= object->GetLife();
						message.dwData2		= ( ( CMonster* )object )->GetMonsterKind();
						message.dwData3		= GetID();

						PACKEDDATA_OBJ->QuickSend( this, &message, sizeof( message ) );
					}

					break;
				}
				// 080616 LUJ, 받은 데미지를 MP로 바꾸자
			case eStatusKind_DamageToMana:
				{
					if( eObjectKind_Player != object->GetObjectKind() )
					{
						break;
					}

					CCharacterCalcManager::StartUpdateMana(
						( CPlayer* )object,
						DWORD( GetValue( *skillInfo, float( result.RealDamage ) ) ),
						1,
						0 );
					break;
				}
			}

			break;
		}
		// 080616 LUJ, 다른 오브젝트에게 피살될 때 발생하는 이벤트
	case CEvent::TypeDie:
		{
			CDieEvent& dieEvent	= *( ( CDieEvent* )&event );

			switch( skillInfo->Status )
			{
			case eStatusKind_EventSkillToVictim:
				{
					// 080708 LUJ, 피해자 오브젝트를 이벤트 클래스에서 획득하도록 함
					CObject* victim = dieEvent.GetVictim();

					if( ! victim )
					{
						break;
					}

					SKILLMGR->AddActiveSkill(
						*victim,
						skillInfo->mEventSkillIndex );
					
					break;
				}
			case eStatusKind_EventSkillToKiller:
				{
					CObject* killer = dieEvent.GetKiller();

					if( ! killer )
					{
						break;
					}

					SKILLMGR->AddActiveSkill(
						*killer,
						skillInfo->mEventSkillIndex );
					break;
				}
			}

			break;
		}
		// 080616 LUJ, 다른 오브젝트를 살해할 때 발생하는 이벤트
	case CEvent::TypeKill:
		{
			CKillEvent& killEvent = *( ( CKillEvent* )&event );

			switch( skillInfo->Status )
			{
			case eStatusKind_EventSkillToVictim:
				{
					CObject* victim = killEvent.GetVictim();

					if( ! victim )
					{
						break;
					}

					SKILLMGR->AddActiveSkill(
						*victim,
						skillInfo->mEventSkillIndex );

					break;
				}
			case eStatusKind_EventSkillToKiller:
				{
					// 080708 LUJ, 살인자 오브젝트를 이벤트 클래스에서 획득하도록 함
					CObject* killer = killEvent.GetKiller();

					if( ! killer )
					{
						break;
					}

					SKILLMGR->AddActiveSkill(
						*killer,
						skillInfo->mEventSkillIndex );

					break;
				}
			}

			break;
		}
	}
}

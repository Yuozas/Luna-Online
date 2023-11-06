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
	// 080602 LUJ, ������Ʈ Ÿ���� �ʱ�ȭ�Ѵ�
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
	/// cSkillObject�� Init�Լ� ȣ��
	cSkillObject::Init( pInfo );

	DWORD idx = 0;

	/// ���� ��ų ����
	for( WORD i = 0; i < MAX_BUFF_COUNT; i++ )
	{
		idx = ( ( cActiveSkillInfo* )mpSkillInfo )->GetSkillInfo()->Buff[ i ];

		if( idx )
		{
			mpBuffSkillInfo[i] = ( cBuffSkillInfo* )( SKILLMGR->GetSkillInfo( idx ) );
			mBuffRate[i] = ( ( cActiveSkillInfo* )mpSkillInfo )->GetSkillInfo()->BuffRate[ i ];

			// 080219 LUJ, �Ⱓ�� ��ų�� ��� ��� ��� �Ⱓ�� �������־�� �Ѵ�
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
	/// �÷��̾��� ���
	if( GetOperator()->GetObjectKind() == eObjectKind_Player )
	{
		/// �� ������ ����
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

	/// ���� ������ üũ
	DWORD casting = ( ( cActiveSkillInfo* )mpSkillInfo )->GetSkillInfo()->CastingTime;
	
	GetOperator()->CurCastingSkillID = GetID();

	if( casting )
	/// �ִٸ�
	{
		/// ���� ����
		CastingStart();
	}
	/// ���ٸ�
	{
		/// �ߵ� ����
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
	/// ������
	case SKILL_STATE_CASTING:
		{
			CastingState();
		}
		break;
	/// �ߵ���
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

	/// ���� ��ų ����
	CObject* pTarget = NULL;
	cBuffSkillObject* pBuff = NULL;

	/// ��ų ȿ�� �ߵ�
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
		// 071203 KTH --- BuffStatus���� DecreaseManaRate ��ŭ�� ���� �Ҹ��� �ٿ��ش�.
		DWORD NeedMana = DWORD( float( pOperator->GetMana() ) * info->mMana.mPercent + info->mMana.mPlus );
		NeedMana = (DWORD) (NeedMana + ( NeedMana * (pOperator->GetRateBuffStatus()->DecreaseManaRate / 100 ) ) );
		pOperator->ReduceMana( NeedMana );
		//---
		
		// 080710 LUJ, ��ų ��� �� HP�� �Ҹ��ϵ��� ��
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
		// 071203 KTH --- BuffStatus���� DecreaseManaRate ��ŭ�� ���� �Ҹ��� �ٿ��ش�.
		DWORD NeedMana = DWORD( float( pOperator->GetMana() ) * info->mMana.mPercent + info->mMana.mPlus );
		NeedMana = (DWORD) (NeedMana + ( NeedMana * (pOperator->GetRateBuffStatus()->DecreaseManaRate / 100 ) ) );
		pOperator->ReduceMana( NeedMana );
		//---
		
		// 080710 LUJ, ��ų ��� �� HP�� �Ҹ��ϵ��� ��
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
		/// ���� ��ų ������ ������ �н�
		if( !mpBuffSkillInfo[ i ] )
		{
			continue;
		}

		WORD rate = (WORD)(rand() % 100);
		if( mBuffRate[i] <= rate )
		{
			continue;
		}

		/// Ÿ�� ����
		/// Single Target�� ���
		if( mpTarget->GetKind() < TARGET_KIND_MULTI )
		{
			pTarget = mpTarget->GetData();

			if( pTarget )
			{
				if( pTarget->GetState()  == eObjectState_Die )
				{
					return;
				}

				/// �⺻ ���� ����
				memcpy( &( mBuffSkillObjectInfo[ i ] ), &mSkillObjectInfo, sizeof( sSKILL_CREATE_INFO ) );

				pBuff = ( cBuffSkillObject* )( SKILLMGR->OnBuffSkillStart( mpBuffSkillInfo[i], &( mBuffSkillObjectInfo[ i ] ) ) );

				if( pBuff )
				{
					AddBuffSkillObject( pBuff );

					// 080708 LUJ, �̺�Ʈ�� �ʿ��� ��ų�� �÷��̾�� ����س��´�
					pTarget->AddEventSkill( *pBuff );
				}
			}
		}
		/// Multi Target�� ���
		else
		{
			mpTarget->SetPositionHead();
			while( pTarget = mpTarget->GetData() )
			{
				if( pTarget->GetState()  == eObjectState_Die )
				{
					continue;
				}

				/// �⺻ ���� ����
				memcpy( &( mBuffSkillObjectInfo[ i ] ), &mSkillObjectInfo, sizeof( sSKILL_CREATE_INFO ) );

				mBuffSkillObjectInfo[ i ].mainTarget.SetMainTarget( pTarget->GetID() );

				pBuff = ( cBuffSkillObject* )( SKILLMGR->OnBuffSkillStart( mpBuffSkillInfo[i], &( mBuffSkillObjectInfo[ i ] ) ) );

				if( pBuff )
				{
					AddBuffSkillObject( pBuff );

					// 080708 LUJ, �̺�Ʈ�� �ʿ��� ��ų�� �÷��̾�� ����س��´�
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
	/// ��ų ���� �ʱ�ȭ
	mState = SKILL_STATE_CASTING;
}

void cActiveSkillObject::CastingState()
{
	/// ���� ������ üũ
	if( !mpDelay[ DELAY_KIND_CASTING ]->Check() )
	{
		/// �ߵ� ����
		ProcessStart();
	}
}

void cActiveSkillObject::ProcessStart()
{
	/// �ߵ� ������ üũ
	WORD type = ( ( cActiveSkillInfo* )mpSkillInfo )->GetSkillInfo()->DelayType;

	switch( type )
	{
		/// �ߵ� ������ ����
	case DELAY_NONE:
		{
			/// ��ų ���� ���·� ����
			mState = SKILL_STATE_END;

			/// ��ų ����
			Excute();
		}
		break;
		/// �ð���
	case DELAY_TIME:
		{
			/// ������ ����
			DWORD delay = ( ( cActiveSkillInfo* )mpSkillInfo )->GetSkillInfo()->DelayTime;

			delay = ( DWORD )( delay / mAttackSpeedRate );
			mpDelay[ DELAY_KIND_PROCESS ]->Init( delay );
			mpDelay[ DELAY_KIND_PROCESS ]->Start();

			/// �ߵ��� ���·� ����
			mState = SKILL_STATE_PROCESS;
		}
		break;
		/// �̵���
	case DELAY_SCOPE:
		{
			/// �̵��ӵ�
			DWORD speed = ( ( cActiveSkillInfo* )mpSkillInfo )->GetSkillInfo()->DelayTime;

			/// ������ ��ġ Ÿ�� ������ �Ÿ� ���
			VECTOR3 oper;
			VECTOR3 tar;

			/// ������ ��ġ
			GetOperator()->GetPosition( &oper );

			/// Ÿ�� ��ġ
			tar.x = mSkillObjectInfo.mainTarget.cpTargetPos.wx;
			tar.z = mSkillObjectInfo.mainTarget.cpTargetPos.wz;

			/// �Ÿ� ���
			DWORD distance = ( DWORD )CalcDistanceXZ( &oper, &tar );

			/// �ð� ���
			DWORD delay = ( distance / speed ) * 1000;

			mpDelay[ DELAY_KIND_PROCESS ]->Init( delay );
			mpDelay[ DELAY_KIND_PROCESS ]->Start();

			/// �ߵ��� ���·� ����
			mState = SKILL_STATE_PROCESS;
		}
		break;
	}
}

void cActiveSkillObject::ProcessState()
{
	/// �ߵ� ������ üũ
	if( !mpDelay[ DELAY_KIND_PROCESS ]->Check() )
	{
		/// ����
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

// 080602 LUJ, �ش� ������Ʈ�� �ߵ��� ���� ��ų�� ��� �����Ѵ�
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

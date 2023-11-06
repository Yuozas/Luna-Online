#include "stdafx.h"
#include ".\attackskillunit.h"

#include "../Target/Target.h"
#include "../Object/SkillObject.h"
#include "../Info/ActiveSkillInfo.h"

#include "AttackManager.h"
#include "PackedData.h"
// 080616 LUJ, �̺�Ʈ �߻� �ÿ� ��ų�� �׼��� ���ϱ� ���� ����
#include "Event.h"

cAttackSkillUnit::cAttackSkillUnit( cSkillObject* pParentSkill, cTarget* pTarget )
: cSkillUnit( pParentSkill, pTarget )
{
}

cAttackSkillUnit::~cAttackSkillUnit(void)
{
}

BOOL cAttackSkillUnit::Excute()
{
	CObject*	pTarget = NULL;
	RESULTINFO	damage;
	MSG_SKILL_RESULT msg;

	msg.InitMsg( mpParentSkill->GetID() );
	msg.SkillDamageKind = false;

	CTargetListIterator iter(&msg.TargetList);

	CObject* skillOperator = mpParentSkill->GetOperator();

	// 080616 LUJ, ��ų�� �ߵ��� ��ü�� ���ٸ� ó�� ������ ����
	if( ! skillOperator )
	{
		return FALSE;
	}

	if( mUnitType == UNITKIND_PHYSIC_ATTCK )
	{
		/// Single Target�� ���
		if( mpTarget->GetKind() < TARGET_KIND_MULTI )
		{
			pTarget = mpTarget->GetData();

			if(!pTarget)
				return FALSE;

			if( pTarget->GetState()  == eObjectState_Die )
			{
				return FALSE;
			}

			ATTACKMGR->PhysicAttack( skillOperator, pTarget, &damage, mAccuracy, mAddDamage, mAddType );

			iter.AddTargetWithResultInfo( pTarget->GetID(), 1, &damage);
			iter.Release();

			PACKEDDATA_OBJ->QuickSend( pTarget, &msg, msg.GetMsgLength() );

			// 080616 LUJ, ��ų �̺�Ʈ�� ó���Ѵ�
			skillOperator->Execute( CGiveDamageEvent( pTarget, damage ) );
			pTarget->Execute( CTakeDamageEvent( skillOperator, damage ) );
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
				
				ATTACKMGR->PhysicAttack( skillOperator, pTarget, &damage, mAccuracy, mAddDamage, mAddType );

				iter.AddTargetWithResultInfo( pTarget->GetID(), 1, &damage);

				// 080616 LUJ, ��ų �̺�Ʈ�� ó���Ѵ�
				skillOperator->Execute( CGiveDamageEvent( pTarget, damage ) );
				pTarget->Execute( CTakeDamageEvent( skillOperator, damage ) );
			}
			iter.Release();

			// 080715 LUJ, mpParentSkill->GetOperator() ���� 0�� �Ǵ� ��찡 ����. �����ص� ���� ����ϵ��� ��
			PACKEDDATA_OBJ->QuickSend( skillOperator, &msg, msg.GetMsgLength() );
		}
	}
	else if( mUnitType == UNITKIND_MAGIC_ATTCK )
	{
		/// Single Target�� ���
		if( mpTarget->GetKind() < TARGET_KIND_MULTI )
		{
			pTarget = mpTarget->GetData();
			
			if(!pTarget)
				return FALSE;

			if( pTarget->GetState()  == eObjectState_Die )
			{
				return FALSE;
			}

			ATTACKMGR->MagicAttack( skillOperator, pTarget, &damage, mAccuracy, mAddDamage, mAddType );

			iter.AddTargetWithResultInfo( pTarget->GetID(), 1, &damage);
			iter.Release();

			PACKEDDATA_OBJ->QuickSend( pTarget, &msg, msg.GetMsgLength() );

			// 080616 LUJ, ��ų �̺�Ʈ�� ó���Ѵ�
			skillOperator->Execute( CGiveDamageEvent( pTarget, damage ) );
			pTarget->Execute( CTakeDamageEvent( skillOperator, damage ) );
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

				ATTACKMGR->MagicAttack( skillOperator, pTarget, &damage, mAccuracy, mAddDamage, mAddType );

				iter.AddTargetWithResultInfo( pTarget->GetID(), 1, &damage);

				// 080616 LUJ, ��ų �̺�Ʈ�� ó���Ѵ�
				skillOperator->Execute( CGiveDamageEvent( pTarget, damage ) );
				pTarget->Execute( CTakeDamageEvent( skillOperator, damage ) );
			}
			iter.Release();

			PACKEDDATA_OBJ->QuickSend( mpParentSkill->GetOperator(), &msg, msg.GetMsgLength() );
		}
	}

	return TRUE;
}

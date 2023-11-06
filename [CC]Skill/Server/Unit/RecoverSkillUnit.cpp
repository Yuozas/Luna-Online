#include "stdafx.h"
#include ".\recoverskillunit.h"

#include "../Target/Target.h"
#include "../Object/SkillObject.h"
#include "../Info/ActiveSkillInfo.h"

#include "AttackManager.h"
#include "PackedData.h"

cRecoverSkillUnit::cRecoverSkillUnit( cSkillObject* pParentSkill, cTarget* pTarget )
: cSkillUnit( pParentSkill, pTarget )
{
}

cRecoverSkillUnit::~cRecoverSkillUnit(void)
{
}

BOOL cRecoverSkillUnit::Excute()
{
	CObject*	pTarget = NULL;
	RESULTINFO	resultinfo;
	MSG_SKILL_RESULT msg;

	msg.InitMsg( mpParentSkill->GetID() );
	msg.SkillDamageKind = false;

	CTargetListIterator iter(&msg.TargetList);

	/// Single Target인 경우
	if( mpTarget->GetKind() < TARGET_KIND_MULTI )
	{
		pTarget = mpTarget->GetData();

		if(!pTarget)
			return FALSE;

		if( pTarget->GetState()  == eObjectState_Die )
		{
			return FALSE;
		}

		resultinfo.Clear();

		if( pTarget->GetObjectKind() != eObjectKind_Monster )
		{
			if( mUnitType == UNITKIND_LIFE_RECOVER )
			{
				ATTACKMGR->RecoverLife(
					mpParentSkill->GetOperator(),
					pTarget,
					0,
					mAddDamage,
					&resultinfo);
			}
			else if( mUnitType == UNITKIND_MANA_RECOVER )
			{
				ATTACKMGR->RecoverMana(
					mpParentSkill->GetOperator(),
					pTarget,
					0,
					mAddDamage,
					&resultinfo);
			}
		}
		iter.AddTargetWithResultInfo( pTarget->GetID(), 1, &resultinfo);
		iter.Release();

		PACKEDDATA_OBJ->QuickSend( pTarget, &msg, msg.GetMsgLength() );
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

			resultinfo.Clear();

			if( mUnitType == UNITKIND_LIFE_RECOVER )
			{
				ATTACKMGR->RecoverLife(
					mpParentSkill->GetOperator(),
					pTarget,
					0,
					mAddDamage,
					&resultinfo);
			}
			else if( mUnitType == UNITKIND_MANA_RECOVER )
			{
				ATTACKMGR->RecoverMana(
					mpParentSkill->GetOperator(),
					pTarget,
					0,
					mAddDamage,
					&resultinfo);
			}

			iter.AddTargetWithResultInfo( pTarget->GetID(), 1, &resultinfo);

		}
		iter.Release();

		PACKEDDATA_OBJ->QuickSend( mpParentSkill->GetOperator(), &msg, msg.GetMsgLength() );
	}

	return TRUE;
}
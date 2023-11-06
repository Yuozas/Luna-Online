#include "stdafx.h"
#include ".\cdebuffskillunit.h"
#include "../Object/BuffSkillObject.h"
#include "../Info/BuffSkillInfo.h"
#include "../Target/Target.h"

cDeBuffSkillUnit::cDeBuffSkillUnit( cSkillObject* pParentSkill, cTarget* pTarget )
: cSkillUnit( pParentSkill, pTarget )
{
}

cDeBuffSkillUnit::~cDeBuffSkillUnit(void)
{
}

BOOL cDeBuffSkillUnit::Excute()
{
	CObject*	pTarget = NULL;
	CYHHashTable<cSkillObject>* pBuffList = NULL;
	cBuffSkillObject* pBuffSkill = NULL;
	cBuffSkillInfo* pSkillInfo = NULL;
	WORD ParentLevel = mpParentSkill->GetSkillInfo()->GetLevel() * 3;
	WORD TargetLevel = 0;

	if( mpTarget->GetKind() < TARGET_KIND_MULTI )
	{
		pTarget = mpTarget->GetData();

		if(!pTarget)
			return FALSE;
		
		if( pTarget->GetState()  == eObjectState_Die )
		{
			return FALSE;
		}

		pBuffList = pTarget->GetBuffSkillList();

		pBuffList->SetPositionHead();
		while( pBuffSkill = ( cBuffSkillObject* )( pBuffList->GetData() ) )
		{
			pSkillInfo = ( cBuffSkillInfo* )( pBuffSkill->GetSkillInfo() );

			if( pSkillInfo->GetSkillInfo()->Status == mAddDamage )
			{
				TargetLevel = pSkillInfo->GetLevel();

				if( ParentLevel >= TargetLevel )
				{
					pBuffSkill->SetEndState();
					pBuffSkill->EndState();
				}
			}
		}
	}
	/// Multi TargetÀÎ °æ¿ì
	else
	{
		mpTarget->SetPositionHead();
		while( pTarget = mpTarget->GetData() )
		{
			if( pTarget->GetState()  == eObjectState_Die )
			{
				continue;
			}

			pBuffList = pTarget->GetBuffSkillList();

			pBuffList->SetPositionHead();
			while( pBuffSkill = ( cBuffSkillObject* )( pBuffList->GetData() ) )
			{
				pSkillInfo = ( cBuffSkillInfo* )( pBuffSkill->GetSkillInfo() );

				if( pSkillInfo->GetSkillInfo()->Status == mAddDamage )
				{
					TargetLevel = pSkillInfo->GetLevel();

					if( ParentLevel >= TargetLevel )
					{
						pBuffSkill->SetEndState();
						pBuffSkill->EndState();
					}
				}
			}
		}
	}
	return TRUE;
}
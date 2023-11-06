#include "stdafx.h"
#include ".\skillobjectfactory.h"
#include "../Info/SkillInfo.h"
#include "../Info/ActiveSkillInfo.h"
#include "../Info/BuffSkillInfo.h"
#include "../Object/SkillObject.h"
#include "../Object/ActiveSkillObject.h"
#include "../Object/OnOffSkillObject.h"
#include "../Object/BuffSkillObject.h"
#include "../Delay/Delay.h"
#include "../Target/Target.h"
#include "../Target/SingleTarget.h"
#include "../Target/CircleSingleTarget.h"
#include "../Target/MultiTarget.h"
#include "../Target/CircleMultiTarget.h"
#include "../Target/RectMultiTarget.h"
#include "../Target/ArcMultiTarget.h"
#include "../Unit/SkillUnit.h"
#include "../Unit/AttackSkillUnit.h"
#include "../Unit/RecoverSkillUnit.h"
#include "../Unit/cDeBuffSkillUnit.h"
#include "../Unit/cSpecialSkillUnit.h"
#include "../Status/Status.h"
// 080616 LUJ, ���� ��ų ���� �߰�
#include "../Unit/TrapSkillUnit.h"

cSkillObjectFactory::cSkillObjectFactory( void )
{
}

cSkillObjectFactory::~cSkillObjectFactory( void )
{
}

cSkillObject* cSkillObjectFactory::CreateSkillObject( cSkillInfo* pSkillInfo )
{
	cTarget*		target = NULL;
	cSkillObject*	skill = NULL;

	/// ��ų ���� �Ǵ�
	WORD SkillKind = pSkillInfo->GetKind();
	
	/// ���� ��ų�� �ƴѰ��
	if( SkillKind != SKILLKIND_BUFF )
	{
		/// Ÿ�� �Ӽ��� �´� Ÿ�� ����
		ACTIVE_SKILL_INFO* pInfo = ( ( cActiveSkillInfo* )pSkillInfo )->GetSkillInfo();

		/// Ÿ�� ���� - ���߿� �Լ������ͷ� �ٲ㺸��
		switch( pInfo->Area )
		{
		case TARGET_KIND_SINGLE:
			{
				target = ( cTarget* )( new cSingleTarget );
			}
			break;
		case TARGET_KIND_SINGLE_CIRCLE:
			{
				target = ( cTarget* )( new cCircleSingleTarget );
			}
			break;
		case TARGET_KIND_MULTI_CIRCLE:
			{
				target = ( cTarget* )( new cCircleMultiTarget );
			}
			break;
		case TARGET_KIND_MULTI_RECT:
			{
				target = ( cTarget* )( new cRectMultiTarget );
			}
			break;
		case TARGET_KIND_MULTI_ARC:
			{
				target = ( cTarget* )( new cArcMultiTarget );
			}
			break;
		case TARGET_KIND_MULTI:
		// 080602 LUJ, ������ ��� ��ų
		case TARGET_KIND_WORLD:
		// 080708 LUJ, ���� ��� ���� ��ų
		case TARGET_KIND_MULTI_MONSTER:
			{
				target = ( cTarget* )( new cMultiTarget );
			}
			break;
		}
	}
	else
	{
		/// ���� ��ų�� ���� 1�� Ÿ�ٸ� ����
		target = ( cTarget* )( new cSingleTarget );
	}
	/// ��ų ������Ʈ ���� - ���߿� �Լ������ͷ� �ٲ㺸��
	switch( SkillKind )
	{
	case SKILLKIND_PHYSIC:
	case SKILLKIND_MAGIC:
	case SKILLKIND_ONOFF:
		{
			skill = CreateActiveSkillObject( pSkillInfo, target );
		}
		break;
	case SKILLKIND_BUFF:
		{
			skill = CreateBuffSkillObject( pSkillInfo, target );
		}
		break;
//	case SKILLKIND_ONOFF:
//		{
//			skill = CreateOnOffSkillObject( pSkillInfo, target );
//		}
//		break;
	}

	return skill;
}

cSkillObject* cSkillObjectFactory::CreateActiveSkillObject( cSkillInfo* pSkillInfo, cTarget* pTarget )
{
	cSkillUnit*			unit = NULL;
	cActiveSkillObject*	skill = NULL;

	skill = new cActiveSkillObject( pSkillInfo, pTarget );

	ACTIVE_SKILL_INFO* pInfo = ( ( cActiveSkillInfo* )pSkillInfo )->GetSkillInfo();

	switch( pInfo->Unit )
	{
	case UNITKIND_PHYSIC_ATTCK:
	case UNITKIND_MAGIC_ATTCK:
		{
			unit = new cAttackSkillUnit( skill, pTarget );
		}
		break;
	case UNITKIND_LIFE_RECOVER:
	case UNITKIND_MANA_RECOVER:
		{
			unit = new cRecoverSkillUnit( skill, pTarget );
		}
		break;
	case UNITKIND_DEBUFF:
		{
			unit = new cDeBuffSkillUnit( skill, pTarget );
		}
		break;
	case UNITKIND_RESURRECTION:
	case UNITKIND_FAKE_DAMAGE:
	case UNITKIND_DECOY:
	case UNITKIND_RETURN:
	case UNITKIND_SPELLBREAK:
		{
			unit = new cSpecialSkillUnit( skill, pTarget );
		}
		break;
		// 080616 LUJ, ���� ��ų
	case UNITKIND_TRAP:
		{
			unit = new cTrapSkillUnit( skill, pTarget );
		}
		break;
	}

	if( unit )
	{
		unit->Init();
	}
	skill->AddSkillUnit( unit );

	return (cSkillObject*)skill;
}

cSkillObject* cSkillObjectFactory::CreateOnOffSkillObject( cSkillInfo* pSkillInfo, cTarget* pTarget )
{
	cSkillUnit*			unit = NULL;
	cOnOffSkillObject*	skill = NULL;

	skill = new cOnOffSkillObject( pSkillInfo, pTarget );
	
	skill->AddSkillUnit( unit );

	return (cSkillObject*)skill;
}

cSkillObject* cSkillObjectFactory::CreateBuffSkillObject( cSkillInfo* pSkillInfo, cTarget* pTarget )
{
	cBuffSkillObject*	skill = NULL;

	skill = new cBuffSkillObject( pSkillInfo, pTarget );

	BUFF_SKILL_INFO* pInfo = ( ( cBuffSkillInfo* )pSkillInfo )->GetSkillInfo();
	
	return (cSkillObject*)skill;
}

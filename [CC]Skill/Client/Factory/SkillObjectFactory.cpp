#include "stdafx.h"
#include ".\skillobjectfactory.h"
#include "../Info/SkillInfo.h"
#include "../Object/SkillObject.h"
#include "../Object/BuffSkillObject.h"
#include "../Delay/Delay.h"
#include "../Status/Status.h"

cSkillObjectFactory::cSkillObjectFactory(void)
{
}

cSkillObjectFactory::~cSkillObjectFactory(void)
{
}

cSkillObject* cSkillObjectFactory::CreateSkillObject(cSkillInfo* pSkillInfo)
{
	cSkillObject*	skill = NULL;

	/// 스킬 오브젝트 생성 - 함수포인터로 바꿔보자
	switch( pSkillInfo->GetKind() )
	{
	case SKILLKIND_BUFF:
		{
			skill = CreateBuffSkillObject( pSkillInfo );
		}
		break;
	case SKILLKIND_PASSIVE:
		{
			return NULL;
		}
		break;
	default:
		{
			skill = CreateNonBuffSkillObject( pSkillInfo );
		}
		break;
	}

	return skill;
}

cSkillObject* cSkillObjectFactory::CreateNonBuffSkillObject( cSkillInfo* pSkillInfo )
{
	cSkillObject*	skill = NULL;

	skill = new cSkillObject( pSkillInfo );

	return skill;
}

cSkillObject* cSkillObjectFactory::CreateBuffSkillObject( cSkillInfo* pSkillInfo )
{
	cStatus*			status = NULL;
	cBuffSkillObject*	skill = NULL;

	skill = new cBuffSkillObject( pSkillInfo );

	return (cSkillObject*)skill;
}

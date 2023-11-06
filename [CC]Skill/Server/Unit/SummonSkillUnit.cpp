#include "StdAfx.h"
#include "SummonSkillUnit.h"
#include "..\Object\ActiveSkillObject.h"
#include "..\Target\Target.h"
#include "AISystem.h"
#include "RegenManager.h"
#include "Monster.h"
#include "StateMachinen.h"
#include "..\[CC]Header\GameResourceManager.h"

cSummonSkillUnit::cSummonSkillUnit(cActiveSkillObject& skillObject) :
cSkillUnit(skillObject)
{}

cSummonSkillUnit::~cSummonSkillUnit(void)
{}

BOOL cSummonSkillUnit::Excute()
{
	CObject* const ownerObject = mpParentSkill->GetOperator();

	if(0 == ownerObject)
	{
		return FALSE;
	}

	const int totalSize = mpParentSkill->GetInfo().UnitDataType;
	const int summonedSize = ownerObject->GetSummondSize(mpParentSkill->GetInfo());

	if(summonedSize > totalSize)
	{
		return FALSE;
	}

	int summoningSize = totalSize - summonedSize;

	// �÷��̾�� 1���� ��ȯ�Ѵ�
	if(eObjectKind_Player == ownerObject->GetObjectKind())
	{		
		if(summonedSize == totalSize)
		{
			ownerObject->RemoveSummonedOldest(
				mpParentSkill->GetInfo());
		}

		summoningSize = 1;
	}

	const WORD summonMonsterKind = WORD(mpParentSkill->GetInfo().UnitData);
	const BASE_MONSTER_LIST* const monsterList = GAMERESRCMNGR->GetMonsterListInfo(
		summonMonsterKind);

	if(0 ==	monsterList)
	{
		return FALSE;
	}

	mpParentSkill->GetTarget().SetPositionHead();
	size_t targetSize = mpParentSkill->GetTarget().GetSize();

	while(CObject* const targetObject = mpParentSkill->GetTarget().GetData())
	{
		VECTOR3 targetPosition = {0};
		targetObject->GetPosition(
			&targetPosition);

		if(0 >= summoningSize)
		{
			break;
		}

		/*
		�� ��ų�� �������� �� ��츦 �����غ���. ��ȯ�� 5���� ��� 7���� �����Ϸ��� ��� �������ұ�. �Ǵ�
		��ȯ�� 11���� ��� 5���� �����Ϸ��� ��� �������ұ�.

		ceil()�� ����ؼ� �����̶� �Ҽ����� �߻��ϸ� 1�� ���ش�. �׻� �� ���߿��� 1�� ������ ������ ���
		������ �� ��������. ���� ������ ������ 0�� �ȴ�. �̷� ������ ���ڶ�� ���� �׻� ���� ������ �� �ִ�
		*/
		const int eachSummoningSize = int(ceil(float(summoningSize) / targetSize--));
		summoningSize = summoningSize - eachSummoningSize;

		for(int i = 0; i < eachSummoningSize; ++i)
		{
			const float randomRateX = float(rand()) / RAND_MAX;
			const float randomRateZ = float(rand()) / RAND_MAX;
			const float randomAxisX = (randomRateX < 0.5f ? -1.0f : 1.0f) * (ownerObject->GetRadius() * (1.0f + randomRateX));
			const float randomAxisZ = (randomRateZ < 0.5f ? -1.0f : 1.0f) * (ownerObject->GetRadius() * (1.0f + randomRateZ));

			VECTOR3 summonPosition = {0};
			summonPosition.x = targetPosition.x + randomAxisX;
			summonPosition.z = targetPosition.z + randomAxisZ;

			CMonster* const monster = REGENMGR->RegenObject(
				g_pAISystem.GeneraterMonsterID(),
				0,
				ownerObject->GetGridID(),
				monsterList->ObjectKind,
				monsterList->MonsterKind,
				&summonPosition,
				0,
				0,
				0,
				FALSE,
				TRUE,
				ownerObject->GetID());

			if(0 == monster)
			{
				return FALSE;
			}
			else if(TRUE == monster->SetTObject(ownerObject->GetTObject()))
			{
				GSTATEMACHINE.SetState(
					monster,
					eMA_PERSUIT);
			}

			ownerObject->AddSummoned(
				mpParentSkill->GetInfo(),
				monster->GetID());
		}
	}

	return TRUE;
}
/*********************************************************************

	 ����		: Animal.cpp
	 �ۼ���		: Shinobi
	 �ۼ���		: 2008/03/08

	 ���ϼ���	: ���� Ŭ������ �ҽ�

 *********************************************************************/

#include "stdafx.h"
#include "../Common/SHMath.h"

#include "Animal.h"

#if defined(_AGENTSERVER)
#elif defined(_MAPSERVER_)
	#include "../Farm/SHFarmManager.h"
	#include "MapDBMsgParser.h"
#else
	#include "../Farm/SHFarmManager.h"
	#include "ObjectManager.h"
	#include "GameIn.h"
	#include "MonsterGuageDlg.h"
	#include "ObjectBalloon.h"
	#include "OBalloonName.h"
	#include "ObjectGuagen.h"
#endif

DWORD CAnimal::ANIMAL_IN_STALL_TIME;															// ������ ��翡 �ִ� �ð�(��)
DWORD CAnimal::ANIMAL_LIFE_DECREASE_TIME;														// ������ ����� -1 �ð�
DWORD CAnimal::ANIMAL_RETRY_TIME;																// ������ ��翡 �ٽ� ��ȯ�ϴ� �� �ʿ��� �ð� (��)
DWORD CAnimal::ANIMAL_FEED_TIME;																// ���࿡�� ���̸� �ִ� �ð�
DWORD CAnimal::ANIMAL_FEED_DELAY_TIME;															// ���࿡�� �ٽ� ���̸� �ִµ� �ʿ��� �ð�
DWORD CAnimal::ANIMAL_CLEANING_TIME;															// ������ �ı�´� �ҿ�Ǵ� �ð� (��)
DWORD CAnimal::ANIMAL_CLEANING_RETRY_TIME;														// ������ �ٽ� �ı�´� �ɸ��� �ð� (��)
DWORD CAnimal::ANIMAL_DIE_DELAY_TIME = 24*60;													// ������ ������� 0�� �� �� �ױ������ ������ �ð� (��)
DWORD CAnimal::ANIMAL_STALL_REWARD_TIME;														// ���࿡�� �������� ��´� �ɸ��� �ð� (��)
DWORD CAnimal::ANIMAL_CONTENTMENT_DECREASE_TIME;												// ������ �������� -1 �Ǵ� �ð�
DWORD CAnimal::ANIMAL_INTEREST_DECREASE_TIME;													// ������ ���ɵ��� -1 �Ǵ� �ð�

UINT CAnimal::ANIMAL_STEP_MIN_LIFE[4][2] = {0,};
UINT CAnimal::ANIMAL_STEP_MAX_LIFE = 4;
UINT CAnimal::ANIMAL_STEP_MAX_CONTENTMENT = 100;
UINT CAnimal::ANIMAL_STEP_MAX_INTEREST = 100;
//UINT CAnimal::ANIMAL_BREED_PROBA[5][ANIMAL_BREED_KIND_MAX+2] = {0,};
UINT		ANIMAL_REWARD_LIST[CAnimal::ANIMAL_KIND_MAX] = {0,};
UINT CAnimal::ANIMAL_STEP_FEED_LIFE[ANIMAL_FEED_KIND_MAX][2] = {0,};
char CAnimal::ANIMAL_NAME_TBL[ANIMAL_KIND_MAX][256];

//----------------------------------------------------------------------------------------------------------------------------------------
// CAnimal Method																													������
//
CAnimal::CAnimal()
{
	SetAnimal(ANIMAL_RESULT_DIE);
}

//----------------------------------------------------------------------------------------------------------------------------------------
// ~CAnimal Method																													�ı���
//
CAnimal::~CAnimal()
{
}
//----------------------------------------------------------------------------------------------------------------------------------------
// MainLoop Method																										��� ���� ��ƾ ó��
//
VOID CAnimal::MainLoop()
{
	CSHFarmObj::MainLoop();
#if defined(_AGENTSERVER)
#elif defined(_MAPSERVER_)
	if(m_nLife > 0)
	{
		if( m_eStep != ANIMAL_STEP_COMPLETE )
		{
			// ������ �ð� üũ
			if( m_nContentment > 0 && gCurTime - m_nContentmentTimeTick > ANIMAL_CONTENTMENT_DECREASE_TIME )
			{
				m_nContentmentTimeTick = gCurTime;
				m_nBreedTimeTick = gCurTime;
				m_nContentment--;

				if( m_nContentment < 0 )
					m_nContentment = 0;

				AddEvent(NULL, ANIMAL_RESULT_CONTENTMENT);
			}
			// ���ɵ� �ð� üũ
			if( m_nInterest > 0 && gCurTime - m_nInterestTimeTick > ANIMAL_INTEREST_DECREASE_TIME )
			{
				m_nInterestTimeTick = gCurTime;
				m_nInterest--;

				if( m_nInterest < 0 )
					m_nInterest = 0;

				AddEvent(NULL, ANIMAL_RESULT_INTEREST);
			}
			// �������� 0�̰� ����� ���� �ð� üũ
			if( m_nContentment == 0 && gCurTime - m_nBreedTimeTick > ANIMAL_LIFE_DECREASE_TIME )
			{
				m_nBreedTimeTick = gCurTime;
				m_nLife--;

				// Ŭ���̾�Ʈ�� �˸��� ���� ����
				AddEvent(NULL, ANIMAL_RESULT_LIFE);

				if( m_nLife == 0 )
				{
					CPen* pAnimalCage = (CPen*)GetParent();
					CSHFarm* pFarm = (CSHFarm*)(pAnimalCage ? pAnimalCage->GetParent() : 0 );

					if( pFarm )
					{
						InsertLogFarmAnimal(pFarm, this, eLog_FamilyFarmDieAnimal);
					}

					AddEvent(NULL, ANIMAL_RESULT_DIE);
					SetAnimal(ANIMAL_RESULT_DIE);
				}
			}

			// ���� �ð��� �Ǹ� ���࿡�Լ� �������� ������ �ִ� ó�� �߰� �ʿ�.
		}
	}
	else
	{
		// ������ Ÿ�� ����
/*		CPen*		pPen		= (CPen*)GetParent();
		CSHFarm*	pFarm		= (CSHFarm*)(pPen ? pPen->GetParent() : 0 );

		if( pFarm )
		{
			// �α� �߰� �Ͽ�����.
			//InsertLogFarmAnimal(farm, this, eLog_FamolyFarmDieAnimal);
		}

		// Ŭ���̾�Ʈ�� �˸��� ���� ����
		AddEvent(NULL, ANIMAL_RESULT_DIE);
		SetAnimal(ANIMAL_RESULT_DIE);

		return;*/
	}
#else
	// Ŭ���̾�Ʈ������ ������ ��� �ִ� ������ ���� ó�� Step�� ANIMAL_STEP_1�� �ƴϱ� ������ ������ Ÿ�� �Ŀ� ����
	if( m_nLife > 0 )
	{
		if( m_eStep == ANIMAL_STEP_IN_STALL )
		{
			// ������ ��翡 �ִ� ���� �� ������ ǥ�ø� ����...
			CMonsterGuageDlg* pGuageDlg = GAMEIN->GetMonsterGuageDlg();
			if( pGuageDlg )
			{
				if( (CObject*)m_pcsParent->GetRenderObjEx(GetID()+5) == OBJECTMGR->GetSelectedObject())
				{
					pGuageDlg->SetMonsterLife(gCurTime-GetBreedTimeTick(), CAnimal::ANIMAL_IN_STALL_TIME);
				}
			}
			// �ִ� ��� ��
			if( gCurTime - m_nBreedTimeTick > ANIMAL_IN_STALL_TIME )
			{
				m_eStep = ANIMAL_STEP_1;
				AddEvent(NULL, ANIMAL_RESULT_IN_STALL);
			}
		}
		// ��Ȯ ��� �ð� << ������ �߰� �ؾ���. ���� ������ ��Ȯ�� �������� üũ �ϴ� �κ��� �ʿ���.
		else if(m_eStep == ANIMAL_STEP_COMPLETE)//( m_eStep > ANIMAL_STEP_IN_STALL && m_eStep <= ANIMAL_STEP_COMPLETE )
		{
			if( m_nBreedTimeTick )
			{
				if( gCurTime - m_nBreedTimeTick > ANIMAL_STALL_REWARD_TIME )
				{
					m_nBreedTimeTick = NULL;
					// ��Ȯ ��� ��
					AddEvent(NULL, ANIMAL_RESULT_REWARD);
				}
			}
		}
		else
		{
			// ���࿡ ��Ḧ �ְ� �ִ� �ð�
			if( GetFeedTimeTick() )
			{
				CMonsterGuageDlg* pGuageDlg = GAMEIN->GetMonsterGuageDlg();

				if( gCurTime - GetFeedTimeTick() > CAnimal::ANIMAL_FEED_TIME )
				{
					if( pGuageDlg )
					{
						if( (CObject*)GetRenderObj() == OBJECTMGR->GetSelectedObject() )
						{
							pGuageDlg->SetMonsterLife(GetLife(), GetMaxLife());
						}
					}
					SetFeedTimeTick(NULL);
					AddEvent(NULL, ANIMAL_RESULT_FEEDING);
				}
				else
				{
					if( pGuageDlg )
					{
						if( (CObject*)GetRenderObj() == OBJECTMGR->GetSelectedObject() )
						{
							int nDelta = ((GetLife()-GetBeforeLife())*(gCurTime-GetFeedTimeTick())<<8)/CAnimal::ANIMAL_FEED_TIME;
							pGuageDlg->SetMonsterLife((GetBeforeLife()<<8)+nDelta, GetMaxLife()<<8);
						}
					}
				}
			}
			// ��� û�Ҹ� �ϰ� �ִ� �ð�
		}
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
// Breed Method																													��� �ϱ�
//
VOID CAnimal::Breed(int nPlayerID, int nAnimalCageGrade, int nKind)
{
	m_nOwnerID = nPlayerID;
	// Ŭ���̾�Ʈ������ ��翡 �ִ� ������ ���� Step�� �ٸ��� �Ѵ�. ���� Ŭ���̾�Ʈ���� ������ ������ ����
#if defined(_MAPSERVER_)
	m_eStep = ANIMAL_STEP_1;
#else
	m_eStep = ANIMAL_STEP_IN_STALL;
#endif
	//CPen*		pPen		= (CPen*)GetParent();
	m_nKind = nKind;
	m_nBreedTimeTick = gCurTime;
	m_nContentmentTimeTick = gCurTime;
	m_nInterestTimeTick = gCurTime;
	m_nGrade = 0;
	m_nLife = ANIMAL_STEP_MAX_LIFE;
	// ��簡 ���׷��̵� 2�ܰ� �̻��̶�� ���̺� �ִ� ���� �����´�.
	if( nAnimalCageGrade > 2 )
	{
		m_nContentment	= ANIMAL_STEP_MIN_LIFE[nAnimalCageGrade/2][0];
		m_nInterest		= ANIMAL_STEP_MIN_LIFE[nAnimalCageGrade/2][1];
	}
	else
	{
		m_nContentment = 0;
		m_nInterest = 0;
	}
	//m_nLife = GetLifeFromTbl( ((CPen*)GetParent())->GetGrade() );
}

// -------------------------------------------------------------------------------------------------------------------------------------
// Feed Method																													��� �ֱ�
//
VOID CAnimal::Feed(int nGrade)
{
	int nPlusContentment = GetFeedContentmentFromTbl(nGrade);
	int nPlusInterest = GetFeedInterestFromTbl(nGrade);

	m_nBeforeContentment = m_nContentment;
	m_nBeforeInterest = m_nInterest;

	m_nContentment += nPlusContentment;
	m_nInterest += nPlusInterest;

	if( m_nContentment > 100 )
	{
		m_nContentment = 100;
	}

	if( m_nInterest > 100 )
	{
		m_nInterest = 100;
	}

	AddEvent(NULL, ANIMAL_RESULT_CONTENTMENT);
	AddEvent(NULL, ANIMAL_RESULT_INTEREST);

	if( m_nContentment == 100 && m_nInterest > 10 )
	{
		SetAnimal(ANIMAL_RESULT_COMPLETE);
		AddEvent(NULL, ANIMAL_RESULT_COMPLETE);
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------------
// GetItem Method																												������ ���
//
//CAnimal::ANIMAL_BREED_KIND	CAnimal::Reward()
//{
	// ���࿡�� ������ ���
	// ������� ����ɿ� ���� ������ Ȯ��
//	int nRandom = CSHMath::GetRandomNum(1, 100);
	//int nTbaItemIndex = ANIMAL_BREED_KIND_GREAT;
	//int nCheckNum = sizeof(ANIMAL_BREED_PROBA)/sizeof(ANIMAL_BREED_PROBA[0]);
	//for( int i = 0; i < nCheckNum; i++ )
	//{
	//	if( GetLife() >= ANIMAL_BREED_PROBA[i][0] && GetLife() <= ANIMAL_BREED_PROBA[i][1] )
	//	{
	//		nTbaItemIndex = i;
	//		break;
	//	}
	//}

	//// ����� ��޿� ���� ���� Ȯ��
	//int nProba = 0;
	//int nItemKind = ANIMAL_BREED_KIND_NORMAL;
	//for(int i=0; i < ANIMAL_BREED_KIND_MAX; i++)
	//{
	//	nProba += ANIMAL_BREED_PROBA[nTbaItemIndex][2+i]+ANIMAL_BREED_DELTA_PROBA[m_pcsParent->GetGrade()-1][i];
	//	if (nRandom < nProba)
	//	{
	//		nItemKind = i;
	//		break;
	//	}
	//}

	//// ���� ��Ḧ ����� ���� ������
	//if( GetFeedFreq(ANIMAL_FEED_KIND_GRADE_1) )
	//{
	//	if( nItemKind == ANIMAL_BREED_KIND_GREAT ||
	//		nItemKind == ANIMAL_BREED_KIND_GOOD )
	//	{
	//		nItemKind = ANIMAL_BREED_KIND_NORMAL;
	//	}
	//}
	//// �߱� ��Ḧ ����� ���� ������
	//else if( GetFeedFreq(ANIMAL_FEED_KIND_GRADE_2) )
	//{
	//	if( nItemKind == ANIMAL_BREED_KIND_GREAT )
	//	{
	//		nItemKind = ANIMAL_BREED_KIND_GOOD;
	//	}
	//}

	//return (ANIMAL_BREED_KIND)nItemKind;
	//return (ANIMAL_BREED_KIND)1;
//}

// -------------------------------------------------------------------------------------------------------------------------------------
// Plant Method																													���� ����
//
VOID CAnimal::SetAnimal(ANIMAL_RESULT eResult)
{
	switch(eResult)
	{
	case ANIMAL_RESULT_DIE:
		m_nOwnerID = 0;
		m_nKind = 0;
		m_nGrade = 0;
		m_eStep = ANIMAL_STEP_EMPTY;
		m_nLife = 0;
		m_nBreedTimeTick = NULL;
		m_nFeedTimeTick = NULL;
		m_nNextStepTime = 0;
		m_nBeforeLife = 0;
		m_nContentment = 0;
		m_nInterest = 0;
		break;
	case ANIMAL_RESULT_COMPLETE:
		m_eStep = ANIMAL_STEP_COMPLETE;
		m_nBreedTimeTick = NULL;
		break;
	}
}

// -------------------------------------------------------------------------------------------------------------------------------------
// GetMaxLifeFromTbl Method																				���� �ִ� ����� ��� (���̺���)
//
WORD CAnimal::GetMaxLife()
{
	return ANIMAL_STEP_MAX_LIFE;
}
// -------------------------------------------------------------------------------------------------------------------------------------
// GetMaxLifeFromTbl Method																			  ��� ��� ����� ��� (���̺���)
//
WORD CAnimal::GetFeedLifeFromTbl(int nGrade)
{
	int nReturn = 0; //ANIMAL_STEP_FEED_LIFE[nGrade];

	return nReturn;
}

WORD CAnimal::GetFeedContentmentFromTbl(int nGrade)
{
	int nReturn = ANIMAL_STEP_FEED_LIFE[nGrade][0];

	return nReturn;
}

WORD CAnimal::GetFeedInterestFromTbl(int nGrade)
{
	int nReturn = ANIMAL_STEP_FEED_LIFE[nGrade][1];

	return nReturn;
}

VOID CAnimal::Cleaning(int nGrade)
{
	//int nPlusContentment = GetFeedContentmentFromTbl(nGrade);
	//int nPlusInterest = GetFeedInterestFromTbl(nGrade);
	int nPlusContentment = 10;
	int nPlusInterest = 1;

	m_nBeforeContentment = m_nContentment;
	m_nBeforeInterest = m_nInterest;

	m_nContentment += nPlusContentment;
	m_nInterest += nPlusInterest;

	if( m_nContentment > 100 )
	{
		m_nContentment = 100;
	}

	if( m_nInterest > 100 )
	{
		m_nInterest = 100;
	}

	AddEvent(NULL, ANIMAL_RESULT_CONTENTMENT);
	AddEvent(NULL, ANIMAL_RESULT_INTEREST);

	if( m_nContentment == 100 && m_nInterest > 10 )
	{
		SetAnimal(ANIMAL_RESULT_COMPLETE);
		AddEvent(NULL, ANIMAL_RESULT_COMPLETE);
	}
}

WORD CAnimal::GetGrade()
{
	if( m_nInterest >= 10 && m_nInterest < 20 )
		return 0;
	else if( m_nInterest < 29 )
		return 1;
	else if( m_nInterest >= 29 )
		return 2;

	return 0;
}
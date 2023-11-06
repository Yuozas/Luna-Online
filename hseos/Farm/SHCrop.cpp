/*********************************************************************

	 ����		: SHCrop.cpp
	 �ۼ���		: hseos
	 �ۼ���		: 2007/04/10

	 ���ϼ���	: ���۹� Ŭ������ �ҽ�

 *********************************************************************/

#include "stdafx.h"
#include "../Common/SHMath.h"

#include "SHCrop.h"

#if defined(_AGENTSERVER)
#elif defined(_MAPSERVER_)
	#include "../Farm/SHFarmManager.h"

	// 071220 LUJ
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

DWORD	CSHCrop::CROP_LIFE_DECREASE_TIME;														// ���۹� ����� -1 �ð�
DWORD	CSHCrop::CROP_PLANT_SEEDING_TIME;														// ���۹��� �ɰ� ���� �����µ� �ɸ��� �ð�
DWORD	CSHCrop::CROP_PLANT_RETRY_TIME;															// ���۹��� �ٽ� �ɴ� �� �ʿ��� �ð� (��)
DWORD	CSHCrop::CROP_MANURE_TIME;																// ���۹��� ��Ḧ �ִ� �ð�
DWORD	CSHCrop::CROP_MANURE_RETRY_TIME;														// ���۹��� �ٽ� ��Ḧ �ִ� �� �ʿ��� �ð� (��)
DWORD	CSHCrop::CROP_HARVEST_TIME;																// ���۹��� ��Ȯ ��û �� ��Ȯ�ϴµ� �ɸ��� �ð�
DWORD	CSHCrop::CROP_DIE_DELAY_TIME = 24*60;													// ������� 1�� �� �� �ױ������ ������ �ð� (��)

UINT	CSHCrop::CROP_NEXT_STEP_TIME[CROP_SEED_GRADE_MAX][CROP_STEP_MAX-1] = {0,};				// ���� �ܰ谡 �Ǳ� ���� �ʿ��� �ð� (��)
UINT	CSHCrop::CROP_STEP_MIN_LIFE[CROP_SEED_GRADE_MAX][CROP_STEP_MAX-1] = {0,};				// �ܰ迡 ���� �ּ�/�ִ� �����
UINT	CSHCrop::CROP_STEP_MAX_LIFE[CROP_SEED_GRADE_MAX][CROP_STEP_MAX-1] = {0,};				// �ܰ迡 ���� �ּ�/�ִ� �����
UINT	CSHCrop::CROP_STEP_MANURE_MIN_LIFE[CROP_SEED_GRADE_MAX][CROP_MANURE_KIND_MAX] = {0,};	// ��ῡ ���� ���� �����
UINT	CSHCrop::CROP_STEP_MANURE_MAX_LIFE[CROP_SEED_GRADE_MAX][CROP_MANURE_KIND_MAX] = {0,};	// ��ῡ ���� ���� �����

UINT	CSHCrop::CROP_HARVEST_PROBA[5][CROP_HARVEST_KIND_MAX+2] = {0,};							// ���۹� ����¿� ���� ��Ȯ Ȯ��
UINT	CSHCrop::CROP_HARVEST_DELTA_PROBA[CROP_SEED_GRADE_MAX][CROP_HARVEST_KIND_MAX] = {0,};	// �Թ� ��޿� ���� ��Ȯ ���� Ȯ��

// -------------------------------------------------------------------------------------------------------------------------------------
// CSHCrop Method																												  ������
//
CSHCrop::CSHCrop()
{
	SetCrop(CROP_RESULT_DIE);
}

// -------------------------------------------------------------------------------------------------------------------------------------
// ~CSHCrop Method																												 �ı���
//
CSHCrop::~CSHCrop()
{
}

// -------------------------------------------------------------------------------------------------------------------------------------
// MainLoop Method																									 �Թ� ���� ��ƾ ó��
//
VOID CSHCrop::MainLoop()
{
  	CSHFarmObj::MainLoop();
#if defined(_AGENTSERVER)
#elif defined(_MAPSERVER_)
//	if (g_csFarmManager.GetFarmZone(0) == NULL) return;

 	if (m_nLife > 0)
	{
		if (m_eStep != CROP_STEP_COMPLETE)
		{
			// ������� 1 �̸� �ױ������ ������ �ð��� üũ�ؼ� ���� ó���� �Ѵ�.
			if (m_nLife == 1)
			{
				if (m_nDieDelayTimeTick)
				{
					if (gCurTime - m_nDieDelayTimeTick > SHMath_MINUTE(1))
					{
						m_nDieDelayTimeTick = gCurTime;
						m_nDieDelayTime++;
						if (m_nDieDelayTime >= CROP_DIE_DELAY_TIME)
						{
							// 071218 LUJ,	�α�. ���۹��� ����(SHGarden)�� �����ְ�, ������ ����(CSHFarm)�� �����ִ�
							{
								CSHGarden*	garden	= ( CSHGarden* )GetParent();
								CSHFarm*	farm	= ( CSHFarm* )( garden ? garden->GetParent() : 0 );

								if( farm )
								{
									InsertLogFarmCrop( farm, this, eLog_FamilyFarmDieCrop );
								}
							}
							
							m_nLife = 0;
							// Ŭ���̾�Ʈ�� �˸��� ���� ����
 							AddEvent(NULL, CROP_RESULT_DIE);
							SetCrop(CROP_RESULT_DIE);
						}
					}
					return;
				}
			}

			// ����� ���� �ð� üũ
			if (gCurTime - m_nPlantTimeTick > CROP_LIFE_DECREASE_TIME)
			{
				m_nPlantTimeTick = gCurTime;
				m_nLife--;
				// ������� ����
				if (m_nLife == 0)
				{
					m_nLife = 1;
					// �ױ� ������ �����̽ð� ����
					m_nDieDelayTimeTick = gCurTime;
					m_nDieDelayTime = 0;
					// Ŭ���̾�Ʈ�� �˸��� ���� ����
 					AddEvent(NULL, CROP_RESULT_LIFE);
					return;
				}
				else
				{
					// Ŭ���̾�Ʈ�� �˸��� ���� ����
 					AddEvent(NULL, CROP_RESULT_LIFE);
				}
			}

			//  �ܰ� ��� ó��
			if (gCurTime - m_nNextStepTimeTick > SHMath_MINUTE(1))
			{
				m_nNextStepTimeTick = gCurTime;
				m_nNextStepTime++;
				if (m_nNextStepTime >= GetNextStepTimeTickFromTbl(m_nSeedGrade, m_eStep))
				{
					int nTmpStep = m_eStep + 1;

					// ���� �Ϸ�
					if (nTmpStep >= CROP_STEP_COMPLETE)
					{
						// Ŭ���̾�Ʈ�� �˸��� ���� ����
						AddEvent(NULL, CROP_RESULT_STEP_UP_COMPLETE);
						SetCrop(CROP_RESULT_STEP_UP_COMPLETE);
					}
					else
					{
						// Ŭ���̾�Ʈ�� �˸��� ���� ����
						AddEvent(NULL, CROP_RESULT_STEP_UP);
						SetCrop(CROP_RESULT_STEP_UP);
					}
				}
				else
				{
					AddEvent(NULL, CROP_RESULT_NEXT_STEP_TIME);
				}
			}
		}
	}
#else
	// Ŭ���̾�Ʈ������ �ɴ� ������ ���� ó�� Step�� CROP_STEP_1�� �ƴϱ� ������ ������ Ÿ�� �Ŀ� ����
   	if (m_nLife > 0)
	{
 		if (m_eStep == CROP_STEP_SEEDING)
		{
			// ���۹��� �ɴ� ���� �� ������ ǥ�ø� ����..
			CMonsterGuageDlg* pGuageDlg = GAMEIN->GetMonsterGuageDlg() ;
			if (pGuageDlg)
			{
				if ((CObject*)m_pcsParent->GetRenderObjEx(GetID()+15) == OBJECTMGR->GetSelectedObject())
				{
					pGuageDlg->SetMonsterLife(gCurTime-GetPlantTimeTick(), CSHCrop::CROP_PLANT_SEEDING_TIME);
				}
			}
			// �ɴ� ��� ��
			if (gCurTime - m_nPlantTimeTick > CROP_PLANT_SEEDING_TIME)
			{
				m_eStep = CROP_STEP_1;
				AddEvent(NULL, CROP_RESULT_PLANT);
			}
		}
		// ��Ȯ ��� �ð�
		else if (m_eStep == CROP_STEP_COMPLETE)
		{
			if (m_nPlantTimeTick)
			{
				if (gCurTime - m_nPlantTimeTick > CROP_HARVEST_TIME)
				{
					m_nPlantTimeTick = NULL;
					// ��Ȯ ��� ��
					AddEvent(NULL, CROP_RESULT_HARVEST);
				}
			}
		}
		else
		{
 			// ���۹��� ��Ḧ �ְ� �ִ� �ð�
   			if (GetManureTimeTick())
			{
				CMonsterGuageDlg* pGuageDlg = GAMEIN->GetMonsterGuageDlg() ;

				if (gCurTime - GetManureTimeTick() > CSHCrop::CROP_MANURE_TIME)
				{
					if (pGuageDlg)
					{
						if ((CObject*)GetRenderObj() == OBJECTMGR->GetSelectedObject())
						{
							pGuageDlg->SetMonsterLife(GetLife(), GetMaxLife());
						}
					}
					SetManureTimeTick(NULL);
					AddEvent(NULL, CROP_RESULT_MANURE);
				}
				else
				{
					if (pGuageDlg)
					{
						if ((CObject*)GetRenderObj() == OBJECTMGR->GetSelectedObject())
						{
							int nDelta = ((GetLife()-GetBeforeLife())*(gCurTime - GetManureTimeTick())<<8)/CSHCrop::CROP_MANURE_TIME;
							pGuageDlg->SetMonsterLife((GetBeforeLife()<<8)+nDelta, GetMaxLife()<<8);
						}
					}
				}
			}
		}
	}
#endif
}

// -------------------------------------------------------------------------------------------------------------------------------------
// Plant Method																												 ���۹� �ɱ�
//
VOID CSHCrop::Plant(int nPlayerID, int nSeedGrade, int nKind)
{
	m_nOwnerID = nPlayerID;
	// Ŭ���̾�Ʈ������ �ɴ� ������ ���� Step �� �ٸ��� �Ѵ�. ���� Ŭ���̾�Ʈ���� ������ ������ ��
#if defined(_MAPSERVER_)
	m_eStep = CROP_STEP_1;
#else
	m_eStep = CROP_STEP_SEEDING;
#endif
	m_nKind = nKind;
	m_nPlantTimeTick = gCurTime;
	m_nSeedGrade = nSeedGrade;
	m_nNextStepTimeTick = gCurTime;
	m_nLife = GetLifeFromTbl(m_nSeedGrade, m_eStep);
}

// -------------------------------------------------------------------------------------------------------------------------------------
// Manure Method																											   ��� �ֱ�
//
VOID CSHCrop::Manure(int nManureGrade, int nManureKind)
{
	int nPlusLife = GetManureLifeFromTbl(nManureGrade, nManureKind);

	m_nBeforeLife = m_nLife;
	m_nLife += nPlusLife;

	int nMaxLife = GetMaxLifeFromTbl(m_nSeedGrade, m_eStep);
	if (m_nLife > nMaxLife)
	{
		m_nLife = nMaxLife;
	}

	m_nManureFreq[nManureKind-1]++;

	m_nDieDelayTimeTick = 0;
	m_nDieDelayTime = 0;
}

// -------------------------------------------------------------------------------------------------------------------------------------
// Harvest Method																												��Ȯ�ϱ�
//
CSHCrop::CROP_HARVEST_KIND CSHCrop::Harvest()
{
	// ���۹� ��Ȯ�ϱ�
	// ..���۹��� ����¿� ���� ��Ȯ Ȯ��
	int nRandom = CSHMath::GetRandomNum(1, 100);
	int nTbaHarvestIndex = CROP_HARVEST_KIND_GREAT;
	int nCheckNum = sizeof(CROP_HARVEST_PROBA)/sizeof(CROP_HARVEST_PROBA[0]);
	for(int i=0; i<nCheckNum; i++)
	{
		if ( GetLife() >= CROP_HARVEST_PROBA[i][0] && GetLife() <= CROP_HARVEST_PROBA[i][1])
		{
			nTbaHarvestIndex = i;
			break;
		}
	}

	// ..�Թ��� ��޿� ���� ���� Ȯ��
	int nProba = 0;
	int nHarvestKind = CROP_HARVEST_KIND_NORMAL;
	for(int i=0; i<CROP_HARVEST_KIND_MAX; i++)
	{
		nProba += CROP_HARVEST_PROBA[nTbaHarvestIndex][2+i]+CROP_HARVEST_DELTA_PROBA[m_pcsParent->GetGrade()-1][i];
		if (nRandom < nProba)
		{
			nHarvestKind = i;
			break;
		}
	}

	// ���� ��Ḧ ����� ���� ������ ��ǳ,ǳ�� -> ����
	if (GetManureFreq(CROP_MANURE_KIND_GRADE1))
	{
		if (nHarvestKind == CROP_HARVEST_KIND_GREAT || 
			nHarvestKind == CROP_HARVEST_KIND_GOOD)
		{
			nHarvestKind = CROP_HARVEST_KIND_NORMAL;
		}
	}
	// �߱� ��Ḧ ����� ���� ������ ��ǳ -> ǳ��
	else if (GetManureFreq(CROP_MANURE_KIND_GRADE2))
	{
		if (nHarvestKind == CROP_HARVEST_KIND_GREAT)
		{
			nHarvestKind = CROP_HARVEST_KIND_GOOD;
		}
	}

	return (CROP_HARVEST_KIND)nHarvestKind;
}

// -------------------------------------------------------------------------------------------------------------------------------------
// Plant Method																												 ���۹� ����
//
VOID CSHCrop::SetCrop(CROP_RESULT eResult)
{
	switch(eResult)
	{
	case CROP_RESULT_DIE:
		m_nOwnerID = 0;
		m_nKind = 0;			
		m_nSeedGrade = 0;			
		m_eStep = CROP_STEP_EMPTY;				
		m_nLife = 0;				
		m_nPlantTimeTick = NULL;		
		m_nManureTimeTick = NULL;
		m_nNextStepTimeTick = gCurTime;	
		m_nNextStepTime = 0;
		m_nBeforeLife = 0;
		m_nDieDelayTimeTick = 0;
		m_nDieDelayTime = 0;
		ZeroMemory(m_nManureFreq, sizeof(m_nManureFreq));
		break;
	case CROP_RESULT_STEP_UP:
		m_nPlantTimeTick = NULL;		
		m_nManureTimeTick = NULL;
		m_nNextStepTimeTick = gCurTime;	
		m_nNextStepTime = 0;
		m_nDieDelayTimeTick = 0;
		m_nDieDelayTime = 0;
		m_eStep = (CROP_STEP)((int)m_eStep + 1);
		m_nLife = GetLifeFromTbl(m_nSeedGrade, m_eStep);
		break;
	case CROP_RESULT_STEP_UP_COMPLETE:
		m_nPlantTimeTick = NULL;		
		m_nManureTimeTick = NULL;
		m_nDieDelayTimeTick = 0;
		m_nDieDelayTime = 0;
		m_nNextStepTimeTick = gCurTime;	
		m_eStep = (CROP_STEP)((int)m_eStep + 1);
		break;
	}
}

// -------------------------------------------------------------------------------------------------------------------------------------
// GetNextStepTimeTick Method																���۹� ���� �ܰ� �ʿ� �ð� ��� (���̺���)
//
WORD CSHCrop::GetNextStepTimeTickFromTbl(int nSeedGrade, CROP_STEP eCurStep)
{
	if (eCurStep < CROP_STEP_1) eCurStep = CROP_STEP_1;
	else if (eCurStep == CROP_STEP_COMPLETE) eCurStep = (CROP_STEP)(CROP_STEP_COMPLETE-1);

	return CROP_NEXT_STEP_TIME[nSeedGrade-1][eCurStep-CROP_STEP_1];
}

// -------------------------------------------------------------------------------------------------------------------------------------
// GetLifeFromTbl Method																				 ���۹� ����� ��� (���̺���)
//
WORD CSHCrop::GetLifeFromTbl(int nSeedGrade, CROP_STEP eCurStep)
{
	if (eCurStep < CROP_STEP_1) eCurStep = CROP_STEP_1;
	else if (eCurStep == CROP_STEP_COMPLETE) eCurStep = (CROP_STEP)(CROP_STEP_COMPLETE-1);

	return CROP_STEP_MIN_LIFE[nSeedGrade-1][eCurStep-CROP_STEP_1];
}

// -------------------------------------------------------------------------------------------------------------------------------------
// GetMaxLifeFromTbl Method																			���۹� �ִ� ����� ��� (���̺���)
//
WORD CSHCrop::GetMaxLifeFromTbl(int nSeedGrade, CROP_STEP eCurStep)
{
	if (eCurStep < CROP_STEP_1) eCurStep = CROP_STEP_1;
	else if (eCurStep == CROP_STEP_COMPLETE) eCurStep = (CROP_STEP)(CROP_STEP_COMPLETE-1);

	return CROP_STEP_MAX_LIFE[nSeedGrade-1][eCurStep-CROP_STEP_1];
}

// -------------------------------------------------------------------------------------------------------------------------------------
// GetMaxLifeFromTbl Method																			  ��� ��� ����� ��� (���̺���)
//
WORD CSHCrop::GetManureLifeFromTbl(int nGrade, int nKind)
{
	int nReturn = CSHMath::GetRandomNum(CROP_STEP_MANURE_MIN_LIFE[nGrade-1][nKind-1], CROP_STEP_MANURE_MAX_LIFE[nGrade-1][nKind-1]);

	return nReturn;
}

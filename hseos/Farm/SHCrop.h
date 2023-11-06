/*********************************************************************

	 ����		: SHCrop.h
	 �ۼ���		: hseos
	 �ۼ���		: 2007/04/10

	 ���ϼ���	: ���۹� Ŭ������ ���

 *********************************************************************/

#pragma once

#include "SHFarmObj.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
class CSHCrop : public CSHFarmObj
{
public:
	//----------------------------------------------------------------------------------------------------------------
	enum CROP_RESULT															// ���� �Թ� ���� ���
	{
		CROP_RESULT_LIFE,														// ..����� ��ȭ
		CROP_RESULT_DIE,														// ..����
		CROP_RESULT_STEP_UP,													// ..�ܰ� ���
		CROP_RESULT_STEP_UP_COMPLETE,											// ..�ܰ� ��� �Ϸ�
		CROP_RESULT_NEXT_STEP_TIME,												// ..���� �ܰ� �ð�
		CROP_RESULT_PLANT,														// ..�ɱ�
		CROP_RESULT_MANURE,														// ..����ֱ�
		CROP_RESULT_HARVEST,													// ..��Ȯ
		CROP_RESULT_MAX,														// ..��
	} ;

	//----------------------------------------------------------------------------------------------------------------
	enum CROP_STEP																// ���۹� �ܰ� ����											
	{
		CROP_STEP_EMPTY = 0,													// ..����
		CROP_STEP_SEEDING,														// ..�ɴ� ��
		CROP_STEP_1,															// ..1�ܰ�
		CROP_STEP_2,															// ..2�ܰ�
		CROP_STEP_3,															// ..3�ܰ�
		CROP_STEP_COMPLETE,														// ..�Ϸ�
		CROP_STEP_MAX = CROP_STEP_COMPLETE-CROP_STEP_SEEDING,
	} ;

	//----------------------------------------------------------------------------------------------------------------
	enum CROP_MANURE_KIND														// ��� ���� ����											
	{
		CROP_MANURE_KIND_GRADE1 = 1,											// ..����
		CROP_MANURE_KIND_GRADE2,												// ..�߱�
		CROP_MANURE_KIND_GRADE3,												// ..���
		CROP_MANURE_KIND_MAX	= CROP_MANURE_KIND_GRADE3,
	};

	//----------------------------------------------------------------------------------------------------------------
	enum CROP_SEED_GRADE														// ���� ��� ����
	{
		CROP_SEED_GRADE_1 = 1,
		CROP_SEED_GRADE_2,
		CROP_SEED_GRADE_3,
		CROP_SEED_GRADE_4,
		CROP_SEED_GRADE_5,
		CROP_SEED_GRADE_6,
		CROP_SEED_GRADE_7,
		CROP_SEED_GRADE_8,
		CROP_SEED_GRADE_MAX = CROP_SEED_GRADE_8,
	} ;

	//----------------------------------------------------------------------------------------------------------------
	enum CROP_HARVEST_KIND														// ���۹� ��Ȯ ����
	{								
		CROP_HARVEST_KIND_GREAT = 0,											// ..��ǳ
		CROP_HARVEST_KIND_GOOD,													// ..ǳ��
		CROP_HARVEST_KIND_NORMAL,												// ..����
		CROP_HARVEST_KIND_BAD,													// ..����
		CROP_HARVEST_KIND_WORST,												// ..����
		CROP_HARVEST_KIND_MAX,
	};

	//----------------------------------------------------------------------------------------------------------------
	static DWORD	CROP_LIFE_DECREASE_TIME; 												// ���۹� ����� -1 �ð�
	static DWORD	CROP_PLANT_SEEDING_TIME;												// ���۹��� �ɰ� ���� �����µ� �ɸ��� �ð�
	static DWORD	CROP_PLANT_RETRY_TIME;													// ���۹��� �ٽ� �ɴ� �� �ʿ��� �ð� (��)
	static DWORD	CROP_MANURE_TIME;														// ���۹��� ��Ḧ �ִ� �ð�
	static DWORD	CROP_MANURE_RETRY_TIME;													// ���۹��� �ٽ� ��Ḧ �ִ� �� �ʿ��� �ð� (��)
	static DWORD	CROP_HARVEST_TIME;														// ���۹��� ��Ȯ ��û �� ��Ȯ�ϴµ� �ɸ��� �ð�
	static DWORD	CROP_DIE_DELAY_TIME;													// ������� 0�� �� �� �ױ������ ������ �ð� (��)

	static UINT		CROP_NEXT_STEP_TIME[CROP_SEED_GRADE_MAX][CROP_STEP_MAX-1];				// ���� �ܰ谡 �Ǳ� ���� �ʿ��� �ð� (��)
	static UINT		CROP_STEP_MIN_LIFE[CROP_SEED_GRADE_MAX][CROP_STEP_MAX-1];				// �ܰ迡 ���� �ּ�/�ִ� �����
	static UINT		CROP_STEP_MAX_LIFE[CROP_SEED_GRADE_MAX][CROP_STEP_MAX-1];				// �ܰ迡 ���� �ּ�/�ִ� �����
	static UINT		CROP_STEP_MANURE_MIN_LIFE[CROP_SEED_GRADE_MAX][CROP_MANURE_KIND_MAX];	// ��ῡ ���� ���� �����
	static UINT		CROP_STEP_MANURE_MAX_LIFE[CROP_SEED_GRADE_MAX][CROP_MANURE_KIND_MAX];	// ��ῡ ���� ���� �����

	static UINT		CROP_HARVEST_PROBA[5][CROP_HARVEST_KIND_MAX+2];							// ���۹� ����¿� ���� ��Ȯ Ȯ��
	static UINT		CROP_HARVEST_DELTA_PROBA[CROP_SEED_GRADE_MAX][CROP_HARVEST_KIND_MAX];	// �Թ� ��޿� ���� ��Ȯ ���� Ȯ��

	//----------------------------------------------------------------------------------------------------------------
	struct PACKET_FARM_GARDEN_CROP : public MSGBASE								// �Թ� ��Ŷ ����
	{
		int					nFarmZone;											// ..���� ����
		int					nFarmID;											// ..���� �ε���
		int					nGardenID;											// ..�Թ� �ε���
		int					nCropID;											// ..���۹� �ε���
		int					nResult;											// ..��� ����(�������� ó�� �� ����� �����ؼ� Ŭ���̾�Ʈ�� ����)
	};

	//----------------------------------------------------------------------------------------------------------------
	struct stResult																// Ŭ���̾�Ʈ�� �˷��� �� ���۹� ��� ����
	{
		CROP_RESULT eResult[256];												// ..��� ����. �ִ� 256��
		int nResultNum;															// ..��� ����						
	};

private:
	//----------------------------------------------------------------------------------------------------------------
	int						m_nKind;											// ..����(ex: ȭ����, ������..)
	int						m_nSeedGrade;										// ..���� ���
	CROP_STEP				m_eStep;											// ..�ܰ�
	DWORD					m_nPlantTimeTick;									// ..���� �ð� ƽ
	DWORD					m_nManureTimeTick;									// ..��� �� �ð� ƽ
	WORD					m_nNextStepTime;									// ..�ɰ� �帥 �ð� (��)
	DWORD					m_nNextStepTimeTick;								// ..���� �ܰ� �ʿ� �ð� ƽ
	WORD					m_nBeforeLife;										// ..� ������ ���� ������� ��ȭ�Ǳ� ���� �����
	DWORD					m_nDieDelayTimeTick;								// ..������� 1 �� �Ǿ��� �� �ױ������ ������ �ð� ƽ
	DWORD					m_nDieDelayTime;									// ..������� 1 �� �Ǿ��� �� �ױ������ ������ �ð� (��)

	int						m_nManureFreq[CROP_MANURE_KIND_MAX];				// ..��Ḧ ����� Ƚ��

public:
	//----------------------------------------------------------------------------------------------------------------
	CSHCrop();
	~CSHCrop();

	//----------------------------------------------------------------------------------------------------------------
	//							���۹� ���� ����
	VOID						SetKind(int nKind)			{ m_nKind = nKind; }
	//							���۹� ���� ���
	int							GetKind()					{ return m_nKind; }

	//----------------------------------------------------------------------------------------------------------------
	//							���۹� �ܰ� ����
	VOID						SetStep(CROP_STEP eStep)	{ m_eStep = eStep; }
	//							���۹� �ܰ� ���
	CROP_STEP					GetStep()					{ return m_eStep; }

	//----------------------------------------------------------------------------------------------------------------
	//							���۹� ���� �ð� ����
	VOID						SetPlantTimeTick(DWORD nTick) { m_nPlantTimeTick = nTick; }
	//							���۹� ���� �ð� ���
	DWORD						GetPlantTimeTick()			{ return m_nPlantTimeTick; }

	//----------------------------------------------------------------------------------------------------------------
	//							��� �� �ð� ����
	VOID						SetManureTimeTick(DWORD nTick) { m_nManureTimeTick = nTick; }
	//							��� �� �ð� ���
	DWORD						GetManureTimeTick()			{ return m_nManureTimeTick; }

	//----------------------------------------------------------------------------------------------------------------
	//							���۹� ����
	VOID						SetCrop(CROP_RESULT eResult); 

	//----------------------------------------------------------------------------------------------------------------
	//							���۹� ���� ��� ����
	VOID						SetSeedGrade(int nGrade)	{ m_nSeedGrade = nGrade; } 
	WORD						GetSeedGrade()				{ return m_nSeedGrade; } 
	int							GetManureFreq(CROP_MANURE_KIND eManureKind)		{ return m_nManureFreq[eManureKind-1]; }
	//----------------------------------------------------------------------------------------------------------------
	//							���۹� �ɱ�
	VOID						Plant(int nPlayerID, int nSeedGrade, int nKind);
	//							��� �ֱ�
	VOID						Manure(int nManureGrade, int nManureKind);
	//							��Ȯ�ϱ�
	CROP_HARVEST_KIND			Harvest();

	//----------------------------------------------------------------------------------------------------------------
	//							���۹� ���� �ܰ� �ʿ� �ð� ��� (���̺���)
	WORD						GetNextStepTimeTickFromTbl(int nSeedGrade, CROP_STEP eCurStep);
	//							���۹� �ɰ� �帥 �ð� ���
	WORD						GetNextStepTime()				{ return m_nNextStepTime; }
	//							���۹� �ɰ� �帥 �ð� ����
	VOID						SetNextStepTime(WORD nTime)		{ m_nNextStepTime = nTime; }

	//----------------------------------------------------------------------------------------------------------------
	//							���۹� ����� ��� (���̺���)
	WORD						GetLifeFromTbl(int nSeedGrade, CROP_STEP eCurStep);
	//							���۹� �ִ� ����� ��� (���̺���)
	WORD						GetMaxLifeFromTbl(int nSeedGrade, CROP_STEP eCurStep);
	//							���۹� �ִ� ����� ���
	WORD						GetMaxLife()				{ return GetMaxLifeFromTbl(m_nSeedGrade, m_eStep); }
	//							��� ��� ����� ��� (���̺���)
	WORD						GetManureLifeFromTbl(int nGrade, int nKind);
	
	//----------------------------------------------------------------------------------------------------------------
	//							��ȭ �� ����� ����
	WORD						SetBeforeLife() { m_nBeforeLife = m_nLife; }
	//							��ȭ �� ����� ���
	WORD						GetBeforeLife()	{ return m_nBeforeLife; }

	//----------------------------------------------------------------------------------------------------------------
	//							�ױ� �� ������Ÿ�� ����
	VOID						SetDieDelayTimeTick(DWORD nTick) { m_nDieDelayTimeTick = nTick; }

	//----------------------------------------------------------------------------------------------------------------
	//							���� ��ƾ ó��
	VOID						MainLoop();																
};

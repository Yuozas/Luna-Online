/*********************************************************************

	 ����		: Animal.h
	 �ۼ���		: Shinobi
	 �ۼ���		: 2008/03/08

	 ���ϼ���	: ���� Ŭ������ ���

 *********************************************************************/

#pragma once

#include "SHFarmObj.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//

class CAnimal : public CSHFarmObj
{
public:
	//-------------------------------------------------------------------------------------------------------------------------------------------------
	enum ANIMAL_RESULT																				// ���� ���� ���
	{
		ANIMAL_RESULT_LIFE,																			// ����� ��ȭ
		ANIMAL_RESULT_DIE,																			// ����
		ANIMAL_RESULT_IN_STALL,																		// ��翡 ���� �ֱ�
		ANIMAL_RESULT_FEEDING,																		// �����ֱ�
		ANIMAL_RESULT_CLEANNING,																	// �ı�
		ANIMAL_RESULT_REWARD,																		// ������ �������� ��´�
		ANIMAL_RESULT_CONTENTMENT,																	// ������ ��ȭ
		ANIMAL_RESULT_INTEREST,																		// ���ɵ� ��ȭ
		ANIMAL_RESULT_COMPLETE,
		ANIMAL_RESULT_MAX,																			// �Ϸ�
	};

	//-------------------------------------------------------------------------------------------------------------------------------------------------
	enum ANIMAL_STEP																				// ������ �ܰ�
	{
		ANIMAL_STEP_EMPTY = 0,																		// ����.
		ANIMAL_STEP_IN_STALL,																		// ��翡 �ֱ�
		ANIMAL_STEP_1,																				// 1�ܰ�
		ANIMAL_STEP_2,																				// 2�ܰ�
		ANIMAL_STEP_3,																				// 3�ܰ�
		ANIMAL_STEP_COMPLETE,																		// �Ϸ�
		ANIMAL_STEP_MAX = ANIMAL_STEP_COMPLETE - ANIMAL_STEP_IN_STALL,
	};

	//-------------------------------------------------------------------------------------------------------------------------------------------------
	enum ANIMAL_FEED_KIND																			// ��� ���
	{
		ANIMAL_FEED_KIND_GRADE_1 = 0,																// �Ϲ� ���
		ANIMAL_FEED_KIND_GRADE_2,																	// ��� ���
		ANIMAL_FEED_KIND_MAX
	};

	enum ANIMAL_CLEANER_KIND
	{
		ANIMAL_CLEANER_KIND_GRADE_1 = 0,
		ANIMAL_CLEANER_KIND_GRADE_2,
		ANIMAL_CLEANER_KIND_MAX
	};

	//-------------------------------------------------------------------------------------------------------------------------------------------------
	enum ANIMAL_KIND
	{
		ANIMAL_KIND_MELEE,
		ANIMAL_KIND_MAGIC,
		ANIMAL_KIND_BUFF,
		ANIMAL_KIND_MAX,
	};


	enum ANIMAL_BREED_KIND																			// ��� ��Ȯ
	{
		ANIMAL_BREED_KIND_BAD,																		// ������
		ANIMAL_BREED_KIND_NORMAL,																	// �߰�
		ANIMAL_BREED_KIND_GOOD,																		// ����
		ANIMAL_BREED_KIND_WORST,																	// �ſ� ������
		ANIMAL_BREED_KIND_MAX,
	};	

	static DWORD ANIMAL_IN_STALL_TIME;																// ������ ��翡 �ִ� �ð� (��)
	static DWORD ANIMAL_LIFE_DECREASE_TIME;															// ������ ����� -1 �ð�
	static DWORD ANIMAL_RETRY_TIME;																	// ������ ��翡 �ٽ� ��ȯ�ϴ� �� �ʿ��� �ð� (��)
	static DWORD ANIMAL_FEED_TIME;																	// ���࿡�� ���̸� �ִ� �ð�
	static DWORD ANIMAL_CLEANING_TIME;																	// ������ �ı�´� �ҿ�Ǵ� �ð�
	static DWORD ANIMAL_CLEANING_RETRY_TIME;															// ������ �ٽ� �ı�´� �ɸ��� �ð� (��)
	static DWORD ANIMAL_DIE_DELAY_TIME;																// ������ ������� 0�� �� �� �ױ������ ������ �ð� (��)
	static DWORD ANIMAL_STALL_REWARD_TIME;														// ���࿡�� �������� ��´� �ɸ��� �ð� (��)
	static DWORD ANIMAL_FEED_DELAY_TIME;															// ���࿡�� ���̸� �ٽ� �ִµ� �ɸ��� �ð�

	static UINT		ANIMAL_STEP_MIN_LIFE[4][2];														// �ܰ迡 ���� �ּ�/�ִ� �����
	static UINT		ANIMAL_STEP_MAX_LIFE;															// ������ �ִ� HP��
	static UINT		ANIMAL_STEP_FEED_LIFE[ANIMAL_FEED_KIND_MAX][2];									// ��ῡ ���� ���� �����

	static UINT		ANIMAL_STEP_MAX_CONTENTMENT;
	static UINT		ANIMAL_STEP_MAX_INTEREST;

	static DWORD ANIMAL_CONTENTMENT_DECREASE_TIME;													// ������ �������� -1 �Ǵ� �ð�
	static DWORD ANIMAL_INTEREST_DECREASE_TIME;														// ������ ���ɵ��� -1 �Ǵ� �ð�

	//static UINT		ANIMAL_BREED_PROBA[5][ANIMAL_BREED_KIND_MAX+2];									// ���� ����¿� ���� ��Ȯ Ȯ��

	static UINT		ANIMAL_REWARD_LIST[ANIMAL_KIND_MAX];											// ���� ��Ȯ�� ��Ե� ������
	static char		ANIMAL_NAME_TBL[ANIMAL_KIND_MAX][256];
private:
	//----------------------------------------------------------------------------------------------------------------
	int						m_nKind;																// ..����
	ANIMAL_STEP				m_eStep;																// ..�ܰ�
	DWORD					m_nBreedTimeTick;														// ..������ �ð� ƽ
	DWORD					m_nFeedTimeTick;														// ..��� �� �ð� ƽ
	DWORD					m_nCleaningTimeTick;														// ..û�� �ð� ƽ
	WORD					m_nNextStepTime;														// ..������ �帥 �ð� (��)
	WORD					m_nBeforeLife;															// ..� ������ ���� ������� ��ȭ�Ǳ� ���� �����
	WORD					m_nBeforeContentment;
	WORD					m_nBeforeInterest;

	WORD					m_nContentment;															// ..����
	WORD					m_nInterest;															// ..����
	DWORD					m_nContentmentTimeTick;
	DWORD					m_nInterestTimeTick;

	//int						m_nFeedFreq[ANIMAL_FEED_KIND_MAX];										// ..��Ḧ ����� Ƚ��

public:
	//----------------------------------------------------------------------------------------------------------------
	CAnimal();
	~CAnimal();

	VOID						SetContentment(WORD contentment)	{	m_nContentment = contentment;	}
	WORD						GetContentment()					{	return m_nContentment;	}

	VOID						SetInterest(WORD interest)			{	m_nInterest = interest;	}
	WORD						GetInterest()						{	return m_nInterest;	}

	//----------------------------------------------------------------------------------------------------------------
	//							���� ���� ����
	VOID						SetKind(int nKind)			{ m_nKind = nKind; }
	//							���� ���� ���
	int							GetKind()					{ return m_nKind; }

	//----------------------------------------------------------------------------------------------------------------
	//							���� �ܰ� ����
	VOID						SetStep(ANIMAL_STEP eStep)	{ m_eStep = eStep; }
	//							���� �ܰ� ���
	ANIMAL_STEP					GetStep()					{ return m_eStep; }

	//----------------------------------------------------------------------------------------------------------------
	//							���� ��Ų �ð� ����
	VOID						SetBreedTimeTick(DWORD nTick) { m_nBreedTimeTick = nTick; }
	//							������ ������ �ð� ���
	DWORD						GetBreedTimeTick()			{ return m_nBreedTimeTick; }

	//----------------------------------------------------------------------------------------------------------------
	//							��� �� �ð� ����
	VOID						SetFeedTimeTick(DWORD nTick) { m_nFeedTimeTick = nTick; }
	//							��� �� �ð� ���
	DWORD						GetFeedTimeTick()			{ return m_nFeedTimeTick; }

	//							û�� �� �ð� ����
	VOID						SetCleaningTimeTick(DWORD nTick)	{	m_nCleaningTimeTick = nTick;	}
	//							û�� �� �ð� ���
	DWORD						GetCleaningTimeTick()		{	return m_nCleaningTimeTick;	}

	//----------------------------------------------------------------------------------------------------------------
	//							���۹� ����
	VOID						SetAnimal(ANIMAL_RESULT eResult); 

	//----------------------------------------------------------------------------------------------------------------
	//int							GetFeedFreq(ANIMAL_FEED_KIND eManureKind)		{ return m_nFeedFreq[eManureKind-1]; }
	//----------------------------------------------------------------------------------------------------------------
	//							���� �����ϱ�
	VOID						Breed(int nPlayerID, int nAnimalCageGrade, int nKind);
	//							��� �ֱ�
	VOID						Feed(int nGrade);
	//							���� û��
	VOID						Cleaning(int nGrade);
	//							������ ���
	//DWORD						Reward();

	//----------------------------------------------------------------------------------------------------------------
	//							���� ���� �ܰ� �ʿ� �ð� ��� (���̺���)
	WORD						GetNextStepTimeTickFromTbl(int nGrade, ANIMAL_STEP eCurStep);
	//							������ �����ϰ� �帥 �ð� ���
	WORD						GetNextStepTime()				{ return m_nNextStepTime; }
	//							������ �����ϰ� �帥 �ð� ����
	VOID						SetNextStepTime(WORD nTime)		{ m_nNextStepTime = nTime; }

	//----------------------------------------------------------------------------------------------------------------
	//							���� �ִ� ����� ���
	WORD						GetMaxLife();
	//							��� ��� ����� ��� (���̺���)
	WORD						GetFeedLifeFromTbl(int nGrade);

	WORD						GetFeedContentmentFromTbl(int nGrade);
	WORD						GetFeedInterestFromTbl(int nGrade);
	
	//----------------------------------------------------------------------------------------------------------------
	//							��ȭ �� ����� ����
	WORD						SetBeforeLife() { m_nBeforeLife = m_nLife; }
	//							��ȭ �� ����� ���
	WORD						GetBeforeLife()	{ return m_nBeforeLife; }

	//----------------------------------------------------------------------------------------------------------------
	//							�ױ� �� ������Ÿ�� ����
//	VOID						SetDieDelayTimeTick(DWORD nTick) { m_nDieDelayTimeTick = nTick; }

	WORD						GetGrade();

	//----------------------------------------------------------------------------------------------------------------
	//							���� ��ƾ ó��
	VOID						MainLoop();	
};

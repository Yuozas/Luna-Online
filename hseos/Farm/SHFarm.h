/*********************************************************************

	 ����		: SHFarm.h
	 �ۼ���		: hseos
	 �ۼ���		: 2007/04/10

	 ���ϼ���	: ���� Ŭ������ ���

 *********************************************************************/

#pragma once

#include "SHGarden.h"
#include "Pen.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
class CSHFarm : public CSHFarmObj
{
public:
	//----------------------------------------------------------------------------------------------------------------
	enum FARM_EVENT										// ���� �̺�Ʈ
	{
		FARM_EVENT_NOTHING = 0,							// ..�ƹ��͵� �ƴ�
		FARM_EVENT_GARDEN,								// ..�Թ�
		FARM_EVENT_ANIMALCAGE,							// ..���
		FARM_EVENT_FENCE,								// ..�WŸ��
		FARM_EVENT_HOUSE,								// ..��
		FARM_EVENT_WAREHOUSE,							// ..â��
		FARM_EVENT_MAX = FARM_EVENT_WAREHOUSE,
	};

private:
	//----------------------------------------------------------------------------------------------------------------
	CSHGarden*				m_pcsGarden;								// �Թ�
	CSHFarmObj*				m_pcsFence;									// ��Ÿ��
	// 080410 KTH
	CPen*					m_pcsAnimalCage;							// ���
	CSHFarmObj*				m_pcsHouse;									// ��
	CSHFarmObj*				m_pcsWarehouse;								// â��

	int						m_nGardenNum;								// �Թ� ����
	int						m_nFenceNum;								// ��Ÿ�� ����
	int						m_nAnimalCageNum;							// ��� ����
	int						m_nHouseNum;								// �� ����
	int						m_nWarehouseNum;							// â�� ����

	BOOL					m_bRendering;								// ���� ���ΰ�

	UINT					m_nTaxArrearageFreq;						// ������ ��ü Ƚ��
	char					m_szTaxPayPlayerName[MAX_NAME_LENGTH+1];	// ������ ���� �÷��̾�

public:
	//----------------------------------------------------------------------------------------------------------------
	CSHFarm();
	~CSHFarm();

	//----------------------------------------------------------------------------------------------------------------
	//						�ʱ�ȭ
	VOID					Init();

	//----------------------------------------------------------------------------------------------------------------
	//						�Թ� ����
	VOID					CreateGarden(int nGardenNum);
	//						�Թ� ���
	CSHGarden*				GetGarden(int nGardenID)	{ return &m_pcsGarden[nGardenID]; }
	//						�Թ� ���� ���
	int						GetGardenNum()				{ return m_nGardenNum; }

	//----------------------------------------------------------------------------------------------------------------
	//						��Ÿ�� ����
	VOID					CreateFence(int nFenceNum);
	//						��Ÿ�� ���
	CSHFarmObj*				GetFence(int nFenceID)		{ return &m_pcsFence[nFenceID]; }
	//						��Ÿ�� ���� ���
	int						GetFenceNum()				{ return m_nFenceNum; }

	//----------------------------------------------------------------------------------------------------------------
	//						��� ����
	VOID					CreateAnimalCage(int nAnimalCageNum);
	//						��� ���
	CPen*					GetAnimalCage(int nAnimalCageID)	{ return &m_pcsAnimalCage[nAnimalCageID]; }
	//						��� ���� ���
	int						GetAnimalCageNum()					{ return m_nAnimalCageNum; }

	//----------------------------------------------------------------------------------------------------------------
	//						�� ����
	VOID					CreateHouse(int nHouseNum);
	//						�� ���
	CSHFarmObj*				GetHouse(int nHouseID)		{ return &m_pcsHouse[nHouseID]; }
	//						�� ���� ���
	int						GetHouseNum()				{ return m_nHouseNum; }

	//----------------------------------------------------------------------------------------------------------------
	//						â�� ����
	VOID					CreateWarehouse(int nWarehouseNum);
	//						â�� ���
	CSHFarmObj*				GetWarehouse(int nWarehouseID)		{ return &m_pcsWarehouse[nWarehouseID]; }
	//						â�� ���� ���
	int						GetWarehouseNum()					{ return m_nWarehouseNum; }

	VOID					SetIsRendering(BOOL bRendering)		{ m_bRendering = bRendering; }
	BOOL					GetIsRendering()					{ return m_bRendering; }

	//----------------------------------------------------------------------------------------------------------------
	//						������ ��ü Ƚ�� ����
	VOID					SetTaxArrearageFreq(UINT nFreq)		{ m_nTaxArrearageFreq = nFreq; }
	//						������ ��ü Ƚ�� ���
	UINT					GetTaxArrearageFreq()				{ return m_nTaxArrearageFreq; }
	//						������ ���� �÷��̾� ����
	VOID					SetTaxPayPlayerName(char* pszName)	{ pszName == NULL ? ZeroMemory(m_szTaxPayPlayerName, sizeof(m_szTaxPayPlayerName)) : SafeStrCpy(m_szTaxPayPlayerName, pszName, MAX_NAME_LENGTH+1); }
	//						������ ���� �÷��̾� ���
	char*					GetTaxPayPlayerName()				{ return m_szTaxPayPlayerName; }
	//						������ ���� ���� ���
	BOOL					IsPayTax()							{ return m_szTaxPayPlayerName[0]; }
};
/*********************************************************************

	 파일		: SHFarm.h
	 작성자		: hseos
	 작성일		: 2007/04/10

	 파일설명	: 농장 클래스의 헤더

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
	enum FARM_EVENT										// 농장 이벤트
	{
		FARM_EVENT_NOTHING = 0,							// ..아무것도 아님
		FARM_EVENT_GARDEN,								// ..텃밭
		FARM_EVENT_ANIMALCAGE,							// ..축사
		FARM_EVENT_FENCE,								// ..웉타리
		FARM_EVENT_HOUSE,								// ..집
		FARM_EVENT_WAREHOUSE,							// ..창고
		FARM_EVENT_MAX = FARM_EVENT_WAREHOUSE,
	};

private:
	//----------------------------------------------------------------------------------------------------------------
	CSHGarden*				m_pcsGarden;								// 텃밭
	CSHFarmObj*				m_pcsFence;									// 울타리
	// 080410 KTH
	CPen*					m_pcsAnimalCage;							// 축사
	CSHFarmObj*				m_pcsHouse;									// 집
	CSHFarmObj*				m_pcsWarehouse;								// 창고

	int						m_nGardenNum;								// 텃밭 개수
	int						m_nFenceNum;								// 울타리 개수
	int						m_nAnimalCageNum;							// 축사 개수
	int						m_nHouseNum;								// 집 개수
	int						m_nWarehouseNum;							// 창고 개수

	BOOL					m_bRendering;								// 렌더 중인가

	UINT					m_nTaxArrearageFreq;						// 관리비 연체 횟수
	char					m_szTaxPayPlayerName[MAX_NAME_LENGTH+1];	// 관리비 납부 플레이어

public:
	//----------------------------------------------------------------------------------------------------------------
	CSHFarm();
	~CSHFarm();

	//----------------------------------------------------------------------------------------------------------------
	//						초기화
	VOID					Init();

	//----------------------------------------------------------------------------------------------------------------
	//						텃밭 생성
	VOID					CreateGarden(int nGardenNum);
	//						텃밭 얻기
	CSHGarden*				GetGarden(int nGardenID)	{ return &m_pcsGarden[nGardenID]; }
	//						텃밭 개수 얻기
	int						GetGardenNum()				{ return m_nGardenNum; }

	//----------------------------------------------------------------------------------------------------------------
	//						울타리 생성
	VOID					CreateFence(int nFenceNum);
	//						울타리 얻기
	CSHFarmObj*				GetFence(int nFenceID)		{ return &m_pcsFence[nFenceID]; }
	//						울타리 개수 얻기
	int						GetFenceNum()				{ return m_nFenceNum; }

	//----------------------------------------------------------------------------------------------------------------
	//						축사 생성
	VOID					CreateAnimalCage(int nAnimalCageNum);
	//						축사 얻기
	CPen*					GetAnimalCage(int nAnimalCageID)	{ return &m_pcsAnimalCage[nAnimalCageID]; }
	//						축사 개수 얻기
	int						GetAnimalCageNum()					{ return m_nAnimalCageNum; }

	//----------------------------------------------------------------------------------------------------------------
	//						집 생성
	VOID					CreateHouse(int nHouseNum);
	//						집 얻기
	CSHFarmObj*				GetHouse(int nHouseID)		{ return &m_pcsHouse[nHouseID]; }
	//						집 개수 얻기
	int						GetHouseNum()				{ return m_nHouseNum; }

	//----------------------------------------------------------------------------------------------------------------
	//						창고 생성
	VOID					CreateWarehouse(int nWarehouseNum);
	//						창고 얻기
	CSHFarmObj*				GetWarehouse(int nWarehouseID)		{ return &m_pcsWarehouse[nWarehouseID]; }
	//						창고 개수 얻기
	int						GetWarehouseNum()					{ return m_nWarehouseNum; }

	VOID					SetIsRendering(BOOL bRendering)		{ m_bRendering = bRendering; }
	BOOL					GetIsRendering()					{ return m_bRendering; }

	//----------------------------------------------------------------------------------------------------------------
	//						관리비 연체 횟수 설정
	VOID					SetTaxArrearageFreq(UINT nFreq)		{ m_nTaxArrearageFreq = nFreq; }
	//						관리비 연체 횟수 얻기
	UINT					GetTaxArrearageFreq()				{ return m_nTaxArrearageFreq; }
	//						관리비 납부 플레이어 설정
	VOID					SetTaxPayPlayerName(char* pszName)	{ pszName == NULL ? ZeroMemory(m_szTaxPayPlayerName, sizeof(m_szTaxPayPlayerName)) : SafeStrCpy(m_szTaxPayPlayerName, pszName, MAX_NAME_LENGTH+1); }
	//						관리비 납부 플레이어 얻기
	char*					GetTaxPayPlayerName()				{ return m_szTaxPayPlayerName; }
	//						관리비 납부 상태 얻기
	BOOL					IsPayTax()							{ return m_szTaxPayPlayerName[0]; }
};
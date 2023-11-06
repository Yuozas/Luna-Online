/*********************************************************************

	 파일		: SHFarm.cpp
	 작성자		: hseos
	 작성일		: 2007/04/10

	 파일설명	: 농장 클래스의 소스

 *********************************************************************/

#include "stdafx.h"
#include "../Common/SHMath.h"
 
#include "SHFarm.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CSHFarm
//

// -------------------------------------------------------------------------------------------------------------------------------------
// CSHFarm Method																												  생성자
//
CSHFarm::CSHFarm()
{
	// 초기화
	m_pcsGarden = NULL;
	m_pcsFence = NULL;
	m_pcsAnimalCage = NULL;
	m_pcsHouse = NULL;
	m_pcsWarehouse = NULL;
	m_nGardenNum = 0;
	m_nEventKind = FARM_EVENT_GARDEN;
	m_bRendering = FALSE;
	SetTaxArrearageFreq(0);
	SetTaxPayPlayerName(0);
}

// -------------------------------------------------------------------------------------------------------------------------------------
// ~CSHFarm Method																												  파괴자
//
CSHFarm::~CSHFarm()
{
	SAFE_DELETE_ARRAY(m_pcsGarden);
	SAFE_DELETE_ARRAY(m_pcsFence);
	SAFE_DELETE_ARRAY(m_pcsAnimalCage);
	SAFE_DELETE_ARRAY(m_pcsHouse);
	SAFE_DELETE_ARRAY(m_pcsWarehouse);
}

// -------------------------------------------------------------------------------------------------------------------------------------
// Init Method																													  초기화
//
VOID CSHFarm::Init()
{
	SetOwnState(CSHFarmObj::OWN_STATE_EMPTY);
	SetOwner(0);
	SetGrade(0);
	GetGarden(0)->SetGrade(0);
	GetHouse(0)->SetGrade(0);
	GetWarehouse(0)->SetGrade(0);
	GetAnimalCage(0)->SetGrade(0);
	GetFence(0)->SetGrade(0);
	SetTaxArrearageFreq(0);
	SetTaxPayPlayerName(0);
}

// -------------------------------------------------------------------------------------------------------------------------------------
// CreateGarden Method																									하위 구성물 생성
//
VOID CSHFarm::CreateGarden(int nGardenNum)
{
	// 텃밭 생성
	SAFE_DELETE_ARRAY(m_pcsGarden);
	m_pcsGarden = new CSHGarden[nGardenNum];
	m_nGardenNum = nGardenNum;
	for(int i=0; i<nGardenNum; i++)
	{
		m_pcsGarden[i].SetID(i);
		m_pcsGarden[i].SetParent(this);
		AddChild((CSHFarmObj*)&m_pcsGarden[i], FARM_EVENT_GARDEN);
	}
}

// -------------------------------------------------------------------------------------------------------------------------------------
// CreateFence Method																									하위 구성물 생성
//
VOID CSHFarm::CreateFence(int nFenceNum)
{
	// 울타리 생성
	SAFE_DELETE_ARRAY(m_pcsFence);
	m_pcsFence = new CSHFarmObj[nFenceNum];
	m_nFenceNum = nFenceNum;
	for(int i=0; i<nFenceNum; i++)
	{
		m_pcsFence[i].SetID(i);
		m_pcsFence[i].SetParent(this);
		AddChild(&m_pcsFence[i], FARM_EVENT_FENCE);
	}
}

// -------------------------------------------------------------------------------------------------------------------------------------
// CreateAnimalCage Method																								하위 구성물 생성
//
VOID CSHFarm::CreateAnimalCage(int nAnimalCageNum)
{
	// 울타리 생성
	SAFE_DELETE_ARRAY(m_pcsAnimalCage);
	//m_pcsAnimalCage = new CSHFarmObj[nAnimalCageNum];
	// 080312 KTH
	m_pcsAnimalCage = new CPen[nAnimalCageNum];
	m_nAnimalCageNum = nAnimalCageNum;
	for(int i=0; i<nAnimalCageNum; i++)
	{
		m_pcsAnimalCage[i].SetID(i);
		m_pcsAnimalCage[i].SetParent(this);
		AddChild(&m_pcsAnimalCage[i], FARM_EVENT_ANIMALCAGE);
	}
}

// -------------------------------------------------------------------------------------------------------------------------------------
// CreateHouse Method																									하위 구성물 생성
//
VOID CSHFarm::CreateHouse(int nHouseNum)
{
	// 울타리 생성
	SAFE_DELETE_ARRAY(m_pcsHouse);
	m_pcsHouse = new CSHFarmObj[nHouseNum];
	m_nHouseNum = nHouseNum;
	for(int i=0; i<nHouseNum; i++)
	{
		m_pcsHouse[i].SetID(i);
		m_pcsHouse[i].SetParent(this);
		AddChild(&m_pcsHouse[i], FARM_EVENT_HOUSE);
	}
}

// -------------------------------------------------------------------------------------------------------------------------------------
// CreateWarehouse Method																								하위 구성물 생성
//
VOID CSHFarm::CreateWarehouse(int nWarehouseNum)
{
	// 울타리 생성
	SAFE_DELETE_ARRAY(m_pcsWarehouse);
	m_pcsWarehouse = new CSHFarmObj[nWarehouseNum];
	m_nWarehouseNum = nWarehouseNum;
	for(int i=0; i<nWarehouseNum; i++)
	{
		m_pcsWarehouse[i].SetID(i);
		m_pcsWarehouse[i].SetParent(this);
		AddChild(&m_pcsWarehouse[i], FARM_EVENT_WAREHOUSE);
	}
}
/*********************************************************************

	 파일		: SHFarmZone.cpp
	 작성자		: hseos
	 작성일		: 2007/04/10

	 파일설명	: 농장 지역 클래스의 소스

 *********************************************************************/

#include "stdafx.h"
#include "../Common/SHMath.h"
#include "SHFarmZone.h"


// -------------------------------------------------------------------------------------------------------------------------------------
// CSHFarmZone Method																											  생성자
//
CSHFarmZone::CSHFarmZone(FARM_ZONE eBelongZone, int nMapID, int nFarmNum)
{
	// 농장 생성 및 초기화
	m_csFarm = new CSHFarm[nFarmNum];
	for(int i=0; i<nFarmNum; i++)
	{
		m_csFarm[i].SetID(i);
		m_csFarm[i].SetParent(this);
		AddChild((CSHFarmObj*)&m_csFarm[i], FARM_ZONE_EVENT_FARM);
	}
	m_eBelongZone = eBelongZone;
	m_nMapID = nMapID;
	m_nFarmNum = nFarmNum;
	m_nEventKind = FARM_ZONE_EVENT_FARM;
}

// -------------------------------------------------------------------------------------------------------------------------------------
// ~CSHFarmZone Method																											  파괴자
//
CSHFarmZone::~CSHFarmZone()
{
	SAFE_DELETE_ARRAY(m_csFarm);
}
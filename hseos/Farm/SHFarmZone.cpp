/*********************************************************************

	 ����		: SHFarmZone.cpp
	 �ۼ���		: hseos
	 �ۼ���		: 2007/04/10

	 ���ϼ���	: ���� ���� Ŭ������ �ҽ�

 *********************************************************************/

#include "stdafx.h"
#include "../Common/SHMath.h"
#include "SHFarmZone.h"


// -------------------------------------------------------------------------------------------------------------------------------------
// CSHFarmZone Method																											  ������
//
CSHFarmZone::CSHFarmZone(FARM_ZONE eBelongZone, int nMapID, int nFarmNum)
{
	// ���� ���� �� �ʱ�ȭ
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
// ~CSHFarmZone Method																											  �ı���
//
CSHFarmZone::~CSHFarmZone()
{
	SAFE_DELETE_ARRAY(m_csFarm);
}
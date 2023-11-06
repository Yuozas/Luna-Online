/*********************************************************************

	 ����		: SHGarden.cpp
	 �ۼ���		: hseos
	 �ۼ���		: 2007/04/10

	 ���ϼ���	: �Թ� Ŭ������ �ҽ�

 *********************************************************************/

#include "stdafx.h"
#include "../Common/SHMath.h"
#include "SHGarden.h"


// -------------------------------------------------------------------------------------------------------------------------------------
// CSHGarden Method																												  ������
//
CSHGarden::CSHGarden()
{
	m_pcsCrop = NULL;
	m_nEventKind = GARDEN_EVENT_CROP;
}

// -------------------------------------------------------------------------------------------------------------------------------------
// ~CSHGarden Method																											  �ı���
//
CSHGarden::~CSHGarden()
{
	SAFE_DELETE_ARRAY(m_pcsCrop);
}

// -------------------------------------------------------------------------------------------------------------------------------------
// Create Method																										���۹� ���� ����
//
VOID CSHGarden::Create(int nCropNum)
{
	// ���۹� ����
	SAFE_DELETE_ARRAY(m_pcsCrop);
	m_pcsCrop = new CSHCrop[nCropNum];
	m_nCropNum = nCropNum;
	for(int i=0; i<nCropNum; i++)
	{
		m_pcsCrop[i].SetID(i);
		m_pcsCrop[i].SetParent(this);
		AddChild((CSHFarmObj*)&m_pcsCrop[i], GARDEN_EVENT_CROP);
	}
	CreateRenderObjEx(nCropNum);
}

// -------------------------------------------------------------------------------------------------------------------------------------
// MainLoop Method																									 �Թ� ���� ��ƾ ó��
//
VOID CSHGarden::MainLoop()
{
	CSHFarmObj::MainLoop();
}
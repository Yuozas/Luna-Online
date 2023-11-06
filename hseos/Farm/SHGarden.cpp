/*********************************************************************

	 파일		: SHGarden.cpp
	 작성자		: hseos
	 작성일		: 2007/04/10

	 파일설명	: 텃밭 클래스의 소스

 *********************************************************************/

#include "stdafx.h"
#include "../Common/SHMath.h"
#include "SHGarden.h"


// -------------------------------------------------------------------------------------------------------------------------------------
// CSHGarden Method																												  생성자
//
CSHGarden::CSHGarden()
{
	m_pcsCrop = NULL;
	m_nEventKind = GARDEN_EVENT_CROP;
}

// -------------------------------------------------------------------------------------------------------------------------------------
// ~CSHGarden Method																											  파괴자
//
CSHGarden::~CSHGarden()
{
	SAFE_DELETE_ARRAY(m_pcsCrop);
}

// -------------------------------------------------------------------------------------------------------------------------------------
// Create Method																										농작물 공간 생성
//
VOID CSHGarden::Create(int nCropNum)
{
	// 농작물 생성
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
// MainLoop Method																									 텃밭 메인 루틴 처리
//
VOID CSHGarden::MainLoop()
{
	CSHFarmObj::MainLoop();
}
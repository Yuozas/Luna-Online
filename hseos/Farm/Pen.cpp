/*********************************************************************

	 파일		: Pen.cpp
	 작성자		: Shinobi
	 작성일		: 2008/03/11

	 파일설명	: 축사 클래스의 소스

 *********************************************************************/

#include "stdafx.h"
#include "../Common/SHMath.h"
#include "Pen.h"


// -------------------------------------------------------------------------------------------------------------------------------------
// CPen Method																												  생성자
//
CPen::CPen()
{
	m_pcsAnimal = NULL;
	m_nEventKind = GARDEN_EVENT_ANIMAL;
}

// -------------------------------------------------------------------------------------------------------------------------------------
// ~CPen Method																											  파괴자
//
CPen::~CPen()
{
	SAFE_DELETE_ARRAY(m_pcsAnimal);
}

// -------------------------------------------------------------------------------------------------------------------------------------
// Create Method																										농작물 공간 생성
//
VOID CPen::Create(int nAnimalNum)
{
	// 농작물 생성
	SAFE_DELETE_ARRAY(m_pcsAnimal);
	m_pcsAnimal = new CAnimal[nAnimalNum];
	m_nAnimalNum = nAnimalNum;
	for(int i=0; i<nAnimalNum; i++)
	{
		m_pcsAnimal[i].SetID(i);
		m_pcsAnimal[i].SetParent(this);
		AddChild((CSHFarmObj*)&m_pcsAnimal[i], GARDEN_EVENT_ANIMAL);
	}
	CreateRenderObjEx(nAnimalNum);
}

// -------------------------------------------------------------------------------------------------------------------------------------
// MainLoop Method																									 텃밭 메인 루틴 처리
//
VOID CPen::MainLoop()
{
	CSHFarmObj::MainLoop();
}
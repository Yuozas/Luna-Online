/*********************************************************************

	 ����		: Pen.cpp
	 �ۼ���		: Shinobi
	 �ۼ���		: 2008/03/11

	 ���ϼ���	: ��� Ŭ������ �ҽ�

 *********************************************************************/

#include "stdafx.h"
#include "../Common/SHMath.h"
#include "Pen.h"


// -------------------------------------------------------------------------------------------------------------------------------------
// CPen Method																												  ������
//
CPen::CPen()
{
	m_pcsAnimal = NULL;
	m_nEventKind = GARDEN_EVENT_ANIMAL;
}

// -------------------------------------------------------------------------------------------------------------------------------------
// ~CPen Method																											  �ı���
//
CPen::~CPen()
{
	SAFE_DELETE_ARRAY(m_pcsAnimal);
}

// -------------------------------------------------------------------------------------------------------------------------------------
// Create Method																										���۹� ���� ����
//
VOID CPen::Create(int nAnimalNum)
{
	// ���۹� ����
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
// MainLoop Method																									 �Թ� ���� ��ƾ ó��
//
VOID CPen::MainLoop()
{
	CSHFarmObj::MainLoop();
}
/*********************************************************************

	 ����		: SHFarmObj.cpp
	 �ۼ���		: hseos
	 �ۼ���		: 2007/04/18

	 ���ϼ���	: ���� �⺻ ��ü Ŭ������ �ҽ�

 *********************************************************************/

#include "stdafx.h"
#include "../SHMain.h"
#include "SHFarmObj.h"

// -------------------------------------------------------------------------------------------------------------------------------------
// CSHFarmObj Method																											  ������
//
CSHFarmObj::CSHFarmObj()
{
	m_pcsParent = NULL;
	ZeroMemory(m_pcsChild, sizeof(m_pcsChild));
	m_nChildNum = 0;
	m_nID = 0;
	m_nOwnerID = 0;
	m_eOwnState = OWN_STATE_EMPTY;
	m_nLife = 0;
	m_nGrade = 0;
	ZeroMemory(&m_stEvent, sizeof(m_stEvent));
	m_nEventKind = 0;
	m_pcsRenderObj = NULL;
	ZeroMemory(m_ppcsRenderObjEx,	sizeof(m_ppcsRenderObjEx));
	m_nDir = 0.0f;
}

// -------------------------------------------------------------------------------------------------------------------------------------
// ~CSHFarmObj Method																											  �ı���
//
CSHFarmObj::~CSHFarmObj()
{
	if (m_pcsRenderObj)
	{
		m_pcsRenderObj->Release();
	} 

	int nCnt = sizeof(m_ppcsRenderObjEx)/sizeof(m_ppcsRenderObjEx[0]);
	for(int i=0; i<nCnt; i++)
	{
		if (m_ppcsRenderObjEx[i])
		{
			m_ppcsRenderObjEx[i]->Release();
		}
	}
}

// -------------------------------------------------------------------------------------------------------------------------------------
// CreateRenderObj																								 ȭ�� ��� ������Ʈ ����
//
VOID CSHFarmObj::CreateRenderObjEx(int nNum)
{
}

// -------------------------------------------------------------------------------------------------------------------------------------
// MainLoop Method																											   ���� ����
//
VOID CSHFarmObj::MainLoop()
{
	if (m_pcsChild)
	{
		for(int i=0; i<m_nChildNum; i++)
		{
			m_pcsChild[i]->MainLoop();
		}
	}
}
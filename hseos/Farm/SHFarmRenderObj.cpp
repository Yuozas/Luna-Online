/*********************************************************************

	 ����		: SHFarmRenderObj.cpp
	 �ۼ���		: hseos
	 �ۼ���		: 2007/05/07

	 ���ϼ���	: xxx Ŭ������ �ҽ�

 *********************************************************************/

#include "stdafx.h"
#include "SHFarmRenderObj.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CSHFarmRenderObj
//

// -------------------------------------------------------------------------------------------------------------------------------------
// CSHFarmRenderObj Method																										  ������
//
CSHFarmRenderObj::CSHFarmRenderObj()
{
}

// -------------------------------------------------------------------------------------------------------------------------------------
// ~CSHFarmRenderObj Method																										  �ı���
//
CSHFarmRenderObj::~CSHFarmRenderObj()
{
}

// -------------------------------------------------------------------------------------------------------------------------------------
// Init Method																													  �ʱ�ȭ
//
BOOL CSHFarmRenderObj::Init(EObjectKind kind, BASEOBJECT_INFO* pBaseObjectInfo, stFarmRenderObjInfo* pstFarmRenderObjInfo)
{
#if !defined(_AGENTSERVER)
	m_stFarmRenderObjInfo = *pstFarmRenderObjInfo;

	return CObject::Init(kind, pBaseObjectInfo);
#endif
	return TRUE;
}

// -------------------------------------------------------------------------------------------------------------------------------------
// Release Method																												 
//
VOID CSHFarmRenderObj::Release()
{
#if !defined(_AGENTSERVER)
	CObject::Release();
#endif
}
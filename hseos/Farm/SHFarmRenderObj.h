/*********************************************************************

	 ����		: SHFarmRenderObj.h
	 �ۼ���		: hseos
	 �ۼ���		: 2007/04/10

	 ���ϼ���	: CSHFarmRenderObj Ŭ������ ���
				  Ŭ���̾�Ʈ���� ���� ȭ�� ��°� ������ ���� ���� ������Ʈ Ŭ������

 *********************************************************************/

#pragma once

#if defined(_AGENTSERVER)
	class CObject
	{
	};

	struct BASEOBJECT_INFO;
	enum EObjectKind;
#else
	#include "Object.h"
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
class CSHFarmRenderObj : public CObject
{
public:
	//----------------------------------------------------------------------------------------------------------------
	typedef struct
	{
		DWORD	nID;
		WORD	nKind;
		WORD	nMapNum;
	} stFarmRenderObjInfo;

private:
	//----------------------------------------------------------------------------------------------------------------
	stFarmRenderObjInfo		m_stFarmRenderObjInfo;						// ���� ������Ʈ ����

public:
	//----------------------------------------------------------------------------------------------------------------
	CSHFarmRenderObj();
	virtual ~CSHFarmRenderObj();

	//----------------------------------------------------------------------------------------------------------------
	// �ʱ�ȭ
	virtual BOOL	Init(EObjectKind kind, BASEOBJECT_INFO* pBaseObjectInfo, stFarmRenderObjInfo* pstFarmRenderObjInfo);
	virtual void	Release();

	// ���� ������Ʈ ���� ���
	stFarmRenderObjInfo*	GetInfo()	{ return &m_stFarmRenderObjInfo; }

	// �̸� ����
	void			SetName(char* pszName)		
	{
		#if !defined(_AGENTSERVER)
			SafeStrCpy(m_BaseObjectInfo.ObjectName, pszName, MAX_NAME_LENGTH); 
		#endif
	}
};
/*********************************************************************

	 ����		: SHFarmObj.h
	 �ۼ���		: hseos
	 �ۼ���		: 2007/04/18

	 ���ϼ���	: ���� �⺻ ��ü Ŭ������ ���

 *********************************************************************/

#pragma once

#include "SHFarmRenderObj.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
class CSHFarmObj
{
public:
	//----------------------------------------------------------------------------------------------------------------
	static const int	MAX_CHILD_NUM					= 256;		// �ִ� �ڽ� ����

	//----------------------------------------------------------------------------------------------------------------
	enum OWN_STATE													// ���� ����						
	{
		OWN_STATE_EMPTY = 0,										// ..��� �ִ� ����										
		OWN_STATE_OWNED,											// ..�������� �����ϰ� �ִ� ����
		OWN_STATE_MAX = 100000,										// ..��
	};

	//----------------------------------------------------------------------------------------------------------------
	enum GRADE														// ���
	{
		GRADE_1 = 1,												 
		GRADE_2,
		GRADE_3,
		GRADE_4,
		GRADE_5,
		GRADE_6,
		GRADE_7,
		GRADE_8,
		MAX_GRADE = GRADE_8,
	};

	//----------------------------------------------------------------------------------------------------------------
	struct stEVENT													// �߻��� �̺�Ʈ
	{
		DWORD nOwnerID;												// ..������
		WORD nEvent[256];											// ..�̺�Ʈ
		WORD nEventNum;												// ..�̺�Ʈ ����
	};

protected:
	//----------------------------------------------------------------------------------------------------------------
	CSHFarmObj*						m_pcsParent;					// �θ�
	CSHFarmObj*						m_pcsChild[MAX_CHILD_NUM];		// �ڽ�
	WORD							m_nChildNum;					// �ڽ� ����

	//----------------------------------------------------------------------------------------------------------------
	WORD							m_nID;							// ID

	DWORD							m_nOwnerID;						// ������
	OWN_STATE						m_eOwnState;					// ��������
	WORD							m_nLife;						// ����
	WORD							m_nGrade;						// ���

	VECTOR3							m_stPos;						// ��ǥ
	float							m_nDir;							// ����

	//----------------------------------------------------------------------------------------------------------------
	stEVENT							m_stEvent;						// �̺�Ʈ
	WORD							m_nEventKind;					// �̺�Ʈ ����

	//----------------------------------------------------------------------------------------------------------------
	CSHFarmRenderObj*				m_pcsRenderObj;					// ȭ����� ������Ʈ.
	CSHFarmRenderObj*				m_ppcsRenderObjEx[30];			// ȭ����� Ȯ�� ������Ʈ.

public:
	//----------------------------------------------------------------------------------------------------------------
	CSHFarmObj();
	~CSHFarmObj();

	//----------------------------------------------------------------------------------------------------------------
	//								�θ� ����
	VOID							SetParent(CSHFarmObj* pcsParent)	{ m_pcsParent = pcsParent; }
	//								�θ� ���
	CSHFarmObj*						GetParent()							{ return m_pcsParent; }

	//----------------------------------------------------------------------------------------------------------------
	//								�ڽ� ����
	VOID							AddChild(CSHFarmObj* pcsChild, int nParentEvent = NULL)	
									{ 
										m_pcsChild[m_nChildNum] = pcsChild;
										m_pcsChild[m_nChildNum]->SetEventKind(nParentEvent);
										m_nChildNum++;
										if (m_nChildNum >= MAX_CHILD_NUM)
										{
											m_nChildNum = MAX_CHILD_NUM-1;
											#if defined(_DEBUG)
												MessageBox(NULL, "Overflow : CSHFarmObj::MAX_CHILD_NUM", NULL, NULL);
											#endif
										}
									}

	//----------------------------------------------------------------------------------------------------------------
	//								ID ����
	VOID							SetID(WORD nID)						{ m_nID = nID; }
	//								ID ���
	WORD							GetID()								{ return m_nID; }

	//----------------------------------------------------------------------------------------------------------------
	//								������ ����
	VOID							SetOwner(DWORD nOwnerID)			{ m_nOwnerID = nOwnerID; }
	//								������ ���
	DWORD							GetOwner()							{ return m_nOwnerID; }

	//----------------------------------------------------------------------------------------------------------------
	//								���� ���� ����
	VOID							SetOwnState(OWN_STATE eOwnState)	{ m_eOwnState = eOwnState; }
	//								���� ���� ���
	OWN_STATE						GetOwnState()						{ return m_eOwnState; }

	//----------------------------------------------------------------------------------------------------------------
	//								���� ����
	VOID							SetLife(WORD nLife)					{ m_nLife = nLife; }
	//								���� ���
	WORD							GetLife()							{ return m_nLife; }

	//----------------------------------------------------------------------------------------------------------------
	//								��� ����
	VOID							SetGrade(WORD nGrade)				{ m_nGrade = nGrade; }
	//								��� ���
	WORD							GetGrade()							{ return m_nGrade; }

	//----------------------------------------------------------------------------------------------------------------
	//								��ǥ ����
	VOID							SetPos(VECTOR3* pPos)				{ m_stPos = *pPos; }
	//								��ǥ ���
	VECTOR3*						GetPos()							{ return &m_stPos; }

	//----------------------------------------------------------------------------------------------------------------
	//								���� ����
	VOID							SetDir(float nDir)					{ m_nDir = nDir; }
	//								���� ���
	float							GetDir()							{ return m_nDir; }

	//----------------------------------------------------------------------------------------------------------------
	//								�̺�Ʈ �ʱ�ȭ
	VOID							InitEvent()
									{
										ZeroMemory(&m_stEvent, sizeof(m_stEvent));
									}
	//								�̺�Ʈ �߰�
	VOID							AddEvent(DWORD nOwnerID, WORD nEvent)
									{
										m_stEvent.nOwnerID = m_nOwnerID;
										m_stEvent.nEvent[m_stEvent.nEventNum] = nEvent;
										m_stEvent.nEventNum++;
									}
	//								�̺�Ʈ ���
	stEVENT*						GetEvent()							{ return &m_stEvent; }
	//								�̺�Ʈ ���� ����
	VOID							SetEventKind(WORD nEventKind)		{ m_nEventKind = nEventKind; }
	//								�̺�Ʈ ���� ���
	WORD							GetEventKind()						{ return m_nEventKind; }

	//----------------------------------------------------------------------------------------------------------------
	//								ȭ�� ��� ������Ʈ ����
	VOID							CreateRenderObjEx(int nNum);
	//								ȭ�� ��� ������Ʈ ����
	VOID							SetRenderObj(CSHFarmRenderObj* pObj)				{ m_pcsRenderObj = pObj; }
	VOID							SetRenderObjEx(CSHFarmRenderObj* pObj, int nID)		{ m_ppcsRenderObjEx[nID] = pObj; }
	//								ȭ�� ��� ������Ʈ ���
	CSHFarmRenderObj*				GetRenderObj()										{ return m_pcsRenderObj; }
	CSHFarmRenderObj*				GetRenderObjEx(int nID)								{ return m_ppcsRenderObjEx[nID]; }

	//----------------------------------------------------------------------------------------------------------------
	//								���� ����
	virtual VOID					MainLoop();
};
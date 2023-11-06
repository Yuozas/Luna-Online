/*
	GXSCHEDULE_PROC_MSG.h
	
	typedef.h �� ���ǵ� GXSchedulePROC �Լ��� �޽����� ������ ����.

	2002/05/30	ó�� ����.

	typedef DWORD (*GXSchedulePROC)(I4DyuchiGXExecutive* pExecutive,GXOBJECT_HANDLE gxh,DWORD msg,int arg1,int arg2,void* pData);
*/
#pragma once

#ifndef		_GXSCHEDULE_PROC_MSG_h_
#define		_GXSCHEDULE_PROC_MSG_h_

// �浹�� �Ͼ����.
#define		GXSCHEDULE_PROC_MSG_COLLISION				10

struct	GXSCHEDULE_PROC_MSG_COLLISION_ARG
{
	DWORD				dwCollisionType;	// �浹�� ������Ʈ�� Ÿ��.		GX_MAP_OBJECT_TYPE ���� �ϳ���.
	GXOBJECT_HANDLE		gxHandle;			// �浹�� ������Ʈ�� �ڵ�,		dwCollisionType�� ���� �ٸ��� �ؼ���... �ۿ��� ���� '������Ʈ'�ܿ� �ǹ̰� ��������.
	VECTOR3				vMeetPos;			// �浹�ȶ��� �Ǻ�.
	VECTOR3				vWhereMeet;			// ���� �浹 ���� �浹�� ����.
//	VECTOR3				vToCandidate;		// �浹�� ���� �������� �־��� �� To �ĺ�, �̶� �� From �ĺ��� vFromCandidate �� �ȴ�.
	PLANE				MeetPlane;
};

enum	GXSCHEDULE_PROC_MSG_COLLISION_RETURN
{
	GXSCHEDULE_PROC_MSG_COLLISION_RETURN_STOP,				// �����϶�.
	GXSCHEDULE_PROC_MSG_COLLISION_RETURN_SLIDE				// �̲�������.
//	GXSCHEDULE_PROC_MSG_COLLISION_RETURN_REFLECT			// �ݻ��϶�.
};

// ��ü�� �̵��� �̺�Ʈ �ڽ��� ��������. arg1�Ʒ��� ���ڰ� ���´�.
#define		GXSCHEDULE_PROC_MSG_MEET_EVENT_TRIGGER		11

struct	GXSCHEDULE_PROC_MSG_MEET_EVENT_TRIGGER_ARG
{
	DWORD				dwEventTriggerID;			// ���� �̺�ƮƮ������ ID
};

#endif

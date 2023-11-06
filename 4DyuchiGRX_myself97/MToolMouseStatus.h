// MToolMouseStatus.h: interface for the CToolMouseStatus class.
//
//////////////////////////////////////////////////////////////////////
/*
	���������� ���ư� �� �ִ� Ŭ������ �ƴϴ�.
*/
#if !defined(AFX_TOOLMOUSESTATUS_H__D1FE7C12_4295_41B2_90A3_5EC282CCA92A__INCLUDED_)
#define AFX_TOOLMOUSESTATUS_H__D1FE7C12_4295_41B2_90A3_5EC282CCA92A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../4DyuchiGRX_common/IGeometry.h"



class MToolMouseStatus  
{
public:

	// �ʱ�ȭ �Ҷ� �� �־�����Ѵ�.
	BOOL	Initialize(I4DyuchiGXGeometry* pGeometry);
	
	// WM_MOUSEMOVE �޽����� �޾����� ȣ���� �Լ�. dwFlag�� ��ư ����.
	bool MoveMouse( DWORD dwFlag, int iMouseX, int iMouseY);

	void	DoNotRotate(void);
	void	AllowRotate(void);

	void	PushCameraAngleAndPos(DWORD	dwStackNum);
	void	PopCameraAngleAndPos(DWORD	dwStackNum);

	MToolMouseStatus();
	virtual ~MToolMouseStatus();

protected:		// ������.
	int		iOldMouseX;
	int		iOldMouseY;
	int		iMouseX;
	int		iMouseY;
	int		iMouseZ;		// ��.

	float	fMoveHorizonStep;
	float	fMoveVerticalStep;
	float	fRotateStep;	// Shift�� ���� ���°� �޶����� ȸ��, �̵� �ӵ���.

	bool	bLButtonDown;
	bool	bRButtonDown;
	bool	bMButtonDown;
	bool	bCtrlDown;
	bool	bShiftDown;		// ����Ʈ ��ư�� ��� �̵�.

	DWORD	dwMouseMode;

	I4DyuchiGXGeometry*			m_pGeometry;

	BOOL	bDoNotRotate;

	VECTOR3		pCameraStackPos[10];
	float		pfCameraStackFromTo[10];
	VECTOR3		pCameraStackAngle[10];

protected:		// �Լ���.
	bool MouseNone(void);
	bool MouseRotate(void);
	bool MouseMoveVertical(void);
	bool MouseMoveHorizon(void);


};

#endif // !defined(AFX_TOOLMOUSESTATUS_H__D1FE7C12_4295_41B2_90A3_5EC282CCA92A__INCLUDED_)

// MHCamera.h: interface for the CMHCamera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MHCAMERA_H__3EE48679_085F_4E12_AF97_C06D8AB29A00__INCLUDED_)
#define AFX_MHCAMERA_H__3EE48679_085F_4E12_AF97_C06D8AB29A00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Angle.h"
#include ".\Engine\EngineCamera.h"

class CObject;
class CEngineObject;


struct MHCAMERASHAKE;


#define MAX_CAMERA_DESC	8
#define CAMERA	USINGTON(CMHCamera)

enum eCAMERAMODE
{
	eCM_Free = 0,
	eCM_QuarterView,
	eCM_FollowView,
	eCM_LOGIN,
	eCM_EyeView,
};

enum eCAMERASTATE
{
	eCS_Normal,
	eCS_Sit,
	eCS_Die,
};

enum
{
	eCP_CHARMAKE = 0,
	eCP_CHARSEL,
	eCP_INTERPOLATE,

	eCP_MAX,
};

struct MHCAMERAPOS
{
	float m_AngleX;
	float m_AngleY;
	float m_Dist;

	VECTOR3 m_Pivot;
};


struct MHCAMERADESC
{
	CAngle m_AngleX;
	CAngle m_AngleY;

	float m_fDistance;

	VECTOR3 m_CameraPos;
	VECTOR3 m_CameraDir;
	
	VECTOR3 m_CameraDownVector;
	VECTOR3 m_CameraDownPositon;

	BOOL m_bCameraAngleMovingX;
	DWORD m_CameraAngleMoveStartX;
	DWORD m_CameraAngleMoveDurationX;
	
	BOOL m_bCameraAngleMovingY;
	DWORD m_CameraAngleMoveStartY;
	DWORD m_CameraAngleMoveDurationY;

	BOOL m_bCameraDistanceMoving;
	DWORD m_CameraDistanceMoveStart;
	DWORD m_CameraDistanceMoveDuration;
	float m_fDistanceStart;
	float m_fDistanceChange;

	float m_CharHeight;			//����߿��� ī�޶� ������ ����������.
	float m_StartCharHeight;
	float m_TargetCharHeight;
	BOOL  m_bHeightChanging;
	DWORD m_dwHeightStartTime;
	DWORD m_dwHeightChangeDuration;

	CObject* m_pPivotObject;	// ���߿� �ϳ��� �� �־�� �Ѵ�.
	VECTOR3 m_PivotPos;

	BOOL m_bPivotObject;		// TRUE �� PivotObject FALSE �� PivotPos
};


class CMHCamera  
{
	//camerafilter
	CEngineObject* m_pCameraFilter;
	float m_MinAngle;
	float m_MaxAngle;
	
	VECTOR3 m_DownVector;
	DWORD m_CurCameraMode;
	CEngineCamera m_EngineCamera;

	MHCAMERADESC m_CameraDesc[MAX_CAMERA_DESC];
	int m_CurCamera;

	float m_fAddHeight;

//---sight distance option
	float m_fDistanceRate;	//50~100% 
	float m_fMaxSightDistance;
	float m_fFov;


	//////////////////////////////////////////////////////////////////////////
	// ���� ��� ������ ���� ������
	VECTOR3 m_VibratePos;
	VECTOR3 m_VibrateAngle;
	VECTOR3 m_PushedVector;
	DWORD m_VibrateDuration;
	DWORD m_VibrateStartTime;
	BOOL m_bIsVibrating;


	void CalcVibration();
	//////////////////////////////////////////////////////////////////////////	

	
	//////////////////////////////////////////////////////////////////////////
	// ����ũ ����� ���� ������
	MHCAMERASHAKE** m_ppCameraShakeDesc;		// ����ŷ �� ���� Desc ��..
	VECTOR3 m_ShakingPos;
	BOOL m_bIsShaking;
	DWORD m_nCurShakingNum;						// ���� �ϰ� �ִ� ����ŷ Desc Index (-1�̸� �ϰ� ���� �ʴ�)
	DWORD m_CurShakingPos;						// ���� �������� ����ŷ Desc�������� ��ġ
	DWORD m_LastShakingPosChangedTime;			// ���������� m_CurShakingPos�� �ٲ� �ð�.
	float m_Amplitude;

	void CalcShaking();
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// �̵������� ���� ������
	MHCAMERAPOS m_MovePos[eCP_MAX];
	int m_CurPosition;
	int m_GotoPosition;
	
	float m_MoveTime;
	float m_MoveStartTime;
	float m_MoveSpeed;
	
	BOOL m_bMoved;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// ī�޶������̵��� ���� ������
	DWORD m_LastMoveNoticeTime;			// ĳ���Ͱ� �̵��ߴٰ� ������ �˷��� �ð�



	//////////////////////////////////////////////////////////////////////////
	
	
	void CalcCameraPosAngle();
	void CalcFreeCamera();
	void ProcessCameraMode();
	void ProcessCameraMoving();

	void RestrictCameraAngleDistance();
	
	void FollowCamera();

	//---KES Map GXObject Alpha 071020
	cPtrList m_AlphaHandleList;
	//-----------------------------

public:
	CMHCamera();
//	//MAKESINGLETON(CMHCamera);
	~CMHCamera();

	//---KES Map GXObject Alpha 071020
	void GXOAlphaProcess();
	//-----------------------------

	void Init(CObject* pObject,float angleX,float angleY,float fDistance);
	void InitCamera(int CameraNum,float angleX,float angleY,float fDistance,CObject* pObject);
	void InitCamera(int CameraNum,float angleX,float angleY,float fDistance,VECTOR3* pPivotPos);

	void SetToEngineCamera();


	void SetCameraFilter(CEngineObject* pEngineObj,float minAngle = 0,float maxAngle = gPHI/4);


	void MouseRotate(int x,int y);

	void Process();

	void SetCurCamera(int Camera);
	void SetCameraPivotObject(int Camera,CObject* pObject);
	void SetCameraPivotPos(int Camera,VECTOR3* pPos);

	void SetTargetAngleX(int Camera,float fAngleX,DWORD Duration);
	void SetTargetAngleY(int Camera,float fAngleY,DWORD Duration);
	void SetDistance(int Camera,float fDistance,DWORD Duration);
	float GetDistance(int Camera) { return m_CameraDesc[Camera].m_fDistance; }

	void Rotate(int Camera,float RotateAngle,DWORD Duration);
	void ZoomDistance(int Camera,float fDistanceChange,DWORD Duration);

	void ChangeCameraSmooth(int Camera,DWORD Duration);	// ���� ī�޶󿡼� �ٸ� ī�޶�� �ε巴�� �̵��Ѵ�.
	void ChangeToDefaultCamera();	// ���� ī�޶��� �������� �״�� 0�� ī�޶� �����Ѵ�.
	void StopAllMoving(int Camera);	// ȸ���̳� �ܵ��� �����.

	void SetMaxSightDistance( float fMaxDistance )		{ m_fMaxSightDistance = fMaxDistance; m_fDistanceRate = 0.0f;  }
	void SetCameraFov( float fFov )						{ m_fFov = fFov; }

	void SetSightDistanceRate( float fDistanceRate );


	//////////////////////////////////////////////////////////////////////////
	// ��������� ���� �Լ���
	void SetPushedVibration(VECTOR3* PushedVector,DWORD Duration);
	//////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////
	// ����ŷ ����� ���� �Լ���
	void SetShaking(int ShakeNum, float amp);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// �̵������� ���� �Լ���
	void SetMovePos(int Index, MHCAMERAPOS* pCameraPos);
	void MoveStart(int GotoIndex);
	void Interpolation();
	BOOL IsMoved()				{	return m_bMoved;	}
	int GetGotoPosition()		{	return m_GotoPosition;	}
	void ResetInterPolation();
	//////////////////////////////////////////////////////////////////////////


	int GetCameraMode() { return m_CurCameraMode; }
	void SetCameraMode(int CameraMode);	
	void ToggleCameraViewMode();
	
	void SetCharState( int eCharState, DWORD dwDuration );
//	BOOL CanChangeCameraAngleTo( float xRad, float yRad );
	void Fly( float fFly );
	
	// ���� ī�޶� ������ �������� �Լ�
	MHCAMERADESC* GetCameraDesc() { return &m_CameraDesc[m_CurCamera]; }
};
EXTERNGLOBALTON(CMHCamera)
#endif // !defined(AFX_MHCAMERA_H__3EE48679_085F_4E12_AF97_C06D8AB29A00__INCLUDED_)

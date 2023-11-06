#include "MyMouseLib.h"
#include "../4dyuchiGRX_myself97/myself97common.h"
#include <math.h>

// �����ʹ� ������ ���� ����.(0,1,0)
// z, x, y ������ ������ �ɵ�.
void RotateCamera(  CAMERA_DESC& cd, float x, float y, float z)
{


	CAMERA_DESC		Camera;
	// �������� Ʈ������.
	VECTOR3		CameraTo, CameraUp;
	CameraTo.x = cd.v3To.x - cd.v3From.x;
	CameraTo.y = cd.v3To.y - cd.v3From.y;
	CameraTo.z = cd.v3To.z - cd.v3From.z;
	CameraUp.x = cd.v3Up.x - cd.v3From.x;
	CameraUp.y = cd.v3Up.y - cd.v3From.y;
	CameraUp.z = cd.v3Up.z - cd.v3From.z;

	float	fNewX, fNewY, fNewZ;
	float	fUpX, fUpY, fUpZ;
	float	fXRot, fYRot, fZRot;

	// ������ fXRot, fYRot, fZRot�� ���Ѵ�.
	// ������ fXRot.

	float	fLength	=	sqrt(CameraTo.x*CameraTo.x + CameraTo.z*CameraTo.z);
	if( fLength == 0)	// ���� �Ʒ��� ���� ������.
	{
		if( CameraTo.z > 0)		fXRot	=	0.0f;
		if( CameraTo.z < 0)		fXRot	=	180.0f;
		if( CameraTo.z == 0)
		{
			_asm int 3;		// ī�޶� �߸� ���õǾ��ִ� ���.
			fXRot	=	90.0f;
		}
	}
	else
	{
		fXRot	=	(atan2( CameraTo.y, fLength))*180/3.1415926f;
	}

	//	������ fYRot.
	fYRot = atan2( CameraTo.z, CameraTo.x);
	// 180���� ���ؾ��ϳ�.?




//struct CAMERA_DESC
//{
//	VECTOR3			v3From;
//	VECTOR3			v3To;
//	MATRIX4*		pMatrixResultFrom;
//	MATRIX4*		pMatrixResultTo;
//
//	VECTOR3			v3Up;			// ī�޶��� up.
//	float			fYRot;			// ���� ��ǥ����� ī�޶��� ȸ�� ����. yaw.
//	float			fXRot;			// ���� ��ǥ����� ī�޶��� ���� ����. pitch.
//};

//	VECTOR3	OnXZPlane;
//	OnXZPlane.x = CameraTo.x;
//	OnXZPlane.y = 0;
//	OnXZPlane.z = CameraTo.z;
//	float	fNewX	=	(CameraTo.x * (cos(y)*cos(z) + sin(x)*sin(y)*sin(z))) + (CameraTo.y * ( cos(y)*sin(z)-sin(x)*sin(y)*cos(z)) + CameraTo.z * (cos(x)*sin(y)));
//	float	fNewY	=	(CameraTo.x * (-1*cos(x)* sin(z))) + (CameraTo.y * (cos(x)*cos(z))) + (CameraTo.z * (sin(x)));
//	float	fNewZ	=	(CameraTo.x * (-1*sin(y)*cos(z) + sin(x)*cos(y)*sin(z))) + (CameraTo.y * (-1*sin(y)*sin(z) - sin(x)*cos(y)*cos(z))) + (CameraTo.z * (cos(x)*cos(y)));

	fNewX	=	(CameraTo.x * (cos(y)*cos(z) - sin(x)*sin(y)*sin(z))) + (CameraTo.y * (cos(x)*sin(z))) + (CameraTo.z * (sin(y)*cos(z) + sin(x)*cos(y)*sin(z)));
	fNewY	=	(CameraTo.x * (-1*cos(y)*sin(z) - sin(x)*sin(y)*cos(z))) + (CameraTo.y * (cos(x)*cos(z)) ) + (CameraTo.z * (-1*sin(y)*sin(z) + sin(x)*cos(y)*cos(z)));
	fNewZ	=	(CameraTo.x * (-1*cos(x)*sin(y))) + (CameraTo.y * (-1*sin(x))) + (CameraTo.z * (cos(x)*cos(y)));

	fUpX	=	(CameraUp.x * (cos(y)*cos(z) - sin(x)*sin(y)*sin(z))) + (CameraUp.y * (cos(x)*sin(z))) + (CameraUp.z * (sin(y)*cos(z) + sin(x)*cos(y)*sin(z)));
	fUpY	=	(CameraUp.x * (-1*cos(y)*sin(z) - sin(x)*sin(y)*cos(z))) + (CameraUp.y * (cos(x)*cos(z)) ) + (CameraUp.z * (-1*sin(y)*sin(z) + sin(x)*cos(y)*cos(z)));
	fUpZ	=	(CameraUp.x * (-1*cos(x)*sin(y))) + (CameraUp.y * (-1*sin(x))) + (CameraUp.z * (cos(x)*cos(y)));
	// rotate z
//	fNewX	=	CameraTo.x * cos(z) + CameraTo.y * sin(z);
//	fNewY	=	CameraTo.y * -1 * sin(z) + CameraTo.y * cos(z);
//	fNewZ	=	CameraTo.z * 1;
	

	// rotate x
//	fNewX	=	CameraTo.x * 1;
//	fNewY	=	CameraTo.y * cos(x) + CameraTo.z * sin(x);
//	fNewZ	=	CameraTo.y * -1 * sin(x) + CameraTo.z * cos(x);


	// rotate y
	fNewX	=	CameraTo.x * cos(y) + CameraTo.z * sin(y);
	fNewY	=	CameraTo.y * 1;
	fNewZ	=	CameraTo.x * -1 * sin(y) + CameraTo.z * cos(y);
	

	cd.v3To.x = cd.v3From.x + fNewX;
	cd.v3To.y = cd.v3From.y + fNewY;
	cd.v3To.z = cd.v3From.z + fNewZ;

	cd.v3Up.x = cd.v3From.x + fUpX;
	cd.v3Up.y = cd.v3From.y + fUpY;
	cd.v3Up.z = cd.v3From.z + fUpZ;

}

CAMERA_DESC MoveCamera( const CAMERA_DESC& cd, float x, float y, float z)
{
	CAMERA_DESC		Camera;

	Camera.v3From.x = 	cd.v3From.x + x;
	Camera.v3From.y = 	cd.v3From.y + y;
	Camera.v3From.z = 	cd.v3From.z + z;

	Camera.v3To.x	=	cd.v3To.x + x;
	Camera.v3To.y	=	cd.v3To.y + y;
	Camera.v3To.z	=	cd.v3To.z + z;
	

	return Camera;
}


/*
struct CAMERA_DESC
{
	VECTOR3			v3From;
	VECTOR3			v3To;
	MATRIX4*		pMatrixResultFrom;
	MATRIX4*		pMatrixResultTo;
};
*/
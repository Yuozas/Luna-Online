
#ifndef	_CollisionTestMovingEllipsoidMeetTriangle_h_
#define _CollisionTestMovingEllipsoidMeetTriangle_h_

#include	"../myself97common.h"

/*
// ����� Ÿ��ü.
struct	AAELLIPSOID
{
	VECTOR3		P;			// �Ǻ�, ���밪.
	float		w;			// x, z�� ����.
	float		h;			// y���� ����.
};
*/
struct		MOVING_ELLIPSOID
{
	VECTOR3		From;
	float		fWidth;
	float		fHeight;
	VECTOR3		Velocity;
};
/*
struct		ELLIPSOID_COLLISION_INFO
{
	VECTOR3		MeetPivot;		// ���������� �Ǻ�.
	float		fMeetTime;		// ���������� from-to�� ����.

	VECTOR3		WhereMeet;		// ���� ����.
	PLANE		MeetPlane;		// ���� ������ ���.
};
*/
// �����̴� Ÿ���� �ﰢ���� �־������� ���� ������ ������ ���Ѵ�.
// ���� ��� TRUE, �ȸ������ FALSE�� �����Ѵ�.
BOOL	CollisionTestMovingEllipsoidMeetTriangle( COLLISION_RESULT* pOut, MOVING_ELLIPSOID* pEllipsoid, VECTOR3* pTriangle);

// Ÿ���� Ÿ������ �� ���� �־����� �� ������� ���Ѵ�.
// Ÿ���� �־��� ���� ������ ���� ��� FALSE ���� �����̴�.
BOOL	CalculatePLANEOnEllipsoidWithVertex( PLANE* pOutPlane, AAELLIPSOID* pEllipsoid, VECTOR3* pVertex);

// �����̴� Ÿ���� ��� Ÿ���� �־����� �� ���� ������ ������ ���Ѵ�.
// ���� ��� TRUE, �ȸ��� ��� FALSE�� �����Ѵ�.


// �̵��ϴ� Ÿ���� �־��� ����� �������� �δ�.
BOOL	PushEllipsoidFromPLANE( VECTOR3* pOut, COLLISION_RESULT* pCollisionInfo, VECTOR3* pCurrentPivot, VECTOR3* pTo);


#endif

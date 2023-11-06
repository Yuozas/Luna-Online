
#include "CollisionTestMovingEllipsoidMeetTriangle.h"
#include "CollisionTestMovingSphereMeetTriangle.h"
#include	"../../4DyuchiGXGFunc/global.h"



// �����̴� Ÿ���� �ﰢ���� �־������� ���� ������ ������ ���Ѵ�.
// ���� ��� TRUE, �ȸ������ FALSE�� �����Ѵ�.
//BOOL	CollisionTestMovingEllipsoidMeetTriangle( VECTOR3* pOutWhereMeet, float* pOutT, MOVING_ELLIPSOID* pEllipsoid, VECTOR3* pTriangle)
BOOL	CollisionTestMovingEllipsoidMeetTriangle( COLLISION_RESULT* pOut, MOVING_ELLIPSOID* pEllipsoid, VECTOR3* pTriangle)
{
	// �ӵ� ���� üũ, �������� �ʴ� Ÿ��ü��� �ٷ� �i�Ƴ���.
	float	fVelocity	=	VECTOR3Length( &pEllipsoid->Velocity);
	if( fVelocity == 0.0f)
		return	FALSE;

	float		fFactorUnit	=	100.0f;		// �������� ũ��. 1.0f���� �ϸ� ����� 1.0�� �Ѱ�� ���� �͵�� ������ �����.
//	float		fFactorUnit	=	pEllipsoid->fWidth;


	// x, z������ �״�� �ΰ� y���⸸ x, z�� ���� ������ �Ѵ�.

	BOOL	bResult;
	// �־��� Ÿ��ü�� ���� ���Ǿ�(?)�� �����, �־��� �ﰢ���� ���� ���Ѵ�.
	float		fXFactor;			// Ÿ��ü�� x ������, �ﰢ���� ��ǥ�� �̰Ϳ� ���� �پ���.
	fXFactor	=	pEllipsoid->fWidth	/	fFactorUnit;
	float		fYFactor;			// Ÿ��ü�� ���� ������.
	fYFactor	=	pEllipsoid->fHeight		/	fFactorUnit;
	float		fZFactor	=	fXFactor;	// x�� z�� ����.

	VECTOR3		pST[3];			// ���Ǿ� �������� �ٲ� pTri;
	pST[0].x	=	pTriangle[0].x	/	fXFactor;
	pST[0].y	=	pTriangle[0].y	/	fYFactor;
	pST[0].z	=	pTriangle[0].z	/	fZFactor;
	pST[1].x	=	pTriangle[1].x	/	fXFactor;
	pST[1].y	=	pTriangle[1].y	/	fYFactor;
	pST[1].z	=	pTriangle[1].z	/	fZFactor;
	pST[2].x	=	pTriangle[2].x	/	fXFactor;
	pST[2].y	=	pTriangle[2].y	/	fYFactor;
	pST[2].z	=	pTriangle[2].z	/	fZFactor;

	MOVING_SPHERE	Sphere;		// ���Ǿ� �� �ٲ� Ÿ��ü.
	Sphere.fRadius	=	fFactorUnit;
	Sphere.From.x	=	pEllipsoid->From.x	/	fXFactor;
	Sphere.From.y	=	pEllipsoid->From.y	/	fYFactor;
	Sphere.From.z	=	pEllipsoid->From.z	/	fZFactor;
	Sphere.Velocity.x	=	pEllipsoid->Velocity.x / fXFactor;
	Sphere.Velocity.y	=	pEllipsoid->Velocity.y / fYFactor;
	Sphere.Velocity.z	=	pEllipsoid->Velocity.z / fZFactor;

	// ������� ���Ǿ �׽�Ʈ.
	float		fMeetT;
	VECTOR3		WhereMeet;
	DWORD		dwLastComponentType;
	BOOL	bFound	=	CollisionTestMovingSphereMeetTriangle( &dwLastComponentType, &WhereMeet, &fMeetT, &Sphere, pST);

	// ������ �ִ°�.?
	if( bFound == FALSE)
	{
		return	FALSE;
	}

	if( dwLastComponentType	== 10)
		_asm nop;

//	������ ����.
	pOut->fMeetTime		=	fMeetT;
//	������ ����.
	pOut->vMeetPivot		=	pEllipsoid->From	+	(pEllipsoid->Velocity * fMeetT);
//	������ ��.
	pOut->vWhereMeet.x	=	WhereMeet.x	*	fXFactor;
	pOut->vWhereMeet.y	=	WhereMeet.y	*	fYFactor;
	pOut->vWhereMeet.z	=	WhereMeet.z	*	fZFactor;
//	������ ���.
	PLANE		OutPlane;
	AAELLIPSOID		EllipsoidOnMeetPos;
	EllipsoidOnMeetPos.P	=	pOut->vMeetPivot;
	EllipsoidOnMeetPos.h	=	pEllipsoid->fHeight;
	EllipsoidOnMeetPos.w	=	pEllipsoid->fWidth;
	bResult	=	CalculatePLANEOnEllipsoidWithVertex( &OutPlane, &EllipsoidOnMeetPos, &pOut->vWhereMeet);
	if( bResult == FALSE)
		_asm int 3;	// �̷����� ���� ����� �Ѵ�.
	pOut->MeetPlane		=	OutPlane;

	// ������ ���� �ӵ����Ͱ� �����Ѱ�.?
	float	fMeetPlaneDotVelocity	=	DotProduct(&(pOut->MeetPlane.v3Up), &(pEllipsoid->Velocity));
	if( fMeetPlaneDotVelocity >= 0.0f)
	{
		_asm nop;		// �ϸ� ������ �־ �� �̷����� ����°�.?
		return		FALSE;
	}

	// ��??? ��? ��? �̷� ���� ����� ���� �ȵȴ�.
	float	fDotPlaneVelocity	=	DotProduct( &pEllipsoid->Velocity, &OutPlane.v3Up);
	if( fDotPlaneVelocity >= 0.001f)
		_asm int 3;

	// ó�� �־��� �ӵ����� �� �ָ����� �����ٰ� �����Ǵ� ���?
	float	fFP	=	VECTOR3Length(&(pOut->vMeetPivot - pEllipsoid->From));
	if( fFP > fVelocity)
	{
		return	FALSE;
	}

	pOut->dwComponentType	=	dwLastComponentType;

	


	return	TRUE;
}

// Ÿ���� Ÿ������ �� ���� �־����� �� ������� ���Ѵ�.
// Ÿ���� �־��� ���� ������ ���� ��� FALSE ���� �����̴�.
BOOL	CalculatePLANEOnEllipsoidWithVertex( PLANE* pOutPlane, AAELLIPSOID* pEllipsoid, VECTOR3* pVertex)
{
#ifdef	_DEBUG
	// pVertex�� pEllipsoid ���� �ִ°� �ϴ� �˻�.
	float	fDetX	=	(pVertex->x - pEllipsoid->P.x);
	float	fDetZ	=	(pVertex->z - pEllipsoid->P.z);
	float	fDetY	=	(pVertex->y - pEllipsoid->P.y);

	float	fDet1	=	((fDetX * fDetX) + (fDetZ * fDetZ)) / (pEllipsoid->w * pEllipsoid->w);
	float	fDet2	=	(fDetY*fDetY)/(pEllipsoid->h*pEllipsoid->h);

	float	fDet	=	fDet1	+	fDet2;

	if( fDet > 1.00001f || fDet < -0.99999f)
		_asm nop;
#endif




/*
	Ÿ������ �־��� �� ��(x1, y1, z1)
	Ÿ���� ���� (xc, yx, zc) ��� �� ��

	(x1 - xc)(x - xc)   (y1 - yc)(y - yc)   (z1 - zc)(z - zc)
	----------------- + ----------------- + -----------------    =    1
	       a*a                 b*b                 b*b


	(x1 - xc)         (y1 - yc)         (z1 - zc)         (x1 - xc) * xc     (y1 - yc) * yc     (z1 - zc) * zc
	--------- * x  +  --------- * y  +  --------- * z  -  --------------  -  --------------  -  --------------  -  d   =   0
	   a*a               b*b               c*c                  a*a                 b*b               c*c



1) Ÿ���� ���ϴ� �����

			(x1 - xc)
	N.x =	---------
			   a*a

			(y1 - yc)
	N.y =	---------
			   b*b

			(z1 - zc)
	N.z =	---------
			   c*c

			   (x1 - xc) * xc     (y1 - yc) * yc     (z1 - zc) * zc
����� d = 	-  --------------  -  --------------  -  --------------  -  1
					a*a                 b*b               c*c


2) �̶� ������ ����� �븻 ������ Ÿ���� �ٱ����̹Ƿ�, ����� a, b, c, d ������ -�� ���ؼ� �븻 ������ �������ش�.

	N.x = -N.x;
	N.y = -N.y;
	N.z = -N.z;
	d = -d


3) �״��� �븻������.

*/  

	float	x1xc	=	pVertex->x - pEllipsoid->P.x;
	float	y1yc	=	pVertex->y - pEllipsoid->P.y;
	float	z1zc	=	pVertex->z - pEllipsoid->P.z;

	float	aa		=	pEllipsoid->w * pEllipsoid->w;
	float	bb		=	pEllipsoid->h * pEllipsoid->h;
	float&	cc		=	aa;




	PLANE	OnEllipsoid;
// 1)	�����.
	OnEllipsoid.v3Up.x	=	x1xc / aa;
	OnEllipsoid.v3Up.y	=	y1yc / bb;
	OnEllipsoid.v3Up.z	=	z1zc / cc;
	OnEllipsoid.D		=	-1.0f * (x1xc * pEllipsoid->P.x / aa) - (y1yc * pEllipsoid->P.y / bb) - (z1zc * pEllipsoid->P.z / cc) - 1;

// 2, 3) �� ������ �븻������.
	float	fPlaneLength	=	(float)sqrt( OnEllipsoid.v3Up.x * OnEllipsoid.v3Up.x  +  OnEllipsoid.v3Up.y * OnEllipsoid.v3Up.y  +  OnEllipsoid.v3Up.z * OnEllipsoid.v3Up.z );
	OnEllipsoid.v3Up	=	(OnEllipsoid.v3Up	/	fPlaneLength)	*	-1.0f;
	OnEllipsoid.D		=	OnEllipsoid.D		/	fPlaneLength	*	-1.0f;;

	*pOutPlane		=	OnEllipsoid;

	
#ifdef	_DEBUG	
	// �鿡�� �Ÿ��� �󸶳� �������ֳ�? �׽�Ʈ�ڵ�.
	float	fDebug	=	DotProduct( pVertex, &(pOutPlane->v3Up))	+	pOutPlane->D;
	if( fDebug > 0.1f || fDebug < -0.1f)
	{
		_asm int 3;
		return	FALSE;
	}
#endif



	return	TRUE;
}

// �̵��ϴ� Ÿ���� �־��� ����� �������� �δ�.
BOOL	PushEllipsoidFromPLANE( VECTOR3* pOut, COLLISION_RESULT* pCollisionInfo, VECTOR3* pCurrentPivot, VECTOR3* pVelocity)
{
#ifdef	_DEBUG
	BOOL	bResult;
	bResult	=	IsValidFloat( pCollisionInfo->fMeetTime);
	if( bResult == FALSE)
		_asm int 3;
	bResult	=	IsValidVector3( &pCollisionInfo->vMeetPivot, 1);
	if( bResult == FALSE)
		_asm int 3;
	bResult	=	IsValidVector3( &pCollisionInfo->MeetPlane.v3Up, 1);
	if( bResult == FALSE)
		_asm int 3;
	bResult	=	IsValidVector3( &pCollisionInfo->vWhereMeet, 1);
	if( bResult == FALSE)
		_asm int 3;
	// Ȥ�� ���� ���� ���� ������ ����?�̸�.?
	float	fDotPlaneVelocity	=	DotProduct( pVelocity, &pCollisionInfo->MeetPlane.v3Up);
	if( fDotPlaneVelocity >= 0.0f)
		_asm int 3;								// ���⿡ �ɸ��� �ۿ�, �浹ó������ �ﰢ���� ���� ���Ͷ� ����̶� ���ϴµ��� �Ӱ�ġ�� �����Ѵ�.
#endif

	VECTOR3		To	=	*pCurrentPivot + *pVelocity;

	//	���Ҷ� Ÿ���� ������ ����鰣�� �Ÿ�.
	float	a	=	pCollisionInfo->vMeetPivot * pCollisionInfo->MeetPlane.v3Up	+	pCollisionInfo->MeetPlane.D;
	if( a <= 0.0f)
		_asm int 3;		// �̹� ����� ���� ���ٴ� ���ΰ�..

	//	�� Ÿ��ü�� �������� ����� �Ÿ�.
	float	b	=	To	*	pCollisionInfo->MeetPlane.v3Up		+		pCollisionInfo->MeetPlane.D;

	
	//	���� ���⺤�ͷ� pTo�� �о�� �� ũ��.
	float	c	=	a	-	b;//	+	1.0f;

	//	c�� ����� ���⺤��.
	VECTOR3		Push;
	Push	=	pCollisionInfo->MeetPlane.v3Up		*	c;

	// ��� �з��ִ� ��ǥ��,
	*pOut	=	To	+	Push;

	// ����.
	return	TRUE;
}

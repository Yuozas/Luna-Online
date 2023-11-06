
#include "CollisionTestMovingSphereMeetTriangle.h"
#include	"../../4DyuchiGXGFunc/global.h"

const	float	fTestValue	=	-0.001f;			// �ϴ� ����?

// �����̴� ���Ǿ�� �ﰢ���� ���κ��� ������ ��.
BOOL	TestMovingSphereMeetPLANE2( VECTOR3* pOutWhereMeet, float* pOutT, MOVING_SPHERE* pSphere, VECTOR3* pTriangle);

// �����̴� ���Ǿ�� �ﰢ���� ������ ������ ��.
BOOL	TestMovingSphereMeetTriEdge2( VECTOR3* pOutWhereMeet, float* pOutT, MOVING_SPHERE* pSphere, VECTOR3* pTriangle);

// �����̴� ���Ǿ�� ������ ������ ��.
BOOL	TestMovingSphereMeetLine2( VECTOR3* pOutWhereMeet, float* pOutT, MOVING_SPHERE* pSphere, VECTOR3* pLFrom, VECTOR3* pLTo);
BOOL	TestMovingSphereMeetLine3( VECTOR3* pOutWhereMeet, float* pOutT, MOVING_SPHERE* pSphere, VECTOR3* pLFrom, VECTOR3* pLTo);

// �����̴� ���Ǿ�� ���� ������ ��.
BOOL	TestMovingSphereMeetVertex2( VECTOR3* pOutWhereMeet, float* pOutT, MOVING_SPHERE* pSphere, VECTOR3* pVertex);

BOOL	CollisionTestMovingSphereMeetTriangle( DWORD* pdwOutComponentType, VECTOR3* pOutWhereMeet, float* pOutT, MOVING_SPHERE* pInSphere, VECTOR3* pInTriangle)
{
	// �ϴ�, �ӵ� ���Ϳ� �־��� ����� �븻�� �����̰ų�, ���� ������ ��� ������ false ����.
	PLANE	TestP;
	CalcPlaneEquation( &TestP, pInTriangle);
	
	float	fDot	=	DotProduct(&(TestP.v3Up), &(pInSphere->Velocity));
//	if( fDot >= -0.00001f)
	if( fDot >= 0.0f)
		return	FALSE;
	

	if( pInSphere->Velocity.y != 0.0f)
		_asm nop;

	
	//	100.0f�� �������� ������ �Ǿ��ֳ�.?
	float		fOriginalRadius	=	pInSphere->fRadius;
	float		fUnitFactor		=	100.0f;
	float		fScaleFactor	=	fUnitFactor	/	fOriginalRadius;

	MOVING_SPHERE	Sphere	=	*pInSphere;
	VECTOR3		pTri[3];
	pTri[0]		=	pInTriangle[0];
	pTri[1]		=	pInTriangle[1];
	pTri[2]		=	pInTriangle[2];

	BOOL	bScaled	=	FALSE;

	if( fOriginalRadius != 100.0f)
	{
		// �������� ��Ų��.
		bScaled		=	TRUE;
		
		Sphere.fRadius		*=	fScaleFactor;
		VECTOR3_MULEQU_FLOAT( &Sphere.From, fScaleFactor);
		VECTOR3_MULEQU_FLOAT( &Sphere.Velocity, fScaleFactor);

		VECTOR3_MULEQU_FLOAT( &(pTri[0]), fScaleFactor);
		VECTOR3_MULEQU_FLOAT( &(pTri[1]), fScaleFactor);
		VECTOR3_MULEQU_FLOAT( &(pTri[2]), fScaleFactor);
	}


	BOOL	bFound	=	FALSE;
	*pOutT	=	1.1f;

	DWORD	dwLastMeetComponent	=	0xffffffff;
	// 0 ���, 1, ����, 2, ��.

	float		fResultTEdge;
	float		fResultTPlane;
	float		fResultTVertex;
	VECTOR3		WhereMeet;


	//	�ﰢ���� ������ �׽�Ʈ.
	pInSphere->fRadius -= -10.0f;
	BOOL	bEdgeMeet	=	TestMovingSphereMeetTriEdge2( &WhereMeet, &fResultTEdge, &Sphere, pTri);
	if( bEdgeMeet == TRUE)
	{
		if( *pOutT > fResultTEdge)
		{
			CheckVectorArray( &WhereMeet, 1);
			bFound	=	TRUE;
			*pOutT	=	fResultTEdge;
			*pOutWhereMeet	=	WhereMeet;

			dwLastMeetComponent	=	10;
		}
	}
	pInSphere->fRadius += 10.0f;

	//	�ﰢ���� ���� �׽�Ʈ.
	BOOL	bPlaneMeet	=	TestMovingSphereMeetPLANE2( &WhereMeet, &fResultTPlane, &Sphere, pTri);
	if( bPlaneMeet == TRUE)
	{
		if( *pOutT > fResultTPlane)
		{
			CheckVectorArray( &WhereMeet, 1);
			bFound	=	TRUE;
			*pOutT	=	fResultTPlane;
			*pOutWhereMeet	=	WhereMeet;

			dwLastMeetComponent		=	0;
		}
	}

	if( dwLastMeetComponent == 10)
	{
		_asm nop;
//		return	FALSE;
	}

	//	�ﰢ���� �𼭸��� �׽�Ʈ.
	int i;
	BOOL		bMeet;
	for( i = 0; i < 3; i++)
	{
		bMeet	=	TestMovingSphereMeetVertex2( &WhereMeet, &fResultTVertex, &Sphere, &(pTri[i]));
		if( bMeet == TRUE)
		{
//			BreakIfTrue( fResultT < -0.01f || fResultT > 1.01f);
			if( *pOutT > fResultTVertex)
			{
				CheckVectorArray( &WhereMeet, 1);
				bFound	=	TRUE;
				*pOutT	=	fResultTVertex;
				*pOutWhereMeet	=	WhereMeet;

				dwLastMeetComponent	=	20+i;
			}
		}
	}



	
	if( bFound == FALSE)
	{
		return	FALSE;
	}

	if( dwLastMeetComponent == 10)
	{
		_asm nop;
	}


	// �ӵ� ���Ϳ�, ������ ����� ������ ���.? ��¼���.???
	VECTOR3			MeetPivot;
	MeetPivot		=	Sphere.From		+	Sphere.Velocity * (*pOutT);

	VECTOR3			PlaneNormal;
	PlaneNormal		=	MeetPivot	-	*pOutWhereMeet;
	float			fNormal	=	VECTOR3Length( &PlaneNormal);

	Normalize( &PlaneNormal, &PlaneNormal);

	float		fTest;
	fTest		=	DotProduct( &PlaneNormal, &Sphere.Velocity);
	if( fTest > fTestValue)
		return	FALSE;



	// ��������
	float	fRescale;
	if( bScaled)
	{
		fRescale	=		fOriginalRadius / fUnitFactor;
		VECTOR3_MULEQU_FLOAT( pOutWhereMeet, fRescale);
	}

	// �ٽ� üũ.
	VECTOR3		MeetPivot2;
	MeetPivot2		=	pInSphere->From		+	pInSphere->Velocity * (*pOutT);

	VECTOR3		PlaneNormal2;
	PlaneNormal2	=	MeetPivot2	-	*pOutWhereMeet;
	float			fNormal2	=	VECTOR3Length( &PlaneNormal2);

	Normalize( &PlaneNormal2, &PlaneNormal2);

	float	fTest2;
	fTest2		=	DotProduct( &PlaneNormal2, &(pInSphere->Velocity));
//	if( fTest2 > fTestValue)
//		return	FALSE;
//	if( fTest2 >= 0.0f)
//		_asm int 3;



	CheckVectorArray( pOutWhereMeet, 1);
	*pdwOutComponentType	=	dwLastMeetComponent;
	return	TRUE;
}

BOOL	TestMovingSphereMeetPLANE2( VECTOR3* pOutWhereMeet, float* pOutT, MOVING_SPHERE* pSphere, VECTOR3* pTri)
{
	BOOL		bResult;
	float		fTemp;

	float		fPlaneDotVelocity;

	float		t;			// ���Ǿ �鿡 �������� ����.
	VECTOR3		X;			// ���Ǿ �鿡 ��� ��ġ.
	VECTOR3		MeetPos;	// ���Ǿ�� ���� �´�� ��.

	VECTOR3		Velocity	=	pSphere->Velocity;

	PLANE	p;
	CalcPlaneEquation( &p, pTri);

//	MakePLANE( &p, &(pTri[0]) , &(pTri[1]), &(pTri[2]));

	// �����ϰų�, ���Ǿ��� ���� ����� ���� �븻�� ���� �����̸� ������ �� ��ģ��.
	fPlaneDotVelocity	=	DotProduct( &(p.v3Up), &Velocity);
	if( fPlaneDotVelocity >= -0.0001f)
	{
		// �ȸ����� ����.
		return	FALSE;
	}

	// ���Ǿ ��� ���ʿ� ������ ��� �н�.
	float	fDet	=	DotProduct( &(p.v3Up), &(pSphere->From))	+	p.D;
	if( fDet < 0.0f)
	{
		return	FALSE;
	}

	// ���Ǿ��� ����	: X = F + (T-F)*t
	// ��鿡�� ���Ǿ� ���������� �Ÿ�.		: X*N + d = r  (r�� ���Ǿ� ������.)
	// t = -1*(N*F+d-r)/(N*(T-F))
	t	=	DotProduct( &(p.v3Up), &(pSphere->From));
	t	=	pSphere->fRadius - (t + p.D);
	t	=	t / fPlaneDotVelocity;
	// X = F + (T-F)*t
	VECTOR3_MUL_FLOAT( &X, &Velocity, t);
	VECTOR3_ADDEQU_VECTOR3( &X, &(pSphere->From));

	if( t > 1.0f)
	{
		return	FALSE;
	}

	if( t < -1.0f)	//	�̷� ����,
	{
		//	�׳� ���� �ص� �ɱ�.?
		//	����� ��Ȯ�ϴٸ� �翬�ϰ���.?
		return	FALSE;
	}
/*		//	'-1'�̸� �������ǰ�.? �� ����.?

		// ���࿡ ���Ǿ��� ������ ���� �Ѿ���ȴٸ� ���̻� ����� ��ġ�� ����.
		// ������ �׿��� �̷� ��Ȳ�� �� ���� �ִ�.
		float	det	=	DotProduct( &(pSphere->From), &(p.v3Up))	+	p.D;
		if( det < 0.0f)
			return	FALSE;
*/
	// ���⼭ 0�� ����� t���� �׳� 0���� ó�� �ϴ°� ���� ������.?

	// ���Ǿ�� ����� ������ ���� �´��� ����.?
	// X���� ���븻�� �ݴ�������� ��������ŭ �� ��.
	// MeetPos	=	X + N*(-1*fRadius)
	MeetPos	=	p.v3Up;
	VECTOR3_MULEQU_FLOAT( &MeetPos, pSphere->fRadius * -1.0f);
	VECTOR3_ADDEQU_VECTOR3( &MeetPos, &X);

	// �� ������ ���� �ﰢ�� �ȿ� �ִ°�.?
//	VECTOR2		v2Meet;
//	bResult	=	IsTriMeetVertex( &v2Meet, pTri, &MeetPos);
	bResult	=	IsTriMeetVertex( pTri, &MeetPos);

	if( bResult	== FALSE)
		return	FALSE;

	// ���� float�� ������ ���� �̹� ������ �ִ� ������ �Ǻ��� ��츦 ������ ó���Ѵ�.
	if( t < 0.0f)
	{
		// ���� �̹� ������ �ִ� ����� ���Ǿ �����̱� ���� ��ġ�� �����Ѵ�.
		fTemp	=	DotProduct( &(pSphere->From), &(p.v3Up)) + p.D;
		// ���Ǿ��� �������� ����ȭ�� ���� 100.0f�̹Ƿ�, ������ 0.0f���� �� ���� ���ٰ� ���� �ȴ�.
		if( fTemp <= 0.0f )	// ������ ����ٸ�,  �� ��� ������ 1¥���� �Դٰ��� �ϸ� ������ �ɷ���?
			return	FALSE;
		else
			_asm nop;
	}


	*pOutT			=	t;

	*pOutWhereMeet	=	MeetPos;

#ifdef	_DEBUG
	CheckVectorArray( pOutWhereMeet, 1);


	// ���� �ӵ����Ͱ� �����ϸ� ���ʺ��� ���� �ȵȴ�. �װ� ��Ƴ���.
	VECTOR3		MeetPivot;
	MeetPivot		=	pSphere->From		+	pSphere->Velocity * (*pOutT);

	VECTOR3		PlaneNormal;
	PlaneNormal		=	MeetPivot	-	*pOutWhereMeet;
	float			fNormal	=	VECTOR3Length( &PlaneNormal);

	Normalize( &PlaneNormal, &PlaneNormal);

	float	fTest;
	fTest		=	DotProduct( &PlaneNormal, &(pSphere->Velocity));
	if( fTest > fTestValue)
		return	FALSE;
//		_asm int 3;


#endif	


	// ����.
	return	TRUE;

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL	TestMovingSphereMeetTriEdge2( VECTOR3* pOutWhereMeet, float* pOutT, MOVING_SPHERE* pSphere, VECTOR3* pTriangle)
{
	// ������ ������ ���ƺ���.
	BOOL	bResult[3];
	BOOL	bFound	=	FALSE;
	VECTOR3		WhereMeet[3];
	float		fMeetT[3];
	float		fLastMeetT	=	1.0f;
	VECTOR3		LastWhereMeet;

	DWORD		dwMeetTime	=	0;
	
	DWORD	i;
	for( i = 0; i < 3; i++)
	{
//		bResult[i]	=	TestMovingSphereMeetLine2( &WhereMeet[i], &fMeetT[i], pSphere, &pTriangle[i], &pTriangle[(i+1)%3]);
		bResult[i]	=	TestMovingSphereMeetLine3( &WhereMeet[i], &fMeetT[i], pSphere, &pTriangle[i], &pTriangle[(i+1)%3]);
		if( bResult[i] == TRUE)
		{
			CheckVectorArray( &WhereMeet[i], 0);
			if( fMeetT[i] > 1.0f)
				_asm int 3;

			if( fMeetT[i] < fLastMeetT)
			{
				bFound	=	TRUE;
				dwMeetTime++;
				fLastMeetT		=	fMeetT[i];
				LastWhereMeet		=	WhereMeet[i];
			}
		}
	}


	if( dwMeetTime == 2)
		_asm nop;

	if( bFound == FALSE)
	{
		return	FALSE;
	}

	CheckVectorArray( &LastWhereMeet, 0);

	*pOutT				=	fLastMeetT;
	*pOutWhereMeet		=	LastWhereMeet;

	return	TRUE;


}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	2003.03.19	t�� '-'�� ��� �ٽ� ������ �ִ��� üũ�ϰ� ��ġ��. �� ����ۿ� ����. -_-;
	2003.03.18	t�� '-'�� ��� �������� ��ġ��.
	�����̴� ������ ���ΰ� �����°�.?

	������		M = A + B*s;
	���� ������ P = C + D*t;

	�̶�
	1. B*(M-P) = 0;
	2. (P-M)*(P-M) = r^2
	�ΰ��� ���´�.

	��1�� M, P�� �����Ͽ� s = ~~  ������ ������ ��,
	���� M = A + B*s�� �����Ͽ� M�� t�� ���� ������ ����.

	M�� P�� ��2�� �����ؼ�, (E+F*t)*(E+F*t) = r^2 ����,

	F*F*t^2 + 2*E*F*t + E*E-r^2 = 0
	 = a        = b       = c

	�Ǻ��� det = b^2 - 4*a*c;

	���� �ΰ��϶� t1 = (-b - root(det)) / 2a �ϳ��� ��� (t2�� �ǹ̰� ����.)

    �̶� 0 < s < 1 �� �ƴϸ� ���� �ǹ̰� ����.
*/

BOOL	TestMovingSphereMeetLine2( VECTOR3* pOutWhereMeet, float* pOutT, MOVING_SPHERE* pSphere, VECTOR3* pLFrom, VECTOR3* pLTo)
{
	VECTOR3		A	=	*pLFrom;		// LineFrom
	VECTOR3		B;									// LineVelocity
	VECTOR3_SUB_VECTOR3( &B, pLTo, pLFrom);
	if( VECTOR3Length( &B) == 0.0f)
		return	FALSE;					// 

	VECTOR3		M;									// Line���� ����.
	float		s;									// �׶��� ����.

	VECTOR3		C	=	pSphere->From;					// Sphere From.
	VECTOR3		D	=	pSphere->Velocity;					// Sphere Velocity.

	// ���� ������ ����� ���Ǿ��� ���� ������ ���ٸ� ����� �ǹ̰� �����Ƿ� �ٷ� ����������.
	VECTOR3		NormalizedB;
	Normalize( &NormalizedB, &B);
	VECTOR3		NormalizedD;
	Normalize( &NormalizedD, &D);
	VECTOR3_SUBEQU_VECTOR3( &NormalizedB, &NormalizedD);
	float	fV	=	VECTOR3Length( &NormalizedB);
	if( fV <= 0.0001f || fV >= 1.9999f)
		return	FALSE;


	
	//	B�� D�� ������ ���.
//	������ ����� ���Ǿ �����̴� ������ 

	VECTOR3		P;									// Sphere���� ����.
	float		t;									// �׶��� ����.

	float		dotBB	=	DotProduct( &B, &B);
	float		fTemp	=	(DotProduct( &B, &C) - DotProduct( &A, &B)) / dotBB;
	VECTOR3		E	=	B;
	VECTOR3_MULEQU_FLOAT( &E, fTemp);
	VECTOR3_SUBEQU_VECTOR3( &E, &C);
	VECTOR3_ADDEQU_VECTOR3( &E, &A);

	VECTOR3		F	=	B;
	VECTOR3_MULEQU_FLOAT( &F, DotProduct(&B, &D)/dotBB);
	VECTOR3_SUBEQU_VECTOR3( &F, &D);


	// a*t^2 + b*t + c = 0
	float	a	=	DotProduct( &F, &F);
	float	b	=	2.0f * DotProduct( &E, &F);
	float	c	=	DotProduct( &E, &E)	-	pSphere->fRadius * pSphere->fRadius;

	float	det	=	b*b - 4.0f*a*c;					// �Ǻ���.

	// �Ǻ��� < 0
	if( det < -0.001f)
	{
		// ������ ����.? �����.
		return	FALSE;
	}

	// �Ǻ��� <= 0		// �ǹ� ���ٰ� ������.?
	if( det <= 0.0f)
	{
		return	FALSE;
	}

	// �Ǻ��� > 0
	t	=	(-b - (float)sqrt(det)) / (2.0f*a);

	// t�� �ʹ� �� ���. 
	if( t > 1.0f)
	{
		return	FALSE;
	}

	// ������ Sphere�� ������ ���.?
	P	=	C + D*t;

	// ������ Line�� ��ġ�� ���.?
	s	=	(B*C - A*B + B*D*t) /dotBB;
	if( s < 0.0f || s > 1.0f)		// line�� ��ġ�� �����,
	{
		return	FALSE;
	}
	M	=	A + B*s;		// ������ ������ �κ�.

	// t�� '-'�� ��� ���Ǿ��� ���� ��ġ���� �̹� ������ �ִ°��� üũ�Ѵ�.
	if( t < 0.0f)
	{
//		return	FALSE;
		// ���Ǿ��� �������� �� �ڿ� �ִ� ���.
		VECTOR3			Temp1	=	M	-	pSphere->From;
		VECTOR3			Temp2	=	pSphere->Velocity;
		float			Temp3	=	DotProduct(&Temp1, &Temp2);
		if( Temp3 < 0.00001f)
			return	FALSE;

		// ����� �н��ϸ� �̹� ������ �ְų� ������ ��¦ �İ� ���� ���.
	}

	// ������ ���� �� ���� ���� ����� �����ΰ�? �׷��ٸ� ����.
	VECTOR3		WheneMeet	=	M	-	P;
	float		fDotWheneMeet	=	DotProduct( &WheneMeet, &pSphere->Velocity);
//	if( fDotWheneMeet <= 0.00001f)
	if( fDotWheneMeet <= 0.0f)
	{
		return	FALSE;
	}


	*pOutWhereMeet	=	M;
	*pOutT			=	t;

	return	TRUE;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	�����̴� ���Ǿ ���� �����°�.?
	
	���� P, 
	������ ������ X = A + B*t;	�� ����.
	PX�� ���̴� r.

*/
BOOL	TestMovingSphereMeetVertex2( VECTOR3* pOutWhereMeet, float* pOutT, MOVING_SPHERE* pSphere, VECTOR3* pVertex)
{
	VECTOR3		A	=	pSphere->From;								// ���� ����.
	VECTOR3		B	=	pSphere->Velocity;							// ���� �ӵ�����.
//	VECTOR3_SUB_VECTOR3( &B, &(pSphere->->To), &(pSphere->From));			
	VECTOR3		X;												// ���찡 ���� �������� ��ġ.
	float		t;												// ������ ����.
	// X = A + B*t

	VECTOR3		P	=	*pVertex;		// Ÿ�� ��.

	VECTOR3		PA;
	VECTOR3_SUB_VECTOR3( &PA, &A, &P);

	float		a	=	DotProduct( &B, &B);
	float		b	=	2.0f * DotProduct( &B, &PA);
	float		c	=	DotProduct( &PA, &PA)	-	pSphere->fRadius * pSphere->fRadius;

	float		det	=	b*b		-	4*a*c;

	if( det <= 0.00000f)
	{
		return	FALSE;
	}
	
	t	=	(-b - (float)sqrt(det)) / (2*a);
	if( t > 1.0f)
	{
		return	FALSE;
	}

	//	X	=	A	+	B*t;
	VECTOR3_MUL_FLOAT( &X, &B, t);
	VECTOR3_ADDEQU_VECTOR3( &X, &A);

	// t�� '-'�̴��� �̹� ������ �ְ�, ���Ǿ��� ������ ���� �� ���� ���������� TRUE ����.
	float	fTemp;
	if( t < 0.0f)
	{
		_asm nop;
		// �ϴ� ���Ǿ� ���� ���ԵǴ°�?
		VECTOR3		vTemp;
		VECTOR3_SUB_VECTOR3( &vTemp, &(pSphere->From), &P);
		fTemp	=	VECTOR3Length( &vTemp);
		if( fTemp >= pSphere->fRadius)
			return	FALSE;

		// ���Ǿ� From ���� �ְ�, ���Ǿ��� �ӵ����͸� �븻�� ������ ��鿡 ����, '��'�� �տ� �ֳ� �ڿ� �ֳ�.?
		// �տ� �ִٸ� ��ȿ�� ���̶� ���̰�, �ڿ� �ִٸ� ��ȿ�� ���̶� ���̴�.
		// �ϴ� ���Ǿ��� ũ��� 100.0f�� ������ �� �����̴� �Ͼ ������.
		PLANE	p;
		p.v3Up		=	pSphere->Velocity;
		Normalize(&p.v3Up, &p.v3Up);
		p.D		=	-1.0f * DotProduct( &p.v3Up, &(pSphere->From));

		fTemp	=	DotProduct( &p.v3Up, pVertex)	+	p.D;
		if( fTemp <= 0.0f)
			return	FALSE;
		else
			_asm nop
/*
		VECTOR3		vTemp;
		VECTOR3_SUB_VECTOR3( &vTemp, &(pSphere->From), &P);
		float	fDist	=	VECTOR3Length( &vTemp);
		if( fDist >= pSphere->fRadius)
		{
			return	FALSE;
		}
*/
	}

	// pOut ����.
	*pOutT			=	t;			// �������� ����.
//	pOut->MeetPivot	=	X;			// �������� ���� �Ǻ�.
	*pOutWhereMeet	=	P;			// ��� ������.?

#ifdef	_DEBUG
	CheckVectorArray( pOutWhereMeet, 1);


	// ���� �ӵ����Ͱ� �����ϸ� ���ʺ��� ���� �ȵȴ�. �װ� ��Ƴ���.
	VECTOR3		MeetPivot;
	MeetPivot		=	pSphere->From		+	pSphere->Velocity * (*pOutT);

	VECTOR3		PlaneNormal;
	PlaneNormal		=	MeetPivot	-	*pOutWhereMeet;
	float			fNormal	=	VECTOR3Length( &PlaneNormal);

	Normalize( &PlaneNormal, &PlaneNormal);

	float	fTest;
	fTest		=	DotProduct( &PlaneNormal, &(pSphere->Velocity));
	if( fTest > fTestValue)
		return	FALSE;
//		_asm int 3;


#endif	

	return	TRUE;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	2003.03.19	t�� '-'�� ��� �ٽ� ������ �ִ��� üũ�ϰ� ��ġ��. �� ����ۿ� ����. -_-;
	2003.03.18	t�� '-'�� ��� �������� ��ġ��.
	�����̴� ������ ���ΰ� �����°�.?

	������		M = A + B*s;
	���� ������ P = C + D*t;

	�̶�
	1. B*(M-P) = 0;
	2. (P-M)*(P-M) = r^2
	�ΰ��� ���´�.

	��1�� M, P�� �����Ͽ� s = ~~  ������ ������ ��,
	���� M = A + B*s�� �����Ͽ� M�� t�� ���� ������ ����.

	M�� P�� ��2�� �����ؼ�, (E+F*t)*(E+F*t) = r^2 ����,

	F*F*t^2 + 2*E*F*t + E*E-r^2 = 0
	 = a        = b       = c

	�Ǻ��� det = b^2 - 4*a*c;

	���� �ΰ��϶� t1 = (-b - root(det)) / 2a �ϳ��� ��� (t2�� �ǹ̰� ����.)

    �̶� 0 < s < 1 �� �ƴϸ� ���� �ǹ̰� ����.
*/

BOOL	TestMovingSphereMeetLine3( VECTOR3* pOutWhereMeet, float* pOutT, MOVING_SPHERE* pSphere, VECTOR3* pLFrom, VECTOR3* pLTo)
{
	VECTOR3		A	=	*pLFrom;		// LineFrom
	VECTOR3		B;									// LineVelocity
	VECTOR3_SUB_VECTOR3( &B, pLTo, pLFrom);
	if( VECTOR3Length( &B) == 0.0f)
		return	FALSE;					// 

	VECTOR3		M;									// Line���� ����.
	float		s;									// �׶��� ����.

	VECTOR3		C	=	pSphere->From;					// Sphere From.
	VECTOR3		D	=	pSphere->Velocity;					// Sphere Velocity.

	// ���� ������ ����� ���Ǿ��� ���� ������ ���ٸ� ����� �ǹ̰� �����Ƿ� �ٷ� ����������.
	VECTOR3		NormalizedB;
	Normalize( &NormalizedB, &B);
	VECTOR3		NormalizedD;
	Normalize( &NormalizedD, &D);
	VECTOR3_SUBEQU_VECTOR3( &NormalizedB, &NormalizedD);
	float	fV	=	VECTOR3Length( &NormalizedB);
	if( fV <= 0.0001f || fV >= 1.9999f)
		return	FALSE;


	
	//	B�� D�� ������ ���.
//	������ ����� ���Ǿ �����̴� ������ 

	VECTOR3		P;									// Sphere���� ����.
	float		t;									// �׶��� ����.

	float		dotBB	=	DotProduct( &B, &B);
	float		fTemp	=	(DotProduct( &B, &C) - DotProduct( &A, &B)) / dotBB;
	VECTOR3		E	=	B;
	VECTOR3_MULEQU_FLOAT( &E, fTemp);
	VECTOR3_SUBEQU_VECTOR3( &E, &C);
	VECTOR3_ADDEQU_VECTOR3( &E, &A);

	VECTOR3		F	=	B;
	VECTOR3_MULEQU_FLOAT( &F, DotProduct(&B, &D)/dotBB);
	VECTOR3_SUBEQU_VECTOR3( &F, &D);


	// a*t^2 + b*t + c = 0
	float	a	=	DotProduct( &F, &F);
	float	b	=	2.0f * DotProduct( &E, &F);
	float	c	=	DotProduct( &E, &E)	-	pSphere->fRadius * pSphere->fRadius;

	float	det	=	b*b - 4.0f*a*c;					// �Ǻ���.

	// �Ǻ��� < 0
	if( det < -0.001f)
	{
		// ������ ����.? �����.
		return	FALSE;
	}

	// �Ǻ��� <= 0		// �ǹ� ���ٰ� ������.?
	if( det <= 0.0f)
	{
		return	FALSE;
	}

	// �Ǻ��� > 0
	t	=	(-b - (float)sqrt(det)) / (2.0f*a);

	// t�� �ʹ� �� ���. 
	if( t > 1.0f)
	{
		return	FALSE;
	}

	// ������ Sphere�� ������ ���.?
	P	=	C + D*t;

	// ������ Line�� ��ġ�� ���.?
	s	=	(B*C - A*B + B*D*t) /dotBB;
	if( s < 0.0f || s > 1.0f)		// line�� ��ġ�� �����,
	{
		return	FALSE;
	}
	M	=	A + B*s;		// ������ ������ �κ�.

	// t�� '-'�� ��� ���Ǿ��� ���� ��ġ���� �̹� ������ �ִ°��� üũ�Ѵ�.
	if( t < 0.0f)
	{
//		return	FALSE;
		// ���Ǿ��� �������� �� �ڿ� �ִ� ���.
		VECTOR3			Temp1	=	M	-	pSphere->From;
		VECTOR3			Temp2	=	pSphere->Velocity;
		float			Temp3	=	DotProduct(&Temp1, &Temp2);
		if( Temp3 < 0.00001f)
			return	FALSE;

		// ����� �н��ϸ� �̹� ������ �ְų� ������ ��¦ �İ� ���� ���.
	}

	// ������ ���� �� ���� ���� ����� �����ΰ�? �׷��ٸ� ����.
	VECTOR3		WheneMeet	=	M	-	P;
	float		fDotWheneMeet	=	DotProduct( &WheneMeet, &pSphere->Velocity);
//	if( fDotWheneMeet <= 0.00001f)
	if( fDotWheneMeet <= 0.0f)
	{
		return	FALSE;
	}


	*pOutWhereMeet	=	M;
	*pOutT			=	t;

	return	TRUE;
}

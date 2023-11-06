#include "SphereCollisionDetect.h"
#include "../4DyuchiGRX_myself97/myself97common.h"
#include "../4DyuchiGXGFunc/global.h"

// ��� ������ ��������,t ��� ������ ��������,t, ���Ǿ��� ����, to, ������, ���� ���.
// ���ϰ��� �ȸ����� 0, ������ 1.
BOOL	MovingSphereMeetTriPlane( VECTOR3* OUT pCandidate, float* OUT pT, VECTOR3* pWhereMeet, VECTOR3* IN pSPivot, VECTOR3* IN pSTo, float fSRadius, VECTOR3* pTri);
BOOL	MovingSphereMeetTriEdge	( VECTOR3* OUT pCandidate, float* OUT pT, VECTOR3* pWhereMeet, VECTOR3* IN pSPivot, VECTOR3* IN pSTo, float fSRadius, VECTOR3* IN pTri);
BOOL	MovingSphereMeetLine( VECTOR3* pCandidate, float* pT1, VECTOR3* pWhereMeet, float* pT2, VECTOR3* pSPivot, VECTOR3* pSTo, float fSRadius, VECTOR3* pLFrom, VECTOR3* pLTo);
BOOL	MovingSphereMeetVertex( VECTOR3* pCandidate, float* pT, VECTOR3* pSPivot, VECTOR3* pSTo, float fSRadius, VECTOR3* pVertex);


// �����̴� ���Ǿ �ﰢ���� ���� ������.?
//	���, ����, �𼭸�, ������ ���� ������ �Ÿ��� üũ, ������ �ʴ°��� ���� �����ٰ� ������ ���� ���� �������� �������� ����.
BOOL	WhereDidMovingSphereMeetTri( VECTOR3* OUT pCandidate, float* pT, VECTOR3* pWhereMeet, VECTOR3* pSFrom, VECTOR3* pSTo, float fSRadius, VECTOR3* pTri)
// ������ ��ġ, �������� t, �������� �����̵� �÷���, ���� ����, ���� to, �� ������, �ﰢ��.
{
	VECTOR3		vTemp;
	VECTOR3		vTemp2;
	float		fTemp;
	BOOL		bResult, bFound = FALSE;
	VECTOR3		Dir;
	VECTOR3_SUB_VECTOR3( &Dir, pSTo, pSFrom);

	VECTOR3	C1, W;
	float	t1;		// t�� ������ ��쿡 ���ؼ� '-'���� ���� �� �ִ�. �����°� ȣ��Ǵ� �Լ����� �̸� �׽�Ʈ�ǰ�, t�� -�� ������ ���� �����ٴ� �ǹ̴�.
	//	�ﰢ���� �̷�� ���� ������.?	����� '��'�� �˻�.

	{
		// �ﰢ���� ���� ������ ���� ã�´�. 
		bResult		=	MovingSphereMeetTriPlane( &C1, &t1, &W, pSFrom, pSTo, fSRadius, pTri );
		
		if( bResult	== TRUE)
		{
			if( t1 < 1)
			{
				// �������� ����.
				*pCandidate	=	C1;
				*pT			=	t1;
				*pWhereMeet	=	W;
				bFound		=	TRUE;
			}
		}

	}


	//	������ ���� �ﰢ�� �ȿ� ������, ���� ������ ������.?
	{
		bResult	=	MovingSphereMeetTriEdge( &C1, &t1, &W, pSFrom, pSTo, fSRadius, pTri);
		if( bResult	== TRUE)
		{
			if( (bFound == TRUE && *pT > t1) || bFound == FALSE)
			{
				*pCandidate	=	C1;
				*pT			=	t1;
				*pWhereMeet	=	W;
				bFound		=	TRUE;
			}
		}

	}

	//	������ ���� �ﰢ���� �𼭸��ΰ�.?
	{
		DWORD	i;
		for( i = 0; i < 3; i++)

		{
			bResult	=	MovingSphereMeetVertex( &C1, &t1, pSFrom, pSTo, fSRadius, &(pTri[i]));
			if( bResult == TRUE)
			{
				if( (bFound == TRUE && *pT > t1) || bFound == FALSE)
				{
					*pCandidate	=	C1;
					*pT			=	t1;
					*pWhereMeet	=	pTri[i];
					bFound		=	TRUE;
				}
			}
		}
	}


	if( bFound == FALSE)		// �ݴ°� �ϳ��� ����.
	{
		return	FALSE;
	}
	// �����̵� ����� ���� �޴� ���忡�� ����� ����.

	// ������ ���� �ݴ���, ������� ���Ϳ� ����� ���ٸ� ������ �ʴ´�.
	// ����(��������-����)�� ������� ������ ������ 0���� �۰ų� ������ ��ȿ.

	VECTOR3_SUB_VECTOR3( &vTemp, pSTo, pSFrom);
	VECTOR3_SUB_VECTOR3( &vTemp2, pWhereMeet, pSFrom);
	fTemp	=	DotProduct( &vTemp, &vTemp2);

//	if( (*pWhereMeet-*pSFrom)*(*pSTo-*pSFrom) <= 0)
	if( fTemp <= 0)
		return FALSE;

//	*pCandidate	=	*pSFrom + (*pSTo - *pSFrom) * *pT;
	VECTOR3_SUB_VECTOR3( &vTemp, pSTo, pSFrom);
	VECTOR3_MUL_FLOAT( &vTemp, &vTemp, (*pT));
	VECTOR3_ADD_VECTOR3( pCandidate, pSFrom, &vTemp);

	return TRUE;
}


// �ﰢ���� ���� �ǵ鸮��.? ������ ���� �׷��� �������, �̵����⿡ ���� �ﰢ���� �ݴ�� ���� �ִ°��� �浹ó�� �ȵȴ�..
BOOL	MovingSphereMeetTriPlane
	( VECTOR3* OUT pCandidate1, float* OUT pT1, VECTOR3* pWhereMeet, VECTOR3* IN pSPivot, VECTOR3* IN pSTo, float fSRadius, VECTOR3* pTri)

{
	VECTOR3	vTemp;
	float	fTemp;
	float	t;

	VECTOR3	LDir;
	VECTOR3_SUB_VECTOR3( &LDir, pSTo, pSPivot);
	
	PLANE	Plane;
	MakePLANE(  &Plane, &(pTri[0]), &(pTri[1]), &(pTri[2]));
	
	float	dot	=	DotProduct( &LDir, &(Plane.v3Up));
	if( dot == 0)	return FALSE;		// �����ϴ�. -_-;

	// ���� ������ ���Ǿ��� ��ġ.
//	float	fUnder	=	Plane.v3Up.x*(LDir.x) + Plane.v3Up.y*LDir.y + Plane.v3Up.z*LDir.z;
	float	fUnder	=	DotProduct(&(Plane.v3Up), &LDir);

//	float	fOver	=	-1*( Plane.v3Up.x*pSPivot->x + Plane.v3Up.y*pSPivot->y + Plane.v3Up.z*pSPivot->z + Plane.D  );
	fTemp	=	DotProduct( &(Plane.v3Up), pSPivot);
	float	fOver	=	-1*( fTemp + Plane.D  );
	*pT1	=	(fOver + fSRadius) / fUnder;
//	*pCandidate1	=	*pSPivot + (LDir*(*pT1));
	VECTOR3_MUL_FLOAT( &vTemp, &LDir, *pT1);
	VECTOR3_ADD_VECTOR3( pCandidate1, pSPivot, &vTemp);

	if( *pT1 > 1 )
		return	FALSE;
	if( *pT1 < 0)	// ����� �հ� ���ִ� ���, ������ �ȸ����ٸ� �����ϴ°� �ƴϰ�, ���̻� �İ� ���� ���ϰ� �ؾ��Ѵ�.
	// Ư���� ����� �ִ°� �ƴϰ�, ������ �����°� detect �ؼ� �׷��ٰ� ���ָ� �ǰ���.
	{
		t	=	Plane.v3Up * (*pCandidate1) + Plane.D;
		if( -1 * t > fSRadius)	// ������ ��� ����.
			return FALSE;
	}

	// �̶� ���� ������ ����.?
	fUnder		=	(Plane.v3Up.x)*(Plane.v3Up.x) + (Plane.v3Up.y)*(Plane.v3Up.y) + (Plane.v3Up.z)*(Plane.v3Up.z);
	fOver		=	(Plane.v3Up.x*(pCandidate1->x)) + (Plane.v3Up.y*(pCandidate1->y)) + (Plane.v3Up.z*(pCandidate1->z)) + Plane.D;
	t	=	fOver / fUnder;
	*pWhereMeet		=	*pCandidate1 - (Plane.v3Up*t);		// ������� ��.

	// ���� �ﰢ�� ���� �����°�.?	�� ������ ������ ���� ����� ����� ���ο� ���°� �׽�Ʈ.
	VECTOR3	Mid;//		=	GetTriMiddle( &pTri[0], &pTri[1], &pTri[2]);
	GetTriMiddle( &Mid, pTri);
	VECTOR3	OnLine;
	VECTOR3	ToMid;
	PLANE	p;

	// ù��° ����.
//	OnLine	=	FindNearestVertexOnLine( pTri[0], pTri[1], Mid);
	FindNearestVertexOnLine( &OnLine, &fTemp, pTri[0], pTri[1], Mid);
	VECTOR3_SUB_VECTOR3( &ToMid, &Mid, &OnLine);
	Normalize( &(p.v3Up), &ToMid);
	p.D			=	-1 * DotProduct( &(p.v3Up), &OnLine);		// A dot B = -d, ��������.
	// ���� ������ ���� ù��° ���� �ȿ� �ִ°�.?
	t	=	(p.v3Up) * (*pWhereMeet);
	if( t <= -1*p.D )
	{
		return FALSE;
	}

	// �ι�° ����.
//	OnLine	=	FindNearestVertexOnLine( pTri[1], pTri[2], Mid);
	FindNearestVertexOnLine( &OnLine, &fTemp, pTri[1], pTri[2], Mid);
	VECTOR3_SUB_VECTOR3( &ToMid, &Mid, &OnLine);
	Normalize( &(p.v3Up), &ToMid);
	p.D			=	-1 * DotProduct( &(p.v3Up), &OnLine);
	t	=	(p.v3Up) * (*pWhereMeet);
	if( t <= -1*p.D )
	{
		return FALSE;
	}

	// ����° ����.
//	OnLine	=	FindNearestVertexOnLine( pTri[2], pTri[0], Mid);
	FindNearestVertexOnLine( &OnLine, &fTemp, pTri[2], pTri[0], Mid);
	VECTOR3_SUB_VECTOR3( &ToMid, &Mid, &OnLine);
	Normalize( &(p.v3Up), &ToMid);
	p.D			=	-1 * DotProduct( &(p.v3Up), &OnLine);
	t	=	(p.v3Up) * (*pWhereMeet);
	if( t <= -1*p.D )
	{
		return FALSE;
	}

	// �̱��� ���� ������.
	return	TRUE;

}

// �ﰢ���� ������ �ǵ鸮��... ������ ������ ������ �� ���.
BOOL	MovingSphereMeetTriEdge
	( VECTOR3* OUT pCandidate, float* OUT pT1, VECTOR3* pWhereMeet, VECTOR3* IN pSPivot, VECTOR3* IN pSTo, float fSRadius, VECTOR3* IN pTri)
	// ������ ���Ǿ��� ��ġ, ����, ������ ������ ��, ������ �������� ���⺤��(��麤�ͷ� ���� �ȴ�.), ���Ǿ� ����, ���Ǿ�to, ������, �ﰢ�� ��1, 2, 3
{
	BOOL	bMeet	=	FALSE;
	VECTOR3	C, W;
	float	t1, t2, CandidateT	=	1.0f;
	// ù��° ����.

	if( MovingSphereMeetLine( &C, &t1, &W, &t2, pSPivot, pSTo, fSRadius, &(pTri[0]), &(pTri[1])))
	{
		if( t1 <= 1 && t2 >= 0 && t2 <= 1)	
		{
			bMeet	=	TRUE;
			*pCandidate		=	C;
			CandidateT		=	*pT1	=	t1;
			*pWhereMeet		=	W;
		}
	}

	// �ι�° ����.
	if( MovingSphereMeetLine( &C, &t1, &W, &t2, pSPivot, pSTo, fSRadius, &(pTri[1]), &(pTri[2])))
	{
		if( t1 <= 1 && t2 >= 0 && t2 <= 1)
		{
			if( bMeet == FALSE || CandidateT < t1)
			{
				bMeet	=	TRUE;
				*pCandidate		=	C;
				*pT1			=	t1;
				*pWhereMeet		=	W;
			}
		}
	}
	// ����° ����.

	if( MovingSphereMeetLine( &C, &t1, &W, &t2, pSPivot, pSTo, fSRadius, &(pTri[2]), &(pTri[0])))
	{
		if( t1 <= 1 && t2 >= 0 && t2 <= 1)
		{
			if( bMeet == FALSE || CandidateT < t1)
			{
				bMeet	=	TRUE;
				*pCandidate		=	C;
				*pT1			=	t1;
				*pWhereMeet		=	W;
			}
		}
	}

	if( bMeet == TRUE)
	{
		return	TRUE;
	}


	return	FALSE;
}

// ���а� �����̴� ���Ǿ ��� �����°�.? 
BOOL	MovingSphereMeetLine( VECTOR3* pCandidate, float* pT1, VECTOR3* pWhereMeet, float* pT2, VECTOR3* pSPivot, VECTOR3* pSTo, float fSRadius, VECTOR3* pLFrom, VECTOR3* pLTo)
{
	VECTOR3		nDir;
	Normalize( &nDir, &(*pSTo - *pSPivot));
	VECTOR3&	A	=	*pLFrom;					// ���� ����.
	VECTOR3		B	=	*pLTo	-	*pLFrom;		// ���� ����.
	VECTOR3&	C	=	*pSPivot;					// ���Ǿ� ����.
	VECTOR3		D	=	*pSTo	-	*pSPivot;		// ���Ǿ� ���� ����.

	// �����Ѱ�.?
	VECTOR3		tempvec1;
	VECTOR3		tempvec2;
	Normalize(	&tempvec1, &B);
	Normalize(	&tempvec2, &D);
	if( tempvec1 == tempvec2)
	{
//		_asm nop;
		// ��ȿ���ڶ��� ��ġ�°� ���ڱ�.
		return	FALSE;
	}


	float	dotBB	=	B*B;
	VECTOR3	AC	=	C-A;

//	VECTOR3		n	=	A + B*((B*(C-A))/(B*B)) - C;
	VECTOR3		n	=	A + B*((B*AC)/dotBB) - C;
//	VECTOR3		m	=	( B * ((B*D)/(B*B)) ) - D;
	VECTOR3		m	=	( B * ((B*D)/dotBB) ) - D;

	float	a	=	m * m;
	float	b	=	m * n * 2;
//	float	c	=	(n * n) - ((fSRadius+fBorder)*(fSRadius+fBorder));
	float	c	=	(n * n) - ((fSRadius)*(fSRadius));

	float	t	=	b*b - 4*a*c;
	if( t < -0.0001f)			// t�� ��� ��ȿ���� �Ƴ�.?
	{
		_asm nop;
		// �ƹ��͵� �ȸ����ٴ� ���ΰ�.
		return	FALSE;
	}
	else if( t >= -0.0001f && t < 0.0001f)	// ���� �ϳ��� ���. ��ȿ���� ������ �� �ִµ�..
	{
		_asm nop;
		*pT1	=	(-1 * b) / (2 * a);
	}
	else if( t >= 0.0001f)		// ���� �ΰ��� ���. 
	{
		// ������� �ΰ� �������� t1�� �ǹ̰� �ְ�, t2�� �ǹ̰� ����.
		t	=	float(sqrt(double(t)));
		float	t1	=	(-b - t)/(2*a);
//		float	t2	=	(-b + t)/(2*a);		// �ǹ̾��� t2
		*pT1	=	t1;
	}

	if( *pT1 > 1 )
		return	FALSE;

	*pCandidate		=	C + D*(*pT1);		

	*pT2			=	(B * (AC + D*(*pT1))) / dotBB;
	*pWhereMeet		=	A + B*(*pT2);

	float	fCW	=	VECTOR3Length( &(*pCandidate - *pSPivot));

//	if( *pT1 < 0 && fCW > fSRadius)	//  ��·�� �����ٸ� -�̴��� t�� ����.
	if( fCW > fSRadius+0.1f)	//  ��·�� �����ٸ� -�̴��� t�� ����.
	{
		return FALSE;
	}


	return		TRUE;

}

BOOL	MovingSphereMeetVertex( VECTOR3* pCandidate, float* pT, VECTOR3* pSPivot, VECTOR3* pSTo, float fSRadius, VECTOR3* pVertex)
{

//	VECTOR3&	A		=	*pSPivot;
	VECTOR3		B		=	*pSTo		-	*pSPivot;
	VECTOR3		VA		=	*pSPivot	-	*pVertex;
	float		a		=	B*B;
	float		b		=	2 * ( B * VA);
	float		c		=	VA * VA		-	fSRadius * fSRadius;

	float		t	=	b*b - 4*a*c;

	if( t < -0.0001f)
	{
		return	FALSE;
	}
	else if( t >=	-0.0001f && t < 0.0001f)
	{
		*pT		=	(-1*b) / (2*b);
	}
	else if( t >= 0.0001f)
	{
		t		=	float(sqrt(double(t)));
		*pT		=	(-1*b-t) / (2*a);
	}
	
	if( *pT	> 1)
	{
		return	FALSE;
	}
	*pCandidate		=	*pSPivot	+	B*(*pT);

	// �Ÿ� üũ, '-'���� ������ '������' üũ.
	float	fCW	=	VECTOR3Length( &( *pCandidate - *pVertex ));
	if(	*pT < 0 && fCW > fSRadius)
	{
		return	0;
	}
	return	TRUE;

}


// �־��� ��鿡�� from,to�� ��������� �־��� ������ �о��ش�. pMeet�� ������ ����, pMeetPos�� �������� ���� ��ġ, pSlidingPlane�� ���. pCandidate�� ��� ���.
// fMinimumFromPlane�� ��鿡�� �ּ��� �������־�� �ϴ� �Ÿ�. �ٿ�� ���Ǿ �ش�Ǵ°������� �𸣰ڴµ�, �̰� �İ� �������� ������ �ȵǹǷ� �� ������ ������ �Ѵ�.
void	SpherePushDir( VECTOR3* OUT pCandidate, VECTOR3* IN pFrom, VECTOR3* IN pTo, VECTOR3* IN pMeet, VECTOR3* IN pMeetPos, PLANE* IN pSlidingPlane, BOUNDING_VOLUME* pBV)
{
	const	float		fBorder	=	1.0f;
	VECTOR3				vPush	=	*pMeetPos - *pMeet;

	float			fPush	=	VECTOR3Length(&vPush);
	float			fTo		=	pSlidingPlane->v3Up.x*pTo->x + pSlidingPlane->v3Up.y*pTo->y + pSlidingPlane->v3Up.z*pTo->z + pSlidingPlane->D ;

	//	VECTOR3		vElasticity;
	if( pBV->fElasticity < 0.0f)
	{
		_asm int 3;		// �̷����� ������ �ȵ��� �ʴ°�.?
	}

//	*pCandidate	=	*pTo	+	( pSlidingPlane->v3Up * ( fPush - fTo) *	(1.0f + pBV->fElasticity)  );
	*pCandidate	=	*pTo	+	( pSlidingPlane->v3Up * ( pBV->fRadius - fTo + fBorder) *	(1.0f + pBV->fElasticity)  );
}


// �����̴� ���Ǿ ���Ǿ��� ���� ������.?
// ������ ��ġ, ������ t, ������ ����, �����̴� ���Ǿ�from, to, ������, ������ ���Ǿ�pivot, ������.
BOOL	WhereDidMovingSphereMeetSphere( VECTOR3* pCandidate, float* pT, VECTOR3* pWhereMeet, VECTOR3* pSFrom, VECTOR3* pSTo, float fSRadius, VECTOR3* pTSPivot, float fTSRadius)
{
	VECTOR3		vTemp;
	float		fTemp;
	VECTOR3&	A	=	*pSFrom;
	VECTOR3		B	=	*pSTo - A;
	VECTOR3&	C	=	*pCandidate;		// C = A + B*t;
	VECTOR3&	W	=	*pWhereMeet;
	float		r	=	fSRadius;
	float&		t	=	*pT;
	
	VECTOR3&	S	=	*pTSPivot;
	float		s	=	fTSRadius;

	// |CS| = (r+s)^2
	// C = A + B*t �̹Ƿ�,
	// |(A-S) + B*t|^2 = (r+s)^2;
	// B*B*t^2 + 2*t*B*(A-S) + (A-S)*(A-S)-((r+s)^2)
	float	a	=	B*B;
	float	b	=	B*(A-S)*2.0f;
	float	c	=	(A-S)*(A-S)-(r+s)*(r+s);
	// �Ǻ��� b^2-ac = 
	float	det	=	b*b - a*c*4.0f;

	if( det < -0.00001f)		// ������ �ʴ´�.
	{
		return	FALSE;
	}
	else if( det >= -0.00001f && det <= 0.00001f)		// �������� ������.
	{
		t	=	(-1*b) / (2*a);
	}
	else if( det >= 0.00001f)
	{
		t	=	((-1*b) - sqrt( det)) / (2*a);
	}

	if( t < 0) // �̹� ������ �ִ� ��� üũ.
	{
		vTemp	=	A-S;
		fTemp	=	vTemp*vTemp - (r+s)*(r+s);
		if( fTemp > 0.0f)		// ������ �ʴ´�.
		{
			return	FALSE;
		}
		_asm nop;
	}
	if( t >= 1.0f)		// �ʹ� �־ ������ �ʴ´�.
	{
		return	FALSE;
	}
	
	C	=	A + B*t;
	W	=	C + (S-C)*(r / (r+s));

	fTemp	=	(*pWhereMeet - A) * (B);
	if( fTemp <= 0.0f)
		return	FALSE;

	return	TRUE;
}

#include <math.H>
#include "../4DYuchiGXGFunc/global.h"
#include "myself97Common.h"
#include "logger.h"
#include "CheckClock.h"

// a�� b�� �ȸ����� false;
// a�� b�� ������ true;
bool		IsAABBMeetAABB( MAABB& a, MAABB& b)
{
	if( a.Min.x <= b.Max.x && a.Max.x >= b.Min.x)
	{
		if( a.Min.y <= b.Max.y && a.Max.y >= b.Min.y)
		{
			if( a.Min.z <= b.Max.z && a.Max.z >= b.Min.z)
			{
				// ������ ���.
				return true;
			}
		}
	}
	return false;
}

//	���Ǿ�� ���Ǿ �����°�.?
bool		IsSphereMeetSphere( BOUNDING_SPHERE& bs1, BOUNDING_SPHERE& bs2)
{
	VECTOR3		Length;
	VECTOR3_SUB_VECTOR3( &Length, &(bs1.v3Point), &(bs2.v3Point));
	float		fLength	=	VECTOR3Length( &Length);
	if( fLength <= bs1.fRs + bs2.fRs)
		return	true;

	return	false;
}

// ���Ǿ�� �ڽ��� �����°�?
bool		IsSphereMeetAABB( BOUNDING_SPHERE& bs, MAABB& Box)
{
	if( Box.Max.x >= bs.v3Point.x-bs.fRs)
		if( Box.Max.y >= bs.v3Point.y-bs.fRs)
			if( Box.Max.z >= bs.v3Point.z-bs.fRs)
				if( Box.Min.x <= bs.v3Point.x+bs.fRs)
					if( Box.Min.y <= bs.v3Point.y+bs.fRs)
						if( Box.Min.z <= bs.v3Point.z+bs.fRs)
							return	true;
	return		false;
}

// 0: ��ģ��. 1: ������ �ȿ� ������ ����. -1: �������� ������ �����.
int			WhereIsAABBFromViewVolume( MAABB& b, VIEW_VOLUME& vv)
{
/*	// �ٿ�� �������� �ӽ÷� ...
	VECTOR3		Mid;
	Mid.x = (b.Max.x + b.Min.x)/2;
	Mid.y = (b.Max.y + b.Min.y)/2;
	Mid.z = (b.Max.z + b.Min.z)/2;
	float		fRadius = sqrt( (Mid.x-b.Max.x)*(Mid.x-b.Max.x) + (Mid.x-b.Max.x)*(Mid.x-b.Max.x) + (Mid.x-b.Max.x)*(Mid.x-b.Max.x));
	int k = 0;
	int iD = 0;
	for( k = 0; k < 4; k++)
	{
		float fDetermin = vv.Plane[k].v3Up.x*Mid.x + vv.Plane[k].v3Up.y*Mid.y + vv.Plane[k].v3Up.z*Mid.z + vv.Plane[k].D;
		if( fDetermin > fRadius)	return -1;
		if( fDetermin < -fRadius)	iD++;
	}
	if( iD >= 4)	return 1;
	return 0;
*/
//////////////////////////////////////////////////////////////////////////

	int i = 0, j = 0, D = 0, D2 = 0;		// D�� ���͹� ��.
	float fResult = 0;
	// ���� ������ �����.
	VECTOR3 v[8];
	v[0].x = v[2].x = v[4].x = v[6].x = b.Min.x;
	v[1].x = v[3].x = v[5].x = v[7].x = b.Max.x;

	v[0].y = v[1].y = v[2].y = v[3].y = b.Max.y;
	v[4].y = v[5].y = v[6].y = v[7].y = b.Min.y;

	v[0].z = v[1].z = v[4].z = v[5].z = b.Max.z;
	v[2].z = v[3].z = v[6].z = v[7].z = b.Min.z;

	D2 = 0;

	


	for( j = 0; j < 4; j++)
	{
		D = 0;
		for( i = 0; i < 8; i++)
		{
			fResult =  vv.Plane[j].v3Up.x*v[i].x + vv.Plane[j].v3Up.y*v[i].y + vv.Plane[j].v3Up.z*v[i].z + vv.Plane[j].D;
			if( fResult > 0) D = D + 1;
			else if( fResult < 0) D = D - 1;
		}
		if( D >= 8)		// ������ ������ ��� �տ� ������,
		{
			return -1;	// �� ��鿡�� ������ ����� -1 ����.
		}
		if( D <= -8)	// ������ ������ ��� �ڿ� ������,
		{
			D2++;
		}
	}

	// �亼���� From�� �ٿ���ڽ� �ȿ� �ִ°�� return 0;
	if( vv.From.x > b.Min.x && vv.From.x < b.Max.x)
		if( vv.From.y > b.Min.y && vv.From.y < b.Max.y)
			if( vv.From.z > b.Min.z && vv.From.z < b.Max.z)
				return 0;

	// �Ÿ�.
	D = 0;
		VECTOR3		TempVec3;
	for( i = 0; i < 8; i++)
	{
		// �������� ���� ������ �Ÿ��� vv.fFar���� �ָ� -1, ������ +1�� �Ѵ�.
		if( vv.From.x > v[i].x)	TempVec3.x = vv.From.x-v[i].x;
		else					TempVec3.x = v[i].x - vv.From.x;
		if( vv.From.y > v[i].y)	TempVec3.y = vv.From.y-v[i].y;
		else					TempVec3.y = v[i].y - vv.From.y;
		if( vv.From.z > v[i].z)	TempVec3.z = vv.From.z-v[i].z;
		else					TempVec3.z = v[i].z - vv.From.z;

		fResult	=	TempVec3.x*TempVec3.x + TempVec3.y*TempVec3.y + TempVec3.z*TempVec3.z;
		float fMaxLength = vv.fFar * vv.fFar;
		if( fResult < fMaxLength)	D++;
		if( fResult > fMaxLength)	D--;
	}

	if( D <= -8)	return -1;		// �亼������ ������ �����.

	if( D >= 8 && D2 >= 4)
	{
		return 1;		// �亼���� ������ ����.
	}
	return 0;						// ���߰��ϰ� ��ģ��.

}
int WhereISBOUNDING_BOXFromViewVolume( BOUNDING_BOX& bb, VIEW_VOLUME& vv)
{
	VECTOR3*	v	=	bb.v3Oct;
	int			iDet = 0, iDet2 = 0;
	float		fResult;
	int	i, j;
	for( j = 0; j < 4; j++)
	{
		for( i = 0; i < 8; i++)
		{
			fResult =  vv.Plane[j].v3Up.x*v[i].x + vv.Plane[j].v3Up.y*v[i].y + vv.Plane[j].v3Up.z*v[i].z + vv.Plane[j].D;
			if( fResult > 0) iDet++;
			else if( fResult < 0) iDet--;
		}
		if( iDet >= 8)		// ������ ������ ��� �տ� ������,
		{
			return -1;	// �� ��鿡�� ������ ����� -1 ����.
		}
		if( iDet <= -8)	// ������ ������ ��� �ڿ� ������,
		{
			iDet2++;
		}
	}

	// �ӽñ���.
	if( iDet2 == 4)	return	1;
	return	0;
}

int		WhereIsAABBFromBoundingSphere( MAABB& b, BOUNDING_SPHERE& bs)
{
	// aabb�� �ٿ�� ���Ǿ ���� ���ؼ� ���� ���.
	VECTOR3		BoxMid;
	BoxMid.x	=	(b.Max.x + b.Min.x) / 2;
	BoxMid.y	=	(b.Max.y + b.Min.y) / 2;
	BoxMid.z	=	(b.Max.z + b.Min.z) / 2;
	float	fBoxRad	=	(float)sqrt( (BoxMid.x-b.Max.x)*(BoxMid.x-b.Max.x) + (BoxMid.y-b.Max.y)*(BoxMid.y-b.Max.y) + (BoxMid.z-b.Max.z)*(BoxMid.z-b.Max.z));

	float	fDist	=	(float)sqrt((BoxMid.x-bs.v3Point.x)*(BoxMid.x-bs.v3Point.x) + (BoxMid.y-bs.v3Point.y)*(BoxMid.y-bs.v3Point.y) + (BoxMid.z-bs.v3Point.z)*(BoxMid.z-bs.v3Point.z));
	if( fDist > bs.fRs + fBoxRad)	// �ٿ�� ���Ǿ�� �ȸ�����..
	{
		return -1;		// ������ �ȸ�����. ����.
	}
	// �̹����� ���� ���ɼ��� �ִ�.....
	if( fDist + fBoxRad < bs.fRs)	// �ٿ�� ���Ǿ��.. fRadius�� ������ ���ԵǸ�....
	{
		return 1;		// ������ ���Եȴ�.
	}

	// �ϴ� ���߰��ϰ� ���Եȴٷ�... ���߿� ����.
	return 0;
}


BOUNDING_SPHERE		GetSphereFromAABB( MAABB& Box)
{
	BOUNDING_SPHERE		Sphere;
	VECTOR3	Mid;
	Mid.x	=	(Box.Max.x+Box.Min.x)/2;
	Mid.y	=	(Box.Max.y+Box.Min.y)/2;
	Mid.z	=	(Box.Max.z+Box.Min.z)/2;
	float fRadius	=	(float)sqrt( (Mid.x-Box.Max.x)*(Mid.x-Box.Max.x) + (Mid.y-Box.Max.y)*(Mid.y-Box.Max.y) + (Mid.z-Box.Max.z)*(Mid.z-Box.Max.z));

	Sphere.fRs			=	fRadius;
	Sphere.v3Point		=	Mid;

	return Sphere;
}


// 0: ��ģ��. 1: ���� �ȿ� ������ ����. -1: �������� ������ �����.
int			WhereIsSphereFromViewVolume( BOUNDING_SPHERE& Sphere, VIEW_VOLUME& vv)
{
	int i = 0, D = 0;
	for( i = 0; i < 4; i++)
	{
		float	fDetermin	=	vv.Plane[i].v3Up.x*Sphere.v3Point.x  +  vv.Plane[i].v3Up.y*Sphere.v3Point.y + vv.Plane[i].v3Up.z*Sphere.v3Point.z + vv.Plane[i].D;
		if( fDetermin > Sphere.fRs)		return -1;
		if( fDetermin < -1*Sphere.fRs)	D++;
	}
	// �Ÿ� Ŭ����.

	if( D >=4)	return 1;
	return 0;
}
/*
BOOL ClipSphereWithViewVolume(VIEW_VOLUME* pVolume,BOUNDING_SPHERE* pSphere)
{
	BOOL		bResult = FALSE;
	float		D = 0,fDist;
	VECTOR3		v3Dist;
	
	DWORD	j;
	// �亼�� Ŭ����.
	for( j = 0; j < 4; j++)
	{
		// �ٿ�� ���Ǿ�.
		D = DotProduct(&pVolume->Plane[j].v3Up,&pSphere->v3Point) + pVolume->Plane[j].D;

		if( D > pSphere->fRs)	
			goto lb_return;
	}

	// �亼�� �Ÿ� Ŭ����.
	VECTOR3_SUB_VECTOR3(&v3Dist,&pVolume->From,&pSphere->v3Point);

	fDist = VECTOR3Length(&v3Dist);
	if( pSphere->fRs + pVolume->fFar < fDist )	
		goto lb_return;


	bResult = TRUE;

lb_return:
	return bResult;
}
*/

/*

  2004�� 7�� 10�� GFunc�� �Լ��� ��ü�Կ� ���� �ּ�ó��
void	MakePLANE( PLANE* pP, VECTOR3* pv1, VECTOR3* pv2, VECTOR3* pv3)
{
	VECTOR3	vec1;
	VECTOR3	vec2;
//	vec1	=	v2	-	v1;
//	vec2	=	v3	-	v1;
	VECTOR3_SUB_VECTOR3( &vec1, pv2, pv1);
	VECTOR3_SUB_VECTOR3( &vec2, pv3, pv1);

	CrossProduct( &(pP->v3Up), &vec1, &vec2);
	Normalize( &(pP->v3Up), &(pP->v3Up));
	pP->D = -1 * ( DotProduct( &(pP->v3Up), pv1));

}

//	���� From-To ������ �� v�� ���� ����� ���� ã�´�.
//	LineFrom�� LineTo�� ���� �ʴٰ� �����Ѵ�.


void			FindNearestVertexOnLine( VECTOR3* pOut, float* pT, VECTOR3& From, VECTOR3& To, VECTOR3& v)
{
	//	���� From-To���� �� ���� �� Result��� �Ҷ�, ���� From-To��  ���� Result-v�� ������ 0�̴�.
	//	������ ����Result = ����From + t*����FromTo
	//	t�� ���� ���ϰ�, �� t�� ���������Ŀ� ����.
	//	t = ((T-F)*(V-F)) / ((T-F)*(T-F))

//	*pT			=	((To.x-From.x)*(v.x-From.x) + (To.y-From.y)*(v.y-From.y) + (To.z-From.z)*(v.z-From.z)) / ( (To.x-From.x)*(To.x-From.x) + (To.y-From.y)*(To.y-From.y) + (To.z-From.z)*(To.z-From.z) );


//	(*pOut).x	=	From.x + (*pT) * (To.x - From.x);
//	(*pOut).y	=	From.y + (*pT) * (To.y - From.y);
//	(*pOut).z	=	From.z + (*pT) * (To.z - From.z);
//	VECTOR3_SUB_VECTOR3( pOut, &To, &From);
//	VECTOR3_MULEQU_FLOAT( pOut, *pT);
//	VECTOR3_ADDEQU_VECTOR3( pOut, &From);

	VECTOR3		Velocity;
	VECTOR3_SUB_VECTOR3( &Velocity, &To, &From);
	VECTOR3		FV;
	VECTOR3_SUB_VECTOR3( &FV, &v, &From);

	*pT		=	DotProduct( &Velocity, &FV) / DotProduct( &Velocity, &Velocity);

	*pOut	=	Velocity;
	VECTOR3_MULEQU_FLOAT( pOut, *pT);
	VECTOR3_ADDEQU_VECTOR3( pOut, &From);
}

// ���Ǿ�� ������ ��� �����°�.? ����0�̸� �ȸ�����, 1�̸� ��������, 2�� �������� ������.

  */
// �̳��� trymoveto ����� �Ǹ� ������ ���̴�.
DWORD	WhereDidSphereMeetLine( VECTOR3* OUT pCandidate1, float* OUT pT1, VECTOR3* OUT pCandidate2, float* OUT pT2, VECTOR3* IN pLFrom, VECTOR3* IN pLTo, VECTOR3* IN pSPivot, float fSRadius)
{
	// �ϴ� ������ �ƴ� ��, ���� ���� �ΰ� ���Դ°�.?
	if( IsVECTOR3Same( pLFrom, pLTo))
	{
		// �׳� �ȸ����ɷ� �ϰ� ����.
		return 0;
	}

	// ���� ���� -_-;   (-b (+-) ��Ʈ(b*b-4ac) )/(2a)
	VECTOR3		V;
	float		tV;
	FindNearestVertexOnLine( &V, &tV, pLFrom, pLTo, pSPivot);
	float		fPV	=	CalcDistance( &V, pSPivot);

	if( (fPV - fSRadius) >= 0.5 )			//	�ƹ��͵� ������ �ʴ´�.
	{
		return	0;		// ���� �� ���� ����.
	}

	// �ϳ�, �Ǵ� �ΰ��� ������ ������. �Ǻ��� b����-4ac��.
	float	t1;		// ù��° ��.
	float	t2;		// �ι�° ��.
	float	a, b, c;


	a	=	((*pLTo).x-(*pLFrom).x)*((*pLTo).x-(*pLFrom).x)	+	((*pLTo).y-(*pLFrom).y)*((*pLTo).y-(*pLFrom).y)	+	((*pLTo).z-(*pLFrom).z)*((*pLTo).z-(*pLFrom).z);
	b	=	2*( ((*pLTo).x-(*pLFrom).x)*((*pLFrom).x-(*pSPivot).x) + ((*pLTo).y-(*pLFrom).y)*((*pLFrom).y-(*pSPivot).y) + ((*pLTo).z-(*pLFrom).z)*((*pLFrom).z-(*pSPivot).z) );
	c	=	((*pLFrom).x-(*pSPivot).x)*((*pLFrom).x-(*pSPivot).x)	+	((*pLFrom).y-(*pSPivot).y)*((*pLFrom).y-(*pSPivot).y)	+	((*pLFrom).z-(*pSPivot).z)*((*pLFrom).z-(*pSPivot).z) - fSRadius*fSRadius;

	if( ( b*b - 4*a*c)	<=	0.1)		// 0.1�� 0�� �ٻ�ġ... 
	{
		if( (b*b - 4*a*c) <	-0.1)
		{
			_asm int 3;
		}
	}
	float	fTemp	=	(float)sqrt(b*b-4*a*c);
	t1	=	(-1*b - fTemp)	/	(2*a);
	t2	=	(-1*b + fTemp)	/	(2*a);

	if( t1 < 0)
	{
		if( t1 >= -0.1)	t1	=	0.0f;		// t1�� ���ġ �̳����� 0���� ������.?
//		else	_asm int 3;
	}

	*pT1	=	t1;
	(*pCandidate1).x	=	(*pLFrom).x + t1*((*pLTo).x - (*pLFrom).x);
	(*pCandidate1).y	=	(*pLFrom).y + t1*((*pLTo).y - (*pLFrom).y);
	(*pCandidate1).z	=	(*pLFrom).z + t1*((*pLTo).z - (*pLFrom).z);

	*pT2	=	t2;
	(*pCandidate2).x	=	(*pLFrom).x + t2*((*pLTo).x - (*pLFrom).x);
	(*pCandidate2).y	=	(*pLFrom).y + t2*((*pLTo).y - (*pLFrom).y);
	(*pCandidate2).z	=	(*pLFrom).z + t2*((*pLTo).z - (*pLFrom).z);


	if( (fPV - fSRadius) < 0.5 && (fPV-fSRadius) >= -0.5)
	{
		// �������� �����ٰ� �� �� �ִ�. 
		return 1;
	}
	return 2;
}


BOOL	IsVECTOR3Same( VECTOR3* pV1, VECTOR3* pV2)
{
	float	x	=	(*pV1).x - (*pV2).x;
	float	y	=	(*pV1).y - (*pV2).y;
	float	z	=	(*pV1).z - (*pV2).z;
	if( x >= (fFloatMin/2) && x < (fFloatMin/2) && y >= (fFloatMin/2) && y < (fFloatMin/2) && z >= (fFloatMin/2) && z < (fFloatMin/2) )	return TRUE;
	return FALSE;
}


void		GetTriMiddle( VECTOR3* pMid, VECTOR3* pTri)
{
	(*pMid).x		=	(pTri[0].x + pTri[1].x + pTri[2].x) / 3;
	(*pMid).y		=	(pTri[0].y + pTri[1].y + pTri[2].y) / 3;
	(*pMid).z		=	(pTri[0].z + pTri[1].z + pTri[2].z) / 3;
}

// �ٿ�� ���Ǿ�� �ٿ�� ĸ���� �����°�.?
// bc�� �������� ������ F, T, �������� �� ���� X, bs�� ������ P�� �ϸ�,
// X = F + t(T-F); (T-F)(X-P) = 0;
int		BOUNDING_CAPSULE_Meet_BOUNDING_SPHERE( BOUNDING_CAPSULE& bc, BOUNDING_SPHERE& bs)
{
	VECTOR3		X;

	VECTOR3		TF	=	bc.v3To		-	bc.v3From;		// T-F
	VECTOR3		FP	=	bc.v3From	-	bs.v3Point;		//	F-P

	float		t	=	-1.0f * DotProduct(&TF, &FP) / DotProduct( &TF, &TF);

	// X == ??
	float		fLength;
	if( t > 1)		// to�� ������Ƿ� to�� ���Ǿ��� ������ �Ÿ� ���.
	{
		X		=	bc.v3To;
	}
	else if( t < 0)		// from�� ������Ƿ� from�� ���Ǿ��� ������ �Ÿ� ���.
	{
		X		=	bc.v3From;
	}
	else //  0 <= t <= 1
	{
//		X		=	bc.v3From	+	TF * t;
		VECTOR3_MUL_FLOAT( &X, &TF, t);
		VECTOR3_ADDEQU_VECTOR3( &X, &(bc.v3From));

	}
	VECTOR3_SUB_VECTOR3( &X, &X, &(bs.v3Point));
	fLength		=	VECTOR3Length( &X);


	if( fLength > bs.fRs + bc.fRadius )		// ������ �ȸ����� ���.
		return	-1;

	if( fLength + bs.fRs > bc.fRadius)		// ��ġ�鼭 ������ ���.
		return	0;

	// ������ ������ ���.
	return	1;
}

// �ﰢ����, �� ������� �����Ŷ�� �����ϴ� ���ؽ��� �����°�.
// ���ϰ��� ������ TRUE�̰�, WhereMeet�� ����01*x + ����02*y�� ��Ÿ���� �ؽ�����ǥ������.
//BOOL	IsTriMeetVertex( VECTOR2* pWhereMeet, VECTOR3* pTri, VECTOR3* pV)
BOOL	IsTriMeetVertex( VECTOR3* pTri, VECTOR3* pV)
{

	VECTOR3		OnLine;
	float		fWhere;
	PLANE		p;
	float		fTemp;
	//	0,1
	FindNearestVertexOnLine( &OnLine, &fWhere, &(pTri[0]), &(pTri[1]), &(pTri[2]));
	p.v3Up	=	pTri[2]	-	OnLine;
	p.D		=	-1.0f * DotProduct( &(p.v3Up), &OnLine);

	fTemp	=	DotProduct( &(p.v3Up), pV)	+	p.D;
	if( fTemp < 0.0f)
		return	FALSE;

	//	1,2
	FindNearestVertexOnLine( &OnLine, &fWhere, &(pTri[1]), &(pTri[2]), &(pTri[0]));
	p.v3Up	=	pTri[0]	-	OnLine;
	p.D		=	-1.0f * DotProduct( &(p.v3Up), &OnLine);

	fTemp	=	DotProduct( &(p.v3Up), pV)	+	p.D;
	if( fTemp < 0.0f)
		return	FALSE;


	//	2,0
	FindNearestVertexOnLine( &OnLine, &fWhere, &(pTri[2]), &(pTri[0]), &(pTri[1]));
	p.v3Up	=	pTri[1]	-	OnLine;
	p.D		=	-1.0f * DotProduct( &(p.v3Up), &OnLine);

	fTemp	=	DotProduct( &(p.v3Up), pV)	+	p.D;
	if( fTemp < 0.0f)
		return	FALSE;


	return	TRUE;

/*
	float&	s	=	pWhereMeet->x;
	float&	t	=	pWhereMeet->y;

	PLANE		p;		// �Ÿ��� ���Ҷ� �� ���.

	VECTOR3		OnLine;	// �ﰢ�� ���� ���� ��.
	float		f1;		// �ﰢ���� �������� ������ ������������ �Ÿ�.
	float		f2;		// �ﰢ���� �������� �� v������ �Ÿ�.
	
	float		fTemp;

	// s	(0,1)
	// 0,2���� ��� p���Ѵ�.
	FindNearestVertexOnLine2( &OnLine, &fTemp, pTri, &(pTri[2]), &(pTri[1]));
	VECTOR3_SUB_VECTOR3( &p.v3Up, &(pTri[1]), &OnLine);
	Normalize( &p.v3Up, &p.v3Up);
	p.D	=	-1.0f * DotProduct( &p.v3Up, &OnLine);
	
	f1	=	DotProduct( &p.v3Up, &pTri[1])	+	p.D;
	f2	=	DotProduct( &p.v3Up, pV)	+	p.D;

	s	=	f2 / f1;
	
	// t	(0,2)
	// 0,1���� ��� p ���Ѵ�.
	FindNearestVertexOnLine2( &OnLine, &fTemp, pTri, &(pTri[1]), &(pTri[2]));
	VECTOR3_SUB_VECTOR3( &p.v3Up, &pTri[2], &OnLine);
	Normalize( &p.v3Up, &p.v3Up);
	p.D	=	-1.0f * DotProduct( &p.v3Up, &OnLine);

	f1	=	DotProduct( &p.v3Up, &pTri[2])	+	p.D;
	f2	=	DotProduct( &p.v3Up, pV)	+	p.D;

	t	=	f2 / f1;

	// s�� t�� ��ȿ�Ѱ�?( �ﰢ�� �ȿ� ���°�) �Ǻ�.
	if( s > 1.0f || s < 0.0f || t > 1.0f || t < 0.0f || s+t > 1.0f)
		return	FALSE;
	
	return	TRUE;
*/
}



//	 �־��� �ﰢ���� ����� ��� �ִ°�.?
//	����� �տ� ������ 2, ����� �ڿ� ������ -2, ����� �����ִµ� �븻�� ������� 1, �븻�� �ݴ��� ��� -1, 
//	���� �ﰢ���� �����ϰ� ������ 0 ����.
int		WhereIsTriangleFromPLANE(VECTOR3 *pTri, PLANE *pPlane)
{
	DWORD	i;
	VECTOR3		Normal, vec1, vec2;
	
	DWORD	dwFront = 0;		// ����� �տ� �մ� �� ����.
	DWORD	dwBack = 0;			// �ڿ� �ִ� �� ����.
	DWORD	dwOn = 0;			// ��� ���� �ִ� �� ����.
	
	float	d;
	for( i = 0; i < 3; i++)
	{
//		d	=	pPlane->v3Up.x*pTri[i].x + pPlane->v3Up.y*pTri[i].y + pPlane->v3Up.z*pTri[i].z + pPlane->D;
		d	=	DotProduct( &(pPlane->v3Up), &pTri[i]) + pPlane->D;
		if( d > 0.01f )			dwFront++;
		else if( d < -0.01f )	dwBack++;
		else					dwOn++;
	}
	
	if( dwOn == 3)	// �� ������ �� ������� �ִ°��, �븻�� �Ǻ��ؼ� -1, 1�� ����.
	{
		VECTOR3_SUB_VECTOR3( &vec1, &(pTri[1]), &(pTri[0]));
		VECTOR3_SUB_VECTOR3( &vec2, &(pTri[2]), &(pTri[0]));
		CrossProduct(	&Normal, &vec1, &vec2);
		// ��� ���Ϳ� �ﰢ�� �븻�� ��.
		float	dot	=	DotProduct( &Normal, &(pPlane->v3Up));
		if( dot > 0)	return 1;
		else if( dot < 0)	return -1;
		else	_asm int 3;
	}
	
	if( dwFront > 0)	// ���� �ּ� �ϳ��� �տ� �������.
	{
		if( dwBack == 0)	// ������ �ڿ� �ϳ��� ���°��. 
		{
			return 2;
		}
		// ���� �ϳ��� �ڿ� ������, cross.
		return 0;
	}
	
	// �̱��� ���°� ������ ��� �ڿ� �ִٴ� ��.
	return	-2;
}

// pTri�� �־��� �ﰢ���� AABB�� ���Ѵ�.
// Ȥ�� ���ǰ� ���ٸ� FALSE�� ����.
BOOL	MakeAABBFromTri( MAABB* pOutBox, VECTOR3* pTri)
{
	pOutBox->Max	=	pOutBox->Min	=	pTri[0];

	DWORD	i;
	for( i = 1; i < 3; i++)
	{
		VECTOR3*	pV	=	&(pTri[i]);
		if( pOutBox->Max.x < pV->x)
				pOutBox->Max.x	=	pV->x;
		if( pOutBox->Max.y < pV->y)
				pOutBox->Max.y	=	pV->y;
		if( pOutBox->Max.z < pV->z)
				pOutBox->Max.z	=	pV->z;

		if( pOutBox->Min.x > pV->x)
				pOutBox->Min.x	=	pV->x;
		if( pOutBox->Min.y > pV->y)
				pOutBox->Min.y	=	pV->y;
		if( pOutBox->Min.z > pV->z)
				pOutBox->Min.z	=	pV->z;
	}

	if( pOutBox->Max == pOutBox->Min)
	{
		return	FALSE;
	}

	return	TRUE;
}

int	BoundingCapsuleMeetPlane( BOUNDING_CAPSULE* pBC, PLANE* pPlane)
{
#ifdef	_DEBUG
	// �������� 0?
	if( pBC->fRadius < 0.1f)
		_asm int 3;
	// �ٿ�� ĸ���� �ƴ϶� �׳� ���̸�.?
	if( pBC->v3From == pBC->v3To)
		_asm int 3;
#endif

	// �ϴ� ����� �븻 ���Ϳ�, �ٿ��ĸ���� ���⺤��(?)�� ������ ���, �������� ������ �Ǻ��Ѵ�.
	VECTOR3		vBCVec;
	VECTOR3_SUB_VECTOR3( &vBCVec, &pBC->v3To, &pBC->v3From);
	VECTOR3		vBCDir;
	Normalize( &vBCDir, &vBCVec);
	float	fDotPlaneNormalBCDir	=	DotProduct( &vBCDir, &pPlane->v3Up);
	if( fDotPlaneNormalBCDir <= 0.0001f && fDotPlaneNormalBCDir >= -0.0001f)		// == 0.0f
	{
		float	fDistFromPlane	=	DotProduct( &pBC->v3From, &pPlane->v3Up)	+	pPlane->D;
		if( fDistFromPlane < 0.0f)
			fDistFromPlane *= -1.0f;
		// �������� �����,
		if( fDistFromPlane > pBC->fRadius)
		{
			return	-1;				// �ƿ� ������ �ʴ´�.
		}
		// ������ ������ ���ٸ�,
		if( fDistFromPlane < pBC->fRadius - 0.1f)				// �ణ�� �Ӱ�ġ.
		{
			return	1;				// ������ ������.
		}

		// �� ���ϴ� �����,?
		return	0;

	}
	
	// �Ϲ����� ����.
	// bc�� vector���� ���� X = F + (T-F)*t �� X*N + d = r ����,
	// t = (r - d - F*N) / ((T-F)*N)
	float	fFN	=	DotProduct(&pBC->v3From, &pPlane->v3Up);

	VECTOR3		TF	=	pBC->v3To;
	VECTOR3_SUBEQU_VECTOR3( &TF, &pBC->v3From);
	float	fTFN	=	DotProduct( &TF, &pPlane->v3Up);
	
	// ��, t���� ���Դ�.
	float	t	=	(pBC->fRadius - pPlane->D - fFN) / ( fTFN);

	// �������� Ȯ���� ��������,?
//	if( t > 0.0f && t < 1.0f)
	if( t < 1.0f)
	{
		return	1;
	}

	// ���� BC�� Vector���� ������ �������,?
	if( t > 1.0f || t < 0.0f)
	{
		return	-1;
	}

	return	0;			// t�� 0�ΰų� t�� 1�̸� �� �����°��̹Ƿ�,
	
}

/*
	Myself97Common.h
	�Ϲ������� ���̴� �Լ� ����.
*/

#ifndef	_Myself97Common_H_
#define	_Myself97Common_H_


#include "../4DyuchiGRX_common/typedef.h"
#include <limits.h>

// �浹 ó�� ���ϰ� ����.
// ���ϰ��� ���� COLLISIONTYPE�̰�, ���ϵǴ� ���� �Ʒ��� ������ �Ǵ� ��.
#define		COLLISIONTYPE						int

#define		COLLISIONTYPE_Error					-1

#define		COLLISIONTYPE_None					0
#define		COLLISIONTYPE_Vertex				1
#define		COLLISIONTYPE_Line					2
#define		COLLISIONTYPE_LineSegment			3
#define		COLLISIONTYPE_Plane					4
#define		COLLISIONTYPE_Triangle				5


// aabb�� aabb�� �����°�.?
bool		IsAABBMeetAABB( MAABB& a, MAABB& b);

//	���Ǿ�� ���Ǿ �����°�.?
bool		IsSphereMeetSphere( BOUNDING_SPHERE& bs1, BOUNDING_SPHERE& bs2);

// ���Ǿ�� �ڽ��� �����°�?
bool		IsSphereMeetAABB( BOUNDING_SPHERE& bs, MAABB& Box);

// 0: ��ģ��. 1: ���� �տ� �ִ�. -1: ���� �ڿ� �ִ�.
int			WhereIsAABBFromViewVolume( MAABB& b, VIEW_VOLUME& vv);
// 0: ��ģ��. 1: ������ �ȿ� ������ ����. -1: �������� ������ �����.
//int			WhereIsBBoxFrom
//	MAABB���� BOUNDING_SPHERE�� ������ִ� �Լ�.
BOUNDING_SPHERE		GetSphereFromAABB( MAABB& Box);
int			WhereIsSphereFromViewVolume( BOUNDING_SPHERE& Sphere, VIEW_VOLUME& vv);
// 0: ��ģ��. 1: ���� �ȿ� ������ ����. -1: �������� ������ �����.
int		WhereIsAABBFromBoundingSphere( MAABB& b, BOUNDING_SPHERE& bs);
// 0: ��ģ��. 1: ���� �ȿ� ������ ����. -1: �������� ������ �����.
int			WhereISBOUNDING_BOXFromViewVolume( BOUNDING_BOX& bb, VIEW_VOLUME& vv);
// 0: ��ģ��. 1: ���� �ȿ� ������ ����. -1: �������� ������ �����.
int		BOUNDING_CAPSULE_Meet_BOUNDING_SPHERE( BOUNDING_CAPSULE& bc, BOUNDING_SPHERE& bs);
// true, false ������ �ȸ�����.
// ���Ǿ�� ������ ��� �����°�.? ����0�̸� �ȸ�����, 1�̸� ��������, 2�� �������� ������.
DWORD WhereDidSphereMeetLine( VECTOR3* OUT pCandidate1, float* OUT pT1, VECTOR3* OUT pCandidate2, float* OUT pT2, VECTOR3* IN pLFrom, VECTOR3* IN pLTo, VECTOR3* IN pSPivot, float fSRadius);



//	���� From-To ������ �� v�� ���� ����� ���� ã�´�.
//	LineFrom�� LineTo�� ���� �ʴٰ� �����Ѵ�.



BOOL		IsVECTOR3Same( VECTOR3* pV1, VECTOR3* pV2);

void		GetTriMiddle( VECTOR3* pMid, VECTOR3* pTri);

// �ﰢ����, �� ������� �����Ŷ�� �����ϴ� ���ؽ��� �����°�.
// ���ϰ��� ������ TRUE�̰�, WhereMeet�� ����01*x + ����02*y�� ��Ÿ���� �ؽ�����ǥ������.
BOOL		IsTriMeetVertex(  VECTOR3* pTri, VECTOR3* pVertex);



#ifdef	_DEBUG

inline	void	CheckVectorArray( VECTOR3* pArray, DWORD dwCount)
{
	float*	p	=	(float*)pArray;
	for( DWORD i = 0; i < dwCount*3; i++)
	{
		if( p[i] < -80000.0f || p[i] > 80000.0f)
		{
			_asm int 3;
			return;
		}
	}
	return;
}
#else

#define CheckVectorArray(x,count)
#endif


// �ζ���ȭ �Ѱ�, ����׸���� �� �Լ�ȣ���� ���ֺ�����... -_-;

#ifdef	_DEBUG
inline void	PrintfDebugString( char* szFormat, ...)
{
	va_list argptr;
	char cBuf[1024];


	va_start( argptr, szFormat );
	vsprintf( cBuf, szFormat, argptr );
	va_end( argptr );

	OutputDebugString( cBuf);

}

#define	PrintfDebugCodeLine() PrintfDebugString( "Code = %s, Line = %d\n", __FILE__, __LINE__)



#else

#define	PrintfDebugString /**/

#endif


//	 �־��� �ﰢ���� ����� ��� �ִ°�.?
//	����� �տ� ������ 2, ����� �ڿ� ������ -2, ����� �����ִµ� �븻�� ������� 1, �븻�� �ݴ��� ��� -1, 
//	���� �ﰢ���� �����ϰ� ������ 0 ����.
int		WhereIsTriangleFromPLANE(VECTOR3 *pTri, PLANE *pPlane);

// pTri�� �־��� �ﰢ���� AABB�� ���Ѵ�.
// Ȥ�� ���ǰ� ���ٸ� FALSE�� ����.
BOOL	MakeAABBFromTri( MAABB* pOutBox, VECTOR3* pTri);
//BOOL ClipSphereWithViewVolume(VIEW_VOLUME* pVolume,BOUNDING_SPHERE* pSphere);

// ������ ������ +1, �� ���ϸ� 0, �ȸ����� -1
int	BoundingCapsuleMeetPlane( BOUNDING_CAPSULE* pBC, PLANE* pPlane);

#endif

// ��ġ�� �ּ�ó������...GFunc�� �ִ� �Լ��� ��ü
//void	MakePLANE( PLANE* pP, VECTOR3* pv1, VECTOR3* pv2, VECTOR3* pv3);
//void			FindNearestVertexOnLine( VECTOR3* pOut, float* pT, VECTOR3& LineFrom, VECTOR3& LineTo, VECTOR3& v);

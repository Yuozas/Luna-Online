/*
	AAEllipsoidCollisionTest.h
*/

#ifndef	_Ellipsoid_h_
#define	_Ellipsoid_h_

#include "../../4DyuchiGRX_common/typedef.h"



struct	ELLIPSOID
{
	// ����� �ܺο��� �Է� �޴°�.
	VECTOR3		P;			// �Ǻ�, ���밪.
	float		w;			// x, z�� ����.
	float		h;			// y���� ����.

	// ���⼭���ʹ� �Է� �����ɷ� ����°�.
	float		hperw;			// h/w ��.
	float		k;				// ������ ��,�Ʒ��������� �Ÿ�.
	VECTOR3		U;			// Ÿ���� ���� ����, P������ ��� ��ġ�� �ƴϴ�.
	VECTOR3		L;			// Ÿ���� �Ʒ��� ����, P������ ��� ��ġ�� �ƴϴ�.

	void	Create( const VECTOR3& Pivot, float fWidth, float fHeight);
	void	SetPosition( const VECTOR3& Pivot);			// �ű��. U�� L�� ���ŵȴ�.
	
};

#endif
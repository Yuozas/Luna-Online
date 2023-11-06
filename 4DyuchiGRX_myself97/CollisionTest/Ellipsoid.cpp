
#include "Ellipsoid.h"

#include	"../myself97common.h"


void ELLIPSOID::Create( const VECTOR3& Pivot, float fWidth, float fHeight)
{
	// ���̺��� ���̰� ũ�� ���ȴ�.
	BreakIfTrue( w < 1.0f || h < 1.0f || h < w);

	// �ϴ� �������� �״��...
	w	=	fWidth;
	h	=	fHeight;

	if( w == h)
	{
		k	=	0.0f;
		hperw	=	1.0f;
	}
	else	// w > h
	{
		// �Ǻ����κ��� J, K������ �Ÿ�.
		k		=	(float)sqrt( h*h - w*w);
		// h/w ?
		hperw	=	h / w;
	}

	SetPosition( Pivot);

}

// �ű��. U�� L�� ���ŵȴ�.
void ELLIPSOID::SetPosition( const VECTOR3& Pivot)
{
	this->P	=	Pivot;
	
	// U	���� ��.
	U		=	Pivot;
	U.y		+=	k;

	// L	�Ʒ�����.
	L		=	Pivot;
	L.y		-=	k;

}

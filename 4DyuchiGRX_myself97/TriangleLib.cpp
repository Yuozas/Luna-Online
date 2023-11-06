
#include	"TriangleLib.h"
#include	"../4DyuchiGXGFunc/global.h"

void	SetTri( VECTOR3* pOut, VECTOR3* pV0, VECTOR3* pV1, VECTOR3* pV2)
{
	pOut[0]	=	*pV0;
	pOut[1]	=	*pV1;
	pOut[2]	=	*pV2;
}

DWORD	ClipTriangle( VECTOR3* pOut, PLANE p, VECTOR3* pV0, VECTOR3* pV1, VECTOR3* pV2 )
{
	// �� �������� ��� ������ �Ÿ�. ��ȣ�� ���δ�.
	float	fD0	=	DotProduct( &p.v3Up, pV0)	+	p.D;
	if( fD0 <= 0.1f && fD0 >= -0.1f)
		fD0	=	0.0f;
	float	fD1	=	DotProduct( &p.v3Up, pV1)	+	p.D;
	if( fD1 <= 0.1f && fD1 >= -0.1f)
		fD1	=	0.0f;
	float	fD2	=	DotProduct( &p.v3Up, pV2)	+	p.D;
	if( fD2 <= 0.1f && fD2 >= -0.1f)
		fD2	=	0.0f;

	// ��� �𼭸��� �߷ȳ�.?
	bool	bEdge01Clipped	=	false;
	bool	bEdge12Clipped	=	false;
	bool	bEdge20Clipped	=	false;

	// �𼭸��� �߸���.
	VECTOR3		Edge01Clipped;
	VECTOR3		Edge12Clipped;
	VECTOR3		Edge20Clipped;

	// �𼭸��� �߸� ����.
	DWORD	dwClippedCount	=	0;

	VECTOR3	V;


	float	t;
	float	tUnder;

	// 0-1 ���̰� �߷��� ���.
	if( fD0 * fD1 < 0)
	{
		// 01������ �߸����� ���Ѵ�.
		// X = 0 + (1-0)*t,
		// X*N + d = 0;
		// t = -1*(0*N+d)/((1-0)*n)		t != 0 �̾�� �Ѵ�.;
		VECTOR3_SUB_VECTOR3( &V, pV1, pV0);
		tUnder	=	DotProduct( &V, &p.v3Up);
		if( tUnder <= 0.00001f && tUnder >= -0.00001f)
		{
			_asm int 3;
			// ������ ���� �����ٰ� �����Ǿ����� ����̶� ������ �����Ҷ�. ���� �ȵǴ� ����.
			goto lbHere1;
		}
		t		=	-1.0f * (DotProduct( pV0, &p.v3Up) + p.D) / tUnder;
		if( t > 1.0f || t < 0.0f)
		{
			_asm int 3;
			// ������ ����� �����ٰ� �����Ǿ�����, ���ѵ��� ������ ������ ����. ���� �ȵǴ� ����.
			goto lbHere1;
		}
		
//		Edge01Clipped	=	*pV0	+	V * t;
		VECTOR3_MUL_FLOAT( &Edge01Clipped, &V, t);
		VECTOR3_ADDEQU_VECTOR3( &Edge01Clipped, pV0);
		
		dwClippedCount++;
		bEdge01Clipped	=	true;
	}
lbHere1:

	if( fD1*fD2 < 0)
	{
		// 12������ �߸����� ���Ѵ�.
		// X = 1 + (2-1)*t,
		// X*N + d = 0;
		// t = -1*(1*N+d)/((2-1)*n)		t != 0 �̾�� �Ѵ�.;
		VECTOR3_SUB_VECTOR3( &V, pV2, pV1);
		tUnder	=	DotProduct( &V, &p.v3Up);
		if( tUnder <= 0.00001f && tUnder >= -0.00001f)
		{
			_asm int 3;
			// ������ ���� �����ٰ� �����Ǿ����� ����̶� ������ �����Ҷ�. ���� �ȵǴ� ����.
			goto	lbHere2;
		}
		t		=	-1.0f * (DotProduct( pV1, &p.v3Up) + p.D) / tUnder;
		if( t > 1.0f || t < 0.0f)
		{
			_asm int 3;
			// ������ ����� �����ٰ� �����Ǿ�����, ���ѵ��� ������ ������ ����. ���� �ȵǴ� ����.
			goto	lbHere2;
		}
		
//		Edge12Clipped	=	*pV0	+	V * t;
		VECTOR3_MUL_FLOAT( &Edge12Clipped, &V, t);
		VECTOR3_ADDEQU_VECTOR3( &Edge12Clipped, pV1);

		dwClippedCount++;
		bEdge12Clipped	=	true;
	}
lbHere2:

	if( fD2*fD0 < 0)
	{
		// 20������ �߸����� ���Ѵ�.
		// X = 2 + (0-2)*t,
		// X*N + d = 0;
		// t = -1*(2*N+d)/((0-2)*n)		t != 0 �̾�� �Ѵ�.;
		VECTOR3_SUB_VECTOR3( &V, pV0, pV2);
		tUnder	=	DotProduct( &V, &p.v3Up);
		if( tUnder <= 0.00001f && tUnder >= -0.00001f)
		{
			_asm int 3;
			// ������ ���� �����ٰ� �����Ǿ����� ����̶� ������ �����Ҷ�. ���� �ȵǴ� ����.
			goto	lbHere3;
		}
		t		=	-1.0f * (DotProduct( pV2, &p.v3Up) + p.D) / tUnder;
		if( t > 1.0f || t < 0.0f)
		{
			_asm int 3;
			// ������ ����� �����ٰ� �����Ǿ�����, ���ѵ��� ������ ������ ����. ���� �ȵǴ� ����.
			goto	lbHere3;
		}
		
//		Edge20Clipped	=	*pV0	+	V * t;
		VECTOR3_MUL_FLOAT( &Edge20Clipped, &V, t);
		VECTOR3_ADDEQU_VECTOR3( &Edge20Clipped, pV2);

		dwClippedCount++;
		bEdge20Clipped	=	true;
	}
lbHere3:

	switch( dwClippedCount)
	{
		case	0:		// �ϳ��� ���߸� ���. ������ ���ų�, ������ �ȵ��ų� ���� �ϳ���.
			if( fD0 > 0.0f || fD1 > 0.0f || fD2 > 0.0f)
			{	
				SetTri( &(pOut[0]), pV0, pV1, pV2);
//				pOut[0]	=	*pV0;
//				pOut[1]	=	*pV1;
//				pOut[2]	=	*pV2;
				return	1;
			}
			else
			{
				return	0;
			}
			break;
		case	1:		// ���� �ϳ��� �߸����. �׷��� �������� ���� ���� �����ִ�.
			// ���� ����� ���ؽ��� 3���̶�� �ϰ� 0���� �ɸ���� 013, 032�� �ﰢ���� ������.
			if( fD0 == 0.0f)		// 0���� �߰��� �ɷ��ְ�, 1,2 ������ �߸����.
			{
				if( fD1 > 0.0f)
				{
					// 013
//					pOut[3*0+0]	=	*pV0;
//					pOut[3*0+1]	=	*pV1;
//					pOut[3*0+2]	=	Edge12Clipped;
					SetTri( pOut, pV0, pV1, &Edge12Clipped);
				}
				else	//(fD2 > 0.0f)
				{
					// 032
//					pOut[3*0+0]	=	*pV0;
//					pOut[3*0+1]	=	Edge12Clipped;
//					pOut[3*0+2]	=	*pV2;
					SetTri(pOut, pV0, &Edge12Clipped, pV2);
				}
			}
			else if( fD1 == 0.0f)	// 1���� �߰��� �ɷ��ְ�, 0,2 ������ �ɸ����.
			{
				if( fD2 > 0.0f)
				{
					// 123
//					pOut[3*0+0]	=	*pV1;
//					pOut[3*0+1]	=	*pV2;
//					pOut[3*0+2]	=	Edge20Clipped;
					SetTri( pOut, pV1, pV2, &Edge20Clipped);
				}
				else	//(fD0 > 0.0f)
				{
					// 130
//					pOut[3*0+0]	=	*pV1;
//					pOut[3*0+1]	=	Edge20Clipped;
//					pOut[3*0+2]	=	*pV0;
					SetTri( pOut, pV1, &Edge20Clipped, pV0);
				}

			}
			else if( fD2 == 0.0f)	//	2���� �߰��� �ɸ��� 0,1 ������ �ɸ� ���.
			{
				if( fD0 > 0.0f)
				{
					// 203
					SetTri( pOut, pV2, pV0, &Edge01Clipped);
//					pOut[3*0+0]	=	*pV2;
//					pOut[3*0+1]	=	*pV0;
//					pOut[3*0+2]	=	Edge01Clipped;
				}
				else	// (fD1 > 0.0f)
				{
					// 231
//					pOut[3*0+0]	=	*pV2;
//					pOut[3*0+1]	=	Edge01Clipped;
//					pOut[3*0+2]	=	*pV1;
					SetTri( pOut, pV2, &Edge01Clipped, pV1);
				}
			}
			else
			{
				// �׳� ����ϸ� �������.?
				_asm int 3;
			}

			return	1;
			break;
		case	2:	// ������ �α������� �߸� ���.
			if( bEdge12Clipped == false)
			{
				if( fD0 > 0.0f)
				{
					// 0(01)(20)
//					pOut[3*0+0]	=	*pV0;
//					pOut[3*0+1]	=	Edge01Clipped;
//					pOut[3*0+2]	=	Edge20Clipped;

					SetTri( pOut, pV0, &Edge01Clipped, &Edge20Clipped);
					return	1;
				}
				else
				{
					// 1(20)(01)
//					pOut[3*0+0]	=	*pV1;
//					pOut[3*0+1]	=	Edge20Clipped;
//					pOut[3*0+2]	=	Edge01Clipped;

					SetTri( pOut, pV1, &Edge20Clipped, &Edge01Clipped);
					// 2(20)1
//					pOut[3*1+0]	=	*pV2;
//					pOut[3*1+1]	=	Edge20Clipped;
//					pOut[3*1+2]	=	*pV1;

					SetTri( &(pOut[3]), pV2, &Edge20Clipped, pV1);
				}
			}
			else if( bEdge20Clipped == false)
			{
				if( fD1 > 0.0f)
				{
					// 1(12)(01)
//					pOut[3*0+0]	=	*pV1;
//					pOut[3*0+1]	=	Edge12Clipped;
//					pOut[3*0+2]	=	Edge01Clipped;

					SetTri( pOut, pV1, &Edge12Clipped, &Edge01Clipped);
					return	1;
				}
				else
				{
					// 2(01)(12)
//					pOut[3*0+0]	=	*pV2;
//					pOut[3*0+1]	=	Edge01Clipped;
//					pOut[3*0+2]	=	Edge12Clipped;

					SetTri( pOut, pV2, &Edge01Clipped, &Edge12Clipped);
					// 0(01)2
//					pOut[3*1+0]	=	*pV0;
//					pOut[3*1+1]	=	Edge01Clipped;
//					pOut[3*1+2]	=	*pV2;

					SetTri( &(pOut[3]), pV0, &Edge01Clipped, pV2);
				}
			}
			else if( bEdge01Clipped	== false)
			{
				if( fD2 > 0.0f)
				{
					// 2(20)(12)
//					pOut[3*0+0]	=	*pV2;
//					pOut[3*0+1]	=	Edge20Clipped;
//					pOut[3*0+2]	=	Edge12Clipped;

					SetTri( pOut, pV2, &Edge20Clipped, &Edge12Clipped);
					return	1;
				}
				else
				{
					// 0(12)(20)
//					pOut[3*0+0]	=	*pV0;
//					pOut[3*0+1]	=	Edge12Clipped;
//					pOut[3*0+2]	=	Edge20Clipped;

					SetTri( pOut, pV0, &Edge12Clipped, &Edge20Clipped);
					// 1(12)0
//					pOut[3*1+0]	=	*pV1;
//					pOut[3*1+1]	=	Edge12Clipped;
//					pOut[3*1+2]	=	*pV0;

					SetTri( &pOut[3], pV1, &Edge12Clipped, pV0);
				}
			}
			else		
			{
				// �׳� ����ϸ� �������.?
				_asm int 3;
			}
			return	2;
			break;
#ifdef	_DEBUG
		case	3:
		default:		// �̷����� ����. ���� �� �߸��ٰų� �ϴ�.. -_-;
			_asm int 3;
#endif
	}

#ifdef	_DEBUG
	_asm int 3;	// �̱��� �� �� ���� �����°͵� ���.
#endif
	return	0;
}

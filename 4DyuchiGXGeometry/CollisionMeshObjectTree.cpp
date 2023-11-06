// CollisionMeshObjectTree.cpp: implementation of the CCollisionMeshObjectTree class.
//
//////////////////////////////////////////////////////////////////////

#include "CollisionMeshObjectTree.h"
#include "CollisionStaticModel.h"
#include "../4DyuchiGXGFunc/global.h"
#include	"../4DyuchiGRX_myself97/myself97common.h"
#include	"../4DyuchiGRX_myself97/CheckClock.h"
#include	"../4DyuchiGRX_myself97/CollisionTest/CollisionTestMovingEllipsoidMeetTriangle.h"





/*
struct	BSP_BUILD_INFO1
{
	// phase1
	DWORD						dwTriCount;
	DWORD						dwCurrentCount;
	VECTOR3**					ppTriBuff;					// �ﰢ���� ������ ����.
	PLANE**						ppPlaneBuff;				// ����� ������ ����.
	CCollisionMeshObject**		ppObjPointer;
};
BSP_BUILD_INFO1		gBuildInfo;

struct	BSP_BUILD_INFO2
{
	DWORD						dwPlaneCount;
	PLANE**						ppResultPlane;				// ��� ��Ƶ� ����.
	DWORD*						pPlaneReffCount;			// �ش� �ε����� ����� ����̳� �����糪?
	DWORD*						pResultPlaneIndex;			// �ش� �ﰢ���� ��� �ε���.
};
BSP_BUILD_INFO2		gPhase2;
*/


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCollisionMeshObjectTree::CCollisionMeshObjectTree()
{
	m_Triangle.dwCount		=	0;
	m_Triangle.ppBuff		=	0;
	m_Triangle.pdwPlaneIndex	=	0;
	m_Triangle.pBox			=	0;

	m_Plane.pPlane			=	0;
	m_Plane.dwCount			=	0;

	m_dwMemoryUsage			=	0;

	m_pOutputBuffer			=	0;
	m_dwOutputBufferCount	=	0;
//	m_pOutputLink			=	0;
	

	memset( &m_Grid, 0, sizeof(m_Grid));
}

CCollisionMeshObjectTree::~CCollisionMeshObjectTree()
{
	// �޸� ��縮 üũ.
	if( m_Triangle.dwCount != 0)
		_asm int 3;
	if( m_Triangle.ppBuff != 0)
		_asm int 3;
	if( m_Triangle.pdwPlaneIndex != 0)
		_asm int 3;

	if( m_Plane.dwCount != 0)
		_asm int 3;
	if( m_Plane.pPlane != 0)
		_asm int 3;

}


BOOL CCollisionMeshObjectTree::CreateTree(CCollisionMeshObject *pObjBuff, DWORD dwObjCount)
{
	PrintfDebugString("CCollisionMeshObjectTree::CreateTree() ObjCount = %d\n", dwObjCount);
//	PrintfDebugCodeLine();
	DWORD	dwFuncClock	=	GetTickCount();
	
	DWORD	i, j;

	DWORD	dwTriPhaseClock	=	GetTickCount();

	// ������Ʈ�� ���� �ﰢ���� ������ ��´�.
	CCollisionMeshObject*	pObj	=	0;
	m_Triangle.dwCount	=	0;
	for( i = 0;i < dwObjCount; i++)
	{
		pObj	=	&(pObjBuff[i]);
		m_Triangle.dwCount	+=	pObj->GetFaceCount();
	}

	// ���鼭 �ﰢ���� ����� ��´�.
	m_Triangle.ppBuff			=	new	VECTOR3*[m_Triangle.dwCount];		// �ﰢ�� ����.
	m_Triangle.pdwPlaneIndex	=	new	DWORD[m_Triangle.dwCount];			// �ﰢ���� ��� �ε���.
	m_Triangle.ppObjBuff		=	new	CCollisionMeshObject*[m_Triangle.dwCount];		// ������Ʈ ����.
	m_Triangle.pBox				=	new	MAABB[m_Triangle.dwCount];


	m_Plane.pPlane				=	new	PLANE[m_Triangle.dwCount];			// ����� �� �ӽù��۷� ����..
#ifdef	_DEBUG
	// Ȥ�ó� �� ���.
	memset( m_Triangle.ppBuff, 0, sizeof(VECTOR3*)*m_Triangle.dwCount);
	memset( m_Triangle.pdwPlaneIndex, 0, sizeof(DWORD)*m_Triangle.dwCount);
	memset( m_Triangle.ppObjBuff, 0, sizeof(CCollisionMeshObject*)*m_Triangle.dwCount);
	memset( m_Plane.pPlane, 0, sizeof(PLANE)*m_Triangle.dwCount);
#endif
	DWORD	dwTemp	=	0;
	for( i = 0; i < dwObjCount; i++)
	{
		pObj	=	&(pObjBuff[i]);
		DWORD			dwObjTriCount	=	pObj->GetFaceCount();
		VECTOR3*		pObjTriBuff		=	pObj->GetTriBuffer();
		for( j = 0; j < dwObjTriCount; j++)
		{
			// �ﰢ�� ����.
			m_Triangle.ppBuff[dwTemp]	=	&(pObjTriBuff[j*3]);			// �ﰢ�� ����.
			// ������Ʈ ����
			m_Triangle.ppObjBuff[dwTemp]	=	&(pObjBuff[i]);
			// ��� ����� �ִ´�.

			// �ε� Ÿ���� ���̱� ���� �Ž����� ���� ���ð�...�翬�� ���� �ʵ���Ÿ��� �ٽ� �ͽ���Ʈ�ؾ�...-yuchi
//			m_Plane.ppPlane[dwTemp]		=	&(pObjPlane[j]);				// ��� ����.
//			MakePLANE( &(m_Plane.pPlane[dwTemp]), m_Triangle.ppBuff[dwTemp], m_Triangle.ppBuff[dwTemp]+1, m_Triangle.ppBuff[dwTemp]+2);
			CalcPlaneEquation( &(m_Plane.pPlane[dwTemp]), m_Triangle.ppBuff[dwTemp]);
			
			float	fTemp	=	VECTOR3Length( &(m_Plane.pPlane[dwTemp].v3Up));
			if( fTemp <= 0.9999f)
				_asm int 3;
			if( fTemp > 1.1f)
				_asm int 3;
			// MAABB �����ִ´�.
			BOOL	bMakeAABB	=	MakeAABBFromTri( &(m_Triangle.pBox[dwTemp]), m_Triangle.ppBuff[dwTemp]);
			if( bMakeAABB == FALSE)
			{
				// ���ǰ� ���� �ﰢ���� ���Դ�.
				PrintfDebugString( "CCollisionMeshObjectTree::CreateTree() ���ǰ� ���� �ﰢ���� .col ���Ͽ� �����ִ�. (The Triangle has no Volume!)\n");
				_asm int 3;
			}

			
			dwTemp++;
		}
	}

#ifdef	_DEBUG
	if( dwTemp != m_Triangle.dwCount)
		_asm int 3;
#endif
	dwTriPhaseClock	=	GetTickCount()	-	dwTriPhaseClock;
	PrintfDebugString("CCollisionMeshObjectTree::CreateTree() Tri Phase Tick = %d\n", dwTriPhaseClock);
	PrintfDebugString("CCollisionMeshObjectTree::CreateTree() TriCount = %d\n", m_Triangle.dwCount);




	// ����� ���鼭 ���� ����� ��󳽴�.
	PLANE*		pPlane	=	new	PLANE[m_Triangle.dwCount];			// Ÿ�� ����.
#ifdef	_DEBUG
	memset( pPlane, 0, sizeof(PLANE)*m_Triangle.dwCount);
#endif
	// �ﰢ�� ������ �ϳ��ų� ���ų� �ϸ�,
	if( m_Triangle.dwCount < 2)
		_asm int 3;



	DWORD	dwFindSamePlaneClock	=	GetTickCount();



	// �ϴ� �ϳ��� ���������� �ΰ�,
	pPlane[0]	=	m_Plane.pPlane[0];
	m_Triangle.pdwPlaneIndex[0]	=	0;
	dwTemp		=	1;

	// �ﰢ�� ��ü ����� ���鼭,
	for( i = 1; i < m_Triangle.dwCount; i++)
	{
		PLANE*		pSrc	=	&(m_Plane.pPlane[i]);
		// Ÿ�� ������ ������ ���ƺ��鼭 �������� ã�´�.
		for( j = 0; j < dwTemp; j++)		
		{
			PLANE*		pDest	=	&(pPlane[j]);

			// �ϴ� D
			float	fDist	=	pSrc->D		-	pDest->D;
			if( fDist > -0.01f && fDist < 0.01f)
			{
				// �븻 ����.
				float	fDot	=	DotProduct( &(pDest->v3Up), &(pSrc->v3Up));
				if( fDot > 1.000001f)
					_asm int 3;
				if( fDot > 0.99f)
				{
					// �̳��� ���� ���̴�.
					m_Triangle.pdwPlaneIndex[i]	=	j;
					break;		// ���� ����������.
				}
			}
				_asm nop
		}

		// ���� ��ã������,
		if( j == dwTemp)
		{
			pPlane[dwTemp]	=	*pSrc;					// ���ۿ� �ϳ� �����ϰ�,
			m_Triangle.pdwPlaneIndex[i]	=	dwTemp;		// �ε����� �� ����� �� ����,
			dwTemp++;									// Ÿ�� ��� �ϳ� ����.
		}
	}

//	dwTemp;// �� ����� ����.


	// ��� ���� ����.
	m_Plane.dwCount		=	dwTemp;
	delete[]	m_Plane.pPlane;
	m_Plane.pPlane		=	new	PLANE[dwTemp];

	m_dwMemoryUsage	+=	sizeof(DWORD)*1;
	m_dwMemoryUsage	+=	sizeof(PLANE)*m_Plane.dwCount;


	memcpy( m_Plane.pPlane, pPlane, sizeof(PLANE)*dwTemp);

	delete[]	pPlane;
	pPlane	=	0;

	// ok
	dwFindSamePlaneClock	=	GetTickCount()	-	dwFindSamePlaneClock;
	PrintfDebugString("CCollisionMeshObjectTree::CreateTree() Plane Phase Ticks = %d\n", dwFindSamePlaneClock);





	// �׸��� �����.
	CreateGrid();

	// �ƿ�ǲ ����.
	CreateFindBuffer( 1);			// �̶� 2�� �׳� �׽�Ʈ���̴�.

	// �޸� ��뷮��.
	// �ﰢ��.
	m_dwMemoryUsage	+=	sizeof(DWORD)*m_Triangle.dwCount;
	m_dwMemoryUsage	+=	sizeof(VECTOR3*)*m_Triangle.dwCount;
	m_dwMemoryUsage	+=	sizeof(CCollisionMeshObject*)*m_Triangle.dwCount;
	m_dwMemoryUsage	+=	sizeof(MAABB)*m_Triangle.dwCount;

	// �׸���.
	m_dwMemoryUsage	+=	sizeof(M_GRID_SECTOR)*m_Grid.dwGridXCount*m_Grid.dwGridYCount*m_Grid.dwGridZCount;
	m_dwMemoryUsage	+=	sizeof(DWORD)*m_Grid.dwTriIndexCount;

	// �ƿ�ǲ ����.
	m_dwMemoryUsage	+=	sizeof(DWORD)*m_Triangle.dwCount;
	// �ƿ�ǲ ��ũ.
	m_dwMemoryUsage	+=	(sizeof(OUTPUT_LINK)*m_Triangle.dwCount+1);

	dwFuncClock	=	GetTickCount()	-	dwFuncClock;
	PrintfDebugString("CCollisionMeshObjectTree::CreateTree() Total Build Tree Ticks = %d, MemoryUsage = %d\n", dwFuncClock, m_dwMemoryUsage);
	return	TRUE;
}


void CCollisionMeshObjectTree::DeleteTree()
{
	// ��� ���� �����.
	DeleteFindBuffer();

	// �׸��� �����.
	DeleteGrid();

	// �ﰢ�� �����.
	if(m_Plane.dwCount)
	{
		delete[]	m_Plane.pPlane;
		m_Plane.pPlane		=	0;
		m_Plane.dwCount		=	0;
	}
	if( m_Triangle.dwCount)
	{
		delete[]	m_Triangle.pdwPlaneIndex;
		m_Triangle.pdwPlaneIndex	=	0;
		delete[]	m_Triangle.ppBuff;
		m_Triangle.ppBuff			=	0;
		delete[]	m_Triangle.ppObjBuff;
		m_Triangle.ppObjBuff		=	0;
		delete[]	m_Triangle.pBox;
		m_Triangle.pBox				=	0;
		m_Triangle.dwCount			=	0;
	}

	
}

void CCollisionMeshObjectTree::CreateGrid()
{
	DWORD	i, j, k;
	DWORD	a, b, c;
	M_GRID_SECTOR*	pGrid;
	// 1. ��ü ���带 �Ʒ��� bsp�� �ѰŶ�� �����ϰ� ������ ���.?
	// ��������� ���带 ������ �ϴ°�.?
	// ������? ���� ũ��? �ﰢ�� ����? ����.?
	// ���� ũ�Ⱑ ���ڴ�.
	// ���ϰ� ������ ���� ������ ���, ���ſ� ���� ���ſ� ���. ������ ������ ��¼���.

	// 2. �׸��� ��ü������ �ﰢ���� ������ ���� ���. ������ ���� ������, �׽�Ʈ�� �� �ؼ� �� �߰� �ڸ���,
	// ���� ���� ������ �ﰢ���� ������ Ʈ���� �����Ѵ�.

	//�Ʒ��� �ӽ� ����. ������ ������ �ٽ� ��� �ǰ�, ��� ����, Ȥ�� �׸��� ������ �ܺο��� �Է� �� �����̴�.
	VECTOR3		vGridStep;
	vGridStep.x					=	100.0f;
	vGridStep.y					=	100.0f;
	vGridStep.z					=	100.0f;

	// �ϴ� ���� �ڽ��� ���Ѵ�.
	MAABB	Box;
	Box.Min	=	Box.Max	=	*(m_Triangle.ppBuff[0]);
	for( i = 0; i < m_Triangle.dwCount; i++)
	{
		VECTOR3*		pTriangle	=	m_Triangle.ppBuff[i];
		for( j = 0; j < 3; j++)
		{
			VECTOR3*	pV	=	pTriangle+j;		// ������ ���ϱ�. ����.
			if( Box.Max.x < pV->x)
					Box.Max.x = pV->x;
			if( Box.Max.y < pV->y)
					Box.Max.y = pV->y;
			if( Box.Max.z < pV->z)
					Box.Max.z = pV->z;

			if( Box.Min.x > pV->x)
					Box.Min.x = pV->x;
			if( Box.Min.y > pV->y)
					Box.Min.y = pV->y;
			if( Box.Min.z > pV->z)
					Box.Min.z = pV->z;
		}
	}

	// +10.0f ������ ������...	 �ʿ䰡 �ִ����� ������ ���� ������ �Ǵ�.
	Box.Max.x	+=	10.0f;
	Box.Max.y	+=	10.0f;
	Box.Max.z	+=	10.0f;
	Box.Min.x	-=	10.0f;
	Box.Min.y	-=	10.0f;
	Box.Min.z	-=	10.0f;

	m_Grid.WorldBox	=	Box;

	// �׸��� ���� ���ϰ�,
	m_Grid.dwGridXCount	=	DWORD((Box.Max.x - Box.Min.x)	/	vGridStep.x);
	if( m_Grid.dwGridXCount == 0)
			m_Grid.dwGridXCount		=	1;			// �ּ� �Ѱ��� �־��....
	m_Grid.fGridXStep	=	(Box.Max.x	-	Box.Min.x)	/	m_Grid.dwGridXCount;		// ������ �ٽ� ���.

	m_Grid.dwGridYCount	=	DWORD((Box.Max.y - Box.Min.y)	/	vGridStep.y);
	if( m_Grid.dwGridYCount == 0)
			m_Grid.dwGridYCount		=	1;			// �ּ� �Ѱ��� �־��....
	m_Grid.fGridYStep	=	(Box.Max.y	-	Box.Min.y)	/	m_Grid.dwGridYCount;		// ������ �ٽ� ���.

	m_Grid.dwGridZCount	=	DWORD((Box.Max.z - Box.Min.z)	/	vGridStep.z);
	if( m_Grid.dwGridZCount == 0)
			m_Grid.dwGridZCount		=	1;			// �ּ� �Ѱ��� �־��....
	m_Grid.fGridZStep	=	(Box.Max.z	-	Box.Min.z)	/	m_Grid.dwGridZCount;		// ������ �ٽ� ���.

	// �� �׸��带 �����ϰ�, ���� ������ ä���. bsp�� �� ���߿�.
	DWORD	dwGridCount	=	m_Grid.dwGridXCount * m_Grid.dwGridYCount * m_Grid.dwGridZCount;
//	m_Grid.

	m_Grid.pGrid		=	new	M_GRID_SECTOR[dwGridCount];


	memset( m_Grid.pGrid, 0, sizeof(M_GRID_SECTOR)*dwGridCount);

	// �׸��带 ���鼭 AABB�� ä���.
	for( k = 0; k < m_Grid.dwGridZCount; k++)
	{
		for( j = 0; j < m_Grid.dwGridYCount; j++)
		{
			for( i = 0; i < m_Grid.dwGridXCount; i++)
			{
				_asm nop;
				// ���� �ش� �׸����� AABB �ڽ��� �����.
				pGrid		=	m_Grid.GetSector( i, j, k);
				Box.Min.x	=	m_Grid.WorldBox.Min.x + ((m_Grid.WorldBox.Max.x - m_Grid.WorldBox.Min.x) / m_Grid.dwGridXCount) * i;
				Box.Max.x	=	m_Grid.WorldBox.Min.x + ((m_Grid.WorldBox.Max.x - m_Grid.WorldBox.Min.x) / m_Grid.dwGridXCount) * (i+1);

				Box.Min.y	=	m_Grid.WorldBox.Min.y + ((m_Grid.WorldBox.Max.y - m_Grid.WorldBox.Min.y) / m_Grid.dwGridYCount) * j;
				Box.Max.y	=	m_Grid.WorldBox.Min.y + ((m_Grid.WorldBox.Max.y - m_Grid.WorldBox.Min.y) / m_Grid.dwGridYCount) * (j+1);

				Box.Min.z	=	m_Grid.WorldBox.Min.z + ((m_Grid.WorldBox.Max.z - m_Grid.WorldBox.Min.z) / m_Grid.dwGridZCount) * k;
				Box.Max.z	=	m_Grid.WorldBox.Min.z + ((m_Grid.WorldBox.Max.z - m_Grid.WorldBox.Min.z) / m_Grid.dwGridZCount) * (k+1);

				pGrid->BB	=	Box;
			}
		}
	}



	////////////////////////////////////////////////////////////////
	// �ﰢ�� �� �׸��� �ȿ� �ִ´�.
	////////////////////////////////////////////////////////////////

	DWORD	dwIntegratedBuffSize	=	0;
	for( i = 0; i < m_Triangle.dwCount; i++)			// �ﰢ���� ���ƺ��鼭,
	{
		VECTOR3*		pTri	=	m_Triangle.ppBuff[i];
		MAABB*			pBox	=	&(m_Triangle.pBox[i]);
		PLANE*			pPlane	=	&(m_Plane.pPlane[m_Triangle.pdwPlaneIndex[i]]);
		// �� �ﰢ���� �� �׸������ ���Ѵ�.
		DWORD	dwXStart	=	DWORD((pBox->Min.x - m_Grid.WorldBox.Min.x) / m_Grid.fGridXStep);
		DWORD	dwXEnd		=	DWORD((pBox->Max.x - m_Grid.WorldBox.Min.x) / m_Grid.fGridXStep);

		DWORD	dwYStart	=	DWORD((pBox->Min.y - m_Grid.WorldBox.Min.y) / m_Grid.fGridYStep);
		DWORD	dwYEnd		=	DWORD((pBox->Max.y - m_Grid.WorldBox.Min.y) / m_Grid.fGridYStep);

		DWORD	dwZStart	=	DWORD((pBox->Min.z - m_Grid.WorldBox.Min.z) / m_Grid.fGridZStep);
		DWORD	dwZEnd		=	DWORD((pBox->Max.z - m_Grid.WorldBox.Min.z) / m_Grid.fGridZStep);

#ifdef	_DEBUG
	// ����ڽ����� �ﰢ���� �� �ָ� �ִ� ���. �ﰢ���� �������� ����ڽ��� ������µ� ����ڽ��� ����� �ﰢ���� ���� �� ����.
	if( dwXStart > m_Grid.dwGridXCount || dwXEnd > m_Grid.dwGridXCount)
		_asm int 3;
	if( dwYStart > m_Grid.dwGridYCount || dwYEnd > m_Grid.dwGridYCount)
		_asm int 3;
	if( dwZStart > m_Grid.dwGridZCount || dwZEnd > m_Grid.dwGridZCount)
		_asm int 3;
#endif

		// ���۰� ���� �������Ƿ� �ش� �׸��忡 �ﰢ������ ����ִ´�.



		for( c = dwZStart; c <= dwZEnd; c++)
		{
			for( b = dwYStart; b <= dwYEnd; b++)
			{
				for( a = dwXStart; a <= dwXEnd; a++)
				{
					// ��ü �׸��忡 ���� ���� ���۸� ����� ���� �ʿ䷮�� ����.
					_asm nop;
					pGrid	=	m_Grid.GetSector(a, b, c);
					// ���� �� �׸��忡 �ﰢ���� ���� �Ǵ°�.?
					// �ֱ� ���� �׽�Ʈ.
					BOOL	bGridMeetTri	=	TestPlaneWithGrid( pGrid, pPlane);
					if( bGridMeetTri)
					{
						// �ϴ� ����Ÿ�Ӷ� �ӽ÷� ��ũ����Ʈ�� dwTriStartIndex���� ���ųְ�, 
						// �ﰢ�� �ε��� ��ũ�� �߰�.
						M_TRIANGLE_LINK*		pLink	=	new	M_TRIANGLE_LINK;
						pLink->dwIndex		=	i;
						if( pGrid->pLink)
						{
							pLink->pNext	=	pGrid->pLink;			// �߰��� �����ִ´�.
							pGrid->pLink	=	pLink;
						}
						else	// pGrid->pLink == 0;
						{
							pGrid->pLink			=	pLink;
							pGrid->pLink->pNext		=	0;
						}


						pGrid->dwTriCount++;
						dwIntegratedBuffSize++;
					}
					else
						_asm nop;

				}
			}
		}
		_asm nop;
	}

	dwIntegratedBuffSize;

	DWORD*		pTriIndexBuffer	=	new	DWORD[dwIntegratedBuffSize];
	DWORD		dwTriBuffUsage	=	0;


	for( c = 0; c < m_Grid.dwGridZCount; c++)
	{
		for( b = 0; b < m_Grid.dwGridYCount; b++)
		{
			for( a = 0; a < m_Grid.dwGridXCount; a++)
			{
				pGrid	=	m_Grid.GetSector(a, b, c);
//				if( pGrid->bCheck)
//					_asm int 3;
//				pGrid->bCheck	=	TRUE;
				// ��ũ�� ���鼭 ���ۿ� �ε����� ����ִ´�.
				if( pGrid->dwTriCount)		// �ﰢ���� �ϳ��� ������,
				{
					DWORD	dwStartIndex	=	dwTriBuffUsage;	// pGrid->dwStart   ����� �ٷ� ������ ����ü�� pLink�� ����������.
					M_TRIANGLE_LINK*		pLink	=	pGrid->pLink;
					M_TRIANGLE_LINK*		pNext;
					while( pLink)
					{
						pNext	=	pLink->pNext;		// ����Ÿ� ����ؼ�...
						pTriIndexBuffer[dwTriBuffUsage]	=	pLink->dwIndex;
						dwTriBuffUsage++;
						delete	pLink;
						pLink	=	pNext;
					}
//					if( pGrid->pLink)
//						_asm int 3;
					pGrid->pLink		=	0;
					pGrid->dwTriStartIndex	=	dwStartIndex;			// ���� ���ۿ����� �����ּ�.
				}
				else						// �ϳ��� ������ �׳� �Ѿ�� ����.?
				{
					pGrid->dwTriStartIndex	=	0xffffffff;		// �翬 ����.
					if( pGrid->dwTriCount != 0)
						_asm int 3;
				}
				
				_asm nop;
			}
		}
	}
	_asm nop;
	m_Grid.pTriIndexBuffer		=	pTriIndexBuffer;
	m_Grid.dwTriIndexCount		=	dwTriBuffUsage;

	// �׸��忡 �ﰢ���� �ִ� ���� ��ũ �����°� ���� ����.
	_asm nop
}

void CCollisionMeshObjectTree::DeleteGrid()
{
	if( m_Grid.pGrid)
	{
		if( m_Grid.dwTriIndexCount)
		{
			delete[]	m_Grid.pTriIndexBuffer;
			m_Grid.pTriIndexBuffer		=	0;
		}

		delete[]	m_Grid.pGrid;
		m_Grid.pGrid	=	0;
	}
	_asm nop
}

// �׸���� �ﰢ���� �����°�.?
BOOL CCollisionMeshObjectTree::TestPlaneWithGrid(M_GRID_SECTOR *pGrid, PLANE* pPlane)
{
	VECTOR3		v[8];
	// x > 0, y > 0, z > 0
	v[0]	=	pGrid->BB.Max;
	// x > 0, y > 0, z < 0
	v[1].x	=	pGrid->BB.Max.x;
	v[1].y	=	pGrid->BB.Max.y;
	v[1].z	=	pGrid->BB.Min.z;
	// x > 0, y < 0, z > 0
	v[2].x	=	pGrid->BB.Max.x;
	v[2].y	=	pGrid->BB.Min.y;
	v[2].z	=	pGrid->BB.Max.z;
	// x > 0, y < 0, z < 0
	v[3].x	=	pGrid->BB.Max.x;
	v[3].y	=	pGrid->BB.Min.y;
	v[3].z	=	pGrid->BB.Min.z;
	// x < 0, y > 0, z > 0
	v[4].x	=	pGrid->BB.Min.x;
	v[4].y	=	pGrid->BB.Max.y;
	v[4].z	=	pGrid->BB.Max.z;
	// x < 0, y > 0, z < 0
	v[5].x	=	pGrid->BB.Min.x;
	v[5].y	=	pGrid->BB.Max.y;
	v[5].z	=	pGrid->BB.Min.z;
	// x < 0, y < 0, z > 0
	v[6].x	=	pGrid->BB.Min.x;
	v[6].y	=	pGrid->BB.Min.y;
	v[6].z	=	pGrid->BB.Max.z;
	// x < 0, y < 0, z < 0
	v[7].x	=	pGrid->BB.Min.x;
	v[7].y	=	pGrid->BB.Min.y;
	v[7].z	=	pGrid->BB.Min.z;

	DWORD	i;
	DWORD	dwPositive	=	0;		// ��� ����.
	DWORD	dwNegative	=	0;		// ��� ����.
	for( i = 0; i < 8; i++)
	{
		float	fDet	=	DotProduct( &(pPlane->v3Up), &(v[i]))	+	pPlane->D;
		if( fDet < 0.0f)
			dwNegative++;
//		else
//			dwPositive++;
	}

	if( dwNegative != 8)
		return	TRUE;

//aa

	// �ϴ� ��� �׽�Ʈ,
//	PLANE*	pPlane	=	m_Plane.pPlane[m_Triangle.pdwPlaneIndex[dwTriIndex]];

	return	FALSE;
}

/*
DWORD CCollisionMeshObjectTree::FindTriangleWithBS(BOUNDING_SPHERE *pBS)
{

	return	0;
}
*/

DWORD CCollisionMeshObjectTree::FindTriangleWithBC(BOUNDING_CAPSULE *pBC, BOOL bFaceFromVertex, DWORD dwOutputBufferIndex)
{
#ifdef	_DEBUG
	DBG_CheckOutputBufferIndex( dwOutputBufferIndex);
#endif
	// ���� �׸���� �� �׸����?
	MAABB	Box;
	// x
	if( pBC->v3From.x > pBC->v3To.x)
	{
		Box.Min.x	=	pBC->v3To.x;
		Box.Max.x	=	pBC->v3From.x;
	}
	else
	{
		Box.Min.x	=	pBC->v3From.x;
		Box.Max.x	=	pBC->v3To.x;
	}
	Box.Min.x	-=	pBC->fRadius;
	Box.Max.x	+=	pBC->fRadius;
	
	// y
	if( pBC->v3From.y > pBC->v3To.y)
	{
		Box.Min.y	=	pBC->v3To.y;
		Box.Max.y	=	pBC->v3From.y;
	}
	else
	{
		Box.Min.y	=	pBC->v3From.y;
		Box.Max.y	=	pBC->v3To.y;
	}
	Box.Min.y	-=	pBC->fRadius;
	Box.Max.y	+=	pBC->fRadius;

	// z
	if( pBC->v3From.z > pBC->v3To.z)
	{
		Box.Min.z	=	pBC->v3To.z;
		Box.Max.z	=	pBC->v3From.z;
	}
	else
	{
		Box.Min.z	=	pBC->v3From.z;
		Box.Max.z	=	pBC->v3To.z;
	}
	Box.Min.z	-=	pBC->fRadius;
	Box.Max.z	+=	pBC->fRadius;
	
	// ����, �� �ε���.
	int		iXStart	=	int( (Box.Min.x - m_Grid.WorldBox.Min.x) / m_Grid.fGridXStep);
	int	iXEnd		=	int( (Box.Max.x - m_Grid.WorldBox.Min.x) / m_Grid.fGridXStep);
	
	int	iYStart	=	int( (Box.Min.y - m_Grid.WorldBox.Min.y) / m_Grid.fGridYStep);
	int	iYEnd		=	int( (Box.Max.y - m_Grid.WorldBox.Min.y) / m_Grid.fGridYStep);
	
	int	iZStart	=	int( (Box.Min.z - m_Grid.WorldBox.Min.z) / m_Grid.fGridZStep);
	int	iZEnd		=	int( (Box.Max.z - m_Grid.WorldBox.Min.z) / m_Grid.fGridZStep);


	if( iXStart < 0)			// '-' ���� ���� ���� �ִ�.
			iXStart	=	0;
	if( iXEnd >= (int)m_Grid.dwGridXCount)
		iXEnd	=	m_Grid.dwGridXCount-1;
	if( iYStart < 0)			// '-' ���� ���� ���� �ִ�.
			iYStart	=	0;
	if( iYEnd >= (int)m_Grid.dwGridYCount)
		iYEnd	=	m_Grid.dwGridYCount-1;
	if( iZStart < 0)			// '-' ���� ���� ���� �ִ�.
			iZStart	=	0;
	if( iZEnd >= (int)m_Grid.dwGridZCount)
		iZEnd	=	m_Grid.dwGridZCount-1;

	

	// �ƿ�ǲ ��ũ Ŭ���� �ϰ�.
	ClearOutputLink( dwOutputBufferIndex);
	OUTPUT_INDEX_BUFFER*	pBuffer		=	&(m_pOutputBuffer[dwOutputBufferIndex]);
	int	i, j, k;
	for( k = iZStart; k <= iZEnd; k++)
	{
		for( j = iYStart; j <= iYEnd; j++)
		{
			for( i = iXStart; i <= iXEnd; i++)
			{
				M_GRID_SECTOR*	pGrid	=	m_Grid.GetSector( i, j, k);
				DWORD			dwStart	=	pGrid->dwTriStartIndex;						// ���� �ε���.
				DWORD			dwEnd	=	pGrid->dwTriCount	+	dwStart;			// �� �ε��� �ٷδ���(?).
				// �� �׸����� �ﰢ�� �ε������� �ƿ�ǲ ���ۿ� �ʾ��.
				DWORD	a;
				for( a = dwStart; a < dwEnd; a++)
				{
					int			iIndex	=	m_Grid.pTriIndexBuffer[a];		// �ε��� �δٸ� ���ļ���.?

					// �ϴ� BC�� ����� �ݴ°�.?
					PLANE*		pP	=	&(m_Plane.pPlane[m_Triangle.pdwPlaneIndex[iIndex]]);
//					int		iMeet	=	BoundingCapsuleMeetPlane( pBC, pP);
//					if( i > 0)
					{
						// face �ɼ� ��������, 
						if( bFaceFromVertex)			// ����� pBC->v3From�� ���� �ִ��� üũ �ϰ� 
						{
							float	fDet	=	DotProduct( &(pP->v3Up), &(pBC->v3From))	+	pP->D;
							if( fDet >= 0)
							{
//								int		iMeet	=	BoundingCapsuleMeetPlane( pBC, pP);
//								if( iMeet >= 0)
									AddOutputLink( iIndex, dwOutputBufferIndex);	// Output ���ۿ� ���.
//								else
									_asm nop;
							}
								
						}
						else
						{
							int		iMeet	=	BoundingCapsuleMeetPlane( pBC, pP);
							if( iMeet > 0)
								AddOutputLink( iIndex, dwOutputBufferIndex);		// Output ���ۿ� �׳� ���.

						}
					}


//					����
					_asm nop;
				}

			}
		}
	}
	
	// ��� ��ũ ����.
//	m_dwOutputIndexCount		=	0;
	pBuffer->dwOutputIndexCount		=	0;
//	DWORD	dwCurrentLink	=	m_pOutputLink[0].dwNext;			// 0���̾� -1�̴ϱ�.
	DWORD	dwCurrentLink	=	pBuffer->pOutputLink[0].dwNext;
	while(1)
	{
//		OUTPUT_LINK*	pCurrentLink	=	&(m_pOutputLink[dwCurrentLink]);
		OUTPUT_LINK*	pCurrentLink	=	&(pBuffer->pOutputLink[dwCurrentLink]);

		// �� �����..
		if( pCurrentLink->iIndex == INT_MAX)		
			break;

		// ����尡 �ƴ϶�� ���ۿ� ���.
//		m_pdwOutputIndexBuffer[m_dwOutputIndexCount]	=	pCurrentLink->iIndex;
//		m_dwOutputIndexCount++;
		pBuffer->pdwOutputIndexBuffer[pBuffer->dwOutputIndexCount]		=	pCurrentLink->iIndex;
		pBuffer->dwOutputIndexCount++;
		dwCurrentLink	=	pCurrentLink->dwNext;
	}
	


//	return	m_dwOutputIndexCount;
	return	pBuffer->dwOutputIndexCount;
}

void CCollisionMeshObjectTree::AddOutputLink(int iIndex, DWORD dwBufferIndex)
{
#ifdef	_DEBUG
	DBG_CheckOutputBufferIndex( dwBufferIndex);
#endif
	
	DWORD		dwCurrent	=	0;			// ��� ���.

#ifdef	_DEBUG
	if( iIndex < 0)
		_asm int 3;
	if( iIndex >= INT_MAX)
		_asm int 3;
#endif

	OUTPUT_INDEX_BUFFER*		pBuffer		=	&(m_pOutputBuffer[dwBufferIndex]);

	while(1)
	{
		int		iCurrentIndex	=	pBuffer->pOutputLink[dwCurrent].iIndex;
		// �̹� ���� �ִ� �ε����� ���Դٸ�, �� �� ���� ������.
		if( iCurrentIndex == iIndex)
			break;		// return?
		// ���� ���� �ִ� ��庸�� �۴ٸ� ���� ���� �Ѿ��.
		if( iCurrentIndex < iIndex)
		{
			dwCurrent	=	pBuffer->pOutputLink[dwCurrent].dwNext;
			continue;
		}
#ifdef	_DEBUG
		if( iCurrentIndex <= iIndex)
			_asm int 3;
#endif
		// ���� ���� �ִ� ��尡 iIndex���� Ŭ ���.
		// ���� ���� ���� ��� ���̿� ��带 �����ϰ� iIndex�� �����ִ´�.
		DWORD	dwNewNode	=	pBuffer->dwOutputLinkCurrent;//m_dwOutputLinkCurrent;
//		m_dwOutputLinkCurrent++;
		pBuffer->dwOutputLinkCurrent++;
//		m_pOutputLink[dwNewNode].dwNext	=	dwCurrent;
//		m_pOutputLink[dwNewNode].dwPrev	=	m_pOutputLink[dwCurrent].dwPrev;
//		m_pOutputLink[dwNewNode].iIndex	=	iIndex;
		pBuffer->pOutputLink[dwNewNode].dwNext	=	dwCurrent;
		pBuffer->pOutputLink[dwNewNode].dwPrev	=	pBuffer->pOutputLink[dwCurrent].dwPrev;
		pBuffer->pOutputLink[dwNewNode].iIndex	=	iIndex;

		DWORD	dwPrevNode	=	pBuffer->pOutputLink[dwCurrent].dwPrev;
		pBuffer->pOutputLink[dwPrevNode].dwNext	=	dwNewNode;

		pBuffer->pOutputLink[dwCurrent].dwPrev		=	dwNewNode;
		break;
	}




}

void CCollisionMeshObjectTree::ClearOutputLink( DWORD dwIndex)
{
#ifdef	_DEBUG
	DBG_CheckOutputBufferIndex( dwIndex);
#endif
	m_pOutputBuffer[dwIndex].dwOutputLinkCurrent	=	2;
//	m_dwOutputLinkCurrent	=	2;				// �Ҵ��� �Ҷ��� �ε���.

//	m_pOutputLink[0].dwPrev		=	0;
//	m_pOutputLink[0].dwNext		=	1;
//	m_pOutputLink[0].iIndex		=	-1;	// ��·�� �̳��� ���� �ճ��̴�.
	m_pOutputBuffer[dwIndex].pOutputLink[0].dwPrev		=	0;
	m_pOutputBuffer[dwIndex].pOutputLink[0].dwNext		=	1;
	m_pOutputBuffer[dwIndex].pOutputLink[0].iIndex		=	-1;	// ��·�� �̳��� ���� �ճ��̴�.

//	m_pOutputLink[1].dwNext		=	0;
//	m_pOutputLink[1].dwPrev		=	0;
//	m_pOutputLink[1].iIndex		=	INT_MAX;		// ��·�� �̳��� ���� �޳��̴�.
	m_pOutputBuffer[dwIndex].pOutputLink[1].dwNext		=	0;
	m_pOutputBuffer[dwIndex].pOutputLink[1].dwPrev		=	0;
	m_pOutputBuffer[dwIndex].pOutputLink[1].iIndex		=	INT_MAX;		// ��·�� �̳��� ���� �޳��̴�.

	
}

BOOL CCollisionMeshObjectTree::CollisionTestLine(float *pOutT, VECTOR3 *pFrom, VECTOR3 *pTo, DWORD dwOutputBufferIndex)
{
#ifdef	_DEBUG
	DBG_CheckOutputBufferIndex( dwOutputBufferIndex);
#endif

	// ������ Ʈ���� ���ľ� ������ '�ӽ�'�� ���� ���鼭 ���� ã�´�.
	VECTOR3		vVelocity;//	=	*pTo	-	*pFrom;
	VECTOR3_SUB_VECTOR3( &vVelocity, pTo, pFrom);
#ifdef	_DEBUG
	if( *pTo == *pFrom)
	{
		PrintfDebugString( "CCollisionMeshObjectTree::CollisionTestLine() pFrom�� pTo�� ����.( pFrom and pTo are Same!)\n");
		_asm int 3;
		return	FALSE;
	}
#else
	if( *pTo == *pFrom)
		return	FALSE;


	if( vVelocity.x	!= 0.0f)
		_asm nop;
	if( vVelocity.z != 0.0f)
		_asm nop;

#endif



	// Ʈ���� ������ �ϴ� �ﰢ�� ����Ʈ�� ��´�.
	BOUNDING_CAPSULE		BC;
//	BC.fRadius	=	1.0f;
	BC.fRadius	=	0.0f;
	BC.v3From	=	*pFrom;
	BC.v3To		=	*pTo;
	

	DWORD	dwFindTreeClock	=	GetLowClock();

	DWORD	dwTriFound	=	FindTriangleWithBC(&BC, TRUE, dwOutputBufferIndex);
	if( dwTriFound == 0)		// ã���� ������ �ٷ� ����������,
		return	FALSE;

	dwFindTreeClock	=	GetLowClock()	-	dwFindTreeClock;

	DWORD*	pdwFoundTri	=	this->GetResultIndex(dwOutputBufferIndex);

	float	fNearestT	=	1.1f;				// �׳� 1.0���� ū��.
	DWORD	i;
	DWORD	dwIndex;
	for( i = 0; i < dwTriFound; i++)
	{
		dwIndex		=	pdwFoundTri[i];
		
		PLANE*	pPlane	=	&(m_Plane.pPlane[m_Triangle.pdwPlaneIndex[dwIndex]]);
		// �ϴ� �� �ﰢ���� ���� �־��� ������ ���������� �Ǻ��Ѵ�.
		float	fDot1	=	DotProduct( pFrom, &(pPlane->v3Up))	+	pPlane->D;
//		if( fDot1 > -0.0001f && fDot1 < 0.0001f)
//			fDot1	=	0.0f;
		float	fDot2	=	DotProduct( pTo, &(pPlane->v3Up))		+	pPlane->D;
//		if( fDot2 > -0.0001f && fDot2 < 0.0001f)
//			fDot2	=	0.0f;
		if( fDot1 * fDot2 >= -0.00001f)
		{
			// �н�.
			continue;
		}

		float	t	=	-1.0f * (DotProduct( &(pPlane->v3Up), pFrom) + pPlane->D) / (DotProduct( &(pPlane->v3Up),  &vVelocity));
#ifdef	_DEBUG
		if( t > 1.0f || t < 0.0f)
//	_asm int 3;					// ���⼭ �ɸ��� fDot ����� ������.
			_asm nop
#endif
		// ������� ��.
		VECTOR3		OnPlane	=	*pFrom + vVelocity * t;

		// �����°�.?
		BOOL	bMeet	=	IsTriMeetVertex( m_Triangle.ppBuff[dwIndex], &OnPlane);
		if( bMeet)
		{
			// t üũ.
			if( fNearestT > t)
			{
				fNearestT	=	t;
			}
			else
				_asm nop
		}

	}

	// ������ �ʾҴٸ� �׳� ����.
	if( fNearestT	==	1.1f)
		return	FALSE;

	// �����ٸ� ���ϰ� �����ϰ� ����.
	*pOutT	=	fNearestT;

	return	TRUE;

}


BOOL CCollisionMeshObjectTree::CollisionTestAAELLIPSOID(VECTOR3 *pOut, VECTOR3 *pOutLastVelocity, BOOL *pbOutLanding, AAELLIPSOID *pEllipsoid, VECTOR3 *pTo, DWORD dwOutputBufferIndex)
{
#ifdef	_DEBUG
	DBG_CheckOutputBufferIndex( dwOutputBufferIndex);
#endif

	BOOL			bFuncResult	=	FALSE;					// ���� ���.
	*pbOutLanding			=	FALSE;

	pOutLastVelocity->x		=	0.0f;
	pOutLastVelocity->y		=	0.0f;
	pOutLastVelocity->z		=	0.0f;

	*pOut	=	*pTo;

	if( pEllipsoid->P == *pTo)
	{
		// ���������� �ȴ±���.
		*pOut	=	*pTo;
		return	bFuncResult;
	}
	
	BOOL	bLanding	=	FALSE;		// ������� �ٴڿ� ��ҳ�.?
	
	float		fRealTime	=	0.0f;		// ���� �ӵ����ͷ� �־��� �ð�.
	// ���� ��� �ͽ�ťƼ�� ���� �������� 30�������� ���, �浹 ó�� �Լ��� 1/30 �ʸ��� �ѹ� ȣ�� �ǰ�,
	// fRealTime�� 1/30 �ʿ��� ���� 1/30�ʱ��� �帥��.
	// �̰��� �ӵ� ���͸� �����ؼ� �ٽ� �����ϱ� ���� ���δ�.

	// �ӵ� ����.?
	VECTOR3		vCurrentVelocity	=	*pTo	-	pEllipsoid->P;
	// �ӵ�.
	float		fCurrentVelocity	=	VECTOR3Length( &vCurrentVelocity);
	if( fCurrentVelocity == 0.0f)
		_asm int 3;		// ������ �ɷ����ߵǴ°� �ƴѰ�.?



	// ���� ��ġ.
	AAELLIPSOID			CurrentEllipsoid	=	*pEllipsoid;
	
	VECTOR3				vPrevVelocity;
	AAELLIPSOID			PrevEllipsoid;
	
	
	
	DWORD		dwLastMeetTriIndex	=	0xffffffff;
	DWORD	dwDebugLoopCount	=	0;
	while(1)
	{
		dwDebugLoopCount++;
		if( dwDebugLoopCount > 10)
			_asm nop;
		if( dwDebugLoopCount > 20)
		{
			// �׳� ����� ����.
			fCurrentVelocity	=	0.0f;
			vCurrentVelocity.x	=	0.0f;
			vCurrentVelocity.y	=	0.0f;
			vCurrentVelocity.z	=	0.0f;
		}

		// ���� �ι�° �����ε��� CurrentEllipsoid�� vCurrentVelocity�� �ٲ��� �ʾҴٸ� �̰� ���� �ִ°���.
		if( fCurrentVelocity == 0.0f)
			break;

		// �ؿ� �������� ������� �ٲ����� �˾Ƴ��� ���� �����صд�.
		vPrevVelocity		=	vCurrentVelocity;
		PrevEllipsoid		=	CurrentEllipsoid;


		// Ʈ���� ���� �ﰢ�� �ܾ�ͼ�,
		BOUNDING_CAPSULE		BC;
		BC.fRadius		=	CurrentEllipsoid.h;
		if( CurrentEllipsoid.h < CurrentEllipsoid.w)
			BC.fRadius		=	CurrentEllipsoid.w+1000.0f;
		BC.v3From		=	CurrentEllipsoid.P;
		BC.v3To			=	CurrentEllipsoid.P + vCurrentVelocity;//*pTo;

		// Ʈ�� Ÿ����,
		DWORD	dwFindTreeClock	=	GetLowClock();
		DWORD	dwFoundTri	=	FindTriangleWithBC(&BC, TRUE, dwOutputBufferIndex);
		if( dwFoundTri > 50)
			_asm nop;			// �ﰢ���� �ʹ� ���� ������.?
		dwFindTreeClock	=	GetLowClock()	-	dwFindTreeClock;

		// Ʈ�� ź ����.
		DWORD*	pFoundTriIndex	=	this->GetResultIndex( dwOutputBufferIndex);


		// �ﰢ�� ������ ���� �浹 ����.
		DWORD	i;
		COLLISION_RESULT		CurrentOut;
		CurrentOut.fMeetTime	=	1.1f;
		VECTOR3*		pTargetTri;
		PLANE*			pTargetPlane;
		float			fDotPlaneVelocity;
		for( i = 0; i < dwFoundTri; i++)
		{
			
			// ���ڿ� ���� Ÿ��ü. �̰� Ÿ��ü+�ӵ��� �ٲܱ�.?
			MOVING_ELLIPSOID			TestingEllipsoid;
			TestingEllipsoid.fHeight		=	CurrentEllipsoid.h;
			TestingEllipsoid.fWidth			=	CurrentEllipsoid.w;
			TestingEllipsoid.From			=	CurrentEllipsoid.P;
			TestingEllipsoid.Velocity		=	vCurrentVelocity;

			// Ÿ�� �ﰢ��.
			pTargetTri		=	m_Triangle.ppBuff[pFoundTriIndex[i]];
			pTargetPlane	=	&(m_Plane.pPlane[m_Triangle.pdwPlaneIndex[pFoundTriIndex[i]]]);

			// Ÿ�� �ﰢ���� ���� �ӵ����Ͱ� �����ϸ� ÷���� �н�.
			fDotPlaneVelocity	=	DotProduct( &(pTargetPlane->v3Up), &vCurrentVelocity);
			if( fDotPlaneVelocity >= 0.0f)
//			if( fDotPlaneVelocity >= -0.001f)
			{
				goto	lb_PassTest;
			}

			// ��������.
			COLLISION_RESULT		Out;		// ����� ����.
			BOOL	bFound;

			DWORD	dwCollTestClock;
			dwCollTestClock	=	GetLowClock();

			bFound	=	CollisionTestMovingEllipsoidMeetTriangle( &Out, &TestingEllipsoid, pTargetTri);
			dwCollTestClock	=	GetLowClock()	-	dwCollTestClock;

			if( bFound)
			{
				// ���� �����ǰ� �׽�Ʈ.
				if( CurrentOut.fMeetTime > Out.fMeetTime)	
				{
					// ���� ���� ���� ������ �������� �ʳ�.?
					float	fMeetPlaneDotVelocity	=	DotProduct( &(Out.MeetPlane.v3Up), &(TestingEllipsoid.Velocity));
					if( fMeetPlaneDotVelocity < 0)
					{
						CurrentOut	=	Out;					// �̳��� ����.
						dwLastMeetTriIndex		=	pFoundTriIndex[i];		// �� �ﰢ���� �ε���.
						bFuncResult				=	TRUE;					// �ϴ� �ѹ��̶� ��������.
					}
					else
					{
						_asm nop;
					}
				}
			}
lb_PassTest:
			_asm nop;

		}
		

		// ���� ����� t? �ӵ� ����.
		if( CurrentOut.fMeetTime != 1.1f)
		{
			if( CurrentOut.dwComponentType == 10)
				_asm nop;
			// bLanding?
			CurrentOut.MeetPlane.v3Up;
			VECTOR3		vUp;
			vUp.x	=	0.0f;
			vUp.y	=	1.0f;
			vUp.z	=	0.0f;
			float	fDotLanding	=	DotProduct( &CurrentOut.MeetPlane.v3Up, &vUp);
			if( fDotLanding > 0.6f)
				bLanding		=	TRUE;
//			else
//				bLanding		=	FALSE;

			// ã���� ������� Ǫ��~
			VECTOR3		NewTo;
			BOOL		bPush;
			bPush	= PushEllipsoidFromPLANE( &NewTo, &CurrentOut,  &(CurrentEllipsoid.P), &vCurrentVelocity);
			CheckVectorArray( &NewTo, 1);
			// ���ο� From�� To.

			CurrentEllipsoid.P		=	CurrentOut.vMeetPivot;
			vCurrentVelocity		=	NewTo	-	CurrentOut.vMeetPivot;
			fCurrentVelocity		=	VECTOR3Length( &vCurrentVelocity);

			fRealTime	=	fRealTime	+	((1.0f - fRealTime) * CurrentOut.fMeetTime);	// �ð��� ��� ó��.

			if( fRealTime < 0.0f)
					fRealTime	=	0.0f;
			if( fRealTime > 1.0f)
					fRealTime	=	1.0f;
			if( fRealTime != 0.0f)
				_asm nop;

			_asm nop;
		}
		else
		{
			break;
		}

		// �浹�̶�� ������ ������ ���Ѱ� ������ �ƹ��͵� �ƴ��� �ʴ°�.?
		// �����صа͵��� �׽�Ʈ �Ѵ�.
		if( PrevEllipsoid.P == CurrentEllipsoid.P)
		{
			if( vPrevVelocity == vCurrentVelocity)
			{
				// �׳� ����� ����.
				fCurrentVelocity	=	0.0f;
				vCurrentVelocity.x	=	0.0f;
				vCurrentVelocity.y	=	0.0f;
				vCurrentVelocity.z	=	0.0f;
				_asm nop;
			}
			else
			{
				_asm nop;
			}


		}

		
		_asm nop;
	}


//lbFinishCollision:
	// �� �浹���� ���ϴ���, ��·�� ���� �ؾ�...
	// ������ �ѹ� ����.
	if( bLanding)
		VECTOR3_MULEQU_FLOAT( &vCurrentVelocity, 0.9f);//m_fFrictionalForce);
	*pOut	=	CurrentEllipsoid.P + vCurrentVelocity;

	float	fTimeScale;					// ���� ������ ���Ϳ� ����Ǵ� �ð���.?

	fTimeScale	=	1.0f	-	fRealTime;

	if( fRealTime < 0.0f)
		_asm nop;							// �̰��� �� ��ġ������ �ʳ�.?
	if( fRealTime == 0.0f)
		_asm nop;							// �׳� �ƹ��͵� �ȸ��� ���.
	if( fRealTime == 1.0f)
		_asm nop;							// �־��� �ð��� �� ä�� ������.?
	if( fRealTime > 1.0f)
		_asm nop;							// �̰� ���İ�... -_-;

	VECTOR3		vRealVelocity	=	vCurrentVelocity;			// �ǽð���.?
	VECTOR3_MULEQU_FLOAT( &vRealVelocity, fTimeScale);

	*pOutLastVelocity	=	vRealVelocity;

	DWORD	pdwProperty[4];
	if( bFuncResult == TRUE)
	{
		m_Triangle.ppObjBuff[dwLastMeetTriIndex]->GetProperty(pdwProperty);				// ����.
	}
	// �ٴڿ� ��ҳ� �׽�Ʈ.
//	�̴�� �ȵ�. �������� true �����ϰ�...
	*pbOutLanding	=	bLanding;



	return	bFuncResult;
}

DWORD*	CCollisionMeshObjectTree::GetResultIndex(DWORD dwOutputBufferIndex)
{
#ifdef	_DEBUG
	DBG_CheckOutputBufferIndex( dwOutputBufferIndex);
#endif
	return	m_pOutputBuffer[dwOutputBufferIndex].pdwOutputIndexBuffer;
}


void CCollisionMeshObjectTree::CreateFindBuffer(DWORD dwCount)
{
//	m_dwMemoryUsage	+=	sizeof(DWORD)*m_Triangle.dwCount;

	if( dwCount == 0 )
	{
		PrintfDebugString( "CCollisionMeshObjectTree::CreateFindBuffer  ���ڰ� 0�̴�.\n");
		_asm int 3;
		dwCount = 1;
	}

	
	m_dwOutputBufferCount		=	dwCount;
	m_pOutputBuffer				=	new	OUTPUT_INDEX_BUFFER[m_dwOutputBufferCount];
	memset( m_pOutputBuffer, 0, sizeof(OUTPUT_INDEX_BUFFER)*m_dwOutputBufferCount);
	DWORD		i;
	for( i = 0; i < m_dwOutputBufferCount; i++)
	{
		m_pOutputBuffer[i].dwOutputIndexCount		=	0;
		m_pOutputBuffer[i].pdwOutputIndexBuffer		=	new	DWORD[m_Triangle.dwCount];
		memset( m_pOutputBuffer[i].pdwOutputIndexBuffer, 0, sizeof(DWORD)*m_Triangle.dwCount);
		m_pOutputBuffer[i].pOutputLink				=	new	OUTPUT_LINK[m_Triangle.dwCount+2];			// +2�� ���� ��ũ�� ����.
		ClearOutputLink(i);
	}

	// �޸� ��뷮.
	m_dwMemoryUsage	+=	sizeof(OUTPUT_INDEX_BUFFER)*m_dwOutputBufferCount;

}

void CCollisionMeshObjectTree::DeleteFindBuffer()
{
	if( m_pOutputBuffer)
	{
		DWORD	i;
		for( i = 0; i < m_dwOutputBufferCount; i++)
		{
			if(m_pOutputBuffer[i].pdwOutputIndexBuffer)
			{
				delete[]	m_pOutputBuffer[i].pdwOutputIndexBuffer;
				m_pOutputBuffer[i].pdwOutputIndexBuffer		=	0;
				m_pOutputBuffer[i].dwOutputIndexCount		=	0;
			}
			if( m_pOutputBuffer[i].pOutputLink)
			{
				delete[]	m_pOutputBuffer[i].pOutputLink;
				m_pOutputBuffer[i].pOutputLink				=	0;
				m_pOutputBuffer[i].dwOutputLinkCurrent		=	0;
			}
		}

		delete[]	m_pOutputBuffer;
		m_pOutputBuffer				=	0;
	}
}

void CCollisionMeshObjectTree::SetSimultaneousAccessCount(DWORD dwCount)
{
	if( dwCount == 0)
	{
		PrintfDebugString( "CCollisionMeshObjectTree::SetSimultaneousAccessCount	dwCount == 0 ?\n");
		_asm int 3;
		return;
	}

	if( m_dwOutputBufferCount != dwCount)
	{
		// ���� ���� ��������.
		DeleteFindBuffer();
//		m_dwOutputBufferCount		=	dwCount;
		CreateFindBuffer(dwCount);
	}
	else
	{
		_asm nop
	}
}

#ifdef	_DEBUG
void CCollisionMeshObjectTree::DBG_CheckOutputBufferIndex( DWORD dwBufferIndex)
{
	if( dwBufferIndex >= m_dwOutputBufferCount)
	{
		PrintfDebugString( "CCollisionMeshObjectTree::DBG_CheckOutputBufferIndex �־��� ���� �ε��� %d �� ���� ������ ���� %d�� �Ѿ���.\n", dwBufferIndex, m_dwOutputBufferCount);
		_asm int 3;
	}
}
#endif

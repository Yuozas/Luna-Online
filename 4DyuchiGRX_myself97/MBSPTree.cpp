// MBSPTree.cpp: implementation of the MBSPTree class.
//
//////////////////////////////////////////////////////////////////////

#include "MBSPTree.h"
#include "myself97common.h"
#include "../4DyuchiGXGFunc/global.h"
#include "CheckClock.h"

BOOL	TempComparePlane( PLANE* pP1, PLANE* pP2);
float	TriangleSize( VECTOR3* pv1, VECTOR3* pv2, VECTOR3* pv3);

BOOL	TestEdge();
VECTOR3		*gpvE1, *gpvE2, *gpvMid, *gpvP;
float		gfMaxDist;
PLANE		*gpPlane;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int MBSPTree::siCount	=	0;

MBSPTree::MBSPTree()
{
	memset( this, 0, sizeof(MBSPTree));
}

MBSPTree::~MBSPTree()
{
	this->DeleteAll();
}

BOOL MBSPTree::Init()
{
	_asm nop;
	this->siCount++ ;

	if( mpNode != 0)
	{
		_asm int 3;
		return FALSE;
	}
	
	this->mpPreTriangle	=	new VECTOR3[10000*3];
	memset( this->mpPreTriangle, 0, sizeof(VECTOR3)*10000*3);
	
	mbBuilt	=	false;

	return TRUE;	
}

DWORD MBSPTree::AddTriangle(VECTOR3* IN pTri)
{
	if( mbBuilt == true)
		_asm int 3;
	if( this->mdwPreTriangleCount >= 10000-1)
	{
		// �ӽ� �ﰢ�� ���� �ʰ�.
		_asm int 3;
	}
	VECTOR3		vTemp;
	int			i;
	this->mpPreTriangle[this->mdwPreTriangleCount*3+0]	=	pTri[0];
	this->mpPreTriangle[this->mdwPreTriangleCount*3+1]	=	pTri[1];
	this->mpPreTriangle[this->mdwPreTriangleCount*3+2]	=	pTri[2];
	
	// �ݿø�.
	for( i = 0; i < 3; i++)
	{
//		vTemp.x	=	Round(pTri[i].x);
//		vTemp.y	=	Round(pTri[i].y);
//		vTemp.z	=	Round(pTri[i].z);
		vTemp	=	pTri[i];
		this->mpPreTriangle[this->mdwPreTriangleCount*3+i]	=	vTemp;
	}

	this->mdwPreTriangleCount++;
	
	return mdwPreTriangleCount-1;		// ���� �ε���(?) ����.	
}

BOOL MBSPTree::BuildTree()
{
	mbBuilt	=	true;

	DWORD		dwBuildTreeClock	=	GetLowClock();
	VECTOR3		vTemp;
	float		fTemp;
	if( mdwPreTriangleCount == 0)
	{
		delete[]	this->mpPreTriangle;
		mpPreTriangle		=	0;
		return FALSE;
	}
	int		i, j;
	BOOL	bFound	=	FALSE;
	// ���� �����ϰ�,
	{
		// �ﰢ�� ����.
		mpTri	=	new VECTOR3[ this->mdwPreTriangleCount*3];					
		memcpy( mpTri, mpPreTriangle, sizeof(VECTOR3)*this->mdwPreTriangleCount*3);
		this->miTriCount	=	mdwPreTriangleCount;
		delete[]	mpPreTriangle;
		mpPreTriangle	=	0;
		this->mdwPreTriangleCount	=	0;

		mpTriSphere		=	new	BOUNDING_SPHERE[this->miTriCount];		// �ﰢ���� �ٿ�� ���Ǿ�.

		for( i = 0; i < this->miTriCount; i++)
		{
			GetTriMiddle( &vTemp, &(mpTri[i*3]));
			mpTriSphere[i].v3Point	=	vTemp;

			fTemp		=	VECTOR3Length(&(mpTriSphere[i].v3Point - this->mpTri[i*3]));
			mpTriSphere[i].fRs	=	fTemp;
		}
		
		// ��� ����.
		PLANE*	pTempPlane	=	new PLANE[miTriCount];		// ���ڰ� �پ�� ���̴� �ӽù��۸� �ϴ�, 
		memset( pTempPlane, 0, sizeof(PLANE)*miTriCount);
		miPlaneCount	=	0;
		float*	pTempPlaneSize		=	new	float[miTriCount];
		memset( pTempPlaneSize, 0, sizeof(float)*miTriCount);
		DWORD*	pTempPlaneIndex	=	new	DWORD[miTriCount];
		memset( pTempPlaneIndex, 0, sizeof(DWORD)*miTriCount);
		
		MakePLANE( &(pTempPlane[0]), &(mpTri[0]), &(mpTri[1]), &(mpTri[2]));	// ù��° ���.

		miPlaneCount = 1;
		pTempPlaneIndex[0]	=	0;
		
		// �ﰢ���� ���ư��鼭, ����� ã�Ƴ���. ��ġ�°� �ε����� ���.
		for( i = 1; i < miTriCount; i++)
		{
			PLANE	p;
			MakePLANE( &p, &(mpTri[i*3]), &(mpTri[i*3+1]), &(mpTri[i*3+2]));
			fTemp	=	VECTOR3Length( &(p.v3Up));
			if( fTemp < 0.1f)
				_asm nop;
			bFound	=	FALSE;
			for( j = 0; j < miPlaneCount; j++)
			{
				if( TempComparePlane( &p, &pTempPlane[j]))
				{
					pTempPlaneIndex[i]	=	j;
					float	fThisTriSize	=	TriangleSize( &(mpTri[i*3]), &(mpTri[i*3+1]), &(mpTri[i*3+2]));		
					pTempPlaneSize[j]	=	pTempPlaneSize[j] + fThisTriSize;			// �� ����� ���Ǵ� ����.
					bFound	=	TRUE;
					_asm nop;
				}
			}
			if( bFound	==	FALSE)		// ���ο� ���.
			{
				pTempPlane[miPlaneCount]	=	p;
				pTempPlaneIndex[i]	=	miPlaneCount;
				pTempPlaneSize[miPlaneCount]	=	TriangleSize( &(mpTri[i*3]), &(mpTri[i*3+1]), &(mpTri[i*3+2]));
				miPlaneCount++;
			}
		}
		
		mpPlane	=	new PLANE[miPlaneCount];					// ��� ����.
		memcpy( mpPlane, pTempPlane, sizeof(PLANE)*miPlaneCount);
		delete[]	pTempPlane;
		for( i = 0; i < this->miPlaneCount; i++)
		{
			mpPlane[i].fAreaSize	=	pTempPlaneSize[i];
		}
		delete[]	pTempPlaneSize;
		mpPlaneIndex	=	new	DWORD[miTriCount];					// �ﰢ���� ��� �ε��� ����.
		memcpy( mpPlaneIndex, pTempPlaneIndex, sizeof(DWORD)*miTriCount);
		delete[]	pTempPlaneIndex;
		
	}
	
	// Ʈ�� �����.
	DWORD*	pTemp	=	new DWORD[this->miTriCount];
	for( i = 0; i < this->miTriCount; i++)
	{
		pTemp[i]	=	i;
	}
	DWORD*	pTempCheckedPlane	=	new DWORD[this->miPlaneCount];
	memset( pTempCheckedPlane, 0, sizeof(DWORD)*this->miPlaneCount);

	// ��尡 �� ����.
	miNodeTemp	=	miPlaneCount*20;		// �ӽ� ����, �̰� �ȸ����� ��¼��.?
	mpNode		=	new BSPTreeNode[miNodeTemp];
	miNodeCount	=	0;

	// Ʈ�� �����, 
	CreateNodeReculsive( -1, 0, pTemp, this->miTriCount, pTempCheckedPlane, 0);

	delete[]	pTempCheckedPlane;
	pTempCheckedPlane	=	0;
	delete[]	pTemp;
	pTemp	=	0;

	// ��� ����.
	BSPTreeNode*		pTempNode	=	0;
	if( miNodeCount != 0)
	{
		pTempNode	=	new	BSPTreeNode[miNodeCount];
		memcpy( pTempNode, mpNode, sizeof(BSPTreeNode)*miNodeCount);
	}
	delete[]	mpNode;
	mpNode	=	pTempNode;

	// �ﰢ�� �ε����� �������ۿ� �о�ִ´�.
	int		iTemp	=	this->miNodeCount * this->miTriCount;
	this->miNodeTriIndexBuffUsedCount	=	0;
	this->mpNodeTriIndexBuff			=	new		int[iTemp];
	for( i = 0; i < this->miNodeCount; i++)
	{
		if( mpNode[i].iTriCount >= 0)
		{
			mpNode[i].iTriStartIndex	=	miNodeTriIndexBuffUsedCount;
			for( j = 0; j < mpNode[i].iTriCount; j++)
			{
				this->mpNodeTriIndexBuff[miNodeTriIndexBuffUsedCount]	=	mpNode[i].r->pTriIndex[j];
				miNodeTriIndexBuffUsedCount++;
			}
			if( mpNode[i].r->pChecked)
			{
				delete[]	mpNode[i].r->pChecked;
				mpNode[i].r->pChecked		=	0;
			}
			if( mpNode[i].r->pPlaneIndex)
			{
				delete[]	mpNode[i].r->pPlaneIndex;
				mpNode[i].r->pPlaneIndex	=	0;
			}
			if( mpNode[i].r->pTriIndex)
			{
				delete[]	mpNode[i].r->pTriIndex;
				mpNode[i].r->pTriIndex		=	0;
			}
		}
	}
	int*	piTemp	=	new	int[miNodeTriIndexBuffUsedCount];
	memcpy( piTemp, mpNodeTriIndexBuff, sizeof(int)*miNodeTriIndexBuffUsedCount);
	delete[]	mpNodeTriIndexBuff;
	mpNodeTriIndexBuff		=	piTemp;



	
	//	ã��� ���� �����.
	this->pOutputBuffer	=	new DWORD[this->miTriCount];
	this->mpTempOutput	=	new BYTE[this->miTriCount];	// ��¿� �ε��� ����.
	
	this->miNormalNodeCount	=	this->miNodeCount	-	this->miLeafCount	-	this->miNullLeafCount;

	if( this->miTriCount <= 0)
		_asm int 3;

	dwBuildTreeClock	=	GetLowClock()	-	dwBuildTreeClock;

	if( mpPreTriangle != 0)
		_asm int 3;
	if( mdwPreTriangleCount != 0)
		_asm int 3;

	return TRUE;	
}

// bBack�� true�̸� ��, false�̸� ��. -_-;
int MBSPTree::CreateNodeReculsive( int iParent, BOOL bIsBack, DWORD* pTriIndex, DWORD dwTriCount, DWORD* pCheckedPlane, int iLevel)
{
	if( mpPreTriangle != 0)
		_asm int 3;
	if( mdwPreTriangleCount != 0)
		_asm int 3;


	if( iLevel > miMaxLevel)	miMaxLevel	=	iLevel;
	int	i;//, j;
	int		iResult;

	if( dwTriCount != 0 && pCheckedPlane == 0)
	{
		_asm nop;
	}

	// �ϴ� ��� �����.
	if( this->miNodeCount >= this->miNodeTemp)
	{
		_asm int 3;
//		return -1;   //ū�ϳ���.
	}

	BSPTreeNode*	pNode	=	&(mpNode[this->miNodeCount]);
	int				iNode	=	miNodeCount;
	memset( pNode, 0, sizeof(BSPTreeNode));
	pNode->iBack	=	-1;
	pNode->iFront	=	-1;
	this->miNodeCount++;
	pNode->r		=	new	BSPTreeNodeRuntime;
	memset( pNode->r, 0, sizeof(BSPTreeNodeRuntime));
	
	pNode->iParent		=	iParent;
	pNode->iLevel		=	iLevel;
	
	if( dwTriCount == 0)	// NULL leaf.
	{
		//		_asm int 3;
		pNode->bIsLeaf	=	TRUE;
		this->miNullLeafCount++;
		return iNode;
	}
	
	// �ﰢ�� ����, ��������� �� ������ ������ �ƴϸ� ��������.
	pNode->r->pTriIndex	=	new DWORD[ dwTriCount];		
	memcpy( pNode->r->pTriIndex, pTriIndex, sizeof(DWORD)*dwTriCount);
	pNode->iTriCount	=	dwTriCount;
	// ��� üũ ����.
	pNode->r->pChecked		=	new	DWORD[ miPlaneCount];
	memcpy( pNode->r->pChecked, pCheckedPlane, sizeof(DWORD)*miPlaneCount);
	
	// �θ𿡰� ���� �ﰢ�� ���տ��� ���� ������(?)�� �ϳ��� ��󳽴�.
	int		iTemp	=	FindDivideCandidate( pNode->r->pTriIndex, pNode->iTriCount, pNode->r->pChecked);
	
	// ���� ���� ã��.
	if( iTemp == -1) // ���̻� ����� �������� ������, 
	{
		pNode->bIsLeaf	=	TRUE;
		this->miLeafCount++;
		return	iNode;
	}
	if( iTemp > this->miTriCount)
		_asm int 3;
	
	
	
	
	pNode->iDivPlane		=	this->mpPlaneIndex[pNode->r->pTriIndex[iTemp]];
	pNode->r->pChecked[pNode->iDivPlane]	=	1;	// üũ.
	
	
	// �ﰢ���� ��鿡 ���� �з��Ҷ� ���� ����.
	DWORD*	pBack	=	new DWORD[dwTriCount];
	DWORD	dwBack	=	0;
	
	DWORD*	pFront	=	new DWORD[dwTriCount];
	DWORD	dwFront	=	0;
	
	// ������� �߽����� ����� �ﰢ���� ��, �ڷ� ���,
	{
		for( i = 0; i < pNode->iTriCount; i++)
		{
			iResult	=	WhereIsTriangleFromPlane( &(this->mpTri[(pNode->r->pTriIndex[i])*3]), &(this->mpPlane[pNode->iDivPlane]));
			switch( iResult)
			{
			case	-2:		// ����ϰ� �ڿ� �ְų�,
			case	-1:		// ����� ��ġ���� �븻�� �ݴ��� ���.
				pBack[dwBack]	=	pNode->r->pTriIndex[i];
				dwBack++;
				
				break;
			case	0:		// ���� �߽����� �ﰢ���� �������ִ� ���.
				pBack[dwBack]	=	pNode->r->pTriIndex[i];
				dwBack++;
				pFront[dwFront]	=	pNode->r->pTriIndex[i];
				dwFront++;
				break;
			case	1:		// ����� ��ġ�� �븻�� ���� �����ΰ��.
			case	2:		// ����ϰ� �տ� �ְų�.
				pFront[dwFront]	=	pNode->r->pTriIndex[i];
				dwFront++;
				break;
			default:
				_asm int 3;
				break;
			}// switch
		}// for
	}
	
	
	// �ڽ� ���.
	{
		// �� �ڽĵ� ���.
		if( dwFront != 0)		// �ּ��� Ǯ�� �� ���� �����ٴ� ���̴�.
		{
			pNode->iFront	=	this->CreateNodeReculsive( iNode, FALSE, pFront, dwFront, pNode->r->pChecked, pNode->iLevel+1);
		}
		
		// �� �ڽĵ� ���.
		if( dwBack != 0)		// �ּ��� Ǯ�� �� ���� �����ٴ� ���̴�.
		{
			pNode->iBack	=	this->CreateNodeReculsive( iNode, TRUE, pBack, dwBack, pNode->r->pChecked, pNode->iLevel+1);
		}
	}
	
	// ���� ����.
	delete[]	pBack;
	pBack	=	0;
	delete[]	pFront;
	pFront	=	0;
	
	delete[]	pNode->r->pTriIndex;		// ��尡 ������ �ִ� �ﰢ�� ����Ʈ�� ������.
	pNode->r->pTriIndex	=	0;
	pNode->iTriCount	=	0;
	delete[]	pNode->r->pChecked;
	pNode->r->pChecked		=	0;
	
	return	iNode;
}


// �־��� �ﰢ���� ����Ű�� ����� bsp�������� ���� ������ ����� ã�´�.
//	���ϰ��� �־��� �������� �ﰢ�� �ε���.
int MBSPTree::FindDivideCandidate(DWORD* pTriIndex, DWORD dwTriCount, DWORD* pCheckedPlane)
{
	if( mpPreTriangle != 0)
		_asm int 3;
	if( mdwPreTriangleCount != 0)
		_asm int 3;


	VECTOR3		vTemp;
	float		fTemp;
	if( dwTriCount == 0 || pTriIndex == 0 || pCheckedPlane == 0)		// �̷� ��Ȳ�� ���� �ȵȴ�.
	{
		_asm int 3;		
		return -1;
	}
	
	DWORD	i;// , j;
	DWORD	dwPlaneIndex;

	DWORD	dwCandidate	=	0xffffffff;		//  �ĺ� �ﰢ��.
	
	BOOL	bFound	=	FALSE;					// ���� üũ�Ǿ��ִ����̸� �� �÷��� for���� �������͵� false�̴�.
	float		fTopSize	=	0;
	
	// x,z��� ����ε� ��� �켱.
	for( i = 0; i < dwTriCount; i++)
	{
		dwPlaneIndex	=	this->mpPlaneIndex[pTriIndex[i]];
		if( pCheckedPlane[dwPlaneIndex] == 1)	continue;
		if( mpPlane[dwPlaneIndex].v3Up.y >=	0.999f)
		{
			bFound	=	TRUE;
			if( mpPlane[dwPlaneIndex].fAreaSize > fTopSize)
			{
				dwCandidate	=	i;
				fTopSize	=	mpPlane[dwPlaneIndex].fAreaSize;
			}
		}
	}
	if( bFound)
		goto	lbReturn;
	// y,z��鿡 ����ε� ��� �켱.
	fTopSize	=	0;
	for( i = 0; i < dwTriCount; i++)
	{
		dwPlaneIndex	=	this->mpPlaneIndex[pTriIndex[i]];
		if( pCheckedPlane[dwPlaneIndex] == 1)	continue;
		if( mpPlane[dwPlaneIndex].v3Up.x >=	0.999f)
		{
			bFound	=	TRUE;
			if( mpPlane[dwPlaneIndex].fAreaSize > fTopSize)
			{
				dwCandidate	=	i;
				fTopSize	=	mpPlane[dwPlaneIndex].fAreaSize;
			}
		}
	}
	if( bFound)
		goto	lbReturn;

	// x, y��鿡 ����ε� ��.
	fTopSize	=	0;
	for( i = 0; i < dwTriCount; i++)
	{
		dwPlaneIndex	=	this->mpPlaneIndex[pTriIndex[i]];
		if( pCheckedPlane[dwPlaneIndex] == 1)	continue;
		if( mpPlane[dwPlaneIndex].v3Up.z >=	0.999f)
		{
			bFound	=	TRUE;
			if( mpPlane[dwPlaneIndex].fAreaSize > fTopSize)
			{
				dwCandidate	=	i;
				fTopSize	=	mpPlane[dwPlaneIndex].fAreaSize;
			}
		}
	}
	if( bFound)
		goto	lbReturn;
	// �� �� y��� ������ ��� 3����.
	fTopSize	=	0;
	for( i = 0; i < dwTriCount; i++)
	{
		dwPlaneIndex	=	this->mpPlaneIndex[pTriIndex[i]];
		if( pCheckedPlane[dwPlaneIndex] == 1)	continue;
		vTemp.x		=	0.0f;
		vTemp.y		=	1.0f;
		vTemp.z		=	0.0f;
		fTemp	=	vTemp * mpPlane[dwPlaneIndex].v3Up;
//		if( pPlane[dwPlaneIndex].v3Up.z >=	0.999f)
		if( fTemp >= -0.00001f && fTemp <= 0.00001f)
		{
			bFound	=	TRUE;
			if( mpPlane[dwPlaneIndex].fAreaSize > fTopSize)
			{
				dwCandidate	=	i;
				fTopSize	=	mpPlane[dwPlaneIndex].fAreaSize;
			}
		}
	}
	if( bFound)
		goto	lbReturn;

	// �׿� �ﰢ���� ������ ���� ���� ����� �ĺ���.
	bFound	=	FALSE;
	fTopSize	=	0;
	for( i = 0; i < dwTriCount; i++)
	{
		dwPlaneIndex	=	this->mpPlaneIndex[pTriIndex[i]];
		if( pCheckedPlane[dwPlaneIndex] == 1)	continue;
		bFound	=	TRUE;
//		if( this->pPlaneSize[dwPlaneIndex] > dwTopSize)
		if( mpPlane[dwPlaneIndex].fAreaSize > fTopSize)
		{
			dwCandidate	=	i;
			fTopSize	=	mpPlane[dwPlaneIndex].fAreaSize;
		}
	}


lbReturn:
	if( bFound == TRUE)		return		dwCandidate;

	// �̱��� ���� �ﰢ������ �� üũ�Ǿ��ִ� == �����̴�.
	return -1;
}



//	 �־��� �ﰢ���� ����� ��� �ִ°�.?
//	����� �տ� ������ 2, ����� �ڿ� ������ -2, ����� �����ִµ� �븻�� ������� 1, �븻�� �ݴ��� ��� -1, 
//	�ΰ��� ���� ������ ������ 0 ����.
int MBSPTree::WhereIsTriangleFromPlane(VECTOR3 *pTri, PLANE *pPlane)
{
	if( mpPreTriangle != 0)
		_asm int 3;
	if( mdwPreTriangleCount != 0)
		_asm int 3;

	DWORD	i;
	VECTOR3		Normal, vec1, vec2;
	
	DWORD	dwFront = 0;		// ����� �տ� �մ� �� ����.
	DWORD	dwBack = 0;			// �ڿ� �ִ� �� ����.
	DWORD	dwOn = 0;			// ��� ���� �ִ� �� ����.
	
	float	d;
	for( i = 0; i < 3; i++)
	{
		d	=	pPlane->v3Up.x*pTri[i].x + pPlane->v3Up.y*pTri[i].y + pPlane->v3Up.z*pTri[i].z + pPlane->D;
		if( d > 0.5f )			dwFront++;
		else if( d < -0.5f )	dwBack++;
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

DWORD MBSPTree::FindNearTri(DWORD *pBuff, DWORD dwBuffSize, VECTOR3* pSPivot, float fSRadius)
{
	if( mpPreTriangle != 0)
		_asm int 3;
	if( mdwPreTriangleCount != 0)
		_asm int 3;
	
	float	fTemp, fTemp2;
	if( this->miTriCount == 0)
	{
		return	0;
	}

	int		i;
	this->pSPivot	=	pSPivot;
	this->fSRadius	=	fSRadius;
	memset( this->mpTempOutput, 0, sizeof(BYTE)*this->miTriCount);
	DWORD	dwFound	=	0;
	DWORD	dwPreFound	=	0;
	DWORD	dwTempLoop = 0;

	// Ʈ�� Ÿ��, 
	DWORD	a = GetLowClock();
	this->FindNearTriReculsive( 0);
	a = GetLowClock() - a;

	DWORD	b	=	GetLowClock();

	DWORD	c;
	// ���� ���ư��鼭 üũ.
	for( i = 0; i < this->miTriCount; i++)
	{
		if( this->mpTempOutput[i] == 1)
		{
			dwTempLoop++;
			// ������ �ﰢ���� ���Ǿ� �ȿ� ������ �˻��ؾ� �Ѵ�.
 			c	=	GetLowClock();
//			if( FALSE	== this->IsTooFarBetweenVertexAndTri( fSRadius, i, pSPivot))    // <= ���� ���� ���ϸ� ���� �Դ´�. ������ �ٲ���ұ�.?

//			vTemp	=	mpTriSphere[i].v3Point - *pSPivot;
//			fTemp	=	vTemp * vTemp;
//			fTemp2	=	(mpTriSphere[i].fRs+1.0f)*(mpTriSphere[i].fRs+1.0f)	+	(fSRadius+1.0f)*(fSRadius+1.0f);

			fTemp	=	(mpTriSphere[i].v3Point - *pSPivot)*(mpTriSphere[i].v3Point - *pSPivot);
			fTemp2	=	(mpTriSphere[i].fRs + fSRadius) * (mpTriSphere[i].fRs + fSRadius);

			if( fTemp <= fTemp2) 
			{
				dwPreFound++;
				if( FALSE	== this->IsTooFarBetweenVertexAndTri( fSRadius, i, pSPivot))    // <= ���� ���� ���ϸ� ���� �Դ´�. ������ �ٲ���ұ�.?
				{
					this->pOutputBuffer[dwFound]		=	i;	// �ﰢ���� �ε����� ����. �ܺο��� �ﰢ�� ���ۿ� ���� �����ϰ� �ϴ°� �޸� �� �Ƴ���.
					++dwFound;
				}
			}
			c = GetLowClock() - c;
			_asm nop;
		}
	}
	b	=	GetLowClock() - b;
	_asm nop;
	if( dwFound > dwBuffSize)
	{
		_asm nop;
		dwFound	=	dwBuffSize;
	}
	DWORD	d	=	GetLowClock();
	memcpy( pBuff, this->pOutputBuffer, sizeof(DWORD)*dwFound);
	d		=	GetLowClock()	-	d;
	return	dwFound;
}

void MBSPTree::FindNearTriReculsive(int iNode )
{
	if( mpPreTriangle != 0)
		_asm int 3;
	if( mdwPreTriangleCount != 0)
		_asm int 3;

	mdwDBG_NodeTravelCount	=	0;
//	float	fTemp;
	int				iCurrentNode	=	iNode;
	BSPTreeNode*	pCurrentNode	=	&(mpNode[iNode]);
//	float	fWhere;
	PLANE*	pP;

lb_Start:
	mdwDBG_NodeTravelCount++;
	if( pCurrentNode->bIsLeaf == TRUE)		// �� ��尡 �������,
	{
		int i; 
//		for( i = 0; i < pCurrentNode->iTriCount; i++)
		{
			// ������ �ִ� �ﰢ�� ����Ʈ�� �� �ʾ��.
//			this->pTempOutput[pCurrentNode->r->pTriIndex[i]]	=	1;
		}
		for( i = pCurrentNode->iTriStartIndex; i < pCurrentNode->iTriCount + pCurrentNode->iTriStartIndex; i++)
		{
			int		iIndex				=	this->mpNodeTriIndexBuff[i];
			this->mpTempOutput[iIndex]	=	1;
		}
		// �׳� ����.?
		goto lb_Return;
	}
	
	// �Ϲ� ���, �ϴ� ����� ��� ���� ������ �ִ��� �Ǻ�.
	pP	=	&(mpPlane[pCurrentNode->iDivPlane]);
	pCurrentNode->r->fWhere	=	pP->v3Up.x*pSPivot->x + pP->v3Up.y*pSPivot->y + pP->v3Up.z*pSPivot->z + pP->D;	// ���� ������.
//	pCurrentNode->fWhere	=	DotProduct( &(pP->v3Up), pSPivot) + pP->D;			// 2����.
//	pCurrentNode->fWhere	=	(pP->v3Up)*(*pSPivot) + pP->D;			 // ���� ������.

	// �� ��� ���.
	if( pCurrentNode->r->fWhere >= -fSRadius && pCurrentNode->iFront != -1)
	{

//		pCurrentNode	=	pCurrentNode->pFront;
		iCurrentNode	=	pCurrentNode->iFront;
		pCurrentNode	=	&(mpNode[iCurrentNode]);
		
		goto	lb_Start;
	}
lb_FrontNodeReculsived:
	// �� ��� ���.
	if( pCurrentNode->r->fWhere <= fSRadius && pCurrentNode->iBack != -1)
	{
		iCurrentNode	=	pCurrentNode->iBack;
		pCurrentNode	=	&(mpNode[iCurrentNode]);
//		pCurrentNode	=	pCurrentNode->pBack;
		goto	lb_Start;
	}
lb_BackNodeReculsived:

lb_Return:
	if( pCurrentNode	==	mpNode)
		return ;
//	if( pCurrentNode->pParent->pFront	==	pCurrentNode)
	if( mpNode[pCurrentNode->iParent].iFront == iCurrentNode)
	{
		iCurrentNode	=	pCurrentNode->iParent;
		pCurrentNode	=	&(mpNode[iCurrentNode]);
//		pCurrentNode	=	pCurrentNode->pParent;
		goto lb_FrontNodeReculsived;
	}
//	if( pCurrentNode->pParent->pBack	==	pCurrentNode)
	if( mpNode[pCurrentNode->iParent].iBack == iCurrentNode)
	{
		iCurrentNode	=	pCurrentNode->iParent;
		pCurrentNode	=	&(mpNode[iCurrentNode]);
//		pCurrentNode	=	pCurrentNode->pParent;
		goto	lb_BackNodeReculsived;
	}

_asm nop;
/*
	DWORD i; 
	if( pNode->bIsLeaf == TRUE)		// �� ��尡 �������,
	{
		for( i = 0; i < pNode->dwTriCount; i++)
		{
			// ������ �ִ� �ﰢ�� ����Ʈ�� �� �ʾ��.
			this->pTempOutput[pNode->pTriIndex[i]]	=	1;
		}
		// �׳� ����.?
		return ;
	}
	
	// �Ϲ� ���, �ϴ� ����� ��� ���� ������ �ִ��� �Ǻ�.
	const	PLANE&	p	=	pPlane[pNode->dwDivPlane];
	float	fWhere	=	p.v3Up.x*pSPivot->x + p.v3Up.y*pSPivot->y + p.v3Up.z*pSPivot->z + p.D;

	// �� ��� ���.
	if( fWhere >= -fSRadius)
	{
		FindNearTriReculsive( pNode->pFront );
	}
	
	// �� ��� ���.
	if( fWhere <= fSRadius)
	{
		FindNearTriReculsive( pNode->pBack );
	}
*/
}


void MBSPTree::DeleteAll()
{
	if( mpPreTriangle != 0)
		_asm int 3;
	if( mdwPreTriangleCount != 0)
		_asm int 3;
	
	// ��� ����.
	if( mpPlane)		delete[]	 mpPlane;
	mpPlane			=	0;
	miPlaneCount	=	0;

	// �ﰢ�� ����.
	if( mpTri)		delete[]	mpTri;
	mpTri			=	0;
	miTriCount		=	0;
	if( mpPlaneIndex)	delete[]	mpPlaneIndex;
	mpPlaneIndex		=	0;
	mpPlaneIndex		=	0;
	
	// �ٿ�� ���Ǿ� ����.
	if( mpTriSphere)	delete[]	mpTriSphere;
	mpTriSphere		=	0;

	// ��¿� ����.
	if( mpTempOutput)	delete[]	mpTempOutput;
	mpTempOutput		=	0;
	
	// �ӽù���.
	if( mpPreTriangle)
	{
		_asm int 3;	// ������ ��������.
		delete[]	mpPreTriangle;
		mpPreTriangle	=	0;
	}
	mdwPreTriangleCount	=	0;
	
	if( mpNodeTriIndexBuff)			delete[]	mpNodeTriIndexBuff;

	// Ʈ��.
	if( mpNode)
	{
		for( int i = 0; i < miNodeCount; i++)
		{
			if( mpNode[i].r->pChecked)			delete[]	mpNode[i].r->pChecked;
			if( mpNode[i].r->pPlaneIndex)		delete[]	mpNode[i].r->pPlaneIndex;
			if( mpNode[i].r->pTriIndex)		delete[]	mpNode[i].r->pTriIndex;
			delete		mpNode[i].r;			
		}
		delete[]	mpNode;
		mpNode	=	0;
	}

	

	if( pOutputBuffer)	delete[]	pOutputBuffer;
	pOutputBuffer	=	0;
	
	memset( this, 0, sizeof(MBSPTree));
}


BOOL	TempComparePlane( PLANE* pP1, PLANE* pP2)
{
	float	fTemp;

	// �븻 ��.
	VECTOR3		n1, n2, n;
	Normalize( &n1, &(pP1->v3Up));
	Normalize( &n2, &(pP2->v3Up));
	// ����(?) ������.?
	VECTOR3_SUB_VECTOR3( &n, &n1, &n2);
	fTemp	=	VECTOR3Length( &n);
	if( fTemp	> 0.001f)
		return	FALSE;

	// d�� ���� ������.?
	fTemp	=	pP1->D - pP2->D;
	if( fTemp > 0.5f || fTemp <= -0.5f)
		return	FALSE;

	//	������� ���� ����.
	return	TRUE;
}

// �ʹ� �ָ� TRUE, �Ÿ� �ȿ� ������ FALSE
BOOL	MBSPTree::IsTooFarBetweenVertexAndTri( float fMaxDist, DWORD dwTriIndex, VECTOR3* pVertex)
{
	VECTOR3		vTemp;
	float	fTemp;
	PLANE&		Plane	=	this->mpPlane[this->mpPlaneIndex[dwTriIndex]];
	VECTOR3*	pTri	=	&(this->mpTri[dwTriIndex*3]);
	VECTOR3&	v		=	*pVertex;
	float		fDist;
	fDist	=	DotProduct( &(Plane.v3Up), &v) + Plane.D;


	if( fDist < 0 || fDist > fMaxDist)
	{
		goto lbReturnTRUE;
	}

	VECTOR3		Mid;
	GetTriMiddle( &Mid, pTri);
	VECTOR3		A, B;
	VECTOR3		cross;
	VECTOR3		OnLine;
	float		fOnLineT;
	// ù��° ���� �׽�Ʈ.
//	A	=	pTri[1] - pTri[0];
	VECTOR3_SUB_VECTOR3( &A, &(pTri[1]), &(pTri[0]));
//	B	=	(*pVertex) - pTri[0];
	VECTOR3_SUB_VECTOR3( &B, pVertex, &(pTri[0]));
	CrossProduct( &cross, &A, &B);
	Normalize( &cross, &cross);
//	fTemp	=	cross*Plane.v3Up;
	fTemp	=	DotProduct( &cross, &(Plane.v3Up));
	if( fTemp <= 0)   // ������ �������.
	{
		// ���� ������ ���� ������� �׽�Ʈ.
		FindNearestVertexOnLine( &OnLine, &fOnLineT, pTri[0], pTri[1], v);
		if( fOnLineT > 1)
		{
//			vTemp	=	pTri[1]-v;
			VECTOR3_SUB_VECTOR3( &vTemp, &(pTri[1]), &v);
			fTemp	=	VECTOR3Length(&vTemp);
			if( fTemp > fMaxDist)
				goto	lbReturnTRUE;
			else
				goto	lbReturnFALSE;
		}
		if( fOnLineT < 0)
		{
//			vTemp	=	pTri[0]-v;
			VECTOR3_SUB_VECTOR3( &vTemp, &(pTri[0]), &v);
			fTemp	=	VECTOR3Length(&vTemp);
			if( fTemp > fMaxDist)
				goto	lbReturnTRUE;
			else
				goto	lbReturnFALSE;
		}
		if( fOnLineT <= 1 && fOnLineT >= 0)
		{
//			vTemp	=	OnLine-v;
			VECTOR3_SUB_VECTOR3( &vTemp, &OnLine, &v);
			fTemp	=	VECTOR3Length( &vTemp);
			if( fTemp > fMaxDist)	
				goto	lbReturnTRUE;
			else
				goto	lbReturnFALSE;
		}
	}
	// �ι�° ���� �׽�Ʈ.
//	A	=	pTri[2] - pTri[1];
	VECTOR3_SUB_VECTOR3( &A, &(pTri[2]), &(pTri[1]));
//	B	=	(*pVertex) - pTri[1];
	VECTOR3_SUB_VECTOR3( &B, pVertex, &(pTri[1]));
	CrossProduct( &cross, &A, &B);
	Normalize( &cross, &cross);
//	fTemp	=	cross*Plane.v3Up;
	fTemp	=	DotProduct( &cross, &(Plane.v3Up));
	if( fTemp <= 0)   // ������ �������.
	{
		// ���� ������ ���� ������� �׽�Ʈ.
		FindNearestVertexOnLine( &OnLine, &fOnLineT, pTri[1], pTri[2], v);
		if( fOnLineT > 1)
		{
//			vTemp	=	pTri[2]-v;
			VECTOR3_SUB_VECTOR3( &vTemp, &(pTri[2]), &v);
			fTemp	=	VECTOR3Length(&vTemp);
			if( fTemp > fMaxDist)
				goto	lbReturnTRUE;
			else
				goto	lbReturnFALSE;
		}
		if( fOnLineT < 0)
		{
//			vTemp	=	pTri[1]-v;
			VECTOR3_SUB_VECTOR3( &vTemp, &(pTri[1]), &v);
			fTemp	=	VECTOR3Length(&vTemp);
			if( fTemp > fMaxDist)
				goto	lbReturnTRUE;
			else
				goto	lbReturnFALSE;
		}
		if( fOnLineT <= 1 && fOnLineT >= 0)
		{
//			vTemp	=	OnLine-v;
			VECTOR3_SUB_VECTOR3( &vTemp, &OnLine, &v);
			fTemp	=	VECTOR3Length( &vTemp);
			if( fTemp > fMaxDist)	
				goto	lbReturnTRUE;
			else
				goto	lbReturnFALSE;
		}
	}
	// ����° ���� �׽�Ʈ.
//	A	=	pTri[0] - pTri[2];
	VECTOR3_SUB_VECTOR3( &A, &(pTri[0]), &(pTri[2]));
//	B	=	(*pVertex) - pTri[2];
	VECTOR3_SUB_VECTOR3( &B, pVertex, &(pTri[2]));
	CrossProduct( &cross, &A, &B);
	Normalize( &cross, &cross);
//	fTemp	=	cross*Plane.v3Up;
	fTemp	=	DotProduct( &cross, &(Plane.v3Up));
	if( fTemp <= 0)   // ������ �������.
	{
		// ���� ������ ���� ������� �׽�Ʈ.
		FindNearestVertexOnLine( &OnLine, &fOnLineT, pTri[2], pTri[0], v);
		if( fOnLineT > 1)
		{
//			vTemp	=	pTri[0]-v;
			VECTOR3_SUB_VECTOR3( &vTemp, &(pTri[0]), &v);
			fTemp	=	VECTOR3Length(&vTemp);
			if( fTemp > fMaxDist)
				goto	lbReturnTRUE;
			else
				goto	lbReturnFALSE;
		}
		if( fOnLineT < 0)
		{
//			vTemp	=	pTri[2]-v;
			VECTOR3_SUB_VECTOR3( &vTemp, &(pTri[2]), &v);
			fTemp	=	VECTOR3Length(&vTemp);
			if( fTemp > fMaxDist)
				goto	lbReturnTRUE;
			else
				goto	lbReturnFALSE;
		}
		if( fOnLineT <= 1 && fOnLineT >= 0)
		{
//			vTemp	=	OnLine-v;
			VECTOR3_SUB_VECTOR3( &vTemp, &OnLine, &v);
			fTemp	=	VECTOR3Length( &vTemp);
			if( fTemp > fMaxDist)	
				goto	lbReturnTRUE;
			else
				goto	lbReturnFALSE;
		}
	}





lbReturnFALSE:
	return		FALSE;
lbReturnTRUE:
	return		TRUE;

}

BOOL	TestEdge()
{
	VECTOR3		A, B, cross, OnLine, vTemp;
	float		fTemp, fOnLineT;
//	A	=	pTri[1] - pTri[0];
	VECTOR3_SUB_VECTOR3( &A, gpvE2, gpvE1);
//	B	=	(*pVertex) - pTri[0];
	VECTOR3_SUB_VECTOR3( &B, gpvP, gpvE1);
	CrossProduct( &cross, &A, &B);
	Normalize( &cross, &cross);
//	fTemp	=	cross*Plane.v3Up;
	fTemp	=	DotProduct( &cross, &(gpPlane->v3Up));
	if( fTemp <= 0)   // ������ �������.
	{
		// ���� ������ ���� ������� �׽�Ʈ.
		FindNearestVertexOnLine( &OnLine, &fOnLineT, (*gpvE1), (*gpvE2), (*gpvP));
		if( fOnLineT > 1)
		{
//			vTemp	=	pTri[1]-v;
			VECTOR3_SUB_VECTOR3( &vTemp, gpvE2, gpvP);
			fTemp	=	VECTOR3Length(&vTemp);
			if( fTemp > gfMaxDist)
				goto	lbReturnTRUE;
			else
				goto	lbReturnFALSE;
		}
		if( fOnLineT < 0)
		{
//			vTemp	=	pTri[0]-v;
			VECTOR3_SUB_VECTOR3( &vTemp, gpvE1, gpvP);
			fTemp	=	VECTOR3Length(&vTemp);
			if( fTemp > gfMaxDist)
				goto	lbReturnTRUE;
			else
				goto	lbReturnFALSE;
		}
		if( fOnLineT <= 1 && fOnLineT >= 0)
		{
//			vTemp	=	OnLine-v;
			VECTOR3_SUB_VECTOR3( &vTemp, &OnLine, gpvP);
			fTemp	=	VECTOR3Length( &vTemp);
			if( fTemp > gfMaxDist)	
				goto	lbReturnTRUE;
			else
				goto	lbReturnFALSE;
		}
	}
lbReturnFALSE:
	return		FALSE;
lbReturnTRUE:
	return		TRUE;

}



float	TriangleSize( VECTOR3* pv1, VECTOR3* pv2, VECTOR3* pv3)
{
	VECTOR3	p;
	float	t;
	FindNearestVertexOnLine( &p, &t, *pv2, *pv3, *pv1);
	float	fWidth		=	VECTOR3Length(&(*pv2 - *pv3));
	float	fHeight		=	VECTOR3Length(&(*pv1 - p));
	float	fSize		=	fWidth * fHeight * 0.5f;
	return	fSize;
}

/****************************************************************************
	Ʈ�� ����.
****************************************************************************/
bool MBSPTree::Save(HANDLE hFile)
{
	bool		bResult;
	DWORD		dwTemp	=	0;
	DWORD		dwSize	=	0;

	// ��� ����ϱ�.
	MBSPTreeFileHeader		header;
	{
		// ����.
		header.iVer			=	iBSPVer;
		// �ﰢ�� ����.
		header.iTriCount	=	this->miTriCount;
		// ��� ����.
		header.iPlaneCount	=	this->miPlaneCount;
		// ��� ����.
		header.iNodeCount			=	this->miNodeCount;
		header.iNormalNodeCount		=	this->miNormalNodeCount;
		header.iLeafCount			=	this->miLeafCount;	// ���� ����.
		header.iNullLeafCount		=	this->miNullLeafCount;	// �ﰢ���� �ƹ��͵� ������ ���� ���� ī��Ʈ.
		header.iMaxLevel			=	this->miMaxLevel;
	}
	bResult		=	WriteDataToFile( hFile, &header, sizeof(header));
	
	
	// �ﰢ�� ���.
	{
		// �ﰢ��,
		dwSize		=	sizeof(VECTOR3)*3*miTriCount;
		bResult		=	WriteDataToFile( hFile, mpTri, dwSize);
		// �ٿ�� ���Ǿ�,
		dwSize		=	sizeof( BOUNDING_SPHERE)*miTriCount;
		bResult		=	WriteDataToFile( hFile, mpTriSphere, dwSize);
		// ��� �ε���.
		dwSize		=	sizeof( DWORD)*miTriCount;
		bResult		=	WriteDataToFile( hFile, mpPlaneIndex, dwSize);
	}

	// ��� ���.
	{
		dwSize		=	sizeof(PLANE)*this->miPlaneCount;
		bResult		=	WriteDataToFile( hFile, mpPlane, dwSize);
	}

	// bsp ����.
	{
		dwSize		=	sizeof(BSPTreeNode)*this->miNodeCount;
		bResult		=	WriteDataToFile( hFile, mpNode, dwSize);
	}
	
	// ��尡 ������ �ﰢ���� ����.
	{
		// ���� ũ��.
		dwSize		=	sizeof(int);
		bResult		=	WriteDataToFile( hFile, &miNodeTriIndexBuffUsedCount, dwSize);

		// ����.
		dwSize		=	sizeof(int)*miNodeTriIndexBuffUsedCount;
		bResult		=	WriteDataToFile( hFile, mpNodeTriIndexBuff, dwSize);
	}

	return		true;

}


bool	MBSPTree::Load(HANDLE hFile)
{
	bool		bResult	=	false;
	DWORD		dwTemp	=	0;
	DWORD		dwSize	=	0;

	// ��� �о����.
	MBSPTreeFileHeader		header;
	bResult		=	ReadDataFromFile( hFile, &header, sizeof(header));


	if( header.iVer != iBSPVer)
	{
		_asm int 3;
		return	false;
	}

	miTriCount				=	header.iTriCount;
	miPlaneCount			=	header.iPlaneCount;
	miNodeCount				=	header.iNodeCount;
	miNormalNodeCount		=	header.iNormalNodeCount;
	miLeafCount				=	header.iLeafCount;
	miNullLeafCount			=	header.iNullLeafCount;
	miMaxLevel				=	header.iMaxLevel;

	// �ﰢ�� �б�.
	{
		// �ﰢ��,
		mpTri		=	new VECTOR3[3*miTriCount];
		bResult		=	ReadDataFromFile( hFile, mpTri, sizeof(VECTOR3)*3*miTriCount);

		// �ٿ�� ���Ǿ�,
		mpTriSphere	=	new	BOUNDING_SPHERE[miTriCount];
		bResult		=	ReadDataFromFile( hFile, mpTriSphere, sizeof(BOUNDING_SPHERE)*miTriCount);

		// ��� �ε���.
		mpPlaneIndex	=	new	DWORD[miTriCount];
		bResult		=	ReadDataFromFile( hFile, mpPlaneIndex, sizeof(DWORD)*miTriCount);
	}

	// ��� �б�.
	{
		mpPlane		=	new	PLANE[miPlaneCount];
		bResult		=	ReadDataFromFile( hFile, mpPlane, sizeof(PLANE)*miPlaneCount);
	}

	// bsp ����.
	{
		mpNode		=	new	BSPTreeNode[miNodeCount];
		bResult		=	ReadDataFromFile( hFile, mpNode, sizeof(BSPTreeNode)*miNodeCount);
	}
	
	// ��尡 ������ �ﰢ���� ����.
	{
		bResult		=	ReadDataFromFile( hFile, &miNodeTriIndexBuffUsedCount, sizeof(int));
		
		mpNodeTriIndexBuff		=	new	int[miNodeTriIndexBuffUsedCount];
		bResult		=	ReadDataFromFile( hFile, mpNodeTriIndexBuff, sizeof(int)*miNodeTriIndexBuffUsedCount);
	}

	miNodeTemp		=	0;

	//	ã��� ���� �����.
	this->pOutputBuffer	=	new DWORD[this->miTriCount];
	memset( pOutputBuffer, 0, sizeof(DWORD)*miTriCount);
	this->mpTempOutput	=	new BYTE[this->miTriCount];	// ��¿� �ε��� ����.
	memset( mpTempOutput, 0, sizeof(BYTE)*miTriCount);

	return	true;

}

bool	MBSPTree::ReadDataFromFile( HANDLE hFile, void* pData, DWORD dwCountOfByte)
{
		DWORD	dwTemp		=	0;
		BOOL	bResult		=	ReadFile( hFile, pData, dwCountOfByte, &dwTemp, 0);
		if( bResult == FALSE || dwCountOfByte != dwTemp)
		{
			_asm int 3;
			return	false;
		}
		return	true;
}

bool	MBSPTree::WriteDataToFile( HANDLE hFile, void* pData, DWORD dwCountOfByte)
{
	DWORD	dwTemp	=	0;
	BOOL	bResult		=	WriteFile( hFile, pData, dwCountOfByte, &dwTemp, 0);
	if( dwTemp != dwCountOfByte || bResult == FALSE)
	{
		_asm int 3;
		return	false;
	}
	return	true;
}

// CollisionMeshObjectTree.h: interface for the CCollisionMeshObjectTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLLISIONMESHOBJECTTREE_H__E4BF3706_8152_4D5D_AB80_33BD7659275F__INCLUDED_)
#define AFX_COLLISIONMESHOBJECTTREE_H__E4BF3706_8152_4D5D_AB80_33BD7659275F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../4DYUCHIGRX_COMMON/typedef.h"

class	CCollisionMeshObject;

// ���⼭ �ٷ� �ﰢ����.
struct	M_TRIANGLE_BUFFER
{
	DWORD						dwCount;			// �ﰢ���� ����.
	VECTOR3**					ppBuff;				// �ﰢ���� ������. �� ���۴� ��ü�� �ƴϴ�. ������ �� ppBuff[0]+n ������..
	DWORD*						pdwPlaneIndex;		// �ﰢ���� ��� �ε���.
	CCollisionMeshObject**		ppObjBuff;			// �ش� �ﰢ���� ������Ʈ ������.
	MAABB*						pBox;				// �ﰢ���� �ٿ�� �ڽ�.
};

// �� �ﰢ���� ��� ����.
struct	M_PLANE_BUFFER
{
	DWORD			dwCount;			// ��ü ����� ����.
	PLANE*			pPlane;				// ��� ����. ��ü��.
};

// �Ʒ� �׸��忡�� ���� �ӽ� ��ũ����Ʈ.
// �ڼ��Ѱ� CreateGrid()���� ����.
struct	M_TRIANGLE_LINK
{
	M_TRIANGLE_LINK*	pNext;
	DWORD				dwIndex;
};


// �ϳ��� �׸���.
struct	M_GRID_SECTOR
{
	MAABB					BB;				// Bounding Box;
	DWORD					dwTriCount;		// ��� �ﰢ���� ����.
	union
	{
		DWORD				dwTriStartIndex;		// ���չ���(?)���� �ﰢ���� ���� �ε���. ��ü �׸��忡�� new�� �ѹ��� �ϱ� ���� �Ǽ���.
		M_TRIANGLE_LINK*	pLink;					// ���� Ÿ�Ӷ� ���ϳ�, �ڼ��Ѱ� ũ������Ʈ�׸��忡��...
	};
	//	BOOL					bCheck;
};

// �׸����.
struct	M_GRID
{
	MAABB		WorldBox;
	
	DWORD		dwGridXCount;
	float		fGridXStep;
	
	DWORD		dwGridYCount;
	float		fGridYStep;
	
	DWORD		dwGridZCount;
	float		fGridZStep;
	
	M_GRID_SECTOR*			pGrid;				// ������ ����, x + (y * dwGridXCount) + (z * dwGridXCount * dwGridYCount);
	M_GRID_SECTOR*			GetSector( DWORD x, DWORD y, DWORD z)
	{
#ifdef	_DEBUG
		if( x >= this->dwGridXCount || y >= this->dwGridYCount || z >= this->dwGridZCount)
			_asm int 3;			// ������ �����.
#endif
		return	&(pGrid[x + (y*dwGridXCount) + (z*dwGridXCount*dwGridYCount)]);
	}
	DWORD*		pTriIndexBuffer;				// �׸��� ���Ͱ� ������ �ִ� �ﰢ�� �ε����� �迭 ���� ����.
	DWORD		dwTriIndexCount;				// ���� �迭�� ����.
};

// �ƿ�ǲ ���ۿ� �Ѹ��� ���� ��ũ.
struct	OUTPUT_LINK
{
	DWORD			dwPrev;
	DWORD			dwNext;

	DWORD			iIndex;
};

struct	OUTPUT_INDEX_BUFFER
{
	DWORD*						pdwOutputIndexBuffer;
	DWORD						dwOutputIndexCount;				// ���ۿ� �������͵� ����.
	OUTPUT_LINK*				pOutputLink;
	DWORD						dwOutputLinkCurrent;				// ���� ��ũ�� ��뷮.
};

class CCollisionMeshObjectTree  
{
public:
	void SetSimultaneousAccessCount( DWORD dwCount);
	BOOL		CollisionTestAAELLIPSOID( VECTOR3* pOut, VECTOR3* pOutLastVelocity, BOOL* pbOutLanding, AAELLIPSOID* pEllipsoid, VECTOR3* pTo, DWORD dwOutputBufferIndex);
	
	BOOL		CreateTree( CCollisionMeshObject* pObjBuff, DWORD dwObjCount);
	
	BOOL		CollisionTestLine( float* pOutT, VECTOR3* pFrom, VECTOR3* pTo, DWORD dwOutputBufferIndex);
	
	DWORD		FindTriangleWithBC(BOUNDING_CAPSULE *pBC, BOOL bFaceFromVertex, DWORD dwOutputBufferIndex);
	// bFaceFromVertex�� pBC�� from�̴�. from�� �ﰢ���� �տ� �ִ°ɷ� �����ȳ� ����.
	
	DWORD*		GetResultIndex(DWORD dwOutputBufferIndex);
	
	void		DeleteTree();
	
	CCollisionMeshObjectTree();
	virtual ~CCollisionMeshObjectTree();
	
protected:
	M_TRIANGLE_BUFFER			m_Triangle;
	M_PLANE_BUFFER				m_Plane;
	M_GRID						m_Grid;

	OUTPUT_INDEX_BUFFER*		m_pOutputBuffer;
	DWORD						m_dwOutputBufferCount;
//	DWORD*						m_pdwOutputIndexBuffer;
//	DWORD						m_dwOutputIndexCount;				// ���ۿ� �������͵� ����.
//	OUTPUT_LINK*				m_pOutputLink;
//	DWORD						m_dwOutputLinkCurrent;				// ���� ��ũ�� ��뷮.
	
	// ����׿��ΰ�.?
	DWORD						m_dwMemoryUsage;			// byte
	
	void		CreateGrid();
	void		DeleteGrid();

	void		DeleteFindBuffer(void);
	void		CreateFindBuffer( DWORD dwCount);

	// ã����� �Լ� �������� ��¸�ũ �ʱ�ȭ.
	void		ClearOutputLink( DWORD dwIndex);
	// ��� ��ũ�� �ε��� �ֵ�.
	void		AddOutputLink( int iIndex, DWORD dwBufferIndex);

	BOOL		TestPlaneWithGrid( M_GRID_SECTOR* pGrid, PLANE* pPlane);

#ifdef	_DEBUG
	void		DBG_CheckOutputBufferIndex(DWORD dwIndex);
#endif
};

#endif // !defined(AFX_COLLISIONMESHOBJECTTREE_H__E4BF3706_8152_4D5D_AB80_33BD7659275F__INCLUDED_)

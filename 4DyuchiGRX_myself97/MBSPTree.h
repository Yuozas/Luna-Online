/*
	�浹 ó���� ���� leaf based bsp tree.
	find �迭�� Ư�� ���� ���� ������ leaf�� ã�� ������� �۵��Ѵ�.
*/
#if !defined(AFX_MBSPTREE_H__9041AE2E_08D7_4BC4_B325_217816FAAF3D__INCLUDED_)
#define AFX_MBSPTREE_H__9041AE2E_08D7_4BC4_B325_217816FAAF3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../4Dyuchigrx_common/typedef.h"


#define		iBSPVer		1

struct	MBSPTreeFileHeader
{
	// ���.
//	char	pHeader[4];
	// ����.
	int		iVer;
	// �ﰢ�� ����.
	int		iTriCount;
	// ��� ����.
	int		iPlaneCount;
	// ��� ����
	int		iNodeCount;
	int		iNormalNodeCount;
	int		iLeafCount;	// ���� ����.
	int		iNullLeafCount;	// �ﰢ���� �ƹ��͵� ������ ���� ���� ī��Ʈ.
	int		iMaxLevel;

};

struct	BSPTreeNodeRuntime
{
	DWORD*			pTriIndex;		//	�� ��尡 ���� �ﰢ���� �ε���.
	DWORD*			pPlaneIndex;	//	�� ��尡 ���� �ﰢ���� ����� �ε���. -_-;
	DWORD			dwPlaneCount;	//	�� ��尡 ���� �ﰢ���� ����� ����.
	DWORD*			pChecked;		//	�� ��尡 ���� ����� ���Ǿ�����.?
	DWORD			dwCheckedPlaneCount;	//	���� ��� ����.
	float			fWhere;			// Ʈ�� ����� �� ������...

};

struct	BSPTreeNode
{
	int				iFront;
	int				iBack;
	int				iParent;

	BOOL			bIsLeaf;		// �̳��� �����ΰ�.?
	
	int				iDivPlane;		//	�� ��尡 ������ ���.

	int				iTriCount;		//	�� ��尡 ���� �ﰢ���� ����.
	int				iTriStartIndex;


	int				iLevel;		// �� ����� ��.


	// �ﰢ����, Ʈ���� ����� �����̳� ��尡 ������ ��� �ܿ� �ʿ䰡 ����. 
	BSPTreeNodeRuntime*		r;
};


class MBSPTree  
{
public:
	bool	Save( HANDLE hFile);			// �ܺο��� �� �ڵ�, �׳� �� ������ ���̺�.
	bool	Load( HANDLE hFile);

	BOOL	Init(void);		// �ʱ�ȭ �ϰ�,
	DWORD	AddTriangle( VECTOR3* IN pTri);		// �ﰢ���� Ǫ�� Ǫ��.
	BOOL	BuildTree(void);	// Ʈ�� �����.

	// pBuff�� ���ϵǴ� ���� �ε��� ���̰�, �ﰢ���� GetTriBuffer�� ���� ��´�. pTri�� �����Ǹ� ���...
	DWORD	FindNearTri( DWORD* pBuff, DWORD dwBuffSize, VECTOR3* pSPivot, float fSRadius);	// pSPivot�� fSRadius�� �־��� ������ ������ �ﰢ���� ã�´�.
	VECTOR3*	GetTriBuffer(void){	return this->mpTri;}
	DWORD	GetTriCount(){return miTriCount;}

	void DeleteAll(void);

	MBSPTree();
	~MBSPTree();

private:
	// Save;
	VECTOR3*				mpTri;				// ������.
	BOUNDING_SPHERE*		mpTriSphere;		// �ﰢ���� �ٿ�� ���Ǿ�.
	int						miTriCount;			// ��ü �ﰢ���� ����.
	DWORD*					mpPlaneIndex;		// �� �ﰢ���� ��� �ε���.

	PLANE*					mpPlane;				// ���, ������ �ﰢ���� ���ų� �ﰢ���������� �۰���.
	int						miPlaneCount;

	// bsp ����.
	BSPTreeNode*			mpNode;
	int						miNodeCount;

	int						miNormalNodeCount;
	int						miLeafCount;	// ���� ����.
	int						miNullLeafCount;	// �ﰢ���� �ƹ��͵� ������ ���� ���� ī��Ʈ.
	int						miMaxLevel;
	
	// ��尡 ������ �ﰢ���� ����.
	int*					mpNodeTriIndexBuff;
	int						miNodeTriIndexBuffUsedCount;

	// do not Save
	int						miNodeTemp;

	BYTE*					mpTempOutput;	// ��¿� �ε��� ����.

	VECTOR3*				mpPreTriangle;		// AddTriangle�� ���� �ӽ� ����.
	DWORD					mdwPreTriangleCount;		// ����...

	DWORD*					pOutputBuffer;


	static	int				siCount;


	bool					mbBuilt;


	int		CreateNodeReculsive( int iParent, BOOL bIsBack, DWORD* pTriIndex, DWORD dwTriCount, DWORD* pCheckedPlane, int iLevel);// ��������� Ʈ�� �����.
	int		WhereIsTriangleFromPlane( VECTOR3* pTri, PLANE* pPlane);	// �־��� �ﰢ���� ����� ��� �ִ°�.?

	int		FindDivideCandidate( DWORD* pTriIndex, DWORD dwTriCount, DWORD*	pCheckedPlane);	// �־��� �ﰢ����, ��� ���� ���� ���� ���� ��󳽴�.
	void	FindNearTriReculsive( int iNode);	// FindNearObject�� ���� ȣ���.	
	VECTOR3*	pSPivot;
	float		fSRadius;
	
	DWORD					mdwDBG_NodeTravelCount;			// Ʈ���� ������ ��带 ���ƴٴ� Ƚ��.

	BOOL	IsTooFarBetweenVertexAndTri( float fMaxDist, DWORD dwTriIndex, VECTOR3* pVertex);		// �ʹ� �ָ� TRUE, �Ÿ� �ȿ� ������ FALSE

	bool	ReadDataFromFile( HANDLE hFile, void* pData, DWORD dwCountOfByte);
	bool	WriteDataToFile( HANDLE hFile, void* pData, DWORD dwCountOfByte);

};

#endif // !defined(AFX_MBSPTREE_H__9041AE2E_08D7_4BC4_B325_217816FAAF3D__INCLUDED_)

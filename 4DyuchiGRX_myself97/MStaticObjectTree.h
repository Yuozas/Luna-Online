// MStaticObjectTree.h: interface for the MStaticObjectTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSTATICOBJECTTREE_H__0E167021_FE94_4C6F_A9DC_0EE8E0D3BB84__INCLUDED_)
#define AFX_MSTATICOBJECTTREE_H__0E167021_FE94_4C6F_A9DC_0EE8E0D3BB84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../4Dyuchigrx_common/typedef.h"

class MStaticObjectTree  
{
public:
	BOOL	Save( HANDLE hFile){return FALSE;}			// �ܺο��� �� �ڵ�, �׳� �� ������ ���̺�.
	BOOL	Load( HANDLE hFile){return FALSE;}

	BOOL	Init(UINT uiTriCount);						// �ʱ�ȭ �ϰ�,
	DWORD	AddTriangle( VECTOR3* IN pTri);				// �ﰢ���� Ǫ�� Ǫ��.
	BOOL	BuildTree(UINT iDensity);							// Ʈ�� �����.

	// pBuff�� ���ϵǴ� ���� �ε��� ���̰�, �ﰢ���� GetTriBuffer�� ���� ��´�. pTri�� �����Ǹ� ���...
	// pSPivot�� fSRadius�� �־��� ������ ������ �ﰢ���� ã�´�. bLookAtPivotOnly�� pBS�� ������ ���ѳ� �ɷ�����.
	DWORD	FindTriWithSphere( DWORD* pIndexBuff, DWORD dwBuffSize, BOUNDING_SPHERE* pBS, BOOL bLookAtPivotOnly);
	DWORD	FindTriWithBOUNDING_CAPSULE( DWORD* pIndexBuff, DWORD dwBuffSize, BOUNDING_CAPSULE* pBC, BOOL bLookAtPivotOnly);
	VECTOR3*	GetTriBuffer(void){	return	mpVertex;}
//	DWORD	GetTriCount(){return 0;}
	void	GetTriBoundingSphere( BOUNDING_SPHERE* pOutBS, UINT uiTriNum){ *pOutBS = mpTri[uiTriNum].BS;}

	void	DeleteAll(void);

	MStaticObjectTree();
	~MStaticObjectTree();

protected:
	
	struct	STriangle
	{
		BOUNDING_SPHERE		BS;
		MAABB				Box;
		PLANE				Plane;
//		VECTOR3				pVertex[3];

	};

	STriangle*		mpTri;
	VECTOR3*		mpVertex;
	UINT			muiTriCount;

	UINT			muiDensity;				// ���� �ϳ��� �׸���� �� �ﰢ�� ����.
	UINT			muiGridCount;			// �������� �׸����� ����.
	// ������ �ﰢ�� ������ mdwGridCount^3 * mdwDensity
	float			mfGridXStep;		// x����,
	float			mfGridYStep;		// y����.
	float			mfGridZStep;		// z����.
	struct	SGrid
	{
		BOUNDING_SPHERE		BS;
		MAABB				Box;
		UINT*				puiTri;
		UINT				uiTriCount;
	};
	SGrid*				mpGrid;

	UINT			muiTemp;

	MAABB			mBox;
	BOUNDING_SPHERE		mBS;

	BYTE*			mpCheck;

};

#endif // !defined(AFX_MSTATICOBJECTTREE_H__0E167021_FE94_4C6F_A9DC_0EE8E0D3BB84__INCLUDED_)

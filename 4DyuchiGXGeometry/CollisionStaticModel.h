// CollsionStaticModel.h: interface for the CCollisionStaticModel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLLSIONSTATICMODEL_H__0C5C9E86_1BDD_4EDD_B9D5_F5EC86C0CD22__INCLUDED_)
#define AFX_COLLSIONSTATICMODEL_H__0C5C9E86_1BDD_4EDD_B9D5_F5EC86C0CD22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "colmeshobj.h"

#pragma pack(push,4)
struct FILE_COLLISION_STATIC_MODEL_HEADER
{
	DWORD					dwVersion;
	DWORD					dwObjectNum;
};
#pragma pack(pop)

//! CoStaticObject�� �浹 ó�� ����.
/*!
	�浹 ó���� �޽��� �浹ó�� �޼ҵ带 ���ο� ������ �ִ�.
*/
class CCollisionStaticModel  
{
	DWORD							m_dwObjectsNum;
	CCollisionMeshObject*			m_pCollisionMeshObjectList;

	CCollisionMeshObjectTree*		m_pSearchTree;

	BOOL BuildSearchTree( DWORD dwFlag);	// ������ ���� �� ����Ʈ��.
	void ReleaseSearchTree();

public:
	void	SetSimultaneousAccessCount( DWORD dwSimultaneousAccessCount);			// ���ÿ� ������ ����.
	/// ���ΰ� ������ �� ����.
	BOOL CollisionTestLine( float* pOutT, VECTOR3* pFrom, VECTOR3* pTo, DWORD dwOutputBufferIndex);
	/// �־��� Ÿ��ü�� ������ �浹 ó�� �׽�Ʈ. �ݹ��� ��� ���ֳ�?
	BOOL CollisionTestAAELLIPSOID( VECTOR3* pOut, VECTOR3* pOutLastVelocity, BOOL* pbOutLanding, CollisionTestCallBackProcedure CallBack, AAELLIPSOID* pEllipsoid, VECTOR3* pTo, DWORD dwOutputBufferIndex);

	BOOL							Render(I4DyuchiGXRenderer* pRenderer,DWORD dwFlag);
	BOOL							ReadFile(char* szFileName);

	CCollisionStaticModel();
	~CCollisionStaticModel();

};

#endif // !defined(AFX_COLLSIONSTATICMODEL_H__0C5C9E86_1BDD_4EDD_B9D5_F5EC86C0CD22__INCLUDED_)

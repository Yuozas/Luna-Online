// GXDecal.h: interface for the CGXDecal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GXDECAL_H__A91A25C0_EB23_49E0_88E7_9AA0DE45307D__INCLUDED_)
#define AFX_GXDECAL_H__A91A25C0_EB23_49E0_88E7_9AA0DE45307D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define		DECAL_TRI_MAX		200

#include "../4DyuchiGRX_common/typedef.h"
#include "GXMapObject.h"


class	CoExecutive;
class	CGXDecal  : public CGXMapObject
{
	
	DWORD			m_dwTriCount;

	CoExecutive*	m_pExecutive;
	DECAL_DESC		m_DecalDesc;
//	MATRIX4			m_RotateMatrix;
	VECTOR3			m_X, m_Y, m_Z;
	PLANE			m_pBoxPlane[6];	// ��, ��, ���� �� ������ �Ʒ�.

	// ���͸���.
	void*			m_MaterialHandle;
	// �ٿ�� ����.
	MAABB				m_Box;
	BOUNDING_SPHERE		m_BS;

	// ������Ʈ Ÿ��.
//	DWORD			m_dwObjType;			// GX_MAP_OBJECT_TYPE_DECAL �̳�����.

	GXDecalSchedulePROC			m_pProc;
	IDIImmMeshObject*			m_pImmMeshObject;
	IDIMeshObject*				m_pRenderMeshObject;
//	IVERTEX*					m_pTriBuffer;
	DWORD						m_dwTriNum;

public:
	COLLISION_MESH_OBJECT_DESC*	__stdcall	GetCollisionMeshObjectDesc();
	DWORD						__stdcall	OnFrame(I4DyuchiGXExecutive* pExecutive,DWORD msg,int arg1,int arg2);
	DWORD						__stdcall	GetObjectIndexInModel();
	BOOL						__stdcall	Render();
	BOOL						__stdcall	IsCollisionWithRay(VECTOR3* pv3IntersectPoint,DWORD* pdwModelIndex,DWORD* pdwObjIndex,float* pfDist,VECTOR3* pv3RayPos, VECTOR3* pv3RayDir,DWORD dwViewportIndex,DWORD dwFlag);
	BOOL						__stdcall	IsCollisionWithScreenCoord(VECTOR3* pv3IntersectPoint,DWORD* pdwModelIndex,DWORD* pdwObjIndex,float* pfDist,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag);
	DWORD						__stdcall	CreateIVertexList(IVERTEX** ppVertex);
	void						__stdcall	ReleaseIVertexList(IVERTEX*	pVertex);
	BOOL						__stdcall	ShadeLightMapObject(VECTOR3* pv3,DWORD dwFacesNum,LIGHT_DESC* pLight,SHADE_FUNC pFunc);
	
	
	BOOL	Initialize( CoExecutive* pExecutive, DECAL_DESC* pDesc,GXDecalSchedulePROC pProc);
	DWORD	AddTri( VECTOR3* pTri);				// �ϳ��� �ﰢ���� add. ������ ��Į���� �ﰢ�� �����ȴ�.
	DWORD	Build();							// ��Į ���� ������. ���ϰ��� ��Į �ﰢ�� ����.

	BOOL	ChangeMaterial( char* szFileName);

	void	Delete();		// �޸𸮿��� ����°� �ƴϰ�, �׳� ���.

	GX_MAP_OBJECT_TYPE	GetObjectType(){ return GX_MAP_OBJECT_TYPE_DECAL;}
	void	GetAABB( MAABB* pBox){ *pBox	=	m_Box;}
	void	GetBoundingSphere( BOUNDING_SPHERE* pBS){ *pBS = m_BS;}

private:
	void	GenTexCoor();
	void	CreateBoundingVolume();

	DWORD	ClipPolygonWithPlane( VECTOR3* pOutPoly, VECTOR3* pInPoly, DWORD dwInPolyCount, PLANE* pClipPlane);

	DWORD	ClipOneTri( VECTOR3* pTri);

	void	CreateMaterial( char* szFileName);
	void	DeleteMaterial();

};



#endif // !defined(AFX_GXDECAL_H__A91A25C0_EB23_49E0_88E7_9AA0DE45307D__INCLUDED_)

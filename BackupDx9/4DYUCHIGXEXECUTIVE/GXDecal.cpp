// GXDecal.cpp: implementation of the CGXDecal class.
//
//////////////////////////////////////////////////////////////////////

#include "GXDecal.h"
#include "../4DYuchiGRX_myself97/TriangleLib.h"
#include "../4DYuchiGRX_myself97/CheckClock.h"
#include "../4DyuchiGRX_myself97/myself97common.h"
#include "../4DYuchiGXGFunc/global.h"
#include "Executive.h"
#include "GXDefault.h"


VECTOR3		g_pBuff1[DECAL_TRI_MAX];
VECTOR3		g_pBuff2[DECAL_TRI_MAX];
int			g_piDet[DECAL_TRI_MAX];
IVERTEX		g_pTri[3*DECAL_TRI_MAX];




DWORD CGXDecal::ClipOneTri( VECTOR3* pTri)
{
	if( m_dwTriCount == DECAL_TRI_MAX)
	{ 
#ifdef	_DEBUG
//		_asm int 3;			// �ִ�ġ�� �Ѵ� ���.
#endif
		return	0;
		
	}
	VECTOR3*	pSrc	=	g_pBuff1;
	VECTOR3*	pDest	=	g_pBuff2;
	VECTOR3*	pTemp;
	memcpy( pSrc, pTri, sizeof(VECTOR3)*3);
	DWORD		dwSrc	=	3;
	DWORD		dwDest	=	0;
	DWORD	i;
	for( i = 0; i < 6; i++)
	{
		dwDest	=	ClipPolygonWithPlane( pDest, pSrc, dwSrc, &(m_pBoxPlane[i]));
		if( dwDest == 0)
			return 0;
		if( dwDest <= 2)
		{
			_asm nop;
			return	0;
		}
		if( dwDest >= DECAL_TRI_MAX)			// �ִ�ġ�� ������... �׳� ������.
		{
			_asm int 3;
			return	0;
		}

		// �ҽ��� ����Ʈ �ٲٰ�,
		pTemp	=	pSrc;
		pSrc	=	pDest;
		pDest	=	pTemp;
		dwSrc	=	dwDest;
	}

	if( dwSrc == 0)
		return	0;

	if( dwSrc <= 2)
	{
		_asm int 3;		// ����� �������� ���� ��� �ﰢ���� ���� �ȵǹǷ� ��~
		return	0;
	}
	
	DWORD	j;
	// �������� �ﰢ������ Ǯ� IVERTEX�� �ִ´�. ���߿� �ﰢ�� ������ �ٷ� ����.
	for( i = 1; i < dwSrc-1; i++)
	{
		g_pTri[m_dwTriCount*3+0].x	=	pSrc[0].x;
		g_pTri[m_dwTriCount*3+0].y	=	pSrc[0].y;
		g_pTri[m_dwTriCount*3+0].z	=	pSrc[0].z;

		for( j = 0; j < 2; j++)
		{
			g_pTri[m_dwTriCount*3+1+j].x	=	pSrc[i+j].x;
			g_pTri[m_dwTriCount*3+1+j].y	=	pSrc[i+j].y;
			g_pTri[m_dwTriCount*3+1+j].z	=	pSrc[i+j].z;
		}

		m_dwTriCount++;
		if( m_dwTriCount >= DECAL_TRI_MAX)
			break;
	}

	_asm nop;
}

// pOutPoly, ����� ����.
// ���ϰ��� �߸����� ������ ���ؽ� ����.
// pInPoly, �Է� ����.
// dwInPolyCount, ���� �������� ���ؽ� ����.
// pClipPlane, �ڸ� ���.
// ����� �� �������� ������.
DWORD CGXDecal::ClipPolygonWithPlane( VECTOR3* pOutPoly, VECTOR3* pInPoly, DWORD dwInPolyCount, PLANE* pClipPlane)
{

#ifdef	_DEBUG
	if( dwInPolyCount < 3)
	{
		_asm int 3;				// �ּ����� �ﰢ���� �ȵȴ�. ��鿡 �߸� �ڰݵ� ���� ������.?
		return	0;
	}
#endif

	DWORD	dwOutPolyCount	=	0;

//	int		piDet[DECAL_TRI_MAX];			// �� ���� ����� ��� �ִ°�.? +1, 0, -1
	int*	piDet	=	g_piDet;
	// ��ġ ����. ���߿� ���̴°� �װ�Ƽ�길�ΰ�.?
//	DWORD	dwPositive	=	0;
	DWORD	dwNegative	=	0;
//	DWORD	dwZero		=	0;
	
	float	t;						// �ӽ� ���� ������ t
	VECTOR3		v;

	DWORD	i;

	// ��ȣ �˻��ϰ�,
	for( i = 0; i < dwInPolyCount; i++)
	{
		float	fDet	=	DotProduct( &(pInPoly[i]), &(pClipPlane->v3Up))	+	pClipPlane->D;
		if( fDet > 0.0001f)
		{
			piDet[i]	=	1;
//			dwPositive++;
		}
		else if( fDet < -0.0001f)
		{
			piDet[i]	=	-1;
			dwNegative++;
		}
		else
		{
			piDet[i]	=	0;
//			dwZero++;
		}
	}

	// Ȥ�� ��� ���� ��� ���ʿ� �����ϴ°�.?
	if( dwInPolyCount == dwNegative)
	{
		return	0;		// �׷��� �ƹ��͵� �ڸ����� ������ ����.
	}

	// Ȥ�� ������� ����� �� Ȥ�� ������� �ִ°�.?
	if( dwNegative == 0)
	{
		// �״�� ī���ؼ� ��������.
		memcpy( pOutPoly, pInPoly, sizeof(VECTOR3)*dwInPolyCount);
		return	dwInPolyCount;
	}

	// ���� ����.������ ���鼭, ����� �տ� �ִ� �ǴܵǸ� �ƿ�ǲ ���ۿ� �߰��Ѵ�. �ܼ��ϰ� ��������.
	for( i = 0; i < dwInPolyCount; i++)
	{
		// ���� ó���� ���� �ε���.
		DWORD	dwCurrent	=	i;				// ������ �ε���.
		DWORD	dwNext;					// ������ �ε���.
		if( i == dwInPolyCount-1)
			dwNext	=	0;
		else	dwNext	=	i+1;

		if( piDet[dwCurrent] != -1)		// '+', '0'
		{
			// ���� �ڵ�, ������ �߰�.
			pOutPoly[dwOutPolyCount]	=	pInPoly[dwCurrent];
			dwOutPolyCount++;
			if( dwOutPolyCount >= DECAL_TRI_MAX)
				_asm int 3;

			if( piDet[dwNext] == -1)	// ������ '-' �� ���
			{
				// �߸��� �߰�.	
//				v	=	pInPoly[dwNext] - pInPoly[dwCurrent];
				VECTOR3_SUB_VECTOR3( &v, &(pInPoly[dwNext]), &(pInPoly[dwCurrent]));
				t	=	-1.0f * ( pClipPlane->D + DotProduct( &(pClipPlane->v3Up), &(pInPoly[dwCurrent]))) / ( DotProduct(&(pClipPlane->v3Up), &v));
//				pOutPoly[dwOutPolyCount]	=	pInPoly[dwCurrent]	+	v * t;
				VECTOR3_MUL_FLOAT( &(pOutPoly[dwOutPolyCount]), &v, t);
				VECTOR3_ADDEQU_VECTOR3( &(pOutPoly[dwOutPolyCount]), &(pInPoly[dwCurrent]));
				dwOutPolyCount++;
				if( dwOutPolyCount >= DECAL_TRI_MAX)
					_asm int 3;
			}
		}
		else // piDet[dwCurrent] == -1	// '-'
		{
			if( piDet[dwNext] == 1)			// '+' -> '+' ���� ��°�� add
			{
				// �߸��� �߰�.
//				v	=	pInPoly[dwNext] - pInPoly[dwCurrent];
				VECTOR3_SUB_VECTOR3( &v, &(pInPoly[dwNext]), &(pInPoly[dwCurrent]));
				t	=	-1.0f * ( pClipPlane->D + DotProduct( &(pClipPlane->v3Up), &(pInPoly[dwCurrent]))) / ( DotProduct(&(pClipPlane->v3Up), &v));
//				pOutPoly[dwOutPolyCount]	=	pInPoly[dwCurrent]	+	v * t;
				VECTOR3_MUL_FLOAT( &(pOutPoly[dwOutPolyCount]), &v, t);
				VECTOR3_ADDEQU_VECTOR3( &(pOutPoly[dwOutPolyCount]), &(pInPoly[dwCurrent]));
				dwOutPolyCount++;
				if( dwOutPolyCount >= DECAL_TRI_MAX)
				{
					_asm int 3;
					break;
				}
			}
			else		// �׿ܿ� �ƹ��͵� ����.
			{
				_asm nop;
			}
		}
	}

	return	dwOutPolyCount;
}


BOOL __stdcall CGXDecal::Render()
{
	DWORD	dwRenderClock	=	GetLowClock();

	I4DyuchiGXRenderer*	pRenderer	=	m_pExecutive->GetRenderer();

	pRenderer->RenderMeshObject(m_pRenderMeshObject,0,0,m_dwAlphaFlag,NULL,0,NULL,0,(DWORD)m_MaterialHandle,0,0);
	dwRenderClock	=	GetLowClock()	-	dwRenderClock;
	
	return	TRUE;
}




void CGXDecal::GenTexCoor()
{

	// �ؽ��� �� �÷��� ���� ���� �ؽ��� Ȥ�� �Ǹ����� �ؽ�����ǥ ����.(����)
	VECTOR3	Current;
	DWORD	i;
	float	fDist;
	for( i = 0; i < m_dwTriCount*3; i++)
	{
		Current.x	=	g_pTri[i].x;
		Current.y	=	g_pTri[i].y;
		Current.z	=	g_pTri[i].z;
		// u��ǥ(�ؽ���x�� ������ �������� �Ÿ�)
		fDist	=	DotProduct( &Current, &(m_pBoxPlane[4].v3Up)) + m_pBoxPlane[4].D;
		g_pTri[i].u1	=	fDist / (m_DecalDesc.v3XYZScale.x*2);
		// v��ǥ(�ؽ���y�� �Ʒ� �������� �Ÿ�)
		fDist	=	DotProduct( &Current, &(m_pBoxPlane[5].v3Up)) + m_pBoxPlane[5].D;
		g_pTri[i].v1	=	fDist / (m_DecalDesc.v3XYZScale.y*2);
	}

}

void CGXDecal::CreateBoundingVolume()
{
#ifdef	_DEBUG
	if( m_dwTriCount == 0)			// �ﰢ���� �ϳ��� ���µ� ȣ����.
	{
		_asm int 3;
		return	;
	}
#endif

	// �ٿ�� �ڽ�.
	MAABB	Box;
	Box.Max.x	=	Box.Min.x	=	g_pTri[0].x;
	Box.Max.y	=	Box.Min.y	=	g_pTri[0].y;
	Box.Max.z	=	Box.Min.z	=	g_pTri[0].z;

	for( DWORD i = 1; i < m_dwTriCount; i++)
	{
		// x
		if( Box.Max.x < g_pTri[i].x)			// x max
			Box.Max.x	=	g_pTri[i].x;
		if( Box.Min.x > g_pTri[i].x)			// x min
			Box.Min.x	=	g_pTri[i].x;
		
		// y
		if( Box.Max.y < g_pTri[i].y)			// y max
			Box.Max.y	=	g_pTri[i].y;
		if( Box.Min.y > g_pTri[i].y)			// y min
			Box.Min.y	=	g_pTri[i].y;

		// z 
		if( Box.Max.z < g_pTri[i].z)			// z max
			Box.Max.z	=	g_pTri[i].z;
		if( Box.Min.z > g_pTri[i].z)			// z min
			Box.Min.z	=	g_pTri[i].z;
	}
	m_Box	=	Box;

	// �ٿ�� ���Ǿ�.
	BOUNDING_SPHERE		bs;
	bs.v3Point	=	(Box.Max + Box.Min) / 2.0f;
	bs.fRs		=	VECTOR3Length( &(Box.Max - bs.v3Point));

	m_BS		=	bs;


	return;
}

BOOL CGXDecal::Initialize( CoExecutive* pExecutive, DECAL_DESC* pDesc,GXDecalSchedulePROC pProc)
{


#ifdef	_DEBUG
	if( pExecutive == 0)
	{
		_asm int 3;
	}
#endif

	memset(this,0,sizeof(CGXDecal));

	CGXMapObject::Cleanup();
	m_dwRenderFlag = 0xfffffff5;
	m_dwAlphaFlag = 0x000000ff;
	m_dwID = 0xfffffff0;

	m_pProc = pProc;
	if (!m_pProc)
		m_pProc = GXDefaultDecalSchedulePROC;


	m_pExecutive	=	pExecutive;
	m_DecalDesc		=	*pDesc;
	m_dwTriCount	=	0;




	// Ŭ���ο� ����� ����� ���� ��Į ��ü�� X,Y,Z�� ���Ѵ�.
	CrossProduct( &m_X, &(m_DecalDesc.v3UpDirection), &(m_DecalDesc.v3FaceDirection));
	Normalize( &m_X, &m_X);
	VECTOR3_MULEQU_FLOAT( &m_X, m_DecalDesc.v3XYZScale.x);
	Normalize( &m_Y, &(m_DecalDesc.v3UpDirection));
	VECTOR3_MULEQU_FLOAT( &m_Y, m_DecalDesc.v3XYZScale.y);
	Normalize( &m_Z, &(m_DecalDesc.v3FaceDirection));
	VECTOR3_MULEQU_FLOAT( &m_Z, m_DecalDesc.v3XYZScale.z);

	// ��� �����.
	VECTOR3		PlaneTri[3];
	// ù��° ���.	��.
	PlaneTri[0]		=	pDesc->v3Position - m_X + m_Y + m_Z;
	PlaneTri[1]		=	pDesc->v3Position + m_X + m_Y + m_Z;
	PlaneTri[2]		=	pDesc->v3Position + m_X - m_Y + m_Z;
	CalcPlaneEquation( &(m_pBoxPlane[0]), PlaneTri);
	// �ι�° ���.	��.
	PlaneTri[0]		=	pDesc->v3Position + m_X + m_Y - m_Z;			// ���߿� ��� �ڷ� ���°� �߰�.
	PlaneTri[1]		=	pDesc->v3Position - m_X + m_Y - m_Z;
	PlaneTri[2]		=	pDesc->v3Position - m_X - m_Y - m_Z;
	CalcPlaneEquation( &(m_pBoxPlane[1]), PlaneTri);
	// ����° ���.	����.
	PlaneTri[0]		=	pDesc->v3Position - m_X + m_Y;
	PlaneTri[1]		=	pDesc->v3Position - m_X + m_Y + m_Z;
	PlaneTri[2]		=	pDesc->v3Position - m_X - m_Y;
	CalcPlaneEquation( &(m_pBoxPlane[2]), PlaneTri);
	// �׹�° ���.	��.
	PlaneTri[0]		=	pDesc->v3Position + m_X + m_Y;
	PlaneTri[1]		=	pDesc->v3Position + m_X + m_Y + m_Z;
	PlaneTri[2]		=	pDesc->v3Position - m_X + m_Y;
	CalcPlaneEquation( &(m_pBoxPlane[3]), PlaneTri);
	// �ټ���° ���.	������.
	PlaneTri[0]		=	pDesc->v3Position + m_X + m_Y;
	PlaneTri[1]		=	pDesc->v3Position + m_X - m_Y;
	PlaneTri[2]		=	pDesc->v3Position + m_X - m_Y + m_Z;
	CalcPlaneEquation( &(m_pBoxPlane[4]), PlaneTri);
	// ������° ���.	�Ʒ�.
	PlaneTri[0]		=	pDesc->v3Position + m_X - m_Y;
	PlaneTri[1]		=	pDesc->v3Position - m_X - m_Y;
	PlaneTri[2]		=	pDesc->v3Position - m_X - m_Y + m_Z;
	CalcPlaneEquation( &(m_pBoxPlane[5]), PlaneTri);

	return	TRUE;
}

DWORD CGXDecal::AddTri( VECTOR3* pTri)
{
#ifdef	_DEBUG
	DWORD	dwClipTriangleClock;
	CheckVectorArray( pTri, 3);
#endif
	// ���� ������ �ﰢ������ Ʈ������ �ɷ�������̹Ƿ� ���� �״�� ������ �ɵ� �ϴ�.

	// ������ �̷�� ������ �������� �ﰢ���� �ڸ���.
#ifdef	_DEBUG
	dwClipTriangleClock	=	GetLowClock();
#endif
	DWORD	dwClippedTriCount	=	ClipOneTri( pTri);
#ifdef	_DEBUG
	dwClipTriangleClock	=	GetLowClock()	-	dwClipTriangleClock;
#endif
	// �ﰢ�� �� �߶���.
	return	dwClippedTriCount;
}

DWORD CGXDecal::Build()
{
	// �ﰢ�� �� �߸� ��Ȳ���� �ﰢ���� �ϳ��� ���� ��� �⺻ ������ �����.
	if( m_dwTriCount != 0)
		goto	lbReturnTrue;

	VECTOR3		pTemp[6];
	pTemp[0]		=	m_DecalDesc.v3Position	+	m_X	+	m_Y;
	pTemp[1]		=	m_DecalDesc.v3Position	-	m_X	+	m_Y;
	pTemp[2]		=	m_DecalDesc.v3Position	-	m_X	-	m_Y;
	
	pTemp[3]		=	m_DecalDesc.v3Position	+	m_X	+	m_Y;
	pTemp[4]		=	m_DecalDesc.v3Position	-	m_X	-	m_Y;
	pTemp[5]		=	m_DecalDesc.v3Position	+	m_X	-	m_Y;

	DWORD	i;
	for( i = 0; i < 2*3; i++)
	{
		g_pTri[i].x		=	pTemp[i].x;
		g_pTri[i].y		=	pTemp[i].y;
		g_pTri[i].z		=	pTemp[i].z;
	}
	m_dwTriCount	=	2;

lbReturnTrue:
	CreateBoundingVolume();
	GenTexCoor();
	CreateMaterial( m_DecalDesc.szMaterialName);

	m_pRenderMeshObject = m_pExecutive->GetRenderer()->CreateImmMeshObject(g_pTri,m_dwTriCount,m_MaterialHandle,0);
	if (!m_pRenderMeshObject)
	{
		m_dwTriCount = 0;
		goto lb_return;
	}

	m_pRenderMeshObject->QueryInterface(IID_IMM_MESH_OBJECT,(void**)&m_pImmMeshObject);


lb_return:
	return	m_dwTriCount;
}

BOOL CGXDecal::ChangeMaterial( char* szFileName)
{
	DeleteMaterial();
	CreateMaterial( szFileName);
	
	return	FALSE;
}

void CGXDecal::CreateMaterial( char* szFileName)
{
	if( m_MaterialHandle)
	{
		_asm int 3;
		DeleteMaterial();
	}
	// ���͸���.
	MATERIAL	Material;
	memset( &Material, 0, sizeof(Material));
	Material.dwDiffuse	=	0xff444444;
	Material.dwAmbient	=	0xff444444;
	Material.fTransparency = 1.0f;



	lstrcpy( Material.szDiffuseTexmapFileName, szFileName);
	GetNameRemovePath(Material.szDiffuseTexmapFileName,Material.szDiffuseTexmapFileName);
	
	CharToSmallASCIILen(Material.szDiffuseTexmapFileName,Material.szDiffuseTexmapFileName);
	



	void* MaterialHandle	=	m_pExecutive->GetRenderer()->CreateMaterial( &Material,NULL,NULL, 0);
	if( MaterialHandle == 0)
		_asm int 3;
	m_MaterialHandle	=	MaterialHandle;
}

void CGXDecal::DeleteMaterial()
{
	if( m_MaterialHandle)
		m_pExecutive->GetRenderer()->DeleteMaterial( m_MaterialHandle);
	m_MaterialHandle	=	0;
}

void CGXDecal::Delete()
{
	CGXMap*	pMap = m_pExecutive->GetGXMap();

	pMap->DeleteDecal(m_dwClipperIndex);
	if (m_pImmMeshObject)
	{
		m_pImmMeshObject->Release();
		m_pImmMeshObject = NULL;
	}
	if (m_pRenderMeshObject)
	{
		m_pRenderMeshObject->Release();
		m_pRenderMeshObject = NULL;
	}

	DeleteMaterial();
}

COLLISION_MESH_OBJECT_DESC*	__stdcall	CGXDecal::GetCollisionMeshObjectDesc()
{
	return NULL;
}
DWORD __stdcall	CGXDecal::OnFrame(I4DyuchiGXExecutive* pExecutive,DWORD msg,int arg1,int arg2)
{
	return m_pProc(pExecutive,this,msg,arg1,arg2,m_pData);

}
DWORD __stdcall	CGXDecal::GetObjectIndexInModel()
{
	return 0;
}
BOOL __stdcall CGXDecal::IsCollisionWithRay(VECTOR3* pv3IntersectPoint,DWORD* pdwModelIndex,DWORD* pdwObjIndex,float* pfDist,VECTOR3* pv3RayPos, VECTOR3* pv3RayDir,DWORD dwViewportIndex,DWORD dwFlag)
{
	*pdwModelIndex = 0xffffffff;
	*pdwObjIndex = 0xffffffff;
	return FALSE;
}
BOOL __stdcall CGXDecal::IsCollisionWithScreenCoord(VECTOR3* pv3IntersectPoint,DWORD* pdwModelIndex,DWORD* pdwObjIndex,float* pfDist,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag)
{
	*pdwModelIndex = 0xffffffff;
	*pdwObjIndex = 0xffffffff;
	return FALSE;
}
DWORD __stdcall	CGXDecal::CreateIVertexList(IVERTEX** ppVertex)
{
	return 0;
}
void __stdcall CGXDecal::ReleaseIVertexList(IVERTEX*	pVertex)
{
}
BOOL __stdcall CGXDecal::ShadeLightMapObject(VECTOR3* pv3,DWORD dwFacesNum,LIGHT_DESC* pLight,SHADE_FUNC pFunc)
{
	return FALSE;
}

#pragma once

#include "object.h"
#include "../4DyuchiGRX_common/IRenderer.h"
#include "LightTexture.h"
#include "physique.h"

class CMaterialList;

#pragma pack(push,4)

struct	FILE_MESH_HEADER
{
	DWORD				dwMaxVertexNum;
	DWORD				dwVertexNum;
	DWORD				dwOriginalVertexNum;
	DWORD				dwExtVertexNum;
	DWORD				dwTexVertexNum;
	DWORD				dwMtlIndex;
	DWORD				dwFaceGroupNum;
	CMeshFlag			meshFlag;
	DWORD				dwGridIndex;
	VECTOR3				v3Dir;

};
#pragma pack(pop)


class CMeshObject : public CBaseObject , public IGeometryControllerStatic
{	
	DWORD				m_dwMaxVertexNum;
	DWORD				m_dwVertexNum;
	DWORD				m_dwOriginalVertexNum;
	DWORD				m_dwExtVertexNum;
	DWORD				m_dwTexVertexNum;
	DWORD				m_dwMtlIndex;
	DWORD				m_dwFaceGroupNum;
	CMeshFlag			m_meshFlag;
public:
	// �ε��Ҷ� ���⼭���� 16����Ʈ ������ ���ʿ��� �κ��̳� ���� ���ϰ��� ȣȯ���� ���� �ʵ� ����� �����ϰ� ��� �ٸ� �뵵�� ����Ѵ�
	DWORD				m_dwReserved;
	SORT_LINK			m_sortLink;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	VECTOR3*			m_pv3World;
	VECTOR3*			m_pv3Local;
	TVERTEX*			m_ptUV;
	VECTOR3*			m_pv3NormalWorld;
	VECTOR3*			m_pv3NormalLocal;
	VECTOR3*			m_pv3TangentULocal;
	DWORD*				m_pVertexColor;
	CLightTexture*		m_pLightTexture;
	CFaceGroup*			m_pFaceGroup;
	DWORD				m_dwTotalFacesNum;
	DWORD*				m_pdwExtVertexIndex;
	IDIMeshObject*		m_pDeviceMeshObj;
	CPhysique			m_Physique;
	
	MATERIAL			m_mtlDefault;
	
	VECTOR3				m_v3Rect[4];				// for billboard
	float				m_fLengthX;
	float				m_fLengthY;

	// for duplicate...static�� ������ ������ �����..
	COLLISION_MESH_OBJECT_DESC*			m_pColMeshTemp;
	BOOL				m_bShaded;


	
	// collision �𵨵�ũ���� ��Ʈ�� ����Ʈ������ �ڱ� �ڽ��� ������Ʈ ��ũ���ͱ����� �ɼ�
	DWORD				m_dwColMeshObjectDescOffset;	
	

	COLLISION_MODEL_DESC*	m_pColMeshModelDescWorld;	// StaticModeld���� �����ϴ� ������.STM������Ʈ�ΰ�츸 �ش�ȴ�.
	
//	void				SetVertexColor();

	DWORD				SetVertexNormal(VECTOR3* pv3NormalResult,VECTOR3* pv3VertexList,DWORD dwVertexNum);
	DWORD				SetVertexTangentU(VECTOR3* pv3TangentUResult,VECTOR3* pv3VertexList,TVERTEX* ptvList,DWORD dwVertexNum);
	VECTOR3*			CreateVertexList(DWORD dwNum);
	TVERTEX*			CreateTexVertexList(DWORD dwNum);

	BOOL				DuplicateFaceGroup(CMeshObject* pMeshObject);
	
	void				ReleaseVertexList();
	void				ReleaseTexVertexList();
	void				ReleaseFaceGroups();
	
	void				ReleaseTangentUVector();
	void				ReleaseVertexColor();
	void				ReleaseVertexNormal();
	void				ReleaseLightTexture();
	

public:
	void				ReleaseIDIMeshObject();
	void				ReleaseLightTexturePatches();

	BOOL			__stdcall	IsIncludeViewVolume();
	BOOL			__stdcall	OnFault();
	BOOL			__stdcall	OnOut();
	BOOL			__stdcall	IsIncludeViewSphere();

	void				__stdcall	GetTileBuffer(TILE_BUFFER_DESC** ppTileBufferDesc,DWORD* pdwTileBufferDescNum);
	DWORD				__stdcall	GetTileBufferDescNum();
	TILE_BUFFER_DESC*	__stdcall	GetTileBufferDesc(DWORD dwIndex);


	MATERIAL*			GetDefaultMaterial() {return &m_mtlDefault;}

	
	void				SetAlignedViewMatrix(MATRIX4* pMatrixEntry);
	DWORD				ReadVertexColor(void* pFP);

	void				ResetTransform(MATRIX4* pMatrixEntry);
//	void				LocalizeCoord(MATRIX4* pMat);


	

	void				Optimize();
	DWORD				GetFaceGroupNum() {return m_dwFaceGroupNum;}
	BOOL				Duplicate(CMeshObject* pMeshObject,MATRIX4* pObjResult,COLLISION_MESH_OBJECT_DESC* pColMeshObjDesc);
	
	CLightTexture*		GetLightTexture()	{return m_pLightTexture;}
	BOOL				CreateBoundingMesh(COLLISION_MODEL_DESC* pEntryDesc,COLLISION_MESH_OBJECT_DESC* pDesc,COLLISION_MESH_OBJECT_DESC* pDescWorld);
	BOOL				CreateBoundingMesh(COLLISION_MESH_OBJECT_DESC* pDescLocal,COLLISION_MESH_OBJECT_DESC* pDescWorld);
	COLLISION_MESH_OBJECT_DESC*		GetColMeshTemp()	{return m_pColMeshTemp;}



	void				SetGeometry(CoGeometry*	pGeometry) {m_pGeometry = pGeometry;}
	void				SetCollsionModelDescEntry(COLLISION_MODEL_DESC* pColEntry)
	{
		m_pColMeshModelDescWorld = pColEntry;
	}
	CMeshFlag			GetMeshFlag() {return m_meshFlag;}
	BOOL				CommitDevice(IDIMeshObject* pDevObj,CMaterialList* pMtlList);
	BOOL				Initialize(CMaterialList* pMtlList,CoModel* pModel,CoGeometry* pGeometry);
	DWORD				CreateIVertexList(IVERTEX** ppVertex,MATRIX4* pTM);
	void				ReleaseIVertexList(IVERTEX* pv);
	
	
	DWORD				GetTotalFacesNum() {return m_dwTotalFacesNum;}
	TVERTEX*			GetTexVertexPtr() {return m_ptUV;}
	VECTOR3*			GetWorldVertexPtr() {return m_pv3World;}
	VECTOR3*			GetLocalVertexPtr() {return m_pv3Local;}
	CFaceGroup*			GetFaceGroup() {return m_pFaceGroup;}
	void				SetMtlIndex(DWORD index) { m_dwMtlIndex = index;}
	DWORD				GetMtlIndex() {return m_dwMtlIndex;}
	IDIMeshObject*		GetDeviceObject() {return m_pDeviceMeshObj;}
	DWORD				GetVertexNum() {return m_dwVertexNum;}
	BOOL				IsCollisionMeshAndRay(VECTOR3* pv3IntersectPoint,float* pfDist,DWORD* pdwFaceGroupIndex,VECTOR3* pv3Pos,VECTOR3* pv3Dir,MATRIX4* pTM);
	BOOL				IsCollisionMeshAndBoundingBox(VECTOR3* pv3IntersectPoint,float* pfDist,DWORD* pdwFaceGroupIndex,COLLISION_MESH_OBJECT_DESC* pDesc,MATRIX4* pTM);
	
	BOOL				CreateFaceGroup(DWORD dwNum);
	DWORD*				GetVertexColor() {return m_pVertexColor;}

	BOOL				BeginShadeVertexColor(DWORD dwColor);
	void				ShadeVertexColor(VECTOR3* pv3,DWORD dwFacesNum,LIGHT_DESC* pLight,SHADE_FUNC pFunc);
	void				EndShadeVertexColor(FILE* fp);
	void				InitializePhysique();
	COLLISION_MESH_OBJECT_DESC*	GetColMeshObject(COLLISION_MODEL_DESC* pColEntry)
	{
		COLLISION_MESH_OBJECT_DESC* pDesc = 
		(COLLISION_MESH_OBJECT_DESC*)( (DWORD)pColEntry + (DWORD)m_dwColMeshObjectDescOffset );
		return pDesc;
	}	

	DWORD				ReadFile(void* pFP,DWORD dwVersion);
	DWORD				WriteFile(FILE* fp);
//	void				ClearVBCache(I4DyuchiGXRenderer* pRenderer);

	
	BOOL	__stdcall	ApplyResultMatrix(MATRIX4** ppResult,DWORD dwRefIndex,DWORD dwFlag);
	BOOL	__stdcall	ApplyPhysique(BYTE* pVertex,DWORD dwVertexSize,DWORD dwRefIndex,DWORD dwFlag);
	BOOL	__stdcall	ApplyMorph(VECTOR3** ppv3Local,TVERTEX** pptv,DWORD dwRefIndex,DWORD dwFlag);
	BOOL	__stdcall	ApplyView(MATRIX4* pView,DWORD dwRefIndex,DWORD dwFlag);
	BOOL	__stdcall	ApplyMotionStatus(MOTION_STATUS* pMotionStatus,DWORD dwRefIndex,DWORD dwFlag);
	BOOL	__stdcall	ApplyWorldMatrixForPhysique(MATRIX4* pResultWorldMatrix,DWORD dwRefIndex,DWORD dwFlag);

//	BOOL	__stdcall	ApplyVertexNormal(BYTE* pVertex,DWORD dwVertexStartOffset,DWORD dwVertexSize,DWORD dwRefIndex,DWORD dwFlag);
	

	

	CMeshObject();
	~CMeshObject();
};
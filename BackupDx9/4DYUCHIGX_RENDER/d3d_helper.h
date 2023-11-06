#ifndef D3D_HELPER_H
#define D3D_HELPER_H

#include <d3d9.h>
#include "renderer_typedef.h"

class CIndexBuffer;
class CD3DResourceManager;




// �ؽ����� ũ�⸦ ���δ�.
// pSrc�� ���� ����. pDest�� ����� ������. dwWidth�� ���� �ؽ����� ���α���, dwHeight�� ���� �ؽ����� ����,
//	dwScale�� ��� ������(2�� 1/2�� �پ���.)
// dwWidth�� dwHeight�� ������ 2�� n���ΰ� �����Ѵ�. �ƴϸ� ��.
// ��Ʈ ������ 565�� ����.
void ScaleDownTexture565( WORD* pTex, WORD* pDest, DWORD dwWidth, DWORD dwHeight, DWORD dwScale);

// 4444 ����.
void ScaleDownTexture4444( WORD* pTex, WORD* pDest, DWORD dwWidth, DWORD dwHeight, DWORD dwScale);
// 8888 ���� 
void ScaleDownTexture8888(DWORD* pTex, DWORD* pDest, DWORD dwWidth, DWORD dwHeight, DWORD dwScale);




void RestoreLightTexture(LPDIRECT3DTEXTURE9 pLightTexture,TEXTURE_PLANE* pTexPlane,DWORD dwTexPlaneNum);

void SetColorFactorShader(LPDIRECT3DDEVICE9 pDevice,D3DCOLOR color,DWORD dwTexStageIndex );
void RestoreTextureShader(LPDIRECT3DDEVICE9 pDevice,DWORD dwTexStageIndex);


HRESULT SetAdjustedProjectionMatrix( MATRIX4* mat, FLOAT fFOV, FLOAT fAspect,
                                     FLOAT fNearPlane, FLOAT fFarPlane,
                                     FLOAT fPixDx, FLOAT fPixDy, FLOAT fVPWidth, 
                                     FLOAT fVPHeight );
void SetFrustumMatrix( MATRIX4* mat, FLOAT fLeft, FLOAT fRight, FLOAT fTop,
                                     FLOAT fBottom, FLOAT fNearPlane, FLOAT fFarPlane );
void SetHFieldTexMatrix(MATRIX4* pMat,VECTOR3* pv3Rect);
void SetHFieldTexMatrix4(MATRIX4* pMat,VECTOR3* pv3Rect,float fFaceSize,DWORD dwWidthHeight,DWORD dwDirFlag);

void __stdcall EnableAlphaSetting(LPDIRECT3DDEVICE9 pDevice,DWORD dwTexStageIndex,DWORD dwAlpha);
void __stdcall BeginAlphaMeshObject(LPDIRECT3DDEVICE9 pDevice,DWORD dwTexStage,DWORD dwMtlAlpha,DWORD dwObjAlpha);
void __stdcall BeginAlphaMeshObjectPerStageIndex(LPDIRECT3DDEVICE9 pDevice,DWORD dwTexStageIndex,DWORD dwMtlAlpha,DWORD dwObjAlpha);
void __stdcall EndAlphaMeshObject(LPDIRECT3DDEVICE9 pDevice);




BOOL __stdcall CompareLightDesc(LIGHT_DESC* pLight1,LIGHT_DESC* pLight2,float fMinDistance);



class CoD3DDevice;
void OutputErrorMsg(CoD3DDevice* pRenderer,HRESULT hr,char* szMsg);

BOOL DIB24To16(BYTE* pIn, BYTE* pOut, int iWidth, int iHeight);

typedef HRESULT (__stdcall *OnAllocBufferFunc)(CD3DResourceManager* pResourceManager,IDirect3DResource9** ppBuffer,DWORD dwFVF,D3DPOOL pool,DWORD dwUsage,DWORD dwSize);
typedef HRESULT (__stdcall *OnAllocTexture)(CD3DResourceManager* pResourceManager,IDirect3DResource9** ppBuffer,UINT Width,UINT Height,UINT Levels,D3DFORMAT Format,D3DPOOL pool,DWORD dwUsage,DWORD dwSize);


HRESULT __stdcall AllocVBFunc(CD3DResourceManager* pResourceManager,IDirect3DResource9** ppBuffer,DWORD dwFVF,D3DPOOL pool,DWORD dwUsage,DWORD dwSize);
HRESULT __stdcall AllocIBFunc(CD3DResourceManager* pResourceManager,IDirect3DResource9** ppBuffer,DWORD dwFVF,D3DPOOL pool,DWORD dwUsage,DWORD dwSize);

BOOL InitializeShader(LPDIRECT3DDEVICE9 pDevice,SHADER_HANDLE* pHandle,DWORD* pdwDecl,char* szFileName,SHADER_TYPE type);
void ReleaseShader(LPDIRECT3DDEVICE9 pDevice,SHADER_HANDLE* pHandle);

#endif

//void EnableAlphaSetting(LPDIRECT3DDEVICE9 pDevice,DWORD dwTexStageIndex,DWORD dwAlpha);
//void DefaultAlphaSetting(LPDIRECT3DDEVICE9 pDevice,DWORD dwTexStageIndex);
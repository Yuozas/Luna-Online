#ifndef RENDER_TEXTURE_H
#define RENDER_TEXTURE_H

#include "d3d_helper.h"
#include "CTexture.h"
class CoD3DDevice;

#define RENDER_TEXTURE_CLEAR		0x00000001

class CRenderTexture  
{	
	LPDIRECT3DTEXTURE9			m_pTexture;	
	LPDIRECT3DTEXTURE9			m_pDefaultTexture;	

	LPDIRECT3DSURFACE9			m_pSurface;		
	LPDIRECT3DSURFACE9			m_pZBuffer;	
	D3DVIEWPORT9				m_vp;

	LPDIRECT3DSURFACE9			m_pOldSurface;		
	LPDIRECT3DSURFACE9			m_pOldZBuffer;
	
	DWORD						m_dwWidth;
	DWORD						m_dwHeight;
	PRJLIGHT_DESC				m_prjDesc;
	CoD3DDevice*				m_pDevice;

	BOOL						m_bMustUpdate;
	BOOL						m_bEnable;
	
	CRenderTexture**			m_ppOwner;			// �� ��ü�� �������� �����ؽ����� ������
	BOOL*						m_pbUse;			// �� ��ü�� �������� ���������� ��� �÷��� 
	
	void						Cleanup();
public:	
	BOOL						Clear(DWORD dwFlag);

	void						MarkAllocated(CRenderTexture** ppOwner,BOOL* pbUseRenderTextureContainer)
	{
		m_ppOwner = ppOwner;
		m_pbUse = pbUseRenderTextureContainer;
	}
	void						SetMustUpdate(BOOL bMustUpdate)	{m_bMustUpdate = bMustUpdate;}
	void						Enable(BOOL bEnable)	{m_bEnable = bEnable;}
	void						ClearMember();
	
	CTexture					m_Texture;
	PRJLIGHT_DESC*				GetPrjDesc() {return &m_prjDesc;}
	
	LPDIRECT3DTEXTURE9			GetTexture()
	{
		DWORD	dwMask = ~(m_bEnable - 1); 
		DWORD	pTex = 

			(DWORD)( (dwMask) & (DWORD)m_pTexture ) |		// 1�϶� �����ؽ��� ���� ,0�϶� null
			(DWORD)( (~dwMask) & (DWORD)m_pDefaultTexture ) ;	// 1�϶� null, 0�϶� ����Ʈ �ؽ���
		return (LPDIRECT3DTEXTURE9)pTex;
	};
	DWORD						GetWidth() {return m_dwWidth;}
	DWORD						GetHeight() {return m_dwHeight;}

	LPDIRECT3DSURFACE9			GetSurface() {return m_pSurface;}
	BOOL						Create(CoD3DDevice* pDev,DWORD dwWidthHeight);
	
	BOOL						BeginRender(DWORD dwFlag);
	void						EndRender();	
	void						SetViewVolume(VIEW_VOLUME* pVV);
	void						SetMatrixInPipeLine(TRANSFORM_TYPE type);
	void						GetMatrixInPipeLine(MATRIX4* pMat,TRANSFORM_TYPE type);

	
	
	CRenderTexture();
	~CRenderTexture();
};


#endif

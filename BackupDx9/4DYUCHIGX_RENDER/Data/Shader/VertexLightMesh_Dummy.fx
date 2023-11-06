//--------------------------------------------------
//	VertexLightMesh.fx
//	[7/13/2010 LeHide]
//
//	VertexLightMesh용 쉐이더
//
//	Version		:	1.0.2009916
//--------------------------------------------------


///--------------------------------------------------
/// 상수들, 형식선언
///--------------------------------------------------

// 변환행렬
float4x4 matW: WORLD;
float4x4 matV: VIEW;
float4x4 matP: PROJECTION;

// 텍스처
texture	tex0;

// 매트릭스 팔레트
float4x4 matPal[50];

// VLMesh에서 스키닝을 하는가 하지 않는가 플래그
bool VLMesh_IsSkinning;



// 입력정점 형식
struct VS_INPUT
{
    float3 pos		: POSITION;			// Skin Offset Position 0
    float4 normal	: NORMAL0;
    float2 tex		: TEXCOORD0;
    float4 b		: TEXCOORD1;		// Blend Weight
	float4 i		: TEXCOORD2;		// Blend Index
    float3 SP1		: TEXCOORD3;		// Skin Offset Position 1
	float3 SP2		: TEXCOORD4;		// Skin Offset Position 2
	float3 SP3		: TEXCOORD5;		// Skin Offset Position 3
};

// 출력정점 형식
struct VS_OUTPUT
{
    float4 pos		: POSITION;
    float4 normal	: COLOR;
    float2 tex		: TEXCOORD0;
};


///--------------------------------------------------
/// Vertex Shader
///--------------------------------------------------
VS_OUTPUT VS( VS_INPUT In )
{
	// 출력 정점
	VS_OUTPUT Out = (VS_OUTPUT)0;
	


    return Out;
}

///--------------------------------------------------
/// Texture Sampler
///--------------------------------------------------
sampler Sampler = sampler_state
{
    Texture   = (tex0); // g_pd3dDevice->SetTexture( 0, g_pTexture );
    MipFilter = GAUSSIANQUAD; // g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
    MinFilter = GAUSSIANQUAD; // g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    MagFilter = GAUSSIANQUAD; // g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
};


///--------------------------------------------------
/// PixelShader
///--------------------------------------------------
float4 PS( float4 normal : COLOR0, float2 Tex : TEXCOORD0) : COLOR
{
	// 텍스쳐 색
	return tex2D(Sampler, Tex);
}



///--------------------------------------------------
/// Technique
///--------------------------------------------------
technique VLMeshShader
{
    pass P0	// 최초의 0번째 패스
    {
        // 광원은 끈다 
        Lighting       = FALSE;	// g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

        // samplers
        Sampler[0] = (Sampler);

        // texture stages
        ColorOp[0]   = MODULATE; // g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
        ColorArg1[0] = TEXTURE;  // g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
        ColorArg2[0] = DIFFUSE;  // g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
        AlphaOp[0]   = DISABLE;  // g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

        ColorOp[1]   = DISABLE;  // g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
        AlphaOp[1]   = DISABLE;  // g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

	/*
        // shaders
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
	*/
    }
    
    
}

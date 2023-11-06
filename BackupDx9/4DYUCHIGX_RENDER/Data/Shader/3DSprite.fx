//--------------------------------------------------
//	3DSprite.fx
//	[8/11/2010 LeHide]
//
//	3DSprite용 쉐이더
//
//	Version		:	1.0.2010811
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

float TextureAlphaFactor;


// 입력정점 형식
struct VS_INPUT
{
    float4 pos		: POSITION;			// xyzw
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
	
    // matW와 matV행렬을 곱해서 WorldView행렬생성
    float4x4 WorldView = mul(matW, matV);
    
    
    float3 p2 = float3( 0, 0, 0 );
    
    if (VLMesh_IsSkinning == 0)
    {   
		/// 일반 출력
		p2 = In.pos;
	}
	else
	{	
 		/// 하드웨어 스키닝
		 		
		// 임시변수
		float3 p;

		// 첫번째 매트릭스 팔레트연산
		p = mul( float4(In.pos, 1), matPal[In.i.x] );	// p = v * mat[index1]
		p *= In.b.x;									// p = p * weight[index1]
		p2 = p;											// p2 = p2 + p
		
		
		// 두번째 매트릭스 팔레트연산
		float3 l_BoneOffset2 = float3(In.SP1.x, In.SP1.y, In.SP1.z);	
			
		p = mul( float4(l_BoneOffset2, 1), matPal[In.i.y] );
		p *= In.b.y;
		p2 += p;
		
		
		// 세번째 매트릭스 팔레트연산
		float3 l_BoneOffset3 = float3(In.SP2.x, In.SP2.y, In.SP2.z);
		
		p = mul( float4(l_BoneOffset3, 1), matPal[In.i.z] );
		p *= In.b.z;
		p2 += p;
						
		
		// 네번째 매트릭스 팔레트연산
		float3 l_BoneOffset4 = float3(In.SP3.x, In.SP3.y, In.SP3.z);
			
		p = mul( float4(l_BoneOffset4, 1), matPal[In.i.w] );
		p *= In.b.w;
		p2 += p;
		
	}

 
    float3 P = mul(float4(p2, 1),(float4x3)WorldView);		// view공간에서의 위치계산
    Out.pos  = mul(float4(P, 1), matP);						// 투영공간에서의 위치계산
    
	Out.normal = In.normal;	
	Out.tex.x = In.tex.x;
	Out.tex.y = In.tex.y;
	

    return Out;
}

///--------------------------------------------------
/// Texture Sampler
///--------------------------------------------------
sampler Sampler = sampler_state
{
    Texture   = (tex0); // g_pd3dDevice->SetTexture( 0, g_pTexture );
    MipFilter = LINEAR; // g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
    MinFilter = LINEAR; // g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    MagFilter = LINEAR; // g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

    AddressU = Wrap;
    AddressV = Wrap;
};

/*
sampler Sampler1 = sampler_state
{
    Texture   = (tex0); // g_pd3dDevice->SetTexture( 0, g_pTexture );
    MipFilter = LINEAR; // g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
    MinFilter = LINEAR; // g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    MagFilter = LINEAR; // g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

    AddressU = Wrap;
    AddressV = Wrap;
};
*/


///--------------------------------------------------
/// PixelShader
///--------------------------------------------------
float4 PS( float4 normal : COLOR0, float2 Tex : TEXCOORD0) : COLOR
{
	// 텍스쳐 색
	//return tex2D(Sampler, Tex);
	
	// 테스트용 - 텍스쳐 색 + 노말 값
	//return (tex2D(Sampler, Tex) + normal)/2;

	// 테스트용 - 노말값
	//return normal;
	
	// 스키닝을 타지 않는 녀석들 좀 세어보자
	/*
	if (VLMesh_IsSkinning == 0)
    	{   
		// 테스트용 - 회색 처리
		float4 l_Gray;
		l_Gray = tex2D(Sampler, Tex);
 		l_Gray.g = l_Gray.r;
 		l_Gray.b = l_Gray.r;
 		l_Gray.a = l_Gray.a;
		
		return l_Gray;
	}
	*/


	// 텍스쳐 색
	//return tex2D(Sampler, Tex);

	
	// 알파 비율 값을 적용
	float4 l_FinalColor = tex2D(Sampler, Tex);
	l_FinalColor.a = l_FinalColor.a * TextureAlphaFactor;
	return l_FinalColor;
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
		//AlphaBlendEnable = true;
 		//SrcBlend = SrcAlpha;
 		//DestBlend = InvSrcAlpha;
 		//TextureFactor = TextureAlphaFactor;
 		
        //ColorOp[0]		= MODULATE;
        //ColorArg1[0]	= TEXTURE;
        //ColorArg2[0]	= DIFFUSE;
        //AlphaArg1[0]	= TEXTURE;
        //AlphaArg2[0]	= TFACTOR;
		//AlphaOp[0]		= MODULATE;
		
 		//ColorOp[1]   = MODULATE;	// g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_MODULATE );
        //ColorArg1[1] = TEXTURE;		// g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
        //ColorArg2[1] = DIFFUSE;		// g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
        //AlphaArg1[1] = TEXTURE;
        //AlphaArg2[1] = TFACTOR;		// pDevice->SetTextureStageState( i, D3DTSS_ALPHAARG2, D3DTA_TFACTOR );
        //AlphaOp[1]   = MODULATE;	// g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );


        // shaders
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
    }
    
    
}

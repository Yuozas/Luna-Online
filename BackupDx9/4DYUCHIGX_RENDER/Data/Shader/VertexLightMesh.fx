//--------------------------------------------------
//	VertexLightMesh.fx
//	[7/13/2010 LeHide]
//
//	VertexLightMesh�� ���̴�
//
//	Version		:	1.0.2009916
//--------------------------------------------------


///--------------------------------------------------
/// �����, ���ļ���
///--------------------------------------------------

// ��ȯ���
float4x4 matW: WORLD;
float4x4 matV: VIEW;
float4x4 matP: PROJECTION;

// ��Ʈ���� �ȷ�Ʈ
float4x4 matPal[50];

// VLMesh���� ��Ű���� �ϴ°� ���� �ʴ°� �÷���
bool VLMesh_IsSkinning;

// �ؽ�ó
texture	tex0;

// ��ü���� ���ĸ� �����ϴ� Factor
float TextureAlphaFactor;

// ��Ʈ����
float4 l_Ambient;
float4 l_Diffuse;


// �Է����� ����
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

// ������� ����
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
	// ��� ����
	VS_OUTPUT Out = (VS_OUTPUT)0;
	
    // matW�� matV����� ���ؼ� WorldView��Ļ���
    float4x4 WorldView = mul(matW, matV);
    
    
    float3 p2 = float3( 0, 0, 0 );
    
    if (VLMesh_IsSkinning == 0)
    {   
		/// �Ϲ� ���
		p2 = In.pos;
	}
	else
	{	
 		/// �ϵ���� ��Ű��
		 		
		// �ӽú���
		float3 p;

		// ù��° ��Ʈ���� �ȷ�Ʈ����
		p = mul( float4(In.pos, 1), matPal[In.i.x] );	// p = v * mat[index1]
		p *= In.b.x;									// p = p * weight[index1]
		p2 = p;											// p2 = p2 + p
		
		
		// �ι�° ��Ʈ���� �ȷ�Ʈ����
		float3 l_BoneOffset2 = float3(In.SP1.x, In.SP1.y, In.SP1.z);	
			
		p = mul( float4(l_BoneOffset2, 1), matPal[In.i.y] );
		p *= In.b.y;
		p2 += p;
		
		
		// ����° ��Ʈ���� �ȷ�Ʈ����
		float3 l_BoneOffset3 = float3(In.SP2.x, In.SP2.y, In.SP2.z);
		
		p = mul( float4(l_BoneOffset3, 1), matPal[In.i.z] );
		p *= In.b.z;
		p2 += p;
						
		
		// �׹�° ��Ʈ���� �ȷ�Ʈ����
		float3 l_BoneOffset4 = float3(In.SP3.x, In.SP3.y, In.SP3.z);
			
		p = mul( float4(l_BoneOffset4, 1), matPal[In.i.w] );
		p *= In.b.w;
		p2 += p;
		
	}

 
    float3 P = mul(float4(p2, 1),(float4x3)WorldView);		// view���������� ��ġ���
    Out.pos  = mul(float4(P, 1), matP);						// �������������� ��ġ���
    
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
    Texture		= (tex0);			// g_pd3dDevice->SetTexture( 0, g_pTexture );
    MipFilter	= ANISOTROPIC;		// g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC );
    MinFilter	= ANISOTROPIC;		// g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC );
    MagFilter	= ANISOTROPIC;		// g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC );

    AddressU	= Wrap;
    AddressV	= Wrap;
   
};


///--------------------------------------------------
/// PixelShader
///--------------------------------------------------
float4 PS( float4 normal : COLOR0, float2 Tex : TEXCOORD0) : COLOR
{
	// �ܼ� �ؽ��� ��
	//return tex2D(Sampler, Tex);
	
	// �ؽ��� �� + �븻 ��
	//return (tex2D(Sampler, Tex) + normal)/2;

	// �븻��
	//return normal;
	
	// ��Ű���� Ÿ�� �ʴ� �༮�� �� �����
	/*
	if (VLMesh_IsSkinning == 0)
    	{   
		// �׽�Ʈ�� - ȸ�� ó��
		float4 l_Gray;
		l_Gray = tex2D(Sampler, Tex);
 		l_Gray.g = l_Gray.r;
 		l_Gray.b = l_Gray.r;
 		l_Gray.a = l_Gray.a;
		
		return l_Gray;
	}
	*/


	// �ؽ��� ��, Diffuse, ���� ���� ��
	float4 l_FinalColor = tex2D(Sampler, Tex);
	l_FinalColor.r *= l_Diffuse.r;
	l_FinalColor.g *= l_Diffuse.g;	
	l_FinalColor.b *= l_Diffuse.b;
	l_FinalColor.a = l_FinalColor.a * TextureAlphaFactor;
	return l_FinalColor;

/*
	// �ؽ��� �� * ���� ���� ��
	float4 l_FinalColor = tex2D(Sampler, Tex);
	l_FinalColor.a = l_FinalColor.a * TextureAlphaFactor;
	return l_FinalColor;
*/
	
	/*
			// �׽�Ʈ�� - ȸ�� ó��
		float4 l_Gray;
		l_Gray = tex2D(Sampler, Tex);
		l_Gray.r = l_Gray.a;
 		l_Gray.g = l_Gray.a;
 		l_Gray.b = l_Gray.a;
 		l_Gray.a = l_Gray.a;

		
		return l_Gray;
		 		*/

}



///--------------------------------------------------
/// Technique
///--------------------------------------------------
technique VLMeshShader
{
    pass P0	// ������ 0��° �н�
    {
        // ������ ���� 
        Lighting       = FALSE;	// g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

        // samplers
        Sampler[0] = (Sampler);
        
   /*
        // texture stages
		AlphaBlendEnable	= true;
		
		BlendOp				= ADD;
 		SrcBlend			= SRCALPHA;
 		DestBlend			= ONE;
 		TextureFactor		= TextureAlphaFactor;
 		
        ColorOp[0]			= MODULATE;
 		ColorOp[1]			= MODULATE;
 		
 
 		   
 		//AlphaArg2[0]		= TFACTOR;
		//AlphaOp[0]		= MODULATE;
		
			 	
		//AlphaArg1[0]		= TEXTURE;
		//AlphaOp[0]		= SELECTARG1;
		
		*/
 		
        // shaders
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
    }
    
}


//float, float2, float3, float4 == vector
//float1x3, float2x3, float3x3, float4x4 = matrix
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture g_Texture;
textureCUBE g_CubeTexture;

#define MAX_LIGHTS 8 // ���̴����� ó���� �ִ� ����Ʈ �� (�ʿ信 ���� ����)

struct Light
{
    int Type; // 0: Directional, 1: Point, 2: Spot
    float3 Position; // ���� ���� ��ġ (Point/Spot)
    float3 Direction; // ���� ���� ���� (Directional/Spot)
    float4 Color; // ����Ʈ ���� �� ����
    float Range; // ���� ���� (Point/Spot)
    float Attenuation0; // ���� ����� (Constant Attenuation)
    float Attenuation1; // ���� ������ (Linear Attenuation)
    float Attenuation2; // ���� ������ (Quadratic Attenuation)
    // --- ����Ʈ����Ʈ�� �߰� ���� (�ʿ��) ---
    // float SpotFalloff; // ����Ʈ����Ʈ ���� ���� (Power)
    // float SpotCosInnerAngle; // ����Ʈ����Ʈ ���� �� �ڻ��ΰ�
    // float SpotCosOuterAngle; // ����Ʈ����Ʈ �ܺ� �� �ڻ��ΰ�
};

struct Material
{
    float4  Diffuse;    // ������ Ȯ�걤 �ݻ��� (�ؽ�ó�� ������)
    float4  Ambient;    // ������ �ֺ��� �ݻ���
    float4  Specular;   // ������ ���ݻ籤 ����
    float4  Emissive;   // ������ ��ü �߱� ����
    float   Power;      // ������ ���ݻ� ���� (Shininess)
};

Material g_Material : MATERIAL;
Light g_Lights[MAX_LIGHTS] : LIGHTS; // ����Ʈ ���� �迭
int g_NumActiveLights; // ���� ��ü�� ������ �ִ� Ȱ�� ����Ʈ ���� (C++���� ����)

// �� ���� ���� ���� �߰� (C++���� ���� �ʿ�)
float3 g_LightDirection = normalize(float3(-1.f, -1.f, -1.f)); // ����: ���� ���� (ī�޶� ����?)
float4 g_LightColor = float4(1.f, 1.f, 1.f, 1.f); // ����: ���� ����
float4 g_AmbientLightColor = float4(0.2f, 0.2f, 0.2f, 1.f); // ����: �ֺ��� ����

// ī�޶� ��ġ (Specular ��꿡 �ʿ�, C++���� ����)
float3 g_CameraPosition;

// <<< ���� �Ӽ� ���� ���� �߰� (C++���� ����) >>>
float4 g_MaterialAmbient = float4(0.2f, 0.2f, 0.2f, 1.f); // ������ �ֺ��� �ݻ���
float4 g_MaterialDiffuse = float4(1.f, 1.f, 1.f, 1.f); // ������ Ȯ�걤 �ݻ��� (�ؽ�ó�� ������)
float4 g_MaterialSpecular = float4(1.f, 1.f, 1.f, 1.f); // ������ ���ݻ籤 ����
float  g_MaterialSpecularPower = 32.f; // ������ ���ݻ� ���� (Shininess)
float4 g_MaterialEmissive = float4(0.f, 0.f, 0.f, 1.f); // ������ ��ü �߱� ����

sampler CubeSampler = sampler_state
{
    texture = g_CubeTexture;
    minfilter = linear;
    magfilter = linear;
    mipfilter = linear;
};

sampler DefaultSampler = sampler_state
{
    texture = g_Texture;
    minfilter = linear;
    magfilter = linear;
    mipfilter = linear;
};

//VertexBuffer = ����0 ����1 ����2 ����3
//IndexBuffer = 0 1 2 0 2 3 


struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL0;
    float2 vTexcoord : TEXCOORD0;
};

// VS -> PS �� ������ ������ ����ü (World Normal �߰�)
struct VS_OUT
{
    float4 vPosition : POSITION; // Ŭ�� ���� ��ġ
    float2 vTexcoord : TEXCOORD0; // �ؽ�ó ��ǥ
    float4 vWorldPos : TEXCOORD1; // ���� ���� ��ġ (�ʿ�� ���)
    float3 vNormal : TEXCOORD2; // ���� ���� ���� ���� �߰�!
};


/* ������ ��ȯ(����, ��, ����) */
/* ������ ������ �ٲ۴�.  */
VS_OUT VS_MAIN(VS_IN In)      
{
    VS_OUT Out;

    // ���� ��ȯ (World Position ���)
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);

    // ���� Ŭ�� ���� ��ġ ��� (World * View * Projection)
    Out.vPosition = mul(Out.vWorldPos, g_ViewMatrix);
    Out.vPosition = mul(Out.vPosition, g_ProjMatrix);

    // �ؽ�ó ��ǥ ����
    Out.vTexcoord = In.vTexcoord;

    // ���� ���͸� ���� �������� ��ȯ�Ͽ� ����
    // WorldMatrix�� non-uniform scaling�� ���ٸ� �̰����� ���
    // ���� �ִٸ�, WorldInverseTranspose ����� ����ؾ� ��
    Out.vNormal = normalize(mul(In.vNormal, (float3x3) g_WorldMatrix));

    return Out;
}

VS_OUT VS_ORTHO(VS_IN In)
{
    VS_OUT Out;

    // ���� ��ȯ (World Position ���)
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);

    // ���� Ŭ�� ���� ��ġ ��� (World * View * Projection)
    Out.vPosition = mul(Out.vWorldPos, g_ViewMatrix);
    Out.vPosition = mul(Out.vPosition, g_ProjMatrix);

    // �ؽ�ó ��ǥ ����
    Out.vTexcoord = In.vTexcoord;

    // ���� ���͸� ���� �������� ��ȯ�Ͽ� ����
    // WorldMatrix�� non-uniform scaling�� ���ٸ� �̰����� ���
    // ���� �ִٸ�, WorldInverseTranspose ����� ����ؾ� ��
    Out.vNormal = normalize(mul(In.vNormal, (float3x3) g_WorldMatrix));

    return Out;
}

/* ����3���� ���´� */
/* w�����⿬��. */
/* ����Ʈ��ȯ */
/* �����Ͷ����� -> �ȼ��������. */

struct PS_IN
{
    float4 vPosition : POSITION; // Ŭ�� ���� ��ġ
    float2 vTexcoord : TEXCOORD0; // �ؽ�ó ��ǥ
    float4 vWorldPos : TEXCOORD1; // ���� ���� ��ġ (�ʿ�� ���)
    float3 vNormal : TEXCOORD2; // ���� ���� ���� ���� �߰�!
};

struct PS_OUT
{
    vector vColor : COLOR0;
};

/* �ȼ��� ���� �����Ѵ�. */
// vector PS_MAIN(PS_IN In) : COLOR0


PS_OUT PS_MAIN(PS_IN In, float facing : VFACE)
{
    PS_OUT Out;

    // 1. ���� ���� ���� ���� (VFACE ���)
    float3 normal = normalize(In.vNormal * sign(facing));

    // 2. �ü� ���� ��� (ǥ�� -> ī�޶�)
    float3 viewDir = normalize(g_CameraPosition - In.vWorldPos.xyz);

    // 3. ����Ʈ ���� ��� (ǥ�� -> ����)
    float3 lightVec = -g_LightDirection; // g_LightDirection�� �������� ������ ������ ���

    // 4. �ֺ���(Ambient) ���
    float4 ambient = g_AmbientLightColor * g_Material.Ambient;

    // 5. Ȯ�걤(Diffuse) ���
    float NdotL = saturate(dot(normal, lightVec));
    float4 diffuse = NdotL * g_LightColor * g_Material.Diffuse;

    // 6. ���ݻ籤(Specular) ��� (Blinn-Phong �� ���)
    float3 halfwayDir = normalize(lightVec + viewDir); // ���� ����
    float NdotH = saturate(dot(normal, halfwayDir));
    float specPower = pow(NdotH, g_Material.Power); // ���̶���Ʈ ���ߵ�
    float4 specular = specPower * g_LightColor * g_Material.Specular;

    // 7. �ؽ�ó ���� ��������
    float4 baseColor = tex2D(DefaultSampler, In.vTexcoord);

    // 8. ���� ���� ����
    // �������� = ��ü�߱� + �ֺ��� + (Ȯ�걤 * �ؽ�ó) + ���ݻ籤
    Out.vColor.rgb = g_Material.Emissive.rgb + ambient.rgb + (diffuse.rgb * baseColor.rgb) + specular.rgb;

    // 9. ���� �� ó�� (�ؽ�ó ���� * ���� Ȯ�걤 ����)
    Out.vColor.a = baseColor.a * g_Material.Diffuse.a;

    // --- �ʿ��ϴٸ� ���� ���� ó�� ���� ���� ---
    // if (Out.vColor.a < 0.1f)
    //    discard;
    // Out.vColor.a = 0.5f;
    // ---

    return Out;
}

// ���ο� 'Unlit' �ȼ� ���̴�
PS_OUT PS_UNLIT(PS_IN In, float facing : VFACE) // VFACE�� ��� ������ �� �ʿ��� �� ����
{
    PS_OUT Out;

    // 1. �ؽ�ó ���� ��������
    float4 baseColor = tex2D(DefaultSampler, In.vTexcoord);

    // 2. (���� ����) ������ Diffuse ������ ���Ͽ� �⺻ ���� ����
    // baseColor *= g_Material.Diffuse; // Material ����ü ���

    // 3. ���� �������� �ؽ�ó ����(�Ǵ� ������ ����) �ٷ� ���
    Out.vColor = baseColor;

    // 4. ���� �� ó�� (���� PS_MAIN�� �����ϰ� ó���ϰų� �ʿ信 �°� ����)
    //    �ؽ�ó ���Ŀ� ���� ���ĸ� ���ϴ� ��� ����
    Out.vColor.a = baseColor.a * g_Material.Diffuse.a;

    // 5. (���� ����) ���� ���� ���� discard ���� (�ʿ� ��)
    // if (Out.vColor.a < 0.1f)
    //    discard;

    return Out;
}

PS_OUT PS_MAIN_RED(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = vector(1.f, 0.f, 0.f, 1.f);
        
    return Out;
}



technique DefaultTechnique
{
    pass DefaultPass
    {

        aLPHAbLENDeNABLE = True;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        BlendOp = Add;

        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_UNLIT();
    }

    pass DoubleSidedPlane
    {
        // --- ���� ���� ���� ---
        // ��� �������� ���� �ø� ��Ȱ��ȭ
        CullMode = None;

        // ���� ���� ���� (���� �ڵ� ����, ��Ÿ ����)
        AlphaBlendEnable = True; // aLPHAbLENDeNABLE -> AlphaBlendEnable
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        BlendOp = Add;

        // --- ���̴� ���� ---
        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_MAIN(); // �� ����� ���Ե� PS_MAIN ���
    }

    pass DoubleSidedPlane
    {
        // --- ���� ���� ���� ---
        // ��� �������� ���� �ø� ��Ȱ��ȭ
        CullMode = None;

        // ���� ���� ���� (���� �ڵ� ����, ��Ÿ ����)
        AlphaBlendEnable = True; // aLPHAbLENDeNABLE -> AlphaBlendEnable
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        BlendOp = Add;

        // --- ���̴� ���� ---
        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_MAIN(); // �� ����� ���Ե� PS_MAIN ���
    }

    pass asdouble
    {
        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_MAIN_RED();
    }
}

// ī�޶� ��ġ (Specular ��꿡 �ʿ�, C++���� ����)
float3 g_CameraPosition;

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture g_Texture;
textureCUBE g_CubeTexture;

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

    AddressU = Wrap;
    AddressV = Wrap;
};

#define MAX_LIGHTS 8 // ���̴����� ó���� �ִ� ����Ʈ �� (�ʿ信 ���� ����)

struct Light
{
    int     Type; // 0: UnUsed 1: Point, 2: Spot, 3: Direction
    float3 _unused_;
    float4  Color; // ����Ʈ ���� �� ����
    float4  Position; // ���� ���� ��ġ (Point/Spot)
    float4  Direction; // ���� ���� ���� (Directional/Spot)
    float   Range; // ���� ���� (Point/Spot)
    float   Attenuation0; // ���� ����� (Constant Attenuation)
    float   Attenuation1; // ���� ������ (Linear Attenuation)
    float   Attenuation2; // ���� ������ (Quadratic Attenuation)
    // --- ����Ʈ����Ʈ�� �߰� ���� (�ʿ��) ---
    // float SpotFalloff; // ����Ʈ����Ʈ ���� ���� (Power)
    // float SpotCosInnerAngle; // ����Ʈ����Ʈ ���� �� �ڻ��ΰ�
    // float SpotCosOuterAngle; // ����Ʈ����Ʈ �ܺ� �� �ڻ��ΰ�
};
Light g_LightTest : LIGHT;
Light g_Lights[MAX_LIGHTS] : LIGHTS; // ����Ʈ ���� �迭
int g_NumActiveLights; // ���� ��ü�� ������ �ִ� Ȱ�� ����Ʈ ���� (C++���� ����)

struct Material
{
    float4 Diffuse; // ������ Ȯ�걤 �ݻ��� (�ؽ�ó�� ������)
    float4 Ambient; // ������ �ֺ��� �ݻ���
    float4 Specular; // ������ ���ݻ籤 ����
    float4 Emissive; // ������ ��ü �߱� ����
    float Power; // ������ ���ݻ� ���� (Shininess)
};

Material g_Material : MATERIAL;

// �� ���� ���� ���� �߰� (C++���� ���� �ʿ�)
float3 g_LightPosition = float3(0.8f, 1.0f, 8.f);
float3 g_LightDirection = normalize(float3(0.f, -1.f, 0.f)); // ����: ���� ���� (ī�޶� ����?)
float4 g_LightColor = float4(0.3f, 0.3f, 0.3f, 1.f); // ����: ���� ����
float4 g_AmbientLightColor = float4(0.5f, 0.5f, 0.5f, 1.f); // ����: �ֺ��� ����

// <<< ���� �Ӽ� ���� ���� �߰� (C++���� ����) >>>
float4 g_MaterialAmbient = float4(0.2f, 0.2f, 0.2f, 1.f); // ������ �ֺ��� �ݻ���
float4 g_MaterialDiffuse = float4(1.f, 1.f, 1.f, 1.f); // ������ Ȯ�걤 �ݻ��� (�ؽ�ó�� ������)
float4 g_MaterialSpecular = float4(1.f, 1.f, 1.f, 1.f); // ������ ���ݻ籤 ����
float g_MaterialSpecularPower = 32.f; // ������ ���ݻ� ���� (Shininess)
float4 g_MaterialEmissive = float4(0.f, 0.f, 0.f, 1.f); // ������ ��ü �߱� ����


// Ÿ�ϸ� �������� �߰�
float2 g_ScaleFactor    = float2(1.0f, 1.0f);
float2 g_OffsetFactor   = float2(0.0f, 0.0f);

// --- �Ȱ� ȿ�� ���� ���� �߰� (C++���� ����) ---
float3 g_FogColor = float3(0.5f, 0.9f, 0.9f); // �Ȱ� ���� (�Ͼ��)
float g_FogStart = 8.0f; // �Ȱ� ���� �Ÿ�
float g_FogEnd = 20.0f; // �Ȱ� �� �Ÿ�

struct VS_IN
{
    float3 vPosition    : POSITION;
    float3 vNormal      : NORMAL0;
    float2 vTexcoord    : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition    :  POSITION;  // Ŭ�� ���� ��ġ
    float2 vTexcoord    :  TEXCOORD0; // �ؽ�ó ��ǥ
    float4 vWorldPos    :  TEXCOORD1; // ���� ���� ��ġ (�ʿ�� ���)
    float3 vNormal      :  TEXCOORD2; // ���� ���� ���� ���� �߰�!
    float3 vViewPos     :  TEXCOORD3; // �� ���� ��ġ (�Ȱ� ����) <-- �߰�
    float3 vScale       :  TEXCOORD4;
};


VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;

    // ���� ��ȯ (World Position ���)
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    
    float3 tmpX = g_WorldMatrix._11_12_13;
    float3 tmpY = g_WorldMatrix._21_22_23;
    float3 tmpZ = g_WorldMatrix._31_32_33;
    
    float3 scale = { length(tmpX), length(tmpY), length(tmpZ) };
    
    Out.vScale = scale;
    
    // �� ���� ��ġ ��� (World * View) <-- �߰�
    Out.vViewPos = mul(Out.vWorldPos, g_ViewMatrix).xyz;

    // ���� Ŭ�� ���� ��ġ ��� (World * View * Projection)
    //Out.vPosition = mul(Out.vWorldPos, g_ViewMatrix);
    Out.vPosition = mul(float4(Out.vViewPos, 1.f), g_ProjMatrix);

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
    // �� ���� ��ġ ��� (World * View) <-- �߰�
    Out.vViewPos = mul(Out.vWorldPos, g_ViewMatrix).xyz;

    // ���� Ŭ�� ���� ��ġ ��� (World * View * Projection)
    //Out.vPosition = mul(Out.vWorldPos, g_ViewMatrix);
    //Out.vPosition = mul(Out.vPosition, g_ProjMatrix);
    Out.vPosition = mul(float4(Out.vViewPos, 1.f), g_ProjMatrix); // vViewPos ����ϵ��� ����
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
    float3 vViewPos : TEXCOORD3; // �� ���� ��ġ (�Ȱ� ����) <-- �߰�
    float3 vScale : TEXCOORD4;
};

struct PS_OUT
{
    vector vColor : COLOR0;
};

/* �ȼ��� ���� �����Ѵ�. */
PS_OUT PS_LIT(PS_IN In, float facing : VFACE)
{
    PS_OUT Out;

    // --- ���� ��꿡 �ʿ��� ���͵� ��� ---
    float3 normal = normalize(In.vNormal * sign(facing));
    float3 viewDir = normalize(g_CameraPosition - In.vWorldPos.xyz);
    //float3 lightVec = -g_LightDirection; // ����Ʈ ����

    // --- �ؽ�ó ���ø� ---
    float2 tiledUV = In.vTexcoord * g_ScaleFactor + g_OffsetFactor;
    float4 baseColor = tex2D(DefaultSampler, tiledUV);

    // --- ���� ��� ��� ---

    // --- ���� ���� ����� ���� ���� �ʱ�ȭ ---
    // 1. �⺻ ���� ��� (Ambient + Emissive)
    float3 accumulatedColor = (baseColor.rgb * g_Material.Ambient.rgb * g_Material.Diffuse.rgb) + g_Material.Emissive.rgb;
    
    // --- ����(Global) ���⼺ ����Ʈ ��� �߰� ---
    /*float3 globalLightVec = normalize(-g_LightDirection); // ����Ʈ�� ���ϴ� ����
    float NdotL_global = saturate(dot(normal, globalLightVec)); // ������ ����Ʈ ���� ����

    if (NdotL_global > 0.0f) // ����Ʈ�� �޴� �κи� ���
    {
        // ���� ����Ʈ Diffuse ���
        float3 globalDiffuse = baseColor.rgb * g_Material.Diffuse.rgb * NdotL_global * g_LightColor.rgb;

        // ���� ����Ʈ Specular ���
        float3 halfwayDir_global = normalize(globalLightVec + viewDir);
        float NdotH_global = saturate(dot(normal, halfwayDir_global));
        float specPower_global = pow(NdotH_global, g_Material.Power); // ������ Power ���
        float3 globalSpecular = g_Material.Specular.rgb * specPower_global * g_LightColor.rgb;

        // ���� ����Ʈ �⿩�� ���� (���⼺ ����Ʈ�� ���� ����)
        accumulatedColor += (globalDiffuse + globalSpecular);
    }*/
    // --- ���� ����Ʈ ��� �� ---

    // 2. �� ����Ʈ ���Կ� ���� �⿩�� ��� �� ����
    for (int i = 0; i < g_NumActiveLights; ++i)
    {
        
        // ���� ó���� ����Ʈ ���� ��������
        Light currentLight = g_Lights[i];

        // --- Ÿ�� Ȯ��: 0��(LT_UNUSED)�̸� ��� �ǳʶٱ� ---
        if (currentLight.Type == 0) // LT_UNUSED
        {
            continue; // ���� ����Ʈ�� �Ѿ
        }
        
        

        // ���� ����Ʈ ��꿡 �ʿ��� ���� �ʱ�ȭ
        float3 lightVec = float3(0.0f, 0.0f, 0.0f); // �ȼ����� ����Ʈ�� ���ϴ� ����ȭ�� ����
        float attenuation = 0.0f; // ����Ʈ ���� �� (0.0 ~ 1.0)
        float NdotL = 0.0f; // ������ ����Ʈ ���� ���� ��

        // ����Ʈ Ÿ�Ժ� ��� (Ÿ�� ��ȣ �����!)
        if (currentLight.Type == 3) // Directional Light (LT_DIR = 3)
        {
            lightVec = normalize(-currentLight.Direction).xyz;
            NdotL = saturate(dot(normal, lightVec));
            attenuation = 1.0f; // ���⼺�� ���� ����
        }
        else if (currentLight.Type == 1) // Point Light (LT_POINT = 1)
        {
            float3 dirToLight = currentLight.Position.xyz - In.vWorldPos.xyz;
            float distSq = dot(dirToLight, dirToLight);
            float dist = sqrt(distSq);

            if (dist < currentLight.Range)
            {
                lightVec = dirToLight / dist;
                NdotL = saturate(dot(normal, lightVec));
                attenuation = saturate(1.0f / (currentLight.Attenuation0 +
                                                currentLight.Attenuation1 * dist +
                                                currentLight.Attenuation2 * distSq));
            }
            else
            {
                NdotL = 0.0f;
                attenuation = 0.0f;
            }
        }
        else if (currentLight.Type == 2) // Spot Light (LT_SPOT = 2)
        {
            // ����Ʈ����Ʈ ��� (����� ����Ʈ ����Ʈ�� �����ϰ� ó��, ���� Ȯ�� ����)
            float3 dirToLight = currentLight.Position.xyz - In.vWorldPos.xyz;
            float distSq = dot(dirToLight, dirToLight);
            float dist = sqrt(distSq);

            if (dist < currentLight.Range)
            {
                lightVec = dirToLight / dist;
                NdotL = saturate(dot(normal, lightVec));
                attenuation = saturate(1.0f / (currentLight.Attenuation0 +
                                                currentLight.Attenuation1 * dist +
                                                currentLight.Attenuation2 * distSq));

                // TODO: ���⿡ ����Ʈ����Ʈ ���԰�/���� ��� �߰�
                // float spotFactor = ... ��� ...
                // attenuation *= spotFactor;
            }
            else
            {
                NdotL = 0.0f;
                attenuation = 0.0f;
            }
        } // End of light type branching
        
        

        // ���� ����Ʈ�� �⿩���� ��ȿ�ϴٸ� (NdotL > 0, attenuation > 0)
        // (Type 0�� ������ continue�� �ɷ������Ƿ� ���⼭�� 1, 2, 3 Ÿ�Ը� �����)
        
        if (NdotL > 0.0f && attenuation > 0.0f)
        {
            
            // ��ǻ��(Diffuse) ���
            float3 diffuse = baseColor.rgb * g_Material.Diffuse.rgb * NdotL * currentLight.Color.rgb;

            // ����ŧ��(Specular) ���
            float3 halfwayDir = normalize(lightVec + viewDir);
            float NdotH = saturate(dot(normal, halfwayDir));
            float specPower = pow(NdotH, g_Material.Power);
            float3 specular = g_Material.Specular.rgb * specPower * currentLight.Color.rgb;

            // ���� ����Ʈ�� ���� �⿩�� = (��ǻ�� + ����ŧ��) * ����
            float3 currentLightContribution = (diffuse + specular) * attenuation;

            // ���� ���� ���� ����Ʈ �⿩�� ����
            accumulatedColor += currentLightContribution;
        }

    } // End of for loop*/


    // --- ���� ���� ���� �� ��ó�� ---
    float4 litColor;
    litColor.rgb = saturate(accumulatedColor);
    litColor.a = baseColor.a * g_Material.Diffuse.a; // ���� ��

    // --- �Ȱ� ȿ�� ��� ---
    float distance = length(In.vViewPos);
    float fogFactor = saturate((distance - g_FogStart) / (g_FogEnd - g_FogStart));
    
    // === ���ο� Additive ��� ===
    // 1. �Ȱ� �⿩�� ��� (�Ȱ� ���� * �Ȱ� ����)
    float3 fogContribution = g_FogColor * fogFactor;
    // 2. ���� ���� ���� �Ȱ� �⿩���� ���� (saturate�� 0~1 ���� ����)
    Out.vColor = lerp(litColor, float4(g_FogColor, litColor.a), fogFactor);
    //Out.vColor.rgb = saturate(litColor.rgb + fogContribution);
    // 3. ���� ���� ���� ���� �� ����
    Out.vColor.a = litColor.a;

    // --- ���� �׽�Ʈ ---
    if (Out.vColor.a < 0.8f) discard;

    return Out;
}

// ���ο� 'Unlit' �ȼ� ���̴�
PS_OUT PS_WEAPON(PS_IN In, float facing : VFACE) // VFACE�� ��� ������ �� �ʿ��� �� ����
{
    PS_OUT Out;

    // 1. �ؽ�ó ���� ��������
    float2 tiledUV = In.vTexcoord * g_ScaleFactor + g_OffsetFactor;
    float4 baseColor = tex2D(DefaultSampler, tiledUV);

    // 2. (���� ����) ������ Diffuse ������ ���Ͽ� �⺻ ���� ����
    baseColor *= g_Material.Diffuse; // Material ����ü ��� ��
    Out.vColor = baseColor;

    // 4. ���� �� ó�� (�ؽ�ó ���Ŀ� ���� ���� ���ϱ� ��)
    // Out.vColor.a = baseColor.a * g_Material.Diffuse.a; // ���� Diffuse�� ���ߴٸ�

    // 5. (���� ����) ���� ���� ���� discard ����
    if (Out.vColor.a < 0.2f)
        discard;

    return Out;
}

// ���ο� 'Unlit' �ȼ� ���̴�
PS_OUT PS_UNLIT(PS_IN In, float facing : VFACE) // VFACE�� ��� ������ �� �ʿ��� �� ����
{
    PS_OUT Out;

    // 1. �ؽ�ó ���� ��������
    float2 tiledUV = In.vTexcoord * g_ScaleFactor + g_OffsetFactor;
    float4 baseColor = tex2D(DefaultSampler, tiledUV);

    // 2. (���� ����) ������ Diffuse ������ ���Ͽ� �⺻ ���� ����
    baseColor *= g_Material.Diffuse; // Material ����ü ��� ��

    // --- �Ȱ� ȿ�� ��� ���� ---
    float distance = length(In.vViewPos); // ������ �� ���� ��ġ ���
    float fogFactor = saturate((distance - g_FogStart) / (g_FogEnd - g_FogStart));

    // 3. �ؽ�ó(�Ǵ� ������) ����� �Ȱ� ������ ȥ��
    // === ���ο� Additive ��� ===
    // 1. �Ȱ� �⿩�� ��� (�Ȱ� ���� * �Ȱ� ����)
    float3 fogContribution = g_FogColor * fogFactor;
    // 2. ���� ���� ���� �Ȱ� �⿩���� ���� (saturate�� 0~1 ���� ����)
    Out.vColor.rgb = saturate(baseColor.rgb + fogContribution);
    // 3. ���� ���� ���� ���� �� ����
    Out.vColor.a = baseColor.a;
//    Out.vColor = lerp(baseColor, float4(g_FogColor, baseColor.a), fogFactor);
    // --- �Ȱ� ȿ�� ��� �� ---

    // 4. ���� �� ó�� (�ؽ�ó ���Ŀ� ���� ���� ���ϱ� ��)
    // Out.vColor.a = baseColor.a * g_Material.Diffuse.a; // ���� Diffuse�� ���ߴٸ�

    // 5. (���� ����) ���� ���� ���� discard ����
    if (Out.vColor.a < 0.8f)
         discard;

    return Out;
}

PS_OUT PS_TEST_LIGHTING(PS_IN In, float facing : VFACE)
{
    PS_OUT Out;

    // --- �⺻ ���� ��� ---
    float3 normal = normalize(In.vNormal * sign(facing));
    float3 viewDir = normalize(g_CameraPosition - In.vWorldPos.xyz);

    // --- �ؽ�ó ���ø� ---
    float4 baseColor = tex2D(DefaultSampler, In.vTexcoord); // Ÿ�ϸ� ������

    // --- ���� ���� ����� ���� ���� �ʱ�ȭ ---
    float3 accumulatedColor = (baseColor.rgb * g_Material.Diffuse.rgb * g_AmbientLightColor.rgb) + g_Material.Emissive.rgb;

    Light currentLight = g_Lights[0];
    
    /*float3 lightVec = float3(0.0f, 0.0f, 0.0f);
    float attenuation = 0.0f;
    float NdotL = 0.0f;*/
    
            // --- Ȯ���ϰ� ���� ���� ���� �Ʒ� �ڵ� �� �ϳ��� �ּ� ���� ---

            // [����� 1] ����Ʈ Ÿ��(Type) Ȯ���ϱ�
            // Type ���� ���� �ٸ� ���� ��� (0:����, 1:����, 2:�ʷ�, 3:�Ķ�)
            
            /*if (currentLight.Type == 1) { Out.vColor = float4(1.f, 0.f, 0.f, 1.f); return Out; } // Point = Red
            else if (currentLight.Type == 2) { Out.vColor = float4(0.f, 1.f, 0.f, 1.f); return Out; } // Spot = Green
            else if (currentLight.Type == 3) { Out.vColor = float4(0.f, 0.f, 1.f, 1.f); return Out; } // Dir = Blue
            else { Out.vColor = float4(0.f, 0.f, 0.f, 1.f); return Out; } // Unused(0) or Unknown = Black*/
            

            // [����� 2] ����Ʈ ����(Color/fDiffuse) Ȯ���ϱ�
            // ����Ʈ�� Color ���� �״�� ��� (Alpha�� 1�� ����)
            
            /*Out.vColor = float4(currentLight.Color.rgb, 1.0f);
            return Out; */

            // [����� 3] ����Ʈ ��ġ(Position) Ȯ���ϱ� (x,y,z -> R,G,B)
            // ��ġ ���� ������ ũ�Ƿ� ������ �����ϸ��Ͽ� [0,1] ������ ����� �ð�ȭ
            // ��: ���� ��ǥ ���� -50 ~ +50 ������ [0,1]�� ���� ����
            
            /*float3 posColor = saturate(abs(currentLight.Position) / 50.f); // ���밪�� 50���� ������ [0,1]�� ����
            Out.vColor = float4(posColor, 1.0f);
            return Out;*/
            

            // [����� 4] ����Ʈ ����(Direction) Ȯ���ϱ� (x,y,z -> R,G,B)
            // ���� ���� ��(-1 ~ +1)�� [0,1] ������ ��ȯ�Ͽ� �ð�ȭ
            
            /*float3 dirForColor = (normalize(currentLight.Direction) + 1.0f) * 0.5f;
            Out.vColor = float4(dirForColor, 1.0f);
            return Out;*/
            

            // [����� 5] ����Ʈ ����(Range) Ȯ���ϱ� (������ ä�� ���)
            // Range ���� Ư�� �ִ밪���� ������ [0,1] ������ ����� �ð�ȭ
            // ��: �ִ� Range 50���� ����
            
            
            /*float rangeColor = currentLight.Range / 50.f;  //saturate(currentLight.Range / 50.f);
            Out.vColor = float4(rangeColor, 0.f, 0.f, 1.f);
            return Out;*/
            

            // [����� 6] ����Ʈ ����(Attenuation1) Ȯ���ϱ� (��� ä�� ���)
            // Attenuation1 ���� Ư�� �ִ밪���� ������ [0,1] ������ ����� �ð�ȭ
            // ��: �ִ� Attenuation1 1.0���� ����
            
            /* float atten1Color = saturate(currentLight.Attenuation1 / 1.0f);
            Out.vColor = float4(0.f, atten1Color, 0.f, 1.f);
            return Out; */
    

            // [����� 7] ���� ���� ���谪(attenuation) Ȯ���ϱ� (ȸ����)
            // �� ����� �ڵ带 ����Ϸ��� �Ʒ��� ���� ��� ���� �Ϻΰ� �ʿ���
            
            /*float3 dirToLight = currentLight.Position.xyz - In.vWorldPos.xyz;
            float distSq = dot(dirToLight, dirToLight);
            float dist = sqrt(distSq);
            float attenuation = 0.0f;
            if(currentLight.Type == 1 || currentLight.Type == 2) { // ����Ʈ �Ǵ� ����
                 if (dist < currentLight.Range) {
                     attenuation = saturate(1.0f / (currentLight.Attenuation0 + currentLight.Attenuation1 * dist + currentLight.Attenuation2 * distSq));
                 }
            } else if (currentLight.Type == 3) { // ���⼺
                 attenuation = 1.0f;
            }
            Out.vColor = float4(attenuation, attenuation, attenuation, 1.0f);
            return Out;*/
            

            // [����� 8] ���� ���� NdotL �� Ȯ���ϱ� (ȸ����)
            // �� ����� �ڵ带 ����Ϸ��� �Ʒ��� ���� ��� ���� �Ϻΰ� �ʿ���
            
            float3 lightVec = float3(0,0,0);
            // ... (Ÿ�Կ� ���� lightVec ��� ���� �ʿ�) ...
            if (currentLight.Type == 3) lightVec = normalize(-currentLight.Direction).xyz;
            else if (currentLight.Type == 1 || currentLight.Type == 2) {
                float3 dirToLight = currentLight.Position.xyz - In.vWorldPos.xyz;
                float dist = length(dirToLight);
                if(dist > 0.001f) lightVec = dirToLight / dist; // Normalize
            }
            float NdotL = saturate(dot(normal, lightVec));
            Out.vColor = float4(NdotL, NdotL, NdotL, 1.0f);
            return Out;

            // --- ����� �ڵ� �� ---
        // ==============================================================
        //            �ڡڡ� ����� �ڵ� ���� �� �ڡڡ�
        // ==============================================================


        // --- ������ ����Ʈ ��� ���� ---


    // --- ���� ���� ���� �� ��ó�� ---
    float4 litColor;
    litColor.rgb = saturate(accumulatedColor);
    litColor.a = baseColor.a; // ���� ��

    // --- �Ȱ� ȿ�� ��� ---
    float distance = length(In.vViewPos);
    float fogFactor = saturate((distance - g_FogStart) / (g_FogEnd - g_FogStart));
    Out.vColor = lerp(litColor, float4(g_FogColor, litColor.a), fogFactor);

    return Out;
}

/*PS_OUT PS_LIT_MULTIPASS(PS_IN In, float facing : VFACE)
{
    PS_OUT Out;

    // --- �⺻ ���� ��� ---
    float3 normal = normalize(In.vNormal * sign(facing));
    float3 viewDir = normalize(g_CameraPosition - In.vWorldPos.xyz);

    // --- �ؽ�ó ���ø� ---
    float2 tiledUV = In.vTexcoord * In.vScale; // ������ ���
    float4 baseColor = tex2D(DefaultSampler, tiledUV);

    // --- ���� ���� ����� ���� ���� �ʱ�ȭ ---
    float3 accumulatedColor = float3(0.0f, 0.0f, 0.0f); // �߰� �н��� ���� 0���� �ʱ�ȭ

    // ù ��° �н�(Base Pass)�� ��쿡�� Ambient�� Emissive�� ���
    if (g_CurrentPassIndex == 0)
    {
        // ����: Ambient ��� ��� Ȯ�� �ʿ� (g_Material.Ambient�� g_Material.Diffuse �� �� ���ϴ� ���� �Ϲ������� ���� �� ����)
        // �Ϲ����� Ambient: baseColor.rgb * g_Material.Ambient.rgb * g_AmbientLightColor.rgb
        // �Ǵ�: baseColor.rgb * g_Material.Ambient.rgb (�ؽ�ó�� ���� �ֺ����� ���)
        // ���� �ڵ� ����:
        accumulatedColor = (baseColor.rgb * g_Material.Ambient.rgb * g_Material.Diffuse.rgb) + g_Material.Emissive.rgb;
    }

    // �̹� �н����� ó���� ����Ʈ�鿡 ���� ��� �� ����
    // for ������ ���۰� ���� C++���� ���޵� �ε����� ������ ����
    int loopEnd = g_LightStartIndex + g_NumLightsInPass;
    for (int i = g_LightStartIndex; i < loopEnd; ++i)
    {
        // g_Lights �迭 ���� Ȯ�� (������������ ������)
        if (i >= MAX_LIGHTS) // �Ǵ� ���� C++���� �����ϴ� ��ü ����Ʈ ��
            break;

        Light currentLight = g_Lights[i];

        if (currentLight.Type == 0)
            continue; // LT_UNUSED

        // --- ����Ʈ Ÿ�Ժ� ��� (���� �ڵ�� ���� ����) ---
        float3 lightVec = float3(0.0f, 0.0f, 0.0f);
        float attenuation = 0.0f;
        float NdotL = 0.0f;

        if (currentLight.Type == 3) // Directional
        {
            lightVec = normalize(-currentLight.Direction.xyz); // Direction�� xyz ���
            NdotL = saturate(dot(normal, lightVec));
            attenuation = 1.0f;
        }
        else if (currentLight.Type == 1) // Point
        {
            float3 dirToLight = currentLight.Position.xyz - In.vWorldPos.xyz; // Position�� xyz ���
            float distSq = dot(dirToLight, dirToLight);
            float dist = sqrt(distSq);

            if (dist < currentLight.Range)
            {
                lightVec = dirToLight / dist;
                NdotL = saturate(dot(normal, lightVec));
                attenuation = saturate(1.0f / (currentLight.Attenuation0 +
                                              currentLight.Attenuation1 * dist +
                                              currentLight.Attenuation2 * distSq));
            }
            // else: NdotL=0, attenuation=0 (�ʱⰪ ����)
        }
        else if (currentLight.Type == 2) // Spot
        {
            // ����Ʈ����Ʈ ��� (����� ����Ʈ ����Ʈ�� �����ϰ� ó����, �ʿ�� Ȯ��)
            float3 dirToLight = currentLight.Position.xyz - In.vWorldPos.xyz; // Position�� xyz ���
            float distSq = dot(dirToLight, dirToLight);
            float dist = sqrt(distSq);

            if (dist < currentLight.Range)
            {
                lightVec = dirToLight / dist;
                NdotL = saturate(dot(normal, lightVec));
                attenuation = saturate(1.0f / (currentLight.Attenuation0 +
                                              currentLight.Attenuation1 * dist +
                                              currentLight.Attenuation2 * distSq));

                // TODO: ���⿡ ����Ʈ����Ʈ ���԰�/���� ��� �߰�
                // float spotFactor = ... ��� ...
                // attenuation *= spotFactor;
            }
             // else: NdotL=0, attenuation=0 (�ʱⰪ ����)
        }

        // ���� ����Ʈ�� �⿩���� ��ȿ�ϴٸ� ����
        if (NdotL > 0.0f && attenuation > 0.0f)
        {
            // ��ǻ��(Diffuse) ���
            float3 diffuse = baseColor.rgb * g_Material.Diffuse.rgb * NdotL * currentLight.Color.rgb;

            // ����ŧ��(Specular) ���
            float3 halfwayDir = normalize(lightVec + viewDir);
            float NdotH = saturate(dot(normal, halfwayDir));
            float specPower = pow(NdotH, g_Material.Power);
            float3 specular = g_Material.Specular.rgb * specPower * currentLight.Color.rgb;

            // ���� ����Ʈ�� ���� �⿩�� = (��ǻ�� + ����ŧ��) * ����
            float3 currentLightContribution = (diffuse + specular) * attenuation;

            // ���� ���� ���� ����Ʈ �⿩�� ����
            accumulatedColor += currentLightContribution;
        }
    } // End of for loop

    // --- ���� ���� ���� �� ��ó�� ---

    // ù ��° �н������� �Ȱ��� �����ϰų�, ��� �н��� ���� �� ��ó���� �Ȱ��� �����ϴ� ���� ����.
    // ���⼭�� ù ��° �н������� �Ȱ��� �����ϴ� ����
    float4 finalColor;
    finalColor.rgb = saturate(accumulatedColor);
    finalColor.a = baseColor.a * g_Material.Diffuse.a; // ���� ��

    if (g_CurrentPassIndex == 0)
    {
        // --- �Ȱ� ȿ�� ��� (ù �н����� ����) ---
        float distance = length(In.vViewPos);
        float fogFactor = saturate((distance - g_FogStart) / (g_FogEnd - g_FogStart));
        Out.vColor = lerp(finalColor, float4(g_FogColor, finalColor.a), fogFactor);
    }
    else
    {
        // �߰� �н������� ���� ���� �⿩���� ��� (�Ȱ� ����)
        Out.vColor = finalColor;
    }

    // --- ���� �׽�Ʈ ---
    // if (Out.vColor.a < 0.1f) discard; // �ʿ��ϴٸ� ��� �н��� ����

    return Out;
}*/

PS_OUT PS_MAIN_BLACK(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = vector(0.f, 0.f, 0.f, 1.f);
        
    return Out;
}

technique DefaultTechnique
{
    
    pass DefaultPass
    {
        CullMode = None;
        aLPHAbLENDeNABLE = True;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        BlendOp = Add;

        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_UNLIT();
    }

    pass PlanePass
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
        PixelShader = compile ps_3_0 PS_LIT(); // �� ����� ���Ե� PS_MAIN ���
    }

    pass Weapon
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
        PixelShader = compile ps_3_0 PS_WEAPON(); // �� ����� ���Ե� PS_MAIN ���
    }

    pass BLACK
    {
        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_MAIN_BLACK();
    }

    pass TEST
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
        PixelShader = compile ps_3_0 PS_TEST_LIGHTING(); // �� ����� ���Ե� PS_MAIN ���
    }
}   
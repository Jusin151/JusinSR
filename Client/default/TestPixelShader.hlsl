// RedLightPixelShader.hlsl
sampler TextureSampler : register(s0);
float4 g_RedLightColor : register(c0); // ���� �� ����

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
};

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 texColor = tex2D(TextureSampler, input.TexCoord);
    float4 finalColor = texColor * g_RedLightColor; // �ؽ�ó ����� ���� �� ���� ���ϱ�
    return finalColor;
}
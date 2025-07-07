// PixelShader.hlsl

float3 g_ViewPos; // 월드 기준 카메라 위치
float4x4 g_World;

sampler2D g_DiffuseTex : register(s0);

struct VS_OUT
{
    float4 pos : POSITION;
    float2 tex : TEXCOORD0;
    float3 worldNormal : TEXCOORD1;
    float3 worldPos : TEXCOORD2;
};

float4 PS_Main(VS_OUT input) : COLOR
{
    float3 normal = normalize(input.worldNormal);

    // 월드 기준 고정 광원 방향
    float3 lightDir = normalize(float3(1, -1, -1)); // 모든 오브젝트에 대해 동일하게 적용됨
    float3 rotatedLightDir = normalize(mul(lightDir, (float3x3) g_World));
    // Half-Lambert shading
    float NdotL = dot(normal, rotatedLightDir);
    float halfLambert = saturate(NdotL * 0.5 + 0.5);

    // Rim Lighting
    float3 viewDir = normalize(g_ViewPos - input.worldPos);
    float rim = pow(1.0 - saturate(dot(normal, viewDir)), 2.0);
    float3 rimColor = float3(1.0, 0.85, 0.6);

    float3 texColor = tex2D(g_DiffuseTex, input.tex).rgb;
    float3 finalColor = texColor * halfLambert + rim * rimColor;

    return float4(finalColor, 1.0);
}

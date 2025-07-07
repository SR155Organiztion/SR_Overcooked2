// LambertDiffuse.hlsl

// ----------- 전역 상수 -----------
float4x4 g_WVP; // World * View * Projection
float4x4 g_World; // 노멀 변환용

sampler2D g_TexSampler; // 텍스처 샘플러

// ----------- 입력 구조체 -----------
struct VS_IN
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
};

struct VS_OUT
{
    float4 pos : POSITION;
    float2 tex : TEXCOORD0;
    float3 brightness : COLOR0; // 조명 색 * 밝기
};

// ----------- 버텍스 셰이더 -----------
VS_OUT VS_Main(VS_IN input)
{
    VS_OUT o;

    // 위치 변환
    o.pos = mul(input.pos, g_WVP);
    o.tex = input.tex;

    // 노멀 회전
    float3 worldNormal = normalize(mul(input.normal, (float3x3) g_World));

    // 광원 방향 & 색상 하드코딩
    float3 lightDir = normalize(float3(1, -1, -1)); // ← 방향 고정
    float3 lightColor = float3(1.0, 1.0, 1.0); // ← 흰색 빛

    float diffuse = max(dot(worldNormal, -lightDir), 0.0);
    o.brightness = lightColor * diffuse;

    return o;
}

// ----------- 픽셀 셰이더 -----------
float4 PS_Main(VS_OUT input) : COLOR
{
    float3 texColor = tex2D(g_TexSampler, input.tex).rgb;
    float3 finalColor = texColor * input.brightness;
    return float4(finalColor, 1.0);
}

// Shadow_VertexShader.hlsl

float4x4 g_ShadowWVP; // 월드 → 그림자 투영 → 클립 공간

struct VS_IN
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
};

struct VS_OUT
{
    float4 pos : POSITION;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT o = (VS_OUT) 0;

    // 월드 공간에서 그림자 행렬 → 투영 행렬 → 클립 공간
    o.pos = mul(input.pos, g_ShadowWVP);

    return o;
}

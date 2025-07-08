float4x4 g_ShadowWVP; // 월드 → 그림자 투영 → 클립 공간

struct VS_IN
{
    float4 pos : POSITION; // POSITION → float4 (w = 1)
    float3 normal : NORMAL; // 법선
    float3 tex : TEXCOORD0; // FVF와 일치시키기 위해 추가
};

struct VS_OUT
{
    float4 pos : POSITION;
    float3 tex : TEXCOORD0;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT o;
    float4 pos = input.pos;
    pos.w = 1.0f;

    o.pos = mul(pos, g_ShadowWVP);
    o.tex = input.tex;
    return o;
}

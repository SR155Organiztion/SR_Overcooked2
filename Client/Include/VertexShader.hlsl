float4x4 g_WVP;
float4x4 g_World;

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
    float3 worldNormal : TEXCOORD1;
    float3 worldPos : TEXCOORD2;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT o;

    float4 worldPos = mul(input.pos, g_World);
    float3 worldNormal = normalize(mul(input.normal, (float3x3) g_World));

    o.pos = mul(input.pos, g_WVP);
    o.tex = input.tex;
    o.worldNormal = worldNormal;
    o.worldPos = worldPos.xyz;

    return o;
}

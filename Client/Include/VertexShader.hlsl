float4x4 g_ShadowWVP; // ���� �� �׸��� ���� �� Ŭ�� ����

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

    // �׸��� ��ġ = ���� �� �׸��� ���� �� Ŭ�� ����
    o.pos = mul(input.pos, g_ShadowWVP);
    return o;
}

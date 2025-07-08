float4x4 g_ShadowWVP; // ���� �� �׸��� ���� �� Ŭ�� ����

struct VS_IN
{
    float4 pos : POSITION; // POSITION �� float4 (w = 1)
    float3 normal : NORMAL; // ����
    float3 tex : TEXCOORD0; // FVF�� ��ġ��Ű�� ���� �߰�
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

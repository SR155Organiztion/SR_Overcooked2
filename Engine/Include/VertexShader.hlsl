float4x4 g_ShadowWVP; // ���� �� �׸��� ���� �� Ŭ�� ����

struct VS_IN
{
    float4 pos : POSITION;
};

struct VS_OUT
{
    float4 pos : POSITION;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT o;

    // �׸��� ��ġ = ���� �� �׸��� ���� �� Ŭ�� ����
    o.pos = mul(input.pos, g_ShadowWVP);
    return o;
}

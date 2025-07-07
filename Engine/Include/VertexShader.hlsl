// LambertDiffuse.hlsl

// ----------- ���� ��� -----------
float4x4 g_WVP; // World * View * Projection
float4x4 g_World; // ��� ��ȯ��
float3 g_LightDir; // ���� ���� (���� ����)
float3 g_LightColor; // ���� ����

sampler2D g_TexSampler; // �ؽ�ó ���÷�

// ----------- �Է� ����ü -----------
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
    float3 brightness : COLOR0; // ���� �� * ���
};

// ----------- ���ؽ� ���̴� -----------
VS_OUT VS_Main(VS_IN input)
{
    VS_OUT o;

    // ��ġ ��ȯ
    o.pos = mul(input.pos, g_WVP);
    o.tex = input.tex;

    // ��� ȸ��
    float3 worldNormal = normalize(mul(input.normal, (float3x3) g_World));

    // ���� ���� ���� ���
    float diffuse = max(dot(worldNormal, normalize(-g_LightDir)), 0.0);

    // ��� = ����� * ����
    o.brightness = g_LightColor * diffuse;

    return o;
}

// ----------- �ȼ� ���̴� -----------
float4 PS_Main(VS_OUT input) : COLOR
{
    float3 texColor = tex2D(g_TexSampler, input.tex).rgb;
    float3 finalColor = texColor * input.brightness;
    return float4(finalColor, 1.0);
}

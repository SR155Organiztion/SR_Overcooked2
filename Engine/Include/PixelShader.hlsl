// Shadow_PixelShader.hlsl

float4 PS_Main() : COLOR
{
    // 그림자 색상 (반투명한 검은색 그림자)
    return float4(0.0f, 0.0f, 0.0f, 0.5f);
}

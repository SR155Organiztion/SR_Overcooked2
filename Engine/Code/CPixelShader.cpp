#include "CPixelShader.h"

CPixelShader::~CPixelShader()
{
}

HRESULT CPixelShader::Ready_Shader(LPDIRECT3DDEVICE9 pGraphicDev, LPD3DVERTEXELEMENT9 _pDecl)
{
    HRESULT hr;

    hr = D3DXCompileShaderFromFile(
        m_szShaderFileName
        , nullptr, nullptr
        , "PS_Main", "ps_2_0"
        , 0, &m_pShaderBuf
        , &m_pErrorMsg, &m_pConstTable
    );

    if (FAILED(hr)) {
        if (m_pErrorMsg)
            OutputDebugStringA((char*)m_pErrorMsg->GetBufferPointer());
        return E_FAIL;
    }

    hr = pGraphicDev->CreateVertexShader(
        (DWORD*)m_pShaderBuf->GetBufferPointer()
        , &m_pVertexShader
    );

    if (FAILED(hr)) {
        MSG_BOX("Create Pixel Shader Failed");
        return E_FAIL;
    }

    pGraphicDev->CreateVertexDeclaration(_pDecl, &m_pDecl);
    pGraphicDev->SetVertexDeclaration(m_pDecl);
    return S_OK;
}

void CPixelShader::Render_Shader(LPDIRECT3DDEVICE9 pGraphicDev)
{
    pGraphicDev->SetVertexShader(m_pVertexShader);
}

CComponent* CPixelShader::Clone()
{
    return new CPixelShader(*this);
}

CPixelShader* CPixelShader::Create(LPDIRECT3DDEVICE9 pGraphicDev
    , LPD3DVERTEXELEMENT9 _pDecl) {
    CPixelShader* pVertexShader = new CPixelShader(pGraphicDev);

    if (FAILED(pVertexShader->Ready_Shader(pGraphicDev, _pDecl))) {
        Safe_Release(pVertexShader);
        MSG_BOX("PixelShader Create Failed");
        return nullptr;
    }

    return pVertexShader;
}

#include "CPixelShader.h"

CPixelShader::CPixelShader()
{
}

CPixelShader::CPixelShader(LPDIRECT3DDEVICE9 pGraphicDev)
    : CShader(pGraphicDev)
{
}

CPixelShader::CPixelShader(const CPixelShader& rhs)
    : CShader(rhs), m_pVertexShader(rhs.m_pVertexShader)
    , m_pDecl(rhs.m_pDecl)
{
}

CPixelShader::~CPixelShader() {
    Free();
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

_int CPixelShader::Update_Component(const _float& fTimeDelta) {


    return 0;
}
void CPixelShader::LateUpdate_Component() {

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

void CPixelShader::Free() {
    Safe_Delete(m_pShaderBuf);
    Safe_Delete(m_pErrorMsg);
    Safe_Delete(m_pConstTable);
    Safe_Delete(m_szShaderFileName);
    Safe_Delete(m_pVertexShader);
    Safe_Delete(m_pDecl);
}

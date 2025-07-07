#include "CVertexShader.h"
#include "CManagement.h"

CVertexShader::~CVertexShader()
{
}

HRESULT CVertexShader::Ready_Shader(LPDIRECT3DDEVICE9 pGraphicDev, LPD3DVERTEXELEMENT9 _pDecl)
{
    HRESULT hr;

     hr = D3DXCompileShaderFromFile(
        m_szShaderFileName
        , nullptr, nullptr
        , "VS_Main", "vs_2_0"
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
         MSG_BOX("Create Vertex Shader Failed");
         return E_FAIL;
     }

     pGraphicDev->CreateVertexDeclaration(_pDecl, &m_pDecl);
     pGraphicDev->SetVertexDeclaration(m_pDecl);
     return S_OK;
}

_int CVertexShader::Update_Component(const _float& fTimeDelta) {


    return 0;
}
void CVertexShader::LateUpdate_Component() {

}

void CVertexShader::Render_Shader(LPDIRECT3DDEVICE9 pGraphicDev)
{
    pGraphicDev->SetVertexShader(m_pVertexShader);
}

void CVertexShader::Set_Values(_matrix* _pMatWorld, IDirect3DBaseTexture9* _pTex)
{
    m_matWorld = *_pMatWorld;
    m_pTex = _pTex;
    CManagement::GetInstance()->Get_Component()
}

CComponent* CVertexShader::Clone()
{
    return new CVertexShader(*this);
}

CVertexShader* CVertexShader::Create(LPDIRECT3DDEVICE9 pGraphicDev
    , LPD3DVERTEXELEMENT9 _pDecl) {
    CVertexShader* pVertexShader = new CVertexShader(pGraphicDev);

    if (FAILED(pVertexShader->Ready_Shader(pGraphicDev, _pDecl))) {
        Safe_Release(pVertexShader);
        MSG_BOX("VertexShader Create Failed");
        return nullptr;
    }

    return pVertexShader;
}

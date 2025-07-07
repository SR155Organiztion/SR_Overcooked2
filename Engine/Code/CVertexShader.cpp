#include "CVertexShader.h"
#include "CManagement.h"
#include "CCamera.h"

CVertexShader::CVertexShader()
{
}

CVertexShader::CVertexShader(LPDIRECT3DDEVICE9 pGraphicDev)
    : CShader(pGraphicDev)
{
}

CVertexShader::CVertexShader(const CVertexShader& rhs)
    : CShader(rhs), m_pVertexShader(rhs.m_pVertexShader)
    , m_pDecl(rhs.m_pDecl), m_matWorld(rhs.m_matWorld)
    , m_matView(rhs.m_matView) , m_matProj(rhs.m_matProj)
{
}

CVertexShader::~CVertexShader() {
    Free();
}

HRESULT CVertexShader::Ready_Shader(LPDIRECT3DDEVICE9 pGraphicDev, const D3DVERTEXELEMENT9 _pDecl[])
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

void CVertexShader::Set_Values(_matrix* _pMatWorld)
{
    m_matWorld = *_pMatWorld;

    CCamera* pCamera 
        = dynamic_cast<CCamera*>(
                CManagement::GetInstance()->Get_GameObject(
                    L"Environment_Layer"
                    , L"DynamicCamera"
                )
            );

    m_matView = *pCamera->Get_View();
    m_matProj = *pCamera->Get_Projection();

    D3DXHANDLE worldHandle = nullptr;
    m_pConstTable->GetConstantByName(worldHandle, "g_World");
    m_pConstTable->SetMatrix(m_pGraphicDev, worldHandle, &m_matWorld);

    _matrix WVP = m_matWorld * m_matView * m_matProj;
    D3DXHANDLE worldWVPHandle = nullptr;
    m_pConstTable->GetConstantByName(worldHandle, "g_WVP");
    m_pConstTable->SetMatrix(m_pGraphicDev, worldHandle, &WVP);
}

CComponent* CVertexShader::Clone()
{
    return new CVertexShader(*this);
}

CVertexShader* CVertexShader::Create(LPDIRECT3DDEVICE9 pGraphicDev
    , const D3DVERTEXELEMENT9 _pDecl[]) {
    CVertexShader* pVertexShader = new CVertexShader(pGraphicDev);

    if (FAILED(pVertexShader->Ready_Shader(pGraphicDev, _pDecl))) {
        Safe_Release(pVertexShader);
        MSG_BOX("VertexShader Create Failed");
        return nullptr;
    }

    return pVertexShader;
}

void CVertexShader::Free() {
    Safe_Delete(m_pShaderBuf);
    Safe_Delete(m_pErrorMsg);
    Safe_Delete(m_pConstTable);
    Safe_Delete(m_szShaderFileName);
    Safe_Delete(m_pVertexShader);
    Safe_Delete(m_pDecl);
}

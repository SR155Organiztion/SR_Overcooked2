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
    , m_pPixelShader(rhs.m_pPixelShader)
    , m_pDecl(rhs.m_pDecl), m_matWorld(rhs.m_matWorld)
    , m_matView(rhs.m_matView) , m_matProj(rhs.m_matProj)
{
    if (m_pVertexShader)   m_pVertexShader->AddRef();
    if (m_pPixelShader)    m_pPixelShader->AddRef();
    if (m_pDecl)           m_pDecl->AddRef();
}

CVertexShader::~CVertexShader() {
    Free();
}

HRESULT CVertexShader::Ready_Shader(LPDIRECT3DDEVICE9 pGraphicDev, const D3DVERTEXELEMENT9 _pDecl[])
{
    HRESULT hr;
    // Vertex Shader
     hr = D3DXCompileShaderFromFile(
         L"VertexShader.hlsl"
        , nullptr, nullptr
        , "VS_Main", "vs_2_0"
        , 0, &m_pVsShaderBuf
        , &m_pVsErrorMsg, &m_pVsConstTable
    );

     if (FAILED(hr)) {
         if (m_pVsErrorMsg)
             OutputDebugStringA((char*)m_pVsErrorMsg->GetBufferPointer());
         return E_FAIL;
     }

     hr = pGraphicDev->CreateVertexShader(
         (DWORD*)m_pVsShaderBuf->GetBufferPointer()
         , &m_pVertexShader
     );
     Safe_Release(m_pVsShaderBuf);

     if (FAILED(hr)) {
         MSG_BOX("Create Vertex Shader Failed");
         return E_FAIL;
     }
     //

     // Pixel Shader
     hr = D3DXCompileShaderFromFile(
         L"PixelShader.hlsl"
         , nullptr, nullptr
         , "PS_Main", "ps_2_0"
         , 0, &m_pPsShaderBuf
         , &m_pPsErrorMsg, &m_pPsConstTable
     );

     if (FAILED(hr)) {
         if (m_pPsErrorMsg) {
             OutputDebugStringA("[픽셀 셰이더 컴파일 에러]\n");
             OutputDebugStringA((char*)m_pPsErrorMsg->GetBufferPointer());
         }
         
         return E_FAIL;
     }

     hr = pGraphicDev->CreatePixelShader(
         (DWORD*)m_pPsShaderBuf->GetBufferPointer()
         , &m_pPixelShader
     );
     Safe_Release(m_pPsShaderBuf);

     if (FAILED(hr)) {
         MSG_BOX("Create Pixel Shader Failed");
         return E_FAIL;
     }
     //

     pGraphicDev->CreateVertexDeclaration(_pDecl, &m_pDecl);
     return S_OK;
}

_int CVertexShader::Update_Component(const _float& fTimeDelta) {


    return 0;
}
void CVertexShader::LateUpdate_Component() {

}

void CVertexShader::Render_Shader(LPDIRECT3DDEVICE9 pGraphicDev, const _matrix* _pMatWorld)
{
    pGraphicDev->SetVertexDeclaration(m_pDecl);
    pGraphicDev->SetVertexShader(m_pVertexShader);
    pGraphicDev->SetPixelShader(m_pPixelShader);

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
    m_matCameraWorld = *pCamera->Get_World();

    // 버텍스
    // 1. 방향광 정의 (하늘에서 대각선 아래로)
    D3DXVECTOR4 lightDir = D3DXVECTOR4(0.3f, 1.0f, 0.3f, 0.0f);

    // 2. 그림자 투영 행렬 생성 (y = 0 바닥 기준)
    D3DXPLANE shadowPlane = { 0, 1, 0, 0 }; // y=0 평면
    D3DXMATRIX matShadow;
    D3DXMatrixShadow(&matShadow, &lightDir, &shadowPlane);

    // 3. 광원 기준 투영 행렬 (직교 투영)
    D3DXMATRIX matLightProj;
    D3DXMatrixOrthoLH(&matLightProj, 50.f, 50.f, 0.1f, 100.f); // 조명용 투영

    // 4. 최종 그림자 WVP 계산: 월드 → 그림자 투영 → 광원투영 → 클립 공간
    D3DXMATRIX matShadowWVP = m_matWorld * matShadow * matLightProj;

    // 5. 셰이더에 넘기기
    D3DXHANDLE hShadowWVP = m_pVsConstTable->GetConstantByName(nullptr, "g_ShadowWVP");
    m_pVsConstTable->SetMatrix(m_pGraphicDev, hShadowWVP, &matShadowWVP);

    // 블렌딩 설정
    pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

void CVertexShader::End_RenderShader(LPDIRECT3DDEVICE9 pGraphicDev)
{
    pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    pGraphicDev->SetVertexShader(nullptr);
    pGraphicDev->SetPixelShader(nullptr);
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
    Safe_Release(m_pVsErrorMsg);
    Safe_Release(m_pVsConstTable);

    Safe_Release(m_pPsErrorMsg);
    Safe_Release(m_pPsConstTable);

    Safe_Release(m_pVertexShader);
    Safe_Release(m_pPixelShader);
    Safe_Release(m_pDecl);
}

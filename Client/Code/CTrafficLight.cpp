#include "pch.h"
#include "CTrafficLight.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CCamera.h"

CTrafficLight::CTrafficLight(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CGameObject(pGraphicDev), m_iFrame(0)
{
}

CTrafficLight::CTrafficLight(const CGameObject& rhs)
    : Engine::CGameObject(rhs), m_iFrame(0)
{
}

CTrafficLight::~CTrafficLight()
{
}

HRESULT CTrafficLight::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    return S_OK;
}

_int CTrafficLight::Update_GameObject(const _float& fTimeDelta)
{
    _uint iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    BillBoard();

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CTrafficLight::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

    _vec3 vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);
    Engine::CGameObject::Compute_ViewZ(&vPos);
}

void CTrafficLight::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

    m_pTextureCom->Set_Texture(m_iFrame);

    if (FAILED(Engine::CGameObject::Set_Material()))
        return;

    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void CTrafficLight::Set_Scale(const _float& fX, const _float& fY, const _float& fZ)
{
    if (m_pTransformCom)
    {
        MSG_BOX("TrafficLight Scale Set Failed");
        return;
    }

    _vec3 vScale = { fX, fY, fZ };
    m_pTransformCom->Set_Scale(vScale);
}

void CTrafficLight::Set_Texture(TLPOSDIR eDir)
{
    switch (eDir)
    {
    case L_FWD: 
        m_iFrame = 0;
        break;
    case R_FWD:
        m_iFrame = 1;
        break;
    case R_LEFT:
        m_iFrame = 2;
        break;
    }
}

HRESULT CTrafficLight::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTex"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Deco_TrafficLight"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

    return S_OK;
}

void CTrafficLight::BillBoard()
{
    _matrix		matWorld, matView, matBill;

    m_pTransformCom->Get_World(&matWorld);
    CGameObject* pCamera = CManagement::GetInstance()->Get_GameObject(L"Environment_Layer", L"DynamicCamera");
    matView = *dynamic_cast<CCamera*>(pCamera)->Get_View();
    D3DXMatrixIdentity(&matBill);

    matBill._11 = matView._11;
    matBill._12 = matView._12;
    matBill._13 = matView._13;
    matBill._21 = matView._21;
    matBill._22 = matView._22;
    matBill._23 = matView._23;
    matBill._31 = matView._31;
    matBill._32 = matView._32;
    matBill._33 = matView._33;

    D3DXMatrixInverse(&matBill, 0, &matBill);

    matWorld = matBill * matWorld;

    m_pTransformCom->Set_World(&matWorld);
}

CTrafficLight* CTrafficLight::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CTrafficLight* pTrafficLight = new CTrafficLight(pGraphicDev);

    if (FAILED(pTrafficLight->Ready_GameObject()))
    {
        Safe_Release(pTrafficLight);
        MSG_BOX("TrafficLight Create Failed");
        return nullptr;
    }

    return pTrafficLight;
}

void CTrafficLight::Free()
{
    Engine::CGameObject::Free();
}
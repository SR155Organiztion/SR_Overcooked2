#include "pch.h"
#include "CBasket.h"
#include "CProtoMgr.h"
#include "CRenderer.h"

CBasket::CBasket(LPDIRECT3DDEVICE9 pGraphicDev)
    : CGameObject(pGraphicDev)
{
}

CBasket::CBasket(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

CBasket::~CBasket()
{
}

HRESULT CBasket::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    m_pTransformCom->Set_Scale({ 3.f, 1.f, 0.5f });
    m_pTransformCom->Set_Pos(1.5f, 0.5f, 0.f);

    return S_OK;
}

_int CBasket::Update_GameObject(const _float& fTimeDelta)
{
    _uint iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

    return iExit;
}

void CBasket::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

    return;
}

void CBasket::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (FAILED(Set_Material()))
        return;

    m_pTextureCom->Set_Texture(0);

    m_pBufferCom->Render_Buffer();
}

void CBasket::Set_Scale(const _float& fX, const _float& fY, const _float& fZ)
{
    if (!m_pTransformCom)
    {
        MSG_BOX("Basket Scale Set Failed");
        return;
    }

    _vec3 vScale = { fX, fY, fZ };
    m_pTransformCom->Set_Scale(vScale);
}

HRESULT CBasket::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<Engine::CCubeTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_CubeTex"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_EnvironmentTexture_Wall_Basket"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

    return S_OK;
}

CBasket* CBasket::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CBasket* pBasket = new CBasket(pGraphicDev);

    if (FAILED(pBasket->Ready_GameObject()))
    {
        Safe_Release(pBasket);
        MSG_BOX("Wall_basket Create Failed");
        return nullptr;
    }

    return pBasket;
}

void CBasket::Free()
{
    Engine::CGameObject::Free();
}

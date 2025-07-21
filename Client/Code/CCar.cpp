#include "pch.h"
#include "CCar.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CCamera.h"

CCar::CCar(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CGameObject(pGraphicDev), m_iFrame(0)
{
}

CCar::CCar(const CGameObject& rhs)
    : Engine::CGameObject(rhs), m_iFrame(0)
{
}

CCar::~CCar()
{
}

HRESULT CCar::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    m_iFrame = rand() % 4;

    return S_OK;
}

_int CCar::Update_GameObject(const _float& fTimeDelta)
{
    _uint iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CCar::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

    _vec3 vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);
    Engine::CGameObject::Compute_ViewZ(&vPos);
}

void CCar::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

    m_pTextureCom->Set_Texture(m_iFrame);

    if (FAILED(Engine::CGameObject::Set_Material()))
        return;

    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void CCar::Set_Scale(const _float& fX, const _float& fY, const _float& fZ)
{
    if (!m_pTransformCom)
    {
        MSG_BOX("Car Scale Set Failed");
        return;
    }

    _vec3 vScale = { fX, fY, fZ };
    m_pTransformCom->Set_Scale(vScale);
}

HRESULT CCar::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTileTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTileTex"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_DecoTexture_Car"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

    return S_OK;
}

CCar* CCar::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CCar* pCar = new CCar(pGraphicDev);

    if (FAILED(pCar->Ready_GameObject()))
    {
        Safe_Release(pCar);
        MSG_BOX("Car Create Failed");
        return nullptr;
    }

    return pCar;
}

void CCar::Free()
{
    Engine::CGameObject::Free();
}

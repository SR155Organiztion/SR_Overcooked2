#include "pch.h"
#include "CBrickWall.h"
#include "CProtoMgr.h"
#include "CRenderer.h"

CBrickWall::CBrickWall(LPDIRECT3DDEVICE9 pGraphicDev)
    : CGameObject(pGraphicDev), m_iFrame(0)
{
}

CBrickWall::CBrickWall(const CGameObject& rhs)
    : CGameObject(rhs), m_iFrame(0)
{
}

CBrickWall::~CBrickWall()
{
}

HRESULT CBrickWall::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    m_pTransformCom->Set_Scale({3.f, 2.f, 0.5f});
    m_pTransformCom->Set_Pos(1.5f, 1.f, 0.f);

    return S_OK;
}

_int CBrickWall::Update_GameObject(const _float& fTimeDelta)
{
    _uint iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

    return iExit;
}

void CBrickWall::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

    return;
}

void CBrickWall::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (FAILED(Set_Material()))
        return;

    m_pTextureCom->Set_Texture(m_iFrame);

    m_pBufferCom->Render_Buffer();
}

void CBrickWall::Set_Scale(const _float& fX, const _float& fY, const _float& fZ)
{
    if (m_pTransformCom)
    {
        MSG_BOX("BrickWall Scale Set Failed");
        return;
    }

    _vec3 vScale = { fX, fY, fZ };
    m_pTransformCom->Set_Scale(vScale);
}

void CBrickWall::Set_Texture(BRICKTYPE eType)
{
    switch (eType)
    {
    case REDBROWN:
        m_iFrame = 0;
        break;
    case PINKBROWN:
        m_iFrame = 1;
        break;
    }
}

HRESULT CBrickWall::Add_Component()
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

    pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_EnvironmentTexture_Wall_Brick"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

    return S_OK;
}

CBrickWall* CBrickWall::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CBrickWall* pBrickWall = new CBrickWall(pGraphicDev);

    if (FAILED(pBrickWall->Ready_GameObject()))
    {
        Safe_Release(pBrickWall);
        MSG_BOX("Wall_brick Create Failed");
        return nullptr;
    }

    return pBrickWall;
}

void CBrickWall::Free()
{
    Engine::CGameObject::Free();
}
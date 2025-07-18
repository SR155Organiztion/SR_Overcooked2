#include "pch.h"
#include "CWoodWall.h"
#include "CProtoMgr.h"
#include "CRenderer.h"

CWoodWall::CWoodWall(LPDIRECT3DDEVICE9 pGraphicDev)
    : CGameObject(pGraphicDev)
{
}

CWoodWall::CWoodWall(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

CWoodWall::~CWoodWall()
{
}

HRESULT CWoodWall::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    return S_OK;
}

_int CWoodWall::Update_GameObject(const _float& fTimeDelta)
{
    _uint iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

    return iExit;
}

void CWoodWall::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

    return;
}

void CWoodWall::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (FAILED(Set_Material()))
        return;

    m_pTextureCom->Set_Texture(0);

    m_pBufferCom->Render_Buffer();
}

HRESULT CWoodWall::Add_Component()
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

    pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_EnvironmentTexture_Wall_Wood"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

    return S_OK;
}

CWoodWall* CWoodWall::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CWoodWall* pWoodWall = new CWoodWall(pGraphicDev);

    if (FAILED(pWoodWall->Ready_GameObject()))
    {
        Safe_Release(pWoodWall);
        MSG_BOX("Wall_wood Create Failed");
        return nullptr;
    }

    return pWoodWall;
}

void CWoodWall::Free()
{
    Engine::CGameObject::Free();
}
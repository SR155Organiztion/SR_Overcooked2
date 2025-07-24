#include "pch.h"
#include "CRoadTile.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"

CRoadTile::CRoadTile(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CGameObject(pGraphicDev), m_iTextureNum(0)
{
}

CRoadTile::CRoadTile(const CGameObject& rhs)
    : Engine::CGameObject(rhs), m_iTextureNum(0)
{
}

CRoadTile::~CRoadTile()
{
}

HRESULT CRoadTile::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    m_pTransformCom->Set_Scale({ 34.f, 34.f, 34.f });
    m_pTransformCom->Set_Pos(7.f, 0.f, 12.f);

    return S_OK;
}

_int CRoadTile::Update_GameObject(const _float& fTimeDelta)
{
    _uint iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

    return iExit;
}

void CRoadTile::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

    return;
}

void CRoadTile::Render_GameObject()
{
    D3DXMATRIX matWorld;
    m_pTransformCom->Get_World(&matWorld);
    m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

    m_pTextureCom->Set_Texture(m_iTextureNum);

    if (FAILED(Set_Metarial()))
        return;

    m_pBufferCom->Render_Buffer();

    // m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CRoadTile::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTileTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTileTex"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_EnvironmentTexture_Tile_Road"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });


    return S_OK;
}

HRESULT CRoadTile::Set_Metarial()
{
    D3DMATERIAL9 tMetarial;
    ZeroMemory(&tMetarial, sizeof(D3DMATERIAL9));

    tMetarial.Diffuse = D3DXCOLOR(1.f, 0.f, 1.f, 1.f);
    tMetarial.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    tMetarial.Ambient = D3DXCOLOR(0.6f, 0.6f, 0.6f, 0.6f);

    tMetarial.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
    tMetarial.Power = 0.f;

    m_pGraphicDev->SetMaterial(&tMetarial);

    return S_OK;
}

CRoadTile* CRoadTile::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CRoadTile* pRoadTile = new CRoadTile(pGraphicDev);

    if (FAILED(pRoadTile->Ready_GameObject()))
    {
        Safe_Release(pRoadTile);
        MSG_BOX("RoadTile Create Failed");
        return nullptr;
    }

    return pRoadTile;
}

void CRoadTile::Free()
{
    Engine::CGameObject::Free();
}

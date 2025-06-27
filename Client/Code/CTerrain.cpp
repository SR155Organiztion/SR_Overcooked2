#include "pch.h"
#include "CTerrain.h"
#include "CProtoMgr.h"
#include "CRenderer.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CGameObject(pGraphicDev)
{
}

CTerrain::CTerrain(const CGameObject& rhs)
    : Engine::CGameObject(rhs)
{
}

CTerrain::~CTerrain()
{
}

HRESULT CTerrain::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    return S_OK;
}

_int CTerrain::Update_GameObject(const _float& fTimeDelta)
{
    _uint iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

    return iExit;
}

void CTerrain::LateUpdate_GameObject(const _float& fTimeDelta)
{

    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CTerrain::Render_GameObject()
{
    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    //m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
   //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

    m_pTextureCom->Set_Texture(0);

    // 재질 정보 셋팅
    if (FAILED(Set_Material()))
        return;

    m_pBufferCom->Render_Buffer();

    // m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    //m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

}

HRESULT CTerrain::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<Engine::CTerrainTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_TerrainTex"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_TerrainTexture"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });


    return S_OK;
}

HRESULT CTerrain::Set_Material()
{
    D3DMATERIAL9        tMtrl;
    ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

    tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    tMtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    tMtrl.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);

    tMtrl.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
    tMtrl.Power = 0.f;      // 정반사의 세기

    m_pGraphicDev->SetMaterial(&tMtrl);

    return S_OK;
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CTerrain* pPlayer = new CTerrain(pGraphicDev);

    if (FAILED(pPlayer->Ready_GameObject()))
    {
        Safe_Release(pPlayer);
        MSG_BOX("pPlayer Create Failed");
        return nullptr;
    }

    return pPlayer;
}

void CTerrain::Free()
{
    Engine::CGameObject::Free();
}

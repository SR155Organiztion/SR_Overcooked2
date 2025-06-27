#include "pch.h"
#include "CPlayer.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CDInputMgr.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CGameObject(pGraphicDev)
{
}

CPlayer::CPlayer(const CGameObject& rhs)
    : Engine::CGameObject(rhs)
{
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
     _uint iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

     Set_OnTerrain();

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

    return iExit;
}

void CPlayer::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Key_Input(fTimeDelta);

    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CPlayer::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

    m_pTextureCom->Set_Texture(0);

    m_pBufferCom->Render_Buffer();

   // m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CPlayer::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTex"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_PlayerTexture"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    pComponent = m_pCalculatorCom = dynamic_cast<Engine::CCalculator*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Calculator"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Calculator", pComponent });


    return S_OK;
}

void CPlayer::Key_Input(const _float& fTimeDelta)
{
    _vec3       vUp;
    m_pTransformCom->Get_Info(INFO_LOOK, &vUp);

    if (GetAsyncKeyState(VK_UP))
    {
        m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vUp, &vUp), 10.f, fTimeDelta);
    }
    if (GetAsyncKeyState(VK_DOWN))
    {
        m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vUp, &vUp), -10.f, fTimeDelta);
    }

      if (GetAsyncKeyState(VK_LEFT))
    {
        m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180.f * fTimeDelta));
    }

    if (GetAsyncKeyState(VK_RIGHT))
    {
        m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180.f * -fTimeDelta));
    }

    if (CDInputMgr::GetInstance()->Get_DIMouseState(DIM_LB) & 0x80)
    {
        _vec3 vPickPos = Pickion_OnTerrain();

        _vec3   vPos;
        m_pTransformCom->Get_Info(INFO_POS, &vPos);

        _vec3   vDir = vPickPos - vPos;
        m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vDir, &vDir), 10.f, fTimeDelta);                
    }


}

void CPlayer::Set_OnTerrain()
{
    _vec3   vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);

    Engine::CTerrainTex* pTerrainBufferCom = dynamic_cast<Engine::CTerrainTex*>
        (CManagement::GetInstance()->Get_Component(ID_STATIC, L"Environment_Layer", L"Terrain", L"Com_Buffer"));


    float fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos());

    m_pTransformCom->Set_Pos(vPos.x, fHeight + 1.f, vPos.z);

}

_vec3 CPlayer::Pickion_OnTerrain()
{
    Engine::CTerrainTex* pTerrainBufferCom = dynamic_cast<Engine::CTerrainTex*>
        (CManagement::GetInstance()->Get_Component(ID_STATIC, L"Environment_Layer", L"Terrain", L"Com_Buffer"));

    Engine::CTransform* pTerrainTransformCom = dynamic_cast<Engine::CTransform*>
        (CManagement::GetInstance()->Get_Component(ID_DYNAMIC, L"Environment_Layer", L"Terrain", L"Com_Transform"));

    return m_pCalculatorCom->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CPlayer* pPlayer = new CPlayer(pGraphicDev);

    if (FAILED(pPlayer->Ready_GameObject()))
    {
        Safe_Release(pPlayer);
        MSG_BOX("pPlayer Create Failed");
        return nullptr;
    }

    return pPlayer;
}

void CPlayer::Free()
{
    Engine::CGameObject::Free();
}

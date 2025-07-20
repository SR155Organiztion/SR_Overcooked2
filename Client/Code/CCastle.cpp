#include "pch.h"
#include "CCastle.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"

CCastle::CCastle(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CGameObject(pGraphicDev)
{
}

CCastle::CCastle(const CGameObject& rhs)
    : Engine::CGameObject(rhs)
{
}

CCastle::~CCastle()
{
}

HRESULT CCastle::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    return S_OK;
}

_int CCastle::Update_GameObject(const _float& fTimeDelta)
{

    _uint iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    _matrix matWorld, matView, matBill;

    m_pTransformCom->Get_World(&matWorld);
    m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
    D3DXMatrixIdentity(&matBill);
    D3DXMatrixInverse(&matView, 0, &matView);

    _vec3	vViewScale, vViewTrans;
    D3DXQUATERNION qViewRot;
    D3DXMatrixDecompose(&vViewScale, &qViewRot, &vViewTrans, &matView);
    _matrix matViewRot;  D3DXMatrixRotationQuaternion(&matViewRot, &qViewRot);

    matWorld = matViewRot * matWorld;

    m_pTransformCom->m_matWorld = matWorld;

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CCastle::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
    
    _vec3 vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);
    Engine::CGameObject::Compute_ViewZ(&vPos);

    return;
}

void CCastle::Render_GameObject()
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


void CCastle::Set_Angle(_float _fAngle)
{
    m_pTransformCom->m_vAngle.y = _fAngle;
}


HRESULT CCastle::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTex"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_EnvironmentObject_Map_Castle"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });


    return S_OK;
}

HRESULT CCastle::Set_Metarial()
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

CCastle* CCastle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CCastle* pCastle = new CCastle(pGraphicDev);

    if (FAILED(pCastle->Ready_GameObject()))
    {
        Safe_Release(pCastle);
        MSG_BOX("pCastle Create Failed");
        return nullptr;
    }

    return pCastle;
}

void CCastle::Free()
{
    Engine::CGameObject::Free();
}

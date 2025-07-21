#include "pch.h"
#include "CPlant.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"

CPlant::CPlant(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CGameObject(pGraphicDev)
{
}

CPlant::CPlant(const CGameObject& rhs)
    : Engine::CGameObject(rhs)
{
}

CPlant::~CPlant()
{
}

HRESULT CPlant::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    m_bEnable = true;

    return S_OK;
}

_int CPlant::Update_GameObject(const _float& fTimeDelta)
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

void CPlant::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

    _vec3 vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);
    Engine::CGameObject::Compute_ViewZ(&vPos);

    return;
}

void CPlant::Render_GameObject()
{
    if (!m_bEnable)
        return;

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

void CPlant::Set_Texture(const _uint _iID)
{
    m_iTextureNum = (_iID);
}

void CPlant::Set_Angle(const _float _fAngle)
{
    m_pTransformCom->m_vAngle.y = _fAngle;
}

void CPlant::Set_Scale(const _vec3 _vScale)
{
    m_pTransformCom->Set_Scale(_vScale);
}


HRESULT CPlant::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTex"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_EnvironmentObject_Map_Plant"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });


    return S_OK;
}

HRESULT CPlant::Set_Metarial()
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

CPlant* CPlant::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CPlant* pPlant = new CPlant(pGraphicDev);

    if (FAILED(pPlant->Ready_GameObject()))
    {
        Safe_Release(pPlant);
        MSG_BOX("pPlant Create Failed");
        return nullptr;
    }

    return pPlant;
}

void CPlant::Free()
{
    Engine::CGameObject::Free();
}

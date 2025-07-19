#include "pch.h"
#include "CSandbag.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CCamera.h"

CSandbag::CSandbag(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CGameObject(pGraphicDev)
{
}

CSandbag::CSandbag(const CGameObject& rhs)
    : Engine::CGameObject(rhs)
{
}

CSandbag::~CSandbag()
{
}

HRESULT CSandbag::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    m_iFrame = rand() % 4;

    return S_OK;
}

_int CSandbag::Update_GameObject(const _float& fTimeDelta)
{
    _uint iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    BillBoard();

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CSandbag::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

    _vec3 vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);
    Engine::CGameObject::Compute_ViewZ(&vPos);
}

void CSandbag::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

    m_pTextureCom->Set_Texture(m_iFrame);

    if (FAILED(Engine::CGameObject::Set_Material()))
        return;

    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void CSandbag::Set_Scale(const _float& fX, const _float& fY, const _float& fZ)
{
    if (m_pTransformCom)
    {
        MSG_BOX("Sandbag Scale Set Failed");
        return;
    }

    _vec3 vScale = { fX, fY, fZ };
    m_pTransformCom->Set_Scale(vScale);
}

HRESULT CSandbag::Add_Component()
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

    pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Deco_Sandbag"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

    return S_OK;
}

void CSandbag::BillBoard()
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

CSandbag* CSandbag::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CSandbag* pSandbag = new CSandbag(pGraphicDev);

    if (FAILED(pSandbag->Ready_GameObject()))
    {
        Safe_Release(pSandbag);
        MSG_BOX("Sandbag Create Failed");
        return nullptr;
    }

    return pSandbag;
}

void CSandbag::Free()
{
    Engine::CGameObject::Free();
}
#include "pch.h"
#include "CBamboo.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CCamera.h"

CBamboo::CBamboo(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CGameObject(pGraphicDev), m_iTextureNum(0)
{
}

CBamboo::CBamboo(const CGameObject& rhs)
    : Engine::CGameObject(rhs), m_iTextureNum(0)
{
}

CBamboo::~CBamboo()
{
}

HRESULT CBamboo::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    return S_OK;
}

_int CBamboo::Update_GameObject(const _float& fTimeDelta)
{
    _uint iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    BillBoard();

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CBamboo::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

    _vec3 vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);
    Engine::CGameObject::Compute_ViewZ(&vPos);
}

void CBamboo::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

    m_pTextureCom->Set_Texture(m_iTextureNum);

    if (FAILED(Engine::CGameObject::Set_Material()))
        return;

    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void CBamboo::Set_Scale(const _float& fX, const _float& fY, const _float& fZ)
{
    if (!m_pTransformCom)
    {
        MSG_BOX("Bamboo Scale Set Failed");
        return;
    }

    _vec3 vScale = { fX, fY, fZ };
    vScale *= 2.f;
    m_pTransformCom->Set_Scale(vScale);
}

void CBamboo::Set_Texture(const _uint _iID)
{
    m_iTextureNum = (_iID);
}

HRESULT CBamboo::Add_Component()
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

    pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_DecoTexture_Bamboo"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

    return S_OK;
}

void CBamboo::BillBoard()
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

CBamboo* CBamboo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CBamboo* pBamboo = new CBamboo(pGraphicDev);

    if (FAILED(pBamboo->Ready_GameObject()))
    {
        Safe_Release(pBamboo);
        MSG_BOX("Bamboo Create Failed");
        return nullptr;
    }

    return pBamboo;
}

void CBamboo::Free()
{
    Engine::CGameObject::Free();
}

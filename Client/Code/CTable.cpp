#include "pch.h"
#include "CTable.h"
#include "CProtoMgr.h"
#include "CRenderer.h"

CTable::CTable(LPDIRECT3DDEVICE9 pGraphicDev)
    : CGameObject(pGraphicDev), m_iFrame(0)
{
}

CTable::CTable(const CGameObject& rhs)
    : CGameObject(rhs), m_iFrame(0)
{
}

CTable::~CTable()
{
}

HRESULT CTable::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    return S_OK;
}

_int CTable::Update_GameObject(const _float& fTimeDelta)
{
    _uint iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CTable::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

    return;
}

void CTable::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    if (FAILED(Set_Material()))
        return;

    m_pTextureCom->Set_Texture(m_iFrame);

    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CTable::Set_Scale(const _float& fX, const _float& fY, const _float& fZ)
{
    if (!m_pTransformCom)
    {
        MSG_BOX("Table Scale Set Failed");
        return;
    }

    _vec3 vScale = { fX, fY, fZ };
    m_pTransformCom->Set_Scale(vScale);
}

void CTable::Set_Texture(TABLETYPE eDir)
{
    switch (eDir)
    {
    case BLACK:
        m_iFrame = 0;
        break;
    case WHITE:
        m_iFrame = 1;
        break;
    case GREEN:
        m_iFrame = 2;
        break;
    case CHECK:
        m_iFrame = 3;
        break;
    }
}

HRESULT CTable::Add_Component()
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

    pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_DecoTexture_Table"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

    return S_OK;
}

CTable* CTable::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CTable* pTable = new CTable(pGraphicDev);

    if (FAILED(pTable->Ready_GameObject()))
    {
        Safe_Release(pTable);
        MSG_BOX("Table Create Failed");
        return nullptr;
    }

    return pTable;
}

void CTable::Free()
{
    Engine::CGameObject::Free();
}

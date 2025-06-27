#include "pch.h"
#include "CBackGround.h"
#include "CProtoMgr.h"
#include "CRenderer.h"

CBackGround::CBackGround(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CGameObject(pGraphicDev)
{
}

CBackGround::CBackGround(const CGameObject& rhs)
    : Engine::CGameObject(rhs)
{
}

CBackGround::~CBackGround()
{
}

HRESULT CBackGround::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    return S_OK;
}

_int CBackGround::Update_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_PRIORITY, this);

    return 0;
}

void CBackGround::LateUpdate_GameObject(const _float& fTimeDelta)
{

    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CBackGround::Render_GameObject()
{ 
    m_pTextureCom->Set_Texture(0);

    m_pBufferCom->Render_Buffer();    
}

HRESULT CBackGround::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTex"));
    if (nullptr == pComponent)
        return E_FAIL;  
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });  

    pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_LogoTexture"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

    return S_OK;
}


CBackGround* CBackGround::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CBackGround* pBackGround = new CBackGround(pGraphicDev);

    if (FAILED(pBackGround->Ready_GameObject()))
    {
        Safe_Release(pBackGround);
        MSG_BOX("pBackGround Create Failed");
        return nullptr;

    }

    return pBackGround;
}

void CBackGround::Free()
{
    Engine::CGameObject::Free();
}

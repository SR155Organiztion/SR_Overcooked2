#include "pch.h"
#include "CUi.h"
#include "CProtoMgr.h" 

CUi::CUi(LPDIRECT3DDEVICE9 pGraphicDev): Engine::CGameObject(pGraphicDev)
{
}

CUi::CUi(const CGameObject& rhs):Engine::CGameObject(rhs)
{
}

CUi::~CUi()
{
}

_int		CUi::Update_GameObject(const _float& fTimeDelta) 
{
    return 0;
}
void		CUi::LateUpdate_GameObject(const _float& fTimeDelta) 
{

}

void CUi::Render_GameObject()
{
 
}

HRESULT CUi::Add_Component()
{
        Engine::CComponent* pComponent = nullptr;

        // Buffer
        pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTex"));
        if (nullptr == pComponent)
            return E_FAIL;
        m_mapComponent[ID_DYNAMIC].insert({ L"Com_Buffer", pComponent });

        // SPrite
        pComponent = m_pSpriteCom = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Button"));
        if (nullptr == pComponent)
            return E_FAIL;
        m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite", pComponent });
  
   
    return S_OK;
}


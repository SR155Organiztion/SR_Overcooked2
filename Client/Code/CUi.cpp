#include "pch.h"
#include "CUi.h"
#include "CProtoMgr.h" 

CUi::CUi(): CGameObject(nullptr), m_iNonAlpha(255), m_pCenter(nullptr)
{
   
    memset(&m_iAlpha, 255, sizeof(int[5]));
}

CUi::CUi(LPDIRECT3DDEVICE9 pGraphicDev): Engine::CGameObject(pGraphicDev), m_iNonAlpha(255), m_pCenter(nullptr)
{
   
    memset(&m_iAlpha, 255, sizeof(int[5]));
}

CUi::CUi(const CGameObject& rhs):Engine::CGameObject(rhs), m_iNonAlpha(255), m_pCenter(nullptr)
{
    
    memset(&m_iAlpha, 255, sizeof(int[5]));
  
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

     
        pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTex"));
        if (nullptr == pComponent)
            return E_FAIL;
        m_mapComponent[ID_DYNAMIC].insert({ L"Com_Buffer", pComponent });
       
        pComponent = m_pSpriteCom = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Button"));
        if (nullptr == pComponent)
            return E_FAIL;
        m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite", pComponent });

        pComponent = m_pSpriteCom2 = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Object"));
        if (nullptr == pComponent)
            return E_FAIL;
        m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite2", pComponent });
   
    return S_OK;
}


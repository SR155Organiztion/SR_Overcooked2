#include "pch.h"
#include "CUi.h"

CUi::CUi(): CGameObject(nullptr), m_iNonAlpha(255), m_pCenter(nullptr), m_eGaugeType(END_GAUGE), m_eButtonType(END_BUTTON)
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

_int CUi::Update_GameObject(const _float& fTimeDelta) 
{
    return 0;
}
void CUi::LateUpdate_GameObject(const _float& fTimeDelta) 
{

}

void CUi::Render_GameObject()
{
 
}

HRESULT CUi::Add_Component()
{
      
        
    return S_OK;
}


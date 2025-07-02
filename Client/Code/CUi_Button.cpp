#include "pch.h"
#include "CUi_Button.h"
#include "CUi_Icon.h"
#include "CUi_MainMenu.h"

//Engine
#include "Engine_Define.h"
#include "CRenderer.h"
#include "CSprite.h"



CUi_Button::CUi_Button(LPDIRECT3DDEVICE9 pGraphicDev):CUi(pGraphicDev),
m_pTexBtn(nullptr), m_pTexLock(nullptr), m_pTexLockImage(nullptr), m_pTexMenuHover(nullptr),
m_pTexScroll(nullptr), m_pTexScrollStart(nullptr), m_pTexOkBox(nullptr), m_pTexNoBox(nullptr),
 m_pTexName(nullptr)

{
}

CUi_Button::CUi_Button(const CGameObject& rhs):CUi(rhs)
{
}

CUi_Button::~CUi_Button()
{
}

HRESULT CUi_Button::Ready_GameObject(LPDIRECT3DDEVICE9 pGraphicDev, BUTTON_TYPE _cValue) 
{
	if (FAILED(Add_Component()))
		return E_FAIL;


	switch (_cValue)
	{
	case START_BUTTON:
	{	
		///위치 조정
		m_vPos = D3DXVECTOR3(100, 100, 0);

		m_eType = _cValue;
	}
	break;
	}
	return S_OK;
}



_int CUi_Button::Update_GameObject(const _float& fTimeDelta) 
{
	_uint iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

	return iExit;
}


void CUi_Button::LateUpdate_GameObject(const _float& fTimeDelta ) 
{
	Select_Button(fTimeDelta);

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}


void CUi_Button::Render_GameObject()
{

	
	switch (m_eType)
	{
	case START_BUTTON:
	{
		m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

		if (m_pSpriteCom != nullptr)
		{
			m_pSpriteCom->Render_Sprite(m_vPos);
			
		}
	}
	break;

		
	}

}


void CUi_Button::Select_Button(float _fTimeDelta)
{



}

void CUi_Button::Free()
{
}
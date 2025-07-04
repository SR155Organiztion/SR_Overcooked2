#include "pch.h"
#include "CUi_Button.h"
#include "CUi_Icon.h"
#include "CUi_MainMenu.h"

//Engine
#include "Engine_Define.h"
#include "CRenderer.h"
#include "CSprite.h"
#include "CDInputMgr.h"


CUi_Button::CUi_Button(LPDIRECT3DDEVICE9 pGraphicDev):CUi(pGraphicDev),
m_pTexBtn(nullptr), m_pTexLockImage(nullptr),m_pTexScroll(nullptr), m_pTexScrollStart(nullptr), m_pTexOkBox(nullptr), m_pTexNoBox(nullptr),
  m_iNonAlpha(255), m_iKeyPress(0)
{
	memset(&m_iAlpha, 255, sizeof(int[5]));
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
	case STORY_BUTTON:
	{	
		///위치 조정
		m_vPos = D3DXVECTOR3(60, 100, 0);

		m_eType = _cValue;
	}
	break;


	case ARCADE_BUTTON:
	{
		///위치 조정
		m_vPos = D3DXVECTOR3(1110, 100, 0);

		m_eType = _cValue;
	}
	break;

	case BATTLE_BUTTON:
	{
		///위치 조정
		m_vPos = D3DXVECTOR3(2160, 100, 0);

		m_eType = _cValue;
	}
	break;

	case CHEF_BUTTON:
	{
		///위치 조정
		m_vPos = D3DXVECTOR3(3210, 100, 0);

		m_eType = _cValue;
	}
	break;
	
	case STATE_BUTTON:
	{
		///위치 조정
		m_vPos = D3DXVECTOR3(4260, 100, 0);

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
		KeyInput();
		Select_Button(fTimeDelta);

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}


void CUi_Button::Render_GameObject()
{

	
	switch (m_eType)
	{
	case STORY_BUTTON:
	{
		m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

		if (m_pSpriteCom != nullptr)
		{
		
			m_pSpriteCom->Render_Sprite(m_vPos, 1, m_iNonAlpha);
			m_pSpriteCom->Render_Sprite(m_vPos, 0, m_iAlpha[0]);
			

		}
	}
	break;

	case ARCADE_BUTTON:
	{
		m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

		if (m_pSpriteCom != nullptr)
		{
			m_pSpriteCom->Render_Sprite(m_vPos, 4, m_iNonAlpha);
			m_pSpriteCom->Render_Sprite(m_vPos, 3, m_iAlpha[1]);
		}
	}
	break;

	case BATTLE_BUTTON:
	{
		m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

		if (m_pSpriteCom != nullptr)
		{
			m_pSpriteCom->Render_Sprite(m_vPos, 7, m_iNonAlpha);
			m_pSpriteCom->Render_Sprite(m_vPos, 6, m_iAlpha[2]);
		}
	}
	break;

	case CHEF_BUTTON:
	{
		m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

		if (m_pSpriteCom != nullptr)
		{
			m_pSpriteCom->Render_Sprite(m_vPos, 10, m_iNonAlpha);
			m_pSpriteCom->Render_Sprite(m_vPos, 9, m_iAlpha[3]);
		}
	}
	break;

	case STATE_BUTTON:
	{
		m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

		if (m_pSpriteCom != nullptr)
		{
			m_pSpriteCom->Render_Sprite(m_vPos, 13, m_iNonAlpha);
			m_pSpriteCom->Render_Sprite(m_vPos, 12, m_iAlpha[4]);
		}
	}
	break;
		
	}

}


void CUi_Button::Select_Button(float _fTimeDelta)
{

	if (m_iKeyPress < STORY_BUTTON)
	{
		m_iKeyPress += 1;
	}

	if (m_iKeyPress == STORY_BUTTON)
	{
		m_iAlpha[0] = 0;
		m_iAlpha[1] = 255;
		m_iAlpha[2] = 255;
		m_iAlpha[3] = 255;
		m_iAlpha[4] = 255;
	}

	if (m_iKeyPress == ARCADE_BUTTON)
	{
		m_iAlpha[0] = 255;
		m_iAlpha[1] = 0;
		m_iAlpha[2] = 255;
		m_iAlpha[3] = 255;
		m_iAlpha[4] = 255;
	}
		
	if (m_iKeyPress == BATTLE_BUTTON)
	{
		m_iAlpha[0] = 255;
		m_iAlpha[1] = 255;
		m_iAlpha[2] = 0;
		m_iAlpha[3] = 255;
		m_iAlpha[4] = 255;
	}

	if (m_iKeyPress == CHEF_BUTTON)
	{
		m_iAlpha[0] = 255;
		m_iAlpha[1] = 255;
		m_iAlpha[2] = 255;
		m_iAlpha[3] = 0;
		m_iAlpha[4] = 255;

	}

	if (m_iKeyPress == STATE_BUTTON)
	{
		m_iAlpha[0] = 255;
		m_iAlpha[1] = 255;
		m_iAlpha[2] = 255;
		m_iAlpha[3] = 255;
		m_iAlpha[4] = 0;
	}
	
	if (m_iKeyPress > STATE_BUTTON)
	{
		m_iKeyPress -= 1;
	}
	
}

void CUi_Button::KeyInput()
{

	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_LEFT) & 0x80) { m_iKeyPress -= 1; }
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_RIGHT) & 0x80) { m_iKeyPress += 1; }	
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_UP) & 0x80) { m_iKeyPress += 1; }	
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_DOWN) & 0x80) { m_iKeyPress -= 1; }
	
	
}

void CUi_Button::Free()
{
}
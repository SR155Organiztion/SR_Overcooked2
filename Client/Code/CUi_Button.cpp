#include "pch.h"
#include "CUi_Button.h"
#include "CUi_Icon.h"
#include "CUi_MainMenu.h"

//Engine
#include "Engine_Define.h"
#include "CRenderer.h"
#include "CSprite.h"
#include "CDInputMgr.h"
#include "CProtoMgr.h"

//스태틱 정의
int CUi_Button::m_iKeyPress = 0;
DWORD64 CUi_Button::m_dwlastInputTime = 0;

CUi_Button::CUi_Button(LPDIRECT3DDEVICE9 pGraphicDev):CUi(pGraphicDev),
m_pTexBtn(nullptr), m_pTexLockImage(nullptr),m_pTexScroll(nullptr), m_pTexScrollStart(nullptr), m_pTexOkBox(nullptr), m_pTexNoBox(nullptr)
{
	memset(&m_iAlpha, 255, sizeof(int[5]));
}

CUi_Button::CUi_Button(const CGameObject& rhs) :CUi(rhs)
{
	memset(&m_iAlpha, 255, sizeof(int[5]));

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
			m_tXScale = 0.15f;
			m_tYScale = 0.15f;
			m_eButtonType = _cValue;
		}
		break;


		case ARCADE_BUTTON:
		{
			///위치 조정
			m_vPos = D3DXVECTOR3(1110, 100, 0);
			m_tXScale = 0.15f;
			m_tYScale = 0.15f;
			m_eButtonType = _cValue;
		}
		break;

		case BATTLE_BUTTON:
		{
			///위치 조정
			m_vPos = D3DXVECTOR3(2160, 100, 0);
			m_tXScale = 0.15f;
			m_tYScale = 0.15f;
			m_eButtonType = _cValue;
		}
		break;

		case CHEF_BUTTON:
		{
			///위치 조정
			m_vPos = D3DXVECTOR3(3210, 100, 0);
			m_tXScale = 0.15f;
			m_tYScale = 0.15f;
			m_eButtonType = _cValue;
		}
		break;
	
		case STATE_BUTTON:
		{
			///위치 조정
			m_vPos = D3DXVECTOR3(4260, 100, 0); //4260
			m_tXScale = 0.15f;
			m_tYScale = 0.15f;
			m_eButtonType = _cValue;
		}
		break;

	}
	return S_OK;
}



_int CUi_Button::Update_GameObject(const _float& fTimeDelta) 
{
	_uint iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);
	KeyInput(m_iKeyPress);
	Select_Button(fTimeDelta, m_iKeyPress, STORY_BUTTON);
	return iExit;
}


void CUi_Button::LateUpdate_GameObject(const _float& fTimeDelta ) 
{

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}


void CUi_Button::Render_GameObject()
{

	RECT* SrcRect = nullptr;

	switch (m_eButtonType)
	{

		case STORY_BUTTON:
		{
			m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

			if (m_pSpriteCom != nullptr)
			{
		
				m_pSpriteCom->Render_SpriteAlpha(m_tXScale, m_tYScale, SrcRect, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/Button/MainButton1.png", m_iNonAlpha);
				m_pSpriteCom->Render_SpriteAlpha(m_tXScale, m_tYScale, SrcRect, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/Button/MainButton0.png", m_iAlpha[0]);
			

			}
		}
		break;

		case ARCADE_BUTTON:
		{
			m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

			if (m_pSpriteCom != nullptr)
			{
				m_pSpriteCom->Render_SpriteAlpha(m_tXScale, m_tYScale, SrcRect, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/Button/MainButton4.png", m_iNonAlpha);
				m_pSpriteCom->Render_SpriteAlpha(m_tXScale, m_tYScale, SrcRect, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/Button/MainButton3.png", m_iAlpha[1]);
			}
		}
		break;

		case BATTLE_BUTTON:
		{
			m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

			if (m_pSpriteCom != nullptr)
			{
				m_pSpriteCom->Render_SpriteAlpha(m_tXScale, m_tYScale, SrcRect, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/Button/MainButton7.png", m_iNonAlpha);
				m_pSpriteCom->Render_SpriteAlpha(m_tXScale, m_tYScale, SrcRect, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/Button/MainButton6.png", m_iAlpha[2]);
			}
		}
		break;

		case CHEF_BUTTON:
		{
			m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

			if (m_pSpriteCom != nullptr)
			{
				m_pSpriteCom->Render_SpriteAlpha(m_tXScale, m_tYScale, SrcRect, m_pCenter,  m_vPos, L"../Bin/Resource/Texture/UI/Button/MainButton10.png", m_iNonAlpha);
				m_pSpriteCom->Render_SpriteAlpha(m_tXScale, m_tYScale, SrcRect, m_pCenter,  m_vPos, L"../Bin/Resource/Texture/UI/Button/MainButton9.png", m_iAlpha[3]);
			}
		}
		break;

		case STATE_BUTTON:
		{
			m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

			if (m_pSpriteCom != nullptr)
			{
				m_pSpriteCom->Render_SpriteAlpha(m_tXScale, m_tYScale, SrcRect, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/Button/MainButton13.png", m_iNonAlpha);
				m_pSpriteCom->Render_SpriteAlpha(m_tXScale, m_tYScale, SrcRect, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/Button/MainButton12.png", m_iAlpha[4]);
			}
		}
		break;
		
	}

}


void CUi_Button::Select_Button(float _fTimeDelta, int _m_iKeyPress, BUTTON_TYPE _type)
{
	m_iKeyPress = _m_iKeyPress;

	if (m_iKeyPress < STORY_BUTTON)
	{
		m_iKeyPress = 4;
	}

	if (m_iKeyPress == STORY_BUTTON)
	{
		m_iAlpha[0] = 0;
		m_iAlpha[1] = 255;
		m_iAlpha[2] = 255;
		m_iAlpha[3] = 255;
		m_iAlpha[4] = 255;
		//if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_SPACE) & 0x80)
		//{
		//	//누르면 클릭 이벤트 발생
		//}
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
		m_iKeyPress = 0;
	}
	
}

void CUi_Button::KeyInput(int _m_iKeyPress)
{
	m_iKeyPress = _m_iKeyPress;
	DWORD64 now = GetTickCount64();
	if		((CDInputMgr::GetInstance()->Get_DIKeyState(DIK_W) & 0x80) && (now - m_dwlastInputTime > 200)) { m_iKeyPress += 1; m_dwlastInputTime = now;}
	else if ((CDInputMgr::GetInstance()->Get_DIKeyState(DIK_A) & 0x80) && (now - m_dwlastInputTime > 200)) { m_iKeyPress -= 1; m_dwlastInputTime = now;}
	else if ((CDInputMgr::GetInstance()->Get_DIKeyState(DIK_S) & 0x80) && (now - m_dwlastInputTime > 200)) { m_iKeyPress -= 1; m_dwlastInputTime = now;}
	else if ((CDInputMgr::GetInstance()->Get_DIKeyState(DIK_D) & 0x80) && (now - m_dwlastInputTime > 200)) { m_iKeyPress += 1; m_dwlastInputTime = now;}
	if (m_iKeyPress < 0)
		m_iKeyPress = 4;
	if (m_iKeyPress > 4)
		m_iKeyPress = 0;
	
}


HRESULT CUi_Button::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;
	pComponent = m_pSpriteCom = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Button"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite", pComponent });

	return S_OK;
}

void CUi_Button::Free()
{
}
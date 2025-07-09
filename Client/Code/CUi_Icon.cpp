#include "pch.h"
#include "CUi_Icon.h"

CUi_Icon::CUi_Icon(LPDIRECT3DDEVICE9 pGraphicDev):CUi(pGraphicDev), m_pTexIcon(nullptr), m_szName(nullptr), m_pPosition(0,0,0)
{
}

CUi_Icon::CUi_Icon(const CGameObject& rhs):CUi(rhs), m_pTexIcon(nullptr), m_szName(nullptr), m_pPosition(0,0,0)
{
}

CUi_Icon::~CUi_Icon()
{
}

HRESULT CUi_Icon::Ready_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

int CUi_Icon::Update_GameObject(const _float& _fTimeDelta)
{
	_uint iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

	return iExit;
}

void CUi_Icon::LateUpdate_GameObject()
{
}

void CUi_Icon::Render_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev)
{
	switch (m_eType)
	{
	case 1: 
	{
		m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/in_game/Icon0.png");
	}
	default:
		break;
	}
	
	m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/in_game/Icon1.png");
	m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/in_game/Icon2.png");
	m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/in_game/Icon3.png");
	m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/in_game/Icon4.png");
	m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/in_game/Icon5.png");
	m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/in_game/Icon6.png");
	m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/in_game/Icon7.png");

}

HRESULT CUi_Icon::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pSpriteCom = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Icon"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite", pComponent });
	return S_OK;
}

void CUi_Icon::Free()
{
}